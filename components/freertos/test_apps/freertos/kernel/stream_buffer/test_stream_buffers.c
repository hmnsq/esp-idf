/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#അന്തര്ഭാവ <stdint.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ "freertos/FreeRTOS.h"
#അന്തര്ഭാവ "freertos/task.h"
#അന്തര്ഭാവ "freertos/semphr.h"
#അന്തര്ഭാവ "freertos/stream_buffer.h"
#അന്തര്ഭാവ "unity.h"
#അന്തര്ഭാവ "test_utils.h"

#define TEST_അക്കം_BYTES                  100
#define TEST_RECEIVER_TIMEOUT_TICKS     pdMS_TO_TICKS(1000)    // 1ms timeout for receiver task

തരനിർവചനം ഘടന {
    StreamBufferHandle_t stream_buffer;
    SemaphoreHandle_t done_sem;
} test_args_t;

സ്ഥിതം ശൂന്യം sender_task(ശൂന്യം *arg)
{
    test_args_t *test_args = (test_args_t *)arg;
    printf("Starting sender task... \n");

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < TEST_അക്കം_BYTES; i++) {
        // Send a single byte, with the byte's value being the number of bytes sent thus far
        uint8_t data = (uint8_t)i;
        TEST_ASSERT_EQUAL(1, xStreamBufferSend(test_args->stream_buffer, &data, 1, 0));
        // Short delay to give a chance for receiver task to receive
        vTaskDelay(1);
    }

    xSemaphoreGive(test_args->done_sem);
    vTaskDelete(ശൂന്യ);
}

സ്ഥിതം ശൂന്യം receiver_task(ശൂന്യം *arg)
{
    test_args_t *test_args = (test_args_t *)arg;
    printf("Starting receiver task... \n");

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < TEST_അക്കം_BYTES; i++) {
        // Receive  a single byte. The received byte's value being the number of bytes sent/received thus far
        uint8_t data;
        TEST_ASSERT_EQUAL(1, xStreamBufferReceive(test_args->stream_buffer, &data, 1, TEST_RECEIVER_TIMEOUT_TICKS));
        TEST_ASSERT_EQUAL(i, data);
    }

    xSemaphoreGive(test_args->done_sem);
    vTaskDelete(ശൂന്യ);
}

TEST_CASE("Stream Buffer: Send-receive tasks", "[freertos]")
{
    test_args_t test_args;
    test_args.stream_buffer = xStreamBufferCreate(TEST_അക്കം_BYTES, 1);
    test_args.done_sem = xSemaphoreCreateCounting(2, 0);
    TEST_ASSERT_NOT_NULL(test_args.stream_buffer);
    TEST_ASSERT_NOT_NULL(test_args.done_sem);
    BaseType_t sender_core_id;
    BaseType_t receiver_core_id;
    sender_core_id = 0;
#എങ്കിൽ CONFIG_FREERTOS_UNICORE
    receiver_core_id = 0;
#അല്ലെങ്കിൽ
    receiver_core_id = 1;
#endif
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(sender_task, "sender", 4096, &test_args, UNITY_FREERTOS_PRIORITY + 2, ശൂന്യ, sender_core_id));
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(receiver_task, "receiver", 4096, &test_args, UNITY_FREERTOS_PRIORITY + 1, ശൂന്യ, receiver_core_id));

    // Wait for both tasks to complete
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 2; i++) {
        xSemaphoreTake(test_args.done_sem, portMAX_DELAY);
    }

    vStreamBufferDelete(test_args.stream_buffer);
    vSemaphoreDelete(test_args.done_sem);
}
