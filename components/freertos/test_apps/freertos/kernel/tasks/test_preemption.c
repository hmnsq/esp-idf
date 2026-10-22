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

ಸ್ಥಿರ ಅಸ್ಥಿರ bool trigger;
ಸ್ಥಿರ ಅಸ್ಥಿರ bool flag;

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CONFIG_FREERTOS_SMP
#ವ್ಯಾಖ್ಯಾನಿಸು MAX_YIELD_ಎಣಿಕೆ 10000
#ಇಲ್ಲದಿದ್ದರೆ
//TODO: IDF-5081
#ವ್ಯಾಖ್ಯಾನಿಸು MAX_YIELD_ಎಣಿಕೆ 17000
#ಆದರೆ_ಅಂತ್ಯ // CONFIG_FREERTOS_SMP

/* Task:
   - Waits for 'trigger' variable to be set
   - Reads the cycle count on this CPU
   - Pushes it into a queue supplied as a param
   - Busy-waits until the main task terminates it
*/
ಸ್ಥಿರ ಶೂನ್ಯ task_send_ಗಮ್ಯ_ಸರತಿ(ಶೂನ್ಯ *param)
{
    ಸರತಿ_Handle_t ಸರತಿ = (ಸರತಿ_Handle_t) param;
    uint32_t ccount;

    ಆಗ (!trigger) {}

    ccount = esp_cpu_get_ಚಕ್ರ_ಎಣಿಕೆ();
    flag = true;
    x_ಸರತಿ_Send_ಗಮ್ಯ_ಹಿಂಭಾಗ(ಸರತಿ, &ccount, 0);
    /* This is to ensure that higher priority task
       won't wake anyhow, due to this task terminating.

       The task runs until terminated by the main task.
    */
    ಆಗ (1) {}
}

TEST_CASE("Yield from lower priority task, same CPU", "[freertos]")
{
    /* Do this 3 times, mostly for the benchmark value - the first
       run includes a cache miss so uses more cycles than it should. */
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 3; i++) {
        TaskHandle_t sender_task;
        ಸರತಿ_Handle_t ಸರತಿ = x_ಸರತಿ_Create(1, ಗಾತ್ರ(uint32_t));
        flag = false;
        trigger = false;

        /* "yield" task sits on our CPU, lower priority to us */
        x_Task_Create_Pinned_ಗಮ್ಯ_Core(task_send_ಗಮ್ಯ_ಸರತಿ, "YIELD", 2048, (ಶೂನ್ಯ *)ಸರತಿ, UNITY_FREERTOS_ಆದ್ಯತೆ - 1, &sender_task, UNITY_FREERTOS_CPU);

        vTaskDelay(1); /* make sure everything is set up */
        trigger = true;

        uint32_t yield_ccount, now_ccount, delta;
        TEST_ASSERT(x_ಸರತಿ_Receive(ಸರತಿ, &yield_ccount, 100 / ಪೋರ್ಟ್_TICK_PERIOD_MS));
        now_ccount = esp_cpu_get_ಚಕ್ರ_ಎಣಿಕೆ();
        TEST_ASSERT(flag);

        delta = now_ccount - yield_ccount;
        ಬರೆ("Yielding from lower priority task took %"PRIu32" cycles\n", delta);
        TEST_ASSERT(delta < MAX_YIELD_ಎಣಿಕೆ);

        vTaskDelete(sender_task);
        v_ಸರತಿ_Delete(ಸರತಿ);
    }
}

#ಆದರೆ (CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES == 2) && CONFIG_FREERTOS_ಒಳ_IRAM
TEST_CASE("Yield from lower priority task, other CPU", "[freertos]")
{
    uint32_t trigger_ccount, yield_ccount, now_ccount, delta;

    /* Do this 3 times, mostly for the benchmark value - the first
       run includes a cache miss so uses more cycles than it should. */
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 3; i++) {
        TaskHandle_t sender_task;
        ಸರತಿ_Handle_t ಸರತಿ = x_ಸರತಿ_Create(1, ಗಾತ್ರ(uint32_t));
        trigger = false;
        flag = false;

        /* "send_to_queue" task sits on the other CPU, lower priority to us */
        x_Task_Create_Pinned_ಗಮ್ಯ_Core(task_send_ಗಮ್ಯ_ಸರತಿ, "YIELD", 2048, (ಶೂನ್ಯ *)ಸರತಿ, UNITY_FREERTOS_ಆದ್ಯತೆ - 1,
                                &sender_task, !UNITY_FREERTOS_CPU);

        vTaskDelay(2); /* make sure everything is set up */
        trigger = true;
        trigger_ccount = esp_cpu_get_ಚಕ್ರ_ಎಣಿಕೆ();

        // yield_ccount is not useful in this test as it's the other core's CCOUNT
        // so we use trigger_ccount instead
        TEST_ASSERT(x_ಸರತಿ_Receive(ಸರತಿ, &yield_ccount, 100 / ಪೋರ್ಟ್_TICK_PERIOD_MS));
        now_ccount = esp_cpu_get_ಚಕ್ರ_ಎಣಿಕೆ();
        TEST_ASSERT(flag);

        delta = now_ccount - trigger_ccount;
        ಬರೆ("Yielding from task on other core took %"PRIu32" cycles\n", delta);
        TEST_ASSERT(delta < MAX_YIELD_ಎಣಿಕೆ);

        v_ಸರತಿ_Delete(ಸರತಿ);
        vTaskDelete(sender_task);
    }
}
#ಆದರೆ_ಅಂತ್ಯ
