/*
 * SPDX-FileCopyrightText: 2024-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ <string.h>
#అన్తర్భావ "soc/soc_caps.h"
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "freertos/semphr.h"
#అన్తర్భావ "unity.h"
#ఒకవేళ SOC_CPU_HAS_HWLOOP
#అన్తర్భావ "esp_intr_alloc.h"
#అన్తర్భావ "esp_cpu.h"
#అన్తర్భావ "soc/system_intr.h"
#అన్తర్భావ "soc/system_reg.h"
#అయితే_ముగింపు

/**
 * On RISC-V targets that have coprocessors, the contexts are saved at the lowest address of the stack,
 * which can lead to wrong stack watermark calculation in FreeRTOS in theory.
 * As such, the port layer of FreeRTOS will adjust the lowest address of the stack when a coprocessor
 * context is saved.
 */
#ఒకవేళ SOC_CPU_HAS_HWLOOP

uint32_t use_hwlp(uint32_t లెక్క);

TEST_CASE("HWLP: Context save does not affect stack watermark", "[freertos]")
{
    /* Force the FreeRTOS port layer to store a HWLP context in the current task.
     * So let's use the it and make sure another task, on the SAME CORE, also uses it */
    స్థిరరాశి TaskHandle_t current_handle = xTaskGetCurrentTaskHandle();

    /* Get the current stack watermark */
    స్థిరరాశి UBaseType_t before_watermark = ux_Task_Get_దొంతర_ఎగువ_Water_Mark(current_handle);

    /* Use the HWLP unit, the context will NOT be flushed until a context switch is done */
    use_hwlp(20);

    /* Make sure FreeRTOS switches to another task, even Idle task, so that the current Task saves
     * the HWLP current context */
    vTaskDelay(10);

    స్థిరరాశి UBaseType_t after_watermark = ux_Task_Get_దొంతర_ఎగువ_Water_Mark(current_handle);

    TEST_ASSERT_TRUE(after_watermark > before_watermark / 2);
}

#ఒకవేళ CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES > 1

రకనిర్వచనం నిర్మాణం {
    uint32_t లెక్క;
    TaskHandle_t ప్రధాన;
} hwlp_params_t;

స్థిర శూన్య calculation(శూన్య* arg)
{
    hwlp_params_t* p = (hwlp_params_t*) arg;
    స్థిరరాశి uint32_t లెక్క = p->లెక్క;
    uint32_t ఫలితం = 0;
    పూర్ణాంకం i = 0;

    కొరకు (i = 0; i < 50000; i++) {
        uint32_t current = use_hwlp(లెక్క);
        ఫలితం += current;

        /* Using TEST_ASSERT_TRUE triggers a stack overflow, make sure the count is still correct.
         * The function `use_hwlp` should return (count * 16)  */
        ప్రతిజ్ఞా(లెక్క * 16 == current);
    }

    /* Make sure the result is correct */
    ప్రతిజ్ఞా(లెక్క * 16 * i == ఫలితం);

    xTaskNotifyGive(p->ప్రధాన);
    vTaskDelete(ఖాళీ);
}

TEST_CASE("HWLP: Unsolicited context switch between tasks using HWLP", "[freertos]")
{
    /* Create two tasks that are on the same core and use the same FPU */
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    TaskHandle_t tasks[2];
    hwlp_params_t params[2] = {
        { .లెక్క = 1024, .ప్రధాన = unity_task_handle },
        { .లెక్క = 2048, .ప్రధాన = unity_task_handle },
    };

    x_Task_Create_Pinned_గమ్యం_Core(calculation, "Task1", 2048, params + 0, CONFIG_UNITY_FREERTOS_ప్రాధాన్యత + 1, &tasks[0], 1);
    x_Task_Create_Pinned_గమ్యం_Core(calculation, "Task2", 2048, params + 1, CONFIG_UNITY_FREERTOS_ప్రాధాన్యత + 1, &tasks[1], 1);

    ulTaskNotifyTake(pdTRUE, పోర్టు_MAX_DELAY);
    ulTaskNotifyTake(pdTRUE, పోర్టు_MAX_DELAY);
}

#అయితే_ముగింపు /* CONFIG_FREERTOS_NUMBER_OF_CORES > 1 */

/*
 * Deterministic HWLOOP context-preservation test for any core with the hardware
 * loop unit.
 *
 * A "victim" runs a real hardware loop (esp.lp.setup) and pends a from-CPU
 * software interrupt from inside the loop body at both critical boundaries (loop
 * entry and just before the last instruction) on every iteration. The store only
 * requests the interrupt; delivery takes a few cycles, so the ISR typically
 * preempts near those windows rather than on the trigger instruction itself.
 * The ISR then yields to a higher-priority "contender" pinned to the same core,
 * which runs its own hardware loop and thus clobbers the shared HWLOOP registers.
 * When control returns to the victim, its loop must resume with the correct
 * context; if the live loop context was not saved (e.g. the ESP32-P4 rev < v3.0
 * state erratum), the victim's result will be wrong (or the loop will never
 * terminate).
 */
uint32_t use_hwlp_irq(uint32_t లెక్క, అస్థిర uint32_t *trig_reg, uint32_t trig_val);

స్థిర SemaphoreHandle_t s_go;
స్థిర అస్థిర uint32_t s_contender_runs;

స్థిర శూన్య from_cpu_isr(శూన్య *arg)
{
    (శూన్య) arg;
    /* Clear the from-CPU interrupt pending bit */
    REG_WRITE(SYSTEM_CPU_INTR_FROM_CPU_2_REG, 0);
    పునాది_Type_t yield = pdFALSE;
    xSemaphoreGiveFromISR(s_go, &yield);
    ఒకవేళ (yield) {
        పోర్టు_YIELD_FROM_ISR();
    }
}

స్థిర శూన్య hwlp_contender_task(శూన్య *arg)
{
    (శూన్య) arg;
    కొరకు (;;) {
        ఒకవేళ (xSemaphoreTake(s_go, పోర్టు_MAX_DELAY) == pdTRUE) {
            /* Use a different loop context to clobber the victim's live registers */
            (శూన్య) use_hwlp(333);
            s_contender_runs++;
        }
    }
}

TEST_CASE("HWLP: Deterministic preemption inside a live hardware loop", "[freertos]")
{
    s_go = x_Semaphore_Create_ద్విమాన();
    TEST_ASSERT_NOT_NULL(s_go);
    s_contender_runs = 0;

    intr_handle_t ih;
    TEST_ESP_OK(esp_intr_alloc(SYS_CPU_INTR_FROM_CPU_2_ఆధారం, ESP_INTR_FLAG_LEVEL1, from_cpu_isr, ఖాళీ, &ih));

    స్థిరరాశి పునాది_Type_t core = x_పోర్టు_Get_Core_ID();
    TaskHandle_t contender = ఖాళీ;
    x_Task_Create_Pinned_గమ్యం_Core(hwlp_contender_task, "hwlpC", 2048, ఖాళీ, CONFIG_UNITY_FREERTOS_ప్రాధాన్యత + 2, &contender, core);

    స్థిరరాశి uint32_t లెక్క = 64;
    bool corrupted = false;
    uint32_t మొదటి_bad = 0;

    కొరకు (పూర్ణాంకం i = 0; i < 2000 && !corrupted; i++) {
        uint32_t r = use_hwlp_irq(లెక్క, (అస్థిర uint32_t *)SYSTEM_CPU_INTR_FROM_CPU_2_REG, SYSTEM_CPU_INTR_FROM_CPU_2);
        ఒకవేళ (r != లెక్క * 16) {
            corrupted = true;
            మొదటి_bad = r;
        }
        /* Give the contender a chance to drain if it is still pending */
        ఒకవేళ ((i & 0x3f) == 0) {
            taskYIELD();
        }
    }

    vTaskDelete(contender);
    esp_intr_free(ih);
    vSemaphoreDelete(s_go);

    రాయి("HWLP deterministic preemption: contender_runs=%u, result=%s (bad=%u)\n", (గుర్తులేని)s_contender_runs, corrupted ? "CORRUPTED" : "ok", (గుర్తులేని)మొదటి_bad);

    TEST_ASSERT_TRUE_సందేశం(s_contender_runs > 0, "Contender never ran: the mid-loop interrupt was not delivered");
    TEST_ASSERT_FALSE_సందేశం(corrupted, "HWLOOP context corrupted by mid-loop preemption (erratum reproduced)");
}

/*
 * Manual reproduction aid focused on the alignment constraint only:
 * use_hwlp_broken() misaligns both the first and the last hardware-loop
 * instruction (the exact thing the esp-dsp .balign 4 patch fixes) and is
 * preempted at its boundaries. Body size and instruction types are kept valid.
 * Marked [ignore] because it will fault/hang.
 */
uint32_t use_hwlp_broken(uint32_t లెక్క, అస్థిర uint32_t *trig_reg, uint32_t trig_val);

TEST_CASE("HWLP: Violate loop alignment/size constraints", "[freertos][ignore]")
{
    s_go = x_Semaphore_Create_ద్విమాన();
    TEST_ASSERT_NOT_NULL(s_go);
    s_contender_runs = 0;

    intr_handle_t ih;
    TEST_ESP_OK(esp_intr_alloc(SYS_CPU_INTR_FROM_CPU_2_ఆధారం, ESP_INTR_FLAG_LEVEL1, from_cpu_isr, ఖాళీ, &ih));

    స్థిరరాశి పునాది_Type_t core = x_పోర్టు_Get_Core_ID();
    TaskHandle_t contender = ఖాళీ;
    x_Task_Create_Pinned_గమ్యం_Core(hwlp_contender_task, "hwlpC", 2048, ఖాళీ, CONFIG_UNITY_FREERTOS_ప్రాధాన్యత + 2, &contender, core);

    bool bad = false;
    uint32_t మొదటి_bad = 0;
    కొరకు (పూర్ణాంకం i = 0; i < 100000 && !bad; i++) {
        uint32_t r = use_hwlp_broken(64, (అస్థిర uint32_t *)SYSTEM_CPU_INTR_FROM_CPU_2_REG, SYSTEM_CPU_INTR_FROM_CPU_2);
        ఒకవేళ (r != 64 * 9) {
            bad = true;
            మొదటి_bad = r;
        }
        ఒకవేళ ((i & 0x3f) == 0) {
            taskYIELD();
        }
    }

    vTaskDelete(contender);
    esp_intr_free(ih);
    vSemaphoreDelete(s_go);
    రాయి("use_hwlp_broken (misaligned boundaries): contender_runs=%u result=%s (bad=%u)\n", (గుర్తులేని)s_contender_runs, bad ? "CORRUPTED" : "ok", (గుర్తులేని)మొదటి_bad);
}

#అయితే_ముగింపు // SOC_CPU_HAS_HWLOOP
