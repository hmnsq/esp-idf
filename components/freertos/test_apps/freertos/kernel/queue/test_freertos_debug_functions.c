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

#ಆದರೆ (CONFIG_FREERTOS_ಸರತಿ_REGISTRY_ಗಾತ್ರ > 0)
#ವ್ಯಾಖ್ಯಾನಿಸು NO_ದ_QUEUES_ಪ್ರತಿ_CORE  ((ಪೂರ್ಣಾಂಕ)((CONFIG_FREERTOS_ಸರತಿ_REGISTRY_ಗಾತ್ರ - 3)/CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES))     //Save space for some preallocated tasks
#ವ್ಯಾಖ್ಯಾನಿಸು NO_ದ_QUEUES_ಒಟ್ಟು    (NO_ದ_QUEUES_ಪ್ರತಿ_CORE * CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES)
#ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿ_NAME_MAX_ಉದ್ದಳತೆ    30

ಸ್ಥಿರ SemaphoreHandle_t ಆರಂಭ_sem[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES];
ಸ್ಥಿರ SemaphoreHandle_t ಮುಗಿದ_sem = ಖಾಲಿ;
ಸ್ಥಿರ ಅಕ್ಷರ *ಹೆಸರುಗಳು[NO_ದ_QUEUES_ಒಟ್ಟು];
ಸ್ಥಿರ ಸರತಿ_Handle_t handles[NO_ದ_QUEUES_ಒಟ್ಟು];

ಶೂನ್ಯ test_ಸರತಿ_registry_task(ಶೂನ್ಯ *arg)
{
    ಪೂರ್ಣಾಂಕ core = x_ಪೋರ್ಟ್_Get_Core_ID();
    ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ = core * NO_ದ_QUEUES_ಪ್ರತಿ_CORE;
    //Create queues and accompanying queue names
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < NO_ದ_QUEUES_ಪ್ರತಿ_CORE; i++) {
        handles[i + ಸ್ಥಾನಾಂತರ] = x_ಸರತಿ_Create(1, 1);  //Create queues
        ಹೆಸರುಗಳು[i + ಸ್ಥಾನಾಂತರ] = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(ಸರತಿ_NAME_MAX_ಉದ್ದಳತೆ, ಗಾತ್ರ(ಅಕ್ಷರ));
        sprintf(ಹೆಸರುಗಳು[i + ಸ್ಥಾನಾಂತರ], "Queue%d%d", core, i);
    }

    xSemaphoreTake(ಆರಂಭ_sem[core], ಪೋರ್ಟ್_MAX_DELAY);   //Wait for start vQueueAddToRegistry()
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < NO_ದ_QUEUES_ಪ್ರತಿ_CORE; i++) {
        v_ಸರತಿ_ಕೂಡಿಸು_ಗಮ್ಯ_Registry(handles[i + ಸ್ಥಾನಾಂತರ], ಹೆಸರುಗಳು[i + ಸ್ಥಾನಾಂತರ]);    //Register queues to queue registry
    }
    xSemaphoreGive(ಮುಗಿದ_sem);   //Signal that vQueueAddToRegistry() has completed

    vTaskDelay(1);

    xSemaphoreTake(ಆರಂಭ_sem[core], ಪೋರ್ಟ್_MAX_DELAY);   //Wait to start vQueueUnregisterQueue()
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < NO_ದ_QUEUES_ಪ್ರತಿ_CORE; i++) {
        v_ಸರತಿ_Delete(handles[i + ಸ್ಥಾನಾಂತರ]);  //Internally calls vQueueUnregisterQueue
    }
    xSemaphoreGive(ಮುಗಿದ_sem);   //Signal done

    vTaskDelete(ಖಾಲಿ);  //Delete self
}

TEST_CASE("Test FreeRTOS Queue Registry", "[freertos]")
{
    //Create synchronization semaphores and tasks to test queue registry
    ಮುಗಿದ_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES, 0);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; i++) {
        ಆರಂಭ_sem[i] = x_Semaphore_Create_ದ್ವಿಮಾನ();
        x_Task_Create_Pinned_ಗಮ್ಯ_Core(test_ಸರತಿ_registry_task, "testing task", 4096, ಖಾಲಿ, UNITY_FREERTOS_ಆದ್ಯತೆ + 1, ಖಾಲಿ, i);
    }

    ಪೋರ್ಟ್_DISABLE_INTERRUPTS();
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; i++) {
        xSemaphoreGive(ಆರಂಭ_sem[i]);  //Trigger start
    }
    ಪೋರ್ಟ್_ENABLE_INTERRUPTS();
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; i++) {
        xSemaphoreTake(ಮುಗಿದ_sem, ಪೋರ್ಟ್_MAX_DELAY);    //Wait for tasks to complete vQueueAddToRegistry
    }
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < NO_ದ_QUEUES_ಒಟ್ಟು; i++) {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *addr = pc_ಸರತಿ_Get_Name(handles[i]);
        TEST_ASSERT(addr == ಹೆಸರುಗಳು[i]);   //Check vQueueAddToRegistry was successful
    }

    ಪೋರ್ಟ್_DISABLE_INTERRUPTS();
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; i++) {
        xSemaphoreGive(ಆರಂಭ_sem[i]);  //Trigger start
    }
    ಪೋರ್ಟ್_ENABLE_INTERRUPTS();
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; i++) {
        xSemaphoreTake(ಮುಗಿದ_sem, ಪೋರ್ಟ್_MAX_DELAY);    //Wait for tasks to complete vQueueUnregisterQueue
    }
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i <  NO_ದ_QUEUES_ಒಟ್ಟು; i++) {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *addr = pc_ಸರತಿ_Get_Name(handles[i]);
        TEST_ASSERT(addr == ಖಾಲಿ);   //Check vQueueUnregisterQueue was successful
        handles[i] = ಖಾಲಿ;
    }

    //Cleanup
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < NO_ದ_QUEUES_ಒಟ್ಟು; i++) {
        ಬಿಡುಗಡೆ(ಹೆಸರುಗಳು[i]);
        ಹೆಸರುಗಳು[i] = ಖಾಲಿ;
    }
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; i++) {
        vSemaphoreDelete(ಆರಂಭ_sem[i]);
        ಆರಂಭ_sem[i] = ಖಾಲಿ;
    }
    vSemaphoreDelete(ಮುಗಿದ_sem);
    ಮುಗಿದ_sem = ಖಾಲಿ;
}
#ಆದರೆ_ಅಂತ್ಯ      //(CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE > 0)
