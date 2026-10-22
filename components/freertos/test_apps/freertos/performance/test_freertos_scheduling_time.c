/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#அந்தர்பாவ <esp_types.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <inttypes.h>
#அந்தர்பாவ "freertos/FreeRTOS.h"
#அந்தர்பாவ "freertos/task.h"
#அந்தர்பாவ "freertos/semphr.h"
#அந்தர்பாவ "freertos/queue.h"
#அந்தர்பாவ "esp_intr_alloc.h"
#அந்தர்பாவ "esp_cpu.h"
#அந்தர்பாவ "unity.h"
#அந்தர்பாவ "test_utils.h"
#அந்தர்பாவ "freertos_performance.h"

#define எண்_OF_ITERATIONS 1023

நிலையான முழு compare_uint32(மாறிலி வெறுமை *a, மாறிலி வெறுமை *b)
{
    திரும்பு (*(uint32_t *)a - * (uint32_t *)b);
}

நிலையான uint32_t calculate_median(uint32_t *மதிப்புகள், முழு size)
{
    /* Sort the array of values containing the scheduling time of each iteration */
    qsort(மதிப்புகள், size, அளவு(uint32_t), compare_uint32);

    /* Return the median value. (size is an odd number) */
    திரும்பு மதிப்புகள்[size / 2];
}

வகைவரை கட்டமைப்பு {
    SemaphoreHandle_t end_sema;
    uint32_t before_sched;
    uint32_t cycles_to_sched[எண்_OF_ITERATIONS];
    TaskHandle_t t1_handle;
} test_context_t;

நிலையான வெறுமை test_task_1(வெறுமை *arg)
{
    test_context_t *context = (test_context_t *)arg;

    க்கு (;;) {
        context->before_sched = esp_cpu_get_cycle_மொத்தம்();
        taskYIELD();
    }

    vTaskDelete(வெற்று);
}

நிலையான வெறுமை test_task_2(வெறுமை *arg)
{
    test_context_t *context = (test_context_t *)arg;

    vTaskPrioritySet(வெற்று, CONFIG_UNITY_FREERTOS_PRIORITY + 1);
    vTaskPrioritySet(context->t1_handle, CONFIG_UNITY_FREERTOS_PRIORITY + 1);
    taskYIELD();

    க்கு (முழு i = 0; i < எண்_OF_ITERATIONS; i++) {
        context->cycles_to_sched[i] = esp_cpu_get_cycle_மொத்தம்() - context->before_sched;
        taskYIELD();
    }

    xSemaphoreGive(context->end_sema);
    vTaskDelete(context->t1_handle);
    vTaskDelete(வெற்று);
}

TEST_CASE("scheduling time test", "[freertos]")
{
    test_context_t context;

    context.end_sema = x_Semaphore_Create_இருமம்();
    TEST_ASSERT(context.end_sema != வெற்று);

#எனில் !CONFIG_FREERTOS_UNICORE
    xTaskCreatePinnedToCore(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, &context.t1_handle, 1);
    xTaskCreatePinnedToCore(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, வெற்று, 1);
#இல்லையெனில்
    xTaskCreatePinnedToCore(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, &context.t1_handle, 0);
    xTaskCreatePinnedToCore(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, வெற்று, 0);
#endif

    BaseType_t result = xSemaphoreTake(context.end_sema, portMAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, result);

    /* We calculate the median instead of the average to avoid outliers which may occur due to cache speed variabilities */
    uint32_t median_cycles = calculate_median(context.cycles_to_sched, எண்_OF_ITERATIONS);
    TEST_PERFORMANCE_LESS_THAN(SCHEDULING_TIME, "%"PRIu32" cycles", median_cycles);

    /* Cleanup */
    vSemaphoreDelete(context.end_sema);
}
