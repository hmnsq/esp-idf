/*
 * SPDX-FileCopyrightText: 2022-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/semphr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/queue.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/event_groups.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "driver/gptimer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity_test_utils.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਅੰਕ_TASKS               8
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ALL_TASK_ਬਿੱਟ           ((1 << ਅੰਕ_TASKS) - 1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਦੋਆਧਾਰੀ_ਅੰਕ_CALL(task)          (1 << (task))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਦੋਆਧਾਰੀ_ਅੰਕ_RESPONSE(task)      (1 << ((task) + ਅੰਕ_TASKS))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ALL_CALL_ਬਿੱਟ           (ALL_TASK_ਬਿੱਟ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ALL_RESPONSE_ਬਿੱਟ       (ALL_TASK_ਬਿੱਟ << ਅੰਕ_TASKS)

ਸਥਿਰ ਅਚਲ ਪੂਰਨ ਗਿਣਤੀ_3 = 1000;
ਸਥਿਰ Event_ਸਮੂਹ_Handle_t eg;
ਸਥਿਰ SemaphoreHandle_t ਪੂਰਾ_sem;

ਸਥਿਰ ਖਾਲੀ task_event_ਸਮੂਹ_call_response(ਖਾਲੀ *param)
{
    ਪੂਰਨ task_ਅੰਕ = (ਪੂਰਨ)param;

    ਲਿਖੋ("Started %d\n", task_ਅੰਕ);

    ਲਈ (ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_3; i++) {
        /* Wait until the common "call" bit is set, starts off all tasks
           (clear on return) */
        TEST_ASSERT(x_Event_ਸਮੂਹ_Wait_ਬਿੱਟ(eg, ਦੋਆਧਾਰੀ_ਅੰਕ_CALL(task_ਅੰਕ), true, false, ਪੋਰਟ_MAX_DELAY));

        /* Set our individual "response" bit */
        x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ(eg, ਦੋਆਧਾਰੀ_ਅੰਕ_RESPONSE(task_ਅੰਕ));
    }

    ਲਿਖੋ("Task %d done\n", task_ਅੰਕ);
    xSemaphoreGive(ਪੂਰਾ_sem);
    // Wait to be deleted
    vTaskSuspend(ਨਿਰਾ);
}

TEST_CASE("FreeRTOS Event Groups", "[freertos]")
{
    eg = x_Event_ਸਮੂਹ_Create();
    ਪੂਰਾ_sem = xSemaphoreCreateCounting(ਅੰਕ_TASKS, 0);

    TaskHandle_t task_handles[ਅੰਕ_TASKS];

    /* Note: task_event_group_call_response all have higher priority than this task, so on this core
       they will always preempt this task.

       This is important because we need to know all tasks have blocked on their own BIT_CALL(task_num) each time we
       signal it, or they get out of sync.
     */
    ਲਈ (ਪੂਰਨ c = 0; c < ਅੰਕ_TASKS; c++) {
        x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(task_event_ਸਮੂਹ_call_response, "tsk_call_resp", 4096, (ਖਾਲੀ *)c, configMAX_PRIORITIES - 1, &task_handles[c], c % CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES);
    }

    /* Tasks all start instantly, but this task will resume running at the same time as the higher priority tasks on the
       other processor may still be setting up, so allow time for them to also block on BIT_CALL()... */
    vTaskDelay(10);

    ਲਈ (ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_3; i++) {
        /* signal all the "CALL" bits of each task */
        x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ(eg, ALL_CALL_ਬਿੱਟ);

        /* Wait until all tasks have set their respective response bits */
        TEST_ASSERT_EQUAL_HEX16(ALL_RESPONSE_ਬਿੱਟ, x_Event_ਸਮੂਹ_Wait_ਬਿੱਟ(eg, ALL_RESPONSE_ਬਿੱਟ, true, true, ਪੋਰਟ_MAX_DELAY));
    }

    /* Ensure all tasks have suspend themselves */
    ਲਈ (ਪੂਰਨ c = 0; c < ਅੰਕ_TASKS; c++) {
        TEST_ASSERT(xSemaphoreTake(ਪੂਰਾ_sem, 100 / ਪੋਰਟ_TICK_PERIOD_MS));
    }

    ਲਈ (ਪੂਰਨ c = 0; c < ਅੰਕ_TASKS; c++) {
        unity_utils_task_delete(task_handles[c]);
    }

    vSemaphoreDelete(ਪੂਰਾ_sem);
    v_Event_ਸਮੂਹ_Delete(eg);
}

ਸਥਿਰ ਖਾਲੀ task_test_sync(ਖਾਲੀ *param)
{
    ਪੂਰਨ task_ਅੰਕ = (ਪੂਰਨ)param;

    ਲਿਖੋ("Started %d\n", task_ਅੰਕ);

    ਲਈ (ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_3; i++) {
        /* set our bit, and wait on all tasks to set their bits */
        x_Event_ਸਮੂਹ_Sync(eg, ਦੋਆਧਾਰੀ_ਅੰਕ_CALL(task_ਅੰਕ), ALL_CALL_ਬਿੱਟ, ਪੋਰਟ_MAX_DELAY);
        /* clear our bit */
        x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ(eg, ਦੋਆਧਾਰੀ_ਅੰਕ_CALL(task_ਅੰਕ));
    }
    ਪੂਰਨ after_ਪੂਰਾ = x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ(eg, ਦੋਆਧਾਰੀ_ਅੰਕ_RESPONSE(task_ਅੰਕ));

    ਲਿਖੋ("Done %d = 0x%08x\n", task_ਅੰਕ, after_ਪੂਰਾ);

    xSemaphoreGive(ਪੂਰਾ_sem);
    vTaskDelete(ਨਿਰਾ);
}

TEST_CASE("FreeRTOS Event Group Sync", "[freertos]")
{
    eg = x_Event_ਸਮੂਹ_Create();
    ਪੂਰਾ_sem = xSemaphoreCreateCounting(ਅੰਕ_TASKS, 0);

    ਲਈ (ਪੂਰਨ c = 0; c < ਅੰਕ_TASKS; c++) {
        x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(task_test_sync, "task_test_sync", 4096, (ਖਾਲੀ *)c, configMAX_PRIORITIES - 1, ਨਿਰਾ, c % CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES);
    }

    ਲਈ (ਪੂਰਨ c = 0; c < ਅੰਕ_TASKS; c++) {
        ਲਿਖੋ("Waiting on %d (0x%08x)\n", c, ਦੋਆਧਾਰੀ_ਅੰਕ_RESPONSE(c));
        TEST_ASSERT(x_Event_ਸਮੂਹ_Wait_ਬਿੱਟ(eg, ਦੋਆਧਾਰੀ_ਅੰਕ_RESPONSE(c), false, false, ਪੋਰਟ_MAX_DELAY));
    }

    /* Ensure all tasks cleaned up correctly */
    ਲਈ (ਪੂਰਨ c = 0; c < ਅੰਕ_TASKS; c++) {
        TEST_ASSERT(xSemaphoreTake(ਪੂਰਾ_sem, 100 / ਪੋਰਟ_TICK_PERIOD_MS));
    }

    vSemaphoreDelete(ਪੂਰਾ_sem);
    v_Event_ਸਮੂਹ_Delete(eg);
}

ਸਥਿਰ TaskHandle_t ਲੜੀਵਾਰ_ਕ੍ਰਮ[2];
ਸਥਿਰ uint32_t ਲੜੀਵਾਰ_ਕ੍ਰਮ_ਸੂਚਕਾਂਕ = 0;

ਖਾਲੀ task_test_eg_prio(ਖਾਲੀ *arg)
{
    TaskHandle_t main_task_hdl = (TaskHandle_t)arg;

    /* Notify the main task that this task has been created */
    xTaskNotifyGive(main_task_hdl);

    /* Wait for the event group bits to be set */
    TEST_ASSERT_EQUAL(1, x_Event_ਸਮੂਹ_Wait_ਬਿੱਟ(eg, 1, pdTRUE, pdTRUE, ਪੋਰਟ_MAX_DELAY));

    /* Record the task handle in the run order array */
    ਲੜੀਵਾਰ_ਕ੍ਰਮ[ਲੜੀਵਾਰ_ਕ੍ਰਮ_ਸੂਚਕਾਂਕ++] = xTaskGetCurrentTaskHandle();

    /* Suspend the task */
    vTaskSuspend(ਨਿਰਾ);
}

TEST_CASE("FreeRTOS Event Groups do not cause priority inversion when higher priority task is unblocked", "[freertos]")
{
    ਲੜੀਵਾਰ_ਕ੍ਰਮ[0] = ਨਿਰਾ;
    ਲੜੀਵਾਰ_ਕ੍ਰਮ[1] = ਨਿਰਾ;
    ਲੜੀਵਾਰ_ਕ੍ਰਮ_ਸੂਚਕਾਂਕ = 0;

    /* Initialize the event group */
    eg = x_Event_ਸਮੂਹ_Create();

    /* Create a task with higher priority than the task that will set the event group bits */
    TaskHandle_t higher_prio_hdl;
    TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(task_test_eg_prio, "task_test_eg_prio", 2048, (ਖਾਲੀ *)xTaskGetCurrentTaskHandle(), CONFIG_UNITY_FREERTOS_ਤਰਜੀਹ + 1, &higher_prio_hdl, CONFIG_UNITY_FREERTOS_CPU));

    /* Wait for the task to be created */
    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);

    /* Set the event group bits */
    x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ(eg, 1);

    /* Record the task handle in the run order array */
    ਲੜੀਵਾਰ_ਕ੍ਰਮ[ਲੜੀਵਾਰ_ਕ੍ਰਮ_ਸੂਚਕਾਂਕ++] = xTaskGetCurrentTaskHandle();

    /* Verify that the higher priority task was unblocked and immediately scheduled and the lower priority task was preempted */
    TEST_ASSERT_EQUAL(higher_prio_hdl, ਲੜੀਵਾਰ_ਕ੍ਰਮ[0]);
    TEST_ASSERT_EQUAL(xTaskGetCurrentTaskHandle(), ਲੜੀਵਾਰ_ਕ੍ਰਮ[1]);

    /* Clean up */
    v_Event_ਸਮੂਹ_Delete(eg);
    vTaskDelete(higher_prio_hdl);
}

/*-----------------Test case for event group trace facilities-----------------*/
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ  CONFIG_FREERTOS_USE_TRACE_FACILITY
#ਜੇ SOC_GPTIMER_SUPPORTED
/*
 * Test event group Trace Facility functions such as
 * xEventGroupClearBitsFromISR(), xEventGroupSetBitsFromISR()
 */

//Use a timer to trigger an ISr
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਬਿੱਟ            0xAA
ਸਥਿਰ gptimer_handle_t gptimer;
ਸਥਿਰ bool test_ਸੈੱਟ_ਬਿੱਟ;
ਸਥਿਰ bool test_clear_ਬਿੱਟ;

ਸਥਿਰ bool on_timer_alarm_cb(gptimer_handle_t timer, ਅਚਲ gptimer_alarm_event_ਅੰਕੜੇ_t *edata, ਖਾਲੀ *user_ctx)
{
    ਅਧਾਰ_Type_t task_woken = pdFALSE;

    gptimer_stop(timer);

    ਜੇ (test_ਸੈੱਟ_ਬਿੱਟ) {
        x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ_From_ISR(eg, ਬਿੱਟ, &task_woken);
        test_ਸੈੱਟ_ਬਿੱਟ = false;
    } ਨਹੀਂਤਾਂ ਜੇ (test_clear_ਬਿੱਟ) {
        x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ_From_ISR(eg, ਬਿੱਟ);
        xSemaphoreGiveFromISR(ਪੂਰਾ_sem, &task_woken);
        test_clear_ਬਿੱਟ = false;
    }
    //Switch context if necessary
    ਵਾਪਸ task_woken == pdTRUE;
}

TEST_CASE("FreeRTOS Event Group ISR", "[freertos]")
{
    ਪੂਰਾ_sem = x_Semaphore_Create_ਦੁਆਧਾਰੀ();
    eg = x_Event_ਸਮੂਹ_Create();
    test_ਸੈੱਟ_ਬਿੱਟ = false;
    test_clear_ਬਿੱਟ = false;
    //Setup timer for ISR
    gptimer_config_t config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_ਗਿਣਤੀ_ਉੱਪਰ,
        .resolution_hz = 1000000,
    };
    TEST_ESP_OK(gptimer_new_timer(&config, &gptimer));
    gptimer_alarm_config_t alarm_config = {
        .reload_ਗਿਣਤੀ = 0,
        .alarm_ਗਿਣਤੀ = 200000,
    };
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_timer_alarm_cb,
    };
    TEST_ESP_OK(gptimer_ਰਜਿਸਟਰ_event_callbacks(gptimer, &cbs, ਨਿਰਾ));
    TEST_ESP_OK(gptimer_enable(gptimer));
    TEST_ESP_OK(gptimer_ਸੈੱਟ_alarm_action(gptimer, &alarm_config));

    //Test set bits
    ਲਿਖੋ("test set bits\r\n");
    test_ਸੈੱਟ_ਬਿੱਟ = true;
    TEST_ESP_OK(gptimer_ਸ਼ੁਰੂ(gptimer));
    TEST_ASSERT_EQUAL(ਬਿੱਟ, x_Event_ਸਮੂਹ_Wait_ਬਿੱਟ(eg, ਬਿੱਟ, pdFALSE, pdTRUE, ਪੋਰਟ_MAX_DELAY));     //Let ISR set event group bits

    //Test clear bits
    ਲਿਖੋ("test clear bits\r\n");
    x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ(eg, ਬਿੱਟ);                   //Set bits to be cleared
    test_clear_ਬਿੱਟ = true;
    TEST_ESP_OK(gptimer_ਸੈੱਟ_raw_ਗਿਣਤੀ(gptimer, 0));
    TEST_ESP_OK(gptimer_ਸ਼ੁਰੂ(gptimer));
    xSemaphoreTake(ਪੂਰਾ_sem, ਪੋਰਟ_MAX_DELAY);        //Wait for ISR to clear bits
    vTaskDelay(10);                                 //Event group clear bits runs via daemon task, delay so daemon can run
    TEST_ASSERT_EQUAL(0, x_Event_ਸਮੂਹ_Get_ਬਿੱਟ(eg));   //Check bits are cleared

    //Clean up
    TEST_ESP_OK(gptimer_disable(gptimer));
    TEST_ESP_OK(gptimer_del_timer(gptimer));
    v_Event_ਸਮੂਹ_Delete(eg);
    vSemaphoreDelete(ਪੂਰਾ_sem);
    vTaskDelay(10);     //Give time for idle task to clear up deleted tasks
}
#ਜੇ_ਅੰਤ //SOC_GPTIMER_SUPPORTED
#ਜੇ_ਅੰਤ      //CONFIG_FREERTOS_USE_TRACE_FACILITY
