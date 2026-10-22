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

    لكل (صحيح core = 0; core < CONFIG_FREERTOS_شمېره_د_CORES; core++) {
        TaskHandle_t test_task_hdl;
        TaskHandle_t ret_task_hdl;
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_منزل_Core(test_task_get_handle, "test0", 1024, عدم, UNITY_FREERTOS_لومړیتوب + 1, &test_task_hdl, core));
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

#تعریف_کړه کتار_LEN   1
#تعریف_کړه بهیر_بفر_LEN   (حجم(uint32_t))

تعريفنوع بنية {
    کتار_Handle_t کتار;
    SemaphoreHandle_t sem;
    SemaphoreHandle_t mux;
    بهیر_بفر_Handle_t بهیر_بفر;
    Event_ډله_Handle_t evt_grp;
} abort_delay_test_obj_t;

ساكن فراغ test_task_abort_delay(فراغ *arg)
{
    abort_delay_test_obj_t *test_objs = (abort_delay_test_obj_t *)arg;

    //Block indefinitely on an empty queue. Delay should be aborted so we expect a failure to be returned
    uint32_t معلومات = 0;
    TEST_ASSERT_EQUAL(pdFALSE, x_کتار_Receive(test_objs->کتار, &معلومات, پورټ_MAX_DELAY));

    TEST_ASSERT_EQUAL(pdFALSE, xSemaphoreTake(test_objs->sem, پورټ_MAX_DELAY));

    TEST_ASSERT_EQUAL(pdFALSE, xSemaphoreTake(test_objs->mux, پورټ_MAX_DELAY));

    uint32_t Rx_معلومات;
    size_t x_Received_بایټونه = x_بهیر_بفر_Receive(test_objs->بهیر_بفر, (فراغ *)&Rx_معلومات, بهیر_بفر_LEN, پورټ_MAX_DELAY);
    TEST_ASSERT_EQUAL(0, x_Received_بایټونه);

    Event_بټونه_t ux_بټونه = x_Event_ډله_Wait_بټونه(test_objs->evt_grp, 0xFF, pdTRUE, pdTRUE, پورټ_MAX_DELAY);
    TEST_ASSERT_EQUAL(0, ux_بټونه);

    vTaskDelete(عدم);
}

TEST_CASE("FreeRTOS xTaskAbortDelay()", "[freertos]")
{
    abort_delay_test_obj_t test_objs;

    test_objs.کتار = x_کتار_Create(کتار_LEN, حجم(uint32_t));
    test_objs.sem = x_Semaphore_Create_دوګونی();
    test_objs.mux = xSemaphoreCreateMutex();
    test_objs.بهیر_بفر = x_بهیر_بفر_Create(بهیر_بفر_LEN, 1);
    test_objs.evt_grp = x_Event_ډله_Create();

    لكل (صحيح core = 0; core < CONFIG_FREERTOS_شمېره_د_CORES; core++) {
        //Take the MUX so that test task will block on it
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(test_objs.mux, 0));

        TaskHandle_t task_hdl;
        x_Task_Create_Pinned_منزل_Core(test_task_abort_delay, "test", 1024, (فراغ *)&test_objs, UNITY_FREERTOS_لومړیتوب + 1, &task_hdl, core);

        لكل (صحيح i = 0; i < 5; i++) {
            vTaskDelay(10);
            TEST_ASSERT_EQUAL(pdPASS, xTaskAbortDelay(task_hdl));
        }

        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(test_objs.mux));
    }

    vTaskDelay(10); //Delay to let IDLE task clean up
}
