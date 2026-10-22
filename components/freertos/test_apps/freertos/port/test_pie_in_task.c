/*
 * SPDX-FileCopyrightText: 2024-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ <math.h>
#ಅನ್ತರ್ಭಾವ "soc/soc_caps.h"
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "freertos/semphr.h"
#ಅನ್ತರ್ಭಾವ "freertos/idf_additions.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "unity_test_runner.h"
#ಅನ್ತರ್ಭಾವ "esp_system.h"
#ಅನ್ತರ್ಭಾವ "test_utils.h"

/* PIE instructions set is currently only supported in GCC compiler */
#ಆದರೆ SOC_CPU_HAS_PIE

/**
 * @brief Performs the signed sum of two 4-word vectors using the PIE.
 *
 * @param a First vector
 * @param b Second vector
 * @param dst Destination to store the sum
 */
ಶೂನ್ಯ pie_ಸದಿಶ_signed_ಕೂಡಿಸು(ಸ್ಥಿರಾಂಕ int32_t a[4], ಸ್ಥಿರಾಂಕ int32_t b[4], int32_t dst[4]);

/**
 * @brief Performs a XOR operation on two 4-word vectors using the PIE.
 *
 * @param a First vector
 * @param b Second vector
 * @param dst Destination to store the result
 */
ಶೂನ್ಯ pie_ಸದಿಶ_xor(ಸ್ಥಿರಾಂಕ int32_t a[4], ಸ್ಥಿರಾಂಕ int32_t b[4], int32_t dst[4]);

/* ------------------------------------------------------------------------------------------------------------------ */

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    int32_t cst;
    TaskHandle_t ಮುಖ್ಯ;
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

#ವ್ಯಾಖ್ಯಾನಿಸು TEST_PINNED_ಅಂಕೆ_TASKS       3
#ವ್ಯಾಖ್ಯಾನಿಸು TEST_PINNED_ಅಂಕೆ_ITERS       5

ಸ್ಥಿರ ಶೂನ್ಯ pinned_task(ಶೂನ್ಯ *arg)
{
    pie_params_t *param = (pie_params_t*) arg;
    ulTaskNotifyTake(pdTRUE, ಪೋರ್ಟ್_MAX_DELAY);

    int32_t constant = 42 * param->cst;
    int32_t a[4] = { constant, constant, constant, constant };
    int32_t b[4] = { 10, 20, 30, 40 };
    int32_t dst[4] = { 0 };

    pie_ಸದಿಶ_signed_ಕೂಡಿಸು(a, b, dst);

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಗಾತ್ರ(a) / ಗಾತ್ರ(uint32_t); i++) {
        TEST_ASSERT_EQUAL(dst[i], a[i] + b[i]);
    }

    // Indicate done and wait to be deleted
    xSemaphoreGive((SemaphoreHandle_t)param->sem);
    vTaskSuspend(ಖಾಲಿ);
}

TEST_CASE("PIE: Usage in task", "[freertos]")
{
    SemaphoreHandle_t ಮುಗಿದ_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES * TEST_PINNED_ಅಂಕೆ_TASKS, 0);
    TEST_ASSERT_NOT_EQUAL(ಖಾಲಿ, ಮುಗಿದ_sem);

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ iter = 0; iter < TEST_PINNED_ಅಂಕೆ_ITERS; iter++) {
        TaskHandle_t task_handles[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES][TEST_PINNED_ಅಂಕೆ_TASKS];
        pie_params_t params[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES][TEST_PINNED_ಅಂಕೆ_TASKS];

        // Create test tasks for each core
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; i++) {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < TEST_PINNED_ಅಂಕೆ_TASKS; j++) {
                params[i][j] = (pie_params_t) {
                    .cst = i + j + 1,
                    .sem = ಮುಗಿದ_sem,
                };
                TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_ಗಮ್ಯ_Core(pinned_task, "task", 4096, (ಶೂನ್ಯ *) &params[i][j], UNITY_FREERTOS_ಆದ್ಯತೆ + 1, &task_handles[i][j], i));
            }
        }

        // Start the created tasks
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; i++) {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < TEST_PINNED_ಅಂಕೆ_TASKS; j++) {
                xTaskNotifyGive(task_handles[i][j]);
            }
        }

        // Wait for the tasks to complete
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES * TEST_PINNED_ಅಂಕೆ_TASKS; i++) {
            xSemaphoreTake(ಮುಗಿದ_sem, ಪೋರ್ಟ್_MAX_DELAY);
        }

        // Delete the tasks
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; i++) {
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < TEST_PINNED_ಅಂಕೆ_TASKS; j++) {
                vTaskDelete(task_handles[i][j]);
            }
        }

        vTaskDelay(10); // Short delay to allow idle task to be free task memory and FPU contexts
    }

    vSemaphoreDelete(ಮುಗಿದ_sem);
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

#ಆದರೆ CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES > 1

#ವ್ಯಾಖ್ಯಾನಿಸು TEST_UNPINNED_ಅಂಕೆ_ITERS     5

ಸ್ಥಿರ ಶೂನ್ಯ check_core_affinity(ಶೂನ್ಯ)
{
#ಆದರೆ CONFIG_IDF_ಗುರಿ_ESP32_S31
    /* On the ESP32-S31, only core 1 has PIE support, so all tasks should be pinned to core 1 */
    ತಳ_Type_t core_ಅಂಕೆ = 1;
#ಇಲ್ಲದಿದ್ದರೆ
    ತಳ_Type_t core_ಅಂಕೆ = xTaskGetCoreID(ಖಾಲಿ);
#ಆದರೆ_ಅಂತ್ಯ

    TEST_ASSERT_EQUAL(core_ಅಂಕೆ, xTaskGetCoreID(ಖಾಲಿ));
}

ಸ್ಥಿರ ಶೂನ್ಯ unpinned_task(ಶೂನ್ಯ *arg)
{
    // Check that the task is unpinned
#ಆದರೆ !CONFIG_FREERTOS_UNICORE
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, xTaskGetCoreID(ಖಾಲಿ));
#ಆದರೆ_ಅಂತ್ಯ // !CONFIG_FREERTOS_UNICORE

    int32_t a[4] = { 0, 1, 2, 3};
    int32_t b[4] = { 111, 222, 333, 444 };
    int32_t dst[4] = { 0 };

    pie_ಸದಿಶ_signed_ಕೂಡಿಸು(a, b, dst);

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಗಾತ್ರ(a) / ಗಾತ್ರ(uint32_t); i++) {
        TEST_ASSERT_EQUAL(dst[i], a[i] + b[i]);
    }

#ಆದರೆ !CONFIG_FREERTOS_UNICORE
    check_core_affinity();
#ಆದರೆ_ಅಂತ್ಯ // !CONFIG_FREERTOS_UNICORE

    // Indicate done and self delete
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(ಖಾಲಿ);
}

TEST_CASE("PIE: Usage in unpinned task", "[freertos]")
{
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ iter = 0; iter < TEST_UNPINNED_ಅಂಕೆ_ITERS; iter++) {
        // Create unpinned task
        xTaskCreate(unpinned_task, "unpin", 4096, (ಶೂನ್ಯ *)unity_task_handle, UNITY_FREERTOS_ಆದ್ಯತೆ + 1, ಖಾಲಿ);
        // Wait for task to complete
        ulTaskNotifyTake(pdTRUE, ಪೋರ್ಟ್_MAX_DELAY);
        vTaskDelay(10); // Short delay to allow task memory to be freed
    }
}

/**
 * @brief Function performing some simple calculation using the PIE coprocessor.
 *        The goal is to be preempted by a task that also uses the PIE on the same core.
 */
ಸ್ಥಿರ ಶೂನ್ಯ pie_calculation(ಶೂನ್ಯ* arg)
{
    pie_params_t* p = (pie_params_t*) arg;
    ಸ್ಥಿರಾಂಕ int32_t cst = p->cst;
    int32_t a[4] = { cst, cst, cst, cst };
    int32_t dst[4] = { 0 };

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 10; i++) {
        pie_ಸದಿಶ_signed_ಕೂಡಿಸು(a, dst, dst);

        /* Give some time to the other to interrupt us before checking `f` value */
        esp_rom_delay_us(1000);

        /* Using TEST_ASSERT_TRUE triggers a stack overflow, make sure the sign is still correct */
        ಪ್ರತಿಜ್ಞಾ((dst[0] < 0 && cst < 0) || (dst[0] > 0 && cst > 0));

        /* Give the hand back to FreeRTOS to avoid any watchdog error */
        vTaskDelay(2);
    }

    /* Make sure the result is correct */
    ಪ್ರತಿಜ್ಞಾ((dst[0] * cst == 10));

    xTaskNotifyGive(p->ಮುಖ್ಯ);
    vTaskDelete(ಖಾಲಿ);
}

TEST_CASE("PIE: Unsolicited context switch between tasks using the PIE", "[freertos]")
{
    /* Create two tasks that are on the same core and use the same FPU */
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    TaskHandle_t tasks[2];
    pie_params_t params[2] = {
        { .cst =  1, .ಮುಖ್ಯ = unity_task_handle },
        { .cst = -1, .ಮುಖ್ಯ = unity_task_handle },
    };

    x_Task_Create_Pinned_ಗಮ್ಯ_Core(pie_calculation, "Task1", 2048, params + 0, UNITY_FREERTOS_ಆದ್ಯತೆ + 1, &tasks[0], 1);
    x_Task_Create_Pinned_ಗಮ್ಯ_Core(pie_calculation, "Task2", 2048, params + 1, UNITY_FREERTOS_ಆದ್ಯತೆ + 1, &tasks[1], 1);

    ulTaskNotifyTake(pdTRUE, ಪೋರ್ಟ್_MAX_DELAY);
    ulTaskNotifyTake(pdTRUE, ಪೋರ್ಟ್_MAX_DELAY);
}

#ಆದರೆ CONFIG_IDF_ಗುರಿ_ESP32_S31

ಸ್ಥಿರ ಶೂನ್ಯ core0_fpu_pie(ಶೂನ್ಯ *arg)
{
    /* Use the FPU in this task and make sure we are pinned to core 0 */
    ದಶಮಾಂಶ f = 1.0f;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 10; i++) {
        f = f * 2.0f;
    }
    TEST_ASSERT_EQUAL(1024.0f, f);

    /* Check that the task is pinned to core 0 */
    TEST_ASSERT_EQUAL(0, xTaskGetCoreID(ಖಾಲಿ));

    /* Use the PIE in this task and make sure we are migrated to core 1 */
    int32_t a[4] = { 0, 1, 2, 3};
    int32_t b[4] = { 111, 222, 333, 444 };
    int32_t dst[4] = { 0 };

    pie_ಸದಿಶ_signed_ಕೂಡಿಸು(a, b, dst);

    TEST_ASSERT_EQUAL(1, xTaskGetCoreID(ಖಾಲಿ));

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಗಾತ್ರ(a) / ಗಾತ್ರ(uint32_t); i++) {
        TEST_ASSERT_EQUAL(dst[i], (a[i] + b[i]));
    }

    /* Try to use the FPU again on the new core */
    f = f * 2.0f;
    TEST_ASSERT_EQUAL(2048.0f, f);

    /* Indicate done and self delete */
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(ಖಾಲಿ);
}

TEST_CASE("PIE: Usage in a task that owns the FPU on core 0", "[freertos]")
{
    /* Make sure that a task that owns the FPU on core 0 can be migrated to core 1 when using the PIE */
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    x_Task_Create_Pinned_ಗಮ್ಯ_Core(core0_fpu_pie, "core0_fpu_pie", 4096,
                            (ಶೂನ್ಯ*) unity_task_handle, UNITY_FREERTOS_ಆದ್ಯತೆ + 1, ಖಾಲಿ, 0);
    // Wait for task to complete
    ulTaskNotifyTake(pdTRUE, ಪೋರ್ಟ್_MAX_DELAY);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#ವ್ಯಾಖ್ಯಾನಿಸು PIE_INSTR_ಎಣಿಕೆ 2

ಸ್ಥಿರ ಶೂನ್ಯ core0_pie_instr_task(ಶೂನ್ಯ *arg)
{
    pie_params_t *param = (pie_params_t*) arg;

    ಆಯ್ಕೆ (param->cst) {
    ಸ್ಥಿತಿ 0: {
        int32_t a = 1;
        int32_t b = 2;
        int32_t dst = 0;

        /* Opcode of type 0b0110011 */
        __asm__ ಅಸ್ಥಿರ("esp.addx2 %0, %1, %2" : "=r"(dst) : "r"(a), "r"(b));
        ನಿಲ್ಲಿಸು;
    }
    ಸ್ಥಿತಿ 1: {
        int32_t a[4] = { 0xffffff, 0xaaaaaaaa, 0x11111111, 0xffffffff };
        int32_t b[4] = { 0x11111111, 0xaaaaaaaa, 0xffffff, 0xffffffff };
        int32_t dst[4] = { 0 };
        /* Opcode of type 0bxx11x11 (x: don't care) */
        pie_ಸದಿಶ_xor(a, b, dst);
        ನಿಲ್ಲಿಸು;
    }
    }
    /* Indicate done and wait for deletion */
    xTaskNotifyGive(param->ಮುಖ್ಯ);
    vTaskDelay(ಪೋರ್ಟ್_MAX_DELAY);
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
    param.ಮುಖ್ಯ = xTaskGetCurrentTaskHandle();

    /* Create tasks that will run on core 0 only. Each of them will execute one PIE instruction.
     * If they don't crash, it's a success.*/
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < PIE_INSTR_ಎಣಿಕೆ; i++) {
        param.cst = i;
        x_Task_Create_Pinned_ಗಮ್ಯ_Core(core0_pie_instr_task, "Core0PIE", 2048, &param, UNITY_FREERTOS_ಆದ್ಯತೆ + 1, &task_handle, 0);
        /* Wait for the task to complete */
        ulTaskNotifyTake(pdTRUE, ಪೋರ್ಟ್_MAX_DELAY);
        /* Delete the task and start again */
        vTaskDelete(task_handle);
    }
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_IDF_TARGET_ESP32S31 */

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_NUMBER_OF_CORES > 1 */

#ಆದರೆ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST

TEST_CASE("PIE: Blacklist setter and getter", "[freertos]")
{
    ತಳ_Type_t before = xTaskGetPieBlacklisted(ಖಾಲಿ);
    v_Task_ಗಣ_Pie_Blacklisted(ಖಾಲಿ, pdTRUE);
    ತಳ_Type_t ಗಣ = xTaskGetPieBlacklisted(ಖಾಲಿ);
    v_Task_ಗಣ_Pie_Blacklisted(ಖಾಲಿ, pdFALSE);
    ತಳ_Type_t cleared = xTaskGetPieBlacklisted(ಖಾಲಿ);

    TEST_ASSERT_EQUAL(pdFALSE, before);
    TEST_ASSERT_EQUAL(pdTRUE, ಗಣ);
    TEST_ASSERT_EQUAL(pdFALSE, cleared);
}

ಸ್ಥಿರ ಶೂನ್ಯ pie_after_cleared_blacklist(ಶೂನ್ಯ *arg)
{
    int32_t a[4] = { 0, 1, 2, 3 };
    int32_t b[4] = { 1, 2, 3, 4 };
    int32_t dst[4] = { 0 };

    v_Task_ಗಣ_Pie_Blacklisted(ಖಾಲಿ, pdTRUE);
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetPieBlacklisted(ಖಾಲಿ));
    v_Task_ಗಣ_Pie_Blacklisted(ಖಾಲಿ, pdFALSE);
    pie_ಸದಿಶ_signed_ಕೂಡಿಸು(a, b, dst);

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 4; i++) {
        TEST_ASSERT_EQUAL(a[i] + b[i], dst[i]);
    }

    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(ಖಾಲಿ);
}

TEST_CASE("PIE: Cleared blacklist allows PIE use", "[freertos]")
{
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreate(pie_after_cleared_blacklist, "pie_clr", 4096,
                                          (ಶೂನ್ಯ *)unity_task_handle, UNITY_FREERTOS_ಆದ್ಯತೆ + 1, ಖಾಲಿ));
    ulTaskNotifyTake(pdTRUE, ಪೋರ್ಟ್_MAX_DELAY);
    vTaskDelay(10);
}

ಸ್ಥಿರ ಶೂನ್ಯ pie_blacklist_then_ಖಂಡ(ಶೂನ್ಯ *arg)
{
    v_Task_ಗಣ_Pie_Blacklisted(ಖಾಲಿ, pdTRUE);
    xTaskNotifyGive((TaskHandle_t)arg);
    ulTaskNotifyTake(pdTRUE, ಪೋರ್ಟ್_MAX_DELAY);
    vTaskDelete(ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ pie_notify_then_ಖಂಡ(ಶೂನ್ಯ *arg)
{
    xTaskNotifyGive((TaskHandle_t)arg);
    ulTaskNotifyTake(pdTRUE, ಪೋರ್ಟ್_MAX_DELAY);
    vTaskDelete(ಖಾಲಿ);
}

/*
 * Both helpers block in ulTaskNotifyTake after notifying the parent. The parent
 * never notifies them back, so vTaskDelete(NULL) in the helpers does not run.
 * vTaskDelete() here is what deletes them and runs the blacklist cleanup hook.
 */
TEST_CASE("PIE: Deleted task is removed from blacklist", "[freertos]")
{
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    TaskHandle_t ಮಗು = ಖಾಲಿ;
    TaskHandle_t child2 = ಖಾಲಿ;

    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreate(pie_blacklist_then_ಖಂಡ, "pie_del", 4096,
                                          (ಶೂನ್ಯ *)unity_task_handle, UNITY_FREERTOS_ಆದ್ಯತೆ + 1, &ಮಗು));
    ulTaskNotifyTake(pdTRUE, ಪೋರ್ಟ್_MAX_DELAY);
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetPieBlacklisted(ಮಗು));
    vTaskDelete(ಮಗು);
    vTaskDelay(10);
    TEST_ASSERT_EQUAL(pdFALSE, xTaskGetPieBlacklisted(ಮಗು));

    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreate(pie_notify_then_ಖಂಡ, "pie_del2", 4096,
                                          (ಶೂನ್ಯ *)unity_task_handle, UNITY_FREERTOS_ಆದ್ಯತೆ + 1, &child2));
    ulTaskNotifyTake(pdTRUE, ಪೋರ್ಟ್_MAX_DELAY);
    TEST_ASSERT_EQUAL(pdFALSE, xTaskGetPieBlacklisted(child2));
    vTaskDelete(child2);
    vTaskDelay(10);
}

ಸ್ಥಿರ ಶೂನ್ಯ pie_blacklisted_task_executes_pie(ಶೂನ್ಯ)
{
    int32_t a[4] = { 0, 1, 2, 3 };
    int32_t b[4] = { 1, 2, 3, 4 };
    int32_t dst[4] = { 0 };

    v_Task_ಗಣ_Pie_Blacklisted(ಖಾಲಿ, pdTRUE);
    pie_ಸದಿಶ_signed_ಕೂಡಿಸು(a, b, dst);
    TEST_FAIL_ಸಂದೇಶ("blacklisted task executed PIE without rebooting");
}

ಸ್ಥಿರ ಶೂನ್ಯ check_pie_blacklist_panic_reset(ಶೂನ್ಯ)
{
    TEST_ASSERT_EQUAL(ESP_RST_PANIC, esp_reset_reason());
}

TEST_CASE_ಅಪವರ್ತ್ಯ_STAGES("PIE: Blacklisted task aborts on PIE use",
                          "[freertos][reset=SW_CPU_RESET]",
                          pie_blacklisted_task_executes_pie,
                          check_pie_blacklist_panic_reset);

ಸ್ಥಿರ ಶೂನ್ಯ pie_blacklisted_owner_after_yield(ಶೂನ್ಯ)
{
    int32_t a[4] = { 0, 1, 2, 3 };
    int32_t b[4] = { 1, 2, 3, 4 };
    int32_t dst[4] = { 0 };

    pie_ಸದಿಶ_signed_ಕೂಡಿಸು(a, b, dst);
    v_Task_ಗಣ_Pie_Blacklisted(ಖಾಲಿ, pdTRUE);
    /* A same-task yield leaves PIE enabled, so the next instruction would not trap.
     * Blocking lets another task run and disables PIE. */
    vTaskDelay(1);
    pie_ಸದಿಶ_signed_ಕೂಡಿಸು(a, b, dst);
    TEST_FAIL_ಸಂದೇಶ("blacklisted owner used PIE after a switch without rebooting");
}

TEST_CASE_ಅಪವರ್ತ್ಯ_STAGES("PIE: Blacklisted owner aborts after yield",
                          "[freertos][reset=SW_CPU_RESET]",
                          pie_blacklisted_owner_after_yield,
                          check_pie_blacklist_panic_reset);

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */

#ಆದರೆ_ಅಂತ್ಯ /* SOC_CPU_HAS_PIE */
