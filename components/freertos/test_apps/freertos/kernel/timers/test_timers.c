/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* FreeRTOS timer tests
*/
#ضمن <stdio.h>
#ضمن "unity.h"
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "freertos/timers.h"
#ضمن "test_utils.h"

ساكن فراغ timer_callback(TimerHandle_t timer)
{
    متطاير صحيح *شمار;
    شمار = (متطاير صحيح *)pvTimerGetTimerID(timer);
    (*شمار)++;
    چاپ_کن("Callback timer %p count %p = %d\n", timer, شمار, *شمار);
}

TEST_CASE("Oneshot FreeRTOS timers", "[freertos]")
{
    متطاير صحيح شمار = 0;
    TimerHandle_t oneshot = xTimerCreate("oneshot", 100 / پورت_TICK_PERIOD_MS, pdFALSE,
                                         (فراغ *)&شمار, timer_callback);
    TEST_ASSERT(oneshot);
    TEST_ASSERT_EQUAL(pdFALSE, x_Timer_Is_Timer_فعال(oneshot));
    TEST_ASSERT_EQUAL(0, شمار);

    TEST_ASSERT(x_Timer_شروع(oneshot, 1));
    vTaskDelay(2); /* give the timer task a chance to process the message */

    TEST_ASSERT_EQUAL(pdTRUE, x_Timer_Is_Timer_فعال(oneshot));
    TEST_ASSERT_EQUAL(0, شمار);

    vTaskDelay(250 / پورت_TICK_PERIOD_MS); // 2.5 timer periods

    TEST_ASSERT_EQUAL(1, شمار);
    TEST_ASSERT_EQUAL(pdFALSE, x_Timer_Is_Timer_فعال(oneshot));

    TEST_ASSERT(xTimerDelete(oneshot, 1));
}

TEST_CASE("Recurring FreeRTOS timers", "[freertos]")
{
    متطاير صحيح شمار = 0;
    TimerHandle_t recurring = xTimerCreate("oneshot", 100 / پورت_TICK_PERIOD_MS, pdTRUE,
                                           (فراغ *)&شمار, timer_callback);
    TEST_ASSERT(recurring);
    TEST_ASSERT_EQUAL(pdFALSE, x_Timer_Is_Timer_فعال(recurring));
    TEST_ASSERT_EQUAL(0, شمار);

    TEST_ASSERT(x_Timer_شروع(recurring, 1));

    vTaskDelay(2); // let timer task process the queue
    TEST_ASSERT_EQUAL(pdTRUE, x_Timer_Is_Timer_فعال(recurring));
    TEST_ASSERT_EQUAL(0, شمار);

    vTaskDelay(250 / پورت_TICK_PERIOD_MS); // 2.5 timer periods

    TEST_ASSERT_EQUAL(2, شمار);
    TEST_ASSERT_EQUAL(pdTRUE, x_Timer_Is_Timer_فعال(recurring));

    TEST_ASSERT(xTimerStop(recurring, 1));

    TEST_ASSERT_EQUAL(2, شمار);

    vTaskDelay(100 / پورت_TICK_PERIOD_MS); // One more timer period
    TEST_ASSERT_EQUAL(2, شمار); // hasn't gone up
    TEST_ASSERT_EQUAL(pdFALSE, x_Timer_Is_Timer_فعال(recurring));

    TEST_ASSERT(xTimerDelete(recurring, 1));
}

TEST_CASE("Static timer creation", "[freertos]")
{
    StaticTimer_t static_timer;
    TimerHandle_t created_timer;
    متطاير صحيح شمار = 0;

    created_timer = xTimerCreateStatic("oneshot", 100 / پورت_TICK_PERIOD_MS,
                                       pdTRUE,
                                       (فراغ *)&شمار,
                                       timer_callback,
                                       &static_timer);

    TEST_ASSERT_NOT_NULL(created_timer);
}
