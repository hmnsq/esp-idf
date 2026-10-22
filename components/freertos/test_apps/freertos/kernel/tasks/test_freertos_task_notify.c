/*
 * SPDX-FileCopyrightText: 2022-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Test of FreeRTOS task notifications. This test creates a sender and receiver
 task under different core permutations. For each permutation, the sender task
 will test the xTaskNotify(), xTaskNotifyGive(), xTaskNotifyFromISR(), and
 vTaskNotifyGiveFromISR(), whereas the receiver task will test
 xTaskNotifyWait() and ulTaskNotifyTake().
*/
#ضمن "sdkconfig.h"
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "freertos/semphr.h"
#ضمن "driver/gptimer.h"
#ضمن "unity.h"
#ضمن "test_utils.h"

#إذا SOC_GPTIMER_SUPPORTED

#وصف_ڪريو NO_جو_NOTIFS    4
#وصف_ڪريو NO_جو_TASKS     2       //Sender and receiver
#وصف_ڪريو پيغام         0xFF

ساكن uint32_t send_core_پيغام = 0;
ساكن TaskHandle_t recv_task_handle;
ساكن bool isr_give = false;
ساكن bool test_شروعات = false;
ساكن gptimer_handle_t gptimers[CONFIG_FREERTOS_عدد_جو_CORES];
ساكن SemaphoreHandle_t trigger_send_semphr;
ساكن SemaphoreHandle_t task_delete_semphr;

//Test tracking vars
ساكن متطاير uint32_t notifs_موڪليل = 0;
ساكن متطاير uint32_t notifs_rec = 0;
ساكن bool wrong_core = false;

ساكن bool on_alarm_sender_cb(gptimer_handle_t timer, ثابت gptimer_alarm_event_معلومات_t *edata, فراغ *user_ctx)
{
    gptimer_stop(timer);
    إذا (!test_شروعات) {
        أرجع false;
    }
    صحيح curcore = x_پورٽ_Get_Core_ID();
    إذا (isr_give) { //Test vTaskNotifyGiveFromISR() on same core
        notifs_موڪليل++;
        vTaskNotifyGiveFromISR(recv_task_handle, عدم);
    } وإلا { //Test xTaskNotifyFromISR()
        notifs_موڪليل++;
        xTaskNotifyFromISR(recv_task_handle, (پيغام << curcore), e_سيٽ_قدر_With_Overwrite, عدم);
    }
    // always trigger a task switch when exit ISR context
    أرجع true;
}

ساكن فراغ test_gptimer_شروعات(فراغ *arg)
{
    gptimer_handle_t gptimer = (gptimer_handle_t)arg;
    gptimer_alarm_config_t alarm_config = {
        .reload_ڳڻپ = 0,
        .alarm_ڳڻپ = 1000,
    };
    TEST_ESP_OK(gptimer_سيٽ_raw_ڳڻپ(gptimer, 0));
    TEST_ESP_OK(gptimer_سيٽ_alarm_action(gptimer, &alarm_config));
    TEST_ESP_OK(gptimer_شروعات(gptimer));
}

ساكن فراغ sender_task(فراغ *arg)
{
    gptimer_handle_t gptimer = (gptimer_handle_t)arg;
    صحيح curcore = x_پورٽ_Get_Core_ID();

    //Test xTaskNotify
    xSemaphoreTake(trigger_send_semphr, پورٽ_MAX_DELAY);
    notifs_موڪليل++;
    xTaskNotify(recv_task_handle, (پيغام << curcore), e_سيٽ_قدر_With_Overwrite);

    //Test xTaskNotifyGive
    xSemaphoreTake(trigger_send_semphr, پورٽ_MAX_DELAY);
    notifs_موڪليل++;
    xTaskNotifyGive(recv_task_handle);

    //Test xTaskNotifyFromISR
    xSemaphoreTake(trigger_send_semphr, پورٽ_MAX_DELAY);
    isr_give = false;
    test_gptimer_شروعات(gptimer);

    //Test vTaskNotifyGiveFromISR
    xSemaphoreTake(trigger_send_semphr, پورٽ_MAX_DELAY);
    isr_give = true;
    test_gptimer_شروعات(gptimer);

    //Delete Task and Semaphores
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(عدم);
}

ساكن فراغ receiver_task(فراغ *arg)
{
    uint32_t notify_قدر;

    //Test xTaskNotifyWait from task
    xTaskNotifyWait(0, 0xFFFFFFFF, &notify_قدر, پورٽ_MAX_DELAY);
    إذا (notify_قدر != send_core_پيغام) {
        wrong_core = true;
    }
    notifs_rec++;

    //Test ulTaskNotifyTake from task
    xSemaphoreGive(trigger_send_semphr);
    ulTaskNotifyTake(pdTRUE, پورٽ_MAX_DELAY);
    notifs_rec++;

    //Test xTaskNotifyWait from ISR
    xSemaphoreGive(trigger_send_semphr);
    xTaskNotifyWait(0, 0xFFFFFFFF, &notify_قدر, پورٽ_MAX_DELAY);
    إذا (notify_قدر != send_core_پيغام) {
        wrong_core = true;
    }
    notifs_rec++;

    //Test ulTaskNotifyTake from ISR
    xSemaphoreGive(trigger_send_semphr);
    ulTaskNotifyTake(pdTRUE, پورٽ_MAX_DELAY);
    notifs_rec++;

    //Test complete, stop timer and delete task
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(عدم);
}

ساكن فراغ install_gptimer_on_core(فراغ *arg)
{
    صحيح core_id = (صحيح)arg;
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_ڳڻپ_مٿي,
        .resolution_hz = 1000000, // 1MHz, 1 tick = 1us
    };
    TEST_ESP_OK(gptimer_new_timer(&timer_config, &gptimers[core_id]));
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_alarm_sender_cb,
    };
    TEST_ESP_OK(gptimer_رجسٽر_event_callbacks(gptimers[core_id], &cbs, عدم));
    TEST_ESP_OK(gptimer_enable(gptimers[core_id]));
    test_gptimer_شروعات(gptimers[core_id]);
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(عدم);
}

TEST_CASE("Test Task_Notify", "[freertos]")
{
    test_شروعات = false;
    trigger_send_semphr = x_Semaphore_Create_ٻٽو();
    task_delete_semphr = x_قطار_Create_Counting_Semaphore(10, 0);
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_عدد_جو_CORES; i++) {
        x_Task_Create_Pinned_منزل_Core(install_gptimer_on_core, "install_gptimer", 4096, (فراغ *ثابت)i, UNITY_FREERTOS_ترجيح + 1, عدم, i);
        TEST_ASSERT(xSemaphoreTake(task_delete_semphr, pd_MS_منزل_TICKS(1000)));
    }
    // wait the gptimer installation done on specific core
    vTaskDelay(10);
    // test start
    test_شروعات = true;
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_عدد_جو_CORES; i++) { //Sending Core
        لكل (صحيح j = 0; j < CONFIG_FREERTOS_عدد_جو_CORES; j++) { //Receiving Core
            //Reset Values
            notifs_موڪليل = 0;
            notifs_rec = 0;
            wrong_core = false;
            send_core_پيغام = (0xFF << i); //0xFF if core 0, 0xFF0 if core 1
            // receiver task has higher priority than sender task
            x_Task_Create_Pinned_منزل_Core(receiver_task, "recv task", 1000, عدم, UNITY_FREERTOS_ترجيح + 2, &recv_task_handle, j);
            x_Task_Create_Pinned_منزل_Core(sender_task, "send task", 1000, gptimers[i], UNITY_FREERTOS_ترجيح + 1, عدم, i);
            vTaskDelay(5);      //Wait for task creation to complete

            xSemaphoreGive(trigger_send_semphr);    //Trigger sender task
            لكل (صحيح k = 0; k < NO_جو_TASKS; k++) { //Wait for sender and receiver task deletion
                TEST_ASSERT(xSemaphoreTake(task_delete_semphr, pd_MS_منزل_TICKS(2000)));
            }
            vTaskDelay(5);      //Give time tasks to delete

            TEST_ASSERT_EQUAL(NO_جو_NOTIFS, notifs_موڪليل);
            TEST_ASSERT_EQUAL(NO_جو_NOTIFS, notifs_rec);
            TEST_ASSERT_EQUAL(false, wrong_core);
        }
    }

    //Delete Semaphroes and timer ISRs
    vSemaphoreDelete(trigger_send_semphr);
    vSemaphoreDelete(task_delete_semphr);
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_عدد_جو_CORES; i++) {
        TEST_ESP_OK(gptimer_disable(gptimers[i]));
        TEST_ESP_OK(gptimer_del_timer(gptimers[i]));
    }
}
#جيڪڏهن_پورو //SOC_GPTIMER_SUPPORTED

/* Test causes asserts, so it cannot be run as a normal unity test case.
   Test case is ran as a separate test case in test_task_notify_too_high_index_fails
 */
TEST_CASE("Notify too high index fails", "[ignore]")
{
    uint32_t notification_قدر = 47;
    x_Task_Notify_اشاريي_وارو(xTaskGetCurrentTaskHandle(), 2, notification_قدر, eNoAction);
}

/* Test causes asserts, so it cannot be run as a normal unity test case.
   Test case is ran as a separate test case in test_task_notify_wait_too_high_index_fails
 */
TEST_CASE("Notify Wait too high index fails", "[ignore]")
{
    uint32_t notification_قدر;
    x_Task_Notify_Wait_اشاريي_وارو(2, 0, 0, &notification_قدر, pd_MS_منزل_TICKS(10));
}

#إذا CONFIG_FREERTOS_TASK_NOTIFICATION_ARRAY_ENTRIES > 1
ثابت uint32_t NOTIFICATION_قدر_0 = 47;
ثابت uint32_t NOTIFICATION_قدر_1 = 48;

فراغ notify(فراغ *arg)
{
    TaskHandle_t main_task = (TaskHandle_t) arg;
    x_Task_Notify_اشاريي_وارو(main_task, 0, NOTIFICATION_قدر_0, e_سيٽ_قدر_With_Overwrite);
    x_Task_Notify_اشاريي_وارو(main_task, 1, NOTIFICATION_قدر_1, e_سيٽ_قدر_With_Overwrite);
    vTaskDelete(عدم);
}
ساكن TaskHandle_t notificator_task;

TEST_CASE("Notify to different indexes works", "[freertos]")
{
    uint32_t notification_قدر_0 = 0;
    uint32_t notification_قدر_1 = 0;

    TaskHandle_t main_task = xTaskGetCurrentTaskHandle();
    xTaskCreate(notify, "notificator", 2048, main_task, 2, &notificator_task);

    x_Task_Notify_Wait_اشاريي_وارو(0, 0, 0xFFFFFFFF, &notification_قدر_0, pd_MS_منزل_TICKS(10));
    x_Task_Notify_Wait_اشاريي_وارو(1, 0, 0xFFFFFFFF, &notification_قدر_1, pd_MS_منزل_TICKS(10));

    TEST_ASSERT_EQUAL(notification_قدر_0, NOTIFICATION_قدر_0);
    TEST_ASSERT_EQUAL(notification_قدر_1, NOTIFICATION_قدر_1);
}
#جيڪڏهن_پورو
