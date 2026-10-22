/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* Tests for FreeRTOS task suspend & resume */
#شامل <stdio.h>
#شامل <string.h>
#شامل <inttypes.h>
#شامل "sdkconfig.h"
#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل "freertos/semphr.h"
#شامل "freertos/timers.h"
#شامل "freertos/queue.h"
#شامل "unity.h"
#شامل "test_utils.h"
#شامل "driver/gptimer.h"
#شامل "esp_rom_sys.h"

/* Counter task counts a target variable forever */
ساکن خالی task_گنتی(خالی *vp_شمار)
{
    متلون غیرعلامتی *شمار_2 = (متلون غیرعلامتی *)vp_شمار;
    *شمار_2 = 0;

    جب (1) {
        (*شمار_2)++;
        vTaskDelay(1);
    }
}

ساکن خالی test_suspend_resume(صحیح ہدف_core)
{
    متلون غیرعلامتی شمار_2 = 0;
    TaskHandle_t شمار_task;

    x_Task_Create_Pinned_مقصد_Core(task_گنتی, "Count", 2048,
                            (خالی *)&شمار_2, UNITY_FREERTOS_ترجیح + 1,
                            &شمار_task, ہدف_core);

    vTaskDelay(10);
    /* check some counting has happened */
    TEST_ASSERT_NOT_EQUAL(0, شمار_2);

    // Do the next part a few times, just to be sure multiple suspends & resumes
    // work as expected...
    ثابت صحیح TEST_ITERATIONS = 5;
    برائے (صحیح i = 0; i < TEST_ITERATIONS; i++) {
        vTaskSuspend(شمار_task);
        غیرعلامتی suspend_گنتی = شمار_2;
        لکھو("Suspending @ %d\n", suspend_گنتی);

        vTaskDelay(2);

        لکھو("Still suspended @ %d\n", شمار_2);

        /* check the counter hasn't gone up while the task is suspended */
        TEST_ASSERT_EQUAL(suspend_گنتی, شمار_2);
        vTaskResume(شمار_task);
        vTaskDelay(2);

        لکھو("Resumed @ %d\n", شمار_2);
        /* check the counter is going up again now the task is resumed */
        TEST_ASSERT_NOT_EQUAL(suspend_گنتی, شمار_2);
    }

    vTaskDelete(شمار_task);
}

TEST_CASE("Suspend/resume task on same core", "[freertos]")
{
    test_suspend_resume(UNITY_FREERTOS_CPU);
}

#اگر_غیر_تعریف_شدہ CONFIG_FREERTOS_UNICORE
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
#اگر_ختم

/* Task suspends itself, then sets a flag and deletes itself */
ساکن خالی task_suspend_self(خالی *vp_resumed)
{
    متلون bool *resumed = (متلون bool *)vp_resumed;
    *resumed = false;
    vTaskSuspend(تہی);
    *resumed = true;
    vTaskDelete(تہی);
}

TEST_CASE("Suspend the current running task", "[freertos]")
{
    متلون bool resumed = false;
    TaskHandle_t suspend_task;

    x_Task_Create_Pinned_مقصد_Core(task_suspend_self, "suspend_self", 2048,
                            (خالی *)&resumed, UNITY_FREERTOS_ترجیح + 1,
                            &suspend_task, UNITY_FREERTOS_CPU);

    vTaskDelay(1);
    TEST_ASSERT_FALSE(resumed);

    vTaskResume(suspend_task);
    // Shouldn't need any delay here, as task should resume on this CPU immediately
    TEST_ASSERT_TRUE(resumed);
}

#اگر SOC_GPTIMER_SUPPORTED

ساکن متلون bool timer_isr_fired;
ساکن gptimer_handle_t gptimer = تہی;

/* Timer ISR clears interrupt, sets flag, then resumes the task supplied in the
 * callback argument.
 */
bool on_timer_alarm_cb(gptimer_handle_t timer, ثابت gptimer_alarm_event_ڈیٹا_t *edata, خالی *user_ctx)
{
    TaskHandle_t handle = user_ctx;
    بنیاد_Type_t higher_ترجیح_Task_Woken = pdFALSE;
    gptimer_stop(timer);
    timer_isr_fired = true;
    higher_ترجیح_Task_Woken = xTaskResumeFromISR(handle);
    واپس higher_ترجیح_Task_Woken == pdTRUE;
}

/* Task suspends itself, then sets parameter value to the current timer group counter and deletes itself */
ساکن IRAM_ATTR خالی task_suspend_self_with_timer(خالی *vp_resumed)
{
    متلون uint64_t *resumed_شمار = vp_resumed;
    *resumed_شمار = 0;
    vTaskSuspend(تہی);
    gptimer_get_raw_گنتی(gptimer, (uint64_t *)resumed_شمار);
    vTaskDelete(تہی);
}

/* Create a task which suspends itself, then resume it from a timer
 * interrupt. */
ساکن خالی test_resume_task_from_isr(صحیح ہدف_core)
{
    متلون uint64_t resumed_شمار = 99;
    TaskHandle_t suspend_task;

    x_Task_Create_Pinned_مقصد_Core(task_suspend_self_with_timer, "suspend_self", 2048,
                            (خالی *)&resumed_شمار, UNITY_FREERTOS_ترجیح + 1,
                            &suspend_task, ہدف_core);
    // delay to make the task has resumed itself
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(0, resumed_شمار);

    /* Configure timer ISR */
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_گنتی_اوپر,
        .resolution_hz = 1000000, // 1MHz, 1 tick = 1us
    };
    TEST_ESP_OK(gptimer_new_timer(&timer_config, &gptimer));
    timer_isr_fired = false;
    vTaskDelay(1); // Make sure we're at the start of a new tick
    gptimer_alarm_config_t alarm_config = {
        .alarm_گنتی = 1000000 / config_TICK_شرح_HZ / 2,
        .reload_گنتی = 0,
    };
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_timer_alarm_cb,
    };
    TEST_ESP_OK(gptimer_رجسٹر_event_callbacks(gptimer, &cbs, suspend_task));
    TEST_ESP_OK(gptimer_سیٹ_alarm_action(gptimer, &alarm_config));
    TEST_ESP_OK(gptimer_enable(gptimer));
    TEST_ESP_OK(gptimer_آغاز(gptimer));
    // wait the timer interrupt fires up
    vTaskDelay(2);
    TEST_ASSERT_TRUE(timer_isr_fired);
    // check the task was resumed
    TEST_ASSERT_NOT_EQUAL(0, resumed_شمار);
    // The task should have woken within 500us of the timer interrupt event (note: task may be a flash cache miss)
    لکھو("alarm value %llu task resumed at %u\n", alarm_config.alarm_گنتی, (غیرعلامتی)resumed_شمار);
    TEST_ASSERT_UINT32_WITHIN(100, alarm_config.alarm_گنتی, (غیرعلامتی)resumed_شمار);

    // clean up
    TEST_ESP_OK(gptimer_disable(gptimer));
    TEST_ESP_OK(gptimer_del_timer(gptimer));
}

TEST_CASE("Resume task from ISR (same core)", "[freertos]")
{
    test_resume_task_from_isr(UNITY_FREERTOS_CPU);
}

#اگر_غیر_تعریف_شدہ CONFIG_FREERTOS_UNICORE
TEST_CASE("Resume task from ISR (other core)", "[freertos]")
{
    test_resume_task_from_isr(!UNITY_FREERTOS_CPU);
}
#اگر_ختم // CONFIG_FREERTOS_UNICORE

#اگر_ختم //SOC_GPTIMER_SUPPORTED
