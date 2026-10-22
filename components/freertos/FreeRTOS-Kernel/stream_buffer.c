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
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <string.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* FreeRTOS includes. */
#ಅನ್ತರ್ಭಾವ "FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "task.h"
#ಅನ್ತರ್ಭಾವ "stream_buffer.h"
/* Include private IDF API additions for critical thread safety macros */
#ಅನ್ತರ್ಭಾವ "esp_private/freertos_idf_additions_priv.h"

#ಯದಿ ( configUSE_TASK_NOTIFICATIONS != 1 )
    #error configUSE_TASK_NOTIFICATIONS must be set to 1 to build stream_buffer.c
#endif

#ಯದಿ ( INCLUDE_xTaskGetCurrentTaskHandle != 1 )
    #error INCLUDE_xTaskGetCurrentTaskHandle must be set to 1 to build stream_buffer.c
#endif

/* Lint e961, e9021 and e750 are suppressed as a MISRA exception justified
 * because the MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#undef MPU_WRAPPERS_INCLUDED_FROM_API_FILE /*lint !e961 !e750 !e9021. */

/* If the user has not provided application specific Rx notification macros,
 * or #defined the notification macros away, then provide default implementations
 * that uses task notifications. */
/*lint -save -e9026 Function like macros allowed and needed here so they can be overridden. */
#ifndef sbRECEIVE_COMPLETED
    #define sbRECEIVE_COMPLETED( pxStreamBuffer )                               \
    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( pxStreamBuffer->xStreamBufferLock ) ); \
    {                                                                           \
        ಯದಿ( ( pxStreamBuffer )->xTaskWaitingToSend != ರಿಕ್ತ )                    \
        {                                                                       \
            ( ಶೂನ್ಯ ) xTaskNotify( ( pxStreamBuffer )->xTaskWaitingToSend,       \
                                  ( uint32_t ) 0,                               \
                                  eNoAction );                                  \
            ( pxStreamBuffer )->xTaskWaitingToSend = ರಿಕ್ತ;                      \
        }                                                                       \
    }                                                                           \
    ( ಶೂನ್ಯ ) prvEXIT_CRITICAL_OR_RESUME_ALL( &( pxStreamBuffer->xStreamBufferLock ) );
#endif /* sbRECEIVE_COMPLETED */

/* If user has provided a per-instance receive complete callback, then
 * invoke the callback else use the receive complete macro which is provided by default for all instances.
 */
#ಯದಿ ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #define prvRECEIVE_COMPLETED( pxStreamBuffer )                                               \
    {                                                                                            \
        ಯದಿ( ( pxStreamBuffer )->pxReceiveCompletedCallback != ರಿಕ್ತ )                             \
        {                                                                                        \
            ( pxStreamBuffer )->pxReceiveCompletedCallback( ( pxStreamBuffer ), pdFALSE, ರಿಕ್ತ ); \
        }                                                                                        \
        ಅನ್ಯಥಾ                                                                                     \
        {                                                                                        \
            sbRECEIVE_COMPLETED( ( pxStreamBuffer ) );                                           \
        }                                                                                        \
    }
#ಅನ್ಯಥಾ /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #define prvRECEIVE_COMPLETED( pxStreamBuffer )    sbRECEIVE_COMPLETED( ( pxStreamBuffer ) )
#endif /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

#ifndef sbRECEIVE_COMPLETED_FROM_ISR
    #define sbRECEIVE_COMPLETED_FROM_ISR( pxStreamBuffer,                            \
                                          pxHigherPriorityTaskWoken )                \
    {                                                                                \
        UBaseType_t uxSavedInterruptStatus;                                          \
                                                                                     \
        uxSavedInterruptStatus = ( UBaseType_t ) portSET_INTERRUPT_MASK_FROM_ISR();  \
        {                                                                            \
            ಯದಿ( ( pxStreamBuffer )->xTaskWaitingToSend != ರಿಕ್ತ )                     \
            {                                                                        \
                ( ಶೂನ್ಯ ) xTaskNotifyFromISR( ( pxStreamBuffer )->xTaskWaitingToSend, \
                                             ( uint32_t ) 0,                         \
                                             eNoAction,                              \
                                             ( pxHigherPriorityTaskWoken ) );        \
                ( pxStreamBuffer )->xTaskWaitingToSend = ರಿಕ್ತ;                       \
            }                                                                        \
        }                                                                            \
        portCLEAR_INTERRUPT_MASK_FROM_ISR( uxSavedInterruptStatus );                 \
    }
#endif /* sbRECEIVE_COMPLETED_FROM_ISR */

#ಯದಿ ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #define prvRECEIVE_COMPLETED_FROM_ISR( pxStreamBuffer,                                                               \
                                           pxHigherPriorityTaskWoken )                                                   \
    {                                                                                                                    \
        ಯದಿ( ( pxStreamBuffer )->pxReceiveCompletedCallback != ರಿಕ್ತ )                                                     \
        {                                                                                                                \
            ( pxStreamBuffer )->pxReceiveCompletedCallback( ( pxStreamBuffer ), pdTRUE, ( pxHigherPriorityTaskWoken ) ); \
        }                                                                                                                \
        ಅನ್ಯಥಾ                                                                                                             \
        {                                                                                                                \
            sbRECEIVE_COMPLETED_FROM_ISR( ( pxStreamBuffer ), ( pxHigherPriorityTaskWoken ) );                           \
        }                                                                                                                \
    }
#ಅನ್ಯಥಾ /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #define prvRECEIVE_COMPLETED_FROM_ISR( pxStreamBuffer, pxHigherPriorityTaskWoken ) \
    sbRECEIVE_COMPLETED_FROM_ISR( ( pxStreamBuffer ), ( pxHigherPriorityTaskWoken ) )
#endif /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

/* If the user has not provided an application specific Tx notification macro,
 * or #defined the notification macro away, then provide a default
 * implementation that uses task notifications.
 */
#ifndef sbSEND_COMPLETED
    #define sbSEND_COMPLETED( pxStreamBuffer )                                  \
    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( pxStreamBuffer->xStreamBufferLock ) ); \
    {                                                                           \
        ಯದಿ( ( pxStreamBuffer )->xTaskWaitingToReceive != ರಿಕ್ತ )                 \
        {                                                                       \
            ( ಶೂನ್ಯ ) xTaskNotify( ( pxStreamBuffer )->xTaskWaitingToReceive,    \
                                  ( uint32_t ) 0,                               \
                                  eNoAction );                                  \
            ( pxStreamBuffer )->xTaskWaitingToReceive = ರಿಕ್ತ;                   \
        }                                                                       \
    }                                                                           \
    ( ಶೂನ್ಯ ) prvEXIT_CRITICAL_OR_RESUME_ALL( &( pxStreamBuffer->xStreamBufferLock ) );
#endif /* sbSEND_COMPLETED */

/* If user has provided a per-instance send completed callback, then
 * invoke the callback else use the send complete macro which is provided by default for all instances.
 */
#ಯದಿ ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #define prvSEND_COMPLETED( pxStreamBuffer )                                           \
    {                                                                                     \
        ಯದಿ( ( pxStreamBuffer )->pxSendCompletedCallback != ರಿಕ್ತ )                         \
        {                                                                                 \
            pxStreamBuffer->pxSendCompletedCallback( ( pxStreamBuffer ), pdFALSE, ರಿಕ್ತ ); \
        }                                                                                 \
        ಅನ್ಯಥಾ                                                                              \
        {                                                                                 \
            sbSEND_COMPLETED( ( pxStreamBuffer ) );                                       \
        }                                                                                 \
    }
#ಅನ್ಯಥಾ /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #define prvSEND_COMPLETED( pxStreamBuffer )    sbSEND_COMPLETED( ( pxStreamBuffer ) )
#endif /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */


#ifndef sbSEND_COMPLETE_FROM_ISR
    #define sbSEND_COMPLETE_FROM_ISR( pxStreamBuffer, pxHigherPriorityTaskWoken )       \
    {                                                                                   \
        UBaseType_t uxSavedInterruptStatus;                                             \
                                                                                        \
        uxSavedInterruptStatus = ( UBaseType_t ) portSET_INTERRUPT_MASK_FROM_ISR();     \
        {                                                                               \
            ಯದಿ( ( pxStreamBuffer )->xTaskWaitingToReceive != ರಿಕ್ತ )                     \
            {                                                                           \
                ( ಶೂನ್ಯ ) xTaskNotifyFromISR( ( pxStreamBuffer )->xTaskWaitingToReceive, \
                                             ( uint32_t ) 0,                            \
                                             eNoAction,                                 \
                                             ( pxHigherPriorityTaskWoken ) );           \
                ( pxStreamBuffer )->xTaskWaitingToReceive = ರಿಕ್ತ;                       \
            }                                                                           \
        }                                                                               \
        portCLEAR_INTERRUPT_MASK_FROM_ISR( uxSavedInterruptStatus );                    \
    }
#endif /* sbSEND_COMPLETE_FROM_ISR */


#ಯದಿ ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #define prvSEND_COMPLETE_FROM_ISR( pxStreamBuffer, pxHigherPriorityTaskWoken )                                    \
    {                                                                                                                 \
        ಯದಿ( ( pxStreamBuffer )->pxSendCompletedCallback != ರಿಕ್ತ )                                                     \
        {                                                                                                             \
            ( pxStreamBuffer )->pxSendCompletedCallback( ( pxStreamBuffer ), pdTRUE, ( pxHigherPriorityTaskWoken ) ); \
        }                                                                                                             \
        ಅನ್ಯಥಾ                                                                                                          \
        {                                                                                                             \
            sbSEND_COMPLETE_FROM_ISR( ( pxStreamBuffer ), ( pxHigherPriorityTaskWoken ) );                            \
        }                                                                                                             \
    }
#ಅನ್ಯಥಾ /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #define prvSEND_COMPLETE_FROM_ISR( pxStreamBuffer, pxHigherPriorityTaskWoken ) \
    sbSEND_COMPLETE_FROM_ISR( ( pxStreamBuffer ), ( pxHigherPriorityTaskWoken ) )
#endif /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

/*lint -restore (9026) */

/* The number of bytes used to hold the length of a message in the buffer. */
#define sbBYTES_TO_STORE_MESSAGE_LENGTH    ( ಪರಿಮಾಣ( configMESSAGE_BUFFER_LENGTH_TYPE ) )

/* Bits stored in the ucFlags field of the stream buffer. */
#define sbFLAGS_IS_MESSAGE_BUFFER          ( ( uint8_t ) 1 ) /* Set if the stream buffer was created as a message buffer, in which case it holds discrete messages rather than a stream. */
#define sbFLAGS_IS_STATICALLY_ALLOCATED    ( ( uint8_t ) 2 ) /* Set if the stream buffer was created using statically allocated memory. */

/*-----------------------------------------------------------*/

/* Structure that hold state information on the buffer. */
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ StreamBufferDef_t                 /*lint !e9058 Style convention uses tag. */
{
    ಚಞ್ಚಲ size_t xTail;                       /* Index to the next item to read within the buffer. */
    ಚಞ್ಚಲ size_t xHead;                       /* Index to the next item to write within the buffer. */
    size_t xLength;                              /* The length of the buffer pointed to by pucBuffer. */
    size_t xTriggerLevelBytes;                   /* The number of bytes that must be in the stream buffer before a task that is waiting for data is unblocked. */
    ಚಞ್ಚಲ TaskHandle_t xTaskWaitingToReceive; /* Holds the handle of a task waiting for data, or NULL if no tasks are waiting. */
    ಚಞ್ಚಲ TaskHandle_t xTaskWaitingToSend;    /* Holds the handle of a task waiting to send data to a message buffer that is full. */
    uint8_t * pucBuffer;                         /* Points to the buffer itself - that is - the RAM that stores the data passed through the buffer. */
    uint8_t ucFlags;

    #ಯದಿ ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_Stream_Buffer_ಸಂಖ್ಯೆ; /* Used for tracing purposes. */
    #endif

    #ಯದಿ ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        StreamBufferCallbackFunction_t pxSendCompletedCallback;    /* Optional callback called on send complete. sbSEND_COMPLETED is called if this is NULL. */
        StreamBufferCallbackFunction_t pxReceiveCompletedCallback; /* Optional callback called on receive complete.  sbRECEIVE_COMPLETED is called if this is NULL. */
    #endif

    portMUX_TYPE xStreamBufferLock; /* Spinlock required for SMP critical sections */
} StreamBuffer_t;

/*
 * The number of bytes available to be read from the buffer.
 */
ಸ್ಥಿರ size_t prvBytesInBuffer( ಧ್ರುವ StreamBuffer_t * ಧ್ರುವ pxStreamBuffer ) PRIVILEGED_FUNCTION;

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
ಸ್ಥಿರ size_t prvWriteBytesToBuffer( StreamBuffer_t * ಧ್ರುವ pxStreamBuffer,
                                     ಧ್ರುವ uint8_t * pucData,
                                     size_t x_ಎಣಿಕೆ,
                                     size_t xHead ) PRIVILEGED_FUNCTION;

/*
 * If the stream buffer is being used as a message buffer, then reads an entire
 * message out of the buffer.  If the stream buffer is being used as a stream
 * buffer then read as many bytes as possible from the buffer.
 * prvReadBytesFromBuffer() is called to actually extract the bytes from the
 * buffer's data storage area.
 */
ಸ್ಥಿರ size_t prvReadMessageFromBuffer( StreamBuffer_t * pxStreamBuffer,
                                        ಶೂನ್ಯ * pvRxData,
                                        size_t xBufferLengthBytes,
                                        size_t xBytesAvailable ) PRIVILEGED_FUNCTION;

/*
 * If the stream buffer is being used as a message buffer, then writes an entire
 * message to the buffer.  If the stream buffer is being used as a stream
 * buffer then write as many bytes as possible to the buffer.
 * prvWriteBytestoBuffer() is called to actually send the bytes to the buffer's
 * data storage area.
 */
ಸ್ಥಿರ size_t prvWriteMessageToBuffer( StreamBuffer_t * ಧ್ರುವ pxStreamBuffer,
                                       ಧ್ರುವ ಶೂನ್ಯ * pvTxData,
                                       size_t xDataLengthBytes,
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
ಸ್ಥಿರ size_t prvReadBytesFromBuffer( StreamBuffer_t * pxStreamBuffer,
                                      uint8_t * pucData,
                                      size_t x_ಎಣಿಕೆ,
                                      size_t xTail ) PRIVILEGED_FUNCTION;

/*
 * Called by both pxStreamBufferCreate() and pxStreamBufferCreateStatic() to
 * initialise the members of the newly created stream buffer structure.
 */
ಸ್ಥಿರ ಶೂನ್ಯ prvInitialiseNewStreamBuffer( StreamBuffer_t * ಧ್ರುವ pxStreamBuffer,
                                          uint8_t * ಧ್ರುವ pucBuffer,
                                          size_t xBufferSizeBytes,
                                          size_t xTriggerLevelBytes,
                                          uint8_t ucFlags,
                                          StreamBufferCallbackFunction_t pxSendCompletedCallback,
                                          StreamBufferCallbackFunction_t pxReceiveCompletedCallback ) PRIVILEGED_FUNCTION;

/*-----------------------------------------------------------*/
#ಯದಿ ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
    StreamBufferHandle_t xStreamBufferGenericCreate( size_t xBufferSizeBytes,
                                                     size_t xTriggerLevelBytes,
                                                     BaseType_t xIsMessageBuffer,
                                                     StreamBufferCallbackFunction_t pxSendCompletedCallback,
                                                     StreamBufferCallbackFunction_t pxReceiveCompletedCallback )
    {
        uint8_t * pucAllocatedMemory;
        uint8_t ucFlags;

        /* In case the stream buffer is going to be used as a message buffer
         * (that is, it will hold discrete messages with a little meta data that
         * says how big the next message is) check the buffer will be large enough
         * to hold at least one message. */
        ಯದಿ( xIsMessageBuffer == pdTRUE )
        {
            /* Is a message buffer but not statically allocated. */
            ucFlags = sbFLAGS_IS_MESSAGE_BUFFER;
            configASSERT( xBufferSizeBytes > sbBYTES_TO_STORE_MESSAGE_LENGTH );
        }
        ಅನ್ಯಥಾ
        {
            /* Not a message buffer and not statically allocated. */
            ucFlags = 0;
            configASSERT( xBufferSizeBytes > 0 );
        }

        configASSERT( xTriggerLevelBytes <= xBufferSizeBytes );

        /* A trigger level of 0 would cause a waiting task to unblock even when
         * the buffer was empty. */
        ಯದಿ( xTriggerLevelBytes == ( size_t ) 0 )
        {
            xTriggerLevelBytes = ( size_t ) 1;
        }

        /* A stream buffer requires a StreamBuffer_t structure and a buffer.
         * Both are allocated in a single call to pvPortMalloc().  The
         * StreamBuffer_t structure is placed at the start of the allocated memory
         * and the buffer follows immediately after.  The requested size is
         * incremented so the free space is returned as the user would expect -
         * this is a quirk of the implementation that means otherwise the free
         * space would be reported as one byte smaller than would be logically
         * expected. */
        ಯದಿ( xBufferSizeBytes < ( xBufferSizeBytes + 1 + ಪರಿಮಾಣ( StreamBuffer_t ) ) )
        {
            xBufferSizeBytes++;
            pucAllocatedMemory = ( uint8_t * ) pvPortMalloc( xBufferSizeBytes + ಪರಿಮಾಣ( StreamBuffer_t ) ); /*lint !e9079 malloc() only returns void*. */
        }
        ಅನ್ಯಥಾ
        {
            pucAllocatedMemory = ರಿಕ್ತ;
        }

        ಯದಿ( pucAllocatedMemory != ರಿಕ್ತ )
        {
            prvInitialiseNewStreamBuffer( ( StreamBuffer_t * ) pucAllocatedMemory,       /* Structure at the start of the allocated memory. */ /*lint !e9087 Safe cast as allocated memory is aligned. */ /*lint !e826 Area is not too small and alignment is guaranteed provided malloc() behaves as expected and returns aligned buffer. */
                                          pucAllocatedMemory + ಪರಿಮಾಣ( StreamBuffer_t ), /* Storage area follows. */ /*lint !e9016 Indexing past structure valid for uint8_t pointer, also storage area has no alignment requirement. */
                                          xBufferSizeBytes,
                                          xTriggerLevelBytes,
                                          ucFlags,
                                          pxSendCompletedCallback,
                                          pxReceiveCompletedCallback );

            /* Initialize the stream buffer's spinlock separately, as
             * prvInitialiseNewStreamBuffer() is also called from
             * xStreamBufferReset(). */
            portMUX_INITIALIZE( &( ( ( StreamBuffer_t * ) pucAllocatedMemory )->xStreamBufferLock ) );

            traceSTREAM_BUFFER_CREATE( ( ( StreamBuffer_t * ) pucAllocatedMemory ), xIsMessageBuffer );
        }
        ಅನ್ಯಥಾ
        {
            traceSTREAM_BUFFER_CREATE_FAILED( xIsMessageBuffer );
        }

        ಪ್ರತ್ಯಾಗಮ ( StreamBufferHandle_t ) pucAllocatedMemory; /*lint !e9087 !e826 Safe cast as allocated memory is aligned. */
    }
#endif /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

#ಯದಿ ( configSUPPORT_STATIC_ALLOCATION == 1 )

    StreamBufferHandle_t xStreamBufferGenericCreateStatic( size_t xBufferSizeBytes,
                                                           size_t xTriggerLevelBytes,
                                                           BaseType_t xIsMessageBuffer,
                                                           uint8_t * ಧ್ರುವ pucStreamBufferStorageArea,
                                                           StaticStreamBuffer_t * ಧ್ರುವ pxStaticStreamBuffer,
                                                           StreamBufferCallbackFunction_t pxSendCompletedCallback,
                                                           StreamBufferCallbackFunction_t pxReceiveCompletedCallback )
    {
        StreamBuffer_t * ಧ್ರುವ pxStreamBuffer = ( StreamBuffer_t * ) pxStaticStreamBuffer; /*lint !e740 !e9087 Safe cast as StaticStreamBuffer_t is opaque Streambuffer_t. */
        StreamBufferHandle_t xReturn;
        uint8_t ucFlags;

        configASSERT( pucStreamBufferStorageArea );
        configASSERT( pxStaticStreamBuffer );
        configASSERT( xTriggerLevelBytes <= xBufferSizeBytes );

        /* A trigger level of 0 would cause a waiting task to unblock even when
         * the buffer was empty. */
        ಯದಿ( xTriggerLevelBytes == ( size_t ) 0 )
        {
            xTriggerLevelBytes = ( size_t ) 1;
        }

        ಯದಿ( xIsMessageBuffer != pdFALSE )
        {
            /* Statically allocated message buffer. */
            ucFlags = sbFLAGS_IS_MESSAGE_BUFFER | sbFLAGS_IS_STATICALLY_ALLOCATED;
        }
        ಅನ್ಯಥಾ
        {
            /* Statically allocated stream buffer. */
            ucFlags = sbFLAGS_IS_STATICALLY_ALLOCATED;
        }

        /* In case the stream buffer is going to be used as a message buffer
         * (that is, it will hold discrete messages with a little meta data that
         * says how big the next message is) check the buffer will be large enough
         * to hold at least one message. */
        configASSERT( xBufferSizeBytes > sbBYTES_TO_STORE_MESSAGE_LENGTH );

        #ಯದಿ ( configASSERT_DEFINED == 1 )
        {
            /* Sanity check that the size of the structure used to declare a
             * variable of type StaticStreamBuffer_t equals the size of the real
             * message buffer structure. */
            ಚಞ್ಚಲ size_t xSize = ಪರಿಮಾಣ( StaticStreamBuffer_t );
            configASSERT( xSize == ಪರಿಮಾಣ( StreamBuffer_t ) );
            ( ಶೂನ್ಯ ) xSize; /* Prevent unused variable warning when configASSERT() is not used. */
        } /*lint !e529 xSize is referenced is configASSERT() is defined. */
        #endif /* configASSERT_DEFINED */

        ಯದಿ( ( pucStreamBufferStorageArea != ರಿಕ್ತ ) && ( pxStaticStreamBuffer != ರಿಕ್ತ ) )
        {
            prvInitialiseNewStreamBuffer( pxStreamBuffer,
                                          pucStreamBufferStorageArea,
                                          xBufferSizeBytes,
                                          xTriggerLevelBytes,
                                          ucFlags,
                                          pxSendCompletedCallback,
                                          pxReceiveCompletedCallback );

            /* Remember this was statically allocated in case it is ever deleted
             * again. */
            pxStreamBuffer->ucFlags |= sbFLAGS_IS_STATICALLY_ALLOCATED;

            /* Initialize the stream buffer's spinlock separately, as
             * prvInitialiseNewStreamBuffer() is also called from
             * xStreamBufferReset(). */
            portMUX_INITIALIZE( &( pxStreamBuffer->xStreamBufferLock ) );

            traceSTREAM_BUFFER_CREATE( pxStreamBuffer, xIsMessageBuffer );

            xReturn = ( StreamBufferHandle_t ) pxStaticStreamBuffer; /*lint !e9087 Data hiding requires cast to opaque type. */
        }
        ಅನ್ಯಥಾ
        {
            xReturn = ರಿಕ್ತ;
            traceSTREAM_BUFFER_CREATE_STATIC_FAILED( xReturn, xIsMessageBuffer );
        }

        ಪ್ರತ್ಯಾಗಮ xReturn;
    }
#endif /* ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*-----------------------------------------------------------*/

#ಯದಿ ( configSUPPORT_STATIC_ALLOCATION == 1 )
    BaseType_t xStreamBufferGetStaticBuffers( StreamBufferHandle_t xStreamBuffer,
                                              uint8_t ** ppucStreamBufferStorageArea,
                                              StaticStreamBuffer_t ** ppxStaticStreamBuffer )
    {
        BaseType_t xReturn;
        ಧ್ರುವ StreamBuffer_t * ಧ್ರುವ pxStreamBuffer = xStreamBuffer;

        configASSERT( pxStreamBuffer );
        configASSERT( ppucStreamBufferStorageArea );
        configASSERT( ppxStaticStreamBuffer );

        ಯದಿ( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_STATICALLY_ALLOCATED ) != ( uint8_t ) 0 )
        {
            *ppucStreamBufferStorageArea = pxStreamBuffer->pucBuffer;
            *ppxStaticStreamBuffer = ( StaticStreamBuffer_t * ) pxStreamBuffer;
            xReturn = pdTRUE;
        }
        ಅನ್ಯಥಾ
        {
            xReturn = pdFALSE;
        }

        ಪ್ರತ್ಯಾಗಮ xReturn;
    }
#endif /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

ಶೂನ್ಯ vStreamBufferDelete( StreamBufferHandle_t xStreamBuffer )
{
    StreamBuffer_t * pxStreamBuffer = xStreamBuffer;

    configASSERT( pxStreamBuffer );

    traceSTREAM_BUFFER_DELETE( xStreamBuffer );

    ಯದಿ( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_STATICALLY_ALLOCATED ) == ( uint8_t ) pdFALSE )
    {
        #ಯದಿ ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
        {
            /* Both the structure and the buffer were allocated using a single call
            * to pvPortMalloc(), hence only one call to vPortFree() is required. */
            vPortFree( ( ಶೂನ್ಯ * ) pxStreamBuffer ); /*lint !e9087 Standard free() semantics require void *, plus pxStreamBuffer was allocated by pvPortMalloc(). */
        }
        #ಅನ್ಯಥಾ
        {
            /* Should not be possible to get here, ucFlags must be corrupt.
             * Force an assert. */
            configASSERT( xStreamBuffer == ( StreamBufferHandle_t ) ~0 );
        }
        #endif
    }
    ಅನ್ಯಥಾ
    {
        /* The structure and buffer were not allocated dynamically and cannot be
         * freed - just scrub the structure so future use will assert. */
        ( ಶೂನ್ಯ ) memset( pxStreamBuffer, 0x00, ಪರಿಮಾಣ( StreamBuffer_t ) );
    }
}
/*-----------------------------------------------------------*/

BaseType_t xStreamBufferReset( StreamBufferHandle_t xStreamBuffer )
{
    StreamBuffer_t * ಧ್ರುವ pxStreamBuffer = xStreamBuffer;
    BaseType_t xReturn = pdFAIL;
    StreamBufferCallbackFunction_t pxSendCallback = ರಿಕ್ತ, pxReceiveCallback = ರಿಕ್ತ;

    #ಯದಿ ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_Stream_Buffer_ಸಂಖ್ಯೆ;
    #endif

    configASSERT( pxStreamBuffer );

    #ಯದಿ ( configUSE_TRACE_FACILITY == 1 )
    {
        /* Store the stream buffer number so it can be restored after the
         * reset. */
        ux_Stream_Buffer_ಸಂಖ್ಯೆ = pxStreamBuffer->ux_Stream_Buffer_ಸಂಖ್ಯೆ;
    }
    #endif

    /* Can only reset a message buffer if there are no tasks blocked on it. */
    taskENTER_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );
    {
        ಯದಿ( ( pxStreamBuffer->xTaskWaitingToReceive == ರಿಕ್ತ ) && ( pxStreamBuffer->xTaskWaitingToSend == ರಿಕ್ತ ) )
        {
            #ಯದಿ ( configUSE_SB_COMPLETED_CALLBACK == 1 )
            {
                pxSendCallback = pxStreamBuffer->pxSendCompletedCallback;
                pxReceiveCallback = pxStreamBuffer->pxReceiveCompletedCallback;
            }
            #endif

            prvInitialiseNewStreamBuffer( pxStreamBuffer,
                                          pxStreamBuffer->pucBuffer,
                                          pxStreamBuffer->xLength,
                                          pxStreamBuffer->xTriggerLevelBytes,
                                          pxStreamBuffer->ucFlags,
                                          pxSendCallback,
                                          pxReceiveCallback );

            #ಯದಿ ( configUSE_TRACE_FACILITY == 1 )
            {
                pxStreamBuffer->ux_Stream_Buffer_ಸಂಖ್ಯೆ = ux_Stream_Buffer_ಸಂಖ್ಯೆ;
            }
            #endif

            traceSTREAM_BUFFER_RESET( xStreamBuffer );

            xReturn = pdPASS;
        }
    }
    taskEXIT_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );

    ಪ್ರತ್ಯಾಗಮ xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xStreamBufferSetTriggerLevel( StreamBufferHandle_t xStreamBuffer,
                                         size_t xTriggerLevel )
{
    StreamBuffer_t * ಧ್ರುವ pxStreamBuffer = xStreamBuffer;
    BaseType_t xReturn;

    configASSERT( pxStreamBuffer );

    /* It is not valid for the trigger level to be 0. */
    ಯದಿ( xTriggerLevel == ( size_t ) 0 )
    {
        xTriggerLevel = ( size_t ) 1;
    }

    /* The trigger level is the number of bytes that must be in the stream
     * buffer before a task that is waiting for data is unblocked. */
    ಯದಿ( xTriggerLevel < pxStreamBuffer->xLength )
    {
        pxStreamBuffer->xTriggerLevelBytes = xTriggerLevel;
        xReturn = pdPASS;
    }
    ಅನ್ಯಥಾ
    {
        xReturn = pdFALSE;
    }

    ಪ್ರತ್ಯಾಗಮ xReturn;
}
/*-----------------------------------------------------------*/

size_t xStreamBufferSpacesAvailable( StreamBufferHandle_t xStreamBuffer )
{
    ಧ್ರುವ StreamBuffer_t * ಧ್ರುವ pxStreamBuffer = xStreamBuffer;
    size_t xSpace;
    size_t xOriginalTail;

    configASSERT( pxStreamBuffer );

    /* The code below reads xTail and then xHead.  This is safe if the stream
     * buffer is updated once between the two reads - but not if the stream buffer
     * is updated more than once between the two reads - hence the loop. */
    ಕುರು
    {
        xOriginalTail = pxStreamBuffer->xTail;
        xSpace = pxStreamBuffer->xLength + pxStreamBuffer->xTail;
        xSpace -= pxStreamBuffer->xHead;
    } ಯಾವತ್( xOriginalTail != pxStreamBuffer->xTail );

    xSpace -= ( size_t ) 1;

    ಯದಿ( xSpace >= pxStreamBuffer->xLength )
    {
        xSpace -= pxStreamBuffer->xLength;
    }
    ಅನ್ಯಥಾ
    {
        mtCOVERAGE_TEST_MARKER();
    }

    ಪ್ರತ್ಯಾಗಮ xSpace;
}
/*-----------------------------------------------------------*/

size_t xStreamBufferBytesAvailable( StreamBufferHandle_t xStreamBuffer )
{
    ಧ್ರುವ StreamBuffer_t * ಧ್ರುವ pxStreamBuffer = xStreamBuffer;
    size_t xReturn;

    configASSERT( pxStreamBuffer );

    xReturn = prvBytesInBuffer( pxStreamBuffer );
    ಪ್ರತ್ಯಾಗಮ xReturn;
}
/*-----------------------------------------------------------*/

size_t xStreamBufferSend( StreamBufferHandle_t xStreamBuffer,
                          ಧ್ರುವ ಶೂನ್ಯ * pvTxData,
                          size_t xDataLengthBytes,
                          TickType_t xTicksToWait )
{
    StreamBuffer_t * ಧ್ರುವ pxStreamBuffer = xStreamBuffer;
    size_t xReturn, xSpace = 0;
    size_t xRequiredSpace = xDataLengthBytes;
    TimeOut_t xTimeOut;
    size_t xMaxReportedSpace = 0;

    configASSERT( pvTxData );
    configASSERT( pxStreamBuffer );

    /* The maximum amount of space a stream buffer will ever report is its length
     * minus 1. */
    xMaxReportedSpace = pxStreamBuffer->xLength - ( size_t ) 1;

    /* This send function is used to write to both message buffers and stream
     * buffers.  If this is a message buffer then the space needed must be
     * increased by the amount of bytes needed to store the length of the
     * message. */
    ಯದಿ( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) != ( uint8_t ) 0 )
    {
        xRequiredSpace += sbBYTES_TO_STORE_MESSAGE_LENGTH;

        /* Overflow? */
        configASSERT( xRequiredSpace > xDataLengthBytes );

        /* If this is a message buffer then it must be possible to write the
         * whole message. */
        ಯದಿ( xRequiredSpace > xMaxReportedSpace )
        {
            /* The message would not fit even if the entire buffer was empty,
             * so don't wait for space. */
            xTicksToWait = ( TickType_t ) 0;
        }
        ಅನ್ಯಥಾ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    ಅನ್ಯಥಾ
    {
        /* If this is a stream buffer then it is acceptable to write only part
         * of the message to the buffer.  Cap the length to the total length of
         * the buffer. */
        ಯದಿ( xRequiredSpace > xMaxReportedSpace )
        {
            xRequiredSpace = xMaxReportedSpace;
        }
        ಅನ್ಯಥಾ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }

    ಯದಿ( xTicksToWait != ( TickType_t ) 0 )
    {
        vTaskSetTimeOutState( &xTimeOut );

        ಕುರು
        {
            /* Wait until the required number of bytes are free in the message
             * buffer. */
            taskENTER_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );
            {
                xSpace = xStreamBufferSpacesAvailable( pxStreamBuffer );

                ಯದಿ( xSpace < xRequiredSpace )
                {
                    /* Clear notification state as going to wait for space. */
                    ( ಶೂನ್ಯ ) xTaskNotifyStateClear( ರಿಕ್ತ );

                    /* Should only be one writer. */
                    configASSERT( pxStreamBuffer->xTaskWaitingToSend == ರಿಕ್ತ );
                    pxStreamBuffer->xTaskWaitingToSend = xTaskGetCurrentTaskHandle();
                }
                ಅನ್ಯಥಾ
                {
                    taskEXIT_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );
                    ಭಙ್ಗ;
                }
            }
            taskEXIT_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );

            traceBLOCKING_ON_STREAM_BUFFER_SEND( xStreamBuffer );
            ( ಶೂನ್ಯ ) xTaskNotifyWait( ( uint32_t ) 0, ( uint32_t ) 0, ರಿಕ್ತ, xTicksToWait );
            /* In SMP mode, the task may have been woken and scheduled on
             * another core. Hence, we must clear the xTaskWaitingToSend
             * handle in a critical section. */
            #ಯದಿ ( config_ಸಂಖ್ಯೆ_OF_CORES > 1 )
                taskENTER_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );
            #endif /* configNUMBER_OF_CORES > 1 */
            {
                pxStreamBuffer->xTaskWaitingToSend = ರಿಕ್ತ;
            }
            #ಯದಿ ( config_ಸಂಖ್ಯೆ_OF_CORES > 1 )
                taskEXIT_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );
            #endif /* configNUMBER_OF_CORES > 1 */
        } ಯಾವತ್( xTaskCheckForTimeOut( &xTimeOut, &xTicksToWait ) == pdFALSE );
    }
    ಅನ್ಯಥಾ
    {
        mtCOVERAGE_TEST_MARKER();
    }

    ಯದಿ( xSpace == ( size_t ) 0 )
    {
        xSpace = xStreamBufferSpacesAvailable( pxStreamBuffer );
    }
    ಅನ್ಯಥಾ
    {
        mtCOVERAGE_TEST_MARKER();
    }

    xReturn = prvWriteMessageToBuffer( pxStreamBuffer, pvTxData, xDataLengthBytes, xSpace, xRequiredSpace );

    ಯದಿ( xReturn > ( size_t ) 0 )
    {
        traceSTREAM_BUFFER_SEND( xStreamBuffer, xReturn );

        /* Was a task waiting for the data? */
        ಯದಿ( prvBytesInBuffer( pxStreamBuffer ) >= pxStreamBuffer->xTriggerLevelBytes )
        {
            prvSEND_COMPLETED( pxStreamBuffer );
        }
        ಅನ್ಯಥಾ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    ಅನ್ಯಥಾ
    {
        mtCOVERAGE_TEST_MARKER();
        traceSTREAM_BUFFER_SEND_FAILED( xStreamBuffer );
    }

    ಪ್ರತ್ಯಾಗಮ xReturn;
}
/*-----------------------------------------------------------*/

size_t xStreamBufferSendFromISR( StreamBufferHandle_t xStreamBuffer,
                                 ಧ್ರುವ ಶೂನ್ಯ * pvTxData,
                                 size_t xDataLengthBytes,
                                 BaseType_t * ಧ್ರುವ pxHigherPriorityTaskWoken )
{
    StreamBuffer_t * ಧ್ರುವ pxStreamBuffer = xStreamBuffer;
    size_t xReturn, xSpace;
    size_t xRequiredSpace = xDataLengthBytes;

    configASSERT( pvTxData );
    configASSERT( pxStreamBuffer );

    /* This send function is used to write to both message buffers and stream
     * buffers.  If this is a message buffer then the space needed must be
     * increased by the amount of bytes needed to store the length of the
     * message. */
    ಯದಿ( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) != ( uint8_t ) 0 )
    {
        xRequiredSpace += sbBYTES_TO_STORE_MESSAGE_LENGTH;
    }
    ಅನ್ಯಥಾ
    {
        mtCOVERAGE_TEST_MARKER();
    }

    xSpace = xStreamBufferSpacesAvailable( pxStreamBuffer );
    xReturn = prvWriteMessageToBuffer( pxStreamBuffer, pvTxData, xDataLengthBytes, xSpace, xRequiredSpace );

    ಯದಿ( xReturn > ( size_t ) 0 )
    {
        /* Was a task waiting for the data? */
        ಯದಿ( prvBytesInBuffer( pxStreamBuffer ) >= pxStreamBuffer->xTriggerLevelBytes )
        {
            prvSEND_COMPLETE_FROM_ISR( pxStreamBuffer, pxHigherPriorityTaskWoken );
        }
        ಅನ್ಯಥಾ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    ಅನ್ಯಥಾ
    {
        mtCOVERAGE_TEST_MARKER();
    }

    traceSTREAM_BUFFER_SEND_FROM_ISR( xStreamBuffer, xReturn );

    ಪ್ರತ್ಯಾಗಮ xReturn;
}
/*-----------------------------------------------------------*/

ಸ್ಥಿರ size_t prvWriteMessageToBuffer( StreamBuffer_t * ಧ್ರುವ pxStreamBuffer,
                                       ಧ್ರುವ ಶೂನ್ಯ * pvTxData,
                                       size_t xDataLengthBytes,
                                       size_t xSpace,
                                       size_t xRequiredSpace )
{
    size_t xNextHead = pxStreamBuffer->xHead;
    configMESSAGE_BUFFER_LENGTH_TYPE xMessageLength;

    ಯದಿ( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) != ( uint8_t ) 0 )
    {
        /* This is a message buffer, as opposed to a stream buffer. */

        /* Convert xDataLengthBytes to the message length type. */
        xMessageLength = ( configMESSAGE_BUFFER_LENGTH_TYPE ) xDataLengthBytes;

        /* Ensure the data length given fits within configMESSAGE_BUFFER_LENGTH_TYPE. */
        configASSERT( ( size_t ) xMessageLength == xDataLengthBytes );

        ಯದಿ( xSpace >= xRequiredSpace )
        {
            /* There is enough space to write both the message length and the message
             * itself into the buffer.  Start by writing the length of the data, the data
             * itself will be written later in this function. */
            xNextHead = prvWriteBytesToBuffer( pxStreamBuffer, ( ಧ್ರುವ uint8_t * ) &( xMessageLength ), sbBYTES_TO_STORE_MESSAGE_LENGTH, xNextHead );
        }
        ಅನ್ಯಥಾ
        {
            /* Not enough space, so do not write data to the buffer. */
            xDataLengthBytes = 0;
        }
    }
    ಅನ್ಯಥಾ
    {
        /* This is a stream buffer, as opposed to a message buffer, so writing a
         * stream of bytes rather than discrete messages.  Plan to write as many
         * bytes as possible. */
        xDataLengthBytes = configMIN( xDataLengthBytes, xSpace );
    }

    ಯದಿ( xDataLengthBytes != ( size_t ) 0 )
    {
        /* Write the data to the buffer. */
        pxStreamBuffer->xHead = prvWriteBytesToBuffer( pxStreamBuffer, ( ಧ್ರುವ uint8_t * ) pvTxData, xDataLengthBytes, xNextHead ); /*lint !e9079 Storage buffer is implemented as uint8_t for ease of sizing, alignment and access. */
    }

    ಪ್ರತ್ಯಾಗಮ xDataLengthBytes;
}
/*-----------------------------------------------------------*/

size_t xStreamBufferReceive( StreamBufferHandle_t xStreamBuffer,
                             ಶೂನ್ಯ * pvRxData,
                             size_t xBufferLengthBytes,
                             TickType_t xTicksToWait )
{
    StreamBuffer_t * ಧ್ರುವ pxStreamBuffer = xStreamBuffer;
    size_t xReceivedLength = 0, xBytesAvailable, xBytesToStoreMessageLength;

    configASSERT( pvRxData );
    configASSERT( pxStreamBuffer );

    /* This receive function is used by both message buffers, which store
     * discrete messages, and stream buffers, which store a continuous stream of
     * bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the
     * message. */
    ಯದಿ( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) != ( uint8_t ) 0 )
    {
        xBytesToStoreMessageLength = sbBYTES_TO_STORE_MESSAGE_LENGTH;
    }
    ಅನ್ಯಥಾ
    {
        xBytesToStoreMessageLength = 0;
    }

    ಯದಿ( xTicksToWait != ( TickType_t ) 0 )
    {
        /* Checking if there is data and clearing the notification state must be
         * performed atomically. */
        taskENTER_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );
        {
            xBytesAvailable = prvBytesInBuffer( pxStreamBuffer );

            /* If this function was invoked by a message buffer read then
             * xBytesToStoreMessageLength holds the number of bytes used to hold
             * the length of the next discrete message.  If this function was
             * invoked by a stream buffer read then xBytesToStoreMessageLength will
             * be 0. */
            ಯದಿ( xBytesAvailable <= xBytesToStoreMessageLength )
            {
                /* Clear notification state as going to wait for data. */
                ( ಶೂನ್ಯ ) xTaskNotifyStateClear( ರಿಕ್ತ );

                /* Should only be one reader. */
                configASSERT( pxStreamBuffer->xTaskWaitingToReceive == ರಿಕ್ತ );
                pxStreamBuffer->xTaskWaitingToReceive = xTaskGetCurrentTaskHandle();
            }
            ಅನ್ಯಥಾ
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        taskEXIT_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );

        ಯದಿ( xBytesAvailable <= xBytesToStoreMessageLength )
        {
            /* Wait for data to be available. */
            traceBLOCKING_ON_STREAM_BUFFER_RECEIVE( xStreamBuffer );
            ( ಶೂನ್ಯ ) xTaskNotifyWait( ( uint32_t ) 0, ( uint32_t ) 0, ರಿಕ್ತ, xTicksToWait );

            /* In SMP mode, the task may have been woken and scheduled on
             * another core. Hence, we must clear the xTaskWaitingToReceive
             * handle in a critical section. */
            #ಯದಿ ( config_ಸಂಖ್ಯೆ_OF_CORES > 1 )
                taskENTER_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );
            #endif /* configNUMBER_OF_CORES > 1 */
            {
                pxStreamBuffer->xTaskWaitingToReceive = ರಿಕ್ತ;
            }
            #ಯದಿ ( config_ಸಂಖ್ಯೆ_OF_CORES > 1 )
                taskEXIT_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );
            #endif /* configNUMBER_OF_CORES > 1 */

            /* Recheck the data available after blocking. */
            xBytesAvailable = prvBytesInBuffer( pxStreamBuffer );
        }
        ಅನ್ಯಥಾ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    ಅನ್ಯಥಾ
    {
        xBytesAvailable = prvBytesInBuffer( pxStreamBuffer );
    }

    /* Whether receiving a discrete message (where xBytesToStoreMessageLength
     * holds the number of bytes used to store the message length) or a stream of
     * bytes (where xBytesToStoreMessageLength is zero), the number of bytes
     * available must be greater than xBytesToStoreMessageLength to be able to
     * read bytes from the buffer. */
    ಯದಿ( xBytesAvailable > xBytesToStoreMessageLength )
    {
        xReceivedLength = prvReadMessageFromBuffer( pxStreamBuffer, pvRxData, xBufferLengthBytes, xBytesAvailable );

        /* Was a task waiting for space in the buffer? */
        ಯದಿ( xReceivedLength != ( size_t ) 0 )
        {
            traceSTREAM_BUFFER_RECEIVE( xStreamBuffer, xReceivedLength );
            prvRECEIVE_COMPLETED( xStreamBuffer );
        }
        ಅನ್ಯಥಾ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    ಅನ್ಯಥಾ
    {
        traceSTREAM_BUFFER_RECEIVE_FAILED( xStreamBuffer );
        mtCOVERAGE_TEST_MARKER();
    }

    ಪ್ರತ್ಯಾಗಮ xReceivedLength;
}
/*-----------------------------------------------------------*/

size_t xStreamBufferNextMessageLengthBytes( StreamBufferHandle_t xStreamBuffer )
{
    StreamBuffer_t * ಧ್ರುವ pxStreamBuffer = xStreamBuffer;
    size_t xReturn, xBytesAvailable;
    configMESSAGE_BUFFER_LENGTH_TYPE xTempReturn;

    configASSERT( pxStreamBuffer );

    /* Ensure the stream buffer is being used as a message buffer. */
    ಯದಿ( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) != ( uint8_t ) 0 )
    {
        xBytesAvailable = prvBytesInBuffer( pxStreamBuffer );

        ಯದಿ( xBytesAvailable > sbBYTES_TO_STORE_MESSAGE_LENGTH )
        {
            /* The number of bytes available is greater than the number of bytes
             * required to hold the length of the next message, so another message
             * is available. */
            ( ಶೂನ್ಯ ) prvReadBytesFromBuffer( pxStreamBuffer, ( uint8_t * ) &xTempReturn, sbBYTES_TO_STORE_MESSAGE_LENGTH, pxStreamBuffer->xTail );
            xReturn = ( size_t ) xTempReturn;
        }
        ಅನ್ಯಥಾ
        {
            /* The minimum amount of bytes in a message buffer is
             * ( sbBYTES_TO_STORE_MESSAGE_LENGTH + 1 ), so if xBytesAvailable is
             * less than sbBYTES_TO_STORE_MESSAGE_LENGTH the only other valid
             * value is 0. */
            configASSERT( xBytesAvailable == 0 );
            xReturn = 0;
        }
    }
    ಅನ್ಯಥಾ
    {
        xReturn = 0;
    }

    ಪ್ರತ್ಯಾಗಮ xReturn;
}
/*-----------------------------------------------------------*/

size_t xStreamBufferReceiveFromISR( StreamBufferHandle_t xStreamBuffer,
                                    ಶೂನ್ಯ * pvRxData,
                                    size_t xBufferLengthBytes,
                                    BaseType_t * ಧ್ರುವ pxHigherPriorityTaskWoken )
{
    StreamBuffer_t * ಧ್ರುವ pxStreamBuffer = xStreamBuffer;
    size_t xReceivedLength = 0, xBytesAvailable, xBytesToStoreMessageLength;

    configASSERT( pvRxData );
    configASSERT( pxStreamBuffer );

    /* This receive function is used by both message buffers, which store
     * discrete messages, and stream buffers, which store a continuous stream of
     * bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the
     * message. */
    ಯದಿ( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) != ( uint8_t ) 0 )
    {
        xBytesToStoreMessageLength = sbBYTES_TO_STORE_MESSAGE_LENGTH;
    }
    ಅನ್ಯಥಾ
    {
        xBytesToStoreMessageLength = 0;
    }

    xBytesAvailable = prvBytesInBuffer( pxStreamBuffer );

    /* Whether receiving a discrete message (where xBytesToStoreMessageLength
     * holds the number of bytes used to store the message length) or a stream of
     * bytes (where xBytesToStoreMessageLength is zero), the number of bytes
     * available must be greater than xBytesToStoreMessageLength to be able to
     * read bytes from the buffer. */
    ಯದಿ( xBytesAvailable > xBytesToStoreMessageLength )
    {
        xReceivedLength = prvReadMessageFromBuffer( pxStreamBuffer, pvRxData, xBufferLengthBytes, xBytesAvailable );

        /* Was a task waiting for space in the buffer? */
        ಯದಿ( xReceivedLength != ( size_t ) 0 )
        {
            prvRECEIVE_COMPLETED_FROM_ISR( pxStreamBuffer, pxHigherPriorityTaskWoken );
        }
        ಅನ್ಯಥಾ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    ಅನ್ಯಥಾ
    {
        mtCOVERAGE_TEST_MARKER();
    }

    traceSTREAM_BUFFER_RECEIVE_FROM_ISR( xStreamBuffer, xReceivedLength );

    ಪ್ರತ್ಯಾಗಮ xReceivedLength;
}
/*-----------------------------------------------------------*/

ಸ್ಥಿರ size_t prvReadMessageFromBuffer( StreamBuffer_t * pxStreamBuffer,
                                        ಶೂನ್ಯ * pvRxData,
                                        size_t xBufferLengthBytes,
                                        size_t xBytesAvailable )
{
    size_t x_ಎಣಿಕೆ, xNextMessageLength;
    configMESSAGE_BUFFER_LENGTH_TYPE xTempNextMessageLength;
    size_t xNextTail = pxStreamBuffer->xTail;

    ಯದಿ( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) != ( uint8_t ) 0 )
    {
        /* A discrete message is being received.  First receive the length
         * of the message. */
        xNextTail = prvReadBytesFromBuffer( pxStreamBuffer, ( uint8_t * ) &xTempNextMessageLength, sbBYTES_TO_STORE_MESSAGE_LENGTH, xNextTail );
        xNextMessageLength = ( size_t ) xTempNextMessageLength;

        /* Reduce the number of bytes available by the number of bytes just
         * read out. */
        xBytesAvailable -= sbBYTES_TO_STORE_MESSAGE_LENGTH;

        /* Check there is enough space in the buffer provided by the
         * user. */
        ಯದಿ( xNextMessageLength > xBufferLengthBytes )
        {
            /* The user has provided insufficient space to read the message. */
            xNextMessageLength = 0;
        }
        ಅನ್ಯಥಾ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    ಅನ್ಯಥಾ
    {
        /* A stream of bytes is being received (as opposed to a discrete
         * message), so read as many bytes as possible. */
        xNextMessageLength = xBufferLengthBytes;
    }

    /* Use the minimum of the wanted bytes and the available bytes. */
    x_ಎಣಿಕೆ = configMIN( xNextMessageLength, xBytesAvailable );

    ಯದಿ( x_ಎಣಿಕೆ != ( size_t ) 0 )
    {
        /* Read the actual data and update the tail to mark the data as officially consumed. */
        pxStreamBuffer->xTail = prvReadBytesFromBuffer( pxStreamBuffer, ( uint8_t * ) pvRxData, x_ಎಣಿಕೆ, xNextTail ); /*lint !e9079 Data storage area is implemented as uint8_t array for ease of sizing, indexing and alignment. */
    }

    ಪ್ರತ್ಯಾಗಮ x_ಎಣಿಕೆ;
}
/*-----------------------------------------------------------*/

BaseType_t xStreamBufferIsEmpty( StreamBufferHandle_t xStreamBuffer )
{
    ಧ್ರುವ StreamBuffer_t * ಧ್ರುವ pxStreamBuffer = xStreamBuffer;
    BaseType_t xReturn;
    size_t xTail;

    configASSERT( pxStreamBuffer );

    /* True if no bytes are available. */
    xTail = pxStreamBuffer->xTail;

    ಯದಿ( pxStreamBuffer->xHead == xTail )
    {
        xReturn = pdTRUE;
    }
    ಅನ್ಯಥಾ
    {
        xReturn = pdFALSE;
    }

    ಪ್ರತ್ಯಾಗಮ xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xStreamBufferIsFull( StreamBufferHandle_t xStreamBuffer )
{
    BaseType_t xReturn;
    size_t xBytesToStoreMessageLength;
    ಧ್ರುವ StreamBuffer_t * ಧ್ರುವ pxStreamBuffer = xStreamBuffer;

    configASSERT( pxStreamBuffer );

    /* This generic version of the receive function is used by both message
     * buffers, which store discrete messages, and stream buffers, which store a
     * continuous stream of bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the message. */
    ಯದಿ( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) != ( uint8_t ) 0 )
    {
        xBytesToStoreMessageLength = sbBYTES_TO_STORE_MESSAGE_LENGTH;
    }
    ಅನ್ಯಥಾ
    {
        xBytesToStoreMessageLength = 0;
    }

    /* True if the available space equals zero. */
    ಯದಿ( xStreamBufferSpacesAvailable( xStreamBuffer ) <= xBytesToStoreMessageLength )
    {
        xReturn = pdTRUE;
    }
    ಅನ್ಯಥಾ
    {
        xReturn = pdFALSE;
    }

    ಪ್ರತ್ಯಾಗಮ xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xStreamBufferSendCompletedFromISR( StreamBufferHandle_t xStreamBuffer,
                                              BaseType_t * pxHigherPriorityTaskWoken )
{
    StreamBuffer_t * ಧ್ರುವ pxStreamBuffer = xStreamBuffer;
    BaseType_t xReturn;
    UBaseType_t uxSavedInterruptStatus;

    configASSERT( pxStreamBuffer );

    uxSavedInterruptStatus = ( UBaseType_t ) portSET_INTERRUPT_MASK_FROM_ISR();
    {
        ಯದಿ( ( pxStreamBuffer )->xTaskWaitingToReceive != ರಿಕ್ತ )
        {
            ( ಶೂನ್ಯ ) xTaskNotifyFromISR( ( pxStreamBuffer )->xTaskWaitingToReceive,
                                         ( uint32_t ) 0,
                                         eNoAction,
                                         pxHigherPriorityTaskWoken );
            ( pxStreamBuffer )->xTaskWaitingToReceive = ರಿಕ್ತ;
            xReturn = pdTRUE;
        }
        ಅನ್ಯಥಾ
        {
            xReturn = pdFALSE;
        }
    }
    portCLEAR_INTERRUPT_MASK_FROM_ISR( uxSavedInterruptStatus );

    ಪ್ರತ್ಯಾಗಮ xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xStreamBufferReceiveCompletedFromISR( StreamBufferHandle_t xStreamBuffer,
                                                 BaseType_t * pxHigherPriorityTaskWoken )
{
    StreamBuffer_t * ಧ್ರುವ pxStreamBuffer = xStreamBuffer;
    BaseType_t xReturn;
    UBaseType_t uxSavedInterruptStatus;

    configASSERT( pxStreamBuffer );

    uxSavedInterruptStatus = ( UBaseType_t ) portSET_INTERRUPT_MASK_FROM_ISR();
    {
        ಯದಿ( ( pxStreamBuffer )->xTaskWaitingToSend != ರಿಕ್ತ )
        {
            ( ಶೂನ್ಯ ) xTaskNotifyFromISR( ( pxStreamBuffer )->xTaskWaitingToSend,
                                         ( uint32_t ) 0,
                                         eNoAction,
                                         pxHigherPriorityTaskWoken );
            ( pxStreamBuffer )->xTaskWaitingToSend = ರಿಕ್ತ;
            xReturn = pdTRUE;
        }
        ಅನ್ಯಥಾ
        {
            xReturn = pdFALSE;
        }
    }
    portCLEAR_INTERRUPT_MASK_FROM_ISR( uxSavedInterruptStatus );

    ಪ್ರತ್ಯಾಗಮ xReturn;
}
/*-----------------------------------------------------------*/

ಸ್ಥಿರ size_t prvWriteBytesToBuffer( StreamBuffer_t * ಧ್ರುವ pxStreamBuffer,
                                     ಧ್ರುವ uint8_t * pucData,
                                     size_t x_ಎಣಿಕೆ,
                                     size_t xHead )
{
    size_t xFirstLength;

    configASSERT( x_ಎಣಿಕೆ > ( size_t ) 0 );

    /* Calculate the number of bytes that can be added in the first write -
     * which may be less than the total number of bytes that need to be added if
     * the buffer will wrap back to the beginning. */
    xFirstLength = configMIN( pxStreamBuffer->xLength - xHead, x_ಎಣಿಕೆ );

    /* Write as many bytes as can be written in the first write. */
    configASSERT( ( xHead + xFirstLength ) <= pxStreamBuffer->xLength );
    ( ಶೂನ್ಯ ) memcpy( ( ಶೂನ್ಯ * ) ( &( pxStreamBuffer->pucBuffer[ xHead ] ) ), ( ಧ್ರುವ ಶೂನ್ಯ * ) pucData, xFirstLength ); /*lint !e9087 memcpy() requires void *. */

    /* If the number of bytes written was less than the number that could be
     * written in the first write... */
    ಯದಿ( x_ಎಣಿಕೆ > xFirstLength )
    {
        /* ...then write the remaining bytes to the start of the buffer. */
        configASSERT( ( x_ಎಣಿಕೆ - xFirstLength ) <= pxStreamBuffer->xLength );
        ( ಶೂನ್ಯ ) memcpy( ( ಶೂನ್ಯ * ) pxStreamBuffer->pucBuffer, ( ಧ್ರುವ ಶೂನ್ಯ * ) &( pucData[ xFirstLength ] ), x_ಎಣಿಕೆ - xFirstLength ); /*lint !e9087 memcpy() requires void *. */
    }
    ಅನ್ಯಥಾ
    {
        mtCOVERAGE_TEST_MARKER();
    }

    xHead += x_ಎಣಿಕೆ;

    ಯದಿ( xHead >= pxStreamBuffer->xLength )
    {
        xHead -= pxStreamBuffer->xLength;
    }
    ಅನ್ಯಥಾ
    {
        mtCOVERAGE_TEST_MARKER();
    }

    ಪ್ರತ್ಯಾಗಮ xHead;
}
/*-----------------------------------------------------------*/

ಸ್ಥಿರ size_t prvReadBytesFromBuffer( StreamBuffer_t * pxStreamBuffer,
                                      uint8_t * pucData,
                                      size_t x_ಎಣಿಕೆ,
                                      size_t xTail )
{
    size_t xFirstLength;

    configASSERT( x_ಎಣಿಕೆ != ( size_t ) 0 );

    /* Calculate the number of bytes that can be read - which may be
     * less than the number wanted if the data wraps around to the start of
     * the buffer. */
    xFirstLength = configMIN( pxStreamBuffer->xLength - xTail, x_ಎಣಿಕೆ );

    /* Obtain the number of bytes it is possible to obtain in the first
     * read.  Asserts check bounds of read and write. */
    configASSERT( xFirstLength <= x_ಎಣಿಕೆ );
    configASSERT( ( xTail + xFirstLength ) <= pxStreamBuffer->xLength );
    ( ಶೂನ್ಯ ) memcpy( ( ಶೂನ್ಯ * ) pucData, ( ಧ್ರುವ ಶೂನ್ಯ * ) &( pxStreamBuffer->pucBuffer[ xTail ] ), xFirstLength ); /*lint !e9087 memcpy() requires void *. */

    /* If the total number of wanted bytes is greater than the number
     * that could be read in the first read... */
    ಯದಿ( x_ಎಣಿಕೆ > xFirstLength )
    {
        /* ...then read the remaining bytes from the start of the buffer. */
        ( ಶೂನ್ಯ ) memcpy( ( ಶೂನ್ಯ * ) &( pucData[ xFirstLength ] ), ( ಶೂನ್ಯ * ) ( pxStreamBuffer->pucBuffer ), x_ಎಣಿಕೆ - xFirstLength ); /*lint !e9087 memcpy() requires void *. */
    }
    ಅನ್ಯಥಾ
    {
        mtCOVERAGE_TEST_MARKER();
    }

    /* Move the tail pointer to effectively remove the data read from the buffer. */
    xTail += x_ಎಣಿಕೆ;

    ಯದಿ( xTail >= pxStreamBuffer->xLength )
    {
        xTail -= pxStreamBuffer->xLength;
    }

    ಪ್ರತ್ಯಾಗಮ xTail;
}
/*-----------------------------------------------------------*/

ಸ್ಥಿರ size_t prvBytesInBuffer( ಧ್ರುವ StreamBuffer_t * ಧ್ರುವ pxStreamBuffer )
{
/* Returns the distance between xTail and xHead. */
    size_t x_ಎಣಿಕೆ;

    x_ಎಣಿಕೆ = pxStreamBuffer->xLength + pxStreamBuffer->xHead;
    x_ಎಣಿಕೆ -= pxStreamBuffer->xTail;

    ಯದಿ( x_ಎಣಿಕೆ >= pxStreamBuffer->xLength )
    {
        x_ಎಣಿಕೆ -= pxStreamBuffer->xLength;
    }
    ಅನ್ಯಥಾ
    {
        mtCOVERAGE_TEST_MARKER();
    }

    ಪ್ರತ್ಯಾಗಮ x_ಎಣಿಕೆ;
}
/*-----------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ prvInitialiseNewStreamBuffer( StreamBuffer_t * ಧ್ರುವ pxStreamBuffer,
                                          uint8_t * ಧ್ರುವ pucBuffer,
                                          size_t xBufferSizeBytes,
                                          size_t xTriggerLevelBytes,
                                          uint8_t ucFlags,
                                          StreamBufferCallbackFunction_t pxSendCompletedCallback,
                                          StreamBufferCallbackFunction_t pxReceiveCompletedCallback )
{
    /* Assert here is deliberately writing to the entire buffer to ensure it can
     * be written to without generating exceptions, and is setting the buffer to a
     * known value to assist in development/debugging. */
    #ಯದಿ ( configASSERT_DEFINED == 1 )
    {
        /* The value written just has to be identifiable when looking at the
         * memory.  Don't use 0xA5 as that is the stack fill value and could
         * result in confusion as to what is actually being observed. */
        ಧ್ರುವ BaseType_t x_Write_ಮೌಲ್ಯ = 0x55;
        configASSERT( memset( pucBuffer, ( ಪೂರ್ಣಾಙ್ಕ ) x_Write_ಮೌಲ್ಯ, xBufferSizeBytes ) == pucBuffer );
        ( ಶೂನ್ಯ ) x_Write_ಮೌಲ್ಯ;
    } /*lint !e529 !e438 xWriteValue is only used if configASSERT() is defined. */
    #endif

    /* This function could be called from xStreamBufferReset(), so we reset the
     * stream buffer fields manually in order to avoid clearing
     * xStreamBufferLock. The xStreamBufferLock is initialized separately on
     * stream buffer creation. */
    pxStreamBuffer->xTail = ( size_t ) 0;
    pxStreamBuffer->xHead = ( size_t ) 0;
    pxStreamBuffer->xTaskWaitingToReceive = ( TaskHandle_t ) 0;
    pxStreamBuffer->xTaskWaitingToSend = ( TaskHandle_t ) 0;
    #ಯದಿ ( configUSE_TRACE_FACILITY == 1 )
        pxStreamBuffer->ux_Stream_Buffer_ಸಂಖ್ಯೆ = ( UBaseType_t ) 0;
    #endif
    pxStreamBuffer->pucBuffer = pucBuffer;
    pxStreamBuffer->xLength = xBufferSizeBytes;
    pxStreamBuffer->xTriggerLevelBytes = xTriggerLevelBytes;
    pxStreamBuffer->ucFlags = ucFlags;
    #ಯದಿ ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    {
        pxStreamBuffer->pxSendCompletedCallback = pxSendCompletedCallback;
        pxStreamBuffer->pxReceiveCompletedCallback = pxReceiveCompletedCallback;
    }
    #ಅನ್ಯಥಾ
    {
        ( ಶೂನ್ಯ ) pxSendCompletedCallback;
        ( ಶೂನ್ಯ ) pxReceiveCompletedCallback;
    }
    #endif
}

#ಯದಿ ( configUSE_TRACE_FACILITY == 1 )

    UBaseType_t ux_Stream_Buffer_Get_Stream_Buffer_ಸಂಖ್ಯೆ( StreamBufferHandle_t xStreamBuffer )
    {
        ಪ್ರತ್ಯಾಗಮ xStreamBuffer->ux_Stream_Buffer_ಸಂಖ್ಯೆ;
    }

#endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#ಯದಿ ( configUSE_TRACE_FACILITY == 1 )

    ಶೂನ್ಯ v_Stream_Buffer_Set_Stream_Buffer_ಸಂಖ್ಯೆ( StreamBufferHandle_t xStreamBuffer,
                                             UBaseType_t ux_Stream_Buffer_ಸಂಖ್ಯೆ )
    {
        xStreamBuffer->ux_Stream_Buffer_ಸಂಖ್ಯೆ = ux_Stream_Buffer_ಸಂಖ್ಯೆ;
    }

#endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#ಯದಿ ( configUSE_TRACE_FACILITY == 1 )

    uint8_t ucStreamBufferGetStreamBufferType( StreamBufferHandle_t xStreamBuffer )
    {
        ಪ್ರತ್ಯಾಗಮ( xStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER );
    }

#endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/
