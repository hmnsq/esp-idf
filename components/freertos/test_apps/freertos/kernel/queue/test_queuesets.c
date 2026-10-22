/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#অন্তর্ভাব "sdkconfig.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব "freertos/FreeRTOS.h"
#অন্তর্ভাব "freertos/task.h"
#অন্তর্ভাব "freertos/queue.h"
#অন্তর্ভাব "freertos/semphr.h"
#অন্তর্ভাব "freertos/idf_additions.h"
#অন্তর্ভাব "unity.h"
#অন্তর্ভাব "test_utils.h"

#define QUEUE_LEN       4

স্থির শূন্য allocate_resources(পূর্ণাঙ্ক অঙ্ক_queues, পূর্ণাঙ্ক queue_len, QueueHandle_t *queue_list_ret, QueueSetHandle_t *queue_set_ret)
{
    // Create queue set
    *queue_set_ret = xQueueCreateSet(অঙ্ক_queues * queue_len);
    TEST_ASSERT_NOT_EQUAL(রিক্ত, *queue_set_ret);
    // Create queues and add them to the queue set
    প্রতি (পূর্ণাঙ্ক i = 0; i < অঙ্ক_queues; i++) {
        queue_list_ret[i] = xQueueCreate(queue_len, পরিমাণ(BaseType_t));
        TEST_ASSERT_NOT_EQUAL(রিক্ত, queue_list_ret[i]);
        TEST_ASSERT_EQUAL(pdPASS, xQueueAddToSet(queue_list_ret[i], *queue_set_ret));
    }
}

স্থির শূন্য free_resources(পূর্ণাঙ্ক অঙ্ক_queues, QueueHandle_t *queue_list, QueueSetHandle_t queue_set)
{
    // Remove queues form queue set and delete the queues
    প্রতি (পূর্ণাঙ্ক i = 0; i < অঙ্ক_queues; i++) {
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

#define অঙ্ক_QUEUES      5

TEST_CASE("Test Queue sets", "[freertos]")
{
    // Create queues and queue set
    QueueHandle_t queues[অঙ্ক_QUEUES];
    QueueSetHandle_t queue_set;
    allocate_resources(অঙ্ক_QUEUES, QUEUE_LEN, queues, &queue_set);

    // Fill each queue sequentially with QUEUE_LEN items
    প্রতি (পূর্ণাঙ্ক i = 0; i < অঙ্ক_QUEUES; i++) {
        প্রতি (পূর্ণাঙ্ক j = 0; j < QUEUE_LEN; j++) {
            BaseType_t item = j;
            TEST_ASSERT_EQUAL(pdTRUE, xQueueSend(queues[i], &item, 0));
        }
    }

    প্রতি (পূর্ণাঙ্ক i = 0; i < অঙ্ক_QUEUES; i++) {
        প্রতি (পূর্ণাঙ্ক j = 0; j < QUEUE_LEN; j++) {
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
    TEST_ASSERT_EQUAL(রিক্ত, xQueueSelectFromSet(queue_set, 0));

    // Cleanup queues and queue set
    free_resources(অঙ্ক_QUEUES, queues, queue_set);
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

স্থির চঞ্চল bool start_other_cores;
স্থির SemaphoreHandle_t done_sem = রিক্ত;

স্থির শূন্য send_func(শূন্য *arg)
{
    QueueHandle_t queue = (QueueHandle_t)arg;
    BaseType_t core_id = xPortGetCoreID();
    যদি (core_id == 0) {
        // We are core 0. Trigger the other cores to start
        start_other_cores = true;
    } অন্যথা {
        // Wait to be started by main core
        যাবত্ (!start_other_cores) {
            ;
        }
    }

    // Fill the queue assigned to the current core
    প্রতি (পূর্ণাঙ্ক i = 0; i < QUEUE_LEN; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xQueueSend(queue, &core_id, 0));
    }

    যদি (core_id != 0) {
        // Indicate completion to core 0 and self delete
        xSemaphoreGive(done_sem);
        vTaskDelete(রিক্ত);
    }
}

TEST_CASE("Test queue sets multi-core", "[freertos]")
{
    // Create done semaphore
    done_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_সংখ্যা_OF_CORES - 1, 0);
    TEST_ASSERT_NOT_EQUAL(রিক্ত, done_sem);

    // Create queues and queue set
    QueueHandle_t queues[CONFIG_FREERTOS_সংখ্যা_OF_CORES];
    QueueSetHandle_t queue_set;
    allocate_resources(CONFIG_FREERTOS_সংখ্যা_OF_CORES, QUEUE_LEN, queues, &queue_set);

    // Create tasks of the same priority for all cores except for core 0
    প্রতি (পূর্ণাঙ্ক i = 1; i < CONFIG_FREERTOS_সংখ্যা_OF_CORES; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(send_func, "send", 2048, (শূন্য *)queues[i], UNITY_FREERTOS_PRIORITY, রিক্ত, i));
    }

    // Core 0 calls send_func as well triggering the simultaneous sends from all cores
    send_func((শূন্য *)queues[0]);

    // Wait for all other cores to be done
    প্রতি (পূর্ণাঙ্ক i = 1; i < CONFIG_FREERTOS_সংখ্যা_OF_CORES; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);
    }

    // Read queues from the queue set, then read an item from the queue
    uint32_t queues_check_গুনতি[CONFIG_FREERTOS_সংখ্যা_OF_CORES] = {0};
    QueueSetMemberHandle_t member = xQueueSelectFromSet(queue_set, 0);
    যাবত্ (member != রিক্ত) {
        // Read the core ID from the queue, check that core ID is sane
        BaseType_t core_id;
        TEST_ASSERT_EQUAL(pdTRUE, xQueueReceive(member, &core_id, 0));
        TEST_ASSERT_LESS_THAN(CONFIG_FREERTOS_সংখ্যা_OF_CORES, core_id);
        queues_check_গুনতি[core_id]++;

        // Get next member
        member = xQueueSelectFromSet(queue_set, 0);
    }
    // Check that all items from all queues have been read
    প্রতি (পূর্ণাঙ্ক i = 0; i < CONFIG_FREERTOS_সংখ্যা_OF_CORES; i++) {
        TEST_ASSERT_EQUAL(QUEUE_LEN, queues_check_গুনতি[i]);
    }

    // Cleanup queues and queue set
    free_resources(CONFIG_FREERTOS_সংখ্যা_OF_CORES, queues, queue_set);
    // Cleanup done sem
    vSemaphoreDelete(done_sem);
    done_sem = রিক্ত;
}
#endif // CONFIG_FREERTOS_UNICORE
