/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#അന്തര്ഭാവ "sdkconfig.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ "freertos/FreeRTOS.h"
#അന്തര്ഭാവ "freertos/task.h"
#അന്തര്ഭാവ "freertos/queue.h"
#അന്തര്ഭാവ "freertos/semphr.h"
#അന്തര്ഭാവ "freertos/idf_additions.h"
#അന്തര്ഭാവ "unity.h"
#അന്തര്ഭാവ "test_utils.h"

#define QUEUE_LEN       4

സ്ഥിതം ശൂന്യം allocate_resources(പൂർണ്ണസംഖ്യ അക്കം_queues, പൂർണ്ണസംഖ്യ queue_len, QueueHandle_t *queue_list_ret, QueueSetHandle_t *queue_set_ret)
{
    // Create queue set
    *queue_set_ret = xQueueCreateSet(അക്കം_queues * queue_len);
    TEST_ASSERT_NOT_EQUAL(ശൂന്യ, *queue_set_ret);
    // Create queues and add them to the queue set
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_queues; i++) {
        queue_list_ret[i] = xQueueCreate(queue_len, വലുപ്പം(BaseType_t));
        TEST_ASSERT_NOT_EQUAL(ശൂന്യ, queue_list_ret[i]);
        TEST_ASSERT_EQUAL(pdPASS, xQueueAddToSet(queue_list_ret[i], *queue_set_ret));
    }
}

സ്ഥിതം ശൂന്യം free_resources(പൂർണ്ണസംഖ്യ അക്കം_queues, QueueHandle_t *queue_list, QueueSetHandle_t queue_set)
{
    // Remove queues form queue set and delete the queues
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_queues; i++) {
        TEST_ASSERT_EQUAL(pdPASS, xQueueRemoveFromSet(queue_list[i], queue_set));
        vQueueDelete(queue_list[i]);
    }
    vQueueDelete(queue_set);
}

/*
Test queue sets basic

Purpose:
    - Test that queue set works as expected

Procedure:
    - Create NUM_QUEUES queues and add them to the same queue set
    - Fill each queue sequentially with QUEUE_LEN items

Expected:
    - Each call to xQueueSend() should generate a member in the queue set
    - The order of the members should match the order in which xQueueSend() was called
    - The item sent by the xQueueSend() is correct when read
*/

#define അക്കം_QUEUES      5

TEST_CASE("Test Queue sets", "[freertos]")
{
    // Create queues and queue set
    QueueHandle_t queues[അക്കം_QUEUES];
    QueueSetHandle_t queue_set;
    allocate_resources(അക്കം_QUEUES, QUEUE_LEN, queues, &queue_set);

    // Fill each queue sequentially with QUEUE_LEN items
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_QUEUES; i++) {
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < QUEUE_LEN; j++) {
            BaseType_t item = j;
            TEST_ASSERT_EQUAL(pdTRUE, xQueueSend(queues[i], &item, 0));
        }
    }

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_QUEUES; i++) {
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < QUEUE_LEN; j++) {
            // Check the queue set member
            QueueSetMemberHandle_t member = xQueueSelectFromSet(queue_set, 0);
            TEST_ASSERT_EQUAL(queues[i], member);
            // Check the queue's items
            BaseType_t item;
            TEST_ASSERT_EQUAL(pdTRUE, xQueueReceive(member, &item, 0));
            TEST_ASSERT_EQUAL(j, item);
        }
    }
    // Check that there are no more members
    TEST_ASSERT_EQUAL(ശൂന്യ, xQueueSelectFromSet(queue_set, 0));

    // Cleanup queues and queue set
    free_resources(അക്കം_QUEUES, queues, queue_set);
}

#ifndef CONFIG_FREERTOS_UNICORE
/*
Test queue set SMP thread safety

Purpose:
    - Test that queue set works when being used from different cores simultaneously

Procedure:
    - Create a queue for each core and add them to the same queue set
    - Create a task on each core to send QUEUE_LEN items to their assigned queue
    - Synchronize the tasks so that the start sending at the same time

Expected:
    - Each call to xQueueSend() should generate a member in the queue set
    - The item sent by the xQueueSend() is correct when read
*/

സ്ഥിതം അസ്ഥിര bool start_other_cores;
സ്ഥിതം SemaphoreHandle_t done_sem = ശൂന്യ;

സ്ഥിതം ശൂന്യം send_func(ശൂന്യം *arg)
{
    QueueHandle_t queue = (QueueHandle_t)arg;
    BaseType_t core_id = xPortGetCoreID();
    എങ്കിൽ (core_id == 0) {
        // We are core 0. Trigger the other cores to start
        start_other_cores = true;
    } അല്ലെങ്കിൽ {
        // Wait to be started by main core
        ആകുമ്പോൾ (!start_other_cores) {
            ;
        }
    }

    // Fill the queue assigned to the current core
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < QUEUE_LEN; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xQueueSend(queue, &core_id, 0));
    }

    എങ്കിൽ (core_id != 0) {
        // Indicate completion to core 0 and self delete
        xSemaphoreGive(done_sem);
        vTaskDelete(ശൂന്യ);
    }
}

TEST_CASE("Test queue sets multi-core", "[freertos]")
{
    // Create done semaphore
    done_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_സംഖ്യ_OF_CORES - 1, 0);
    TEST_ASSERT_NOT_EQUAL(ശൂന്യ, done_sem);

    // Create queues and queue set
    QueueHandle_t queues[CONFIG_FREERTOS_സംഖ്യ_OF_CORES];
    QueueSetHandle_t queue_set;
    allocate_resources(CONFIG_FREERTOS_സംഖ്യ_OF_CORES, QUEUE_LEN, queues, &queue_set);

    // Create tasks of the same priority for all cores except for core 0
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 1; i < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(send_func, "send", 2048, (ശൂന്യം *)queues[i], UNITY_FREERTOS_PRIORITY, ശൂന്യ, i));
    }

    // Core 0 calls send_func as well triggering the simultaneous sends from all cores
    send_func((ശൂന്യം *)queues[0]);

    // Wait for all other cores to be done
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 1; i < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);
    }

    // Read queues from the queue set, then read an item from the queue
    uint32_t queues_check_എണ്ണം[CONFIG_FREERTOS_സംഖ്യ_OF_CORES] = {0};
    QueueSetMemberHandle_t member = xQueueSelectFromSet(queue_set, 0);
    ആകുമ്പോൾ (member != ശൂന്യ) {
        // Read the core ID from the queue, check that core ID is sane
        BaseType_t core_id;
        TEST_ASSERT_EQUAL(pdTRUE, xQueueReceive(member, &core_id, 0));
        TEST_ASSERT_LESS_THAN(CONFIG_FREERTOS_സംഖ്യ_OF_CORES, core_id);
        queues_check_എണ്ണം[core_id]++;

        // Get next member
        member = xQueueSelectFromSet(queue_set, 0);
    }
    // Check that all items from all queues have been read
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; i++) {
        TEST_ASSERT_EQUAL(QUEUE_LEN, queues_check_എണ്ണം[i]);
    }

    // Cleanup queues and queue set
    free_resources(CONFIG_FREERTOS_സംഖ്യ_OF_CORES, queues, queue_set);
    // Cleanup done sem
    vSemaphoreDelete(done_sem);
    done_sem = ശൂന്യ;
}
#endif // CONFIG_FREERTOS_UNICORE
