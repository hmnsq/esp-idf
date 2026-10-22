/*
 * SPDX-FileCopyrightText: 2022-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity_test_runner.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_heap_caps.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "test_utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_task_wdt.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_ਸਮ੍ਰਿਤੀ_LEAK_ਹੱਦ (-512)

ਸਥਿਰ size_t before_free_8bit;
ਸਥਿਰ size_t before_free_32bit;

ਸਥਿਰ ਖਾਲੀ check_leak(size_t before_free, size_t after_free, ਅਚਲ ਅੱਖਰ *type)
{
    ssize_t delta = after_free - before_free;
    ਲਿਖੋ("MALLOC_CAP_%s: Before %u bytes free, After %u bytes free (delta %d)\n", type, before_free, after_free, delta);
    TEST_ASSERT_ਸੁਨੇਹਾ(delta >= TEST_ਸਮ੍ਰਿਤੀ_LEAK_ਹੱਦ, "memory leak");
}

ਖਾਲੀ ਸੈੱਟ_ਉੱਪਰ(ਖਾਲੀ)
{
    before_free_8bit = ਢੇਰ_caps_get_free_ਮਾਪ(MALLOC_CAP_8_ਦੋਆਧਾਰੀ_ਅੰਕ);
    before_free_32bit = ਢੇਰ_caps_get_free_ਮਾਪ(MALLOC_CAP_32_ਦੋਆਧਾਰੀ_ਅੰਕ);

#ਜੇ SOC_WDT_SUPPORTED
    esp_task_wdt_ਜੋੜੋ(ਨਿਰਾ);
    esp_task_wdt_reset();
#ਜੇ_ਅੰਤ

}

ਖਾਲੀ tear_ਥੱਲੇ(ਖਾਲੀ)
{
#ਜੇ SOC_WDT_SUPPORTED
    esp_task_wdt_reset();
    esp_task_wdt_delete(ਨਿਰਾ);
#ਜੇ_ਅੰਤ

    // Add a short delay of 10 ticks to allow the idle task to free remaining memory.
    vTaskDelay(10);
    size_t after_free_8bit = ਢੇਰ_caps_get_free_ਮਾਪ(MALLOC_CAP_8_ਦੋਆਧਾਰੀ_ਅੰਕ);
    size_t after_free_32bit = ਢੇਰ_caps_get_free_ਮਾਪ(MALLOC_CAP_32_ਦੋਆਧਾਰੀ_ਅੰਕ);
    check_leak(before_free_8bit, after_free_8bit, "8BIT");
    check_leak(before_free_32bit, after_free_32bit, "32BIT");
}

ਖਾਲੀ app_main(ਖਾਲੀ)
{

#ਜੇ SOC_WDT_SUPPORTED
    // Configure the task watchdog timer to catch any tests that hang
    esp_task_wdt_config_t config = {
        .timeout_ms = 25 * 1000, // 25 seconds, smaller than the default pytest timeout
        .idle_core_ਨਕਾਬ = 0,
        .trigger_panic = true,
    };

    esp_task_wdt_init(&config);
#ਜੇ_ਅੰਤ // SOC_WDT_SUPPORTED

    /*
    Some FreeRTOS tests are reliant on the main task being at priority UNITY_FREERTOS_PRIORITY to test scheduling
    behavior. Thus, we raise the main task's priority before any tasks are run. See IDF-6088
    */
    v_Task_ਤਰਜੀਹ_ਸੈੱਟ(ਨਿਰਾ, UNITY_FREERTOS_ਤਰਜੀਹ);
    ਲਿਖੋ(" ______             _____ _______ ____   _____\n");
    ਲਿਖੋ("|  ____|           |  __ \\__   __/ __ \\ / ____|\n");
    ਲਿਖੋ("| |__ _ __ ___  ___| |__) | | | | |  | | (___\n");
    ਲਿਖੋ("|  __| '__/ _ \\/ _ \\  _  /  | | | |  | |\\___ \\\n");
    ਲਿਖੋ("| |  | | |  __/  __/ | \\ \\  | | | |__| |____) |\n");
    ਲਿਖੋ("|_|  |_|  \\___|\\___|_|  \\_\\ |_|  \\____/|_____/\n");
    unity_ਲੜੀਵਾਰ_menu();
}
