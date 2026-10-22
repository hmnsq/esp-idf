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

#समावेश <esp_types.h>
#समावेश <stdio.h>
#समावेश <inttypes.h>
#समावेश "freertos/FreeRTOS.h"
#समावेश "freertos/task.h"
#समावेश "freertos/semphr.h"
#समावेश "freertos/queue.h"
#समावेश "freertos/event_groups.h"
#समावेश "freertos/stream_buffer.h"
#समावेश "freertos/timers.h"
#समावेश "esp_cpu.h"
#समावेश "unity.h"
#समावेश "test_utils.h"

#घोषणा संख्या_OF_SAMPLES 1000

अटल पूर्णांक compare_uint32(स्थिर व्योम *a, स्थिर व्योम *b)
{
    वापस (*(uint32_t *)a - * (uint32_t *)b);
}

अटल uint32_t calculate_median(uint32_t *मूल्यावली, पूर्णांक आकार)
{
    /* Sort the array of values containing the execution time of each iteration */
    क्विक(मूल्यावली, आकार, माप(uint32_t), compare_uint32);
    /* Return the median value */
    वापस मूल्यावली[आकार / 2];
}

अटल uint32_t calculate_average(uint32_t *मूल्यावली, पूर्णांक आकार)
{
    uint64_t योग_2 = 0;
    क्रम (पूर्णांक i = 0; i < आकार; i++) {
        योग_2 += मूल्यावली[i];
    }
    वापस (uint32_t)(योग_2 / आकार);
}

TEST_CASE("freertos api performance test", "[freertos][ignore]")
{
    म_लिखो("=== FreeRTOS API Performance Test ===\n");
    म_लिखो("This test measures execution time of commonly used FreeRTOS APIs.\n");
    म_लिखो("Results help evaluate the impact of memory placement (Flash vs IRAM).\n\n");

#अगर defined(CONFIG_FREERTOS_IN_IRAM)
    म_लिखो("FreeRTOS functions placed in IRAM.\r\n");
#अन्यथा
    म_लिखो("FreeRTOS functions placed in Flash.\r\n");
#पूर्ण_अगर
    म_लिखो("Testing with %d samples per API\n", संख्या_OF_SAMPLES);
    म_लिखो("Note: Performance varies by target architecture, flash cache, and system load.\n\n");

    uint32_t execution_times[संख्या_OF_SAMPLES];
    uint32_t test_तथ्य = 0x12345678;

    // Initialize test objects
    QueueHandle_t test_queue = xQueueCreate(10, माप(uint32_t));
    SemaphoreHandle_t test_semaphore = x_Semaphore_Create_द्विआधारी();
    EventGroupHandle_t test_event_group = xEventGroupCreate();
    Stream_बफर_Handle_t test_stream_बफर = x_Stream_बफर_Create(1024, 1);

    TEST_ASSERT(test_queue != शून्य);
    TEST_ASSERT(test_semaphore != शून्य);
    TEST_ASSERT(test_event_group != शून्य);
    TEST_ASSERT(test_stream_बफर != शून्य);

    // Test 1: Queue APIs
    म_लिखो("1. Testing Queue APIs...\n");

    // xQueueSend
    क्रम (पूर्णांक i = 0; i < संख्या_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_गिनती();
        xQueueSend(test_queue, &test_तथ्य, 0);
        execution_times[i] = esp_cpu_get_cycle_गिनती() - start_cycles;
    }
    uint32_t queue_send_median = calculate_median(execution_times, संख्या_OF_SAMPLES);
    uint32_t queue_send_avg = calculate_average(execution_times, संख्या_OF_SAMPLES);
    म_लिखो("   xQueueSend: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", queue_send_median, queue_send_avg);

    // xQueueReceive
    क्रम (पूर्णांक i = 0; i < संख्या_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_गिनती();
        xQueueReceive(test_queue, &test_तथ्य, 0);
        execution_times[i] = esp_cpu_get_cycle_गिनती() - start_cycles;
    }
    uint32_t queue_receive_median = calculate_median(execution_times, संख्या_OF_SAMPLES);
    uint32_t queue_receive_avg = calculate_average(execution_times, संख्या_OF_SAMPLES);
    म_लिखो("   xQueueReceive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", queue_receive_median, queue_receive_avg);

    // Test 2: Semaphore APIs
    म_लिखो("\n2. Testing Semaphore APIs...\n");

    // xSemaphoreGive
    क्रम (पूर्णांक i = 0; i < संख्या_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_गिनती();
        xSemaphoreGive(test_semaphore);
        execution_times[i] = esp_cpu_get_cycle_गिनती() - start_cycles;
    }
    uint32_t sem_give_median = calculate_median(execution_times, संख्या_OF_SAMPLES);
    uint32_t sem_give_avg = calculate_average(execution_times, संख्या_OF_SAMPLES);
    म_लिखो("   xSemaphoreGive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", sem_give_median, sem_give_avg);

    // xSemaphoreTake
    क्रम (पूर्णांक i = 0; i < संख्या_OF_SAMPLES; i++) {
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
    क्रम (पूर्णांक i = 0; i < संख्या_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_गिनती();
        xEventGroupSetBits(test_event_group, 0x01);
        execution_times[i] = esp_cpu_get_cycle_गिनती() - start_cycles;
    }
    uint32_t event_set_median = calculate_median(execution_times, संख्या_OF_SAMPLES);
    uint32_t event_set_avg = calculate_average(execution_times, संख्या_OF_SAMPLES);
    म_लिखो("   xEventGroupSetBits: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", event_set_median, event_set_avg);

    // xEventGroupGetBits
    क्रम (पूर्णांक i = 0; i < संख्या_OF_SAMPLES; i++) {
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
    क्रम (पूर्णांक i = 0; i < संख्या_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_गिनती();
        x_Stream_बफर_Send(test_stream_बफर, &test_तथ्य, माप(uint32_t), 0);
        execution_times[i] = esp_cpu_get_cycle_गिनती() - start_cycles;
    }
    uint32_t stream_send_median = calculate_median(execution_times, संख्या_OF_SAMPLES);
    uint32_t stream_send_avg = calculate_average(execution_times, संख्या_OF_SAMPLES);
    म_लिखो("   xStreamBufferSend: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", stream_send_median, stream_send_avg);

    // xStreamBufferReceive
    क्रम (पूर्णांक i = 0; i < संख्या_OF_SAMPLES; i++) {
        uint32_t start_cycles = esp_cpu_get_cycle_गिनती();
        x_Stream_बफर_Receive(test_stream_बफर, &test_तथ्य, माप(uint32_t), 0);
        execution_times[i] = esp_cpu_get_cycle_गिनती() - start_cycles;
    }
    uint32_t stream_receive_median = calculate_median(execution_times, संख्या_OF_SAMPLES);
    uint32_t stream_receive_avg = calculate_average(execution_times, संख्या_OF_SAMPLES);
    म_लिखो("   xStreamBufferReceive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", stream_receive_median, stream_receive_avg);

    // Test 5: Task APIs
    म_लिखो("\n5. Testing Task APIs...\n");

    // xTaskGetTickCount
    क्रम (पूर्णांक i = 0; i < संख्या_OF_SAMPLES; i++) {
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
    v_Stream_बफर_Delete(test_stream_बफर);
    vEventGroupDelete(test_event_group);
    vSemaphoreDelete(test_semaphore);
    vQueueDelete(test_queue);
}
