/*
 * SPDX-FileCopyrightText: 2022-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "freertos/semphr.h"
#ಅನ್ತರ್ಭಾವ "freertos/queue.h"
#ಅನ್ತರ್ಭಾವ "freertos/event_groups.h"
#ಅನ್ತರ್ಭಾವ "driver/gptimer.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "unity_test_utils.h"

#ವ್ಯಾಖ್ಯಾನಿಸು ಅಂಕೆ_TASKS               8
#ವ್ಯಾಖ್ಯಾನಿಸು ALL_TASK_ಬಿಟ್ಟುಗಳು           ((1 << ಅಂಕೆ_TASKS) - 1)
#ವ್ಯಾಖ್ಯಾನಿಸು ಬಿಟ್_CALL(task)          (1 << (task))
#ವ್ಯಾಖ್ಯಾನಿಸು ಬಿಟ್_RESPONSE(task)      (1 << ((task) + ಅಂಕೆ_TASKS))
#ವ್ಯಾಖ್ಯಾನಿಸು ALL_CALL_ಬಿಟ್ಟುಗಳು           (ALL_TASK_ಬಿಟ್ಟುಗಳು)
#ವ್ಯಾಖ್ಯಾನಿಸು ALL_RESPONSE_ಬಿಟ್ಟುಗಳು       (ALL_TASK_ಬಿಟ್ಟುಗಳು << ಅಂಕೆ_TASKS)

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3 = 1000;
ಸ್ಥಿರ Event_ಗುಂಪು_Handle_t eg;
ಸ್ಥಿರ SemaphoreHandle_t ಮುಗಿದ_sem;

ಸ್ಥಿರ ಶೂನ್ಯ task_event_ಗುಂಪು_call_response(ಶೂನ್ಯ *param)
{
    ಪೂರ್ಣಾಂಕ task_ಅಂಕೆ = (ಪೂರ್ಣಾಂಕ)param;

    ಬರೆ("Started %d\n", task_ಅಂಕೆ);

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಎಣಿಕೆ_3; i++) {
        /* Wait until the common "call" bit is set, starts off all tasks
           (clear on return) */
        TEST_ASSERT(x_Event_ಗುಂಪು_Wait_ಬಿಟ್ಟುಗಳು(eg, ಬಿಟ್_CALL(task_ಅಂಕೆ), true, false, ಪೋರ್ಟ್_MAX_DELAY));

        /* Set our individual "response" bit */
        x_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು(eg, ಬಿಟ್_RESPONSE(task_ಅಂಕೆ));
    }

    ಬರೆ("Task %d done\n", task_ಅಂಕೆ);
    xSemaphoreGive(ಮುಗಿದ_sem);
    // Wait to be deleted
    vTaskSuspend(ಖಾಲಿ);
}

TEST_CASE("FreeRTOS Event Groups", "[freertos]")
{
    eg = x_Event_ಗುಂಪು_Create();
    ಮುಗಿದ_sem = xSemaphoreCreateCounting(ಅಂಕೆ_TASKS, 0);

    TaskHandle_t task_handles[ಅಂಕೆ_TASKS];

    /* Note: task_event_group_call_response all have higher priority than this task, so on this core
       they will always preempt this task.

       This is important because we need to know all tasks have blocked on their own BIT_CALL(task_num) each time we
       signal it, or they get out of sync.
     */
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ c = 0; c < ಅಂಕೆ_TASKS; c++) {
        x_Task_Create_Pinned_ಗಮ್ಯ_Core(task_event_ಗುಂಪು_call_response, "tsk_call_resp", 4096, (ಶೂನ್ಯ *)c, configMAX_PRIORITIES - 1, &task_handles[c], c % CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES);
    }

    /* Tasks all start instantly, but this task will resume running at the same time as the higher priority tasks on the
       other processor may still be setting up, so allow time for them to also block on BIT_CALL()... */
    vTaskDelay(10);

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಎಣಿಕೆ_3; i++) {
        /* signal all the "CALL" bits of each task */
        x_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು(eg, ALL_CALL_ಬಿಟ್ಟುಗಳು);

        /* Wait until all tasks have set their respective response bits */
        TEST_ASSERT_EQUAL_HEX16(ALL_RESPONSE_ಬಿಟ್ಟುಗಳು, x_Event_ಗುಂಪು_Wait_ಬಿಟ್ಟುಗಳು(eg, ALL_RESPONSE_ಬಿಟ್ಟುಗಳು, true, true, ಪೋರ್ಟ್_MAX_DELAY));
    }

    /* Ensure all tasks have suspend themselves */
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ c = 0; c < ಅಂಕೆ_TASKS; c++) {
        TEST_ASSERT(xSemaphoreTake(ಮುಗಿದ_sem, 100 / ಪೋರ್ಟ್_TICK_PERIOD_MS));
    }

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ c = 0; c < ಅಂಕೆ_TASKS; c++) {
        unity_utils_task_delete(task_handles[c]);
    }

    vSemaphoreDelete(ಮುಗಿದ_sem);
    v_Event_ಗುಂಪು_Delete(eg);
}

ಸ್ಥಿರ ಶೂನ್ಯ task_test_sync(ಶೂನ್ಯ *param)
{
    ಪೂರ್ಣಾಂಕ task_ಅಂಕೆ = (ಪೂರ್ಣಾಂಕ)param;

    ಬರೆ("Started %d\n", task_ಅಂಕೆ);

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಎಣಿಕೆ_3; i++) {
        /* set our bit, and wait on all tasks to set their bits */
        x_Event_ಗುಂಪು_Sync(eg, ಬಿಟ್_CALL(task_ಅಂಕೆ), ALL_CALL_ಬಿಟ್ಟುಗಳು, ಪೋರ್ಟ್_MAX_DELAY);
        /* clear our bit */
        x_Event_ಗುಂಪು_Clear_ಬಿಟ್ಟುಗಳು(eg, ಬಿಟ್_CALL(task_ಅಂಕೆ));
    }
    ಪೂರ್ಣಾಂಕ after_ಮುಗಿದ = x_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು(eg, ಬಿಟ್_RESPONSE(task_ಅಂಕೆ));

    ಬರೆ("Done %d = 0x%08x\n", task_ಅಂಕೆ, after_ಮುಗಿದ);

    xSemaphoreGive(ಮುಗಿದ_sem);
    vTaskDelete(ಖಾಲಿ);
}

TEST_CASE("FreeRTOS Event Group Sync", "[freertos]")
{
    eg = x_Event_ಗುಂಪು_Create();
    ಮುಗಿದ_sem = xSemaphoreCreateCounting(ಅಂಕೆ_TASKS, 0);

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ c = 0; c < ಅಂಕೆ_TASKS; c++) {
        x_Task_Create_Pinned_ಗಮ್ಯ_Core(task_test_sync, "task_test_sync", 4096, (ಶೂನ್ಯ *)c, configMAX_PRIORITIES - 1, ಖಾಲಿ, c % CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES);
    }

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ c = 0; c < ಅಂಕೆ_TASKS; c++) {
        ಬರೆ("Waiting on %d (0x%08x)\n", c, ಬಿಟ್_RESPONSE(c));
        TEST_ASSERT(x_Event_ಗುಂಪು_Wait_ಬಿಟ್ಟುಗಳು(eg, ಬಿಟ್_RESPONSE(c), false, false, ಪೋರ್ಟ್_MAX_DELAY));
    }

    /* Ensure all tasks cleaned up correctly */
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ c = 0; c < ಅಂಕೆ_TASKS; c++) {
        TEST_ASSERT(xSemaphoreTake(ಮುಗಿದ_sem, 100 / ಪೋರ್ಟ್_TICK_PERIOD_MS));
    }

    vSemaphoreDelete(ಮುಗಿದ_sem);
    v_Event_ಗುಂಪು_Delete(eg);
}

ಸ್ಥಿರ TaskHandle_t ಸತತ_ಕ್ರಮ[2];
ಸ್ಥಿರ uint32_t ಸತತ_ಕ್ರಮ_ಸೂಚ್ಯಂಕ = 0;

ಶೂನ್ಯ task_test_eg_prio(ಶೂನ್ಯ *arg)
{
    TaskHandle_t main_task_hdl = (TaskHandle_t)arg;

    /* Notify the main task that this task has been created */
    xTaskNotifyGive(main_task_hdl);

    /* Wait for the event group bits to be set */
    TEST_ASSERT_EQUAL(1, x_Event_ಗುಂಪು_Wait_ಬಿಟ್ಟುಗಳು(eg, 1, pdTRUE, pdTRUE, ಪೋರ್ಟ್_MAX_DELAY));

    /* Record the task handle in the run order array */
    ಸತತ_ಕ್ರಮ[ಸತತ_ಕ್ರಮ_ಸೂಚ್ಯಂಕ++] = xTaskGetCurrentTaskHandle();

    /* Suspend the task */
    vTaskSuspend(ಖಾಲಿ);
}

TEST_CASE("FreeRTOS Event Groups do not cause priority inversion when higher priority task is unblocked", "[freertos]")
{
    ಸತತ_ಕ್ರಮ[0] = ಖಾಲಿ;
    ಸತತ_ಕ್ರಮ[1] = ಖಾಲಿ;
    ಸತತ_ಕ್ರಮ_ಸೂಚ್ಯಂಕ = 0;

    /* Initialize the event group */
    eg = x_Event_ಗುಂಪು_Create();

    /* Create a task with higher priority than the task that will set the event group bits */
    TaskHandle_t higher_prio_hdl;
    TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_ಗಮ್ಯ_Core(task_test_eg_prio, "task_test_eg_prio", 2048, (ಶೂನ್ಯ *)xTaskGetCurrentTaskHandle(), CONFIG_UNITY_FREERTOS_ಆದ್ಯತೆ + 1, &higher_prio_hdl, CONFIG_UNITY_FREERTOS_CPU));

    /* Wait for the task to be created */
    ulTaskNotifyTake(pdTRUE, ಪೋರ್ಟ್_MAX_DELAY);

    /* Set the event group bits */
    x_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು(eg, 1);

    /* Record the task handle in the run order array */
    ಸತತ_ಕ್ರಮ[ಸತತ_ಕ್ರಮ_ಸೂಚ್ಯಂಕ++] = xTaskGetCurrentTaskHandle();

    /* Verify that the higher priority task was unblocked and immediately scheduled and the lower priority task was preempted */
    TEST_ASSERT_EQUAL(higher_prio_hdl, ಸತತ_ಕ್ರಮ[0]);
    TEST_ASSERT_EQUAL(xTaskGetCurrentTaskHandle(), ಸತತ_ಕ್ರಮ[1]);

    /* Clean up */
    v_Event_ಗುಂಪು_Delete(eg);
    vTaskDelete(higher_prio_hdl);
}

/*-----------------Test case for event group trace facilities-----------------*/
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ  CONFIG_FREERTOS_USE_TRACE_FACILITY
#ಆದರೆ SOC_GPTIMER_SUPPORTED
/*
 * Test event group Trace Facility functions such as
 * xEventGroupClearBitsFromISR(), xEventGroupSetBitsFromISR()
 */

//Use a timer to trigger an ISr
#ವ್ಯಾಖ್ಯಾನಿಸು ಬಿಟ್ಟುಗಳು            0xAA
ಸ್ಥಿರ gptimer_handle_t gptimer;
ಸ್ಥಿರ bool test_ಗಣ_ಬಿಟ್ಟುಗಳು;
ಸ್ಥಿರ bool test_clear_ಬಿಟ್ಟುಗಳು;

ಸ್ಥಿರ bool on_timer_alarm_cb(gptimer_handle_t timer, ಸ್ಥಿರಾಂಕ gptimer_alarm_event_ದತ್ತಾಂಶ_t *edata, ಶೂನ್ಯ *user_ctx)
{
    ತಳ_Type_t task_woken = pdFALSE;

    gptimer_stop(timer);

    ಆದರೆ (test_ಗಣ_ಬಿಟ್ಟುಗಳು) {
        x_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು_From_ISR(eg, ಬಿಟ್ಟುಗಳು, &task_woken);
        test_ಗಣ_ಬಿಟ್ಟುಗಳು = false;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (test_clear_ಬಿಟ್ಟುಗಳು) {
        x_Event_ಗುಂಪು_Clear_ಬಿಟ್ಟುಗಳು_From_ISR(eg, ಬಿಟ್ಟುಗಳು);
        xSemaphoreGiveFromISR(ಮುಗಿದ_sem, &task_woken);
        test_clear_ಬಿಟ್ಟುಗಳು = false;
    }
    //Switch context if necessary
    ಹಿಂತಿರುಗಿ task_woken == pdTRUE;
}

TEST_CASE("FreeRTOS Event Group ISR", "[freertos]")
{
    ಮುಗಿದ_sem = x_Semaphore_Create_ದ್ವಿಮಾನ();
    eg = x_Event_ಗುಂಪು_Create();
    test_ಗಣ_ಬಿಟ್ಟುಗಳು = false;
    test_clear_ಬಿಟ್ಟುಗಳು = false;
    //Setup timer for ISR
    gptimer_config_t config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_ಎಣಿಕೆ_ಮೇಲೆ,
        .resolution_hz = 1000000,
    };
    TEST_ESP_OK(gptimer_new_timer(&config, &gptimer));
    gptimer_alarm_config_t alarm_config = {
        .reload_ಎಣಿಕೆ = 0,
        .alarm_ಎಣಿಕೆ = 200000,
    };
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_timer_alarm_cb,
    };
    TEST_ESP_OK(gptimer_ರಿಜಿಸ್ಟರ್_event_callbacks(gptimer, &cbs, ಖಾಲಿ));
    TEST_ESP_OK(gptimer_enable(gptimer));
    TEST_ESP_OK(gptimer_ಗಣ_alarm_action(gptimer, &alarm_config));

    //Test set bits
    ಬರೆ("test set bits\r\n");
    test_ಗಣ_ಬಿಟ್ಟುಗಳು = true;
    TEST_ESP_OK(gptimer_ಆರಂಭ(gptimer));
    TEST_ASSERT_EQUAL(ಬಿಟ್ಟುಗಳು, x_Event_ಗುಂಪು_Wait_ಬಿಟ್ಟುಗಳು(eg, ಬಿಟ್ಟುಗಳು, pdFALSE, pdTRUE, ಪೋರ್ಟ್_MAX_DELAY));     //Let ISR set event group bits

    //Test clear bits
    ಬರೆ("test clear bits\r\n");
    x_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು(eg, ಬಿಟ್ಟುಗಳು);                   //Set bits to be cleared
    test_clear_ಬಿಟ್ಟುಗಳು = true;
    TEST_ESP_OK(gptimer_ಗಣ_raw_ಎಣಿಕೆ(gptimer, 0));
    TEST_ESP_OK(gptimer_ಆರಂಭ(gptimer));
    xSemaphoreTake(ಮುಗಿದ_sem, ಪೋರ್ಟ್_MAX_DELAY);        //Wait for ISR to clear bits
    vTaskDelay(10);                                 //Event group clear bits runs via daemon task, delay so daemon can run
    TEST_ASSERT_EQUAL(0, x_Event_ಗುಂಪು_Get_ಬಿಟ್ಟುಗಳು(eg));   //Check bits are cleared

    //Clean up
    TEST_ESP_OK(gptimer_disable(gptimer));
    TEST_ESP_OK(gptimer_del_timer(gptimer));
    v_Event_ಗುಂಪು_Delete(eg);
    vSemaphoreDelete(ಮುಗಿದ_sem);
    vTaskDelay(10);     //Give time for idle task to clear up deleted tasks
}
#ಆದರೆ_ಅಂತ್ಯ //SOC_GPTIMER_SUPPORTED
#ಆದರೆ_ಅಂತ್ಯ      //CONFIG_FREERTOS_USE_TRACE_FACILITY
