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
#অন্তর্ভাব <stdint.h>
#অন্তর্ভাব <string.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* FreeRTOS includes. */
#অন্তর্ভাব "FreeRTOS.h"
#অন্তর্ভাব "task.h"
#অন্তর্ভাব "stream_buffer.h"
/* Include private IDF API additions for critical thread safety macros */
#অন্তর্ভাব "esp_private/freertos_idf_additions_priv.h"

#যদি ( configUSE_TASK_NOTIFICATIONS != 1 )
    #error configUSE_TASK_NOTIFICATIONS must be set to 1 to build stream_বাফার.c
#endif

#যদি ( INCLUDE_xTaskGetCurrentTaskHandle != 1 )
    #error INCLUDE_xTaskGetCurrentTaskHandle must be set to 1 to build stream_বাফার.c
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
    #define sbRECEIVE_COMPLETED( px_Stream_বাফার )                               \
    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( px_Stream_বাফার->x_Stream_বাফার_Lock ) ); \
    {                                                                           \
        যদি( ( px_Stream_বাফার )->xTaskWaitingToSend != ফাঁকা )                    \
        {                                                                       \
            ( শূন্য ) xTaskNotify( ( px_Stream_বাফার )->xTaskWaitingToSend,       \
                                  ( uint32_t ) 0,                               \
                                  eNoAction );                                  \
            ( px_Stream_বাফার )->xTaskWaitingToSend = ফাঁকা;                      \
        }                                                                       \
    }                                                                           \
    ( শূন্য ) prvEXIT_CRITICAL_OR_RESUME_ALL( &( px_Stream_বাফার->x_Stream_বাফার_Lock ) );
#endif /* sbRECEIVE_COMPLETED */

/* If user has provided a per-instance receive complete callback, then
 * invoke the callback else use the receive complete macro which is provided by default for all instances.
 */
#যদি ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #define prvRECEIVE_COMPLETED( px_Stream_বাফার )                                               \
    {                                                                                            \
        যদি( ( px_Stream_বাফার )->pxReceiveCompletedCallback != ফাঁকা )                             \
        {                                                                                        \
            ( px_Stream_বাফার )->pxReceiveCompletedCallback( ( px_Stream_বাফার ), pdFALSE, ফাঁকা ); \
        }                                                                                        \
        অন্যথা                                                                                     \
        {                                                                                        \
            sbRECEIVE_COMPLETED( ( px_Stream_বাফার ) );                                           \
        }                                                                                        \
    }
#অন্যথা /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #define prvRECEIVE_COMPLETED( px_Stream_বাফার )    sbRECEIVE_COMPLETED( ( px_Stream_বাফার ) )
#endif /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

#ifndef sbRECEIVE_COMPLETED_FROM_ISR
    #define sbRECEIVE_COMPLETED_FROM_ISR( px_Stream_বাফার,                            \
                                          pxHigherPriorityTaskWoken )                \
    {                                                                                \
        UBaseType_t uxSavedInterruptStatus;                                          \
                                                                                     \
        uxSavedInterruptStatus = ( UBaseType_t ) portSET_INTERRUPT_MASK_FROM_ISR();  \
        {                                                                            \
            যদি( ( px_Stream_বাফার )->xTaskWaitingToSend != ফাঁকা )                     \
            {                                                                        \
                ( শূন্য ) xTaskNotifyFromISR( ( px_Stream_বাফার )->xTaskWaitingToSend, \
                                             ( uint32_t ) 0,                         \
                                             eNoAction,                              \
                                             ( pxHigherPriorityTaskWoken ) );        \
                ( px_Stream_বাফার )->xTaskWaitingToSend = ফাঁকা;                       \
            }                                                                        \
        }                                                                            \
        portCLEAR_INTERRUPT_MASK_FROM_ISR( uxSavedInterruptStatus );                 \
    }
#endif /* sbRECEIVE_COMPLETED_FROM_ISR */

#যদি ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #define prvRECEIVE_COMPLETED_FROM_ISR( px_Stream_বাফার,                                                               \
                                           pxHigherPriorityTaskWoken )                                                   \
    {                                                                                                                    \
        যদি( ( px_Stream_বাফার )->pxReceiveCompletedCallback != ফাঁকা )                                                     \
        {                                                                                                                \
            ( px_Stream_বাফার )->pxReceiveCompletedCallback( ( px_Stream_বাফার ), pdTRUE, ( pxHigherPriorityTaskWoken ) ); \
        }                                                                                                                \
        অন্যথা                                                                                                             \
        {                                                                                                                \
            sbRECEIVE_COMPLETED_FROM_ISR( ( px_Stream_বাফার ), ( pxHigherPriorityTaskWoken ) );                           \
        }                                                                                                                \
    }
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
    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( px_Stream_বাফার->x_Stream_বাফার_Lock ) ); \
    {                                                                           \
        যদি( ( px_Stream_বাফার )->xTaskWaitingToReceive != ফাঁকা )                 \
        {                                                                       \
            ( শূন্য ) xTaskNotify( ( px_Stream_বাফার )->xTaskWaitingToReceive,    \
                                  ( uint32_t ) 0,                               \
                                  eNoAction );                                  \
            ( px_Stream_বাফার )->xTaskWaitingToReceive = ফাঁকা;                   \
        }                                                                       \
    }                                                                           \
    ( শূন্য ) prvEXIT_CRITICAL_OR_RESUME_ALL( &( px_Stream_বাফার->x_Stream_বাফার_Lock ) );
#endif /* sbSEND_COMPLETED */

/* If user has provided a per-instance send completed callback, then
 * invoke the callback else use the send complete macro which is provided by default for all instances.
 */
#যদি ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #define prvSEND_COMPLETED( px_Stream_বাফার )                                           \
    {                                                                                     \
        যদি( ( px_Stream_বাফার )->pxSendCompletedCallback != ফাঁকা )                         \
        {                                                                                 \
            px_Stream_বাফার->pxSendCompletedCallback( ( px_Stream_বাফার ), pdFALSE, ফাঁকা ); \
        }                                                                                 \
        অন্যথা                                                                              \
        {                                                                                 \
            sbSEND_COMPLETED( ( px_Stream_বাফার ) );                                       \
        }                                                                                 \
    }
#অন্যথা /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #define prvSEND_COMPLETED( px_Stream_বাফার )    sbSEND_COMPLETED( ( px_Stream_বাফার ) )
#endif /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */


#ifndef sbSEND_COMPLETE_FROM_ISR
    #define sbSEND_COMPLETE_FROM_ISR( px_Stream_বাফার, pxHigherPriorityTaskWoken )       \
    {                                                                                   \
        UBaseType_t uxSavedInterruptStatus;                                             \
                                                                                        \
        uxSavedInterruptStatus = ( UBaseType_t ) portSET_INTERRUPT_MASK_FROM_ISR();     \
        {                                                                               \
            যদি( ( px_Stream_বাফার )->xTaskWaitingToReceive != ফাঁকা )                     \
            {                                                                           \
                ( শূন্য ) xTaskNotifyFromISR( ( px_Stream_বাফার )->xTaskWaitingToReceive, \
                                             ( uint32_t ) 0,                            \
                                             eNoAction,                                 \
                                             ( pxHigherPriorityTaskWoken ) );           \
                ( px_Stream_বাফার )->xTaskWaitingToReceive = ফাঁকা;                       \
            }                                                                           \
        }                                                                               \
        portCLEAR_INTERRUPT_MASK_FROM_ISR( uxSavedInterruptStatus );                    \
    }
#endif /* sbSEND_COMPLETE_FROM_ISR */


#যদি ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #define prvSEND_COMPLETE_FROM_ISR( px_Stream_বাফার, pxHigherPriorityTaskWoken )                                    \
    {                                                                                                                 \
        যদি( ( px_Stream_বাফার )->pxSendCompletedCallback != ফাঁকা )                                                     \
        {                                                                                                             \
            ( px_Stream_বাফার )->pxSendCompletedCallback( ( px_Stream_বাফার ), pdTRUE, ( pxHigherPriorityTaskWoken ) ); \
        }                                                                                                             \
        অন্যথা                                                                                                          \
        {                                                                                                             \
            sbSEND_COMPLETE_FROM_ISR( ( px_Stream_বাফার ), ( pxHigherPriorityTaskWoken ) );                            \
        }                                                                                                             \
    }
#অন্যথা /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #define prvSEND_COMPLETE_FROM_ISR( px_Stream_বাফার, pxHigherPriorityTaskWoken ) \
    sbSEND_COMPLETE_FROM_ISR( ( px_Stream_বাফার ), ( pxHigherPriorityTaskWoken ) )
#endif /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

/*lint -restore (9026) */

/* The number of bytes used to hold the length of a message in the buffer. */
#define sb_BYTES_TO_STORE_MESSAGE_দৈর্ঘ্য    ( আকার( config_MESSAGE_বাফার_দৈর্ঘ্য_TYPE ) )

/* Bits stored in the ucFlags field of the stream buffer. */
#define sb_FLAGS_IS_MESSAGE_বাফার          ( ( uint8_t ) 1 ) /* Set if the stream buffer was created as a message buffer, in which case it holds discrete messages rather than a stream. */
#define sbFLAGS_IS_STATICALLY_ALLOCATED    ( ( uint8_t ) 2 ) /* Set if the stream buffer was created using statically allocated memory. */

/*-----------------------------------------------------------*/

/* Structure that hold state information on the buffer. */
প্রকারলেখ কাঠামো Stream_বাফার_Def_t                 /*lint !e9058 Style convention uses tag. */
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

    portMUX_TYPE x_Stream_বাফার_Lock; /* Spinlock required for SMP critical sections */
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
                                                     BaseType_t x_Is_Message_বাফার,
                                                     Stream_বাফার_Callback_Function_t pxSendCompletedCallback,
                                                     Stream_বাফার_Callback_Function_t pxReceiveCompletedCallback )
    {
        uint8_t * pucAllocatedMemory;
        uint8_t ucFlags;

        /* In case the stream buffer is going to be used as a message buffer
         * (that is, it will hold discrete messages with a little meta data that
         * says how big the next message is) check the buffer will be large enough
         * to hold at least one message. */
        যদি( x_Is_Message_বাফার == pdTRUE )
        {
            /* Is a message buffer but not statically allocated. */
            ucFlags = sb_FLAGS_IS_MESSAGE_বাফার;
            configASSERT( x_বাফার_আকার_Bytes > sb_BYTES_TO_STORE_MESSAGE_দৈর্ঘ্য );
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
        যদি( x_বাফার_আকার_Bytes < ( x_বাফার_আকার_Bytes + 1 + আকার( Stream_বাফার_t ) ) )
        {
            x_বাফার_আকার_Bytes++;
            pucAllocatedMemory = ( uint8_t * ) pvPortMalloc( x_বাফার_আকার_Bytes + আকার( Stream_বাফার_t ) ); /*lint !e9079 malloc() only returns void*. */
        }
        অন্যথা
        {
            pucAllocatedMemory = ফাঁকা;
        }

        যদি( pucAllocatedMemory != ফাঁকা )
        {
            prv_Initialise_New_Stream_বাফার( ( Stream_বাফার_t * ) pucAllocatedMemory,       /* Structure at the start of the allocated memory. */ /*lint !e9087 Safe cast as allocated memory is aligned. */ /*lint !e826 Area is not too small and alignment is guaranteed provided malloc() behaves as expected and returns aligned buffer. */
                                          pucAllocatedMemory + আকার( Stream_বাফার_t ), /* Storage area follows. */ /*lint !e9016 Indexing past structure valid for uint8_t pointer, also storage area has no alignment requirement. */
                                          x_বাফার_আকার_Bytes,
                                          xTriggerLevelBytes,
                                          ucFlags,
                                          pxSendCompletedCallback,
                                          pxReceiveCompletedCallback );

            /* Initialize the stream buffer's spinlock separately, as
             * prvInitialiseNewStreamBuffer() is also called from
             * xStreamBufferReset(). */
            portMUX_INITIALIZE( &( ( ( Stream_বাফার_t * ) pucAllocatedMemory )->x_Stream_বাফার_Lock ) );

            trace_STREAM_বাফার_CREATE( ( ( Stream_বাফার_t * ) pucAllocatedMemory ), x_Is_Message_বাফার );
        }
        অন্যথা
        {
            trace_STREAM_বাফার_CREATE_FAILED( x_Is_Message_বাফার );
        }

        ফেরত ( Stream_বাফার_Handle_t ) pucAllocatedMemory; /*lint !e9087 !e826 Safe cast as allocated memory is aligned. */
    }
#endif /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

#যদি ( configSUPPORT_STATIC_ALLOCATION == 1 )

    Stream_বাফার_Handle_t x_Stream_বাফার_Generic_Create_Static( size_t x_বাফার_আকার_Bytes,
                                                           size_t xTriggerLevelBytes,
                                                           BaseType_t x_Is_Message_বাফার,
                                                           uint8_t * ধ্রুবক puc_Stream_বাফার_Storage_Area,
                                                           Static_Stream_বাফার_t * ধ্রুবক px_Static_Stream_বাফার,
                                                           Stream_বাফার_Callback_Function_t pxSendCompletedCallback,
                                                           Stream_বাফার_Callback_Function_t pxReceiveCompletedCallback )
    {
        Stream_বাফার_t * ধ্রুবক px_Stream_বাফার = ( Stream_বাফার_t * ) px_Static_Stream_বাফার; /*lint !e740 !e9087 Safe cast as StaticStreamBuffer_t is opaque Streambuffer_t. */
        Stream_বাফার_Handle_t xReturn;
        uint8_t ucFlags;

        configASSERT( puc_Stream_বাফার_Storage_Area );
        configASSERT( px_Static_Stream_বাফার );
        configASSERT( xTriggerLevelBytes <= x_বাফার_আকার_Bytes );

        /* A trigger level of 0 would cause a waiting task to unblock even when
         * the buffer was empty. */
        যদি( xTriggerLevelBytes == ( size_t ) 0 )
        {
            xTriggerLevelBytes = ( size_t ) 1;
        }

        যদি( x_Is_Message_বাফার != pdFALSE )
        {
            /* Statically allocated message buffer. */
            ucFlags = sb_FLAGS_IS_MESSAGE_বাফার | sbFLAGS_IS_STATICALLY_ALLOCATED;
        }
        অন্যথা
        {
            /* Statically allocated stream buffer. */
            ucFlags = sbFLAGS_IS_STATICALLY_ALLOCATED;
        }

        /* In case the stream buffer is going to be used as a message buffer
         * (that is, it will hold discrete messages with a little meta data that
         * says how big the next message is) check the buffer will be large enough
         * to hold at least one message. */
        configASSERT( x_বাফার_আকার_Bytes > sb_BYTES_TO_STORE_MESSAGE_দৈর্ঘ্য );

        #যদি ( configASSERT_DEFINED == 1 )
        {
            /* Sanity check that the size of the structure used to declare a
             * variable of type StaticStreamBuffer_t equals the size of the real
             * message buffer structure. */
            অস্থির size_t x_আকার = আকার( Static_Stream_বাফার_t );
            configASSERT( x_আকার == আকার( Stream_বাফার_t ) );
            ( শূন্য ) x_আকার; /* Prevent unused variable warning when configASSERT() is not used. */
        } /*lint !e529 xSize is referenced is configASSERT() is defined. */
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

            /* Initialize the stream buffer's spinlock separately, as
             * prvInitialiseNewStreamBuffer() is also called from
             * xStreamBufferReset(). */
            portMUX_INITIALIZE( &( px_Stream_বাফার->x_Stream_বাফার_Lock ) );

            trace_STREAM_বাফার_CREATE( px_Stream_বাফার, x_Is_Message_বাফার );

            xReturn = ( Stream_বাফার_Handle_t ) px_Static_Stream_বাফার; /*lint !e9087 Data hiding requires cast to opaque type. */
        }
        অন্যথা
        {
            xReturn = ফাঁকা;
            trace_STREAM_বাফার_CREATE_STATIC_FAILED( xReturn, x_Is_Message_বাফার );
        }

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
        ধ্রুবক Stream_বাফার_t * ধ্রুবক px_Stream_বাফার = x_Stream_বাফার;

        configASSERT( px_Stream_বাফার );
        configASSERT( ppuc_Stream_বাফার_Storage_Area );
        configASSERT( ppx_Static_Stream_বাফার );

        যদি( ( px_Stream_বাফার->ucFlags & sbFLAGS_IS_STATICALLY_ALLOCATED ) != ( uint8_t ) 0 )
        {
            *ppuc_Stream_বাফার_Storage_Area = px_Stream_বাফার->puc_বাফার;
            *ppx_Static_Stream_বাফার = ( Static_Stream_বাফার_t * ) px_Stream_বাফার;
            xReturn = pdTRUE;
        }
        অন্যথা
        {
            xReturn = pdFALSE;
        }

        ফেরত xReturn;
    }
#endif /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

শূন্য v_Stream_বাফার_Delete( Stream_বাফার_Handle_t x_Stream_বাফার )
{
    Stream_বাফার_t * px_Stream_বাফার = x_Stream_বাফার;

    configASSERT( px_Stream_বাফার );

    trace_STREAM_বাফার_DELETE( x_Stream_বাফার );

    যদি( ( px_Stream_বাফার->ucFlags & sbFLAGS_IS_STATICALLY_ALLOCATED ) == ( uint8_t ) pdFALSE )
    {
        #যদি ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
        {
            /* Both the structure and the buffer were allocated using a single call
            * to pvPortMalloc(), hence only one call to vPortFree() is required. */
            vPortFree( ( শূন্য * ) px_Stream_বাফার ); /*lint !e9087 Standard free() semantics require void *, plus pxStreamBuffer was allocated by pvPortMalloc(). */
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

    configASSERT( px_Stream_বাফার );

    #যদি ( configUSE_TRACE_FACILITY == 1 )
    {
        /* Store the stream buffer number so it can be restored after the
         * reset. */
        ux_Stream_বাফার_সংখ্যা = px_Stream_বাফার->ux_Stream_বাফার_সংখ্যা;
    }
    #endif

    /* Can only reset a message buffer if there are no tasks blocked on it. */
    taskENTER_CRITICAL( &( px_Stream_বাফার->x_Stream_বাফার_Lock ) );
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
    taskEXIT_CRITICAL( &( px_Stream_বাফার->x_Stream_বাফার_Lock ) );

    ফেরত xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t x_Stream_বাফার_Set_Trigger_Level( Stream_বাফার_Handle_t x_Stream_বাফার,
                                         size_t xTriggerLevel )
{
    Stream_বাফার_t * ধ্রুবক px_Stream_বাফার = x_Stream_বাফার;
    BaseType_t xReturn;

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

    ফেরত xReturn;
}
/*-----------------------------------------------------------*/

size_t x_Stream_বাফার_Spaces_Available( Stream_বাফার_Handle_t x_Stream_বাফার )
{
    ধ্রুবক Stream_বাফার_t * ধ্রুবক px_Stream_বাফার = x_Stream_বাফার;
    size_t xSpace;
    size_t x_Original_লেজ;

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

    ফেরত xSpace;
}
/*-----------------------------------------------------------*/

size_t x_Stream_বাফার_Bytes_Available( Stream_বাফার_Handle_t x_Stream_বাফার )
{
    ধ্রুবক Stream_বাফার_t * ধ্রুবক px_Stream_বাফার = x_Stream_বাফার;
    size_t xReturn;

    configASSERT( px_Stream_বাফার );

    xReturn = prv_Bytes_In_বাফার( px_Stream_বাফার );
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
            taskENTER_CRITICAL( &( px_Stream_বাফার->x_Stream_বাফার_Lock ) );
            {
                xSpace = x_Stream_বাফার_Spaces_Available( px_Stream_বাফার );

                যদি( xSpace < xRequiredSpace )
                {
                    /* Clear notification state as going to wait for space. */
                    ( শূন্য ) xTaskNotifyStateClear( ফাঁকা );

                    /* Should only be one writer. */
                    configASSERT( px_Stream_বাফার->xTaskWaitingToSend == ফাঁকা );
                    px_Stream_বাফার->xTaskWaitingToSend = xTaskGetCurrentTaskHandle();
                }
                অন্যথা
                {
                    taskEXIT_CRITICAL( &( px_Stream_বাফার->x_Stream_বাফার_Lock ) );
                    বিরতি;
                }
            }
            taskEXIT_CRITICAL( &( px_Stream_বাফার->x_Stream_বাফার_Lock ) );

            trace_BLOCKING_ON_STREAM_বাফার_SEND( x_Stream_বাফার );
            ( শূন্য ) xTaskNotifyWait( ( uint32_t ) 0, ( uint32_t ) 0, ফাঁকা, xTicksToWait );
            /* In SMP mode, the task may have been woken and scheduled on
             * another core. Hence, we must clear the xTaskWaitingToSend
             * handle in a critical section. */
            #যদি ( config_সংখ্যা_OF_CORES > 1 )
                taskENTER_CRITICAL( &( px_Stream_বাফার->x_Stream_বাফার_Lock ) );
            #endif /* configNUMBER_OF_CORES > 1 */
            {
                px_Stream_বাফার->xTaskWaitingToSend = ফাঁকা;
            }
            #যদি ( config_সংখ্যা_OF_CORES > 1 )
                taskEXIT_CRITICAL( &( px_Stream_বাফার->x_Stream_বাফার_Lock ) );
            #endif /* configNUMBER_OF_CORES > 1 */
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
        px_Stream_বাফার->x_শীর্ষ = prv_Write_Bytes_To_বাফার( px_Stream_বাফার, ( ধ্রুবক uint8_t * ) pv_Tx_তথ্য, x_তথ্য_দৈর্ঘ্য_Bytes, x_পরবর্তী_শীর্ষ ); /*lint !e9079 Storage buffer is implemented as uint8_t for ease of sizing, alignment and access. */
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

    যদি( xTicksToWait != ( TickType_t ) 0 )
    {
        /* Checking if there is data and clearing the notification state must be
         * performed atomically. */
        taskENTER_CRITICAL( &( px_Stream_বাফার->x_Stream_বাফার_Lock ) );
        {
            xBytesAvailable = prv_Bytes_In_বাফার( px_Stream_বাফার );

            /* If this function was invoked by a message buffer read then
             * xBytesToStoreMessageLength holds the number of bytes used to hold
             * the length of the next discrete message.  If this function was
             * invoked by a stream buffer read then xBytesToStoreMessageLength will
             * be 0. */
            যদি( xBytesAvailable <= x_Bytes_To_Store_Message_দৈর্ঘ্য )
            {
                /* Clear notification state as going to wait for data. */
                ( শূন্য ) xTaskNotifyStateClear( ফাঁকা );

                /* Should only be one reader. */
                configASSERT( px_Stream_বাফার->xTaskWaitingToReceive == ফাঁকা );
                px_Stream_বাফার->xTaskWaitingToReceive = xTaskGetCurrentTaskHandle();
            }
            অন্যথা
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        taskEXIT_CRITICAL( &( px_Stream_বাফার->x_Stream_বাফার_Lock ) );

        যদি( xBytesAvailable <= x_Bytes_To_Store_Message_দৈর্ঘ্য )
        {
            /* Wait for data to be available. */
            trace_BLOCKING_ON_STREAM_বাফার_RECEIVE( x_Stream_বাফার );
            ( শূন্য ) xTaskNotifyWait( ( uint32_t ) 0, ( uint32_t ) 0, ফাঁকা, xTicksToWait );

            /* In SMP mode, the task may have been woken and scheduled on
             * another core. Hence, we must clear the xTaskWaitingToReceive
             * handle in a critical section. */
            #যদি ( config_সংখ্যা_OF_CORES > 1 )
                taskENTER_CRITICAL( &( px_Stream_বাফার->x_Stream_বাফার_Lock ) );
            #endif /* configNUMBER_OF_CORES > 1 */
            {
                px_Stream_বাফার->xTaskWaitingToReceive = ফাঁকা;
            }
            #যদি ( config_সংখ্যা_OF_CORES > 1 )
                taskEXIT_CRITICAL( &( px_Stream_বাফার->x_Stream_বাফার_Lock ) );
            #endif /* configNUMBER_OF_CORES > 1 */

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

    ফেরত x_Received_দৈর্ঘ্য;
}
/*-----------------------------------------------------------*/

size_t x_Stream_বাফার_পরবর্তী_Message_দৈর্ঘ্য_Bytes( Stream_বাফার_Handle_t x_Stream_বাফার )
{
    Stream_বাফার_t * ধ্রুবক px_Stream_বাফার = x_Stream_বাফার;
    size_t xReturn, xBytesAvailable;
    config_MESSAGE_বাফার_দৈর্ঘ্য_TYPE x_অস্থায়ী_Return;

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
        px_Stream_বাফার->x_লেজ = prv_Read_Bytes_From_বাফার( px_Stream_বাফার, ( uint8_t * ) pv_Rx_তথ্য, x_গুনতি, x_পরবর্তী_লেজ ); /*lint !e9079 Data storage area is implemented as uint8_t array for ease of sizing, indexing and alignment. */
    }

    ফেরত x_গুনতি;
}
/*-----------------------------------------------------------*/

BaseType_t x_Stream_বাফার_Is_Empty( Stream_বাফার_Handle_t x_Stream_বাফার )
{
    ধ্রুবক Stream_বাফার_t * ধ্রুবক px_Stream_বাফার = x_Stream_বাফার;
    BaseType_t xReturn;
    size_t x_লেজ;

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

    ফেরত xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t x_Stream_বাফার_Is_Full( Stream_বাফার_Handle_t x_Stream_বাফার )
{
    BaseType_t xReturn;
    size_t x_Bytes_To_Store_Message_দৈর্ঘ্য;
    ধ্রুবক Stream_বাফার_t * ধ্রুবক px_Stream_বাফার = x_Stream_বাফার;

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

    ফেরত xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t x_Stream_বাফার_Send_Completed_From_ISR( Stream_বাফার_Handle_t x_Stream_বাফার,
                                              BaseType_t * pxHigherPriorityTaskWoken )
{
    Stream_বাফার_t * ধ্রুবক px_Stream_বাফার = x_Stream_বাফার;
    BaseType_t xReturn;
    UBaseType_t uxSavedInterruptStatus;

    configASSERT( px_Stream_বাফার );

    uxSavedInterruptStatus = ( UBaseType_t ) portSET_INTERRUPT_MASK_FROM_ISR();
    {
        যদি( ( px_Stream_বাফার )->xTaskWaitingToReceive != ফাঁকা )
        {
            ( শূন্য ) xTaskNotifyFromISR( ( px_Stream_বাফার )->xTaskWaitingToReceive,
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
    portCLEAR_INTERRUPT_MASK_FROM_ISR( uxSavedInterruptStatus );

    ফেরত xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t x_Stream_বাফার_Receive_Completed_From_ISR( Stream_বাফার_Handle_t x_Stream_বাফার,
                                                 BaseType_t * pxHigherPriorityTaskWoken )
{
    Stream_বাফার_t * ধ্রুবক px_Stream_বাফার = x_Stream_বাফার;
    BaseType_t xReturn;
    UBaseType_t uxSavedInterruptStatus;

    configASSERT( px_Stream_বাফার );

    uxSavedInterruptStatus = ( UBaseType_t ) portSET_INTERRUPT_MASK_FROM_ISR();
    {
        যদি( ( px_Stream_বাফার )->xTaskWaitingToSend != ফাঁকা )
        {
            ( শূন্য ) xTaskNotifyFromISR( ( px_Stream_বাফার )->xTaskWaitingToSend,
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
    portCLEAR_INTERRUPT_MASK_FROM_ISR( uxSavedInterruptStatus );

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
    ( শূন্য ) memcpy( ( শূন্য * ) ( &( px_Stream_বাফার->puc_বাফার[ x_শীর্ষ ] ) ), ( ধ্রুবক শূন্য * ) puc_তথ্য, x_First_দৈর্ঘ্য ); /*lint !e9087 memcpy() requires void *. */

    /* If the number of bytes written was less than the number that could be
     * written in the first write... */
    যদি( x_গুনতি > x_First_দৈর্ঘ্য )
    {
        /* ...then write the remaining bytes to the start of the buffer. */
        configASSERT( ( x_গুনতি - x_First_দৈর্ঘ্য ) <= px_Stream_বাফার->x_দৈর্ঘ্য );
        ( শূন্য ) memcpy( ( শূন্য * ) px_Stream_বাফার->puc_বাফার, ( ধ্রুবক শূন্য * ) &( puc_তথ্য[ x_First_দৈর্ঘ্য ] ), x_গুনতি - x_First_দৈর্ঘ্য ); /*lint !e9087 memcpy() requires void *. */
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
    ( শূন্য ) memcpy( ( শূন্য * ) puc_তথ্য, ( ধ্রুবক শূন্য * ) &( px_Stream_বাফার->puc_বাফার[ x_লেজ ] ), x_First_দৈর্ঘ্য ); /*lint !e9087 memcpy() requires void *. */

    /* If the total number of wanted bytes is greater than the number
     * that could be read in the first read... */
    যদি( x_গুনতি > x_First_দৈর্ঘ্য )
    {
        /* ...then read the remaining bytes from the start of the buffer. */
        ( শূন্য ) memcpy( ( শূন্য * ) &( puc_তথ্য[ x_First_দৈর্ঘ্য ] ), ( শূন্য * ) ( px_Stream_বাফার->puc_বাফার ), x_গুনতি - x_First_দৈর্ঘ্য ); /*lint !e9087 memcpy() requires void *. */
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
        ধ্রুবক BaseType_t x_Write_মান = 0x55;
        configASSERT( memset( puc_বাফার, ( পূর্ণসংখ্যা ) x_Write_মান, x_বাফার_আকার_Bytes ) == puc_বাফার );
        ( শূন্য ) x_Write_মান;
    } /*lint !e529 !e438 xWriteValue is only used if configASSERT() is defined. */
    #endif

    /* This function could be called from xStreamBufferReset(), so we reset the
     * stream buffer fields manually in order to avoid clearing
     * xStreamBufferLock. The xStreamBufferLock is initialized separately on
     * stream buffer creation. */
    px_Stream_বাফার->x_লেজ = ( size_t ) 0;
    px_Stream_বাফার->x_শীর্ষ = ( size_t ) 0;
    px_Stream_বাফার->xTaskWaitingToReceive = ( TaskHandle_t ) 0;
    px_Stream_বাফার->xTaskWaitingToSend = ( TaskHandle_t ) 0;
    #যদি ( configUSE_TRACE_FACILITY == 1 )
        px_Stream_বাফার->ux_Stream_বাফার_সংখ্যা = ( UBaseType_t ) 0;
    #endif
    px_Stream_বাফার->puc_বাফার = puc_বাফার;
    px_Stream_বাফার->x_দৈর্ঘ্য = x_বাফার_আকার_Bytes;
    px_Stream_বাফার->xTriggerLevelBytes = xTriggerLevelBytes;
    px_Stream_বাফার->ucFlags = ucFlags;
    #যদি ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    {
        px_Stream_বাফার->pxSendCompletedCallback = pxSendCompletedCallback;
        px_Stream_বাফার->pxReceiveCompletedCallback = pxReceiveCompletedCallback;
    }
    #অন্যথা
    {
        ( শূন্য ) pxSendCompletedCallback;
        ( শূন্য ) pxReceiveCompletedCallback;
    }
    #endif
}

#যদি ( configUSE_TRACE_FACILITY == 1 )

    UBaseType_t ux_Stream_বাফার_Get_Stream_বাফার_সংখ্যা( Stream_বাফার_Handle_t x_Stream_বাফার )
    {
        ফেরত x_Stream_বাফার->ux_Stream_বাফার_সংখ্যা;
    }

#endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#যদি ( configUSE_TRACE_FACILITY == 1 )

    শূন্য v_Stream_বাফার_Set_Stream_বাফার_সংখ্যা( Stream_বাফার_Handle_t x_Stream_বাফার,
                                             UBaseType_t ux_Stream_বাফার_সংখ্যা )
    {
        x_Stream_বাফার->ux_Stream_বাফার_সংখ্যা = ux_Stream_বাফার_সংখ্যা;
    }

#endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#যদি ( configUSE_TRACE_FACILITY == 1 )

    uint8_t uc_Stream_বাফার_Get_Stream_বাফার_Type( Stream_বাফার_Handle_t x_Stream_বাফার )
    {
        ফেরত( x_Stream_বাফার->ucFlags & sb_FLAGS_IS_MESSAGE_বাফার );
    }

#endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/
