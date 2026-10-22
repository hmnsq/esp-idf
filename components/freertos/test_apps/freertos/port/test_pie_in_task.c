/*
 * SPDX-FileCopyrightText: 2024-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव <math.h>
#अन्तर्भाव "soc/soc_caps.h"
#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"
#अन्तर्भाव "freertos/semphr.h"
#अन्तर्भाव "freertos/idf_additions.h"
#अन्तर्भाव "unity.h"
#अन्तर्भाव "unity_test_runner.h"
#अन्तर्भाव "esp_system.h"
#अन्तर्भाव "test_utils.h"

/* PIE instructions set is currently only supported in GCC compiler */
#यदि SOC_CPU_HAS_PIE

/**
 * @brief Performs the signed sum of two 4-word vectors using the PIE.
 *
 * @param a First vector
 * @param b Second vector
 * @param dst Destination to store the sum
 */
शून्य pie_सदिशम्_signed_सङ्कलय(ध्रुव int32_t a[4], ध्रुव int32_t b[4], int32_t dst[4]);

/**
 * @brief Performs a XOR operation on two 4-word vectors using the PIE.
 *
 * @param a First vector
 * @param b Second vector
 * @param dst Destination to store the result
 */
शून्य pie_सदिशम्_xor(ध्रुव int32_t a[4], ध्रुव int32_t b[4], int32_t dst[4]);

/* ------------------------------------------------------------------------------------------------------------------ */

प्रकारनिर्देश संरचना {
    int32_t cst;
    TaskHandle_t मुख्यम्;
    SemaphoreHandle_t sem;
} pie_params_t;

/*
Test PIE usage from a task context

Purpose:
    - Test that the PIE can be used from a task context
    - Test that PIE context is properly saved and restored
    - Test that PIE context is cleaned up on task deletion by running multiple iterations
Procedure:
    - Create TEST_PINNED_NUM_TASKS tasks pinned to each core
    - Start each task
    - Each task updates a float variable and then blocks (to allow other tasks to run thus forcing the a PIE context
      save and restore).
    - Delete each task
    - Repeat test for TEST_PINNED_NUM_ITERS iterations
Expected:
    - Correct float value calculated by each task
    - Each task cleans up its PIE context on deletion
*/

#परिभाषय TEST_PINNED_अङ्कः_TASKS       3
#परिभाषय TEST_PINNED_अङ्कः_ITERS       5

स्थिर शून्य pinned_task(शून्य *arg)
{
    pie_params_t *param = (pie_params_t*) arg;
    ulTaskNotifyTake(pdTRUE, पत्तनम्_MAX_DELAY);

    int32_t constant = 42 * param->cst;
    int32_t a[4] = { constant, constant, constant, constant };
    int32_t b[4] = { 10, 20, 30, 40 };
    int32_t dst[4] = { 0 };

    pie_सदिशम्_signed_सङ्कलय(a, b, dst);

    प्रति (पूर्णाङ्क i = 0; i < परिमाण(a) / परिमाण(uint32_t); i++) {
        TEST_ASSERT_EQUAL(dst[i], a[i] + b[i]);
    }

    // Indicate done and wait to be deleted
    xSemaphoreGive((SemaphoreHandle_t)param->sem);
    vTaskSuspend(रिक्त);
}

TEST_CASE("PIE: Usage in task", "[freertos]")
{
    SemaphoreHandle_t समाप्तम्_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES * TEST_PINNED_अङ्कः_TASKS, 0);
    TEST_ASSERT_NOT_EQUAL(रिक्त, समाप्तम्_sem);

    प्रति (पूर्णाङ्क iter = 0; iter < TEST_PINNED_अङ्कः_ITERS; iter++) {
        TaskHandle_t task_handles[CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES][TEST_PINNED_अङ्कः_TASKS];
        pie_params_t params[CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES][TEST_PINNED_अङ्कः_TASKS];

        // Create test tasks for each core
        प्रति (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES; i++) {
            प्रति (पूर्णाङ्क j = 0; j < TEST_PINNED_अङ्कः_TASKS; j++) {
                params[i][j] = (pie_params_t) {
                    .cst = i + j + 1,
                    .sem = समाप्तम्_sem,
                };
                TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_गन्तव्यम्_Core(pinned_task, "task", 4096, (शून्य *) &params[i][j], UNITY_FREERTOS_प्राथमिकता + 1, &task_handles[i][j], i));
            }
        }

        // Start the created tasks
        प्रति (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES; i++) {
            प्रति (पूर्णाङ्क j = 0; j < TEST_PINNED_अङ्कः_TASKS; j++) {
                xTaskNotifyGive(task_handles[i][j]);
            }
        }

        // Wait for the tasks to complete
        प्रति (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES * TEST_PINNED_अङ्कः_TASKS; i++) {
            xSemaphoreTake(समाप्तम्_sem, पत्तनम्_MAX_DELAY);
        }

        // Delete the tasks
        प्रति (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES; i++) {
            प्रति (पूर्णाङ्क j = 0; j < TEST_PINNED_अङ्कः_TASKS; j++) {
                vTaskDelete(task_handles[i][j]);
            }
        }

        vTaskDelay(10); // Short delay to allow idle task to be free task memory and FPU contexts
    }

    vSemaphoreDelete(समाप्तम्_sem);
}

/* ------------------------------------------------------------------------------------------------------------------ */

/*
Test PIE usage will pin an unpinned task

Purpose:
    - Test that unpinned tasks are automatically pinned to the current core on the task's first use of the PIE
    - Test that PIE context is cleaned up on task deletion by running multiple iterations
Procedure:
    - Create an unpinned task
    - Task disables scheduling/preemption to ensure that it does not switch cores
    - Task uses the PIE
    - Task checks its core affinity after PIE usage
    - Task deletes itself
    - Repeat test for TEST_UNPINNED_NUM_ITERS iterations
Expected:
    - Task remains unpinned until its first usage of the PIE
    - The task becomes pinned to the current core after first use of the PIE
    - Each task cleans up its PIE context on deletion
*/

#यदि CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES > 1

#परिभाषय TEST_UNPINNED_अङ्कः_ITERS     5

स्थिर शून्य check_core_affinity(शून्य)
{
#यदि CONFIG_IDF_TARGET_ESP32S31
    /* On the ESP32-S31, only core 1 has PIE support, so all tasks should be pinned to core 1 */
    आधारः_Type_t core_अङ्कः = 1;
#अन्यथा
    आधारः_Type_t core_अङ्कः = xTaskGetCoreID(रिक्त);
#यदि_अन्तः

    TEST_ASSERT_EQUAL(core_अङ्कः, xTaskGetCoreID(रिक्त));
}

स्थिर शून्य unpinned_task(शून्य *arg)
{
    // Check that the task is unpinned
#यदि !CONFIG_FREERTOS_UNICORE
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, xTaskGetCoreID(रिक्त));
#यदि_अन्तः // !CONFIG_FREERTOS_UNICORE

    int32_t a[4] = { 0, 1, 2, 3};
    int32_t b[4] = { 111, 222, 333, 444 };
    int32_t dst[4] = { 0 };

    pie_सदिशम्_signed_सङ्कलय(a, b, dst);

    प्रति (पूर्णाङ्क i = 0; i < परिमाण(a) / परिमाण(uint32_t); i++) {
        TEST_ASSERT_EQUAL(dst[i], a[i] + b[i]);
    }

#यदि !CONFIG_FREERTOS_UNICORE
    check_core_affinity();
#यदि_अन्तः // !CONFIG_FREERTOS_UNICORE

    // Indicate done and self delete
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(रिक्त);
}

TEST_CASE("PIE: Usage in unpinned task", "[freertos]")
{
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    प्रति (पूर्णाङ्क iter = 0; iter < TEST_UNPINNED_अङ्कः_ITERS; iter++) {
        // Create unpinned task
        xTaskCreate(unpinned_task, "unpin", 4096, (शून्य *)unity_task_handle, UNITY_FREERTOS_प्राथमिकता + 1, रिक्त);
        // Wait for task to complete
        ulTaskNotifyTake(pdTRUE, पत्तनम्_MAX_DELAY);
        vTaskDelay(10); // Short delay to allow task memory to be freed
    }
}

/**
 * @brief Function performing some simple calculation using the PIE coprocessor.
 *        The goal is to be preempted by a task that also uses the PIE on the same core.
 */
स्थिर शून्य pie_calculation(शून्य* arg)
{
    pie_params_t* p = (pie_params_t*) arg;
    ध्रुव int32_t cst = p->cst;
    int32_t a[4] = { cst, cst, cst, cst };
    int32_t dst[4] = { 0 };

    प्रति (पूर्णाङ्क i = 0; i < 10; i++) {
        pie_सदिशम्_signed_सङ्कलय(a, dst, dst);

        /* Give some time to the other to interrupt us before checking `f` value */
        esp_rom_delay_us(1000);

        /* Using TEST_ASSERT_TRUE triggers a stack overflow, make sure the sign is still correct */
        प्रतिज्ञा((dst[0] < 0 && cst < 0) || (dst[0] > 0 && cst > 0));

        /* Give the hand back to FreeRTOS to avoid any watchdog error */
        vTaskDelay(2);
    }

    /* Make sure the result is correct */
    प्रतिज्ञा((dst[0] * cst == 10));

    xTaskNotifyGive(p->मुख्यम्);
    vTaskDelete(रिक्त);
}

TEST_CASE("PIE: Unsolicited context switch between tasks using the PIE", "[freertos]")
{
    /* Create two tasks that are on the same core and use the same FPU */
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    TaskHandle_t tasks[2];
    pie_params_t params[2] = {
        { .cst =  1, .मुख्यम् = unity_task_handle },
        { .cst = -1, .मुख्यम् = unity_task_handle },
    };

    x_Task_Create_Pinned_गन्तव्यम्_Core(pie_calculation, "Task1", 2048, params + 0, UNITY_FREERTOS_प्राथमिकता + 1, &tasks[0], 1);
    x_Task_Create_Pinned_गन्तव्यम्_Core(pie_calculation, "Task2", 2048, params + 1, UNITY_FREERTOS_प्राथमिकता + 1, &tasks[1], 1);

    ulTaskNotifyTake(pdTRUE, पत्तनम्_MAX_DELAY);
    ulTaskNotifyTake(pdTRUE, पत्तनम्_MAX_DELAY);
}

#यदि CONFIG_IDF_TARGET_ESP32S31

स्थिर शून्य core0_fpu_pie(शून्य *arg)
{
    /* Use the FPU in this task and make sure we are pinned to core 0 */
    भिन्न f = 1.0f;
    प्रति (पूर्णाङ्क i = 0; i < 10; i++) {
        f = f * 2.0f;
    }
    TEST_ASSERT_EQUAL(1024.0f, f);

    /* Check that the task is pinned to core 0 */
    TEST_ASSERT_EQUAL(0, xTaskGetCoreID(रिक्त));

    /* Use the PIE in this task and make sure we are migrated to core 1 */
    int32_t a[4] = { 0, 1, 2, 3};
    int32_t b[4] = { 111, 222, 333, 444 };
    int32_t dst[4] = { 0 };

    pie_सदिशम्_signed_सङ्कलय(a, b, dst);

    TEST_ASSERT_EQUAL(1, xTaskGetCoreID(रिक्त));

    प्रति (पूर्णाङ्क i = 0; i < परिमाण(a) / परिमाण(uint32_t); i++) {
        TEST_ASSERT_EQUAL(dst[i], (a[i] + b[i]));
    }

    /* Try to use the FPU again on the new core */
    f = f * 2.0f;
    TEST_ASSERT_EQUAL(2048.0f, f);

    /* Indicate done and self delete */
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(रिक्त);
}

TEST_CASE("PIE: Usage in a task that owns the FPU on core 0", "[freertos]")
{
    /* Make sure that a task that owns the FPU on core 0 can be migrated to core 1 when using the PIE */
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    x_Task_Create_Pinned_गन्तव्यम्_Core(core0_fpu_pie, "core0_fpu_pie", 4096,
                            (शून्य*) unity_task_handle, UNITY_FREERTOS_प्राथमिकता + 1, रिक्त, 0);
    // Wait for task to complete
    ulTaskNotifyTake(pdTRUE, पत्तनम्_MAX_DELAY);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#परिभाषय PIE_INSTR_गणनम् 2

स्थिर शून्य core0_pie_instr_task(शून्य *arg)
{
    pie_params_t *param = (pie_params_t*) arg;

    विकल्प (param->cst) {
    पक्ष 0: {
        int32_t a = 1;
        int32_t b = 2;
        int32_t dst = 0;

        /* Opcode of type 0b0110011 */
        __asm__ चञ्चल("esp.addx2 %0, %1, %2" : "=r"(dst) : "r"(a), "r"(b));
        भङ्ग;
    }
    पक्ष 1: {
        int32_t a[4] = { 0xffffff, 0xaaaaaaaa, 0x11111111, 0xffffffff };
        int32_t b[4] = { 0x11111111, 0xaaaaaaaa, 0xffffff, 0xffffffff };
        int32_t dst[4] = { 0 };
        /* Opcode of type 0bxx11x11 (x: don't care) */
        pie_सदिशम्_xor(a, b, dst);
        भङ्ग;
    }
    }
    /* Indicate done and wait for deletion */
    xTaskNotifyGive(param->मुख्यम्);
    vTaskDelay(पत्तनम्_MAX_DELAY);
}

/**
 * @brief On the ESP32-S3, core 0 does not natively parse PIE instructions due to a hardware quirk.
 * As such, the EXT_ILL CSR will not have the PIE bit set when a PIE instruction executes on core 0.
 * To handle this, instruction parsing is performed in software. This test case ensures that the software
 * parser correctly recognizes PIE instructions.
 */
TEST_CASE("PIE: Core 0 parses PIE instructions properly", "[freertos]")
{
    TaskHandle_t task_handle;
    pie_params_t param = { 0 };
    param.मुख्यम् = xTaskGetCurrentTaskHandle();

    /* Create tasks that will run on core 0 only. Each of them will execute one PIE instruction.
     * If they don't crash, it's a success.*/
    प्रति (पूर्णाङ्क i = 0; i < PIE_INSTR_गणनम्; i++) {
        param.cst = i;
        x_Task_Create_Pinned_गन्तव्यम्_Core(core0_pie_instr_task, "Core0PIE", 2048, &param, UNITY_FREERTOS_प्राथमिकता + 1, &task_handle, 0);
        /* Wait for the task to complete */
        ulTaskNotifyTake(pdTRUE, पत्तनम्_MAX_DELAY);
        /* Delete the task and start again */
        vTaskDelete(task_handle);
    }
}
#यदि_अन्तः /* CONFIG_IDF_TARGET_ESP32S31 */

#यदि_अन्तः /* CONFIG_FREERTOS_NUMBER_OF_CORES > 1 */

#यदि CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST

TEST_CASE("PIE: Blacklist setter and getter", "[freertos]")
{
    आधारः_Type_t before = xTaskGetPieBlacklisted(रिक्त);
    v_Task_समुच्चयः_Pie_Blacklisted(रिक्त, pdTRUE);
    आधारः_Type_t समुच्चयः = xTaskGetPieBlacklisted(रिक्त);
    v_Task_समुच्चयः_Pie_Blacklisted(रिक्त, pdFALSE);
    आधारः_Type_t cleared = xTaskGetPieBlacklisted(रिक्त);

    TEST_ASSERT_EQUAL(pdFALSE, before);
    TEST_ASSERT_EQUAL(pdTRUE, समुच्चयः);
    TEST_ASSERT_EQUAL(pdFALSE, cleared);
}

स्थिर शून्य pie_after_cleared_blacklist(शून्य *arg)
{
    int32_t a[4] = { 0, 1, 2, 3 };
    int32_t b[4] = { 1, 2, 3, 4 };
    int32_t dst[4] = { 0 };

    v_Task_समुच्चयः_Pie_Blacklisted(रिक्त, pdTRUE);
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetPieBlacklisted(रिक्त));
    v_Task_समुच्चयः_Pie_Blacklisted(रिक्त, pdFALSE);
    pie_सदिशम्_signed_सङ्कलय(a, b, dst);

    प्रति (पूर्णाङ्क i = 0; i < 4; i++) {
        TEST_ASSERT_EQUAL(a[i] + b[i], dst[i]);
    }

    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(रिक्त);
}

TEST_CASE("PIE: Cleared blacklist allows PIE use", "[freertos]")
{
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreate(pie_after_cleared_blacklist, "pie_clr", 4096,
                                          (शून्य *)unity_task_handle, UNITY_FREERTOS_प्राथमिकता + 1, रिक्त));
    ulTaskNotifyTake(pdTRUE, पत्तनम्_MAX_DELAY);
    vTaskDelay(10);
}

स्थिर शून्य pie_blacklist_then_खण्डम्(शून्य *arg)
{
    v_Task_समुच्चयः_Pie_Blacklisted(रिक्त, pdTRUE);
    xTaskNotifyGive((TaskHandle_t)arg);
    ulTaskNotifyTake(pdTRUE, पत्तनम्_MAX_DELAY);
    vTaskDelete(रिक्त);
}

स्थिर शून्य pie_notify_then_खण्डम्(शून्य *arg)
{
    xTaskNotifyGive((TaskHandle_t)arg);
    ulTaskNotifyTake(pdTRUE, पत्तनम्_MAX_DELAY);
    vTaskDelete(रिक्त);
}

/*
 * Both helpers block in ulTaskNotifyTake after notifying the parent. The parent
 * never notifies them back, so vTaskDelete(NULL) in the helpers does not run.
 * vTaskDelete() here is what deletes them and runs the blacklist cleanup hook.
 */
TEST_CASE("PIE: Deleted task is removed from blacklist", "[freertos]")
{
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    TaskHandle_t child = रिक्त;
    TaskHandle_t child2 = रिक्त;

    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreate(pie_blacklist_then_खण्डम्, "pie_del", 4096,
                                          (शून्य *)unity_task_handle, UNITY_FREERTOS_प्राथमिकता + 1, &child));
    ulTaskNotifyTake(pdTRUE, पत्तनम्_MAX_DELAY);
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetPieBlacklisted(child));
    vTaskDelete(child);
    vTaskDelay(10);
    TEST_ASSERT_EQUAL(pdFALSE, xTaskGetPieBlacklisted(child));

    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreate(pie_notify_then_खण्डम्, "pie_del2", 4096,
                                          (शून्य *)unity_task_handle, UNITY_FREERTOS_प्राथमिकता + 1, &child2));
    ulTaskNotifyTake(pdTRUE, पत्तनम्_MAX_DELAY);
    TEST_ASSERT_EQUAL(pdFALSE, xTaskGetPieBlacklisted(child2));
    vTaskDelete(child2);
    vTaskDelay(10);
}

स्थिर शून्य pie_blacklisted_task_executes_pie(शून्य)
{
    int32_t a[4] = { 0, 1, 2, 3 };
    int32_t b[4] = { 1, 2, 3, 4 };
    int32_t dst[4] = { 0 };

    v_Task_समुच्चयः_Pie_Blacklisted(रिक्त, pdTRUE);
    pie_सदिशम्_signed_सङ्कलय(a, b, dst);
    TEST_FAIL_सन्देशः("blacklisted task executed PIE without rebooting");
}

स्थिर शून्य check_pie_blacklist_panic_reset(शून्य)
{
    TEST_ASSERT_EQUAL(ESP_RST_PANIC, esp_reset_reason());
}

TEST_CASE_गुणजः_STAGES("PIE: Blacklisted task aborts on PIE use",
                          "[freertos][reset=SW_CPU_RESET]",
                          pie_blacklisted_task_executes_pie,
                          check_pie_blacklist_panic_reset);

स्थिर शून्य pie_blacklisted_owner_after_yield(शून्य)
{
    int32_t a[4] = { 0, 1, 2, 3 };
    int32_t b[4] = { 1, 2, 3, 4 };
    int32_t dst[4] = { 0 };

    pie_सदिशम्_signed_सङ्कलय(a, b, dst);
    v_Task_समुच्चयः_Pie_Blacklisted(रिक्त, pdTRUE);
    /* A same-task yield leaves PIE enabled, so the next instruction would not trap.
     * Blocking lets another task run and disables PIE. */
    vTaskDelay(1);
    pie_सदिशम्_signed_सङ्कलय(a, b, dst);
    TEST_FAIL_सन्देशः("blacklisted owner used PIE after a switch without rebooting");
}

TEST_CASE_गुणजः_STAGES("PIE: Blacklisted owner aborts after yield",
                          "[freertos][reset=SW_CPU_RESET]",
                          pie_blacklisted_owner_after_yield,
                          check_pie_blacklist_panic_reset);

#यदि_अन्तः /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */

#यदि_अन्तः /* SOC_CPU_HAS_PIE */
