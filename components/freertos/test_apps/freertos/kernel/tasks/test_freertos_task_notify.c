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
#समावेश "sdkconfig.h"
#समावेश <stdio.h>
#समावेश <stdlib.h>
#समावेश "freertos/FreeRTOS.h"
#समावेश "freertos/task.h"
#समावेश "freertos/semphr.h"
#समावेश "driver/gptimer.h"
#समावेश "unity.h"
#समावेश "test_utils.h"

#अगर SOC_GPTIMER_SUPPORTED

#घोषणा NO_OF_NOTIFS    4
#घोषणा NO_OF_TASKS     2       //Sender and receiver
#घोषणा MESSAGE         0xFF

अटल uint32_t send_core_message = 0;
अटल TaskHandle_t recv_task_handle;
अटल bool isr_give = false;
अटल bool test_start = false;
अटल gptimer_handle_t gptimers[CONFIG_FREERTOS_संख्या_OF_CORES];
अटल SemaphoreHandle_t trigger_send_semphr;
अटल SemaphoreHandle_t task_delete_semphr;

//Test tracking vars
अटल अस्थिर uint32_t notifs_sent = 0;
अटल अस्थिर uint32_t notifs_rec = 0;
अटल bool wrong_core = false;

अटल bool on_alarm_sender_cb(gptimer_handle_t timer, स्थिर gptimer_alarm_event_तथ्य_t *edata, व्योम *user_ctx)
{
    gptimer_stop(timer);
    अगर (!test_start) {
        वापस false;
    }
    पूर्णांक curcore = xPortGetCoreID();
    अगर (isr_give) { //Test vTaskNotifyGiveFromISR() on same core
        notifs_sent++;
        vTaskNotifyGiveFromISR(recv_task_handle, शून्य);
    } अन्यथा { //Test xTaskNotifyFromISR()
        notifs_sent++;
        xTaskNotifyFromISR(recv_task_handle, (MESSAGE << curcore), e_Set_मूल्य_With_Overwrite, शून्य);
    }
    // always trigger a task switch when exit ISR context
    वापस true;
}

अटल व्योम test_gptimer_start(व्योम *arg)
{
    gptimer_handle_t gptimer = (gptimer_handle_t)arg;
    gptimer_alarm_config_t alarm_config = {
        .reload_गिनती = 0,
        .alarm_गिनती = 1000,
    };
    TEST_ESP_OK(gptimer_set_raw_गिनती(gptimer, 0));
    TEST_ESP_OK(gptimer_set_alarm_action(gptimer, &alarm_config));
    TEST_ESP_OK(gptimer_start(gptimer));
}

अटल व्योम sender_task(व्योम *arg)
{
    gptimer_handle_t gptimer = (gptimer_handle_t)arg;
    पूर्णांक curcore = xPortGetCoreID();

    //Test xTaskNotify
    xSemaphoreTake(trigger_send_semphr, portMAX_DELAY);
    notifs_sent++;
    xTaskNotify(recv_task_handle, (MESSAGE << curcore), e_Set_मूल्य_With_Overwrite);

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
    vTaskDelete(शून्य);
}

अटल व्योम receiver_task(व्योम *arg)
{
    uint32_t notify_मूल्य;

    //Test xTaskNotifyWait from task
    xTaskNotifyWait(0, 0xFFFFFFFF, &notify_मूल्य, portMAX_DELAY);
    अगर (notify_मूल्य != send_core_message) {
        wrong_core = true;
    }
    notifs_rec++;

    //Test ulTaskNotifyTake from task
    xSemaphoreGive(trigger_send_semphr);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    notifs_rec++;

    //Test xTaskNotifyWait from ISR
    xSemaphoreGive(trigger_send_semphr);
    xTaskNotifyWait(0, 0xFFFFFFFF, &notify_मूल्य, portMAX_DELAY);
    अगर (notify_मूल्य != send_core_message) {
        wrong_core = true;
    }
    notifs_rec++;

    //Test ulTaskNotifyTake from ISR
    xSemaphoreGive(trigger_send_semphr);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    notifs_rec++;

    //Test complete, stop timer and delete task
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(शून्य);
}

अटल व्योम install_gptimer_on_core(व्योम *arg)
{
    पूर्णांक core_id = (पूर्णांक)arg;
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_गिनती_UP,
        .resolution_hz = 1000000, // 1MHz, 1 tick = 1us
    };
    TEST_ESP_OK(gptimer_new_timer(&timer_config, &gptimers[core_id]));
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_alarm_sender_cb,
    };
    TEST_ESP_OK(gptimer_register_event_callbacks(gptimers[core_id], &cbs, शून्य));
    TEST_ESP_OK(gptimer_enable(gptimers[core_id]));
    test_gptimer_start(gptimers[core_id]);
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(शून्य);
}

TEST_CASE("Test Task_Notify", "[freertos]")
{
    test_start = false;
    trigger_send_semphr = x_Semaphore_Create_द्विआधारी();
    task_delete_semphr = xQueueCreateCountingSemaphore(10, 0);
    क्रम (पूर्णांक i = 0; i < CONFIG_FREERTOS_संख्या_OF_CORES; i++) {
        xTaskCreatePinnedToCore(install_gptimer_on_core, "install_gptimer", 4096, (व्योम *स्थिर)i, UNITY_FREERTOS_PRIORITY + 1, शून्य, i);
        TEST_ASSERT(xSemaphoreTake(task_delete_semphr, pdMS_TO_TICKS(1000)));
    }
    // wait the gptimer installation done on specific core
    vTaskDelay(10);
    // test start
    test_start = true;
    क्रम (पूर्णांक i = 0; i < CONFIG_FREERTOS_संख्या_OF_CORES; i++) { //Sending Core
        क्रम (पूर्णांक j = 0; j < CONFIG_FREERTOS_संख्या_OF_CORES; j++) { //Receiving Core
            //Reset Values
            notifs_sent = 0;
            notifs_rec = 0;
            wrong_core = false;
            send_core_message = (0xFF << i); //0xFF if core 0, 0xFF0 if core 1
            // receiver task has higher priority than sender task
            xTaskCreatePinnedToCore(receiver_task, "recv task", 1000, शून्य, UNITY_FREERTOS_PRIORITY + 2, &recv_task_handle, j);
            xTaskCreatePinnedToCore(sender_task, "send task", 1000, gptimers[i], UNITY_FREERTOS_PRIORITY + 1, शून्य, i);
            vTaskDelay(5);      //Wait for task creation to complete

            xSemaphoreGive(trigger_send_semphr);    //Trigger sender task
            क्रम (पूर्णांक k = 0; k < NO_OF_TASKS; k++) { //Wait for sender and receiver task deletion
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
    क्रम (पूर्णांक i = 0; i < CONFIG_FREERTOS_संख्या_OF_CORES; i++) {
        TEST_ESP_OK(gptimer_disable(gptimers[i]));
        TEST_ESP_OK(gptimer_del_timer(gptimers[i]));
    }
}
#पूर्ण_अगर //SOC_GPTIMER_SUPPORTED

/* Test causes asserts, so it cannot be run as a normal unity test case.
   Test case is ran as a separate test case in test_task_notify_too_high_index_fails
 */
TEST_CASE("Notify too high index fails", "[ignore]")
{
    uint32_t notification_मूल्य = 47;
    xTaskNotifyIndexed(xTaskGetCurrentTaskHandle(), 2, notification_मूल्य, eNoAction);
}

/* Test causes asserts, so it cannot be run as a normal unity test case.
   Test case is ran as a separate test case in test_task_notify_wait_too_high_index_fails
 */
TEST_CASE("Notify Wait too high index fails", "[ignore]")
{
    uint32_t notification_मूल्य;
    xTaskNotifyWaitIndexed(2, 0, 0, &notification_मूल्य, pdMS_TO_TICKS(10));
}

#अगर CONFIG_FREERTOS_TASK_NOTIFICATION_ARRAY_ENTRIES > 1
स्थिर uint32_t NOTIFICATION_मूल्य_0 = 47;
स्थिर uint32_t NOTIFICATION_मूल्य_1 = 48;

व्योम notify(व्योम *arg)
{
    TaskHandle_t main_task = (TaskHandle_t) arg;
    xTaskNotifyIndexed(main_task, 0, NOTIFICATION_मूल्य_0, e_Set_मूल्य_With_Overwrite);
    xTaskNotifyIndexed(main_task, 1, NOTIFICATION_मूल्य_1, e_Set_मूल्य_With_Overwrite);
    vTaskDelete(शून्य);
}
अटल TaskHandle_t notificator_task;

TEST_CASE("Notify to different indexes works", "[freertos]")
{
    uint32_t notification_मूल्य_0 = 0;
    uint32_t notification_मूल्य_1 = 0;

    TaskHandle_t main_task = xTaskGetCurrentTaskHandle();
    xTaskCreate(notify, "notificator", 2048, main_task, 2, &notificator_task);

    xTaskNotifyWaitIndexed(0, 0, 0xFFFFFFFF, &notification_मूल्य_0, pdMS_TO_TICKS(10));
    xTaskNotifyWaitIndexed(1, 0, 0xFFFFFFFF, &notification_मूल्य_1, pdMS_TO_TICKS(10));

    TEST_ASSERT_EQUAL(notification_मूल्य_0, NOTIFICATION_मूल्य_0);
    TEST_ASSERT_EQUAL(notification_मूल्य_1, NOTIFICATION_मूल्य_1);
}
#पूर्ण_अगर
