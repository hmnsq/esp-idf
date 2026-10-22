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

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/event_groups.h"
#ಅನ್ತರ್ಭಾವ "freertos/idf_additions.h"
#ಅನ್ತರ್ಭಾವ "esp_heap_caps.h"

/* ------------------------------ Event Groups ------------------------------ */

#ಆದರೆ ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 )

    Event_ಗುಂಪು_Handle_t x_Event_ಗುಂಪು_Create_With_Caps( UBaseType_t ux_ಸ್ಮರಣೆ_Caps )
    {
        Event_ಗುಂಪು_Handle_t x_Event_ಗುಂಪು;
        Static_Event_ಗುಂಪು_t * px_Event_ಗುಂಪು_ಬಫರ್;

        /* Allocate memory for the event group using the provided memory caps */
        px_Event_ಗುಂಪು_ಬಫರ್ = ರಾಶಿ_caps_malloc( ಗಾತ್ರ( Static_Event_ಗುಂಪು_t ), ux_ಸ್ಮರಣೆ_Caps );

        ಆದರೆ( px_Event_ಗುಂಪು_ಬಫರ್ == ಖಾಲಿ )
        {
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }

        /* Create the event group using static creation API */
        x_Event_ಗುಂಪು = x_Event_ಗುಂಪು_Create_Static( px_Event_ಗುಂಪು_ಬಫರ್ );

        ಆದರೆ( x_Event_ಗುಂಪು == ಖಾಲಿ )
        {
            ರಾಶಿ_caps_free( px_Event_ಗುಂಪು_ಬಫರ್ );
        }

        ಹಿಂತಿರುಗಿ x_Event_ಗುಂಪು;
    }

#ಆದರೆ_ಅಂತ್ಯ /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#ಆದರೆ ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 )

    ಶೂನ್ಯ v_Event_ಗುಂಪು_Delete_With_Caps( Event_ಗುಂಪು_Handle_t x_Event_ಗುಂಪು )
    {
        // Return value unused if asserts are disabled
        ತಳ_Type_t __attribute__((ಬಳಸದ)) x_ಫಲಿತಾಂಶ;
        Static_Event_ಗುಂಪು_t * px_Event_ಗುಂಪು_ಬಫರ್;

        /* Retrieve the buffer used to create the event group before deleting it
         * */
        x_ಫಲಿತಾಂಶ = x_Event_ಗುಂಪು_Get_Static_ಬಫರ್( x_Event_ಗುಂಪು, &px_Event_ಗುಂಪು_ಬಫರ್ );
        configASSERT( x_ಫಲಿತಾಂಶ == pdTRUE );

        /* Delete the event group */
        v_Event_ಗುಂಪು_Delete( x_Event_ಗುಂಪು );

        /* Free the memory buffer */
        ರಾಶಿ_caps_free( px_Event_ಗುಂಪು_ಬಫರ್ );
    }

#ಆದರೆ_ಅಂತ್ಯ /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/
