/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#अन्तर्भाव "sdkconfig.h"

#यदि CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST

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
    फर्काऊ ( xTask != रिक्त ) ? xTask : xTaskGetCurrentTaskHandle();
}

/*
 * Reads are intentionally lock-free. xTaskGetPieBlacklisted() is called from the
 * PIE exception handler, where taking the spinlock could deadlock. A stale read
 * is acceptable for a debug aid.
 */
स्थिर BaseType_t prvIsTaskPieBlacklisted( TaskHandle_t xTask )
{
    लागि( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
    {
        यदि( s_pie_blacklist[ i ] == xTask )
        {
            फर्काऊ pdTRUE;
        }
    }

    फर्काऊ pdFALSE;
}

शून्य vTaskSetPieBlacklisted( TaskHandle_t xTask,
                             BaseType_t xBlacklisted )
{
    TaskHandle_t xHandle = prvResolveTask( xTask );
    BaseType_t xStored = pdTRUE;

    taskENTER_CRITICAL( &s_pie_blacklist_mux );

    यदि( xBlacklisted != pdFALSE )
    {
        यदि( prvIsTaskPieBlacklisted( xHandle ) == pdFALSE )
        {
            xStored = pdFALSE;

            लागि( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
            {
                यदि( s_pie_blacklist[ i ] == रिक्त )
                {
                    s_pie_blacklist[ i ] = xHandle;
                    xStored = pdTRUE;
                    रोक;
                }
            }
        }
    }
    अन्यथा
    {
        लागि( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
        {
            यदि( s_pie_blacklist[ i ] == xHandle )
            {
                s_pie_blacklist[ i ] = रिक्त;
                रोक;
            }
        }
    }

    taskEXIT_CRITICAL( &s_pie_blacklist_mux );

    यदि( xStored == pdFALSE )
    {
        ESP_EARLY_LOGE( "FreeRTOS", "PIE blacklist table is full (max %d)", CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX );
        configASSERT( xStored == pdTRUE );
    }
}

BaseType_t xTaskGetPieBlacklisted( TaskHandle_t xTask )
{
    फर्काऊ prvIsTaskPieBlacklisted( prvResolveTask( xTask ) );
}

#endif /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */
