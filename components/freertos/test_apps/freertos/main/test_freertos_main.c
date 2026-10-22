/*
 * SPDX-FileCopyrightText: 2022-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل "unity.h"
#شامل "unity_test_runner.h"
#شامل "esp_heap_caps.h"
#شامل "test_utils.h"
#شامل "esp_task_wdt.h"

#تعریف_کرو TEST_حافظہ_LEAK_حد (-512)

ساکن size_t before_free_8bit;
ساکن size_t before_free_32bit;

ساکن خالی check_leak(size_t before_free, size_t after_free, ثابت حرف *type)
{
    ssize_t delta = after_free - before_free;
    لکھو("MALLOC_CAP_%s: Before %u bytes free, After %u bytes free (delta %d)\n", type, before_free, after_free, delta);
    TEST_ASSERT_پیغام(delta >= TEST_حافظہ_LEAK_حد, "memory leak");
}

خالی سیٹ_اوپر(خالی)
{
    before_free_8bit = ڈھیر_caps_get_free_سائز(MALLOC_CAP_8_ثنائی_ہندسہ);
    before_free_32bit = ڈھیر_caps_get_free_سائز(MALLOC_CAP_32_ثنائی_ہندسہ);

#اگر SOC_WDT_SUPPORTED
    esp_task_wdt_جوڑو(تہی);
    esp_task_wdt_reset();
#اگر_ختم

}

خالی tear_نیچے_کو(خالی)
{
#اگر SOC_WDT_SUPPORTED
    esp_task_wdt_reset();
    esp_task_wdt_delete(تہی);
#اگر_ختم

    // Add a short delay of 10 ticks to allow the idle task to free remaining memory.
    vTaskDelay(10);
    size_t after_free_8bit = ڈھیر_caps_get_free_سائز(MALLOC_CAP_8_ثنائی_ہندسہ);
    size_t after_free_32bit = ڈھیر_caps_get_free_سائز(MALLOC_CAP_32_ثنائی_ہندسہ);
    check_leak(before_free_8bit, after_free_8bit, "8BIT");
    check_leak(before_free_32bit, after_free_32bit, "32BIT");
}

خالی app_main(خالی)
{

#اگر SOC_WDT_SUPPORTED
    // Configure the task watchdog timer to catch any tests that hang
    esp_task_wdt_config_t config = {
        .timeout_ms = 25 * 1000, // 25 seconds, smaller than the default pytest timeout
        .idle_core_نقاب = 0,
        .trigger_panic = true,
    };

    esp_task_wdt_init(&config);
#اگر_ختم // SOC_WDT_SUPPORTED

    /*
    Some FreeRTOS tests are reliant on the main task being at priority UNITY_FREERTOS_PRIORITY to test scheduling
    behavior. Thus, we raise the main task's priority before any tasks are run. See IDF-6088
    */
    v_Task_ترجیح_سیٹ(تہی, UNITY_FREERTOS_ترجیح);
    لکھو(" ______             _____ _______ ____   _____\n");
    لکھو("|  ____|           |  __ \\__   __/ __ \\ / ____|\n");
    لکھو("| |__ _ __ ___  ___| |__) | | | | |  | | (___\n");
    لکھو("|  __| '__/ _ \\/ _ \\  _  /  | | | |  | |\\___ \\\n");
    لکھو("| |  | | |  __/  __/ | \\ \\  | | | |__| |____) |\n");
    لکھو("|_|  |_|  \\___|\\___|_|  \\_\\ |_|  \\____/|_____/\n");
    unity_تسلسل_menu();
}
