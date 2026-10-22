/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#अन्तर्भाव <esp_types.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <inttypes.h>
#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"
#अन्तर्भाव "freertos/semphr.h"
#अन्तर्भाव "freertos/queue.h"
#अन्तर्भाव "esp_intr_alloc.h"
#अन्तर्भाव "esp_cpu.h"
#अन्तर्भाव "unity.h"
#अन्तर्भाव "test_utils.h"
#अन्तर्भाव "freertos_performance.h"

#define संख्या_OF_ITERATIONS 1023

स्थिर पूर्णांक compare_uint32(स्थिरांक शून्य *a, स्थिरांक शून्य *b)
{
    परत (*(uint32_t *)a - * (uint32_t *)b);
}

स्थिर uint32_t calculate_median(uint32_t *मूल्ये, पूर्णांक size)
{
    /* Sort the array of values containing the scheduling time of each iteration */
    क्विक(मूल्ये, size, आकार(uint32_t), compare_uint32);

    /* Return the median value. (size is an odd number) */
    परत मूल्ये[size / 2];
}

प्रकारव्याख्या संरचना {
    SemaphoreHandle_t end_sema;
    uint32_t before_sched;
    uint32_t cycles_to_sched[संख्या_OF_ITERATIONS];
    TaskHandle_t t1_handle;
} test_context_t;

स्थिर शून्य test_task_1(शून्य *arg)
{
    test_context_t *context = (test_context_t *)arg;

    साठी (;;) {
        context->before_sched = esp_cpu_get_cycle_मोजणी();
        taskYIELD();
    }

    vTaskDelete(रिक्त);
}

स्थिर शून्य test_task_2(शून्य *arg)
{
    test_context_t *context = (test_context_t *)arg;

    vTaskPrioritySet(रिक्त, CONFIG_UNITY_FREERTOS_PRIORITY + 1);
    vTaskPrioritySet(context->t1_handle, CONFIG_UNITY_FREERTOS_PRIORITY + 1);
    taskYIELD();

    साठी (पूर्णांक i = 0; i < संख्या_OF_ITERATIONS; i++) {
        context->cycles_to_sched[i] = esp_cpu_get_cycle_मोजणी() - context->before_sched;
        taskYIELD();
    }

    xSemaphoreGive(context->end_sema);
    vTaskDelete(context->t1_handle);
    vTaskDelete(रिक्त);
}

TEST_CASE("scheduling time test", "[freertos]")
{
    test_context_t context;

    context.end_sema = x_Semaphore_Create_द्विमान();
    TEST_ASSERT(context.end_sema != रिक्त);

#जर !CONFIG_FREERTOS_UNICORE
    xTaskCreatePinnedToCore(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, &context.t1_handle, 1);
    xTaskCreatePinnedToCore(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, रिक्त, 1);
#अन्यथा
    xTaskCreatePinnedToCore(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, &context.t1_handle, 0);
    xTaskCreatePinnedToCore(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, रिक्त, 0);
#endif

    BaseType_t result = xSemaphoreTake(context.end_sema, portMAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, result);

    /* We calculate the median instead of the average to avoid outliers which may occur due to cache speed variabilities */
    uint32_t median_cycles = calculate_median(context.cycles_to_sched, संख्या_OF_ITERATIONS);
    TEST_PERFORMANCE_LESS_THAN(SCHEDULING_TIME, "%"PRIu32" cycles", median_cycles);

    /* Cleanup */
    vSemaphoreDelete(context.end_sema);
}
