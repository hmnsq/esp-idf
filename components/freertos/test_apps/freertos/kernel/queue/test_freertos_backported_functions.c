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
#تعریف_کن TMR_PERIOD_TICKS  10
#تعریف_کن TIMER_ID    0xFF
#تعریف_کن TICK_DELTA  5

ساكن StaticTimer_t timer_میانگیر;
ساكن TickType_t tmr_ideal_exp;

ساكن فراغ tmr_cb(TimerHandle_t xtimer)
{
    //Check cb is called in timely fashion
    TEST_ASSERT_UINT32_WITHIN(TICK_DELTA, tmr_ideal_exp, x_Task_Get_Tick_شمار());
}

//No need for smp test as Timer Task always runs on core 0
TEST_CASE("Test FreeRTOS backported timer functions", "[freertos]")
{
    //Create one shot static timer with period TMR_PERIOD_TICKS
    TimerHandle_t tmr_handle = xTimerCreateStatic("static_tmr", TMR_PERIOD_TICKS, pdFALSE, عدم, tmr_cb, &timer_میانگیر);
    TEST_ASSERT_EQUAL(TMR_PERIOD_TICKS, xTimerGetPeriod(tmr_handle));       //Test xTimerGetPeriod()

    v_Timer_مجموعه_Timer_ID(tmr_handle, (فراغ *)TIMER_ID);
    TEST_ASSERT_EQUAL(TIMER_ID, (uint32_t)pvTimerGetTimerID(tmr_handle));   //Test vTimerSetTimerID()

    TEST_ASSERT_EQUAL(pdTRUE, x_Timer_شروع(tmr_handle, 1));      //Start Timer
    tmr_ideal_exp = x_Task_Get_Tick_شمار() + TMR_PERIOD_TICKS;      //Calculate ideal expiration time
    vTaskDelay(2);  //Need to yield to allow daemon task to process start command, or else expiration time will be NULL

    TEST_ASSERT_UINT32_WITHIN(TICK_DELTA, tmr_ideal_exp, x_Timer_Get_Expiry_زمان(tmr_handle));    //Test xTimerGetExpiryTime()

    vTaskDelay(2 * TMR_PERIOD_TICKS);   //Delay until one shot timer has triggered
    TEST_ASSERT_EQUAL(pdPASS, xTimerDelete(tmr_handle, پورت_MAX_DELAY));     //Clean up

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

#تعریف_کن قلم_اندازه       3
#تعریف_کن NO_از_اقلام     3
#تعریف_کن DELAY_TICKS     2

ساكن Static_صف_t صف_میانگیر;       //Queues, Semaphores, and Mutex use the same queue structure
ساكن uint8_t صف_storage_area[(قلم_اندازه * NO_از_اقلام)];  //Queue storage provided in separate buffer to queue struct

TEST_CASE("Test FreeRTOS backported Queue and Semphr functions", "[freertos]")
{
    //Test static queue
    uint8_t صف_قلم_مقصد_send[قلم_اندازه];
    uint8_t صف_قلم_received[قلم_اندازه];
    لكل (صحيح i = 0; i < قلم_اندازه; i++) {
        صف_قلم_مقصد_send[i] = (0xF << i);
    }
    صف_Handle_t handle = x_صف_Create_Static(NO_از_اقلام, قلم_اندازه, (uint8_t*) &صف_storage_area, &صف_میانگیر);
    TEST_ASSERT_EQUAL(pdTRUE, x_صف_Send_مقصد_پشت(handle, &صف_قلم_مقصد_send, DELAY_TICKS));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(pdTRUE, x_صف_Receive(handle, صف_قلم_received, DELAY_TICKS));
    vTaskDelay(1);
    لكل (صحيح i = 0; i < قلم_اندازه; i++) {
        TEST_ASSERT_EQUAL(صف_قلم_مقصد_send[i], صف_قلم_received[i]);   //Check received contents are correct
    }
    v_صف_Delete(handle);   //Technically not needed as deleting static queue/semphr doesn't clear static memory

    //Test static binary semaphore
    handle = x_Semaphore_Create_دودویی_Static(&صف_میانگیر);   //Queue and Semphr handles are the same
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(handle));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(handle, DELAY_TICKS));
    vTaskDelay(1);
    vSemaphoreDelete(handle);

    //Test static counting semaphore and uxSemaphoreGetCount()
    handle = xSemaphoreCreateCountingStatic(NO_از_اقلام, 0, &صف_میانگیر);
    لكل (صحيح i = 0; i < NO_از_اقلام; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(handle));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(NO_از_اقلام, ux_Semaphore_Get_شمار(handle));    //Test uxSemaphoreGetCount()
    لكل (صحيح i = 0; i < NO_از_اقلام; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(handle, DELAY_TICKS));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(0, ux_Semaphore_Get_شمار(handle));
    vSemaphoreDelete(handle);

    //Test static mutex
    handle = xSemaphoreCreateMutexStatic(&صف_میانگیر);
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(handle, DELAY_TICKS));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR((فراغ *)xTaskGetCurrentTaskHandle(), xSemaphoreGetMutexHolder(handle));   //Current task should now hold mutex
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(handle));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR(عدم, xSemaphoreGetMutexHolder(handle));      //Mutex should have been released
    vSemaphoreDelete(handle);

    //Test static mutex recursive
    handle = xSemaphoreCreateRecursiveMutexStatic(&صف_میانگیر);
    لكل (صحيح i = 0; i < NO_از_اقلام; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTakeRecursive(handle, DELAY_TICKS));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR((فراغ *)xTaskGetCurrentTaskHandle(), xSemaphoreGetMutexHolder(handle));   //Current task should hold mutex
    لكل (صحيح i = 0; i < NO_از_اقلام; i++) {
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

#تعریف_کن پشته_اندازه  2048    //Task stack size

ساكن پشته_Type_t task_پشته[پشته_اندازه];  //Static buffer for task stack
ساكن StaticTask_t task_میانگیر;            //Static buffer for TCB
ساكن bool has_پیاپی[CONFIG_FREERTOS_عدد_از_CORES];

ساكن فراغ task(فراغ *arg)
{
    has_پیاپی[x_پورت_Get_Core_ID()] = true;       //Signify static task cb has run
    vTaskDelete(عدم);
}

TEST_CASE("Test FreeRTOS static task allocation", "[freertos]")
{
    لكل (صحيح core = 0; core < CONFIG_FREERTOS_عدد_از_CORES; core++) {
        has_پیاپی[core] = false;     //Clear has_run flag
        TaskHandle_t handle = x_Task_Create_Static_Pinned_مقصد_Core(task, "static task", پشته_اندازه, عدم,
                                                            UNITY_FREERTOS_اولویت + 1, (پشته_Type_t *)&task_پشته,
                                                            (StaticTask_t *)&task_میانگیر, core);
        vTaskDelay(5); //Allow for static task to run, delete, and idle to clean up
        TEST_ASSERT_NOT_EQUAL(عدم, handle);    //Check static task was successfully allocated
        TEST_ASSERT_TRUE(has_پیاپی[core]);    //Check static task has run
    }
}

/* ------------- Test backported static event group allocation -------------------
 * Test xEventGroupCreateStatic() but creating static event group then waiting
 * for an event.
 */

#تعریف_کن WAIT_بیتها   0x01    //Wait for first bit

ساكن Static_Event_گروه_t event_گروه;
ساكن Event_گروه_Handle_t eg_handle;

TEST_CASE("Test FreeRTOS backported eventgroup functions", "[freertos]")
{
    eg_handle = x_Event_گروه_Create_Static((Static_Event_گروه_t *)&event_گروه);
    x_Event_گروه_مجموعه_بیتها(eg_handle, WAIT_بیتها);
    TEST_ASSERT_EQUAL(WAIT_بیتها, x_Event_گروه_Wait_بیتها(eg_handle, WAIT_بیتها, pdTRUE, pdTRUE, پورت_MAX_DELAY));
    //Cleanup static event
    v_Event_گروه_Delete(eg_handle);
}
