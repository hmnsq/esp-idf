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
#समावेश "sdkconfig.h"
#समावेश <stdio.h>
#समावेश <stdlib.h>
#समावेश <string.h>
#समावेश "freertos/FreeRTOS.h"
#समावेश "freertos/task.h"
#समावेश "freertos/semphr.h"
#समावेश "freertos/event_groups.h"
#समावेश "unity.h"
#समावेश "test_utils.h"

#अगर_घोषित CONFIG_FREERTOS_USE_TRACE_FACILITY
#घोषणा TSK_PRIORITY    (UNITY_FREERTOS_PRIORITY + 1)

#घोषणा NO_OF_CORES CONFIG_FREERTOS_संख्या_OF_CORES
#घोषणा BIN_SEM_QUEUE_TYPE queue_QUEUE_TYPE_द्विआधारी_SEMAPHORE     //Expected Queue Type

अटल QueueHandle_t test_queues[NO_OF_CORES];
अटल TaskHandle_t task_handles[NO_OF_CORES];

व्योम task_test_trace_utilities(व्योम *arg)
{
    पूर्णांक core = xPortGetCoreID();
    TaskHandle_t handle = xTaskGetCurrentTaskHandle();
    uint32_t id = (uint32_t)arg;

    v_Task_Set_Task_संख्या(handle, (UBaseType_t)id);    //cast and store id as task number
    v_Queue_Set_Queue_संख्या(test_queues[core], id);    //store id as queue number

    //Wait to start
    xSemaphoreTake(test_queues[core], portMAX_DELAY);

    //Tests on this core
    TEST_ASSERT(ux_Task_Get_Task_संख्या(task_handles[core]) == (0x0F << (core)));
    TEST_ASSERT(ux_Queue_Get_Queue_संख्या(test_queues[core]) == (0x0F << (core)));
    TEST_ASSERT(ucQueueGetQueueType(test_queues[core]) == BIN_SEM_QUEUE_TYPE);

    //Test on other core
#अगर_अघोषित CONFIG_FREERTOS_UNICORE
    TEST_ASSERT(ux_Task_Get_Task_संख्या(task_handles[!core]) == (0x0F << (!core)));
    TEST_ASSERT(ux_Queue_Get_Queue_संख्या(test_queues[!core]) == (0x0F << (!core)));
    TEST_ASSERT(ucQueueGetQueueType(test_queues[!core]) == BIN_SEM_QUEUE_TYPE);
#पूर्ण_अगर

    xSemaphoreGive(test_queues[core]);      //Signal done
    vTaskDelete(शून्य);
}

TEST_CASE("Test freertos trace facility functions", "[freertos]")
{
    क्रम (पूर्णांक i = 0; i < NO_OF_CORES; i++) {
        test_queues[i] = x_Semaphore_Create_द्विआधारी();   //Create a queue as binary semaphore for each core
        xTaskCreatePinnedToCore(task_test_trace_utilities, "Test Task", 4096, (व्योम *)(0x0F << i), TSK_PRIORITY, &task_handles[i], i);
    }

    vTaskDelay(10);

    //Start the tasks
    क्रम (पूर्णांक i = NO_OF_CORES - 1; i >= 0; i--) {
        xSemaphoreGive(test_queues[i]);
    }

    vTaskDelay(10); //Small delay to ensure semaphores are taken

    //Wait for done
    क्रम (पूर्णांक i = 0; i < NO_OF_CORES; i++) {
        xSemaphoreTake(test_queues[i], portMAX_DELAY);
        vSemaphoreDelete(test_queues[i]);
    }

    vTaskDelay(10);     //Give time for idle task to clean up
}

#घोषणा MAX_TASKS           15
#घोषणा TASKS_TO_CREATE     5

अटल TaskHandle_t created_handles[TASKS_TO_CREATE];
अटल TaskStatus_t *tsk_status_array;

व्योम created_task(व्योम* arg)
{
    जबतक (1) {
        vTaskDelay(100);
    }
}

TEST_CASE("Test freertos uxTaskGetSystemState", "[freertos]")
{
    tsk_status_array = सुस्मृति(MAX_TASKS, माप(TaskStatus_t));
    क्रम (पूर्णांक i = 0; i < TASKS_TO_CREATE; i++) {
        xTaskCreatePinnedToCore(created_task, "Created Task", 1024, शून्य, TSK_PRIORITY, &created_handles[i], 0);
    }

    //Get System states
    पूर्णांक no_of_tasks = uxTaskGetSystemState(tsk_status_array, MAX_TASKS, शून्य);
    TEST_ASSERT((no_of_tasks > 0) && (no_of_tasks <= MAX_TASKS));

    //Check if get system state has got all created tasks
    bool not_found = false;
    क्रम (पूर्णांक i = 0; i < TASKS_TO_CREATE; i++) {
        bool found = false;
        क्रम (पूर्णांक j = 0; j < MAX_TASKS; j++) {
            अगर (tsk_status_array[j].xHandle == created_handles[i]) {
                found = true;
                अवरोध;
            }
        }
        अगर (!found) {
            not_found = true;
            अवरोध;
        }
    }
    TEST_ASSERT(not_found == false);

    //Cleanup
    क्रम (पूर्णांक i = 0; i < TASKS_TO_CREATE; i++) {
        vTaskDelete(created_handles[i]);
    }
    मुक्त(tsk_status_array);
    vTaskDelay(10);
}

#पूर्ण_अगर //CONFIG_FREERTOS_USE_TRACE_FACILITY
