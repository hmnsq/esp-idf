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

#define TEST_अङ्क_TASKS       4

प्रकारनिर्देश संरचना {
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
अचर uint32_t g_dsp_constant = 100000;

स्थिर शून्य pinned_task(शून्य *arg)
{
    dsp_params_t *param = (dsp_params_t*) arg;
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    param->result = dsp_id_mul(param->id);

    /* Indicate done and wait to be deleted */
    xSemaphoreGive((SemaphoreHandle_t)param->sem);
    vTaskSuspend(रिक्त);
}

TEST_CASE("DSP: Usage in task", "[freertos]")
{
    TaskHandle_t task_handles[CONFIG_FREERTOS_सङ्ख्या_OF_CORES][TEST_अङ्क_TASKS];
    dsp_params_t params[CONFIG_FREERTOS_सङ्ख्या_OF_CORES][TEST_अङ्क_TASKS];

    SemaphoreHandle_t done_sem = xSemaphoreCreateCounting(TEST_अङ्क_TASKS, 0);
    TEST_ASSERT_NOT_EQUAL(रिक्त, done_sem);

    // Create test tasks for each core
    लागि (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_सङ्ख्या_OF_CORES; i++) {
        लागि (पूर्णाङ्क j = 0; j < TEST_अङ्क_TASKS; j++) {
            params[i][j] = (dsp_params_t) {
                .id = i * TEST_अङ्क_TASKS + j + 1,
                .sem = done_sem,
            };
            TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(pinned_task, "task", 4096, (शून्य *) &params[i][j], UNITY_FREERTOS_PRIORITY + 1, &task_handles[i][j], i));
        }
    }

    // Start the created tasks
    लागि (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_सङ्ख्या_OF_CORES; i++) {
        लागि (पूर्णाङ्क j = 0; j < TEST_अङ्क_TASKS; j++) {
            xTaskNotifyGive(task_handles[i][j]);
        }
    }

    // Wait for the tasks to complete
    लागि (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_सङ्ख्या_OF_CORES * TEST_अङ्क_TASKS; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);
    }

    // Delete the tasks
    लागि (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_सङ्ख्या_OF_CORES; i++) {
        लागि (पूर्णाङ्क j = 0; j < TEST_अङ्क_TASKS; j++) {
            vTaskDelete(task_handles[i][j]);
        }
    }

    // Check the values
    लागि (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_सङ्ख्या_OF_CORES; i++) {
        लागि (पूर्णाङ्क j = 0; j < TEST_अङ्क_TASKS; j++) {
            dsp_params_t* param = &params[i][j];
            TEST_ASSERT_EQUAL(param->id * g_dsp_constant, param->result);
        }
    }

    vSemaphoreDelete(done_sem);
}

#endif /* SOC_CPU_HAS_DSP */
