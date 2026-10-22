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
#அந்தர்பாவ <string.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* FreeRTOS includes. */
#அந்தர்பாவ "FreeRTOS.h"
#அந்தர்பாவ "task.h"
#அந்தர்பாவ "stream_buffer.h"

#எனில் ( configUSE_TASK_NOTIFICATIONS != 1 )
    #error configUSE_TASK_NOTIFICATIONS must be set to 1 to build stream_buffer.c
#endif

#எனில் ( INCLUDE_xTaskGetCurrentTaskHandle != 1 )
    #error INCLUDE_xTaskGetCurrentTaskHandle must be set to 1 to build stream_buffer.c
#endif

/* The MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#undef MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* This entire source file will be skipped if the application is not configured
 * to include stream buffer functionality. This #if is closed at the very bottom
 * of this file. If you want to include stream buffers then ensure
 * configUSE_STREAM_BUFFERS is set to 1 in FreeRTOSConfig.h. */
#எனில் ( configUSE_STREAM_BUFFERS == 1 )

/* If the user has not provided application specific Rx notification macros,
 * or #defined the notification macros away, then provide default implementations
 * that uses task notifications. */
    #ifndef sbRECEIVE_COMPLETED
        #define sbRECEIVE_COMPLETED( pxStreamBuffer )                                 \
    செய்                                                                                \
    {                                                                                 \
        vTaskSuspendAll();                                                            \
        {                                                                             \
            எனில்( ( pxStreamBuffer )->xTaskWaitingToSend != வெற்று )                      \
            {                                                                         \
                ( வெறுமை ) xTaskNotifyIndexed( ( pxStreamBuffer )->xTaskWaitingToSend,  \
                                             ( pxStreamBuffer )->uxNotificationIndex, \
                                             ( uint32_t ) 0,                          \
                                             eNoAction );                             \
                ( pxStreamBuffer )->xTaskWaitingToSend = வெற்று;                        \
            }                                                                         \
        }                                                                             \
        ( வெறுமை ) xTaskResumeAll();                                                    \
    } போது( 0 )
    #endif /* sbRECEIVE_COMPLETED */

/* If user has provided a per-instance receive complete callback, then
 * invoke the callback else use the receive complete macro which is provided by default for all instances.
 */
    #எனில் ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        #define prvRECEIVE_COMPLETED( pxStreamBuffer )                                           \
    செய் {                                                                                         \
        எனில்( ( pxStreamBuffer )->pxReceiveCompletedCallback != வெற்று )                             \
        {                                                                                        \
            ( pxStreamBuffer )->pxReceiveCompletedCallback( ( pxStreamBuffer ), pdFALSE, வெற்று ); \
        }                                                                                        \
        இல்லையெனில்                                                                                     \
        {                                                                                        \
            sbRECEIVE_COMPLETED( ( pxStreamBuffer ) );                                           \
        }                                                                                        \
    } போது( 0 )
    #இல்லையெனில் /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
        #define prvRECEIVE_COMPLETED( pxStreamBuffer )    sbRECEIVE_COMPLETED( ( pxStreamBuffer ) )
    #endif /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

    #ifndef sbRECEIVE_COMPLETED_FROM_ISR
        #define sbRECEIVE_COMPLETED_FROM_ISR( pxStreamBuffer,                                \
                                              pxHigherPriorityTaskWoken )                    \
    செய் {                                                                                     \
        UBaseType_t uxSavedInterruptStatus;                                                  \
                                                                                             \
        uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();                              \
        {                                                                                    \
            எனில்( ( pxStreamBuffer )->xTaskWaitingToSend != வெற்று )                             \
            {                                                                                \
                ( வெறுமை ) xTaskNotifyIndexedFromISR( ( pxStreamBuffer )->xTaskWaitingToSend,  \
                                                    ( pxStreamBuffer )->uxNotificationIndex, \
                                                    ( uint32_t ) 0,                          \
                                                    eNoAction,                               \
                                                    ( pxHigherPriorityTaskWoken ) );         \
                ( pxStreamBuffer )->xTaskWaitingToSend = வெற்று;                               \
            }                                                                                \
        }                                                                                    \
        taskEXIT_CRITICAL_FROM_ISR( uxSavedInterruptStatus );                                \
    } போது( 0 )
    #endif /* sbRECEIVE_COMPLETED_FROM_ISR */

    #எனில் ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        #define prvRECEIVE_COMPLETED_FROM_ISR( pxStreamBuffer,                                                           \
                                               pxHigherPriorityTaskWoken )                                               \
    செய் {                                                                                                                 \
        எனில்( ( pxStreamBuffer )->pxReceiveCompletedCallback != வெற்று )                                                     \
        {                                                                                                                \
            ( pxStreamBuffer )->pxReceiveCompletedCallback( ( pxStreamBuffer ), pdTRUE, ( pxHigherPriorityTaskWoken ) ); \
        }                                                                                                                \
        இல்லையெனில்                                                                                                             \
        {                                                                                                                \
            sbRECEIVE_COMPLETED_FROM_ISR( ( pxStreamBuffer ), ( pxHigherPriorityTaskWoken ) );                           \
        }                                                                                                                \
    } போது( 0 )
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
    vTaskSuspendAll();                                                              \
    {                                                                               \
        எனில்( ( pxStreamBuffer )->xTaskWaitingToReceive != வெற்று )                     \
        {                                                                           \
            ( வெறுமை ) xTaskNotifyIndexed( ( pxStreamBuffer )->xTaskWaitingToReceive, \
                                         ( pxStreamBuffer )->uxNotificationIndex,   \
                                         ( uint32_t ) 0,                            \
                                         eNoAction );                               \
            ( pxStreamBuffer )->xTaskWaitingToReceive = வெற்று;                       \
        }                                                                           \
    }                                                                               \
    ( வெறுமை ) xTaskResumeAll()
    #endif /* sbSEND_COMPLETED */

/* If user has provided a per-instance send completed callback, then
 * invoke the callback else use the send complete macro which is provided by default for all instances.
 */
    #எனில் ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        #define prvSEND_COMPLETED( pxStreamBuffer )                                           \
    செய் {                                                                                      \
        எனில்( ( pxStreamBuffer )->pxSendCompletedCallback != வெற்று )                             \
        {                                                                                     \
            ( pxStreamBuffer )->pxSendCompletedCallback( ( pxStreamBuffer ), pdFALSE, வெற்று ); \
        }                                                                                     \
        இல்லையெனில்                                                                                  \
        {                                                                                     \
            sbSEND_COMPLETED( ( pxStreamBuffer ) );                                           \
        }                                                                                     \
    } போது( 0 )
    #இல்லையெனில் /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
        #define prvSEND_COMPLETED( pxStreamBuffer )    sbSEND_COMPLETED( ( pxStreamBuffer ) )
    #endif /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */


    #ifndef sbSEND_COMPLETE_FROM_ISR
        #define sbSEND_COMPLETE_FROM_ISR( pxStreamBuffer, pxHigherPriorityTaskWoken )          \
    செய் {                                                                                       \
        UBaseType_t uxSavedInterruptStatus;                                                    \
                                                                                               \
        uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();                                \
        {                                                                                      \
            எனில்( ( pxStreamBuffer )->xTaskWaitingToReceive != வெற்று )                            \
            {                                                                                  \
                ( வெறுமை ) xTaskNotifyIndexedFromISR( ( pxStreamBuffer )->xTaskWaitingToReceive, \
                                                    ( pxStreamBuffer )->uxNotificationIndex,   \
                                                    ( uint32_t ) 0,                            \
                                                    eNoAction,                                 \
                                                    ( pxHigherPriorityTaskWoken ) );           \
                ( pxStreamBuffer )->xTaskWaitingToReceive = வெற்று;                              \
            }                                                                                  \
        }                                                                                      \
        taskEXIT_CRITICAL_FROM_ISR( uxSavedInterruptStatus );                                  \
    } போது( 0 )
    #endif /* sbSEND_COMPLETE_FROM_ISR */


    #எனில் ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        #define prvSEND_COMPLETE_FROM_ISR( pxStreamBuffer, pxHigherPriorityTaskWoken )                                \
    செய் {                                                                                                              \
        எனில்( ( pxStreamBuffer )->pxSendCompletedCallback != வெற்று )                                                     \
        {                                                                                                             \
            ( pxStreamBuffer )->pxSendCompletedCallback( ( pxStreamBuffer ), pdTRUE, ( pxHigherPriorityTaskWoken ) ); \
        }                                                                                                             \
        இல்லையெனில்                                                                                                          \
        {                                                                                                             \
            sbSEND_COMPLETE_FROM_ISR( ( pxStreamBuffer ), ( pxHigherPriorityTaskWoken ) );                            \
        }                                                                                                             \
    } போது( 0 )
    #இல்லையெனில் /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
        #define prvSEND_COMPLETE_FROM_ISR( pxStreamBuffer, pxHigherPriorityTaskWoken ) \
    sbSEND_COMPLETE_FROM_ISR( ( pxStreamBuffer ), ( pxHigherPriorityTaskWoken ) )
    #endif /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

/* The number of bytes used to hold the length of a message in the buffer. */
    #define sbBYTES_TO_STORE_MESSAGE_LENGTH    ( அளவு( configMESSAGE_BUFFER_LENGTH_TYPE ) )

/* Bits stored in the ucFlags field of the stream buffer. */
    #define sbFLAGS_IS_MESSAGE_BUFFER          ( ( uint8_t ) 1 ) /* Set if the stream buffer was created as a message buffer, in which case it holds discrete messages rather than a stream. */
    #define sbFLAGS_IS_STATICALLY_ALLOCATED    ( ( uint8_t ) 2 ) /* Set if the stream buffer was created using statically allocated memory. */
    #define sbFLAGS_IS_BATCHING_BUFFER         ( ( uint8_t ) 4 ) /* Set if the stream buffer was created as a batching buffer, meaning the receiver task will only unblock when the trigger level exceededs. */

/*-----------------------------------------------------------*/

/* Structure that hold state information on the buffer. */
வகைவரை கட்டமைப்பு StreamBufferDef_t
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
    UBaseType_t uxNotificationIndex;                               /* The index we are using for notification, by default tskDEFAULT_INDEX_TO_NOTIFY. */
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
                                                     BaseType_t xStreamBufferType,
                                                     StreamBufferCallbackFunction_t pxSendCompletedCallback,
                                                     StreamBufferCallbackFunction_t pxReceiveCompletedCallback )
    {
        வெறுமை * pvAllocatedMemory;
        uint8_t ucFlags;

        traceENTER_xStreamBufferGenericCreate( xBufferSizeBytes, xTriggerLevelBytes, xStreamBufferType, pxSendCompletedCallback, pxReceiveCompletedCallback );

        /* In case the stream buffer is going to be used as a message buffer
         * (that is, it will hold discrete messages with a little meta data that
         * says how big the next message is) check the buffer will be large enough
         * to hold at least one message. */
        எனில்( xStreamBufferType == sbTYPE_MESSAGE_BUFFER )
        {
            /* Is a message buffer but not statically allocated. */
            ucFlags = sbFLAGS_IS_MESSAGE_BUFFER;
            configASSERT( xBufferSizeBytes > sbBYTES_TO_STORE_MESSAGE_LENGTH );
        }
        இல்லையெனில் எனில்( xStreamBufferType == sbTYPE_STREAM_BATCHING_BUFFER )
        {
            /* Is a batching buffer but not statically allocated. */
            ucFlags = sbFLAGS_IS_BATCHING_BUFFER;
            configASSERT( xBufferSizeBytes > 0 );
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
        எனில்( xBufferSizeBytes < ( xBufferSizeBytes + 1U + அளவு( StreamBuffer_t ) ) )
        {
            xBufferSizeBytes++;
            pvAllocatedMemory = pvPortMalloc( xBufferSizeBytes + அளவு( StreamBuffer_t ) );
        }
        இல்லையெனில்
        {
            pvAllocatedMemory = வெற்று;
        }

        எனில்( pvAllocatedMemory != வெற்று )
        {
            /* MISRA Ref 11.5.1 [Malloc memory assignment] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
            /* coverity[misra_c_2012_rule_11_5_violation] */
            prvInitialiseNewStreamBuffer( ( StreamBuffer_t * ) pvAllocatedMemory,                         /* Structure at the start of the allocated memory. */
                                                                                                          /* MISRA Ref 11.5.1 [Malloc memory assignment] */
                                                                                                          /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
                                                                                                          /* coverity[misra_c_2012_rule_11_5_violation] */
                                          ( ( uint8_t * ) pvAllocatedMemory ) + அளவு( StreamBuffer_t ), /* Storage area follows. */
                                          xBufferSizeBytes,
                                          xTriggerLevelBytes,
                                          ucFlags,
                                          pxSendCompletedCallback,
                                          pxReceiveCompletedCallback );

            traceSTREAM_BUFFER_CREATE( ( ( StreamBuffer_t * ) pvAllocatedMemory ), xStreamBufferType );
        }
        இல்லையெனில்
        {
            traceSTREAM_BUFFER_CREATE_FAILED( xStreamBufferType );
        }

        traceRETURN_xStreamBufferGenericCreate( pvAllocatedMemory );

        /* MISRA Ref 11.5.1 [Malloc memory assignment] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
        /* coverity[misra_c_2012_rule_11_5_violation] */
        திரும்பு ( StreamBufferHandle_t ) pvAllocatedMemory;
    }
    #endif /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

    #எனில் ( configSUPPORT_STATIC_ALLOCATION == 1 )

    StreamBufferHandle_t xStreamBufferGenericCreateStatic( size_t xBufferSizeBytes,
                                                           size_t xTriggerLevelBytes,
                                                           BaseType_t xStreamBufferType,
                                                           uint8_t * மாறிலி pucStreamBufferStorageArea,
                                                           StaticStreamBuffer_t * மாறிலி pxStaticStreamBuffer,
                                                           StreamBufferCallbackFunction_t pxSendCompletedCallback,
                                                           StreamBufferCallbackFunction_t pxReceiveCompletedCallback )
    {
        /* MISRA Ref 11.3.1 [Misaligned access] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
        /* coverity[misra_c_2012_rule_11_3_violation] */
        StreamBuffer_t * மாறிலி pxStreamBuffer = ( StreamBuffer_t * ) pxStaticStreamBuffer;
        StreamBufferHandle_t xReturn;
        uint8_t ucFlags;

        traceENTER_xStreamBufferGenericCreateStatic( xBufferSizeBytes, xTriggerLevelBytes, xStreamBufferType, pucStreamBufferStorageArea, pxStaticStreamBuffer, pxSendCompletedCallback, pxReceiveCompletedCallback );

        configASSERT( pucStreamBufferStorageArea );
        configASSERT( pxStaticStreamBuffer );
        configASSERT( xTriggerLevelBytes <= xBufferSizeBytes );

        /* A trigger level of 0 would cause a waiting task to unblock even when
         * the buffer was empty. */
        எனில்( xTriggerLevelBytes == ( size_t ) 0 )
        {
            xTriggerLevelBytes = ( size_t ) 1;
        }

        /* In case the stream buffer is going to be used as a message buffer
         * (that is, it will hold discrete messages with a little meta data that
         * says how big the next message is) check the buffer will be large enough
         * to hold at least one message. */

        எனில்( xStreamBufferType == sbTYPE_MESSAGE_BUFFER )
        {
            /* Statically allocated message buffer. */
            ucFlags = sbFLAGS_IS_MESSAGE_BUFFER | sbFLAGS_IS_STATICALLY_ALLOCATED;
            configASSERT( xBufferSizeBytes > sbBYTES_TO_STORE_MESSAGE_LENGTH );
        }
        இல்லையெனில் எனில்( xStreamBufferType == sbTYPE_STREAM_BATCHING_BUFFER )
        {
            /* Statically allocated batching buffer. */
            ucFlags = sbFLAGS_IS_BATCHING_BUFFER | sbFLAGS_IS_STATICALLY_ALLOCATED;
            configASSERT( xBufferSizeBytes > 0 );
        }
        இல்லையெனில்
        {
            /* Statically allocated stream buffer. */
            ucFlags = sbFLAGS_IS_STATICALLY_ALLOCATED;
        }

        #எனில் ( configASSERT_DEFINED == 1 )
        {
            /* Sanity check that the size of the structure used to declare a
             * variable of type StaticStreamBuffer_t equals the size of the real
             * message buffer structure. */
            நிலையற்ற size_t xSize = அளவு( StaticStreamBuffer_t );
            configASSERT( xSize == அளவு( StreamBuffer_t ) );
        }
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

            traceSTREAM_BUFFER_CREATE( pxStreamBuffer, xStreamBufferType );

            /* MISRA Ref 11.3.1 [Misaligned access] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
            /* coverity[misra_c_2012_rule_11_3_violation] */
            xReturn = ( StreamBufferHandle_t ) pxStaticStreamBuffer;
        }
        இல்லையெனில்
        {
            xReturn = வெற்று;
            traceSTREAM_BUFFER_CREATE_STATIC_FAILED( xReturn, xStreamBufferType );
        }

        traceRETURN_xStreamBufferGenericCreateStatic( xReturn );

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
        StreamBuffer_t * மாறிலி pxStreamBuffer = xStreamBuffer;

        traceENTER_xStreamBufferGetStaticBuffers( xStreamBuffer, ppucStreamBufferStorageArea, ppxStaticStreamBuffer );

        configASSERT( pxStreamBuffer );
        configASSERT( ppucStreamBufferStorageArea );
        configASSERT( ppxStaticStreamBuffer );

        எனில்( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_STATICALLY_ALLOCATED ) != ( uint8_t ) 0 )
        {
            *ppucStreamBufferStorageArea = pxStreamBuffer->pucBuffer;
            /* MISRA Ref 11.3.1 [Misaligned access] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
            /* coverity[misra_c_2012_rule_11_3_violation] */
            *ppxStaticStreamBuffer = ( StaticStreamBuffer_t * ) pxStreamBuffer;
            xReturn = pdTRUE;
        }
        இல்லையெனில்
        {
            xReturn = pdFALSE;
        }

        traceRETURN_xStreamBufferGetStaticBuffers( xReturn );

        திரும்பு xReturn;
    }
    #endif /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

வெறுமை vStreamBufferDelete( StreamBufferHandle_t xStreamBuffer )
{
    StreamBuffer_t * pxStreamBuffer = xStreamBuffer;

    traceENTER_vStreamBufferDelete( xStreamBuffer );

    configASSERT( pxStreamBuffer );

    traceSTREAM_BUFFER_DELETE( xStreamBuffer );

    எனில்( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_STATICALLY_ALLOCATED ) == ( uint8_t ) pdFALSE )
    {
        #எனில் ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
        {
            /* Both the structure and the buffer were allocated using a single call
            * to pvPortMalloc(), hence only one call to vPortFree() is required. */
            vPortFree( ( வெறுமை * ) pxStreamBuffer );
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

    traceRETURN_vStreamBufferDelete();
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

    traceENTER_xStreamBufferReset( xStreamBuffer );

    configASSERT( pxStreamBuffer );

    #எனில் ( configUSE_TRACE_FACILITY == 1 )
    {
        /* Store the stream buffer number so it can be restored after the
         * reset. */
        ux_Stream_Buffer_எண் = pxStreamBuffer->ux_Stream_Buffer_எண்;
    }
    #endif

    /* Can only reset a message buffer if there are no tasks blocked on it. */
    taskENTER_CRITICAL();
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
    taskEXIT_CRITICAL();

    traceRETURN_xStreamBufferReset( xReturn );

    திரும்பு xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xStreamBufferResetFromISR( StreamBufferHandle_t xStreamBuffer )
{
    StreamBuffer_t * மாறிலி pxStreamBuffer = xStreamBuffer;
    BaseType_t xReturn = pdFAIL;
    StreamBufferCallbackFunction_t pxSendCallback = வெற்று, pxReceiveCallback = வெற்று;
    UBaseType_t uxSavedInterruptStatus;

    #எனில் ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_Stream_Buffer_எண்;
    #endif

    traceENTER_xStreamBufferResetFromISR( xStreamBuffer );

    configASSERT( pxStreamBuffer );

    #எனில் ( configUSE_TRACE_FACILITY == 1 )
    {
        /* Store the stream buffer number so it can be restored after the
         * reset. */
        ux_Stream_Buffer_எண் = pxStreamBuffer->ux_Stream_Buffer_எண்;
    }
    #endif

    /* Can only reset a message buffer if there are no tasks blocked on it. */
    /* MISRA Ref 4.7.1 [Return value shall be checked] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
    /* coverity[misra_c_2012_directive_4_7_violation] */
    uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
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

            traceSTREAM_BUFFER_RESET_FROM_ISR( xStreamBuffer );

            xReturn = pdPASS;
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( uxSavedInterruptStatus );

    traceRETURN_xStreamBufferResetFromISR( xReturn );

    திரும்பு xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xStreamBufferSetTriggerLevel( StreamBufferHandle_t xStreamBuffer,
                                         size_t xTriggerLevel )
{
    StreamBuffer_t * மாறிலி pxStreamBuffer = xStreamBuffer;
    BaseType_t xReturn;

    traceENTER_xStreamBufferSetTriggerLevel( xStreamBuffer, xTriggerLevel );

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

    traceRETURN_xStreamBufferSetTriggerLevel( xReturn );

    திரும்பு xReturn;
}
/*-----------------------------------------------------------*/

size_t xStreamBufferSpacesAvailable( StreamBufferHandle_t xStreamBuffer )
{
    மாறிலி StreamBuffer_t * மாறிலி pxStreamBuffer = xStreamBuffer;
    size_t xSpace;
    size_t xOriginalTail;

    traceENTER_xStreamBufferSpacesAvailable( xStreamBuffer );

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

    traceRETURN_xStreamBufferSpacesAvailable( xSpace );

    திரும்பு xSpace;
}
/*-----------------------------------------------------------*/

size_t xStreamBufferBytesAvailable( StreamBufferHandle_t xStreamBuffer )
{
    மாறிலி StreamBuffer_t * மாறிலி pxStreamBuffer = xStreamBuffer;
    size_t xReturn;

    traceENTER_xStreamBufferBytesAvailable( xStreamBuffer );

    configASSERT( pxStreamBuffer );

    xReturn = prvBytesInBuffer( pxStreamBuffer );

    traceRETURN_xStreamBufferBytesAvailable( xReturn );

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

    traceENTER_xStreamBufferSend( xStreamBuffer, pvTxData, xDataLengthBytes, xTicksToWait );

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
            taskENTER_CRITICAL();
            {
                xSpace = xStreamBufferSpacesAvailable( pxStreamBuffer );

                எனில்( xSpace < xRequiredSpace )
                {
                    /* Clear notification state as going to wait for space. */
                    ( வெறுமை ) xTaskNotifyStateClearIndexed( வெற்று, pxStreamBuffer->uxNotificationIndex );

                    /* Should only be one writer. */
                    configASSERT( pxStreamBuffer->xTaskWaitingToSend == வெற்று );
                    pxStreamBuffer->xTaskWaitingToSend = xTaskGetCurrentTaskHandle();
                }
                இல்லையெனில்
                {
                    taskEXIT_CRITICAL();
                    நிறுத்து;
                }
            }
            taskEXIT_CRITICAL();

            traceBLOCKING_ON_STREAM_BUFFER_SEND( xStreamBuffer );
            ( வெறுமை ) xTaskNotifyWaitIndexed( pxStreamBuffer->uxNotificationIndex, ( uint32_t ) 0, ( uint32_t ) 0, வெற்று, xTicksToWait );
            pxStreamBuffer->xTaskWaitingToSend = வெற்று;
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

    traceRETURN_xStreamBufferSend( xReturn );

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

    traceENTER_xStreamBufferSendFromISR( xStreamBuffer, pvTxData, xDataLengthBytes, pxHigherPriorityTaskWoken );

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
            /* MISRA Ref 4.7.1 [Return value shall be checked] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
            /* coverity[misra_c_2012_directive_4_7_violation] */
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
    traceRETURN_xStreamBufferSendFromISR( xReturn );

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
        /* MISRA Ref 11.5.5 [Void pointer assignment] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
        /* coverity[misra_c_2012_rule_11_5_violation] */
        pxStreamBuffer->xHead = prvWriteBytesToBuffer( pxStreamBuffer, ( மாறிலி uint8_t * ) pvTxData, xDataLengthBytes, xNextHead );
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

    traceENTER_xStreamBufferReceive( xStreamBuffer, pvRxData, xBufferLengthBytes, xTicksToWait );

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
    இல்லையெனில் எனில்( ( pxStreamBuffer->ucFlags & sbFLAGS_IS_BATCHING_BUFFER ) != ( uint8_t ) 0 )
    {
        /* Force task to block if the batching buffer contains less bytes than
         * the trigger level. */
        xBytesToStoreMessageLength = pxStreamBuffer->xTriggerLevelBytes;
    }
    இல்லையெனில்
    {
        xBytesToStoreMessageLength = 0;
    }

    எனில்( xTicksToWait != ( TickType_t ) 0 )
    {
        /* Checking if there is data and clearing the notification state must be
         * performed atomically. */
        taskENTER_CRITICAL();
        {
            xBytesAvailable = prvBytesInBuffer( pxStreamBuffer );

            /* If this function was invoked by a message buffer read then
             * xBytesToStoreMessageLength holds the number of bytes used to hold
             * the length of the next discrete message.  If this function was
             * invoked by a stream buffer read then xBytesToStoreMessageLength will
             * be 0. If this function was invoked by a stream batch buffer read
             * then xBytesToStoreMessageLength will be xTriggerLevelBytes value
             * for the buffer.*/
            எனில்( xBytesAvailable <= xBytesToStoreMessageLength )
            {
                /* Clear notification state as going to wait for data. */
                ( வெறுமை ) xTaskNotifyStateClearIndexed( வெற்று, pxStreamBuffer->uxNotificationIndex );

                /* Should only be one reader. */
                configASSERT( pxStreamBuffer->xTaskWaitingToReceive == வெற்று );
                pxStreamBuffer->xTaskWaitingToReceive = xTaskGetCurrentTaskHandle();
            }
            இல்லையெனில்
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        taskEXIT_CRITICAL();

        எனில்( xBytesAvailable <= xBytesToStoreMessageLength )
        {
            /* Wait for data to be available. */
            traceBLOCKING_ON_STREAM_BUFFER_RECEIVE( xStreamBuffer );
            ( வெறுமை ) xTaskNotifyWaitIndexed( pxStreamBuffer->uxNotificationIndex, ( uint32_t ) 0, ( uint32_t ) 0, வெற்று, xTicksToWait );
            pxStreamBuffer->xTaskWaitingToReceive = வெற்று;

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

    traceRETURN_xStreamBufferReceive( xReceivedLength );

    திரும்பு xReceivedLength;
}
/*-----------------------------------------------------------*/

size_t xStreamBufferNextMessageLengthBytes( StreamBufferHandle_t xStreamBuffer )
{
    StreamBuffer_t * மாறிலி pxStreamBuffer = xStreamBuffer;
    size_t xReturn, xBytesAvailable;
    configMESSAGE_BUFFER_LENGTH_TYPE xTempReturn;

    traceENTER_xStreamBufferNextMessageLengthBytes( xStreamBuffer );

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

    traceRETURN_xStreamBufferNextMessageLengthBytes( xReturn );

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

    traceENTER_xStreamBufferReceiveFromISR( xStreamBuffer, pvRxData, xBufferLengthBytes, pxHigherPriorityTaskWoken );

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
            /* MISRA Ref 4.7.1 [Return value shall be checked] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
            /* coverity[misra_c_2012_directive_4_7_violation] */
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
    traceRETURN_xStreamBufferReceiveFromISR( xReceivedLength );

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
        /* MISRA Ref 11.5.5 [Void pointer assignment] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
        /* coverity[misra_c_2012_rule_11_5_violation] */
        pxStreamBuffer->xTail = prvReadBytesFromBuffer( pxStreamBuffer, ( uint8_t * ) pvRxData, x_மொத்தம், xNextTail );
    }

    திரும்பு x_மொத்தம்;
}
/*-----------------------------------------------------------*/

BaseType_t xStreamBufferIsEmpty( StreamBufferHandle_t xStreamBuffer )
{
    மாறிலி StreamBuffer_t * மாறிலி pxStreamBuffer = xStreamBuffer;
    BaseType_t xReturn;
    size_t xTail;

    traceENTER_xStreamBufferIsEmpty( xStreamBuffer );

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

    traceRETURN_xStreamBufferIsEmpty( xReturn );

    திரும்பு xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xStreamBufferIsFull( StreamBufferHandle_t xStreamBuffer )
{
    BaseType_t xReturn;
    size_t xBytesToStoreMessageLength;
    மாறிலி StreamBuffer_t * மாறிலி pxStreamBuffer = xStreamBuffer;

    traceENTER_xStreamBufferIsFull( xStreamBuffer );

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

    traceRETURN_xStreamBufferIsFull( xReturn );

    திரும்பு xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xStreamBufferSendCompletedFromISR( StreamBufferHandle_t xStreamBuffer,
                                              BaseType_t * pxHigherPriorityTaskWoken )
{
    StreamBuffer_t * மாறிலி pxStreamBuffer = xStreamBuffer;
    BaseType_t xReturn;
    UBaseType_t uxSavedInterruptStatus;

    traceENTER_xStreamBufferSendCompletedFromISR( xStreamBuffer, pxHigherPriorityTaskWoken );

    configASSERT( pxStreamBuffer );

    /* MISRA Ref 4.7.1 [Return value shall be checked] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
    /* coverity[misra_c_2012_directive_4_7_violation] */
    uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
    {
        எனில்( ( pxStreamBuffer )->xTaskWaitingToReceive != வெற்று )
        {
            ( வெறுமை ) xTaskNotifyIndexedFromISR( ( pxStreamBuffer )->xTaskWaitingToReceive,
                                                ( pxStreamBuffer )->uxNotificationIndex,
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
    taskEXIT_CRITICAL_FROM_ISR( uxSavedInterruptStatus );

    traceRETURN_xStreamBufferSendCompletedFromISR( xReturn );

    திரும்பு xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xStreamBufferReceiveCompletedFromISR( StreamBufferHandle_t xStreamBuffer,
                                                 BaseType_t * pxHigherPriorityTaskWoken )
{
    StreamBuffer_t * மாறிலி pxStreamBuffer = xStreamBuffer;
    BaseType_t xReturn;
    UBaseType_t uxSavedInterruptStatus;

    traceENTER_xStreamBufferReceiveCompletedFromISR( xStreamBuffer, pxHigherPriorityTaskWoken );

    configASSERT( pxStreamBuffer );

    /* MISRA Ref 4.7.1 [Return value shall be checked] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
    /* coverity[misra_c_2012_directive_4_7_violation] */
    uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
    {
        எனில்( ( pxStreamBuffer )->xTaskWaitingToSend != வெற்று )
        {
            ( வெறுமை ) xTaskNotifyIndexedFromISR( ( pxStreamBuffer )->xTaskWaitingToSend,
                                                ( pxStreamBuffer )->uxNotificationIndex,
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
    taskEXIT_CRITICAL_FROM_ISR( uxSavedInterruptStatus );

    traceRETURN_xStreamBufferReceiveCompletedFromISR( xReturn );

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
    ( வெறுமை ) memcpy( ( வெறுமை * ) ( &( pxStreamBuffer->pucBuffer[ xHead ] ) ), ( மாறிலி வெறுமை * ) pucData, xFirstLength );

    /* If the number of bytes written was less than the number that could be
     * written in the first write... */
    எனில்( x_மொத்தம் > xFirstLength )
    {
        /* ...then write the remaining bytes to the start of the buffer. */
        configASSERT( ( x_மொத்தம் - xFirstLength ) <= pxStreamBuffer->xLength );
        ( வெறுமை ) memcpy( ( வெறுமை * ) pxStreamBuffer->pucBuffer, ( மாறிலி வெறுமை * ) &( pucData[ xFirstLength ] ), x_மொத்தம் - xFirstLength );
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
    ( வெறுமை ) memcpy( ( வெறுமை * ) pucData, ( மாறிலி வெறுமை * ) &( pxStreamBuffer->pucBuffer[ xTail ] ), xFirstLength );

    /* If the total number of wanted bytes is greater than the number
     * that could be read in the first read... */
    எனில்( x_மொத்தம் > xFirstLength )
    {
        /* ...then read the remaining bytes from the start of the buffer. */
        ( வெறுமை ) memcpy( ( வெறுமை * ) &( pucData[ xFirstLength ] ), ( வெறுமை * ) ( pxStreamBuffer->pucBuffer ), x_மொத்தம் - xFirstLength );
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
        #define STREAM_BUFFER_BUFFER_WRITE_மதிப்பு    ( 0x55 )
        configASSERT( memset( pucBuffer, ( முழு ) STREAM_BUFFER_BUFFER_WRITE_மதிப்பு, xBufferSizeBytes ) == pucBuffer );
    }
    #endif

    ( வெறுமை ) memset( ( வெறுமை * ) pxStreamBuffer, 0x00, அளவு( StreamBuffer_t ) );
    pxStreamBuffer->pucBuffer = pucBuffer;
    pxStreamBuffer->xLength = xBufferSizeBytes;
    pxStreamBuffer->xTriggerLevelBytes = xTriggerLevelBytes;
    pxStreamBuffer->ucFlags = ucFlags;
    pxStreamBuffer->uxNotificationIndex = tskDEFAULT_INDEX_TO_NOTIFY;
    #எனில் ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    {
        pxStreamBuffer->pxSendCompletedCallback = pxSendCompletedCallback;
        pxStreamBuffer->pxReceiveCompletedCallback = pxReceiveCompletedCallback;
    }
    #இல்லையெனில்
    {
        /* MISRA Ref 11.1.1 [Object type casting] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-111 */
        /* coverity[misra_c_2012_rule_11_1_violation] */
        ( வெறுமை ) pxSendCompletedCallback;

        /* MISRA Ref 11.1.1 [Object type casting] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-111 */
        /* coverity[misra_c_2012_rule_11_1_violation] */
        ( வெறுமை ) pxReceiveCompletedCallback;
    }
    #endif /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
}
/*-----------------------------------------------------------*/

UBaseType_t uxStreamBufferGetStreamBufferNotificationIndex( StreamBufferHandle_t xStreamBuffer )
{
    StreamBuffer_t * மாறிலி pxStreamBuffer = xStreamBuffer;

    traceENTER_uxStreamBufferGetStreamBufferNotificationIndex( xStreamBuffer );

    configASSERT( pxStreamBuffer );

    traceRETURN_uxStreamBufferGetStreamBufferNotificationIndex( pxStreamBuffer->uxNotificationIndex );

    திரும்பு pxStreamBuffer->uxNotificationIndex;
}
/*-----------------------------------------------------------*/

வெறுமை vStreamBufferSetStreamBufferNotificationIndex( StreamBufferHandle_t xStreamBuffer,
                                                    UBaseType_t uxNotificationIndex )
{
    StreamBuffer_t * மாறிலி pxStreamBuffer = xStreamBuffer;

    traceENTER_vStreamBufferSetStreamBufferNotificationIndex( xStreamBuffer, uxNotificationIndex );

    configASSERT( pxStreamBuffer );

    /* There should be no task waiting otherwise we'd never resume them. */
    configASSERT( pxStreamBuffer->xTaskWaitingToReceive == வெற்று );
    configASSERT( pxStreamBuffer->xTaskWaitingToSend == வெற்று );

    /* Check that the task notification index is valid. */
    configASSERT( uxNotificationIndex < configTASK_NOTIFICATION_ARRAY_ENTRIES );

    pxStreamBuffer->uxNotificationIndex = uxNotificationIndex;

    traceRETURN_vStreamBufferSetStreamBufferNotificationIndex();
}
/*-----------------------------------------------------------*/

    #எனில் ( configUSE_TRACE_FACILITY == 1 )

    UBaseType_t ux_Stream_Buffer_Get_Stream_Buffer_எண்( StreamBufferHandle_t xStreamBuffer )
    {
        trace_ENTER_ux_Stream_Buffer_Get_Stream_Buffer_எண்( xStreamBuffer );

        trace_RETURN_ux_Stream_Buffer_Get_Stream_Buffer_எண்( xStreamBuffer->ux_Stream_Buffer_எண் );

        திரும்பு xStreamBuffer->ux_Stream_Buffer_எண்;
    }

    #endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

    #எனில் ( configUSE_TRACE_FACILITY == 1 )

    வெறுமை v_Stream_Buffer_Set_Stream_Buffer_எண்( StreamBufferHandle_t xStreamBuffer,
                                             UBaseType_t ux_Stream_Buffer_எண் )
    {
        trace_ENTER_v_Stream_Buffer_Set_Stream_Buffer_எண்( xStreamBuffer, ux_Stream_Buffer_எண் );

        xStreamBuffer->ux_Stream_Buffer_எண் = ux_Stream_Buffer_எண்;

        trace_RETURN_v_Stream_Buffer_Set_Stream_Buffer_எண்();
    }

    #endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

    #எனில் ( configUSE_TRACE_FACILITY == 1 )

    uint8_t ucStreamBufferGetStreamBufferType( StreamBufferHandle_t xStreamBuffer )
    {
        traceENTER_ucStreamBufferGetStreamBufferType( xStreamBuffer );

        traceRETURN_ucStreamBufferGetStreamBufferType( ( uint8_t ) ( xStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) );

        திரும்பு( ( uint8_t ) ( xStreamBuffer->ucFlags & sbFLAGS_IS_MESSAGE_BUFFER ) );
    }

    #endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

/* This entire source file will be skipped if the application is not configured
 * to include stream buffer functionality. This #if is closed at the very bottom
 * of this file. If you want to include stream buffers then ensure
 * configUSE_STREAM_BUFFERS is set to 1 in FreeRTOSConfig.h. */
#endif /* configUSE_STREAM_BUFFERS == 1 */
