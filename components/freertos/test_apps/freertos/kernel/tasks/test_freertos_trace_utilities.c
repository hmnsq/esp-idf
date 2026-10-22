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
#ضمن "sdkconfig.h"
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <string.h>
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "freertos/semphr.h"
#ضمن "freertos/event_groups.h"
#ضمن "unity.h"
#ضمن "test_utils.h"

#جيڪڏهن_وصف_ٿيل CONFIG_FREERTOS_USE_TRACE_FACILITY
#وصف_ڪريو TSK_ترجيح    (UNITY_FREERTOS_ترجيح + 1)

#وصف_ڪريو NO_جو_CORES CONFIG_FREERTOS_عدد_جو_CORES
#وصف_ڪريو BIN_SEM_قطار_TYPE قطار_قطار_TYPE_ٻٽو_SEMAPHORE     //Expected Queue Type

ساكن قطار_Handle_t test_queues[NO_جو_CORES];
ساكن TaskHandle_t task_handles[NO_جو_CORES];

فراغ task_test_trace_utilities(فراغ *arg)
{
    صحيح core = x_پورٽ_Get_Core_ID();
    TaskHandle_t handle = xTaskGetCurrentTaskHandle();
    uint32_t id = (uint32_t)arg;

    v_Task_سيٽ_Task_عدد(handle, (UBaseType_t)id);    //cast and store id as task number
    v_قطار_سيٽ_قطار_عدد(test_queues[core], id);    //store id as queue number

    //Wait to start
    xSemaphoreTake(test_queues[core], پورٽ_MAX_DELAY);

    //Tests on this core
    TEST_ASSERT(ux_Task_Get_Task_عدد(task_handles[core]) == (0x0F << (core)));
    TEST_ASSERT(ux_قطار_Get_قطار_عدد(test_queues[core]) == (0x0F << (core)));
    TEST_ASSERT(uc_قطار_Get_قطار_Type(test_queues[core]) == BIN_SEM_قطار_TYPE);

    //Test on other core
#جيڪڏهن_وصف_نه_ٿيل CONFIG_FREERTOS_UNICORE
    TEST_ASSERT(ux_Task_Get_Task_عدد(task_handles[!core]) == (0x0F << (!core)));
    TEST_ASSERT(ux_قطار_Get_قطار_عدد(test_queues[!core]) == (0x0F << (!core)));
    TEST_ASSERT(uc_قطار_Get_قطار_Type(test_queues[!core]) == BIN_SEM_قطار_TYPE);
#جيڪڏهن_پورو

    xSemaphoreGive(test_queues[core]);      //Signal done
    vTaskDelete(عدم);
}

TEST_CASE("Test freertos trace facility functions", "[freertos]")
{
    لكل (صحيح i = 0; i < NO_جو_CORES; i++) {
        test_queues[i] = x_Semaphore_Create_ٻٽو();   //Create a queue as binary semaphore for each core
        x_Task_Create_Pinned_منزل_Core(task_test_trace_utilities, "Test Task", 4096, (فراغ *)(0x0F << i), TSK_ترجيح, &task_handles[i], i);
    }

    vTaskDelay(10);

    //Start the tasks
    لكل (صحيح i = NO_جو_CORES - 1; i >= 0; i--) {
        xSemaphoreGive(test_queues[i]);
    }

    vTaskDelay(10); //Small delay to ensure semaphores are taken

    //Wait for done
    لكل (صحيح i = 0; i < NO_جو_CORES; i++) {
        xSemaphoreTake(test_queues[i], پورٽ_MAX_DELAY);
        vSemaphoreDelete(test_queues[i]);
    }

    vTaskDelay(10);     //Give time for idle task to clean up
}

#وصف_ڪريو MAX_TASKS           15
#وصف_ڪريو TASKS_منزل_CREATE     5

ساكن TaskHandle_t created_handles[TASKS_منزل_CREATE];
ساكن Task_حالت_t *tsk_حالت_array;

فراغ created_task(فراغ* arg)
{
    طالما (1) {
        vTaskDelay(100);
    }
}

TEST_CASE("Test freertos uxTaskGetSystemState", "[freertos]")
{
    tsk_حالت_array = صفر_يادگيري_ڏيو(MAX_TASKS, حجم(Task_حالت_t));
    لكل (صحيح i = 0; i < TASKS_منزل_CREATE; i++) {
        x_Task_Create_Pinned_منزل_Core(created_task, "Created Task", 1024, عدم, TSK_ترجيح, &created_handles[i], 0);
    }

    //Get System states
    صحيح no_جو_tasks = uxTaskGetSystemState(tsk_حالت_array, MAX_TASKS, عدم);
    TEST_ASSERT((no_جو_tasks > 0) && (no_جو_tasks <= MAX_TASKS));

    //Check if get system state has got all created tasks
    bool not_مليو = false;
    لكل (صحيح i = 0; i < TASKS_منزل_CREATE; i++) {
        bool مليو = false;
        لكل (صحيح j = 0; j < MAX_TASKS; j++) {
            إذا (tsk_حالت_array[j].xHandle == created_handles[i]) {
                مليو = true;
                اكسر;
            }
        }
        إذا (!مليو) {
            not_مليو = true;
            اكسر;
        }
    }
    TEST_ASSERT(not_مليو == false);

    //Cleanup
    لكل (صحيح i = 0; i < TASKS_منزل_CREATE; i++) {
        vTaskDelete(created_handles[i]);
    }
    حرر(tsk_حالت_array);
    vTaskDelay(10);
}

#جيڪڏهن_پورو //CONFIG_FREERTOS_USE_TRACE_FACILITY
