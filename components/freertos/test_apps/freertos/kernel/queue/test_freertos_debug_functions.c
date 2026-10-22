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

#અન્તર્ભાવ "sdkconfig.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ "freertos/FreeRTOS.h"
#અન્તર્ભાવ "freertos/task.h"
#અન્તર્ભાવ "freertos/semphr.h"
#અન્તર્ભાવ "freertos/queue.h"
#અન્તર્ભાવ "unity.h"
#અન્તર્ભાવ "test_utils.h"

#જો (CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE > 0)
#define NO_OF_QUEUES_PER_CORE  ((પૂર્ણાંક)((CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE - 3)/CONFIG_FREERTOS_સંખ્યા_OF_CORES))     //Save space for some preallocated tasks
#define NO_OF_QUEUES_TOTAL    (NO_OF_QUEUES_PER_CORE * CONFIG_FREERTOS_સંખ્યા_OF_CORES)
#define QUEUE_NAME_MAX_LENGTH    30

સ્થિર SemaphoreHandle_t start_sem[CONFIG_FREERTOS_સંખ્યા_OF_CORES];
સ્થિર SemaphoreHandle_t done_sem = ખાલી;
સ્થિર અક્ષર *names[NO_OF_QUEUES_TOTAL];
સ્થિર QueueHandle_t handles[NO_OF_QUEUES_TOTAL];

શૂન્ય test_queue_registry_task(શૂન્ય *arg)
{
    પૂર્ણાંક core = xPortGetCoreID();
    પૂર્ણાંક offset = core * NO_OF_QUEUES_PER_CORE;
    //Create queues and accompanying queue names
    માટે (પૂર્ણાંક i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        handles[i + offset] = xQueueCreate(1, 1);  //Create queues
        names[i + offset] = calloc(QUEUE_NAME_MAX_LENGTH, કદ(અક્ષર));
        sprintf(names[i + offset], "Queue%d%d", core, i);
    }

    xSemaphoreTake(start_sem[core], portMAX_DELAY);   //Wait for start vQueueAddToRegistry()
    માટે (પૂર્ણાંક i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        vQueueAddToRegistry(handles[i + offset], names[i + offset]);    //Register queues to queue registry
    }
    xSemaphoreGive(done_sem);   //Signal that vQueueAddToRegistry() has completed

    vTaskDelay(1);

    xSemaphoreTake(start_sem[core], portMAX_DELAY);   //Wait to start vQueueUnregisterQueue()
    માટે (પૂર્ણાંક i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        vQueueDelete(handles[i + offset]);  //Internally calls vQueueUnregisterQueue
    }
    xSemaphoreGive(done_sem);   //Signal done

    vTaskDelete(ખાલી);  //Delete self
}

TEST_CASE("Test FreeRTOS Queue Registry", "[freertos]")
{
    //Create synchronization semaphores and tasks to test queue registry
    done_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_સંખ્યા_OF_CORES, 0);
    માટે (પૂર્ણાંક i = 0; i < CONFIG_FREERTOS_સંખ્યા_OF_CORES; i++) {
        start_sem[i] = x_Semaphore_Create_દ્વિઅંકી();
        xTaskCreatePinnedToCore(test_queue_registry_task, "testing task", 4096, ખાલી, UNITY_FREERTOS_PRIORITY + 1, ખાલી, i);
    }

    portDISABLE_INTERRUPTS();
    માટે (પૂર્ણાંક i = 0; i < CONFIG_FREERTOS_સંખ્યા_OF_CORES; i++) {
        xSemaphoreGive(start_sem[i]);  //Trigger start
    }
    portENABLE_INTERRUPTS();
    માટે (પૂર્ણાંક i = 0; i < CONFIG_FREERTOS_સંખ્યા_OF_CORES; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);    //Wait for tasks to complete vQueueAddToRegistry
    }
    માટે (પૂર્ણાંક i = 0; i < NO_OF_QUEUES_TOTAL; i++) {
        અચલ અક્ષર *addr = pcQueueGetName(handles[i]);
        TEST_ASSERT(addr == names[i]);   //Check vQueueAddToRegistry was successful
    }

    portDISABLE_INTERRUPTS();
    માટે (પૂર્ણાંક i = 0; i < CONFIG_FREERTOS_સંખ્યા_OF_CORES; i++) {
        xSemaphoreGive(start_sem[i]);  //Trigger start
    }
    portENABLE_INTERRUPTS();
    માટે (પૂર્ણાંક i = 0; i < CONFIG_FREERTOS_સંખ્યા_OF_CORES; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);    //Wait for tasks to complete vQueueUnregisterQueue
    }
    માટે (પૂર્ણાંક i = 0; i <  NO_OF_QUEUES_TOTAL; i++) {
        અચલ અક્ષર *addr = pcQueueGetName(handles[i]);
        TEST_ASSERT(addr == ખાલી);   //Check vQueueUnregisterQueue was successful
        handles[i] = ખાલી;
    }

    //Cleanup
    માટે (પૂર્ણાંક i = 0; i < NO_OF_QUEUES_TOTAL; i++) {
        મુક્ત(names[i]);
        names[i] = ખાલી;
    }
    માટે (પૂર્ણાંક i = 0; i < CONFIG_FREERTOS_સંખ્યા_OF_CORES; i++) {
        vSemaphoreDelete(start_sem[i]);
        start_sem[i] = ખાલી;
    }
    vSemaphoreDelete(done_sem);
    done_sem = ખાલી;
}
#endif      //(CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE > 0)
