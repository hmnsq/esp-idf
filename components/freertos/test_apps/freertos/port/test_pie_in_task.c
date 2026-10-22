/*
 * SPDX-FileCopyrightText: 2024-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#അന്തര്ഭാവ "sdkconfig.h"
#അന്തര്ഭാവ <math.h>
#അന്തര്ഭാവ "soc/soc_caps.h"
#അന്തര്ഭാവ "freertos/FreeRTOS.h"
#അന്തര്ഭാവ "freertos/task.h"
#അന്തര്ഭാവ "freertos/semphr.h"
#അന്തര്ഭാവ "freertos/idf_additions.h"
#അന്തര്ഭാവ "unity.h"
#അന്തര്ഭാവ "unity_test_runner.h"
#അന്തര്ഭാവ "esp_system.h"
#അന്തര്ഭാവ "test_utils.h"

/* PIE instructions set is currently only supported in GCC compiler */
#എങ്കിൽ SOC_CPU_HAS_PIE

/**
 * @brief Performs the signed sum of two 4-word vectors using the PIE.
 *
 * @param a First vector
 * @param b Second vector
 * @param dst Destination to store the sum
 */
ശൂന്യം pie_vector_signed_add(സ്ഥിരം int32_t a[4], സ്ഥിരം int32_t b[4], int32_t dst[4]);

/**
 * @brief Performs a XOR operation on two 4-word vectors using the PIE.
 *
 * @param a First vector
 * @param b Second vector
 * @param dst Destination to store the result
 */
ശൂന്യം pie_vector_xor(സ്ഥിരം int32_t a[4], സ്ഥിരം int32_t b[4], int32_t dst[4]);

/* ------------------------------------------------------------------------------------------------------------------ */

തരനിർവചനം ഘടന {
    int32_t cst;
    TaskHandle_t പ്രധാന;
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

#define TEST_PINNED_അക്കം_TASKS       3
#define TEST_PINNED_അക്കം_ITERS       5

സ്ഥിതം ശൂന്യം pinned_task(ശൂന്യം *arg)
{
    pie_params_t *param = (pie_params_t*) arg;
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    int32_t constant = 42 * param->cst;
    int32_t a[4] = { constant, constant, constant, constant };
    int32_t b[4] = { 10, 20, 30, 40 };
    int32_t dst[4] = { 0 };

    pie_vector_signed_add(a, b, dst);

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < വലുപ്പം(a) / വലുപ്പം(uint32_t); i++) {
        TEST_ASSERT_EQUAL(dst[i], a[i] + b[i]);
    }

    // Indicate done and wait to be deleted
    xSemaphoreGive((SemaphoreHandle_t)param->sem);
    vTaskSuspend(ശൂന്യ);
}

TEST_CASE("PIE: Usage in task", "[freertos]")
{
    SemaphoreHandle_t done_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_സംഖ്യ_OF_CORES * TEST_PINNED_അക്കം_TASKS, 0);
    TEST_ASSERT_NOT_EQUAL(ശൂന്യ, done_sem);

    വേണ്ടി (പൂർണ്ണസംഖ്യ iter = 0; iter < TEST_PINNED_അക്കം_ITERS; iter++) {
        TaskHandle_t task_handles[CONFIG_FREERTOS_സംഖ്യ_OF_CORES][TEST_PINNED_അക്കം_TASKS];
        pie_params_t params[CONFIG_FREERTOS_സംഖ്യ_OF_CORES][TEST_PINNED_അക്കം_TASKS];

        // Create test tasks for each core
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; i++) {
            വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < TEST_PINNED_അക്കം_TASKS; j++) {
                params[i][j] = (pie_params_t) {
                    .cst = i + j + 1,
                    .sem = done_sem,
                };
                TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(pinned_task, "task", 4096, (ശൂന്യം *) &params[i][j], UNITY_FREERTOS_PRIORITY + 1, &task_handles[i][j], i));
            }
        }

        // Start the created tasks
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; i++) {
            വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < TEST_PINNED_അക്കം_TASKS; j++) {
                xTaskNotifyGive(task_handles[i][j]);
            }
        }

        // Wait for the tasks to complete
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < CONFIG_FREERTOS_സംഖ്യ_OF_CORES * TEST_PINNED_അക്കം_TASKS; i++) {
            xSemaphoreTake(done_sem, portMAX_DELAY);
        }

        // Delete the tasks
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; i++) {
            വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < TEST_PINNED_അക്കം_TASKS; j++) {
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

#എങ്കിൽ CONFIG_FREERTOS_സംഖ്യ_OF_CORES > 1

#define TEST_UNPINNED_അക്കം_ITERS     5

സ്ഥിതം ശൂന്യം check_core_affinity(ശൂന്യം)
{
#എങ്കിൽ CONFIG_IDF_TARGET_ESP32S31
    /* On the ESP32-S31, only core 1 has PIE support, so all tasks should be pinned to core 1 */
    BaseType_t core_അക്കം = 1;
#അല്ലെങ്കിൽ
    BaseType_t core_അക്കം = xTaskGetCoreID(ശൂന്യ);
#endif

    TEST_ASSERT_EQUAL(core_അക്കം, xTaskGetCoreID(ശൂന്യ));
}

സ്ഥിതം ശൂന്യം unpinned_task(ശൂന്യം *arg)
{
    // Check that the task is unpinned
#എങ്കിൽ !CONFIG_FREERTOS_UNICORE
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, xTaskGetCoreID(ശൂന്യ));
#endif // !CONFIG_FREERTOS_UNICORE

    int32_t a[4] = { 0, 1, 2, 3};
    int32_t b[4] = { 111, 222, 333, 444 };
    int32_t dst[4] = { 0 };

    pie_vector_signed_add(a, b, dst);

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < വലുപ്പം(a) / വലുപ്പം(uint32_t); i++) {
        TEST_ASSERT_EQUAL(dst[i], a[i] + b[i]);
    }

#എങ്കിൽ !CONFIG_FREERTOS_UNICORE
    check_core_affinity();
#endif // !CONFIG_FREERTOS_UNICORE

    // Indicate done and self delete
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(ശൂന്യ);
}

TEST_CASE("PIE: Usage in unpinned task", "[freertos]")
{
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    വേണ്ടി (പൂർണ്ണസംഖ്യ iter = 0; iter < TEST_UNPINNED_അക്കം_ITERS; iter++) {
        // Create unpinned task
        xTaskCreate(unpinned_task, "unpin", 4096, (ശൂന്യം *)unity_task_handle, UNITY_FREERTOS_PRIORITY + 1, ശൂന്യ);
        // Wait for task to complete
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        vTaskDelay(10); // Short delay to allow task memory to be freed
    }
}

/**
 * @brief Function performing some simple calculation using the PIE coprocessor.
 *        The goal is to be preempted by a task that also uses the PIE on the same core.
 */
സ്ഥിതം ശൂന്യം pie_calculation(ശൂന്യം* arg)
{
    pie_params_t* p = (pie_params_t*) arg;
    സ്ഥിരം int32_t cst = p->cst;
    int32_t a[4] = { cst, cst, cst, cst };
    int32_t dst[4] = { 0 };

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 10; i++) {
        pie_vector_signed_add(a, dst, dst);

        /* Give some time to the other to interrupt us before checking `f` value */
        esp_rom_delay_us(1000);

        /* Using TEST_ASSERT_TRUE triggers a stack overflow, make sure the sign is still correct */
        പ്രതിജ്ഞാ((dst[0] < 0 && cst < 0) || (dst[0] > 0 && cst > 0));

        /* Give the hand back to FreeRTOS to avoid any watchdog error */
        vTaskDelay(2);
    }

    /* Make sure the result is correct */
    പ്രതിജ്ഞാ((dst[0] * cst == 10));

    xTaskNotifyGive(p->പ്രധാന);
    vTaskDelete(ശൂന്യ);
}

TEST_CASE("PIE: Unsolicited context switch between tasks using the PIE", "[freertos]")
{
    /* Create two tasks that are on the same core and use the same FPU */
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    TaskHandle_t tasks[2];
    pie_params_t params[2] = {
        { .cst =  1, .പ്രധാന = unity_task_handle },
        { .cst = -1, .പ്രധാന = unity_task_handle },
    };

    xTaskCreatePinnedToCore(pie_calculation, "Task1", 2048, params + 0, UNITY_FREERTOS_PRIORITY + 1, &tasks[0], 1);
    xTaskCreatePinnedToCore(pie_calculation, "Task2", 2048, params + 1, UNITY_FREERTOS_PRIORITY + 1, &tasks[1], 1);

    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
}

#എങ്കിൽ CONFIG_IDF_TARGET_ESP32S31

സ്ഥിതം ശൂന്യം core0_fpu_pie(ശൂന്യം *arg)
{
    /* Use the FPU in this task and make sure we are pinned to core 0 */
    ദശാംശം f = 1.0f;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 10; i++) {
        f = f * 2.0f;
    }
    TEST_ASSERT_EQUAL(1024.0f, f);

    /* Check that the task is pinned to core 0 */
    TEST_ASSERT_EQUAL(0, xTaskGetCoreID(ശൂന്യ));

    /* Use the PIE in this task and make sure we are migrated to core 1 */
    int32_t a[4] = { 0, 1, 2, 3};
    int32_t b[4] = { 111, 222, 333, 444 };
    int32_t dst[4] = { 0 };

    pie_vector_signed_add(a, b, dst);

    TEST_ASSERT_EQUAL(1, xTaskGetCoreID(ശൂന്യ));

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < വലുപ്പം(a) / വലുപ്പം(uint32_t); i++) {
        TEST_ASSERT_EQUAL(dst[i], (a[i] + b[i]));
    }

    /* Try to use the FPU again on the new core */
    f = f * 2.0f;
    TEST_ASSERT_EQUAL(2048.0f, f);

    /* Indicate done and self delete */
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(ശൂന്യ);
}

TEST_CASE("PIE: Usage in a task that owns the FPU on core 0", "[freertos]")
{
    /* Make sure that a task that owns the FPU on core 0 can be migrated to core 1 when using the PIE */
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    xTaskCreatePinnedToCore(core0_fpu_pie, "core0_fpu_pie", 4096,
                            (ശൂന്യം*) unity_task_handle, UNITY_FREERTOS_PRIORITY + 1, ശൂന്യ, 0);
    // Wait for task to complete
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#define PIE_INSTR_എണ്ണം 2

സ്ഥിതം ശൂന്യം core0_pie_instr_task(ശൂന്യം *arg)
{
    pie_params_t *param = (pie_params_t*) arg;

    തിരഞ്ഞെടുപ്പ് (param->cst) {
    അവസ്ഥ 0: {
        int32_t a = 1;
        int32_t b = 2;
        int32_t dst = 0;

        /* Opcode of type 0b0110011 */
        __asm__ അസ്ഥിര("esp.addx2 %0, %1, %2" : "=r"(dst) : "r"(a), "r"(b));
        നിർത്തുക;
    }
    അവസ്ഥ 1: {
        int32_t a[4] = { 0xffffff, 0xaaaaaaaa, 0x11111111, 0xffffffff };
        int32_t b[4] = { 0x11111111, 0xaaaaaaaa, 0xffffff, 0xffffffff };
        int32_t dst[4] = { 0 };
        /* Opcode of type 0bxx11x11 (x: don't care) */
        pie_vector_xor(a, b, dst);
        നിർത്തുക;
    }
    }
    /* Indicate done and wait for deletion */
    xTaskNotifyGive(param->പ്രധാന);
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
    param.പ്രധാന = xTaskGetCurrentTaskHandle();

    /* Create tasks that will run on core 0 only. Each of them will execute one PIE instruction.
     * If they don't crash, it's a success.*/
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < PIE_INSTR_എണ്ണം; i++) {
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

#എങ്കിൽ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST

TEST_CASE("PIE: Blacklist setter and getter", "[freertos]")
{
    BaseType_t before = xTaskGetPieBlacklisted(ശൂന്യ);
    vTaskSetPieBlacklisted(ശൂന്യ, pdTRUE);
    BaseType_t set = xTaskGetPieBlacklisted(ശൂന്യ);
    vTaskSetPieBlacklisted(ശൂന്യ, pdFALSE);
    BaseType_t cleared = xTaskGetPieBlacklisted(ശൂന്യ);

    TEST_ASSERT_EQUAL(pdFALSE, before);
    TEST_ASSERT_EQUAL(pdTRUE, set);
    TEST_ASSERT_EQUAL(pdFALSE, cleared);
}

സ്ഥിതം ശൂന്യം pie_after_cleared_blacklist(ശൂന്യം *arg)
{
    int32_t a[4] = { 0, 1, 2, 3 };
    int32_t b[4] = { 1, 2, 3, 4 };
    int32_t dst[4] = { 0 };

    vTaskSetPieBlacklisted(ശൂന്യ, pdTRUE);
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetPieBlacklisted(ശൂന്യ));
    vTaskSetPieBlacklisted(ശൂന്യ, pdFALSE);
    pie_vector_signed_add(a, b, dst);

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 4; i++) {
        TEST_ASSERT_EQUAL(a[i] + b[i], dst[i]);
    }

    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(ശൂന്യ);
}

TEST_CASE("PIE: Cleared blacklist allows PIE use", "[freertos]")
{
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreate(pie_after_cleared_blacklist, "pie_clr", 4096,
                                          (ശൂന്യം *)unity_task_handle, UNITY_FREERTOS_PRIORITY + 1, ശൂന്യ));
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    vTaskDelay(10);
}

സ്ഥിതം ശൂന്യം pie_blacklist_then_block(ശൂന്യം *arg)
{
    vTaskSetPieBlacklisted(ശൂന്യ, pdTRUE);
    xTaskNotifyGive((TaskHandle_t)arg);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    vTaskDelete(ശൂന്യ);
}

സ്ഥിതം ശൂന്യം pie_notify_then_block(ശൂന്യം *arg)
{
    xTaskNotifyGive((TaskHandle_t)arg);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    vTaskDelete(ശൂന്യ);
}

/*
 * Both helpers block in ulTaskNotifyTake after notifying the parent. The parent
 * never notifies them back, so vTaskDelete(NULL) in the helpers does not run.
 * vTaskDelete() here is what deletes them and runs the blacklist cleanup hook.
 */
TEST_CASE("PIE: Deleted task is removed from blacklist", "[freertos]")
{
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    TaskHandle_t child = ശൂന്യ;
    TaskHandle_t child2 = ശൂന്യ;

    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreate(pie_blacklist_then_block, "pie_del", 4096,
                                          (ശൂന്യം *)unity_task_handle, UNITY_FREERTOS_PRIORITY + 1, &child));
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetPieBlacklisted(child));
    vTaskDelete(child);
    vTaskDelay(10);
    TEST_ASSERT_EQUAL(pdFALSE, xTaskGetPieBlacklisted(child));

    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreate(pie_notify_then_block, "pie_del2", 4096,
                                          (ശൂന്യം *)unity_task_handle, UNITY_FREERTOS_PRIORITY + 1, &child2));
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    TEST_ASSERT_EQUAL(pdFALSE, xTaskGetPieBlacklisted(child2));
    vTaskDelete(child2);
    vTaskDelay(10);
}

സ്ഥിതം ശൂന്യം pie_blacklisted_task_executes_pie(ശൂന്യം)
{
    int32_t a[4] = { 0, 1, 2, 3 };
    int32_t b[4] = { 1, 2, 3, 4 };
    int32_t dst[4] = { 0 };

    vTaskSetPieBlacklisted(ശൂന്യ, pdTRUE);
    pie_vector_signed_add(a, b, dst);
    TEST_FAIL_MESSAGE("blacklisted task executed PIE without rebooting");
}

സ്ഥിതം ശൂന്യം check_pie_blacklist_panic_reset(ശൂന്യം)
{
    TEST_ASSERT_EQUAL(ESP_RST_PANIC, esp_reset_reason());
}

TEST_CASE_MULTIPLE_STAGES("PIE: Blacklisted task aborts on PIE use",
                          "[freertos][reset=SW_CPU_RESET]",
                          pie_blacklisted_task_executes_pie,
                          check_pie_blacklist_panic_reset);

സ്ഥിതം ശൂന്യം pie_blacklisted_owner_after_yield(ശൂന്യം)
{
    int32_t a[4] = { 0, 1, 2, 3 };
    int32_t b[4] = { 1, 2, 3, 4 };
    int32_t dst[4] = { 0 };

    pie_vector_signed_add(a, b, dst);
    vTaskSetPieBlacklisted(ശൂന്യ, pdTRUE);
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
