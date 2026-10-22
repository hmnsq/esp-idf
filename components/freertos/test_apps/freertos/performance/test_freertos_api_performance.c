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

#تعریف_کرو نمبر_دا_SAMPLES 1000

ساكن صحيح compare_uint32(ثابت فراغ *a, ثابت فراغ *b)
{
    أرجع (*(uint32_t *)a - * (uint32_t *)b);
}

ساكن uint32_t calculate_median(uint32_t *اقدار, صحيح ناپ)
{
    /* Sort the array of values containing the execution time of each iteration */
    qsort(اقدار, ناپ, حجم(uint32_t), compare_uint32);
    /* Return the median value */
    أرجع اقدار[ناپ / 2];
}

ساكن uint32_t calculate_اوسطن(uint32_t *اقدار, صحيح ناپ)
{
    uint64_t جوڑ = 0;
    لكل (صحيح i = 0; i < ناپ; i++) {
        جوڑ += اقدار[i];
    }
    أرجع (uint32_t)(جوڑ / ناپ);
}

TEST_CASE("freertos api performance test", "[freertos][ignore]")
{
    لکھو("=== FreeRTOS API Performance Test ===\n");
    لکھو("This test measures execution time of commonly used FreeRTOS APIs.\n");
    لکھو("Results help evaluate the impact of memory placement (Flash vs IRAM).\n\n");

#إذا defined(CONFIG_FREERTOS_وچ_IRAM)
    لکھو("FreeRTOS functions placed in IRAM.\r\n");
#وإلا
    لکھو("FreeRTOS functions placed in Flash.\r\n");
#جے_انت
    لکھو("Testing with %d samples per API\n", نمبر_دا_SAMPLES);
    لکھو("Note: Performance varies by target architecture, flash cache, and system load.\n\n");

    uint32_t execution_times[نمبر_دا_SAMPLES];
    uint32_t test_انکڑے = 0x12345678;

    // Initialize test objects
    قطار_Handle_t test_قطار = x_قطار_Create(10, حجم(uint32_t));
    SemaphoreHandle_t test_semaphore = x_Semaphore_Create_ثنائی();
    Event_گروہ_Handle_t test_event_گروہ = x_Event_گروہ_Create();
    دھارا_بفر_Handle_t test_دھارا_بفر = x_دھارا_بفر_Create(1024, 1);

    TEST_ASSERT(test_قطار != عدم);
    TEST_ASSERT(test_semaphore != عدم);
    TEST_ASSERT(test_event_گروہ != عدم);
    TEST_ASSERT(test_دھارا_بفر != عدم);

    // Test 1: Queue APIs
    لکھو("1. Testing Queue APIs...\n");

    // xQueueSend
    لكل (صحيح i = 0; i < نمبر_دا_SAMPLES; i++) {
        uint32_t شروع_چکراں = esp_cpu_get_چکر_گنتی();
        x_قطار_Send(test_قطار, &test_انکڑے, 0);
        execution_times[i] = esp_cpu_get_چکر_گنتی() - شروع_چکراں;
    }
    uint32_t قطار_send_median = calculate_median(execution_times, نمبر_دا_SAMPLES);
    uint32_t قطار_send_avg = calculate_اوسطن(execution_times, نمبر_دا_SAMPLES);
    لکھو("   xQueueSend: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", قطار_send_median, قطار_send_avg);

    // xQueueReceive
    لكل (صحيح i = 0; i < نمبر_دا_SAMPLES; i++) {
        uint32_t شروع_چکراں = esp_cpu_get_چکر_گنتی();
        x_قطار_Receive(test_قطار, &test_انکڑے, 0);
        execution_times[i] = esp_cpu_get_چکر_گنتی() - شروع_چکراں;
    }
    uint32_t قطار_receive_median = calculate_median(execution_times, نمبر_دا_SAMPLES);
    uint32_t قطار_receive_avg = calculate_اوسطن(execution_times, نمبر_دا_SAMPLES);
    لکھو("   xQueueReceive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", قطار_receive_median, قطار_receive_avg);

    // Test 2: Semaphore APIs
    لکھو("\n2. Testing Semaphore APIs...\n");

    // xSemaphoreGive
    لكل (صحيح i = 0; i < نمبر_دا_SAMPLES; i++) {
        uint32_t شروع_چکراں = esp_cpu_get_چکر_گنتی();
        xSemaphoreGive(test_semaphore);
        execution_times[i] = esp_cpu_get_چکر_گنتی() - شروع_چکراں;
    }
    uint32_t sem_give_median = calculate_median(execution_times, نمبر_دا_SAMPLES);
    uint32_t sem_give_avg = calculate_اوسطن(execution_times, نمبر_دا_SAMPLES);
    لکھو("   xSemaphoreGive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", sem_give_median, sem_give_avg);

    // xSemaphoreTake
    لكل (صحيح i = 0; i < نمبر_دا_SAMPLES; i++) {
        uint32_t شروع_چکراں = esp_cpu_get_چکر_گنتی();
        xSemaphoreTake(test_semaphore, 0);
        execution_times[i] = esp_cpu_get_چکر_گنتی() - شروع_چکراں;
    }
    uint32_t sem_take_median = calculate_median(execution_times, نمبر_دا_SAMPLES);
    uint32_t sem_take_avg = calculate_اوسطن(execution_times, نمبر_دا_SAMPLES);
    لکھو("   xSemaphoreTake: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", sem_take_median, sem_take_avg);

    // Test 3: Event Group APIs
    لکھو("\n3. Testing Event Group APIs...\n");

    // xEventGroupSetBits
    لكل (صحيح i = 0; i < نمبر_دا_SAMPLES; i++) {
        uint32_t شروع_چکراں = esp_cpu_get_چکر_گنتی();
        x_Event_گروہ_سیٹ_بٹ(test_event_گروہ, 0x01);
        execution_times[i] = esp_cpu_get_چکر_گنتی() - شروع_چکراں;
    }
    uint32_t event_سیٹ_median = calculate_median(execution_times, نمبر_دا_SAMPLES);
    uint32_t event_سیٹ_avg = calculate_اوسطن(execution_times, نمبر_دا_SAMPLES);
    لکھو("   xEventGroupSetBits: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", event_سیٹ_median, event_سیٹ_avg);

    // xEventGroupGetBits
    لكل (صحيح i = 0; i < نمبر_دا_SAMPLES; i++) {
        uint32_t شروع_چکراں = esp_cpu_get_چکر_گنتی();
        x_Event_گروہ_Get_بٹ(test_event_گروہ);
        execution_times[i] = esp_cpu_get_چکر_گنتی() - شروع_چکراں;
    }
    uint32_t event_get_median = calculate_median(execution_times, نمبر_دا_SAMPLES);
    uint32_t event_get_avg = calculate_اوسطن(execution_times, نمبر_دا_SAMPLES);
    لکھو("   xEventGroupGetBits: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", event_get_median, event_get_avg);

    // Test 4: Stream Buffer APIs
    لکھو("\n4. Testing Stream Buffer APIs...\n");

    // xStreamBufferSend
    لكل (صحيح i = 0; i < نمبر_دا_SAMPLES; i++) {
        uint32_t شروع_چکراں = esp_cpu_get_چکر_گنتی();
        x_دھارا_بفر_Send(test_دھارا_بفر, &test_انکڑے, حجم(uint32_t), 0);
        execution_times[i] = esp_cpu_get_چکر_گنتی() - شروع_چکراں;
    }
    uint32_t دھارا_send_median = calculate_median(execution_times, نمبر_دا_SAMPLES);
    uint32_t دھارا_send_avg = calculate_اوسطن(execution_times, نمبر_دا_SAMPLES);
    لکھو("   xStreamBufferSend: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", دھارا_send_median, دھارا_send_avg);

    // xStreamBufferReceive
    لكل (صحيح i = 0; i < نمبر_دا_SAMPLES; i++) {
        uint32_t شروع_چکراں = esp_cpu_get_چکر_گنتی();
        x_دھارا_بفر_Receive(test_دھارا_بفر, &test_انکڑے, حجم(uint32_t), 0);
        execution_times[i] = esp_cpu_get_چکر_گنتی() - شروع_چکراں;
    }
    uint32_t دھارا_receive_median = calculate_median(execution_times, نمبر_دا_SAMPLES);
    uint32_t دھارا_receive_avg = calculate_اوسطن(execution_times, نمبر_دا_SAMPLES);
    لکھو("   xStreamBufferReceive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", دھارا_receive_median, دھارا_receive_avg);

    // Test 5: Task APIs
    لکھو("\n5. Testing Task APIs...\n");

    // xTaskGetTickCount
    لكل (صحيح i = 0; i < نمبر_دا_SAMPLES; i++) {
        uint32_t شروع_چکراں = esp_cpu_get_چکر_گنتی();
        x_Task_Get_Tick_گنتی();
        execution_times[i] = esp_cpu_get_چکر_گنتی() - شروع_چکراں;
    }
    uint32_t task_tick_median = calculate_median(execution_times, نمبر_دا_SAMPLES);
    uint32_t task_tick_avg = calculate_اوسطن(execution_times, نمبر_دا_SAMPLES);
    لکھو("   xTaskGetTickCount: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", task_tick_median, task_tick_avg);

    لکھو("\n=== Performance Test Complete ===\n");
    لکھو("Note: Compare results between Flash and IRAM configurations.\n");
    لکھو("Consider memory cost vs performance benefit for your application.\n");

    // Cleanup
    v_دھارا_بفر_Delete(test_دھارا_بفر);
    v_Event_گروہ_Delete(test_event_گروہ);
    vSemaphoreDelete(test_semaphore);
    v_قطار_Delete(test_قطار);
}
