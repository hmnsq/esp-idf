/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"
#अन्तर्भाव "freertos/queue.h"
#अन्तर्भाव "freertos/semphr.h"
#अन्तर्भाव "freertos/idf_additions.h"
#अन्तर्भाव "unity.h"
#अन्तर्भाव "test_utils.h"

#define QUEUE_LEN       4

स्थिर शून्य allocate_resources(पूर्णाङ्क अङ्कः_queues, पूर्णाङ्क queue_len, QueueHandle_t *queue_list_ret, QueueSetHandle_t *queue_set_ret)
{
    // Create queue set
    *queue_set_ret = xQueueCreateSet(अङ्कः_queues * queue_len);
    TEST_ASSERT_NOT_EQUAL(रिक्त, *queue_set_ret);
    // Create queues and add them to the queue set
    प्रति (पूर्णाङ्क i = 0; i < अङ्कः_queues; i++) {
        queue_list_ret[i] = xQueueCreate(queue_len, परिमाण(BaseType_t));
        TEST_ASSERT_NOT_EQUAL(रिक्त, queue_list_ret[i]);
        TEST_ASSERT_EQUAL(pdPASS, xQueueAddToSet(queue_list_ret[i], *queue_set_ret));
    }
}

स्थिर शून्य free_resources(पूर्णाङ्क अङ्कः_queues, QueueHandle_t *queue_list, QueueSetHandle_t queue_set)
{
    // Remove queues form queue set and delete the queues
    प्रति (पूर्णाङ्क i = 0; i < अङ्कः_queues; i++) {
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

#define अङ्कः_QUEUES      5

TEST_CASE("Test Queue sets", "[freertos]")
{
    // Create queues and queue set
    QueueHandle_t queues[अङ्कः_QUEUES];
    QueueSetHandle_t queue_set;
    allocate_resources(अङ्कः_QUEUES, QUEUE_LEN, queues, &queue_set);

    // Fill each queue sequentially with QUEUE_LEN items
    प्रति (पूर्णाङ्क i = 0; i < अङ्कः_QUEUES; i++) {
        प्रति (पूर्णाङ्क j = 0; j < QUEUE_LEN; j++) {
            BaseType_t item = j;
            TEST_ASSERT_EQUAL(pdTRUE, xQueueSend(queues[i], &item, 0));
        }
    }

    प्रति (पूर्णाङ्क i = 0; i < अङ्कः_QUEUES; i++) {
        प्रति (पूर्णाङ्क j = 0; j < QUEUE_LEN; j++) {
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
    TEST_ASSERT_EQUAL(रिक्त, xQueueSelectFromSet(queue_set, 0));

    // Cleanup queues and queue set
    free_resources(अङ्कः_QUEUES, queues, queue_set);
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

स्थिर चञ्चल bool start_other_cores;
स्थिर SemaphoreHandle_t done_sem = रिक्त;

स्थिर शून्य send_func(शून्य *arg)
{
    QueueHandle_t queue = (QueueHandle_t)arg;
    BaseType_t core_id = xPortGetCoreID();
    जइ (core_id == 0) {
        // We are core 0. Trigger the other cores to start
        start_other_cores = true;
    } अण्णहा {
        // Wait to be started by main core
        जाव (!start_other_cores) {
            ;
        }
    }

    // Fill the queue assigned to the current core
    प्रति (पूर्णाङ्क i = 0; i < QUEUE_LEN; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xQueueSend(queue, &core_id, 0));
    }

    जइ (core_id != 0) {
        // Indicate completion to core 0 and self delete
        xSemaphoreGive(done_sem);
        vTaskDelete(रिक्त);
    }
}

TEST_CASE("Test queue sets multi-core", "[freertos]")
{
    // Create done semaphore
    done_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_सङ्ख्या_OF_CORES - 1, 0);
    TEST_ASSERT_NOT_EQUAL(रिक्त, done_sem);

    // Create queues and queue set
    QueueHandle_t queues[CONFIG_FREERTOS_सङ्ख्या_OF_CORES];
    QueueSetHandle_t queue_set;
    allocate_resources(CONFIG_FREERTOS_सङ्ख्या_OF_CORES, QUEUE_LEN, queues, &queue_set);

    // Create tasks of the same priority for all cores except for core 0
    प्रति (पूर्णाङ्क i = 1; i < CONFIG_FREERTOS_सङ्ख्या_OF_CORES; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(send_func, "send", 2048, (शून्य *)queues[i], UNITY_FREERTOS_PRIORITY, रिक्त, i));
    }

    // Core 0 calls send_func as well triggering the simultaneous sends from all cores
    send_func((शून्य *)queues[0]);

    // Wait for all other cores to be done
    प्रति (पूर्णाङ्क i = 1; i < CONFIG_FREERTOS_सङ्ख्या_OF_CORES; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);
    }

    // Read queues from the queue set, then read an item from the queue
    uint32_t queues_check_गणनम्[CONFIG_FREERTOS_सङ्ख्या_OF_CORES] = {0};
    QueueSetMemberHandle_t member = xQueueSelectFromSet(queue_set, 0);
    जाव (member != रिक्त) {
        // Read the core ID from the queue, check that core ID is sane
        BaseType_t core_id;
        TEST_ASSERT_EQUAL(pdTRUE, xQueueReceive(member, &core_id, 0));
        TEST_ASSERT_LESS_THAN(CONFIG_FREERTOS_सङ्ख्या_OF_CORES, core_id);
        queues_check_गणनम्[core_id]++;

        // Get next member
        member = xQueueSelectFromSet(queue_set, 0);
    }
    // Check that all items from all queues have been read
    प्रति (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_सङ्ख्या_OF_CORES; i++) {
        TEST_ASSERT_EQUAL(QUEUE_LEN, queues_check_गणनम्[i]);
    }

    // Cleanup queues and queue set
    free_resources(CONFIG_FREERTOS_सङ्ख्या_OF_CORES, queues, queue_set);
    // Cleanup done sem
    vSemaphoreDelete(done_sem);
    done_sem = रिक्त;
}
#endif // CONFIG_FREERTOS_UNICORE
