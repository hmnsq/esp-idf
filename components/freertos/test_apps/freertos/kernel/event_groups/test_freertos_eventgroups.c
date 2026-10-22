/*
 * SPDX-FileCopyrightText: 2022-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ "sdkconfig.h"
#അന്തര്ഭാവ "freertos/FreeRTOS.h"
#അന്തര്ഭാവ "freertos/task.h"
#അന്തര്ഭാവ "freertos/semphr.h"
#അന്തര്ഭാവ "freertos/queue.h"
#അന്തര്ഭാവ "freertos/event_groups.h"
#അന്തര്ഭാവ "driver/gptimer.h"
#അന്തര്ഭാവ "unity.h"
#അന്തര്ഭാവ "unity_test_utils.h"

#define അക്കം_TASKS               8
#define ALL_TASK_BITS           ((1 << അക്കം_TASKS) - 1)
#define BIT_CALL(task)          (1 << (task))
#define BIT_RESPONSE(task)      (1 << ((task) + അക്കം_TASKS))
#define ALL_CALL_BITS           (ALL_TASK_BITS)
#define ALL_RESPONSE_BITS       (ALL_TASK_BITS << അക്കം_TASKS)

സ്ഥിതം സ്ഥിരം പൂർണ്ണസംഖ്യ എണ്ണം_2 = 1000;
സ്ഥിതം EventGroupHandle_t eg;
സ്ഥിതം SemaphoreHandle_t done_sem;

സ്ഥിതം ശൂന്യം task_event_group_call_response(ശൂന്യം *param)
{
    പൂർണ്ണസംഖ്യ task_അക്കം = (പൂർണ്ണസംഖ്യ)param;

    printf("Started %d\n", task_അക്കം);

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം_2; i++) {
        /* Wait until the common "call" bit is set, starts off all tasks
           (clear on return) */
        TEST_ASSERT(xEventGroupWaitBits(eg, BIT_CALL(task_അക്കം), true, false, portMAX_DELAY));

        /* Set our individual "response" bit */
        xEventGroupSetBits(eg, BIT_RESPONSE(task_അക്കം));
    }

    printf("Task %d done\n", task_അക്കം);
    xSemaphoreGive(done_sem);
    // Wait to be deleted
    vTaskSuspend(ശൂന്യ);
}

TEST_CASE("FreeRTOS Event Groups", "[freertos]")
{
    eg = xEventGroupCreate();
    done_sem = xSemaphoreCreateCounting(അക്കം_TASKS, 0);

    TaskHandle_t task_handles[അക്കം_TASKS];

    /* Note: task_event_group_call_response all have higher priority than this task, so on this core
       they will always preempt this task.

       This is important because we need to know all tasks have blocked on their own BIT_CALL(task_num) each time we
       signal it, or they get out of sync.
     */
    വേണ്ടി (പൂർണ്ണസംഖ്യ c = 0; c < അക്കം_TASKS; c++) {
        xTaskCreatePinnedToCore(task_event_group_call_response, "tsk_call_resp", 4096, (ശൂന്യം *)c, configMAX_PRIORITIES - 1, &task_handles[c], c % CONFIG_FREERTOS_സംഖ്യ_OF_CORES);
    }

    /* Tasks all start instantly, but this task will resume running at the same time as the higher priority tasks on the
       other processor may still be setting up, so allow time for them to also block on BIT_CALL()... */
    vTaskDelay(10);

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം_2; i++) {
        /* signal all the "CALL" bits of each task */
        xEventGroupSetBits(eg, ALL_CALL_BITS);

        /* Wait until all tasks have set their respective response bits */
        TEST_ASSERT_EQUAL_HEX16(ALL_RESPONSE_BITS, xEventGroupWaitBits(eg, ALL_RESPONSE_BITS, true, true, portMAX_DELAY));
    }

    /* Ensure all tasks have suspend themselves */
    വേണ്ടി (പൂർണ്ണസംഖ്യ c = 0; c < അക്കം_TASKS; c++) {
        TEST_ASSERT(xSemaphoreTake(done_sem, 100 / portTICK_PERIOD_MS));
    }

    വേണ്ടി (പൂർണ്ണസംഖ്യ c = 0; c < അക്കം_TASKS; c++) {
        unity_utils_task_delete(task_handles[c]);
    }

    vSemaphoreDelete(done_sem);
    vEventGroupDelete(eg);
}

സ്ഥിതം ശൂന്യം task_test_sync(ശൂന്യം *param)
{
    പൂർണ്ണസംഖ്യ task_അക്കം = (പൂർണ്ണസംഖ്യ)param;

    printf("Started %d\n", task_അക്കം);

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം_2; i++) {
        /* set our bit, and wait on all tasks to set their bits */
        xEventGroupSync(eg, BIT_CALL(task_അക്കം), ALL_CALL_BITS, portMAX_DELAY);
        /* clear our bit */
        xEventGroupClearBits(eg, BIT_CALL(task_അക്കം));
    }
    പൂർണ്ണസംഖ്യ after_done = xEventGroupSetBits(eg, BIT_RESPONSE(task_അക്കം));

    printf("Done %d = 0x%08x\n", task_അക്കം, after_done);

    xSemaphoreGive(done_sem);
    vTaskDelete(ശൂന്യ);
}

TEST_CASE("FreeRTOS Event Group Sync", "[freertos]")
{
    eg = xEventGroupCreate();
    done_sem = xSemaphoreCreateCounting(അക്കം_TASKS, 0);

    വേണ്ടി (പൂർണ്ണസംഖ്യ c = 0; c < അക്കം_TASKS; c++) {
        xTaskCreatePinnedToCore(task_test_sync, "task_test_sync", 4096, (ശൂന്യം *)c, configMAX_PRIORITIES - 1, ശൂന്യ, c % CONFIG_FREERTOS_സംഖ്യ_OF_CORES);
    }

    വേണ്ടി (പൂർണ്ണസംഖ്യ c = 0; c < അക്കം_TASKS; c++) {
        printf("Waiting on %d (0x%08x)\n", c, BIT_RESPONSE(c));
        TEST_ASSERT(xEventGroupWaitBits(eg, BIT_RESPONSE(c), false, false, portMAX_DELAY));
    }

    /* Ensure all tasks cleaned up correctly */
    വേണ്ടി (പൂർണ്ണസംഖ്യ c = 0; c < അക്കം_TASKS; c++) {
        TEST_ASSERT(xSemaphoreTake(done_sem, 100 / portTICK_PERIOD_MS));
    }

    vSemaphoreDelete(done_sem);
    vEventGroupDelete(eg);
}

സ്ഥിതം TaskHandle_t run_order[2];
സ്ഥിതം uint32_t run_order_index = 0;

ശൂന്യം task_test_eg_prio(ശൂന്യം *arg)
{
    TaskHandle_t main_task_hdl = (TaskHandle_t)arg;

    /* Notify the main task that this task has been created */
    xTaskNotifyGive(main_task_hdl);

    /* Wait for the event group bits to be set */
    TEST_ASSERT_EQUAL(1, xEventGroupWaitBits(eg, 1, pdTRUE, pdTRUE, portMAX_DELAY));

    /* Record the task handle in the run order array */
    run_order[run_order_index++] = xTaskGetCurrentTaskHandle();

    /* Suspend the task */
    vTaskSuspend(ശൂന്യ);
}

TEST_CASE("FreeRTOS Event Groups do not cause priority inversion when higher priority task is unblocked", "[freertos]")
{
    run_order[0] = ശൂന്യ;
    run_order[1] = ശൂന്യ;
    run_order_index = 0;

    /* Initialize the event group */
    eg = xEventGroupCreate();

    /* Create a task with higher priority than the task that will set the event group bits */
    TaskHandle_t higher_prio_hdl;
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(task_test_eg_prio, "task_test_eg_prio", 2048, (ശൂന്യം *)xTaskGetCurrentTaskHandle(), CONFIG_UNITY_FREERTOS_PRIORITY + 1, &higher_prio_hdl, CONFIG_UNITY_FREERTOS_CPU));

    /* Wait for the task to be created */
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    /* Set the event group bits */
    xEventGroupSetBits(eg, 1);

    /* Record the task handle in the run order array */
    run_order[run_order_index++] = xTaskGetCurrentTaskHandle();

    /* Verify that the higher priority task was unblocked and immediately scheduled and the lower priority task was preempted */
    TEST_ASSERT_EQUAL(higher_prio_hdl, run_order[0]);
    TEST_ASSERT_EQUAL(xTaskGetCurrentTaskHandle(), run_order[1]);

    /* Clean up */
    vEventGroupDelete(eg);
    vTaskDelete(higher_prio_hdl);
}

/*-----------------Test case for event group trace facilities-----------------*/
#ifdef  CONFIG_FREERTOS_USE_TRACE_FACILITY
#എങ്കിൽ SOC_GPTIMER_SUPPORTED
/*
 * Test event group Trace Facility functions such as
 * xEventGroupClearBitsFromISR(), xEventGroupSetBitsFromISR()
 */

//Use a timer to trigger an ISr
#define BITS            0xAA
സ്ഥിതം gptimer_handle_t gptimer;
സ്ഥിതം bool test_set_bits;
സ്ഥിതം bool test_clear_bits;

സ്ഥിതം bool on_timer_alarm_cb(gptimer_handle_t timer, സ്ഥിരം gptimer_alarm_event_data_t *edata, ശൂന്യം *user_ctx)
{
    BaseType_t task_woken = pdFALSE;

    gptimer_stop(timer);

    എങ്കിൽ (test_set_bits) {
        xEventGroupSetBitsFromISR(eg, BITS, &task_woken);
        test_set_bits = false;
    } അല്ലെങ്കിൽ എങ്കിൽ (test_clear_bits) {
        xEventGroupClearBitsFromISR(eg, BITS);
        xSemaphoreGiveFromISR(done_sem, &task_woken);
        test_clear_bits = false;
    }
    //Switch context if necessary
    തിരികെ task_woken == pdTRUE;
}

TEST_CASE("FreeRTOS Event Group ISR", "[freertos]")
{
    done_sem = x_Semaphore_Create_ദ്വിമാന();
    eg = xEventGroupCreate();
    test_set_bits = false;
    test_clear_bits = false;
    //Setup timer for ISR
    gptimer_config_t config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_എണ്ണം_UP,
        .resolution_hz = 1000000,
    };
    TEST_ESP_OK(gptimer_new_timer(&config, &gptimer));
    gptimer_alarm_config_t alarm_config = {
        .reload_എണ്ണം = 0,
        .alarm_എണ്ണം = 200000,
    };
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_timer_alarm_cb,
    };
    TEST_ESP_OK(gptimer_register_event_callbacks(gptimer, &cbs, ശൂന്യ));
    TEST_ESP_OK(gptimer_enable(gptimer));
    TEST_ESP_OK(gptimer_set_alarm_action(gptimer, &alarm_config));

    //Test set bits
    printf("test set bits\r\n");
    test_set_bits = true;
    TEST_ESP_OK(gptimer_start(gptimer));
    TEST_ASSERT_EQUAL(BITS, xEventGroupWaitBits(eg, BITS, pdFALSE, pdTRUE, portMAX_DELAY));     //Let ISR set event group bits

    //Test clear bits
    printf("test clear bits\r\n");
    xEventGroupSetBits(eg, BITS);                   //Set bits to be cleared
    test_clear_bits = true;
    TEST_ESP_OK(gptimer_set_raw_എണ്ണം(gptimer, 0));
    TEST_ESP_OK(gptimer_start(gptimer));
    xSemaphoreTake(done_sem, portMAX_DELAY);        //Wait for ISR to clear bits
    vTaskDelay(10);                                 //Event group clear bits runs via daemon task, delay so daemon can run
    TEST_ASSERT_EQUAL(0, xEventGroupGetBits(eg));   //Check bits are cleared

    //Clean up
    TEST_ESP_OK(gptimer_disable(gptimer));
    TEST_ESP_OK(gptimer_del_timer(gptimer));
    vEventGroupDelete(eg);
    vSemaphoreDelete(done_sem);
    vTaskDelay(10);     //Give time for idle task to clear up deleted tasks
}
#endif //SOC_GPTIMER_SUPPORTED
#endif      //CONFIG_FREERTOS_USE_TRACE_FACILITY
