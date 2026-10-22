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
#அந்தர்பாவ <stdint.h>
#அந்தர்பாவ <string.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* FreeRTOS includes. */
#அந்தர்பாவ "FreeRTOS.h"
#அந்தர்பாவ "task.h"
#அந்தர்பாவ "stream_buffer.h"
/* Include private IDF API additions for critical thread safety macros */
#அந்தர்பாவ "esp_private/freertos_idf_additions_priv.h"

#எனில் ( configUSE_TASK_NOTIFICATIONS != 1 )
    #error configUSE_TASK_NOTIFICATIONS must be set to 1 to build stream_buffer.c
#endif

#எனில் ( INCLUDE_xTaskGetCurrentTaskHandle != 1 )
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
        எனில்( ( pxStreamBuffer )->xTaskWaitingToSend != வெற்று )                    \
        {                                                                       \
            ( வெறுமை ) xTaskNotify( ( pxStreamBuffer )->xTaskWaitingToSend,       \
                                  ( uint32_t ) 0,                               \
                                  eNoAction );                                  \
            ( pxStreamBuffer )->xTaskWaitingToSend = வெற்று;                      \
        }                                                                       \
    }                                                                           \
    ( வெறுமை ) prvEXIT_CRITICAL_OR_RESUME_ALL( &( pxStreamBuffer->xStreamBufferLock ) );
#endif /* sbRECEIVE_COMPLETED */

/* If user has provided a per-instance receive complete callback, then
 * invoke the callback else use the receive complete macro which is provided by default for all instances.
 */
#எனில் ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #define prvRECEIVE_COMPLETED( pxStreamBuffer )                                               \
    {                                                                                            \
        எனில்( ( pxStreamBuffer )->pxReceiveCompletedCallback != வெற்று )                             \
        {                                                                                        \
            ( pxStreamBuffer )->pxReceiveCompletedCallback( ( pxStreamBuffer ), pdFALSE, வெற்று ); \
        }                                                                                        \
        இல்லையெனில்                                                                                     \
        {                                                                                        \
            sbRECEIVE_COMPLETED( ( pxStreamBuffer ) );                                           \
        }                                                                                        \
    }
#இல்லையெனில் /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
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
            எனில்( ( pxStreamBuffer )->xTaskWaitingToSend != வெற்று )                     \
            {                                                                        \
                ( வெறுமை ) xTaskNotifyFromISR( ( pxStreamBuffer )->xTaskWaitingToSend, \
                                             ( uint32_t ) 0,                         \
                                             eNoAction,                              \
                                             ( pxHigherPriorityTaskWoken ) );        \
                ( pxStreamBuffer )->xTaskWaitingToSend = வெற்று;                       \
            }                                                                        \
        }                                                                            \
        portCLEAR_INTERRUPT_MASK_FROM_ISR( uxSavedInterruptStatus );                 \
    }
#endif /* sbRECEIVE_COMPLETED_FROM_ISR */

#எனில் ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #define prvRECEIVE_COMPLETED_FROM_ISR( pxStreamBuffer,                                                               \
                                           pxHigherPriorityTaskWoken )                                                   \
    {                                                                                                                    \
        எனில்( ( pxStreamBuffer )->pxReceiveCompletedCallback != வெற்று )                                                     \
        {                                                                                                                \
            ( pxStreamBuffer )->pxReceiveCompletedCallback( ( pxStreamBuffer ), pdTRUE, ( pxHigherPriorityTaskWoken ) ); \
        }                                                                                                                \
        இல்லையெனில்                                                                                                             \
        {                                                                                                                \
            sbRECEIVE_COMPLETED_FROM_ISR( ( pxStreamBuffer ), ( pxHigherPriorityTaskWoken ) );                           \
        }                                                                                                                \
    }
#இல்லையெனில் /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
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
        எனில்( ( pxStreamBuffer )->xTaskWaitingToReceive != வெற்று )                 \
        {                                                                       \
            ( வெறுமை ) xTaskNotify( ( pxStreamBuffer )->xTaskWaitingToReceive,    \
                                  ( uint32_t ) 0,                               \
                                  eNoAction );                                  \
            ( pxStreamBuffer )->xTaskWaitingToReceive = வெற்று;                   \
        }                                                                       \
    }                                                                           \
    ( வெறுமை ) prvEXIT_CRITICAL_OR_RESUME_ALL( &( pxStreamBuffer->xStreamBufferLock ) );
#endif /* sbSEND_COMPLETED */

/* If user has provided a per-instance send completed callback, then
 * invoke the callback else use the send complete macro which is provided by default for all instances.
 */
#எனில் ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #define prvSEND_COMPLETED( pxStreamBuffer )                                           \
    {                                                                                     \
        எனில்( ( pxStreamBuffer )->pxSendCompletedCallback != வெற்று )                         \
        {                                                                                 \
            pxStreamBuffer->pxSendCompletedCallback( ( pxStreamBuffer ), pdFALSE, வெற்று ); \
        }                                                                                 \
        இல்லையெனில்                                                                              \
        {                                                                                 \
            sbSEND_COMPLETED( ( pxStreamBuffer ) );                                       \
        }                                                                                 \
    }
#இல்லையெனில் /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #define prvSEND_COMPLETED( pxStreamBuffer )    sbSEND_COMPLETED( ( pxStreamBuffer ) )
#endif /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */


#ifndef sbSEND_COMPLETE_FROM_ISR
    #define sbSEND_COMPLETE_FROM_ISR( pxStreamBuffer, pxHigherPriorityTaskWoken )       \
    {                                                                                   \
        UBaseType_t uxSavedInterruptStatus;                                             \
                                                                                        \
        uxSavedInterruptStatus = ( UBaseType_t ) portSET_INTERRUPT_MASK_FROM_ISR();     \
        {                                                                               \
            எனில்( ( pxStreamBuffer )->xTaskWaitingToReceive != வெற்று )                     \
            {                                                                           \
                ( வெறுமை ) xTaskNotifyFromISR( ( pxStreamBuffer )->xTaskWaitingToReceive, \
                                             ( uint32_t ) 0,                            \
                                             eNoAction,                                 \
                                             ( pxHigherPriorityTaskWoken ) );           \
                ( pxStreamBuffer )->xTaskWaitingToReceive = வெற்று;                       \
            }                                                                           \
        }                                                                               \
        portCLEAR_INTERRUPT_MASK_FROM_ISR( uxSavedInterruptStatus );                    \
    }
#endif /* sbSEND_COMPLETE_FROM_ISR */


#எனில் ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #define prvSEND_COMPLETE_FROM_ISR( pxStreamBuffer, pxHigherPriorityTaskWoken )                                    \
    {                                                                                                                 \
        எனில்( ( pxStreamBuffer )->pxSendCompletedCallback != வெற்று )                                                     \
        {                                                                                                             \
            ( pxStreamBuffer )->pxSendCompletedCallback( ( pxStreamBuffer ), pdTRUE, ( pxHigherPriorityTaskWoken ) ); \
        }                                                                                                             \
        இல்லையெனில்                                                                                                          \
        {                                                                                                             \
            sbSEND_COMPLETE_FROM_ISR( ( pxStreamBuffer ), ( pxHigherPriorityTaskWoken ) );                            \
        }                                                                                                             \
    }
#இல்லையெனில் /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #define prvSEND_COMPLETE_FROM_ISR( pxStreamBuffer, pxHigherPriorityTaskWoken ) \
    sbSEND_COMPLETE_FROM_ISR( ( pxStreamBuffer ), ( pxHigherPriorityTaskWoken ) )
#endif /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

/*lint -restore (9026) */

/* The number of bytes used to hold the length of a message in the buffer. */
#define sbBYTES_TO_STORE_MESSAGE_LENGTH    ( அளவு( configMESSAGE_BUFFER_LENGTH_TYPE ) )

/* Bits stored in the ucFlags field of the stream buffer. */
#define sbFLAGS_IS_MESSAGE_BUFFER          ( ( uint8_t ) 1 ) /* Set if the stream buffer was created as a message buffer, in which case it holds discrete messages rather than a stream. */
#define sbFLAGS_IS_STATICALLY_ALLOCATED    ( ( uint8_t ) 2 ) /* Set if the stream buffer was created using statically allocated memory. */

/*-----------------------------------------------------------*/

/* Structure that hold state information on the buffer. */
வகைவரை கட்டமைப்பு StreamBufferDef_t                 /*lint !e9058 Style convention uses tag. */
{
    நிலையற்ற size_t xTail;                       /* Index to the next item to read within the buffer. */
    நிலையற்ற size_t xHead;                       /* Index to the next item to write within the buffer. */
    size_t xLength;                              /* The length of the buffer pointed to by pucBuffer. */
    size_t xTriggerLevelBytes;                   /* The number of bytes that must be in the stream buffer before a task that is waiting for data is unblocked. */
    நிலையற்ற TaskHandle_t xTaskWaitingToReceive; /* Holds the handle of a task waiting for data, or NULL if no tasks are waiting. */
    நிலையற்ற TaskHandle_t xTaskWaitingToSend;    /* Holds the handle of a task waiting to send data to a message buffer that is full. */
    uint8_t * pucBuffer;                         /* Points to the buffer itself - that is - the RAM that stores the data passed through the buffer. */
    uint8_t ucFlags;

    #எனில் ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_Stream_Buffer_எண்; /* Used for tracing purposes. */
    #endif

    #எனில் ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        StreamBufferCallbackFunction_t pxSendCompletedCallback;    /* Optional callback called on send complete. sbSEND_COMPLETED is called if this is NULL. */
        StreamBufferCallbackFunction_t pxReceiveCompletedCallback; /* Optional callback called on receive complete.  sbRECEIVE_COMPLETED is called if this is NULL. */
    #endif

    portMUX_TYPE xStreamBufferLock; /* Spinlock required for SMP critical sections */
} StreamBuffer_t;

/*
 * The number of bytes available to be read from the buffer.
 */
நிலையான size_t prvBytesInBuffer( மாறிலி StreamBuffer_t * மாறிலி pxStreamBuffer ) PRIVILEGED_FUNCTION;

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
நிலையான size_t prvWriteBytesToBuffer( StreamBuffer_t * மாறிலி pxStreamBuffer,
                                     மாறிலி uint8_t * pucData,
                                     size_t x_மொத்தம்,
                                     size_t xHead ) PRIVILEGED_FUNCTION;

/*
 * If the stream buffer is being used as a message buffer, then reads an entire
 * message out of the buffer.  If the stream buffer is being used as a stream
 * buffer then read as many bytes as possible from the buffer.
 * prvReadBytesFromBuffer() is called to actually extract the bytes from the
 * buffer's data storage area.
 */
நிலையான size_t prvReadMessageFromBuffer( StreamBuffer_t * pxStreamBuffer,
                                        வெறுமை * pvRxData,
                                        size_t xBufferLengthBytes,
                                        size_t xBytesAvailable ) PRIVILEGED_FUNCTION;

/*
 * If the stream buffer is being used as a message buffer, then writes an entire
 * message to the buffer.  If the stream buffer is being used as a stream
 * buffer then write as many bytes as possible to the buffer.
 * prvWriteBytestoBuffer() is called to actually send the bytes to the buffer's
 * data storage area.
 */
நிலையான size_t prvWriteMessageToBuffer( StreamBuffer_t * மாறிலி pxStreamBuffer,
                                       மாறிலி வெறுமை * pvTxData,
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
நிலையான size_t prvReadBytesFromBuffer( StreamBuffer_t * pxStreamBuffer,
                                      uint8_t * pucData,
                                      size_t x_மொத்தம்,
                                      size_t xTail ) PRIVILEGED_FUNCTION;

/*
 * Called by both pxStreamBufferCreate() and pxStreamBufferCreateStatic() to
 * initialise the members of the newly created stream buffer structure.
 */
நிலையான வெறுமை prvInitialiseNewStreamBuffer( StreamBuffer_t * மாறிலி pxStreamBuffer,
                                          uint8_t * மாறிலி pucBuffer,
                                          size_t xBufferSizeBytes,
                                          size_t xTriggerLevelBytes,
                                          uint8_t ucFlags,
                                          StreamBufferCallbackFunction_t pxSendCompletedCallback,
                                          StreamBufferCallbackFunction_t pxReceiveCompletedCallback ) PRIVILEGED_FUNCTION;

/*-----------------------------------------------------------*/
#எனில் ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
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
        எனில்( xIsMessageBuffer == pdTRUE )
        {
            /* Is a message buffer but not statically allocated. */
            ucFlags = sbFLAGS_IS_MESSAGE_BUFFER;
            configASSERT( xBufferSizeBytes > sbBYTES_TO_STORE_MESSAGE_LENGTH );
        }
        இல்லையெனில்
        {
            /* Not a message buffer and not statically allocated. */
            ucFlags = 0;
            configASSERT( xBufferSizeBytes > 0 );
        }

        configASSERT( xTriggerLevelBytes <= xBufferSizeBytes );

        /* A trigger level of 0 would cause a waiting task to unblock even when
         * the buffer was empty. */
        எனில்( xTriggerLevelBytes == ( size_t ) 0 )
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
        எனில்( xBufferSizeBytes < ( xBufferSizeBytes + 1 + அளவு( StreamBuffer_t ) ) )
        {
            xBufferSizeBytes++;
            pucAllocatedMemory = ( uint8_t * ) pvPortMalloc( xBufferSizeBytes + அளவு( StreamBuffer_t ) ); /*lint !e9079 malloc() only returns void*. */
        }
        இல்லையெனில்
        {
            pucAllocatedMemory = வெற்று;
        }

        எனில்( pucAllocatedMemory != வெற்று )
        {
            prvInitialiseNewStreamBuffer( ( StreamBuffer_t * ) pucAllocatedMemory,       /* Structure at the start of the allocated memory. */ /*lint !e9087 Safe cast as allocated memory is aligned. */ /*lint !e826 Area is not too small and alignment is guaranteed provided malloc() behaves as expected and returns aligned buffer. */
                                          pucAllocatedMemory + அளவு( StreamBuffer_t ), /* Storage area follows. */ /*lint !e9016 Indexing past structure valid for uint8_t pointer, also storage area has no alignment requirement. */
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
        இல்லையெனில்
        {
            traceSTREAM_BUFFER_CREATE_FAILED( xIsMessageBuffer );
        }

        திரும்பு ( StreamBufferHandle_t ) pucAllocatedMemory; /*lint !e9087 !e826 Safe cast as allocated memory is aligned. */
    }
#endif /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

#எனில் ( configSUPPORT_STATIC_ALLOCATION == 1 )

    StreamBufferHandle_t xStreamBufferGenericCreateStatic( size_t xBufferSizeBytes,
                                                           size_t xTriggerLevelBytes,
                                                           BaseType_t xIsMessageBuffer,
                                                           uint8_t * மாறிலி pucStreamBufferStorageArea,
                                                           StaticStreamBuffer_t * மாறிலி pxStaticStreamBuffer,
                                                           StreamBufferCallbackFunction_t pxSendCompletedCallback,
                                                           StreamBufferCallbackFunction_t pxReceiveCompletedCallback )
    {
        StreamBuffer_t * மாறிலி pxStreamBuffer = ( StreamBuffer_t * ) pxStaticStreamBuffer; /*lint !e740 !e9087 Safe cast as StaticStreamBuffer_t is opaque Streambuffer_t. */
        StreamBufferHandle_t xReturn;
        uint8_t ucFlags;

        configASSERT( pucStreamBufferStorageArea );
        configASSERT( pxStaticStreamBuffer );
        configASSERT( xTriggerLevelBytes <= xBufferSizeBytes );

        /* A trigger level of 0 would cause a waiting task to unblock even when
         * the buffer was empty. */
        எனில்( xTriggerLevelBytes == ( size_t ) 0 )
        {
            xTriggerLevelBytes = ( size_t ) 1;
        }

        எனில்( xIsMessageBuffer != pdFALSE )
        {
            /* Statically allocated message buffer. */
            ucFlags = sbFLAGS_IS_MESSAGE_BUFFER | sbFLAGS_IS_STATICALLY_ALLOCATED;
        }
        இல்லையெனில்
        {
            /* Statically allocated stream buffer. */
            ucFlags = sbFLAGS_IS_STATICALLY_ALLOCATED;
        }

        /* In case the stream buffer is going to be used as a message buffer
         * (that is, it will hold discrete messages with a little meta data that
         * says how big the next message is) check the buffer will be large enough
         * to hold at least one message. */
        configASSERT( xBufferSizeBytes > sbBYTES_TO_STORE_MESSAGE_LENGTH );

        #எனில் ( configASSERT_DEFINED == 1 )
        {
            /* Sanity check that the size of the structure used to declare a
             * variable of type StaticStreamBuffer_t equals the size of the real
             * message buffer structure. */
            நிலையற்ற size_t xSize = அளவு( StaticStreamBuffer_t );
            configASSERT( xSize == அளவு( StreamBuffer_t ) );
            ( வெறுமை ) xSize; /* Prevent unused variable warning when configASSERT() is not used. */
        } /*lint !e529 xSize is referenced is configASSERT() is defined. */
        #endif /* configASSERT_DEFINED */

        எனில்( ( pucStreamBufferStorageArea != வெற்று ) && ( pxStaticStreamBuffer != வெற்று ) )
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
        இல்லையெனில்
        {
            xReturn = வெற்று;
            traceSTREAM_BUFFER_CREATE_STATIC_FAILED( xReturn, xIsMessageBuffer );
        }

        திரும்பு xReturn;
    }
#endif /* ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*-----------------------------------------------------------*/

#எனில் ( configSUPPORT_STATIC_ALLOCATION == 1 )
    BaseType_t xStreamBufferGetStaticBuffers( StreamBufferHandle_t xStreamBuffer,
                                              uint8_t ** ppucStreamBufferStorageArea,
                                              StaticStreamBuffer_t ** ppxStaticStreamBuffer )
    {
        BaseType_t xReturn;
        மாறிலி StreamBuffer_t * மாறிலி pxStreamBuffer = xStreamBuffer;

        configASSERT( pxStreamBuffer );
        configASSERT( ppucStreamBufferStorageArea );
        configASSERT( ppxStaticStreamBuffer );

        எனில்( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_STATICALLY_ALLOCATED ) != ( uint8_t ) 0 )
        {
            *ppucStreamBufferStorageArea = pxStreamBuffer->pucBuffer;
            *ppxStaticStreamBuffer = ( StaticStreamBuffer_t * ) pxStreamBuffer;
            xReturn = pdTRUE;
        }
        இல்லையெனில்
        {
            xReturn = pdFALSE;
        }

        திரும்பு xReturn;
    }
#endif /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

வெறுமை vStreamBufferDelete( StreamBufferHandle_t xStreamBuffer )
{
    StreamBuffer_t * pxStreamBuffer = xStreamBuffer;

    configASSERT( pxStreamBuffer );

    traceSTREAM_BUFFER_DELETE( xStreamBuffer );

    எனில்( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_STATICALLY_ALLOCATED ) == ( uint8_t ) pdFALSE )
    {
        #எனில் ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
        {
            /* Both the structure and the buffer were allocated using a single call
            * to pvPortMalloc(), hence only one call to vPortFree() is required. */
            vPortFree( ( வெறுமை * ) pxStreamBuffer ); /*lint !e9087 Standard free() semantics require void *, plus pxStreamBuffer was allocated by pvPortMalloc(). */
        }
        #இல்லையெனில்
        {
            /* Should not be possible to get here, ucFlags must be corrupt.
             * Force an assert. */
            configASSERT( xStreamBuffer == ( StreamBufferHandle_t ) ~0 );
        }
        #endif
    }
    இல்லையெனில்
    {
        /* The structure and buffer were not allocated dynamically and cannot be
         * freed - just scrub the structure so future use will assert. */
        ( வெறுமை ) memset( pxStreamBuffer, 0x00, அளவு( StreamBuffer_t ) );
    }
}
/*-----------------------------------------------------------*/

BaseType_t xStreamBufferReset( StreamBufferHandle_t xStreamBuffer )
{
    StreamBuffer_t * மாறிலி pxStreamBuffer = xStreamBuffer;
    BaseType_t xReturn = pdFAIL;
    StreamBufferCallbackFunction_t pxSendCallback = வெற்று, pxReceiveCallback = வெற்று;

    #எனில் ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_Stream_Buffer_எண்;
    #endif

    configASSERT( pxStreamBuffer );

    #எனில் ( configUSE_TRACE_FACILITY == 1 )
    {
        /* Store the stream buffer number so it can be restored after the
         * reset. */
        ux_Stream_Buffer_எண் = pxStreamBuffer->ux_Stream_Buffer_எண்;
    }
    #endif

    /* Can only reset a message buffer if there are no tasks blocked on it. */
    taskENTER_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );
    {
        எனில்( ( pxStreamBuffer->xTaskWaitingToReceive == வெற்று ) && ( pxStreamBuffer->xTaskWaitingToSend == வெற்று ) )
        {
            #எனில் ( configUSE_SB_COMPLETED_CALLBACK == 1 )
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

            #எனில் ( configUSE_TRACE_FACILITY == 1 )
            {
                pxStreamBuffer->ux_Stream_Buffer_எண் = ux_Stream_Buffer_எண்;
            }
            #endif

            traceSTREAM_BUFFER_RESET( xStreamBuffer );

            xReturn = pdPASS;
        }
    }
    taskEXIT_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );

    திரும்பு xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xStreamBufferSetTriggerLevel( StreamBufferHandle_t xStreamBuffer,
                                         size_t xTriggerLevel )
{
    StreamBuffer_t * மாறிலி pxStreamBuffer = xStreamBuffer;
    BaseType_t xReturn;

    configASSERT( pxStreamBuffer );

    /* It is not valid for the trigger level to be 0. */
    எனில்( xTriggerLevel == ( size_t ) 0 )
    {
        xTriggerLevel = ( size_t ) 1;
    }

    /* The trigger level is the number of bytes that must be in the stream
     * buffer before a task that is waiting for data is unblocked. */
    எனில்( xTriggerLevel < pxStreamBuffer->xLength )
    {
        pxStreamBuffer->xTriggerLevelBytes = xTriggerLevel;
        xReturn = pdPASS;
    }
    இல்லையெனில்
    {
        xReturn = pdFALSE;
    }

    திரும்பு xReturn;
}
/*-----------------------------------------------------------*/

size_t xStreamBufferSpacesAvailable( StreamBufferHandle_t xStreamBuffer )
{
    மாறிலி StreamBuffer_t * மாறிலி pxStreamBuffer = xStreamBuffer;
    size_t xSpace;
    size_t xOriginalTail;

    configASSERT( pxStreamBuffer );

    /* The code below reads xTail and then xHead.  This is safe if the stream
     * buffer is updated once between the two reads - but not if the stream buffer
     * is updated more than once between the two reads - hence the loop. */
    செய்
    {
        xOriginalTail = pxStreamBuffer->xTail;
        xSpace = pxStreamBuffer->xLength + pxStreamBuffer->xTail;
        xSpace -= pxStreamBuffer->xHead;
    } போது( xOriginalTail != pxStreamBuffer->xTail );

    xSpace -= ( size_t ) 1;

    எனில்( xSpace >= pxStreamBuffer->xLength )
    {
        xSpace -= pxStreamBuffer->xLength;
    }
    இல்லையெனில்
    {
        mtCOVERAGE_TEST_MARKER();
    }

    திரும்பு xSpace;
}
/*-----------------------------------------------------------*/

size_t xStreamBufferBytesAvailable( StreamBufferHandle_t xStreamBuffer )
{
    மாறிலி StreamBuffer_t * மாறிலி pxStreamBuffer = xStreamBuffer;
    size_t xReturn;

    configASSERT( pxStreamBuffer );

    xReturn = prvBytesInBuffer( pxStreamBuffer );
    திரும்பு xReturn;
}
/*-----------------------------------------------------------*/

size_t xStreamBufferSend( StreamBufferHandle_t xStreamBuffer,
                          மாறிலி வெறுமை * pvTxData,
                          size_t xDataLengthBytes,
                          TickType_t xTicksToWait )
{
    StreamBuffer_t * மாறிலி pxStreamBuffer = xStreamBuffer;
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
    எனில்( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) != ( uint8_t ) 0 )
    {
        xRequiredSpace += sbBYTES_TO_STORE_MESSAGE_LENGTH;

        /* Overflow? */
        configASSERT( xRequiredSpace > xDataLengthBytes );

        /* If this is a message buffer then it must be possible to write the
         * whole message. */
        எனில்( xRequiredSpace > xMaxReportedSpace )
        {
            /* The message would not fit even if the entire buffer was empty,
             * so don't wait for space. */
            xTicksToWait = ( TickType_t ) 0;
        }
        இல்லையெனில்
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    இல்லையெனில்
    {
        /* If this is a stream buffer then it is acceptable to write only part
         * of the message to the buffer.  Cap the length to the total length of
         * the buffer. */
        எனில்( xRequiredSpace > xMaxReportedSpace )
        {
            xRequiredSpace = xMaxReportedSpace;
        }
        இல்லையெனில்
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }

    எனில்( xTicksToWait != ( TickType_t ) 0 )
    {
        vTaskSetTimeOutState( &xTimeOut );

        செய்
        {
            /* Wait until the required number of bytes are free in the message
             * buffer. */
            taskENTER_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );
            {
                xSpace = xStreamBufferSpacesAvailable( pxStreamBuffer );

                எனில்( xSpace < xRequiredSpace )
                {
                    /* Clear notification state as going to wait for space. */
                    ( வெறுமை ) xTaskNotifyStateClear( வெற்று );

                    /* Should only be one writer. */
                    configASSERT( pxStreamBuffer->xTaskWaitingToSend == வெற்று );
                    pxStreamBuffer->xTaskWaitingToSend = xTaskGetCurrentTaskHandle();
                }
                இல்லையெனில்
                {
                    taskEXIT_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );
                    நிறுத்து;
                }
            }
            taskEXIT_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );

            traceBLOCKING_ON_STREAM_BUFFER_SEND( xStreamBuffer );
            ( வெறுமை ) xTaskNotifyWait( ( uint32_t ) 0, ( uint32_t ) 0, வெற்று, xTicksToWait );
            /* In SMP mode, the task may have been woken and scheduled on
             * another core. Hence, we must clear the xTaskWaitingToSend
             * handle in a critical section. */
            #எனில் ( config_எண்_OF_CORES > 1 )
                taskENTER_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );
            #endif /* configNUMBER_OF_CORES > 1 */
            {
                pxStreamBuffer->xTaskWaitingToSend = வெற்று;
            }
            #எனில் ( config_எண்_OF_CORES > 1 )
                taskEXIT_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );
            #endif /* configNUMBER_OF_CORES > 1 */
        } போது( xTaskCheckForTimeOut( &xTimeOut, &xTicksToWait ) == pdFALSE );
    }
    இல்லையெனில்
    {
        mtCOVERAGE_TEST_MARKER();
    }

    எனில்( xSpace == ( size_t ) 0 )
    {
        xSpace = xStreamBufferSpacesAvailable( pxStreamBuffer );
    }
    இல்லையெனில்
    {
        mtCOVERAGE_TEST_MARKER();
    }

    xReturn = prvWriteMessageToBuffer( pxStreamBuffer, pvTxData, xDataLengthBytes, xSpace, xRequiredSpace );

    எனில்( xReturn > ( size_t ) 0 )
    {
        traceSTREAM_BUFFER_SEND( xStreamBuffer, xReturn );

        /* Was a task waiting for the data? */
        எனில்( prvBytesInBuffer( pxStreamBuffer ) >= pxStreamBuffer->xTriggerLevelBytes )
        {
            prvSEND_COMPLETED( pxStreamBuffer );
        }
        இல்லையெனில்
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    இல்லையெனில்
    {
        mtCOVERAGE_TEST_MARKER();
        traceSTREAM_BUFFER_SEND_FAILED( xStreamBuffer );
    }

    திரும்பு xReturn;
}
/*-----------------------------------------------------------*/

size_t xStreamBufferSendFromISR( StreamBufferHandle_t xStreamBuffer,
                                 மாறிலி வெறுமை * pvTxData,
                                 size_t xDataLengthBytes,
                                 BaseType_t * மாறிலி pxHigherPriorityTaskWoken )
{
    StreamBuffer_t * மாறிலி pxStreamBuffer = xStreamBuffer;
    size_t xReturn, xSpace;
    size_t xRequiredSpace = xDataLengthBytes;

    configASSERT( pvTxData );
    configASSERT( pxStreamBuffer );

    /* This send function is used to write to both message buffers and stream
     * buffers.  If this is a message buffer then the space needed must be
     * increased by the amount of bytes needed to store the length of the
     * message. */
    எனில்( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) != ( uint8_t ) 0 )
    {
        xRequiredSpace += sbBYTES_TO_STORE_MESSAGE_LENGTH;
    }
    இல்லையெனில்
    {
        mtCOVERAGE_TEST_MARKER();
    }

    xSpace = xStreamBufferSpacesAvailable( pxStreamBuffer );
    xReturn = prvWriteMessageToBuffer( pxStreamBuffer, pvTxData, xDataLengthBytes, xSpace, xRequiredSpace );

    எனில்( xReturn > ( size_t ) 0 )
    {
        /* Was a task waiting for the data? */
        எனில்( prvBytesInBuffer( pxStreamBuffer ) >= pxStreamBuffer->xTriggerLevelBytes )
        {
            prvSEND_COMPLETE_FROM_ISR( pxStreamBuffer, pxHigherPriorityTaskWoken );
        }
        இல்லையெனில்
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    இல்லையெனில்
    {
        mtCOVERAGE_TEST_MARKER();
    }

    traceSTREAM_BUFFER_SEND_FROM_ISR( xStreamBuffer, xReturn );

    திரும்பு xReturn;
}
/*-----------------------------------------------------------*/

நிலையான size_t prvWriteMessageToBuffer( StreamBuffer_t * மாறிலி pxStreamBuffer,
                                       மாறிலி வெறுமை * pvTxData,
                                       size_t xDataLengthBytes,
                                       size_t xSpace,
                                       size_t xRequiredSpace )
{
    size_t xNextHead = pxStreamBuffer->xHead;
    configMESSAGE_BUFFER_LENGTH_TYPE xMessageLength;

    எனில்( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) != ( uint8_t ) 0 )
    {
        /* This is a message buffer, as opposed to a stream buffer. */

        /* Convert xDataLengthBytes to the message length type. */
        xMessageLength = ( configMESSAGE_BUFFER_LENGTH_TYPE ) xDataLengthBytes;

        /* Ensure the data length given fits within configMESSAGE_BUFFER_LENGTH_TYPE. */
        configASSERT( ( size_t ) xMessageLength == xDataLengthBytes );

        எனில்( xSpace >= xRequiredSpace )
        {
            /* There is enough space to write both the message length and the message
             * itself into the buffer.  Start by writing the length of the data, the data
             * itself will be written later in this function. */
            xNextHead = prvWriteBytesToBuffer( pxStreamBuffer, ( மாறிலி uint8_t * ) &( xMessageLength ), sbBYTES_TO_STORE_MESSAGE_LENGTH, xNextHead );
        }
        இல்லையெனில்
        {
            /* Not enough space, so do not write data to the buffer. */
            xDataLengthBytes = 0;
        }
    }
    இல்லையெனில்
    {
        /* This is a stream buffer, as opposed to a message buffer, so writing a
         * stream of bytes rather than discrete messages.  Plan to write as many
         * bytes as possible. */
        xDataLengthBytes = configMIN( xDataLengthBytes, xSpace );
    }

    எனில்( xDataLengthBytes != ( size_t ) 0 )
    {
        /* Write the data to the buffer. */
        pxStreamBuffer->xHead = prvWriteBytesToBuffer( pxStreamBuffer, ( மாறிலி uint8_t * ) pvTxData, xDataLengthBytes, xNextHead ); /*lint !e9079 Storage buffer is implemented as uint8_t for ease of sizing, alignment and access. */
    }

    திரும்பு xDataLengthBytes;
}
/*-----------------------------------------------------------*/

size_t xStreamBufferReceive( StreamBufferHandle_t xStreamBuffer,
                             வெறுமை * pvRxData,
                             size_t xBufferLengthBytes,
                             TickType_t xTicksToWait )
{
    StreamBuffer_t * மாறிலி pxStreamBuffer = xStreamBuffer;
    size_t xReceivedLength = 0, xBytesAvailable, xBytesToStoreMessageLength;

    configASSERT( pvRxData );
    configASSERT( pxStreamBuffer );

    /* This receive function is used by both message buffers, which store
     * discrete messages, and stream buffers, which store a continuous stream of
     * bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the
     * message. */
    எனில்( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) != ( uint8_t ) 0 )
    {
        xBytesToStoreMessageLength = sbBYTES_TO_STORE_MESSAGE_LENGTH;
    }
    இல்லையெனில்
    {
        xBytesToStoreMessageLength = 0;
    }

    எனில்( xTicksToWait != ( TickType_t ) 0 )
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
            எனில்( xBytesAvailable <= xBytesToStoreMessageLength )
            {
                /* Clear notification state as going to wait for data. */
                ( வெறுமை ) xTaskNotifyStateClear( வெற்று );

                /* Should only be one reader. */
                configASSERT( pxStreamBuffer->xTaskWaitingToReceive == வெற்று );
                pxStreamBuffer->xTaskWaitingToReceive = xTaskGetCurrentTaskHandle();
            }
            இல்லையெனில்
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        taskEXIT_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );

        எனில்( xBytesAvailable <= xBytesToStoreMessageLength )
        {
            /* Wait for data to be available. */
            traceBLOCKING_ON_STREAM_BUFFER_RECEIVE( xStreamBuffer );
            ( வெறுமை ) xTaskNotifyWait( ( uint32_t ) 0, ( uint32_t ) 0, வெற்று, xTicksToWait );

            /* In SMP mode, the task may have been woken and scheduled on
             * another core. Hence, we must clear the xTaskWaitingToReceive
             * handle in a critical section. */
            #எனில் ( config_எண்_OF_CORES > 1 )
                taskENTER_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );
            #endif /* configNUMBER_OF_CORES > 1 */
            {
                pxStreamBuffer->xTaskWaitingToReceive = வெற்று;
            }
            #எனில் ( config_எண்_OF_CORES > 1 )
                taskEXIT_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );
            #endif /* configNUMBER_OF_CORES > 1 */

            /* Recheck the data available after blocking. */
            xBytesAvailable = prvBytesInBuffer( pxStreamBuffer );
        }
        இல்லையெனில்
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    இல்லையெனில்
    {
        xBytesAvailable = prvBytesInBuffer( pxStreamBuffer );
    }

    /* Whether receiving a discrete message (where xBytesToStoreMessageLength
     * holds the number of bytes used to store the message length) or a stream of
     * bytes (where xBytesToStoreMessageLength is zero), the number of bytes
     * available must be greater than xBytesToStoreMessageLength to be able to
     * read bytes from the buffer. */
    எனில்( xBytesAvailable > xBytesToStoreMessageLength )
    {
        xReceivedLength = prvReadMessageFromBuffer( pxStreamBuffer, pvRxData, xBufferLengthBytes, xBytesAvailable );

        /* Was a task waiting for space in the buffer? */
        எனில்( xReceivedLength != ( size_t ) 0 )
        {
            traceSTREAM_BUFFER_RECEIVE( xStreamBuffer, xReceivedLength );
            prvRECEIVE_COMPLETED( xStreamBuffer );
        }
        இல்லையெனில்
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    இல்லையெனில்
    {
        traceSTREAM_BUFFER_RECEIVE_FAILED( xStreamBuffer );
        mtCOVERAGE_TEST_MARKER();
    }

    திரும்பு xReceivedLength;
}
/*-----------------------------------------------------------*/

size_t xStreamBufferNextMessageLengthBytes( StreamBufferHandle_t xStreamBuffer )
{
    StreamBuffer_t * மாறிலி pxStreamBuffer = xStreamBuffer;
    size_t xReturn, xBytesAvailable;
    configMESSAGE_BUFFER_LENGTH_TYPE xTempReturn;

    configASSERT( pxStreamBuffer );

    /* Ensure the stream buffer is being used as a message buffer. */
    எனில்( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) != ( uint8_t ) 0 )
    {
        xBytesAvailable = prvBytesInBuffer( pxStreamBuffer );

        எனில்( xBytesAvailable > sbBYTES_TO_STORE_MESSAGE_LENGTH )
        {
            /* The number of bytes available is greater than the number of bytes
             * required to hold the length of the next message, so another message
             * is available. */
            ( வெறுமை ) prvReadBytesFromBuffer( pxStreamBuffer, ( uint8_t * ) &xTempReturn, sbBYTES_TO_STORE_MESSAGE_LENGTH, pxStreamBuffer->xTail );
            xReturn = ( size_t ) xTempReturn;
        }
        இல்லையெனில்
        {
            /* The minimum amount of bytes in a message buffer is
             * ( sbBYTES_TO_STORE_MESSAGE_LENGTH + 1 ), so if xBytesAvailable is
             * less than sbBYTES_TO_STORE_MESSAGE_LENGTH the only other valid
             * value is 0. */
            configASSERT( xBytesAvailable == 0 );
            xReturn = 0;
        }
    }
    இல்லையெனில்
    {
        xReturn = 0;
    }

    திரும்பு xReturn;
}
/*-----------------------------------------------------------*/

size_t xStreamBufferReceiveFromISR( StreamBufferHandle_t xStreamBuffer,
                                    வெறுமை * pvRxData,
                                    size_t xBufferLengthBytes,
                                    BaseType_t * மாறிலி pxHigherPriorityTaskWoken )
{
    StreamBuffer_t * மாறிலி pxStreamBuffer = xStreamBuffer;
    size_t xReceivedLength = 0, xBytesAvailable, xBytesToStoreMessageLength;

    configASSERT( pvRxData );
    configASSERT( pxStreamBuffer );

    /* This receive function is used by both message buffers, which store
     * discrete messages, and stream buffers, which store a continuous stream of
     * bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the
     * message. */
    எனில்( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) != ( uint8_t ) 0 )
    {
        xBytesToStoreMessageLength = sbBYTES_TO_STORE_MESSAGE_LENGTH;
    }
    இல்லையெனில்
    {
        xBytesToStoreMessageLength = 0;
    }

    xBytesAvailable = prvBytesInBuffer( pxStreamBuffer );

    /* Whether receiving a discrete message (where xBytesToStoreMessageLength
     * holds the number of bytes used to store the message length) or a stream of
     * bytes (where xBytesToStoreMessageLength is zero), the number of bytes
     * available must be greater than xBytesToStoreMessageLength to be able to
     * read bytes from the buffer. */
    எனில்( xBytesAvailable > xBytesToStoreMessageLength )
    {
        xReceivedLength = prvReadMessageFromBuffer( pxStreamBuffer, pvRxData, xBufferLengthBytes, xBytesAvailable );

        /* Was a task waiting for space in the buffer? */
        எனில்( xReceivedLength != ( size_t ) 0 )
        {
            prvRECEIVE_COMPLETED_FROM_ISR( pxStreamBuffer, pxHigherPriorityTaskWoken );
        }
        இல்லையெனில்
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    இல்லையெனில்
    {
        mtCOVERAGE_TEST_MARKER();
    }

    traceSTREAM_BUFFER_RECEIVE_FROM_ISR( xStreamBuffer, xReceivedLength );

    திரும்பு xReceivedLength;
}
/*-----------------------------------------------------------*/

நிலையான size_t prvReadMessageFromBuffer( StreamBuffer_t * pxStreamBuffer,
                                        வெறுமை * pvRxData,
                                        size_t xBufferLengthBytes,
                                        size_t xBytesAvailable )
{
    size_t x_மொத்தம், xNextMessageLength;
    configMESSAGE_BUFFER_LENGTH_TYPE xTempNextMessageLength;
    size_t xNextTail = pxStreamBuffer->xTail;

    எனில்( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) != ( uint8_t ) 0 )
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
        எனில்( xNextMessageLength > xBufferLengthBytes )
        {
            /* The user has provided insufficient space to read the message. */
            xNextMessageLength = 0;
        }
        இல்லையெனில்
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    இல்லையெனில்
    {
        /* A stream of bytes is being received (as opposed to a discrete
         * message), so read as many bytes as possible. */
        xNextMessageLength = xBufferLengthBytes;
    }

    /* Use the minimum of the wanted bytes and the available bytes. */
    x_மொத்தம் = configMIN( xNextMessageLength, xBytesAvailable );

    எனில்( x_மொத்தம் != ( size_t ) 0 )
    {
        /* Read the actual data and update the tail to mark the data as officially consumed. */
        pxStreamBuffer->xTail = prvReadBytesFromBuffer( pxStreamBuffer, ( uint8_t * ) pvRxData, x_மொத்தம், xNextTail ); /*lint !e9079 Data storage area is implemented as uint8_t array for ease of sizing, indexing and alignment. */
    }

    திரும்பு x_மொத்தம்;
}
/*-----------------------------------------------------------*/

BaseType_t xStreamBufferIsEmpty( StreamBufferHandle_t xStreamBuffer )
{
    மாறிலி StreamBuffer_t * மாறிலி pxStreamBuffer = xStreamBuffer;
    BaseType_t xReturn;
    size_t xTail;

    configASSERT( pxStreamBuffer );

    /* True if no bytes are available. */
    xTail = pxStreamBuffer->xTail;

    எனில்( pxStreamBuffer->xHead == xTail )
    {
        xReturn = pdTRUE;
    }
    இல்லையெனில்
    {
        xReturn = pdFALSE;
    }

    திரும்பு xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xStreamBufferIsFull( StreamBufferHandle_t xStreamBuffer )
{
    BaseType_t xReturn;
    size_t xBytesToStoreMessageLength;
    மாறிலி StreamBuffer_t * மாறிலி pxStreamBuffer = xStreamBuffer;

    configASSERT( pxStreamBuffer );

    /* This generic version of the receive function is used by both message
     * buffers, which store discrete messages, and stream buffers, which store a
     * continuous stream of bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the message. */
    எனில்( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) != ( uint8_t ) 0 )
    {
        xBytesToStoreMessageLength = sbBYTES_TO_STORE_MESSAGE_LENGTH;
    }
    இல்லையெனில்
    {
        xBytesToStoreMessageLength = 0;
    }

    /* True if the available space equals zero. */
    எனில்( xStreamBufferSpacesAvailable( xStreamBuffer ) <= xBytesToStoreMessageLength )
    {
        xReturn = pdTRUE;
    }
    இல்லையெனில்
    {
        xReturn = pdFALSE;
    }

    திரும்பு xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xStreamBufferSendCompletedFromISR( StreamBufferHandle_t xStreamBuffer,
                                              BaseType_t * pxHigherPriorityTaskWoken )
{
    StreamBuffer_t * மாறிலி pxStreamBuffer = xStreamBuffer;
    BaseType_t xReturn;
    UBaseType_t uxSavedInterruptStatus;

    configASSERT( pxStreamBuffer );

    uxSavedInterruptStatus = ( UBaseType_t ) portSET_INTERRUPT_MASK_FROM_ISR();
    {
        எனில்( ( pxStreamBuffer )->xTaskWaitingToReceive != வெற்று )
        {
            ( வெறுமை ) xTaskNotifyFromISR( ( pxStreamBuffer )->xTaskWaitingToReceive,
                                         ( uint32_t ) 0,
                                         eNoAction,
                                         pxHigherPriorityTaskWoken );
            ( pxStreamBuffer )->xTaskWaitingToReceive = வெற்று;
            xReturn = pdTRUE;
        }
        இல்லையெனில்
        {
            xReturn = pdFALSE;
        }
    }
    portCLEAR_INTERRUPT_MASK_FROM_ISR( uxSavedInterruptStatus );

    திரும்பு xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xStreamBufferReceiveCompletedFromISR( StreamBufferHandle_t xStreamBuffer,
                                                 BaseType_t * pxHigherPriorityTaskWoken )
{
    StreamBuffer_t * மாறிலி pxStreamBuffer = xStreamBuffer;
    BaseType_t xReturn;
    UBaseType_t uxSavedInterruptStatus;

    configASSERT( pxStreamBuffer );

    uxSavedInterruptStatus = ( UBaseType_t ) portSET_INTERRUPT_MASK_FROM_ISR();
    {
        எனில்( ( pxStreamBuffer )->xTaskWaitingToSend != வெற்று )
        {
            ( வெறுமை ) xTaskNotifyFromISR( ( pxStreamBuffer )->xTaskWaitingToSend,
                                         ( uint32_t ) 0,
                                         eNoAction,
                                         pxHigherPriorityTaskWoken );
            ( pxStreamBuffer )->xTaskWaitingToSend = வெற்று;
            xReturn = pdTRUE;
        }
        இல்லையெனில்
        {
            xReturn = pdFALSE;
        }
    }
    portCLEAR_INTERRUPT_MASK_FROM_ISR( uxSavedInterruptStatus );

    திரும்பு xReturn;
}
/*-----------------------------------------------------------*/

நிலையான size_t prvWriteBytesToBuffer( StreamBuffer_t * மாறிலி pxStreamBuffer,
                                     மாறிலி uint8_t * pucData,
                                     size_t x_மொத்தம்,
                                     size_t xHead )
{
    size_t xFirstLength;

    configASSERT( x_மொத்தம் > ( size_t ) 0 );

    /* Calculate the number of bytes that can be added in the first write -
     * which may be less than the total number of bytes that need to be added if
     * the buffer will wrap back to the beginning. */
    xFirstLength = configMIN( pxStreamBuffer->xLength - xHead, x_மொத்தம் );

    /* Write as many bytes as can be written in the first write. */
    configASSERT( ( xHead + xFirstLength ) <= pxStreamBuffer->xLength );
    ( வெறுமை ) memcpy( ( வெறுமை * ) ( &( pxStreamBuffer->pucBuffer[ xHead ] ) ), ( மாறிலி வெறுமை * ) pucData, xFirstLength ); /*lint !e9087 memcpy() requires void *. */

    /* If the number of bytes written was less than the number that could be
     * written in the first write... */
    எனில்( x_மொத்தம் > xFirstLength )
    {
        /* ...then write the remaining bytes to the start of the buffer. */
        configASSERT( ( x_மொத்தம் - xFirstLength ) <= pxStreamBuffer->xLength );
        ( வெறுமை ) memcpy( ( வெறுமை * ) pxStreamBuffer->pucBuffer, ( மாறிலி வெறுமை * ) &( pucData[ xFirstLength ] ), x_மொத்தம் - xFirstLength ); /*lint !e9087 memcpy() requires void *. */
    }
    இல்லையெனில்
    {
        mtCOVERAGE_TEST_MARKER();
    }

    xHead += x_மொத்தம்;

    எனில்( xHead >= pxStreamBuffer->xLength )
    {
        xHead -= pxStreamBuffer->xLength;
    }
    இல்லையெனில்
    {
        mtCOVERAGE_TEST_MARKER();
    }

    திரும்பு xHead;
}
/*-----------------------------------------------------------*/

நிலையான size_t prvReadBytesFromBuffer( StreamBuffer_t * pxStreamBuffer,
                                      uint8_t * pucData,
                                      size_t x_மொத்தம்,
                                      size_t xTail )
{
    size_t xFirstLength;

    configASSERT( x_மொத்தம் != ( size_t ) 0 );

    /* Calculate the number of bytes that can be read - which may be
     * less than the number wanted if the data wraps around to the start of
     * the buffer. */
    xFirstLength = configMIN( pxStreamBuffer->xLength - xTail, x_மொத்தம் );

    /* Obtain the number of bytes it is possible to obtain in the first
     * read.  Asserts check bounds of read and write. */
    configASSERT( xFirstLength <= x_மொத்தம் );
    configASSERT( ( xTail + xFirstLength ) <= pxStreamBuffer->xLength );
    ( வெறுமை ) memcpy( ( வெறுமை * ) pucData, ( மாறிலி வெறுமை * ) &( pxStreamBuffer->pucBuffer[ xTail ] ), xFirstLength ); /*lint !e9087 memcpy() requires void *. */

    /* If the total number of wanted bytes is greater than the number
     * that could be read in the first read... */
    எனில்( x_மொத்தம் > xFirstLength )
    {
        /* ...then read the remaining bytes from the start of the buffer. */
        ( வெறுமை ) memcpy( ( வெறுமை * ) &( pucData[ xFirstLength ] ), ( வெறுமை * ) ( pxStreamBuffer->pucBuffer ), x_மொத்தம் - xFirstLength ); /*lint !e9087 memcpy() requires void *. */
    }
    இல்லையெனில்
    {
        mtCOVERAGE_TEST_MARKER();
    }

    /* Move the tail pointer to effectively remove the data read from the buffer. */
    xTail += x_மொத்தம்;

    எனில்( xTail >= pxStreamBuffer->xLength )
    {
        xTail -= pxStreamBuffer->xLength;
    }

    திரும்பு xTail;
}
/*-----------------------------------------------------------*/

நிலையான size_t prvBytesInBuffer( மாறிலி StreamBuffer_t * மாறிலி pxStreamBuffer )
{
/* Returns the distance between xTail and xHead. */
    size_t x_மொத்தம்;

    x_மொத்தம் = pxStreamBuffer->xLength + pxStreamBuffer->xHead;
    x_மொத்தம் -= pxStreamBuffer->xTail;

    எனில்( x_மொத்தம் >= pxStreamBuffer->xLength )
    {
        x_மொத்தம் -= pxStreamBuffer->xLength;
    }
    இல்லையெனில்
    {
        mtCOVERAGE_TEST_MARKER();
    }

    திரும்பு x_மொத்தம்;
}
/*-----------------------------------------------------------*/

நிலையான வெறுமை prvInitialiseNewStreamBuffer( StreamBuffer_t * மாறிலி pxStreamBuffer,
                                          uint8_t * மாறிலி pucBuffer,
                                          size_t xBufferSizeBytes,
                                          size_t xTriggerLevelBytes,
                                          uint8_t ucFlags,
                                          StreamBufferCallbackFunction_t pxSendCompletedCallback,
                                          StreamBufferCallbackFunction_t pxReceiveCompletedCallback )
{
    /* Assert here is deliberately writing to the entire buffer to ensure it can
     * be written to without generating exceptions, and is setting the buffer to a
     * known value to assist in development/debugging. */
    #எனில் ( configASSERT_DEFINED == 1 )
    {
        /* The value written just has to be identifiable when looking at the
         * memory.  Don't use 0xA5 as that is the stack fill value and could
         * result in confusion as to what is actually being observed. */
        மாறிலி BaseType_t x_Write_மதிப்பு = 0x55;
        configASSERT( memset( pucBuffer, ( முழு ) x_Write_மதிப்பு, xBufferSizeBytes ) == pucBuffer );
        ( வெறுமை ) x_Write_மதிப்பு;
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
    #எனில் ( configUSE_TRACE_FACILITY == 1 )
        pxStreamBuffer->ux_Stream_Buffer_எண் = ( UBaseType_t ) 0;
    #endif
    pxStreamBuffer->pucBuffer = pucBuffer;
    pxStreamBuffer->xLength = xBufferSizeBytes;
    pxStreamBuffer->xTriggerLevelBytes = xTriggerLevelBytes;
    pxStreamBuffer->ucFlags = ucFlags;
    #எனில் ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    {
        pxStreamBuffer->pxSendCompletedCallback = pxSendCompletedCallback;
        pxStreamBuffer->pxReceiveCompletedCallback = pxReceiveCompletedCallback;
    }
    #இல்லையெனில்
    {
        ( வெறுமை ) pxSendCompletedCallback;
        ( வெறுமை ) pxReceiveCompletedCallback;
    }
    #endif
}

#எனில் ( configUSE_TRACE_FACILITY == 1 )

    UBaseType_t ux_Stream_Buffer_Get_Stream_Buffer_எண்( StreamBufferHandle_t xStreamBuffer )
    {
        திரும்பு xStreamBuffer->ux_Stream_Buffer_எண்;
    }

#endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#எனில் ( configUSE_TRACE_FACILITY == 1 )

    வெறுமை v_Stream_Buffer_Set_Stream_Buffer_எண்( StreamBufferHandle_t xStreamBuffer,
                                             UBaseType_t ux_Stream_Buffer_எண் )
    {
        xStreamBuffer->ux_Stream_Buffer_எண் = ux_Stream_Buffer_எண்;
    }

#endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#எனில் ( configUSE_TRACE_FACILITY == 1 )

    uint8_t ucStreamBufferGetStreamBufferType( StreamBufferHandle_t xStreamBuffer )
    {
        திரும்பு( xStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER );
    }

#endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/
