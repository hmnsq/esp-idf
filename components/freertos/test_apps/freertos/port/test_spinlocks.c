/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Combined unit tests & benchmarking for spinlock "portMUX" functionality
*/

#ਅਨ੍ਤਰ੍ਭਾਵ <esp_types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <inttypes.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/semphr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/queue.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_cpu.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "test_utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos_performance.h"

#ਜੇ !CONFIG_FREERTOS_SMP    // Known issue in Amazon SMP FreeRTOS port IDF-6204

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ REPEAT_OPS 10000

ਸਥਿਰ uint32_t ਸ਼ੁਰੂ, ਸਿਰਾ;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BENCHMARK_ਸ਼ੁਰੂ() ਕਰੋ {                  \
        ਸ਼ੁਰੂ = esp_cpu_get_ਚੱਕਰ_ਗਿਣਤੀ();                     \
    } ਜਦੋਂ(0)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BENCHMARK_ਸਿਰਾ(OPERATION) ਕਰੋ {                       \
        ਸਿਰਾ = esp_cpu_get_ਚੱਕਰ_ਗਿਣਤੀ();                                          \
        ਲਿਖੋ("%s took %"PRIu32" cycles/op (%"PRIu32" cycles for %d ops)\n",     \
               OPERATION, (ਸਿਰਾ - ਸ਼ੁਰੂ)/REPEAT_OPS,                 \
               (ਸਿਰਾ - ਸ਼ੁਰੂ), REPEAT_OPS);                          \
    } ਜਦੋਂ(0)

TEST_CASE("portMUX spinlocks (no contention)", "[freertos]")
{
    ਪੋਰਟ_MUX_TYPE mux = ਪੋਰਟ_MUX_INITIALIZER_UNLOCKED;
    BENCHMARK_ਸ਼ੁਰੂ();

    ਲਈ (ਪੂਰਨ i = 0; i < REPEAT_OPS; i++) {
        ਪੋਰਟ_ENTER_CRITICAL_ISR(&mux);
        ਪੋਰਟ_EXIT_CRITICAL_ISR(&mux);
    }
    BENCHMARK_ਸਿਰਾ("no contention lock");

#ਜੇ CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_ਚੱਕਰਾਂ_ਪ੍ਰਤੀ_OP_UNICORE, "%"PRIu32" cycles/op", ((ਸਿਰਾ - ਸ਼ੁਰੂ) / REPEAT_OPS));
#ਨਹੀਂਤਾਂ
#ਜੇ CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_ਚੱਕਰਾਂ_ਪ੍ਰਤੀ_OP_PSRAM, "%"PRIu32" cycles/op", ((ਸਿਰਾ - ਸ਼ੁਰੂ) / REPEAT_OPS));
#ਨਹੀਂਤਾਂ
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_ਚੱਕਰਾਂ_ਪ੍ਰਤੀ_OP, "%"PRIu32" cycles/op", ((ਸਿਰਾ - ਸ਼ੁਰੂ) / REPEAT_OPS));
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
}

TEST_CASE("portMUX recursive locks (no contention)", "[freertos]")
{
    ਪੋਰਟ_MUX_TYPE mux = ਪੋਰਟ_MUX_INITIALIZER_UNLOCKED;
    BENCHMARK_ਸ਼ੁਰੂ();

    ਅਚਲ ਪੂਰਨ RECURSE_ਗਿਣਤੀ = 25;

    ਲਈ (ਪੂਰਨ i = 0; i < REPEAT_OPS / RECURSE_ਗਿਣਤੀ; i++) {
        ਲਈ (ਪੂਰਨ j = 0; j < RECURSE_ਗਿਣਤੀ; j++) {
            ਪੋਰਟ_ENTER_CRITICAL(&mux);
        }
        ਲਈ (ਪੂਰਨ j = 0; j < RECURSE_ਗਿਣਤੀ; j++) {
            ਪੋਰਟ_EXIT_CRITICAL(&mux);
        }
    }
    BENCHMARK_ਸਿਰਾ("no contention recursive");

#ਜੇ CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_ਚੱਕਰਾਂ_ਪ੍ਰਤੀ_OP_UNICORE, "%"PRIu32" cycles/op", ((ਸਿਰਾ - ਸ਼ੁਰੂ) / REPEAT_OPS));
#ਨਹੀਂਤਾਂ
#ਜੇ CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_ਚੱਕਰਾਂ_ਪ੍ਰਤੀ_OP_PSRAM, "%"PRIu32" cycles/op", ((ਸਿਰਾ - ਸ਼ੁਰੂ) / REPEAT_OPS));
#ਨਹੀਂਤਾਂ
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_ਚੱਕਰਾਂ_ਪ੍ਰਤੀ_OP, "%"PRIu32" cycles/op", ((ਸਿਰਾ - ਸ਼ੁਰੂ) / REPEAT_OPS));
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
}

#ਜੇ CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES == 2

ਸਥਿਰ ਅਸਥਿਰ ਪੂਰਨ shared_ਮੁੱਲ;
ਸਥਿਰ ਪੋਰਟ_MUX_TYPE *shared_mux;
ਸਥਿਰ SemaphoreHandle_t ਪੂਰਾ_sem;

ਸਥਿਰ ਖਾਲੀ task_shared_ਮੁੱਲ_increment(ਖਾਲੀ *ignore)
{
    ਲਈ (ਪੂਰਨ i = 0; i < REPEAT_OPS; i++) {
        ਪੋਰਟ_ENTER_CRITICAL(shared_mux);
        shared_ਮੁੱਲ++;
        ਪੋਰਟ_EXIT_CRITICAL(shared_mux);
    }
    xSemaphoreGive(ਪੂਰਾ_sem);
    vTaskDelete(ਨਿਰਾ);
}

TEST_CASE("portMUX cross-core locking", "[freertos]")
{
    shared_mux = ਢੇਰ_caps_malloc(ਆਕਾਰ(ਪੋਰਟ_MUX_TYPE), MALLOC_CAP_8_ਦੋਆਧਾਰੀ_ਅੰਕ | MALLOC_CAP_ਅੰਦਰੂਨੀ);
    ਪੂਰਾ_sem = xSemaphoreCreateCounting(2, 0);
    ਪੋਰਟ_MUX_INITIALIZE(shared_mux);
    shared_ਮੁੱਲ = 0;

    BENCHMARK_ਸ਼ੁਰੂ();

    x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(task_shared_ਮੁੱਲ_increment, "INC0", 2048, ਨਿਰਾ, UNITY_FREERTOS_ਤਰਜੀਹ + 1, ਨਿਰਾ, UNITY_FREERTOS_CPU ? 0 : 1);
    x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(task_shared_ਮੁੱਲ_increment, "INC1", 2048, ਨਿਰਾ, UNITY_FREERTOS_ਤਰਜੀਹ + 1, ਨਿਰਾ, UNITY_FREERTOS_CPU);

    ਲਈ (ਪੂਰਨ i = 0; i < 2; i++) {
        ਜੇ (!xSemaphoreTake(ਪੂਰਾ_sem, 10000 / ਪੋਰਟ_TICK_PERIOD_MS)) {
            TEST_FAIL_ਸੁਨੇਹਾ("done_sem not released by test task");
        }
    }

    BENCHMARK_ਸਿਰਾ("cross-core incrementing");
    vSemaphoreDelete(ਪੂਰਾ_sem);
    ਮੁਕਤ(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * 2, shared_ਮੁੱਲ);
}

ਖਾਲੀ portmux_ਉੱਚਾ_contention_test(uint32_t ਤਾਲਾ_malloc_caps)
{
    ਅਚਲ ਪੂਰਨ ਕੁੱਲ_TASKS = 8; /* half on each core */
    shared_mux = ਢੇਰ_caps_malloc(ਆਕਾਰ(ਪੋਰਟ_MUX_TYPE), ਤਾਲਾ_malloc_caps);
    ਪੂਰਾ_sem = xSemaphoreCreateCounting(ਕੁੱਲ_TASKS, 0);
    ਪੋਰਟ_MUX_INITIALIZE(shared_mux);
    shared_ਮੁੱਲ = 0;

    BENCHMARK_ਸ਼ੁਰੂ();

    ਲਈ (ਪੂਰਨ i = 0; i < ਕੁੱਲ_TASKS / 2; i++) {
        /* as each task has a higher priority than previous, expect
           them to preempt the earlier created task, at least on the
           other core (this core has the unity task, until that
           blocks)... */
        x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(task_shared_ਮੁੱਲ_increment, "INC0", 2048, ਨਿਰਾ, tsk_IDLE_ਤਰਜੀਹ + 1 + i, ਨਿਰਾ, UNITY_FREERTOS_CPU ? 0 : 1);
        x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(task_shared_ਮੁੱਲ_increment, "INC1", 2048, ਨਿਰਾ, tsk_IDLE_ਤਰਜੀਹ + 1 + i, ਨਿਰਾ, UNITY_FREERTOS_CPU);
    }

    ਲਈ (ਪੂਰਨ i = 0; i < ਕੁੱਲ_TASKS; i++) {
        ਜੇ (!xSemaphoreTake(ਪੂਰਾ_sem, 10000 / ਪੋਰਟ_TICK_PERIOD_MS)) {
            TEST_FAIL_ਸੁਨੇਹਾ("done_sem not released by test task");
        }
    }

    BENCHMARK_ਸਿਰਾ("cross-core high contention");
    vSemaphoreDelete(ਪੂਰਾ_sem);
    ਮੁਕਤ(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * ਕੁੱਲ_TASKS, shared_ਮੁੱਲ);
}

TEST_CASE("portMUX high contention", "[freertos]")
{
    portmux_ਉੱਚਾ_contention_test(MALLOC_CAP_8_ਦੋਆਧਾਰੀ_ਅੰਕ | MALLOC_CAP_ਅੰਦਰੂਨੀ);
}

#ਜੇ CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC
TEST_CASE("portMUX high contention, PSRAM", "[freertos]")
{
    portmux_ਉੱਚਾ_contention_test(MALLOC_CAP_8_ਦੋਆਧਾਰੀ_ਅੰਕ | MALLOC_CAP_SPIRAM);
}
#ਜੇ_ਅੰਤ// CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC

#ਜੇ_ਅੰਤ // CONFIG_FREERTOS_NUMBER_OF_CORES == 2

#ਜੇ_ਅੰਤ // !CONFIG_FREERTOS_SMP
