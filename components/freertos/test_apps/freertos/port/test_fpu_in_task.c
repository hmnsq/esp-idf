/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ <math.h>
#అన్తర్భావ "soc/soc_caps.h"
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "freertos/semphr.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "test_utils.h"

#ఒకవేళ SOC_CPU_HAS_FPU

/* ------------------------------------------------------------------------------------------------------------------ */

/*
Test FPU usage from a task context

Purpose:
    - Test that the FPU can be used from a task context
    - Test that FPU context is properly saved and restored
    - Test that FPU context is cleaned up on task deletion by running multiple iterations
Procedure:
    - Create TEST_PINNED_NUM_TASKS tasks pinned to each core
    - Start each task
    - Each task updates a float variable and then blocks (to allow other tasks to run thus forcing the an FPU context
      save and restore).
    - Delete each task
    - Repeat test for TEST_PINNED_NUM_ITERS iterations
Expected:
    - Correct float value calculated by each task
    - Each task cleans up its FPU context on deletion
*/

#నిర్వచించు TEST_PINNED_అంకె_TASKS       3
#నిర్వచించు TEST_PINNED_అంకె_ITERS       5

స్థిర శూన్య pinned_task(శూన్య *arg)
{
    ulTaskNotifyTake(pdTRUE, పోర్టు_MAX_DELAY);

    /*
    Use the FPU
    - We test using a calculation that will cause a change in mantissa and exponent for extra thoroughness
    - cosf(0.0f) should return 1.0f, thus we are simply doubling test_float every iteration.
    - Therefore, we should end up with (0.01) * (2^8) = 2.56 at the end of the loop
    */
    అస్థిర దశాంశం test_float = 0.01f;
    కొరకు (పూర్ణాంకం i = 0; i < 8; i++) {
        test_float = test_float * 2.0f * cosf(0.0f);
        vTaskDelay(1);  // Block to cause a context switch, forcing the FPU context to be saved
    }
    // We allow a 0.1% delta on the final result in case of any loss of precision from floating point calculations
    TEST_ASSERT_FLOAT_WITHIN(0.00256f, 2.56f, test_float);

    // Indicate done wand wait to be deleted
    xSemaphoreGive((SemaphoreHandle_t)arg);
    vTaskSuspend(ఖాళీ);
}

TEST_CASE("FPU: Usage in task", "[freertos]")
{
    SemaphoreHandle_t పూర్తి_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES * TEST_PINNED_అంకె_TASKS, 0);
    TEST_ASSERT_NOT_EQUAL(ఖాళీ, పూర్తి_sem);

    కొరకు (పూర్ణాంకం iter = 0; iter < TEST_PINNED_అంకె_ITERS; iter++) {
        TaskHandle_t task_handles[CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES][TEST_PINNED_అంకె_TASKS];

        // Create test tasks for each core
        కొరకు (పూర్ణాంకం i = 0; i < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; i++) {
            కొరకు (పూర్ణాంకం j = 0; j < TEST_PINNED_అంకె_TASKS; j++) {
                TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_గమ్యం_Core(pinned_task, "task", 4096, (శూన్య *)పూర్తి_sem, UNITY_FREERTOS_ప్రాధాన్యత + 1, &task_handles[i][j], i));
            }
        }

        // Start the created tasks simultaneously
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
Test FPU usage will pin an unpinned task

Purpose:
    - Test that unpinned tasks are automatically pinned to the current core on the task's first use of the FPU
    - Test that FPU context is cleaned up on task deletion by running multiple iterations
Procedure:
    - Create an unpinned task
    - Task disables scheduling/preemption to ensure that it does not switch cores
    - Task uses the FPU
    - Task checks its core affinity after FPU usage
    - Task deletes itself
    - Repeat test for TEST_UNPINNED_NUM_ITERS iterations
Expected:
    - Task remains unpinned until its first usage of the FPU
    - The task becomes pinned to the current core after first use of the FPU
    - Each task cleans up its FPU context on deletion
*/

#ఒకవేళ CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES > 1

#నిర్వచించు TEST_UNPINNED_అంకె_ITERS     5

స్థిర శూన్య unpinned_task(శూన్య *arg)
{
    // Disable scheduling/preemption to make sure current core ID doesn't change
#ఒకవేళ ( ( CONFIG_FREERTOS_SMP ) && ( !CONFIG_FREERTOS_UNICORE ) )
    vTaskPreemptionDisable(ఖాళీ);
#లేకపోతే
    vTaskSuspendAll();
#అయితే_ముగింపు
    పునాది_Type_t cur_core_అంకె = x_పోర్టు_Get_Core_ID();
    // Check that the task is unpinned
#ఒకవేళ !CONFIG_FREERTOS_UNICORE
#ఒకవేళ CONFIG_FREERTOS_SMP
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, vTaskCoreAffinityGet(ఖాళీ));
#లేకపోతే
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, xTaskGetCoreID(ఖాళీ));
#అయితే_ముగింపు
#అయితే_ముగింపు // !CONFIG_FREERTOS_UNICORE

    /*
    Use the FPU
    - We test using a calculation that will cause a change in mantissa and exponent for extra thoroughness
    - cosf(0.0f) should return 1.0f, thus we are simply doubling test_float every iteration.
    - Therefore, we should end up with (0.01) * (2^8) = 2.56 at the end of the loop
    */
    అస్థిర దశాంశం test_float = 0.01f;
    కొరకు (పూర్ణాంకం i = 0; i < 8; i++) {
        test_float = test_float * 2.0f * cosf(0.0f);
    }
    // We allow a 0.1% delta on the final result in case of any loss of precision from floating point calculations
    TEST_ASSERT_FLOAT_WITHIN(0.00256f, 2.56f, test_float);

#ఒకవేళ !CONFIG_FREERTOS_UNICORE
#ఒకవేళ CONFIG_FREERTOS_SMP
    TEST_ASSERT_EQUAL(1 << cur_core_అంకె, vTaskCoreAffinityGet(ఖాళీ));
#లేకపోతే
    TEST_ASSERT_EQUAL(cur_core_అంకె, xTaskGetCoreID(ఖాళీ));
#అయితే_ముగింపు
#అయితే_ముగింపు // !CONFIG_FREERTOS_UNICORE
    // Re-enable scheduling/preemption
#ఒకవేళ ( ( CONFIG_FREERTOS_SMP ) && ( !CONFIG_FREERTOS_UNICORE ) )
    vTaskPreemptionEnable(ఖాళీ);
#లేకపోతే
    xTaskResumeAll();
#అయితే_ముగింపు

    // Indicate done and self delete
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(ఖాళీ);
}

TEST_CASE("FPU: Usage in unpinned task", "[freertos]")
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

రకనిర్వచనం నిర్మాణం {
    bool negative;
    TaskHandle_t ప్రధాన;
} fpu_params_t;

/**
 * @brief Function performing some simple calculation using several FPU registers.
 *        The goal is to be preempted by a task that also uses the FPU on the same core.
 */
శూన్య fpu_calculation(శూన్య* arg)
{
    fpu_params_t* p = (fpu_params_t*) arg;
    స్థిరరాశి bool negative = p->negative;
    స్థిరరాశి దశాంశం init = negative ? -1.f : 1.f;
    దశాంశం f = init;

    కొరకు (పూర్ణాంకం i = 0; i < 10; i++) {
        /* The following calculation doesn't really have any meaning, we try to use several FPU registers and operations */
        దశాంశం delta = negative ? -1.1f : 1.1f;
        కొరకు (పూర్ణాంకం i = 0; i < 1000; i++) {
            f += delta;
            delta += negative ? -0.1f : 0.1f;
        }
        /* Give some time to the other to interrupt us before checking `f` value */
        esp_rom_delay_us(1000);

        /**
         * If the FPU context was not saved properly by FreeRTOS, our value `f` will not have to correct sign!
         * It'll have the sign of the other tasks' `f` value.
         * Use assert to make sure the sign is correct. Using TEST_ASSERT_TRUE triggers a stack overflow.
         */
        ప్రతిజ్ఞా((negative && f < 0.0f) || (!negative && f > 0.0f));
        f = init;

        /* Give the hand back to FreeRTOS to avoid any watchdog */
        vTaskDelay(2);
    }

    xTaskNotifyGive(p->ప్రధాన);
    vTaskDelete(ఖాళీ);
}

TEST_CASE("FPU: Unsolicited context switch between tasks using FPU", "[freertos]")
{
    /* Create two tasks that are on the same core and use the same FPU */
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    TaskHandle_t tasks[2];
    fpu_params_t params[2] = {
        { .negative = false, .ప్రధాన = unity_task_handle },
        { .negative = true,  .ప్రధాన = unity_task_handle },
    };

    x_Task_Create_Pinned_గమ్యం_Core(fpu_calculation, "Task1", 2048, params + 0, UNITY_FREERTOS_ప్రాధాన్యత + 1, &tasks[0], 1);
    x_Task_Create_Pinned_గమ్యం_Core(fpu_calculation, "Task2", 2048, params + 1, UNITY_FREERTOS_ప్రాధాన్యత + 1, &tasks[1], 1);

    ulTaskNotifyTake(pdTRUE, పోర్టు_MAX_DELAY);
    ulTaskNotifyTake(pdTRUE, పోర్టు_MAX_DELAY);
}

#అయితే_ముగింపు // CONFIG_FREERTOS_NUMBER_OF_CORES > 1
#అయితే_ముగింపు // SOC_CPU_HAS_FPU
