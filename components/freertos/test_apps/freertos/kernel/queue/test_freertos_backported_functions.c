/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * Test features that are backported from version FreeRTOS 9.0.0.
 *
 * 1) Test backported timer functions
 *      - xTimerCreateStatic(), vTimerSetTimerId(), xTimerGetPeriod(), xTimerGetExpiryTime()
 * 2) Test backported queue/semaphore functions
 *      - xQueueCreateStatic()
 *      - xSemaphoreCreateBinaryStatic(), xSemaphoreCreateCountingStatic(), uxSemaphoreGetCount()
 *      - xSemaphoreCreateMutexStatic(), xSemaphoreCreateRecursiveMutexStatic()
 * 3) Test static allocation of tasks
 *      - xTaskCreateStaticPinnedToCore()
 * 4) Test static allocation of event group
 *      - xEventGroupCreateStatic()
 * 5) Test Thread Local Storage Pointers and Deletion Callbacks
 *      - vTaskSetThreadLocalStoragePointerAndDelCallback()
 *      - pvTaskGetThreadLocalStoragePointer()
 *
 * Note: The *pcQueueGetName() function is also backported, but is not tested in
 *       the following test cases (see Queue Registry test cases instead)
 * For more details please refer the the ESP-IDF FreeRTOS changes documentation
 */
#समावेश "sdkconfig.h"
#समावेश <stdio.h>
#समावेश "freertos/FreeRTOS.h"
#समावेश "freertos/task.h"
#समावेश "freertos/timers.h"
#समावेश "freertos/queue.h"
#समावेश "freertos/semphr.h"
#समावेश "freertos/event_groups.h"
#समावेश "unity.h"
#समावेश "test_utils.h"

/* ---------------------Test 1: Backported Timer functions-----------------------
 * Test xTimerCreateStatic(), vTimerSetTimerId(), xTimerGetPeriod(), xTimerGetExpiryTime()
 *
 * This test creates a one-shot static timer, sets/checks the timer's id and period. Then ensures
 * the timer cb is executed in a timely fashion.
 */
#घोषणा TMR_PERIOD_TICKS  10
#घोषणा TIMER_ID    0xFF
#घोषणा TICK_DELTA  5

अटल StaticTimer_t timer_बफर;
अटल TickType_t tmr_ideal_exp;

अटल व्योम tmr_cb(TimerHandle_t xtimer)
{
    //Check cb is called in timely fashion
    TEST_ASSERT_UINT32_WITHIN(TICK_DELTA, tmr_ideal_exp, x_Task_Get_Tick_गिनती());
}

//No need for smp test as Timer Task always runs on core 0
TEST_CASE("Test FreeRTOS backported timer functions", "[freertos]")
{
    //Create one shot static timer with period TMR_PERIOD_TICKS
    TimerHandle_t tmr_handle = xTimerCreateStatic("static_tmr", TMR_PERIOD_TICKS, pdFALSE, शून्य, tmr_cb, &timer_बफर);
    TEST_ASSERT_EQUAL(TMR_PERIOD_TICKS, xTimerGetPeriod(tmr_handle));       //Test xTimerGetPeriod()

    vTimerSetTimerID(tmr_handle, (व्योम *)TIMER_ID);
    TEST_ASSERT_EQUAL(TIMER_ID, (uint32_t)pvTimerGetTimerID(tmr_handle));   //Test vTimerSetTimerID()

    TEST_ASSERT_EQUAL(pdTRUE, xTimerStart(tmr_handle, 1));      //Start Timer
    tmr_ideal_exp = x_Task_Get_Tick_गिनती() + TMR_PERIOD_TICKS;      //Calculate ideal expiration time
    vTaskDelay(2);  //Need to yield to allow daemon task to process start command, or else expiration time will be NULL

    TEST_ASSERT_UINT32_WITHIN(TICK_DELTA, tmr_ideal_exp, xTimerGetExpiryTime(tmr_handle));    //Test xTimerGetExpiryTime()

    vTaskDelay(2 * TMR_PERIOD_TICKS);   //Delay until one shot timer has triggered
    TEST_ASSERT_EQUAL(pdPASS, xTimerDelete(tmr_handle, portMAX_DELAY));     //Clean up

}

/* ---------------Test backported queue/semaphore functions-------------------
 * xQueueCreateStatic()
 * xSemaphoreCreateBinaryStatic(), xSemaphoreCreateCountingStatic()
 * xSemaphoreCreateMutexStatic(), xSemaphoreCreateRecursiveMutexStatic()
 * uxSemaphoreGetCount() is also tested on the static counting semaphore
 *
 * This test creates various static queue/semphrs listed above and tests them by
 * doing a simple send/give and rec/take.
 */

#घोषणा ITEM_आकार       3
#घोषणा NO_OF_वस्तुएँ     3
#घोषणा DELAY_TICKS     2

अटल StaticQueue_t queue_बफर;       //Queues, Semaphores, and Mutex use the same queue structure
अटल uint8_t queue_storage_area[(ITEM_आकार * NO_OF_वस्तुएँ)];  //Queue storage provided in separate buffer to queue struct

TEST_CASE("Test FreeRTOS backported Queue and Semphr functions", "[freertos]")
{
    //Test static queue
    uint8_t queue_item_to_send[ITEM_आकार];
    uint8_t queue_item_received[ITEM_आकार];
    क्रम (पूर्णांक i = 0; i < ITEM_आकार; i++) {
        queue_item_to_send[i] = (0xF << i);
    }
    QueueHandle_t handle = xQueueCreateStatic(NO_OF_वस्तुएँ, ITEM_आकार, (uint8_t*) &queue_storage_area, &queue_बफर);
    TEST_ASSERT_EQUAL(pdTRUE, xQueueSendToBack(handle, &queue_item_to_send, DELAY_TICKS));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(pdTRUE, xQueueReceive(handle, queue_item_received, DELAY_TICKS));
    vTaskDelay(1);
    क्रम (पूर्णांक i = 0; i < ITEM_आकार; i++) {
        TEST_ASSERT_EQUAL(queue_item_to_send[i], queue_item_received[i]);   //Check received contents are correct
    }
    vQueueDelete(handle);   //Technically not needed as deleting static queue/semphr doesn't clear static memory

    //Test static binary semaphore
    handle = x_Semaphore_Create_द्विआधारी_Static(&queue_बफर);   //Queue and Semphr handles are the same
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(handle));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(handle, DELAY_TICKS));
    vTaskDelay(1);
    vSemaphoreDelete(handle);

    //Test static counting semaphore and uxSemaphoreGetCount()
    handle = xSemaphoreCreateCountingStatic(NO_OF_वस्तुएँ, 0, &queue_बफर);
    क्रम (पूर्णांक i = 0; i < NO_OF_वस्तुएँ; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(handle));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(NO_OF_वस्तुएँ, ux_Semaphore_Get_गिनती(handle));    //Test uxSemaphoreGetCount()
    क्रम (पूर्णांक i = 0; i < NO_OF_वस्तुएँ; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(handle, DELAY_TICKS));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(0, ux_Semaphore_Get_गिनती(handle));
    vSemaphoreDelete(handle);

    //Test static mutex
    handle = xSemaphoreCreateMutexStatic(&queue_बफर);
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(handle, DELAY_TICKS));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR((व्योम *)xTaskGetCurrentTaskHandle(), xSemaphoreGetMutexHolder(handle));   //Current task should now hold mutex
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(handle));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR(शून्य, xSemaphoreGetMutexHolder(handle));      //Mutex should have been released
    vSemaphoreDelete(handle);

    //Test static mutex recursive
    handle = xSemaphoreCreateRecursiveMutexStatic(&queue_बफर);
    क्रम (पूर्णांक i = 0; i < NO_OF_वस्तुएँ; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTakeRecursive(handle, DELAY_TICKS));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR((व्योम *)xTaskGetCurrentTaskHandle(), xSemaphoreGetMutexHolder(handle));   //Current task should hold mutex
    क्रम (पूर्णांक i = 0; i < NO_OF_वस्तुएँ; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGiveRecursive(handle));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR(शून्य, xSemaphoreGetMutexHolder(handle));      //Mutex should have been released
    vSemaphoreDelete(handle);

}

/* -----------------Test backported static task allocation  -------------------
 * Test xTaskCreateStaticPinnedToCore() but creating static task on each core
 * and checking the task cb has run successfully.
 */

#घोषणा ढेर_आकार  2048    //Task stack size

अटल ढेर_Type_t task_ढेर[ढेर_आकार];  //Static buffer for task stack
अटल StaticTask_t task_बफर;            //Static buffer for TCB
अटल bool has_run[CONFIG_FREERTOS_संख्या_OF_CORES];

अटल व्योम task(व्योम *arg)
{
    has_run[xPortGetCoreID()] = true;       //Signify static task cb has run
    vTaskDelete(शून्य);
}

TEST_CASE("Test FreeRTOS static task allocation", "[freertos]")
{
    क्रम (पूर्णांक core = 0; core < CONFIG_FREERTOS_संख्या_OF_CORES; core++) {
        has_run[core] = false;     //Clear has_run flag
        TaskHandle_t handle = xTaskCreateStaticPinnedToCore(task, "static task", ढेर_आकार, शून्य,
                                                            UNITY_FREERTOS_PRIORITY + 1, (ढेर_Type_t *)&task_ढेर,
                                                            (StaticTask_t *)&task_बफर, core);
        vTaskDelay(5); //Allow for static task to run, delete, and idle to clean up
        TEST_ASSERT_NOT_EQUAL(शून्य, handle);    //Check static task was successfully allocated
        TEST_ASSERT_TRUE(has_run[core]);    //Check static task has run
    }
}

/* ------------- Test backported static event group allocation -------------------
 * Test xEventGroupCreateStatic() but creating static event group then waiting
 * for an event.
 */

#घोषणा WAIT_BITS   0x01    //Wait for first bit

अटल StaticEventGroup_t event_group;
अटल EventGroupHandle_t eg_handle;

TEST_CASE("Test FreeRTOS backported eventgroup functions", "[freertos]")
{
    eg_handle = xEventGroupCreateStatic((StaticEventGroup_t *)&event_group);
    xEventGroupSetBits(eg_handle, WAIT_BITS);
    TEST_ASSERT_EQUAL(WAIT_BITS, xEventGroupWaitBits(eg_handle, WAIT_BITS, pdTRUE, pdTRUE, portMAX_DELAY));
    //Cleanup static event
    vEventGroupDelete(eg_handle);
}
