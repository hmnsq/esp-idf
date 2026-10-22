/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#અન્તર્ભાવ "sdkconfig.h"

#જો CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST

#અન્તર્ભાવ "freertos/FreeRTOS.h"
#અન્તર્ભાવ "freertos/task.h"
#અન્તર્ભાવ "freertos/idf_additions.h"
#અન્તર્ભાવ "esp_log.h"

/*
 * Keep the blacklist out of the upstream TCB. A small handle table is enough
 * for this debug option and is safe to scan from the PIE exception path.
 */
સ્થિર TaskHandle_t અસ્થિર s_pie_blacklist[ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX ];
સ્થિર portMUX_TYPE s_pie_blacklist_mux = portMUX_INITIALIZER_UNLOCKED;

સ્થિર TaskHandle_t prvResolveTask( TaskHandle_t xTask )
{
    પાછા ( xTask != ખાલી ) ? xTask : xTaskGetCurrentTaskHandle();
}

/*
 * Reads are intentionally lock-free. xTaskGetPieBlacklisted() is called from the
 * PIE exception handler, where taking the spinlock could deadlock. A stale read
 * is acceptable for a debug aid.
 */
સ્થિર BaseType_t prvIsTaskPieBlacklisted( TaskHandle_t xTask )
{
    માટે( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
    {
        જો( s_pie_blacklist[ i ] == xTask )
        {
            પાછા pdTRUE;
        }
    }

    પાછા pdFALSE;
}

શૂન્ય vTaskSetPieBlacklisted( TaskHandle_t xTask,
                             BaseType_t xBlacklisted )
{
    TaskHandle_t xHandle = prvResolveTask( xTask );
    BaseType_t xStored = pdTRUE;

    taskENTER_CRITICAL( &s_pie_blacklist_mux );

    જો( xBlacklisted != pdFALSE )
    {
        જો( prvIsTaskPieBlacklisted( xHandle ) == pdFALSE )
        {
            xStored = pdFALSE;

            માટે( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
            {
                જો( s_pie_blacklist[ i ] == ખાલી )
                {
                    s_pie_blacklist[ i ] = xHandle;
                    xStored = pdTRUE;
                    તોડો;
                }
            }
        }
    }
    નહીંતર
    {
        માટે( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
        {
            જો( s_pie_blacklist[ i ] == xHandle )
            {
                s_pie_blacklist[ i ] = ખાલી;
                તોડો;
            }
        }
    }

    taskEXIT_CRITICAL( &s_pie_blacklist_mux );

    જો( xStored == pdFALSE )
    {
        ESP_EARLY_LOGE( "FreeRTOS", "PIE blacklist table is full (max %d)", CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX );
        configASSERT( xStored == pdTRUE );
    }
}

BaseType_t xTaskGetPieBlacklisted( TaskHandle_t xTask )
{
    પાછા prvIsTaskPieBlacklisted( prvResolveTask( xTask ) );
}

#endif /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */
