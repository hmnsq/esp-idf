/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#অন্তর্ভাব <stdint.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব "freertos/FreeRTOS.h"
#অন্তর্ভাব "freertos/task.h"
#অন্তর্ভাব "freertos/semphr.h"
#অন্তর্ভাব "freertos/stream_buffer.h"
#অন্তর্ভাব "unity.h"
#অন্তর্ভাব "test_utils.h"

#define TEST_অঙ্ক_BYTES                  100
#define TEST_RECEIVER_TIMEOUT_TICKS     pdMS_TO_TICKS(1000)    // 1ms timeout for receiver task

প্রকারলেখ কাঠামো {
    Stream_বাফার_Handle_t stream_বাফার;
    SemaphoreHandle_t done_sem;
} test_args_t;

স্থির শূন্য sender_task(শূন্য *arg)
{
    test_args_t *test_args = (test_args_t *)arg;
    printf("Starting sender task... \n");

    জন্য (পূর্ণসংখ্যা i = 0; i < TEST_অঙ্ক_BYTES; i++) {
        // Send a single byte, with the byte's value being the number of bytes sent thus far
        uint8_t তথ্য = (uint8_t)i;
        TEST_ASSERT_EQUAL(1, x_Stream_বাফার_Send(test_args->stream_বাফার, &তথ্য, 1, 0));
        // Short delay to give a chance for receiver task to receive
        vTaskDelay(1);
    }

    xSemaphoreGive(test_args->done_sem);
    vTaskDelete(ফাঁকা);
}

স্থির শূন্য receiver_task(শূন্য *arg)
{
    test_args_t *test_args = (test_args_t *)arg;
    printf("Starting receiver task... \n");

    জন্য (পূর্ণসংখ্যা i = 0; i < TEST_অঙ্ক_BYTES; i++) {
        // Receive  a single byte. The received byte's value being the number of bytes sent/received thus far
        uint8_t তথ্য;
        TEST_ASSERT_EQUAL(1, x_Stream_বাফার_Receive(test_args->stream_বাফার, &তথ্য, 1, TEST_RECEIVER_TIMEOUT_TICKS));
        TEST_ASSERT_EQUAL(i, তথ্য);
    }

    xSemaphoreGive(test_args->done_sem);
    vTaskDelete(ফাঁকা);
}

TEST_CASE("Stream Buffer: Send-receive tasks", "[freertos]")
{
    test_args_t test_args;
    test_args.stream_বাফার = x_Stream_বাফার_Create(TEST_অঙ্ক_BYTES, 1);
    test_args.done_sem = xSemaphoreCreateCounting(2, 0);
    TEST_ASSERT_NOT_NULL(test_args.stream_বাফার);
    TEST_ASSERT_NOT_NULL(test_args.done_sem);
    BaseType_t sender_core_id;
    BaseType_t receiver_core_id;
    sender_core_id = 0;
#যদি CONFIG_FREERTOS_UNICORE
    receiver_core_id = 0;
#অন্যথা
    receiver_core_id = 1;
#endif
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(sender_task, "sender", 4096, &test_args, UNITY_FREERTOS_PRIORITY + 2, ফাঁকা, sender_core_id));
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(receiver_task, "receiver", 4096, &test_args, UNITY_FREERTOS_PRIORITY + 1, ফাঁকা, receiver_core_id));

    // Wait for both tasks to complete
    জন্য (পূর্ণসংখ্যা i = 0; i < 2; i++) {
        xSemaphoreTake(test_args.done_sem, portMAX_DELAY);
    }

    v_Stream_বাফার_Delete(test_args.stream_বাফার);
    vSemaphoreDelete(test_args.done_sem);
}
