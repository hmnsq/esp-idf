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

#تعریف_کن TEST_رقم_بایتها                  100
#تعریف_کن TEST_RECEIVER_TIMEOUT_TICKS     pd_MS_مقصد_TICKS(1000)    // 1ms timeout for receiver task

تعريفنوع بنية {
    جریان_میانگیر_Handle_t جریان_میانگیر;
    SemaphoreHandle_t انجامشده_sem;
} test_آرگومانها_t;

ساكن فراغ sender_task(فراغ *arg)
{
    test_آرگومانها_t *test_آرگومانها = (test_آرگومانها_t *)arg;
    چاپ_کن("Starting sender task... \n");

    لكل (صحيح i = 0; i < TEST_رقم_بایتها; i++) {
        // Send a single byte, with the byte's value being the number of bytes sent thus far
        uint8_t داده = (uint8_t)i;
        TEST_ASSERT_EQUAL(1, x_جریان_میانگیر_Send(test_آرگومانها->جریان_میانگیر, &داده, 1, 0));
        // Short delay to give a chance for receiver task to receive
        vTaskDelay(1);
    }

    xSemaphoreGive(test_آرگومانها->انجامشده_sem);
    vTaskDelete(عدم);
}

ساكن فراغ receiver_task(فراغ *arg)
{
    test_آرگومانها_t *test_آرگومانها = (test_آرگومانها_t *)arg;
    چاپ_کن("Starting receiver task... \n");

    لكل (صحيح i = 0; i < TEST_رقم_بایتها; i++) {
        // Receive  a single byte. The received byte's value being the number of bytes sent/received thus far
        uint8_t داده;
        TEST_ASSERT_EQUAL(1, x_جریان_میانگیر_Receive(test_آرگومانها->جریان_میانگیر, &داده, 1, TEST_RECEIVER_TIMEOUT_TICKS));
        TEST_ASSERT_EQUAL(i, داده);
    }

    xSemaphoreGive(test_آرگومانها->انجامشده_sem);
    vTaskDelete(عدم);
}

TEST_CASE("Stream Buffer: Send-receive tasks", "[freertos]")
{
    test_آرگومانها_t test_آرگومانها;
    test_آرگومانها.جریان_میانگیر = x_جریان_میانگیر_Create(TEST_رقم_بایتها, 1);
    test_آرگومانها.انجامشده_sem = xSemaphoreCreateCounting(2, 0);
    TEST_ASSERT_NOT_NULL(test_آرگومانها.جریان_میانگیر);
    TEST_ASSERT_NOT_NULL(test_آرگومانها.انجامشده_sem);
    پایه_Type_t sender_core_id;
    پایه_Type_t receiver_core_id;
    sender_core_id = 0;
#إذا CONFIG_FREERTOS_UNICORE
    receiver_core_id = 0;
#وإلا
    receiver_core_id = 1;
#پایان_اگر
    TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_مقصد_Core(sender_task, "sender", 4096, &test_آرگومانها, UNITY_FREERTOS_اولویت + 2, عدم, sender_core_id));
    TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_مقصد_Core(receiver_task, "receiver", 4096, &test_آرگومانها, UNITY_FREERTOS_اولویت + 1, عدم, receiver_core_id));

    // Wait for both tasks to complete
    لكل (صحيح i = 0; i < 2; i++) {
        xSemaphoreTake(test_آرگومانها.انجامشده_sem, پورت_MAX_DELAY);
    }

    v_جریان_میانگیر_Delete(test_آرگومانها.جریان_میانگیر);
    vSemaphoreDelete(test_آرگومانها.انجامشده_sem);
}
