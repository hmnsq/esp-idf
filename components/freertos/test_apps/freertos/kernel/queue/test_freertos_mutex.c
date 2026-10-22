/*
 * SPDX-FileCopyrightText: 2022-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#অন্তর্ভাৱ "freertos/FreeRTOS.h"
#অন্তর্ভাৱ "freertos/task.h"
#অন্তর্ভাৱ "freertos/semphr.h"
#অন্তর্ভাৱ <stdbool.h>
#অন্তর্ভাৱ "esp_attr.h"
#অন্তর্ভাৱ "soc/soc_caps.h"
#যদি SOC_CACHE_INTERNAL_MEM_VIA_L1CACHE
#অন্তর্ভাৱ "hal/cache_ll.h"
#endif
#অন্তর্ভাৱ "unity.h"
#অন্তর্ভাৱ "test_utils.h"

/* If assertions aren't set to fail this code still crashes, but not with an abort... */
#যদি !CONFIG_COMPILER_OPTIMIZATION_ASSERTIONS_DISABLE

/* Persists across reset so stage 2 can confirm stage 1 reached the failure path. */
স্থির __NOINIT_ATTR bool s_restart_marker;
/* Reinitialized on reboot so stage 2 can distinguish a real restart from normal return. */
স্থির bool s_runtime_marker;

স্থির শূন্য mutex_release_task(শূন্য* arg)
{
    SemaphoreHandle_t mutex = (SemaphoreHandle_t) arg;
    xSemaphoreGive(mutex);
    /* The spawned task gives a mutex it does not own. Because the mutex is still held by the
       main task, the give flows through xSemaphoreGive(), xQueueGenericSend(),
       prvCopyDataToQueue(), and xTaskPriorityDisinherit(), where configASSERT(pxTCB ==
       pxCurrentTCB) fires and the panic resets the chip. */
    TEST_FAIL_MESSAGE("should not be reached");
}

স্থির শূন্য setup_restart_markers(শূন্য)
{
    s_restart_marker = true;
    s_runtime_marker = true;

#যদি SOC_CACHE_INTERNAL_MEM_VIA_L1CACHE
    /* If internal data is behind a cache it might not be written to the physical memory when we crash
       force a full writeback here to ensure this */
    cache_ll_writeback_all(CACHE_LL_LEVEL_INT_MEM, CACHE_TYPE_DATA, CACHE_LL_ID_ALL);
#endif
}

স্থির শূন্য check_system_restarted(শূন্য)
{
    /* s_restart_marker is NOINIT so it still holds the true value set in stage 1 across the
     * software reset; s_runtime_marker is BSS so it is re-initialized to false on every boot. */
    TEST_ASSERT_TRUE(s_restart_marker);
    TEST_ASSERT_FALSE(s_runtime_marker);

    s_restart_marker = false;
}

স্থির শূন্য do_mutex_release_assert(শূন্য)
{
    setup_restart_markers();

    SemaphoreHandle_t mutex = xSemaphoreCreateMutex();
    xSemaphoreTake(mutex, portMAX_DELAY);
    xTaskCreate(&mutex_release_task, "mutex_release", 2048, mutex, UNITY_FREERTOS_PRIORITY + 1, রিক্ত);
    /* Stage 1 must not return to Unity before the releaser trips the panic. */
    vTaskSuspend(রিক্ত);
}

TEST_CASE_MULTIPLE_STAGES("mutex released not by owner causes an assert", "[freertos]",
                          /* On dual-core ESP32 the panic handler can deadlock on S32C1I if the
                           * stalled core was mid-atomic, causing an RTC WDT reset (ESP_RST_WDT)
                           * instead of ESP_RST_PANIC. Stage 2 therefore verifies a real reboot
                           * via NOINIT and BSS state instead of checking the reset reason. */
                          do_mutex_release_assert,
                          check_system_restarted)

#endif
