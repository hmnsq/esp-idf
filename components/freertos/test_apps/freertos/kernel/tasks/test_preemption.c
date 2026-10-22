/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Unit tests for FreeRTOS preemption
*/

#ਅਨ੍ਤਰ੍ਭਾਵ <esp_types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <inttypes.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/semphr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/queue.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_cpu.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "test_utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"

ਸਥਿਰ ਅਸਥਿਰ bool trigger;
ਸਥਿਰ ਅਸਥਿਰ bool flag;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CONFIG_FREERTOS_SMP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_YIELD_ਗਿਣਤੀ 10000
#ਨਹੀਂਤਾਂ
//TODO: IDF-5081
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_YIELD_ਗਿਣਤੀ 17000
#ਜੇ_ਅੰਤ // CONFIG_FREERTOS_SMP

/* Task:
   - Waits for 'trigger' variable to be set
   - Reads the cycle count on this CPU
   - Pushes it into a queue supplied as a param
   - Busy-waits until the main task terminates it
*/
ਸਥਿਰ ਖਾਲੀ task_send_ਮੰਜ਼ਿਲ_ਕਤਾਰ(ਖਾਲੀ *param)
{
    ਕਤਾਰ_Handle_t ਕਤਾਰ = (ਕਤਾਰ_Handle_t) param;
    uint32_t ccount;

    ਜਦੋਂ (!trigger) {}

    ccount = esp_cpu_get_ਚੱਕਰ_ਗਿਣਤੀ();
    flag = true;
    x_ਕਤਾਰ_Send_ਮੰਜ਼ਿਲ_ਪਿੱਠ(ਕਤਾਰ, &ccount, 0);
    /* This is to ensure that higher priority task
       won't wake anyhow, due to this task terminating.

       The task runs until terminated by the main task.
    */
    ਜਦੋਂ (1) {}
}

TEST_CASE("Yield from lower priority task, same CPU", "[freertos]")
{
    /* Do this 3 times, mostly for the benchmark value - the first
       run includes a cache miss so uses more cycles than it should. */
    ਲਈ (ਪੂਰਨ i = 0; i < 3; i++) {
        TaskHandle_t sender_task;
        ਕਤਾਰ_Handle_t ਕਤਾਰ = x_ਕਤਾਰ_Create(1, ਆਕਾਰ(uint32_t));
        flag = false;
        trigger = false;

        /* "yield" task sits on our CPU, lower priority to us */
        x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(task_send_ਮੰਜ਼ਿਲ_ਕਤਾਰ, "YIELD", 2048, (ਖਾਲੀ *)ਕਤਾਰ, UNITY_FREERTOS_ਤਰਜੀਹ - 1, &sender_task, UNITY_FREERTOS_CPU);

        vTaskDelay(1); /* make sure everything is set up */
        trigger = true;

        uint32_t yield_ccount, now_ccount, delta;
        TEST_ASSERT(x_ਕਤਾਰ_Receive(ਕਤਾਰ, &yield_ccount, 100 / ਪੋਰਟ_TICK_PERIOD_MS));
        now_ccount = esp_cpu_get_ਚੱਕਰ_ਗਿਣਤੀ();
        TEST_ASSERT(flag);

        delta = now_ccount - yield_ccount;
        ਲਿਖੋ("Yielding from lower priority task took %"PRIu32" cycles\n", delta);
        TEST_ASSERT(delta < MAX_YIELD_ਗਿਣਤੀ);

        vTaskDelete(sender_task);
        v_ਕਤਾਰ_Delete(ਕਤਾਰ);
    }
}

#ਜੇ (CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES == 2) && CONFIG_FREERTOS_ਵਿੱਚ_IRAM
TEST_CASE("Yield from lower priority task, other CPU", "[freertos]")
{
    uint32_t trigger_ccount, yield_ccount, now_ccount, delta;

    /* Do this 3 times, mostly for the benchmark value - the first
       run includes a cache miss so uses more cycles than it should. */
    ਲਈ (ਪੂਰਨ i = 0; i < 3; i++) {
        TaskHandle_t sender_task;
        ਕਤਾਰ_Handle_t ਕਤਾਰ = x_ਕਤਾਰ_Create(1, ਆਕਾਰ(uint32_t));
        trigger = false;
        flag = false;

        /* "send_to_queue" task sits on the other CPU, lower priority to us */
        x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(task_send_ਮੰਜ਼ਿਲ_ਕਤਾਰ, "YIELD", 2048, (ਖਾਲੀ *)ਕਤਾਰ, UNITY_FREERTOS_ਤਰਜੀਹ - 1,
                                &sender_task, !UNITY_FREERTOS_CPU);

        vTaskDelay(2); /* make sure everything is set up */
        trigger = true;
        trigger_ccount = esp_cpu_get_ਚੱਕਰ_ਗਿਣਤੀ();

        // yield_ccount is not useful in this test as it's the other core's CCOUNT
        // so we use trigger_ccount instead
        TEST_ASSERT(x_ਕਤਾਰ_Receive(ਕਤਾਰ, &yield_ccount, 100 / ਪੋਰਟ_TICK_PERIOD_MS));
        now_ccount = esp_cpu_get_ਚੱਕਰ_ਗਿਣਤੀ();
        TEST_ASSERT(flag);

        delta = now_ccount - trigger_ccount;
        ਲਿਖੋ("Yielding from task on other core took %"PRIu32" cycles\n", delta);
        TEST_ASSERT(delta < MAX_YIELD_ਗਿਣਤੀ);

        v_ਕਤਾਰ_Delete(ਕਤਾਰ);
        vTaskDelete(sender_task);
    }
}
#ਜੇ_ਅੰਤ
