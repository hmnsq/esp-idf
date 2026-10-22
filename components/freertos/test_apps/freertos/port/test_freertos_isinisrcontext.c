/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 See if xPortInIsrContext works
*/

#ضمن <esp_types.h>
#ضمن <stdio.h>
#ضمن "sdkconfig.h"
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "freertos/semphr.h"
#ضمن "freertos/queue.h"
#ضمن "unity.h"
#ضمن "esp_intr_alloc.h"
#ضمن "esp_rom_sys.h"
#ضمن "esp_freertos_hooks.h"

#إذا CONFIG_FREERTOS_CORETIMER_0

ساكن متطاير صحيح دننه_int_context, int_handled;

ساكن فراغ testint(فراغ)
{
    esp_rom_printf("INT!\n");
    إذا (x_پورټ_دننه_Isr_Context()) {
        دننه_int_context++;
    }
    int_handled++;
}

ساكن فراغ testthread(فراغ *arg)
{
    دننه_int_context = 0;
    int_handled = 0;
    TEST_ASSERT(!x_پورټ_دننه_Isr_Context());
    esp_err_t err = esp_ثبتوونکی_freertos_tick_hook_for_cpu(testint, x_پورټ_Get_Core_ID());
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);
    vTaskDelay(100 / پورټ_TICK_PERIOD_MS);
    TEST_ASSERT(int_handled);
    TEST_ASSERT(دننه_int_context);
    esp_deregister_freertos_tick_hook_for_cpu(testint, x_پورټ_Get_Core_ID());
    vTaskDelete(عدم);
}

TEST_CASE("xPortInIsrContext test", "[freertos]")
{
    x_Task_Create_Pinned_منزل_Core(testthread, "tst", 4096, عدم, 3, عدم, 0);
    vTaskDelay(150 / پورټ_TICK_PERIOD_MS);
#إذا CONFIG_FREERTOS_شمېره_د_CORES == 2
    x_Task_Create_Pinned_منزل_Core(testthread, "tst", 4096, عدم, 3, عدم, 1);
    vTaskDelay(150 / پورټ_TICK_PERIOD_MS);
#د_که_پای
}

#د_که_پای

#إذا !CONFIG_FREERTOS_SMP // TODO: Enable when IDF-10540 is fixed

ساكن فراغ testint_assert(فراغ)
{
    esp_rom_printf("INT!\n");
    پورټ_ASSERT_IF_دننه_ISR();
}

TEST_CASE("port must assert if in ISR context", "[ignore]")
{
    esp_err_t err = esp_ثبتوونکی_freertos_tick_hook_for_cpu(testint_assert, x_پورټ_Get_Core_ID());
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);
    vTaskDelay(100 / پورټ_TICK_PERIOD_MS);
    esp_deregister_freertos_tick_hook_for_cpu(testint_assert, x_پورټ_Get_Core_ID());
}
#د_که_پای // !CONFIG_FREERTOS_SMP
