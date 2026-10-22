/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "sdkconfig.h"
#ضمن <math.h>
#ضمن "soc/soc_caps.h"
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "freertos/semphr.h"
#ضمن "unity.h"
#ضمن "test_utils.h"

#إذا SOC_CPU_HAS_DSP

#define TEST_ژمارە_TASKS       4

تعريفنوع بنية {
    int32_t  id;
    uint32_t result;
    TaskHandle_t main;
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

ساكن فراغ pinned_task(فراغ *arg)
{
    dsp_params_t *param = (dsp_params_t*) arg;
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    param->result = dsp_id_mul(param->id);

    /* Indicate done and wait to be deleted */
    xSemaphoreGive((SemaphoreHandle_t)param->sem);
    vTaskSuspend(عدم);
}

TEST_CASE("DSP: Usage in task", "[freertos]")
{
    TaskHandle_t task_handles[CONFIG_FREERTOS_ژمار_OF_CORES][TEST_ژمارە_TASKS];
    dsp_params_t params[CONFIG_FREERTOS_ژمار_OF_CORES][TEST_ژمارە_TASKS];

    SemaphoreHandle_t done_sem = xSemaphoreCreateCounting(TEST_ژمارە_TASKS, 0);
    TEST_ASSERT_NOT_EQUAL(عدم, done_sem);

    // Create test tasks for each core
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_ژمار_OF_CORES; i++) {
        لكل (صحيح j = 0; j < TEST_ژمارە_TASKS; j++) {
            params[i][j] = (dsp_params_t) {
                .id = i * TEST_ژمارە_TASKS + j + 1,
                .sem = done_sem,
            };
            TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(pinned_task, "task", 4096, (فراغ *) &params[i][j], UNITY_FREERTOS_PRIORITY + 1, &task_handles[i][j], i));
        }
    }

    // Start the created tasks
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_ژمار_OF_CORES; i++) {
        لكل (صحيح j = 0; j < TEST_ژمارە_TASKS; j++) {
            xTaskNotifyGive(task_handles[i][j]);
        }
    }

    // Wait for the tasks to complete
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_ژمار_OF_CORES * TEST_ژمارە_TASKS; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);
    }

    // Delete the tasks
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_ژمار_OF_CORES; i++) {
        لكل (صحيح j = 0; j < TEST_ژمارە_TASKS; j++) {
            vTaskDelete(task_handles[i][j]);
        }
    }

    // Check the values
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_ژمار_OF_CORES; i++) {
        لكل (صحيح j = 0; j < TEST_ژمارە_TASKS; j++) {
            dsp_params_t* param = &params[i][j];
            TEST_ASSERT_EQUAL(param->id * g_dsp_constant, param->result);
        }
    }

    vSemaphoreDelete(done_sem);
}

#endif /* SOC_CPU_HAS_DSP */
