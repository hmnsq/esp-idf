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

#அந்தர்பாவ "sdkconfig.h"
#அந்தர்பாவ "freertos/FreeRTOS.h"
#அந்தர்பாவ "freertos/event_groups.h"
#அந்தர்பாவ "freertos/idf_additions.h"
#அந்தர்பாவ "esp_heap_caps.h"

/* ------------------------------ Event Groups ------------------------------ */

#எனில் ( configSUPPORT_STATIC_ALLOCATION == 1 )

    EventGroupHandle_t xEventGroupCreateWithCaps( UBaseType_t uxMemoryCaps )
    {
        EventGroupHandle_t xEventGroup;
        StaticEventGroup_t * pxEventGroupBuffer;

        /* Allocate memory for the event group using the provided memory caps */
        pxEventGroupBuffer = heap_caps_malloc( அளவு( StaticEventGroup_t ), uxMemoryCaps );

        எனில்( pxEventGroupBuffer == வெற்று )
        {
            திரும்பு வெற்று;
        }

        /* Create the event group using static creation API */
        xEventGroup = xEventGroupCreateStatic( pxEventGroupBuffer );

        எனில்( xEventGroup == வெற்று )
        {
            heap_caps_free( pxEventGroupBuffer );
        }

        திரும்பு xEventGroup;
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#எனில் ( configSUPPORT_STATIC_ALLOCATION == 1 )

    வெறுமை vEventGroupDeleteWithCaps( EventGroupHandle_t xEventGroup )
    {
        // Return value unused if asserts are disabled
        BaseType_t __attribute__((unused)) xResult;
        StaticEventGroup_t * pxEventGroupBuffer;

        /* Retrieve the buffer used to create the event group before deleting it
         * */
        xResult = xEventGroupGetStaticBuffer( xEventGroup, &pxEventGroupBuffer );
        configASSERT( xResult == pdTRUE );

        /* Delete the event group */
        vEventGroupDelete( xEventGroup );

        /* Free the memory buffer */
        heap_caps_free( pxEventGroupBuffer );
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/
