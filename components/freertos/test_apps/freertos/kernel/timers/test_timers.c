/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* FreeRTOS timer tests
*/
#అన్తర్భావ <stdio.h>
#అన్తర్భావ "unity.h"
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "freertos/timers.h"
#అన్తర్భావ "test_utils.h"

స్థిర శూన్య timer_callback(TimerHandle_t timer)
{
    అస్థిర పూర్ణాంకం *లెక్క;
    లెక్క = (అస్థిర పూర్ణాంకం *)pvTimerGetTimerID(timer);
    (*లెక్క)++;
    రాయి("Callback timer %p count %p = %d\n", timer, లెక్క, *లెక్క);
}

TEST_CASE("Oneshot FreeRTOS timers", "[freertos]")
{
    అస్థిర పూర్ణాంకం లెక్క = 0;
    TimerHandle_t oneshot = xTimerCreate("oneshot", 100 / పోర్టు_TICK_PERIOD_MS, pdFALSE,
                                         (శూన్య *)&లెక్క, timer_callback);
    TEST_ASSERT(oneshot);
    TEST_ASSERT_EQUAL(pdFALSE, x_Timer_Is_Timer_చురుకైన(oneshot));
    TEST_ASSERT_EQUAL(0, లెక్క);

    TEST_ASSERT(x_Timer_ప్రారంభం(oneshot, 1));
    vTaskDelay(2); /* give the timer task a chance to process the message */

    TEST_ASSERT_EQUAL(pdTRUE, x_Timer_Is_Timer_చురుకైన(oneshot));
    TEST_ASSERT_EQUAL(0, లెక్క);

    vTaskDelay(250 / పోర్టు_TICK_PERIOD_MS); // 2.5 timer periods

    TEST_ASSERT_EQUAL(1, లెక్క);
    TEST_ASSERT_EQUAL(pdFALSE, x_Timer_Is_Timer_చురుకైన(oneshot));

    TEST_ASSERT(xTimerDelete(oneshot, 1));
}

TEST_CASE("Recurring FreeRTOS timers", "[freertos]")
{
    అస్థిర పూర్ణాంకం లెక్క = 0;
    TimerHandle_t recurring = xTimerCreate("oneshot", 100 / పోర్టు_TICK_PERIOD_MS, pdTRUE,
                                           (శూన్య *)&లెక్క, timer_callback);
    TEST_ASSERT(recurring);
    TEST_ASSERT_EQUAL(pdFALSE, x_Timer_Is_Timer_చురుకైన(recurring));
    TEST_ASSERT_EQUAL(0, లెక్క);

    TEST_ASSERT(x_Timer_ప్రారంభం(recurring, 1));

    vTaskDelay(2); // let timer task process the queue
    TEST_ASSERT_EQUAL(pdTRUE, x_Timer_Is_Timer_చురుకైన(recurring));
    TEST_ASSERT_EQUAL(0, లెక్క);

    vTaskDelay(250 / పోర్టు_TICK_PERIOD_MS); // 2.5 timer periods

    TEST_ASSERT_EQUAL(2, లెక్క);
    TEST_ASSERT_EQUAL(pdTRUE, x_Timer_Is_Timer_చురుకైన(recurring));

    TEST_ASSERT(xTimerStop(recurring, 1));

    TEST_ASSERT_EQUAL(2, లెక్క);

    vTaskDelay(100 / పోర్టు_TICK_PERIOD_MS); // One more timer period
    TEST_ASSERT_EQUAL(2, లెక్క); // hasn't gone up
    TEST_ASSERT_EQUAL(pdFALSE, x_Timer_Is_Timer_చురుకైన(recurring));

    TEST_ASSERT(xTimerDelete(recurring, 1));
}

TEST_CASE("Static timer creation", "[freertos]")
{
    StaticTimer_t static_timer;
    TimerHandle_t created_timer;
    అస్థిర పూర్ణాంకం లెక్క = 0;

    created_timer = xTimerCreateStatic("oneshot", 100 / పోర్టు_TICK_PERIOD_MS,
                                       pdTRUE,
                                       (శూన్య *)&లెక్క,
                                       timer_callback,
                                       &static_timer);

    TEST_ASSERT_NOT_NULL(created_timer);
}
