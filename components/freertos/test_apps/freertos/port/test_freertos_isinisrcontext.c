/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 See if xPortInIsrContext works
*/

#ଅନ୍ତର୍ଭାଵ <esp_types.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ "sdkconfig.h"
#ଅନ୍ତର୍ଭାଵ "freertos/FreeRTOS.h"
#ଅନ୍ତର୍ଭାଵ "freertos/task.h"
#ଅନ୍ତର୍ଭାଵ "freertos/semphr.h"
#ଅନ୍ତର୍ଭାଵ "freertos/queue.h"
#ଅନ୍ତର୍ଭାଵ "unity.h"
#ଅନ୍ତର୍ଭାଵ "esp_intr_alloc.h"
#ଅନ୍ତର୍ଭାଵ "esp_rom_sys.h"
#ଅନ୍ତର୍ଭାଵ "esp_freertos_hooks.h"

#ଯଦି CONFIG_FREERTOS_CORETIMER_0

ସ୍ଥିର ଅସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା in_int_context, int_handled;

ସ୍ଥିର ଶୂନ୍ୟ testint(ଶୂନ୍ୟ)
{
    esp_rom_printf("INT!\n");
    ଯଦି (xPortInIsrContext()) {
        in_int_context++;
    }
    int_handled++;
}

ସ୍ଥିର ଶୂନ୍ୟ testthread(ଶୂନ୍ୟ *arg)
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
    vTaskDelete(ଖାଲି);
}

TEST_CASE("xPortInIsrContext test", "[freertos]")
{
    xTaskCreatePinnedToCore(testthread, "tst", 4096, ଖାଲି, 3, ଖାଲି, 0);
    vTaskDelay(150 / portTICK_PERIOD_MS);
#ଯଦି CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES == 2
    xTaskCreatePinnedToCore(testthread, "tst", 4096, ଖାଲି, 3, ଖାଲି, 1);
    vTaskDelay(150 / portTICK_PERIOD_MS);
#endif
}

#endif

#ଯଦି !CONFIG_FREERTOS_SMP // TODO: Enable when IDF-10540 is fixed

ସ୍ଥିର ଶୂନ୍ୟ testint_assert(ଶୂନ୍ୟ)
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
