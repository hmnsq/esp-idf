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
#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "freertos/semphr.h"
#అన్తర్భావ "freertos/event_groups.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "test_utils.h"

#నిర్వచితమైతే CONFIG_FREERTOS_USE_TRACE_FACILITY
#నిర్వచించు TSK_ప్రాధాన్యత    (UNITY_FREERTOS_ప్రాధాన్యత + 1)

#నిర్వచించు NO_యొక్క_CORES CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES
#నిర్వచించు BIN_SEM_వరుస_TYPE వరుస_వరుస_TYPE_ద్విమాన_SEMAPHORE     //Expected Queue Type

స్థిర వరుస_Handle_t test_queues[NO_యొక్క_CORES];
స్థిర TaskHandle_t task_handles[NO_యొక్క_CORES];

శూన్య task_test_trace_utilities(శూన్య *arg)
{
    పూర్ణాంకం core = x_పోర్టు_Get_Core_ID();
    TaskHandle_t handle = xTaskGetCurrentTaskHandle();
    uint32_t id = (uint32_t)arg;

    v_Task_సమితి_Task_సంఖ్య(handle, (UBaseType_t)id);    //cast and store id as task number
    v_వరుస_సమితి_వరుస_సంఖ్య(test_queues[core], id);    //store id as queue number

    //Wait to start
    xSemaphoreTake(test_queues[core], పోర్టు_MAX_DELAY);

    //Tests on this core
    TEST_ASSERT(ux_Task_Get_Task_సంఖ్య(task_handles[core]) == (0x0F << (core)));
    TEST_ASSERT(ux_వరుస_Get_వరుస_సంఖ్య(test_queues[core]) == (0x0F << (core)));
    TEST_ASSERT(uc_వరుస_Get_వరుస_Type(test_queues[core]) == BIN_SEM_వరుస_TYPE);

    //Test on other core
#నిర్వచితం_కాకపోతే CONFIG_FREERTOS_UNICORE
    TEST_ASSERT(ux_Task_Get_Task_సంఖ్య(task_handles[!core]) == (0x0F << (!core)));
    TEST_ASSERT(ux_వరుస_Get_వరుస_సంఖ్య(test_queues[!core]) == (0x0F << (!core)));
    TEST_ASSERT(uc_వరుస_Get_వరుస_Type(test_queues[!core]) == BIN_SEM_వరుస_TYPE);
#అయితే_ముగింపు

    xSemaphoreGive(test_queues[core]);      //Signal done
    vTaskDelete(ఖాళీ);
}

TEST_CASE("Test freertos trace facility functions", "[freertos]")
{
    కొరకు (పూర్ణాంకం i = 0; i < NO_యొక్క_CORES; i++) {
        test_queues[i] = x_Semaphore_Create_ద్విమాన();   //Create a queue as binary semaphore for each core
        x_Task_Create_Pinned_గమ్యం_Core(task_test_trace_utilities, "Test Task", 4096, (శూన్య *)(0x0F << i), TSK_ప్రాధాన్యత, &task_handles[i], i);
    }

    vTaskDelay(10);

    //Start the tasks
    కొరకు (పూర్ణాంకం i = NO_యొక్క_CORES - 1; i >= 0; i--) {
        xSemaphoreGive(test_queues[i]);
    }

    vTaskDelay(10); //Small delay to ensure semaphores are taken

    //Wait for done
    కొరకు (పూర్ణాంకం i = 0; i < NO_యొక్క_CORES; i++) {
        xSemaphoreTake(test_queues[i], పోర్టు_MAX_DELAY);
        vSemaphoreDelete(test_queues[i]);
    }

    vTaskDelay(10);     //Give time for idle task to clean up
}

#నిర్వచించు MAX_TASKS           15
#నిర్వచించు TASKS_గమ్యం_CREATE     5

స్థిర TaskHandle_t created_handles[TASKS_గమ్యం_CREATE];
స్థిర Task_స్థితి_t *tsk_స్థితి_array;

శూన్య created_task(శూన్య* arg)
{
    అయితే (1) {
        vTaskDelay(100);
    }
}

TEST_CASE("Test freertos uxTaskGetSystemState", "[freertos]")
{
    tsk_స్థితి_array = సున్నా_జ్ఞాపకం_ఇవ్వు(MAX_TASKS, పరిమాణం(Task_స్థితి_t));
    కొరకు (పూర్ణాంకం i = 0; i < TASKS_గమ్యం_CREATE; i++) {
        x_Task_Create_Pinned_గమ్యం_Core(created_task, "Created Task", 1024, ఖాళీ, TSK_ప్రాధాన్యత, &created_handles[i], 0);
    }

    //Get System states
    పూర్ణాంకం no_యొక్క_tasks = uxTaskGetSystemState(tsk_స్థితి_array, MAX_TASKS, ఖాళీ);
    TEST_ASSERT((no_యొక్క_tasks > 0) && (no_యొక్క_tasks <= MAX_TASKS));

    //Check if get system state has got all created tasks
    bool not_దొరికింది = false;
    కొరకు (పూర్ణాంకం i = 0; i < TASKS_గమ్యం_CREATE; i++) {
        bool దొరికింది = false;
        కొరకు (పూర్ణాంకం j = 0; j < MAX_TASKS; j++) {
            ఒకవేళ (tsk_స్థితి_array[j].xHandle == created_handles[i]) {
                దొరికింది = true;
                ఆపు;
            }
        }
        ఒకవేళ (!దొరికింది) {
            not_దొరికింది = true;
            ఆపు;
        }
    }
    TEST_ASSERT(not_దొరికింది == false);

    //Cleanup
    కొరకు (పూర్ణాంకం i = 0; i < TASKS_గమ్యం_CREATE; i++) {
        vTaskDelete(created_handles[i]);
    }
    విడుదల(tsk_స్థితి_array);
    vTaskDelay(10);
}

#అయితే_ముగింపు //CONFIG_FREERTOS_USE_TRACE_FACILITY
