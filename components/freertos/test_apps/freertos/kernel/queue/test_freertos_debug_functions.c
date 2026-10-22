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

#അന്തര്ഭാവ "sdkconfig.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ "freertos/FreeRTOS.h"
#അന്തര്ഭാവ "freertos/task.h"
#അന്തര്ഭാവ "freertos/semphr.h"
#അന്തര്ഭാവ "freertos/queue.h"
#അന്തര്ഭാവ "unity.h"
#അന്തര്ഭാവ "test_utils.h"

#എങ്കിൽ (CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE > 0)
#define NO_OF_QUEUES_PER_CORE  ((പൂർണ്ണസംഖ്യ)((CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE - 3)/CONFIG_FREERTOS_സംഖ്യ_OF_CORES))     //Save space for some preallocated tasks
#define NO_OF_QUEUES_TOTAL    (NO_OF_QUEUES_PER_CORE * CONFIG_FREERTOS_സംഖ്യ_OF_CORES)
#define QUEUE_NAME_MAX_LENGTH    30

സ്ഥിതം SemaphoreHandle_t start_sem[CONFIG_FREERTOS_സംഖ്യ_OF_CORES];
സ്ഥിതം SemaphoreHandle_t done_sem = ശൂന്യ;
സ്ഥിതം അക്ഷരം *names[NO_OF_QUEUES_TOTAL];
സ്ഥിതം QueueHandle_t handles[NO_OF_QUEUES_TOTAL];

ശൂന്യം test_queue_registry_task(ശൂന്യം *arg)
{
    പൂർണ്ണസംഖ്യ core = xPortGetCoreID();
    പൂർണ്ണസംഖ്യ offset = core * NO_OF_QUEUES_PER_CORE;
    //Create queues and accompanying queue names
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        handles[i + offset] = xQueueCreate(1, 1);  //Create queues
        names[i + offset] = calloc(QUEUE_NAME_MAX_LENGTH, വലുപ്പം(അക്ഷരം));
        sprintf(names[i + offset], "Queue%d%d", core, i);
    }

    xSemaphoreTake(start_sem[core], portMAX_DELAY);   //Wait for start vQueueAddToRegistry()
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        vQueueAddToRegistry(handles[i + offset], names[i + offset]);    //Register queues to queue registry
    }
    xSemaphoreGive(done_sem);   //Signal that vQueueAddToRegistry() has completed

    vTaskDelay(1);

    xSemaphoreTake(start_sem[core], portMAX_DELAY);   //Wait to start vQueueUnregisterQueue()
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        vQueueDelete(handles[i + offset]);  //Internally calls vQueueUnregisterQueue
    }
    xSemaphoreGive(done_sem);   //Signal done

    vTaskDelete(ശൂന്യ);  //Delete self
}

TEST_CASE("Test FreeRTOS Queue Registry", "[freertos]")
{
    //Create synchronization semaphores and tasks to test queue registry
    done_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_സംഖ്യ_OF_CORES, 0);
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; i++) {
        start_sem[i] = x_Semaphore_Create_ദ്വിമാന();
        xTaskCreatePinnedToCore(test_queue_registry_task, "testing task", 4096, ശൂന്യ, UNITY_FREERTOS_PRIORITY + 1, ശൂന്യ, i);
    }

    portDISABLE_INTERRUPTS();
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; i++) {
        xSemaphoreGive(start_sem[i]);  //Trigger start
    }
    portENABLE_INTERRUPTS();
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);    //Wait for tasks to complete vQueueAddToRegistry
    }
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < NO_OF_QUEUES_TOTAL; i++) {
        സ്ഥിരം അക്ഷരം *addr = pcQueueGetName(handles[i]);
        TEST_ASSERT(addr == names[i]);   //Check vQueueAddToRegistry was successful
    }

    portDISABLE_INTERRUPTS();
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; i++) {
        xSemaphoreGive(start_sem[i]);  //Trigger start
    }
    portENABLE_INTERRUPTS();
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);    //Wait for tasks to complete vQueueUnregisterQueue
    }
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i <  NO_OF_QUEUES_TOTAL; i++) {
        സ്ഥിരം അക്ഷരം *addr = pcQueueGetName(handles[i]);
        TEST_ASSERT(addr == ശൂന്യ);   //Check vQueueUnregisterQueue was successful
        handles[i] = ശൂന്യ;
    }

    //Cleanup
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < NO_OF_QUEUES_TOTAL; i++) {
        സ്വതന്ത്രം(names[i]);
        names[i] = ശൂന്യ;
    }
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; i++) {
        vSemaphoreDelete(start_sem[i]);
        start_sem[i] = ശൂന്യ;
    }
    vSemaphoreDelete(done_sem);
    done_sem = ശൂന്യ;
}
#endif      //(CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE > 0)
