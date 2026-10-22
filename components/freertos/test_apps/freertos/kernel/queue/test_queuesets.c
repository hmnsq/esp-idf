/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "sdkconfig.h"
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "freertos/queue.h"
#ضمن "freertos/semphr.h"
#ضمن "freertos/idf_additions.h"
#ضمن "unity.h"
#ضمن "test_utils.h"

#define QUEUE_LEN       4

ساكن فراغ allocate_resources(صحيح num_queues, صحيح queue_len, QueueHandle_t *queue_list_ret, QueueSetHandle_t *queue_set_ret)
{
    // Create queue set
    *queue_set_ret = xQueueCreateSet(num_queues * queue_len);
    TEST_ASSERT_NOT_EQUAL(عدم, *queue_set_ret);
    // Create queues and add them to the queue set
    עבור (صحيح i = 0; i < num_queues; i++) {
        queue_list_ret[i] = xQueueCreate(queue_len, حجم(BaseType_t));
        TEST_ASSERT_NOT_EQUAL(عدم, queue_list_ret[i]);
        TEST_ASSERT_EQUAL(pdPASS, xQueueAddToSet(queue_list_ret[i], *queue_set_ret));
    }
}

ساكن فراغ free_resources(صحيح num_queues, QueueHandle_t *queue_list, QueueSetHandle_t queue_set)
{
    // Remove queues form queue set and delete the queues
    עבור (صحيح i = 0; i < num_queues; i++) {
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

#define NUM_QUEUES      5

TEST_CASE("Test Queue sets", "[freertos]")
{
    // Create queues and queue set
    QueueHandle_t queues[NUM_QUEUES];
    QueueSetHandle_t queue_set;
    allocate_resources(NUM_QUEUES, QUEUE_LEN, queues, &queue_set);

    // Fill each queue sequentially with QUEUE_LEN items
    עבור (صحيح i = 0; i < NUM_QUEUES; i++) {
        עבור (صحيح j = 0; j < QUEUE_LEN; j++) {
            BaseType_t item = j;
            TEST_ASSERT_EQUAL(pdTRUE, xQueueSend(queues[i], &item, 0));
        }
    }

    עבור (صحيح i = 0; i < NUM_QUEUES; i++) {
        עבור (صحيح j = 0; j < QUEUE_LEN; j++) {
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
    TEST_ASSERT_EQUAL(عدم, xQueueSelectFromSet(queue_set, 0));

    // Cleanup queues and queue set
    free_resources(NUM_QUEUES, queues, queue_set);
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

ساكن متطاير bool start_other_cores;
ساكن SemaphoreHandle_t done_sem = عدم;

ساكن فراغ send_func(فراغ *arg)
{
    QueueHandle_t queue = (QueueHandle_t)arg;
    BaseType_t core_id = xPortGetCoreID();
    אם (core_id == 0) {
        // We are core 0. Trigger the other cores to start
        start_other_cores = true;
    } אחרת {
        // Wait to be started by main core
        כלעוד (!start_other_cores) {
            ;
        }
    }

    // Fill the queue assigned to the current core
    עבור (صحيح i = 0; i < QUEUE_LEN; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xQueueSend(queue, &core_id, 0));
    }

    אם (core_id != 0) {
        // Indicate completion to core 0 and self delete
        xSemaphoreGive(done_sem);
        vTaskDelete(عدم);
    }
}

TEST_CASE("Test queue sets multi-core", "[freertos]")
{
    // Create done semaphore
    done_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_NUMBER_OF_CORES - 1, 0);
    TEST_ASSERT_NOT_EQUAL(عدم, done_sem);

    // Create queues and queue set
    QueueHandle_t queues[CONFIG_FREERTOS_NUMBER_OF_CORES];
    QueueSetHandle_t queue_set;
    allocate_resources(CONFIG_FREERTOS_NUMBER_OF_CORES, QUEUE_LEN, queues, &queue_set);

    // Create tasks of the same priority for all cores except for core 0
    עבור (صحيح i = 1; i < CONFIG_FREERTOS_NUMBER_OF_CORES; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(send_func, "send", 2048, (فراغ *)queues[i], UNITY_FREERTOS_PRIORITY, عدم, i));
    }

    // Core 0 calls send_func as well triggering the simultaneous sends from all cores
    send_func((فراغ *)queues[0]);

    // Wait for all other cores to be done
    עבור (صحيح i = 1; i < CONFIG_FREERTOS_NUMBER_OF_CORES; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);
    }

    // Read queues from the queue set, then read an item from the queue
    uint32_t queues_check_count[CONFIG_FREERTOS_NUMBER_OF_CORES] = {0};
    QueueSetMemberHandle_t member = xQueueSelectFromSet(queue_set, 0);
    כלעוד (member != عدم) {
        // Read the core ID from the queue, check that core ID is sane
        BaseType_t core_id;
        TEST_ASSERT_EQUAL(pdTRUE, xQueueReceive(member, &core_id, 0));
        TEST_ASSERT_LESS_THAN(CONFIG_FREERTOS_NUMBER_OF_CORES, core_id);
        queues_check_count[core_id]++;

        // Get next member
        member = xQueueSelectFromSet(queue_set, 0);
    }
    // Check that all items from all queues have been read
    עבור (صحيح i = 0; i < CONFIG_FREERTOS_NUMBER_OF_CORES; i++) {
        TEST_ASSERT_EQUAL(QUEUE_LEN, queues_check_count[i]);
    }

    // Cleanup queues and queue set
    free_resources(CONFIG_FREERTOS_NUMBER_OF_CORES, queues, queue_set);
    // Cleanup done sem
    vSemaphoreDelete(done_sem);
    done_sem = عدم;
}
#endif // CONFIG_FREERTOS_UNICORE
