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

#وصف_ڪريو عدد_جو_SAMPLES 1000

ساكن صحيح compare_uint32(ثابت فراغ *a, ثابت فراغ *b)
{
    أرجع (*(uint32_t *)a - * (uint32_t *)b);
}

ساكن uint32_t calculate_median(uint32_t *قدرون, صحيح سائيز)
{
    /* Sort the array of values containing the execution time of each iteration */
    qsort(قدرون, سائيز, حجم(uint32_t), compare_uint32);
    /* Return the median value */
    أرجع قدرون[سائيز / 2];
}

ساكن uint32_t calculate_اوسط(uint32_t *قدرون, صحيح سائيز)
{
    uint64_t جوڙ = 0;
    لكل (صحيح i = 0; i < سائيز; i++) {
        جوڙ += قدرون[i];
    }
    أرجع (uint32_t)(جوڙ / سائيز);
}

TEST_CASE("freertos api performance test", "[freertos][ignore]")
{
    لکو("=== FreeRTOS API Performance Test ===\n");
    لکو("This test measures execution time of commonly used FreeRTOS APIs.\n");
    لکو("Results help evaluate the impact of memory placement (Flash vs IRAM).\n\n");

#إذا defined(CONFIG_FREERTOS_اندريون_IRAM)
    لکو("FreeRTOS functions placed in IRAM.\r\n");
#وإلا
    لکو("FreeRTOS functions placed in Flash.\r\n");
#جيڪڏهن_پورو
    لکو("Testing with %d samples per API\n", عدد_جو_SAMPLES);
    لکو("Note: Performance varies by target architecture, flash cache, and system load.\n\n");

    uint32_t execution_times[عدد_جو_SAMPLES];
    uint32_t test_معلومات = 0x12345678;

    // Initialize test objects
    قطار_Handle_t test_قطار = x_قطار_Create(10, حجم(uint32_t));
    SemaphoreHandle_t test_semaphore = x_Semaphore_Create_ٻٽو();
    Event_گروهه_Handle_t test_event_گروهه = x_Event_گروهه_Create();
    وهڪرو_بفر_Handle_t test_وهڪرو_بفر = x_وهڪرو_بفر_Create(1024, 1);

    TEST_ASSERT(test_قطار != عدم);
    TEST_ASSERT(test_semaphore != عدم);
    TEST_ASSERT(test_event_گروهه != عدم);
    TEST_ASSERT(test_وهڪرو_بفر != عدم);

    // Test 1: Queue APIs
    لکو("1. Testing Queue APIs...\n");

    // xQueueSend
    لكل (صحيح i = 0; i < عدد_جو_SAMPLES; i++) {
        uint32_t شروعات_چڪرن = esp_cpu_get_چڪر_ڳڻپ();
        x_قطار_Send(test_قطار, &test_معلومات, 0);
        execution_times[i] = esp_cpu_get_چڪر_ڳڻپ() - شروعات_چڪرن;
    }
    uint32_t قطار_send_median = calculate_median(execution_times, عدد_جو_SAMPLES);
    uint32_t قطار_send_avg = calculate_اوسط(execution_times, عدد_جو_SAMPLES);
    لکو("   xQueueSend: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", قطار_send_median, قطار_send_avg);

    // xQueueReceive
    لكل (صحيح i = 0; i < عدد_جو_SAMPLES; i++) {
        uint32_t شروعات_چڪرن = esp_cpu_get_چڪر_ڳڻپ();
        x_قطار_Receive(test_قطار, &test_معلومات, 0);
        execution_times[i] = esp_cpu_get_چڪر_ڳڻپ() - شروعات_چڪرن;
    }
    uint32_t قطار_receive_median = calculate_median(execution_times, عدد_جو_SAMPLES);
    uint32_t قطار_receive_avg = calculate_اوسط(execution_times, عدد_جو_SAMPLES);
    لکو("   xQueueReceive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", قطار_receive_median, قطار_receive_avg);

    // Test 2: Semaphore APIs
    لکو("\n2. Testing Semaphore APIs...\n");

    // xSemaphoreGive
    لكل (صحيح i = 0; i < عدد_جو_SAMPLES; i++) {
        uint32_t شروعات_چڪرن = esp_cpu_get_چڪر_ڳڻپ();
        xSemaphoreGive(test_semaphore);
        execution_times[i] = esp_cpu_get_چڪر_ڳڻپ() - شروعات_چڪرن;
    }
    uint32_t sem_give_median = calculate_median(execution_times, عدد_جو_SAMPLES);
    uint32_t sem_give_avg = calculate_اوسط(execution_times, عدد_جو_SAMPLES);
    لکو("   xSemaphoreGive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", sem_give_median, sem_give_avg);

    // xSemaphoreTake
    لكل (صحيح i = 0; i < عدد_جو_SAMPLES; i++) {
        uint32_t شروعات_چڪرن = esp_cpu_get_چڪر_ڳڻپ();
        xSemaphoreTake(test_semaphore, 0);
        execution_times[i] = esp_cpu_get_چڪر_ڳڻپ() - شروعات_چڪرن;
    }
    uint32_t sem_take_median = calculate_median(execution_times, عدد_جو_SAMPLES);
    uint32_t sem_take_avg = calculate_اوسط(execution_times, عدد_جو_SAMPLES);
    لکو("   xSemaphoreTake: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", sem_take_median, sem_take_avg);

    // Test 3: Event Group APIs
    لکو("\n3. Testing Event Group APIs...\n");

    // xEventGroupSetBits
    لكل (صحيح i = 0; i < عدد_جو_SAMPLES; i++) {
        uint32_t شروعات_چڪرن = esp_cpu_get_چڪر_ڳڻپ();
        x_Event_گروهه_سيٽ_بٽ(test_event_گروهه, 0x01);
        execution_times[i] = esp_cpu_get_چڪر_ڳڻپ() - شروعات_چڪرن;
    }
    uint32_t event_سيٽ_median = calculate_median(execution_times, عدد_جو_SAMPLES);
    uint32_t event_سيٽ_avg = calculate_اوسط(execution_times, عدد_جو_SAMPLES);
    لکو("   xEventGroupSetBits: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", event_سيٽ_median, event_سيٽ_avg);

    // xEventGroupGetBits
    لكل (صحيح i = 0; i < عدد_جو_SAMPLES; i++) {
        uint32_t شروعات_چڪرن = esp_cpu_get_چڪر_ڳڻپ();
        x_Event_گروهه_Get_بٽ(test_event_گروهه);
        execution_times[i] = esp_cpu_get_چڪر_ڳڻپ() - شروعات_چڪرن;
    }
    uint32_t event_get_median = calculate_median(execution_times, عدد_جو_SAMPLES);
    uint32_t event_get_avg = calculate_اوسط(execution_times, عدد_جو_SAMPLES);
    لکو("   xEventGroupGetBits: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", event_get_median, event_get_avg);

    // Test 4: Stream Buffer APIs
    لکو("\n4. Testing Stream Buffer APIs...\n");

    // xStreamBufferSend
    لكل (صحيح i = 0; i < عدد_جو_SAMPLES; i++) {
        uint32_t شروعات_چڪرن = esp_cpu_get_چڪر_ڳڻپ();
        x_وهڪرو_بفر_Send(test_وهڪرو_بفر, &test_معلومات, حجم(uint32_t), 0);
        execution_times[i] = esp_cpu_get_چڪر_ڳڻپ() - شروعات_چڪرن;
    }
    uint32_t وهڪرو_send_median = calculate_median(execution_times, عدد_جو_SAMPLES);
    uint32_t وهڪرو_send_avg = calculate_اوسط(execution_times, عدد_جو_SAMPLES);
    لکو("   xStreamBufferSend: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", وهڪرو_send_median, وهڪرو_send_avg);

    // xStreamBufferReceive
    لكل (صحيح i = 0; i < عدد_جو_SAMPLES; i++) {
        uint32_t شروعات_چڪرن = esp_cpu_get_چڪر_ڳڻپ();
        x_وهڪرو_بفر_Receive(test_وهڪرو_بفر, &test_معلومات, حجم(uint32_t), 0);
        execution_times[i] = esp_cpu_get_چڪر_ڳڻپ() - شروعات_چڪرن;
    }
    uint32_t وهڪرو_receive_median = calculate_median(execution_times, عدد_جو_SAMPLES);
    uint32_t وهڪرو_receive_avg = calculate_اوسط(execution_times, عدد_جو_SAMPLES);
    لکو("   xStreamBufferReceive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", وهڪرو_receive_median, وهڪرو_receive_avg);

    // Test 5: Task APIs
    لکو("\n5. Testing Task APIs...\n");

    // xTaskGetTickCount
    لكل (صحيح i = 0; i < عدد_جو_SAMPLES; i++) {
        uint32_t شروعات_چڪرن = esp_cpu_get_چڪر_ڳڻپ();
        x_Task_Get_Tick_ڳڻپ();
        execution_times[i] = esp_cpu_get_چڪر_ڳڻپ() - شروعات_چڪرن;
    }
    uint32_t task_tick_median = calculate_median(execution_times, عدد_جو_SAMPLES);
    uint32_t task_tick_avg = calculate_اوسط(execution_times, عدد_جو_SAMPLES);
    لکو("   xTaskGetTickCount: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", task_tick_median, task_tick_avg);

    لکو("\n=== Performance Test Complete ===\n");
    لکو("Note: Compare results between Flash and IRAM configurations.\n");
    لکو("Consider memory cost vs performance benefit for your application.\n");

    // Cleanup
    v_وهڪرو_بفر_Delete(test_وهڪرو_بفر);
    v_Event_گروهه_Delete(test_event_گروهه);
    vSemaphoreDelete(test_semaphore);
    v_قطار_Delete(test_قطار);
}
