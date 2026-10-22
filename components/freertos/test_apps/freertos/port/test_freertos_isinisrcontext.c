/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 See if xPortInIsrContext works
*/

#समावेश <esp_types.h>
#समावेश <stdio.h>
#समावेश "sdkconfig.h"
#समावेश "freertos/FreeRTOS.h"
#समावेश "freertos/task.h"
#समावेश "freertos/semphr.h"
#समावेश "freertos/queue.h"
#समावेश "unity.h"
#समावेश "esp_intr_alloc.h"
#समावेश "esp_rom_sys.h"
#समावेश "esp_freertos_hooks.h"

#अगर CONFIG_FREERTOS_CORETIMER_0

अटल अस्थिर पूर्णांक in_int_context, int_handled;

अटल व्योम testint(व्योम)
{
    esp_rom_printf("INT!\n");
    अगर (xPortInIsrContext()) {
        in_int_context++;
    }
    int_handled++;
}

अटल व्योम testthread(व्योम *arg)
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
    vTaskDelete(शून्य);
}

TEST_CASE("xPortInIsrContext test", "[freertos]")
{
    xTaskCreatePinnedToCore(testthread, "tst", 4096, शून्य, 3, शून्य, 0);
    vTaskDelay(150 / portTICK_PERIOD_MS);
#अगर CONFIG_FREERTOS_संख्या_OF_CORES == 2
    xTaskCreatePinnedToCore(testthread, "tst", 4096, शून्य, 3, शून्य, 1);
    vTaskDelay(150 / portTICK_PERIOD_MS);
#पूर्ण_अगर
}

#पूर्ण_अगर

#अगर !CONFIG_FREERTOS_SMP // TODO: Enable when IDF-10540 is fixed

अटल व्योम testint_assert(व्योम)
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
#पूर्ण_अगर // !CONFIG_FREERTOS_SMP
