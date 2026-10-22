/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Unit tests for FreeRTOS preemption
*/

#अन्तर्भाव <esp_types.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <inttypes.h>

#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"
#अन्तर्भाव "freertos/semphr.h"
#अन्तर्भाव "freertos/queue.h"
#अन्तर्भाव "unity.h"
#अन्तर्भाव "esp_cpu.h"
#अन्तर्भाव "test_utils.h"
#अन्तर्भाव "sdkconfig.h"

स्थिर चञ्चल bool trigger;
स्थिर चञ्चल bool flag;

#यदि_अपरिभाषितम् CONFIG_FREERTOS_SMP
#परिभाषय MAX_YIELD_गणनम् 10000
#अन्यथा
//TODO: IDF-5081
#परिभाषय MAX_YIELD_गणनम् 17000
#यदि_अन्तः // CONFIG_FREERTOS_SMP

/* Task:
   - Waits for 'trigger' variable to be set
   - Reads the cycle count on this CPU
   - Pushes it into a queue supplied as a param
   - Busy-waits until the main task terminates it
*/
स्थिर शून्य task_send_गन्तव्यम्_queue(शून्य *param)
{
    QueueHandle_t queue = (QueueHandle_t) param;
    uint32_t ccount;

    यावत् (!trigger) {}

    ccount = esp_cpu_get_आवृत्तिः_गणनम्();
    flag = true;
    x_Queue_Send_गन्तव्यम्_पृष्ठभागः(queue, &ccount, 0);
    /* This is to ensure that higher priority task
       won't wake anyhow, due to this task terminating.

       The task runs until terminated by the main task.
    */
    यावत् (1) {}
}

TEST_CASE("Yield from lower priority task, same CPU", "[freertos]")
{
    /* Do this 3 times, mostly for the benchmark value - the first
       run includes a cache miss so uses more cycles than it should. */
    प्रति (पूर्णाङ्क i = 0; i < 3; i++) {
        TaskHandle_t sender_task;
        QueueHandle_t queue = xQueueCreate(1, परिमाण(uint32_t));
        flag = false;
        trigger = false;

        /* "yield" task sits on our CPU, lower priority to us */
        x_Task_Create_Pinned_गन्तव्यम्_Core(task_send_गन्तव्यम्_queue, "YIELD", 2048, (शून्य *)queue, UNITY_FREERTOS_प्राथमिकता - 1, &sender_task, UNITY_FREERTOS_CPU);

        vTaskDelay(1); /* make sure everything is set up */
        trigger = true;

        uint32_t yield_ccount, now_ccount, delta;
        TEST_ASSERT(xQueueReceive(queue, &yield_ccount, 100 / पत्तनम्_TICK_PERIOD_MS));
        now_ccount = esp_cpu_get_आवृत्तिः_गणनम्();
        TEST_ASSERT(flag);

        delta = now_ccount - yield_ccount;
        प्रारूप_लिख("Yielding from lower priority task took %"PRIu32" cycles\n", delta);
        TEST_ASSERT(delta < MAX_YIELD_गणनम्);

        vTaskDelete(sender_task);
        vQueueDelete(queue);
    }
}

#यदि (CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES == 2) && CONFIG_FREERTOS_अन्तर्_IRAM
TEST_CASE("Yield from lower priority task, other CPU", "[freertos]")
{
    uint32_t trigger_ccount, yield_ccount, now_ccount, delta;

    /* Do this 3 times, mostly for the benchmark value - the first
       run includes a cache miss so uses more cycles than it should. */
    प्रति (पूर्णाङ्क i = 0; i < 3; i++) {
        TaskHandle_t sender_task;
        QueueHandle_t queue = xQueueCreate(1, परिमाण(uint32_t));
        trigger = false;
        flag = false;

        /* "send_to_queue" task sits on the other CPU, lower priority to us */
        x_Task_Create_Pinned_गन्तव्यम्_Core(task_send_गन्तव्यम्_queue, "YIELD", 2048, (शून्य *)queue, UNITY_FREERTOS_प्राथमिकता - 1,
                                &sender_task, !UNITY_FREERTOS_CPU);

        vTaskDelay(2); /* make sure everything is set up */
        trigger = true;
        trigger_ccount = esp_cpu_get_आवृत्तिः_गणनम्();

        // yield_ccount is not useful in this test as it's the other core's CCOUNT
        // so we use trigger_ccount instead
        TEST_ASSERT(xQueueReceive(queue, &yield_ccount, 100 / पत्तनम्_TICK_PERIOD_MS));
        now_ccount = esp_cpu_get_आवृत्तिः_गणनम्();
        TEST_ASSERT(flag);

        delta = now_ccount - trigger_ccount;
        प्रारूप_लिख("Yielding from task on other core took %"PRIu32" cycles\n", delta);
        TEST_ASSERT(delta < MAX_YIELD_गणनम्);

        vQueueDelete(queue);
        vTaskDelete(sender_task);
    }
}
#यदि_अन्तः
