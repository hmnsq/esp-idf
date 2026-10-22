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

#إذا (CONFIG_FREERTOS_صف_REGISTRY_اندازه > 0)
#تعریف_کن NO_از_QUEUES_در_هر_CORE  ((صحيح)((CONFIG_FREERTOS_صف_REGISTRY_اندازه - 3)/CONFIG_FREERTOS_عدد_از_CORES))     //Save space for some preallocated tasks
#تعریف_کن NO_از_QUEUES_مجموع    (NO_از_QUEUES_در_هر_CORE * CONFIG_FREERTOS_عدد_از_CORES)
#تعریف_کن صف_NAME_MAX_درازی    30

ساكن SemaphoreHandle_t شروع_sem[CONFIG_FREERTOS_عدد_از_CORES];
ساكن SemaphoreHandle_t انجامشده_sem = عدم;
ساكن حرف *نامها[NO_از_QUEUES_مجموع];
ساكن صف_Handle_t handles[NO_از_QUEUES_مجموع];

فراغ test_صف_registry_task(فراغ *arg)
{
    صحيح core = x_پورت_Get_Core_ID();
    صحيح جابه_جایی = core * NO_از_QUEUES_در_هر_CORE;
    //Create queues and accompanying queue names
    لكل (صحيح i = 0; i < NO_از_QUEUES_در_هر_CORE; i++) {
        handles[i + جابه_جایی] = x_صف_Create(1, 1);  //Create queues
        نامها[i + جابه_جایی] = حافظه_صفر_بده(صف_NAME_MAX_درازی, حجم(حرف));
        sprintf(نامها[i + جابه_جایی], "Queue%d%d", core, i);
    }

    xSemaphoreTake(شروع_sem[core], پورت_MAX_DELAY);   //Wait for start vQueueAddToRegistry()
    لكل (صحيح i = 0; i < NO_از_QUEUES_در_هر_CORE; i++) {
        v_صف_جمع_کن_مقصد_Registry(handles[i + جابه_جایی], نامها[i + جابه_جایی]);    //Register queues to queue registry
    }
    xSemaphoreGive(انجامشده_sem);   //Signal that vQueueAddToRegistry() has completed

    vTaskDelay(1);

    xSemaphoreTake(شروع_sem[core], پورت_MAX_DELAY);   //Wait to start vQueueUnregisterQueue()
    لكل (صحيح i = 0; i < NO_از_QUEUES_در_هر_CORE; i++) {
        v_صف_Delete(handles[i + جابه_جایی]);  //Internally calls vQueueUnregisterQueue
    }
    xSemaphoreGive(انجامشده_sem);   //Signal done

    vTaskDelete(عدم);  //Delete self
}

TEST_CASE("Test FreeRTOS Queue Registry", "[freertos]")
{
    //Create synchronization semaphores and tasks to test queue registry
    انجامشده_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_عدد_از_CORES, 0);
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_عدد_از_CORES; i++) {
        شروع_sem[i] = x_Semaphore_Create_دودویی();
        x_Task_Create_Pinned_مقصد_Core(test_صف_registry_task, "testing task", 4096, عدم, UNITY_FREERTOS_اولویت + 1, عدم, i);
    }

    پورت_DISABLE_INTERRUPTS();
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_عدد_از_CORES; i++) {
        xSemaphoreGive(شروع_sem[i]);  //Trigger start
    }
    پورت_ENABLE_INTERRUPTS();
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_عدد_از_CORES; i++) {
        xSemaphoreTake(انجامشده_sem, پورت_MAX_DELAY);    //Wait for tasks to complete vQueueAddToRegistry
    }
    لكل (صحيح i = 0; i < NO_از_QUEUES_مجموع; i++) {
        ثابت حرف *addr = pc_صف_Get_Name(handles[i]);
        TEST_ASSERT(addr == نامها[i]);   //Check vQueueAddToRegistry was successful
    }

    پورت_DISABLE_INTERRUPTS();
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_عدد_از_CORES; i++) {
        xSemaphoreGive(شروع_sem[i]);  //Trigger start
    }
    پورت_ENABLE_INTERRUPTS();
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_عدد_از_CORES; i++) {
        xSemaphoreTake(انجامشده_sem, پورت_MAX_DELAY);    //Wait for tasks to complete vQueueUnregisterQueue
    }
    لكل (صحيح i = 0; i <  NO_از_QUEUES_مجموع; i++) {
        ثابت حرف *addr = pc_صف_Get_Name(handles[i]);
        TEST_ASSERT(addr == عدم);   //Check vQueueUnregisterQueue was successful
        handles[i] = عدم;
    }

    //Cleanup
    لكل (صحيح i = 0; i < NO_از_QUEUES_مجموع; i++) {
        حرر(نامها[i]);
        نامها[i] = عدم;
    }
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_عدد_از_CORES; i++) {
        vSemaphoreDelete(شروع_sem[i]);
        شروع_sem[i] = عدم;
    }
    vSemaphoreDelete(انجامشده_sem);
    انجامشده_sem = عدم;
}
#پایان_اگر      //(CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE > 0)
