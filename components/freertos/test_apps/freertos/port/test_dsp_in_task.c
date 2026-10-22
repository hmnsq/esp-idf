/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ <math.h>
#అన్తర్భావ "soc/soc_caps.h"
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "freertos/semphr.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "test_utils.h"

#ఒకవేళ SOC_CPU_HAS_DSP

#నిర్వచించు TEST_అంకె_TASKS       4

రకనిర్వచనం నిర్మాణం {
    int32_t  id;
    uint32_t ఫలితం;
    TaskHandle_t ప్రధాన;
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
స్థిరరాశి uint32_t g_dsp_constant = 100000;

స్థిర శూన్య pinned_task(శూన్య *arg)
{
    dsp_params_t *param = (dsp_params_t*) arg;
    ulTaskNotifyTake(pdTRUE, పోర్టు_MAX_DELAY);

    param->ఫలితం = dsp_id_mul(param->id);

    /* Indicate done and wait to be deleted */
    xSemaphoreGive((SemaphoreHandle_t)param->sem);
    vTaskSuspend(ఖాళీ);
}

TEST_CASE("DSP: Usage in task", "[freertos]")
{
    TaskHandle_t task_handles[CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES][TEST_అంకె_TASKS];
    dsp_params_t params[CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES][TEST_అంకె_TASKS];

    SemaphoreHandle_t పూర్తి_sem = xSemaphoreCreateCounting(TEST_అంకె_TASKS, 0);
    TEST_ASSERT_NOT_EQUAL(ఖాళీ, పూర్తి_sem);

    // Create test tasks for each core
    కొరకు (పూర్ణాంకం i = 0; i < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; i++) {
        కొరకు (పూర్ణాంకం j = 0; j < TEST_అంకె_TASKS; j++) {
            params[i][j] = (dsp_params_t) {
                .id = i * TEST_అంకె_TASKS + j + 1,
                .sem = పూర్తి_sem,
            };
            TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_గమ్యం_Core(pinned_task, "task", 4096, (శూన్య *) &params[i][j], UNITY_FREERTOS_ప్రాధాన్యత + 1, &task_handles[i][j], i));
        }
    }

    // Start the created tasks
    కొరకు (పూర్ణాంకం i = 0; i < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; i++) {
        కొరకు (పూర్ణాంకం j = 0; j < TEST_అంకె_TASKS; j++) {
            xTaskNotifyGive(task_handles[i][j]);
        }
    }

    // Wait for the tasks to complete
    కొరకు (పూర్ణాంకం i = 0; i < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES * TEST_అంకె_TASKS; i++) {
        xSemaphoreTake(పూర్తి_sem, పోర్టు_MAX_DELAY);
    }

    // Delete the tasks
    కొరకు (పూర్ణాంకం i = 0; i < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; i++) {
        కొరకు (పూర్ణాంకం j = 0; j < TEST_అంకె_TASKS; j++) {
            vTaskDelete(task_handles[i][j]);
        }
    }

    // Check the values
    కొరకు (పూర్ణాంకం i = 0; i < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; i++) {
        కొరకు (పూర్ణాంకం j = 0; j < TEST_అంకె_TASKS; j++) {
            dsp_params_t* param = &params[i][j];
            TEST_ASSERT_EQUAL(param->id * g_dsp_constant, param->ఫలితం);
        }
    }

    vSemaphoreDelete(పూర్తి_sem);
}

#అయితే_ముగింపు /* SOC_CPU_HAS_DSP */
