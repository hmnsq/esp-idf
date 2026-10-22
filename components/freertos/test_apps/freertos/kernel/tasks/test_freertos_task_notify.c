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
#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "freertos/semphr.h"
#ಅನ್ತರ್ಭಾವ "driver/gptimer.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "test_utils.h"

#ಯದಿ SOC_GPTIMER_SUPPORTED

#define NO_OF_NOTIFS    4
#define NO_OF_TASKS     2       //Sender and receiver
#define MESSAGE         0xFF

ಸ್ಥಿರ uint32_t send_core_message = 0;
ಸ್ಥಿರ TaskHandle_t recv_task_handle;
ಸ್ಥಿರ bool isr_give = false;
ಸ್ಥಿರ bool test_start = false;
ಸ್ಥಿರ gptimer_handle_t gptimers[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES];
ಸ್ಥಿರ SemaphoreHandle_t trigger_send_semphr;
ಸ್ಥಿರ SemaphoreHandle_t task_delete_semphr;

//Test tracking vars
ಸ್ಥಿರ ಚಞ್ಚಲ uint32_t notifs_sent = 0;
ಸ್ಥಿರ ಚಞ್ಚಲ uint32_t notifs_rec = 0;
ಸ್ಥಿರ bool wrong_core = false;

ಸ್ಥಿರ bool on_alarm_sender_cb(gptimer_handle_t timer, ಧ್ರುವ gptimer_alarm_event_data_t *edata, ಶೂನ್ಯ *user_ctx)
{
    gptimer_stop(timer);
    ಯದಿ (!test_start) {
        ಪ್ರತ್ಯಾಗಮ false;
    }
    ಪೂರ್ಣಾಙ್ಕ curcore = xPortGetCoreID();
    ಯದಿ (isr_give) { //Test vTaskNotifyGiveFromISR() on same core
        notifs_sent++;
        vTaskNotifyGiveFromISR(recv_task_handle, ರಿಕ್ತ);
    } ಅನ್ಯಥಾ { //Test xTaskNotifyFromISR()
        notifs_sent++;
        xTaskNotifyFromISR(recv_task_handle, (MESSAGE << curcore), e_Set_ಮೌಲ್ಯ_With_Overwrite, ರಿಕ್ತ);
    }
    // always trigger a task switch when exit ISR context
    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ test_gptimer_start(ಶೂನ್ಯ *arg)
{
    gptimer_handle_t gptimer = (gptimer_handle_t)arg;
    gptimer_alarm_config_t alarm_config = {
        .reload_ಎಣಿಕೆ = 0,
        .alarm_ಎಣಿಕೆ = 1000,
    };
    TEST_ESP_OK(gptimer_set_raw_ಎಣಿಕೆ(gptimer, 0));
    TEST_ESP_OK(gptimer_set_alarm_action(gptimer, &alarm_config));
    TEST_ESP_OK(gptimer_start(gptimer));
}

ಸ್ಥಿರ ಶೂನ್ಯ sender_task(ಶೂನ್ಯ *arg)
{
    gptimer_handle_t gptimer = (gptimer_handle_t)arg;
    ಪೂರ್ಣಾಙ್ಕ curcore = xPortGetCoreID();

    //Test xTaskNotify
    xSemaphoreTake(trigger_send_semphr, portMAX_DELAY);
    notifs_sent++;
    xTaskNotify(recv_task_handle, (MESSAGE << curcore), e_Set_ಮೌಲ್ಯ_With_Overwrite);

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
    vTaskDelete(ರಿಕ್ತ);
}

ಸ್ಥಿರ ಶೂನ್ಯ receiver_task(ಶೂನ್ಯ *arg)
{
    uint32_t notify_ಮೌಲ್ಯ;

    //Test xTaskNotifyWait from task
    xTaskNotifyWait(0, 0xFFFFFFFF, &notify_ಮೌಲ್ಯ, portMAX_DELAY);
    ಯದಿ (notify_ಮೌಲ್ಯ != send_core_message) {
        wrong_core = true;
    }
    notifs_rec++;

    //Test ulTaskNotifyTake from task
    xSemaphoreGive(trigger_send_semphr);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    notifs_rec++;

    //Test xTaskNotifyWait from ISR
    xSemaphoreGive(trigger_send_semphr);
    xTaskNotifyWait(0, 0xFFFFFFFF, &notify_ಮೌಲ್ಯ, portMAX_DELAY);
    ಯದಿ (notify_ಮೌಲ್ಯ != send_core_message) {
        wrong_core = true;
    }
    notifs_rec++;

    //Test ulTaskNotifyTake from ISR
    xSemaphoreGive(trigger_send_semphr);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    notifs_rec++;

    //Test complete, stop timer and delete task
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(ರಿಕ್ತ);
}

ಸ್ಥಿರ ಶೂನ್ಯ install_gptimer_on_core(ಶೂನ್ಯ *arg)
{
    ಪೂರ್ಣಾಙ್ಕ core_id = (ಪೂರ್ಣಾಙ್ಕ)arg;
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_ಎಣಿಕೆ_UP,
        .resolution_hz = 1000000, // 1MHz, 1 tick = 1us
    };
    TEST_ESP_OK(gptimer_new_timer(&timer_config, &gptimers[core_id]));
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_alarm_sender_cb,
    };
    TEST_ESP_OK(gptimer_register_event_callbacks(gptimers[core_id], &cbs, ರಿಕ್ತ));
    TEST_ESP_OK(gptimer_enable(gptimers[core_id]));
    test_gptimer_start(gptimers[core_id]);
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(ರಿಕ್ತ);
}

TEST_CASE("Test Task_Notify", "[freertos]")
{
    test_start = false;
    trigger_send_semphr = x_Semaphore_Create_ದ್ವಿಮಾನ();
    task_delete_semphr = xQueueCreateCountingSemaphore(10, 0);
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES; i++) {
        xTaskCreatePinnedToCore(install_gptimer_on_core, "install_gptimer", 4096, (ಶೂನ್ಯ *ಧ್ರುವ)i, UNITY_FREERTOS_PRIORITY + 1, ರಿಕ್ತ, i);
        TEST_ASSERT(xSemaphoreTake(task_delete_semphr, pdMS_TO_TICKS(1000)));
    }
    // wait the gptimer installation done on specific core
    vTaskDelay(10);
    // test start
    test_start = true;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES; i++) { //Sending Core
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES; j++) { //Receiving Core
            //Reset Values
            notifs_sent = 0;
            notifs_rec = 0;
            wrong_core = false;
            send_core_message = (0xFF << i); //0xFF if core 0, 0xFF0 if core 1
            // receiver task has higher priority than sender task
            xTaskCreatePinnedToCore(receiver_task, "recv task", 1000, ರಿಕ್ತ, UNITY_FREERTOS_PRIORITY + 2, &recv_task_handle, j);
            xTaskCreatePinnedToCore(sender_task, "send task", 1000, gptimers[i], UNITY_FREERTOS_PRIORITY + 1, ರಿಕ್ತ, i);
            vTaskDelay(5);      //Wait for task creation to complete

            xSemaphoreGive(trigger_send_semphr);    //Trigger sender task
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < NO_OF_TASKS; k++) { //Wait for sender and receiver task deletion
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
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES; i++) {
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
    uint32_t notification_ಮೌಲ್ಯ = 47;
    xTaskNotifyIndexed(xTaskGetCurrentTaskHandle(), 2, notification_ಮೌಲ್ಯ, eNoAction);
}

/* Test causes asserts, so it cannot be run as a normal unity test case.
   Test case is ran as a separate test case in test_task_notify_wait_too_high_index_fails
 */
TEST_CASE("Notify Wait too high index fails", "[ignore]")
{
    uint32_t notification_ಮೌಲ್ಯ;
    xTaskNotifyWaitIndexed(2, 0, 0, &notification_ಮೌಲ್ಯ, pdMS_TO_TICKS(10));
}

#ಯದಿ CONFIG_FREERTOS_TASK_NOTIFICATION_ARRAY_ENTRIES > 1
ಧ್ರುವ uint32_t NOTIFICATION_ಮೌಲ್ಯ_0 = 47;
ಧ್ರುವ uint32_t NOTIFICATION_ಮೌಲ್ಯ_1 = 48;

ಶೂನ್ಯ notify(ಶೂನ್ಯ *arg)
{
    TaskHandle_t main_task = (TaskHandle_t) arg;
    xTaskNotifyIndexed(main_task, 0, NOTIFICATION_ಮೌಲ್ಯ_0, e_Set_ಮೌಲ್ಯ_With_Overwrite);
    xTaskNotifyIndexed(main_task, 1, NOTIFICATION_ಮೌಲ್ಯ_1, e_Set_ಮೌಲ್ಯ_With_Overwrite);
    vTaskDelete(ರಿಕ್ತ);
}
ಸ್ಥಿರ TaskHandle_t notificator_task;

TEST_CASE("Notify to different indexes works", "[freertos]")
{
    uint32_t notification_ಮೌಲ್ಯ_0 = 0;
    uint32_t notification_ಮೌಲ್ಯ_1 = 0;

    TaskHandle_t main_task = xTaskGetCurrentTaskHandle();
    xTaskCreate(notify, "notificator", 2048, main_task, 2, &notificator_task);

    xTaskNotifyWaitIndexed(0, 0, 0xFFFFFFFF, &notification_ಮೌಲ್ಯ_0, pdMS_TO_TICKS(10));
    xTaskNotifyWaitIndexed(1, 0, 0xFFFFFFFF, &notification_ಮೌಲ್ಯ_1, pdMS_TO_TICKS(10));

    TEST_ASSERT_EQUAL(notification_ಮೌಲ್ಯ_0, NOTIFICATION_ಮೌಲ್ಯ_0);
    TEST_ASSERT_EQUAL(notification_ಮೌಲ್ಯ_1, NOTIFICATION_ಮೌಲ್ಯ_1);
}
#endif
