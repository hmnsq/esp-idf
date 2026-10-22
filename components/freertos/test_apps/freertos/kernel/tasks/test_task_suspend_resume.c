/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* Tests for FreeRTOS task suspend & resume */
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <inttypes.h>
#অন্তর্ভাব "sdkconfig.h"
#অন্তর্ভাব "freertos/FreeRTOS.h"
#অন্তর্ভাব "freertos/task.h"
#অন্তর্ভাব "freertos/semphr.h"
#অন্তর্ভাব "freertos/timers.h"
#অন্তর্ভাব "freertos/queue.h"
#অন্তর্ভাব "unity.h"
#অন্তর্ভাব "test_utils.h"
#অন্তর্ভাব "driver/gptimer.h"
#অন্তর্ভাব "esp_rom_sys.h"

/* Counter task counts a target variable forever */
স্থির শূন্য task_গুনতি(শূন্য *vp_গণক)
{
    অস্থির unsigned *গণক = (অস্থির unsigned *)vp_গণক;
    *গণক = 0;

    যখন (1) {
        (*গণক)++;
        vTaskDelay(1);
    }
}

স্থির শূন্য test_suspend_resume(পূর্ণসংখ্যা target_core)
{
    অস্থির unsigned গণক = 0;
    TaskHandle_t গণক_task;

    xTaskCreatePinnedToCore(task_গুনতি, "Count", 2048,
                            (শূন্য *)&গণক, UNITY_FREERTOS_PRIORITY + 1,
                            &গণক_task, target_core);

    vTaskDelay(10);
    /* check some counting has happened */
    TEST_ASSERT_NOT_EQUAL(0, গণক);

    // Do the next part a few times, just to be sure multiple suspends & resumes
    // work as expected...
    ধ্রুবক পূর্ণসংখ্যা TEST_ITERATIONS = 5;
    জন্য (পূর্ণসংখ্যা i = 0; i < TEST_ITERATIONS; i++) {
        vTaskSuspend(গণক_task);
        unsigned suspend_গুনতি = গণক;
        printf("Suspending @ %d\n", suspend_গুনতি);

        vTaskDelay(2);

        printf("Still suspended @ %d\n", গণক);

        /* check the counter hasn't gone up while the task is suspended */
        TEST_ASSERT_EQUAL(suspend_গুনতি, গণক);
        vTaskResume(গণক_task);
        vTaskDelay(2);

        printf("Resumed @ %d\n", গণক);
        /* check the counter is going up again now the task is resumed */
        TEST_ASSERT_NOT_EQUAL(suspend_গুনতি, গণক);
    }

    vTaskDelete(গণক_task);
}

TEST_CASE("Suspend/resume task on same core", "[freertos]")
{
    test_suspend_resume(UNITY_FREERTOS_CPU);
}

#ifndef CONFIG_FREERTOS_UNICORE
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
#endif

/* Task suspends itself, then sets a flag and deletes itself */
স্থির শূন্য task_suspend_self(শূন্য *vp_resumed)
{
    অস্থির bool *resumed = (অস্থির bool *)vp_resumed;
    *resumed = false;
    vTaskSuspend(ফাঁকা);
    *resumed = true;
    vTaskDelete(ফাঁকা);
}

TEST_CASE("Suspend the current running task", "[freertos]")
{
    অস্থির bool resumed = false;
    TaskHandle_t suspend_task;

    xTaskCreatePinnedToCore(task_suspend_self, "suspend_self", 2048,
                            (শূন্য *)&resumed, UNITY_FREERTOS_PRIORITY + 1,
                            &suspend_task, UNITY_FREERTOS_CPU);

    vTaskDelay(1);
    TEST_ASSERT_FALSE(resumed);

    vTaskResume(suspend_task);
    // Shouldn't need any delay here, as task should resume on this CPU immediately
    TEST_ASSERT_TRUE(resumed);
}

#যদি SOC_GPTIMER_SUPPORTED

স্থির অস্থির bool timer_isr_fired;
স্থির gptimer_handle_t gptimer = ফাঁকা;

/* Timer ISR clears interrupt, sets flag, then resumes the task supplied in the
 * callback argument.
 */
bool on_timer_alarm_cb(gptimer_handle_t timer, ধ্রুবক gptimer_alarm_event_তথ্য_t *edata, শূন্য *user_ctx)
{
    TaskHandle_t handle = user_ctx;
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    gptimer_stop(timer);
    timer_isr_fired = true;
    higherPriorityTaskWoken = xTaskResumeFromISR(handle);
    ফেরত higherPriorityTaskWoken == pdTRUE;
}

/* Task suspends itself, then sets parameter value to the current timer group counter and deletes itself */
স্থির IRAM_ATTR শূন্য task_suspend_self_with_timer(শূন্য *vp_resumed)
{
    অস্থির uint64_t *resumed_গণক = vp_resumed;
    *resumed_গণক = 0;
    vTaskSuspend(ফাঁকা);
    gptimer_get_raw_গুনতি(gptimer, (uint64_t *)resumed_গণক);
    vTaskDelete(ফাঁকা);
}

/* Create a task which suspends itself, then resume it from a timer
 * interrupt. */
স্থির শূন্য test_resume_task_from_isr(পূর্ণসংখ্যা target_core)
{
    অস্থির uint64_t resumed_গণক = 99;
    TaskHandle_t suspend_task;

    xTaskCreatePinnedToCore(task_suspend_self_with_timer, "suspend_self", 2048,
                            (শূন্য *)&resumed_গণক, UNITY_FREERTOS_PRIORITY + 1,
                            &suspend_task, target_core);
    // delay to make the task has resumed itself
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(0, resumed_গণক);

    /* Configure timer ISR */
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_গুনতি_UP,
        .resolution_hz = 1000000, // 1MHz, 1 tick = 1us
    };
    TEST_ESP_OK(gptimer_new_timer(&timer_config, &gptimer));
    timer_isr_fired = false;
    vTaskDelay(1); // Make sure we're at the start of a new tick
    gptimer_alarm_config_t alarm_config = {
        .alarm_গুনতি = 1000000 / configTICK_RATE_HZ / 2,
        .reload_গুনতি = 0,
    };
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_timer_alarm_cb,
    };
    TEST_ESP_OK(gptimer_register_event_callbacks(gptimer, &cbs, suspend_task));
    TEST_ESP_OK(gptimer_set_alarm_action(gptimer, &alarm_config));
    TEST_ESP_OK(gptimer_enable(gptimer));
    TEST_ESP_OK(gptimer_start(gptimer));
    // wait the timer interrupt fires up
    vTaskDelay(2);
    TEST_ASSERT_TRUE(timer_isr_fired);
    // check the task was resumed
    TEST_ASSERT_NOT_EQUAL(0, resumed_গণক);
    // The task should have woken within 500us of the timer interrupt event (note: task may be a flash cache miss)
    printf("alarm value %llu task resumed at %u\n", alarm_config.alarm_গুনতি, (unsigned)resumed_গণক);
    TEST_ASSERT_UINT32_WITHIN(100, alarm_config.alarm_গুনতি, (unsigned)resumed_গণক);

    // clean up
    TEST_ESP_OK(gptimer_disable(gptimer));
    TEST_ESP_OK(gptimer_del_timer(gptimer));
}

TEST_CASE("Resume task from ISR (same core)", "[freertos]")
{
    test_resume_task_from_isr(UNITY_FREERTOS_CPU);
}

#ifndef CONFIG_FREERTOS_UNICORE
TEST_CASE("Resume task from ISR (other core)", "[freertos]")
{
    test_resume_task_from_isr(!UNITY_FREERTOS_CPU);
}
#endif // CONFIG_FREERTOS_UNICORE

#endif //SOC_GPTIMER_SUPPORTED
