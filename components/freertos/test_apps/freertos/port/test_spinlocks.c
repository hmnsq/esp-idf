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

#تعریف_کرو REPEAT_OPS 10000

ساکن uint32_t آغاز, انتہا;

#تعریف_کرو BENCHMARK_آغاز() کرو {                  \
        آغاز = esp_cpu_get_چکر_گنتی();                     \
    } جب(0)

#تعریف_کرو BENCHMARK_انتہا(OPERATION) کرو {                       \
        انتہا = esp_cpu_get_چکر_گنتی();                                          \
        لکھو("%s took %"PRIu32" cycles/op (%"PRIu32" cycles for %d ops)\n",     \
               OPERATION, (انتہا - آغاز)/REPEAT_OPS,                 \
               (انتہا - آغاز), REPEAT_OPS);                          \
    } جب(0)

TEST_CASE("portMUX spinlocks (no contention)", "[freertos]")
{
    پورٹ_MUX_TYPE mux = پورٹ_MUX_INITIALIZER_UNLOCKED;
    BENCHMARK_آغاز();

    برائے (صحیح i = 0; i < REPEAT_OPS; i++) {
        پورٹ_ENTER_CRITICAL_ISR(&mux);
        پورٹ_EXIT_CRITICAL_ISR(&mux);
    }
    BENCHMARK_انتہا("no contention lock");

#اگر CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_ادوار_فی_OP_UNICORE, "%"PRIu32" cycles/op", ((انتہا - آغاز) / REPEAT_OPS));
#ورنہ
#اگر CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_ادوار_فی_OP_PSRAM, "%"PRIu32" cycles/op", ((انتہا - آغاز) / REPEAT_OPS));
#ورنہ
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_ادوار_فی_OP, "%"PRIu32" cycles/op", ((انتہا - آغاز) / REPEAT_OPS));
#اگر_ختم
#اگر_ختم
}

TEST_CASE("portMUX recursive locks (no contention)", "[freertos]")
{
    پورٹ_MUX_TYPE mux = پورٹ_MUX_INITIALIZER_UNLOCKED;
    BENCHMARK_آغاز();

    ثابت صحیح RECURSE_گنتی = 25;

    برائے (صحیح i = 0; i < REPEAT_OPS / RECURSE_گنتی; i++) {
        برائے (صحیح j = 0; j < RECURSE_گنتی; j++) {
            پورٹ_ENTER_CRITICAL(&mux);
        }
        برائے (صحیح j = 0; j < RECURSE_گنتی; j++) {
            پورٹ_EXIT_CRITICAL(&mux);
        }
    }
    BENCHMARK_انتہا("no contention recursive");

#اگر CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_ادوار_فی_OP_UNICORE, "%"PRIu32" cycles/op", ((انتہا - آغاز) / REPEAT_OPS));
#ورنہ
#اگر CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_ادوار_فی_OP_PSRAM, "%"PRIu32" cycles/op", ((انتہا - آغاز) / REPEAT_OPS));
#ورنہ
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_ادوار_فی_OP, "%"PRIu32" cycles/op", ((انتہا - آغاز) / REPEAT_OPS));
#اگر_ختم
#اگر_ختم
}

#اگر CONFIG_FREERTOS_نمبر_کا_CORES == 2

ساکن متلون صحیح shared_قدر;
ساکن پورٹ_MUX_TYPE *shared_mux;
ساکن SemaphoreHandle_t مکمل_sem;

ساکن خالی task_shared_قدر_increment(خالی *ignore)
{
    برائے (صحیح i = 0; i < REPEAT_OPS; i++) {
        پورٹ_ENTER_CRITICAL(shared_mux);
        shared_قدر++;
        پورٹ_EXIT_CRITICAL(shared_mux);
    }
    xSemaphoreGive(مکمل_sem);
    vTaskDelete(تہی);
}

TEST_CASE("portMUX cross-core locking", "[freertos]")
{
    shared_mux = ڈھیر_caps_malloc(حجم(پورٹ_MUX_TYPE), MALLOC_CAP_8_ثنائی_ہندسہ | MALLOC_CAP_اندرونی);
    مکمل_sem = xSemaphoreCreateCounting(2, 0);
    پورٹ_MUX_INITIALIZE(shared_mux);
    shared_قدر = 0;

    BENCHMARK_آغاز();

    x_Task_Create_Pinned_مقصد_Core(task_shared_قدر_increment, "INC0", 2048, تہی, UNITY_FREERTOS_ترجیح + 1, تہی, UNITY_FREERTOS_CPU ? 0 : 1);
    x_Task_Create_Pinned_مقصد_Core(task_shared_قدر_increment, "INC1", 2048, تہی, UNITY_FREERTOS_ترجیح + 1, تہی, UNITY_FREERTOS_CPU);

    برائے (صحیح i = 0; i < 2; i++) {
        اگر (!xSemaphoreTake(مکمل_sem, 10000 / پورٹ_TICK_PERIOD_MS)) {
            TEST_FAIL_پیغام("done_sem not released by test task");
        }
    }

    BENCHMARK_انتہا("cross-core incrementing");
    vSemaphoreDelete(مکمل_sem);
    آزاد(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * 2, shared_قدر);
}

خالی portmux_بلند_contention_test(uint32_t تالا_malloc_caps)
{
    ثابت صحیح کل_TASKS = 8; /* half on each core */
    shared_mux = ڈھیر_caps_malloc(حجم(پورٹ_MUX_TYPE), تالا_malloc_caps);
    مکمل_sem = xSemaphoreCreateCounting(کل_TASKS, 0);
    پورٹ_MUX_INITIALIZE(shared_mux);
    shared_قدر = 0;

    BENCHMARK_آغاز();

    برائے (صحیح i = 0; i < کل_TASKS / 2; i++) {
        /* as each task has a higher priority than previous, expect
           them to preempt the earlier created task, at least on the
           other core (this core has the unity task, until that
           blocks)... */
        x_Task_Create_Pinned_مقصد_Core(task_shared_قدر_increment, "INC0", 2048, تہی, tsk_IDLE_ترجیح + 1 + i, تہی, UNITY_FREERTOS_CPU ? 0 : 1);
        x_Task_Create_Pinned_مقصد_Core(task_shared_قدر_increment, "INC1", 2048, تہی, tsk_IDLE_ترجیح + 1 + i, تہی, UNITY_FREERTOS_CPU);
    }

    برائے (صحیح i = 0; i < کل_TASKS; i++) {
        اگر (!xSemaphoreTake(مکمل_sem, 10000 / پورٹ_TICK_PERIOD_MS)) {
            TEST_FAIL_پیغام("done_sem not released by test task");
        }
    }

    BENCHMARK_انتہا("cross-core high contention");
    vSemaphoreDelete(مکمل_sem);
    آزاد(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * کل_TASKS, shared_قدر);
}

TEST_CASE("portMUX high contention", "[freertos]")
{
    portmux_بلند_contention_test(MALLOC_CAP_8_ثنائی_ہندسہ | MALLOC_CAP_اندرونی);
}

#اگر CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC
TEST_CASE("portMUX high contention, PSRAM", "[freertos]")
{
    portmux_بلند_contention_test(MALLOC_CAP_8_ثنائی_ہندسہ | MALLOC_CAP_SPIRAM);
}
#اگر_ختم// CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC

#اگر_ختم // CONFIG_FREERTOS_NUMBER_OF_CORES == 2

#اگر_ختم // !CONFIG_FREERTOS_SMP
