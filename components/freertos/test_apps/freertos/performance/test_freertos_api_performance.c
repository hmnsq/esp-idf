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

#ವ್ಯಾಖ್ಯಾನಿಸು ಸಂಖ್ಯೆ_ದ_SAMPLES 1000

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ compare_uint32(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *b)
{
    ಹಿಂತಿರುಗಿ (*(uint32_t *)a - * (uint32_t *)b);
}

ಸ್ಥಿರ uint32_t calculate_median(uint32_t *ಮೌಲ್ಯಗಳು, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2)
{
    /* Sort the array of values containing the execution time of each iteration */
    qsort(ಮೌಲ್ಯಗಳು, ಗಾತ್ರ_2, ಗಾತ್ರ(uint32_t), compare_uint32);
    /* Return the median value */
    ಹಿಂತಿರುಗಿ ಮೌಲ್ಯಗಳು[ಗಾತ್ರ_2 / 2];
}

ಸ್ಥಿರ uint32_t calculate_ಸರಾಸರಿ_ಮೌಲ್ಯ(uint32_t *ಮೌಲ್ಯಗಳು, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2)
{
    uint64_t ಮೊತ್ತ = 0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಗಾತ್ರ_2; i++) {
        ಮೊತ್ತ += ಮೌಲ್ಯಗಳು[i];
    }
    ಹಿಂತಿರುಗಿ (uint32_t)(ಮೊತ್ತ / ಗಾತ್ರ_2);
}

TEST_CASE("freertos api performance test", "[freertos][ignore]")
{
    ಬರೆ("=== FreeRTOS API Performance Test ===\n");
    ಬರೆ("This test measures execution time of commonly used FreeRTOS APIs.\n");
    ಬರೆ("Results help evaluate the impact of memory placement (Flash vs IRAM).\n\n");

#ಆದರೆ defined(CONFIG_FREERTOS_ಒಳ_IRAM)
    ಬರೆ("FreeRTOS functions placed in IRAM.\r\n");
#ಇಲ್ಲದಿದ್ದರೆ
    ಬರೆ("FreeRTOS functions placed in Flash.\r\n");
#ಆದರೆ_ಅಂತ್ಯ
    ಬರೆ("Testing with %d samples per API\n", ಸಂಖ್ಯೆ_ದ_SAMPLES);
    ಬರೆ("Note: Performance varies by target architecture, flash cache, and system load.\n\n");

    uint32_t execution_times[ಸಂಖ್ಯೆ_ದ_SAMPLES];
    uint32_t test_ದತ್ತಾಂಶ = 0x12345678;

    // Initialize test objects
    ಸರತಿ_Handle_t test_ಸರತಿ = x_ಸರತಿ_Create(10, ಗಾತ್ರ(uint32_t));
    SemaphoreHandle_t test_semaphore = x_Semaphore_Create_ದ್ವಿಮಾನ();
    Event_ಗುಂಪು_Handle_t test_event_ಗುಂಪು = x_Event_ಗುಂಪು_Create();
    ಹರಿವು_ಬಫರ್_Handle_t test_ಹರಿವು_ಬಫರ್ = x_ಹರಿವು_ಬಫರ್_Create(1024, 1);

    TEST_ASSERT(test_ಸರತಿ != ಖಾಲಿ);
    TEST_ASSERT(test_semaphore != ಖಾಲಿ);
    TEST_ASSERT(test_event_ಗುಂಪು != ಖಾಲಿ);
    TEST_ASSERT(test_ಹರಿವು_ಬಫರ್ != ಖಾಲಿ);

    // Test 1: Queue APIs
    ಬರೆ("1. Testing Queue APIs...\n");

    // xQueueSend
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಸಂಖ್ಯೆ_ದ_SAMPLES; i++) {
        uint32_t ಆರಂಭ_ಚಕ್ರಗಳು = esp_cpu_get_ಚಕ್ರ_ಎಣಿಕೆ();
        x_ಸರತಿ_Send(test_ಸರತಿ, &test_ದತ್ತಾಂಶ, 0);
        execution_times[i] = esp_cpu_get_ಚಕ್ರ_ಎಣಿಕೆ() - ಆರಂಭ_ಚಕ್ರಗಳು;
    }
    uint32_t ಸರತಿ_send_median = calculate_median(execution_times, ಸಂಖ್ಯೆ_ದ_SAMPLES);
    uint32_t ಸರತಿ_send_avg = calculate_ಸರಾಸರಿ_ಮೌಲ್ಯ(execution_times, ಸಂಖ್ಯೆ_ದ_SAMPLES);
    ಬರೆ("   xQueueSend: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", ಸರತಿ_send_median, ಸರತಿ_send_avg);

    // xQueueReceive
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಸಂಖ್ಯೆ_ದ_SAMPLES; i++) {
        uint32_t ಆರಂಭ_ಚಕ್ರಗಳು = esp_cpu_get_ಚಕ್ರ_ಎಣಿಕೆ();
        x_ಸರತಿ_Receive(test_ಸರತಿ, &test_ದತ್ತಾಂಶ, 0);
        execution_times[i] = esp_cpu_get_ಚಕ್ರ_ಎಣಿಕೆ() - ಆರಂಭ_ಚಕ್ರಗಳು;
    }
    uint32_t ಸರತಿ_receive_median = calculate_median(execution_times, ಸಂಖ್ಯೆ_ದ_SAMPLES);
    uint32_t ಸರತಿ_receive_avg = calculate_ಸರಾಸರಿ_ಮೌಲ್ಯ(execution_times, ಸಂಖ್ಯೆ_ದ_SAMPLES);
    ಬರೆ("   xQueueReceive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", ಸರತಿ_receive_median, ಸರತಿ_receive_avg);

    // Test 2: Semaphore APIs
    ಬರೆ("\n2. Testing Semaphore APIs...\n");

    // xSemaphoreGive
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಸಂಖ್ಯೆ_ದ_SAMPLES; i++) {
        uint32_t ಆರಂಭ_ಚಕ್ರಗಳು = esp_cpu_get_ಚಕ್ರ_ಎಣಿಕೆ();
        xSemaphoreGive(test_semaphore);
        execution_times[i] = esp_cpu_get_ಚಕ್ರ_ಎಣಿಕೆ() - ಆರಂಭ_ಚಕ್ರಗಳು;
    }
    uint32_t sem_give_median = calculate_median(execution_times, ಸಂಖ್ಯೆ_ದ_SAMPLES);
    uint32_t sem_give_avg = calculate_ಸರಾಸರಿ_ಮೌಲ್ಯ(execution_times, ಸಂಖ್ಯೆ_ದ_SAMPLES);
    ಬರೆ("   xSemaphoreGive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", sem_give_median, sem_give_avg);

    // xSemaphoreTake
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಸಂಖ್ಯೆ_ದ_SAMPLES; i++) {
        uint32_t ಆರಂಭ_ಚಕ್ರಗಳು = esp_cpu_get_ಚಕ್ರ_ಎಣಿಕೆ();
        xSemaphoreTake(test_semaphore, 0);
        execution_times[i] = esp_cpu_get_ಚಕ್ರ_ಎಣಿಕೆ() - ಆರಂಭ_ಚಕ್ರಗಳು;
    }
    uint32_t sem_take_median = calculate_median(execution_times, ಸಂಖ್ಯೆ_ದ_SAMPLES);
    uint32_t sem_take_avg = calculate_ಸರಾಸರಿ_ಮೌಲ್ಯ(execution_times, ಸಂಖ್ಯೆ_ದ_SAMPLES);
    ಬರೆ("   xSemaphoreTake: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", sem_take_median, sem_take_avg);

    // Test 3: Event Group APIs
    ಬರೆ("\n3. Testing Event Group APIs...\n");

    // xEventGroupSetBits
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಸಂಖ್ಯೆ_ದ_SAMPLES; i++) {
        uint32_t ಆರಂಭ_ಚಕ್ರಗಳು = esp_cpu_get_ಚಕ್ರ_ಎಣಿಕೆ();
        x_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು(test_event_ಗುಂಪು, 0x01);
        execution_times[i] = esp_cpu_get_ಚಕ್ರ_ಎಣಿಕೆ() - ಆರಂಭ_ಚಕ್ರಗಳು;
    }
    uint32_t event_ಗಣ_median = calculate_median(execution_times, ಸಂಖ್ಯೆ_ದ_SAMPLES);
    uint32_t event_ಗಣ_avg = calculate_ಸರಾಸರಿ_ಮೌಲ್ಯ(execution_times, ಸಂಖ್ಯೆ_ದ_SAMPLES);
    ಬರೆ("   xEventGroupSetBits: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", event_ಗಣ_median, event_ಗಣ_avg);

    // xEventGroupGetBits
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಸಂಖ್ಯೆ_ದ_SAMPLES; i++) {
        uint32_t ಆರಂಭ_ಚಕ್ರಗಳು = esp_cpu_get_ಚಕ್ರ_ಎಣಿಕೆ();
        x_Event_ಗುಂಪು_Get_ಬಿಟ್ಟುಗಳು(test_event_ಗುಂಪು);
        execution_times[i] = esp_cpu_get_ಚಕ್ರ_ಎಣಿಕೆ() - ಆರಂಭ_ಚಕ್ರಗಳು;
    }
    uint32_t event_get_median = calculate_median(execution_times, ಸಂಖ್ಯೆ_ದ_SAMPLES);
    uint32_t event_get_avg = calculate_ಸರಾಸರಿ_ಮೌಲ್ಯ(execution_times, ಸಂಖ್ಯೆ_ದ_SAMPLES);
    ಬರೆ("   xEventGroupGetBits: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", event_get_median, event_get_avg);

    // Test 4: Stream Buffer APIs
    ಬರೆ("\n4. Testing Stream Buffer APIs...\n");

    // xStreamBufferSend
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಸಂಖ್ಯೆ_ದ_SAMPLES; i++) {
        uint32_t ಆರಂಭ_ಚಕ್ರಗಳು = esp_cpu_get_ಚಕ್ರ_ಎಣಿಕೆ();
        x_ಹರಿವು_ಬಫರ್_Send(test_ಹರಿವು_ಬಫರ್, &test_ದತ್ತಾಂಶ, ಗಾತ್ರ(uint32_t), 0);
        execution_times[i] = esp_cpu_get_ಚಕ್ರ_ಎಣಿಕೆ() - ಆರಂಭ_ಚಕ್ರಗಳು;
    }
    uint32_t ಹರಿವು_send_median = calculate_median(execution_times, ಸಂಖ್ಯೆ_ದ_SAMPLES);
    uint32_t ಹರಿವು_send_avg = calculate_ಸರಾಸರಿ_ಮೌಲ್ಯ(execution_times, ಸಂಖ್ಯೆ_ದ_SAMPLES);
    ಬರೆ("   xStreamBufferSend: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", ಹರಿವು_send_median, ಹರಿವು_send_avg);

    // xStreamBufferReceive
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಸಂಖ್ಯೆ_ದ_SAMPLES; i++) {
        uint32_t ಆರಂಭ_ಚಕ್ರಗಳು = esp_cpu_get_ಚಕ್ರ_ಎಣಿಕೆ();
        x_ಹರಿವು_ಬಫರ್_Receive(test_ಹರಿವು_ಬಫರ್, &test_ದತ್ತಾಂಶ, ಗಾತ್ರ(uint32_t), 0);
        execution_times[i] = esp_cpu_get_ಚಕ್ರ_ಎಣಿಕೆ() - ಆರಂಭ_ಚಕ್ರಗಳು;
    }
    uint32_t ಹರಿವು_receive_median = calculate_median(execution_times, ಸಂಖ್ಯೆ_ದ_SAMPLES);
    uint32_t ಹರಿವು_receive_avg = calculate_ಸರಾಸರಿ_ಮೌಲ್ಯ(execution_times, ಸಂಖ್ಯೆ_ದ_SAMPLES);
    ಬರೆ("   xStreamBufferReceive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", ಹರಿವು_receive_median, ಹರಿವು_receive_avg);

    // Test 5: Task APIs
    ಬರೆ("\n5. Testing Task APIs...\n");

    // xTaskGetTickCount
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಸಂಖ್ಯೆ_ದ_SAMPLES; i++) {
        uint32_t ಆರಂಭ_ಚಕ್ರಗಳು = esp_cpu_get_ಚಕ್ರ_ಎಣಿಕೆ();
        x_Task_Get_Tick_ಎಣಿಕೆ();
        execution_times[i] = esp_cpu_get_ಚಕ್ರ_ಎಣಿಕೆ() - ಆರಂಭ_ಚಕ್ರಗಳು;
    }
    uint32_t task_tick_median = calculate_median(execution_times, ಸಂಖ್ಯೆ_ದ_SAMPLES);
    uint32_t task_tick_avg = calculate_ಸರಾಸರಿ_ಮೌಲ್ಯ(execution_times, ಸಂಖ್ಯೆ_ದ_SAMPLES);
    ಬರೆ("   xTaskGetTickCount: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", task_tick_median, task_tick_avg);

    ಬರೆ("\n=== Performance Test Complete ===\n");
    ಬರೆ("Note: Compare results between Flash and IRAM configurations.\n");
    ಬರೆ("Consider memory cost vs performance benefit for your application.\n");

    // Cleanup
    v_ಹರಿವು_ಬಫರ್_Delete(test_ಹರಿವು_ಬಫರ್);
    v_Event_ಗುಂಪು_Delete(test_event_ಗುಂಪು);
    vSemaphoreDelete(test_semaphore);
    v_ಸರತಿ_Delete(test_ಸರತಿ);
}
