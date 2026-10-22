/*
 * SPDX-FileCopyrightText: 2022-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#شامل <stdio.h>
#شامل "sdkconfig.h"
#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل "freertos/semphr.h"
#شامل "freertos/queue.h"
#شامل "freertos/event_groups.h"
#شامل "driver/gptimer.h"
#شامل "unity.h"
#شامل "unity_test_utils.h"

#تعریف_کرو عدد_TASKS               8
#تعریف_کرو ALL_TASK_بٹ           ((1 << عدد_TASKS) - 1)
#تعریف_کرو ثنائی_ہندسہ_CALL(task)          (1 << (task))
#تعریف_کرو ثنائی_ہندسہ_RESPONSE(task)      (1 << ((task) + عدد_TASKS))
#تعریف_کرو ALL_CALL_بٹ           (ALL_TASK_بٹ)
#تعریف_کرو ALL_RESPONSE_بٹ       (ALL_TASK_بٹ << عدد_TASKS)

ساکن ثابت صحیح گنتی_2 = 1000;
ساکن Event_گروہ_Handle_t eg;
ساکن SemaphoreHandle_t مکمل_sem;

ساکن خالی task_event_گروہ_call_response(خالی *param)
{
    صحیح task_عدد = (صحیح)param;

    لکھو("Started %d\n", task_عدد);

    برائے (صحیح i = 0; i < گنتی_2; i++) {
        /* Wait until the common "call" bit is set, starts off all tasks
           (clear on return) */
        TEST_ASSERT(x_Event_گروہ_Wait_بٹ(eg, ثنائی_ہندسہ_CALL(task_عدد), true, false, پورٹ_MAX_DELAY));

        /* Set our individual "response" bit */
        x_Event_گروہ_سیٹ_بٹ(eg, ثنائی_ہندسہ_RESPONSE(task_عدد));
    }

    لکھو("Task %d done\n", task_عدد);
    xSemaphoreGive(مکمل_sem);
    // Wait to be deleted
    vTaskSuspend(تہی);
}

TEST_CASE("FreeRTOS Event Groups", "[freertos]")
{
    eg = x_Event_گروہ_Create();
    مکمل_sem = xSemaphoreCreateCounting(عدد_TASKS, 0);

    TaskHandle_t task_handles[عدد_TASKS];

    /* Note: task_event_group_call_response all have higher priority than this task, so on this core
       they will always preempt this task.

       This is important because we need to know all tasks have blocked on their own BIT_CALL(task_num) each time we
       signal it, or they get out of sync.
     */
    برائے (صحیح c = 0; c < عدد_TASKS; c++) {
        x_Task_Create_Pinned_مقصد_Core(task_event_گروہ_call_response, "tsk_call_resp", 4096, (خالی *)c, configMAX_PRIORITIES - 1, &task_handles[c], c % CONFIG_FREERTOS_نمبر_کا_CORES);
    }

    /* Tasks all start instantly, but this task will resume running at the same time as the higher priority tasks on the
       other processor may still be setting up, so allow time for them to also block on BIT_CALL()... */
    vTaskDelay(10);

    برائے (صحیح i = 0; i < گنتی_2; i++) {
        /* signal all the "CALL" bits of each task */
        x_Event_گروہ_سیٹ_بٹ(eg, ALL_CALL_بٹ);

        /* Wait until all tasks have set their respective response bits */
        TEST_ASSERT_EQUAL_HEX16(ALL_RESPONSE_بٹ, x_Event_گروہ_Wait_بٹ(eg, ALL_RESPONSE_بٹ, true, true, پورٹ_MAX_DELAY));
    }

    /* Ensure all tasks have suspend themselves */
    برائے (صحیح c = 0; c < عدد_TASKS; c++) {
        TEST_ASSERT(xSemaphoreTake(مکمل_sem, 100 / پورٹ_TICK_PERIOD_MS));
    }

    برائے (صحیح c = 0; c < عدد_TASKS; c++) {
        unity_utils_task_delete(task_handles[c]);
    }

    vSemaphoreDelete(مکمل_sem);
    v_Event_گروہ_Delete(eg);
}

ساکن خالی task_test_sync(خالی *param)
{
    صحیح task_عدد = (صحیح)param;

    لکھو("Started %d\n", task_عدد);

    برائے (صحیح i = 0; i < گنتی_2; i++) {
        /* set our bit, and wait on all tasks to set their bits */
        x_Event_گروہ_Sync(eg, ثنائی_ہندسہ_CALL(task_عدد), ALL_CALL_بٹ, پورٹ_MAX_DELAY);
        /* clear our bit */
        x_Event_گروہ_Clear_بٹ(eg, ثنائی_ہندسہ_CALL(task_عدد));
    }
    صحیح after_مکمل = x_Event_گروہ_سیٹ_بٹ(eg, ثنائی_ہندسہ_RESPONSE(task_عدد));

    لکھو("Done %d = 0x%08x\n", task_عدد, after_مکمل);

    xSemaphoreGive(مکمل_sem);
    vTaskDelete(تہی);
}

TEST_CASE("FreeRTOS Event Group Sync", "[freertos]")
{
    eg = x_Event_گروہ_Create();
    مکمل_sem = xSemaphoreCreateCounting(عدد_TASKS, 0);

    برائے (صحیح c = 0; c < عدد_TASKS; c++) {
        x_Task_Create_Pinned_مقصد_Core(task_test_sync, "task_test_sync", 4096, (خالی *)c, configMAX_PRIORITIES - 1, تہی, c % CONFIG_FREERTOS_نمبر_کا_CORES);
    }

    برائے (صحیح c = 0; c < عدد_TASKS; c++) {
        لکھو("Waiting on %d (0x%08x)\n", c, ثنائی_ہندسہ_RESPONSE(c));
        TEST_ASSERT(x_Event_گروہ_Wait_بٹ(eg, ثنائی_ہندسہ_RESPONSE(c), false, false, پورٹ_MAX_DELAY));
    }

    /* Ensure all tasks cleaned up correctly */
    برائے (صحیح c = 0; c < عدد_TASKS; c++) {
        TEST_ASSERT(xSemaphoreTake(مکمل_sem, 100 / پورٹ_TICK_PERIOD_MS));
    }

    vSemaphoreDelete(مکمل_sem);
    v_Event_گروہ_Delete(eg);
}

ساکن TaskHandle_t تسلسل_ترتیب[2];
ساکن uint32_t تسلسل_ترتیب_اشاریہ = 0;

خالی task_test_eg_prio(خالی *arg)
{
    TaskHandle_t main_task_hdl = (TaskHandle_t)arg;

    /* Notify the main task that this task has been created */
    xTaskNotifyGive(main_task_hdl);

    /* Wait for the event group bits to be set */
    TEST_ASSERT_EQUAL(1, x_Event_گروہ_Wait_بٹ(eg, 1, pdTRUE, pdTRUE, پورٹ_MAX_DELAY));

    /* Record the task handle in the run order array */
    تسلسل_ترتیب[تسلسل_ترتیب_اشاریہ++] = xTaskGetCurrentTaskHandle();

    /* Suspend the task */
    vTaskSuspend(تہی);
}

TEST_CASE("FreeRTOS Event Groups do not cause priority inversion when higher priority task is unblocked", "[freertos]")
{
    تسلسل_ترتیب[0] = تہی;
    تسلسل_ترتیب[1] = تہی;
    تسلسل_ترتیب_اشاریہ = 0;

    /* Initialize the event group */
    eg = x_Event_گروہ_Create();

    /* Create a task with higher priority than the task that will set the event group bits */
    TaskHandle_t higher_prio_hdl;
    TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_مقصد_Core(task_test_eg_prio, "task_test_eg_prio", 2048, (خالی *)xTaskGetCurrentTaskHandle(), CONFIG_UNITY_FREERTOS_ترجیح + 1, &higher_prio_hdl, CONFIG_UNITY_FREERTOS_CPU));

    /* Wait for the task to be created */
    ulTaskNotifyTake(pdTRUE, پورٹ_MAX_DELAY);

    /* Set the event group bits */
    x_Event_گروہ_سیٹ_بٹ(eg, 1);

    /* Record the task handle in the run order array */
    تسلسل_ترتیب[تسلسل_ترتیب_اشاریہ++] = xTaskGetCurrentTaskHandle();

    /* Verify that the higher priority task was unblocked and immediately scheduled and the lower priority task was preempted */
    TEST_ASSERT_EQUAL(higher_prio_hdl, تسلسل_ترتیب[0]);
    TEST_ASSERT_EQUAL(xTaskGetCurrentTaskHandle(), تسلسل_ترتیب[1]);

    /* Clean up */
    v_Event_گروہ_Delete(eg);
    vTaskDelete(higher_prio_hdl);
}

/*-----------------Test case for event group trace facilities-----------------*/
#اگر_تعریف_شدہ  CONFIG_FREERTOS_USE_TRACE_FACILITY
#اگر SOC_GPTIMER_SUPPORTED
/*
 * Test event group Trace Facility functions such as
 * xEventGroupClearBitsFromISR(), xEventGroupSetBitsFromISR()
 */

//Use a timer to trigger an ISr
#تعریف_کرو بٹ            0xAA
ساکن gptimer_handle_t gptimer;
ساکن bool test_سیٹ_بٹ;
ساکن bool test_clear_بٹ;

ساکن bool on_timer_alarm_cb(gptimer_handle_t timer, ثابت gptimer_alarm_event_ڈیٹا_t *edata, خالی *user_ctx)
{
    بنیاد_Type_t task_woken = pdFALSE;

    gptimer_stop(timer);

    اگر (test_سیٹ_بٹ) {
        x_Event_گروہ_سیٹ_بٹ_From_ISR(eg, بٹ, &task_woken);
        test_سیٹ_بٹ = false;
    } ورنہ اگر (test_clear_بٹ) {
        x_Event_گروہ_Clear_بٹ_From_ISR(eg, بٹ);
        xSemaphoreGiveFromISR(مکمل_sem, &task_woken);
        test_clear_بٹ = false;
    }
    //Switch context if necessary
    واپس task_woken == pdTRUE;
}

TEST_CASE("FreeRTOS Event Group ISR", "[freertos]")
{
    مکمل_sem = x_Semaphore_Create_ثنائی();
    eg = x_Event_گروہ_Create();
    test_سیٹ_بٹ = false;
    test_clear_بٹ = false;
    //Setup timer for ISR
    gptimer_config_t config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_گنتی_اوپر,
        .resolution_hz = 1000000,
    };
    TEST_ESP_OK(gptimer_new_timer(&config, &gptimer));
    gptimer_alarm_config_t alarm_config = {
        .reload_گنتی = 0,
        .alarm_گنتی = 200000,
    };
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_timer_alarm_cb,
    };
    TEST_ESP_OK(gptimer_رجسٹر_event_callbacks(gptimer, &cbs, تہی));
    TEST_ESP_OK(gptimer_enable(gptimer));
    TEST_ESP_OK(gptimer_سیٹ_alarm_action(gptimer, &alarm_config));

    //Test set bits
    لکھو("test set bits\r\n");
    test_سیٹ_بٹ = true;
    TEST_ESP_OK(gptimer_آغاز(gptimer));
    TEST_ASSERT_EQUAL(بٹ, x_Event_گروہ_Wait_بٹ(eg, بٹ, pdFALSE, pdTRUE, پورٹ_MAX_DELAY));     //Let ISR set event group bits

    //Test clear bits
    لکھو("test clear bits\r\n");
    x_Event_گروہ_سیٹ_بٹ(eg, بٹ);                   //Set bits to be cleared
    test_clear_بٹ = true;
    TEST_ESP_OK(gptimer_سیٹ_raw_گنتی(gptimer, 0));
    TEST_ESP_OK(gptimer_آغاز(gptimer));
    xSemaphoreTake(مکمل_sem, پورٹ_MAX_DELAY);        //Wait for ISR to clear bits
    vTaskDelay(10);                                 //Event group clear bits runs via daemon task, delay so daemon can run
    TEST_ASSERT_EQUAL(0, x_Event_گروہ_Get_بٹ(eg));   //Check bits are cleared

    //Clean up
    TEST_ESP_OK(gptimer_disable(gptimer));
    TEST_ESP_OK(gptimer_del_timer(gptimer));
    v_Event_گروہ_Delete(eg);
    vSemaphoreDelete(مکمل_sem);
    vTaskDelay(10);     //Give time for idle task to clear up deleted tasks
}
#اگر_ختم //SOC_GPTIMER_SUPPORTED
#اگر_ختم      //CONFIG_FREERTOS_USE_TRACE_FACILITY
