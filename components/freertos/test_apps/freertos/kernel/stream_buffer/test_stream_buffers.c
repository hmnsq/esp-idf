/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ଅନ୍ତର୍ଭାଵ <stdint.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ "freertos/FreeRTOS.h"
#ଅନ୍ତର୍ଭାଵ "freertos/task.h"
#ଅନ୍ତର୍ଭାଵ "freertos/semphr.h"
#ଅନ୍ତର୍ଭାଵ "freertos/stream_buffer.h"
#ଅନ୍ତର୍ଭାଵ "unity.h"
#ଅନ୍ତର୍ଭାଵ "test_utils.h"

#define TEST_ଅଙ୍କ_BYTES                  100
#define TEST_RECEIVER_TIMEOUT_TICKS     pdMS_TO_TICKS(1000)    // 1ms timeout for receiver task

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    StreamBufferHandle_t stream_buffer;
    SemaphoreHandle_t done_sem;
} test_args_t;

ସ୍ଥିର ଶୂନ୍ୟ sender_task(ଶୂନ୍ୟ *arg)
{
    test_args_t *test_args = (test_args_t *)arg;
    printf("Starting sender task... \n");

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < TEST_ଅଙ୍କ_BYTES; i++) {
        // Send a single byte, with the byte's value being the number of bytes sent thus far
        uint8_t data = (uint8_t)i;
        TEST_ASSERT_EQUAL(1, xStreamBufferSend(test_args->stream_buffer, &data, 1, 0));
        // Short delay to give a chance for receiver task to receive
        vTaskDelay(1);
    }

    xSemaphoreGive(test_args->done_sem);
    vTaskDelete(ଖାଲି);
}

ସ୍ଥିର ଶୂନ୍ୟ receiver_task(ଶୂନ୍ୟ *arg)
{
    test_args_t *test_args = (test_args_t *)arg;
    printf("Starting receiver task... \n");

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < TEST_ଅଙ୍କ_BYTES; i++) {
        // Receive  a single byte. The received byte's value being the number of bytes sent/received thus far
        uint8_t data;
        TEST_ASSERT_EQUAL(1, xStreamBufferReceive(test_args->stream_buffer, &data, 1, TEST_RECEIVER_TIMEOUT_TICKS));
        TEST_ASSERT_EQUAL(i, data);
    }

    xSemaphoreGive(test_args->done_sem);
    vTaskDelete(ଖାଲି);
}

TEST_CASE("Stream Buffer: Send-receive tasks", "[freertos]")
{
    test_args_t test_args;
    test_args.stream_buffer = xStreamBufferCreate(TEST_ଅଙ୍କ_BYTES, 1);
    test_args.done_sem = xSemaphoreCreateCounting(2, 0);
    TEST_ASSERT_NOT_NULL(test_args.stream_buffer);
    TEST_ASSERT_NOT_NULL(test_args.done_sem);
    BaseType_t sender_core_id;
    BaseType_t receiver_core_id;
    sender_core_id = 0;
#ଯଦି CONFIG_FREERTOS_UNICORE
    receiver_core_id = 0;
#ଅନ୍ୟଥା
    receiver_core_id = 1;
#endif
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(sender_task, "sender", 4096, &test_args, UNITY_FREERTOS_PRIORITY + 2, ଖାଲି, sender_core_id));
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(receiver_task, "receiver", 4096, &test_args, UNITY_FREERTOS_PRIORITY + 1, ଖାଲି, receiver_core_id));

    // Wait for both tasks to complete
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < 2; i++) {
        xSemaphoreTake(test_args.done_sem, portMAX_DELAY);
    }

    vStreamBufferDelete(test_args.stream_buffer);
    vSemaphoreDelete(test_args.done_sem);
}
