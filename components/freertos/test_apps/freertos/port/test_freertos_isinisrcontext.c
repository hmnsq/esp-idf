/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 See if xPortInIsrContext works
*/

#അന്തര്ഭാവ <esp_types.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ "sdkconfig.h"
#അന്തര്ഭാവ "freertos/FreeRTOS.h"
#അന്തര്ഭാവ "freertos/task.h"
#അന്തര്ഭാവ "freertos/semphr.h"
#അന്തര്ഭാവ "freertos/queue.h"
#അന്തര്ഭാവ "unity.h"
#അന്തര്ഭാവ "esp_intr_alloc.h"
#അന്തര്ഭാവ "esp_rom_sys.h"
#അന്തര്ഭാവ "esp_freertos_hooks.h"

#എങ്കിൽ CONFIG_FREERTOS_CORETIMER_0

സ്ഥിതം അസ്ഥിര പൂർണ്ണസംഖ്യ in_int_context, int_handled;

സ്ഥിതം ശൂന്യം testint(ശൂന്യം)
{
    esp_rom_printf("INT!\n");
    എങ്കിൽ (xPortInIsrContext()) {
        in_int_context++;
    }
    int_handled++;
}

സ്ഥിതം ശൂന്യം testthread(ശൂന്യം *arg)
{
    in_int_context = 0;
    int_handled = 0;
    TEST_ASSERT(!xPortInIsrContext());
    esp_err_t err = esp_register_freertos_tick_hook_for_cpu(testint, xPortGetCoreID());
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    TEST_ASSERT(int_handled);
    TEST_ASSERT(in_int_context);
    esp_deregister_freertos_tick_hook_for_cpu(testint, xPortGetCoreID());
    vTaskDelete(ശൂന്യ);
}

TEST_CASE("xPortInIsrContext test", "[freertos]")
{
    xTaskCreatePinnedToCore(testthread, "tst", 4096, ശൂന്യ, 3, ശൂന്യ, 0);
    vTaskDelay(150 / portTICK_PERIOD_MS);
#എങ്കിൽ CONFIG_FREERTOS_സംഖ്യ_OF_CORES == 2
    xTaskCreatePinnedToCore(testthread, "tst", 4096, ശൂന്യ, 3, ശൂന്യ, 1);
    vTaskDelay(150 / portTICK_PERIOD_MS);
#endif
}

#endif

#എങ്കിൽ !CONFIG_FREERTOS_SMP // TODO: Enable when IDF-10540 is fixed

സ്ഥിതം ശൂന്യം testint_assert(ശൂന്യം)
{
    esp_rom_printf("INT!\n");
    portASSERT_IF_IN_ISR();
}

TEST_CASE("port must assert if in ISR context", "[ignore]")
{
    esp_err_t err = esp_register_freertos_tick_hook_for_cpu(testint_assert, xPortGetCoreID());
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    esp_deregister_freertos_tick_hook_for_cpu(testint_assert, xPortGetCoreID());
}
#endif // !CONFIG_FREERTOS_SMP
