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
#تعریف_کرو TMR_PERIOD_TICKS  10
#تعریف_کرو TIMER_ID    0xFF
#تعریف_کرو TICK_DELTA  5

ساكن StaticTimer_t timer_بفر;
ساكن TickType_t tmr_ideal_exp;

ساكن فراغ tmr_cb(TimerHandle_t xtimer)
{
    //Check cb is called in timely fashion
    TEST_ASSERT_UINT32_WITHIN(TICK_DELTA, tmr_ideal_exp, x_Task_Get_Tick_گنتی());
}

//No need for smp test as Timer Task always runs on core 0
TEST_CASE("Test FreeRTOS backported timer functions", "[freertos]")
{
    //Create one shot static timer with period TMR_PERIOD_TICKS
    TimerHandle_t tmr_handle = xTimerCreateStatic("static_tmr", TMR_PERIOD_TICKS, pdFALSE, عدم, tmr_cb, &timer_بفر);
    TEST_ASSERT_EQUAL(TMR_PERIOD_TICKS, xTimerGetPeriod(tmr_handle));       //Test xTimerGetPeriod()

    v_Timer_سیٹ_Timer_ID(tmr_handle, (فراغ *)TIMER_ID);
    TEST_ASSERT_EQUAL(TIMER_ID, (uint32_t)pvTimerGetTimerID(tmr_handle));   //Test vTimerSetTimerID()

    TEST_ASSERT_EQUAL(pdTRUE, x_Timer_شروع(tmr_handle, 1));      //Start Timer
    tmr_ideal_exp = x_Task_Get_Tick_گنتی() + TMR_PERIOD_TICKS;      //Calculate ideal expiration time
    vTaskDelay(2);  //Need to yield to allow daemon task to process start command, or else expiration time will be NULL

    TEST_ASSERT_UINT32_WITHIN(TICK_DELTA, tmr_ideal_exp, x_Timer_Get_Expiry_ویلا(tmr_handle));    //Test xTimerGetExpiryTime()

    vTaskDelay(2 * TMR_PERIOD_TICKS);   //Delay until one shot timer has triggered
    TEST_ASSERT_EQUAL(pdPASS, xTimerDelete(tmr_handle, پورٹ_MAX_DELAY));     //Clean up

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

#تعریف_کرو شے_ناپ       3
#تعریف_کرو NO_دا_اشیاء     3
#تعریف_کرو DELAY_TICKS     2

ساكن Static_قطار_t قطار_بفر;       //Queues, Semaphores, and Mutex use the same queue structure
ساكن uint8_t قطار_storage_area[(شے_ناپ * NO_دا_اشیاء)];  //Queue storage provided in separate buffer to queue struct

TEST_CASE("Test FreeRTOS backported Queue and Semphr functions", "[freertos]")
{
    //Test static queue
    uint8_t قطار_شے_منزل_send[شے_ناپ];
    uint8_t قطار_شے_received[شے_ناپ];
    لكل (صحيح i = 0; i < شے_ناپ; i++) {
        قطار_شے_منزل_send[i] = (0xF << i);
    }
    قطار_Handle_t handle = x_قطار_Create_Static(NO_دا_اشیاء, شے_ناپ, (uint8_t*) &قطار_storage_area, &قطار_بفر);
    TEST_ASSERT_EQUAL(pdTRUE, x_قطار_Send_منزل_پٹھ(handle, &قطار_شے_منزل_send, DELAY_TICKS));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(pdTRUE, x_قطار_Receive(handle, قطار_شے_received, DELAY_TICKS));
    vTaskDelay(1);
    لكل (صحيح i = 0; i < شے_ناپ; i++) {
        TEST_ASSERT_EQUAL(قطار_شے_منزل_send[i], قطار_شے_received[i]);   //Check received contents are correct
    }
    v_قطار_Delete(handle);   //Technically not needed as deleting static queue/semphr doesn't clear static memory

    //Test static binary semaphore
    handle = x_Semaphore_Create_ثنائی_Static(&قطار_بفر);   //Queue and Semphr handles are the same
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(handle));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(handle, DELAY_TICKS));
    vTaskDelay(1);
    vSemaphoreDelete(handle);

    //Test static counting semaphore and uxSemaphoreGetCount()
    handle = xSemaphoreCreateCountingStatic(NO_دا_اشیاء, 0, &قطار_بفر);
    لكل (صحيح i = 0; i < NO_دا_اشیاء; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(handle));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(NO_دا_اشیاء, ux_Semaphore_Get_گنتی(handle));    //Test uxSemaphoreGetCount()
    لكل (صحيح i = 0; i < NO_دا_اشیاء; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(handle, DELAY_TICKS));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(0, ux_Semaphore_Get_گنتی(handle));
    vSemaphoreDelete(handle);

    //Test static mutex
    handle = xSemaphoreCreateMutexStatic(&قطار_بفر);
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(handle, DELAY_TICKS));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR((فراغ *)xTaskGetCurrentTaskHandle(), xSemaphoreGetMutexHolder(handle));   //Current task should now hold mutex
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(handle));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR(عدم, xSemaphoreGetMutexHolder(handle));      //Mutex should have been released
    vSemaphoreDelete(handle);

    //Test static mutex recursive
    handle = xSemaphoreCreateRecursiveMutexStatic(&قطار_بفر);
    لكل (صحيح i = 0; i < NO_دا_اشیاء; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTakeRecursive(handle, DELAY_TICKS));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR((فراغ *)xTaskGetCurrentTaskHandle(), xSemaphoreGetMutexHolder(handle));   //Current task should hold mutex
    لكل (صحيح i = 0; i < NO_دا_اشیاء; i++) {
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

#تعریف_کرو ڈھیر_ناپ  2048    //Task stack size

ساكن ڈھیر_Type_t task_ڈھیر[ڈھیر_ناپ];  //Static buffer for task stack
ساكن StaticTask_t task_بفر;            //Static buffer for TCB
ساكن bool has_لگاتار[CONFIG_FREERTOS_نمبر_دا_CORES];

ساكن فراغ task(فراغ *arg)
{
    has_لگاتار[x_پورٹ_Get_Core_ID()] = true;       //Signify static task cb has run
    vTaskDelete(عدم);
}

TEST_CASE("Test FreeRTOS static task allocation", "[freertos]")
{
    لكل (صحيح core = 0; core < CONFIG_FREERTOS_نمبر_دا_CORES; core++) {
        has_لگاتار[core] = false;     //Clear has_run flag
        TaskHandle_t handle = x_Task_Create_Static_Pinned_منزل_Core(task, "static task", ڈھیر_ناپ, عدم,
                                                            UNITY_FREERTOS_ترجیح + 1, (ڈھیر_Type_t *)&task_ڈھیر,
                                                            (StaticTask_t *)&task_بفر, core);
        vTaskDelay(5); //Allow for static task to run, delete, and idle to clean up
        TEST_ASSERT_NOT_EQUAL(عدم, handle);    //Check static task was successfully allocated
        TEST_ASSERT_TRUE(has_لگاتار[core]);    //Check static task has run
    }
}

/* ------------- Test backported static event group allocation -------------------
 * Test xEventGroupCreateStatic() but creating static event group then waiting
 * for an event.
 */

#تعریف_کرو WAIT_بٹ   0x01    //Wait for first bit

ساكن Static_Event_گروہ_t event_گروہ;
ساكن Event_گروہ_Handle_t eg_handle;

TEST_CASE("Test FreeRTOS backported eventgroup functions", "[freertos]")
{
    eg_handle = x_Event_گروہ_Create_Static((Static_Event_گروہ_t *)&event_گروہ);
    x_Event_گروہ_سیٹ_بٹ(eg_handle, WAIT_بٹ);
    TEST_ASSERT_EQUAL(WAIT_بٹ, x_Event_گروہ_Wait_بٹ(eg_handle, WAIT_بٹ, pdTRUE, pdTRUE, پورٹ_MAX_DELAY));
    //Cleanup static event
    v_Event_گروہ_Delete(eg_handle);
}
