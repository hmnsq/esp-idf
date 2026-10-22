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

#تعریف_کن صف_LEN       4

ساكن فراغ allocate_resources(صحيح رقم_queues, صحيح صف_len, صف_Handle_t *صف_list_ret, صف_مجموعه_Handle_t *صف_مجموعه_ret)
{
    // Create queue set
    *صف_مجموعه_ret = x_صف_Create_مجموعه(رقم_queues * صف_len);
    TEST_ASSERT_NOT_EQUAL(عدم, *صف_مجموعه_ret);
    // Create queues and add them to the queue set
    لكل (صحيح i = 0; i < رقم_queues; i++) {
        صف_list_ret[i] = x_صف_Create(صف_len, حجم(پایه_Type_t));
        TEST_ASSERT_NOT_EQUAL(عدم, صف_list_ret[i]);
        TEST_ASSERT_EQUAL(pdPASS, x_صف_جمع_کن_مقصد_مجموعه(صف_list_ret[i], *صف_مجموعه_ret));
    }
}

ساكن فراغ free_resources(صحيح رقم_queues, صف_Handle_t *صف_list, صف_مجموعه_Handle_t صف_مجموعه)
{
    // Remove queues form queue set and delete the queues
    لكل (صحيح i = 0; i < رقم_queues; i++) {
        TEST_ASSERT_EQUAL(pdPASS, x_صف_Remove_From_مجموعه(صف_list[i], صف_مجموعه));
        v_صف_Delete(صف_list[i]);
    }
    v_صف_Delete(صف_مجموعه);
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

#تعریف_کن رقم_QUEUES      5

TEST_CASE("Test Queue sets", "[freertos]")
{
    // Create queues and queue set
    صف_Handle_t queues[رقم_QUEUES];
    صف_مجموعه_Handle_t صف_مجموعه;
    allocate_resources(رقم_QUEUES, صف_LEN, queues, &صف_مجموعه);

    // Fill each queue sequentially with QUEUE_LEN items
    لكل (صحيح i = 0; i < رقم_QUEUES; i++) {
        لكل (صحيح j = 0; j < صف_LEN; j++) {
            پایه_Type_t قلم = j;
            TEST_ASSERT_EQUAL(pdTRUE, x_صف_Send(queues[i], &قلم, 0));
        }
    }

    لكل (صحيح i = 0; i < رقم_QUEUES; i++) {
        لكل (صحيح j = 0; j < صف_LEN; j++) {
            // Check the queue set member
            صف_مجموعه_Member_Handle_t member = x_صف_Select_From_مجموعه(صف_مجموعه, 0);
            TEST_ASSERT_EQUAL(queues[i], member);
            // Check the queue's items
            پایه_Type_t قلم;
            TEST_ASSERT_EQUAL(pdTRUE, x_صف_Receive(member, &قلم, 0));
            TEST_ASSERT_EQUAL(j, قلم);
        }
    }
    // Check that there are no more members
    TEST_ASSERT_EQUAL(عدم, x_صف_Select_From_مجموعه(صف_مجموعه, 0));

    // Cleanup queues and queue set
    free_resources(رقم_QUEUES, queues, صف_مجموعه);
}

#اگر_تعریف_نشده CONFIG_FREERTOS_UNICORE
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

ساكن متطاير bool شروع_other_cores;
ساكن SemaphoreHandle_t انجامشده_sem = عدم;

ساكن فراغ send_func(فراغ *arg)
{
    صف_Handle_t صف = (صف_Handle_t)arg;
    پایه_Type_t core_id = x_پورت_Get_Core_ID();
    إذا (core_id == 0) {
        // We are core 0. Trigger the other cores to start
        شروع_other_cores = true;
    } وإلا {
        // Wait to be started by main core
        طالما (!شروع_other_cores) {
            ;
        }
    }

    // Fill the queue assigned to the current core
    لكل (صحيح i = 0; i < صف_LEN; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, x_صف_Send(صف, &core_id, 0));
    }

    إذا (core_id != 0) {
        // Indicate completion to core 0 and self delete
        xSemaphoreGive(انجامشده_sem);
        vTaskDelete(عدم);
    }
}

TEST_CASE("Test queue sets multi-core", "[freertos]")
{
    // Create done semaphore
    انجامشده_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_عدد_از_CORES - 1, 0);
    TEST_ASSERT_NOT_EQUAL(عدم, انجامشده_sem);

    // Create queues and queue set
    صف_Handle_t queues[CONFIG_FREERTOS_عدد_از_CORES];
    صف_مجموعه_Handle_t صف_مجموعه;
    allocate_resources(CONFIG_FREERTOS_عدد_از_CORES, صف_LEN, queues, &صف_مجموعه);

    // Create tasks of the same priority for all cores except for core 0
    لكل (صحيح i = 1; i < CONFIG_FREERTOS_عدد_از_CORES; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_مقصد_Core(send_func, "send", 2048, (فراغ *)queues[i], UNITY_FREERTOS_اولویت, عدم, i));
    }

    // Core 0 calls send_func as well triggering the simultaneous sends from all cores
    send_func((فراغ *)queues[0]);

    // Wait for all other cores to be done
    لكل (صحيح i = 1; i < CONFIG_FREERTOS_عدد_از_CORES; i++) {
        xSemaphoreTake(انجامشده_sem, پورت_MAX_DELAY);
    }

    // Read queues from the queue set, then read an item from the queue
    uint32_t queues_check_شمار[CONFIG_FREERTOS_عدد_از_CORES] = {0};
    صف_مجموعه_Member_Handle_t member = x_صف_Select_From_مجموعه(صف_مجموعه, 0);
    طالما (member != عدم) {
        // Read the core ID from the queue, check that core ID is sane
        پایه_Type_t core_id;
        TEST_ASSERT_EQUAL(pdTRUE, x_صف_Receive(member, &core_id, 0));
        TEST_ASSERT_LESS_THAN(CONFIG_FREERTOS_عدد_از_CORES, core_id);
        queues_check_شمار[core_id]++;

        // Get next member
        member = x_صف_Select_From_مجموعه(صف_مجموعه, 0);
    }
    // Check that all items from all queues have been read
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_عدد_از_CORES; i++) {
        TEST_ASSERT_EQUAL(صف_LEN, queues_check_شمار[i]);
    }

    // Cleanup queues and queue set
    free_resources(CONFIG_FREERTOS_عدد_از_CORES, queues, صف_مجموعه);
    // Cleanup done sem
    vSemaphoreDelete(انجامشده_sem);
    انجامشده_sem = عدم;
}
#پایان_اگر // CONFIG_FREERTOS_UNICORE
