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
#ضمن <stdint.h>
#ضمن <string.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* FreeRTOS includes. */
#ضمن "FreeRTOS.h"
#ضمن "task.h"
#ضمن "stream_buffer.h"
/* Include private IDF API additions for critical thread safety macros */
#ضمن "esp_private/freertos_idf_additions_priv.h"

#إذا ( configUSE_TASK_NOTIFICATIONS != 1 )
    #error configUSE_TASK_NOTIFICATIONS must be set to 1 to build stream_buffer.c
#endif

#إذا ( INCLUDE_xTaskGetCurrentTaskHandle != 1 )
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
        إذا( ( pxStreamBuffer )->xTaskWaitingToSend != عدم )                    \
        {                                                                       \
            ( فراغ ) xTaskNotify( ( pxStreamBuffer )->xTaskWaitingToSend,       \
                                  ( uint32_t ) 0,                               \
                                  eNoAction );                                  \
            ( pxStreamBuffer )->xTaskWaitingToSend = عدم;                      \
        }                                                                       \
    }                                                                           \
    ( فراغ ) prvEXIT_CRITICAL_OR_RESUME_ALL( &( pxStreamBuffer->xStreamBufferLock ) );
#endif /* sbRECEIVE_COMPLETED */

/* If user has provided a per-instance receive complete callback, then
 * invoke the callback else use the receive complete macro which is provided by default for all instances.
 */
#إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #define prvRECEIVE_COMPLETED( pxStreamBuffer )                                               \
    {                                                                                            \
        إذا( ( pxStreamBuffer )->pxReceiveCompletedCallback != عدم )                             \
        {                                                                                        \
            ( pxStreamBuffer )->pxReceiveCompletedCallback( ( pxStreamBuffer ), pdFALSE, عدم ); \
        }                                                                                        \
        وإلا                                                                                     \
        {                                                                                        \
            sbRECEIVE_COMPLETED( ( pxStreamBuffer ) );                                           \
        }                                                                                        \
    }
#وإلا /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
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
            إذا( ( pxStreamBuffer )->xTaskWaitingToSend != عدم )                     \
            {                                                                        \
                ( فراغ ) xTaskNotifyFromISR( ( pxStreamBuffer )->xTaskWaitingToSend, \
                                             ( uint32_t ) 0,                         \
                                             eNoAction,                              \
                                             ( pxHigherPriorityTaskWoken ) );        \
                ( pxStreamBuffer )->xTaskWaitingToSend = عدم;                       \
            }                                                                        \
        }                                                                            \
        portCLEAR_INTERRUPT_MASK_FROM_ISR( uxSavedInterruptStatus );                 \
    }
#endif /* sbRECEIVE_COMPLETED_FROM_ISR */

#إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #define prvRECEIVE_COMPLETED_FROM_ISR( pxStreamBuffer,                                                               \
                                           pxHigherPriorityTaskWoken )                                                   \
    {                                                                                                                    \
        إذا( ( pxStreamBuffer )->pxReceiveCompletedCallback != عدم )                                                     \
        {                                                                                                                \
            ( pxStreamBuffer )->pxReceiveCompletedCallback( ( pxStreamBuffer ), pdTRUE, ( pxHigherPriorityTaskWoken ) ); \
        }                                                                                                                \
        وإلا                                                                                                             \
        {                                                                                                                \
            sbRECEIVE_COMPLETED_FROM_ISR( ( pxStreamBuffer ), ( pxHigherPriorityTaskWoken ) );                           \
        }                                                                                                                \
    }
#وإلا /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
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
        إذا( ( pxStreamBuffer )->xTaskWaitingToReceive != عدم )                 \
        {                                                                       \
            ( فراغ ) xTaskNotify( ( pxStreamBuffer )->xTaskWaitingToReceive,    \
                                  ( uint32_t ) 0,                               \
                                  eNoAction );                                  \
            ( pxStreamBuffer )->xTaskWaitingToReceive = عدم;                   \
        }                                                                       \
    }                                                                           \
    ( فراغ ) prvEXIT_CRITICAL_OR_RESUME_ALL( &( pxStreamBuffer->xStreamBufferLock ) );
#endif /* sbSEND_COMPLETED */

/* If user has provided a per-instance send completed callback, then
 * invoke the callback else use the send complete macro which is provided by default for all instances.
 */
#إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #define prvSEND_COMPLETED( pxStreamBuffer )                                           \
    {                                                                                     \
        إذا( ( pxStreamBuffer )->pxSendCompletedCallback != عدم )                         \
        {                                                                                 \
            pxStreamBuffer->pxSendCompletedCallback( ( pxStreamBuffer ), pdFALSE, عدم ); \
        }                                                                                 \
        وإلا                                                                              \
        {                                                                                 \
            sbSEND_COMPLETED( ( pxStreamBuffer ) );                                       \
        }                                                                                 \
    }
#وإلا /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #define prvSEND_COMPLETED( pxStreamBuffer )    sbSEND_COMPLETED( ( pxStreamBuffer ) )
#endif /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */


#ifndef sbSEND_COMPLETE_FROM_ISR
    #define sbSEND_COMPLETE_FROM_ISR( pxStreamBuffer, pxHigherPriorityTaskWoken )       \
    {                                                                                   \
        UBaseType_t uxSavedInterruptStatus;                                             \
                                                                                        \
        uxSavedInterruptStatus = ( UBaseType_t ) portSET_INTERRUPT_MASK_FROM_ISR();     \
        {                                                                               \
            إذا( ( pxStreamBuffer )->xTaskWaitingToReceive != عدم )                     \
            {                                                                           \
                ( فراغ ) xTaskNotifyFromISR( ( pxStreamBuffer )->xTaskWaitingToReceive, \
                                             ( uint32_t ) 0,                            \
                                             eNoAction,                                 \
                                             ( pxHigherPriorityTaskWoken ) );           \
                ( pxStreamBuffer )->xTaskWaitingToReceive = عدم;                       \
            }                                                                           \
        }                                                                               \
        portCLEAR_INTERRUPT_MASK_FROM_ISR( uxSavedInterruptStatus );                    \
    }
#endif /* sbSEND_COMPLETE_FROM_ISR */


#إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #define prvSEND_COMPLETE_FROM_ISR( pxStreamBuffer, pxHigherPriorityTaskWoken )                                    \
    {                                                                                                                 \
        إذا( ( pxStreamBuffer )->pxSendCompletedCallback != عدم )                                                     \
        {                                                                                                             \
            ( pxStreamBuffer )->pxSendCompletedCallback( ( pxStreamBuffer ), pdTRUE, ( pxHigherPriorityTaskWoken ) ); \
        }                                                                                                             \
        وإلا                                                                                                          \
        {                                                                                                             \
            sbSEND_COMPLETE_FROM_ISR( ( pxStreamBuffer ), ( pxHigherPriorityTaskWoken ) );                            \
        }                                                                                                             \
    }
#وإلا /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #define prvSEND_COMPLETE_FROM_ISR( pxStreamBuffer, pxHigherPriorityTaskWoken ) \
    sbSEND_COMPLETE_FROM_ISR( ( pxStreamBuffer ), ( pxHigherPriorityTaskWoken ) )
#endif /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

/*lint -restore (9026) */

/* The number of bytes used to hold the length of a message in the buffer. */
#define sbBYTES_TO_STORE_MESSAGE_LENGTH    ( حجم( configMESSAGE_BUFFER_LENGTH_TYPE ) )

/* Bits stored in the ucFlags field of the stream buffer. */
#define sbFLAGS_IS_MESSAGE_BUFFER          ( ( uint8_t ) 1 ) /* Set if the stream buffer was created as a message buffer, in which case it holds discrete messages rather than a stream. */
#define sbFLAGS_IS_STATICALLY_ALLOCATED    ( ( uint8_t ) 2 ) /* Set if the stream buffer was created using statically allocated memory. */

/*-----------------------------------------------------------*/

/* Structure that hold state information on the buffer. */
تعريفنوع بنية StreamBufferDef_t                 /*lint !e9058 Style convention uses tag. */
{
    متطاير size_t xTail;                       /* Index to the next item to read within the buffer. */
    متطاير size_t xHead;                       /* Index to the next item to write within the buffer. */
    size_t xLength;                              /* The length of the buffer pointed to by pucBuffer. */
    size_t xTriggerLevelBytes;                   /* The number of bytes that must be in the stream buffer before a task that is waiting for data is unblocked. */
    متطاير TaskHandle_t xTaskWaitingToReceive; /* Holds the handle of a task waiting for data, or NULL if no tasks are waiting. */
    متطاير TaskHandle_t xTaskWaitingToSend;    /* Holds the handle of a task waiting to send data to a message buffer that is full. */
    uint8_t * pucBuffer;                         /* Points to the buffer itself - that is - the RAM that stores the data passed through the buffer. */
    uint8_t ucFlags;

    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_Stream_Buffer_سان; /* Used for tracing purposes. */
    #endif

    #إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        StreamBufferCallbackFunction_t pxSendCompletedCallback;    /* Optional callback called on send complete. sbSEND_COMPLETED is called if this is NULL. */
        StreamBufferCallbackFunction_t pxReceiveCompletedCallback; /* Optional callback called on receive complete.  sbRECEIVE_COMPLETED is called if this is NULL. */
    #endif

    portMUX_TYPE xStreamBufferLock; /* Spinlock required for SMP critical sections */
} StreamBuffer_t;

/*
 * The number of bytes available to be read from the buffer.
 */
ساكن size_t prvBytesInBuffer( ثابت StreamBuffer_t * ثابت pxStreamBuffer ) PRIVILEGED_FUNCTION;

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
ساكن size_t prvWriteBytesToBuffer( StreamBuffer_t * ثابت pxStreamBuffer,
                                     ثابت uint8_t * pucData,
                                     size_t x_سانى,
                                     size_t xHead ) PRIVILEGED_FUNCTION;

/*
 * If the stream buffer is being used as a message buffer, then reads an entire
 * message out of the buffer.  If the stream buffer is being used as a stream
 * buffer then read as many bytes as possible from the buffer.
 * prvReadBytesFromBuffer() is called to actually extract the bytes from the
 * buffer's data storage area.
 */
ساكن size_t prvReadMessageFromBuffer( StreamBuffer_t * pxStreamBuffer,
                                        فراغ * pvRxData,
                                        size_t xBufferLengthBytes,
                                        size_t xBytesAvailable ) PRIVILEGED_FUNCTION;

/*
 * If the stream buffer is being used as a message buffer, then writes an entire
 * message to the buffer.  If the stream buffer is being used as a stream
 * buffer then write as many bytes as possible to the buffer.
 * prvWriteBytestoBuffer() is called to actually send the bytes to the buffer's
 * data storage area.
 */
ساكن size_t prvWriteMessageToBuffer( StreamBuffer_t * ثابت pxStreamBuffer,
                                       ثابت فراغ * pvTxData,
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
ساكن size_t prvReadBytesFromBuffer( StreamBuffer_t * pxStreamBuffer,
                                      uint8_t * pucData,
                                      size_t x_سانى,
                                      size_t xTail ) PRIVILEGED_FUNCTION;

/*
 * Called by both pxStreamBufferCreate() and pxStreamBufferCreateStatic() to
 * initialise the members of the newly created stream buffer structure.
 */
ساكن فراغ prvInitialiseNewStreamBuffer( StreamBuffer_t * ثابت pxStreamBuffer,
                                          uint8_t * ثابت pucBuffer,
                                          size_t xBufferSizeBytes,
                                          size_t xTriggerLevelBytes,
                                          uint8_t ucFlags,
                                          StreamBufferCallbackFunction_t pxSendCompletedCallback,
                                          StreamBufferCallbackFunction_t pxReceiveCompletedCallback ) PRIVILEGED_FUNCTION;

/*-----------------------------------------------------------*/
#إذا ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
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
        إذا( xIsMessageBuffer == pdTRUE )
        {
            /* Is a message buffer but not statically allocated. */
            ucFlags = sbFLAGS_IS_MESSAGE_BUFFER;
            configASSERT( xBufferSizeBytes > sbBYTES_TO_STORE_MESSAGE_LENGTH );
        }
        وإلا
        {
            /* Not a message buffer and not statically allocated. */
            ucFlags = 0;
            configASSERT( xBufferSizeBytes > 0 );
        }

        configASSERT( xTriggerLevelBytes <= xBufferSizeBytes );

        /* A trigger level of 0 would cause a waiting task to unblock even when
         * the buffer was empty. */
        إذا( xTriggerLevelBytes == ( size_t ) 0 )
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
        إذا( xBufferSizeBytes < ( xBufferSizeBytes + 1 + حجم( StreamBuffer_t ) ) )
        {
            xBufferSizeBytes++;
            pucAllocatedMemory = ( uint8_t * ) pvPortMalloc( xBufferSizeBytes + حجم( StreamBuffer_t ) ); /*lint !e9079 malloc() only returns void*. */
        }
        وإلا
        {
            pucAllocatedMemory = عدم;
        }

        إذا( pucAllocatedMemory != عدم )
        {
            prvInitialiseNewStreamBuffer( ( StreamBuffer_t * ) pucAllocatedMemory,       /* Structure at the start of the allocated memory. */ /*lint !e9087 Safe cast as allocated memory is aligned. */ /*lint !e826 Area is not too small and alignment is guaranteed provided malloc() behaves as expected and returns aligned buffer. */
                                          pucAllocatedMemory + حجم( StreamBuffer_t ), /* Storage area follows. */ /*lint !e9016 Indexing past structure valid for uint8_t pointer, also storage area has no alignment requirement. */
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
        وإلا
        {
            traceSTREAM_BUFFER_CREATE_FAILED( xIsMessageBuffer );
        }

        أرجع ( StreamBufferHandle_t ) pucAllocatedMemory; /*lint !e9087 !e826 Safe cast as allocated memory is aligned. */
    }
#endif /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

#إذا ( configSUPPORT_STATIC_ALLOCATION == 1 )

    StreamBufferHandle_t xStreamBufferGenericCreateStatic( size_t xBufferSizeBytes,
                                                           size_t xTriggerLevelBytes,
                                                           BaseType_t xIsMessageBuffer,
                                                           uint8_t * ثابت pucStreamBufferStorageArea,
                                                           StaticStreamBuffer_t * ثابت pxStaticStreamBuffer,
                                                           StreamBufferCallbackFunction_t pxSendCompletedCallback,
                                                           StreamBufferCallbackFunction_t pxReceiveCompletedCallback )
    {
        StreamBuffer_t * ثابت pxStreamBuffer = ( StreamBuffer_t * ) pxStaticStreamBuffer; /*lint !e740 !e9087 Safe cast as StaticStreamBuffer_t is opaque Streambuffer_t. */
        StreamBufferHandle_t xReturn;
        uint8_t ucFlags;

        configASSERT( pucStreamBufferStorageArea );
        configASSERT( pxStaticStreamBuffer );
        configASSERT( xTriggerLevelBytes <= xBufferSizeBytes );

        /* A trigger level of 0 would cause a waiting task to unblock even when
         * the buffer was empty. */
        إذا( xTriggerLevelBytes == ( size_t ) 0 )
        {
            xTriggerLevelBytes = ( size_t ) 1;
        }

        إذا( xIsMessageBuffer != pdFALSE )
        {
            /* Statically allocated message buffer. */
            ucFlags = sbFLAGS_IS_MESSAGE_BUFFER | sbFLAGS_IS_STATICALLY_ALLOCATED;
        }
        وإلا
        {
            /* Statically allocated stream buffer. */
            ucFlags = sbFLAGS_IS_STATICALLY_ALLOCATED;
        }

        /* In case the stream buffer is going to be used as a message buffer
         * (that is, it will hold discrete messages with a little meta data that
         * says how big the next message is) check the buffer will be large enough
         * to hold at least one message. */
        configASSERT( xBufferSizeBytes > sbBYTES_TO_STORE_MESSAGE_LENGTH );

        #إذا ( configASSERT_DEFINED == 1 )
        {
            /* Sanity check that the size of the structure used to declare a
             * variable of type StaticStreamBuffer_t equals the size of the real
             * message buffer structure. */
            متطاير size_t xSize = حجم( StaticStreamBuffer_t );
            configASSERT( xSize == حجم( StreamBuffer_t ) );
            ( فراغ ) xSize; /* Prevent unused variable warning when configASSERT() is not used. */
        } /*lint !e529 xSize is referenced is configASSERT() is defined. */
        #endif /* configASSERT_DEFINED */

        إذا( ( pucStreamBufferStorageArea != عدم ) && ( pxStaticStreamBuffer != عدم ) )
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
        وإلا
        {
            xReturn = عدم;
            traceSTREAM_BUFFER_CREATE_STATIC_FAILED( xReturn, xIsMessageBuffer );
        }

        أرجع xReturn;
    }
#endif /* ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*-----------------------------------------------------------*/

#إذا ( configSUPPORT_STATIC_ALLOCATION == 1 )
    BaseType_t xStreamBufferGetStaticBuffers( StreamBufferHandle_t xStreamBuffer,
                                              uint8_t ** ppucStreamBufferStorageArea,
                                              StaticStreamBuffer_t ** ppxStaticStreamBuffer )
    {
        BaseType_t xReturn;
        ثابت StreamBuffer_t * ثابت pxStreamBuffer = xStreamBuffer;

        configASSERT( pxStreamBuffer );
        configASSERT( ppucStreamBufferStorageArea );
        configASSERT( ppxStaticStreamBuffer );

        إذا( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_STATICALLY_ALLOCATED ) != ( uint8_t ) 0 )
        {
            *ppucStreamBufferStorageArea = pxStreamBuffer->pucBuffer;
            *ppxStaticStreamBuffer = ( StaticStreamBuffer_t * ) pxStreamBuffer;
            xReturn = pdTRUE;
        }
        وإلا
        {
            xReturn = pdFALSE;
        }

        أرجع xReturn;
    }
#endif /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

فراغ vStreamBufferDelete( StreamBufferHandle_t xStreamBuffer )
{
    StreamBuffer_t * pxStreamBuffer = xStreamBuffer;

    configASSERT( pxStreamBuffer );

    traceSTREAM_BUFFER_DELETE( xStreamBuffer );

    إذا( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_STATICALLY_ALLOCATED ) == ( uint8_t ) pdFALSE )
    {
        #إذا ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
        {
            /* Both the structure and the buffer were allocated using a single call
            * to pvPortMalloc(), hence only one call to vPortFree() is required. */
            vPortFree( ( فراغ * ) pxStreamBuffer ); /*lint !e9087 Standard free() semantics require void *, plus pxStreamBuffer was allocated by pvPortMalloc(). */
        }
        #وإلا
        {
            /* Should not be possible to get here, ucFlags must be corrupt.
             * Force an assert. */
            configASSERT( xStreamBuffer == ( StreamBufferHandle_t ) ~0 );
        }
        #endif
    }
    وإلا
    {
        /* The structure and buffer were not allocated dynamically and cannot be
         * freed - just scrub the structure so future use will assert. */
        ( فراغ ) memset( pxStreamBuffer, 0x00, حجم( StreamBuffer_t ) );
    }
}
/*-----------------------------------------------------------*/

BaseType_t xStreamBufferReset( StreamBufferHandle_t xStreamBuffer )
{
    StreamBuffer_t * ثابت pxStreamBuffer = xStreamBuffer;
    BaseType_t xReturn = pdFAIL;
    StreamBufferCallbackFunction_t pxSendCallback = عدم, pxReceiveCallback = عدم;

    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_Stream_Buffer_سان;
    #endif

    configASSERT( pxStreamBuffer );

    #إذا ( configUSE_TRACE_FACILITY == 1 )
    {
        /* Store the stream buffer number so it can be restored after the
         * reset. */
        ux_Stream_Buffer_سان = pxStreamBuffer->ux_Stream_Buffer_سان;
    }
    #endif

    /* Can only reset a message buffer if there are no tasks blocked on it. */
    taskENTER_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );
    {
        إذا( ( pxStreamBuffer->xTaskWaitingToReceive == عدم ) && ( pxStreamBuffer->xTaskWaitingToSend == عدم ) )
        {
            #إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
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

            #إذا ( configUSE_TRACE_FACILITY == 1 )
            {
                pxStreamBuffer->ux_Stream_Buffer_سان = ux_Stream_Buffer_سان;
            }
            #endif

            traceSTREAM_BUFFER_RESET( xStreamBuffer );

            xReturn = pdPASS;
        }
    }
    taskEXIT_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xStreamBufferSetTriggerLevel( StreamBufferHandle_t xStreamBuffer,
                                         size_t xTriggerLevel )
{
    StreamBuffer_t * ثابت pxStreamBuffer = xStreamBuffer;
    BaseType_t xReturn;

    configASSERT( pxStreamBuffer );

    /* It is not valid for the trigger level to be 0. */
    إذا( xTriggerLevel == ( size_t ) 0 )
    {
        xTriggerLevel = ( size_t ) 1;
    }

    /* The trigger level is the number of bytes that must be in the stream
     * buffer before a task that is waiting for data is unblocked. */
    إذا( xTriggerLevel < pxStreamBuffer->xLength )
    {
        pxStreamBuffer->xTriggerLevelBytes = xTriggerLevel;
        xReturn = pdPASS;
    }
    وإلا
    {
        xReturn = pdFALSE;
    }

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

size_t xStreamBufferSpacesAvailable( StreamBufferHandle_t xStreamBuffer )
{
    ثابت StreamBuffer_t * ثابت pxStreamBuffer = xStreamBuffer;
    size_t xSpace;
    size_t xOriginalTail;

    configASSERT( pxStreamBuffer );

    /* The code below reads xTail and then xHead.  This is safe if the stream
     * buffer is updated once between the two reads - but not if the stream buffer
     * is updated more than once between the two reads - hence the loop. */
    افعل
    {
        xOriginalTail = pxStreamBuffer->xTail;
        xSpace = pxStreamBuffer->xLength + pxStreamBuffer->xTail;
        xSpace -= pxStreamBuffer->xHead;
    } طالما( xOriginalTail != pxStreamBuffer->xTail );

    xSpace -= ( size_t ) 1;

    إذا( xSpace >= pxStreamBuffer->xLength )
    {
        xSpace -= pxStreamBuffer->xLength;
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    أرجع xSpace;
}
/*-----------------------------------------------------------*/

size_t xStreamBufferBytesAvailable( StreamBufferHandle_t xStreamBuffer )
{
    ثابت StreamBuffer_t * ثابت pxStreamBuffer = xStreamBuffer;
    size_t xReturn;

    configASSERT( pxStreamBuffer );

    xReturn = prvBytesInBuffer( pxStreamBuffer );
    أرجع xReturn;
}
/*-----------------------------------------------------------*/

size_t xStreamBufferSend( StreamBufferHandle_t xStreamBuffer,
                          ثابت فراغ * pvTxData,
                          size_t xDataLengthBytes,
                          TickType_t xTicksToWait )
{
    StreamBuffer_t * ثابت pxStreamBuffer = xStreamBuffer;
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
    إذا( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) != ( uint8_t ) 0 )
    {
        xRequiredSpace += sbBYTES_TO_STORE_MESSAGE_LENGTH;

        /* Overflow? */
        configASSERT( xRequiredSpace > xDataLengthBytes );

        /* If this is a message buffer then it must be possible to write the
         * whole message. */
        إذا( xRequiredSpace > xMaxReportedSpace )
        {
            /* The message would not fit even if the entire buffer was empty,
             * so don't wait for space. */
            xTicksToWait = ( TickType_t ) 0;
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    وإلا
    {
        /* If this is a stream buffer then it is acceptable to write only part
         * of the message to the buffer.  Cap the length to the total length of
         * the buffer. */
        إذا( xRequiredSpace > xMaxReportedSpace )
        {
            xRequiredSpace = xMaxReportedSpace;
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }

    إذا( xTicksToWait != ( TickType_t ) 0 )
    {
        vTaskSetTimeOutState( &xTimeOut );

        افعل
        {
            /* Wait until the required number of bytes are free in the message
             * buffer. */
            taskENTER_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );
            {
                xSpace = xStreamBufferSpacesAvailable( pxStreamBuffer );

                إذا( xSpace < xRequiredSpace )
                {
                    /* Clear notification state as going to wait for space. */
                    ( فراغ ) xTaskNotifyStateClear( عدم );

                    /* Should only be one writer. */
                    configASSERT( pxStreamBuffer->xTaskWaitingToSend == عدم );
                    pxStreamBuffer->xTaskWaitingToSend = xTaskGetCurrentTaskHandle();
                }
                وإلا
                {
                    taskEXIT_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );
                    اكسر;
                }
            }
            taskEXIT_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );

            traceBLOCKING_ON_STREAM_BUFFER_SEND( xStreamBuffer );
            ( فراغ ) xTaskNotifyWait( ( uint32_t ) 0, ( uint32_t ) 0, عدم, xTicksToWait );
            /* In SMP mode, the task may have been woken and scheduled on
             * another core. Hence, we must clear the xTaskWaitingToSend
             * handle in a critical section. */
            #إذا ( config_سان_OF_CORES > 1 )
                taskENTER_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );
            #endif /* configNUMBER_OF_CORES > 1 */
            {
                pxStreamBuffer->xTaskWaitingToSend = عدم;
            }
            #إذا ( config_سان_OF_CORES > 1 )
                taskEXIT_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );
            #endif /* configNUMBER_OF_CORES > 1 */
        } طالما( xTaskCheckForTimeOut( &xTimeOut, &xTicksToWait ) == pdFALSE );
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    إذا( xSpace == ( size_t ) 0 )
    {
        xSpace = xStreamBufferSpacesAvailable( pxStreamBuffer );
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    xReturn = prvWriteMessageToBuffer( pxStreamBuffer, pvTxData, xDataLengthBytes, xSpace, xRequiredSpace );

    إذا( xReturn > ( size_t ) 0 )
    {
        traceSTREAM_BUFFER_SEND( xStreamBuffer, xReturn );

        /* Was a task waiting for the data? */
        إذا( prvBytesInBuffer( pxStreamBuffer ) >= pxStreamBuffer->xTriggerLevelBytes )
        {
            prvSEND_COMPLETED( pxStreamBuffer );
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
        traceSTREAM_BUFFER_SEND_FAILED( xStreamBuffer );
    }

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

size_t xStreamBufferSendFromISR( StreamBufferHandle_t xStreamBuffer,
                                 ثابت فراغ * pvTxData,
                                 size_t xDataLengthBytes,
                                 BaseType_t * ثابت pxHigherPriorityTaskWoken )
{
    StreamBuffer_t * ثابت pxStreamBuffer = xStreamBuffer;
    size_t xReturn, xSpace;
    size_t xRequiredSpace = xDataLengthBytes;

    configASSERT( pvTxData );
    configASSERT( pxStreamBuffer );

    /* This send function is used to write to both message buffers and stream
     * buffers.  If this is a message buffer then the space needed must be
     * increased by the amount of bytes needed to store the length of the
     * message. */
    إذا( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) != ( uint8_t ) 0 )
    {
        xRequiredSpace += sbBYTES_TO_STORE_MESSAGE_LENGTH;
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    xSpace = xStreamBufferSpacesAvailable( pxStreamBuffer );
    xReturn = prvWriteMessageToBuffer( pxStreamBuffer, pvTxData, xDataLengthBytes, xSpace, xRequiredSpace );

    إذا( xReturn > ( size_t ) 0 )
    {
        /* Was a task waiting for the data? */
        إذا( prvBytesInBuffer( pxStreamBuffer ) >= pxStreamBuffer->xTriggerLevelBytes )
        {
            prvSEND_COMPLETE_FROM_ISR( pxStreamBuffer, pxHigherPriorityTaskWoken );
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    traceSTREAM_BUFFER_SEND_FROM_ISR( xStreamBuffer, xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

ساكن size_t prvWriteMessageToBuffer( StreamBuffer_t * ثابت pxStreamBuffer,
                                       ثابت فراغ * pvTxData,
                                       size_t xDataLengthBytes,
                                       size_t xSpace,
                                       size_t xRequiredSpace )
{
    size_t xNextHead = pxStreamBuffer->xHead;
    configMESSAGE_BUFFER_LENGTH_TYPE xMessageLength;

    إذا( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) != ( uint8_t ) 0 )
    {
        /* This is a message buffer, as opposed to a stream buffer. */

        /* Convert xDataLengthBytes to the message length type. */
        xMessageLength = ( configMESSAGE_BUFFER_LENGTH_TYPE ) xDataLengthBytes;

        /* Ensure the data length given fits within configMESSAGE_BUFFER_LENGTH_TYPE. */
        configASSERT( ( size_t ) xMessageLength == xDataLengthBytes );

        إذا( xSpace >= xRequiredSpace )
        {
            /* There is enough space to write both the message length and the message
             * itself into the buffer.  Start by writing the length of the data, the data
             * itself will be written later in this function. */
            xNextHead = prvWriteBytesToBuffer( pxStreamBuffer, ( ثابت uint8_t * ) &( xMessageLength ), sbBYTES_TO_STORE_MESSAGE_LENGTH, xNextHead );
        }
        وإلا
        {
            /* Not enough space, so do not write data to the buffer. */
            xDataLengthBytes = 0;
        }
    }
    وإلا
    {
        /* This is a stream buffer, as opposed to a message buffer, so writing a
         * stream of bytes rather than discrete messages.  Plan to write as many
         * bytes as possible. */
        xDataLengthBytes = configMIN( xDataLengthBytes, xSpace );
    }

    إذا( xDataLengthBytes != ( size_t ) 0 )
    {
        /* Write the data to the buffer. */
        pxStreamBuffer->xHead = prvWriteBytesToBuffer( pxStreamBuffer, ( ثابت uint8_t * ) pvTxData, xDataLengthBytes, xNextHead ); /*lint !e9079 Storage buffer is implemented as uint8_t for ease of sizing, alignment and access. */
    }

    أرجع xDataLengthBytes;
}
/*-----------------------------------------------------------*/

size_t xStreamBufferReceive( StreamBufferHandle_t xStreamBuffer,
                             فراغ * pvRxData,
                             size_t xBufferLengthBytes,
                             TickType_t xTicksToWait )
{
    StreamBuffer_t * ثابت pxStreamBuffer = xStreamBuffer;
    size_t xReceivedLength = 0, xBytesAvailable, xBytesToStoreMessageLength;

    configASSERT( pvRxData );
    configASSERT( pxStreamBuffer );

    /* This receive function is used by both message buffers, which store
     * discrete messages, and stream buffers, which store a continuous stream of
     * bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the
     * message. */
    إذا( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) != ( uint8_t ) 0 )
    {
        xBytesToStoreMessageLength = sbBYTES_TO_STORE_MESSAGE_LENGTH;
    }
    وإلا
    {
        xBytesToStoreMessageLength = 0;
    }

    إذا( xTicksToWait != ( TickType_t ) 0 )
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
            إذا( xBytesAvailable <= xBytesToStoreMessageLength )
            {
                /* Clear notification state as going to wait for data. */
                ( فراغ ) xTaskNotifyStateClear( عدم );

                /* Should only be one reader. */
                configASSERT( pxStreamBuffer->xTaskWaitingToReceive == عدم );
                pxStreamBuffer->xTaskWaitingToReceive = xTaskGetCurrentTaskHandle();
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        taskEXIT_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );

        إذا( xBytesAvailable <= xBytesToStoreMessageLength )
        {
            /* Wait for data to be available. */
            traceBLOCKING_ON_STREAM_BUFFER_RECEIVE( xStreamBuffer );
            ( فراغ ) xTaskNotifyWait( ( uint32_t ) 0, ( uint32_t ) 0, عدم, xTicksToWait );

            /* In SMP mode, the task may have been woken and scheduled on
             * another core. Hence, we must clear the xTaskWaitingToReceive
             * handle in a critical section. */
            #إذا ( config_سان_OF_CORES > 1 )
                taskENTER_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );
            #endif /* configNUMBER_OF_CORES > 1 */
            {
                pxStreamBuffer->xTaskWaitingToReceive = عدم;
            }
            #إذا ( config_سان_OF_CORES > 1 )
                taskEXIT_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );
            #endif /* configNUMBER_OF_CORES > 1 */

            /* Recheck the data available after blocking. */
            xBytesAvailable = prvBytesInBuffer( pxStreamBuffer );
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    وإلا
    {
        xBytesAvailable = prvBytesInBuffer( pxStreamBuffer );
    }

    /* Whether receiving a discrete message (where xBytesToStoreMessageLength
     * holds the number of bytes used to store the message length) or a stream of
     * bytes (where xBytesToStoreMessageLength is zero), the number of bytes
     * available must be greater than xBytesToStoreMessageLength to be able to
     * read bytes from the buffer. */
    إذا( xBytesAvailable > xBytesToStoreMessageLength )
    {
        xReceivedLength = prvReadMessageFromBuffer( pxStreamBuffer, pvRxData, xBufferLengthBytes, xBytesAvailable );

        /* Was a task waiting for space in the buffer? */
        إذا( xReceivedLength != ( size_t ) 0 )
        {
            traceSTREAM_BUFFER_RECEIVE( xStreamBuffer, xReceivedLength );
            prvRECEIVE_COMPLETED( xStreamBuffer );
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    وإلا
    {
        traceSTREAM_BUFFER_RECEIVE_FAILED( xStreamBuffer );
        mtCOVERAGE_TEST_MARKER();
    }

    أرجع xReceivedLength;
}
/*-----------------------------------------------------------*/

size_t xStreamBufferNextMessageLengthBytes( StreamBufferHandle_t xStreamBuffer )
{
    StreamBuffer_t * ثابت pxStreamBuffer = xStreamBuffer;
    size_t xReturn, xBytesAvailable;
    configMESSAGE_BUFFER_LENGTH_TYPE xTempReturn;

    configASSERT( pxStreamBuffer );

    /* Ensure the stream buffer is being used as a message buffer. */
    إذا( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) != ( uint8_t ) 0 )
    {
        xBytesAvailable = prvBytesInBuffer( pxStreamBuffer );

        إذا( xBytesAvailable > sbBYTES_TO_STORE_MESSAGE_LENGTH )
        {
            /* The number of bytes available is greater than the number of bytes
             * required to hold the length of the next message, so another message
             * is available. */
            ( فراغ ) prvReadBytesFromBuffer( pxStreamBuffer, ( uint8_t * ) &xTempReturn, sbBYTES_TO_STORE_MESSAGE_LENGTH, pxStreamBuffer->xTail );
            xReturn = ( size_t ) xTempReturn;
        }
        وإلا
        {
            /* The minimum amount of bytes in a message buffer is
             * ( sbBYTES_TO_STORE_MESSAGE_LENGTH + 1 ), so if xBytesAvailable is
             * less than sbBYTES_TO_STORE_MESSAGE_LENGTH the only other valid
             * value is 0. */
            configASSERT( xBytesAvailable == 0 );
            xReturn = 0;
        }
    }
    وإلا
    {
        xReturn = 0;
    }

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

size_t xStreamBufferReceiveFromISR( StreamBufferHandle_t xStreamBuffer,
                                    فراغ * pvRxData,
                                    size_t xBufferLengthBytes,
                                    BaseType_t * ثابت pxHigherPriorityTaskWoken )
{
    StreamBuffer_t * ثابت pxStreamBuffer = xStreamBuffer;
    size_t xReceivedLength = 0, xBytesAvailable, xBytesToStoreMessageLength;

    configASSERT( pvRxData );
    configASSERT( pxStreamBuffer );

    /* This receive function is used by both message buffers, which store
     * discrete messages, and stream buffers, which store a continuous stream of
     * bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the
     * message. */
    إذا( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) != ( uint8_t ) 0 )
    {
        xBytesToStoreMessageLength = sbBYTES_TO_STORE_MESSAGE_LENGTH;
    }
    وإلا
    {
        xBytesToStoreMessageLength = 0;
    }

    xBytesAvailable = prvBytesInBuffer( pxStreamBuffer );

    /* Whether receiving a discrete message (where xBytesToStoreMessageLength
     * holds the number of bytes used to store the message length) or a stream of
     * bytes (where xBytesToStoreMessageLength is zero), the number of bytes
     * available must be greater than xBytesToStoreMessageLength to be able to
     * read bytes from the buffer. */
    إذا( xBytesAvailable > xBytesToStoreMessageLength )
    {
        xReceivedLength = prvReadMessageFromBuffer( pxStreamBuffer, pvRxData, xBufferLengthBytes, xBytesAvailable );

        /* Was a task waiting for space in the buffer? */
        إذا( xReceivedLength != ( size_t ) 0 )
        {
            prvRECEIVE_COMPLETED_FROM_ISR( pxStreamBuffer, pxHigherPriorityTaskWoken );
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    traceSTREAM_BUFFER_RECEIVE_FROM_ISR( xStreamBuffer, xReceivedLength );

    أرجع xReceivedLength;
}
/*-----------------------------------------------------------*/

ساكن size_t prvReadMessageFromBuffer( StreamBuffer_t * pxStreamBuffer,
                                        فراغ * pvRxData,
                                        size_t xBufferLengthBytes,
                                        size_t xBytesAvailable )
{
    size_t x_سانى, xNextMessageLength;
    configMESSAGE_BUFFER_LENGTH_TYPE xTempNextMessageLength;
    size_t xNextTail = pxStreamBuffer->xTail;

    إذا( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) != ( uint8_t ) 0 )
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
        إذا( xNextMessageLength > xBufferLengthBytes )
        {
            /* The user has provided insufficient space to read the message. */
            xNextMessageLength = 0;
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    وإلا
    {
        /* A stream of bytes is being received (as opposed to a discrete
         * message), so read as many bytes as possible. */
        xNextMessageLength = xBufferLengthBytes;
    }

    /* Use the minimum of the wanted bytes and the available bytes. */
    x_سانى = configMIN( xNextMessageLength, xBytesAvailable );

    إذا( x_سانى != ( size_t ) 0 )
    {
        /* Read the actual data and update the tail to mark the data as officially consumed. */
        pxStreamBuffer->xTail = prvReadBytesFromBuffer( pxStreamBuffer, ( uint8_t * ) pvRxData, x_سانى, xNextTail ); /*lint !e9079 Data storage area is implemented as uint8_t array for ease of sizing, indexing and alignment. */
    }

    أرجع x_سانى;
}
/*-----------------------------------------------------------*/

BaseType_t xStreamBufferIsEmpty( StreamBufferHandle_t xStreamBuffer )
{
    ثابت StreamBuffer_t * ثابت pxStreamBuffer = xStreamBuffer;
    BaseType_t xReturn;
    size_t xTail;

    configASSERT( pxStreamBuffer );

    /* True if no bytes are available. */
    xTail = pxStreamBuffer->xTail;

    إذا( pxStreamBuffer->xHead == xTail )
    {
        xReturn = pdTRUE;
    }
    وإلا
    {
        xReturn = pdFALSE;
    }

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xStreamBufferIsFull( StreamBufferHandle_t xStreamBuffer )
{
    BaseType_t xReturn;
    size_t xBytesToStoreMessageLength;
    ثابت StreamBuffer_t * ثابت pxStreamBuffer = xStreamBuffer;

    configASSERT( pxStreamBuffer );

    /* This generic version of the receive function is used by both message
     * buffers, which store discrete messages, and stream buffers, which store a
     * continuous stream of bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the message. */
    إذا( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) != ( uint8_t ) 0 )
    {
        xBytesToStoreMessageLength = sbBYTES_TO_STORE_MESSAGE_LENGTH;
    }
    وإلا
    {
        xBytesToStoreMessageLength = 0;
    }

    /* True if the available space equals zero. */
    إذا( xStreamBufferSpacesAvailable( xStreamBuffer ) <= xBytesToStoreMessageLength )
    {
        xReturn = pdTRUE;
    }
    وإلا
    {
        xReturn = pdFALSE;
    }

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xStreamBufferSendCompletedFromISR( StreamBufferHandle_t xStreamBuffer,
                                              BaseType_t * pxHigherPriorityTaskWoken )
{
    StreamBuffer_t * ثابت pxStreamBuffer = xStreamBuffer;
    BaseType_t xReturn;
    UBaseType_t uxSavedInterruptStatus;

    configASSERT( pxStreamBuffer );

    uxSavedInterruptStatus = ( UBaseType_t ) portSET_INTERRUPT_MASK_FROM_ISR();
    {
        إذا( ( pxStreamBuffer )->xTaskWaitingToReceive != عدم )
        {
            ( فراغ ) xTaskNotifyFromISR( ( pxStreamBuffer )->xTaskWaitingToReceive,
                                         ( uint32_t ) 0,
                                         eNoAction,
                                         pxHigherPriorityTaskWoken );
            ( pxStreamBuffer )->xTaskWaitingToReceive = عدم;
            xReturn = pdTRUE;
        }
        وإلا
        {
            xReturn = pdFALSE;
        }
    }
    portCLEAR_INTERRUPT_MASK_FROM_ISR( uxSavedInterruptStatus );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xStreamBufferReceiveCompletedFromISR( StreamBufferHandle_t xStreamBuffer,
                                                 BaseType_t * pxHigherPriorityTaskWoken )
{
    StreamBuffer_t * ثابت pxStreamBuffer = xStreamBuffer;
    BaseType_t xReturn;
    UBaseType_t uxSavedInterruptStatus;

    configASSERT( pxStreamBuffer );

    uxSavedInterruptStatus = ( UBaseType_t ) portSET_INTERRUPT_MASK_FROM_ISR();
    {
        إذا( ( pxStreamBuffer )->xTaskWaitingToSend != عدم )
        {
            ( فراغ ) xTaskNotifyFromISR( ( pxStreamBuffer )->xTaskWaitingToSend,
                                         ( uint32_t ) 0,
                                         eNoAction,
                                         pxHigherPriorityTaskWoken );
            ( pxStreamBuffer )->xTaskWaitingToSend = عدم;
            xReturn = pdTRUE;
        }
        وإلا
        {
            xReturn = pdFALSE;
        }
    }
    portCLEAR_INTERRUPT_MASK_FROM_ISR( uxSavedInterruptStatus );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

ساكن size_t prvWriteBytesToBuffer( StreamBuffer_t * ثابت pxStreamBuffer,
                                     ثابت uint8_t * pucData,
                                     size_t x_سانى,
                                     size_t xHead )
{
    size_t xFirstLength;

    configASSERT( x_سانى > ( size_t ) 0 );

    /* Calculate the number of bytes that can be added in the first write -
     * which may be less than the total number of bytes that need to be added if
     * the buffer will wrap back to the beginning. */
    xFirstLength = configMIN( pxStreamBuffer->xLength - xHead, x_سانى );

    /* Write as many bytes as can be written in the first write. */
    configASSERT( ( xHead + xFirstLength ) <= pxStreamBuffer->xLength );
    ( فراغ ) memcpy( ( فراغ * ) ( &( pxStreamBuffer->pucBuffer[ xHead ] ) ), ( ثابت فراغ * ) pucData, xFirstLength ); /*lint !e9087 memcpy() requires void *. */

    /* If the number of bytes written was less than the number that could be
     * written in the first write... */
    إذا( x_سانى > xFirstLength )
    {
        /* ...then write the remaining bytes to the start of the buffer. */
        configASSERT( ( x_سانى - xFirstLength ) <= pxStreamBuffer->xLength );
        ( فراغ ) memcpy( ( فراغ * ) pxStreamBuffer->pucBuffer, ( ثابت فراغ * ) &( pucData[ xFirstLength ] ), x_سانى - xFirstLength ); /*lint !e9087 memcpy() requires void *. */
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    xHead += x_سانى;

    إذا( xHead >= pxStreamBuffer->xLength )
    {
        xHead -= pxStreamBuffer->xLength;
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    أرجع xHead;
}
/*-----------------------------------------------------------*/

ساكن size_t prvReadBytesFromBuffer( StreamBuffer_t * pxStreamBuffer,
                                      uint8_t * pucData,
                                      size_t x_سانى,
                                      size_t xTail )
{
    size_t xFirstLength;

    configASSERT( x_سانى != ( size_t ) 0 );

    /* Calculate the number of bytes that can be read - which may be
     * less than the number wanted if the data wraps around to the start of
     * the buffer. */
    xFirstLength = configMIN( pxStreamBuffer->xLength - xTail, x_سانى );

    /* Obtain the number of bytes it is possible to obtain in the first
     * read.  Asserts check bounds of read and write. */
    configASSERT( xFirstLength <= x_سانى );
    configASSERT( ( xTail + xFirstLength ) <= pxStreamBuffer->xLength );
    ( فراغ ) memcpy( ( فراغ * ) pucData, ( ثابت فراغ * ) &( pxStreamBuffer->pucBuffer[ xTail ] ), xFirstLength ); /*lint !e9087 memcpy() requires void *. */

    /* If the total number of wanted bytes is greater than the number
     * that could be read in the first read... */
    إذا( x_سانى > xFirstLength )
    {
        /* ...then read the remaining bytes from the start of the buffer. */
        ( فراغ ) memcpy( ( فراغ * ) &( pucData[ xFirstLength ] ), ( فراغ * ) ( pxStreamBuffer->pucBuffer ), x_سانى - xFirstLength ); /*lint !e9087 memcpy() requires void *. */
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    /* Move the tail pointer to effectively remove the data read from the buffer. */
    xTail += x_سانى;

    إذا( xTail >= pxStreamBuffer->xLength )
    {
        xTail -= pxStreamBuffer->xLength;
    }

    أرجع xTail;
}
/*-----------------------------------------------------------*/

ساكن size_t prvBytesInBuffer( ثابت StreamBuffer_t * ثابت pxStreamBuffer )
{
/* Returns the distance between xTail and xHead. */
    size_t x_سانى;

    x_سانى = pxStreamBuffer->xLength + pxStreamBuffer->xHead;
    x_سانى -= pxStreamBuffer->xTail;

    إذا( x_سانى >= pxStreamBuffer->xLength )
    {
        x_سانى -= pxStreamBuffer->xLength;
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    أرجع x_سانى;
}
/*-----------------------------------------------------------*/

ساكن فراغ prvInitialiseNewStreamBuffer( StreamBuffer_t * ثابت pxStreamBuffer,
                                          uint8_t * ثابت pucBuffer,
                                          size_t xBufferSizeBytes,
                                          size_t xTriggerLevelBytes,
                                          uint8_t ucFlags,
                                          StreamBufferCallbackFunction_t pxSendCompletedCallback,
                                          StreamBufferCallbackFunction_t pxReceiveCompletedCallback )
{
    /* Assert here is deliberately writing to the entire buffer to ensure it can
     * be written to without generating exceptions, and is setting the buffer to a
     * known value to assist in development/debugging. */
    #إذا ( configASSERT_DEFINED == 1 )
    {
        /* The value written just has to be identifiable when looking at the
         * memory.  Don't use 0xA5 as that is the stack fill value and could
         * result in confusion as to what is actually being observed. */
        ثابت BaseType_t x_Write_قىممەت = 0x55;
        configASSERT( memset( pucBuffer, ( صحيح ) x_Write_قىممەت, xBufferSizeBytes ) == pucBuffer );
        ( فراغ ) x_Write_قىممەت;
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
    #إذا ( configUSE_TRACE_FACILITY == 1 )
        pxStreamBuffer->ux_Stream_Buffer_سان = ( UBaseType_t ) 0;
    #endif
    pxStreamBuffer->pucBuffer = pucBuffer;
    pxStreamBuffer->xLength = xBufferSizeBytes;
    pxStreamBuffer->xTriggerLevelBytes = xTriggerLevelBytes;
    pxStreamBuffer->ucFlags = ucFlags;
    #إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    {
        pxStreamBuffer->pxSendCompletedCallback = pxSendCompletedCallback;
        pxStreamBuffer->pxReceiveCompletedCallback = pxReceiveCompletedCallback;
    }
    #وإلا
    {
        ( فراغ ) pxSendCompletedCallback;
        ( فراغ ) pxReceiveCompletedCallback;
    }
    #endif
}

#إذا ( configUSE_TRACE_FACILITY == 1 )

    UBaseType_t ux_Stream_Buffer_Get_Stream_Buffer_سان( StreamBufferHandle_t xStreamBuffer )
    {
        أرجع xStreamBuffer->ux_Stream_Buffer_سان;
    }

#endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#إذا ( configUSE_TRACE_FACILITY == 1 )

    فراغ v_Stream_Buffer_Set_Stream_Buffer_سان( StreamBufferHandle_t xStreamBuffer,
                                             UBaseType_t ux_Stream_Buffer_سان )
    {
        xStreamBuffer->ux_Stream_Buffer_سان = ux_Stream_Buffer_سان;
    }

#endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#إذا ( configUSE_TRACE_FACILITY == 1 )

    uint8_t ucStreamBufferGetStreamBufferType( StreamBufferHandle_t xStreamBuffer )
    {
        أرجع( xStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER );
    }

#endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/
