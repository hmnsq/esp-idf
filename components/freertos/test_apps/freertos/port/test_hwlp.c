/*
 * SPDX-FileCopyrightText: 2024-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#অন্তর্ভাব "sdkconfig.h"
#অন্তর্ভাব <string.h>
#অন্তর্ভাব "soc/soc_caps.h"
#অন্তর্ভাব "freertos/FreeRTOS.h"
#অন্তর্ভাব "freertos/task.h"
#অন্তর্ভাব "freertos/semphr.h"
#অন্তর্ভাব "unity.h"
#যদি SOC_CPU_HAS_HWLOOP
#অন্তর্ভাব "esp_intr_alloc.h"
#অন্তর্ভাব "esp_cpu.h"
#অন্তর্ভাব "soc/system_intr.h"
#অন্তর্ভাব "soc/system_reg.h"
#endif

/**
 * On RISC-V targets that have coprocessors, the contexts are saved at the lowest address of the stack,
 * which can lead to wrong stack watermark calculation in FreeRTOS in theory.
 * As such, the port layer of FreeRTOS will adjust the lowest address of the stack when a coprocessor
 * context is saved.
 */
#যদি SOC_CPU_HAS_HWLOOP

uint32_t use_hwlp(uint32_t গুনতি);

TEST_CASE("HWLP: Context save does not affect stack watermark", "[freertos]")
{
    /* Force the FreeRTOS port layer to store a HWLP context in the current task.
     * So let's use the it and make sure another task, on the SAME CORE, also uses it */
    ধ্রুবক TaskHandle_t current_handle = xTaskGetCurrentTaskHandle();

    /* Get the current stack watermark */
    ধ্রুবক UBaseType_t before_watermark = ux_Task_Get_স্তূপ_উপরের_Water_Mark(current_handle);

    /* Use the HWLP unit, the context will NOT be flushed until a context switch is done */
    use_hwlp(20);

    /* Make sure FreeRTOS switches to another task, even Idle task, so that the current Task saves
     * the HWLP current context */
    vTaskDelay(10);

    ধ্রুবক UBaseType_t after_watermark = ux_Task_Get_স্তূপ_উপরের_Water_Mark(current_handle);

    TEST_ASSERT_TRUE(after_watermark > before_watermark / 2);
}

#যদি CONFIG_FREERTOS_সংখ্যা_OF_CORES > 1

প্রকারলেখ কাঠামো {
    uint32_t গুনতি;
    TaskHandle_t প্রধান;
} hwlp_params_t;

স্থির শূন্য calculation(শূন্য* arg)
{
    hwlp_params_t* p = (hwlp_params_t*) arg;
    ধ্রুবক uint32_t গুনতি = p->গুনতি;
    uint32_t ফলাফল = 0;
    পূর্ণসংখ্যা i = 0;

    জন্য (i = 0; i < 50000; i++) {
        uint32_t current = use_hwlp(গুনতি);
        ফলাফল += current;

        /* Using TEST_ASSERT_TRUE triggers a stack overflow, make sure the count is still correct.
         * The function `use_hwlp` should return (count * 16)  */
        প্রতিজ্ঞা(গুনতি * 16 == current);
    }

    /* Make sure the result is correct */
    প্রতিজ্ঞা(গুনতি * 16 * i == ফলাফল);

    xTaskNotifyGive(p->প্রধান);
    vTaskDelete(ফাঁকা);
}

TEST_CASE("HWLP: Unsolicited context switch between tasks using HWLP", "[freertos]")
{
    /* Create two tasks that are on the same core and use the same FPU */
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    TaskHandle_t tasks[2];
    hwlp_params_t params[2] = {
        { .গুনতি = 1024, .প্রধান = unity_task_handle },
        { .গুনতি = 2048, .প্রধান = unity_task_handle },
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
uint32_t use_hwlp_irq(uint32_t গুনতি, অস্থির uint32_t *trig_reg, uint32_t trig_val);

স্থির SemaphoreHandle_t s_go;
স্থির অস্থির uint32_t s_contender_runs;

স্থির শূন্য from_cpu_isr(শূন্য *arg)
{
    (শূন্য) arg;
    /* Clear the from-CPU interrupt pending bit */
    REG_WRITE(SYSTEM_CPU_INTR_FROM_CPU_2_REG, 0);
    BaseType_t yield = pdFALSE;
    xSemaphoreGiveFromISR(s_go, &yield);
    যদি (yield) {
        portYIELD_FROM_ISR();
    }
}

স্থির শূন্য hwlp_contender_task(শূন্য *arg)
{
    (শূন্য) arg;
    জন্য (;;) {
        যদি (xSemaphoreTake(s_go, portMAX_DELAY) == pdTRUE) {
            /* Use a different loop context to clobber the victim's live registers */
            (শূন্য) use_hwlp(333);
            s_contender_runs++;
        }
    }
}

TEST_CASE("HWLP: Deterministic preemption inside a live hardware loop", "[freertos]")
{
    s_go = x_Semaphore_Create_দ্বিমিক();
    TEST_ASSERT_NOT_NULL(s_go);
    s_contender_runs = 0;

    intr_handle_t ih;
    TEST_ESP_OK(esp_intr_alloc(SYS_CPU_INTR_FROM_CPU_2_SOURCE, ESP_INTR_FLAG_LEVEL1, from_cpu_isr, ফাঁকা, &ih));

    ধ্রুবক BaseType_t core = xPortGetCoreID();
    TaskHandle_t contender = ফাঁকা;
    xTaskCreatePinnedToCore(hwlp_contender_task, "hwlpC", 2048, ফাঁকা, CONFIG_UNITY_FREERTOS_PRIORITY + 2, &contender, core);

    ধ্রুবক uint32_t গুনতি = 64;
    bool corrupted = false;
    uint32_t first_bad = 0;

    জন্য (পূর্ণসংখ্যা i = 0; i < 2000 && !corrupted; i++) {
        uint32_t r = use_hwlp_irq(গুনতি, (অস্থির uint32_t *)SYSTEM_CPU_INTR_FROM_CPU_2_REG, SYSTEM_CPU_INTR_FROM_CPU_2);
        যদি (r != গুনতি * 16) {
            corrupted = true;
            first_bad = r;
        }
        /* Give the contender a chance to drain if it is still pending */
        যদি ((i & 0x3f) == 0) {
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
uint32_t use_hwlp_broken(uint32_t গুনতি, অস্থির uint32_t *trig_reg, uint32_t trig_val);

TEST_CASE("HWLP: Violate loop alignment/size constraints", "[freertos][ignore]")
{
    s_go = x_Semaphore_Create_দ্বিমিক();
    TEST_ASSERT_NOT_NULL(s_go);
    s_contender_runs = 0;

    intr_handle_t ih;
    TEST_ESP_OK(esp_intr_alloc(SYS_CPU_INTR_FROM_CPU_2_SOURCE, ESP_INTR_FLAG_LEVEL1, from_cpu_isr, ফাঁকা, &ih));

    ধ্রুবক BaseType_t core = xPortGetCoreID();
    TaskHandle_t contender = ফাঁকা;
    xTaskCreatePinnedToCore(hwlp_contender_task, "hwlpC", 2048, ফাঁকা, CONFIG_UNITY_FREERTOS_PRIORITY + 2, &contender, core);

    bool bad = false;
    uint32_t first_bad = 0;
    জন্য (পূর্ণসংখ্যা i = 0; i < 100000 && !bad; i++) {
        uint32_t r = use_hwlp_broken(64, (অস্থির uint32_t *)SYSTEM_CPU_INTR_FROM_CPU_2_REG, SYSTEM_CPU_INTR_FROM_CPU_2);
        যদি (r != 64 * 9) {
            bad = true;
            first_bad = r;
        }
        যদি ((i & 0x3f) == 0) {
            taskYIELD();
        }
    }

    vTaskDelete(contender);
    esp_intr_free(ih);
    vSemaphoreDelete(s_go);
    printf("use_hwlp_broken (misaligned boundaries): contender_runs=%u result=%s (bad=%u)\n", (unsigned)s_contender_runs, bad ? "CORRUPTED" : "ok", (unsigned)first_bad);
}

#endif // SOC_CPU_HAS_HWLOOP
