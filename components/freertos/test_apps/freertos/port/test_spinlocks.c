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

#ಯದಿ !CONFIG_FREERTOS_SMP    // Known issue in Amazon SMP FreeRTOS port IDF-6204

#define REPEAT_OPS 10000

ಸ್ಥಿರ uint32_t start, end;

#define BENCHMARK_START() ಕುರು {                  \
        start = esp_cpu_get_cycle_ಎಣಿಕೆ();                     \
    } ಯಾವತ್(0)

#define BENCHMARK_END(OPERATION) ಕುರು {                       \
        end = esp_cpu_get_cycle_ಎಣಿಕೆ();                                          \
        printf("%s took %"PRIu32" cycles/op (%"PRIu32" cycles for %d ops)\n",     \
               OPERATION, (end - start)/REPEAT_OPS,                 \
               (end - start), REPEAT_OPS);                          \
    } ಯಾವತ್(0)

TEST_CASE("portMUX spinlocks (no contention)", "[freertos]")
{
    portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
    BENCHMARK_START();

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < REPEAT_OPS; i++) {
        portENTER_CRITICAL_ISR(&mux);
        portEXIT_CRITICAL_ISR(&mux);
    }
    BENCHMARK_END("no contention lock");

#ಯದಿ CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_UNICORE, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#ಅನ್ಯಥಾ
#ಯದಿ CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_PSRAM, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#ಅನ್ಯಥಾ
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#endif
#endif
}

TEST_CASE("portMUX recursive locks (no contention)", "[freertos]")
{
    portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
    BENCHMARK_START();

    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ RECURSE_ಎಣಿಕೆ = 25;

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < REPEAT_OPS / RECURSE_ಎಣಿಕೆ; i++) {
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < RECURSE_ಎಣಿಕೆ; j++) {
            portENTER_CRITICAL(&mux);
        }
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < RECURSE_ಎಣಿಕೆ; j++) {
            portEXIT_CRITICAL(&mux);
        }
    }
    BENCHMARK_END("no contention recursive");

#ಯದಿ CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_UNICORE, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#ಅನ್ಯಥಾ
#ಯದಿ CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_PSRAM, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#ಅನ್ಯಥಾ
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#endif
#endif
}

#ಯದಿ CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES == 2

ಸ್ಥಿರ ಚಞ್ಚಲ ಪೂರ್ಣಾಙ್ಕ shared_ಮೌಲ್ಯ;
ಸ್ಥಿರ portMUX_TYPE *shared_mux;
ಸ್ಥಿರ SemaphoreHandle_t done_sem;

ಸ್ಥಿರ ಶೂನ್ಯ task_shared_ಮೌಲ್ಯ_increment(ಶೂನ್ಯ *ignore)
{
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < REPEAT_OPS; i++) {
        portENTER_CRITICAL(shared_mux);
        shared_ಮೌಲ್ಯ++;
        portEXIT_CRITICAL(shared_mux);
    }
    xSemaphoreGive(done_sem);
    vTaskDelete(ರಿಕ್ತ);
}

TEST_CASE("portMUX cross-core locking", "[freertos]")
{
    shared_mux = heap_caps_malloc(ಪರಿಮಾಣ(portMUX_TYPE), MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL);
    done_sem = xSemaphoreCreateCounting(2, 0);
    portMUX_INITIALIZE(shared_mux);
    shared_ಮೌಲ್ಯ = 0;

    BENCHMARK_START();

    xTaskCreatePinnedToCore(task_shared_ಮೌಲ್ಯ_increment, "INC0", 2048, ರಿಕ್ತ, UNITY_FREERTOS_PRIORITY + 1, ರಿಕ್ತ, UNITY_FREERTOS_CPU ? 0 : 1);
    xTaskCreatePinnedToCore(task_shared_ಮೌಲ್ಯ_increment, "INC1", 2048, ರಿಕ್ತ, UNITY_FREERTOS_PRIORITY + 1, ರಿಕ್ತ, UNITY_FREERTOS_CPU);

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < 2; i++) {
        ಯದಿ (!xSemaphoreTake(done_sem, 10000 / portTICK_PERIOD_MS)) {
            TEST_FAIL_MESSAGE("done_sem not released by test task");
        }
    }

    BENCHMARK_END("cross-core incrementing");
    vSemaphoreDelete(done_sem);
    ಮುಕ್ತ(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * 2, shared_ಮೌಲ್ಯ);
}

ಶೂನ್ಯ portmux_ಮೇಲಿನ_contention_test(uint32_t lock_malloc_caps)
{
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ TOTAL_TASKS = 8; /* half on each core */
    shared_mux = heap_caps_malloc(ಪರಿಮಾಣ(portMUX_TYPE), lock_malloc_caps);
    done_sem = xSemaphoreCreateCounting(TOTAL_TASKS, 0);
    portMUX_INITIALIZE(shared_mux);
    shared_ಮೌಲ್ಯ = 0;

    BENCHMARK_START();

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < TOTAL_TASKS / 2; i++) {
        /* as each task has a higher priority than previous, expect
           them to preempt the earlier created task, at least on the
           other core (this core has the unity task, until that
           blocks)... */
        xTaskCreatePinnedToCore(task_shared_ಮೌಲ್ಯ_increment, "INC0", 2048, ರಿಕ್ತ, tskIDLE_PRIORITY + 1 + i, ರಿಕ್ತ, UNITY_FREERTOS_CPU ? 0 : 1);
        xTaskCreatePinnedToCore(task_shared_ಮೌಲ್ಯ_increment, "INC1", 2048, ರಿಕ್ತ, tskIDLE_PRIORITY + 1 + i, ರಿಕ್ತ, UNITY_FREERTOS_CPU);
    }

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < TOTAL_TASKS; i++) {
        ಯದಿ (!xSemaphoreTake(done_sem, 10000 / portTICK_PERIOD_MS)) {
            TEST_FAIL_MESSAGE("done_sem not released by test task");
        }
    }

    BENCHMARK_END("cross-core high contention");
    vSemaphoreDelete(done_sem);
    ಮುಕ್ತ(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * TOTAL_TASKS, shared_ಮೌಲ್ಯ);
}

TEST_CASE("portMUX high contention", "[freertos]")
{
    portmux_ಮೇಲಿನ_contention_test(MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL);
}

#ಯದಿ CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC
TEST_CASE("portMUX high contention, PSRAM", "[freertos]")
{
    portmux_ಮೇಲಿನ_contention_test(MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);
}
#endif// CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC

#endif // CONFIG_FREERTOS_NUMBER_OF_CORES == 2

#endif // !CONFIG_FREERTOS_SMP
