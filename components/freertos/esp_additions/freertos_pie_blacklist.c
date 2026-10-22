/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ "sdkconfig.h"

#ఒకవేళ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST

#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "freertos/idf_additions.h"
#అన్తర్భావ "esp_log.h"

/*
 * Keep the blacklist out of the upstream TCB. A small handle table is enough
 * for this debug option and is safe to scan from the PIE exception path.
 */
స్థిర TaskHandle_t అస్థిర s_pie_blacklist[ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX ];
స్థిర పోర్టు_MUX_TYPE s_pie_blacklist_mux = పోర్టు_MUX_INITIALIZER_UNLOCKED;

స్థిర TaskHandle_t prvResolveTask( TaskHandle_t xTask )
{
    తిరిగి ( xTask != ఖాళీ ) ? xTask : xTaskGetCurrentTaskHandle();
}

/*
 * Reads are intentionally lock-free. xTaskGetPieBlacklisted() is called from the
 * PIE exception handler, where taking the spinlock could deadlock. A stale read
 * is acceptable for a debug aid.
 */
స్థిర పునాది_Type_t prvIsTaskPieBlacklisted( TaskHandle_t xTask )
{
    కొరకు( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
    {
        ఒకవేళ( s_pie_blacklist[ i ] == xTask )
        {
            తిరిగి pdTRUE;
        }
    }

    తిరిగి pdFALSE;
}

శూన్య v_Task_సమితి_Pie_Blacklisted( TaskHandle_t xTask,
                             పునాది_Type_t xBlacklisted )
{
    TaskHandle_t xHandle = prvResolveTask( xTask );
    పునాది_Type_t xStored = pdTRUE;

    taskENTER_CRITICAL( &s_pie_blacklist_mux );

    ఒకవేళ( xBlacklisted != pdFALSE )
    {
        ఒకవేళ( prvIsTaskPieBlacklisted( xHandle ) == pdFALSE )
        {
            xStored = pdFALSE;

            కొరకు( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
            {
                ఒకవేళ( s_pie_blacklist[ i ] == ఖాళీ )
                {
                    s_pie_blacklist[ i ] = xHandle;
                    xStored = pdTRUE;
                    ఆపు;
                }
            }
        }
    }
    లేకపోతే
    {
        కొరకు( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
        {
            ఒకవేళ( s_pie_blacklist[ i ] == xHandle )
            {
                s_pie_blacklist[ i ] = ఖాళీ;
                ఆపు;
            }
        }
    }

    taskEXIT_CRITICAL( &s_pie_blacklist_mux );

    ఒకవేళ( xStored == pdFALSE )
    {
        ESP_EARLY_LOGE( "FreeRTOS", "PIE blacklist table is full (max %d)", CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX );
        configASSERT( xStored == pdTRUE );
    }
}

పునాది_Type_t xTaskGetPieBlacklisted( TaskHandle_t xTask )
{
    తిరిగి prvIsTaskPieBlacklisted( prvResolveTask( xTask ) );
}

#అయితే_ముగింపు /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */
