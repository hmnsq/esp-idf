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
#شامل "sdkconfig.h"
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل "freertos/semphr.h"
#شامل "driver/gptimer.h"
#شامل "unity.h"
#شامل "test_utils.h"

#اگر SOC_GPTIMER_SUPPORTED

#تعریف_کرو NO_کا_NOTIFS    4
#تعریف_کرو NO_کا_TASKS     2       //Sender and receiver
#تعریف_کرو پیغام_2         0xFF

ساکن uint32_t send_core_پیغام = 0;
ساکن TaskHandle_t recv_task_handle;
ساکن bool isr_give = false;
ساکن bool test_آغاز = false;
ساکن gptimer_handle_t gptimers[CONFIG_FREERTOS_نمبر_کا_CORES];
ساکن SemaphoreHandle_t trigger_send_semphr;
ساکن SemaphoreHandle_t task_delete_semphr;

//Test tracking vars
ساکن متلون uint32_t notifs_بھیجا = 0;
ساکن متلون uint32_t notifs_rec = 0;
ساکن bool wrong_core = false;

ساکن bool on_alarm_sender_cb(gptimer_handle_t timer, ثابت gptimer_alarm_event_ڈیٹا_t *edata, خالی *user_ctx)
{
    gptimer_stop(timer);
    اگر (!test_آغاز) {
        واپس false;
    }
    صحیح curcore = x_پورٹ_Get_Core_ID();
    اگر (isr_give) { //Test vTaskNotifyGiveFromISR() on same core
        notifs_بھیجا++;
        vTaskNotifyGiveFromISR(recv_task_handle, تہی);
    } ورنہ { //Test xTaskNotifyFromISR()
        notifs_بھیجا++;
        xTaskNotifyFromISR(recv_task_handle, (پیغام_2 << curcore), e_سیٹ_قدر_With_Overwrite, تہی);
    }
    // always trigger a task switch when exit ISR context
    واپس true;
}

ساکن خالی test_gptimer_آغاز(خالی *arg)
{
    gptimer_handle_t gptimer = (gptimer_handle_t)arg;
    gptimer_alarm_config_t alarm_config = {
        .reload_گنتی = 0,
        .alarm_گنتی = 1000,
    };
    TEST_ESP_OK(gptimer_سیٹ_raw_گنتی(gptimer, 0));
    TEST_ESP_OK(gptimer_سیٹ_alarm_action(gptimer, &alarm_config));
    TEST_ESP_OK(gptimer_آغاز(gptimer));
}

ساکن خالی sender_task(خالی *arg)
{
    gptimer_handle_t gptimer = (gptimer_handle_t)arg;
    صحیح curcore = x_پورٹ_Get_Core_ID();

    //Test xTaskNotify
    xSemaphoreTake(trigger_send_semphr, پورٹ_MAX_DELAY);
    notifs_بھیجا++;
    xTaskNotify(recv_task_handle, (پیغام_2 << curcore), e_سیٹ_قدر_With_Overwrite);

    //Test xTaskNotifyGive
    xSemaphoreTake(trigger_send_semphr, پورٹ_MAX_DELAY);
    notifs_بھیجا++;
    xTaskNotifyGive(recv_task_handle);

    //Test xTaskNotifyFromISR
    xSemaphoreTake(trigger_send_semphr, پورٹ_MAX_DELAY);
    isr_give = false;
    test_gptimer_آغاز(gptimer);

    //Test vTaskNotifyGiveFromISR
    xSemaphoreTake(trigger_send_semphr, پورٹ_MAX_DELAY);
    isr_give = true;
    test_gptimer_آغاز(gptimer);

    //Delete Task and Semaphores
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(تہی);
}

ساکن خالی receiver_task(خالی *arg)
{
    uint32_t notify_قدر;

    //Test xTaskNotifyWait from task
    xTaskNotifyWait(0, 0xFFFFFFFF, &notify_قدر, پورٹ_MAX_DELAY);
    اگر (notify_قدر != send_core_پیغام) {
        wrong_core = true;
    }
    notifs_rec++;

    //Test ulTaskNotifyTake from task
    xSemaphoreGive(trigger_send_semphr);
    ulTaskNotifyTake(pdTRUE, پورٹ_MAX_DELAY);
    notifs_rec++;

    //Test xTaskNotifyWait from ISR
    xSemaphoreGive(trigger_send_semphr);
    xTaskNotifyWait(0, 0xFFFFFFFF, &notify_قدر, پورٹ_MAX_DELAY);
    اگر (notify_قدر != send_core_پیغام) {
        wrong_core = true;
    }
    notifs_rec++;

    //Test ulTaskNotifyTake from ISR
    xSemaphoreGive(trigger_send_semphr);
    ulTaskNotifyTake(pdTRUE, پورٹ_MAX_DELAY);
    notifs_rec++;

    //Test complete, stop timer and delete task
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(تہی);
}

ساکن خالی install_gptimer_on_core(خالی *arg)
{
    صحیح core_id = (صحیح)arg;
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_گنتی_اوپر,
        .resolution_hz = 1000000, // 1MHz, 1 tick = 1us
    };
    TEST_ESP_OK(gptimer_new_timer(&timer_config, &gptimers[core_id]));
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_alarm_sender_cb,
    };
    TEST_ESP_OK(gptimer_رجسٹر_event_callbacks(gptimers[core_id], &cbs, تہی));
    TEST_ESP_OK(gptimer_enable(gptimers[core_id]));
    test_gptimer_آغاز(gptimers[core_id]);
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(تہی);
}

TEST_CASE("Test Task_Notify", "[freertos]")
{
    test_آغاز = false;
    trigger_send_semphr = x_Semaphore_Create_ثنائی();
    task_delete_semphr = x_قطار_Create_Counting_Semaphore(10, 0);
    برائے (صحیح i = 0; i < CONFIG_FREERTOS_نمبر_کا_CORES; i++) {
        x_Task_Create_Pinned_مقصد_Core(install_gptimer_on_core, "install_gptimer", 4096, (خالی *ثابت)i, UNITY_FREERTOS_ترجیح + 1, تہی, i);
        TEST_ASSERT(xSemaphoreTake(task_delete_semphr, pd_MS_مقصد_TICKS(1000)));
    }
    // wait the gptimer installation done on specific core
    vTaskDelay(10);
    // test start
    test_آغاز = true;
    برائے (صحیح i = 0; i < CONFIG_FREERTOS_نمبر_کا_CORES; i++) { //Sending Core
        برائے (صحیح j = 0; j < CONFIG_FREERTOS_نمبر_کا_CORES; j++) { //Receiving Core
            //Reset Values
            notifs_بھیجا = 0;
            notifs_rec = 0;
            wrong_core = false;
            send_core_پیغام = (0xFF << i); //0xFF if core 0, 0xFF0 if core 1
            // receiver task has higher priority than sender task
            x_Task_Create_Pinned_مقصد_Core(receiver_task, "recv task", 1000, تہی, UNITY_FREERTOS_ترجیح + 2, &recv_task_handle, j);
            x_Task_Create_Pinned_مقصد_Core(sender_task, "send task", 1000, gptimers[i], UNITY_FREERTOS_ترجیح + 1, تہی, i);
            vTaskDelay(5);      //Wait for task creation to complete

            xSemaphoreGive(trigger_send_semphr);    //Trigger sender task
            برائے (صحیح k = 0; k < NO_کا_TASKS; k++) { //Wait for sender and receiver task deletion
                TEST_ASSERT(xSemaphoreTake(task_delete_semphr, pd_MS_مقصد_TICKS(2000)));
            }
            vTaskDelay(5);      //Give time tasks to delete

            TEST_ASSERT_EQUAL(NO_کا_NOTIFS, notifs_بھیجا);
            TEST_ASSERT_EQUAL(NO_کا_NOTIFS, notifs_rec);
            TEST_ASSERT_EQUAL(false, wrong_core);
        }
    }

    //Delete Semaphroes and timer ISRs
    vSemaphoreDelete(trigger_send_semphr);
    vSemaphoreDelete(task_delete_semphr);
    برائے (صحیح i = 0; i < CONFIG_FREERTOS_نمبر_کا_CORES; i++) {
        TEST_ESP_OK(gptimer_disable(gptimers[i]));
        TEST_ESP_OK(gptimer_del_timer(gptimers[i]));
    }
}
#اگر_ختم //SOC_GPTIMER_SUPPORTED

/* Test causes asserts, so it cannot be run as a normal unity test case.
   Test case is ran as a separate test case in test_task_notify_too_high_index_fails
 */
TEST_CASE("Notify too high index fails", "[ignore]")
{
    uint32_t notification_قدر = 47;
    x_Task_Notify_اشاریہ_دار(xTaskGetCurrentTaskHandle(), 2, notification_قدر, eNoAction);
}

/* Test causes asserts, so it cannot be run as a normal unity test case.
   Test case is ran as a separate test case in test_task_notify_wait_too_high_index_fails
 */
TEST_CASE("Notify Wait too high index fails", "[ignore]")
{
    uint32_t notification_قدر;
    x_Task_Notify_Wait_اشاریہ_دار(2, 0, 0, &notification_قدر, pd_MS_مقصد_TICKS(10));
}

#اگر CONFIG_FREERTOS_TASK_NOTIFICATION_ARRAY_ENTRIES > 1
ثابت uint32_t NOTIFICATION_قدر_0 = 47;
ثابت uint32_t NOTIFICATION_قدر_1 = 48;

خالی notify(خالی *arg)
{
    TaskHandle_t main_task = (TaskHandle_t) arg;
    x_Task_Notify_اشاریہ_دار(main_task, 0, NOTIFICATION_قدر_0, e_سیٹ_قدر_With_Overwrite);
    x_Task_Notify_اشاریہ_دار(main_task, 1, NOTIFICATION_قدر_1, e_سیٹ_قدر_With_Overwrite);
    vTaskDelete(تہی);
}
ساکن TaskHandle_t notificator_task;

TEST_CASE("Notify to different indexes works", "[freertos]")
{
    uint32_t notification_قدر_0 = 0;
    uint32_t notification_قدر_1 = 0;

    TaskHandle_t main_task = xTaskGetCurrentTaskHandle();
    xTaskCreate(notify, "notificator", 2048, main_task, 2, &notificator_task);

    x_Task_Notify_Wait_اشاریہ_دار(0, 0, 0xFFFFFFFF, &notification_قدر_0, pd_MS_مقصد_TICKS(10));
    x_Task_Notify_Wait_اشاریہ_دار(1, 0, 0xFFFFFFFF, &notification_قدر_1, pd_MS_مقصد_TICKS(10));

    TEST_ASSERT_EQUAL(notification_قدر_0, NOTIFICATION_قدر_0);
    TEST_ASSERT_EQUAL(notification_قدر_1, NOTIFICATION_قدر_1);
}
#اگر_ختم
