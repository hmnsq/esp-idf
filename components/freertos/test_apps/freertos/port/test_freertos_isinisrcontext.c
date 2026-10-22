/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 See if xPortInIsrContext works
*/

#அந்தர்பாவ <esp_types.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ "sdkconfig.h"
#அந்தர்பாவ "freertos/FreeRTOS.h"
#அந்தர்பாவ "freertos/task.h"
#அந்தர்பாவ "freertos/semphr.h"
#அந்தர்பாவ "freertos/queue.h"
#அந்தர்பாவ "unity.h"
#அந்தர்பாவ "esp_intr_alloc.h"
#அந்தர்பாவ "esp_rom_sys.h"
#அந்தர்பாவ "esp_freertos_hooks.h"

#எனில் CONFIG_FREERTOS_CORETIMER_0

நிலையான நிலையற்ற முழு in_int_context, int_handled;

நிலையான வெறுமை testint(வெறுமை)
{
    esp_rom_printf("INT!\n");
    எனில் (xPortInIsrContext()) {
        in_int_context++;
    }
    int_handled++;
}

நிலையான வெறுமை testthread(வெறுமை *arg)
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
    vTaskDelete(வெற்று);
}

TEST_CASE("xPortInIsrContext test", "[freertos]")
{
    xTaskCreatePinnedToCore(testthread, "tst", 4096, வெற்று, 3, வெற்று, 0);
    vTaskDelay(150 / portTICK_PERIOD_MS);
#எனில் CONFIG_FREERTOS_எண்_OF_CORES == 2
    xTaskCreatePinnedToCore(testthread, "tst", 4096, வெற்று, 3, வெற்று, 1);
    vTaskDelay(150 / portTICK_PERIOD_MS);
#endif
}

#endif

#எனில் !CONFIG_FREERTOS_SMP // TODO: Enable when IDF-10540 is fixed

நிலையான வெறுமை testint_assert(வெறுமை)
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
