/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ <esp_types.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <inttypes.h>
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "freertos/semphr.h"
#అన్తర్భావ "freertos/queue.h"
#అన్తర్భావ "esp_intr_alloc.h"
#అన్తర్భావ "esp_cpu.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "test_utils.h"
#అన్తర్భావ "freertos_performance.h"

#నిర్వచించు సంఖ్య_యొక్క_ITERATIONS 1023

స్థిర పూర్ణాంకం compare_uint32(స్థిరరాశి శూన్య *a, స్థిరరాశి శూన్య *b)
{
    తిరిగి (*(uint32_t *)a - * (uint32_t *)b);
}

స్థిర uint32_t calculate_median(uint32_t *విలువలు, పూర్ణాంకం కొలత)
{
    /* Sort the array of values containing the scheduling time of each iteration */
    qsort(విలువలు, కొలత, పరిమాణం(uint32_t), compare_uint32);

    /* Return the median value. (size is an odd number) */
    తిరిగి విలువలు[కొలత / 2];
}

రకనిర్వచనం నిర్మాణం {
    SemaphoreHandle_t చివర_sema;
    uint32_t before_sched;
    uint32_t చక్రాలు_గమ్యం_sched[సంఖ్య_యొక్క_ITERATIONS];
    TaskHandle_t t1_handle;
} test_context_t;

స్థిర శూన్య test_task_1(శూన్య *arg)
{
    test_context_t *context = (test_context_t *)arg;

    కొరకు (;;) {
        context->before_sched = esp_cpu_get_చక్రం_లెక్క();
        taskYIELD();
    }

    vTaskDelete(ఖాళీ);
}

స్థిర శూన్య test_task_2(శూన్య *arg)
{
    test_context_t *context = (test_context_t *)arg;

    v_Task_ప్రాధాన్యత_సమితి(ఖాళీ, CONFIG_UNITY_FREERTOS_ప్రాధాన్యత + 1);
    v_Task_ప్రాధాన్యత_సమితి(context->t1_handle, CONFIG_UNITY_FREERTOS_ప్రాధాన్యత + 1);
    taskYIELD();

    కొరకు (పూర్ణాంకం i = 0; i < సంఖ్య_యొక్క_ITERATIONS; i++) {
        context->చక్రాలు_గమ్యం_sched[i] = esp_cpu_get_చక్రం_లెక్క() - context->before_sched;
        taskYIELD();
    }

    xSemaphoreGive(context->చివర_sema);
    vTaskDelete(context->t1_handle);
    vTaskDelete(ఖాళీ);
}

TEST_CASE("scheduling time test", "[freertos]")
{
    test_context_t context;

    context.చివర_sema = x_Semaphore_Create_ద్విమాన();
    TEST_ASSERT(context.చివర_sema != ఖాళీ);

#ఒకవేళ !CONFIG_FREERTOS_UNICORE
    x_Task_Create_Pinned_గమ్యం_Core(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_ప్రాధాన్యత - 1, &context.t1_handle, 1);
    x_Task_Create_Pinned_గమ్యం_Core(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_ప్రాధాన్యత - 1, ఖాళీ, 1);
#లేకపోతే
    x_Task_Create_Pinned_గమ్యం_Core(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_ప్రాధాన్యత - 1, &context.t1_handle, 0);
    x_Task_Create_Pinned_గమ్యం_Core(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_ప్రాధాన్యత - 1, ఖాళీ, 0);
#అయితే_ముగింపు

    పునాది_Type_t ఫలితం = xSemaphoreTake(context.చివర_sema, పోర్టు_MAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, ఫలితం);

    /* We calculate the median instead of the average to avoid outliers which may occur due to cache speed variabilities */
    uint32_t median_చక్రాలు = calculate_median(context.చక్రాలు_గమ్యం_sched, సంఖ్య_యొక్క_ITERATIONS);
    TEST_PERFORMANCE_LESS_THAN(SCHEDULING_సమయం, "%"PRIu32" cycles", median_చక్రాలు);

    /* Cleanup */
    vSemaphoreDelete(context.చివర_sema);
}
