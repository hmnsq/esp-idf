/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Unit tests for FreeRTOS preemption
*/

#అన్తర్భావ <esp_types.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <inttypes.h>

#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "freertos/semphr.h"
#అన్తర్భావ "freertos/queue.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "esp_cpu.h"
#అన్తర్భావ "test_utils.h"
#అన్తర్భావ "sdkconfig.h"

స్థిర అస్థిర bool trigger;
స్థిర అస్థిర bool flag;

#నిర్వచితం_కాకపోతే CONFIG_FREERTOS_SMP
#నిర్వచించు MAX_YIELD_లెక్క 10000
#లేకపోతే
//TODO: IDF-5081
#నిర్వచించు MAX_YIELD_లెక్క 17000
#అయితే_ముగింపు // CONFIG_FREERTOS_SMP

/* Task:
   - Waits for 'trigger' variable to be set
   - Reads the cycle count on this CPU
   - Pushes it into a queue supplied as a param
   - Busy-waits until the main task terminates it
*/
స్థిర శూన్య task_send_గమ్యం_వరుస(శూన్య *param)
{
    వరుస_Handle_t వరుస = (వరుస_Handle_t) param;
    uint32_t ccount;

    అయితే (!trigger) {}

    ccount = esp_cpu_get_చక్రం_లెక్క();
    flag = true;
    x_వరుస_Send_గమ్యం_వెనుక(వరుస, &ccount, 0);
    /* This is to ensure that higher priority task
       won't wake anyhow, due to this task terminating.

       The task runs until terminated by the main task.
    */
    అయితే (1) {}
}

TEST_CASE("Yield from lower priority task, same CPU", "[freertos]")
{
    /* Do this 3 times, mostly for the benchmark value - the first
       run includes a cache miss so uses more cycles than it should. */
    కొరకు (పూర్ణాంకం i = 0; i < 3; i++) {
        TaskHandle_t sender_task;
        వరుస_Handle_t వరుస = x_వరుస_Create(1, పరిమాణం(uint32_t));
        flag = false;
        trigger = false;

        /* "yield" task sits on our CPU, lower priority to us */
        x_Task_Create_Pinned_గమ్యం_Core(task_send_గమ్యం_వరుస, "YIELD", 2048, (శూన్య *)వరుస, UNITY_FREERTOS_ప్రాధాన్యత - 1, &sender_task, UNITY_FREERTOS_CPU);

        vTaskDelay(1); /* make sure everything is set up */
        trigger = true;

        uint32_t yield_ccount, now_ccount, delta;
        TEST_ASSERT(x_వరుస_Receive(వరుస, &yield_ccount, 100 / పోర్టు_TICK_PERIOD_MS));
        now_ccount = esp_cpu_get_చక్రం_లెక్క();
        TEST_ASSERT(flag);

        delta = now_ccount - yield_ccount;
        రాయి("Yielding from lower priority task took %"PRIu32" cycles\n", delta);
        TEST_ASSERT(delta < MAX_YIELD_లెక్క);

        vTaskDelete(sender_task);
        v_వరుస_Delete(వరుస);
    }
}

#ఒకవేళ (CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES == 2) && CONFIG_FREERTOS_లోపల_IRAM
TEST_CASE("Yield from lower priority task, other CPU", "[freertos]")
{
    uint32_t trigger_ccount, yield_ccount, now_ccount, delta;

    /* Do this 3 times, mostly for the benchmark value - the first
       run includes a cache miss so uses more cycles than it should. */
    కొరకు (పూర్ణాంకం i = 0; i < 3; i++) {
        TaskHandle_t sender_task;
        వరుస_Handle_t వరుస = x_వరుస_Create(1, పరిమాణం(uint32_t));
        trigger = false;
        flag = false;

        /* "send_to_queue" task sits on the other CPU, lower priority to us */
        x_Task_Create_Pinned_గమ్యం_Core(task_send_గమ్యం_వరుస, "YIELD", 2048, (శూన్య *)వరుస, UNITY_FREERTOS_ప్రాధాన్యత - 1,
                                &sender_task, !UNITY_FREERTOS_CPU);

        vTaskDelay(2); /* make sure everything is set up */
        trigger = true;
        trigger_ccount = esp_cpu_get_చక్రం_లెక్క();

        // yield_ccount is not useful in this test as it's the other core's CCOUNT
        // so we use trigger_ccount instead
        TEST_ASSERT(x_వరుస_Receive(వరుస, &yield_ccount, 100 / పోర్టు_TICK_PERIOD_MS));
        now_ccount = esp_cpu_get_చక్రం_లెక్క();
        TEST_ASSERT(flag);

        delta = now_ccount - trigger_ccount;
        రాయి("Yielding from task on other core took %"PRIu32" cycles\n", delta);
        TEST_ASSERT(delta < MAX_YIELD_లెక్క);

        v_వరుస_Delete(వరుస);
        vTaskDelete(sender_task);
    }
}
#అయితే_ముగింపు
