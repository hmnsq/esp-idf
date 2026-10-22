/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* FreeRTOS timer tests
*/
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ "unity.h"
#അന്തര്ഭാവ "freertos/FreeRTOS.h"
#അന്തര്ഭാവ "freertos/task.h"
#അന്തര്ഭാവ "freertos/timers.h"
#അന്തര്ഭാവ "test_utils.h"

സ്ഥിതം ശൂന്യം timer_callback(TimerHandle_t timer)
{
    അസ്ഥിര പൂർണ്ണസംഖ്യ *എണ്ണം;
    എണ്ണം = (അസ്ഥിര പൂർണ്ണസംഖ്യ *)pvTimerGetTimerID(timer);
    (*എണ്ണം)++;
    printf("Callback timer %p count %p = %d\n", timer, എണ്ണം, *എണ്ണം);
}

TEST_CASE("Oneshot FreeRTOS timers", "[freertos]")
{
    അസ്ഥിര പൂർണ്ണസംഖ്യ എണ്ണം = 0;
    TimerHandle_t oneshot = xTimerCreate("oneshot", 100 / portTICK_PERIOD_MS, pdFALSE,
                                         (ശൂന്യം *)&എണ്ണം, timer_callback);
    TEST_ASSERT(oneshot);
    TEST_ASSERT_EQUAL(pdFALSE, xTimerIsTimerActive(oneshot));
    TEST_ASSERT_EQUAL(0, എണ്ണം);

    TEST_ASSERT(xTimerStart(oneshot, 1));
    vTaskDelay(2); /* give the timer task a chance to process the message */

    TEST_ASSERT_EQUAL(pdTRUE, xTimerIsTimerActive(oneshot));
    TEST_ASSERT_EQUAL(0, എണ്ണം);

    vTaskDelay(250 / portTICK_PERIOD_MS); // 2.5 timer periods

    TEST_ASSERT_EQUAL(1, എണ്ണം);
    TEST_ASSERT_EQUAL(pdFALSE, xTimerIsTimerActive(oneshot));

    TEST_ASSERT(xTimerDelete(oneshot, 1));
}

TEST_CASE("Recurring FreeRTOS timers", "[freertos]")
{
    അസ്ഥിര പൂർണ്ണസംഖ്യ എണ്ണം = 0;
    TimerHandle_t recurring = xTimerCreate("oneshot", 100 / portTICK_PERIOD_MS, pdTRUE,
                                           (ശൂന്യം *)&എണ്ണം, timer_callback);
    TEST_ASSERT(recurring);
    TEST_ASSERT_EQUAL(pdFALSE, xTimerIsTimerActive(recurring));
    TEST_ASSERT_EQUAL(0, എണ്ണം);

    TEST_ASSERT(xTimerStart(recurring, 1));

    vTaskDelay(2); // let timer task process the queue
    TEST_ASSERT_EQUAL(pdTRUE, xTimerIsTimerActive(recurring));
    TEST_ASSERT_EQUAL(0, എണ്ണം);

    vTaskDelay(250 / portTICK_PERIOD_MS); // 2.5 timer periods

    TEST_ASSERT_EQUAL(2, എണ്ണം);
    TEST_ASSERT_EQUAL(pdTRUE, xTimerIsTimerActive(recurring));

    TEST_ASSERT(xTimerStop(recurring, 1));

    TEST_ASSERT_EQUAL(2, എണ്ണം);

    vTaskDelay(100 / portTICK_PERIOD_MS); // One more timer period
    TEST_ASSERT_EQUAL(2, എണ്ണം); // hasn't gone up
    TEST_ASSERT_EQUAL(pdFALSE, xTimerIsTimerActive(recurring));

    TEST_ASSERT(xTimerDelete(recurring, 1));
}

TEST_CASE("Static timer creation", "[freertos]")
{
    StaticTimer_t static_timer;
    TimerHandle_t created_timer;
    അസ്ഥിര പൂർണ്ണസംഖ്യ എണ്ണം = 0;

    created_timer = xTimerCreateStatic("oneshot", 100 / portTICK_PERIOD_MS,
                                       pdTRUE,
                                       (ശൂന്യം *)&എണ്ണം,
                                       timer_callback,
                                       &static_timer);

    TEST_ASSERT_NOT_NULL(created_timer);
}
