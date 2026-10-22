/*
 * SPDX-FileCopyrightText: 2022-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "unity_test_runner.h"
#ಅನ್ತರ್ಭಾವ "esp_heap_caps.h"
#ಅನ್ತರ್ಭಾವ "test_utils.h"
#ಅನ್ತರ್ಭಾವ "esp_task_wdt.h"

#define TEST_MEMORY_LEAK_THRESHOLD (-512)

ಸ್ಥಿರ size_t before_free_8bit;
ಸ್ಥಿರ size_t before_free_32bit;

ಸ್ಥಿರ ಶೂನ್ಯ check_leak(size_t before_free, size_t after_free, ಧ್ರುವ ವರ್ಣ *type)
{
    ssize_t delta = after_free - before_free;
    printf("MALLOC_CAP_%s: Before %u bytes free, After %u bytes free (delta %d)\n", type, before_free, after_free, delta);
    TEST_ASSERT_MESSAGE(delta >= TEST_MEMORY_LEAK_THRESHOLD, "memory leak");
}

ಶೂನ್ಯ setUp(ಶೂನ್ಯ)
{
    before_free_8bit = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    before_free_32bit = heap_caps_get_free_size(MALLOC_CAP_32BIT);

#ಯದಿ SOC_WDT_SUPPORTED
    esp_task_wdt_add(ರಿಕ್ತ);
    esp_task_wdt_reset();
#endif

}

ಶೂನ್ಯ tearDown(ಶೂನ್ಯ)
{
#ಯದಿ SOC_WDT_SUPPORTED
    esp_task_wdt_reset();
    esp_task_wdt_delete(ರಿಕ್ತ);
#endif

    // Add a short delay of 10 ticks to allow the idle task to free remaining memory.
    vTaskDelay(10);
    size_t after_free_8bit = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    size_t after_free_32bit = heap_caps_get_free_size(MALLOC_CAP_32BIT);
    check_leak(before_free_8bit, after_free_8bit, "8BIT");
    check_leak(before_free_32bit, after_free_32bit, "32BIT");
}

ಶೂನ್ಯ app_main(ಶೂನ್ಯ)
{

#ಯದಿ SOC_WDT_SUPPORTED
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
    vTaskPrioritySet(ರಿಕ್ತ, UNITY_FREERTOS_PRIORITY);
    printf(" ______             _____ _______ ____   _____\n");
    printf("|  ____|           |  __ \\__   __/ __ \\ / ____|\n");
    printf("| |__ _ __ ___  ___| |__) | | | | |  | | (___\n");
    printf("|  __| '__/ _ \\/ _ \\  _  /  | | | |  | |\\___ \\\n");
    printf("| |  | | |  __/  __/ | \\ \\  | | | |__| |____) |\n");
    printf("|_|  |_|  \\___|\\___|_|  \\_\\ |_|  \\____/|_____/\n");
    unity_run_menu();
}
