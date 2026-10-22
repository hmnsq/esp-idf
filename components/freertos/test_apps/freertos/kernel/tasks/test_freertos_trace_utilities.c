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

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FREERTOS_USE_TRACE_FACILITY
#ವ್ಯಾಖ್ಯಾನಿಸು TSK_ಆದ್ಯತೆ    (UNITY_FREERTOS_ಆದ್ಯತೆ + 1)

#ವ್ಯಾಖ್ಯಾನಿಸು NO_ದ_CORES CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES
#ವ್ಯಾಖ್ಯಾನಿಸು BIN_SEM_ಸರತಿ_TYPE ಸರತಿ_ಸರತಿ_TYPE_ದ್ವಿಮಾನ_SEMAPHORE     //Expected Queue Type

ಸ್ಥಿರ ಸರತಿ_Handle_t test_queues[NO_ದ_CORES];
ಸ್ಥಿರ TaskHandle_t task_handles[NO_ದ_CORES];

ಶೂನ್ಯ task_test_trace_utilities(ಶೂನ್ಯ *arg)
{
    ಪೂರ್ಣಾಂಕ core = x_ಪೋರ್ಟ್_Get_Core_ID();
    TaskHandle_t handle = xTaskGetCurrentTaskHandle();
    uint32_t id = (uint32_t)arg;

    v_Task_ಗಣ_Task_ಸಂಖ್ಯೆ(handle, (UBaseType_t)id);    //cast and store id as task number
    v_ಸರತಿ_ಗಣ_ಸರತಿ_ಸಂಖ್ಯೆ(test_queues[core], id);    //store id as queue number

    //Wait to start
    xSemaphoreTake(test_queues[core], ಪೋರ್ಟ್_MAX_DELAY);

    //Tests on this core
    TEST_ASSERT(ux_Task_Get_Task_ಸಂಖ್ಯೆ(task_handles[core]) == (0x0F << (core)));
    TEST_ASSERT(ux_ಸರತಿ_Get_ಸರತಿ_ಸಂಖ್ಯೆ(test_queues[core]) == (0x0F << (core)));
    TEST_ASSERT(uc_ಸರತಿ_Get_ಸರತಿ_Type(test_queues[core]) == BIN_SEM_ಸರತಿ_TYPE);

    //Test on other core
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CONFIG_FREERTOS_UNICORE
    TEST_ASSERT(ux_Task_Get_Task_ಸಂಖ್ಯೆ(task_handles[!core]) == (0x0F << (!core)));
    TEST_ASSERT(ux_ಸರತಿ_Get_ಸರತಿ_ಸಂಖ್ಯೆ(test_queues[!core]) == (0x0F << (!core)));
    TEST_ASSERT(uc_ಸರತಿ_Get_ಸರತಿ_Type(test_queues[!core]) == BIN_SEM_ಸರತಿ_TYPE);
#ಆದರೆ_ಅಂತ್ಯ

    xSemaphoreGive(test_queues[core]);      //Signal done
    vTaskDelete(ಖಾಲಿ);
}

TEST_CASE("Test freertos trace facility functions", "[freertos]")
{
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < NO_ದ_CORES; i++) {
        test_queues[i] = x_Semaphore_Create_ದ್ವಿಮಾನ();   //Create a queue as binary semaphore for each core
        x_Task_Create_Pinned_ಗಮ್ಯ_Core(task_test_trace_utilities, "Test Task", 4096, (ಶೂನ್ಯ *)(0x0F << i), TSK_ಆದ್ಯತೆ, &task_handles[i], i);
    }

    vTaskDelay(10);

    //Start the tasks
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = NO_ದ_CORES - 1; i >= 0; i--) {
        xSemaphoreGive(test_queues[i]);
    }

    vTaskDelay(10); //Small delay to ensure semaphores are taken

    //Wait for done
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < NO_ದ_CORES; i++) {
        xSemaphoreTake(test_queues[i], ಪೋರ್ಟ್_MAX_DELAY);
        vSemaphoreDelete(test_queues[i]);
    }

    vTaskDelay(10);     //Give time for idle task to clean up
}

#ವ್ಯಾಖ್ಯಾನಿಸು MAX_TASKS           15
#ವ್ಯಾಖ್ಯಾನಿಸು TASKS_ಗಮ್ಯ_CREATE     5

ಸ್ಥಿರ TaskHandle_t created_handles[TASKS_ಗಮ್ಯ_CREATE];
ಸ್ಥಿರ Task_ಸ್ಥಿತಿ_t *tsk_ಸ್ಥಿತಿ_array;

ಶೂನ್ಯ created_task(ಶೂನ್ಯ* arg)
{
    ಆಗ (1) {
        vTaskDelay(100);
    }
}

TEST_CASE("Test freertos uxTaskGetSystemState", "[freertos]")
{
    tsk_ಸ್ಥಿತಿ_array = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(MAX_TASKS, ಗಾತ್ರ(Task_ಸ್ಥಿತಿ_t));
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < TASKS_ಗಮ್ಯ_CREATE; i++) {
        x_Task_Create_Pinned_ಗಮ್ಯ_Core(created_task, "Created Task", 1024, ಖಾಲಿ, TSK_ಆದ್ಯತೆ, &created_handles[i], 0);
    }

    //Get System states
    ಪೂರ್ಣಾಂಕ no_ದ_tasks = uxTaskGetSystemState(tsk_ಸ್ಥಿತಿ_array, MAX_TASKS, ಖಾಲಿ);
    TEST_ASSERT((no_ದ_tasks > 0) && (no_ದ_tasks <= MAX_TASKS));

    //Check if get system state has got all created tasks
    bool not_ಸಿಕ್ಕಿತು = false;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < TASKS_ಗಮ್ಯ_CREATE; i++) {
        bool ಸಿಕ್ಕಿತು = false;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < MAX_TASKS; j++) {
            ಆದರೆ (tsk_ಸ್ಥಿತಿ_array[j].xHandle == created_handles[i]) {
                ಸಿಕ್ಕಿತು = true;
                ನಿಲ್ಲಿಸು;
            }
        }
        ಆದರೆ (!ಸಿಕ್ಕಿತು) {
            not_ಸಿಕ್ಕಿತು = true;
            ನಿಲ್ಲಿಸು;
        }
    }
    TEST_ASSERT(not_ಸಿಕ್ಕಿತು == false);

    //Cleanup
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < TASKS_ಗಮ್ಯ_CREATE; i++) {
        vTaskDelete(created_handles[i]);
    }
    ಬಿಡುಗಡೆ(tsk_ಸ್ಥಿತಿ_array);
    vTaskDelay(10);
}

#ಆದರೆ_ಅಂತ್ಯ //CONFIG_FREERTOS_USE_TRACE_FACILITY
