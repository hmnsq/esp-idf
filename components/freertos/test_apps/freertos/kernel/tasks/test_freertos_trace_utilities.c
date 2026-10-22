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
#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "freertos/semphr.h"
#ಅನ್ತರ್ಭಾವ "freertos/event_groups.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "test_utils.h"

#ifdef CONFIG_FREERTOS_USE_TRACE_FACILITY
#define TSK_PRIORITY    (UNITY_FREERTOS_PRIORITY + 1)

#define NO_OF_CORES CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES
#define BIN_SEM_QUEUE_TYPE queue_QUEUE_TYPE_ದ್ವಿಮಾನ_SEMAPHORE     //Expected Queue Type

ಸ್ಥಿರ QueueHandle_t test_queues[NO_OF_CORES];
ಸ್ಥಿರ TaskHandle_t task_handles[NO_OF_CORES];

ಶೂನ್ಯ task_test_trace_utilities(ಶೂನ್ಯ *arg)
{
    ಪೂರ್ಣಾಙ್ಕ core = xPortGetCoreID();
    TaskHandle_t handle = xTaskGetCurrentTaskHandle();
    uint32_t id = (uint32_t)arg;

    v_Task_Set_Task_ಸಂಖ್ಯೆ(handle, (UBaseType_t)id);    //cast and store id as task number
    v_Queue_Set_Queue_ಸಂಖ್ಯೆ(test_queues[core], id);    //store id as queue number

    //Wait to start
    xSemaphoreTake(test_queues[core], portMAX_DELAY);

    //Tests on this core
    TEST_ASSERT(ux_Task_Get_Task_ಸಂಖ್ಯೆ(task_handles[core]) == (0x0F << (core)));
    TEST_ASSERT(ux_Queue_Get_Queue_ಸಂಖ್ಯೆ(test_queues[core]) == (0x0F << (core)));
    TEST_ASSERT(ucQueueGetQueueType(test_queues[core]) == BIN_SEM_QUEUE_TYPE);

    //Test on other core
#ifndef CONFIG_FREERTOS_UNICORE
    TEST_ASSERT(ux_Task_Get_Task_ಸಂಖ್ಯೆ(task_handles[!core]) == (0x0F << (!core)));
    TEST_ASSERT(ux_Queue_Get_Queue_ಸಂಖ್ಯೆ(test_queues[!core]) == (0x0F << (!core)));
    TEST_ASSERT(ucQueueGetQueueType(test_queues[!core]) == BIN_SEM_QUEUE_TYPE);
#endif

    xSemaphoreGive(test_queues[core]);      //Signal done
    vTaskDelete(ರಿಕ್ತ);
}

TEST_CASE("Test freertos trace facility functions", "[freertos]")
{
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < NO_OF_CORES; i++) {
        test_queues[i] = x_Semaphore_Create_ದ್ವಿಮಾನ();   //Create a queue as binary semaphore for each core
        xTaskCreatePinnedToCore(task_test_trace_utilities, "Test Task", 4096, (ಶೂನ್ಯ *)(0x0F << i), TSK_PRIORITY, &task_handles[i], i);
    }

    vTaskDelay(10);

    //Start the tasks
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = NO_OF_CORES - 1; i >= 0; i--) {
        xSemaphoreGive(test_queues[i]);
    }

    vTaskDelay(10); //Small delay to ensure semaphores are taken

    //Wait for done
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < NO_OF_CORES; i++) {
        xSemaphoreTake(test_queues[i], portMAX_DELAY);
        vSemaphoreDelete(test_queues[i]);
    }

    vTaskDelay(10);     //Give time for idle task to clean up
}

#define MAX_TASKS           15
#define TASKS_TO_CREATE     5

ಸ್ಥಿರ TaskHandle_t created_handles[TASKS_TO_CREATE];
ಸ್ಥಿರ TaskStatus_t *tsk_status_array;

ಶೂನ್ಯ created_task(ಶೂನ್ಯ* arg)
{
    ಯಾವತ್ (1) {
        vTaskDelay(100);
    }
}

TEST_CASE("Test freertos uxTaskGetSystemState", "[freertos]")
{
    tsk_status_array = calloc(MAX_TASKS, ಪರಿಮಾಣ(TaskStatus_t));
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < TASKS_TO_CREATE; i++) {
        xTaskCreatePinnedToCore(created_task, "Created Task", 1024, ರಿಕ್ತ, TSK_PRIORITY, &created_handles[i], 0);
    }

    //Get System states
    ಪೂರ್ಣಾಙ್ಕ no_of_tasks = uxTaskGetSystemState(tsk_status_array, MAX_TASKS, ರಿಕ್ತ);
    TEST_ASSERT((no_of_tasks > 0) && (no_of_tasks <= MAX_TASKS));

    //Check if get system state has got all created tasks
    bool not_found = false;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < TASKS_TO_CREATE; i++) {
        bool found = false;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < MAX_TASKS; j++) {
            ಯದಿ (tsk_status_array[j].xHandle == created_handles[i]) {
                found = true;
                ಭಙ್ಗ;
            }
        }
        ಯದಿ (!found) {
            not_found = true;
            ಭಙ್ಗ;
        }
    }
    TEST_ASSERT(not_found == false);

    //Cleanup
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < TASKS_TO_CREATE; i++) {
        vTaskDelete(created_handles[i]);
    }
    ಮುಕ್ತ(tsk_status_array);
    vTaskDelay(10);
}

#endif //CONFIG_FREERTOS_USE_TRACE_FACILITY
