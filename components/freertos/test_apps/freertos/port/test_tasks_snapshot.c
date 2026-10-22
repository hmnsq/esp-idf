/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdbool.h>
#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"
#अन्तर्भाव "freertos/freertos_debug.h"
#अन्तर्भाव "esp_cpu.h"
#अन्तर्भाव "esp_rom_sys.h"
#अन्तर्भाव "unity.h"
#अन्तर्भाव "sdkconfig.h"

#define TEST_MAX_TASKS_अंक      32
#define अंक_TASKS_PER_LIST      2
#define TASK_PRIORITY           (configMAX_PRIORITIES - 2)

स्थिर अस्थिर bool start_ready_flag = false;
स्थिर अस्थिर bool start_resp_flag = false;
स्थिर अस्थिर bool stop_flag = false;

स्थिर शून्य ready_task(शून्य *arg)
{
    पर्यंत (1) {
#जर !CONFIG_FREERTOS_UNICORE
        taskDISABLE_INTERRUPTS();
        /*
        The main task (on core 0) has a higher priority than this task. Thus, by the time this flag is set, this task
        is guaranteed to running on core 1.
        */
        जर (start_ready_flag) {   // Is the test starting?
            start_resp_flag = true;   // Indicate we are ready to start
            पर्यंत (!stop_flag) {
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

स्थिर शून्य blocked_task(शून्य *arg)
{
    // Delay for portMAX_DELAY - 1 as not to go on the suspended list
    vTaskDelay(portMAX_DELAY - 1);
}

स्थिर शून्य suspended_task(शून्य *arg)
{
    vTaskSuspend(रिक्त);
}

स्थिर शून्य setup(TaskHandle_t *task_list, पूर्णांक *अंक_tasks_ret, UBaseType_t *old_priority_ret)
{
    // Raise our priority so that we aren't preempted
    *old_priority_ret = uxTaskPriorityGet(रिक्त);
    vTaskPrioritySet(रिक्त, configMAX_PRIORITIES - 1);

    // Create tasks
    पूर्णांक अंक_tasks = 0;
    साठी (पूर्णांक i = 0; i < अंक_TASKS_PER_LIST; i++) {
        //Ready task
        xTaskCreate(ready_task, "ready", 1024, रिक्त, TASK_PRIORITY, &(task_list[अंक_tasks]));
        अंक_tasks++;
        //Blocked task
        xTaskCreate(blocked_task, "blkd", 1024, रिक्त, TASK_PRIORITY, &(task_list[अंक_tasks]));
        अंक_tasks++;
        //Suspended task
        xTaskCreate(suspended_task, "susp", 1024, रिक्त, TASK_PRIORITY, &(task_list[अंक_tasks]));
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
#जर !CONFIG_FREERTOS_UNICORE
    // For SMP, we need to do a 2-way handshake to ensure the other core spins with interrupts disabled.
    start_ready_flag = true;
    पर्यंत (!start_resp_flag) {
        ;
    }
#endif // !CONFIG_FREERTOS_UNICORE
}

स्थिर शून्य check_snapshots(TaskHandle_t *task_list, पूर्णांक अंक_tasks, TaskSnapshot_t *task_snapshots, UBaseType_t अंक_snapshots)
{
    // Check task snapshots. Every created task should be found in the task snapshot
    साठी (पूर्णांक i = 0; i < अंक_tasks; i++) {
        bool found = false;
        साठी (पूर्णांक j = 0; j < अंक_snapshots; j++) {
            जर (task_list[i] == (TaskHandle_t)task_snapshots[j].pxTCB) {
                found = true;
                थांबा;
            }
        }
        TEST_ASSERT(found);
    }
}

स्थिर शून्य teardown(TaskHandle_t *task_list, पूर्णांक अंक_tasks, UBaseType_t old_priority)
{
#जर !CONFIG_FREERTOS_UNICORE
    // Signal to the other core that it can exit the loop
    stop_flag = true;
#endif // !CONFIG_FREERTOS_UNICORE
    // Re-enable interrupts on the current core.
    taskENABLE_INTERRUPTS();

    साठी (पूर्णांक i = 0; i < अंक_tasks; i++) {
        vTaskDelete(task_list[i]);
    }
    // Restore priority
    vTaskPrioritySet(रिक्त, old_priority);
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
    UBaseType_t tcb_size;
    UBaseType_t अंक_snapshots;
    अंक_snapshots = uxTaskGetSnapshotAll(task_snapshots, TEST_MAX_TASKS_अंक, &tcb_size);
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
    पर्यंत (xTaskGetNext(&task_iterator) != -1) {
        जर (task_iterator.pxTaskHandle != रिक्त) {
            // Get the task's snapshot
            BaseType_t Result = vTaskGetSnapshot(task_iterator.pxTaskHandle, &task_snapshots[अंक_snapshots]);
            TEST_ASSERT_EQUAL(pdTRUE, Result);
            अंक_snapshots++;
        }
    }
    TEST_ASSERT_LESS_OR_EQUAL(TEST_MAX_TASKS_अंक, अंक_snapshots);

    check_snapshots(task_list, अंक_tasks, task_snapshots, अंक_snapshots);
    teardown(task_list, अंक_tasks, old_priority);
}
