/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن <stdint.h>
#ضمن <stdio.h>
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "freertos/semphr.h"
#ضمن "freertos/stream_buffer.h"
#ضمن "unity.h"
#ضمن "test_utils.h"

#تعریف_کرو TEST_عدد_بائٹاں                  100
#تعریف_کرو TEST_RECEIVER_TIMEOUT_TICKS     pd_MS_منزل_TICKS(1000)    // 1ms timeout for receiver task

تعريفنوع بنية {
    دھارا_بفر_Handle_t دھارا_بفر;
    SemaphoreHandle_t پورا_sem;
} test_دلیلاں_t;

ساكن فراغ sender_task(فراغ *arg)
{
    test_دلیلاں_t *test_دلیلاں = (test_دلیلاں_t *)arg;
    لکھو("Starting sender task... \n");

    لكل (صحيح i = 0; i < TEST_عدد_بائٹاں; i++) {
        // Send a single byte, with the byte's value being the number of bytes sent thus far
        uint8_t انکڑے = (uint8_t)i;
        TEST_ASSERT_EQUAL(1, x_دھارا_بفر_Send(test_دلیلاں->دھارا_بفر, &انکڑے, 1, 0));
        // Short delay to give a chance for receiver task to receive
        vTaskDelay(1);
    }

    xSemaphoreGive(test_دلیلاں->پورا_sem);
    vTaskDelete(عدم);
}

ساكن فراغ receiver_task(فراغ *arg)
{
    test_دلیلاں_t *test_دلیلاں = (test_دلیلاں_t *)arg;
    لکھو("Starting receiver task... \n");

    لكل (صحيح i = 0; i < TEST_عدد_بائٹاں; i++) {
        // Receive  a single byte. The received byte's value being the number of bytes sent/received thus far
        uint8_t انکڑے;
        TEST_ASSERT_EQUAL(1, x_دھارا_بفر_Receive(test_دلیلاں->دھارا_بفر, &انکڑے, 1, TEST_RECEIVER_TIMEOUT_TICKS));
        TEST_ASSERT_EQUAL(i, انکڑے);
    }

    xSemaphoreGive(test_دلیلاں->پورا_sem);
    vTaskDelete(عدم);
}

TEST_CASE("Stream Buffer: Send-receive tasks", "[freertos]")
{
    test_دلیلاں_t test_دلیلاں;
    test_دلیلاں.دھارا_بفر = x_دھارا_بفر_Create(TEST_عدد_بائٹاں, 1);
    test_دلیلاں.پورا_sem = xSemaphoreCreateCounting(2, 0);
    TEST_ASSERT_NOT_NULL(test_دلیلاں.دھارا_بفر);
    TEST_ASSERT_NOT_NULL(test_دلیلاں.پورا_sem);
    ادھار_Type_t sender_core_id;
    ادھار_Type_t receiver_core_id;
    sender_core_id = 0;
#إذا CONFIG_FREERTOS_UNICORE
    receiver_core_id = 0;
#وإلا
    receiver_core_id = 1;
#جے_انت
    TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_منزل_Core(sender_task, "sender", 4096, &test_دلیلاں, UNITY_FREERTOS_ترجیح + 2, عدم, sender_core_id));
    TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_منزل_Core(receiver_task, "receiver", 4096, &test_دلیلاں, UNITY_FREERTOS_ترجیح + 1, عدم, receiver_core_id));

    // Wait for both tasks to complete
    لكل (صحيح i = 0; i < 2; i++) {
        xSemaphoreTake(test_دلیلاں.پورا_sem, پورٹ_MAX_DELAY);
    }

    v_دھارا_بفر_Delete(test_دلیلاں.دھارا_بفر);
    vSemaphoreDelete(test_دلیلاں.پورا_sem);
}
