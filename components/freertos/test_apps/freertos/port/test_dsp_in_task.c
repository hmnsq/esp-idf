/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#அந்தர்பாவ "sdkconfig.h"
#அந்தர்பாவ <math.h>
#அந்தர்பாவ "soc/soc_caps.h"
#அந்தர்பாவ "freertos/FreeRTOS.h"
#அந்தர்பாவ "freertos/task.h"
#அந்தர்பாவ "freertos/semphr.h"
#அந்தர்பாவ "unity.h"
#அந்தர்பாவ "test_utils.h"

#எனில் SOC_CPU_HAS_DSP

#define TEST_இலக்கம்_TASKS       4

வகைவரை கட்டமைப்பு {
    int32_t  id;
    uint32_t result;
    TaskHandle_t முதன்மை;
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
மாறிலி uint32_t g_dsp_constant = 100000;

நிலையான வெறுமை pinned_task(வெறுமை *arg)
{
    dsp_params_t *param = (dsp_params_t*) arg;
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    param->result = dsp_id_mul(param->id);

    /* Indicate done and wait to be deleted */
    xSemaphoreGive((SemaphoreHandle_t)param->sem);
    vTaskSuspend(வெற்று);
}

TEST_CASE("DSP: Usage in task", "[freertos]")
{
    TaskHandle_t task_handles[CONFIG_FREERTOS_எண்_OF_CORES][TEST_இலக்கம்_TASKS];
    dsp_params_t params[CONFIG_FREERTOS_எண்_OF_CORES][TEST_இலக்கம்_TASKS];

    SemaphoreHandle_t done_sem = xSemaphoreCreateCounting(TEST_இலக்கம்_TASKS, 0);
    TEST_ASSERT_NOT_EQUAL(வெற்று, done_sem);

    // Create test tasks for each core
    க்கு (முழு i = 0; i < CONFIG_FREERTOS_எண்_OF_CORES; i++) {
        க்கு (முழு j = 0; j < TEST_இலக்கம்_TASKS; j++) {
            params[i][j] = (dsp_params_t) {
                .id = i * TEST_இலக்கம்_TASKS + j + 1,
                .sem = done_sem,
            };
            TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(pinned_task, "task", 4096, (வெறுமை *) &params[i][j], UNITY_FREERTOS_PRIORITY + 1, &task_handles[i][j], i));
        }
    }

    // Start the created tasks
    க்கு (முழு i = 0; i < CONFIG_FREERTOS_எண்_OF_CORES; i++) {
        க்கு (முழு j = 0; j < TEST_இலக்கம்_TASKS; j++) {
            xTaskNotifyGive(task_handles[i][j]);
        }
    }

    // Wait for the tasks to complete
    க்கு (முழு i = 0; i < CONFIG_FREERTOS_எண்_OF_CORES * TEST_இலக்கம்_TASKS; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);
    }

    // Delete the tasks
    க்கு (முழு i = 0; i < CONFIG_FREERTOS_எண்_OF_CORES; i++) {
        க்கு (முழு j = 0; j < TEST_இலக்கம்_TASKS; j++) {
            vTaskDelete(task_handles[i][j]);
        }
    }

    // Check the values
    க்கு (முழு i = 0; i < CONFIG_FREERTOS_எண்_OF_CORES; i++) {
        க்கு (முழு j = 0; j < TEST_இலக்கம்_TASKS; j++) {
            dsp_params_t* param = &params[i][j];
            TEST_ASSERT_EQUAL(param->id * g_dsp_constant, param->result);
        }
    }

    vSemaphoreDelete(done_sem);
}

#endif /* SOC_CPU_HAS_DSP */
