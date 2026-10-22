/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#شامل "sdkconfig.h"
#شامل <stdbool.h>
#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل "freertos/queue.h"
#شامل "esp_rom_sys.h"
#شامل "esp_cpu.h"
#شامل "unity.h"
#شامل "test_utils.h"

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

#اگر !defined(CONFIG_FREERTOS_UNICORE) && (defined(CONFIG_FREERTOS_CORETIMER_0) || defined(CONFIG_FREERTOS_CORETIMER_1))

#تعریف_کرو SPIN_TASK_PRIO                  (CONFIG_UNITY_FREERTOS_ترجیح + 1)
#تعریف_کرو SPIN_TASK_عدد_ITER              3
#تعریف_کرو TASK_ڈھیر_سائز                 1024
#تعریف_کرو عدد_PINNED_SPIN_TASK_فی_CORE   3
#اگر defined(CONFIG_FREERTOS_CORETIMER_0)
#تعریف_کرو TICK_INTR_IDX                   6
#ورنہ   //defined(CONFIG_FREERTOS_CORETIMER_1)
#تعریف_کرو TICK_INTR_IDX                   15
#اگر_ختم

ساکن قطار_Handle_t core0_تسلسل_ترتیب_قطار;
ساکن قطار_Handle_t core1_تسلسل_ترتیب_قطار;
ساکن uint32_t کل_iter_گنتی[CONFIG_FREERTOS_نمبر_کا_CORES] = {0};

ساکن خالی spin_task(خالی *arg)
{
    uint32_t task_رمز = (uint32_t)arg;
    قطار_Handle_t تسلسل_ترتیب_قطار = ((task_رمز >> 4) == 0) ? core0_تسلسل_ترتیب_قطار : core1_تسلسل_ترتیب_قطار;

    //Wait to be started
    ulTaskNotifyTake(pdTRUE, پورٹ_MAX_DELAY);

    برائے (صحیح i = 0; i < SPIN_TASK_عدد_ITER; i++) {
        x_قطار_Send(تسلسل_ترتیب_قطار, &task_رمز, 0);
        //No need for critical sections as tick interrupt is disabled
        کل_iter_گنتی[x_پورٹ_Get_Core_ID()]++;
        taskYIELD();
    }

    //Last iteration of the last spin task on this core. Reenable this core's tick interrupt
    اگر (کل_iter_گنتی[x_پورٹ_Get_Core_ID()] == (عدد_PINNED_SPIN_TASK_فی_CORE * SPIN_TASK_عدد_ITER)) {
        esp_cpu_intr_enable(1 << TICK_INTR_IDX);
    }
    vTaskDelete(تہی);
}

ساکن خالی blocker_task(خالی *arg)
{
    متلون bool *exit_loop = (متلون bool *)arg;

    //Disable tick interrupts on core 1 the duration of the test
    taskDISABLE_INTERRUPTS();
    esp_cpu_intr_disable(1 << TICK_INTR_IDX);
    taskENABLE_INTERRUPTS();

    جب (!*exit_loop) {
        ;
    }

    //Wait to be resumed
    vTaskSuspend(تہی);

    //Reenable tick interrupt on core 1
    taskDISABLE_INTERRUPTS();
    esp_cpu_intr_enable(1 << TICK_INTR_IDX);
    taskENABLE_INTERRUPTS();

    vTaskDelete(تہی);
}

TEST_CASE("Test FreeRTOS Scheduling Round Robin", "[freertos]")
{
    core0_تسلسل_ترتیب_قطار = x_قطار_Create(SPIN_TASK_عدد_ITER * عدد_PINNED_SPIN_TASK_فی_CORE, حجم(uint32_t));
    core1_تسلسل_ترتیب_قطار = x_قطار_Create(SPIN_TASK_عدد_ITER * عدد_PINNED_SPIN_TASK_فی_CORE, حجم(uint32_t));

    /* Increase priority of unity task so that the spin tasks don't preempt us
    during task creation. */
    v_Task_ترجیح_سیٹ(تہی, SPIN_TASK_PRIO + 1);
    /* Create a task on core 1 of the same priority to block core 1 */
    متلون bool suspend_blocker = false;
    TaskHandle_t blocker_task_hdl;
    x_Task_Create_Pinned_مقصد_Core(blocker_task, "blk", TASK_ڈھیر_سائز, (خالی *)&suspend_blocker, SPIN_TASK_PRIO + 1, &blocker_task_hdl, 1);

    //Disable tick interrupts on core 0 the duration of the test
    taskDISABLE_INTERRUPTS();
    esp_cpu_intr_disable(1 << TICK_INTR_IDX);
    taskENABLE_INTERRUPTS();

    TaskHandle_t core0_task_hdls[عدد_PINNED_SPIN_TASK_فی_CORE];
    TaskHandle_t core1_task_hdls[عدد_PINNED_SPIN_TASK_فی_CORE];

    برائے (صحیح i = 0; i < عدد_PINNED_SPIN_TASK_فی_CORE; i++) {
        //Create a spin task pinned to core 0
        x_Task_Create_Pinned_مقصد_Core(spin_task, "spin", TASK_ڈھیر_سائز, (خالی *)(0x00 + i), SPIN_TASK_PRIO, &core0_task_hdls[i], 0);
        //Create a spin task pinned to core 1
        x_Task_Create_Pinned_مقصد_Core(spin_task, "spin", TASK_ڈھیر_سائز, (خالی *)(0x10 + i), SPIN_TASK_PRIO, &core1_task_hdls[i], 1);
    }

    /* Start the tasks in a particular order. This order should be reflected in
    in the round robin scheduling on each core */
    برائے (صحیح i = 0; i < عدد_PINNED_SPIN_TASK_فی_CORE; i++) {
        //Start a spin task on core 0
        xTaskNotifyGive(core0_task_hdls[i]);
        //Start a spin task on core 1
        xTaskNotifyGive(core1_task_hdls[i]);
    }

    //Lower priority of this task and stop blocker task to allow the spin tasks to be scheduled
    suspend_blocker = true;
    v_Task_ترجیح_سیٹ(تہی, UNITY_FREERTOS_ترجیح);

    //Give a enough delay to allow all iterations of the round robin to occur
    esp_rom_delay_us(10000);

    برائے (صحیح i = 0; i < SPIN_TASK_عدد_ITER; i++) {
        برائے (صحیح j = 0; j < عدد_PINNED_SPIN_TASK_فی_CORE; j++) {
            uint32_t core0_اندراج;
            uint32_t core1_اندراج;
            TEST_ASSERT_EQUAL(pdTRUE, x_قطار_Receive(core0_تسلسل_ترتیب_قطار, &core0_اندراج, 0));
            TEST_ASSERT_EQUAL(pdTRUE, x_قطار_Receive(core1_تسلسل_ترتیب_قطار, &core1_اندراج, 0));
            TEST_ASSERT_EQUAL(0x00 + j, core0_اندراج);
            TEST_ASSERT_EQUAL(0x10 + j, core1_اندراج);
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
    v_قطار_Delete(core0_تسلسل_ترتیب_قطار);
    v_قطار_Delete(core1_تسلسل_ترتیب_قطار);
}

#اگر_ختم  //!defined(CONFIG_FREERTOS_UNICORE) && (efined(CONFIG_FREERTOS_CORETIMER_0) || defined(CONFIG_FREERTOS_CORETIMER_1))
