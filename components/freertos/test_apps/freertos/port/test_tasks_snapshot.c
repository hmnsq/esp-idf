/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "freertos/freertos_debug.h"
#ಅನ್ತರ್ಭಾವ "esp_cpu.h"
#ಅನ್ತರ್ಭಾವ "esp_rom_sys.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "sdkconfig.h"

#ವ್ಯಾಖ್ಯಾನಿಸು TEST_MAX_TASKS_ಅಂಕೆ      32
#ವ್ಯಾಖ್ಯಾನಿಸು ಅಂಕೆ_TASKS_ಪ್ರತಿ_LIST      2
#ವ್ಯಾಖ್ಯಾನಿಸು TASK_ಆದ್ಯತೆ           (configMAX_PRIORITIES - 2)

ಸ್ಥಿರ ಅಸ್ಥಿರ bool ಆರಂಭ_ready_flag = false;
ಸ್ಥಿರ ಅಸ್ಥಿರ bool ಆರಂಭ_resp_flag = false;
ಸ್ಥಿರ ಅಸ್ಥಿರ bool stop_flag = false;

ಸ್ಥಿರ ಶೂನ್ಯ ready_task(ಶೂನ್ಯ *arg)
{
    ಆಗ (1) {
#ಆದರೆ !CONFIG_FREERTOS_UNICORE
        taskDISABLE_INTERRUPTS();
        /*
        The main task (on core 0) has a higher priority than this task. Thus, by the time this flag is set, this task
        is guaranteed to running on core 1.
        */
        ಆದರೆ (ಆರಂಭ_ready_flag) {   // Is the test starting?
            ಆರಂಭ_resp_flag = true;   // Indicate we are ready to start
            ಆಗ (!stop_flag) {
                ;   // Wait until test completes
            }

            // Reset the flags
            ಆರಂಭ_ready_flag = false;
            ಆರಂಭ_resp_flag = false;
            stop_flag = false;
        }
        taskENABLE_INTERRUPTS();
        // Short non-blocking delay to allow INT WDT to feed
        esp_rom_delay_us(1000);
#ಆದರೆ_ಅಂತ್ಯ // !CONFIG_FREERTOS_UNICORE
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ blocked_task(ಶೂನ್ಯ *arg)
{
    // Delay for portMAX_DELAY - 1 as not to go on the suspended list
    vTaskDelay(ಪೋರ್ಟ್_MAX_DELAY - 1);
}

ಸ್ಥಿರ ಶೂನ್ಯ suspended_task(ಶೂನ್ಯ *arg)
{
    vTaskSuspend(ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸಿದ್ಧತೆ(TaskHandle_t *task_list, ಪೂರ್ಣಾಂಕ *ಅಂಕೆ_tasks_ret, UBaseType_t *old_ಆದ್ಯತೆ_ret)
{
    // Raise our priority so that we aren't preempted
    *old_ಆದ್ಯತೆ_ret = ux_Task_ಆದ್ಯತೆ_Get(ಖಾಲಿ);
    v_Task_ಆದ್ಯತೆ_ಗಣ(ಖಾಲಿ, configMAX_PRIORITIES - 1);

    // Create tasks
    ಪೂರ್ಣಾಂಕ ಅಂಕೆ_tasks = 0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಅಂಕೆ_TASKS_ಪ್ರತಿ_LIST; i++) {
        //Ready task
        xTaskCreate(ready_task, "ready", 1024, ಖಾಲಿ, TASK_ಆದ್ಯತೆ, &(task_list[ಅಂಕೆ_tasks]));
        ಅಂಕೆ_tasks++;
        //Blocked task
        xTaskCreate(blocked_task, "blkd", 1024, ಖಾಲಿ, TASK_ಆದ್ಯತೆ, &(task_list[ಅಂಕೆ_tasks]));
        ಅಂಕೆ_tasks++;
        //Suspended task
        xTaskCreate(suspended_task, "susp", 1024, ಖಾಲಿ, TASK_ಆದ್ಯತೆ, &(task_list[ಅಂಕೆ_tasks]));
        ಅಂಕೆ_tasks++;
    }
    *ಅಂಕೆ_tasks_ret = ಅಂಕೆ_tasks;
    // Short delay to allow tasks to spin up
    vTaskDelay(10);

    /*
    Task snapshot functions are only meant to be called when FreeRTOS is no longer running (e.g., during a panic),
    thus assumes that nothing else (interrupt or the other core) will access the task lists simultaneously. However, for
    unit tests, FreeRTOS is still running. So we must guarantee thread safe access another way.

    - For single core, simply disabling interrupts will guarantee that the current task is the only thread to access the
      task lists
    - For SMP, we also need to stop the other core from accessing the task lists. Thus, we use a 2-way handshake to put
      the other core into a while loop with interrupts disabled.
    */
    // Disable interrupts so that the current core isn't preempted.
    taskDISABLE_INTERRUPTS();
#ಆದರೆ !CONFIG_FREERTOS_UNICORE
    // For SMP, we need to do a 2-way handshake to ensure the other core spins with interrupts disabled.
    ಆರಂಭ_ready_flag = true;
    ಆಗ (!ಆರಂಭ_resp_flag) {
        ;
    }
#ಆದರೆ_ಅಂತ್ಯ // !CONFIG_FREERTOS_UNICORE
}

ಸ್ಥಿರ ಶೂನ್ಯ check_snapshots(TaskHandle_t *task_list, ಪೂರ್ಣಾಂಕ ಅಂಕೆ_tasks, TaskSnapshot_t *task_snapshots, UBaseType_t ಅಂಕೆ_snapshots)
{
    // Check task snapshots. Every created task should be found in the task snapshot
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಅಂಕೆ_tasks; i++) {
        bool ಸಿಕ್ಕಿತು = false;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < ಅಂಕೆ_snapshots; j++) {
            ಆದರೆ (task_list[i] == (TaskHandle_t)task_snapshots[j].pxTCB) {
                ಸಿಕ್ಕಿತು = true;
                ನಿಲ್ಲಿಸು;
            }
        }
        TEST_ASSERT(ಸಿಕ್ಕಿತು);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ teardown(TaskHandle_t *task_list, ಪೂರ್ಣಾಂಕ ಅಂಕೆ_tasks, UBaseType_t old_ಆದ್ಯತೆ)
{
#ಆದರೆ !CONFIG_FREERTOS_UNICORE
    // Signal to the other core that it can exit the loop
    stop_flag = true;
#ಆದರೆ_ಅಂತ್ಯ // !CONFIG_FREERTOS_UNICORE
    // Re-enable interrupts on the current core.
    taskENABLE_INTERRUPTS();

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಅಂಕೆ_tasks; i++) {
        vTaskDelete(task_list[i]);
    }
    // Restore priority
    v_Task_ಆದ್ಯತೆ_ಗಣ(ಖಾಲಿ, old_ಆದ್ಯತೆ);
    // Short delay to allow tasks to clean up
    vTaskDelay(10);
}

TEST_CASE("Task snapshot: Get all", "[freertos]")
{
    // Short delay to allow both cores to spin up
    vTaskDelay(10);

    TaskHandle_t task_list[TEST_MAX_TASKS_ಅಂಕೆ];
    ಪೂರ್ಣಾಂಕ ಅಂಕೆ_tasks;
    UBaseType_t old_ಆದ್ಯತೆ;
    ಸಿದ್ಧತೆ(task_list, &ಅಂಕೆ_tasks, &old_ಆದ್ಯತೆ);

    // Get task snapshots using uxTaskGetSnapshotAll()
    TaskSnapshot_t task_snapshots[TEST_MAX_TASKS_ಅಂಕೆ];
    UBaseType_t tcb_ಗಾತ್ರ;
    UBaseType_t ಅಂಕೆ_snapshots;
    ಅಂಕೆ_snapshots = uxTaskGetSnapshotAll(task_snapshots, TEST_MAX_TASKS_ಅಂಕೆ, &tcb_ಗಾತ್ರ);
    TEST_ASSERT_LESS_OR_EQUAL(TEST_MAX_TASKS_ಅಂಕೆ, ಅಂಕೆ_snapshots);

    check_snapshots(task_list, ಅಂಕೆ_tasks, task_snapshots, ಅಂಕೆ_snapshots);
    teardown(task_list, ಅಂಕೆ_tasks, old_ಆದ್ಯತೆ);
}

TEST_CASE("Task snapshot: Iterate", "[freertos]")
{
    // Short delay to allow both cores to spin up
    vTaskDelay(10);

    TaskHandle_t task_list[TEST_MAX_TASKS_ಅಂಕೆ];
    ಪೂರ್ಣಾಂಕ ಅಂಕೆ_tasks;
    UBaseType_t old_ಆದ್ಯತೆ;
    ಸಿದ್ಧತೆ(task_list, &ಅಂಕೆ_tasks, &old_ಆದ್ಯತೆ);

    // Get task snapshots using xTaskGetNext() and vTaskGetSnapshot()
    TaskSnapshot_t task_snapshots[TEST_MAX_TASKS_ಅಂಕೆ];
    UBaseType_t ಅಂಕೆ_snapshots = 0;
    TaskIterator_t task_iterator = {0};
    ಆಗ (x_Task_Get_ಮುಂದಿನ(&task_iterator) != -1) {
        ಆದರೆ (task_iterator.pxTaskHandle != ಖಾಲಿ) {
            // Get the task's snapshot
            ತಳ_Type_t ಫಲಿತಾಂಶ_2 = vTaskGetSnapshot(task_iterator.pxTaskHandle, &task_snapshots[ಅಂಕೆ_snapshots]);
            TEST_ASSERT_EQUAL(pdTRUE, ಫಲಿತಾಂಶ_2);
            ಅಂಕೆ_snapshots++;
        }
    }
    TEST_ASSERT_LESS_OR_EQUAL(TEST_MAX_TASKS_ಅಂಕೆ, ಅಂಕೆ_snapshots);

    check_snapshots(task_list, ಅಂಕೆ_tasks, task_snapshots, ಅಂಕೆ_snapshots);
    teardown(task_list, ಅಂಕೆ_tasks, old_ಆದ್ಯತೆ);
}
