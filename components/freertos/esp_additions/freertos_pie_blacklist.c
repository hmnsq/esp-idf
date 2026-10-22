/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#अन्तर्भाव "sdkconfig.h"

#सचे CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST

#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"
#अन्तर्भाव "freertos/idf_additions.h"
#अन्तर्भाव "esp_log.h"

/*
 * Keep the blacklist out of the upstream TCB. A small handle table is enough
 * for this debug option and is safe to scan from the PIE exception path.
 */
स्थिर TaskHandle_t चञ्चल s_pie_blacklist[ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX ];
स्थिर portMUX_TYPE s_pie_blacklist_mux = portMUX_INITIALIZER_UNLOCKED;

स्थिर TaskHandle_t prvResolveTask( TaskHandle_t xTask )
{
    पटिनिवत्त ( xTask != रिक्त ) ? xTask : xTaskGetCurrentTaskHandle();
}

/*
 * Reads are intentionally lock-free. xTaskGetPieBlacklisted() is called from the
 * PIE exception handler, where taking the spinlock could deadlock. A stale read
 * is acceptable for a debug aid.
 */
स्थिर BaseType_t prvIsTaskPieBlacklisted( TaskHandle_t xTask )
{
    अत्थाय( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
    {
        सचे( s_pie_blacklist[ i ] == xTask )
        {
            पटिनिवत्त pdTRUE;
        }
    }

    पटिनिवत्त pdFALSE;
}

शून्य vTaskSetPieBlacklisted( TaskHandle_t xTask,
                             BaseType_t xBlacklisted )
{
    TaskHandle_t xHandle = prvResolveTask( xTask );
    BaseType_t xStored = pdTRUE;

    taskENTER_CRITICAL( &s_pie_blacklist_mux );

    सचे( xBlacklisted != pdFALSE )
    {
        सचे( prvIsTaskPieBlacklisted( xHandle ) == pdFALSE )
        {
            xStored = pdFALSE;

            अत्थाय( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
            {
                सचे( s_pie_blacklist[ i ] == रिक्त )
                {
                    s_pie_blacklist[ i ] = xHandle;
                    xStored = pdTRUE;
                    भङ्ग;
                }
            }
        }
    }
    अञ्ञथा
    {
        अत्थाय( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
        {
            सचे( s_pie_blacklist[ i ] == xHandle )
            {
                s_pie_blacklist[ i ] = रिक्त;
                भङ्ग;
            }
        }
    }

    taskEXIT_CRITICAL( &s_pie_blacklist_mux );

    सचे( xStored == pdFALSE )
    {
        ESP_EARLY_LOGE( "FreeRTOS", "PIE blacklist table is full (max %d)", CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX );
        configASSERT( xStored == pdTRUE );
    }
}

BaseType_t xTaskGetPieBlacklisted( TaskHandle_t xTask )
{
    पटिनिवत्त prvIsTaskPieBlacklisted( prvResolveTask( xTask ) );
}

#endif /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */
