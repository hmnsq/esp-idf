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

#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"
#अन्तर्भाव "freertos/semphr.h"
#अन्तर्भाव "freertos/queue.h"
#अन्तर्भाव "unity.h"
#अन्तर्भाव "test_utils.h"

#यदि (CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE > 0)
#define NO_OF_QUEUES_PER_CORE  ((पूर्णाङ्क)((CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE - 3)/CONFIG_FREERTOS_संख्या_OF_CORES))     //Save space for some preallocated tasks
#define NO_OF_QUEUES_TOTAL    (NO_OF_QUEUES_PER_CORE * CONFIG_FREERTOS_संख्या_OF_CORES)
#define QUEUE_NAME_MAX_LENGTH    30

स्थिर SemaphoreHandle_t start_sem[CONFIG_FREERTOS_संख्या_OF_CORES];
स्थिर SemaphoreHandle_t done_sem = रिक्त;
स्थिर वर्ण *names[NO_OF_QUEUES_TOTAL];
स्थिर QueueHandle_t handles[NO_OF_QUEUES_TOTAL];

शून्य test_queue_registry_task(शून्य *arg)
{
    पूर्णाङ्क core = xPortGetCoreID();
    पूर्णाङ्क offset = core * NO_OF_QUEUES_PER_CORE;
    //Create queues and accompanying queue names
    प्रति (पूर्णाङ्क i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        handles[i + offset] = xQueueCreate(1, 1);  //Create queues
        names[i + offset] = सुस्मृति(QUEUE_NAME_MAX_LENGTH, परिमाण(वर्ण));
        प्र_लिखो(names[i + offset], "Queue%d%d", core, i);
    }

    xSemaphoreTake(start_sem[core], portMAX_DELAY);   //Wait for start vQueueAddToRegistry()
    प्रति (पूर्णाङ्क i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        vQueueAddToRegistry(handles[i + offset], names[i + offset]);    //Register queues to queue registry
    }
    xSemaphoreGive(done_sem);   //Signal that vQueueAddToRegistry() has completed

    vTaskDelay(1);

    xSemaphoreTake(start_sem[core], portMAX_DELAY);   //Wait to start vQueueUnregisterQueue()
    प्रति (पूर्णाङ्क i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        vQueueDelete(handles[i + offset]);  //Internally calls vQueueUnregisterQueue
    }
    xSemaphoreGive(done_sem);   //Signal done

    vTaskDelete(रिक्त);  //Delete self
}

TEST_CASE("Test FreeRTOS Queue Registry", "[freertos]")
{
    //Create synchronization semaphores and tasks to test queue registry
    done_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_संख्या_OF_CORES, 0);
    प्रति (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_संख्या_OF_CORES; i++) {
        start_sem[i] = x_Semaphore_Create_द्विआधारी();
        xTaskCreatePinnedToCore(test_queue_registry_task, "testing task", 4096, रिक्त, UNITY_FREERTOS_PRIORITY + 1, रिक्त, i);
    }

    portDISABLE_INTERRUPTS();
    प्रति (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_संख्या_OF_CORES; i++) {
        xSemaphoreGive(start_sem[i]);  //Trigger start
    }
    portENABLE_INTERRUPTS();
    प्रति (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_संख्या_OF_CORES; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);    //Wait for tasks to complete vQueueAddToRegistry
    }
    प्रति (पूर्णाङ्क i = 0; i < NO_OF_QUEUES_TOTAL; i++) {
        ध्रुव वर्ण *addr = pcQueueGetName(handles[i]);
        TEST_ASSERT(addr == names[i]);   //Check vQueueAddToRegistry was successful
    }

    portDISABLE_INTERRUPTS();
    प्रति (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_संख्या_OF_CORES; i++) {
        xSemaphoreGive(start_sem[i]);  //Trigger start
    }
    portENABLE_INTERRUPTS();
    प्रति (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_संख्या_OF_CORES; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);    //Wait for tasks to complete vQueueUnregisterQueue
    }
    प्रति (पूर्णाङ्क i = 0; i <  NO_OF_QUEUES_TOTAL; i++) {
        ध्रुव वर्ण *addr = pcQueueGetName(handles[i]);
        TEST_ASSERT(addr == रिक्त);   //Check vQueueUnregisterQueue was successful
        handles[i] = रिक्त;
    }

    //Cleanup
    प्रति (पूर्णाङ्क i = 0; i < NO_OF_QUEUES_TOTAL; i++) {
        मुक्त(names[i]);
        names[i] = रिक्त;
    }
    प्रति (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_संख्या_OF_CORES; i++) {
        vSemaphoreDelete(start_sem[i]);
        start_sem[i] = रिक्त;
    }
    vSemaphoreDelete(done_sem);
    done_sem = रिक्त;
}
#endif      //(CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE > 0)
