/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#समावेश "sdkconfig.h"
#समावेश "freertos/FreeRTOS.h"
#समावेश "freertos/task.h"
#समावेश "freertos/queue.h"
#समावेश "freertos/semphr.h"
#समावेश "freertos/stream_buffer.h"
#समावेश "freertos/event_groups.h"
#समावेश "unity.h"
#समावेश "test_utils.h"

/*
Test that we can get a task's handle via the task's name using xTaskGetHandle()
*/

अटल व्योम test_task_get_handle(व्योम *arg)
{
    vTaskSuspend(शून्य);
}

TEST_CASE("FreeRTOS xTaskGetHandle()", "[freertos]")
{
    vTaskDelay(10); //Small delay to let init/daemon tasks finish running

    क्रम (पूर्णांक core = 0; core < CONFIG_FREERTOS_संख्या_OF_CORES; core++) {
        TaskHandle_t test_task_hdl;
        TaskHandle_t ret_task_hdl;
        TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(test_task_get_handle, "test0", 1024, शून्य, UNITY_FREERTOS_PRIORITY + 1, &test_task_hdl, core));
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

#घोषणा QUEUE_LEN   1
#घोषणा STREAM_बफर_LEN   (माप(uint32_t))

प्रकार काष्ठा {
    QueueHandle_t queue;
    SemaphoreHandle_t sem;
    SemaphoreHandle_t mux;
    Stream_बफर_Handle_t stream_बफर;
    EventGroupHandle_t evt_grp;
} abort_delay_test_obj_t;

अटल व्योम test_task_abort_delay(व्योम *arg)
{
    abort_delay_test_obj_t *test_objs = (abort_delay_test_obj_t *)arg;

    //Block indefinitely on an empty queue. Delay should be aborted so we expect a failure to be returned
    uint32_t तथ्य = 0;
    TEST_ASSERT_EQUAL(pdFALSE, xQueueReceive(test_objs->queue, &तथ्य, portMAX_DELAY));

    TEST_ASSERT_EQUAL(pdFALSE, xSemaphoreTake(test_objs->sem, portMAX_DELAY));

    TEST_ASSERT_EQUAL(pdFALSE, xSemaphoreTake(test_objs->mux, portMAX_DELAY));

    uint32_t Rx_तथ्य;
    माप_प्रकार xReceivedBytes = x_Stream_बफर_Receive(test_objs->stream_बफर, (व्योम *)&Rx_तथ्य, STREAM_बफर_LEN, portMAX_DELAY);
    TEST_ASSERT_EQUAL(0, xReceivedBytes);

    EventBits_t uxBits = xEventGroupWaitBits(test_objs->evt_grp, 0xFF, pdTRUE, pdTRUE, portMAX_DELAY);
    TEST_ASSERT_EQUAL(0, uxBits);

    vTaskDelete(शून्य);
}

TEST_CASE("FreeRTOS xTaskAbortDelay()", "[freertos]")
{
    abort_delay_test_obj_t test_objs;

    test_objs.queue = xQueueCreate(QUEUE_LEN, माप(uint32_t));
    test_objs.sem = x_Semaphore_Create_द्विआधारी();
    test_objs.mux = xSemaphoreCreateMutex();
    test_objs.stream_बफर = x_Stream_बफर_Create(STREAM_बफर_LEN, 1);
    test_objs.evt_grp = xEventGroupCreate();

    क्रम (पूर्णांक core = 0; core < CONFIG_FREERTOS_संख्या_OF_CORES; core++) {
        //Take the MUX so that test task will block on it
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(test_objs.mux, 0));

        TaskHandle_t task_hdl;
        xTaskCreatePinnedToCore(test_task_abort_delay, "test", 1024, (व्योम *)&test_objs, UNITY_FREERTOS_PRIORITY + 1, &task_hdl, core);

        क्रम (पूर्णांक i = 0; i < 5; i++) {
            vTaskDelay(10);
            TEST_ASSERT_EQUAL(pdPASS, xTaskAbortDelay(task_hdl));
        }

        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(test_objs.mux));
    }

    vTaskDelay(10); //Delay to let IDLE task clean up
}
