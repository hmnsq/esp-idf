/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#அந்தர்பாவ "sdkconfig.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ "freertos/FreeRTOS.h"
#அந்தர்பாவ "freertos/task.h"
#அந்தர்பாவ "freertos/queue.h"
#அந்தர்பாவ "freertos/semphr.h"
#அந்தர்பாவ "freertos/idf_additions.h"
#அந்தர்பாவ "unity.h"
#அந்தர்பாவ "test_utils.h"

#define QUEUE_LEN       4

நிலையான வெறுமை allocate_resources(முழு இலக்கம்_queues, முழு queue_len, QueueHandle_t *queue_list_ret, QueueSetHandle_t *queue_set_ret)
{
    // Create queue set
    *queue_set_ret = xQueueCreateSet(இலக்கம்_queues * queue_len);
    TEST_ASSERT_NOT_EQUAL(வெற்று, *queue_set_ret);
    // Create queues and add them to the queue set
    க்கு (முழு i = 0; i < இலக்கம்_queues; i++) {
        queue_list_ret[i] = xQueueCreate(queue_len, அளவு(BaseType_t));
        TEST_ASSERT_NOT_EQUAL(வெற்று, queue_list_ret[i]);
        TEST_ASSERT_EQUAL(pdPASS, xQueueAddToSet(queue_list_ret[i], *queue_set_ret));
    }
}

நிலையான வெறுமை free_resources(முழு இலக்கம்_queues, QueueHandle_t *queue_list, QueueSetHandle_t queue_set)
{
    // Remove queues form queue set and delete the queues
    க்கு (முழு i = 0; i < இலக்கம்_queues; i++) {
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

#define இலக்கம்_QUEUES      5

TEST_CASE("Test Queue sets", "[freertos]")
{
    // Create queues and queue set
    QueueHandle_t queues[இலக்கம்_QUEUES];
    QueueSetHandle_t queue_set;
    allocate_resources(இலக்கம்_QUEUES, QUEUE_LEN, queues, &queue_set);

    // Fill each queue sequentially with QUEUE_LEN items
    க்கு (முழு i = 0; i < இலக்கம்_QUEUES; i++) {
        க்கு (முழு j = 0; j < QUEUE_LEN; j++) {
            BaseType_t item = j;
            TEST_ASSERT_EQUAL(pdTRUE, xQueueSend(queues[i], &item, 0));
        }
    }

    க்கு (முழு i = 0; i < இலக்கம்_QUEUES; i++) {
        க்கு (முழு j = 0; j < QUEUE_LEN; j++) {
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
    TEST_ASSERT_EQUAL(வெற்று, xQueueSelectFromSet(queue_set, 0));

    // Cleanup queues and queue set
    free_resources(இலக்கம்_QUEUES, queues, queue_set);
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

நிலையான நிலையற்ற bool start_other_cores;
நிலையான SemaphoreHandle_t done_sem = வெற்று;

நிலையான வெறுமை send_func(வெறுமை *arg)
{
    QueueHandle_t queue = (QueueHandle_t)arg;
    BaseType_t core_id = xPortGetCoreID();
    எனில் (core_id == 0) {
        // We are core 0. Trigger the other cores to start
        start_other_cores = true;
    } இல்லையெனில் {
        // Wait to be started by main core
        போது (!start_other_cores) {
            ;
        }
    }

    // Fill the queue assigned to the current core
    க்கு (முழு i = 0; i < QUEUE_LEN; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xQueueSend(queue, &core_id, 0));
    }

    எனில் (core_id != 0) {
        // Indicate completion to core 0 and self delete
        xSemaphoreGive(done_sem);
        vTaskDelete(வெற்று);
    }
}

TEST_CASE("Test queue sets multi-core", "[freertos]")
{
    // Create done semaphore
    done_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_எண்_OF_CORES - 1, 0);
    TEST_ASSERT_NOT_EQUAL(வெற்று, done_sem);

    // Create queues and queue set
    QueueHandle_t queues[CONFIG_FREERTOS_எண்_OF_CORES];
    QueueSetHandle_t queue_set;
    allocate_resources(CONFIG_FREERTOS_எண்_OF_CORES, QUEUE_LEN, queues, &queue_set);

    // Create tasks of the same priority for all cores except for core 0
    க்கு (முழு i = 1; i < CONFIG_FREERTOS_எண்_OF_CORES; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(send_func, "send", 2048, (வெறுமை *)queues[i], UNITY_FREERTOS_PRIORITY, வெற்று, i));
    }

    // Core 0 calls send_func as well triggering the simultaneous sends from all cores
    send_func((வெறுமை *)queues[0]);

    // Wait for all other cores to be done
    க்கு (முழு i = 1; i < CONFIG_FREERTOS_எண்_OF_CORES; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);
    }

    // Read queues from the queue set, then read an item from the queue
    uint32_t queues_check_மொத்தம்[CONFIG_FREERTOS_எண்_OF_CORES] = {0};
    QueueSetMemberHandle_t member = xQueueSelectFromSet(queue_set, 0);
    போது (member != வெற்று) {
        // Read the core ID from the queue, check that core ID is sane
        BaseType_t core_id;
        TEST_ASSERT_EQUAL(pdTRUE, xQueueReceive(member, &core_id, 0));
        TEST_ASSERT_LESS_THAN(CONFIG_FREERTOS_எண்_OF_CORES, core_id);
        queues_check_மொத்தம்[core_id]++;

        // Get next member
        member = xQueueSelectFromSet(queue_set, 0);
    }
    // Check that all items from all queues have been read
    க்கு (முழு i = 0; i < CONFIG_FREERTOS_எண்_OF_CORES; i++) {
        TEST_ASSERT_EQUAL(QUEUE_LEN, queues_check_மொத்தம்[i]);
    }

    // Cleanup queues and queue set
    free_resources(CONFIG_FREERTOS_எண்_OF_CORES, queues, queue_set);
    // Cleanup done sem
    vSemaphoreDelete(done_sem);
    done_sem = வெற்று;
}
#endif // CONFIG_FREERTOS_UNICORE
