/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#അന്തര്ഭാവ "sdkconfig.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdbool.h>
#അന്തര്ഭാവ "freertos/FreeRTOS.h"
#അന്തര്ഭാവ "freertos/task.h"
#അന്തര്ഭാവ "freertos/freertos_debug.h"
#അന്തര്ഭാവ "esp_cpu.h"
#അന്തര്ഭാവ "esp_rom_sys.h"
#അന്തര്ഭാവ "unity.h"
#അന്തര്ഭാവ "sdkconfig.h"

#define TEST_MAX_TASKS_അക്കം      32
#define അക്കം_TASKS_PER_LIST      2
#define TASK_PRIORITY           (configMAX_PRIORITIES - 2)

സ്ഥിതം അസ്ഥിര bool start_ready_flag = false;
സ്ഥിതം അസ്ഥിര bool start_resp_flag = false;
സ്ഥിതം അസ്ഥിര bool stop_flag = false;

സ്ഥിതം ശൂന്യം ready_task(ശൂന്യം *arg)
{
    ആകുമ്പോൾ (1) {
#എങ്കിൽ !CONFIG_FREERTOS_UNICORE
        taskDISABLE_INTERRUPTS();
        /*
        The main task (on core 0) has a higher priority than this task. Thus, by the time this flag is set, this task
        is guaranteed to running on core 1.
        */
        എങ്കിൽ (start_ready_flag) {   // Is the test starting?
            start_resp_flag = true;   // Indicate we are ready to start
            ആകുമ്പോൾ (!stop_flag) {
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

സ്ഥിതം ശൂന്യം blocked_task(ശൂന്യം *arg)
{
    // Delay for portMAX_DELAY - 1 as not to go on the suspended list
    vTaskDelay(portMAX_DELAY - 1);
}

സ്ഥിതം ശൂന്യം suspended_task(ശൂന്യം *arg)
{
    vTaskSuspend(ശൂന്യ);
}

സ്ഥിതം ശൂന്യം setup(TaskHandle_t *task_list, പൂർണ്ണസംഖ്യ *അക്കം_tasks_ret, UBaseType_t *old_priority_ret)
{
    // Raise our priority so that we aren't preempted
    *old_priority_ret = uxTaskPriorityGet(ശൂന്യ);
    vTaskPrioritySet(ശൂന്യ, configMAX_PRIORITIES - 1);

    // Create tasks
    പൂർണ്ണസംഖ്യ അക്കം_tasks = 0;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_TASKS_PER_LIST; i++) {
        //Ready task
        xTaskCreate(ready_task, "ready", 1024, ശൂന്യ, TASK_PRIORITY, &(task_list[അക്കം_tasks]));
        അക്കം_tasks++;
        //Blocked task
        xTaskCreate(blocked_task, "blkd", 1024, ശൂന്യ, TASK_PRIORITY, &(task_list[അക്കം_tasks]));
        അക്കം_tasks++;
        //Suspended task
        xTaskCreate(suspended_task, "susp", 1024, ശൂന്യ, TASK_PRIORITY, &(task_list[അക്കം_tasks]));
        അക്കം_tasks++;
    }
    *അക്കം_tasks_ret = അക്കം_tasks;
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
#എങ്കിൽ !CONFIG_FREERTOS_UNICORE
    // For SMP, we need to do a 2-way handshake to ensure the other core spins with interrupts disabled.
    start_ready_flag = true;
    ആകുമ്പോൾ (!start_resp_flag) {
        ;
    }
#endif // !CONFIG_FREERTOS_UNICORE
}

സ്ഥിതം ശൂന്യം check_snapshots(TaskHandle_t *task_list, പൂർണ്ണസംഖ്യ അക്കം_tasks, TaskSnapshot_t *task_snapshots, UBaseType_t അക്കം_snapshots)
{
    // Check task snapshots. Every created task should be found in the task snapshot
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_tasks; i++) {
        bool found = false;
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < അക്കം_snapshots; j++) {
            എങ്കിൽ (task_list[i] == (TaskHandle_t)task_snapshots[j].pxTCB) {
                found = true;
                നിർത്തുക;
            }
        }
        TEST_ASSERT(found);
    }
}

സ്ഥിതം ശൂന്യം teardown(TaskHandle_t *task_list, പൂർണ്ണസംഖ്യ അക്കം_tasks, UBaseType_t old_priority)
{
#എങ്കിൽ !CONFIG_FREERTOS_UNICORE
    // Signal to the other core that it can exit the loop
    stop_flag = true;
#endif // !CONFIG_FREERTOS_UNICORE
    // Re-enable interrupts on the current core.
    taskENABLE_INTERRUPTS();

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_tasks; i++) {
        vTaskDelete(task_list[i]);
    }
    // Restore priority
    vTaskPrioritySet(ശൂന്യ, old_priority);
    // Short delay to allow tasks to clean up
    vTaskDelay(10);
}

TEST_CASE("Task snapshot: Get all", "[freertos]")
{
    // Short delay to allow both cores to spin up
    vTaskDelay(10);

    TaskHandle_t task_list[TEST_MAX_TASKS_അക്കം];
    പൂർണ്ണസംഖ്യ അക്കം_tasks;
    UBaseType_t old_priority;
    setup(task_list, &അക്കം_tasks, &old_priority);

    // Get task snapshots using uxTaskGetSnapshotAll()
    TaskSnapshot_t task_snapshots[TEST_MAX_TASKS_അക്കം];
    UBaseType_t tcb_size;
    UBaseType_t അക്കം_snapshots;
    അക്കം_snapshots = uxTaskGetSnapshotAll(task_snapshots, TEST_MAX_TASKS_അക്കം, &tcb_size);
    TEST_ASSERT_LESS_OR_EQUAL(TEST_MAX_TASKS_അക്കം, അക്കം_snapshots);

    check_snapshots(task_list, അക്കം_tasks, task_snapshots, അക്കം_snapshots);
    teardown(task_list, അക്കം_tasks, old_priority);
}

TEST_CASE("Task snapshot: Iterate", "[freertos]")
{
    // Short delay to allow both cores to spin up
    vTaskDelay(10);

    TaskHandle_t task_list[TEST_MAX_TASKS_അക്കം];
    പൂർണ്ണസംഖ്യ അക്കം_tasks;
    UBaseType_t old_priority;
    setup(task_list, &അക്കം_tasks, &old_priority);

    // Get task snapshots using xTaskGetNext() and vTaskGetSnapshot()
    TaskSnapshot_t task_snapshots[TEST_MAX_TASKS_അക്കം];
    UBaseType_t അക്കം_snapshots = 0;
    TaskIterator_t task_iterator = {0};
    ആകുമ്പോൾ (xTaskGetNext(&task_iterator) != -1) {
        എങ്കിൽ (task_iterator.pxTaskHandle != ശൂന്യ) {
            // Get the task's snapshot
            BaseType_t Result = vTaskGetSnapshot(task_iterator.pxTaskHandle, &task_snapshots[അക്കം_snapshots]);
            TEST_ASSERT_EQUAL(pdTRUE, Result);
            അക്കം_snapshots++;
        }
    }
    TEST_ASSERT_LESS_OR_EQUAL(TEST_MAX_TASKS_അക്കം, അക്കം_snapshots);

    check_snapshots(task_list, അക്കം_tasks, task_snapshots, അക്കം_snapshots);
    teardown(task_list, അക്കം_tasks, old_priority);
}
