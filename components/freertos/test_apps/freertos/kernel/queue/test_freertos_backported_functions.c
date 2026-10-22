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
#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "freertos/timers.h"
#ಅನ್ತರ್ಭಾವ "freertos/queue.h"
#ಅನ್ತರ್ಭಾವ "freertos/semphr.h"
#ಅನ್ತರ್ಭಾವ "freertos/event_groups.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "test_utils.h"

/* ---------------------Test 1: Backported Timer functions-----------------------
 * Test xTimerCreateStatic(), vTimerSetTimerId(), xTimerGetPeriod(), xTimerGetExpiryTime()
 *
 * This test creates a one-shot static timer, sets/checks the timer's id and period. Then ensures
 * the timer cb is executed in a timely fashion.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು TMR_PERIOD_TICKS  10
#ವ್ಯಾಖ್ಯಾನಿಸು TIMER_ID    0xFF
#ವ್ಯಾಖ್ಯಾನಿಸು TICK_DELTA  5

ಸ್ಥಿರ StaticTimer_t timer_ಬಫರ್;
ಸ್ಥಿರ TickType_t tmr_ideal_exp;

ಸ್ಥಿರ ಶೂನ್ಯ tmr_cb(TimerHandle_t xtimer)
{
    //Check cb is called in timely fashion
    TEST_ASSERT_UINT32_WITHIN(TICK_DELTA, tmr_ideal_exp, x_Task_Get_Tick_ಎಣಿಕೆ());
}

//No need for smp test as Timer Task always runs on core 0
TEST_CASE("Test FreeRTOS backported timer functions", "[freertos]")
{
    //Create one shot static timer with period TMR_PERIOD_TICKS
    TimerHandle_t tmr_handle = xTimerCreateStatic("static_tmr", TMR_PERIOD_TICKS, pdFALSE, ಖಾಲಿ, tmr_cb, &timer_ಬಫರ್);
    TEST_ASSERT_EQUAL(TMR_PERIOD_TICKS, xTimerGetPeriod(tmr_handle));       //Test xTimerGetPeriod()

    v_Timer_ಗಣ_Timer_ID(tmr_handle, (ಶೂನ್ಯ *)TIMER_ID);
    TEST_ASSERT_EQUAL(TIMER_ID, (uint32_t)pvTimerGetTimerID(tmr_handle));   //Test vTimerSetTimerID()

    TEST_ASSERT_EQUAL(pdTRUE, x_Timer_ಆರಂಭ(tmr_handle, 1));      //Start Timer
    tmr_ideal_exp = x_Task_Get_Tick_ಎಣಿಕೆ() + TMR_PERIOD_TICKS;      //Calculate ideal expiration time
    vTaskDelay(2);  //Need to yield to allow daemon task to process start command, or else expiration time will be NULL

    TEST_ASSERT_UINT32_WITHIN(TICK_DELTA, tmr_ideal_exp, x_Timer_Get_Expiry_ಸಮಯ(tmr_handle));    //Test xTimerGetExpiryTime()

    vTaskDelay(2 * TMR_PERIOD_TICKS);   //Delay until one shot timer has triggered
    TEST_ASSERT_EQUAL(pdPASS, xTimerDelete(tmr_handle, ಪೋರ್ಟ್_MAX_DELAY));     //Clean up

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

#ವ್ಯಾಖ್ಯಾನಿಸು ಅಂಶ_ಗಾತ್ರ       3
#ವ್ಯಾಖ್ಯಾನಿಸು NO_ದ_ವಸ್ತುಗಳು     3
#ವ್ಯಾಖ್ಯಾನಿಸು DELAY_TICKS     2

ಸ್ಥಿರ Static_ಸರತಿ_t ಸರತಿ_ಬಫರ್;       //Queues, Semaphores, and Mutex use the same queue structure
ಸ್ಥಿರ uint8_t ಸರತಿ_storage_area[(ಅಂಶ_ಗಾತ್ರ * NO_ದ_ವಸ್ತುಗಳು)];  //Queue storage provided in separate buffer to queue struct

TEST_CASE("Test FreeRTOS backported Queue and Semphr functions", "[freertos]")
{
    //Test static queue
    uint8_t ಸರತಿ_ಅಂಶ_ಗಮ್ಯ_send[ಅಂಶ_ಗಾತ್ರ];
    uint8_t ಸರತಿ_ಅಂಶ_received[ಅಂಶ_ಗಾತ್ರ];
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಅಂಶ_ಗಾತ್ರ; i++) {
        ಸರತಿ_ಅಂಶ_ಗಮ್ಯ_send[i] = (0xF << i);
    }
    ಸರತಿ_Handle_t handle = x_ಸರತಿ_Create_Static(NO_ದ_ವಸ್ತುಗಳು, ಅಂಶ_ಗಾತ್ರ, (uint8_t*) &ಸರತಿ_storage_area, &ಸರತಿ_ಬಫರ್);
    TEST_ASSERT_EQUAL(pdTRUE, x_ಸರತಿ_Send_ಗಮ್ಯ_ಹಿಂಭಾಗ(handle, &ಸರತಿ_ಅಂಶ_ಗಮ್ಯ_send, DELAY_TICKS));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(pdTRUE, x_ಸರತಿ_Receive(handle, ಸರತಿ_ಅಂಶ_received, DELAY_TICKS));
    vTaskDelay(1);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಅಂಶ_ಗಾತ್ರ; i++) {
        TEST_ASSERT_EQUAL(ಸರತಿ_ಅಂಶ_ಗಮ್ಯ_send[i], ಸರತಿ_ಅಂಶ_received[i]);   //Check received contents are correct
    }
    v_ಸರತಿ_Delete(handle);   //Technically not needed as deleting static queue/semphr doesn't clear static memory

    //Test static binary semaphore
    handle = x_Semaphore_Create_ದ್ವಿಮಾನ_Static(&ಸರತಿ_ಬಫರ್);   //Queue and Semphr handles are the same
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(handle));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(handle, DELAY_TICKS));
    vTaskDelay(1);
    vSemaphoreDelete(handle);

    //Test static counting semaphore and uxSemaphoreGetCount()
    handle = xSemaphoreCreateCountingStatic(NO_ದ_ವಸ್ತುಗಳು, 0, &ಸರತಿ_ಬಫರ್);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < NO_ದ_ವಸ್ತುಗಳು; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(handle));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(NO_ದ_ವಸ್ತುಗಳು, ux_Semaphore_Get_ಎಣಿಕೆ(handle));    //Test uxSemaphoreGetCount()
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < NO_ದ_ವಸ್ತುಗಳು; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(handle, DELAY_TICKS));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(0, ux_Semaphore_Get_ಎಣಿಕೆ(handle));
    vSemaphoreDelete(handle);

    //Test static mutex
    handle = xSemaphoreCreateMutexStatic(&ಸರತಿ_ಬಫರ್);
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(handle, DELAY_TICKS));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR((ಶೂನ್ಯ *)xTaskGetCurrentTaskHandle(), xSemaphoreGetMutexHolder(handle));   //Current task should now hold mutex
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(handle));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR(ಖಾಲಿ, xSemaphoreGetMutexHolder(handle));      //Mutex should have been released
    vSemaphoreDelete(handle);

    //Test static mutex recursive
    handle = xSemaphoreCreateRecursiveMutexStatic(&ಸರತಿ_ಬಫರ್);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < NO_ದ_ವಸ್ತುಗಳು; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTakeRecursive(handle, DELAY_TICKS));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR((ಶೂನ್ಯ *)xTaskGetCurrentTaskHandle(), xSemaphoreGetMutexHolder(handle));   //Current task should hold mutex
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < NO_ದ_ವಸ್ತುಗಳು; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGiveRecursive(handle));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR(ಖಾಲಿ, xSemaphoreGetMutexHolder(handle));      //Mutex should have been released
    vSemaphoreDelete(handle);

}

/* -----------------Test backported static task allocation  -------------------
 * Test xTaskCreateStaticPinnedToCore() but creating static task on each core
 * and checking the task cb has run successfully.
 */

#ವ್ಯಾಖ್ಯಾನಿಸು ರಾಶಿ_ಗಾತ್ರ  2048    //Task stack size

ಸ್ಥಿರ ರಾಶಿ_Type_t task_ರಾಶಿ[ರಾಶಿ_ಗಾತ್ರ];  //Static buffer for task stack
ಸ್ಥಿರ StaticTask_t task_ಬಫರ್;            //Static buffer for TCB
ಸ್ಥಿರ bool has_ಸತತ[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES];

ಸ್ಥಿರ ಶೂನ್ಯ task(ಶೂನ್ಯ *arg)
{
    has_ಸತತ[x_ಪೋರ್ಟ್_Get_Core_ID()] = true;       //Signify static task cb has run
    vTaskDelete(ಖಾಲಿ);
}

TEST_CASE("Test FreeRTOS static task allocation", "[freertos]")
{
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ core = 0; core < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; core++) {
        has_ಸತತ[core] = false;     //Clear has_run flag
        TaskHandle_t handle = x_Task_Create_Static_Pinned_ಗಮ್ಯ_Core(task, "static task", ರಾಶಿ_ಗಾತ್ರ, ಖಾಲಿ,
                                                            UNITY_FREERTOS_ಆದ್ಯತೆ + 1, (ರಾಶಿ_Type_t *)&task_ರಾಶಿ,
                                                            (StaticTask_t *)&task_ಬಫರ್, core);
        vTaskDelay(5); //Allow for static task to run, delete, and idle to clean up
        TEST_ASSERT_NOT_EQUAL(ಖಾಲಿ, handle);    //Check static task was successfully allocated
        TEST_ASSERT_TRUE(has_ಸತತ[core]);    //Check static task has run
    }
}

/* ------------- Test backported static event group allocation -------------------
 * Test xEventGroupCreateStatic() but creating static event group then waiting
 * for an event.
 */

#ವ್ಯಾಖ್ಯಾನಿಸು WAIT_ಬಿಟ್ಟುಗಳು   0x01    //Wait for first bit

ಸ್ಥಿರ Static_Event_ಗುಂಪು_t event_ಗುಂಪು;
ಸ್ಥಿರ Event_ಗುಂಪು_Handle_t eg_handle;

TEST_CASE("Test FreeRTOS backported eventgroup functions", "[freertos]")
{
    eg_handle = x_Event_ಗುಂಪು_Create_Static((Static_Event_ಗುಂಪು_t *)&event_ಗುಂಪು);
    x_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು(eg_handle, WAIT_ಬಿಟ್ಟುಗಳು);
    TEST_ASSERT_EQUAL(WAIT_ಬಿಟ್ಟುಗಳು, x_Event_ಗುಂಪು_Wait_ಬಿಟ್ಟುಗಳು(eg_handle, WAIT_ಬಿಟ್ಟುಗಳು, pdTRUE, pdTRUE, ಪೋರ್ಟ್_MAX_DELAY));
    //Cleanup static event
    v_Event_ಗುಂಪು_Delete(eg_handle);
}
