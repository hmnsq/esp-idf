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
#അന്തര്ഭാവ "sdkconfig.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ "freertos/FreeRTOS.h"
#അന്തര്ഭാവ "freertos/task.h"
#അന്തര്ഭാവ "freertos/semphr.h"
#അന്തര്ഭാവ "freertos/event_groups.h"
#അന്തര്ഭാവ "unity.h"
#അന്തര്ഭാവ "test_utils.h"

#ifdef CONFIG_FREERTOS_USE_TRACE_FACILITY
#define TSK_PRIORITY    (UNITY_FREERTOS_PRIORITY + 1)

#define NO_OF_CORES CONFIG_FREERTOS_സംഖ്യ_OF_CORES
#define BIN_SEM_QUEUE_TYPE queue_QUEUE_TYPE_ദ്വിമാന_SEMAPHORE     //Expected Queue Type

സ്ഥിതം QueueHandle_t test_queues[NO_OF_CORES];
സ്ഥിതം TaskHandle_t task_handles[NO_OF_CORES];

ശൂന്യം task_test_trace_utilities(ശൂന്യം *arg)
{
    പൂർണ്ണസംഖ്യ core = xPortGetCoreID();
    TaskHandle_t handle = xTaskGetCurrentTaskHandle();
    uint32_t id = (uint32_t)arg;

    v_Task_Set_Task_സംഖ്യ(handle, (UBaseType_t)id);    //cast and store id as task number
    v_Queue_Set_Queue_സംഖ്യ(test_queues[core], id);    //store id as queue number

    //Wait to start
    xSemaphoreTake(test_queues[core], portMAX_DELAY);

    //Tests on this core
    TEST_ASSERT(ux_Task_Get_Task_സംഖ്യ(task_handles[core]) == (0x0F << (core)));
    TEST_ASSERT(ux_Queue_Get_Queue_സംഖ്യ(test_queues[core]) == (0x0F << (core)));
    TEST_ASSERT(ucQueueGetQueueType(test_queues[core]) == BIN_SEM_QUEUE_TYPE);

    //Test on other core
#ifndef CONFIG_FREERTOS_UNICORE
    TEST_ASSERT(ux_Task_Get_Task_സംഖ്യ(task_handles[!core]) == (0x0F << (!core)));
    TEST_ASSERT(ux_Queue_Get_Queue_സംഖ്യ(test_queues[!core]) == (0x0F << (!core)));
    TEST_ASSERT(ucQueueGetQueueType(test_queues[!core]) == BIN_SEM_QUEUE_TYPE);
#endif

    xSemaphoreGive(test_queues[core]);      //Signal done
    vTaskDelete(ശൂന്യ);
}

TEST_CASE("Test freertos trace facility functions", "[freertos]")
{
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < NO_OF_CORES; i++) {
        test_queues[i] = x_Semaphore_Create_ദ്വിമാന();   //Create a queue as binary semaphore for each core
        xTaskCreatePinnedToCore(task_test_trace_utilities, "Test Task", 4096, (ശൂന്യം *)(0x0F << i), TSK_PRIORITY, &task_handles[i], i);
    }

    vTaskDelay(10);

    //Start the tasks
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = NO_OF_CORES - 1; i >= 0; i--) {
        xSemaphoreGive(test_queues[i]);
    }

    vTaskDelay(10); //Small delay to ensure semaphores are taken

    //Wait for done
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < NO_OF_CORES; i++) {
        xSemaphoreTake(test_queues[i], portMAX_DELAY);
        vSemaphoreDelete(test_queues[i]);
    }

    vTaskDelay(10);     //Give time for idle task to clean up
}

#define MAX_TASKS           15
#define TASKS_TO_CREATE     5

സ്ഥിതം TaskHandle_t created_handles[TASKS_TO_CREATE];
സ്ഥിതം TaskStatus_t *tsk_status_array;

ശൂന്യം created_task(ശൂന്യം* arg)
{
    ആകുമ്പോൾ (1) {
        vTaskDelay(100);
    }
}

TEST_CASE("Test freertos uxTaskGetSystemState", "[freertos]")
{
    tsk_status_array = calloc(MAX_TASKS, വലുപ്പം(TaskStatus_t));
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < TASKS_TO_CREATE; i++) {
        xTaskCreatePinnedToCore(created_task, "Created Task", 1024, ശൂന്യ, TSK_PRIORITY, &created_handles[i], 0);
    }

    //Get System states
    പൂർണ്ണസംഖ്യ no_of_tasks = uxTaskGetSystemState(tsk_status_array, MAX_TASKS, ശൂന്യ);
    TEST_ASSERT((no_of_tasks > 0) && (no_of_tasks <= MAX_TASKS));

    //Check if get system state has got all created tasks
    bool not_found = false;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < TASKS_TO_CREATE; i++) {
        bool found = false;
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < MAX_TASKS; j++) {
            എങ്കിൽ (tsk_status_array[j].xHandle == created_handles[i]) {
                found = true;
                നിർത്തുക;
            }
        }
        എങ്കിൽ (!found) {
            not_found = true;
            നിർത്തുക;
        }
    }
    TEST_ASSERT(not_found == false);

    //Cleanup
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < TASKS_TO_CREATE; i++) {
        vTaskDelete(created_handles[i]);
    }
    സ്വതന്ത്രം(tsk_status_array);
    vTaskDelay(10);
}

#endif //CONFIG_FREERTOS_USE_TRACE_FACILITY
