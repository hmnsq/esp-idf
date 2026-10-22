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

#شامل "sdkconfig.h"
#شامل <stdio.h>
#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل "freertos/semphr.h"
#شامل "freertos/queue.h"
#شامل "unity.h"
#شامل "test_utils.h"

#اگر (CONFIG_FREERTOS_قطار_REGISTRY_سائز > 0)
#تعریف_کرو NO_کا_QUEUES_فی_CORE  ((صحیح)((CONFIG_FREERTOS_قطار_REGISTRY_سائز - 3)/CONFIG_FREERTOS_نمبر_کا_CORES))     //Save space for some preallocated tasks
#تعریف_کرو NO_کا_QUEUES_کل    (NO_کا_QUEUES_فی_CORE * CONFIG_FREERTOS_نمبر_کا_CORES)
#تعریف_کرو قطار_NAME_MAX_لمبائی    30

ساکن SemaphoreHandle_t آغاز_sem[CONFIG_FREERTOS_نمبر_کا_CORES];
ساکن SemaphoreHandle_t مکمل_sem = تہی;
ساکن حرف *نام[NO_کا_QUEUES_کل];
ساکن قطار_Handle_t handles[NO_کا_QUEUES_کل];

خالی test_قطار_registry_task(خالی *arg)
{
    صحیح core = x_پورٹ_Get_Core_ID();
    صحیح انحراف = core * NO_کا_QUEUES_فی_CORE;
    //Create queues and accompanying queue names
    برائے (صحیح i = 0; i < NO_کا_QUEUES_فی_CORE; i++) {
        handles[i + انحراف] = x_قطار_Create(1, 1);  //Create queues
        نام[i + انحراف] = صفر_حافظہ_دو(قطار_NAME_MAX_لمبائی, حجم(حرف));
        sprintf(نام[i + انحراف], "Queue%d%d", core, i);
    }

    xSemaphoreTake(آغاز_sem[core], پورٹ_MAX_DELAY);   //Wait for start vQueueAddToRegistry()
    برائے (صحیح i = 0; i < NO_کا_QUEUES_فی_CORE; i++) {
        v_قطار_جوڑو_مقصد_Registry(handles[i + انحراف], نام[i + انحراف]);    //Register queues to queue registry
    }
    xSemaphoreGive(مکمل_sem);   //Signal that vQueueAddToRegistry() has completed

    vTaskDelay(1);

    xSemaphoreTake(آغاز_sem[core], پورٹ_MAX_DELAY);   //Wait to start vQueueUnregisterQueue()
    برائے (صحیح i = 0; i < NO_کا_QUEUES_فی_CORE; i++) {
        v_قطار_Delete(handles[i + انحراف]);  //Internally calls vQueueUnregisterQueue
    }
    xSemaphoreGive(مکمل_sem);   //Signal done

    vTaskDelete(تہی);  //Delete self
}

TEST_CASE("Test FreeRTOS Queue Registry", "[freertos]")
{
    //Create synchronization semaphores and tasks to test queue registry
    مکمل_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_نمبر_کا_CORES, 0);
    برائے (صحیح i = 0; i < CONFIG_FREERTOS_نمبر_کا_CORES; i++) {
        آغاز_sem[i] = x_Semaphore_Create_ثنائی();
        x_Task_Create_Pinned_مقصد_Core(test_قطار_registry_task, "testing task", 4096, تہی, UNITY_FREERTOS_ترجیح + 1, تہی, i);
    }

    پورٹ_DISABLE_INTERRUPTS();
    برائے (صحیح i = 0; i < CONFIG_FREERTOS_نمبر_کا_CORES; i++) {
        xSemaphoreGive(آغاز_sem[i]);  //Trigger start
    }
    پورٹ_ENABLE_INTERRUPTS();
    برائے (صحیح i = 0; i < CONFIG_FREERTOS_نمبر_کا_CORES; i++) {
        xSemaphoreTake(مکمل_sem, پورٹ_MAX_DELAY);    //Wait for tasks to complete vQueueAddToRegistry
    }
    برائے (صحیح i = 0; i < NO_کا_QUEUES_کل; i++) {
        ثابت حرف *addr = pc_قطار_Get_Name(handles[i]);
        TEST_ASSERT(addr == نام[i]);   //Check vQueueAddToRegistry was successful
    }

    پورٹ_DISABLE_INTERRUPTS();
    برائے (صحیح i = 0; i < CONFIG_FREERTOS_نمبر_کا_CORES; i++) {
        xSemaphoreGive(آغاز_sem[i]);  //Trigger start
    }
    پورٹ_ENABLE_INTERRUPTS();
    برائے (صحیح i = 0; i < CONFIG_FREERTOS_نمبر_کا_CORES; i++) {
        xSemaphoreTake(مکمل_sem, پورٹ_MAX_DELAY);    //Wait for tasks to complete vQueueUnregisterQueue
    }
    برائے (صحیح i = 0; i <  NO_کا_QUEUES_کل; i++) {
        ثابت حرف *addr = pc_قطار_Get_Name(handles[i]);
        TEST_ASSERT(addr == تہی);   //Check vQueueUnregisterQueue was successful
        handles[i] = تہی;
    }

    //Cleanup
    برائے (صحیح i = 0; i < NO_کا_QUEUES_کل; i++) {
        آزاد(نام[i]);
        نام[i] = تہی;
    }
    برائے (صحیح i = 0; i < CONFIG_FREERTOS_نمبر_کا_CORES; i++) {
        vSemaphoreDelete(آغاز_sem[i]);
        آغاز_sem[i] = تہی;
    }
    vSemaphoreDelete(مکمل_sem);
    مکمل_sem = تہی;
}
#اگر_ختم      //(CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE > 0)
