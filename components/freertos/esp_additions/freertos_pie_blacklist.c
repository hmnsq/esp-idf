/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"

#ಆದರೆ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST

#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "freertos/idf_additions.h"
#ಅನ್ತರ್ಭಾವ "esp_log.h"

/*
 * Keep the blacklist out of the upstream TCB. A small handle table is enough
 * for this debug option and is safe to scan from the PIE exception path.
 */
ಸ್ಥಿರ TaskHandle_t ಅಸ್ಥಿರ s_pie_blacklist[ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX ];
ಸ್ಥಿರ ಪೋರ್ಟ್_MUX_TYPE s_pie_blacklist_mux = ಪೋರ್ಟ್_MUX_INITIALIZER_UNLOCKED;

ಸ್ಥಿರ TaskHandle_t prvResolveTask( TaskHandle_t xTask )
{
    ಹಿಂತಿರುಗಿ ( xTask != ಖಾಲಿ ) ? xTask : xTaskGetCurrentTaskHandle();
}

/*
 * Reads are intentionally lock-free. xTaskGetPieBlacklisted() is called from the
 * PIE exception handler, where taking the spinlock could deadlock. A stale read
 * is acceptable for a debug aid.
 */
ಸ್ಥಿರ ತಳ_Type_t prvIsTaskPieBlacklisted( TaskHandle_t xTask )
{
    ಗಾಗಿ( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
    {
        ಆದರೆ( s_pie_blacklist[ i ] == xTask )
        {
            ಹಿಂತಿರುಗಿ pdTRUE;
        }
    }

    ಹಿಂತಿರುಗಿ pdFALSE;
}

ಶೂನ್ಯ v_Task_ಗಣ_Pie_Blacklisted( TaskHandle_t xTask,
                             ತಳ_Type_t xBlacklisted )
{
    TaskHandle_t xHandle = prvResolveTask( xTask );
    ತಳ_Type_t xStored = pdTRUE;

    taskENTER_CRITICAL( &s_pie_blacklist_mux );

    ಆದರೆ( xBlacklisted != pdFALSE )
    {
        ಆದರೆ( prvIsTaskPieBlacklisted( xHandle ) == pdFALSE )
        {
            xStored = pdFALSE;

            ಗಾಗಿ( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
            {
                ಆದರೆ( s_pie_blacklist[ i ] == ಖಾಲಿ )
                {
                    s_pie_blacklist[ i ] = xHandle;
                    xStored = pdTRUE;
                    ನಿಲ್ಲಿಸು;
                }
            }
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಗಾಗಿ( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
        {
            ಆದರೆ( s_pie_blacklist[ i ] == xHandle )
            {
                s_pie_blacklist[ i ] = ಖಾಲಿ;
                ನಿಲ್ಲಿಸು;
            }
        }
    }

    taskEXIT_CRITICAL( &s_pie_blacklist_mux );

    ಆದರೆ( xStored == pdFALSE )
    {
        ESP_EARLY_LOGE( "FreeRTOS", "PIE blacklist table is full (max %d)", CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX );
        configASSERT( xStored == pdTRUE );
    }
}

ತಳ_Type_t xTaskGetPieBlacklisted( TaskHandle_t xTask )
{
    ಹಿಂತಿರುಗಿ prvIsTaskPieBlacklisted( prvResolveTask( xTask ) );
}

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */
