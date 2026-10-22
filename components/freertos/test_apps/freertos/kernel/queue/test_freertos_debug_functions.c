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

#அந்தர்பாவ "sdkconfig.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ "freertos/FreeRTOS.h"
#அந்தர்பாவ "freertos/task.h"
#அந்தர்பாவ "freertos/semphr.h"
#அந்தர்பாவ "freertos/queue.h"
#அந்தர்பாவ "unity.h"
#அந்தர்பாவ "test_utils.h"

#எனில் (CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE > 0)
#define NO_OF_QUEUES_PER_CORE  ((முழு)((CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE - 3)/CONFIG_FREERTOS_எண்_OF_CORES))     //Save space for some preallocated tasks
#define NO_OF_QUEUES_TOTAL    (NO_OF_QUEUES_PER_CORE * CONFIG_FREERTOS_எண்_OF_CORES)
#define QUEUE_NAME_MAX_LENGTH    30

நிலையான SemaphoreHandle_t start_sem[CONFIG_FREERTOS_எண்_OF_CORES];
நிலையான SemaphoreHandle_t done_sem = வெற்று;
நிலையான எழுத்து *names[NO_OF_QUEUES_TOTAL];
நிலையான QueueHandle_t handles[NO_OF_QUEUES_TOTAL];

வெறுமை test_queue_registry_task(வெறுமை *arg)
{
    முழு core = xPortGetCoreID();
    முழு offset = core * NO_OF_QUEUES_PER_CORE;
    //Create queues and accompanying queue names
    க்கு (முழு i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        handles[i + offset] = xQueueCreate(1, 1);  //Create queues
        names[i + offset] = calloc(QUEUE_NAME_MAX_LENGTH, அளவு(எழுத்து));
        sprintf(names[i + offset], "Queue%d%d", core, i);
    }

    xSemaphoreTake(start_sem[core], portMAX_DELAY);   //Wait for start vQueueAddToRegistry()
    க்கு (முழு i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        vQueueAddToRegistry(handles[i + offset], names[i + offset]);    //Register queues to queue registry
    }
    xSemaphoreGive(done_sem);   //Signal that vQueueAddToRegistry() has completed

    vTaskDelay(1);

    xSemaphoreTake(start_sem[core], portMAX_DELAY);   //Wait to start vQueueUnregisterQueue()
    க்கு (முழு i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        vQueueDelete(handles[i + offset]);  //Internally calls vQueueUnregisterQueue
    }
    xSemaphoreGive(done_sem);   //Signal done

    vTaskDelete(வெற்று);  //Delete self
}

TEST_CASE("Test FreeRTOS Queue Registry", "[freertos]")
{
    //Create synchronization semaphores and tasks to test queue registry
    done_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_எண்_OF_CORES, 0);
    க்கு (முழு i = 0; i < CONFIG_FREERTOS_எண்_OF_CORES; i++) {
        start_sem[i] = x_Semaphore_Create_இருமம்();
        xTaskCreatePinnedToCore(test_queue_registry_task, "testing task", 4096, வெற்று, UNITY_FREERTOS_PRIORITY + 1, வெற்று, i);
    }

    portDISABLE_INTERRUPTS();
    க்கு (முழு i = 0; i < CONFIG_FREERTOS_எண்_OF_CORES; i++) {
        xSemaphoreGive(start_sem[i]);  //Trigger start
    }
    portENABLE_INTERRUPTS();
    க்கு (முழு i = 0; i < CONFIG_FREERTOS_எண்_OF_CORES; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);    //Wait for tasks to complete vQueueAddToRegistry
    }
    க்கு (முழு i = 0; i < NO_OF_QUEUES_TOTAL; i++) {
        மாறிலி எழுத்து *addr = pcQueueGetName(handles[i]);
        TEST_ASSERT(addr == names[i]);   //Check vQueueAddToRegistry was successful
    }

    portDISABLE_INTERRUPTS();
    க்கு (முழு i = 0; i < CONFIG_FREERTOS_எண்_OF_CORES; i++) {
        xSemaphoreGive(start_sem[i]);  //Trigger start
    }
    portENABLE_INTERRUPTS();
    க்கு (முழு i = 0; i < CONFIG_FREERTOS_எண்_OF_CORES; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);    //Wait for tasks to complete vQueueUnregisterQueue
    }
    க்கு (முழு i = 0; i <  NO_OF_QUEUES_TOTAL; i++) {
        மாறிலி எழுத்து *addr = pcQueueGetName(handles[i]);
        TEST_ASSERT(addr == வெற்று);   //Check vQueueUnregisterQueue was successful
        handles[i] = வெற்று;
    }

    //Cleanup
    க்கு (முழு i = 0; i < NO_OF_QUEUES_TOTAL; i++) {
        விடுவி(names[i]);
        names[i] = வெற்று;
    }
    க்கு (முழு i = 0; i < CONFIG_FREERTOS_எண்_OF_CORES; i++) {
        vSemaphoreDelete(start_sem[i]);
        start_sem[i] = வெற்று;
    }
    vSemaphoreDelete(done_sem);
    done_sem = வெற்று;
}
#endif      //(CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE > 0)
