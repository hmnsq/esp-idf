/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Combined unit tests & benchmarking for spinlock "portMUX" functionality
*/

#شامل <esp_types.h>
#شامل <stdio.h>
#شامل <inttypes.h>
#شامل "sdkconfig.h"
#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل "freertos/semphr.h"
#شامل "freertos/queue.h"
#شامل "unity.h"
#شامل "esp_cpu.h"

#شامل "test_utils.h"
#شامل "freertos_performance.h"

#اگر !CONFIG_FREERTOS_SMP    // Known issue in Amazon SMP FreeRTOS port IDF-6204

#define REPEAT_OPS 10000

ایستا uint32_t start, end;

#define BENCHMARK_START() انجامبده {                  \
        start = esp_cpu_get_cycle_شمار();                     \
    } وقتی(0)

#define BENCHMARK_END(OPERATION) انجامبده {                       \
        end = esp_cpu_get_cycle_شمار();                                          \
        printf("%s took %"PRIu32" cycles/op (%"PRIu32" cycles for %d ops)\n",     \
               OPERATION, (end - start)/REPEAT_OPS,                 \
               (end - start), REPEAT_OPS);                          \
    } وقتی(0)

TEST_CASE("portMUX spinlocks (no contention)", "[freertos]")
{
    portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
    BENCHMARK_START();

    برای (صحیح i = 0; i < REPEAT_OPS; i++) {
        portENTER_CRITICAL_ISR(&mux);
        portEXIT_CRITICAL_ISR(&mux);
    }
    BENCHMARK_END("no contention lock");

#اگر CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_UNICORE, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#وگرنه
#اگر CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_PSRAM, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#وگرنه
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#endif
#endif
}

TEST_CASE("portMUX recursive locks (no contention)", "[freertos]")
{
    portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
    BENCHMARK_START();

    ثابت صحیح RECURSE_شمار = 25;

    برای (صحیح i = 0; i < REPEAT_OPS / RECURSE_شمار; i++) {
        برای (صحیح j = 0; j < RECURSE_شمار; j++) {
            portENTER_CRITICAL(&mux);
        }
        برای (صحیح j = 0; j < RECURSE_شمار; j++) {
            portEXIT_CRITICAL(&mux);
        }
    }
    BENCHMARK_END("no contention recursive");

#اگر CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_UNICORE, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#وگرنه
#اگر CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_PSRAM, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#وگرنه
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#endif
#endif
}

#اگر CONFIG_FREERTOS_عدد_OF_CORES == 2

ایستا فرار صحیح shared_مقدار;
ایستا portMUX_TYPE *shared_mux;
ایستا SemaphoreHandle_t done_sem;

ایستا تهی task_shared_مقدار_increment(تهی *ignore)
{
    برای (صحیح i = 0; i < REPEAT_OPS; i++) {
        portENTER_CRITICAL(shared_mux);
        shared_مقدار++;
        portEXIT_CRITICAL(shared_mux);
    }
    xSemaphoreGive(done_sem);
    vTaskDelete(پوچ);
}

TEST_CASE("portMUX cross-core locking", "[freertos]")
{
    shared_mux = heap_caps_malloc(اندازه(portMUX_TYPE), MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL);
    done_sem = xSemaphoreCreateCounting(2, 0);
    portMUX_INITIALIZE(shared_mux);
    shared_مقدار = 0;

    BENCHMARK_START();

    xTaskCreatePinnedToCore(task_shared_مقدار_increment, "INC0", 2048, پوچ, UNITY_FREERTOS_PRIORITY + 1, پوچ, UNITY_FREERTOS_CPU ? 0 : 1);
    xTaskCreatePinnedToCore(task_shared_مقدار_increment, "INC1", 2048, پوچ, UNITY_FREERTOS_PRIORITY + 1, پوچ, UNITY_FREERTOS_CPU);

    برای (صحیح i = 0; i < 2; i++) {
        اگر (!xSemaphoreTake(done_sem, 10000 / portTICK_PERIOD_MS)) {
            TEST_FAIL_MESSAGE("done_sem not released by test task");
        }
    }

    BENCHMARK_END("cross-core incrementing");
    vSemaphoreDelete(done_sem);
    آزاد(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * 2, shared_مقدار);
}

تهی portmux_بالا_contention_test(uint32_t lock_malloc_caps)
{
    ثابت صحیح TOTAL_TASKS = 8; /* half on each core */
    shared_mux = heap_caps_malloc(اندازه(portMUX_TYPE), lock_malloc_caps);
    done_sem = xSemaphoreCreateCounting(TOTAL_TASKS, 0);
    portMUX_INITIALIZE(shared_mux);
    shared_مقدار = 0;

    BENCHMARK_START();

    برای (صحیح i = 0; i < TOTAL_TASKS / 2; i++) {
        /* as each task has a higher priority than previous, expect
           them to preempt the earlier created task, at least on the
           other core (this core has the unity task, until that
           blocks)... */
        xTaskCreatePinnedToCore(task_shared_مقدار_increment, "INC0", 2048, پوچ, tskIDLE_PRIORITY + 1 + i, پوچ, UNITY_FREERTOS_CPU ? 0 : 1);
        xTaskCreatePinnedToCore(task_shared_مقدار_increment, "INC1", 2048, پوچ, tskIDLE_PRIORITY + 1 + i, پوچ, UNITY_FREERTOS_CPU);
    }

    برای (صحیح i = 0; i < TOTAL_TASKS; i++) {
        اگر (!xSemaphoreTake(done_sem, 10000 / portTICK_PERIOD_MS)) {
            TEST_FAIL_MESSAGE("done_sem not released by test task");
        }
    }

    BENCHMARK_END("cross-core high contention");
    vSemaphoreDelete(done_sem);
    آزاد(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * TOTAL_TASKS, shared_مقدار);
}

TEST_CASE("portMUX high contention", "[freertos]")
{
    portmux_بالا_contention_test(MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL);
}

#اگر CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC
TEST_CASE("portMUX high contention, PSRAM", "[freertos]")
{
    portmux_بالا_contention_test(MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);
}
#endif// CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC

#endif // CONFIG_FREERTOS_NUMBER_OF_CORES == 2

#endif // !CONFIG_FREERTOS_SMP
