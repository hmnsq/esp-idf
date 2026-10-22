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
#അന്തര്ഭാവ "sdkconfig.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ "freertos/FreeRTOS.h"
#അന്തര്ഭാവ "freertos/task.h"
#അന്തര്ഭാവ "freertos/semphr.h"
#അന്തര്ഭാവ "driver/gptimer.h"
#അന്തര്ഭാവ "unity.h"
#അന്തര്ഭാവ "test_utils.h"

#എങ്കിൽ SOC_GPTIMER_SUPPORTED

#define NO_OF_NOTIFS    4
#define NO_OF_TASKS     2       //Sender and receiver
#define MESSAGE         0xFF

സ്ഥിതം uint32_t send_core_message = 0;
സ്ഥിതം TaskHandle_t recv_task_handle;
സ്ഥിതം bool isr_give = false;
സ്ഥിതം bool test_start = false;
സ്ഥിതം gptimer_handle_t gptimers[CONFIG_FREERTOS_സംഖ്യ_OF_CORES];
സ്ഥിതം SemaphoreHandle_t trigger_send_semphr;
സ്ഥിതം SemaphoreHandle_t task_delete_semphr;

//Test tracking vars
സ്ഥിതം അസ്ഥിര uint32_t notifs_sent = 0;
സ്ഥിതം അസ്ഥിര uint32_t notifs_rec = 0;
സ്ഥിതം bool wrong_core = false;

സ്ഥിതം bool on_alarm_sender_cb(gptimer_handle_t timer, സ്ഥിരം gptimer_alarm_event_data_t *edata, ശൂന്യം *user_ctx)
{
    gptimer_stop(timer);
    എങ്കിൽ (!test_start) {
        തിരികെ false;
    }
    പൂർണ്ണസംഖ്യ curcore = xPortGetCoreID();
    എങ്കിൽ (isr_give) { //Test vTaskNotifyGiveFromISR() on same core
        notifs_sent++;
        vTaskNotifyGiveFromISR(recv_task_handle, ശൂന്യ);
    } അല്ലെങ്കിൽ { //Test xTaskNotifyFromISR()
        notifs_sent++;
        xTaskNotifyFromISR(recv_task_handle, (MESSAGE << curcore), e_Set_മൂല്യം_With_Overwrite, ശൂന്യ);
    }
    // always trigger a task switch when exit ISR context
    തിരികെ true;
}

സ്ഥിതം ശൂന്യം test_gptimer_start(ശൂന്യം *arg)
{
    gptimer_handle_t gptimer = (gptimer_handle_t)arg;
    gptimer_alarm_config_t alarm_config = {
        .reload_എണ്ണം = 0,
        .alarm_എണ്ണം = 1000,
    };
    TEST_ESP_OK(gptimer_set_raw_എണ്ണം(gptimer, 0));
    TEST_ESP_OK(gptimer_set_alarm_action(gptimer, &alarm_config));
    TEST_ESP_OK(gptimer_start(gptimer));
}

സ്ഥിതം ശൂന്യം sender_task(ശൂന്യം *arg)
{
    gptimer_handle_t gptimer = (gptimer_handle_t)arg;
    പൂർണ്ണസംഖ്യ curcore = xPortGetCoreID();

    //Test xTaskNotify
    xSemaphoreTake(trigger_send_semphr, portMAX_DELAY);
    notifs_sent++;
    xTaskNotify(recv_task_handle, (MESSAGE << curcore), e_Set_മൂല്യം_With_Overwrite);

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
    vTaskDelete(ശൂന്യ);
}

സ്ഥിതം ശൂന്യം receiver_task(ശൂന്യം *arg)
{
    uint32_t notify_മൂല്യം;

    //Test xTaskNotifyWait from task
    xTaskNotifyWait(0, 0xFFFFFFFF, &notify_മൂല്യം, portMAX_DELAY);
    എങ്കിൽ (notify_മൂല്യം != send_core_message) {
        wrong_core = true;
    }
    notifs_rec++;

    //Test ulTaskNotifyTake from task
    xSemaphoreGive(trigger_send_semphr);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    notifs_rec++;

    //Test xTaskNotifyWait from ISR
    xSemaphoreGive(trigger_send_semphr);
    xTaskNotifyWait(0, 0xFFFFFFFF, &notify_മൂല്യം, portMAX_DELAY);
    എങ്കിൽ (notify_മൂല്യം != send_core_message) {
        wrong_core = true;
    }
    notifs_rec++;

    //Test ulTaskNotifyTake from ISR
    xSemaphoreGive(trigger_send_semphr);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    notifs_rec++;

    //Test complete, stop timer and delete task
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(ശൂന്യ);
}

സ്ഥിതം ശൂന്യം install_gptimer_on_core(ശൂന്യം *arg)
{
    പൂർണ്ണസംഖ്യ core_id = (പൂർണ്ണസംഖ്യ)arg;
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_എണ്ണം_UP,
        .resolution_hz = 1000000, // 1MHz, 1 tick = 1us
    };
    TEST_ESP_OK(gptimer_new_timer(&timer_config, &gptimers[core_id]));
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_alarm_sender_cb,
    };
    TEST_ESP_OK(gptimer_register_event_callbacks(gptimers[core_id], &cbs, ശൂന്യ));
    TEST_ESP_OK(gptimer_enable(gptimers[core_id]));
    test_gptimer_start(gptimers[core_id]);
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(ശൂന്യ);
}

TEST_CASE("Test Task_Notify", "[freertos]")
{
    test_start = false;
    trigger_send_semphr = x_Semaphore_Create_ദ്വിമാന();
    task_delete_semphr = xQueueCreateCountingSemaphore(10, 0);
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; i++) {
        xTaskCreatePinnedToCore(install_gptimer_on_core, "install_gptimer", 4096, (ശൂന്യം *സ്ഥിരം)i, UNITY_FREERTOS_PRIORITY + 1, ശൂന്യ, i);
        TEST_ASSERT(xSemaphoreTake(task_delete_semphr, pdMS_TO_TICKS(1000)));
    }
    // wait the gptimer installation done on specific core
    vTaskDelay(10);
    // test start
    test_start = true;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; i++) { //Sending Core
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; j++) { //Receiving Core
            //Reset Values
            notifs_sent = 0;
            notifs_rec = 0;
            wrong_core = false;
            send_core_message = (0xFF << i); //0xFF if core 0, 0xFF0 if core 1
            // receiver task has higher priority than sender task
            xTaskCreatePinnedToCore(receiver_task, "recv task", 1000, ശൂന്യ, UNITY_FREERTOS_PRIORITY + 2, &recv_task_handle, j);
            xTaskCreatePinnedToCore(sender_task, "send task", 1000, gptimers[i], UNITY_FREERTOS_PRIORITY + 1, ശൂന്യ, i);
            vTaskDelay(5);      //Wait for task creation to complete

            xSemaphoreGive(trigger_send_semphr);    //Trigger sender task
            വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < NO_OF_TASKS; k++) { //Wait for sender and receiver task deletion
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
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; i++) {
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
    uint32_t notification_മൂല്യം = 47;
    xTaskNotifyIndexed(xTaskGetCurrentTaskHandle(), 2, notification_മൂല്യം, eNoAction);
}

/* Test causes asserts, so it cannot be run as a normal unity test case.
   Test case is ran as a separate test case in test_task_notify_wait_too_high_index_fails
 */
TEST_CASE("Notify Wait too high index fails", "[ignore]")
{
    uint32_t notification_മൂല്യം;
    xTaskNotifyWaitIndexed(2, 0, 0, &notification_മൂല്യം, pdMS_TO_TICKS(10));
}

#എങ്കിൽ CONFIG_FREERTOS_TASK_NOTIFICATION_ARRAY_ENTRIES > 1
സ്ഥിരം uint32_t NOTIFICATION_മൂല്യം_0 = 47;
സ്ഥിരം uint32_t NOTIFICATION_മൂല്യം_1 = 48;

ശൂന്യം notify(ശൂന്യം *arg)
{
    TaskHandle_t main_task = (TaskHandle_t) arg;
    xTaskNotifyIndexed(main_task, 0, NOTIFICATION_മൂല്യം_0, e_Set_മൂല്യം_With_Overwrite);
    xTaskNotifyIndexed(main_task, 1, NOTIFICATION_മൂല്യം_1, e_Set_മൂല്യം_With_Overwrite);
    vTaskDelete(ശൂന്യ);
}
സ്ഥിതം TaskHandle_t notificator_task;

TEST_CASE("Notify to different indexes works", "[freertos]")
{
    uint32_t notification_മൂല്യം_0 = 0;
    uint32_t notification_മൂല്യം_1 = 0;

    TaskHandle_t main_task = xTaskGetCurrentTaskHandle();
    xTaskCreate(notify, "notificator", 2048, main_task, 2, &notificator_task);

    xTaskNotifyWaitIndexed(0, 0, 0xFFFFFFFF, &notification_മൂല്യം_0, pdMS_TO_TICKS(10));
    xTaskNotifyWaitIndexed(1, 0, 0xFFFFFFFF, &notification_മൂല്യം_1, pdMS_TO_TICKS(10));

    TEST_ASSERT_EQUAL(notification_മൂല്യം_0, NOTIFICATION_മൂല്യം_0);
    TEST_ASSERT_EQUAL(notification_മൂല്യം_1, NOTIFICATION_മൂല്യം_1);
}
#endif
