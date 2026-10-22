/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 See if xPortInIsrContext works
*/

#شامل <esp_types.h>
#شامل <stdio.h>
#شامل "sdkconfig.h"
#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل "freertos/semphr.h"
#شامل "freertos/queue.h"
#شامل "unity.h"
#شامل "esp_intr_alloc.h"
#شامل "esp_rom_sys.h"
#شامل "esp_freertos_hooks.h"

#اگر CONFIG_FREERTOS_CORETIMER_0

ساکن متلون صحیح اندر_int_context, int_handled;

ساکن خالی testint(خالی)
{
    esp_rom_printf("INT!\n");
    اگر (x_پورٹ_اندر_Isr_Context()) {
        اندر_int_context++;
    }
    int_handled++;
}

ساکن خالی testthread(خالی *arg)
{
    اندر_int_context = 0;
    int_handled = 0;
    TEST_ASSERT(!x_پورٹ_اندر_Isr_Context());
    esp_err_t err = esp_رجسٹر_freertos_tick_hook_for_cpu(testint, x_پورٹ_Get_Core_ID());
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);
    vTaskDelay(100 / پورٹ_TICK_PERIOD_MS);
    TEST_ASSERT(int_handled);
    TEST_ASSERT(اندر_int_context);
    esp_deregister_freertos_tick_hook_for_cpu(testint, x_پورٹ_Get_Core_ID());
    vTaskDelete(تہی);
}

TEST_CASE("xPortInIsrContext test", "[freertos]")
{
    x_Task_Create_Pinned_مقصد_Core(testthread, "tst", 4096, تہی, 3, تہی, 0);
    vTaskDelay(150 / پورٹ_TICK_PERIOD_MS);
#اگر CONFIG_FREERTOS_نمبر_کا_CORES == 2
    x_Task_Create_Pinned_مقصد_Core(testthread, "tst", 4096, تہی, 3, تہی, 1);
    vTaskDelay(150 / پورٹ_TICK_PERIOD_MS);
#اگر_ختم
}

#اگر_ختم

#اگر !CONFIG_FREERTOS_SMP // TODO: Enable when IDF-10540 is fixed

ساکن خالی testint_assert(خالی)
{
    esp_rom_printf("INT!\n");
    پورٹ_ASSERT_IF_اندر_ISR();
}

TEST_CASE("port must assert if in ISR context", "[ignore]")
{
    esp_err_t err = esp_رجسٹر_freertos_tick_hook_for_cpu(testint_assert, x_پورٹ_Get_Core_ID());
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);
    vTaskDelay(100 / پورٹ_TICK_PERIOD_MS);
    esp_deregister_freertos_tick_hook_for_cpu(testint_assert, x_پورٹ_Get_Core_ID());
}
#اگر_ختم // !CONFIG_FREERTOS_SMP
