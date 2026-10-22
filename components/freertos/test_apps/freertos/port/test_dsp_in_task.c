/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ଅନ୍ତର୍ଭାଵ "sdkconfig.h"
#ଅନ୍ତର୍ଭାଵ <math.h>
#ଅନ୍ତର୍ଭାଵ "soc/soc_caps.h"
#ଅନ୍ତର୍ଭାଵ "freertos/FreeRTOS.h"
#ଅନ୍ତର୍ଭାଵ "freertos/task.h"
#ଅନ୍ତର୍ଭାଵ "freertos/semphr.h"
#ଅନ୍ତର୍ଭାଵ "unity.h"
#ଅନ୍ତର୍ଭାଵ "test_utils.h"

#ଯଦି SOC_CPU_HAS_DSP

#define TEST_ଅଙ୍କ_TASKS       4

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    int32_t  id;
    uint32_t result;
    TaskHandle_t ମୁଖ୍ୟ;
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
ଅଚଳ uint32_t g_dsp_constant = 100000;

ସ୍ଥିର ଶୂନ୍ୟ pinned_task(ଶୂନ୍ୟ *arg)
{
    dsp_params_t *param = (dsp_params_t*) arg;
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    param->result = dsp_id_mul(param->id);

    /* Indicate done and wait to be deleted */
    xSemaphoreGive((SemaphoreHandle_t)param->sem);
    vTaskSuspend(ଖାଲି);
}

TEST_CASE("DSP: Usage in task", "[freertos]")
{
    TaskHandle_t task_handles[CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES][TEST_ଅଙ୍କ_TASKS];
    dsp_params_t params[CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES][TEST_ଅଙ୍କ_TASKS];

    SemaphoreHandle_t done_sem = xSemaphoreCreateCounting(TEST_ଅଙ୍କ_TASKS, 0);
    TEST_ASSERT_NOT_EQUAL(ଖାଲି, done_sem);

    // Create test tasks for each core
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; i++) {
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < TEST_ଅଙ୍କ_TASKS; j++) {
            params[i][j] = (dsp_params_t) {
                .id = i * TEST_ଅଙ୍କ_TASKS + j + 1,
                .sem = done_sem,
            };
            TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(pinned_task, "task", 4096, (ଶୂନ୍ୟ *) &params[i][j], UNITY_FREERTOS_PRIORITY + 1, &task_handles[i][j], i));
        }
    }

    // Start the created tasks
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; i++) {
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < TEST_ଅଙ୍କ_TASKS; j++) {
            xTaskNotifyGive(task_handles[i][j]);
        }
    }

    // Wait for the tasks to complete
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES * TEST_ଅଙ୍କ_TASKS; i++) {
        xSemaphoreTake(done_sem, portMAX_DELAY);
    }

    // Delete the tasks
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; i++) {
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < TEST_ଅଙ୍କ_TASKS; j++) {
            vTaskDelete(task_handles[i][j]);
        }
    }

    // Check the values
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; i++) {
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < TEST_ଅଙ୍କ_TASKS; j++) {
            dsp_params_t* param = &params[i][j];
            TEST_ASSERT_EQUAL(param->id * g_dsp_constant, param->result);
        }
    }

    vSemaphoreDelete(done_sem);
}

#endif /* SOC_CPU_HAS_DSP */
