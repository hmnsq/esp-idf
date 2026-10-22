/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Combined unit tests & benchmarking for spinlock "portMUX" functionality
*/

#ضمن <esp_types.h>
#ضمن <stdio.h>
#ضمن <inttypes.h>
#ضمن "sdkconfig.h"
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "freertos/semphr.h"
#ضمن "freertos/queue.h"
#ضمن "unity.h"
#ضمن "esp_cpu.h"

#ضمن "test_utils.h"
#ضمن "freertos_performance.h"

#إذا !CONFIG_FREERTOS_SMP    // Known issue in Amazon SMP FreeRTOS port IDF-6204

#define REPEAT_OPS 10000

ساكن uint32_t start, end;

#define BENCHMARK_START() افعل {                  \
        start = esp_cpu_get_cycle_عدد();                     \
    } طالما(0)

#define BENCHMARK_END(OPERATION) افعل {                       \
        end = esp_cpu_get_cycle_عدد();                                          \
        printf("%s took %"PRIu32" cycles/op (%"PRIu32" cycles for %d ops)\n",     \
               OPERATION, (end - start)/REPEAT_OPS,                 \
               (end - start), REPEAT_OPS);                          \
    } طالما(0)

TEST_CASE("portMUX spinlocks (no contention)", "[freertos]")
{
    portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
    BENCHMARK_START();

    لكل (صحيح i = 0; i < REPEAT_OPS; i++) {
        portENTER_CRITICAL_ISR(&mux);
        portEXIT_CRITICAL_ISR(&mux);
    }
    BENCHMARK_END("no contention lock");

#إذا CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_UNICORE, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#وإلا
#إذا CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_PSRAM, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#وإلا
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#endif
#endif
}

TEST_CASE("portMUX recursive locks (no contention)", "[freertos]")
{
    portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
    BENCHMARK_START();

    ثابت صحيح RECURSE_عدد = 25;

    لكل (صحيح i = 0; i < REPEAT_OPS / RECURSE_عدد; i++) {
        لكل (صحيح j = 0; j < RECURSE_عدد; j++) {
            portENTER_CRITICAL(&mux);
        }
        لكل (صحيح j = 0; j < RECURSE_عدد; j++) {
            portEXIT_CRITICAL(&mux);
        }
    }
    BENCHMARK_END("no contention recursive");

#إذا CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_UNICORE, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#وإلا
#إذا CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_PSRAM, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#وإلا
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#endif
#endif
}

#إذا CONFIG_FREERTOS_رقم_OF_CORES == 2

ساكن متطاير صحيح shared_قيمة;
ساكن portMUX_TYPE *shared_mux;
ساكن SemaphoreHandle_t done_sem;

ساكن فراغ task_shared_قيمة_increment(فراغ *ignore)
{
    لكل (صحيح i = 0; i < REPEAT_OPS; i++) {
        portENTER_CRITICAL(shared_mux);
        shared_قيمة++;
        portEXIT_CRITICAL(shared_mux);
    }
    xSemaphoreGive(done_sem);
    vTaskDelete(عدم);
}

TEST_CASE("portMUX cross-core locking", "[freertos]")
{
    shared_mux = heap_caps_malloc(حجم(portMUX_TYPE), MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL);
    done_sem = xSemaphoreCreateCounting(2, 0);
    portMUX_INITIALIZE(shared_mux);
    shared_قيمة = 0;

    BENCHMARK_START();

    xTaskCreatePinnedToCore(task_shared_قيمة_increment, "INC0", 2048, عدم, UNITY_FREERTOS_PRIORITY + 1, عدم, UNITY_FREERTOS_CPU ? 0 : 1);
    xTaskCreatePinnedToCore(task_shared_قيمة_increment, "INC1", 2048, عدم, UNITY_FREERTOS_PRIORITY + 1, عدم, UNITY_FREERTOS_CPU);

    لكل (صحيح i = 0; i < 2; i++) {
        إذا (!xSemaphoreTake(done_sem, 10000 / portTICK_PERIOD_MS)) {
            TEST_FAIL_MESSAGE("done_sem not released by test task");
        }
    }

    BENCHMARK_END("cross-core incrementing");
    vSemaphoreDelete(done_sem);
    حرر(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * 2, shared_قيمة);
}

فراغ portmux_عالي_contention_test(uint32_t lock_malloc_caps)
{
    ثابت صحيح TOTAL_TASKS = 8; /* half on each core */
    shared_mux = heap_caps_malloc(حجم(portMUX_TYPE), lock_malloc_caps);
    done_sem = xSemaphoreCreateCounting(TOTAL_TASKS, 0);
    portMUX_INITIALIZE(shared_mux);
    shared_قيمة = 0;

    BENCHMARK_START();

    لكل (صحيح i = 0; i < TOTAL_TASKS / 2; i++) {
        /* as each task has a higher priority than previous, expect
           them to preempt the earlier created task, at least on the
           other core (this core has the unity task, until that
           blocks)... */
        xTaskCreatePinnedToCore(task_shared_قيمة_increment, "INC0", 2048, عدم, tskIDLE_PRIORITY + 1 + i, عدم, UNITY_FREERTOS_CPU ? 0 : 1);
        xTaskCreatePinnedToCore(task_shared_قيمة_increment, "INC1", 2048, عدم, tskIDLE_PRIORITY + 1 + i, عدم, UNITY_FREERTOS_CPU);
    }

    لكل (صحيح i = 0; i < TOTAL_TASKS; i++) {
        إذا (!xSemaphoreTake(done_sem, 10000 / portTICK_PERIOD_MS)) {
            TEST_FAIL_MESSAGE("done_sem not released by test task");
        }
    }

    BENCHMARK_END("cross-core high contention");
    vSemaphoreDelete(done_sem);
    حرر(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * TOTAL_TASKS, shared_قيمة);
}

TEST_CASE("portMUX high contention", "[freertos]")
{
    portmux_عالي_contention_test(MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL);
}

#إذا CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC
TEST_CASE("portMUX high contention, PSRAM", "[freertos]")
{
    portmux_عالي_contention_test(MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);
}
#endif// CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC

#endif // CONFIG_FREERTOS_NUMBER_OF_CORES == 2

#endif // !CONFIG_FREERTOS_SMP
