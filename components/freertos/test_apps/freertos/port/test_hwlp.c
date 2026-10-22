/*
 * SPDX-FileCopyrightText: 2024-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#അന്തര്ഭാവ "sdkconfig.h"
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ "soc/soc_caps.h"
#അന്തര്ഭാവ "freertos/FreeRTOS.h"
#അന്തര്ഭാവ "freertos/task.h"
#അന്തര്ഭാവ "freertos/semphr.h"
#അന്തര്ഭാവ "unity.h"
#എങ്കിൽ SOC_CPU_HAS_HWLOOP
#അന്തര്ഭാവ "esp_intr_alloc.h"
#അന്തര്ഭാവ "esp_cpu.h"
#അന്തര്ഭാവ "soc/system_intr.h"
#അന്തര്ഭാവ "soc/system_reg.h"
#endif

/**
 * On RISC-V targets that have coprocessors, the contexts are saved at the lowest address of the stack,
 * which can lead to wrong stack watermark calculation in FreeRTOS in theory.
 * As such, the port layer of FreeRTOS will adjust the lowest address of the stack when a coprocessor
 * context is saved.
 */
#എങ്കിൽ SOC_CPU_HAS_HWLOOP

uint32_t use_hwlp(uint32_t എണ്ണം);

TEST_CASE("HWLP: Context save does not affect stack watermark", "[freertos]")
{
    /* Force the FreeRTOS port layer to store a HWLP context in the current task.
     * So let's use the it and make sure another task, on the SAME CORE, also uses it */
    സ്ഥിരം TaskHandle_t current_handle = xTaskGetCurrentTaskHandle();

    /* Get the current stack watermark */
    സ്ഥിരം UBaseType_t before_watermark = ux_Task_Get_അട്ടി_മുകളിലെ_Water_Mark(current_handle);

    /* Use the HWLP unit, the context will NOT be flushed until a context switch is done */
    use_hwlp(20);

    /* Make sure FreeRTOS switches to another task, even Idle task, so that the current Task saves
     * the HWLP current context */
    vTaskDelay(10);

    സ്ഥിരം UBaseType_t after_watermark = ux_Task_Get_അട്ടി_മുകളിലെ_Water_Mark(current_handle);

    TEST_ASSERT_TRUE(after_watermark > before_watermark / 2);
}

#എങ്കിൽ CONFIG_FREERTOS_സംഖ്യ_OF_CORES > 1

തരനിർവചനം ഘടന {
    uint32_t എണ്ണം;
    TaskHandle_t പ്രധാന;
} hwlp_params_t;

സ്ഥിതം ശൂന്യം calculation(ശൂന്യം* arg)
{
    hwlp_params_t* p = (hwlp_params_t*) arg;
    സ്ഥിരം uint32_t എണ്ണം = p->എണ്ണം;
    uint32_t result = 0;
    പൂർണ്ണസംഖ്യ i = 0;

    വേണ്ടി (i = 0; i < 50000; i++) {
        uint32_t current = use_hwlp(എണ്ണം);
        result += current;

        /* Using TEST_ASSERT_TRUE triggers a stack overflow, make sure the count is still correct.
         * The function `use_hwlp` should return (count * 16)  */
        പ്രതിജ്ഞാ(എണ്ണം * 16 == current);
    }

    /* Make sure the result is correct */
    പ്രതിജ്ഞാ(എണ്ണം * 16 * i == result);

    xTaskNotifyGive(p->പ്രധാന);
    vTaskDelete(ശൂന്യ);
}

TEST_CASE("HWLP: Unsolicited context switch between tasks using HWLP", "[freertos]")
{
    /* Create two tasks that are on the same core and use the same FPU */
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    TaskHandle_t tasks[2];
    hwlp_params_t params[2] = {
        { .എണ്ണം = 1024, .പ്രധാന = unity_task_handle },
        { .എണ്ണം = 2048, .പ്രധാന = unity_task_handle },
    };

    xTaskCreatePinnedToCore(calculation, "Task1", 2048, params + 0, CONFIG_UNITY_FREERTOS_PRIORITY + 1, &tasks[0], 1);
    xTaskCreatePinnedToCore(calculation, "Task2", 2048, params + 1, CONFIG_UNITY_FREERTOS_PRIORITY + 1, &tasks[1], 1);

    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
}

#endif /* CONFIG_FREERTOS_NUMBER_OF_CORES > 1 */

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
uint32_t use_hwlp_irq(uint32_t എണ്ണം, അസ്ഥിര uint32_t *trig_reg, uint32_t trig_val);

സ്ഥിതം SemaphoreHandle_t s_go;
സ്ഥിതം അസ്ഥിര uint32_t s_contender_runs;

സ്ഥിതം ശൂന്യം from_cpu_isr(ശൂന്യം *arg)
{
    (ശൂന്യം) arg;
    /* Clear the from-CPU interrupt pending bit */
    REG_WRITE(SYSTEM_CPU_INTR_FROM_CPU_2_REG, 0);
    BaseType_t yield = pdFALSE;
    xSemaphoreGiveFromISR(s_go, &yield);
    എങ്കിൽ (yield) {
        portYIELD_FROM_ISR();
    }
}

സ്ഥിതം ശൂന്യം hwlp_contender_task(ശൂന്യം *arg)
{
    (ശൂന്യം) arg;
    വേണ്ടി (;;) {
        എങ്കിൽ (xSemaphoreTake(s_go, portMAX_DELAY) == pdTRUE) {
            /* Use a different loop context to clobber the victim's live registers */
            (ശൂന്യം) use_hwlp(333);
            s_contender_runs++;
        }
    }
}

TEST_CASE("HWLP: Deterministic preemption inside a live hardware loop", "[freertos]")
{
    s_go = x_Semaphore_Create_ദ്വിമാന();
    TEST_ASSERT_NOT_NULL(s_go);
    s_contender_runs = 0;

    intr_handle_t ih;
    TEST_ESP_OK(esp_intr_alloc(SYS_CPU_INTR_FROM_CPU_2_SOURCE, ESP_INTR_FLAG_LEVEL1, from_cpu_isr, ശൂന്യ, &ih));

    സ്ഥിരം BaseType_t core = xPortGetCoreID();
    TaskHandle_t contender = ശൂന്യ;
    xTaskCreatePinnedToCore(hwlp_contender_task, "hwlpC", 2048, ശൂന്യ, CONFIG_UNITY_FREERTOS_PRIORITY + 2, &contender, core);

    സ്ഥിരം uint32_t എണ്ണം = 64;
    bool corrupted = false;
    uint32_t first_bad = 0;

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 2000 && !corrupted; i++) {
        uint32_t r = use_hwlp_irq(എണ്ണം, (അസ്ഥിര uint32_t *)SYSTEM_CPU_INTR_FROM_CPU_2_REG, SYSTEM_CPU_INTR_FROM_CPU_2);
        എങ്കിൽ (r != എണ്ണം * 16) {
            corrupted = true;
            first_bad = r;
        }
        /* Give the contender a chance to drain if it is still pending */
        എങ്കിൽ ((i & 0x3f) == 0) {
            taskYIELD();
        }
    }

    vTaskDelete(contender);
    esp_intr_free(ih);
    vSemaphoreDelete(s_go);

    printf("HWLP deterministic preemption: contender_runs=%u, result=%s (bad=%u)\n", (unsigned)s_contender_runs, corrupted ? "CORRUPTED" : "ok", (unsigned)first_bad);

    TEST_ASSERT_TRUE_MESSAGE(s_contender_runs > 0, "Contender never ran: the mid-loop interrupt was not delivered");
    TEST_ASSERT_FALSE_MESSAGE(corrupted, "HWLOOP context corrupted by mid-loop preemption (erratum reproduced)");
}

/*
 * Manual reproduction aid focused on the alignment constraint only:
 * use_hwlp_broken() misaligns both the first and the last hardware-loop
 * instruction (the exact thing the esp-dsp .balign 4 patch fixes) and is
 * preempted at its boundaries. Body size and instruction types are kept valid.
 * Marked [ignore] because it will fault/hang.
 */
uint32_t use_hwlp_broken(uint32_t എണ്ണം, അസ്ഥിര uint32_t *trig_reg, uint32_t trig_val);

TEST_CASE("HWLP: Violate loop alignment/size constraints", "[freertos][ignore]")
{
    s_go = x_Semaphore_Create_ദ്വിമാന();
    TEST_ASSERT_NOT_NULL(s_go);
    s_contender_runs = 0;

    intr_handle_t ih;
    TEST_ESP_OK(esp_intr_alloc(SYS_CPU_INTR_FROM_CPU_2_SOURCE, ESP_INTR_FLAG_LEVEL1, from_cpu_isr, ശൂന്യ, &ih));

    സ്ഥിരം BaseType_t core = xPortGetCoreID();
    TaskHandle_t contender = ശൂന്യ;
    xTaskCreatePinnedToCore(hwlp_contender_task, "hwlpC", 2048, ശൂന്യ, CONFIG_UNITY_FREERTOS_PRIORITY + 2, &contender, core);

    bool bad = false;
    uint32_t first_bad = 0;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 100000 && !bad; i++) {
        uint32_t r = use_hwlp_broken(64, (അസ്ഥിര uint32_t *)SYSTEM_CPU_INTR_FROM_CPU_2_REG, SYSTEM_CPU_INTR_FROM_CPU_2);
        എങ്കിൽ (r != 64 * 9) {
            bad = true;
            first_bad = r;
        }
        എങ്കിൽ ((i & 0x3f) == 0) {
            taskYIELD();
        }
    }

    vTaskDelete(contender);
    esp_intr_free(ih);
    vSemaphoreDelete(s_go);
    printf("use_hwlp_broken (misaligned boundaries): contender_runs=%u result=%s (bad=%u)\n", (unsigned)s_contender_runs, bad ? "CORRUPTED" : "ok", (unsigned)first_bad);
}

#endif // SOC_CPU_HAS_HWLOOP
