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

#define TEST_رقم_BYTES                  100
#define TEST_RECEIVER_TIMEOUT_TICKS     pdMS_TO_TICKS(1000)    // 1ms timeout for receiver task

تعريفنوع بنية {
    StreamBufferHandle_t stream_buffer;
    SemaphoreHandle_t done_sem;
} test_وسائط_t;

ساكن فراغ sender_task(فراغ *arg)
{
    test_وسائط_t *test_وسائط = (test_وسائط_t *)arg;
    printf("Starting sender task... \n");

    لكل (صحيح i = 0; i < TEST_رقم_BYTES; i++) {
        // Send a single byte, with the byte's value being the number of bytes sent thus far
        uint8_t data = (uint8_t)i;
        TEST_ASSERT_EQUAL(1, xStreamBufferSend(test_وسائط->stream_buffer, &data, 1, 0));
        // Short delay to give a chance for receiver task to receive
        vTaskDelay(1);
    }

    xSemaphoreGive(test_وسائط->done_sem);
    vTaskDelete(عدم);
}

ساكن فراغ receiver_task(فراغ *arg)
{
    test_وسائط_t *test_وسائط = (test_وسائط_t *)arg;
    printf("Starting receiver task... \n");

    لكل (صحيح i = 0; i < TEST_رقم_BYTES; i++) {
        // Receive  a single byte. The received byte's value being the number of bytes sent/received thus far
        uint8_t data;
        TEST_ASSERT_EQUAL(1, xStreamBufferReceive(test_وسائط->stream_buffer, &data, 1, TEST_RECEIVER_TIMEOUT_TICKS));
        TEST_ASSERT_EQUAL(i, data);
    }

    xSemaphoreGive(test_وسائط->done_sem);
    vTaskDelete(عدم);
}

TEST_CASE("Stream Buffer: Send-receive tasks", "[freertos]")
{
    test_وسائط_t test_وسائط;
    test_وسائط.stream_buffer = xStreamBufferCreate(TEST_رقم_BYTES, 1);
    test_وسائط.done_sem = xSemaphoreCreateCounting(2, 0);
    TEST_ASSERT_NOT_NULL(test_وسائط.stream_buffer);
    TEST_ASSERT_NOT_NULL(test_وسائط.done_sem);
    BaseType_t sender_core_id;
    BaseType_t receiver_core_id;
    sender_core_id = 0;
#إذا CONFIG_FREERTOS_UNICORE
    receiver_core_id = 0;
#وإلا
    receiver_core_id = 1;
#endif
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(sender_task, "sender", 4096, &test_وسائط, UNITY_FREERTOS_PRIORITY + 2, عدم, sender_core_id));
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(receiver_task, "receiver", 4096, &test_وسائط, UNITY_FREERTOS_PRIORITY + 1, عدم, receiver_core_id));

    // Wait for both tasks to complete
    لكل (صحيح i = 0; i < 2; i++) {
        xSemaphoreTake(test_وسائط.done_sem, portMAX_DELAY);
    }

    vStreamBufferDelete(test_وسائط.stream_buffer);
    vSemaphoreDelete(test_وسائط.done_sem);
}
