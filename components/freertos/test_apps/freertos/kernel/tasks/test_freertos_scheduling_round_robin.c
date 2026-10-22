/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "sdkconfig.h"
#ضمن <stdbool.h>
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "freertos/queue.h"
#ضمن "esp_rom_sys.h"
#ضمن "esp_cpu.h"
#ضمن "unity.h"
#ضمن "test_utils.h"

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

#إذا !defined(CONFIG_FREERTOS_UNICORE) && (defined(CONFIG_FREERTOS_CORETIMER_0) || defined(CONFIG_FREERTOS_CORETIMER_1))

#وصف_ڪريو SPIN_TASK_PRIO                  (CONFIG_UNITY_FREERTOS_ترجيح + 1)
#وصف_ڪريو SPIN_TASK_انگ_ITER              3
#وصف_ڪريو TASK_ڍير_سائيز                 1024
#وصف_ڪريو انگ_PINNED_SPIN_TASK_في_CORE   3
#إذا defined(CONFIG_FREERTOS_CORETIMER_0)
#وصف_ڪريو TICK_INTR_IDX                   6
#وإلا   //defined(CONFIG_FREERTOS_CORETIMER_1)
#وصف_ڪريو TICK_INTR_IDX                   15
#جيڪڏهن_پورو

ساكن قطار_Handle_t core0_لاڳيتو_ترتيب_قطار;
ساكن قطار_Handle_t core1_لاڳيتو_ترتيب_قطار;
ساكن uint32_t ڪل_iter_ڳڻپ[CONFIG_FREERTOS_عدد_جو_CORES] = {0};

ساكن فراغ spin_task(فراغ *arg)
{
    uint32_t task_ڪوڊ = (uint32_t)arg;
    قطار_Handle_t لاڳيتو_ترتيب_قطار = ((task_ڪوڊ >> 4) == 0) ? core0_لاڳيتو_ترتيب_قطار : core1_لاڳيتو_ترتيب_قطار;

    //Wait to be started
    ulTaskNotifyTake(pdTRUE, پورٽ_MAX_DELAY);

    لكل (صحيح i = 0; i < SPIN_TASK_انگ_ITER; i++) {
        x_قطار_Send(لاڳيتو_ترتيب_قطار, &task_ڪوڊ, 0);
        //No need for critical sections as tick interrupt is disabled
        ڪل_iter_ڳڻپ[x_پورٽ_Get_Core_ID()]++;
        taskYIELD();
    }

    //Last iteration of the last spin task on this core. Reenable this core's tick interrupt
    إذا (ڪل_iter_ڳڻپ[x_پورٽ_Get_Core_ID()] == (انگ_PINNED_SPIN_TASK_في_CORE * SPIN_TASK_انگ_ITER)) {
        esp_cpu_intr_enable(1 << TICK_INTR_IDX);
    }
    vTaskDelete(عدم);
}

ساكن فراغ blocker_task(فراغ *arg)
{
    متطاير bool *exit_loop = (متطاير bool *)arg;

    //Disable tick interrupts on core 1 the duration of the test
    taskDISABLE_INTERRUPTS();
    esp_cpu_intr_disable(1 << TICK_INTR_IDX);
    taskENABLE_INTERRUPTS();

    طالما (!*exit_loop) {
        ;
    }

    //Wait to be resumed
    vTaskSuspend(عدم);

    //Reenable tick interrupt on core 1
    taskDISABLE_INTERRUPTS();
    esp_cpu_intr_enable(1 << TICK_INTR_IDX);
    taskENABLE_INTERRUPTS();

    vTaskDelete(عدم);
}

TEST_CASE("Test FreeRTOS Scheduling Round Robin", "[freertos]")
{
    core0_لاڳيتو_ترتيب_قطار = x_قطار_Create(SPIN_TASK_انگ_ITER * انگ_PINNED_SPIN_TASK_في_CORE, حجم(uint32_t));
    core1_لاڳيتو_ترتيب_قطار = x_قطار_Create(SPIN_TASK_انگ_ITER * انگ_PINNED_SPIN_TASK_في_CORE, حجم(uint32_t));

    /* Increase priority of unity task so that the spin tasks don't preempt us
    during task creation. */
    v_Task_ترجيح_سيٽ(عدم, SPIN_TASK_PRIO + 1);
    /* Create a task on core 1 of the same priority to block core 1 */
    متطاير bool suspend_blocker = false;
    TaskHandle_t blocker_task_hdl;
    x_Task_Create_Pinned_منزل_Core(blocker_task, "blk", TASK_ڍير_سائيز, (فراغ *)&suspend_blocker, SPIN_TASK_PRIO + 1, &blocker_task_hdl, 1);

    //Disable tick interrupts on core 0 the duration of the test
    taskDISABLE_INTERRUPTS();
    esp_cpu_intr_disable(1 << TICK_INTR_IDX);
    taskENABLE_INTERRUPTS();

    TaskHandle_t core0_task_hdls[انگ_PINNED_SPIN_TASK_في_CORE];
    TaskHandle_t core1_task_hdls[انگ_PINNED_SPIN_TASK_في_CORE];

    لكل (صحيح i = 0; i < انگ_PINNED_SPIN_TASK_في_CORE; i++) {
        //Create a spin task pinned to core 0
        x_Task_Create_Pinned_منزل_Core(spin_task, "spin", TASK_ڍير_سائيز, (فراغ *)(0x00 + i), SPIN_TASK_PRIO, &core0_task_hdls[i], 0);
        //Create a spin task pinned to core 1
        x_Task_Create_Pinned_منزل_Core(spin_task, "spin", TASK_ڍير_سائيز, (فراغ *)(0x10 + i), SPIN_TASK_PRIO, &core1_task_hdls[i], 1);
    }

    /* Start the tasks in a particular order. This order should be reflected in
    in the round robin scheduling on each core */
    لكل (صحيح i = 0; i < انگ_PINNED_SPIN_TASK_في_CORE; i++) {
        //Start a spin task on core 0
        xTaskNotifyGive(core0_task_hdls[i]);
        //Start a spin task on core 1
        xTaskNotifyGive(core1_task_hdls[i]);
    }

    //Lower priority of this task and stop blocker task to allow the spin tasks to be scheduled
    suspend_blocker = true;
    v_Task_ترجيح_سيٽ(عدم, UNITY_FREERTOS_ترجيح);

    //Give a enough delay to allow all iterations of the round robin to occur
    esp_rom_delay_us(10000);

    لكل (صحيح i = 0; i < SPIN_TASK_انگ_ITER; i++) {
        لكل (صحيح j = 0; j < انگ_PINNED_SPIN_TASK_في_CORE; j++) {
            uint32_t core0_داخلا;
            uint32_t core1_داخلا;
            TEST_ASSERT_EQUAL(pdTRUE, x_قطار_Receive(core0_لاڳيتو_ترتيب_قطار, &core0_داخلا, 0));
            TEST_ASSERT_EQUAL(pdTRUE, x_قطار_Receive(core1_لاڳيتو_ترتيب_قطار, &core1_داخلا, 0));
            TEST_ASSERT_EQUAL(0x00 + j, core0_داخلا);
            TEST_ASSERT_EQUAL(0x10 + j, core1_داخلا);
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
    v_قطار_Delete(core0_لاڳيتو_ترتيب_قطار);
    v_قطار_Delete(core1_لاڳيتو_ترتيب_قطار);
}

#جيڪڏهن_پورو  //!defined(CONFIG_FREERTOS_UNICORE) && (efined(CONFIG_FREERTOS_CORETIMER_0) || defined(CONFIG_FREERTOS_CORETIMER_1))
