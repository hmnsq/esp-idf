/*
 * SPDX-FileCopyrightText: 2024-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "sdkconfig.h"
#ضمن <string.h>
#ضمن "soc/soc_caps.h"
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "freertos/semphr.h"
#ضمن "unity.h"
#إذا SOC_CPU_HAS_HWLOOP
#ضمن "esp_intr_alloc.h"
#ضمن "esp_cpu.h"
#ضمن "soc/system_intr.h"
#ضمن "soc/system_reg.h"
#جيڪڏهن_پورو

/**
 * On RISC-V targets that have coprocessors, the contexts are saved at the lowest address of the stack,
 * which can lead to wrong stack watermark calculation in FreeRTOS in theory.
 * As such, the port layer of FreeRTOS will adjust the lowest address of the stack when a coprocessor
 * context is saved.
 */
#إذا SOC_CPU_HAS_HWLOOP

uint32_t use_hwlp(uint32_t ڳڻپ);

TEST_CASE("HWLP: Context save does not affect stack watermark", "[freertos]")
{
    /* Force the FreeRTOS port layer to store a HWLP context in the current task.
     * So let's use the it and make sure another task, on the SAME CORE, also uses it */
    ثابت TaskHandle_t current_handle = xTaskGetCurrentTaskHandle();

    /* Get the current stack watermark */
    ثابت UBaseType_t before_watermark = ux_Task_Get_ڍير_مٿاهون_Water_Mark(current_handle);

    /* Use the HWLP unit, the context will NOT be flushed until a context switch is done */
    use_hwlp(20);

    /* Make sure FreeRTOS switches to another task, even Idle task, so that the current Task saves
     * the HWLP current context */
    vTaskDelay(10);

    ثابت UBaseType_t after_watermark = ux_Task_Get_ڍير_مٿاهون_Water_Mark(current_handle);

    TEST_ASSERT_TRUE(after_watermark > before_watermark / 2);
}

#إذا CONFIG_FREERTOS_عدد_جو_CORES > 1

تعريفنوع بنية {
    uint32_t ڳڻپ;
    TaskHandle_t مکيه;
} hwlp_params_t;

ساكن فراغ calculation(فراغ* arg)
{
    hwlp_params_t* p = (hwlp_params_t*) arg;
    ثابت uint32_t ڳڻپ = p->ڳڻپ;
    uint32_t نتيجو = 0;
    صحيح i = 0;

    لكل (i = 0; i < 50000; i++) {
        uint32_t current = use_hwlp(ڳڻپ);
        نتيجو += current;

        /* Using TEST_ASSERT_TRUE triggers a stack overflow, make sure the count is still correct.
         * The function `use_hwlp` should return (count * 16)  */
        أكد(ڳڻپ * 16 == current);
    }

    /* Make sure the result is correct */
    أكد(ڳڻپ * 16 * i == نتيجو);

    xTaskNotifyGive(p->مکيه);
    vTaskDelete(عدم);
}

TEST_CASE("HWLP: Unsolicited context switch between tasks using HWLP", "[freertos]")
{
    /* Create two tasks that are on the same core and use the same FPU */
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    TaskHandle_t tasks[2];
    hwlp_params_t params[2] = {
        { .ڳڻپ = 1024, .مکيه = unity_task_handle },
        { .ڳڻپ = 2048, .مکيه = unity_task_handle },
    };

    x_Task_Create_Pinned_منزل_Core(calculation, "Task1", 2048, params + 0, CONFIG_UNITY_FREERTOS_ترجيح + 1, &tasks[0], 1);
    x_Task_Create_Pinned_منزل_Core(calculation, "Task2", 2048, params + 1, CONFIG_UNITY_FREERTOS_ترجيح + 1, &tasks[1], 1);

    ulTaskNotifyTake(pdTRUE, پورٽ_MAX_DELAY);
    ulTaskNotifyTake(pdTRUE, پورٽ_MAX_DELAY);
}

#جيڪڏهن_پورو /* CONFIG_FREERTOS_NUMBER_OF_CORES > 1 */

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
uint32_t use_hwlp_irq(uint32_t ڳڻپ, متطاير uint32_t *trig_reg, uint32_t trig_val);

ساكن SemaphoreHandle_t s_go;
ساكن متطاير uint32_t s_contender_runs;

ساكن فراغ from_cpu_isr(فراغ *arg)
{
    (فراغ) arg;
    /* Clear the from-CPU interrupt pending bit */
    REG_WRITE(SYSTEM_CPU_INTR_FROM_CPU_2_REG, 0);
    بنياد_Type_t yield = pdFALSE;
    xSemaphoreGiveFromISR(s_go, &yield);
    إذا (yield) {
        پورٽ_YIELD_FROM_ISR();
    }
}

ساكن فراغ hwlp_contender_task(فراغ *arg)
{
    (فراغ) arg;
    لكل (;;) {
        إذا (xSemaphoreTake(s_go, پورٽ_MAX_DELAY) == pdTRUE) {
            /* Use a different loop context to clobber the victim's live registers */
            (فراغ) use_hwlp(333);
            s_contender_runs++;
        }
    }
}

TEST_CASE("HWLP: Deterministic preemption inside a live hardware loop", "[freertos]")
{
    s_go = x_Semaphore_Create_ٻٽو();
    TEST_ASSERT_NOT_NULL(s_go);
    s_contender_runs = 0;

    intr_handle_t ih;
    TEST_ESP_OK(esp_intr_alloc(SYS_CPU_INTR_FROM_CPU_2_ذريعو, ESP_INTR_FLAG_LEVEL1, from_cpu_isr, عدم, &ih));

    ثابت بنياد_Type_t core = x_پورٽ_Get_Core_ID();
    TaskHandle_t contender = عدم;
    x_Task_Create_Pinned_منزل_Core(hwlp_contender_task, "hwlpC", 2048, عدم, CONFIG_UNITY_FREERTOS_ترجيح + 2, &contender, core);

    ثابت uint32_t ڳڻپ = 64;
    bool corrupted = false;
    uint32_t پهريون_bad = 0;

    لكل (صحيح i = 0; i < 2000 && !corrupted; i++) {
        uint32_t r = use_hwlp_irq(ڳڻپ, (متطاير uint32_t *)SYSTEM_CPU_INTR_FROM_CPU_2_REG, SYSTEM_CPU_INTR_FROM_CPU_2);
        إذا (r != ڳڻپ * 16) {
            corrupted = true;
            پهريون_bad = r;
        }
        /* Give the contender a chance to drain if it is still pending */
        إذا ((i & 0x3f) == 0) {
            taskYIELD();
        }
    }

    vTaskDelete(contender);
    esp_intr_free(ih);
    vSemaphoreDelete(s_go);

    لکو("HWLP deterministic preemption: contender_runs=%u, result=%s (bad=%u)\n", (بنانشان)s_contender_runs, corrupted ? "CORRUPTED" : "ok", (بنانشان)پهريون_bad);

    TEST_ASSERT_TRUE_پيغام(s_contender_runs > 0, "Contender never ran: the mid-loop interrupt was not delivered");
    TEST_ASSERT_FALSE_پيغام(corrupted, "HWLOOP context corrupted by mid-loop preemption (erratum reproduced)");
}

/*
 * Manual reproduction aid focused on the alignment constraint only:
 * use_hwlp_broken() misaligns both the first and the last hardware-loop
 * instruction (the exact thing the esp-dsp .balign 4 patch fixes) and is
 * preempted at its boundaries. Body size and instruction types are kept valid.
 * Marked [ignore] because it will fault/hang.
 */
uint32_t use_hwlp_broken(uint32_t ڳڻپ, متطاير uint32_t *trig_reg, uint32_t trig_val);

TEST_CASE("HWLP: Violate loop alignment/size constraints", "[freertos][ignore]")
{
    s_go = x_Semaphore_Create_ٻٽو();
    TEST_ASSERT_NOT_NULL(s_go);
    s_contender_runs = 0;

    intr_handle_t ih;
    TEST_ESP_OK(esp_intr_alloc(SYS_CPU_INTR_FROM_CPU_2_ذريعو, ESP_INTR_FLAG_LEVEL1, from_cpu_isr, عدم, &ih));

    ثابت بنياد_Type_t core = x_پورٽ_Get_Core_ID();
    TaskHandle_t contender = عدم;
    x_Task_Create_Pinned_منزل_Core(hwlp_contender_task, "hwlpC", 2048, عدم, CONFIG_UNITY_FREERTOS_ترجيح + 2, &contender, core);

    bool bad = false;
    uint32_t پهريون_bad = 0;
    لكل (صحيح i = 0; i < 100000 && !bad; i++) {
        uint32_t r = use_hwlp_broken(64, (متطاير uint32_t *)SYSTEM_CPU_INTR_FROM_CPU_2_REG, SYSTEM_CPU_INTR_FROM_CPU_2);
        إذا (r != 64 * 9) {
            bad = true;
            پهريون_bad = r;
        }
        إذا ((i & 0x3f) == 0) {
            taskYIELD();
        }
    }

    vTaskDelete(contender);
    esp_intr_free(ih);
    vSemaphoreDelete(s_go);
    لکو("use_hwlp_broken (misaligned boundaries): contender_runs=%u result=%s (bad=%u)\n", (بنانشان)s_contender_runs, bad ? "CORRUPTED" : "ok", (بنانشان)پهريون_bad);
}

#جيڪڏهن_پورو // SOC_CPU_HAS_HWLOOP
