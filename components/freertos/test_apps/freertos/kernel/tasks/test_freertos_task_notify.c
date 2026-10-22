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
#অন্তর্ভাব "sdkconfig.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব "freertos/FreeRTOS.h"
#অন্তর্ভাব "freertos/task.h"
#অন্তর্ভাব "freertos/semphr.h"
#অন্তর্ভাব "driver/gptimer.h"
#অন্তর্ভাব "unity.h"
#অন্তর্ভাব "test_utils.h"

#যদি SOC_GPTIMER_SUPPORTED

#define NO_OF_NOTIFS    4
#define NO_OF_TASKS     2       //Sender and receiver
#define MESSAGE         0xFF

স্থির uint32_t send_core_message = 0;
স্থির TaskHandle_t recv_task_handle;
স্থির bool isr_give = false;
স্থির bool test_start = false;
স্থির gptimer_handle_t gptimers[CONFIG_FREERTOS_সংখ্যা_OF_CORES];
স্থির SemaphoreHandle_t trigger_send_semphr;
স্থির SemaphoreHandle_t task_delete_semphr;

//Test tracking vars
স্থির চঞ্চল uint32_t notifs_sent = 0;
স্থির চঞ্চল uint32_t notifs_rec = 0;
স্থির bool wrong_core = false;

স্থির bool on_alarm_sender_cb(gptimer_handle_t timer, ধ্রুব gptimer_alarm_event_data_t *edata, শূন্য *user_ctx)
{
    gptimer_stop(timer);
    যদি (!test_start) {
        প্রত্যাগম false;
    }
    পূর্ণাঙ্ক curcore = xPortGetCoreID();
    যদি (isr_give) { //Test vTaskNotifyGiveFromISR() on same core
        notifs_sent++;
        vTaskNotifyGiveFromISR(recv_task_handle, রিক্ত);
    } অন্যথা { //Test xTaskNotifyFromISR()
        notifs_sent++;
        xTaskNotifyFromISR(recv_task_handle, (MESSAGE << curcore), e_Set_মান_With_Overwrite, রিক্ত);
    }
    // always trigger a task switch when exit ISR context
    প্রত্যাগম true;
}

স্থির শূন্য test_gptimer_start(শূন্য *arg)
{
    gptimer_handle_t gptimer = (gptimer_handle_t)arg;
    gptimer_alarm_config_t alarm_config = {
        .reload_গুনতি = 0,
        .alarm_গুনতি = 1000,
    };
    TEST_ESP_OK(gptimer_set_raw_গুনতি(gptimer, 0));
    TEST_ESP_OK(gptimer_set_alarm_action(gptimer, &alarm_config));
    TEST_ESP_OK(gptimer_start(gptimer));
}

স্থির শূন্য sender_task(শূন্য *arg)
{
    gptimer_handle_t gptimer = (gptimer_handle_t)arg;
    পূর্ণাঙ্ক curcore = xPortGetCoreID();

    //Test xTaskNotify
    xSemaphoreTake(trigger_send_semphr, portMAX_DELAY);
    notifs_sent++;
    xTaskNotify(recv_task_handle, (MESSAGE << curcore), e_Set_মান_With_Overwrite);

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
    vTaskDelete(রিক্ত);
}

স্থির শূন্য receiver_task(শূন্য *arg)
{
    uint32_t notify_মান;

    //Test xTaskNotifyWait from task
    xTaskNotifyWait(0, 0xFFFFFFFF, &notify_মান, portMAX_DELAY);
    যদি (notify_মান != send_core_message) {
        wrong_core = true;
    }
    notifs_rec++;

    //Test ulTaskNotifyTake from task
    xSemaphoreGive(trigger_send_semphr);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    notifs_rec++;

    //Test xTaskNotifyWait from ISR
    xSemaphoreGive(trigger_send_semphr);
    xTaskNotifyWait(0, 0xFFFFFFFF, &notify_মান, portMAX_DELAY);
    যদি (notify_মান != send_core_message) {
        wrong_core = true;
    }
    notifs_rec++;

    //Test ulTaskNotifyTake from ISR
    xSemaphoreGive(trigger_send_semphr);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    notifs_rec++;

    //Test complete, stop timer and delete task
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(রিক্ত);
}

স্থির শূন্য install_gptimer_on_core(শূন্য *arg)
{
    পূর্ণাঙ্ক core_id = (পূর্ণাঙ্ক)arg;
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_গুনতি_UP,
        .resolution_hz = 1000000, // 1MHz, 1 tick = 1us
    };
    TEST_ESP_OK(gptimer_new_timer(&timer_config, &gptimers[core_id]));
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_alarm_sender_cb,
    };
    TEST_ESP_OK(gptimer_register_event_callbacks(gptimers[core_id], &cbs, রিক্ত));
    TEST_ESP_OK(gptimer_enable(gptimers[core_id]));
    test_gptimer_start(gptimers[core_id]);
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(রিক্ত);
}

TEST_CASE("Test Task_Notify", "[freertos]")
{
    test_start = false;
    trigger_send_semphr = x_Semaphore_Create_দ্বিমিক();
    task_delete_semphr = xQueueCreateCountingSemaphore(10, 0);
    প্রতি (পূর্ণাঙ্ক i = 0; i < CONFIG_FREERTOS_সংখ্যা_OF_CORES; i++) {
        xTaskCreatePinnedToCore(install_gptimer_on_core, "install_gptimer", 4096, (শূন্য *ধ্রুব)i, UNITY_FREERTOS_PRIORITY + 1, রিক্ত, i);
        TEST_ASSERT(xSemaphoreTake(task_delete_semphr, pdMS_TO_TICKS(1000)));
    }
    // wait the gptimer installation done on specific core
    vTaskDelay(10);
    // test start
    test_start = true;
    প্রতি (পূর্ণাঙ্ক i = 0; i < CONFIG_FREERTOS_সংখ্যা_OF_CORES; i++) { //Sending Core
        প্রতি (পূর্ণাঙ্ক j = 0; j < CONFIG_FREERTOS_সংখ্যা_OF_CORES; j++) { //Receiving Core
            //Reset Values
            notifs_sent = 0;
            notifs_rec = 0;
            wrong_core = false;
            send_core_message = (0xFF << i); //0xFF if core 0, 0xFF0 if core 1
            // receiver task has higher priority than sender task
            xTaskCreatePinnedToCore(receiver_task, "recv task", 1000, রিক্ত, UNITY_FREERTOS_PRIORITY + 2, &recv_task_handle, j);
            xTaskCreatePinnedToCore(sender_task, "send task", 1000, gptimers[i], UNITY_FREERTOS_PRIORITY + 1, রিক্ত, i);
            vTaskDelay(5);      //Wait for task creation to complete

            xSemaphoreGive(trigger_send_semphr);    //Trigger sender task
            প্রতি (পূর্ণাঙ্ক k = 0; k < NO_OF_TASKS; k++) { //Wait for sender and receiver task deletion
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
    প্রতি (পূর্ণাঙ্ক i = 0; i < CONFIG_FREERTOS_সংখ্যা_OF_CORES; i++) {
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
    uint32_t notification_মান = 47;
    xTaskNotifyIndexed(xTaskGetCurrentTaskHandle(), 2, notification_মান, eNoAction);
}

/* Test causes asserts, so it cannot be run as a normal unity test case.
   Test case is ran as a separate test case in test_task_notify_wait_too_high_index_fails
 */
TEST_CASE("Notify Wait too high index fails", "[ignore]")
{
    uint32_t notification_মান;
    xTaskNotifyWaitIndexed(2, 0, 0, &notification_মান, pdMS_TO_TICKS(10));
}

#যদি CONFIG_FREERTOS_TASK_NOTIFICATION_ARRAY_ENTRIES > 1
ধ্রুব uint32_t NOTIFICATION_মান_0 = 47;
ধ্রুব uint32_t NOTIFICATION_মান_1 = 48;

শূন্য notify(শূন্য *arg)
{
    TaskHandle_t main_task = (TaskHandle_t) arg;
    xTaskNotifyIndexed(main_task, 0, NOTIFICATION_মান_0, e_Set_মান_With_Overwrite);
    xTaskNotifyIndexed(main_task, 1, NOTIFICATION_মান_1, e_Set_মান_With_Overwrite);
    vTaskDelete(রিক্ত);
}
স্থির TaskHandle_t notificator_task;

TEST_CASE("Notify to different indexes works", "[freertos]")
{
    uint32_t notification_মান_0 = 0;
    uint32_t notification_মান_1 = 0;

    TaskHandle_t main_task = xTaskGetCurrentTaskHandle();
    xTaskCreate(notify, "notificator", 2048, main_task, 2, &notificator_task);

    xTaskNotifyWaitIndexed(0, 0, 0xFFFFFFFF, &notification_মান_0, pdMS_TO_TICKS(10));
    xTaskNotifyWaitIndexed(1, 0, 0xFFFFFFFF, &notification_মান_1, pdMS_TO_TICKS(10));

    TEST_ASSERT_EQUAL(notification_মান_0, NOTIFICATION_মান_0);
    TEST_ASSERT_EQUAL(notification_মান_1, NOTIFICATION_মান_1);
}
#endif
