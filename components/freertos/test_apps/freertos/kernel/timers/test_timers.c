/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* FreeRTOS timer tests
*/
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ "unity.h"
#ଅନ୍ତର୍ଭାଵ "freertos/FreeRTOS.h"
#ଅନ୍ତର୍ଭାଵ "freertos/task.h"
#ଅନ୍ତର୍ଭାଵ "freertos/timers.h"
#ଅନ୍ତର୍ଭାଵ "test_utils.h"

ସ୍ଥିର ଶୂନ୍ୟ timer_callback(TimerHandle_t timer)
{
    ଅସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ଗଣନା_2;
    ଗଣନା_2 = (ଅସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା *)pvTimerGetTimerID(timer);
    (*ଗଣନା_2)++;
    printf("Callback timer %p count %p = %d\n", timer, ଗଣନା_2, *ଗଣନା_2);
}

TEST_CASE("Oneshot FreeRTOS timers", "[freertos]")
{
    ଅସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
    TimerHandle_t oneshot = xTimerCreate("oneshot", 100 / portTICK_PERIOD_MS, pdFALSE,
                                         (ଶୂନ୍ୟ *)&ଗଣନା_2, timer_callback);
    TEST_ASSERT(oneshot);
    TEST_ASSERT_EQUAL(pdFALSE, xTimerIsTimerActive(oneshot));
    TEST_ASSERT_EQUAL(0, ଗଣନା_2);

    TEST_ASSERT(xTimerStart(oneshot, 1));
    vTaskDelay(2); /* give the timer task a chance to process the message */

    TEST_ASSERT_EQUAL(pdTRUE, xTimerIsTimerActive(oneshot));
    TEST_ASSERT_EQUAL(0, ଗଣନା_2);

    vTaskDelay(250 / portTICK_PERIOD_MS); // 2.5 timer periods

    TEST_ASSERT_EQUAL(1, ଗଣନା_2);
    TEST_ASSERT_EQUAL(pdFALSE, xTimerIsTimerActive(oneshot));

    TEST_ASSERT(xTimerDelete(oneshot, 1));
}

TEST_CASE("Recurring FreeRTOS timers", "[freertos]")
{
    ଅସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
    TimerHandle_t recurring = xTimerCreate("oneshot", 100 / portTICK_PERIOD_MS, pdTRUE,
                                           (ଶୂନ୍ୟ *)&ଗଣନା_2, timer_callback);
    TEST_ASSERT(recurring);
    TEST_ASSERT_EQUAL(pdFALSE, xTimerIsTimerActive(recurring));
    TEST_ASSERT_EQUAL(0, ଗଣନା_2);

    TEST_ASSERT(xTimerStart(recurring, 1));

    vTaskDelay(2); // let timer task process the queue
    TEST_ASSERT_EQUAL(pdTRUE, xTimerIsTimerActive(recurring));
    TEST_ASSERT_EQUAL(0, ଗଣନା_2);

    vTaskDelay(250 / portTICK_PERIOD_MS); // 2.5 timer periods

    TEST_ASSERT_EQUAL(2, ଗଣନା_2);
    TEST_ASSERT_EQUAL(pdTRUE, xTimerIsTimerActive(recurring));

    TEST_ASSERT(xTimerStop(recurring, 1));

    TEST_ASSERT_EQUAL(2, ଗଣନା_2);

    vTaskDelay(100 / portTICK_PERIOD_MS); // One more timer period
    TEST_ASSERT_EQUAL(2, ଗଣନା_2); // hasn't gone up
    TEST_ASSERT_EQUAL(pdFALSE, xTimerIsTimerActive(recurring));

    TEST_ASSERT(xTimerDelete(recurring, 1));
}

TEST_CASE("Static timer creation", "[freertos]")
{
    StaticTimer_t static_timer;
    TimerHandle_t created_timer;
    ଅସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;

    created_timer = xTimerCreateStatic("oneshot", 100 / portTICK_PERIOD_MS,
                                       pdTRUE,
                                       (ଶୂନ୍ୟ *)&ଗଣନା_2,
                                       timer_callback,
                                       &static_timer);

    TEST_ASSERT_NOT_NULL(created_timer);
}
