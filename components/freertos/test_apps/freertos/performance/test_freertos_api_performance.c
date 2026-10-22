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

#অন্তর্ভাৱ <esp_types.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <inttypes.h>
#অন্তর্ভাৱ "freertos/FreeRTOS.h"
#অন্তর্ভাৱ "freertos/task.h"
#অন্তর্ভাৱ "freertos/semphr.h"
#অন্তর্ভাৱ "freertos/queue.h"
#অন্তর্ভাৱ "freertos/event_groups.h"
#অন্তর্ভাৱ "freertos/stream_buffer.h"
#অন্তর্ভাৱ "freertos/timers.h"
#অন্তর্ভাৱ "esp_cpu.h"
#অন্তর্ভাৱ "unity.h"
#অন্তর্ভাৱ "test_utils.h"

#define সংখ্যা_OF_SAMPLES 1000

স্থির পূর্ণাঙ্ক compare_uint32(ধ্রুৱ শূন্য *a, ধ্রুৱ শূন্য *b)
{
    ঘূৰাই (*(uint32_t *)a - * (uint32_t *)b);
}

স্থির uint32_t calculate_median(uint32_t *মানসমূহ, পূর্ণাঙ্ক size)
{
    /* Sort the array of values containing the execution time of each iteration */
    qsort(মানসমূহ, size, পরিমাণ(uint32_t), compare_uint32);
    /* Return the median value */
    ঘূৰাই মানসমূহ[size / 2];
}

স্থির uint32_t calculate_average(uint32_t *মানসমূহ, পূর্ণাঙ্ক size)
{
    uint64_t sum = 0;
    বাবে (পূর্ণাঙ্ক i = 0; i < size; i++) {
        sum += মানসমূহ[i];
    }
    ঘূৰাই (uint32_t)(sum / size);
}

TEST_CASE("freertos api performance test", "[freertos][ignore]")
{
    printf("=== FreeRTOS API Performance Test ===\n");
    printf("This test measures execution time of commonly used FreeRTOS APIs.\n");
    printf("Results help evaluate the impact of memory placement (Flash vs IRAM).\n\n");

#যদি defined(CONFIG_FREERTOS_IN_IRAM)
    printf("FreeRTOS functions placed in IRAM.\r\n");
#অন্যথা
    printf("FreeRTOS functions placed in Flash.\r\n");
#endif
    printf("Testing with %d samples per API\n", সংখ্যা_OF_SAMPLES);
    printf("Note: Performance varies by target architecture, flash cache, and system load.\n\n");

    uint32_t execution_times[সংখ্যা_OF_SAMPLES];
    uint32_t test_data = 0x12345678;

    // Initialize test objects
    QueueHandle_t test_queue = xQueueCreate(10, পরিমাণ(uint32_t));
    SemaphoreHandle_t test_semaphore = x_Semaphore_Create_দ্বিমিক();
    EventGroupHandle_t test_event_group = xEventGroupCreate();
    StreamBufferHandle_t test_stream_buffer = xStreamBufferCreate(1024, 1);

    TEST_ASSERT(test_queue != রিক্ত);
    TEST_ASSERT(test_semaphore != রিক্ত);
    TEST_ASSERT(test_event_group != রিক্ত);
    TEST_ASSERT(test_stream_buffer != রিক্ত);

    // Test 1: Queue APIs
    printf("1. Testing Queue APIs...\n");

    // xQueueSend
    বাবে (পূর্ণাঙ্ক i = 0; i < সংখ্যা_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_গুনতি();
        xQueueSend(test_queue, &test_data, 0);
        execution_times[i] = esp_cpu_get_cycle_গুনতি() - start_cycles;
    }
    uint32_t queue_send_median = calculate_median(execution_times, সংখ্যা_OF_SAMPLES);
    uint32_t queue_send_avg = calculate_average(execution_times, সংখ্যা_OF_SAMPLES);
    printf("   xQueueSend: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", queue_send_median, queue_send_avg);

    // xQueueReceive
    বাবে (পূর্ণাঙ্ক i = 0; i < সংখ্যা_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_গুনতি();
        xQueueReceive(test_queue, &test_data, 0);
        execution_times[i] = esp_cpu_get_cycle_গুনতি() - start_cycles;
    }
    uint32_t queue_receive_median = calculate_median(execution_times, সংখ্যা_OF_SAMPLES);
    uint32_t queue_receive_avg = calculate_average(execution_times, সংখ্যা_OF_SAMPLES);
    printf("   xQueueReceive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", queue_receive_median, queue_receive_avg);

    // Test 2: Semaphore APIs
    printf("\n2. Testing Semaphore APIs...\n");

    // xSemaphoreGive
    বাবে (পূর্ণাঙ্ক i = 0; i < সংখ্যা_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_গুনতি();
        xSemaphoreGive(test_semaphore);
        execution_times[i] = esp_cpu_get_cycle_গুনতি() - start_cycles;
    }
    uint32_t sem_give_median = calculate_median(execution_times, সংখ্যা_OF_SAMPLES);
    uint32_t sem_give_avg = calculate_average(execution_times, সংখ্যা_OF_SAMPLES);
    printf("   xSemaphoreGive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", sem_give_median, sem_give_avg);

    // xSemaphoreTake
    বাবে (পূর্ণাঙ্ক i = 0; i < সংখ্যা_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_গুনতি();
        xSemaphoreTake(test_semaphore, 0);
        execution_times[i] = esp_cpu_get_cycle_গুনতি() - start_cycles;
    }
    uint32_t sem_take_median = calculate_median(execution_times, সংখ্যা_OF_SAMPLES);
    uint32_t sem_take_avg = calculate_average(execution_times, সংখ্যা_OF_SAMPLES);
    printf("   xSemaphoreTake: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", sem_take_median, sem_take_avg);

    // Test 3: Event Group APIs
    printf("\n3. Testing Event Group APIs...\n");

    // xEventGroupSetBits
    বাবে (পূর্ণাঙ্ক i = 0; i < সংখ্যা_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_গুনতি();
        xEventGroupSetBits(test_event_group, 0x01);
        execution_times[i] = esp_cpu_get_cycle_গুনতি() - start_cycles;
    }
    uint32_t event_set_median = calculate_median(execution_times, সংখ্যা_OF_SAMPLES);
    uint32_t event_set_avg = calculate_average(execution_times, সংখ্যা_OF_SAMPLES);
    printf("   xEventGroupSetBits: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", event_set_median, event_set_avg);

    // xEventGroupGetBits
    বাবে (পূর্ণাঙ্ক i = 0; i < সংখ্যা_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_গুনতি();
        xEventGroupGetBits(test_event_group);
        execution_times[i] = esp_cpu_get_cycle_গুনতি() - start_cycles;
    }
    uint32_t event_get_median = calculate_median(execution_times, সংখ্যা_OF_SAMPLES);
    uint32_t event_get_avg = calculate_average(execution_times, সংখ্যা_OF_SAMPLES);
    printf("   xEventGroupGetBits: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", event_get_median, event_get_avg);

    // Test 4: Stream Buffer APIs
    printf("\n4. Testing Stream Buffer APIs...\n");

    // xStreamBufferSend
    বাবে (পূর্ণাঙ্ক i = 0; i < সংখ্যা_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_গুনতি();
        xStreamBufferSend(test_stream_buffer, &test_data, পরিমাণ(uint32_t), 0);
        execution_times[i] = esp_cpu_get_cycle_গুনতি() - start_cycles;
    }
    uint32_t stream_send_median = calculate_median(execution_times, সংখ্যা_OF_SAMPLES);
    uint32_t stream_send_avg = calculate_average(execution_times, সংখ্যা_OF_SAMPLES);
    printf("   xStreamBufferSend: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", stream_send_median, stream_send_avg);

    // xStreamBufferReceive
    বাবে (পূর্ণাঙ্ক i = 0; i < সংখ্যা_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_গুনতি();
        xStreamBufferReceive(test_stream_buffer, &test_data, পরিমাণ(uint32_t), 0);
        execution_times[i] = esp_cpu_get_cycle_গুনতি() - start_cycles;
    }
    uint32_t stream_receive_median = calculate_median(execution_times, সংখ্যা_OF_SAMPLES);
    uint32_t stream_receive_avg = calculate_average(execution_times, সংখ্যা_OF_SAMPLES);
    printf("   xStreamBufferReceive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", stream_receive_median, stream_receive_avg);

    // Test 5: Task APIs
    printf("\n5. Testing Task APIs...\n");

    // xTaskGetTickCount
    বাবে (পূর্ণাঙ্ক i = 0; i < সংখ্যা_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_গুনতি();
        x_Task_Get_Tick_গুনতি();
        execution_times[i] = esp_cpu_get_cycle_গুনতি() - start_cycles;
    }
    uint32_t task_tick_median = calculate_median(execution_times, সংখ্যা_OF_SAMPLES);
    uint32_t task_tick_avg = calculate_average(execution_times, সংখ্যা_OF_SAMPLES);
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
