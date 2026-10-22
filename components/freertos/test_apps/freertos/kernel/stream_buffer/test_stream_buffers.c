/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#समावेश <stdint.h>
#समावेश <stdio.h>
#समावेश "freertos/FreeRTOS.h"
#समावेश "freertos/task.h"
#समावेश "freertos/semphr.h"
#समावेश "freertos/stream_buffer.h"
#समावेश "unity.h"
#समावेश "test_utils.h"

#घोषणा TEST_अंक_BYTES                  100
#घोषणा TEST_RECEIVER_TIMEOUT_TICKS     pdMS_TO_TICKS(1000)    // 1ms timeout for receiver task

प्रकार काष्ठा {
    Stream_बफर_Handle_t stream_बफर;
    SemaphoreHandle_t done_sem;
} test_args_t;

अटल व्योम sender_task(व्योम *arg)
{
    test_args_t *test_args = (test_args_t *)arg;
    म_लिखो("Starting sender task... \n");

    क्रम (पूर्णांक i = 0; i < TEST_अंक_BYTES; i++) {
        // Send a single byte, with the byte's value being the number of bytes sent thus far
        uint8_t तथ्य = (uint8_t)i;
        TEST_ASSERT_EQUAL(1, x_Stream_बफर_Send(test_args->stream_बफर, &तथ्य, 1, 0));
        // Short delay to give a chance for receiver task to receive
        vTaskDelay(1);
    }

    xSemaphoreGive(test_args->done_sem);
    vTaskDelete(शून्य);
}

अटल व्योम receiver_task(व्योम *arg)
{
    test_args_t *test_args = (test_args_t *)arg;
    म_लिखो("Starting receiver task... \n");

    क्रम (पूर्णांक i = 0; i < TEST_अंक_BYTES; i++) {
        // Receive  a single byte. The received byte's value being the number of bytes sent/received thus far
        uint8_t तथ्य;
        TEST_ASSERT_EQUAL(1, x_Stream_बफर_Receive(test_args->stream_बफर, &तथ्य, 1, TEST_RECEIVER_TIMEOUT_TICKS));
        TEST_ASSERT_EQUAL(i, तथ्य);
    }

    xSemaphoreGive(test_args->done_sem);
    vTaskDelete(शून्य);
}

TEST_CASE("Stream Buffer: Send-receive tasks", "[freertos]")
{
    test_args_t test_args;
    test_args.stream_बफर = x_Stream_बफर_Create(TEST_अंक_BYTES, 1);
    test_args.done_sem = xSemaphoreCreateCounting(2, 0);
    TEST_ASSERT_NOT_NULL(test_args.stream_बफर);
    TEST_ASSERT_NOT_NULL(test_args.done_sem);
    BaseType_t sender_core_id;
    BaseType_t receiver_core_id;
    sender_core_id = 0;
#अगर CONFIG_FREERTOS_UNICORE
    receiver_core_id = 0;
#अन्यथा
    receiver_core_id = 1;
#पूर्ण_अगर
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(sender_task, "sender", 4096, &test_args, UNITY_FREERTOS_PRIORITY + 2, शून्य, sender_core_id));
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(receiver_task, "receiver", 4096, &test_args, UNITY_FREERTOS_PRIORITY + 1, शून्य, receiver_core_id));

    // Wait for both tasks to complete
    क्रम (पूर्णांक i = 0; i < 2; i++) {
        xSemaphoreTake(test_args.done_sem, portMAX_DELAY);
    }

    v_Stream_बफर_Delete(test_args.stream_बफर);
    vSemaphoreDelete(test_args.done_sem);
}
