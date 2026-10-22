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

#تعریف_کرو نمبر_کا_ITERATIONS 1023

ساکن صحیح compare_uint32(ثابت خالی *a, ثابت خالی *b)
{
    واپس (*(uint32_t *)a - * (uint32_t *)b);
}

ساکن uint32_t calculate_median(uint32_t *اقدار, صحیح سائز)
{
    /* Sort the array of values containing the scheduling time of each iteration */
    qsort(اقدار, سائز, حجم(uint32_t), compare_uint32);

    /* Return the median value. (size is an odd number) */
    واپس اقدار[سائز / 2];
}

قسمتعریف ساخت {
    SemaphoreHandle_t انتہا_sema;
    uint32_t before_sched;
    uint32_t ادوار_مقصد_sched[نمبر_کا_ITERATIONS];
    TaskHandle_t t1_handle;
} test_context_t;

ساکن خالی test_task_1(خالی *arg)
{
    test_context_t *context = (test_context_t *)arg;

    برائے (;;) {
        context->before_sched = esp_cpu_get_چکر_گنتی();
        taskYIELD();
    }

    vTaskDelete(تہی);
}

ساکن خالی test_task_2(خالی *arg)
{
    test_context_t *context = (test_context_t *)arg;

    v_Task_ترجیح_سیٹ(تہی, CONFIG_UNITY_FREERTOS_ترجیح + 1);
    v_Task_ترجیح_سیٹ(context->t1_handle, CONFIG_UNITY_FREERTOS_ترجیح + 1);
    taskYIELD();

    برائے (صحیح i = 0; i < نمبر_کا_ITERATIONS; i++) {
        context->ادوار_مقصد_sched[i] = esp_cpu_get_چکر_گنتی() - context->before_sched;
        taskYIELD();
    }

    xSemaphoreGive(context->انتہا_sema);
    vTaskDelete(context->t1_handle);
    vTaskDelete(تہی);
}

TEST_CASE("scheduling time test", "[freertos]")
{
    test_context_t context;

    context.انتہا_sema = x_Semaphore_Create_ثنائی();
    TEST_ASSERT(context.انتہا_sema != تہی);

#اگر !CONFIG_FREERTOS_UNICORE
    x_Task_Create_Pinned_مقصد_Core(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_ترجیح - 1, &context.t1_handle, 1);
    x_Task_Create_Pinned_مقصد_Core(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_ترجیح - 1, تہی, 1);
#ورنہ
    x_Task_Create_Pinned_مقصد_Core(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_ترجیح - 1, &context.t1_handle, 0);
    x_Task_Create_Pinned_مقصد_Core(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_ترجیح - 1, تہی, 0);
#اگر_ختم

    بنیاد_Type_t نتیجہ = xSemaphoreTake(context.انتہا_sema, پورٹ_MAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, نتیجہ);

    /* We calculate the median instead of the average to avoid outliers which may occur due to cache speed variabilities */
    uint32_t median_ادوار = calculate_median(context.ادوار_مقصد_sched, نمبر_کا_ITERATIONS);
    TEST_PERFORMANCE_LESS_THAN(SCHEDULING_وقت, "%"PRIu32" cycles", median_ادوار);

    /* Cleanup */
    vSemaphoreDelete(context.انتہا_sema);
}
