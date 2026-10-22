/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 See if xPortInIsrContext works
*/

#అన్తర్భావ <esp_types.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "freertos/semphr.h"
#అన్తర్భావ "freertos/queue.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "esp_intr_alloc.h"
#అన్తర్భావ "esp_rom_sys.h"
#అన్తర్భావ "esp_freertos_hooks.h"

#ఒకవేళ CONFIG_FREERTOS_CORETIMER_0

స్థిర అస్థిర పూర్ణాంకం లోపల_int_context, int_handled;

స్థిర శూన్య testint(శూన్య)
{
    esp_rom_printf("INT!\n");
    ఒకవేళ (x_పోర్టు_లోపల_Isr_Context()) {
        లోపల_int_context++;
    }
    int_handled++;
}

స్థిర శూన్య testthread(శూన్య *arg)
{
    లోపల_int_context = 0;
    int_handled = 0;
    TEST_ASSERT(!x_పోర్టు_లోపల_Isr_Context());
    esp_err_t err = esp_రిజిస్టరు_freertos_tick_hook_for_cpu(testint, x_పోర్టు_Get_Core_ID());
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);
    vTaskDelay(100 / పోర్టు_TICK_PERIOD_MS);
    TEST_ASSERT(int_handled);
    TEST_ASSERT(లోపల_int_context);
    esp_deregister_freertos_tick_hook_for_cpu(testint, x_పోర్టు_Get_Core_ID());
    vTaskDelete(ఖాళీ);
}

TEST_CASE("xPortInIsrContext test", "[freertos]")
{
    x_Task_Create_Pinned_గమ్యం_Core(testthread, "tst", 4096, ఖాళీ, 3, ఖాళీ, 0);
    vTaskDelay(150 / పోర్టు_TICK_PERIOD_MS);
#ఒకవేళ CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES == 2
    x_Task_Create_Pinned_గమ్యం_Core(testthread, "tst", 4096, ఖాళీ, 3, ఖాళీ, 1);
    vTaskDelay(150 / పోర్టు_TICK_PERIOD_MS);
#అయితే_ముగింపు
}

#అయితే_ముగింపు

#ఒకవేళ !CONFIG_FREERTOS_SMP // TODO: Enable when IDF-10540 is fixed

స్థిర శూన్య testint_assert(శూన్య)
{
    esp_rom_printf("INT!\n");
    పోర్టు_ASSERT_IF_లోపల_ISR();
}

TEST_CASE("port must assert if in ISR context", "[ignore]")
{
    esp_err_t err = esp_రిజిస్టరు_freertos_tick_hook_for_cpu(testint_assert, x_పోర్టు_Get_Core_ID());
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);
    vTaskDelay(100 / పోర్టు_TICK_PERIOD_MS);
    esp_deregister_freertos_tick_hook_for_cpu(testint_assert, x_పోర్టు_Get_Core_ID());
}
#అయితే_ముగింపు // !CONFIG_FREERTOS_SMP
