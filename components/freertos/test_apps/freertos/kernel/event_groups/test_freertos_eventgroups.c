/*
 * SPDX-FileCopyrightText: 2022-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#अन्तर्भाव <stdio.h>
#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"
#अन्तर्भाव "freertos/semphr.h"
#अन्तर्भाव "freertos/queue.h"
#अन्तर्भाव "freertos/event_groups.h"
#अन्तर्भाव "driver/gptimer.h"
#अन्तर्भाव "unity.h"
#अन्तर्भाव "unity_test_utils.h"

#define अङ्क_TASKS               8
#define ALL_TASK_BITS           ((1 << अङ्क_TASKS) - 1)
#define BIT_CALL(task)          (1 << (task))
#define BIT_RESPONSE(task)      (1 << ((task) + अङ्क_TASKS))
#define ALL_CALL_BITS           (ALL_TASK_BITS)
#define ALL_RESPONSE_BITS       (ALL_TASK_BITS << अङ्क_TASKS)

स्थिर अचर पूर्णाङ्क गन्ती_2 = 1000;
स्थिर EventGroupHandle_t eg;
स्थिर SemaphoreHandle_t done_sem;

स्थिर शून्य task_event_group_call_response(शून्य *param)
{
    पूर्णाङ्क task_अङ्क = (पूर्णाङ्क)param;

    म_लिखो("Started %d\n", task_अङ्क);

    लागि (पूर्णाङ्क i = 0; i < गन्ती_2; i++) {
        /* Wait until the common "call" bit is set, starts off all tasks
           (clear on return) */
        TEST_ASSERT(xEventGroupWaitBits(eg, BIT_CALL(task_अङ्क), true, false, portMAX_DELAY));

        /* Set our individual "response" bit */
        xEventGroupSetBits(eg, BIT_RESPONSE(task_अङ्क));
    }

    म_लिखो("Task %d done\n", task_अङ्क);
    xSemaphoreGive(done_sem);
    // Wait to be deleted
    vTaskSuspend(रिक्त);
}

TEST_CASE("FreeRTOS Event Groups", "[freertos]")
{
    eg = xEventGroupCreate();
    done_sem = xSemaphoreCreateCounting(अङ्क_TASKS, 0);

    TaskHandle_t task_handles[अङ्क_TASKS];

    /* Note: task_event_group_call_response all have higher priority than this task, so on this core
       they will always preempt this task.

       This is important because we need to know all tasks have blocked on their own BIT_CALL(task_num) each time we
       signal it, or they get out of sync.
     */
    लागि (पूर्णाङ्क c = 0; c < अङ्क_TASKS; c++) {
        xTaskCreatePinnedToCore(task_event_group_call_response, "tsk_call_resp", 4096, (शून्य *)c, configMAX_PRIORITIES - 1, &task_handles[c], c % CONFIG_FREERTOS_सङ्ख्या_OF_CORES);
    }

    /* Tasks all start instantly, but this task will resume running at the same time as the higher priority tasks on the
       other processor may still be setting up, so allow time for them to also block on BIT_CALL()... */
    vTaskDelay(10);

    लागि (पूर्णाङ्क i = 0; i < गन्ती_2; i++) {
        /* signal all the "CALL" bits of each task */
        xEventGroupSetBits(eg, ALL_CALL_BITS);

        /* Wait until all tasks have set their respective response bits */
        TEST_ASSERT_EQUAL_HEX16(ALL_RESPONSE_BITS, xEventGroupWaitBits(eg, ALL_RESPONSE_BITS, true, true, portMAX_DELAY));
    }

    /* Ensure all tasks have suspend themselves */
    लागि (पूर्णाङ्क c = 0; c < अङ्क_TASKS; c++) {
        TEST_ASSERT(xSemaphoreTake(done_sem, 100 / portTICK_PERIOD_MS));
    }

    लागि (पूर्णाङ्क c = 0; c < अङ्क_TASKS; c++) {
        unity_utils_task_delete(task_handles[c]);
    }

    vSemaphoreDelete(done_sem);
    vEventGroupDelete(eg);
}

स्थिर शून्य task_test_sync(शून्य *param)
{
    पूर्णाङ्क task_अङ्क = (पूर्णाङ्क)param;

    म_लिखो("Started %d\n", task_अङ्क);

    लागि (पूर्णाङ्क i = 0; i < गन्ती_2; i++) {
        /* set our bit, and wait on all tasks to set their bits */
        xEventGroupSync(eg, BIT_CALL(task_अङ्क), ALL_CALL_BITS, portMAX_DELAY);
        /* clear our bit */
        xEventGroupClearBits(eg, BIT_CALL(task_अङ्क));
    }
    पूर्णाङ्क after_done = xEventGroupSetBits(eg, BIT_RESPONSE(task_अङ्क));

    म_लिखो("Done %d = 0x%08x\n", task_अङ्क, after_done);

    xSemaphoreGive(done_sem);
    vTaskDelete(रिक्त);
}

TEST_CASE("FreeRTOS Event Group Sync", "[freertos]")
{
    eg = xEventGroupCreate();
    done_sem = xSemaphoreCreateCounting(अङ्क_TASKS, 0);

    लागि (पूर्णाङ्क c = 0; c < अङ्क_TASKS; c++) {
        xTaskCreatePinnedToCore(task_test_sync, "task_test_sync", 4096, (शून्य *)c, configMAX_PRIORITIES - 1, रिक्त, c % CONFIG_FREERTOS_सङ्ख्या_OF_CORES);
    }

    लागि (पूर्णाङ्क c = 0; c < अङ्क_TASKS; c++) {
        म_लिखो("Waiting on %d (0x%08x)\n", c, BIT_RESPONSE(c));
        TEST_ASSERT(xEventGroupWaitBits(eg, BIT_RESPONSE(c), false, false, portMAX_DELAY));
    }

    /* Ensure all tasks cleaned up correctly */
    लागि (पूर्णाङ्क c = 0; c < अङ्क_TASKS; c++) {
        TEST_ASSERT(xSemaphoreTake(done_sem, 100 / portTICK_PERIOD_MS));
    }

    vSemaphoreDelete(done_sem);
    vEventGroupDelete(eg);
}

स्थिर TaskHandle_t run_order[2];
स्थिर uint32_t run_order_index = 0;

शून्य task_test_eg_prio(शून्य *arg)
{
    TaskHandle_t main_task_hdl = (TaskHandle_t)arg;

    /* Notify the main task that this task has been created */
    xTaskNotifyGive(main_task_hdl);

    /* Wait for the event group bits to be set */
    TEST_ASSERT_EQUAL(1, xEventGroupWaitBits(eg, 1, pdTRUE, pdTRUE, portMAX_DELAY));

    /* Record the task handle in the run order array */
    run_order[run_order_index++] = xTaskGetCurrentTaskHandle();

    /* Suspend the task */
    vTaskSuspend(रिक्त);
}

TEST_CASE("FreeRTOS Event Groups do not cause priority inversion when higher priority task is unblocked", "[freertos]")
{
    run_order[0] = रिक्त;
    run_order[1] = रिक्त;
    run_order_index = 0;

    /* Initialize the event group */
    eg = xEventGroupCreate();

    /* Create a task with higher priority than the task that will set the event group bits */
    TaskHandle_t higher_prio_hdl;
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(task_test_eg_prio, "task_test_eg_prio", 2048, (शून्य *)xTaskGetCurrentTaskHandle(), CONFIG_UNITY_FREERTOS_PRIORITY + 1, &higher_prio_hdl, CONFIG_UNITY_FREERTOS_CPU));

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
#यदि SOC_GPTIMER_SUPPORTED
/*
 * Test event group Trace Facility functions such as
 * xEventGroupClearBitsFromISR(), xEventGroupSetBitsFromISR()
 */

//Use a timer to trigger an ISr
#define BITS            0xAA
स्थिर gptimer_handle_t gptimer;
स्थिर bool test_set_bits;
स्थिर bool test_clear_bits;

स्थिर bool on_timer_alarm_cb(gptimer_handle_t timer, अचर gptimer_alarm_event_data_t *edata, शून्य *user_ctx)
{
    BaseType_t task_woken = pdFALSE;

    gptimer_stop(timer);

    यदि (test_set_bits) {
        xEventGroupSetBitsFromISR(eg, BITS, &task_woken);
        test_set_bits = false;
    } अन्यथा यदि (test_clear_bits) {
        xEventGroupClearBitsFromISR(eg, BITS);
        xSemaphoreGiveFromISR(done_sem, &task_woken);
        test_clear_bits = false;
    }
    //Switch context if necessary
    फर्काऊ task_woken == pdTRUE;
}

TEST_CASE("FreeRTOS Event Group ISR", "[freertos]")
{
    done_sem = x_Semaphore_Create_द्विआधारी();
    eg = xEventGroupCreate();
    test_set_bits = false;
    test_clear_bits = false;
    //Setup timer for ISR
    gptimer_config_t config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_गन्ती_UP,
        .resolution_hz = 1000000,
    };
    TEST_ESP_OK(gptimer_new_timer(&config, &gptimer));
    gptimer_alarm_config_t alarm_config = {
        .reload_गन्ती = 0,
        .alarm_गन्ती = 200000,
    };
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_timer_alarm_cb,
    };
    TEST_ESP_OK(gptimer_register_event_callbacks(gptimer, &cbs, रिक्त));
    TEST_ESP_OK(gptimer_enable(gptimer));
    TEST_ESP_OK(gptimer_set_alarm_action(gptimer, &alarm_config));

    //Test set bits
    म_लिखो("test set bits\r\n");
    test_set_bits = true;
    TEST_ESP_OK(gptimer_start(gptimer));
    TEST_ASSERT_EQUAL(BITS, xEventGroupWaitBits(eg, BITS, pdFALSE, pdTRUE, portMAX_DELAY));     //Let ISR set event group bits

    //Test clear bits
    म_लिखो("test clear bits\r\n");
    xEventGroupSetBits(eg, BITS);                   //Set bits to be cleared
    test_clear_bits = true;
    TEST_ESP_OK(gptimer_set_raw_गन्ती(gptimer, 0));
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
