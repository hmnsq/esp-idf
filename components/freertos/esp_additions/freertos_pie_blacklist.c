/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#അന്തര്ഭാവ "sdkconfig.h"

#എങ്കിൽ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST

#അന്തര്ഭാവ "freertos/FreeRTOS.h"
#അന്തര്ഭാവ "freertos/task.h"
#അന്തര്ഭാവ "freertos/idf_additions.h"
#അന്തര്ഭാവ "esp_log.h"

/*
 * Keep the blacklist out of the upstream TCB. A small handle table is enough
 * for this debug option and is safe to scan from the PIE exception path.
 */
സ്ഥിതം TaskHandle_t അസ്ഥിര s_pie_blacklist[ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX ];
സ്ഥിതം portMUX_TYPE s_pie_blacklist_mux = portMUX_INITIALIZER_UNLOCKED;

സ്ഥിതം TaskHandle_t prvResolveTask( TaskHandle_t xTask )
{
    തിരികെ ( xTask != ശൂന്യ ) ? xTask : xTaskGetCurrentTaskHandle();
}

/*
 * Reads are intentionally lock-free. xTaskGetPieBlacklisted() is called from the
 * PIE exception handler, where taking the spinlock could deadlock. A stale read
 * is acceptable for a debug aid.
 */
സ്ഥിതം BaseType_t prvIsTaskPieBlacklisted( TaskHandle_t xTask )
{
    വേണ്ടി( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
    {
        എങ്കിൽ( s_pie_blacklist[ i ] == xTask )
        {
            തിരികെ pdTRUE;
        }
    }

    തിരികെ pdFALSE;
}

ശൂന്യം vTaskSetPieBlacklisted( TaskHandle_t xTask,
                             BaseType_t xBlacklisted )
{
    TaskHandle_t xHandle = prvResolveTask( xTask );
    BaseType_t xStored = pdTRUE;

    taskENTER_CRITICAL( &s_pie_blacklist_mux );

    എങ്കിൽ( xBlacklisted != pdFALSE )
    {
        എങ്കിൽ( prvIsTaskPieBlacklisted( xHandle ) == pdFALSE )
        {
            xStored = pdFALSE;

            വേണ്ടി( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
            {
                എങ്കിൽ( s_pie_blacklist[ i ] == ശൂന്യ )
                {
                    s_pie_blacklist[ i ] = xHandle;
                    xStored = pdTRUE;
                    നിർത്തുക;
                }
            }
        }
    }
    അല്ലെങ്കിൽ
    {
        വേണ്ടി( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
        {
            എങ്കിൽ( s_pie_blacklist[ i ] == xHandle )
            {
                s_pie_blacklist[ i ] = ശൂന്യ;
                നിർത്തുക;
            }
        }
    }

    taskEXIT_CRITICAL( &s_pie_blacklist_mux );

    എങ്കിൽ( xStored == pdFALSE )
    {
        ESP_EARLY_LOGE( "FreeRTOS", "PIE blacklist table is full (max %d)", CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX );
        configASSERT( xStored == pdTRUE );
    }
}

BaseType_t xTaskGetPieBlacklisted( TaskHandle_t xTask )
{
    തിരികെ prvIsTaskPieBlacklisted( prvResolveTask( xTask ) );
}

#endif /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */
