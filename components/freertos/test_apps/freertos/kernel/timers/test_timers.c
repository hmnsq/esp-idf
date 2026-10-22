/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* FreeRTOS timer tests
*/
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/timers.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "test_utils.h"

ਸਥਿਰ ਖਾਲੀ timer_callback(TimerHandle_t timer)
{
    ਅਸਥਿਰ ਪੂਰਨ *ਗਿਣਤੀ_2;
    ਗਿਣਤੀ_2 = (ਅਸਥਿਰ ਪੂਰਨ *)pvTimerGetTimerID(timer);
    (*ਗਿਣਤੀ_2)++;
    ਲਿਖੋ("Callback timer %p count %p = %d\n", timer, ਗਿਣਤੀ_2, *ਗਿਣਤੀ_2);
}

TEST_CASE("Oneshot FreeRTOS timers", "[freertos]")
{
    ਅਸਥਿਰ ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
    TimerHandle_t oneshot = xTimerCreate("oneshot", 100 / ਪੋਰਟ_TICK_PERIOD_MS, pdFALSE,
                                         (ਖਾਲੀ *)&ਗਿਣਤੀ_2, timer_callback);
    TEST_ASSERT(oneshot);
    TEST_ASSERT_EQUAL(pdFALSE, x_Timer_Is_Timer_ਸਰਗਰਮ(oneshot));
    TEST_ASSERT_EQUAL(0, ਗਿਣਤੀ_2);

    TEST_ASSERT(x_Timer_ਸ਼ੁਰੂ(oneshot, 1));
    vTaskDelay(2); /* give the timer task a chance to process the message */

    TEST_ASSERT_EQUAL(pdTRUE, x_Timer_Is_Timer_ਸਰਗਰਮ(oneshot));
    TEST_ASSERT_EQUAL(0, ਗਿਣਤੀ_2);

    vTaskDelay(250 / ਪੋਰਟ_TICK_PERIOD_MS); // 2.5 timer periods

    TEST_ASSERT_EQUAL(1, ਗਿਣਤੀ_2);
    TEST_ASSERT_EQUAL(pdFALSE, x_Timer_Is_Timer_ਸਰਗਰਮ(oneshot));

    TEST_ASSERT(xTimerDelete(oneshot, 1));
}

TEST_CASE("Recurring FreeRTOS timers", "[freertos]")
{
    ਅਸਥਿਰ ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
    TimerHandle_t recurring = xTimerCreate("oneshot", 100 / ਪੋਰਟ_TICK_PERIOD_MS, pdTRUE,
                                           (ਖਾਲੀ *)&ਗਿਣਤੀ_2, timer_callback);
    TEST_ASSERT(recurring);
    TEST_ASSERT_EQUAL(pdFALSE, x_Timer_Is_Timer_ਸਰਗਰਮ(recurring));
    TEST_ASSERT_EQUAL(0, ਗਿਣਤੀ_2);

    TEST_ASSERT(x_Timer_ਸ਼ੁਰੂ(recurring, 1));

    vTaskDelay(2); // let timer task process the queue
    TEST_ASSERT_EQUAL(pdTRUE, x_Timer_Is_Timer_ਸਰਗਰਮ(recurring));
    TEST_ASSERT_EQUAL(0, ਗਿਣਤੀ_2);

    vTaskDelay(250 / ਪੋਰਟ_TICK_PERIOD_MS); // 2.5 timer periods

    TEST_ASSERT_EQUAL(2, ਗਿਣਤੀ_2);
    TEST_ASSERT_EQUAL(pdTRUE, x_Timer_Is_Timer_ਸਰਗਰਮ(recurring));

    TEST_ASSERT(xTimerStop(recurring, 1));

    TEST_ASSERT_EQUAL(2, ਗਿਣਤੀ_2);

    vTaskDelay(100 / ਪੋਰਟ_TICK_PERIOD_MS); // One more timer period
    TEST_ASSERT_EQUAL(2, ਗਿਣਤੀ_2); // hasn't gone up
    TEST_ASSERT_EQUAL(pdFALSE, x_Timer_Is_Timer_ਸਰਗਰਮ(recurring));

    TEST_ASSERT(xTimerDelete(recurring, 1));
}

TEST_CASE("Static timer creation", "[freertos]")
{
    StaticTimer_t static_timer;
    TimerHandle_t created_timer;
    ਅਸਥਿਰ ਪੂਰਨ ਗਿਣਤੀ_2 = 0;

    created_timer = xTimerCreateStatic("oneshot", 100 / ਪੋਰਟ_TICK_PERIOD_MS,
                                       pdTRUE,
                                       (ਖਾਲੀ *)&ਗਿਣਤੀ_2,
                                       timer_callback,
                                       &static_timer);

    TEST_ASSERT_NOT_NULL(created_timer);
}
