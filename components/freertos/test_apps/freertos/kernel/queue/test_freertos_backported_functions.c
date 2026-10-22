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
#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "freertos/timers.h"
#అన్తర్భావ "freertos/queue.h"
#అన్తర్భావ "freertos/semphr.h"
#అన్తర్భావ "freertos/event_groups.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "test_utils.h"

/* ---------------------Test 1: Backported Timer functions-----------------------
 * Test xTimerCreateStatic(), vTimerSetTimerId(), xTimerGetPeriod(), xTimerGetExpiryTime()
 *
 * This test creates a one-shot static timer, sets/checks the timer's id and period. Then ensures
 * the timer cb is executed in a timely fashion.
 */
#నిర్వచించు TMR_PERIOD_TICKS  10
#నిర్వచించు TIMER_ID    0xFF
#నిర్వచించు TICK_DELTA  5

స్థిర StaticTimer_t timer_బఫరు;
స్థిర TickType_t tmr_ideal_exp;

స్థిర శూన్య tmr_cb(TimerHandle_t xtimer)
{
    //Check cb is called in timely fashion
    TEST_ASSERT_UINT32_WITHIN(TICK_DELTA, tmr_ideal_exp, x_Task_Get_Tick_లెక్క());
}

//No need for smp test as Timer Task always runs on core 0
TEST_CASE("Test FreeRTOS backported timer functions", "[freertos]")
{
    //Create one shot static timer with period TMR_PERIOD_TICKS
    TimerHandle_t tmr_handle = xTimerCreateStatic("static_tmr", TMR_PERIOD_TICKS, pdFALSE, ఖాళీ, tmr_cb, &timer_బఫరు);
    TEST_ASSERT_EQUAL(TMR_PERIOD_TICKS, xTimerGetPeriod(tmr_handle));       //Test xTimerGetPeriod()

    v_Timer_సమితి_Timer_ID(tmr_handle, (శూన్య *)TIMER_ID);
    TEST_ASSERT_EQUAL(TIMER_ID, (uint32_t)pvTimerGetTimerID(tmr_handle));   //Test vTimerSetTimerID()

    TEST_ASSERT_EQUAL(pdTRUE, x_Timer_ప్రారంభం(tmr_handle, 1));      //Start Timer
    tmr_ideal_exp = x_Task_Get_Tick_లెక్క() + TMR_PERIOD_TICKS;      //Calculate ideal expiration time
    vTaskDelay(2);  //Need to yield to allow daemon task to process start command, or else expiration time will be NULL

    TEST_ASSERT_UINT32_WITHIN(TICK_DELTA, tmr_ideal_exp, x_Timer_Get_Expiry_సమయం(tmr_handle));    //Test xTimerGetExpiryTime()

    vTaskDelay(2 * TMR_PERIOD_TICKS);   //Delay until one shot timer has triggered
    TEST_ASSERT_EQUAL(pdPASS, xTimerDelete(tmr_handle, పోర్టు_MAX_DELAY));     //Clean up

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

#నిర్వచించు అంశం_కొలత       3
#నిర్వచించు NO_యొక్క_వస్తువులు     3
#నిర్వచించు DELAY_TICKS     2

స్థిర Static_వరుస_t వరుస_బఫరు;       //Queues, Semaphores, and Mutex use the same queue structure
స్థిర uint8_t వరుస_storage_area[(అంశం_కొలత * NO_యొక్క_వస్తువులు)];  //Queue storage provided in separate buffer to queue struct

TEST_CASE("Test FreeRTOS backported Queue and Semphr functions", "[freertos]")
{
    //Test static queue
    uint8_t వరుస_అంశం_గమ్యం_send[అంశం_కొలత];
    uint8_t వరుస_అంశం_received[అంశం_కొలత];
    కొరకు (పూర్ణాంకం i = 0; i < అంశం_కొలత; i++) {
        వరుస_అంశం_గమ్యం_send[i] = (0xF << i);
    }
    వరుస_Handle_t handle = x_వరుస_Create_Static(NO_యొక్క_వస్తువులు, అంశం_కొలత, (uint8_t*) &వరుస_storage_area, &వరుస_బఫరు);
    TEST_ASSERT_EQUAL(pdTRUE, x_వరుస_Send_గమ్యం_వెనుక(handle, &వరుస_అంశం_గమ్యం_send, DELAY_TICKS));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(pdTRUE, x_వరుస_Receive(handle, వరుస_అంశం_received, DELAY_TICKS));
    vTaskDelay(1);
    కొరకు (పూర్ణాంకం i = 0; i < అంశం_కొలత; i++) {
        TEST_ASSERT_EQUAL(వరుస_అంశం_గమ్యం_send[i], వరుస_అంశం_received[i]);   //Check received contents are correct
    }
    v_వరుస_Delete(handle);   //Technically not needed as deleting static queue/semphr doesn't clear static memory

    //Test static binary semaphore
    handle = x_Semaphore_Create_ద్విమాన_Static(&వరుస_బఫరు);   //Queue and Semphr handles are the same
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(handle));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(handle, DELAY_TICKS));
    vTaskDelay(1);
    vSemaphoreDelete(handle);

    //Test static counting semaphore and uxSemaphoreGetCount()
    handle = xSemaphoreCreateCountingStatic(NO_యొక్క_వస్తువులు, 0, &వరుస_బఫరు);
    కొరకు (పూర్ణాంకం i = 0; i < NO_యొక్క_వస్తువులు; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(handle));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(NO_యొక్క_వస్తువులు, ux_Semaphore_Get_లెక్క(handle));    //Test uxSemaphoreGetCount()
    కొరకు (పూర్ణాంకం i = 0; i < NO_యొక్క_వస్తువులు; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(handle, DELAY_TICKS));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(0, ux_Semaphore_Get_లెక్క(handle));
    vSemaphoreDelete(handle);

    //Test static mutex
    handle = xSemaphoreCreateMutexStatic(&వరుస_బఫరు);
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(handle, DELAY_TICKS));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR((శూన్య *)xTaskGetCurrentTaskHandle(), xSemaphoreGetMutexHolder(handle));   //Current task should now hold mutex
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(handle));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR(ఖాళీ, xSemaphoreGetMutexHolder(handle));      //Mutex should have been released
    vSemaphoreDelete(handle);

    //Test static mutex recursive
    handle = xSemaphoreCreateRecursiveMutexStatic(&వరుస_బఫరు);
    కొరకు (పూర్ణాంకం i = 0; i < NO_యొక్క_వస్తువులు; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTakeRecursive(handle, DELAY_TICKS));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR((శూన్య *)xTaskGetCurrentTaskHandle(), xSemaphoreGetMutexHolder(handle));   //Current task should hold mutex
    కొరకు (పూర్ణాంకం i = 0; i < NO_యొక్క_వస్తువులు; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGiveRecursive(handle));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR(ఖాళీ, xSemaphoreGetMutexHolder(handle));      //Mutex should have been released
    vSemaphoreDelete(handle);

}

/* -----------------Test backported static task allocation  -------------------
 * Test xTaskCreateStaticPinnedToCore() but creating static task on each core
 * and checking the task cb has run successfully.
 */

#నిర్వచించు దొంతర_కొలత  2048    //Task stack size

స్థిర దొంతర_Type_t task_దొంతర[దొంతర_కొలత];  //Static buffer for task stack
స్థిర StaticTask_t task_బఫరు;            //Static buffer for TCB
స్థిర bool has_నిరంతరం[CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES];

స్థిర శూన్య task(శూన్య *arg)
{
    has_నిరంతరం[x_పోర్టు_Get_Core_ID()] = true;       //Signify static task cb has run
    vTaskDelete(ఖాళీ);
}

TEST_CASE("Test FreeRTOS static task allocation", "[freertos]")
{
    కొరకు (పూర్ణాంకం core = 0; core < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; core++) {
        has_నిరంతరం[core] = false;     //Clear has_run flag
        TaskHandle_t handle = x_Task_Create_Static_Pinned_గమ్యం_Core(task, "static task", దొంతర_కొలత, ఖాళీ,
                                                            UNITY_FREERTOS_ప్రాధాన్యత + 1, (దొంతర_Type_t *)&task_దొంతర,
                                                            (StaticTask_t *)&task_బఫరు, core);
        vTaskDelay(5); //Allow for static task to run, delete, and idle to clean up
        TEST_ASSERT_NOT_EQUAL(ఖాళీ, handle);    //Check static task was successfully allocated
        TEST_ASSERT_TRUE(has_నిరంతరం[core]);    //Check static task has run
    }
}

/* ------------- Test backported static event group allocation -------------------
 * Test xEventGroupCreateStatic() but creating static event group then waiting
 * for an event.
 */

#నిర్వచించు WAIT_బిట్లు   0x01    //Wait for first bit

స్థిర Static_Event_సమూహం_t event_సమూహం;
స్థిర Event_సమూహం_Handle_t eg_handle;

TEST_CASE("Test FreeRTOS backported eventgroup functions", "[freertos]")
{
    eg_handle = x_Event_సమూహం_Create_Static((Static_Event_సమూహం_t *)&event_సమూహం);
    x_Event_సమూహం_సమితి_బిట్లు(eg_handle, WAIT_బిట్లు);
    TEST_ASSERT_EQUAL(WAIT_బిట్లు, x_Event_సమూహం_Wait_బిట్లు(eg_handle, WAIT_బిట్లు, pdTRUE, pdTRUE, పోర్టు_MAX_DELAY));
    //Cleanup static event
    v_Event_సమూహం_Delete(eg_handle);
}
