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

#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/event_groups.h"
#अन्तर्भाव "freertos/idf_additions.h"
#अन्तर्भाव "esp_heap_caps.h"

/* ------------------------------ Event Groups ------------------------------ */

#यदि ( config_SUPPORT_STATIC_आवण्टनम् == 1 )

    Event_गणः_Handle_t x_Event_गणः_Create_With_Caps( UBaseType_t ux_स्मृतिः_Caps )
    {
        Event_गणः_Handle_t x_Event_गणः;
        Static_Event_गणः_t * px_Event_गणः_बफर;

        /* Allocate memory for the event group using the provided memory caps */
        px_Event_गणः_बफर = heap_caps_malloc( परिमाण( Static_Event_गणः_t ), ux_स्मृतिः_Caps );

        यदि( px_Event_गणः_बफर == रिक्त )
        {
            प्रत्यागम रिक्त;
        }

        /* Create the event group using static creation API */
        x_Event_गणः = x_Event_गणः_Create_Static( px_Event_गणः_बफर );

        यदि( x_Event_गणः == रिक्त )
        {
            heap_caps_free( px_Event_गणः_बफर );
        }

        प्रत्यागम x_Event_गणः;
    }

#यदि_अन्तः /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#यदि ( config_SUPPORT_STATIC_आवण्टनम् == 1 )

    शून्य v_Event_गणः_Delete_With_Caps( Event_गणः_Handle_t x_Event_गणः )
    {
        // Return value unused if asserts are disabled
        आधारः_Type_t __attribute__((अप्रयुक्तम्)) x_परिणाम;
        Static_Event_गणः_t * px_Event_गणः_बफर;

        /* Retrieve the buffer used to create the event group before deleting it
         * */
        x_परिणाम = x_Event_गणः_Get_Static_बफर( x_Event_गणः, &px_Event_गणः_बफर );
        configASSERT( x_परिणाम == pdTRUE );

        /* Delete the event group */
        v_Event_गणः_Delete( x_Event_गणः );

        /* Free the memory buffer */
        heap_caps_free( px_Event_गणः_बफर );
    }

#यदि_अन्तः /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/
