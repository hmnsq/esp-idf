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

#که_نه_تعریف_شوی CONFIG_FREERTOS_SMP
#تعریف_کړه MAX_YIELD_شمېر 10000
#وإلا
//TODO: IDF-5081
#تعریف_کړه MAX_YIELD_شمېر 17000
#د_که_پای // CONFIG_FREERTOS_SMP

/* Task:
   - Waits for 'trigger' variable to be set
   - Reads the cycle count on this CPU
   - Pushes it into a queue supplied as a param
   - Busy-waits until the main task terminates it
*/
ساكن فراغ task_send_منزل_کتار(فراغ *param)
{
    کتار_Handle_t کتار = (کتار_Handle_t) param;
    uint32_t ccount;

    طالما (!trigger) {}

    ccount = esp_cpu_get_دوره_شمېر();
    flag = true;
    x_کتار_Send_منزل_شا(کتار, &ccount, 0);
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
        کتار_Handle_t کتار = x_کتار_Create(1, حجم(uint32_t));
        flag = false;
        trigger = false;

        /* "yield" task sits on our CPU, lower priority to us */
        x_Task_Create_Pinned_منزل_Core(task_send_منزل_کتار, "YIELD", 2048, (فراغ *)کتار, UNITY_FREERTOS_لومړیتوب - 1, &sender_task, UNITY_FREERTOS_CPU);

        vTaskDelay(1); /* make sure everything is set up */
        trigger = true;

        uint32_t yield_ccount, now_ccount, delta;
        TEST_ASSERT(x_کتار_Receive(کتار, &yield_ccount, 100 / پورټ_TICK_PERIOD_MS));
        now_ccount = esp_cpu_get_دوره_شمېر();
        TEST_ASSERT(flag);

        delta = now_ccount - yield_ccount;
        چاپ_کړه("Yielding from lower priority task took %"PRIu32" cycles\n", delta);
        TEST_ASSERT(delta < MAX_YIELD_شمېر);

        vTaskDelete(sender_task);
        v_کتار_Delete(کتار);
    }
}

#إذا (CONFIG_FREERTOS_شمېره_د_CORES == 2) && CONFIG_FREERTOS_دننه_IRAM
TEST_CASE("Yield from lower priority task, other CPU", "[freertos]")
{
    uint32_t trigger_ccount, yield_ccount, now_ccount, delta;

    /* Do this 3 times, mostly for the benchmark value - the first
       run includes a cache miss so uses more cycles than it should. */
    لكل (صحيح i = 0; i < 3; i++) {
        TaskHandle_t sender_task;
        کتار_Handle_t کتار = x_کتار_Create(1, حجم(uint32_t));
        trigger = false;
        flag = false;

        /* "send_to_queue" task sits on the other CPU, lower priority to us */
        x_Task_Create_Pinned_منزل_Core(task_send_منزل_کتار, "YIELD", 2048, (فراغ *)کتار, UNITY_FREERTOS_لومړیتوب - 1,
                                &sender_task, !UNITY_FREERTOS_CPU);

        vTaskDelay(2); /* make sure everything is set up */
        trigger = true;
        trigger_ccount = esp_cpu_get_دوره_شمېر();

        // yield_ccount is not useful in this test as it's the other core's CCOUNT
        // so we use trigger_ccount instead
        TEST_ASSERT(x_کتار_Receive(کتار, &yield_ccount, 100 / پورټ_TICK_PERIOD_MS));
        now_ccount = esp_cpu_get_دوره_شمېر();
        TEST_ASSERT(flag);

        delta = now_ccount - trigger_ccount;
        چاپ_کړه("Yielding from task on other core took %"PRIu32" cycles\n", delta);
        TEST_ASSERT(delta < MAX_YIELD_شمېر);

        v_کتار_Delete(کتار);
        vTaskDelete(sender_task);
    }
}
#د_که_پای
