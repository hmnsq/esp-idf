/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "freertos/semphr.h"
#ಅನ್ತರ್ಭಾವ "freertos/stream_buffer.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "test_utils.h"

#ವ್ಯಾಖ್ಯಾನಿಸು TEST_ಅಂಕೆ_ಬೈಟುಗಳು                  100
#ವ್ಯಾಖ್ಯಾನಿಸು TEST_RECEIVER_TIMEOUT_TICKS     pd_MS_ಗಮ್ಯ_TICKS(1000)    // 1ms timeout for receiver task

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ಹರಿವು_ಬಫರ್_Handle_t ಹರಿವು_ಬಫರ್;
    SemaphoreHandle_t ಮುಗಿದ_sem;
} test_ವಾದಗಳು_t;

ಸ್ಥಿರ ಶೂನ್ಯ sender_task(ಶೂನ್ಯ *arg)
{
    test_ವಾದಗಳು_t *test_ವಾದಗಳು = (test_ವಾದಗಳು_t *)arg;
    ಬರೆ("Starting sender task... \n");

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < TEST_ಅಂಕೆ_ಬೈಟುಗಳು; i++) {
        // Send a single byte, with the byte's value being the number of bytes sent thus far
        uint8_t ದತ್ತಾಂಶ = (uint8_t)i;
        TEST_ASSERT_EQUAL(1, x_ಹರಿವು_ಬಫರ್_Send(test_ವಾದಗಳು->ಹರಿವು_ಬಫರ್, &ದತ್ತಾಂಶ, 1, 0));
        // Short delay to give a chance for receiver task to receive
        vTaskDelay(1);
    }

    xSemaphoreGive(test_ವಾದಗಳು->ಮುಗಿದ_sem);
    vTaskDelete(ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ receiver_task(ಶೂನ್ಯ *arg)
{
    test_ವಾದಗಳು_t *test_ವಾದಗಳು = (test_ವಾದಗಳು_t *)arg;
    ಬರೆ("Starting receiver task... \n");

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < TEST_ಅಂಕೆ_ಬೈಟುಗಳು; i++) {
        // Receive  a single byte. The received byte's value being the number of bytes sent/received thus far
        uint8_t ದತ್ತಾಂಶ;
        TEST_ASSERT_EQUAL(1, x_ಹರಿವು_ಬಫರ್_Receive(test_ವಾದಗಳು->ಹರಿವು_ಬಫರ್, &ದತ್ತಾಂಶ, 1, TEST_RECEIVER_TIMEOUT_TICKS));
        TEST_ASSERT_EQUAL(i, ದತ್ತಾಂಶ);
    }

    xSemaphoreGive(test_ವಾದಗಳು->ಮುಗಿದ_sem);
    vTaskDelete(ಖಾಲಿ);
}

TEST_CASE("Stream Buffer: Send-receive tasks", "[freertos]")
{
    test_ವಾದಗಳು_t test_ವಾದಗಳು;
    test_ವಾದಗಳು.ಹರಿವು_ಬಫರ್ = x_ಹರಿವು_ಬಫರ್_Create(TEST_ಅಂಕೆ_ಬೈಟುಗಳು, 1);
    test_ವಾದಗಳು.ಮುಗಿದ_sem = xSemaphoreCreateCounting(2, 0);
    TEST_ASSERT_NOT_NULL(test_ವಾದಗಳು.ಹರಿವು_ಬಫರ್);
    TEST_ASSERT_NOT_NULL(test_ವಾದಗಳು.ಮುಗಿದ_sem);
    ತಳ_Type_t sender_core_id;
    ತಳ_Type_t receiver_core_id;
    sender_core_id = 0;
#ಆದರೆ CONFIG_FREERTOS_UNICORE
    receiver_core_id = 0;
#ಇಲ್ಲದಿದ್ದರೆ
    receiver_core_id = 1;
#ಆದರೆ_ಅಂತ್ಯ
    TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_ಗಮ್ಯ_Core(sender_task, "sender", 4096, &test_ವಾದಗಳು, UNITY_FREERTOS_ಆದ್ಯತೆ + 2, ಖಾಲಿ, sender_core_id));
    TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_ಗಮ್ಯ_Core(receiver_task, "receiver", 4096, &test_ವಾದಗಳು, UNITY_FREERTOS_ಆದ್ಯತೆ + 1, ಖಾಲಿ, receiver_core_id));

    // Wait for both tasks to complete
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 2; i++) {
        xSemaphoreTake(test_ವಾದಗಳು.ಮುಗಿದ_sem, ಪೋರ್ಟ್_MAX_DELAY);
    }

    v_ಹರಿವು_ಬಫರ್_Delete(test_ವಾದಗಳು.ಹರಿವು_ಬಫರ್);
    vSemaphoreDelete(test_ವಾದಗಳು.ಮುಗಿದ_sem);
}
