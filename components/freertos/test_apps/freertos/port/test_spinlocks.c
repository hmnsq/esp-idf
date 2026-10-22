/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Combined unit tests & benchmarking for spinlock "portMUX" functionality
*/

#അന്തര്ഭാവ <esp_types.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <inttypes.h>
#അന്തര്ഭാവ "sdkconfig.h"
#അന്തര്ഭാവ "freertos/FreeRTOS.h"
#അന്തര്ഭാവ "freertos/task.h"
#അന്തര്ഭാവ "freertos/semphr.h"
#അന്തര്ഭാവ "freertos/queue.h"
#അന്തര്ഭാവ "unity.h"
#അന്തര്ഭാവ "esp_cpu.h"

#അന്തര്ഭാവ "test_utils.h"
#അന്തര്ഭാവ "freertos_performance.h"

#എങ്കിൽ !CONFIG_FREERTOS_SMP    // Known issue in Amazon SMP FreeRTOS port IDF-6204

#define REPEAT_OPS 10000

സ്ഥിതം uint32_t start, end;

#define BENCHMARK_START() ചെയ്യുക {                  \
        start = esp_cpu_get_cycle_എണ്ണം();                     \
    } ആകുമ്പോൾ(0)

#define BENCHMARK_END(OPERATION) ചെയ്യുക {                       \
        end = esp_cpu_get_cycle_എണ്ണം();                                          \
        printf("%s took %"PRIu32" cycles/op (%"PRIu32" cycles for %d ops)\n",     \
               OPERATION, (end - start)/REPEAT_OPS,                 \
               (end - start), REPEAT_OPS);                          \
    } ആകുമ്പോൾ(0)

TEST_CASE("portMUX spinlocks (no contention)", "[freertos]")
{
    portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
    BENCHMARK_START();

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < REPEAT_OPS; i++) {
        portENTER_CRITICAL_ISR(&mux);
        portEXIT_CRITICAL_ISR(&mux);
    }
    BENCHMARK_END("no contention lock");

#എങ്കിൽ CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_UNICORE, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#അല്ലെങ്കിൽ
#എങ്കിൽ CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_PSRAM, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#അല്ലെങ്കിൽ
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#endif
#endif
}

TEST_CASE("portMUX recursive locks (no contention)", "[freertos]")
{
    portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
    BENCHMARK_START();

    സ്ഥിരം പൂർണ്ണസംഖ്യ RECURSE_എണ്ണം = 25;

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < REPEAT_OPS / RECURSE_എണ്ണം; i++) {
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < RECURSE_എണ്ണം; j++) {
            portENTER_CRITICAL(&mux);
        }
        വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < RECURSE_എണ്ണം; j++) {
            portEXIT_CRITICAL(&mux);
        }
    }
    BENCHMARK_END("no contention recursive");

#എങ്കിൽ CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_UNICORE, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#അല്ലെങ്കിൽ
#എങ്കിൽ CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_PSRAM, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#അല്ലെങ്കിൽ
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#endif
#endif
}

#എങ്കിൽ CONFIG_FREERTOS_സംഖ്യ_OF_CORES == 2

സ്ഥിതം അസ്ഥിര പൂർണ്ണസംഖ്യ shared_മൂല്യം;
സ്ഥിതം portMUX_TYPE *shared_mux;
സ്ഥിതം SemaphoreHandle_t done_sem;

സ്ഥിതം ശൂന്യം task_shared_മൂല്യം_increment(ശൂന്യം *ignore)
{
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < REPEAT_OPS; i++) {
        portENTER_CRITICAL(shared_mux);
        shared_മൂല്യം++;
        portEXIT_CRITICAL(shared_mux);
    }
    xSemaphoreGive(done_sem);
    vTaskDelete(ശൂന്യ);
}

TEST_CASE("portMUX cross-core locking", "[freertos]")
{
    shared_mux = heap_caps_malloc(വലുപ്പം(portMUX_TYPE), MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL);
    done_sem = xSemaphoreCreateCounting(2, 0);
    portMUX_INITIALIZE(shared_mux);
    shared_മൂല്യം = 0;

    BENCHMARK_START();

    xTaskCreatePinnedToCore(task_shared_മൂല്യം_increment, "INC0", 2048, ശൂന്യ, UNITY_FREERTOS_PRIORITY + 1, ശൂന്യ, UNITY_FREERTOS_CPU ? 0 : 1);
    xTaskCreatePinnedToCore(task_shared_മൂല്യം_increment, "INC1", 2048, ശൂന്യ, UNITY_FREERTOS_PRIORITY + 1, ശൂന്യ, UNITY_FREERTOS_CPU);

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 2; i++) {
        എങ്കിൽ (!xSemaphoreTake(done_sem, 10000 / portTICK_PERIOD_MS)) {
            TEST_FAIL_MESSAGE("done_sem not released by test task");
        }
    }

    BENCHMARK_END("cross-core incrementing");
    vSemaphoreDelete(done_sem);
    സ്വതന്ത്രം(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * 2, shared_മൂല്യം);
}

ശൂന്യം portmux_മുകളിലെ_contention_test(uint32_t lock_malloc_caps)
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ TOTAL_TASKS = 8; /* half on each core */
    shared_mux = heap_caps_malloc(വലുപ്പം(portMUX_TYPE), lock_malloc_caps);
    done_sem = xSemaphoreCreateCounting(TOTAL_TASKS, 0);
    portMUX_INITIALIZE(shared_mux);
    shared_മൂല്യം = 0;

    BENCHMARK_START();

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < TOTAL_TASKS / 2; i++) {
        /* as each task has a higher priority than previous, expect
           them to preempt the earlier created task, at least on the
           other core (this core has the unity task, until that
           blocks)... */
        xTaskCreatePinnedToCore(task_shared_മൂല്യം_increment, "INC0", 2048, ശൂന്യ, tskIDLE_PRIORITY + 1 + i, ശൂന്യ, UNITY_FREERTOS_CPU ? 0 : 1);
        xTaskCreatePinnedToCore(task_shared_മൂല്യം_increment, "INC1", 2048, ശൂന്യ, tskIDLE_PRIORITY + 1 + i, ശൂന്യ, UNITY_FREERTOS_CPU);
    }

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < TOTAL_TASKS; i++) {
        എങ്കിൽ (!xSemaphoreTake(done_sem, 10000 / portTICK_PERIOD_MS)) {
            TEST_FAIL_MESSAGE("done_sem not released by test task");
        }
    }

    BENCHMARK_END("cross-core high contention");
    vSemaphoreDelete(done_sem);
    സ്വതന്ത്രം(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * TOTAL_TASKS, shared_മൂല്യം);
}

TEST_CASE("portMUX high contention", "[freertos]")
{
    portmux_മുകളിലെ_contention_test(MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL);
}

#എങ്കിൽ CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC
TEST_CASE("portMUX high contention, PSRAM", "[freertos]")
{
    portmux_മുകളിലെ_contention_test(MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);
}
#endif// CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC

#endif // CONFIG_FREERTOS_NUMBER_OF_CORES == 2

#endif // !CONFIG_FREERTOS_SMP
