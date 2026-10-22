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
#અન્તર્ભાવ "sdkconfig.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ "freertos/FreeRTOS.h"
#અન્તર્ભાવ "freertos/task.h"
#અન્તર્ભાવ "freertos/semphr.h"
#અન્તર્ભાવ "driver/gptimer.h"
#અન્તર્ભાવ "unity.h"
#અન્તર્ભાવ "test_utils.h"

#જો SOC_GPTIMER_SUPPORTED

#define NO_OF_NOTIFS    4
#define NO_OF_TASKS     2       //Sender and receiver
#define MESSAGE         0xFF

સ્થિર uint32_t send_core_message = 0;
સ્થિર TaskHandle_t recv_task_handle;
સ્થિર bool isr_give = false;
સ્થિર bool test_start = false;
સ્થિર gptimer_handle_t gptimers[CONFIG_FREERTOS_સંખ્યા_OF_CORES];
સ્થિર SemaphoreHandle_t trigger_send_semphr;
સ્થિર SemaphoreHandle_t task_delete_semphr;

//Test tracking vars
સ્થિર અસ્થિર uint32_t notifs_sent = 0;
સ્થિર અસ્થિર uint32_t notifs_rec = 0;
સ્થિર bool wrong_core = false;

સ્થિર bool on_alarm_sender_cb(gptimer_handle_t timer, અચલ gptimer_alarm_event_data_t *edata, શૂન્ય *user_ctx)
{
    gptimer_stop(timer);
    જો (!test_start) {
        પાછા false;
    }
    પૂર્ણાંક curcore = xPortGetCoreID();
    જો (isr_give) { //Test vTaskNotifyGiveFromISR() on same core
        notifs_sent++;
        vTaskNotifyGiveFromISR(recv_task_handle, ખાલી);
    } નહીંતર { //Test xTaskNotifyFromISR()
        notifs_sent++;
        xTaskNotifyFromISR(recv_task_handle, (MESSAGE << curcore), e_Set_કિંમત_With_Overwrite, ખાલી);
    }
    // always trigger a task switch when exit ISR context
    પાછા true;
}

સ્થિર શૂન્ય test_gptimer_start(શૂન્ય *arg)
{
    gptimer_handle_t gptimer = (gptimer_handle_t)arg;
    gptimer_alarm_config_t alarm_config = {
        .reload_ગણતરી = 0,
        .alarm_ગણતરી = 1000,
    };
    TEST_ESP_OK(gptimer_set_raw_ગણતરી(gptimer, 0));
    TEST_ESP_OK(gptimer_set_alarm_action(gptimer, &alarm_config));
    TEST_ESP_OK(gptimer_start(gptimer));
}

સ્થિર શૂન્ય sender_task(શૂન્ય *arg)
{
    gptimer_handle_t gptimer = (gptimer_handle_t)arg;
    પૂર્ણાંક curcore = xPortGetCoreID();

    //Test xTaskNotify
    xSemaphoreTake(trigger_send_semphr, portMAX_DELAY);
    notifs_sent++;
    xTaskNotify(recv_task_handle, (MESSAGE << curcore), e_Set_કિંમત_With_Overwrite);

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
    vTaskDelete(ખાલી);
}

સ્થિર શૂન્ય receiver_task(શૂન્ય *arg)
{
    uint32_t notify_કિંમત;

    //Test xTaskNotifyWait from task
    xTaskNotifyWait(0, 0xFFFFFFFF, &notify_કિંમત, portMAX_DELAY);
    જો (notify_કિંમત != send_core_message) {
        wrong_core = true;
    }
    notifs_rec++;

    //Test ulTaskNotifyTake from task
    xSemaphoreGive(trigger_send_semphr);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    notifs_rec++;

    //Test xTaskNotifyWait from ISR
    xSemaphoreGive(trigger_send_semphr);
    xTaskNotifyWait(0, 0xFFFFFFFF, &notify_કિંમત, portMAX_DELAY);
    જો (notify_કિંમત != send_core_message) {
        wrong_core = true;
    }
    notifs_rec++;

    //Test ulTaskNotifyTake from ISR
    xSemaphoreGive(trigger_send_semphr);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    notifs_rec++;

    //Test complete, stop timer and delete task
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(ખાલી);
}

સ્થિર શૂન્ય install_gptimer_on_core(શૂન્ય *arg)
{
    પૂર્ણાંક core_id = (પૂર્ણાંક)arg;
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_ગણતરી_UP,
        .resolution_hz = 1000000, // 1MHz, 1 tick = 1us
    };
    TEST_ESP_OK(gptimer_new_timer(&timer_config, &gptimers[core_id]));
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_alarm_sender_cb,
    };
    TEST_ESP_OK(gptimer_register_event_callbacks(gptimers[core_id], &cbs, ખાલી));
    TEST_ESP_OK(gptimer_enable(gptimers[core_id]));
    test_gptimer_start(gptimers[core_id]);
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(ખાલી);
}

TEST_CASE("Test Task_Notify", "[freertos]")
{
    test_start = false;
    trigger_send_semphr = x_Semaphore_Create_દ્વિઅંકી();
    task_delete_semphr = xQueueCreateCountingSemaphore(10, 0);
    માટે (પૂર્ણાંક i = 0; i < CONFIG_FREERTOS_સંખ્યા_OF_CORES; i++) {
        xTaskCreatePinnedToCore(install_gptimer_on_core, "install_gptimer", 4096, (શૂન્ય *અચલ)i, UNITY_FREERTOS_PRIORITY + 1, ખાલી, i);
        TEST_ASSERT(xSemaphoreTake(task_delete_semphr, pdMS_TO_TICKS(1000)));
    }
    // wait the gptimer installation done on specific core
    vTaskDelay(10);
    // test start
    test_start = true;
    માટે (પૂર્ણાંક i = 0; i < CONFIG_FREERTOS_સંખ્યા_OF_CORES; i++) { //Sending Core
        માટે (પૂર્ણાંક j = 0; j < CONFIG_FREERTOS_સંખ્યા_OF_CORES; j++) { //Receiving Core
            //Reset Values
            notifs_sent = 0;
            notifs_rec = 0;
            wrong_core = false;
            send_core_message = (0xFF << i); //0xFF if core 0, 0xFF0 if core 1
            // receiver task has higher priority than sender task
            xTaskCreatePinnedToCore(receiver_task, "recv task", 1000, ખાલી, UNITY_FREERTOS_PRIORITY + 2, &recv_task_handle, j);
            xTaskCreatePinnedToCore(sender_task, "send task", 1000, gptimers[i], UNITY_FREERTOS_PRIORITY + 1, ખાલી, i);
            vTaskDelay(5);      //Wait for task creation to complete

            xSemaphoreGive(trigger_send_semphr);    //Trigger sender task
            માટે (પૂર્ણાંક k = 0; k < NO_OF_TASKS; k++) { //Wait for sender and receiver task deletion
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
    માટે (પૂર્ણાંક i = 0; i < CONFIG_FREERTOS_સંખ્યા_OF_CORES; i++) {
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
    uint32_t notification_કિંમત = 47;
    xTaskNotifyIndexed(xTaskGetCurrentTaskHandle(), 2, notification_કિંમત, eNoAction);
}

/* Test causes asserts, so it cannot be run as a normal unity test case.
   Test case is ran as a separate test case in test_task_notify_wait_too_high_index_fails
 */
TEST_CASE("Notify Wait too high index fails", "[ignore]")
{
    uint32_t notification_કિંમત;
    xTaskNotifyWaitIndexed(2, 0, 0, &notification_કિંમત, pdMS_TO_TICKS(10));
}

#જો CONFIG_FREERTOS_TASK_NOTIFICATION_ARRAY_ENTRIES > 1
અચલ uint32_t NOTIFICATION_કિંમત_0 = 47;
અચલ uint32_t NOTIFICATION_કિંમત_1 = 48;

શૂન્ય notify(શૂન્ય *arg)
{
    TaskHandle_t main_task = (TaskHandle_t) arg;
    xTaskNotifyIndexed(main_task, 0, NOTIFICATION_કિંમત_0, e_Set_કિંમત_With_Overwrite);
    xTaskNotifyIndexed(main_task, 1, NOTIFICATION_કિંમત_1, e_Set_કિંમત_With_Overwrite);
    vTaskDelete(ખાલી);
}
સ્થિર TaskHandle_t notificator_task;

TEST_CASE("Notify to different indexes works", "[freertos]")
{
    uint32_t notification_કિંમત_0 = 0;
    uint32_t notification_કિંમત_1 = 0;

    TaskHandle_t main_task = xTaskGetCurrentTaskHandle();
    xTaskCreate(notify, "notificator", 2048, main_task, 2, &notificator_task);

    xTaskNotifyWaitIndexed(0, 0, 0xFFFFFFFF, &notification_કિંમત_0, pdMS_TO_TICKS(10));
    xTaskNotifyWaitIndexed(1, 0, 0xFFFFFFFF, &notification_કિંમત_1, pdMS_TO_TICKS(10));

    TEST_ASSERT_EQUAL(notification_કિંમત_0, NOTIFICATION_કિંમત_0);
    TEST_ASSERT_EQUAL(notification_કિંમત_1, NOTIFICATION_કિંમત_1);
}
#endif
