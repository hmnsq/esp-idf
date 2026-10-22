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

#ଅନ୍ତର୍ଭାଵ "sdkconfig.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ "freertos/FreeRTOS.h"
#ଅନ୍ତର୍ଭାଵ "freertos/task.h"
#ଅନ୍ତର୍ଭାଵ "freertos/semphr.h"
#ଅନ୍ତର୍ଭାଵ "freertos/queue.h"
#ଅନ୍ତର୍ଭାଵ "unity.h"
#ଅନ୍ତର୍ଭାଵ "test_utils.h"

#ଯଦି (CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE > 0)
#define NO_OF_QUEUES_PER_CORE  ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)((CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE - 3)/CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES))     //Save space for some preallocated tasks
#define NO_OF_QUEUES_TOTAL    (NO_OF_QUEUES_PER_CORE * CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES)
#define QUEUE_NAME_MAX_LENGTH    30

ସ୍ଥିର SemaphoreHandle_t start_sem[CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES];
ସ୍ଥିର SemaphoreHandle_t done_sem = ଖାଲି;
ସ୍ଥିର ଅକ୍ଷର *names[NO_OF_QUEUES_TOTAL];
ସ୍ଥିର QueueHandle_t handles[NO_OF_QUEUES_TOTAL];

ଶୂନ୍ୟ test_queue_registry_task(ଶୂନ୍ୟ *arg)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା core = xPortGetCoreID();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset = core * NO_OF_QUEUES_PER_CORE;
    //Create queues and accompanying queue names
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        handles[i + offset] = xQueueCreate(1, 1);  //Create queues
        names[i + offset] = calloc(QUEUE_NAME_MAX_LENGTH, ଆକାର(ଅକ୍ଷର));
        sprintf(names[i + offset], "Queue%d%d", core, i);
    }

    xSemaphoreTake(start_sem[core], portMAX_DELAY);   //Wait for start vQueueAddToRegistry()
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        vQueueAddToRegistry(handles[i + offset], names[i + offset]);    //Register queues to queue registry
    }
    xSemaphoreGive(done_sem);   //Signal that vQueueAddToRegistry() has completed

    vTaskDelay(1);

    xSemaphoreTake(start_sem[core], portMAX_DELAY);   //Wait to start vQueueUnregisterQueue()
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        vQueueDelete(handles[i + offset]);  //Internally calls vQueueUnregisterQueue
    }
    xSemaphoreGive(done_sem);   //Signal done

    vTaskDelete(ଖାଲି);  //Delete self
}

TEST_CASE("Test FreeRTOS Queue Registry", "[freertos]")
{
    //Create synchronization semaphores and tasks to test queue registry
    done_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES, 0);
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; i++) {
        start_sem[i] = x_Semaphore_Create_ଦ୍ୱିମିକ();
        xTaskCreatePinnedToCore(test_queue_registry_task, "testing task", 4096, ଖାଲି, UNITY_FREERTOS_PRIORITY + 1, ଖାଲି, i);
    }

    portDISABLE_INTERRUPTS();
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; i++) {
        xSemaphoreGive(start_sem[i]);  //Trigger start
    }
    portENABLE_INTERRUPTS();
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);    //Wait for tasks to complete vQueueAddToRegistry
    }
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < NO_OF_QUEUES_TOTAL; i++) {
        ଅଚଳ ଅକ୍ଷର *addr = pcQueueGetName(handles[i]);
        TEST_ASSERT(addr == names[i]);   //Check vQueueAddToRegistry was successful
    }

    portDISABLE_INTERRUPTS();
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; i++) {
        xSemaphoreGive(start_sem[i]);  //Trigger start
    }
    portENABLE_INTERRUPTS();
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);    //Wait for tasks to complete vQueueUnregisterQueue
    }
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i <  NO_OF_QUEUES_TOTAL; i++) {
        ଅଚଳ ଅକ୍ଷର *addr = pcQueueGetName(handles[i]);
        TEST_ASSERT(addr == ଖାଲି);   //Check vQueueUnregisterQueue was successful
        handles[i] = ଖାଲି;
    }

    //Cleanup
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < NO_OF_QUEUES_TOTAL; i++) {
        ମୁକ୍ତ(names[i]);
        names[i] = ଖାଲି;
    }
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; i++) {
        vSemaphoreDelete(start_sem[i]);
        start_sem[i] = ଖାଲି;
    }
    vSemaphoreDelete(done_sem);
    done_sem = ଖାଲି;
}
#endif      //(CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE > 0)
