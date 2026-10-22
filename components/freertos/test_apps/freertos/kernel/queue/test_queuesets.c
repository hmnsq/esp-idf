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

#تعریف_کړه کتار_LEN       4

ساكن فراغ allocate_resources(صحيح عدد_queues, صحيح کتار_len, کتار_Handle_t *کتار_list_ret, کتار_سېټ_Handle_t *کتار_سېټ_ret)
{
    // Create queue set
    *کتار_سېټ_ret = x_کتار_Create_سېټ(عدد_queues * کتار_len);
    TEST_ASSERT_NOT_EQUAL(عدم, *کتار_سېټ_ret);
    // Create queues and add them to the queue set
    لكل (صحيح i = 0; i < عدد_queues; i++) {
        کتار_list_ret[i] = x_کتار_Create(کتار_len, حجم(بنسټ_Type_t));
        TEST_ASSERT_NOT_EQUAL(عدم, کتار_list_ret[i]);
        TEST_ASSERT_EQUAL(pdPASS, x_کتار_جمع_کړه_منزل_سېټ(کتار_list_ret[i], *کتار_سېټ_ret));
    }
}

ساكن فراغ free_resources(صحيح عدد_queues, کتار_Handle_t *کتار_list, کتار_سېټ_Handle_t کتار_سېټ)
{
    // Remove queues form queue set and delete the queues
    لكل (صحيح i = 0; i < عدد_queues; i++) {
        TEST_ASSERT_EQUAL(pdPASS, x_کتار_Remove_From_سېټ(کتار_list[i], کتار_سېټ));
        v_کتار_Delete(کتار_list[i]);
    }
    v_کتار_Delete(کتار_سېټ);
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

#تعریف_کړه عدد_QUEUES      5

TEST_CASE("Test Queue sets", "[freertos]")
{
    // Create queues and queue set
    کتار_Handle_t queues[عدد_QUEUES];
    کتار_سېټ_Handle_t کتار_سېټ;
    allocate_resources(عدد_QUEUES, کتار_LEN, queues, &کتار_سېټ);

    // Fill each queue sequentially with QUEUE_LEN items
    لكل (صحيح i = 0; i < عدد_QUEUES; i++) {
        لكل (صحيح j = 0; j < کتار_LEN; j++) {
            بنسټ_Type_t توکی = j;
            TEST_ASSERT_EQUAL(pdTRUE, x_کتار_Send(queues[i], &توکی, 0));
        }
    }

    لكل (صحيح i = 0; i < عدد_QUEUES; i++) {
        لكل (صحيح j = 0; j < کتار_LEN; j++) {
            // Check the queue set member
            کتار_سېټ_Member_Handle_t member = x_کتار_Select_From_سېټ(کتار_سېټ, 0);
            TEST_ASSERT_EQUAL(queues[i], member);
            // Check the queue's items
            بنسټ_Type_t توکی;
            TEST_ASSERT_EQUAL(pdTRUE, x_کتار_Receive(member, &توکی, 0));
            TEST_ASSERT_EQUAL(j, توکی);
        }
    }
    // Check that there are no more members
    TEST_ASSERT_EQUAL(عدم, x_کتار_Select_From_سېټ(کتار_سېټ, 0));

    // Cleanup queues and queue set
    free_resources(عدد_QUEUES, queues, کتار_سېټ);
}

#که_نه_تعریف_شوی CONFIG_FREERTOS_UNICORE
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

ساكن متطاير bool پیل_other_cores;
ساكن SemaphoreHandle_t بشپړ_sem = عدم;

ساكن فراغ send_func(فراغ *arg)
{
    کتار_Handle_t کتار = (کتار_Handle_t)arg;
    بنسټ_Type_t core_id = x_پورټ_Get_Core_ID();
    إذا (core_id == 0) {
        // We are core 0. Trigger the other cores to start
        پیل_other_cores = true;
    } وإلا {
        // Wait to be started by main core
        طالما (!پیل_other_cores) {
            ;
        }
    }

    // Fill the queue assigned to the current core
    لكل (صحيح i = 0; i < کتار_LEN; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, x_کتار_Send(کتار, &core_id, 0));
    }

    إذا (core_id != 0) {
        // Indicate completion to core 0 and self delete
        xSemaphoreGive(بشپړ_sem);
        vTaskDelete(عدم);
    }
}

TEST_CASE("Test queue sets multi-core", "[freertos]")
{
    // Create done semaphore
    بشپړ_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_شمېره_د_CORES - 1, 0);
    TEST_ASSERT_NOT_EQUAL(عدم, بشپړ_sem);

    // Create queues and queue set
    کتار_Handle_t queues[CONFIG_FREERTOS_شمېره_د_CORES];
    کتار_سېټ_Handle_t کتار_سېټ;
    allocate_resources(CONFIG_FREERTOS_شمېره_د_CORES, کتار_LEN, queues, &کتار_سېټ);

    // Create tasks of the same priority for all cores except for core 0
    لكل (صحيح i = 1; i < CONFIG_FREERTOS_شمېره_د_CORES; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_منزل_Core(send_func, "send", 2048, (فراغ *)queues[i], UNITY_FREERTOS_لومړیتوب, عدم, i));
    }

    // Core 0 calls send_func as well triggering the simultaneous sends from all cores
    send_func((فراغ *)queues[0]);

    // Wait for all other cores to be done
    لكل (صحيح i = 1; i < CONFIG_FREERTOS_شمېره_د_CORES; i++) {
        xSemaphoreTake(بشپړ_sem, پورټ_MAX_DELAY);
    }

    // Read queues from the queue set, then read an item from the queue
    uint32_t queues_check_شمېر[CONFIG_FREERTOS_شمېره_د_CORES] = {0};
    کتار_سېټ_Member_Handle_t member = x_کتار_Select_From_سېټ(کتار_سېټ, 0);
    طالما (member != عدم) {
        // Read the core ID from the queue, check that core ID is sane
        بنسټ_Type_t core_id;
        TEST_ASSERT_EQUAL(pdTRUE, x_کتار_Receive(member, &core_id, 0));
        TEST_ASSERT_LESS_THAN(CONFIG_FREERTOS_شمېره_د_CORES, core_id);
        queues_check_شمېر[core_id]++;

        // Get next member
        member = x_کتار_Select_From_سېټ(کتار_سېټ, 0);
    }
    // Check that all items from all queues have been read
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_شمېره_د_CORES; i++) {
        TEST_ASSERT_EQUAL(کتار_LEN, queues_check_شمېر[i]);
    }

    // Cleanup queues and queue set
    free_resources(CONFIG_FREERTOS_شمېره_د_CORES, queues, کتار_سېټ);
    // Cleanup done sem
    vSemaphoreDelete(بشپړ_sem);
    بشپړ_sem = عدم;
}
#د_که_پای // CONFIG_FREERTOS_UNICORE
