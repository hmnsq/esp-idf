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

#إذا ( config_SUPPORT_STATIC_ونڈ == 1 )

    Event_گروہ_Handle_t x_Event_گروہ_Create_With_Caps( UBaseType_t ux_حافظہ_Caps )
    {
        Event_گروہ_Handle_t x_Event_گروہ;
        Static_Event_گروہ_t * px_Event_گروہ_بفر;

        /* Allocate memory for the event group using the provided memory caps */
        px_Event_گروہ_بفر = ڈھیر_caps_malloc( حجم( Static_Event_گروہ_t ), ux_حافظہ_Caps );

        إذا( px_Event_گروہ_بفر == عدم )
        {
            أرجع عدم;
        }

        /* Create the event group using static creation API */
        x_Event_گروہ = x_Event_گروہ_Create_Static( px_Event_گروہ_بفر );

        إذا( x_Event_گروہ == عدم )
        {
            ڈھیر_caps_free( px_Event_گروہ_بفر );
        }

        أرجع x_Event_گروہ;
    }

#جے_انت /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_ونڈ == 1 )

    فراغ v_Event_گروہ_Delete_With_Caps( Event_گروہ_Handle_t x_Event_گروہ )
    {
        // Return value unused if asserts are disabled
        ادھار_Type_t __attribute__((بن_ورتیا)) x_نتیجہ;
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

#جے_انت /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/
