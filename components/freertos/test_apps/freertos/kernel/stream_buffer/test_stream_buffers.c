/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/semphr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/stream_buffer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "test_utils.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_ਅੰਕ_ਬਾਈਟਾਂ                  100
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_RECEIVER_TIMEOUT_TICKS     pd_MS_ਮੰਜ਼ਿਲ_TICKS(1000)    // 1ms timeout for receiver task

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ਧਾਰਾ_ਬਫਰ_Handle_t ਧਾਰਾ_ਬਫਰ;
    SemaphoreHandle_t ਪੂਰਾ_sem;
} test_ਦਲੀਲਾਂ_t;

ਸਥਿਰ ਖਾਲੀ sender_task(ਖਾਲੀ *arg)
{
    test_ਦਲੀਲਾਂ_t *test_ਦਲੀਲਾਂ = (test_ਦਲੀਲਾਂ_t *)arg;
    ਲਿਖੋ("Starting sender task... \n");

    ਲਈ (ਪੂਰਨ i = 0; i < TEST_ਅੰਕ_ਬਾਈਟਾਂ; i++) {
        // Send a single byte, with the byte's value being the number of bytes sent thus far
        uint8_t ਅੰਕੜੇ = (uint8_t)i;
        TEST_ASSERT_EQUAL(1, x_ਧਾਰਾ_ਬਫਰ_Send(test_ਦਲੀਲਾਂ->ਧਾਰਾ_ਬਫਰ, &ਅੰਕੜੇ, 1, 0));
        // Short delay to give a chance for receiver task to receive
        vTaskDelay(1);
    }

    xSemaphoreGive(test_ਦਲੀਲਾਂ->ਪੂਰਾ_sem);
    vTaskDelete(ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ receiver_task(ਖਾਲੀ *arg)
{
    test_ਦਲੀਲਾਂ_t *test_ਦਲੀਲਾਂ = (test_ਦਲੀਲਾਂ_t *)arg;
    ਲਿਖੋ("Starting receiver task... \n");

    ਲਈ (ਪੂਰਨ i = 0; i < TEST_ਅੰਕ_ਬਾਈਟਾਂ; i++) {
        // Receive  a single byte. The received byte's value being the number of bytes sent/received thus far
        uint8_t ਅੰਕੜੇ;
        TEST_ASSERT_EQUAL(1, x_ਧਾਰਾ_ਬਫਰ_Receive(test_ਦਲੀਲਾਂ->ਧਾਰਾ_ਬਫਰ, &ਅੰਕੜੇ, 1, TEST_RECEIVER_TIMEOUT_TICKS));
        TEST_ASSERT_EQUAL(i, ਅੰਕੜੇ);
    }

    xSemaphoreGive(test_ਦਲੀਲਾਂ->ਪੂਰਾ_sem);
    vTaskDelete(ਨਿਰਾ);
}

TEST_CASE("Stream Buffer: Send-receive tasks", "[freertos]")
{
    test_ਦਲੀਲਾਂ_t test_ਦਲੀਲਾਂ;
    test_ਦਲੀਲਾਂ.ਧਾਰਾ_ਬਫਰ = x_ਧਾਰਾ_ਬਫਰ_Create(TEST_ਅੰਕ_ਬਾਈਟਾਂ, 1);
    test_ਦਲੀਲਾਂ.ਪੂਰਾ_sem = xSemaphoreCreateCounting(2, 0);
    TEST_ASSERT_NOT_NULL(test_ਦਲੀਲਾਂ.ਧਾਰਾ_ਬਫਰ);
    TEST_ASSERT_NOT_NULL(test_ਦਲੀਲਾਂ.ਪੂਰਾ_sem);
    ਅਧਾਰ_Type_t sender_core_id;
    ਅਧਾਰ_Type_t receiver_core_id;
    sender_core_id = 0;
#ਜੇ CONFIG_FREERTOS_UNICORE
    receiver_core_id = 0;
#ਨਹੀਂਤਾਂ
    receiver_core_id = 1;
#ਜੇ_ਅੰਤ
    TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(sender_task, "sender", 4096, &test_ਦਲੀਲਾਂ, UNITY_FREERTOS_ਤਰਜੀਹ + 2, ਨਿਰਾ, sender_core_id));
    TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(receiver_task, "receiver", 4096, &test_ਦਲੀਲਾਂ, UNITY_FREERTOS_ਤਰਜੀਹ + 1, ਨਿਰਾ, receiver_core_id));

    // Wait for both tasks to complete
    ਲਈ (ਪੂਰਨ i = 0; i < 2; i++) {
        xSemaphoreTake(test_ਦਲੀਲਾਂ.ਪੂਰਾ_sem, ਪੋਰਟ_MAX_DELAY);
    }

    v_ਧਾਰਾ_ਬਫਰ_Delete(test_ਦਲੀਲਾਂ.ਧਾਰਾ_ਬਫਰ);
    vSemaphoreDelete(test_ਦਲੀਲਾਂ.ਪੂਰਾ_sem);
}
