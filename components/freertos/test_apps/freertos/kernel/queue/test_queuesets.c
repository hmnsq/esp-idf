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

#تعریف_کرو قطار_LEN       4

ساكن فراغ allocate_resources(صحيح عدد_queues, صحيح قطار_len, قطار_Handle_t *قطار_list_ret, قطار_سیٹ_Handle_t *قطار_سیٹ_ret)
{
    // Create queue set
    *قطار_سیٹ_ret = x_قطار_Create_سیٹ(عدد_queues * قطار_len);
    TEST_ASSERT_NOT_EQUAL(عدم, *قطار_سیٹ_ret);
    // Create queues and add them to the queue set
    لكل (صحيح i = 0; i < عدد_queues; i++) {
        قطار_list_ret[i] = x_قطار_Create(قطار_len, حجم(ادھار_Type_t));
        TEST_ASSERT_NOT_EQUAL(عدم, قطار_list_ret[i]);
        TEST_ASSERT_EQUAL(pdPASS, x_قطار_جوڑو_منزل_سیٹ(قطار_list_ret[i], *قطار_سیٹ_ret));
    }
}

ساكن فراغ free_resources(صحيح عدد_queues, قطار_Handle_t *قطار_list, قطار_سیٹ_Handle_t قطار_سیٹ)
{
    // Remove queues form queue set and delete the queues
    لكل (صحيح i = 0; i < عدد_queues; i++) {
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
    لكل (صحيح i = 0; i < عدد_QUEUES; i++) {
        لكل (صحيح j = 0; j < قطار_LEN; j++) {
            ادھار_Type_t شے = j;
            TEST_ASSERT_EQUAL(pdTRUE, x_قطار_Send(queues[i], &شے, 0));
        }
    }

    لكل (صحيح i = 0; i < عدد_QUEUES; i++) {
        لكل (صحيح j = 0; j < قطار_LEN; j++) {
            // Check the queue set member
            قطار_سیٹ_Member_Handle_t member = x_قطار_Select_From_سیٹ(قطار_سیٹ, 0);
            TEST_ASSERT_EQUAL(queues[i], member);
            // Check the queue's items
            ادھار_Type_t شے;
            TEST_ASSERT_EQUAL(pdTRUE, x_قطار_Receive(member, &شے, 0));
            TEST_ASSERT_EQUAL(j, شے);
        }
    }
    // Check that there are no more members
    TEST_ASSERT_EQUAL(عدم, x_قطار_Select_From_سیٹ(قطار_سیٹ, 0));

    // Cleanup queues and queue set
    free_resources(عدد_QUEUES, queues, قطار_سیٹ);
}

#جے_بنا_تعریف CONFIG_FREERTOS_UNICORE
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
ساكن SemaphoreHandle_t پورا_sem = عدم;

ساكن فراغ send_func(فراغ *arg)
{
    قطار_Handle_t قطار = (قطار_Handle_t)arg;
    ادھار_Type_t core_id = x_پورٹ_Get_Core_ID();
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
    لكل (صحيح i = 0; i < قطار_LEN; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, x_قطار_Send(قطار, &core_id, 0));
    }

    إذا (core_id != 0) {
        // Indicate completion to core 0 and self delete
        xSemaphoreGive(پورا_sem);
        vTaskDelete(عدم);
    }
}

TEST_CASE("Test queue sets multi-core", "[freertos]")
{
    // Create done semaphore
    پورا_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_نمبر_دا_CORES - 1, 0);
    TEST_ASSERT_NOT_EQUAL(عدم, پورا_sem);

    // Create queues and queue set
    قطار_Handle_t queues[CONFIG_FREERTOS_نمبر_دا_CORES];
    قطار_سیٹ_Handle_t قطار_سیٹ;
    allocate_resources(CONFIG_FREERTOS_نمبر_دا_CORES, قطار_LEN, queues, &قطار_سیٹ);

    // Create tasks of the same priority for all cores except for core 0
    لكل (صحيح i = 1; i < CONFIG_FREERTOS_نمبر_دا_CORES; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_منزل_Core(send_func, "send", 2048, (فراغ *)queues[i], UNITY_FREERTOS_ترجیح, عدم, i));
    }

    // Core 0 calls send_func as well triggering the simultaneous sends from all cores
    send_func((فراغ *)queues[0]);

    // Wait for all other cores to be done
    لكل (صحيح i = 1; i < CONFIG_FREERTOS_نمبر_دا_CORES; i++) {
        xSemaphoreTake(پورا_sem, پورٹ_MAX_DELAY);
    }

    // Read queues from the queue set, then read an item from the queue
    uint32_t queues_check_گنتی[CONFIG_FREERTOS_نمبر_دا_CORES] = {0};
    قطار_سیٹ_Member_Handle_t member = x_قطار_Select_From_سیٹ(قطار_سیٹ, 0);
    طالما (member != عدم) {
        // Read the core ID from the queue, check that core ID is sane
        ادھار_Type_t core_id;
        TEST_ASSERT_EQUAL(pdTRUE, x_قطار_Receive(member, &core_id, 0));
        TEST_ASSERT_LESS_THAN(CONFIG_FREERTOS_نمبر_دا_CORES, core_id);
        queues_check_گنتی[core_id]++;

        // Get next member
        member = x_قطار_Select_From_سیٹ(قطار_سیٹ, 0);
    }
    // Check that all items from all queues have been read
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_نمبر_دا_CORES; i++) {
        TEST_ASSERT_EQUAL(قطار_LEN, queues_check_گنتی[i]);
    }

    // Cleanup queues and queue set
    free_resources(CONFIG_FREERTOS_نمبر_دا_CORES, queues, قطار_سیٹ);
    // Cleanup done sem
    vSemaphoreDelete(پورا_sem);
    پورا_sem = عدم;
}
#جے_انت // CONFIG_FREERTOS_UNICORE
