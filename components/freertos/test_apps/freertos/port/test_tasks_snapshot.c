/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#અન્તર્ભાવ "sdkconfig.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdbool.h>
#અન્તર્ભાવ "freertos/FreeRTOS.h"
#અન્તર્ભાવ "freertos/task.h"
#અન્તર્ભાવ "freertos/freertos_debug.h"
#અન્તર્ભાવ "esp_cpu.h"
#અન્તર્ભાવ "esp_rom_sys.h"
#અન્તર્ભાવ "unity.h"
#અન્તર્ભાવ "sdkconfig.h"

#define TEST_MAX_TASKS_અંક      32
#define અંક_TASKS_PER_LIST      2
#define TASK_PRIORITY           (configMAX_PRIORITIES - 2)

સ્થિર અસ્થિર bool start_ready_flag = false;
સ્થિર અસ્થિર bool start_resp_flag = false;
સ્થિર અસ્થિર bool stop_flag = false;

સ્થિર શૂન્ય ready_task(શૂન્ય *arg)
{
    જ્યારે (1) {
#જો !CONFIG_FREERTOS_UNICORE
        taskDISABLE_INTERRUPTS();
        /*
        The main task (on core 0) has a higher priority than this task. Thus, by the time this flag is set, this task
        is guaranteed to running on core 1.
        */
        જો (start_ready_flag) {   // Is the test starting?
            start_resp_flag = true;   // Indicate we are ready to start
            જ્યારે (!stop_flag) {
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

સ્થિર શૂન્ય blocked_task(શૂન્ય *arg)
{
    // Delay for portMAX_DELAY - 1 as not to go on the suspended list
    vTaskDelay(portMAX_DELAY - 1);
}

સ્થિર શૂન્ય suspended_task(શૂન્ય *arg)
{
    vTaskSuspend(ખાલી);
}

સ્થિર શૂન્ય setup(TaskHandle_t *task_list, પૂર્ણાંક *અંક_tasks_ret, UBaseType_t *old_priority_ret)
{
    // Raise our priority so that we aren't preempted
    *old_priority_ret = uxTaskPriorityGet(ખાલી);
    vTaskPrioritySet(ખાલી, configMAX_PRIORITIES - 1);

    // Create tasks
    પૂર્ણાંક અંક_tasks = 0;
    માટે (પૂર્ણાંક i = 0; i < અંક_TASKS_PER_LIST; i++) {
        //Ready task
        xTaskCreate(ready_task, "ready", 1024, ખાલી, TASK_PRIORITY, &(task_list[અંક_tasks]));
        અંક_tasks++;
        //Blocked task
        xTaskCreate(blocked_task, "blkd", 1024, ખાલી, TASK_PRIORITY, &(task_list[અંક_tasks]));
        અંક_tasks++;
        //Suspended task
        xTaskCreate(suspended_task, "susp", 1024, ખાલી, TASK_PRIORITY, &(task_list[અંક_tasks]));
        અંક_tasks++;
    }
    *અંક_tasks_ret = અંક_tasks;
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
#જો !CONFIG_FREERTOS_UNICORE
    // For SMP, we need to do a 2-way handshake to ensure the other core spins with interrupts disabled.
    start_ready_flag = true;
    જ્યારે (!start_resp_flag) {
        ;
    }
#endif // !CONFIG_FREERTOS_UNICORE
}

સ્થિર શૂન્ય check_snapshots(TaskHandle_t *task_list, પૂર્ણાંક અંક_tasks, TaskSnapshot_t *task_snapshots, UBaseType_t અંક_snapshots)
{
    // Check task snapshots. Every created task should be found in the task snapshot
    માટે (પૂર્ણાંક i = 0; i < અંક_tasks; i++) {
        bool found = false;
        માટે (પૂર્ણાંક j = 0; j < અંક_snapshots; j++) {
            જો (task_list[i] == (TaskHandle_t)task_snapshots[j].pxTCB) {
                found = true;
                તોડો;
            }
        }
        TEST_ASSERT(found);
    }
}

સ્થિર શૂન્ય teardown(TaskHandle_t *task_list, પૂર્ણાંક અંક_tasks, UBaseType_t old_priority)
{
#જો !CONFIG_FREERTOS_UNICORE
    // Signal to the other core that it can exit the loop
    stop_flag = true;
#endif // !CONFIG_FREERTOS_UNICORE
    // Re-enable interrupts on the current core.
    taskENABLE_INTERRUPTS();

    માટે (પૂર્ણાંક i = 0; i < અંક_tasks; i++) {
        vTaskDelete(task_list[i]);
    }
    // Restore priority
    vTaskPrioritySet(ખાલી, old_priority);
    // Short delay to allow tasks to clean up
    vTaskDelay(10);
}

TEST_CASE("Task snapshot: Get all", "[freertos]")
{
    // Short delay to allow both cores to spin up
    vTaskDelay(10);

    TaskHandle_t task_list[TEST_MAX_TASKS_અંક];
    પૂર્ણાંક અંક_tasks;
    UBaseType_t old_priority;
    setup(task_list, &અંક_tasks, &old_priority);

    // Get task snapshots using uxTaskGetSnapshotAll()
    TaskSnapshot_t task_snapshots[TEST_MAX_TASKS_અંક];
    UBaseType_t tcb_size;
    UBaseType_t અંક_snapshots;
    અંક_snapshots = uxTaskGetSnapshotAll(task_snapshots, TEST_MAX_TASKS_અંક, &tcb_size);
    TEST_ASSERT_LESS_OR_EQUAL(TEST_MAX_TASKS_અંક, અંક_snapshots);

    check_snapshots(task_list, અંક_tasks, task_snapshots, અંક_snapshots);
    teardown(task_list, અંક_tasks, old_priority);
}

TEST_CASE("Task snapshot: Iterate", "[freertos]")
{
    // Short delay to allow both cores to spin up
    vTaskDelay(10);

    TaskHandle_t task_list[TEST_MAX_TASKS_અંક];
    પૂર્ણાંક અંક_tasks;
    UBaseType_t old_priority;
    setup(task_list, &અંક_tasks, &old_priority);

    // Get task snapshots using xTaskGetNext() and vTaskGetSnapshot()
    TaskSnapshot_t task_snapshots[TEST_MAX_TASKS_અંક];
    UBaseType_t અંક_snapshots = 0;
    TaskIterator_t task_iterator = {0};
    જ્યારે (xTaskGetNext(&task_iterator) != -1) {
        જો (task_iterator.pxTaskHandle != ખાલી) {
            // Get the task's snapshot
            BaseType_t Result = vTaskGetSnapshot(task_iterator.pxTaskHandle, &task_snapshots[અંક_snapshots]);
            TEST_ASSERT_EQUAL(pdTRUE, Result);
            અંક_snapshots++;
        }
    }
    TEST_ASSERT_LESS_OR_EQUAL(TEST_MAX_TASKS_અંક, અંક_snapshots);

    check_snapshots(task_list, અંક_tasks, task_snapshots, અંક_snapshots);
    teardown(task_list, અંક_tasks, old_priority);
}
