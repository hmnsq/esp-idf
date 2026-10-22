/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Combined unit tests & benchmarking for spinlock "portMUX" functionality
*/

#ಅನ್ತರ್ಭಾವ <esp_types.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <inttypes.h>
#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "freertos/semphr.h"
#ಅನ್ತರ್ಭಾವ "freertos/queue.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "esp_cpu.h"

#ಅನ್ತರ್ಭಾವ "test_utils.h"
#ಅನ್ತರ್ಭಾವ "freertos_performance.h"

#ಆದರೆ !CONFIG_FREERTOS_SMP    // Known issue in Amazon SMP FreeRTOS port IDF-6204

#ವ್ಯಾಖ್ಯಾನಿಸು REPEAT_OPS 10000

ಸ್ಥಿರ uint32_t ಆರಂಭ, ಕೊನೆ;

#ವ್ಯಾಖ್ಯಾನಿಸು BENCHMARK_ಆರಂಭ() ಮಾಡು {                  \
        ಆರಂಭ = esp_cpu_get_ಚಕ್ರ_ಎಣಿಕೆ();                     \
    } ಆಗ(0)

#ವ್ಯಾಖ್ಯಾನಿಸು BENCHMARK_ಕೊನೆ(OPERATION) ಮಾಡು {                       \
        ಕೊನೆ = esp_cpu_get_ಚಕ್ರ_ಎಣಿಕೆ();                                          \
        ಬರೆ("%s took %"PRIu32" cycles/op (%"PRIu32" cycles for %d ops)\n",     \
               OPERATION, (ಕೊನೆ - ಆರಂಭ)/REPEAT_OPS,                 \
               (ಕೊನೆ - ಆರಂಭ), REPEAT_OPS);                          \
    } ಆಗ(0)

TEST_CASE("portMUX spinlocks (no contention)", "[freertos]")
{
    ಪೋರ್ಟ್_MUX_TYPE mux = ಪೋರ್ಟ್_MUX_INITIALIZER_UNLOCKED;
    BENCHMARK_ಆರಂಭ();

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < REPEAT_OPS; i++) {
        ಪೋರ್ಟ್_ENTER_CRITICAL_ISR(&mux);
        ಪೋರ್ಟ್_EXIT_CRITICAL_ISR(&mux);
    }
    BENCHMARK_ಕೊನೆ("no contention lock");

#ಆದರೆ CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_ಚಕ್ರಗಳು_ಪ್ರತಿ_OP_UNICORE, "%"PRIu32" cycles/op", ((ಕೊನೆ - ಆರಂಭ) / REPEAT_OPS));
#ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_ಚಕ್ರಗಳು_ಪ್ರತಿ_OP_PSRAM, "%"PRIu32" cycles/op", ((ಕೊನೆ - ಆರಂಭ) / REPEAT_OPS));
#ಇಲ್ಲದಿದ್ದರೆ
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_ಚಕ್ರಗಳು_ಪ್ರತಿ_OP, "%"PRIu32" cycles/op", ((ಕೊನೆ - ಆರಂಭ) / REPEAT_OPS));
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
}

TEST_CASE("portMUX recursive locks (no contention)", "[freertos]")
{
    ಪೋರ್ಟ್_MUX_TYPE mux = ಪೋರ್ಟ್_MUX_INITIALIZER_UNLOCKED;
    BENCHMARK_ಆರಂಭ();

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ RECURSE_ಎಣಿಕೆ = 25;

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < REPEAT_OPS / RECURSE_ಎಣಿಕೆ; i++) {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < RECURSE_ಎಣಿಕೆ; j++) {
            ಪೋರ್ಟ್_ENTER_CRITICAL(&mux);
        }
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < RECURSE_ಎಣಿಕೆ; j++) {
            ಪೋರ್ಟ್_EXIT_CRITICAL(&mux);
        }
    }
    BENCHMARK_ಕೊನೆ("no contention recursive");

#ಆದರೆ CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_ಚಕ್ರಗಳು_ಪ್ರತಿ_OP_UNICORE, "%"PRIu32" cycles/op", ((ಕೊನೆ - ಆರಂಭ) / REPEAT_OPS));
#ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_ಚಕ್ರಗಳು_ಪ್ರತಿ_OP_PSRAM, "%"PRIu32" cycles/op", ((ಕೊನೆ - ಆರಂಭ) / REPEAT_OPS));
#ಇಲ್ಲದಿದ್ದರೆ
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_ಚಕ್ರಗಳು_ಪ್ರತಿ_OP, "%"PRIu32" cycles/op", ((ಕೊನೆ - ಆರಂಭ) / REPEAT_OPS));
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
}

#ಆದರೆ CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES == 2

ಸ್ಥಿರ ಅಸ್ಥಿರ ಪೂರ್ಣಾಂಕ shared_ಮೌಲ್ಯ;
ಸ್ಥಿರ ಪೋರ್ಟ್_MUX_TYPE *shared_mux;
ಸ್ಥಿರ SemaphoreHandle_t ಮುಗಿದ_sem;

ಸ್ಥಿರ ಶೂನ್ಯ task_shared_ಮೌಲ್ಯ_increment(ಶೂನ್ಯ *ignore)
{
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < REPEAT_OPS; i++) {
        ಪೋರ್ಟ್_ENTER_CRITICAL(shared_mux);
        shared_ಮೌಲ್ಯ++;
        ಪೋರ್ಟ್_EXIT_CRITICAL(shared_mux);
    }
    xSemaphoreGive(ಮುಗಿದ_sem);
    vTaskDelete(ಖಾಲಿ);
}

TEST_CASE("portMUX cross-core locking", "[freertos]")
{
    shared_mux = ರಾಶಿ_caps_malloc(ಗಾತ್ರ(ಪೋರ್ಟ್_MUX_TYPE), MALLOC_CAP_8_ಬಿಟ್ | MALLOC_CAP_ಆಂತರಿಕ);
    ಮುಗಿದ_sem = xSemaphoreCreateCounting(2, 0);
    ಪೋರ್ಟ್_MUX_INITIALIZE(shared_mux);
    shared_ಮೌಲ್ಯ = 0;

    BENCHMARK_ಆರಂಭ();

    x_Task_Create_Pinned_ಗಮ್ಯ_Core(task_shared_ಮೌಲ್ಯ_increment, "INC0", 2048, ಖಾಲಿ, UNITY_FREERTOS_ಆದ್ಯತೆ + 1, ಖಾಲಿ, UNITY_FREERTOS_CPU ? 0 : 1);
    x_Task_Create_Pinned_ಗಮ್ಯ_Core(task_shared_ಮೌಲ್ಯ_increment, "INC1", 2048, ಖಾಲಿ, UNITY_FREERTOS_ಆದ್ಯತೆ + 1, ಖಾಲಿ, UNITY_FREERTOS_CPU);

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 2; i++) {
        ಆದರೆ (!xSemaphoreTake(ಮುಗಿದ_sem, 10000 / ಪೋರ್ಟ್_TICK_PERIOD_MS)) {
            TEST_FAIL_ಸಂದೇಶ("done_sem not released by test task");
        }
    }

    BENCHMARK_ಕೊನೆ("cross-core incrementing");
    vSemaphoreDelete(ಮುಗಿದ_sem);
    ಬಿಡುಗಡೆ(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * 2, shared_ಮೌಲ್ಯ);
}

ಶೂನ್ಯ portmux_ಮೇಲಿನ_contention_test(uint32_t ಬೀಗ_malloc_caps)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಒಟ್ಟು_TASKS = 8; /* half on each core */
    shared_mux = ರಾಶಿ_caps_malloc(ಗಾತ್ರ(ಪೋರ್ಟ್_MUX_TYPE), ಬೀಗ_malloc_caps);
    ಮುಗಿದ_sem = xSemaphoreCreateCounting(ಒಟ್ಟು_TASKS, 0);
    ಪೋರ್ಟ್_MUX_INITIALIZE(shared_mux);
    shared_ಮೌಲ್ಯ = 0;

    BENCHMARK_ಆರಂಭ();

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಒಟ್ಟು_TASKS / 2; i++) {
        /* as each task has a higher priority than previous, expect
           them to preempt the earlier created task, at least on the
           other core (this core has the unity task, until that
           blocks)... */
        x_Task_Create_Pinned_ಗಮ್ಯ_Core(task_shared_ಮೌಲ್ಯ_increment, "INC0", 2048, ಖಾಲಿ, tsk_IDLE_ಆದ್ಯತೆ + 1 + i, ಖಾಲಿ, UNITY_FREERTOS_CPU ? 0 : 1);
        x_Task_Create_Pinned_ಗಮ್ಯ_Core(task_shared_ಮೌಲ್ಯ_increment, "INC1", 2048, ಖಾಲಿ, tsk_IDLE_ಆದ್ಯತೆ + 1 + i, ಖಾಲಿ, UNITY_FREERTOS_CPU);
    }

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಒಟ್ಟು_TASKS; i++) {
        ಆದರೆ (!xSemaphoreTake(ಮುಗಿದ_sem, 10000 / ಪೋರ್ಟ್_TICK_PERIOD_MS)) {
            TEST_FAIL_ಸಂದೇಶ("done_sem not released by test task");
        }
    }

    BENCHMARK_ಕೊನೆ("cross-core high contention");
    vSemaphoreDelete(ಮುಗಿದ_sem);
    ಬಿಡುಗಡೆ(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * ಒಟ್ಟು_TASKS, shared_ಮೌಲ್ಯ);
}

TEST_CASE("portMUX high contention", "[freertos]")
{
    portmux_ಮೇಲಿನ_contention_test(MALLOC_CAP_8_ಬಿಟ್ | MALLOC_CAP_ಆಂತರಿಕ);
}

#ಆದರೆ CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC
TEST_CASE("portMUX high contention, PSRAM", "[freertos]")
{
    portmux_ಮೇಲಿನ_contention_test(MALLOC_CAP_8_ಬಿಟ್ | MALLOC_CAP_SPIRAM);
}
#ಆದರೆ_ಅಂತ್ಯ// CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC

#ಆದರೆ_ಅಂತ್ಯ // CONFIG_FREERTOS_NUMBER_OF_CORES == 2

#ಆದರೆ_ಅಂತ್ಯ // !CONFIG_FREERTOS_SMP
