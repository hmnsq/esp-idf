/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ <math.h>
#ಅನ್ತರ್ಭಾವ "soc/soc_caps.h"
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "freertos/semphr.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "test_utils.h"

#ಯದಿ SOC_CPU_HAS_FPU

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

#define TEST_PINNED_ಅಂಕೆ_TASKS       3
#define TEST_PINNED_ಅಂಕೆ_ITERS       5

ಸ್ಥಿರ ಶೂನ್ಯ pinned_task(ಶೂನ್ಯ *arg)
{
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    /*
    Use the FPU
    - We test using a calculation that will cause a change in mantissa and exponent for extra thoroughness
    - cosf(0.0f) should return 1.0f, thus we are simply doubling test_float every iteration.
    - Therefore, we should end up with (0.01) * (2^8) = 2.56 at the end of the loop
    */
    ಚಞ್ಚಲ ಭಿನ್ನ test_float = 0.01f;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < 8; i++) {
        test_float = test_float * 2.0f * cosf(0.0f);
        vTaskDelay(1);  // Block to cause a context switch, forcing the FPU context to be saved
    }
    // We allow a 0.1% delta on the final result in case of any loss of precision from floating point calculations
    TEST_ASSERT_FLOAT_WITHIN(0.00256f, 2.56f, test_float);

    // Indicate done wand wait to be deleted
    xSemaphoreGive((SemaphoreHandle_t)arg);
    vTaskSuspend(ರಿಕ್ತ);
}

TEST_CASE("FPU: Usage in task", "[freertos]")
{
    SemaphoreHandle_t done_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES * TEST_PINNED_ಅಂಕೆ_TASKS, 0);
    TEST_ASSERT_NOT_EQUAL(ರಿಕ್ತ, done_sem);

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ iter = 0; iter < TEST_PINNED_ಅಂಕೆ_ITERS; iter++) {
        TaskHandle_t task_handles[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES][TEST_PINNED_ಅಂಕೆ_TASKS];

        // Create test tasks for each core
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES; i++) {
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < TEST_PINNED_ಅಂಕೆ_TASKS; j++) {
                TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(pinned_task, "task", 4096, (ಶೂನ್ಯ *)done_sem, UNITY_FREERTOS_PRIORITY + 1, &task_handles[i][j], i));
            }
        }

        // Start the created tasks simultaneously
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

#ಯದಿ CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES > 1

#define TEST_UNPINNED_ಅಂಕೆ_ITERS     5

ಸ್ಥಿರ ಶೂನ್ಯ unpinned_task(ಶೂನ್ಯ *arg)
{
    // Disable scheduling/preemption to make sure current core ID doesn't change
#ಯದಿ ( ( CONFIG_FREERTOS_SMP ) && ( !CONFIG_FREERTOS_UNICORE ) )
    vTaskPreemptionDisable(ರಿಕ್ತ);
#ಅನ್ಯಥಾ
    vTaskSuspendAll();
#endif
    BaseType_t cur_core_ಅಂಕೆ = xPortGetCoreID();
    // Check that the task is unpinned
#ಯದಿ !CONFIG_FREERTOS_UNICORE
#ಯದಿ CONFIG_FREERTOS_SMP
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, vTaskCoreAffinityGet(ರಿಕ್ತ));
#ಅನ್ಯಥಾ
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, xTaskGetCoreID(ರಿಕ್ತ));
#endif
#endif // !CONFIG_FREERTOS_UNICORE

    /*
    Use the FPU
    - We test using a calculation that will cause a change in mantissa and exponent for extra thoroughness
    - cosf(0.0f) should return 1.0f, thus we are simply doubling test_float every iteration.
    - Therefore, we should end up with (0.01) * (2^8) = 2.56 at the end of the loop
    */
    ಚಞ್ಚಲ ಭಿನ್ನ test_float = 0.01f;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < 8; i++) {
        test_float = test_float * 2.0f * cosf(0.0f);
    }
    // We allow a 0.1% delta on the final result in case of any loss of precision from floating point calculations
    TEST_ASSERT_FLOAT_WITHIN(0.00256f, 2.56f, test_float);

#ಯದಿ !CONFIG_FREERTOS_UNICORE
#ಯದಿ CONFIG_FREERTOS_SMP
    TEST_ASSERT_EQUAL(1 << cur_core_ಅಂಕೆ, vTaskCoreAffinityGet(ರಿಕ್ತ));
#ಅನ್ಯಥಾ
    TEST_ASSERT_EQUAL(cur_core_ಅಂಕೆ, xTaskGetCoreID(ರಿಕ್ತ));
#endif
#endif // !CONFIG_FREERTOS_UNICORE
    // Re-enable scheduling/preemption
#ಯದಿ ( ( CONFIG_FREERTOS_SMP ) && ( !CONFIG_FREERTOS_UNICORE ) )
    vTaskPreemptionEnable(ರಿಕ್ತ);
#ಅನ್ಯಥಾ
    xTaskResumeAll();
#endif

    // Indicate done and self delete
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(ರಿಕ್ತ);
}

TEST_CASE("FPU: Usage in unpinned task", "[freertos]")
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

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    bool negative;
    TaskHandle_t main;
} fpu_params_t;

/**
 * @brief Function performing some simple calculation using several FPU registers.
 *        The goal is to be preempted by a task that also uses the FPU on the same core.
 */
ಶೂನ್ಯ fpu_calculation(ಶೂನ್ಯ* arg)
{
    fpu_params_t* p = (fpu_params_t*) arg;
    ಧ್ರುವ bool negative = p->negative;
    ಧ್ರುವ ಭಿನ್ನ init = negative ? -1.f : 1.f;
    ಭಿನ್ನ f = init;

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < 10; i++) {
        /* The following calculation doesn't really have any meaning, we try to use several FPU registers and operations */
        ಭಿನ್ನ delta = negative ? -1.1f : 1.1f;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < 1000; i++) {
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
        ಪ್ರತಿಜ್ಞಾ((negative && f < 0.0f) || (!negative && f > 0.0f));
        f = init;

        /* Give the hand back to FreeRTOS to avoid any watchdog */
        vTaskDelay(2);
    }

    xTaskNotifyGive(p->main);
    vTaskDelete(ರಿಕ್ತ);
}

TEST_CASE("FPU: Unsolicited context switch between tasks using FPU", "[freertos]")
{
    /* Create two tasks that are on the same core and use the same FPU */
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    TaskHandle_t tasks[2];
    fpu_params_t params[2] = {
        { .negative = false, .main = unity_task_handle },
        { .negative = true,  .main = unity_task_handle },
    };

    xTaskCreatePinnedToCore(fpu_calculation, "Task1", 2048, params + 0, UNITY_FREERTOS_PRIORITY + 1, &tasks[0], 1);
    xTaskCreatePinnedToCore(fpu_calculation, "Task2", 2048, params + 1, UNITY_FREERTOS_PRIORITY + 1, &tasks[1], 1);

    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
}

#endif // CONFIG_FREERTOS_NUMBER_OF_CORES > 1
#endif // SOC_CPU_HAS_FPU
