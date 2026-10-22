/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#شامل <stdint.h>
#شامل <stdio.h>
#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل "freertos/semphr.h"
#شامل "freertos/stream_buffer.h"
#شامل "unity.h"
#شامل "test_utils.h"

#تعریف_کرو TEST_عدد_بائٹیں                  100
#تعریف_کرو TEST_RECEIVER_TIMEOUT_TICKS     pd_MS_مقصد_TICKS(1000)    // 1ms timeout for receiver task

قسمتعریف ساخت {
    دھارا_بفر_Handle_t دھارا_بفر;
    SemaphoreHandle_t مکمل_sem;
} test_دلائل_t;

ساکن خالی sender_task(خالی *arg)
{
    test_دلائل_t *test_دلائل = (test_دلائل_t *)arg;
    لکھو("Starting sender task... \n");

    برائے (صحیح i = 0; i < TEST_عدد_بائٹیں; i++) {
        // Send a single byte, with the byte's value being the number of bytes sent thus far
        uint8_t ڈیٹا = (uint8_t)i;
        TEST_ASSERT_EQUAL(1, x_دھارا_بفر_Send(test_دلائل->دھارا_بفر, &ڈیٹا, 1, 0));
        // Short delay to give a chance for receiver task to receive
        vTaskDelay(1);
    }

    xSemaphoreGive(test_دلائل->مکمل_sem);
    vTaskDelete(تہی);
}

ساکن خالی receiver_task(خالی *arg)
{
    test_دلائل_t *test_دلائل = (test_دلائل_t *)arg;
    لکھو("Starting receiver task... \n");

    برائے (صحیح i = 0; i < TEST_عدد_بائٹیں; i++) {
        // Receive  a single byte. The received byte's value being the number of bytes sent/received thus far
        uint8_t ڈیٹا;
        TEST_ASSERT_EQUAL(1, x_دھارا_بفر_Receive(test_دلائل->دھارا_بفر, &ڈیٹا, 1, TEST_RECEIVER_TIMEOUT_TICKS));
        TEST_ASSERT_EQUAL(i, ڈیٹا);
    }

    xSemaphoreGive(test_دلائل->مکمل_sem);
    vTaskDelete(تہی);
}

TEST_CASE("Stream Buffer: Send-receive tasks", "[freertos]")
{
    test_دلائل_t test_دلائل;
    test_دلائل.دھارا_بفر = x_دھارا_بفر_Create(TEST_عدد_بائٹیں, 1);
    test_دلائل.مکمل_sem = xSemaphoreCreateCounting(2, 0);
    TEST_ASSERT_NOT_NULL(test_دلائل.دھارا_بفر);
    TEST_ASSERT_NOT_NULL(test_دلائل.مکمل_sem);
    بنیاد_Type_t sender_core_id;
    بنیاد_Type_t receiver_core_id;
    sender_core_id = 0;
#اگر CONFIG_FREERTOS_UNICORE
    receiver_core_id = 0;
#ورنہ
    receiver_core_id = 1;
#اگر_ختم
    TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_مقصد_Core(sender_task, "sender", 4096, &test_دلائل, UNITY_FREERTOS_ترجیح + 2, تہی, sender_core_id));
    TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_مقصد_Core(receiver_task, "receiver", 4096, &test_دلائل, UNITY_FREERTOS_ترجیح + 1, تہی, receiver_core_id));

    // Wait for both tasks to complete
    برائے (صحیح i = 0; i < 2; i++) {
        xSemaphoreTake(test_دلائل.مکمل_sem, پورٹ_MAX_DELAY);
    }

    v_دھارا_بفر_Delete(test_دلائل.دھارا_بفر);
    vSemaphoreDelete(test_دلائل.مکمل_sem);
}
