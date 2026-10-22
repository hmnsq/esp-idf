/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Combined unit tests & benchmarking for spinlock "portMUX" functionality
*/

#अन्तर्भाव <esp_types.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <inttypes.h>
#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"
#अन्तर्भाव "freertos/semphr.h"
#अन्तर्भाव "freertos/queue.h"
#अन्तर्भाव "unity.h"
#अन्तर्भाव "esp_cpu.h"

#अन्तर्भाव "test_utils.h"
#अन्तर्भाव "freertos_performance.h"

#यदि !CONFIG_FREERTOS_SMP    // Known issue in Amazon SMP FreeRTOS port IDF-6204

#परिभाषय REPEAT_OPS 10000

स्थिर uint32_t start, अन्तः_2;

#परिभाषय BENCHMARK_START() कुरु {                  \
        start = esp_cpu_get_आवृत्तिः_गणनम्();                     \
    } यावत्(0)

#परिभाषय BENCHMARK_अन्तः(OPERATION) कुरु {                       \
        अन्तः_2 = esp_cpu_get_आवृत्तिः_गणनम्();                                          \
        प्रारूप_लिख("%s took %"PRIu32" cycles/op (%"PRIu32" cycles for %d ops)\n",     \
               OPERATION, (अन्तः_2 - start)/REPEAT_OPS,                 \
               (अन्तः_2 - start), REPEAT_OPS);                          \
    } यावत्(0)

TEST_CASE("portMUX spinlocks (no contention)", "[freertos]")
{
    पत्तनम्_MUX_TYPE mux = पत्तनम्_MUX_INITIALIZER_UNLOCKED;
    BENCHMARK_START();

    प्रति (पूर्णाङ्क i = 0; i < REPEAT_OPS; i++) {
        पत्तनम्_ENTER_CRITICAL_ISR(&mux);
        पत्तनम्_EXIT_CRITICAL_ISR(&mux);
    }
    BENCHMARK_अन्तः("no contention lock");

#यदि CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_आवृत्तयः_प्रत्येकम्_OP_UNICORE, "%"PRIu32" cycles/op", ((अन्तः_2 - start) / REPEAT_OPS));
#अन्यथा
#यदि CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_आवृत्तयः_प्रत्येकम्_OP_PSRAM, "%"PRIu32" cycles/op", ((अन्तः_2 - start) / REPEAT_OPS));
#अन्यथा
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_आवृत्तयः_प्रत्येकम्_OP, "%"PRIu32" cycles/op", ((अन्तः_2 - start) / REPEAT_OPS));
#यदि_अन्तः
#यदि_अन्तः
}

TEST_CASE("portMUX recursive locks (no contention)", "[freertos]")
{
    पत्तनम्_MUX_TYPE mux = पत्तनम्_MUX_INITIALIZER_UNLOCKED;
    BENCHMARK_START();

    ध्रुव पूर्णाङ्क RECURSE_गणनम् = 25;

    प्रति (पूर्णाङ्क i = 0; i < REPEAT_OPS / RECURSE_गणनम्; i++) {
        प्रति (पूर्णाङ्क j = 0; j < RECURSE_गणनम्; j++) {
            पत्तनम्_ENTER_CRITICAL(&mux);
        }
        प्रति (पूर्णाङ्क j = 0; j < RECURSE_गणनम्; j++) {
            पत्तनम्_EXIT_CRITICAL(&mux);
        }
    }
    BENCHMARK_अन्तः("no contention recursive");

#यदि CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_आवृत्तयः_प्रत्येकम्_OP_UNICORE, "%"PRIu32" cycles/op", ((अन्तः_2 - start) / REPEAT_OPS));
#अन्यथा
#यदि CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_आवृत्तयः_प्रत्येकम्_OP_PSRAM, "%"PRIu32" cycles/op", ((अन्तः_2 - start) / REPEAT_OPS));
#अन्यथा
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_आवृत्तयः_प्रत्येकम्_OP, "%"PRIu32" cycles/op", ((अन्तः_2 - start) / REPEAT_OPS));
#यदि_अन्तः
#यदि_अन्तः
}

#यदि CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES == 2

स्थिर चञ्चल पूर्णाङ्क shared_मानम्;
स्थिर पत्तनम्_MUX_TYPE *shared_mux;
स्थिर SemaphoreHandle_t समाप्तम्_sem;

स्थिर शून्य task_shared_मानम्_increment(शून्य *ignore)
{
    प्रति (पूर्णाङ्क i = 0; i < REPEAT_OPS; i++) {
        पत्तनम्_ENTER_CRITICAL(shared_mux);
        shared_मानम्++;
        पत्तनम्_EXIT_CRITICAL(shared_mux);
    }
    xSemaphoreGive(समाप्तम्_sem);
    vTaskDelete(रिक्त);
}

TEST_CASE("portMUX cross-core locking", "[freertos]")
{
    shared_mux = heap_caps_malloc(परिमाण(पत्तनम्_MUX_TYPE), MALLOC_CAP_8BIT | MALLOC_CAP_आन्तरिकम्);
    समाप्तम्_sem = xSemaphoreCreateCounting(2, 0);
    पत्तनम्_MUX_INITIALIZE(shared_mux);
    shared_मानम् = 0;

    BENCHMARK_START();

    x_Task_Create_Pinned_गन्तव्यम्_Core(task_shared_मानम्_increment, "INC0", 2048, रिक्त, UNITY_FREERTOS_प्राथमिकता + 1, रिक्त, UNITY_FREERTOS_CPU ? 0 : 1);
    x_Task_Create_Pinned_गन्तव्यम्_Core(task_shared_मानम्_increment, "INC1", 2048, रिक्त, UNITY_FREERTOS_प्राथमिकता + 1, रिक्त, UNITY_FREERTOS_CPU);

    प्रति (पूर्णाङ्क i = 0; i < 2; i++) {
        यदि (!xSemaphoreTake(समाप्तम्_sem, 10000 / पत्तनम्_TICK_PERIOD_MS)) {
            TEST_FAIL_सन्देशः("done_sem not released by test task");
        }
    }

    BENCHMARK_अन्तः("cross-core incrementing");
    vSemaphoreDelete(समाप्तम्_sem);
    मुक्त(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * 2, shared_मानम्);
}

शून्य portmux_उच्चः_contention_test(uint32_t तालकम्_malloc_caps)
{
    ध्रुव पूर्णाङ्क योग_TASKS = 8; /* half on each core */
    shared_mux = heap_caps_malloc(परिमाण(पत्तनम्_MUX_TYPE), तालकम्_malloc_caps);
    समाप्तम्_sem = xSemaphoreCreateCounting(योग_TASKS, 0);
    पत्तनम्_MUX_INITIALIZE(shared_mux);
    shared_मानम् = 0;

    BENCHMARK_START();

    प्रति (पूर्णाङ्क i = 0; i < योग_TASKS / 2; i++) {
        /* as each task has a higher priority than previous, expect
           them to preempt the earlier created task, at least on the
           other core (this core has the unity task, until that
           blocks)... */
        x_Task_Create_Pinned_गन्तव्यम्_Core(task_shared_मानम्_increment, "INC0", 2048, रिक्त, tsk_IDLE_प्राथमिकता + 1 + i, रिक्त, UNITY_FREERTOS_CPU ? 0 : 1);
        x_Task_Create_Pinned_गन्तव्यम्_Core(task_shared_मानम्_increment, "INC1", 2048, रिक्त, tsk_IDLE_प्राथमिकता + 1 + i, रिक्त, UNITY_FREERTOS_CPU);
    }

    प्रति (पूर्णाङ्क i = 0; i < योग_TASKS; i++) {
        यदि (!xSemaphoreTake(समाप्तम्_sem, 10000 / पत्तनम्_TICK_PERIOD_MS)) {
            TEST_FAIL_सन्देशः("done_sem not released by test task");
        }
    }

    BENCHMARK_अन्तः("cross-core high contention");
    vSemaphoreDelete(समाप्तम्_sem);
    मुक्त(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * योग_TASKS, shared_मानम्);
}

TEST_CASE("portMUX high contention", "[freertos]")
{
    portmux_उच्चः_contention_test(MALLOC_CAP_8BIT | MALLOC_CAP_आन्तरिकम्);
}

#यदि CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC
TEST_CASE("portMUX high contention, PSRAM", "[freertos]")
{
    portmux_उच्चः_contention_test(MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);
}
#यदि_अन्तः// CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC

#यदि_अन्तः // CONFIG_FREERTOS_NUMBER_OF_CORES == 2

#यदि_अन्तः // !CONFIG_FREERTOS_SMP
