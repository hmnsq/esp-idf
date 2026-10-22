/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/queue.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/semphr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/stream_buffer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/event_groups.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "test_utils.h"

/*
Test that we can get a task's handle via the task's name using xTaskGetHandle()
*/

ਸਥਿਰ ਖਾਲੀ test_task_get_handle(ਖਾਲੀ *arg)
{
    vTaskSuspend(ਨਿਰਾ);
}

TEST_CASE("FreeRTOS xTaskGetHandle()", "[freertos]")
{
    vTaskDelay(10); //Small delay to let init/daemon tasks finish running

    ਲਈ (ਪੂਰਨ core = 0; core < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; core++) {
        TaskHandle_t test_task_hdl;
        TaskHandle_t ret_task_hdl;
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(test_task_get_handle, "test0", 1024, ਨਿਰਾ, UNITY_FREERTOS_ਤਰਜੀਹ + 1, &test_task_hdl, core));
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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_LEN   1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਧਾਰਾ_ਬਫਰ_LEN   (ਆਕਾਰ(uint32_t))

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ਕਤਾਰ_Handle_t ਕਤਾਰ;
    SemaphoreHandle_t sem;
    SemaphoreHandle_t mux;
    ਧਾਰਾ_ਬਫਰ_Handle_t ਧਾਰਾ_ਬਫਰ;
    Event_ਸਮੂਹ_Handle_t evt_grp;
} abort_delay_test_obj_t;

ਸਥਿਰ ਖਾਲੀ test_task_abort_delay(ਖਾਲੀ *arg)
{
    abort_delay_test_obj_t *test_objs = (abort_delay_test_obj_t *)arg;

    //Block indefinitely on an empty queue. Delay should be aborted so we expect a failure to be returned
    uint32_t ਅੰਕੜੇ = 0;
    TEST_ASSERT_EQUAL(pdFALSE, x_ਕਤਾਰ_Receive(test_objs->ਕਤਾਰ, &ਅੰਕੜੇ, ਪੋਰਟ_MAX_DELAY));

    TEST_ASSERT_EQUAL(pdFALSE, xSemaphoreTake(test_objs->sem, ਪੋਰਟ_MAX_DELAY));

    TEST_ASSERT_EQUAL(pdFALSE, xSemaphoreTake(test_objs->mux, ਪੋਰਟ_MAX_DELAY));

    uint32_t Rx_ਅੰਕੜੇ;
    size_t x_Received_ਬਾਈਟਾਂ = x_ਧਾਰਾ_ਬਫਰ_Receive(test_objs->ਧਾਰਾ_ਬਫਰ, (ਖਾਲੀ *)&Rx_ਅੰਕੜੇ, ਧਾਰਾ_ਬਫਰ_LEN, ਪੋਰਟ_MAX_DELAY);
    TEST_ASSERT_EQUAL(0, x_Received_ਬਾਈਟਾਂ);

    Event_ਬਿੱਟ_t ux_ਬਿੱਟ = x_Event_ਸਮੂਹ_Wait_ਬਿੱਟ(test_objs->evt_grp, 0xFF, pdTRUE, pdTRUE, ਪੋਰਟ_MAX_DELAY);
    TEST_ASSERT_EQUAL(0, ux_ਬਿੱਟ);

    vTaskDelete(ਨਿਰਾ);
}

TEST_CASE("FreeRTOS xTaskAbortDelay()", "[freertos]")
{
    abort_delay_test_obj_t test_objs;

    test_objs.ਕਤਾਰ = x_ਕਤਾਰ_Create(ਕਤਾਰ_LEN, ਆਕਾਰ(uint32_t));
    test_objs.sem = x_Semaphore_Create_ਦੁਆਧਾਰੀ();
    test_objs.mux = xSemaphoreCreateMutex();
    test_objs.ਧਾਰਾ_ਬਫਰ = x_ਧਾਰਾ_ਬਫਰ_Create(ਧਾਰਾ_ਬਫਰ_LEN, 1);
    test_objs.evt_grp = x_Event_ਸਮੂਹ_Create();

    ਲਈ (ਪੂਰਨ core = 0; core < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; core++) {
        //Take the MUX so that test task will block on it
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(test_objs.mux, 0));

        TaskHandle_t task_hdl;
        x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(test_task_abort_delay, "test", 1024, (ਖਾਲੀ *)&test_objs, UNITY_FREERTOS_ਤਰਜੀਹ + 1, &task_hdl, core);

        ਲਈ (ਪੂਰਨ i = 0; i < 5; i++) {
            vTaskDelay(10);
            TEST_ASSERT_EQUAL(pdPASS, xTaskAbortDelay(task_hdl));
        }

        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(test_objs.mux));
    }

    vTaskDelay(10); //Delay to let IDLE task clean up
}
