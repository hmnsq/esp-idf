/*
 * SPDX-FileCopyrightText: 2022-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#समावेश <stdio.h>
#समावेश "sdkconfig.h"
#समावेश "freertos/FreeRTOS.h"
#समावेश "freertos/task.h"
#समावेश "freertos/semphr.h"
#समावेश "freertos/queue.h"
#समावेश "freertos/event_groups.h"
#समावेश "driver/gptimer.h"
#समावेश "unity.h"
#समावेश "unity_test_utils.h"

#घोषणा अंक_TASKS               8
#घोषणा ALL_TASK_BITS           ((1 << अंक_TASKS) - 1)
#घोषणा BIT_CALL(task)          (1 << (task))
#घोषणा BIT_RESPONSE(task)      (1 << ((task) + अंक_TASKS))
#घोषणा ALL_CALL_BITS           (ALL_TASK_BITS)
#घोषणा ALL_RESPONSE_BITS       (ALL_TASK_BITS << अंक_TASKS)

अटल स्थिर पूर्णांक गिनती_2 = 1000;
अटल EventGroupHandle_t eg;
अटल SemaphoreHandle_t done_sem;

अटल व्योम task_event_group_call_response(व्योम *param)
{
    पूर्णांक task_अंक = (पूर्णांक)param;

    म_लिखो("Started %d\n", task_अंक);

    क्रम (पूर्णांक i = 0; i < गिनती_2; i++) {
        /* Wait until the common "call" bit is set, starts off all tasks
           (clear on return) */
        TEST_ASSERT(xEventGroupWaitBits(eg, BIT_CALL(task_अंक), true, false, portMAX_DELAY));

        /* Set our individual "response" bit */
        xEventGroupSetBits(eg, BIT_RESPONSE(task_अंक));
    }

    म_लिखो("Task %d done\n", task_अंक);
    xSemaphoreGive(done_sem);
    // Wait to be deleted
    vTaskSuspend(शून्य);
}

TEST_CASE("FreeRTOS Event Groups", "[freertos]")
{
    eg = xEventGroupCreate();
    done_sem = xSemaphoreCreateCounting(अंक_TASKS, 0);

    TaskHandle_t task_handles[अंक_TASKS];

    /* Note: task_event_group_call_response all have higher priority than this task, so on this core
       they will always preempt this task.

       This is important because we need to know all tasks have blocked on their own BIT_CALL(task_num) each time we
       signal it, or they get out of sync.
     */
    क्रम (पूर्णांक c = 0; c < अंक_TASKS; c++) {
        xTaskCreatePinnedToCore(task_event_group_call_response, "tsk_call_resp", 4096, (व्योम *)c, configMAX_PRIORITIES - 1, &task_handles[c], c % CONFIG_FREERTOS_संख्या_OF_CORES);
    }

    /* Tasks all start instantly, but this task will resume running at the same time as the higher priority tasks on the
       other processor may still be setting up, so allow time for them to also block on BIT_CALL()... */
    vTaskDelay(10);

    क्रम (पूर्णांक i = 0; i < गिनती_2; i++) {
        /* signal all the "CALL" bits of each task */
        xEventGroupSetBits(eg, ALL_CALL_BITS);

        /* Wait until all tasks have set their respective response bits */
        TEST_ASSERT_EQUAL_HEX16(ALL_RESPONSE_BITS, xEventGroupWaitBits(eg, ALL_RESPONSE_BITS, true, true, portMAX_DELAY));
    }

    /* Ensure all tasks have suspend themselves */
    क्रम (पूर्णांक c = 0; c < अंक_TASKS; c++) {
        TEST_ASSERT(xSemaphoreTake(done_sem, 100 / portTICK_PERIOD_MS));
    }

    क्रम (पूर्णांक c = 0; c < अंक_TASKS; c++) {
        unity_utils_task_delete(task_handles[c]);
    }

    vSemaphoreDelete(done_sem);
    vEventGroupDelete(eg);
}

अटल व्योम task_test_sync(व्योम *param)
{
    पूर्णांक task_अंक = (पूर्णांक)param;

    म_लिखो("Started %d\n", task_अंक);

    क्रम (पूर्णांक i = 0; i < गिनती_2; i++) {
        /* set our bit, and wait on all tasks to set their bits */
        xEventGroupSync(eg, BIT_CALL(task_अंक), ALL_CALL_BITS, portMAX_DELAY);
        /* clear our bit */
        xEventGroupClearBits(eg, BIT_CALL(task_अंक));
    }
    पूर्णांक after_done = xEventGroupSetBits(eg, BIT_RESPONSE(task_अंक));

    म_लिखो("Done %d = 0x%08x\n", task_अंक, after_done);

    xSemaphoreGive(done_sem);
    vTaskDelete(शून्य);
}

TEST_CASE("FreeRTOS Event Group Sync", "[freertos]")
{
    eg = xEventGroupCreate();
    done_sem = xSemaphoreCreateCounting(अंक_TASKS, 0);

    क्रम (पूर्णांक c = 0; c < अंक_TASKS; c++) {
        xTaskCreatePinnedToCore(task_test_sync, "task_test_sync", 4096, (व्योम *)c, configMAX_PRIORITIES - 1, शून्य, c % CONFIG_FREERTOS_संख्या_OF_CORES);
    }

    क्रम (पूर्णांक c = 0; c < अंक_TASKS; c++) {
        म_लिखो("Waiting on %d (0x%08x)\n", c, BIT_RESPONSE(c));
        TEST_ASSERT(xEventGroupWaitBits(eg, BIT_RESPONSE(c), false, false, portMAX_DELAY));
    }

    /* Ensure all tasks cleaned up correctly */
    क्रम (पूर्णांक c = 0; c < अंक_TASKS; c++) {
        TEST_ASSERT(xSemaphoreTake(done_sem, 100 / portTICK_PERIOD_MS));
    }

    vSemaphoreDelete(done_sem);
    vEventGroupDelete(eg);
}

अटल TaskHandle_t run_order[2];
अटल uint32_t run_order_सूचकांक = 0;

व्योम task_test_eg_prio(व्योम *arg)
{
    TaskHandle_t main_task_hdl = (TaskHandle_t)arg;

    /* Notify the main task that this task has been created */
    xTaskNotifyGive(main_task_hdl);

    /* Wait for the event group bits to be set */
    TEST_ASSERT_EQUAL(1, xEventGroupWaitBits(eg, 1, pdTRUE, pdTRUE, portMAX_DELAY));

    /* Record the task handle in the run order array */
    run_order[run_order_सूचकांक++] = xTaskGetCurrentTaskHandle();

    /* Suspend the task */
    vTaskSuspend(शून्य);
}

TEST_CASE("FreeRTOS Event Groups do not cause priority inversion when higher priority task is unblocked", "[freertos]")
{
    run_order[0] = शून्य;
    run_order[1] = शून्य;
    run_order_सूचकांक = 0;

    /* Initialize the event group */
    eg = xEventGroupCreate();

    /* Create a task with higher priority than the task that will set the event group bits */
    TaskHandle_t higher_prio_hdl;
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(task_test_eg_prio, "task_test_eg_prio", 2048, (व्योम *)xTaskGetCurrentTaskHandle(), CONFIG_UNITY_FREERTOS_PRIORITY + 1, &higher_prio_hdl, CONFIG_UNITY_FREERTOS_CPU));

    /* Wait for the task to be created */
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    /* Set the event group bits */
    xEventGroupSetBits(eg, 1);

    /* Record the task handle in the run order array */
    run_order[run_order_सूचकांक++] = xTaskGetCurrentTaskHandle();

    /* Verify that the higher priority task was unblocked and immediately scheduled and the lower priority task was preempted */
    TEST_ASSERT_EQUAL(higher_prio_hdl, run_order[0]);
    TEST_ASSERT_EQUAL(xTaskGetCurrentTaskHandle(), run_order[1]);

    /* Clean up */
    vEventGroupDelete(eg);
    vTaskDelete(higher_prio_hdl);
}

/*-----------------Test case for event group trace facilities-----------------*/
#अगर_घोषित  CONFIG_FREERTOS_USE_TRACE_FACILITY
#अगर SOC_GPTIMER_SUPPORTED
/*
 * Test event group Trace Facility functions such as
 * xEventGroupClearBitsFromISR(), xEventGroupSetBitsFromISR()
 */

//Use a timer to trigger an ISr
#घोषणा BITS            0xAA
अटल gptimer_handle_t gptimer;
अटल bool test_set_bits;
अटल bool test_clear_bits;

अटल bool on_timer_alarm_cb(gptimer_handle_t timer, स्थिर gptimer_alarm_event_तथ्य_t *edata, व्योम *user_ctx)
{
    BaseType_t task_woken = pdFALSE;

    gptimer_stop(timer);

    अगर (test_set_bits) {
        xEventGroupSetBitsFromISR(eg, BITS, &task_woken);
        test_set_bits = false;
    } अन्यथा अगर (test_clear_bits) {
        xEventGroupClearBitsFromISR(eg, BITS);
        xSemaphoreGiveFromISR(done_sem, &task_woken);
        test_clear_bits = false;
    }
    //Switch context if necessary
    वापस task_woken == pdTRUE;
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
        .direction = GPTIMER_गिनती_UP,
        .resolution_hz = 1000000,
    };
    TEST_ESP_OK(gptimer_new_timer(&config, &gptimer));
    gptimer_alarm_config_t alarm_config = {
        .reload_गिनती = 0,
        .alarm_गिनती = 200000,
    };
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_timer_alarm_cb,
    };
    TEST_ESP_OK(gptimer_register_event_callbacks(gptimer, &cbs, शून्य));
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
    TEST_ESP_OK(gptimer_set_raw_गिनती(gptimer, 0));
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
#पूर्ण_अगर //SOC_GPTIMER_SUPPORTED
#पूर्ण_अगर      //CONFIG_FREERTOS_USE_TRACE_FACILITY
