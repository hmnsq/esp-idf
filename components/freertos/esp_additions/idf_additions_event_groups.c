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

#شامل "sdkconfig.h"
#شامل "freertos/FreeRTOS.h"
#شامل "freertos/event_groups.h"
#شامل "freertos/idf_additions.h"
#شامل "esp_heap_caps.h"

/* ------------------------------ Event Groups ------------------------------ */

#اگر ( config_SUPPORT_STATIC_تخصیص == 1 )

    Event_گروہ_Handle_t x_Event_گروہ_Create_With_Caps( UBaseType_t ux_حافظہ_Caps )
    {
        Event_گروہ_Handle_t x_Event_گروہ;
        Static_Event_گروہ_t * px_Event_گروہ_بفر;

        /* Allocate memory for the event group using the provided memory caps */
        px_Event_گروہ_بفر = ڈھیر_caps_malloc( حجم( Static_Event_گروہ_t ), ux_حافظہ_Caps );

        اگر( px_Event_گروہ_بفر == تہی )
        {
            واپس تہی;
        }

        /* Create the event group using static creation API */
        x_Event_گروہ = x_Event_گروہ_Create_Static( px_Event_گروہ_بفر );

        اگر( x_Event_گروہ == تہی )
        {
            ڈھیر_caps_free( px_Event_گروہ_بفر );
        }

        واپس x_Event_گروہ;
    }

#اگر_ختم /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#اگر ( config_SUPPORT_STATIC_تخصیص == 1 )

    خالی v_Event_گروہ_Delete_With_Caps( Event_گروہ_Handle_t x_Event_گروہ )
    {
        // Return value unused if asserts are disabled
        بنیاد_Type_t __attribute__((غیر_مستعمل)) x_نتیجہ;
        Static_Event_گروہ_t * px_Event_گروہ_بفر;

        /* Retrieve the buffer used to create the event group before deleting it
         * */
        x_نتیجہ = x_Event_گروہ_Get_Static_بفر( x_Event_گروہ, &px_Event_گروہ_بفر );
        configASSERT( x_نتیجہ == pdTRUE );

        /* Delete the event group */
        v_Event_گروہ_Delete( x_Event_گروہ );

        /* Free the memory buffer */
        ڈھیر_caps_free( px_Event_گروہ_بفر );
    }

#اگر_ختم /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/
