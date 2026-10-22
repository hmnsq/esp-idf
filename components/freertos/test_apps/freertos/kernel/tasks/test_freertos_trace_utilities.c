/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * Test FreeRTOS trace facility functions. These following functions are enabled
 * when configUSE_TRACE_FACILITY is defined 1 in FreeRTOS.
 * Tasks:           uxTaskGetTaskNumber(), uxTaskSetTaskNumber()
 * Queues:          ucQueueGetQueueType(), vQueueSetQueueNumber(), uxQueueGetQueueNumber()
 * Event Groups:    xEventGroupSetBitsFromISR(), xEventGroupClearBitsFromISR(), uxEventGroupGetNumber()
 *
 * Note: uxTaskGetSystemState() is tested in a separate unit test
 */
#অন্তর্ভাব "sdkconfig.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব "freertos/FreeRTOS.h"
#অন্তর্ভাব "freertos/task.h"
#অন্তর্ভাব "freertos/semphr.h"
#অন্তর্ভাব "freertos/event_groups.h"
#অন্তর্ভাব "unity.h"
#অন্তর্ভাব "test_utils.h"

#ifdef CONFIG_FREERTOS_USE_TRACE_FACILITY
#define TSK_PRIORITY    (UNITY_FREERTOS_PRIORITY + 1)

#define NO_OF_CORES CONFIG_FREERTOS_সংখ্যা_OF_CORES
#define BIN_SEM_QUEUE_TYPE queue_QUEUE_TYPE_দ্বিমিক_SEMAPHORE     //Expected Queue Type

স্থির QueueHandle_t test_queues[NO_OF_CORES];
স্থির TaskHandle_t task_handles[NO_OF_CORES];

শূন্য task_test_trace_utilities(শূন্য *arg)
{
    পূর্ণসংখ্যা core = xPortGetCoreID();
    TaskHandle_t handle = xTaskGetCurrentTaskHandle();
    uint32_t id = (uint32_t)arg;

    v_Task_Set_Task_সংখ্যা(handle, (UBaseType_t)id);    //cast and store id as task number
    v_Queue_Set_Queue_সংখ্যা(test_queues[core], id);    //store id as queue number

    //Wait to start
    xSemaphoreTake(test_queues[core], portMAX_DELAY);

    //Tests on this core
    TEST_ASSERT(ux_Task_Get_Task_সংখ্যা(task_handles[core]) == (0x0F << (core)));
    TEST_ASSERT(ux_Queue_Get_Queue_সংখ্যা(test_queues[core]) == (0x0F << (core)));
    TEST_ASSERT(ucQueueGetQueueType(test_queues[core]) == BIN_SEM_QUEUE_TYPE);

    //Test on other core
#ifndef CONFIG_FREERTOS_UNICORE
    TEST_ASSERT(ux_Task_Get_Task_সংখ্যা(task_handles[!core]) == (0x0F << (!core)));
    TEST_ASSERT(ux_Queue_Get_Queue_সংখ্যা(test_queues[!core]) == (0x0F << (!core)));
    TEST_ASSERT(ucQueueGetQueueType(test_queues[!core]) == BIN_SEM_QUEUE_TYPE);
#endif

    xSemaphoreGive(test_queues[core]);      //Signal done
    vTaskDelete(ফাঁকা);
}

TEST_CASE("Test freertos trace facility functions", "[freertos]")
{
    জন্য (পূর্ণসংখ্যা i = 0; i < NO_OF_CORES; i++) {
        test_queues[i] = x_Semaphore_Create_দ্বিমিক();   //Create a queue as binary semaphore for each core
        xTaskCreatePinnedToCore(task_test_trace_utilities, "Test Task", 4096, (শূন্য *)(0x0F << i), TSK_PRIORITY, &task_handles[i], i);
    }

    vTaskDelay(10);

    //Start the tasks
    জন্য (পূর্ণসংখ্যা i = NO_OF_CORES - 1; i >= 0; i--) {
        xSemaphoreGive(test_queues[i]);
    }

    vTaskDelay(10); //Small delay to ensure semaphores are taken

    //Wait for done
    জন্য (পূর্ণসংখ্যা i = 0; i < NO_OF_CORES; i++) {
        xSemaphoreTake(test_queues[i], portMAX_DELAY);
        vSemaphoreDelete(test_queues[i]);
    }

    vTaskDelay(10);     //Give time for idle task to clean up
}

#define MAX_TASKS           15
#define TASKS_TO_CREATE     5

স্থির TaskHandle_t created_handles[TASKS_TO_CREATE];
স্থির TaskStatus_t *tsk_status_array;

শূন্য created_task(শূন্য* arg)
{
    যখন (1) {
        vTaskDelay(100);
    }
}

TEST_CASE("Test freertos uxTaskGetSystemState", "[freertos]")
{
    tsk_status_array = calloc(MAX_TASKS, আকার(TaskStatus_t));
    জন্য (পূর্ণসংখ্যা i = 0; i < TASKS_TO_CREATE; i++) {
        xTaskCreatePinnedToCore(created_task, "Created Task", 1024, ফাঁকা, TSK_PRIORITY, &created_handles[i], 0);
    }

    //Get System states
    পূর্ণসংখ্যা no_of_tasks = uxTaskGetSystemState(tsk_status_array, MAX_TASKS, ফাঁকা);
    TEST_ASSERT((no_of_tasks > 0) && (no_of_tasks <= MAX_TASKS));

    //Check if get system state has got all created tasks
    bool not_found = false;
    জন্য (পূর্ণসংখ্যা i = 0; i < TASKS_TO_CREATE; i++) {
        bool found = false;
        জন্য (পূর্ণসংখ্যা j = 0; j < MAX_TASKS; j++) {
            যদি (tsk_status_array[j].xHandle == created_handles[i]) {
                found = true;
                বিরতি;
            }
        }
        যদি (!found) {
            not_found = true;
            বিরতি;
        }
    }
    TEST_ASSERT(not_found == false);

    //Cleanup
    জন্য (পূর্ণসংখ্যা i = 0; i < TASKS_TO_CREATE; i++) {
        vTaskDelete(created_handles[i]);
    }
    মুক্ত(tsk_status_array);
    vTaskDelay(10);
}

#endif //CONFIG_FREERTOS_USE_TRACE_FACILITY
