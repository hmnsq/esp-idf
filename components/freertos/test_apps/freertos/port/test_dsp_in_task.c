/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#समावेश "sdkconfig.h"
#समावेश <math.h>
#समावेश "soc/soc_caps.h"
#समावेश "freertos/FreeRTOS.h"
#समावेश "freertos/task.h"
#समावेश "freertos/semphr.h"
#समावेश "unity.h"
#समावेश "test_utils.h"

#अगर SOC_CPU_HAS_DSP

#घोषणा TEST_अंक_TASKS       4

प्रकार काष्ठा {
    int32_t  id;
    uint32_t परिणाम;
    TaskHandle_t मुख्य;
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
स्थिर uint32_t g_dsp_constant = 100000;

अटल व्योम pinned_task(व्योम *arg)
{
    dsp_params_t *param = (dsp_params_t*) arg;
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    param->परिणाम = dsp_id_mul(param->id);

    /* Indicate done and wait to be deleted */
    xSemaphoreGive((SemaphoreHandle_t)param->sem);
    vTaskSuspend(शून्य);
}

TEST_CASE("DSP: Usage in task", "[freertos]")
{
    TaskHandle_t task_handles[CONFIG_FREERTOS_संख्या_OF_CORES][TEST_अंक_TASKS];
    dsp_params_t params[CONFIG_FREERTOS_संख्या_OF_CORES][TEST_अंक_TASKS];

    SemaphoreHandle_t done_sem = xSemaphoreCreateCounting(TEST_अंक_TASKS, 0);
    TEST_ASSERT_NOT_EQUAL(शून्य, done_sem);

    // Create test tasks for each core
    क्रम (पूर्णांक i = 0; i < CONFIG_FREERTOS_संख्या_OF_CORES; i++) {
        क्रम (पूर्णांक j = 0; j < TEST_अंक_TASKS; j++) {
            params[i][j] = (dsp_params_t) {
                .id = i * TEST_अंक_TASKS + j + 1,
                .sem = done_sem,
            };
            TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(pinned_task, "task", 4096, (व्योम *) &params[i][j], UNITY_FREERTOS_PRIORITY + 1, &task_handles[i][j], i));
        }
    }

    // Start the created tasks
    क्रम (पूर्णांक i = 0; i < CONFIG_FREERTOS_संख्या_OF_CORES; i++) {
        क्रम (पूर्णांक j = 0; j < TEST_अंक_TASKS; j++) {
            xTaskNotifyGive(task_handles[i][j]);
        }
    }

    // Wait for the tasks to complete
    क्रम (पूर्णांक i = 0; i < CONFIG_FREERTOS_संख्या_OF_CORES * TEST_अंक_TASKS; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);
    }

    // Delete the tasks
    क्रम (पूर्णांक i = 0; i < CONFIG_FREERTOS_संख्या_OF_CORES; i++) {
        क्रम (पूर्णांक j = 0; j < TEST_अंक_TASKS; j++) {
            vTaskDelete(task_handles[i][j]);
        }
    }

    // Check the values
    क्रम (पूर्णांक i = 0; i < CONFIG_FREERTOS_संख्या_OF_CORES; i++) {
        क्रम (पूर्णांक j = 0; j < TEST_अंक_TASKS; j++) {
            dsp_params_t* param = &params[i][j];
            TEST_ASSERT_EQUAL(param->id * g_dsp_constant, param->परिणाम);
        }
    }

    vSemaphoreDelete(done_sem);
}

#पूर्ण_अगर /* SOC_CPU_HAS_DSP */
