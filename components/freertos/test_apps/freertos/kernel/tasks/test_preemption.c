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

#اگر_تعریف_نشده CONFIG_FREERTOS_SMP
#تعریف_کن MAX_YIELD_شمار 10000
#وإلا
//TODO: IDF-5081
#تعریف_کن MAX_YIELD_شمار 17000
#پایان_اگر // CONFIG_FREERTOS_SMP

/* Task:
   - Waits for 'trigger' variable to be set
   - Reads the cycle count on this CPU
   - Pushes it into a queue supplied as a param
   - Busy-waits until the main task terminates it
*/
ساكن فراغ task_send_مقصد_صف(فراغ *param)
{
    صف_Handle_t صف = (صف_Handle_t) param;
    uint32_t ccount;

    طالما (!trigger) {}

    ccount = esp_cpu_get_چرخه_شمار();
    flag = true;
    x_صف_Send_مقصد_پشت(صف, &ccount, 0);
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
        صف_Handle_t صف = x_صف_Create(1, حجم(uint32_t));
        flag = false;
        trigger = false;

        /* "yield" task sits on our CPU, lower priority to us */
        x_Task_Create_Pinned_مقصد_Core(task_send_مقصد_صف, "YIELD", 2048, (فراغ *)صف, UNITY_FREERTOS_اولویت - 1, &sender_task, UNITY_FREERTOS_CPU);

        vTaskDelay(1); /* make sure everything is set up */
        trigger = true;

        uint32_t yield_ccount, now_ccount, delta;
        TEST_ASSERT(x_صف_Receive(صف, &yield_ccount, 100 / پورت_TICK_PERIOD_MS));
        now_ccount = esp_cpu_get_چرخه_شمار();
        TEST_ASSERT(flag);

        delta = now_ccount - yield_ccount;
        چاپ_کن("Yielding from lower priority task took %"PRIu32" cycles\n", delta);
        TEST_ASSERT(delta < MAX_YIELD_شمار);

        vTaskDelete(sender_task);
        v_صف_Delete(صف);
    }
}

#إذا (CONFIG_FREERTOS_عدد_از_CORES == 2) && CONFIG_FREERTOS_درون_IRAM
TEST_CASE("Yield from lower priority task, other CPU", "[freertos]")
{
    uint32_t trigger_ccount, yield_ccount, now_ccount, delta;

    /* Do this 3 times, mostly for the benchmark value - the first
       run includes a cache miss so uses more cycles than it should. */
    لكل (صحيح i = 0; i < 3; i++) {
        TaskHandle_t sender_task;
        صف_Handle_t صف = x_صف_Create(1, حجم(uint32_t));
        trigger = false;
        flag = false;

        /* "send_to_queue" task sits on the other CPU, lower priority to us */
        x_Task_Create_Pinned_مقصد_Core(task_send_مقصد_صف, "YIELD", 2048, (فراغ *)صف, UNITY_FREERTOS_اولویت - 1,
                                &sender_task, !UNITY_FREERTOS_CPU);

        vTaskDelay(2); /* make sure everything is set up */
        trigger = true;
        trigger_ccount = esp_cpu_get_چرخه_شمار();

        // yield_ccount is not useful in this test as it's the other core's CCOUNT
        // so we use trigger_ccount instead
        TEST_ASSERT(x_صف_Receive(صف, &yield_ccount, 100 / پورت_TICK_PERIOD_MS));
        now_ccount = esp_cpu_get_چرخه_شمار();
        TEST_ASSERT(flag);

        delta = now_ccount - trigger_ccount;
        چاپ_کن("Yielding from task on other core took %"PRIu32" cycles\n", delta);
        TEST_ASSERT(delta < MAX_YIELD_شمار);

        v_صف_Delete(صف);
        vTaskDelete(sender_task);
    }
}
#پایان_اگر
