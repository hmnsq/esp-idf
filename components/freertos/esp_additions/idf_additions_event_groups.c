/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * This file contains the implementation for some the functions in
 * idf_additions.h
 *
 * event group functions are split into a separate file due to the dependency chain
 * xEventGroupCreateWithCaps->xEventGroupCreateStatic->event_groups.c->xTimerPendFunctionCallFromISR->timers.c
 *
 * In some cases this results in the weak timer task function getting overridden and
 * used even if the event group functions were discarded due to not being used.
 *
 * Putting the event groups function in a separate file avoids this issue unless the users himself calls
 * event group functions
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/event_groups.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/idf_additions.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_heap_caps.h"

/* ------------------------------ Event Groups ------------------------------ */

#ਜੇ ( config_SUPPORT_STATIC_ਵੰਡ == 1 )

    Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ_Create_With_Caps( UBaseType_t ux_ਸਮ੍ਰਿਤੀ_Caps )
    {
        Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ;
        Static_Event_ਸਮੂਹ_t * px_Event_ਸਮੂਹ_ਬਫਰ;

        /* Allocate memory for the event group using the provided memory caps */
        px_Event_ਸਮੂਹ_ਬਫਰ = ਢੇਰ_caps_malloc( ਆਕਾਰ( Static_Event_ਸਮੂਹ_t ), ux_ਸਮ੍ਰਿਤੀ_Caps );

        ਜੇ( px_Event_ਸਮੂਹ_ਬਫਰ == ਨਿਰਾ )
        {
            ਵਾਪਸ ਨਿਰਾ;
        }

        /* Create the event group using static creation API */
        x_Event_ਸਮੂਹ = x_Event_ਸਮੂਹ_Create_Static( px_Event_ਸਮੂਹ_ਬਫਰ );

        ਜੇ( x_Event_ਸਮੂਹ == ਨਿਰਾ )
        {
            ਢੇਰ_caps_free( px_Event_ਸਮੂਹ_ਬਫਰ );
        }

        ਵਾਪਸ x_Event_ਸਮੂਹ;
    }

#ਜੇ_ਅੰਤ /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#ਜੇ ( config_SUPPORT_STATIC_ਵੰਡ == 1 )

    ਖਾਲੀ v_Event_ਸਮੂਹ_Delete_With_Caps( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ )
    {
        // Return value unused if asserts are disabled
        ਅਧਾਰ_Type_t __attribute__((ਅਣਵਰਤਿਆ)) x_ਨਤੀਜਾ;
        Static_Event_ਸਮੂਹ_t * px_Event_ਸਮੂਹ_ਬਫਰ;

        /* Retrieve the buffer used to create the event group before deleting it
         * */
        x_ਨਤੀਜਾ = x_Event_ਸਮੂਹ_Get_Static_ਬਫਰ( x_Event_ਸਮੂਹ, &px_Event_ਸਮੂਹ_ਬਫਰ );
        configASSERT( x_ਨਤੀਜਾ == pdTRUE );

        /* Delete the event group */
        v_Event_ਸਮੂਹ_Delete( x_Event_ਸਮੂਹ );

        /* Free the memory buffer */
        ਢੇਰ_caps_free( px_Event_ਸਮੂਹ_ਬਫਰ );
    }

#ਜੇ_ਅੰਤ /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/
