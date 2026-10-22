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

#define TEST_MEMORY_LEAK_THRESHOLD (-512)

स्थिर माप_प्रकार before_free_8bit;
स्थिर माप_प्रकार before_free_32bit;

स्थिर शून्य check_leak(माप_प्रकार before_free, माप_प्रकार after_free, ध्रुव वर्ण *type)
{
    ssize_t delta = after_free - before_free;
    म_लिखो("MALLOC_CAP_%s: Before %u bytes free, After %u bytes free (delta %d)\n", type, before_free, after_free, delta);
    TEST_ASSERT_MESSAGE(delta >= TEST_MEMORY_LEAK_THRESHOLD, "memory leak");
}

शून्य setUp(शून्य)
{
    before_free_8bit = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    before_free_32bit = heap_caps_get_free_size(MALLOC_CAP_32BIT);

#यदि SOC_WDT_SUPPORTED
    esp_task_wdt_add(रिक्त);
    esp_task_wdt_reset();
#endif

}

शून्य tearDown(शून्य)
{
#यदि SOC_WDT_SUPPORTED
    esp_task_wdt_reset();
    esp_task_wdt_delete(रिक्त);
#endif

    // Add a short delay of 10 ticks to allow the idle task to free remaining memory.
    vTaskDelay(10);
    माप_प्रकार after_free_8bit = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    माप_प्रकार after_free_32bit = heap_caps_get_free_size(MALLOC_CAP_32BIT);
    check_leak(before_free_8bit, after_free_8bit, "8BIT");
    check_leak(before_free_32bit, after_free_32bit, "32BIT");
}

शून्य app_main(शून्य)
{

#यदि SOC_WDT_SUPPORTED
    // Configure the task watchdog timer to catch any tests that hang
    esp_task_wdt_config_t config = {
        .timeout_ms = 25 * 1000, // 25 seconds, smaller than the default pytest timeout
        .idle_core_mask = 0,
        .trigger_panic = true,
    };

    esp_task_wdt_init(&config);
#endif // SOC_WDT_SUPPORTED

    /*
    Some FreeRTOS tests are reliant on the main task being at priority UNITY_FREERTOS_PRIORITY to test scheduling
    behavior. Thus, we raise the main task's priority before any tasks are run. See IDF-6088
    */
    vTaskPrioritySet(रिक्त, UNITY_FREERTOS_PRIORITY);
    म_लिखो(" ______             _____ _______ ____   _____\n");
    म_लिखो("|  ____|           |  __ \\__   __/ __ \\ / ____|\n");
    म_लिखो("| |__ _ __ ___  ___| |__) | | | | |  | | (___\n");
    म_लिखो("|  __| '__/ _ \\/ _ \\  _  /  | | | |  | |\\___ \\\n");
    म_लिखो("| |  | | |  __/  __/ | \\ \\  | | | |__| |____) |\n");
    म_लिखो("|_|  |_|  \\___|\\___|_|  \\_\\ |_|  \\____/|_____/\n");
    unity_run_menu();
}
