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

#إذا ( config_SUPPORT_STATIC_تخصیص == 1 )

    Event_گروه_Handle_t x_Event_گروه_Create_With_Caps( UBaseType_t ux_حافظه_Caps )
    {
        Event_گروه_Handle_t x_Event_گروه;
        Static_Event_گروه_t * px_Event_گروه_میانگیر;

        /* Allocate memory for the event group using the provided memory caps */
        px_Event_گروه_میانگیر = انباشته_caps_malloc( حجم( Static_Event_گروه_t ), ux_حافظه_Caps );

        إذا( px_Event_گروه_میانگیر == عدم )
        {
            أرجع عدم;
        }

        /* Create the event group using static creation API */
        x_Event_گروه = x_Event_گروه_Create_Static( px_Event_گروه_میانگیر );

        إذا( x_Event_گروه == عدم )
        {
            انباشته_caps_free( px_Event_گروه_میانگیر );
        }

        أرجع x_Event_گروه;
    }

#پایان_اگر /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_تخصیص == 1 )

    فراغ v_Event_گروه_Delete_With_Caps( Event_گروه_Handle_t x_Event_گروه )
    {
        // Return value unused if asserts are disabled
        پایه_Type_t __attribute__((استفادهنشده)) x_نتیجه;
        Static_Event_گروه_t * px_Event_گروه_میانگیر;

        /* Retrieve the buffer used to create the event group before deleting it
         * */
        x_نتیجه = x_Event_گروه_Get_Static_میانگیر( x_Event_گروه, &px_Event_گروه_میانگیر );
        configASSERT( x_نتیجه == pdTRUE );

        /* Delete the event group */
        v_Event_گروه_Delete( x_Event_گروه );

        /* Free the memory buffer */
        انباشته_caps_free( px_Event_گروه_میانگیر );
    }

#پایان_اگر /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/
