/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव <stdbool.h>
#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"
#अन्तर्भाव "freertos/queue.h"
#अन्तर्भाव "esp_rom_sys.h"
#अन्तर्भाव "esp_cpu.h"
#अन्तर्भाव "unity.h"
#अन्तर्भाव "test_utils.h"

/*
Test Best Effort Round Robin Scheduling:

The following test case tests the "Best Effort Round Robin Scheduling" that fixes the skipping behavior found in older
versions of the ESP-IDF SMP FreeRTOS (see docs for more details about Best Effort Round Robin Scheduling).

This test...
- Only runs under dual core configuration
- Will disable the tick interrupts of both cores

Test flow as follows:

1. Stop preemption on core 0 by raising the priority of the unity task
2. Stop preemption on core 0 by creating a blocker task
3. Disable tick interrupts on both cores
4. Create N spin tasks on each core, each with a sequential task_code
5. Unblock those spin tasks in a sequential order
6. Lower priority of unity task and stop the blocker task so that spin tasks are run
7. Each time a spin task is run (i.e., an iteration) it will send its task code to a queue
8. Spin tasks will clean themselves up
9. The queue should contain the task codes of the spin tasks in the order they were started in, thus showing that round
   robin schedules the tasks in a sequential order.
*/

#यदि !defined(CONFIG_FREERTOS_UNICORE) && (defined(CONFIG_FREERTOS_CORETIMER_0) || defined(CONFIG_FREERTOS_CORETIMER_1))

#परिभाषय SPIN_TASK_PRIO                  (CONFIG_UNITY_FREERTOS_प्राथमिकता + 1)
#परिभाषय SPIN_TASK_अङ्कः_ITER              3
#परिभाषय TASK_राशिः_आकार                 1024
#परिभाषय अङ्कः_PINNED_SPIN_TASK_प्रत्येकम्_CORE   3
#यदि defined(CONFIG_FREERTOS_CORETIMER_0)
#परिभाषय TICK_INTR_IDX                   6
#अन्यथा   //defined(CONFIG_FREERTOS_CORETIMER_1)
#परिभाषय TICK_INTR_IDX                   15
#यदि_अन्तः

स्थिर QueueHandle_t core0_सततम्_क्रमः_queue;
स्थिर QueueHandle_t core1_सततम्_क्रमः_queue;
स्थिर uint32_t योग_iter_गणनम्[CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES] = {0};

स्थिर शून्य spin_task(शून्य *arg)
{
    uint32_t task_संहिता = (uint32_t)arg;
    QueueHandle_t सततम्_क्रमः_queue = ((task_संहिता >> 4) == 0) ? core0_सततम्_क्रमः_queue : core1_सततम्_क्रमः_queue;

    //Wait to be started
    ulTaskNotifyTake(pdTRUE, पत्तनम्_MAX_DELAY);

    प्रति (पूर्णाङ्क i = 0; i < SPIN_TASK_अङ्कः_ITER; i++) {
        xQueueSend(सततम्_क्रमः_queue, &task_संहिता, 0);
        //No need for critical sections as tick interrupt is disabled
        योग_iter_गणनम्[x_पत्तनम्_Get_Core_ID()]++;
        taskYIELD();
    }

    //Last iteration of the last spin task on this core. Reenable this core's tick interrupt
    यदि (योग_iter_गणनम्[x_पत्तनम्_Get_Core_ID()] == (अङ्कः_PINNED_SPIN_TASK_प्रत्येकम्_CORE * SPIN_TASK_अङ्कः_ITER)) {
        esp_cpu_intr_enable(1 << TICK_INTR_IDX);
    }
    vTaskDelete(रिक्त);
}

स्थिर शून्य blocker_task(शून्य *arg)
{
    चञ्चल bool *exit_loop = (चञ्चल bool *)arg;

    //Disable tick interrupts on core 1 the duration of the test
    taskDISABLE_INTERRUPTS();
    esp_cpu_intr_disable(1 << TICK_INTR_IDX);
    taskENABLE_INTERRUPTS();

    यावत् (!*exit_loop) {
        ;
    }

    //Wait to be resumed
    vTaskSuspend(रिक्त);

    //Reenable tick interrupt on core 1
    taskDISABLE_INTERRUPTS();
    esp_cpu_intr_enable(1 << TICK_INTR_IDX);
    taskENABLE_INTERRUPTS();

    vTaskDelete(रिक्त);
}

TEST_CASE("Test FreeRTOS Scheduling Round Robin", "[freertos]")
{
    core0_सततम्_क्रमः_queue = xQueueCreate(SPIN_TASK_अङ्कः_ITER * अङ्कः_PINNED_SPIN_TASK_प्रत्येकम्_CORE, परिमाण(uint32_t));
    core1_सततम्_क्रमः_queue = xQueueCreate(SPIN_TASK_अङ्कः_ITER * अङ्कः_PINNED_SPIN_TASK_प्रत्येकम्_CORE, परिमाण(uint32_t));

    /* Increase priority of unity task so that the spin tasks don't preempt us
    during task creation. */
    v_Task_प्राथमिकता_समुच्चयः(रिक्त, SPIN_TASK_PRIO + 1);
    /* Create a task on core 1 of the same priority to block core 1 */
    चञ्चल bool suspend_blocker = false;
    TaskHandle_t blocker_task_hdl;
    x_Task_Create_Pinned_गन्तव्यम्_Core(blocker_task, "blk", TASK_राशिः_आकार, (शून्य *)&suspend_blocker, SPIN_TASK_PRIO + 1, &blocker_task_hdl, 1);

    //Disable tick interrupts on core 0 the duration of the test
    taskDISABLE_INTERRUPTS();
    esp_cpu_intr_disable(1 << TICK_INTR_IDX);
    taskENABLE_INTERRUPTS();

    TaskHandle_t core0_task_hdls[अङ्कः_PINNED_SPIN_TASK_प्रत्येकम्_CORE];
    TaskHandle_t core1_task_hdls[अङ्कः_PINNED_SPIN_TASK_प्रत्येकम्_CORE];

    प्रति (पूर्णाङ्क i = 0; i < अङ्कः_PINNED_SPIN_TASK_प्रत्येकम्_CORE; i++) {
        //Create a spin task pinned to core 0
        x_Task_Create_Pinned_गन्तव्यम्_Core(spin_task, "spin", TASK_राशिः_आकार, (शून्य *)(0x00 + i), SPIN_TASK_PRIO, &core0_task_hdls[i], 0);
        //Create a spin task pinned to core 1
        x_Task_Create_Pinned_गन्तव्यम्_Core(spin_task, "spin", TASK_राशिः_आकार, (शून्य *)(0x10 + i), SPIN_TASK_PRIO, &core1_task_hdls[i], 1);
    }

    /* Start the tasks in a particular order. This order should be reflected in
    in the round robin scheduling on each core */
    प्रति (पूर्णाङ्क i = 0; i < अङ्कः_PINNED_SPIN_TASK_प्रत्येकम्_CORE; i++) {
        //Start a spin task on core 0
        xTaskNotifyGive(core0_task_hdls[i]);
        //Start a spin task on core 1
        xTaskNotifyGive(core1_task_hdls[i]);
    }

    //Lower priority of this task and stop blocker task to allow the spin tasks to be scheduled
    suspend_blocker = true;
    v_Task_प्राथमिकता_समुच्चयः(रिक्त, UNITY_FREERTOS_प्राथमिकता);

    //Give a enough delay to allow all iterations of the round robin to occur
    esp_rom_delay_us(10000);

    प्रति (पूर्णाङ्क i = 0; i < SPIN_TASK_अङ्कः_ITER; i++) {
        प्रति (पूर्णाङ्क j = 0; j < अङ्कः_PINNED_SPIN_TASK_प्रत्येकम्_CORE; j++) {
            uint32_t core0_प्रविष्टिः;
            uint32_t core1_प्रविष्टिः;
            TEST_ASSERT_EQUAL(pdTRUE, xQueueReceive(core0_सततम्_क्रमः_queue, &core0_प्रविष्टिः, 0));
            TEST_ASSERT_EQUAL(pdTRUE, xQueueReceive(core1_सततम्_क्रमः_queue, &core1_प्रविष्टिः, 0));
            TEST_ASSERT_EQUAL(0x00 + j, core0_प्रविष्टिः);
            TEST_ASSERT_EQUAL(0x10 + j, core1_प्रविष्टिः);
        }
    }

    //Resume the blocker task for cleanup
    vTaskResume(blocker_task_hdl);
    //Reenable tick interrupt on core 0
    taskDISABLE_INTERRUPTS();
    esp_cpu_intr_enable(1 << TICK_INTR_IDX);
    taskENABLE_INTERRUPTS();
    vTaskDelay(10); //Wait for blocker task to clean up
    //Clean up queues
    vQueueDelete(core0_सततम्_क्रमः_queue);
    vQueueDelete(core1_सततम्_क्रमः_queue);
}

#यदि_अन्तः  //!defined(CONFIG_FREERTOS_UNICORE) && (efined(CONFIG_FREERTOS_CORETIMER_0) || defined(CONFIG_FREERTOS_CORETIMER_1))
