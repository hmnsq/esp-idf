/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#അന്തര്ഭാവ "sdkconfig.h"
#അന്തര്ഭാവ <math.h>
#അന്തര്ഭാവ "soc/soc_caps.h"
#അന്തര്ഭാവ "freertos/FreeRTOS.h"
#അന്തര്ഭാവ "freertos/task.h"
#അന്തര്ഭാവ "freertos/semphr.h"
#അന്തര്ഭാവ "unity.h"
#അന്തര്ഭാവ "test_utils.h"

#എങ്കിൽ SOC_CPU_HAS_DSP

#define TEST_അക്കം_TASKS       4

തരനിർവചനം ഘടന {
    int32_t  id;
    uint32_t result;
    TaskHandle_t പ്രധാന;
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
സ്ഥിരം uint32_t g_dsp_constant = 100000;

സ്ഥിതം ശൂന്യം pinned_task(ശൂന്യം *arg)
{
    dsp_params_t *param = (dsp_params_t*) arg;
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    param->result = dsp_id_mul(param->id);

    /* Indicate done and wait to be deleted */
    xSemaphoreGive((SemaphoreHandle_t)param->sem);
    vTaskSuspend(ശൂന്യ);
}

TEST_CASE("DSP: Usage in task", "[freertos]")
{
    TaskHandle_t task_handles[CONFIG_FREERTOS_സംഖ്യ_OF_CORES][TEST_അക്കം_TASKS];
    dsp_params_t params[CONFIG_FREERTOS_സംഖ്യ_OF_CORES][TEST_അക്കം_TASKS];

    SemaphoreHandle_t done_sem = xSemaphoreCreateCounting(TEST_അക്കം_TASKS, 0);
    TEST_ASSERT_NOT_EQUAL(ശൂന്യ, done_sem);

    // Create test tasks for each core
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; i++) {
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < TEST_അക്കം_TASKS; j++) {
            params[i][j] = (dsp_params_t) {
                .id = i * TEST_അക്കം_TASKS + j + 1,
                .sem = done_sem,
            };
            TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(pinned_task, "task", 4096, (ശൂന്യം *) &params[i][j], UNITY_FREERTOS_PRIORITY + 1, &task_handles[i][j], i));
        }
    }

    // Start the created tasks
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; i++) {
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < TEST_അക്കം_TASKS; j++) {
            xTaskNotifyGive(task_handles[i][j]);
        }
    }

    // Wait for the tasks to complete
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < CONFIG_FREERTOS_സംഖ്യ_OF_CORES * TEST_അക്കം_TASKS; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);
    }

    // Delete the tasks
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; i++) {
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < TEST_അക്കം_TASKS; j++) {
            vTaskDelete(task_handles[i][j]);
        }
    }

    // Check the values
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; i++) {
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < TEST_അക്കം_TASKS; j++) {
            dsp_params_t* param = &params[i][j];
            TEST_ASSERT_EQUAL(param->id * g_dsp_constant, param->result);
        }
    }

    vSemaphoreDelete(done_sem);
}

#endif /* SOC_CPU_HAS_DSP */
