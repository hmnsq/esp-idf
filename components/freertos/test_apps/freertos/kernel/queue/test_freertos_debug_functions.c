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

#include "sdkconfig.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "unity.h"
#include "test_utils.h"

#ถ้า (CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE > 0)
#define NO_OF_QUEUES_PER_CORE  ((จำนวนเต็ม)((CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE - 3)/CONFIG_FREERTOS_NUMBER_OF_CORES))     //Save space for some preallocated tasks
#define NO_OF_QUEUES_TOTAL    (NO_OF_QUEUES_PER_CORE * CONFIG_FREERTOS_NUMBER_OF_CORES)
#define QUEUE_NAME_MAX_LENGTH    30

static SemaphoreHandle_t start_sem[CONFIG_FREERTOS_NUMBER_OF_CORES];
static SemaphoreHandle_t done_sem = NULL;
static var_na *names[NO_OF_QUEUES_TOTAL];
static QueueHandle_t handles[NO_OF_QUEUES_TOTAL];

void test_queue_registry_task(void *arg)
{
    จำนวนเต็ม core = xPortGetCoreID();
    จำนวนเต็ม offset = core * NO_OF_QUEUES_PER_CORE;
    //Create queues and accompanying queue names
    สำหรับ (จำนวนเต็ม i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        handles[i + offset] = xQueueCreate(1, 1);  //Create queues
        names[i + offset] = calloc(QUEUE_NAME_MAX_LENGTH, parimaa_na(var_na));
        sprintf(names[i + offset], "Queue%d%d", core, i);
    }

    xSemaphoreTake(start_sem[core], portMAX_DELAY);   //Wait for start vQueueAddToRegistry()
    สำหรับ (จำนวนเต็ม i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        vQueueAddToRegistry(handles[i + offset], names[i + offset]);    //Register queues to queue registry
    }
    xSemaphoreGive(done_sem);   //Signal that vQueueAddToRegistry() has completed

    vTaskDelay(1);

    xSemaphoreTake(start_sem[core], portMAX_DELAY);   //Wait to start vQueueUnregisterQueue()
    สำหรับ (จำนวนเต็ม i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        vQueueDelete(handles[i + offset]);  //Internally calls vQueueUnregisterQueue
    }
    xSemaphoreGive(done_sem);   //Signal done

    vTaskDelete(NULL);  //Delete self
}

TEST_CASE("Test FreeRTOS Queue Registry", "[freertos]")
{
    //Create synchronization semaphores and tasks to test queue registry
    done_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_NUMBER_OF_CORES, 0);
    สำหรับ (จำนวนเต็ม i = 0; i < CONFIG_FREERTOS_NUMBER_OF_CORES; i++) {
        start_sem[i] = xSemaphoreCreateBinary();
        xTaskCreatePinnedToCore(test_queue_registry_task, "testing task", 4096, NULL, UNITY_FREERTOS_PRIORITY + 1, NULL, i);
    }

    portDISABLE_INTERRUPTS();
    สำหรับ (จำนวนเต็ม i = 0; i < CONFIG_FREERTOS_NUMBER_OF_CORES; i++) {
        xSemaphoreGive(start_sem[i]);  //Trigger start
    }
    portENABLE_INTERRUPTS();
    สำหรับ (จำนวนเต็ม i = 0; i < CONFIG_FREERTOS_NUMBER_OF_CORES; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);    //Wait for tasks to complete vQueueAddToRegistry
    }
    สำหรับ (จำนวนเต็ม i = 0; i < NO_OF_QUEUES_TOTAL; i++) {
        const var_na *addr = pcQueueGetName(handles[i]);
        TEST_ASSERT(addr == names[i]);   //Check vQueueAddToRegistry was successful
    }

    portDISABLE_INTERRUPTS();
    สำหรับ (จำนวนเต็ม i = 0; i < CONFIG_FREERTOS_NUMBER_OF_CORES; i++) {
        xSemaphoreGive(start_sem[i]);  //Trigger start
    }
    portENABLE_INTERRUPTS();
    สำหรับ (จำนวนเต็ม i = 0; i < CONFIG_FREERTOS_NUMBER_OF_CORES; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);    //Wait for tasks to complete vQueueUnregisterQueue
    }
    สำหรับ (จำนวนเต็ม i = 0; i <  NO_OF_QUEUES_TOTAL; i++) {
        const var_na *addr = pcQueueGetName(handles[i]);
        TEST_ASSERT(addr == NULL);   //Check vQueueUnregisterQueue was successful
        handles[i] = NULL;
    }

    //Cleanup
    สำหรับ (จำนวนเต็ม i = 0; i < NO_OF_QUEUES_TOTAL; i++) {
        free(names[i]);
        names[i] = NULL;
    }
    สำหรับ (จำนวนเต็ม i = 0; i < CONFIG_FREERTOS_NUMBER_OF_CORES; i++) {
        vSemaphoreDelete(start_sem[i]);
        start_sem[i] = NULL;
    }
    vSemaphoreDelete(done_sem);
    done_sem = NULL;
}
#endif      //(CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE > 0)
