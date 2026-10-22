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

स्थिर चञ्चल पूर्णाङ्क अन्तर्_int_context, int_handled;

स्थिर शून्य testint(शून्य)
{
    esp_rom_printf("INT!\n");
    यदि (x_पत्तनम्_अन्तर्_Isr_Context()) {
        अन्तर्_int_context++;
    }
    int_handled++;
}

स्थिर शून्य testthread(शून्य *arg)
{
    अन्तर्_int_context = 0;
    int_handled = 0;
    TEST_ASSERT(!x_पत्तनम्_अन्तर्_Isr_Context());
    esp_err_t err = esp_पञ्जिकाकोशः_freertos_tick_hook_for_cpu(testint, x_पत्तनम्_Get_Core_ID());
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);
    vTaskDelay(100 / पत्तनम्_TICK_PERIOD_MS);
    TEST_ASSERT(int_handled);
    TEST_ASSERT(अन्तर्_int_context);
    esp_deregister_freertos_tick_hook_for_cpu(testint, x_पत्तनम्_Get_Core_ID());
    vTaskDelete(रिक्त);
}

TEST_CASE("xPortInIsrContext test", "[freertos]")
{
    x_Task_Create_Pinned_गन्तव्यम्_Core(testthread, "tst", 4096, रिक्त, 3, रिक्त, 0);
    vTaskDelay(150 / पत्तनम्_TICK_PERIOD_MS);
#यदि CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES == 2
    x_Task_Create_Pinned_गन्तव्यम्_Core(testthread, "tst", 4096, रिक्त, 3, रिक्त, 1);
    vTaskDelay(150 / पत्तनम्_TICK_PERIOD_MS);
#यदि_अन्तः
}

#यदि_अन्तः

#यदि !CONFIG_FREERTOS_SMP // TODO: Enable when IDF-10540 is fixed

स्थिर शून्य testint_assert(शून्य)
{
    esp_rom_printf("INT!\n");
    पत्तनम्_ASSERT_IF_अन्तर्_ISR();
}

TEST_CASE("port must assert if in ISR context", "[ignore]")
{
    esp_err_t err = esp_पञ्जिकाकोशः_freertos_tick_hook_for_cpu(testint_assert, x_पत्तनम्_Get_Core_ID());
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);
    vTaskDelay(100 / पत्तनम्_TICK_PERIOD_MS);
    esp_deregister_freertos_tick_hook_for_cpu(testint_assert, x_पत्तनम्_Get_Core_ID());
}
#यदि_अन्तः // !CONFIG_FREERTOS_SMP
