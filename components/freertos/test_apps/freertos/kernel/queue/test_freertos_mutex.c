/*
 * SPDX-FileCopyrightText: 2022-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/semphr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_attr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "soc/soc_caps.h"
#ਜੇ SOC_CACHE_ਅੰਦਰੂਨੀ_MEM_ਰਾਹੀਂ_L1_CACHE
#ਅਨ੍ਤਰ੍ਭਾਵ "hal/cache_ll.h"
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "test_utils.h"

/* If assertions aren't set to fail this code still crashes, but not with an abort... */
#ਜੇ !CONFIG_COMPILER_OPTIMIZATION_ASSERTIONS_DISABLE

/* Persists across reset so stage 2 can confirm stage 1 reached the failure path. */
ਸਥਿਰ __NOINIT_ATTR bool s_restart_marker;
/* Reinitialized on reboot so stage 2 can distinguish a real restart from normal return. */
ਸਥਿਰ bool s_runtime_marker;

ਸਥਿਰ ਖਾਲੀ mutex_release_task(ਖਾਲੀ* arg)
{
    SemaphoreHandle_t mutex = (SemaphoreHandle_t) arg;
    xSemaphoreGive(mutex);
    /* The spawned task gives a mutex it does not own. Because the mutex is still held by the
       main task, the give flows through xSemaphoreGive(), xQueueGenericSend(),
       prvCopyDataToQueue(), and xTaskPriorityDisinherit(), where configASSERT(pxTCB ==
       pxCurrentTCB) fires and the panic resets the chip. */
    TEST_FAIL_ਸੁਨੇਹਾ("should not be reached");
}

ਸਥਿਰ ਖਾਲੀ ਤਿਆਰੀ_restart_markers(ਖਾਲੀ)
{
    s_restart_marker = true;
    s_runtime_marker = true;

#ਜੇ SOC_CACHE_ਅੰਦਰੂਨੀ_MEM_ਰਾਹੀਂ_L1_CACHE
    /* If internal data is behind a cache it might not be written to the physical memory when we crash
       force a full writeback here to ensure this */
    cache_ll_writeback_all(CACHE_LL_LEVEL_INT_MEM, CACHE_TYPE_ਅੰਕੜੇ, CACHE_LL_ID_ALL);
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਖਾਲੀ check_system_restarted(ਖਾਲੀ)
{
    /* s_restart_marker is NOINIT so it still holds the true value set in stage 1 across the
     * software reset; s_runtime_marker is BSS so it is re-initialized to false on every boot. */
    TEST_ASSERT_TRUE(s_restart_marker);
    TEST_ASSERT_FALSE(s_runtime_marker);

    s_restart_marker = false;
}

ਸਥਿਰ ਖਾਲੀ do_mutex_release_assert(ਖਾਲੀ)
{
    ਤਿਆਰੀ_restart_markers();

    SemaphoreHandle_t mutex = xSemaphoreCreateMutex();
    xSemaphoreTake(mutex, ਪੋਰਟ_MAX_DELAY);
    xTaskCreate(&mutex_release_task, "mutex_release", 2048, mutex, UNITY_FREERTOS_ਤਰਜੀਹ + 1, ਨਿਰਾ);
    /* Stage 1 must not return to Unity before the releaser trips the panic. */
    vTaskSuspend(ਨਿਰਾ);
}

TEST_CASE_ਗੁਣਜ_STAGES("mutex released not by owner causes an assert", "[freertos]",
                          /* On dual-core ESP32 the panic handler can deadlock on S32C1I if the
                           * stalled core was mid-atomic, causing an RTC WDT reset (ESP_RST_WDT)
                           * instead of ESP_RST_PANIC. Stage 2 therefore verifies a real reboot
                           * via NOINIT and BSS state instead of checking the reset reason. */
                          do_mutex_release_assert,
                          check_system_restarted)

#ਜੇ_ਅੰਤ
