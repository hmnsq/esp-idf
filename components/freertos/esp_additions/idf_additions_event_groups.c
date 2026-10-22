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

#അന്തര്ഭാവ "sdkconfig.h"
#അന്തര്ഭാവ "freertos/FreeRTOS.h"
#അന്തര്ഭാവ "freertos/event_groups.h"
#അന്തര്ഭാവ "freertos/idf_additions.h"
#അന്തര്ഭാവ "esp_heap_caps.h"

/* ------------------------------ Event Groups ------------------------------ */

#എങ്കിൽ ( configSUPPORT_STATIC_ALLOCATION == 1 )

    EventGroupHandle_t xEventGroupCreateWithCaps( UBaseType_t uxMemoryCaps )
    {
        EventGroupHandle_t xEventGroup;
        StaticEventGroup_t * pxEventGroupBuffer;

        /* Allocate memory for the event group using the provided memory caps */
        pxEventGroupBuffer = heap_caps_malloc( വലുപ്പം( StaticEventGroup_t ), uxMemoryCaps );

        എങ്കിൽ( pxEventGroupBuffer == ശൂന്യ )
        {
            തിരികെ ശൂന്യ;
        }

        /* Create the event group using static creation API */
        xEventGroup = xEventGroupCreateStatic( pxEventGroupBuffer );

        എങ്കിൽ( xEventGroup == ശൂന്യ )
        {
            heap_caps_free( pxEventGroupBuffer );
        }

        തിരികെ xEventGroup;
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#എങ്കിൽ ( configSUPPORT_STATIC_ALLOCATION == 1 )

    ശൂന്യം vEventGroupDeleteWithCaps( EventGroupHandle_t xEventGroup )
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
