/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ଅନ୍ତର୍ଭାଵ <esp_types.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <inttypes.h>
#ଅନ୍ତର୍ଭାଵ "freertos/FreeRTOS.h"
#ଅନ୍ତର୍ଭାଵ "freertos/task.h"
#ଅନ୍ତର୍ଭାଵ "freertos/semphr.h"
#ଅନ୍ତର୍ଭାଵ "freertos/queue.h"
#ଅନ୍ତର୍ଭାଵ "esp_intr_alloc.h"
#ଅନ୍ତର୍ଭାଵ "esp_cpu.h"
#ଅନ୍ତର୍ଭାଵ "unity.h"
#ଅନ୍ତର୍ଭାଵ "test_utils.h"
#ଅନ୍ତର୍ଭାଵ "freertos_performance.h"

#define ସଂଖ୍ୟା_OF_ITERATIONS 1023

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା compare_uint32(ଅଚଳ ଶୂନ୍ୟ *a, ଅଚଳ ଶୂନ୍ୟ *b)
{
    ଫେରନ୍ତୁ (*(uint32_t *)a - * (uint32_t *)b);
}

ସ୍ଥିର uint32_t calculate_median(uint32_t *ମୂଲ୍ୟଗୁଡ଼ିକ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size)
{
    /* Sort the array of values containing the scheduling time of each iteration */
    qsort(ମୂଲ୍ୟଗୁଡ଼ିକ, size, ଆକାର(uint32_t), compare_uint32);

    /* Return the median value. (size is an odd number) */
    ଫେରନ୍ତୁ ମୂଲ୍ୟଗୁଡ଼ିକ[size / 2];
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    SemaphoreHandle_t end_sema;
    uint32_t before_sched;
    uint32_t cycles_to_sched[ସଂଖ୍ୟା_OF_ITERATIONS];
    TaskHandle_t t1_handle;
} test_context_t;

ସ୍ଥିର ଶୂନ୍ୟ test_task_1(ଶୂନ୍ୟ *arg)
{
    test_context_t *context = (test_context_t *)arg;

    ପାଇଁ (;;) {
        context->before_sched = esp_cpu_get_cycle_ଗଣନା();
        taskYIELD();
    }

    vTaskDelete(ଖାଲି);
}

ସ୍ଥିର ଶୂନ୍ୟ test_task_2(ଶୂନ୍ୟ *arg)
{
    test_context_t *context = (test_context_t *)arg;

    vTaskPrioritySet(ଖାଲି, CONFIG_UNITY_FREERTOS_PRIORITY + 1);
    vTaskPrioritySet(context->t1_handle, CONFIG_UNITY_FREERTOS_PRIORITY + 1);
    taskYIELD();

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ସଂଖ୍ୟା_OF_ITERATIONS; i++) {
        context->cycles_to_sched[i] = esp_cpu_get_cycle_ଗଣନା() - context->before_sched;
        taskYIELD();
    }

    xSemaphoreGive(context->end_sema);
    vTaskDelete(context->t1_handle);
    vTaskDelete(ଖାଲି);
}

TEST_CASE("scheduling time test", "[freertos]")
{
    test_context_t context;

    context.end_sema = x_Semaphore_Create_ଦ୍ୱିମିକ();
    TEST_ASSERT(context.end_sema != ଖାଲି);

#ଯଦି !CONFIG_FREERTOS_UNICORE
    xTaskCreatePinnedToCore(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, &context.t1_handle, 1);
    xTaskCreatePinnedToCore(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, ଖାଲି, 1);
#ଅନ୍ୟଥା
    xTaskCreatePinnedToCore(test_task_1, "test1", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, &context.t1_handle, 0);
    xTaskCreatePinnedToCore(test_task_2, "test2", 4096, &context, CONFIG_UNITY_FREERTOS_PRIORITY - 1, ଖାଲି, 0);
#endif

    BaseType_t result = xSemaphoreTake(context.end_sema, portMAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, result);

    /* We calculate the median instead of the average to avoid outliers which may occur due to cache speed variabilities */
    uint32_t median_cycles = calculate_median(context.cycles_to_sched, ସଂଖ୍ୟା_OF_ITERATIONS);
    TEST_PERFORMANCE_LESS_THAN(SCHEDULING_TIME, "%"PRIu32" cycles", median_cycles);

    /* Cleanup */
    vSemaphoreDelete(context.end_sema);
}
