/*
 * SPDX-FileCopyrightText: 2022-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "unity.h"
#ضمن "unity_test_runner.h"
#ضمن "esp_heap_caps.h"
#ضمن "test_utils.h"
#ضمن "esp_task_wdt.h"

#تعریف_کرو TEST_حافظہ_LEAK_حد (-512)

ساكن size_t before_free_8bit;
ساكن size_t before_free_32bit;

ساكن فراغ check_leak(size_t before_free, size_t after_free, ثابت حرف *type)
{
    ssize_t delta = after_free - before_free;
    لکھو("MALLOC_CAP_%s: Before %u bytes free, After %u bytes free (delta %d)\n", type, before_free, after_free, delta);
    TEST_ASSERT_سنیہا(delta >= TEST_حافظہ_LEAK_حد, "memory leak");
}

فراغ سیٹ_اتے(فراغ)
{
    before_free_8bit = ڈھیر_caps_get_free_ناپ(MALLOC_CAP_8_ثنائی_ہندسہ);
    before_free_32bit = ڈھیر_caps_get_free_ناپ(MALLOC_CAP_32_ثنائی_ہندسہ);

#إذا SOC_WDT_SUPPORTED
    esp_task_wdt_جوڑو(عدم);
    esp_task_wdt_reset();
#جے_انت

}

فراغ tear_تھلے(فراغ)
{
#إذا SOC_WDT_SUPPORTED
    esp_task_wdt_reset();
    esp_task_wdt_delete(عدم);
#جے_انت

    // Add a short delay of 10 ticks to allow the idle task to free remaining memory.
    vTaskDelay(10);
    size_t after_free_8bit = ڈھیر_caps_get_free_ناپ(MALLOC_CAP_8_ثنائی_ہندسہ);
    size_t after_free_32bit = ڈھیر_caps_get_free_ناپ(MALLOC_CAP_32_ثنائی_ہندسہ);
    check_leak(before_free_8bit, after_free_8bit, "8BIT");
    check_leak(before_free_32bit, after_free_32bit, "32BIT");
}

فراغ app_main(فراغ)
{

#إذا SOC_WDT_SUPPORTED
    // Configure the task watchdog timer to catch any tests that hang
    esp_task_wdt_config_t config = {
        .timeout_ms = 25 * 1000, // 25 seconds, smaller than the default pytest timeout
        .idle_core_نقاب = 0,
        .trigger_panic = true,
    };

    esp_task_wdt_init(&config);
#جے_انت // SOC_WDT_SUPPORTED

    /*
    Some FreeRTOS tests are reliant on the main task being at priority UNITY_FREERTOS_PRIORITY to test scheduling
    behavior. Thus, we raise the main task's priority before any tasks are run. See IDF-6088
    */
    v_Task_ترجیح_سیٹ(عدم, UNITY_FREERTOS_ترجیح);
    لکھو(" ______             _____ _______ ____   _____\n");
    لکھو("|  ____|           |  __ \\__   __/ __ \\ / ____|\n");
    لکھو("| |__ _ __ ___  ___| |__) | | | | |  | | (___\n");
    لکھو("|  __| '__/ _ \\/ _ \\  _  /  | | | |  | |\\___ \\\n");
    لکھو("| |  | | |  __/  __/ | \\ \\  | | | |__| |____) |\n");
    لکھو("|_|  |_|  \\___|\\___|_|  \\_\\ |_|  \\____/|_____/\n");
    unity_لگاتار_menu();
}
