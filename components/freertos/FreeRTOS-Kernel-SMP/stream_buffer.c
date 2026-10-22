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
#অন্তর্ভাব <string.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* FreeRTOS includes. */
#অন্তর্ভাব "FreeRTOS.h"
#অন্তর্ভাব "task.h"
#অন্তর্ভাব "stream_buffer.h"

#যদি ( configUSE_TASK_NOTIFICATIONS != 1 )
    #error configUSE_TASK_NOTIFICATIONS must be set to 1 to build stream_বাফার.c
#endif

#যদি ( INCLUDE_xTaskGetCurrentTaskHandle != 1 )
    #error INCLUDE_xTaskGetCurrentTaskHandle must be set to 1 to build stream_বাফার.c
#endif

/* The MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#undef MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* This entire source file will be skipped if the application is not configured
 * to include stream buffer functionality. This #if is closed at the very bottom
 * of this file. If you want to include stream buffers then ensure
 * configUSE_STREAM_BUFFERS is set to 1 in FreeRTOSConfig.h. */
#যদি ( configUSE_STREAM_BUFFERS == 1 )

/* If the user has not provided application specific Rx notification macros,
 * or #defined the notification macros away, then provide default implementations
 * that uses task notifications. */
    #ifndef sbRECEIVE_COMPLETED
        #define sbRECEIVE_COMPLETED( px_Stream_বাফার )                                 \
    করো                                                                                \
    {                                                                                 \
        vTaskSuspendAll();                                                            \
        {                                                                             \
            যদি( ( px_Stream_বাফার )->xTaskWaitingToSend != ফাঁকা )                      \
            {                                                                         \
                ( শূন্য ) xTaskNotifyIndexed( ( px_Stream_বাফার )->xTaskWaitingToSend,  \
                                             ( px_Stream_বাফার )->ux_Notification_সূচক, \
                                             ( uint32_t ) 0,                          \
                                             eNoAction );                             \
                ( px_Stream_বাফার )->xTaskWaitingToSend = ফাঁকা;                        \
            }                                                                         \
        }                                                                             \
        ( শূন্য ) xTaskResumeAll();                                                    \
    } যখন( 0 )
    #endif /* sbRECEIVE_COMPLETED */

/* If user has provided a per-instance receive complete callback, then
 * invoke the callback else use the receive complete macro which is provided by default for all instances.
 */
    #যদি ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        #define prvRECEIVE_COMPLETED( px_Stream_বাফার )                                           \
    করো {                                                                                         \
        যদি( ( px_Stream_বাফার )->pxReceiveCompletedCallback != ফাঁকা )                             \
        {                                                                                        \
            ( px_Stream_বাফার )->pxReceiveCompletedCallback( ( px_Stream_বাফার ), pdFALSE, ফাঁকা ); \
        }                                                                                        \
        অন্যথা                                                                                     \
        {                                                                                        \
            sbRECEIVE_COMPLETED( ( px_Stream_বাফার ) );                                           \
        }                                                                                        \
    } যখন( 0 )
    #অন্যথা /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
        #define prvRECEIVE_COMPLETED( px_Stream_বাফার )    sbRECEIVE_COMPLETED( ( px_Stream_বাফার ) )
    #endif /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

    #ifndef sbRECEIVE_COMPLETED_FROM_ISR
        #define sbRECEIVE_COMPLETED_FROM_ISR( px_Stream_বাফার,                                \
                                              pxHigherPriorityTaskWoken )                    \
    করো {                                                                                     \
        UBaseType_t uxSavedInterruptStatus;                                                  \
                                                                                             \
        uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();                              \
        {                                                                                    \
            যদি( ( px_Stream_বাফার )->xTaskWaitingToSend != ফাঁকা )                             \
            {                                                                                \
                ( শূন্য ) xTaskNotifyIndexedFromISR( ( px_Stream_বাফার )->xTaskWaitingToSend,  \
                                                    ( px_Stream_বাফার )->ux_Notification_সূচক, \
                                                    ( uint32_t ) 0,                          \
                                                    eNoAction,                               \
                                                    ( pxHigherPriorityTaskWoken ) );         \
                ( px_Stream_বাফার )->xTaskWaitingToSend = ফাঁকা;                               \
            }                                                                                \
        }                                                                                    \
        taskEXIT_CRITICAL_FROM_ISR( uxSavedInterruptStatus );                                \
    } যখন( 0 )
    #endif /* sbRECEIVE_COMPLETED_FROM_ISR */

    #যদি ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        #define prvRECEIVE_COMPLETED_FROM_ISR( px_Stream_বাফার,                                                           \
                                               pxHigherPriorityTaskWoken )                                               \
    করো {                                                                                                                 \
        যদি( ( px_Stream_বাফার )->pxReceiveCompletedCallback != ফাঁকা )                                                     \
        {                                                                                                                \
            ( px_Stream_বাফার )->pxReceiveCompletedCallback( ( px_Stream_বাফার ), pdTRUE, ( pxHigherPriorityTaskWoken ) ); \
        }                                                                                                                \
        অন্যথা                                                                                                             \
        {                                                                                                                \
            sbRECEIVE_COMPLETED_FROM_ISR( ( px_Stream_বাফার ), ( pxHigherPriorityTaskWoken ) );                           \
        }                                                                                                                \
    } যখন( 0 )
    #অন্যথা /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
        #define prvRECEIVE_COMPLETED_FROM_ISR( px_Stream_বাফার, pxHigherPriorityTaskWoken ) \
    sbRECEIVE_COMPLETED_FROM_ISR( ( px_Stream_বাফার ), ( pxHigherPriorityTaskWoken ) )
    #endif /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

/* If the user has not provided an application specific Tx notification macro,
 * or #defined the notification macro away, then provide a default
 * implementation that uses task notifications.
 */
    #ifndef sbSEND_COMPLETED
        #define sbSEND_COMPLETED( px_Stream_বাফার )                                  \
    vTaskSuspendAll();                                                              \
    {                                                                               \
        যদি( ( px_Stream_বাফার )->xTaskWaitingToReceive != ফাঁকা )                     \
        {                                                                           \
            ( শূন্য ) xTaskNotifyIndexed( ( px_Stream_বাফার )->xTaskWaitingToReceive, \
                                         ( px_Stream_বাফার )->ux_Notification_সূচক,   \
                                         ( uint32_t ) 0,                            \
                                         eNoAction );                               \
            ( px_Stream_বাফার )->xTaskWaitingToReceive = ফাঁকা;                       \
        }                                                                           \
    }                                                                               \
    ( শূন্য ) xTaskResumeAll()
    #endif /* sbSEND_COMPLETED */

/* If user has provided a per-instance send completed callback, then
 * invoke the callback else use the send complete macro which is provided by default for all instances.
 */
    #যদি ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        #define prvSEND_COMPLETED( px_Stream_বাফার )                                           \
    করো {                                                                                      \
        যদি( ( px_Stream_বাফার )->pxSendCompletedCallback != ফাঁকা )                             \
        {                                                                                     \
            ( px_Stream_বাফার )->pxSendCompletedCallback( ( px_Stream_বাফার ), pdFALSE, ফাঁকা ); \
        }                                                                                     \
        অন্যথা                                                                                  \
        {                                                                                     \
            sbSEND_COMPLETED( ( px_Stream_বাফার ) );                                           \
        }                                                                                     \
    } যখন( 0 )
    #অন্যথা /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
        #define prvSEND_COMPLETED( px_Stream_বাফার )    sbSEND_COMPLETED( ( px_Stream_বাফার ) )
    #endif /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */


    #ifndef sbSEND_COMPLETE_FROM_ISR
        #define sbSEND_COMPLETE_FROM_ISR( px_Stream_বাফার, pxHigherPriorityTaskWoken )          \
    করো {                                                                                       \
        UBaseType_t uxSavedInterruptStatus;                                                    \
                                                                                               \
        uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();                                \
        {                                                                                      \
            যদি( ( px_Stream_বাফার )->xTaskWaitingToReceive != ফাঁকা )                            \
            {                                                                                  \
                ( শূন্য ) xTaskNotifyIndexedFromISR( ( px_Stream_বাফার )->xTaskWaitingToReceive, \
                                                    ( px_Stream_বাফার )->ux_Notification_সূচক,   \
                                                    ( uint32_t ) 0,                            \
                                                    eNoAction,                                 \
                                                    ( pxHigherPriorityTaskWoken ) );           \
                ( px_Stream_বাফার )->xTaskWaitingToReceive = ফাঁকা;                              \
            }                                                                                  \
        }                                                                                      \
        taskEXIT_CRITICAL_FROM_ISR( uxSavedInterruptStatus );                                  \
    } যখন( 0 )
    #endif /* sbSEND_COMPLETE_FROM_ISR */


    #যদি ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        #define prvSEND_COMPLETE_FROM_ISR( px_Stream_বাফার, pxHigherPriorityTaskWoken )                                \
    করো {                                                                                                              \
        যদি( ( px_Stream_বাফার )->pxSendCompletedCallback != ফাঁকা )                                                     \
        {                                                                                                             \
            ( px_Stream_বাফার )->pxSendCompletedCallback( ( px_Stream_বাফার ), pdTRUE, ( pxHigherPriorityTaskWoken ) ); \
        }                                                                                                             \
        অন্যথা                                                                                                          \
        {                                                                                                             \
            sbSEND_COMPLETE_FROM_ISR( ( px_Stream_বাফার ), ( pxHigherPriorityTaskWoken ) );                            \
        }                                                                                                             \
    } যখন( 0 )
    #অন্যথা /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
        #define prvSEND_COMPLETE_FROM_ISR( px_Stream_বাফার, pxHigherPriorityTaskWoken ) \
    sbSEND_COMPLETE_FROM_ISR( ( px_Stream_বাফার ), ( pxHigherPriorityTaskWoken ) )
    #endif /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

/* The number of bytes used to hold the length of a message in the buffer. */
    #define sb_BYTES_TO_STORE_MESSAGE_দৈর্ঘ্য    ( আকার( config_MESSAGE_বাফার_দৈর্ঘ্য_TYPE ) )

/* Bits stored in the ucFlags field of the stream buffer. */
    #define sb_FLAGS_IS_MESSAGE_বাফার          ( ( uint8_t ) 1 ) /* Set if the stream buffer was created as a message buffer, in which case it holds discrete messages rather than a stream. */
    #define sbFLAGS_IS_STATICALLY_ALLOCATED    ( ( uint8_t ) 2 ) /* Set if the stream buffer was created using statically allocated memory. */
    #define sb_FLAGS_IS_BATCHING_বাফার         ( ( uint8_t ) 4 ) /* Set if the stream buffer was created as a batching buffer, meaning the receiver task will only unblock when the trigger level exceededs. */

/*-----------------------------------------------------------*/

/* Structure that hold state information on the buffer. */
প্রকারলেখ কাঠামো Stream_বাফার_Def_t
{
    অস্থির size_t x_লেজ;                       /* Index to the next item to read within the buffer. */
    অস্থির size_t x_শীর্ষ;                       /* Index to the next item to write within the buffer. */
    size_t x_দৈর্ঘ্য;                              /* The length of the buffer pointed to by pucBuffer. */
    size_t xTriggerLevelBytes;                   /* The number of bytes that must be in the stream buffer before a task that is waiting for data is unblocked. */
    অস্থির TaskHandle_t xTaskWaitingToReceive; /* Holds the handle of a task waiting for data, or NULL if no tasks are waiting. */
    অস্থির TaskHandle_t xTaskWaitingToSend;    /* Holds the handle of a task waiting to send data to a message buffer that is full. */
    uint8_t * puc_বাফার;                         /* Points to the buffer itself - that is - the RAM that stores the data passed through the buffer. */
    uint8_t ucFlags;

    #যদি ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_Stream_বাফার_সংখ্যা; /* Used for tracing purposes. */
    #endif

    #যদি ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        Stream_বাফার_Callback_Function_t pxSendCompletedCallback;    /* Optional callback called on send complete. sbSEND_COMPLETED is called if this is NULL. */
        Stream_বাফার_Callback_Function_t pxReceiveCompletedCallback; /* Optional callback called on receive complete.  sbRECEIVE_COMPLETED is called if this is NULL. */
    #endif
    UBaseType_t ux_Notification_সূচক;                               /* The index we are using for notification, by default tskDEFAULT_INDEX_TO_NOTIFY. */
} Stream_বাফার_t;

/*
 * The number of bytes available to be read from the buffer.
 */
স্থির size_t prv_Bytes_In_বাফার( ধ্রুবক Stream_বাফার_t * ধ্রুবক px_Stream_বাফার ) PRIVILEGED_FUNCTION;

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
স্থির size_t prv_Write_Bytes_To_বাফার( Stream_বাফার_t * ধ্রুবক px_Stream_বাফার,
                                     ধ্রুবক uint8_t * puc_তথ্য,
                                     size_t x_গুনতি,
                                     size_t x_শীর্ষ ) PRIVILEGED_FUNCTION;

/*
 * If the stream buffer is being used as a message buffer, then reads an entire
 * message out of the buffer.  If the stream buffer is being used as a stream
 * buffer then read as many bytes as possible from the buffer.
 * prvReadBytesFromBuffer() is called to actually extract the bytes from the
 * buffer's data storage area.
 */
স্থির size_t prv_Read_Message_From_বাফার( Stream_বাফার_t * px_Stream_বাফার,
                                        শূন্য * pv_Rx_তথ্য,
                                        size_t x_বাফার_দৈর্ঘ্য_Bytes,
                                        size_t xBytesAvailable ) PRIVILEGED_FUNCTION;

/*
 * If the stream buffer is being used as a message buffer, then writes an entire
 * message to the buffer.  If the stream buffer is being used as a stream
 * buffer then write as many bytes as possible to the buffer.
 * prvWriteBytestoBuffer() is called to actually send the bytes to the buffer's
 * data storage area.
 */
স্থির size_t prv_Write_Message_To_বাফার( Stream_বাফার_t * ধ্রুবক px_Stream_বাফার,
                                       ধ্রুবক শূন্য * pv_Tx_তথ্য,
                                       size_t x_তথ্য_দৈর্ঘ্য_Bytes,
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
স্থির size_t prv_Read_Bytes_From_বাফার( Stream_বাফার_t * px_Stream_বাফার,
                                      uint8_t * puc_তথ্য,
                                      size_t x_গুনতি,
                                      size_t x_লেজ ) PRIVILEGED_FUNCTION;

/*
 * Called by both pxStreamBufferCreate() and pxStreamBufferCreateStatic() to
 * initialise the members of the newly created stream buffer structure.
 */
স্থির শূন্য prv_Initialise_New_Stream_বাফার( Stream_বাফার_t * ধ্রুবক px_Stream_বাফার,
                                          uint8_t * ধ্রুবক puc_বাফার,
                                          size_t x_বাফার_আকার_Bytes,
                                          size_t xTriggerLevelBytes,
                                          uint8_t ucFlags,
                                          Stream_বাফার_Callback_Function_t pxSendCompletedCallback,
                                          Stream_বাফার_Callback_Function_t pxReceiveCompletedCallback ) PRIVILEGED_FUNCTION;

/*-----------------------------------------------------------*/
    #যদি ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
    Stream_বাফার_Handle_t x_Stream_বাফার_Generic_Create( size_t x_বাফার_আকার_Bytes,
                                                     size_t xTriggerLevelBytes,
                                                     BaseType_t x_Stream_বাফার_Type,
                                                     Stream_বাফার_Callback_Function_t pxSendCompletedCallback,
                                                     Stream_বাফার_Callback_Function_t pxReceiveCompletedCallback )
    {
        শূন্য * pvAllocatedMemory;
        uint8_t ucFlags;

        trace_ENTER_x_Stream_বাফার_Generic_Create( x_বাফার_আকার_Bytes, xTriggerLevelBytes, x_Stream_বাফার_Type, pxSendCompletedCallback, pxReceiveCompletedCallback );

        /* In case the stream buffer is going to be used as a message buffer
         * (that is, it will hold discrete messages with a little meta data that
         * says how big the next message is) check the buffer will be large enough
         * to hold at least one message. */
        যদি( x_Stream_বাফার_Type == sb_TYPE_MESSAGE_বাফার )
        {
            /* Is a message buffer but not statically allocated. */
            ucFlags = sb_FLAGS_IS_MESSAGE_বাফার;
            configASSERT( x_বাফার_আকার_Bytes > sb_BYTES_TO_STORE_MESSAGE_দৈর্ঘ্য );
        }
        অন্যথা যদি( x_Stream_বাফার_Type == sb_TYPE_STREAM_BATCHING_বাফার )
        {
            /* Is a batching buffer but not statically allocated. */
            ucFlags = sb_FLAGS_IS_BATCHING_বাফার;
            configASSERT( x_বাফার_আকার_Bytes > 0 );
        }
        অন্যথা
        {
            /* Not a message buffer and not statically allocated. */
            ucFlags = 0;
            configASSERT( x_বাফার_আকার_Bytes > 0 );
        }

        configASSERT( xTriggerLevelBytes <= x_বাফার_আকার_Bytes );

        /* A trigger level of 0 would cause a waiting task to unblock even when
         * the buffer was empty. */
        যদি( xTriggerLevelBytes == ( size_t ) 0 )
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
        যদি( x_বাফার_আকার_Bytes < ( x_বাফার_আকার_Bytes + 1U + আকার( Stream_বাফার_t ) ) )
        {
            x_বাফার_আকার_Bytes++;
            pvAllocatedMemory = pvPortMalloc( x_বাফার_আকার_Bytes + আকার( Stream_বাফার_t ) );
        }
        অন্যথা
        {
            pvAllocatedMemory = ফাঁকা;
        }

        যদি( pvAllocatedMemory != ফাঁকা )
        {
            /* MISRA Ref 11.5.1 [Malloc memory assignment] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
            /* coverity[misra_c_2012_rule_11_5_violation] */
            prv_Initialise_New_Stream_বাফার( ( Stream_বাফার_t * ) pvAllocatedMemory,                         /* Structure at the start of the allocated memory. */
                                                                                                          /* MISRA Ref 11.5.1 [Malloc memory assignment] */
                                                                                                          /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
                                                                                                          /* coverity[misra_c_2012_rule_11_5_violation] */
                                          ( ( uint8_t * ) pvAllocatedMemory ) + আকার( Stream_বাফার_t ), /* Storage area follows. */
                                          x_বাফার_আকার_Bytes,
                                          xTriggerLevelBytes,
                                          ucFlags,
                                          pxSendCompletedCallback,
                                          pxReceiveCompletedCallback );

            trace_STREAM_বাফার_CREATE( ( ( Stream_বাফার_t * ) pvAllocatedMemory ), x_Stream_বাফার_Type );
        }
        অন্যথা
        {
            trace_STREAM_বাফার_CREATE_FAILED( x_Stream_বাফার_Type );
        }

        trace_RETURN_x_Stream_বাফার_Generic_Create( pvAllocatedMemory );

        /* MISRA Ref 11.5.1 [Malloc memory assignment] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
        /* coverity[misra_c_2012_rule_11_5_violation] */
        ফেরত ( Stream_বাফার_Handle_t ) pvAllocatedMemory;
    }
    #endif /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

    #যদি ( configSUPPORT_STATIC_ALLOCATION == 1 )

    Stream_বাফার_Handle_t x_Stream_বাফার_Generic_Create_Static( size_t x_বাফার_আকার_Bytes,
                                                           size_t xTriggerLevelBytes,
                                                           BaseType_t x_Stream_বাফার_Type,
                                                           uint8_t * ধ্রুবক puc_Stream_বাফার_Storage_Area,
                                                           Static_Stream_বাফার_t * ধ্রুবক px_Static_Stream_বাফার,
                                                           Stream_বাফার_Callback_Function_t pxSendCompletedCallback,
                                                           Stream_বাফার_Callback_Function_t pxReceiveCompletedCallback )
    {
        /* MISRA Ref 11.3.1 [Misaligned access] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
        /* coverity[misra_c_2012_rule_11_3_violation] */
        Stream_বাফার_t * ধ্রুবক px_Stream_বাফার = ( Stream_বাফার_t * ) px_Static_Stream_বাফার;
        Stream_বাফার_Handle_t xReturn;
        uint8_t ucFlags;

        trace_ENTER_x_Stream_বাফার_Generic_Create_Static( x_বাফার_আকার_Bytes, xTriggerLevelBytes, x_Stream_বাফার_Type, puc_Stream_বাফার_Storage_Area, px_Static_Stream_বাফার, pxSendCompletedCallback, pxReceiveCompletedCallback );

        configASSERT( puc_Stream_বাফার_Storage_Area );
        configASSERT( px_Static_Stream_বাফার );
        configASSERT( xTriggerLevelBytes <= x_বাফার_আকার_Bytes );

        /* A trigger level of 0 would cause a waiting task to unblock even when
         * the buffer was empty. */
        যদি( xTriggerLevelBytes == ( size_t ) 0 )
        {
            xTriggerLevelBytes = ( size_t ) 1;
        }

        /* In case the stream buffer is going to be used as a message buffer
         * (that is, it will hold discrete messages with a little meta data that
         * says how big the next message is) check the buffer will be large enough
         * to hold at least one message. */

        যদি( x_Stream_বাফার_Type == sb_TYPE_MESSAGE_বাফার )
        {
            /* Statically allocated message buffer. */
            ucFlags = sb_FLAGS_IS_MESSAGE_বাফার | sbFLAGS_IS_STATICALLY_ALLOCATED;
            configASSERT( x_বাফার_আকার_Bytes > sb_BYTES_TO_STORE_MESSAGE_দৈর্ঘ্য );
        }
        অন্যথা যদি( x_Stream_বাফার_Type == sb_TYPE_STREAM_BATCHING_বাফার )
        {
            /* Statically allocated batching buffer. */
            ucFlags = sb_FLAGS_IS_BATCHING_বাফার | sbFLAGS_IS_STATICALLY_ALLOCATED;
            configASSERT( x_বাফার_আকার_Bytes > 0 );
        }
        অন্যথা
        {
            /* Statically allocated stream buffer. */
            ucFlags = sbFLAGS_IS_STATICALLY_ALLOCATED;
        }

        #যদি ( configASSERT_DEFINED == 1 )
        {
            /* Sanity check that the size of the structure used to declare a
             * variable of type StaticStreamBuffer_t equals the size of the real
             * message buffer structure. */
            অস্থির size_t x_আকার = আকার( Static_Stream_বাফার_t );
            configASSERT( x_আকার == আকার( Stream_বাফার_t ) );
        }
        #endif /* configASSERT_DEFINED */

        যদি( ( puc_Stream_বাফার_Storage_Area != ফাঁকা ) && ( px_Static_Stream_বাফার != ফাঁকা ) )
        {
            prv_Initialise_New_Stream_বাফার( px_Stream_বাফার,
                                          puc_Stream_বাফার_Storage_Area,
                                          x_বাফার_আকার_Bytes,
                                          xTriggerLevelBytes,
                                          ucFlags,
                                          pxSendCompletedCallback,
                                          pxReceiveCompletedCallback );

            /* Remember this was statically allocated in case it is ever deleted
             * again. */
            px_Stream_বাফার->ucFlags |= sbFLAGS_IS_STATICALLY_ALLOCATED;

            trace_STREAM_বাফার_CREATE( px_Stream_বাফার, x_Stream_বাফার_Type );

            /* MISRA Ref 11.3.1 [Misaligned access] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
            /* coverity[misra_c_2012_rule_11_3_violation] */
            xReturn = ( Stream_বাফার_Handle_t ) px_Static_Stream_বাফার;
        }
        অন্যথা
        {
            xReturn = ফাঁকা;
            trace_STREAM_বাফার_CREATE_STATIC_FAILED( xReturn, x_Stream_বাফার_Type );
        }

        trace_RETURN_x_Stream_বাফার_Generic_Create_Static( xReturn );

        ফেরত xReturn;
    }
    #endif /* ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*-----------------------------------------------------------*/

    #যদি ( configSUPPORT_STATIC_ALLOCATION == 1 )
    BaseType_t x_Stream_বাফার_Get_Static_Buffers( Stream_বাফার_Handle_t x_Stream_বাফার,
                                              uint8_t ** ppuc_Stream_বাফার_Storage_Area,
                                              Static_Stream_বাফার_t ** ppx_Static_Stream_বাফার )
    {
        BaseType_t xReturn;
        Stream_বাফার_t * ধ্রুবক px_Stream_বাফার = x_Stream_বাফার;

        trace_ENTER_x_Stream_বাফার_Get_Static_Buffers( x_Stream_বাফার, ppuc_Stream_বাফার_Storage_Area, ppx_Static_Stream_বাফার );

        configASSERT( px_Stream_বাফার );
        configASSERT( ppuc_Stream_বাফার_Storage_Area );
        configASSERT( ppx_Static_Stream_বাফার );

        যদি( ( px_Stream_বাফার->ucFlags & sbFLAGS_IS_STATICALLY_ALLOCATED ) != ( uint8_t ) 0 )
        {
            *ppuc_Stream_বাফার_Storage_Area = px_Stream_বাফার->puc_বাফার;
            /* MISRA Ref 11.3.1 [Misaligned access] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
            /* coverity[misra_c_2012_rule_11_3_violation] */
            *ppx_Static_Stream_বাফার = ( Static_Stream_বাফার_t * ) px_Stream_বাফার;
            xReturn = pdTRUE;
        }
        অন্যথা
        {
            xReturn = pdFALSE;
        }

        trace_RETURN_x_Stream_বাফার_Get_Static_Buffers( xReturn );

        ফেরত xReturn;
    }
    #endif /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

শূন্য v_Stream_বাফার_Delete( Stream_বাফার_Handle_t x_Stream_বাফার )
{
    Stream_বাফার_t * px_Stream_বাফার = x_Stream_বাফার;

    trace_ENTER_v_Stream_বাফার_Delete( x_Stream_বাফার );

    configASSERT( px_Stream_বাফার );

    trace_STREAM_বাফার_DELETE( x_Stream_বাফার );

    যদি( ( px_Stream_বাফার->ucFlags & sbFLAGS_IS_STATICALLY_ALLOCATED ) == ( uint8_t ) pdFALSE )
    {
        #যদি ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
        {
            /* Both the structure and the buffer were allocated using a single call
            * to pvPortMalloc(), hence only one call to vPortFree() is required. */
            vPortFree( ( শূন্য * ) px_Stream_বাফার );
        }
        #অন্যথা
        {
            /* Should not be possible to get here, ucFlags must be corrupt.
             * Force an assert. */
            configASSERT( x_Stream_বাফার == ( Stream_বাফার_Handle_t ) ~0 );
        }
        #endif
    }
    অন্যথা
    {
        /* The structure and buffer were not allocated dynamically and cannot be
         * freed - just scrub the structure so future use will assert. */
        ( শূন্য ) memset( px_Stream_বাফার, 0x00, আকার( Stream_বাফার_t ) );
    }

    trace_RETURN_v_Stream_বাফার_Delete();
}
/*-----------------------------------------------------------*/

BaseType_t x_Stream_বাফার_Reset( Stream_বাফার_Handle_t x_Stream_বাফার )
{
    Stream_বাফার_t * ধ্রুবক px_Stream_বাফার = x_Stream_বাফার;
    BaseType_t xReturn = pdFAIL;
    Stream_বাফার_Callback_Function_t pxSendCallback = ফাঁকা, pxReceiveCallback = ফাঁকা;

    #যদি ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_Stream_বাফার_সংখ্যা;
    #endif

    trace_ENTER_x_Stream_বাফার_Reset( x_Stream_বাফার );

    configASSERT( px_Stream_বাফার );

    #যদি ( configUSE_TRACE_FACILITY == 1 )
    {
        /* Store the stream buffer number so it can be restored after the
         * reset. */
        ux_Stream_বাফার_সংখ্যা = px_Stream_বাফার->ux_Stream_বাফার_সংখ্যা;
    }
    #endif

    /* Can only reset a message buffer if there are no tasks blocked on it. */
    taskENTER_CRITICAL();
    {
        যদি( ( px_Stream_বাফার->xTaskWaitingToReceive == ফাঁকা ) && ( px_Stream_বাফার->xTaskWaitingToSend == ফাঁকা ) )
        {
            #যদি ( configUSE_SB_COMPLETED_CALLBACK == 1 )
            {
                pxSendCallback = px_Stream_বাফার->pxSendCompletedCallback;
                pxReceiveCallback = px_Stream_বাফার->pxReceiveCompletedCallback;
            }
            #endif

            prv_Initialise_New_Stream_বাফার( px_Stream_বাফার,
                                          px_Stream_বাফার->puc_বাফার,
                                          px_Stream_বাফার->x_দৈর্ঘ্য,
                                          px_Stream_বাফার->xTriggerLevelBytes,
                                          px_Stream_বাফার->ucFlags,
                                          pxSendCallback,
                                          pxReceiveCallback );

            #যদি ( configUSE_TRACE_FACILITY == 1 )
            {
                px_Stream_বাফার->ux_Stream_বাফার_সংখ্যা = ux_Stream_বাফার_সংখ্যা;
            }
            #endif

            trace_STREAM_বাফার_RESET( x_Stream_বাফার );

            xReturn = pdPASS;
        }
    }
    taskEXIT_CRITICAL();

    trace_RETURN_x_Stream_বাফার_Reset( xReturn );

    ফেরত xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t x_Stream_বাফার_Reset_From_ISR( Stream_বাফার_Handle_t x_Stream_বাফার )
{
    Stream_বাফার_t * ধ্রুবক px_Stream_বাফার = x_Stream_বাফার;
    BaseType_t xReturn = pdFAIL;
    Stream_বাফার_Callback_Function_t pxSendCallback = ফাঁকা, pxReceiveCallback = ফাঁকা;
    UBaseType_t uxSavedInterruptStatus;

    #যদি ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_Stream_বাফার_সংখ্যা;
    #endif

    trace_ENTER_x_Stream_বাফার_Reset_From_ISR( x_Stream_বাফার );

    configASSERT( px_Stream_বাফার );

    #যদি ( configUSE_TRACE_FACILITY == 1 )
    {
        /* Store the stream buffer number so it can be restored after the
         * reset. */
        ux_Stream_বাফার_সংখ্যা = px_Stream_বাফার->ux_Stream_বাফার_সংখ্যা;
    }
    #endif

    /* Can only reset a message buffer if there are no tasks blocked on it. */
    /* MISRA Ref 4.7.1 [Return value shall be checked] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
    /* coverity[misra_c_2012_directive_4_7_violation] */
    uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
    {
        যদি( ( px_Stream_বাফার->xTaskWaitingToReceive == ফাঁকা ) && ( px_Stream_বাফার->xTaskWaitingToSend == ফাঁকা ) )
        {
            #যদি ( configUSE_SB_COMPLETED_CALLBACK == 1 )
            {
                pxSendCallback = px_Stream_বাফার->pxSendCompletedCallback;
                pxReceiveCallback = px_Stream_বাফার->pxReceiveCompletedCallback;
            }
            #endif

            prv_Initialise_New_Stream_বাফার( px_Stream_বাফার,
                                          px_Stream_বাফার->puc_বাফার,
                                          px_Stream_বাফার->x_দৈর্ঘ্য,
                                          px_Stream_বাফার->xTriggerLevelBytes,
                                          px_Stream_বাফার->ucFlags,
                                          pxSendCallback,
                                          pxReceiveCallback );

            #যদি ( configUSE_TRACE_FACILITY == 1 )
            {
                px_Stream_বাফার->ux_Stream_বাফার_সংখ্যা = ux_Stream_বাফার_সংখ্যা;
            }
            #endif

            trace_STREAM_বাফার_RESET_FROM_ISR( x_Stream_বাফার );

            xReturn = pdPASS;
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( uxSavedInterruptStatus );

    trace_RETURN_x_Stream_বাফার_Reset_From_ISR( xReturn );

    ফেরত xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t x_Stream_বাফার_Set_Trigger_Level( Stream_বাফার_Handle_t x_Stream_বাফার,
                                         size_t xTriggerLevel )
{
    Stream_বাফার_t * ধ্রুবক px_Stream_বাফার = x_Stream_বাফার;
    BaseType_t xReturn;

    trace_ENTER_x_Stream_বাফার_Set_Trigger_Level( x_Stream_বাফার, xTriggerLevel );

    configASSERT( px_Stream_বাফার );

    /* It is not valid for the trigger level to be 0. */
    যদি( xTriggerLevel == ( size_t ) 0 )
    {
        xTriggerLevel = ( size_t ) 1;
    }

    /* The trigger level is the number of bytes that must be in the stream
     * buffer before a task that is waiting for data is unblocked. */
    যদি( xTriggerLevel < px_Stream_বাফার->x_দৈর্ঘ্য )
    {
        px_Stream_বাফার->xTriggerLevelBytes = xTriggerLevel;
        xReturn = pdPASS;
    }
    অন্যথা
    {
        xReturn = pdFALSE;
    }

    trace_RETURN_x_Stream_বাফার_Set_Trigger_Level( xReturn );

    ফেরত xReturn;
}
/*-----------------------------------------------------------*/

size_t x_Stream_বাফার_Spaces_Available( Stream_বাফার_Handle_t x_Stream_বাফার )
{
    ধ্রুবক Stream_বাফার_t * ধ্রুবক px_Stream_বাফার = x_Stream_বাফার;
    size_t xSpace;
    size_t x_Original_লেজ;

    trace_ENTER_x_Stream_বাফার_Spaces_Available( x_Stream_বাফার );

    configASSERT( px_Stream_বাফার );

    /* The code below reads xTail and then xHead.  This is safe if the stream
     * buffer is updated once between the two reads - but not if the stream buffer
     * is updated more than once between the two reads - hence the loop. */
    করো
    {
        x_Original_লেজ = px_Stream_বাফার->x_লেজ;
        xSpace = px_Stream_বাফার->x_দৈর্ঘ্য + px_Stream_বাফার->x_লেজ;
        xSpace -= px_Stream_বাফার->x_শীর্ষ;
    } যখন( x_Original_লেজ != px_Stream_বাফার->x_লেজ );

    xSpace -= ( size_t ) 1;

    যদি( xSpace >= px_Stream_বাফার->x_দৈর্ঘ্য )
    {
        xSpace -= px_Stream_বাফার->x_দৈর্ঘ্য;
    }
    অন্যথা
    {
        mtCOVERAGE_TEST_MARKER();
    }

    trace_RETURN_x_Stream_বাফার_Spaces_Available( xSpace );

    ফেরত xSpace;
}
/*-----------------------------------------------------------*/

size_t x_Stream_বাফার_Bytes_Available( Stream_বাফার_Handle_t x_Stream_বাফার )
{
    ধ্রুবক Stream_বাফার_t * ধ্রুবক px_Stream_বাফার = x_Stream_বাফার;
    size_t xReturn;

    trace_ENTER_x_Stream_বাফার_Bytes_Available( x_Stream_বাফার );

    configASSERT( px_Stream_বাফার );

    xReturn = prv_Bytes_In_বাফার( px_Stream_বাফার );

    trace_RETURN_x_Stream_বাফার_Bytes_Available( xReturn );

    ফেরত xReturn;
}
/*-----------------------------------------------------------*/

size_t x_Stream_বাফার_Send( Stream_বাফার_Handle_t x_Stream_বাফার,
                          ধ্রুবক শূন্য * pv_Tx_তথ্য,
                          size_t x_তথ্য_দৈর্ঘ্য_Bytes,
                          TickType_t xTicksToWait )
{
    Stream_বাফার_t * ধ্রুবক px_Stream_বাফার = x_Stream_বাফার;
    size_t xReturn, xSpace = 0;
    size_t xRequiredSpace = x_তথ্য_দৈর্ঘ্য_Bytes;
    TimeOut_t xTimeOut;
    size_t xMaxReportedSpace = 0;

    trace_ENTER_x_Stream_বাফার_Send( x_Stream_বাফার, pv_Tx_তথ্য, x_তথ্য_দৈর্ঘ্য_Bytes, xTicksToWait );

    configASSERT( pv_Tx_তথ্য );
    configASSERT( px_Stream_বাফার );

    /* The maximum amount of space a stream buffer will ever report is its length
     * minus 1. */
    xMaxReportedSpace = px_Stream_বাফার->x_দৈর্ঘ্য - ( size_t ) 1;

    /* This send function is used to write to both message buffers and stream
     * buffers.  If this is a message buffer then the space needed must be
     * increased by the amount of bytes needed to store the length of the
     * message. */
    যদি( ( px_Stream_বাফার->ucFlags & sb_FLAGS_IS_MESSAGE_বাফার ) != ( uint8_t ) 0 )
    {
        xRequiredSpace += sb_BYTES_TO_STORE_MESSAGE_দৈর্ঘ্য;

        /* Overflow? */
        configASSERT( xRequiredSpace > x_তথ্য_দৈর্ঘ্য_Bytes );

        /* If this is a message buffer then it must be possible to write the
         * whole message. */
        যদি( xRequiredSpace > xMaxReportedSpace )
        {
            /* The message would not fit even if the entire buffer was empty,
             * so don't wait for space. */
            xTicksToWait = ( TickType_t ) 0;
        }
        অন্যথা
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    অন্যথা
    {
        /* If this is a stream buffer then it is acceptable to write only part
         * of the message to the buffer.  Cap the length to the total length of
         * the buffer. */
        যদি( xRequiredSpace > xMaxReportedSpace )
        {
            xRequiredSpace = xMaxReportedSpace;
        }
        অন্যথা
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }

    যদি( xTicksToWait != ( TickType_t ) 0 )
    {
        vTaskSetTimeOutState( &xTimeOut );

        করো
        {
            /* Wait until the required number of bytes are free in the message
             * buffer. */
            taskENTER_CRITICAL();
            {
                xSpace = x_Stream_বাফার_Spaces_Available( px_Stream_বাফার );

                যদি( xSpace < xRequiredSpace )
                {
                    /* Clear notification state as going to wait for space. */
                    ( শূন্য ) xTaskNotifyStateClearIndexed( ফাঁকা, px_Stream_বাফার->ux_Notification_সূচক );

                    /* Should only be one writer. */
                    configASSERT( px_Stream_বাফার->xTaskWaitingToSend == ফাঁকা );
                    px_Stream_বাফার->xTaskWaitingToSend = xTaskGetCurrentTaskHandle();
                }
                অন্যথা
                {
                    taskEXIT_CRITICAL();
                    বিরতি;
                }
            }
            taskEXIT_CRITICAL();

            trace_BLOCKING_ON_STREAM_বাফার_SEND( x_Stream_বাফার );
            ( শূন্য ) xTaskNotifyWaitIndexed( px_Stream_বাফার->ux_Notification_সূচক, ( uint32_t ) 0, ( uint32_t ) 0, ফাঁকা, xTicksToWait );
            px_Stream_বাফার->xTaskWaitingToSend = ফাঁকা;
        } যখন( xTaskCheckForTimeOut( &xTimeOut, &xTicksToWait ) == pdFALSE );
    }
    অন্যথা
    {
        mtCOVERAGE_TEST_MARKER();
    }

    যদি( xSpace == ( size_t ) 0 )
    {
        xSpace = x_Stream_বাফার_Spaces_Available( px_Stream_বাফার );
    }
    অন্যথা
    {
        mtCOVERAGE_TEST_MARKER();
    }

    xReturn = prv_Write_Message_To_বাফার( px_Stream_বাফার, pv_Tx_তথ্য, x_তথ্য_দৈর্ঘ্য_Bytes, xSpace, xRequiredSpace );

    যদি( xReturn > ( size_t ) 0 )
    {
        trace_STREAM_বাফার_SEND( x_Stream_বাফার, xReturn );

        /* Was a task waiting for the data? */
        যদি( prv_Bytes_In_বাফার( px_Stream_বাফার ) >= px_Stream_বাফার->xTriggerLevelBytes )
        {
            prvSEND_COMPLETED( px_Stream_বাফার );
        }
        অন্যথা
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    অন্যথা
    {
        mtCOVERAGE_TEST_MARKER();
        trace_STREAM_বাফার_SEND_FAILED( x_Stream_বাফার );
    }

    trace_RETURN_x_Stream_বাফার_Send( xReturn );

    ফেরত xReturn;
}
/*-----------------------------------------------------------*/

size_t x_Stream_বাফার_Send_From_ISR( Stream_বাফার_Handle_t x_Stream_বাফার,
                                 ধ্রুবক শূন্য * pv_Tx_তথ্য,
                                 size_t x_তথ্য_দৈর্ঘ্য_Bytes,
                                 BaseType_t * ধ্রুবক pxHigherPriorityTaskWoken )
{
    Stream_বাফার_t * ধ্রুবক px_Stream_বাফার = x_Stream_বাফার;
    size_t xReturn, xSpace;
    size_t xRequiredSpace = x_তথ্য_দৈর্ঘ্য_Bytes;

    trace_ENTER_x_Stream_বাফার_Send_From_ISR( x_Stream_বাফার, pv_Tx_তথ্য, x_তথ্য_দৈর্ঘ্য_Bytes, pxHigherPriorityTaskWoken );

    configASSERT( pv_Tx_তথ্য );
    configASSERT( px_Stream_বাফার );

    /* This send function is used to write to both message buffers and stream
     * buffers.  If this is a message buffer then the space needed must be
     * increased by the amount of bytes needed to store the length of the
     * message. */
    যদি( ( px_Stream_বাফার->ucFlags & sb_FLAGS_IS_MESSAGE_বাফার ) != ( uint8_t ) 0 )
    {
        xRequiredSpace += sb_BYTES_TO_STORE_MESSAGE_দৈর্ঘ্য;
    }
    অন্যথা
    {
        mtCOVERAGE_TEST_MARKER();
    }

    xSpace = x_Stream_বাফার_Spaces_Available( px_Stream_বাফার );
    xReturn = prv_Write_Message_To_বাফার( px_Stream_বাফার, pv_Tx_তথ্য, x_তথ্য_দৈর্ঘ্য_Bytes, xSpace, xRequiredSpace );

    যদি( xReturn > ( size_t ) 0 )
    {
        /* Was a task waiting for the data? */
        যদি( prv_Bytes_In_বাফার( px_Stream_বাফার ) >= px_Stream_বাফার->xTriggerLevelBytes )
        {
            /* MISRA Ref 4.7.1 [Return value shall be checked] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
            /* coverity[misra_c_2012_directive_4_7_violation] */
            prvSEND_COMPLETE_FROM_ISR( px_Stream_বাফার, pxHigherPriorityTaskWoken );
        }
        অন্যথা
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    অন্যথা
    {
        mtCOVERAGE_TEST_MARKER();
    }

    trace_STREAM_বাফার_SEND_FROM_ISR( x_Stream_বাফার, xReturn );
    trace_RETURN_x_Stream_বাফার_Send_From_ISR( xReturn );

    ফেরত xReturn;
}
/*-----------------------------------------------------------*/

স্থির size_t prv_Write_Message_To_বাফার( Stream_বাফার_t * ধ্রুবক px_Stream_বাফার,
                                       ধ্রুবক শূন্য * pv_Tx_তথ্য,
                                       size_t x_তথ্য_দৈর্ঘ্য_Bytes,
                                       size_t xSpace,
                                       size_t xRequiredSpace )
{
    size_t x_পরবর্তী_শীর্ষ = px_Stream_বাফার->x_শীর্ষ;
    config_MESSAGE_বাফার_দৈর্ঘ্য_TYPE x_Message_দৈর্ঘ্য;

    যদি( ( px_Stream_বাফার->ucFlags & sb_FLAGS_IS_MESSAGE_বাফার ) != ( uint8_t ) 0 )
    {
        /* This is a message buffer, as opposed to a stream buffer. */

        /* Convert xDataLengthBytes to the message length type. */
        x_Message_দৈর্ঘ্য = ( config_MESSAGE_বাফার_দৈর্ঘ্য_TYPE ) x_তথ্য_দৈর্ঘ্য_Bytes;

        /* Ensure the data length given fits within configMESSAGE_BUFFER_LENGTH_TYPE. */
        configASSERT( ( size_t ) x_Message_দৈর্ঘ্য == x_তথ্য_দৈর্ঘ্য_Bytes );

        যদি( xSpace >= xRequiredSpace )
        {
            /* There is enough space to write both the message length and the message
             * itself into the buffer.  Start by writing the length of the data, the data
             * itself will be written later in this function. */
            x_পরবর্তী_শীর্ষ = prv_Write_Bytes_To_বাফার( px_Stream_বাফার, ( ধ্রুবক uint8_t * ) &( x_Message_দৈর্ঘ্য ), sb_BYTES_TO_STORE_MESSAGE_দৈর্ঘ্য, x_পরবর্তী_শীর্ষ );
        }
        অন্যথা
        {
            /* Not enough space, so do not write data to the buffer. */
            x_তথ্য_দৈর্ঘ্য_Bytes = 0;
        }
    }
    অন্যথা
    {
        /* This is a stream buffer, as opposed to a message buffer, so writing a
         * stream of bytes rather than discrete messages.  Plan to write as many
         * bytes as possible. */
        x_তথ্য_দৈর্ঘ্য_Bytes = configMIN( x_তথ্য_দৈর্ঘ্য_Bytes, xSpace );
    }

    যদি( x_তথ্য_দৈর্ঘ্য_Bytes != ( size_t ) 0 )
    {
        /* Write the data to the buffer. */
        /* MISRA Ref 11.5.5 [Void pointer assignment] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
        /* coverity[misra_c_2012_rule_11_5_violation] */
        px_Stream_বাফার->x_শীর্ষ = prv_Write_Bytes_To_বাফার( px_Stream_বাফার, ( ধ্রুবক uint8_t * ) pv_Tx_তথ্য, x_তথ্য_দৈর্ঘ্য_Bytes, x_পরবর্তী_শীর্ষ );
    }

    ফেরত x_তথ্য_দৈর্ঘ্য_Bytes;
}
/*-----------------------------------------------------------*/

size_t x_Stream_বাফার_Receive( Stream_বাফার_Handle_t x_Stream_বাফার,
                             শূন্য * pv_Rx_তথ্য,
                             size_t x_বাফার_দৈর্ঘ্য_Bytes,
                             TickType_t xTicksToWait )
{
    Stream_বাফার_t * ধ্রুবক px_Stream_বাফার = x_Stream_বাফার;
    size_t x_Received_দৈর্ঘ্য = 0, xBytesAvailable, x_Bytes_To_Store_Message_দৈর্ঘ্য;

    trace_ENTER_x_Stream_বাফার_Receive( x_Stream_বাফার, pv_Rx_তথ্য, x_বাফার_দৈর্ঘ্য_Bytes, xTicksToWait );

    configASSERT( pv_Rx_তথ্য );
    configASSERT( px_Stream_বাফার );

    /* This receive function is used by both message buffers, which store
     * discrete messages, and stream buffers, which store a continuous stream of
     * bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the
     * message. */
    যদি( ( px_Stream_বাফার->ucFlags & sb_FLAGS_IS_MESSAGE_বাফার ) != ( uint8_t ) 0 )
    {
        x_Bytes_To_Store_Message_দৈর্ঘ্য = sb_BYTES_TO_STORE_MESSAGE_দৈর্ঘ্য;
    }
    অন্যথা যদি( ( px_Stream_বাফার->ucFlags & sb_FLAGS_IS_BATCHING_বাফার ) != ( uint8_t ) 0 )
    {
        /* Force task to block if the batching buffer contains less bytes than
         * the trigger level. */
        x_Bytes_To_Store_Message_দৈর্ঘ্য = px_Stream_বাফার->xTriggerLevelBytes;
    }
    অন্যথা
    {
        x_Bytes_To_Store_Message_দৈর্ঘ্য = 0;
    }

    যদি( xTicksToWait != ( TickType_t ) 0 )
    {
        /* Checking if there is data and clearing the notification state must be
         * performed atomically. */
        taskENTER_CRITICAL();
        {
            xBytesAvailable = prv_Bytes_In_বাফার( px_Stream_বাফার );

            /* If this function was invoked by a message buffer read then
             * xBytesToStoreMessageLength holds the number of bytes used to hold
             * the length of the next discrete message.  If this function was
             * invoked by a stream buffer read then xBytesToStoreMessageLength will
             * be 0. If this function was invoked by a stream batch buffer read
             * then xBytesToStoreMessageLength will be xTriggerLevelBytes value
             * for the buffer.*/
            যদি( xBytesAvailable <= x_Bytes_To_Store_Message_দৈর্ঘ্য )
            {
                /* Clear notification state as going to wait for data. */
                ( শূন্য ) xTaskNotifyStateClearIndexed( ফাঁকা, px_Stream_বাফার->ux_Notification_সূচক );

                /* Should only be one reader. */
                configASSERT( px_Stream_বাফার->xTaskWaitingToReceive == ফাঁকা );
                px_Stream_বাফার->xTaskWaitingToReceive = xTaskGetCurrentTaskHandle();
            }
            অন্যথা
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        taskEXIT_CRITICAL();

        যদি( xBytesAvailable <= x_Bytes_To_Store_Message_দৈর্ঘ্য )
        {
            /* Wait for data to be available. */
            trace_BLOCKING_ON_STREAM_বাফার_RECEIVE( x_Stream_বাফার );
            ( শূন্য ) xTaskNotifyWaitIndexed( px_Stream_বাফার->ux_Notification_সূচক, ( uint32_t ) 0, ( uint32_t ) 0, ফাঁকা, xTicksToWait );
            px_Stream_বাফার->xTaskWaitingToReceive = ফাঁকা;

            /* Recheck the data available after blocking. */
            xBytesAvailable = prv_Bytes_In_বাফার( px_Stream_বাফার );
        }
        অন্যথা
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    অন্যথা
    {
        xBytesAvailable = prv_Bytes_In_বাফার( px_Stream_বাফার );
    }

    /* Whether receiving a discrete message (where xBytesToStoreMessageLength
     * holds the number of bytes used to store the message length) or a stream of
     * bytes (where xBytesToStoreMessageLength is zero), the number of bytes
     * available must be greater than xBytesToStoreMessageLength to be able to
     * read bytes from the buffer. */
    যদি( xBytesAvailable > x_Bytes_To_Store_Message_দৈর্ঘ্য )
    {
        x_Received_দৈর্ঘ্য = prv_Read_Message_From_বাফার( px_Stream_বাফার, pv_Rx_তথ্য, x_বাফার_দৈর্ঘ্য_Bytes, xBytesAvailable );

        /* Was a task waiting for space in the buffer? */
        যদি( x_Received_দৈর্ঘ্য != ( size_t ) 0 )
        {
            trace_STREAM_বাফার_RECEIVE( x_Stream_বাফার, x_Received_দৈর্ঘ্য );
            prvRECEIVE_COMPLETED( x_Stream_বাফার );
        }
        অন্যথা
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    অন্যথা
    {
        trace_STREAM_বাফার_RECEIVE_FAILED( x_Stream_বাফার );
        mtCOVERAGE_TEST_MARKER();
    }

    trace_RETURN_x_Stream_বাফার_Receive( x_Received_দৈর্ঘ্য );

    ফেরত x_Received_দৈর্ঘ্য;
}
/*-----------------------------------------------------------*/

size_t x_Stream_বাফার_পরবর্তী_Message_দৈর্ঘ্য_Bytes( Stream_বাফার_Handle_t x_Stream_বাফার )
{
    Stream_বাফার_t * ধ্রুবক px_Stream_বাফার = x_Stream_বাফার;
    size_t xReturn, xBytesAvailable;
    config_MESSAGE_বাফার_দৈর্ঘ্য_TYPE x_অস্থায়ী_Return;

    trace_ENTER_x_Stream_বাফার_পরবর্তী_Message_দৈর্ঘ্য_Bytes( x_Stream_বাফার );

    configASSERT( px_Stream_বাফার );

    /* Ensure the stream buffer is being used as a message buffer. */
    যদি( ( px_Stream_বাফার->ucFlags & sb_FLAGS_IS_MESSAGE_বাফার ) != ( uint8_t ) 0 )
    {
        xBytesAvailable = prv_Bytes_In_বাফার( px_Stream_বাফার );

        যদি( xBytesAvailable > sb_BYTES_TO_STORE_MESSAGE_দৈর্ঘ্য )
        {
            /* The number of bytes available is greater than the number of bytes
             * required to hold the length of the next message, so another message
             * is available. */
            ( শূন্য ) prv_Read_Bytes_From_বাফার( px_Stream_বাফার, ( uint8_t * ) &x_অস্থায়ী_Return, sb_BYTES_TO_STORE_MESSAGE_দৈর্ঘ্য, px_Stream_বাফার->x_লেজ );
            xReturn = ( size_t ) x_অস্থায়ী_Return;
        }
        অন্যথা
        {
            /* The minimum amount of bytes in a message buffer is
             * ( sbBYTES_TO_STORE_MESSAGE_LENGTH + 1 ), so if xBytesAvailable is
             * less than sbBYTES_TO_STORE_MESSAGE_LENGTH the only other valid
             * value is 0. */
            configASSERT( xBytesAvailable == 0 );
            xReturn = 0;
        }
    }
    অন্যথা
    {
        xReturn = 0;
    }

    trace_RETURN_x_Stream_বাফার_পরবর্তী_Message_দৈর্ঘ্য_Bytes( xReturn );

    ফেরত xReturn;
}
/*-----------------------------------------------------------*/

size_t x_Stream_বাফার_Receive_From_ISR( Stream_বাফার_Handle_t x_Stream_বাফার,
                                    শূন্য * pv_Rx_তথ্য,
                                    size_t x_বাফার_দৈর্ঘ্য_Bytes,
                                    BaseType_t * ধ্রুবক pxHigherPriorityTaskWoken )
{
    Stream_বাফার_t * ধ্রুবক px_Stream_বাফার = x_Stream_বাফার;
    size_t x_Received_দৈর্ঘ্য = 0, xBytesAvailable, x_Bytes_To_Store_Message_দৈর্ঘ্য;

    trace_ENTER_x_Stream_বাফার_Receive_From_ISR( x_Stream_বাফার, pv_Rx_তথ্য, x_বাফার_দৈর্ঘ্য_Bytes, pxHigherPriorityTaskWoken );

    configASSERT( pv_Rx_তথ্য );
    configASSERT( px_Stream_বাফার );

    /* This receive function is used by both message buffers, which store
     * discrete messages, and stream buffers, which store a continuous stream of
     * bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the
     * message. */
    যদি( ( px_Stream_বাফার->ucFlags & sb_FLAGS_IS_MESSAGE_বাফার ) != ( uint8_t ) 0 )
    {
        x_Bytes_To_Store_Message_দৈর্ঘ্য = sb_BYTES_TO_STORE_MESSAGE_দৈর্ঘ্য;
    }
    অন্যথা
    {
        x_Bytes_To_Store_Message_দৈর্ঘ্য = 0;
    }

    xBytesAvailable = prv_Bytes_In_বাফার( px_Stream_বাফার );

    /* Whether receiving a discrete message (where xBytesToStoreMessageLength
     * holds the number of bytes used to store the message length) or a stream of
     * bytes (where xBytesToStoreMessageLength is zero), the number of bytes
     * available must be greater than xBytesToStoreMessageLength to be able to
     * read bytes from the buffer. */
    যদি( xBytesAvailable > x_Bytes_To_Store_Message_দৈর্ঘ্য )
    {
        x_Received_দৈর্ঘ্য = prv_Read_Message_From_বাফার( px_Stream_বাফার, pv_Rx_তথ্য, x_বাফার_দৈর্ঘ্য_Bytes, xBytesAvailable );

        /* Was a task waiting for space in the buffer? */
        যদি( x_Received_দৈর্ঘ্য != ( size_t ) 0 )
        {
            /* MISRA Ref 4.7.1 [Return value shall be checked] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
            /* coverity[misra_c_2012_directive_4_7_violation] */
            prvRECEIVE_COMPLETED_FROM_ISR( px_Stream_বাফার, pxHigherPriorityTaskWoken );
        }
        অন্যথা
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    অন্যথা
    {
        mtCOVERAGE_TEST_MARKER();
    }

    trace_STREAM_বাফার_RECEIVE_FROM_ISR( x_Stream_বাফার, x_Received_দৈর্ঘ্য );
    trace_RETURN_x_Stream_বাফার_Receive_From_ISR( x_Received_দৈর্ঘ্য );

    ফেরত x_Received_দৈর্ঘ্য;
}
/*-----------------------------------------------------------*/

স্থির size_t prv_Read_Message_From_বাফার( Stream_বাফার_t * px_Stream_বাফার,
                                        শূন্য * pv_Rx_তথ্য,
                                        size_t x_বাফার_দৈর্ঘ্য_Bytes,
                                        size_t xBytesAvailable )
{
    size_t x_গুনতি, x_পরবর্তী_Message_দৈর্ঘ্য;
    config_MESSAGE_বাফার_দৈর্ঘ্য_TYPE x_অস্থায়ী_পরবর্তী_Message_দৈর্ঘ্য;
    size_t x_পরবর্তী_লেজ = px_Stream_বাফার->x_লেজ;

    যদি( ( px_Stream_বাফার->ucFlags & sb_FLAGS_IS_MESSAGE_বাফার ) != ( uint8_t ) 0 )
    {
        /* A discrete message is being received.  First receive the length
         * of the message. */
        x_পরবর্তী_লেজ = prv_Read_Bytes_From_বাফার( px_Stream_বাফার, ( uint8_t * ) &x_অস্থায়ী_পরবর্তী_Message_দৈর্ঘ্য, sb_BYTES_TO_STORE_MESSAGE_দৈর্ঘ্য, x_পরবর্তী_লেজ );
        x_পরবর্তী_Message_দৈর্ঘ্য = ( size_t ) x_অস্থায়ী_পরবর্তী_Message_দৈর্ঘ্য;

        /* Reduce the number of bytes available by the number of bytes just
         * read out. */
        xBytesAvailable -= sb_BYTES_TO_STORE_MESSAGE_দৈর্ঘ্য;

        /* Check there is enough space in the buffer provided by the
         * user. */
        যদি( x_পরবর্তী_Message_দৈর্ঘ্য > x_বাফার_দৈর্ঘ্য_Bytes )
        {
            /* The user has provided insufficient space to read the message. */
            x_পরবর্তী_Message_দৈর্ঘ্য = 0;
        }
        অন্যথা
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    অন্যথা
    {
        /* A stream of bytes is being received (as opposed to a discrete
         * message), so read as many bytes as possible. */
        x_পরবর্তী_Message_দৈর্ঘ্য = x_বাফার_দৈর্ঘ্য_Bytes;
    }

    /* Use the minimum of the wanted bytes and the available bytes. */
    x_গুনতি = configMIN( x_পরবর্তী_Message_দৈর্ঘ্য, xBytesAvailable );

    যদি( x_গুনতি != ( size_t ) 0 )
    {
        /* Read the actual data and update the tail to mark the data as officially consumed. */
        /* MISRA Ref 11.5.5 [Void pointer assignment] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
        /* coverity[misra_c_2012_rule_11_5_violation] */
        px_Stream_বাফার->x_লেজ = prv_Read_Bytes_From_বাফার( px_Stream_বাফার, ( uint8_t * ) pv_Rx_তথ্য, x_গুনতি, x_পরবর্তী_লেজ );
    }

    ফেরত x_গুনতি;
}
/*-----------------------------------------------------------*/

BaseType_t x_Stream_বাফার_Is_Empty( Stream_বাফার_Handle_t x_Stream_বাফার )
{
    ধ্রুবক Stream_বাফার_t * ধ্রুবক px_Stream_বাফার = x_Stream_বাফার;
    BaseType_t xReturn;
    size_t x_লেজ;

    trace_ENTER_x_Stream_বাফার_Is_Empty( x_Stream_বাফার );

    configASSERT( px_Stream_বাফার );

    /* True if no bytes are available. */
    x_লেজ = px_Stream_বাফার->x_লেজ;

    যদি( px_Stream_বাফার->x_শীর্ষ == x_লেজ )
    {
        xReturn = pdTRUE;
    }
    অন্যথা
    {
        xReturn = pdFALSE;
    }

    trace_RETURN_x_Stream_বাফার_Is_Empty( xReturn );

    ফেরত xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t x_Stream_বাফার_Is_Full( Stream_বাফার_Handle_t x_Stream_বাফার )
{
    BaseType_t xReturn;
    size_t x_Bytes_To_Store_Message_দৈর্ঘ্য;
    ধ্রুবক Stream_বাফার_t * ধ্রুবক px_Stream_বাফার = x_Stream_বাফার;

    trace_ENTER_x_Stream_বাফার_Is_Full( x_Stream_বাফার );

    configASSERT( px_Stream_বাফার );

    /* This generic version of the receive function is used by both message
     * buffers, which store discrete messages, and stream buffers, which store a
     * continuous stream of bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the message. */
    যদি( ( px_Stream_বাফার->ucFlags & sb_FLAGS_IS_MESSAGE_বাফার ) != ( uint8_t ) 0 )
    {
        x_Bytes_To_Store_Message_দৈর্ঘ্য = sb_BYTES_TO_STORE_MESSAGE_দৈর্ঘ্য;
    }
    অন্যথা
    {
        x_Bytes_To_Store_Message_দৈর্ঘ্য = 0;
    }

    /* True if the available space equals zero. */
    যদি( x_Stream_বাফার_Spaces_Available( x_Stream_বাফার ) <= x_Bytes_To_Store_Message_দৈর্ঘ্য )
    {
        xReturn = pdTRUE;
    }
    অন্যথা
    {
        xReturn = pdFALSE;
    }

    trace_RETURN_x_Stream_বাফার_Is_Full( xReturn );

    ফেরত xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t x_Stream_বাফার_Send_Completed_From_ISR( Stream_বাফার_Handle_t x_Stream_বাফার,
                                              BaseType_t * pxHigherPriorityTaskWoken )
{
    Stream_বাফার_t * ধ্রুবক px_Stream_বাফার = x_Stream_বাফার;
    BaseType_t xReturn;
    UBaseType_t uxSavedInterruptStatus;

    trace_ENTER_x_Stream_বাফার_Send_Completed_From_ISR( x_Stream_বাফার, pxHigherPriorityTaskWoken );

    configASSERT( px_Stream_বাফার );

    /* MISRA Ref 4.7.1 [Return value shall be checked] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
    /* coverity[misra_c_2012_directive_4_7_violation] */
    uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
    {
        যদি( ( px_Stream_বাফার )->xTaskWaitingToReceive != ফাঁকা )
        {
            ( শূন্য ) xTaskNotifyIndexedFromISR( ( px_Stream_বাফার )->xTaskWaitingToReceive,
                                                ( px_Stream_বাফার )->ux_Notification_সূচক,
                                                ( uint32_t ) 0,
                                                eNoAction,
                                                pxHigherPriorityTaskWoken );
            ( px_Stream_বাফার )->xTaskWaitingToReceive = ফাঁকা;
            xReturn = pdTRUE;
        }
        অন্যথা
        {
            xReturn = pdFALSE;
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( uxSavedInterruptStatus );

    trace_RETURN_x_Stream_বাফার_Send_Completed_From_ISR( xReturn );

    ফেরত xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t x_Stream_বাফার_Receive_Completed_From_ISR( Stream_বাফার_Handle_t x_Stream_বাফার,
                                                 BaseType_t * pxHigherPriorityTaskWoken )
{
    Stream_বাফার_t * ধ্রুবক px_Stream_বাফার = x_Stream_বাফার;
    BaseType_t xReturn;
    UBaseType_t uxSavedInterruptStatus;

    trace_ENTER_x_Stream_বাফার_Receive_Completed_From_ISR( x_Stream_বাফার, pxHigherPriorityTaskWoken );

    configASSERT( px_Stream_বাফার );

    /* MISRA Ref 4.7.1 [Return value shall be checked] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
    /* coverity[misra_c_2012_directive_4_7_violation] */
    uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
    {
        যদি( ( px_Stream_বাফার )->xTaskWaitingToSend != ফাঁকা )
        {
            ( শূন্য ) xTaskNotifyIndexedFromISR( ( px_Stream_বাফার )->xTaskWaitingToSend,
                                                ( px_Stream_বাফার )->ux_Notification_সূচক,
                                                ( uint32_t ) 0,
                                                eNoAction,
                                                pxHigherPriorityTaskWoken );
            ( px_Stream_বাফার )->xTaskWaitingToSend = ফাঁকা;
            xReturn = pdTRUE;
        }
        অন্যথা
        {
            xReturn = pdFALSE;
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( uxSavedInterruptStatus );

    trace_RETURN_x_Stream_বাফার_Receive_Completed_From_ISR( xReturn );

    ফেরত xReturn;
}
/*-----------------------------------------------------------*/

স্থির size_t prv_Write_Bytes_To_বাফার( Stream_বাফার_t * ধ্রুবক px_Stream_বাফার,
                                     ধ্রুবক uint8_t * puc_তথ্য,
                                     size_t x_গুনতি,
                                     size_t x_শীর্ষ )
{
    size_t x_First_দৈর্ঘ্য;

    configASSERT( x_গুনতি > ( size_t ) 0 );

    /* Calculate the number of bytes that can be added in the first write -
     * which may be less than the total number of bytes that need to be added if
     * the buffer will wrap back to the beginning. */
    x_First_দৈর্ঘ্য = configMIN( px_Stream_বাফার->x_দৈর্ঘ্য - x_শীর্ষ, x_গুনতি );

    /* Write as many bytes as can be written in the first write. */
    configASSERT( ( x_শীর্ষ + x_First_দৈর্ঘ্য ) <= px_Stream_বাফার->x_দৈর্ঘ্য );
    ( শূন্য ) memcpy( ( শূন্য * ) ( &( px_Stream_বাফার->puc_বাফার[ x_শীর্ষ ] ) ), ( ধ্রুবক শূন্য * ) puc_তথ্য, x_First_দৈর্ঘ্য );

    /* If the number of bytes written was less than the number that could be
     * written in the first write... */
    যদি( x_গুনতি > x_First_দৈর্ঘ্য )
    {
        /* ...then write the remaining bytes to the start of the buffer. */
        configASSERT( ( x_গুনতি - x_First_দৈর্ঘ্য ) <= px_Stream_বাফার->x_দৈর্ঘ্য );
        ( শূন্য ) memcpy( ( শূন্য * ) px_Stream_বাফার->puc_বাফার, ( ধ্রুবক শূন্য * ) &( puc_তথ্য[ x_First_দৈর্ঘ্য ] ), x_গুনতি - x_First_দৈর্ঘ্য );
    }
    অন্যথা
    {
        mtCOVERAGE_TEST_MARKER();
    }

    x_শীর্ষ += x_গুনতি;

    যদি( x_শীর্ষ >= px_Stream_বাফার->x_দৈর্ঘ্য )
    {
        x_শীর্ষ -= px_Stream_বাফার->x_দৈর্ঘ্য;
    }
    অন্যথা
    {
        mtCOVERAGE_TEST_MARKER();
    }

    ফেরত x_শীর্ষ;
}
/*-----------------------------------------------------------*/

স্থির size_t prv_Read_Bytes_From_বাফার( Stream_বাফার_t * px_Stream_বাফার,
                                      uint8_t * puc_তথ্য,
                                      size_t x_গুনতি,
                                      size_t x_লেজ )
{
    size_t x_First_দৈর্ঘ্য;

    configASSERT( x_গুনতি != ( size_t ) 0 );

    /* Calculate the number of bytes that can be read - which may be
     * less than the number wanted if the data wraps around to the start of
     * the buffer. */
    x_First_দৈর্ঘ্য = configMIN( px_Stream_বাফার->x_দৈর্ঘ্য - x_লেজ, x_গুনতি );

    /* Obtain the number of bytes it is possible to obtain in the first
     * read.  Asserts check bounds of read and write. */
    configASSERT( x_First_দৈর্ঘ্য <= x_গুনতি );
    configASSERT( ( x_লেজ + x_First_দৈর্ঘ্য ) <= px_Stream_বাফার->x_দৈর্ঘ্য );
    ( শূন্য ) memcpy( ( শূন্য * ) puc_তথ্য, ( ধ্রুবক শূন্য * ) &( px_Stream_বাফার->puc_বাফার[ x_লেজ ] ), x_First_দৈর্ঘ্য );

    /* If the total number of wanted bytes is greater than the number
     * that could be read in the first read... */
    যদি( x_গুনতি > x_First_দৈর্ঘ্য )
    {
        /* ...then read the remaining bytes from the start of the buffer. */
        ( শূন্য ) memcpy( ( শূন্য * ) &( puc_তথ্য[ x_First_দৈর্ঘ্য ] ), ( শূন্য * ) ( px_Stream_বাফার->puc_বাফার ), x_গুনতি - x_First_দৈর্ঘ্য );
    }
    অন্যথা
    {
        mtCOVERAGE_TEST_MARKER();
    }

    /* Move the tail pointer to effectively remove the data read from the buffer. */
    x_লেজ += x_গুনতি;

    যদি( x_লেজ >= px_Stream_বাফার->x_দৈর্ঘ্য )
    {
        x_লেজ -= px_Stream_বাফার->x_দৈর্ঘ্য;
    }

    ফেরত x_লেজ;
}
/*-----------------------------------------------------------*/

স্থির size_t prv_Bytes_In_বাফার( ধ্রুবক Stream_বাফার_t * ধ্রুবক px_Stream_বাফার )
{
    /* Returns the distance between xTail and xHead. */
    size_t x_গুনতি;

    x_গুনতি = px_Stream_বাফার->x_দৈর্ঘ্য + px_Stream_বাফার->x_শীর্ষ;
    x_গুনতি -= px_Stream_বাফার->x_লেজ;

    যদি( x_গুনতি >= px_Stream_বাফার->x_দৈর্ঘ্য )
    {
        x_গুনতি -= px_Stream_বাফার->x_দৈর্ঘ্য;
    }
    অন্যথা
    {
        mtCOVERAGE_TEST_MARKER();
    }

    ফেরত x_গুনতি;
}
/*-----------------------------------------------------------*/

স্থির শূন্য prv_Initialise_New_Stream_বাফার( Stream_বাফার_t * ধ্রুবক px_Stream_বাফার,
                                          uint8_t * ধ্রুবক puc_বাফার,
                                          size_t x_বাফার_আকার_Bytes,
                                          size_t xTriggerLevelBytes,
                                          uint8_t ucFlags,
                                          Stream_বাফার_Callback_Function_t pxSendCompletedCallback,
                                          Stream_বাফার_Callback_Function_t pxReceiveCompletedCallback )
{
    /* Assert here is deliberately writing to the entire buffer to ensure it can
     * be written to without generating exceptions, and is setting the buffer to a
     * known value to assist in development/debugging. */
    #যদি ( configASSERT_DEFINED == 1 )
    {
        /* The value written just has to be identifiable when looking at the
         * memory.  Don't use 0xA5 as that is the stack fill value and could
         * result in confusion as to what is actually being observed. */
        #define STREAM_বাফার_বাফার_WRITE_মান    ( 0x55 )
        configASSERT( memset( puc_বাফার, ( পূর্ণসংখ্যা ) STREAM_বাফার_বাফার_WRITE_মান, x_বাফার_আকার_Bytes ) == puc_বাফার );
    }
    #endif

    ( শূন্য ) memset( ( শূন্য * ) px_Stream_বাফার, 0x00, আকার( Stream_বাফার_t ) );
    px_Stream_বাফার->puc_বাফার = puc_বাফার;
    px_Stream_বাফার->x_দৈর্ঘ্য = x_বাফার_আকার_Bytes;
    px_Stream_বাফার->xTriggerLevelBytes = xTriggerLevelBytes;
    px_Stream_বাফার->ucFlags = ucFlags;
    px_Stream_বাফার->ux_Notification_সূচক = tsk_DEFAULT_সূচক_TO_NOTIFY;
    #যদি ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    {
        px_Stream_বাফার->pxSendCompletedCallback = pxSendCompletedCallback;
        px_Stream_বাফার->pxReceiveCompletedCallback = pxReceiveCompletedCallback;
    }
    #অন্যথা
    {
        /* MISRA Ref 11.1.1 [Object type casting] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-111 */
        /* coverity[misra_c_2012_rule_11_1_violation] */
        ( শূন্য ) pxSendCompletedCallback;

        /* MISRA Ref 11.1.1 [Object type casting] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-111 */
        /* coverity[misra_c_2012_rule_11_1_violation] */
        ( শূন্য ) pxReceiveCompletedCallback;
    }
    #endif /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
}
/*-----------------------------------------------------------*/

UBaseType_t ux_Stream_বাফার_Get_Stream_বাফার_Notification_সূচক( Stream_বাফার_Handle_t x_Stream_বাফার )
{
    Stream_বাফার_t * ধ্রুবক px_Stream_বাফার = x_Stream_বাফার;

    trace_ENTER_ux_Stream_বাফার_Get_Stream_বাফার_Notification_সূচক( x_Stream_বাফার );

    configASSERT( px_Stream_বাফার );

    trace_RETURN_ux_Stream_বাফার_Get_Stream_বাফার_Notification_সূচক( px_Stream_বাফার->ux_Notification_সূচক );

    ফেরত px_Stream_বাফার->ux_Notification_সূচক;
}
/*-----------------------------------------------------------*/

শূন্য v_Stream_বাফার_Set_Stream_বাফার_Notification_সূচক( Stream_বাফার_Handle_t x_Stream_বাফার,
                                                    UBaseType_t ux_Notification_সূচক )
{
    Stream_বাফার_t * ধ্রুবক px_Stream_বাফার = x_Stream_বাফার;

    trace_ENTER_v_Stream_বাফার_Set_Stream_বাফার_Notification_সূচক( x_Stream_বাফার, ux_Notification_সূচক );

    configASSERT( px_Stream_বাফার );

    /* There should be no task waiting otherwise we'd never resume them. */
    configASSERT( px_Stream_বাফার->xTaskWaitingToReceive == ফাঁকা );
    configASSERT( px_Stream_বাফার->xTaskWaitingToSend == ফাঁকা );

    /* Check that the task notification index is valid. */
    configASSERT( ux_Notification_সূচক < configTASK_NOTIFICATION_ARRAY_ENTRIES );

    px_Stream_বাফার->ux_Notification_সূচক = ux_Notification_সূচক;

    trace_RETURN_v_Stream_বাফার_Set_Stream_বাফার_Notification_সূচক();
}
/*-----------------------------------------------------------*/

    #যদি ( configUSE_TRACE_FACILITY == 1 )

    UBaseType_t ux_Stream_বাফার_Get_Stream_বাফার_সংখ্যা( Stream_বাফার_Handle_t x_Stream_বাফার )
    {
        trace_ENTER_ux_Stream_বাফার_Get_Stream_বাফার_সংখ্যা( x_Stream_বাফার );

        trace_RETURN_ux_Stream_বাফার_Get_Stream_বাফার_সংখ্যা( x_Stream_বাফার->ux_Stream_বাফার_সংখ্যা );

        ফেরত x_Stream_বাফার->ux_Stream_বাফার_সংখ্যা;
    }

    #endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

    #যদি ( configUSE_TRACE_FACILITY == 1 )

    শূন্য v_Stream_বাফার_Set_Stream_বাফার_সংখ্যা( Stream_বাফার_Handle_t x_Stream_বাফার,
                                             UBaseType_t ux_Stream_বাফার_সংখ্যা )
    {
        trace_ENTER_v_Stream_বাফার_Set_Stream_বাফার_সংখ্যা( x_Stream_বাফার, ux_Stream_বাফার_সংখ্যা );

        x_Stream_বাফার->ux_Stream_বাফার_সংখ্যা = ux_Stream_বাফার_সংখ্যা;

        trace_RETURN_v_Stream_বাফার_Set_Stream_বাফার_সংখ্যা();
    }

    #endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

    #যদি ( configUSE_TRACE_FACILITY == 1 )

    uint8_t uc_Stream_বাফার_Get_Stream_বাফার_Type( Stream_বাফার_Handle_t x_Stream_বাফার )
    {
        trace_ENTER_uc_Stream_বাফার_Get_Stream_বাফার_Type( x_Stream_বাফার );

        trace_RETURN_uc_Stream_বাফার_Get_Stream_বাফার_Type( ( uint8_t ) ( x_Stream_বাফার->ucFlags & sb_FLAGS_IS_MESSAGE_বাফার ) );

        ফেরত( ( uint8_t ) ( x_Stream_বাফার->ucFlags & sb_FLAGS_IS_MESSAGE_বাফার ) );
    }

    #endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

/* This entire source file will be skipped if the application is not configured
 * to include stream buffer functionality. This #if is closed at the very bottom
 * of this file. If you want to include stream buffers then ensure
 * configUSE_STREAM_BUFFERS is set to 1 in FreeRTOSConfig.h. */
#endif /* configUSE_STREAM_BUFFERS == 1 */
