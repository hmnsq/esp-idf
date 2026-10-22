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

#அந்தர்பாவ <esp_types.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <inttypes.h>
#அந்தர்பாவ "freertos/FreeRTOS.h"
#அந்தர்பாவ "freertos/task.h"
#அந்தர்பாவ "freertos/semphr.h"
#அந்தர்பாவ "freertos/queue.h"
#அந்தர்பாவ "freertos/event_groups.h"
#அந்தர்பாவ "freertos/stream_buffer.h"
#அந்தர்பாவ "freertos/timers.h"
#அந்தர்பாவ "esp_cpu.h"
#அந்தர்பாவ "unity.h"
#அந்தர்பாவ "test_utils.h"

#define எண்_OF_SAMPLES 1000

நிலையான முழு compare_uint32(மாறிலி வெறுமை *a, மாறிலி வெறுமை *b)
{
    திரும்பு (*(uint32_t *)a - * (uint32_t *)b);
}

நிலையான uint32_t calculate_median(uint32_t *மதிப்புகள், முழு size)
{
    /* Sort the array of values containing the execution time of each iteration */
    qsort(மதிப்புகள், size, அளவு(uint32_t), compare_uint32);
    /* Return the median value */
    திரும்பு மதிப்புகள்[size / 2];
}

நிலையான uint32_t calculate_average(uint32_t *மதிப்புகள், முழு size)
{
    uint64_t sum = 0;
    க்கு (முழு i = 0; i < size; i++) {
        sum += மதிப்புகள்[i];
    }
    திரும்பு (uint32_t)(sum / size);
}

TEST_CASE("freertos api performance test", "[freertos][ignore]")
{
    printf("=== FreeRTOS API Performance Test ===\n");
    printf("This test measures execution time of commonly used FreeRTOS APIs.\n");
    printf("Results help evaluate the impact of memory placement (Flash vs IRAM).\n\n");

#எனில் defined(CONFIG_FREERTOS_IN_IRAM)
    printf("FreeRTOS functions placed in IRAM.\r\n");
#இல்லையெனில்
    printf("FreeRTOS functions placed in Flash.\r\n");
#endif
    printf("Testing with %d samples per API\n", எண்_OF_SAMPLES);
    printf("Note: Performance varies by target architecture, flash cache, and system load.\n\n");

    uint32_t execution_times[எண்_OF_SAMPLES];
    uint32_t test_data = 0x12345678;

    // Initialize test objects
    QueueHandle_t test_queue = xQueueCreate(10, அளவு(uint32_t));
    SemaphoreHandle_t test_semaphore = x_Semaphore_Create_இருமம்();
    EventGroupHandle_t test_event_group = xEventGroupCreate();
    StreamBufferHandle_t test_stream_buffer = xStreamBufferCreate(1024, 1);

    TEST_ASSERT(test_queue != வெற்று);
    TEST_ASSERT(test_semaphore != வெற்று);
    TEST_ASSERT(test_event_group != வெற்று);
    TEST_ASSERT(test_stream_buffer != வெற்று);

    // Test 1: Queue APIs
    printf("1. Testing Queue APIs...\n");

    // xQueueSend
    க்கு (முழு i = 0; i < எண்_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_மொத்தம்();
        xQueueSend(test_queue, &test_data, 0);
        execution_times[i] = esp_cpu_get_cycle_மொத்தம்() - start_cycles;
    }
    uint32_t queue_send_median = calculate_median(execution_times, எண்_OF_SAMPLES);
    uint32_t queue_send_avg = calculate_average(execution_times, எண்_OF_SAMPLES);
    printf("   xQueueSend: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", queue_send_median, queue_send_avg);

    // xQueueReceive
    க்கு (முழு i = 0; i < எண்_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_மொத்தம்();
        xQueueReceive(test_queue, &test_data, 0);
        execution_times[i] = esp_cpu_get_cycle_மொத்தம்() - start_cycles;
    }
    uint32_t queue_receive_median = calculate_median(execution_times, எண்_OF_SAMPLES);
    uint32_t queue_receive_avg = calculate_average(execution_times, எண்_OF_SAMPLES);
    printf("   xQueueReceive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", queue_receive_median, queue_receive_avg);

    // Test 2: Semaphore APIs
    printf("\n2. Testing Semaphore APIs...\n");

    // xSemaphoreGive
    க்கு (முழு i = 0; i < எண்_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_மொத்தம்();
        xSemaphoreGive(test_semaphore);
        execution_times[i] = esp_cpu_get_cycle_மொத்தம்() - start_cycles;
    }
    uint32_t sem_give_median = calculate_median(execution_times, எண்_OF_SAMPLES);
    uint32_t sem_give_avg = calculate_average(execution_times, எண்_OF_SAMPLES);
    printf("   xSemaphoreGive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", sem_give_median, sem_give_avg);

    // xSemaphoreTake
    க்கு (முழு i = 0; i < எண்_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_மொத்தம்();
        xSemaphoreTake(test_semaphore, 0);
        execution_times[i] = esp_cpu_get_cycle_மொத்தம்() - start_cycles;
    }
    uint32_t sem_take_median = calculate_median(execution_times, எண்_OF_SAMPLES);
    uint32_t sem_take_avg = calculate_average(execution_times, எண்_OF_SAMPLES);
    printf("   xSemaphoreTake: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", sem_take_median, sem_take_avg);

    // Test 3: Event Group APIs
    printf("\n3. Testing Event Group APIs...\n");

    // xEventGroupSetBits
    க்கு (முழு i = 0; i < எண்_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_மொத்தம்();
        xEventGroupSetBits(test_event_group, 0x01);
        execution_times[i] = esp_cpu_get_cycle_மொத்தம்() - start_cycles;
    }
    uint32_t event_set_median = calculate_median(execution_times, எண்_OF_SAMPLES);
    uint32_t event_set_avg = calculate_average(execution_times, எண்_OF_SAMPLES);
    printf("   xEventGroupSetBits: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", event_set_median, event_set_avg);

    // xEventGroupGetBits
    க்கு (முழு i = 0; i < எண்_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_மொத்தம்();
        xEventGroupGetBits(test_event_group);
        execution_times[i] = esp_cpu_get_cycle_மொத்தம்() - start_cycles;
    }
    uint32_t event_get_median = calculate_median(execution_times, எண்_OF_SAMPLES);
    uint32_t event_get_avg = calculate_average(execution_times, எண்_OF_SAMPLES);
    printf("   xEventGroupGetBits: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", event_get_median, event_get_avg);

    // Test 4: Stream Buffer APIs
    printf("\n4. Testing Stream Buffer APIs...\n");

    // xStreamBufferSend
    க்கு (முழு i = 0; i < எண்_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_மொத்தம்();
        xStreamBufferSend(test_stream_buffer, &test_data, அளவு(uint32_t), 0);
        execution_times[i] = esp_cpu_get_cycle_மொத்தம்() - start_cycles;
    }
    uint32_t stream_send_median = calculate_median(execution_times, எண்_OF_SAMPLES);
    uint32_t stream_send_avg = calculate_average(execution_times, எண்_OF_SAMPLES);
    printf("   xStreamBufferSend: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", stream_send_median, stream_send_avg);

    // xStreamBufferReceive
    க்கு (முழு i = 0; i < எண்_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_மொத்தம்();
        xStreamBufferReceive(test_stream_buffer, &test_data, அளவு(uint32_t), 0);
        execution_times[i] = esp_cpu_get_cycle_மொத்தம்() - start_cycles;
    }
    uint32_t stream_receive_median = calculate_median(execution_times, எண்_OF_SAMPLES);
    uint32_t stream_receive_avg = calculate_average(execution_times, எண்_OF_SAMPLES);
    printf("   xStreamBufferReceive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", stream_receive_median, stream_receive_avg);

    // Test 5: Task APIs
    printf("\n5. Testing Task APIs...\n");

    // xTaskGetTickCount
    க்கு (முழு i = 0; i < எண்_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_மொத்தம்();
        x_Task_Get_Tick_மொத்தம்();
        execution_times[i] = esp_cpu_get_cycle_மொத்தம்() - start_cycles;
    }
    uint32_t task_tick_median = calculate_median(execution_times, எண்_OF_SAMPLES);
    uint32_t task_tick_avg = calculate_average(execution_times, எண்_OF_SAMPLES);
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
