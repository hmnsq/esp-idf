/*
 * SPDX-FileCopyrightText: 2024-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ "soc/soc_caps.h"
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "freertos/semphr.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಆದರೆ SOC_CPU_HAS_HWLOOP
#ಅನ್ತರ್ಭಾವ "esp_intr_alloc.h"
#ಅನ್ತರ್ಭಾವ "esp_cpu.h"
#ಅನ್ತರ್ಭಾವ "soc/system_intr.h"
#ಅನ್ತರ್ಭಾವ "soc/system_reg.h"
#ಆದರೆ_ಅಂತ್ಯ

/**
 * On RISC-V targets that have coprocessors, the contexts are saved at the lowest address of the stack,
 * which can lead to wrong stack watermark calculation in FreeRTOS in theory.
 * As such, the port layer of FreeRTOS will adjust the lowest address of the stack when a coprocessor
 * context is saved.
 */
#ಆದರೆ SOC_CPU_HAS_HWLOOP

uint32_t use_hwlp(uint32_t ಎಣಿಕೆ_2);

TEST_CASE("HWLP: Context save does not affect stack watermark", "[freertos]")
{
    /* Force the FreeRTOS port layer to store a HWLP context in the current task.
     * So let's use the it and make sure another task, on the SAME CORE, also uses it */
    ಸ್ಥಿರಾಂಕ TaskHandle_t current_handle = xTaskGetCurrentTaskHandle();

    /* Get the current stack watermark */
    ಸ್ಥಿರಾಂಕ UBaseType_t before_watermark = ux_Task_Get_ರಾಶಿ_ಮೇಲಿನ_Water_Mark(current_handle);

    /* Use the HWLP unit, the context will NOT be flushed until a context switch is done */
    use_hwlp(20);

    /* Make sure FreeRTOS switches to another task, even Idle task, so that the current Task saves
     * the HWLP current context */
    vTaskDelay(10);

    ಸ್ಥಿರಾಂಕ UBaseType_t after_watermark = ux_Task_Get_ರಾಶಿ_ಮೇಲಿನ_Water_Mark(current_handle);

    TEST_ASSERT_TRUE(after_watermark > before_watermark / 2);
}

#ಆದರೆ CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES > 1

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    uint32_t ಎಣಿಕೆ_2;
    TaskHandle_t ಮುಖ್ಯ;
} hwlp_params_t;

ಸ್ಥಿರ ಶೂನ್ಯ calculation(ಶೂನ್ಯ* arg)
{
    hwlp_params_t* p = (hwlp_params_t*) arg;
    ಸ್ಥಿರಾಂಕ uint32_t ಎಣಿಕೆ_2 = p->ಎಣಿಕೆ_2;
    uint32_t ಫಲಿತಾಂಶ = 0;
    ಪೂರ್ಣಾಂಕ i = 0;

    ಗಾಗಿ (i = 0; i < 50000; i++) {
        uint32_t current = use_hwlp(ಎಣಿಕೆ_2);
        ಫಲಿತಾಂಶ += current;

        /* Using TEST_ASSERT_TRUE triggers a stack overflow, make sure the count is still correct.
         * The function `use_hwlp` should return (count * 16)  */
        ಪ್ರತಿಜ್ಞಾ(ಎಣಿಕೆ_2 * 16 == current);
    }

    /* Make sure the result is correct */
    ಪ್ರತಿಜ್ಞಾ(ಎಣಿಕೆ_2 * 16 * i == ಫಲಿತಾಂಶ);

    xTaskNotifyGive(p->ಮುಖ್ಯ);
    vTaskDelete(ಖಾಲಿ);
}

TEST_CASE("HWLP: Unsolicited context switch between tasks using HWLP", "[freertos]")
{
    /* Create two tasks that are on the same core and use the same FPU */
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    TaskHandle_t tasks[2];
    hwlp_params_t params[2] = {
        { .ಎಣಿಕೆ_2 = 1024, .ಮುಖ್ಯ = unity_task_handle },
        { .ಎಣಿಕೆ_2 = 2048, .ಮುಖ್ಯ = unity_task_handle },
    };

    x_Task_Create_Pinned_ಗಮ್ಯ_Core(calculation, "Task1", 2048, params + 0, CONFIG_UNITY_FREERTOS_ಆದ್ಯತೆ + 1, &tasks[0], 1);
    x_Task_Create_Pinned_ಗಮ್ಯ_Core(calculation, "Task2", 2048, params + 1, CONFIG_UNITY_FREERTOS_ಆದ್ಯತೆ + 1, &tasks[1], 1);

    ulTaskNotifyTake(pdTRUE, ಪೋರ್ಟ್_MAX_DELAY);
    ulTaskNotifyTake(pdTRUE, ಪೋರ್ಟ್_MAX_DELAY);
}

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_NUMBER_OF_CORES > 1 */

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
uint32_t use_hwlp_irq(uint32_t ಎಣಿಕೆ_2, ಅಸ್ಥಿರ uint32_t *trig_reg, uint32_t trig_val);

ಸ್ಥಿರ SemaphoreHandle_t s_go;
ಸ್ಥಿರ ಅಸ್ಥಿರ uint32_t s_contender_runs;

ಸ್ಥಿರ ಶೂನ್ಯ from_cpu_isr(ಶೂನ್ಯ *arg)
{
    (ಶೂನ್ಯ) arg;
    /* Clear the from-CPU interrupt pending bit */
    REG_WRITE(SYSTEM_CPU_INTR_FROM_CPU_2_REG, 0);
    ತಳ_Type_t yield = pdFALSE;
    xSemaphoreGiveFromISR(s_go, &yield);
    ಆದರೆ (yield) {
        ಪೋರ್ಟ್_YIELD_FROM_ISR();
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ hwlp_contender_task(ಶೂನ್ಯ *arg)
{
    (ಶೂನ್ಯ) arg;
    ಗಾಗಿ (;;) {
        ಆದರೆ (xSemaphoreTake(s_go, ಪೋರ್ಟ್_MAX_DELAY) == pdTRUE) {
            /* Use a different loop context to clobber the victim's live registers */
            (ಶೂನ್ಯ) use_hwlp(333);
            s_contender_runs++;
        }
    }
}

TEST_CASE("HWLP: Deterministic preemption inside a live hardware loop", "[freertos]")
{
    s_go = x_Semaphore_Create_ದ್ವಿಮಾನ();
    TEST_ASSERT_NOT_NULL(s_go);
    s_contender_runs = 0;

    intr_handle_t ih;
    TEST_ESP_OK(esp_intr_alloc(SYS_CPU_INTR_FROM_CPU_2_ಮೂಲ, ESP_INTR_FLAG_LEVEL1, from_cpu_isr, ಖಾಲಿ, &ih));

    ಸ್ಥಿರಾಂಕ ತಳ_Type_t core = x_ಪೋರ್ಟ್_Get_Core_ID();
    TaskHandle_t contender = ಖಾಲಿ;
    x_Task_Create_Pinned_ಗಮ್ಯ_Core(hwlp_contender_task, "hwlpC", 2048, ಖಾಲಿ, CONFIG_UNITY_FREERTOS_ಆದ್ಯತೆ + 2, &contender, core);

    ಸ್ಥಿರಾಂಕ uint32_t ಎಣಿಕೆ_2 = 64;
    bool corrupted = false;
    uint32_t ಮೊದಲ_bad = 0;

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 2000 && !corrupted; i++) {
        uint32_t r = use_hwlp_irq(ಎಣಿಕೆ_2, (ಅಸ್ಥಿರ uint32_t *)SYSTEM_CPU_INTR_FROM_CPU_2_REG, SYSTEM_CPU_INTR_FROM_CPU_2);
        ಆದರೆ (r != ಎಣಿಕೆ_2 * 16) {
            corrupted = true;
            ಮೊದಲ_bad = r;
        }
        /* Give the contender a chance to drain if it is still pending */
        ಆದರೆ ((i & 0x3f) == 0) {
            taskYIELD();
        }
    }

    vTaskDelete(contender);
    esp_intr_free(ih);
    vSemaphoreDelete(s_go);

    ಬರೆ("HWLP deterministic preemption: contender_runs=%u, result=%s (bad=%u)\n", (ಚಿಹ್ನೆರಹಿತ)s_contender_runs, corrupted ? "CORRUPTED" : "ok", (ಚಿಹ್ನೆರಹಿತ)ಮೊದಲ_bad);

    TEST_ASSERT_TRUE_ಸಂದೇಶ(s_contender_runs > 0, "Contender never ran: the mid-loop interrupt was not delivered");
    TEST_ASSERT_FALSE_ಸಂದೇಶ(corrupted, "HWLOOP context corrupted by mid-loop preemption (erratum reproduced)");
}

/*
 * Manual reproduction aid focused on the alignment constraint only:
 * use_hwlp_broken() misaligns both the first and the last hardware-loop
 * instruction (the exact thing the esp-dsp .balign 4 patch fixes) and is
 * preempted at its boundaries. Body size and instruction types are kept valid.
 * Marked [ignore] because it will fault/hang.
 */
uint32_t use_hwlp_broken(uint32_t ಎಣಿಕೆ_2, ಅಸ್ಥಿರ uint32_t *trig_reg, uint32_t trig_val);

TEST_CASE("HWLP: Violate loop alignment/size constraints", "[freertos][ignore]")
{
    s_go = x_Semaphore_Create_ದ್ವಿಮಾನ();
    TEST_ASSERT_NOT_NULL(s_go);
    s_contender_runs = 0;

    intr_handle_t ih;
    TEST_ESP_OK(esp_intr_alloc(SYS_CPU_INTR_FROM_CPU_2_ಮೂಲ, ESP_INTR_FLAG_LEVEL1, from_cpu_isr, ಖಾಲಿ, &ih));

    ಸ್ಥಿರಾಂಕ ತಳ_Type_t core = x_ಪೋರ್ಟ್_Get_Core_ID();
    TaskHandle_t contender = ಖಾಲಿ;
    x_Task_Create_Pinned_ಗಮ್ಯ_Core(hwlp_contender_task, "hwlpC", 2048, ಖಾಲಿ, CONFIG_UNITY_FREERTOS_ಆದ್ಯತೆ + 2, &contender, core);

    bool bad = false;
    uint32_t ಮೊದಲ_bad = 0;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 100000 && !bad; i++) {
        uint32_t r = use_hwlp_broken(64, (ಅಸ್ಥಿರ uint32_t *)SYSTEM_CPU_INTR_FROM_CPU_2_REG, SYSTEM_CPU_INTR_FROM_CPU_2);
        ಆದರೆ (r != 64 * 9) {
            bad = true;
            ಮೊದಲ_bad = r;
        }
        ಆದರೆ ((i & 0x3f) == 0) {
            taskYIELD();
        }
    }

    vTaskDelete(contender);
    esp_intr_free(ih);
    vSemaphoreDelete(s_go);
    ಬರೆ("use_hwlp_broken (misaligned boundaries): contender_runs=%u result=%s (bad=%u)\n", (ಚಿಹ್ನೆರಹಿತ)s_contender_runs, bad ? "CORRUPTED" : "ok", (ಚಿಹ್ನೆರಹಿತ)ಮೊದಲ_bad);
}

#ಆದರೆ_ಅಂತ್ಯ // SOC_CPU_HAS_HWLOOP
