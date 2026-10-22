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
#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"
#अन्तर्भाव "freertos/semphr.h"
#अन्तर्भाव "freertos/event_groups.h"
#अन्तर्भाव "unity.h"
#अन्तर्भाव "test_utils.h"

#ifdef CONFIG_FREERTOS_USE_TRACE_FACILITY
#define TSK_PRIORITY    (UNITY_FREERTOS_PRIORITY + 1)

#define NO_OF_CORES CONFIG_FREERTOS_सङ्ख्या_OF_CORES
#define BIN_SEM_QUEUE_TYPE queue_QUEUE_TYPE_द्विआधारी_SEMAPHORE     //Expected Queue Type

स्थिर QueueHandle_t test_queues[NO_OF_CORES];
स्थिर TaskHandle_t task_handles[NO_OF_CORES];

शून्य task_test_trace_utilities(शून्य *arg)
{
    पूर्णाङ्क core = xPortGetCoreID();
    TaskHandle_t handle = xTaskGetCurrentTaskHandle();
    uint32_t id = (uint32_t)arg;

    v_Task_Set_Task_सङ्ख्या(handle, (UBaseType_t)id);    //cast and store id as task number
    v_Queue_Set_Queue_सङ्ख्या(test_queues[core], id);    //store id as queue number

    //Wait to start
    xSemaphoreTake(test_queues[core], portMAX_DELAY);

    //Tests on this core
    TEST_ASSERT(ux_Task_Get_Task_सङ्ख्या(task_handles[core]) == (0x0F << (core)));
    TEST_ASSERT(ux_Queue_Get_Queue_सङ्ख्या(test_queues[core]) == (0x0F << (core)));
    TEST_ASSERT(ucQueueGetQueueType(test_queues[core]) == BIN_SEM_QUEUE_TYPE);

    //Test on other core
#ifndef CONFIG_FREERTOS_UNICORE
    TEST_ASSERT(ux_Task_Get_Task_सङ्ख्या(task_handles[!core]) == (0x0F << (!core)));
    TEST_ASSERT(ux_Queue_Get_Queue_सङ्ख्या(test_queues[!core]) == (0x0F << (!core)));
    TEST_ASSERT(ucQueueGetQueueType(test_queues[!core]) == BIN_SEM_QUEUE_TYPE);
#endif

    xSemaphoreGive(test_queues[core]);      //Signal done
    vTaskDelete(रिक्त);
}

TEST_CASE("Test freertos trace facility functions", "[freertos]")
{
    लागि (पूर्णाङ्क i = 0; i < NO_OF_CORES; i++) {
        test_queues[i] = x_Semaphore_Create_द्विआधारी();   //Create a queue as binary semaphore for each core
        xTaskCreatePinnedToCore(task_test_trace_utilities, "Test Task", 4096, (शून्य *)(0x0F << i), TSK_PRIORITY, &task_handles[i], i);
    }

    vTaskDelay(10);

    //Start the tasks
    लागि (पूर्णाङ्क i = NO_OF_CORES - 1; i >= 0; i--) {
        xSemaphoreGive(test_queues[i]);
    }

    vTaskDelay(10); //Small delay to ensure semaphores are taken

    //Wait for done
    लागि (पूर्णाङ्क i = 0; i < NO_OF_CORES; i++) {
        xSemaphoreTake(test_queues[i], portMAX_DELAY);
        vSemaphoreDelete(test_queues[i]);
    }

    vTaskDelay(10);     //Give time for idle task to clean up
}

#define MAX_TASKS           15
#define TASKS_TO_CREATE     5

स्थिर TaskHandle_t created_handles[TASKS_TO_CREATE];
स्थिर TaskStatus_t *tsk_status_array;

शून्य created_task(शून्य* arg)
{
    जबसम्म (1) {
        vTaskDelay(100);
    }
}

TEST_CASE("Test freertos uxTaskGetSystemState", "[freertos]")
{
    tsk_status_array = सुस्मृति(MAX_TASKS, परिमाण(TaskStatus_t));
    लागि (पूर्णाङ्क i = 0; i < TASKS_TO_CREATE; i++) {
        xTaskCreatePinnedToCore(created_task, "Created Task", 1024, रिक्त, TSK_PRIORITY, &created_handles[i], 0);
    }

    //Get System states
    पूर्णाङ्क no_of_tasks = uxTaskGetSystemState(tsk_status_array, MAX_TASKS, रिक्त);
    TEST_ASSERT((no_of_tasks > 0) && (no_of_tasks <= MAX_TASKS));

    //Check if get system state has got all created tasks
    bool not_found = false;
    लागि (पूर्णाङ्क i = 0; i < TASKS_TO_CREATE; i++) {
        bool found = false;
        लागि (पूर्णाङ्क j = 0; j < MAX_TASKS; j++) {
            यदि (tsk_status_array[j].xHandle == created_handles[i]) {
                found = true;
                रोक;
            }
        }
        यदि (!found) {
            not_found = true;
            रोक;
        }
    }
    TEST_ASSERT(not_found == false);

    //Cleanup
    लागि (पूर्णाङ्क i = 0; i < TASKS_TO_CREATE; i++) {
        vTaskDelete(created_handles[i]);
    }
    मुक्त(tsk_status_array);
    vTaskDelay(10);
}

#endif //CONFIG_FREERTOS_USE_TRACE_FACILITY
