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

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "freertos/semphr.h"
#ಅನ್ತರ್ಭಾವ "freertos/queue.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "test_utils.h"

#ಯದಿ (CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE > 0)
#define NO_OF_QUEUES_PER_CORE  ((ಪೂರ್ಣಾಙ್ಕ)((CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE - 3)/CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES))     //Save space for some preallocated tasks
#define NO_OF_QUEUES_TOTAL    (NO_OF_QUEUES_PER_CORE * CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES)
#define QUEUE_NAME_MAX_LENGTH    30

ಸ್ಥಿರ SemaphoreHandle_t start_sem[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES];
ಸ್ಥಿರ SemaphoreHandle_t done_sem = ರಿಕ್ತ;
ಸ್ಥಿರ ವರ್ಣ *names[NO_OF_QUEUES_TOTAL];
ಸ್ಥಿರ QueueHandle_t handles[NO_OF_QUEUES_TOTAL];

ಶೂನ್ಯ test_queue_registry_task(ಶೂನ್ಯ *arg)
{
    ಪೂರ್ಣಾಙ್ಕ core = xPortGetCoreID();
    ಪೂರ್ಣಾಙ್ಕ offset = core * NO_OF_QUEUES_PER_CORE;
    //Create queues and accompanying queue names
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        handles[i + offset] = xQueueCreate(1, 1);  //Create queues
        names[i + offset] = calloc(QUEUE_NAME_MAX_LENGTH, ಪರಿಮಾಣ(ವರ್ಣ));
        sprintf(names[i + offset], "Queue%d%d", core, i);
    }

    xSemaphoreTake(start_sem[core], portMAX_DELAY);   //Wait for start vQueueAddToRegistry()
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        vQueueAddToRegistry(handles[i + offset], names[i + offset]);    //Register queues to queue registry
    }
    xSemaphoreGive(done_sem);   //Signal that vQueueAddToRegistry() has completed

    vTaskDelay(1);

    xSemaphoreTake(start_sem[core], portMAX_DELAY);   //Wait to start vQueueUnregisterQueue()
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        vQueueDelete(handles[i + offset]);  //Internally calls vQueueUnregisterQueue
    }
    xSemaphoreGive(done_sem);   //Signal done

    vTaskDelete(ರಿಕ್ತ);  //Delete self
}

TEST_CASE("Test FreeRTOS Queue Registry", "[freertos]")
{
    //Create synchronization semaphores and tasks to test queue registry
    done_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES, 0);
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES; i++) {
        start_sem[i] = x_Semaphore_Create_ದ್ವಿಮಾನ();
        xTaskCreatePinnedToCore(test_queue_registry_task, "testing task", 4096, ರಿಕ್ತ, UNITY_FREERTOS_PRIORITY + 1, ರಿಕ್ತ, i);
    }

    portDISABLE_INTERRUPTS();
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES; i++) {
        xSemaphoreGive(start_sem[i]);  //Trigger start
    }
    portENABLE_INTERRUPTS();
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);    //Wait for tasks to complete vQueueAddToRegistry
    }
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < NO_OF_QUEUES_TOTAL; i++) {
        ಧ್ರುವ ವರ್ಣ *addr = pcQueueGetName(handles[i]);
        TEST_ASSERT(addr == names[i]);   //Check vQueueAddToRegistry was successful
    }

    portDISABLE_INTERRUPTS();
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES; i++) {
        xSemaphoreGive(start_sem[i]);  //Trigger start
    }
    portENABLE_INTERRUPTS();
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);    //Wait for tasks to complete vQueueUnregisterQueue
    }
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i <  NO_OF_QUEUES_TOTAL; i++) {
        ಧ್ರುವ ವರ್ಣ *addr = pcQueueGetName(handles[i]);
        TEST_ASSERT(addr == ರಿಕ್ತ);   //Check vQueueUnregisterQueue was successful
        handles[i] = ರಿಕ್ತ;
    }

    //Cleanup
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < NO_OF_QUEUES_TOTAL; i++) {
        ಮುಕ್ತ(names[i]);
        names[i] = ರಿಕ್ತ;
    }
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES; i++) {
        vSemaphoreDelete(start_sem[i]);
        start_sem[i] = ರಿಕ್ತ;
    }
    vSemaphoreDelete(done_sem);
    done_sem = ರಿಕ್ತ;
}
#endif      //(CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE > 0)
