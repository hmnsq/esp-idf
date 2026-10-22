/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/queue.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/semphr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/idf_additions.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "test_utils.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_LEN       4

ਸਥਿਰ ਖਾਲੀ allocate_resources(ਪੂਰਨ ਅੰਕ_queues, ਪੂਰਨ ਕਤਾਰ_len, ਕਤਾਰ_Handle_t *ਕਤਾਰ_list_ret, ਕਤਾਰ_ਸੈੱਟ_Handle_t *ਕਤਾਰ_ਸੈੱਟ_ret)
{
    // Create queue set
    *ਕਤਾਰ_ਸੈੱਟ_ret = x_ਕਤਾਰ_Create_ਸੈੱਟ(ਅੰਕ_queues * ਕਤਾਰ_len);
    TEST_ASSERT_NOT_EQUAL(ਨਿਰਾ, *ਕਤਾਰ_ਸੈੱਟ_ret);
    // Create queues and add them to the queue set
    ਲਈ (ਪੂਰਨ i = 0; i < ਅੰਕ_queues; i++) {
        ਕਤਾਰ_list_ret[i] = x_ਕਤਾਰ_Create(ਕਤਾਰ_len, ਆਕਾਰ(ਅਧਾਰ_Type_t));
        TEST_ASSERT_NOT_EQUAL(ਨਿਰਾ, ਕਤਾਰ_list_ret[i]);
        TEST_ASSERT_EQUAL(pdPASS, x_ਕਤਾਰ_ਜੋੜੋ_ਮੰਜ਼ਿਲ_ਸੈੱਟ(ਕਤਾਰ_list_ret[i], *ਕਤਾਰ_ਸੈੱਟ_ret));
    }
}

ਸਥਿਰ ਖਾਲੀ free_resources(ਪੂਰਨ ਅੰਕ_queues, ਕਤਾਰ_Handle_t *ਕਤਾਰ_list, ਕਤਾਰ_ਸੈੱਟ_Handle_t ਕਤਾਰ_ਸੈੱਟ)
{
    // Remove queues form queue set and delete the queues
    ਲਈ (ਪੂਰਨ i = 0; i < ਅੰਕ_queues; i++) {
        TEST_ASSERT_EQUAL(pdPASS, x_ਕਤਾਰ_Remove_From_ਸੈੱਟ(ਕਤਾਰ_list[i], ਕਤਾਰ_ਸੈੱਟ));
        v_ਕਤਾਰ_Delete(ਕਤਾਰ_list[i]);
    }
    v_ਕਤਾਰ_Delete(ਕਤਾਰ_ਸੈੱਟ);
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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਅੰਕ_QUEUES      5

TEST_CASE("Test Queue sets", "[freertos]")
{
    // Create queues and queue set
    ਕਤਾਰ_Handle_t queues[ਅੰਕ_QUEUES];
    ਕਤਾਰ_ਸੈੱਟ_Handle_t ਕਤਾਰ_ਸੈੱਟ;
    allocate_resources(ਅੰਕ_QUEUES, ਕਤਾਰ_LEN, queues, &ਕਤਾਰ_ਸੈੱਟ);

    // Fill each queue sequentially with QUEUE_LEN items
    ਲਈ (ਪੂਰਨ i = 0; i < ਅੰਕ_QUEUES; i++) {
        ਲਈ (ਪੂਰਨ j = 0; j < ਕਤਾਰ_LEN; j++) {
            ਅਧਾਰ_Type_t ਵਸਤੂ = j;
            TEST_ASSERT_EQUAL(pdTRUE, x_ਕਤਾਰ_Send(queues[i], &ਵਸਤੂ, 0));
        }
    }

    ਲਈ (ਪੂਰਨ i = 0; i < ਅੰਕ_QUEUES; i++) {
        ਲਈ (ਪੂਰਨ j = 0; j < ਕਤਾਰ_LEN; j++) {
            // Check the queue set member
            ਕਤਾਰ_ਸੈੱਟ_Member_Handle_t member = x_ਕਤਾਰ_Select_From_ਸੈੱਟ(ਕਤਾਰ_ਸੈੱਟ, 0);
            TEST_ASSERT_EQUAL(queues[i], member);
            // Check the queue's items
            ਅਧਾਰ_Type_t ਵਸਤੂ;
            TEST_ASSERT_EQUAL(pdTRUE, x_ਕਤਾਰ_Receive(member, &ਵਸਤੂ, 0));
            TEST_ASSERT_EQUAL(j, ਵਸਤੂ);
        }
    }
    // Check that there are no more members
    TEST_ASSERT_EQUAL(ਨਿਰਾ, x_ਕਤਾਰ_Select_From_ਸੈੱਟ(ਕਤਾਰ_ਸੈੱਟ, 0));

    // Cleanup queues and queue set
    free_resources(ਅੰਕ_QUEUES, queues, ਕਤਾਰ_ਸੈੱਟ);
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CONFIG_FREERTOS_UNICORE
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

ਸਥਿਰ ਅਸਥਿਰ bool ਸ਼ੁਰੂ_other_cores;
ਸਥਿਰ SemaphoreHandle_t ਪੂਰਾ_sem = ਨਿਰਾ;

ਸਥਿਰ ਖਾਲੀ send_func(ਖਾਲੀ *arg)
{
    ਕਤਾਰ_Handle_t ਕਤਾਰ = (ਕਤਾਰ_Handle_t)arg;
    ਅਧਾਰ_Type_t core_id = x_ਪੋਰਟ_Get_Core_ID();
    ਜੇ (core_id == 0) {
        // We are core 0. Trigger the other cores to start
        ਸ਼ੁਰੂ_other_cores = true;
    } ਨਹੀਂਤਾਂ {
        // Wait to be started by main core
        ਜਦੋਂ (!ਸ਼ੁਰੂ_other_cores) {
            ;
        }
    }

    // Fill the queue assigned to the current core
    ਲਈ (ਪੂਰਨ i = 0; i < ਕਤਾਰ_LEN; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, x_ਕਤਾਰ_Send(ਕਤਾਰ, &core_id, 0));
    }

    ਜੇ (core_id != 0) {
        // Indicate completion to core 0 and self delete
        xSemaphoreGive(ਪੂਰਾ_sem);
        vTaskDelete(ਨਿਰਾ);
    }
}

TEST_CASE("Test queue sets multi-core", "[freertos]")
{
    // Create done semaphore
    ਪੂਰਾ_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES - 1, 0);
    TEST_ASSERT_NOT_EQUAL(ਨਿਰਾ, ਪੂਰਾ_sem);

    // Create queues and queue set
    ਕਤਾਰ_Handle_t queues[CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES];
    ਕਤਾਰ_ਸੈੱਟ_Handle_t ਕਤਾਰ_ਸੈੱਟ;
    allocate_resources(CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES, ਕਤਾਰ_LEN, queues, &ਕਤਾਰ_ਸੈੱਟ);

    // Create tasks of the same priority for all cores except for core 0
    ਲਈ (ਪੂਰਨ i = 1; i < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(send_func, "send", 2048, (ਖਾਲੀ *)queues[i], UNITY_FREERTOS_ਤਰਜੀਹ, ਨਿਰਾ, i));
    }

    // Core 0 calls send_func as well triggering the simultaneous sends from all cores
    send_func((ਖਾਲੀ *)queues[0]);

    // Wait for all other cores to be done
    ਲਈ (ਪੂਰਨ i = 1; i < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; i++) {
        xSemaphoreTake(ਪੂਰਾ_sem, ਪੋਰਟ_MAX_DELAY);
    }

    // Read queues from the queue set, then read an item from the queue
    uint32_t queues_check_ਗਿਣਤੀ[CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES] = {0};
    ਕਤਾਰ_ਸੈੱਟ_Member_Handle_t member = x_ਕਤਾਰ_Select_From_ਸੈੱਟ(ਕਤਾਰ_ਸੈੱਟ, 0);
    ਜਦੋਂ (member != ਨਿਰਾ) {
        // Read the core ID from the queue, check that core ID is sane
        ਅਧਾਰ_Type_t core_id;
        TEST_ASSERT_EQUAL(pdTRUE, x_ਕਤਾਰ_Receive(member, &core_id, 0));
        TEST_ASSERT_LESS_THAN(CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES, core_id);
        queues_check_ਗਿਣਤੀ[core_id]++;

        // Get next member
        member = x_ਕਤਾਰ_Select_From_ਸੈੱਟ(ਕਤਾਰ_ਸੈੱਟ, 0);
    }
    // Check that all items from all queues have been read
    ਲਈ (ਪੂਰਨ i = 0; i < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; i++) {
        TEST_ASSERT_EQUAL(ਕਤਾਰ_LEN, queues_check_ਗਿਣਤੀ[i]);
    }

    // Cleanup queues and queue set
    free_resources(CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES, queues, ਕਤਾਰ_ਸੈੱਟ);
    // Cleanup done sem
    vSemaphoreDelete(ਪੂਰਾ_sem);
    ਪੂਰਾ_sem = ਨਿਰਾ;
}
#ਜੇ_ਅੰਤ // CONFIG_FREERTOS_UNICORE
