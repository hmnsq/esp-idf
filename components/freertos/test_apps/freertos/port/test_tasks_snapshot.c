/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#অন্তর্ভাব "sdkconfig.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdbool.h>
#অন্তর্ভাব "freertos/FreeRTOS.h"
#অন্তর্ভাব "freertos/task.h"
#অন্তর্ভাব "freertos/freertos_debug.h"
#অন্তর্ভাব "esp_cpu.h"
#অন্তর্ভাব "esp_rom_sys.h"
#অন্তর্ভাব "unity.h"
#অন্তর্ভাব "sdkconfig.h"

#define TEST_MAX_TASKS_অঙ্ক      32
#define অঙ্ক_TASKS_PER_LIST      2
#define TASK_PRIORITY           (configMAX_PRIORITIES - 2)

স্থির চঞ্চল bool start_ready_flag = false;
স্থির চঞ্চল bool start_resp_flag = false;
স্থির চঞ্চল bool stop_flag = false;

স্থির শূন্য ready_task(শূন্য *arg)
{
    যাবত্ (1) {
#যদি !CONFIG_FREERTOS_UNICORE
        taskDISABLE_INTERRUPTS();
        /*
        The main task (on core 0) has a higher priority than this task. Thus, by the time this flag is set, this task
        is guaranteed to running on core 1.
        */
        যদি (start_ready_flag) {   // Is the test starting?
            start_resp_flag = true;   // Indicate we are ready to start
            যাবত্ (!stop_flag) {
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

স্থির শূন্য blocked_task(শূন্য *arg)
{
    // Delay for portMAX_DELAY - 1 as not to go on the suspended list
    vTaskDelay(portMAX_DELAY - 1);
}

স্থির শূন্য suspended_task(শূন্য *arg)
{
    vTaskSuspend(রিক্ত);
}

স্থির শূন্য setup(TaskHandle_t *task_list, পূর্ণাঙ্ক *অঙ্ক_tasks_ret, UBaseType_t *old_priority_ret)
{
    // Raise our priority so that we aren't preempted
    *old_priority_ret = uxTaskPriorityGet(রিক্ত);
    vTaskPrioritySet(রিক্ত, configMAX_PRIORITIES - 1);

    // Create tasks
    পূর্ণাঙ্ক অঙ্ক_tasks = 0;
    প্রতি (পূর্ণাঙ্ক i = 0; i < অঙ্ক_TASKS_PER_LIST; i++) {
        //Ready task
        xTaskCreate(ready_task, "ready", 1024, রিক্ত, TASK_PRIORITY, &(task_list[অঙ্ক_tasks]));
        অঙ্ক_tasks++;
        //Blocked task
        xTaskCreate(blocked_task, "blkd", 1024, রিক্ত, TASK_PRIORITY, &(task_list[অঙ্ক_tasks]));
        অঙ্ক_tasks++;
        //Suspended task
        xTaskCreate(suspended_task, "susp", 1024, রিক্ত, TASK_PRIORITY, &(task_list[অঙ্ক_tasks]));
        অঙ্ক_tasks++;
    }
    *অঙ্ক_tasks_ret = অঙ্ক_tasks;
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
#যদি !CONFIG_FREERTOS_UNICORE
    // For SMP, we need to do a 2-way handshake to ensure the other core spins with interrupts disabled.
    start_ready_flag = true;
    যাবত্ (!start_resp_flag) {
        ;
    }
#endif // !CONFIG_FREERTOS_UNICORE
}

স্থির শূন্য check_snapshots(TaskHandle_t *task_list, পূর্ণাঙ্ক অঙ্ক_tasks, TaskSnapshot_t *task_snapshots, UBaseType_t অঙ্ক_snapshots)
{
    // Check task snapshots. Every created task should be found in the task snapshot
    প্রতি (পূর্ণাঙ্ক i = 0; i < অঙ্ক_tasks; i++) {
        bool found = false;
        প্রতি (পূর্ণাঙ্ক j = 0; j < অঙ্ক_snapshots; j++) {
            যদি (task_list[i] == (TaskHandle_t)task_snapshots[j].pxTCB) {
                found = true;
                ভঙ্গ;
            }
        }
        TEST_ASSERT(found);
    }
}

স্থির শূন্য teardown(TaskHandle_t *task_list, পূর্ণাঙ্ক অঙ্ক_tasks, UBaseType_t old_priority)
{
#যদি !CONFIG_FREERTOS_UNICORE
    // Signal to the other core that it can exit the loop
    stop_flag = true;
#endif // !CONFIG_FREERTOS_UNICORE
    // Re-enable interrupts on the current core.
    taskENABLE_INTERRUPTS();

    প্রতি (পূর্ণাঙ্ক i = 0; i < অঙ্ক_tasks; i++) {
        vTaskDelete(task_list[i]);
    }
    // Restore priority
    vTaskPrioritySet(রিক্ত, old_priority);
    // Short delay to allow tasks to clean up
    vTaskDelay(10);
}

TEST_CASE("Task snapshot: Get all", "[freertos]")
{
    // Short delay to allow both cores to spin up
    vTaskDelay(10);

    TaskHandle_t task_list[TEST_MAX_TASKS_অঙ্ক];
    পূর্ণাঙ্ক অঙ্ক_tasks;
    UBaseType_t old_priority;
    setup(task_list, &অঙ্ক_tasks, &old_priority);

    // Get task snapshots using uxTaskGetSnapshotAll()
    TaskSnapshot_t task_snapshots[TEST_MAX_TASKS_অঙ্ক];
    UBaseType_t tcb_size;
    UBaseType_t অঙ্ক_snapshots;
    অঙ্ক_snapshots = uxTaskGetSnapshotAll(task_snapshots, TEST_MAX_TASKS_অঙ্ক, &tcb_size);
    TEST_ASSERT_LESS_OR_EQUAL(TEST_MAX_TASKS_অঙ্ক, অঙ্ক_snapshots);

    check_snapshots(task_list, অঙ্ক_tasks, task_snapshots, অঙ্ক_snapshots);
    teardown(task_list, অঙ্ক_tasks, old_priority);
}

TEST_CASE("Task snapshot: Iterate", "[freertos]")
{
    // Short delay to allow both cores to spin up
    vTaskDelay(10);

    TaskHandle_t task_list[TEST_MAX_TASKS_অঙ্ক];
    পূর্ণাঙ্ক অঙ্ক_tasks;
    UBaseType_t old_priority;
    setup(task_list, &অঙ্ক_tasks, &old_priority);

    // Get task snapshots using xTaskGetNext() and vTaskGetSnapshot()
    TaskSnapshot_t task_snapshots[TEST_MAX_TASKS_অঙ্ক];
    UBaseType_t অঙ্ক_snapshots = 0;
    TaskIterator_t task_iterator = {0};
    যাবত্ (xTaskGetNext(&task_iterator) != -1) {
        যদি (task_iterator.pxTaskHandle != রিক্ত) {
            // Get the task's snapshot
            BaseType_t Result = vTaskGetSnapshot(task_iterator.pxTaskHandle, &task_snapshots[অঙ্ক_snapshots]);
            TEST_ASSERT_EQUAL(pdTRUE, Result);
            অঙ্ক_snapshots++;
        }
    }
    TEST_ASSERT_LESS_OR_EQUAL(TEST_MAX_TASKS_অঙ্ক, অঙ্ক_snapshots);

    check_snapshots(task_list, অঙ্ক_tasks, task_snapshots, অঙ্ক_snapshots);
    teardown(task_list, অঙ্ক_tasks, old_priority);
}
