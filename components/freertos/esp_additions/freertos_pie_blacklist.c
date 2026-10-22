/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"

#ਜੇ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST

#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/idf_additions.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_log.h"

/*
 * Keep the blacklist out of the upstream TCB. A small handle table is enough
 * for this debug option and is safe to scan from the PIE exception path.
 */
ਸਥਿਰ TaskHandle_t ਅਸਥਿਰ s_pie_blacklist[ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX ];
ਸਥਿਰ ਪੋਰਟ_MUX_TYPE s_pie_blacklist_mux = ਪੋਰਟ_MUX_INITIALIZER_UNLOCKED;

ਸਥਿਰ TaskHandle_t prvResolveTask( TaskHandle_t xTask )
{
    ਵਾਪਸ ( xTask != ਨਿਰਾ ) ? xTask : xTaskGetCurrentTaskHandle();
}

/*
 * Reads are intentionally lock-free. xTaskGetPieBlacklisted() is called from the
 * PIE exception handler, where taking the spinlock could deadlock. A stale read
 * is acceptable for a debug aid.
 */
ਸਥਿਰ ਅਧਾਰ_Type_t prvIsTaskPieBlacklisted( TaskHandle_t xTask )
{
    ਲਈ( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
    {
        ਜੇ( s_pie_blacklist[ i ] == xTask )
        {
            ਵਾਪਸ pdTRUE;
        }
    }

    ਵਾਪਸ pdFALSE;
}

ਖਾਲੀ v_Task_ਸੈੱਟ_Pie_Blacklisted( TaskHandle_t xTask,
                             ਅਧਾਰ_Type_t xBlacklisted )
{
    TaskHandle_t xHandle = prvResolveTask( xTask );
    ਅਧਾਰ_Type_t xStored = pdTRUE;

    taskENTER_CRITICAL( &s_pie_blacklist_mux );

    ਜੇ( xBlacklisted != pdFALSE )
    {
        ਜੇ( prvIsTaskPieBlacklisted( xHandle ) == pdFALSE )
        {
            xStored = pdFALSE;

            ਲਈ( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
            {
                ਜੇ( s_pie_blacklist[ i ] == ਨਿਰਾ )
                {
                    s_pie_blacklist[ i ] = xHandle;
                    xStored = pdTRUE;
                    ਰੋਕੋ;
                }
            }
        }
    }
    ਨਹੀਂਤਾਂ
    {
        ਲਈ( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
        {
            ਜੇ( s_pie_blacklist[ i ] == xHandle )
            {
                s_pie_blacklist[ i ] = ਨਿਰਾ;
                ਰੋਕੋ;
            }
        }
    }

    taskEXIT_CRITICAL( &s_pie_blacklist_mux );

    ਜੇ( xStored == pdFALSE )
    {
        ESP_EARLY_LOGE( "FreeRTOS", "PIE blacklist table is full (max %d)", CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX );
        configASSERT( xStored == pdTRUE );
    }
}

ਅਧਾਰ_Type_t xTaskGetPieBlacklisted( TaskHandle_t xTask )
{
    ਵਾਪਸ prvIsTaskPieBlacklisted( prvResolveTask( xTask ) );
}

#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */
