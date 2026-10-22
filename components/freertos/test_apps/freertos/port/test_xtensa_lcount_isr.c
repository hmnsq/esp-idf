/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "sdkconfig.h"

#إذا CONFIG_IDF_هدف_ARCH_XTENSA

#ضمن <stdint.h>
#ضمن "esp_intr_alloc.h"
#ضمن "esp_rom_sys.h"
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "unity.h"
#ضمن "xtensa/config/core-isa.h"
#ضمن "xtensa/xt_specreg.h"
#ضمن "xtensa_api.h"

#إذا XCHAL_HAVE_LOOPS

#وصف_ڪريو SW_ISR_LEVEL_1      7
#وصف_ڪريو SW_ISR_نقاب         (1U << SW_ISR_LEVEL_1)

ساكن متطاير uint32_t s_isr_body_ڪاميابيون;

ساكن مضمن uint32_t read_lbeg(فراغ)
{
    uint32_t val;
    __asm__ متطاير("rsr.lbeg %0" : "=r"(val));
    أرجع val;
}

ساكن مضمن uint32_t read_lend(فراغ)
{
    uint32_t val;
    __asm__ متطاير("rsr.lend %0" : "=r"(val));
    أرجع val;
}

ساكن مضمن uint32_t read_lcount(فراغ)
{
    uint32_t val;
    __asm__ متطاير("rsr.lcount %0" : "=r"(val));
    أرجع val;
}

/*
 * Re-entry reproducer:
 * - Task call: program loop regs via loopnez, trigger SW interrupt from loop body.
 * - ISR call: enter same function but skip loopnez (do not program loop regs).
 *   If stale LCOUNT/LBEG/LEND leaked into ISR, reaching old LEND will loop.
 */
ساكن uint32_t IRAM_ATTR __attribute__((noinline, noclone)) zol_reentry_probe(uint32_t تياري_loop)
{
    uint32_t body_ڪاميابيون = 0;
    uint32_t loop_ڳڻپ = 256;
    uint32_t trigger_once = تياري_loop == 1;

    esp_rom_printf("From %s: setup_loop=%u\n", x_پورٽ_اندريون_Isr_Context() ? "ISR" : "TASK", تياري_loop);
    esp_rom_printf("LBEG=0x%08x LEND=0x%08x LCOUNT=0x%08x\n", read_lbeg(), read_lend(), read_lcount());

    __asm__ متطاير(
        "beqz   %[setup], 1f\n"
        "loopnez %[cnt], .loopEnd\n"
        "addi   %[hits], %[hits], 1\n"
        "beqz   %[trigger], 1f\n"
        "wsr.intset %[int_mask]\n"      // trigger SW interrupt
        "movi   %[trigger], 0\n"
        "1:\n"  // if setup == 0, skip loopnez and LCOUNT should be 0.
        "nop\n"
        ".loopEnd:\n"
        : [ڪاميابيون] "+r"(body_ڪاميابيون),
        [cnt] "+r"(loop_ڳڻپ),
        [trigger] "+r"(trigger_once)
        : [تياري] "r"(تياري_loop),
        [int_نقاب] "r"(SW_ISR_نقاب)
        : "memory"
    );

    أرجع body_ڪاميابيون;
}

ساكن فراغ IRAM_ATTR lcount_probe_isr(فراغ *arg)
{
    TaskHandle_t waiter_task = (TaskHandle_t)arg;
    بنياد_Type_t should_yield = pdFALSE;

    xt_سيٽ_intclear(SW_ISR_نقاب);
    s_isr_body_ڪاميابيون = zol_reentry_probe(0);

    vTaskNotifyGiveFromISR(waiter_task, &should_yield);
    إذا (should_yield == pdTRUE) {
        پورٽ_YIELD_FROM_ISR();
    }
}

TEST_CASE("Xtensa: ISR does not inherit active ZOL state", "[freertos]")
{
    intr_handle_t isr_handle = عدم;
    TaskHandle_t this_task = xTaskGetCurrentTaskHandle();

    xt_سيٽ_intclear(SW_ISR_نقاب);
    TEST_ESP_OK(esp_intr_alloc(ETS_اندروني_SW0_INTR_ذريعو, ESP_INTR_FLAG_LEVEL1, lcount_probe_isr, (فراغ *)this_task, &isr_handle));

    s_isr_body_ڪاميابيون = 0xFFFF;
    uint32_t task_body_ڪاميابيون = zol_reentry_probe(1);

    TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdTRUE, pd_MS_منزل_TICKS(1000)));
    TEST_ESP_OK(esp_intr_free(isr_handle));

    TEST_ASSERT_EQUAL_UINT32(0, s_isr_body_ڪاميابيون);
    TEST_ASSERT_EQUAL_UINT32(256, task_body_ڪاميابيون);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#جيڪڏهن_پورو /* XCHAL_HAVE_LOOPS */
#جيڪڏهن_پورو /* CONFIG_IDF_TARGET_ARCH_XTENSA */
