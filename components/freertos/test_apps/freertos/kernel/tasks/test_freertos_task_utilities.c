/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "sdkconfig.h"
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "freertos/queue.h"
#ضمن "freertos/semphr.h"
#ضمن "freertos/stream_buffer.h"
#ضمن "freertos/event_groups.h"
#ضمن "unity.h"
#ضمن "test_utils.h"

/*
Test that we can get a task's handle via the task's name using xTaskGetHandle()
*/

ساكن فراغ test_task_get_handle(فراغ *arg)
{
    vTaskSuspend(عدم);
}

TEST_CASE("FreeRTOS xTaskGetHandle()", "[freertos]")
{
    vTaskDelay(10); //Small delay to let init/daemon tasks finish running

    لكل (صحيح core = 0; core < CONFIG_FREERTOS_عدد_از_CORES; core++) {
        TaskHandle_t test_task_hdl;
        TaskHandle_t ret_task_hdl;
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_مقصد_Core(test_task_get_handle, "test0", 1024, عدم, UNITY_FREERTOS_اولویت + 1, &test_task_hdl, core));
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

#تعریف_کن صف_LEN   1
#تعریف_کن جریان_میانگیر_LEN   (حجم(uint32_t))

تعريفنوع بنية {
    صف_Handle_t صف;
    SemaphoreHandle_t sem;
    SemaphoreHandle_t mux;
    جریان_میانگیر_Handle_t جریان_میانگیر;
    Event_گروه_Handle_t evt_grp;
} abort_delay_test_obj_t;

ساكن فراغ test_task_abort_delay(فراغ *arg)
{
    abort_delay_test_obj_t *test_objs = (abort_delay_test_obj_t *)arg;

    //Block indefinitely on an empty queue. Delay should be aborted so we expect a failure to be returned
    uint32_t داده = 0;
    TEST_ASSERT_EQUAL(pdFALSE, x_صف_Receive(test_objs->صف, &داده, پورت_MAX_DELAY));

    TEST_ASSERT_EQUAL(pdFALSE, xSemaphoreTake(test_objs->sem, پورت_MAX_DELAY));

    TEST_ASSERT_EQUAL(pdFALSE, xSemaphoreTake(test_objs->mux, پورت_MAX_DELAY));

    uint32_t Rx_داده;
    size_t x_Received_بایتها = x_جریان_میانگیر_Receive(test_objs->جریان_میانگیر, (فراغ *)&Rx_داده, جریان_میانگیر_LEN, پورت_MAX_DELAY);
    TEST_ASSERT_EQUAL(0, x_Received_بایتها);

    Event_بیتها_t ux_بیتها = x_Event_گروه_Wait_بیتها(test_objs->evt_grp, 0xFF, pdTRUE, pdTRUE, پورت_MAX_DELAY);
    TEST_ASSERT_EQUAL(0, ux_بیتها);

    vTaskDelete(عدم);
}

TEST_CASE("FreeRTOS xTaskAbortDelay()", "[freertos]")
{
    abort_delay_test_obj_t test_objs;

    test_objs.صف = x_صف_Create(صف_LEN, حجم(uint32_t));
    test_objs.sem = x_Semaphore_Create_دودویی();
    test_objs.mux = xSemaphoreCreateMutex();
    test_objs.جریان_میانگیر = x_جریان_میانگیر_Create(جریان_میانگیر_LEN, 1);
    test_objs.evt_grp = x_Event_گروه_Create();

    لكل (صحيح core = 0; core < CONFIG_FREERTOS_عدد_از_CORES; core++) {
        //Take the MUX so that test task will block on it
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(test_objs.mux, 0));

        TaskHandle_t task_hdl;
        x_Task_Create_Pinned_مقصد_Core(test_task_abort_delay, "test", 1024, (فراغ *)&test_objs, UNITY_FREERTOS_اولویت + 1, &task_hdl, core);

        لكل (صحيح i = 0; i < 5; i++) {
            vTaskDelay(10);
            TEST_ASSERT_EQUAL(pdPASS, xTaskAbortDelay(task_hdl));
        }

        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(test_objs.mux));
    }

    vTaskDelay(10); //Delay to let IDLE task clean up
}
