/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 See if xPortInIsrContext works
*/

#شامل <esp_types.h>
#شامل <stdio.h>
#شامل "sdkconfig.h"
#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل "freertos/semphr.h"
#شامل "freertos/queue.h"
#شامل "unity.h"
#شامل "esp_intr_alloc.h"
#شامل "esp_rom_sys.h"
#شامل "esp_freertos_hooks.h"

#اگر CONFIG_FREERTOS_CORETIMER_0

ایستا فرار صحیح in_int_context, int_handled;

ایستا تهی testint(تهی)
{
    esp_rom_printf("INT!\n");
    اگر (xPortInIsrContext()) {
        in_int_context++;
    }
    int_handled++;
}

ایستا تهی testthread(تهی *arg)
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
    vTaskDelete(پوچ);
}

TEST_CASE("xPortInIsrContext test", "[freertos]")
{
    xTaskCreatePinnedToCore(testthread, "tst", 4096, پوچ, 3, پوچ, 0);
    vTaskDelay(150 / portTICK_PERIOD_MS);
#اگر CONFIG_FREERTOS_عدد_OF_CORES == 2
    xTaskCreatePinnedToCore(testthread, "tst", 4096, پوچ, 3, پوچ, 1);
    vTaskDelay(150 / portTICK_PERIOD_MS);
#endif
}

#endif

#اگر !CONFIG_FREERTOS_SMP // TODO: Enable when IDF-10540 is fixed

ایستا تهی testint_assert(تهی)
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
