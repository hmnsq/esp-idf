/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ଅନ୍ତର୍ଭାଵ "sdkconfig.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ "freertos/FreeRTOS.h"
#ଅନ୍ତର୍ଭାଵ "freertos/task.h"
#ଅନ୍ତର୍ଭାଵ "freertos/queue.h"
#ଅନ୍ତର୍ଭାଵ "freertos/semphr.h"
#ଅନ୍ତର୍ଭାଵ "freertos/idf_additions.h"
#ଅନ୍ତର୍ଭାଵ "unity.h"
#ଅନ୍ତର୍ଭାଵ "test_utils.h"

#define QUEUE_LEN       4

ସ୍ଥିର ଶୂନ୍ୟ allocate_resources(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_queues, ପୂର୍ଣ୍ଣସଂଖ୍ୟା queue_len, QueueHandle_t *queue_list_ret, QueueSetHandle_t *queue_set_ret)
{
    // Create queue set
    *queue_set_ret = xQueueCreateSet(ଅଙ୍କ_queues * queue_len);
    TEST_ASSERT_NOT_EQUAL(ଖାଲି, *queue_set_ret);
    // Create queues and add them to the queue set
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଅଙ୍କ_queues; i++) {
        queue_list_ret[i] = xQueueCreate(queue_len, ଆକାର(BaseType_t));
        TEST_ASSERT_NOT_EQUAL(ଖାଲି, queue_list_ret[i]);
        TEST_ASSERT_EQUAL(pdPASS, xQueueAddToSet(queue_list_ret[i], *queue_set_ret));
    }
}

ସ୍ଥିର ଶୂନ୍ୟ free_resources(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_queues, QueueHandle_t *queue_list, QueueSetHandle_t queue_set)
{
    // Remove queues form queue set and delete the queues
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଅଙ୍କ_queues; i++) {
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

#define ଅଙ୍କ_QUEUES      5

TEST_CASE("Test Queue sets", "[freertos]")
{
    // Create queues and queue set
    QueueHandle_t queues[ଅଙ୍କ_QUEUES];
    QueueSetHandle_t queue_set;
    allocate_resources(ଅଙ୍କ_QUEUES, QUEUE_LEN, queues, &queue_set);

    // Fill each queue sequentially with QUEUE_LEN items
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଅଙ୍କ_QUEUES; i++) {
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < QUEUE_LEN; j++) {
            BaseType_t item = j;
            TEST_ASSERT_EQUAL(pdTRUE, xQueueSend(queues[i], &item, 0));
        }
    }

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଅଙ୍କ_QUEUES; i++) {
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < QUEUE_LEN; j++) {
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
    TEST_ASSERT_EQUAL(ଖାଲି, xQueueSelectFromSet(queue_set, 0));

    // Cleanup queues and queue set
    free_resources(ଅଙ୍କ_QUEUES, queues, queue_set);
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

ସ୍ଥିର ଅସ୍ଥିର bool start_other_cores;
ସ୍ଥିର SemaphoreHandle_t done_sem = ଖାଲି;

ସ୍ଥିର ଶୂନ୍ୟ send_func(ଶୂନ୍ୟ *arg)
{
    QueueHandle_t queue = (QueueHandle_t)arg;
    BaseType_t core_id = xPortGetCoreID();
    ଯଦି (core_id == 0) {
        // We are core 0. Trigger the other cores to start
        start_other_cores = true;
    } ଅନ୍ୟଥା {
        // Wait to be started by main core
        ଯେତେବେଳେ (!start_other_cores) {
            ;
        }
    }

    // Fill the queue assigned to the current core
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < QUEUE_LEN; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xQueueSend(queue, &core_id, 0));
    }

    ଯଦି (core_id != 0) {
        // Indicate completion to core 0 and self delete
        xSemaphoreGive(done_sem);
        vTaskDelete(ଖାଲି);
    }
}

TEST_CASE("Test queue sets multi-core", "[freertos]")
{
    // Create done semaphore
    done_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES - 1, 0);
    TEST_ASSERT_NOT_EQUAL(ଖାଲି, done_sem);

    // Create queues and queue set
    QueueHandle_t queues[CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES];
    QueueSetHandle_t queue_set;
    allocate_resources(CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES, QUEUE_LEN, queues, &queue_set);

    // Create tasks of the same priority for all cores except for core 0
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(send_func, "send", 2048, (ଶୂନ୍ୟ *)queues[i], UNITY_FREERTOS_PRIORITY, ଖାଲି, i));
    }

    // Core 0 calls send_func as well triggering the simultaneous sends from all cores
    send_func((ଶୂନ୍ୟ *)queues[0]);

    // Wait for all other cores to be done
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);
    }

    // Read queues from the queue set, then read an item from the queue
    uint32_t queues_check_ଗଣନା[CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES] = {0};
    QueueSetMemberHandle_t member = xQueueSelectFromSet(queue_set, 0);
    ଯେତେବେଳେ (member != ଖାଲି) {
        // Read the core ID from the queue, check that core ID is sane
        BaseType_t core_id;
        TEST_ASSERT_EQUAL(pdTRUE, xQueueReceive(member, &core_id, 0));
        TEST_ASSERT_LESS_THAN(CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES, core_id);
        queues_check_ଗଣନା[core_id]++;

        // Get next member
        member = xQueueSelectFromSet(queue_set, 0);
    }
    // Check that all items from all queues have been read
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; i++) {
        TEST_ASSERT_EQUAL(QUEUE_LEN, queues_check_ଗଣନା[i]);
    }

    // Cleanup queues and queue set
    free_resources(CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES, queues, queue_set);
    // Cleanup done sem
    vSemaphoreDelete(done_sem);
    done_sem = ଖାଲି;
}
#endif // CONFIG_FREERTOS_UNICORE
