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

#تعریف_کرو REPEAT_OPS 10000

ساكن uint32_t شروع, سرا;

#تعریف_کرو BENCHMARK_شروع() افعل {                  \
        شروع = esp_cpu_get_چکر_گنتی();                     \
    } طالما(0)

#تعریف_کرو BENCHMARK_سرا(OPERATION) افعل {                       \
        سرا = esp_cpu_get_چکر_گنتی();                                          \
        لکھو("%s took %"PRIu32" cycles/op (%"PRIu32" cycles for %d ops)\n",     \
               OPERATION, (سرا - شروع)/REPEAT_OPS,                 \
               (سرا - شروع), REPEAT_OPS);                          \
    } طالما(0)

TEST_CASE("portMUX spinlocks (no contention)", "[freertos]")
{
    پورٹ_MUX_TYPE mux = پورٹ_MUX_INITIALIZER_UNLOCKED;
    BENCHMARK_شروع();

    لكل (صحيح i = 0; i < REPEAT_OPS; i++) {
        پورٹ_ENTER_CRITICAL_ISR(&mux);
        پورٹ_EXIT_CRITICAL_ISR(&mux);
    }
    BENCHMARK_سرا("no contention lock");

#إذا CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_چکراں_فی_OP_UNICORE, "%"PRIu32" cycles/op", ((سرا - شروع) / REPEAT_OPS));
#وإلا
#إذا CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_چکراں_فی_OP_PSRAM, "%"PRIu32" cycles/op", ((سرا - شروع) / REPEAT_OPS));
#وإلا
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_چکراں_فی_OP, "%"PRIu32" cycles/op", ((سرا - شروع) / REPEAT_OPS));
#جے_انت
#جے_انت
}

TEST_CASE("portMUX recursive locks (no contention)", "[freertos]")
{
    پورٹ_MUX_TYPE mux = پورٹ_MUX_INITIALIZER_UNLOCKED;
    BENCHMARK_شروع();

    ثابت صحيح RECURSE_گنتی = 25;

    لكل (صحيح i = 0; i < REPEAT_OPS / RECURSE_گنتی; i++) {
        لكل (صحيح j = 0; j < RECURSE_گنتی; j++) {
            پورٹ_ENTER_CRITICAL(&mux);
        }
        لكل (صحيح j = 0; j < RECURSE_گنتی; j++) {
            پورٹ_EXIT_CRITICAL(&mux);
        }
    }
    BENCHMARK_سرا("no contention recursive");

#إذا CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_چکراں_فی_OP_UNICORE, "%"PRIu32" cycles/op", ((سرا - شروع) / REPEAT_OPS));
#وإلا
#إذا CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_چکراں_فی_OP_PSRAM, "%"PRIu32" cycles/op", ((سرا - شروع) / REPEAT_OPS));
#وإلا
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_چکراں_فی_OP, "%"PRIu32" cycles/op", ((سرا - شروع) / REPEAT_OPS));
#جے_انت
#جے_انت
}

#إذا CONFIG_FREERTOS_نمبر_دا_CORES == 2

ساكن متطاير صحيح shared_قدر;
ساكن پورٹ_MUX_TYPE *shared_mux;
ساكن SemaphoreHandle_t پورا_sem;

ساكن فراغ task_shared_قدر_increment(فراغ *ignore)
{
    لكل (صحيح i = 0; i < REPEAT_OPS; i++) {
        پورٹ_ENTER_CRITICAL(shared_mux);
        shared_قدر++;
        پورٹ_EXIT_CRITICAL(shared_mux);
    }
    xSemaphoreGive(پورا_sem);
    vTaskDelete(عدم);
}

TEST_CASE("portMUX cross-core locking", "[freertos]")
{
    shared_mux = ڈھیر_caps_malloc(حجم(پورٹ_MUX_TYPE), MALLOC_CAP_8_ثنائی_ہندسہ | MALLOC_CAP_اندرونی);
    پورا_sem = xSemaphoreCreateCounting(2, 0);
    پورٹ_MUX_INITIALIZE(shared_mux);
    shared_قدر = 0;

    BENCHMARK_شروع();

    x_Task_Create_Pinned_منزل_Core(task_shared_قدر_increment, "INC0", 2048, عدم, UNITY_FREERTOS_ترجیح + 1, عدم, UNITY_FREERTOS_CPU ? 0 : 1);
    x_Task_Create_Pinned_منزل_Core(task_shared_قدر_increment, "INC1", 2048, عدم, UNITY_FREERTOS_ترجیح + 1, عدم, UNITY_FREERTOS_CPU);

    لكل (صحيح i = 0; i < 2; i++) {
        إذا (!xSemaphoreTake(پورا_sem, 10000 / پورٹ_TICK_PERIOD_MS)) {
            TEST_FAIL_سنیہا("done_sem not released by test task");
        }
    }

    BENCHMARK_سرا("cross-core incrementing");
    vSemaphoreDelete(پورا_sem);
    حرر(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * 2, shared_قدر);
}

فراغ portmux_بلند_contention_test(uint32_t جندرا_malloc_caps)
{
    ثابت صحيح کل_TASKS = 8; /* half on each core */
    shared_mux = ڈھیر_caps_malloc(حجم(پورٹ_MUX_TYPE), جندرا_malloc_caps);
    پورا_sem = xSemaphoreCreateCounting(کل_TASKS, 0);
    پورٹ_MUX_INITIALIZE(shared_mux);
    shared_قدر = 0;

    BENCHMARK_شروع();

    لكل (صحيح i = 0; i < کل_TASKS / 2; i++) {
        /* as each task has a higher priority than previous, expect
           them to preempt the earlier created task, at least on the
           other core (this core has the unity task, until that
           blocks)... */
        x_Task_Create_Pinned_منزل_Core(task_shared_قدر_increment, "INC0", 2048, عدم, tsk_IDLE_ترجیح + 1 + i, عدم, UNITY_FREERTOS_CPU ? 0 : 1);
        x_Task_Create_Pinned_منزل_Core(task_shared_قدر_increment, "INC1", 2048, عدم, tsk_IDLE_ترجیح + 1 + i, عدم, UNITY_FREERTOS_CPU);
    }

    لكل (صحيح i = 0; i < کل_TASKS; i++) {
        إذا (!xSemaphoreTake(پورا_sem, 10000 / پورٹ_TICK_PERIOD_MS)) {
            TEST_FAIL_سنیہا("done_sem not released by test task");
        }
    }

    BENCHMARK_سرا("cross-core high contention");
    vSemaphoreDelete(پورا_sem);
    حرر(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * کل_TASKS, shared_قدر);
}

TEST_CASE("portMUX high contention", "[freertos]")
{
    portmux_بلند_contention_test(MALLOC_CAP_8_ثنائی_ہندسہ | MALLOC_CAP_اندرونی);
}

#إذا CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC
TEST_CASE("portMUX high contention, PSRAM", "[freertos]")
{
    portmux_بلند_contention_test(MALLOC_CAP_8_ثنائی_ہندسہ | MALLOC_CAP_SPIRAM);
}
#جے_انت// CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC

#جے_انت // CONFIG_FREERTOS_NUMBER_OF_CORES == 2

#جے_انت // !CONFIG_FREERTOS_SMP
