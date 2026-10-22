/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#समावेश "sdkconfig.h"
#समावेश <math.h>
#समावेश "soc/soc_caps.h"
#समावेश "freertos/FreeRTOS.h"
#समावेश "freertos/task.h"
#समावेश "freertos/semphr.h"
#समावेश "unity.h"
#समावेश "test_utils.h"

#अगर SOC_CPU_HAS_FPU

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

#घोषणा TEST_PINNED_अंक_TASKS       3
#घोषणा TEST_PINNED_अंक_ITERS       5

अटल व्योम pinned_task(व्योम *arg)
{
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    /*
    Use the FPU
    - We test using a calculation that will cause a change in mantissa and exponent for extra thoroughness
    - cosf(0.0f) should return 1.0f, thus we are simply doubling test_float every iteration.
    - Therefore, we should end up with (0.01) * (2^8) = 2.56 at the end of the loop
    */
    अस्थिर भग्न test_float = 0.01f;
    क्रम (पूर्णांक i = 0; i < 8; i++) {
        test_float = test_float * 2.0f * cosf(0.0f);
        vTaskDelay(1);  // Block to cause a context switch, forcing the FPU context to be saved
    }
    // We allow a 0.1% delta on the final result in case of any loss of precision from floating point calculations
    TEST_ASSERT_FLOAT_WITHIN(0.00256f, 2.56f, test_float);

    // Indicate done wand wait to be deleted
    xSemaphoreGive((SemaphoreHandle_t)arg);
    vTaskSuspend(शून्य);
}

TEST_CASE("FPU: Usage in task", "[freertos]")
{
    SemaphoreHandle_t done_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_संख्या_OF_CORES * TEST_PINNED_अंक_TASKS, 0);
    TEST_ASSERT_NOT_EQUAL(शून्य, done_sem);

    क्रम (पूर्णांक iter = 0; iter < TEST_PINNED_अंक_ITERS; iter++) {
        TaskHandle_t task_handles[CONFIG_FREERTOS_संख्या_OF_CORES][TEST_PINNED_अंक_TASKS];

        // Create test tasks for each core
        क्रम (पूर्णांक i = 0; i < CONFIG_FREERTOS_संख्या_OF_CORES; i++) {
            क्रम (पूर्णांक j = 0; j < TEST_PINNED_अंक_TASKS; j++) {
                TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(pinned_task, "task", 4096, (व्योम *)done_sem, UNITY_FREERTOS_PRIORITY + 1, &task_handles[i][j], i));
            }
        }

        // Start the created tasks simultaneously
        क्रम (पूर्णांक i = 0; i < CONFIG_FREERTOS_संख्या_OF_CORES; i++) {
            क्रम (पूर्णांक j = 0; j < TEST_PINNED_अंक_TASKS; j++) {
                xTaskNotifyGive(task_handles[i][j]);
            }
        }

        // Wait for the tasks to complete
        क्रम (पूर्णांक i = 0; i < CONFIG_FREERTOS_संख्या_OF_CORES * TEST_PINNED_अंक_TASKS; i++) {
            xSemaphoreTake(done_sem, portMAX_DELAY);
        }

        // Delete the tasks
        क्रम (पूर्णांक i = 0; i < CONFIG_FREERTOS_संख्या_OF_CORES; i++) {
            क्रम (पूर्णांक j = 0; j < TEST_PINNED_अंक_TASKS; j++) {
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

#अगर CONFIG_FREERTOS_संख्या_OF_CORES > 1

#घोषणा TEST_UNPINNED_अंक_ITERS     5

अटल व्योम unpinned_task(व्योम *arg)
{
    // Disable scheduling/preemption to make sure current core ID doesn't change
#अगर ( ( CONFIG_FREERTOS_SMP ) && ( !CONFIG_FREERTOS_UNICORE ) )
    vTaskPreemptionDisable(शून्य);
#अन्यथा
    vTaskSuspendAll();
#पूर्ण_अगर
    BaseType_t cur_core_अंक = xPortGetCoreID();
    // Check that the task is unpinned
#अगर !CONFIG_FREERTOS_UNICORE
#अगर CONFIG_FREERTOS_SMP
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, vTaskCoreAffinityGet(शून्य));
#अन्यथा
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, xTaskGetCoreID(शून्य));
#पूर्ण_अगर
#पूर्ण_अगर // !CONFIG_FREERTOS_UNICORE

    /*
    Use the FPU
    - We test using a calculation that will cause a change in mantissa and exponent for extra thoroughness
    - cosf(0.0f) should return 1.0f, thus we are simply doubling test_float every iteration.
    - Therefore, we should end up with (0.01) * (2^8) = 2.56 at the end of the loop
    */
    अस्थिर भग्न test_float = 0.01f;
    क्रम (पूर्णांक i = 0; i < 8; i++) {
        test_float = test_float * 2.0f * cosf(0.0f);
    }
    // We allow a 0.1% delta on the final result in case of any loss of precision from floating point calculations
    TEST_ASSERT_FLOAT_WITHIN(0.00256f, 2.56f, test_float);

#अगर !CONFIG_FREERTOS_UNICORE
#अगर CONFIG_FREERTOS_SMP
    TEST_ASSERT_EQUAL(1 << cur_core_अंक, vTaskCoreAffinityGet(शून्य));
#अन्यथा
    TEST_ASSERT_EQUAL(cur_core_अंक, xTaskGetCoreID(शून्य));
#पूर्ण_अगर
#पूर्ण_अगर // !CONFIG_FREERTOS_UNICORE
    // Re-enable scheduling/preemption
#अगर ( ( CONFIG_FREERTOS_SMP ) && ( !CONFIG_FREERTOS_UNICORE ) )
    vTaskPreemptionEnable(शून्य);
#अन्यथा
    xTaskResumeAll();
#पूर्ण_अगर

    // Indicate done and self delete
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(शून्य);
}

TEST_CASE("FPU: Usage in unpinned task", "[freertos]")
{
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    क्रम (पूर्णांक iter = 0; iter < TEST_UNPINNED_अंक_ITERS; iter++) {
        // Create unpinned task
        xTaskCreate(unpinned_task, "unpin", 4096, (व्योम *)unity_task_handle, UNITY_FREERTOS_PRIORITY + 1, शून्य);
        // Wait for task to complete
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        vTaskDelay(10); // Short delay to allow task memory to be freed
    }
}

प्रकार काष्ठा {
    bool negative;
    TaskHandle_t मुख्य;
} fpu_params_t;

/**
 * @brief Function performing some simple calculation using several FPU registers.
 *        The goal is to be preempted by a task that also uses the FPU on the same core.
 */
व्योम fpu_calculation(व्योम* arg)
{
    fpu_params_t* p = (fpu_params_t*) arg;
    स्थिर bool negative = p->negative;
    स्थिर भग्न init = negative ? -1.f : 1.f;
    भग्न f = init;

    क्रम (पूर्णांक i = 0; i < 10; i++) {
        /* The following calculation doesn't really have any meaning, we try to use several FPU registers and operations */
        भग्न delta = negative ? -1.1f : 1.1f;
        क्रम (पूर्णांक i = 0; i < 1000; i++) {
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
        निश्चित((negative && f < 0.0f) || (!negative && f > 0.0f));
        f = init;

        /* Give the hand back to FreeRTOS to avoid any watchdog */
        vTaskDelay(2);
    }

    xTaskNotifyGive(p->मुख्य);
    vTaskDelete(शून्य);
}

TEST_CASE("FPU: Unsolicited context switch between tasks using FPU", "[freertos]")
{
    /* Create two tasks that are on the same core and use the same FPU */
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    TaskHandle_t tasks[2];
    fpu_params_t params[2] = {
        { .negative = false, .मुख्य = unity_task_handle },
        { .negative = true,  .मुख्य = unity_task_handle },
    };

    xTaskCreatePinnedToCore(fpu_calculation, "Task1", 2048, params + 0, UNITY_FREERTOS_PRIORITY + 1, &tasks[0], 1);
    xTaskCreatePinnedToCore(fpu_calculation, "Task2", 2048, params + 1, UNITY_FREERTOS_PRIORITY + 1, &tasks[1], 1);

    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
}

#पूर्ण_अगर // CONFIG_FREERTOS_NUMBER_OF_CORES > 1
#पूर्ण_अगर // SOC_CPU_HAS_FPU
