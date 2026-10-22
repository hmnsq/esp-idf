/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 See if xPortInIsrContext works
*/

#ಅನ್ತರ್ಭಾವ <esp_types.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "freertos/semphr.h"
#ಅನ್ತರ್ಭಾವ "freertos/queue.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "esp_intr_alloc.h"
#ಅನ್ತರ್ಭಾವ "esp_rom_sys.h"
#ಅನ್ತರ್ಭಾವ "esp_freertos_hooks.h"

#ಯದಿ CONFIG_FREERTOS_CORETIMER_0

ಸ್ಥಿರ ಚಞ್ಚಲ ಪೂರ್ಣಾಙ್ಕ in_int_context, int_handled;

ಸ್ಥಿರ ಶೂನ್ಯ testint(ಶೂನ್ಯ)
{
    esp_rom_printf("INT!\n");
    ಯದಿ (xPortInIsrContext()) {
        in_int_context++;
    }
    int_handled++;
}

ಸ್ಥಿರ ಶೂನ್ಯ testthread(ಶೂನ್ಯ *arg)
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
    vTaskDelete(ರಿಕ್ತ);
}

TEST_CASE("xPortInIsrContext test", "[freertos]")
{
    xTaskCreatePinnedToCore(testthread, "tst", 4096, ರಿಕ್ತ, 3, ರಿಕ್ತ, 0);
    vTaskDelay(150 / portTICK_PERIOD_MS);
#ಯದಿ CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES == 2
    xTaskCreatePinnedToCore(testthread, "tst", 4096, ರಿಕ್ತ, 3, ರಿಕ್ತ, 1);
    vTaskDelay(150 / portTICK_PERIOD_MS);
#endif
}

#endif

#ಯದಿ !CONFIG_FREERTOS_SMP // TODO: Enable when IDF-10540 is fixed

ಸ್ಥಿರ ಶೂನ್ಯ testint_assert(ಶೂನ್ಯ)
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
