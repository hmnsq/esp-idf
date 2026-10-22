/*
 * SPDX-FileCopyrightText: 2022-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"
#अन्तर्भाव "unity.h"
#अन्तर्भाव "unity_test_runner.h"
#अन्तर्भाव "esp_heap_caps.h"
#अन्तर्भाव "test_utils.h"
#अन्तर्भाव "esp_task_wdt.h"

#परिभाषय TEST_स्मृतिः_LEAK_देहली (-512)

स्थिर size_t before_free_8bit;
स्थिर size_t before_free_32bit;

स्थिर शून्य check_leak(size_t before_free, size_t after_free, ध्रुव वर्ण *type)
{
    ssize_t delta = after_free - before_free;
    प्रारूप_लिख("MALLOC_CAP_%s: Before %u bytes free, After %u bytes free (delta %d)\n", type, before_free, after_free, delta);
    TEST_ASSERT_सन्देशः(delta >= TEST_स्मृतिः_LEAK_देहली, "memory leak");
}

शून्य समुच्चयः_ऊर्ध्वम्(शून्य)
{
    before_free_8bit = heap_caps_get_free_आकार(MALLOC_CAP_8BIT);
    before_free_32bit = heap_caps_get_free_आकार(MALLOC_CAP_32BIT);

#यदि SOC_WDT_SUPPORTED
    esp_task_wdt_सङ्कलय(रिक्त);
    esp_task_wdt_reset();
#यदि_अन्तः

}

शून्य tear_अधस्तात्(शून्य)
{
#यदि SOC_WDT_SUPPORTED
    esp_task_wdt_reset();
    esp_task_wdt_delete(रिक्त);
#यदि_अन्तः

    // Add a short delay of 10 ticks to allow the idle task to free remaining memory.
    vTaskDelay(10);
    size_t after_free_8bit = heap_caps_get_free_आकार(MALLOC_CAP_8BIT);
    size_t after_free_32bit = heap_caps_get_free_आकार(MALLOC_CAP_32BIT);
    check_leak(before_free_8bit, after_free_8bit, "8BIT");
    check_leak(before_free_32bit, after_free_32bit, "32BIT");
}

शून्य app_main(शून्य)
{

#यदि SOC_WDT_SUPPORTED
    // Configure the task watchdog timer to catch any tests that hang
    esp_task_wdt_config_t config = {
        .timeout_ms = 25 * 1000, // 25 seconds, smaller than the default pytest timeout
        .idle_core_आवरणम् = 0,
        .trigger_panic = true,
    };

    esp_task_wdt_init(&config);
#यदि_अन्तः // SOC_WDT_SUPPORTED

    /*
    Some FreeRTOS tests are reliant on the main task being at priority UNITY_FREERTOS_PRIORITY to test scheduling
    behavior. Thus, we raise the main task's priority before any tasks are run. See IDF-6088
    */
    v_Task_प्राथमिकता_समुच्चयः(रिक्त, UNITY_FREERTOS_प्राथमिकता);
    प्रारूप_लिख(" ______             _____ _______ ____   _____\n");
    प्रारूप_लिख("|  ____|           |  __ \\__   __/ __ \\ / ____|\n");
    प्रारूप_लिख("| |__ _ __ ___  ___| |__) | | | | |  | | (___\n");
    प्रारूप_लिख("|  __| '__/ _ \\/ _ \\  _  /  | | | |  | |\\___ \\\n");
    प्रारूप_लिख("| |  | | |  __/  __/ | \\ \\  | | | |__| |____) |\n");
    प्रारूप_लिख("|_|  |_|  \\___|\\___|_|  \\_\\ |_|  \\____/|_____/\n");
    unity_सततम्_menu();
}
