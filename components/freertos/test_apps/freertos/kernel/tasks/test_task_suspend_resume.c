/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* Tests for FreeRTOS task suspend & resume */
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <inttypes.h>
#ଅନ୍ତର୍ଭାଵ "sdkconfig.h"
#ଅନ୍ତର୍ଭାଵ "freertos/FreeRTOS.h"
#ଅନ୍ତର୍ଭାଵ "freertos/task.h"
#ଅନ୍ତର୍ଭାଵ "freertos/semphr.h"
#ଅନ୍ତର୍ଭାଵ "freertos/timers.h"
#ଅନ୍ତର୍ଭାଵ "freertos/queue.h"
#ଅନ୍ତର୍ଭାଵ "unity.h"
#ଅନ୍ତର୍ଭାଵ "test_utils.h"
#ଅନ୍ତର୍ଭାଵ "driver/gptimer.h"
#ଅନ୍ତର୍ଭାଵ "esp_rom_sys.h"

/* Counter task counts a target variable forever */
ସ୍ଥିର ଶୂନ୍ୟ task_ଗଣନା(ଶୂନ୍ୟ *vp_counter)
{
    ଅସ୍ଥିର unsigned *counter = (ଅସ୍ଥିର unsigned *)vp_counter;
    *counter = 0;

    ଯେତେବେଳେ (1) {
        (*counter)++;
        vTaskDelay(1);
    }
}

ସ୍ଥିର ଶୂନ୍ୟ test_suspend_resume(ପୂର୍ଣ୍ଣସଂଖ୍ୟା target_core)
{
    ଅସ୍ଥିର unsigned counter = 0;
    TaskHandle_t counter_task;

    xTaskCreatePinnedToCore(task_ଗଣନା, "Count", 2048,
                            (ଶୂନ୍ୟ *)&counter, UNITY_FREERTOS_PRIORITY + 1,
                            &counter_task, target_core);

    vTaskDelay(10);
    /* check some counting has happened */
    TEST_ASSERT_NOT_EQUAL(0, counter);

    // Do the next part a few times, just to be sure multiple suspends & resumes
    // work as expected...
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା TEST_ITERATIONS = 5;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < TEST_ITERATIONS; i++) {
        vTaskSuspend(counter_task);
        unsigned suspend_ଗଣନା = counter;
        printf("Suspending @ %d\n", suspend_ଗଣନା);

        vTaskDelay(2);

        printf("Still suspended @ %d\n", counter);

        /* check the counter hasn't gone up while the task is suspended */
        TEST_ASSERT_EQUAL(suspend_ଗଣନା, counter);
        vTaskResume(counter_task);
        vTaskDelay(2);

        printf("Resumed @ %d\n", counter);
        /* check the counter is going up again now the task is resumed */
        TEST_ASSERT_NOT_EQUAL(suspend_ଗଣନା, counter);
    }

    vTaskDelete(counter_task);
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
ସ୍ଥିର ଶୂନ୍ୟ task_suspend_self(ଶୂନ୍ୟ *vp_resumed)
{
    ଅସ୍ଥିର bool *resumed = (ଅସ୍ଥିର bool *)vp_resumed;
    *resumed = false;
    vTaskSuspend(ଖାଲି);
    *resumed = true;
    vTaskDelete(ଖାଲି);
}

TEST_CASE("Suspend the current running task", "[freertos]")
{
    ଅସ୍ଥିର bool resumed = false;
    TaskHandle_t suspend_task;

    xTaskCreatePinnedToCore(task_suspend_self, "suspend_self", 2048,
                            (ଶୂନ୍ୟ *)&resumed, UNITY_FREERTOS_PRIORITY + 1,
                            &suspend_task, UNITY_FREERTOS_CPU);

    vTaskDelay(1);
    TEST_ASSERT_FALSE(resumed);

    vTaskResume(suspend_task);
    // Shouldn't need any delay here, as task should resume on this CPU immediately
    TEST_ASSERT_TRUE(resumed);
}

#ଯଦି SOC_GPTIMER_SUPPORTED

ସ୍ଥିର ଅସ୍ଥିର bool timer_isr_fired;
ସ୍ଥିର gptimer_handle_t gptimer = ଖାଲି;

/* Timer ISR clears interrupt, sets flag, then resumes the task supplied in the
 * callback argument.
 */
bool on_timer_alarm_cb(gptimer_handle_t timer, ଅଚଳ gptimer_alarm_event_data_t *edata, ଶୂନ୍ୟ *user_ctx)
{
    TaskHandle_t handle = user_ctx;
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    gptimer_stop(timer);
    timer_isr_fired = true;
    higherPriorityTaskWoken = xTaskResumeFromISR(handle);
    ଫେରନ୍ତୁ higherPriorityTaskWoken == pdTRUE;
}

/* Task suspends itself, then sets parameter value to the current timer group counter and deletes itself */
ସ୍ଥିର IRAM_ATTR ଶୂନ୍ୟ task_suspend_self_with_timer(ଶୂନ୍ୟ *vp_resumed)
{
    ଅସ୍ଥିର uint64_t *resumed_counter = vp_resumed;
    *resumed_counter = 0;
    vTaskSuspend(ଖାଲି);
    gptimer_get_raw_ଗଣନା(gptimer, (uint64_t *)resumed_counter);
    vTaskDelete(ଖାଲି);
}

/* Create a task which suspends itself, then resume it from a timer
 * interrupt. */
ସ୍ଥିର ଶୂନ୍ୟ test_resume_task_from_isr(ପୂର୍ଣ୍ଣସଂଖ୍ୟା target_core)
{
    ଅସ୍ଥିର uint64_t resumed_counter = 99;
    TaskHandle_t suspend_task;

    xTaskCreatePinnedToCore(task_suspend_self_with_timer, "suspend_self", 2048,
                            (ଶୂନ୍ୟ *)&resumed_counter, UNITY_FREERTOS_PRIORITY + 1,
                            &suspend_task, target_core);
    // delay to make the task has resumed itself
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(0, resumed_counter);

    /* Configure timer ISR */
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_ଗଣନା_UP,
        .resolution_hz = 1000000, // 1MHz, 1 tick = 1us
    };
    TEST_ESP_OK(gptimer_new_timer(&timer_config, &gptimer));
    timer_isr_fired = false;
    vTaskDelay(1); // Make sure we're at the start of a new tick
    gptimer_alarm_config_t alarm_config = {
        .alarm_ଗଣନା = 1000000 / configTICK_RATE_HZ / 2,
        .reload_ଗଣନା = 0,
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
    TEST_ASSERT_NOT_EQUAL(0, resumed_counter);
    // The task should have woken within 500us of the timer interrupt event (note: task may be a flash cache miss)
    printf("alarm value %llu task resumed at %u\n", alarm_config.alarm_ଗଣନା, (unsigned)resumed_counter);
    TEST_ASSERT_UINT32_WITHIN(100, alarm_config.alarm_ଗଣନା, (unsigned)resumed_counter);

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
