/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव <string.h>
#अन्तर्भाव "soc/soc_caps.h"
#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"
#अन्तर्भाव "unity.h"

#परिभाषय TASKS_स्थितिः_ARRAY_LEN  16

/**
 * On RISC-V targets that have coprocessors, the contexts are saved at the lowest address of the stack,
 * which can lead to wrong stack watermark calculation in FreeRTOS in theory.
 * As such, the port layer of FreeRTOS will adjust the lowest address of the stack when a coprocessor
 * context is saved.
 */
#यदि SOC_CPU_HAS_PIE

/**
 * @brief Performs the signed sum of two 4-word vectors using the PIE.
 *
 * @param a First vector
 * @param b Second vector
 * @param dst Destination to store the sum
 */
शून्य pie_सदिशम्_unsigned_सङ्कलय(ध्रुव uint32_t a[4], ध्रुव uint32_t b[4], uint32_t dst[4]);

स्थिर शून्य other_task(शून्य* arg)
{
    uint32_t a[4] = { 1, 2, 3, 4};
    uint32_t b[4] = { 42, 43, 44, 45};
    uint32_t dst[4] = { 0 };
    ध्रुव TaskHandle_t main_task = (TaskHandle_t) arg;

    /* This task must also use the PIE coprocessor to force a PIE context flush on the main task */
    pie_सदिशम्_unsigned_सङ्कलय(a, b, dst);

    xTaskNotifyGive(main_task);
    vTaskDelete(रिक्त);
}

TEST_CASE("PIE: Context save does not affect stack watermark", "[freertos]")
{
    /* Setup some random values */
    uint32_t a[4] = { 0x3f00ffff, 0xffe10045, 0xffe10096, 0x42434546};
    uint32_t b[4] = { 0x42, 0xbb43, 0x6644, 0x845};
    uint32_t dst[4] = { 0 };

    TaskHandle_t pvCreatedTask;
    /* Force the FreeRTOS port layer to store a PIE context in the current task.
     * So let's use the PIE and make sure another task, on the SAME CORE, also uses it */
    ध्रुव पूर्णाङ्क core_id = x_पत्तनम्_Get_Core_ID();
    ध्रुव TaskHandle_t current_handle = xTaskGetCurrentTaskHandle();

    /* Get the current stack watermark */
    ध्रुव UBaseType_t before_watermark = ux_Task_Get_राशिः_उच्चः_Water_Mark(current_handle);

    /* Use the PIE unit, the context will NOT be flushed until another task starts using it */
    pie_सदिशम्_unsigned_सङ्कलय(a, b, dst);

    x_Task_Create_Pinned_गन्तव्यम्_Core(other_task,
                            "OtherTask",
                            2048,
                            (शून्य*) current_handle,
                            CONFIG_UNITY_FREERTOS_प्राथमिकता - 1,
                            &pvCreatedTask,
                            core_id);

    vTaskDelay(10);

    /* Wait for other task to complete */
    ulTaskNotifyTake(pdTRUE, पत्तनम्_MAX_DELAY);

    ध्रुव UBaseType_t after_watermark = ux_Task_Get_राशिः_उच्चः_Water_Mark(current_handle);

    /* The current task has seen a PIE registers context save, so we have at least 8 16-byte registers saved on the
     * stack, which represents 128 bytes. In practice, it may be very different, for example a call to printf would
     * result is more than 1KB of additional stack space used. So let's just make sure that the watermark is bigger
     * than 50% of the former watermark. */
    TEST_ASSERT_TRUE(after_watermark > before_watermark / 2);
}

#यदि_अन्तः // SOC_CPU_HAS_PIE
