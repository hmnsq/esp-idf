/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 See if xPortInIsrContext works
*/

#ضمن <esp_types.h>
#ضمن <stdio.h>
#ضمن "sdkconfig.h"
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "freertos/semphr.h"
#ضمن "freertos/queue.h"
#ضمن "unity.h"
#ضمن "esp_intr_alloc.h"
#ضمن "esp_rom_sys.h"
#ضمن "esp_freertos_hooks.h"

#إذا CONFIG_FREERTOS_CORETIMER_0

ساكن متطاير صحيح وچ_int_context, int_handled;

ساكن فراغ testint(فراغ)
{
    esp_rom_printf("INT!\n");
    إذا (x_پورٹ_وچ_Isr_Context()) {
        وچ_int_context++;
    }
    int_handled++;
}

ساكن فراغ testthread(فراغ *arg)
{
    وچ_int_context = 0;
    int_handled = 0;
    TEST_ASSERT(!x_پورٹ_وچ_Isr_Context());
    esp_err_t err = esp_رجسٹر_freertos_tick_hook_for_cpu(testint, x_پورٹ_Get_Core_ID());
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);
    vTaskDelay(100 / پورٹ_TICK_PERIOD_MS);
    TEST_ASSERT(int_handled);
    TEST_ASSERT(وچ_int_context);
    esp_deregister_freertos_tick_hook_for_cpu(testint, x_پورٹ_Get_Core_ID());
    vTaskDelete(عدم);
}

TEST_CASE("xPortInIsrContext test", "[freertos]")
{
    x_Task_Create_Pinned_منزل_Core(testthread, "tst", 4096, عدم, 3, عدم, 0);
    vTaskDelay(150 / پورٹ_TICK_PERIOD_MS);
#إذا CONFIG_FREERTOS_نمبر_دا_CORES == 2
    x_Task_Create_Pinned_منزل_Core(testthread, "tst", 4096, عدم, 3, عدم, 1);
    vTaskDelay(150 / پورٹ_TICK_PERIOD_MS);
#جے_انت
}

#جے_انت

#إذا !CONFIG_FREERTOS_SMP // TODO: Enable when IDF-10540 is fixed

ساكن فراغ testint_assert(فراغ)
{
    esp_rom_printf("INT!\n");
    پورٹ_ASSERT_IF_وچ_ISR();
}

TEST_CASE("port must assert if in ISR context", "[ignore]")
{
    esp_err_t err = esp_رجسٹر_freertos_tick_hook_for_cpu(testint_assert, x_پورٹ_Get_Core_ID());
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);
    vTaskDelay(100 / پورٹ_TICK_PERIOD_MS);
    esp_deregister_freertos_tick_hook_for_cpu(testint_assert, x_پورٹ_Get_Core_ID());
}
#جے_انت // !CONFIG_FREERTOS_SMP
