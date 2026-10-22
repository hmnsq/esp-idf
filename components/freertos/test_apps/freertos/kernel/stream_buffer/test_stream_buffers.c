/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#अन्तर्भाव <stdint.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"
#अन्तर्भाव "freertos/semphr.h"
#अन्तर्भाव "freertos/stream_buffer.h"
#अन्तर्भाव "unity.h"
#अन्तर्भाव "test_utils.h"

#परिभाषय TEST_अङ्कः_अष्टकानि                  100
#परिभाषय TEST_RECEIVER_TIMEOUT_TICKS     pd_MS_गन्तव्यम्_TICKS(1000)    // 1ms timeout for receiver task

प्रकारनिर्देश संरचना {
    प्रवाहः_बफर_Handle_t प्रवाहः_बफर;
    SemaphoreHandle_t समाप्तम्_sem;
} test_तर्काः_t;

स्थिर शून्य sender_task(शून्य *arg)
{
    test_तर्काः_t *test_तर्काः = (test_तर्काः_t *)arg;
    प्रारूप_लिख("Starting sender task... \n");

    प्रति (पूर्णाङ्क i = 0; i < TEST_अङ्कः_अष्टकानि; i++) {
        // Send a single byte, with the byte's value being the number of bytes sent thus far
        uint8_t दत्त = (uint8_t)i;
        TEST_ASSERT_EQUAL(1, x_प्रवाहः_बफर_Send(test_तर्काः->प्रवाहः_बफर, &दत्त, 1, 0));
        // Short delay to give a chance for receiver task to receive
        vTaskDelay(1);
    }

    xSemaphoreGive(test_तर्काः->समाप्तम्_sem);
    vTaskDelete(रिक्त);
}

स्थिर शून्य receiver_task(शून्य *arg)
{
    test_तर्काः_t *test_तर्काः = (test_तर्काः_t *)arg;
    प्रारूप_लिख("Starting receiver task... \n");

    प्रति (पूर्णाङ्क i = 0; i < TEST_अङ्कः_अष्टकानि; i++) {
        // Receive  a single byte. The received byte's value being the number of bytes sent/received thus far
        uint8_t दत्त;
        TEST_ASSERT_EQUAL(1, x_प्रवाहः_बफर_Receive(test_तर्काः->प्रवाहः_बफर, &दत्त, 1, TEST_RECEIVER_TIMEOUT_TICKS));
        TEST_ASSERT_EQUAL(i, दत्त);
    }

    xSemaphoreGive(test_तर्काः->समाप्तम्_sem);
    vTaskDelete(रिक्त);
}

TEST_CASE("Stream Buffer: Send-receive tasks", "[freertos]")
{
    test_तर्काः_t test_तर्काः;
    test_तर्काः.प्रवाहः_बफर = x_प्रवाहः_बफर_Create(TEST_अङ्कः_अष्टकानि, 1);
    test_तर्काः.समाप्तम्_sem = xSemaphoreCreateCounting(2, 0);
    TEST_ASSERT_NOT_NULL(test_तर्काः.प्रवाहः_बफर);
    TEST_ASSERT_NOT_NULL(test_तर्काः.समाप्तम्_sem);
    आधारः_Type_t sender_core_id;
    आधारः_Type_t receiver_core_id;
    sender_core_id = 0;
#यदि CONFIG_FREERTOS_UNICORE
    receiver_core_id = 0;
#अन्यथा
    receiver_core_id = 1;
#यदि_अन्तः
    TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_गन्तव्यम्_Core(sender_task, "sender", 4096, &test_तर्काः, UNITY_FREERTOS_प्राथमिकता + 2, रिक्त, sender_core_id));
    TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_गन्तव्यम्_Core(receiver_task, "receiver", 4096, &test_तर्काः, UNITY_FREERTOS_प्राथमिकता + 1, रिक्त, receiver_core_id));

    // Wait for both tasks to complete
    प्रति (पूर्णाङ्क i = 0; i < 2; i++) {
        xSemaphoreTake(test_तर्काः.समाप्तम्_sem, पत्तनम्_MAX_DELAY);
    }

    v_प्रवाहः_बफर_Delete(test_तर्काः.प्रवाहः_बफर);
    vSemaphoreDelete(test_तर्काः.समाप्तम्_sem);
}
