/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ <stdint.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "freertos/semphr.h"
#అన్తర్భావ "freertos/stream_buffer.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "test_utils.h"

#నిర్వచించు TEST_అంకె_బైట్లు                  100
#నిర్వచించు TEST_RECEIVER_TIMEOUT_TICKS     pd_MS_గమ్యం_TICKS(1000)    // 1ms timeout for receiver task

రకనిర్వచనం నిర్మాణం {
    ప్రవాహం_బఫరు_Handle_t ప్రవాహం_బఫరు;
    SemaphoreHandle_t పూర్తి_sem;
} test_వాదనలు_t;

స్థిర శూన్య sender_task(శూన్య *arg)
{
    test_వాదనలు_t *test_వాదనలు = (test_వాదనలు_t *)arg;
    రాయి("Starting sender task... \n");

    కొరకు (పూర్ణాంకం i = 0; i < TEST_అంకె_బైట్లు; i++) {
        // Send a single byte, with the byte's value being the number of bytes sent thus far
        uint8_t దత్తాంశం = (uint8_t)i;
        TEST_ASSERT_EQUAL(1, x_ప్రవాహం_బఫరు_Send(test_వాదనలు->ప్రవాహం_బఫరు, &దత్తాంశం, 1, 0));
        // Short delay to give a chance for receiver task to receive
        vTaskDelay(1);
    }

    xSemaphoreGive(test_వాదనలు->పూర్తి_sem);
    vTaskDelete(ఖాళీ);
}

స్థిర శూన్య receiver_task(శూన్య *arg)
{
    test_వాదనలు_t *test_వాదనలు = (test_వాదనలు_t *)arg;
    రాయి("Starting receiver task... \n");

    కొరకు (పూర్ణాంకం i = 0; i < TEST_అంకె_బైట్లు; i++) {
        // Receive  a single byte. The received byte's value being the number of bytes sent/received thus far
        uint8_t దత్తాంశం;
        TEST_ASSERT_EQUAL(1, x_ప్రవాహం_బఫరు_Receive(test_వాదనలు->ప్రవాహం_బఫరు, &దత్తాంశం, 1, TEST_RECEIVER_TIMEOUT_TICKS));
        TEST_ASSERT_EQUAL(i, దత్తాంశం);
    }

    xSemaphoreGive(test_వాదనలు->పూర్తి_sem);
    vTaskDelete(ఖాళీ);
}

TEST_CASE("Stream Buffer: Send-receive tasks", "[freertos]")
{
    test_వాదనలు_t test_వాదనలు;
    test_వాదనలు.ప్రవాహం_బఫరు = x_ప్రవాహం_బఫరు_Create(TEST_అంకె_బైట్లు, 1);
    test_వాదనలు.పూర్తి_sem = xSemaphoreCreateCounting(2, 0);
    TEST_ASSERT_NOT_NULL(test_వాదనలు.ప్రవాహం_బఫరు);
    TEST_ASSERT_NOT_NULL(test_వాదనలు.పూర్తి_sem);
    పునాది_Type_t sender_core_id;
    పునాది_Type_t receiver_core_id;
    sender_core_id = 0;
#ఒకవేళ CONFIG_FREERTOS_UNICORE
    receiver_core_id = 0;
#లేకపోతే
    receiver_core_id = 1;
#అయితే_ముగింపు
    TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_గమ్యం_Core(sender_task, "sender", 4096, &test_వాదనలు, UNITY_FREERTOS_ప్రాధాన్యత + 2, ఖాళీ, sender_core_id));
    TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_గమ్యం_Core(receiver_task, "receiver", 4096, &test_వాదనలు, UNITY_FREERTOS_ప్రాధాన్యత + 1, ఖాళీ, receiver_core_id));

    // Wait for both tasks to complete
    కొరకు (పూర్ణాంకం i = 0; i < 2; i++) {
        xSemaphoreTake(test_వాదనలు.పూర్తి_sem, పోర్టు_MAX_DELAY);
    }

    v_ప్రవాహం_బఫరు_Delete(test_వాదనలు.ప్రవాహం_బఫరు);
    vSemaphoreDelete(test_వాదనలు.పూర్తి_sem);
}
