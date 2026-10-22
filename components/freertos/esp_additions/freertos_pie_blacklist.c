/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"

#ಯದಿ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST

#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "freertos/idf_additions.h"
#ಅನ್ತರ್ಭಾವ "esp_log.h"

/*
 * Keep the blacklist out of the upstream TCB. A small handle table is enough
 * for this debug option and is safe to scan from the PIE exception path.
 */
ಸ್ಥಿರ TaskHandle_t ಚಞ್ಚಲ s_pie_blacklist[ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX ];
ಸ್ಥಿರ portMUX_TYPE s_pie_blacklist_mux = portMUX_INITIALIZER_UNLOCKED;

ಸ್ಥಿರ TaskHandle_t prvResolveTask( TaskHandle_t xTask )
{
    ಪ್ರತ್ಯಾಗಮ ( xTask != ರಿಕ್ತ ) ? xTask : xTaskGetCurrentTaskHandle();
}

/*
 * Reads are intentionally lock-free. xTaskGetPieBlacklisted() is called from the
 * PIE exception handler, where taking the spinlock could deadlock. A stale read
 * is acceptable for a debug aid.
 */
ಸ್ಥಿರ BaseType_t prvIsTaskPieBlacklisted( TaskHandle_t xTask )
{
    ಪ್ರತಿ( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
    {
        ಯದಿ( s_pie_blacklist[ i ] == xTask )
        {
            ಪ್ರತ್ಯಾಗಮ pdTRUE;
        }
    }

    ಪ್ರತ್ಯಾಗಮ pdFALSE;
}

ಶೂನ್ಯ vTaskSetPieBlacklisted( TaskHandle_t xTask,
                             BaseType_t xBlacklisted )
{
    TaskHandle_t xHandle = prvResolveTask( xTask );
    BaseType_t xStored = pdTRUE;

    taskENTER_CRITICAL( &s_pie_blacklist_mux );

    ಯದಿ( xBlacklisted != pdFALSE )
    {
        ಯದಿ( prvIsTaskPieBlacklisted( xHandle ) == pdFALSE )
        {
            xStored = pdFALSE;

            ಪ್ರತಿ( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
            {
                ಯದಿ( s_pie_blacklist[ i ] == ರಿಕ್ತ )
                {
                    s_pie_blacklist[ i ] = xHandle;
                    xStored = pdTRUE;
                    ಭಙ್ಗ;
                }
            }
        }
    }
    ಅನ್ಯಥಾ
    {
        ಪ್ರತಿ( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
        {
            ಯದಿ( s_pie_blacklist[ i ] == xHandle )
            {
                s_pie_blacklist[ i ] = ರಿಕ್ತ;
                ಭಙ್ಗ;
            }
        }
    }

    taskEXIT_CRITICAL( &s_pie_blacklist_mux );

    ಯದಿ( xStored == pdFALSE )
    {
        ESP_EARLY_LOGE( "FreeRTOS", "PIE blacklist table is full (max %d)", CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX );
        configASSERT( xStored == pdTRUE );
    }
}

BaseType_t xTaskGetPieBlacklisted( TaskHandle_t xTask )
{
    ಪ್ರತ್ಯಾಗಮ prvIsTaskPieBlacklisted( prvResolveTask( xTask ) );
}

#endif /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */
