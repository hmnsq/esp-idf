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

#यदि (CONFIG_FREERTOS_QUEUE_REGISTRY_आकार > 0)
#परिभाषय NO_सम्बन्धि_QUEUES_प्रत्येकम्_CORE  ((पूर्णाङ्क)((CONFIG_FREERTOS_QUEUE_REGISTRY_आकार - 3)/CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES))     //Save space for some preallocated tasks
#परिभाषय NO_सम्बन्धि_QUEUES_योग    (NO_सम्बन्धि_QUEUES_प्रत्येकम्_CORE * CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES)
#परिभाषय QUEUE_NAME_MAX_दीर्घता    30

स्थिर SemaphoreHandle_t start_sem[CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES];
स्थिर SemaphoreHandle_t समाप्तम्_sem = रिक्त;
स्थिर वर्ण *नामानि[NO_सम्बन्धि_QUEUES_योग];
स्थिर QueueHandle_t handles[NO_सम्बन्धि_QUEUES_योग];

शून्य test_queue_registry_task(शून्य *arg)
{
    पूर्णाङ्क core = x_पत्तनम्_Get_Core_ID();
    पूर्णाङ्क विस्थापनम् = core * NO_सम्बन्धि_QUEUES_प्रत्येकम्_CORE;
    //Create queues and accompanying queue names
    प्रति (पूर्णाङ्क i = 0; i < NO_सम्बन्धि_QUEUES_प्रत्येकम्_CORE; i++) {
        handles[i + विस्थापनम्] = xQueueCreate(1, 1);  //Create queues
        नामानि[i + विस्थापनम्] = शून्यस्मृतिं_देहि(QUEUE_NAME_MAX_दीर्घता, परिमाण(वर्ण));
        sprintf(नामानि[i + विस्थापनम्], "Queue%d%d", core, i);
    }

    xSemaphoreTake(start_sem[core], पत्तनम्_MAX_DELAY);   //Wait for start vQueueAddToRegistry()
    प्रति (पूर्णाङ्क i = 0; i < NO_सम्बन्धि_QUEUES_प्रत्येकम्_CORE; i++) {
        v_Queue_सङ्कलय_गन्तव्यम्_Registry(handles[i + विस्थापनम्], नामानि[i + विस्थापनम्]);    //Register queues to queue registry
    }
    xSemaphoreGive(समाप्तम्_sem);   //Signal that vQueueAddToRegistry() has completed

    vTaskDelay(1);

    xSemaphoreTake(start_sem[core], पत्तनम्_MAX_DELAY);   //Wait to start vQueueUnregisterQueue()
    प्रति (पूर्णाङ्क i = 0; i < NO_सम्बन्धि_QUEUES_प्रत्येकम्_CORE; i++) {
        vQueueDelete(handles[i + विस्थापनम्]);  //Internally calls vQueueUnregisterQueue
    }
    xSemaphoreGive(समाप्तम्_sem);   //Signal done

    vTaskDelete(रिक्त);  //Delete self
}

TEST_CASE("Test FreeRTOS Queue Registry", "[freertos]")
{
    //Create synchronization semaphores and tasks to test queue registry
    समाप्तम्_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES, 0);
    प्रति (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES; i++) {
        start_sem[i] = x_Semaphore_Create_द्विमान();
        x_Task_Create_Pinned_गन्तव्यम्_Core(test_queue_registry_task, "testing task", 4096, रिक्त, UNITY_FREERTOS_प्राथमिकता + 1, रिक्त, i);
    }

    पत्तनम्_DISABLE_INTERRUPTS();
    प्रति (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES; i++) {
        xSemaphoreGive(start_sem[i]);  //Trigger start
    }
    पत्तनम्_ENABLE_INTERRUPTS();
    प्रति (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES; i++) {
        xSemaphoreTake(समाप्तम्_sem, पत्तनम्_MAX_DELAY);    //Wait for tasks to complete vQueueAddToRegistry
    }
    प्रति (पूर्णाङ्क i = 0; i < NO_सम्बन्धि_QUEUES_योग; i++) {
        ध्रुव वर्ण *addr = pcQueueGetName(handles[i]);
        TEST_ASSERT(addr == नामानि[i]);   //Check vQueueAddToRegistry was successful
    }

    पत्तनम्_DISABLE_INTERRUPTS();
    प्रति (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES; i++) {
        xSemaphoreGive(start_sem[i]);  //Trigger start
    }
    पत्तनम्_ENABLE_INTERRUPTS();
    प्रति (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES; i++) {
        xSemaphoreTake(समाप्तम्_sem, पत्तनम्_MAX_DELAY);    //Wait for tasks to complete vQueueUnregisterQueue
    }
    प्रति (पूर्णाङ्क i = 0; i <  NO_सम्बन्धि_QUEUES_योग; i++) {
        ध्रुव वर्ण *addr = pcQueueGetName(handles[i]);
        TEST_ASSERT(addr == रिक्त);   //Check vQueueUnregisterQueue was successful
        handles[i] = रिक्त;
    }

    //Cleanup
    प्रति (पूर्णाङ्क i = 0; i < NO_सम्बन्धि_QUEUES_योग; i++) {
        मुक्त(नामानि[i]);
        नामानि[i] = रिक्त;
    }
    प्रति (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES; i++) {
        vSemaphoreDelete(start_sem[i]);
        start_sem[i] = रिक्त;
    }
    vSemaphoreDelete(समाप्तम्_sem);
    समाप्तम्_sem = रिक्त;
}
#यदि_अन्तः      //(CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE > 0)
