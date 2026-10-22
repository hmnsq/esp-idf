/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"

#ಆದರೆ CONFIG_IDF_ಗುರಿ_ARCH_XTENSA

#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ "esp_intr_alloc.h"
#ಅನ್ತರ್ಭಾವ "esp_rom_sys.h"
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "xtensa/config/core-isa.h"
#ಅನ್ತರ್ಭಾವ "xtensa/xt_specreg.h"
#ಅನ್ತರ್ಭಾವ "xtensa_api.h"

#ಆದರೆ XCHAL_HAVE_LOOPS

#ವ್ಯಾಖ್ಯಾನಿಸು SW_ISR_LEVEL_1      7
#ವ್ಯಾಖ್ಯಾನಿಸು SW_ISR_ಮುಖವಾಡ         (1U << SW_ISR_LEVEL_1)

ಸ್ಥಿರ ಅಸ್ಥಿರ uint32_t s_isr_body_ದೊರಕಿದವು;

ಸ್ಥಿರ ಸಾಲೊಳಗೆ uint32_t read_lbeg(ಶೂನ್ಯ)
{
    uint32_t val;
    __asm__ ಅಸ್ಥಿರ("rsr.lbeg %0" : "=r"(val));
    ಹಿಂತಿರುಗಿ val;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ uint32_t read_lend(ಶೂನ್ಯ)
{
    uint32_t val;
    __asm__ ಅಸ್ಥಿರ("rsr.lend %0" : "=r"(val));
    ಹಿಂತಿರುಗಿ val;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ uint32_t read_lcount(ಶೂನ್ಯ)
{
    uint32_t val;
    __asm__ ಅಸ್ಥಿರ("rsr.lcount %0" : "=r"(val));
    ಹಿಂತಿರುಗಿ val;
}

/*
 * Re-entry reproducer:
 * - Task call: program loop regs via loopnez, trigger SW interrupt from loop body.
 * - ISR call: enter same function but skip loopnez (do not program loop regs).
 *   If stale LCOUNT/LBEG/LEND leaked into ISR, reaching old LEND will loop.
 */
ಸ್ಥಿರ uint32_t IRAM_ATTR __attribute__((noinline, noclone)) zol_reentry_probe(uint32_t ಸಿದ್ಧತೆ_loop)
{
    uint32_t body_ದೊರಕಿದವು = 0;
    uint32_t loop_ಎಣಿಕೆ = 256;
    uint32_t trigger_once = ಸಿದ್ಧತೆ_loop == 1;

    esp_rom_printf("From %s: setup_loop=%u\n", x_ಪೋರ್ಟ್_ಒಳ_Isr_Context() ? "ISR" : "TASK", ಸಿದ್ಧತೆ_loop);
    esp_rom_printf("LBEG=0x%08x LEND=0x%08x LCOUNT=0x%08x\n", read_lbeg(), read_lend(), read_lcount());

    __asm__ ಅಸ್ಥಿರ(
        "beqz   %[setup], 1f\n"
        "loopnez %[cnt], .loopEnd\n"
        "addi   %[hits], %[hits], 1\n"
        "beqz   %[trigger], 1f\n"
        "wsr.intset %[int_mask]\n"      // trigger SW interrupt
        "movi   %[trigger], 0\n"
        "1:\n"  // if setup == 0, skip loopnez and LCOUNT should be 0.
        "nop\n"
        ".loopEnd:\n"
        : [ದೊರಕಿದವು] "+r"(body_ದೊರಕಿದವು),
        [cnt] "+r"(loop_ಎಣಿಕೆ),
        [trigger] "+r"(trigger_once)
        : [ಸಿದ್ಧತೆ] "r"(ಸಿದ್ಧತೆ_loop),
        [int_ಮುಖವಾಡ] "r"(SW_ISR_ಮುಖವಾಡ)
        : "memory"
    );

    ಹಿಂತಿರುಗಿ body_ದೊರಕಿದವು;
}

ಸ್ಥಿರ ಶೂನ್ಯ IRAM_ATTR lcount_probe_isr(ಶೂನ್ಯ *arg)
{
    TaskHandle_t waiter_task = (TaskHandle_t)arg;
    ತಳ_Type_t should_yield = pdFALSE;

    xt_ಗಣ_intclear(SW_ISR_ಮುಖವಾಡ);
    s_isr_body_ದೊರಕಿದವು = zol_reentry_probe(0);

    vTaskNotifyGiveFromISR(waiter_task, &should_yield);
    ಆದರೆ (should_yield == pdTRUE) {
        ಪೋರ್ಟ್_YIELD_FROM_ISR();
    }
}

TEST_CASE("Xtensa: ISR does not inherit active ZOL state", "[freertos]")
{
    intr_handle_t isr_handle = ಖಾಲಿ;
    TaskHandle_t this_task = xTaskGetCurrentTaskHandle();

    xt_ಗಣ_intclear(SW_ISR_ಮುಖವಾಡ);
    TEST_ESP_OK(esp_intr_alloc(ETS_ಆಂತರಿಕ_SW0_INTR_ಮೂಲ, ESP_INTR_FLAG_LEVEL1, lcount_probe_isr, (ಶೂನ್ಯ *)this_task, &isr_handle));

    s_isr_body_ದೊರಕಿದವು = 0xFFFF;
    uint32_t task_body_ದೊರಕಿದವು = zol_reentry_probe(1);

    TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdTRUE, pd_MS_ಗಮ್ಯ_TICKS(1000)));
    TEST_ESP_OK(esp_intr_free(isr_handle));

    TEST_ASSERT_EQUAL_UINT32(0, s_isr_body_ದೊರಕಿದವು);
    TEST_ASSERT_EQUAL_UINT32(256, task_body_ದೊರಕಿದವು);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#ಆದರೆ_ಅಂತ್ಯ /* XCHAL_HAVE_LOOPS */
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_IDF_TARGET_ARCH_XTENSA */
