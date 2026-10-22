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

#که_تعریف_شوی CONFIG_FREERTOS_USE_TRACE_FACILITY
#تعریف_کړه TSK_لومړیتوب    (UNITY_FREERTOS_لومړیتوب + 1)

#تعریف_کړه NO_د_CORES CONFIG_FREERTOS_شمېره_د_CORES
#تعریف_کړه BIN_SEM_کتار_TYPE کتار_کتار_TYPE_دوګونی_SEMAPHORE     //Expected Queue Type

ساكن کتار_Handle_t test_queues[NO_د_CORES];
ساكن TaskHandle_t task_handles[NO_د_CORES];

فراغ task_test_trace_utilities(فراغ *arg)
{
    صحيح core = x_پورټ_Get_Core_ID();
    TaskHandle_t handle = xTaskGetCurrentTaskHandle();
    uint32_t id = (uint32_t)arg;

    v_Task_سېټ_Task_شمېره(handle, (UBaseType_t)id);    //cast and store id as task number
    v_کتار_سېټ_کتار_شمېره(test_queues[core], id);    //store id as queue number

    //Wait to start
    xSemaphoreTake(test_queues[core], پورټ_MAX_DELAY);

    //Tests on this core
    TEST_ASSERT(ux_Task_Get_Task_شمېره(task_handles[core]) == (0x0F << (core)));
    TEST_ASSERT(ux_کتار_Get_کتار_شمېره(test_queues[core]) == (0x0F << (core)));
    TEST_ASSERT(uc_کتار_Get_کتار_Type(test_queues[core]) == BIN_SEM_کتار_TYPE);

    //Test on other core
#که_نه_تعریف_شوی CONFIG_FREERTOS_UNICORE
    TEST_ASSERT(ux_Task_Get_Task_شمېره(task_handles[!core]) == (0x0F << (!core)));
    TEST_ASSERT(ux_کتار_Get_کتار_شمېره(test_queues[!core]) == (0x0F << (!core)));
    TEST_ASSERT(uc_کتار_Get_کتار_Type(test_queues[!core]) == BIN_SEM_کتار_TYPE);
#د_که_پای

    xSemaphoreGive(test_queues[core]);      //Signal done
    vTaskDelete(عدم);
}

TEST_CASE("Test freertos trace facility functions", "[freertos]")
{
    لكل (صحيح i = 0; i < NO_د_CORES; i++) {
        test_queues[i] = x_Semaphore_Create_دوګونی();   //Create a queue as binary semaphore for each core
        x_Task_Create_Pinned_منزل_Core(task_test_trace_utilities, "Test Task", 4096, (فراغ *)(0x0F << i), TSK_لومړیتوب, &task_handles[i], i);
    }

    vTaskDelay(10);

    //Start the tasks
    لكل (صحيح i = NO_د_CORES - 1; i >= 0; i--) {
        xSemaphoreGive(test_queues[i]);
    }

    vTaskDelay(10); //Small delay to ensure semaphores are taken

    //Wait for done
    لكل (صحيح i = 0; i < NO_د_CORES; i++) {
        xSemaphoreTake(test_queues[i], پورټ_MAX_DELAY);
        vSemaphoreDelete(test_queues[i]);
    }

    vTaskDelay(10);     //Give time for idle task to clean up
}

#تعریف_کړه MAX_TASKS           15
#تعریف_کړه TASKS_منزل_CREATE     5

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
    tsk_حالت_array = حافظه_صفر_ورکړه(MAX_TASKS, حجم(Task_حالت_t));
    لكل (صحيح i = 0; i < TASKS_منزل_CREATE; i++) {
        x_Task_Create_Pinned_منزل_Core(created_task, "Created Task", 1024, عدم, TSK_لومړیتوب, &created_handles[i], 0);
    }

    //Get System states
    صحيح no_د_tasks = uxTaskGetSystemState(tsk_حالت_array, MAX_TASKS, عدم);
    TEST_ASSERT((no_د_tasks > 0) && (no_د_tasks <= MAX_TASKS));

    //Check if get system state has got all created tasks
    bool not_وموندل = false;
    لكل (صحيح i = 0; i < TASKS_منزل_CREATE; i++) {
        bool وموندل = false;
        لكل (صحيح j = 0; j < MAX_TASKS; j++) {
            إذا (tsk_حالت_array[j].xHandle == created_handles[i]) {
                وموندل = true;
                اكسر;
            }
        }
        إذا (!وموندل) {
            not_وموندل = true;
            اكسر;
        }
    }
    TEST_ASSERT(not_وموندل == false);

    //Cleanup
    لكل (صحيح i = 0; i < TASKS_منزل_CREATE; i++) {
        vTaskDelete(created_handles[i]);
    }
    حرر(tsk_حالت_array);
    vTaskDelay(10);
}

#د_که_پای //CONFIG_FREERTOS_USE_TRACE_FACILITY
