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
#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "freertos/semphr.h"
#అన్తర్భావ "driver/gptimer.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "test_utils.h"

#ఒకవేళ SOC_GPTIMER_SUPPORTED

#నిర్వచించు NO_యొక్క_NOTIFS    4
#నిర్వచించు NO_యొక్క_TASKS     2       //Sender and receiver
#నిర్వచించు సందేశం_2         0xFF

స్థిర uint32_t send_core_సందేశం = 0;
స్థిర TaskHandle_t recv_task_handle;
స్థిర bool isr_give = false;
స్థిర bool test_ప్రారంభం = false;
స్థిర gptimer_handle_t gptimers[CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES];
స్థిర SemaphoreHandle_t trigger_send_semphr;
స్థిర SemaphoreHandle_t task_delete_semphr;

//Test tracking vars
స్థిర అస్థిర uint32_t notifs_పంపిన = 0;
స్థిర అస్థిర uint32_t notifs_rec = 0;
స్థిర bool wrong_core = false;

స్థిర bool on_alarm_sender_cb(gptimer_handle_t timer, స్థిరరాశి gptimer_alarm_event_దత్తాంశం_t *edata, శూన్య *user_ctx)
{
    gptimer_stop(timer);
    ఒకవేళ (!test_ప్రారంభం) {
        తిరిగి false;
    }
    పూర్ణాంకం curcore = x_పోర్టు_Get_Core_ID();
    ఒకవేళ (isr_give) { //Test vTaskNotifyGiveFromISR() on same core
        notifs_పంపిన++;
        vTaskNotifyGiveFromISR(recv_task_handle, ఖాళీ);
    } లేకపోతే { //Test xTaskNotifyFromISR()
        notifs_పంపిన++;
        xTaskNotifyFromISR(recv_task_handle, (సందేశం_2 << curcore), e_సమితి_విలువ_With_Overwrite, ఖాళీ);
    }
    // always trigger a task switch when exit ISR context
    తిరిగి true;
}

స్థిర శూన్య test_gptimer_ప్రారంభం(శూన్య *arg)
{
    gptimer_handle_t gptimer = (gptimer_handle_t)arg;
    gptimer_alarm_config_t alarm_config = {
        .reload_లెక్క = 0,
        .alarm_లెక్క = 1000,
    };
    TEST_ESP_OK(gptimer_సమితి_raw_లెక్క(gptimer, 0));
    TEST_ESP_OK(gptimer_సమితి_alarm_action(gptimer, &alarm_config));
    TEST_ESP_OK(gptimer_ప్రారంభం(gptimer));
}

స్థిర శూన్య sender_task(శూన్య *arg)
{
    gptimer_handle_t gptimer = (gptimer_handle_t)arg;
    పూర్ణాంకం curcore = x_పోర్టు_Get_Core_ID();

    //Test xTaskNotify
    xSemaphoreTake(trigger_send_semphr, పోర్టు_MAX_DELAY);
    notifs_పంపిన++;
    xTaskNotify(recv_task_handle, (సందేశం_2 << curcore), e_సమితి_విలువ_With_Overwrite);

    //Test xTaskNotifyGive
    xSemaphoreTake(trigger_send_semphr, పోర్టు_MAX_DELAY);
    notifs_పంపిన++;
    xTaskNotifyGive(recv_task_handle);

    //Test xTaskNotifyFromISR
    xSemaphoreTake(trigger_send_semphr, పోర్టు_MAX_DELAY);
    isr_give = false;
    test_gptimer_ప్రారంభం(gptimer);

    //Test vTaskNotifyGiveFromISR
    xSemaphoreTake(trigger_send_semphr, పోర్టు_MAX_DELAY);
    isr_give = true;
    test_gptimer_ప్రారంభం(gptimer);

    //Delete Task and Semaphores
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(ఖాళీ);
}

స్థిర శూన్య receiver_task(శూన్య *arg)
{
    uint32_t notify_విలువ;

    //Test xTaskNotifyWait from task
    xTaskNotifyWait(0, 0xFFFFFFFF, &notify_విలువ, పోర్టు_MAX_DELAY);
    ఒకవేళ (notify_విలువ != send_core_సందేశం) {
        wrong_core = true;
    }
    notifs_rec++;

    //Test ulTaskNotifyTake from task
    xSemaphoreGive(trigger_send_semphr);
    ulTaskNotifyTake(pdTRUE, పోర్టు_MAX_DELAY);
    notifs_rec++;

    //Test xTaskNotifyWait from ISR
    xSemaphoreGive(trigger_send_semphr);
    xTaskNotifyWait(0, 0xFFFFFFFF, &notify_విలువ, పోర్టు_MAX_DELAY);
    ఒకవేళ (notify_విలువ != send_core_సందేశం) {
        wrong_core = true;
    }
    notifs_rec++;

    //Test ulTaskNotifyTake from ISR
    xSemaphoreGive(trigger_send_semphr);
    ulTaskNotifyTake(pdTRUE, పోర్టు_MAX_DELAY);
    notifs_rec++;

    //Test complete, stop timer and delete task
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(ఖాళీ);
}

స్థిర శూన్య install_gptimer_on_core(శూన్య *arg)
{
    పూర్ణాంకం core_id = (పూర్ణాంకం)arg;
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_లెక్క_పైకి,
        .resolution_hz = 1000000, // 1MHz, 1 tick = 1us
    };
    TEST_ESP_OK(gptimer_new_timer(&timer_config, &gptimers[core_id]));
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_alarm_sender_cb,
    };
    TEST_ESP_OK(gptimer_రిజిస్టరు_event_callbacks(gptimers[core_id], &cbs, ఖాళీ));
    TEST_ESP_OK(gptimer_enable(gptimers[core_id]));
    test_gptimer_ప్రారంభం(gptimers[core_id]);
    xSemaphoreGive(task_delete_semphr);
    vTaskDelete(ఖాళీ);
}

TEST_CASE("Test Task_Notify", "[freertos]")
{
    test_ప్రారంభం = false;
    trigger_send_semphr = x_Semaphore_Create_ద్విమాన();
    task_delete_semphr = x_వరుస_Create_Counting_Semaphore(10, 0);
    కొరకు (పూర్ణాంకం i = 0; i < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; i++) {
        x_Task_Create_Pinned_గమ్యం_Core(install_gptimer_on_core, "install_gptimer", 4096, (శూన్య *స్థిరరాశి)i, UNITY_FREERTOS_ప్రాధాన్యత + 1, ఖాళీ, i);
        TEST_ASSERT(xSemaphoreTake(task_delete_semphr, pd_MS_గమ్యం_TICKS(1000)));
    }
    // wait the gptimer installation done on specific core
    vTaskDelay(10);
    // test start
    test_ప్రారంభం = true;
    కొరకు (పూర్ణాంకం i = 0; i < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; i++) { //Sending Core
        కొరకు (పూర్ణాంకం j = 0; j < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; j++) { //Receiving Core
            //Reset Values
            notifs_పంపిన = 0;
            notifs_rec = 0;
            wrong_core = false;
            send_core_సందేశం = (0xFF << i); //0xFF if core 0, 0xFF0 if core 1
            // receiver task has higher priority than sender task
            x_Task_Create_Pinned_గమ్యం_Core(receiver_task, "recv task", 1000, ఖాళీ, UNITY_FREERTOS_ప్రాధాన్యత + 2, &recv_task_handle, j);
            x_Task_Create_Pinned_గమ్యం_Core(sender_task, "send task", 1000, gptimers[i], UNITY_FREERTOS_ప్రాధాన్యత + 1, ఖాళీ, i);
            vTaskDelay(5);      //Wait for task creation to complete

            xSemaphoreGive(trigger_send_semphr);    //Trigger sender task
            కొరకు (పూర్ణాంకం k = 0; k < NO_యొక్క_TASKS; k++) { //Wait for sender and receiver task deletion
                TEST_ASSERT(xSemaphoreTake(task_delete_semphr, pd_MS_గమ్యం_TICKS(2000)));
            }
            vTaskDelay(5);      //Give time tasks to delete

            TEST_ASSERT_EQUAL(NO_యొక్క_NOTIFS, notifs_పంపిన);
            TEST_ASSERT_EQUAL(NO_యొక్క_NOTIFS, notifs_rec);
            TEST_ASSERT_EQUAL(false, wrong_core);
        }
    }

    //Delete Semaphroes and timer ISRs
    vSemaphoreDelete(trigger_send_semphr);
    vSemaphoreDelete(task_delete_semphr);
    కొరకు (పూర్ణాంకం i = 0; i < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; i++) {
        TEST_ESP_OK(gptimer_disable(gptimers[i]));
        TEST_ESP_OK(gptimer_del_timer(gptimers[i]));
    }
}
#అయితే_ముగింపు //SOC_GPTIMER_SUPPORTED

/* Test causes asserts, so it cannot be run as a normal unity test case.
   Test case is ran as a separate test case in test_task_notify_too_high_index_fails
 */
TEST_CASE("Notify too high index fails", "[ignore]")
{
    uint32_t notification_విలువ = 47;
    x_Task_Notify_సూచీకృత(xTaskGetCurrentTaskHandle(), 2, notification_విలువ, eNoAction);
}

/* Test causes asserts, so it cannot be run as a normal unity test case.
   Test case is ran as a separate test case in test_task_notify_wait_too_high_index_fails
 */
TEST_CASE("Notify Wait too high index fails", "[ignore]")
{
    uint32_t notification_విలువ;
    x_Task_Notify_Wait_సూచీకృత(2, 0, 0, &notification_విలువ, pd_MS_గమ్యం_TICKS(10));
}

#ఒకవేళ CONFIG_FREERTOS_TASK_NOTIFICATION_ARRAY_ENTRIES > 1
స్థిరరాశి uint32_t NOTIFICATION_విలువ_0 = 47;
స్థిరరాశి uint32_t NOTIFICATION_విలువ_1 = 48;

శూన్య notify(శూన్య *arg)
{
    TaskHandle_t main_task = (TaskHandle_t) arg;
    x_Task_Notify_సూచీకృత(main_task, 0, NOTIFICATION_విలువ_0, e_సమితి_విలువ_With_Overwrite);
    x_Task_Notify_సూచీకృత(main_task, 1, NOTIFICATION_విలువ_1, e_సమితి_విలువ_With_Overwrite);
    vTaskDelete(ఖాళీ);
}
స్థిర TaskHandle_t notificator_task;

TEST_CASE("Notify to different indexes works", "[freertos]")
{
    uint32_t notification_విలువ_0 = 0;
    uint32_t notification_విలువ_1 = 0;

    TaskHandle_t main_task = xTaskGetCurrentTaskHandle();
    xTaskCreate(notify, "notificator", 2048, main_task, 2, &notificator_task);

    x_Task_Notify_Wait_సూచీకృత(0, 0, 0xFFFFFFFF, &notification_విలువ_0, pd_MS_గమ్యం_TICKS(10));
    x_Task_Notify_Wait_సూచీకృత(1, 0, 0xFFFFFFFF, &notification_విలువ_1, pd_MS_గమ్యం_TICKS(10));

    TEST_ASSERT_EQUAL(notification_విలువ_0, NOTIFICATION_విలువ_0);
    TEST_ASSERT_EQUAL(notification_విలువ_1, NOTIFICATION_విలువ_1);
}
#అయితే_ముగింపు
