/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ <esp_types.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <inttypes.h>
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "freertos/semphr.h"
#ಅನ್ತರ್ಭಾವ "freertos/queue.h"
#ಅನ್ತರ್ಭಾವ "esp_intr_alloc.h"
#ಅನ್ತರ್ಭಾವ "esp_cpu.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "test_utils.h"
#ಅನ್ತರ್ಭಾವ "freertos_performance.h"

#define ಸಂಖ್ಯೆ_OF_ITERATIONS 1023

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ compare_uint32(ಧ್ರುವ ಶೂನ್ಯ *a, ಧ್ರುವ ಶೂನ್ಯ *b)
{
    ಪ್ರತ್ಯಾಗಮ (*(uint32_t *)a - * (uint32_t *)b);
}

ಸ್ಥಿರ uint32_t calculate_median(uint32_t *ಮೌಲ್ಯಗಳು, ಪೂರ್ಣಾಙ್ಕ size)
{
    /* Sort the array of values containing the scheduling time of each iteration */
    qsort(ಮೌಲ್ಯಗಳು, size, ಪರಿಮಾಣ(uint32_t), compare_uint32);

    /* Return the median value. (size is an odd number) */
    ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯಗಳು[size / 2];
}

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    SemaphoreHandle_t end_sema;
    uint32_t before_sched;
    uint32_t cycles_to_sched[ಸಂಖ್ಯೆ_OF_ITERATIONS];
    TaskHandle_t t1_handle;
} test_context_t;

ಸ್ಥಿರ ಶೂನ್ಯ test_task_1(ಶೂನ್ಯ *arg)
{
    test_context_t *context = (test_context_t *)arg;

    ಪ್ರತಿ (;;) {
        context->before_sched = esp_cpu_get_cycle_ಎಣಿಕೆ();
        taskYIELD();
    }

    vTaskDelete(ರಿಕ್ತ);
}

ಸ್ಥಿರ ಶೂನ್ಯ test_task_2(ಶೂನ್ಯ *arg)
{
    test_context_t *context = (test_context_t *)arg;

    vTaskPrioritySet(ರಿಕ್ತ, CONFIG_UNITY_FREERTOS_PRIORITY + 1);
    vTaskPrioritySet(context->t1_handle, CONFIG_UNITY_FREERTOS_PRIORITY + 1);
    taskYIELD();

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಸಂಖ್ಯೆ_OF_ITERATIONS; i++) {
        context->cycles_to_sched[i] = esp_cpu_get_cycle_ಎಣಿಕೆ() - context->before_sched;
        taskYIELD();
    }

    xSemaphoreGive(context->end_sema);
    vTaskDelete(context->t1_handle);
    vTaskDelete(ರಿಕ್ತ);
}

TEST_CASE("scheduling time test", "[freertos]")
{
    test_context_t context;

    context.end_sema = x_Semaphore_Create_ದ್ವಿಮಾನ();
    TEST_ASSERT(context.end_sema != ರಿಕ್ತ);

#ಯದಿ !CONFIG_FREERTOS_UNICORE
    xTaskCreatePinnedToCore(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, &context.t1_handle, 1);
    xTaskCreatePinnedToCore(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, ರಿಕ್ತ, 1);
#ಅನ್ಯಥಾ
    xTaskCreatePinnedToCore(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, &context.t1_handle, 0);
    xTaskCreatePinnedToCore(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, ರಿಕ್ತ, 0);
#endif

    BaseType_t result = xSemaphoreTake(context.end_sema, portMAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, result);

    /* We calculate the median instead of the average to avoid outliers which may occur due to cache speed variabilities */
    uint32_t median_cycles = calculate_median(context.cycles_to_sched, ಸಂಖ್ಯೆ_OF_ITERATIONS);
    TEST_PERFORMANCE_LESS_THAN(SCHEDULING_TIME, "%"PRIu32" cycles", median_cycles);

    /* Cleanup */
    vSemaphoreDelete(context.end_sema);
}
