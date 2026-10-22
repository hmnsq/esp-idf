/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#அந்தர்பாவ "sdkconfig.h"

#எனில் CONFIG_IDF_TARGET_ARCH_XTENSA

#அந்தர்பாவ <stdint.h>
#அந்தர்பாவ "esp_intr_alloc.h"
#அந்தர்பாவ "esp_rom_sys.h"
#அந்தர்பாவ "freertos/FreeRTOS.h"
#அந்தர்பாவ "freertos/task.h"
#அந்தர்பாவ "unity.h"
#அந்தர்பாவ "xtensa/config/core-isa.h"
#அந்தர்பாவ "xtensa/xt_specreg.h"
#அந்தர்பாவ "xtensa_api.h"

#எனில் XCHAL_HAVE_LOOPS

#define SW_ISR_LEVEL_1      7
#define SW_ISR_MASK         (1U << SW_ISR_LEVEL_1)

நிலையான நிலையற்ற uint32_t s_isr_body_hits;

நிலையான உள்வரி uint32_t read_lbeg(வெறுமை)
{
    uint32_t val;
    __asm__ நிலையற்ற("rsr.lbeg %0" : "=r"(val));
    திரும்பு val;
}

நிலையான உள்வரி uint32_t read_lend(வெறுமை)
{
    uint32_t val;
    __asm__ நிலையற்ற("rsr.lend %0" : "=r"(val));
    திரும்பு val;
}

நிலையான உள்வரி uint32_t read_lcount(வெறுமை)
{
    uint32_t val;
    __asm__ நிலையற்ற("rsr.lcount %0" : "=r"(val));
    திரும்பு val;
}

/*
 * Re-entry reproducer:
 * - Task call: program loop regs via loopnez, trigger SW interrupt from loop body.
 * - ISR call: enter same function but skip loopnez (do not program loop regs).
 *   If stale LCOUNT/LBEG/LEND leaked into ISR, reaching old LEND will loop.
 */
நிலையான uint32_t IRAM_ATTR __attribute__((noinline, noclone)) zol_reentry_probe(uint32_t setup_loop)
{
    uint32_t body_hits = 0;
    uint32_t loop_மொத்தம் = 256;
    uint32_t trigger_once = setup_loop == 1;

    esp_rom_printf("From %s: setup_loop=%u\n", xPortInIsrContext() ? "ISR" : "TASK", setup_loop);
    esp_rom_printf("LBEG=0x%08x LEND=0x%08x LCOUNT=0x%08x\n", read_lbeg(), read_lend(), read_lcount());

    __asm__ நிலையற்ற(
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
        [cnt] "+r"(loop_மொத்தம்),
        [trigger] "+r"(trigger_once)
        : [setup] "r"(setup_loop),
        [int_mask] "r"(SW_ISR_MASK)
        : "memory"
    );

    திரும்பு body_hits;
}

நிலையான வெறுமை IRAM_ATTR lcount_probe_isr(வெறுமை *arg)
{
    TaskHandle_t waiter_task = (TaskHandle_t)arg;
    BaseType_t should_yield = pdFALSE;

    xt_set_intclear(SW_ISR_MASK);
    s_isr_body_hits = zol_reentry_probe(0);

    vTaskNotifyGiveFromISR(waiter_task, &should_yield);
    எனில் (should_yield == pdTRUE) {
        portYIELD_FROM_ISR();
    }
}

TEST_CASE("Xtensa: ISR does not inherit active ZOL state", "[freertos]")
{
    intr_handle_t isr_handle = வெற்று;
    TaskHandle_t this_task = xTaskGetCurrentTaskHandle();

    xt_set_intclear(SW_ISR_MASK);
    TEST_ESP_OK(esp_intr_alloc(ETS_INTERNAL_SW0_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1, lcount_probe_isr, (வெறுமை *)this_task, &isr_handle));

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
