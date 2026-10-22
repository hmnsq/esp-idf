/*
 * SPDX-FileCopyrightText: 2022-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن <stdio.h>
#ضمن "sdkconfig.h"
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "freertos/semphr.h"
#ضمن "freertos/queue.h"
#ضمن "freertos/event_groups.h"
#ضمن "driver/gptimer.h"
#ضمن "unity.h"
#ضمن "unity_test_utils.h"

#وصف_ڪريو انگ_TASKS               8
#وصف_ڪريو ALL_TASK_بٽ           ((1 << انگ_TASKS) - 1)
#وصف_ڪريو ٻٽو_انگ_CALL(task)          (1 << (task))
#وصف_ڪريو ٻٽو_انگ_RESPONSE(task)      (1 << ((task) + انگ_TASKS))
#وصف_ڪريو ALL_CALL_بٽ           (ALL_TASK_بٽ)
#وصف_ڪريو ALL_RESPONSE_بٽ       (ALL_TASK_بٽ << انگ_TASKS)

ساكن ثابت صحيح ڳڻپ_2 = 1000;
ساكن Event_گروهه_Handle_t eg;
ساكن SemaphoreHandle_t پورو_sem;

ساكن فراغ task_event_گروهه_call_response(فراغ *param)
{
    صحيح task_انگ = (صحيح)param;

    لکو("Started %d\n", task_انگ);

    لكل (صحيح i = 0; i < ڳڻپ_2; i++) {
        /* Wait until the common "call" bit is set, starts off all tasks
           (clear on return) */
        TEST_ASSERT(x_Event_گروهه_Wait_بٽ(eg, ٻٽو_انگ_CALL(task_انگ), true, false, پورٽ_MAX_DELAY));

        /* Set our individual "response" bit */
        x_Event_گروهه_سيٽ_بٽ(eg, ٻٽو_انگ_RESPONSE(task_انگ));
    }

    لکو("Task %d done\n", task_انگ);
    xSemaphoreGive(پورو_sem);
    // Wait to be deleted
    vTaskSuspend(عدم);
}

TEST_CASE("FreeRTOS Event Groups", "[freertos]")
{
    eg = x_Event_گروهه_Create();
    پورو_sem = xSemaphoreCreateCounting(انگ_TASKS, 0);

    TaskHandle_t task_handles[انگ_TASKS];

    /* Note: task_event_group_call_response all have higher priority than this task, so on this core
       they will always preempt this task.

       This is important because we need to know all tasks have blocked on their own BIT_CALL(task_num) each time we
       signal it, or they get out of sync.
     */
    لكل (صحيح c = 0; c < انگ_TASKS; c++) {
        x_Task_Create_Pinned_منزل_Core(task_event_گروهه_call_response, "tsk_call_resp", 4096, (فراغ *)c, configMAX_PRIORITIES - 1, &task_handles[c], c % CONFIG_FREERTOS_عدد_جو_CORES);
    }

    /* Tasks all start instantly, but this task will resume running at the same time as the higher priority tasks on the
       other processor may still be setting up, so allow time for them to also block on BIT_CALL()... */
    vTaskDelay(10);

    لكل (صحيح i = 0; i < ڳڻپ_2; i++) {
        /* signal all the "CALL" bits of each task */
        x_Event_گروهه_سيٽ_بٽ(eg, ALL_CALL_بٽ);

        /* Wait until all tasks have set their respective response bits */
        TEST_ASSERT_EQUAL_HEX16(ALL_RESPONSE_بٽ, x_Event_گروهه_Wait_بٽ(eg, ALL_RESPONSE_بٽ, true, true, پورٽ_MAX_DELAY));
    }

    /* Ensure all tasks have suspend themselves */
    لكل (صحيح c = 0; c < انگ_TASKS; c++) {
        TEST_ASSERT(xSemaphoreTake(پورو_sem, 100 / پورٽ_TICK_PERIOD_MS));
    }

    لكل (صحيح c = 0; c < انگ_TASKS; c++) {
        unity_utils_task_delete(task_handles[c]);
    }

    vSemaphoreDelete(پورو_sem);
    v_Event_گروهه_Delete(eg);
}

ساكن فراغ task_test_sync(فراغ *param)
{
    صحيح task_انگ = (صحيح)param;

    لکو("Started %d\n", task_انگ);

    لكل (صحيح i = 0; i < ڳڻپ_2; i++) {
        /* set our bit, and wait on all tasks to set their bits */
        x_Event_گروهه_Sync(eg, ٻٽو_انگ_CALL(task_انگ), ALL_CALL_بٽ, پورٽ_MAX_DELAY);
        /* clear our bit */
        x_Event_گروهه_Clear_بٽ(eg, ٻٽو_انگ_CALL(task_انگ));
    }
    صحيح after_پورو = x_Event_گروهه_سيٽ_بٽ(eg, ٻٽو_انگ_RESPONSE(task_انگ));

    لکو("Done %d = 0x%08x\n", task_انگ, after_پورو);

    xSemaphoreGive(پورو_sem);
    vTaskDelete(عدم);
}

TEST_CASE("FreeRTOS Event Group Sync", "[freertos]")
{
    eg = x_Event_گروهه_Create();
    پورو_sem = xSemaphoreCreateCounting(انگ_TASKS, 0);

    لكل (صحيح c = 0; c < انگ_TASKS; c++) {
        x_Task_Create_Pinned_منزل_Core(task_test_sync, "task_test_sync", 4096, (فراغ *)c, configMAX_PRIORITIES - 1, عدم, c % CONFIG_FREERTOS_عدد_جو_CORES);
    }

    لكل (صحيح c = 0; c < انگ_TASKS; c++) {
        لکو("Waiting on %d (0x%08x)\n", c, ٻٽو_انگ_RESPONSE(c));
        TEST_ASSERT(x_Event_گروهه_Wait_بٽ(eg, ٻٽو_انگ_RESPONSE(c), false, false, پورٽ_MAX_DELAY));
    }

    /* Ensure all tasks cleaned up correctly */
    لكل (صحيح c = 0; c < انگ_TASKS; c++) {
        TEST_ASSERT(xSemaphoreTake(پورو_sem, 100 / پورٽ_TICK_PERIOD_MS));
    }

    vSemaphoreDelete(پورو_sem);
    v_Event_گروهه_Delete(eg);
}

ساكن TaskHandle_t لاڳيتو_ترتيب[2];
ساكن uint32_t لاڳيتو_ترتيب_اشاريو = 0;

فراغ task_test_eg_prio(فراغ *arg)
{
    TaskHandle_t main_task_hdl = (TaskHandle_t)arg;

    /* Notify the main task that this task has been created */
    xTaskNotifyGive(main_task_hdl);

    /* Wait for the event group bits to be set */
    TEST_ASSERT_EQUAL(1, x_Event_گروهه_Wait_بٽ(eg, 1, pdTRUE, pdTRUE, پورٽ_MAX_DELAY));

    /* Record the task handle in the run order array */
    لاڳيتو_ترتيب[لاڳيتو_ترتيب_اشاريو++] = xTaskGetCurrentTaskHandle();

    /* Suspend the task */
    vTaskSuspend(عدم);
}

TEST_CASE("FreeRTOS Event Groups do not cause priority inversion when higher priority task is unblocked", "[freertos]")
{
    لاڳيتو_ترتيب[0] = عدم;
    لاڳيتو_ترتيب[1] = عدم;
    لاڳيتو_ترتيب_اشاريو = 0;

    /* Initialize the event group */
    eg = x_Event_گروهه_Create();

    /* Create a task with higher priority than the task that will set the event group bits */
    TaskHandle_t higher_prio_hdl;
    TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_منزل_Core(task_test_eg_prio, "task_test_eg_prio", 2048, (فراغ *)xTaskGetCurrentTaskHandle(), CONFIG_UNITY_FREERTOS_ترجيح + 1, &higher_prio_hdl, CONFIG_UNITY_FREERTOS_CPU));

    /* Wait for the task to be created */
    ulTaskNotifyTake(pdTRUE, پورٽ_MAX_DELAY);

    /* Set the event group bits */
    x_Event_گروهه_سيٽ_بٽ(eg, 1);

    /* Record the task handle in the run order array */
    لاڳيتو_ترتيب[لاڳيتو_ترتيب_اشاريو++] = xTaskGetCurrentTaskHandle();

    /* Verify that the higher priority task was unblocked and immediately scheduled and the lower priority task was preempted */
    TEST_ASSERT_EQUAL(higher_prio_hdl, لاڳيتو_ترتيب[0]);
    TEST_ASSERT_EQUAL(xTaskGetCurrentTaskHandle(), لاڳيتو_ترتيب[1]);

    /* Clean up */
    v_Event_گروهه_Delete(eg);
    vTaskDelete(higher_prio_hdl);
}

/*-----------------Test case for event group trace facilities-----------------*/
#جيڪڏهن_وصف_ٿيل  CONFIG_FREERTOS_USE_TRACE_FACILITY
#إذا SOC_GPTIMER_SUPPORTED
/*
 * Test event group Trace Facility functions such as
 * xEventGroupClearBitsFromISR(), xEventGroupSetBitsFromISR()
 */

//Use a timer to trigger an ISr
#وصف_ڪريو بٽ            0xAA
ساكن gptimer_handle_t gptimer;
ساكن bool test_سيٽ_بٽ;
ساكن bool test_clear_بٽ;

ساكن bool on_timer_alarm_cb(gptimer_handle_t timer, ثابت gptimer_alarm_event_معلومات_t *edata, فراغ *user_ctx)
{
    بنياد_Type_t task_woken = pdFALSE;

    gptimer_stop(timer);

    إذا (test_سيٽ_بٽ) {
        x_Event_گروهه_سيٽ_بٽ_From_ISR(eg, بٽ, &task_woken);
        test_سيٽ_بٽ = false;
    } وإلا إذا (test_clear_بٽ) {
        x_Event_گروهه_Clear_بٽ_From_ISR(eg, بٽ);
        xSemaphoreGiveFromISR(پورو_sem, &task_woken);
        test_clear_بٽ = false;
    }
    //Switch context if necessary
    أرجع task_woken == pdTRUE;
}

TEST_CASE("FreeRTOS Event Group ISR", "[freertos]")
{
    پورو_sem = x_Semaphore_Create_ٻٽو();
    eg = x_Event_گروهه_Create();
    test_سيٽ_بٽ = false;
    test_clear_بٽ = false;
    //Setup timer for ISR
    gptimer_config_t config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_ڳڻپ_مٿي,
        .resolution_hz = 1000000,
    };
    TEST_ESP_OK(gptimer_new_timer(&config, &gptimer));
    gptimer_alarm_config_t alarm_config = {
        .reload_ڳڻپ = 0,
        .alarm_ڳڻپ = 200000,
    };
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_timer_alarm_cb,
    };
    TEST_ESP_OK(gptimer_رجسٽر_event_callbacks(gptimer, &cbs, عدم));
    TEST_ESP_OK(gptimer_enable(gptimer));
    TEST_ESP_OK(gptimer_سيٽ_alarm_action(gptimer, &alarm_config));

    //Test set bits
    لکو("test set bits\r\n");
    test_سيٽ_بٽ = true;
    TEST_ESP_OK(gptimer_شروعات(gptimer));
    TEST_ASSERT_EQUAL(بٽ, x_Event_گروهه_Wait_بٽ(eg, بٽ, pdFALSE, pdTRUE, پورٽ_MAX_DELAY));     //Let ISR set event group bits

    //Test clear bits
    لکو("test clear bits\r\n");
    x_Event_گروهه_سيٽ_بٽ(eg, بٽ);                   //Set bits to be cleared
    test_clear_بٽ = true;
    TEST_ESP_OK(gptimer_سيٽ_raw_ڳڻپ(gptimer, 0));
    TEST_ESP_OK(gptimer_شروعات(gptimer));
    xSemaphoreTake(پورو_sem, پورٽ_MAX_DELAY);        //Wait for ISR to clear bits
    vTaskDelay(10);                                 //Event group clear bits runs via daemon task, delay so daemon can run
    TEST_ASSERT_EQUAL(0, x_Event_گروهه_Get_بٽ(eg));   //Check bits are cleared

    //Clean up
    TEST_ESP_OK(gptimer_disable(gptimer));
    TEST_ESP_OK(gptimer_del_timer(gptimer));
    v_Event_گروهه_Delete(eg);
    vSemaphoreDelete(پورو_sem);
    vTaskDelay(10);     //Give time for idle task to clear up deleted tasks
}
#جيڪڏهن_پورو //SOC_GPTIMER_SUPPORTED
#جيڪڏهن_پورو      //CONFIG_FREERTOS_USE_TRACE_FACILITY
