/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ଅନ୍ତର୍ଭାଵ "sdkconfig.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdbool.h>
#ଅନ୍ତର୍ଭାଵ "freertos/FreeRTOS.h"
#ଅନ୍ତର୍ଭାଵ "freertos/task.h"
#ଅନ୍ତର୍ଭାଵ "freertos/freertos_debug.h"
#ଅନ୍ତର୍ଭାଵ "esp_cpu.h"
#ଅନ୍ତର୍ଭାଵ "esp_rom_sys.h"
#ଅନ୍ତର୍ଭାଵ "unity.h"
#ଅନ୍ତର୍ଭାଵ "sdkconfig.h"

#define TEST_MAX_TASKS_ଅଙ୍କ      32
#define ଅଙ୍କ_TASKS_PER_LIST      2
#define TASK_PRIORITY           (configMAX_PRIORITIES - 2)

ସ୍ଥିର ଅସ୍ଥିର bool start_ready_flag = false;
ସ୍ଥିର ଅସ୍ଥିର bool start_resp_flag = false;
ସ୍ଥିର ଅସ୍ଥିର bool stop_flag = false;

ସ୍ଥିର ଶୂନ୍ୟ ready_task(ଶୂନ୍ୟ *arg)
{
    ଯେତେବେଳେ (1) {
#ଯଦି !CONFIG_FREERTOS_UNICORE
        taskDISABLE_INTERRUPTS();
        /*
        The main task (on core 0) has a higher priority than this task. Thus, by the time this flag is set, this task
        is guaranteed to running on core 1.
        */
        ଯଦି (start_ready_flag) {   // Is the test starting?
            start_resp_flag = true;   // Indicate we are ready to start
            ଯେତେବେଳେ (!stop_flag) {
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

ସ୍ଥିର ଶୂନ୍ୟ blocked_task(ଶୂନ୍ୟ *arg)
{
    // Delay for portMAX_DELAY - 1 as not to go on the suspended list
    vTaskDelay(portMAX_DELAY - 1);
}

ସ୍ଥିର ଶୂନ୍ୟ suspended_task(ଶୂନ୍ୟ *arg)
{
    vTaskSuspend(ଖାଲି);
}

ସ୍ଥିର ଶୂନ୍ୟ setup(TaskHandle_t *task_list, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ଅଙ୍କ_tasks_ret, UBaseType_t *old_priority_ret)
{
    // Raise our priority so that we aren't preempted
    *old_priority_ret = uxTaskPriorityGet(ଖାଲି);
    vTaskPrioritySet(ଖାଲି, configMAX_PRIORITIES - 1);

    // Create tasks
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_tasks = 0;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଅଙ୍କ_TASKS_PER_LIST; i++) {
        //Ready task
        xTaskCreate(ready_task, "ready", 1024, ଖାଲି, TASK_PRIORITY, &(task_list[ଅଙ୍କ_tasks]));
        ଅଙ୍କ_tasks++;
        //Blocked task
        xTaskCreate(blocked_task, "blkd", 1024, ଖାଲି, TASK_PRIORITY, &(task_list[ଅଙ୍କ_tasks]));
        ଅଙ୍କ_tasks++;
        //Suspended task
        xTaskCreate(suspended_task, "susp", 1024, ଖାଲି, TASK_PRIORITY, &(task_list[ଅଙ୍କ_tasks]));
        ଅଙ୍କ_tasks++;
    }
    *ଅଙ୍କ_tasks_ret = ଅଙ୍କ_tasks;
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
#ଯଦି !CONFIG_FREERTOS_UNICORE
    // For SMP, we need to do a 2-way handshake to ensure the other core spins with interrupts disabled.
    start_ready_flag = true;
    ଯେତେବେଳେ (!start_resp_flag) {
        ;
    }
#endif // !CONFIG_FREERTOS_UNICORE
}

ସ୍ଥିର ଶୂନ୍ୟ check_snapshots(TaskHandle_t *task_list, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_tasks, TaskSnapshot_t *task_snapshots, UBaseType_t ଅଙ୍କ_snapshots)
{
    // Check task snapshots. Every created task should be found in the task snapshot
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଅଙ୍କ_tasks; i++) {
        bool found = false;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < ଅଙ୍କ_snapshots; j++) {
            ଯଦି (task_list[i] == (TaskHandle_t)task_snapshots[j].pxTCB) {
                found = true;
                ଭାଙ୍ଗ;
            }
        }
        TEST_ASSERT(found);
    }
}

ସ୍ଥିର ଶୂନ୍ୟ teardown(TaskHandle_t *task_list, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_tasks, UBaseType_t old_priority)
{
#ଯଦି !CONFIG_FREERTOS_UNICORE
    // Signal to the other core that it can exit the loop
    stop_flag = true;
#endif // !CONFIG_FREERTOS_UNICORE
    // Re-enable interrupts on the current core.
    taskENABLE_INTERRUPTS();

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଅଙ୍କ_tasks; i++) {
        vTaskDelete(task_list[i]);
    }
    // Restore priority
    vTaskPrioritySet(ଖାଲି, old_priority);
    // Short delay to allow tasks to clean up
    vTaskDelay(10);
}

TEST_CASE("Task snapshot: Get all", "[freertos]")
{
    // Short delay to allow both cores to spin up
    vTaskDelay(10);

    TaskHandle_t task_list[TEST_MAX_TASKS_ଅଙ୍କ];
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_tasks;
    UBaseType_t old_priority;
    setup(task_list, &ଅଙ୍କ_tasks, &old_priority);

    // Get task snapshots using uxTaskGetSnapshotAll()
    TaskSnapshot_t task_snapshots[TEST_MAX_TASKS_ଅଙ୍କ];
    UBaseType_t tcb_size;
    UBaseType_t ଅଙ୍କ_snapshots;
    ଅଙ୍କ_snapshots = uxTaskGetSnapshotAll(task_snapshots, TEST_MAX_TASKS_ଅଙ୍କ, &tcb_size);
    TEST_ASSERT_LESS_OR_EQUAL(TEST_MAX_TASKS_ଅଙ୍କ, ଅଙ୍କ_snapshots);

    check_snapshots(task_list, ଅଙ୍କ_tasks, task_snapshots, ଅଙ୍କ_snapshots);
    teardown(task_list, ଅଙ୍କ_tasks, old_priority);
}

TEST_CASE("Task snapshot: Iterate", "[freertos]")
{
    // Short delay to allow both cores to spin up
    vTaskDelay(10);

    TaskHandle_t task_list[TEST_MAX_TASKS_ଅଙ୍କ];
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_tasks;
    UBaseType_t old_priority;
    setup(task_list, &ଅଙ୍କ_tasks, &old_priority);

    // Get task snapshots using xTaskGetNext() and vTaskGetSnapshot()
    TaskSnapshot_t task_snapshots[TEST_MAX_TASKS_ଅଙ୍କ];
    UBaseType_t ଅଙ୍କ_snapshots = 0;
    TaskIterator_t task_iterator = {0};
    ଯେତେବେଳେ (xTaskGetNext(&task_iterator) != -1) {
        ଯଦି (task_iterator.pxTaskHandle != ଖାଲି) {
            // Get the task's snapshot
            BaseType_t Result = vTaskGetSnapshot(task_iterator.pxTaskHandle, &task_snapshots[ଅଙ୍କ_snapshots]);
            TEST_ASSERT_EQUAL(pdTRUE, Result);
            ଅଙ୍କ_snapshots++;
        }
    }
    TEST_ASSERT_LESS_OR_EQUAL(TEST_MAX_TASKS_ଅଙ୍କ, ଅଙ୍କ_snapshots);

    check_snapshots(task_list, ଅଙ୍କ_tasks, task_snapshots, ଅଙ୍କ_snapshots);
    teardown(task_list, ଅଙ୍କ_tasks, old_priority);
}
