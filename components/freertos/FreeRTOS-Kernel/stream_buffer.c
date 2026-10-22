/*
 * FreeRTOS Kernel V10.5.1 (ESP-IDF SMP modified)
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* FreeRTOS includes. */
#ਅਨ੍ਤਰ੍ਭਾਵ "FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stream_buffer.h"
/* Include private IDF API additions for critical thread safety macros */
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_private/freertos_idf_additions_priv.h"

#ਜੇ ( configUSE_TASK_NOTIFICATIONS != 1 )
    #ਗਲਤੀ configUSE_TASK_NOTIFICATIONS must be ਸੈੱਟ ਮੰਜ਼ਿਲ 1 ਮੰਜ਼ਿਲ build ਧਾਰਾ_ਬਫਰ.c
#ਜੇ_ਅੰਤ

#ਜੇ ( INCLUDE_xTaskGetCurrentTaskHandle != 1 )
    #ਗਲਤੀ INCLUDE_xTaskGetCurrentTaskHandle must be ਸੈੱਟ ਮੰਜ਼ਿਲ 1 ਮੰਜ਼ਿਲ build ਧਾਰਾ_ਬਫਰ.c
#ਜੇ_ਅੰਤ

/* Lint e961, e9021 and e750 are suppressed as a MISRA exception justified
 * because the MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ MPU_WRAPPERS_INCLUDED_FROM_API_FILE /*lint !e961 !e750 !e9021. */

/* If the user has not provided application specific Rx notification macros,
 * or #defined the notification macros away, then provide default implementations
 * that uses task notifications. */
/*lint -save -e9026 Function like macros allowed and needed here so they can be overridden. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ sbRECEIVE_COMPLETED
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sbRECEIVE_COMPLETED( px_ਧਾਰਾ_ਬਫਰ )                               \
    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( px_ਧਾਰਾ_ਬਫਰ->x_ਧਾਰਾ_ਬਫਰ_ਤਾਲਾ ) ); \
    {                                                                           \
        ਜੇ( ( px_ਧਾਰਾ_ਬਫਰ )->x_Task_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send != ਨਿਰਾ )                    \
        {                                                                       \
            ( ਖਾਲੀ ) xTaskNotify( ( px_ਧਾਰਾ_ਬਫਰ )->x_Task_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send,       \
                                  ( uint32_t ) 0,                               \
                                  eNoAction );                                  \
            ( px_ਧਾਰਾ_ਬਫਰ )->x_Task_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send = ਨਿਰਾ;                      \
        }                                                                       \
    }                                                                           \
    ( ਖਾਲੀ ) prvEXIT_CRITICAL_OR_RESUME_ALL( &( px_ਧਾਰਾ_ਬਫਰ->x_ਧਾਰਾ_ਬਫਰ_ਤਾਲਾ ) );
#ਜੇ_ਅੰਤ /* sbRECEIVE_COMPLETED */

/* If user has provided a per-instance receive complete callback, then
 * invoke the callback else use the receive complete macro which is provided by default for all instances.
 */
#ਜੇ ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ prvRECEIVE_COMPLETED( px_ਧਾਰਾ_ਬਫਰ )                                               \
    {                                                                                            \
        ਜੇ( ( px_ਧਾਰਾ_ਬਫਰ )->pxReceiveCompletedCallback != ਨਿਰਾ )                             \
        {                                                                                        \
            ( px_ਧਾਰਾ_ਬਫਰ )->pxReceiveCompletedCallback( ( px_ਧਾਰਾ_ਬਫਰ ), pdFALSE, ਨਿਰਾ ); \
        }                                                                                        \
        ਨਹੀਂਤਾਂ                                                                                     \
        {                                                                                        \
            sbRECEIVE_COMPLETED( ( px_ਧਾਰਾ_ਬਫਰ ) );                                           \
        }                                                                                        \
    }
#ਨਹੀਂਤਾਂ /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ prvRECEIVE_COMPLETED( px_ਧਾਰਾ_ਬਫਰ )    sbRECEIVE_COMPLETED( ( px_ਧਾਰਾ_ਬਫਰ ) )
#ਜੇ_ਅੰਤ /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ sbRECEIVE_COMPLETED_FROM_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sbRECEIVE_COMPLETED_FROM_ISR( px_ਧਾਰਾ_ਬਫਰ,                            \
                                          px_Higher_ਤਰਜੀਹ_Task_Woken )                \
    {                                                                                \
        UBaseType_t ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ;                                          \
                                                                                     \
        ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ = ( UBaseType_t ) ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR();  \
        {                                                                            \
            ਜੇ( ( px_ਧਾਰਾ_ਬਫਰ )->x_Task_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send != ਨਿਰਾ )                     \
            {                                                                        \
                ( ਖਾਲੀ ) xTaskNotifyFromISR( ( px_ਧਾਰਾ_ਬਫਰ )->x_Task_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send, \
                                             ( uint32_t ) 0,                         \
                                             eNoAction,                              \
                                             ( px_Higher_ਤਰਜੀਹ_Task_Woken ) );        \
                ( px_ਧਾਰਾ_ਬਫਰ )->x_Task_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send = ਨਿਰਾ;                       \
            }                                                                        \
        }                                                                            \
        ਪੋਰਟ_CLEAR_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR( ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ );                 \
    }
#ਜੇ_ਅੰਤ /* sbRECEIVE_COMPLETED_FROM_ISR */

#ਜੇ ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ prvRECEIVE_COMPLETED_FROM_ISR( px_ਧਾਰਾ_ਬਫਰ,                                                               \
                                           px_Higher_ਤਰਜੀਹ_Task_Woken )                                                   \
    {                                                                                                                    \
        ਜੇ( ( px_ਧਾਰਾ_ਬਫਰ )->pxReceiveCompletedCallback != ਨਿਰਾ )                                                     \
        {                                                                                                                \
            ( px_ਧਾਰਾ_ਬਫਰ )->pxReceiveCompletedCallback( ( px_ਧਾਰਾ_ਬਫਰ ), pdTRUE, ( px_Higher_ਤਰਜੀਹ_Task_Woken ) ); \
        }                                                                                                                \
        ਨਹੀਂਤਾਂ                                                                                                             \
        {                                                                                                                \
            sbRECEIVE_COMPLETED_FROM_ISR( ( px_ਧਾਰਾ_ਬਫਰ ), ( px_Higher_ਤਰਜੀਹ_Task_Woken ) );                           \
        }                                                                                                                \
    }
#ਨਹੀਂਤਾਂ /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ prvRECEIVE_COMPLETED_FROM_ISR( px_ਧਾਰਾ_ਬਫਰ, px_Higher_ਤਰਜੀਹ_Task_Woken ) \
    sbRECEIVE_COMPLETED_FROM_ISR( ( px_ਧਾਰਾ_ਬਫਰ ), ( px_Higher_ਤਰਜੀਹ_Task_Woken ) )
#ਜੇ_ਅੰਤ /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

/* If the user has not provided an application specific Tx notification macro,
 * or #defined the notification macro away, then provide a default
 * implementation that uses task notifications.
 */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ sbSEND_COMPLETED
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sbSEND_COMPLETED( px_ਧਾਰਾ_ਬਫਰ )                                  \
    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( px_ਧਾਰਾ_ਬਫਰ->x_ਧਾਰਾ_ਬਫਰ_ਤਾਲਾ ) ); \
    {                                                                           \
        ਜੇ( ( px_ਧਾਰਾ_ਬਫਰ )->x_Task_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive != ਨਿਰਾ )                 \
        {                                                                       \
            ( ਖਾਲੀ ) xTaskNotify( ( px_ਧਾਰਾ_ਬਫਰ )->x_Task_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive,    \
                                  ( uint32_t ) 0,                               \
                                  eNoAction );                                  \
            ( px_ਧਾਰਾ_ਬਫਰ )->x_Task_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive = ਨਿਰਾ;                   \
        }                                                                       \
    }                                                                           \
    ( ਖਾਲੀ ) prvEXIT_CRITICAL_OR_RESUME_ALL( &( px_ਧਾਰਾ_ਬਫਰ->x_ਧਾਰਾ_ਬਫਰ_ਤਾਲਾ ) );
#ਜੇ_ਅੰਤ /* sbSEND_COMPLETED */

/* If user has provided a per-instance send completed callback, then
 * invoke the callback else use the send complete macro which is provided by default for all instances.
 */
#ਜੇ ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ prvSEND_COMPLETED( px_ਧਾਰਾ_ਬਫਰ )                                           \
    {                                                                                     \
        ਜੇ( ( px_ਧਾਰਾ_ਬਫਰ )->pxSendCompletedCallback != ਨਿਰਾ )                         \
        {                                                                                 \
            px_ਧਾਰਾ_ਬਫਰ->pxSendCompletedCallback( ( px_ਧਾਰਾ_ਬਫਰ ), pdFALSE, ਨਿਰਾ ); \
        }                                                                                 \
        ਨਹੀਂਤਾਂ                                                                              \
        {                                                                                 \
            sbSEND_COMPLETED( ( px_ਧਾਰਾ_ਬਫਰ ) );                                       \
        }                                                                                 \
    }
#ਨਹੀਂਤਾਂ /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ prvSEND_COMPLETED( px_ਧਾਰਾ_ਬਫਰ )    sbSEND_COMPLETED( ( px_ਧਾਰਾ_ਬਫਰ ) )
#ਜੇ_ਅੰਤ /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ sbSEND_COMPLETE_FROM_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sbSEND_COMPLETE_FROM_ISR( px_ਧਾਰਾ_ਬਫਰ, px_Higher_ਤਰਜੀਹ_Task_Woken )       \
    {                                                                                   \
        UBaseType_t ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ;                                             \
                                                                                        \
        ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ = ( UBaseType_t ) ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR();     \
        {                                                                               \
            ਜੇ( ( px_ਧਾਰਾ_ਬਫਰ )->x_Task_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive != ਨਿਰਾ )                     \
            {                                                                           \
                ( ਖਾਲੀ ) xTaskNotifyFromISR( ( px_ਧਾਰਾ_ਬਫਰ )->x_Task_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive, \
                                             ( uint32_t ) 0,                            \
                                             eNoAction,                                 \
                                             ( px_Higher_ਤਰਜੀਹ_Task_Woken ) );           \
                ( px_ਧਾਰਾ_ਬਫਰ )->x_Task_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive = ਨਿਰਾ;                       \
            }                                                                           \
        }                                                                               \
        ਪੋਰਟ_CLEAR_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR( ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ );                    \
    }
#ਜੇ_ਅੰਤ /* sbSEND_COMPLETE_FROM_ISR */


#ਜੇ ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ prvSEND_COMPLETE_FROM_ISR( px_ਧਾਰਾ_ਬਫਰ, px_Higher_ਤਰਜੀਹ_Task_Woken )                                    \
    {                                                                                                                 \
        ਜੇ( ( px_ਧਾਰਾ_ਬਫਰ )->pxSendCompletedCallback != ਨਿਰਾ )                                                     \
        {                                                                                                             \
            ( px_ਧਾਰਾ_ਬਫਰ )->pxSendCompletedCallback( ( px_ਧਾਰਾ_ਬਫਰ ), pdTRUE, ( px_Higher_ਤਰਜੀਹ_Task_Woken ) ); \
        }                                                                                                             \
        ਨਹੀਂਤਾਂ                                                                                                          \
        {                                                                                                             \
            sbSEND_COMPLETE_FROM_ISR( ( px_ਧਾਰਾ_ਬਫਰ ), ( px_Higher_ਤਰਜੀਹ_Task_Woken ) );                            \
        }                                                                                                             \
    }
#ਨਹੀਂਤਾਂ /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ prvSEND_COMPLETE_FROM_ISR( px_ਧਾਰਾ_ਬਫਰ, px_Higher_ਤਰਜੀਹ_Task_Woken ) \
    sbSEND_COMPLETE_FROM_ISR( ( px_ਧਾਰਾ_ਬਫਰ ), ( px_Higher_ਤਰਜੀਹ_Task_Woken ) )
#ਜੇ_ਅੰਤ /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

/*lint -restore (9026) */

/* The number of bytes used to hold the length of a message in the buffer. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sb_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_STORE_ਸੁਨੇਹਾ_ਲੰਮਾਈ    ( ਆਕਾਰ( config_ਸੁਨੇਹਾ_ਬਫਰ_ਲੰਮਾਈ_TYPE ) )

/* Bits stored in the ucFlags field of the stream buffer. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sb_FLAGS_IS_ਸੁਨੇਹਾ_ਬਫਰ          ( ( uint8_t ) 1 ) /* Set if the stream buffer was created as a message buffer, in which case it holds discrete messages rather than a stream. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sbFLAGS_IS_STATICALLY_ALLOCATED    ( ( uint8_t ) 2 ) /* Set if the stream buffer was created using statically allocated memory. */

/*-----------------------------------------------------------*/

/* Structure that hold state information on the buffer. */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ਧਾਰਾ_ਬਫਰ_Def_t                 /*lint !e9058 Style convention uses tag. */
{
    ਅਸਥਿਰ size_t x_ਪੂਛ;                       /* Index to the next item to read within the buffer. */
    ਅਸਥਿਰ size_t x_ਸਿਰ;                       /* Index to the next item to write within the buffer. */
    size_t x_ਲੰਮਾਈ;                              /* The length of the buffer pointed to by pucBuffer. */
    size_t x_Trigger_Level_ਬਾਈਟਾਂ;                   /* The number of bytes that must be in the stream buffer before a task that is waiting for data is unblocked. */
    ਅਸਥਿਰ TaskHandle_t x_Task_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive; /* Holds the handle of a task waiting for data, or NULL if no tasks are waiting. */
    ਅਸਥਿਰ TaskHandle_t x_Task_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send;    /* Holds the handle of a task waiting to send data to a message buffer that is full. */
    uint8_t * puc_ਬਫਰ;                         /* Points to the buffer itself - that is - the RAM that stores the data passed through the buffer. */
    uint8_t ucFlags;

    #ਜੇ ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_ਧਾਰਾ_ਬਫਰ_ਸੰਖਿਆ; /* Used for tracing purposes. */
    #ਜੇ_ਅੰਤ

    #ਜੇ ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        ਧਾਰਾ_ਬਫਰ_Callback_Function_t pxSendCompletedCallback;    /* Optional callback called on send complete. sbSEND_COMPLETED is called if this is NULL. */
        ਧਾਰਾ_ਬਫਰ_Callback_Function_t pxReceiveCompletedCallback; /* Optional callback called on receive complete.  sbRECEIVE_COMPLETED is called if this is NULL. */
    #ਜੇ_ਅੰਤ

    ਪੋਰਟ_MUX_TYPE x_ਧਾਰਾ_ਬਫਰ_ਤਾਲਾ; /* Spinlock required for SMP critical sections */
} ਧਾਰਾ_ਬਫਰ_t;

/*
 * The number of bytes available to be read from the buffer.
 */
ਸਥਿਰ size_t prv_ਬਾਈਟਾਂ_ਵਿੱਚ_ਬਫਰ( ਅਚਲ ਧਾਰਾ_ਬਫਰ_t * ਅਚਲ px_ਧਾਰਾ_ਬਫਰ ) PRIVILEGED_FUNCTION;

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
ਸਥਿਰ size_t prv_Write_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_ਬਫਰ( ਧਾਰਾ_ਬਫਰ_t * ਅਚਲ px_ਧਾਰਾ_ਬਫਰ,
                                     ਅਚਲ uint8_t * puc_ਅੰਕੜੇ,
                                     size_t x_ਗਿਣਤੀ,
                                     size_t x_ਸਿਰ ) PRIVILEGED_FUNCTION;

/*
 * If the stream buffer is being used as a message buffer, then reads an entire
 * message out of the buffer.  If the stream buffer is being used as a stream
 * buffer then read as many bytes as possible from the buffer.
 * prvReadBytesFromBuffer() is called to actually extract the bytes from the
 * buffer's data storage area.
 */
ਸਥਿਰ size_t prv_Read_ਸੁਨੇਹਾ_From_ਬਫਰ( ਧਾਰਾ_ਬਫਰ_t * px_ਧਾਰਾ_ਬਫਰ,
                                        ਖਾਲੀ * pv_Rx_ਅੰਕੜੇ,
                                        size_t x_ਬਫਰ_ਲੰਮਾਈ_ਬਾਈਟਾਂ,
                                        size_t x_ਬਾਈਟਾਂ_ਉਪਲਬਧ ) PRIVILEGED_FUNCTION;

/*
 * If the stream buffer is being used as a message buffer, then writes an entire
 * message to the buffer.  If the stream buffer is being used as a stream
 * buffer then write as many bytes as possible to the buffer.
 * prvWriteBytestoBuffer() is called to actually send the bytes to the buffer's
 * data storage area.
 */
ਸਥਿਰ size_t prv_Write_ਸੁਨੇਹਾ_ਮੰਜ਼ਿਲ_ਬਫਰ( ਧਾਰਾ_ਬਫਰ_t * ਅਚਲ px_ਧਾਰਾ_ਬਫਰ,
                                       ਅਚਲ ਖਾਲੀ * pv_Tx_ਅੰਕੜੇ,
                                       size_t x_ਅੰਕੜੇ_ਲੰਮਾਈ_ਬਾਈਟਾਂ,
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
ਸਥਿਰ size_t prv_Read_ਬਾਈਟਾਂ_From_ਬਫਰ( ਧਾਰਾ_ਬਫਰ_t * px_ਧਾਰਾ_ਬਫਰ,
                                      uint8_t * puc_ਅੰਕੜੇ,
                                      size_t x_ਗਿਣਤੀ,
                                      size_t x_ਪੂਛ ) PRIVILEGED_FUNCTION;

/*
 * Called by both pxStreamBufferCreate() and pxStreamBufferCreateStatic() to
 * initialise the members of the newly created stream buffer structure.
 */
ਸਥਿਰ ਖਾਲੀ prv_Initialise_New_ਧਾਰਾ_ਬਫਰ( ਧਾਰਾ_ਬਫਰ_t * ਅਚਲ px_ਧਾਰਾ_ਬਫਰ,
                                          uint8_t * ਅਚਲ puc_ਬਫਰ,
                                          size_t x_ਬਫਰ_ਮਾਪ_ਬਾਈਟਾਂ,
                                          size_t x_Trigger_Level_ਬਾਈਟਾਂ,
                                          uint8_t ucFlags,
                                          ਧਾਰਾ_ਬਫਰ_Callback_Function_t pxSendCompletedCallback,
                                          ਧਾਰਾ_ਬਫਰ_Callback_Function_t pxReceiveCompletedCallback ) PRIVILEGED_FUNCTION;

/*-----------------------------------------------------------*/
#ਜੇ ( config_SUPPORT_DYNAMIC_ਵੰਡ == 1 )
    ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ_Generic_Create( size_t x_ਬਫਰ_ਮਾਪ_ਬਾਈਟਾਂ,
                                                     size_t x_Trigger_Level_ਬਾਈਟਾਂ,
                                                     ਅਧਾਰ_Type_t x_Is_ਸੁਨੇਹਾ_ਬਫਰ,
                                                     ਧਾਰਾ_ਬਫਰ_Callback_Function_t pxSendCompletedCallback,
                                                     ਧਾਰਾ_ਬਫਰ_Callback_Function_t pxReceiveCompletedCallback )
    {
        uint8_t * puc_Allocated_ਸਮ੍ਰਿਤੀ;
        uint8_t ucFlags;

        /* In case the stream buffer is going to be used as a message buffer
         * (that is, it will hold discrete messages with a little meta data that
         * says how big the next message is) check the buffer will be large enough
         * to hold at least one message. */
        ਜੇ( x_Is_ਸੁਨੇਹਾ_ਬਫਰ == pdTRUE )
        {
            /* Is a message buffer but not statically allocated. */
            ucFlags = sb_FLAGS_IS_ਸੁਨੇਹਾ_ਬਫਰ;
            configASSERT( x_ਬਫਰ_ਮਾਪ_ਬਾਈਟਾਂ > sb_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_STORE_ਸੁਨੇਹਾ_ਲੰਮਾਈ );
        }
        ਨਹੀਂਤਾਂ
        {
            /* Not a message buffer and not statically allocated. */
            ucFlags = 0;
            configASSERT( x_ਬਫਰ_ਮਾਪ_ਬਾਈਟਾਂ > 0 );
        }

        configASSERT( x_Trigger_Level_ਬਾਈਟਾਂ <= x_ਬਫਰ_ਮਾਪ_ਬਾਈਟਾਂ );

        /* A trigger level of 0 would cause a waiting task to unblock even when
         * the buffer was empty. */
        ਜੇ( x_Trigger_Level_ਬਾਈਟਾਂ == ( size_t ) 0 )
        {
            x_Trigger_Level_ਬਾਈਟਾਂ = ( size_t ) 1;
        }

        /* A stream buffer requires a StreamBuffer_t structure and a buffer.
         * Both are allocated in a single call to pvPortMalloc().  The
         * StreamBuffer_t structure is placed at the start of the allocated memory
         * and the buffer follows immediately after.  The requested size is
         * incremented so the free space is returned as the user would expect -
         * this is a quirk of the implementation that means otherwise the free
         * space would be reported as one byte smaller than would be logically
         * expected. */
        ਜੇ( x_ਬਫਰ_ਮਾਪ_ਬਾਈਟਾਂ < ( x_ਬਫਰ_ਮਾਪ_ਬਾਈਟਾਂ + 1 + ਆਕਾਰ( ਧਾਰਾ_ਬਫਰ_t ) ) )
        {
            x_ਬਫਰ_ਮਾਪ_ਬਾਈਟਾਂ++;
            puc_Allocated_ਸਮ੍ਰਿਤੀ = ( uint8_t * ) pv_ਪੋਰਟ_Malloc( x_ਬਫਰ_ਮਾਪ_ਬਾਈਟਾਂ + ਆਕਾਰ( ਧਾਰਾ_ਬਫਰ_t ) ); /*lint !e9079 malloc() only returns void*. */
        }
        ਨਹੀਂਤਾਂ
        {
            puc_Allocated_ਸਮ੍ਰਿਤੀ = ਨਿਰਾ;
        }

        ਜੇ( puc_Allocated_ਸਮ੍ਰਿਤੀ != ਨਿਰਾ )
        {
            prv_Initialise_New_ਧਾਰਾ_ਬਫਰ( ( ਧਾਰਾ_ਬਫਰ_t * ) puc_Allocated_ਸਮ੍ਰਿਤੀ,       /* Structure at the start of the allocated memory. */ /*lint !e9087 Safe cast as allocated memory is aligned. */ /*lint !e826 Area is not too small and alignment is guaranteed provided malloc() behaves as expected and returns aligned buffer. */
                                          puc_Allocated_ਸਮ੍ਰਿਤੀ + ਆਕਾਰ( ਧਾਰਾ_ਬਫਰ_t ), /* Storage area follows. */ /*lint !e9016 Indexing past structure valid for uint8_t pointer, also storage area has no alignment requirement. */
                                          x_ਬਫਰ_ਮਾਪ_ਬਾਈਟਾਂ,
                                          x_Trigger_Level_ਬਾਈਟਾਂ,
                                          ucFlags,
                                          pxSendCompletedCallback,
                                          pxReceiveCompletedCallback );

            /* Initialize the stream buffer's spinlock separately, as
             * prvInitialiseNewStreamBuffer() is also called from
             * xStreamBufferReset(). */
            ਪੋਰਟ_MUX_INITIALIZE( &( ( ( ਧਾਰਾ_ਬਫਰ_t * ) puc_Allocated_ਸਮ੍ਰਿਤੀ )->x_ਧਾਰਾ_ਬਫਰ_ਤਾਲਾ ) );

            trace_ਧਾਰਾ_ਬਫਰ_CREATE( ( ( ਧਾਰਾ_ਬਫਰ_t * ) puc_Allocated_ਸਮ੍ਰਿਤੀ ), x_Is_ਸੁਨੇਹਾ_ਬਫਰ );
        }
        ਨਹੀਂਤਾਂ
        {
            trace_ਧਾਰਾ_ਬਫਰ_CREATE_FAILED( x_Is_ਸੁਨੇਹਾ_ਬਫਰ );
        }

        ਵਾਪਸ ( ਧਾਰਾ_ਬਫਰ_Handle_t ) puc_Allocated_ਸਮ੍ਰਿਤੀ; /*lint !e9087 !e826 Safe cast as allocated memory is aligned. */
    }
#ਜੇ_ਅੰਤ /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

#ਜੇ ( config_SUPPORT_STATIC_ਵੰਡ == 1 )

    ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ_Generic_Create_Static( size_t x_ਬਫਰ_ਮਾਪ_ਬਾਈਟਾਂ,
                                                           size_t x_Trigger_Level_ਬਾਈਟਾਂ,
                                                           ਅਧਾਰ_Type_t x_Is_ਸੁਨੇਹਾ_ਬਫਰ,
                                                           uint8_t * ਅਚਲ puc_ਧਾਰਾ_ਬਫਰ_Storage_Area,
                                                           Static_ਧਾਰਾ_ਬਫਰ_t * ਅਚਲ px_Static_ਧਾਰਾ_ਬਫਰ,
                                                           ਧਾਰਾ_ਬਫਰ_Callback_Function_t pxSendCompletedCallback,
                                                           ਧਾਰਾ_ਬਫਰ_Callback_Function_t pxReceiveCompletedCallback )
    {
        ਧਾਰਾ_ਬਫਰ_t * ਅਚਲ px_ਧਾਰਾ_ਬਫਰ = ( ਧਾਰਾ_ਬਫਰ_t * ) px_Static_ਧਾਰਾ_ਬਫਰ; /*lint !e740 !e9087 Safe cast as StaticStreamBuffer_t is opaque Streambuffer_t. */
        ਧਾਰਾ_ਬਫਰ_Handle_t xReturn;
        uint8_t ucFlags;

        configASSERT( puc_ਧਾਰਾ_ਬਫਰ_Storage_Area );
        configASSERT( px_Static_ਧਾਰਾ_ਬਫਰ );
        configASSERT( x_Trigger_Level_ਬਾਈਟਾਂ <= x_ਬਫਰ_ਮਾਪ_ਬਾਈਟਾਂ );

        /* A trigger level of 0 would cause a waiting task to unblock even when
         * the buffer was empty. */
        ਜੇ( x_Trigger_Level_ਬਾਈਟਾਂ == ( size_t ) 0 )
        {
            x_Trigger_Level_ਬਾਈਟਾਂ = ( size_t ) 1;
        }

        ਜੇ( x_Is_ਸੁਨੇਹਾ_ਬਫਰ != pdFALSE )
        {
            /* Statically allocated message buffer. */
            ucFlags = sb_FLAGS_IS_ਸੁਨੇਹਾ_ਬਫਰ | sbFLAGS_IS_STATICALLY_ALLOCATED;
        }
        ਨਹੀਂਤਾਂ
        {
            /* Statically allocated stream buffer. */
            ucFlags = sbFLAGS_IS_STATICALLY_ALLOCATED;
        }

        /* In case the stream buffer is going to be used as a message buffer
         * (that is, it will hold discrete messages with a little meta data that
         * says how big the next message is) check the buffer will be large enough
         * to hold at least one message. */
        configASSERT( x_ਬਫਰ_ਮਾਪ_ਬਾਈਟਾਂ > sb_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_STORE_ਸੁਨੇਹਾ_ਲੰਮਾਈ );

        #ਜੇ ( configASSERT_DEFINED == 1 )
        {
            /* Sanity check that the size of the structure used to declare a
             * variable of type StaticStreamBuffer_t equals the size of the real
             * message buffer structure. */
            ਅਸਥਿਰ size_t x_ਮਾਪ = ਆਕਾਰ( Static_ਧਾਰਾ_ਬਫਰ_t );
            configASSERT( x_ਮਾਪ == ਆਕਾਰ( ਧਾਰਾ_ਬਫਰ_t ) );
            ( ਖਾਲੀ ) x_ਮਾਪ; /* Prevent unused variable warning when configASSERT() is not used. */
        } /*lint !e529 xSize is referenced is configASSERT() is defined. */
        #ਜੇ_ਅੰਤ /* configASSERT_DEFINED */

        ਜੇ( ( puc_ਧਾਰਾ_ਬਫਰ_Storage_Area != ਨਿਰਾ ) && ( px_Static_ਧਾਰਾ_ਬਫਰ != ਨਿਰਾ ) )
        {
            prv_Initialise_New_ਧਾਰਾ_ਬਫਰ( px_ਧਾਰਾ_ਬਫਰ,
                                          puc_ਧਾਰਾ_ਬਫਰ_Storage_Area,
                                          x_ਬਫਰ_ਮਾਪ_ਬਾਈਟਾਂ,
                                          x_Trigger_Level_ਬਾਈਟਾਂ,
                                          ucFlags,
                                          pxSendCompletedCallback,
                                          pxReceiveCompletedCallback );

            /* Remember this was statically allocated in case it is ever deleted
             * again. */
            px_ਧਾਰਾ_ਬਫਰ->ucFlags |= sbFLAGS_IS_STATICALLY_ALLOCATED;

            /* Initialize the stream buffer's spinlock separately, as
             * prvInitialiseNewStreamBuffer() is also called from
             * xStreamBufferReset(). */
            ਪੋਰਟ_MUX_INITIALIZE( &( px_ਧਾਰਾ_ਬਫਰ->x_ਧਾਰਾ_ਬਫਰ_ਤਾਲਾ ) );

            trace_ਧਾਰਾ_ਬਫਰ_CREATE( px_ਧਾਰਾ_ਬਫਰ, x_Is_ਸੁਨੇਹਾ_ਬਫਰ );

            xReturn = ( ਧਾਰਾ_ਬਫਰ_Handle_t ) px_Static_ਧਾਰਾ_ਬਫਰ; /*lint !e9087 Data hiding requires cast to opaque type. */
        }
        ਨਹੀਂਤਾਂ
        {
            xReturn = ਨਿਰਾ;
            trace_ਧਾਰਾ_ਬਫਰ_CREATE_STATIC_FAILED( xReturn, x_Is_ਸੁਨੇਹਾ_ਬਫਰ );
        }

        ਵਾਪਸ xReturn;
    }
#ਜੇ_ਅੰਤ /* ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*-----------------------------------------------------------*/

#ਜੇ ( config_SUPPORT_STATIC_ਵੰਡ == 1 )
    ਅਧਾਰ_Type_t x_ਧਾਰਾ_ਬਫਰ_Get_Static_Buffers( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ,
                                              uint8_t ** ppuc_ਧਾਰਾ_ਬਫਰ_Storage_Area,
                                              Static_ਧਾਰਾ_ਬਫਰ_t ** ppx_Static_ਧਾਰਾ_ਬਫਰ )
    {
        ਅਧਾਰ_Type_t xReturn;
        ਅਚਲ ਧਾਰਾ_ਬਫਰ_t * ਅਚਲ px_ਧਾਰਾ_ਬਫਰ = x_ਧਾਰਾ_ਬਫਰ;

        configASSERT( px_ਧਾਰਾ_ਬਫਰ );
        configASSERT( ppuc_ਧਾਰਾ_ਬਫਰ_Storage_Area );
        configASSERT( ppx_Static_ਧਾਰਾ_ਬਫਰ );

        ਜੇ( ( px_ਧਾਰਾ_ਬਫਰ->ucFlags & sbFLAGS_IS_STATICALLY_ALLOCATED ) != ( uint8_t ) 0 )
        {
            *ppuc_ਧਾਰਾ_ਬਫਰ_Storage_Area = px_ਧਾਰਾ_ਬਫਰ->puc_ਬਫਰ;
            *ppx_Static_ਧਾਰਾ_ਬਫਰ = ( Static_ਧਾਰਾ_ਬਫਰ_t * ) px_ਧਾਰਾ_ਬਫਰ;
            xReturn = pdTRUE;
        }
        ਨਹੀਂਤਾਂ
        {
            xReturn = pdFALSE;
        }

        ਵਾਪਸ xReturn;
    }
#ਜੇ_ਅੰਤ /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

ਖਾਲੀ v_ਧਾਰਾ_ਬਫਰ_Delete( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ )
{
    ਧਾਰਾ_ਬਫਰ_t * px_ਧਾਰਾ_ਬਫਰ = x_ਧਾਰਾ_ਬਫਰ;

    configASSERT( px_ਧਾਰਾ_ਬਫਰ );

    trace_ਧਾਰਾ_ਬਫਰ_DELETE( x_ਧਾਰਾ_ਬਫਰ );

    ਜੇ( ( px_ਧਾਰਾ_ਬਫਰ->ucFlags & sbFLAGS_IS_STATICALLY_ALLOCATED ) == ( uint8_t ) pdFALSE )
    {
        #ਜੇ ( config_SUPPORT_DYNAMIC_ਵੰਡ == 1 )
        {
            /* Both the structure and the buffer were allocated using a single call
            * to pvPortMalloc(), hence only one call to vPortFree() is required. */
            v_ਪੋਰਟ_Free( ( ਖਾਲੀ * ) px_ਧਾਰਾ_ਬਫਰ ); /*lint !e9087 Standard free() semantics require void *, plus pxStreamBuffer was allocated by pvPortMalloc(). */
        }
        #ਨਹੀਂਤਾਂ
        {
            /* Should not be possible to get here, ucFlags must be corrupt.
             * Force an assert. */
            configASSERT( x_ਧਾਰਾ_ਬਫਰ == ( ਧਾਰਾ_ਬਫਰ_Handle_t ) ~0 );
        }
        #ਜੇ_ਅੰਤ
    }
    ਨਹੀਂਤਾਂ
    {
        /* The structure and buffer were not allocated dynamically and cannot be
         * freed - just scrub the structure so future use will assert. */
        ( ਖਾਲੀ ) memset( px_ਧਾਰਾ_ਬਫਰ, 0x00, ਆਕਾਰ( ਧਾਰਾ_ਬਫਰ_t ) );
    }
}
/*-----------------------------------------------------------*/

ਅਧਾਰ_Type_t x_ਧਾਰਾ_ਬਫਰ_Reset( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ )
{
    ਧਾਰਾ_ਬਫਰ_t * ਅਚਲ px_ਧਾਰਾ_ਬਫਰ = x_ਧਾਰਾ_ਬਫਰ;
    ਅਧਾਰ_Type_t xReturn = pdFAIL;
    ਧਾਰਾ_ਬਫਰ_Callback_Function_t pxSendCallback = ਨਿਰਾ, pxReceiveCallback = ਨਿਰਾ;

    #ਜੇ ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_ਧਾਰਾ_ਬਫਰ_ਸੰਖਿਆ;
    #ਜੇ_ਅੰਤ

    configASSERT( px_ਧਾਰਾ_ਬਫਰ );

    #ਜੇ ( configUSE_TRACE_FACILITY == 1 )
    {
        /* Store the stream buffer number so it can be restored after the
         * reset. */
        ux_ਧਾਰਾ_ਬਫਰ_ਸੰਖਿਆ = px_ਧਾਰਾ_ਬਫਰ->ux_ਧਾਰਾ_ਬਫਰ_ਸੰਖਿਆ;
    }
    #ਜੇ_ਅੰਤ

    /* Can only reset a message buffer if there are no tasks blocked on it. */
    taskENTER_CRITICAL( &( px_ਧਾਰਾ_ਬਫਰ->x_ਧਾਰਾ_ਬਫਰ_ਤਾਲਾ ) );
    {
        ਜੇ( ( px_ਧਾਰਾ_ਬਫਰ->x_Task_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive == ਨਿਰਾ ) && ( px_ਧਾਰਾ_ਬਫਰ->x_Task_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send == ਨਿਰਾ ) )
        {
            #ਜੇ ( configUSE_SB_COMPLETED_CALLBACK == 1 )
            {
                pxSendCallback = px_ਧਾਰਾ_ਬਫਰ->pxSendCompletedCallback;
                pxReceiveCallback = px_ਧਾਰਾ_ਬਫਰ->pxReceiveCompletedCallback;
            }
            #ਜੇ_ਅੰਤ

            prv_Initialise_New_ਧਾਰਾ_ਬਫਰ( px_ਧਾਰਾ_ਬਫਰ,
                                          px_ਧਾਰਾ_ਬਫਰ->puc_ਬਫਰ,
                                          px_ਧਾਰਾ_ਬਫਰ->x_ਲੰਮਾਈ,
                                          px_ਧਾਰਾ_ਬਫਰ->x_Trigger_Level_ਬਾਈਟਾਂ,
                                          px_ਧਾਰਾ_ਬਫਰ->ucFlags,
                                          pxSendCallback,
                                          pxReceiveCallback );

            #ਜੇ ( configUSE_TRACE_FACILITY == 1 )
            {
                px_ਧਾਰਾ_ਬਫਰ->ux_ਧਾਰਾ_ਬਫਰ_ਸੰਖਿਆ = ux_ਧਾਰਾ_ਬਫਰ_ਸੰਖਿਆ;
            }
            #ਜੇ_ਅੰਤ

            trace_ਧਾਰਾ_ਬਫਰ_RESET( x_ਧਾਰਾ_ਬਫਰ );

            xReturn = pdPASS;
        }
    }
    taskEXIT_CRITICAL( &( px_ਧਾਰਾ_ਬਫਰ->x_ਧਾਰਾ_ਬਫਰ_ਤਾਲਾ ) );

    ਵਾਪਸ xReturn;
}
/*-----------------------------------------------------------*/

ਅਧਾਰ_Type_t x_ਧਾਰਾ_ਬਫਰ_ਸੈੱਟ_Trigger_Level( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ,
                                         size_t xTriggerLevel )
{
    ਧਾਰਾ_ਬਫਰ_t * ਅਚਲ px_ਧਾਰਾ_ਬਫਰ = x_ਧਾਰਾ_ਬਫਰ;
    ਅਧਾਰ_Type_t xReturn;

    configASSERT( px_ਧਾਰਾ_ਬਫਰ );

    /* It is not valid for the trigger level to be 0. */
    ਜੇ( xTriggerLevel == ( size_t ) 0 )
    {
        xTriggerLevel = ( size_t ) 1;
    }

    /* The trigger level is the number of bytes that must be in the stream
     * buffer before a task that is waiting for data is unblocked. */
    ਜੇ( xTriggerLevel < px_ਧਾਰਾ_ਬਫਰ->x_ਲੰਮਾਈ )
    {
        px_ਧਾਰਾ_ਬਫਰ->x_Trigger_Level_ਬਾਈਟਾਂ = xTriggerLevel;
        xReturn = pdPASS;
    }
    ਨਹੀਂਤਾਂ
    {
        xReturn = pdFALSE;
    }

    ਵਾਪਸ xReturn;
}
/*-----------------------------------------------------------*/

size_t x_ਧਾਰਾ_ਬਫਰ_Spaces_ਉਪਲਬਧ( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ )
{
    ਅਚਲ ਧਾਰਾ_ਬਫਰ_t * ਅਚਲ px_ਧਾਰਾ_ਬਫਰ = x_ਧਾਰਾ_ਬਫਰ;
    size_t xSpace;
    size_t x_Original_ਪੂਛ;

    configASSERT( px_ਧਾਰਾ_ਬਫਰ );

    /* The code below reads xTail and then xHead.  This is safe if the stream
     * buffer is updated once between the two reads - but not if the stream buffer
     * is updated more than once between the two reads - hence the loop. */
    ਕਰੋ
    {
        x_Original_ਪੂਛ = px_ਧਾਰਾ_ਬਫਰ->x_ਪੂਛ;
        xSpace = px_ਧਾਰਾ_ਬਫਰ->x_ਲੰਮਾਈ + px_ਧਾਰਾ_ਬਫਰ->x_ਪੂਛ;
        xSpace -= px_ਧਾਰਾ_ਬਫਰ->x_ਸਿਰ;
    } ਜਦੋਂ( x_Original_ਪੂਛ != px_ਧਾਰਾ_ਬਫਰ->x_ਪੂਛ );

    xSpace -= ( size_t ) 1;

    ਜੇ( xSpace >= px_ਧਾਰਾ_ਬਫਰ->x_ਲੰਮਾਈ )
    {
        xSpace -= px_ਧਾਰਾ_ਬਫਰ->x_ਲੰਮਾਈ;
    }
    ਨਹੀਂਤਾਂ
    {
        mtCOVERAGE_TEST_MARKER();
    }

    ਵਾਪਸ xSpace;
}
/*-----------------------------------------------------------*/

size_t x_ਧਾਰਾ_ਬਫਰ_ਬਾਈਟਾਂ_ਉਪਲਬਧ( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ )
{
    ਅਚਲ ਧਾਰਾ_ਬਫਰ_t * ਅਚਲ px_ਧਾਰਾ_ਬਫਰ = x_ਧਾਰਾ_ਬਫਰ;
    size_t xReturn;

    configASSERT( px_ਧਾਰਾ_ਬਫਰ );

    xReturn = prv_ਬਾਈਟਾਂ_ਵਿੱਚ_ਬਫਰ( px_ਧਾਰਾ_ਬਫਰ );
    ਵਾਪਸ xReturn;
}
/*-----------------------------------------------------------*/

size_t x_ਧਾਰਾ_ਬਫਰ_Send( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ,
                          ਅਚਲ ਖਾਲੀ * pv_Tx_ਅੰਕੜੇ,
                          size_t x_ਅੰਕੜੇ_ਲੰਮਾਈ_ਬਾਈਟਾਂ,
                          TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait )
{
    ਧਾਰਾ_ਬਫਰ_t * ਅਚਲ px_ਧਾਰਾ_ਬਫਰ = x_ਧਾਰਾ_ਬਫਰ;
    size_t xReturn, xSpace = 0;
    size_t xRequiredSpace = x_ਅੰਕੜੇ_ਲੰਮਾਈ_ਬਾਈਟਾਂ;
    ਸਮਾਂ_ਬਾਹਰਲਾ_t x_ਸਮਾਂ_ਬਾਹਰਲਾ_2;
    size_t xMaxReportedSpace = 0;

    configASSERT( pv_Tx_ਅੰਕੜੇ );
    configASSERT( px_ਧਾਰਾ_ਬਫਰ );

    /* The maximum amount of space a stream buffer will ever report is its length
     * minus 1. */
    xMaxReportedSpace = px_ਧਾਰਾ_ਬਫਰ->x_ਲੰਮਾਈ - ( size_t ) 1;

    /* This send function is used to write to both message buffers and stream
     * buffers.  If this is a message buffer then the space needed must be
     * increased by the amount of bytes needed to store the length of the
     * message. */
    ਜੇ( ( px_ਧਾਰਾ_ਬਫਰ->ucFlags & sb_FLAGS_IS_ਸੁਨੇਹਾ_ਬਫਰ ) != ( uint8_t ) 0 )
    {
        xRequiredSpace += sb_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_STORE_ਸੁਨੇਹਾ_ਲੰਮਾਈ;

        /* Overflow? */
        configASSERT( xRequiredSpace > x_ਅੰਕੜੇ_ਲੰਮਾਈ_ਬਾਈਟਾਂ );

        /* If this is a message buffer then it must be possible to write the
         * whole message. */
        ਜੇ( xRequiredSpace > xMaxReportedSpace )
        {
            /* The message would not fit even if the entire buffer was empty,
             * so don't wait for space. */
            x_Ticks_ਮੰਜ਼ਿਲ_Wait = ( TickType_t ) 0;
        }
        ਨਹੀਂਤਾਂ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    ਨਹੀਂਤਾਂ
    {
        /* If this is a stream buffer then it is acceptable to write only part
         * of the message to the buffer.  Cap the length to the total length of
         * the buffer. */
        ਜੇ( xRequiredSpace > xMaxReportedSpace )
        {
            xRequiredSpace = xMaxReportedSpace;
        }
        ਨਹੀਂਤਾਂ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }

    ਜੇ( x_Ticks_ਮੰਜ਼ਿਲ_Wait != ( TickType_t ) 0 )
    {
        v_Task_ਸੈੱਟ_ਸਮਾਂ_ਬਾਹਰਲਾ_State( &x_ਸਮਾਂ_ਬਾਹਰਲਾ_2 );

        ਕਰੋ
        {
            /* Wait until the required number of bytes are free in the message
             * buffer. */
            taskENTER_CRITICAL( &( px_ਧਾਰਾ_ਬਫਰ->x_ਧਾਰਾ_ਬਫਰ_ਤਾਲਾ ) );
            {
                xSpace = x_ਧਾਰਾ_ਬਫਰ_Spaces_ਉਪਲਬਧ( px_ਧਾਰਾ_ਬਫਰ );

                ਜੇ( xSpace < xRequiredSpace )
                {
                    /* Clear notification state as going to wait for space. */
                    ( ਖਾਲੀ ) xTaskNotifyStateClear( ਨਿਰਾ );

                    /* Should only be one writer. */
                    configASSERT( px_ਧਾਰਾ_ਬਫਰ->x_Task_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send == ਨਿਰਾ );
                    px_ਧਾਰਾ_ਬਫਰ->x_Task_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send = xTaskGetCurrentTaskHandle();
                }
                ਨਹੀਂਤਾਂ
                {
                    taskEXIT_CRITICAL( &( px_ਧਾਰਾ_ਬਫਰ->x_ਧਾਰਾ_ਬਫਰ_ਤਾਲਾ ) );
                    ਰੋਕੋ;
                }
            }
            taskEXIT_CRITICAL( &( px_ਧਾਰਾ_ਬਫਰ->x_ਧਾਰਾ_ਬਫਰ_ਤਾਲਾ ) );

            trace_BLOCKING_ON_ਧਾਰਾ_ਬਫਰ_SEND( x_ਧਾਰਾ_ਬਫਰ );
            ( ਖਾਲੀ ) xTaskNotifyWait( ( uint32_t ) 0, ( uint32_t ) 0, ਨਿਰਾ, x_Ticks_ਮੰਜ਼ਿਲ_Wait );
            /* In SMP mode, the task may have been woken and scheduled on
             * another core. Hence, we must clear the xTaskWaitingToSend
             * handle in a critical section. */
            #ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 )
                taskENTER_CRITICAL( &( px_ਧਾਰਾ_ਬਫਰ->x_ਧਾਰਾ_ਬਫਰ_ਤਾਲਾ ) );
            #ਜੇ_ਅੰਤ /* configNUMBER_OF_CORES > 1 */
            {
                px_ਧਾਰਾ_ਬਫਰ->x_Task_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send = ਨਿਰਾ;
            }
            #ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 )
                taskEXIT_CRITICAL( &( px_ਧਾਰਾ_ਬਫਰ->x_ਧਾਰਾ_ਬਫਰ_ਤਾਲਾ ) );
            #ਜੇ_ਅੰਤ /* configNUMBER_OF_CORES > 1 */
        } ਜਦੋਂ( x_Task_Check_For_ਸਮਾਂ_ਬਾਹਰਲਾ( &x_ਸਮਾਂ_ਬਾਹਰਲਾ_2, &x_Ticks_ਮੰਜ਼ਿਲ_Wait ) == pdFALSE );
    }
    ਨਹੀਂਤਾਂ
    {
        mtCOVERAGE_TEST_MARKER();
    }

    ਜੇ( xSpace == ( size_t ) 0 )
    {
        xSpace = x_ਧਾਰਾ_ਬਫਰ_Spaces_ਉਪਲਬਧ( px_ਧਾਰਾ_ਬਫਰ );
    }
    ਨਹੀਂਤਾਂ
    {
        mtCOVERAGE_TEST_MARKER();
    }

    xReturn = prv_Write_ਸੁਨੇਹਾ_ਮੰਜ਼ਿਲ_ਬਫਰ( px_ਧਾਰਾ_ਬਫਰ, pv_Tx_ਅੰਕੜੇ, x_ਅੰਕੜੇ_ਲੰਮਾਈ_ਬਾਈਟਾਂ, xSpace, xRequiredSpace );

    ਜੇ( xReturn > ( size_t ) 0 )
    {
        trace_ਧਾਰਾ_ਬਫਰ_SEND( x_ਧਾਰਾ_ਬਫਰ, xReturn );

        /* Was a task waiting for the data? */
        ਜੇ( prv_ਬਾਈਟਾਂ_ਵਿੱਚ_ਬਫਰ( px_ਧਾਰਾ_ਬਫਰ ) >= px_ਧਾਰਾ_ਬਫਰ->x_Trigger_Level_ਬਾਈਟਾਂ )
        {
            prvSEND_COMPLETED( px_ਧਾਰਾ_ਬਫਰ );
        }
        ਨਹੀਂਤਾਂ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    ਨਹੀਂਤਾਂ
    {
        mtCOVERAGE_TEST_MARKER();
        trace_ਧਾਰਾ_ਬਫਰ_SEND_FAILED( x_ਧਾਰਾ_ਬਫਰ );
    }

    ਵਾਪਸ xReturn;
}
/*-----------------------------------------------------------*/

size_t x_ਧਾਰਾ_ਬਫਰ_Send_From_ISR( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ,
                                 ਅਚਲ ਖਾਲੀ * pv_Tx_ਅੰਕੜੇ,
                                 size_t x_ਅੰਕੜੇ_ਲੰਮਾਈ_ਬਾਈਟਾਂ,
                                 ਅਧਾਰ_Type_t * ਅਚਲ px_Higher_ਤਰਜੀਹ_Task_Woken )
{
    ਧਾਰਾ_ਬਫਰ_t * ਅਚਲ px_ਧਾਰਾ_ਬਫਰ = x_ਧਾਰਾ_ਬਫਰ;
    size_t xReturn, xSpace;
    size_t xRequiredSpace = x_ਅੰਕੜੇ_ਲੰਮਾਈ_ਬਾਈਟਾਂ;

    configASSERT( pv_Tx_ਅੰਕੜੇ );
    configASSERT( px_ਧਾਰਾ_ਬਫਰ );

    /* This send function is used to write to both message buffers and stream
     * buffers.  If this is a message buffer then the space needed must be
     * increased by the amount of bytes needed to store the length of the
     * message. */
    ਜੇ( ( px_ਧਾਰਾ_ਬਫਰ->ucFlags & sb_FLAGS_IS_ਸੁਨੇਹਾ_ਬਫਰ ) != ( uint8_t ) 0 )
    {
        xRequiredSpace += sb_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_STORE_ਸੁਨੇਹਾ_ਲੰਮਾਈ;
    }
    ਨਹੀਂਤਾਂ
    {
        mtCOVERAGE_TEST_MARKER();
    }

    xSpace = x_ਧਾਰਾ_ਬਫਰ_Spaces_ਉਪਲਬਧ( px_ਧਾਰਾ_ਬਫਰ );
    xReturn = prv_Write_ਸੁਨੇਹਾ_ਮੰਜ਼ਿਲ_ਬਫਰ( px_ਧਾਰਾ_ਬਫਰ, pv_Tx_ਅੰਕੜੇ, x_ਅੰਕੜੇ_ਲੰਮਾਈ_ਬਾਈਟਾਂ, xSpace, xRequiredSpace );

    ਜੇ( xReturn > ( size_t ) 0 )
    {
        /* Was a task waiting for the data? */
        ਜੇ( prv_ਬਾਈਟਾਂ_ਵਿੱਚ_ਬਫਰ( px_ਧਾਰਾ_ਬਫਰ ) >= px_ਧਾਰਾ_ਬਫਰ->x_Trigger_Level_ਬਾਈਟਾਂ )
        {
            prvSEND_COMPLETE_FROM_ISR( px_ਧਾਰਾ_ਬਫਰ, px_Higher_ਤਰਜੀਹ_Task_Woken );
        }
        ਨਹੀਂਤਾਂ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    ਨਹੀਂਤਾਂ
    {
        mtCOVERAGE_TEST_MARKER();
    }

    trace_ਧਾਰਾ_ਬਫਰ_SEND_FROM_ISR( x_ਧਾਰਾ_ਬਫਰ, xReturn );

    ਵਾਪਸ xReturn;
}
/*-----------------------------------------------------------*/

ਸਥਿਰ size_t prv_Write_ਸੁਨੇਹਾ_ਮੰਜ਼ਿਲ_ਬਫਰ( ਧਾਰਾ_ਬਫਰ_t * ਅਚਲ px_ਧਾਰਾ_ਬਫਰ,
                                       ਅਚਲ ਖਾਲੀ * pv_Tx_ਅੰਕੜੇ,
                                       size_t x_ਅੰਕੜੇ_ਲੰਮਾਈ_ਬਾਈਟਾਂ,
                                       size_t xSpace,
                                       size_t xRequiredSpace )
{
    size_t x_ਅਗਲਾ_ਸਿਰ = px_ਧਾਰਾ_ਬਫਰ->x_ਸਿਰ;
    config_ਸੁਨੇਹਾ_ਬਫਰ_ਲੰਮਾਈ_TYPE x_ਸੁਨੇਹਾ_ਲੰਮਾਈ;

    ਜੇ( ( px_ਧਾਰਾ_ਬਫਰ->ucFlags & sb_FLAGS_IS_ਸੁਨੇਹਾ_ਬਫਰ ) != ( uint8_t ) 0 )
    {
        /* This is a message buffer, as opposed to a stream buffer. */

        /* Convert xDataLengthBytes to the message length type. */
        x_ਸੁਨੇਹਾ_ਲੰਮਾਈ = ( config_ਸੁਨੇਹਾ_ਬਫਰ_ਲੰਮਾਈ_TYPE ) x_ਅੰਕੜੇ_ਲੰਮਾਈ_ਬਾਈਟਾਂ;

        /* Ensure the data length given fits within configMESSAGE_BUFFER_LENGTH_TYPE. */
        configASSERT( ( size_t ) x_ਸੁਨੇਹਾ_ਲੰਮਾਈ == x_ਅੰਕੜੇ_ਲੰਮਾਈ_ਬਾਈਟਾਂ );

        ਜੇ( xSpace >= xRequiredSpace )
        {
            /* There is enough space to write both the message length and the message
             * itself into the buffer.  Start by writing the length of the data, the data
             * itself will be written later in this function. */
            x_ਅਗਲਾ_ਸਿਰ = prv_Write_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_ਬਫਰ( px_ਧਾਰਾ_ਬਫਰ, ( ਅਚਲ uint8_t * ) &( x_ਸੁਨੇਹਾ_ਲੰਮਾਈ ), sb_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_STORE_ਸੁਨੇਹਾ_ਲੰਮਾਈ, x_ਅਗਲਾ_ਸਿਰ );
        }
        ਨਹੀਂਤਾਂ
        {
            /* Not enough space, so do not write data to the buffer. */
            x_ਅੰਕੜੇ_ਲੰਮਾਈ_ਬਾਈਟਾਂ = 0;
        }
    }
    ਨਹੀਂਤਾਂ
    {
        /* This is a stream buffer, as opposed to a message buffer, so writing a
         * stream of bytes rather than discrete messages.  Plan to write as many
         * bytes as possible. */
        x_ਅੰਕੜੇ_ਲੰਮਾਈ_ਬਾਈਟਾਂ = configMIN( x_ਅੰਕੜੇ_ਲੰਮਾਈ_ਬਾਈਟਾਂ, xSpace );
    }

    ਜੇ( x_ਅੰਕੜੇ_ਲੰਮਾਈ_ਬਾਈਟਾਂ != ( size_t ) 0 )
    {
        /* Write the data to the buffer. */
        px_ਧਾਰਾ_ਬਫਰ->x_ਸਿਰ = prv_Write_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_ਬਫਰ( px_ਧਾਰਾ_ਬਫਰ, ( ਅਚਲ uint8_t * ) pv_Tx_ਅੰਕੜੇ, x_ਅੰਕੜੇ_ਲੰਮਾਈ_ਬਾਈਟਾਂ, x_ਅਗਲਾ_ਸਿਰ ); /*lint !e9079 Storage buffer is implemented as uint8_t for ease of sizing, alignment and access. */
    }

    ਵਾਪਸ x_ਅੰਕੜੇ_ਲੰਮਾਈ_ਬਾਈਟਾਂ;
}
/*-----------------------------------------------------------*/

size_t x_ਧਾਰਾ_ਬਫਰ_Receive( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ,
                             ਖਾਲੀ * pv_Rx_ਅੰਕੜੇ,
                             size_t x_ਬਫਰ_ਲੰਮਾਈ_ਬਾਈਟਾਂ,
                             TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait )
{
    ਧਾਰਾ_ਬਫਰ_t * ਅਚਲ px_ਧਾਰਾ_ਬਫਰ = x_ਧਾਰਾ_ਬਫਰ;
    size_t x_Received_ਲੰਮਾਈ = 0, x_ਬਾਈਟਾਂ_ਉਪਲਬਧ, x_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_Store_ਸੁਨੇਹਾ_ਲੰਮਾਈ;

    configASSERT( pv_Rx_ਅੰਕੜੇ );
    configASSERT( px_ਧਾਰਾ_ਬਫਰ );

    /* This receive function is used by both message buffers, which store
     * discrete messages, and stream buffers, which store a continuous stream of
     * bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the
     * message. */
    ਜੇ( ( px_ਧਾਰਾ_ਬਫਰ->ucFlags & sb_FLAGS_IS_ਸੁਨੇਹਾ_ਬਫਰ ) != ( uint8_t ) 0 )
    {
        x_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_Store_ਸੁਨੇਹਾ_ਲੰਮਾਈ = sb_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_STORE_ਸੁਨੇਹਾ_ਲੰਮਾਈ;
    }
    ਨਹੀਂਤਾਂ
    {
        x_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_Store_ਸੁਨੇਹਾ_ਲੰਮਾਈ = 0;
    }

    ਜੇ( x_Ticks_ਮੰਜ਼ਿਲ_Wait != ( TickType_t ) 0 )
    {
        /* Checking if there is data and clearing the notification state must be
         * performed atomically. */
        taskENTER_CRITICAL( &( px_ਧਾਰਾ_ਬਫਰ->x_ਧਾਰਾ_ਬਫਰ_ਤਾਲਾ ) );
        {
            x_ਬਾਈਟਾਂ_ਉਪਲਬਧ = prv_ਬਾਈਟਾਂ_ਵਿੱਚ_ਬਫਰ( px_ਧਾਰਾ_ਬਫਰ );

            /* If this function was invoked by a message buffer read then
             * xBytesToStoreMessageLength holds the number of bytes used to hold
             * the length of the next discrete message.  If this function was
             * invoked by a stream buffer read then xBytesToStoreMessageLength will
             * be 0. */
            ਜੇ( x_ਬਾਈਟਾਂ_ਉਪਲਬਧ <= x_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_Store_ਸੁਨੇਹਾ_ਲੰਮਾਈ )
            {
                /* Clear notification state as going to wait for data. */
                ( ਖਾਲੀ ) xTaskNotifyStateClear( ਨਿਰਾ );

                /* Should only be one reader. */
                configASSERT( px_ਧਾਰਾ_ਬਫਰ->x_Task_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive == ਨਿਰਾ );
                px_ਧਾਰਾ_ਬਫਰ->x_Task_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive = xTaskGetCurrentTaskHandle();
            }
            ਨਹੀਂਤਾਂ
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        taskEXIT_CRITICAL( &( px_ਧਾਰਾ_ਬਫਰ->x_ਧਾਰਾ_ਬਫਰ_ਤਾਲਾ ) );

        ਜੇ( x_ਬਾਈਟਾਂ_ਉਪਲਬਧ <= x_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_Store_ਸੁਨੇਹਾ_ਲੰਮਾਈ )
        {
            /* Wait for data to be available. */
            trace_BLOCKING_ON_ਧਾਰਾ_ਬਫਰ_RECEIVE( x_ਧਾਰਾ_ਬਫਰ );
            ( ਖਾਲੀ ) xTaskNotifyWait( ( uint32_t ) 0, ( uint32_t ) 0, ਨਿਰਾ, x_Ticks_ਮੰਜ਼ਿਲ_Wait );

            /* In SMP mode, the task may have been woken and scheduled on
             * another core. Hence, we must clear the xTaskWaitingToReceive
             * handle in a critical section. */
            #ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 )
                taskENTER_CRITICAL( &( px_ਧਾਰਾ_ਬਫਰ->x_ਧਾਰਾ_ਬਫਰ_ਤਾਲਾ ) );
            #ਜੇ_ਅੰਤ /* configNUMBER_OF_CORES > 1 */
            {
                px_ਧਾਰਾ_ਬਫਰ->x_Task_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive = ਨਿਰਾ;
            }
            #ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 )
                taskEXIT_CRITICAL( &( px_ਧਾਰਾ_ਬਫਰ->x_ਧਾਰਾ_ਬਫਰ_ਤਾਲਾ ) );
            #ਜੇ_ਅੰਤ /* configNUMBER_OF_CORES > 1 */

            /* Recheck the data available after blocking. */
            x_ਬਾਈਟਾਂ_ਉਪਲਬਧ = prv_ਬਾਈਟਾਂ_ਵਿੱਚ_ਬਫਰ( px_ਧਾਰਾ_ਬਫਰ );
        }
        ਨਹੀਂਤਾਂ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    ਨਹੀਂਤਾਂ
    {
        x_ਬਾਈਟਾਂ_ਉਪਲਬਧ = prv_ਬਾਈਟਾਂ_ਵਿੱਚ_ਬਫਰ( px_ਧਾਰਾ_ਬਫਰ );
    }

    /* Whether receiving a discrete message (where xBytesToStoreMessageLength
     * holds the number of bytes used to store the message length) or a stream of
     * bytes (where xBytesToStoreMessageLength is zero), the number of bytes
     * available must be greater than xBytesToStoreMessageLength to be able to
     * read bytes from the buffer. */
    ਜੇ( x_ਬਾਈਟਾਂ_ਉਪਲਬਧ > x_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_Store_ਸੁਨੇਹਾ_ਲੰਮਾਈ )
    {
        x_Received_ਲੰਮਾਈ = prv_Read_ਸੁਨੇਹਾ_From_ਬਫਰ( px_ਧਾਰਾ_ਬਫਰ, pv_Rx_ਅੰਕੜੇ, x_ਬਫਰ_ਲੰਮਾਈ_ਬਾਈਟਾਂ, x_ਬਾਈਟਾਂ_ਉਪਲਬਧ );

        /* Was a task waiting for space in the buffer? */
        ਜੇ( x_Received_ਲੰਮਾਈ != ( size_t ) 0 )
        {
            trace_ਧਾਰਾ_ਬਫਰ_RECEIVE( x_ਧਾਰਾ_ਬਫਰ, x_Received_ਲੰਮਾਈ );
            prvRECEIVE_COMPLETED( x_ਧਾਰਾ_ਬਫਰ );
        }
        ਨਹੀਂਤਾਂ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    ਨਹੀਂਤਾਂ
    {
        trace_ਧਾਰਾ_ਬਫਰ_RECEIVE_FAILED( x_ਧਾਰਾ_ਬਫਰ );
        mtCOVERAGE_TEST_MARKER();
    }

    ਵਾਪਸ x_Received_ਲੰਮਾਈ;
}
/*-----------------------------------------------------------*/

size_t x_ਧਾਰਾ_ਬਫਰ_ਅਗਲਾ_ਸੁਨੇਹਾ_ਲੰਮਾਈ_ਬਾਈਟਾਂ( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ )
{
    ਧਾਰਾ_ਬਫਰ_t * ਅਚਲ px_ਧਾਰਾ_ਬਫਰ = x_ਧਾਰਾ_ਬਫਰ;
    size_t xReturn, x_ਬਾਈਟਾਂ_ਉਪਲਬਧ;
    config_ਸੁਨੇਹਾ_ਬਫਰ_ਲੰਮਾਈ_TYPE x_ਆਰਜ਼ੀ_Return;

    configASSERT( px_ਧਾਰਾ_ਬਫਰ );

    /* Ensure the stream buffer is being used as a message buffer. */
    ਜੇ( ( px_ਧਾਰਾ_ਬਫਰ->ucFlags & sb_FLAGS_IS_ਸੁਨੇਹਾ_ਬਫਰ ) != ( uint8_t ) 0 )
    {
        x_ਬਾਈਟਾਂ_ਉਪਲਬਧ = prv_ਬਾਈਟਾਂ_ਵਿੱਚ_ਬਫਰ( px_ਧਾਰਾ_ਬਫਰ );

        ਜੇ( x_ਬਾਈਟਾਂ_ਉਪਲਬਧ > sb_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_STORE_ਸੁਨੇਹਾ_ਲੰਮਾਈ )
        {
            /* The number of bytes available is greater than the number of bytes
             * required to hold the length of the next message, so another message
             * is available. */
            ( ਖਾਲੀ ) prv_Read_ਬਾਈਟਾਂ_From_ਬਫਰ( px_ਧਾਰਾ_ਬਫਰ, ( uint8_t * ) &x_ਆਰਜ਼ੀ_Return, sb_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_STORE_ਸੁਨੇਹਾ_ਲੰਮਾਈ, px_ਧਾਰਾ_ਬਫਰ->x_ਪੂਛ );
            xReturn = ( size_t ) x_ਆਰਜ਼ੀ_Return;
        }
        ਨਹੀਂਤਾਂ
        {
            /* The minimum amount of bytes in a message buffer is
             * ( sbBYTES_TO_STORE_MESSAGE_LENGTH + 1 ), so if xBytesAvailable is
             * less than sbBYTES_TO_STORE_MESSAGE_LENGTH the only other valid
             * value is 0. */
            configASSERT( x_ਬਾਈਟਾਂ_ਉਪਲਬਧ == 0 );
            xReturn = 0;
        }
    }
    ਨਹੀਂਤਾਂ
    {
        xReturn = 0;
    }

    ਵਾਪਸ xReturn;
}
/*-----------------------------------------------------------*/

size_t x_ਧਾਰਾ_ਬਫਰ_Receive_From_ISR( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ,
                                    ਖਾਲੀ * pv_Rx_ਅੰਕੜੇ,
                                    size_t x_ਬਫਰ_ਲੰਮਾਈ_ਬਾਈਟਾਂ,
                                    ਅਧਾਰ_Type_t * ਅਚਲ px_Higher_ਤਰਜੀਹ_Task_Woken )
{
    ਧਾਰਾ_ਬਫਰ_t * ਅਚਲ px_ਧਾਰਾ_ਬਫਰ = x_ਧਾਰਾ_ਬਫਰ;
    size_t x_Received_ਲੰਮਾਈ = 0, x_ਬਾਈਟਾਂ_ਉਪਲਬਧ, x_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_Store_ਸੁਨੇਹਾ_ਲੰਮਾਈ;

    configASSERT( pv_Rx_ਅੰਕੜੇ );
    configASSERT( px_ਧਾਰਾ_ਬਫਰ );

    /* This receive function is used by both message buffers, which store
     * discrete messages, and stream buffers, which store a continuous stream of
     * bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the
     * message. */
    ਜੇ( ( px_ਧਾਰਾ_ਬਫਰ->ucFlags & sb_FLAGS_IS_ਸੁਨੇਹਾ_ਬਫਰ ) != ( uint8_t ) 0 )
    {
        x_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_Store_ਸੁਨੇਹਾ_ਲੰਮਾਈ = sb_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_STORE_ਸੁਨੇਹਾ_ਲੰਮਾਈ;
    }
    ਨਹੀਂਤਾਂ
    {
        x_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_Store_ਸੁਨੇਹਾ_ਲੰਮਾਈ = 0;
    }

    x_ਬਾਈਟਾਂ_ਉਪਲਬਧ = prv_ਬਾਈਟਾਂ_ਵਿੱਚ_ਬਫਰ( px_ਧਾਰਾ_ਬਫਰ );

    /* Whether receiving a discrete message (where xBytesToStoreMessageLength
     * holds the number of bytes used to store the message length) or a stream of
     * bytes (where xBytesToStoreMessageLength is zero), the number of bytes
     * available must be greater than xBytesToStoreMessageLength to be able to
     * read bytes from the buffer. */
    ਜੇ( x_ਬਾਈਟਾਂ_ਉਪਲਬਧ > x_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_Store_ਸੁਨੇਹਾ_ਲੰਮਾਈ )
    {
        x_Received_ਲੰਮਾਈ = prv_Read_ਸੁਨੇਹਾ_From_ਬਫਰ( px_ਧਾਰਾ_ਬਫਰ, pv_Rx_ਅੰਕੜੇ, x_ਬਫਰ_ਲੰਮਾਈ_ਬਾਈਟਾਂ, x_ਬਾਈਟਾਂ_ਉਪਲਬਧ );

        /* Was a task waiting for space in the buffer? */
        ਜੇ( x_Received_ਲੰਮਾਈ != ( size_t ) 0 )
        {
            prvRECEIVE_COMPLETED_FROM_ISR( px_ਧਾਰਾ_ਬਫਰ, px_Higher_ਤਰਜੀਹ_Task_Woken );
        }
        ਨਹੀਂਤਾਂ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    ਨਹੀਂਤਾਂ
    {
        mtCOVERAGE_TEST_MARKER();
    }

    trace_ਧਾਰਾ_ਬਫਰ_RECEIVE_FROM_ISR( x_ਧਾਰਾ_ਬਫਰ, x_Received_ਲੰਮਾਈ );

    ਵਾਪਸ x_Received_ਲੰਮਾਈ;
}
/*-----------------------------------------------------------*/

ਸਥਿਰ size_t prv_Read_ਸੁਨੇਹਾ_From_ਬਫਰ( ਧਾਰਾ_ਬਫਰ_t * px_ਧਾਰਾ_ਬਫਰ,
                                        ਖਾਲੀ * pv_Rx_ਅੰਕੜੇ,
                                        size_t x_ਬਫਰ_ਲੰਮਾਈ_ਬਾਈਟਾਂ,
                                        size_t x_ਬਾਈਟਾਂ_ਉਪਲਬਧ )
{
    size_t x_ਗਿਣਤੀ, x_ਅਗਲਾ_ਸੁਨੇਹਾ_ਲੰਮਾਈ;
    config_ਸੁਨੇਹਾ_ਬਫਰ_ਲੰਮਾਈ_TYPE x_ਆਰਜ਼ੀ_ਅਗਲਾ_ਸੁਨੇਹਾ_ਲੰਮਾਈ;
    size_t x_ਅਗਲਾ_ਪੂਛ = px_ਧਾਰਾ_ਬਫਰ->x_ਪੂਛ;

    ਜੇ( ( px_ਧਾਰਾ_ਬਫਰ->ucFlags & sb_FLAGS_IS_ਸੁਨੇਹਾ_ਬਫਰ ) != ( uint8_t ) 0 )
    {
        /* A discrete message is being received.  First receive the length
         * of the message. */
        x_ਅਗਲਾ_ਪੂਛ = prv_Read_ਬਾਈਟਾਂ_From_ਬਫਰ( px_ਧਾਰਾ_ਬਫਰ, ( uint8_t * ) &x_ਆਰਜ਼ੀ_ਅਗਲਾ_ਸੁਨੇਹਾ_ਲੰਮਾਈ, sb_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_STORE_ਸੁਨੇਹਾ_ਲੰਮਾਈ, x_ਅਗਲਾ_ਪੂਛ );
        x_ਅਗਲਾ_ਸੁਨੇਹਾ_ਲੰਮਾਈ = ( size_t ) x_ਆਰਜ਼ੀ_ਅਗਲਾ_ਸੁਨੇਹਾ_ਲੰਮਾਈ;

        /* Reduce the number of bytes available by the number of bytes just
         * read out. */
        x_ਬਾਈਟਾਂ_ਉਪਲਬਧ -= sb_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_STORE_ਸੁਨੇਹਾ_ਲੰਮਾਈ;

        /* Check there is enough space in the buffer provided by the
         * user. */
        ਜੇ( x_ਅਗਲਾ_ਸੁਨੇਹਾ_ਲੰਮਾਈ > x_ਬਫਰ_ਲੰਮਾਈ_ਬਾਈਟਾਂ )
        {
            /* The user has provided insufficient space to read the message. */
            x_ਅਗਲਾ_ਸੁਨੇਹਾ_ਲੰਮਾਈ = 0;
        }
        ਨਹੀਂਤਾਂ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    ਨਹੀਂਤਾਂ
    {
        /* A stream of bytes is being received (as opposed to a discrete
         * message), so read as many bytes as possible. */
        x_ਅਗਲਾ_ਸੁਨੇਹਾ_ਲੰਮਾਈ = x_ਬਫਰ_ਲੰਮਾਈ_ਬਾਈਟਾਂ;
    }

    /* Use the minimum of the wanted bytes and the available bytes. */
    x_ਗਿਣਤੀ = configMIN( x_ਅਗਲਾ_ਸੁਨੇਹਾ_ਲੰਮਾਈ, x_ਬਾਈਟਾਂ_ਉਪਲਬਧ );

    ਜੇ( x_ਗਿਣਤੀ != ( size_t ) 0 )
    {
        /* Read the actual data and update the tail to mark the data as officially consumed. */
        px_ਧਾਰਾ_ਬਫਰ->x_ਪੂਛ = prv_Read_ਬਾਈਟਾਂ_From_ਬਫਰ( px_ਧਾਰਾ_ਬਫਰ, ( uint8_t * ) pv_Rx_ਅੰਕੜੇ, x_ਗਿਣਤੀ, x_ਅਗਲਾ_ਪੂਛ ); /*lint !e9079 Data storage area is implemented as uint8_t array for ease of sizing, indexing and alignment. */
    }

    ਵਾਪਸ x_ਗਿਣਤੀ;
}
/*-----------------------------------------------------------*/

ਅਧਾਰ_Type_t x_ਧਾਰਾ_ਬਫਰ_Is_ਖਾਲੀ_ਥਾਵਾਂ( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ )
{
    ਅਚਲ ਧਾਰਾ_ਬਫਰ_t * ਅਚਲ px_ਧਾਰਾ_ਬਫਰ = x_ਧਾਰਾ_ਬਫਰ;
    ਅਧਾਰ_Type_t xReturn;
    size_t x_ਪੂਛ;

    configASSERT( px_ਧਾਰਾ_ਬਫਰ );

    /* True if no bytes are available. */
    x_ਪੂਛ = px_ਧਾਰਾ_ਬਫਰ->x_ਪੂਛ;

    ਜੇ( px_ਧਾਰਾ_ਬਫਰ->x_ਸਿਰ == x_ਪੂਛ )
    {
        xReturn = pdTRUE;
    }
    ਨਹੀਂਤਾਂ
    {
        xReturn = pdFALSE;
    }

    ਵਾਪਸ xReturn;
}
/*-----------------------------------------------------------*/

ਅਧਾਰ_Type_t x_ਧਾਰਾ_ਬਫਰ_Is_ਭਰਿਆ( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ )
{
    ਅਧਾਰ_Type_t xReturn;
    size_t x_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_Store_ਸੁਨੇਹਾ_ਲੰਮਾਈ;
    ਅਚਲ ਧਾਰਾ_ਬਫਰ_t * ਅਚਲ px_ਧਾਰਾ_ਬਫਰ = x_ਧਾਰਾ_ਬਫਰ;

    configASSERT( px_ਧਾਰਾ_ਬਫਰ );

    /* This generic version of the receive function is used by both message
     * buffers, which store discrete messages, and stream buffers, which store a
     * continuous stream of bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the message. */
    ਜੇ( ( px_ਧਾਰਾ_ਬਫਰ->ucFlags & sb_FLAGS_IS_ਸੁਨੇਹਾ_ਬਫਰ ) != ( uint8_t ) 0 )
    {
        x_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_Store_ਸੁਨੇਹਾ_ਲੰਮਾਈ = sb_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_STORE_ਸੁਨੇਹਾ_ਲੰਮਾਈ;
    }
    ਨਹੀਂਤਾਂ
    {
        x_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_Store_ਸੁਨੇਹਾ_ਲੰਮਾਈ = 0;
    }

    /* True if the available space equals zero. */
    ਜੇ( x_ਧਾਰਾ_ਬਫਰ_Spaces_ਉਪਲਬਧ( x_ਧਾਰਾ_ਬਫਰ ) <= x_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_Store_ਸੁਨੇਹਾ_ਲੰਮਾਈ )
    {
        xReturn = pdTRUE;
    }
    ਨਹੀਂਤਾਂ
    {
        xReturn = pdFALSE;
    }

    ਵਾਪਸ xReturn;
}
/*-----------------------------------------------------------*/

ਅਧਾਰ_Type_t x_ਧਾਰਾ_ਬਫਰ_Send_Completed_From_ISR( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ,
                                              ਅਧਾਰ_Type_t * px_Higher_ਤਰਜੀਹ_Task_Woken )
{
    ਧਾਰਾ_ਬਫਰ_t * ਅਚਲ px_ਧਾਰਾ_ਬਫਰ = x_ਧਾਰਾ_ਬਫਰ;
    ਅਧਾਰ_Type_t xReturn;
    UBaseType_t ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ;

    configASSERT( px_ਧਾਰਾ_ਬਫਰ );

    ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ = ( UBaseType_t ) ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR();
    {
        ਜੇ( ( px_ਧਾਰਾ_ਬਫਰ )->x_Task_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive != ਨਿਰਾ )
        {
            ( ਖਾਲੀ ) xTaskNotifyFromISR( ( px_ਧਾਰਾ_ਬਫਰ )->x_Task_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive,
                                         ( uint32_t ) 0,
                                         eNoAction,
                                         px_Higher_ਤਰਜੀਹ_Task_Woken );
            ( px_ਧਾਰਾ_ਬਫਰ )->x_Task_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive = ਨਿਰਾ;
            xReturn = pdTRUE;
        }
        ਨਹੀਂਤਾਂ
        {
            xReturn = pdFALSE;
        }
    }
    ਪੋਰਟ_CLEAR_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR( ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ );

    ਵਾਪਸ xReturn;
}
/*-----------------------------------------------------------*/

ਅਧਾਰ_Type_t x_ਧਾਰਾ_ਬਫਰ_Receive_Completed_From_ISR( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ,
                                                 ਅਧਾਰ_Type_t * px_Higher_ਤਰਜੀਹ_Task_Woken )
{
    ਧਾਰਾ_ਬਫਰ_t * ਅਚਲ px_ਧਾਰਾ_ਬਫਰ = x_ਧਾਰਾ_ਬਫਰ;
    ਅਧਾਰ_Type_t xReturn;
    UBaseType_t ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ;

    configASSERT( px_ਧਾਰਾ_ਬਫਰ );

    ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ = ( UBaseType_t ) ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR();
    {
        ਜੇ( ( px_ਧਾਰਾ_ਬਫਰ )->x_Task_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send != ਨਿਰਾ )
        {
            ( ਖਾਲੀ ) xTaskNotifyFromISR( ( px_ਧਾਰਾ_ਬਫਰ )->x_Task_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send,
                                         ( uint32_t ) 0,
                                         eNoAction,
                                         px_Higher_ਤਰਜੀਹ_Task_Woken );
            ( px_ਧਾਰਾ_ਬਫਰ )->x_Task_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send = ਨਿਰਾ;
            xReturn = pdTRUE;
        }
        ਨਹੀਂਤਾਂ
        {
            xReturn = pdFALSE;
        }
    }
    ਪੋਰਟ_CLEAR_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR( ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ );

    ਵਾਪਸ xReturn;
}
/*-----------------------------------------------------------*/

ਸਥਿਰ size_t prv_Write_ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_ਬਫਰ( ਧਾਰਾ_ਬਫਰ_t * ਅਚਲ px_ਧਾਰਾ_ਬਫਰ,
                                     ਅਚਲ uint8_t * puc_ਅੰਕੜੇ,
                                     size_t x_ਗਿਣਤੀ,
                                     size_t x_ਸਿਰ )
{
    size_t x_ਪਹਿਲਾ_ਲੰਮਾਈ;

    configASSERT( x_ਗਿਣਤੀ > ( size_t ) 0 );

    /* Calculate the number of bytes that can be added in the first write -
     * which may be less than the total number of bytes that need to be added if
     * the buffer will wrap back to the beginning. */
    x_ਪਹਿਲਾ_ਲੰਮਾਈ = configMIN( px_ਧਾਰਾ_ਬਫਰ->x_ਲੰਮਾਈ - x_ਸਿਰ, x_ਗਿਣਤੀ );

    /* Write as many bytes as can be written in the first write. */
    configASSERT( ( x_ਸਿਰ + x_ਪਹਿਲਾ_ਲੰਮਾਈ ) <= px_ਧਾਰਾ_ਬਫਰ->x_ਲੰਮਾਈ );
    ( ਖਾਲੀ ) ਸਮ੍ਰਿਤੀ_ਨਕਲ( ( ਖਾਲੀ * ) ( &( px_ਧਾਰਾ_ਬਫਰ->puc_ਬਫਰ[ x_ਸਿਰ ] ) ), ( ਅਚਲ ਖਾਲੀ * ) puc_ਅੰਕੜੇ, x_ਪਹਿਲਾ_ਲੰਮਾਈ ); /*lint !e9087 memcpy() requires void *. */

    /* If the number of bytes written was less than the number that could be
     * written in the first write... */
    ਜੇ( x_ਗਿਣਤੀ > x_ਪਹਿਲਾ_ਲੰਮਾਈ )
    {
        /* ...then write the remaining bytes to the start of the buffer. */
        configASSERT( ( x_ਗਿਣਤੀ - x_ਪਹਿਲਾ_ਲੰਮਾਈ ) <= px_ਧਾਰਾ_ਬਫਰ->x_ਲੰਮਾਈ );
        ( ਖਾਲੀ ) ਸਮ੍ਰਿਤੀ_ਨਕਲ( ( ਖਾਲੀ * ) px_ਧਾਰਾ_ਬਫਰ->puc_ਬਫਰ, ( ਅਚਲ ਖਾਲੀ * ) &( puc_ਅੰਕੜੇ[ x_ਪਹਿਲਾ_ਲੰਮਾਈ ] ), x_ਗਿਣਤੀ - x_ਪਹਿਲਾ_ਲੰਮਾਈ ); /*lint !e9087 memcpy() requires void *. */
    }
    ਨਹੀਂਤਾਂ
    {
        mtCOVERAGE_TEST_MARKER();
    }

    x_ਸਿਰ += x_ਗਿਣਤੀ;

    ਜੇ( x_ਸਿਰ >= px_ਧਾਰਾ_ਬਫਰ->x_ਲੰਮਾਈ )
    {
        x_ਸਿਰ -= px_ਧਾਰਾ_ਬਫਰ->x_ਲੰਮਾਈ;
    }
    ਨਹੀਂਤਾਂ
    {
        mtCOVERAGE_TEST_MARKER();
    }

    ਵਾਪਸ x_ਸਿਰ;
}
/*-----------------------------------------------------------*/

ਸਥਿਰ size_t prv_Read_ਬਾਈਟਾਂ_From_ਬਫਰ( ਧਾਰਾ_ਬਫਰ_t * px_ਧਾਰਾ_ਬਫਰ,
                                      uint8_t * puc_ਅੰਕੜੇ,
                                      size_t x_ਗਿਣਤੀ,
                                      size_t x_ਪੂਛ )
{
    size_t x_ਪਹਿਲਾ_ਲੰਮਾਈ;

    configASSERT( x_ਗਿਣਤੀ != ( size_t ) 0 );

    /* Calculate the number of bytes that can be read - which may be
     * less than the number wanted if the data wraps around to the start of
     * the buffer. */
    x_ਪਹਿਲਾ_ਲੰਮਾਈ = configMIN( px_ਧਾਰਾ_ਬਫਰ->x_ਲੰਮਾਈ - x_ਪੂਛ, x_ਗਿਣਤੀ );

    /* Obtain the number of bytes it is possible to obtain in the first
     * read.  Asserts check bounds of read and write. */
    configASSERT( x_ਪਹਿਲਾ_ਲੰਮਾਈ <= x_ਗਿਣਤੀ );
    configASSERT( ( x_ਪੂਛ + x_ਪਹਿਲਾ_ਲੰਮਾਈ ) <= px_ਧਾਰਾ_ਬਫਰ->x_ਲੰਮਾਈ );
    ( ਖਾਲੀ ) ਸਮ੍ਰਿਤੀ_ਨਕਲ( ( ਖਾਲੀ * ) puc_ਅੰਕੜੇ, ( ਅਚਲ ਖਾਲੀ * ) &( px_ਧਾਰਾ_ਬਫਰ->puc_ਬਫਰ[ x_ਪੂਛ ] ), x_ਪਹਿਲਾ_ਲੰਮਾਈ ); /*lint !e9087 memcpy() requires void *. */

    /* If the total number of wanted bytes is greater than the number
     * that could be read in the first read... */
    ਜੇ( x_ਗਿਣਤੀ > x_ਪਹਿਲਾ_ਲੰਮਾਈ )
    {
        /* ...then read the remaining bytes from the start of the buffer. */
        ( ਖਾਲੀ ) ਸਮ੍ਰਿਤੀ_ਨਕਲ( ( ਖਾਲੀ * ) &( puc_ਅੰਕੜੇ[ x_ਪਹਿਲਾ_ਲੰਮਾਈ ] ), ( ਖਾਲੀ * ) ( px_ਧਾਰਾ_ਬਫਰ->puc_ਬਫਰ ), x_ਗਿਣਤੀ - x_ਪਹਿਲਾ_ਲੰਮਾਈ ); /*lint !e9087 memcpy() requires void *. */
    }
    ਨਹੀਂਤਾਂ
    {
        mtCOVERAGE_TEST_MARKER();
    }

    /* Move the tail pointer to effectively remove the data read from the buffer. */
    x_ਪੂਛ += x_ਗਿਣਤੀ;

    ਜੇ( x_ਪੂਛ >= px_ਧਾਰਾ_ਬਫਰ->x_ਲੰਮਾਈ )
    {
        x_ਪੂਛ -= px_ਧਾਰਾ_ਬਫਰ->x_ਲੰਮਾਈ;
    }

    ਵਾਪਸ x_ਪੂਛ;
}
/*-----------------------------------------------------------*/

ਸਥਿਰ size_t prv_ਬਾਈਟਾਂ_ਵਿੱਚ_ਬਫਰ( ਅਚਲ ਧਾਰਾ_ਬਫਰ_t * ਅਚਲ px_ਧਾਰਾ_ਬਫਰ )
{
/* Returns the distance between xTail and xHead. */
    size_t x_ਗਿਣਤੀ;

    x_ਗਿਣਤੀ = px_ਧਾਰਾ_ਬਫਰ->x_ਲੰਮਾਈ + px_ਧਾਰਾ_ਬਫਰ->x_ਸਿਰ;
    x_ਗਿਣਤੀ -= px_ਧਾਰਾ_ਬਫਰ->x_ਪੂਛ;

    ਜੇ( x_ਗਿਣਤੀ >= px_ਧਾਰਾ_ਬਫਰ->x_ਲੰਮਾਈ )
    {
        x_ਗਿਣਤੀ -= px_ਧਾਰਾ_ਬਫਰ->x_ਲੰਮਾਈ;
    }
    ਨਹੀਂਤਾਂ
    {
        mtCOVERAGE_TEST_MARKER();
    }

    ਵਾਪਸ x_ਗਿਣਤੀ;
}
/*-----------------------------------------------------------*/

ਸਥਿਰ ਖਾਲੀ prv_Initialise_New_ਧਾਰਾ_ਬਫਰ( ਧਾਰਾ_ਬਫਰ_t * ਅਚਲ px_ਧਾਰਾ_ਬਫਰ,
                                          uint8_t * ਅਚਲ puc_ਬਫਰ,
                                          size_t x_ਬਫਰ_ਮਾਪ_ਬਾਈਟਾਂ,
                                          size_t x_Trigger_Level_ਬਾਈਟਾਂ,
                                          uint8_t ucFlags,
                                          ਧਾਰਾ_ਬਫਰ_Callback_Function_t pxSendCompletedCallback,
                                          ਧਾਰਾ_ਬਫਰ_Callback_Function_t pxReceiveCompletedCallback )
{
    /* Assert here is deliberately writing to the entire buffer to ensure it can
     * be written to without generating exceptions, and is setting the buffer to a
     * known value to assist in development/debugging. */
    #ਜੇ ( configASSERT_DEFINED == 1 )
    {
        /* The value written just has to be identifiable when looking at the
         * memory.  Don't use 0xA5 as that is the stack fill value and could
         * result in confusion as to what is actually being observed. */
        ਅਚਲ ਅਧਾਰ_Type_t x_Write_ਮੁੱਲ = 0x55;
        configASSERT( memset( puc_ਬਫਰ, ( ਪੂਰਨ ) x_Write_ਮੁੱਲ, x_ਬਫਰ_ਮਾਪ_ਬਾਈਟਾਂ ) == puc_ਬਫਰ );
        ( ਖਾਲੀ ) x_Write_ਮੁੱਲ;
    } /*lint !e529 !e438 xWriteValue is only used if configASSERT() is defined. */
    #ਜੇ_ਅੰਤ

    /* This function could be called from xStreamBufferReset(), so we reset the
     * stream buffer fields manually in order to avoid clearing
     * xStreamBufferLock. The xStreamBufferLock is initialized separately on
     * stream buffer creation. */
    px_ਧਾਰਾ_ਬਫਰ->x_ਪੂਛ = ( size_t ) 0;
    px_ਧਾਰਾ_ਬਫਰ->x_ਸਿਰ = ( size_t ) 0;
    px_ਧਾਰਾ_ਬਫਰ->x_Task_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive = ( TaskHandle_t ) 0;
    px_ਧਾਰਾ_ਬਫਰ->x_Task_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send = ( TaskHandle_t ) 0;
    #ਜੇ ( configUSE_TRACE_FACILITY == 1 )
        px_ਧਾਰਾ_ਬਫਰ->ux_ਧਾਰਾ_ਬਫਰ_ਸੰਖਿਆ = ( UBaseType_t ) 0;
    #ਜੇ_ਅੰਤ
    px_ਧਾਰਾ_ਬਫਰ->puc_ਬਫਰ = puc_ਬਫਰ;
    px_ਧਾਰਾ_ਬਫਰ->x_ਲੰਮਾਈ = x_ਬਫਰ_ਮਾਪ_ਬਾਈਟਾਂ;
    px_ਧਾਰਾ_ਬਫਰ->x_Trigger_Level_ਬਾਈਟਾਂ = x_Trigger_Level_ਬਾਈਟਾਂ;
    px_ਧਾਰਾ_ਬਫਰ->ucFlags = ucFlags;
    #ਜੇ ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    {
        px_ਧਾਰਾ_ਬਫਰ->pxSendCompletedCallback = pxSendCompletedCallback;
        px_ਧਾਰਾ_ਬਫਰ->pxReceiveCompletedCallback = pxReceiveCompletedCallback;
    }
    #ਨਹੀਂਤਾਂ
    {
        ( ਖਾਲੀ ) pxSendCompletedCallback;
        ( ਖਾਲੀ ) pxReceiveCompletedCallback;
    }
    #ਜੇ_ਅੰਤ
}

#ਜੇ ( configUSE_TRACE_FACILITY == 1 )

    UBaseType_t ux_ਧਾਰਾ_ਬਫਰ_Get_ਧਾਰਾ_ਬਫਰ_ਸੰਖਿਆ( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ )
    {
        ਵਾਪਸ x_ਧਾਰਾ_ਬਫਰ->ux_ਧਾਰਾ_ਬਫਰ_ਸੰਖਿਆ;
    }

#ਜੇ_ਅੰਤ /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#ਜੇ ( configUSE_TRACE_FACILITY == 1 )

    ਖਾਲੀ v_ਧਾਰਾ_ਬਫਰ_ਸੈੱਟ_ਧਾਰਾ_ਬਫਰ_ਸੰਖਿਆ( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ,
                                             UBaseType_t ux_ਧਾਰਾ_ਬਫਰ_ਸੰਖਿਆ )
    {
        x_ਧਾਰਾ_ਬਫਰ->ux_ਧਾਰਾ_ਬਫਰ_ਸੰਖਿਆ = ux_ਧਾਰਾ_ਬਫਰ_ਸੰਖਿਆ;
    }

#ਜੇ_ਅੰਤ /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#ਜੇ ( configUSE_TRACE_FACILITY == 1 )

    uint8_t uc_ਧਾਰਾ_ਬਫਰ_Get_ਧਾਰਾ_ਬਫਰ_Type( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ )
    {
        ਵਾਪਸ( x_ਧਾਰਾ_ਬਫਰ->ucFlags & sb_FLAGS_IS_ਸੁਨੇਹਾ_ਬਫਰ );
    }

#ਜੇ_ਅੰਤ /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/
