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

#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/event_groups.h"
#అన్తర్భావ "freertos/idf_additions.h"
#అన్తర్భావ "esp_heap_caps.h"

/* ------------------------------ Event Groups ------------------------------ */

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

    Event_సమూహం_Handle_t x_Event_సమూహం_Create_With_Caps( UBaseType_t ux_జ్ఞాపకం_Caps )
    {
        Event_సమూహం_Handle_t x_Event_సమూహం;
        Static_Event_సమూహం_t * px_Event_సమూహం_బఫరు;

        /* Allocate memory for the event group using the provided memory caps */
        px_Event_సమూహం_బఫరు = కుప్ప_caps_malloc( పరిమాణం( Static_Event_సమూహం_t ), ux_జ్ఞాపకం_Caps );

        ఒకవేళ( px_Event_సమూహం_బఫరు == ఖాళీ )
        {
            తిరిగి ఖాళీ;
        }

        /* Create the event group using static creation API */
        x_Event_సమూహం = x_Event_సమూహం_Create_Static( px_Event_సమూహం_బఫరు );

        ఒకవేళ( x_Event_సమూహం == ఖాళీ )
        {
            కుప్ప_caps_free( px_Event_సమూహం_బఫరు );
        }

        తిరిగి x_Event_సమూహం;
    }

#అయితే_ముగింపు /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

    శూన్య v_Event_సమూహం_Delete_With_Caps( Event_సమూహం_Handle_t x_Event_సమూహం )
    {
        // Return value unused if asserts are disabled
        పునాది_Type_t __attribute__((వాడని)) x_ఫలితం;
        Static_Event_సమూహం_t * px_Event_సమూహం_బఫరు;

        /* Retrieve the buffer used to create the event group before deleting it
         * */
        x_ఫలితం = x_Event_సమూహం_Get_Static_బఫరు( x_Event_సమూహం, &px_Event_సమూహం_బఫరు );
        configASSERT( x_ఫలితం == pdTRUE );

        /* Delete the event group */
        v_Event_సమూహం_Delete( x_Event_సమూహం );

        /* Free the memory buffer */
        కుప్ప_caps_free( px_Event_సమూహం_బఫరు );
    }

#అయితే_ముగింపు /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/
