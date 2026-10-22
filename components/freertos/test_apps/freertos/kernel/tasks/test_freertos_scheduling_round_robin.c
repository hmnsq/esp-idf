/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "freertos/queue.h"
#ಅನ್ತರ್ಭಾವ "esp_rom_sys.h"
#ಅನ್ತರ್ಭಾವ "esp_cpu.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "test_utils.h"

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

#ಆದರೆ !defined(CONFIG_FREERTOS_UNICORE) && (defined(CONFIG_FREERTOS_CORETIMER_0) || defined(CONFIG_FREERTOS_CORETIMER_1))

#ವ್ಯಾಖ್ಯಾನಿಸು SPIN_TASK_PRIO                  (CONFIG_UNITY_FREERTOS_ಆದ್ಯತೆ + 1)
#ವ್ಯಾಖ್ಯಾನಿಸು SPIN_TASK_ಅಂಕೆ_ITER              3
#ವ್ಯಾಖ್ಯಾನಿಸು TASK_ರಾಶಿ_ಗಾತ್ರ                 1024
#ವ್ಯಾಖ್ಯಾನಿಸು ಅಂಕೆ_PINNED_SPIN_TASK_ಪ್ರತಿ_CORE   3
#ಆದರೆ defined(CONFIG_FREERTOS_CORETIMER_0)
#ವ್ಯಾಖ್ಯಾನಿಸು TICK_INTR_IDX                   6
#ಇಲ್ಲದಿದ್ದರೆ   //defined(CONFIG_FREERTOS_CORETIMER_1)
#ವ್ಯಾಖ್ಯಾನಿಸು TICK_INTR_IDX                   15
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಸರತಿ_Handle_t core0_ಸತತ_ಕ್ರಮ_ಸರತಿ;
ಸ್ಥಿರ ಸರತಿ_Handle_t core1_ಸತತ_ಕ್ರಮ_ಸರತಿ;
ಸ್ಥಿರ uint32_t ಒಟ್ಟು_iter_ಎಣಿಕೆ[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES] = {0};

ಸ್ಥಿರ ಶೂನ್ಯ spin_task(ಶೂನ್ಯ *arg)
{
    uint32_t task_ಸಂಕೇತ = (uint32_t)arg;
    ಸರತಿ_Handle_t ಸತತ_ಕ್ರಮ_ಸರತಿ = ((task_ಸಂಕೇತ >> 4) == 0) ? core0_ಸತತ_ಕ್ರಮ_ಸರತಿ : core1_ಸತತ_ಕ್ರಮ_ಸರತಿ;

    //Wait to be started
    ulTaskNotifyTake(pdTRUE, ಪೋರ್ಟ್_MAX_DELAY);

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < SPIN_TASK_ಅಂಕೆ_ITER; i++) {
        x_ಸರತಿ_Send(ಸತತ_ಕ್ರಮ_ಸರತಿ, &task_ಸಂಕೇತ, 0);
        //No need for critical sections as tick interrupt is disabled
        ಒಟ್ಟು_iter_ಎಣಿಕೆ[x_ಪೋರ್ಟ್_Get_Core_ID()]++;
        taskYIELD();
    }

    //Last iteration of the last spin task on this core. Reenable this core's tick interrupt
    ಆದರೆ (ಒಟ್ಟು_iter_ಎಣಿಕೆ[x_ಪೋರ್ಟ್_Get_Core_ID()] == (ಅಂಕೆ_PINNED_SPIN_TASK_ಪ್ರತಿ_CORE * SPIN_TASK_ಅಂಕೆ_ITER)) {
        esp_cpu_intr_enable(1 << TICK_INTR_IDX);
    }
    vTaskDelete(ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ blocker_task(ಶೂನ್ಯ *arg)
{
    ಅಸ್ಥಿರ bool *exit_loop = (ಅಸ್ಥಿರ bool *)arg;

    //Disable tick interrupts on core 1 the duration of the test
    taskDISABLE_INTERRUPTS();
    esp_cpu_intr_disable(1 << TICK_INTR_IDX);
    taskENABLE_INTERRUPTS();

    ಆಗ (!*exit_loop) {
        ;
    }

    //Wait to be resumed
    vTaskSuspend(ಖಾಲಿ);

    //Reenable tick interrupt on core 1
    taskDISABLE_INTERRUPTS();
    esp_cpu_intr_enable(1 << TICK_INTR_IDX);
    taskENABLE_INTERRUPTS();

    vTaskDelete(ಖಾಲಿ);
}

TEST_CASE("Test FreeRTOS Scheduling Round Robin", "[freertos]")
{
    core0_ಸತತ_ಕ್ರಮ_ಸರತಿ = x_ಸರತಿ_Create(SPIN_TASK_ಅಂಕೆ_ITER * ಅಂಕೆ_PINNED_SPIN_TASK_ಪ್ರತಿ_CORE, ಗಾತ್ರ(uint32_t));
    core1_ಸತತ_ಕ್ರಮ_ಸರತಿ = x_ಸರತಿ_Create(SPIN_TASK_ಅಂಕೆ_ITER * ಅಂಕೆ_PINNED_SPIN_TASK_ಪ್ರತಿ_CORE, ಗಾತ್ರ(uint32_t));

    /* Increase priority of unity task so that the spin tasks don't preempt us
    during task creation. */
    v_Task_ಆದ್ಯತೆ_ಗಣ(ಖಾಲಿ, SPIN_TASK_PRIO + 1);
    /* Create a task on core 1 of the same priority to block core 1 */
    ಅಸ್ಥಿರ bool suspend_blocker = false;
    TaskHandle_t blocker_task_hdl;
    x_Task_Create_Pinned_ಗಮ್ಯ_Core(blocker_task, "blk", TASK_ರಾಶಿ_ಗಾತ್ರ, (ಶೂನ್ಯ *)&suspend_blocker, SPIN_TASK_PRIO + 1, &blocker_task_hdl, 1);

    //Disable tick interrupts on core 0 the duration of the test
    taskDISABLE_INTERRUPTS();
    esp_cpu_intr_disable(1 << TICK_INTR_IDX);
    taskENABLE_INTERRUPTS();

    TaskHandle_t core0_task_hdls[ಅಂಕೆ_PINNED_SPIN_TASK_ಪ್ರತಿ_CORE];
    TaskHandle_t core1_task_hdls[ಅಂಕೆ_PINNED_SPIN_TASK_ಪ್ರತಿ_CORE];

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಅಂಕೆ_PINNED_SPIN_TASK_ಪ್ರತಿ_CORE; i++) {
        //Create a spin task pinned to core 0
        x_Task_Create_Pinned_ಗಮ್ಯ_Core(spin_task, "spin", TASK_ರಾಶಿ_ಗಾತ್ರ, (ಶೂನ್ಯ *)(0x00 + i), SPIN_TASK_PRIO, &core0_task_hdls[i], 0);
        //Create a spin task pinned to core 1
        x_Task_Create_Pinned_ಗಮ್ಯ_Core(spin_task, "spin", TASK_ರಾಶಿ_ಗಾತ್ರ, (ಶೂನ್ಯ *)(0x10 + i), SPIN_TASK_PRIO, &core1_task_hdls[i], 1);
    }

    /* Start the tasks in a particular order. This order should be reflected in
    in the round robin scheduling on each core */
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಅಂಕೆ_PINNED_SPIN_TASK_ಪ್ರತಿ_CORE; i++) {
        //Start a spin task on core 0
        xTaskNotifyGive(core0_task_hdls[i]);
        //Start a spin task on core 1
        xTaskNotifyGive(core1_task_hdls[i]);
    }

    //Lower priority of this task and stop blocker task to allow the spin tasks to be scheduled
    suspend_blocker = true;
    v_Task_ಆದ್ಯತೆ_ಗಣ(ಖಾಲಿ, UNITY_FREERTOS_ಆದ್ಯತೆ);

    //Give a enough delay to allow all iterations of the round robin to occur
    esp_rom_delay_us(10000);

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < SPIN_TASK_ಅಂಕೆ_ITER; i++) {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < ಅಂಕೆ_PINNED_SPIN_TASK_ಪ್ರತಿ_CORE; j++) {
            uint32_t core0_ನಮೂದು;
            uint32_t core1_ನಮೂದು;
            TEST_ASSERT_EQUAL(pdTRUE, x_ಸರತಿ_Receive(core0_ಸತತ_ಕ್ರಮ_ಸರತಿ, &core0_ನಮೂದು, 0));
            TEST_ASSERT_EQUAL(pdTRUE, x_ಸರತಿ_Receive(core1_ಸತತ_ಕ್ರಮ_ಸರತಿ, &core1_ನಮೂದು, 0));
            TEST_ASSERT_EQUAL(0x00 + j, core0_ನಮೂದು);
            TEST_ASSERT_EQUAL(0x10 + j, core1_ನಮೂದು);
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
    v_ಸರತಿ_Delete(core0_ಸತತ_ಕ್ರಮ_ಸರತಿ);
    v_ಸರತಿ_Delete(core1_ಸತತ_ಕ್ರಮ_ಸರತಿ);
}

#ಆದರೆ_ಅಂತ್ಯ  //!defined(CONFIG_FREERTOS_UNICORE) && (efined(CONFIG_FREERTOS_CORETIMER_0) || defined(CONFIG_FREERTOS_CORETIMER_1))
