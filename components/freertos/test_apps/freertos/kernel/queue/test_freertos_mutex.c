/*
 * SPDX-FileCopyrightText: 2022-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "freertos/semphr.h"
#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ "esp_attr.h"
#ಅನ್ತರ್ಭಾವ "soc/soc_caps.h"
#ಆದರೆ SOC_CACHE_ಆಂತರಿಕ_MEM_ಮೂಲಕ_L1_CACHE
#ಅನ್ತರ್ಭಾವ "hal/cache_ll.h"
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "test_utils.h"

/* If assertions aren't set to fail this code still crashes, but not with an abort... */
#ಆದರೆ !CONFIG_COMPILER_OPTIMIZATION_ASSERTIONS_DISABLE

/* Persists across reset so stage 2 can confirm stage 1 reached the failure path. */
ಸ್ಥಿರ __NOINIT_ATTR bool s_restart_marker;
/* Reinitialized on reboot so stage 2 can distinguish a real restart from normal return. */
ಸ್ಥಿರ bool s_runtime_marker;

ಸ್ಥಿರ ಶೂನ್ಯ mutex_release_task(ಶೂನ್ಯ* arg)
{
    SemaphoreHandle_t mutex = (SemaphoreHandle_t) arg;
    xSemaphoreGive(mutex);
    /* The spawned task gives a mutex it does not own. Because the mutex is still held by the
       main task, the give flows through xSemaphoreGive(), xQueueGenericSend(),
       prvCopyDataToQueue(), and xTaskPriorityDisinherit(), where configASSERT(pxTCB ==
       pxCurrentTCB) fires and the panic resets the chip. */
    TEST_FAIL_ಸಂದೇಶ("should not be reached");
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸಿದ್ಧತೆ_restart_markers(ಶೂನ್ಯ)
{
    s_restart_marker = true;
    s_runtime_marker = true;

#ಆದರೆ SOC_CACHE_ಆಂತರಿಕ_MEM_ಮೂಲಕ_L1_CACHE
    /* If internal data is behind a cache it might not be written to the physical memory when we crash
       force a full writeback here to ensure this */
    cache_ll_writeback_all(CACHE_LL_LEVEL_INT_MEM, CACHE_TYPE_ದತ್ತಾಂಶ, CACHE_LL_ID_ALL);
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಶೂನ್ಯ check_system_restarted(ಶೂನ್ಯ)
{
    /* s_restart_marker is NOINIT so it still holds the true value set in stage 1 across the
     * software reset; s_runtime_marker is BSS so it is re-initialized to false on every boot. */
    TEST_ASSERT_TRUE(s_restart_marker);
    TEST_ASSERT_FALSE(s_runtime_marker);

    s_restart_marker = false;
}

ಸ್ಥಿರ ಶೂನ್ಯ do_mutex_release_assert(ಶೂನ್ಯ)
{
    ಸಿದ್ಧತೆ_restart_markers();

    SemaphoreHandle_t mutex = xSemaphoreCreateMutex();
    xSemaphoreTake(mutex, ಪೋರ್ಟ್_MAX_DELAY);
    xTaskCreate(&mutex_release_task, "mutex_release", 2048, mutex, UNITY_FREERTOS_ಆದ್ಯತೆ + 1, ಖಾಲಿ);
    /* Stage 1 must not return to Unity before the releaser trips the panic. */
    vTaskSuspend(ಖಾಲಿ);
}

TEST_CASE_ಅಪವರ್ತ್ಯ_STAGES("mutex released not by owner causes an assert", "[freertos]",
                          /* On dual-core ESP32 the panic handler can deadlock on S32C1I if the
                           * stalled core was mid-atomic, causing an RTC WDT reset (ESP_RST_WDT)
                           * instead of ESP_RST_PANIC. Stage 2 therefore verifies a real reboot
                           * via NOINIT and BSS state instead of checking the reset reason. */
                          do_mutex_release_assert,
                          check_system_restarted)

#ಆದರೆ_ಅಂತ್ಯ
