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
#شامل "sdkconfig.h"
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <string.h>
#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل "freertos/semphr.h"
#شامل "freertos/event_groups.h"
#شامل "unity.h"
#شامل "test_utils.h"

#اگر_تعریف_شدہ CONFIG_FREERTOS_USE_TRACE_FACILITY
#تعریف_کرو TSK_ترجیح    (UNITY_FREERTOS_ترجیح + 1)

#تعریف_کرو NO_کا_CORES CONFIG_FREERTOS_نمبر_کا_CORES
#تعریف_کرو BIN_SEM_قطار_TYPE قطار_قطار_TYPE_ثنائی_SEMAPHORE     //Expected Queue Type

ساکن قطار_Handle_t test_queues[NO_کا_CORES];
ساکن TaskHandle_t task_handles[NO_کا_CORES];

خالی task_test_trace_utilities(خالی *arg)
{
    صحیح core = x_پورٹ_Get_Core_ID();
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
#اگر_غیر_تعریف_شدہ CONFIG_FREERTOS_UNICORE
    TEST_ASSERT(ux_Task_Get_Task_نمبر(task_handles[!core]) == (0x0F << (!core)));
    TEST_ASSERT(ux_قطار_Get_قطار_نمبر(test_queues[!core]) == (0x0F << (!core)));
    TEST_ASSERT(uc_قطار_Get_قطار_Type(test_queues[!core]) == BIN_SEM_قطار_TYPE);
#اگر_ختم

    xSemaphoreGive(test_queues[core]);      //Signal done
    vTaskDelete(تہی);
}

TEST_CASE("Test freertos trace facility functions", "[freertos]")
{
    برائے (صحیح i = 0; i < NO_کا_CORES; i++) {
        test_queues[i] = x_Semaphore_Create_ثنائی();   //Create a queue as binary semaphore for each core
        x_Task_Create_Pinned_مقصد_Core(task_test_trace_utilities, "Test Task", 4096, (خالی *)(0x0F << i), TSK_ترجیح, &task_handles[i], i);
    }

    vTaskDelay(10);

    //Start the tasks
    برائے (صحیح i = NO_کا_CORES - 1; i >= 0; i--) {
        xSemaphoreGive(test_queues[i]);
    }

    vTaskDelay(10); //Small delay to ensure semaphores are taken

    //Wait for done
    برائے (صحیح i = 0; i < NO_کا_CORES; i++) {
        xSemaphoreTake(test_queues[i], پورٹ_MAX_DELAY);
        vSemaphoreDelete(test_queues[i]);
    }

    vTaskDelay(10);     //Give time for idle task to clean up
}

#تعریف_کرو MAX_TASKS           15
#تعریف_کرو TASKS_مقصد_CREATE     5

ساکن TaskHandle_t created_handles[TASKS_مقصد_CREATE];
ساکن Task_حالت_t *tsk_حالت_array;

خالی created_task(خالی* arg)
{
    جب (1) {
        vTaskDelay(100);
    }
}

TEST_CASE("Test freertos uxTaskGetSystemState", "[freertos]")
{
    tsk_حالت_array = صفر_حافظہ_دو(MAX_TASKS, حجم(Task_حالت_t));
    برائے (صحیح i = 0; i < TASKS_مقصد_CREATE; i++) {
        x_Task_Create_Pinned_مقصد_Core(created_task, "Created Task", 1024, تہی, TSK_ترجیح, &created_handles[i], 0);
    }

    //Get System states
    صحیح no_کا_tasks = uxTaskGetSystemState(tsk_حالت_array, MAX_TASKS, تہی);
    TEST_ASSERT((no_کا_tasks > 0) && (no_کا_tasks <= MAX_TASKS));

    //Check if get system state has got all created tasks
    bool not_ملا = false;
    برائے (صحیح i = 0; i < TASKS_مقصد_CREATE; i++) {
        bool ملا = false;
        برائے (صحیح j = 0; j < MAX_TASKS; j++) {
            اگر (tsk_حالت_array[j].xHandle == created_handles[i]) {
                ملا = true;
                توڑو;
            }
        }
        اگر (!ملا) {
            not_ملا = true;
            توڑو;
        }
    }
    TEST_ASSERT(not_ملا == false);

    //Cleanup
    برائے (صحیح i = 0; i < TASKS_مقصد_CREATE; i++) {
        vTaskDelete(created_handles[i]);
    }
    آزاد(tsk_حالت_array);
    vTaskDelay(10);
}

#اگر_ختم //CONFIG_FREERTOS_USE_TRACE_FACILITY
