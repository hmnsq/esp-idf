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

#ಅನ್ತರ್ಭಾವ <esp_types.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <inttypes.h>
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "freertos/semphr.h"
#ಅನ್ತರ್ಭಾವ "freertos/queue.h"
#ಅನ್ತರ್ಭಾವ "freertos/event_groups.h"
#ಅನ್ತರ್ಭಾವ "freertos/stream_buffer.h"
#ಅನ್ತರ್ಭಾವ "freertos/timers.h"
#ಅನ್ತರ್ಭಾವ "esp_cpu.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "test_utils.h"

#define ಸಂಖ್ಯೆ_OF_SAMPLES 1000

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ compare_uint32(ಧ್ರುವ ಶೂನ್ಯ *a, ಧ್ರುವ ಶೂನ್ಯ *b)
{
    ಪ್ರತ್ಯಾಗಮ (*(uint32_t *)a - * (uint32_t *)b);
}

ಸ್ಥಿರ uint32_t calculate_median(uint32_t *ಮೌಲ್ಯಗಳು, ಪೂರ್ಣಾಙ್ಕ size)
{
    /* Sort the array of values containing the execution time of each iteration */
    qsort(ಮೌಲ್ಯಗಳು, size, ಪರಿಮಾಣ(uint32_t), compare_uint32);
    /* Return the median value */
    ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯಗಳು[size / 2];
}

ಸ್ಥಿರ uint32_t calculate_average(uint32_t *ಮೌಲ್ಯಗಳು, ಪೂರ್ಣಾಙ್ಕ size)
{
    uint64_t sum = 0;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < size; i++) {
        sum += ಮೌಲ್ಯಗಳು[i];
    }
    ಪ್ರತ್ಯಾಗಮ (uint32_t)(sum / size);
}

TEST_CASE("freertos api performance test", "[freertos][ignore]")
{
    printf("=== FreeRTOS API Performance Test ===\n");
    printf("This test measures execution time of commonly used FreeRTOS APIs.\n");
    printf("Results help evaluate the impact of memory placement (Flash vs IRAM).\n\n");

#ಯದಿ defined(CONFIG_FREERTOS_IN_IRAM)
    printf("FreeRTOS functions placed in IRAM.\r\n");
#ಅನ್ಯಥಾ
    printf("FreeRTOS functions placed in Flash.\r\n");
#endif
    printf("Testing with %d samples per API\n", ಸಂಖ್ಯೆ_OF_SAMPLES);
    printf("Note: Performance varies by target architecture, flash cache, and system load.\n\n");

    uint32_t execution_times[ಸಂಖ್ಯೆ_OF_SAMPLES];
    uint32_t test_data = 0x12345678;

    // Initialize test objects
    QueueHandle_t test_queue = xQueueCreate(10, ಪರಿಮಾಣ(uint32_t));
    SemaphoreHandle_t test_semaphore = x_Semaphore_Create_ದ್ವಿಮಾನ();
    EventGroupHandle_t test_event_group = xEventGroupCreate();
    StreamBufferHandle_t test_stream_buffer = xStreamBufferCreate(1024, 1);

    TEST_ASSERT(test_queue != ರಿಕ್ತ);
    TEST_ASSERT(test_semaphore != ರಿಕ್ತ);
    TEST_ASSERT(test_event_group != ರಿಕ್ತ);
    TEST_ASSERT(test_stream_buffer != ರಿಕ್ತ);

    // Test 1: Queue APIs
    printf("1. Testing Queue APIs...\n");

    // xQueueSend
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಸಂಖ್ಯೆ_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_ಎಣಿಕೆ();
        xQueueSend(test_queue, &test_data, 0);
        execution_times[i] = esp_cpu_get_cycle_ಎಣಿಕೆ() - start_cycles;
    }
    uint32_t queue_send_median = calculate_median(execution_times, ಸಂಖ್ಯೆ_OF_SAMPLES);
    uint32_t queue_send_avg = calculate_average(execution_times, ಸಂಖ್ಯೆ_OF_SAMPLES);
    printf("   xQueueSend: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", queue_send_median, queue_send_avg);

    // xQueueReceive
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಸಂಖ್ಯೆ_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_ಎಣಿಕೆ();
        xQueueReceive(test_queue, &test_data, 0);
        execution_times[i] = esp_cpu_get_cycle_ಎಣಿಕೆ() - start_cycles;
    }
    uint32_t queue_receive_median = calculate_median(execution_times, ಸಂಖ್ಯೆ_OF_SAMPLES);
    uint32_t queue_receive_avg = calculate_average(execution_times, ಸಂಖ್ಯೆ_OF_SAMPLES);
    printf("   xQueueReceive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", queue_receive_median, queue_receive_avg);

    // Test 2: Semaphore APIs
    printf("\n2. Testing Semaphore APIs...\n");

    // xSemaphoreGive
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಸಂಖ್ಯೆ_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_ಎಣಿಕೆ();
        xSemaphoreGive(test_semaphore);
        execution_times[i] = esp_cpu_get_cycle_ಎಣಿಕೆ() - start_cycles;
    }
    uint32_t sem_give_median = calculate_median(execution_times, ಸಂಖ್ಯೆ_OF_SAMPLES);
    uint32_t sem_give_avg = calculate_average(execution_times, ಸಂಖ್ಯೆ_OF_SAMPLES);
    printf("   xSemaphoreGive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", sem_give_median, sem_give_avg);

    // xSemaphoreTake
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಸಂಖ್ಯೆ_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_ಎಣಿಕೆ();
        xSemaphoreTake(test_semaphore, 0);
        execution_times[i] = esp_cpu_get_cycle_ಎಣಿಕೆ() - start_cycles;
    }
    uint32_t sem_take_median = calculate_median(execution_times, ಸಂಖ್ಯೆ_OF_SAMPLES);
    uint32_t sem_take_avg = calculate_average(execution_times, ಸಂಖ್ಯೆ_OF_SAMPLES);
    printf("   xSemaphoreTake: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", sem_take_median, sem_take_avg);

    // Test 3: Event Group APIs
    printf("\n3. Testing Event Group APIs...\n");

    // xEventGroupSetBits
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಸಂಖ್ಯೆ_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_ಎಣಿಕೆ();
        xEventGroupSetBits(test_event_group, 0x01);
        execution_times[i] = esp_cpu_get_cycle_ಎಣಿಕೆ() - start_cycles;
    }
    uint32_t event_set_median = calculate_median(execution_times, ಸಂಖ್ಯೆ_OF_SAMPLES);
    uint32_t event_set_avg = calculate_average(execution_times, ಸಂಖ್ಯೆ_OF_SAMPLES);
    printf("   xEventGroupSetBits: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", event_set_median, event_set_avg);

    // xEventGroupGetBits
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಸಂಖ್ಯೆ_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_ಎಣಿಕೆ();
        xEventGroupGetBits(test_event_group);
        execution_times[i] = esp_cpu_get_cycle_ಎಣಿಕೆ() - start_cycles;
    }
    uint32_t event_get_median = calculate_median(execution_times, ಸಂಖ್ಯೆ_OF_SAMPLES);
    uint32_t event_get_avg = calculate_average(execution_times, ಸಂಖ್ಯೆ_OF_SAMPLES);
    printf("   xEventGroupGetBits: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", event_get_median, event_get_avg);

    // Test 4: Stream Buffer APIs
    printf("\n4. Testing Stream Buffer APIs...\n");

    // xStreamBufferSend
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಸಂಖ್ಯೆ_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_ಎಣಿಕೆ();
        xStreamBufferSend(test_stream_buffer, &test_data, ಪರಿಮಾಣ(uint32_t), 0);
        execution_times[i] = esp_cpu_get_cycle_ಎಣಿಕೆ() - start_cycles;
    }
    uint32_t stream_send_median = calculate_median(execution_times, ಸಂಖ್ಯೆ_OF_SAMPLES);
    uint32_t stream_send_avg = calculate_average(execution_times, ಸಂಖ್ಯೆ_OF_SAMPLES);
    printf("   xStreamBufferSend: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", stream_send_median, stream_send_avg);

    // xStreamBufferReceive
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಸಂಖ್ಯೆ_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_ಎಣಿಕೆ();
        xStreamBufferReceive(test_stream_buffer, &test_data, ಪರಿಮಾಣ(uint32_t), 0);
        execution_times[i] = esp_cpu_get_cycle_ಎಣಿಕೆ() - start_cycles;
    }
    uint32_t stream_receive_median = calculate_median(execution_times, ಸಂಖ್ಯೆ_OF_SAMPLES);
    uint32_t stream_receive_avg = calculate_average(execution_times, ಸಂಖ್ಯೆ_OF_SAMPLES);
    printf("   xStreamBufferReceive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", stream_receive_median, stream_receive_avg);

    // Test 5: Task APIs
    printf("\n5. Testing Task APIs...\n");

    // xTaskGetTickCount
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಸಂಖ್ಯೆ_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_ಎಣಿಕೆ();
        x_Task_Get_Tick_ಎಣಿಕೆ();
        execution_times[i] = esp_cpu_get_cycle_ಎಣಿಕೆ() - start_cycles;
    }
    uint32_t task_tick_median = calculate_median(execution_times, ಸಂಖ್ಯೆ_OF_SAMPLES);
    uint32_t task_tick_avg = calculate_average(execution_times, ಸಂಖ್ಯೆ_OF_SAMPLES);
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
