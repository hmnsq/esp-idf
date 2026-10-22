/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#شامل <esp_types.h>
#شامل <stdio.h>
#شامل <inttypes.h>
#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل "freertos/semphr.h"
#شامل "freertos/queue.h"
#شامل "esp_intr_alloc.h"
#شامل "esp_cpu.h"
#شامل "unity.h"
#شامل "test_utils.h"
#شامل "freertos_performance.h"

#define عدد_OF_ITERATIONS 1023

ایستا صحیح compare_uint32(ثابت تهی *a, ثابت تهی *b)
{
    بازگشت (*(uint32_t *)a - * (uint32_t *)b);
}

ایستا uint32_t calculate_median(uint32_t *مقادیر, صحیح size)
{
    /* Sort the array of values containing the scheduling time of each iteration */
    qsort(مقادیر, size, اندازه(uint32_t), compare_uint32);

    /* Return the median value. (size is an odd number) */
    بازگشت مقادیر[size / 2];
}

تعریفنوع ساختار {
    SemaphoreHandle_t end_sema;
    uint32_t before_sched;
    uint32_t cycles_to_sched[عدد_OF_ITERATIONS];
    TaskHandle_t t1_handle;
} test_context_t;

ایستا تهی test_task_1(تهی *arg)
{
    test_context_t *context = (test_context_t *)arg;

    برای (;;) {
        context->before_sched = esp_cpu_get_cycle_شمار();
        taskYIELD();
    }

    vTaskDelete(پوچ);
}

ایستا تهی test_task_2(تهی *arg)
{
    test_context_t *context = (test_context_t *)arg;

    vTaskPrioritySet(پوچ, CONFIG_UNITY_FREERTOS_PRIORITY + 1);
    vTaskPrioritySet(context->t1_handle, CONFIG_UNITY_FREERTOS_PRIORITY + 1);
    taskYIELD();

    برای (صحیح i = 0; i < عدد_OF_ITERATIONS; i++) {
        context->cycles_to_sched[i] = esp_cpu_get_cycle_شمار() - context->before_sched;
        taskYIELD();
    }

    xSemaphoreGive(context->end_sema);
    vTaskDelete(context->t1_handle);
    vTaskDelete(پوچ);
}

TEST_CASE("scheduling time test", "[freertos]")
{
    test_context_t context;

    context.end_sema = x_Semaphore_Create_دودویی();
    TEST_ASSERT(context.end_sema != پوچ);

#اگر !CONFIG_FREERTOS_UNICORE
    xTaskCreatePinnedToCore(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, &context.t1_handle, 1);
    xTaskCreatePinnedToCore(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, پوچ, 1);
#وگرنه
    xTaskCreatePinnedToCore(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, &context.t1_handle, 0);
    xTaskCreatePinnedToCore(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, پوچ, 0);
#endif

    BaseType_t result = xSemaphoreTake(context.end_sema, portMAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, result);

    /* We calculate the median instead of the average to avoid outliers which may occur due to cache speed variabilities */
    uint32_t median_cycles = calculate_median(context.cycles_to_sched, عدد_OF_ITERATIONS);
    TEST_PERFORMANCE_LESS_THAN(SCHEDULING_TIME, "%"PRIu32" cycles", median_cycles);

    /* Cleanup */
    vSemaphoreDelete(context.end_sema);
}
