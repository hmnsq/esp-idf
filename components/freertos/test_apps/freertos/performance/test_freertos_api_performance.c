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

#شامل <esp_types.h>
#شامل <stdio.h>
#شامل <inttypes.h>
#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل "freertos/semphr.h"
#شامل "freertos/queue.h"
#شامل "freertos/event_groups.h"
#شامل "freertos/stream_buffer.h"
#شامل "freertos/timers.h"
#شامل "esp_cpu.h"
#شامل "unity.h"
#شامل "test_utils.h"

#define عدد_OF_SAMPLES 1000

ایستا صحیح compare_uint32(ثابت تهی *a, ثابت تهی *b)
{
    بازگشت (*(uint32_t *)a - * (uint32_t *)b);
}

ایستا uint32_t calculate_median(uint32_t *مقادیر, صحیح size)
{
    /* Sort the array of values containing the execution time of each iteration */
    qsort(مقادیر, size, اندازه(uint32_t), compare_uint32);
    /* Return the median value */
    بازگشت مقادیر[size / 2];
}

ایستا uint32_t calculate_average(uint32_t *مقادیر, صحیح size)
{
    uint64_t sum = 0;
    برای (صحیح i = 0; i < size; i++) {
        sum += مقادیر[i];
    }
    بازگشت (uint32_t)(sum / size);
}

TEST_CASE("freertos api performance test", "[freertos][ignore]")
{
    printf("=== FreeRTOS API Performance Test ===\n");
    printf("This test measures execution time of commonly used FreeRTOS APIs.\n");
    printf("Results help evaluate the impact of memory placement (Flash vs IRAM).\n\n");

#اگر defined(CONFIG_FREERTOS_IN_IRAM)
    printf("FreeRTOS functions placed in IRAM.\r\n");
#وگرنه
    printf("FreeRTOS functions placed in Flash.\r\n");
#endif
    printf("Testing with %d samples per API\n", عدد_OF_SAMPLES);
    printf("Note: Performance varies by target architecture, flash cache, and system load.\n\n");

    uint32_t execution_times[عدد_OF_SAMPLES];
    uint32_t test_data = 0x12345678;

    // Initialize test objects
    QueueHandle_t test_queue = xQueueCreate(10, اندازه(uint32_t));
    SemaphoreHandle_t test_semaphore = x_Semaphore_Create_دودویی();
    EventGroupHandle_t test_event_group = xEventGroupCreate();
    StreamBufferHandle_t test_stream_buffer = xStreamBufferCreate(1024, 1);

    TEST_ASSERT(test_queue != پوچ);
    TEST_ASSERT(test_semaphore != پوچ);
    TEST_ASSERT(test_event_group != پوچ);
    TEST_ASSERT(test_stream_buffer != پوچ);

    // Test 1: Queue APIs
    printf("1. Testing Queue APIs...\n");

    // xQueueSend
    برای (صحیح i = 0; i < عدد_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_شمار();
        xQueueSend(test_queue, &test_data, 0);
        execution_times[i] = esp_cpu_get_cycle_شمار() - start_cycles;
    }
    uint32_t queue_send_median = calculate_median(execution_times, عدد_OF_SAMPLES);
    uint32_t queue_send_avg = calculate_average(execution_times, عدد_OF_SAMPLES);
    printf("   xQueueSend: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", queue_send_median, queue_send_avg);

    // xQueueReceive
    برای (صحیح i = 0; i < عدد_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_شمار();
        xQueueReceive(test_queue, &test_data, 0);
        execution_times[i] = esp_cpu_get_cycle_شمار() - start_cycles;
    }
    uint32_t queue_receive_median = calculate_median(execution_times, عدد_OF_SAMPLES);
    uint32_t queue_receive_avg = calculate_average(execution_times, عدد_OF_SAMPLES);
    printf("   xQueueReceive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", queue_receive_median, queue_receive_avg);

    // Test 2: Semaphore APIs
    printf("\n2. Testing Semaphore APIs...\n");

    // xSemaphoreGive
    برای (صحیح i = 0; i < عدد_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_شمار();
        xSemaphoreGive(test_semaphore);
        execution_times[i] = esp_cpu_get_cycle_شمار() - start_cycles;
    }
    uint32_t sem_give_median = calculate_median(execution_times, عدد_OF_SAMPLES);
    uint32_t sem_give_avg = calculate_average(execution_times, عدد_OF_SAMPLES);
    printf("   xSemaphoreGive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", sem_give_median, sem_give_avg);

    // xSemaphoreTake
    برای (صحیح i = 0; i < عدد_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_شمار();
        xSemaphoreTake(test_semaphore, 0);
        execution_times[i] = esp_cpu_get_cycle_شمار() - start_cycles;
    }
    uint32_t sem_take_median = calculate_median(execution_times, عدد_OF_SAMPLES);
    uint32_t sem_take_avg = calculate_average(execution_times, عدد_OF_SAMPLES);
    printf("   xSemaphoreTake: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", sem_take_median, sem_take_avg);

    // Test 3: Event Group APIs
    printf("\n3. Testing Event Group APIs...\n");

    // xEventGroupSetBits
    برای (صحیح i = 0; i < عدد_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_شمار();
        xEventGroupSetBits(test_event_group, 0x01);
        execution_times[i] = esp_cpu_get_cycle_شمار() - start_cycles;
    }
    uint32_t event_set_median = calculate_median(execution_times, عدد_OF_SAMPLES);
    uint32_t event_set_avg = calculate_average(execution_times, عدد_OF_SAMPLES);
    printf("   xEventGroupSetBits: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", event_set_median, event_set_avg);

    // xEventGroupGetBits
    برای (صحیح i = 0; i < عدد_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_شمار();
        xEventGroupGetBits(test_event_group);
        execution_times[i] = esp_cpu_get_cycle_شمار() - start_cycles;
    }
    uint32_t event_get_median = calculate_median(execution_times, عدد_OF_SAMPLES);
    uint32_t event_get_avg = calculate_average(execution_times, عدد_OF_SAMPLES);
    printf("   xEventGroupGetBits: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", event_get_median, event_get_avg);

    // Test 4: Stream Buffer APIs
    printf("\n4. Testing Stream Buffer APIs...\n");

    // xStreamBufferSend
    برای (صحیح i = 0; i < عدد_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_شمار();
        xStreamBufferSend(test_stream_buffer, &test_data, اندازه(uint32_t), 0);
        execution_times[i] = esp_cpu_get_cycle_شمار() - start_cycles;
    }
    uint32_t stream_send_median = calculate_median(execution_times, عدد_OF_SAMPLES);
    uint32_t stream_send_avg = calculate_average(execution_times, عدد_OF_SAMPLES);
    printf("   xStreamBufferSend: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", stream_send_median, stream_send_avg);

    // xStreamBufferReceive
    برای (صحیح i = 0; i < عدد_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_شمار();
        xStreamBufferReceive(test_stream_buffer, &test_data, اندازه(uint32_t), 0);
        execution_times[i] = esp_cpu_get_cycle_شمار() - start_cycles;
    }
    uint32_t stream_receive_median = calculate_median(execution_times, عدد_OF_SAMPLES);
    uint32_t stream_receive_avg = calculate_average(execution_times, عدد_OF_SAMPLES);
    printf("   xStreamBufferReceive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", stream_receive_median, stream_receive_avg);

    // Test 5: Task APIs
    printf("\n5. Testing Task APIs...\n");

    // xTaskGetTickCount
    برای (صحیح i = 0; i < عدد_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_شمار();
        x_Task_Get_Tick_شمار();
        execution_times[i] = esp_cpu_get_cycle_شمار() - start_cycles;
    }
    uint32_t task_tick_median = calculate_median(execution_times, عدد_OF_SAMPLES);
    uint32_t task_tick_avg = calculate_average(execution_times, عدد_OF_SAMPLES);
    printf("   xTaskGetTickCount: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", task_tick_median, task_tick_avg);

    printf("\n=== Performance Test Complete ===\n");
    printf("Note: Compare results between Flash and IRAM configurations.\n");
    printf("Consider memory cost vs performance benefit for your application.\n");

    // Cleanup
    vStreamBufferDelete(test_stream_buffer);
    vEventGroupDelete(test_event_group);
    vSemaphoreDelete(test_semaphore);
    vQueueDelete(test_queue);
}
