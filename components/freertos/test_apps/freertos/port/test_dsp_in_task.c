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

#define TEST_رقم_TASKS       4

تعریفنوع ساختار {
    int32_t  id;
    uint32_t result;
    TaskHandle_t اصلی;
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

ایستا تهی pinned_task(تهی *arg)
{
    dsp_params_t *param = (dsp_params_t*) arg;
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    param->result = dsp_id_mul(param->id);

    /* Indicate done and wait to be deleted */
    xSemaphoreGive((SemaphoreHandle_t)param->sem);
    vTaskSuspend(پوچ);
}

TEST_CASE("DSP: Usage in task", "[freertos]")
{
    TaskHandle_t task_handles[CONFIG_FREERTOS_عدد_OF_CORES][TEST_رقم_TASKS];
    dsp_params_t params[CONFIG_FREERTOS_عدد_OF_CORES][TEST_رقم_TASKS];

    SemaphoreHandle_t done_sem = xSemaphoreCreateCounting(TEST_رقم_TASKS, 0);
    TEST_ASSERT_NOT_EQUAL(پوچ, done_sem);

    // Create test tasks for each core
    برای (صحیح i = 0; i < CONFIG_FREERTOS_عدد_OF_CORES; i++) {
        برای (صحیح j = 0; j < TEST_رقم_TASKS; j++) {
            params[i][j] = (dsp_params_t) {
                .id = i * TEST_رقم_TASKS + j + 1,
                .sem = done_sem,
            };
            TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(pinned_task, "task", 4096, (تهی *) &params[i][j], UNITY_FREERTOS_PRIORITY + 1, &task_handles[i][j], i));
        }
    }

    // Start the created tasks
    برای (صحیح i = 0; i < CONFIG_FREERTOS_عدد_OF_CORES; i++) {
        برای (صحیح j = 0; j < TEST_رقم_TASKS; j++) {
            xTaskNotifyGive(task_handles[i][j]);
        }
    }

    // Wait for the tasks to complete
    برای (صحیح i = 0; i < CONFIG_FREERTOS_عدد_OF_CORES * TEST_رقم_TASKS; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);
    }

    // Delete the tasks
    برای (صحیح i = 0; i < CONFIG_FREERTOS_عدد_OF_CORES; i++) {
        برای (صحیح j = 0; j < TEST_رقم_TASKS; j++) {
            vTaskDelete(task_handles[i][j]);
        }
    }

    // Check the values
    برای (صحیح i = 0; i < CONFIG_FREERTOS_عدد_OF_CORES; i++) {
        برای (صحیح j = 0; j < TEST_رقم_TASKS; j++) {
            dsp_params_t* param = &params[i][j];
            TEST_ASSERT_EQUAL(param->id * g_dsp_constant, param->result);
        }
    }

    vSemaphoreDelete(done_sem);
}

#endif /* SOC_CPU_HAS_DSP */
