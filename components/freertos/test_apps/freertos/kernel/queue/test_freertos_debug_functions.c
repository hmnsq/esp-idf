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

#إذا (CONFIG_FREERTOS_قطار_REGISTRY_سائيز > 0)
#وصف_ڪريو NO_جو_QUEUES_في_CORE  ((صحيح)((CONFIG_FREERTOS_قطار_REGISTRY_سائيز - 3)/CONFIG_FREERTOS_عدد_جو_CORES))     //Save space for some preallocated tasks
#وصف_ڪريو NO_جو_QUEUES_ڪل    (NO_جو_QUEUES_في_CORE * CONFIG_FREERTOS_عدد_جو_CORES)
#وصف_ڪريو قطار_NAME_MAX_ڊيگهائي    30

ساكن SemaphoreHandle_t شروعات_sem[CONFIG_FREERTOS_عدد_جو_CORES];
ساكن SemaphoreHandle_t پورو_sem = عدم;
ساكن حرف *نالا[NO_جو_QUEUES_ڪل];
ساكن قطار_Handle_t handles[NO_جو_QUEUES_ڪل];

فراغ test_قطار_registry_task(فراغ *arg)
{
    صحيح core = x_پورٽ_Get_Core_ID();
    صحيح وٿي = core * NO_جو_QUEUES_في_CORE;
    //Create queues and accompanying queue names
    لكل (صحيح i = 0; i < NO_جو_QUEUES_في_CORE; i++) {
        handles[i + وٿي] = x_قطار_Create(1, 1);  //Create queues
        نالا[i + وٿي] = صفر_يادگيري_ڏيو(قطار_NAME_MAX_ڊيگهائي, حجم(حرف));
        sprintf(نالا[i + وٿي], "Queue%d%d", core, i);
    }

    xSemaphoreTake(شروعات_sem[core], پورٽ_MAX_DELAY);   //Wait for start vQueueAddToRegistry()
    لكل (صحيح i = 0; i < NO_جو_QUEUES_في_CORE; i++) {
        v_قطار_جوڙيو_منزل_Registry(handles[i + وٿي], نالا[i + وٿي]);    //Register queues to queue registry
    }
    xSemaphoreGive(پورو_sem);   //Signal that vQueueAddToRegistry() has completed

    vTaskDelay(1);

    xSemaphoreTake(شروعات_sem[core], پورٽ_MAX_DELAY);   //Wait to start vQueueUnregisterQueue()
    لكل (صحيح i = 0; i < NO_جو_QUEUES_في_CORE; i++) {
        v_قطار_Delete(handles[i + وٿي]);  //Internally calls vQueueUnregisterQueue
    }
    xSemaphoreGive(پورو_sem);   //Signal done

    vTaskDelete(عدم);  //Delete self
}

TEST_CASE("Test FreeRTOS Queue Registry", "[freertos]")
{
    //Create synchronization semaphores and tasks to test queue registry
    پورو_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_عدد_جو_CORES, 0);
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_عدد_جو_CORES; i++) {
        شروعات_sem[i] = x_Semaphore_Create_ٻٽو();
        x_Task_Create_Pinned_منزل_Core(test_قطار_registry_task, "testing task", 4096, عدم, UNITY_FREERTOS_ترجيح + 1, عدم, i);
    }

    پورٽ_DISABLE_INTERRUPTS();
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_عدد_جو_CORES; i++) {
        xSemaphoreGive(شروعات_sem[i]);  //Trigger start
    }
    پورٽ_ENABLE_INTERRUPTS();
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_عدد_جو_CORES; i++) {
        xSemaphoreTake(پورو_sem, پورٽ_MAX_DELAY);    //Wait for tasks to complete vQueueAddToRegistry
    }
    لكل (صحيح i = 0; i < NO_جو_QUEUES_ڪل; i++) {
        ثابت حرف *addr = pc_قطار_Get_Name(handles[i]);
        TEST_ASSERT(addr == نالا[i]);   //Check vQueueAddToRegistry was successful
    }

    پورٽ_DISABLE_INTERRUPTS();
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_عدد_جو_CORES; i++) {
        xSemaphoreGive(شروعات_sem[i]);  //Trigger start
    }
    پورٽ_ENABLE_INTERRUPTS();
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_عدد_جو_CORES; i++) {
        xSemaphoreTake(پورو_sem, پورٽ_MAX_DELAY);    //Wait for tasks to complete vQueueUnregisterQueue
    }
    لكل (صحيح i = 0; i <  NO_جو_QUEUES_ڪل; i++) {
        ثابت حرف *addr = pc_قطار_Get_Name(handles[i]);
        TEST_ASSERT(addr == عدم);   //Check vQueueUnregisterQueue was successful
        handles[i] = عدم;
    }

    //Cleanup
    لكل (صحيح i = 0; i < NO_جو_QUEUES_ڪل; i++) {
        حرر(نالا[i]);
        نالا[i] = عدم;
    }
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_عدد_جو_CORES; i++) {
        vSemaphoreDelete(شروعات_sem[i]);
        شروعات_sem[i] = عدم;
    }
    vSemaphoreDelete(پورو_sem);
    پورو_sem = عدم;
}
#جيڪڏهن_پورو      //(CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE > 0)
