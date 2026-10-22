/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Combined unit tests & benchmarking for spinlock "portMUX" functionality
*/

#समावेश <esp_types.h>
#समावेश <stdio.h>
#समावेश <inttypes.h>
#समावेश "sdkconfig.h"
#समावेश "freertos/FreeRTOS.h"
#समावेश "freertos/task.h"
#समावेश "freertos/semphr.h"
#समावेश "freertos/queue.h"
#समावेश "unity.h"
#समावेश "esp_cpu.h"

#समावेश "test_utils.h"
#समावेश "freertos_performance.h"

#अगर !CONFIG_FREERTOS_SMP    // Known issue in Amazon SMP FreeRTOS port IDF-6204

#घोषणा REPEAT_OPS 10000

अटल uint32_t start, end;

#घोषणा BENCHMARK_START() करो {                  \
        start = esp_cpu_get_cycle_गिनती();                     \
    } जबतक(0)

#घोषणा BENCHMARK_END(OPERATION) करो {                       \
        end = esp_cpu_get_cycle_गिनती();                                          \
        म_लिखो("%s took %"PRIu32" cycles/op (%"PRIu32" cycles for %d ops)\n",     \
               OPERATION, (end - start)/REPEAT_OPS,                 \
               (end - start), REPEAT_OPS);                          \
    } जबतक(0)

TEST_CASE("portMUX spinlocks (no contention)", "[freertos]")
{
    portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
    BENCHMARK_START();

    क्रम (पूर्णांक i = 0; i < REPEAT_OPS; i++) {
        portENTER_CRITICAL_ISR(&mux);
        portEXIT_CRITICAL_ISR(&mux);
    }
    BENCHMARK_END("no contention lock");

#अगर CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_UNICORE, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#अन्यथा
#अगर CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_PSRAM, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#अन्यथा
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#पूर्ण_अगर
#पूर्ण_अगर
}

TEST_CASE("portMUX recursive locks (no contention)", "[freertos]")
{
    portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
    BENCHMARK_START();

    स्थिर पूर्णांक RECURSE_गिनती = 25;

    क्रम (पूर्णांक i = 0; i < REPEAT_OPS / RECURSE_गिनती; i++) {
        क्रम (पूर्णांक j = 0; j < RECURSE_गिनती; j++) {
            portENTER_CRITICAL(&mux);
        }
        क्रम (पूर्णांक j = 0; j < RECURSE_गिनती; j++) {
            portEXIT_CRITICAL(&mux);
        }
    }
    BENCHMARK_END("no contention recursive");

#अगर CONFIG_FREERTOS_UNICORE
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_UNICORE, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#अन्यथा
#अगर CONFIG_SPIRAM
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP_PSRAM, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#अन्यथा
    TEST_PERFORMANCE_LESS_THAN(FREERTOS_SPINLOCK_CYCLES_PER_OP, "%"PRIu32" cycles/op", ((end - start) / REPEAT_OPS));
#पूर्ण_अगर
#पूर्ण_अगर
}

#अगर CONFIG_FREERTOS_संख्या_OF_CORES == 2

अटल अस्थिर पूर्णांक shared_मूल्य;
अटल portMUX_TYPE *shared_mux;
अटल SemaphoreHandle_t done_sem;

अटल व्योम task_shared_मूल्य_increment(व्योम *ignore)
{
    क्रम (पूर्णांक i = 0; i < REPEAT_OPS; i++) {
        portENTER_CRITICAL(shared_mux);
        shared_मूल्य++;
        portEXIT_CRITICAL(shared_mux);
    }
    xSemaphoreGive(done_sem);
    vTaskDelete(शून्य);
}

TEST_CASE("portMUX cross-core locking", "[freertos]")
{
    shared_mux = heap_caps_malloc(माप(portMUX_TYPE), MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL);
    done_sem = xSemaphoreCreateCounting(2, 0);
    portMUX_INITIALIZE(shared_mux);
    shared_मूल्य = 0;

    BENCHMARK_START();

    xTaskCreatePinnedToCore(task_shared_मूल्य_increment, "INC0", 2048, शून्य, UNITY_FREERTOS_PRIORITY + 1, शून्य, UNITY_FREERTOS_CPU ? 0 : 1);
    xTaskCreatePinnedToCore(task_shared_मूल्य_increment, "INC1", 2048, शून्य, UNITY_FREERTOS_PRIORITY + 1, शून्य, UNITY_FREERTOS_CPU);

    क्रम (पूर्णांक i = 0; i < 2; i++) {
        अगर (!xSemaphoreTake(done_sem, 10000 / portTICK_PERIOD_MS)) {
            TEST_FAIL_MESSAGE("done_sem not released by test task");
        }
    }

    BENCHMARK_END("cross-core incrementing");
    vSemaphoreDelete(done_sem);
    मुक्त(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * 2, shared_मूल्य);
}

व्योम portmux_ऊपरी_contention_test(uint32_t lock_malloc_caps)
{
    स्थिर पूर्णांक योग_TASKS = 8; /* half on each core */
    shared_mux = heap_caps_malloc(माप(portMUX_TYPE), lock_malloc_caps);
    done_sem = xSemaphoreCreateCounting(योग_TASKS, 0);
    portMUX_INITIALIZE(shared_mux);
    shared_मूल्य = 0;

    BENCHMARK_START();

    क्रम (पूर्णांक i = 0; i < योग_TASKS / 2; i++) {
        /* as each task has a higher priority than previous, expect
           them to preempt the earlier created task, at least on the
           other core (this core has the unity task, until that
           blocks)... */
        xTaskCreatePinnedToCore(task_shared_मूल्य_increment, "INC0", 2048, शून्य, tskIDLE_PRIORITY + 1 + i, शून्य, UNITY_FREERTOS_CPU ? 0 : 1);
        xTaskCreatePinnedToCore(task_shared_मूल्य_increment, "INC1", 2048, शून्य, tskIDLE_PRIORITY + 1 + i, शून्य, UNITY_FREERTOS_CPU);
    }

    क्रम (पूर्णांक i = 0; i < योग_TASKS; i++) {
        अगर (!xSemaphoreTake(done_sem, 10000 / portTICK_PERIOD_MS)) {
            TEST_FAIL_MESSAGE("done_sem not released by test task");
        }
    }

    BENCHMARK_END("cross-core high contention");
    vSemaphoreDelete(done_sem);
    मुक्त(shared_mux);

    TEST_ASSERT_EQUAL_INT(REPEAT_OPS * योग_TASKS, shared_मूल्य);
}

TEST_CASE("portMUX high contention", "[freertos]")
{
    portmux_ऊपरी_contention_test(MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL);
}

#अगर CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC
TEST_CASE("portMUX high contention, PSRAM", "[freertos]")
{
    portmux_ऊपरी_contention_test(MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);
}
#पूर्ण_अगर// CONFIG_SPIRAM_USE_MALLOC || CONFIG_SPIRAM_USE_CAPS_ALLOC

#पूर्ण_अगर // CONFIG_FREERTOS_NUMBER_OF_CORES == 2

#पूर्ण_अगर // !CONFIG_FREERTOS_SMP
