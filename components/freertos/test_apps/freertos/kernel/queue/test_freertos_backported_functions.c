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
#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/timers.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/queue.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/semphr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/event_groups.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "test_utils.h"

/* ---------------------Test 1: Backported Timer functions-----------------------
 * Test xTimerCreateStatic(), vTimerSetTimerId(), xTimerGetPeriod(), xTimerGetExpiryTime()
 *
 * This test creates a one-shot static timer, sets/checks the timer's id and period. Then ensures
 * the timer cb is executed in a timely fashion.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TMR_PERIOD_TICKS  10
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TIMER_ID    0xFF
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TICK_DELTA  5

ਸਥਿਰ StaticTimer_t timer_ਬਫਰ;
ਸਥਿਰ TickType_t tmr_ideal_exp;

ਸਥਿਰ ਖਾਲੀ tmr_cb(TimerHandle_t xtimer)
{
    //Check cb is called in timely fashion
    TEST_ASSERT_UINT32_WITHIN(TICK_DELTA, tmr_ideal_exp, x_Task_Get_Tick_ਗਿਣਤੀ());
}

//No need for smp test as Timer Task always runs on core 0
TEST_CASE("Test FreeRTOS backported timer functions", "[freertos]")
{
    //Create one shot static timer with period TMR_PERIOD_TICKS
    TimerHandle_t tmr_handle = xTimerCreateStatic("static_tmr", TMR_PERIOD_TICKS, pdFALSE, ਨਿਰਾ, tmr_cb, &timer_ਬਫਰ);
    TEST_ASSERT_EQUAL(TMR_PERIOD_TICKS, xTimerGetPeriod(tmr_handle));       //Test xTimerGetPeriod()

    v_Timer_ਸੈੱਟ_Timer_ID(tmr_handle, (ਖਾਲੀ *)TIMER_ID);
    TEST_ASSERT_EQUAL(TIMER_ID, (uint32_t)pvTimerGetTimerID(tmr_handle));   //Test vTimerSetTimerID()

    TEST_ASSERT_EQUAL(pdTRUE, x_Timer_ਸ਼ੁਰੂ(tmr_handle, 1));      //Start Timer
    tmr_ideal_exp = x_Task_Get_Tick_ਗਿਣਤੀ() + TMR_PERIOD_TICKS;      //Calculate ideal expiration time
    vTaskDelay(2);  //Need to yield to allow daemon task to process start command, or else expiration time will be NULL

    TEST_ASSERT_UINT32_WITHIN(TICK_DELTA, tmr_ideal_exp, x_Timer_Get_Expiry_ਸਮਾਂ(tmr_handle));    //Test xTimerGetExpiryTime()

    vTaskDelay(2 * TMR_PERIOD_TICKS);   //Delay until one shot timer has triggered
    TEST_ASSERT_EQUAL(pdPASS, xTimerDelete(tmr_handle, ਪੋਰਟ_MAX_DELAY));     //Clean up

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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਵਸਤੂ_ਮਾਪ       3
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NO_ਦਾ_ਚੀਜ਼ਾਂ     3
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DELAY_TICKS     2

ਸਥਿਰ Static_ਕਤਾਰ_t ਕਤਾਰ_ਬਫਰ;       //Queues, Semaphores, and Mutex use the same queue structure
ਸਥਿਰ uint8_t ਕਤਾਰ_storage_area[(ਵਸਤੂ_ਮਾਪ * NO_ਦਾ_ਚੀਜ਼ਾਂ)];  //Queue storage provided in separate buffer to queue struct

TEST_CASE("Test FreeRTOS backported Queue and Semphr functions", "[freertos]")
{
    //Test static queue
    uint8_t ਕਤਾਰ_ਵਸਤੂ_ਮੰਜ਼ਿਲ_send[ਵਸਤੂ_ਮਾਪ];
    uint8_t ਕਤਾਰ_ਵਸਤੂ_received[ਵਸਤੂ_ਮਾਪ];
    ਲਈ (ਪੂਰਨ i = 0; i < ਵਸਤੂ_ਮਾਪ; i++) {
        ਕਤਾਰ_ਵਸਤੂ_ਮੰਜ਼ਿਲ_send[i] = (0xF << i);
    }
    ਕਤਾਰ_Handle_t handle = x_ਕਤਾਰ_Create_Static(NO_ਦਾ_ਚੀਜ਼ਾਂ, ਵਸਤੂ_ਮਾਪ, (uint8_t*) &ਕਤਾਰ_storage_area, &ਕਤਾਰ_ਬਫਰ);
    TEST_ASSERT_EQUAL(pdTRUE, x_ਕਤਾਰ_Send_ਮੰਜ਼ਿਲ_ਪਿੱਠ(handle, &ਕਤਾਰ_ਵਸਤੂ_ਮੰਜ਼ਿਲ_send, DELAY_TICKS));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(pdTRUE, x_ਕਤਾਰ_Receive(handle, ਕਤਾਰ_ਵਸਤੂ_received, DELAY_TICKS));
    vTaskDelay(1);
    ਲਈ (ਪੂਰਨ i = 0; i < ਵਸਤੂ_ਮਾਪ; i++) {
        TEST_ASSERT_EQUAL(ਕਤਾਰ_ਵਸਤੂ_ਮੰਜ਼ਿਲ_send[i], ਕਤਾਰ_ਵਸਤੂ_received[i]);   //Check received contents are correct
    }
    v_ਕਤਾਰ_Delete(handle);   //Technically not needed as deleting static queue/semphr doesn't clear static memory

    //Test static binary semaphore
    handle = x_Semaphore_Create_ਦੁਆਧਾਰੀ_Static(&ਕਤਾਰ_ਬਫਰ);   //Queue and Semphr handles are the same
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(handle));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(handle, DELAY_TICKS));
    vTaskDelay(1);
    vSemaphoreDelete(handle);

    //Test static counting semaphore and uxSemaphoreGetCount()
    handle = xSemaphoreCreateCountingStatic(NO_ਦਾ_ਚੀਜ਼ਾਂ, 0, &ਕਤਾਰ_ਬਫਰ);
    ਲਈ (ਪੂਰਨ i = 0; i < NO_ਦਾ_ਚੀਜ਼ਾਂ; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(handle));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(NO_ਦਾ_ਚੀਜ਼ਾਂ, ux_Semaphore_Get_ਗਿਣਤੀ(handle));    //Test uxSemaphoreGetCount()
    ਲਈ (ਪੂਰਨ i = 0; i < NO_ਦਾ_ਚੀਜ਼ਾਂ; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(handle, DELAY_TICKS));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(0, ux_Semaphore_Get_ਗਿਣਤੀ(handle));
    vSemaphoreDelete(handle);

    //Test static mutex
    handle = xSemaphoreCreateMutexStatic(&ਕਤਾਰ_ਬਫਰ);
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(handle, DELAY_TICKS));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR((ਖਾਲੀ *)xTaskGetCurrentTaskHandle(), xSemaphoreGetMutexHolder(handle));   //Current task should now hold mutex
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(handle));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR(ਨਿਰਾ, xSemaphoreGetMutexHolder(handle));      //Mutex should have been released
    vSemaphoreDelete(handle);

    //Test static mutex recursive
    handle = xSemaphoreCreateRecursiveMutexStatic(&ਕਤਾਰ_ਬਫਰ);
    ਲਈ (ਪੂਰਨ i = 0; i < NO_ਦਾ_ਚੀਜ਼ਾਂ; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTakeRecursive(handle, DELAY_TICKS));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR((ਖਾਲੀ *)xTaskGetCurrentTaskHandle(), xSemaphoreGetMutexHolder(handle));   //Current task should hold mutex
    ਲਈ (ਪੂਰਨ i = 0; i < NO_ਦਾ_ਚੀਜ਼ਾਂ; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGiveRecursive(handle));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR(ਨਿਰਾ, xSemaphoreGetMutexHolder(handle));      //Mutex should have been released
    vSemaphoreDelete(handle);

}

/* -----------------Test backported static task allocation  -------------------
 * Test xTaskCreateStaticPinnedToCore() but creating static task on each core
 * and checking the task cb has run successfully.
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਢੇਰ_ਮਾਪ  2048    //Task stack size

ਸਥਿਰ ਢੇਰ_Type_t task_ਢੇਰ[ਢੇਰ_ਮਾਪ];  //Static buffer for task stack
ਸਥਿਰ StaticTask_t task_ਬਫਰ;            //Static buffer for TCB
ਸਥਿਰ bool has_ਲੜੀਵਾਰ[CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES];

ਸਥਿਰ ਖਾਲੀ task(ਖਾਲੀ *arg)
{
    has_ਲੜੀਵਾਰ[x_ਪੋਰਟ_Get_Core_ID()] = true;       //Signify static task cb has run
    vTaskDelete(ਨਿਰਾ);
}

TEST_CASE("Test FreeRTOS static task allocation", "[freertos]")
{
    ਲਈ (ਪੂਰਨ core = 0; core < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; core++) {
        has_ਲੜੀਵਾਰ[core] = false;     //Clear has_run flag
        TaskHandle_t handle = x_Task_Create_Static_Pinned_ਮੰਜ਼ਿਲ_Core(task, "static task", ਢੇਰ_ਮਾਪ, ਨਿਰਾ,
                                                            UNITY_FREERTOS_ਤਰਜੀਹ + 1, (ਢੇਰ_Type_t *)&task_ਢੇਰ,
                                                            (StaticTask_t *)&task_ਬਫਰ, core);
        vTaskDelay(5); //Allow for static task to run, delete, and idle to clean up
        TEST_ASSERT_NOT_EQUAL(ਨਿਰਾ, handle);    //Check static task was successfully allocated
        TEST_ASSERT_TRUE(has_ਲੜੀਵਾਰ[core]);    //Check static task has run
    }
}

/* ------------- Test backported static event group allocation -------------------
 * Test xEventGroupCreateStatic() but creating static event group then waiting
 * for an event.
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ WAIT_ਬਿੱਟ   0x01    //Wait for first bit

ਸਥਿਰ Static_Event_ਸਮੂਹ_t event_ਸਮੂਹ;
ਸਥਿਰ Event_ਸਮੂਹ_Handle_t eg_handle;

TEST_CASE("Test FreeRTOS backported eventgroup functions", "[freertos]")
{
    eg_handle = x_Event_ਸਮੂਹ_Create_Static((Static_Event_ਸਮੂਹ_t *)&event_ਸਮੂਹ);
    x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ(eg_handle, WAIT_ਬਿੱਟ);
    TEST_ASSERT_EQUAL(WAIT_ਬਿੱਟ, x_Event_ਸਮੂਹ_Wait_ਬਿੱਟ(eg_handle, WAIT_ਬਿੱਟ, pdTRUE, pdTRUE, ਪੋਰਟ_MAX_DELAY));
    //Cleanup static event
    v_Event_ਸਮੂਹ_Delete(eg_handle);
}
