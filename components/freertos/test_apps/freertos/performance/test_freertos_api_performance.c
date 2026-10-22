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

#అన్తర్భావ <esp_types.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <inttypes.h>
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "freertos/semphr.h"
#అన్తర్భావ "freertos/queue.h"
#అన్తర్భావ "freertos/event_groups.h"
#అన్తర్భావ "freertos/stream_buffer.h"
#అన్తర్భావ "freertos/timers.h"
#అన్తర్భావ "esp_cpu.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "test_utils.h"

#నిర్వచించు సంఖ్య_యొక్క_SAMPLES 1000

స్థిర పూర్ణాంకం compare_uint32(స్థిరరాశి శూన్య *a, స్థిరరాశి శూన్య *b)
{
    తిరిగి (*(uint32_t *)a - * (uint32_t *)b);
}

స్థిర uint32_t calculate_median(uint32_t *విలువలు, పూర్ణాంకం కొలత)
{
    /* Sort the array of values containing the execution time of each iteration */
    qsort(విలువలు, కొలత, పరిమాణం(uint32_t), compare_uint32);
    /* Return the median value */
    తిరిగి విలువలు[కొలత / 2];
}

స్థిర uint32_t calculate_సరాసరి(uint32_t *విలువలు, పూర్ణాంకం కొలత)
{
    uint64_t కూడిక = 0;
    కొరకు (పూర్ణాంకం i = 0; i < కొలత; i++) {
        కూడిక += విలువలు[i];
    }
    తిరిగి (uint32_t)(కూడిక / కొలత);
}

TEST_CASE("freertos api performance test", "[freertos][ignore]")
{
    రాయి("=== FreeRTOS API Performance Test ===\n");
    రాయి("This test measures execution time of commonly used FreeRTOS APIs.\n");
    రాయి("Results help evaluate the impact of memory placement (Flash vs IRAM).\n\n");

#ఒకవేళ defined(CONFIG_FREERTOS_లోపల_IRAM)
    రాయి("FreeRTOS functions placed in IRAM.\r\n");
#లేకపోతే
    రాయి("FreeRTOS functions placed in Flash.\r\n");
#అయితే_ముగింపు
    రాయి("Testing with %d samples per API\n", సంఖ్య_యొక్క_SAMPLES);
    రాయి("Note: Performance varies by target architecture, flash cache, and system load.\n\n");

    uint32_t execution_times[సంఖ్య_యొక్క_SAMPLES];
    uint32_t test_దత్తాంశం = 0x12345678;

    // Initialize test objects
    వరుస_Handle_t test_వరుస = x_వరుస_Create(10, పరిమాణం(uint32_t));
    SemaphoreHandle_t test_semaphore = x_Semaphore_Create_ద్విమాన();
    Event_సమూహం_Handle_t test_event_సమూహం = x_Event_సమూహం_Create();
    ప్రవాహం_బఫరు_Handle_t test_ప్రవాహం_బఫరు = x_ప్రవాహం_బఫరు_Create(1024, 1);

    TEST_ASSERT(test_వరుస != ఖాళీ);
    TEST_ASSERT(test_semaphore != ఖాళీ);
    TEST_ASSERT(test_event_సమూహం != ఖాళీ);
    TEST_ASSERT(test_ప్రవాహం_బఫరు != ఖాళీ);

    // Test 1: Queue APIs
    రాయి("1. Testing Queue APIs...\n");

    // xQueueSend
    కొరకు (పూర్ణాంకం i = 0; i < సంఖ్య_యొక్క_SAMPLES; i++) {
        uint32_t ప్రారంభం_చక్రాలు = esp_cpu_get_చక్రం_లెక్క();
        x_వరుస_Send(test_వరుస, &test_దత్తాంశం, 0);
        execution_times[i] = esp_cpu_get_చక్రం_లెక్క() - ప్రారంభం_చక్రాలు;
    }
    uint32_t వరుస_send_median = calculate_median(execution_times, సంఖ్య_యొక్క_SAMPLES);
    uint32_t వరుస_send_avg = calculate_సరాసరి(execution_times, సంఖ్య_యొక్క_SAMPLES);
    రాయి("   xQueueSend: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", వరుస_send_median, వరుస_send_avg);

    // xQueueReceive
    కొరకు (పూర్ణాంకం i = 0; i < సంఖ్య_యొక్క_SAMPLES; i++) {
        uint32_t ప్రారంభం_చక్రాలు = esp_cpu_get_చక్రం_లెక్క();
        x_వరుస_Receive(test_వరుస, &test_దత్తాంశం, 0);
        execution_times[i] = esp_cpu_get_చక్రం_లెక్క() - ప్రారంభం_చక్రాలు;
    }
    uint32_t వరుస_receive_median = calculate_median(execution_times, సంఖ్య_యొక్క_SAMPLES);
    uint32_t వరుస_receive_avg = calculate_సరాసరి(execution_times, సంఖ్య_యొక్క_SAMPLES);
    రాయి("   xQueueReceive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", వరుస_receive_median, వరుస_receive_avg);

    // Test 2: Semaphore APIs
    రాయి("\n2. Testing Semaphore APIs...\n");

    // xSemaphoreGive
    కొరకు (పూర్ణాంకం i = 0; i < సంఖ్య_యొక్క_SAMPLES; i++) {
        uint32_t ప్రారంభం_చక్రాలు = esp_cpu_get_చక్రం_లెక్క();
        xSemaphoreGive(test_semaphore);
        execution_times[i] = esp_cpu_get_చక్రం_లెక్క() - ప్రారంభం_చక్రాలు;
    }
    uint32_t sem_give_median = calculate_median(execution_times, సంఖ్య_యొక్క_SAMPLES);
    uint32_t sem_give_avg = calculate_సరాసరి(execution_times, సంఖ్య_యొక్క_SAMPLES);
    రాయి("   xSemaphoreGive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", sem_give_median, sem_give_avg);

    // xSemaphoreTake
    కొరకు (పూర్ణాంకం i = 0; i < సంఖ్య_యొక్క_SAMPLES; i++) {
        uint32_t ప్రారంభం_చక్రాలు = esp_cpu_get_చక్రం_లెక్క();
        xSemaphoreTake(test_semaphore, 0);
        execution_times[i] = esp_cpu_get_చక్రం_లెక్క() - ప్రారంభం_చక్రాలు;
    }
    uint32_t sem_take_median = calculate_median(execution_times, సంఖ్య_యొక్క_SAMPLES);
    uint32_t sem_take_avg = calculate_సరాసరి(execution_times, సంఖ్య_యొక్క_SAMPLES);
    రాయి("   xSemaphoreTake: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", sem_take_median, sem_take_avg);

    // Test 3: Event Group APIs
    రాయి("\n3. Testing Event Group APIs...\n");

    // xEventGroupSetBits
    కొరకు (పూర్ణాంకం i = 0; i < సంఖ్య_యొక్క_SAMPLES; i++) {
        uint32_t ప్రారంభం_చక్రాలు = esp_cpu_get_చక్రం_లెక్క();
        x_Event_సమూహం_సమితి_బిట్లు(test_event_సమూహం, 0x01);
        execution_times[i] = esp_cpu_get_చక్రం_లెక్క() - ప్రారంభం_చక్రాలు;
    }
    uint32_t event_సమితి_median = calculate_median(execution_times, సంఖ్య_యొక్క_SAMPLES);
    uint32_t event_సమితి_avg = calculate_సరాసరి(execution_times, సంఖ్య_యొక్క_SAMPLES);
    రాయి("   xEventGroupSetBits: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", event_సమితి_median, event_సమితి_avg);

    // xEventGroupGetBits
    కొరకు (పూర్ణాంకం i = 0; i < సంఖ్య_యొక్క_SAMPLES; i++) {
        uint32_t ప్రారంభం_చక్రాలు = esp_cpu_get_చక్రం_లెక్క();
        x_Event_సమూహం_Get_బిట్లు(test_event_సమూహం);
        execution_times[i] = esp_cpu_get_చక్రం_లెక్క() - ప్రారంభం_చక్రాలు;
    }
    uint32_t event_get_median = calculate_median(execution_times, సంఖ్య_యొక్క_SAMPLES);
    uint32_t event_get_avg = calculate_సరాసరి(execution_times, సంఖ్య_యొక్క_SAMPLES);
    రాయి("   xEventGroupGetBits: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", event_get_median, event_get_avg);

    // Test 4: Stream Buffer APIs
    రాయి("\n4. Testing Stream Buffer APIs...\n");

    // xStreamBufferSend
    కొరకు (పూర్ణాంకం i = 0; i < సంఖ్య_యొక్క_SAMPLES; i++) {
        uint32_t ప్రారంభం_చక్రాలు = esp_cpu_get_చక్రం_లెక్క();
        x_ప్రవాహం_బఫరు_Send(test_ప్రవాహం_బఫరు, &test_దత్తాంశం, పరిమాణం(uint32_t), 0);
        execution_times[i] = esp_cpu_get_చక్రం_లెక్క() - ప్రారంభం_చక్రాలు;
    }
    uint32_t ప్రవాహం_send_median = calculate_median(execution_times, సంఖ్య_యొక్క_SAMPLES);
    uint32_t ప్రవాహం_send_avg = calculate_సరాసరి(execution_times, సంఖ్య_యొక్క_SAMPLES);
    రాయి("   xStreamBufferSend: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", ప్రవాహం_send_median, ప్రవాహం_send_avg);

    // xStreamBufferReceive
    కొరకు (పూర్ణాంకం i = 0; i < సంఖ్య_యొక్క_SAMPLES; i++) {
        uint32_t ప్రారంభం_చక్రాలు = esp_cpu_get_చక్రం_లెక్క();
        x_ప్రవాహం_బఫరు_Receive(test_ప్రవాహం_బఫరు, &test_దత్తాంశం, పరిమాణం(uint32_t), 0);
        execution_times[i] = esp_cpu_get_చక్రం_లెక్క() - ప్రారంభం_చక్రాలు;
    }
    uint32_t ప్రవాహం_receive_median = calculate_median(execution_times, సంఖ్య_యొక్క_SAMPLES);
    uint32_t ప్రవాహం_receive_avg = calculate_సరాసరి(execution_times, సంఖ్య_యొక్క_SAMPLES);
    రాయి("   xStreamBufferReceive: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", ప్రవాహం_receive_median, ప్రవాహం_receive_avg);

    // Test 5: Task APIs
    రాయి("\n5. Testing Task APIs...\n");

    // xTaskGetTickCount
    కొరకు (పూర్ణాంకం i = 0; i < సంఖ్య_యొక్క_SAMPLES; i++) {
        uint32_t ప్రారంభం_చక్రాలు = esp_cpu_get_చక్రం_లెక్క();
        x_Task_Get_Tick_లెక్క();
        execution_times[i] = esp_cpu_get_చక్రం_లెక్క() - ప్రారంభం_చక్రాలు;
    }
    uint32_t task_tick_median = calculate_median(execution_times, సంఖ్య_యొక్క_SAMPLES);
    uint32_t task_tick_avg = calculate_సరాసరి(execution_times, సంఖ్య_యొక్క_SAMPLES);
    రాయి("   xTaskGetTickCount: Median=%"PRIu32" cycles, Average=%"PRIu32" cycles\n", task_tick_median, task_tick_avg);

    రాయి("\n=== Performance Test Complete ===\n");
    రాయి("Note: Compare results between Flash and IRAM configurations.\n");
    రాయి("Consider memory cost vs performance benefit for your application.\n");

    // Cleanup
    v_ప్రవాహం_బఫరు_Delete(test_ప్రవాహం_బఫరు);
    v_Event_సమూహం_Delete(test_event_సమూహం);
    vSemaphoreDelete(test_semaphore);
    v_వరుస_Delete(test_వరుస);
}
