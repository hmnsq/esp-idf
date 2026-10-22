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

#यदि_परिभाषितम् CONFIG_FREERTOS_USE_TRACE_FACILITY
#परिभाषय TSK_प्राथमिकता    (UNITY_FREERTOS_प्राथमिकता + 1)

#परिभाषय NO_सम्बन्धि_CORES CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES
#परिभाषय BIN_SEM_QUEUE_TYPE queue_QUEUE_TYPE_द्विमान_SEMAPHORE     //Expected Queue Type

स्थिर QueueHandle_t test_queues[NO_सम्बन्धि_CORES];
स्थिर TaskHandle_t task_handles[NO_सम्बन्धि_CORES];

शून्य task_test_trace_utilities(शून्य *arg)
{
    पूर्णाङ्क core = x_पत्तनम्_Get_Core_ID();
    TaskHandle_t handle = xTaskGetCurrentTaskHandle();
    uint32_t id = (uint32_t)arg;

    v_Task_समुच्चयः_Task_सङ्ख्या(handle, (UBaseType_t)id);    //cast and store id as task number
    v_Queue_समुच्चयः_Queue_सङ्ख्या(test_queues[core], id);    //store id as queue number

    //Wait to start
    xSemaphoreTake(test_queues[core], पत्तनम्_MAX_DELAY);

    //Tests on this core
    TEST_ASSERT(ux_Task_Get_Task_सङ्ख्या(task_handles[core]) == (0x0F << (core)));
    TEST_ASSERT(ux_Queue_Get_Queue_सङ्ख्या(test_queues[core]) == (0x0F << (core)));
    TEST_ASSERT(ucQueueGetQueueType(test_queues[core]) == BIN_SEM_QUEUE_TYPE);

    //Test on other core
#यदि_अपरिभाषितम् CONFIG_FREERTOS_UNICORE
    TEST_ASSERT(ux_Task_Get_Task_सङ्ख्या(task_handles[!core]) == (0x0F << (!core)));
    TEST_ASSERT(ux_Queue_Get_Queue_सङ्ख्या(test_queues[!core]) == (0x0F << (!core)));
    TEST_ASSERT(ucQueueGetQueueType(test_queues[!core]) == BIN_SEM_QUEUE_TYPE);
#यदि_अन्तः

    xSemaphoreGive(test_queues[core]);      //Signal done
    vTaskDelete(रिक्त);
}

TEST_CASE("Test freertos trace facility functions", "[freertos]")
{
    प्रति (पूर्णाङ्क i = 0; i < NO_सम्बन्धि_CORES; i++) {
        test_queues[i] = x_Semaphore_Create_द्विमान();   //Create a queue as binary semaphore for each core
        x_Task_Create_Pinned_गन्तव्यम्_Core(task_test_trace_utilities, "Test Task", 4096, (शून्य *)(0x0F << i), TSK_प्राथमिकता, &task_handles[i], i);
    }

    vTaskDelay(10);

    //Start the tasks
    प्रति (पूर्णाङ्क i = NO_सम्बन्धि_CORES - 1; i >= 0; i--) {
        xSemaphoreGive(test_queues[i]);
    }

    vTaskDelay(10); //Small delay to ensure semaphores are taken

    //Wait for done
    प्रति (पूर्णाङ्क i = 0; i < NO_सम्बन्धि_CORES; i++) {
        xSemaphoreTake(test_queues[i], पत्तनम्_MAX_DELAY);
        vSemaphoreDelete(test_queues[i]);
    }

    vTaskDelay(10);     //Give time for idle task to clean up
}

#परिभाषय MAX_TASKS           15
#परिभाषय TASKS_गन्तव्यम्_CREATE     5

स्थिर TaskHandle_t created_handles[TASKS_गन्तव्यम्_CREATE];
स्थिर Task_स्थितिः_t *tsk_स्थितिः_array;

शून्य created_task(शून्य* arg)
{
    यावत् (1) {
        vTaskDelay(100);
    }
}

TEST_CASE("Test freertos uxTaskGetSystemState", "[freertos]")
{
    tsk_स्थितिः_array = शून्यस्मृतिं_देहि(MAX_TASKS, परिमाण(Task_स्थितिः_t));
    प्रति (पूर्णाङ्क i = 0; i < TASKS_गन्तव्यम्_CREATE; i++) {
        x_Task_Create_Pinned_गन्तव्यम्_Core(created_task, "Created Task", 1024, रिक्त, TSK_प्राथमिकता, &created_handles[i], 0);
    }

    //Get System states
    पूर्णाङ्क no_सम्बन्धि_tasks = uxTaskGetSystemState(tsk_स्थितिः_array, MAX_TASKS, रिक्त);
    TEST_ASSERT((no_सम्बन्धि_tasks > 0) && (no_सम्बन्धि_tasks <= MAX_TASKS));

    //Check if get system state has got all created tasks
    bool not_प्राप्तम् = false;
    प्रति (पूर्णाङ्क i = 0; i < TASKS_गन्तव्यम्_CREATE; i++) {
        bool प्राप्तम् = false;
        प्रति (पूर्णाङ्क j = 0; j < MAX_TASKS; j++) {
            यदि (tsk_स्थितिः_array[j].xHandle == created_handles[i]) {
                प्राप्तम् = true;
                भङ्ग;
            }
        }
        यदि (!प्राप्तम्) {
            not_प्राप्तम् = true;
            भङ्ग;
        }
    }
    TEST_ASSERT(not_प्राप्तम् == false);

    //Cleanup
    प्रति (पूर्णाङ्क i = 0; i < TASKS_गन्तव्यम्_CREATE; i++) {
        vTaskDelete(created_handles[i]);
    }
    मुक्त(tsk_स्थितिः_array);
    vTaskDelay(10);
}

#यदि_अन्तः //CONFIG_FREERTOS_USE_TRACE_FACILITY
