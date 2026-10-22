/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "freertos/queue.h"
#అన్తర్భావ "freertos/semphr.h"
#అన్తర్భావ "freertos/idf_additions.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "test_utils.h"

#నిర్వచించు వరుస_LEN       4

స్థిర శూన్య allocate_resources(పూర్ణాంకం అంకె_queues, పూర్ణాంకం వరుస_len, వరుస_Handle_t *వరుస_list_ret, వరుస_సమితి_Handle_t *వరుస_సమితి_ret)
{
    // Create queue set
    *వరుస_సమితి_ret = x_వరుస_Create_సమితి(అంకె_queues * వరుస_len);
    TEST_ASSERT_NOT_EQUAL(ఖాళీ, *వరుస_సమితి_ret);
    // Create queues and add them to the queue set
    కొరకు (పూర్ణాంకం i = 0; i < అంకె_queues; i++) {
        వరుస_list_ret[i] = x_వరుస_Create(వరుస_len, పరిమాణం(పునాది_Type_t));
        TEST_ASSERT_NOT_EQUAL(ఖాళీ, వరుస_list_ret[i]);
        TEST_ASSERT_EQUAL(pdPASS, x_వరుస_కూడు_గమ్యం_సమితి(వరుస_list_ret[i], *వరుస_సమితి_ret));
    }
}

స్థిర శూన్య free_resources(పూర్ణాంకం అంకె_queues, వరుస_Handle_t *వరుస_list, వరుస_సమితి_Handle_t వరుస_సమితి)
{
    // Remove queues form queue set and delete the queues
    కొరకు (పూర్ణాంకం i = 0; i < అంకె_queues; i++) {
        TEST_ASSERT_EQUAL(pdPASS, x_వరుస_Remove_From_సమితి(వరుస_list[i], వరుస_సమితి));
        v_వరుస_Delete(వరుస_list[i]);
    }
    v_వరుస_Delete(వరుస_సమితి);
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

#నిర్వచించు అంకె_QUEUES      5

TEST_CASE("Test Queue sets", "[freertos]")
{
    // Create queues and queue set
    వరుస_Handle_t queues[అంకె_QUEUES];
    వరుస_సమితి_Handle_t వరుస_సమితి;
    allocate_resources(అంకె_QUEUES, వరుస_LEN, queues, &వరుస_సమితి);

    // Fill each queue sequentially with QUEUE_LEN items
    కొరకు (పూర్ణాంకం i = 0; i < అంకె_QUEUES; i++) {
        కొరకు (పూర్ణాంకం j = 0; j < వరుస_LEN; j++) {
            పునాది_Type_t అంశం = j;
            TEST_ASSERT_EQUAL(pdTRUE, x_వరుస_Send(queues[i], &అంశం, 0));
        }
    }

    కొరకు (పూర్ణాంకం i = 0; i < అంకె_QUEUES; i++) {
        కొరకు (పూర్ణాంకం j = 0; j < వరుస_LEN; j++) {
            // Check the queue set member
            వరుస_సమితి_Member_Handle_t member = x_వరుస_Select_From_సమితి(వరుస_సమితి, 0);
            TEST_ASSERT_EQUAL(queues[i], member);
            // Check the queue's items
            పునాది_Type_t అంశం;
            TEST_ASSERT_EQUAL(pdTRUE, x_వరుస_Receive(member, &అంశం, 0));
            TEST_ASSERT_EQUAL(j, అంశం);
        }
    }
    // Check that there are no more members
    TEST_ASSERT_EQUAL(ఖాళీ, x_వరుస_Select_From_సమితి(వరుస_సమితి, 0));

    // Cleanup queues and queue set
    free_resources(అంకె_QUEUES, queues, వరుస_సమితి);
}

#నిర్వచితం_కాకపోతే CONFIG_FREERTOS_UNICORE
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

స్థిర అస్థిర bool ప్రారంభం_other_cores;
స్థిర SemaphoreHandle_t పూర్తి_sem = ఖాళీ;

స్థిర శూన్య send_func(శూన్య *arg)
{
    వరుస_Handle_t వరుస = (వరుస_Handle_t)arg;
    పునాది_Type_t core_id = x_పోర్టు_Get_Core_ID();
    ఒకవేళ (core_id == 0) {
        // We are core 0. Trigger the other cores to start
        ప్రారంభం_other_cores = true;
    } లేకపోతే {
        // Wait to be started by main core
        అయితే (!ప్రారంభం_other_cores) {
            ;
        }
    }

    // Fill the queue assigned to the current core
    కొరకు (పూర్ణాంకం i = 0; i < వరుస_LEN; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, x_వరుస_Send(వరుస, &core_id, 0));
    }

    ఒకవేళ (core_id != 0) {
        // Indicate completion to core 0 and self delete
        xSemaphoreGive(పూర్తి_sem);
        vTaskDelete(ఖాళీ);
    }
}

TEST_CASE("Test queue sets multi-core", "[freertos]")
{
    // Create done semaphore
    పూర్తి_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES - 1, 0);
    TEST_ASSERT_NOT_EQUAL(ఖాళీ, పూర్తి_sem);

    // Create queues and queue set
    వరుస_Handle_t queues[CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES];
    వరుస_సమితి_Handle_t వరుస_సమితి;
    allocate_resources(CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES, వరుస_LEN, queues, &వరుస_సమితి);

    // Create tasks of the same priority for all cores except for core 0
    కొరకు (పూర్ణాంకం i = 1; i < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_గమ్యం_Core(send_func, "send", 2048, (శూన్య *)queues[i], UNITY_FREERTOS_ప్రాధాన్యత, ఖాళీ, i));
    }

    // Core 0 calls send_func as well triggering the simultaneous sends from all cores
    send_func((శూన్య *)queues[0]);

    // Wait for all other cores to be done
    కొరకు (పూర్ణాంకం i = 1; i < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; i++) {
        xSemaphoreTake(పూర్తి_sem, పోర్టు_MAX_DELAY);
    }

    // Read queues from the queue set, then read an item from the queue
    uint32_t queues_check_లెక్క[CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES] = {0};
    వరుస_సమితి_Member_Handle_t member = x_వరుస_Select_From_సమితి(వరుస_సమితి, 0);
    అయితే (member != ఖాళీ) {
        // Read the core ID from the queue, check that core ID is sane
        పునాది_Type_t core_id;
        TEST_ASSERT_EQUAL(pdTRUE, x_వరుస_Receive(member, &core_id, 0));
        TEST_ASSERT_LESS_THAN(CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES, core_id);
        queues_check_లెక్క[core_id]++;

        // Get next member
        member = x_వరుస_Select_From_సమితి(వరుస_సమితి, 0);
    }
    // Check that all items from all queues have been read
    కొరకు (పూర్ణాంకం i = 0; i < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; i++) {
        TEST_ASSERT_EQUAL(వరుస_LEN, queues_check_లెక్క[i]);
    }

    // Cleanup queues and queue set
    free_resources(CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES, queues, వరుస_సమితి);
    // Cleanup done sem
    vSemaphoreDelete(పూర్తి_sem);
    పూర్తి_sem = ఖాళీ;
}
#అయితే_ముగింపు // CONFIG_FREERTOS_UNICORE
