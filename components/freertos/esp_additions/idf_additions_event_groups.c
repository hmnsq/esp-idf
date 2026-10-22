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

#ضمن "sdkconfig.h"
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/event_groups.h"
#ضمن "freertos/idf_additions.h"
#ضمن "esp_heap_caps.h"

/* ------------------------------ Event Groups ------------------------------ */

#إذا ( config_SUPPORT_STATIC_ویش == 1 )

    Event_ډله_Handle_t x_Event_ډله_Create_With_Caps( UBaseType_t ux_حافظه_Caps )
    {
        Event_ډله_Handle_t x_Event_ډله;
        Static_Event_ډله_t * px_Event_ډله_بفر;

        /* Allocate memory for the event group using the provided memory caps */
        px_Event_ډله_بفر = ډېرۍ_caps_malloc( حجم( Static_Event_ډله_t ), ux_حافظه_Caps );

        إذا( px_Event_ډله_بفر == عدم )
        {
            أرجع عدم;
        }

        /* Create the event group using static creation API */
        x_Event_ډله = x_Event_ډله_Create_Static( px_Event_ډله_بفر );

        إذا( x_Event_ډله == عدم )
        {
            ډېرۍ_caps_free( px_Event_ډله_بفر );
        }

        أرجع x_Event_ډله;
    }

#د_که_پای /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_ویش == 1 )

    فراغ v_Event_ډله_Delete_With_Caps( Event_ډله_Handle_t x_Event_ډله )
    {
        // Return value unused if asserts are disabled
        بنسټ_Type_t __attribute__((ناکارول_شوی)) x_پایله;
        Static_Event_ډله_t * px_Event_ډله_بفر;

        /* Retrieve the buffer used to create the event group before deleting it
         * */
        x_پایله = x_Event_ډله_Get_Static_بفر( x_Event_ډله, &px_Event_ډله_بفر );
        configASSERT( x_پایله == pdTRUE );

        /* Delete the event group */
        v_Event_ډله_Delete( x_Event_ډله );

        /* Free the memory buffer */
        ډېرۍ_caps_free( px_Event_ډله_بفر );
    }

#د_که_پای /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/
