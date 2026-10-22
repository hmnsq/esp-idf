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

#وصف_ڪريو عدد_جو_ITERATIONS 1023

ساكن صحيح compare_uint32(ثابت فراغ *a, ثابت فراغ *b)
{
    أرجع (*(uint32_t *)a - * (uint32_t *)b);
}

ساكن uint32_t calculate_median(uint32_t *قدرون, صحيح سائيز)
{
    /* Sort the array of values containing the scheduling time of each iteration */
    qsort(قدرون, سائيز, حجم(uint32_t), compare_uint32);

    /* Return the median value. (size is an odd number) */
    أرجع قدرون[سائيز / 2];
}

تعريفنوع بنية {
    SemaphoreHandle_t پڇاڙي_sema;
    uint32_t before_sched;
    uint32_t چڪرن_منزل_sched[عدد_جو_ITERATIONS];
    TaskHandle_t t1_handle;
} test_context_t;

ساكن فراغ test_task_1(فراغ *arg)
{
    test_context_t *context = (test_context_t *)arg;

    لكل (;;) {
        context->before_sched = esp_cpu_get_چڪر_ڳڻپ();
        taskYIELD();
    }

    vTaskDelete(عدم);
}

ساكن فراغ test_task_2(فراغ *arg)
{
    test_context_t *context = (test_context_t *)arg;

    v_Task_ترجيح_سيٽ(عدم, CONFIG_UNITY_FREERTOS_ترجيح + 1);
    v_Task_ترجيح_سيٽ(context->t1_handle, CONFIG_UNITY_FREERTOS_ترجيح + 1);
    taskYIELD();

    لكل (صحيح i = 0; i < عدد_جو_ITERATIONS; i++) {
        context->چڪرن_منزل_sched[i] = esp_cpu_get_چڪر_ڳڻپ() - context->before_sched;
        taskYIELD();
    }

    xSemaphoreGive(context->پڇاڙي_sema);
    vTaskDelete(context->t1_handle);
    vTaskDelete(عدم);
}

TEST_CASE("scheduling time test", "[freertos]")
{
    test_context_t context;

    context.پڇاڙي_sema = x_Semaphore_Create_ٻٽو();
    TEST_ASSERT(context.پڇاڙي_sema != عدم);

#إذا !CONFIG_FREERTOS_UNICORE
    x_Task_Create_Pinned_منزل_Core(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_ترجيح - 1, &context.t1_handle, 1);
    x_Task_Create_Pinned_منزل_Core(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_ترجيح - 1, عدم, 1);
#وإلا
    x_Task_Create_Pinned_منزل_Core(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_ترجيح - 1, &context.t1_handle, 0);
    x_Task_Create_Pinned_منزل_Core(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_ترجيح - 1, عدم, 0);
#جيڪڏهن_پورو

    بنياد_Type_t نتيجو = xSemaphoreTake(context.پڇاڙي_sema, پورٽ_MAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, نتيجو);

    /* We calculate the median instead of the average to avoid outliers which may occur due to cache speed variabilities */
    uint32_t median_چڪرن = calculate_median(context.چڪرن_منزل_sched, عدد_جو_ITERATIONS);
    TEST_PERFORMANCE_LESS_THAN(SCHEDULING_وقت, "%"PRIu32" cycles", median_چڪرن);

    /* Cleanup */
    vSemaphoreDelete(context.پڇاڙي_sema);
}
