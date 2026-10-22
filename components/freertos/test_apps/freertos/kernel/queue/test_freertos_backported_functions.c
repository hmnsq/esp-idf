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
#شامل "sdkconfig.h"
#شامل <stdio.h>
#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل "freertos/timers.h"
#شامل "freertos/queue.h"
#شامل "freertos/semphr.h"
#شامل "freertos/event_groups.h"
#شامل "unity.h"
#شامل "test_utils.h"

/* ---------------------Test 1: Backported Timer functions-----------------------
 * Test xTimerCreateStatic(), vTimerSetTimerId(), xTimerGetPeriod(), xTimerGetExpiryTime()
 *
 * This test creates a one-shot static timer, sets/checks the timer's id and period. Then ensures
 * the timer cb is executed in a timely fashion.
 */
#تعریف_کرو TMR_PERIOD_TICKS  10
#تعریف_کرو TIMER_ID    0xFF
#تعریف_کرو TICK_DELTA  5

ساکن StaticTimer_t timer_بفر;
ساکن TickType_t tmr_ideal_exp;

ساکن خالی tmr_cb(TimerHandle_t xtimer)
{
    //Check cb is called in timely fashion
    TEST_ASSERT_UINT32_WITHIN(TICK_DELTA, tmr_ideal_exp, x_Task_Get_Tick_گنتی());
}

//No need for smp test as Timer Task always runs on core 0
TEST_CASE("Test FreeRTOS backported timer functions", "[freertos]")
{
    //Create one shot static timer with period TMR_PERIOD_TICKS
    TimerHandle_t tmr_handle = xTimerCreateStatic("static_tmr", TMR_PERIOD_TICKS, pdFALSE, تہی, tmr_cb, &timer_بفر);
    TEST_ASSERT_EQUAL(TMR_PERIOD_TICKS, xTimerGetPeriod(tmr_handle));       //Test xTimerGetPeriod()

    v_Timer_سیٹ_Timer_ID(tmr_handle, (خالی *)TIMER_ID);
    TEST_ASSERT_EQUAL(TIMER_ID, (uint32_t)pvTimerGetTimerID(tmr_handle));   //Test vTimerSetTimerID()

    TEST_ASSERT_EQUAL(pdTRUE, x_Timer_آغاز(tmr_handle, 1));      //Start Timer
    tmr_ideal_exp = x_Task_Get_Tick_گنتی() + TMR_PERIOD_TICKS;      //Calculate ideal expiration time
    vTaskDelay(2);  //Need to yield to allow daemon task to process start command, or else expiration time will be NULL

    TEST_ASSERT_UINT32_WITHIN(TICK_DELTA, tmr_ideal_exp, x_Timer_Get_Expiry_وقت(tmr_handle));    //Test xTimerGetExpiryTime()

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

#تعریف_کرو شے_سائز       3
#تعریف_کرو NO_کا_اشیاء     3
#تعریف_کرو DELAY_TICKS     2

ساکن Static_قطار_t قطار_بفر;       //Queues, Semaphores, and Mutex use the same queue structure
ساکن uint8_t قطار_storage_area[(شے_سائز * NO_کا_اشیاء)];  //Queue storage provided in separate buffer to queue struct

TEST_CASE("Test FreeRTOS backported Queue and Semphr functions", "[freertos]")
{
    //Test static queue
    uint8_t قطار_شے_مقصد_send[شے_سائز];
    uint8_t قطار_شے_received[شے_سائز];
    برائے (صحیح i = 0; i < شے_سائز; i++) {
        قطار_شے_مقصد_send[i] = (0xF << i);
    }
    قطار_Handle_t handle = x_قطار_Create_Static(NO_کا_اشیاء, شے_سائز, (uint8_t*) &قطار_storage_area, &قطار_بفر);
    TEST_ASSERT_EQUAL(pdTRUE, x_قطار_Send_مقصد_عقب(handle, &قطار_شے_مقصد_send, DELAY_TICKS));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(pdTRUE, x_قطار_Receive(handle, قطار_شے_received, DELAY_TICKS));
    vTaskDelay(1);
    برائے (صحیح i = 0; i < شے_سائز; i++) {
        TEST_ASSERT_EQUAL(قطار_شے_مقصد_send[i], قطار_شے_received[i]);   //Check received contents are correct
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
    handle = xSemaphoreCreateCountingStatic(NO_کا_اشیاء, 0, &قطار_بفر);
    برائے (صحیح i = 0; i < NO_کا_اشیاء; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(handle));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(NO_کا_اشیاء, ux_Semaphore_Get_گنتی(handle));    //Test uxSemaphoreGetCount()
    برائے (صحیح i = 0; i < NO_کا_اشیاء; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(handle, DELAY_TICKS));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(0, ux_Semaphore_Get_گنتی(handle));
    vSemaphoreDelete(handle);

    //Test static mutex
    handle = xSemaphoreCreateMutexStatic(&قطار_بفر);
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(handle, DELAY_TICKS));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR((خالی *)xTaskGetCurrentTaskHandle(), xSemaphoreGetMutexHolder(handle));   //Current task should now hold mutex
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(handle));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR(تہی, xSemaphoreGetMutexHolder(handle));      //Mutex should have been released
    vSemaphoreDelete(handle);

    //Test static mutex recursive
    handle = xSemaphoreCreateRecursiveMutexStatic(&قطار_بفر);
    برائے (صحیح i = 0; i < NO_کا_اشیاء; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTakeRecursive(handle, DELAY_TICKS));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR((خالی *)xTaskGetCurrentTaskHandle(), xSemaphoreGetMutexHolder(handle));   //Current task should hold mutex
    برائے (صحیح i = 0; i < NO_کا_اشیاء; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGiveRecursive(handle));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR(تہی, xSemaphoreGetMutexHolder(handle));      //Mutex should have been released
    vSemaphoreDelete(handle);

}

/* -----------------Test backported static task allocation  -------------------
 * Test xTaskCreateStaticPinnedToCore() but creating static task on each core
 * and checking the task cb has run successfully.
 */

#تعریف_کرو ڈھیر_سائز  2048    //Task stack size

ساکن ڈھیر_Type_t task_ڈھیر[ڈھیر_سائز];  //Static buffer for task stack
ساکن StaticTask_t task_بفر;            //Static buffer for TCB
ساکن bool has_تسلسل[CONFIG_FREERTOS_نمبر_کا_CORES];

ساکن خالی task(خالی *arg)
{
    has_تسلسل[x_پورٹ_Get_Core_ID()] = true;       //Signify static task cb has run
    vTaskDelete(تہی);
}

TEST_CASE("Test FreeRTOS static task allocation", "[freertos]")
{
    برائے (صحیح core = 0; core < CONFIG_FREERTOS_نمبر_کا_CORES; core++) {
        has_تسلسل[core] = false;     //Clear has_run flag
        TaskHandle_t handle = x_Task_Create_Static_Pinned_مقصد_Core(task, "static task", ڈھیر_سائز, تہی,
                                                            UNITY_FREERTOS_ترجیح + 1, (ڈھیر_Type_t *)&task_ڈھیر,
                                                            (StaticTask_t *)&task_بفر, core);
        vTaskDelay(5); //Allow for static task to run, delete, and idle to clean up
        TEST_ASSERT_NOT_EQUAL(تہی, handle);    //Check static task was successfully allocated
        TEST_ASSERT_TRUE(has_تسلسل[core]);    //Check static task has run
    }
}

/* ------------- Test backported static event group allocation -------------------
 * Test xEventGroupCreateStatic() but creating static event group then waiting
 * for an event.
 */

#تعریف_کرو WAIT_بٹ   0x01    //Wait for first bit

ساکن Static_Event_گروہ_t event_گروہ;
ساکن Event_گروہ_Handle_t eg_handle;

TEST_CASE("Test FreeRTOS backported eventgroup functions", "[freertos]")
{
    eg_handle = x_Event_گروہ_Create_Static((Static_Event_گروہ_t *)&event_گروہ);
    x_Event_گروہ_سیٹ_بٹ(eg_handle, WAIT_بٹ);
    TEST_ASSERT_EQUAL(WAIT_بٹ, x_Event_گروہ_Wait_بٹ(eg_handle, WAIT_بٹ, pdTRUE, pdTRUE, پورٹ_MAX_DELAY));
    //Cleanup static event
    v_Event_گروہ_Delete(eg_handle);
}
