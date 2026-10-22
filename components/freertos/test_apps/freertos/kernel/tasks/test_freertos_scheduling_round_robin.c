/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#समावेश "sdkconfig.h"
#समावेश <stdbool.h>
#समावेश "freertos/FreeRTOS.h"
#समावेश "freertos/task.h"
#समावेश "freertos/queue.h"
#समावेश "esp_rom_sys.h"
#समावेश "esp_cpu.h"
#समावेश "unity.h"
#समावेश "test_utils.h"

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

#अगर !defined(CONFIG_FREERTOS_UNICORE) && (defined(CONFIG_FREERTOS_CORETIMER_0) || defined(CONFIG_FREERTOS_CORETIMER_1))

#घोषणा SPIN_TASK_PRIO                  (CONFIG_UNITY_FREERTOS_PRIORITY + 1)
#घोषणा SPIN_TASK_अंक_ITER              3
#घोषणा TASK_ढेर_आकार                 1024
#घोषणा अंक_PINNED_SPIN_TASK_PER_CORE   3
#अगर defined(CONFIG_FREERTOS_CORETIMER_0)
#घोषणा TICK_INTR_IDX                   6
#अन्यथा   //defined(CONFIG_FREERTOS_CORETIMER_1)
#घोषणा TICK_INTR_IDX                   15
#पूर्ण_अगर

अटल QueueHandle_t core0_run_order_queue;
अटल QueueHandle_t core1_run_order_queue;
अटल uint32_t योग_iter_गिनती[CONFIG_FREERTOS_संख्या_OF_CORES] = {0};

अटल व्योम spin_task(व्योम *arg)
{
    uint32_t task_code = (uint32_t)arg;
    QueueHandle_t run_order_queue = ((task_code >> 4) == 0) ? core0_run_order_queue : core1_run_order_queue;

    //Wait to be started
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    क्रम (पूर्णांक i = 0; i < SPIN_TASK_अंक_ITER; i++) {
        xQueueSend(run_order_queue, &task_code, 0);
        //No need for critical sections as tick interrupt is disabled
        योग_iter_गिनती[xPortGetCoreID()]++;
        taskYIELD();
    }

    //Last iteration of the last spin task on this core. Reenable this core's tick interrupt
    अगर (योग_iter_गिनती[xPortGetCoreID()] == (अंक_PINNED_SPIN_TASK_PER_CORE * SPIN_TASK_अंक_ITER)) {
        esp_cpu_intr_enable(1 << TICK_INTR_IDX);
    }
    vTaskDelete(शून्य);
}

अटल व्योम blocker_task(व्योम *arg)
{
    अस्थिर bool *exit_loop = (अस्थिर bool *)arg;

    //Disable tick interrupts on core 1 the duration of the test
    taskDISABLE_INTERRUPTS();
    esp_cpu_intr_disable(1 << TICK_INTR_IDX);
    taskENABLE_INTERRUPTS();

    जबतक (!*exit_loop) {
        ;
    }

    //Wait to be resumed
    vTaskSuspend(शून्य);

    //Reenable tick interrupt on core 1
    taskDISABLE_INTERRUPTS();
    esp_cpu_intr_enable(1 << TICK_INTR_IDX);
    taskENABLE_INTERRUPTS();

    vTaskDelete(शून्य);
}

TEST_CASE("Test FreeRTOS Scheduling Round Robin", "[freertos]")
{
    core0_run_order_queue = xQueueCreate(SPIN_TASK_अंक_ITER * अंक_PINNED_SPIN_TASK_PER_CORE, माप(uint32_t));
    core1_run_order_queue = xQueueCreate(SPIN_TASK_अंक_ITER * अंक_PINNED_SPIN_TASK_PER_CORE, माप(uint32_t));

    /* Increase priority of unity task so that the spin tasks don't preempt us
    during task creation. */
    vTaskPrioritySet(शून्य, SPIN_TASK_PRIO + 1);
    /* Create a task on core 1 of the same priority to block core 1 */
    अस्थिर bool suspend_blocker = false;
    TaskHandle_t blocker_task_hdl;
    xTaskCreatePinnedToCore(blocker_task, "blk", TASK_ढेर_आकार, (व्योम *)&suspend_blocker, SPIN_TASK_PRIO + 1, &blocker_task_hdl, 1);

    //Disable tick interrupts on core 0 the duration of the test
    taskDISABLE_INTERRUPTS();
    esp_cpu_intr_disable(1 << TICK_INTR_IDX);
    taskENABLE_INTERRUPTS();

    TaskHandle_t core0_task_hdls[अंक_PINNED_SPIN_TASK_PER_CORE];
    TaskHandle_t core1_task_hdls[अंक_PINNED_SPIN_TASK_PER_CORE];

    क्रम (पूर्णांक i = 0; i < अंक_PINNED_SPIN_TASK_PER_CORE; i++) {
        //Create a spin task pinned to core 0
        xTaskCreatePinnedToCore(spin_task, "spin", TASK_ढेर_आकार, (व्योम *)(0x00 + i), SPIN_TASK_PRIO, &core0_task_hdls[i], 0);
        //Create a spin task pinned to core 1
        xTaskCreatePinnedToCore(spin_task, "spin", TASK_ढेर_आकार, (व्योम *)(0x10 + i), SPIN_TASK_PRIO, &core1_task_hdls[i], 1);
    }

    /* Start the tasks in a particular order. This order should be reflected in
    in the round robin scheduling on each core */
    क्रम (पूर्णांक i = 0; i < अंक_PINNED_SPIN_TASK_PER_CORE; i++) {
        //Start a spin task on core 0
        xTaskNotifyGive(core0_task_hdls[i]);
        //Start a spin task on core 1
        xTaskNotifyGive(core1_task_hdls[i]);
    }

    //Lower priority of this task and stop blocker task to allow the spin tasks to be scheduled
    suspend_blocker = true;
    vTaskPrioritySet(शून्य, UNITY_FREERTOS_PRIORITY);

    //Give a enough delay to allow all iterations of the round robin to occur
    esp_rom_delay_us(10000);

    क्रम (पूर्णांक i = 0; i < SPIN_TASK_अंक_ITER; i++) {
        क्रम (पूर्णांक j = 0; j < अंक_PINNED_SPIN_TASK_PER_CORE; j++) {
            uint32_t core0_entry;
            uint32_t core1_entry;
            TEST_ASSERT_EQUAL(pdTRUE, xQueueReceive(core0_run_order_queue, &core0_entry, 0));
            TEST_ASSERT_EQUAL(pdTRUE, xQueueReceive(core1_run_order_queue, &core1_entry, 0));
            TEST_ASSERT_EQUAL(0x00 + j, core0_entry);
            TEST_ASSERT_EQUAL(0x10 + j, core1_entry);
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
    vQueueDelete(core0_run_order_queue);
    vQueueDelete(core1_run_order_queue);
}

#पूर्ण_अगर  //!defined(CONFIG_FREERTOS_UNICORE) && (efined(CONFIG_FREERTOS_CORETIMER_0) || defined(CONFIG_FREERTOS_CORETIMER_1))
