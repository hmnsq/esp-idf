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

#تعریف_کړه TEST_حافظه_LEAK_پوله (-512)

ساكن size_t before_free_8bit;
ساكن size_t before_free_32bit;

ساكن فراغ check_leak(size_t before_free, size_t after_free, ثابت حرف *type)
{
    ssize_t delta = after_free - before_free;
    چاپ_کړه("MALLOC_CAP_%s: Before %u bytes free, After %u bytes free (delta %d)\n", type, before_free, after_free, delta);
    TEST_ASSERT_پیغام(delta >= TEST_حافظه_LEAK_پوله, "memory leak");
}

فراغ سېټ_پورته(فراغ)
{
    before_free_8bit = ډېرۍ_caps_get_free_اندازه(MALLOC_CAP_8_بټ);
    before_free_32bit = ډېرۍ_caps_get_free_اندازه(MALLOC_CAP_32_بټ);

#إذا SOC_WDT_SUPPORTED
    esp_task_wdt_جمع_کړه(عدم);
    esp_task_wdt_reset();
#د_که_پای

}

فراغ tear_ښکته(فراغ)
{
#إذا SOC_WDT_SUPPORTED
    esp_task_wdt_reset();
    esp_task_wdt_delete(عدم);
#د_که_پای

    // Add a short delay of 10 ticks to allow the idle task to free remaining memory.
    vTaskDelay(10);
    size_t after_free_8bit = ډېرۍ_caps_get_free_اندازه(MALLOC_CAP_8_بټ);
    size_t after_free_32bit = ډېرۍ_caps_get_free_اندازه(MALLOC_CAP_32_بټ);
    check_leak(before_free_8bit, after_free_8bit, "8BIT");
    check_leak(before_free_32bit, after_free_32bit, "32BIT");
}

فراغ app_main(فراغ)
{

#إذا SOC_WDT_SUPPORTED
    // Configure the task watchdog timer to catch any tests that hang
    esp_task_wdt_config_t config = {
        .timeout_ms = 25 * 1000, // 25 seconds, smaller than the default pytest timeout
        .idle_core_ماسک = 0,
        .trigger_panic = true,
    };

    esp_task_wdt_init(&config);
#د_که_پای // SOC_WDT_SUPPORTED

    /*
    Some FreeRTOS tests are reliant on the main task being at priority UNITY_FREERTOS_PRIORITY to test scheduling
    behavior. Thus, we raise the main task's priority before any tasks are run. See IDF-6088
    */
    v_Task_لومړیتوب_سېټ(عدم, UNITY_FREERTOS_لومړیتوب);
    چاپ_کړه(" ______             _____ _______ ____   _____\n");
    چاپ_کړه("|  ____|           |  __ \\__   __/ __ \\ / ____|\n");
    چاپ_کړه("| |__ _ __ ___  ___| |__) | | | | |  | | (___\n");
    چاپ_کړه("|  __| '__/ _ \\/ _ \\  _  /  | | | |  | |\\___ \\\n");
    چاپ_کړه("| |  | | |  __/  __/ | \\ \\  | | | |__| |____) |\n");
    چاپ_کړه("|_|  |_|  \\___|\\___|_|  \\_\\ |_|  \\____/|_____/\n");
    unity_پرلپسې_menu();
}
