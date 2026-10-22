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

#परिभाषय QUEUE_LEN       4

स्थिर शून्य allocate_resources(पूर्णाङ्क अङ्कः_queues, पूर्णाङ्क queue_len, QueueHandle_t *queue_list_ret, Queue_समुच्चयः_Handle_t *queue_समुच्चयः_ret)
{
    // Create queue set
    *queue_समुच्चयः_ret = x_Queue_Create_समुच्चयः(अङ्कः_queues * queue_len);
    TEST_ASSERT_NOT_EQUAL(रिक्त, *queue_समुच्चयः_ret);
    // Create queues and add them to the queue set
    प्रति (पूर्णाङ्क i = 0; i < अङ्कः_queues; i++) {
        queue_list_ret[i] = xQueueCreate(queue_len, परिमाण(आधारः_Type_t));
        TEST_ASSERT_NOT_EQUAL(रिक्त, queue_list_ret[i]);
        TEST_ASSERT_EQUAL(pdPASS, x_Queue_सङ्कलय_गन्तव्यम्_समुच्चयः(queue_list_ret[i], *queue_समुच्चयः_ret));
    }
}

स्थिर शून्य free_resources(पूर्णाङ्क अङ्कः_queues, QueueHandle_t *queue_list, Queue_समुच्चयः_Handle_t queue_समुच्चयः)
{
    // Remove queues form queue set and delete the queues
    प्रति (पूर्णाङ्क i = 0; i < अङ्कः_queues; i++) {
        TEST_ASSERT_EQUAL(pdPASS, x_Queue_Remove_From_समुच्चयः(queue_list[i], queue_समुच्चयः));
        vQueueDelete(queue_list[i]);
    }
    vQueueDelete(queue_समुच्चयः);
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

#परिभाषय अङ्कः_QUEUES      5

TEST_CASE("Test Queue sets", "[freertos]")
{
    // Create queues and queue set
    QueueHandle_t queues[अङ्कः_QUEUES];
    Queue_समुच्चयः_Handle_t queue_समुच्चयः;
    allocate_resources(अङ्कः_QUEUES, QUEUE_LEN, queues, &queue_समुच्चयः);

    // Fill each queue sequentially with QUEUE_LEN items
    प्रति (पूर्णाङ्क i = 0; i < अङ्कः_QUEUES; i++) {
        प्रति (पूर्णाङ्क j = 0; j < QUEUE_LEN; j++) {
            आधारः_Type_t item = j;
            TEST_ASSERT_EQUAL(pdTRUE, xQueueSend(queues[i], &item, 0));
        }
    }

    प्रति (पूर्णाङ्क i = 0; i < अङ्कः_QUEUES; i++) {
        प्रति (पूर्णाङ्क j = 0; j < QUEUE_LEN; j++) {
            // Check the queue set member
            Queue_समुच्चयः_Member_Handle_t member = x_Queue_Select_From_समुच्चयः(queue_समुच्चयः, 0);
            TEST_ASSERT_EQUAL(queues[i], member);
            // Check the queue's items
            आधारः_Type_t item;
            TEST_ASSERT_EQUAL(pdTRUE, xQueueReceive(member, &item, 0));
            TEST_ASSERT_EQUAL(j, item);
        }
    }
    // Check that there are no more members
    TEST_ASSERT_EQUAL(रिक्त, x_Queue_Select_From_समुच्चयः(queue_समुच्चयः, 0));

    // Cleanup queues and queue set
    free_resources(अङ्कः_QUEUES, queues, queue_समुच्चयः);
}

#यदि_अपरिभाषितम् CONFIG_FREERTOS_UNICORE
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
स्थिर SemaphoreHandle_t समाप्तम्_sem = रिक्त;

स्थिर शून्य send_func(शून्य *arg)
{
    QueueHandle_t queue = (QueueHandle_t)arg;
    आधारः_Type_t core_id = x_पत्तनम्_Get_Core_ID();
    यदि (core_id == 0) {
        // We are core 0. Trigger the other cores to start
        start_other_cores = true;
    } अन्यथा {
        // Wait to be started by main core
        यावत् (!start_other_cores) {
            ;
        }
    }

    // Fill the queue assigned to the current core
    प्रति (पूर्णाङ्क i = 0; i < QUEUE_LEN; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xQueueSend(queue, &core_id, 0));
    }

    यदि (core_id != 0) {
        // Indicate completion to core 0 and self delete
        xSemaphoreGive(समाप्तम्_sem);
        vTaskDelete(रिक्त);
    }
}

TEST_CASE("Test queue sets multi-core", "[freertos]")
{
    // Create done semaphore
    समाप्तम्_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES - 1, 0);
    TEST_ASSERT_NOT_EQUAL(रिक्त, समाप्तम्_sem);

    // Create queues and queue set
    QueueHandle_t queues[CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES];
    Queue_समुच्चयः_Handle_t queue_समुच्चयः;
    allocate_resources(CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES, QUEUE_LEN, queues, &queue_समुच्चयः);

    // Create tasks of the same priority for all cores except for core 0
    प्रति (पूर्णाङ्क i = 1; i < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_गन्तव्यम्_Core(send_func, "send", 2048, (शून्य *)queues[i], UNITY_FREERTOS_प्राथमिकता, रिक्त, i));
    }

    // Core 0 calls send_func as well triggering the simultaneous sends from all cores
    send_func((शून्य *)queues[0]);

    // Wait for all other cores to be done
    प्रति (पूर्णाङ्क i = 1; i < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES; i++) {
        xSemaphoreTake(समाप्तम्_sem, पत्तनम्_MAX_DELAY);
    }

    // Read queues from the queue set, then read an item from the queue
    uint32_t queues_check_गणनम्[CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES] = {0};
    Queue_समुच्चयः_Member_Handle_t member = x_Queue_Select_From_समुच्चयः(queue_समुच्चयः, 0);
    यावत् (member != रिक्त) {
        // Read the core ID from the queue, check that core ID is sane
        आधारः_Type_t core_id;
        TEST_ASSERT_EQUAL(pdTRUE, xQueueReceive(member, &core_id, 0));
        TEST_ASSERT_LESS_THAN(CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES, core_id);
        queues_check_गणनम्[core_id]++;

        // Get next member
        member = x_Queue_Select_From_समुच्चयः(queue_समुच्चयः, 0);
    }
    // Check that all items from all queues have been read
    प्रति (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES; i++) {
        TEST_ASSERT_EQUAL(QUEUE_LEN, queues_check_गणनम्[i]);
    }

    // Cleanup queues and queue set
    free_resources(CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES, queues, queue_समुच्चयः);
    // Cleanup done sem
    vSemaphoreDelete(समाप्तम्_sem);
    समाप्तम्_sem = रिक्त;
}
#यदि_अन्तः // CONFIG_FREERTOS_UNICORE
