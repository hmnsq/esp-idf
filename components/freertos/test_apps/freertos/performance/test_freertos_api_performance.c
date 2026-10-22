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

#ਅਨ੍ਤਰ੍ਭਾਵ <esp_types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <inttypes.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/semphr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/queue.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/event_groups.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/stream_buffer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/timers.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_cpu.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "test_utils.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸੰਖਿਆ_ਦਾ_SAMPLES 1000

ਸਥਿਰ ਪੂਰਨ compare_uint32(ਅਚਲ ਖਾਲੀ *a, ਅਚਲ ਖਾਲੀ *b)
{
    ਵਾਪਸ (*(uint32_t *)a - * (uint32_t *)b);
}

ਸਥਿਰ uint32_t calculate_median(uint32_t *ਮੁੱਲਾਂ, ਪੂਰਨ ਮਾਪ)
{
    /* Sort the array of values containing the execution time of each iteration */
    qsort(ਮੁੱਲਾਂ, ਮਾਪ, ਆਕਾਰ(uint32_t), compare_uint32);
    /* Return the median value */
    ਵਾਪਸ ਮੁੱਲਾਂ[ਮਾਪ / 2];
}

ਸਥਿਰ uint32_t calculate_ਔਸਤਨ(uint32_t *ਮੁੱਲਾਂ, ਪੂਰਨ ਮਾਪ)
{
    uint64_t ਜੋੜ = 0;
    ਲਈ (ਪੂਰਨ i = 0; i < ਮਾਪ; i++) {
        ਜੋੜ += ਮੁੱਲਾਂ[i];
    }
    ਵਾਪਸ (uint32_t)(ਜੋੜ / ਮਾਪ);
}

TEST_CASE("freertos api performance test", "[freertos][ignore]")
{
    ਲਿਖੋ("=== FreeRTOS API Performance Test ===\n");
    ਲਿਖੋ("This test measures execution time of commonly used FreeRTOS APIs.\n");
    ਲਿਖੋ("Results help evaluate the impact of memory placement (Flash vs IRAM).\n\n");

#ਜੇ defined(CONFIG_FREERTOS_ਵਿੱਚ_IRAM)
    ਲਿਖੋ("FreeRTOS functions placed in IRAM.\r\n");
#ਨਹੀਂਤਾਂ
    ਲਿਖੋ("FreeRTOS functions placed in Flash.\r\n");
#ਜੇ_ਅੰਤ
    ਲਿਖੋ("Testing with %d samples per API\n", ਸੰਖਿਆ_ਦਾ_SAMPLES);
    ਲਿਖੋ("Note: Performance varies by target architecture, flash cache, and system load.\n\n");

    uint32_t execution_times[ਸੰਖਿਆ_ਦਾ_SAMPLES];
    uint32_t test_ਅੰਕੜੇ = 0x12345678;

    // Initialize test objects
    ਕਤਾਰ_Handle_t test_ਕਤਾਰ = x_ਕਤਾਰ_Create(10, ਆਕਾਰ(uint32_t));
    SemaphoreHandle_t test_semaphore = x_Semaphore_Create_ਦੁਆਧਾਰੀ();
    Event_ਸਮੂਹ_Handle_t test_event_ਸਮੂਹ = x_Event_ਸਮੂਹ_Create();
    ਧਾਰਾ_ਬਫਰ_Handle_t test_ਧਾਰਾ_ਬਫਰ = x_ਧਾਰਾ_ਬਫਰ_Create(1024, 1);

    TEST_ASSERT(test_ਕਤਾਰ != ਨਿਰਾ);
    TEST_ASSERT(test_semaphore != ਨਿਰਾ);
    TEST_ASSERT(test_event_ਸਮੂਹ != ਨਿਰਾ);
    TEST_ASSERT(test_ਧਾਰਾ_ਬਫਰ != ਨਿਰਾ);

    // Test 1: Queue APIs
    ਲਿਖੋ("1. Testing Queue APIs...\n");

    // xQueueSend
    ਲਈ (ਪੂਰਨ i = 0; i < ਸੰਖਿਆ_ਦਾ_SAMPLES; i++) {
        uint32_t ਸ਼ੁਰੂ_ਚੱਕਰਾਂ = esp_cpu_get_ਚੱਕਰ_ਗਿਣਤੀ();
        x_ਕਤਾਰ_Send(test_ਕਤਾਰ, &test_ਅੰਕੜੇ, 0);
        execution_times[i] = esp_cpu_get_ਚੱਕਰ_ਗਿਣਤੀ() - ਸ਼ੁਰੂ_ਚੱਕਰਾਂ;
    }
    uint32_t ਕਤਾਰ_send_median = calculate_median(execution_times, ਸੰਖਿਆ_ਦਾ_SAMPLES);
    uint32_t ਕਤਾਰ_send_avg = calculate_ਔਸਤਨ(execution_times, ਸੰਖਿਆ_ਦਾ_SAMPLES);
    ਲਿਖੋ("   xQueueSend: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", ਕਤਾਰ_send_median, ਕਤਾਰ_send_avg);

    // xQueueReceive
    ਲਈ (ਪੂਰਨ i = 0; i < ਸੰਖਿਆ_ਦਾ_SAMPLES; i++) {
        uint32_t ਸ਼ੁਰੂ_ਚੱਕਰਾਂ = esp_cpu_get_ਚੱਕਰ_ਗਿਣਤੀ();
        x_ਕਤਾਰ_Receive(test_ਕਤਾਰ, &test_ਅੰਕੜੇ, 0);
        execution_times[i] = esp_cpu_get_ਚੱਕਰ_ਗਿਣਤੀ() - ਸ਼ੁਰੂ_ਚੱਕਰਾਂ;
    }
    uint32_t ਕਤਾਰ_receive_median = calculate_median(execution_times, ਸੰਖਿਆ_ਦਾ_SAMPLES);
    uint32_t ਕਤਾਰ_receive_avg = calculate_ਔਸਤਨ(execution_times, ਸੰਖਿਆ_ਦਾ_SAMPLES);
    ਲਿਖੋ("   xQueueReceive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", ਕਤਾਰ_receive_median, ਕਤਾਰ_receive_avg);

    // Test 2: Semaphore APIs
    ਲਿਖੋ("\n2. Testing Semaphore APIs...\n");

    // xSemaphoreGive
    ਲਈ (ਪੂਰਨ i = 0; i < ਸੰਖਿਆ_ਦਾ_SAMPLES; i++) {
        uint32_t ਸ਼ੁਰੂ_ਚੱਕਰਾਂ = esp_cpu_get_ਚੱਕਰ_ਗਿਣਤੀ();
        xSemaphoreGive(test_semaphore);
        execution_times[i] = esp_cpu_get_ਚੱਕਰ_ਗਿਣਤੀ() - ਸ਼ੁਰੂ_ਚੱਕਰਾਂ;
    }
    uint32_t sem_give_median = calculate_median(execution_times, ਸੰਖਿਆ_ਦਾ_SAMPLES);
    uint32_t sem_give_avg = calculate_ਔਸਤਨ(execution_times, ਸੰਖਿਆ_ਦਾ_SAMPLES);
    ਲਿਖੋ("   xSemaphoreGive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", sem_give_median, sem_give_avg);

    // xSemaphoreTake
    ਲਈ (ਪੂਰਨ i = 0; i < ਸੰਖਿਆ_ਦਾ_SAMPLES; i++) {
        uint32_t ਸ਼ੁਰੂ_ਚੱਕਰਾਂ = esp_cpu_get_ਚੱਕਰ_ਗਿਣਤੀ();
        xSemaphoreTake(test_semaphore, 0);
        execution_times[i] = esp_cpu_get_ਚੱਕਰ_ਗਿਣਤੀ() - ਸ਼ੁਰੂ_ਚੱਕਰਾਂ;
    }
    uint32_t sem_take_median = calculate_median(execution_times, ਸੰਖਿਆ_ਦਾ_SAMPLES);
    uint32_t sem_take_avg = calculate_ਔਸਤਨ(execution_times, ਸੰਖਿਆ_ਦਾ_SAMPLES);
    ਲਿਖੋ("   xSemaphoreTake: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", sem_take_median, sem_take_avg);

    // Test 3: Event Group APIs
    ਲਿਖੋ("\n3. Testing Event Group APIs...\n");

    // xEventGroupSetBits
    ਲਈ (ਪੂਰਨ i = 0; i < ਸੰਖਿਆ_ਦਾ_SAMPLES; i++) {
        uint32_t ਸ਼ੁਰੂ_ਚੱਕਰਾਂ = esp_cpu_get_ਚੱਕਰ_ਗਿਣਤੀ();
        x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ(test_event_ਸਮੂਹ, 0x01);
        execution_times[i] = esp_cpu_get_ਚੱਕਰ_ਗਿਣਤੀ() - ਸ਼ੁਰੂ_ਚੱਕਰਾਂ;
    }
    uint32_t event_ਸੈੱਟ_median = calculate_median(execution_times, ਸੰਖਿਆ_ਦਾ_SAMPLES);
    uint32_t event_ਸੈੱਟ_avg = calculate_ਔਸਤਨ(execution_times, ਸੰਖਿਆ_ਦਾ_SAMPLES);
    ਲਿਖੋ("   xEventGroupSetBits: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", event_ਸੈੱਟ_median, event_ਸੈੱਟ_avg);

    // xEventGroupGetBits
    ਲਈ (ਪੂਰਨ i = 0; i < ਸੰਖਿਆ_ਦਾ_SAMPLES; i++) {
        uint32_t ਸ਼ੁਰੂ_ਚੱਕਰਾਂ = esp_cpu_get_ਚੱਕਰ_ਗਿਣਤੀ();
        x_Event_ਸਮੂਹ_Get_ਬਿੱਟ(test_event_ਸਮੂਹ);
        execution_times[i] = esp_cpu_get_ਚੱਕਰ_ਗਿਣਤੀ() - ਸ਼ੁਰੂ_ਚੱਕਰਾਂ;
    }
    uint32_t event_get_median = calculate_median(execution_times, ਸੰਖਿਆ_ਦਾ_SAMPLES);
    uint32_t event_get_avg = calculate_ਔਸਤਨ(execution_times, ਸੰਖਿਆ_ਦਾ_SAMPLES);
    ਲਿਖੋ("   xEventGroupGetBits: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", event_get_median, event_get_avg);

    // Test 4: Stream Buffer APIs
    ਲਿਖੋ("\n4. Testing Stream Buffer APIs...\n");

    // xStreamBufferSend
    ਲਈ (ਪੂਰਨ i = 0; i < ਸੰਖਿਆ_ਦਾ_SAMPLES; i++) {
        uint32_t ਸ਼ੁਰੂ_ਚੱਕਰਾਂ = esp_cpu_get_ਚੱਕਰ_ਗਿਣਤੀ();
        x_ਧਾਰਾ_ਬਫਰ_Send(test_ਧਾਰਾ_ਬਫਰ, &test_ਅੰਕੜੇ, ਆਕਾਰ(uint32_t), 0);
        execution_times[i] = esp_cpu_get_ਚੱਕਰ_ਗਿਣਤੀ() - ਸ਼ੁਰੂ_ਚੱਕਰਾਂ;
    }
    uint32_t ਧਾਰਾ_send_median = calculate_median(execution_times, ਸੰਖਿਆ_ਦਾ_SAMPLES);
    uint32_t ਧਾਰਾ_send_avg = calculate_ਔਸਤਨ(execution_times, ਸੰਖਿਆ_ਦਾ_SAMPLES);
    ਲਿਖੋ("   xStreamBufferSend: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", ਧਾਰਾ_send_median, ਧਾਰਾ_send_avg);

    // xStreamBufferReceive
    ਲਈ (ਪੂਰਨ i = 0; i < ਸੰਖਿਆ_ਦਾ_SAMPLES; i++) {
        uint32_t ਸ਼ੁਰੂ_ਚੱਕਰਾਂ = esp_cpu_get_ਚੱਕਰ_ਗਿਣਤੀ();
        x_ਧਾਰਾ_ਬਫਰ_Receive(test_ਧਾਰਾ_ਬਫਰ, &test_ਅੰਕੜੇ, ਆਕਾਰ(uint32_t), 0);
        execution_times[i] = esp_cpu_get_ਚੱਕਰ_ਗਿਣਤੀ() - ਸ਼ੁਰੂ_ਚੱਕਰਾਂ;
    }
    uint32_t ਧਾਰਾ_receive_median = calculate_median(execution_times, ਸੰਖਿਆ_ਦਾ_SAMPLES);
    uint32_t ਧਾਰਾ_receive_avg = calculate_ਔਸਤਨ(execution_times, ਸੰਖਿਆ_ਦਾ_SAMPLES);
    ਲਿਖੋ("   xStreamBufferReceive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", ਧਾਰਾ_receive_median, ਧਾਰਾ_receive_avg);

    // Test 5: Task APIs
    ਲਿਖੋ("\n5. Testing Task APIs...\n");

    // xTaskGetTickCount
    ਲਈ (ਪੂਰਨ i = 0; i < ਸੰਖਿਆ_ਦਾ_SAMPLES; i++) {
        uint32_t ਸ਼ੁਰੂ_ਚੱਕਰਾਂ = esp_cpu_get_ਚੱਕਰ_ਗਿਣਤੀ();
        x_Task_Get_Tick_ਗਿਣਤੀ();
        execution_times[i] = esp_cpu_get_ਚੱਕਰ_ਗਿਣਤੀ() - ਸ਼ੁਰੂ_ਚੱਕਰਾਂ;
    }
    uint32_t task_tick_median = calculate_median(execution_times, ਸੰਖਿਆ_ਦਾ_SAMPLES);
    uint32_t task_tick_avg = calculate_ਔਸਤਨ(execution_times, ਸੰਖਿਆ_ਦਾ_SAMPLES);
    ਲਿਖੋ("   xTaskGetTickCount: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", task_tick_median, task_tick_avg);

    ਲਿਖੋ("\n=== Performance Test Complete ===\n");
    ਲਿਖੋ("Note: Compare results between Flash and IRAM configurations.\n");
    ਲਿਖੋ("Consider memory cost vs performance benefit for your application.\n");

    // Cleanup
    v_ਧਾਰਾ_ਬਫਰ_Delete(test_ਧਾਰਾ_ਬਫਰ);
    v_Event_ਸਮੂਹ_Delete(test_event_ਸਮੂਹ);
    vSemaphoreDelete(test_semaphore);
    v_ਕਤਾਰ_Delete(test_ਕਤਾਰ);
}
