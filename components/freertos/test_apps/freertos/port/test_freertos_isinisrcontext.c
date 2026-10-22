/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 See if xPortInIsrContext works
*/

#অন্তর্ভাৱ <esp_types.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ "sdkconfig.h"
#অন্তর্ভাৱ "freertos/FreeRTOS.h"
#অন্তর্ভাৱ "freertos/task.h"
#অন্তর্ভাৱ "freertos/semphr.h"
#অন্তর্ভাৱ "freertos/queue.h"
#অন্তর্ভাৱ "unity.h"
#অন্তর্ভাৱ "esp_intr_alloc.h"
#অন্তর্ভাৱ "esp_rom_sys.h"
#অন্তর্ভাৱ "esp_freertos_hooks.h"

#যদি CONFIG_FREERTOS_CORETIMER_0

স্থির চঞ্চল পূর্ণাঙ্ক in_int_context, int_handled;

স্থির শূন্য testint(শূন্য)
{
    esp_rom_printf("INT!\n");
    যদি (xPortInIsrContext()) {
        in_int_context++;
    }
    int_handled++;
}

স্থির শূন্য testthread(শূন্য *arg)
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
    vTaskDelete(রিক্ত);
}

TEST_CASE("xPortInIsrContext test", "[freertos]")
{
    xTaskCreatePinnedToCore(testthread, "tst", 4096, রিক্ত, 3, রিক্ত, 0);
    vTaskDelay(150 / portTICK_PERIOD_MS);
#যদি CONFIG_FREERTOS_সংখ্যা_OF_CORES == 2
    xTaskCreatePinnedToCore(testthread, "tst", 4096, রিক্ত, 3, রিক্ত, 1);
    vTaskDelay(150 / portTICK_PERIOD_MS);
#endif
}

#endif

#যদি !CONFIG_FREERTOS_SMP // TODO: Enable when IDF-10540 is fixed

স্থির শূন্য testint_assert(শূন্য)
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
