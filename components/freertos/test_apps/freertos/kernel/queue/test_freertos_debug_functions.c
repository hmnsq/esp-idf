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

#समावेश "sdkconfig.h"
#समावेश <stdio.h>
#समावेश "freertos/FreeRTOS.h"
#समावेश "freertos/task.h"
#समावेश "freertos/semphr.h"
#समावेश "freertos/queue.h"
#समावेश "unity.h"
#समावेश "test_utils.h"

#अगर (CONFIG_FREERTOS_QUEUE_REGISTRY_आकार > 0)
#घोषणा NO_OF_QUEUES_PER_CORE  ((पूर्णांक)((CONFIG_FREERTOS_QUEUE_REGISTRY_आकार - 3)/CONFIG_FREERTOS_संख्या_OF_CORES))     //Save space for some preallocated tasks
#घोषणा NO_OF_QUEUES_योग    (NO_OF_QUEUES_PER_CORE * CONFIG_FREERTOS_संख्या_OF_CORES)
#घोषणा QUEUE_NAME_MAX_लंबाई    30

अटल SemaphoreHandle_t start_sem[CONFIG_FREERTOS_संख्या_OF_CORES];
अटल SemaphoreHandle_t done_sem = शून्य;
अटल अक्षर *names[NO_OF_QUEUES_योग];
अटल QueueHandle_t handles[NO_OF_QUEUES_योग];

व्योम test_queue_registry_task(व्योम *arg)
{
    पूर्णांक core = xPortGetCoreID();
    पूर्णांक offset = core * NO_OF_QUEUES_PER_CORE;
    //Create queues and accompanying queue names
    क्रम (पूर्णांक i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        handles[i + offset] = xQueueCreate(1, 1);  //Create queues
        names[i + offset] = सुस्मृति(QUEUE_NAME_MAX_लंबाई, माप(अक्षर));
        प्र_लिखो(names[i + offset], "Queue%d%d", core, i);
    }

    xSemaphoreTake(start_sem[core], portMAX_DELAY);   //Wait for start vQueueAddToRegistry()
    क्रम (पूर्णांक i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        vQueueAddToRegistry(handles[i + offset], names[i + offset]);    //Register queues to queue registry
    }
    xSemaphoreGive(done_sem);   //Signal that vQueueAddToRegistry() has completed

    vTaskDelay(1);

    xSemaphoreTake(start_sem[core], portMAX_DELAY);   //Wait to start vQueueUnregisterQueue()
    क्रम (पूर्णांक i = 0; i < NO_OF_QUEUES_PER_CORE; i++) {
        vQueueDelete(handles[i + offset]);  //Internally calls vQueueUnregisterQueue
    }
    xSemaphoreGive(done_sem);   //Signal done

    vTaskDelete(शून्य);  //Delete self
}

TEST_CASE("Test FreeRTOS Queue Registry", "[freertos]")
{
    //Create synchronization semaphores and tasks to test queue registry
    done_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_संख्या_OF_CORES, 0);
    क्रम (पूर्णांक i = 0; i < CONFIG_FREERTOS_संख्या_OF_CORES; i++) {
        start_sem[i] = x_Semaphore_Create_द्विआधारी();
        xTaskCreatePinnedToCore(test_queue_registry_task, "testing task", 4096, शून्य, UNITY_FREERTOS_PRIORITY + 1, शून्य, i);
    }

    portDISABLE_INTERRUPTS();
    क्रम (पूर्णांक i = 0; i < CONFIG_FREERTOS_संख्या_OF_CORES; i++) {
        xSemaphoreGive(start_sem[i]);  //Trigger start
    }
    portENABLE_INTERRUPTS();
    क्रम (पूर्णांक i = 0; i < CONFIG_FREERTOS_संख्या_OF_CORES; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);    //Wait for tasks to complete vQueueAddToRegistry
    }
    क्रम (पूर्णांक i = 0; i < NO_OF_QUEUES_योग; i++) {
        स्थिर अक्षर *addr = pcQueueGetName(handles[i]);
        TEST_ASSERT(addr == names[i]);   //Check vQueueAddToRegistry was successful
    }

    portDISABLE_INTERRUPTS();
    क्रम (पूर्णांक i = 0; i < CONFIG_FREERTOS_संख्या_OF_CORES; i++) {
        xSemaphoreGive(start_sem[i]);  //Trigger start
    }
    portENABLE_INTERRUPTS();
    क्रम (पूर्णांक i = 0; i < CONFIG_FREERTOS_संख्या_OF_CORES; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);    //Wait for tasks to complete vQueueUnregisterQueue
    }
    क्रम (पूर्णांक i = 0; i <  NO_OF_QUEUES_योग; i++) {
        स्थिर अक्षर *addr = pcQueueGetName(handles[i]);
        TEST_ASSERT(addr == शून्य);   //Check vQueueUnregisterQueue was successful
        handles[i] = शून्य;
    }

    //Cleanup
    क्रम (पूर्णांक i = 0; i < NO_OF_QUEUES_योग; i++) {
        मुक्त(names[i]);
        names[i] = शून्य;
    }
    क्रम (पूर्णांक i = 0; i < CONFIG_FREERTOS_संख्या_OF_CORES; i++) {
        vSemaphoreDelete(start_sem[i]);
        start_sem[i] = शून्य;
    }
    vSemaphoreDelete(done_sem);
    done_sem = शून्य;
}
#पूर्ण_अगर      //(CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE > 0)
