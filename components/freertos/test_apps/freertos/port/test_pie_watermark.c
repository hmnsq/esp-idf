/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#অন্তর্ভাব "sdkconfig.h"
#অন্তর্ভাব <string.h>
#অন্তর্ভাব "soc/soc_caps.h"
#অন্তর্ভাব "freertos/FreeRTOS.h"
#অন্তর্ভাব "freertos/task.h"
#অন্তর্ভাব "unity.h"

#define TASKS_STATUS_ARRAY_LEN  16

/**
 * On RISC-V targets that have coprocessors, the contexts are saved at the lowest address of the stack,
 * which can lead to wrong stack watermark calculation in FreeRTOS in theory.
 * As such, the port layer of FreeRTOS will adjust the lowest address of the stack when a coprocessor
 * context is saved.
 */
#যদি SOC_CPU_HAS_PIE

/**
 * @brief Performs the signed sum of two 4-word vectors using the PIE.
 *
 * @param a First vector
 * @param b Second vector
 * @param dst Destination to store the sum
 */
শূন্য pie_vector_unsigned_add(ধ্রুবক uint32_t a[4], ধ্রুবক uint32_t b[4], uint32_t dst[4]);

স্থির শূন্য other_task(শূন্য* arg)
{
    uint32_t a[4] = { 1, 2, 3, 4};
    uint32_t b[4] = { 42, 43, 44, 45};
    uint32_t dst[4] = { 0 };
    ধ্রুবক TaskHandle_t main_task = (TaskHandle_t) arg;

    /* This task must also use the PIE coprocessor to force a PIE context flush on the main task */
    pie_vector_unsigned_add(a, b, dst);

    xTaskNotifyGive(main_task);
    vTaskDelete(ফাঁকা);
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
    ধ্রুবক পূর্ণসংখ্যা core_id = xPortGetCoreID();
    ধ্রুবক TaskHandle_t current_handle = xTaskGetCurrentTaskHandle();

    /* Get the current stack watermark */
    ধ্রুবক UBaseType_t before_watermark = ux_Task_Get_স্তূপ_উপরের_Water_Mark(current_handle);

    /* Use the PIE unit, the context will NOT be flushed until another task starts using it */
    pie_vector_unsigned_add(a, b, dst);

    xTaskCreatePinnedToCore(other_task,
                            "OtherTask",
                            2048,
                            (শূন্য*) current_handle,
                            CONFIG_UNITY_FREERTOS_PRIORITY - 1,
                            &pvCreatedTask,
                            core_id);

    vTaskDelay(10);

    /* Wait for other task to complete */
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    ধ্রুবক UBaseType_t after_watermark = ux_Task_Get_স্তূপ_উপরের_Water_Mark(current_handle);

    /* The current task has seen a PIE registers context save, so we have at least 8 16-byte registers saved on the
     * stack, which represents 128 bytes. In practice, it may be very different, for example a call to printf would
     * result is more than 1KB of additional stack space used. So let's just make sure that the watermark is bigger
     * than 50% of the former watermark. */
    TEST_ASSERT_TRUE(after_watermark > before_watermark / 2);
}

#endif // SOC_CPU_HAS_PIE
