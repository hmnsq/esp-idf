/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#অন্তর্ভাৱ "sdkconfig.h"

#যদি CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST

#অন্তর্ভাৱ "freertos/FreeRTOS.h"
#অন্তর্ভাৱ "freertos/task.h"
#অন্তর্ভাৱ "freertos/idf_additions.h"
#অন্তর্ভাৱ "esp_log.h"

/*
 * Keep the blacklist out of the upstream TCB. A small handle table is enough
 * for this debug option and is safe to scan from the PIE exception path.
 */
স্থির TaskHandle_t চঞ্চল s_pie_blacklist[ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX ];
স্থির portMUX_TYPE s_pie_blacklist_mux = portMUX_INITIALIZER_UNLOCKED;

স্থির TaskHandle_t prvResolveTask( TaskHandle_t xTask )
{
    ঘূৰাই ( xTask != রিক্ত ) ? xTask : xTaskGetCurrentTaskHandle();
}

/*
 * Reads are intentionally lock-free. xTaskGetPieBlacklisted() is called from the
 * PIE exception handler, where taking the spinlock could deadlock. A stale read
 * is acceptable for a debug aid.
 */
স্থির BaseType_t prvIsTaskPieBlacklisted( TaskHandle_t xTask )
{
    বাবে( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
    {
        যদি( s_pie_blacklist[ i ] == xTask )
        {
            ঘূৰাই pdTRUE;
        }
    }

    ঘূৰাই pdFALSE;
}

শূন্য vTaskSetPieBlacklisted( TaskHandle_t xTask,
                             BaseType_t xBlacklisted )
{
    TaskHandle_t xHandle = prvResolveTask( xTask );
    BaseType_t xStored = pdTRUE;

    taskENTER_CRITICAL( &s_pie_blacklist_mux );

    যদি( xBlacklisted != pdFALSE )
    {
        যদি( prvIsTaskPieBlacklisted( xHandle ) == pdFALSE )
        {
            xStored = pdFALSE;

            বাবে( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
            {
                যদি( s_pie_blacklist[ i ] == রিক্ত )
                {
                    s_pie_blacklist[ i ] = xHandle;
                    xStored = pdTRUE;
                    ভঙ্গ;
                }
            }
        }
    }
    অন্যথা
    {
        বাবে( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
        {
            যদি( s_pie_blacklist[ i ] == xHandle )
            {
                s_pie_blacklist[ i ] = রিক্ত;
                ভঙ্গ;
            }
        }
    }

    taskEXIT_CRITICAL( &s_pie_blacklist_mux );

    যদি( xStored == pdFALSE )
    {
        ESP_EARLY_LOGE( "FreeRTOS", "PIE blacklist table is full (max %d)", CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX );
        configASSERT( xStored == pdTRUE );
    }
}

BaseType_t xTaskGetPieBlacklisted( TaskHandle_t xTask )
{
    ঘূৰাই prvIsTaskPieBlacklisted( prvResolveTask( xTask ) );
}

#endif /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */
