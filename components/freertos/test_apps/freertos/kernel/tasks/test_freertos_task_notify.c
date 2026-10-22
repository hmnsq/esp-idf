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

#define NO_OF_NOTIFS    4
#define NO_OF_TASKS     2       //Sender and receiver
#define MESSAGE         0xFF

ایستا uint32_t send_core_message = 0;
ایستا TaskHandle_t recv_task_handle;
ایستا bool isr_give = false;
ایستا bool test_start = false;
ایستا gptimer_handle_t gptimers[CONFIG_FREERTOS_عدد_OF_CORES];
ایستا SemaphoreHandle_t trigger_send_semphr;
ایستا SemaphoreHandle_t task_delete_semphr;

//Test tracking vars
ایستا فرار uint32_t notifs_sent = 0;
ایستا فرار uint32_t notifs_rec = 0;
ایستا bool wrong_core = false;

ایستا bool on_alarm_sender_cb(gptimer_handle_t timer, ثابت gptimer_alarm_event_data_t *edata, تهی *user_ctx)
{
    gptimer_stop(timer);
    اگر (!test_start) {
        بازگشت false;
    }
    صحیح curcore = xPortGetCoreID();
    اگر (isr_give) { //Test vTaskNotifyGiveFromISR() on same core
        notifs_sent++;
        vTaskNotifyGiveFromISR(recv_task_handle, پوچ);
    } وگرنه { //Test xTaskNotifyFromISR()
        notifs_sent++;
        xTaskNotifyFromISR(recv_task_handle, (MESSAGE << curcore), e_Set_مقدار_With_Overwrite, پوچ);
    }
    // always trigger a task switch when exit ISR context
    بازگشت true;
}

ایستا تهی test_gptimer_start(تهی *arg)
{
    gptimer_handle_t gptimer = (gptimer_handle_t)arg;
    gptimer_alarm_config_t alarm_config = {
        .reload_شمار = 0,
        .alarm_شمار = 1000,
    };
    TEST_ESP_OK(gptimer_set_raw_شمار(gptimer, 0));
    TEST_ESP_OK(gptimer_set_alarm_action(gptimer, &alarm_config));
    TEST_ESP_OK(gptimer_start(gptimer));
}

ایستا تهی sender_task(تهی *arg)
{
    gptimer_handle_t gptimer = (gptimer_handle_t)arg;
    صحیح curcore = xPortGetCoreID();

    //Test xTaskNotify
    xSemaphoreTake(trigger_send_semphr, portMAX_DELAY);
    notifs_sent++;
    xTaskNotify(recv_task_handle, (MESSAGE << curcore), e_Set_مقدار_With_Overwrite);

    //Test xTaskNotifyGive
    xSemaphoreTake(trigger_send_semphr, portMAX_DELAY);
    notifs_sent++;
    xTaskNotifyGive(recv_task_handle);

    //Test xTaskNotifyFromISR
    xSemaphoreTake(trigger_send_semphr, portMAX_DELAY);
    isr_give = false;
    test_gptimer_start(gptimer);

    //Test vTaskNotifyGiveFromISR
    xSemaphoreTake(trigger_send_semphr, portMAX_DELAY);
    isr_give = true;
    test_gptimer_start(gptimer);

    //Delete Task and Semaphores
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(پوچ);
}

ایستا تهی receiver_task(تهی *arg)
{
    uint32_t notify_مقدار;

    //Test xTaskNotifyWait from task
    xTaskNotifyWait(0, 0xFFFFFFFF, &notify_مقدار, portMAX_DELAY);
    اگر (notify_مقدار != send_core_message) {
        wrong_core = true;
    }
    notifs_rec++;

    //Test ulTaskNotifyTake from task
    xSemaphoreGive(trigger_send_semphr);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    notifs_rec++;

    //Test xTaskNotifyWait from ISR
    xSemaphoreGive(trigger_send_semphr);
    xTaskNotifyWait(0, 0xFFFFFFFF, &notify_مقدار, portMAX_DELAY);
    اگر (notify_مقدار != send_core_message) {
        wrong_core = true;
    }
    notifs_rec++;

    //Test ulTaskNotifyTake from ISR
    xSemaphoreGive(trigger_send_semphr);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    notifs_rec++;

    //Test complete, stop timer and delete task
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(پوچ);
}

ایستا تهی install_gptimer_on_core(تهی *arg)
{
    صحیح core_id = (صحیح)arg;
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_شمار_UP,
        .resolution_hz = 1000000, // 1MHz, 1 tick = 1us
    };
    TEST_ESP_OK(gptimer_new_timer(&timer_config, &gptimers[core_id]));
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_alarm_sender_cb,
    };
    TEST_ESP_OK(gptimer_register_event_callbacks(gptimers[core_id], &cbs, پوچ));
    TEST_ESP_OK(gptimer_enable(gptimers[core_id]));
    test_gptimer_start(gptimers[core_id]);
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(پوچ);
}

TEST_CASE("Test Task_Notify", "[freertos]")
{
    test_start = false;
    trigger_send_semphr = x_Semaphore_Create_دودویی();
    task_delete_semphr = xQueueCreateCountingSemaphore(10, 0);
    برای (صحیح i = 0; i < CONFIG_FREERTOS_عدد_OF_CORES; i++) {
        xTaskCreatePinnedToCore(install_gptimer_on_core, "install_gptimer", 4096, (تهی *ثابت)i, UNITY_FREERTOS_PRIORITY + 1, پوچ, i);
        TEST_ASSERT(xSemaphoreTake(task_delete_semphr, pdMS_TO_TICKS(1000)));
    }
    // wait the gptimer installation done on specific core
    vTaskDelay(10);
    // test start
    test_start = true;
    برای (صحیح i = 0; i < CONFIG_FREERTOS_عدد_OF_CORES; i++) { //Sending Core
        برای (صحیح j = 0; j < CONFIG_FREERTOS_عدد_OF_CORES; j++) { //Receiving Core
            //Reset Values
            notifs_sent = 0;
            notifs_rec = 0;
            wrong_core = false;
            send_core_message = (0xFF << i); //0xFF if core 0, 0xFF0 if core 1
            // receiver task has higher priority than sender task
            xTaskCreatePinnedToCore(receiver_task, "recv task", 1000, پوچ, UNITY_FREERTOS_PRIORITY + 2, &recv_task_handle, j);
            xTaskCreatePinnedToCore(sender_task, "send task", 1000, gptimers[i], UNITY_FREERTOS_PRIORITY + 1, پوچ, i);
            vTaskDelay(5);      //Wait for task creation to complete

            xSemaphoreGive(trigger_send_semphr);    //Trigger sender task
            برای (صحیح k = 0; k < NO_OF_TASKS; k++) { //Wait for sender and receiver task deletion
                TEST_ASSERT(xSemaphoreTake(task_delete_semphr, pdMS_TO_TICKS(2000)));
            }
            vTaskDelay(5);      //Give time tasks to delete

            TEST_ASSERT_EQUAL(NO_OF_NOTIFS, notifs_sent);
            TEST_ASSERT_EQUAL(NO_OF_NOTIFS, notifs_rec);
            TEST_ASSERT_EQUAL(false, wrong_core);
        }
    }

    //Delete Semaphroes and timer ISRs
    vSemaphoreDelete(trigger_send_semphr);
    vSemaphoreDelete(task_delete_semphr);
    برای (صحیح i = 0; i < CONFIG_FREERTOS_عدد_OF_CORES; i++) {
        TEST_ESP_OK(gptimer_disable(gptimers[i]));
        TEST_ESP_OK(gptimer_del_timer(gptimers[i]));
    }
}
#endif //SOC_GPTIMER_SUPPORTED

/* Test causes asserts, so it cannot be run as a normal unity test case.
   Test case is ran as a separate test case in test_task_notify_too_high_index_fails
 */
TEST_CASE("Notify too high index fails", "[ignore]")
{
    uint32_t notification_مقدار = 47;
    xTaskNotifyIndexed(xTaskGetCurrentTaskHandle(), 2, notification_مقدار, eNoAction);
}

/* Test causes asserts, so it cannot be run as a normal unity test case.
   Test case is ran as a separate test case in test_task_notify_wait_too_high_index_fails
 */
TEST_CASE("Notify Wait too high index fails", "[ignore]")
{
    uint32_t notification_مقدار;
    xTaskNotifyWaitIndexed(2, 0, 0, &notification_مقدار, pdMS_TO_TICKS(10));
}

#اگر CONFIG_FREERTOS_TASK_NOTIFICATION_ARRAY_ENTRIES > 1
ثابت uint32_t NOTIFICATION_مقدار_0 = 47;
ثابت uint32_t NOTIFICATION_مقدار_1 = 48;

تهی notify(تهی *arg)
{
    TaskHandle_t main_task = (TaskHandle_t) arg;
    xTaskNotifyIndexed(main_task, 0, NOTIFICATION_مقدار_0, e_Set_مقدار_With_Overwrite);
    xTaskNotifyIndexed(main_task, 1, NOTIFICATION_مقدار_1, e_Set_مقدار_With_Overwrite);
    vTaskDelete(پوچ);
}
ایستا TaskHandle_t notificator_task;

TEST_CASE("Notify to different indexes works", "[freertos]")
{
    uint32_t notification_مقدار_0 = 0;
    uint32_t notification_مقدار_1 = 0;

    TaskHandle_t main_task = xTaskGetCurrentTaskHandle();
    xTaskCreate(notify, "notificator", 2048, main_task, 2, &notificator_task);

    xTaskNotifyWaitIndexed(0, 0, 0xFFFFFFFF, &notification_مقدار_0, pdMS_TO_TICKS(10));
    xTaskNotifyWaitIndexed(1, 0, 0xFFFFFFFF, &notification_مقدار_1, pdMS_TO_TICKS(10));

    TEST_ASSERT_EQUAL(notification_مقدار_0, NOTIFICATION_مقدار_0);
    TEST_ASSERT_EQUAL(notification_مقدار_1, NOTIFICATION_مقدار_1);
}
#endif
