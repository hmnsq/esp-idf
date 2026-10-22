/*
 * SPDX-FileCopyrightText: 2022-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "unity_test_runner.h"
#అన్తర్భావ "esp_heap_caps.h"
#అన్తర్భావ "test_utils.h"
#అన్తర్భావ "esp_task_wdt.h"

#నిర్వచించు TEST_జ్ఞాపకం_LEAK_పరిమితి (-512)

స్థిర size_t before_free_8bit;
స్థిర size_t before_free_32bit;

స్థిర శూన్య check_leak(size_t before_free, size_t after_free, స్థిరరాశి అక్షరం *type)
{
    ssize_t delta = after_free - before_free;
    రాయి("MALLOC_CAP_%s: Before %u bytes free, After %u bytes free (delta %d)\n", type, before_free, after_free, delta);
    TEST_ASSERT_సందేశం(delta >= TEST_జ్ఞాపకం_LEAK_పరిమితి, "memory leak");
}

శూన్య సమితి_పైకి(శూన్య)
{
    before_free_8bit = కుప్ప_caps_get_free_కొలత(MALLOC_CAP_8_బిట్టు);
    before_free_32bit = కుప్ప_caps_get_free_కొలత(MALLOC_CAP_32_బిట్టు);

#ఒకవేళ SOC_WDT_SUPPORTED
    esp_task_wdt_కూడు(ఖాళీ);
    esp_task_wdt_reset();
#అయితే_ముగింపు

}

శూన్య tear_కిందికి(శూన్య)
{
#ఒకవేళ SOC_WDT_SUPPORTED
    esp_task_wdt_reset();
    esp_task_wdt_delete(ఖాళీ);
#అయితే_ముగింపు

    // Add a short delay of 10 ticks to allow the idle task to free remaining memory.
    vTaskDelay(10);
    size_t after_free_8bit = కుప్ప_caps_get_free_కొలత(MALLOC_CAP_8_బిట్టు);
    size_t after_free_32bit = కుప్ప_caps_get_free_కొలత(MALLOC_CAP_32_బిట్టు);
    check_leak(before_free_8bit, after_free_8bit, "8BIT");
    check_leak(before_free_32bit, after_free_32bit, "32BIT");
}

శూన్య app_main(శూన్య)
{

#ఒకవేళ SOC_WDT_SUPPORTED
    // Configure the task watchdog timer to catch any tests that hang
    esp_task_wdt_config_t config = {
        .timeout_ms = 25 * 1000, // 25 seconds, smaller than the default pytest timeout
        .idle_core_ముసుగు = 0,
        .trigger_panic = true,
    };

    esp_task_wdt_init(&config);
#అయితే_ముగింపు // SOC_WDT_SUPPORTED

    /*
    Some FreeRTOS tests are reliant on the main task being at priority UNITY_FREERTOS_PRIORITY to test scheduling
    behavior. Thus, we raise the main task's priority before any tasks are run. See IDF-6088
    */
    v_Task_ప్రాధాన్యత_సమితి(ఖాళీ, UNITY_FREERTOS_ప్రాధాన్యత);
    రాయి(" ______             _____ _______ ____   _____\n");
    రాయి("|  ____|           |  __ \\__   __/ __ \\ / ____|\n");
    రాయి("| |__ _ __ ___  ___| |__) | | | | |  | | (___\n");
    రాయి("|  __| '__/ _ \\/ _ \\  _  /  | | | |  | |\\___ \\\n");
    రాయి("| |  | | |  __/  __/ | \\ \\  | | | |__| |____) |\n");
    రాయి("|_|  |_|  \\___|\\___|_|  \\_\\ |_|  \\____/|_____/\n");
    unity_నిరంతరం_menu();
}
