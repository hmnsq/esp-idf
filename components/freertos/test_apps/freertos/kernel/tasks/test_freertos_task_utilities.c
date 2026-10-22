/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "freertos/queue.h"
#ಅನ್ತರ್ಭಾವ "freertos/semphr.h"
#ಅನ್ತರ್ಭಾವ "freertos/stream_buffer.h"
#ಅನ್ತರ್ಭಾವ "freertos/event_groups.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "test_utils.h"

/*
Test that we can get a task's handle via the task's name using xTaskGetHandle()
*/

ಸ್ಥಿರ ಶೂನ್ಯ test_task_get_handle(ಶೂನ್ಯ *arg)
{
    vTaskSuspend(ಖಾಲಿ);
}

TEST_CASE("FreeRTOS xTaskGetHandle()", "[freertos]")
{
    vTaskDelay(10); //Small delay to let init/daemon tasks finish running

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ core = 0; core < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; core++) {
        TaskHandle_t test_task_hdl;
        TaskHandle_t ret_task_hdl;
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_ಗಮ್ಯ_Core(test_task_get_handle, "test0", 1024, ಖಾಲಿ, UNITY_FREERTOS_ಆದ್ಯತೆ + 1, &test_task_hdl, core));
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

#ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿ_LEN   1
#ವ್ಯಾಖ್ಯಾನಿಸು ಹರಿವು_ಬಫರ್_LEN   (ಗಾತ್ರ(uint32_t))

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ಸರತಿ_Handle_t ಸರತಿ;
    SemaphoreHandle_t sem;
    SemaphoreHandle_t mux;
    ಹರಿವು_ಬಫರ್_Handle_t ಹರಿವು_ಬಫರ್;
    Event_ಗುಂಪು_Handle_t evt_grp;
} abort_delay_test_obj_t;

ಸ್ಥಿರ ಶೂನ್ಯ test_task_abort_delay(ಶೂನ್ಯ *arg)
{
    abort_delay_test_obj_t *test_objs = (abort_delay_test_obj_t *)arg;

    //Block indefinitely on an empty queue. Delay should be aborted so we expect a failure to be returned
    uint32_t ದತ್ತಾಂಶ = 0;
    TEST_ASSERT_EQUAL(pdFALSE, x_ಸರತಿ_Receive(test_objs->ಸರತಿ, &ದತ್ತಾಂಶ, ಪೋರ್ಟ್_MAX_DELAY));

    TEST_ASSERT_EQUAL(pdFALSE, xSemaphoreTake(test_objs->sem, ಪೋರ್ಟ್_MAX_DELAY));

    TEST_ASSERT_EQUAL(pdFALSE, xSemaphoreTake(test_objs->mux, ಪೋರ್ಟ್_MAX_DELAY));

    uint32_t Rx_ದತ್ತಾಂಶ;
    size_t x_Received_ಬೈಟುಗಳು = x_ಹರಿವು_ಬಫರ್_Receive(test_objs->ಹರಿವು_ಬಫರ್, (ಶೂನ್ಯ *)&Rx_ದತ್ತಾಂಶ, ಹರಿವು_ಬಫರ್_LEN, ಪೋರ್ಟ್_MAX_DELAY);
    TEST_ASSERT_EQUAL(0, x_Received_ಬೈಟುಗಳು);

    Event_ಬಿಟ್ಟುಗಳು_t ux_ಬಿಟ್ಟುಗಳು = x_Event_ಗುಂಪು_Wait_ಬಿಟ್ಟುಗಳು(test_objs->evt_grp, 0xFF, pdTRUE, pdTRUE, ಪೋರ್ಟ್_MAX_DELAY);
    TEST_ASSERT_EQUAL(0, ux_ಬಿಟ್ಟುಗಳು);

    vTaskDelete(ಖಾಲಿ);
}

TEST_CASE("FreeRTOS xTaskAbortDelay()", "[freertos]")
{
    abort_delay_test_obj_t test_objs;

    test_objs.ಸರತಿ = x_ಸರತಿ_Create(ಸರತಿ_LEN, ಗಾತ್ರ(uint32_t));
    test_objs.sem = x_Semaphore_Create_ದ್ವಿಮಾನ();
    test_objs.mux = xSemaphoreCreateMutex();
    test_objs.ಹರಿವು_ಬಫರ್ = x_ಹರಿವು_ಬಫರ್_Create(ಹರಿವು_ಬಫರ್_LEN, 1);
    test_objs.evt_grp = x_Event_ಗುಂಪು_Create();

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ core = 0; core < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; core++) {
        //Take the MUX so that test task will block on it
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(test_objs.mux, 0));

        TaskHandle_t task_hdl;
        x_Task_Create_Pinned_ಗಮ್ಯ_Core(test_task_abort_delay, "test", 1024, (ಶೂನ್ಯ *)&test_objs, UNITY_FREERTOS_ಆದ್ಯತೆ + 1, &task_hdl, core);

        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 5; i++) {
            vTaskDelay(10);
            TEST_ASSERT_EQUAL(pdPASS, xTaskAbortDelay(task_hdl));
        }

        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(test_objs.mux));
    }

    vTaskDelay(10); //Delay to let IDLE task clean up
}
