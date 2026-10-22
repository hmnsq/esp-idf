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

#অন্তর্ভাব "sdkconfig.h"
#অন্তর্ভাব "freertos/FreeRTOS.h"
#অন্তর্ভাব "freertos/event_groups.h"
#অন্তর্ভাব "freertos/idf_additions.h"
#অন্তর্ভাব "esp_heap_caps.h"

/* ------------------------------ Event Groups ------------------------------ */

#যদি ( configSUPPORT_STATIC_ALLOCATION == 1 )

    EventGroupHandle_t xEventGroupCreateWithCaps( UBaseType_t uxMemoryCaps )
    {
        EventGroupHandle_t xEventGroup;
        StaticEventGroup_t * px_Event_Group_বাফার;

        /* Allocate memory for the event group using the provided memory caps */
        px_Event_Group_বাফার = heap_caps_malloc( আকার( StaticEventGroup_t ), uxMemoryCaps );

        যদি( px_Event_Group_বাফার == ফাঁকা )
        {
            ফেরত ফাঁকা;
        }

        /* Create the event group using static creation API */
        xEventGroup = xEventGroupCreateStatic( px_Event_Group_বাফার );

        যদি( xEventGroup == ফাঁকা )
        {
            heap_caps_free( px_Event_Group_বাফার );
        }

        ফেরত xEventGroup;
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#যদি ( configSUPPORT_STATIC_ALLOCATION == 1 )

    শূন্য vEventGroupDeleteWithCaps( EventGroupHandle_t xEventGroup )
    {
        // Return value unused if asserts are disabled
        BaseType_t __attribute__((unused)) x_ফলাফল;
        StaticEventGroup_t * px_Event_Group_বাফার;

        /* Retrieve the buffer used to create the event group before deleting it
         * */
        x_ফলাফল = x_Event_Group_Get_Static_বাফার( xEventGroup, &px_Event_Group_বাফার );
        configASSERT( x_ফলাফল == pdTRUE );

        /* Delete the event group */
        vEventGroupDelete( xEventGroup );

        /* Free the memory buffer */
        heap_caps_free( px_Event_Group_বাফার );
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/
