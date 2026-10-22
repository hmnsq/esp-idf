/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#অন্তর্ভাব "sdkconfig.h"

#যদি CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST

#অন্তর্ভাব "freertos/FreeRTOS.h"
#অন্তর্ভাব "freertos/task.h"
#অন্তর্ভাব "freertos/idf_additions.h"
#অন্তর্ভাব "esp_log.h"

/*
 * Keep the blacklist out of the upstream TCB. A small handle table is enough
 * for this debug option and is safe to scan from the PIE exception path.
 */
স্থির TaskHandle_t অস্থির s_pie_blacklist[ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX ];
স্থির portMUX_TYPE s_pie_blacklist_mux = portMUX_INITIALIZER_UNLOCKED;

স্থির TaskHandle_t prvResolveTask( TaskHandle_t xTask )
{
    ফেরত ( xTask != ফাঁকা ) ? xTask : xTaskGetCurrentTaskHandle();
}

/*
 * Reads are intentionally lock-free. xTaskGetPieBlacklisted() is called from the
 * PIE exception handler, where taking the spinlock could deadlock. A stale read
 * is acceptable for a debug aid.
 */
স্থির BaseType_t prvIsTaskPieBlacklisted( TaskHandle_t xTask )
{
    জন্য( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
    {
        যদি( s_pie_blacklist[ i ] == xTask )
        {
            ফেরত pdTRUE;
        }
    }

    ফেরত pdFALSE;
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

            জন্য( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
            {
                যদি( s_pie_blacklist[ i ] == ফাঁকা )
                {
                    s_pie_blacklist[ i ] = xHandle;
                    xStored = pdTRUE;
                    বিরতি;
                }
            }
        }
    }
    অন্যথা
    {
        জন্য( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
        {
            যদি( s_pie_blacklist[ i ] == xHandle )
            {
                s_pie_blacklist[ i ] = ফাঁকা;
                বিরতি;
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
    ফেরত prvIsTaskPieBlacklisted( prvResolveTask( xTask ) );
}

#endif /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */
