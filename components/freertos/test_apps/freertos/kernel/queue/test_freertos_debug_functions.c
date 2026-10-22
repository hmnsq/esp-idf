/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * Test FreeRTOS debug functions and utilities.
 * - Queue registry functions vQueueAddToRegistry(), vQueueUnregisterQueue(),
 *   and pcQueueGetName(backported)
 *
 *
 */

#ضمن "sdkconfig.h"
#ضمن <stdio.h>
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "freertos/semphr.h"
#ضمن "freertos/queue.h"
#ضمن "unity.h"
#ضمن "test_utils.h"

#إذا (CONFIG_FREERTOS_کتار_REGISTRY_اندازه > 0)
#تعریف_کړه NO_د_QUEUES_فی_CORE  ((صحيح)((CONFIG_FREERTOS_کتار_REGISTRY_اندازه - 3)/CONFIG_FREERTOS_شمېره_د_CORES))     //Save space for some preallocated tasks
#تعریف_کړه NO_د_QUEUES_ټول    (NO_د_QUEUES_فی_CORE * CONFIG_FREERTOS_شمېره_د_CORES)
#تعریف_کړه کتار_NAME_MAX_اوږدتیا    30

ساكن SemaphoreHandle_t پیل_sem[CONFIG_FREERTOS_شمېره_د_CORES];
ساكن SemaphoreHandle_t بشپړ_sem = عدم;
ساكن حرف *نومونه[NO_د_QUEUES_ټول];
ساكن کتار_Handle_t handles[NO_د_QUEUES_ټول];

فراغ test_کتار_registry_task(فراغ *arg)
{
    صحيح core = x_پورټ_Get_Core_ID();
    صحيح دننه_واټن = core * NO_د_QUEUES_فی_CORE;
    //Create queues and accompanying queue names
    لكل (صحيح i = 0; i < NO_د_QUEUES_فی_CORE; i++) {
        handles[i + دننه_واټن] = x_کتار_Create(1, 1);  //Create queues
        نومونه[i + دننه_واټن] = حافظه_صفر_ورکړه(کتار_NAME_MAX_اوږدتیا, حجم(حرف));
        sprintf(نومونه[i + دننه_واټن], "Queue%d%d", core, i);
    }

    xSemaphoreTake(پیل_sem[core], پورټ_MAX_DELAY);   //Wait for start vQueueAddToRegistry()
    لكل (صحيح i = 0; i < NO_د_QUEUES_فی_CORE; i++) {
        v_کتار_جمع_کړه_منزل_Registry(handles[i + دننه_واټن], نومونه[i + دننه_واټن]);    //Register queues to queue registry
    }
    xSemaphoreGive(بشپړ_sem);   //Signal that vQueueAddToRegistry() has completed

    vTaskDelay(1);

    xSemaphoreTake(پیل_sem[core], پورټ_MAX_DELAY);   //Wait to start vQueueUnregisterQueue()
    لكل (صحيح i = 0; i < NO_د_QUEUES_فی_CORE; i++) {
        v_کتار_Delete(handles[i + دننه_واټن]);  //Internally calls vQueueUnregisterQueue
    }
    xSemaphoreGive(بشپړ_sem);   //Signal done

    vTaskDelete(عدم);  //Delete self
}

TEST_CASE("Test FreeRTOS Queue Registry", "[freertos]")
{
    //Create synchronization semaphores and tasks to test queue registry
    بشپړ_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_شمېره_د_CORES, 0);
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_شمېره_د_CORES; i++) {
        پیل_sem[i] = x_Semaphore_Create_دوګونی();
        x_Task_Create_Pinned_منزل_Core(test_کتار_registry_task, "testing task", 4096, عدم, UNITY_FREERTOS_لومړیتوب + 1, عدم, i);
    }

    پورټ_DISABLE_INTERRUPTS();
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_شمېره_د_CORES; i++) {
        xSemaphoreGive(پیل_sem[i]);  //Trigger start
    }
    پورټ_ENABLE_INTERRUPTS();
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_شمېره_د_CORES; i++) {
        xSemaphoreTake(بشپړ_sem, پورټ_MAX_DELAY);    //Wait for tasks to complete vQueueAddToRegistry
    }
    لكل (صحيح i = 0; i < NO_د_QUEUES_ټول; i++) {
        ثابت حرف *addr = pc_کتار_Get_Name(handles[i]);
        TEST_ASSERT(addr == نومونه[i]);   //Check vQueueAddToRegistry was successful
    }

    پورټ_DISABLE_INTERRUPTS();
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_شمېره_د_CORES; i++) {
        xSemaphoreGive(پیل_sem[i]);  //Trigger start
    }
    پورټ_ENABLE_INTERRUPTS();
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_شمېره_د_CORES; i++) {
        xSemaphoreTake(بشپړ_sem, پورټ_MAX_DELAY);    //Wait for tasks to complete vQueueUnregisterQueue
    }
    لكل (صحيح i = 0; i <  NO_د_QUEUES_ټول; i++) {
        ثابت حرف *addr = pc_کتار_Get_Name(handles[i]);
        TEST_ASSERT(addr == عدم);   //Check vQueueUnregisterQueue was successful
        handles[i] = عدم;
    }

    //Cleanup
    لكل (صحيح i = 0; i < NO_د_QUEUES_ټول; i++) {
        حرر(نومونه[i]);
        نومونه[i] = عدم;
    }
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_شمېره_د_CORES; i++) {
        vSemaphoreDelete(پیل_sem[i]);
        پیل_sem[i] = عدم;
    }
    vSemaphoreDelete(بشپړ_sem);
    بشپړ_sem = عدم;
}
#د_که_پای      //(CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE > 0)
