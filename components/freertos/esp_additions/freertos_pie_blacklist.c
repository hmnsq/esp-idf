/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#شامل "sdkconfig.h"

#اگر CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST

#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل "freertos/idf_additions.h"
#شامل "esp_log.h"

/*
 * Keep the blacklist out of the upstream TCB. A small handle table is enough
 * for this debug option and is safe to scan from the PIE exception path.
 */
ساکن TaskHandle_t متلون s_pie_blacklist[ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX ];
ساکن پورٹ_MUX_TYPE s_pie_blacklist_mux = پورٹ_MUX_INITIALIZER_UNLOCKED;

ساکن TaskHandle_t prvResolveTask( TaskHandle_t xTask )
{
    واپس ( xTask != تہی ) ? xTask : xTaskGetCurrentTaskHandle();
}

/*
 * Reads are intentionally lock-free. xTaskGetPieBlacklisted() is called from the
 * PIE exception handler, where taking the spinlock could deadlock. A stale read
 * is acceptable for a debug aid.
 */
ساکن بنیاد_Type_t prvIsTaskPieBlacklisted( TaskHandle_t xTask )
{
    برائے( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
    {
        اگر( s_pie_blacklist[ i ] == xTask )
        {
            واپس pdTRUE;
        }
    }

    واپس pdFALSE;
}

خالی v_Task_سیٹ_Pie_Blacklisted( TaskHandle_t xTask,
                             بنیاد_Type_t xBlacklisted )
{
    TaskHandle_t xHandle = prvResolveTask( xTask );
    بنیاد_Type_t xStored = pdTRUE;

    taskENTER_CRITICAL( &s_pie_blacklist_mux );

    اگر( xBlacklisted != pdFALSE )
    {
        اگر( prvIsTaskPieBlacklisted( xHandle ) == pdFALSE )
        {
            xStored = pdFALSE;

            برائے( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
            {
                اگر( s_pie_blacklist[ i ] == تہی )
                {
                    s_pie_blacklist[ i ] = xHandle;
                    xStored = pdTRUE;
                    توڑو;
                }
            }
        }
    }
    ورنہ
    {
        برائے( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
        {
            اگر( s_pie_blacklist[ i ] == xHandle )
            {
                s_pie_blacklist[ i ] = تہی;
                توڑو;
            }
        }
    }

    taskEXIT_CRITICAL( &s_pie_blacklist_mux );

    اگر( xStored == pdFALSE )
    {
        ESP_EARLY_LOGE( "FreeRTOS", "PIE blacklist table is full (max %d)", CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX );
        configASSERT( xStored == pdTRUE );
    }
}

بنیاد_Type_t xTaskGetPieBlacklisted( TaskHandle_t xTask )
{
    واپس prvIsTaskPieBlacklisted( prvResolveTask( xTask ) );
}

#اگر_ختم /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */
