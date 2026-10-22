/*
 * SPDX-FileCopyrightText: 2022-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#அந்தர்பாவ "freertos/FreeRTOS.h"
#அந்தர்பாவ "freertos/task.h"
#அந்தர்பாவ "unity.h"
#அந்தர்பாவ "unity_test_runner.h"
#அந்தர்பாவ "esp_heap_caps.h"
#அந்தர்பாவ "test_utils.h"
#அந்தர்பாவ "esp_task_wdt.h"

#define TEST_MEMORY_LEAK_THRESHOLD (-512)

நிலையான size_t before_free_8bit;
நிலையான size_t before_free_32bit;

நிலையான வெறுமை check_leak(size_t before_free, size_t after_free, மாறிலி எழுத்து *type)
{
    ssize_t delta = after_free - before_free;
    printf("MALLOC_CAP_%s: Before %u bytes free, After %u bytes free (delta %d)\n", type, before_free, after_free, delta);
    TEST_ASSERT_MESSAGE(delta >= TEST_MEMORY_LEAK_THRESHOLD, "memory leak");
}

வெறுமை setUp(வெறுமை)
{
    before_free_8bit = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    before_free_32bit = heap_caps_get_free_size(MALLOC_CAP_32BIT);

#எனில் SOC_WDT_SUPPORTED
    esp_task_wdt_add(வெற்று);
    esp_task_wdt_reset();
#endif

}

வெறுமை tearDown(வெறுமை)
{
#எனில் SOC_WDT_SUPPORTED
    esp_task_wdt_reset();
    esp_task_wdt_delete(வெற்று);
#endif

    // Add a short delay of 10 ticks to allow the idle task to free remaining memory.
    vTaskDelay(10);
    size_t after_free_8bit = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    size_t after_free_32bit = heap_caps_get_free_size(MALLOC_CAP_32BIT);
    check_leak(before_free_8bit, after_free_8bit, "8BIT");
    check_leak(before_free_32bit, after_free_32bit, "32BIT");
}

வெறுமை app_main(வெறுமை)
{

#எனில் SOC_WDT_SUPPORTED
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
    vTaskPrioritySet(வெற்று, UNITY_FREERTOS_PRIORITY);
    printf(" ______             _____ _______ ____   _____\n");
    printf("|  ____|           |  __ \\__   __/ __ \\ / ____|\n");
    printf("| |__ _ __ ___  ___| |__) | | | | |  | | (___\n");
    printf("|  __| '__/ _ \\/ _ \\  _  /  | | | |  | |\\___ \\\n");
    printf("| |  | | |  __/  __/ | \\ \\  | | | |__| |____) |\n");
    printf("|_|  |_|  \\___|\\___|_|  \\_\\ |_|  \\____/|_____/\n");
    unity_run_menu();
}
