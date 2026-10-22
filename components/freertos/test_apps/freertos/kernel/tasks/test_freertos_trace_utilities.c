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

#اگر_تعریف_شده CONFIG_FREERTOS_USE_TRACE_FACILITY
#تعریف_کن TSK_اولویت    (UNITY_FREERTOS_اولویت + 1)

#تعریف_کن NO_از_CORES CONFIG_FREERTOS_عدد_از_CORES
#تعریف_کن BIN_SEM_صف_TYPE صف_صف_TYPE_دودویی_SEMAPHORE     //Expected Queue Type

ساكن صف_Handle_t test_queues[NO_از_CORES];
ساكن TaskHandle_t task_handles[NO_از_CORES];

فراغ task_test_trace_utilities(فراغ *arg)
{
    صحيح core = x_پورت_Get_Core_ID();
    TaskHandle_t handle = xTaskGetCurrentTaskHandle();
    uint32_t id = (uint32_t)arg;

    v_Task_مجموعه_Task_عدد(handle, (UBaseType_t)id);    //cast and store id as task number
    v_صف_مجموعه_صف_عدد(test_queues[core], id);    //store id as queue number

    //Wait to start
    xSemaphoreTake(test_queues[core], پورت_MAX_DELAY);

    //Tests on this core
    TEST_ASSERT(ux_Task_Get_Task_عدد(task_handles[core]) == (0x0F << (core)));
    TEST_ASSERT(ux_صف_Get_صف_عدد(test_queues[core]) == (0x0F << (core)));
    TEST_ASSERT(uc_صف_Get_صف_Type(test_queues[core]) == BIN_SEM_صف_TYPE);

    //Test on other core
#اگر_تعریف_نشده CONFIG_FREERTOS_UNICORE
    TEST_ASSERT(ux_Task_Get_Task_عدد(task_handles[!core]) == (0x0F << (!core)));
    TEST_ASSERT(ux_صف_Get_صف_عدد(test_queues[!core]) == (0x0F << (!core)));
    TEST_ASSERT(uc_صف_Get_صف_Type(test_queues[!core]) == BIN_SEM_صف_TYPE);
#پایان_اگر

    xSemaphoreGive(test_queues[core]);      //Signal done
    vTaskDelete(عدم);
}

TEST_CASE("Test freertos trace facility functions", "[freertos]")
{
    لكل (صحيح i = 0; i < NO_از_CORES; i++) {
        test_queues[i] = x_Semaphore_Create_دودویی();   //Create a queue as binary semaphore for each core
        x_Task_Create_Pinned_مقصد_Core(task_test_trace_utilities, "Test Task", 4096, (فراغ *)(0x0F << i), TSK_اولویت, &task_handles[i], i);
    }

    vTaskDelay(10);

    //Start the tasks
    لكل (صحيح i = NO_از_CORES - 1; i >= 0; i--) {
        xSemaphoreGive(test_queues[i]);
    }

    vTaskDelay(10); //Small delay to ensure semaphores are taken

    //Wait for done
    لكل (صحيح i = 0; i < NO_از_CORES; i++) {
        xSemaphoreTake(test_queues[i], پورت_MAX_DELAY);
        vSemaphoreDelete(test_queues[i]);
    }

    vTaskDelay(10);     //Give time for idle task to clean up
}

#تعریف_کن MAX_TASKS           15
#تعریف_کن TASKS_مقصد_CREATE     5

ساكن TaskHandle_t created_handles[TASKS_مقصد_CREATE];
ساكن Task_وضعیت_t *tsk_وضعیت_array;

فراغ created_task(فراغ* arg)
{
    طالما (1) {
        vTaskDelay(100);
    }
}

TEST_CASE("Test freertos uxTaskGetSystemState", "[freertos]")
{
    tsk_وضعیت_array = حافظه_صفر_بده(MAX_TASKS, حجم(Task_وضعیت_t));
    لكل (صحيح i = 0; i < TASKS_مقصد_CREATE; i++) {
        x_Task_Create_Pinned_مقصد_Core(created_task, "Created Task", 1024, عدم, TSK_اولویت, &created_handles[i], 0);
    }

    //Get System states
    صحيح no_از_tasks = uxTaskGetSystemState(tsk_وضعیت_array, MAX_TASKS, عدم);
    TEST_ASSERT((no_از_tasks > 0) && (no_از_tasks <= MAX_TASKS));

    //Check if get system state has got all created tasks
    bool not_یافت = false;
    لكل (صحيح i = 0; i < TASKS_مقصد_CREATE; i++) {
        bool یافت = false;
        لكل (صحيح j = 0; j < MAX_TASKS; j++) {
            إذا (tsk_وضعیت_array[j].xHandle == created_handles[i]) {
                یافت = true;
                اكسر;
            }
        }
        إذا (!یافت) {
            not_یافت = true;
            اكسر;
        }
    }
    TEST_ASSERT(not_یافت == false);

    //Cleanup
    لكل (صحيح i = 0; i < TASKS_مقصد_CREATE; i++) {
        vTaskDelete(created_handles[i]);
    }
    حرر(tsk_وضعیت_array);
    vTaskDelay(10);
}

#پایان_اگر //CONFIG_FREERTOS_USE_TRACE_FACILITY
