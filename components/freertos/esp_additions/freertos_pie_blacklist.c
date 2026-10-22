/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "sdkconfig.h"

#གལ་ཏེ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/idf_additions.h"
#include "esp_log.h"

/*
 * Keep the blacklist out of the upstream TCB. A small handle table is enough
 * for this debug option and is safe to scan from the PIE exception path.
 */
static TaskHandle_t ca_ncala s_pie_blacklist[ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX ];
static portMUX_TYPE s_pie_blacklist_mux = portMUX_INITIALIZER_UNLOCKED;

static TaskHandle_t prvResolveTask( TaskHandle_t xTask )
{
    ཕྱིར ( xTask != NULL ) ? xTask : xTaskGetCurrentTaskHandle();
}

/*
 * Reads are intentionally lock-free. xTaskGetPieBlacklisted() is called from the
 * PIE exception handler, where taking the spinlock could deadlock. A stale read
 * is acceptable for a debug aid.
 */
static BaseType_t prvIsTaskPieBlacklisted( TaskHandle_t xTask )
{
    ཆེད་དུ( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
    {
        གལ་ཏེ( s_pie_blacklist[ i ] == xTask )
        {
            ཕྱིར pdTRUE;
        }
    }

    ཕྱིར pdFALSE;
}

void vTaskSetPieBlacklisted( TaskHandle_t xTask,
                             BaseType_t xBlacklisted )
{
    TaskHandle_t xHandle = prvResolveTask( xTask );
    BaseType_t xStored = pdTRUE;

    taskENTER_CRITICAL( &s_pie_blacklist_mux );

    གལ་ཏེ( xBlacklisted != pdFALSE )
    {
        གལ་ཏེ( prvIsTaskPieBlacklisted( xHandle ) == pdFALSE )
        {
            xStored = pdFALSE;

            ཆེད་དུ( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
            {
                གལ་ཏེ( s_pie_blacklist[ i ] == NULL )
                {
                    s_pie_blacklist[ i ] = xHandle;
                    xStored = pdTRUE;
                    bha_nga;
                }
            }
        }
    }
    དེ་མིན
    {
        ཆེད་དུ( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
        {
            གལ་ཏེ( s_pie_blacklist[ i ] == xHandle )
            {
                s_pie_blacklist[ i ] = NULL;
                bha_nga;
            }
        }
    }

    taskEXIT_CRITICAL( &s_pie_blacklist_mux );

    གལ་ཏེ( xStored == pdFALSE )
    {
        ESP_EARLY_LOGE( "FreeRTOS", "PIE blacklist table is full (max %d)", CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX );
        configASSERT( xStored == pdTRUE );
    }
}

BaseType_t xTaskGetPieBlacklisted( TaskHandle_t xTask )
{
    ཕྱིར prvIsTaskPieBlacklisted( prvResolveTask( xTask ) );
}

#endif /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */
