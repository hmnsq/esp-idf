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

#تعریف_کړه عدد_TASKS               8
#تعریف_کړه ALL_TASK_بټونه           ((1 << عدد_TASKS) - 1)
#تعریف_کړه بټ_CALL(task)          (1 << (task))
#تعریف_کړه بټ_RESPONSE(task)      (1 << ((task) + عدد_TASKS))
#تعریف_کړه ALL_CALL_بټونه           (ALL_TASK_بټونه)
#تعریف_کړه ALL_RESPONSE_بټونه       (ALL_TASK_بټونه << عدد_TASKS)

ساكن ثابت صحيح شمېر_2 = 1000;
ساكن Event_ډله_Handle_t eg;
ساكن SemaphoreHandle_t بشپړ_sem;

ساكن فراغ task_event_ډله_call_response(فراغ *param)
{
    صحيح task_عدد = (صحيح)param;

    چاپ_کړه("Started %d\n", task_عدد);

    لكل (صحيح i = 0; i < شمېر_2; i++) {
        /* Wait until the common "call" bit is set, starts off all tasks
           (clear on return) */
        TEST_ASSERT(x_Event_ډله_Wait_بټونه(eg, بټ_CALL(task_عدد), true, false, پورټ_MAX_DELAY));

        /* Set our individual "response" bit */
        x_Event_ډله_سېټ_بټونه(eg, بټ_RESPONSE(task_عدد));
    }

    چاپ_کړه("Task %d done\n", task_عدد);
    xSemaphoreGive(بشپړ_sem);
    // Wait to be deleted
    vTaskSuspend(عدم);
}

TEST_CASE("FreeRTOS Event Groups", "[freertos]")
{
    eg = x_Event_ډله_Create();
    بشپړ_sem = xSemaphoreCreateCounting(عدد_TASKS, 0);

    TaskHandle_t task_handles[عدد_TASKS];

    /* Note: task_event_group_call_response all have higher priority than this task, so on this core
       they will always preempt this task.

       This is important because we need to know all tasks have blocked on their own BIT_CALL(task_num) each time we
       signal it, or they get out of sync.
     */
    لكل (صحيح c = 0; c < عدد_TASKS; c++) {
        x_Task_Create_Pinned_منزل_Core(task_event_ډله_call_response, "tsk_call_resp", 4096, (فراغ *)c, configMAX_PRIORITIES - 1, &task_handles[c], c % CONFIG_FREERTOS_شمېره_د_CORES);
    }

    /* Tasks all start instantly, but this task will resume running at the same time as the higher priority tasks on the
       other processor may still be setting up, so allow time for them to also block on BIT_CALL()... */
    vTaskDelay(10);

    لكل (صحيح i = 0; i < شمېر_2; i++) {
        /* signal all the "CALL" bits of each task */
        x_Event_ډله_سېټ_بټونه(eg, ALL_CALL_بټونه);

        /* Wait until all tasks have set their respective response bits */
        TEST_ASSERT_EQUAL_HEX16(ALL_RESPONSE_بټونه, x_Event_ډله_Wait_بټونه(eg, ALL_RESPONSE_بټونه, true, true, پورټ_MAX_DELAY));
    }

    /* Ensure all tasks have suspend themselves */
    لكل (صحيح c = 0; c < عدد_TASKS; c++) {
        TEST_ASSERT(xSemaphoreTake(بشپړ_sem, 100 / پورټ_TICK_PERIOD_MS));
    }

    لكل (صحيح c = 0; c < عدد_TASKS; c++) {
        unity_utils_task_delete(task_handles[c]);
    }

    vSemaphoreDelete(بشپړ_sem);
    v_Event_ډله_Delete(eg);
}

ساكن فراغ task_test_sync(فراغ *param)
{
    صحيح task_عدد = (صحيح)param;

    چاپ_کړه("Started %d\n", task_عدد);

    لكل (صحيح i = 0; i < شمېر_2; i++) {
        /* set our bit, and wait on all tasks to set their bits */
        x_Event_ډله_Sync(eg, بټ_CALL(task_عدد), ALL_CALL_بټونه, پورټ_MAX_DELAY);
        /* clear our bit */
        x_Event_ډله_Clear_بټونه(eg, بټ_CALL(task_عدد));
    }
    صحيح after_بشپړ = x_Event_ډله_سېټ_بټونه(eg, بټ_RESPONSE(task_عدد));

    چاپ_کړه("Done %d = 0x%08x\n", task_عدد, after_بشپړ);

    xSemaphoreGive(بشپړ_sem);
    vTaskDelete(عدم);
}

TEST_CASE("FreeRTOS Event Group Sync", "[freertos]")
{
    eg = x_Event_ډله_Create();
    بشپړ_sem = xSemaphoreCreateCounting(عدد_TASKS, 0);

    لكل (صحيح c = 0; c < عدد_TASKS; c++) {
        x_Task_Create_Pinned_منزل_Core(task_test_sync, "task_test_sync", 4096, (فراغ *)c, configMAX_PRIORITIES - 1, عدم, c % CONFIG_FREERTOS_شمېره_د_CORES);
    }

    لكل (صحيح c = 0; c < عدد_TASKS; c++) {
        چاپ_کړه("Waiting on %d (0x%08x)\n", c, بټ_RESPONSE(c));
        TEST_ASSERT(x_Event_ډله_Wait_بټونه(eg, بټ_RESPONSE(c), false, false, پورټ_MAX_DELAY));
    }

    /* Ensure all tasks cleaned up correctly */
    لكل (صحيح c = 0; c < عدد_TASKS; c++) {
        TEST_ASSERT(xSemaphoreTake(بشپړ_sem, 100 / پورټ_TICK_PERIOD_MS));
    }

    vSemaphoreDelete(بشپړ_sem);
    v_Event_ډله_Delete(eg);
}

ساكن TaskHandle_t پرلپسې_ترتیب[2];
ساكن uint32_t پرلپسې_ترتیب_شاخص = 0;

فراغ task_test_eg_prio(فراغ *arg)
{
    TaskHandle_t main_task_hdl = (TaskHandle_t)arg;

    /* Notify the main task that this task has been created */
    xTaskNotifyGive(main_task_hdl);

    /* Wait for the event group bits to be set */
    TEST_ASSERT_EQUAL(1, x_Event_ډله_Wait_بټونه(eg, 1, pdTRUE, pdTRUE, پورټ_MAX_DELAY));

    /* Record the task handle in the run order array */
    پرلپسې_ترتیب[پرلپسې_ترتیب_شاخص++] = xTaskGetCurrentTaskHandle();

    /* Suspend the task */
    vTaskSuspend(عدم);
}

TEST_CASE("FreeRTOS Event Groups do not cause priority inversion when higher priority task is unblocked", "[freertos]")
{
    پرلپسې_ترتیب[0] = عدم;
    پرلپسې_ترتیب[1] = عدم;
    پرلپسې_ترتیب_شاخص = 0;

    /* Initialize the event group */
    eg = x_Event_ډله_Create();

    /* Create a task with higher priority than the task that will set the event group bits */
    TaskHandle_t higher_prio_hdl;
    TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_منزل_Core(task_test_eg_prio, "task_test_eg_prio", 2048, (فراغ *)xTaskGetCurrentTaskHandle(), CONFIG_UNITY_FREERTOS_لومړیتوب + 1, &higher_prio_hdl, CONFIG_UNITY_FREERTOS_CPU));

    /* Wait for the task to be created */
    ulTaskNotifyTake(pdTRUE, پورټ_MAX_DELAY);

    /* Set the event group bits */
    x_Event_ډله_سېټ_بټونه(eg, 1);

    /* Record the task handle in the run order array */
    پرلپسې_ترتیب[پرلپسې_ترتیب_شاخص++] = xTaskGetCurrentTaskHandle();

    /* Verify that the higher priority task was unblocked and immediately scheduled and the lower priority task was preempted */
    TEST_ASSERT_EQUAL(higher_prio_hdl, پرلپسې_ترتیب[0]);
    TEST_ASSERT_EQUAL(xTaskGetCurrentTaskHandle(), پرلپسې_ترتیب[1]);

    /* Clean up */
    v_Event_ډله_Delete(eg);
    vTaskDelete(higher_prio_hdl);
}

/*-----------------Test case for event group trace facilities-----------------*/
#که_تعریف_شوی  CONFIG_FREERTOS_USE_TRACE_FACILITY
#إذا SOC_GPTIMER_SUPPORTED
/*
 * Test event group Trace Facility functions such as
 * xEventGroupClearBitsFromISR(), xEventGroupSetBitsFromISR()
 */

//Use a timer to trigger an ISr
#تعریف_کړه بټونه            0xAA
ساكن gptimer_handle_t gptimer;
ساكن bool test_سېټ_بټونه;
ساكن bool test_clear_بټونه;

ساكن bool on_timer_alarm_cb(gptimer_handle_t timer, ثابت gptimer_alarm_event_معلومات_t *edata, فراغ *user_ctx)
{
    بنسټ_Type_t task_woken = pdFALSE;

    gptimer_stop(timer);

    إذا (test_سېټ_بټونه) {
        x_Event_ډله_سېټ_بټونه_From_ISR(eg, بټونه, &task_woken);
        test_سېټ_بټونه = false;
    } وإلا إذا (test_clear_بټونه) {
        x_Event_ډله_Clear_بټونه_From_ISR(eg, بټونه);
        xSemaphoreGiveFromISR(بشپړ_sem, &task_woken);
        test_clear_بټونه = false;
    }
    //Switch context if necessary
    أرجع task_woken == pdTRUE;
}

TEST_CASE("FreeRTOS Event Group ISR", "[freertos]")
{
    بشپړ_sem = x_Semaphore_Create_دوګونی();
    eg = x_Event_ډله_Create();
    test_سېټ_بټونه = false;
    test_clear_بټونه = false;
    //Setup timer for ISR
    gptimer_config_t config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_شمېر_پورته,
        .resolution_hz = 1000000,
    };
    TEST_ESP_OK(gptimer_new_timer(&config, &gptimer));
    gptimer_alarm_config_t alarm_config = {
        .reload_شمېر = 0,
        .alarm_شمېر = 200000,
    };
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_timer_alarm_cb,
    };
    TEST_ESP_OK(gptimer_ثبتوونکی_event_callbacks(gptimer, &cbs, عدم));
    TEST_ESP_OK(gptimer_enable(gptimer));
    TEST_ESP_OK(gptimer_سېټ_alarm_action(gptimer, &alarm_config));

    //Test set bits
    چاپ_کړه("test set bits\r\n");
    test_سېټ_بټونه = true;
    TEST_ESP_OK(gptimer_پیل(gptimer));
    TEST_ASSERT_EQUAL(بټونه, x_Event_ډله_Wait_بټونه(eg, بټونه, pdFALSE, pdTRUE, پورټ_MAX_DELAY));     //Let ISR set event group bits

    //Test clear bits
    چاپ_کړه("test clear bits\r\n");
    x_Event_ډله_سېټ_بټونه(eg, بټونه);                   //Set bits to be cleared
    test_clear_بټونه = true;
    TEST_ESP_OK(gptimer_سېټ_raw_شمېر(gptimer, 0));
    TEST_ESP_OK(gptimer_پیل(gptimer));
    xSemaphoreTake(بشپړ_sem, پورټ_MAX_DELAY);        //Wait for ISR to clear bits
    vTaskDelay(10);                                 //Event group clear bits runs via daemon task, delay so daemon can run
    TEST_ASSERT_EQUAL(0, x_Event_ډله_Get_بټونه(eg));   //Check bits are cleared

    //Clean up
    TEST_ESP_OK(gptimer_disable(gptimer));
    TEST_ESP_OK(gptimer_del_timer(gptimer));
    v_Event_ډله_Delete(eg);
    vSemaphoreDelete(بشپړ_sem);
    vTaskDelay(10);     //Give time for idle task to clear up deleted tasks
}
#د_که_پای //SOC_GPTIMER_SUPPORTED
#د_که_پای      //CONFIG_FREERTOS_USE_TRACE_FACILITY
