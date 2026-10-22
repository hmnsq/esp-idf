/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/semphr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_pm.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_private/esp_clk.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_clk_tree.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"

#ਜੇ CONFIG_FREERTOS_USE_TICKLESS_IDLE && CONFIG_PM_ENABLE

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MHZ (1000000)

ਸਥਿਰ SemaphoreHandle_t test_sem;
ਸਥਿਰ ਅਸਥਿਰ bool test_timeout_occurred = false;

ਸਥਿਰ ਖਾਲੀ ਉਤਪਾਦਕ_task(ਖਾਲੀ *arg)
{
    ਜਦੋਂ (1) {
        xSemaphoreGive(test_sem);
        vTaskDelay(pd_MS_ਮੰਜ਼ਿਲ_TICKS(5));  // Give semaphore every 5ms
    }
}

ਸਥਿਰ ਖਾਲੀ ਖਪਤਕਾਰ_task(ਖਾਲੀ *arg)
{
    ਜਦੋਂ (1) {
        // Try to take semaphore with 1 second timeout
        // Should never timeout as producer is giving the semaphore every 5ms
        ਜੇ (xSemaphoreTake(test_sem, pd_MS_ਮੰਜ਼ਿਲ_TICKS(1000)) == pdFALSE) {
            test_timeout_occurred = true;  // Mark that timeout occurred
            ਰੋਕੋ;
        }
    }

    vTaskSuspend(ਨਿਰਾ);
}

/**
 * Test case to verify timing accuracy during tickless idle mode
 *
 * This test verifies that FreeRTOS maintains accurate timing when the system
 * enters and exits tickless idle mode, particularly when combined with scheduler
 * suspension operations.
 *
 * Test Setup:
 * - Configures the system to use tickless idle mode
 * - Creates a producer-consumer pair using a semaphore:
 *   * Producer gives semaphore every 5ms
 *   * Consumer takes semaphore with 1-second timeout
 * - Periodically suspend and resume the scheduler
 *
 * Expected Behavior:
 * - The consumer should never timeout as the producer gives the semaphore
 *   every 5ms, well within the 1-second timeout window
 * - Any timing inaccuracies during tickless idle would manifest as
 *   unexpected semaphore timeouts
 * - The test passes if no timeouts occur.
 */
TEST_CASE("Test semaphore timeout during tickless idle", "[freertos]")
{
    // Configure tickless idle
    uint32_t xtal_hz = 0;
    esp_clk_ਰੁੱਖ_src_get_freq_hz(SOC_MOD_CLK_XTAL, ESP_CLK_ਰੁੱਖ_SRC_FREQ_PRECISION_EXACT, &xtal_hz);
    esp_pm_config_t pm_config = {
        .max_freq_mhz = CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ,
        .min_freq_mhz = xtal_hz / MHZ,
        .light_sleep_enable = true,
    };
    TEST_ESP_OK(esp_pm_configure(&pm_config));

    // Create test semaphore
    test_sem = x_Semaphore_Create_ਦੁਆਧਾਰੀ();
    TEST_ASSERT_NOT_NULL(test_sem);

    test_timeout_occurred = false;

    // Create producer and consumer tasks
    TaskHandle_t ਉਤਪਾਦਕ_handle, ਖਪਤਕਾਰ_handle;
    TEST_ASSERT_EQUAL(pdPASS, xTaskCreate(ਉਤਪਾਦਕ_task, "producer", 2048, ਨਿਰਾ, 1, &ਉਤਪਾਦਕ_handle));
    TEST_ASSERT_EQUAL(pdPASS, xTaskCreate(ਖਪਤਕਾਰ_task, "consumer", 2048, ਨਿਰਾ, 2, &ਖਪਤਕਾਰ_handle));

    // Run the test for a while to ensure tickless idle mode has
    // enough time to enter and exit multiple times
    vTaskDelay(pd_MS_ਮੰਜ਼ਿਲ_TICKS(10000));

    // Suspend scheduler briefly
    vTaskSuspendAll();

    // Resume scheduler. This unrolls pended tick events.
    // This should not cause any unexpected timeouts if
    // tickless idle mode is working correctly.
    xTaskResumeAll();

    // Verify no timeout occurred
    TEST_ASSERT_FALSE(test_timeout_occurred);

    // Disable power management before exit to prevent memory leaks
    esp_pm_config_t default_config = {
        .max_freq_mhz = CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ,
        .min_freq_mhz = CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ,
        .light_sleep_enable = false,
    };
    esp_pm_configure(&default_config);

    // Cleanup
    vTaskDelete(ਉਤਪਾਦਕ_handle);
    vTaskDelete(ਖਪਤਕਾਰ_handle);
    vSemaphoreDelete(test_sem);
}

#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_USE_TICKLESS_IDLE && CONFIG_PM_ENABLE */
