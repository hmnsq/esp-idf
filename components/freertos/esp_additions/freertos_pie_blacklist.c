/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#அந்தர்பாவ "sdkconfig.h"

#எனில் CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST

#அந்தர்பாவ "freertos/FreeRTOS.h"
#அந்தர்பாவ "freertos/task.h"
#அந்தர்பாவ "freertos/idf_additions.h"
#அந்தர்பாவ "esp_log.h"

/*
 * Keep the blacklist out of the upstream TCB. A small handle table is enough
 * for this debug option and is safe to scan from the PIE exception path.
 */
நிலையான TaskHandle_t நிலையற்ற s_pie_blacklist[ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX ];
நிலையான portMUX_TYPE s_pie_blacklist_mux = portMUX_INITIALIZER_UNLOCKED;

நிலையான TaskHandle_t prvResolveTask( TaskHandle_t xTask )
{
    திரும்பு ( xTask != வெற்று ) ? xTask : xTaskGetCurrentTaskHandle();
}

/*
 * Reads are intentionally lock-free. xTaskGetPieBlacklisted() is called from the
 * PIE exception handler, where taking the spinlock could deadlock. A stale read
 * is acceptable for a debug aid.
 */
நிலையான BaseType_t prvIsTaskPieBlacklisted( TaskHandle_t xTask )
{
    க்கு( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
    {
        எனில்( s_pie_blacklist[ i ] == xTask )
        {
            திரும்பு pdTRUE;
        }
    }

    திரும்பு pdFALSE;
}

வெறுமை vTaskSetPieBlacklisted( TaskHandle_t xTask,
                             BaseType_t xBlacklisted )
{
    TaskHandle_t xHandle = prvResolveTask( xTask );
    BaseType_t xStored = pdTRUE;

    taskENTER_CRITICAL( &s_pie_blacklist_mux );

    எனில்( xBlacklisted != pdFALSE )
    {
        எனில்( prvIsTaskPieBlacklisted( xHandle ) == pdFALSE )
        {
            xStored = pdFALSE;

            க்கு( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
            {
                எனில்( s_pie_blacklist[ i ] == வெற்று )
                {
                    s_pie_blacklist[ i ] = xHandle;
                    xStored = pdTRUE;
                    நிறுத்து;
                }
            }
        }
    }
    இல்லையெனில்
    {
        க்கு( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
        {
            எனில்( s_pie_blacklist[ i ] == xHandle )
            {
                s_pie_blacklist[ i ] = வெற்று;
                நிறுத்து;
            }
        }
    }

    taskEXIT_CRITICAL( &s_pie_blacklist_mux );

    எனில்( xStored == pdFALSE )
    {
        ESP_EARLY_LOGE( "FreeRTOS", "PIE blacklist table is full (max %d)", CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX );
        configASSERT( xStored == pdTRUE );
    }
}

BaseType_t xTaskGetPieBlacklisted( TaskHandle_t xTask )
{
    திரும்பு prvIsTaskPieBlacklisted( prvResolveTask( xTask ) );
}

#endif /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */
