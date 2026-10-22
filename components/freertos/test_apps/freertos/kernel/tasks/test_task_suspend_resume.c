/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* Tests for FreeRTOS task suspend & resume */
#समावेश <stdio.h>
#समावेश <string.h>
#समावेश <inttypes.h>
#समावेश "sdkconfig.h"
#समावेश "freertos/FreeRTOS.h"
#समावेश "freertos/task.h"
#समावेश "freertos/semphr.h"
#समावेश "freertos/timers.h"
#समावेश "freertos/queue.h"
#समावेश "unity.h"
#समावेश "test_utils.h"
#समावेश "driver/gptimer.h"
#समावेश "esp_rom_sys.h"

/* Counter task counts a target variable forever */
अटल व्योम task_गिनती(व्योम *vp_गणक)
{
    अस्थिर अचिन्हित *गणक = (अस्थिर अचिन्हित *)vp_गणक;
    *गणक = 0;

    जबतक (1) {
        (*गणक)++;
        vTaskDelay(1);
    }
}

अटल व्योम test_suspend_resume(पूर्णांक target_core)
{
    अस्थिर अचिन्हित गणक = 0;
    TaskHandle_t गणक_task;

    xTaskCreatePinnedToCore(task_गिनती, "Count", 2048,
                            (व्योम *)&गणक, UNITY_FREERTOS_PRIORITY + 1,
                            &गणक_task, target_core);

    vTaskDelay(10);
    /* check some counting has happened */
    TEST_ASSERT_NOT_EQUAL(0, गणक);

    // Do the next part a few times, just to be sure multiple suspends & resumes
    // work as expected...
    स्थिर पूर्णांक TEST_ITERATIONS = 5;
    क्रम (पूर्णांक i = 0; i < TEST_ITERATIONS; i++) {
        vTaskSuspend(गणक_task);
        अचिन्हित suspend_गिनती = गणक;
        म_लिखो("Suspending @ %d\n", suspend_गिनती);

        vTaskDelay(2);

        म_लिखो("Still suspended @ %d\n", गणक);

        /* check the counter hasn't gone up while the task is suspended */
        TEST_ASSERT_EQUAL(suspend_गिनती, गणक);
        vTaskResume(गणक_task);
        vTaskDelay(2);

        म_लिखो("Resumed @ %d\n", गणक);
        /* check the counter is going up again now the task is resumed */
        TEST_ASSERT_NOT_EQUAL(suspend_गिनती, गणक);
    }

    vTaskDelete(गणक_task);
}

TEST_CASE("Suspend/resume task on same core", "[freertos]")
{
    test_suspend_resume(UNITY_FREERTOS_CPU);
}

#अगर_अघोषित CONFIG_FREERTOS_UNICORE
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
#पूर्ण_अगर

/* Task suspends itself, then sets a flag and deletes itself */
अटल व्योम task_suspend_self(व्योम *vp_resumed)
{
    अस्थिर bool *resumed = (अस्थिर bool *)vp_resumed;
    *resumed = false;
    vTaskSuspend(शून्य);
    *resumed = true;
    vTaskDelete(शून्य);
}

TEST_CASE("Suspend the current running task", "[freertos]")
{
    अस्थिर bool resumed = false;
    TaskHandle_t suspend_task;

    xTaskCreatePinnedToCore(task_suspend_self, "suspend_self", 2048,
                            (व्योम *)&resumed, UNITY_FREERTOS_PRIORITY + 1,
                            &suspend_task, UNITY_FREERTOS_CPU);

    vTaskDelay(1);
    TEST_ASSERT_FALSE(resumed);

    vTaskResume(suspend_task);
    // Shouldn't need any delay here, as task should resume on this CPU immediately
    TEST_ASSERT_TRUE(resumed);
}

#अगर SOC_GPTIMER_SUPPORTED

अटल अस्थिर bool timer_isr_fired;
अटल gptimer_handle_t gptimer = शून्य;

/* Timer ISR clears interrupt, sets flag, then resumes the task supplied in the
 * callback argument.
 */
bool on_timer_alarm_cb(gptimer_handle_t timer, स्थिर gptimer_alarm_event_तथ्य_t *edata, व्योम *user_ctx)
{
    TaskHandle_t handle = user_ctx;
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    gptimer_stop(timer);
    timer_isr_fired = true;
    higherPriorityTaskWoken = xTaskResumeFromISR(handle);
    वापस higherPriorityTaskWoken == pdTRUE;
}

/* Task suspends itself, then sets parameter value to the current timer group counter and deletes itself */
अटल IRAM_ATTR व्योम task_suspend_self_with_timer(व्योम *vp_resumed)
{
    अस्थिर uint64_t *resumed_गणक = vp_resumed;
    *resumed_गणक = 0;
    vTaskSuspend(शून्य);
    gptimer_get_raw_गिनती(gptimer, (uint64_t *)resumed_गणक);
    vTaskDelete(शून्य);
}

/* Create a task which suspends itself, then resume it from a timer
 * interrupt. */
अटल व्योम test_resume_task_from_isr(पूर्णांक target_core)
{
    अस्थिर uint64_t resumed_गणक = 99;
    TaskHandle_t suspend_task;

    xTaskCreatePinnedToCore(task_suspend_self_with_timer, "suspend_self", 2048,
                            (व्योम *)&resumed_गणक, UNITY_FREERTOS_PRIORITY + 1,
                            &suspend_task, target_core);
    // delay to make the task has resumed itself
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(0, resumed_गणक);

    /* Configure timer ISR */
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_गिनती_UP,
        .resolution_hz = 1000000, // 1MHz, 1 tick = 1us
    };
    TEST_ESP_OK(gptimer_new_timer(&timer_config, &gptimer));
    timer_isr_fired = false;
    vTaskDelay(1); // Make sure we're at the start of a new tick
    gptimer_alarm_config_t alarm_config = {
        .alarm_गिनती = 1000000 / configTICK_RATE_HZ / 2,
        .reload_गिनती = 0,
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
    TEST_ASSERT_NOT_EQUAL(0, resumed_गणक);
    // The task should have woken within 500us of the timer interrupt event (note: task may be a flash cache miss)
    म_लिखो("alarm value %llu task resumed at %u\n", alarm_config.alarm_गिनती, (अचिन्हित)resumed_गणक);
    TEST_ASSERT_UINT32_WITHIN(100, alarm_config.alarm_गिनती, (अचिन्हित)resumed_गणक);

    // clean up
    TEST_ESP_OK(gptimer_disable(gptimer));
    TEST_ESP_OK(gptimer_del_timer(gptimer));
}

TEST_CASE("Resume task from ISR (same core)", "[freertos]")
{
    test_resume_task_from_isr(UNITY_FREERTOS_CPU);
}

#अगर_अघोषित CONFIG_FREERTOS_UNICORE
TEST_CASE("Resume task from ISR (other core)", "[freertos]")
{
    test_resume_task_from_isr(!UNITY_FREERTOS_CPU);
}
#पूर्ण_अगर // CONFIG_FREERTOS_UNICORE

#पूर्ण_अगर //SOC_GPTIMER_SUPPORTED
