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
#அந்தர்பாவ "sdkconfig.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ "freertos/FreeRTOS.h"
#அந்தர்பாவ "freertos/task.h"
#அந்தர்பாவ "freertos/semphr.h"
#அந்தர்பாவ "driver/gptimer.h"
#அந்தர்பாவ "unity.h"
#அந்தர்பாவ "test_utils.h"

#எனில் SOC_GPTIMER_SUPPORTED

#define NO_OF_NOTIFS    4
#define NO_OF_TASKS     2       //Sender and receiver
#define MESSAGE         0xFF

நிலையான uint32_t send_core_message = 0;
நிலையான TaskHandle_t recv_task_handle;
நிலையான bool isr_give = false;
நிலையான bool test_start = false;
நிலையான gptimer_handle_t gptimers[CONFIG_FREERTOS_எண்_OF_CORES];
நிலையான SemaphoreHandle_t trigger_send_semphr;
நிலையான SemaphoreHandle_t task_delete_semphr;

//Test tracking vars
நிலையான நிலையற்ற uint32_t notifs_sent = 0;
நிலையான நிலையற்ற uint32_t notifs_rec = 0;
நிலையான bool wrong_core = false;

நிலையான bool on_alarm_sender_cb(gptimer_handle_t timer, மாறிலி gptimer_alarm_event_data_t *edata, வெறுமை *user_ctx)
{
    gptimer_stop(timer);
    எனில் (!test_start) {
        திரும்பு false;
    }
    முழு curcore = xPortGetCoreID();
    எனில் (isr_give) { //Test vTaskNotifyGiveFromISR() on same core
        notifs_sent++;
        vTaskNotifyGiveFromISR(recv_task_handle, வெற்று);
    } இல்லையெனில் { //Test xTaskNotifyFromISR()
        notifs_sent++;
        xTaskNotifyFromISR(recv_task_handle, (MESSAGE << curcore), e_Set_மதிப்பு_With_Overwrite, வெற்று);
    }
    // always trigger a task switch when exit ISR context
    திரும்பு true;
}

நிலையான வெறுமை test_gptimer_start(வெறுமை *arg)
{
    gptimer_handle_t gptimer = (gptimer_handle_t)arg;
    gptimer_alarm_config_t alarm_config = {
        .reload_மொத்தம் = 0,
        .alarm_மொத்தம் = 1000,
    };
    TEST_ESP_OK(gptimer_set_raw_மொத்தம்(gptimer, 0));
    TEST_ESP_OK(gptimer_set_alarm_action(gptimer, &alarm_config));
    TEST_ESP_OK(gptimer_start(gptimer));
}

நிலையான வெறுமை sender_task(வெறுமை *arg)
{
    gptimer_handle_t gptimer = (gptimer_handle_t)arg;
    முழு curcore = xPortGetCoreID();

    //Test xTaskNotify
    xSemaphoreTake(trigger_send_semphr, portMAX_DELAY);
    notifs_sent++;
    xTaskNotify(recv_task_handle, (MESSAGE << curcore), e_Set_மதிப்பு_With_Overwrite);

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
    vTaskDelete(வெற்று);
}

நிலையான வெறுமை receiver_task(வெறுமை *arg)
{
    uint32_t notify_மதிப்பு;

    //Test xTaskNotifyWait from task
    xTaskNotifyWait(0, 0xFFFFFFFF, &notify_மதிப்பு, portMAX_DELAY);
    எனில் (notify_மதிப்பு != send_core_message) {
        wrong_core = true;
    }
    notifs_rec++;

    //Test ulTaskNotifyTake from task
    xSemaphoreGive(trigger_send_semphr);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    notifs_rec++;

    //Test xTaskNotifyWait from ISR
    xSemaphoreGive(trigger_send_semphr);
    xTaskNotifyWait(0, 0xFFFFFFFF, &notify_மதிப்பு, portMAX_DELAY);
    எனில் (notify_மதிப்பு != send_core_message) {
        wrong_core = true;
    }
    notifs_rec++;

    //Test ulTaskNotifyTake from ISR
    xSemaphoreGive(trigger_send_semphr);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    notifs_rec++;

    //Test complete, stop timer and delete task
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(வெற்று);
}

நிலையான வெறுமை install_gptimer_on_core(வெறுமை *arg)
{
    முழு core_id = (முழு)arg;
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_மொத்தம்_UP,
        .resolution_hz = 1000000, // 1MHz, 1 tick = 1us
    };
    TEST_ESP_OK(gptimer_new_timer(&timer_config, &gptimers[core_id]));
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_alarm_sender_cb,
    };
    TEST_ESP_OK(gptimer_register_event_callbacks(gptimers[core_id], &cbs, வெற்று));
    TEST_ESP_OK(gptimer_enable(gptimers[core_id]));
    test_gptimer_start(gptimers[core_id]);
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(வெற்று);
}

TEST_CASE("Test Task_Notify", "[freertos]")
{
    test_start = false;
    trigger_send_semphr = x_Semaphore_Create_இருமம்();
    task_delete_semphr = xQueueCreateCountingSemaphore(10, 0);
    க்கு (முழு i = 0; i < CONFIG_FREERTOS_எண்_OF_CORES; i++) {
        xTaskCreatePinnedToCore(install_gptimer_on_core, "install_gptimer", 4096, (வெறுமை *மாறிலி)i, UNITY_FREERTOS_PRIORITY + 1, வெற்று, i);
        TEST_ASSERT(xSemaphoreTake(task_delete_semphr, pdMS_TO_TICKS(1000)));
    }
    // wait the gptimer installation done on specific core
    vTaskDelay(10);
    // test start
    test_start = true;
    க்கு (முழு i = 0; i < CONFIG_FREERTOS_எண்_OF_CORES; i++) { //Sending Core
        க்கு (முழு j = 0; j < CONFIG_FREERTOS_எண்_OF_CORES; j++) { //Receiving Core
            //Reset Values
            notifs_sent = 0;
            notifs_rec = 0;
            wrong_core = false;
            send_core_message = (0xFF << i); //0xFF if core 0, 0xFF0 if core 1
            // receiver task has higher priority than sender task
            xTaskCreatePinnedToCore(receiver_task, "recv task", 1000, வெற்று, UNITY_FREERTOS_PRIORITY + 2, &recv_task_handle, j);
            xTaskCreatePinnedToCore(sender_task, "send task", 1000, gptimers[i], UNITY_FREERTOS_PRIORITY + 1, வெற்று, i);
            vTaskDelay(5);      //Wait for task creation to complete

            xSemaphoreGive(trigger_send_semphr);    //Trigger sender task
            க்கு (முழு k = 0; k < NO_OF_TASKS; k++) { //Wait for sender and receiver task deletion
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
    க்கு (முழு i = 0; i < CONFIG_FREERTOS_எண்_OF_CORES; i++) {
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
    uint32_t notification_மதிப்பு = 47;
    xTaskNotifyIndexed(xTaskGetCurrentTaskHandle(), 2, notification_மதிப்பு, eNoAction);
}

/* Test causes asserts, so it cannot be run as a normal unity test case.
   Test case is ran as a separate test case in test_task_notify_wait_too_high_index_fails
 */
TEST_CASE("Notify Wait too high index fails", "[ignore]")
{
    uint32_t notification_மதிப்பு;
    xTaskNotifyWaitIndexed(2, 0, 0, &notification_மதிப்பு, pdMS_TO_TICKS(10));
}

#எனில் CONFIG_FREERTOS_TASK_NOTIFICATION_ARRAY_ENTRIES > 1
மாறிலி uint32_t NOTIFICATION_மதிப்பு_0 = 47;
மாறிலி uint32_t NOTIFICATION_மதிப்பு_1 = 48;

வெறுமை notify(வெறுமை *arg)
{
    TaskHandle_t main_task = (TaskHandle_t) arg;
    xTaskNotifyIndexed(main_task, 0, NOTIFICATION_மதிப்பு_0, e_Set_மதிப்பு_With_Overwrite);
    xTaskNotifyIndexed(main_task, 1, NOTIFICATION_மதிப்பு_1, e_Set_மதிப்பு_With_Overwrite);
    vTaskDelete(வெற்று);
}
நிலையான TaskHandle_t notificator_task;

TEST_CASE("Notify to different indexes works", "[freertos]")
{
    uint32_t notification_மதிப்பு_0 = 0;
    uint32_t notification_மதிப்பு_1 = 0;

    TaskHandle_t main_task = xTaskGetCurrentTaskHandle();
    xTaskCreate(notify, "notificator", 2048, main_task, 2, &notificator_task);

    xTaskNotifyWaitIndexed(0, 0, 0xFFFFFFFF, &notification_மதிப்பு_0, pdMS_TO_TICKS(10));
    xTaskNotifyWaitIndexed(1, 0, 0xFFFFFFFF, &notification_மதிப்பு_1, pdMS_TO_TICKS(10));

    TEST_ASSERT_EQUAL(notification_மதிப்பு_0, NOTIFICATION_மதிப்பு_0);
    TEST_ASSERT_EQUAL(notification_மதிப்பு_1, NOTIFICATION_மதிப்பு_1);
}
#endif
