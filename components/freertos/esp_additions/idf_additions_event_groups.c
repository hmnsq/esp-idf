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

#إذا ( config_SUPPORT_STATIC_ورهاست == 1 )

    Event_گروهه_Handle_t x_Event_گروهه_Create_With_Caps( UBaseType_t ux_يادگيري_Caps )
    {
        Event_گروهه_Handle_t x_Event_گروهه;
        Static_Event_گروهه_t * px_Event_گروهه_بفر;

        /* Allocate memory for the event group using the provided memory caps */
        px_Event_گروهه_بفر = ڍير_caps_malloc( حجم( Static_Event_گروهه_t ), ux_يادگيري_Caps );

        إذا( px_Event_گروهه_بفر == عدم )
        {
            أرجع عدم;
        }

        /* Create the event group using static creation API */
        x_Event_گروهه = x_Event_گروهه_Create_Static( px_Event_گروهه_بفر );

        إذا( x_Event_گروهه == عدم )
        {
            ڍير_caps_free( px_Event_گروهه_بفر );
        }

        أرجع x_Event_گروهه;
    }

#جيڪڏهن_پورو /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_ورهاست == 1 )

    فراغ v_Event_گروهه_Delete_With_Caps( Event_گروهه_Handle_t x_Event_گروهه )
    {
        // Return value unused if asserts are disabled
        بنياد_Type_t __attribute__((اڻ_استعمال_ٿيل)) x_نتيجو;
        Static_Event_گروهه_t * px_Event_گروهه_بفر;

        /* Retrieve the buffer used to create the event group before deleting it
         * */
        x_نتيجو = x_Event_گروهه_Get_Static_بفر( x_Event_گروهه, &px_Event_گروهه_بفر );
        configASSERT( x_نتيجو == pdTRUE );

        /* Delete the event group */
        v_Event_گروهه_Delete( x_Event_گروهه );

        /* Free the memory buffer */
        ڍير_caps_free( px_Event_گروهه_بفر );
    }

#جيڪڏهن_پورو /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/
