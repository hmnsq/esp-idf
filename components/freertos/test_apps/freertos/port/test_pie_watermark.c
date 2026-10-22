/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "sdkconfig.h"
#ضمن <string.h>
#ضمن "soc/soc_caps.h"
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "unity.h"

#تعریف_کرو TASKS_حالت_ARRAY_LEN  16

/**
 * On RISC-V targets that have coprocessors, the contexts are saved at the lowest address of the stack,
 * which can lead to wrong stack watermark calculation in FreeRTOS in theory.
 * As such, the port layer of FreeRTOS will adjust the lowest address of the stack when a coprocessor
 * context is saved.
 */
#إذا SOC_CPU_HAS_PIE

/**
 * @brief Performs the signed sum of two 4-word vectors using the PIE.
 *
 * @param a First vector
 * @param b Second vector
 * @param dst Destination to store the sum
 */
فراغ pie_سمتیہ_unsigned_جوڑو(ثابت uint32_t a[4], ثابت uint32_t b[4], uint32_t dst[4]);

ساكن فراغ other_task(فراغ* arg)
{
    uint32_t a[4] = { 1, 2, 3, 4};
    uint32_t b[4] = { 42, 43, 44, 45};
    uint32_t dst[4] = { 0 };
    ثابت TaskHandle_t main_task = (TaskHandle_t) arg;

    /* This task must also use the PIE coprocessor to force a PIE context flush on the main task */
    pie_سمتیہ_unsigned_جوڑو(a, b, dst);

    xTaskNotifyGive(main_task);
    vTaskDelete(عدم);
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
    ثابت صحيح core_id = x_پورٹ_Get_Core_ID();
    ثابت TaskHandle_t current_handle = xTaskGetCurrentTaskHandle();

    /* Get the current stack watermark */
    ثابت UBaseType_t before_watermark = ux_Task_Get_ڈھیر_بلند_Water_Mark(current_handle);

    /* Use the PIE unit, the context will NOT be flushed until another task starts using it */
    pie_سمتیہ_unsigned_جوڑو(a, b, dst);

    x_Task_Create_Pinned_منزل_Core(other_task,
                            "OtherTask",
                            2048,
                            (فراغ*) current_handle,
                            CONFIG_UNITY_FREERTOS_ترجیح - 1,
                            &pvCreatedTask,
                            core_id);

    vTaskDelay(10);

    /* Wait for other task to complete */
    ulTaskNotifyTake(pdTRUE, پورٹ_MAX_DELAY);

    ثابت UBaseType_t after_watermark = ux_Task_Get_ڈھیر_بلند_Water_Mark(current_handle);

    /* The current task has seen a PIE registers context save, so we have at least 8 16-byte registers saved on the
     * stack, which represents 128 bytes. In practice, it may be very different, for example a call to printf would
     * result is more than 1KB of additional stack space used. So let's just make sure that the watermark is bigger
     * than 50% of the former watermark. */
    TEST_ASSERT_TRUE(after_watermark > before_watermark / 2);
}

#جے_انت // SOC_CPU_HAS_PIE
