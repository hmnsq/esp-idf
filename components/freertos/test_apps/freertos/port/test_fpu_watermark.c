/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#அந்தர்பாவ "sdkconfig.h"
#அந்தர்பாவ <string.h>
#அந்தர்பாவ "soc/soc_caps.h"
#அந்தர்பாவ "freertos/FreeRTOS.h"
#அந்தர்பாவ "freertos/task.h"
#அந்தர்பாவ "unity.h"

#define TASKS_STATUS_ARRAY_LEN  16

/**
 * On RISC-V targets that have coprocessors, the contexts are saved at the lowest address of the stack,
 * which can lead to wrong stack watermark calculation in FreeRTOS in theory.
 * As such, the port layer of FreeRTOS will adjust the lowest address of the stack when a coprocessor
 * context is saved.
 */
#எனில் CONFIG_IDF_TARGET_ARCH_RISCV && SOC_CPU_COPROC_இலக்கம் > 0

நிலையற்ற தசம s_float = 0.1999f;

நிலையான தசம add_floats(தசம f1, தசம f2)
{
    திரும்பு f1 + f2;
}

நிலையான வெறுமை other_task(வெறுமை* arg)
{
    மாறிலி TaskHandle_t main_task = (TaskHandle_t) arg;

    /* The second task shall also use the FPU to force a context flush on the main task */
    add_floats(s_float, s_float);

    xTaskNotifyGive(main_task);

    /* The FPU context should be flushed on the main task, notify it and return */
    vTaskDelete(வெற்று);
}

TEST_CASE("FPU: Context save does not affect stack watermark", "[freertos]")
{
    TaskHandle_t pvCreatedTask;
    /* Force the FreeRTOS port layer to store an FPU context in the current task.
     * So let's use the FPU and make sure another task, on the SAME CORE, also uses it */
    மாறிலி முழு core_id = xPortGetCoreID();
    மாறிலி TaskHandle_t current_handle = xTaskGetCurrentTaskHandle();

    /* Get the current stack watermark */
    மாறிலி UBaseType_t before_watermark = ux_Task_Get_அடுக்கு_உயர்வு_Water_Mark(current_handle);

    /* Use the FPU unit, the context will NOT be flushed until another task starts using it */
    add_floats(s_float, s_float);

    xTaskCreatePinnedToCore(other_task,
                            "OtherTask",
                            2048,
                            (வெறுமை*) current_handle,
                            CONFIG_UNITY_FREERTOS_PRIORITY - 1,
                            &pvCreatedTask,
                            core_id);

    vTaskDelay(10);

    /* Wait for other task to complete */
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    மாறிலி UBaseType_t after_watermark = ux_Task_Get_அடுக்கு_உயர்வு_Water_Mark(current_handle);

    /* The current task has seen a general-purpose registers context save as well as an FPU context save
     * so we have at least 32*2 registers saved on the stack, which represents 256 bytes.
     * In practice, it may be very different, for example a call to printf would result is more than 1KB
     * of additional stack space used. So let's just make sure that the watermark is bigger than 50% of the
     * former watermark. */
    TEST_ASSERT_TRUE(after_watermark > before_watermark / 2);
}

#endif // CONFIG_IDF_TARGET_ARCH_RISCV && SOC_CPU_COPROC_NUM > 0
