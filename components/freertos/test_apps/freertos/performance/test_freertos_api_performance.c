/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * This test is a baseline test to calculate the performance of some of the frequently used FreeRTOS APIs.
 * It measures the execution time (in CPU cycles) of various FreeRTOS functions to help developers
 * understand the performance characteristics of different memory placement configurations.
 *
 * Usage:
 * Users can add other functions as desired by following the same measurement pattern.
 * Users can experiment with different memory placement configurations such as CONFIG_FREERTOS_IN_IRAM.
 *
 * Performance Factors:
 * Performance depends on several factors such as:
 * - Flash cache patterns and hit rates
 * - Frequency of API usage in the application
 * - Compiler optimizations and code generation
 * - System load and interrupt frequency
 * - Flash memory speed and timing
 *
 * Results Interpretation:
 * - Lower cycle counts indicate better performance
 * - Consider memory cost vs performance benefit trade-offs
 */

#ضمن <esp_types.h>
#ضمن <stdio.h>
#ضمن <inttypes.h>
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "freertos/semphr.h"
#ضمن "freertos/queue.h"
#ضمن "freertos/event_groups.h"
#ضمن "freertos/stream_buffer.h"
#ضمن "freertos/timers.h"
#ضمن "esp_cpu.h"
#ضمن "unity.h"
#ضمن "test_utils.h"

#تعریف_کن عدد_از_SAMPLES 1000

ساكن صحيح compare_uint32(ثابت فراغ *a, ثابت فراغ *b)
{
    أرجع (*(uint32_t *)a - * (uint32_t *)b);
}

ساكن uint32_t calculate_median(uint32_t *مقادیر, صحيح اندازه)
{
    /* Sort the array of values containing the execution time of each iteration */
    qsort(مقادیر, اندازه, حجم(uint32_t), compare_uint32);
    /* Return the median value */
    أرجع مقادیر[اندازه / 2];
}

ساكن uint32_t calculate_معدل(uint32_t *مقادیر, صحيح اندازه)
{
    uint64_t جمع = 0;
    لكل (صحيح i = 0; i < اندازه; i++) {
        جمع += مقادیر[i];
    }
    أرجع (uint32_t)(جمع / اندازه);
}

TEST_CASE("freertos api performance test", "[freertos][ignore]")
{
    چاپ_کن("=== FreeRTOS API Performance Test ===\n");
    چاپ_کن("This test measures execution time of commonly used FreeRTOS APIs.\n");
    چاپ_کن("Results help evaluate the impact of memory placement (Flash vs IRAM).\n\n");

#إذا defined(CONFIG_FREERTOS_درون_IRAM)
    چاپ_کن("FreeRTOS functions placed in IRAM.\r\n");
#وإلا
    چاپ_کن("FreeRTOS functions placed in Flash.\r\n");
#پایان_اگر
    چاپ_کن("Testing with %d samples per API\n", عدد_از_SAMPLES);
    چاپ_کن("Note: Performance varies by target architecture, flash cache, and system load.\n\n");

    uint32_t execution_times[عدد_از_SAMPLES];
    uint32_t test_داده = 0x12345678;

    // Initialize test objects
    صف_Handle_t test_صف = x_صف_Create(10, حجم(uint32_t));
    SemaphoreHandle_t test_semaphore = x_Semaphore_Create_دودویی();
    Event_گروه_Handle_t test_event_گروه = x_Event_گروه_Create();
    جریان_میانگیر_Handle_t test_جریان_میانگیر = x_جریان_میانگیر_Create(1024, 1);

    TEST_ASSERT(test_صف != عدم);
    TEST_ASSERT(test_semaphore != عدم);
    TEST_ASSERT(test_event_گروه != عدم);
    TEST_ASSERT(test_جریان_میانگیر != عدم);

    // Test 1: Queue APIs
    چاپ_کن("1. Testing Queue APIs...\n");

    // xQueueSend
    لكل (صحيح i = 0; i < عدد_از_SAMPLES; i++) {
        uint32_t شروع_چرخهها = esp_cpu_get_چرخه_شمار();
        x_صف_Send(test_صف, &test_داده, 0);
        execution_times[i] = esp_cpu_get_چرخه_شمار() - شروع_چرخهها;
    }
    uint32_t صف_send_median = calculate_median(execution_times, عدد_از_SAMPLES);
    uint32_t صف_send_avg = calculate_معدل(execution_times, عدد_از_SAMPLES);
    چاپ_کن("   xQueueSend: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", صف_send_median, صف_send_avg);

    // xQueueReceive
    لكل (صحيح i = 0; i < عدد_از_SAMPLES; i++) {
        uint32_t شروع_چرخهها = esp_cpu_get_چرخه_شمار();
        x_صف_Receive(test_صف, &test_داده, 0);
        execution_times[i] = esp_cpu_get_چرخه_شمار() - شروع_چرخهها;
    }
    uint32_t صف_receive_median = calculate_median(execution_times, عدد_از_SAMPLES);
    uint32_t صف_receive_avg = calculate_معدل(execution_times, عدد_از_SAMPLES);
    چاپ_کن("   xQueueReceive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", صف_receive_median, صف_receive_avg);

    // Test 2: Semaphore APIs
    چاپ_کن("\n2. Testing Semaphore APIs...\n");

    // xSemaphoreGive
    لكل (صحيح i = 0; i < عدد_از_SAMPLES; i++) {
        uint32_t شروع_چرخهها = esp_cpu_get_چرخه_شمار();
        xSemaphoreGive(test_semaphore);
        execution_times[i] = esp_cpu_get_چرخه_شمار() - شروع_چرخهها;
    }
    uint32_t sem_give_median = calculate_median(execution_times, عدد_از_SAMPLES);
    uint32_t sem_give_avg = calculate_معدل(execution_times, عدد_از_SAMPLES);
    چاپ_کن("   xSemaphoreGive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", sem_give_median, sem_give_avg);

    // xSemaphoreTake
    لكل (صحيح i = 0; i < عدد_از_SAMPLES; i++) {
        uint32_t شروع_چرخهها = esp_cpu_get_چرخه_شمار();
        xSemaphoreTake(test_semaphore, 0);
        execution_times[i] = esp_cpu_get_چرخه_شمار() - شروع_چرخهها;
    }
    uint32_t sem_take_median = calculate_median(execution_times, عدد_از_SAMPLES);
    uint32_t sem_take_avg = calculate_معدل(execution_times, عدد_از_SAMPLES);
    چاپ_کن("   xSemaphoreTake: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", sem_take_median, sem_take_avg);

    // Test 3: Event Group APIs
    چاپ_کن("\n3. Testing Event Group APIs...\n");

    // xEventGroupSetBits
    لكل (صحيح i = 0; i < عدد_از_SAMPLES; i++) {
        uint32_t شروع_چرخهها = esp_cpu_get_چرخه_شمار();
        x_Event_گروه_مجموعه_بیتها(test_event_گروه, 0x01);
        execution_times[i] = esp_cpu_get_چرخه_شمار() - شروع_چرخهها;
    }
    uint32_t event_مجموعه_median = calculate_median(execution_times, عدد_از_SAMPLES);
    uint32_t event_مجموعه_avg = calculate_معدل(execution_times, عدد_از_SAMPLES);
    چاپ_کن("   xEventGroupSetBits: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", event_مجموعه_median, event_مجموعه_avg);

    // xEventGroupGetBits
    لكل (صحيح i = 0; i < عدد_از_SAMPLES; i++) {
        uint32_t شروع_چرخهها = esp_cpu_get_چرخه_شمار();
        x_Event_گروه_Get_بیتها(test_event_گروه);
        execution_times[i] = esp_cpu_get_چرخه_شمار() - شروع_چرخهها;
    }
    uint32_t event_get_median = calculate_median(execution_times, عدد_از_SAMPLES);
    uint32_t event_get_avg = calculate_معدل(execution_times, عدد_از_SAMPLES);
    چاپ_کن("   xEventGroupGetBits: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", event_get_median, event_get_avg);

    // Test 4: Stream Buffer APIs
    چاپ_کن("\n4. Testing Stream Buffer APIs...\n");

    // xStreamBufferSend
    لكل (صحيح i = 0; i < عدد_از_SAMPLES; i++) {
        uint32_t شروع_چرخهها = esp_cpu_get_چرخه_شمار();
        x_جریان_میانگیر_Send(test_جریان_میانگیر, &test_داده, حجم(uint32_t), 0);
        execution_times[i] = esp_cpu_get_چرخه_شمار() - شروع_چرخهها;
    }
    uint32_t جریان_send_median = calculate_median(execution_times, عدد_از_SAMPLES);
    uint32_t جریان_send_avg = calculate_معدل(execution_times, عدد_از_SAMPLES);
    چاپ_کن("   xStreamBufferSend: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", جریان_send_median, جریان_send_avg);

    // xStreamBufferReceive
    لكل (صحيح i = 0; i < عدد_از_SAMPLES; i++) {
        uint32_t شروع_چرخهها = esp_cpu_get_چرخه_شمار();
        x_جریان_میانگیر_Receive(test_جریان_میانگیر, &test_داده, حجم(uint32_t), 0);
        execution_times[i] = esp_cpu_get_چرخه_شمار() - شروع_چرخهها;
    }
    uint32_t جریان_receive_median = calculate_median(execution_times, عدد_از_SAMPLES);
    uint32_t جریان_receive_avg = calculate_معدل(execution_times, عدد_از_SAMPLES);
    چاپ_کن("   xStreamBufferReceive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", جریان_receive_median, جریان_receive_avg);

    // Test 5: Task APIs
    چاپ_کن("\n5. Testing Task APIs...\n");

    // xTaskGetTickCount
    لكل (صحيح i = 0; i < عدد_از_SAMPLES; i++) {
        uint32_t شروع_چرخهها = esp_cpu_get_چرخه_شمار();
        x_Task_Get_Tick_شمار();
        execution_times[i] = esp_cpu_get_چرخه_شمار() - شروع_چرخهها;
    }
    uint32_t task_tick_median = calculate_median(execution_times, عدد_از_SAMPLES);
    uint32_t task_tick_avg = calculate_معدل(execution_times, عدد_از_SAMPLES);
    چاپ_کن("   xTaskGetTickCount: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", task_tick_median, task_tick_avg);

    چاپ_کن("\n=== Performance Test Complete ===\n");
    چاپ_کن("Note: Compare results between Flash and IRAM configurations.\n");
    چاپ_کن("Consider memory cost vs performance benefit for your application.\n");

    // Cleanup
    v_جریان_میانگیر_Delete(test_جریان_میانگیر);
    v_Event_گروه_Delete(test_event_گروه);
    vSemaphoreDelete(test_semaphore);
    v_صف_Delete(test_صف);
}
