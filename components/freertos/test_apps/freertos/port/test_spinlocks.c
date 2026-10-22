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

#تعریف_کړه REPEAT_OPS 10000

ساكن uint32_t پیل, پای_ټکی;

#تعریف_کړه BENCHMARK_پیل() افعل {                  \
        پیل = esp_cpu_get_دوره_شمېر();                     \
    } طالما(0)

#تعریف_کړه BENCHMARK_پای_ټکی(OPERATION) افعل {                       \
        پای_ټکی = esp_cpu_get_دوره_شمېر();                                          \
        چاپ_کړه("%s took %"PRIu32" cycles/op (%"PRIu32" cycles for %d ops)\n",     \
               OPERATION, (پای_ټکی - پیل)/REPEAT_OPS,                 \
               (پای_ټکی - پیل), REPEAT_OPS);                          \
    } طالما(0)

TEST_CASE("portMUX spinlocks (no contention)", "[freertos]")
{
    پورټ_MUX_TYPE mux = پورټ_MUX_INITIALIZER_UNLOCKED;
    BENCHMARK_پیل();

    لكل (صحيح i = 0; i < REPEAT_OPS; i++) {
        پورټ_ENTER_CRITICAL_ISR(&mux);
        پورټ_EXIT_CRITICAL_ISR(&mux);
    }
    BENCHMARK_پای_ټکی("no contention lock");

#إذا CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_دورې_فی_OP_UNICORE, "%"PRIu32" cycles/op", ((پای_ټکی - پیل) / REPEAT_OPS));
#وإلا
#إذا CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_دورې_فی_OP_PSRAM, "%"PRIu32" cycles/op", ((پای_ټکی - پیل) / REPEAT_OPS));
#وإلا
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_دورې_فی_OP, "%"PRIu32" cycles/op", ((پای_ټکی - پیل) / REPEAT_OPS));
#د_که_پای
#د_که_پای
}

TEST_CASE("portMUX recursive locks (no contention)", "[freertos]")
{
    پورټ_MUX_TYPE mux = پورټ_MUX_INITIALIZER_UNLOCKED;
    BENCHMARK_پیل();

    ثابت صحيح RECURSE_شمېر = 25;

    لكل (صحيح i = 0; i < REPEAT_OPS / RECURSE_شمېر; i++) {
        لكل (صحيح j = 0; j < RECURSE_شمېر; j++) {
            پورټ_ENTER_CRITICAL(&mux);
        }
        لكل (صحيح j = 0; j < RECURSE_شمېر; j++) {
            پورټ_EXIT_CRITICAL(&mux);
        }
    }
    BENCHMARK_پای_ټکی("no contention recursive");

#إذا CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_دورې_فی_OP_UNICORE, "%"PRIu32" cycles/op", ((پای_ټکی - پیل) / REPEAT_OPS));
#وإلا
#إذا CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_دورې_فی_OP_PSRAM, "%"PRIu32" cycles/op", ((پای_ټکی - پیل) / REPEAT_OPS));
#وإلا
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_دورې_فی_OP, "%"PRIu32" cycles/op", ((پای_ټکی - پیل) / REPEAT_OPS));
#د_که_پای
#د_که_پای
}

#إذا CONFIG_FREERTOS_شمېره_د_CORES == 2

ساكن متطاير صحيح shared_ارزښت;
ساكن پورټ_MUX_TYPE *shared_mux;
ساكن SemaphoreHandle_t بشپړ_sem;

ساكن فراغ task_shared_ارزښت_increment(فراغ *ignore)
{
    لكل (صحيح i = 0; i < REPEAT_OPS; i++) {
        پورټ_ENTER_CRITICAL(shared_mux);
        shared_ارزښت++;
        پورټ_EXIT_CRITICAL(shared_mux);
    }
    xSemaphoreGive(بشپړ_sem);
    vTaskDelete(عدم);
}

TEST_CASE("portMUX cross-core locking", "[freertos]")
{
    shared_mux = ډېرۍ_caps_malloc(حجم(پورټ_MUX_TYPE), MALLOC_CAP_8_بټ | MALLOC_CAP_دننی);
    بشپړ_sem = xSemaphoreCreateCounting(2, 0);
    پورټ_MUX_INITIALIZE(shared_mux);
    shared_ارزښت = 0;

    BENCHMARK_پیل();

    x_Task_Create_Pinned_منزل_Core(task_shared_ارزښت_increment, "INC0", 2048, عدم, UNITY_FREERTOS_لومړیتوب + 1, عدم, UNITY_FREERTOS_CPU ? 0 : 1);
    x_Task_Create_Pinned_منزل_Core(task_shared_ارزښت_increment, "INC1", 2048, عدم, UNITY_FREERTOS_لومړیتوب + 1, عدم, UNITY_FREERTOS_CPU);

    لكل (صحيح i = 0; i < 2; i++) {
        إذا (!xSemaphoreTake(بشپړ_sem, 10000 / پورټ_TICK_PERIOD_MS)) {
            TEST_FAIL_پیغام("done_sem not released by test task");
        }
    }

    BENCHMARK_پای_ټکی("cross-core incrementing");
    vSemaphoreDelete(بشپړ_sem);
    حرر(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * 2, shared_ارزښت);
}

فراغ portmux_لوړ_contention_test(uint32_t قلف_malloc_caps)
{
    ثابت صحيح ټول_TASKS = 8; /* half on each core */
    shared_mux = ډېرۍ_caps_malloc(حجم(پورټ_MUX_TYPE), قلف_malloc_caps);
    بشپړ_sem = xSemaphoreCreateCounting(ټول_TASKS, 0);
    پورټ_MUX_INITIALIZE(shared_mux);
    shared_ارزښت = 0;

    BENCHMARK_پیل();

    لكل (صحيح i = 0; i < ټول_TASKS / 2; i++) {
        /* as each task has a higher priority than previous, expect
           them to preempt the earlier created task, at least on the
           other core (this core has the unity task, until that
           blocks)... */
        x_Task_Create_Pinned_منزل_Core(task_shared_ارزښت_increment, "INC0", 2048, عدم, tsk_IDLE_لومړیتوب + 1 + i, عدم, UNITY_FREERTOS_CPU ? 0 : 1);
        x_Task_Create_Pinned_منزل_Core(task_shared_ارزښت_increment, "INC1", 2048, عدم, tsk_IDLE_لومړیتوب + 1 + i, عدم, UNITY_FREERTOS_CPU);
    }

    لكل (صحيح i = 0; i < ټول_TASKS; i++) {
        إذا (!xSemaphoreTake(بشپړ_sem, 10000 / پورټ_TICK_PERIOD_MS)) {
            TEST_FAIL_پیغام("done_sem not released by test task");
        }
    }

    BENCHMARK_پای_ټکی("cross-core high contention");
    vSemaphoreDelete(بشپړ_sem);
    حرر(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * ټول_TASKS, shared_ارزښت);
}

TEST_CASE("portMUX high contention", "[freertos]")
{
    portmux_لوړ_contention_test(MALLOC_CAP_8_بټ | MALLOC_CAP_دننی);
}

#إذا CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC
TEST_CASE("portMUX high contention, PSRAM", "[freertos]")
{
    portmux_لوړ_contention_test(MALLOC_CAP_8_بټ | MALLOC_CAP_SPIRAM);
}
#د_که_پای// CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC

#د_که_پای // CONFIG_FREERTOS_NUMBER_OF_CORES == 2

#د_که_پای // !CONFIG_FREERTOS_SMP
