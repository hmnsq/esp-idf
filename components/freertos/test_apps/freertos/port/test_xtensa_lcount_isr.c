/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"

#ਜੇ CONFIG_IDF_ਨਿਸ਼ਾਨਾ_ARCH_XTENSA

#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_intr_alloc.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_rom_sys.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xtensa/config/core-isa.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xtensa/xt_specreg.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xtensa_api.h"

#ਜੇ XCHAL_HAVE_LOOPS

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SW_ISR_LEVEL_1      7
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SW_ISR_ਨਕਾਬ         (1U << SW_ISR_LEVEL_1)

ਸਥਿਰ ਅਸਥਿਰ uint32_t s_isr_body_ਪ੍ਰਾਪਤੀਆਂ;

ਸਥਿਰ ਸਤਰਅੰਦਰ uint32_t read_lbeg(ਖਾਲੀ)
{
    uint32_t val;
    __asm__ ਅਸਥਿਰ("rsr.lbeg %0" : "=r"(val));
    ਵਾਪਸ val;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ uint32_t read_lend(ਖਾਲੀ)
{
    uint32_t val;
    __asm__ ਅਸਥਿਰ("rsr.lend %0" : "=r"(val));
    ਵਾਪਸ val;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ uint32_t read_lcount(ਖਾਲੀ)
{
    uint32_t val;
    __asm__ ਅਸਥਿਰ("rsr.lcount %0" : "=r"(val));
    ਵਾਪਸ val;
}

/*
 * Re-entry reproducer:
 * - Task call: program loop regs via loopnez, trigger SW interrupt from loop body.
 * - ISR call: enter same function but skip loopnez (do not program loop regs).
 *   If stale LCOUNT/LBEG/LEND leaked into ISR, reaching old LEND will loop.
 */
ਸਥਿਰ uint32_t IRAM_ATTR __attribute__((noinline, noclone)) zol_reentry_probe(uint32_t ਤਿਆਰੀ_loop)
{
    uint32_t body_ਪ੍ਰਾਪਤੀਆਂ = 0;
    uint32_t loop_ਗਿਣਤੀ = 256;
    uint32_t trigger_once = ਤਿਆਰੀ_loop == 1;

    esp_rom_printf("From %s: setup_loop=%u\n", x_ਪੋਰਟ_ਵਿੱਚ_Isr_Context() ? "ISR" : "TASK", ਤਿਆਰੀ_loop);
    esp_rom_printf("LBEG=0x%08x LEND=0x%08x LCOUNT=0x%08x\n", read_lbeg(), read_lend(), read_lcount());

    __asm__ ਅਸਥਿਰ(
        "beqz   %[setup], 1f\n"
        "loopnez %[cnt], .loopEnd\n"
        "addi   %[hits], %[hits], 1\n"
        "beqz   %[trigger], 1f\n"
        "wsr.intset %[int_mask]\n"      // trigger SW interrupt
        "movi   %[trigger], 0\n"
        "1:\n"  // if setup == 0, skip loopnez and LCOUNT should be 0.
        "nop\n"
        ".loopEnd:\n"
        : [ਪ੍ਰਾਪਤੀਆਂ] "+r"(body_ਪ੍ਰਾਪਤੀਆਂ),
        [cnt] "+r"(loop_ਗਿਣਤੀ),
        [trigger] "+r"(trigger_once)
        : [ਤਿਆਰੀ] "r"(ਤਿਆਰੀ_loop),
        [int_ਨਕਾਬ] "r"(SW_ISR_ਨਕਾਬ)
        : "memory"
    );

    ਵਾਪਸ body_ਪ੍ਰਾਪਤੀਆਂ;
}

ਸਥਿਰ ਖਾਲੀ IRAM_ATTR lcount_probe_isr(ਖਾਲੀ *arg)
{
    TaskHandle_t waiter_task = (TaskHandle_t)arg;
    ਅਧਾਰ_Type_t should_yield = pdFALSE;

    xt_ਸੈੱਟ_intclear(SW_ISR_ਨਕਾਬ);
    s_isr_body_ਪ੍ਰਾਪਤੀਆਂ = zol_reentry_probe(0);

    vTaskNotifyGiveFromISR(waiter_task, &should_yield);
    ਜੇ (should_yield == pdTRUE) {
        ਪੋਰਟ_YIELD_FROM_ISR();
    }
}

TEST_CASE("Xtensa: ISR does not inherit active ZOL state", "[freertos]")
{
    intr_handle_t isr_handle = ਨਿਰਾ;
    TaskHandle_t this_task = xTaskGetCurrentTaskHandle();

    xt_ਸੈੱਟ_intclear(SW_ISR_ਨਕਾਬ);
    TEST_ESP_OK(esp_intr_alloc(ETS_ਅੰਦਰੂਨੀ_SW0_INTR_ਸਰੋਤ, ESP_INTR_FLAG_LEVEL1, lcount_probe_isr, (ਖਾਲੀ *)this_task, &isr_handle));

    s_isr_body_ਪ੍ਰਾਪਤੀਆਂ = 0xFFFF;
    uint32_t task_body_ਪ੍ਰਾਪਤੀਆਂ = zol_reentry_probe(1);

    TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdTRUE, pd_MS_ਮੰਜ਼ਿਲ_TICKS(1000)));
    TEST_ESP_OK(esp_intr_free(isr_handle));

    TEST_ASSERT_EQUAL_UINT32(0, s_isr_body_ਪ੍ਰਾਪਤੀਆਂ);
    TEST_ASSERT_EQUAL_UINT32(256, task_body_ਪ੍ਰਾਪਤੀਆਂ);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#ਜੇ_ਅੰਤ /* XCHAL_HAVE_LOOPS */
#ਜੇ_ਅੰਤ /* CONFIG_IDF_TARGET_ARCH_XTENSA */
