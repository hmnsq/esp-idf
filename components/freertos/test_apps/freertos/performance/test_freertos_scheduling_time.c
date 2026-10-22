/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#समावेश <esp_types.h>
#समावेश <stdio.h>
#समावेश <inttypes.h>
#समावेश "freertos/FreeRTOS.h"
#समावेश "freertos/task.h"
#समावेश "freertos/semphr.h"
#समावेश "freertos/queue.h"
#समावेश "esp_intr_alloc.h"
#समावेश "esp_cpu.h"
#समावेश "unity.h"
#समावेश "test_utils.h"
#समावेश "freertos_performance.h"

#घोषणा संख्या_OF_ITERATIONS 1023

अटल पूर्णांक compare_uint32(स्थिर व्योम *a, स्थिर व्योम *b)
{
    वापस (*(uint32_t *)a - * (uint32_t *)b);
}

अटल uint32_t calculate_median(uint32_t *मूल्यावली, पूर्णांक आकार)
{
    /* Sort the array of values containing the scheduling time of each iteration */
    क्विक(मूल्यावली, आकार, माप(uint32_t), compare_uint32);

    /* Return the median value. (size is an odd number) */
    वापस मूल्यावली[आकार / 2];
}

प्रकार काष्ठा {
    SemaphoreHandle_t end_sema;
    uint32_t before_sched;
    uint32_t cycles_to_sched[संख्या_OF_ITERATIONS];
    TaskHandle_t t1_handle;
} test_context_t;

अटल व्योम test_task_1(व्योम *arg)
{
    test_context_t *context = (test_context_t *)arg;

    क्रम (;;) {
        context->before_sched = esp_cpu_get_cycle_गिनती();
        taskYIELD();
    }

    vTaskDelete(शून्य);
}

अटल व्योम test_task_2(व्योम *arg)
{
    test_context_t *context = (test_context_t *)arg;

    vTaskPrioritySet(शून्य, CONFIG_UNITY_FREERTOS_PRIORITY + 1);
    vTaskPrioritySet(context->t1_handle, CONFIG_UNITY_FREERTOS_PRIORITY + 1);
    taskYIELD();

    क्रम (पूर्णांक i = 0; i < संख्या_OF_ITERATIONS; i++) {
        context->cycles_to_sched[i] = esp_cpu_get_cycle_गिनती() - context->before_sched;
        taskYIELD();
    }

    xSemaphoreGive(context->end_sema);
    vTaskDelete(context->t1_handle);
    vTaskDelete(शून्य);
}

TEST_CASE("scheduling time test", "[freertos]")
{
    test_context_t context;

    context.end_sema = x_Semaphore_Create_द्विआधारी();
    TEST_ASSERT(context.end_sema != शून्य);

#अगर !CONFIG_FREERTOS_UNICORE
    xTaskCreatePinnedToCore(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, &context.t1_handle, 1);
    xTaskCreatePinnedToCore(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, शून्य, 1);
#अन्यथा
    xTaskCreatePinnedToCore(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, &context.t1_handle, 0);
    xTaskCreatePinnedToCore(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, शून्य, 0);
#पूर्ण_अगर

    BaseType_t परिणाम = xSemaphoreTake(context.end_sema, portMAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, परिणाम);

    /* We calculate the median instead of the average to avoid outliers which may occur due to cache speed variabilities */
    uint32_t median_cycles = calculate_median(context.cycles_to_sched, संख्या_OF_ITERATIONS);
    TEST_PERFORMANCE_LESS_THAN(SCHEDULING_TIME, "%"PRIu32" cycles", median_cycles);

    /* Cleanup */
    vSemaphoreDelete(context.end_sema);
}
