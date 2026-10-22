/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#অন্তর্ভাব "sdkconfig.h"
#অন্তর্ভাব "freertos/FreeRTOS.h"
#অন্তর্ভাব "freertos/task.h"
#অন্তর্ভাব "freertos/queue.h"
#অন্তর্ভাব "freertos/semphr.h"
#অন্তর্ভাব "freertos/stream_buffer.h"
#অন্তর্ভাব "freertos/event_groups.h"
#অন্তর্ভাব "unity.h"
#অন্তর্ভাব "test_utils.h"

/*
Test that we can get a task's handle via the task's name using xTaskGetHandle()
*/

স্থির শূন্য test_task_get_handle(শূন্য *arg)
{
    vTaskSuspend(ফাঁকা);
}

TEST_CASE("FreeRTOS xTaskGetHandle()", "[freertos]")
{
    vTaskDelay(10); //Small delay to let init/daemon tasks finish running

    জন্য (পূর্ণসংখ্যা core = 0; core < CONFIG_FREERTOS_সংখ্যা_OF_CORES; core++) {
        TaskHandle_t test_task_hdl;
        TaskHandle_t ret_task_hdl;
        TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(test_task_get_handle, "test0", 1024, ফাঁকা, UNITY_FREERTOS_PRIORITY + 1, &test_task_hdl, core));
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
#define STREAM_বাফার_LEN   (আকার(uint32_t))

প্রকারলেখ কাঠামো {
    QueueHandle_t queue;
    SemaphoreHandle_t sem;
    SemaphoreHandle_t mux;
    Stream_বাফার_Handle_t stream_বাফার;
    EventGroupHandle_t evt_grp;
} abort_delay_test_obj_t;

স্থির শূন্য test_task_abort_delay(শূন্য *arg)
{
    abort_delay_test_obj_t *test_objs = (abort_delay_test_obj_t *)arg;

    //Block indefinitely on an empty queue. Delay should be aborted so we expect a failure to be returned
    uint32_t তথ্য = 0;
    TEST_ASSERT_EQUAL(pdFALSE, xQueueReceive(test_objs->queue, &তথ্য, portMAX_DELAY));

    TEST_ASSERT_EQUAL(pdFALSE, xSemaphoreTake(test_objs->sem, portMAX_DELAY));

    TEST_ASSERT_EQUAL(pdFALSE, xSemaphoreTake(test_objs->mux, portMAX_DELAY));

    uint32_t Rx_তথ্য;
    size_t xReceivedBytes = x_Stream_বাফার_Receive(test_objs->stream_বাফার, (শূন্য *)&Rx_তথ্য, STREAM_বাফার_LEN, portMAX_DELAY);
    TEST_ASSERT_EQUAL(0, xReceivedBytes);

    EventBits_t uxBits = xEventGroupWaitBits(test_objs->evt_grp, 0xFF, pdTRUE, pdTRUE, portMAX_DELAY);
    TEST_ASSERT_EQUAL(0, uxBits);

    vTaskDelete(ফাঁকা);
}

TEST_CASE("FreeRTOS xTaskAbortDelay()", "[freertos]")
{
    abort_delay_test_obj_t test_objs;

    test_objs.queue = xQueueCreate(QUEUE_LEN, আকার(uint32_t));
    test_objs.sem = x_Semaphore_Create_দ্বিমিক();
    test_objs.mux = xSemaphoreCreateMutex();
    test_objs.stream_বাফার = x_Stream_বাফার_Create(STREAM_বাফার_LEN, 1);
    test_objs.evt_grp = xEventGroupCreate();

    জন্য (পূর্ণসংখ্যা core = 0; core < CONFIG_FREERTOS_সংখ্যা_OF_CORES; core++) {
        //Take the MUX so that test task will block on it
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(test_objs.mux, 0));

        TaskHandle_t task_hdl;
        xTaskCreatePinnedToCore(test_task_abort_delay, "test", 1024, (শূন্য *)&test_objs, UNITY_FREERTOS_PRIORITY + 1, &task_hdl, core);

        জন্য (পূর্ণসংখ্যা i = 0; i < 5; i++) {
            vTaskDelay(10);
            TEST_ASSERT_EQUAL(pdPASS, xTaskAbortDelay(task_hdl));
        }

        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(test_objs.mux));
    }

    vTaskDelay(10); //Delay to let IDLE task clean up
}
