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

#define ಅಂಕೆ_TASKS               8
#define ALL_TASK_BITS           ((1 << ಅಂಕೆ_TASKS) - 1)
#define BIT_CALL(task)          (1 << (task))
#define BIT_RESPONSE(task)      (1 << ((task) + ಅಂಕೆ_TASKS))
#define ALL_CALL_BITS           (ALL_TASK_BITS)
#define ALL_RESPONSE_BITS       (ALL_TASK_BITS << ಅಂಕೆ_TASKS)

ಸ್ಥಿರ ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ_2 = 1000;
ಸ್ಥಿರ EventGroupHandle_t eg;
ಸ್ಥಿರ SemaphoreHandle_t done_sem;

ಸ್ಥಿರ ಶೂನ್ಯ task_event_group_call_response(ಶೂನ್ಯ *param)
{
    ಪೂರ್ಣಾಙ್ಕ task_ಅಂಕೆ = (ಪೂರ್ಣಾಙ್ಕ)param;

    printf("Started %d\n", task_ಅಂಕೆ);

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ_2; i++) {
        /* Wait until the common "call" bit is set, starts off all tasks
           (clear on return) */
        TEST_ASSERT(xEventGroupWaitBits(eg, BIT_CALL(task_ಅಂಕೆ), true, false, portMAX_DELAY));

        /* Set our individual "response" bit */
        xEventGroupSetBits(eg, BIT_RESPONSE(task_ಅಂಕೆ));
    }

    printf("Task %d done\n", task_ಅಂಕೆ);
    xSemaphoreGive(done_sem);
    // Wait to be deleted
    vTaskSuspend(ರಿಕ್ತ);
}

TEST_CASE("FreeRTOS Event Groups", "[freertos]")
{
    eg = xEventGroupCreate();
    done_sem = xSemaphoreCreateCounting(ಅಂಕೆ_TASKS, 0);

    TaskHandle_t task_handles[ಅಂಕೆ_TASKS];

    /* Note: task_event_group_call_response all have higher priority than this task, so on this core
       they will always preempt this task.

       This is important because we need to know all tasks have blocked on their own BIT_CALL(task_num) each time we
       signal it, or they get out of sync.
     */
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ c = 0; c < ಅಂಕೆ_TASKS; c++) {
        xTaskCreatePinnedToCore(task_event_group_call_response, "tsk_call_resp", 4096, (ಶೂನ್ಯ *)c, configMAX_PRIORITIES - 1, &task_handles[c], c % CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES);
    }

    /* Tasks all start instantly, but this task will resume running at the same time as the higher priority tasks on the
       other processor may still be setting up, so allow time for them to also block on BIT_CALL()... */
    vTaskDelay(10);

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ_2; i++) {
        /* signal all the "CALL" bits of each task */
        xEventGroupSetBits(eg, ALL_CALL_BITS);

        /* Wait until all tasks have set their respective response bits */
        TEST_ASSERT_EQUAL_HEX16(ALL_RESPONSE_BITS, xEventGroupWaitBits(eg, ALL_RESPONSE_BITS, true, true, portMAX_DELAY));
    }

    /* Ensure all tasks have suspend themselves */
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ c = 0; c < ಅಂಕೆ_TASKS; c++) {
        TEST_ASSERT(xSemaphoreTake(done_sem, 100 / portTICK_PERIOD_MS));
    }

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ c = 0; c < ಅಂಕೆ_TASKS; c++) {
        unity_utils_task_delete(task_handles[c]);
    }

    vSemaphoreDelete(done_sem);
    vEventGroupDelete(eg);
}

ಸ್ಥಿರ ಶೂನ್ಯ task_test_sync(ಶೂನ್ಯ *param)
{
    ಪೂರ್ಣಾಙ್ಕ task_ಅಂಕೆ = (ಪೂರ್ಣಾಙ್ಕ)param;

    printf("Started %d\n", task_ಅಂಕೆ);

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ_2; i++) {
        /* set our bit, and wait on all tasks to set their bits */
        xEventGroupSync(eg, BIT_CALL(task_ಅಂಕೆ), ALL_CALL_BITS, portMAX_DELAY);
        /* clear our bit */
        xEventGroupClearBits(eg, BIT_CALL(task_ಅಂಕೆ));
    }
    ಪೂರ್ಣಾಙ್ಕ after_done = xEventGroupSetBits(eg, BIT_RESPONSE(task_ಅಂಕೆ));

    printf("Done %d = 0x%08x\n", task_ಅಂಕೆ, after_done);

    xSemaphoreGive(done_sem);
    vTaskDelete(ರಿಕ್ತ);
}

TEST_CASE("FreeRTOS Event Group Sync", "[freertos]")
{
    eg = xEventGroupCreate();
    done_sem = xSemaphoreCreateCounting(ಅಂಕೆ_TASKS, 0);

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ c = 0; c < ಅಂಕೆ_TASKS; c++) {
        xTaskCreatePinnedToCore(task_test_sync, "task_test_sync", 4096, (ಶೂನ್ಯ *)c, configMAX_PRIORITIES - 1, ರಿಕ್ತ, c % CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES);
    }

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ c = 0; c < ಅಂಕೆ_TASKS; c++) {
        printf("Waiting on %d (0x%08x)\n", c, BIT_RESPONSE(c));
        TEST_ASSERT(xEventGroupWaitBits(eg, BIT_RESPONSE(c), false, false, portMAX_DELAY));
    }

    /* Ensure all tasks cleaned up correctly */
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ c = 0; c < ಅಂಕೆ_TASKS; c++) {
        TEST_ASSERT(xSemaphoreTake(done_sem, 100 / portTICK_PERIOD_MS));
    }

    vSemaphoreDelete(done_sem);
    vEventGroupDelete(eg);
}

ಸ್ಥಿರ TaskHandle_t run_order[2];
ಸ್ಥಿರ uint32_t run_order_index = 0;

ಶೂನ್ಯ task_test_eg_prio(ಶೂನ್ಯ *arg)
{
    TaskHandle_t main_task_hdl = (TaskHandle_t)arg;

    /* Notify the main task that this task has been created */
    xTaskNotifyGive(main_task_hdl);

    /* Wait for the event group bits to be set */
    TEST_ASSERT_EQUAL(1, xEventGroupWaitBits(eg, 1, pdTRUE, pdTRUE, portMAX_DELAY));

    /* Record the task handle in the run order array */
    run_order[run_order_index++] = xTaskGetCurrentTaskHandle();

    /* Suspend the task */
    vTaskSuspend(ರಿಕ್ತ);
}

TEST_CASE("FreeRTOS Event Groups do not cause priority inversion when higher priority task is unblocked", "[freertos]")
{
    run_order[0] = ರಿಕ್ತ;
    run_order[1] = ರಿಕ್ತ;
    run_order_index = 0;

    /* Initialize the event group */
    eg = xEventGroupCreate();

    /* Create a task with higher priority than the task that will set the event group bits */
    TaskHandle_t higher_prio_hdl;
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(task_test_eg_prio, "task_test_eg_prio", 2048, (ಶೂನ್ಯ *)xTaskGetCurrentTaskHandle(), CONFIG_UNITY_FREERTOS_PRIORITY + 1, &higher_prio_hdl, CONFIG_UNITY_FREERTOS_CPU));

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
#ಯದಿ SOC_GPTIMER_SUPPORTED
/*
 * Test event group Trace Facility functions such as
 * xEventGroupClearBitsFromISR(), xEventGroupSetBitsFromISR()
 */

//Use a timer to trigger an ISr
#define BITS            0xAA
ಸ್ಥಿರ gptimer_handle_t gptimer;
ಸ್ಥಿರ bool test_set_bits;
ಸ್ಥಿರ bool test_clear_bits;

ಸ್ಥಿರ bool on_timer_alarm_cb(gptimer_handle_t timer, ಧ್ರುವ gptimer_alarm_event_data_t *edata, ಶೂನ್ಯ *user_ctx)
{
    BaseType_t task_woken = pdFALSE;

    gptimer_stop(timer);

    ಯದಿ (test_set_bits) {
        xEventGroupSetBitsFromISR(eg, BITS, &task_woken);
        test_set_bits = false;
    } ಅನ್ಯಥಾ ಯದಿ (test_clear_bits) {
        xEventGroupClearBitsFromISR(eg, BITS);
        xSemaphoreGiveFromISR(done_sem, &task_woken);
        test_clear_bits = false;
    }
    //Switch context if necessary
    ಪ್ರತ್ಯಾಗಮ task_woken == pdTRUE;
}

TEST_CASE("FreeRTOS Event Group ISR", "[freertos]")
{
    done_sem = x_Semaphore_Create_ದ್ವಿಮಾನ();
    eg = xEventGroupCreate();
    test_set_bits = false;
    test_clear_bits = false;
    //Setup timer for ISR
    gptimer_config_t config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_ಎಣಿಕೆ_UP,
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
    TEST_ESP_OK(gptimer_register_event_callbacks(gptimer, &cbs, ರಿಕ್ತ));
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
    TEST_ESP_OK(gptimer_set_raw_ಎಣಿಕೆ(gptimer, 0));
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
