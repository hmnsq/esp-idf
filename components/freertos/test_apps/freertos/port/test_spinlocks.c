/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Combined unit tests & benchmarking for spinlock "portMUX" functionality
*/

#ଅନ୍ତର୍ଭାଵ <esp_types.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <inttypes.h>
#ଅନ୍ତର୍ଭାଵ "sdkconfig.h"
#ଅନ୍ତର୍ଭାଵ "freertos/FreeRTOS.h"
#ଅନ୍ତର୍ଭାଵ "freertos/task.h"
#ଅନ୍ତର୍ଭାଵ "freertos/semphr.h"
#ଅନ୍ତର୍ଭାଵ "freertos/queue.h"
#ଅନ୍ତର୍ଭାଵ "unity.h"
#ଅନ୍ତର୍ଭାଵ "esp_cpu.h"

#ଅନ୍ତର୍ଭାଵ "test_utils.h"
#ଅନ୍ତର୍ଭାଵ "freertos_performance.h"

#ଯଦି !CONFIG_FREERTOS_SMP    // Known issue in Amazon SMP FreeRTOS port IDF-6204

#define REPEAT_OPS 10000

ସ୍ଥିର uint32_t start, end;

#define BENCHMARK_START() କର {                  \
        start = esp_cpu_get_cycle_ଗଣନା();                     \
    } ଯେତେବେଳେ(0)

#define BENCHMARK_END(OPERATION) କର {                       \
        end = esp_cpu_get_cycle_ଗଣନା();                                          \
        printf("%s took %"PRIu32" cycles/op (%"PRIu32" cycles for %d ops)\n",     \
               OPERATION, (end - start)/REPEAT_OPS,                 \
               (end - start), REPEAT_OPS);                          \
    } ଯେତେବେଳେ(0)

TEST_CASE("portMUX spinlocks (no contention)", "[freertos]")
{
    portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
    BENCHMARK_START();

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < REPEAT_OPS; i++) {
        portENTER_CRITICAL_ISR(&mux);
        portEXIT_CRITICAL_ISR(&mux);
    }
    BENCHMARK_END("no contention lock");

#ଯଦି CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_UNICORE, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#ଅନ୍ୟଥା
#ଯଦି CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_PSRAM, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#ଅନ୍ୟଥା
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#endif
#endif
}

TEST_CASE("portMUX recursive locks (no contention)", "[freertos]")
{
    portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
    BENCHMARK_START();

    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା RECURSE_ଗଣନା = 25;

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < REPEAT_OPS / RECURSE_ଗଣନା; i++) {
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < RECURSE_ଗଣନା; j++) {
            portENTER_CRITICAL(&mux);
        }
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < RECURSE_ଗଣନା; j++) {
            portEXIT_CRITICAL(&mux);
        }
    }
    BENCHMARK_END("no contention recursive");

#ଯଦି CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_UNICORE, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#ଅନ୍ୟଥା
#ଯଦି CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_PSRAM, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#ଅନ୍ୟଥା
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#endif
#endif
}

#ଯଦି CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES == 2

ସ୍ଥିର ଅସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା shared_ମୂଲ୍ୟ;
ସ୍ଥିର portMUX_TYPE *shared_mux;
ସ୍ଥିର SemaphoreHandle_t done_sem;

ସ୍ଥିର ଶୂନ୍ୟ task_shared_ମୂଲ୍ୟ_increment(ଶୂନ୍ୟ *ignore)
{
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < REPEAT_OPS; i++) {
        portENTER_CRITICAL(shared_mux);
        shared_ମୂଲ୍ୟ++;
        portEXIT_CRITICAL(shared_mux);
    }
    xSemaphoreGive(done_sem);
    vTaskDelete(ଖାଲି);
}

TEST_CASE("portMUX cross-core locking", "[freertos]")
{
    shared_mux = heap_caps_malloc(ଆକାର(portMUX_TYPE), MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL);
    done_sem = xSemaphoreCreateCounting(2, 0);
    portMUX_INITIALIZE(shared_mux);
    shared_ମୂଲ୍ୟ = 0;

    BENCHMARK_START();

    xTaskCreatePinnedToCore(task_shared_ମୂଲ୍ୟ_increment, "INC0", 2048, ଖାଲି, UNITY_FREERTOS_PRIORITY + 1, ଖାଲି, UNITY_FREERTOS_CPU ? 0 : 1);
    xTaskCreatePinnedToCore(task_shared_ମୂଲ୍ୟ_increment, "INC1", 2048, ଖାଲି, UNITY_FREERTOS_PRIORITY + 1, ଖାଲି, UNITY_FREERTOS_CPU);

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < 2; i++) {
        ଯଦି (!xSemaphoreTake(done_sem, 10000 / portTICK_PERIOD_MS)) {
            TEST_FAIL_MESSAGE("done_sem not released by test task");
        }
    }

    BENCHMARK_END("cross-core incrementing");
    vSemaphoreDelete(done_sem);
    ମୁକ୍ତ(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * 2, shared_ମୂଲ୍ୟ);
}

ଶୂନ୍ୟ portmux_ଉପର_contention_test(uint32_t lock_malloc_caps)
{
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା TOTAL_TASKS = 8; /* half on each core */
    shared_mux = heap_caps_malloc(ଆକାର(portMUX_TYPE), lock_malloc_caps);
    done_sem = xSemaphoreCreateCounting(TOTAL_TASKS, 0);
    portMUX_INITIALIZE(shared_mux);
    shared_ମୂଲ୍ୟ = 0;

    BENCHMARK_START();

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < TOTAL_TASKS / 2; i++) {
        /* as each task has a higher priority than previous, expect
           them to preempt the earlier created task, at least on the
           other core (this core has the unity task, until that
           blocks)... */
        xTaskCreatePinnedToCore(task_shared_ମୂଲ୍ୟ_increment, "INC0", 2048, ଖାଲି, tskIDLE_PRIORITY + 1 + i, ଖାଲି, UNITY_FREERTOS_CPU ? 0 : 1);
        xTaskCreatePinnedToCore(task_shared_ମୂଲ୍ୟ_increment, "INC1", 2048, ଖାଲି, tskIDLE_PRIORITY + 1 + i, ଖାଲି, UNITY_FREERTOS_CPU);
    }

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < TOTAL_TASKS; i++) {
        ଯଦି (!xSemaphoreTake(done_sem, 10000 / portTICK_PERIOD_MS)) {
            TEST_FAIL_MESSAGE("done_sem not released by test task");
        }
    }

    BENCHMARK_END("cross-core high contention");
    vSemaphoreDelete(done_sem);
    ମୁକ୍ତ(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * TOTAL_TASKS, shared_ମୂଲ୍ୟ);
}

TEST_CASE("portMUX high contention", "[freertos]")
{
    portmux_ଉପର_contention_test(MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL);
}

#ଯଦି CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC
TEST_CASE("portMUX high contention, PSRAM", "[freertos]")
{
    portmux_ଉପର_contention_test(MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);
}
#endif// CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC

#endif // CONFIG_FREERTOS_NUMBER_OF_CORES == 2

#endif // !CONFIG_FREERTOS_SMP
