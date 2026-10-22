/*
 * SPDX-FileCopyrightText: 2024-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <math.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "soc/soc_caps.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/semphr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/idf_additions.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity_test_runner.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "test_utils.h"

/* PIE instructions set is currently only supported in GCC compiler */
#ਜੇ SOC_CPU_HAS_PIE

/**
 * @brief Performs the signed sum of two 4-word vectors using the PIE.
 *
 * @param a First vector
 * @param b Second vector
 * @param dst Destination to store the sum
 */
ਖਾਲੀ pie_ਸਦਿਸ਼_signed_ਜੋੜੋ(ਅਚਲ int32_t a[4], ਅਚਲ int32_t b[4], int32_t dst[4]);

/**
 * @brief Performs a XOR operation on two 4-word vectors using the PIE.
 *
 * @param a First vector
 * @param b Second vector
 * @param dst Destination to store the result
 */
ਖਾਲੀ pie_ਸਦਿਸ਼_xor(ਅਚਲ int32_t a[4], ਅਚਲ int32_t b[4], int32_t dst[4]);

/* ------------------------------------------------------------------------------------------------------------------ */

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    int32_t cst;
    TaskHandle_t ਮੁੱਖ;
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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_PINNED_ਅੰਕ_TASKS       3
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_PINNED_ਅੰਕ_ITERS       5

ਸਥਿਰ ਖਾਲੀ pinned_task(ਖਾਲੀ *arg)
{
    pie_params_t *param = (pie_params_t*) arg;
    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);

    int32_t constant = 42 * param->cst;
    int32_t a[4] = { constant, constant, constant, constant };
    int32_t b[4] = { 10, 20, 30, 40 };
    int32_t dst[4] = { 0 };

    pie_ਸਦਿਸ਼_signed_ਜੋੜੋ(a, b, dst);

    ਲਈ (ਪੂਰਨ i = 0; i < ਆਕਾਰ(a) / ਆਕਾਰ(uint32_t); i++) {
        TEST_ASSERT_EQUAL(dst[i], a[i] + b[i]);
    }

    // Indicate done and wait to be deleted
    xSemaphoreGive((SemaphoreHandle_t)param->sem);
    vTaskSuspend(ਨਿਰਾ);
}

TEST_CASE("PIE: Usage in task", "[freertos]")
{
    SemaphoreHandle_t ਪੂਰਾ_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES * TEST_PINNED_ਅੰਕ_TASKS, 0);
    TEST_ASSERT_NOT_EQUAL(ਨਿਰਾ, ਪੂਰਾ_sem);

    ਲਈ (ਪੂਰਨ iter = 0; iter < TEST_PINNED_ਅੰਕ_ITERS; iter++) {
        TaskHandle_t task_handles[CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES][TEST_PINNED_ਅੰਕ_TASKS];
        pie_params_t params[CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES][TEST_PINNED_ਅੰਕ_TASKS];

        // Create test tasks for each core
        ਲਈ (ਪੂਰਨ i = 0; i < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; i++) {
            ਲਈ (ਪੂਰਨ j = 0; j < TEST_PINNED_ਅੰਕ_TASKS; j++) {
                params[i][j] = (pie_params_t) {
                    .cst = i + j + 1,
                    .sem = ਪੂਰਾ_sem,
                };
                TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(pinned_task, "task", 4096, (ਖਾਲੀ *) &params[i][j], UNITY_FREERTOS_ਤਰਜੀਹ + 1, &task_handles[i][j], i));
            }
        }

        // Start the created tasks
        ਲਈ (ਪੂਰਨ i = 0; i < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; i++) {
            ਲਈ (ਪੂਰਨ j = 0; j < TEST_PINNED_ਅੰਕ_TASKS; j++) {
                xTaskNotifyGive(task_handles[i][j]);
            }
        }

        // Wait for the tasks to complete
        ਲਈ (ਪੂਰਨ i = 0; i < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES * TEST_PINNED_ਅੰਕ_TASKS; i++) {
            xSemaphoreTake(ਪੂਰਾ_sem, ਪੋਰਟ_MAX_DELAY);
        }

        // Delete the tasks
        ਲਈ (ਪੂਰਨ i = 0; i < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; i++) {
            ਲਈ (ਪੂਰਨ j = 0; j < TEST_PINNED_ਅੰਕ_TASKS; j++) {
                vTaskDelete(task_handles[i][j]);
            }
        }

        vTaskDelay(10); // Short delay to allow idle task to be free task memory and FPU contexts
    }

    vSemaphoreDelete(ਪੂਰਾ_sem);
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

#ਜੇ CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES > 1

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_UNPINNED_ਅੰਕ_ITERS     5

ਸਥਿਰ ਖਾਲੀ check_core_affinity(ਖਾਲੀ)
{
#ਜੇ CONFIG_IDF_ਨਿਸ਼ਾਨਾ_ESP32_S31
    /* On the ESP32-S31, only core 1 has PIE support, so all tasks should be pinned to core 1 */
    ਅਧਾਰ_Type_t core_ਅੰਕ = 1;
#ਨਹੀਂਤਾਂ
    ਅਧਾਰ_Type_t core_ਅੰਕ = xTaskGetCoreID(ਨਿਰਾ);
#ਜੇ_ਅੰਤ

    TEST_ASSERT_EQUAL(core_ਅੰਕ, xTaskGetCoreID(ਨਿਰਾ));
}

ਸਥਿਰ ਖਾਲੀ unpinned_task(ਖਾਲੀ *arg)
{
    // Check that the task is unpinned
#ਜੇ !CONFIG_FREERTOS_UNICORE
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, xTaskGetCoreID(ਨਿਰਾ));
#ਜੇ_ਅੰਤ // !CONFIG_FREERTOS_UNICORE

    int32_t a[4] = { 0, 1, 2, 3};
    int32_t b[4] = { 111, 222, 333, 444 };
    int32_t dst[4] = { 0 };

    pie_ਸਦਿਸ਼_signed_ਜੋੜੋ(a, b, dst);

    ਲਈ (ਪੂਰਨ i = 0; i < ਆਕਾਰ(a) / ਆਕਾਰ(uint32_t); i++) {
        TEST_ASSERT_EQUAL(dst[i], a[i] + b[i]);
    }

#ਜੇ !CONFIG_FREERTOS_UNICORE
    check_core_affinity();
#ਜੇ_ਅੰਤ // !CONFIG_FREERTOS_UNICORE

    // Indicate done and self delete
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(ਨਿਰਾ);
}

TEST_CASE("PIE: Usage in unpinned task", "[freertos]")
{
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    ਲਈ (ਪੂਰਨ iter = 0; iter < TEST_UNPINNED_ਅੰਕ_ITERS; iter++) {
        // Create unpinned task
        xTaskCreate(unpinned_task, "unpin", 4096, (ਖਾਲੀ *)unity_task_handle, UNITY_FREERTOS_ਤਰਜੀਹ + 1, ਨਿਰਾ);
        // Wait for task to complete
        ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);
        vTaskDelay(10); // Short delay to allow task memory to be freed
    }
}

/**
 * @brief Function performing some simple calculation using the PIE coprocessor.
 *        The goal is to be preempted by a task that also uses the PIE on the same core.
 */
ਸਥਿਰ ਖਾਲੀ pie_calculation(ਖਾਲੀ* arg)
{
    pie_params_t* p = (pie_params_t*) arg;
    ਅਚਲ int32_t cst = p->cst;
    int32_t a[4] = { cst, cst, cst, cst };
    int32_t dst[4] = { 0 };

    ਲਈ (ਪੂਰਨ i = 0; i < 10; i++) {
        pie_ਸਦਿਸ਼_signed_ਜੋੜੋ(a, dst, dst);

        /* Give some time to the other to interrupt us before checking `f` value */
        esp_rom_delay_us(1000);

        /* Using TEST_ASSERT_TRUE triggers a stack overflow, make sure the sign is still correct */
        ਪ੍ਰਤਿਜ੍ਞਾ((dst[0] < 0 && cst < 0) || (dst[0] > 0 && cst > 0));

        /* Give the hand back to FreeRTOS to avoid any watchdog error */
        vTaskDelay(2);
    }

    /* Make sure the result is correct */
    ਪ੍ਰਤਿਜ੍ਞਾ((dst[0] * cst == 10));

    xTaskNotifyGive(p->ਮੁੱਖ);
    vTaskDelete(ਨਿਰਾ);
}

TEST_CASE("PIE: Unsolicited context switch between tasks using the PIE", "[freertos]")
{
    /* Create two tasks that are on the same core and use the same FPU */
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    TaskHandle_t tasks[2];
    pie_params_t params[2] = {
        { .cst =  1, .ਮੁੱਖ = unity_task_handle },
        { .cst = -1, .ਮੁੱਖ = unity_task_handle },
    };

    x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(pie_calculation, "Task1", 2048, params + 0, UNITY_FREERTOS_ਤਰਜੀਹ + 1, &tasks[0], 1);
    x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(pie_calculation, "Task2", 2048, params + 1, UNITY_FREERTOS_ਤਰਜੀਹ + 1, &tasks[1], 1);

    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);
    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);
}

#ਜੇ CONFIG_IDF_ਨਿਸ਼ਾਨਾ_ESP32_S31

ਸਥਿਰ ਖਾਲੀ core0_fpu_pie(ਖਾਲੀ *arg)
{
    /* Use the FPU in this task and make sure we are pinned to core 0 */
    ਦਸ਼ਮਲਵ f = 1.0f;
    ਲਈ (ਪੂਰਨ i = 0; i < 10; i++) {
        f = f * 2.0f;
    }
    TEST_ASSERT_EQUAL(1024.0f, f);

    /* Check that the task is pinned to core 0 */
    TEST_ASSERT_EQUAL(0, xTaskGetCoreID(ਨਿਰਾ));

    /* Use the PIE in this task and make sure we are migrated to core 1 */
    int32_t a[4] = { 0, 1, 2, 3};
    int32_t b[4] = { 111, 222, 333, 444 };
    int32_t dst[4] = { 0 };

    pie_ਸਦਿਸ਼_signed_ਜੋੜੋ(a, b, dst);

    TEST_ASSERT_EQUAL(1, xTaskGetCoreID(ਨਿਰਾ));

    ਲਈ (ਪੂਰਨ i = 0; i < ਆਕਾਰ(a) / ਆਕਾਰ(uint32_t); i++) {
        TEST_ASSERT_EQUAL(dst[i], (a[i] + b[i]));
    }

    /* Try to use the FPU again on the new core */
    f = f * 2.0f;
    TEST_ASSERT_EQUAL(2048.0f, f);

    /* Indicate done and self delete */
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(ਨਿਰਾ);
}

TEST_CASE("PIE: Usage in a task that owns the FPU on core 0", "[freertos]")
{
    /* Make sure that a task that owns the FPU on core 0 can be migrated to core 1 when using the PIE */
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(core0_fpu_pie, "core0_fpu_pie", 4096,
                            (ਖਾਲੀ*) unity_task_handle, UNITY_FREERTOS_ਤਰਜੀਹ + 1, ਨਿਰਾ, 0);
    // Wait for task to complete
    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PIE_INSTR_ਗਿਣਤੀ 2

ਸਥਿਰ ਖਾਲੀ core0_pie_instr_task(ਖਾਲੀ *arg)
{
    pie_params_t *param = (pie_params_t*) arg;

    ਚੋਣ (param->cst) {
    ਹਾਲਤ 0: {
        int32_t a = 1;
        int32_t b = 2;
        int32_t dst = 0;

        /* Opcode of type 0b0110011 */
        __asm__ ਅਸਥਿਰ("esp.addx2 %0, %1, %2" : "=r"(dst) : "r"(a), "r"(b));
        ਰੋਕੋ;
    }
    ਹਾਲਤ 1: {
        int32_t a[4] = { 0xffffff, 0xaaaaaaaa, 0x11111111, 0xffffffff };
        int32_t b[4] = { 0x11111111, 0xaaaaaaaa, 0xffffff, 0xffffffff };
        int32_t dst[4] = { 0 };
        /* Opcode of type 0bxx11x11 (x: don't care) */
        pie_ਸਦਿਸ਼_xor(a, b, dst);
        ਰੋਕੋ;
    }
    }
    /* Indicate done and wait for deletion */
    xTaskNotifyGive(param->ਮੁੱਖ);
    vTaskDelay(ਪੋਰਟ_MAX_DELAY);
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
    param.ਮੁੱਖ = xTaskGetCurrentTaskHandle();

    /* Create tasks that will run on core 0 only. Each of them will execute one PIE instruction.
     * If they don't crash, it's a success.*/
    ਲਈ (ਪੂਰਨ i = 0; i < PIE_INSTR_ਗਿਣਤੀ; i++) {
        param.cst = i;
        x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(core0_pie_instr_task, "Core0PIE", 2048, &param, UNITY_FREERTOS_ਤਰਜੀਹ + 1, &task_handle, 0);
        /* Wait for the task to complete */
        ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);
        /* Delete the task and start again */
        vTaskDelete(task_handle);
    }
}
#ਜੇ_ਅੰਤ /* CONFIG_IDF_TARGET_ESP32S31 */

#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_NUMBER_OF_CORES > 1 */

#ਜੇ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST

TEST_CASE("PIE: Blacklist setter and getter", "[freertos]")
{
    ਅਧਾਰ_Type_t before = xTaskGetPieBlacklisted(ਨਿਰਾ);
    v_Task_ਸੈੱਟ_Pie_Blacklisted(ਨਿਰਾ, pdTRUE);
    ਅਧਾਰ_Type_t ਸੈੱਟ = xTaskGetPieBlacklisted(ਨਿਰਾ);
    v_Task_ਸੈੱਟ_Pie_Blacklisted(ਨਿਰਾ, pdFALSE);
    ਅਧਾਰ_Type_t cleared = xTaskGetPieBlacklisted(ਨਿਰਾ);

    TEST_ASSERT_EQUAL(pdFALSE, before);
    TEST_ASSERT_EQUAL(pdTRUE, ਸੈੱਟ);
    TEST_ASSERT_EQUAL(pdFALSE, cleared);
}

ਸਥਿਰ ਖਾਲੀ pie_after_cleared_blacklist(ਖਾਲੀ *arg)
{
    int32_t a[4] = { 0, 1, 2, 3 };
    int32_t b[4] = { 1, 2, 3, 4 };
    int32_t dst[4] = { 0 };

    v_Task_ਸੈੱਟ_Pie_Blacklisted(ਨਿਰਾ, pdTRUE);
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetPieBlacklisted(ਨਿਰਾ));
    v_Task_ਸੈੱਟ_Pie_Blacklisted(ਨਿਰਾ, pdFALSE);
    pie_ਸਦਿਸ਼_signed_ਜੋੜੋ(a, b, dst);

    ਲਈ (ਪੂਰਨ i = 0; i < 4; i++) {
        TEST_ASSERT_EQUAL(a[i] + b[i], dst[i]);
    }

    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(ਨਿਰਾ);
}

TEST_CASE("PIE: Cleared blacklist allows PIE use", "[freertos]")
{
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreate(pie_after_cleared_blacklist, "pie_clr", 4096,
                                          (ਖਾਲੀ *)unity_task_handle, UNITY_FREERTOS_ਤਰਜੀਹ + 1, ਨਿਰਾ));
    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);
    vTaskDelay(10);
}

ਸਥਿਰ ਖਾਲੀ pie_blacklist_then_ਖੰਡ(ਖਾਲੀ *arg)
{
    v_Task_ਸੈੱਟ_Pie_Blacklisted(ਨਿਰਾ, pdTRUE);
    xTaskNotifyGive((TaskHandle_t)arg);
    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);
    vTaskDelete(ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ pie_notify_then_ਖੰਡ(ਖਾਲੀ *arg)
{
    xTaskNotifyGive((TaskHandle_t)arg);
    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);
    vTaskDelete(ਨਿਰਾ);
}

/*
 * Both helpers block in ulTaskNotifyTake after notifying the parent. The parent
 * never notifies them back, so vTaskDelete(NULL) in the helpers does not run.
 * vTaskDelete() here is what deletes them and runs the blacklist cleanup hook.
 */
TEST_CASE("PIE: Deleted task is removed from blacklist", "[freertos]")
{
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    TaskHandle_t ਬੱਚਾ = ਨਿਰਾ;
    TaskHandle_t child2 = ਨਿਰਾ;

    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreate(pie_blacklist_then_ਖੰਡ, "pie_del", 4096,
                                          (ਖਾਲੀ *)unity_task_handle, UNITY_FREERTOS_ਤਰਜੀਹ + 1, &ਬੱਚਾ));
    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetPieBlacklisted(ਬੱਚਾ));
    vTaskDelete(ਬੱਚਾ);
    vTaskDelay(10);
    TEST_ASSERT_EQUAL(pdFALSE, xTaskGetPieBlacklisted(ਬੱਚਾ));

    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreate(pie_notify_then_ਖੰਡ, "pie_del2", 4096,
                                          (ਖਾਲੀ *)unity_task_handle, UNITY_FREERTOS_ਤਰਜੀਹ + 1, &child2));
    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);
    TEST_ASSERT_EQUAL(pdFALSE, xTaskGetPieBlacklisted(child2));
    vTaskDelete(child2);
    vTaskDelay(10);
}

ਸਥਿਰ ਖਾਲੀ pie_blacklisted_task_executes_pie(ਖਾਲੀ)
{
    int32_t a[4] = { 0, 1, 2, 3 };
    int32_t b[4] = { 1, 2, 3, 4 };
    int32_t dst[4] = { 0 };

    v_Task_ਸੈੱਟ_Pie_Blacklisted(ਨਿਰਾ, pdTRUE);
    pie_ਸਦਿਸ਼_signed_ਜੋੜੋ(a, b, dst);
    TEST_FAIL_ਸੁਨੇਹਾ("blacklisted task executed PIE without rebooting");
}

ਸਥਿਰ ਖਾਲੀ check_pie_blacklist_panic_reset(ਖਾਲੀ)
{
    TEST_ASSERT_EQUAL(ESP_RST_PANIC, esp_reset_reason());
}

TEST_CASE_ਗੁਣਜ_STAGES("PIE: Blacklisted task aborts on PIE use",
                          "[freertos][reset=SW_CPU_RESET]",
                          pie_blacklisted_task_executes_pie,
                          check_pie_blacklist_panic_reset);

ਸਥਿਰ ਖਾਲੀ pie_blacklisted_owner_after_yield(ਖਾਲੀ)
{
    int32_t a[4] = { 0, 1, 2, 3 };
    int32_t b[4] = { 1, 2, 3, 4 };
    int32_t dst[4] = { 0 };

    pie_ਸਦਿਸ਼_signed_ਜੋੜੋ(a, b, dst);
    v_Task_ਸੈੱਟ_Pie_Blacklisted(ਨਿਰਾ, pdTRUE);
    /* A same-task yield leaves PIE enabled, so the next instruction would not trap.
     * Blocking lets another task run and disables PIE. */
    vTaskDelay(1);
    pie_ਸਦਿਸ਼_signed_ਜੋੜੋ(a, b, dst);
    TEST_FAIL_ਸੁਨੇਹਾ("blacklisted owner used PIE after a switch without rebooting");
}

TEST_CASE_ਗੁਣਜ_STAGES("PIE: Blacklisted owner aborts after yield",
                          "[freertos][reset=SW_CPU_RESET]",
                          pie_blacklisted_owner_after_yield,
                          check_pie_blacklist_panic_reset);

#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */

#ਜੇ_ਅੰਤ /* SOC_CPU_HAS_PIE */
