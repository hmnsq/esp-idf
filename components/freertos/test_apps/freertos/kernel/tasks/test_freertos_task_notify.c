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

#ಆದರೆ SOC_GPTIMER_SUPPORTED

#ವ್ಯಾಖ್ಯಾನಿಸು NO_ದ_NOTIFS    4
#ವ್ಯಾಖ್ಯಾನಿಸು NO_ದ_TASKS     2       //Sender and receiver
#ವ್ಯಾಖ್ಯಾನಿಸು ಸಂದೇಶ_2         0xFF

ಸ್ಥಿರ uint32_t send_core_ಸಂದೇಶ = 0;
ಸ್ಥಿರ TaskHandle_t recv_task_handle;
ಸ್ಥಿರ bool isr_give = false;
ಸ್ಥಿರ bool test_ಆರಂಭ = false;
ಸ್ಥಿರ gptimer_handle_t gptimers[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES];
ಸ್ಥಿರ SemaphoreHandle_t trigger_send_semphr;
ಸ್ಥಿರ SemaphoreHandle_t task_delete_semphr;

//Test tracking vars
ಸ್ಥಿರ ಅಸ್ಥಿರ uint32_t notifs_ಕಳುಹಿಸಿದ = 0;
ಸ್ಥಿರ ಅಸ್ಥಿರ uint32_t notifs_rec = 0;
ಸ್ಥಿರ bool wrong_core = false;

ಸ್ಥಿರ bool on_alarm_sender_cb(gptimer_handle_t timer, ಸ್ಥಿರಾಂಕ gptimer_alarm_event_ದತ್ತಾಂಶ_t *edata, ಶೂನ್ಯ *user_ctx)
{
    gptimer_stop(timer);
    ಆದರೆ (!test_ಆರಂಭ) {
        ಹಿಂತಿರುಗಿ false;
    }
    ಪೂರ್ಣಾಂಕ curcore = x_ಪೋರ್ಟ್_Get_Core_ID();
    ಆದರೆ (isr_give) { //Test vTaskNotifyGiveFromISR() on same core
        notifs_ಕಳುಹಿಸಿದ++;
        vTaskNotifyGiveFromISR(recv_task_handle, ಖಾಲಿ);
    } ಇಲ್ಲದಿದ್ದರೆ { //Test xTaskNotifyFromISR()
        notifs_ಕಳುಹಿಸಿದ++;
        xTaskNotifyFromISR(recv_task_handle, (ಸಂದೇಶ_2 << curcore), e_ಗಣ_ಮೌಲ್ಯ_With_Overwrite, ಖಾಲಿ);
    }
    // always trigger a task switch when exit ISR context
    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ test_gptimer_ಆರಂಭ(ಶೂನ್ಯ *arg)
{
    gptimer_handle_t gptimer = (gptimer_handle_t)arg;
    gptimer_alarm_config_t alarm_config = {
        .reload_ಎಣಿಕೆ = 0,
        .alarm_ಎಣಿಕೆ = 1000,
    };
    TEST_ESP_OK(gptimer_ಗಣ_raw_ಎಣಿಕೆ(gptimer, 0));
    TEST_ESP_OK(gptimer_ಗಣ_alarm_action(gptimer, &alarm_config));
    TEST_ESP_OK(gptimer_ಆರಂಭ(gptimer));
}

ಸ್ಥಿರ ಶೂನ್ಯ sender_task(ಶೂನ್ಯ *arg)
{
    gptimer_handle_t gptimer = (gptimer_handle_t)arg;
    ಪೂರ್ಣಾಂಕ curcore = x_ಪೋರ್ಟ್_Get_Core_ID();

    //Test xTaskNotify
    xSemaphoreTake(trigger_send_semphr, ಪೋರ್ಟ್_MAX_DELAY);
    notifs_ಕಳುಹಿಸಿದ++;
    xTaskNotify(recv_task_handle, (ಸಂದೇಶ_2 << curcore), e_ಗಣ_ಮೌಲ್ಯ_With_Overwrite);

    //Test xTaskNotifyGive
    xSemaphoreTake(trigger_send_semphr, ಪೋರ್ಟ್_MAX_DELAY);
    notifs_ಕಳುಹಿಸಿದ++;
    xTaskNotifyGive(recv_task_handle);

    //Test xTaskNotifyFromISR
    xSemaphoreTake(trigger_send_semphr, ಪೋರ್ಟ್_MAX_DELAY);
    isr_give = false;
    test_gptimer_ಆರಂಭ(gptimer);

    //Test vTaskNotifyGiveFromISR
    xSemaphoreTake(trigger_send_semphr, ಪೋರ್ಟ್_MAX_DELAY);
    isr_give = true;
    test_gptimer_ಆರಂಭ(gptimer);

    //Delete Task and Semaphores
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ receiver_task(ಶೂನ್ಯ *arg)
{
    uint32_t notify_ಮೌಲ್ಯ;

    //Test xTaskNotifyWait from task
    xTaskNotifyWait(0, 0xFFFFFFFF, &notify_ಮೌಲ್ಯ, ಪೋರ್ಟ್_MAX_DELAY);
    ಆದರೆ (notify_ಮೌಲ್ಯ != send_core_ಸಂದೇಶ) {
        wrong_core = true;
    }
    notifs_rec++;

    //Test ulTaskNotifyTake from task
    xSemaphoreGive(trigger_send_semphr);
    ulTaskNotifyTake(pdTRUE, ಪೋರ್ಟ್_MAX_DELAY);
    notifs_rec++;

    //Test xTaskNotifyWait from ISR
    xSemaphoreGive(trigger_send_semphr);
    xTaskNotifyWait(0, 0xFFFFFFFF, &notify_ಮೌಲ್ಯ, ಪೋರ್ಟ್_MAX_DELAY);
    ಆದರೆ (notify_ಮೌಲ್ಯ != send_core_ಸಂದೇಶ) {
        wrong_core = true;
    }
    notifs_rec++;

    //Test ulTaskNotifyTake from ISR
    xSemaphoreGive(trigger_send_semphr);
    ulTaskNotifyTake(pdTRUE, ಪೋರ್ಟ್_MAX_DELAY);
    notifs_rec++;

    //Test complete, stop timer and delete task
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ install_gptimer_on_core(ಶೂನ್ಯ *arg)
{
    ಪೂರ್ಣಾಂಕ core_id = (ಪೂರ್ಣಾಂಕ)arg;
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_ಎಣಿಕೆ_ಮೇಲೆ,
        .resolution_hz = 1000000, // 1MHz, 1 tick = 1us
    };
    TEST_ESP_OK(gptimer_new_timer(&timer_config, &gptimers[core_id]));
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_alarm_sender_cb,
    };
    TEST_ESP_OK(gptimer_ರಿಜಿಸ್ಟರ್_event_callbacks(gptimers[core_id], &cbs, ಖಾಲಿ));
    TEST_ESP_OK(gptimer_enable(gptimers[core_id]));
    test_gptimer_ಆರಂಭ(gptimers[core_id]);
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(ಖಾಲಿ);
}

TEST_CASE("Test Task_Notify", "[freertos]")
{
    test_ಆರಂಭ = false;
    trigger_send_semphr = x_Semaphore_Create_ದ್ವಿಮಾನ();
    task_delete_semphr = x_ಸರತಿ_Create_Counting_Semaphore(10, 0);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; i++) {
        x_Task_Create_Pinned_ಗಮ್ಯ_Core(install_gptimer_on_core, "install_gptimer", 4096, (ಶೂನ್ಯ *ಸ್ಥಿರಾಂಕ)i, UNITY_FREERTOS_ಆದ್ಯತೆ + 1, ಖಾಲಿ, i);
        TEST_ASSERT(xSemaphoreTake(task_delete_semphr, pd_MS_ಗಮ್ಯ_TICKS(1000)));
    }
    // wait the gptimer installation done on specific core
    vTaskDelay(10);
    // test start
    test_ಆರಂಭ = true;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; i++) { //Sending Core
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; j++) { //Receiving Core
            //Reset Values
            notifs_ಕಳುಹಿಸಿದ = 0;
            notifs_rec = 0;
            wrong_core = false;
            send_core_ಸಂದೇಶ = (0xFF << i); //0xFF if core 0, 0xFF0 if core 1
            // receiver task has higher priority than sender task
            x_Task_Create_Pinned_ಗಮ್ಯ_Core(receiver_task, "recv task", 1000, ಖಾಲಿ, UNITY_FREERTOS_ಆದ್ಯತೆ + 2, &recv_task_handle, j);
            x_Task_Create_Pinned_ಗಮ್ಯ_Core(sender_task, "send task", 1000, gptimers[i], UNITY_FREERTOS_ಆದ್ಯತೆ + 1, ಖಾಲಿ, i);
            vTaskDelay(5);      //Wait for task creation to complete

            xSemaphoreGive(trigger_send_semphr);    //Trigger sender task
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < NO_ದ_TASKS; k++) { //Wait for sender and receiver task deletion
                TEST_ASSERT(xSemaphoreTake(task_delete_semphr, pd_MS_ಗಮ್ಯ_TICKS(2000)));
            }
            vTaskDelay(5);      //Give time tasks to delete

            TEST_ASSERT_EQUAL(NO_ದ_NOTIFS, notifs_ಕಳುಹಿಸಿದ);
            TEST_ASSERT_EQUAL(NO_ದ_NOTIFS, notifs_rec);
            TEST_ASSERT_EQUAL(false, wrong_core);
        }
    }

    //Delete Semaphroes and timer ISRs
    vSemaphoreDelete(trigger_send_semphr);
    vSemaphoreDelete(task_delete_semphr);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; i++) {
        TEST_ESP_OK(gptimer_disable(gptimers[i]));
        TEST_ESP_OK(gptimer_del_timer(gptimers[i]));
    }
}
#ಆದರೆ_ಅಂತ್ಯ //SOC_GPTIMER_SUPPORTED

/* Test causes asserts, so it cannot be run as a normal unity test case.
   Test case is ran as a separate test case in test_task_notify_too_high_index_fails
 */
TEST_CASE("Notify too high index fails", "[ignore]")
{
    uint32_t notification_ಮೌಲ್ಯ = 47;
    x_Task_Notify_ಸೂಚ್ಯಂಕಿತ(xTaskGetCurrentTaskHandle(), 2, notification_ಮೌಲ್ಯ, eNoAction);
}

/* Test causes asserts, so it cannot be run as a normal unity test case.
   Test case is ran as a separate test case in test_task_notify_wait_too_high_index_fails
 */
TEST_CASE("Notify Wait too high index fails", "[ignore]")
{
    uint32_t notification_ಮೌಲ್ಯ;
    x_Task_Notify_Wait_ಸೂಚ್ಯಂಕಿತ(2, 0, 0, &notification_ಮೌಲ್ಯ, pd_MS_ಗಮ್ಯ_TICKS(10));
}

#ಆದರೆ CONFIG_FREERTOS_TASK_NOTIFICATION_ARRAY_ENTRIES > 1
ಸ್ಥಿರಾಂಕ uint32_t NOTIFICATION_ಮೌಲ್ಯ_0 = 47;
ಸ್ಥಿರಾಂಕ uint32_t NOTIFICATION_ಮೌಲ್ಯ_1 = 48;

ಶೂನ್ಯ notify(ಶೂನ್ಯ *arg)
{
    TaskHandle_t main_task = (TaskHandle_t) arg;
    x_Task_Notify_ಸೂಚ್ಯಂಕಿತ(main_task, 0, NOTIFICATION_ಮೌಲ್ಯ_0, e_ಗಣ_ಮೌಲ್ಯ_With_Overwrite);
    x_Task_Notify_ಸೂಚ್ಯಂಕಿತ(main_task, 1, NOTIFICATION_ಮೌಲ್ಯ_1, e_ಗಣ_ಮೌಲ್ಯ_With_Overwrite);
    vTaskDelete(ಖಾಲಿ);
}
ಸ್ಥಿರ TaskHandle_t notificator_task;

TEST_CASE("Notify to different indexes works", "[freertos]")
{
    uint32_t notification_ಮೌಲ್ಯ_0 = 0;
    uint32_t notification_ಮೌಲ್ಯ_1 = 0;

    TaskHandle_t main_task = xTaskGetCurrentTaskHandle();
    xTaskCreate(notify, "notificator", 2048, main_task, 2, &notificator_task);

    x_Task_Notify_Wait_ಸೂಚ್ಯಂಕಿತ(0, 0, 0xFFFFFFFF, &notification_ಮೌಲ್ಯ_0, pd_MS_ಗಮ್ಯ_TICKS(10));
    x_Task_Notify_Wait_ಸೂಚ್ಯಂಕಿತ(1, 0, 0xFFFFFFFF, &notification_ಮೌಲ್ಯ_1, pd_MS_ಗಮ್ಯ_TICKS(10));

    TEST_ASSERT_EQUAL(notification_ಮೌಲ್ಯ_0, NOTIFICATION_ಮೌಲ್ಯ_0);
    TEST_ASSERT_EQUAL(notification_ಮೌಲ್ಯ_1, NOTIFICATION_ಮೌಲ್ಯ_1);
}
#ಆದರೆ_ಅಂತ್ಯ
