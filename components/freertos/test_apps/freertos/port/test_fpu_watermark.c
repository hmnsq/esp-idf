/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "soc/soc_caps.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TASKS_ਸਥਿਤੀ_ARRAY_LEN  16

/**
 * On RISC-V targets that have coprocessors, the contexts are saved at the lowest address of the stack,
 * which can lead to wrong stack watermark calculation in FreeRTOS in theory.
 * As such, the port layer of FreeRTOS will adjust the lowest address of the stack when a coprocessor
 * context is saved.
 */
#ਜੇ CONFIG_IDF_ਨਿਸ਼ਾਨਾ_ARCH_RISCV && SOC_CPU_COPROC_ਅੰਕ > 0

ਅਸਥਿਰ ਦਸ਼ਮਲਵ s_float = 0.1999f;

ਸਥਿਰ ਦਸ਼ਮਲਵ ਜੋੜੋ_floats(ਦਸ਼ਮਲਵ f1, ਦਸ਼ਮਲਵ f2)
{
    ਵਾਪਸ f1 + f2;
}

ਸਥਿਰ ਖਾਲੀ other_task(ਖਾਲੀ* arg)
{
    ਅਚਲ TaskHandle_t main_task = (TaskHandle_t) arg;

    /* The second task shall also use the FPU to force a context flush on the main task */
    ਜੋੜੋ_floats(s_float, s_float);

    xTaskNotifyGive(main_task);

    /* The FPU context should be flushed on the main task, notify it and return */
    vTaskDelete(ਨਿਰਾ);
}

TEST_CASE("FPU: Context save does not affect stack watermark", "[freertos]")
{
    TaskHandle_t pvCreatedTask;
    /* Force the FreeRTOS port layer to store an FPU context in the current task.
     * So let's use the FPU and make sure another task, on the SAME CORE, also uses it */
    ਅਚਲ ਪੂਰਨ core_id = x_ਪੋਰਟ_Get_Core_ID();
    ਅਚਲ TaskHandle_t current_handle = xTaskGetCurrentTaskHandle();

    /* Get the current stack watermark */
    ਅਚਲ UBaseType_t before_watermark = ux_Task_Get_ਢੇਰ_ਉੱਚਾ_Water_Mark(current_handle);

    /* Use the FPU unit, the context will NOT be flushed until another task starts using it */
    ਜੋੜੋ_floats(s_float, s_float);

    x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(other_task,
                            "OtherTask",
                            2048,
                            (ਖਾਲੀ*) current_handle,
                            CONFIG_UNITY_FREERTOS_ਤਰਜੀਹ - 1,
                            &pvCreatedTask,
                            core_id);

    vTaskDelay(10);

    /* Wait for other task to complete */
    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);

    ਅਚਲ UBaseType_t after_watermark = ux_Task_Get_ਢੇਰ_ਉੱਚਾ_Water_Mark(current_handle);

    /* The current task has seen a general-purpose registers context save as well as an FPU context save
     * so we have at least 32*2 registers saved on the stack, which represents 256 bytes.
     * In practice, it may be very different, for example a call to printf would result is more than 1KB
     * of additional stack space used. So let's just make sure that the watermark is bigger than 50% of the
     * former watermark. */
    TEST_ASSERT_TRUE(after_watermark > before_watermark / 2);
}

#ਜੇ_ਅੰਤ // CONFIG_IDF_TARGET_ARCH_RISCV && SOC_CPU_COPROC_NUM > 0
