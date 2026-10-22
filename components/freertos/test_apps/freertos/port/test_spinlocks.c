/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Combined unit tests & benchmarking for spinlock "portMUX" functionality
*/

#அந்தர்பாவ <esp_types.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <inttypes.h>
#அந்தர்பாவ "sdkconfig.h"
#அந்தர்பாவ "freertos/FreeRTOS.h"
#அந்தர்பாவ "freertos/task.h"
#அந்தர்பாவ "freertos/semphr.h"
#அந்தர்பாவ "freertos/queue.h"
#அந்தர்பாவ "unity.h"
#அந்தர்பாவ "esp_cpu.h"

#அந்தர்பாவ "test_utils.h"
#அந்தர்பாவ "freertos_performance.h"

#எனில் !CONFIG_FREERTOS_SMP    // Known issue in Amazon SMP FreeRTOS port IDF-6204

#define REPEAT_OPS 10000

நிலையான uint32_t start, end;

#define BENCHMARK_START() செய் {                  \
        start = esp_cpu_get_cycle_மொத்தம்();                     \
    } போது(0)

#define BENCHMARK_END(OPERATION) செய் {                       \
        end = esp_cpu_get_cycle_மொத்தம்();                                          \
        printf("%s took %"PRIu32" cycles/op (%"PRIu32" cycles for %d ops)\n",     \
               OPERATION, (end - start)/REPEAT_OPS,                 \
               (end - start), REPEAT_OPS);                          \
    } போது(0)

TEST_CASE("portMUX spinlocks (no contention)", "[freertos]")
{
    portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
    BENCHMARK_START();

    க்கு (முழு i = 0; i < REPEAT_OPS; i++) {
        portENTER_CRITICAL_ISR(&mux);
        portEXIT_CRITICAL_ISR(&mux);
    }
    BENCHMARK_END("no contention lock");

#எனில் CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_UNICORE, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#இல்லையெனில்
#எனில் CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_PSRAM, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#இல்லையெனில்
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#endif
#endif
}

TEST_CASE("portMUX recursive locks (no contention)", "[freertos]")
{
    portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
    BENCHMARK_START();

    மாறிலி முழு RECURSE_மொத்தம் = 25;

    க்கு (முழு i = 0; i < REPEAT_OPS / RECURSE_மொத்தம்; i++) {
        க்கு (முழு j = 0; j < RECURSE_மொத்தம்; j++) {
            portENTER_CRITICAL(&mux);
        }
        க்கு (முழு j = 0; j < RECURSE_மொத்தம்; j++) {
            portEXIT_CRITICAL(&mux);
        }
    }
    BENCHMARK_END("no contention recursive");

#எனில் CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_UNICORE, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#இல்லையெனில்
#எனில் CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_PSRAM, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#இல்லையெனில்
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#endif
#endif
}

#எனில் CONFIG_FREERTOS_எண்_OF_CORES == 2

நிலையான நிலையற்ற முழு shared_மதிப்பு;
நிலையான portMUX_TYPE *shared_mux;
நிலையான SemaphoreHandle_t done_sem;

நிலையான வெறுமை task_shared_மதிப்பு_increment(வெறுமை *ignore)
{
    க்கு (முழு i = 0; i < REPEAT_OPS; i++) {
        portENTER_CRITICAL(shared_mux);
        shared_மதிப்பு++;
        portEXIT_CRITICAL(shared_mux);
    }
    xSemaphoreGive(done_sem);
    vTaskDelete(வெற்று);
}

TEST_CASE("portMUX cross-core locking", "[freertos]")
{
    shared_mux = heap_caps_malloc(அளவு(portMUX_TYPE), MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL);
    done_sem = xSemaphoreCreateCounting(2, 0);
    portMUX_INITIALIZE(shared_mux);
    shared_மதிப்பு = 0;

    BENCHMARK_START();

    xTaskCreatePinnedToCore(task_shared_மதிப்பு_increment, "INC0", 2048, வெற்று, UNITY_FREERTOS_PRIORITY + 1, வெற்று, UNITY_FREERTOS_CPU ? 0 : 1);
    xTaskCreatePinnedToCore(task_shared_மதிப்பு_increment, "INC1", 2048, வெற்று, UNITY_FREERTOS_PRIORITY + 1, வெற்று, UNITY_FREERTOS_CPU);

    க்கு (முழு i = 0; i < 2; i++) {
        எனில் (!xSemaphoreTake(done_sem, 10000 / portTICK_PERIOD_MS)) {
            TEST_FAIL_MESSAGE("done_sem not released by test task");
        }
    }

    BENCHMARK_END("cross-core incrementing");
    vSemaphoreDelete(done_sem);
    விடுவி(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * 2, shared_மதிப்பு);
}

வெறுமை portmux_உயர்வு_contention_test(uint32_t lock_malloc_caps)
{
    மாறிலி முழு TOTAL_TASKS = 8; /* half on each core */
    shared_mux = heap_caps_malloc(அளவு(portMUX_TYPE), lock_malloc_caps);
    done_sem = xSemaphoreCreateCounting(TOTAL_TASKS, 0);
    portMUX_INITIALIZE(shared_mux);
    shared_மதிப்பு = 0;

    BENCHMARK_START();

    க்கு (முழு i = 0; i < TOTAL_TASKS / 2; i++) {
        /* as each task has a higher priority than previous, expect
           them to preempt the earlier created task, at least on the
           other core (this core has the unity task, until that
           blocks)... */
        xTaskCreatePinnedToCore(task_shared_மதிப்பு_increment, "INC0", 2048, வெற்று, tskIDLE_PRIORITY + 1 + i, வெற்று, UNITY_FREERTOS_CPU ? 0 : 1);
        xTaskCreatePinnedToCore(task_shared_மதிப்பு_increment, "INC1", 2048, வெற்று, tskIDLE_PRIORITY + 1 + i, வெற்று, UNITY_FREERTOS_CPU);
    }

    க்கு (முழு i = 0; i < TOTAL_TASKS; i++) {
        எனில் (!xSemaphoreTake(done_sem, 10000 / portTICK_PERIOD_MS)) {
            TEST_FAIL_MESSAGE("done_sem not released by test task");
        }
    }

    BENCHMARK_END("cross-core high contention");
    vSemaphoreDelete(done_sem);
    விடுவி(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * TOTAL_TASKS, shared_மதிப்பு);
}

TEST_CASE("portMUX high contention", "[freertos]")
{
    portmux_உயர்வு_contention_test(MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL);
}

#எனில் CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC
TEST_CASE("portMUX high contention, PSRAM", "[freertos]")
{
    portmux_உயர்வு_contention_test(MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);
}
#endif// CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC

#endif // CONFIG_FREERTOS_NUMBER_OF_CORES == 2

#endif // !CONFIG_FREERTOS_SMP
