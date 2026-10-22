/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* Tests for FreeRTOS task suspend & resume */
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <inttypes.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/semphr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/timers.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/queue.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "test_utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "driver/gptimer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_rom_sys.h"

/* Counter task counts a target variable forever */
ਸਥਿਰ ਖਾਲੀ task_ਗਿਣਤੀ(ਖਾਲੀ *vp_counter)
{
    ਅਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ *counter = (ਅਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ *)vp_counter;
    *counter = 0;

    ਜਦੋਂ (1) {
        (*counter)++;
        vTaskDelay(1);
    }
}

ਸਥਿਰ ਖਾਲੀ test_suspend_resume(ਪੂਰਨ ਨਿਸ਼ਾਨਾ_core)
{
    ਅਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ counter = 0;
    TaskHandle_t counter_task;

    x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(task_ਗਿਣਤੀ, "Count", 2048,
                            (ਖਾਲੀ *)&counter, UNITY_FREERTOS_ਤਰਜੀਹ + 1,
                            &counter_task, ਨਿਸ਼ਾਨਾ_core);

    vTaskDelay(10);
    /* check some counting has happened */
    TEST_ASSERT_NOT_EQUAL(0, counter);

    // Do the next part a few times, just to be sure multiple suspends & resumes
    // work as expected...
    ਅਚਲ ਪੂਰਨ TEST_ITERATIONS = 5;
    ਲਈ (ਪੂਰਨ i = 0; i < TEST_ITERATIONS; i++) {
        vTaskSuspend(counter_task);
        ਬਿਨਾਨਿਸ਼ਾਨੀ suspend_ਗਿਣਤੀ = counter;
        ਲਿਖੋ("Suspending @ %d\n", suspend_ਗਿਣਤੀ);

        vTaskDelay(2);

        ਲਿਖੋ("Still suspended @ %d\n", counter);

        /* check the counter hasn't gone up while the task is suspended */
        TEST_ASSERT_EQUAL(suspend_ਗਿਣਤੀ, counter);
        vTaskResume(counter_task);
        vTaskDelay(2);

        ਲਿਖੋ("Resumed @ %d\n", counter);
        /* check the counter is going up again now the task is resumed */
        TEST_ASSERT_NOT_EQUAL(suspend_ਗਿਣਤੀ, counter);
    }

    vTaskDelete(counter_task);
}

TEST_CASE("Suspend/resume task on same core", "[freertos]")
{
    test_suspend_resume(UNITY_FREERTOS_CPU);
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CONFIG_FREERTOS_UNICORE
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
#ਜੇ_ਅੰਤ

/* Task suspends itself, then sets a flag and deletes itself */
ਸਥਿਰ ਖਾਲੀ task_suspend_self(ਖਾਲੀ *vp_resumed)
{
    ਅਸਥਿਰ bool *resumed = (ਅਸਥਿਰ bool *)vp_resumed;
    *resumed = false;
    vTaskSuspend(ਨਿਰਾ);
    *resumed = true;
    vTaskDelete(ਨਿਰਾ);
}

TEST_CASE("Suspend the current running task", "[freertos]")
{
    ਅਸਥਿਰ bool resumed = false;
    TaskHandle_t suspend_task;

    x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(task_suspend_self, "suspend_self", 2048,
                            (ਖਾਲੀ *)&resumed, UNITY_FREERTOS_ਤਰਜੀਹ + 1,
                            &suspend_task, UNITY_FREERTOS_CPU);

    vTaskDelay(1);
    TEST_ASSERT_FALSE(resumed);

    vTaskResume(suspend_task);
    // Shouldn't need any delay here, as task should resume on this CPU immediately
    TEST_ASSERT_TRUE(resumed);
}

#ਜੇ SOC_GPTIMER_SUPPORTED

ਸਥਿਰ ਅਸਥਿਰ bool timer_isr_fired;
ਸਥਿਰ gptimer_handle_t gptimer = ਨਿਰਾ;

/* Timer ISR clears interrupt, sets flag, then resumes the task supplied in the
 * callback argument.
 */
bool on_timer_alarm_cb(gptimer_handle_t timer, ਅਚਲ gptimer_alarm_event_ਅੰਕੜੇ_t *edata, ਖਾਲੀ *user_ctx)
{
    TaskHandle_t handle = user_ctx;
    ਅਧਾਰ_Type_t higher_ਤਰਜੀਹ_Task_Woken = pdFALSE;
    gptimer_stop(timer);
    timer_isr_fired = true;
    higher_ਤਰਜੀਹ_Task_Woken = xTaskResumeFromISR(handle);
    ਵਾਪਸ higher_ਤਰਜੀਹ_Task_Woken == pdTRUE;
}

/* Task suspends itself, then sets parameter value to the current timer group counter and deletes itself */
ਸਥਿਰ IRAM_ATTR ਖਾਲੀ task_suspend_self_with_timer(ਖਾਲੀ *vp_resumed)
{
    ਅਸਥਿਰ uint64_t *resumed_counter = vp_resumed;
    *resumed_counter = 0;
    vTaskSuspend(ਨਿਰਾ);
    gptimer_get_raw_ਗਿਣਤੀ(gptimer, (uint64_t *)resumed_counter);
    vTaskDelete(ਨਿਰਾ);
}

/* Create a task which suspends itself, then resume it from a timer
 * interrupt. */
ਸਥਿਰ ਖਾਲੀ test_resume_task_from_isr(ਪੂਰਨ ਨਿਸ਼ਾਨਾ_core)
{
    ਅਸਥਿਰ uint64_t resumed_counter = 99;
    TaskHandle_t suspend_task;

    x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(task_suspend_self_with_timer, "suspend_self", 2048,
                            (ਖਾਲੀ *)&resumed_counter, UNITY_FREERTOS_ਤਰਜੀਹ + 1,
                            &suspend_task, ਨਿਸ਼ਾਨਾ_core);
    // delay to make the task has resumed itself
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(0, resumed_counter);

    /* Configure timer ISR */
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_ਗਿਣਤੀ_ਉੱਪਰ,
        .resolution_hz = 1000000, // 1MHz, 1 tick = 1us
    };
    TEST_ESP_OK(gptimer_new_timer(&timer_config, &gptimer));
    timer_isr_fired = false;
    vTaskDelay(1); // Make sure we're at the start of a new tick
    gptimer_alarm_config_t alarm_config = {
        .alarm_ਗਿਣਤੀ = 1000000 / config_TICK_ਦਰ_HZ / 2,
        .reload_ਗਿਣਤੀ = 0,
    };
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_timer_alarm_cb,
    };
    TEST_ESP_OK(gptimer_ਰਜਿਸਟਰ_event_callbacks(gptimer, &cbs, suspend_task));
    TEST_ESP_OK(gptimer_ਸੈੱਟ_alarm_action(gptimer, &alarm_config));
    TEST_ESP_OK(gptimer_enable(gptimer));
    TEST_ESP_OK(gptimer_ਸ਼ੁਰੂ(gptimer));
    // wait the timer interrupt fires up
    vTaskDelay(2);
    TEST_ASSERT_TRUE(timer_isr_fired);
    // check the task was resumed
    TEST_ASSERT_NOT_EQUAL(0, resumed_counter);
    // The task should have woken within 500us of the timer interrupt event (note: task may be a flash cache miss)
    ਲਿਖੋ("alarm value %llu task resumed at %u\n", alarm_config.alarm_ਗਿਣਤੀ, (ਬਿਨਾਨਿਸ਼ਾਨੀ)resumed_counter);
    TEST_ASSERT_UINT32_WITHIN(100, alarm_config.alarm_ਗਿਣਤੀ, (ਬਿਨਾਨਿਸ਼ਾਨੀ)resumed_counter);

    // clean up
    TEST_ESP_OK(gptimer_disable(gptimer));
    TEST_ESP_OK(gptimer_del_timer(gptimer));
}

TEST_CASE("Resume task from ISR (same core)", "[freertos]")
{
    test_resume_task_from_isr(UNITY_FREERTOS_CPU);
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CONFIG_FREERTOS_UNICORE
TEST_CASE("Resume task from ISR (other core)", "[freertos]")
{
    test_resume_task_from_isr(!UNITY_FREERTOS_CPU);
}
#ਜੇ_ਅੰਤ // CONFIG_FREERTOS_UNICORE

#ਜੇ_ਅੰਤ //SOC_GPTIMER_SUPPORTED
