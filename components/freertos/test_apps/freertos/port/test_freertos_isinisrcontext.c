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

#ಆದರೆ CONFIG_FREERTOS_CORETIMER_0

ಸ್ಥಿರ ಅಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಒಳ_int_context, int_handled;

ಸ್ಥಿರ ಶೂನ್ಯ testint(ಶೂನ್ಯ)
{
    esp_rom_printf("INT!\n");
    ಆದರೆ (x_ಪೋರ್ಟ್_ಒಳ_Isr_Context()) {
        ಒಳ_int_context++;
    }
    int_handled++;
}

ಸ್ಥಿರ ಶೂನ್ಯ testthread(ಶೂನ್ಯ *arg)
{
    ಒಳ_int_context = 0;
    int_handled = 0;
    TEST_ASSERT(!x_ಪೋರ್ಟ್_ಒಳ_Isr_Context());
    esp_err_t err = esp_ರಿಜಿಸ್ಟರ್_freertos_tick_hook_for_cpu(testint, x_ಪೋರ್ಟ್_Get_Core_ID());
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);
    vTaskDelay(100 / ಪೋರ್ಟ್_TICK_PERIOD_MS);
    TEST_ASSERT(int_handled);
    TEST_ASSERT(ಒಳ_int_context);
    esp_deregister_freertos_tick_hook_for_cpu(testint, x_ಪೋರ್ಟ್_Get_Core_ID());
    vTaskDelete(ಖಾಲಿ);
}

TEST_CASE("xPortInIsrContext test", "[freertos]")
{
    x_Task_Create_Pinned_ಗಮ್ಯ_Core(testthread, "tst", 4096, ಖಾಲಿ, 3, ಖಾಲಿ, 0);
    vTaskDelay(150 / ಪೋರ್ಟ್_TICK_PERIOD_MS);
#ಆದರೆ CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES == 2
    x_Task_Create_Pinned_ಗಮ್ಯ_Core(testthread, "tst", 4096, ಖಾಲಿ, 3, ಖಾಲಿ, 1);
    vTaskDelay(150 / ಪೋರ್ಟ್_TICK_PERIOD_MS);
#ಆದರೆ_ಅಂತ್ಯ
}

#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ !CONFIG_FREERTOS_SMP // TODO: Enable when IDF-10540 is fixed

ಸ್ಥಿರ ಶೂನ್ಯ testint_assert(ಶೂನ್ಯ)
{
    esp_rom_printf("INT!\n");
    ಪೋರ್ಟ್_ASSERT_IF_ಒಳ_ISR();
}

TEST_CASE("port must assert if in ISR context", "[ignore]")
{
    esp_err_t err = esp_ರಿಜಿಸ್ಟರ್_freertos_tick_hook_for_cpu(testint_assert, x_ಪೋರ್ಟ್_Get_Core_ID());
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);
    vTaskDelay(100 / ಪೋರ್ಟ್_TICK_PERIOD_MS);
    esp_deregister_freertos_tick_hook_for_cpu(testint_assert, x_ಪೋರ್ಟ್_Get_Core_ID());
}
#ಆದರೆ_ಅಂತ್ಯ // !CONFIG_FREERTOS_SMP
