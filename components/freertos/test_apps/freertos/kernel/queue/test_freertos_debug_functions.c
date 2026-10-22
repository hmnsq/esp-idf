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

#অন্তর্ভাব "sdkconfig.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব "freertos/FreeRTOS.h"
#অন্তর্ভাব "freertos/task.h"
#অন্তর্ভাব "freertos/semphr.h"
#অন্তর্ভাব "freertos/queue.h"
#অন্তর্ভাব "unity.h"
#অন্তর্ভাব "test_utils.h"

#যদি (CONFIG_FREERTOS_QUEUE_REGISTRY_আকার > 0)
#define NO_OF_QUEUES_PER_CORE  ((পূর্ণসংখ্যা)((CONFIG_FREERTOS_QUEUE_REGISTRY_আকার - 3)/CONFIG_FREERTOS_সংখ্যা_OF_CORES))     //Save space for some preallocated tasks
#define NO_OF_QUEUES_যোগ    (NO_OF_QUEUES_PER_CORE * CONFIG_FREERTOS_সংখ্যা_OF_CORES)
#define QUEUE_NAME_MAX_দৈর্ঘ্য    30

স্থির SemaphoreHandle_t start_sem[CONFIG_FREERTOS_সংখ্যা_OF_CORES];
স্থির SemaphoreHandle_t done_sem = ফাঁকা;
স্থির অক্ষর *names[NO_OF_QUEUES_যোগ];
স্থির QueueHandle_t handles[NO_OF_QUEUES_যোগ];

শূন্য test_queue_registry_task(শূন্য *arg)
{
    পূর্ণসংখ্যা core = xPortGetCoreID();
    পূর্ণসংখ্যা offset = core * NO_OF_QUEUES_PER_CORE;
    //Create queues and accompanying queue names
    জন্য (পূর্ণসংখ্যা i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        handles[i + offset] = xQueueCreate(1, 1);  //Create queues
        names[i + offset] = calloc(QUEUE_NAME_MAX_দৈর্ঘ্য, আকার(অক্ষর));
        sprintf(names[i + offset], "Queue%d%d", core, i);
    }

    xSemaphoreTake(start_sem[core], portMAX_DELAY);   //Wait for start vQueueAddToRegistry()
    জন্য (পূর্ণসংখ্যা i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        vQueueAddToRegistry(handles[i + offset], names[i + offset]);    //Register queues to queue registry
    }
    xSemaphoreGive(done_sem);   //Signal that vQueueAddToRegistry() has completed

    vTaskDelay(1);

    xSemaphoreTake(start_sem[core], portMAX_DELAY);   //Wait to start vQueueUnregisterQueue()
    জন্য (পূর্ণসংখ্যা i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        vQueueDelete(handles[i + offset]);  //Internally calls vQueueUnregisterQueue
    }
    xSemaphoreGive(done_sem);   //Signal done

    vTaskDelete(ফাঁকা);  //Delete self
}

TEST_CASE("Test FreeRTOS Queue Registry", "[freertos]")
{
    //Create synchronization semaphores and tasks to test queue registry
    done_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_সংখ্যা_OF_CORES, 0);
    জন্য (পূর্ণসংখ্যা i = 0; i < CONFIG_FREERTOS_সংখ্যা_OF_CORES; i++) {
        start_sem[i] = x_Semaphore_Create_দ্বিমিক();
        xTaskCreatePinnedToCore(test_queue_registry_task, "testing task", 4096, ফাঁকা, UNITY_FREERTOS_PRIORITY + 1, ফাঁকা, i);
    }

    portDISABLE_INTERRUPTS();
    জন্য (পূর্ণসংখ্যা i = 0; i < CONFIG_FREERTOS_সংখ্যা_OF_CORES; i++) {
        xSemaphoreGive(start_sem[i]);  //Trigger start
    }
    portENABLE_INTERRUPTS();
    জন্য (পূর্ণসংখ্যা i = 0; i < CONFIG_FREERTOS_সংখ্যা_OF_CORES; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);    //Wait for tasks to complete vQueueAddToRegistry
    }
    জন্য (পূর্ণসংখ্যা i = 0; i < NO_OF_QUEUES_যোগ; i++) {
        ধ্রুবক অক্ষর *addr = pcQueueGetName(handles[i]);
        TEST_ASSERT(addr == names[i]);   //Check vQueueAddToRegistry was successful
    }

    portDISABLE_INTERRUPTS();
    জন্য (পূর্ণসংখ্যা i = 0; i < CONFIG_FREERTOS_সংখ্যা_OF_CORES; i++) {
        xSemaphoreGive(start_sem[i]);  //Trigger start
    }
    portENABLE_INTERRUPTS();
    জন্য (পূর্ণসংখ্যা i = 0; i < CONFIG_FREERTOS_সংখ্যা_OF_CORES; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);    //Wait for tasks to complete vQueueUnregisterQueue
    }
    জন্য (পূর্ণসংখ্যা i = 0; i <  NO_OF_QUEUES_যোগ; i++) {
        ধ্রুবক অক্ষর *addr = pcQueueGetName(handles[i]);
        TEST_ASSERT(addr == ফাঁকা);   //Check vQueueUnregisterQueue was successful
        handles[i] = ফাঁকা;
    }

    //Cleanup
    জন্য (পূর্ণসংখ্যা i = 0; i < NO_OF_QUEUES_যোগ; i++) {
        মুক্ত(names[i]);
        names[i] = ফাঁকা;
    }
    জন্য (পূর্ণসংখ্যা i = 0; i < CONFIG_FREERTOS_সংখ্যা_OF_CORES; i++) {
        vSemaphoreDelete(start_sem[i]);
        start_sem[i] = ফাঁকা;
    }
    vSemaphoreDelete(done_sem);
    done_sem = ফাঁকা;
}
#endif      //(CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE > 0)
