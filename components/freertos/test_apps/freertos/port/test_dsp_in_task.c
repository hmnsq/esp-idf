/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#અન્તર્ભાવ "sdkconfig.h"
#અન્તર્ભાવ <math.h>
#અન્તર્ભાવ "soc/soc_caps.h"
#અન્તર્ભાવ "freertos/FreeRTOS.h"
#અન્તર્ભાવ "freertos/task.h"
#અન્તર્ભાવ "freertos/semphr.h"
#અન્તર્ભાવ "unity.h"
#અન્તર્ભાવ "test_utils.h"

#જો SOC_CPU_HAS_DSP

#define TEST_અંક_TASKS       4

પ્રકારવ્યાખ્યા સંરચના {
    int32_t  id;
    uint32_t result;
    TaskHandle_t મુખ્ય;
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
અચલ uint32_t g_dsp_constant = 100000;

સ્થિર શૂન્ય pinned_task(શૂન્ય *arg)
{
    dsp_params_t *param = (dsp_params_t*) arg;
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    param->result = dsp_id_mul(param->id);

    /* Indicate done and wait to be deleted */
    xSemaphoreGive((SemaphoreHandle_t)param->sem);
    vTaskSuspend(ખાલી);
}

TEST_CASE("DSP: Usage in task", "[freertos]")
{
    TaskHandle_t task_handles[CONFIG_FREERTOS_સંખ્યા_OF_CORES][TEST_અંક_TASKS];
    dsp_params_t params[CONFIG_FREERTOS_સંખ્યા_OF_CORES][TEST_અંક_TASKS];

    SemaphoreHandle_t done_sem = xSemaphoreCreateCounting(TEST_અંક_TASKS, 0);
    TEST_ASSERT_NOT_EQUAL(ખાલી, done_sem);

    // Create test tasks for each core
    માટે (પૂર્ણાંક i = 0; i < CONFIG_FREERTOS_સંખ્યા_OF_CORES; i++) {
        માટે (પૂર્ણાંક j = 0; j < TEST_અંક_TASKS; j++) {
            params[i][j] = (dsp_params_t) {
                .id = i * TEST_અંક_TASKS + j + 1,
                .sem = done_sem,
            };
            TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(pinned_task, "task", 4096, (શૂન્ય *) &params[i][j], UNITY_FREERTOS_PRIORITY + 1, &task_handles[i][j], i));
        }
    }

    // Start the created tasks
    માટે (પૂર્ણાંક i = 0; i < CONFIG_FREERTOS_સંખ્યા_OF_CORES; i++) {
        માટે (પૂર્ણાંક j = 0; j < TEST_અંક_TASKS; j++) {
            xTaskNotifyGive(task_handles[i][j]);
        }
    }

    // Wait for the tasks to complete
    માટે (પૂર્ણાંક i = 0; i < CONFIG_FREERTOS_સંખ્યા_OF_CORES * TEST_અંક_TASKS; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);
    }

    // Delete the tasks
    માટે (પૂર્ણાંક i = 0; i < CONFIG_FREERTOS_સંખ્યા_OF_CORES; i++) {
        માટે (પૂર્ણાંક j = 0; j < TEST_અંક_TASKS; j++) {
            vTaskDelete(task_handles[i][j]);
        }
    }

    // Check the values
    માટે (પૂર્ણાંક i = 0; i < CONFIG_FREERTOS_સંખ્યા_OF_CORES; i++) {
        માટે (પૂર્ણાંક j = 0; j < TEST_અંક_TASKS; j++) {
            dsp_params_t* param = &params[i][j];
            TEST_ASSERT_EQUAL(param->id * g_dsp_constant, param->result);
        }
    }

    vSemaphoreDelete(done_sem);
}

#endif /* SOC_CPU_HAS_DSP */
