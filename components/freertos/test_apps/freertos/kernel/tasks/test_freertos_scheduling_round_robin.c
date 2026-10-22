/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ <stdbool.h>
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "freertos/queue.h"
#అన్తర్భావ "esp_rom_sys.h"
#అన్తర్భావ "esp_cpu.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "test_utils.h"

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

#ఒకవేళ !defined(CONFIG_FREERTOS_UNICORE) && (defined(CONFIG_FREERTOS_CORETIMER_0) || defined(CONFIG_FREERTOS_CORETIMER_1))

#నిర్వచించు SPIN_TASK_PRIO                  (CONFIG_UNITY_FREERTOS_ప్రాధాన్యత + 1)
#నిర్వచించు SPIN_TASK_అంకె_ITER              3
#నిర్వచించు TASK_దొంతర_కొలత                 1024
#నిర్వచించు అంకె_PINNED_SPIN_TASK_చొప్పున_CORE   3
#ఒకవేళ defined(CONFIG_FREERTOS_CORETIMER_0)
#నిర్వచించు TICK_INTR_IDX                   6
#లేకపోతే   //defined(CONFIG_FREERTOS_CORETIMER_1)
#నిర్వచించు TICK_INTR_IDX                   15
#అయితే_ముగింపు

స్థిర వరుస_Handle_t core0_నిరంతరం_క్రమం_వరుస;
స్థిర వరుస_Handle_t core1_నిరంతరం_క్రమం_వరుస;
స్థిర uint32_t మొత్తం_iter_లెక్క[CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES] = {0};

స్థిర శూన్య spin_task(శూన్య *arg)
{
    uint32_t task_సంకేతం = (uint32_t)arg;
    వరుస_Handle_t నిరంతరం_క్రమం_వరుస = ((task_సంకేతం >> 4) == 0) ? core0_నిరంతరం_క్రమం_వరుస : core1_నిరంతరం_క్రమం_వరుస;

    //Wait to be started
    ulTaskNotifyTake(pdTRUE, పోర్టు_MAX_DELAY);

    కొరకు (పూర్ణాంకం i = 0; i < SPIN_TASK_అంకె_ITER; i++) {
        x_వరుస_Send(నిరంతరం_క్రమం_వరుస, &task_సంకేతం, 0);
        //No need for critical sections as tick interrupt is disabled
        మొత్తం_iter_లెక్క[x_పోర్టు_Get_Core_ID()]++;
        taskYIELD();
    }

    //Last iteration of the last spin task on this core. Reenable this core's tick interrupt
    ఒకవేళ (మొత్తం_iter_లెక్క[x_పోర్టు_Get_Core_ID()] == (అంకె_PINNED_SPIN_TASK_చొప్పున_CORE * SPIN_TASK_అంకె_ITER)) {
        esp_cpu_intr_enable(1 << TICK_INTR_IDX);
    }
    vTaskDelete(ఖాళీ);
}

స్థిర శూన్య blocker_task(శూన్య *arg)
{
    అస్థిర bool *exit_loop = (అస్థిర bool *)arg;

    //Disable tick interrupts on core 1 the duration of the test
    taskDISABLE_INTERRUPTS();
    esp_cpu_intr_disable(1 << TICK_INTR_IDX);
    taskENABLE_INTERRUPTS();

    అయితే (!*exit_loop) {
        ;
    }

    //Wait to be resumed
    vTaskSuspend(ఖాళీ);

    //Reenable tick interrupt on core 1
    taskDISABLE_INTERRUPTS();
    esp_cpu_intr_enable(1 << TICK_INTR_IDX);
    taskENABLE_INTERRUPTS();

    vTaskDelete(ఖాళీ);
}

TEST_CASE("Test FreeRTOS Scheduling Round Robin", "[freertos]")
{
    core0_నిరంతరం_క్రమం_వరుస = x_వరుస_Create(SPIN_TASK_అంకె_ITER * అంకె_PINNED_SPIN_TASK_చొప్పున_CORE, పరిమాణం(uint32_t));
    core1_నిరంతరం_క్రమం_వరుస = x_వరుస_Create(SPIN_TASK_అంకె_ITER * అంకె_PINNED_SPIN_TASK_చొప్పున_CORE, పరిమాణం(uint32_t));

    /* Increase priority of unity task so that the spin tasks don't preempt us
    during task creation. */
    v_Task_ప్రాధాన్యత_సమితి(ఖాళీ, SPIN_TASK_PRIO + 1);
    /* Create a task on core 1 of the same priority to block core 1 */
    అస్థిర bool suspend_blocker = false;
    TaskHandle_t blocker_task_hdl;
    x_Task_Create_Pinned_గమ్యం_Core(blocker_task, "blk", TASK_దొంతర_కొలత, (శూన్య *)&suspend_blocker, SPIN_TASK_PRIO + 1, &blocker_task_hdl, 1);

    //Disable tick interrupts on core 0 the duration of the test
    taskDISABLE_INTERRUPTS();
    esp_cpu_intr_disable(1 << TICK_INTR_IDX);
    taskENABLE_INTERRUPTS();

    TaskHandle_t core0_task_hdls[అంకె_PINNED_SPIN_TASK_చొప్పున_CORE];
    TaskHandle_t core1_task_hdls[అంకె_PINNED_SPIN_TASK_చొప్పున_CORE];

    కొరకు (పూర్ణాంకం i = 0; i < అంకె_PINNED_SPIN_TASK_చొప్పున_CORE; i++) {
        //Create a spin task pinned to core 0
        x_Task_Create_Pinned_గమ్యం_Core(spin_task, "spin", TASK_దొంతర_కొలత, (శూన్య *)(0x00 + i), SPIN_TASK_PRIO, &core0_task_hdls[i], 0);
        //Create a spin task pinned to core 1
        x_Task_Create_Pinned_గమ్యం_Core(spin_task, "spin", TASK_దొంతర_కొలత, (శూన్య *)(0x10 + i), SPIN_TASK_PRIO, &core1_task_hdls[i], 1);
    }

    /* Start the tasks in a particular order. This order should be reflected in
    in the round robin scheduling on each core */
    కొరకు (పూర్ణాంకం i = 0; i < అంకె_PINNED_SPIN_TASK_చొప్పున_CORE; i++) {
        //Start a spin task on core 0
        xTaskNotifyGive(core0_task_hdls[i]);
        //Start a spin task on core 1
        xTaskNotifyGive(core1_task_hdls[i]);
    }

    //Lower priority of this task and stop blocker task to allow the spin tasks to be scheduled
    suspend_blocker = true;
    v_Task_ప్రాధాన్యత_సమితి(ఖాళీ, UNITY_FREERTOS_ప్రాధాన్యత);

    //Give a enough delay to allow all iterations of the round robin to occur
    esp_rom_delay_us(10000);

    కొరకు (పూర్ణాంకం i = 0; i < SPIN_TASK_అంకె_ITER; i++) {
        కొరకు (పూర్ణాంకం j = 0; j < అంకె_PINNED_SPIN_TASK_చొప్పున_CORE; j++) {
            uint32_t core0_నమోదు;
            uint32_t core1_నమోదు;
            TEST_ASSERT_EQUAL(pdTRUE, x_వరుస_Receive(core0_నిరంతరం_క్రమం_వరుస, &core0_నమోదు, 0));
            TEST_ASSERT_EQUAL(pdTRUE, x_వరుస_Receive(core1_నిరంతరం_క్రమం_వరుస, &core1_నమోదు, 0));
            TEST_ASSERT_EQUAL(0x00 + j, core0_నమోదు);
            TEST_ASSERT_EQUAL(0x10 + j, core1_నమోదు);
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
    v_వరుస_Delete(core0_నిరంతరం_క్రమం_వరుస);
    v_వరుస_Delete(core1_నిరంతరం_క్రమం_వరుస);
}

#అయితే_ముగింపు  //!defined(CONFIG_FREERTOS_UNICORE) && (efined(CONFIG_FREERTOS_CORETIMER_0) || defined(CONFIG_FREERTOS_CORETIMER_1))
