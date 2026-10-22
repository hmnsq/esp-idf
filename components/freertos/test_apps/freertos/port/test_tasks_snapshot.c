/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "sdkconfig.h"
#ضمن <stdio.h>
#ضمن <stdbool.h>
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "freertos/freertos_debug.h"
#ضمن "esp_cpu.h"
#ضمن "esp_rom_sys.h"
#ضمن "unity.h"
#ضمن "sdkconfig.h"

#define TEST_MAX_TASKS_اڠک      32
#define اڠک_TASKS_PER_LIST      2
#define TASK_PRIORITY           (configMAX_PRIORITIES - 2)

ساكن متطاير bool start_ready_flag = false;
ساكن متطاير bool start_resp_flag = false;
ساكن متطاير bool stop_flag = false;

ساكن فراغ ready_task(فراغ *arg)
{
    طالما (1) {
#إذا !CONFIG_FREERTOS_UNICORE
        taskDISABLE_INTERRUPTS();
        /*
        The main task (on core 0) has a higher priority than this task. Thus, by the time this flag is set, this task
        is guaranteed to running on core 1.
        */
        إذا (start_ready_flag) {   // Is the test starting?
            start_resp_flag = true;   // Indicate we are ready to start
            طالما (!stop_flag) {
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

ساكن فراغ blocked_task(فراغ *arg)
{
    // Delay for portMAX_DELAY - 1 as not to go on the suspended list
    vTaskDelay(portMAX_DELAY - 1);
}

ساكن فراغ suspended_task(فراغ *arg)
{
    vTaskSuspend(عدم);
}

ساكن فراغ setup(TaskHandle_t *task_list, صحيح *اڠک_tasks_ret, UBaseType_t *old_priority_ret)
{
    // Raise our priority so that we aren't preempted
    *old_priority_ret = uxTaskPriorityGet(عدم);
    vTaskPrioritySet(عدم, configMAX_PRIORITIES - 1);

    // Create tasks
    صحيح اڠک_tasks = 0;
    لكل (صحيح i = 0; i < اڠک_TASKS_PER_LIST; i++) {
        //Ready task
        xTaskCreate(ready_task, "ready", 1024, عدم, TASK_PRIORITY, &(task_list[اڠک_tasks]));
        اڠک_tasks++;
        //Blocked task
        xTaskCreate(blocked_task, "blkd", 1024, عدم, TASK_PRIORITY, &(task_list[اڠک_tasks]));
        اڠک_tasks++;
        //Suspended task
        xTaskCreate(suspended_task, "susp", 1024, عدم, TASK_PRIORITY, &(task_list[اڠک_tasks]));
        اڠک_tasks++;
    }
    *اڠک_tasks_ret = اڠک_tasks;
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
#إذا !CONFIG_FREERTOS_UNICORE
    // For SMP, we need to do a 2-way handshake to ensure the other core spins with interrupts disabled.
    start_ready_flag = true;
    طالما (!start_resp_flag) {
        ;
    }
#endif // !CONFIG_FREERTOS_UNICORE
}

ساكن فراغ check_snapshots(TaskHandle_t *task_list, صحيح اڠک_tasks, TaskSnapshot_t *task_snapshots, UBaseType_t اڠک_snapshots)
{
    // Check task snapshots. Every created task should be found in the task snapshot
    لكل (صحيح i = 0; i < اڠک_tasks; i++) {
        bool found = false;
        لكل (صحيح j = 0; j < اڠک_snapshots; j++) {
            إذا (task_list[i] == (TaskHandle_t)task_snapshots[j].pxTCB) {
                found = true;
                اكسر;
            }
        }
        TEST_ASSERT(found);
    }
}

ساكن فراغ teardown(TaskHandle_t *task_list, صحيح اڠک_tasks, UBaseType_t old_priority)
{
#إذا !CONFIG_FREERTOS_UNICORE
    // Signal to the other core that it can exit the loop
    stop_flag = true;
#endif // !CONFIG_FREERTOS_UNICORE
    // Re-enable interrupts on the current core.
    taskENABLE_INTERRUPTS();

    لكل (صحيح i = 0; i < اڠک_tasks; i++) {
        vTaskDelete(task_list[i]);
    }
    // Restore priority
    vTaskPrioritySet(عدم, old_priority);
    // Short delay to allow tasks to clean up
    vTaskDelay(10);
}

TEST_CASE("Task snapshot: Get all", "[freertos]")
{
    // Short delay to allow both cores to spin up
    vTaskDelay(10);

    TaskHandle_t task_list[TEST_MAX_TASKS_اڠک];
    صحيح اڠک_tasks;
    UBaseType_t old_priority;
    setup(task_list, &اڠک_tasks, &old_priority);

    // Get task snapshots using uxTaskGetSnapshotAll()
    TaskSnapshot_t task_snapshots[TEST_MAX_TASKS_اڠک];
    UBaseType_t tcb_size;
    UBaseType_t اڠک_snapshots;
    اڠک_snapshots = uxTaskGetSnapshotAll(task_snapshots, TEST_MAX_TASKS_اڠک, &tcb_size);
    TEST_ASSERT_LESS_OR_EQUAL(TEST_MAX_TASKS_اڠک, اڠک_snapshots);

    check_snapshots(task_list, اڠک_tasks, task_snapshots, اڠک_snapshots);
    teardown(task_list, اڠک_tasks, old_priority);
}

TEST_CASE("Task snapshot: Iterate", "[freertos]")
{
    // Short delay to allow both cores to spin up
    vTaskDelay(10);

    TaskHandle_t task_list[TEST_MAX_TASKS_اڠک];
    صحيح اڠک_tasks;
    UBaseType_t old_priority;
    setup(task_list, &اڠک_tasks, &old_priority);

    // Get task snapshots using xTaskGetNext() and vTaskGetSnapshot()
    TaskSnapshot_t task_snapshots[TEST_MAX_TASKS_اڠک];
    UBaseType_t اڠک_snapshots = 0;
    TaskIterator_t task_iterator = {0};
    طالما (xTaskGetNext(&task_iterator) != -1) {
        إذا (task_iterator.pxTaskHandle != عدم) {
            // Get the task's snapshot
            BaseType_t Result = vTaskGetSnapshot(task_iterator.pxTaskHandle, &task_snapshots[اڠک_snapshots]);
            TEST_ASSERT_EQUAL(pdTRUE, Result);
            اڠک_snapshots++;
        }
    }
    TEST_ASSERT_LESS_OR_EQUAL(TEST_MAX_TASKS_اڠک, اڠک_snapshots);

    check_snapshots(task_list, اڠک_tasks, task_snapshots, اڠک_snapshots);
    teardown(task_list, اڠک_tasks, old_priority);
}
