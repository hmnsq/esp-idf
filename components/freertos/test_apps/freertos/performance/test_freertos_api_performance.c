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

#परिभाषय सङ्ख्या_सम्बन्धि_SAMPLES 1000

स्थिर पूर्णाङ्क compare_uint32(ध्रुव शून्य *a, ध्रुव शून्य *b)
{
    प्रत्यागम (*(uint32_t *)a - * (uint32_t *)b);
}

स्थिर uint32_t calculate_median(uint32_t *मानानि, पूर्णाङ्क आकार)
{
    /* Sort the array of values containing the execution time of each iteration */
    शीघ्रं_क्रमय(मानानि, आकार, परिमाण(uint32_t), compare_uint32);
    /* Return the median value */
    प्रत्यागम मानानि[आकार / 2];
}

स्थिर uint32_t calculate_मध्यमानम्(uint32_t *मानानि, पूर्णाङ्क आकार)
{
    uint64_t योग_2 = 0;
    प्रति (पूर्णाङ्क i = 0; i < आकार; i++) {
        योग_2 += मानानि[i];
    }
    प्रत्यागम (uint32_t)(योग_2 / आकार);
}

TEST_CASE("freertos api performance test", "[freertos][ignore]")
{
    प्रारूप_लिख("=== FreeRTOS API Performance Test ===\n");
    प्रारूप_लिख("This test measures execution time of commonly used FreeRTOS APIs.\n");
    प्रारूप_लिख("Results help evaluate the impact of memory placement (Flash vs IRAM).\n\n");

#यदि defined(CONFIG_FREERTOS_अन्तर्_IRAM)
    प्रारूप_लिख("FreeRTOS functions placed in IRAM.\r\n");
#अन्यथा
    प्रारूप_लिख("FreeRTOS functions placed in Flash.\r\n");
#यदि_अन्तः
    प्रारूप_लिख("Testing with %d samples per API\n", सङ्ख्या_सम्बन्धि_SAMPLES);
    प्रारूप_लिख("Note: Performance varies by target architecture, flash cache, and system load.\n\n");

    uint32_t execution_times[सङ्ख्या_सम्बन्धि_SAMPLES];
    uint32_t test_दत्त = 0x12345678;

    // Initialize test objects
    QueueHandle_t test_queue = xQueueCreate(10, परिमाण(uint32_t));
    SemaphoreHandle_t test_semaphore = x_Semaphore_Create_द्विमान();
    Event_गणः_Handle_t test_event_गणः = x_Event_गणः_Create();
    प्रवाहः_बफर_Handle_t test_प्रवाहः_बफर = x_प्रवाहः_बफर_Create(1024, 1);

    TEST_ASSERT(test_queue != रिक्त);
    TEST_ASSERT(test_semaphore != रिक्त);
    TEST_ASSERT(test_event_गणः != रिक्त);
    TEST_ASSERT(test_प्रवाहः_बफर != रिक्त);

    // Test 1: Queue APIs
    प्रारूप_लिख("1. Testing Queue APIs...\n");

    // xQueueSend
    प्रति (पूर्णाङ्क i = 0; i < सङ्ख्या_सम्बन्धि_SAMPLES; i++) {
        uint32_t start_आवृत्तयः = esp_cpu_get_आवृत्तिः_गणनम्();
        xQueueSend(test_queue, &test_दत्त, 0);
        execution_times[i] = esp_cpu_get_आवृत्तिः_गणनम्() - start_आवृत्तयः;
    }
    uint32_t queue_send_median = calculate_median(execution_times, सङ्ख्या_सम्बन्धि_SAMPLES);
    uint32_t queue_send_avg = calculate_मध्यमानम्(execution_times, सङ्ख्या_सम्बन्धि_SAMPLES);
    प्रारूप_लिख("   xQueueSend: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", queue_send_median, queue_send_avg);

    // xQueueReceive
    प्रति (पूर्णाङ्क i = 0; i < सङ्ख्या_सम्बन्धि_SAMPLES; i++) {
        uint32_t start_आवृत्तयः = esp_cpu_get_आवृत्तिः_गणनम्();
        xQueueReceive(test_queue, &test_दत्त, 0);
        execution_times[i] = esp_cpu_get_आवृत्तिः_गणनम्() - start_आवृत्तयः;
    }
    uint32_t queue_receive_median = calculate_median(execution_times, सङ्ख्या_सम्बन्धि_SAMPLES);
    uint32_t queue_receive_avg = calculate_मध्यमानम्(execution_times, सङ्ख्या_सम्बन्धि_SAMPLES);
    प्रारूप_लिख("   xQueueReceive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", queue_receive_median, queue_receive_avg);

    // Test 2: Semaphore APIs
    प्रारूप_लिख("\n2. Testing Semaphore APIs...\n");

    // xSemaphoreGive
    प्रति (पूर्णाङ्क i = 0; i < सङ्ख्या_सम्बन्धि_SAMPLES; i++) {
        uint32_t start_आवृत्तयः = esp_cpu_get_आवृत्तिः_गणनम्();
        xSemaphoreGive(test_semaphore);
        execution_times[i] = esp_cpu_get_आवृत्तिः_गणनम्() - start_आवृत्तयः;
    }
    uint32_t sem_give_median = calculate_median(execution_times, सङ्ख्या_सम्बन्धि_SAMPLES);
    uint32_t sem_give_avg = calculate_मध्यमानम्(execution_times, सङ्ख्या_सम्बन्धि_SAMPLES);
    प्रारूप_लिख("   xSemaphoreGive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", sem_give_median, sem_give_avg);

    // xSemaphoreTake
    प्रति (पूर्णाङ्क i = 0; i < सङ्ख्या_सम्बन्धि_SAMPLES; i++) {
        uint32_t start_आवृत्तयः = esp_cpu_get_आवृत्तिः_गणनम्();
        xSemaphoreTake(test_semaphore, 0);
        execution_times[i] = esp_cpu_get_आवृत्तिः_गणनम्() - start_आवृत्तयः;
    }
    uint32_t sem_take_median = calculate_median(execution_times, सङ्ख्या_सम्बन्धि_SAMPLES);
    uint32_t sem_take_avg = calculate_मध्यमानम्(execution_times, सङ्ख्या_सम्बन्धि_SAMPLES);
    प्रारूप_लिख("   xSemaphoreTake: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", sem_take_median, sem_take_avg);

    // Test 3: Event Group APIs
    प्रारूप_लिख("\n3. Testing Event Group APIs...\n");

    // xEventGroupSetBits
    प्रति (पूर्णाङ्क i = 0; i < सङ्ख्या_सम्बन्धि_SAMPLES; i++) {
        uint32_t start_आवृत्तयः = esp_cpu_get_आवृत्तिः_गणनम्();
        x_Event_गणः_समुच्चयः_द्वयङ्काः(test_event_गणः, 0x01);
        execution_times[i] = esp_cpu_get_आवृत्तिः_गणनम्() - start_आवृत्तयः;
    }
    uint32_t event_समुच्चयः_median = calculate_median(execution_times, सङ्ख्या_सम्बन्धि_SAMPLES);
    uint32_t event_समुच्चयः_avg = calculate_मध्यमानम्(execution_times, सङ्ख्या_सम्बन्धि_SAMPLES);
    प्रारूप_लिख("   xEventGroupSetBits: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", event_समुच्चयः_median, event_समुच्चयः_avg);

    // xEventGroupGetBits
    प्रति (पूर्णाङ्क i = 0; i < सङ्ख्या_सम्बन्धि_SAMPLES; i++) {
        uint32_t start_आवृत्तयः = esp_cpu_get_आवृत्तिः_गणनम्();
        x_Event_गणः_Get_द्वयङ्काः(test_event_गणः);
        execution_times[i] = esp_cpu_get_आवृत्तिः_गणनम्() - start_आवृत्तयः;
    }
    uint32_t event_get_median = calculate_median(execution_times, सङ्ख्या_सम्बन्धि_SAMPLES);
    uint32_t event_get_avg = calculate_मध्यमानम्(execution_times, सङ्ख्या_सम्बन्धि_SAMPLES);
    प्रारूप_लिख("   xEventGroupGetBits: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", event_get_median, event_get_avg);

    // Test 4: Stream Buffer APIs
    प्रारूप_लिख("\n4. Testing Stream Buffer APIs...\n");

    // xStreamBufferSend
    प्रति (पूर्णाङ्क i = 0; i < सङ्ख्या_सम्बन्धि_SAMPLES; i++) {
        uint32_t start_आवृत्तयः = esp_cpu_get_आवृत्तिः_गणनम्();
        x_प्रवाहः_बफर_Send(test_प्रवाहः_बफर, &test_दत्त, परिमाण(uint32_t), 0);
        execution_times[i] = esp_cpu_get_आवृत्तिः_गणनम्() - start_आवृत्तयः;
    }
    uint32_t प्रवाहः_send_median = calculate_median(execution_times, सङ्ख्या_सम्बन्धि_SAMPLES);
    uint32_t प्रवाहः_send_avg = calculate_मध्यमानम्(execution_times, सङ्ख्या_सम्बन्धि_SAMPLES);
    प्रारूप_लिख("   xStreamBufferSend: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", प्रवाहः_send_median, प्रवाहः_send_avg);

    // xStreamBufferReceive
    प्रति (पूर्णाङ्क i = 0; i < सङ्ख्या_सम्बन्धि_SAMPLES; i++) {
        uint32_t start_आवृत्तयः = esp_cpu_get_आवृत्तिः_गणनम्();
        x_प्रवाहः_बफर_Receive(test_प्रवाहः_बफर, &test_दत्त, परिमाण(uint32_t), 0);
        execution_times[i] = esp_cpu_get_आवृत्तिः_गणनम्() - start_आवृत्तयः;
    }
    uint32_t प्रवाहः_receive_median = calculate_median(execution_times, सङ्ख्या_सम्बन्धि_SAMPLES);
    uint32_t प्रवाहः_receive_avg = calculate_मध्यमानम्(execution_times, सङ्ख्या_सम्बन्धि_SAMPLES);
    प्रारूप_लिख("   xStreamBufferReceive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", प्रवाहः_receive_median, प्रवाहः_receive_avg);

    // Test 5: Task APIs
    प्रारूप_लिख("\n5. Testing Task APIs...\n");

    // xTaskGetTickCount
    प्रति (पूर्णाङ्क i = 0; i < सङ्ख्या_सम्बन्धि_SAMPLES; i++) {
        uint32_t start_आवृत्तयः = esp_cpu_get_आवृत्तिः_गणनम्();
        x_Task_Get_Tick_गणनम्();
        execution_times[i] = esp_cpu_get_आवृत्तिः_गणनम्() - start_आवृत्तयः;
    }
    uint32_t task_tick_median = calculate_median(execution_times, सङ्ख्या_सम्बन्धि_SAMPLES);
    uint32_t task_tick_avg = calculate_मध्यमानम्(execution_times, सङ्ख्या_सम्बन्धि_SAMPLES);
    प्रारूप_लिख("   xTaskGetTickCount: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", task_tick_median, task_tick_avg);

    प्रारूप_लिख("\n=== Performance Test Complete ===\n");
    प्रारूप_लिख("Note: Compare results between Flash and IRAM configurations.\n");
    प्रारूप_लिख("Consider memory cost vs performance benefit for your application.\n");

    // Cleanup
    v_प्रवाहः_बफर_Delete(test_प्रवाहः_बफर);
    v_Event_गणः_Delete(test_event_गणः);
    vSemaphoreDelete(test_semaphore);
    vQueueDelete(test_queue);
}
