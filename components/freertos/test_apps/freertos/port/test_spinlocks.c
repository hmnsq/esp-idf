/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Combined unit tests & benchmarking for spinlock "portMUX" functionality
*/

#અન્તર્ભાવ <esp_types.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <inttypes.h>
#અન્તર્ભાવ "sdkconfig.h"
#અન્તર્ભાવ "freertos/FreeRTOS.h"
#અન્તર્ભાવ "freertos/task.h"
#અન્તર્ભાવ "freertos/semphr.h"
#અન્તર્ભાવ "freertos/queue.h"
#અન્તર્ભાવ "unity.h"
#અન્તર્ભાવ "esp_cpu.h"

#અન્તર્ભાવ "test_utils.h"
#અન્તર્ભાવ "freertos_performance.h"

#જો !CONFIG_FREERTOS_SMP    // Known issue in Amazon SMP FreeRTOS port IDF-6204

#define REPEAT_OPS 10000

સ્થિર uint32_t start, end;

#define BENCHMARK_START() કરો {                  \
        start = esp_cpu_get_cycle_ગણતરી();                     \
    } જ્યારે(0)

#define BENCHMARK_END(OPERATION) કરો {                       \
        end = esp_cpu_get_cycle_ગણતરી();                                          \
        printf("%s took %"PRIu32" cycles/op (%"PRIu32" cycles for %d ops)\n",     \
               OPERATION, (end - start)/REPEAT_OPS,                 \
               (end - start), REPEAT_OPS);                          \
    } જ્યારે(0)

TEST_CASE("portMUX spinlocks (no contention)", "[freertos]")
{
    portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
    BENCHMARK_START();

    માટે (પૂર્ણાંક i = 0; i < REPEAT_OPS; i++) {
        portENTER_CRITICAL_ISR(&mux);
        portEXIT_CRITICAL_ISR(&mux);
    }
    BENCHMARK_END("no contention lock");

#જો CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_UNICORE, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#નહીંતર
#જો CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_PSRAM, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#નહીંતર
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#endif
#endif
}

TEST_CASE("portMUX recursive locks (no contention)", "[freertos]")
{
    portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
    BENCHMARK_START();

    અચલ પૂર્ણાંક RECURSE_ગણતરી = 25;

    માટે (પૂર્ણાંક i = 0; i < REPEAT_OPS / RECURSE_ગણતરી; i++) {
        માટે (પૂર્ણાંક j = 0; j < RECURSE_ગણતરી; j++) {
            portENTER_CRITICAL(&mux);
        }
        માટે (પૂર્ણાંક j = 0; j < RECURSE_ગણતરી; j++) {
            portEXIT_CRITICAL(&mux);
        }
    }
    BENCHMARK_END("no contention recursive");

#જો CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_UNICORE, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#નહીંતર
#જો CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_PSRAM, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#નહીંતર
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#endif
#endif
}

#જો CONFIG_FREERTOS_સંખ્યા_OF_CORES == 2

સ્થિર અસ્થિર પૂર્ણાંક shared_કિંમત;
સ્થિર portMUX_TYPE *shared_mux;
સ્થિર SemaphoreHandle_t done_sem;

સ્થિર શૂન્ય task_shared_કિંમત_increment(શૂન્ય *ignore)
{
    માટે (પૂર્ણાંક i = 0; i < REPEAT_OPS; i++) {
        portENTER_CRITICAL(shared_mux);
        shared_કિંમત++;
        portEXIT_CRITICAL(shared_mux);
    }
    xSemaphoreGive(done_sem);
    vTaskDelete(ખાલી);
}

TEST_CASE("portMUX cross-core locking", "[freertos]")
{
    shared_mux = heap_caps_malloc(કદ(portMUX_TYPE), MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL);
    done_sem = xSemaphoreCreateCounting(2, 0);
    portMUX_INITIALIZE(shared_mux);
    shared_કિંમત = 0;

    BENCHMARK_START();

    xTaskCreatePinnedToCore(task_shared_કિંમત_increment, "INC0", 2048, ખાલી, UNITY_FREERTOS_PRIORITY + 1, ખાલી, UNITY_FREERTOS_CPU ? 0 : 1);
    xTaskCreatePinnedToCore(task_shared_કિંમત_increment, "INC1", 2048, ખાલી, UNITY_FREERTOS_PRIORITY + 1, ખાલી, UNITY_FREERTOS_CPU);

    માટે (પૂર્ણાંક i = 0; i < 2; i++) {
        જો (!xSemaphoreTake(done_sem, 10000 / portTICK_PERIOD_MS)) {
            TEST_FAIL_MESSAGE("done_sem not released by test task");
        }
    }

    BENCHMARK_END("cross-core incrementing");
    vSemaphoreDelete(done_sem);
    મુક્ત(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * 2, shared_કિંમત);
}

શૂન્ય portmux_ઉપલું_contention_test(uint32_t lock_malloc_caps)
{
    અચલ પૂર્ણાંક TOTAL_TASKS = 8; /* half on each core */
    shared_mux = heap_caps_malloc(કદ(portMUX_TYPE), lock_malloc_caps);
    done_sem = xSemaphoreCreateCounting(TOTAL_TASKS, 0);
    portMUX_INITIALIZE(shared_mux);
    shared_કિંમત = 0;

    BENCHMARK_START();

    માટે (પૂર્ણાંક i = 0; i < TOTAL_TASKS / 2; i++) {
        /* as each task has a higher priority than previous, expect
           them to preempt the earlier created task, at least on the
           other core (this core has the unity task, until that
           blocks)... */
        xTaskCreatePinnedToCore(task_shared_કિંમત_increment, "INC0", 2048, ખાલી, tskIDLE_PRIORITY + 1 + i, ખાલી, UNITY_FREERTOS_CPU ? 0 : 1);
        xTaskCreatePinnedToCore(task_shared_કિંમત_increment, "INC1", 2048, ખાલી, tskIDLE_PRIORITY + 1 + i, ખાલી, UNITY_FREERTOS_CPU);
    }

    માટે (પૂર્ણાંક i = 0; i < TOTAL_TASKS; i++) {
        જો (!xSemaphoreTake(done_sem, 10000 / portTICK_PERIOD_MS)) {
            TEST_FAIL_MESSAGE("done_sem not released by test task");
        }
    }

    BENCHMARK_END("cross-core high contention");
    vSemaphoreDelete(done_sem);
    મુક્ત(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * TOTAL_TASKS, shared_કિંમત);
}

TEST_CASE("portMUX high contention", "[freertos]")
{
    portmux_ઉપલું_contention_test(MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL);
}

#જો CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC
TEST_CASE("portMUX high contention, PSRAM", "[freertos]")
{
    portmux_ઉપલું_contention_test(MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);
}
#endif// CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC

#endif // CONFIG_FREERTOS_NUMBER_OF_CORES == 2

#endif // !CONFIG_FREERTOS_SMP
