/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "freertos/queue.h"
#ಅನ್ತರ್ಭಾವ "freertos/semphr.h"
#ಅನ್ತರ್ಭಾವ "freertos/idf_additions.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "test_utils.h"

#ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿ_LEN       4

ಸ್ಥಿರ ಶೂನ್ಯ allocate_resources(ಪೂರ್ಣಾಂಕ ಅಂಕೆ_queues, ಪೂರ್ಣಾಂಕ ಸರತಿ_len, ಸರತಿ_Handle_t *ಸರತಿ_list_ret, ಸರತಿ_ಗಣ_Handle_t *ಸರತಿ_ಗಣ_ret)
{
    // Create queue set
    *ಸರತಿ_ಗಣ_ret = x_ಸರತಿ_Create_ಗಣ(ಅಂಕೆ_queues * ಸರತಿ_len);
    TEST_ASSERT_NOT_EQUAL(ಖಾಲಿ, *ಸರತಿ_ಗಣ_ret);
    // Create queues and add them to the queue set
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಅಂಕೆ_queues; i++) {
        ಸರತಿ_list_ret[i] = x_ಸರತಿ_Create(ಸರತಿ_len, ಗಾತ್ರ(ತಳ_Type_t));
        TEST_ASSERT_NOT_EQUAL(ಖಾಲಿ, ಸರತಿ_list_ret[i]);
        TEST_ASSERT_EQUAL(pdPASS, x_ಸರತಿ_ಕೂಡಿಸು_ಗಮ್ಯ_ಗಣ(ಸರತಿ_list_ret[i], *ಸರತಿ_ಗಣ_ret));
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ free_resources(ಪೂರ್ಣಾಂಕ ಅಂಕೆ_queues, ಸರತಿ_Handle_t *ಸರತಿ_list, ಸರತಿ_ಗಣ_Handle_t ಸರತಿ_ಗಣ)
{
    // Remove queues form queue set and delete the queues
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಅಂಕೆ_queues; i++) {
        TEST_ASSERT_EQUAL(pdPASS, x_ಸರತಿ_Remove_From_ಗಣ(ಸರತಿ_list[i], ಸರತಿ_ಗಣ));
        v_ಸರತಿ_Delete(ಸರತಿ_list[i]);
    }
    v_ಸರತಿ_Delete(ಸರತಿ_ಗಣ);
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

#ವ್ಯಾಖ್ಯಾನಿಸು ಅಂಕೆ_QUEUES      5

TEST_CASE("Test Queue sets", "[freertos]")
{
    // Create queues and queue set
    ಸರತಿ_Handle_t queues[ಅಂಕೆ_QUEUES];
    ಸರತಿ_ಗಣ_Handle_t ಸರತಿ_ಗಣ;
    allocate_resources(ಅಂಕೆ_QUEUES, ಸರತಿ_LEN, queues, &ಸರತಿ_ಗಣ);

    // Fill each queue sequentially with QUEUE_LEN items
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಅಂಕೆ_QUEUES; i++) {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < ಸರತಿ_LEN; j++) {
            ತಳ_Type_t ಅಂಶ = j;
            TEST_ASSERT_EQUAL(pdTRUE, x_ಸರತಿ_Send(queues[i], &ಅಂಶ, 0));
        }
    }

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಅಂಕೆ_QUEUES; i++) {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < ಸರತಿ_LEN; j++) {
            // Check the queue set member
            ಸರತಿ_ಗಣ_Member_Handle_t member = x_ಸರತಿ_Select_From_ಗಣ(ಸರತಿ_ಗಣ, 0);
            TEST_ASSERT_EQUAL(queues[i], member);
            // Check the queue's items
            ತಳ_Type_t ಅಂಶ;
            TEST_ASSERT_EQUAL(pdTRUE, x_ಸರತಿ_Receive(member, &ಅಂಶ, 0));
            TEST_ASSERT_EQUAL(j, ಅಂಶ);
        }
    }
    // Check that there are no more members
    TEST_ASSERT_EQUAL(ಖಾಲಿ, x_ಸರತಿ_Select_From_ಗಣ(ಸರತಿ_ಗಣ, 0));

    // Cleanup queues and queue set
    free_resources(ಅಂಕೆ_QUEUES, queues, ಸರತಿ_ಗಣ);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CONFIG_FREERTOS_UNICORE
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

ಸ್ಥಿರ ಅಸ್ಥಿರ bool ಆರಂಭ_other_cores;
ಸ್ಥಿರ SemaphoreHandle_t ಮುಗಿದ_sem = ಖಾಲಿ;

ಸ್ಥಿರ ಶೂನ್ಯ send_func(ಶೂನ್ಯ *arg)
{
    ಸರತಿ_Handle_t ಸರತಿ = (ಸರತಿ_Handle_t)arg;
    ತಳ_Type_t core_id = x_ಪೋರ್ಟ್_Get_Core_ID();
    ಆದರೆ (core_id == 0) {
        // We are core 0. Trigger the other cores to start
        ಆರಂಭ_other_cores = true;
    } ಇಲ್ಲದಿದ್ದರೆ {
        // Wait to be started by main core
        ಆಗ (!ಆರಂಭ_other_cores) {
            ;
        }
    }

    // Fill the queue assigned to the current core
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಸರತಿ_LEN; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, x_ಸರತಿ_Send(ಸರತಿ, &core_id, 0));
    }

    ಆದರೆ (core_id != 0) {
        // Indicate completion to core 0 and self delete
        xSemaphoreGive(ಮುಗಿದ_sem);
        vTaskDelete(ಖಾಲಿ);
    }
}

TEST_CASE("Test queue sets multi-core", "[freertos]")
{
    // Create done semaphore
    ಮುಗಿದ_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES - 1, 0);
    TEST_ASSERT_NOT_EQUAL(ಖಾಲಿ, ಮುಗಿದ_sem);

    // Create queues and queue set
    ಸರತಿ_Handle_t queues[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES];
    ಸರತಿ_ಗಣ_Handle_t ಸರತಿ_ಗಣ;
    allocate_resources(CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES, ಸರತಿ_LEN, queues, &ಸರತಿ_ಗಣ);

    // Create tasks of the same priority for all cores except for core 0
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 1; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_ಗಮ್ಯ_Core(send_func, "send", 2048, (ಶೂನ್ಯ *)queues[i], UNITY_FREERTOS_ಆದ್ಯತೆ, ಖಾಲಿ, i));
    }

    // Core 0 calls send_func as well triggering the simultaneous sends from all cores
    send_func((ಶೂನ್ಯ *)queues[0]);

    // Wait for all other cores to be done
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 1; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; i++) {
        xSemaphoreTake(ಮುಗಿದ_sem, ಪೋರ್ಟ್_MAX_DELAY);
    }

    // Read queues from the queue set, then read an item from the queue
    uint32_t queues_check_ಎಣಿಕೆ[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES] = {0};
    ಸರತಿ_ಗಣ_Member_Handle_t member = x_ಸರತಿ_Select_From_ಗಣ(ಸರತಿ_ಗಣ, 0);
    ಆಗ (member != ಖಾಲಿ) {
        // Read the core ID from the queue, check that core ID is sane
        ತಳ_Type_t core_id;
        TEST_ASSERT_EQUAL(pdTRUE, x_ಸರತಿ_Receive(member, &core_id, 0));
        TEST_ASSERT_LESS_THAN(CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES, core_id);
        queues_check_ಎಣಿಕೆ[core_id]++;

        // Get next member
        member = x_ಸರತಿ_Select_From_ಗಣ(ಸರತಿ_ಗಣ, 0);
    }
    // Check that all items from all queues have been read
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; i++) {
        TEST_ASSERT_EQUAL(ಸರತಿ_LEN, queues_check_ಎಣಿಕೆ[i]);
    }

    // Cleanup queues and queue set
    free_resources(CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES, queues, ಸರತಿ_ಗಣ);
    // Cleanup done sem
    vSemaphoreDelete(ಮುಗಿದ_sem);
    ಮುಗಿದ_sem = ಖಾಲಿ;
}
#ಆದರೆ_ಅಂತ್ಯ // CONFIG_FREERTOS_UNICORE
