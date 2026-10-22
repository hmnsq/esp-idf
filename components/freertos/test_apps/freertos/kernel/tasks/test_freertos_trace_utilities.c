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
#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/semphr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/event_groups.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "test_utils.h"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FREERTOS_USE_TRACE_FACILITY
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TSK_ਤਰਜੀਹ    (UNITY_FREERTOS_ਤਰਜੀਹ + 1)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NO_ਦਾ_CORES CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BIN_SEM_ਕਤਾਰ_TYPE ਕਤਾਰ_ਕਤਾਰ_TYPE_ਦੁਆਧਾਰੀ_SEMAPHORE     //Expected Queue Type

ਸਥਿਰ ਕਤਾਰ_Handle_t test_queues[NO_ਦਾ_CORES];
ਸਥਿਰ TaskHandle_t task_handles[NO_ਦਾ_CORES];

ਖਾਲੀ task_test_trace_utilities(ਖਾਲੀ *arg)
{
    ਪੂਰਨ core = x_ਪੋਰਟ_Get_Core_ID();
    TaskHandle_t handle = xTaskGetCurrentTaskHandle();
    uint32_t id = (uint32_t)arg;

    v_Task_ਸੈੱਟ_Task_ਸੰਖਿਆ(handle, (UBaseType_t)id);    //cast and store id as task number
    v_ਕਤਾਰ_ਸੈੱਟ_ਕਤਾਰ_ਸੰਖਿਆ(test_queues[core], id);    //store id as queue number

    //Wait to start
    xSemaphoreTake(test_queues[core], ਪੋਰਟ_MAX_DELAY);

    //Tests on this core
    TEST_ASSERT(ux_Task_Get_Task_ਸੰਖਿਆ(task_handles[core]) == (0x0F << (core)));
    TEST_ASSERT(ux_ਕਤਾਰ_Get_ਕਤਾਰ_ਸੰਖਿਆ(test_queues[core]) == (0x0F << (core)));
    TEST_ASSERT(uc_ਕਤਾਰ_Get_ਕਤਾਰ_Type(test_queues[core]) == BIN_SEM_ਕਤਾਰ_TYPE);

    //Test on other core
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CONFIG_FREERTOS_UNICORE
    TEST_ASSERT(ux_Task_Get_Task_ਸੰਖਿਆ(task_handles[!core]) == (0x0F << (!core)));
    TEST_ASSERT(ux_ਕਤਾਰ_Get_ਕਤਾਰ_ਸੰਖਿਆ(test_queues[!core]) == (0x0F << (!core)));
    TEST_ASSERT(uc_ਕਤਾਰ_Get_ਕਤਾਰ_Type(test_queues[!core]) == BIN_SEM_ਕਤਾਰ_TYPE);
#ਜੇ_ਅੰਤ

    xSemaphoreGive(test_queues[core]);      //Signal done
    vTaskDelete(ਨਿਰਾ);
}

TEST_CASE("Test freertos trace facility functions", "[freertos]")
{
    ਲਈ (ਪੂਰਨ i = 0; i < NO_ਦਾ_CORES; i++) {
        test_queues[i] = x_Semaphore_Create_ਦੁਆਧਾਰੀ();   //Create a queue as binary semaphore for each core
        x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(task_test_trace_utilities, "Test Task", 4096, (ਖਾਲੀ *)(0x0F << i), TSK_ਤਰਜੀਹ, &task_handles[i], i);
    }

    vTaskDelay(10);

    //Start the tasks
    ਲਈ (ਪੂਰਨ i = NO_ਦਾ_CORES - 1; i >= 0; i--) {
        xSemaphoreGive(test_queues[i]);
    }

    vTaskDelay(10); //Small delay to ensure semaphores are taken

    //Wait for done
    ਲਈ (ਪੂਰਨ i = 0; i < NO_ਦਾ_CORES; i++) {
        xSemaphoreTake(test_queues[i], ਪੋਰਟ_MAX_DELAY);
        vSemaphoreDelete(test_queues[i]);
    }

    vTaskDelay(10);     //Give time for idle task to clean up
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_TASKS           15
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TASKS_ਮੰਜ਼ਿਲ_CREATE     5

ਸਥਿਰ TaskHandle_t created_handles[TASKS_ਮੰਜ਼ਿਲ_CREATE];
ਸਥਿਰ Task_ਸਥਿਤੀ_t *tsk_ਸਥਿਤੀ_array;

ਖਾਲੀ created_task(ਖਾਲੀ* arg)
{
    ਜਦੋਂ (1) {
        vTaskDelay(100);
    }
}

TEST_CASE("Test freertos uxTaskGetSystemState", "[freertos]")
{
    tsk_ਸਥਿਤੀ_array = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(MAX_TASKS, ਆਕਾਰ(Task_ਸਥਿਤੀ_t));
    ਲਈ (ਪੂਰਨ i = 0; i < TASKS_ਮੰਜ਼ਿਲ_CREATE; i++) {
        x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(created_task, "Created Task", 1024, ਨਿਰਾ, TSK_ਤਰਜੀਹ, &created_handles[i], 0);
    }

    //Get System states
    ਪੂਰਨ no_ਦਾ_tasks = uxTaskGetSystemState(tsk_ਸਥਿਤੀ_array, MAX_TASKS, ਨਿਰਾ);
    TEST_ASSERT((no_ਦਾ_tasks > 0) && (no_ਦਾ_tasks <= MAX_TASKS));

    //Check if get system state has got all created tasks
    bool not_ਮਿਲਿਆ = false;
    ਲਈ (ਪੂਰਨ i = 0; i < TASKS_ਮੰਜ਼ਿਲ_CREATE; i++) {
        bool ਮਿਲਿਆ = false;
        ਲਈ (ਪੂਰਨ j = 0; j < MAX_TASKS; j++) {
            ਜੇ (tsk_ਸਥਿਤੀ_array[j].xHandle == created_handles[i]) {
                ਮਿਲਿਆ = true;
                ਰੋਕੋ;
            }
        }
        ਜੇ (!ਮਿਲਿਆ) {
            not_ਮਿਲਿਆ = true;
            ਰੋਕੋ;
        }
    }
    TEST_ASSERT(not_ਮਿਲਿਆ == false);

    //Cleanup
    ਲਈ (ਪੂਰਨ i = 0; i < TASKS_ਮੰਜ਼ਿਲ_CREATE; i++) {
        vTaskDelete(created_handles[i]);
    }
    ਮੁਕਤ(tsk_ਸਥਿਤੀ_array);
    vTaskDelay(10);
}

#ਜੇ_ਅੰਤ //CONFIG_FREERTOS_USE_TRACE_FACILITY
