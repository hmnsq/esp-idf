/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ଅନ୍ତର୍ଭାଵ "sdkconfig.h"
#ଅନ୍ତର୍ଭାଵ "freertos/FreeRTOS.h"
#ଅନ୍ତର୍ଭାଵ "freertos/task.h"
#ଅନ୍ତର୍ଭାଵ "freertos/queue.h"
#ଅନ୍ତର୍ଭାଵ "freertos/semphr.h"
#ଅନ୍ତର୍ଭାଵ "freertos/stream_buffer.h"
#ଅନ୍ତର୍ଭାଵ "freertos/event_groups.h"
#ଅନ୍ତର୍ଭାଵ "unity.h"
#ଅନ୍ତର୍ଭାଵ "test_utils.h"

/*
Test that we can get a task's handle via the task's name using xTaskGetHandle()
*/

ସ୍ଥିର ଶୂନ୍ୟ test_task_get_handle(ଶୂନ୍ୟ *arg)
{
    vTaskSuspend(ଖାଲି);
}

TEST_CASE("FreeRTOS xTaskGetHandle()", "[freertos]")
{
    vTaskDelay(10); //Small delay to let init/daemon tasks finish running

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା core = 0; core < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; core++) {
        TaskHandle_t test_task_hdl;
        TaskHandle_t ret_task_hdl;
        TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(test_task_get_handle, "test0", 1024, ଖାଲି, UNITY_FREERTOS_PRIORITY + 1, &test_task_hdl, core));
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

#define QUEUE_LEN   1
#define STREAM_BUFFER_LEN   (ଆକାର(uint32_t))

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    QueueHandle_t queue;
    SemaphoreHandle_t sem;
    SemaphoreHandle_t mux;
    StreamBufferHandle_t stream_buffer;
    EventGroupHandle_t evt_grp;
} abort_delay_test_obj_t;

ସ୍ଥିର ଶୂନ୍ୟ test_task_abort_delay(ଶୂନ୍ୟ *arg)
{
    abort_delay_test_obj_t *test_objs = (abort_delay_test_obj_t *)arg;

    //Block indefinitely on an empty queue. Delay should be aborted so we expect a failure to be returned
    uint32_t data = 0;
    TEST_ASSERT_EQUAL(pdFALSE, xQueueReceive(test_objs->queue, &data, portMAX_DELAY));

    TEST_ASSERT_EQUAL(pdFALSE, xSemaphoreTake(test_objs->sem, portMAX_DELAY));

    TEST_ASSERT_EQUAL(pdFALSE, xSemaphoreTake(test_objs->mux, portMAX_DELAY));

    uint32_t RxData;
    size_t xReceivedBytes = xStreamBufferReceive(test_objs->stream_buffer, (ଶୂନ୍ୟ *)&RxData, STREAM_BUFFER_LEN, portMAX_DELAY);
    TEST_ASSERT_EQUAL(0, xReceivedBytes);

    EventBits_t uxBits = xEventGroupWaitBits(test_objs->evt_grp, 0xFF, pdTRUE, pdTRUE, portMAX_DELAY);
    TEST_ASSERT_EQUAL(0, uxBits);

    vTaskDelete(ଖାଲି);
}

TEST_CASE("FreeRTOS xTaskAbortDelay()", "[freertos]")
{
    abort_delay_test_obj_t test_objs;

    test_objs.queue = xQueueCreate(QUEUE_LEN, ଆକାର(uint32_t));
    test_objs.sem = x_Semaphore_Create_ଦ୍ୱିମିକ();
    test_objs.mux = xSemaphoreCreateMutex();
    test_objs.stream_buffer = xStreamBufferCreate(STREAM_BUFFER_LEN, 1);
    test_objs.evt_grp = xEventGroupCreate();

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା core = 0; core < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; core++) {
        //Take the MUX so that test task will block on it
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(test_objs.mux, 0));

        TaskHandle_t task_hdl;
        xTaskCreatePinnedToCore(test_task_abort_delay, "test", 1024, (ଶୂନ୍ୟ *)&test_objs, UNITY_FREERTOS_PRIORITY + 1, &task_hdl, core);

        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < 5; i++) {
            vTaskDelay(10);
            TEST_ASSERT_EQUAL(pdPASS, xTaskAbortDelay(task_hdl));
        }

        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(test_objs.mux));
    }

    vTaskDelay(10); //Delay to let IDLE task clean up
}
