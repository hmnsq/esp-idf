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

ساكن متطاير صحيح اندريون_int_context, int_handled;

ساكن فراغ testint(فراغ)
{
    esp_rom_printf("INT!\n");
    إذا (x_پورٽ_اندريون_Isr_Context()) {
        اندريون_int_context++;
    }
    int_handled++;
}

ساكن فراغ testthread(فراغ *arg)
{
    اندريون_int_context = 0;
    int_handled = 0;
    TEST_ASSERT(!x_پورٽ_اندريون_Isr_Context());
    esp_err_t err = esp_رجسٽر_freertos_tick_hook_for_cpu(testint, x_پورٽ_Get_Core_ID());
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);
    vTaskDelay(100 / پورٽ_TICK_PERIOD_MS);
    TEST_ASSERT(int_handled);
    TEST_ASSERT(اندريون_int_context);
    esp_deregister_freertos_tick_hook_for_cpu(testint, x_پورٽ_Get_Core_ID());
    vTaskDelete(عدم);
}

TEST_CASE("xPortInIsrContext test", "[freertos]")
{
    x_Task_Create_Pinned_منزل_Core(testthread, "tst", 4096, عدم, 3, عدم, 0);
    vTaskDelay(150 / پورٽ_TICK_PERIOD_MS);
#إذا CONFIG_FREERTOS_عدد_جو_CORES == 2
    x_Task_Create_Pinned_منزل_Core(testthread, "tst", 4096, عدم, 3, عدم, 1);
    vTaskDelay(150 / پورٽ_TICK_PERIOD_MS);
#جيڪڏهن_پورو
}

#جيڪڏهن_پورو

#إذا !CONFIG_FREERTOS_SMP // TODO: Enable when IDF-10540 is fixed

ساكن فراغ testint_assert(فراغ)
{
    esp_rom_printf("INT!\n");
    پورٽ_ASSERT_IF_اندريون_ISR();
}

TEST_CASE("port must assert if in ISR context", "[ignore]")
{
    esp_err_t err = esp_رجسٽر_freertos_tick_hook_for_cpu(testint_assert, x_پورٽ_Get_Core_ID());
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);
    vTaskDelay(100 / پورٽ_TICK_PERIOD_MS);
    esp_deregister_freertos_tick_hook_for_cpu(testint_assert, x_پورٽ_Get_Core_ID());
}
#جيڪڏهن_پورو // !CONFIG_FREERTOS_SMP
