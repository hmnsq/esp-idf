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

#𐡄𐡍 CONFIG_FREERTOS_CORETIMER_0

ساكن متطاير صحيح in_int_context, int_handled;

ساكن فراغ testint(فراغ)
{
    esp_rom_printf("INT!\n");
    𐡄𐡍 (xPortInIsrContext()) {
        in_int_context++;
    }
    int_handled++;
}

ساكن فراغ testthread(فراغ *arg)
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
    vTaskDelete(عدم);
}

TEST_CASE("xPortInIsrContext test", "[freertos]")
{
    xTaskCreatePinnedToCore(testthread, "tst", 4096, عدم, 3, عدم, 0);
    vTaskDelay(150 / portTICK_PERIOD_MS);
#𐡄𐡍 CONFIG_FREERTOS_NUMBER_OF_CORES == 2
    xTaskCreatePinnedToCore(testthread, "tst", 4096, عدم, 3, عدم, 1);
    vTaskDelay(150 / portTICK_PERIOD_MS);
#endif
}

#endif

#𐡄𐡍 !CONFIG_FREERTOS_SMP // TODO: Enable when IDF-10540 is fixed

ساكن فراغ testint_assert(فراغ)
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
