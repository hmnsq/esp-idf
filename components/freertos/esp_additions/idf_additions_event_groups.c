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

#समावेश "sdkconfig.h"
#समावेश "freertos/FreeRTOS.h"
#समावेश "freertos/event_groups.h"
#समावेश "freertos/idf_additions.h"
#समावेश "esp_heap_caps.h"

/* ------------------------------ Event Groups ------------------------------ */

#अगर ( configSUPPORT_STATIC_ALLOCATION == 1 )

    EventGroupHandle_t xEventGroupCreateWithCaps( UBaseType_t uxMemoryCaps )
    {
        EventGroupHandle_t xEventGroup;
        StaticEventGroup_t * px_Event_Group_बफर;

        /* Allocate memory for the event group using the provided memory caps */
        px_Event_Group_बफर = heap_caps_malloc( माप( StaticEventGroup_t ), uxMemoryCaps );

        अगर( px_Event_Group_बफर == शून्य )
        {
            वापस शून्य;
        }

        /* Create the event group using static creation API */
        xEventGroup = xEventGroupCreateStatic( px_Event_Group_बफर );

        अगर( xEventGroup == शून्य )
        {
            heap_caps_free( px_Event_Group_बफर );
        }

        वापस xEventGroup;
    }

#पूर्ण_अगर /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#अगर ( configSUPPORT_STATIC_ALLOCATION == 1 )

    व्योम vEventGroupDeleteWithCaps( EventGroupHandle_t xEventGroup )
    {
        // Return value unused if asserts are disabled
        BaseType_t __attribute__((unused)) x_परिणाम;
        StaticEventGroup_t * px_Event_Group_बफर;

        /* Retrieve the buffer used to create the event group before deleting it
         * */
        x_परिणाम = x_Event_Group_Get_Static_बफर( xEventGroup, &px_Event_Group_बफर );
        configASSERT( x_परिणाम == pdTRUE );

        /* Delete the event group */
        vEventGroupDelete( xEventGroup );

        /* Free the memory buffer */
        heap_caps_free( px_Event_Group_बफर );
    }

#पूर्ण_अगर /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/
