/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/queue.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_rom_sys.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_cpu.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "test_utils.h"

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

#ਜੇ !defined(CONFIG_FREERTOS_UNICORE) && (defined(CONFIG_FREERTOS_CORETIMER_0) || defined(CONFIG_FREERTOS_CORETIMER_1))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SPIN_TASK_PRIO                  (CONFIG_UNITY_FREERTOS_ਤਰਜੀਹ + 1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SPIN_TASK_ਅੰਕ_ITER              3
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TASK_ਢੇਰ_ਮਾਪ                 1024
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਅੰਕ_PINNED_SPIN_TASK_ਪ੍ਰਤੀ_CORE   3
#ਜੇ defined(CONFIG_FREERTOS_CORETIMER_0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TICK_INTR_IDX                   6
#ਨਹੀਂਤਾਂ   //defined(CONFIG_FREERTOS_CORETIMER_1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TICK_INTR_IDX                   15
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਕਤਾਰ_Handle_t core0_ਲੜੀਵਾਰ_ਕ੍ਰਮ_ਕਤਾਰ;
ਸਥਿਰ ਕਤਾਰ_Handle_t core1_ਲੜੀਵਾਰ_ਕ੍ਰਮ_ਕਤਾਰ;
ਸਥਿਰ uint32_t ਕੁੱਲ_iter_ਗਿਣਤੀ[CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES] = {0};

ਸਥਿਰ ਖਾਲੀ spin_task(ਖਾਲੀ *arg)
{
    uint32_t task_ਕੋਡ = (uint32_t)arg;
    ਕਤਾਰ_Handle_t ਲੜੀਵਾਰ_ਕ੍ਰਮ_ਕਤਾਰ = ((task_ਕੋਡ >> 4) == 0) ? core0_ਲੜੀਵਾਰ_ਕ੍ਰਮ_ਕਤਾਰ : core1_ਲੜੀਵਾਰ_ਕ੍ਰਮ_ਕਤਾਰ;

    //Wait to be started
    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);

    ਲਈ (ਪੂਰਨ i = 0; i < SPIN_TASK_ਅੰਕ_ITER; i++) {
        x_ਕਤਾਰ_Send(ਲੜੀਵਾਰ_ਕ੍ਰਮ_ਕਤਾਰ, &task_ਕੋਡ, 0);
        //No need for critical sections as tick interrupt is disabled
        ਕੁੱਲ_iter_ਗਿਣਤੀ[x_ਪੋਰਟ_Get_Core_ID()]++;
        taskYIELD();
    }

    //Last iteration of the last spin task on this core. Reenable this core's tick interrupt
    ਜੇ (ਕੁੱਲ_iter_ਗਿਣਤੀ[x_ਪੋਰਟ_Get_Core_ID()] == (ਅੰਕ_PINNED_SPIN_TASK_ਪ੍ਰਤੀ_CORE * SPIN_TASK_ਅੰਕ_ITER)) {
        esp_cpu_intr_enable(1 << TICK_INTR_IDX);
    }
    vTaskDelete(ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ blocker_task(ਖਾਲੀ *arg)
{
    ਅਸਥਿਰ bool *exit_loop = (ਅਸਥਿਰ bool *)arg;

    //Disable tick interrupts on core 1 the duration of the test
    taskDISABLE_INTERRUPTS();
    esp_cpu_intr_disable(1 << TICK_INTR_IDX);
    taskENABLE_INTERRUPTS();

    ਜਦੋਂ (!*exit_loop) {
        ;
    }

    //Wait to be resumed
    vTaskSuspend(ਨਿਰਾ);

    //Reenable tick interrupt on core 1
    taskDISABLE_INTERRUPTS();
    esp_cpu_intr_enable(1 << TICK_INTR_IDX);
    taskENABLE_INTERRUPTS();

    vTaskDelete(ਨਿਰਾ);
}

TEST_CASE("Test FreeRTOS Scheduling Round Robin", "[freertos]")
{
    core0_ਲੜੀਵਾਰ_ਕ੍ਰਮ_ਕਤਾਰ = x_ਕਤਾਰ_Create(SPIN_TASK_ਅੰਕ_ITER * ਅੰਕ_PINNED_SPIN_TASK_ਪ੍ਰਤੀ_CORE, ਆਕਾਰ(uint32_t));
    core1_ਲੜੀਵਾਰ_ਕ੍ਰਮ_ਕਤਾਰ = x_ਕਤਾਰ_Create(SPIN_TASK_ਅੰਕ_ITER * ਅੰਕ_PINNED_SPIN_TASK_ਪ੍ਰਤੀ_CORE, ਆਕਾਰ(uint32_t));

    /* Increase priority of unity task so that the spin tasks don't preempt us
    during task creation. */
    v_Task_ਤਰਜੀਹ_ਸੈੱਟ(ਨਿਰਾ, SPIN_TASK_PRIO + 1);
    /* Create a task on core 1 of the same priority to block core 1 */
    ਅਸਥਿਰ bool suspend_blocker = false;
    TaskHandle_t blocker_task_hdl;
    x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(blocker_task, "blk", TASK_ਢੇਰ_ਮਾਪ, (ਖਾਲੀ *)&suspend_blocker, SPIN_TASK_PRIO + 1, &blocker_task_hdl, 1);

    //Disable tick interrupts on core 0 the duration of the test
    taskDISABLE_INTERRUPTS();
    esp_cpu_intr_disable(1 << TICK_INTR_IDX);
    taskENABLE_INTERRUPTS();

    TaskHandle_t core0_task_hdls[ਅੰਕ_PINNED_SPIN_TASK_ਪ੍ਰਤੀ_CORE];
    TaskHandle_t core1_task_hdls[ਅੰਕ_PINNED_SPIN_TASK_ਪ੍ਰਤੀ_CORE];

    ਲਈ (ਪੂਰਨ i = 0; i < ਅੰਕ_PINNED_SPIN_TASK_ਪ੍ਰਤੀ_CORE; i++) {
        //Create a spin task pinned to core 0
        x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(spin_task, "spin", TASK_ਢੇਰ_ਮਾਪ, (ਖਾਲੀ *)(0x00 + i), SPIN_TASK_PRIO, &core0_task_hdls[i], 0);
        //Create a spin task pinned to core 1
        x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(spin_task, "spin", TASK_ਢੇਰ_ਮਾਪ, (ਖਾਲੀ *)(0x10 + i), SPIN_TASK_PRIO, &core1_task_hdls[i], 1);
    }

    /* Start the tasks in a particular order. This order should be reflected in
    in the round robin scheduling on each core */
    ਲਈ (ਪੂਰਨ i = 0; i < ਅੰਕ_PINNED_SPIN_TASK_ਪ੍ਰਤੀ_CORE; i++) {
        //Start a spin task on core 0
        xTaskNotifyGive(core0_task_hdls[i]);
        //Start a spin task on core 1
        xTaskNotifyGive(core1_task_hdls[i]);
    }

    //Lower priority of this task and stop blocker task to allow the spin tasks to be scheduled
    suspend_blocker = true;
    v_Task_ਤਰਜੀਹ_ਸੈੱਟ(ਨਿਰਾ, UNITY_FREERTOS_ਤਰਜੀਹ);

    //Give a enough delay to allow all iterations of the round robin to occur
    esp_rom_delay_us(10000);

    ਲਈ (ਪੂਰਨ i = 0; i < SPIN_TASK_ਅੰਕ_ITER; i++) {
        ਲਈ (ਪੂਰਨ j = 0; j < ਅੰਕ_PINNED_SPIN_TASK_ਪ੍ਰਤੀ_CORE; j++) {
            uint32_t core0_ਇੰਦਰਾਜ;
            uint32_t core1_ਇੰਦਰਾਜ;
            TEST_ASSERT_EQUAL(pdTRUE, x_ਕਤਾਰ_Receive(core0_ਲੜੀਵਾਰ_ਕ੍ਰਮ_ਕਤਾਰ, &core0_ਇੰਦਰਾਜ, 0));
            TEST_ASSERT_EQUAL(pdTRUE, x_ਕਤਾਰ_Receive(core1_ਲੜੀਵਾਰ_ਕ੍ਰਮ_ਕਤਾਰ, &core1_ਇੰਦਰਾਜ, 0));
            TEST_ASSERT_EQUAL(0x00 + j, core0_ਇੰਦਰਾਜ);
            TEST_ASSERT_EQUAL(0x10 + j, core1_ਇੰਦਰਾਜ);
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
    v_ਕਤਾਰ_Delete(core0_ਲੜੀਵਾਰ_ਕ੍ਰਮ_ਕਤਾਰ);
    v_ਕਤਾਰ_Delete(core1_ਲੜੀਵਾਰ_ਕ੍ਰਮ_ਕਤਾਰ);
}

#ਜੇ_ਅੰਤ  //!defined(CONFIG_FREERTOS_UNICORE) && (efined(CONFIG_FREERTOS_CORETIMER_0) || defined(CONFIG_FREERTOS_CORETIMER_1))
