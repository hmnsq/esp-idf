/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "sdkconfig.h"

#אם CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST

#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "freertos/idf_additions.h"
#ضمن "esp_log.h"

/*
 * Keep the blacklist out of the upstream TCB. A small handle table is enough
 * for this debug option and is safe to scan from the PIE exception path.
 */
ساكن TaskHandle_t متطاير s_pie_blacklist[ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX ];
ساكن portMUX_TYPE s_pie_blacklist_mux = portMUX_INITIALIZER_UNLOCKED;

ساكن TaskHandle_t prvResolveTask( TaskHandle_t xTask )
{
    החזר ( xTask != عدم ) ? xTask : xTaskGetCurrentTaskHandle();
}

/*
 * Reads are intentionally lock-free. xTaskGetPieBlacklisted() is called from the
 * PIE exception handler, where taking the spinlock could deadlock. A stale read
 * is acceptable for a debug aid.
 */
ساكن BaseType_t prvIsTaskPieBlacklisted( TaskHandle_t xTask )
{
    עבור( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
    {
        אם( s_pie_blacklist[ i ] == xTask )
        {
            החזר pdTRUE;
        }
    }

    החזר pdFALSE;
}

فراغ vTaskSetPieBlacklisted( TaskHandle_t xTask,
                             BaseType_t xBlacklisted )
{
    TaskHandle_t xHandle = prvResolveTask( xTask );
    BaseType_t xStored = pdTRUE;

    taskENTER_CRITICAL( &s_pie_blacklist_mux );

    אם( xBlacklisted != pdFALSE )
    {
        אם( prvIsTaskPieBlacklisted( xHandle ) == pdFALSE )
        {
            xStored = pdFALSE;

            עבור( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
            {
                אם( s_pie_blacklist[ i ] == عدم )
                {
                    s_pie_blacklist[ i ] = xHandle;
                    xStored = pdTRUE;
                    اكسر;
                }
            }
        }
    }
    אחרת
    {
        עבור( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
        {
            אם( s_pie_blacklist[ i ] == xHandle )
            {
                s_pie_blacklist[ i ] = عدم;
                اكسر;
            }
        }
    }

    taskEXIT_CRITICAL( &s_pie_blacklist_mux );

    אם( xStored == pdFALSE )
    {
        ESP_EARLY_LOGE( "FreeRTOS", "PIE blacklist table is full (max %d)", CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX );
        configASSERT( xStored == pdTRUE );
    }
}

BaseType_t xTaskGetPieBlacklisted( TaskHandle_t xTask )
{
    החזר prvIsTaskPieBlacklisted( prvResolveTask( xTask ) );
}

#endif /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */
