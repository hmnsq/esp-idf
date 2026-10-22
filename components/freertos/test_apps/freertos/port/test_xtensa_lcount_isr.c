/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ "sdkconfig.h"

#ఒకవేళ CONFIG_IDF_లక్ష్యం_ARCH_XTENSA

#అన్తర్భావ <stdint.h>
#అన్తర్భావ "esp_intr_alloc.h"
#అన్తర్భావ "esp_rom_sys.h"
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "xtensa/config/core-isa.h"
#అన్తర్భావ "xtensa/xt_specreg.h"
#అన్తర్భావ "xtensa_api.h"

#ఒకవేళ XCHAL_HAVE_LOOPS

#నిర్వచించు SW_ISR_LEVEL_1      7
#నిర్వచించు SW_ISR_ముసుగు         (1U << SW_ISR_LEVEL_1)

స్థిర అస్థిర uint32_t s_isr_body_తగిలినవి;

స్థిర అంతఃవరుస uint32_t read_lbeg(శూన్య)
{
    uint32_t val;
    __asm__ అస్థిర("rsr.lbeg %0" : "=r"(val));
    తిరిగి val;
}

స్థిర అంతఃవరుస uint32_t read_lend(శూన్య)
{
    uint32_t val;
    __asm__ అస్థిర("rsr.lend %0" : "=r"(val));
    తిరిగి val;
}

స్థిర అంతఃవరుస uint32_t read_lcount(శూన్య)
{
    uint32_t val;
    __asm__ అస్థిర("rsr.lcount %0" : "=r"(val));
    తిరిగి val;
}

/*
 * Re-entry reproducer:
 * - Task call: program loop regs via loopnez, trigger SW interrupt from loop body.
 * - ISR call: enter same function but skip loopnez (do not program loop regs).
 *   If stale LCOUNT/LBEG/LEND leaked into ISR, reaching old LEND will loop.
 */
స్థిర uint32_t IRAM_ATTR __attribute__((noinline, noclone)) zol_reentry_probe(uint32_t అమరిక_loop)
{
    uint32_t body_తగిలినవి = 0;
    uint32_t loop_లెక్క = 256;
    uint32_t trigger_once = అమరిక_loop == 1;

    esp_rom_printf("From %s: setup_loop=%u\n", x_పోర్టు_లోపల_Isr_Context() ? "ISR" : "TASK", అమరిక_loop);
    esp_rom_printf("LBEG=0x%08x LEND=0x%08x LCOUNT=0x%08x\n", read_lbeg(), read_lend(), read_lcount());

    __asm__ అస్థిర(
        "beqz   %[setup], 1f\n"
        "loopnez %[cnt], .loopEnd\n"
        "addi   %[hits], %[hits], 1\n"
        "beqz   %[trigger], 1f\n"
        "wsr.intset %[int_mask]\n"      // trigger SW interrupt
        "movi   %[trigger], 0\n"
        "1:\n"  // if setup == 0, skip loopnez and LCOUNT should be 0.
        "nop\n"
        ".loopEnd:\n"
        : [తగిలినవి] "+r"(body_తగిలినవి),
        [cnt] "+r"(loop_లెక్క),
        [trigger] "+r"(trigger_once)
        : [అమరిక] "r"(అమరిక_loop),
        [int_ముసుగు] "r"(SW_ISR_ముసుగు)
        : "memory"
    );

    తిరిగి body_తగిలినవి;
}

స్థిర శూన్య IRAM_ATTR lcount_probe_isr(శూన్య *arg)
{
    TaskHandle_t waiter_task = (TaskHandle_t)arg;
    పునాది_Type_t should_yield = pdFALSE;

    xt_సమితి_intclear(SW_ISR_ముసుగు);
    s_isr_body_తగిలినవి = zol_reentry_probe(0);

    vTaskNotifyGiveFromISR(waiter_task, &should_yield);
    ఒకవేళ (should_yield == pdTRUE) {
        పోర్టు_YIELD_FROM_ISR();
    }
}

TEST_CASE("Xtensa: ISR does not inherit active ZOL state", "[freertos]")
{
    intr_handle_t isr_handle = ఖాళీ;
    TaskHandle_t this_task = xTaskGetCurrentTaskHandle();

    xt_సమితి_intclear(SW_ISR_ముసుగు);
    TEST_ESP_OK(esp_intr_alloc(ETS_అంతర్గత_SW0_INTR_ఆధారం, ESP_INTR_FLAG_LEVEL1, lcount_probe_isr, (శూన్య *)this_task, &isr_handle));

    s_isr_body_తగిలినవి = 0xFFFF;
    uint32_t task_body_తగిలినవి = zol_reentry_probe(1);

    TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdTRUE, pd_MS_గమ్యం_TICKS(1000)));
    TEST_ESP_OK(esp_intr_free(isr_handle));

    TEST_ASSERT_EQUAL_UINT32(0, s_isr_body_తగిలినవి);
    TEST_ASSERT_EQUAL_UINT32(256, task_body_తగిలినవి);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#అయితే_ముగింపు /* XCHAL_HAVE_LOOPS */
#అయితే_ముగింపు /* CONFIG_IDF_TARGET_ARCH_XTENSA */
