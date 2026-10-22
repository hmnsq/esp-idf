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

#إذا (CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE > 0)
#define NO_OF_QUEUES_PER_CORE  ((صحيح)((CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE - 3)/CONFIG_FREERTOS_العدد_OF_CORES))     //Save space for some preallocated tasks
#define NO_OF_QUEUES_TOTAL    (NO_OF_QUEUES_PER_CORE * CONFIG_FREERTOS_العدد_OF_CORES)
#define QUEUE_NAME_MAX_LENGTH    30

ساكن SemaphoreHandle_t start_sem[CONFIG_FREERTOS_العدد_OF_CORES];
ساكن SemaphoreHandle_t done_sem = عدم;
ساكن حرف *names[NO_OF_QUEUES_TOTAL];
ساكن QueueHandle_t handles[NO_OF_QUEUES_TOTAL];

فراغ test_queue_registry_task(فراغ *arg)
{
    صحيح core = xPortGetCoreID();
    صحيح offset = core * NO_OF_QUEUES_PER_CORE;
    //Create queues and accompanying queue names
    لكل (صحيح i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        handles[i + offset] = xQueueCreate(1, 1);  //Create queues
        names[i + offset] = calloc(QUEUE_NAME_MAX_LENGTH, حجم(حرف));
        sprintf(names[i + offset], "Queue%d%d", core, i);
    }

    xSemaphoreTake(start_sem[core], portMAX_DELAY);   //Wait for start vQueueAddToRegistry()
    لكل (صحيح i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        vQueueAddToRegistry(handles[i + offset], names[i + offset]);    //Register queues to queue registry
    }
    xSemaphoreGive(done_sem);   //Signal that vQueueAddToRegistry() has completed

    vTaskDelay(1);

    xSemaphoreTake(start_sem[core], portMAX_DELAY);   //Wait to start vQueueUnregisterQueue()
    لكل (صحيح i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        vQueueDelete(handles[i + offset]);  //Internally calls vQueueUnregisterQueue
    }
    xSemaphoreGive(done_sem);   //Signal done

    vTaskDelete(عدم);  //Delete self
}

TEST_CASE("Test FreeRTOS Queue Registry", "[freertos]")
{
    //Create synchronization semaphores and tasks to test queue registry
    done_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_العدد_OF_CORES, 0);
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_العدد_OF_CORES; i++) {
        start_sem[i] = x_Semaphore_Create_ثنائي();
        xTaskCreatePinnedToCore(test_queue_registry_task, "testing task", 4096, عدم, UNITY_FREERTOS_PRIORITY + 1, عدم, i);
    }

    portDISABLE_INTERRUPTS();
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_العدد_OF_CORES; i++) {
        xSemaphoreGive(start_sem[i]);  //Trigger start
    }
    portENABLE_INTERRUPTS();
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_العدد_OF_CORES; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);    //Wait for tasks to complete vQueueAddToRegistry
    }
    لكل (صحيح i = 0; i < NO_OF_QUEUES_TOTAL; i++) {
        ثابت حرف *addr = pcQueueGetName(handles[i]);
        TEST_ASSERT(addr == names[i]);   //Check vQueueAddToRegistry was successful
    }

    portDISABLE_INTERRUPTS();
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_العدد_OF_CORES; i++) {
        xSemaphoreGive(start_sem[i]);  //Trigger start
    }
    portENABLE_INTERRUPTS();
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_العدد_OF_CORES; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);    //Wait for tasks to complete vQueueUnregisterQueue
    }
    لكل (صحيح i = 0; i <  NO_OF_QUEUES_TOTAL; i++) {
        ثابت حرف *addr = pcQueueGetName(handles[i]);
        TEST_ASSERT(addr == عدم);   //Check vQueueUnregisterQueue was successful
        handles[i] = عدم;
    }

    //Cleanup
    لكل (صحيح i = 0; i < NO_OF_QUEUES_TOTAL; i++) {
        حرر(names[i]);
        names[i] = عدم;
    }
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_العدد_OF_CORES; i++) {
        vSemaphoreDelete(start_sem[i]);
        start_sem[i] = عدم;
    }
    vSemaphoreDelete(done_sem);
    done_sem = عدم;
}
#endif      //(CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE > 0)
