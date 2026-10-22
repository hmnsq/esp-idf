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

#إذا (CONFIG_FREERTOS_قطار_REGISTRY_ناپ > 0)
#تعریف_کرو NO_دا_QUEUES_فی_CORE  ((صحيح)((CONFIG_FREERTOS_قطار_REGISTRY_ناپ - 3)/CONFIG_FREERTOS_نمبر_دا_CORES))     //Save space for some preallocated tasks
#تعریف_کرو NO_دا_QUEUES_کل    (NO_دا_QUEUES_فی_CORE * CONFIG_FREERTOS_نمبر_دا_CORES)
#تعریف_کرو قطار_NAME_MAX_لمائی    30

ساكن SemaphoreHandle_t شروع_sem[CONFIG_FREERTOS_نمبر_دا_CORES];
ساكن SemaphoreHandle_t پورا_sem = عدم;
ساكن حرف *ناں[NO_دا_QUEUES_کل];
ساكن قطار_Handle_t handles[NO_دا_QUEUES_کل];

فراغ test_قطار_registry_task(فراغ *arg)
{
    صحيح core = x_پورٹ_Get_Core_ID();
    صحيح وتھ = core * NO_دا_QUEUES_فی_CORE;
    //Create queues and accompanying queue names
    لكل (صحيح i = 0; i < NO_دا_QUEUES_فی_CORE; i++) {
        handles[i + وتھ] = x_قطار_Create(1, 1);  //Create queues
        ناں[i + وتھ] = صفر_حافظہ_دیو(قطار_NAME_MAX_لمائی, حجم(حرف));
        sprintf(ناں[i + وتھ], "Queue%d%d", core, i);
    }

    xSemaphoreTake(شروع_sem[core], پورٹ_MAX_DELAY);   //Wait for start vQueueAddToRegistry()
    لكل (صحيح i = 0; i < NO_دا_QUEUES_فی_CORE; i++) {
        v_قطار_جوڑو_منزل_Registry(handles[i + وتھ], ناں[i + وتھ]);    //Register queues to queue registry
    }
    xSemaphoreGive(پورا_sem);   //Signal that vQueueAddToRegistry() has completed

    vTaskDelay(1);

    xSemaphoreTake(شروع_sem[core], پورٹ_MAX_DELAY);   //Wait to start vQueueUnregisterQueue()
    لكل (صحيح i = 0; i < NO_دا_QUEUES_فی_CORE; i++) {
        v_قطار_Delete(handles[i + وتھ]);  //Internally calls vQueueUnregisterQueue
    }
    xSemaphoreGive(پورا_sem);   //Signal done

    vTaskDelete(عدم);  //Delete self
}

TEST_CASE("Test FreeRTOS Queue Registry", "[freertos]")
{
    //Create synchronization semaphores and tasks to test queue registry
    پورا_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_نمبر_دا_CORES, 0);
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_نمبر_دا_CORES; i++) {
        شروع_sem[i] = x_Semaphore_Create_ثنائی();
        x_Task_Create_Pinned_منزل_Core(test_قطار_registry_task, "testing task", 4096, عدم, UNITY_FREERTOS_ترجیح + 1, عدم, i);
    }

    پورٹ_DISABLE_INTERRUPTS();
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_نمبر_دا_CORES; i++) {
        xSemaphoreGive(شروع_sem[i]);  //Trigger start
    }
    پورٹ_ENABLE_INTERRUPTS();
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_نمبر_دا_CORES; i++) {
        xSemaphoreTake(پورا_sem, پورٹ_MAX_DELAY);    //Wait for tasks to complete vQueueAddToRegistry
    }
    لكل (صحيح i = 0; i < NO_دا_QUEUES_کل; i++) {
        ثابت حرف *addr = pc_قطار_Get_Name(handles[i]);
        TEST_ASSERT(addr == ناں[i]);   //Check vQueueAddToRegistry was successful
    }

    پورٹ_DISABLE_INTERRUPTS();
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_نمبر_دا_CORES; i++) {
        xSemaphoreGive(شروع_sem[i]);  //Trigger start
    }
    پورٹ_ENABLE_INTERRUPTS();
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_نمبر_دا_CORES; i++) {
        xSemaphoreTake(پورا_sem, پورٹ_MAX_DELAY);    //Wait for tasks to complete vQueueUnregisterQueue
    }
    لكل (صحيح i = 0; i <  NO_دا_QUEUES_کل; i++) {
        ثابت حرف *addr = pc_قطار_Get_Name(handles[i]);
        TEST_ASSERT(addr == عدم);   //Check vQueueUnregisterQueue was successful
        handles[i] = عدم;
    }

    //Cleanup
    لكل (صحيح i = 0; i < NO_دا_QUEUES_کل; i++) {
        حرر(ناں[i]);
        ناں[i] = عدم;
    }
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_نمبر_دا_CORES; i++) {
        vSemaphoreDelete(شروع_sem[i]);
        شروع_sem[i] = عدم;
    }
    vSemaphoreDelete(پورا_sem);
    پورا_sem = عدم;
}
#جے_انت      //(CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE > 0)
