/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "freertos/queue.h"
#అన్తర్భావ "freertos/semphr.h"
#అన్తర్భావ "freertos/stream_buffer.h"
#అన్తర్భావ "freertos/event_groups.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "test_utils.h"

/*
Test that we can get a task's handle via the task's name using xTaskGetHandle()
*/

స్థిర శూన్య test_task_get_handle(శూన్య *arg)
{
    vTaskSuspend(ఖాళీ);
}

TEST_CASE("FreeRTOS xTaskGetHandle()", "[freertos]")
{
    vTaskDelay(10); //Small delay to let init/daemon tasks finish running

    కొరకు (పూర్ణాంకం core = 0; core < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; core++) {
        TaskHandle_t test_task_hdl;
        TaskHandle_t ret_task_hdl;
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_గమ్యం_Core(test_task_get_handle, "test0", 1024, ఖాళీ, UNITY_FREERTOS_ప్రాధాన్యత + 1, &test_task_hdl, core));
        vTaskDelay(10); //Delay to let task call vTaskSuspend()
        ret_task_hdl = xTaskGetHandle("test0");
        TEST_ASSERT_EQUAL(test_task_hdl, ret_task_hdl);
        vTaskDelete(test_task_hdl);
        vTaskDelay(10); //Delay to let IDLE task clean up
    }
}

/*
Test that a blocked task (either on a delay or an event object) can be unblocked using xTaskAbortDelay()
*/

#నిర్వచించు వరుస_LEN   1
#నిర్వచించు ప్రవాహం_బఫరు_LEN   (పరిమాణం(uint32_t))

రకనిర్వచనం నిర్మాణం {
    వరుస_Handle_t వరుస;
    SemaphoreHandle_t sem;
    SemaphoreHandle_t mux;
    ప్రవాహం_బఫరు_Handle_t ప్రవాహం_బఫరు;
    Event_సమూహం_Handle_t evt_grp;
} abort_delay_test_obj_t;

స్థిర శూన్య test_task_abort_delay(శూన్య *arg)
{
    abort_delay_test_obj_t *test_objs = (abort_delay_test_obj_t *)arg;

    //Block indefinitely on an empty queue. Delay should be aborted so we expect a failure to be returned
    uint32_t దత్తాంశం = 0;
    TEST_ASSERT_EQUAL(pdFALSE, x_వరుస_Receive(test_objs->వరుస, &దత్తాంశం, పోర్టు_MAX_DELAY));

    TEST_ASSERT_EQUAL(pdFALSE, xSemaphoreTake(test_objs->sem, పోర్టు_MAX_DELAY));

    TEST_ASSERT_EQUAL(pdFALSE, xSemaphoreTake(test_objs->mux, పోర్టు_MAX_DELAY));

    uint32_t Rx_దత్తాంశం;
    size_t x_Received_బైట్లు = x_ప్రవాహం_బఫరు_Receive(test_objs->ప్రవాహం_బఫరు, (శూన్య *)&Rx_దత్తాంశం, ప్రవాహం_బఫరు_LEN, పోర్టు_MAX_DELAY);
    TEST_ASSERT_EQUAL(0, x_Received_బైట్లు);

    Event_బిట్లు_t ux_బిట్లు = x_Event_సమూహం_Wait_బిట్లు(test_objs->evt_grp, 0xFF, pdTRUE, pdTRUE, పోర్టు_MAX_DELAY);
    TEST_ASSERT_EQUAL(0, ux_బిట్లు);

    vTaskDelete(ఖాళీ);
}

TEST_CASE("FreeRTOS xTaskAbortDelay()", "[freertos]")
{
    abort_delay_test_obj_t test_objs;

    test_objs.వరుస = x_వరుస_Create(వరుస_LEN, పరిమాణం(uint32_t));
    test_objs.sem = x_Semaphore_Create_ద్విమాన();
    test_objs.mux = xSemaphoreCreateMutex();
    test_objs.ప్రవాహం_బఫరు = x_ప్రవాహం_బఫరు_Create(ప్రవాహం_బఫరు_LEN, 1);
    test_objs.evt_grp = x_Event_సమూహం_Create();

    కొరకు (పూర్ణాంకం core = 0; core < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; core++) {
        //Take the MUX so that test task will block on it
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(test_objs.mux, 0));

        TaskHandle_t task_hdl;
        x_Task_Create_Pinned_గమ్యం_Core(test_task_abort_delay, "test", 1024, (శూన్య *)&test_objs, UNITY_FREERTOS_ప్రాధాన్యత + 1, &task_hdl, core);

        కొరకు (పూర్ణాంకం i = 0; i < 5; i++) {
            vTaskDelay(10);
            TEST_ASSERT_EQUAL(pdPASS, xTaskAbortDelay(task_hdl));
        }

        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(test_objs.mux));
    }

    vTaskDelay(10); //Delay to let IDLE task clean up
}
