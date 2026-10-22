/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#അന്തര്ഭാവ <esp_types.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <inttypes.h>
#അന്തര്ഭാവ "freertos/FreeRTOS.h"
#അന്തര്ഭാവ "freertos/task.h"
#അന്തര്ഭാവ "freertos/semphr.h"
#അന്തര്ഭാവ "freertos/queue.h"
#അന്തര്ഭാവ "esp_intr_alloc.h"
#അന്തര്ഭാവ "esp_cpu.h"
#അന്തര്ഭാവ "unity.h"
#അന്തര്ഭാവ "test_utils.h"
#അന്തര്ഭാവ "freertos_performance.h"

#define സംഖ്യ_OF_ITERATIONS 1023

സ്ഥിതം പൂർണ്ണസംഖ്യ compare_uint32(സ്ഥിരം ശൂന്യം *a, സ്ഥിരം ശൂന്യം *b)
{
    തിരികെ (*(uint32_t *)a - * (uint32_t *)b);
}

സ്ഥിതം uint32_t calculate_median(uint32_t *മൂല്യങ്ങൾ, പൂർണ്ണസംഖ്യ size)
{
    /* Sort the array of values containing the scheduling time of each iteration */
    qsort(മൂല്യങ്ങൾ, size, വലുപ്പം(uint32_t), compare_uint32);

    /* Return the median value. (size is an odd number) */
    തിരികെ മൂല്യങ്ങൾ[size / 2];
}

തരനിർവചനം ഘടന {
    SemaphoreHandle_t end_sema;
    uint32_t before_sched;
    uint32_t cycles_to_sched[സംഖ്യ_OF_ITERATIONS];
    TaskHandle_t t1_handle;
} test_context_t;

സ്ഥിതം ശൂന്യം test_task_1(ശൂന്യം *arg)
{
    test_context_t *context = (test_context_t *)arg;

    വേണ്ടി (;;) {
        context->before_sched = esp_cpu_get_cycle_എണ്ണം();
        taskYIELD();
    }

    vTaskDelete(ശൂന്യ);
}

സ്ഥിതം ശൂന്യം test_task_2(ശൂന്യം *arg)
{
    test_context_t *context = (test_context_t *)arg;

    vTaskPrioritySet(ശൂന്യ, CONFIG_UNITY_FREERTOS_PRIORITY + 1);
    vTaskPrioritySet(context->t1_handle, CONFIG_UNITY_FREERTOS_PRIORITY + 1);
    taskYIELD();

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < സംഖ്യ_OF_ITERATIONS; i++) {
        context->cycles_to_sched[i] = esp_cpu_get_cycle_എണ്ണം() - context->before_sched;
        taskYIELD();
    }

    xSemaphoreGive(context->end_sema);
    vTaskDelete(context->t1_handle);
    vTaskDelete(ശൂന്യ);
}

TEST_CASE("scheduling time test", "[freertos]")
{
    test_context_t context;

    context.end_sema = x_Semaphore_Create_ദ്വിമാന();
    TEST_ASSERT(context.end_sema != ശൂന്യ);

#എങ്കിൽ !CONFIG_FREERTOS_UNICORE
    xTaskCreatePinnedToCore(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, &context.t1_handle, 1);
    xTaskCreatePinnedToCore(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, ശൂന്യ, 1);
#അല്ലെങ്കിൽ
    xTaskCreatePinnedToCore(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, &context.t1_handle, 0);
    xTaskCreatePinnedToCore(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, ശൂന്യ, 0);
#endif

    BaseType_t result = xSemaphoreTake(context.end_sema, portMAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, result);

    /* We calculate the median instead of the average to avoid outliers which may occur due to cache speed variabilities */
    uint32_t median_cycles = calculate_median(context.cycles_to_sched, സംഖ്യ_OF_ITERATIONS);
    TEST_PERFORMANCE_LESS_THAN(SCHEDULING_TIME, "%"PRIu32" cycles", median_cycles);

    /* Cleanup */
    vSemaphoreDelete(context.end_sema);
}
