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

#define TEST_MAX_TASKS_ಅಂಕೆ      32
#define ಅಂಕೆ_TASKS_PER_LIST      2
#define TASK_PRIORITY           (configMAX_PRIORITIES - 2)

ಸ್ಥಿರ ಚಞ್ಚಲ bool start_ready_flag = false;
ಸ್ಥಿರ ಚಞ್ಚಲ bool start_resp_flag = false;
ಸ್ಥಿರ ಚಞ್ಚಲ bool stop_flag = false;

ಸ್ಥಿರ ಶೂನ್ಯ ready_task(ಶೂನ್ಯ *arg)
{
    ಯಾವತ್ (1) {
#ಯದಿ !CONFIG_FREERTOS_UNICORE
        taskDISABLE_INTERRUPTS();
        /*
        The main task (on core 0) has a higher priority than this task. Thus, by the time this flag is set, this task
        is guaranteed to running on core 1.
        */
        ಯದಿ (start_ready_flag) {   // Is the test starting?
            start_resp_flag = true;   // Indicate we are ready to start
            ಯಾವತ್ (!stop_flag) {
                ;   // Wait until test completes
            }

            // Reset the flags
            start_ready_flag = false;
            start_resp_flag = false;
            stop_flag = false;
        }
        taskENABLE_INTERRUPTS();
        // Short non-blocking delay to allow INT WDT to feed
        esp_rom_delay_us(1000);
#endif // !CONFIG_FREERTOS_UNICORE
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ blocked_task(ಶೂನ್ಯ *arg)
{
    // Delay for portMAX_DELAY - 1 as not to go on the suspended list
    vTaskDelay(portMAX_DELAY - 1);
}

ಸ್ಥಿರ ಶೂನ್ಯ suspended_task(ಶೂನ್ಯ *arg)
{
    vTaskSuspend(ರಿಕ್ತ);
}

ಸ್ಥಿರ ಶೂನ್ಯ setup(TaskHandle_t *task_list, ಪೂರ್ಣಾಙ್ಕ *ಅಂಕೆ_tasks_ret, UBaseType_t *old_priority_ret)
{
    // Raise our priority so that we aren't preempted
    *old_priority_ret = uxTaskPriorityGet(ರಿಕ್ತ);
    vTaskPrioritySet(ರಿಕ್ತ, configMAX_PRIORITIES - 1);

    // Create tasks
    ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_tasks = 0;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಅಂಕೆ_TASKS_PER_LIST; i++) {
        //Ready task
        xTaskCreate(ready_task, "ready", 1024, ರಿಕ್ತ, TASK_PRIORITY, &(task_list[ಅಂಕೆ_tasks]));
        ಅಂಕೆ_tasks++;
        //Blocked task
        xTaskCreate(blocked_task, "blkd", 1024, ರಿಕ್ತ, TASK_PRIORITY, &(task_list[ಅಂಕೆ_tasks]));
        ಅಂಕೆ_tasks++;
        //Suspended task
        xTaskCreate(suspended_task, "susp", 1024, ರಿಕ್ತ, TASK_PRIORITY, &(task_list[ಅಂಕೆ_tasks]));
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
#ಯದಿ !CONFIG_FREERTOS_UNICORE
    // For SMP, we need to do a 2-way handshake to ensure the other core spins with interrupts disabled.
    start_ready_flag = true;
    ಯಾವತ್ (!start_resp_flag) {
        ;
    }
#endif // !CONFIG_FREERTOS_UNICORE
}

ಸ್ಥಿರ ಶೂನ್ಯ check_snapshots(TaskHandle_t *task_list, ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_tasks, TaskSnapshot_t *task_snapshots, UBaseType_t ಅಂಕೆ_snapshots)
{
    // Check task snapshots. Every created task should be found in the task snapshot
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಅಂಕೆ_tasks; i++) {
        bool found = false;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < ಅಂಕೆ_snapshots; j++) {
            ಯದಿ (task_list[i] == (TaskHandle_t)task_snapshots[j].pxTCB) {
                found = true;
                ಭಙ್ಗ;
            }
        }
        TEST_ASSERT(found);
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ teardown(TaskHandle_t *task_list, ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_tasks, UBaseType_t old_priority)
{
#ಯದಿ !CONFIG_FREERTOS_UNICORE
    // Signal to the other core that it can exit the loop
    stop_flag = true;
#endif // !CONFIG_FREERTOS_UNICORE
    // Re-enable interrupts on the current core.
    taskENABLE_INTERRUPTS();

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಅಂಕೆ_tasks; i++) {
        vTaskDelete(task_list[i]);
    }
    // Restore priority
    vTaskPrioritySet(ರಿಕ್ತ, old_priority);
    // Short delay to allow tasks to clean up
    vTaskDelay(10);
}

TEST_CASE("Task snapshot: Get all", "[freertos]")
{
    // Short delay to allow both cores to spin up
    vTaskDelay(10);

    TaskHandle_t task_list[TEST_MAX_TASKS_ಅಂಕೆ];
    ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_tasks;
    UBaseType_t old_priority;
    setup(task_list, &ಅಂಕೆ_tasks, &old_priority);

    // Get task snapshots using uxTaskGetSnapshotAll()
    TaskSnapshot_t task_snapshots[TEST_MAX_TASKS_ಅಂಕೆ];
    UBaseType_t tcb_size;
    UBaseType_t ಅಂಕೆ_snapshots;
    ಅಂಕೆ_snapshots = uxTaskGetSnapshotAll(task_snapshots, TEST_MAX_TASKS_ಅಂಕೆ, &tcb_size);
    TEST_ASSERT_LESS_OR_EQUAL(TEST_MAX_TASKS_ಅಂಕೆ, ಅಂಕೆ_snapshots);

    check_snapshots(task_list, ಅಂಕೆ_tasks, task_snapshots, ಅಂಕೆ_snapshots);
    teardown(task_list, ಅಂಕೆ_tasks, old_priority);
}

TEST_CASE("Task snapshot: Iterate", "[freertos]")
{
    // Short delay to allow both cores to spin up
    vTaskDelay(10);

    TaskHandle_t task_list[TEST_MAX_TASKS_ಅಂಕೆ];
    ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_tasks;
    UBaseType_t old_priority;
    setup(task_list, &ಅಂಕೆ_tasks, &old_priority);

    // Get task snapshots using xTaskGetNext() and vTaskGetSnapshot()
    TaskSnapshot_t task_snapshots[TEST_MAX_TASKS_ಅಂಕೆ];
    UBaseType_t ಅಂಕೆ_snapshots = 0;
    TaskIterator_t task_iterator = {0};
    ಯಾವತ್ (xTaskGetNext(&task_iterator) != -1) {
        ಯದಿ (task_iterator.pxTaskHandle != ರಿಕ್ತ) {
            // Get the task's snapshot
            BaseType_t Result = vTaskGetSnapshot(task_iterator.pxTaskHandle, &task_snapshots[ಅಂಕೆ_snapshots]);
            TEST_ASSERT_EQUAL(pdTRUE, Result);
            ಅಂಕೆ_snapshots++;
        }
    }
    TEST_ASSERT_LESS_OR_EQUAL(TEST_MAX_TASKS_ಅಂಕೆ, ಅಂಕೆ_snapshots);

    check_snapshots(task_list, ಅಂಕೆ_tasks, task_snapshots, ಅಂಕೆ_snapshots);
    teardown(task_list, ಅಂಕೆ_tasks, old_priority);
}
