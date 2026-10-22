/*
 * FreeRTOS Kernel V11.1.0
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * SPDX-FileCopyrightText: 2021 Amazon.com, Inc. or its affiliates
 *
 * SPDX-License-Identifier: MIT
 *
 * SPDX-FileContributor: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/* Standard includes. */
#ಅನ್ತರ್ಭಾವ <string.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#ವ್ಯಾಖ್ಯಾನಿಸು MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* FreeRTOS includes. */
#ಅನ್ತರ್ಭಾವ "FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "task.h"
#ಅನ್ತರ್ಭಾವ "stream_buffer.h"

#ಆದರೆ ( configUSE_TASK_NOTIFICATIONS != 1 )
    #ದೋಷ configUSE_TASK_NOTIFICATIONS must be ಗಣ ಗಮ್ಯ 1 ಗಮ್ಯ build ಹರಿವು_ಬಫರ್.c
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ( INCLUDE_xTaskGetCurrentTaskHandle != 1 )
    #ದೋಷ INCLUDE_xTaskGetCurrentTaskHandle must be ಗಣ ಗಮ್ಯ 1 ಗಮ್ಯ build ಹರಿವು_ಬಫರ್.c
#ಆದರೆ_ಅಂತ್ಯ

/* The MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* This entire source file will be skipped if the application is not configured
 * to include stream buffer functionality. This #if is closed at the very bottom
 * of this file. If you want to include stream buffers then ensure
 * configUSE_STREAM_BUFFERS is set to 1 in FreeRTOSConfig.h. */
#ಆದರೆ ( config_USE_ಹರಿವು_BUFFERS == 1 )

/* If the user has not provided application specific Rx notification macros,
 * or #defined the notification macros away, then provide default implementations
 * that uses task notifications. */
    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ sbRECEIVE_COMPLETED
        #ವ್ಯಾಖ್ಯಾನಿಸು sbRECEIVE_COMPLETED( px_ಹರಿವು_ಬಫರ್ )                                 \
    ಮಾಡು                                                                                \
    {                                                                                 \
        vTaskSuspendAll();                                                            \
        {                                                                             \
            ಆದರೆ( ( px_ಹರಿವು_ಬಫರ್ )->x_Task_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send != ಖಾಲಿ )                      \
            {                                                                         \
                ( ಶೂನ್ಯ ) x_Task_Notify_ಸೂಚ್ಯಂಕಿತ( ( px_ಹರಿವು_ಬಫರ್ )->x_Task_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send,  \
                                             ( px_ಹರಿವು_ಬಫರ್ )->ux_Notification_ಸೂಚ್ಯಂಕ, \
                                             ( uint32_t ) 0,                          \
                                             eNoAction );                             \
                ( px_ಹರಿವು_ಬಫರ್ )->x_Task_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send = ಖಾಲಿ;                        \
            }                                                                         \
        }                                                                             \
        ( ಶೂನ್ಯ ) xTaskResumeAll();                                                    \
    } ಆಗ( 0 )
    #ಆದರೆ_ಅಂತ್ಯ /* sbRECEIVE_COMPLETED */

/* If user has provided a per-instance receive complete callback, then
 * invoke the callback else use the receive complete macro which is provided by default for all instances.
 */
    #ಆದರೆ ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        #ವ್ಯಾಖ್ಯಾನಿಸು prvRECEIVE_COMPLETED( px_ಹರಿವು_ಬಫರ್ )                                           \
    ಮಾಡು {                                                                                         \
        ಆದರೆ( ( px_ಹರಿವು_ಬಫರ್ )->pxReceiveCompletedCallback != ಖಾಲಿ )                             \
        {                                                                                        \
            ( px_ಹರಿವು_ಬಫರ್ )->pxReceiveCompletedCallback( ( px_ಹರಿವು_ಬಫರ್ ), pdFALSE, ಖಾಲಿ ); \
        }                                                                                        \
        ಇಲ್ಲದಿದ್ದರೆ                                                                                     \
        {                                                                                        \
            sbRECEIVE_COMPLETED( ( px_ಹರಿವು_ಬಫರ್ ) );                                           \
        }                                                                                        \
    } ಆಗ( 0 )
    #ಇಲ್ಲದಿದ್ದರೆ /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
        #ವ್ಯಾಖ್ಯಾನಿಸು prvRECEIVE_COMPLETED( px_ಹರಿವು_ಬಫರ್ )    sbRECEIVE_COMPLETED( ( px_ಹರಿವು_ಬಫರ್ ) )
    #ಆದರೆ_ಅಂತ್ಯ /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ sbRECEIVE_COMPLETED_FROM_ISR
        #ವ್ಯಾಖ್ಯಾನಿಸು sbRECEIVE_COMPLETED_FROM_ISR( px_ಹರಿವು_ಬಫರ್,                                \
                                              px_Higher_ಆದ್ಯತೆ_Task_Woken )                    \
    ಮಾಡು {                                                                                     \
        UBaseType_t ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ;                                                  \
                                                                                             \
        ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ = taskENTER_CRITICAL_FROM_ISR();                              \
        {                                                                                    \
            ಆದರೆ( ( px_ಹರಿವು_ಬಫರ್ )->x_Task_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send != ಖಾಲಿ )                             \
            {                                                                                \
                ( ಶೂನ್ಯ ) x_Task_Notify_ಸೂಚ್ಯಂಕಿತ_From_ISR( ( px_ಹರಿವು_ಬಫರ್ )->x_Task_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send,  \
                                                    ( px_ಹರಿವು_ಬಫರ್ )->ux_Notification_ಸೂಚ್ಯಂಕ, \
                                                    ( uint32_t ) 0,                          \
                                                    eNoAction,                               \
                                                    ( px_Higher_ಆದ್ಯತೆ_Task_Woken ) );         \
                ( px_ಹರಿವು_ಬಫರ್ )->x_Task_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send = ಖಾಲಿ;                               \
            }                                                                                \
        }                                                                                    \
        taskEXIT_CRITICAL_FROM_ISR( ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ );                                \
    } ಆಗ( 0 )
    #ಆದರೆ_ಅಂತ್ಯ /* sbRECEIVE_COMPLETED_FROM_ISR */

    #ಆದರೆ ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        #ವ್ಯಾಖ್ಯಾನಿಸು prvRECEIVE_COMPLETED_FROM_ISR( px_ಹರಿವು_ಬಫರ್,                                                           \
                                               px_Higher_ಆದ್ಯತೆ_Task_Woken )                                               \
    ಮಾಡು {                                                                                                                 \
        ಆದರೆ( ( px_ಹರಿವು_ಬಫರ್ )->pxReceiveCompletedCallback != ಖಾಲಿ )                                                     \
        {                                                                                                                \
            ( px_ಹರಿವು_ಬಫರ್ )->pxReceiveCompletedCallback( ( px_ಹರಿವು_ಬಫರ್ ), pdTRUE, ( px_Higher_ಆದ್ಯತೆ_Task_Woken ) ); \
        }                                                                                                                \
        ಇಲ್ಲದಿದ್ದರೆ                                                                                                             \
        {                                                                                                                \
            sbRECEIVE_COMPLETED_FROM_ISR( ( px_ಹರಿವು_ಬಫರ್ ), ( px_Higher_ಆದ್ಯತೆ_Task_Woken ) );                           \
        }                                                                                                                \
    } ಆಗ( 0 )
    #ಇಲ್ಲದಿದ್ದರೆ /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
        #ವ್ಯಾಖ್ಯಾನಿಸು prvRECEIVE_COMPLETED_FROM_ISR( px_ಹರಿವು_ಬಫರ್, px_Higher_ಆದ್ಯತೆ_Task_Woken ) \
    sbRECEIVE_COMPLETED_FROM_ISR( ( px_ಹರಿವು_ಬಫರ್ ), ( px_Higher_ಆದ್ಯತೆ_Task_Woken ) )
    #ಆದರೆ_ಅಂತ್ಯ /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

/* If the user has not provided an application specific Tx notification macro,
 * or #defined the notification macro away, then provide a default
 * implementation that uses task notifications.
 */
    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ sbSEND_COMPLETED
        #ವ್ಯಾಖ್ಯಾನಿಸು sbSEND_COMPLETED( px_ಹರಿವು_ಬಫರ್ )                                  \
    vTaskSuspendAll();                                                              \
    {                                                                               \
        ಆದರೆ( ( px_ಹರಿವು_ಬಫರ್ )->x_Task_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive != ಖಾಲಿ )                     \
        {                                                                           \
            ( ಶೂನ್ಯ ) x_Task_Notify_ಸೂಚ್ಯಂಕಿತ( ( px_ಹರಿವು_ಬಫರ್ )->x_Task_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive, \
                                         ( px_ಹರಿವು_ಬಫರ್ )->ux_Notification_ಸೂಚ್ಯಂಕ,   \
                                         ( uint32_t ) 0,                            \
                                         eNoAction );                               \
            ( px_ಹರಿವು_ಬಫರ್ )->x_Task_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive = ಖಾಲಿ;                       \
        }                                                                           \
    }                                                                               \
    ( ಶೂನ್ಯ ) xTaskResumeAll()
    #ಆದರೆ_ಅಂತ್ಯ /* sbSEND_COMPLETED */

/* If user has provided a per-instance send completed callback, then
 * invoke the callback else use the send complete macro which is provided by default for all instances.
 */
    #ಆದರೆ ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        #ವ್ಯಾಖ್ಯಾನಿಸು prvSEND_COMPLETED( px_ಹರಿವು_ಬಫರ್ )                                           \
    ಮಾಡು {                                                                                      \
        ಆದರೆ( ( px_ಹರಿವು_ಬಫರ್ )->pxSendCompletedCallback != ಖಾಲಿ )                             \
        {                                                                                     \
            ( px_ಹರಿವು_ಬಫರ್ )->pxSendCompletedCallback( ( px_ಹರಿವು_ಬಫರ್ ), pdFALSE, ಖಾಲಿ ); \
        }                                                                                     \
        ಇಲ್ಲದಿದ್ದರೆ                                                                                  \
        {                                                                                     \
            sbSEND_COMPLETED( ( px_ಹರಿವು_ಬಫರ್ ) );                                           \
        }                                                                                     \
    } ಆಗ( 0 )
    #ಇಲ್ಲದಿದ್ದರೆ /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
        #ವ್ಯಾಖ್ಯಾನಿಸು prvSEND_COMPLETED( px_ಹರಿವು_ಬಫರ್ )    sbSEND_COMPLETED( ( px_ಹರಿವು_ಬಫರ್ ) )
    #ಆದರೆ_ಅಂತ್ಯ /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */


    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ sbSEND_COMPLETE_FROM_ISR
        #ವ್ಯಾಖ್ಯಾನಿಸು sbSEND_COMPLETE_FROM_ISR( px_ಹರಿವು_ಬಫರ್, px_Higher_ಆದ್ಯತೆ_Task_Woken )          \
    ಮಾಡು {                                                                                       \
        UBaseType_t ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ;                                                    \
                                                                                               \
        ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ = taskENTER_CRITICAL_FROM_ISR();                                \
        {                                                                                      \
            ಆದರೆ( ( px_ಹರಿವು_ಬಫರ್ )->x_Task_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive != ಖಾಲಿ )                            \
            {                                                                                  \
                ( ಶೂನ್ಯ ) x_Task_Notify_ಸೂಚ್ಯಂಕಿತ_From_ISR( ( px_ಹರಿವು_ಬಫರ್ )->x_Task_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive, \
                                                    ( px_ಹರಿವು_ಬಫರ್ )->ux_Notification_ಸೂಚ್ಯಂಕ,   \
                                                    ( uint32_t ) 0,                            \
                                                    eNoAction,                                 \
                                                    ( px_Higher_ಆದ್ಯತೆ_Task_Woken ) );           \
                ( px_ಹರಿವು_ಬಫರ್ )->x_Task_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive = ಖಾಲಿ;                              \
            }                                                                                  \
        }                                                                                      \
        taskEXIT_CRITICAL_FROM_ISR( ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ );                                  \
    } ಆಗ( 0 )
    #ಆದರೆ_ಅಂತ್ಯ /* sbSEND_COMPLETE_FROM_ISR */


    #ಆದರೆ ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        #ವ್ಯಾಖ್ಯಾನಿಸು prvSEND_COMPLETE_FROM_ISR( px_ಹರಿವು_ಬಫರ್, px_Higher_ಆದ್ಯತೆ_Task_Woken )                                \
    ಮಾಡು {                                                                                                              \
        ಆದರೆ( ( px_ಹರಿವು_ಬಫರ್ )->pxSendCompletedCallback != ಖಾಲಿ )                                                     \
        {                                                                                                             \
            ( px_ಹರಿವು_ಬಫರ್ )->pxSendCompletedCallback( ( px_ಹರಿವು_ಬಫರ್ ), pdTRUE, ( px_Higher_ಆದ್ಯತೆ_Task_Woken ) ); \
        }                                                                                                             \
        ಇಲ್ಲದಿದ್ದರೆ                                                                                                          \
        {                                                                                                             \
            sbSEND_COMPLETE_FROM_ISR( ( px_ಹರಿವು_ಬಫರ್ ), ( px_Higher_ಆದ್ಯತೆ_Task_Woken ) );                            \
        }                                                                                                             \
    } ಆಗ( 0 )
    #ಇಲ್ಲದಿದ್ದರೆ /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
        #ವ್ಯಾಖ್ಯಾನಿಸು prvSEND_COMPLETE_FROM_ISR( px_ಹರಿವು_ಬಫರ್, px_Higher_ಆದ್ಯತೆ_Task_Woken ) \
    sbSEND_COMPLETE_FROM_ISR( ( px_ಹರಿವು_ಬಫರ್ ), ( px_Higher_ಆದ್ಯತೆ_Task_Woken ) )
    #ಆದರೆ_ಅಂತ್ಯ /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

/* The number of bytes used to hold the length of a message in the buffer. */
    #ವ್ಯಾಖ್ಯಾನಿಸು sb_ಬೈಟುಗಳು_ಗಮ್ಯ_STORE_ಸಂದೇಶ_ಉದ್ದಳತೆ    ( ಗಾತ್ರ( config_ಸಂದೇಶ_ಬಫರ್_ಉದ್ದಳತೆ_TYPE ) )

/* Bits stored in the ucFlags field of the stream buffer. */
    #ವ್ಯಾಖ್ಯಾನಿಸು sb_FLAGS_IS_ಸಂದೇಶ_ಬಫರ್          ( ( uint8_t ) 1 ) /* Set if the stream buffer was created as a message buffer, in which case it holds discrete messages rather than a stream. */
    #ವ್ಯಾಖ್ಯಾನಿಸು sbFLAGS_IS_STATICALLY_ALLOCATED    ( ( uint8_t ) 2 ) /* Set if the stream buffer was created using statically allocated memory. */
    #ವ್ಯಾಖ್ಯಾನಿಸು sb_FLAGS_IS_BATCHING_ಬಫರ್         ( ( uint8_t ) 4 ) /* Set if the stream buffer was created as a batching buffer, meaning the receiver task will only unblock when the trigger level exceededs. */

/*-----------------------------------------------------------*/

/* Structure that hold state information on the buffer. */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ಹರಿವು_ಬಫರ್_Def_t
{
    ಅಸ್ಥಿರ size_t x_ಬಾಲ;                       /* Index to the next item to read within the buffer. */
    ಅಸ್ಥಿರ size_t x_ತಲೆ;                       /* Index to the next item to write within the buffer. */
    size_t x_ಉದ್ದಳತೆ;                              /* The length of the buffer pointed to by pucBuffer. */
    size_t x_Trigger_Level_ಬೈಟುಗಳು;                   /* The number of bytes that must be in the stream buffer before a task that is waiting for data is unblocked. */
    ಅಸ್ಥಿರ TaskHandle_t x_Task_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive; /* Holds the handle of a task waiting for data, or NULL if no tasks are waiting. */
    ಅಸ್ಥಿರ TaskHandle_t x_Task_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send;    /* Holds the handle of a task waiting to send data to a message buffer that is full. */
    uint8_t * puc_ಬಫರ್;                         /* Points to the buffer itself - that is - the RAM that stores the data passed through the buffer. */
    uint8_t ucFlags;

    #ಆದರೆ ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ; /* Used for tracing purposes. */
    #ಆದರೆ_ಅಂತ್ಯ

    #ಆದರೆ ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        ಹರಿವು_ಬಫರ್_Callback_Function_t pxSendCompletedCallback;    /* Optional callback called on send complete. sbSEND_COMPLETED is called if this is NULL. */
        ಹರಿವು_ಬಫರ್_Callback_Function_t pxReceiveCompletedCallback; /* Optional callback called on receive complete.  sbRECEIVE_COMPLETED is called if this is NULL. */
    #ಆದರೆ_ಅಂತ್ಯ
    UBaseType_t ux_Notification_ಸೂಚ್ಯಂಕ;                               /* The index we are using for notification, by default tskDEFAULT_INDEX_TO_NOTIFY. */
} ಹರಿವು_ಬಫರ್_t;

/*
 * The number of bytes available to be read from the buffer.
 */
ಸ್ಥಿರ size_t prv_ಬೈಟುಗಳು_ಒಳ_ಬಫರ್( ಸ್ಥಿರಾಂಕ ಹರಿವು_ಬಫರ್_t * ಸ್ಥಿರಾಂಕ px_ಹರಿವು_ಬಫರ್ ) PRIVILEGED_FUNCTION;

/*
 * Add xCount bytes from pucData into the pxStreamBuffer's data storage area.
 * This function does not update the buffer's xHead pointer, so multiple writes
 * may be chained together "atomically". This is useful for Message Buffers where
 * the length and data bytes are written in two separate chunks, and we don't want
 * the reader to see the buffer as having grown until after all data is copied over.
 * This function takes a custom xHead value to indicate where to write to (necessary
 * for chaining) and returns the the resulting xHead position.
 * To mark the write as complete, manually set the buffer's xHead field with the
 * returned xHead from this function.
 */
ಸ್ಥಿರ size_t prv_Write_ಬೈಟುಗಳು_ಗಮ್ಯ_ಬಫರ್( ಹರಿವು_ಬಫರ್_t * ಸ್ಥಿರಾಂಕ px_ಹರಿವು_ಬಫರ್,
                                     ಸ್ಥಿರಾಂಕ uint8_t * puc_ದತ್ತಾಂಶ,
                                     size_t x_ಎಣಿಕೆ,
                                     size_t x_ತಲೆ ) PRIVILEGED_FUNCTION;

/*
 * If the stream buffer is being used as a message buffer, then reads an entire
 * message out of the buffer.  If the stream buffer is being used as a stream
 * buffer then read as many bytes as possible from the buffer.
 * prvReadBytesFromBuffer() is called to actually extract the bytes from the
 * buffer's data storage area.
 */
ಸ್ಥಿರ size_t prv_Read_ಸಂದೇಶ_From_ಬಫರ್( ಹರಿವು_ಬಫರ್_t * px_ಹರಿವು_ಬಫರ್,
                                        ಶೂನ್ಯ * pv_Rx_ದತ್ತಾಂಶ,
                                        size_t x_ಬಫರ್_ಉದ್ದಳತೆ_ಬೈಟುಗಳು,
                                        size_t x_ಬೈಟುಗಳು_ಲಭ್ಯ ) PRIVILEGED_FUNCTION;

/*
 * If the stream buffer is being used as a message buffer, then writes an entire
 * message to the buffer.  If the stream buffer is being used as a stream
 * buffer then write as many bytes as possible to the buffer.
 * prvWriteBytestoBuffer() is called to actually send the bytes to the buffer's
 * data storage area.
 */
ಸ್ಥಿರ size_t prv_Write_ಸಂದೇಶ_ಗಮ್ಯ_ಬಫರ್( ಹರಿವು_ಬಫರ್_t * ಸ್ಥಿರಾಂಕ px_ಹರಿವು_ಬಫರ್,
                                       ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * pv_Tx_ದತ್ತಾಂಶ,
                                       size_t x_ದತ್ತಾಂಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು,
                                       size_t xSpace,
                                       size_t xRequiredSpace ) PRIVILEGED_FUNCTION;

/*
 * Copies xCount bytes from the pxStreamBuffer's data storage area to pucData.
 * This function does not update the buffer's xTail pointer, so multiple reads
 * may be chained together "atomically". This is useful for Message Buffers where
 * the length and data bytes are read in two separate chunks, and we don't want
 * the writer to see the buffer as having more free space until after all data is
 * copied over, especially if we have to abort the read due to insufficient receiving space.
 * This function takes a custom xTail value to indicate where to read from (necessary
 * for chaining) and returns the the resulting xTail position.
 * To mark the read as complete, manually set the buffer's xTail field with the
 * returned xTail from this function.
 */
ಸ್ಥಿರ size_t prv_Read_ಬೈಟುಗಳು_From_ಬಫರ್( ಹರಿವು_ಬಫರ್_t * px_ಹರಿವು_ಬಫರ್,
                                      uint8_t * puc_ದತ್ತಾಂಶ,
                                      size_t x_ಎಣಿಕೆ,
                                      size_t x_ಬಾಲ ) PRIVILEGED_FUNCTION;

/*
 * Called by both pxStreamBufferCreate() and pxStreamBufferCreateStatic() to
 * initialise the members of the newly created stream buffer structure.
 */
ಸ್ಥಿರ ಶೂನ್ಯ prv_Initialise_New_ಹರಿವು_ಬಫರ್( ಹರಿವು_ಬಫರ್_t * ಸ್ಥಿರಾಂಕ px_ಹರಿವು_ಬಫರ್,
                                          uint8_t * ಸ್ಥಿರಾಂಕ puc_ಬಫರ್,
                                          size_t x_ಬಫರ್_ಗಾತ್ರ_ಬೈಟುಗಳು,
                                          size_t x_Trigger_Level_ಬೈಟುಗಳು,
                                          uint8_t ucFlags,
                                          ಹರಿವು_ಬಫರ್_Callback_Function_t pxSendCompletedCallback,
                                          ಹರಿವು_ಬಫರ್_Callback_Function_t pxReceiveCompletedCallback ) PRIVILEGED_FUNCTION;

/*-----------------------------------------------------------*/
    #ಆದರೆ ( config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ == 1 )
    ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್_Generic_Create( size_t x_ಬಫರ್_ಗಾತ್ರ_ಬೈಟುಗಳು,
                                                     size_t x_Trigger_Level_ಬೈಟುಗಳು,
                                                     ತಳ_Type_t x_ಹರಿವು_ಬಫರ್_Type,
                                                     ಹರಿವು_ಬಫರ್_Callback_Function_t pxSendCompletedCallback,
                                                     ಹರಿವು_ಬಫರ್_Callback_Function_t pxReceiveCompletedCallback )
    {
        ಶೂನ್ಯ * pv_Allocated_ಸ್ಮರಣೆ;
        uint8_t ucFlags;

        trace_ENTER_x_ಹರಿವು_ಬಫರ್_Generic_Create( x_ಬಫರ್_ಗಾತ್ರ_ಬೈಟುಗಳು, x_Trigger_Level_ಬೈಟುಗಳು, x_ಹರಿವು_ಬಫರ್_Type, pxSendCompletedCallback, pxReceiveCompletedCallback );

        /* In case the stream buffer is going to be used as a message buffer
         * (that is, it will hold discrete messages with a little meta data that
         * says how big the next message is) check the buffer will be large enough
         * to hold at least one message. */
        ಆದರೆ( x_ಹರಿವು_ಬಫರ್_Type == sb_TYPE_ಸಂದೇಶ_ಬಫರ್ )
        {
            /* Is a message buffer but not statically allocated. */
            ucFlags = sb_FLAGS_IS_ಸಂದೇಶ_ಬಫರ್;
            configASSERT( x_ಬಫರ್_ಗಾತ್ರ_ಬೈಟುಗಳು > sb_ಬೈಟುಗಳು_ಗಮ್ಯ_STORE_ಸಂದೇಶ_ಉದ್ದಳತೆ );
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( x_ಹರಿವು_ಬಫರ್_Type == sb_TYPE_ಹರಿವು_BATCHING_ಬಫರ್ )
        {
            /* Is a batching buffer but not statically allocated. */
            ucFlags = sb_FLAGS_IS_BATCHING_ಬಫರ್;
            configASSERT( x_ಬಫರ್_ಗಾತ್ರ_ಬೈಟುಗಳು > 0 );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            /* Not a message buffer and not statically allocated. */
            ucFlags = 0;
            configASSERT( x_ಬಫರ್_ಗಾತ್ರ_ಬೈಟುಗಳು > 0 );
        }

        configASSERT( x_Trigger_Level_ಬೈಟುಗಳು <= x_ಬಫರ್_ಗಾತ್ರ_ಬೈಟುಗಳು );

        /* A trigger level of 0 would cause a waiting task to unblock even when
         * the buffer was empty. */
        ಆದರೆ( x_Trigger_Level_ಬೈಟುಗಳು == ( size_t ) 0 )
        {
            x_Trigger_Level_ಬೈಟುಗಳು = ( size_t ) 1;
        }

        /* A stream buffer requires a StreamBuffer_t structure and a buffer.
         * Both are allocated in a single call to pvPortMalloc().  The
         * StreamBuffer_t structure is placed at the start of the allocated memory
         * and the buffer follows immediately after.  The requested size is
         * incremented so the free space is returned as the user would expect -
         * this is a quirk of the implementation that means otherwise the free
         * space would be reported as one byte smaller than would be logically
         * expected. */
        ಆದರೆ( x_ಬಫರ್_ಗಾತ್ರ_ಬೈಟುಗಳು < ( x_ಬಫರ್_ಗಾತ್ರ_ಬೈಟುಗಳು + 1U + ಗಾತ್ರ( ಹರಿವು_ಬಫರ್_t ) ) )
        {
            x_ಬಫರ್_ಗಾತ್ರ_ಬೈಟುಗಳು++;
            pv_Allocated_ಸ್ಮರಣೆ = pv_ಪೋರ್ಟ್_Malloc( x_ಬಫರ್_ಗಾತ್ರ_ಬೈಟುಗಳು + ಗಾತ್ರ( ಹರಿವು_ಬಫರ್_t ) );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            pv_Allocated_ಸ್ಮರಣೆ = ಖಾಲಿ;
        }

        ಆದರೆ( pv_Allocated_ಸ್ಮರಣೆ != ಖಾಲಿ )
        {
            /* MISRA Ref 11.5.1 [Malloc memory assignment] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
            /* coverity[misra_c_2012_rule_11_5_violation] */
            prv_Initialise_New_ಹರಿವು_ಬಫರ್( ( ಹರಿವು_ಬಫರ್_t * ) pv_Allocated_ಸ್ಮರಣೆ,                         /* Structure at the start of the allocated memory. */
                                                                                                          /* MISRA Ref 11.5.1 [Malloc memory assignment] */
                                                                                                          /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
                                                                                                          /* coverity[misra_c_2012_rule_11_5_violation] */
                                          ( ( uint8_t * ) pv_Allocated_ಸ್ಮರಣೆ ) + ಗಾತ್ರ( ಹರಿವು_ಬಫರ್_t ), /* Storage area follows. */
                                          x_ಬಫರ್_ಗಾತ್ರ_ಬೈಟುಗಳು,
                                          x_Trigger_Level_ಬೈಟುಗಳು,
                                          ucFlags,
                                          pxSendCompletedCallback,
                                          pxReceiveCompletedCallback );

            trace_ಹರಿವು_ಬಫರ್_CREATE( ( ( ಹರಿವು_ಬಫರ್_t * ) pv_Allocated_ಸ್ಮರಣೆ ), x_ಹರಿವು_ಬಫರ್_Type );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            trace_ಹರಿವು_ಬಫರ್_CREATE_FAILED( x_ಹರಿವು_ಬಫರ್_Type );
        }

        trace_RETURN_x_ಹರಿವು_ಬಫರ್_Generic_Create( pv_Allocated_ಸ್ಮರಣೆ );

        /* MISRA Ref 11.5.1 [Malloc memory assignment] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
        /* coverity[misra_c_2012_rule_11_5_violation] */
        ಹಿಂತಿರುಗಿ ( ಹರಿವು_ಬಫರ್_Handle_t ) pv_Allocated_ಸ್ಮರಣೆ;
    }
    #ಆದರೆ_ಅಂತ್ಯ /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

    #ಆದರೆ ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 )

    ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್_Generic_Create_Static( size_t x_ಬಫರ್_ಗಾತ್ರ_ಬೈಟುಗಳು,
                                                           size_t x_Trigger_Level_ಬೈಟುಗಳು,
                                                           ತಳ_Type_t x_ಹರಿವು_ಬಫರ್_Type,
                                                           uint8_t * ಸ್ಥಿರಾಂಕ puc_ಹರಿವು_ಬಫರ್_Storage_Area,
                                                           Static_ಹರಿವು_ಬಫರ್_t * ಸ್ಥಿರಾಂಕ px_Static_ಹರಿವು_ಬಫರ್,
                                                           ಹರಿವು_ಬಫರ್_Callback_Function_t pxSendCompletedCallback,
                                                           ಹರಿವು_ಬಫರ್_Callback_Function_t pxReceiveCompletedCallback )
    {
        /* MISRA Ref 11.3.1 [Misaligned access] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
        /* coverity[misra_c_2012_rule_11_3_violation] */
        ಹರಿವು_ಬಫರ್_t * ಸ್ಥಿರಾಂಕ px_ಹರಿವು_ಬಫರ್ = ( ಹರಿವು_ಬಫರ್_t * ) px_Static_ಹರಿವು_ಬಫರ್;
        ಹರಿವು_ಬಫರ್_Handle_t xReturn;
        uint8_t ucFlags;

        trace_ENTER_x_ಹರಿವು_ಬಫರ್_Generic_Create_Static( x_ಬಫರ್_ಗಾತ್ರ_ಬೈಟುಗಳು, x_Trigger_Level_ಬೈಟುಗಳು, x_ಹರಿವು_ಬಫರ್_Type, puc_ಹರಿವು_ಬಫರ್_Storage_Area, px_Static_ಹರಿವು_ಬಫರ್, pxSendCompletedCallback, pxReceiveCompletedCallback );

        configASSERT( puc_ಹರಿವು_ಬಫರ್_Storage_Area );
        configASSERT( px_Static_ಹರಿವು_ಬಫರ್ );
        configASSERT( x_Trigger_Level_ಬೈಟುಗಳು <= x_ಬಫರ್_ಗಾತ್ರ_ಬೈಟುಗಳು );

        /* A trigger level of 0 would cause a waiting task to unblock even when
         * the buffer was empty. */
        ಆದರೆ( x_Trigger_Level_ಬೈಟುಗಳು == ( size_t ) 0 )
        {
            x_Trigger_Level_ಬೈಟುಗಳು = ( size_t ) 1;
        }

        /* In case the stream buffer is going to be used as a message buffer
         * (that is, it will hold discrete messages with a little meta data that
         * says how big the next message is) check the buffer will be large enough
         * to hold at least one message. */

        ಆದರೆ( x_ಹರಿವು_ಬಫರ್_Type == sb_TYPE_ಸಂದೇಶ_ಬಫರ್ )
        {
            /* Statically allocated message buffer. */
            ucFlags = sb_FLAGS_IS_ಸಂದೇಶ_ಬಫರ್ | sbFLAGS_IS_STATICALLY_ALLOCATED;
            configASSERT( x_ಬಫರ್_ಗಾತ್ರ_ಬೈಟುಗಳು > sb_ಬೈಟುಗಳು_ಗಮ್ಯ_STORE_ಸಂದೇಶ_ಉದ್ದಳತೆ );
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( x_ಹರಿವು_ಬಫರ್_Type == sb_TYPE_ಹರಿವು_BATCHING_ಬಫರ್ )
        {
            /* Statically allocated batching buffer. */
            ucFlags = sb_FLAGS_IS_BATCHING_ಬಫರ್ | sbFLAGS_IS_STATICALLY_ALLOCATED;
            configASSERT( x_ಬಫರ್_ಗಾತ್ರ_ಬೈಟುಗಳು > 0 );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            /* Statically allocated stream buffer. */
            ucFlags = sbFLAGS_IS_STATICALLY_ALLOCATED;
        }

        #ಆದರೆ ( configASSERT_DEFINED == 1 )
        {
            /* Sanity check that the size of the structure used to declare a
             * variable of type StaticStreamBuffer_t equals the size of the real
             * message buffer structure. */
            ಅಸ್ಥಿರ size_t x_ಗಾತ್ರ = ಗಾತ್ರ( Static_ಹರಿವು_ಬಫರ್_t );
            configASSERT( x_ಗಾತ್ರ == ಗಾತ್ರ( ಹರಿವು_ಬಫರ್_t ) );
        }
        #ಆದರೆ_ಅಂತ್ಯ /* configASSERT_DEFINED */

        ಆದರೆ( ( puc_ಹರಿವು_ಬಫರ್_Storage_Area != ಖಾಲಿ ) && ( px_Static_ಹರಿವು_ಬಫರ್ != ಖಾಲಿ ) )
        {
            prv_Initialise_New_ಹರಿವು_ಬಫರ್( px_ಹರಿವು_ಬಫರ್,
                                          puc_ಹರಿವು_ಬಫರ್_Storage_Area,
                                          x_ಬಫರ್_ಗಾತ್ರ_ಬೈಟುಗಳು,
                                          x_Trigger_Level_ಬೈಟುಗಳು,
                                          ucFlags,
                                          pxSendCompletedCallback,
                                          pxReceiveCompletedCallback );

            /* Remember this was statically allocated in case it is ever deleted
             * again. */
            px_ಹರಿವು_ಬಫರ್->ucFlags |= sbFLAGS_IS_STATICALLY_ALLOCATED;

            trace_ಹರಿವು_ಬಫರ್_CREATE( px_ಹರಿವು_ಬಫರ್, x_ಹರಿವು_ಬಫರ್_Type );

            /* MISRA Ref 11.3.1 [Misaligned access] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
            /* coverity[misra_c_2012_rule_11_3_violation] */
            xReturn = ( ಹರಿವು_ಬಫರ್_Handle_t ) px_Static_ಹರಿವು_ಬಫರ್;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            xReturn = ಖಾಲಿ;
            trace_ಹರಿವು_ಬಫರ್_CREATE_STATIC_FAILED( xReturn, x_ಹರಿವು_ಬಫರ್_Type );
        }

        trace_RETURN_x_ಹರಿವು_ಬಫರ್_Generic_Create_Static( xReturn );

        ಹಿಂತಿರುಗಿ xReturn;
    }
    #ಆದರೆ_ಅಂತ್ಯ /* ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*-----------------------------------------------------------*/

    #ಆದರೆ ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 )
    ತಳ_Type_t x_ಹರಿವು_ಬಫರ್_Get_Static_Buffers( ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್,
                                              uint8_t ** ppuc_ಹರಿವು_ಬಫರ್_Storage_Area,
                                              Static_ಹರಿವು_ಬಫರ್_t ** ppx_Static_ಹರಿವು_ಬಫರ್ )
    {
        ತಳ_Type_t xReturn;
        ಹರಿವು_ಬಫರ್_t * ಸ್ಥಿರಾಂಕ px_ಹರಿವು_ಬಫರ್ = x_ಹರಿವು_ಬಫರ್;

        trace_ENTER_x_ಹರಿವು_ಬಫರ್_Get_Static_Buffers( x_ಹರಿವು_ಬಫರ್, ppuc_ಹರಿವು_ಬಫರ್_Storage_Area, ppx_Static_ಹರಿವು_ಬಫರ್ );

        configASSERT( px_ಹರಿವು_ಬಫರ್ );
        configASSERT( ppuc_ಹರಿವು_ಬಫರ್_Storage_Area );
        configASSERT( ppx_Static_ಹರಿವು_ಬಫರ್ );

        ಆದರೆ( ( px_ಹರಿವು_ಬಫರ್->ucFlags & sbFLAGS_IS_STATICALLY_ALLOCATED ) != ( uint8_t ) 0 )
        {
            *ppuc_ಹರಿವು_ಬಫರ್_Storage_Area = px_ಹರಿವು_ಬಫರ್->puc_ಬಫರ್;
            /* MISRA Ref 11.3.1 [Misaligned access] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
            /* coverity[misra_c_2012_rule_11_3_violation] */
            *ppx_Static_ಹರಿವು_ಬಫರ್ = ( Static_ಹರಿವು_ಬಫರ್_t * ) px_ಹರಿವು_ಬಫರ್;
            xReturn = pdTRUE;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            xReturn = pdFALSE;
        }

        trace_RETURN_x_ಹರಿವು_ಬಫರ್_Get_Static_Buffers( xReturn );

        ಹಿಂತಿರುಗಿ xReturn;
    }
    #ಆದರೆ_ಅಂತ್ಯ /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

ಶೂನ್ಯ v_ಹರಿವು_ಬಫರ್_Delete( ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್ )
{
    ಹರಿವು_ಬಫರ್_t * px_ಹರಿವು_ಬಫರ್ = x_ಹರಿವು_ಬಫರ್;

    trace_ENTER_v_ಹರಿವು_ಬಫರ್_Delete( x_ಹರಿವು_ಬಫರ್ );

    configASSERT( px_ಹರಿವು_ಬಫರ್ );

    trace_ಹರಿವು_ಬಫರ್_DELETE( x_ಹರಿವು_ಬಫರ್ );

    ಆದರೆ( ( px_ಹರಿವು_ಬಫರ್->ucFlags & sbFLAGS_IS_STATICALLY_ALLOCATED ) == ( uint8_t ) pdFALSE )
    {
        #ಆದರೆ ( config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ == 1 )
        {
            /* Both the structure and the buffer were allocated using a single call
            * to pvPortMalloc(), hence only one call to vPortFree() is required. */
            v_ಪೋರ್ಟ್_Free( ( ಶೂನ್ಯ * ) px_ಹರಿವು_ಬಫರ್ );
        }
        #ಇಲ್ಲದಿದ್ದರೆ
        {
            /* Should not be possible to get here, ucFlags must be corrupt.
             * Force an assert. */
            configASSERT( x_ಹರಿವು_ಬಫರ್ == ( ಹರಿವು_ಬಫರ್_Handle_t ) ~0 );
        }
        #ಆದರೆ_ಅಂತ್ಯ
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        /* The structure and buffer were not allocated dynamically and cannot be
         * freed - just scrub the structure so future use will assert. */
        ( ಶೂನ್ಯ ) memset( px_ಹರಿವು_ಬಫರ್, 0x00, ಗಾತ್ರ( ಹರಿವು_ಬಫರ್_t ) );
    }

    trace_RETURN_v_ಹರಿವು_ಬಫರ್_Delete();
}
/*-----------------------------------------------------------*/

ತಳ_Type_t x_ಹರಿವು_ಬಫರ್_Reset( ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್ )
{
    ಹರಿವು_ಬಫರ್_t * ಸ್ಥಿರಾಂಕ px_ಹರಿವು_ಬಫರ್ = x_ಹರಿವು_ಬಫರ್;
    ತಳ_Type_t xReturn = pdFAIL;
    ಹರಿವು_ಬಫರ್_Callback_Function_t pxSendCallback = ಖಾಲಿ, pxReceiveCallback = ಖಾಲಿ;

    #ಆದರೆ ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ;
    #ಆದರೆ_ಅಂತ್ಯ

    trace_ENTER_x_ಹರಿವು_ಬಫರ್_Reset( x_ಹರಿವು_ಬಫರ್ );

    configASSERT( px_ಹರಿವು_ಬಫರ್ );

    #ಆದರೆ ( configUSE_TRACE_FACILITY == 1 )
    {
        /* Store the stream buffer number so it can be restored after the
         * reset. */
        ux_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ = px_ಹರಿವು_ಬಫರ್->ux_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ;
    }
    #ಆದರೆ_ಅಂತ್ಯ

    /* Can only reset a message buffer if there are no tasks blocked on it. */
    taskENTER_CRITICAL();
    {
        ಆದರೆ( ( px_ಹರಿವು_ಬಫರ್->x_Task_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive == ಖಾಲಿ ) && ( px_ಹರಿವು_ಬಫರ್->x_Task_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send == ಖಾಲಿ ) )
        {
            #ಆದರೆ ( configUSE_SB_COMPLETED_CALLBACK == 1 )
            {
                pxSendCallback = px_ಹರಿವು_ಬಫರ್->pxSendCompletedCallback;
                pxReceiveCallback = px_ಹರಿವು_ಬಫರ್->pxReceiveCompletedCallback;
            }
            #ಆದರೆ_ಅಂತ್ಯ

            prv_Initialise_New_ಹರಿವು_ಬಫರ್( px_ಹರಿವು_ಬಫರ್,
                                          px_ಹರಿವು_ಬಫರ್->puc_ಬಫರ್,
                                          px_ಹರಿವು_ಬಫರ್->x_ಉದ್ದಳತೆ,
                                          px_ಹರಿವು_ಬಫರ್->x_Trigger_Level_ಬೈಟುಗಳು,
                                          px_ಹರಿವು_ಬಫರ್->ucFlags,
                                          pxSendCallback,
                                          pxReceiveCallback );

            #ಆದರೆ ( configUSE_TRACE_FACILITY == 1 )
            {
                px_ಹರಿವು_ಬಫರ್->ux_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ = ux_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ;
            }
            #ಆದರೆ_ಅಂತ್ಯ

            trace_ಹರಿವು_ಬಫರ್_RESET( x_ಹರಿವು_ಬಫರ್ );

            xReturn = pdPASS;
        }
    }
    taskEXIT_CRITICAL();

    trace_RETURN_x_ಹರಿವು_ಬಫರ್_Reset( xReturn );

    ಹಿಂತಿರುಗಿ xReturn;
}
/*-----------------------------------------------------------*/

ತಳ_Type_t x_ಹರಿವು_ಬಫರ್_Reset_From_ISR( ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್ )
{
    ಹರಿವು_ಬಫರ್_t * ಸ್ಥಿರಾಂಕ px_ಹರಿವು_ಬಫರ್ = x_ಹರಿವು_ಬಫರ್;
    ತಳ_Type_t xReturn = pdFAIL;
    ಹರಿವು_ಬಫರ್_Callback_Function_t pxSendCallback = ಖಾಲಿ, pxReceiveCallback = ಖಾಲಿ;
    UBaseType_t ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ;

    #ಆದರೆ ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ;
    #ಆದರೆ_ಅಂತ್ಯ

    trace_ENTER_x_ಹರಿವು_ಬಫರ್_Reset_From_ISR( x_ಹರಿವು_ಬಫರ್ );

    configASSERT( px_ಹರಿವು_ಬಫರ್ );

    #ಆದರೆ ( configUSE_TRACE_FACILITY == 1 )
    {
        /* Store the stream buffer number so it can be restored after the
         * reset. */
        ux_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ = px_ಹರಿವು_ಬಫರ್->ux_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ;
    }
    #ಆದರೆ_ಅಂತ್ಯ

    /* Can only reset a message buffer if there are no tasks blocked on it. */
    /* MISRA Ref 4.7.1 [Return value shall be checked] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
    /* coverity[misra_c_2012_directive_4_7_violation] */
    ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ = taskENTER_CRITICAL_FROM_ISR();
    {
        ಆದರೆ( ( px_ಹರಿವು_ಬಫರ್->x_Task_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive == ಖಾಲಿ ) && ( px_ಹರಿವು_ಬಫರ್->x_Task_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send == ಖಾಲಿ ) )
        {
            #ಆದರೆ ( configUSE_SB_COMPLETED_CALLBACK == 1 )
            {
                pxSendCallback = px_ಹರಿವು_ಬಫರ್->pxSendCompletedCallback;
                pxReceiveCallback = px_ಹರಿವು_ಬಫರ್->pxReceiveCompletedCallback;
            }
            #ಆದರೆ_ಅಂತ್ಯ

            prv_Initialise_New_ಹರಿವು_ಬಫರ್( px_ಹರಿವು_ಬಫರ್,
                                          px_ಹರಿವು_ಬಫರ್->puc_ಬಫರ್,
                                          px_ಹರಿವು_ಬಫರ್->x_ಉದ್ದಳತೆ,
                                          px_ಹರಿವು_ಬಫರ್->x_Trigger_Level_ಬೈಟುಗಳು,
                                          px_ಹರಿವು_ಬಫರ್->ucFlags,
                                          pxSendCallback,
                                          pxReceiveCallback );

            #ಆದರೆ ( configUSE_TRACE_FACILITY == 1 )
            {
                px_ಹರಿವು_ಬಫರ್->ux_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ = ux_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ;
            }
            #ಆದರೆ_ಅಂತ್ಯ

            trace_ಹರಿವು_ಬಫರ್_RESET_FROM_ISR( x_ಹರಿವು_ಬಫರ್ );

            xReturn = pdPASS;
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ );

    trace_RETURN_x_ಹರಿವು_ಬಫರ್_Reset_From_ISR( xReturn );

    ಹಿಂತಿರುಗಿ xReturn;
}
/*-----------------------------------------------------------*/

ತಳ_Type_t x_ಹರಿವು_ಬಫರ್_ಗಣ_Trigger_Level( ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್,
                                         size_t xTriggerLevel )
{
    ಹರಿವು_ಬಫರ್_t * ಸ್ಥಿರಾಂಕ px_ಹರಿವು_ಬಫರ್ = x_ಹರಿವು_ಬಫರ್;
    ತಳ_Type_t xReturn;

    trace_ENTER_x_ಹರಿವು_ಬಫರ್_ಗಣ_Trigger_Level( x_ಹರಿವು_ಬಫರ್, xTriggerLevel );

    configASSERT( px_ಹರಿವು_ಬಫರ್ );

    /* It is not valid for the trigger level to be 0. */
    ಆದರೆ( xTriggerLevel == ( size_t ) 0 )
    {
        xTriggerLevel = ( size_t ) 1;
    }

    /* The trigger level is the number of bytes that must be in the stream
     * buffer before a task that is waiting for data is unblocked. */
    ಆದರೆ( xTriggerLevel < px_ಹರಿವು_ಬಫರ್->x_ಉದ್ದಳತೆ )
    {
        px_ಹರಿವು_ಬಫರ್->x_Trigger_Level_ಬೈಟುಗಳು = xTriggerLevel;
        xReturn = pdPASS;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        xReturn = pdFALSE;
    }

    trace_RETURN_x_ಹರಿವು_ಬಫರ್_ಗಣ_Trigger_Level( xReturn );

    ಹಿಂತಿರುಗಿ xReturn;
}
/*-----------------------------------------------------------*/

size_t x_ಹರಿವು_ಬಫರ್_Spaces_ಲಭ್ಯ( ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್ )
{
    ಸ್ಥಿರಾಂಕ ಹರಿವು_ಬಫರ್_t * ಸ್ಥಿರಾಂಕ px_ಹರಿವು_ಬಫರ್ = x_ಹರಿವು_ಬಫರ್;
    size_t xSpace;
    size_t x_Original_ಬಾಲ;

    trace_ENTER_x_ಹರಿವು_ಬಫರ್_Spaces_ಲಭ್ಯ( x_ಹರಿವು_ಬಫರ್ );

    configASSERT( px_ಹರಿವು_ಬಫರ್ );

    /* The code below reads xTail and then xHead.  This is safe if the stream
     * buffer is updated once between the two reads - but not if the stream buffer
     * is updated more than once between the two reads - hence the loop. */
    ಮಾಡು
    {
        x_Original_ಬಾಲ = px_ಹರಿವು_ಬಫರ್->x_ಬಾಲ;
        xSpace = px_ಹರಿವು_ಬಫರ್->x_ಉದ್ದಳತೆ + px_ಹರಿವು_ಬಫರ್->x_ಬಾಲ;
        xSpace -= px_ಹರಿವು_ಬಫರ್->x_ತಲೆ;
    } ಆಗ( x_Original_ಬಾಲ != px_ಹರಿವು_ಬಫರ್->x_ಬಾಲ );

    xSpace -= ( size_t ) 1;

    ಆದರೆ( xSpace >= px_ಹರಿವು_ಬಫರ್->x_ಉದ್ದಳತೆ )
    {
        xSpace -= px_ಹರಿವು_ಬಫರ್->x_ಉದ್ದಳತೆ;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        mtCOVERAGE_TEST_MARKER();
    }

    trace_RETURN_x_ಹರಿವು_ಬಫರ್_Spaces_ಲಭ್ಯ( xSpace );

    ಹಿಂತಿರುಗಿ xSpace;
}
/*-----------------------------------------------------------*/

size_t x_ಹರಿವು_ಬಫರ್_ಬೈಟುಗಳು_ಲಭ್ಯ( ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್ )
{
    ಸ್ಥಿರಾಂಕ ಹರಿವು_ಬಫರ್_t * ಸ್ಥಿರಾಂಕ px_ಹರಿವು_ಬಫರ್ = x_ಹರಿವು_ಬಫರ್;
    size_t xReturn;

    trace_ENTER_x_ಹರಿವು_ಬಫರ್_ಬೈಟುಗಳು_ಲಭ್ಯ( x_ಹರಿವು_ಬಫರ್ );

    configASSERT( px_ಹರಿವು_ಬಫರ್ );

    xReturn = prv_ಬೈಟುಗಳು_ಒಳ_ಬಫರ್( px_ಹರಿವು_ಬಫರ್ );

    trace_RETURN_x_ಹರಿವು_ಬಫರ್_ಬೈಟುಗಳು_ಲಭ್ಯ( xReturn );

    ಹಿಂತಿರುಗಿ xReturn;
}
/*-----------------------------------------------------------*/

size_t x_ಹರಿವು_ಬಫರ್_Send( ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್,
                          ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * pv_Tx_ದತ್ತಾಂಶ,
                          size_t x_ದತ್ತಾಂಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು,
                          TickType_t x_Ticks_ಗಮ್ಯ_Wait )
{
    ಹರಿವು_ಬಫರ್_t * ಸ್ಥಿರಾಂಕ px_ಹರಿವು_ಬಫರ್ = x_ಹರಿವು_ಬಫರ್;
    size_t xReturn, xSpace = 0;
    size_t xRequiredSpace = x_ದತ್ತಾಂಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು;
    ಸಮಯ_ಹೊರಹೋಗು_t x_ಸಮಯ_ಹೊರಹೋಗು_2;
    size_t xMaxReportedSpace = 0;

    trace_ENTER_x_ಹರಿವು_ಬಫರ್_Send( x_ಹರಿವು_ಬಫರ್, pv_Tx_ದತ್ತಾಂಶ, x_ದತ್ತಾಂಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು, x_Ticks_ಗಮ್ಯ_Wait );

    configASSERT( pv_Tx_ದತ್ತಾಂಶ );
    configASSERT( px_ಹರಿವು_ಬಫರ್ );

    /* The maximum amount of space a stream buffer will ever report is its length
     * minus 1. */
    xMaxReportedSpace = px_ಹರಿವು_ಬಫರ್->x_ಉದ್ದಳತೆ - ( size_t ) 1;

    /* This send function is used to write to both message buffers and stream
     * buffers.  If this is a message buffer then the space needed must be
     * increased by the amount of bytes needed to store the length of the
     * message. */
    ಆದರೆ( ( px_ಹರಿವು_ಬಫರ್->ucFlags & sb_FLAGS_IS_ಸಂದೇಶ_ಬಫರ್ ) != ( uint8_t ) 0 )
    {
        xRequiredSpace += sb_ಬೈಟುಗಳು_ಗಮ್ಯ_STORE_ಸಂದೇಶ_ಉದ್ದಳತೆ;

        /* Overflow? */
        configASSERT( xRequiredSpace > x_ದತ್ತಾಂಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು );

        /* If this is a message buffer then it must be possible to write the
         * whole message. */
        ಆದರೆ( xRequiredSpace > xMaxReportedSpace )
        {
            /* The message would not fit even if the entire buffer was empty,
             * so don't wait for space. */
            x_Ticks_ಗಮ್ಯ_Wait = ( TickType_t ) 0;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        /* If this is a stream buffer then it is acceptable to write only part
         * of the message to the buffer.  Cap the length to the total length of
         * the buffer. */
        ಆದರೆ( xRequiredSpace > xMaxReportedSpace )
        {
            xRequiredSpace = xMaxReportedSpace;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }

    ಆದರೆ( x_Ticks_ಗಮ್ಯ_Wait != ( TickType_t ) 0 )
    {
        v_Task_ಗಣ_ಸಮಯ_ಹೊರಹೋಗು_State( &x_ಸಮಯ_ಹೊರಹೋಗು_2 );

        ಮಾಡು
        {
            /* Wait until the required number of bytes are free in the message
             * buffer. */
            taskENTER_CRITICAL();
            {
                xSpace = x_ಹರಿವು_ಬಫರ್_Spaces_ಲಭ್ಯ( px_ಹರಿವು_ಬಫರ್ );

                ಆದರೆ( xSpace < xRequiredSpace )
                {
                    /* Clear notification state as going to wait for space. */
                    ( ಶೂನ್ಯ ) x_Task_Notify_State_Clear_ಸೂಚ್ಯಂಕಿತ( ಖಾಲಿ, px_ಹರಿವು_ಬಫರ್->ux_Notification_ಸೂಚ್ಯಂಕ );

                    /* Should only be one writer. */
                    configASSERT( px_ಹರಿವು_ಬಫರ್->x_Task_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send == ಖಾಲಿ );
                    px_ಹರಿವು_ಬಫರ್->x_Task_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send = xTaskGetCurrentTaskHandle();
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    taskEXIT_CRITICAL();
                    ನಿಲ್ಲಿಸು;
                }
            }
            taskEXIT_CRITICAL();

            trace_BLOCKING_ON_ಹರಿವು_ಬಫರ್_SEND( x_ಹರಿವು_ಬಫರ್ );
            ( ಶೂನ್ಯ ) x_Task_Notify_Wait_ಸೂಚ್ಯಂಕಿತ( px_ಹರಿವು_ಬಫರ್->ux_Notification_ಸೂಚ್ಯಂಕ, ( uint32_t ) 0, ( uint32_t ) 0, ಖಾಲಿ, x_Ticks_ಗಮ್ಯ_Wait );
            px_ಹರಿವು_ಬಫರ್->x_Task_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send = ಖಾಲಿ;
        } ಆಗ( x_Task_Check_For_ಸಮಯ_ಹೊರಹೋಗು( &x_ಸಮಯ_ಹೊರಹೋಗು_2, &x_Ticks_ಗಮ್ಯ_Wait ) == pdFALSE );
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        mtCOVERAGE_TEST_MARKER();
    }

    ಆದರೆ( xSpace == ( size_t ) 0 )
    {
        xSpace = x_ಹರಿವು_ಬಫರ್_Spaces_ಲಭ್ಯ( px_ಹರಿವು_ಬಫರ್ );
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        mtCOVERAGE_TEST_MARKER();
    }

    xReturn = prv_Write_ಸಂದೇಶ_ಗಮ್ಯ_ಬಫರ್( px_ಹರಿವು_ಬಫರ್, pv_Tx_ದತ್ತಾಂಶ, x_ದತ್ತಾಂಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು, xSpace, xRequiredSpace );

    ಆದರೆ( xReturn > ( size_t ) 0 )
    {
        trace_ಹರಿವು_ಬಫರ್_SEND( x_ಹರಿವು_ಬಫರ್, xReturn );

        /* Was a task waiting for the data? */
        ಆದರೆ( prv_ಬೈಟುಗಳು_ಒಳ_ಬಫರ್( px_ಹರಿವು_ಬಫರ್ ) >= px_ಹರಿವು_ಬಫರ್->x_Trigger_Level_ಬೈಟುಗಳು )
        {
            prvSEND_COMPLETED( px_ಹರಿವು_ಬಫರ್ );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        mtCOVERAGE_TEST_MARKER();
        trace_ಹರಿವು_ಬಫರ್_SEND_FAILED( x_ಹರಿವು_ಬಫರ್ );
    }

    trace_RETURN_x_ಹರಿವು_ಬಫರ್_Send( xReturn );

    ಹಿಂತಿರುಗಿ xReturn;
}
/*-----------------------------------------------------------*/

size_t x_ಹರಿವು_ಬಫರ್_Send_From_ISR( ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್,
                                 ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * pv_Tx_ದತ್ತಾಂಶ,
                                 size_t x_ದತ್ತಾಂಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು,
                                 ತಳ_Type_t * ಸ್ಥಿರಾಂಕ px_Higher_ಆದ್ಯತೆ_Task_Woken )
{
    ಹರಿವು_ಬಫರ್_t * ಸ್ಥಿರಾಂಕ px_ಹರಿವು_ಬಫರ್ = x_ಹರಿವು_ಬಫರ್;
    size_t xReturn, xSpace;
    size_t xRequiredSpace = x_ದತ್ತಾಂಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು;

    trace_ENTER_x_ಹರಿವು_ಬಫರ್_Send_From_ISR( x_ಹರಿವು_ಬಫರ್, pv_Tx_ದತ್ತಾಂಶ, x_ದತ್ತಾಂಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು, px_Higher_ಆದ್ಯತೆ_Task_Woken );

    configASSERT( pv_Tx_ದತ್ತಾಂಶ );
    configASSERT( px_ಹರಿವು_ಬಫರ್ );

    /* This send function is used to write to both message buffers and stream
     * buffers.  If this is a message buffer then the space needed must be
     * increased by the amount of bytes needed to store the length of the
     * message. */
    ಆದರೆ( ( px_ಹರಿವು_ಬಫರ್->ucFlags & sb_FLAGS_IS_ಸಂದೇಶ_ಬಫರ್ ) != ( uint8_t ) 0 )
    {
        xRequiredSpace += sb_ಬೈಟುಗಳು_ಗಮ್ಯ_STORE_ಸಂದೇಶ_ಉದ್ದಳತೆ;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        mtCOVERAGE_TEST_MARKER();
    }

    xSpace = x_ಹರಿವು_ಬಫರ್_Spaces_ಲಭ್ಯ( px_ಹರಿವು_ಬಫರ್ );
    xReturn = prv_Write_ಸಂದೇಶ_ಗಮ್ಯ_ಬಫರ್( px_ಹರಿವು_ಬಫರ್, pv_Tx_ದತ್ತಾಂಶ, x_ದತ್ತಾಂಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು, xSpace, xRequiredSpace );

    ಆದರೆ( xReturn > ( size_t ) 0 )
    {
        /* Was a task waiting for the data? */
        ಆದರೆ( prv_ಬೈಟುಗಳು_ಒಳ_ಬಫರ್( px_ಹರಿವು_ಬಫರ್ ) >= px_ಹರಿವು_ಬಫರ್->x_Trigger_Level_ಬೈಟುಗಳು )
        {
            /* MISRA Ref 4.7.1 [Return value shall be checked] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
            /* coverity[misra_c_2012_directive_4_7_violation] */
            prvSEND_COMPLETE_FROM_ISR( px_ಹರಿವು_ಬಫರ್, px_Higher_ಆದ್ಯತೆ_Task_Woken );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        mtCOVERAGE_TEST_MARKER();
    }

    trace_ಹರಿವು_ಬಫರ್_SEND_FROM_ISR( x_ಹರಿವು_ಬಫರ್, xReturn );
    trace_RETURN_x_ಹರಿವು_ಬಫರ್_Send_From_ISR( xReturn );

    ಹಿಂತಿರುಗಿ xReturn;
}
/*-----------------------------------------------------------*/

ಸ್ಥಿರ size_t prv_Write_ಸಂದೇಶ_ಗಮ್ಯ_ಬಫರ್( ಹರಿವು_ಬಫರ್_t * ಸ್ಥಿರಾಂಕ px_ಹರಿವು_ಬಫರ್,
                                       ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * pv_Tx_ದತ್ತಾಂಶ,
                                       size_t x_ದತ್ತಾಂಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು,
                                       size_t xSpace,
                                       size_t xRequiredSpace )
{
    size_t x_ಮುಂದಿನ_ತಲೆ = px_ಹರಿವು_ಬಫರ್->x_ತಲೆ;
    config_ಸಂದೇಶ_ಬಫರ್_ಉದ್ದಳತೆ_TYPE x_ಸಂದೇಶ_ಉದ್ದಳತೆ;

    ಆದರೆ( ( px_ಹರಿವು_ಬಫರ್->ucFlags & sb_FLAGS_IS_ಸಂದೇಶ_ಬಫರ್ ) != ( uint8_t ) 0 )
    {
        /* This is a message buffer, as opposed to a stream buffer. */

        /* Convert xDataLengthBytes to the message length type. */
        x_ಸಂದೇಶ_ಉದ್ದಳತೆ = ( config_ಸಂದೇಶ_ಬಫರ್_ಉದ್ದಳತೆ_TYPE ) x_ದತ್ತಾಂಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು;

        /* Ensure the data length given fits within configMESSAGE_BUFFER_LENGTH_TYPE. */
        configASSERT( ( size_t ) x_ಸಂದೇಶ_ಉದ್ದಳತೆ == x_ದತ್ತಾಂಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು );

        ಆದರೆ( xSpace >= xRequiredSpace )
        {
            /* There is enough space to write both the message length and the message
             * itself into the buffer.  Start by writing the length of the data, the data
             * itself will be written later in this function. */
            x_ಮುಂದಿನ_ತಲೆ = prv_Write_ಬೈಟುಗಳು_ಗಮ್ಯ_ಬಫರ್( px_ಹರಿವು_ಬಫರ್, ( ಸ್ಥಿರಾಂಕ uint8_t * ) &( x_ಸಂದೇಶ_ಉದ್ದಳತೆ ), sb_ಬೈಟುಗಳು_ಗಮ್ಯ_STORE_ಸಂದೇಶ_ಉದ್ದಳತೆ, x_ಮುಂದಿನ_ತಲೆ );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            /* Not enough space, so do not write data to the buffer. */
            x_ದತ್ತಾಂಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು = 0;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        /* This is a stream buffer, as opposed to a message buffer, so writing a
         * stream of bytes rather than discrete messages.  Plan to write as many
         * bytes as possible. */
        x_ದತ್ತಾಂಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು = configMIN( x_ದತ್ತಾಂಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು, xSpace );
    }

    ಆದರೆ( x_ದತ್ತಾಂಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು != ( size_t ) 0 )
    {
        /* Write the data to the buffer. */
        /* MISRA Ref 11.5.5 [Void pointer assignment] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
        /* coverity[misra_c_2012_rule_11_5_violation] */
        px_ಹರಿವು_ಬಫರ್->x_ತಲೆ = prv_Write_ಬೈಟುಗಳು_ಗಮ್ಯ_ಬಫರ್( px_ಹರಿವು_ಬಫರ್, ( ಸ್ಥಿರಾಂಕ uint8_t * ) pv_Tx_ದತ್ತಾಂಶ, x_ದತ್ತಾಂಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು, x_ಮುಂದಿನ_ತಲೆ );
    }

    ಹಿಂತಿರುಗಿ x_ದತ್ತಾಂಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು;
}
/*-----------------------------------------------------------*/

size_t x_ಹರಿವು_ಬಫರ್_Receive( ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್,
                             ಶೂನ್ಯ * pv_Rx_ದತ್ತಾಂಶ,
                             size_t x_ಬಫರ್_ಉದ್ದಳತೆ_ಬೈಟುಗಳು,
                             TickType_t x_Ticks_ಗಮ್ಯ_Wait )
{
    ಹರಿವು_ಬಫರ್_t * ಸ್ಥಿರಾಂಕ px_ಹರಿವು_ಬಫರ್ = x_ಹರಿವು_ಬಫರ್;
    size_t x_Received_ಉದ್ದಳತೆ = 0, x_ಬೈಟುಗಳು_ಲಭ್ಯ, x_ಬೈಟುಗಳು_ಗಮ್ಯ_Store_ಸಂದೇಶ_ಉದ್ದಳತೆ;

    trace_ENTER_x_ಹರಿವು_ಬಫರ್_Receive( x_ಹರಿವು_ಬಫರ್, pv_Rx_ದತ್ತಾಂಶ, x_ಬಫರ್_ಉದ್ದಳತೆ_ಬೈಟುಗಳು, x_Ticks_ಗಮ್ಯ_Wait );

    configASSERT( pv_Rx_ದತ್ತಾಂಶ );
    configASSERT( px_ಹರಿವು_ಬಫರ್ );

    /* This receive function is used by both message buffers, which store
     * discrete messages, and stream buffers, which store a continuous stream of
     * bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the
     * message. */
    ಆದರೆ( ( px_ಹರಿವು_ಬಫರ್->ucFlags & sb_FLAGS_IS_ಸಂದೇಶ_ಬಫರ್ ) != ( uint8_t ) 0 )
    {
        x_ಬೈಟುಗಳು_ಗಮ್ಯ_Store_ಸಂದೇಶ_ಉದ್ದಳತೆ = sb_ಬೈಟುಗಳು_ಗಮ್ಯ_STORE_ಸಂದೇಶ_ಉದ್ದಳತೆ;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ( px_ಹರಿವು_ಬಫರ್->ucFlags & sb_FLAGS_IS_BATCHING_ಬಫರ್ ) != ( uint8_t ) 0 )
    {
        /* Force task to block if the batching buffer contains less bytes than
         * the trigger level. */
        x_ಬೈಟುಗಳು_ಗಮ್ಯ_Store_ಸಂದೇಶ_ಉದ್ದಳತೆ = px_ಹರಿವು_ಬಫರ್->x_Trigger_Level_ಬೈಟುಗಳು;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        x_ಬೈಟುಗಳು_ಗಮ್ಯ_Store_ಸಂದೇಶ_ಉದ್ದಳತೆ = 0;
    }

    ಆದರೆ( x_Ticks_ಗಮ್ಯ_Wait != ( TickType_t ) 0 )
    {
        /* Checking if there is data and clearing the notification state must be
         * performed atomically. */
        taskENTER_CRITICAL();
        {
            x_ಬೈಟುಗಳು_ಲಭ್ಯ = prv_ಬೈಟುಗಳು_ಒಳ_ಬಫರ್( px_ಹರಿವು_ಬಫರ್ );

            /* If this function was invoked by a message buffer read then
             * xBytesToStoreMessageLength holds the number of bytes used to hold
             * the length of the next discrete message.  If this function was
             * invoked by a stream buffer read then xBytesToStoreMessageLength will
             * be 0. If this function was invoked by a stream batch buffer read
             * then xBytesToStoreMessageLength will be xTriggerLevelBytes value
             * for the buffer.*/
            ಆದರೆ( x_ಬೈಟುಗಳು_ಲಭ್ಯ <= x_ಬೈಟುಗಳು_ಗಮ್ಯ_Store_ಸಂದೇಶ_ಉದ್ದಳತೆ )
            {
                /* Clear notification state as going to wait for data. */
                ( ಶೂನ್ಯ ) x_Task_Notify_State_Clear_ಸೂಚ್ಯಂಕಿತ( ಖಾಲಿ, px_ಹರಿವು_ಬಫರ್->ux_Notification_ಸೂಚ್ಯಂಕ );

                /* Should only be one reader. */
                configASSERT( px_ಹರಿವು_ಬಫರ್->x_Task_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive == ಖಾಲಿ );
                px_ಹರಿವು_ಬಫರ್->x_Task_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive = xTaskGetCurrentTaskHandle();
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        taskEXIT_CRITICAL();

        ಆದರೆ( x_ಬೈಟುಗಳು_ಲಭ್ಯ <= x_ಬೈಟುಗಳು_ಗಮ್ಯ_Store_ಸಂದೇಶ_ಉದ್ದಳತೆ )
        {
            /* Wait for data to be available. */
            trace_BLOCKING_ON_ಹರಿವು_ಬಫರ್_RECEIVE( x_ಹರಿವು_ಬಫರ್ );
            ( ಶೂನ್ಯ ) x_Task_Notify_Wait_ಸೂಚ್ಯಂಕಿತ( px_ಹರಿವು_ಬಫರ್->ux_Notification_ಸೂಚ್ಯಂಕ, ( uint32_t ) 0, ( uint32_t ) 0, ಖಾಲಿ, x_Ticks_ಗಮ್ಯ_Wait );
            px_ಹರಿವು_ಬಫರ್->x_Task_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive = ಖಾಲಿ;

            /* Recheck the data available after blocking. */
            x_ಬೈಟುಗಳು_ಲಭ್ಯ = prv_ಬೈಟುಗಳು_ಒಳ_ಬಫರ್( px_ಹರಿವು_ಬಫರ್ );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        x_ಬೈಟುಗಳು_ಲಭ್ಯ = prv_ಬೈಟುಗಳು_ಒಳ_ಬಫರ್( px_ಹರಿವು_ಬಫರ್ );
    }

    /* Whether receiving a discrete message (where xBytesToStoreMessageLength
     * holds the number of bytes used to store the message length) or a stream of
     * bytes (where xBytesToStoreMessageLength is zero), the number of bytes
     * available must be greater than xBytesToStoreMessageLength to be able to
     * read bytes from the buffer. */
    ಆದರೆ( x_ಬೈಟುಗಳು_ಲಭ್ಯ > x_ಬೈಟುಗಳು_ಗಮ್ಯ_Store_ಸಂದೇಶ_ಉದ್ದಳತೆ )
    {
        x_Received_ಉದ್ದಳತೆ = prv_Read_ಸಂದೇಶ_From_ಬಫರ್( px_ಹರಿವು_ಬಫರ್, pv_Rx_ದತ್ತಾಂಶ, x_ಬಫರ್_ಉದ್ದಳತೆ_ಬೈಟುಗಳು, x_ಬೈಟುಗಳು_ಲಭ್ಯ );

        /* Was a task waiting for space in the buffer? */
        ಆದರೆ( x_Received_ಉದ್ದಳತೆ != ( size_t ) 0 )
        {
            trace_ಹರಿವು_ಬಫರ್_RECEIVE( x_ಹರಿವು_ಬಫರ್, x_Received_ಉದ್ದಳತೆ );
            prvRECEIVE_COMPLETED( x_ಹರಿವು_ಬಫರ್ );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        trace_ಹರಿವು_ಬಫರ್_RECEIVE_FAILED( x_ಹರಿವು_ಬಫರ್ );
        mtCOVERAGE_TEST_MARKER();
    }

    trace_RETURN_x_ಹರಿವು_ಬಫರ್_Receive( x_Received_ಉದ್ದಳತೆ );

    ಹಿಂತಿರುಗಿ x_Received_ಉದ್ದಳತೆ;
}
/*-----------------------------------------------------------*/

size_t x_ಹರಿವು_ಬಫರ್_ಮುಂದಿನ_ಸಂದೇಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು( ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್ )
{
    ಹರಿವು_ಬಫರ್_t * ಸ್ಥಿರಾಂಕ px_ಹರಿವು_ಬಫರ್ = x_ಹರಿವು_ಬಫರ್;
    size_t xReturn, x_ಬೈಟುಗಳು_ಲಭ್ಯ;
    config_ಸಂದೇಶ_ಬಫರ್_ಉದ್ದಳತೆ_TYPE x_ತಾತ್ಕಾಲಿಕ_Return;

    trace_ENTER_x_ಹರಿವು_ಬಫರ್_ಮುಂದಿನ_ಸಂದೇಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು( x_ಹರಿವು_ಬಫರ್ );

    configASSERT( px_ಹರಿವು_ಬಫರ್ );

    /* Ensure the stream buffer is being used as a message buffer. */
    ಆದರೆ( ( px_ಹರಿವು_ಬಫರ್->ucFlags & sb_FLAGS_IS_ಸಂದೇಶ_ಬಫರ್ ) != ( uint8_t ) 0 )
    {
        x_ಬೈಟುಗಳು_ಲಭ್ಯ = prv_ಬೈಟುಗಳು_ಒಳ_ಬಫರ್( px_ಹರಿವು_ಬಫರ್ );

        ಆದರೆ( x_ಬೈಟುಗಳು_ಲಭ್ಯ > sb_ಬೈಟುಗಳು_ಗಮ್ಯ_STORE_ಸಂದೇಶ_ಉದ್ದಳತೆ )
        {
            /* The number of bytes available is greater than the number of bytes
             * required to hold the length of the next message, so another message
             * is available. */
            ( ಶೂನ್ಯ ) prv_Read_ಬೈಟುಗಳು_From_ಬಫರ್( px_ಹರಿವು_ಬಫರ್, ( uint8_t * ) &x_ತಾತ್ಕಾಲಿಕ_Return, sb_ಬೈಟುಗಳು_ಗಮ್ಯ_STORE_ಸಂದೇಶ_ಉದ್ದಳತೆ, px_ಹರಿವು_ಬಫರ್->x_ಬಾಲ );
            xReturn = ( size_t ) x_ತಾತ್ಕಾಲಿಕ_Return;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            /* The minimum amount of bytes in a message buffer is
             * ( sbBYTES_TO_STORE_MESSAGE_LENGTH + 1 ), so if xBytesAvailable is
             * less than sbBYTES_TO_STORE_MESSAGE_LENGTH the only other valid
             * value is 0. */
            configASSERT( x_ಬೈಟುಗಳು_ಲಭ್ಯ == 0 );
            xReturn = 0;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        xReturn = 0;
    }

    trace_RETURN_x_ಹರಿವು_ಬಫರ್_ಮುಂದಿನ_ಸಂದೇಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು( xReturn );

    ಹಿಂತಿರುಗಿ xReturn;
}
/*-----------------------------------------------------------*/

size_t x_ಹರಿವು_ಬಫರ್_Receive_From_ISR( ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್,
                                    ಶೂನ್ಯ * pv_Rx_ದತ್ತಾಂಶ,
                                    size_t x_ಬಫರ್_ಉದ್ದಳತೆ_ಬೈಟುಗಳು,
                                    ತಳ_Type_t * ಸ್ಥಿರಾಂಕ px_Higher_ಆದ್ಯತೆ_Task_Woken )
{
    ಹರಿವು_ಬಫರ್_t * ಸ್ಥಿರಾಂಕ px_ಹರಿವು_ಬಫರ್ = x_ಹರಿವು_ಬಫರ್;
    size_t x_Received_ಉದ್ದಳತೆ = 0, x_ಬೈಟುಗಳು_ಲಭ್ಯ, x_ಬೈಟುಗಳು_ಗಮ್ಯ_Store_ಸಂದೇಶ_ಉದ್ದಳತೆ;

    trace_ENTER_x_ಹರಿವು_ಬಫರ್_Receive_From_ISR( x_ಹರಿವು_ಬಫರ್, pv_Rx_ದತ್ತಾಂಶ, x_ಬಫರ್_ಉದ್ದಳತೆ_ಬೈಟುಗಳು, px_Higher_ಆದ್ಯತೆ_Task_Woken );

    configASSERT( pv_Rx_ದತ್ತಾಂಶ );
    configASSERT( px_ಹರಿವು_ಬಫರ್ );

    /* This receive function is used by both message buffers, which store
     * discrete messages, and stream buffers, which store a continuous stream of
     * bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the
     * message. */
    ಆದರೆ( ( px_ಹರಿವು_ಬಫರ್->ucFlags & sb_FLAGS_IS_ಸಂದೇಶ_ಬಫರ್ ) != ( uint8_t ) 0 )
    {
        x_ಬೈಟುಗಳು_ಗಮ್ಯ_Store_ಸಂದೇಶ_ಉದ್ದಳತೆ = sb_ಬೈಟುಗಳು_ಗಮ್ಯ_STORE_ಸಂದೇಶ_ಉದ್ದಳತೆ;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        x_ಬೈಟುಗಳು_ಗಮ್ಯ_Store_ಸಂದೇಶ_ಉದ್ದಳತೆ = 0;
    }

    x_ಬೈಟುಗಳು_ಲಭ್ಯ = prv_ಬೈಟುಗಳು_ಒಳ_ಬಫರ್( px_ಹರಿವು_ಬಫರ್ );

    /* Whether receiving a discrete message (where xBytesToStoreMessageLength
     * holds the number of bytes used to store the message length) or a stream of
     * bytes (where xBytesToStoreMessageLength is zero), the number of bytes
     * available must be greater than xBytesToStoreMessageLength to be able to
     * read bytes from the buffer. */
    ಆದರೆ( x_ಬೈಟುಗಳು_ಲಭ್ಯ > x_ಬೈಟುಗಳು_ಗಮ್ಯ_Store_ಸಂದೇಶ_ಉದ್ದಳತೆ )
    {
        x_Received_ಉದ್ದಳತೆ = prv_Read_ಸಂದೇಶ_From_ಬಫರ್( px_ಹರಿವು_ಬಫರ್, pv_Rx_ದತ್ತಾಂಶ, x_ಬಫರ್_ಉದ್ದಳತೆ_ಬೈಟುಗಳು, x_ಬೈಟುಗಳು_ಲಭ್ಯ );

        /* Was a task waiting for space in the buffer? */
        ಆದರೆ( x_Received_ಉದ್ದಳತೆ != ( size_t ) 0 )
        {
            /* MISRA Ref 4.7.1 [Return value shall be checked] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
            /* coverity[misra_c_2012_directive_4_7_violation] */
            prvRECEIVE_COMPLETED_FROM_ISR( px_ಹರಿವು_ಬಫರ್, px_Higher_ಆದ್ಯತೆ_Task_Woken );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        mtCOVERAGE_TEST_MARKER();
    }

    trace_ಹರಿವು_ಬಫರ್_RECEIVE_FROM_ISR( x_ಹರಿವು_ಬಫರ್, x_Received_ಉದ್ದಳತೆ );
    trace_RETURN_x_ಹರಿವು_ಬಫರ್_Receive_From_ISR( x_Received_ಉದ್ದಳತೆ );

    ಹಿಂತಿರುಗಿ x_Received_ಉದ್ದಳತೆ;
}
/*-----------------------------------------------------------*/

ಸ್ಥಿರ size_t prv_Read_ಸಂದೇಶ_From_ಬಫರ್( ಹರಿವು_ಬಫರ್_t * px_ಹರಿವು_ಬಫರ್,
                                        ಶೂನ್ಯ * pv_Rx_ದತ್ತಾಂಶ,
                                        size_t x_ಬಫರ್_ಉದ್ದಳತೆ_ಬೈಟುಗಳು,
                                        size_t x_ಬೈಟುಗಳು_ಲಭ್ಯ )
{
    size_t x_ಎಣಿಕೆ, x_ಮುಂದಿನ_ಸಂದೇಶ_ಉದ್ದಳತೆ;
    config_ಸಂದೇಶ_ಬಫರ್_ಉದ್ದಳತೆ_TYPE x_ತಾತ್ಕಾಲಿಕ_ಮುಂದಿನ_ಸಂದೇಶ_ಉದ್ದಳತೆ;
    size_t x_ಮುಂದಿನ_ಬಾಲ = px_ಹರಿವು_ಬಫರ್->x_ಬಾಲ;

    ಆದರೆ( ( px_ಹರಿವು_ಬಫರ್->ucFlags & sb_FLAGS_IS_ಸಂದೇಶ_ಬಫರ್ ) != ( uint8_t ) 0 )
    {
        /* A discrete message is being received.  First receive the length
         * of the message. */
        x_ಮುಂದಿನ_ಬಾಲ = prv_Read_ಬೈಟುಗಳು_From_ಬಫರ್( px_ಹರಿವು_ಬಫರ್, ( uint8_t * ) &x_ತಾತ್ಕಾಲಿಕ_ಮುಂದಿನ_ಸಂದೇಶ_ಉದ್ದಳತೆ, sb_ಬೈಟುಗಳು_ಗಮ್ಯ_STORE_ಸಂದೇಶ_ಉದ್ದಳತೆ, x_ಮುಂದಿನ_ಬಾಲ );
        x_ಮುಂದಿನ_ಸಂದೇಶ_ಉದ್ದಳತೆ = ( size_t ) x_ತಾತ್ಕಾಲಿಕ_ಮುಂದಿನ_ಸಂದೇಶ_ಉದ್ದಳತೆ;

        /* Reduce the number of bytes available by the number of bytes just
         * read out. */
        x_ಬೈಟುಗಳು_ಲಭ್ಯ -= sb_ಬೈಟುಗಳು_ಗಮ್ಯ_STORE_ಸಂದೇಶ_ಉದ್ದಳತೆ;

        /* Check there is enough space in the buffer provided by the
         * user. */
        ಆದರೆ( x_ಮುಂದಿನ_ಸಂದೇಶ_ಉದ್ದಳತೆ > x_ಬಫರ್_ಉದ್ದಳತೆ_ಬೈಟುಗಳು )
        {
            /* The user has provided insufficient space to read the message. */
            x_ಮುಂದಿನ_ಸಂದೇಶ_ಉದ್ದಳತೆ = 0;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        /* A stream of bytes is being received (as opposed to a discrete
         * message), so read as many bytes as possible. */
        x_ಮುಂದಿನ_ಸಂದೇಶ_ಉದ್ದಳತೆ = x_ಬಫರ್_ಉದ್ದಳತೆ_ಬೈಟುಗಳು;
    }

    /* Use the minimum of the wanted bytes and the available bytes. */
    x_ಎಣಿಕೆ = configMIN( x_ಮುಂದಿನ_ಸಂದೇಶ_ಉದ್ದಳತೆ, x_ಬೈಟುಗಳು_ಲಭ್ಯ );

    ಆದರೆ( x_ಎಣಿಕೆ != ( size_t ) 0 )
    {
        /* Read the actual data and update the tail to mark the data as officially consumed. */
        /* MISRA Ref 11.5.5 [Void pointer assignment] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
        /* coverity[misra_c_2012_rule_11_5_violation] */
        px_ಹರಿವು_ಬಫರ್->x_ಬಾಲ = prv_Read_ಬೈಟುಗಳು_From_ಬಫರ್( px_ಹರಿವು_ಬಫರ್, ( uint8_t * ) pv_Rx_ದತ್ತಾಂಶ, x_ಎಣಿಕೆ, x_ಮುಂದಿನ_ಬಾಲ );
    }

    ಹಿಂತಿರುಗಿ x_ಎಣಿಕೆ;
}
/*-----------------------------------------------------------*/

ತಳ_Type_t x_ಹರಿವು_ಬಫರ್_Is_ಖಾಲಿಗಳು( ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್ )
{
    ಸ್ಥಿರಾಂಕ ಹರಿವು_ಬಫರ್_t * ಸ್ಥಿರಾಂಕ px_ಹರಿವು_ಬಫರ್ = x_ಹರಿವು_ಬಫರ್;
    ತಳ_Type_t xReturn;
    size_t x_ಬಾಲ;

    trace_ENTER_x_ಹರಿವು_ಬಫರ್_Is_ಖಾಲಿಗಳು( x_ಹರಿವು_ಬಫರ್ );

    configASSERT( px_ಹರಿವು_ಬಫರ್ );

    /* True if no bytes are available. */
    x_ಬಾಲ = px_ಹರಿವು_ಬಫರ್->x_ಬಾಲ;

    ಆದರೆ( px_ಹರಿವು_ಬಫರ್->x_ತಲೆ == x_ಬಾಲ )
    {
        xReturn = pdTRUE;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        xReturn = pdFALSE;
    }

    trace_RETURN_x_ಹರಿವು_ಬಫರ್_Is_ಖಾಲಿಗಳು( xReturn );

    ಹಿಂತಿರುಗಿ xReturn;
}
/*-----------------------------------------------------------*/

ತಳ_Type_t x_ಹರಿವು_ಬಫರ್_Is_ತುಂಬಿದ( ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್ )
{
    ತಳ_Type_t xReturn;
    size_t x_ಬೈಟುಗಳು_ಗಮ್ಯ_Store_ಸಂದೇಶ_ಉದ್ದಳತೆ;
    ಸ್ಥಿರಾಂಕ ಹರಿವು_ಬಫರ್_t * ಸ್ಥಿರಾಂಕ px_ಹರಿವು_ಬಫರ್ = x_ಹರಿವು_ಬಫರ್;

    trace_ENTER_x_ಹರಿವು_ಬಫರ್_Is_ತುಂಬಿದ( x_ಹರಿವು_ಬಫರ್ );

    configASSERT( px_ಹರಿವು_ಬಫರ್ );

    /* This generic version of the receive function is used by both message
     * buffers, which store discrete messages, and stream buffers, which store a
     * continuous stream of bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the message. */
    ಆದರೆ( ( px_ಹರಿವು_ಬಫರ್->ucFlags & sb_FLAGS_IS_ಸಂದೇಶ_ಬಫರ್ ) != ( uint8_t ) 0 )
    {
        x_ಬೈಟುಗಳು_ಗಮ್ಯ_Store_ಸಂದೇಶ_ಉದ್ದಳತೆ = sb_ಬೈಟುಗಳು_ಗಮ್ಯ_STORE_ಸಂದೇಶ_ಉದ್ದಳತೆ;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        x_ಬೈಟುಗಳು_ಗಮ್ಯ_Store_ಸಂದೇಶ_ಉದ್ದಳತೆ = 0;
    }

    /* True if the available space equals zero. */
    ಆದರೆ( x_ಹರಿವು_ಬಫರ್_Spaces_ಲಭ್ಯ( x_ಹರಿವು_ಬಫರ್ ) <= x_ಬೈಟುಗಳು_ಗಮ್ಯ_Store_ಸಂದೇಶ_ಉದ್ದಳತೆ )
    {
        xReturn = pdTRUE;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        xReturn = pdFALSE;
    }

    trace_RETURN_x_ಹರಿವು_ಬಫರ್_Is_ತುಂಬಿದ( xReturn );

    ಹಿಂತಿರುಗಿ xReturn;
}
/*-----------------------------------------------------------*/

ತಳ_Type_t x_ಹರಿವು_ಬಫರ್_Send_Completed_From_ISR( ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್,
                                              ತಳ_Type_t * px_Higher_ಆದ್ಯತೆ_Task_Woken )
{
    ಹರಿವು_ಬಫರ್_t * ಸ್ಥಿರಾಂಕ px_ಹರಿವು_ಬಫರ್ = x_ಹರಿವು_ಬಫರ್;
    ತಳ_Type_t xReturn;
    UBaseType_t ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ;

    trace_ENTER_x_ಹರಿವು_ಬಫರ್_Send_Completed_From_ISR( x_ಹರಿವು_ಬಫರ್, px_Higher_ಆದ್ಯತೆ_Task_Woken );

    configASSERT( px_ಹರಿವು_ಬಫರ್ );

    /* MISRA Ref 4.7.1 [Return value shall be checked] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
    /* coverity[misra_c_2012_directive_4_7_violation] */
    ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ = taskENTER_CRITICAL_FROM_ISR();
    {
        ಆದರೆ( ( px_ಹರಿವು_ಬಫರ್ )->x_Task_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive != ಖಾಲಿ )
        {
            ( ಶೂನ್ಯ ) x_Task_Notify_ಸೂಚ್ಯಂಕಿತ_From_ISR( ( px_ಹರಿವು_ಬಫರ್ )->x_Task_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive,
                                                ( px_ಹರಿವು_ಬಫರ್ )->ux_Notification_ಸೂಚ್ಯಂಕ,
                                                ( uint32_t ) 0,
                                                eNoAction,
                                                px_Higher_ಆದ್ಯತೆ_Task_Woken );
            ( px_ಹರಿವು_ಬಫರ್ )->x_Task_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive = ಖಾಲಿ;
            xReturn = pdTRUE;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            xReturn = pdFALSE;
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ );

    trace_RETURN_x_ಹರಿವು_ಬಫರ್_Send_Completed_From_ISR( xReturn );

    ಹಿಂತಿರುಗಿ xReturn;
}
/*-----------------------------------------------------------*/

ತಳ_Type_t x_ಹರಿವು_ಬಫರ್_Receive_Completed_From_ISR( ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್,
                                                 ತಳ_Type_t * px_Higher_ಆದ್ಯತೆ_Task_Woken )
{
    ಹರಿವು_ಬಫರ್_t * ಸ್ಥಿರಾಂಕ px_ಹರಿವು_ಬಫರ್ = x_ಹರಿವು_ಬಫರ್;
    ತಳ_Type_t xReturn;
    UBaseType_t ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ;

    trace_ENTER_x_ಹರಿವು_ಬಫರ್_Receive_Completed_From_ISR( x_ಹರಿವು_ಬಫರ್, px_Higher_ಆದ್ಯತೆ_Task_Woken );

    configASSERT( px_ಹರಿವು_ಬಫರ್ );

    /* MISRA Ref 4.7.1 [Return value shall be checked] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
    /* coverity[misra_c_2012_directive_4_7_violation] */
    ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ = taskENTER_CRITICAL_FROM_ISR();
    {
        ಆದರೆ( ( px_ಹರಿವು_ಬಫರ್ )->x_Task_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send != ಖಾಲಿ )
        {
            ( ಶೂನ್ಯ ) x_Task_Notify_ಸೂಚ್ಯಂಕಿತ_From_ISR( ( px_ಹರಿವು_ಬಫರ್ )->x_Task_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send,
                                                ( px_ಹರಿವು_ಬಫರ್ )->ux_Notification_ಸೂಚ್ಯಂಕ,
                                                ( uint32_t ) 0,
                                                eNoAction,
                                                px_Higher_ಆದ್ಯತೆ_Task_Woken );
            ( px_ಹರಿವು_ಬಫರ್ )->x_Task_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send = ಖಾಲಿ;
            xReturn = pdTRUE;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            xReturn = pdFALSE;
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ );

    trace_RETURN_x_ಹರಿವು_ಬಫರ್_Receive_Completed_From_ISR( xReturn );

    ಹಿಂತಿರುಗಿ xReturn;
}
/*-----------------------------------------------------------*/

ಸ್ಥಿರ size_t prv_Write_ಬೈಟುಗಳು_ಗಮ್ಯ_ಬಫರ್( ಹರಿವು_ಬಫರ್_t * ಸ್ಥಿರಾಂಕ px_ಹರಿವು_ಬಫರ್,
                                     ಸ್ಥಿರಾಂಕ uint8_t * puc_ದತ್ತಾಂಶ,
                                     size_t x_ಎಣಿಕೆ,
                                     size_t x_ತಲೆ )
{
    size_t x_ಮೊದಲ_ಉದ್ದಳತೆ;

    configASSERT( x_ಎಣಿಕೆ > ( size_t ) 0 );

    /* Calculate the number of bytes that can be added in the first write -
     * which may be less than the total number of bytes that need to be added if
     * the buffer will wrap back to the beginning. */
    x_ಮೊದಲ_ಉದ್ದಳತೆ = configMIN( px_ಹರಿವು_ಬಫರ್->x_ಉದ್ದಳತೆ - x_ತಲೆ, x_ಎಣಿಕೆ );

    /* Write as many bytes as can be written in the first write. */
    configASSERT( ( x_ತಲೆ + x_ಮೊದಲ_ಉದ್ದಳತೆ ) <= px_ಹರಿವು_ಬಫರ್->x_ಉದ್ದಳತೆ );
    ( ಶೂನ್ಯ ) ಸ್ಮರಣೆ_ನಕಲು( ( ಶೂನ್ಯ * ) ( &( px_ಹರಿವು_ಬಫರ್->puc_ಬಫರ್[ x_ತಲೆ ] ) ), ( ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * ) puc_ದತ್ತಾಂಶ, x_ಮೊದಲ_ಉದ್ದಳತೆ );

    /* If the number of bytes written was less than the number that could be
     * written in the first write... */
    ಆದರೆ( x_ಎಣಿಕೆ > x_ಮೊದಲ_ಉದ್ದಳತೆ )
    {
        /* ...then write the remaining bytes to the start of the buffer. */
        configASSERT( ( x_ಎಣಿಕೆ - x_ಮೊದಲ_ಉದ್ದಳತೆ ) <= px_ಹರಿವು_ಬಫರ್->x_ಉದ್ದಳತೆ );
        ( ಶೂನ್ಯ ) ಸ್ಮರಣೆ_ನಕಲು( ( ಶೂನ್ಯ * ) px_ಹರಿವು_ಬಫರ್->puc_ಬಫರ್, ( ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * ) &( puc_ದತ್ತಾಂಶ[ x_ಮೊದಲ_ಉದ್ದಳತೆ ] ), x_ಎಣಿಕೆ - x_ಮೊದಲ_ಉದ್ದಳತೆ );
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        mtCOVERAGE_TEST_MARKER();
    }

    x_ತಲೆ += x_ಎಣಿಕೆ;

    ಆದರೆ( x_ತಲೆ >= px_ಹರಿವು_ಬಫರ್->x_ಉದ್ದಳತೆ )
    {
        x_ತಲೆ -= px_ಹರಿವು_ಬಫರ್->x_ಉದ್ದಳತೆ;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        mtCOVERAGE_TEST_MARKER();
    }

    ಹಿಂತಿರುಗಿ x_ತಲೆ;
}
/*-----------------------------------------------------------*/

ಸ್ಥಿರ size_t prv_Read_ಬೈಟುಗಳು_From_ಬಫರ್( ಹರಿವು_ಬಫರ್_t * px_ಹರಿವು_ಬಫರ್,
                                      uint8_t * puc_ದತ್ತಾಂಶ,
                                      size_t x_ಎಣಿಕೆ,
                                      size_t x_ಬಾಲ )
{
    size_t x_ಮೊದಲ_ಉದ್ದಳತೆ;

    configASSERT( x_ಎಣಿಕೆ != ( size_t ) 0 );

    /* Calculate the number of bytes that can be read - which may be
     * less than the number wanted if the data wraps around to the start of
     * the buffer. */
    x_ಮೊದಲ_ಉದ್ದಳತೆ = configMIN( px_ಹರಿವು_ಬಫರ್->x_ಉದ್ದಳತೆ - x_ಬಾಲ, x_ಎಣಿಕೆ );

    /* Obtain the number of bytes it is possible to obtain in the first
     * read.  Asserts check bounds of read and write. */
    configASSERT( x_ಮೊದಲ_ಉದ್ದಳತೆ <= x_ಎಣಿಕೆ );
    configASSERT( ( x_ಬಾಲ + x_ಮೊದಲ_ಉದ್ದಳತೆ ) <= px_ಹರಿವು_ಬಫರ್->x_ಉದ್ದಳತೆ );
    ( ಶೂನ್ಯ ) ಸ್ಮರಣೆ_ನಕಲು( ( ಶೂನ್ಯ * ) puc_ದತ್ತಾಂಶ, ( ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * ) &( px_ಹರಿವು_ಬಫರ್->puc_ಬಫರ್[ x_ಬಾಲ ] ), x_ಮೊದಲ_ಉದ್ದಳತೆ );

    /* If the total number of wanted bytes is greater than the number
     * that could be read in the first read... */
    ಆದರೆ( x_ಎಣಿಕೆ > x_ಮೊದಲ_ಉದ್ದಳತೆ )
    {
        /* ...then read the remaining bytes from the start of the buffer. */
        ( ಶೂನ್ಯ ) ಸ್ಮರಣೆ_ನಕಲು( ( ಶೂನ್ಯ * ) &( puc_ದತ್ತಾಂಶ[ x_ಮೊದಲ_ಉದ್ದಳತೆ ] ), ( ಶೂನ್ಯ * ) ( px_ಹರಿವು_ಬಫರ್->puc_ಬಫರ್ ), x_ಎಣಿಕೆ - x_ಮೊದಲ_ಉದ್ದಳತೆ );
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        mtCOVERAGE_TEST_MARKER();
    }

    /* Move the tail pointer to effectively remove the data read from the buffer. */
    x_ಬಾಲ += x_ಎಣಿಕೆ;

    ಆದರೆ( x_ಬಾಲ >= px_ಹರಿವು_ಬಫರ್->x_ಉದ್ದಳತೆ )
    {
        x_ಬಾಲ -= px_ಹರಿವು_ಬಫರ್->x_ಉದ್ದಳತೆ;
    }

    ಹಿಂತಿರುಗಿ x_ಬಾಲ;
}
/*-----------------------------------------------------------*/

ಸ್ಥಿರ size_t prv_ಬೈಟುಗಳು_ಒಳ_ಬಫರ್( ಸ್ಥಿರಾಂಕ ಹರಿವು_ಬಫರ್_t * ಸ್ಥಿರಾಂಕ px_ಹರಿವು_ಬಫರ್ )
{
    /* Returns the distance between xTail and xHead. */
    size_t x_ಎಣಿಕೆ;

    x_ಎಣಿಕೆ = px_ಹರಿವು_ಬಫರ್->x_ಉದ್ದಳತೆ + px_ಹರಿವು_ಬಫರ್->x_ತಲೆ;
    x_ಎಣಿಕೆ -= px_ಹರಿವು_ಬಫರ್->x_ಬಾಲ;

    ಆದರೆ( x_ಎಣಿಕೆ >= px_ಹರಿವು_ಬಫರ್->x_ಉದ್ದಳತೆ )
    {
        x_ಎಣಿಕೆ -= px_ಹರಿವು_ಬಫರ್->x_ಉದ್ದಳತೆ;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        mtCOVERAGE_TEST_MARKER();
    }

    ಹಿಂತಿರುಗಿ x_ಎಣಿಕೆ;
}
/*-----------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ prv_Initialise_New_ಹರಿವು_ಬಫರ್( ಹರಿವು_ಬಫರ್_t * ಸ್ಥಿರಾಂಕ px_ಹರಿವು_ಬಫರ್,
                                          uint8_t * ಸ್ಥಿರಾಂಕ puc_ಬಫರ್,
                                          size_t x_ಬಫರ್_ಗಾತ್ರ_ಬೈಟುಗಳು,
                                          size_t x_Trigger_Level_ಬೈಟುಗಳು,
                                          uint8_t ucFlags,
                                          ಹರಿವು_ಬಫರ್_Callback_Function_t pxSendCompletedCallback,
                                          ಹರಿವು_ಬಫರ್_Callback_Function_t pxReceiveCompletedCallback )
{
    /* Assert here is deliberately writing to the entire buffer to ensure it can
     * be written to without generating exceptions, and is setting the buffer to a
     * known value to assist in development/debugging. */
    #ಆದರೆ ( configASSERT_DEFINED == 1 )
    {
        /* The value written just has to be identifiable when looking at the
         * memory.  Don't use 0xA5 as that is the stack fill value and could
         * result in confusion as to what is actually being observed. */
        #ವ್ಯಾಖ್ಯಾನಿಸು ಹರಿವು_ಬಫರ್_ಬಫರ್_WRITE_ಮೌಲ್ಯ    ( 0x55 )
        configASSERT( memset( puc_ಬಫರ್, ( ಪೂರ್ಣಾಂಕ ) ಹರಿವು_ಬಫರ್_ಬಫರ್_WRITE_ಮೌಲ್ಯ, x_ಬಫರ್_ಗಾತ್ರ_ಬೈಟುಗಳು ) == puc_ಬಫರ್ );
    }
    #ಆದರೆ_ಅಂತ್ಯ

    ( ಶೂನ್ಯ ) memset( ( ಶೂನ್ಯ * ) px_ಹರಿವು_ಬಫರ್, 0x00, ಗಾತ್ರ( ಹರಿವು_ಬಫರ್_t ) );
    px_ಹರಿವು_ಬಫರ್->puc_ಬಫರ್ = puc_ಬಫರ್;
    px_ಹರಿವು_ಬಫರ್->x_ಉದ್ದಳತೆ = x_ಬಫರ್_ಗಾತ್ರ_ಬೈಟುಗಳು;
    px_ಹರಿವು_ಬಫರ್->x_Trigger_Level_ಬೈಟುಗಳು = x_Trigger_Level_ಬೈಟುಗಳು;
    px_ಹರಿವು_ಬಫರ್->ucFlags = ucFlags;
    px_ಹರಿವು_ಬಫರ್->ux_Notification_ಸೂಚ್ಯಂಕ = tsk_DEFAULT_ಸೂಚ್ಯಂಕ_ಗಮ್ಯ_NOTIFY;
    #ಆದರೆ ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    {
        px_ಹರಿವು_ಬಫರ್->pxSendCompletedCallback = pxSendCompletedCallback;
        px_ಹರಿವು_ಬಫರ್->pxReceiveCompletedCallback = pxReceiveCompletedCallback;
    }
    #ಇಲ್ಲದಿದ್ದರೆ
    {
        /* MISRA Ref 11.1.1 [Object type casting] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-111 */
        /* coverity[misra_c_2012_rule_11_1_violation] */
        ( ಶೂನ್ಯ ) pxSendCompletedCallback;

        /* MISRA Ref 11.1.1 [Object type casting] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-111 */
        /* coverity[misra_c_2012_rule_11_1_violation] */
        ( ಶೂನ್ಯ ) pxReceiveCompletedCallback;
    }
    #ಆದರೆ_ಅಂತ್ಯ /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
}
/*-----------------------------------------------------------*/

UBaseType_t ux_ಹರಿವು_ಬಫರ್_Get_ಹರಿವು_ಬಫರ್_Notification_ಸೂಚ್ಯಂಕ( ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್ )
{
    ಹರಿವು_ಬಫರ್_t * ಸ್ಥಿರಾಂಕ px_ಹರಿವು_ಬಫರ್ = x_ಹರಿವು_ಬಫರ್;

    trace_ENTER_ux_ಹರಿವು_ಬಫರ್_Get_ಹರಿವು_ಬಫರ್_Notification_ಸೂಚ್ಯಂಕ( x_ಹರಿವು_ಬಫರ್ );

    configASSERT( px_ಹರಿವು_ಬಫರ್ );

    trace_RETURN_ux_ಹರಿವು_ಬಫರ್_Get_ಹರಿವು_ಬಫರ್_Notification_ಸೂಚ್ಯಂಕ( px_ಹರಿವು_ಬಫರ್->ux_Notification_ಸೂಚ್ಯಂಕ );

    ಹಿಂತಿರುಗಿ px_ಹರಿವು_ಬಫರ್->ux_Notification_ಸೂಚ್ಯಂಕ;
}
/*-----------------------------------------------------------*/

ಶೂನ್ಯ v_ಹರಿವು_ಬಫರ್_ಗಣ_ಹರಿವು_ಬಫರ್_Notification_ಸೂಚ್ಯಂಕ( ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್,
                                                    UBaseType_t ux_Notification_ಸೂಚ್ಯಂಕ )
{
    ಹರಿವು_ಬಫರ್_t * ಸ್ಥಿರಾಂಕ px_ಹರಿವು_ಬಫರ್ = x_ಹರಿವು_ಬಫರ್;

    trace_ENTER_v_ಹರಿವು_ಬಫರ್_ಗಣ_ಹರಿವು_ಬಫರ್_Notification_ಸೂಚ್ಯಂಕ( x_ಹರಿವು_ಬಫರ್, ux_Notification_ಸೂಚ್ಯಂಕ );

    configASSERT( px_ಹರಿವು_ಬಫರ್ );

    /* There should be no task waiting otherwise we'd never resume them. */
    configASSERT( px_ಹರಿವು_ಬಫರ್->x_Task_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive == ಖಾಲಿ );
    configASSERT( px_ಹರಿವು_ಬಫರ್->x_Task_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send == ಖಾಲಿ );

    /* Check that the task notification index is valid. */
    configASSERT( ux_Notification_ಸೂಚ್ಯಂಕ < configTASK_NOTIFICATION_ARRAY_ENTRIES );

    px_ಹರಿವು_ಬಫರ್->ux_Notification_ಸೂಚ್ಯಂಕ = ux_Notification_ಸೂಚ್ಯಂಕ;

    trace_RETURN_v_ಹರಿವು_ಬಫರ್_ಗಣ_ಹರಿವು_ಬಫರ್_Notification_ಸೂಚ್ಯಂಕ();
}
/*-----------------------------------------------------------*/

    #ಆದರೆ ( configUSE_TRACE_FACILITY == 1 )

    UBaseType_t ux_ಹರಿವು_ಬಫರ್_Get_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ( ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್ )
    {
        trace_ENTER_ux_ಹರಿವು_ಬಫರ್_Get_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ( x_ಹರಿವು_ಬಫರ್ );

        trace_RETURN_ux_ಹರಿವು_ಬಫರ್_Get_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ( x_ಹರಿವು_ಬಫರ್->ux_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ );

        ಹಿಂತಿರುಗಿ x_ಹರಿವು_ಬಫರ್->ux_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ;
    }

    #ಆದರೆ_ಅಂತ್ಯ /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

    #ಆದರೆ ( configUSE_TRACE_FACILITY == 1 )

    ಶೂನ್ಯ v_ಹರಿವು_ಬಫರ್_ಗಣ_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ( ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್,
                                             UBaseType_t ux_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ )
    {
        trace_ENTER_v_ಹರಿವು_ಬಫರ್_ಗಣ_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ( x_ಹರಿವು_ಬಫರ್, ux_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ );

        x_ಹರಿವು_ಬಫರ್->ux_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ = ux_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ;

        trace_RETURN_v_ಹರಿವು_ಬಫರ್_ಗಣ_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ();
    }

    #ಆದರೆ_ಅಂತ್ಯ /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

    #ಆದರೆ ( configUSE_TRACE_FACILITY == 1 )

    uint8_t uc_ಹರಿವು_ಬಫರ್_Get_ಹರಿವು_ಬಫರ್_Type( ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್ )
    {
        trace_ENTER_uc_ಹರಿವು_ಬಫರ್_Get_ಹರಿವು_ಬಫರ್_Type( x_ಹರಿವು_ಬಫರ್ );

        trace_RETURN_uc_ಹರಿವು_ಬಫರ್_Get_ಹರಿವು_ಬಫರ್_Type( ( uint8_t ) ( x_ಹರಿವು_ಬಫರ್->ucFlags & sb_FLAGS_IS_ಸಂದೇಶ_ಬಫರ್ ) );

        ಹಿಂತಿರುಗಿ( ( uint8_t ) ( x_ಹರಿವು_ಬಫರ್->ucFlags & sb_FLAGS_IS_ಸಂದೇಶ_ಬಫರ್ ) );
    }

    #ಆದರೆ_ಅಂತ್ಯ /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

/* This entire source file will be skipped if the application is not configured
 * to include stream buffer functionality. This #if is closed at the very bottom
 * of this file. If you want to include stream buffers then ensure
 * configUSE_STREAM_BUFFERS is set to 1 in FreeRTOSConfig.h. */
#ಆದರೆ_ಅಂತ್ಯ /* configUSE_STREAM_BUFFERS == 1 */
