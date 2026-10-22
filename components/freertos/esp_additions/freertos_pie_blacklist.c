/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#अन्तर्भाव "sdkconfig.h"

#यदि CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST

#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"
#अन्तर्भाव "freertos/idf_additions.h"
#अन्तर्भाव "esp_log.h"

/*
 * Keep the blacklist out of the upstream TCB. A small handle table is enough
 * for this debug option and is safe to scan from the PIE exception path.
 */
स्थिर TaskHandle_t चञ्चल s_pie_blacklist[ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX ];
स्थिर पत्तनम्_MUX_TYPE s_pie_blacklist_mux = पत्तनम्_MUX_INITIALIZER_UNLOCKED;

स्थिर TaskHandle_t prvResolveTask( TaskHandle_t xTask )
{
    प्रत्यागम ( xTask != रिक्त ) ? xTask : xTaskGetCurrentTaskHandle();
}

/*
 * Reads are intentionally lock-free. xTaskGetPieBlacklisted() is called from the
 * PIE exception handler, where taking the spinlock could deadlock. A stale read
 * is acceptable for a debug aid.
 */
स्थिर आधारः_Type_t prvIsTaskPieBlacklisted( TaskHandle_t xTask )
{
    प्रति( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
    {
        यदि( s_pie_blacklist[ i ] == xTask )
        {
            प्रत्यागम pdTRUE;
        }
    }

    प्रत्यागम pdFALSE;
}

शून्य v_Task_समुच्चयः_Pie_Blacklisted( TaskHandle_t xTask,
                             आधारः_Type_t xBlacklisted )
{
    TaskHandle_t xHandle = prvResolveTask( xTask );
    आधारः_Type_t xStored = pdTRUE;

    taskENTER_CRITICAL( &s_pie_blacklist_mux );

    यदि( xBlacklisted != pdFALSE )
    {
        यदि( prvIsTaskPieBlacklisted( xHandle ) == pdFALSE )
        {
            xStored = pdFALSE;

            प्रति( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
            {
                यदि( s_pie_blacklist[ i ] == रिक्त )
                {
                    s_pie_blacklist[ i ] = xHandle;
                    xStored = pdTRUE;
                    भङ्ग;
                }
            }
        }
    }
    अन्यथा
    {
        प्रति( UBaseType_t i = 0; i < CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX; i++ )
        {
            यदि( s_pie_blacklist[ i ] == xHandle )
            {
                s_pie_blacklist[ i ] = रिक्त;
                भङ्ग;
            }
        }
    }

    taskEXIT_CRITICAL( &s_pie_blacklist_mux );

    यदि( xStored == pdFALSE )
    {
        ESP_EARLY_LOGE( "FreeRTOS", "PIE blacklist table is full (max %d)", CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST_MAX );
        configASSERT( xStored == pdTRUE );
    }
}

आधारः_Type_t xTaskGetPieBlacklisted( TaskHandle_t xTask )
{
    प्रत्यागम prvIsTaskPieBlacklisted( prvResolveTask( xTask ) );
}

#यदि_अन्तः /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */
