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

#تعریف_کړه SPIN_TASK_PRIO                  (CONFIG_UNITY_FREERTOS_لومړیتوب + 1)
#تعریف_کړه SPIN_TASK_عدد_ITER              3
#تعریف_کړه TASK_ډېری_اندازه                 1024
#تعریف_کړه عدد_PINNED_SPIN_TASK_فی_CORE   3
#إذا defined(CONFIG_FREERTOS_CORETIMER_0)
#تعریف_کړه TICK_INTR_IDX                   6
#وإلا   //defined(CONFIG_FREERTOS_CORETIMER_1)
#تعریف_کړه TICK_INTR_IDX                   15
#د_که_پای

ساكن کتار_Handle_t core0_پرلپسې_ترتیب_کتار;
ساكن کتار_Handle_t core1_پرلپسې_ترتیب_کتار;
ساكن uint32_t ټول_iter_شمېر[CONFIG_FREERTOS_شمېره_د_CORES] = {0};

ساكن فراغ spin_task(فراغ *arg)
{
    uint32_t task_کوډ = (uint32_t)arg;
    کتار_Handle_t پرلپسې_ترتیب_کتار = ((task_کوډ >> 4) == 0) ? core0_پرلپسې_ترتیب_کتار : core1_پرلپسې_ترتیب_کتار;

    //Wait to be started
    ulTaskNotifyTake(pdTRUE, پورټ_MAX_DELAY);

    لكل (صحيح i = 0; i < SPIN_TASK_عدد_ITER; i++) {
        x_کتار_Send(پرلپسې_ترتیب_کتار, &task_کوډ, 0);
        //No need for critical sections as tick interrupt is disabled
        ټول_iter_شمېر[x_پورټ_Get_Core_ID()]++;
        taskYIELD();
    }

    //Last iteration of the last spin task on this core. Reenable this core's tick interrupt
    إذا (ټول_iter_شمېر[x_پورټ_Get_Core_ID()] == (عدد_PINNED_SPIN_TASK_فی_CORE * SPIN_TASK_عدد_ITER)) {
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
    core0_پرلپسې_ترتیب_کتار = x_کتار_Create(SPIN_TASK_عدد_ITER * عدد_PINNED_SPIN_TASK_فی_CORE, حجم(uint32_t));
    core1_پرلپسې_ترتیب_کتار = x_کتار_Create(SPIN_TASK_عدد_ITER * عدد_PINNED_SPIN_TASK_فی_CORE, حجم(uint32_t));

    /* Increase priority of unity task so that the spin tasks don't preempt us
    during task creation. */
    v_Task_لومړیتوب_سېټ(عدم, SPIN_TASK_PRIO + 1);
    /* Create a task on core 1 of the same priority to block core 1 */
    متطاير bool suspend_blocker = false;
    TaskHandle_t blocker_task_hdl;
    x_Task_Create_Pinned_منزل_Core(blocker_task, "blk", TASK_ډېری_اندازه, (فراغ *)&suspend_blocker, SPIN_TASK_PRIO + 1, &blocker_task_hdl, 1);

    //Disable tick interrupts on core 0 the duration of the test
    taskDISABLE_INTERRUPTS();
    esp_cpu_intr_disable(1 << TICK_INTR_IDX);
    taskENABLE_INTERRUPTS();

    TaskHandle_t core0_task_hdls[عدد_PINNED_SPIN_TASK_فی_CORE];
    TaskHandle_t core1_task_hdls[عدد_PINNED_SPIN_TASK_فی_CORE];

    لكل (صحيح i = 0; i < عدد_PINNED_SPIN_TASK_فی_CORE; i++) {
        //Create a spin task pinned to core 0
        x_Task_Create_Pinned_منزل_Core(spin_task, "spin", TASK_ډېری_اندازه, (فراغ *)(0x00 + i), SPIN_TASK_PRIO, &core0_task_hdls[i], 0);
        //Create a spin task pinned to core 1
        x_Task_Create_Pinned_منزل_Core(spin_task, "spin", TASK_ډېری_اندازه, (فراغ *)(0x10 + i), SPIN_TASK_PRIO, &core1_task_hdls[i], 1);
    }

    /* Start the tasks in a particular order. This order should be reflected in
    in the round robin scheduling on each core */
    لكل (صحيح i = 0; i < عدد_PINNED_SPIN_TASK_فی_CORE; i++) {
        //Start a spin task on core 0
        xTaskNotifyGive(core0_task_hdls[i]);
        //Start a spin task on core 1
        xTaskNotifyGive(core1_task_hdls[i]);
    }

    //Lower priority of this task and stop blocker task to allow the spin tasks to be scheduled
    suspend_blocker = true;
    v_Task_لومړیتوب_سېټ(عدم, UNITY_FREERTOS_لومړیتوب);

    //Give a enough delay to allow all iterations of the round robin to occur
    esp_rom_delay_us(10000);

    لكل (صحيح i = 0; i < SPIN_TASK_عدد_ITER; i++) {
        لكل (صحيح j = 0; j < عدد_PINNED_SPIN_TASK_فی_CORE; j++) {
            uint32_t core0_ثبت;
            uint32_t core1_ثبت;
            TEST_ASSERT_EQUAL(pdTRUE, x_کتار_Receive(core0_پرلپسې_ترتیب_کتار, &core0_ثبت, 0));
            TEST_ASSERT_EQUAL(pdTRUE, x_کتار_Receive(core1_پرلپسې_ترتیب_کتار, &core1_ثبت, 0));
            TEST_ASSERT_EQUAL(0x00 + j, core0_ثبت);
            TEST_ASSERT_EQUAL(0x10 + j, core1_ثبت);
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
    v_کتار_Delete(core0_پرلپسې_ترتیب_کتار);
    v_کتار_Delete(core1_پرلپسې_ترتیب_کتار);
}

#د_که_پای  //!defined(CONFIG_FREERTOS_UNICORE) && (efined(CONFIG_FREERTOS_CORETIMER_0) || defined(CONFIG_FREERTOS_CORETIMER_1))
