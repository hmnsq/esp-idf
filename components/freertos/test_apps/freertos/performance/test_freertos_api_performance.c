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

#अन्तर्भाव <esp_types.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <inttypes.h>
#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"
#अन्तर्भाव "freertos/semphr.h"
#अन्तर्भाव "freertos/queue.h"
#अन्तर्भाव "freertos/event_groups.h"
#अन्तर्भाव "freertos/stream_buffer.h"
#अन्तर्भाव "freertos/timers.h"
#अन्तर्भाव "esp_cpu.h"
#अन्तर्भाव "unity.h"
#अन्तर्भाव "test_utils.h"

#define संख्या_OF_SAMPLES 1000

स्थिर पूर्णाङ्क compare_uint32(ध्रुव शून्य *a, ध्रुव शून्य *b)
{
    प्रत्यागम (*(uint32_t *)a - * (uint32_t *)b);
}

स्थिर uint32_t calculate_median(uint32_t *मूल्यावली, पूर्णाङ्क size)
{
    /* Sort the array of values containing the execution time of each iteration */
    क्विक(मूल्यावली, size, परिमाण(uint32_t), compare_uint32);
    /* Return the median value */
    प्रत्यागम मूल्यावली[size / 2];
}

स्थिर uint32_t calculate_average(uint32_t *मूल्यावली, पूर्णाङ्क size)
{
    uint64_t sum = 0;
    प्रति (पूर्णाङ्क i = 0; i < size; i++) {
        sum += मूल्यावली[i];
    }
    प्रत्यागम (uint32_t)(sum / size);
}

TEST_CASE("freertos api performance test", "[freertos][ignore]")
{
    म_लिखो("=== FreeRTOS API Performance Test ===\n");
    म_लिखो("This test measures execution time of commonly used FreeRTOS APIs.\n");
    म_लिखो("Results help evaluate the impact of memory placement (Flash vs IRAM).\n\n");

#यदि defined(CONFIG_FREERTOS_IN_IRAM)
    म_लिखो("FreeRTOS functions placed in IRAM.\r\n");
#अन्यथा
    म_लिखो("FreeRTOS functions placed in Flash.\r\n");
#endif
    म_लिखो("Testing with %d samples per API\n", संख्या_OF_SAMPLES);
    म_लिखो("Note: Performance varies by target architecture, flash cache, and system load.\n\n");

    uint32_t execution_times[संख्या_OF_SAMPLES];
    uint32_t test_data = 0x12345678;

    // Initialize test objects
    QueueHandle_t test_queue = xQueueCreate(10, परिमाण(uint32_t));
    SemaphoreHandle_t test_semaphore = x_Semaphore_Create_द्विआधारी();
    EventGroupHandle_t test_event_group = xEventGroupCreate();
    StreamBufferHandle_t test_stream_buffer = xStreamBufferCreate(1024, 1);

    TEST_ASSERT(test_queue != रिक्त);
    TEST_ASSERT(test_semaphore != रिक्त);
    TEST_ASSERT(test_event_group != रिक्त);
    TEST_ASSERT(test_stream_buffer != रिक्त);

    // Test 1: Queue APIs
    म_लिखो("1. Testing Queue APIs...\n");

    // xQueueSend
    प्रति (पूर्णाङ्क i = 0; i < संख्या_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_गिनती();
        xQueueSend(test_queue, &test_data, 0);
        execution_times[i] = esp_cpu_get_cycle_गिनती() - start_cycles;
    }
    uint32_t queue_send_median = calculate_median(execution_times, संख्या_OF_SAMPLES);
    uint32_t queue_send_avg = calculate_average(execution_times, संख्या_OF_SAMPLES);
    म_लिखो("   xQueueSend: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", queue_send_median, queue_send_avg);

    // xQueueReceive
    प्रति (पूर्णाङ्क i = 0; i < संख्या_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_गिनती();
        xQueueReceive(test_queue, &test_data, 0);
        execution_times[i] = esp_cpu_get_cycle_गिनती() - start_cycles;
    }
    uint32_t queue_receive_median = calculate_median(execution_times, संख्या_OF_SAMPLES);
    uint32_t queue_receive_avg = calculate_average(execution_times, संख्या_OF_SAMPLES);
    म_लिखो("   xQueueReceive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", queue_receive_median, queue_receive_avg);

    // Test 2: Semaphore APIs
    म_लिखो("\n2. Testing Semaphore APIs...\n");

    // xSemaphoreGive
    प्रति (पूर्णाङ्क i = 0; i < संख्या_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_गिनती();
        xSemaphoreGive(test_semaphore);
        execution_times[i] = esp_cpu_get_cycle_गिनती() - start_cycles;
    }
    uint32_t sem_give_median = calculate_median(execution_times, संख्या_OF_SAMPLES);
    uint32_t sem_give_avg = calculate_average(execution_times, संख्या_OF_SAMPLES);
    म_लिखो("   xSemaphoreGive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", sem_give_median, sem_give_avg);

    // xSemaphoreTake
    प्रति (पूर्णाङ्क i = 0; i < संख्या_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_गिनती();
        xSemaphoreTake(test_semaphore, 0);
        execution_times[i] = esp_cpu_get_cycle_गिनती() - start_cycles;
    }
    uint32_t sem_take_median = calculate_median(execution_times, संख्या_OF_SAMPLES);
    uint32_t sem_take_avg = calculate_average(execution_times, संख्या_OF_SAMPLES);
    म_लिखो("   xSemaphoreTake: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", sem_take_median, sem_take_avg);

    // Test 3: Event Group APIs
    म_लिखो("\n3. Testing Event Group APIs...\n");

    // xEventGroupSetBits
    प्रति (पूर्णाङ्क i = 0; i < संख्या_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_गिनती();
        xEventGroupSetBits(test_event_group, 0x01);
        execution_times[i] = esp_cpu_get_cycle_गिनती() - start_cycles;
    }
    uint32_t event_set_median = calculate_median(execution_times, संख्या_OF_SAMPLES);
    uint32_t event_set_avg = calculate_average(execution_times, संख्या_OF_SAMPLES);
    म_लिखो("   xEventGroupSetBits: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", event_set_median, event_set_avg);

    // xEventGroupGetBits
    प्रति (पूर्णाङ्क i = 0; i < संख्या_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_गिनती();
        xEventGroupGetBits(test_event_group);
        execution_times[i] = esp_cpu_get_cycle_गिनती() - start_cycles;
    }
    uint32_t event_get_median = calculate_median(execution_times, संख्या_OF_SAMPLES);
    uint32_t event_get_avg = calculate_average(execution_times, संख्या_OF_SAMPLES);
    म_लिखो("   xEventGroupGetBits: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", event_get_median, event_get_avg);

    // Test 4: Stream Buffer APIs
    म_लिखो("\n4. Testing Stream Buffer APIs...\n");

    // xStreamBufferSend
    प्रति (पूर्णाङ्क i = 0; i < संख्या_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_गिनती();
        xStreamBufferSend(test_stream_buffer, &test_data, परिमाण(uint32_t), 0);
        execution_times[i] = esp_cpu_get_cycle_गिनती() - start_cycles;
    }
    uint32_t stream_send_median = calculate_median(execution_times, संख्या_OF_SAMPLES);
    uint32_t stream_send_avg = calculate_average(execution_times, संख्या_OF_SAMPLES);
    म_लिखो("   xStreamBufferSend: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", stream_send_median, stream_send_avg);

    // xStreamBufferReceive
    प्रति (पूर्णाङ्क i = 0; i < संख्या_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_गिनती();
        xStreamBufferReceive(test_stream_buffer, &test_data, परिमाण(uint32_t), 0);
        execution_times[i] = esp_cpu_get_cycle_गिनती() - start_cycles;
    }
    uint32_t stream_receive_median = calculate_median(execution_times, संख्या_OF_SAMPLES);
    uint32_t stream_receive_avg = calculate_average(execution_times, संख्या_OF_SAMPLES);
    म_लिखो("   xStreamBufferReceive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", stream_receive_median, stream_receive_avg);

    // Test 5: Task APIs
    म_लिखो("\n5. Testing Task APIs...\n");

    // xTaskGetTickCount
    प्रति (पूर्णाङ्क i = 0; i < संख्या_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_गिनती();
        x_Task_Get_Tick_गिनती();
        execution_times[i] = esp_cpu_get_cycle_गिनती() - start_cycles;
    }
    uint32_t task_tick_median = calculate_median(execution_times, संख्या_OF_SAMPLES);
    uint32_t task_tick_avg = calculate_average(execution_times, संख्या_OF_SAMPLES);
    म_लिखो("   xTaskGetTickCount: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", task_tick_median, task_tick_avg);

    म_लिखो("\n=== Performance Test Complete ===\n");
    म_लिखो("Note: Compare results between Flash and IRAM configurations.\n");
    म_लिखो("Consider memory cost vs performance benefit for your application.\n");

    // Cleanup
    vStreamBufferDelete(test_stream_buffer);
    vEventGroupDelete(test_event_group);
    vSemaphoreDelete(test_semaphore);
    vQueueDelete(test_queue);
}
