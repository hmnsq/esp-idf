/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#অন্তর্ভাৱ "sdkconfig.h"

#যদি CONFIG_IDF_TARGET_ARCH_XTENSA

#অন্তর্ভাৱ <stdint.h>
#অন্তর্ভাৱ "esp_intr_alloc.h"
#অন্তর্ভাৱ "esp_rom_sys.h"
#অন্তর্ভাৱ "freertos/FreeRTOS.h"
#অন্তর্ভাৱ "freertos/task.h"
#অন্তর্ভাৱ "unity.h"
#অন্তর্ভাৱ "xtensa/config/core-isa.h"
#অন্তর্ভাৱ "xtensa/xt_specreg.h"
#অন্তর্ভাৱ "xtensa_api.h"

#যদি XCHAL_HAVE_LOOPS

#define SW_ISR_LEVEL_1      7
#define SW_ISR_MASK         (1U << SW_ISR_LEVEL_1)

স্থির চঞ্চল uint32_t s_isr_body_hits;

স্থির অন্তঃস্থ uint32_t read_lbeg(শূন্য)
{
    uint32_t val;
    __asm__ চঞ্চল("rsr.lbeg %0" : "=r"(val));
    ঘূৰাই val;
}

স্থির অন্তঃস্থ uint32_t read_lend(শূন্য)
{
    uint32_t val;
    __asm__ চঞ্চল("rsr.lend %0" : "=r"(val));
    ঘূৰাই val;
}

স্থির অন্তঃস্থ uint32_t read_lcount(শূন্য)
{
    uint32_t val;
    __asm__ চঞ্চল("rsr.lcount %0" : "=r"(val));
    ঘূৰাই val;
}

/*
 * Re-entry reproducer:
 * - Task call: program loop regs via loopnez, trigger SW interrupt from loop body.
 * - ISR call: enter same function but skip loopnez (do not program loop regs).
 *   If stale LCOUNT/LBEG/LEND leaked into ISR, reaching old LEND will loop.
 */
স্থির uint32_t IRAM_ATTR __attribute__((noinline, noclone)) zol_reentry_probe(uint32_t setup_loop)
{
    uint32_t body_hits = 0;
    uint32_t loop_গুনতি = 256;
    uint32_t trigger_once = setup_loop == 1;

    esp_rom_printf("From %s: setup_loop=%u\n", xPortInIsrContext() ? "ISR" : "TASK", setup_loop);
    esp_rom_printf("LBEG=0x%08x LEND=0x%08x LCOUNT=0x%08x\n", read_lbeg(), read_lend(), read_lcount());

    __asm__ চঞ্চল(
        "beqz   %[setup], 1f\n"
        "loopnez %[cnt], .loopEnd\n"
        "addi   %[hits], %[hits], 1\n"
        "beqz   %[trigger], 1f\n"
        "wsr.intset %[int_mask]\n"      // trigger SW interrupt
        "movi   %[trigger], 0\n"
        "1:\n"  // if setup == 0, skip loopnez and LCOUNT should be 0.
        "nop\n"
        ".loopEnd:\n"
        : [hits] "+r"(body_hits),
        [cnt] "+r"(loop_গুনতি),
        [trigger] "+r"(trigger_once)
        : [setup] "r"(setup_loop),
        [int_mask] "r"(SW_ISR_MASK)
        : "memory"
    );

    ঘূৰাই body_hits;
}

স্থির শূন্য IRAM_ATTR lcount_probe_isr(শূন্য *arg)
{
    TaskHandle_t waiter_task = (TaskHandle_t)arg;
    BaseType_t should_yield = pdFALSE;

    xt_set_intclear(SW_ISR_MASK);
    s_isr_body_hits = zol_reentry_probe(0);

    vTaskNotifyGiveFromISR(waiter_task, &should_yield);
    যদি (should_yield == pdTRUE) {
        portYIELD_FROM_ISR();
    }
}

TEST_CASE("Xtensa: ISR does not inherit active ZOL state", "[freertos]")
{
    intr_handle_t isr_handle = রিক্ত;
    TaskHandle_t this_task = xTaskGetCurrentTaskHandle();

    xt_set_intclear(SW_ISR_MASK);
    TEST_ESP_OK(esp_intr_alloc(ETS_INTERNAL_SW0_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1, lcount_probe_isr, (শূন্য *)this_task, &isr_handle));

    s_isr_body_hits = 0xFFFF;
    uint32_t task_body_hits = zol_reentry_probe(1);

    TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(1000)));
    TEST_ESP_OK(esp_intr_free(isr_handle));

    TEST_ASSERT_EQUAL_UINT32(0, s_isr_body_hits);
    TEST_ASSERT_EQUAL_UINT32(256, task_body_hits);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#endif /* XCHAL_HAVE_LOOPS */
#endif /* CONFIG_IDF_TARGET_ARCH_XTENSA */
