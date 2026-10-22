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

#تعریف_کن SPIN_TASK_PRIO                  (CONFIG_UNITY_FREERTOS_اولویت + 1)
#تعریف_کن SPIN_TASK_رقم_ITER              3
#تعریف_کن TASK_پشته_اندازه                 1024
#تعریف_کن رقم_PINNED_SPIN_TASK_در_هر_CORE   3
#إذا defined(CONFIG_FREERTOS_CORETIMER_0)
#تعریف_کن TICK_INTR_IDX                   6
#وإلا   //defined(CONFIG_FREERTOS_CORETIMER_1)
#تعریف_کن TICK_INTR_IDX                   15
#پایان_اگر

ساكن صف_Handle_t core0_پیاپی_ترتیب_صف;
ساكن صف_Handle_t core1_پیاپی_ترتیب_صف;
ساكن uint32_t مجموع_iter_شمار[CONFIG_FREERTOS_عدد_از_CORES] = {0};

ساكن فراغ spin_task(فراغ *arg)
{
    uint32_t task_کد = (uint32_t)arg;
    صف_Handle_t پیاپی_ترتیب_صف = ((task_کد >> 4) == 0) ? core0_پیاپی_ترتیب_صف : core1_پیاپی_ترتیب_صف;

    //Wait to be started
    ulTaskNotifyTake(pdTRUE, پورت_MAX_DELAY);

    لكل (صحيح i = 0; i < SPIN_TASK_رقم_ITER; i++) {
        x_صف_Send(پیاپی_ترتیب_صف, &task_کد, 0);
        //No need for critical sections as tick interrupt is disabled
        مجموع_iter_شمار[x_پورت_Get_Core_ID()]++;
        taskYIELD();
    }

    //Last iteration of the last spin task on this core. Reenable this core's tick interrupt
    إذا (مجموع_iter_شمار[x_پورت_Get_Core_ID()] == (رقم_PINNED_SPIN_TASK_در_هر_CORE * SPIN_TASK_رقم_ITER)) {
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
    core0_پیاپی_ترتیب_صف = x_صف_Create(SPIN_TASK_رقم_ITER * رقم_PINNED_SPIN_TASK_در_هر_CORE, حجم(uint32_t));
    core1_پیاپی_ترتیب_صف = x_صف_Create(SPIN_TASK_رقم_ITER * رقم_PINNED_SPIN_TASK_در_هر_CORE, حجم(uint32_t));

    /* Increase priority of unity task so that the spin tasks don't preempt us
    during task creation. */
    v_Task_اولویت_مجموعه(عدم, SPIN_TASK_PRIO + 1);
    /* Create a task on core 1 of the same priority to block core 1 */
    متطاير bool suspend_blocker = false;
    TaskHandle_t blocker_task_hdl;
    x_Task_Create_Pinned_مقصد_Core(blocker_task, "blk", TASK_پشته_اندازه, (فراغ *)&suspend_blocker, SPIN_TASK_PRIO + 1, &blocker_task_hdl, 1);

    //Disable tick interrupts on core 0 the duration of the test
    taskDISABLE_INTERRUPTS();
    esp_cpu_intr_disable(1 << TICK_INTR_IDX);
    taskENABLE_INTERRUPTS();

    TaskHandle_t core0_task_hdls[رقم_PINNED_SPIN_TASK_در_هر_CORE];
    TaskHandle_t core1_task_hdls[رقم_PINNED_SPIN_TASK_در_هر_CORE];

    لكل (صحيح i = 0; i < رقم_PINNED_SPIN_TASK_در_هر_CORE; i++) {
        //Create a spin task pinned to core 0
        x_Task_Create_Pinned_مقصد_Core(spin_task, "spin", TASK_پشته_اندازه, (فراغ *)(0x00 + i), SPIN_TASK_PRIO, &core0_task_hdls[i], 0);
        //Create a spin task pinned to core 1
        x_Task_Create_Pinned_مقصد_Core(spin_task, "spin", TASK_پشته_اندازه, (فراغ *)(0x10 + i), SPIN_TASK_PRIO, &core1_task_hdls[i], 1);
    }

    /* Start the tasks in a particular order. This order should be reflected in
    in the round robin scheduling on each core */
    لكل (صحيح i = 0; i < رقم_PINNED_SPIN_TASK_در_هر_CORE; i++) {
        //Start a spin task on core 0
        xTaskNotifyGive(core0_task_hdls[i]);
        //Start a spin task on core 1
        xTaskNotifyGive(core1_task_hdls[i]);
    }

    //Lower priority of this task and stop blocker task to allow the spin tasks to be scheduled
    suspend_blocker = true;
    v_Task_اولویت_مجموعه(عدم, UNITY_FREERTOS_اولویت);

    //Give a enough delay to allow all iterations of the round robin to occur
    esp_rom_delay_us(10000);

    لكل (صحيح i = 0; i < SPIN_TASK_رقم_ITER; i++) {
        لكل (صحيح j = 0; j < رقم_PINNED_SPIN_TASK_در_هر_CORE; j++) {
            uint32_t core0_مدخل;
            uint32_t core1_مدخل;
            TEST_ASSERT_EQUAL(pdTRUE, x_صف_Receive(core0_پیاپی_ترتیب_صف, &core0_مدخل, 0));
            TEST_ASSERT_EQUAL(pdTRUE, x_صف_Receive(core1_پیاپی_ترتیب_صف, &core1_مدخل, 0));
            TEST_ASSERT_EQUAL(0x00 + j, core0_مدخل);
            TEST_ASSERT_EQUAL(0x10 + j, core1_مدخل);
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
    v_صف_Delete(core0_پیاپی_ترتیب_صف);
    v_صف_Delete(core1_پیاپی_ترتیب_صف);
}

#پایان_اگر  //!defined(CONFIG_FREERTOS_UNICORE) && (efined(CONFIG_FREERTOS_CORETIMER_0) || defined(CONFIG_FREERTOS_CORETIMER_1))
