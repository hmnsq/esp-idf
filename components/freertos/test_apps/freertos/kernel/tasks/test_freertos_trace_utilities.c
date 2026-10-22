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

#جے_تعریف_شدہ CONFIG_FREERTOS_USE_TRACE_FACILITY
#تعریف_کرو TSK_ترجیح    (UNITY_FREERTOS_ترجیح + 1)

#تعریف_کرو NO_دا_CORES CONFIG_FREERTOS_نمبر_دا_CORES
#تعریف_کرو BIN_SEM_قطار_TYPE قطار_قطار_TYPE_ثنائی_SEMAPHORE     //Expected Queue Type

ساكن قطار_Handle_t test_queues[NO_دا_CORES];
ساكن TaskHandle_t task_handles[NO_دا_CORES];

فراغ task_test_trace_utilities(فراغ *arg)
{
    صحيح core = x_پورٹ_Get_Core_ID();
    TaskHandle_t handle = xTaskGetCurrentTaskHandle();
    uint32_t id = (uint32_t)arg;

    v_Task_سیٹ_Task_نمبر(handle, (UBaseType_t)id);    //cast and store id as task number
    v_قطار_سیٹ_قطار_نمبر(test_queues[core], id);    //store id as queue number

    //Wait to start
    xSemaphoreTake(test_queues[core], پورٹ_MAX_DELAY);

    //Tests on this core
    TEST_ASSERT(ux_Task_Get_Task_نمبر(task_handles[core]) == (0x0F << (core)));
    TEST_ASSERT(ux_قطار_Get_قطار_نمبر(test_queues[core]) == (0x0F << (core)));
    TEST_ASSERT(uc_قطار_Get_قطار_Type(test_queues[core]) == BIN_SEM_قطار_TYPE);

    //Test on other core
#جے_بنا_تعریف CONFIG_FREERTOS_UNICORE
    TEST_ASSERT(ux_Task_Get_Task_نمبر(task_handles[!core]) == (0x0F << (!core)));
    TEST_ASSERT(ux_قطار_Get_قطار_نمبر(test_queues[!core]) == (0x0F << (!core)));
    TEST_ASSERT(uc_قطار_Get_قطار_Type(test_queues[!core]) == BIN_SEM_قطار_TYPE);
#جے_انت

    xSemaphoreGive(test_queues[core]);      //Signal done
    vTaskDelete(عدم);
}

TEST_CASE("Test freertos trace facility functions", "[freertos]")
{
    لكل (صحيح i = 0; i < NO_دا_CORES; i++) {
        test_queues[i] = x_Semaphore_Create_ثنائی();   //Create a queue as binary semaphore for each core
        x_Task_Create_Pinned_منزل_Core(task_test_trace_utilities, "Test Task", 4096, (فراغ *)(0x0F << i), TSK_ترجیح, &task_handles[i], i);
    }

    vTaskDelay(10);

    //Start the tasks
    لكل (صحيح i = NO_دا_CORES - 1; i >= 0; i--) {
        xSemaphoreGive(test_queues[i]);
    }

    vTaskDelay(10); //Small delay to ensure semaphores are taken

    //Wait for done
    لكل (صحيح i = 0; i < NO_دا_CORES; i++) {
        xSemaphoreTake(test_queues[i], پورٹ_MAX_DELAY);
        vSemaphoreDelete(test_queues[i]);
    }

    vTaskDelay(10);     //Give time for idle task to clean up
}

#تعریف_کرو MAX_TASKS           15
#تعریف_کرو TASKS_منزل_CREATE     5

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
    tsk_حالت_array = صفر_حافظہ_دیو(MAX_TASKS, حجم(Task_حالت_t));
    لكل (صحيح i = 0; i < TASKS_منزل_CREATE; i++) {
        x_Task_Create_Pinned_منزل_Core(created_task, "Created Task", 1024, عدم, TSK_ترجیح, &created_handles[i], 0);
    }

    //Get System states
    صحيح no_دا_tasks = uxTaskGetSystemState(tsk_حالت_array, MAX_TASKS, عدم);
    TEST_ASSERT((no_دا_tasks > 0) && (no_دا_tasks <= MAX_TASKS));

    //Check if get system state has got all created tasks
    bool not_لبھیا = false;
    لكل (صحيح i = 0; i < TASKS_منزل_CREATE; i++) {
        bool لبھیا = false;
        لكل (صحيح j = 0; j < MAX_TASKS; j++) {
            إذا (tsk_حالت_array[j].xHandle == created_handles[i]) {
                لبھیا = true;
                اكسر;
            }
        }
        إذا (!لبھیا) {
            not_لبھیا = true;
            اكسر;
        }
    }
    TEST_ASSERT(not_لبھیا == false);

    //Cleanup
    لكل (صحيح i = 0; i < TASKS_منزل_CREATE; i++) {
        vTaskDelete(created_handles[i]);
    }
    حرر(tsk_حالت_array);
    vTaskDelay(10);
}

#جے_انت //CONFIG_FREERTOS_USE_TRACE_FACILITY
