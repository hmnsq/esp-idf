/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* FreeRTOS timer tests
*/
#شامل <stdio.h>
#شامل "unity.h"
#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل "freertos/timers.h"
#شامل "test_utils.h"

ساکن خالی timer_callback(TimerHandle_t timer)
{
    متلون صحیح *گنتی;
    گنتی = (متلون صحیح *)pvTimerGetTimerID(timer);
    (*گنتی)++;
    لکھو("Callback timer %p count %p = %d\n", timer, گنتی, *گنتی);
}

TEST_CASE("Oneshot FreeRTOS timers", "[freertos]")
{
    متلون صحیح گنتی = 0;
    TimerHandle_t oneshot = xTimerCreate("oneshot", 100 / پورٹ_TICK_PERIOD_MS, pdFALSE,
                                         (خالی *)&گنتی, timer_callback);
    TEST_ASSERT(oneshot);
    TEST_ASSERT_EQUAL(pdFALSE, x_Timer_Is_Timer_فعال(oneshot));
    TEST_ASSERT_EQUAL(0, گنتی);

    TEST_ASSERT(x_Timer_آغاز(oneshot, 1));
    vTaskDelay(2); /* give the timer task a chance to process the message */

    TEST_ASSERT_EQUAL(pdTRUE, x_Timer_Is_Timer_فعال(oneshot));
    TEST_ASSERT_EQUAL(0, گنتی);

    vTaskDelay(250 / پورٹ_TICK_PERIOD_MS); // 2.5 timer periods

    TEST_ASSERT_EQUAL(1, گنتی);
    TEST_ASSERT_EQUAL(pdFALSE, x_Timer_Is_Timer_فعال(oneshot));

    TEST_ASSERT(xTimerDelete(oneshot, 1));
}

TEST_CASE("Recurring FreeRTOS timers", "[freertos]")
{
    متلون صحیح گنتی = 0;
    TimerHandle_t recurring = xTimerCreate("oneshot", 100 / پورٹ_TICK_PERIOD_MS, pdTRUE,
                                           (خالی *)&گنتی, timer_callback);
    TEST_ASSERT(recurring);
    TEST_ASSERT_EQUAL(pdFALSE, x_Timer_Is_Timer_فعال(recurring));
    TEST_ASSERT_EQUAL(0, گنتی);

    TEST_ASSERT(x_Timer_آغاز(recurring, 1));

    vTaskDelay(2); // let timer task process the queue
    TEST_ASSERT_EQUAL(pdTRUE, x_Timer_Is_Timer_فعال(recurring));
    TEST_ASSERT_EQUAL(0, گنتی);

    vTaskDelay(250 / پورٹ_TICK_PERIOD_MS); // 2.5 timer periods

    TEST_ASSERT_EQUAL(2, گنتی);
    TEST_ASSERT_EQUAL(pdTRUE, x_Timer_Is_Timer_فعال(recurring));

    TEST_ASSERT(xTimerStop(recurring, 1));

    TEST_ASSERT_EQUAL(2, گنتی);

    vTaskDelay(100 / پورٹ_TICK_PERIOD_MS); // One more timer period
    TEST_ASSERT_EQUAL(2, گنتی); // hasn't gone up
    TEST_ASSERT_EQUAL(pdFALSE, x_Timer_Is_Timer_فعال(recurring));

    TEST_ASSERT(xTimerDelete(recurring, 1));
}

TEST_CASE("Static timer creation", "[freertos]")
{
    StaticTimer_t static_timer;
    TimerHandle_t created_timer;
    متلون صحیح گنتی = 0;

    created_timer = xTimerCreateStatic("oneshot", 100 / پورٹ_TICK_PERIOD_MS,
                                       pdTRUE,
                                       (خالی *)&گنتی,
                                       timer_callback,
                                       &static_timer);

    TEST_ASSERT_NOT_NULL(created_timer);
}
