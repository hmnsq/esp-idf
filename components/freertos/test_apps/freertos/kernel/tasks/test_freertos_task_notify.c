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
#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"
#अन्तर्भाव "freertos/semphr.h"
#अन्तर्भाव "driver/gptimer.h"
#अन्तर्भाव "unity.h"
#अन्तर्भाव "test_utils.h"

#यदि SOC_GPTIMER_SUPPORTED

#परिभाषय NO_सम्बन्धि_NOTIFS    4
#परिभाषय NO_सम्बन्धि_TASKS     2       //Sender and receiver
#परिभाषय सन्देशः_2         0xFF

स्थिर uint32_t send_core_सन्देशः = 0;
स्थिर TaskHandle_t recv_task_handle;
स्थिर bool isr_give = false;
स्थिर bool test_start = false;
स्थिर gptimer_handle_t gptimers[CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES];
स्थिर SemaphoreHandle_t trigger_send_semphr;
स्थिर SemaphoreHandle_t task_delete_semphr;

//Test tracking vars
स्थिर चञ्चल uint32_t notifs_प्रेषितम् = 0;
स्थिर चञ्चल uint32_t notifs_rec = 0;
स्थिर bool wrong_core = false;

स्थिर bool on_alarm_sender_cb(gptimer_handle_t timer, ध्रुव gptimer_alarm_event_दत्त_t *edata, शून्य *user_ctx)
{
    gptimer_stop(timer);
    यदि (!test_start) {
        प्रत्यागम false;
    }
    पूर्णाङ्क curcore = x_पत्तनम्_Get_Core_ID();
    यदि (isr_give) { //Test vTaskNotifyGiveFromISR() on same core
        notifs_प्रेषितम्++;
        vTaskNotifyGiveFromISR(recv_task_handle, रिक्त);
    } अन्यथा { //Test xTaskNotifyFromISR()
        notifs_प्रेषितम्++;
        xTaskNotifyFromISR(recv_task_handle, (सन्देशः_2 << curcore), e_समुच्चयः_मानम्_With_Overwrite, रिक्त);
    }
    // always trigger a task switch when exit ISR context
    प्रत्यागम true;
}

स्थिर शून्य test_gptimer_start(शून्य *arg)
{
    gptimer_handle_t gptimer = (gptimer_handle_t)arg;
    gptimer_alarm_config_t alarm_config = {
        .reload_गणनम् = 0,
        .alarm_गणनम् = 1000,
    };
    TEST_ESP_OK(gptimer_समुच्चयः_raw_गणनम्(gptimer, 0));
    TEST_ESP_OK(gptimer_समुच्चयः_alarm_action(gptimer, &alarm_config));
    TEST_ESP_OK(gptimer_start(gptimer));
}

स्थिर शून्य sender_task(शून्य *arg)
{
    gptimer_handle_t gptimer = (gptimer_handle_t)arg;
    पूर्णाङ्क curcore = x_पत्तनम्_Get_Core_ID();

    //Test xTaskNotify
    xSemaphoreTake(trigger_send_semphr, पत्तनम्_MAX_DELAY);
    notifs_प्रेषितम्++;
    xTaskNotify(recv_task_handle, (सन्देशः_2 << curcore), e_समुच्चयः_मानम्_With_Overwrite);

    //Test xTaskNotifyGive
    xSemaphoreTake(trigger_send_semphr, पत्तनम्_MAX_DELAY);
    notifs_प्रेषितम्++;
    xTaskNotifyGive(recv_task_handle);

    //Test xTaskNotifyFromISR
    xSemaphoreTake(trigger_send_semphr, पत्तनम्_MAX_DELAY);
    isr_give = false;
    test_gptimer_start(gptimer);

    //Test vTaskNotifyGiveFromISR
    xSemaphoreTake(trigger_send_semphr, पत्तनम्_MAX_DELAY);
    isr_give = true;
    test_gptimer_start(gptimer);

    //Delete Task and Semaphores
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(रिक्त);
}

स्थिर शून्य receiver_task(शून्य *arg)
{
    uint32_t notify_मानम्;

    //Test xTaskNotifyWait from task
    xTaskNotifyWait(0, 0xFFFFFFFF, &notify_मानम्, पत्तनम्_MAX_DELAY);
    यदि (notify_मानम् != send_core_सन्देशः) {
        wrong_core = true;
    }
    notifs_rec++;

    //Test ulTaskNotifyTake from task
    xSemaphoreGive(trigger_send_semphr);
    ulTaskNotifyTake(pdTRUE, पत्तनम्_MAX_DELAY);
    notifs_rec++;

    //Test xTaskNotifyWait from ISR
    xSemaphoreGive(trigger_send_semphr);
    xTaskNotifyWait(0, 0xFFFFFFFF, &notify_मानम्, पत्तनम्_MAX_DELAY);
    यदि (notify_मानम् != send_core_सन्देशः) {
        wrong_core = true;
    }
    notifs_rec++;

    //Test ulTaskNotifyTake from ISR
    xSemaphoreGive(trigger_send_semphr);
    ulTaskNotifyTake(pdTRUE, पत्तनम्_MAX_DELAY);
    notifs_rec++;

    //Test complete, stop timer and delete task
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(रिक्त);
}

स्थिर शून्य install_gptimer_on_core(शून्य *arg)
{
    पूर्णाङ्क core_id = (पूर्णाङ्क)arg;
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_गणनम्_ऊर्ध्वम्,
        .resolution_hz = 1000000, // 1MHz, 1 tick = 1us
    };
    TEST_ESP_OK(gptimer_new_timer(&timer_config, &gptimers[core_id]));
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_alarm_sender_cb,
    };
    TEST_ESP_OK(gptimer_पञ्जिकाकोशः_event_callbacks(gptimers[core_id], &cbs, रिक्त));
    TEST_ESP_OK(gptimer_enable(gptimers[core_id]));
    test_gptimer_start(gptimers[core_id]);
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(रिक्त);
}

TEST_CASE("Test Task_Notify", "[freertos]")
{
    test_start = false;
    trigger_send_semphr = x_Semaphore_Create_द्विमान();
    task_delete_semphr = xQueueCreateCountingSemaphore(10, 0);
    प्रति (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES; i++) {
        x_Task_Create_Pinned_गन्तव्यम्_Core(install_gptimer_on_core, "install_gptimer", 4096, (शून्य *ध्रुव)i, UNITY_FREERTOS_प्राथमिकता + 1, रिक्त, i);
        TEST_ASSERT(xSemaphoreTake(task_delete_semphr, pd_MS_गन्तव्यम्_TICKS(1000)));
    }
    // wait the gptimer installation done on specific core
    vTaskDelay(10);
    // test start
    test_start = true;
    प्रति (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES; i++) { //Sending Core
        प्रति (पूर्णाङ्क j = 0; j < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES; j++) { //Receiving Core
            //Reset Values
            notifs_प्रेषितम् = 0;
            notifs_rec = 0;
            wrong_core = false;
            send_core_सन्देशः = (0xFF << i); //0xFF if core 0, 0xFF0 if core 1
            // receiver task has higher priority than sender task
            x_Task_Create_Pinned_गन्तव्यम्_Core(receiver_task, "recv task", 1000, रिक्त, UNITY_FREERTOS_प्राथमिकता + 2, &recv_task_handle, j);
            x_Task_Create_Pinned_गन्तव्यम्_Core(sender_task, "send task", 1000, gptimers[i], UNITY_FREERTOS_प्राथमिकता + 1, रिक्त, i);
            vTaskDelay(5);      //Wait for task creation to complete

            xSemaphoreGive(trigger_send_semphr);    //Trigger sender task
            प्रति (पूर्णाङ्क k = 0; k < NO_सम्बन्धि_TASKS; k++) { //Wait for sender and receiver task deletion
                TEST_ASSERT(xSemaphoreTake(task_delete_semphr, pd_MS_गन्तव्यम्_TICKS(2000)));
            }
            vTaskDelay(5);      //Give time tasks to delete

            TEST_ASSERT_EQUAL(NO_सम्बन्धि_NOTIFS, notifs_प्रेषितम्);
            TEST_ASSERT_EQUAL(NO_सम्बन्धि_NOTIFS, notifs_rec);
            TEST_ASSERT_EQUAL(false, wrong_core);
        }
    }

    //Delete Semaphroes and timer ISRs
    vSemaphoreDelete(trigger_send_semphr);
    vSemaphoreDelete(task_delete_semphr);
    प्रति (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES; i++) {
        TEST_ESP_OK(gptimer_disable(gptimers[i]));
        TEST_ESP_OK(gptimer_del_timer(gptimers[i]));
    }
}
#यदि_अन्तः //SOC_GPTIMER_SUPPORTED

/* Test causes asserts, so it cannot be run as a normal unity test case.
   Test case is ran as a separate test case in test_task_notify_too_high_index_fails
 */
TEST_CASE("Notify too high index fails", "[ignore]")
{
    uint32_t notification_मानम् = 47;
    x_Task_Notify_अनुक्रमितम्(xTaskGetCurrentTaskHandle(), 2, notification_मानम्, eNoAction);
}

/* Test causes asserts, so it cannot be run as a normal unity test case.
   Test case is ran as a separate test case in test_task_notify_wait_too_high_index_fails
 */
TEST_CASE("Notify Wait too high index fails", "[ignore]")
{
    uint32_t notification_मानम्;
    x_Task_Notify_Wait_अनुक्रमितम्(2, 0, 0, &notification_मानम्, pd_MS_गन्तव्यम्_TICKS(10));
}

#यदि CONFIG_FREERTOS_TASK_NOTIFICATION_ARRAY_ENTRIES > 1
ध्रुव uint32_t NOTIFICATION_मानम्_0 = 47;
ध्रुव uint32_t NOTIFICATION_मानम्_1 = 48;

शून्य notify(शून्य *arg)
{
    TaskHandle_t main_task = (TaskHandle_t) arg;
    x_Task_Notify_अनुक्रमितम्(main_task, 0, NOTIFICATION_मानम्_0, e_समुच्चयः_मानम्_With_Overwrite);
    x_Task_Notify_अनुक्रमितम्(main_task, 1, NOTIFICATION_मानम्_1, e_समुच्चयः_मानम्_With_Overwrite);
    vTaskDelete(रिक्त);
}
स्थिर TaskHandle_t notificator_task;

TEST_CASE("Notify to different indexes works", "[freertos]")
{
    uint32_t notification_मानम्_0 = 0;
    uint32_t notification_मानम्_1 = 0;

    TaskHandle_t main_task = xTaskGetCurrentTaskHandle();
    xTaskCreate(notify, "notificator", 2048, main_task, 2, &notificator_task);

    x_Task_Notify_Wait_अनुक्रमितम्(0, 0, 0xFFFFFFFF, &notification_मानम्_0, pd_MS_गन्तव्यम्_TICKS(10));
    x_Task_Notify_Wait_अनुक्रमितम्(1, 0, 0xFFFFFFFF, &notification_मानम्_1, pd_MS_गन्तव्यम्_TICKS(10));

    TEST_ASSERT_EQUAL(notification_मानम्_0, NOTIFICATION_मानम्_0);
    TEST_ASSERT_EQUAL(notification_मानम्_1, NOTIFICATION_मानम्_1);
}
#यदि_अन्तः
