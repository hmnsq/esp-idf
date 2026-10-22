/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#شامل "sdkconfig.h"
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل "freertos/queue.h"
#شامل "freertos/semphr.h"
#شامل "freertos/idf_additions.h"
#شامل "unity.h"
#شامل "test_utils.h"

#تعریف_کرو قطار_LEN       4

ساکن خالی allocate_resources(صحیح عدد_queues, صحیح قطار_len, قطار_Handle_t *قطار_list_ret, قطار_سیٹ_Handle_t *قطار_سیٹ_ret)
{
    // Create queue set
    *قطار_سیٹ_ret = x_قطار_Create_سیٹ(عدد_queues * قطار_len);
    TEST_ASSERT_NOT_EQUAL(تہی, *قطار_سیٹ_ret);
    // Create queues and add them to the queue set
    برائے (صحیح i = 0; i < عدد_queues; i++) {
        قطار_list_ret[i] = x_قطار_Create(قطار_len, حجم(بنیاد_Type_t));
        TEST_ASSERT_NOT_EQUAL(تہی, قطار_list_ret[i]);
        TEST_ASSERT_EQUAL(pdPASS, x_قطار_جوڑو_مقصد_سیٹ(قطار_list_ret[i], *قطار_سیٹ_ret));
    }
}

ساکن خالی free_resources(صحیح عدد_queues, قطار_Handle_t *قطار_list, قطار_سیٹ_Handle_t قطار_سیٹ)
{
    // Remove queues form queue set and delete the queues
    برائے (صحیح i = 0; i < عدد_queues; i++) {
        TEST_ASSERT_EQUAL(pdPASS, x_قطار_Remove_From_سیٹ(قطار_list[i], قطار_سیٹ));
        v_قطار_Delete(قطار_list[i]);
    }
    v_قطار_Delete(قطار_سیٹ);
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

#تعریف_کرو عدد_QUEUES      5

TEST_CASE("Test Queue sets", "[freertos]")
{
    // Create queues and queue set
    قطار_Handle_t queues[عدد_QUEUES];
    قطار_سیٹ_Handle_t قطار_سیٹ;
    allocate_resources(عدد_QUEUES, قطار_LEN, queues, &قطار_سیٹ);

    // Fill each queue sequentially with QUEUE_LEN items
    برائے (صحیح i = 0; i < عدد_QUEUES; i++) {
        برائے (صحیح j = 0; j < قطار_LEN; j++) {
            بنیاد_Type_t شے = j;
            TEST_ASSERT_EQUAL(pdTRUE, x_قطار_Send(queues[i], &شے, 0));
        }
    }

    برائے (صحیح i = 0; i < عدد_QUEUES; i++) {
        برائے (صحیح j = 0; j < قطار_LEN; j++) {
            // Check the queue set member
            قطار_سیٹ_Member_Handle_t member = x_قطار_Select_From_سیٹ(قطار_سیٹ, 0);
            TEST_ASSERT_EQUAL(queues[i], member);
            // Check the queue's items
            بنیاد_Type_t شے;
            TEST_ASSERT_EQUAL(pdTRUE, x_قطار_Receive(member, &شے, 0));
            TEST_ASSERT_EQUAL(j, شے);
        }
    }
    // Check that there are no more members
    TEST_ASSERT_EQUAL(تہی, x_قطار_Select_From_سیٹ(قطار_سیٹ, 0));

    // Cleanup queues and queue set
    free_resources(عدد_QUEUES, queues, قطار_سیٹ);
}

#اگر_غیر_تعریف_شدہ CONFIG_FREERTOS_UNICORE
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

ساکن متلون bool آغاز_other_cores;
ساکن SemaphoreHandle_t مکمل_sem = تہی;

ساکن خالی send_func(خالی *arg)
{
    قطار_Handle_t قطار = (قطار_Handle_t)arg;
    بنیاد_Type_t core_id = x_پورٹ_Get_Core_ID();
    اگر (core_id == 0) {
        // We are core 0. Trigger the other cores to start
        آغاز_other_cores = true;
    } ورنہ {
        // Wait to be started by main core
        جب (!آغاز_other_cores) {
            ;
        }
    }

    // Fill the queue assigned to the current core
    برائے (صحیح i = 0; i < قطار_LEN; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, x_قطار_Send(قطار, &core_id, 0));
    }

    اگر (core_id != 0) {
        // Indicate completion to core 0 and self delete
        xSemaphoreGive(مکمل_sem);
        vTaskDelete(تہی);
    }
}

TEST_CASE("Test queue sets multi-core", "[freertos]")
{
    // Create done semaphore
    مکمل_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_نمبر_کا_CORES - 1, 0);
    TEST_ASSERT_NOT_EQUAL(تہی, مکمل_sem);

    // Create queues and queue set
    قطار_Handle_t queues[CONFIG_FREERTOS_نمبر_کا_CORES];
    قطار_سیٹ_Handle_t قطار_سیٹ;
    allocate_resources(CONFIG_FREERTOS_نمبر_کا_CORES, قطار_LEN, queues, &قطار_سیٹ);

    // Create tasks of the same priority for all cores except for core 0
    برائے (صحیح i = 1; i < CONFIG_FREERTOS_نمبر_کا_CORES; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_مقصد_Core(send_func, "send", 2048, (خالی *)queues[i], UNITY_FREERTOS_ترجیح, تہی, i));
    }

    // Core 0 calls send_func as well triggering the simultaneous sends from all cores
    send_func((خالی *)queues[0]);

    // Wait for all other cores to be done
    برائے (صحیح i = 1; i < CONFIG_FREERTOS_نمبر_کا_CORES; i++) {
        xSemaphoreTake(مکمل_sem, پورٹ_MAX_DELAY);
    }

    // Read queues from the queue set, then read an item from the queue
    uint32_t queues_check_گنتی[CONFIG_FREERTOS_نمبر_کا_CORES] = {0};
    قطار_سیٹ_Member_Handle_t member = x_قطار_Select_From_سیٹ(قطار_سیٹ, 0);
    جب (member != تہی) {
        // Read the core ID from the queue, check that core ID is sane
        بنیاد_Type_t core_id;
        TEST_ASSERT_EQUAL(pdTRUE, x_قطار_Receive(member, &core_id, 0));
        TEST_ASSERT_LESS_THAN(CONFIG_FREERTOS_نمبر_کا_CORES, core_id);
        queues_check_گنتی[core_id]++;

        // Get next member
        member = x_قطار_Select_From_سیٹ(قطار_سیٹ, 0);
    }
    // Check that all items from all queues have been read
    برائے (صحیح i = 0; i < CONFIG_FREERTOS_نمبر_کا_CORES; i++) {
        TEST_ASSERT_EQUAL(قطار_LEN, queues_check_گنتی[i]);
    }

    // Cleanup queues and queue set
    free_resources(CONFIG_FREERTOS_نمبر_کا_CORES, queues, قطار_سیٹ);
    // Cleanup done sem
    vSemaphoreDelete(مکمل_sem);
    مکمل_sem = تہی;
}
#اگر_ختم // CONFIG_FREERTOS_UNICORE
