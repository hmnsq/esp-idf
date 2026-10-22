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
#ضمن "sdkconfig.h"
#ضمن <stdio.h>
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "freertos/timers.h"
#ضمن "freertos/queue.h"
#ضمن "freertos/semphr.h"
#ضمن "freertos/event_groups.h"
#ضمن "unity.h"
#ضمن "test_utils.h"

/* ---------------------Test 1: Backported Timer functions-----------------------
 * Test xTimerCreateStatic(), vTimerSetTimerId(), xTimerGetPeriod(), xTimerGetExpiryTime()
 *
 * This test creates a one-shot static timer, sets/checks the timer's id and period. Then ensures
 * the timer cb is executed in a timely fashion.
 */
#تعریف_کړه TMR_PERIOD_TICKS  10
#تعریف_کړه TIMER_ID    0xFF
#تعریف_کړه TICK_DELTA  5

ساكن StaticTimer_t timer_بفر;
ساكن TickType_t tmr_ideal_exp;

ساكن فراغ tmr_cb(TimerHandle_t xtimer)
{
    //Check cb is called in timely fashion
    TEST_ASSERT_UINT32_WITHIN(TICK_DELTA, tmr_ideal_exp, x_Task_Get_Tick_شمېر());
}

//No need for smp test as Timer Task always runs on core 0
TEST_CASE("Test FreeRTOS backported timer functions", "[freertos]")
{
    //Create one shot static timer with period TMR_PERIOD_TICKS
    TimerHandle_t tmr_handle = xTimerCreateStatic("static_tmr", TMR_PERIOD_TICKS, pdFALSE, عدم, tmr_cb, &timer_بفر);
    TEST_ASSERT_EQUAL(TMR_PERIOD_TICKS, xTimerGetPeriod(tmr_handle));       //Test xTimerGetPeriod()

    v_Timer_سېټ_Timer_ID(tmr_handle, (فراغ *)TIMER_ID);
    TEST_ASSERT_EQUAL(TIMER_ID, (uint32_t)pvTimerGetTimerID(tmr_handle));   //Test vTimerSetTimerID()

    TEST_ASSERT_EQUAL(pdTRUE, x_Timer_پیل(tmr_handle, 1));      //Start Timer
    tmr_ideal_exp = x_Task_Get_Tick_شمېر() + TMR_PERIOD_TICKS;      //Calculate ideal expiration time
    vTaskDelay(2);  //Need to yield to allow daemon task to process start command, or else expiration time will be NULL

    TEST_ASSERT_UINT32_WITHIN(TICK_DELTA, tmr_ideal_exp, x_Timer_Get_Expiry_وخت(tmr_handle));    //Test xTimerGetExpiryTime()

    vTaskDelay(2 * TMR_PERIOD_TICKS);   //Delay until one shot timer has triggered
    TEST_ASSERT_EQUAL(pdPASS, xTimerDelete(tmr_handle, پورټ_MAX_DELAY));     //Clean up

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

#تعریف_کړه توکی_اندازه       3
#تعریف_کړه NO_د_توکي     3
#تعریف_کړه DELAY_TICKS     2

ساكن Static_کتار_t کتار_بفر;       //Queues, Semaphores, and Mutex use the same queue structure
ساكن uint8_t کتار_storage_area[(توکی_اندازه * NO_د_توکي)];  //Queue storage provided in separate buffer to queue struct

TEST_CASE("Test FreeRTOS backported Queue and Semphr functions", "[freertos]")
{
    //Test static queue
    uint8_t کتار_توکی_منزل_send[توکی_اندازه];
    uint8_t کتار_توکی_received[توکی_اندازه];
    لكل (صحيح i = 0; i < توکی_اندازه; i++) {
        کتار_توکی_منزل_send[i] = (0xF << i);
    }
    کتار_Handle_t handle = x_کتار_Create_Static(NO_د_توکي, توکی_اندازه, (uint8_t*) &کتار_storage_area, &کتار_بفر);
    TEST_ASSERT_EQUAL(pdTRUE, x_کتار_Send_منزل_شا(handle, &کتار_توکی_منزل_send, DELAY_TICKS));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(pdTRUE, x_کتار_Receive(handle, کتار_توکی_received, DELAY_TICKS));
    vTaskDelay(1);
    لكل (صحيح i = 0; i < توکی_اندازه; i++) {
        TEST_ASSERT_EQUAL(کتار_توکی_منزل_send[i], کتار_توکی_received[i]);   //Check received contents are correct
    }
    v_کتار_Delete(handle);   //Technically not needed as deleting static queue/semphr doesn't clear static memory

    //Test static binary semaphore
    handle = x_Semaphore_Create_دوګونی_Static(&کتار_بفر);   //Queue and Semphr handles are the same
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(handle));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(handle, DELAY_TICKS));
    vTaskDelay(1);
    vSemaphoreDelete(handle);

    //Test static counting semaphore and uxSemaphoreGetCount()
    handle = xSemaphoreCreateCountingStatic(NO_د_توکي, 0, &کتار_بفر);
    لكل (صحيح i = 0; i < NO_د_توکي; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(handle));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(NO_د_توکي, ux_Semaphore_Get_شمېر(handle));    //Test uxSemaphoreGetCount()
    لكل (صحيح i = 0; i < NO_د_توکي; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(handle, DELAY_TICKS));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(0, ux_Semaphore_Get_شمېر(handle));
    vSemaphoreDelete(handle);

    //Test static mutex
    handle = xSemaphoreCreateMutexStatic(&کتار_بفر);
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(handle, DELAY_TICKS));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR((فراغ *)xTaskGetCurrentTaskHandle(), xSemaphoreGetMutexHolder(handle));   //Current task should now hold mutex
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(handle));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR(عدم, xSemaphoreGetMutexHolder(handle));      //Mutex should have been released
    vSemaphoreDelete(handle);

    //Test static mutex recursive
    handle = xSemaphoreCreateRecursiveMutexStatic(&کتار_بفر);
    لكل (صحيح i = 0; i < NO_د_توکي; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTakeRecursive(handle, DELAY_TICKS));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR((فراغ *)xTaskGetCurrentTaskHandle(), xSemaphoreGetMutexHolder(handle));   //Current task should hold mutex
    لكل (صحيح i = 0; i < NO_د_توکي; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGiveRecursive(handle));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR(عدم, xSemaphoreGetMutexHolder(handle));      //Mutex should have been released
    vSemaphoreDelete(handle);

}

/* -----------------Test backported static task allocation  -------------------
 * Test xTaskCreateStaticPinnedToCore() but creating static task on each core
 * and checking the task cb has run successfully.
 */

#تعریف_کړه ډېری_اندازه  2048    //Task stack size

ساكن ډېری_Type_t task_ډېری[ډېری_اندازه];  //Static buffer for task stack
ساكن StaticTask_t task_بفر;            //Static buffer for TCB
ساكن bool has_پرلپسې[CONFIG_FREERTOS_شمېره_د_CORES];

ساكن فراغ task(فراغ *arg)
{
    has_پرلپسې[x_پورټ_Get_Core_ID()] = true;       //Signify static task cb has run
    vTaskDelete(عدم);
}

TEST_CASE("Test FreeRTOS static task allocation", "[freertos]")
{
    لكل (صحيح core = 0; core < CONFIG_FREERTOS_شمېره_د_CORES; core++) {
        has_پرلپسې[core] = false;     //Clear has_run flag
        TaskHandle_t handle = x_Task_Create_Static_Pinned_منزل_Core(task, "static task", ډېری_اندازه, عدم,
                                                            UNITY_FREERTOS_لومړیتوب + 1, (ډېری_Type_t *)&task_ډېری,
                                                            (StaticTask_t *)&task_بفر, core);
        vTaskDelay(5); //Allow for static task to run, delete, and idle to clean up
        TEST_ASSERT_NOT_EQUAL(عدم, handle);    //Check static task was successfully allocated
        TEST_ASSERT_TRUE(has_پرلپسې[core]);    //Check static task has run
    }
}

/* ------------- Test backported static event group allocation -------------------
 * Test xEventGroupCreateStatic() but creating static event group then waiting
 * for an event.
 */

#تعریف_کړه WAIT_بټونه   0x01    //Wait for first bit

ساكن Static_Event_ډله_t event_ډله;
ساكن Event_ډله_Handle_t eg_handle;

TEST_CASE("Test FreeRTOS backported eventgroup functions", "[freertos]")
{
    eg_handle = x_Event_ډله_Create_Static((Static_Event_ډله_t *)&event_ډله);
    x_Event_ډله_سېټ_بټونه(eg_handle, WAIT_بټونه);
    TEST_ASSERT_EQUAL(WAIT_بټونه, x_Event_ډله_Wait_بټونه(eg_handle, WAIT_بټونه, pdTRUE, pdTRUE, پورټ_MAX_DELAY));
    //Cleanup static event
    v_Event_ډله_Delete(eg_handle);
}
