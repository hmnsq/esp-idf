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

#تعریف_کن REPEAT_OPS 10000

ساكن uint32_t شروع, انتها;

#تعریف_کن BENCHMARK_شروع() افعل {                  \
        شروع = esp_cpu_get_چرخه_شمار();                     \
    } طالما(0)

#تعریف_کن BENCHMARK_انتها(OPERATION) افعل {                       \
        انتها = esp_cpu_get_چرخه_شمار();                                          \
        چاپ_کن("%s took %"PRIu32" cycles/op (%"PRIu32" cycles for %d ops)\n",     \
               OPERATION, (انتها - شروع)/REPEAT_OPS,                 \
               (انتها - شروع), REPEAT_OPS);                          \
    } طالما(0)

TEST_CASE("portMUX spinlocks (no contention)", "[freertos]")
{
    پورت_MUX_TYPE mux = پورت_MUX_INITIALIZER_UNLOCKED;
    BENCHMARK_شروع();

    لكل (صحيح i = 0; i < REPEAT_OPS; i++) {
        پورت_ENTER_CRITICAL_ISR(&mux);
        پورت_EXIT_CRITICAL_ISR(&mux);
    }
    BENCHMARK_انتها("no contention lock");

#إذا CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_چرخهها_در_هر_OP_UNICORE, "%"PRIu32" cycles/op", ((انتها - شروع) / REPEAT_OPS));
#وإلا
#إذا CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_چرخهها_در_هر_OP_PSRAM, "%"PRIu32" cycles/op", ((انتها - شروع) / REPEAT_OPS));
#وإلا
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_چرخهها_در_هر_OP, "%"PRIu32" cycles/op", ((انتها - شروع) / REPEAT_OPS));
#پایان_اگر
#پایان_اگر
}

TEST_CASE("portMUX recursive locks (no contention)", "[freertos]")
{
    پورت_MUX_TYPE mux = پورت_MUX_INITIALIZER_UNLOCKED;
    BENCHMARK_شروع();

    ثابت صحيح RECURSE_شمار = 25;

    لكل (صحيح i = 0; i < REPEAT_OPS / RECURSE_شمار; i++) {
        لكل (صحيح j = 0; j < RECURSE_شمار; j++) {
            پورت_ENTER_CRITICAL(&mux);
        }
        لكل (صحيح j = 0; j < RECURSE_شمار; j++) {
            پورت_EXIT_CRITICAL(&mux);
        }
    }
    BENCHMARK_انتها("no contention recursive");

#إذا CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_چرخهها_در_هر_OP_UNICORE, "%"PRIu32" cycles/op", ((انتها - شروع) / REPEAT_OPS));
#وإلا
#إذا CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_چرخهها_در_هر_OP_PSRAM, "%"PRIu32" cycles/op", ((انتها - شروع) / REPEAT_OPS));
#وإلا
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_چرخهها_در_هر_OP, "%"PRIu32" cycles/op", ((انتها - شروع) / REPEAT_OPS));
#پایان_اگر
#پایان_اگر
}

#إذا CONFIG_FREERTOS_عدد_از_CORES == 2

ساكن متطاير صحيح shared_مقدار;
ساكن پورت_MUX_TYPE *shared_mux;
ساكن SemaphoreHandle_t انجامشده_sem;

ساكن فراغ task_shared_مقدار_increment(فراغ *ignore)
{
    لكل (صحيح i = 0; i < REPEAT_OPS; i++) {
        پورت_ENTER_CRITICAL(shared_mux);
        shared_مقدار++;
        پورت_EXIT_CRITICAL(shared_mux);
    }
    xSemaphoreGive(انجامشده_sem);
    vTaskDelete(عدم);
}

TEST_CASE("portMUX cross-core locking", "[freertos]")
{
    shared_mux = انباشته_caps_malloc(حجم(پورت_MUX_TYPE), MALLOC_CAP_8_بیت | MALLOC_CAP_داخلی);
    انجامشده_sem = xSemaphoreCreateCounting(2, 0);
    پورت_MUX_INITIALIZE(shared_mux);
    shared_مقدار = 0;

    BENCHMARK_شروع();

    x_Task_Create_Pinned_مقصد_Core(task_shared_مقدار_increment, "INC0", 2048, عدم, UNITY_FREERTOS_اولویت + 1, عدم, UNITY_FREERTOS_CPU ? 0 : 1);
    x_Task_Create_Pinned_مقصد_Core(task_shared_مقدار_increment, "INC1", 2048, عدم, UNITY_FREERTOS_اولویت + 1, عدم, UNITY_FREERTOS_CPU);

    لكل (صحيح i = 0; i < 2; i++) {
        إذا (!xSemaphoreTake(انجامشده_sem, 10000 / پورت_TICK_PERIOD_MS)) {
            TEST_FAIL_پیام("done_sem not released by test task");
        }
    }

    BENCHMARK_انتها("cross-core incrementing");
    vSemaphoreDelete(انجامشده_sem);
    حرر(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * 2, shared_مقدار);
}

فراغ portmux_بالا_contention_test(uint32_t قفل_malloc_caps)
{
    ثابت صحيح مجموع_TASKS = 8; /* half on each core */
    shared_mux = انباشته_caps_malloc(حجم(پورت_MUX_TYPE), قفل_malloc_caps);
    انجامشده_sem = xSemaphoreCreateCounting(مجموع_TASKS, 0);
    پورت_MUX_INITIALIZE(shared_mux);
    shared_مقدار = 0;

    BENCHMARK_شروع();

    لكل (صحيح i = 0; i < مجموع_TASKS / 2; i++) {
        /* as each task has a higher priority than previous, expect
           them to preempt the earlier created task, at least on the
           other core (this core has the unity task, until that
           blocks)... */
        x_Task_Create_Pinned_مقصد_Core(task_shared_مقدار_increment, "INC0", 2048, عدم, tsk_IDLE_اولویت + 1 + i, عدم, UNITY_FREERTOS_CPU ? 0 : 1);
        x_Task_Create_Pinned_مقصد_Core(task_shared_مقدار_increment, "INC1", 2048, عدم, tsk_IDLE_اولویت + 1 + i, عدم, UNITY_FREERTOS_CPU);
    }

    لكل (صحيح i = 0; i < مجموع_TASKS; i++) {
        إذا (!xSemaphoreTake(انجامشده_sem, 10000 / پورت_TICK_PERIOD_MS)) {
            TEST_FAIL_پیام("done_sem not released by test task");
        }
    }

    BENCHMARK_انتها("cross-core high contention");
    vSemaphoreDelete(انجامشده_sem);
    حرر(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * مجموع_TASKS, shared_مقدار);
}

TEST_CASE("portMUX high contention", "[freertos]")
{
    portmux_بالا_contention_test(MALLOC_CAP_8_بیت | MALLOC_CAP_داخلی);
}

#إذا CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC
TEST_CASE("portMUX high contention, PSRAM", "[freertos]")
{
    portmux_بالا_contention_test(MALLOC_CAP_8_بیت | MALLOC_CAP_SPIRAM);
}
#پایان_اگر// CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC

#پایان_اگر // CONFIG_FREERTOS_NUMBER_OF_CORES == 2

#پایان_اگر // !CONFIG_FREERTOS_SMP
