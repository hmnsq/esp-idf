/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* FreeRTOS timer tests
*/
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "freertos/timers.h"
#ಅನ್ತರ್ಭಾವ "test_utils.h"

ಸ್ಥಿರ ಶೂನ್ಯ timer_callback(TimerHandle_t timer)
{
    ಅಸ್ಥಿರ ಪೂರ್ಣಾಂಕ *ಎಣಿಕೆ_2;
    ಎಣಿಕೆ_2 = (ಅಸ್ಥಿರ ಪೂರ್ಣಾಂಕ *)pvTimerGetTimerID(timer);
    (*ಎಣಿಕೆ_2)++;
    ಬರೆ("Callback timer %p count %p = %d\n", timer, ಎಣಿಕೆ_2, *ಎಣಿಕೆ_2);
}

TEST_CASE("Oneshot FreeRTOS timers", "[freertos]")
{
    ಅಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;
    TimerHandle_t oneshot = xTimerCreate("oneshot", 100 / ಪೋರ್ಟ್_TICK_PERIOD_MS, pdFALSE,
                                         (ಶೂನ್ಯ *)&ಎಣಿಕೆ_2, timer_callback);
    TEST_ASSERT(oneshot);
    TEST_ASSERT_EQUAL(pdFALSE, x_Timer_Is_Timer_ಸಕ್ರಿಯ(oneshot));
    TEST_ASSERT_EQUAL(0, ಎಣಿಕೆ_2);

    TEST_ASSERT(x_Timer_ಆರಂಭ(oneshot, 1));
    vTaskDelay(2); /* give the timer task a chance to process the message */

    TEST_ASSERT_EQUAL(pdTRUE, x_Timer_Is_Timer_ಸಕ್ರಿಯ(oneshot));
    TEST_ASSERT_EQUAL(0, ಎಣಿಕೆ_2);

    vTaskDelay(250 / ಪೋರ್ಟ್_TICK_PERIOD_MS); // 2.5 timer periods

    TEST_ASSERT_EQUAL(1, ಎಣಿಕೆ_2);
    TEST_ASSERT_EQUAL(pdFALSE, x_Timer_Is_Timer_ಸಕ್ರಿಯ(oneshot));

    TEST_ASSERT(xTimerDelete(oneshot, 1));
}

TEST_CASE("Recurring FreeRTOS timers", "[freertos]")
{
    ಅಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;
    TimerHandle_t recurring = xTimerCreate("oneshot", 100 / ಪೋರ್ಟ್_TICK_PERIOD_MS, pdTRUE,
                                           (ಶೂನ್ಯ *)&ಎಣಿಕೆ_2, timer_callback);
    TEST_ASSERT(recurring);
    TEST_ASSERT_EQUAL(pdFALSE, x_Timer_Is_Timer_ಸಕ್ರಿಯ(recurring));
    TEST_ASSERT_EQUAL(0, ಎಣಿಕೆ_2);

    TEST_ASSERT(x_Timer_ಆರಂಭ(recurring, 1));

    vTaskDelay(2); // let timer task process the queue
    TEST_ASSERT_EQUAL(pdTRUE, x_Timer_Is_Timer_ಸಕ್ರಿಯ(recurring));
    TEST_ASSERT_EQUAL(0, ಎಣಿಕೆ_2);

    vTaskDelay(250 / ಪೋರ್ಟ್_TICK_PERIOD_MS); // 2.5 timer periods

    TEST_ASSERT_EQUAL(2, ಎಣಿಕೆ_2);
    TEST_ASSERT_EQUAL(pdTRUE, x_Timer_Is_Timer_ಸಕ್ರಿಯ(recurring));

    TEST_ASSERT(xTimerStop(recurring, 1));

    TEST_ASSERT_EQUAL(2, ಎಣಿಕೆ_2);

    vTaskDelay(100 / ಪೋರ್ಟ್_TICK_PERIOD_MS); // One more timer period
    TEST_ASSERT_EQUAL(2, ಎಣಿಕೆ_2); // hasn't gone up
    TEST_ASSERT_EQUAL(pdFALSE, x_Timer_Is_Timer_ಸಕ್ರಿಯ(recurring));

    TEST_ASSERT(xTimerDelete(recurring, 1));
}

TEST_CASE("Static timer creation", "[freertos]")
{
    StaticTimer_t static_timer;
    TimerHandle_t created_timer;
    ಅಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;

    created_timer = xTimerCreateStatic("oneshot", 100 / ಪೋರ್ಟ್_TICK_PERIOD_MS,
                                       pdTRUE,
                                       (ಶೂನ್ಯ *)&ಎಣಿಕೆ_2,
                                       timer_callback,
                                       &static_timer);

    TEST_ASSERT_NOT_NULL(created_timer);
}
