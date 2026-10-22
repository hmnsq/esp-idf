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
#અન્તર્ભાવ "sdkconfig.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ "freertos/FreeRTOS.h"
#અન્તર્ભાવ "freertos/task.h"
#અન્તર્ભાવ "freertos/semphr.h"
#અન્તર્ભાવ "freertos/event_groups.h"
#અન્તર્ભાવ "unity.h"
#અન્તર્ભાવ "test_utils.h"

#ifdef CONFIG_FREERTOS_USE_TRACE_FACILITY
#define TSK_PRIORITY    (UNITY_FREERTOS_PRIORITY + 1)

#define NO_OF_CORES CONFIG_FREERTOS_સંખ્યા_OF_CORES
#define BIN_SEM_QUEUE_TYPE queue_QUEUE_TYPE_દ્વિઅંકી_SEMAPHORE     //Expected Queue Type

સ્થિર QueueHandle_t test_queues[NO_OF_CORES];
સ્થિર TaskHandle_t task_handles[NO_OF_CORES];

શૂન્ય task_test_trace_utilities(શૂન્ય *arg)
{
    પૂર્ણાંક core = xPortGetCoreID();
    TaskHandle_t handle = xTaskGetCurrentTaskHandle();
    uint32_t id = (uint32_t)arg;

    v_Task_Set_Task_સંખ્યા(handle, (UBaseType_t)id);    //cast and store id as task number
    v_Queue_Set_Queue_સંખ્યા(test_queues[core], id);    //store id as queue number

    //Wait to start
    xSemaphoreTake(test_queues[core], portMAX_DELAY);

    //Tests on this core
    TEST_ASSERT(ux_Task_Get_Task_સંખ્યા(task_handles[core]) == (0x0F << (core)));
    TEST_ASSERT(ux_Queue_Get_Queue_સંખ્યા(test_queues[core]) == (0x0F << (core)));
    TEST_ASSERT(ucQueueGetQueueType(test_queues[core]) == BIN_SEM_QUEUE_TYPE);

    //Test on other core
#ifndef CONFIG_FREERTOS_UNICORE
    TEST_ASSERT(ux_Task_Get_Task_સંખ્યા(task_handles[!core]) == (0x0F << (!core)));
    TEST_ASSERT(ux_Queue_Get_Queue_સંખ્યા(test_queues[!core]) == (0x0F << (!core)));
    TEST_ASSERT(ucQueueGetQueueType(test_queues[!core]) == BIN_SEM_QUEUE_TYPE);
#endif

    xSemaphoreGive(test_queues[core]);      //Signal done
    vTaskDelete(ખાલી);
}

TEST_CASE("Test freertos trace facility functions", "[freertos]")
{
    માટે (પૂર્ણાંક i = 0; i < NO_OF_CORES; i++) {
        test_queues[i] = x_Semaphore_Create_દ્વિઅંકી();   //Create a queue as binary semaphore for each core
        xTaskCreatePinnedToCore(task_test_trace_utilities, "Test Task", 4096, (શૂન્ય *)(0x0F << i), TSK_PRIORITY, &task_handles[i], i);
    }

    vTaskDelay(10);

    //Start the tasks
    માટે (પૂર્ણાંક i = NO_OF_CORES - 1; i >= 0; i--) {
        xSemaphoreGive(test_queues[i]);
    }

    vTaskDelay(10); //Small delay to ensure semaphores are taken

    //Wait for done
    માટે (પૂર્ણાંક i = 0; i < NO_OF_CORES; i++) {
        xSemaphoreTake(test_queues[i], portMAX_DELAY);
        vSemaphoreDelete(test_queues[i]);
    }

    vTaskDelay(10);     //Give time for idle task to clean up
}

#define MAX_TASKS           15
#define TASKS_TO_CREATE     5

સ્થિર TaskHandle_t created_handles[TASKS_TO_CREATE];
સ્થિર TaskStatus_t *tsk_status_array;

શૂન્ય created_task(શૂન્ય* arg)
{
    જ્યારે (1) {
        vTaskDelay(100);
    }
}

TEST_CASE("Test freertos uxTaskGetSystemState", "[freertos]")
{
    tsk_status_array = calloc(MAX_TASKS, કદ(TaskStatus_t));
    માટે (પૂર્ણાંક i = 0; i < TASKS_TO_CREATE; i++) {
        xTaskCreatePinnedToCore(created_task, "Created Task", 1024, ખાલી, TSK_PRIORITY, &created_handles[i], 0);
    }

    //Get System states
    પૂર્ણાંક no_of_tasks = uxTaskGetSystemState(tsk_status_array, MAX_TASKS, ખાલી);
    TEST_ASSERT((no_of_tasks > 0) && (no_of_tasks <= MAX_TASKS));

    //Check if get system state has got all created tasks
    bool not_found = false;
    માટે (પૂર્ણાંક i = 0; i < TASKS_TO_CREATE; i++) {
        bool found = false;
        માટે (પૂર્ણાંક j = 0; j < MAX_TASKS; j++) {
            જો (tsk_status_array[j].xHandle == created_handles[i]) {
                found = true;
                તોડો;
            }
        }
        જો (!found) {
            not_found = true;
            તોડો;
        }
    }
    TEST_ASSERT(not_found == false);

    //Cleanup
    માટે (પૂર્ણાંક i = 0; i < TASKS_TO_CREATE; i++) {
        vTaskDelete(created_handles[i]);
    }
    મુક્ત(tsk_status_array);
    vTaskDelay(10);
}

#endif //CONFIG_FREERTOS_USE_TRACE_FACILITY
