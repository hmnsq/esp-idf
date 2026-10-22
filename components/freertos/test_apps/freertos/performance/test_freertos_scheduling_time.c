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

#ವ್ಯಾಖ್ಯಾನಿಸು ಸಂಖ್ಯೆ_ದ_ITERATIONS 1023

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ compare_uint32(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *b)
{
    ಹಿಂತಿರುಗಿ (*(uint32_t *)a - * (uint32_t *)b);
}

ಸ್ಥಿರ uint32_t calculate_median(uint32_t *ಮೌಲ್ಯಗಳು, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2)
{
    /* Sort the array of values containing the scheduling time of each iteration */
    qsort(ಮೌಲ್ಯಗಳು, ಗಾತ್ರ_2, ಗಾತ್ರ(uint32_t), compare_uint32);

    /* Return the median value. (size is an odd number) */
    ಹಿಂತಿರುಗಿ ಮೌಲ್ಯಗಳು[ಗಾತ್ರ_2 / 2];
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    SemaphoreHandle_t ಕೊನೆ_sema;
    uint32_t before_sched;
    uint32_t ಚಕ್ರಗಳು_ಗಮ್ಯ_sched[ಸಂಖ್ಯೆ_ದ_ITERATIONS];
    TaskHandle_t t1_handle;
} test_context_t;

ಸ್ಥಿರ ಶೂನ್ಯ test_task_1(ಶೂನ್ಯ *arg)
{
    test_context_t *context = (test_context_t *)arg;

    ಗಾಗಿ (;;) {
        context->before_sched = esp_cpu_get_ಚಕ್ರ_ಎಣಿಕೆ();
        taskYIELD();
    }

    vTaskDelete(ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ test_task_2(ಶೂನ್ಯ *arg)
{
    test_context_t *context = (test_context_t *)arg;

    v_Task_ಆದ್ಯತೆ_ಗಣ(ಖಾಲಿ, CONFIG_UNITY_FREERTOS_ಆದ್ಯತೆ + 1);
    v_Task_ಆದ್ಯತೆ_ಗಣ(context->t1_handle, CONFIG_UNITY_FREERTOS_ಆದ್ಯತೆ + 1);
    taskYIELD();

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಸಂಖ್ಯೆ_ದ_ITERATIONS; i++) {
        context->ಚಕ್ರಗಳು_ಗಮ್ಯ_sched[i] = esp_cpu_get_ಚಕ್ರ_ಎಣಿಕೆ() - context->before_sched;
        taskYIELD();
    }

    xSemaphoreGive(context->ಕೊನೆ_sema);
    vTaskDelete(context->t1_handle);
    vTaskDelete(ಖಾಲಿ);
}

TEST_CASE("scheduling time test", "[freertos]")
{
    test_context_t context;

    context.ಕೊನೆ_sema = x_Semaphore_Create_ದ್ವಿಮಾನ();
    TEST_ASSERT(context.ಕೊನೆ_sema != ಖಾಲಿ);

#ಆದರೆ !CONFIG_FREERTOS_UNICORE
    x_Task_Create_Pinned_ಗಮ್ಯ_Core(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_ಆದ್ಯತೆ - 1, &context.t1_handle, 1);
    x_Task_Create_Pinned_ಗಮ್ಯ_Core(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_ಆದ್ಯತೆ - 1, ಖಾಲಿ, 1);
#ಇಲ್ಲದಿದ್ದರೆ
    x_Task_Create_Pinned_ಗಮ್ಯ_Core(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_ಆದ್ಯತೆ - 1, &context.t1_handle, 0);
    x_Task_Create_Pinned_ಗಮ್ಯ_Core(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_ಆದ್ಯತೆ - 1, ಖಾಲಿ, 0);
#ಆದರೆ_ಅಂತ್ಯ

    ತಳ_Type_t ಫಲಿತಾಂಶ = xSemaphoreTake(context.ಕೊನೆ_sema, ಪೋರ್ಟ್_MAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, ಫಲಿತಾಂಶ);

    /* We calculate the median instead of the average to avoid outliers which may occur due to cache speed variabilities */
    uint32_t median_ಚಕ್ರಗಳು = calculate_median(context.ಚಕ್ರಗಳು_ಗಮ್ಯ_sched, ಸಂಖ್ಯೆ_ದ_ITERATIONS);
    TEST_PERFORMANCE_LESS_THAN(SCHEDULING_ಸಮಯ, "%"PRIu32" cycles", median_ಚಕ್ರಗಳು);

    /* Cleanup */
    vSemaphoreDelete(context.ಕೊನೆ_sema);
}
