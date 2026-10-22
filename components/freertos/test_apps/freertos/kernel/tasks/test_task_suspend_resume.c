/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* Tests for FreeRTOS task suspend & resume */
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <inttypes.h>
#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"
#अन्तर्भाव "freertos/semphr.h"
#अन्तर्भाव "freertos/timers.h"
#अन्तर्भाव "freertos/queue.h"
#अन्तर्भाव "unity.h"
#अन्तर्भाव "test_utils.h"
#अन्तर्भाव "driver/gptimer.h"
#अन्तर्भाव "esp_rom_sys.h"

/* Counter task counts a target variable forever */
स्थिर शून्य task_गणनम्(शून्य *vp_counter)
{
    चञ्चल unsigned *counter = (चञ्चल unsigned *)vp_counter;
    *counter = 0;

    जाव (1) {
        (*counter)++;
        vTaskDelay(1);
    }
}

स्थिर शून्य test_suspend_resume(पूर्णाङ्क target_core)
{
    चञ्चल unsigned counter = 0;
    TaskHandle_t counter_task;

    xTaskCreatePinnedToCore(task_गणनम्, "Count", 2048,
                            (शून्य *)&counter, UNITY_FREERTOS_PRIORITY + 1,
                            &counter_task, target_core);

    vTaskDelay(10);
    /* check some counting has happened */
    TEST_ASSERT_NOT_EQUAL(0, counter);

    // Do the next part a few times, just to be sure multiple suspends & resumes
    // work as expected...
    ध्रुव पूर्णाङ्क TEST_ITERATIONS = 5;
    प्रति (पूर्णाङ्क i = 0; i < TEST_ITERATIONS; i++) {
        vTaskSuspend(counter_task);
        unsigned suspend_गणनम् = counter;
        printf("Suspending @ %d\n", suspend_गणनम्);

        vTaskDelay(2);

        printf("Still suspended @ %d\n", counter);

        /* check the counter hasn't gone up while the task is suspended */
        TEST_ASSERT_EQUAL(suspend_गणनम्, counter);
        vTaskResume(counter_task);
        vTaskDelay(2);

        printf("Resumed @ %d\n", counter);
        /* check the counter is going up again now the task is resumed */
        TEST_ASSERT_NOT_EQUAL(suspend_गणनम्, counter);
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
स्थिर शून्य task_suspend_self(शून्य *vp_resumed)
{
    चञ्चल bool *resumed = (चञ्चल bool *)vp_resumed;
    *resumed = false;
    vTaskSuspend(रिक्त);
    *resumed = true;
    vTaskDelete(रिक्त);
}

TEST_CASE("Suspend the current running task", "[freertos]")
{
    चञ्चल bool resumed = false;
    TaskHandle_t suspend_task;

    xTaskCreatePinnedToCore(task_suspend_self, "suspend_self", 2048,
                            (शून्य *)&resumed, UNITY_FREERTOS_PRIORITY + 1,
                            &suspend_task, UNITY_FREERTOS_CPU);

    vTaskDelay(1);
    TEST_ASSERT_FALSE(resumed);

    vTaskResume(suspend_task);
    // Shouldn't need any delay here, as task should resume on this CPU immediately
    TEST_ASSERT_TRUE(resumed);
}

#जइ SOC_GPTIMER_SUPPORTED

स्थिर चञ्चल bool timer_isr_fired;
स्थिर gptimer_handle_t gptimer = रिक्त;

/* Timer ISR clears interrupt, sets flag, then resumes the task supplied in the
 * callback argument.
 */
bool on_timer_alarm_cb(gptimer_handle_t timer, ध्रुव gptimer_alarm_event_data_t *edata, शून्य *user_ctx)
{
    TaskHandle_t handle = user_ctx;
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    gptimer_stop(timer);
    timer_isr_fired = true;
    higherPriorityTaskWoken = xTaskResumeFromISR(handle);
    प्रत्यागम higherPriorityTaskWoken == pdTRUE;
}

/* Task suspends itself, then sets parameter value to the current timer group counter and deletes itself */
स्थिर IRAM_ATTR शून्य task_suspend_self_with_timer(शून्य *vp_resumed)
{
    चञ्चल uint64_t *resumed_counter = vp_resumed;
    *resumed_counter = 0;
    vTaskSuspend(रिक्त);
    gptimer_get_raw_गणनम्(gptimer, (uint64_t *)resumed_counter);
    vTaskDelete(रिक्त);
}

/* Create a task which suspends itself, then resume it from a timer
 * interrupt. */
स्थिर शून्य test_resume_task_from_isr(पूर्णाङ्क target_core)
{
    चञ्चल uint64_t resumed_counter = 99;
    TaskHandle_t suspend_task;

    xTaskCreatePinnedToCore(task_suspend_self_with_timer, "suspend_self", 2048,
                            (शून्य *)&resumed_counter, UNITY_FREERTOS_PRIORITY + 1,
                            &suspend_task, target_core);
    // delay to make the task has resumed itself
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(0, resumed_counter);

    /* Configure timer ISR */
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_गणनम्_UP,
        .resolution_hz = 1000000, // 1MHz, 1 tick = 1us
    };
    TEST_ESP_OK(gptimer_new_timer(&timer_config, &gptimer));
    timer_isr_fired = false;
    vTaskDelay(1); // Make sure we're at the start of a new tick
    gptimer_alarm_config_t alarm_config = {
        .alarm_गणनम् = 1000000 / configTICK_RATE_HZ / 2,
        .reload_गणनम् = 0,
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
    printf("alarm value %llu task resumed at %u\n", alarm_config.alarm_गणनम्, (unsigned)resumed_counter);
    TEST_ASSERT_UINT32_WITHIN(100, alarm_config.alarm_गणनम्, (unsigned)resumed_counter);

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
