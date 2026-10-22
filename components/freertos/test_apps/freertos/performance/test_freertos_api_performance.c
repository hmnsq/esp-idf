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

#تعریف_کړه شمېره_د_SAMPLES 1000

ساكن صحيح compare_uint32(ثابت فراغ *a, ثابت فراغ *b)
{
    أرجع (*(uint32_t *)a - * (uint32_t *)b);
}

ساكن uint32_t calculate_median(uint32_t *ارزښتونه, صحيح اندازه)
{
    /* Sort the array of values containing the execution time of each iteration */
    qsort(ارزښتونه, اندازه, حجم(uint32_t), compare_uint32);
    /* Return the median value */
    أرجع ارزښتونه[اندازه / 2];
}

ساكن uint32_t calculate_اوسط(uint32_t *ارزښتونه, صحيح اندازه)
{
    uint64_t مجموعه = 0;
    لكل (صحيح i = 0; i < اندازه; i++) {
        مجموعه += ارزښتونه[i];
    }
    أرجع (uint32_t)(مجموعه / اندازه);
}

TEST_CASE("freertos api performance test", "[freertos][ignore]")
{
    چاپ_کړه("=== FreeRTOS API Performance Test ===\n");
    چاپ_کړه("This test measures execution time of commonly used FreeRTOS APIs.\n");
    چاپ_کړه("Results help evaluate the impact of memory placement (Flash vs IRAM).\n\n");

#إذا defined(CONFIG_FREERTOS_دننه_IRAM)
    چاپ_کړه("FreeRTOS functions placed in IRAM.\r\n");
#وإلا
    چاپ_کړه("FreeRTOS functions placed in Flash.\r\n");
#د_که_پای
    چاپ_کړه("Testing with %d samples per API\n", شمېره_د_SAMPLES);
    چاپ_کړه("Note: Performance varies by target architecture, flash cache, and system load.\n\n");

    uint32_t execution_times[شمېره_د_SAMPLES];
    uint32_t test_معلومات = 0x12345678;

    // Initialize test objects
    کتار_Handle_t test_کتار = x_کتار_Create(10, حجم(uint32_t));
    SemaphoreHandle_t test_semaphore = x_Semaphore_Create_دوګونی();
    Event_ډله_Handle_t test_event_ډله = x_Event_ډله_Create();
    بهیر_بفر_Handle_t test_بهیر_بفر = x_بهیر_بفر_Create(1024, 1);

    TEST_ASSERT(test_کتار != عدم);
    TEST_ASSERT(test_semaphore != عدم);
    TEST_ASSERT(test_event_ډله != عدم);
    TEST_ASSERT(test_بهیر_بفر != عدم);

    // Test 1: Queue APIs
    چاپ_کړه("1. Testing Queue APIs...\n");

    // xQueueSend
    لكل (صحيح i = 0; i < شمېره_د_SAMPLES; i++) {
        uint32_t پیل_دورې = esp_cpu_get_دوره_شمېر();
        x_کتار_Send(test_کتار, &test_معلومات, 0);
        execution_times[i] = esp_cpu_get_دوره_شمېر() - پیل_دورې;
    }
    uint32_t کتار_send_median = calculate_median(execution_times, شمېره_د_SAMPLES);
    uint32_t کتار_send_avg = calculate_اوسط(execution_times, شمېره_د_SAMPLES);
    چاپ_کړه("   xQueueSend: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", کتار_send_median, کتار_send_avg);

    // xQueueReceive
    لكل (صحيح i = 0; i < شمېره_د_SAMPLES; i++) {
        uint32_t پیل_دورې = esp_cpu_get_دوره_شمېر();
        x_کتار_Receive(test_کتار, &test_معلومات, 0);
        execution_times[i] = esp_cpu_get_دوره_شمېر() - پیل_دورې;
    }
    uint32_t کتار_receive_median = calculate_median(execution_times, شمېره_د_SAMPLES);
    uint32_t کتار_receive_avg = calculate_اوسط(execution_times, شمېره_د_SAMPLES);
    چاپ_کړه("   xQueueReceive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", کتار_receive_median, کتار_receive_avg);

    // Test 2: Semaphore APIs
    چاپ_کړه("\n2. Testing Semaphore APIs...\n");

    // xSemaphoreGive
    لكل (صحيح i = 0; i < شمېره_د_SAMPLES; i++) {
        uint32_t پیل_دورې = esp_cpu_get_دوره_شمېر();
        xSemaphoreGive(test_semaphore);
        execution_times[i] = esp_cpu_get_دوره_شمېر() - پیل_دورې;
    }
    uint32_t sem_give_median = calculate_median(execution_times, شمېره_د_SAMPLES);
    uint32_t sem_give_avg = calculate_اوسط(execution_times, شمېره_د_SAMPLES);
    چاپ_کړه("   xSemaphoreGive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", sem_give_median, sem_give_avg);

    // xSemaphoreTake
    لكل (صحيح i = 0; i < شمېره_د_SAMPLES; i++) {
        uint32_t پیل_دورې = esp_cpu_get_دوره_شمېر();
        xSemaphoreTake(test_semaphore, 0);
        execution_times[i] = esp_cpu_get_دوره_شمېر() - پیل_دورې;
    }
    uint32_t sem_take_median = calculate_median(execution_times, شمېره_د_SAMPLES);
    uint32_t sem_take_avg = calculate_اوسط(execution_times, شمېره_د_SAMPLES);
    چاپ_کړه("   xSemaphoreTake: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", sem_take_median, sem_take_avg);

    // Test 3: Event Group APIs
    چاپ_کړه("\n3. Testing Event Group APIs...\n");

    // xEventGroupSetBits
    لكل (صحيح i = 0; i < شمېره_د_SAMPLES; i++) {
        uint32_t پیل_دورې = esp_cpu_get_دوره_شمېر();
        x_Event_ډله_سېټ_بټونه(test_event_ډله, 0x01);
        execution_times[i] = esp_cpu_get_دوره_شمېر() - پیل_دورې;
    }
    uint32_t event_سېټ_median = calculate_median(execution_times, شمېره_د_SAMPLES);
    uint32_t event_سېټ_avg = calculate_اوسط(execution_times, شمېره_د_SAMPLES);
    چاپ_کړه("   xEventGroupSetBits: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", event_سېټ_median, event_سېټ_avg);

    // xEventGroupGetBits
    لكل (صحيح i = 0; i < شمېره_د_SAMPLES; i++) {
        uint32_t پیل_دورې = esp_cpu_get_دوره_شمېر();
        x_Event_ډله_Get_بټونه(test_event_ډله);
        execution_times[i] = esp_cpu_get_دوره_شمېر() - پیل_دورې;
    }
    uint32_t event_get_median = calculate_median(execution_times, شمېره_د_SAMPLES);
    uint32_t event_get_avg = calculate_اوسط(execution_times, شمېره_د_SAMPLES);
    چاپ_کړه("   xEventGroupGetBits: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", event_get_median, event_get_avg);

    // Test 4: Stream Buffer APIs
    چاپ_کړه("\n4. Testing Stream Buffer APIs...\n");

    // xStreamBufferSend
    لكل (صحيح i = 0; i < شمېره_د_SAMPLES; i++) {
        uint32_t پیل_دورې = esp_cpu_get_دوره_شمېر();
        x_بهیر_بفر_Send(test_بهیر_بفر, &test_معلومات, حجم(uint32_t), 0);
        execution_times[i] = esp_cpu_get_دوره_شمېر() - پیل_دورې;
    }
    uint32_t بهیر_send_median = calculate_median(execution_times, شمېره_د_SAMPLES);
    uint32_t بهیر_send_avg = calculate_اوسط(execution_times, شمېره_د_SAMPLES);
    چاپ_کړه("   xStreamBufferSend: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", بهیر_send_median, بهیر_send_avg);

    // xStreamBufferReceive
    لكل (صحيح i = 0; i < شمېره_د_SAMPLES; i++) {
        uint32_t پیل_دورې = esp_cpu_get_دوره_شمېر();
        x_بهیر_بفر_Receive(test_بهیر_بفر, &test_معلومات, حجم(uint32_t), 0);
        execution_times[i] = esp_cpu_get_دوره_شمېر() - پیل_دورې;
    }
    uint32_t بهیر_receive_median = calculate_median(execution_times, شمېره_د_SAMPLES);
    uint32_t بهیر_receive_avg = calculate_اوسط(execution_times, شمېره_د_SAMPLES);
    چاپ_کړه("   xStreamBufferReceive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", بهیر_receive_median, بهیر_receive_avg);

    // Test 5: Task APIs
    چاپ_کړه("\n5. Testing Task APIs...\n");

    // xTaskGetTickCount
    لكل (صحيح i = 0; i < شمېره_د_SAMPLES; i++) {
        uint32_t پیل_دورې = esp_cpu_get_دوره_شمېر();
        x_Task_Get_Tick_شمېر();
        execution_times[i] = esp_cpu_get_دوره_شمېر() - پیل_دورې;
    }
    uint32_t task_tick_median = calculate_median(execution_times, شمېره_د_SAMPLES);
    uint32_t task_tick_avg = calculate_اوسط(execution_times, شمېره_د_SAMPLES);
    چاپ_کړه("   xTaskGetTickCount: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", task_tick_median, task_tick_avg);

    چاپ_کړه("\n=== Performance Test Complete ===\n");
    چاپ_کړه("Note: Compare results between Flash and IRAM configurations.\n");
    چاپ_کړه("Consider memory cost vs performance benefit for your application.\n");

    // Cleanup
    v_بهیر_بفر_Delete(test_بهیر_بفر);
    v_Event_ډله_Delete(test_event_ډله);
    vSemaphoreDelete(test_semaphore);
    v_کتار_Delete(test_کتار);
}
