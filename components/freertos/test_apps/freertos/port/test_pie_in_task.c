/*
 * SPDX-FileCopyrightText: 2024-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ <math.h>
#అన్తర్భావ "soc/soc_caps.h"
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "freertos/semphr.h"
#అన్తర్భావ "freertos/idf_additions.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "unity_test_runner.h"
#అన్తర్భావ "esp_system.h"
#అన్తర్భావ "test_utils.h"

/* PIE instructions set is currently only supported in GCC compiler */
#ఒకవేళ SOC_CPU_HAS_PIE

/**
 * @brief Performs the signed sum of two 4-word vectors using the PIE.
 *
 * @param a First vector
 * @param b Second vector
 * @param dst Destination to store the sum
 */
శూన్య pie_సదిశ_signed_కూడు(స్థిరరాశి int32_t a[4], స్థిరరాశి int32_t b[4], int32_t dst[4]);

/**
 * @brief Performs a XOR operation on two 4-word vectors using the PIE.
 *
 * @param a First vector
 * @param b Second vector
 * @param dst Destination to store the result
 */
శూన్య pie_సదిశ_xor(స్థిరరాశి int32_t a[4], స్థిరరాశి int32_t b[4], int32_t dst[4]);

/* ------------------------------------------------------------------------------------------------------------------ */

రకనిర్వచనం నిర్మాణం {
    int32_t cst;
    TaskHandle_t ప్రధాన;
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

#నిర్వచించు TEST_PINNED_అంకె_TASKS       3
#నిర్వచించు TEST_PINNED_అంకె_ITERS       5

స్థిర శూన్య pinned_task(శూన్య *arg)
{
    pie_params_t *param = (pie_params_t*) arg;
    ulTaskNotifyTake(pdTRUE, పోర్టు_MAX_DELAY);

    int32_t constant = 42 * param->cst;
    int32_t a[4] = { constant, constant, constant, constant };
    int32_t b[4] = { 10, 20, 30, 40 };
    int32_t dst[4] = { 0 };

    pie_సదిశ_signed_కూడు(a, b, dst);

    కొరకు (పూర్ణాంకం i = 0; i < పరిమాణం(a) / పరిమాణం(uint32_t); i++) {
        TEST_ASSERT_EQUAL(dst[i], a[i] + b[i]);
    }

    // Indicate done and wait to be deleted
    xSemaphoreGive((SemaphoreHandle_t)param->sem);
    vTaskSuspend(ఖాళీ);
}

TEST_CASE("PIE: Usage in task", "[freertos]")
{
    SemaphoreHandle_t పూర్తి_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES * TEST_PINNED_అంకె_TASKS, 0);
    TEST_ASSERT_NOT_EQUAL(ఖాళీ, పూర్తి_sem);

    కొరకు (పూర్ణాంకం iter = 0; iter < TEST_PINNED_అంకె_ITERS; iter++) {
        TaskHandle_t task_handles[CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES][TEST_PINNED_అంకె_TASKS];
        pie_params_t params[CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES][TEST_PINNED_అంకె_TASKS];

        // Create test tasks for each core
        కొరకు (పూర్ణాంకం i = 0; i < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; i++) {
            కొరకు (పూర్ణాంకం j = 0; j < TEST_PINNED_అంకె_TASKS; j++) {
                params[i][j] = (pie_params_t) {
                    .cst = i + j + 1,
                    .sem = పూర్తి_sem,
                };
                TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_గమ్యం_Core(pinned_task, "task", 4096, (శూన్య *) &params[i][j], UNITY_FREERTOS_ప్రాధాన్యత + 1, &task_handles[i][j], i));
            }
        }

        // Start the created tasks
        కొరకు (పూర్ణాంకం i = 0; i < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; i++) {
            కొరకు (పూర్ణాంకం j = 0; j < TEST_PINNED_అంకె_TASKS; j++) {
                xTaskNotifyGive(task_handles[i][j]);
            }
        }

        // Wait for the tasks to complete
        కొరకు (పూర్ణాంకం i = 0; i < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES * TEST_PINNED_అంకె_TASKS; i++) {
            xSemaphoreTake(పూర్తి_sem, పోర్టు_MAX_DELAY);
        }

        // Delete the tasks
        కొరకు (పూర్ణాంకం i = 0; i < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; i++) {
            కొరకు (పూర్ణాంకం j = 0; j < TEST_PINNED_అంకె_TASKS; j++) {
                vTaskDelete(task_handles[i][j]);
            }
        }

        vTaskDelay(10); // Short delay to allow idle task to be free task memory and FPU contexts
    }

    vSemaphoreDelete(పూర్తి_sem);
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

#ఒకవేళ CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES > 1

#నిర్వచించు TEST_UNPINNED_అంకె_ITERS     5

స్థిర శూన్య check_core_affinity(శూన్య)
{
#ఒకవేళ CONFIG_IDF_లక్ష్యం_ESP32_S31
    /* On the ESP32-S31, only core 1 has PIE support, so all tasks should be pinned to core 1 */
    పునాది_Type_t core_అంకె = 1;
#లేకపోతే
    పునాది_Type_t core_అంకె = xTaskGetCoreID(ఖాళీ);
#అయితే_ముగింపు

    TEST_ASSERT_EQUAL(core_అంకె, xTaskGetCoreID(ఖాళీ));
}

స్థిర శూన్య unpinned_task(శూన్య *arg)
{
    // Check that the task is unpinned
#ఒకవేళ !CONFIG_FREERTOS_UNICORE
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, xTaskGetCoreID(ఖాళీ));
#అయితే_ముగింపు // !CONFIG_FREERTOS_UNICORE

    int32_t a[4] = { 0, 1, 2, 3};
    int32_t b[4] = { 111, 222, 333, 444 };
    int32_t dst[4] = { 0 };

    pie_సదిశ_signed_కూడు(a, b, dst);

    కొరకు (పూర్ణాంకం i = 0; i < పరిమాణం(a) / పరిమాణం(uint32_t); i++) {
        TEST_ASSERT_EQUAL(dst[i], a[i] + b[i]);
    }

#ఒకవేళ !CONFIG_FREERTOS_UNICORE
    check_core_affinity();
#అయితే_ముగింపు // !CONFIG_FREERTOS_UNICORE

    // Indicate done and self delete
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(ఖాళీ);
}

TEST_CASE("PIE: Usage in unpinned task", "[freertos]")
{
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    కొరకు (పూర్ణాంకం iter = 0; iter < TEST_UNPINNED_అంకె_ITERS; iter++) {
        // Create unpinned task
        xTaskCreate(unpinned_task, "unpin", 4096, (శూన్య *)unity_task_handle, UNITY_FREERTOS_ప్రాధాన్యత + 1, ఖాళీ);
        // Wait for task to complete
        ulTaskNotifyTake(pdTRUE, పోర్టు_MAX_DELAY);
        vTaskDelay(10); // Short delay to allow task memory to be freed
    }
}

/**
 * @brief Function performing some simple calculation using the PIE coprocessor.
 *        The goal is to be preempted by a task that also uses the PIE on the same core.
 */
స్థిర శూన్య pie_calculation(శూన్య* arg)
{
    pie_params_t* p = (pie_params_t*) arg;
    స్థిరరాశి int32_t cst = p->cst;
    int32_t a[4] = { cst, cst, cst, cst };
    int32_t dst[4] = { 0 };

    కొరకు (పూర్ణాంకం i = 0; i < 10; i++) {
        pie_సదిశ_signed_కూడు(a, dst, dst);

        /* Give some time to the other to interrupt us before checking `f` value */
        esp_rom_delay_us(1000);

        /* Using TEST_ASSERT_TRUE triggers a stack overflow, make sure the sign is still correct */
        ప్రతిజ్ఞా((dst[0] < 0 && cst < 0) || (dst[0] > 0 && cst > 0));

        /* Give the hand back to FreeRTOS to avoid any watchdog error */
        vTaskDelay(2);
    }

    /* Make sure the result is correct */
    ప్రతిజ్ఞా((dst[0] * cst == 10));

    xTaskNotifyGive(p->ప్రధాన);
    vTaskDelete(ఖాళీ);
}

TEST_CASE("PIE: Unsolicited context switch between tasks using the PIE", "[freertos]")
{
    /* Create two tasks that are on the same core and use the same FPU */
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    TaskHandle_t tasks[2];
    pie_params_t params[2] = {
        { .cst =  1, .ప్రధాన = unity_task_handle },
        { .cst = -1, .ప్రధాన = unity_task_handle },
    };

    x_Task_Create_Pinned_గమ్యం_Core(pie_calculation, "Task1", 2048, params + 0, UNITY_FREERTOS_ప్రాధాన్యత + 1, &tasks[0], 1);
    x_Task_Create_Pinned_గమ్యం_Core(pie_calculation, "Task2", 2048, params + 1, UNITY_FREERTOS_ప్రాధాన్యత + 1, &tasks[1], 1);

    ulTaskNotifyTake(pdTRUE, పోర్టు_MAX_DELAY);
    ulTaskNotifyTake(pdTRUE, పోర్టు_MAX_DELAY);
}

#ఒకవేళ CONFIG_IDF_లక్ష్యం_ESP32_S31

స్థిర శూన్య core0_fpu_pie(శూన్య *arg)
{
    /* Use the FPU in this task and make sure we are pinned to core 0 */
    దశాంశం f = 1.0f;
    కొరకు (పూర్ణాంకం i = 0; i < 10; i++) {
        f = f * 2.0f;
    }
    TEST_ASSERT_EQUAL(1024.0f, f);

    /* Check that the task is pinned to core 0 */
    TEST_ASSERT_EQUAL(0, xTaskGetCoreID(ఖాళీ));

    /* Use the PIE in this task and make sure we are migrated to core 1 */
    int32_t a[4] = { 0, 1, 2, 3};
    int32_t b[4] = { 111, 222, 333, 444 };
    int32_t dst[4] = { 0 };

    pie_సదిశ_signed_కూడు(a, b, dst);

    TEST_ASSERT_EQUAL(1, xTaskGetCoreID(ఖాళీ));

    కొరకు (పూర్ణాంకం i = 0; i < పరిమాణం(a) / పరిమాణం(uint32_t); i++) {
        TEST_ASSERT_EQUAL(dst[i], (a[i] + b[i]));
    }

    /* Try to use the FPU again on the new core */
    f = f * 2.0f;
    TEST_ASSERT_EQUAL(2048.0f, f);

    /* Indicate done and self delete */
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(ఖాళీ);
}

TEST_CASE("PIE: Usage in a task that owns the FPU on core 0", "[freertos]")
{
    /* Make sure that a task that owns the FPU on core 0 can be migrated to core 1 when using the PIE */
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    x_Task_Create_Pinned_గమ్యం_Core(core0_fpu_pie, "core0_fpu_pie", 4096,
                            (శూన్య*) unity_task_handle, UNITY_FREERTOS_ప్రాధాన్యత + 1, ఖాళీ, 0);
    // Wait for task to complete
    ulTaskNotifyTake(pdTRUE, పోర్టు_MAX_DELAY);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#నిర్వచించు PIE_INSTR_లెక్క 2

స్థిర శూన్య core0_pie_instr_task(శూన్య *arg)
{
    pie_params_t *param = (pie_params_t*) arg;

    ఎంపిక (param->cst) {
    స్థితి 0: {
        int32_t a = 1;
        int32_t b = 2;
        int32_t dst = 0;

        /* Opcode of type 0b0110011 */
        __asm__ అస్థిర("esp.addx2 %0, %1, %2" : "=r"(dst) : "r"(a), "r"(b));
        ఆపు;
    }
    స్థితి 1: {
        int32_t a[4] = { 0xffffff, 0xaaaaaaaa, 0x11111111, 0xffffffff };
        int32_t b[4] = { 0x11111111, 0xaaaaaaaa, 0xffffff, 0xffffffff };
        int32_t dst[4] = { 0 };
        /* Opcode of type 0bxx11x11 (x: don't care) */
        pie_సదిశ_xor(a, b, dst);
        ఆపు;
    }
    }
    /* Indicate done and wait for deletion */
    xTaskNotifyGive(param->ప్రధాన);
    vTaskDelay(పోర్టు_MAX_DELAY);
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
    param.ప్రధాన = xTaskGetCurrentTaskHandle();

    /* Create tasks that will run on core 0 only. Each of them will execute one PIE instruction.
     * If they don't crash, it's a success.*/
    కొరకు (పూర్ణాంకం i = 0; i < PIE_INSTR_లెక్క; i++) {
        param.cst = i;
        x_Task_Create_Pinned_గమ్యం_Core(core0_pie_instr_task, "Core0PIE", 2048, &param, UNITY_FREERTOS_ప్రాధాన్యత + 1, &task_handle, 0);
        /* Wait for the task to complete */
        ulTaskNotifyTake(pdTRUE, పోర్టు_MAX_DELAY);
        /* Delete the task and start again */
        vTaskDelete(task_handle);
    }
}
#అయితే_ముగింపు /* CONFIG_IDF_TARGET_ESP32S31 */

#అయితే_ముగింపు /* CONFIG_FREERTOS_NUMBER_OF_CORES > 1 */

#ఒకవేళ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST

TEST_CASE("PIE: Blacklist setter and getter", "[freertos]")
{
    పునాది_Type_t before = xTaskGetPieBlacklisted(ఖాళీ);
    v_Task_సమితి_Pie_Blacklisted(ఖాళీ, pdTRUE);
    పునాది_Type_t సమితి = xTaskGetPieBlacklisted(ఖాళీ);
    v_Task_సమితి_Pie_Blacklisted(ఖాళీ, pdFALSE);
    పునాది_Type_t cleared = xTaskGetPieBlacklisted(ఖాళీ);

    TEST_ASSERT_EQUAL(pdFALSE, before);
    TEST_ASSERT_EQUAL(pdTRUE, సమితి);
    TEST_ASSERT_EQUAL(pdFALSE, cleared);
}

స్థిర శూన్య pie_after_cleared_blacklist(శూన్య *arg)
{
    int32_t a[4] = { 0, 1, 2, 3 };
    int32_t b[4] = { 1, 2, 3, 4 };
    int32_t dst[4] = { 0 };

    v_Task_సమితి_Pie_Blacklisted(ఖాళీ, pdTRUE);
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetPieBlacklisted(ఖాళీ));
    v_Task_సమితి_Pie_Blacklisted(ఖాళీ, pdFALSE);
    pie_సదిశ_signed_కూడు(a, b, dst);

    కొరకు (పూర్ణాంకం i = 0; i < 4; i++) {
        TEST_ASSERT_EQUAL(a[i] + b[i], dst[i]);
    }

    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(ఖాళీ);
}

TEST_CASE("PIE: Cleared blacklist allows PIE use", "[freertos]")
{
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreate(pie_after_cleared_blacklist, "pie_clr", 4096,
                                          (శూన్య *)unity_task_handle, UNITY_FREERTOS_ప్రాధాన్యత + 1, ఖాళీ));
    ulTaskNotifyTake(pdTRUE, పోర్టు_MAX_DELAY);
    vTaskDelay(10);
}

స్థిర శూన్య pie_blacklist_then_ఖండం(శూన్య *arg)
{
    v_Task_సమితి_Pie_Blacklisted(ఖాళీ, pdTRUE);
    xTaskNotifyGive((TaskHandle_t)arg);
    ulTaskNotifyTake(pdTRUE, పోర్టు_MAX_DELAY);
    vTaskDelete(ఖాళీ);
}

స్థిర శూన్య pie_notify_then_ఖండం(శూన్య *arg)
{
    xTaskNotifyGive((TaskHandle_t)arg);
    ulTaskNotifyTake(pdTRUE, పోర్టు_MAX_DELAY);
    vTaskDelete(ఖాళీ);
}

/*
 * Both helpers block in ulTaskNotifyTake after notifying the parent. The parent
 * never notifies them back, so vTaskDelete(NULL) in the helpers does not run.
 * vTaskDelete() here is what deletes them and runs the blacklist cleanup hook.
 */
TEST_CASE("PIE: Deleted task is removed from blacklist", "[freertos]")
{
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    TaskHandle_t బిడ్డ = ఖాళీ;
    TaskHandle_t child2 = ఖాళీ;

    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreate(pie_blacklist_then_ఖండం, "pie_del", 4096,
                                          (శూన్య *)unity_task_handle, UNITY_FREERTOS_ప్రాధాన్యత + 1, &బిడ్డ));
    ulTaskNotifyTake(pdTRUE, పోర్టు_MAX_DELAY);
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetPieBlacklisted(బిడ్డ));
    vTaskDelete(బిడ్డ);
    vTaskDelay(10);
    TEST_ASSERT_EQUAL(pdFALSE, xTaskGetPieBlacklisted(బిడ్డ));

    TEST_ASSERT_EQUAL(pdTRUE, xTaskCreate(pie_notify_then_ఖండం, "pie_del2", 4096,
                                          (శూన్య *)unity_task_handle, UNITY_FREERTOS_ప్రాధాన్యత + 1, &child2));
    ulTaskNotifyTake(pdTRUE, పోర్టు_MAX_DELAY);
    TEST_ASSERT_EQUAL(pdFALSE, xTaskGetPieBlacklisted(child2));
    vTaskDelete(child2);
    vTaskDelay(10);
}

స్థిర శూన్య pie_blacklisted_task_executes_pie(శూన్య)
{
    int32_t a[4] = { 0, 1, 2, 3 };
    int32_t b[4] = { 1, 2, 3, 4 };
    int32_t dst[4] = { 0 };

    v_Task_సమితి_Pie_Blacklisted(ఖాళీ, pdTRUE);
    pie_సదిశ_signed_కూడు(a, b, dst);
    TEST_FAIL_సందేశం("blacklisted task executed PIE without rebooting");
}

స్థిర శూన్య check_pie_blacklist_panic_reset(శూన్య)
{
    TEST_ASSERT_EQUAL(ESP_RST_PANIC, esp_reset_reason());
}

TEST_CASE_గుణిజం_STAGES("PIE: Blacklisted task aborts on PIE use",
                          "[freertos][reset=SW_CPU_RESET]",
                          pie_blacklisted_task_executes_pie,
                          check_pie_blacklist_panic_reset);

స్థిర శూన్య pie_blacklisted_owner_after_yield(శూన్య)
{
    int32_t a[4] = { 0, 1, 2, 3 };
    int32_t b[4] = { 1, 2, 3, 4 };
    int32_t dst[4] = { 0 };

    pie_సదిశ_signed_కూడు(a, b, dst);
    v_Task_సమితి_Pie_Blacklisted(ఖాళీ, pdTRUE);
    /* A same-task yield leaves PIE enabled, so the next instruction would not trap.
     * Blocking lets another task run and disables PIE. */
    vTaskDelay(1);
    pie_సదిశ_signed_కూడు(a, b, dst);
    TEST_FAIL_సందేశం("blacklisted owner used PIE after a switch without rebooting");
}

TEST_CASE_గుణిజం_STAGES("PIE: Blacklisted owner aborts after yield",
                          "[freertos][reset=SW_CPU_RESET]",
                          pie_blacklisted_owner_after_yield,
                          check_pie_blacklist_panic_reset);

#అయితే_ముగింపు /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */

#అయితే_ముగింపు /* SOC_CPU_HAS_PIE */
