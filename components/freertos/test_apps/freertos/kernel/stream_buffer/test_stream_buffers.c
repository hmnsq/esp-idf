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

#وصف_ڪريو TEST_انگ_بائيٽون                  100
#وصف_ڪريو TEST_RECEIVER_TIMEOUT_TICKS     pd_MS_منزل_TICKS(1000)    // 1ms timeout for receiver task

تعريفنوع بنية {
    وهڪرو_بفر_Handle_t وهڪرو_بفر;
    SemaphoreHandle_t پورو_sem;
} test_دليلون_t;

ساكن فراغ sender_task(فراغ *arg)
{
    test_دليلون_t *test_دليلون = (test_دليلون_t *)arg;
    لکو("Starting sender task... \n");

    لكل (صحيح i = 0; i < TEST_انگ_بائيٽون; i++) {
        // Send a single byte, with the byte's value being the number of bytes sent thus far
        uint8_t معلومات = (uint8_t)i;
        TEST_ASSERT_EQUAL(1, x_وهڪرو_بفر_Send(test_دليلون->وهڪرو_بفر, &معلومات, 1, 0));
        // Short delay to give a chance for receiver task to receive
        vTaskDelay(1);
    }

    xSemaphoreGive(test_دليلون->پورو_sem);
    vTaskDelete(عدم);
}

ساكن فراغ receiver_task(فراغ *arg)
{
    test_دليلون_t *test_دليلون = (test_دليلون_t *)arg;
    لکو("Starting receiver task... \n");

    لكل (صحيح i = 0; i < TEST_انگ_بائيٽون; i++) {
        // Receive  a single byte. The received byte's value being the number of bytes sent/received thus far
        uint8_t معلومات;
        TEST_ASSERT_EQUAL(1, x_وهڪرو_بفر_Receive(test_دليلون->وهڪرو_بفر, &معلومات, 1, TEST_RECEIVER_TIMEOUT_TICKS));
        TEST_ASSERT_EQUAL(i, معلومات);
    }

    xSemaphoreGive(test_دليلون->پورو_sem);
    vTaskDelete(عدم);
}

TEST_CASE("Stream Buffer: Send-receive tasks", "[freertos]")
{
    test_دليلون_t test_دليلون;
    test_دليلون.وهڪرو_بفر = x_وهڪرو_بفر_Create(TEST_انگ_بائيٽون, 1);
    test_دليلون.پورو_sem = xSemaphoreCreateCounting(2, 0);
    TEST_ASSERT_NOT_NULL(test_دليلون.وهڪرو_بفر);
    TEST_ASSERT_NOT_NULL(test_دليلون.پورو_sem);
    بنياد_Type_t sender_core_id;
    بنياد_Type_t receiver_core_id;
    sender_core_id = 0;
#إذا CONFIG_FREERTOS_UNICORE
    receiver_core_id = 0;
#وإلا
    receiver_core_id = 1;
#جيڪڏهن_پورو
    TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_منزل_Core(sender_task, "sender", 4096, &test_دليلون, UNITY_FREERTOS_ترجيح + 2, عدم, sender_core_id));
    TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_منزل_Core(receiver_task, "receiver", 4096, &test_دليلون, UNITY_FREERTOS_ترجيح + 1, عدم, receiver_core_id));

    // Wait for both tasks to complete
    لكل (صحيح i = 0; i < 2; i++) {
        xSemaphoreTake(test_دليلون.پورو_sem, پورٽ_MAX_DELAY);
    }

    v_وهڪرو_بفر_Delete(test_دليلون.وهڪرو_بفر);
    vSemaphoreDelete(test_دليلون.پورو_sem);
}
