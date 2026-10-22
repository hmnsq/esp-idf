/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#شامل "sdkconfig.h"

#اگر CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST

#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل "freertos/idf_additions.h"
#شامل "esp_log.h"

/*
 * Keep the blacklist out of the upstream TCB. A small handle table is enough
 * for this debug option and is safe to scan from the PIE exception path.
 */
ایستا TaskHandle_t فرار s_pie_blacklist[ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX ];
ایستا portMUX_TYPE s_pie_blacklist_mux = portMUX_INITIALIZER_UNLOCKED;

ایستا TaskHandle_t prvResolveTask( TaskHandle_t xTask )
{
    بازگشت ( xTask != پوچ ) ? xTask : xTaskGetCurrentTaskHandle();
}

/*
 * Reads are intentionally lock-free. xTaskGetPieBlacklisted() is called from the
 * PIE exception handler, where taking the spinlock could deadlock. A stale read
 * is acceptable for a debug aid.
 */
ایستا BaseType_t prvIsTaskPieBlacklisted( TaskHandle_t xTask )
{
    برای( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
    {
        اگر( s_pie_blacklist[ i ] == xTask )
        {
            بازگشت pdTRUE;
        }
    }

    بازگشت pdFALSE;
}

تهی vTaskSetPieBlacklisted( TaskHandle_t xTask,
                             BaseType_t xBlacklisted )
{
    TaskHandle_t xHandle = prvResolveTask( xTask );
    BaseType_t xStored = pdTRUE;

    taskENTER_CRITICAL( &s_pie_blacklist_mux );

    اگر( xBlacklisted != pdFALSE )
    {
        اگر( prvIsTaskPieBlacklisted( xHandle ) == pdFALSE )
        {
            xStored = pdFALSE;

            برای( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
            {
                اگر( s_pie_blacklist[ i ] == پوچ )
                {
                    s_pie_blacklist[ i ] = xHandle;
                    xStored = pdTRUE;
                    بشکن;
                }
            }
        }
    }
    وگرنه
    {
        برای( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
        {
            اگر( s_pie_blacklist[ i ] == xHandle )
            {
                s_pie_blacklist[ i ] = پوچ;
                بشکن;
            }
        }
    }

    taskEXIT_CRITICAL( &s_pie_blacklist_mux );

    اگر( xStored == pdFALSE )
    {
        ESP_EARLY_LOGE( "FreeRTOS", "PIE blacklist table is full (max %d)", CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX );
        configASSERT( xStored == pdTRUE );
    }
}

BaseType_t xTaskGetPieBlacklisted( TaskHandle_t xTask )
{
    بازگشت prvIsTaskPieBlacklisted( prvResolveTask( xTask ) );
}

#endif /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */
