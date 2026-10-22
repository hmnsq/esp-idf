/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#অন্তর্ভাব <esp_types.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <inttypes.h>
#অন্তর্ভাব "freertos/FreeRTOS.h"
#অন্তর্ভাব "freertos/task.h"
#অন্তর্ভাব "freertos/semphr.h"
#অন্তর্ভাব "freertos/queue.h"
#অন্তর্ভাব "esp_intr_alloc.h"
#অন্তর্ভাব "esp_cpu.h"
#অন্তর্ভাব "unity.h"
#অন্তর্ভাব "test_utils.h"
#অন্তর্ভাব "freertos_performance.h"

#define সংখ্যা_OF_ITERATIONS 1023

স্থির পূর্ণসংখ্যা compare_uint32(ধ্রুবক শূন্য *a, ধ্রুবক শূন্য *b)
{
    ফেরত (*(uint32_t *)a - * (uint32_t *)b);
}

স্থির uint32_t calculate_median(uint32_t *মানসমূহ, পূর্ণসংখ্যা আকার_2)
{
    /* Sort the array of values containing the scheduling time of each iteration */
    qsort(মানসমূহ, আকার_2, আকার(uint32_t), compare_uint32);

    /* Return the median value. (size is an odd number) */
    ফেরত মানসমূহ[আকার_2 / 2];
}

প্রকারলেখ কাঠামো {
    SemaphoreHandle_t end_sema;
    uint32_t before_sched;
    uint32_t cycles_to_sched[সংখ্যা_OF_ITERATIONS];
    TaskHandle_t t1_handle;
} test_context_t;

স্থির শূন্য test_task_1(শূন্য *arg)
{
    test_context_t *context = (test_context_t *)arg;

    জন্য (;;) {
        context->before_sched = esp_cpu_get_cycle_গুনতি();
        taskYIELD();
    }

    vTaskDelete(ফাঁকা);
}

স্থির শূন্য test_task_2(শূন্য *arg)
{
    test_context_t *context = (test_context_t *)arg;

    vTaskPrioritySet(ফাঁকা, CONFIG_UNITY_FREERTOS_PRIORITY + 1);
    vTaskPrioritySet(context->t1_handle, CONFIG_UNITY_FREERTOS_PRIORITY + 1);
    taskYIELD();

    জন্য (পূর্ণসংখ্যা i = 0; i < সংখ্যা_OF_ITERATIONS; i++) {
        context->cycles_to_sched[i] = esp_cpu_get_cycle_গুনতি() - context->before_sched;
        taskYIELD();
    }

    xSemaphoreGive(context->end_sema);
    vTaskDelete(context->t1_handle);
    vTaskDelete(ফাঁকা);
}

TEST_CASE("scheduling time test", "[freertos]")
{
    test_context_t context;

    context.end_sema = x_Semaphore_Create_দ্বিমিক();
    TEST_ASSERT(context.end_sema != ফাঁকা);

#যদি !CONFIG_FREERTOS_UNICORE
    xTaskCreatePinnedToCore(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, &context.t1_handle, 1);
    xTaskCreatePinnedToCore(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, ফাঁকা, 1);
#অন্যথা
    xTaskCreatePinnedToCore(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, &context.t1_handle, 0);
    xTaskCreatePinnedToCore(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, ফাঁকা, 0);
#endif

    BaseType_t ফলাফল = xSemaphoreTake(context.end_sema, portMAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, ফলাফল);

    /* We calculate the median instead of the average to avoid outliers which may occur due to cache speed variabilities */
    uint32_t median_cycles = calculate_median(context.cycles_to_sched, সংখ্যা_OF_ITERATIONS);
    TEST_PERFORMANCE_LESS_THAN(SCHEDULING_TIME, "%"PRIu32" cycles", median_cycles);

    /* Cleanup */
    vSemaphoreDelete(context.end_sema);
}
