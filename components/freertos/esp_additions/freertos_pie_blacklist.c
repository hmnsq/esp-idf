/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ଅନ୍ତର୍ଭାଵ "sdkconfig.h"

#ଯଦି CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST

#ଅନ୍ତର୍ଭାଵ "freertos/FreeRTOS.h"
#ଅନ୍ତର୍ଭାଵ "freertos/task.h"
#ଅନ୍ତର୍ଭାଵ "freertos/idf_additions.h"
#ଅନ୍ତର୍ଭାଵ "esp_log.h"

/*
 * Keep the blacklist out of the upstream TCB. A small handle table is enough
 * for this debug option and is safe to scan from the PIE exception path.
 */
ସ୍ଥିର TaskHandle_t ଅସ୍ଥିର s_pie_blacklist[ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX ];
ସ୍ଥିର portMUX_TYPE s_pie_blacklist_mux = portMUX_INITIALIZER_UNLOCKED;

ସ୍ଥିର TaskHandle_t prvResolveTask( TaskHandle_t xTask )
{
    ଫେରନ୍ତୁ ( xTask != ଖାଲି ) ? xTask : xTaskGetCurrentTaskHandle();
}

/*
 * Reads are intentionally lock-free. xTaskGetPieBlacklisted() is called from the
 * PIE exception handler, where taking the spinlock could deadlock. A stale read
 * is acceptable for a debug aid.
 */
ସ୍ଥିର BaseType_t prvIsTaskPieBlacklisted( TaskHandle_t xTask )
{
    ପାଇଁ( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
    {
        ଯଦି( s_pie_blacklist[ i ] == xTask )
        {
            ଫେରନ୍ତୁ pdTRUE;
        }
    }

    ଫେରନ୍ତୁ pdFALSE;
}

ଶୂନ୍ୟ vTaskSetPieBlacklisted( TaskHandle_t xTask,
                             BaseType_t xBlacklisted )
{
    TaskHandle_t xHandle = prvResolveTask( xTask );
    BaseType_t xStored = pdTRUE;

    taskENTER_CRITICAL( &s_pie_blacklist_mux );

    ଯଦି( xBlacklisted != pdFALSE )
    {
        ଯଦି( prvIsTaskPieBlacklisted( xHandle ) == pdFALSE )
        {
            xStored = pdFALSE;

            ପାଇଁ( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
            {
                ଯଦି( s_pie_blacklist[ i ] == ଖାଲି )
                {
                    s_pie_blacklist[ i ] = xHandle;
                    xStored = pdTRUE;
                    ଭାଙ୍ଗ;
                }
            }
        }
    }
    ଅନ୍ୟଥା
    {
        ପାଇଁ( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
        {
            ଯଦି( s_pie_blacklist[ i ] == xHandle )
            {
                s_pie_blacklist[ i ] = ଖାଲି;
                ଭାଙ୍ଗ;
            }
        }
    }

    taskEXIT_CRITICAL( &s_pie_blacklist_mux );

    ଯଦି( xStored == pdFALSE )
    {
        ESP_EARLY_LOGE( "FreeRTOS", "PIE blacklist table is full (max %d)", CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX );
        configASSERT( xStored == pdTRUE );
    }
}

BaseType_t xTaskGetPieBlacklisted( TaskHandle_t xTask )
{
    ଫେରନ୍ତୁ prvIsTaskPieBlacklisted( prvResolveTask( xTask ) );
}

#endif /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */
