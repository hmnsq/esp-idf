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

#وصف_ڪريو قطار_LEN       4

ساكن فراغ allocate_resources(صحيح انگ_queues, صحيح قطار_len, قطار_Handle_t *قطار_list_ret, قطار_سيٽ_Handle_t *قطار_سيٽ_ret)
{
    // Create queue set
    *قطار_سيٽ_ret = x_قطار_Create_سيٽ(انگ_queues * قطار_len);
    TEST_ASSERT_NOT_EQUAL(عدم, *قطار_سيٽ_ret);
    // Create queues and add them to the queue set
    لكل (صحيح i = 0; i < انگ_queues; i++) {
        قطار_list_ret[i] = x_قطار_Create(قطار_len, حجم(بنياد_Type_t));
        TEST_ASSERT_NOT_EQUAL(عدم, قطار_list_ret[i]);
        TEST_ASSERT_EQUAL(pdPASS, x_قطار_جوڙيو_منزل_سيٽ(قطار_list_ret[i], *قطار_سيٽ_ret));
    }
}

ساكن فراغ free_resources(صحيح انگ_queues, قطار_Handle_t *قطار_list, قطار_سيٽ_Handle_t قطار_سيٽ)
{
    // Remove queues form queue set and delete the queues
    لكل (صحيح i = 0; i < انگ_queues; i++) {
        TEST_ASSERT_EQUAL(pdPASS, x_قطار_Remove_From_سيٽ(قطار_list[i], قطار_سيٽ));
        v_قطار_Delete(قطار_list[i]);
    }
    v_قطار_Delete(قطار_سيٽ);
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

#وصف_ڪريو انگ_QUEUES      5

TEST_CASE("Test Queue sets", "[freertos]")
{
    // Create queues and queue set
    قطار_Handle_t queues[انگ_QUEUES];
    قطار_سيٽ_Handle_t قطار_سيٽ;
    allocate_resources(انگ_QUEUES, قطار_LEN, queues, &قطار_سيٽ);

    // Fill each queue sequentially with QUEUE_LEN items
    لكل (صحيح i = 0; i < انگ_QUEUES; i++) {
        لكل (صحيح j = 0; j < قطار_LEN; j++) {
            بنياد_Type_t شي = j;
            TEST_ASSERT_EQUAL(pdTRUE, x_قطار_Send(queues[i], &شي, 0));
        }
    }

    لكل (صحيح i = 0; i < انگ_QUEUES; i++) {
        لكل (صحيح j = 0; j < قطار_LEN; j++) {
            // Check the queue set member
            قطار_سيٽ_Member_Handle_t member = x_قطار_Select_From_سيٽ(قطار_سيٽ, 0);
            TEST_ASSERT_EQUAL(queues[i], member);
            // Check the queue's items
            بنياد_Type_t شي;
            TEST_ASSERT_EQUAL(pdTRUE, x_قطار_Receive(member, &شي, 0));
            TEST_ASSERT_EQUAL(j, شي);
        }
    }
    // Check that there are no more members
    TEST_ASSERT_EQUAL(عدم, x_قطار_Select_From_سيٽ(قطار_سيٽ, 0));

    // Cleanup queues and queue set
    free_resources(انگ_QUEUES, queues, قطار_سيٽ);
}

#جيڪڏهن_وصف_نه_ٿيل CONFIG_FREERTOS_UNICORE
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

ساكن متطاير bool شروعات_other_cores;
ساكن SemaphoreHandle_t پورو_sem = عدم;

ساكن فراغ send_func(فراغ *arg)
{
    قطار_Handle_t قطار = (قطار_Handle_t)arg;
    بنياد_Type_t core_id = x_پورٽ_Get_Core_ID();
    إذا (core_id == 0) {
        // We are core 0. Trigger the other cores to start
        شروعات_other_cores = true;
    } وإلا {
        // Wait to be started by main core
        طالما (!شروعات_other_cores) {
            ;
        }
    }

    // Fill the queue assigned to the current core
    لكل (صحيح i = 0; i < قطار_LEN; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, x_قطار_Send(قطار, &core_id, 0));
    }

    إذا (core_id != 0) {
        // Indicate completion to core 0 and self delete
        xSemaphoreGive(پورو_sem);
        vTaskDelete(عدم);
    }
}

TEST_CASE("Test queue sets multi-core", "[freertos]")
{
    // Create done semaphore
    پورو_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_عدد_جو_CORES - 1, 0);
    TEST_ASSERT_NOT_EQUAL(عدم, پورو_sem);

    // Create queues and queue set
    قطار_Handle_t queues[CONFIG_FREERTOS_عدد_جو_CORES];
    قطار_سيٽ_Handle_t قطار_سيٽ;
    allocate_resources(CONFIG_FREERTOS_عدد_جو_CORES, قطار_LEN, queues, &قطار_سيٽ);

    // Create tasks of the same priority for all cores except for core 0
    لكل (صحيح i = 1; i < CONFIG_FREERTOS_عدد_جو_CORES; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_منزل_Core(send_func, "send", 2048, (فراغ *)queues[i], UNITY_FREERTOS_ترجيح, عدم, i));
    }

    // Core 0 calls send_func as well triggering the simultaneous sends from all cores
    send_func((فراغ *)queues[0]);

    // Wait for all other cores to be done
    لكل (صحيح i = 1; i < CONFIG_FREERTOS_عدد_جو_CORES; i++) {
        xSemaphoreTake(پورو_sem, پورٽ_MAX_DELAY);
    }

    // Read queues from the queue set, then read an item from the queue
    uint32_t queues_check_ڳڻپ[CONFIG_FREERTOS_عدد_جو_CORES] = {0};
    قطار_سيٽ_Member_Handle_t member = x_قطار_Select_From_سيٽ(قطار_سيٽ, 0);
    طالما (member != عدم) {
        // Read the core ID from the queue, check that core ID is sane
        بنياد_Type_t core_id;
        TEST_ASSERT_EQUAL(pdTRUE, x_قطار_Receive(member, &core_id, 0));
        TEST_ASSERT_LESS_THAN(CONFIG_FREERTOS_عدد_جو_CORES, core_id);
        queues_check_ڳڻپ[core_id]++;

        // Get next member
        member = x_قطار_Select_From_سيٽ(قطار_سيٽ, 0);
    }
    // Check that all items from all queues have been read
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_عدد_جو_CORES; i++) {
        TEST_ASSERT_EQUAL(قطار_LEN, queues_check_ڳڻپ[i]);
    }

    // Cleanup queues and queue set
    free_resources(CONFIG_FREERTOS_عدد_جو_CORES, queues, قطار_سيٽ);
    // Cleanup done sem
    vSemaphoreDelete(پورو_sem);
    پورو_sem = عدم;
}
#جيڪڏهن_پورو // CONFIG_FREERTOS_UNICORE
