/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Unit tests for FreeRTOS preemption
*/

#ಅನ್ತರ್ಭಾವ <esp_types.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <inttypes.h>

#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "freertos/semphr.h"
#ಅನ್ತರ್ಭಾವ "freertos/queue.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "esp_cpu.h"
#ಅನ್ತರ್ಭಾವ "test_utils.h"
#ಅನ್ತರ್ಭಾವ "sdkconfig.h"

ಸ್ಥಿರ ಚಞ್ಚಲ bool trigger;
ಸ್ಥಿರ ಚಞ್ಚಲ bool flag;

#ifndef CONFIG_FREERTOS_SMP
#define MAX_YIELD_ಎಣಿಕೆ 10000
#ಅನ್ಯಥಾ
//TODO: IDF-5081
#define MAX_YIELD_ಎಣಿಕೆ 17000
#endif // CONFIG_FREERTOS_SMP

/* Task:
   - Waits for 'trigger' variable to be set
   - Reads the cycle count on this CPU
   - Pushes it into a queue supplied as a param
   - Busy-waits until the main task terminates it
*/
ಸ್ಥಿರ ಶೂನ್ಯ task_send_to_queue(ಶೂನ್ಯ *param)
{
    QueueHandle_t queue = (QueueHandle_t) param;
    uint32_t ccount;

    ಯಾವತ್ (!trigger) {}

    ccount = esp_cpu_get_cycle_ಎಣಿಕೆ();
    flag = true;
    xQueueSendToBack(queue, &ccount, 0);
    /* This is to ensure that higher priority task
       won't wake anyhow, due to this task terminating.

       The task runs until terminated by the main task.
    */
    ಯಾವತ್ (1) {}
}

TEST_CASE("Yield from lower priority task, same CPU", "[freertos]")
{
    /* Do this 3 times, mostly for the benchmark value - the first
       run includes a cache miss so uses more cycles than it should. */
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < 3; i++) {
        TaskHandle_t sender_task;
        QueueHandle_t queue = xQueueCreate(1, ಪರಿಮಾಣ(uint32_t));
        flag = false;
        trigger = false;

        /* "yield" task sits on our CPU, lower priority to us */
        xTaskCreatePinnedToCore(task_send_to_queue, "YIELD", 2048, (ಶೂನ್ಯ *)queue, UNITY_FREERTOS_PRIORITY - 1, &sender_task, UNITY_FREERTOS_CPU);

        vTaskDelay(1); /* make sure everything is set up */
        trigger = true;

        uint32_t yield_ccount, now_ccount, delta;
        TEST_ASSERT(xQueueReceive(queue, &yield_ccount, 100 / portTICK_PERIOD_MS));
        now_ccount = esp_cpu_get_cycle_ಎಣಿಕೆ();
        TEST_ASSERT(flag);

        delta = now_ccount - yield_ccount;
        printf("Yielding from lower priority task took %"PRIu32" cycles\n", delta);
        TEST_ASSERT(delta < MAX_YIELD_ಎಣಿಕೆ);

        vTaskDelete(sender_task);
        vQueueDelete(queue);
    }
}

#ಯದಿ (CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES == 2) && CONFIG_FREERTOS_IN_IRAM
TEST_CASE("Yield from lower priority task, other CPU", "[freertos]")
{
    uint32_t trigger_ccount, yield_ccount, now_ccount, delta;

    /* Do this 3 times, mostly for the benchmark value - the first
       run includes a cache miss so uses more cycles than it should. */
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < 3; i++) {
        TaskHandle_t sender_task;
        QueueHandle_t queue = xQueueCreate(1, ಪರಿಮಾಣ(uint32_t));
        trigger = false;
        flag = false;

        /* "send_to_queue" task sits on the other CPU, lower priority to us */
        xTaskCreatePinnedToCore(task_send_to_queue, "YIELD", 2048, (ಶೂನ್ಯ *)queue, UNITY_FREERTOS_PRIORITY - 1,
                                &sender_task, !UNITY_FREERTOS_CPU);

        vTaskDelay(2); /* make sure everything is set up */
        trigger = true;
        trigger_ccount = esp_cpu_get_cycle_ಎಣಿಕೆ();

        // yield_ccount is not useful in this test as it's the other core's CCOUNT
        // so we use trigger_ccount instead
        TEST_ASSERT(xQueueReceive(queue, &yield_ccount, 100 / portTICK_PERIOD_MS));
        now_ccount = esp_cpu_get_cycle_ಎಣಿಕೆ();
        TEST_ASSERT(flag);

        delta = now_ccount - trigger_ccount;
        printf("Yielding from task on other core took %"PRIu32" cycles\n", delta);
        TEST_ASSERT(delta < MAX_YIELD_ಎಣಿಕೆ);

        vQueueDelete(queue);
        vTaskDelete(sender_task);
    }
}
#endif
