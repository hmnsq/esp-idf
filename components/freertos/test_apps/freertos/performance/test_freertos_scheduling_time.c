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

#تعریف_کړه شمېره_د_ITERATIONS 1023

ساكن صحيح compare_uint32(ثابت فراغ *a, ثابت فراغ *b)
{
    أرجع (*(uint32_t *)a - * (uint32_t *)b);
}

ساكن uint32_t calculate_median(uint32_t *ارزښتونه, صحيح اندازه)
{
    /* Sort the array of values containing the scheduling time of each iteration */
    qsort(ارزښتونه, اندازه, حجم(uint32_t), compare_uint32);

    /* Return the median value. (size is an odd number) */
    أرجع ارزښتونه[اندازه / 2];
}

تعريفنوع بنية {
    SemaphoreHandle_t پای_ټکی_sema;
    uint32_t before_sched;
    uint32_t دورې_منزل_sched[شمېره_د_ITERATIONS];
    TaskHandle_t t1_handle;
} test_context_t;

ساكن فراغ test_task_1(فراغ *arg)
{
    test_context_t *context = (test_context_t *)arg;

    لكل (;;) {
        context->before_sched = esp_cpu_get_دوره_شمېر();
        taskYIELD();
    }

    vTaskDelete(عدم);
}

ساكن فراغ test_task_2(فراغ *arg)
{
    test_context_t *context = (test_context_t *)arg;

    v_Task_لومړیتوب_سېټ(عدم, CONFIG_UNITY_FREERTOS_لومړیتوب + 1);
    v_Task_لومړیتوب_سېټ(context->t1_handle, CONFIG_UNITY_FREERTOS_لومړیتوب + 1);
    taskYIELD();

    لكل (صحيح i = 0; i < شمېره_د_ITERATIONS; i++) {
        context->دورې_منزل_sched[i] = esp_cpu_get_دوره_شمېر() - context->before_sched;
        taskYIELD();
    }

    xSemaphoreGive(context->پای_ټکی_sema);
    vTaskDelete(context->t1_handle);
    vTaskDelete(عدم);
}

TEST_CASE("scheduling time test", "[freertos]")
{
    test_context_t context;

    context.پای_ټکی_sema = x_Semaphore_Create_دوګونی();
    TEST_ASSERT(context.پای_ټکی_sema != عدم);

#إذا !CONFIG_FREERTOS_UNICORE
    x_Task_Create_Pinned_منزل_Core(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_لومړیتوب - 1, &context.t1_handle, 1);
    x_Task_Create_Pinned_منزل_Core(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_لومړیتوب - 1, عدم, 1);
#وإلا
    x_Task_Create_Pinned_منزل_Core(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_لومړیتوب - 1, &context.t1_handle, 0);
    x_Task_Create_Pinned_منزل_Core(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_لومړیتوب - 1, عدم, 0);
#د_که_پای

    بنسټ_Type_t پایله = xSemaphoreTake(context.پای_ټکی_sema, پورټ_MAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, پایله);

    /* We calculate the median instead of the average to avoid outliers which may occur due to cache speed variabilities */
    uint32_t median_دورې = calculate_median(context.دورې_منزل_sched, شمېره_د_ITERATIONS);
    TEST_PERFORMANCE_LESS_THAN(SCHEDULING_وخت, "%"PRIu32" cycles", median_دورې);

    /* Cleanup */
    vSemaphoreDelete(context.پای_ټکی_sema);
}
