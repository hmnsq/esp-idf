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

#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "freertos/semphr.h"
#అన్తర్భావ "freertos/queue.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "test_utils.h"

#ఒకవేళ (CONFIG_FREERTOS_వరుస_REGISTRY_కొలత > 0)
#నిర్వచించు NO_యొక్క_QUEUES_చొప్పున_CORE  ((పూర్ణాంకం)((CONFIG_FREERTOS_వరుస_REGISTRY_కొలత - 3)/CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES))     //Save space for some preallocated tasks
#నిర్వచించు NO_యొక్క_QUEUES_మొత్తం    (NO_యొక్క_QUEUES_చొప్పున_CORE * CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES)
#నిర్వచించు వరుస_NAME_MAX_నిడివి    30

స్థిర SemaphoreHandle_t ప్రారంభం_sem[CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES];
స్థిర SemaphoreHandle_t పూర్తి_sem = ఖాళీ;
స్థిర అక్షరం *పేర్లు[NO_యొక్క_QUEUES_మొత్తం];
స్థిర వరుస_Handle_t handles[NO_యొక్క_QUEUES_మొత్తం];

శూన్య test_వరుస_registry_task(శూన్య *arg)
{
    పూర్ణాంకం core = x_పోర్టు_Get_Core_ID();
    పూర్ణాంకం స్థానభ్రంశం = core * NO_యొక్క_QUEUES_చొప్పున_CORE;
    //Create queues and accompanying queue names
    కొరకు (పూర్ణాంకం i = 0; i < NO_యొక్క_QUEUES_చొప్పున_CORE; i++) {
        handles[i + స్థానభ్రంశం] = x_వరుస_Create(1, 1);  //Create queues
        పేర్లు[i + స్థానభ్రంశం] = సున్నా_జ్ఞాపకం_ఇవ్వు(వరుస_NAME_MAX_నిడివి, పరిమాణం(అక్షరం));
        sprintf(పేర్లు[i + స్థానభ్రంశం], "Queue%d%d", core, i);
    }

    xSemaphoreTake(ప్రారంభం_sem[core], పోర్టు_MAX_DELAY);   //Wait for start vQueueAddToRegistry()
    కొరకు (పూర్ణాంకం i = 0; i < NO_యొక్క_QUEUES_చొప్పున_CORE; i++) {
        v_వరుస_కూడు_గమ్యం_Registry(handles[i + స్థానభ్రంశం], పేర్లు[i + స్థానభ్రంశం]);    //Register queues to queue registry
    }
    xSemaphoreGive(పూర్తి_sem);   //Signal that vQueueAddToRegistry() has completed

    vTaskDelay(1);

    xSemaphoreTake(ప్రారంభం_sem[core], పోర్టు_MAX_DELAY);   //Wait to start vQueueUnregisterQueue()
    కొరకు (పూర్ణాంకం i = 0; i < NO_యొక్క_QUEUES_చొప్పున_CORE; i++) {
        v_వరుస_Delete(handles[i + స్థానభ్రంశం]);  //Internally calls vQueueUnregisterQueue
    }
    xSemaphoreGive(పూర్తి_sem);   //Signal done

    vTaskDelete(ఖాళీ);  //Delete self
}

TEST_CASE("Test FreeRTOS Queue Registry", "[freertos]")
{
    //Create synchronization semaphores and tasks to test queue registry
    పూర్తి_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES, 0);
    కొరకు (పూర్ణాంకం i = 0; i < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; i++) {
        ప్రారంభం_sem[i] = x_Semaphore_Create_ద్విమాన();
        x_Task_Create_Pinned_గమ్యం_Core(test_వరుస_registry_task, "testing task", 4096, ఖాళీ, UNITY_FREERTOS_ప్రాధాన్యత + 1, ఖాళీ, i);
    }

    పోర్టు_DISABLE_INTERRUPTS();
    కొరకు (పూర్ణాంకం i = 0; i < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; i++) {
        xSemaphoreGive(ప్రారంభం_sem[i]);  //Trigger start
    }
    పోర్టు_ENABLE_INTERRUPTS();
    కొరకు (పూర్ణాంకం i = 0; i < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; i++) {
        xSemaphoreTake(పూర్తి_sem, పోర్టు_MAX_DELAY);    //Wait for tasks to complete vQueueAddToRegistry
    }
    కొరకు (పూర్ణాంకం i = 0; i < NO_యొక్క_QUEUES_మొత్తం; i++) {
        స్థిరరాశి అక్షరం *addr = pc_వరుస_Get_Name(handles[i]);
        TEST_ASSERT(addr == పేర్లు[i]);   //Check vQueueAddToRegistry was successful
    }

    పోర్టు_DISABLE_INTERRUPTS();
    కొరకు (పూర్ణాంకం i = 0; i < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; i++) {
        xSemaphoreGive(ప్రారంభం_sem[i]);  //Trigger start
    }
    పోర్టు_ENABLE_INTERRUPTS();
    కొరకు (పూర్ణాంకం i = 0; i < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; i++) {
        xSemaphoreTake(పూర్తి_sem, పోర్టు_MAX_DELAY);    //Wait for tasks to complete vQueueUnregisterQueue
    }
    కొరకు (పూర్ణాంకం i = 0; i <  NO_యొక్క_QUEUES_మొత్తం; i++) {
        స్థిరరాశి అక్షరం *addr = pc_వరుస_Get_Name(handles[i]);
        TEST_ASSERT(addr == ఖాళీ);   //Check vQueueUnregisterQueue was successful
        handles[i] = ఖాళీ;
    }

    //Cleanup
    కొరకు (పూర్ణాంకం i = 0; i < NO_యొక్క_QUEUES_మొత్తం; i++) {
        విడుదల(పేర్లు[i]);
        పేర్లు[i] = ఖాళీ;
    }
    కొరకు (పూర్ణాంకం i = 0; i < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; i++) {
        vSemaphoreDelete(ప్రారంభం_sem[i]);
        ప్రారంభం_sem[i] = ఖాళీ;
    }
    vSemaphoreDelete(పూర్తి_sem);
    పూర్తి_sem = ఖాళీ;
}
#అయితే_ముగింపు      //(CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE > 0)
