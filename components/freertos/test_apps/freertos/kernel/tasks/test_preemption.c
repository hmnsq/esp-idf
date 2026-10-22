/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Unit tests for FreeRTOS preemption
*/

#அந்தர்பாவ <esp_types.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <inttypes.h>

#அந்தர்பாவ "freertos/FreeRTOS.h"
#அந்தர்பாவ "freertos/task.h"
#அந்தர்பாவ "freertos/semphr.h"
#அந்தர்பாவ "freertos/queue.h"
#அந்தர்பாவ "unity.h"
#அந்தர்பாவ "esp_cpu.h"
#அந்தர்பாவ "test_utils.h"
#அந்தர்பாவ "sdkconfig.h"

நிலையான நிலையற்ற bool trigger;
நிலையான நிலையற்ற bool flag;

#ifndef CONFIG_FREERTOS_SMP
#define MAX_YIELD_மொத்தம் 10000
#இல்லையெனில்
//TODO: IDF-5081
#define MAX_YIELD_மொத்தம் 17000
#endif // CONFIG_FREERTOS_SMP

/* Task:
   - Waits for 'trigger' variable to be set
   - Reads the cycle count on this CPU
   - Pushes it into a queue supplied as a param
   - Busy-waits until the main task terminates it
*/
நிலையான வெறுமை task_send_to_queue(வெறுமை *param)
{
    QueueHandle_t queue = (QueueHandle_t) param;
    uint32_t ccount;

    போது (!trigger) {}

    ccount = esp_cpu_get_cycle_மொத்தம்();
    flag = true;
    xQueueSendToBack(queue, &ccount, 0);
    /* This is to ensure that higher priority task
       won't wake anyhow, due to this task terminating.

       The task runs until terminated by the main task.
    */
    போது (1) {}
}

TEST_CASE("Yield from lower priority task, same CPU", "[freertos]")
{
    /* Do this 3 times, mostly for the benchmark value - the first
       run includes a cache miss so uses more cycles than it should. */
    க்கு (முழு i = 0; i < 3; i++) {
        TaskHandle_t sender_task;
        QueueHandle_t queue = xQueueCreate(1, அளவு(uint32_t));
        flag = false;
        trigger = false;

        /* "yield" task sits on our CPU, lower priority to us */
        xTaskCreatePinnedToCore(task_send_to_queue, "YIELD", 2048, (வெறுமை *)queue, UNITY_FREERTOS_PRIORITY - 1, &sender_task, UNITY_FREERTOS_CPU);

        vTaskDelay(1); /* make sure everything is set up */
        trigger = true;

        uint32_t yield_ccount, now_ccount, delta;
        TEST_ASSERT(xQueueReceive(queue, &yield_ccount, 100 / portTICK_PERIOD_MS));
        now_ccount = esp_cpu_get_cycle_மொத்தம்();
        TEST_ASSERT(flag);

        delta = now_ccount - yield_ccount;
        printf("Yielding from lower priority task took %"PRIu32" cycles\n", delta);
        TEST_ASSERT(delta < MAX_YIELD_மொத்தம்);

        vTaskDelete(sender_task);
        vQueueDelete(queue);
    }
}

#எனில் (CONFIG_FREERTOS_எண்_OF_CORES == 2) && CONFIG_FREERTOS_IN_IRAM
TEST_CASE("Yield from lower priority task, other CPU", "[freertos]")
{
    uint32_t trigger_ccount, yield_ccount, now_ccount, delta;

    /* Do this 3 times, mostly for the benchmark value - the first
       run includes a cache miss so uses more cycles than it should. */
    க்கு (முழு i = 0; i < 3; i++) {
        TaskHandle_t sender_task;
        QueueHandle_t queue = xQueueCreate(1, அளவு(uint32_t));
        trigger = false;
        flag = false;

        /* "send_to_queue" task sits on the other CPU, lower priority to us */
        xTaskCreatePinnedToCore(task_send_to_queue, "YIELD", 2048, (வெறுமை *)queue, UNITY_FREERTOS_PRIORITY - 1,
                                &sender_task, !UNITY_FREERTOS_CPU);

        vTaskDelay(2); /* make sure everything is set up */
        trigger = true;
        trigger_ccount = esp_cpu_get_cycle_மொத்தம்();

        // yield_ccount is not useful in this test as it's the other core's CCOUNT
        // so we use trigger_ccount instead
        TEST_ASSERT(xQueueReceive(queue, &yield_ccount, 100 / portTICK_PERIOD_MS));
        now_ccount = esp_cpu_get_cycle_மொத்தம்();
        TEST_ASSERT(flag);

        delta = now_ccount - trigger_ccount;
        printf("Yielding from task on other core took %"PRIu32" cycles\n", delta);
        TEST_ASSERT(delta < MAX_YIELD_மொத்தம்);

        vQueueDelete(queue);
        vTaskDelete(sender_task);
    }
}
#endif
