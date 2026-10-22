/*
 * SPDX-FileCopyrightText: 2022-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "freertos/semphr.h"
#అన్తర్భావ <stdbool.h>
#అన్తర్భావ "esp_attr.h"
#అన్తర్భావ "soc/soc_caps.h"
#ఒకవేళ SOC_CACHE_అంతర్గత_MEM_ద్వారా_L1_CACHE
#అన్తర్భావ "hal/cache_ll.h"
#అయితే_ముగింపు
#అన్తర్భావ "unity.h"
#అన్తర్భావ "test_utils.h"

/* If assertions aren't set to fail this code still crashes, but not with an abort... */
#ఒకవేళ !CONFIG_COMPILER_OPTIMIZATION_ASSERTIONS_DISABLE

/* Persists across reset so stage 2 can confirm stage 1 reached the failure path. */
స్థిర __NOINIT_ATTR bool s_restart_marker;
/* Reinitialized on reboot so stage 2 can distinguish a real restart from normal return. */
స్థిర bool s_runtime_marker;

స్థిర శూన్య mutex_release_task(శూన్య* arg)
{
    SemaphoreHandle_t mutex = (SemaphoreHandle_t) arg;
    xSemaphoreGive(mutex);
    /* The spawned task gives a mutex it does not own. Because the mutex is still held by the
       main task, the give flows through xSemaphoreGive(), xQueueGenericSend(),
       prvCopyDataToQueue(), and xTaskPriorityDisinherit(), where configASSERT(pxTCB ==
       pxCurrentTCB) fires and the panic resets the chip. */
    TEST_FAIL_సందేశం("should not be reached");
}

స్థిర శూన్య అమరిక_restart_markers(శూన్య)
{
    s_restart_marker = true;
    s_runtime_marker = true;

#ఒకవేళ SOC_CACHE_అంతర్గత_MEM_ద్వారా_L1_CACHE
    /* If internal data is behind a cache it might not be written to the physical memory when we crash
       force a full writeback here to ensure this */
    cache_ll_writeback_all(CACHE_LL_LEVEL_INT_MEM, CACHE_TYPE_దత్తాంశం, CACHE_LL_ID_ALL);
#అయితే_ముగింపు
}

స్థిర శూన్య check_system_restarted(శూన్య)
{
    /* s_restart_marker is NOINIT so it still holds the true value set in stage 1 across the
     * software reset; s_runtime_marker is BSS so it is re-initialized to false on every boot. */
    TEST_ASSERT_TRUE(s_restart_marker);
    TEST_ASSERT_FALSE(s_runtime_marker);

    s_restart_marker = false;
}

స్థిర శూన్య do_mutex_release_assert(శూన్య)
{
    అమరిక_restart_markers();

    SemaphoreHandle_t mutex = xSemaphoreCreateMutex();
    xSemaphoreTake(mutex, పోర్టు_MAX_DELAY);
    xTaskCreate(&mutex_release_task, "mutex_release", 2048, mutex, UNITY_FREERTOS_ప్రాధాన్యత + 1, ఖాళీ);
    /* Stage 1 must not return to Unity before the releaser trips the panic. */
    vTaskSuspend(ఖాళీ);
}

TEST_CASE_గుణిజం_STAGES("mutex released not by owner causes an assert", "[freertos]",
                          /* On dual-core ESP32 the panic handler can deadlock on S32C1I if the
                           * stalled core was mid-atomic, causing an RTC WDT reset (ESP_RST_WDT)
                           * instead of ESP_RST_PANIC. Stage 2 therefore verifies a real reboot
                           * via NOINIT and BSS state instead of checking the reset reason. */
                          do_mutex_release_assert,
                          check_system_restarted)

#అయితే_ముగింపు
