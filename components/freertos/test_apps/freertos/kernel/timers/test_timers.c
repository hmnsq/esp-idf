/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* FreeRTOS timer tests
*/
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ "unity.h"
#அந்தர்பாவ "freertos/FreeRTOS.h"
#அந்தர்பாவ "freertos/task.h"
#அந்தர்பாவ "freertos/timers.h"
#அந்தர்பாவ "test_utils.h"

நிலையான வெறுமை timer_callback(TimerHandle_t timer)
{
    நிலையற்ற முழு *மொத்தம்;
    மொத்தம் = (நிலையற்ற முழு *)pvTimerGetTimerID(timer);
    (*மொத்தம்)++;
    printf("Callback timer %p count %p = %d\n", timer, மொத்தம், *மொத்தம்);
}

TEST_CASE("Oneshot FreeRTOS timers", "[freertos]")
{
    நிலையற்ற முழு மொத்தம் = 0;
    TimerHandle_t oneshot = xTimerCreate("oneshot", 100 / portTICK_PERIOD_MS, pdFALSE,
                                         (வெறுமை *)&மொத்தம், timer_callback);
    TEST_ASSERT(oneshot);
    TEST_ASSERT_EQUAL(pdFALSE, xTimerIsTimerActive(oneshot));
    TEST_ASSERT_EQUAL(0, மொத்தம்);

    TEST_ASSERT(xTimerStart(oneshot, 1));
    vTaskDelay(2); /* give the timer task a chance to process the message */

    TEST_ASSERT_EQUAL(pdTRUE, xTimerIsTimerActive(oneshot));
    TEST_ASSERT_EQUAL(0, மொத்தம்);

    vTaskDelay(250 / portTICK_PERIOD_MS); // 2.5 timer periods

    TEST_ASSERT_EQUAL(1, மொத்தம்);
    TEST_ASSERT_EQUAL(pdFALSE, xTimerIsTimerActive(oneshot));

    TEST_ASSERT(xTimerDelete(oneshot, 1));
}

TEST_CASE("Recurring FreeRTOS timers", "[freertos]")
{
    நிலையற்ற முழு மொத்தம் = 0;
    TimerHandle_t recurring = xTimerCreate("oneshot", 100 / portTICK_PERIOD_MS, pdTRUE,
                                           (வெறுமை *)&மொத்தம், timer_callback);
    TEST_ASSERT(recurring);
    TEST_ASSERT_EQUAL(pdFALSE, xTimerIsTimerActive(recurring));
    TEST_ASSERT_EQUAL(0, மொத்தம்);

    TEST_ASSERT(xTimerStart(recurring, 1));

    vTaskDelay(2); // let timer task process the queue
    TEST_ASSERT_EQUAL(pdTRUE, xTimerIsTimerActive(recurring));
    TEST_ASSERT_EQUAL(0, மொத்தம்);

    vTaskDelay(250 / portTICK_PERIOD_MS); // 2.5 timer periods

    TEST_ASSERT_EQUAL(2, மொத்தம்);
    TEST_ASSERT_EQUAL(pdTRUE, xTimerIsTimerActive(recurring));

    TEST_ASSERT(xTimerStop(recurring, 1));

    TEST_ASSERT_EQUAL(2, மொத்தம்);

    vTaskDelay(100 / portTICK_PERIOD_MS); // One more timer period
    TEST_ASSERT_EQUAL(2, மொத்தம்); // hasn't gone up
    TEST_ASSERT_EQUAL(pdFALSE, xTimerIsTimerActive(recurring));

    TEST_ASSERT(xTimerDelete(recurring, 1));
}

TEST_CASE("Static timer creation", "[freertos]")
{
    StaticTimer_t static_timer;
    TimerHandle_t created_timer;
    நிலையற்ற முழு மொத்தம் = 0;

    created_timer = xTimerCreateStatic("oneshot", 100 / portTICK_PERIOD_MS,
                                       pdTRUE,
                                       (வெறுமை *)&மொத்தம்,
                                       timer_callback,
                                       &static_timer);

    TEST_ASSERT_NOT_NULL(created_timer);
}
