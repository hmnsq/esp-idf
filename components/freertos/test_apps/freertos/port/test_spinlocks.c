/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Combined unit tests & benchmarking for spinlock "portMUX" functionality
*/

#অন্তর্ভাব <esp_types.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <inttypes.h>
#অন্তর্ভাব "sdkconfig.h"
#অন্তর্ভাব "freertos/FreeRTOS.h"
#অন্তর্ভাব "freertos/task.h"
#অন্তর্ভাব "freertos/semphr.h"
#অন্তর্ভাব "freertos/queue.h"
#অন্তর্ভাব "unity.h"
#অন্তর্ভাব "esp_cpu.h"

#অন্তর্ভাব "test_utils.h"
#অন্তর্ভাব "freertos_performance.h"

#যদি !CONFIG_FREERTOS_SMP    // Known issue in Amazon SMP FreeRTOS port IDF-6204

#define REPEAT_OPS 10000

স্থির uint32_t start, end;

#define BENCHMARK_START() করো {                  \
        start = esp_cpu_get_cycle_গুনতি();                     \
    } যখন(0)

#define BENCHMARK_END(OPERATION) করো {                       \
        end = esp_cpu_get_cycle_গুনতি();                                          \
        printf("%s took %"PRIu32" cycles/op (%"PRIu32" cycles for %d ops)\n",     \
               OPERATION, (end - start)/REPEAT_OPS,                 \
               (end - start), REPEAT_OPS);                          \
    } যখন(0)

TEST_CASE("portMUX spinlocks (no contention)", "[freertos]")
{
    portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
    BENCHMARK_START();

    জন্য (পূর্ণসংখ্যা i = 0; i < REPEAT_OPS; i++) {
        portENTER_CRITICAL_ISR(&mux);
        portEXIT_CRITICAL_ISR(&mux);
    }
    BENCHMARK_END("no contention lock");

#যদি CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_UNICORE, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#অন্যথা
#যদি CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_PSRAM, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#অন্যথা
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#endif
#endif
}

TEST_CASE("portMUX recursive locks (no contention)", "[freertos]")
{
    portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
    BENCHMARK_START();

    ধ্রুবক পূর্ণসংখ্যা RECURSE_গুনতি = 25;

    জন্য (পূর্ণসংখ্যা i = 0; i < REPEAT_OPS / RECURSE_গুনতি; i++) {
        জন্য (পূর্ণসংখ্যা j = 0; j < RECURSE_গুনতি; j++) {
            portENTER_CRITICAL(&mux);
        }
        জন্য (পূর্ণসংখ্যা j = 0; j < RECURSE_গুনতি; j++) {
            portEXIT_CRITICAL(&mux);
        }
    }
    BENCHMARK_END("no contention recursive");

#যদি CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_UNICORE, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#অন্যথা
#যদি CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_PSRAM, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#অন্যথা
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#endif
#endif
}

#যদি CONFIG_FREERTOS_সংখ্যা_OF_CORES == 2

স্থির অস্থির পূর্ণসংখ্যা shared_মান;
স্থির portMUX_TYPE *shared_mux;
স্থির SemaphoreHandle_t done_sem;

স্থির শূন্য task_shared_মান_increment(শূন্য *ignore)
{
    জন্য (পূর্ণসংখ্যা i = 0; i < REPEAT_OPS; i++) {
        portENTER_CRITICAL(shared_mux);
        shared_মান++;
        portEXIT_CRITICAL(shared_mux);
    }
    xSemaphoreGive(done_sem);
    vTaskDelete(ফাঁকা);
}

TEST_CASE("portMUX cross-core locking", "[freertos]")
{
    shared_mux = heap_caps_malloc(আকার(portMUX_TYPE), MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL);
    done_sem = xSemaphoreCreateCounting(2, 0);
    portMUX_INITIALIZE(shared_mux);
    shared_মান = 0;

    BENCHMARK_START();

    xTaskCreatePinnedToCore(task_shared_মান_increment, "INC0", 2048, ফাঁকা, UNITY_FREERTOS_PRIORITY + 1, ফাঁকা, UNITY_FREERTOS_CPU ? 0 : 1);
    xTaskCreatePinnedToCore(task_shared_মান_increment, "INC1", 2048, ফাঁকা, UNITY_FREERTOS_PRIORITY + 1, ফাঁকা, UNITY_FREERTOS_CPU);

    জন্য (পূর্ণসংখ্যা i = 0; i < 2; i++) {
        যদি (!xSemaphoreTake(done_sem, 10000 / portTICK_PERIOD_MS)) {
            TEST_FAIL_MESSAGE("done_sem not released by test task");
        }
    }

    BENCHMARK_END("cross-core incrementing");
    vSemaphoreDelete(done_sem);
    মুক্ত(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * 2, shared_মান);
}

শূন্য portmux_উপরের_contention_test(uint32_t lock_malloc_caps)
{
    ধ্রুবক পূর্ণসংখ্যা যোগ_TASKS = 8; /* half on each core */
    shared_mux = heap_caps_malloc(আকার(portMUX_TYPE), lock_malloc_caps);
    done_sem = xSemaphoreCreateCounting(যোগ_TASKS, 0);
    portMUX_INITIALIZE(shared_mux);
    shared_মান = 0;

    BENCHMARK_START();

    জন্য (পূর্ণসংখ্যা i = 0; i < যোগ_TASKS / 2; i++) {
        /* as each task has a higher priority than previous, expect
           them to preempt the earlier created task, at least on the
           other core (this core has the unity task, until that
           blocks)... */
        xTaskCreatePinnedToCore(task_shared_মান_increment, "INC0", 2048, ফাঁকা, tskIDLE_PRIORITY + 1 + i, ফাঁকা, UNITY_FREERTOS_CPU ? 0 : 1);
        xTaskCreatePinnedToCore(task_shared_মান_increment, "INC1", 2048, ফাঁকা, tskIDLE_PRIORITY + 1 + i, ফাঁকা, UNITY_FREERTOS_CPU);
    }

    জন্য (পূর্ণসংখ্যা i = 0; i < যোগ_TASKS; i++) {
        যদি (!xSemaphoreTake(done_sem, 10000 / portTICK_PERIOD_MS)) {
            TEST_FAIL_MESSAGE("done_sem not released by test task");
        }
    }

    BENCHMARK_END("cross-core high contention");
    vSemaphoreDelete(done_sem);
    মুক্ত(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * যোগ_TASKS, shared_মান);
}

TEST_CASE("portMUX high contention", "[freertos]")
{
    portmux_উপরের_contention_test(MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL);
}

#যদি CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC
TEST_CASE("portMUX high contention, PSRAM", "[freertos]")
{
    portmux_উপরের_contention_test(MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);
}
#endif// CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC

#endif // CONFIG_FREERTOS_NUMBER_OF_CORES == 2

#endif // !CONFIG_FREERTOS_SMP
