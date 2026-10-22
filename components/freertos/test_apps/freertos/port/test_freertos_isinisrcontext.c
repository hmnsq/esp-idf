/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 See if xPortInIsrContext works
*/

#અન્તર્ભાવ <esp_types.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ "sdkconfig.h"
#અન્તર્ભાવ "freertos/FreeRTOS.h"
#અન્તર્ભાવ "freertos/task.h"
#અન્તર્ભાવ "freertos/semphr.h"
#અન્તર્ભાવ "freertos/queue.h"
#અન્તર્ભાવ "unity.h"
#અન્તર્ભાવ "esp_intr_alloc.h"
#અન્તર્ભાવ "esp_rom_sys.h"
#અન્તર્ભાવ "esp_freertos_hooks.h"

#જો CONFIG_FREERTOS_CORETIMER_0

સ્થિર અસ્થિર પૂર્ણાંક in_int_context, int_handled;

સ્થિર શૂન્ય testint(શૂન્ય)
{
    esp_rom_printf("INT!\n");
    જો (xPortInIsrContext()) {
        in_int_context++;
    }
    int_handled++;
}

સ્થિર શૂન્ય testthread(શૂન્ય *arg)
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
    vTaskDelete(ખાલી);
}

TEST_CASE("xPortInIsrContext test", "[freertos]")
{
    xTaskCreatePinnedToCore(testthread, "tst", 4096, ખાલી, 3, ખાલી, 0);
    vTaskDelay(150 / portTICK_PERIOD_MS);
#જો CONFIG_FREERTOS_સંખ્યા_OF_CORES == 2
    xTaskCreatePinnedToCore(testthread, "tst", 4096, ખાલી, 3, ખાલી, 1);
    vTaskDelay(150 / portTICK_PERIOD_MS);
#endif
}

#endif

#જો !CONFIG_FREERTOS_SMP // TODO: Enable when IDF-10540 is fixed

સ્થિર શૂન્ય testint_assert(શૂન્ય)
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
