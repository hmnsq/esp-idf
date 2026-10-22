/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#அந்தர்பாவ "sdkconfig.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdbool.h>
#அந்தர்பாவ "freertos/FreeRTOS.h"
#அந்தர்பாவ "freertos/task.h"
#அந்தர்பாவ "freertos/freertos_debug.h"
#அந்தர்பாவ "esp_cpu.h"
#அந்தர்பாவ "esp_rom_sys.h"
#அந்தர்பாவ "unity.h"
#அந்தர்பாவ "sdkconfig.h"

#define TEST_MAX_TASKS_இலக்கம்      32
#define இலக்கம்_TASKS_PER_LIST      2
#define TASK_PRIORITY           (configMAX_PRIORITIES - 2)

நிலையான நிலையற்ற bool start_ready_flag = false;
நிலையான நிலையற்ற bool start_resp_flag = false;
நிலையான நிலையற்ற bool stop_flag = false;

நிலையான வெறுமை ready_task(வெறுமை *arg)
{
    போது (1) {
#எனில் !CONFIG_FREERTOS_UNICORE
        taskDISABLE_INTERRUPTS();
        /*
        The main task (on core 0) has a higher priority than this task. Thus, by the time this flag is set, this task
        is guaranteed to running on core 1.
        */
        எனில் (start_ready_flag) {   // Is the test starting?
            start_resp_flag = true;   // Indicate we are ready to start
            போது (!stop_flag) {
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

நிலையான வெறுமை blocked_task(வெறுமை *arg)
{
    // Delay for portMAX_DELAY - 1 as not to go on the suspended list
    vTaskDelay(portMAX_DELAY - 1);
}

நிலையான வெறுமை suspended_task(வெறுமை *arg)
{
    vTaskSuspend(வெற்று);
}

நிலையான வெறுமை setup(TaskHandle_t *task_list, முழு *இலக்கம்_tasks_ret, UBaseType_t *old_priority_ret)
{
    // Raise our priority so that we aren't preempted
    *old_priority_ret = uxTaskPriorityGet(வெற்று);
    vTaskPrioritySet(வெற்று, configMAX_PRIORITIES - 1);

    // Create tasks
    முழு இலக்கம்_tasks = 0;
    க்கு (முழு i = 0; i < இலக்கம்_TASKS_PER_LIST; i++) {
        //Ready task
        xTaskCreate(ready_task, "ready", 1024, வெற்று, TASK_PRIORITY, &(task_list[இலக்கம்_tasks]));
        இலக்கம்_tasks++;
        //Blocked task
        xTaskCreate(blocked_task, "blkd", 1024, வெற்று, TASK_PRIORITY, &(task_list[இலக்கம்_tasks]));
        இலக்கம்_tasks++;
        //Suspended task
        xTaskCreate(suspended_task, "susp", 1024, வெற்று, TASK_PRIORITY, &(task_list[இலக்கம்_tasks]));
        இலக்கம்_tasks++;
    }
    *இலக்கம்_tasks_ret = இலக்கம்_tasks;
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
#எனில் !CONFIG_FREERTOS_UNICORE
    // For SMP, we need to do a 2-way handshake to ensure the other core spins with interrupts disabled.
    start_ready_flag = true;
    போது (!start_resp_flag) {
        ;
    }
#endif // !CONFIG_FREERTOS_UNICORE
}

நிலையான வெறுமை check_snapshots(TaskHandle_t *task_list, முழு இலக்கம்_tasks, TaskSnapshot_t *task_snapshots, UBaseType_t இலக்கம்_snapshots)
{
    // Check task snapshots. Every created task should be found in the task snapshot
    க்கு (முழு i = 0; i < இலக்கம்_tasks; i++) {
        bool found = false;
        க்கு (முழு j = 0; j < இலக்கம்_snapshots; j++) {
            எனில் (task_list[i] == (TaskHandle_t)task_snapshots[j].pxTCB) {
                found = true;
                நிறுத்து;
            }
        }
        TEST_ASSERT(found);
    }
}

நிலையான வெறுமை teardown(TaskHandle_t *task_list, முழு இலக்கம்_tasks, UBaseType_t old_priority)
{
#எனில் !CONFIG_FREERTOS_UNICORE
    // Signal to the other core that it can exit the loop
    stop_flag = true;
#endif // !CONFIG_FREERTOS_UNICORE
    // Re-enable interrupts on the current core.
    taskENABLE_INTERRUPTS();

    க்கு (முழு i = 0; i < இலக்கம்_tasks; i++) {
        vTaskDelete(task_list[i]);
    }
    // Restore priority
    vTaskPrioritySet(வெற்று, old_priority);
    // Short delay to allow tasks to clean up
    vTaskDelay(10);
}

TEST_CASE("Task snapshot: Get all", "[freertos]")
{
    // Short delay to allow both cores to spin up
    vTaskDelay(10);

    TaskHandle_t task_list[TEST_MAX_TASKS_இலக்கம்];
    முழு இலக்கம்_tasks;
    UBaseType_t old_priority;
    setup(task_list, &இலக்கம்_tasks, &old_priority);

    // Get task snapshots using uxTaskGetSnapshotAll()
    TaskSnapshot_t task_snapshots[TEST_MAX_TASKS_இலக்கம்];
    UBaseType_t tcb_size;
    UBaseType_t இலக்கம்_snapshots;
    இலக்கம்_snapshots = uxTaskGetSnapshotAll(task_snapshots, TEST_MAX_TASKS_இலக்கம், &tcb_size);
    TEST_ASSERT_LESS_OR_EQUAL(TEST_MAX_TASKS_இலக்கம், இலக்கம்_snapshots);

    check_snapshots(task_list, இலக்கம்_tasks, task_snapshots, இலக்கம்_snapshots);
    teardown(task_list, இலக்கம்_tasks, old_priority);
}

TEST_CASE("Task snapshot: Iterate", "[freertos]")
{
    // Short delay to allow both cores to spin up
    vTaskDelay(10);

    TaskHandle_t task_list[TEST_MAX_TASKS_இலக்கம்];
    முழு இலக்கம்_tasks;
    UBaseType_t old_priority;
    setup(task_list, &இலக்கம்_tasks, &old_priority);

    // Get task snapshots using xTaskGetNext() and vTaskGetSnapshot()
    TaskSnapshot_t task_snapshots[TEST_MAX_TASKS_இலக்கம்];
    UBaseType_t இலக்கம்_snapshots = 0;
    TaskIterator_t task_iterator = {0};
    போது (xTaskGetNext(&task_iterator) != -1) {
        எனில் (task_iterator.pxTaskHandle != வெற்று) {
            // Get the task's snapshot
            BaseType_t Result = vTaskGetSnapshot(task_iterator.pxTaskHandle, &task_snapshots[இலக்கம்_snapshots]);
            TEST_ASSERT_EQUAL(pdTRUE, Result);
            இலக்கம்_snapshots++;
        }
    }
    TEST_ASSERT_LESS_OR_EQUAL(TEST_MAX_TASKS_இலக்கம், இலக்கம்_snapshots);

    check_snapshots(task_list, இலக்கம்_tasks, task_snapshots, இலக்கம்_snapshots);
    teardown(task_list, இலக்கம்_tasks, old_priority);
}
