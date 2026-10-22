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

#परिभाषय सङ्ख्या_सम्बन्धि_ITERATIONS 1023

स्थिर पूर्णाङ्क compare_uint32(ध्रुव शून्य *a, ध्रुव शून्य *b)
{
    प्रत्यागम (*(uint32_t *)a - * (uint32_t *)b);
}

स्थिर uint32_t calculate_median(uint32_t *मानानि, पूर्णाङ्क आकार)
{
    /* Sort the array of values containing the scheduling time of each iteration */
    शीघ्रं_क्रमय(मानानि, आकार, परिमाण(uint32_t), compare_uint32);

    /* Return the median value. (size is an odd number) */
    प्रत्यागम मानानि[आकार / 2];
}

प्रकारनिर्देश संरचना {
    SemaphoreHandle_t अन्तः_sema;
    uint32_t before_sched;
    uint32_t आवृत्तयः_गन्तव्यम्_sched[सङ्ख्या_सम्बन्धि_ITERATIONS];
    TaskHandle_t t1_handle;
} test_context_t;

स्थिर शून्य test_task_1(शून्य *arg)
{
    test_context_t *context = (test_context_t *)arg;

    प्रति (;;) {
        context->before_sched = esp_cpu_get_आवृत्तिः_गणनम्();
        taskYIELD();
    }

    vTaskDelete(रिक्त);
}

स्थिर शून्य test_task_2(शून्य *arg)
{
    test_context_t *context = (test_context_t *)arg;

    v_Task_प्राथमिकता_समुच्चयः(रिक्त, CONFIG_UNITY_FREERTOS_प्राथमिकता + 1);
    v_Task_प्राथमिकता_समुच्चयः(context->t1_handle, CONFIG_UNITY_FREERTOS_प्राथमिकता + 1);
    taskYIELD();

    प्रति (पूर्णाङ्क i = 0; i < सङ्ख्या_सम्बन्धि_ITERATIONS; i++) {
        context->आवृत्तयः_गन्तव्यम्_sched[i] = esp_cpu_get_आवृत्तिः_गणनम्() - context->before_sched;
        taskYIELD();
    }

    xSemaphoreGive(context->अन्तः_sema);
    vTaskDelete(context->t1_handle);
    vTaskDelete(रिक्त);
}

TEST_CASE("scheduling time test", "[freertos]")
{
    test_context_t context;

    context.अन्तः_sema = x_Semaphore_Create_द्विमान();
    TEST_ASSERT(context.अन्तः_sema != रिक्त);

#यदि !CONFIG_FREERTOS_UNICORE
    x_Task_Create_Pinned_गन्तव्यम्_Core(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_प्राथमिकता - 1, &context.t1_handle, 1);
    x_Task_Create_Pinned_गन्तव्यम्_Core(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_प्राथमिकता - 1, रिक्त, 1);
#अन्यथा
    x_Task_Create_Pinned_गन्तव्यम्_Core(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_प्राथमिकता - 1, &context.t1_handle, 0);
    x_Task_Create_Pinned_गन्तव्यम्_Core(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_प्राथमिकता - 1, रिक्त, 0);
#यदि_अन्तः

    आधारः_Type_t परिणाम = xSemaphoreTake(context.अन्तः_sema, पत्तनम्_MAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, परिणाम);

    /* We calculate the median instead of the average to avoid outliers which may occur due to cache speed variabilities */
    uint32_t median_आवृत्तयः = calculate_median(context.आवृत्तयः_गन्तव्यम्_sched, सङ्ख्या_सम्बन्धि_ITERATIONS);
    TEST_PERFORMANCE_LESS_THAN(SCHEDULING_TIME, "%"PRIu32" cycles", median_आवृत्तयः);

    /* Cleanup */
    vSemaphoreDelete(context.अन्तः_sema);
}
