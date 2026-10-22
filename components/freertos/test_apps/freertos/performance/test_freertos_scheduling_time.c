/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن <esp_types.h>
#ضمن <stdio.h>
#ضمن <inttypes.h>
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "freertos/semphr.h"
#ضمن "freertos/queue.h"
#ضمن "esp_intr_alloc.h"
#ضمن "esp_cpu.h"
#ضمن "unity.h"
#ضمن "test_utils.h"
#ضمن "freertos_performance.h"

#define رقم_OF_ITERATIONS 1023

ساكن صحيح compare_uint32(ثابت فراغ *a, ثابت فراغ *b)
{
    أرجع (*(uint32_t *)a - * (uint32_t *)b);
}

ساكن uint32_t calculate_median(uint32_t *قيم, صحيح size)
{
    /* Sort the array of values containing the scheduling time of each iteration */
    qsort(قيم, size, حجم(uint32_t), compare_uint32);

    /* Return the median value. (size is an odd number) */
    أرجع قيم[size / 2];
}

تعريفنوع بنية {
    SemaphoreHandle_t end_sema;
    uint32_t before_sched;
    uint32_t cycles_to_sched[رقم_OF_ITERATIONS];
    TaskHandle_t t1_handle;
} test_context_t;

ساكن فراغ test_task_1(فراغ *arg)
{
    test_context_t *context = (test_context_t *)arg;

    لكل (;;) {
        context->before_sched = esp_cpu_get_cycle_عدد();
        taskYIELD();
    }

    vTaskDelete(عدم);
}

ساكن فراغ test_task_2(فراغ *arg)
{
    test_context_t *context = (test_context_t *)arg;

    vTaskPrioritySet(عدم, CONFIG_UNITY_FREERTOS_PRIORITY + 1);
    vTaskPrioritySet(context->t1_handle, CONFIG_UNITY_FREERTOS_PRIORITY + 1);
    taskYIELD();

    لكل (صحيح i = 0; i < رقم_OF_ITERATIONS; i++) {
        context->cycles_to_sched[i] = esp_cpu_get_cycle_عدد() - context->before_sched;
        taskYIELD();
    }

    xSemaphoreGive(context->end_sema);
    vTaskDelete(context->t1_handle);
    vTaskDelete(عدم);
}

TEST_CASE("scheduling time test", "[freertos]")
{
    test_context_t context;

    context.end_sema = x_Semaphore_Create_ثنائي();
    TEST_ASSERT(context.end_sema != عدم);

#إذا !CONFIG_FREERTOS_UNICORE
    xTaskCreatePinnedToCore(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, &context.t1_handle, 1);
    xTaskCreatePinnedToCore(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, عدم, 1);
#وإلا
    xTaskCreatePinnedToCore(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, &context.t1_handle, 0);
    xTaskCreatePinnedToCore(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, عدم, 0);
#endif

    BaseType_t result = xSemaphoreTake(context.end_sema, portMAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, result);

    /* We calculate the median instead of the average to avoid outliers which may occur due to cache speed variabilities */
    uint32_t median_cycles = calculate_median(context.cycles_to_sched, رقم_OF_ITERATIONS);
    TEST_PERFORMANCE_LESS_THAN(SCHEDULING_TIME, "%"PRIu32" cycles", median_cycles);

    /* Cleanup */
    vSemaphoreDelete(context.end_sema);
}
