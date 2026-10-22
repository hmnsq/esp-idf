/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* Tests for FreeRTOS task suspend & resume */
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <inttypes.h>
#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "freertos/semphr.h"
#ಅನ್ತರ್ಭಾವ "freertos/timers.h"
#ಅನ್ತರ್ಭಾವ "freertos/queue.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "test_utils.h"
#ಅನ್ತರ್ಭಾವ "driver/gptimer.h"
#ಅನ್ತರ್ಭಾವ "esp_rom_sys.h"

/* Counter task counts a target variable forever */
ಸ್ಥಿರ ಶೂನ್ಯ task_ಎಣಿಕೆ(ಶೂನ್ಯ *vp_counter)
{
    ಅಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ *counter = (ಅಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ *)vp_counter;
    *counter = 0;

    ಆಗ (1) {
        (*counter)++;
        vTaskDelay(1);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ test_suspend_resume(ಪೂರ್ಣಾಂಕ ಗುರಿ_core)
{
    ಅಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ counter = 0;
    TaskHandle_t counter_task;

    x_Task_Create_Pinned_ಗಮ್ಯ_Core(task_ಎಣಿಕೆ, "Count", 2048,
                            (ಶೂನ್ಯ *)&counter, UNITY_FREERTOS_ಆದ್ಯತೆ + 1,
                            &counter_task, ಗುರಿ_core);

    vTaskDelay(10);
    /* check some counting has happened */
    TEST_ASSERT_NOT_EQUAL(0, counter);

    // Do the next part a few times, just to be sure multiple suspends & resumes
    // work as expected...
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ TEST_ITERATIONS = 5;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < TEST_ITERATIONS; i++) {
        vTaskSuspend(counter_task);
        ಚಿಹ್ನೆರಹಿತ suspend_ಎಣಿಕೆ = counter;
        ಬರೆ("Suspending @ %d\n", suspend_ಎಣಿಕೆ);

        vTaskDelay(2);

        ಬರೆ("Still suspended @ %d\n", counter);

        /* check the counter hasn't gone up while the task is suspended */
        TEST_ASSERT_EQUAL(suspend_ಎಣಿಕೆ, counter);
        vTaskResume(counter_task);
        vTaskDelay(2);

        ಬರೆ("Resumed @ %d\n", counter);
        /* check the counter is going up again now the task is resumed */
        TEST_ASSERT_NOT_EQUAL(suspend_ಎಣಿಕೆ, counter);
    }

    vTaskDelete(counter_task);
}

TEST_CASE("Suspend/resume task on same core", "[freertos]")
{
    test_suspend_resume(UNITY_FREERTOS_CPU);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CONFIG_FREERTOS_UNICORE
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
#ಆದರೆ_ಅಂತ್ಯ

/* Task suspends itself, then sets a flag and deletes itself */
ಸ್ಥಿರ ಶೂನ್ಯ task_suspend_self(ಶೂನ್ಯ *vp_resumed)
{
    ಅಸ್ಥಿರ bool *resumed = (ಅಸ್ಥಿರ bool *)vp_resumed;
    *resumed = false;
    vTaskSuspend(ಖಾಲಿ);
    *resumed = true;
    vTaskDelete(ಖಾಲಿ);
}

TEST_CASE("Suspend the current running task", "[freertos]")
{
    ಅಸ್ಥಿರ bool resumed = false;
    TaskHandle_t suspend_task;

    x_Task_Create_Pinned_ಗಮ್ಯ_Core(task_suspend_self, "suspend_self", 2048,
                            (ಶೂನ್ಯ *)&resumed, UNITY_FREERTOS_ಆದ್ಯತೆ + 1,
                            &suspend_task, UNITY_FREERTOS_CPU);

    vTaskDelay(1);
    TEST_ASSERT_FALSE(resumed);

    vTaskResume(suspend_task);
    // Shouldn't need any delay here, as task should resume on this CPU immediately
    TEST_ASSERT_TRUE(resumed);
}

#ಆದರೆ SOC_GPTIMER_SUPPORTED

ಸ್ಥಿರ ಅಸ್ಥಿರ bool timer_isr_fired;
ಸ್ಥಿರ gptimer_handle_t gptimer = ಖಾಲಿ;

/* Timer ISR clears interrupt, sets flag, then resumes the task supplied in the
 * callback argument.
 */
bool on_timer_alarm_cb(gptimer_handle_t timer, ಸ್ಥಿರಾಂಕ gptimer_alarm_event_ದತ್ತಾಂಶ_t *edata, ಶೂನ್ಯ *user_ctx)
{
    TaskHandle_t handle = user_ctx;
    ತಳ_Type_t higher_ಆದ್ಯತೆ_Task_Woken = pdFALSE;
    gptimer_stop(timer);
    timer_isr_fired = true;
    higher_ಆದ್ಯತೆ_Task_Woken = xTaskResumeFromISR(handle);
    ಹಿಂತಿರುಗಿ higher_ಆದ್ಯತೆ_Task_Woken == pdTRUE;
}

/* Task suspends itself, then sets parameter value to the current timer group counter and deletes itself */
ಸ್ಥಿರ IRAM_ATTR ಶೂನ್ಯ task_suspend_self_with_timer(ಶೂನ್ಯ *vp_resumed)
{
    ಅಸ್ಥಿರ uint64_t *resumed_counter = vp_resumed;
    *resumed_counter = 0;
    vTaskSuspend(ಖಾಲಿ);
    gptimer_get_raw_ಎಣಿಕೆ(gptimer, (uint64_t *)resumed_counter);
    vTaskDelete(ಖಾಲಿ);
}

/* Create a task which suspends itself, then resume it from a timer
 * interrupt. */
ಸ್ಥಿರ ಶೂನ್ಯ test_resume_task_from_isr(ಪೂರ್ಣಾಂಕ ಗುರಿ_core)
{
    ಅಸ್ಥಿರ uint64_t resumed_counter = 99;
    TaskHandle_t suspend_task;

    x_Task_Create_Pinned_ಗಮ್ಯ_Core(task_suspend_self_with_timer, "suspend_self", 2048,
                            (ಶೂನ್ಯ *)&resumed_counter, UNITY_FREERTOS_ಆದ್ಯತೆ + 1,
                            &suspend_task, ಗುರಿ_core);
    // delay to make the task has resumed itself
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(0, resumed_counter);

    /* Configure timer ISR */
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_ಎಣಿಕೆ_ಮೇಲೆ,
        .resolution_hz = 1000000, // 1MHz, 1 tick = 1us
    };
    TEST_ESP_OK(gptimer_new_timer(&timer_config, &gptimer));
    timer_isr_fired = false;
    vTaskDelay(1); // Make sure we're at the start of a new tick
    gptimer_alarm_config_t alarm_config = {
        .alarm_ಎಣಿಕೆ = 1000000 / config_TICK_ದರ_HZ / 2,
        .reload_ಎಣಿಕೆ = 0,
    };
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_timer_alarm_cb,
    };
    TEST_ESP_OK(gptimer_ರಿಜಿಸ್ಟರ್_event_callbacks(gptimer, &cbs, suspend_task));
    TEST_ESP_OK(gptimer_ಗಣ_alarm_action(gptimer, &alarm_config));
    TEST_ESP_OK(gptimer_enable(gptimer));
    TEST_ESP_OK(gptimer_ಆರಂಭ(gptimer));
    // wait the timer interrupt fires up
    vTaskDelay(2);
    TEST_ASSERT_TRUE(timer_isr_fired);
    // check the task was resumed
    TEST_ASSERT_NOT_EQUAL(0, resumed_counter);
    // The task should have woken within 500us of the timer interrupt event (note: task may be a flash cache miss)
    ಬರೆ("alarm value %llu task resumed at %u\n", alarm_config.alarm_ಎಣಿಕೆ, (ಚಿಹ್ನೆರಹಿತ)resumed_counter);
    TEST_ASSERT_UINT32_WITHIN(100, alarm_config.alarm_ಎಣಿಕೆ, (ಚಿಹ್ನೆರಹಿತ)resumed_counter);

    // clean up
    TEST_ESP_OK(gptimer_disable(gptimer));
    TEST_ESP_OK(gptimer_del_timer(gptimer));
}

TEST_CASE("Resume task from ISR (same core)", "[freertos]")
{
    test_resume_task_from_isr(UNITY_FREERTOS_CPU);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CONFIG_FREERTOS_UNICORE
TEST_CASE("Resume task from ISR (other core)", "[freertos]")
{
    test_resume_task_from_isr(!UNITY_FREERTOS_CPU);
}
#ಆದರೆ_ಅಂತ್ಯ // CONFIG_FREERTOS_UNICORE

#ಆದರೆ_ಅಂತ್ಯ //SOC_GPTIMER_SUPPORTED
