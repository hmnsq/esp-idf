/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <math.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "soc/soc_caps.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/semphr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "test_utils.h"

#ਜੇ SOC_CPU_HAS_DSP

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_ਅੰਕ_TASKS       4

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    int32_t  id;
    uint32_t ਨਤੀਜਾ;
    TaskHandle_t ਮੁੱਖ;
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
ਅਚਲ uint32_t g_dsp_constant = 100000;

ਸਥਿਰ ਖਾਲੀ pinned_task(ਖਾਲੀ *arg)
{
    dsp_params_t *param = (dsp_params_t*) arg;
    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);

    param->ਨਤੀਜਾ = dsp_id_mul(param->id);

    /* Indicate done and wait to be deleted */
    xSemaphoreGive((SemaphoreHandle_t)param->sem);
    vTaskSuspend(ਨਿਰਾ);
}

TEST_CASE("DSP: Usage in task", "[freertos]")
{
    TaskHandle_t task_handles[CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES][TEST_ਅੰਕ_TASKS];
    dsp_params_t params[CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES][TEST_ਅੰਕ_TASKS];

    SemaphoreHandle_t ਪੂਰਾ_sem = xSemaphoreCreateCounting(TEST_ਅੰਕ_TASKS, 0);
    TEST_ASSERT_NOT_EQUAL(ਨਿਰਾ, ਪੂਰਾ_sem);

    // Create test tasks for each core
    ਲਈ (ਪੂਰਨ i = 0; i < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; i++) {
        ਲਈ (ਪੂਰਨ j = 0; j < TEST_ਅੰਕ_TASKS; j++) {
            params[i][j] = (dsp_params_t) {
                .id = i * TEST_ਅੰਕ_TASKS + j + 1,
                .sem = ਪੂਰਾ_sem,
            };
            TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(pinned_task, "task", 4096, (ਖਾਲੀ *) &params[i][j], UNITY_FREERTOS_ਤਰਜੀਹ + 1, &task_handles[i][j], i));
        }
    }

    // Start the created tasks
    ਲਈ (ਪੂਰਨ i = 0; i < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; i++) {
        ਲਈ (ਪੂਰਨ j = 0; j < TEST_ਅੰਕ_TASKS; j++) {
            xTaskNotifyGive(task_handles[i][j]);
        }
    }

    // Wait for the tasks to complete
    ਲਈ (ਪੂਰਨ i = 0; i < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES * TEST_ਅੰਕ_TASKS; i++) {
        xSemaphoreTake(ਪੂਰਾ_sem, ਪੋਰਟ_MAX_DELAY);
    }

    // Delete the tasks
    ਲਈ (ਪੂਰਨ i = 0; i < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; i++) {
        ਲਈ (ਪੂਰਨ j = 0; j < TEST_ਅੰਕ_TASKS; j++) {
            vTaskDelete(task_handles[i][j]);
        }
    }

    // Check the values
    ਲਈ (ਪੂਰਨ i = 0; i < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; i++) {
        ਲਈ (ਪੂਰਨ j = 0; j < TEST_ਅੰਕ_TASKS; j++) {
            dsp_params_t* param = &params[i][j];
            TEST_ASSERT_EQUAL(param->id * g_dsp_constant, param->ਨਤੀਜਾ);
        }
    }

    vSemaphoreDelete(ਪੂਰਾ_sem);
}

#ਜੇ_ਅੰਤ /* SOC_CPU_HAS_DSP */
