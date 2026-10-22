/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ <esp_types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <inttypes.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/semphr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/queue.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_intr_alloc.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_cpu.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "test_utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos_performance.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸੰਖਿਆ_ਦਾ_ITERATIONS 1023

ਸਥਿਰ ਪੂਰਨ compare_uint32(ਅਚਲ ਖਾਲੀ *a, ਅਚਲ ਖਾਲੀ *b)
{
    ਵਾਪਸ (*(uint32_t *)a - * (uint32_t *)b);
}

ਸਥਿਰ uint32_t calculate_median(uint32_t *ਮੁੱਲਾਂ, ਪੂਰਨ ਮਾਪ)
{
    /* Sort the array of values containing the scheduling time of each iteration */
    qsort(ਮੁੱਲਾਂ, ਮਾਪ, ਆਕਾਰ(uint32_t), compare_uint32);

    /* Return the median value. (size is an odd number) */
    ਵਾਪਸ ਮੁੱਲਾਂ[ਮਾਪ / 2];
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    SemaphoreHandle_t ਸਿਰਾ_sema;
    uint32_t before_sched;
    uint32_t ਚੱਕਰਾਂ_ਮੰਜ਼ਿਲ_sched[ਸੰਖਿਆ_ਦਾ_ITERATIONS];
    TaskHandle_t t1_handle;
} test_context_t;

ਸਥਿਰ ਖਾਲੀ test_task_1(ਖਾਲੀ *arg)
{
    test_context_t *context = (test_context_t *)arg;

    ਲਈ (;;) {
        context->before_sched = esp_cpu_get_ਚੱਕਰ_ਗਿਣਤੀ();
        taskYIELD();
    }

    vTaskDelete(ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ test_task_2(ਖਾਲੀ *arg)
{
    test_context_t *context = (test_context_t *)arg;

    v_Task_ਤਰਜੀਹ_ਸੈੱਟ(ਨਿਰਾ, CONFIG_UNITY_FREERTOS_ਤਰਜੀਹ + 1);
    v_Task_ਤਰਜੀਹ_ਸੈੱਟ(context->t1_handle, CONFIG_UNITY_FREERTOS_ਤਰਜੀਹ + 1);
    taskYIELD();

    ਲਈ (ਪੂਰਨ i = 0; i < ਸੰਖਿਆ_ਦਾ_ITERATIONS; i++) {
        context->ਚੱਕਰਾਂ_ਮੰਜ਼ਿਲ_sched[i] = esp_cpu_get_ਚੱਕਰ_ਗਿਣਤੀ() - context->before_sched;
        taskYIELD();
    }

    xSemaphoreGive(context->ਸਿਰਾ_sema);
    vTaskDelete(context->t1_handle);
    vTaskDelete(ਨਿਰਾ);
}

TEST_CASE("scheduling time test", "[freertos]")
{
    test_context_t context;

    context.ਸਿਰਾ_sema = x_Semaphore_Create_ਦੁਆਧਾਰੀ();
    TEST_ASSERT(context.ਸਿਰਾ_sema != ਨਿਰਾ);

#ਜੇ !CONFIG_FREERTOS_UNICORE
    x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_ਤਰਜੀਹ - 1, &context.t1_handle, 1);
    x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_ਤਰਜੀਹ - 1, ਨਿਰਾ, 1);
#ਨਹੀਂਤਾਂ
    x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_ਤਰਜੀਹ - 1, &context.t1_handle, 0);
    x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_ਤਰਜੀਹ - 1, ਨਿਰਾ, 0);
#ਜੇ_ਅੰਤ

    ਅਧਾਰ_Type_t ਨਤੀਜਾ = xSemaphoreTake(context.ਸਿਰਾ_sema, ਪੋਰਟ_MAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, ਨਤੀਜਾ);

    /* We calculate the median instead of the average to avoid outliers which may occur due to cache speed variabilities */
    uint32_t median_ਚੱਕਰਾਂ = calculate_median(context.ਚੱਕਰਾਂ_ਮੰਜ਼ਿਲ_sched, ਸੰਖਿਆ_ਦਾ_ITERATIONS);
    TEST_PERFORMANCE_LESS_THAN(SCHEDULING_ਸਮਾਂ, "%"PRIu32" cycles", median_ਚੱਕਰਾਂ);

    /* Cleanup */
    vSemaphoreDelete(context.ਸਿਰਾ_sema);
}
