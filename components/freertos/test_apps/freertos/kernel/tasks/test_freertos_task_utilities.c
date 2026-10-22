/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"
#अन्तर्भाव "freertos/queue.h"
#अन्तर्भाव "freertos/semphr.h"
#अन्तर्भाव "freertos/stream_buffer.h"
#अन्तर्भाव "freertos/event_groups.h"
#अन्तर्भाव "unity.h"
#अन्तर्भाव "test_utils.h"

/*
Test that we can get a task's handle via the task's name using xTaskGetHandle()
*/

स्थिर शून्य test_task_get_handle(शून्य *arg)
{
    vTaskSuspend(रिक्त);
}

TEST_CASE("FreeRTOS xTaskGetHandle()", "[freertos]")
{
    vTaskDelay(10); //Small delay to let init/daemon tasks finish running

    प्रति (पूर्णाङ्क core = 0; core < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES; core++) {
        TaskHandle_t test_task_hdl;
        TaskHandle_t ret_task_hdl;
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_गन्तव्यम्_Core(test_task_get_handle, "test0", 1024, रिक्त, UNITY_FREERTOS_प्राथमिकता + 1, &test_task_hdl, core));
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

#परिभाषय QUEUE_LEN   1
#परिभाषय प्रवाहः_बफर_LEN   (परिमाण(uint32_t))

प्रकारनिर्देश संरचना {
    QueueHandle_t queue;
    SemaphoreHandle_t sem;
    SemaphoreHandle_t mux;
    प्रवाहः_बफर_Handle_t प्रवाहः_बफर;
    Event_गणः_Handle_t evt_grp;
} abort_delay_test_obj_t;

स्थिर शून्य test_task_abort_delay(शून्य *arg)
{
    abort_delay_test_obj_t *test_objs = (abort_delay_test_obj_t *)arg;

    //Block indefinitely on an empty queue. Delay should be aborted so we expect a failure to be returned
    uint32_t दत्त = 0;
    TEST_ASSERT_EQUAL(pdFALSE, xQueueReceive(test_objs->queue, &दत्त, पत्तनम्_MAX_DELAY));

    TEST_ASSERT_EQUAL(pdFALSE, xSemaphoreTake(test_objs->sem, पत्तनम्_MAX_DELAY));

    TEST_ASSERT_EQUAL(pdFALSE, xSemaphoreTake(test_objs->mux, पत्तनम्_MAX_DELAY));

    uint32_t Rx_दत्त;
    size_t x_Received_अष्टकानि = x_प्रवाहः_बफर_Receive(test_objs->प्रवाहः_बफर, (शून्य *)&Rx_दत्त, प्रवाहः_बफर_LEN, पत्तनम्_MAX_DELAY);
    TEST_ASSERT_EQUAL(0, x_Received_अष्टकानि);

    Event_द्वयङ्काः_t ux_द्वयङ्काः = x_Event_गणः_Wait_द्वयङ्काः(test_objs->evt_grp, 0xFF, pdTRUE, pdTRUE, पत्तनम्_MAX_DELAY);
    TEST_ASSERT_EQUAL(0, ux_द्वयङ्काः);

    vTaskDelete(रिक्त);
}

TEST_CASE("FreeRTOS xTaskAbortDelay()", "[freertos]")
{
    abort_delay_test_obj_t test_objs;

    test_objs.queue = xQueueCreate(QUEUE_LEN, परिमाण(uint32_t));
    test_objs.sem = x_Semaphore_Create_द्विमान();
    test_objs.mux = xSemaphoreCreateMutex();
    test_objs.प्रवाहः_बफर = x_प्रवाहः_बफर_Create(प्रवाहः_बफर_LEN, 1);
    test_objs.evt_grp = x_Event_गणः_Create();

    प्रति (पूर्णाङ्क core = 0; core < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES; core++) {
        //Take the MUX so that test task will block on it
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(test_objs.mux, 0));

        TaskHandle_t task_hdl;
        x_Task_Create_Pinned_गन्तव्यम्_Core(test_task_abort_delay, "test", 1024, (शून्य *)&test_objs, UNITY_FREERTOS_प्राथमिकता + 1, &task_hdl, core);

        प्रति (पूर्णाङ्क i = 0; i < 5; i++) {
            vTaskDelay(10);
            TEST_ASSERT_EQUAL(pdPASS, xTaskAbortDelay(task_hdl));
        }

        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(test_objs.mux));
    }

    vTaskDelay(10); //Delay to let IDLE task clean up
}
