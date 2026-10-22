/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Combined unit tests & benchmarking for spinlock "portMUX" functionality
*/

#అన్తర్భావ <esp_types.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <inttypes.h>
#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "freertos/semphr.h"
#అన్తర్భావ "freertos/queue.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "esp_cpu.h"

#అన్తర్భావ "test_utils.h"
#అన్తర్భావ "freertos_performance.h"

#ఒకవేళ !CONFIG_FREERTOS_SMP    // Known issue in Amazon SMP FreeRTOS port IDF-6204

#నిర్వచించు REPEAT_OPS 10000

స్థిర uint32_t ప్రారంభం, చివర;

#నిర్వచించు BENCHMARK_ప్రారంభం() చేయి {                  \
        ప్రారంభం = esp_cpu_get_చక్రం_లెక్క();                     \
    } అయితే(0)

#నిర్వచించు BENCHMARK_చివర(OPERATION) చేయి {                       \
        చివర = esp_cpu_get_చక్రం_లెక్క();                                          \
        రాయి("%s took %"PRIu32" cycles/op (%"PRIu32" cycles for %d ops)\n",     \
               OPERATION, (చివర - ప్రారంభం)/REPEAT_OPS,                 \
               (చివర - ప్రారంభం), REPEAT_OPS);                          \
    } అయితే(0)

TEST_CASE("portMUX spinlocks (no contention)", "[freertos]")
{
    పోర్టు_MUX_TYPE mux = పోర్టు_MUX_INITIALIZER_UNLOCKED;
    BENCHMARK_ప్రారంభం();

    కొరకు (పూర్ణాంకం i = 0; i < REPEAT_OPS; i++) {
        పోర్టు_ENTER_CRITICAL_ISR(&mux);
        పోర్టు_EXIT_CRITICAL_ISR(&mux);
    }
    BENCHMARK_చివర("no contention lock");

#ఒకవేళ CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_చక్రాలు_చొప్పున_OP_UNICORE, "%"PRIu32" cycles/op", ((చివర - ప్రారంభం) / REPEAT_OPS));
#లేకపోతే
#ఒకవేళ CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_చక్రాలు_చొప్పున_OP_PSRAM, "%"PRIu32" cycles/op", ((చివర - ప్రారంభం) / REPEAT_OPS));
#లేకపోతే
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_చక్రాలు_చొప్పున_OP, "%"PRIu32" cycles/op", ((చివర - ప్రారంభం) / REPEAT_OPS));
#అయితే_ముగింపు
#అయితే_ముగింపు
}

TEST_CASE("portMUX recursive locks (no contention)", "[freertos]")
{
    పోర్టు_MUX_TYPE mux = పోర్టు_MUX_INITIALIZER_UNLOCKED;
    BENCHMARK_ప్రారంభం();

    స్థిరరాశి పూర్ణాంకం RECURSE_లెక్క = 25;

    కొరకు (పూర్ణాంకం i = 0; i < REPEAT_OPS / RECURSE_లెక్క; i++) {
        కొరకు (పూర్ణాంకం j = 0; j < RECURSE_లెక్క; j++) {
            పోర్టు_ENTER_CRITICAL(&mux);
        }
        కొరకు (పూర్ణాంకం j = 0; j < RECURSE_లెక్క; j++) {
            పోర్టు_EXIT_CRITICAL(&mux);
        }
    }
    BENCHMARK_చివర("no contention recursive");

#ఒకవేళ CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_చక్రాలు_చొప్పున_OP_UNICORE, "%"PRIu32" cycles/op", ((చివర - ప్రారంభం) / REPEAT_OPS));
#లేకపోతే
#ఒకవేళ CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_చక్రాలు_చొప్పున_OP_PSRAM, "%"PRIu32" cycles/op", ((చివర - ప్రారంభం) / REPEAT_OPS));
#లేకపోతే
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_చక్రాలు_చొప్పున_OP, "%"PRIu32" cycles/op", ((చివర - ప్రారంభం) / REPEAT_OPS));
#అయితే_ముగింపు
#అయితే_ముగింపు
}

#ఒకవేళ CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES == 2

స్థిర అస్థిర పూర్ణాంకం shared_విలువ;
స్థిర పోర్టు_MUX_TYPE *shared_mux;
స్థిర SemaphoreHandle_t పూర్తి_sem;

స్థిర శూన్య task_shared_విలువ_increment(శూన్య *ignore)
{
    కొరకు (పూర్ణాంకం i = 0; i < REPEAT_OPS; i++) {
        పోర్టు_ENTER_CRITICAL(shared_mux);
        shared_విలువ++;
        పోర్టు_EXIT_CRITICAL(shared_mux);
    }
    xSemaphoreGive(పూర్తి_sem);
    vTaskDelete(ఖాళీ);
}

TEST_CASE("portMUX cross-core locking", "[freertos]")
{
    shared_mux = కుప్ప_caps_malloc(పరిమాణం(పోర్టు_MUX_TYPE), MALLOC_CAP_8_బిట్టు | MALLOC_CAP_అంతర్గత);
    పూర్తి_sem = xSemaphoreCreateCounting(2, 0);
    పోర్టు_MUX_INITIALIZE(shared_mux);
    shared_విలువ = 0;

    BENCHMARK_ప్రారంభం();

    x_Task_Create_Pinned_గమ్యం_Core(task_shared_విలువ_increment, "INC0", 2048, ఖాళీ, UNITY_FREERTOS_ప్రాధాన్యత + 1, ఖాళీ, UNITY_FREERTOS_CPU ? 0 : 1);
    x_Task_Create_Pinned_గమ్యం_Core(task_shared_విలువ_increment, "INC1", 2048, ఖాళీ, UNITY_FREERTOS_ప్రాధాన్యత + 1, ఖాళీ, UNITY_FREERTOS_CPU);

    కొరకు (పూర్ణాంకం i = 0; i < 2; i++) {
        ఒకవేళ (!xSemaphoreTake(పూర్తి_sem, 10000 / పోర్టు_TICK_PERIOD_MS)) {
            TEST_FAIL_సందేశం("done_sem not released by test task");
        }
    }

    BENCHMARK_చివర("cross-core incrementing");
    vSemaphoreDelete(పూర్తి_sem);
    విడుదల(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * 2, shared_విలువ);
}

శూన్య portmux_ఎగువ_contention_test(uint32_t తాళం_malloc_caps)
{
    స్థిరరాశి పూర్ణాంకం మొత్తం_TASKS = 8; /* half on each core */
    shared_mux = కుప్ప_caps_malloc(పరిమాణం(పోర్టు_MUX_TYPE), తాళం_malloc_caps);
    పూర్తి_sem = xSemaphoreCreateCounting(మొత్తం_TASKS, 0);
    పోర్టు_MUX_INITIALIZE(shared_mux);
    shared_విలువ = 0;

    BENCHMARK_ప్రారంభం();

    కొరకు (పూర్ణాంకం i = 0; i < మొత్తం_TASKS / 2; i++) {
        /* as each task has a higher priority than previous, expect
           them to preempt the earlier created task, at least on the
           other core (this core has the unity task, until that
           blocks)... */
        x_Task_Create_Pinned_గమ్యం_Core(task_shared_విలువ_increment, "INC0", 2048, ఖాళీ, tsk_IDLE_ప్రాధాన్యత + 1 + i, ఖాళీ, UNITY_FREERTOS_CPU ? 0 : 1);
        x_Task_Create_Pinned_గమ్యం_Core(task_shared_విలువ_increment, "INC1", 2048, ఖాళీ, tsk_IDLE_ప్రాధాన్యత + 1 + i, ఖాళీ, UNITY_FREERTOS_CPU);
    }

    కొరకు (పూర్ణాంకం i = 0; i < మొత్తం_TASKS; i++) {
        ఒకవేళ (!xSemaphoreTake(పూర్తి_sem, 10000 / పోర్టు_TICK_PERIOD_MS)) {
            TEST_FAIL_సందేశం("done_sem not released by test task");
        }
    }

    BENCHMARK_చివర("cross-core high contention");
    vSemaphoreDelete(పూర్తి_sem);
    విడుదల(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * మొత్తం_TASKS, shared_విలువ);
}

TEST_CASE("portMUX high contention", "[freertos]")
{
    portmux_ఎగువ_contention_test(MALLOC_CAP_8_బిట్టు | MALLOC_CAP_అంతర్గత);
}

#ఒకవేళ CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC
TEST_CASE("portMUX high contention, PSRAM", "[freertos]")
{
    portmux_ఎగువ_contention_test(MALLOC_CAP_8_బిట్టు | MALLOC_CAP_SPIRAM);
}
#అయితే_ముగింపు// CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC

#అయితే_ముగింపు // CONFIG_FREERTOS_NUMBER_OF_CORES == 2

#అయితే_ముగింపు // !CONFIG_FREERTOS_SMP
