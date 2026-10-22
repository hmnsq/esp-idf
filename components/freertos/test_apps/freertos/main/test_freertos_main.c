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

#ವ್ಯಾಖ್ಯಾನಿಸು TEST_ಸ್ಮರಣೆ_LEAK_ಮಿತಿ (-512)

ಸ್ಥಿರ size_t before_free_8bit;
ಸ್ಥಿರ size_t before_free_32bit;

ಸ್ಥಿರ ಶೂನ್ಯ check_leak(size_t before_free, size_t after_free, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *type)
{
    ssize_t delta = after_free - before_free;
    ಬರೆ("MALLOC_CAP_%s: Before %u bytes free, After %u bytes free (delta %d)\n", type, before_free, after_free, delta);
    TEST_ASSERT_ಸಂದೇಶ(delta >= TEST_ಸ್ಮರಣೆ_LEAK_ಮಿತಿ, "memory leak");
}

ಶೂನ್ಯ ಗಣ_ಮೇಲೆ(ಶೂನ್ಯ)
{
    before_free_8bit = ರಾಶಿ_caps_get_free_ಗಾತ್ರ(MALLOC_CAP_8_ಬಿಟ್);
    before_free_32bit = ರಾಶಿ_caps_get_free_ಗಾತ್ರ(MALLOC_CAP_32_ಬಿಟ್);

#ಆದರೆ SOC_WDT_SUPPORTED
    esp_task_wdt_ಕೂಡಿಸು(ಖಾಲಿ);
    esp_task_wdt_reset();
#ಆದರೆ_ಅಂತ್ಯ

}

ಶೂನ್ಯ tear_ಕೆಳಕ್ಕೆ(ಶೂನ್ಯ)
{
#ಆದರೆ SOC_WDT_SUPPORTED
    esp_task_wdt_reset();
    esp_task_wdt_delete(ಖಾಲಿ);
#ಆದರೆ_ಅಂತ್ಯ

    // Add a short delay of 10 ticks to allow the idle task to free remaining memory.
    vTaskDelay(10);
    size_t after_free_8bit = ರಾಶಿ_caps_get_free_ಗಾತ್ರ(MALLOC_CAP_8_ಬಿಟ್);
    size_t after_free_32bit = ರಾಶಿ_caps_get_free_ಗಾತ್ರ(MALLOC_CAP_32_ಬಿಟ್);
    check_leak(before_free_8bit, after_free_8bit, "8BIT");
    check_leak(before_free_32bit, after_free_32bit, "32BIT");
}

ಶೂನ್ಯ app_main(ಶೂನ್ಯ)
{

#ಆದರೆ SOC_WDT_SUPPORTED
    // Configure the task watchdog timer to catch any tests that hang
    esp_task_wdt_config_t config = {
        .timeout_ms = 25 * 1000, // 25 seconds, smaller than the default pytest timeout
        .idle_core_ಮುಖವಾಡ = 0,
        .trigger_panic = true,
    };

    esp_task_wdt_init(&config);
#ಆದರೆ_ಅಂತ್ಯ // SOC_WDT_SUPPORTED

    /*
    Some FreeRTOS tests are reliant on the main task being at priority UNITY_FREERTOS_PRIORITY to test scheduling
    behavior. Thus, we raise the main task's priority before any tasks are run. See IDF-6088
    */
    v_Task_ಆದ್ಯತೆ_ಗಣ(ಖಾಲಿ, UNITY_FREERTOS_ಆದ್ಯತೆ);
    ಬರೆ(" ______             _____ _______ ____   _____\n");
    ಬರೆ("|  ____|           |  __ \\__   __/ __ \\ / ____|\n");
    ಬರೆ("| |__ _ __ ___  ___| |__) | | | | |  | | (___\n");
    ಬರೆ("|  __| '__/ _ \\/ _ \\  _  /  | | | |  | |\\___ \\\n");
    ಬರೆ("| |  | | |  __/  __/ | \\ \\  | | | |__| |____) |\n");
    ಬರೆ("|_|  |_|  \\___|\\___|_|  \\_\\ |_|  \\____/|_____/\n");
    unity_ಸತತ_menu();
}
