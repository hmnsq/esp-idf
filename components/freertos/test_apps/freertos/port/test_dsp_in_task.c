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

#تعریف_کړه TEST_عدد_TASKS       4

تعريفنوع بنية {
    int32_t  id;
    uint32_t پایله;
    TaskHandle_t اصلي;
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
    ulTaskNotifyTake(pdTRUE, پورټ_MAX_DELAY);

    param->پایله = dsp_id_mul(param->id);

    /* Indicate done and wait to be deleted */
    xSemaphoreGive((SemaphoreHandle_t)param->sem);
    vTaskSuspend(عدم);
}

TEST_CASE("DSP: Usage in task", "[freertos]")
{
    TaskHandle_t task_handles[CONFIG_FREERTOS_شمېره_د_CORES][TEST_عدد_TASKS];
    dsp_params_t params[CONFIG_FREERTOS_شمېره_د_CORES][TEST_عدد_TASKS];

    SemaphoreHandle_t بشپړ_sem = xSemaphoreCreateCounting(TEST_عدد_TASKS, 0);
    TEST_ASSERT_NOT_EQUAL(عدم, بشپړ_sem);

    // Create test tasks for each core
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_شمېره_د_CORES; i++) {
        لكل (صحيح j = 0; j < TEST_عدد_TASKS; j++) {
            params[i][j] = (dsp_params_t) {
                .id = i * TEST_عدد_TASKS + j + 1,
                .sem = بشپړ_sem,
            };
            TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_منزل_Core(pinned_task, "task", 4096, (فراغ *) &params[i][j], UNITY_FREERTOS_لومړیتوب + 1, &task_handles[i][j], i));
        }
    }

    // Start the created tasks
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_شمېره_د_CORES; i++) {
        لكل (صحيح j = 0; j < TEST_عدد_TASKS; j++) {
            xTaskNotifyGive(task_handles[i][j]);
        }
    }

    // Wait for the tasks to complete
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_شمېره_د_CORES * TEST_عدد_TASKS; i++) {
        xSemaphoreTake(بشپړ_sem, پورټ_MAX_DELAY);
    }

    // Delete the tasks
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_شمېره_د_CORES; i++) {
        لكل (صحيح j = 0; j < TEST_عدد_TASKS; j++) {
            vTaskDelete(task_handles[i][j]);
        }
    }

    // Check the values
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_شمېره_د_CORES; i++) {
        لكل (صحيح j = 0; j < TEST_عدد_TASKS; j++) {
            dsp_params_t* param = &params[i][j];
            TEST_ASSERT_EQUAL(param->id * g_dsp_constant, param->پایله);
        }
    }

    vSemaphoreDelete(بشپړ_sem);
}

#د_که_پای /* SOC_CPU_HAS_DSP */
