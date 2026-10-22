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
#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"
#अन्तर्भाव "freertos/timers.h"
#अन्तर्भाव "freertos/queue.h"
#अन्तर्भाव "freertos/semphr.h"
#अन्तर्भाव "freertos/event_groups.h"
#अन्तर्भाव "unity.h"
#अन्तर्भाव "test_utils.h"

/* ---------------------Test 1: Backported Timer functions-----------------------
 * Test xTimerCreateStatic(), vTimerSetTimerId(), xTimerGetPeriod(), xTimerGetExpiryTime()
 *
 * This test creates a one-shot static timer, sets/checks the timer's id and period. Then ensures
 * the timer cb is executed in a timely fashion.
 */
#परिभाषय TMR_PERIOD_TICKS  10
#परिभाषय TIMER_ID    0xFF
#परिभाषय TICK_DELTA  5

स्थिर StaticTimer_t timer_बफर;
स्थिर TickType_t tmr_ideal_exp;

स्थिर शून्य tmr_cb(TimerHandle_t xtimer)
{
    //Check cb is called in timely fashion
    TEST_ASSERT_UINT32_WITHIN(TICK_DELTA, tmr_ideal_exp, x_Task_Get_Tick_गणनम्());
}

//No need for smp test as Timer Task always runs on core 0
TEST_CASE("Test FreeRTOS backported timer functions", "[freertos]")
{
    //Create one shot static timer with period TMR_PERIOD_TICKS
    TimerHandle_t tmr_handle = xTimerCreateStatic("static_tmr", TMR_PERIOD_TICKS, pdFALSE, रिक्त, tmr_cb, &timer_बफर);
    TEST_ASSERT_EQUAL(TMR_PERIOD_TICKS, xTimerGetPeriod(tmr_handle));       //Test xTimerGetPeriod()

    v_Timer_समुच्चयः_Timer_ID(tmr_handle, (शून्य *)TIMER_ID);
    TEST_ASSERT_EQUAL(TIMER_ID, (uint32_t)pvTimerGetTimerID(tmr_handle));   //Test vTimerSetTimerID()

    TEST_ASSERT_EQUAL(pdTRUE, xTimerStart(tmr_handle, 1));      //Start Timer
    tmr_ideal_exp = x_Task_Get_Tick_गणनम्() + TMR_PERIOD_TICKS;      //Calculate ideal expiration time
    vTaskDelay(2);  //Need to yield to allow daemon task to process start command, or else expiration time will be NULL

    TEST_ASSERT_UINT32_WITHIN(TICK_DELTA, tmr_ideal_exp, xTimerGetExpiryTime(tmr_handle));    //Test xTimerGetExpiryTime()

    vTaskDelay(2 * TMR_PERIOD_TICKS);   //Delay until one shot timer has triggered
    TEST_ASSERT_EQUAL(pdPASS, xTimerDelete(tmr_handle, पत्तनम्_MAX_DELAY));     //Clean up

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

#परिभाषय ITEM_आकार       3
#परिभाषय NO_सम्बन्धि_वस्तूनि     3
#परिभाषय DELAY_TICKS     2

स्थिर StaticQueue_t queue_बफर;       //Queues, Semaphores, and Mutex use the same queue structure
स्थिर uint8_t queue_storage_area[(ITEM_आकार * NO_सम्बन्धि_वस्तूनि)];  //Queue storage provided in separate buffer to queue struct

TEST_CASE("Test FreeRTOS backported Queue and Semphr functions", "[freertos]")
{
    //Test static queue
    uint8_t queue_item_गन्तव्यम्_send[ITEM_आकार];
    uint8_t queue_item_received[ITEM_आकार];
    प्रति (पूर्णाङ्क i = 0; i < ITEM_आकार; i++) {
        queue_item_गन्तव्यम्_send[i] = (0xF << i);
    }
    QueueHandle_t handle = xQueueCreateStatic(NO_सम्बन्धि_वस्तूनि, ITEM_आकार, (uint8_t*) &queue_storage_area, &queue_बफर);
    TEST_ASSERT_EQUAL(pdTRUE, x_Queue_Send_गन्तव्यम्_पृष्ठभागः(handle, &queue_item_गन्तव्यम्_send, DELAY_TICKS));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(pdTRUE, xQueueReceive(handle, queue_item_received, DELAY_TICKS));
    vTaskDelay(1);
    प्रति (पूर्णाङ्क i = 0; i < ITEM_आकार; i++) {
        TEST_ASSERT_EQUAL(queue_item_गन्तव्यम्_send[i], queue_item_received[i]);   //Check received contents are correct
    }
    vQueueDelete(handle);   //Technically not needed as deleting static queue/semphr doesn't clear static memory

    //Test static binary semaphore
    handle = x_Semaphore_Create_द्विमान_Static(&queue_बफर);   //Queue and Semphr handles are the same
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(handle));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(handle, DELAY_TICKS));
    vTaskDelay(1);
    vSemaphoreDelete(handle);

    //Test static counting semaphore and uxSemaphoreGetCount()
    handle = xSemaphoreCreateCountingStatic(NO_सम्बन्धि_वस्तूनि, 0, &queue_बफर);
    प्रति (पूर्णाङ्क i = 0; i < NO_सम्बन्धि_वस्तूनि; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(handle));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(NO_सम्बन्धि_वस्तूनि, ux_Semaphore_Get_गणनम्(handle));    //Test uxSemaphoreGetCount()
    प्रति (पूर्णाङ्क i = 0; i < NO_सम्बन्धि_वस्तूनि; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(handle, DELAY_TICKS));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL(0, ux_Semaphore_Get_गणनम्(handle));
    vSemaphoreDelete(handle);

    //Test static mutex
    handle = xSemaphoreCreateMutexStatic(&queue_बफर);
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(handle, DELAY_TICKS));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR((शून्य *)xTaskGetCurrentTaskHandle(), xSemaphoreGetMutexHolder(handle));   //Current task should now hold mutex
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(handle));
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR(रिक्त, xSemaphoreGetMutexHolder(handle));      //Mutex should have been released
    vSemaphoreDelete(handle);

    //Test static mutex recursive
    handle = xSemaphoreCreateRecursiveMutexStatic(&queue_बफर);
    प्रति (पूर्णाङ्क i = 0; i < NO_सम्बन्धि_वस्तूनि; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTakeRecursive(handle, DELAY_TICKS));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR((शून्य *)xTaskGetCurrentTaskHandle(), xSemaphoreGetMutexHolder(handle));   //Current task should hold mutex
    प्रति (पूर्णाङ्क i = 0; i < NO_सम्बन्धि_वस्तूनि; i++) {
        TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGiveRecursive(handle));
    }
    vTaskDelay(1);
    TEST_ASSERT_EQUAL_PTR(रिक्त, xSemaphoreGetMutexHolder(handle));      //Mutex should have been released
    vSemaphoreDelete(handle);

}

/* -----------------Test backported static task allocation  -------------------
 * Test xTaskCreateStaticPinnedToCore() but creating static task on each core
 * and checking the task cb has run successfully.
 */

#परिभाषय राशिः_आकार  2048    //Task stack size

स्थिर राशिः_Type_t task_राशिः[राशिः_आकार];  //Static buffer for task stack
स्थिर StaticTask_t task_बफर;            //Static buffer for TCB
स्थिर bool has_सततम्[CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES];

स्थिर शून्य task(शून्य *arg)
{
    has_सततम्[x_पत्तनम्_Get_Core_ID()] = true;       //Signify static task cb has run
    vTaskDelete(रिक्त);
}

TEST_CASE("Test FreeRTOS static task allocation", "[freertos]")
{
    प्रति (पूर्णाङ्क core = 0; core < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES; core++) {
        has_सततम्[core] = false;     //Clear has_run flag
        TaskHandle_t handle = x_Task_Create_Static_Pinned_गन्तव्यम्_Core(task, "static task", राशिः_आकार, रिक्त,
                                                            UNITY_FREERTOS_प्राथमिकता + 1, (राशिः_Type_t *)&task_राशिः,
                                                            (StaticTask_t *)&task_बफर, core);
        vTaskDelay(5); //Allow for static task to run, delete, and idle to clean up
        TEST_ASSERT_NOT_EQUAL(रिक्त, handle);    //Check static task was successfully allocated
        TEST_ASSERT_TRUE(has_सततम्[core]);    //Check static task has run
    }
}

/* ------------- Test backported static event group allocation -------------------
 * Test xEventGroupCreateStatic() but creating static event group then waiting
 * for an event.
 */

#परिभाषय WAIT_द्वयङ्काः   0x01    //Wait for first bit

स्थिर Static_Event_गणः_t event_गणः;
स्थिर Event_गणः_Handle_t eg_handle;

TEST_CASE("Test FreeRTOS backported eventgroup functions", "[freertos]")
{
    eg_handle = x_Event_गणः_Create_Static((Static_Event_गणः_t *)&event_गणः);
    x_Event_गणः_समुच्चयः_द्वयङ्काः(eg_handle, WAIT_द्वयङ्काः);
    TEST_ASSERT_EQUAL(WAIT_द्वयङ्काः, x_Event_गणः_Wait_द्वयङ्काः(eg_handle, WAIT_द्वयङ्काः, pdTRUE, pdTRUE, पत्तनम्_MAX_DELAY));
    //Cleanup static event
    v_Event_गणः_Delete(eg_handle);
}
