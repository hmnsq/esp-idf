/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#समावेश "sdkconfig.h"
#समावेश <stdio.h>
#समावेश <stdbool.h>
#समावेश "freertos/FreeRTOS.h"
#समावेश "freertos/task.h"
#समावेश "freertos/freertos_debug.h"
#समावेश "esp_cpu.h"
#समावेश "esp_rom_sys.h"
#समावेश "unity.h"
#समावेश "sdkconfig.h"

#घोषणा TEST_MAX_TASKS_अंक      32
#घोषणा अंक_TASKS_PER_LIST      2
#घोषणा TASK_PRIORITY           (configMAX_PRIORITIES - 2)

अटल अस्थिर bool start_ready_flag = false;
अटल अस्थिर bool start_resp_flag = false;
अटल अस्थिर bool stop_flag = false;

अटल व्योम ready_task(व्योम *arg)
{
    जबतक (1) {
#अगर !CONFIG_FREERTOS_UNICORE
        taskDISABLE_INTERRUPTS();
        /*
        The main task (on core 0) has a higher priority than this task. Thus, by the time this flag is set, this task
        is guaranteed to running on core 1.
        */
        अगर (start_ready_flag) {   // Is the test starting?
            start_resp_flag = true;   // Indicate we are ready to start
            जबतक (!stop_flag) {
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
#पूर्ण_अगर // !CONFIG_FREERTOS_UNICORE
    }
}

अटल व्योम blocked_task(व्योम *arg)
{
    // Delay for portMAX_DELAY - 1 as not to go on the suspended list
    vTaskDelay(portMAX_DELAY - 1);
}

अटल व्योम suspended_task(व्योम *arg)
{
    vTaskSuspend(शून्य);
}

अटल व्योम setup(TaskHandle_t *task_list, पूर्णांक *अंक_tasks_ret, UBaseType_t *old_priority_ret)
{
    // Raise our priority so that we aren't preempted
    *old_priority_ret = uxTaskPriorityGet(शून्य);
    vTaskPrioritySet(शून्य, configMAX_PRIORITIES - 1);

    // Create tasks
    पूर्णांक अंक_tasks = 0;
    क्रम (पूर्णांक i = 0; i < अंक_TASKS_PER_LIST; i++) {
        //Ready task
        xTaskCreate(ready_task, "ready", 1024, शून्य, TASK_PRIORITY, &(task_list[अंक_tasks]));
        अंक_tasks++;
        //Blocked task
        xTaskCreate(blocked_task, "blkd", 1024, शून्य, TASK_PRIORITY, &(task_list[अंक_tasks]));
        अंक_tasks++;
        //Suspended task
        xTaskCreate(suspended_task, "susp", 1024, शून्य, TASK_PRIORITY, &(task_list[अंक_tasks]));
        अंक_tasks++;
    }
    *अंक_tasks_ret = अंक_tasks;
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
#अगर !CONFIG_FREERTOS_UNICORE
    // For SMP, we need to do a 2-way handshake to ensure the other core spins with interrupts disabled.
    start_ready_flag = true;
    जबतक (!start_resp_flag) {
        ;
    }
#पूर्ण_अगर // !CONFIG_FREERTOS_UNICORE
}

अटल व्योम check_snapshots(TaskHandle_t *task_list, पूर्णांक अंक_tasks, TaskSnapshot_t *task_snapshots, UBaseType_t अंक_snapshots)
{
    // Check task snapshots. Every created task should be found in the task snapshot
    क्रम (पूर्णांक i = 0; i < अंक_tasks; i++) {
        bool found = false;
        क्रम (पूर्णांक j = 0; j < अंक_snapshots; j++) {
            अगर (task_list[i] == (TaskHandle_t)task_snapshots[j].pxTCB) {
                found = true;
                अवरोध;
            }
        }
        TEST_ASSERT(found);
    }
}

अटल व्योम teardown(TaskHandle_t *task_list, पूर्णांक अंक_tasks, UBaseType_t old_priority)
{
#अगर !CONFIG_FREERTOS_UNICORE
    // Signal to the other core that it can exit the loop
    stop_flag = true;
#पूर्ण_अगर // !CONFIG_FREERTOS_UNICORE
    // Re-enable interrupts on the current core.
    taskENABLE_INTERRUPTS();

    क्रम (पूर्णांक i = 0; i < अंक_tasks; i++) {
        vTaskDelete(task_list[i]);
    }
    // Restore priority
    vTaskPrioritySet(शून्य, old_priority);
    // Short delay to allow tasks to clean up
    vTaskDelay(10);
}

TEST_CASE("Task snapshot: Get all", "[freertos]")
{
    // Short delay to allow both cores to spin up
    vTaskDelay(10);

    TaskHandle_t task_list[TEST_MAX_TASKS_अंक];
    पूर्णांक अंक_tasks;
    UBaseType_t old_priority;
    setup(task_list, &अंक_tasks, &old_priority);

    // Get task snapshots using uxTaskGetSnapshotAll()
    TaskSnapshot_t task_snapshots[TEST_MAX_TASKS_अंक];
    UBaseType_t tcb_आकार;
    UBaseType_t अंक_snapshots;
    अंक_snapshots = uxTaskGetSnapshotAll(task_snapshots, TEST_MAX_TASKS_अंक, &tcb_आकार);
    TEST_ASSERT_LESS_OR_EQUAL(TEST_MAX_TASKS_अंक, अंक_snapshots);

    check_snapshots(task_list, अंक_tasks, task_snapshots, अंक_snapshots);
    teardown(task_list, अंक_tasks, old_priority);
}

TEST_CASE("Task snapshot: Iterate", "[freertos]")
{
    // Short delay to allow both cores to spin up
    vTaskDelay(10);

    TaskHandle_t task_list[TEST_MAX_TASKS_अंक];
    पूर्णांक अंक_tasks;
    UBaseType_t old_priority;
    setup(task_list, &अंक_tasks, &old_priority);

    // Get task snapshots using xTaskGetNext() and vTaskGetSnapshot()
    TaskSnapshot_t task_snapshots[TEST_MAX_TASKS_अंक];
    UBaseType_t अंक_snapshots = 0;
    TaskIterator_t task_iterator = {0};
    जबतक (x_Task_Get_अग्र(&task_iterator) != -1) {
        अगर (task_iterator.pxTaskHandle != शून्य) {
            // Get the task's snapshot
            BaseType_t परिणाम_2 = vTaskGetSnapshot(task_iterator.pxTaskHandle, &task_snapshots[अंक_snapshots]);
            TEST_ASSERT_EQUAL(pdTRUE, परिणाम_2);
            अंक_snapshots++;
        }
    }
    TEST_ASSERT_LESS_OR_EQUAL(TEST_MAX_TASKS_अंक, अंक_snapshots);

    check_snapshots(task_list, अंक_tasks, task_snapshots, अंक_snapshots);
    teardown(task_list, अंक_tasks, old_priority);
}
