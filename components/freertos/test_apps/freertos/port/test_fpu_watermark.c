/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#समावेश "sdkconfig.h"
#समावेश <string.h>
#समावेश "soc/soc_caps.h"
#समावेश "freertos/FreeRTOS.h"
#समावेश "freertos/task.h"
#समावेश "unity.h"

#घोषणा TASKS_STATUS_ARRAY_LEN  16

/**
 * On RISC-V targets that have coprocessors, the contexts are saved at the lowest address of the stack,
 * which can lead to wrong stack watermark calculation in FreeRTOS in theory.
 * As such, the port layer of FreeRTOS will adjust the lowest address of the stack when a coprocessor
 * context is saved.
 */
#अगर CONFIG_IDF_TARGET_ARCH_RISCV && SOC_CPU_COPROC_अंक > 0

अस्थिर भग्न s_float = 0.1999f;

अटल भग्न add_floats(भग्न f1, भग्न f2)
{
    वापस f1 + f2;
}

अटल व्योम other_task(व्योम* arg)
{
    स्थिर TaskHandle_t main_task = (TaskHandle_t) arg;

    /* The second task shall also use the FPU to force a context flush on the main task */
    add_floats(s_float, s_float);

    xTaskNotifyGive(main_task);

    /* The FPU context should be flushed on the main task, notify it and return */
    vTaskDelete(शून्य);
}

TEST_CASE("FPU: Context save does not affect stack watermark", "[freertos]")
{
    TaskHandle_t pvCreatedTask;
    /* Force the FreeRTOS port layer to store an FPU context in the current task.
     * So let's use the FPU and make sure another task, on the SAME CORE, also uses it */
    स्थिर पूर्णांक core_id = xPortGetCoreID();
    स्थिर TaskHandle_t current_handle = xTaskGetCurrentTaskHandle();

    /* Get the current stack watermark */
    स्थिर UBaseType_t before_watermark = ux_Task_Get_ढेर_ऊपरी_Water_Mark(current_handle);

    /* Use the FPU unit, the context will NOT be flushed until another task starts using it */
    add_floats(s_float, s_float);

    xTaskCreatePinnedToCore(other_task,
                            "OtherTask",
                            2048,
                            (व्योम*) current_handle,
                            CONFIG_UNITY_FREERTOS_PRIORITY - 1,
                            &pvCreatedTask,
                            core_id);

    vTaskDelay(10);

    /* Wait for other task to complete */
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    स्थिर UBaseType_t after_watermark = ux_Task_Get_ढेर_ऊपरी_Water_Mark(current_handle);

    /* The current task has seen a general-purpose registers context save as well as an FPU context save
     * so we have at least 32*2 registers saved on the stack, which represents 256 bytes.
     * In practice, it may be very different, for example a call to printf would result is more than 1KB
     * of additional stack space used. So let's just make sure that the watermark is bigger than 50% of the
     * former watermark. */
    TEST_ASSERT_TRUE(after_watermark > before_watermark / 2);
}

#पूर्ण_अगर // CONFIG_IDF_TARGET_ARCH_RISCV && SOC_CPU_COPROC_NUM > 0
