/*
 * SPDX-FileCopyrightText: 2022-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#અન્તર્ભાવ "freertos/FreeRTOS.h"
#અન્તર્ભાવ "freertos/task.h"
#અન્તર્ભાવ "unity.h"
#અન્તર્ભાવ "unity_test_runner.h"
#અન્તર્ભાવ "esp_heap_caps.h"
#અન્તર્ભાવ "test_utils.h"
#અન્તર્ભાવ "esp_task_wdt.h"

#define TEST_MEMORY_LEAK_THRESHOLD (-512)

સ્થિર size_t before_free_8bit;
સ્થિર size_t before_free_32bit;

સ્થિર શૂન્ય check_leak(size_t before_free, size_t after_free, અચલ અક્ષર *type)
{
    ssize_t delta = after_free - before_free;
    printf("MALLOC_CAP_%s: Before %u bytes free, After %u bytes free (delta %d)\n", type, before_free, after_free, delta);
    TEST_ASSERT_MESSAGE(delta >= TEST_MEMORY_LEAK_THRESHOLD, "memory leak");
}

શૂન્ય setUp(શૂન્ય)
{
    before_free_8bit = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    before_free_32bit = heap_caps_get_free_size(MALLOC_CAP_32BIT);

#જો SOC_WDT_SUPPORTED
    esp_task_wdt_add(ખાલી);
    esp_task_wdt_reset();
#endif

}

શૂન્ય tearDown(શૂન્ય)
{
#જો SOC_WDT_SUPPORTED
    esp_task_wdt_reset();
    esp_task_wdt_delete(ખાલી);
#endif

    // Add a short delay of 10 ticks to allow the idle task to free remaining memory.
    vTaskDelay(10);
    size_t after_free_8bit = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    size_t after_free_32bit = heap_caps_get_free_size(MALLOC_CAP_32BIT);
    check_leak(before_free_8bit, after_free_8bit, "8BIT");
    check_leak(before_free_32bit, after_free_32bit, "32BIT");
}

શૂન્ય app_main(શૂન્ય)
{

#જો SOC_WDT_SUPPORTED
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
    vTaskPrioritySet(ખાલી, UNITY_FREERTOS_PRIORITY);
    printf(" ______             _____ _______ ____   _____\n");
    printf("|  ____|           |  __ \\__   __/ __ \\ / ____|\n");
    printf("| |__ _ __ ___  ___| |__) | | | | |  | | (___\n");
    printf("|  __| '__/ _ \\/ _ \\  _  /  | | | |  | |\\___ \\\n");
    printf("| |  | | |  __/  __/ | \\ \\  | | | |__| |____) |\n");
    printf("|_|  |_|  \\___|\\___|_|  \\_\\ |_|  \\____/|_____/\n");
    unity_run_menu();
}
