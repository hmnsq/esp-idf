/*
 * SPDX-FileCopyrightText: 2024-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "sdkconfig.h"
#ضمن <math.h>
#ضمن "soc/soc_caps.h"
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "freertos/semphr.h"
#ضمن "freertos/idf_additions.h"
#ضمن "unity.h"
#ضمن "unity_test_runner.h"
#ضمن "esp_system.h"
#ضمن "test_utils.h"

/* PIE instructions set is currently only supported in GCC compiler */
#إذا SOC_CPU_HAS_PIE

/**
 * @brief Performs the signed sum of two 4-word vectors using the PIE.
 *
 * @param a First vector
 * @param b Second vector
 * @param dst Destination to store the sum
 */
فراغ pie_ویکټور_signed_جمع_کړه(ثابت int32_t a[4], ثابت int32_t b[4], int32_t dst[4]);

/**
 * @brief Performs a XOR operation on two 4-word vectors using the PIE.
 *
 * @param a First vector
 * @param b Second vector
 * @param dst Destination to store the result
 */
فراغ pie_ویکټور_xor(ثابت int32_t a[4], ثابت int32_t b[4], int32_t dst[4]);

/* ------------------------------------------------------------------------------------------------------------------ */

تعريفنوع بنية {
    int32_t cst;
    TaskHandle_t اصلي;
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

#تعریف_کړه TEST_PINNED_عدد_TASKS       3
#تعریف_کړه TEST_PINNED_عدد_ITERS       5

ساكن فراغ pinned_task(فراغ *arg)
{
    pie_params_t *param = (pie_params_t*) arg;
    ulTaskNotifyTake(pdTRUE, پورټ_MAX_DELAY);

    int32_t constant = 42 * param->cst;
    int32_t a[4] = { constant, constant, constant, constant };
    int32_t b[4] = { 10, 20, 30, 40 };
    int32_t dst[4] = { 0 };

    pie_ویکټور_signed_جمع_کړه(a, b, dst);

    لكل (صحيح i = 0; i < حجم(a) / حجم(uint32_t); i++) {
        TEST_ASSERT_EQUAL(dst[i], a[i] + b[i]);
    }

    // Indicate done and wait to be deleted
    xSemaphoreGive((SemaphoreHandle_t)param->sem);
    vTaskSuspend(عدم);
}

TEST_CASE("PIE: Usage in task", "[freertos]")
{
    SemaphoreHandle_t بشپړ_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_شمېره_د_CORES * TEST_PINNED_عدد_TASKS, 0);
    TEST_ASSERT_NOT_EQUAL(عدم, بشپړ_sem);

    لكل (صحيح iter = 0; iter < TEST_PINNED_عدد_ITERS; iter++) {
        TaskHandle_t task_handles[CONFIG_FREERTOS_شمېره_د_CORES][TEST_PINNED_عدد_TASKS];
        pie_params_t params[CONFIG_FREERTOS_شمېره_د_CORES][TEST_PINNED_عدد_TASKS];

        // Create test tasks for each core
        لكل (صحيح i = 0; i < CONFIG_FREERTOS_شمېره_د_CORES; i++) {
            لكل (صحيح j = 0; j < TEST_PINNED_عدد_TASKS; j++) {
                params[i][j] = (pie_params_t) {
                    .cst = i + j + 1,
                    .sem = بشپړ_sem,
                };
                TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_منزل_Core(pinned_task, "task", 4096, (فراغ *) &params[i][j], UNITY_FREERTOS_لومړیتوب + 1, &task_handles[i][j], i));
            }
        }

        // Start the created tasks
        لكل (صحيح i = 0; i < CONFIG_FREERTOS_شمېره_د_CORES; i++) {
            لكل (صحيح j = 0; j < TEST_PINNED_عدد_TASKS; j++) {
                xTaskNotifyGive(task_handles[i][j]);
            }
        }

        // Wait for the tasks to complete
        لكل (صحيح i = 0; i < CONFIG_FREERTOS_شمېره_د_CORES * TEST_PINNED_عدد_TASKS; i++) {
            xSemaphoreTake(بشپړ_sem, پورټ_MAX_DELAY);
        }

        // Delete the tasks
        لكل (صحيح i = 0; i < CONFIG_FREERTOS_شمېره_د_CORES; i++) {
            لكل (صحيح j = 0; j < TEST_PINNED_عدد_TASKS; j++) {
                vTaskDelete(task_handles[i][j]);
            }
        }

        vTaskDelay(10); // Short delay to allow idle task to be free task memory and FPU contexts
    }

    vSemaphoreDelete(بشپړ_sem);
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

#إذا CONFIG_FREERTOS_شمېره_د_CORES > 1

#تعریف_کړه TEST_UNPINNED_عدد_ITERS     5

ساكن فراغ check_core_affinity(فراغ)
{
#إذا CONFIG_IDF_موخه_ESP32_S31
    /* On the ESP32-S31, only core 1 has PIE support, so all tasks should be pinned to core 1 */
    بنسټ_Type_t core_عدد = 1;
#وإلا
    بنسټ_Type_t core_عدد = xTaskGetCoreID(عدم);
#د_که_پای

    TEST_ASSERT_EQUAL(core_عدد, xTaskGetCoreID(عدم));
}

ساكن فراغ unpinned_task(فراغ *arg)
{
    // Check that the task is unpinned
#إذا !CONFIG_FREERTOS_UNICORE
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, xTaskGetCoreID(عدم));
#د_که_پای // !CONFIG_FREERTOS_UNICORE

    int32_t a[4] = { 0, 1, 2, 3};
    int32_t b[4] = { 111, 222, 333, 444 };
    int32_t dst[4] = { 0 };

    pie_ویکټور_signed_جمع_کړه(a, b, dst);

    لكل (صحيح i = 0; i < حجم(a) / حجم(uint32_t); i++) {
        TEST_ASSERT_EQUAL(dst[i], a[i] + b[i]);
    }

#إذا !CONFIG_FREERTOS_UNICORE
    check_core_affinity();
#د_که_پای // !CONFIG_FREERTOS_UNICORE

    // Indicate done and self delete
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(عدم);
}

TEST_CASE("PIE: Usage in unpinned task", "[freertos]")
{
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    لكل (صحيح iter = 0; iter < TEST_UNPINNED_عدد_ITERS; iter++) {
        // Create unpinned task
        xTaskCreate(unpinned_task, "unpin", 4096, (فراغ *)unity_task_handle, UNITY_FREERTOS_لومړیتوب + 1, عدم);
        // Wait for task to complete
        ulTaskNotifyTake(pdTRUE, پورټ_MAX_DELAY);
        vTaskDelay(10); // Short delay to allow task memory to be freed
    }
}

/**
 * @brief Function performing some simple calculation using the PIE coprocessor.
 *        The goal is to be preempted by a task that also uses the PIE on the same core.
 */
ساكن فراغ pie_calculation(فراغ* arg)
{
    pie_params_t* p = (pie_params_t*) arg;
    ثابت int32_t cst = p->cst;
    int32_t a[4] = { cst, cst, cst, cst };
    int32_t dst[4] = { 0 };

    لكل (صحيح i = 0; i < 10; i++) {
        pie_ویکټور_signed_جمع_کړه(a, dst, dst);

        /* Give some time to the other to interrupt us before checking `f` value */
        esp_rom_delay_us(1000);

        /* Using TEST_ASSERT_TRUE triggers a stack overflow, make sure the sign is still correct */
        أكد((dst[0] < 0 && cst < 0) || (dst[0] > 0 && cst > 0));

        /* Give the hand back to FreeRTOS to avoid any watchdog error */
        vTaskDelay(2);
    }

    /* Make sure the result is correct */
    أكد((dst[0] * cst == 10));

    xTaskNotifyGive(p->اصلي);
    vTaskDelete(عدم);
}

TEST_CASE("PIE: Unsolicited context switch between tasks using the PIE", "[freertos]")
{
    /* Create two tasks that are on the same core and use the same FPU */
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    TaskHandle_t tasks[2];
    pie_params_t params[2] = {
        { .cst =  1, .اصلي = unity_task_handle },
        { .cst = -1, .اصلي = unity_task_handle },
    };

    x_Task_Create_Pinned_منزل_Core(pie_calculation, "Task1", 2048, params + 0, UNITY_FREERTOS_لومړیتوب + 1, &tasks[0], 1);
    x_Task_Create_Pinned_منزل_Core(pie_calculation, "Task2", 2048, params + 1, UNITY_FREERTOS_لومړیتوب + 1, &tasks[1], 1);

    ulTaskNotifyTake(pdTRUE, پورټ_MAX_DELAY);
    ulTaskNotifyTake(pdTRUE, پورټ_MAX_DELAY);
}

#إذا CONFIG_IDF_موخه_ESP32_S31

ساكن فراغ core0_fpu_pie(فراغ *arg)
{
    /* Use the FPU in this task and make sure we are pinned to core 0 */
    عائم f = 1.0f;
    لكل (صحيح i = 0; i < 10; i++) {
        f = f * 2.0f;
    }
    TEST_ASSERT_EQUAL(1024.0f, f);

    /* Check that the task is pinned to core 0 */
    TEST_ASSERT_EQUAL(0, xTaskGetCoreID(عدم));

    /* Use the PIE in this task and make sure we are migrated to core 1 */
    int32_t a[4] = { 0, 1, 2, 3};
    int32_t b[4] = { 111, 222, 333, 444 };
    int32_t dst[4] = { 0 };

    pie_ویکټور_signed_جمع_کړه(a, b, dst);

    TEST_ASSERT_EQUAL(1, xTaskGetCoreID(عدم));

    لكل (صحيح i = 0; i < حجم(a) / حجم(uint32_t); i++) {
        TEST_ASSERT_EQUAL(dst[i], (a[i] + b[i]));
    }

    /* Try to use the FPU again on the new core */
    f = f * 2.0f;
    TEST_ASSERT_EQUAL(2048.0f, f);

    /* Indicate done and self delete */
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(عدم);
}

TEST_CASE("PIE: Usage in a task that owns the FPU on core 0", "[freertos]")
{
    /* Make sure that a task that owns the FPU on core 0 can be migrated to core 1 when using the PIE */
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    x_Task_Create_Pinned_منزل_Core(core0_fpu_pie, "core0_fpu_pie", 4096,
                            (فراغ*) unity_task_handle, UNITY_FREERTOS_لومړیتوب + 1, عدم, 0);
    // Wait for task to complete
    ulTaskNotifyTake(pdTRUE, پورټ_MAX_DELAY);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#تعریف_کړه PIE_INSTR_شمېر 2

ساكن فراغ core0_pie_instr_task(فراغ *arg)
{
    pie_params_t *param = (pie_params_t*) arg;

    بدل (param->cst) {
    حالة 0: {
        int32_t a = 1;
        int32_t b = 2;
        int32_t dst = 0;

        /* Opcode of type 0b0110011 */
        __asm__ متطاير("esp.addx2 %0, %1, %2" : "=r"(dst) : "r"(a), "r"(b));
        اكسر;
    }
    حالة 1: {
        int32_t a[4] = { 0xffffff, 0xaaaaaaaa, 0x11111111, 0xffffffff };
        int32_t b[4] = { 0x11111111, 0xaaaaaaaa, 0xffffff, 0xffffffff };
        int32_t dst[4] = { 0 };
        /* Opcode of type 0bxx11x11 (x: don't care) */
        pie_ویکټور_xor(a, b, dst);
        اكسر;
    }
    }
    /* Indicate done and wait for deletion */
    xTaskNotifyGive(param->اصلي);
    vTaskDelay(پورټ_MAX_DELAY);
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
    param.اصلي = xTaskGetCurrentTaskHandle();

    /* Create tasks that will run on core 0 only. Each of them will execute one PIE instruction.
     * If they don't crash, it's a success.*/
    لكل (صحيح i = 0; i < PIE_INSTR_شمېر; i++) {
        param.cst = i;
        x_Task_Create_Pinned_منزل_Core(core0_pie_instr_task, "Core0PIE", 2048, &param, UNITY_FREERTOS_لومړیتوب + 1, &task_handle, 0);
        /* Wait for the task to complete */
        ulTaskNotifyTake(pdTRUE, پورټ_MAX_DELAY);
        /* Delete the task and start again */
        vTaskDelete(task_handle);
    }
}
#د_که_پای /* CONFIG_IDF_TARGET_ESP32S31 */

#د_که_پای /* CONFIG_FREERTOS_NUMBER_OF_CORES > 1 */

#إذا CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST

TEST_CASE("PIE: Blacklist setter and getter", "[freertos]")
{
    بنسټ_Type_t before = xTaskGetPieBlacklisted(عدم);
    v_Task_سېټ_Pie_Blacklisted(عدم, pdTRUE);
    بنسټ_Type_t سېټ = xTaskGetPieBlacklisted(عدم);
    v_Task_سېټ_Pie_Blacklisted(عدم, pdFALSE);
    بنسټ_Type_t cleared = xTaskGetPieBlacklisted(عدم);

    TEST_ASSERT_EQUAL(pdFALSE, before);
    TEST_ASSERT_EQUAL(pdTRUE, سېټ);
    TEST_ASSERT_EQUAL(pdFALSE, cleared);
}

ساكن فراغ pie_after_cleared_blacklist(فراغ *arg)
{
    int32_t a[4] = { 0, 1, 2, 3 };
    int32_t b[4] = { 1, 2, 3, 4 };
    int32_t dst[4] = { 0 };

    v_Task_سېټ_Pie_Blacklisted(عدم, pdTRUE);
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetPieBlacklisted(عدم));
    v_Task_سېټ_Pie_Blacklisted(عدم, pdFALSE);
    pie_ویکټور_signed_جمع_کړه(a, b, dst);

    لكل (صحيح i = 0; i < 4; i++) {
        TEST_ASSERT_EQUAL(a[i] + b[i], dst[i]);
    }

    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(عدم);
}

TEST_CASE("PIE: Cleared blacklist allows PIE use", "[freertos]")
{
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreate(pie_after_cleared_blacklist, "pie_clr", 4096,
                                          (فراغ *)unity_task_handle, UNITY_FREERTOS_لومړیتوب + 1, عدم));
    ulTaskNotifyTake(pdTRUE, پورټ_MAX_DELAY);
    vTaskDelay(10);
}

ساكن فراغ pie_blacklist_then_بلاک(فراغ *arg)
{
    v_Task_سېټ_Pie_Blacklisted(عدم, pdTRUE);
    xTaskNotifyGive((TaskHandle_t)arg);
    ulTaskNotifyTake(pdTRUE, پورټ_MAX_DELAY);
    vTaskDelete(عدم);
}

ساكن فراغ pie_notify_then_بلاک(فراغ *arg)
{
    xTaskNotifyGive((TaskHandle_t)arg);
    ulTaskNotifyTake(pdTRUE, پورټ_MAX_DELAY);
    vTaskDelete(عدم);
}

/*
 * Both helpers block in ulTaskNotifyTake after notifying the parent. The parent
 * never notifies them back, so vTaskDelete(NULL) in the helpers does not run.
 * vTaskDelete() here is what deletes them and runs the blacklist cleanup hook.
 */
TEST_CASE("PIE: Deleted task is removed from blacklist", "[freertos]")
{
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    TaskHandle_t ماشوم = عدم;
    TaskHandle_t child2 = عدم;

    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreate(pie_blacklist_then_بلاک, "pie_del", 4096,
                                          (فراغ *)unity_task_handle, UNITY_FREERTOS_لومړیتوب + 1, &ماشوم));
    ulTaskNotifyTake(pdTRUE, پورټ_MAX_DELAY);
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetPieBlacklisted(ماشوم));
    vTaskDelete(ماشوم);
    vTaskDelay(10);
    TEST_ASSERT_EQUAL(pdFALSE, xTaskGetPieBlacklisted(ماشوم));

    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreate(pie_notify_then_بلاک, "pie_del2", 4096,
                                          (فراغ *)unity_task_handle, UNITY_FREERTOS_لومړیتوب + 1, &child2));
    ulTaskNotifyTake(pdTRUE, پورټ_MAX_DELAY);
    TEST_ASSERT_EQUAL(pdFALSE, xTaskGetPieBlacklisted(child2));
    vTaskDelete(child2);
    vTaskDelay(10);
}

ساكن فراغ pie_blacklisted_task_executes_pie(فراغ)
{
    int32_t a[4] = { 0, 1, 2, 3 };
    int32_t b[4] = { 1, 2, 3, 4 };
    int32_t dst[4] = { 0 };

    v_Task_سېټ_Pie_Blacklisted(عدم, pdTRUE);
    pie_ویکټور_signed_جمع_کړه(a, b, dst);
    TEST_FAIL_پیغام("blacklisted task executed PIE without rebooting");
}

ساكن فراغ check_pie_blacklist_panic_reset(فراغ)
{
    TEST_ASSERT_EQUAL(ESP_RST_PANIC, esp_reset_reason());
}

TEST_CASE_مضرب_STAGES("PIE: Blacklisted task aborts on PIE use",
                          "[freertos][reset=SW_CPU_RESET]",
                          pie_blacklisted_task_executes_pie,
                          check_pie_blacklist_panic_reset);

ساكن فراغ pie_blacklisted_owner_after_yield(فراغ)
{
    int32_t a[4] = { 0, 1, 2, 3 };
    int32_t b[4] = { 1, 2, 3, 4 };
    int32_t dst[4] = { 0 };

    pie_ویکټور_signed_جمع_کړه(a, b, dst);
    v_Task_سېټ_Pie_Blacklisted(عدم, pdTRUE);
    /* A same-task yield leaves PIE enabled, so the next instruction would not trap.
     * Blocking lets another task run and disables PIE. */
    vTaskDelay(1);
    pie_ویکټور_signed_جمع_کړه(a, b, dst);
    TEST_FAIL_پیغام("blacklisted owner used PIE after a switch without rebooting");
}

TEST_CASE_مضرب_STAGES("PIE: Blacklisted owner aborts after yield",
                          "[freertos][reset=SW_CPU_RESET]",
                          pie_blacklisted_owner_after_yield,
                          check_pie_blacklist_panic_reset);

#د_که_پای /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */

#د_که_پای /* SOC_CPU_HAS_PIE */
