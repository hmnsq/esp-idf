/*
 * SPDX-FileCopyrightText: 2022-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#অন্তর্ভাব "freertos/FreeRTOS.h"
#অন্তর্ভাব "freertos/task.h"
#অন্তর্ভাব "unity.h"
#অন্তর্ভাব "unity_test_runner.h"
#অন্তর্ভাব "esp_heap_caps.h"
#অন্তর্ভাব "test_utils.h"
#অন্তর্ভাব "esp_task_wdt.h"

#define TEST_MEMORY_LEAK_THRESHOLD (-512)

স্থির size_t before_free_8bit;
স্থির size_t before_free_32bit;

স্থির শূন্য check_leak(size_t before_free, size_t after_free, ধ্রুব বর্ণ *type)
{
    ssize_t delta = after_free - before_free;
    printf("MALLOC_CAP_%s: Before %u bytes free, After %u bytes free (delta %d)\n", type, before_free, after_free, delta);
    TEST_ASSERT_MESSAGE(delta >= TEST_MEMORY_LEAK_THRESHOLD, "memory leak");
}

শূন্য setUp(শূন্য)
{
    before_free_8bit = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    before_free_32bit = heap_caps_get_free_size(MALLOC_CAP_32BIT);

#যদি SOC_WDT_SUPPORTED
    esp_task_wdt_add(রিক্ত);
    esp_task_wdt_reset();
#endif

}

শূন্য tearDown(শূন্য)
{
#যদি SOC_WDT_SUPPORTED
    esp_task_wdt_reset();
    esp_task_wdt_delete(রিক্ত);
#endif

    // Add a short delay of 10 ticks to allow the idle task to free remaining memory.
    vTaskDelay(10);
    size_t after_free_8bit = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    size_t after_free_32bit = heap_caps_get_free_size(MALLOC_CAP_32BIT);
    check_leak(before_free_8bit, after_free_8bit, "8BIT");
    check_leak(before_free_32bit, after_free_32bit, "32BIT");
}

শূন্য app_main(শূন্য)
{

#যদি SOC_WDT_SUPPORTED
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
    vTaskPrioritySet(রিক্ত, UNITY_FREERTOS_PRIORITY);
    printf(" ______             _____ _______ ____   _____\n");
    printf("|  ____|           |  __ \\__   __/ __ \\ / ____|\n");
    printf("| |__ _ __ ___  ___| |__) | | | | |  | | (___\n");
    printf("|  __| '__/ _ \\/ _ \\  _  /  | | | |  | |\\___ \\\n");
    printf("| |  | | |  __/  __/ | \\ \\  | | | |__| |____) |\n");
    printf("|_|  |_|  \\___|\\___|_|  \\_\\ |_|  \\____/|_____/\n");
    unity_run_menu();
}
