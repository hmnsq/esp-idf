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
#ఒకవేళ CONFIG_IDF_లక్ష్యం_ARCH_RISCV && SOC_CPU_COPROC_అంకె > 0

అస్థిర దశాంశం s_float = 0.1999f;

స్థిర దశాంశం కూడు_floats(దశాంశం f1, దశాంశం f2)
{
    తిరిగి f1 + f2;
}

స్థిర శూన్య other_task(శూన్య* arg)
{
    స్థిరరాశి TaskHandle_t main_task = (TaskHandle_t) arg;

    /* The second task shall also use the FPU to force a context flush on the main task */
    కూడు_floats(s_float, s_float);

    xTaskNotifyGive(main_task);

    /* The FPU context should be flushed on the main task, notify it and return */
    vTaskDelete(ఖాళీ);
}

TEST_CASE("FPU: Context save does not affect stack watermark", "[freertos]")
{
    TaskHandle_t pvCreatedTask;
    /* Force the FreeRTOS port layer to store an FPU context in the current task.
     * So let's use the FPU and make sure another task, on the SAME CORE, also uses it */
    స్థిరరాశి పూర్ణాంకం core_id = x_పోర్టు_Get_Core_ID();
    స్థిరరాశి TaskHandle_t current_handle = xTaskGetCurrentTaskHandle();

    /* Get the current stack watermark */
    స్థిరరాశి UBaseType_t before_watermark = ux_Task_Get_దొంతర_ఎగువ_Water_Mark(current_handle);

    /* Use the FPU unit, the context will NOT be flushed until another task starts using it */
    కూడు_floats(s_float, s_float);

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

    /* The current task has seen a general-purpose registers context save as well as an FPU context save
     * so we have at least 32*2 registers saved on the stack, which represents 256 bytes.
     * In practice, it may be very different, for example a call to printf would result is more than 1KB
     * of additional stack space used. So let's just make sure that the watermark is bigger than 50% of the
     * former watermark. */
    TEST_ASSERT_TRUE(after_watermark > before_watermark / 2);
}

#అయితే_ముగింపు // CONFIG_IDF_TARGET_ARCH_RISCV && SOC_CPU_COPROC_NUM > 0
