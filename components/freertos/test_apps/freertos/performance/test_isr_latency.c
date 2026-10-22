/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ <esp_types.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <inttypes.h>
#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "freertos/semphr.h"
#అన్తర్భావ "freertos/queue.h"
#అన్తర్భావ "esp_intr_alloc.h"
#అన్తర్భావ "esp_cpu.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "test_utils.h"
#అన్తర్భావ "freertos_performance.h"

#ఒకవేళ CONFIG_IDF_లక్ష్యం_ARCH_XTENSA
#అన్తర్భావ "xtensa/hal.h"
#అన్తర్భావ "xtensa_api.h"     // Replace with interrupt allocator API (IDF-3891)
#నిర్వచించు TEST_సమితి_INT_ముసుగు(ముసుగు) xt_సమితి_intset(ముసుగు)
#నిర్వచించు TEST_CLR_INT_ముసుగు(ముసుగు) xt_సమితి_intclear(ముసుగు)
#లేదావేళ CONFIG_IDF_లక్ష్యం_ARCH_RISCV
#అన్తర్భావ "riscv/interrupt.h"
#నిర్వచించు TEST_సమితి_INT_ముసుగు(ముసుగు) esprv_int_enable(ముసుగు)
#నిర్వచించు TEST_CLR_INT_ముసుగు(ముసుగు) esprv_int_disable(ముసుగు)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే __riscv // TODO: IDF-4416

#నిర్వచించు SW_ISR_LEVEL_1  7

స్థిర SemaphoreHandle_t sync;
స్థిర SemaphoreHandle_t చివర_sema;
స్థిర uint32_t చక్రం_before_trigger;
స్థిర uint32_t చక్రం_before_exit;
స్థిర uint32_t delta_enter_చక్రాలు = 0;
స్థిర uint32_t delta_exit_చక్రాలు = 0;

స్థిర శూన్య software_isr_using_parameter_vportyield(శూన్య *arg)
{
    (శూన్య)arg;
    పునాది_Type_t yield;
    delta_enter_చక్రాలు += esp_cpu_get_చక్రం_లెక్క() - చక్రం_before_trigger;
    TEST_CLR_INT_ముసుగు(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    పోర్టు_YIELD_FROM_ISR(yield);

    చక్రం_before_exit = esp_cpu_get_చక్రం_లెక్క();
}

స్థిర శూన్య software_isr_using_no_వాదం_vportyield(శూన్య *arg)
{
    (శూన్య)arg;
    పునాది_Type_t yield;
    delta_enter_చక్రాలు += esp_cpu_get_చక్రం_లెక్క() - చక్రం_before_trigger;

    TEST_CLR_INT_ముసుగు(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    ఒకవేళ (yield) {
        పోర్టు_YIELD_FROM_ISR();
    }
    చక్రం_before_exit = esp_cpu_get_చక్రం_లెక్క();
}

స్థిర శూన్య test_task(శూన్య *arg)
{
    (శూన్య) arg;

    కొరకు (పూర్ణాంకం i = 0; i < 10000; i++) {
        చక్రం_before_trigger = esp_cpu_get_చక్రం_లెక్క();
        TEST_సమితి_INT_ముసుగు(1 << SW_ISR_LEVEL_1);
        xSemaphoreTake(sync, పోర్టు_MAX_DELAY);
        delta_exit_చక్రాలు += esp_cpu_get_చక్రం_లెక్క() - చక్రం_before_exit;
    }

    delta_enter_చక్రాలు /= 10000;
    delta_exit_చక్రాలు /= 10000;

    xSemaphoreGive(చివర_sema);
    vTaskDelete(ఖాళీ);
}

TEST_CASE("isr latency test vport-yield-from-isr with no parameter", "[freertos] [ignore]")
{
    intr_handle_t handle;
    esp_err_t err = esp_intr_alloc(ETS_అంతర్గత_SW0_INTR_ఆధారం, ESP_INTR_FLAG_LEVEL1, &software_isr_using_no_వాదం_vportyield, ఖాళీ, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_ద్విమాన();
    TEST_ASSERT(sync != ఖాళీ);
    చివర_sema = x_Semaphore_Create_ద్విమాన();
    TEST_ASSERT(చివర_sema != ఖాళీ);
    x_Task_Create_Pinned_గమ్యం_Core(test_task, "tst", 4096, ఖాళీ, configMAX_PRIORITIES - 1, ఖాళీ, 0);
    vTaskDelay(100);
    పునాది_Type_t ఫలితం = xSemaphoreTake(చివర_sema, పోర్టు_MAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, ఫలితం);
    TEST_PERFORMANCE_LESS_THAN(ISR_ENTER_చక్రాలు, "%"PRIu32" cycles", delta_enter_చక్రాలు);
    TEST_PERFORMANCE_LESS_THAN(ISR_EXIT_చక్రాలు, "%"PRIu32" cycles", delta_exit_చక్రాలు);

    esp_intr_free(handle);
}

TEST_CASE("isr latency test vport-yield-from-isr with parameter", "[freertos][ignore]")
{
    intr_handle_t handle;
    esp_err_t err = esp_intr_alloc(ETS_అంతర్గత_SW0_INTR_ఆధారం, ESP_INTR_FLAG_LEVEL1, &software_isr_using_parameter_vportyield, ఖాళీ, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_ద్విమాన();
    TEST_ASSERT(sync != ఖాళీ);
    చివర_sema = x_Semaphore_Create_ద్విమాన();
    TEST_ASSERT(చివర_sema != ఖాళీ);
    x_Task_Create_Pinned_గమ్యం_Core(test_task, "tst", 4096, ఖాళీ, configMAX_PRIORITIES - 1, ఖాళీ, 0);
    పునాది_Type_t ఫలితం = xSemaphoreTake(చివర_sema, పోర్టు_MAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, ఫలితం);
    TEST_PERFORMANCE_LESS_THAN(ISR_ENTER_చక్రాలు, "%"PRIu32" cycles", delta_enter_చక్రాలు);
    TEST_PERFORMANCE_LESS_THAN(ISR_EXIT_చక్రాలు, "%"PRIu32" cycles", delta_exit_చక్రాలు);

    esp_intr_free(handle);
}

#అయితే_ముగింపు // __riscv
