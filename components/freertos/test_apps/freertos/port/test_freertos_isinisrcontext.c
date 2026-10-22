/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 See if xPortInIsrContext works
*/

#ਅਨ੍ਤਰ੍ਭਾਵ <esp_types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/semphr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/queue.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_intr_alloc.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_rom_sys.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_freertos_hooks.h"

#ਜੇ CONFIG_FREERTOS_CORETIMER_0

ਸਥਿਰ ਅਸਥਿਰ ਪੂਰਨ ਵਿੱਚ_int_context, int_handled;

ਸਥਿਰ ਖਾਲੀ testint(ਖਾਲੀ)
{
    esp_rom_printf("INT!\n");
    ਜੇ (x_ਪੋਰਟ_ਵਿੱਚ_Isr_Context()) {
        ਵਿੱਚ_int_context++;
    }
    int_handled++;
}

ਸਥਿਰ ਖਾਲੀ testthread(ਖਾਲੀ *arg)
{
    ਵਿੱਚ_int_context = 0;
    int_handled = 0;
    TEST_ASSERT(!x_ਪੋਰਟ_ਵਿੱਚ_Isr_Context());
    esp_err_t err = esp_ਰਜਿਸਟਰ_freertos_tick_hook_for_cpu(testint, x_ਪੋਰਟ_Get_Core_ID());
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);
    vTaskDelay(100 / ਪੋਰਟ_TICK_PERIOD_MS);
    TEST_ASSERT(int_handled);
    TEST_ASSERT(ਵਿੱਚ_int_context);
    esp_deregister_freertos_tick_hook_for_cpu(testint, x_ਪੋਰਟ_Get_Core_ID());
    vTaskDelete(ਨਿਰਾ);
}

TEST_CASE("xPortInIsrContext test", "[freertos]")
{
    x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(testthread, "tst", 4096, ਨਿਰਾ, 3, ਨਿਰਾ, 0);
    vTaskDelay(150 / ਪੋਰਟ_TICK_PERIOD_MS);
#ਜੇ CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES == 2
    x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(testthread, "tst", 4096, ਨਿਰਾ, 3, ਨਿਰਾ, 1);
    vTaskDelay(150 / ਪੋਰਟ_TICK_PERIOD_MS);
#ਜੇ_ਅੰਤ
}

#ਜੇ_ਅੰਤ

#ਜੇ !CONFIG_FREERTOS_SMP // TODO: Enable when IDF-10540 is fixed

ਸਥਿਰ ਖਾਲੀ testint_assert(ਖਾਲੀ)
{
    esp_rom_printf("INT!\n");
    ਪੋਰਟ_ASSERT_IF_ਵਿੱਚ_ISR();
}

TEST_CASE("port must assert if in ISR context", "[ignore]")
{
    esp_err_t err = esp_ਰਜਿਸਟਰ_freertos_tick_hook_for_cpu(testint_assert, x_ਪੋਰਟ_Get_Core_ID());
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);
    vTaskDelay(100 / ਪੋਰਟ_TICK_PERIOD_MS);
    esp_deregister_freertos_tick_hook_for_cpu(testint_assert, x_ਪੋਰਟ_Get_Core_ID());
}
#ਜੇ_ਅੰਤ // !CONFIG_FREERTOS_SMP
