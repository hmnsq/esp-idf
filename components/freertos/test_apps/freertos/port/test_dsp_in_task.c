/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ <math.h>
#ಅನ್ತರ್ಭಾವ "soc/soc_caps.h"
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "freertos/semphr.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "test_utils.h"

#ಆದರೆ SOC_CPU_HAS_DSP

#ವ್ಯಾಖ್ಯಾನಿಸು TEST_ಅಂಕೆ_TASKS       4

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    int32_t  id;
    uint32_t ಫಲಿತಾಂಶ;
    TaskHandle_t ಮುಖ್ಯ;
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
ಸ್ಥಿರಾಂಕ uint32_t g_dsp_constant = 100000;

ಸ್ಥಿರ ಶೂನ್ಯ pinned_task(ಶೂನ್ಯ *arg)
{
    dsp_params_t *param = (dsp_params_t*) arg;
    ulTaskNotifyTake(pdTRUE, ಪೋರ್ಟ್_MAX_DELAY);

    param->ಫಲಿತಾಂಶ = dsp_id_mul(param->id);

    /* Indicate done and wait to be deleted */
    xSemaphoreGive((SemaphoreHandle_t)param->sem);
    vTaskSuspend(ಖಾಲಿ);
}

TEST_CASE("DSP: Usage in task", "[freertos]")
{
    TaskHandle_t task_handles[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES][TEST_ಅಂಕೆ_TASKS];
    dsp_params_t params[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES][TEST_ಅಂಕೆ_TASKS];

    SemaphoreHandle_t ಮುಗಿದ_sem = xSemaphoreCreateCounting(TEST_ಅಂಕೆ_TASKS, 0);
    TEST_ASSERT_NOT_EQUAL(ಖಾಲಿ, ಮುಗಿದ_sem);

    // Create test tasks for each core
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; i++) {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < TEST_ಅಂಕೆ_TASKS; j++) {
            params[i][j] = (dsp_params_t) {
                .id = i * TEST_ಅಂಕೆ_TASKS + j + 1,
                .sem = ಮುಗಿದ_sem,
            };
            TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_ಗಮ್ಯ_Core(pinned_task, "task", 4096, (ಶೂನ್ಯ *) &params[i][j], UNITY_FREERTOS_ಆದ್ಯತೆ + 1, &task_handles[i][j], i));
        }
    }

    // Start the created tasks
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; i++) {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < TEST_ಅಂಕೆ_TASKS; j++) {
            xTaskNotifyGive(task_handles[i][j]);
        }
    }

    // Wait for the tasks to complete
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES * TEST_ಅಂಕೆ_TASKS; i++) {
        xSemaphoreTake(ಮುಗಿದ_sem, ಪೋರ್ಟ್_MAX_DELAY);
    }

    // Delete the tasks
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; i++) {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < TEST_ಅಂಕೆ_TASKS; j++) {
            vTaskDelete(task_handles[i][j]);
        }
    }

    // Check the values
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; i++) {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < TEST_ಅಂಕೆ_TASKS; j++) {
            dsp_params_t* param = &params[i][j];
            TEST_ASSERT_EQUAL(param->id * g_dsp_constant, param->ಫಲಿತಾಂಶ);
        }
    }

    vSemaphoreDelete(ಮುಗಿದ_sem);
}

#ಆದರೆ_ಅಂತ್ಯ /* SOC_CPU_HAS_DSP */
