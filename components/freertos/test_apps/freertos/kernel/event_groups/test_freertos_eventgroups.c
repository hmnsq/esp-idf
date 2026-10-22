/*
 * SPDX-FileCopyrightText: 2022-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ <stdio.h>
#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "freertos/semphr.h"
#అన్తర్భావ "freertos/queue.h"
#అన్తర్భావ "freertos/event_groups.h"
#అన్తర్భావ "driver/gptimer.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "unity_test_utils.h"

#నిర్వచించు అంకె_TASKS               8
#నిర్వచించు ALL_TASK_బిట్లు           ((1 << అంకె_TASKS) - 1)
#నిర్వచించు బిట్టు_CALL(task)          (1 << (task))
#నిర్వచించు బిట్టు_RESPONSE(task)      (1 << ((task) + అంకె_TASKS))
#నిర్వచించు ALL_CALL_బిట్లు           (ALL_TASK_బిట్లు)
#నిర్వచించు ALL_RESPONSE_బిట్లు       (ALL_TASK_బిట్లు << అంకె_TASKS)

స్థిర స్థిరరాశి పూర్ణాంకం లెక్క_2 = 1000;
స్థిర Event_సమూహం_Handle_t eg;
స్థిర SemaphoreHandle_t పూర్తి_sem;

స్థిర శూన్య task_event_సమూహం_call_response(శూన్య *param)
{
    పూర్ణాంకం task_అంకె = (పూర్ణాంకం)param;

    రాయి("Started %d\n", task_అంకె);

    కొరకు (పూర్ణాంకం i = 0; i < లెక్క_2; i++) {
        /* Wait until the common "call" bit is set, starts off all tasks
           (clear on return) */
        TEST_ASSERT(x_Event_సమూహం_Wait_బిట్లు(eg, బిట్టు_CALL(task_అంకె), true, false, పోర్టు_MAX_DELAY));

        /* Set our individual "response" bit */
        x_Event_సమూహం_సమితి_బిట్లు(eg, బిట్టు_RESPONSE(task_అంకె));
    }

    రాయి("Task %d done\n", task_అంకె);
    xSemaphoreGive(పూర్తి_sem);
    // Wait to be deleted
    vTaskSuspend(ఖాళీ);
}

TEST_CASE("FreeRTOS Event Groups", "[freertos]")
{
    eg = x_Event_సమూహం_Create();
    పూర్తి_sem = xSemaphoreCreateCounting(అంకె_TASKS, 0);

    TaskHandle_t task_handles[అంకె_TASKS];

    /* Note: task_event_group_call_response all have higher priority than this task, so on this core
       they will always preempt this task.

       This is important because we need to know all tasks have blocked on their own BIT_CALL(task_num) each time we
       signal it, or they get out of sync.
     */
    కొరకు (పూర్ణాంకం c = 0; c < అంకె_TASKS; c++) {
        x_Task_Create_Pinned_గమ్యం_Core(task_event_సమూహం_call_response, "tsk_call_resp", 4096, (శూన్య *)c, configMAX_PRIORITIES - 1, &task_handles[c], c % CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES);
    }

    /* Tasks all start instantly, but this task will resume running at the same time as the higher priority tasks on the
       other processor may still be setting up, so allow time for them to also block on BIT_CALL()... */
    vTaskDelay(10);

    కొరకు (పూర్ణాంకం i = 0; i < లెక్క_2; i++) {
        /* signal all the "CALL" bits of each task */
        x_Event_సమూహం_సమితి_బిట్లు(eg, ALL_CALL_బిట్లు);

        /* Wait until all tasks have set their respective response bits */
        TEST_ASSERT_EQUAL_HEX16(ALL_RESPONSE_బిట్లు, x_Event_సమూహం_Wait_బిట్లు(eg, ALL_RESPONSE_బిట్లు, true, true, పోర్టు_MAX_DELAY));
    }

    /* Ensure all tasks have suspend themselves */
    కొరకు (పూర్ణాంకం c = 0; c < అంకె_TASKS; c++) {
        TEST_ASSERT(xSemaphoreTake(పూర్తి_sem, 100 / పోర్టు_TICK_PERIOD_MS));
    }

    కొరకు (పూర్ణాంకం c = 0; c < అంకె_TASKS; c++) {
        unity_utils_task_delete(task_handles[c]);
    }

    vSemaphoreDelete(పూర్తి_sem);
    v_Event_సమూహం_Delete(eg);
}

స్థిర శూన్య task_test_sync(శూన్య *param)
{
    పూర్ణాంకం task_అంకె = (పూర్ణాంకం)param;

    రాయి("Started %d\n", task_అంకె);

    కొరకు (పూర్ణాంకం i = 0; i < లెక్క_2; i++) {
        /* set our bit, and wait on all tasks to set their bits */
        x_Event_సమూహం_Sync(eg, బిట్టు_CALL(task_అంకె), ALL_CALL_బిట్లు, పోర్టు_MAX_DELAY);
        /* clear our bit */
        x_Event_సమూహం_Clear_బిట్లు(eg, బిట్టు_CALL(task_అంకె));
    }
    పూర్ణాంకం after_పూర్తి = x_Event_సమూహం_సమితి_బిట్లు(eg, బిట్టు_RESPONSE(task_అంకె));

    రాయి("Done %d = 0x%08x\n", task_అంకె, after_పూర్తి);

    xSemaphoreGive(పూర్తి_sem);
    vTaskDelete(ఖాళీ);
}

TEST_CASE("FreeRTOS Event Group Sync", "[freertos]")
{
    eg = x_Event_సమూహం_Create();
    పూర్తి_sem = xSemaphoreCreateCounting(అంకె_TASKS, 0);

    కొరకు (పూర్ణాంకం c = 0; c < అంకె_TASKS; c++) {
        x_Task_Create_Pinned_గమ్యం_Core(task_test_sync, "task_test_sync", 4096, (శూన్య *)c, configMAX_PRIORITIES - 1, ఖాళీ, c % CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES);
    }

    కొరకు (పూర్ణాంకం c = 0; c < అంకె_TASKS; c++) {
        రాయి("Waiting on %d (0x%08x)\n", c, బిట్టు_RESPONSE(c));
        TEST_ASSERT(x_Event_సమూహం_Wait_బిట్లు(eg, బిట్టు_RESPONSE(c), false, false, పోర్టు_MAX_DELAY));
    }

    /* Ensure all tasks cleaned up correctly */
    కొరకు (పూర్ణాంకం c = 0; c < అంకె_TASKS; c++) {
        TEST_ASSERT(xSemaphoreTake(పూర్తి_sem, 100 / పోర్టు_TICK_PERIOD_MS));
    }

    vSemaphoreDelete(పూర్తి_sem);
    v_Event_సమూహం_Delete(eg);
}

స్థిర TaskHandle_t నిరంతరం_క్రమం[2];
స్థిర uint32_t నిరంతరం_క్రమం_సూచీ = 0;

శూన్య task_test_eg_prio(శూన్య *arg)
{
    TaskHandle_t main_task_hdl = (TaskHandle_t)arg;

    /* Notify the main task that this task has been created */
    xTaskNotifyGive(main_task_hdl);

    /* Wait for the event group bits to be set */
    TEST_ASSERT_EQUAL(1, x_Event_సమూహం_Wait_బిట్లు(eg, 1, pdTRUE, pdTRUE, పోర్టు_MAX_DELAY));

    /* Record the task handle in the run order array */
    నిరంతరం_క్రమం[నిరంతరం_క్రమం_సూచీ++] = xTaskGetCurrentTaskHandle();

    /* Suspend the task */
    vTaskSuspend(ఖాళీ);
}

TEST_CASE("FreeRTOS Event Groups do not cause priority inversion when higher priority task is unblocked", "[freertos]")
{
    నిరంతరం_క్రమం[0] = ఖాళీ;
    నిరంతరం_క్రమం[1] = ఖాళీ;
    నిరంతరం_క్రమం_సూచీ = 0;

    /* Initialize the event group */
    eg = x_Event_సమూహం_Create();

    /* Create a task with higher priority than the task that will set the event group bits */
    TaskHandle_t higher_prio_hdl;
    TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_గమ్యం_Core(task_test_eg_prio, "task_test_eg_prio", 2048, (శూన్య *)xTaskGetCurrentTaskHandle(), CONFIG_UNITY_FREERTOS_ప్రాధాన్యత + 1, &higher_prio_hdl, CONFIG_UNITY_FREERTOS_CPU));

    /* Wait for the task to be created */
    ulTaskNotifyTake(pdTRUE, పోర్టు_MAX_DELAY);

    /* Set the event group bits */
    x_Event_సమూహం_సమితి_బిట్లు(eg, 1);

    /* Record the task handle in the run order array */
    నిరంతరం_క్రమం[నిరంతరం_క్రమం_సూచీ++] = xTaskGetCurrentTaskHandle();

    /* Verify that the higher priority task was unblocked and immediately scheduled and the lower priority task was preempted */
    TEST_ASSERT_EQUAL(higher_prio_hdl, నిరంతరం_క్రమం[0]);
    TEST_ASSERT_EQUAL(xTaskGetCurrentTaskHandle(), నిరంతరం_క్రమం[1]);

    /* Clean up */
    v_Event_సమూహం_Delete(eg);
    vTaskDelete(higher_prio_hdl);
}

/*-----------------Test case for event group trace facilities-----------------*/
#నిర్వచితమైతే  CONFIG_FREERTOS_USE_TRACE_FACILITY
#ఒకవేళ SOC_GPTIMER_SUPPORTED
/*
 * Test event group Trace Facility functions such as
 * xEventGroupClearBitsFromISR(), xEventGroupSetBitsFromISR()
 */

//Use a timer to trigger an ISr
#నిర్వచించు బిట్లు            0xAA
స్థిర gptimer_handle_t gptimer;
స్థిర bool test_సమితి_బిట్లు;
స్థిర bool test_clear_బిట్లు;

స్థిర bool on_timer_alarm_cb(gptimer_handle_t timer, స్థిరరాశి gptimer_alarm_event_దత్తాంశం_t *edata, శూన్య *user_ctx)
{
    పునాది_Type_t task_woken = pdFALSE;

    gptimer_stop(timer);

    ఒకవేళ (test_సమితి_బిట్లు) {
        x_Event_సమూహం_సమితి_బిట్లు_From_ISR(eg, బిట్లు, &task_woken);
        test_సమితి_బిట్లు = false;
    } లేకపోతే ఒకవేళ (test_clear_బిట్లు) {
        x_Event_సమూహం_Clear_బిట్లు_From_ISR(eg, బిట్లు);
        xSemaphoreGiveFromISR(పూర్తి_sem, &task_woken);
        test_clear_బిట్లు = false;
    }
    //Switch context if necessary
    తిరిగి task_woken == pdTRUE;
}

TEST_CASE("FreeRTOS Event Group ISR", "[freertos]")
{
    పూర్తి_sem = x_Semaphore_Create_ద్విమాన();
    eg = x_Event_సమూహం_Create();
    test_సమితి_బిట్లు = false;
    test_clear_బిట్లు = false;
    //Setup timer for ISR
    gptimer_config_t config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_లెక్క_పైకి,
        .resolution_hz = 1000000,
    };
    TEST_ESP_OK(gptimer_new_timer(&config, &gptimer));
    gptimer_alarm_config_t alarm_config = {
        .reload_లెక్క = 0,
        .alarm_లెక్క = 200000,
    };
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_timer_alarm_cb,
    };
    TEST_ESP_OK(gptimer_రిజిస్టరు_event_callbacks(gptimer, &cbs, ఖాళీ));
    TEST_ESP_OK(gptimer_enable(gptimer));
    TEST_ESP_OK(gptimer_సమితి_alarm_action(gptimer, &alarm_config));

    //Test set bits
    రాయి("test set bits\r\n");
    test_సమితి_బిట్లు = true;
    TEST_ESP_OK(gptimer_ప్రారంభం(gptimer));
    TEST_ASSERT_EQUAL(బిట్లు, x_Event_సమూహం_Wait_బిట్లు(eg, బిట్లు, pdFALSE, pdTRUE, పోర్టు_MAX_DELAY));     //Let ISR set event group bits

    //Test clear bits
    రాయి("test clear bits\r\n");
    x_Event_సమూహం_సమితి_బిట్లు(eg, బిట్లు);                   //Set bits to be cleared
    test_clear_బిట్లు = true;
    TEST_ESP_OK(gptimer_సమితి_raw_లెక్క(gptimer, 0));
    TEST_ESP_OK(gptimer_ప్రారంభం(gptimer));
    xSemaphoreTake(పూర్తి_sem, పోర్టు_MAX_DELAY);        //Wait for ISR to clear bits
    vTaskDelay(10);                                 //Event group clear bits runs via daemon task, delay so daemon can run
    TEST_ASSERT_EQUAL(0, x_Event_సమూహం_Get_బిట్లు(eg));   //Check bits are cleared

    //Clean up
    TEST_ESP_OK(gptimer_disable(gptimer));
    TEST_ESP_OK(gptimer_del_timer(gptimer));
    v_Event_సమూహం_Delete(eg);
    vSemaphoreDelete(పూర్తి_sem);
    vTaskDelay(10);     //Give time for idle task to clear up deleted tasks
}
#అయితే_ముగింపు //SOC_GPTIMER_SUPPORTED
#అయితే_ముగింపు      //CONFIG_FREERTOS_USE_TRACE_FACILITY
