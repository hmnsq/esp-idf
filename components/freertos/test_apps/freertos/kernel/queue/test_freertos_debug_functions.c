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

#شامل "sdkconfig.h"
#شامل <stdio.h>
#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل "freertos/semphr.h"
#شامل "freertos/queue.h"
#شامل "unity.h"
#شامل "test_utils.h"

#اگر (CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE > 0)
#define NO_OF_QUEUES_PER_CORE  ((صحیح)((CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE - 3)/CONFIG_FREERTOS_عدد_OF_CORES))     //Save space for some preallocated tasks
#define NO_OF_QUEUES_TOTAL    (NO_OF_QUEUES_PER_CORE * CONFIG_FREERTOS_عدد_OF_CORES)
#define QUEUE_NAME_MAX_LENGTH    30

ایستا SemaphoreHandle_t start_sem[CONFIG_FREERTOS_عدد_OF_CORES];
ایستا SemaphoreHandle_t done_sem = پوچ;
ایستا نویسه *names[NO_OF_QUEUES_TOTAL];
ایستا QueueHandle_t handles[NO_OF_QUEUES_TOTAL];

تهی test_queue_registry_task(تهی *arg)
{
    صحیح core = xPortGetCoreID();
    صحیح offset = core * NO_OF_QUEUES_PER_CORE;
    //Create queues and accompanying queue names
    برای (صحیح i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        handles[i + offset] = xQueueCreate(1, 1);  //Create queues
        names[i + offset] = calloc(QUEUE_NAME_MAX_LENGTH, اندازه(نویسه));
        sprintf(names[i + offset], "Queue%d%d", core, i);
    }

    xSemaphoreTake(start_sem[core], portMAX_DELAY);   //Wait for start vQueueAddToRegistry()
    برای (صحیح i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        vQueueAddToRegistry(handles[i + offset], names[i + offset]);    //Register queues to queue registry
    }
    xSemaphoreGive(done_sem);   //Signal that vQueueAddToRegistry() has completed

    vTaskDelay(1);

    xSemaphoreTake(start_sem[core], portMAX_DELAY);   //Wait to start vQueueUnregisterQueue()
    برای (صحیح i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        vQueueDelete(handles[i + offset]);  //Internally calls vQueueUnregisterQueue
    }
    xSemaphoreGive(done_sem);   //Signal done

    vTaskDelete(پوچ);  //Delete self
}

TEST_CASE("Test FreeRTOS Queue Registry", "[freertos]")
{
    //Create synchronization semaphores and tasks to test queue registry
    done_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_عدد_OF_CORES, 0);
    برای (صحیح i = 0; i < CONFIG_FREERTOS_عدد_OF_CORES; i++) {
        start_sem[i] = x_Semaphore_Create_دودویی();
        xTaskCreatePinnedToCore(test_queue_registry_task, "testing task", 4096, پوچ, UNITY_FREERTOS_PRIORITY + 1, پوچ, i);
    }

    portDISABLE_INTERRUPTS();
    برای (صحیح i = 0; i < CONFIG_FREERTOS_عدد_OF_CORES; i++) {
        xSemaphoreGive(start_sem[i]);  //Trigger start
    }
    portENABLE_INTERRUPTS();
    برای (صحیح i = 0; i < CONFIG_FREERTOS_عدد_OF_CORES; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);    //Wait for tasks to complete vQueueAddToRegistry
    }
    برای (صحیح i = 0; i < NO_OF_QUEUES_TOTAL; i++) {
        ثابت نویسه *addr = pcQueueGetName(handles[i]);
        TEST_ASSERT(addr == names[i]);   //Check vQueueAddToRegistry was successful
    }

    portDISABLE_INTERRUPTS();
    برای (صحیح i = 0; i < CONFIG_FREERTOS_عدد_OF_CORES; i++) {
        xSemaphoreGive(start_sem[i]);  //Trigger start
    }
    portENABLE_INTERRUPTS();
    برای (صحیح i = 0; i < CONFIG_FREERTOS_عدد_OF_CORES; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);    //Wait for tasks to complete vQueueUnregisterQueue
    }
    برای (صحیح i = 0; i <  NO_OF_QUEUES_TOTAL; i++) {
        ثابت نویسه *addr = pcQueueGetName(handles[i]);
        TEST_ASSERT(addr == پوچ);   //Check vQueueUnregisterQueue was successful
        handles[i] = پوچ;
    }

    //Cleanup
    برای (صحیح i = 0; i < NO_OF_QUEUES_TOTAL; i++) {
        آزاد(names[i]);
        names[i] = پوچ;
    }
    برای (صحیح i = 0; i < CONFIG_FREERTOS_عدد_OF_CORES; i++) {
        vSemaphoreDelete(start_sem[i]);
        start_sem[i] = پوچ;
    }
    vSemaphoreDelete(done_sem);
    done_sem = پوچ;
}
#endif      //(CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE > 0)
