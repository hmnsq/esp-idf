/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#অন্তর্ভাৱ "sdkconfig.h"
#অন্তর্ভাৱ <math.h>
#অন্তর্ভাৱ "soc/soc_caps.h"
#অন্তর্ভাৱ "freertos/FreeRTOS.h"
#অন্তর্ভাৱ "freertos/task.h"
#অন্তর্ভাৱ "freertos/semphr.h"
#অন্তর্ভাৱ "unity.h"
#অন্তর্ভাৱ "test_utils.h"

#যদি SOC_CPU_HAS_DSP

#define TEST_অঙ্ক_TASKS       4

প্রকারনির্দেশ সংরচনা {
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
ধ্রুৱ uint32_t g_dsp_constant = 100000;

স্থির শূন্য pinned_task(শূন্য *arg)
{
    dsp_params_t *param = (dsp_params_t*) arg;
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    param->result = dsp_id_mul(param->id);

    /* Indicate done and wait to be deleted */
    xSemaphoreGive((SemaphoreHandle_t)param->sem);
    vTaskSuspend(রিক্ত);
}

TEST_CASE("DSP: Usage in task", "[freertos]")
{
    TaskHandle_t task_handles[CONFIG_FREERTOS_সংখ্যা_OF_CORES][TEST_অঙ্ক_TASKS];
    dsp_params_t params[CONFIG_FREERTOS_সংখ্যা_OF_CORES][TEST_অঙ্ক_TASKS];

    SemaphoreHandle_t done_sem = xSemaphoreCreateCounting(TEST_অঙ্ক_TASKS, 0);
    TEST_ASSERT_NOT_EQUAL(রিক্ত, done_sem);

    // Create test tasks for each core
    বাবে (পূর্ণাঙ্ক i = 0; i < CONFIG_FREERTOS_সংখ্যা_OF_CORES; i++) {
        বাবে (পূর্ণাঙ্ক j = 0; j < TEST_অঙ্ক_TASKS; j++) {
            params[i][j] = (dsp_params_t) {
                .id = i * TEST_অঙ্ক_TASKS + j + 1,
                .sem = done_sem,
            };
            TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(pinned_task, "task", 4096, (শূন্য *) &params[i][j], UNITY_FREERTOS_PRIORITY + 1, &task_handles[i][j], i));
        }
    }

    // Start the created tasks
    বাবে (পূর্ণাঙ্ক i = 0; i < CONFIG_FREERTOS_সংখ্যা_OF_CORES; i++) {
        বাবে (পূর্ণাঙ্ক j = 0; j < TEST_অঙ্ক_TASKS; j++) {
            xTaskNotifyGive(task_handles[i][j]);
        }
    }

    // Wait for the tasks to complete
    বাবে (পূর্ণাঙ্ক i = 0; i < CONFIG_FREERTOS_সংখ্যা_OF_CORES * TEST_অঙ্ক_TASKS; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);
    }

    // Delete the tasks
    বাবে (পূর্ণাঙ্ক i = 0; i < CONFIG_FREERTOS_সংখ্যা_OF_CORES; i++) {
        বাবে (পূর্ণাঙ্ক j = 0; j < TEST_অঙ্ক_TASKS; j++) {
            vTaskDelete(task_handles[i][j]);
        }
    }

    // Check the values
    বাবে (পূর্ণাঙ্ক i = 0; i < CONFIG_FREERTOS_সংখ্যা_OF_CORES; i++) {
        বাবে (পূর্ণাঙ্ক j = 0; j < TEST_অঙ্ক_TASKS; j++) {
            dsp_params_t* param = &params[i][j];
            TEST_ASSERT_EQUAL(param->id * g_dsp_constant, param->result);
        }
    }

    vSemaphoreDelete(done_sem);
}

#endif /* SOC_CPU_HAS_DSP */
