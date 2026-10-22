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
#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/semphr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "driver/gptimer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "test_utils.h"

#ਜੇ SOC_GPTIMER_SUPPORTED

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NO_ਦਾ_NOTIFS    4
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NO_ਦਾ_TASKS     2       //Sender and receiver
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸੁਨੇਹਾ_2         0xFF

ਸਥਿਰ uint32_t send_core_ਸੁਨੇਹਾ = 0;
ਸਥਿਰ TaskHandle_t recv_task_handle;
ਸਥਿਰ bool isr_give = false;
ਸਥਿਰ bool test_ਸ਼ੁਰੂ = false;
ਸਥਿਰ gptimer_handle_t gptimers[CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES];
ਸਥਿਰ SemaphoreHandle_t trigger_send_semphr;
ਸਥਿਰ SemaphoreHandle_t task_delete_semphr;

//Test tracking vars
ਸਥਿਰ ਅਸਥਿਰ uint32_t notifs_ਭੇਜਿਆ = 0;
ਸਥਿਰ ਅਸਥਿਰ uint32_t notifs_rec = 0;
ਸਥਿਰ bool wrong_core = false;

ਸਥਿਰ bool on_alarm_sender_cb(gptimer_handle_t timer, ਅਚਲ gptimer_alarm_event_ਅੰਕੜੇ_t *edata, ਖਾਲੀ *user_ctx)
{
    gptimer_stop(timer);
    ਜੇ (!test_ਸ਼ੁਰੂ) {
        ਵਾਪਸ false;
    }
    ਪੂਰਨ curcore = x_ਪੋਰਟ_Get_Core_ID();
    ਜੇ (isr_give) { //Test vTaskNotifyGiveFromISR() on same core
        notifs_ਭੇਜਿਆ++;
        vTaskNotifyGiveFromISR(recv_task_handle, ਨਿਰਾ);
    } ਨਹੀਂਤਾਂ { //Test xTaskNotifyFromISR()
        notifs_ਭੇਜਿਆ++;
        xTaskNotifyFromISR(recv_task_handle, (ਸੁਨੇਹਾ_2 << curcore), e_ਸੈੱਟ_ਮੁੱਲ_With_Overwrite, ਨਿਰਾ);
    }
    // always trigger a task switch when exit ISR context
    ਵਾਪਸ true;
}

ਸਥਿਰ ਖਾਲੀ test_gptimer_ਸ਼ੁਰੂ(ਖਾਲੀ *arg)
{
    gptimer_handle_t gptimer = (gptimer_handle_t)arg;
    gptimer_alarm_config_t alarm_config = {
        .reload_ਗਿਣਤੀ = 0,
        .alarm_ਗਿਣਤੀ = 1000,
    };
    TEST_ESP_OK(gptimer_ਸੈੱਟ_raw_ਗਿਣਤੀ(gptimer, 0));
    TEST_ESP_OK(gptimer_ਸੈੱਟ_alarm_action(gptimer, &alarm_config));
    TEST_ESP_OK(gptimer_ਸ਼ੁਰੂ(gptimer));
}

ਸਥਿਰ ਖਾਲੀ sender_task(ਖਾਲੀ *arg)
{
    gptimer_handle_t gptimer = (gptimer_handle_t)arg;
    ਪੂਰਨ curcore = x_ਪੋਰਟ_Get_Core_ID();

    //Test xTaskNotify
    xSemaphoreTake(trigger_send_semphr, ਪੋਰਟ_MAX_DELAY);
    notifs_ਭੇਜਿਆ++;
    xTaskNotify(recv_task_handle, (ਸੁਨੇਹਾ_2 << curcore), e_ਸੈੱਟ_ਮੁੱਲ_With_Overwrite);

    //Test xTaskNotifyGive
    xSemaphoreTake(trigger_send_semphr, ਪੋਰਟ_MAX_DELAY);
    notifs_ਭੇਜਿਆ++;
    xTaskNotifyGive(recv_task_handle);

    //Test xTaskNotifyFromISR
    xSemaphoreTake(trigger_send_semphr, ਪੋਰਟ_MAX_DELAY);
    isr_give = false;
    test_gptimer_ਸ਼ੁਰੂ(gptimer);

    //Test vTaskNotifyGiveFromISR
    xSemaphoreTake(trigger_send_semphr, ਪੋਰਟ_MAX_DELAY);
    isr_give = true;
    test_gptimer_ਸ਼ੁਰੂ(gptimer);

    //Delete Task and Semaphores
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ receiver_task(ਖਾਲੀ *arg)
{
    uint32_t notify_ਮੁੱਲ;

    //Test xTaskNotifyWait from task
    xTaskNotifyWait(0, 0xFFFFFFFF, &notify_ਮੁੱਲ, ਪੋਰਟ_MAX_DELAY);
    ਜੇ (notify_ਮੁੱਲ != send_core_ਸੁਨੇਹਾ) {
        wrong_core = true;
    }
    notifs_rec++;

    //Test ulTaskNotifyTake from task
    xSemaphoreGive(trigger_send_semphr);
    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);
    notifs_rec++;

    //Test xTaskNotifyWait from ISR
    xSemaphoreGive(trigger_send_semphr);
    xTaskNotifyWait(0, 0xFFFFFFFF, &notify_ਮੁੱਲ, ਪੋਰਟ_MAX_DELAY);
    ਜੇ (notify_ਮੁੱਲ != send_core_ਸੁਨੇਹਾ) {
        wrong_core = true;
    }
    notifs_rec++;

    //Test ulTaskNotifyTake from ISR
    xSemaphoreGive(trigger_send_semphr);
    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);
    notifs_rec++;

    //Test complete, stop timer and delete task
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ install_gptimer_on_core(ਖਾਲੀ *arg)
{
    ਪੂਰਨ core_id = (ਪੂਰਨ)arg;
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_ਗਿਣਤੀ_ਉੱਪਰ,
        .resolution_hz = 1000000, // 1MHz, 1 tick = 1us
    };
    TEST_ESP_OK(gptimer_new_timer(&timer_config, &gptimers[core_id]));
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_alarm_sender_cb,
    };
    TEST_ESP_OK(gptimer_ਰਜਿਸਟਰ_event_callbacks(gptimers[core_id], &cbs, ਨਿਰਾ));
    TEST_ESP_OK(gptimer_enable(gptimers[core_id]));
    test_gptimer_ਸ਼ੁਰੂ(gptimers[core_id]);
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(ਨਿਰਾ);
}

TEST_CASE("Test Task_Notify", "[freertos]")
{
    test_ਸ਼ੁਰੂ = false;
    trigger_send_semphr = x_Semaphore_Create_ਦੁਆਧਾਰੀ();
    task_delete_semphr = x_ਕਤਾਰ_Create_Counting_Semaphore(10, 0);
    ਲਈ (ਪੂਰਨ i = 0; i < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; i++) {
        x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(install_gptimer_on_core, "install_gptimer", 4096, (ਖਾਲੀ *ਅਚਲ)i, UNITY_FREERTOS_ਤਰਜੀਹ + 1, ਨਿਰਾ, i);
        TEST_ASSERT(xSemaphoreTake(task_delete_semphr, pd_MS_ਮੰਜ਼ਿਲ_TICKS(1000)));
    }
    // wait the gptimer installation done on specific core
    vTaskDelay(10);
    // test start
    test_ਸ਼ੁਰੂ = true;
    ਲਈ (ਪੂਰਨ i = 0; i < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; i++) { //Sending Core
        ਲਈ (ਪੂਰਨ j = 0; j < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; j++) { //Receiving Core
            //Reset Values
            notifs_ਭੇਜਿਆ = 0;
            notifs_rec = 0;
            wrong_core = false;
            send_core_ਸੁਨੇਹਾ = (0xFF << i); //0xFF if core 0, 0xFF0 if core 1
            // receiver task has higher priority than sender task
            x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(receiver_task, "recv task", 1000, ਨਿਰਾ, UNITY_FREERTOS_ਤਰਜੀਹ + 2, &recv_task_handle, j);
            x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(sender_task, "send task", 1000, gptimers[i], UNITY_FREERTOS_ਤਰਜੀਹ + 1, ਨਿਰਾ, i);
            vTaskDelay(5);      //Wait for task creation to complete

            xSemaphoreGive(trigger_send_semphr);    //Trigger sender task
            ਲਈ (ਪੂਰਨ k = 0; k < NO_ਦਾ_TASKS; k++) { //Wait for sender and receiver task deletion
                TEST_ASSERT(xSemaphoreTake(task_delete_semphr, pd_MS_ਮੰਜ਼ਿਲ_TICKS(2000)));
            }
            vTaskDelay(5);      //Give time tasks to delete

            TEST_ASSERT_EQUAL(NO_ਦਾ_NOTIFS, notifs_ਭੇਜਿਆ);
            TEST_ASSERT_EQUAL(NO_ਦਾ_NOTIFS, notifs_rec);
            TEST_ASSERT_EQUAL(false, wrong_core);
        }
    }

    //Delete Semaphroes and timer ISRs
    vSemaphoreDelete(trigger_send_semphr);
    vSemaphoreDelete(task_delete_semphr);
    ਲਈ (ਪੂਰਨ i = 0; i < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; i++) {
        TEST_ESP_OK(gptimer_disable(gptimers[i]));
        TEST_ESP_OK(gptimer_del_timer(gptimers[i]));
    }
}
#ਜੇ_ਅੰਤ //SOC_GPTIMER_SUPPORTED

/* Test causes asserts, so it cannot be run as a normal unity test case.
   Test case is ran as a separate test case in test_task_notify_too_high_index_fails
 */
TEST_CASE("Notify too high index fails", "[ignore]")
{
    uint32_t notification_ਮੁੱਲ = 47;
    x_Task_Notify_ਸੂਚਕਾਂਕਿਤ(xTaskGetCurrentTaskHandle(), 2, notification_ਮੁੱਲ, eNoAction);
}

/* Test causes asserts, so it cannot be run as a normal unity test case.
   Test case is ran as a separate test case in test_task_notify_wait_too_high_index_fails
 */
TEST_CASE("Notify Wait too high index fails", "[ignore]")
{
    uint32_t notification_ਮੁੱਲ;
    x_Task_Notify_Wait_ਸੂਚਕਾਂਕਿਤ(2, 0, 0, &notification_ਮੁੱਲ, pd_MS_ਮੰਜ਼ਿਲ_TICKS(10));
}

#ਜੇ CONFIG_FREERTOS_TASK_NOTIFICATION_ARRAY_ENTRIES > 1
ਅਚਲ uint32_t NOTIFICATION_ਮੁੱਲ_0 = 47;
ਅਚਲ uint32_t NOTIFICATION_ਮੁੱਲ_1 = 48;

ਖਾਲੀ notify(ਖਾਲੀ *arg)
{
    TaskHandle_t main_task = (TaskHandle_t) arg;
    x_Task_Notify_ਸੂਚਕਾਂਕਿਤ(main_task, 0, NOTIFICATION_ਮੁੱਲ_0, e_ਸੈੱਟ_ਮੁੱਲ_With_Overwrite);
    x_Task_Notify_ਸੂਚਕਾਂਕਿਤ(main_task, 1, NOTIFICATION_ਮੁੱਲ_1, e_ਸੈੱਟ_ਮੁੱਲ_With_Overwrite);
    vTaskDelete(ਨਿਰਾ);
}
ਸਥਿਰ TaskHandle_t notificator_task;

TEST_CASE("Notify to different indexes works", "[freertos]")
{
    uint32_t notification_ਮੁੱਲ_0 = 0;
    uint32_t notification_ਮੁੱਲ_1 = 0;

    TaskHandle_t main_task = xTaskGetCurrentTaskHandle();
    xTaskCreate(notify, "notificator", 2048, main_task, 2, &notificator_task);

    x_Task_Notify_Wait_ਸੂਚਕਾਂਕਿਤ(0, 0, 0xFFFFFFFF, &notification_ਮੁੱਲ_0, pd_MS_ਮੰਜ਼ਿਲ_TICKS(10));
    x_Task_Notify_Wait_ਸੂਚਕਾਂਕਿਤ(1, 0, 0xFFFFFFFF, &notification_ਮੁੱਲ_1, pd_MS_ਮੰਜ਼ਿਲ_TICKS(10));

    TEST_ASSERT_EQUAL(notification_ਮੁੱਲ_0, NOTIFICATION_ਮੁੱਲ_0);
    TEST_ASSERT_EQUAL(notification_ਮੁੱਲ_1, NOTIFICATION_ਮੁੱਲ_1);
}
#ਜੇ_ਅੰਤ
