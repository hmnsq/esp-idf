/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "sdkconfig.h"

#إذا CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST

#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "freertos/idf_additions.h"
#ضمن "esp_log.h"

/*
 * Keep the blacklist out of the upstream TCB. A small handle table is enough
 * for this debug option and is safe to scan from the PIE exception path.
 */
ساكن TaskHandle_t متطاير s_pie_blacklist[ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX ];
ساكن پورٽ_MUX_TYPE s_pie_blacklist_mux = پورٽ_MUX_INITIALIZER_UNLOCKED;

ساكن TaskHandle_t prvResolveTask( TaskHandle_t xTask )
{
    أرجع ( xTask != عدم ) ? xTask : xTaskGetCurrentTaskHandle();
}

/*
 * Reads are intentionally lock-free. xTaskGetPieBlacklisted() is called from the
 * PIE exception handler, where taking the spinlock could deadlock. A stale read
 * is acceptable for a debug aid.
 */
ساكن بنياد_Type_t prvIsTaskPieBlacklisted( TaskHandle_t xTask )
{
    لكل( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
    {
        إذا( s_pie_blacklist[ i ] == xTask )
        {
            أرجع pdTRUE;
        }
    }

    أرجع pdFALSE;
}

فراغ v_Task_سيٽ_Pie_Blacklisted( TaskHandle_t xTask,
                             بنياد_Type_t xBlacklisted )
{
    TaskHandle_t xHandle = prvResolveTask( xTask );
    بنياد_Type_t xStored = pdTRUE;

    taskENTER_CRITICAL( &s_pie_blacklist_mux );

    إذا( xBlacklisted != pdFALSE )
    {
        إذا( prvIsTaskPieBlacklisted( xHandle ) == pdFALSE )
        {
            xStored = pdFALSE;

            لكل( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
            {
                إذا( s_pie_blacklist[ i ] == عدم )
                {
                    s_pie_blacklist[ i ] = xHandle;
                    xStored = pdTRUE;
                    اكسر;
                }
            }
        }
    }
    وإلا
    {
        لكل( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
        {
            إذا( s_pie_blacklist[ i ] == xHandle )
            {
                s_pie_blacklist[ i ] = عدم;
                اكسر;
            }
        }
    }

    taskEXIT_CRITICAL( &s_pie_blacklist_mux );

    إذا( xStored == pdFALSE )
    {
        ESP_EARLY_LOGE( "FreeRTOS", "PIE blacklist table is full (max %d)", CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX );
        configASSERT( xStored == pdTRUE );
    }
}

بنياد_Type_t xTaskGetPieBlacklisted( TaskHandle_t xTask )
{
    أرجع prvIsTaskPieBlacklisted( prvResolveTask( xTask ) );
}

#جيڪڏهن_پورو /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */
