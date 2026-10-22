/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ <string.h>
#అన్తర్భావ "soc/soc_caps.h"
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "unity.h"

#నిర్వచించు TASKS_స్థితి_ARRAY_LEN  16

/**
 * On RISC-V targets that have coprocessors, the contexts are saved at the lowest address of the stack,
 * which can lead to wrong stack watermark calculation in FreeRTOS in theory.
 * As such, the port layer of FreeRTOS will adjust the lowest address of the stack when a coprocessor
 * context is saved.
 */
#ఒకవేళ SOC_CPU_HAS_PIE

/**
 * @brief Performs the signed sum of two 4-word vectors using the PIE.
 *
 * @param a First vector
 * @param b Second vector
 * @param dst Destination to store the sum
 */
శూన్య pie_సదిశ_unsigned_కూడు(స్థిరరాశి uint32_t a[4], స్థిరరాశి uint32_t b[4], uint32_t dst[4]);

స్థిర శూన్య other_task(శూన్య* arg)
{
    uint32_t a[4] = { 1, 2, 3, 4};
    uint32_t b[4] = { 42, 43, 44, 45};
    uint32_t dst[4] = { 0 };
    స్థిరరాశి TaskHandle_t main_task = (TaskHandle_t) arg;

    /* This task must also use the PIE coprocessor to force a PIE context flush on the main task */
    pie_సదిశ_unsigned_కూడు(a, b, dst);

    xTaskNotifyGive(main_task);
    vTaskDelete(ఖాళీ);
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
    స్థిరరాశి పూర్ణాంకం core_id = x_పోర్టు_Get_Core_ID();
    స్థిరరాశి TaskHandle_t current_handle = xTaskGetCurrentTaskHandle();

    /* Get the current stack watermark */
    స్థిరరాశి UBaseType_t before_watermark = ux_Task_Get_దొంతర_ఎగువ_Water_Mark(current_handle);

    /* Use the PIE unit, the context will NOT be flushed until another task starts using it */
    pie_సదిశ_unsigned_కూడు(a, b, dst);

    x_Task_Create_Pinned_గమ్యం_Core(other_task,
                            "OtherTask",
                            2048,
                            (శూన్య*) current_handle,
                            CONFIG_UNITY_FREERTOS_ప్రాధాన్యత - 1,
                            &pvCreatedTask,
                            core_id);

    vTaskDelay(10);

    /* Wait for other task to complete */
    ulTaskNotifyTake(pdTRUE, పోర్టు_MAX_DELAY);

    స్థిరరాశి UBaseType_t after_watermark = ux_Task_Get_దొంతర_ఎగువ_Water_Mark(current_handle);

    /* The current task has seen a PIE registers context save, so we have at least 8 16-byte registers saved on the
     * stack, which represents 128 bytes. In practice, it may be very different, for example a call to printf would
     * result is more than 1KB of additional stack space used. So let's just make sure that the watermark is bigger
     * than 50% of the former watermark. */
    TEST_ASSERT_TRUE(after_watermark > before_watermark / 2);
}

#అయితే_ముగింపు // SOC_CPU_HAS_PIE
