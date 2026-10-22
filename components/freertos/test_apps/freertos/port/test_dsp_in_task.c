/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव <math.h>
#अन्तर्भाव "soc/soc_caps.h"
#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"
#अन्तर्भाव "freertos/semphr.h"
#अन्तर्भाव "unity.h"
#अन्तर्भाव "test_utils.h"

#यदि SOC_CPU_HAS_DSP

#परिभाषय TEST_अङ्कः_TASKS       4

प्रकारनिर्देश संरचना {
    int32_t  id;
    uint32_t परिणाम;
    TaskHandle_t मुख्यम्;
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
ध्रुव uint32_t g_dsp_constant = 100000;

स्थिर शून्य pinned_task(शून्य *arg)
{
    dsp_params_t *param = (dsp_params_t*) arg;
    ulTaskNotifyTake(pdTRUE, पत्तनम्_MAX_DELAY);

    param->परिणाम = dsp_id_mul(param->id);

    /* Indicate done and wait to be deleted */
    xSemaphoreGive((SemaphoreHandle_t)param->sem);
    vTaskSuspend(रिक्त);
}

TEST_CASE("DSP: Usage in task", "[freertos]")
{
    TaskHandle_t task_handles[CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES][TEST_अङ्कः_TASKS];
    dsp_params_t params[CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES][TEST_अङ्कः_TASKS];

    SemaphoreHandle_t समाप्तम्_sem = xSemaphoreCreateCounting(TEST_अङ्कः_TASKS, 0);
    TEST_ASSERT_NOT_EQUAL(रिक्त, समाप्तम्_sem);

    // Create test tasks for each core
    प्रति (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES; i++) {
        प्रति (पूर्णाङ्क j = 0; j < TEST_अङ्कः_TASKS; j++) {
            params[i][j] = (dsp_params_t) {
                .id = i * TEST_अङ्कः_TASKS + j + 1,
                .sem = समाप्तम्_sem,
            };
            TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_गन्तव्यम्_Core(pinned_task, "task", 4096, (शून्य *) &params[i][j], UNITY_FREERTOS_प्राथमिकता + 1, &task_handles[i][j], i));
        }
    }

    // Start the created tasks
    प्रति (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES; i++) {
        प्रति (पूर्णाङ्क j = 0; j < TEST_अङ्कः_TASKS; j++) {
            xTaskNotifyGive(task_handles[i][j]);
        }
    }

    // Wait for the tasks to complete
    प्रति (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES * TEST_अङ्कः_TASKS; i++) {
        xSemaphoreTake(समाप्तम्_sem, पत्तनम्_MAX_DELAY);
    }

    // Delete the tasks
    प्रति (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES; i++) {
        प्रति (पूर्णाङ्क j = 0; j < TEST_अङ्कः_TASKS; j++) {
            vTaskDelete(task_handles[i][j]);
        }
    }

    // Check the values
    प्रति (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES; i++) {
        प्रति (पूर्णाङ्क j = 0; j < TEST_अङ्कः_TASKS; j++) {
            dsp_params_t* param = &params[i][j];
            TEST_ASSERT_EQUAL(param->id * g_dsp_constant, param->परिणाम);
        }
    }

    vSemaphoreDelete(समाप्तम्_sem);
}

#यदि_अन्तः /* SOC_CPU_HAS_DSP */
