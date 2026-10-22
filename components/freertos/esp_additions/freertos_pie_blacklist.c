/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#समावेश "sdkconfig.h"

#अगर CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST

#समावेश "freertos/FreeRTOS.h"
#समावेश "freertos/task.h"
#समावेश "freertos/idf_additions.h"
#समावेश "esp_log.h"

/*
 * Keep the blacklist out of the upstream TCB. A small handle table is enough
 * for this debug option and is safe to scan from the PIE exception path.
 */
अटल TaskHandle_t अस्थिर s_pie_blacklist[ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX ];
अटल portMUX_TYPE s_pie_blacklist_mux = portMUX_INITIALIZER_UNLOCKED;

अटल TaskHandle_t prvResolveTask( TaskHandle_t xTask )
{
    वापस ( xTask != शून्य ) ? xTask : xTaskGetCurrentTaskHandle();
}

/*
 * Reads are intentionally lock-free. xTaskGetPieBlacklisted() is called from the
 * PIE exception handler, where taking the spinlock could deadlock. A stale read
 * is acceptable for a debug aid.
 */
अटल BaseType_t prvIsTaskPieBlacklisted( TaskHandle_t xTask )
{
    क्रम( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
    {
        अगर( s_pie_blacklist[ i ] == xTask )
        {
            वापस pdTRUE;
        }
    }

    वापस pdFALSE;
}

व्योम vTaskSetPieBlacklisted( TaskHandle_t xTask,
                             BaseType_t xBlacklisted )
{
    TaskHandle_t xHandle = prvResolveTask( xTask );
    BaseType_t xStored = pdTRUE;

    taskENTER_CRITICAL( &s_pie_blacklist_mux );

    अगर( xBlacklisted != pdFALSE )
    {
        अगर( prvIsTaskPieBlacklisted( xHandle ) == pdFALSE )
        {
            xStored = pdFALSE;

            क्रम( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
            {
                अगर( s_pie_blacklist[ i ] == शून्य )
                {
                    s_pie_blacklist[ i ] = xHandle;
                    xStored = pdTRUE;
                    अवरोध;
                }
            }
        }
    }
    अन्यथा
    {
        क्रम( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
        {
            अगर( s_pie_blacklist[ i ] == xHandle )
            {
                s_pie_blacklist[ i ] = शून्य;
                अवरोध;
            }
        }
    }

    taskEXIT_CRITICAL( &s_pie_blacklist_mux );

    अगर( xStored == pdFALSE )
    {
        ESP_EARLY_LOGE( "FreeRTOS", "PIE blacklist table is full (max %d)", CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX );
        configASSERT( xStored == pdTRUE );
    }
}

BaseType_t xTaskGetPieBlacklisted( TaskHandle_t xTask )
{
    वापस prvIsTaskPieBlacklisted( prvResolveTask( xTask ) );
}

#पूर्ण_अगर /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */
