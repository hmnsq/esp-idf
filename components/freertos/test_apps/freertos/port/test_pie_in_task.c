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
#ಯದಿ SOC_CPU_HAS_PIE

/**
 * @brief Performs the signed sum of two 4-word vectors using the PIE.
 *
 * @param a First vector
 * @param b Second vector
 * @param dst Destination to store the sum
 */
ಶೂನ್ಯ pie_vector_signed_add(ಧ್ರುವ int32_t a[4], ಧ್ರುವ int32_t b[4], int32_t dst[4]);

/**
 * @brief Performs a XOR operation on two 4-word vectors using the PIE.
 *
 * @param a First vector
 * @param b Second vector
 * @param dst Destination to store the result
 */
ಶೂನ್ಯ pie_vector_xor(ಧ್ರುವ int32_t a[4], ಧ್ರುವ int32_t b[4], int32_t dst[4]);

/* ------------------------------------------------------------------------------------------------------------------ */

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    int32_t cst;
    TaskHandle_t main;
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

#define TEST_PINNED_ಅಂಕೆ_TASKS       3
#define TEST_PINNED_ಅಂಕೆ_ITERS       5

ಸ್ಥಿರ ಶೂನ್ಯ pinned_task(ಶೂನ್ಯ *arg)
{
    pie_params_t *param = (pie_params_t*) arg;
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    int32_t constant = 42 * param->cst;
    int32_t a[4] = { constant, constant, constant, constant };
    int32_t b[4] = { 10, 20, 30, 40 };
    int32_t dst[4] = { 0 };

    pie_vector_signed_add(a, b, dst);

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಪರಿಮಾಣ(a) / ಪರಿಮಾಣ(uint32_t); i++) {
        TEST_ASSERT_EQUAL(dst[i], a[i] + b[i]);
    }

    // Indicate done and wait to be deleted
    xSemaphoreGive((SemaphoreHandle_t)param->sem);
    vTaskSuspend(ರಿಕ್ತ);
}

TEST_CASE("PIE: Usage in task", "[freertos]")
{
    SemaphoreHandle_t done_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES * TEST_PINNED_ಅಂಕೆ_TASKS, 0);
    TEST_ASSERT_NOT_EQUAL(ರಿಕ್ತ, done_sem);

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ iter = 0; iter < TEST_PINNED_ಅಂಕೆ_ITERS; iter++) {
        TaskHandle_t task_handles[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES][TEST_PINNED_ಅಂಕೆ_TASKS];
        pie_params_t params[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES][TEST_PINNED_ಅಂಕೆ_TASKS];

        // Create test tasks for each core
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES; i++) {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < TEST_PINNED_ಅಂಕೆ_TASKS; j++) {
                params[i][j] = (pie_params_t) {
                    .cst = i + j + 1,
                    .sem = done_sem,
                };
                TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(pinned_task, "task", 4096, (ಶೂನ್ಯ *) &params[i][j], UNITY_FREERTOS_PRIORITY + 1, &task_handles[i][j], i));
            }
        }

        // Start the created tasks
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES; i++) {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < TEST_PINNED_ಅಂಕೆ_TASKS; j++) {
                xTaskNotifyGive(task_handles[i][j]);
            }
        }

        // Wait for the tasks to complete
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES * TEST_PINNED_ಅಂಕೆ_TASKS; i++) {
            xSemaphoreTake(done_sem, portMAX_DELAY);
        }

        // Delete the tasks
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES; i++) {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < TEST_PINNED_ಅಂಕೆ_TASKS; j++) {
                vTaskDelete(task_handles[i][j]);
            }
        }

        vTaskDelay(10); // Short delay to allow idle task to be free task memory and FPU contexts
    }

    vSemaphoreDelete(done_sem);
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

#ಯದಿ CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES > 1

#define TEST_UNPINNED_ಅಂಕೆ_ITERS     5

ಸ್ಥಿರ ಶೂನ್ಯ check_core_affinity(ಶೂನ್ಯ)
{
#ಯದಿ CONFIG_IDF_TARGET_ESP32S31
    /* On the ESP32-S31, only core 1 has PIE support, so all tasks should be pinned to core 1 */
    BaseType_t core_ಅಂಕೆ = 1;
#ಅನ್ಯಥಾ
    BaseType_t core_ಅಂಕೆ = xTaskGetCoreID(ರಿಕ್ತ);
#endif

    TEST_ASSERT_EQUAL(core_ಅಂಕೆ, xTaskGetCoreID(ರಿಕ್ತ));
}

ಸ್ಥಿರ ಶೂನ್ಯ unpinned_task(ಶೂನ್ಯ *arg)
{
    // Check that the task is unpinned
#ಯದಿ !CONFIG_FREERTOS_UNICORE
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, xTaskGetCoreID(ರಿಕ್ತ));
#endif // !CONFIG_FREERTOS_UNICORE

    int32_t a[4] = { 0, 1, 2, 3};
    int32_t b[4] = { 111, 222, 333, 444 };
    int32_t dst[4] = { 0 };

    pie_vector_signed_add(a, b, dst);

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಪರಿಮಾಣ(a) / ಪರಿಮಾಣ(uint32_t); i++) {
        TEST_ASSERT_EQUAL(dst[i], a[i] + b[i]);
    }

#ಯದಿ !CONFIG_FREERTOS_UNICORE
    check_core_affinity();
#endif // !CONFIG_FREERTOS_UNICORE

    // Indicate done and self delete
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(ರಿಕ್ತ);
}

TEST_CASE("PIE: Usage in unpinned task", "[freertos]")
{
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ iter = 0; iter < TEST_UNPINNED_ಅಂಕೆ_ITERS; iter++) {
        // Create unpinned task
        xTaskCreate(unpinned_task, "unpin", 4096, (ಶೂನ್ಯ *)unity_task_handle, UNITY_FREERTOS_PRIORITY + 1, ರಿಕ್ತ);
        // Wait for task to complete
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
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
    ಧ್ರುವ int32_t cst = p->cst;
    int32_t a[4] = { cst, cst, cst, cst };
    int32_t dst[4] = { 0 };

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < 10; i++) {
        pie_vector_signed_add(a, dst, dst);

        /* Give some time to the other to interrupt us before checking `f` value */
        esp_rom_delay_us(1000);

        /* Using TEST_ASSERT_TRUE triggers a stack overflow, make sure the sign is still correct */
        ಪ್ರತಿಜ್ಞಾ((dst[0] < 0 && cst < 0) || (dst[0] > 0 && cst > 0));

        /* Give the hand back to FreeRTOS to avoid any watchdog error */
        vTaskDelay(2);
    }

    /* Make sure the result is correct */
    ಪ್ರತಿಜ್ಞಾ((dst[0] * cst == 10));

    xTaskNotifyGive(p->main);
    vTaskDelete(ರಿಕ್ತ);
}

TEST_CASE("PIE: Unsolicited context switch between tasks using the PIE", "[freertos]")
{
    /* Create two tasks that are on the same core and use the same FPU */
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    TaskHandle_t tasks[2];
    pie_params_t params[2] = {
        { .cst =  1, .main = unity_task_handle },
        { .cst = -1, .main = unity_task_handle },
    };

    xTaskCreatePinnedToCore(pie_calculation, "Task1", 2048, params + 0, UNITY_FREERTOS_PRIORITY + 1, &tasks[0], 1);
    xTaskCreatePinnedToCore(pie_calculation, "Task2", 2048, params + 1, UNITY_FREERTOS_PRIORITY + 1, &tasks[1], 1);

    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
}

#ಯದಿ CONFIG_IDF_TARGET_ESP32S31

ಸ್ಥಿರ ಶೂನ್ಯ core0_fpu_pie(ಶೂನ್ಯ *arg)
{
    /* Use the FPU in this task and make sure we are pinned to core 0 */
    ಭಿನ್ನ f = 1.0f;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < 10; i++) {
        f = f * 2.0f;
    }
    TEST_ASSERT_EQUAL(1024.0f, f);

    /* Check that the task is pinned to core 0 */
    TEST_ASSERT_EQUAL(0, xTaskGetCoreID(ರಿಕ್ತ));

    /* Use the PIE in this task and make sure we are migrated to core 1 */
    int32_t a[4] = { 0, 1, 2, 3};
    int32_t b[4] = { 111, 222, 333, 444 };
    int32_t dst[4] = { 0 };

    pie_vector_signed_add(a, b, dst);

    TEST_ASSERT_EQUAL(1, xTaskGetCoreID(ರಿಕ್ತ));

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಪರಿಮಾಣ(a) / ಪರಿಮಾಣ(uint32_t); i++) {
        TEST_ASSERT_EQUAL(dst[i], (a[i] + b[i]));
    }

    /* Try to use the FPU again on the new core */
    f = f * 2.0f;
    TEST_ASSERT_EQUAL(2048.0f, f);

    /* Indicate done and self delete */
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(ರಿಕ್ತ);
}

TEST_CASE("PIE: Usage in a task that owns the FPU on core 0", "[freertos]")
{
    /* Make sure that a task that owns the FPU on core 0 can be migrated to core 1 when using the PIE */
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    xTaskCreatePinnedToCore(core0_fpu_pie, "core0_fpu_pie", 4096,
                            (ಶೂನ್ಯ*) unity_task_handle, UNITY_FREERTOS_PRIORITY + 1, ರಿಕ್ತ, 0);
    // Wait for task to complete
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#define PIE_INSTR_ಎಣಿಕೆ 2

ಸ್ಥಿರ ಶೂನ್ಯ core0_pie_instr_task(ಶೂನ್ಯ *arg)
{
    pie_params_t *param = (pie_params_t*) arg;

    ವಿಕಲ್ಪ (param->cst) {
    ಪಕ್ಷ 0: {
        int32_t a = 1;
        int32_t b = 2;
        int32_t dst = 0;

        /* Opcode of type 0b0110011 */
        __asm__ ಚಞ್ಚಲ("esp.addx2 %0, %1, %2" : "=r"(dst) : "r"(a), "r"(b));
        ಭಙ್ಗ;
    }
    ಪಕ್ಷ 1: {
        int32_t a[4] = { 0xffffff, 0xaaaaaaaa, 0x11111111, 0xffffffff };
        int32_t b[4] = { 0x11111111, 0xaaaaaaaa, 0xffffff, 0xffffffff };
        int32_t dst[4] = { 0 };
        /* Opcode of type 0bxx11x11 (x: don't care) */
        pie_vector_xor(a, b, dst);
        ಭಙ್ಗ;
    }
    }
    /* Indicate done and wait for deletion */
    xTaskNotifyGive(param->main);
    vTaskDelay(portMAX_DELAY);
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
    param.main = xTaskGetCurrentTaskHandle();

    /* Create tasks that will run on core 0 only. Each of them will execute one PIE instruction.
     * If they don't crash, it's a success.*/
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < PIE_INSTR_ಎಣಿಕೆ; i++) {
        param.cst = i;
        xTaskCreatePinnedToCore(core0_pie_instr_task, "Core0PIE", 2048, &param, UNITY_FREERTOS_PRIORITY + 1, &task_handle, 0);
        /* Wait for the task to complete */
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        /* Delete the task and start again */
        vTaskDelete(task_handle);
    }
}
#endif /* CONFIG_IDF_TARGET_ESP32S31 */

#endif /* CONFIG_FREERTOS_NUMBER_OF_CORES > 1 */

#ಯದಿ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST

TEST_CASE("PIE: Blacklist setter and getter", "[freertos]")
{
    BaseType_t before = xTaskGetPieBlacklisted(ರಿಕ್ತ);
    vTaskSetPieBlacklisted(ರಿಕ್ತ, pdTRUE);
    BaseType_t set = xTaskGetPieBlacklisted(ರಿಕ್ತ);
    vTaskSetPieBlacklisted(ರಿಕ್ತ, pdFALSE);
    BaseType_t cleared = xTaskGetPieBlacklisted(ರಿಕ್ತ);

    TEST_ASSERT_EQUAL(pdFALSE, before);
    TEST_ASSERT_EQUAL(pdTRUE, set);
    TEST_ASSERT_EQUAL(pdFALSE, cleared);
}

ಸ್ಥಿರ ಶೂನ್ಯ pie_after_cleared_blacklist(ಶೂನ್ಯ *arg)
{
    int32_t a[4] = { 0, 1, 2, 3 };
    int32_t b[4] = { 1, 2, 3, 4 };
    int32_t dst[4] = { 0 };

    vTaskSetPieBlacklisted(ರಿಕ್ತ, pdTRUE);
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetPieBlacklisted(ರಿಕ್ತ));
    vTaskSetPieBlacklisted(ರಿಕ್ತ, pdFALSE);
    pie_vector_signed_add(a, b, dst);

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < 4; i++) {
        TEST_ASSERT_EQUAL(a[i] + b[i], dst[i]);
    }

    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(ರಿಕ್ತ);
}

TEST_CASE("PIE: Cleared blacklist allows PIE use", "[freertos]")
{
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreate(pie_after_cleared_blacklist, "pie_clr", 4096,
                                          (ಶೂನ್ಯ *)unity_task_handle, UNITY_FREERTOS_PRIORITY + 1, ರಿಕ್ತ));
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    vTaskDelay(10);
}

ಸ್ಥಿರ ಶೂನ್ಯ pie_blacklist_then_block(ಶೂನ್ಯ *arg)
{
    vTaskSetPieBlacklisted(ರಿಕ್ತ, pdTRUE);
    xTaskNotifyGive((TaskHandle_t)arg);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    vTaskDelete(ರಿಕ್ತ);
}

ಸ್ಥಿರ ಶೂನ್ಯ pie_notify_then_block(ಶೂನ್ಯ *arg)
{
    xTaskNotifyGive((TaskHandle_t)arg);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    vTaskDelete(ರಿಕ್ತ);
}

/*
 * Both helpers block in ulTaskNotifyTake after notifying the parent. The parent
 * never notifies them back, so vTaskDelete(NULL) in the helpers does not run.
 * vTaskDelete() here is what deletes them and runs the blacklist cleanup hook.
 */
TEST_CASE("PIE: Deleted task is removed from blacklist", "[freertos]")
{
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    TaskHandle_t child = ರಿಕ್ತ;
    TaskHandle_t child2 = ರಿಕ್ತ;

    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreate(pie_blacklist_then_block, "pie_del", 4096,
                                          (ಶೂನ್ಯ *)unity_task_handle, UNITY_FREERTOS_PRIORITY + 1, &child));
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetPieBlacklisted(child));
    vTaskDelete(child);
    vTaskDelay(10);
    TEST_ASSERT_EQUAL(pdFALSE, xTaskGetPieBlacklisted(child));

    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreate(pie_notify_then_block, "pie_del2", 4096,
                                          (ಶೂನ್ಯ *)unity_task_handle, UNITY_FREERTOS_PRIORITY + 1, &child2));
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    TEST_ASSERT_EQUAL(pdFALSE, xTaskGetPieBlacklisted(child2));
    vTaskDelete(child2);
    vTaskDelay(10);
}

ಸ್ಥಿರ ಶೂನ್ಯ pie_blacklisted_task_executes_pie(ಶೂನ್ಯ)
{
    int32_t a[4] = { 0, 1, 2, 3 };
    int32_t b[4] = { 1, 2, 3, 4 };
    int32_t dst[4] = { 0 };

    vTaskSetPieBlacklisted(ರಿಕ್ತ, pdTRUE);
    pie_vector_signed_add(a, b, dst);
    TEST_FAIL_MESSAGE("blacklisted task executed PIE without rebooting");
}

ಸ್ಥಿರ ಶೂನ್ಯ check_pie_blacklist_panic_reset(ಶೂನ್ಯ)
{
    TEST_ASSERT_EQUAL(ESP_RST_PANIC, esp_reset_reason());
}

TEST_CASE_MULTIPLE_STAGES("PIE: Blacklisted task aborts on PIE use",
                          "[freertos][reset=SW_CPU_RESET]",
                          pie_blacklisted_task_executes_pie,
                          check_pie_blacklist_panic_reset);

ಸ್ಥಿರ ಶೂನ್ಯ pie_blacklisted_owner_after_yield(ಶೂನ್ಯ)
{
    int32_t a[4] = { 0, 1, 2, 3 };
    int32_t b[4] = { 1, 2, 3, 4 };
    int32_t dst[4] = { 0 };

    pie_vector_signed_add(a, b, dst);
    vTaskSetPieBlacklisted(ರಿಕ್ತ, pdTRUE);
    /* A same-task yield leaves PIE enabled, so the next instruction would not trap.
     * Blocking lets another task run and disables PIE. */
    vTaskDelay(1);
    pie_vector_signed_add(a, b, dst);
    TEST_FAIL_MESSAGE("blacklisted owner used PIE after a switch without rebooting");
}

TEST_CASE_MULTIPLE_STAGES("PIE: Blacklisted owner aborts after yield",
                          "[freertos][reset=SW_CPU_RESET]",
                          pie_blacklisted_owner_after_yield,
                          check_pie_blacklist_panic_reset);

#endif /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */

#endif /* SOC_CPU_HAS_PIE */
