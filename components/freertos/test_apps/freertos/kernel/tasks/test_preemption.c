/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Unit tests for FreeRTOS preemption
*/

#ضمن <esp_types.h>
#ضمن <stdio.h>
#ضمن <inttypes.h>

#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "freertos/semphr.h"
#ضمن "freertos/queue.h"
#ضمن "unity.h"
#ضمن "esp_cpu.h"
#ضمن "test_utils.h"
#ضمن "sdkconfig.h"

ساكن متطاير bool trigger;
ساكن متطاير bool flag;

#ifndef CONFIG_FREERTOS_SMP
#define MAX_YIELD_COUNT 10000
#وإلا
//TODO: IDF-5081
#define MAX_YIELD_COUNT 17000
#endif // CONFIG_FREERTOS_SMP

/* Task:
   - Waits for 'trigger' variable to be set
   - Reads the cycle count on this CPU
   - Pushes it into a queue supplied as a param
   - Busy-waits until the main task terminates it
*/
ساكن فراغ task_send_to_queue(فراغ *param)
{
    QueueHandle_t queue = (QueueHandle_t) param;
    uint32_t ccount;

    طالما (!trigger) {}

    ccount = esp_cpu_get_cycle_count();
    flag = true;
    xQueueSendToBack(queue, &ccount, 0);
    /* This is to ensure that higher priority task
       won't wake anyhow, due to this task terminating.

       The task runs until terminated by the main task.
    */
    طالما (1) {}
}

TEST_CASE("Yield from lower priority task, same CPU", "[freertos]")
{
    /* Do this 3 times, mostly for the benchmark value - the first
       run includes a cache miss so uses more cycles than it should. */
    لكل (صحيح i = 0; i < 3; i++) {
        TaskHandle_t sender_task;
        QueueHandle_t queue = xQueueCreate(1, حجم(uint32_t));
        flag = false;
        trigger = false;

        /* "yield" task sits on our CPU, lower priority to us */
        xTaskCreatePinnedToCore(task_send_to_queue, "YIELD", 2048, (فراغ *)queue, UNITY_FREERTOS_PRIORITY - 1, &sender_task, UNITY_FREERTOS_CPU);

        vTaskDelay(1); /* make sure everything is set up */
        trigger = true;

        uint32_t yield_ccount, now_ccount, delta;
        TEST_ASSERT(xQueueReceive(queue, &yield_ccount, 100 / portTICK_PERIOD_MS));
        now_ccount = esp_cpu_get_cycle_count();
        TEST_ASSERT(flag);

        delta = now_ccount - yield_ccount;
        printf("Yielding from lower priority task took %"PRIu32" cycles\n", delta);
        TEST_ASSERT(delta < MAX_YIELD_COUNT);

        vTaskDelete(sender_task);
        vQueueDelete(queue);
    }
}

#𐡄𐡍 (CONFIG_FREERTOS_NUMBER_OF_CORES == 2) && CONFIG_FREERTOS_IN_IRAM
TEST_CASE("Yield from lower priority task, other CPU", "[freertos]")
{
    uint32_t trigger_ccount, yield_ccount, now_ccount, delta;

    /* Do this 3 times, mostly for the benchmark value - the first
       run includes a cache miss so uses more cycles than it should. */
    لكل (صحيح i = 0; i < 3; i++) {
        TaskHandle_t sender_task;
        QueueHandle_t queue = xQueueCreate(1, حجم(uint32_t));
        trigger = false;
        flag = false;

        /* "send_to_queue" task sits on the other CPU, lower priority to us */
        xTaskCreatePinnedToCore(task_send_to_queue, "YIELD", 2048, (فراغ *)queue, UNITY_FREERTOS_PRIORITY - 1,
                                &sender_task, !UNITY_FREERTOS_CPU);

        vTaskDelay(2); /* make sure everything is set up */
        trigger = true;
        trigger_ccount = esp_cpu_get_cycle_count();

        // yield_ccount is not useful in this test as it's the other core's CCOUNT
        // so we use trigger_ccount instead
        TEST_ASSERT(xQueueReceive(queue, &yield_ccount, 100 / portTICK_PERIOD_MS));
        now_ccount = esp_cpu_get_cycle_count();
        TEST_ASSERT(flag);

        delta = now_ccount - trigger_ccount;
        printf("Yielding from task on other core took %"PRIu32" cycles\n", delta);
        TEST_ASSERT(delta < MAX_YIELD_COUNT);

        vQueueDelete(queue);
        vTaskDelete(sender_task);
    }
}
#endif
