/*
 * SPDX-FileCopyrightText: 2022-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ "sdkconfig.h"
#அந்தர்பாவ "freertos/FreeRTOS.h"
#அந்தர்பாவ "freertos/task.h"
#அந்தர்பாவ "freertos/semphr.h"
#அந்தர்பாவ "freertos/queue.h"
#அந்தர்பாவ "freertos/event_groups.h"
#அந்தர்பாவ "driver/gptimer.h"
#அந்தர்பாவ "unity.h"
#அந்தர்பாவ "unity_test_utils.h"

#define இலக்கம்_TASKS               8
#define ALL_TASK_BITS           ((1 << இலக்கம்_TASKS) - 1)
#define BIT_CALL(task)          (1 << (task))
#define BIT_RESPONSE(task)      (1 << ((task) + இலக்கம்_TASKS))
#define ALL_CALL_BITS           (ALL_TASK_BITS)
#define ALL_RESPONSE_BITS       (ALL_TASK_BITS << இலக்கம்_TASKS)

நிலையான மாறிலி முழு மொத்தம்_2 = 1000;
நிலையான EventGroupHandle_t eg;
நிலையான SemaphoreHandle_t done_sem;

நிலையான வெறுமை task_event_group_call_response(வெறுமை *param)
{
    முழு task_இலக்கம் = (முழு)param;

    printf("Started %d\n", task_இலக்கம்);

    க்கு (முழு i = 0; i < மொத்தம்_2; i++) {
        /* Wait until the common "call" bit is set, starts off all tasks
           (clear on return) */
        TEST_ASSERT(xEventGroupWaitBits(eg, BIT_CALL(task_இலக்கம்), true, false, portMAX_DELAY));

        /* Set our individual "response" bit */
        xEventGroupSetBits(eg, BIT_RESPONSE(task_இலக்கம்));
    }

    printf("Task %d done\n", task_இலக்கம்);
    xSemaphoreGive(done_sem);
    // Wait to be deleted
    vTaskSuspend(வெற்று);
}

TEST_CASE("FreeRTOS Event Groups", "[freertos]")
{
    eg = xEventGroupCreate();
    done_sem = xSemaphoreCreateCounting(இலக்கம்_TASKS, 0);

    TaskHandle_t task_handles[இலக்கம்_TASKS];

    /* Note: task_event_group_call_response all have higher priority than this task, so on this core
       they will always preempt this task.

       This is important because we need to know all tasks have blocked on their own BIT_CALL(task_num) each time we
       signal it, or they get out of sync.
     */
    க்கு (முழு c = 0; c < இலக்கம்_TASKS; c++) {
        xTaskCreatePinnedToCore(task_event_group_call_response, "tsk_call_resp", 4096, (வெறுமை *)c, configMAX_PRIORITIES - 1, &task_handles[c], c % CONFIG_FREERTOS_எண்_OF_CORES);
    }

    /* Tasks all start instantly, but this task will resume running at the same time as the higher priority tasks on the
       other processor may still be setting up, so allow time for them to also block on BIT_CALL()... */
    vTaskDelay(10);

    க்கு (முழு i = 0; i < மொத்தம்_2; i++) {
        /* signal all the "CALL" bits of each task */
        xEventGroupSetBits(eg, ALL_CALL_BITS);

        /* Wait until all tasks have set their respective response bits */
        TEST_ASSERT_EQUAL_HEX16(ALL_RESPONSE_BITS, xEventGroupWaitBits(eg, ALL_RESPONSE_BITS, true, true, portMAX_DELAY));
    }

    /* Ensure all tasks have suspend themselves */
    க்கு (முழு c = 0; c < இலக்கம்_TASKS; c++) {
        TEST_ASSERT(xSemaphoreTake(done_sem, 100 / portTICK_PERIOD_MS));
    }

    க்கு (முழு c = 0; c < இலக்கம்_TASKS; c++) {
        unity_utils_task_delete(task_handles[c]);
    }

    vSemaphoreDelete(done_sem);
    vEventGroupDelete(eg);
}

நிலையான வெறுமை task_test_sync(வெறுமை *param)
{
    முழு task_இலக்கம் = (முழு)param;

    printf("Started %d\n", task_இலக்கம்);

    க்கு (முழு i = 0; i < மொத்தம்_2; i++) {
        /* set our bit, and wait on all tasks to set their bits */
        xEventGroupSync(eg, BIT_CALL(task_இலக்கம்), ALL_CALL_BITS, portMAX_DELAY);
        /* clear our bit */
        xEventGroupClearBits(eg, BIT_CALL(task_இலக்கம்));
    }
    முழு after_done = xEventGroupSetBits(eg, BIT_RESPONSE(task_இலக்கம்));

    printf("Done %d = 0x%08x\n", task_இலக்கம், after_done);

    xSemaphoreGive(done_sem);
    vTaskDelete(வெற்று);
}

TEST_CASE("FreeRTOS Event Group Sync", "[freertos]")
{
    eg = xEventGroupCreate();
    done_sem = xSemaphoreCreateCounting(இலக்கம்_TASKS, 0);

    க்கு (முழு c = 0; c < இலக்கம்_TASKS; c++) {
        xTaskCreatePinnedToCore(task_test_sync, "task_test_sync", 4096, (வெறுமை *)c, configMAX_PRIORITIES - 1, வெற்று, c % CONFIG_FREERTOS_எண்_OF_CORES);
    }

    க்கு (முழு c = 0; c < இலக்கம்_TASKS; c++) {
        printf("Waiting on %d (0x%08x)\n", c, BIT_RESPONSE(c));
        TEST_ASSERT(xEventGroupWaitBits(eg, BIT_RESPONSE(c), false, false, portMAX_DELAY));
    }

    /* Ensure all tasks cleaned up correctly */
    க்கு (முழு c = 0; c < இலக்கம்_TASKS; c++) {
        TEST_ASSERT(xSemaphoreTake(done_sem, 100 / portTICK_PERIOD_MS));
    }

    vSemaphoreDelete(done_sem);
    vEventGroupDelete(eg);
}

நிலையான TaskHandle_t run_order[2];
நிலையான uint32_t run_order_index = 0;

வெறுமை task_test_eg_prio(வெறுமை *arg)
{
    TaskHandle_t main_task_hdl = (TaskHandle_t)arg;

    /* Notify the main task that this task has been created */
    xTaskNotifyGive(main_task_hdl);

    /* Wait for the event group bits to be set */
    TEST_ASSERT_EQUAL(1, xEventGroupWaitBits(eg, 1, pdTRUE, pdTRUE, portMAX_DELAY));

    /* Record the task handle in the run order array */
    run_order[run_order_index++] = xTaskGetCurrentTaskHandle();

    /* Suspend the task */
    vTaskSuspend(வெற்று);
}

TEST_CASE("FreeRTOS Event Groups do not cause priority inversion when higher priority task is unblocked", "[freertos]")
{
    run_order[0] = வெற்று;
    run_order[1] = வெற்று;
    run_order_index = 0;

    /* Initialize the event group */
    eg = xEventGroupCreate();

    /* Create a task with higher priority than the task that will set the event group bits */
    TaskHandle_t higher_prio_hdl;
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(task_test_eg_prio, "task_test_eg_prio", 2048, (வெறுமை *)xTaskGetCurrentTaskHandle(), CONFIG_UNITY_FREERTOS_PRIORITY + 1, &higher_prio_hdl, CONFIG_UNITY_FREERTOS_CPU));

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
#எனில் SOC_GPTIMER_SUPPORTED
/*
 * Test event group Trace Facility functions such as
 * xEventGroupClearBitsFromISR(), xEventGroupSetBitsFromISR()
 */

//Use a timer to trigger an ISr
#define BITS            0xAA
நிலையான gptimer_handle_t gptimer;
நிலையான bool test_set_bits;
நிலையான bool test_clear_bits;

நிலையான bool on_timer_alarm_cb(gptimer_handle_t timer, மாறிலி gptimer_alarm_event_data_t *edata, வெறுமை *user_ctx)
{
    BaseType_t task_woken = pdFALSE;

    gptimer_stop(timer);

    எனில் (test_set_bits) {
        xEventGroupSetBitsFromISR(eg, BITS, &task_woken);
        test_set_bits = false;
    } இல்லையெனில் எனில் (test_clear_bits) {
        xEventGroupClearBitsFromISR(eg, BITS);
        xSemaphoreGiveFromISR(done_sem, &task_woken);
        test_clear_bits = false;
    }
    //Switch context if necessary
    திரும்பு task_woken == pdTRUE;
}

TEST_CASE("FreeRTOS Event Group ISR", "[freertos]")
{
    done_sem = x_Semaphore_Create_இருமம்();
    eg = xEventGroupCreate();
    test_set_bits = false;
    test_clear_bits = false;
    //Setup timer for ISR
    gptimer_config_t config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_மொத்தம்_UP,
        .resolution_hz = 1000000,
    };
    TEST_ESP_OK(gptimer_new_timer(&config, &gptimer));
    gptimer_alarm_config_t alarm_config = {
        .reload_மொத்தம் = 0,
        .alarm_மொத்தம் = 200000,
    };
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_timer_alarm_cb,
    };
    TEST_ESP_OK(gptimer_register_event_callbacks(gptimer, &cbs, வெற்று));
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
    TEST_ESP_OK(gptimer_set_raw_மொத்தம்(gptimer, 0));
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
