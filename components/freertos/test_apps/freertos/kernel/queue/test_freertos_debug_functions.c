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

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/semphr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/queue.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "test_utils.h"

#ਜੇ (CONFIG_FREERTOS_ਕਤਾਰ_REGISTRY_ਮਾਪ > 0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NO_ਦਾ_QUEUES_ਪ੍ਰਤੀ_CORE  ((ਪੂਰਨ)((CONFIG_FREERTOS_ਕਤਾਰ_REGISTRY_ਮਾਪ - 3)/CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES))     //Save space for some preallocated tasks
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NO_ਦਾ_QUEUES_ਕੁੱਲ    (NO_ਦਾ_QUEUES_ਪ੍ਰਤੀ_CORE * CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_NAME_MAX_ਲੰਮਾਈ    30

ਸਥਿਰ SemaphoreHandle_t ਸ਼ੁਰੂ_sem[CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES];
ਸਥਿਰ SemaphoreHandle_t ਪੂਰਾ_sem = ਨਿਰਾ;
ਸਥਿਰ ਅੱਖਰ *ਨਾਂ[NO_ਦਾ_QUEUES_ਕੁੱਲ];
ਸਥਿਰ ਕਤਾਰ_Handle_t handles[NO_ਦਾ_QUEUES_ਕੁੱਲ];

ਖਾਲੀ test_ਕਤਾਰ_registry_task(ਖਾਲੀ *arg)
{
    ਪੂਰਨ core = x_ਪੋਰਟ_Get_Core_ID();
    ਪੂਰਨ ਵਿੱਥ = core * NO_ਦਾ_QUEUES_ਪ੍ਰਤੀ_CORE;
    //Create queues and accompanying queue names
    ਲਈ (ਪੂਰਨ i = 0; i < NO_ਦਾ_QUEUES_ਪ੍ਰਤੀ_CORE; i++) {
        handles[i + ਵਿੱਥ] = x_ਕਤਾਰ_Create(1, 1);  //Create queues
        ਨਾਂ[i + ਵਿੱਥ] = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਕਤਾਰ_NAME_MAX_ਲੰਮਾਈ, ਆਕਾਰ(ਅੱਖਰ));
        sprintf(ਨਾਂ[i + ਵਿੱਥ], "Queue%d%d", core, i);
    }

    xSemaphoreTake(ਸ਼ੁਰੂ_sem[core], ਪੋਰਟ_MAX_DELAY);   //Wait for start vQueueAddToRegistry()
    ਲਈ (ਪੂਰਨ i = 0; i < NO_ਦਾ_QUEUES_ਪ੍ਰਤੀ_CORE; i++) {
        v_ਕਤਾਰ_ਜੋੜੋ_ਮੰਜ਼ਿਲ_Registry(handles[i + ਵਿੱਥ], ਨਾਂ[i + ਵਿੱਥ]);    //Register queues to queue registry
    }
    xSemaphoreGive(ਪੂਰਾ_sem);   //Signal that vQueueAddToRegistry() has completed

    vTaskDelay(1);

    xSemaphoreTake(ਸ਼ੁਰੂ_sem[core], ਪੋਰਟ_MAX_DELAY);   //Wait to start vQueueUnregisterQueue()
    ਲਈ (ਪੂਰਨ i = 0; i < NO_ਦਾ_QUEUES_ਪ੍ਰਤੀ_CORE; i++) {
        v_ਕਤਾਰ_Delete(handles[i + ਵਿੱਥ]);  //Internally calls vQueueUnregisterQueue
    }
    xSemaphoreGive(ਪੂਰਾ_sem);   //Signal done

    vTaskDelete(ਨਿਰਾ);  //Delete self
}

TEST_CASE("Test FreeRTOS Queue Registry", "[freertos]")
{
    //Create synchronization semaphores and tasks to test queue registry
    ਪੂਰਾ_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES, 0);
    ਲਈ (ਪੂਰਨ i = 0; i < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; i++) {
        ਸ਼ੁਰੂ_sem[i] = x_Semaphore_Create_ਦੁਆਧਾਰੀ();
        x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(test_ਕਤਾਰ_registry_task, "testing task", 4096, ਨਿਰਾ, UNITY_FREERTOS_ਤਰਜੀਹ + 1, ਨਿਰਾ, i);
    }

    ਪੋਰਟ_DISABLE_INTERRUPTS();
    ਲਈ (ਪੂਰਨ i = 0; i < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; i++) {
        xSemaphoreGive(ਸ਼ੁਰੂ_sem[i]);  //Trigger start
    }
    ਪੋਰਟ_ENABLE_INTERRUPTS();
    ਲਈ (ਪੂਰਨ i = 0; i < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; i++) {
        xSemaphoreTake(ਪੂਰਾ_sem, ਪੋਰਟ_MAX_DELAY);    //Wait for tasks to complete vQueueAddToRegistry
    }
    ਲਈ (ਪੂਰਨ i = 0; i < NO_ਦਾ_QUEUES_ਕੁੱਲ; i++) {
        ਅਚਲ ਅੱਖਰ *addr = pc_ਕਤਾਰ_Get_Name(handles[i]);
        TEST_ASSERT(addr == ਨਾਂ[i]);   //Check vQueueAddToRegistry was successful
    }

    ਪੋਰਟ_DISABLE_INTERRUPTS();
    ਲਈ (ਪੂਰਨ i = 0; i < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; i++) {
        xSemaphoreGive(ਸ਼ੁਰੂ_sem[i]);  //Trigger start
    }
    ਪੋਰਟ_ENABLE_INTERRUPTS();
    ਲਈ (ਪੂਰਨ i = 0; i < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; i++) {
        xSemaphoreTake(ਪੂਰਾ_sem, ਪੋਰਟ_MAX_DELAY);    //Wait for tasks to complete vQueueUnregisterQueue
    }
    ਲਈ (ਪੂਰਨ i = 0; i <  NO_ਦਾ_QUEUES_ਕੁੱਲ; i++) {
        ਅਚਲ ਅੱਖਰ *addr = pc_ਕਤਾਰ_Get_Name(handles[i]);
        TEST_ASSERT(addr == ਨਿਰਾ);   //Check vQueueUnregisterQueue was successful
        handles[i] = ਨਿਰਾ;
    }

    //Cleanup
    ਲਈ (ਪੂਰਨ i = 0; i < NO_ਦਾ_QUEUES_ਕੁੱਲ; i++) {
        ਮੁਕਤ(ਨਾਂ[i]);
        ਨਾਂ[i] = ਨਿਰਾ;
    }
    ਲਈ (ਪੂਰਨ i = 0; i < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; i++) {
        vSemaphoreDelete(ਸ਼ੁਰੂ_sem[i]);
        ਸ਼ੁਰੂ_sem[i] = ਨਿਰਾ;
    }
    vSemaphoreDelete(ਪੂਰਾ_sem);
    ਪੂਰਾ_sem = ਨਿਰਾ;
}
#ਜੇ_ਅੰਤ      //(CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE > 0)
