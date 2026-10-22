/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 See if xPortInIsrContext works
*/

#अन्तर्भाव <esp_types.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"
#अन्तर्भाव "freertos/semphr.h"
#अन्तर्भाव "freertos/queue.h"
#अन्तर्भाव "unity.h"
#अन्तर्भाव "esp_intr_alloc.h"
#अन्तर्भाव "esp_rom_sys.h"
#अन्तर्भाव "esp_freertos_hooks.h"

#यदि CONFIG_FREERTOS_CORETIMER_0

स्थिर चञ्चल पूर्णाङ्क in_int_context, int_handled;

स्थिर शून्य testint(शून्य)
{
    esp_rom_printf("INT!\n");
    यदि (xPortInIsrContext()) {
        in_int_context++;
    }
    int_handled++;
}

स्थिर शून्य testthread(शून्य *arg)
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
    vTaskDelete(रिक्त);
}

TEST_CASE("xPortInIsrContext test", "[freertos]")
{
    xTaskCreatePinnedToCore(testthread, "tst", 4096, रिक्त, 3, रिक्त, 0);
    vTaskDelay(150 / portTICK_PERIOD_MS);
#यदि CONFIG_FREERTOS_संख्या_OF_CORES == 2
    xTaskCreatePinnedToCore(testthread, "tst", 4096, रिक्त, 3, रिक्त, 1);
    vTaskDelay(150 / portTICK_PERIOD_MS);
#endif
}

#endif

#यदि !CONFIG_FREERTOS_SMP // TODO: Enable when IDF-10540 is fixed

स्थिर शून्य testint_assert(शून्य)
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
