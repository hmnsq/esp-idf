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

#परिभाषय अङ्कः_TASKS               8
#परिभाषय ALL_TASK_द्वयङ्काः           ((1 << अङ्कः_TASKS) - 1)
#परिभाषय BIT_CALL(task)          (1 << (task))
#परिभाषय BIT_RESPONSE(task)      (1 << ((task) + अङ्कः_TASKS))
#परिभाषय ALL_CALL_द्वयङ्काः           (ALL_TASK_द्वयङ्काः)
#परिभाषय ALL_RESPONSE_द्वयङ्काः       (ALL_TASK_द्वयङ्काः << अङ्कः_TASKS)

स्थिर ध्रुव पूर्णाङ्क गणनम्_2 = 1000;
स्थिर Event_गणः_Handle_t eg;
स्थिर SemaphoreHandle_t समाप्तम्_sem;

स्थिर शून्य task_event_गणः_call_response(शून्य *param)
{
    पूर्णाङ्क task_अङ्कः = (पूर्णाङ्क)param;

    प्रारूप_लिख("Started %d\n", task_अङ्कः);

    प्रति (पूर्णाङ्क i = 0; i < गणनम्_2; i++) {
        /* Wait until the common "call" bit is set, starts off all tasks
           (clear on return) */
        TEST_ASSERT(x_Event_गणः_Wait_द्वयङ्काः(eg, BIT_CALL(task_अङ्कः), true, false, पत्तनम्_MAX_DELAY));

        /* Set our individual "response" bit */
        x_Event_गणः_समुच्चयः_द्वयङ्काः(eg, BIT_RESPONSE(task_अङ्कः));
    }

    प्रारूप_लिख("Task %d done\n", task_अङ्कः);
    xSemaphoreGive(समाप्तम्_sem);
    // Wait to be deleted
    vTaskSuspend(रिक्त);
}

TEST_CASE("FreeRTOS Event Groups", "[freertos]")
{
    eg = x_Event_गणः_Create();
    समाप्तम्_sem = xSemaphoreCreateCounting(अङ्कः_TASKS, 0);

    TaskHandle_t task_handles[अङ्कः_TASKS];

    /* Note: task_event_group_call_response all have higher priority than this task, so on this core
       they will always preempt this task.

       This is important because we need to know all tasks have blocked on their own BIT_CALL(task_num) each time we
       signal it, or they get out of sync.
     */
    प्रति (पूर्णाङ्क c = 0; c < अङ्कः_TASKS; c++) {
        x_Task_Create_Pinned_गन्तव्यम्_Core(task_event_गणः_call_response, "tsk_call_resp", 4096, (शून्य *)c, configMAX_PRIORITIES - 1, &task_handles[c], c % CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES);
    }

    /* Tasks all start instantly, but this task will resume running at the same time as the higher priority tasks on the
       other processor may still be setting up, so allow time for them to also block on BIT_CALL()... */
    vTaskDelay(10);

    प्रति (पूर्णाङ्क i = 0; i < गणनम्_2; i++) {
        /* signal all the "CALL" bits of each task */
        x_Event_गणः_समुच्चयः_द्वयङ्काः(eg, ALL_CALL_द्वयङ्काः);

        /* Wait until all tasks have set their respective response bits */
        TEST_ASSERT_EQUAL_HEX16(ALL_RESPONSE_द्वयङ्काः, x_Event_गणः_Wait_द्वयङ्काः(eg, ALL_RESPONSE_द्वयङ्काः, true, true, पत्तनम्_MAX_DELAY));
    }

    /* Ensure all tasks have suspend themselves */
    प्रति (पूर्णाङ्क c = 0; c < अङ्कः_TASKS; c++) {
        TEST_ASSERT(xSemaphoreTake(समाप्तम्_sem, 100 / पत्तनम्_TICK_PERIOD_MS));
    }

    प्रति (पूर्णाङ्क c = 0; c < अङ्कः_TASKS; c++) {
        unity_utils_task_delete(task_handles[c]);
    }

    vSemaphoreDelete(समाप्तम्_sem);
    v_Event_गणः_Delete(eg);
}

स्थिर शून्य task_test_sync(शून्य *param)
{
    पूर्णाङ्क task_अङ्कः = (पूर्णाङ्क)param;

    प्रारूप_लिख("Started %d\n", task_अङ्कः);

    प्रति (पूर्णाङ्क i = 0; i < गणनम्_2; i++) {
        /* set our bit, and wait on all tasks to set their bits */
        x_Event_गणः_Sync(eg, BIT_CALL(task_अङ्कः), ALL_CALL_द्वयङ्काः, पत्तनम्_MAX_DELAY);
        /* clear our bit */
        x_Event_गणः_Clear_द्वयङ्काः(eg, BIT_CALL(task_अङ्कः));
    }
    पूर्णाङ्क after_समाप्तम् = x_Event_गणः_समुच्चयः_द्वयङ्काः(eg, BIT_RESPONSE(task_अङ्कः));

    प्रारूप_लिख("Done %d = 0x%08x\n", task_अङ्कः, after_समाप्तम्);

    xSemaphoreGive(समाप्तम्_sem);
    vTaskDelete(रिक्त);
}

TEST_CASE("FreeRTOS Event Group Sync", "[freertos]")
{
    eg = x_Event_गणः_Create();
    समाप्तम्_sem = xSemaphoreCreateCounting(अङ्कः_TASKS, 0);

    प्रति (पूर्णाङ्क c = 0; c < अङ्कः_TASKS; c++) {
        x_Task_Create_Pinned_गन्तव्यम्_Core(task_test_sync, "task_test_sync", 4096, (शून्य *)c, configMAX_PRIORITIES - 1, रिक्त, c % CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES);
    }

    प्रति (पूर्णाङ्क c = 0; c < अङ्कः_TASKS; c++) {
        प्रारूप_लिख("Waiting on %d (0x%08x)\n", c, BIT_RESPONSE(c));
        TEST_ASSERT(x_Event_गणः_Wait_द्वयङ्काः(eg, BIT_RESPONSE(c), false, false, पत्तनम्_MAX_DELAY));
    }

    /* Ensure all tasks cleaned up correctly */
    प्रति (पूर्णाङ्क c = 0; c < अङ्कः_TASKS; c++) {
        TEST_ASSERT(xSemaphoreTake(समाप्तम्_sem, 100 / पत्तनम्_TICK_PERIOD_MS));
    }

    vSemaphoreDelete(समाप्तम्_sem);
    v_Event_गणः_Delete(eg);
}

स्थिर TaskHandle_t सततम्_क्रमः[2];
स्थिर uint32_t सततम्_क्रमः_सूचकांक = 0;

शून्य task_test_eg_prio(शून्य *arg)
{
    TaskHandle_t main_task_hdl = (TaskHandle_t)arg;

    /* Notify the main task that this task has been created */
    xTaskNotifyGive(main_task_hdl);

    /* Wait for the event group bits to be set */
    TEST_ASSERT_EQUAL(1, x_Event_गणः_Wait_द्वयङ्काः(eg, 1, pdTRUE, pdTRUE, पत्तनम्_MAX_DELAY));

    /* Record the task handle in the run order array */
    सततम्_क्रमः[सततम्_क्रमः_सूचकांक++] = xTaskGetCurrentTaskHandle();

    /* Suspend the task */
    vTaskSuspend(रिक्त);
}

TEST_CASE("FreeRTOS Event Groups do not cause priority inversion when higher priority task is unblocked", "[freertos]")
{
    सततम्_क्रमः[0] = रिक्त;
    सततम्_क्रमः[1] = रिक्त;
    सततम्_क्रमः_सूचकांक = 0;

    /* Initialize the event group */
    eg = x_Event_गणः_Create();

    /* Create a task with higher priority than the task that will set the event group bits */
    TaskHandle_t higher_prio_hdl;
    TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_गन्तव्यम्_Core(task_test_eg_prio, "task_test_eg_prio", 2048, (शून्य *)xTaskGetCurrentTaskHandle(), CONFIG_UNITY_FREERTOS_प्राथमिकता + 1, &higher_prio_hdl, CONFIG_UNITY_FREERTOS_CPU));

    /* Wait for the task to be created */
    ulTaskNotifyTake(pdTRUE, पत्तनम्_MAX_DELAY);

    /* Set the event group bits */
    x_Event_गणः_समुच्चयः_द्वयङ्काः(eg, 1);

    /* Record the task handle in the run order array */
    सततम्_क्रमः[सततम्_क्रमः_सूचकांक++] = xTaskGetCurrentTaskHandle();

    /* Verify that the higher priority task was unblocked and immediately scheduled and the lower priority task was preempted */
    TEST_ASSERT_EQUAL(higher_prio_hdl, सततम्_क्रमः[0]);
    TEST_ASSERT_EQUAL(xTaskGetCurrentTaskHandle(), सततम्_क्रमः[1]);

    /* Clean up */
    v_Event_गणः_Delete(eg);
    vTaskDelete(higher_prio_hdl);
}

/*-----------------Test case for event group trace facilities-----------------*/
#यदि_परिभाषितम्  CONFIG_FREERTOS_USE_TRACE_FACILITY
#यदि SOC_GPTIMER_SUPPORTED
/*
 * Test event group Trace Facility functions such as
 * xEventGroupClearBitsFromISR(), xEventGroupSetBitsFromISR()
 */

//Use a timer to trigger an ISr
#परिभाषय द्वयङ्काः            0xAA
स्थिर gptimer_handle_t gptimer;
स्थिर bool test_समुच्चयः_द्वयङ्काः;
स्थिर bool test_clear_द्वयङ्काः;

स्थिर bool on_timer_alarm_cb(gptimer_handle_t timer, ध्रुव gptimer_alarm_event_दत्त_t *edata, शून्य *user_ctx)
{
    आधारः_Type_t task_woken = pdFALSE;

    gptimer_stop(timer);

    यदि (test_समुच्चयः_द्वयङ्काः) {
        x_Event_गणः_समुच्चयः_द्वयङ्काः_From_ISR(eg, द्वयङ्काः, &task_woken);
        test_समुच्चयः_द्वयङ्काः = false;
    } अन्यथा यदि (test_clear_द्वयङ्काः) {
        x_Event_गणः_Clear_द्वयङ्काः_From_ISR(eg, द्वयङ्काः);
        xSemaphoreGiveFromISR(समाप्तम्_sem, &task_woken);
        test_clear_द्वयङ्काः = false;
    }
    //Switch context if necessary
    प्रत्यागम task_woken == pdTRUE;
}

TEST_CASE("FreeRTOS Event Group ISR", "[freertos]")
{
    समाप्तम्_sem = x_Semaphore_Create_द्विमान();
    eg = x_Event_गणः_Create();
    test_समुच्चयः_द्वयङ्काः = false;
    test_clear_द्वयङ्काः = false;
    //Setup timer for ISR
    gptimer_config_t config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_गणनम्_ऊर्ध्वम्,
        .resolution_hz = 1000000,
    };
    TEST_ESP_OK(gptimer_new_timer(&config, &gptimer));
    gptimer_alarm_config_t alarm_config = {
        .reload_गणनम् = 0,
        .alarm_गणनम् = 200000,
    };
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_timer_alarm_cb,
    };
    TEST_ESP_OK(gptimer_पञ्जिकाकोशः_event_callbacks(gptimer, &cbs, रिक्त));
    TEST_ESP_OK(gptimer_enable(gptimer));
    TEST_ESP_OK(gptimer_समुच्चयः_alarm_action(gptimer, &alarm_config));

    //Test set bits
    प्रारूप_लिख("test set bits\r\n");
    test_समुच्चयः_द्वयङ्काः = true;
    TEST_ESP_OK(gptimer_start(gptimer));
    TEST_ASSERT_EQUAL(द्वयङ्काः, x_Event_गणः_Wait_द्वयङ्काः(eg, द्वयङ्काः, pdFALSE, pdTRUE, पत्तनम्_MAX_DELAY));     //Let ISR set event group bits

    //Test clear bits
    प्रारूप_लिख("test clear bits\r\n");
    x_Event_गणः_समुच्चयः_द्वयङ्काः(eg, द्वयङ्काः);                   //Set bits to be cleared
    test_clear_द्वयङ्काः = true;
    TEST_ESP_OK(gptimer_समुच्चयः_raw_गणनम्(gptimer, 0));
    TEST_ESP_OK(gptimer_start(gptimer));
    xSemaphoreTake(समाप्तम्_sem, पत्तनम्_MAX_DELAY);        //Wait for ISR to clear bits
    vTaskDelay(10);                                 //Event group clear bits runs via daemon task, delay so daemon can run
    TEST_ASSERT_EQUAL(0, x_Event_गणः_Get_द्वयङ्काः(eg));   //Check bits are cleared

    //Clean up
    TEST_ESP_OK(gptimer_disable(gptimer));
    TEST_ESP_OK(gptimer_del_timer(gptimer));
    v_Event_गणः_Delete(eg);
    vSemaphoreDelete(समाप्तम्_sem);
    vTaskDelay(10);     //Give time for idle task to clear up deleted tasks
}
#यदि_अन्तः //SOC_GPTIMER_SUPPORTED
#यदि_अन्तः      //CONFIG_FREERTOS_USE_TRACE_FACILITY
