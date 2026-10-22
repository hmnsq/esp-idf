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

#تعریف_کن رقم_TASKS               8
#تعریف_کن ALL_TASK_بیتها           ((1 << رقم_TASKS) - 1)
#تعریف_کن بیت_CALL(task)          (1 << (task))
#تعریف_کن بیت_RESPONSE(task)      (1 << ((task) + رقم_TASKS))
#تعریف_کن ALL_CALL_بیتها           (ALL_TASK_بیتها)
#تعریف_کن ALL_RESPONSE_بیتها       (ALL_TASK_بیتها << رقم_TASKS)

ساكن ثابت صحيح شمار_2 = 1000;
ساكن Event_گروه_Handle_t eg;
ساكن SemaphoreHandle_t انجامشده_sem;

ساكن فراغ task_event_گروه_call_response(فراغ *param)
{
    صحيح task_رقم = (صحيح)param;

    چاپ_کن("Started %d\n", task_رقم);

    لكل (صحيح i = 0; i < شمار_2; i++) {
        /* Wait until the common "call" bit is set, starts off all tasks
           (clear on return) */
        TEST_ASSERT(x_Event_گروه_Wait_بیتها(eg, بیت_CALL(task_رقم), true, false, پورت_MAX_DELAY));

        /* Set our individual "response" bit */
        x_Event_گروه_مجموعه_بیتها(eg, بیت_RESPONSE(task_رقم));
    }

    چاپ_کن("Task %d done\n", task_رقم);
    xSemaphoreGive(انجامشده_sem);
    // Wait to be deleted
    vTaskSuspend(عدم);
}

TEST_CASE("FreeRTOS Event Groups", "[freertos]")
{
    eg = x_Event_گروه_Create();
    انجامشده_sem = xSemaphoreCreateCounting(رقم_TASKS, 0);

    TaskHandle_t task_handles[رقم_TASKS];

    /* Note: task_event_group_call_response all have higher priority than this task, so on this core
       they will always preempt this task.

       This is important because we need to know all tasks have blocked on their own BIT_CALL(task_num) each time we
       signal it, or they get out of sync.
     */
    لكل (صحيح c = 0; c < رقم_TASKS; c++) {
        x_Task_Create_Pinned_مقصد_Core(task_event_گروه_call_response, "tsk_call_resp", 4096, (فراغ *)c, configMAX_PRIORITIES - 1, &task_handles[c], c % CONFIG_FREERTOS_عدد_از_CORES);
    }

    /* Tasks all start instantly, but this task will resume running at the same time as the higher priority tasks on the
       other processor may still be setting up, so allow time for them to also block on BIT_CALL()... */
    vTaskDelay(10);

    لكل (صحيح i = 0; i < شمار_2; i++) {
        /* signal all the "CALL" bits of each task */
        x_Event_گروه_مجموعه_بیتها(eg, ALL_CALL_بیتها);

        /* Wait until all tasks have set their respective response bits */
        TEST_ASSERT_EQUAL_HEX16(ALL_RESPONSE_بیتها, x_Event_گروه_Wait_بیتها(eg, ALL_RESPONSE_بیتها, true, true, پورت_MAX_DELAY));
    }

    /* Ensure all tasks have suspend themselves */
    لكل (صحيح c = 0; c < رقم_TASKS; c++) {
        TEST_ASSERT(xSemaphoreTake(انجامشده_sem, 100 / پورت_TICK_PERIOD_MS));
    }

    لكل (صحيح c = 0; c < رقم_TASKS; c++) {
        unity_utils_task_delete(task_handles[c]);
    }

    vSemaphoreDelete(انجامشده_sem);
    v_Event_گروه_Delete(eg);
}

ساكن فراغ task_test_sync(فراغ *param)
{
    صحيح task_رقم = (صحيح)param;

    چاپ_کن("Started %d\n", task_رقم);

    لكل (صحيح i = 0; i < شمار_2; i++) {
        /* set our bit, and wait on all tasks to set their bits */
        x_Event_گروه_Sync(eg, بیت_CALL(task_رقم), ALL_CALL_بیتها, پورت_MAX_DELAY);
        /* clear our bit */
        x_Event_گروه_Clear_بیتها(eg, بیت_CALL(task_رقم));
    }
    صحيح after_انجامشده = x_Event_گروه_مجموعه_بیتها(eg, بیت_RESPONSE(task_رقم));

    چاپ_کن("Done %d = 0x%08x\n", task_رقم, after_انجامشده);

    xSemaphoreGive(انجامشده_sem);
    vTaskDelete(عدم);
}

TEST_CASE("FreeRTOS Event Group Sync", "[freertos]")
{
    eg = x_Event_گروه_Create();
    انجامشده_sem = xSemaphoreCreateCounting(رقم_TASKS, 0);

    لكل (صحيح c = 0; c < رقم_TASKS; c++) {
        x_Task_Create_Pinned_مقصد_Core(task_test_sync, "task_test_sync", 4096, (فراغ *)c, configMAX_PRIORITIES - 1, عدم, c % CONFIG_FREERTOS_عدد_از_CORES);
    }

    لكل (صحيح c = 0; c < رقم_TASKS; c++) {
        چاپ_کن("Waiting on %d (0x%08x)\n", c, بیت_RESPONSE(c));
        TEST_ASSERT(x_Event_گروه_Wait_بیتها(eg, بیت_RESPONSE(c), false, false, پورت_MAX_DELAY));
    }

    /* Ensure all tasks cleaned up correctly */
    لكل (صحيح c = 0; c < رقم_TASKS; c++) {
        TEST_ASSERT(xSemaphoreTake(انجامشده_sem, 100 / پورت_TICK_PERIOD_MS));
    }

    vSemaphoreDelete(انجامشده_sem);
    v_Event_گروه_Delete(eg);
}

ساكن TaskHandle_t پیاپی_ترتیب[2];
ساكن uint32_t پیاپی_ترتیب_شاخص = 0;

فراغ task_test_eg_prio(فراغ *arg)
{
    TaskHandle_t main_task_hdl = (TaskHandle_t)arg;

    /* Notify the main task that this task has been created */
    xTaskNotifyGive(main_task_hdl);

    /* Wait for the event group bits to be set */
    TEST_ASSERT_EQUAL(1, x_Event_گروه_Wait_بیتها(eg, 1, pdTRUE, pdTRUE, پورت_MAX_DELAY));

    /* Record the task handle in the run order array */
    پیاپی_ترتیب[پیاپی_ترتیب_شاخص++] = xTaskGetCurrentTaskHandle();

    /* Suspend the task */
    vTaskSuspend(عدم);
}

TEST_CASE("FreeRTOS Event Groups do not cause priority inversion when higher priority task is unblocked", "[freertos]")
{
    پیاپی_ترتیب[0] = عدم;
    پیاپی_ترتیب[1] = عدم;
    پیاپی_ترتیب_شاخص = 0;

    /* Initialize the event group */
    eg = x_Event_گروه_Create();

    /* Create a task with higher priority than the task that will set the event group bits */
    TaskHandle_t higher_prio_hdl;
    TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_مقصد_Core(task_test_eg_prio, "task_test_eg_prio", 2048, (فراغ *)xTaskGetCurrentTaskHandle(), CONFIG_UNITY_FREERTOS_اولویت + 1, &higher_prio_hdl, CONFIG_UNITY_FREERTOS_CPU));

    /* Wait for the task to be created */
    ulTaskNotifyTake(pdTRUE, پورت_MAX_DELAY);

    /* Set the event group bits */
    x_Event_گروه_مجموعه_بیتها(eg, 1);

    /* Record the task handle in the run order array */
    پیاپی_ترتیب[پیاپی_ترتیب_شاخص++] = xTaskGetCurrentTaskHandle();

    /* Verify that the higher priority task was unblocked and immediately scheduled and the lower priority task was preempted */
    TEST_ASSERT_EQUAL(higher_prio_hdl, پیاپی_ترتیب[0]);
    TEST_ASSERT_EQUAL(xTaskGetCurrentTaskHandle(), پیاپی_ترتیب[1]);

    /* Clean up */
    v_Event_گروه_Delete(eg);
    vTaskDelete(higher_prio_hdl);
}

/*-----------------Test case for event group trace facilities-----------------*/
#اگر_تعریف_شده  CONFIG_FREERTOS_USE_TRACE_FACILITY
#إذا SOC_GPTIMER_SUPPORTED
/*
 * Test event group Trace Facility functions such as
 * xEventGroupClearBitsFromISR(), xEventGroupSetBitsFromISR()
 */

//Use a timer to trigger an ISr
#تعریف_کن بیتها            0xAA
ساكن gptimer_handle_t gptimer;
ساكن bool test_مجموعه_بیتها;
ساكن bool test_clear_بیتها;

ساكن bool on_timer_alarm_cb(gptimer_handle_t timer, ثابت gptimer_alarm_event_داده_t *edata, فراغ *user_ctx)
{
    پایه_Type_t task_woken = pdFALSE;

    gptimer_stop(timer);

    إذا (test_مجموعه_بیتها) {
        x_Event_گروه_مجموعه_بیتها_From_ISR(eg, بیتها, &task_woken);
        test_مجموعه_بیتها = false;
    } وإلا إذا (test_clear_بیتها) {
        x_Event_گروه_Clear_بیتها_From_ISR(eg, بیتها);
        xSemaphoreGiveFromISR(انجامشده_sem, &task_woken);
        test_clear_بیتها = false;
    }
    //Switch context if necessary
    أرجع task_woken == pdTRUE;
}

TEST_CASE("FreeRTOS Event Group ISR", "[freertos]")
{
    انجامشده_sem = x_Semaphore_Create_دودویی();
    eg = x_Event_گروه_Create();
    test_مجموعه_بیتها = false;
    test_clear_بیتها = false;
    //Setup timer for ISR
    gptimer_config_t config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_شمار_به_بالا,
        .resolution_hz = 1000000,
    };
    TEST_ESP_OK(gptimer_new_timer(&config, &gptimer));
    gptimer_alarm_config_t alarm_config = {
        .reload_شمار = 0,
        .alarm_شمار = 200000,
    };
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_timer_alarm_cb,
    };
    TEST_ESP_OK(gptimer_ثبات_event_callbacks(gptimer, &cbs, عدم));
    TEST_ESP_OK(gptimer_enable(gptimer));
    TEST_ESP_OK(gptimer_مجموعه_alarm_action(gptimer, &alarm_config));

    //Test set bits
    چاپ_کن("test set bits\r\n");
    test_مجموعه_بیتها = true;
    TEST_ESP_OK(gptimer_شروع(gptimer));
    TEST_ASSERT_EQUAL(بیتها, x_Event_گروه_Wait_بیتها(eg, بیتها, pdFALSE, pdTRUE, پورت_MAX_DELAY));     //Let ISR set event group bits

    //Test clear bits
    چاپ_کن("test clear bits\r\n");
    x_Event_گروه_مجموعه_بیتها(eg, بیتها);                   //Set bits to be cleared
    test_clear_بیتها = true;
    TEST_ESP_OK(gptimer_مجموعه_raw_شمار(gptimer, 0));
    TEST_ESP_OK(gptimer_شروع(gptimer));
    xSemaphoreTake(انجامشده_sem, پورت_MAX_DELAY);        //Wait for ISR to clear bits
    vTaskDelay(10);                                 //Event group clear bits runs via daemon task, delay so daemon can run
    TEST_ASSERT_EQUAL(0, x_Event_گروه_Get_بیتها(eg));   //Check bits are cleared

    //Clean up
    TEST_ESP_OK(gptimer_disable(gptimer));
    TEST_ESP_OK(gptimer_del_timer(gptimer));
    v_Event_گروه_Delete(eg);
    vSemaphoreDelete(انجامشده_sem);
    vTaskDelay(10);     //Give time for idle task to clear up deleted tasks
}
#پایان_اگر //SOC_GPTIMER_SUPPORTED
#پایان_اگر      //CONFIG_FREERTOS_USE_TRACE_FACILITY
