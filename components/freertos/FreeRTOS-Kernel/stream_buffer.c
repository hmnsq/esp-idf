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
#अन्तर्भाव <stdint.h>
#अन्तर्भाव <string.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* FreeRTOS includes. */
#अन्तर्भाव "FreeRTOS.h"
#अन्तर्भाव "task.h"
#अन्तर्भाव "stream_buffer.h"
/* Include private IDF API additions for critical thread safety macros */
#अन्तर्भाव "esp_private/freertos_idf_additions_priv.h"

#जर ( configUSE_TASK_NOTIFICATIONS != 1 )
    #error configUSE_TASK_NOTIFICATIONS must be set to 1 to build stream_buffer.c
#endif

#जर ( INCLUDE_xTaskGetCurrentTaskHandle != 1 )
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
        जर( ( pxStreamBuffer )->xTaskWaitingToSend != रिक्त )                    \
        {                                                                       \
            ( शून्य ) xTaskNotify( ( pxStreamBuffer )->xTaskWaitingToSend,       \
                                  ( uint32_t ) 0,                               \
                                  eNoAction );                                  \
            ( pxStreamBuffer )->xTaskWaitingToSend = रिक्त;                      \
        }                                                                       \
    }                                                                           \
    ( शून्य ) prvEXIT_CRITICAL_OR_RESUME_ALL( &( pxStreamBuffer->xStreamBufferLock ) );
#endif /* sbRECEIVE_COMPLETED */

/* If user has provided a per-instance receive complete callback, then
 * invoke the callback else use the receive complete macro which is provided by default for all instances.
 */
#जर ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #define prvRECEIVE_COMPLETED( pxStreamBuffer )                                               \
    {                                                                                            \
        जर( ( pxStreamBuffer )->pxReceiveCompletedCallback != रिक्त )                             \
        {                                                                                        \
            ( pxStreamBuffer )->pxReceiveCompletedCallback( ( pxStreamBuffer ), pdFALSE, रिक्त ); \
        }                                                                                        \
        अन्यथा                                                                                     \
        {                                                                                        \
            sbRECEIVE_COMPLETED( ( pxStreamBuffer ) );                                           \
        }                                                                                        \
    }
#अन्यथा /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
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
            जर( ( pxStreamBuffer )->xTaskWaitingToSend != रिक्त )                     \
            {                                                                        \
                ( शून्य ) xTaskNotifyFromISR( ( pxStreamBuffer )->xTaskWaitingToSend, \
                                             ( uint32_t ) 0,                         \
                                             eNoAction,                              \
                                             ( pxHigherPriorityTaskWoken ) );        \
                ( pxStreamBuffer )->xTaskWaitingToSend = रिक्त;                       \
            }                                                                        \
        }                                                                            \
        portCLEAR_INTERRUPT_MASK_FROM_ISR( uxSavedInterruptStatus );                 \
    }
#endif /* sbRECEIVE_COMPLETED_FROM_ISR */

#जर ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #define prvRECEIVE_COMPLETED_FROM_ISR( pxStreamBuffer,                                                               \
                                           pxHigherPriorityTaskWoken )                                                   \
    {                                                                                                                    \
        जर( ( pxStreamBuffer )->pxReceiveCompletedCallback != रिक्त )                                                     \
        {                                                                                                                \
            ( pxStreamBuffer )->pxReceiveCompletedCallback( ( pxStreamBuffer ), pdTRUE, ( pxHigherPriorityTaskWoken ) ); \
        }                                                                                                                \
        अन्यथा                                                                                                             \
        {                                                                                                                \
            sbRECEIVE_COMPLETED_FROM_ISR( ( pxStreamBuffer ), ( pxHigherPriorityTaskWoken ) );                           \
        }                                                                                                                \
    }
#अन्यथा /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
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
        जर( ( pxStreamBuffer )->xTaskWaitingToReceive != रिक्त )                 \
        {                                                                       \
            ( शून्य ) xTaskNotify( ( pxStreamBuffer )->xTaskWaitingToReceive,    \
                                  ( uint32_t ) 0,                               \
                                  eNoAction );                                  \
            ( pxStreamBuffer )->xTaskWaitingToReceive = रिक्त;                   \
        }                                                                       \
    }                                                                           \
    ( शून्य ) prvEXIT_CRITICAL_OR_RESUME_ALL( &( pxStreamBuffer->xStreamBufferLock ) );
#endif /* sbSEND_COMPLETED */

/* If user has provided a per-instance send completed callback, then
 * invoke the callback else use the send complete macro which is provided by default for all instances.
 */
#जर ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #define prvSEND_COMPLETED( pxStreamBuffer )                                           \
    {                                                                                     \
        जर( ( pxStreamBuffer )->pxSendCompletedCallback != रिक्त )                         \
        {                                                                                 \
            pxStreamBuffer->pxSendCompletedCallback( ( pxStreamBuffer ), pdFALSE, रिक्त ); \
        }                                                                                 \
        अन्यथा                                                                              \
        {                                                                                 \
            sbSEND_COMPLETED( ( pxStreamBuffer ) );                                       \
        }                                                                                 \
    }
#अन्यथा /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #define prvSEND_COMPLETED( pxStreamBuffer )    sbSEND_COMPLETED( ( pxStreamBuffer ) )
#endif /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */


#ifndef sbSEND_COMPLETE_FROM_ISR
    #define sbSEND_COMPLETE_FROM_ISR( pxStreamBuffer, pxHigherPriorityTaskWoken )       \
    {                                                                                   \
        UBaseType_t uxSavedInterruptStatus;                                             \
                                                                                        \
        uxSavedInterruptStatus = ( UBaseType_t ) portSET_INTERRUPT_MASK_FROM_ISR();     \
        {                                                                               \
            जर( ( pxStreamBuffer )->xTaskWaitingToReceive != रिक्त )                     \
            {                                                                           \
                ( शून्य ) xTaskNotifyFromISR( ( pxStreamBuffer )->xTaskWaitingToReceive, \
                                             ( uint32_t ) 0,                            \
                                             eNoAction,                                 \
                                             ( pxHigherPriorityTaskWoken ) );           \
                ( pxStreamBuffer )->xTaskWaitingToReceive = रिक्त;                       \
            }                                                                           \
        }                                                                               \
        portCLEAR_INTERRUPT_MASK_FROM_ISR( uxSavedInterruptStatus );                    \
    }
#endif /* sbSEND_COMPLETE_FROM_ISR */


#जर ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #define prvSEND_COMPLETE_FROM_ISR( pxStreamBuffer, pxHigherPriorityTaskWoken )                                    \
    {                                                                                                                 \
        जर( ( pxStreamBuffer )->pxSendCompletedCallback != रिक्त )                                                     \
        {                                                                                                             \
            ( pxStreamBuffer )->pxSendCompletedCallback( ( pxStreamBuffer ), pdTRUE, ( pxHigherPriorityTaskWoken ) ); \
        }                                                                                                             \
        अन्यथा                                                                                                          \
        {                                                                                                             \
            sbSEND_COMPLETE_FROM_ISR( ( pxStreamBuffer ), ( pxHigherPriorityTaskWoken ) );                            \
        }                                                                                                             \
    }
#अन्यथा /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #define prvSEND_COMPLETE_FROM_ISR( pxStreamBuffer, pxHigherPriorityTaskWoken ) \
    sbSEND_COMPLETE_FROM_ISR( ( pxStreamBuffer ), ( pxHigherPriorityTaskWoken ) )
#endif /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

/*lint -restore (9026) */

/* The number of bytes used to hold the length of a message in the buffer. */
#define sbBYTES_TO_STORE_MESSAGE_LENGTH    ( आकार( configMESSAGE_BUFFER_LENGTH_TYPE ) )

/* Bits stored in the ucFlags field of the stream buffer. */
#define sbFLAGS_IS_MESSAGE_BUFFER          ( ( uint8_t ) 1 ) /* Set if the stream buffer was created as a message buffer, in which case it holds discrete messages rather than a stream. */
#define sbFLAGS_IS_STATICALLY_ALLOCATED    ( ( uint8_t ) 2 ) /* Set if the stream buffer was created using statically allocated memory. */

/*-----------------------------------------------------------*/

/* Structure that hold state information on the buffer. */
प्रकारव्याख्या संरचना StreamBufferDef_t                 /*lint !e9058 Style convention uses tag. */
{
    अस्थिर माप_प्रकार xTail;                       /* Index to the next item to read within the buffer. */
    अस्थिर माप_प्रकार xHead;                       /* Index to the next item to write within the buffer. */
    माप_प्रकार xLength;                              /* The length of the buffer pointed to by pucBuffer. */
    माप_प्रकार xTriggerLevelBytes;                   /* The number of bytes that must be in the stream buffer before a task that is waiting for data is unblocked. */
    अस्थिर TaskHandle_t xTaskWaitingToReceive; /* Holds the handle of a task waiting for data, or NULL if no tasks are waiting. */
    अस्थिर TaskHandle_t xTaskWaitingToSend;    /* Holds the handle of a task waiting to send data to a message buffer that is full. */
    uint8_t * pucBuffer;                         /* Points to the buffer itself - that is - the RAM that stores the data passed through the buffer. */
    uint8_t ucFlags;

    #जर ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_Stream_Buffer_संख्या; /* Used for tracing purposes. */
    #endif

    #जर ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        StreamBufferCallbackFunction_t pxSendCompletedCallback;    /* Optional callback called on send complete. sbSEND_COMPLETED is called if this is NULL. */
        StreamBufferCallbackFunction_t pxReceiveCompletedCallback; /* Optional callback called on receive complete.  sbRECEIVE_COMPLETED is called if this is NULL. */
    #endif

    portMUX_TYPE xStreamBufferLock; /* Spinlock required for SMP critical sections */
} StreamBuffer_t;

/*
 * The number of bytes available to be read from the buffer.
 */
स्थिर माप_प्रकार prvBytesInBuffer( स्थिरांक StreamBuffer_t * स्थिरांक pxStreamBuffer ) PRIVILEGED_FUNCTION;

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
स्थिर माप_प्रकार prvWriteBytesToBuffer( StreamBuffer_t * स्थिरांक pxStreamBuffer,
                                     स्थिरांक uint8_t * pucData,
                                     माप_प्रकार x_मोजणी,
                                     माप_प्रकार xHead ) PRIVILEGED_FUNCTION;

/*
 * If the stream buffer is being used as a message buffer, then reads an entire
 * message out of the buffer.  If the stream buffer is being used as a stream
 * buffer then read as many bytes as possible from the buffer.
 * prvReadBytesFromBuffer() is called to actually extract the bytes from the
 * buffer's data storage area.
 */
स्थिर माप_प्रकार prvReadMessageFromBuffer( StreamBuffer_t * pxStreamBuffer,
                                        शून्य * pvRxData,
                                        माप_प्रकार xBufferLengthBytes,
                                        माप_प्रकार xBytesAvailable ) PRIVILEGED_FUNCTION;

/*
 * If the stream buffer is being used as a message buffer, then writes an entire
 * message to the buffer.  If the stream buffer is being used as a stream
 * buffer then write as many bytes as possible to the buffer.
 * prvWriteBytestoBuffer() is called to actually send the bytes to the buffer's
 * data storage area.
 */
स्थिर माप_प्रकार prvWriteMessageToBuffer( StreamBuffer_t * स्थिरांक pxStreamBuffer,
                                       स्थिरांक शून्य * pvTxData,
                                       माप_प्रकार xDataLengthBytes,
                                       माप_प्रकार xSpace,
                                       माप_प्रकार xRequiredSpace ) PRIVILEGED_FUNCTION;

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
स्थिर माप_प्रकार prvReadBytesFromBuffer( StreamBuffer_t * pxStreamBuffer,
                                      uint8_t * pucData,
                                      माप_प्रकार x_मोजणी,
                                      माप_प्रकार xTail ) PRIVILEGED_FUNCTION;

/*
 * Called by both pxStreamBufferCreate() and pxStreamBufferCreateStatic() to
 * initialise the members of the newly created stream buffer structure.
 */
स्थिर शून्य prvInitialiseNewStreamBuffer( StreamBuffer_t * स्थिरांक pxStreamBuffer,
                                          uint8_t * स्थिरांक pucBuffer,
                                          माप_प्रकार xBufferSizeBytes,
                                          माप_प्रकार xTriggerLevelBytes,
                                          uint8_t ucFlags,
                                          StreamBufferCallbackFunction_t pxSendCompletedCallback,
                                          StreamBufferCallbackFunction_t pxReceiveCompletedCallback ) PRIVILEGED_FUNCTION;

/*-----------------------------------------------------------*/
#जर ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
    StreamBufferHandle_t xStreamBufferGenericCreate( माप_प्रकार xBufferSizeBytes,
                                                     माप_प्रकार xTriggerLevelBytes,
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
        जर( xIsMessageBuffer == pdTRUE )
        {
            /* Is a message buffer but not statically allocated. */
            ucFlags = sbFLAGS_IS_MESSAGE_BUFFER;
            configASSERT( xBufferSizeBytes > sbBYTES_TO_STORE_MESSAGE_LENGTH );
        }
        अन्यथा
        {
            /* Not a message buffer and not statically allocated. */
            ucFlags = 0;
            configASSERT( xBufferSizeBytes > 0 );
        }

        configASSERT( xTriggerLevelBytes <= xBufferSizeBytes );

        /* A trigger level of 0 would cause a waiting task to unblock even when
         * the buffer was empty. */
        जर( xTriggerLevelBytes == ( माप_प्रकार ) 0 )
        {
            xTriggerLevelBytes = ( माप_प्रकार ) 1;
        }

        /* A stream buffer requires a StreamBuffer_t structure and a buffer.
         * Both are allocated in a single call to pvPortMalloc().  The
         * StreamBuffer_t structure is placed at the start of the allocated memory
         * and the buffer follows immediately after.  The requested size is
         * incremented so the free space is returned as the user would expect -
         * this is a quirk of the implementation that means otherwise the free
         * space would be reported as one byte smaller than would be logically
         * expected. */
        जर( xBufferSizeBytes < ( xBufferSizeBytes + 1 + आकार( StreamBuffer_t ) ) )
        {
            xBufferSizeBytes++;
            pucAllocatedMemory = ( uint8_t * ) pvPortMalloc( xBufferSizeBytes + आकार( StreamBuffer_t ) ); /*lint !e9079 malloc() only returns void*. */
        }
        अन्यथा
        {
            pucAllocatedMemory = रिक्त;
        }

        जर( pucAllocatedMemory != रिक्त )
        {
            prvInitialiseNewStreamBuffer( ( StreamBuffer_t * ) pucAllocatedMemory,       /* Structure at the start of the allocated memory. */ /*lint !e9087 Safe cast as allocated memory is aligned. */ /*lint !e826 Area is not too small and alignment is guaranteed provided malloc() behaves as expected and returns aligned buffer. */
                                          pucAllocatedMemory + आकार( StreamBuffer_t ), /* Storage area follows. */ /*lint !e9016 Indexing past structure valid for uint8_t pointer, also storage area has no alignment requirement. */
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
        अन्यथा
        {
            traceSTREAM_BUFFER_CREATE_FAILED( xIsMessageBuffer );
        }

        परत ( StreamBufferHandle_t ) pucAllocatedMemory; /*lint !e9087 !e826 Safe cast as allocated memory is aligned. */
    }
#endif /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

#जर ( configSUPPORT_STATIC_ALLOCATION == 1 )

    StreamBufferHandle_t xStreamBufferGenericCreateStatic( माप_प्रकार xBufferSizeBytes,
                                                           माप_प्रकार xTriggerLevelBytes,
                                                           BaseType_t xIsMessageBuffer,
                                                           uint8_t * स्थिरांक pucStreamBufferStorageArea,
                                                           StaticStreamBuffer_t * स्थिरांक pxStaticStreamBuffer,
                                                           StreamBufferCallbackFunction_t pxSendCompletedCallback,
                                                           StreamBufferCallbackFunction_t pxReceiveCompletedCallback )
    {
        StreamBuffer_t * स्थिरांक pxStreamBuffer = ( StreamBuffer_t * ) pxStaticStreamBuffer; /*lint !e740 !e9087 Safe cast as StaticStreamBuffer_t is opaque Streambuffer_t. */
        StreamBufferHandle_t xReturn;
        uint8_t ucFlags;

        configASSERT( pucStreamBufferStorageArea );
        configASSERT( pxStaticStreamBuffer );
        configASSERT( xTriggerLevelBytes <= xBufferSizeBytes );

        /* A trigger level of 0 would cause a waiting task to unblock even when
         * the buffer was empty. */
        जर( xTriggerLevelBytes == ( माप_प्रकार ) 0 )
        {
            xTriggerLevelBytes = ( माप_प्रकार ) 1;
        }

        जर( xIsMessageBuffer != pdFALSE )
        {
            /* Statically allocated message buffer. */
            ucFlags = sbFLAGS_IS_MESSAGE_BUFFER | sbFLAGS_IS_STATICALLY_ALLOCATED;
        }
        अन्यथा
        {
            /* Statically allocated stream buffer. */
            ucFlags = sbFLAGS_IS_STATICALLY_ALLOCATED;
        }

        /* In case the stream buffer is going to be used as a message buffer
         * (that is, it will hold discrete messages with a little meta data that
         * says how big the next message is) check the buffer will be large enough
         * to hold at least one message. */
        configASSERT( xBufferSizeBytes > sbBYTES_TO_STORE_MESSAGE_LENGTH );

        #जर ( configASSERT_DEFINED == 1 )
        {
            /* Sanity check that the size of the structure used to declare a
             * variable of type StaticStreamBuffer_t equals the size of the real
             * message buffer structure. */
            अस्थिर माप_प्रकार xSize = आकार( StaticStreamBuffer_t );
            configASSERT( xSize == आकार( StreamBuffer_t ) );
            ( शून्य ) xSize; /* Prevent unused variable warning when configASSERT() is not used. */
        } /*lint !e529 xSize is referenced is configASSERT() is defined. */
        #endif /* configASSERT_DEFINED */

        जर( ( pucStreamBufferStorageArea != रिक्त ) && ( pxStaticStreamBuffer != रिक्त ) )
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
        अन्यथा
        {
            xReturn = रिक्त;
            traceSTREAM_BUFFER_CREATE_STATIC_FAILED( xReturn, xIsMessageBuffer );
        }

        परत xReturn;
    }
#endif /* ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*-----------------------------------------------------------*/

#जर ( configSUPPORT_STATIC_ALLOCATION == 1 )
    BaseType_t xStreamBufferGetStaticBuffers( StreamBufferHandle_t xStreamBuffer,
                                              uint8_t ** ppucStreamBufferStorageArea,
                                              StaticStreamBuffer_t ** ppxStaticStreamBuffer )
    {
        BaseType_t xReturn;
        स्थिरांक StreamBuffer_t * स्थिरांक pxStreamBuffer = xStreamBuffer;

        configASSERT( pxStreamBuffer );
        configASSERT( ppucStreamBufferStorageArea );
        configASSERT( ppxStaticStreamBuffer );

        जर( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_STATICALLY_ALLOCATED ) != ( uint8_t ) 0 )
        {
            *ppucStreamBufferStorageArea = pxStreamBuffer->pucBuffer;
            *ppxStaticStreamBuffer = ( StaticStreamBuffer_t * ) pxStreamBuffer;
            xReturn = pdTRUE;
        }
        अन्यथा
        {
            xReturn = pdFALSE;
        }

        परत xReturn;
    }
#endif /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

शून्य vStreamBufferDelete( StreamBufferHandle_t xStreamBuffer )
{
    StreamBuffer_t * pxStreamBuffer = xStreamBuffer;

    configASSERT( pxStreamBuffer );

    traceSTREAM_BUFFER_DELETE( xStreamBuffer );

    जर( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_STATICALLY_ALLOCATED ) == ( uint8_t ) pdFALSE )
    {
        #जर ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
        {
            /* Both the structure and the buffer were allocated using a single call
            * to pvPortMalloc(), hence only one call to vPortFree() is required. */
            vPortFree( ( शून्य * ) pxStreamBuffer ); /*lint !e9087 Standard free() semantics require void *, plus pxStreamBuffer was allocated by pvPortMalloc(). */
        }
        #अन्यथा
        {
            /* Should not be possible to get here, ucFlags must be corrupt.
             * Force an assert. */
            configASSERT( xStreamBuffer == ( StreamBufferHandle_t ) ~0 );
        }
        #endif
    }
    अन्यथा
    {
        /* The structure and buffer were not allocated dynamically and cannot be
         * freed - just scrub the structure so future use will assert. */
        ( शून्य ) स_रखो( pxStreamBuffer, 0x00, आकार( StreamBuffer_t ) );
    }
}
/*-----------------------------------------------------------*/

BaseType_t xStreamBufferReset( StreamBufferHandle_t xStreamBuffer )
{
    StreamBuffer_t * स्थिरांक pxStreamBuffer = xStreamBuffer;
    BaseType_t xReturn = pdFAIL;
    StreamBufferCallbackFunction_t pxSendCallback = रिक्त, pxReceiveCallback = रिक्त;

    #जर ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_Stream_Buffer_संख्या;
    #endif

    configASSERT( pxStreamBuffer );

    #जर ( configUSE_TRACE_FACILITY == 1 )
    {
        /* Store the stream buffer number so it can be restored after the
         * reset. */
        ux_Stream_Buffer_संख्या = pxStreamBuffer->ux_Stream_Buffer_संख्या;
    }
    #endif

    /* Can only reset a message buffer if there are no tasks blocked on it. */
    taskENTER_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );
    {
        जर( ( pxStreamBuffer->xTaskWaitingToReceive == रिक्त ) && ( pxStreamBuffer->xTaskWaitingToSend == रिक्त ) )
        {
            #जर ( configUSE_SB_COMPLETED_CALLBACK == 1 )
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

            #जर ( configUSE_TRACE_FACILITY == 1 )
            {
                pxStreamBuffer->ux_Stream_Buffer_संख्या = ux_Stream_Buffer_संख्या;
            }
            #endif

            traceSTREAM_BUFFER_RESET( xStreamBuffer );

            xReturn = pdPASS;
        }
    }
    taskEXIT_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );

    परत xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xStreamBufferSetTriggerLevel( StreamBufferHandle_t xStreamBuffer,
                                         माप_प्रकार xTriggerLevel )
{
    StreamBuffer_t * स्थिरांक pxStreamBuffer = xStreamBuffer;
    BaseType_t xReturn;

    configASSERT( pxStreamBuffer );

    /* It is not valid for the trigger level to be 0. */
    जर( xTriggerLevel == ( माप_प्रकार ) 0 )
    {
        xTriggerLevel = ( माप_प्रकार ) 1;
    }

    /* The trigger level is the number of bytes that must be in the stream
     * buffer before a task that is waiting for data is unblocked. */
    जर( xTriggerLevel < pxStreamBuffer->xLength )
    {
        pxStreamBuffer->xTriggerLevelBytes = xTriggerLevel;
        xReturn = pdPASS;
    }
    अन्यथा
    {
        xReturn = pdFALSE;
    }

    परत xReturn;
}
/*-----------------------------------------------------------*/

माप_प्रकार xStreamBufferSpacesAvailable( StreamBufferHandle_t xStreamBuffer )
{
    स्थिरांक StreamBuffer_t * स्थिरांक pxStreamBuffer = xStreamBuffer;
    माप_प्रकार xSpace;
    माप_प्रकार xOriginalTail;

    configASSERT( pxStreamBuffer );

    /* The code below reads xTail and then xHead.  This is safe if the stream
     * buffer is updated once between the two reads - but not if the stream buffer
     * is updated more than once between the two reads - hence the loop. */
    करा
    {
        xOriginalTail = pxStreamBuffer->xTail;
        xSpace = pxStreamBuffer->xLength + pxStreamBuffer->xTail;
        xSpace -= pxStreamBuffer->xHead;
    } पर्यंत( xOriginalTail != pxStreamBuffer->xTail );

    xSpace -= ( माप_प्रकार ) 1;

    जर( xSpace >= pxStreamBuffer->xLength )
    {
        xSpace -= pxStreamBuffer->xLength;
    }
    अन्यथा
    {
        mtCOVERAGE_TEST_MARKER();
    }

    परत xSpace;
}
/*-----------------------------------------------------------*/

माप_प्रकार xStreamBufferBytesAvailable( StreamBufferHandle_t xStreamBuffer )
{
    स्थिरांक StreamBuffer_t * स्थिरांक pxStreamBuffer = xStreamBuffer;
    माप_प्रकार xReturn;

    configASSERT( pxStreamBuffer );

    xReturn = prvBytesInBuffer( pxStreamBuffer );
    परत xReturn;
}
/*-----------------------------------------------------------*/

माप_प्रकार xStreamBufferSend( StreamBufferHandle_t xStreamBuffer,
                          स्थिरांक शून्य * pvTxData,
                          माप_प्रकार xDataLengthBytes,
                          TickType_t xTicksToWait )
{
    StreamBuffer_t * स्थिरांक pxStreamBuffer = xStreamBuffer;
    माप_प्रकार xReturn, xSpace = 0;
    माप_प्रकार xRequiredSpace = xDataLengthBytes;
    TimeOut_t xTimeOut;
    माप_प्रकार xMaxReportedSpace = 0;

    configASSERT( pvTxData );
    configASSERT( pxStreamBuffer );

    /* The maximum amount of space a stream buffer will ever report is its length
     * minus 1. */
    xMaxReportedSpace = pxStreamBuffer->xLength - ( माप_प्रकार ) 1;

    /* This send function is used to write to both message buffers and stream
     * buffers.  If this is a message buffer then the space needed must be
     * increased by the amount of bytes needed to store the length of the
     * message. */
    जर( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) != ( uint8_t ) 0 )
    {
        xRequiredSpace += sbBYTES_TO_STORE_MESSAGE_LENGTH;

        /* Overflow? */
        configASSERT( xRequiredSpace > xDataLengthBytes );

        /* If this is a message buffer then it must be possible to write the
         * whole message. */
        जर( xRequiredSpace > xMaxReportedSpace )
        {
            /* The message would not fit even if the entire buffer was empty,
             * so don't wait for space. */
            xTicksToWait = ( TickType_t ) 0;
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    अन्यथा
    {
        /* If this is a stream buffer then it is acceptable to write only part
         * of the message to the buffer.  Cap the length to the total length of
         * the buffer. */
        जर( xRequiredSpace > xMaxReportedSpace )
        {
            xRequiredSpace = xMaxReportedSpace;
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }

    जर( xTicksToWait != ( TickType_t ) 0 )
    {
        vTaskSetTimeOutState( &xTimeOut );

        करा
        {
            /* Wait until the required number of bytes are free in the message
             * buffer. */
            taskENTER_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );
            {
                xSpace = xStreamBufferSpacesAvailable( pxStreamBuffer );

                जर( xSpace < xRequiredSpace )
                {
                    /* Clear notification state as going to wait for space. */
                    ( शून्य ) xTaskNotifyStateClear( रिक्त );

                    /* Should only be one writer. */
                    configASSERT( pxStreamBuffer->xTaskWaitingToSend == रिक्त );
                    pxStreamBuffer->xTaskWaitingToSend = xTaskGetCurrentTaskHandle();
                }
                अन्यथा
                {
                    taskEXIT_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );
                    थांबा;
                }
            }
            taskEXIT_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );

            traceBLOCKING_ON_STREAM_BUFFER_SEND( xStreamBuffer );
            ( शून्य ) xTaskNotifyWait( ( uint32_t ) 0, ( uint32_t ) 0, रिक्त, xTicksToWait );
            /* In SMP mode, the task may have been woken and scheduled on
             * another core. Hence, we must clear the xTaskWaitingToSend
             * handle in a critical section. */
            #जर ( config_संख्या_OF_CORES > 1 )
                taskENTER_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );
            #endif /* configNUMBER_OF_CORES > 1 */
            {
                pxStreamBuffer->xTaskWaitingToSend = रिक्त;
            }
            #जर ( config_संख्या_OF_CORES > 1 )
                taskEXIT_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );
            #endif /* configNUMBER_OF_CORES > 1 */
        } पर्यंत( xTaskCheckForTimeOut( &xTimeOut, &xTicksToWait ) == pdFALSE );
    }
    अन्यथा
    {
        mtCOVERAGE_TEST_MARKER();
    }

    जर( xSpace == ( माप_प्रकार ) 0 )
    {
        xSpace = xStreamBufferSpacesAvailable( pxStreamBuffer );
    }
    अन्यथा
    {
        mtCOVERAGE_TEST_MARKER();
    }

    xReturn = prvWriteMessageToBuffer( pxStreamBuffer, pvTxData, xDataLengthBytes, xSpace, xRequiredSpace );

    जर( xReturn > ( माप_प्रकार ) 0 )
    {
        traceSTREAM_BUFFER_SEND( xStreamBuffer, xReturn );

        /* Was a task waiting for the data? */
        जर( prvBytesInBuffer( pxStreamBuffer ) >= pxStreamBuffer->xTriggerLevelBytes )
        {
            prvSEND_COMPLETED( pxStreamBuffer );
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    अन्यथा
    {
        mtCOVERAGE_TEST_MARKER();
        traceSTREAM_BUFFER_SEND_FAILED( xStreamBuffer );
    }

    परत xReturn;
}
/*-----------------------------------------------------------*/

माप_प्रकार xStreamBufferSendFromISR( StreamBufferHandle_t xStreamBuffer,
                                 स्थिरांक शून्य * pvTxData,
                                 माप_प्रकार xDataLengthBytes,
                                 BaseType_t * स्थिरांक pxHigherPriorityTaskWoken )
{
    StreamBuffer_t * स्थिरांक pxStreamBuffer = xStreamBuffer;
    माप_प्रकार xReturn, xSpace;
    माप_प्रकार xRequiredSpace = xDataLengthBytes;

    configASSERT( pvTxData );
    configASSERT( pxStreamBuffer );

    /* This send function is used to write to both message buffers and stream
     * buffers.  If this is a message buffer then the space needed must be
     * increased by the amount of bytes needed to store the length of the
     * message. */
    जर( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) != ( uint8_t ) 0 )
    {
        xRequiredSpace += sbBYTES_TO_STORE_MESSAGE_LENGTH;
    }
    अन्यथा
    {
        mtCOVERAGE_TEST_MARKER();
    }

    xSpace = xStreamBufferSpacesAvailable( pxStreamBuffer );
    xReturn = prvWriteMessageToBuffer( pxStreamBuffer, pvTxData, xDataLengthBytes, xSpace, xRequiredSpace );

    जर( xReturn > ( माप_प्रकार ) 0 )
    {
        /* Was a task waiting for the data? */
        जर( prvBytesInBuffer( pxStreamBuffer ) >= pxStreamBuffer->xTriggerLevelBytes )
        {
            prvSEND_COMPLETE_FROM_ISR( pxStreamBuffer, pxHigherPriorityTaskWoken );
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    अन्यथा
    {
        mtCOVERAGE_TEST_MARKER();
    }

    traceSTREAM_BUFFER_SEND_FROM_ISR( xStreamBuffer, xReturn );

    परत xReturn;
}
/*-----------------------------------------------------------*/

स्थिर माप_प्रकार prvWriteMessageToBuffer( StreamBuffer_t * स्थिरांक pxStreamBuffer,
                                       स्थिरांक शून्य * pvTxData,
                                       माप_प्रकार xDataLengthBytes,
                                       माप_प्रकार xSpace,
                                       माप_प्रकार xRequiredSpace )
{
    माप_प्रकार xNextHead = pxStreamBuffer->xHead;
    configMESSAGE_BUFFER_LENGTH_TYPE xMessageLength;

    जर( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) != ( uint8_t ) 0 )
    {
        /* This is a message buffer, as opposed to a stream buffer. */

        /* Convert xDataLengthBytes to the message length type. */
        xMessageLength = ( configMESSAGE_BUFFER_LENGTH_TYPE ) xDataLengthBytes;

        /* Ensure the data length given fits within configMESSAGE_BUFFER_LENGTH_TYPE. */
        configASSERT( ( माप_प्रकार ) xMessageLength == xDataLengthBytes );

        जर( xSpace >= xRequiredSpace )
        {
            /* There is enough space to write both the message length and the message
             * itself into the buffer.  Start by writing the length of the data, the data
             * itself will be written later in this function. */
            xNextHead = prvWriteBytesToBuffer( pxStreamBuffer, ( स्थिरांक uint8_t * ) &( xMessageLength ), sbBYTES_TO_STORE_MESSAGE_LENGTH, xNextHead );
        }
        अन्यथा
        {
            /* Not enough space, so do not write data to the buffer. */
            xDataLengthBytes = 0;
        }
    }
    अन्यथा
    {
        /* This is a stream buffer, as opposed to a message buffer, so writing a
         * stream of bytes rather than discrete messages.  Plan to write as many
         * bytes as possible. */
        xDataLengthBytes = configMIN( xDataLengthBytes, xSpace );
    }

    जर( xDataLengthBytes != ( माप_प्रकार ) 0 )
    {
        /* Write the data to the buffer. */
        pxStreamBuffer->xHead = prvWriteBytesToBuffer( pxStreamBuffer, ( स्थिरांक uint8_t * ) pvTxData, xDataLengthBytes, xNextHead ); /*lint !e9079 Storage buffer is implemented as uint8_t for ease of sizing, alignment and access. */
    }

    परत xDataLengthBytes;
}
/*-----------------------------------------------------------*/

माप_प्रकार xStreamBufferReceive( StreamBufferHandle_t xStreamBuffer,
                             शून्य * pvRxData,
                             माप_प्रकार xBufferLengthBytes,
                             TickType_t xTicksToWait )
{
    StreamBuffer_t * स्थिरांक pxStreamBuffer = xStreamBuffer;
    माप_प्रकार xReceivedLength = 0, xBytesAvailable, xBytesToStoreMessageLength;

    configASSERT( pvRxData );
    configASSERT( pxStreamBuffer );

    /* This receive function is used by both message buffers, which store
     * discrete messages, and stream buffers, which store a continuous stream of
     * bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the
     * message. */
    जर( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) != ( uint8_t ) 0 )
    {
        xBytesToStoreMessageLength = sbBYTES_TO_STORE_MESSAGE_LENGTH;
    }
    अन्यथा
    {
        xBytesToStoreMessageLength = 0;
    }

    जर( xTicksToWait != ( TickType_t ) 0 )
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
            जर( xBytesAvailable <= xBytesToStoreMessageLength )
            {
                /* Clear notification state as going to wait for data. */
                ( शून्य ) xTaskNotifyStateClear( रिक्त );

                /* Should only be one reader. */
                configASSERT( pxStreamBuffer->xTaskWaitingToReceive == रिक्त );
                pxStreamBuffer->xTaskWaitingToReceive = xTaskGetCurrentTaskHandle();
            }
            अन्यथा
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        taskEXIT_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );

        जर( xBytesAvailable <= xBytesToStoreMessageLength )
        {
            /* Wait for data to be available. */
            traceBLOCKING_ON_STREAM_BUFFER_RECEIVE( xStreamBuffer );
            ( शून्य ) xTaskNotifyWait( ( uint32_t ) 0, ( uint32_t ) 0, रिक्त, xTicksToWait );

            /* In SMP mode, the task may have been woken and scheduled on
             * another core. Hence, we must clear the xTaskWaitingToReceive
             * handle in a critical section. */
            #जर ( config_संख्या_OF_CORES > 1 )
                taskENTER_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );
            #endif /* configNUMBER_OF_CORES > 1 */
            {
                pxStreamBuffer->xTaskWaitingToReceive = रिक्त;
            }
            #जर ( config_संख्या_OF_CORES > 1 )
                taskEXIT_CRITICAL( &( pxStreamBuffer->xStreamBufferLock ) );
            #endif /* configNUMBER_OF_CORES > 1 */

            /* Recheck the data available after blocking. */
            xBytesAvailable = prvBytesInBuffer( pxStreamBuffer );
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    अन्यथा
    {
        xBytesAvailable = prvBytesInBuffer( pxStreamBuffer );
    }

    /* Whether receiving a discrete message (where xBytesToStoreMessageLength
     * holds the number of bytes used to store the message length) or a stream of
     * bytes (where xBytesToStoreMessageLength is zero), the number of bytes
     * available must be greater than xBytesToStoreMessageLength to be able to
     * read bytes from the buffer. */
    जर( xBytesAvailable > xBytesToStoreMessageLength )
    {
        xReceivedLength = prvReadMessageFromBuffer( pxStreamBuffer, pvRxData, xBufferLengthBytes, xBytesAvailable );

        /* Was a task waiting for space in the buffer? */
        जर( xReceivedLength != ( माप_प्रकार ) 0 )
        {
            traceSTREAM_BUFFER_RECEIVE( xStreamBuffer, xReceivedLength );
            prvRECEIVE_COMPLETED( xStreamBuffer );
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    अन्यथा
    {
        traceSTREAM_BUFFER_RECEIVE_FAILED( xStreamBuffer );
        mtCOVERAGE_TEST_MARKER();
    }

    परत xReceivedLength;
}
/*-----------------------------------------------------------*/

माप_प्रकार xStreamBufferNextMessageLengthBytes( StreamBufferHandle_t xStreamBuffer )
{
    StreamBuffer_t * स्थिरांक pxStreamBuffer = xStreamBuffer;
    माप_प्रकार xReturn, xBytesAvailable;
    configMESSAGE_BUFFER_LENGTH_TYPE xTempReturn;

    configASSERT( pxStreamBuffer );

    /* Ensure the stream buffer is being used as a message buffer. */
    जर( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) != ( uint8_t ) 0 )
    {
        xBytesAvailable = prvBytesInBuffer( pxStreamBuffer );

        जर( xBytesAvailable > sbBYTES_TO_STORE_MESSAGE_LENGTH )
        {
            /* The number of bytes available is greater than the number of bytes
             * required to hold the length of the next message, so another message
             * is available. */
            ( शून्य ) prvReadBytesFromBuffer( pxStreamBuffer, ( uint8_t * ) &xTempReturn, sbBYTES_TO_STORE_MESSAGE_LENGTH, pxStreamBuffer->xTail );
            xReturn = ( माप_प्रकार ) xTempReturn;
        }
        अन्यथा
        {
            /* The minimum amount of bytes in a message buffer is
             * ( sbBYTES_TO_STORE_MESSAGE_LENGTH + 1 ), so if xBytesAvailable is
             * less than sbBYTES_TO_STORE_MESSAGE_LENGTH the only other valid
             * value is 0. */
            configASSERT( xBytesAvailable == 0 );
            xReturn = 0;
        }
    }
    अन्यथा
    {
        xReturn = 0;
    }

    परत xReturn;
}
/*-----------------------------------------------------------*/

माप_प्रकार xStreamBufferReceiveFromISR( StreamBufferHandle_t xStreamBuffer,
                                    शून्य * pvRxData,
                                    माप_प्रकार xBufferLengthBytes,
                                    BaseType_t * स्थिरांक pxHigherPriorityTaskWoken )
{
    StreamBuffer_t * स्थिरांक pxStreamBuffer = xStreamBuffer;
    माप_प्रकार xReceivedLength = 0, xBytesAvailable, xBytesToStoreMessageLength;

    configASSERT( pvRxData );
    configASSERT( pxStreamBuffer );

    /* This receive function is used by both message buffers, which store
     * discrete messages, and stream buffers, which store a continuous stream of
     * bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the
     * message. */
    जर( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) != ( uint8_t ) 0 )
    {
        xBytesToStoreMessageLength = sbBYTES_TO_STORE_MESSAGE_LENGTH;
    }
    अन्यथा
    {
        xBytesToStoreMessageLength = 0;
    }

    xBytesAvailable = prvBytesInBuffer( pxStreamBuffer );

    /* Whether receiving a discrete message (where xBytesToStoreMessageLength
     * holds the number of bytes used to store the message length) or a stream of
     * bytes (where xBytesToStoreMessageLength is zero), the number of bytes
     * available must be greater than xBytesToStoreMessageLength to be able to
     * read bytes from the buffer. */
    जर( xBytesAvailable > xBytesToStoreMessageLength )
    {
        xReceivedLength = prvReadMessageFromBuffer( pxStreamBuffer, pvRxData, xBufferLengthBytes, xBytesAvailable );

        /* Was a task waiting for space in the buffer? */
        जर( xReceivedLength != ( माप_प्रकार ) 0 )
        {
            prvRECEIVE_COMPLETED_FROM_ISR( pxStreamBuffer, pxHigherPriorityTaskWoken );
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    अन्यथा
    {
        mtCOVERAGE_TEST_MARKER();
    }

    traceSTREAM_BUFFER_RECEIVE_FROM_ISR( xStreamBuffer, xReceivedLength );

    परत xReceivedLength;
}
/*-----------------------------------------------------------*/

स्थिर माप_प्रकार prvReadMessageFromBuffer( StreamBuffer_t * pxStreamBuffer,
                                        शून्य * pvRxData,
                                        माप_प्रकार xBufferLengthBytes,
                                        माप_प्रकार xBytesAvailable )
{
    माप_प्रकार x_मोजणी, xNextMessageLength;
    configMESSAGE_BUFFER_LENGTH_TYPE xTempNextMessageLength;
    माप_प्रकार xNextTail = pxStreamBuffer->xTail;

    जर( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) != ( uint8_t ) 0 )
    {
        /* A discrete message is being received.  First receive the length
         * of the message. */
        xNextTail = prvReadBytesFromBuffer( pxStreamBuffer, ( uint8_t * ) &xTempNextMessageLength, sbBYTES_TO_STORE_MESSAGE_LENGTH, xNextTail );
        xNextMessageLength = ( माप_प्रकार ) xTempNextMessageLength;

        /* Reduce the number of bytes available by the number of bytes just
         * read out. */
        xBytesAvailable -= sbBYTES_TO_STORE_MESSAGE_LENGTH;

        /* Check there is enough space in the buffer provided by the
         * user. */
        जर( xNextMessageLength > xBufferLengthBytes )
        {
            /* The user has provided insufficient space to read the message. */
            xNextMessageLength = 0;
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    अन्यथा
    {
        /* A stream of bytes is being received (as opposed to a discrete
         * message), so read as many bytes as possible. */
        xNextMessageLength = xBufferLengthBytes;
    }

    /* Use the minimum of the wanted bytes and the available bytes. */
    x_मोजणी = configMIN( xNextMessageLength, xBytesAvailable );

    जर( x_मोजणी != ( माप_प्रकार ) 0 )
    {
        /* Read the actual data and update the tail to mark the data as officially consumed. */
        pxStreamBuffer->xTail = prvReadBytesFromBuffer( pxStreamBuffer, ( uint8_t * ) pvRxData, x_मोजणी, xNextTail ); /*lint !e9079 Data storage area is implemented as uint8_t array for ease of sizing, indexing and alignment. */
    }

    परत x_मोजणी;
}
/*-----------------------------------------------------------*/

BaseType_t xStreamBufferIsEmpty( StreamBufferHandle_t xStreamBuffer )
{
    स्थिरांक StreamBuffer_t * स्थिरांक pxStreamBuffer = xStreamBuffer;
    BaseType_t xReturn;
    माप_प्रकार xTail;

    configASSERT( pxStreamBuffer );

    /* True if no bytes are available. */
    xTail = pxStreamBuffer->xTail;

    जर( pxStreamBuffer->xHead == xTail )
    {
        xReturn = pdTRUE;
    }
    अन्यथा
    {
        xReturn = pdFALSE;
    }

    परत xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xStreamBufferIsFull( StreamBufferHandle_t xStreamBuffer )
{
    BaseType_t xReturn;
    माप_प्रकार xBytesToStoreMessageLength;
    स्थिरांक StreamBuffer_t * स्थिरांक pxStreamBuffer = xStreamBuffer;

    configASSERT( pxStreamBuffer );

    /* This generic version of the receive function is used by both message
     * buffers, which store discrete messages, and stream buffers, which store a
     * continuous stream of bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the message. */
    जर( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) != ( uint8_t ) 0 )
    {
        xBytesToStoreMessageLength = sbBYTES_TO_STORE_MESSAGE_LENGTH;
    }
    अन्यथा
    {
        xBytesToStoreMessageLength = 0;
    }

    /* True if the available space equals zero. */
    जर( xStreamBufferSpacesAvailable( xStreamBuffer ) <= xBytesToStoreMessageLength )
    {
        xReturn = pdTRUE;
    }
    अन्यथा
    {
        xReturn = pdFALSE;
    }

    परत xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xStreamBufferSendCompletedFromISR( StreamBufferHandle_t xStreamBuffer,
                                              BaseType_t * pxHigherPriorityTaskWoken )
{
    StreamBuffer_t * स्थिरांक pxStreamBuffer = xStreamBuffer;
    BaseType_t xReturn;
    UBaseType_t uxSavedInterruptStatus;

    configASSERT( pxStreamBuffer );

    uxSavedInterruptStatus = ( UBaseType_t ) portSET_INTERRUPT_MASK_FROM_ISR();
    {
        जर( ( pxStreamBuffer )->xTaskWaitingToReceive != रिक्त )
        {
            ( शून्य ) xTaskNotifyFromISR( ( pxStreamBuffer )->xTaskWaitingToReceive,
                                         ( uint32_t ) 0,
                                         eNoAction,
                                         pxHigherPriorityTaskWoken );
            ( pxStreamBuffer )->xTaskWaitingToReceive = रिक्त;
            xReturn = pdTRUE;
        }
        अन्यथा
        {
            xReturn = pdFALSE;
        }
    }
    portCLEAR_INTERRUPT_MASK_FROM_ISR( uxSavedInterruptStatus );

    परत xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xStreamBufferReceiveCompletedFromISR( StreamBufferHandle_t xStreamBuffer,
                                                 BaseType_t * pxHigherPriorityTaskWoken )
{
    StreamBuffer_t * स्थिरांक pxStreamBuffer = xStreamBuffer;
    BaseType_t xReturn;
    UBaseType_t uxSavedInterruptStatus;

    configASSERT( pxStreamBuffer );

    uxSavedInterruptStatus = ( UBaseType_t ) portSET_INTERRUPT_MASK_FROM_ISR();
    {
        जर( ( pxStreamBuffer )->xTaskWaitingToSend != रिक्त )
        {
            ( शून्य ) xTaskNotifyFromISR( ( pxStreamBuffer )->xTaskWaitingToSend,
                                         ( uint32_t ) 0,
                                         eNoAction,
                                         pxHigherPriorityTaskWoken );
            ( pxStreamBuffer )->xTaskWaitingToSend = रिक्त;
            xReturn = pdTRUE;
        }
        अन्यथा
        {
            xReturn = pdFALSE;
        }
    }
    portCLEAR_INTERRUPT_MASK_FROM_ISR( uxSavedInterruptStatus );

    परत xReturn;
}
/*-----------------------------------------------------------*/

स्थिर माप_प्रकार prvWriteBytesToBuffer( StreamBuffer_t * स्थिरांक pxStreamBuffer,
                                     स्थिरांक uint8_t * pucData,
                                     माप_प्रकार x_मोजणी,
                                     माप_प्रकार xHead )
{
    माप_प्रकार xFirstLength;

    configASSERT( x_मोजणी > ( माप_प्रकार ) 0 );

    /* Calculate the number of bytes that can be added in the first write -
     * which may be less than the total number of bytes that need to be added if
     * the buffer will wrap back to the beginning. */
    xFirstLength = configMIN( pxStreamBuffer->xLength - xHead, x_मोजणी );

    /* Write as many bytes as can be written in the first write. */
    configASSERT( ( xHead + xFirstLength ) <= pxStreamBuffer->xLength );
    ( शून्य ) स_नकल( ( शून्य * ) ( &( pxStreamBuffer->pucBuffer[ xHead ] ) ), ( स्थिरांक शून्य * ) pucData, xFirstLength ); /*lint !e9087 memcpy() requires void *. */

    /* If the number of bytes written was less than the number that could be
     * written in the first write... */
    जर( x_मोजणी > xFirstLength )
    {
        /* ...then write the remaining bytes to the start of the buffer. */
        configASSERT( ( x_मोजणी - xFirstLength ) <= pxStreamBuffer->xLength );
        ( शून्य ) स_नकल( ( शून्य * ) pxStreamBuffer->pucBuffer, ( स्थिरांक शून्य * ) &( pucData[ xFirstLength ] ), x_मोजणी - xFirstLength ); /*lint !e9087 memcpy() requires void *. */
    }
    अन्यथा
    {
        mtCOVERAGE_TEST_MARKER();
    }

    xHead += x_मोजणी;

    जर( xHead >= pxStreamBuffer->xLength )
    {
        xHead -= pxStreamBuffer->xLength;
    }
    अन्यथा
    {
        mtCOVERAGE_TEST_MARKER();
    }

    परत xHead;
}
/*-----------------------------------------------------------*/

स्थिर माप_प्रकार prvReadBytesFromBuffer( StreamBuffer_t * pxStreamBuffer,
                                      uint8_t * pucData,
                                      माप_प्रकार x_मोजणी,
                                      माप_प्रकार xTail )
{
    माप_प्रकार xFirstLength;

    configASSERT( x_मोजणी != ( माप_प्रकार ) 0 );

    /* Calculate the number of bytes that can be read - which may be
     * less than the number wanted if the data wraps around to the start of
     * the buffer. */
    xFirstLength = configMIN( pxStreamBuffer->xLength - xTail, x_मोजणी );

    /* Obtain the number of bytes it is possible to obtain in the first
     * read.  Asserts check bounds of read and write. */
    configASSERT( xFirstLength <= x_मोजणी );
    configASSERT( ( xTail + xFirstLength ) <= pxStreamBuffer->xLength );
    ( शून्य ) स_नकल( ( शून्य * ) pucData, ( स्थिरांक शून्य * ) &( pxStreamBuffer->pucBuffer[ xTail ] ), xFirstLength ); /*lint !e9087 memcpy() requires void *. */

    /* If the total number of wanted bytes is greater than the number
     * that could be read in the first read... */
    जर( x_मोजणी > xFirstLength )
    {
        /* ...then read the remaining bytes from the start of the buffer. */
        ( शून्य ) स_नकल( ( शून्य * ) &( pucData[ xFirstLength ] ), ( शून्य * ) ( pxStreamBuffer->pucBuffer ), x_मोजणी - xFirstLength ); /*lint !e9087 memcpy() requires void *. */
    }
    अन्यथा
    {
        mtCOVERAGE_TEST_MARKER();
    }

    /* Move the tail pointer to effectively remove the data read from the buffer. */
    xTail += x_मोजणी;

    जर( xTail >= pxStreamBuffer->xLength )
    {
        xTail -= pxStreamBuffer->xLength;
    }

    परत xTail;
}
/*-----------------------------------------------------------*/

स्थिर माप_प्रकार prvBytesInBuffer( स्थिरांक StreamBuffer_t * स्थिरांक pxStreamBuffer )
{
/* Returns the distance between xTail and xHead. */
    माप_प्रकार x_मोजणी;

    x_मोजणी = pxStreamBuffer->xLength + pxStreamBuffer->xHead;
    x_मोजणी -= pxStreamBuffer->xTail;

    जर( x_मोजणी >= pxStreamBuffer->xLength )
    {
        x_मोजणी -= pxStreamBuffer->xLength;
    }
    अन्यथा
    {
        mtCOVERAGE_TEST_MARKER();
    }

    परत x_मोजणी;
}
/*-----------------------------------------------------------*/

स्थिर शून्य prvInitialiseNewStreamBuffer( StreamBuffer_t * स्थिरांक pxStreamBuffer,
                                          uint8_t * स्थिरांक pucBuffer,
                                          माप_प्रकार xBufferSizeBytes,
                                          माप_प्रकार xTriggerLevelBytes,
                                          uint8_t ucFlags,
                                          StreamBufferCallbackFunction_t pxSendCompletedCallback,
                                          StreamBufferCallbackFunction_t pxReceiveCompletedCallback )
{
    /* Assert here is deliberately writing to the entire buffer to ensure it can
     * be written to without generating exceptions, and is setting the buffer to a
     * known value to assist in development/debugging. */
    #जर ( configASSERT_DEFINED == 1 )
    {
        /* The value written just has to be identifiable when looking at the
         * memory.  Don't use 0xA5 as that is the stack fill value and could
         * result in confusion as to what is actually being observed. */
        स्थिरांक BaseType_t x_Write_मूल्य = 0x55;
        configASSERT( स_रखो( pucBuffer, ( पूर्णांक ) x_Write_मूल्य, xBufferSizeBytes ) == pucBuffer );
        ( शून्य ) x_Write_मूल्य;
    } /*lint !e529 !e438 xWriteValue is only used if configASSERT() is defined. */
    #endif

    /* This function could be called from xStreamBufferReset(), so we reset the
     * stream buffer fields manually in order to avoid clearing
     * xStreamBufferLock. The xStreamBufferLock is initialized separately on
     * stream buffer creation. */
    pxStreamBuffer->xTail = ( माप_प्रकार ) 0;
    pxStreamBuffer->xHead = ( माप_प्रकार ) 0;
    pxStreamBuffer->xTaskWaitingToReceive = ( TaskHandle_t ) 0;
    pxStreamBuffer->xTaskWaitingToSend = ( TaskHandle_t ) 0;
    #जर ( configUSE_TRACE_FACILITY == 1 )
        pxStreamBuffer->ux_Stream_Buffer_संख्या = ( UBaseType_t ) 0;
    #endif
    pxStreamBuffer->pucBuffer = pucBuffer;
    pxStreamBuffer->xLength = xBufferSizeBytes;
    pxStreamBuffer->xTriggerLevelBytes = xTriggerLevelBytes;
    pxStreamBuffer->ucFlags = ucFlags;
    #जर ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    {
        pxStreamBuffer->pxSendCompletedCallback = pxSendCompletedCallback;
        pxStreamBuffer->pxReceiveCompletedCallback = pxReceiveCompletedCallback;
    }
    #अन्यथा
    {
        ( शून्य ) pxSendCompletedCallback;
        ( शून्य ) pxReceiveCompletedCallback;
    }
    #endif
}

#जर ( configUSE_TRACE_FACILITY == 1 )

    UBaseType_t ux_Stream_Buffer_Get_Stream_Buffer_संख्या( StreamBufferHandle_t xStreamBuffer )
    {
        परत xStreamBuffer->ux_Stream_Buffer_संख्या;
    }

#endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#जर ( configUSE_TRACE_FACILITY == 1 )

    शून्य v_Stream_Buffer_Set_Stream_Buffer_संख्या( StreamBufferHandle_t xStreamBuffer,
                                             UBaseType_t ux_Stream_Buffer_संख्या )
    {
        xStreamBuffer->ux_Stream_Buffer_संख्या = ux_Stream_Buffer_संख्या;
    }

#endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#जर ( configUSE_TRACE_FACILITY == 1 )

    uint8_t ucStreamBufferGetStreamBufferType( StreamBufferHandle_t xStreamBuffer )
    {
        परत( xStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER );
    }

#endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/
