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

#परिभाषय TEST_MAX_TASKS_अङ्कः      32
#परिभाषय अङ्कः_TASKS_प्रत्येकम्_LIST      2
#परिभाषय TASK_प्राथमिकता           (configMAX_PRIORITIES - 2)

स्थिर चञ्चल bool start_ready_flag = false;
स्थिर चञ्चल bool start_resp_flag = false;
स्थिर चञ्चल bool stop_flag = false;

स्थिर शून्य ready_task(शून्य *arg)
{
    यावत् (1) {
#यदि !CONFIG_FREERTOS_UNICORE
        taskDISABLE_INTERRUPTS();
        /*
        The main task (on core 0) has a higher priority than this task. Thus, by the time this flag is set, this task
        is guaranteed to running on core 1.
        */
        यदि (start_ready_flag) {   // Is the test starting?
            start_resp_flag = true;   // Indicate we are ready to start
            यावत् (!stop_flag) {
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
#यदि_अन्तः // !CONFIG_FREERTOS_UNICORE
    }
}

स्थिर शून्य blocked_task(शून्य *arg)
{
    // Delay for portMAX_DELAY - 1 as not to go on the suspended list
    vTaskDelay(पत्तनम्_MAX_DELAY - 1);
}

स्थिर शून्य suspended_task(शून्य *arg)
{
    vTaskSuspend(रिक्त);
}

स्थिर शून्य सज्जता(TaskHandle_t *task_list, पूर्णाङ्क *अङ्कः_tasks_ret, UBaseType_t *old_प्राथमिकता_ret)
{
    // Raise our priority so that we aren't preempted
    *old_प्राथमिकता_ret = ux_Task_प्राथमिकता_Get(रिक्त);
    v_Task_प्राथमिकता_समुच्चयः(रिक्त, configMAX_PRIORITIES - 1);

    // Create tasks
    पूर्णाङ्क अङ्कः_tasks = 0;
    प्रति (पूर्णाङ्क i = 0; i < अङ्कः_TASKS_प्रत्येकम्_LIST; i++) {
        //Ready task
        xTaskCreate(ready_task, "ready", 1024, रिक्त, TASK_प्राथमिकता, &(task_list[अङ्कः_tasks]));
        अङ्कः_tasks++;
        //Blocked task
        xTaskCreate(blocked_task, "blkd", 1024, रिक्त, TASK_प्राथमिकता, &(task_list[अङ्कः_tasks]));
        अङ्कः_tasks++;
        //Suspended task
        xTaskCreate(suspended_task, "susp", 1024, रिक्त, TASK_प्राथमिकता, &(task_list[अङ्कः_tasks]));
        अङ्कः_tasks++;
    }
    *अङ्कः_tasks_ret = अङ्कः_tasks;
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
#यदि !CONFIG_FREERTOS_UNICORE
    // For SMP, we need to do a 2-way handshake to ensure the other core spins with interrupts disabled.
    start_ready_flag = true;
    यावत् (!start_resp_flag) {
        ;
    }
#यदि_अन्तः // !CONFIG_FREERTOS_UNICORE
}

स्थिर शून्य check_snapshots(TaskHandle_t *task_list, पूर्णाङ्क अङ्कः_tasks, TaskSnapshot_t *task_snapshots, UBaseType_t अङ्कः_snapshots)
{
    // Check task snapshots. Every created task should be found in the task snapshot
    प्रति (पूर्णाङ्क i = 0; i < अङ्कः_tasks; i++) {
        bool प्राप्तम् = false;
        प्रति (पूर्णाङ्क j = 0; j < अङ्कः_snapshots; j++) {
            यदि (task_list[i] == (TaskHandle_t)task_snapshots[j].pxTCB) {
                प्राप्तम् = true;
                भङ्ग;
            }
        }
        TEST_ASSERT(प्राप्तम्);
    }
}

स्थिर शून्य teardown(TaskHandle_t *task_list, पूर्णाङ्क अङ्कः_tasks, UBaseType_t old_प्राथमिकता)
{
#यदि !CONFIG_FREERTOS_UNICORE
    // Signal to the other core that it can exit the loop
    stop_flag = true;
#यदि_अन्तः // !CONFIG_FREERTOS_UNICORE
    // Re-enable interrupts on the current core.
    taskENABLE_INTERRUPTS();

    प्रति (पूर्णाङ्क i = 0; i < अङ्कः_tasks; i++) {
        vTaskDelete(task_list[i]);
    }
    // Restore priority
    v_Task_प्राथमिकता_समुच्चयः(रिक्त, old_प्राथमिकता);
    // Short delay to allow tasks to clean up
    vTaskDelay(10);
}

TEST_CASE("Task snapshot: Get all", "[freertos]")
{
    // Short delay to allow both cores to spin up
    vTaskDelay(10);

    TaskHandle_t task_list[TEST_MAX_TASKS_अङ्कः];
    पूर्णाङ्क अङ्कः_tasks;
    UBaseType_t old_प्राथमिकता;
    सज्जता(task_list, &अङ्कः_tasks, &old_प्राथमिकता);

    // Get task snapshots using uxTaskGetSnapshotAll()
    TaskSnapshot_t task_snapshots[TEST_MAX_TASKS_अङ्कः];
    UBaseType_t tcb_आकार;
    UBaseType_t अङ्कः_snapshots;
    अङ्कः_snapshots = uxTaskGetSnapshotAll(task_snapshots, TEST_MAX_TASKS_अङ्कः, &tcb_आकार);
    TEST_ASSERT_LESS_OR_EQUAL(TEST_MAX_TASKS_अङ्कः, अङ्कः_snapshots);

    check_snapshots(task_list, अङ्कः_tasks, task_snapshots, अङ्कः_snapshots);
    teardown(task_list, अङ्कः_tasks, old_प्राथमिकता);
}

TEST_CASE("Task snapshot: Iterate", "[freertos]")
{
    // Short delay to allow both cores to spin up
    vTaskDelay(10);

    TaskHandle_t task_list[TEST_MAX_TASKS_अङ्कः];
    पूर्णाङ्क अङ्कः_tasks;
    UBaseType_t old_प्राथमिकता;
    सज्जता(task_list, &अङ्कः_tasks, &old_प्राथमिकता);

    // Get task snapshots using xTaskGetNext() and vTaskGetSnapshot()
    TaskSnapshot_t task_snapshots[TEST_MAX_TASKS_अङ्कः];
    UBaseType_t अङ्कः_snapshots = 0;
    TaskIterator_t task_iterator = {0};
    यावत् (x_Task_Get_अग्र(&task_iterator) != -1) {
        यदि (task_iterator.pxTaskHandle != रिक्त) {
            // Get the task's snapshot
            आधारः_Type_t परिणाम_2 = vTaskGetSnapshot(task_iterator.pxTaskHandle, &task_snapshots[अङ्कः_snapshots]);
            TEST_ASSERT_EQUAL(pdTRUE, परिणाम_2);
            अङ्कः_snapshots++;
        }
    }
    TEST_ASSERT_LESS_OR_EQUAL(TEST_MAX_TASKS_अङ्कः, अङ्कः_snapshots);

    check_snapshots(task_list, अङ्कः_tasks, task_snapshots, अङ्कः_snapshots);
    teardown(task_list, अङ्कः_tasks, old_प्राथमिकता);
}
