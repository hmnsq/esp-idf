/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#شامل "sdkconfig.h"
#شامل <math.h>
#شامل "soc/soc_caps.h"
#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل "freertos/semphr.h"
#شامل "unity.h"
#شامل "test_utils.h"

#اگر SOC_CPU_HAS_DSP

#تعریف_کرو TEST_عدد_TASKS       4

قسمتعریف ساخت {
    int32_t  id;
    uint32_t نتیجہ;
    TaskHandle_t مین;
    SemaphoreHandle_t sem;
} dsp_params_t;

/**
 * @brief Multiplies the given ID by a constant.
 *
 * @param id Value to multiply
 */
uint32_t dsp_id_mul(uint32_t id);

/**
 * @brief DSP Assembly routine need to access this constant, make it public.
 * It will be used as a multiplier.
 */
ثابت uint32_t g_dsp_constant = 100000;

ساکن خالی pinned_task(خالی *arg)
{
    dsp_params_t *param = (dsp_params_t*) arg;
    ulTaskNotifyTake(pdTRUE, پورٹ_MAX_DELAY);

    param->نتیجہ = dsp_id_mul(param->id);

    /* Indicate done and wait to be deleted */
    xSemaphoreGive((SemaphoreHandle_t)param->sem);
    vTaskSuspend(تہی);
}

TEST_CASE("DSP: Usage in task", "[freertos]")
{
    TaskHandle_t task_handles[CONFIG_FREERTOS_نمبر_کا_CORES][TEST_عدد_TASKS];
    dsp_params_t params[CONFIG_FREERTOS_نمبر_کا_CORES][TEST_عدد_TASKS];

    SemaphoreHandle_t مکمل_sem = xSemaphoreCreateCounting(TEST_عدد_TASKS, 0);
    TEST_ASSERT_NOT_EQUAL(تہی, مکمل_sem);

    // Create test tasks for each core
    برائے (صحیح i = 0; i < CONFIG_FREERTOS_نمبر_کا_CORES; i++) {
        برائے (صحیح j = 0; j < TEST_عدد_TASKS; j++) {
            params[i][j] = (dsp_params_t) {
                .id = i * TEST_عدد_TASKS + j + 1,
                .sem = مکمل_sem,
            };
            TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_مقصد_Core(pinned_task, "task", 4096, (خالی *) &params[i][j], UNITY_FREERTOS_ترجیح + 1, &task_handles[i][j], i));
        }
    }

    // Start the created tasks
    برائے (صحیح i = 0; i < CONFIG_FREERTOS_نمبر_کا_CORES; i++) {
        برائے (صحیح j = 0; j < TEST_عدد_TASKS; j++) {
            xTaskNotifyGive(task_handles[i][j]);
        }
    }

    // Wait for the tasks to complete
    برائے (صحیح i = 0; i < CONFIG_FREERTOS_نمبر_کا_CORES * TEST_عدد_TASKS; i++) {
        xSemaphoreTake(مکمل_sem, پورٹ_MAX_DELAY);
    }

    // Delete the tasks
    برائے (صحیح i = 0; i < CONFIG_FREERTOS_نمبر_کا_CORES; i++) {
        برائے (صحیح j = 0; j < TEST_عدد_TASKS; j++) {
            vTaskDelete(task_handles[i][j]);
        }
    }

    // Check the values
    برائے (صحیح i = 0; i < CONFIG_FREERTOS_نمبر_کا_CORES; i++) {
        برائے (صحیح j = 0; j < TEST_عدد_TASKS; j++) {
            dsp_params_t* param = &params[i][j];
            TEST_ASSERT_EQUAL(param->id * g_dsp_constant, param->نتیجہ);
        }
    }

    vSemaphoreDelete(مکمل_sem);
}

#اگر_ختم /* SOC_CPU_HAS_DSP */
