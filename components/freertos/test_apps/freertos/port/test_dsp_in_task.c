/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#অন্তর্ভাব "sdkconfig.h"
#অন্তর্ভাব <math.h>
#অন্তর্ভাব "soc/soc_caps.h"
#অন্তর্ভাব "freertos/FreeRTOS.h"
#অন্তর্ভাব "freertos/task.h"
#অন্তর্ভাব "freertos/semphr.h"
#অন্তর্ভাব "unity.h"
#অন্তর্ভাব "test_utils.h"

#যদি SOC_CPU_HAS_DSP

#define TEST_অঙ্ক_TASKS       4

প্রকারলেখ কাঠামো {
    int32_t  id;
    uint32_t ফলাফল;
    TaskHandle_t প্রধান;
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
ধ্রুবক uint32_t g_dsp_constant = 100000;

স্থির শূন্য pinned_task(শূন্য *arg)
{
    dsp_params_t *param = (dsp_params_t*) arg;
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    param->ফলাফল = dsp_id_mul(param->id);

    /* Indicate done and wait to be deleted */
    xSemaphoreGive((SemaphoreHandle_t)param->sem);
    vTaskSuspend(ফাঁকা);
}

TEST_CASE("DSP: Usage in task", "[freertos]")
{
    TaskHandle_t task_handles[CONFIG_FREERTOS_সংখ্যা_OF_CORES][TEST_অঙ্ক_TASKS];
    dsp_params_t params[CONFIG_FREERTOS_সংখ্যা_OF_CORES][TEST_অঙ্ক_TASKS];

    SemaphoreHandle_t done_sem = xSemaphoreCreateCounting(TEST_অঙ্ক_TASKS, 0);
    TEST_ASSERT_NOT_EQUAL(ফাঁকা, done_sem);

    // Create test tasks for each core
    জন্য (পূর্ণসংখ্যা i = 0; i < CONFIG_FREERTOS_সংখ্যা_OF_CORES; i++) {
        জন্য (পূর্ণসংখ্যা j = 0; j < TEST_অঙ্ক_TASKS; j++) {
            params[i][j] = (dsp_params_t) {
                .id = i * TEST_অঙ্ক_TASKS + j + 1,
                .sem = done_sem,
            };
            TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(pinned_task, "task", 4096, (শূন্য *) &params[i][j], UNITY_FREERTOS_PRIORITY + 1, &task_handles[i][j], i));
        }
    }

    // Start the created tasks
    জন্য (পূর্ণসংখ্যা i = 0; i < CONFIG_FREERTOS_সংখ্যা_OF_CORES; i++) {
        জন্য (পূর্ণসংখ্যা j = 0; j < TEST_অঙ্ক_TASKS; j++) {
            xTaskNotifyGive(task_handles[i][j]);
        }
    }

    // Wait for the tasks to complete
    জন্য (পূর্ণসংখ্যা i = 0; i < CONFIG_FREERTOS_সংখ্যা_OF_CORES * TEST_অঙ্ক_TASKS; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);
    }

    // Delete the tasks
    জন্য (পূর্ণসংখ্যা i = 0; i < CONFIG_FREERTOS_সংখ্যা_OF_CORES; i++) {
        জন্য (পূর্ণসংখ্যা j = 0; j < TEST_অঙ্ক_TASKS; j++) {
            vTaskDelete(task_handles[i][j]);
        }
    }

    // Check the values
    জন্য (পূর্ণসংখ্যা i = 0; i < CONFIG_FREERTOS_সংখ্যা_OF_CORES; i++) {
        জন্য (পূর্ণসংখ্যা j = 0; j < TEST_অঙ্ক_TASKS; j++) {
            dsp_params_t* param = &params[i][j];
            TEST_ASSERT_EQUAL(param->id * g_dsp_constant, param->ফলাফল);
        }
    }

    vSemaphoreDelete(done_sem);
}

#endif /* SOC_CPU_HAS_DSP */
