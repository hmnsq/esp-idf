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
#ଅନ୍ତର୍ଭାଵ "sdkconfig.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ "freertos/FreeRTOS.h"
#ଅନ୍ତର୍ଭାଵ "freertos/task.h"
#ଅନ୍ତର୍ଭାଵ "freertos/semphr.h"
#ଅନ୍ତର୍ଭାଵ "driver/gptimer.h"
#ଅନ୍ତର୍ଭାଵ "unity.h"
#ଅନ୍ତର୍ଭାଵ "test_utils.h"

#ଯଦି SOC_GPTIMER_SUPPORTED

#define NO_OF_NOTIFS    4
#define NO_OF_TASKS     2       //Sender and receiver
#define MESSAGE         0xFF

ସ୍ଥିର uint32_t send_core_message = 0;
ସ୍ଥିର TaskHandle_t recv_task_handle;
ସ୍ଥିର bool isr_give = false;
ସ୍ଥିର bool test_start = false;
ସ୍ଥିର gptimer_handle_t gptimers[CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES];
ସ୍ଥିର SemaphoreHandle_t trigger_send_semphr;
ସ୍ଥିର SemaphoreHandle_t task_delete_semphr;

//Test tracking vars
ସ୍ଥିର ଅସ୍ଥିର uint32_t notifs_sent = 0;
ସ୍ଥିର ଅସ୍ଥିର uint32_t notifs_rec = 0;
ସ୍ଥିର bool wrong_core = false;

ସ୍ଥିର bool on_alarm_sender_cb(gptimer_handle_t timer, ଅଚଳ gptimer_alarm_event_data_t *edata, ଶୂନ୍ୟ *user_ctx)
{
    gptimer_stop(timer);
    ଯଦି (!test_start) {
        ଫେରନ୍ତୁ false;
    }
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା curcore = xPortGetCoreID();
    ଯଦି (isr_give) { //Test vTaskNotifyGiveFromISR() on same core
        notifs_sent++;
        vTaskNotifyGiveFromISR(recv_task_handle, ଖାଲି);
    } ଅନ୍ୟଥା { //Test xTaskNotifyFromISR()
        notifs_sent++;
        xTaskNotifyFromISR(recv_task_handle, (MESSAGE << curcore), e_Set_ମୂଲ୍ୟ_With_Overwrite, ଖାଲି);
    }
    // always trigger a task switch when exit ISR context
    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଶୂନ୍ୟ test_gptimer_start(ଶୂନ୍ୟ *arg)
{
    gptimer_handle_t gptimer = (gptimer_handle_t)arg;
    gptimer_alarm_config_t alarm_config = {
        .reload_ଗଣନା = 0,
        .alarm_ଗଣନା = 1000,
    };
    TEST_ESP_OK(gptimer_set_raw_ଗଣନା(gptimer, 0));
    TEST_ESP_OK(gptimer_set_alarm_action(gptimer, &alarm_config));
    TEST_ESP_OK(gptimer_start(gptimer));
}

ସ୍ଥିର ଶୂନ୍ୟ sender_task(ଶୂନ୍ୟ *arg)
{
    gptimer_handle_t gptimer = (gptimer_handle_t)arg;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା curcore = xPortGetCoreID();

    //Test xTaskNotify
    xSemaphoreTake(trigger_send_semphr, portMAX_DELAY);
    notifs_sent++;
    xTaskNotify(recv_task_handle, (MESSAGE << curcore), e_Set_ମୂଲ୍ୟ_With_Overwrite);

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
    vTaskDelete(ଖାଲି);
}

ସ୍ଥିର ଶୂନ୍ୟ receiver_task(ଶୂନ୍ୟ *arg)
{
    uint32_t notify_ମୂଲ୍ୟ;

    //Test xTaskNotifyWait from task
    xTaskNotifyWait(0, 0xFFFFFFFF, &notify_ମୂଲ୍ୟ, portMAX_DELAY);
    ଯଦି (notify_ମୂଲ୍ୟ != send_core_message) {
        wrong_core = true;
    }
    notifs_rec++;

    //Test ulTaskNotifyTake from task
    xSemaphoreGive(trigger_send_semphr);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    notifs_rec++;

    //Test xTaskNotifyWait from ISR
    xSemaphoreGive(trigger_send_semphr);
    xTaskNotifyWait(0, 0xFFFFFFFF, &notify_ମୂଲ୍ୟ, portMAX_DELAY);
    ଯଦି (notify_ମୂଲ୍ୟ != send_core_message) {
        wrong_core = true;
    }
    notifs_rec++;

    //Test ulTaskNotifyTake from ISR
    xSemaphoreGive(trigger_send_semphr);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    notifs_rec++;

    //Test complete, stop timer and delete task
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(ଖାଲି);
}

ସ୍ଥିର ଶୂନ୍ୟ install_gptimer_on_core(ଶୂନ୍ୟ *arg)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା core_id = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)arg;
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_ଗଣନା_UP,
        .resolution_hz = 1000000, // 1MHz, 1 tick = 1us
    };
    TEST_ESP_OK(gptimer_new_timer(&timer_config, &gptimers[core_id]));
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_alarm_sender_cb,
    };
    TEST_ESP_OK(gptimer_register_event_callbacks(gptimers[core_id], &cbs, ଖାଲି));
    TEST_ESP_OK(gptimer_enable(gptimers[core_id]));
    test_gptimer_start(gptimers[core_id]);
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(ଖାଲି);
}

TEST_CASE("Test Task_Notify", "[freertos]")
{
    test_start = false;
    trigger_send_semphr = x_Semaphore_Create_ଦ୍ୱିମିକ();
    task_delete_semphr = xQueueCreateCountingSemaphore(10, 0);
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; i++) {
        xTaskCreatePinnedToCore(install_gptimer_on_core, "install_gptimer", 4096, (ଶୂନ୍ୟ *ଅଚଳ)i, UNITY_FREERTOS_PRIORITY + 1, ଖାଲି, i);
        TEST_ASSERT(xSemaphoreTake(task_delete_semphr, pdMS_TO_TICKS(1000)));
    }
    // wait the gptimer installation done on specific core
    vTaskDelay(10);
    // test start
    test_start = true;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; i++) { //Sending Core
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; j++) { //Receiving Core
            //Reset Values
            notifs_sent = 0;
            notifs_rec = 0;
            wrong_core = false;
            send_core_message = (0xFF << i); //0xFF if core 0, 0xFF0 if core 1
            // receiver task has higher priority than sender task
            xTaskCreatePinnedToCore(receiver_task, "recv task", 1000, ଖାଲି, UNITY_FREERTOS_PRIORITY + 2, &recv_task_handle, j);
            xTaskCreatePinnedToCore(sender_task, "send task", 1000, gptimers[i], UNITY_FREERTOS_PRIORITY + 1, ଖାଲି, i);
            vTaskDelay(5);      //Wait for task creation to complete

            xSemaphoreGive(trigger_send_semphr);    //Trigger sender task
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < NO_OF_TASKS; k++) { //Wait for sender and receiver task deletion
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
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; i++) {
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
    uint32_t notification_ମୂଲ୍ୟ = 47;
    xTaskNotifyIndexed(xTaskGetCurrentTaskHandle(), 2, notification_ମୂଲ୍ୟ, eNoAction);
}

/* Test causes asserts, so it cannot be run as a normal unity test case.
   Test case is ran as a separate test case in test_task_notify_wait_too_high_index_fails
 */
TEST_CASE("Notify Wait too high index fails", "[ignore]")
{
    uint32_t notification_ମୂଲ୍ୟ;
    xTaskNotifyWaitIndexed(2, 0, 0, &notification_ମୂଲ୍ୟ, pdMS_TO_TICKS(10));
}

#ଯଦି CONFIG_FREERTOS_TASK_NOTIFICATION_ARRAY_ENTRIES > 1
ଅଚଳ uint32_t NOTIFICATION_ମୂଲ୍ୟ_0 = 47;
ଅଚଳ uint32_t NOTIFICATION_ମୂଲ୍ୟ_1 = 48;

ଶୂନ୍ୟ notify(ଶୂନ୍ୟ *arg)
{
    TaskHandle_t main_task = (TaskHandle_t) arg;
    xTaskNotifyIndexed(main_task, 0, NOTIFICATION_ମୂଲ୍ୟ_0, e_Set_ମୂଲ୍ୟ_With_Overwrite);
    xTaskNotifyIndexed(main_task, 1, NOTIFICATION_ମୂଲ୍ୟ_1, e_Set_ମୂଲ୍ୟ_With_Overwrite);
    vTaskDelete(ଖାଲି);
}
ସ୍ଥିର TaskHandle_t notificator_task;

TEST_CASE("Notify to different indexes works", "[freertos]")
{
    uint32_t notification_ମୂଲ୍ୟ_0 = 0;
    uint32_t notification_ମୂଲ୍ୟ_1 = 0;

    TaskHandle_t main_task = xTaskGetCurrentTaskHandle();
    xTaskCreate(notify, "notificator", 2048, main_task, 2, &notificator_task);

    xTaskNotifyWaitIndexed(0, 0, 0xFFFFFFFF, &notification_ମୂଲ୍ୟ_0, pdMS_TO_TICKS(10));
    xTaskNotifyWaitIndexed(1, 0, 0xFFFFFFFF, &notification_ମୂଲ୍ୟ_1, pdMS_TO_TICKS(10));

    TEST_ASSERT_EQUAL(notification_ମୂଲ୍ୟ_0, NOTIFICATION_ମୂଲ୍ୟ_0);
    TEST_ASSERT_EQUAL(notification_ମୂଲ୍ୟ_1, NOTIFICATION_ମୂଲ୍ୟ_1);
}
#endif
