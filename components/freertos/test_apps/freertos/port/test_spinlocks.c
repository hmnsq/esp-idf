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

#وصف_ڪريو REPEAT_OPS 10000

ساكن uint32_t شروعات, پڇاڙي;

#وصف_ڪريو BENCHMARK_شروعات() افعل {                  \
        شروعات = esp_cpu_get_چڪر_ڳڻپ();                     \
    } طالما(0)

#وصف_ڪريو BENCHMARK_پڇاڙي(OPERATION) افعل {                       \
        پڇاڙي = esp_cpu_get_چڪر_ڳڻپ();                                          \
        لکو("%s took %"PRIu32" cycles/op (%"PRIu32" cycles for %d ops)\n",     \
               OPERATION, (پڇاڙي - شروعات)/REPEAT_OPS,                 \
               (پڇاڙي - شروعات), REPEAT_OPS);                          \
    } طالما(0)

TEST_CASE("portMUX spinlocks (no contention)", "[freertos]")
{
    پورٽ_MUX_TYPE mux = پورٽ_MUX_INITIALIZER_UNLOCKED;
    BENCHMARK_شروعات();

    لكل (صحيح i = 0; i < REPEAT_OPS; i++) {
        پورٽ_ENTER_CRITICAL_ISR(&mux);
        پورٽ_EXIT_CRITICAL_ISR(&mux);
    }
    BENCHMARK_پڇاڙي("no contention lock");

#إذا CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_چڪرن_في_OP_UNICORE, "%"PRIu32" cycles/op", ((پڇاڙي - شروعات) / REPEAT_OPS));
#وإلا
#إذا CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_چڪرن_في_OP_PSRAM, "%"PRIu32" cycles/op", ((پڇاڙي - شروعات) / REPEAT_OPS));
#وإلا
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_چڪرن_في_OP, "%"PRIu32" cycles/op", ((پڇاڙي - شروعات) / REPEAT_OPS));
#جيڪڏهن_پورو
#جيڪڏهن_پورو
}

TEST_CASE("portMUX recursive locks (no contention)", "[freertos]")
{
    پورٽ_MUX_TYPE mux = پورٽ_MUX_INITIALIZER_UNLOCKED;
    BENCHMARK_شروعات();

    ثابت صحيح RECURSE_ڳڻپ = 25;

    لكل (صحيح i = 0; i < REPEAT_OPS / RECURSE_ڳڻپ; i++) {
        لكل (صحيح j = 0; j < RECURSE_ڳڻپ; j++) {
            پورٽ_ENTER_CRITICAL(&mux);
        }
        لكل (صحيح j = 0; j < RECURSE_ڳڻپ; j++) {
            پورٽ_EXIT_CRITICAL(&mux);
        }
    }
    BENCHMARK_پڇاڙي("no contention recursive");

#إذا CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_چڪرن_في_OP_UNICORE, "%"PRIu32" cycles/op", ((پڇاڙي - شروعات) / REPEAT_OPS));
#وإلا
#إذا CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_چڪرن_في_OP_PSRAM, "%"PRIu32" cycles/op", ((پڇاڙي - شروعات) / REPEAT_OPS));
#وإلا
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_چڪرن_في_OP, "%"PRIu32" cycles/op", ((پڇاڙي - شروعات) / REPEAT_OPS));
#جيڪڏهن_پورو
#جيڪڏهن_پورو
}

#إذا CONFIG_FREERTOS_عدد_جو_CORES == 2

ساكن متطاير صحيح shared_قدر;
ساكن پورٽ_MUX_TYPE *shared_mux;
ساكن SemaphoreHandle_t پورو_sem;

ساكن فراغ task_shared_قدر_increment(فراغ *ignore)
{
    لكل (صحيح i = 0; i < REPEAT_OPS; i++) {
        پورٽ_ENTER_CRITICAL(shared_mux);
        shared_قدر++;
        پورٽ_EXIT_CRITICAL(shared_mux);
    }
    xSemaphoreGive(پورو_sem);
    vTaskDelete(عدم);
}

TEST_CASE("portMUX cross-core locking", "[freertos]")
{
    shared_mux = ڍير_caps_malloc(حجم(پورٽ_MUX_TYPE), MALLOC_CAP_8_ٻٽو_انگ | MALLOC_CAP_اندروني);
    پورو_sem = xSemaphoreCreateCounting(2, 0);
    پورٽ_MUX_INITIALIZE(shared_mux);
    shared_قدر = 0;

    BENCHMARK_شروعات();

    x_Task_Create_Pinned_منزل_Core(task_shared_قدر_increment, "INC0", 2048, عدم, UNITY_FREERTOS_ترجيح + 1, عدم, UNITY_FREERTOS_CPU ? 0 : 1);
    x_Task_Create_Pinned_منزل_Core(task_shared_قدر_increment, "INC1", 2048, عدم, UNITY_FREERTOS_ترجيح + 1, عدم, UNITY_FREERTOS_CPU);

    لكل (صحيح i = 0; i < 2; i++) {
        إذا (!xSemaphoreTake(پورو_sem, 10000 / پورٽ_TICK_PERIOD_MS)) {
            TEST_FAIL_پيغام("done_sem not released by test task");
        }
    }

    BENCHMARK_پڇاڙي("cross-core incrementing");
    vSemaphoreDelete(پورو_sem);
    حرر(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * 2, shared_قدر);
}

فراغ portmux_مٿاهون_contention_test(uint32_t تالو_malloc_caps)
{
    ثابت صحيح ڪل_TASKS = 8; /* half on each core */
    shared_mux = ڍير_caps_malloc(حجم(پورٽ_MUX_TYPE), تالو_malloc_caps);
    پورو_sem = xSemaphoreCreateCounting(ڪل_TASKS, 0);
    پورٽ_MUX_INITIALIZE(shared_mux);
    shared_قدر = 0;

    BENCHMARK_شروعات();

    لكل (صحيح i = 0; i < ڪل_TASKS / 2; i++) {
        /* as each task has a higher priority than previous, expect
           them to preempt the earlier created task, at least on the
           other core (this core has the unity task, until that
           blocks)... */
        x_Task_Create_Pinned_منزل_Core(task_shared_قدر_increment, "INC0", 2048, عدم, tsk_IDLE_ترجيح + 1 + i, عدم, UNITY_FREERTOS_CPU ? 0 : 1);
        x_Task_Create_Pinned_منزل_Core(task_shared_قدر_increment, "INC1", 2048, عدم, tsk_IDLE_ترجيح + 1 + i, عدم, UNITY_FREERTOS_CPU);
    }

    لكل (صحيح i = 0; i < ڪل_TASKS; i++) {
        إذا (!xSemaphoreTake(پورو_sem, 10000 / پورٽ_TICK_PERIOD_MS)) {
            TEST_FAIL_پيغام("done_sem not released by test task");
        }
    }

    BENCHMARK_پڇاڙي("cross-core high contention");
    vSemaphoreDelete(پورو_sem);
    حرر(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * ڪل_TASKS, shared_قدر);
}

TEST_CASE("portMUX high contention", "[freertos]")
{
    portmux_مٿاهون_contention_test(MALLOC_CAP_8_ٻٽو_انگ | MALLOC_CAP_اندروني);
}

#إذا CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC
TEST_CASE("portMUX high contention, PSRAM", "[freertos]")
{
    portmux_مٿاهون_contention_test(MALLOC_CAP_8_ٻٽو_انگ | MALLOC_CAP_SPIRAM);
}
#جيڪڏهن_پورو// CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC

#جيڪڏهن_پورو // CONFIG_FREERTOS_NUMBER_OF_CORES == 2

#جيڪڏهن_پورو // !CONFIG_FREERTOS_SMP
