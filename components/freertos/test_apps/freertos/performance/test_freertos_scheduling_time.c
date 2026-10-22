/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#অন্তর্ভাৱ <esp_types.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <inttypes.h>
#অন্তর্ভাৱ "freertos/FreeRTOS.h"
#অন্তর্ভাৱ "freertos/task.h"
#অন্তর্ভাৱ "freertos/semphr.h"
#অন্তর্ভাৱ "freertos/queue.h"
#অন্তর্ভাৱ "esp_intr_alloc.h"
#অন্তর্ভাৱ "esp_cpu.h"
#অন্তর্ভাৱ "unity.h"
#অন্তর্ভাৱ "test_utils.h"
#অন্তর্ভাৱ "freertos_performance.h"

#define সংখ্যা_OF_ITERATIONS 1023

স্থির পূর্ণাঙ্ক compare_uint32(ধ্রুৱ শূন্য *a, ধ্রুৱ শূন্য *b)
{
    ঘূৰাই (*(uint32_t *)a - * (uint32_t *)b);
}

স্থির uint32_t calculate_median(uint32_t *মানসমূহ, পূর্ণাঙ্ক size)
{
    /* Sort the array of values containing the scheduling time of each iteration */
    qsort(মানসমূহ, size, পরিমাণ(uint32_t), compare_uint32);

    /* Return the median value. (size is an odd number) */
    ঘূৰাই মানসমূহ[size / 2];
}

প্রকারনির্দেশ সংরচনা {
    SemaphoreHandle_t end_sema;
    uint32_t before_sched;
    uint32_t cycles_to_sched[সংখ্যা_OF_ITERATIONS];
    TaskHandle_t t1_handle;
} test_context_t;

স্থির শূন্য test_task_1(শূন্য *arg)
{
    test_context_t *context = (test_context_t *)arg;

    বাবে (;;) {
        context->before_sched = esp_cpu_get_cycle_গুনতি();
        taskYIELD();
    }

    vTaskDelete(রিক্ত);
}

স্থির শূন্য test_task_2(শূন্য *arg)
{
    test_context_t *context = (test_context_t *)arg;

    vTaskPrioritySet(রিক্ত, CONFIG_UNITY_FREERTOS_PRIORITY + 1);
    vTaskPrioritySet(context->t1_handle, CONFIG_UNITY_FREERTOS_PRIORITY + 1);
    taskYIELD();

    বাবে (পূর্ণাঙ্ক i = 0; i < সংখ্যা_OF_ITERATIONS; i++) {
        context->cycles_to_sched[i] = esp_cpu_get_cycle_গুনতি() - context->before_sched;
        taskYIELD();
    }

    xSemaphoreGive(context->end_sema);
    vTaskDelete(context->t1_handle);
    vTaskDelete(রিক্ত);
}

TEST_CASE("scheduling time test", "[freertos]")
{
    test_context_t context;

    context.end_sema = x_Semaphore_Create_দ্বিমিক();
    TEST_ASSERT(context.end_sema != রিক্ত);

#যদি !CONFIG_FREERTOS_UNICORE
    xTaskCreatePinnedToCore(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, &context.t1_handle, 1);
    xTaskCreatePinnedToCore(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, রিক্ত, 1);
#অন্যথা
    xTaskCreatePinnedToCore(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, &context.t1_handle, 0);
    xTaskCreatePinnedToCore(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, রিক্ত, 0);
#endif

    BaseType_t result = xSemaphoreTake(context.end_sema, portMAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, result);

    /* We calculate the median instead of the average to avoid outliers which may occur due to cache speed variabilities */
    uint32_t median_cycles = calculate_median(context.cycles_to_sched, সংখ্যা_OF_ITERATIONS);
    TEST_PERFORMANCE_LESS_THAN(SCHEDULING_TIME, "%"PRIu32" cycles", median_cycles);

    /* Cleanup */
    vSemaphoreDelete(context.end_sema);
}
