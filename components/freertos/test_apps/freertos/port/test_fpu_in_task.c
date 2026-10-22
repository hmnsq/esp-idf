/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <math.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "soc/soc_caps.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/semphr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "test_utils.h"

#ਜੇ SOC_CPU_HAS_FPU

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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_PINNED_ਅੰਕ_TASKS       3
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_PINNED_ਅੰਕ_ITERS       5

ਸਥਿਰ ਖਾਲੀ pinned_task(ਖਾਲੀ *arg)
{
    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);

    /*
    Use the FPU
    - We test using a calculation that will cause a change in mantissa and exponent for extra thoroughness
    - cosf(0.0f) should return 1.0f, thus we are simply doubling test_float every iteration.
    - Therefore, we should end up with (0.01) * (2^8) = 2.56 at the end of the loop
    */
    ਅਸਥਿਰ ਦਸ਼ਮਲਵ test_float = 0.01f;
    ਲਈ (ਪੂਰਨ i = 0; i < 8; i++) {
        test_float = test_float * 2.0f * cosf(0.0f);
        vTaskDelay(1);  // Block to cause a context switch, forcing the FPU context to be saved
    }
    // We allow a 0.1% delta on the final result in case of any loss of precision from floating point calculations
    TEST_ASSERT_FLOAT_WITHIN(0.00256f, 2.56f, test_float);

    // Indicate done wand wait to be deleted
    xSemaphoreGive((SemaphoreHandle_t)arg);
    vTaskSuspend(ਨਿਰਾ);
}

TEST_CASE("FPU: Usage in task", "[freertos]")
{
    SemaphoreHandle_t ਪੂਰਾ_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES * TEST_PINNED_ਅੰਕ_TASKS, 0);
    TEST_ASSERT_NOT_EQUAL(ਨਿਰਾ, ਪੂਰਾ_sem);

    ਲਈ (ਪੂਰਨ iter = 0; iter < TEST_PINNED_ਅੰਕ_ITERS; iter++) {
        TaskHandle_t task_handles[CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES][TEST_PINNED_ਅੰਕ_TASKS];

        // Create test tasks for each core
        ਲਈ (ਪੂਰਨ i = 0; i < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; i++) {
            ਲਈ (ਪੂਰਨ j = 0; j < TEST_PINNED_ਅੰਕ_TASKS; j++) {
                TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(pinned_task, "task", 4096, (ਖਾਲੀ *)ਪੂਰਾ_sem, UNITY_FREERTOS_ਤਰਜੀਹ + 1, &task_handles[i][j], i));
            }
        }

        // Start the created tasks simultaneously
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

#ਜੇ CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES > 1

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_UNPINNED_ਅੰਕ_ITERS     5

ਸਥਿਰ ਖਾਲੀ unpinned_task(ਖਾਲੀ *arg)
{
    // Disable scheduling/preemption to make sure current core ID doesn't change
#ਜੇ ( ( CONFIG_FREERTOS_SMP ) && ( !CONFIG_FREERTOS_UNICORE ) )
    vTaskPreemptionDisable(ਨਿਰਾ);
#ਨਹੀਂਤਾਂ
    vTaskSuspendAll();
#ਜੇ_ਅੰਤ
    ਅਧਾਰ_Type_t cur_core_ਅੰਕ = x_ਪੋਰਟ_Get_Core_ID();
    // Check that the task is unpinned
#ਜੇ !CONFIG_FREERTOS_UNICORE
#ਜੇ CONFIG_FREERTOS_SMP
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, vTaskCoreAffinityGet(ਨਿਰਾ));
#ਨਹੀਂਤਾਂ
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, xTaskGetCoreID(ਨਿਰਾ));
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ // !CONFIG_FREERTOS_UNICORE

    /*
    Use the FPU
    - We test using a calculation that will cause a change in mantissa and exponent for extra thoroughness
    - cosf(0.0f) should return 1.0f, thus we are simply doubling test_float every iteration.
    - Therefore, we should end up with (0.01) * (2^8) = 2.56 at the end of the loop
    */
    ਅਸਥਿਰ ਦਸ਼ਮਲਵ test_float = 0.01f;
    ਲਈ (ਪੂਰਨ i = 0; i < 8; i++) {
        test_float = test_float * 2.0f * cosf(0.0f);
    }
    // We allow a 0.1% delta on the final result in case of any loss of precision from floating point calculations
    TEST_ASSERT_FLOAT_WITHIN(0.00256f, 2.56f, test_float);

#ਜੇ !CONFIG_FREERTOS_UNICORE
#ਜੇ CONFIG_FREERTOS_SMP
    TEST_ASSERT_EQUAL(1 << cur_core_ਅੰਕ, vTaskCoreAffinityGet(ਨਿਰਾ));
#ਨਹੀਂਤਾਂ
    TEST_ASSERT_EQUAL(cur_core_ਅੰਕ, xTaskGetCoreID(ਨਿਰਾ));
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ // !CONFIG_FREERTOS_UNICORE
    // Re-enable scheduling/preemption
#ਜੇ ( ( CONFIG_FREERTOS_SMP ) && ( !CONFIG_FREERTOS_UNICORE ) )
    vTaskPreemptionEnable(ਨਿਰਾ);
#ਨਹੀਂਤਾਂ
    xTaskResumeAll();
#ਜੇ_ਅੰਤ

    // Indicate done and self delete
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(ਨਿਰਾ);
}

TEST_CASE("FPU: Usage in unpinned task", "[freertos]")
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

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    bool negative;
    TaskHandle_t ਮੁੱਖ;
} fpu_params_t;

/**
 * @brief Function performing some simple calculation using several FPU registers.
 *        The goal is to be preempted by a task that also uses the FPU on the same core.
 */
ਖਾਲੀ fpu_calculation(ਖਾਲੀ* arg)
{
    fpu_params_t* p = (fpu_params_t*) arg;
    ਅਚਲ bool negative = p->negative;
    ਅਚਲ ਦਸ਼ਮਲਵ init = negative ? -1.f : 1.f;
    ਦਸ਼ਮਲਵ f = init;

    ਲਈ (ਪੂਰਨ i = 0; i < 10; i++) {
        /* The following calculation doesn't really have any meaning, we try to use several FPU registers and operations */
        ਦਸ਼ਮਲਵ delta = negative ? -1.1f : 1.1f;
        ਲਈ (ਪੂਰਨ i = 0; i < 1000; i++) {
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
        ਪ੍ਰਤਿਜ੍ਞਾ((negative && f < 0.0f) || (!negative && f > 0.0f));
        f = init;

        /* Give the hand back to FreeRTOS to avoid any watchdog */
        vTaskDelay(2);
    }

    xTaskNotifyGive(p->ਮੁੱਖ);
    vTaskDelete(ਨਿਰਾ);
}

TEST_CASE("FPU: Unsolicited context switch between tasks using FPU", "[freertos]")
{
    /* Create two tasks that are on the same core and use the same FPU */
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    TaskHandle_t tasks[2];
    fpu_params_t params[2] = {
        { .negative = false, .ਮੁੱਖ = unity_task_handle },
        { .negative = true,  .ਮੁੱਖ = unity_task_handle },
    };

    x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(fpu_calculation, "Task1", 2048, params + 0, UNITY_FREERTOS_ਤਰਜੀਹ + 1, &tasks[0], 1);
    x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(fpu_calculation, "Task2", 2048, params + 1, UNITY_FREERTOS_ਤਰਜੀਹ + 1, &tasks[1], 1);

    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);
    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);
}

#ਜੇ_ਅੰਤ // CONFIG_FREERTOS_NUMBER_OF_CORES > 1
#ਜੇ_ਅੰਤ // SOC_CPU_HAS_FPU
