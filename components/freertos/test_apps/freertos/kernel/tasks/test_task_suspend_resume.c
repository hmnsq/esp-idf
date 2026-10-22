/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* Tests for FreeRTOS task suspend & resume */
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <inttypes.h>
#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "freertos/semphr.h"
#అన్తర్భావ "freertos/timers.h"
#అన్తర్భావ "freertos/queue.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "test_utils.h"
#అన్తర్భావ "driver/gptimer.h"
#అన్తర్భావ "esp_rom_sys.h"

/* Counter task counts a target variable forever */
స్థిర శూన్య task_లెక్క(శూన్య *vp_counter)
{
    అస్థిర గుర్తులేని *counter = (అస్థిర గుర్తులేని *)vp_counter;
    *counter = 0;

    అయితే (1) {
        (*counter)++;
        vTaskDelay(1);
    }
}

స్థిర శూన్య test_suspend_resume(పూర్ణాంకం లక్ష్యం_core)
{
    అస్థిర గుర్తులేని counter = 0;
    TaskHandle_t counter_task;

    x_Task_Create_Pinned_గమ్యం_Core(task_లెక్క, "Count", 2048,
                            (శూన్య *)&counter, UNITY_FREERTOS_ప్రాధాన్యత + 1,
                            &counter_task, లక్ష్యం_core);

    vTaskDelay(10);
    /* check some counting has happened */
    TEST_ASSERT_NOT_EQUAL(0, counter);

    // Do the next part a few times, just to be sure multiple suspends & resumes
    // work as expected...
    స్థిరరాశి పూర్ణాంకం TEST_ITERATIONS = 5;
    కొరకు (పూర్ణాంకం i = 0; i < TEST_ITERATIONS; i++) {
        vTaskSuspend(counter_task);
        గుర్తులేని suspend_లెక్క = counter;
        రాయి("Suspending @ %d\n", suspend_లెక్క);

        vTaskDelay(2);

        రాయి("Still suspended @ %d\n", counter);

        /* check the counter hasn't gone up while the task is suspended */
        TEST_ASSERT_EQUAL(suspend_లెక్క, counter);
        vTaskResume(counter_task);
        vTaskDelay(2);

        రాయి("Resumed @ %d\n", counter);
        /* check the counter is going up again now the task is resumed */
        TEST_ASSERT_NOT_EQUAL(suspend_లెక్క, counter);
    }

    vTaskDelete(counter_task);
}

TEST_CASE("Suspend/resume task on same core", "[freertos]")
{
    test_suspend_resume(UNITY_FREERTOS_CPU);
}

#నిర్వచితం_కాకపోతే CONFIG_FREERTOS_UNICORE
/*
Note: This test is ignore for now due to a known issue (xKernelLock contention + task state change will lead to a race
condition). More specifically:

- test_suspend_resume() suspends task_count() thus moving it to the suspended list
- But task_count() is already contesting the xKernelLock.
- Changes its own task list once it takes the xKernelLock
- xKernelLock never receives the crosscore interrupt as it was contesting for xKernelLock
Addressed in IDF-5844
*/
TEST_CASE("Suspend/resume task on other core", "[freertos][ignore]")
{
    test_suspend_resume(!UNITY_FREERTOS_CPU);
}
#అయితే_ముగింపు

/* Task suspends itself, then sets a flag and deletes itself */
స్థిర శూన్య task_suspend_self(శూన్య *vp_resumed)
{
    అస్థిర bool *resumed = (అస్థిర bool *)vp_resumed;
    *resumed = false;
    vTaskSuspend(ఖాళీ);
    *resumed = true;
    vTaskDelete(ఖాళీ);
}

TEST_CASE("Suspend the current running task", "[freertos]")
{
    అస్థిర bool resumed = false;
    TaskHandle_t suspend_task;

    x_Task_Create_Pinned_గమ్యం_Core(task_suspend_self, "suspend_self", 2048,
                            (శూన్య *)&resumed, UNITY_FREERTOS_ప్రాధాన్యత + 1,
                            &suspend_task, UNITY_FREERTOS_CPU);

    vTaskDelay(1);
    TEST_ASSERT_FALSE(resumed);

    vTaskResume(suspend_task);
    // Shouldn't need any delay here, as task should resume on this CPU immediately
    TEST_ASSERT_TRUE(resumed);
}

#ఒకవేళ SOC_GPTIMER_SUPPORTED

స్థిర అస్థిర bool timer_isr_fired;
స్థిర gptimer_handle_t gptimer = ఖాళీ;

/* Timer ISR clears interrupt, sets flag, then resumes the task supplied in the
 * callback argument.
 */
bool on_timer_alarm_cb(gptimer_handle_t timer, స్థిరరాశి gptimer_alarm_event_దత్తాంశం_t *edata, శూన్య *user_ctx)
{
    TaskHandle_t handle = user_ctx;
    పునాది_Type_t higher_ప్రాధాన్యత_Task_Woken = pdFALSE;
    gptimer_stop(timer);
    timer_isr_fired = true;
    higher_ప్రాధాన్యత_Task_Woken = xTaskResumeFromISR(handle);
    తిరిగి higher_ప్రాధాన్యత_Task_Woken == pdTRUE;
}

/* Task suspends itself, then sets parameter value to the current timer group counter and deletes itself */
స్థిర IRAM_ATTR శూన్య task_suspend_self_with_timer(శూన్య *vp_resumed)
{
    అస్థిర uint64_t *resumed_counter = vp_resumed;
    *resumed_counter = 0;
    vTaskSuspend(ఖాళీ);
    gptimer_get_raw_లెక్క(gptimer, (uint64_t *)resumed_counter);
    vTaskDelete(ఖాళీ);
}

/* Create a task which suspends itself, then resume it from a timer
 * interrupt. */
స్థిర శూన్య test_resume_task_from_isr(పూర్ణాంకం లక్ష్యం_core)
{
    అస్థిర uint64_t resumed_counter = 99;
    TaskHandle_t suspend_task;

    x_Task_Create_Pinned_గమ్యం_Core(task_suspend_self_with_timer, "suspend_self", 2048,
                            (శూన్య *)&resumed_counter, UNITY_FREERTOS_ప్రాధాన్యత + 1,
                            &suspend_task, లక్ష్యం_core);
    // delay to make the task has resumed itself
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(0, resumed_counter);

    /* Configure timer ISR */
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_లెక్క_పైకి,
        .resolution_hz = 1000000, // 1MHz, 1 tick = 1us
    };
    TEST_ESP_OK(gptimer_new_timer(&timer_config, &gptimer));
    timer_isr_fired = false;
    vTaskDelay(1); // Make sure we're at the start of a new tick
    gptimer_alarm_config_t alarm_config = {
        .alarm_లెక్క = 1000000 / config_TICK_రేటు_HZ / 2,
        .reload_లెక్క = 0,
    };
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_timer_alarm_cb,
    };
    TEST_ESP_OK(gptimer_రిజిస్టరు_event_callbacks(gptimer, &cbs, suspend_task));
    TEST_ESP_OK(gptimer_సమితి_alarm_action(gptimer, &alarm_config));
    TEST_ESP_OK(gptimer_enable(gptimer));
    TEST_ESP_OK(gptimer_ప్రారంభం(gptimer));
    // wait the timer interrupt fires up
    vTaskDelay(2);
    TEST_ASSERT_TRUE(timer_isr_fired);
    // check the task was resumed
    TEST_ASSERT_NOT_EQUAL(0, resumed_counter);
    // The task should have woken within 500us of the timer interrupt event (note: task may be a flash cache miss)
    రాయి("alarm value %llu task resumed at %u\n", alarm_config.alarm_లెక్క, (గుర్తులేని)resumed_counter);
    TEST_ASSERT_UINT32_WITHIN(100, alarm_config.alarm_లెక్క, (గుర్తులేని)resumed_counter);

    // clean up
    TEST_ESP_OK(gptimer_disable(gptimer));
    TEST_ESP_OK(gptimer_del_timer(gptimer));
}

TEST_CASE("Resume task from ISR (same core)", "[freertos]")
{
    test_resume_task_from_isr(UNITY_FREERTOS_CPU);
}

#నిర్వచితం_కాకపోతే CONFIG_FREERTOS_UNICORE
TEST_CASE("Resume task from ISR (other core)", "[freertos]")
{
    test_resume_task_from_isr(!UNITY_FREERTOS_CPU);
}
#అయితే_ముగింపు // CONFIG_FREERTOS_UNICORE

#అయితే_ముగింపు //SOC_GPTIMER_SUPPORTED
