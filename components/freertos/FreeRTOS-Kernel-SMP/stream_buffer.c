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
#समावेश <string.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#घोषणा MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* FreeRTOS includes. */
#समावेश "FreeRTOS.h"
#समावेश "task.h"
#समावेश "stream_buffer.h"

#अगर ( configUSE_TASK_NOTIFICATIONS != 1 )
    #त्रुटि configUSE_TASK_NOTIFICATIONS must be set to 1 to build stream_बफर.c
#पूर्ण_अगर

#अगर ( INCLUDE_xTaskGetCurrentTaskHandle != 1 )
    #त्रुटि INCLUDE_xTaskGetCurrentTaskHandle must be set to 1 to build stream_बफर.c
#पूर्ण_अगर

/* The MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#अघोषित MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* This entire source file will be skipped if the application is not configured
 * to include stream buffer functionality. This #if is closed at the very bottom
 * of this file. If you want to include stream buffers then ensure
 * configUSE_STREAM_BUFFERS is set to 1 in FreeRTOSConfig.h. */
#अगर ( configUSE_STREAM_BUFFERS == 1 )

/* If the user has not provided application specific Rx notification macros,
 * or #defined the notification macros away, then provide default implementations
 * that uses task notifications. */
    #अगर_अघोषित sbRECEIVE_COMPLETED
        #घोषणा sbRECEIVE_COMPLETED( px_Stream_बफर )                                 \
    करो                                                                                \
    {                                                                                 \
        vTaskSuspendAll();                                                            \
        {                                                                             \
            अगर( ( px_Stream_बफर )->xTaskWaitingToSend != शून्य )                      \
            {                                                                         \
                ( व्योम ) xTaskNotifyIndexed( ( px_Stream_बफर )->xTaskWaitingToSend,  \
                                             ( px_Stream_बफर )->ux_Notification_सूचकांक, \
                                             ( uint32_t ) 0,                          \
                                             eNoAction );                             \
                ( px_Stream_बफर )->xTaskWaitingToSend = शून्य;                        \
            }                                                                         \
        }                                                                             \
        ( व्योम ) xTaskResumeAll();                                                    \
    } जबतक( 0 )
    #पूर्ण_अगर /* sbRECEIVE_COMPLETED */

/* If user has provided a per-instance receive complete callback, then
 * invoke the callback else use the receive complete macro which is provided by default for all instances.
 */
    #अगर ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        #घोषणा prvRECEIVE_COMPLETED( px_Stream_बफर )                                           \
    करो {                                                                                         \
        अगर( ( px_Stream_बफर )->pxReceiveCompletedCallback != शून्य )                             \
        {                                                                                        \
            ( px_Stream_बफर )->pxReceiveCompletedCallback( ( px_Stream_बफर ), pdFALSE, शून्य ); \
        }                                                                                        \
        अन्यथा                                                                                     \
        {                                                                                        \
            sbRECEIVE_COMPLETED( ( px_Stream_बफर ) );                                           \
        }                                                                                        \
    } जबतक( 0 )
    #अन्यथा /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
        #घोषणा prvRECEIVE_COMPLETED( px_Stream_बफर )    sbRECEIVE_COMPLETED( ( px_Stream_बफर ) )
    #पूर्ण_अगर /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

    #अगर_अघोषित sbRECEIVE_COMPLETED_FROM_ISR
        #घोषणा sbRECEIVE_COMPLETED_FROM_ISR( px_Stream_बफर,                                \
                                              pxHigherPriorityTaskWoken )                    \
    करो {                                                                                     \
        UBaseType_t uxSavedInterruptStatus;                                                  \
                                                                                             \
        uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();                              \
        {                                                                                    \
            अगर( ( px_Stream_बफर )->xTaskWaitingToSend != शून्य )                             \
            {                                                                                \
                ( व्योम ) xTaskNotifyIndexedFromISR( ( px_Stream_बफर )->xTaskWaitingToSend,  \
                                                    ( px_Stream_बफर )->ux_Notification_सूचकांक, \
                                                    ( uint32_t ) 0,                          \
                                                    eNoAction,                               \
                                                    ( pxHigherPriorityTaskWoken ) );         \
                ( px_Stream_बफर )->xTaskWaitingToSend = शून्य;                               \
            }                                                                                \
        }                                                                                    \
        taskEXIT_CRITICAL_FROM_ISR( uxSavedInterruptStatus );                                \
    } जबतक( 0 )
    #पूर्ण_अगर /* sbRECEIVE_COMPLETED_FROM_ISR */

    #अगर ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        #घोषणा prvRECEIVE_COMPLETED_FROM_ISR( px_Stream_बफर,                                                           \
                                               pxHigherPriorityTaskWoken )                                               \
    करो {                                                                                                                 \
        अगर( ( px_Stream_बफर )->pxReceiveCompletedCallback != शून्य )                                                     \
        {                                                                                                                \
            ( px_Stream_बफर )->pxReceiveCompletedCallback( ( px_Stream_बफर ), pdTRUE, ( pxHigherPriorityTaskWoken ) ); \
        }                                                                                                                \
        अन्यथा                                                                                                             \
        {                                                                                                                \
            sbRECEIVE_COMPLETED_FROM_ISR( ( px_Stream_बफर ), ( pxHigherPriorityTaskWoken ) );                           \
        }                                                                                                                \
    } जबतक( 0 )
    #अन्यथा /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
        #घोषणा prvRECEIVE_COMPLETED_FROM_ISR( px_Stream_बफर, pxHigherPriorityTaskWoken ) \
    sbRECEIVE_COMPLETED_FROM_ISR( ( px_Stream_बफर ), ( pxHigherPriorityTaskWoken ) )
    #पूर्ण_अगर /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

/* If the user has not provided an application specific Tx notification macro,
 * or #defined the notification macro away, then provide a default
 * implementation that uses task notifications.
 */
    #अगर_अघोषित sbSEND_COMPLETED
        #घोषणा sbSEND_COMPLETED( px_Stream_बफर )                                  \
    vTaskSuspendAll();                                                              \
    {                                                                               \
        अगर( ( px_Stream_बफर )->xTaskWaitingToReceive != शून्य )                     \
        {                                                                           \
            ( व्योम ) xTaskNotifyIndexed( ( px_Stream_बफर )->xTaskWaitingToReceive, \
                                         ( px_Stream_बफर )->ux_Notification_सूचकांक,   \
                                         ( uint32_t ) 0,                            \
                                         eNoAction );                               \
            ( px_Stream_बफर )->xTaskWaitingToReceive = शून्य;                       \
        }                                                                           \
    }                                                                               \
    ( व्योम ) xTaskResumeAll()
    #पूर्ण_अगर /* sbSEND_COMPLETED */

/* If user has provided a per-instance send completed callback, then
 * invoke the callback else use the send complete macro which is provided by default for all instances.
 */
    #अगर ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        #घोषणा prvSEND_COMPLETED( px_Stream_बफर )                                           \
    करो {                                                                                      \
        अगर( ( px_Stream_बफर )->pxSendCompletedCallback != शून्य )                             \
        {                                                                                     \
            ( px_Stream_बफर )->pxSendCompletedCallback( ( px_Stream_बफर ), pdFALSE, शून्य ); \
        }                                                                                     \
        अन्यथा                                                                                  \
        {                                                                                     \
            sbSEND_COMPLETED( ( px_Stream_बफर ) );                                           \
        }                                                                                     \
    } जबतक( 0 )
    #अन्यथा /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
        #घोषणा prvSEND_COMPLETED( px_Stream_बफर )    sbSEND_COMPLETED( ( px_Stream_बफर ) )
    #पूर्ण_अगर /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */


    #अगर_अघोषित sbSEND_COMPLETE_FROM_ISR
        #घोषणा sbSEND_COMPLETE_FROM_ISR( px_Stream_बफर, pxHigherPriorityTaskWoken )          \
    करो {                                                                                       \
        UBaseType_t uxSavedInterruptStatus;                                                    \
                                                                                               \
        uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();                                \
        {                                                                                      \
            अगर( ( px_Stream_बफर )->xTaskWaitingToReceive != शून्य )                            \
            {                                                                                  \
                ( व्योम ) xTaskNotifyIndexedFromISR( ( px_Stream_बफर )->xTaskWaitingToReceive, \
                                                    ( px_Stream_बफर )->ux_Notification_सूचकांक,   \
                                                    ( uint32_t ) 0,                            \
                                                    eNoAction,                                 \
                                                    ( pxHigherPriorityTaskWoken ) );           \
                ( px_Stream_बफर )->xTaskWaitingToReceive = शून्य;                              \
            }                                                                                  \
        }                                                                                      \
        taskEXIT_CRITICAL_FROM_ISR( uxSavedInterruptStatus );                                  \
    } जबतक( 0 )
    #पूर्ण_अगर /* sbSEND_COMPLETE_FROM_ISR */


    #अगर ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        #घोषणा prvSEND_COMPLETE_FROM_ISR( px_Stream_बफर, pxHigherPriorityTaskWoken )                                \
    करो {                                                                                                              \
        अगर( ( px_Stream_बफर )->pxSendCompletedCallback != शून्य )                                                     \
        {                                                                                                             \
            ( px_Stream_बफर )->pxSendCompletedCallback( ( px_Stream_बफर ), pdTRUE, ( pxHigherPriorityTaskWoken ) ); \
        }                                                                                                             \
        अन्यथा                                                                                                          \
        {                                                                                                             \
            sbSEND_COMPLETE_FROM_ISR( ( px_Stream_बफर ), ( pxHigherPriorityTaskWoken ) );                            \
        }                                                                                                             \
    } जबतक( 0 )
    #अन्यथा /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
        #घोषणा prvSEND_COMPLETE_FROM_ISR( px_Stream_बफर, pxHigherPriorityTaskWoken ) \
    sbSEND_COMPLETE_FROM_ISR( ( px_Stream_बफर ), ( pxHigherPriorityTaskWoken ) )
    #पूर्ण_अगर /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

/* The number of bytes used to hold the length of a message in the buffer. */
    #घोषणा sb_BYTES_TO_STORE_MESSAGE_लंबाई    ( माप( config_MESSAGE_बफर_लंबाई_TYPE ) )

/* Bits stored in the ucFlags field of the stream buffer. */
    #घोषणा sb_FLAGS_IS_MESSAGE_बफर          ( ( uint8_t ) 1 ) /* Set if the stream buffer was created as a message buffer, in which case it holds discrete messages rather than a stream. */
    #घोषणा sbFLAGS_IS_STATICALLY_ALLOCATED    ( ( uint8_t ) 2 ) /* Set if the stream buffer was created using statically allocated memory. */
    #घोषणा sb_FLAGS_IS_BATCHING_बफर         ( ( uint8_t ) 4 ) /* Set if the stream buffer was created as a batching buffer, meaning the receiver task will only unblock when the trigger level exceededs. */

/*-----------------------------------------------------------*/

/* Structure that hold state information on the buffer. */
प्रकार काष्ठा Stream_बफर_Def_t
{
    अस्थिर माप_प्रकार x_पुच्छ;                       /* Index to the next item to read within the buffer. */
    अस्थिर माप_प्रकार x_शीर्ष;                       /* Index to the next item to write within the buffer. */
    माप_प्रकार x_लंबाई;                              /* The length of the buffer pointed to by pucBuffer. */
    माप_प्रकार xTriggerLevelBytes;                   /* The number of bytes that must be in the stream buffer before a task that is waiting for data is unblocked. */
    अस्थिर TaskHandle_t xTaskWaitingToReceive; /* Holds the handle of a task waiting for data, or NULL if no tasks are waiting. */
    अस्थिर TaskHandle_t xTaskWaitingToSend;    /* Holds the handle of a task waiting to send data to a message buffer that is full. */
    uint8_t * puc_बफर;                         /* Points to the buffer itself - that is - the RAM that stores the data passed through the buffer. */
    uint8_t ucFlags;

    #अगर ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_Stream_बफर_संख्या; /* Used for tracing purposes. */
    #पूर्ण_अगर

    #अगर ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        Stream_बफर_Callback_Function_t pxSendCompletedCallback;    /* Optional callback called on send complete. sbSEND_COMPLETED is called if this is NULL. */
        Stream_बफर_Callback_Function_t pxReceiveCompletedCallback; /* Optional callback called on receive complete.  sbRECEIVE_COMPLETED is called if this is NULL. */
    #पूर्ण_अगर
    UBaseType_t ux_Notification_सूचकांक;                               /* The index we are using for notification, by default tskDEFAULT_INDEX_TO_NOTIFY. */
} Stream_बफर_t;

/*
 * The number of bytes available to be read from the buffer.
 */
अटल माप_प्रकार prv_Bytes_In_बफर( स्थिर Stream_बफर_t * स्थिर px_Stream_बफर ) PRIVILEGED_FUNCTION;

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
अटल माप_प्रकार prv_Write_Bytes_To_बफर( Stream_बफर_t * स्थिर px_Stream_बफर,
                                     स्थिर uint8_t * puc_तथ्य,
                                     माप_प्रकार x_गिनती,
                                     माप_प्रकार x_शीर्ष ) PRIVILEGED_FUNCTION;

/*
 * If the stream buffer is being used as a message buffer, then reads an entire
 * message out of the buffer.  If the stream buffer is being used as a stream
 * buffer then read as many bytes as possible from the buffer.
 * prvReadBytesFromBuffer() is called to actually extract the bytes from the
 * buffer's data storage area.
 */
अटल माप_प्रकार prv_Read_Message_From_बफर( Stream_बफर_t * px_Stream_बफर,
                                        व्योम * pv_Rx_तथ्य,
                                        माप_प्रकार x_बफर_लंबाई_Bytes,
                                        माप_प्रकार xBytesAvailable ) PRIVILEGED_FUNCTION;

/*
 * If the stream buffer is being used as a message buffer, then writes an entire
 * message to the buffer.  If the stream buffer is being used as a stream
 * buffer then write as many bytes as possible to the buffer.
 * prvWriteBytestoBuffer() is called to actually send the bytes to the buffer's
 * data storage area.
 */
अटल माप_प्रकार prv_Write_Message_To_बफर( Stream_बफर_t * स्थिर px_Stream_बफर,
                                       स्थिर व्योम * pv_Tx_तथ्य,
                                       माप_प्रकार x_तथ्य_लंबाई_Bytes,
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
अटल माप_प्रकार prv_Read_Bytes_From_बफर( Stream_बफर_t * px_Stream_बफर,
                                      uint8_t * puc_तथ्य,
                                      माप_प्रकार x_गिनती,
                                      माप_प्रकार x_पुच्छ ) PRIVILEGED_FUNCTION;

/*
 * Called by both pxStreamBufferCreate() and pxStreamBufferCreateStatic() to
 * initialise the members of the newly created stream buffer structure.
 */
अटल व्योम prv_Initialise_New_Stream_बफर( Stream_बफर_t * स्थिर px_Stream_बफर,
                                          uint8_t * स्थिर puc_बफर,
                                          माप_प्रकार x_बफर_आकार_Bytes,
                                          माप_प्रकार xTriggerLevelBytes,
                                          uint8_t ucFlags,
                                          Stream_बफर_Callback_Function_t pxSendCompletedCallback,
                                          Stream_बफर_Callback_Function_t pxReceiveCompletedCallback ) PRIVILEGED_FUNCTION;

/*-----------------------------------------------------------*/
    #अगर ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
    Stream_बफर_Handle_t x_Stream_बफर_Generic_Create( माप_प्रकार x_बफर_आकार_Bytes,
                                                     माप_प्रकार xTriggerLevelBytes,
                                                     BaseType_t x_Stream_बफर_Type,
                                                     Stream_बफर_Callback_Function_t pxSendCompletedCallback,
                                                     Stream_बफर_Callback_Function_t pxReceiveCompletedCallback )
    {
        व्योम * pvAllocatedMemory;
        uint8_t ucFlags;

        trace_ENTER_x_Stream_बफर_Generic_Create( x_बफर_आकार_Bytes, xTriggerLevelBytes, x_Stream_बफर_Type, pxSendCompletedCallback, pxReceiveCompletedCallback );

        /* In case the stream buffer is going to be used as a message buffer
         * (that is, it will hold discrete messages with a little meta data that
         * says how big the next message is) check the buffer will be large enough
         * to hold at least one message. */
        अगर( x_Stream_बफर_Type == sb_TYPE_MESSAGE_बफर )
        {
            /* Is a message buffer but not statically allocated. */
            ucFlags = sb_FLAGS_IS_MESSAGE_बफर;
            configASSERT( x_बफर_आकार_Bytes > sb_BYTES_TO_STORE_MESSAGE_लंबाई );
        }
        अन्यथा अगर( x_Stream_बफर_Type == sb_TYPE_STREAM_BATCHING_बफर )
        {
            /* Is a batching buffer but not statically allocated. */
            ucFlags = sb_FLAGS_IS_BATCHING_बफर;
            configASSERT( x_बफर_आकार_Bytes > 0 );
        }
        अन्यथा
        {
            /* Not a message buffer and not statically allocated. */
            ucFlags = 0;
            configASSERT( x_बफर_आकार_Bytes > 0 );
        }

        configASSERT( xTriggerLevelBytes <= x_बफर_आकार_Bytes );

        /* A trigger level of 0 would cause a waiting task to unblock even when
         * the buffer was empty. */
        अगर( xTriggerLevelBytes == ( माप_प्रकार ) 0 )
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
        अगर( x_बफर_आकार_Bytes < ( x_बफर_आकार_Bytes + 1U + माप( Stream_बफर_t ) ) )
        {
            x_बफर_आकार_Bytes++;
            pvAllocatedMemory = pvPortMalloc( x_बफर_आकार_Bytes + माप( Stream_बफर_t ) );
        }
        अन्यथा
        {
            pvAllocatedMemory = शून्य;
        }

        अगर( pvAllocatedMemory != शून्य )
        {
            /* MISRA Ref 11.5.1 [Malloc memory assignment] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
            /* coverity[misra_c_2012_rule_11_5_violation] */
            prv_Initialise_New_Stream_बफर( ( Stream_बफर_t * ) pvAllocatedMemory,                         /* Structure at the start of the allocated memory. */
                                                                                                          /* MISRA Ref 11.5.1 [Malloc memory assignment] */
                                                                                                          /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
                                                                                                          /* coverity[misra_c_2012_rule_11_5_violation] */
                                          ( ( uint8_t * ) pvAllocatedMemory ) + माप( Stream_बफर_t ), /* Storage area follows. */
                                          x_बफर_आकार_Bytes,
                                          xTriggerLevelBytes,
                                          ucFlags,
                                          pxSendCompletedCallback,
                                          pxReceiveCompletedCallback );

            trace_STREAM_बफर_CREATE( ( ( Stream_बफर_t * ) pvAllocatedMemory ), x_Stream_बफर_Type );
        }
        अन्यथा
        {
            trace_STREAM_बफर_CREATE_FAILED( x_Stream_बफर_Type );
        }

        trace_RETURN_x_Stream_बफर_Generic_Create( pvAllocatedMemory );

        /* MISRA Ref 11.5.1 [Malloc memory assignment] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
        /* coverity[misra_c_2012_rule_11_5_violation] */
        वापस ( Stream_बफर_Handle_t ) pvAllocatedMemory;
    }
    #पूर्ण_अगर /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

    #अगर ( configSUPPORT_STATIC_ALLOCATION == 1 )

    Stream_बफर_Handle_t x_Stream_बफर_Generic_Create_Static( माप_प्रकार x_बफर_आकार_Bytes,
                                                           माप_प्रकार xTriggerLevelBytes,
                                                           BaseType_t x_Stream_बफर_Type,
                                                           uint8_t * स्थिर puc_Stream_बफर_Storage_Area,
                                                           Static_Stream_बफर_t * स्थिर px_Static_Stream_बफर,
                                                           Stream_बफर_Callback_Function_t pxSendCompletedCallback,
                                                           Stream_बफर_Callback_Function_t pxReceiveCompletedCallback )
    {
        /* MISRA Ref 11.3.1 [Misaligned access] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
        /* coverity[misra_c_2012_rule_11_3_violation] */
        Stream_बफर_t * स्थिर px_Stream_बफर = ( Stream_बफर_t * ) px_Static_Stream_बफर;
        Stream_बफर_Handle_t xReturn;
        uint8_t ucFlags;

        trace_ENTER_x_Stream_बफर_Generic_Create_Static( x_बफर_आकार_Bytes, xTriggerLevelBytes, x_Stream_बफर_Type, puc_Stream_बफर_Storage_Area, px_Static_Stream_बफर, pxSendCompletedCallback, pxReceiveCompletedCallback );

        configASSERT( puc_Stream_बफर_Storage_Area );
        configASSERT( px_Static_Stream_बफर );
        configASSERT( xTriggerLevelBytes <= x_बफर_आकार_Bytes );

        /* A trigger level of 0 would cause a waiting task to unblock even when
         * the buffer was empty. */
        अगर( xTriggerLevelBytes == ( माप_प्रकार ) 0 )
        {
            xTriggerLevelBytes = ( माप_प्रकार ) 1;
        }

        /* In case the stream buffer is going to be used as a message buffer
         * (that is, it will hold discrete messages with a little meta data that
         * says how big the next message is) check the buffer will be large enough
         * to hold at least one message. */

        अगर( x_Stream_बफर_Type == sb_TYPE_MESSAGE_बफर )
        {
            /* Statically allocated message buffer. */
            ucFlags = sb_FLAGS_IS_MESSAGE_बफर | sbFLAGS_IS_STATICALLY_ALLOCATED;
            configASSERT( x_बफर_आकार_Bytes > sb_BYTES_TO_STORE_MESSAGE_लंबाई );
        }
        अन्यथा अगर( x_Stream_बफर_Type == sb_TYPE_STREAM_BATCHING_बफर )
        {
            /* Statically allocated batching buffer. */
            ucFlags = sb_FLAGS_IS_BATCHING_बफर | sbFLAGS_IS_STATICALLY_ALLOCATED;
            configASSERT( x_बफर_आकार_Bytes > 0 );
        }
        अन्यथा
        {
            /* Statically allocated stream buffer. */
            ucFlags = sbFLAGS_IS_STATICALLY_ALLOCATED;
        }

        #अगर ( configASSERT_DEFINED == 1 )
        {
            /* Sanity check that the size of the structure used to declare a
             * variable of type StaticStreamBuffer_t equals the size of the real
             * message buffer structure. */
            अस्थिर माप_प्रकार x_आकार = माप( Static_Stream_बफर_t );
            configASSERT( x_आकार == माप( Stream_बफर_t ) );
        }
        #पूर्ण_अगर /* configASSERT_DEFINED */

        अगर( ( puc_Stream_बफर_Storage_Area != शून्य ) && ( px_Static_Stream_बफर != शून्य ) )
        {
            prv_Initialise_New_Stream_बफर( px_Stream_बफर,
                                          puc_Stream_बफर_Storage_Area,
                                          x_बफर_आकार_Bytes,
                                          xTriggerLevelBytes,
                                          ucFlags,
                                          pxSendCompletedCallback,
                                          pxReceiveCompletedCallback );

            /* Remember this was statically allocated in case it is ever deleted
             * again. */
            px_Stream_बफर->ucFlags |= sbFLAGS_IS_STATICALLY_ALLOCATED;

            trace_STREAM_बफर_CREATE( px_Stream_बफर, x_Stream_बफर_Type );

            /* MISRA Ref 11.3.1 [Misaligned access] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
            /* coverity[misra_c_2012_rule_11_3_violation] */
            xReturn = ( Stream_बफर_Handle_t ) px_Static_Stream_बफर;
        }
        अन्यथा
        {
            xReturn = शून्य;
            trace_STREAM_बफर_CREATE_STATIC_FAILED( xReturn, x_Stream_बफर_Type );
        }

        trace_RETURN_x_Stream_बफर_Generic_Create_Static( xReturn );

        वापस xReturn;
    }
    #पूर्ण_अगर /* ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*-----------------------------------------------------------*/

    #अगर ( configSUPPORT_STATIC_ALLOCATION == 1 )
    BaseType_t x_Stream_बफर_Get_Static_Buffers( Stream_बफर_Handle_t x_Stream_बफर,
                                              uint8_t ** ppuc_Stream_बफर_Storage_Area,
                                              Static_Stream_बफर_t ** ppx_Static_Stream_बफर )
    {
        BaseType_t xReturn;
        Stream_बफर_t * स्थिर px_Stream_बफर = x_Stream_बफर;

        trace_ENTER_x_Stream_बफर_Get_Static_Buffers( x_Stream_बफर, ppuc_Stream_बफर_Storage_Area, ppx_Static_Stream_बफर );

        configASSERT( px_Stream_बफर );
        configASSERT( ppuc_Stream_बफर_Storage_Area );
        configASSERT( ppx_Static_Stream_बफर );

        अगर( ( px_Stream_बफर->ucFlags & sbFLAGS_IS_STATICALLY_ALLOCATED ) != ( uint8_t ) 0 )
        {
            *ppuc_Stream_बफर_Storage_Area = px_Stream_बफर->puc_बफर;
            /* MISRA Ref 11.3.1 [Misaligned access] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
            /* coverity[misra_c_2012_rule_11_3_violation] */
            *ppx_Static_Stream_बफर = ( Static_Stream_बफर_t * ) px_Stream_बफर;
            xReturn = pdTRUE;
        }
        अन्यथा
        {
            xReturn = pdFALSE;
        }

        trace_RETURN_x_Stream_बफर_Get_Static_Buffers( xReturn );

        वापस xReturn;
    }
    #पूर्ण_अगर /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

व्योम v_Stream_बफर_Delete( Stream_बफर_Handle_t x_Stream_बफर )
{
    Stream_बफर_t * px_Stream_बफर = x_Stream_बफर;

    trace_ENTER_v_Stream_बफर_Delete( x_Stream_बफर );

    configASSERT( px_Stream_बफर );

    trace_STREAM_बफर_DELETE( x_Stream_बफर );

    अगर( ( px_Stream_बफर->ucFlags & sbFLAGS_IS_STATICALLY_ALLOCATED ) == ( uint8_t ) pdFALSE )
    {
        #अगर ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
        {
            /* Both the structure and the buffer were allocated using a single call
            * to pvPortMalloc(), hence only one call to vPortFree() is required. */
            vPortFree( ( व्योम * ) px_Stream_बफर );
        }
        #अन्यथा
        {
            /* Should not be possible to get here, ucFlags must be corrupt.
             * Force an assert. */
            configASSERT( x_Stream_बफर == ( Stream_बफर_Handle_t ) ~0 );
        }
        #पूर्ण_अगर
    }
    अन्यथा
    {
        /* The structure and buffer were not allocated dynamically and cannot be
         * freed - just scrub the structure so future use will assert. */
        ( व्योम ) स_रखो( px_Stream_बफर, 0x00, माप( Stream_बफर_t ) );
    }

    trace_RETURN_v_Stream_बफर_Delete();
}
/*-----------------------------------------------------------*/

BaseType_t x_Stream_बफर_Reset( Stream_बफर_Handle_t x_Stream_बफर )
{
    Stream_बफर_t * स्थिर px_Stream_बफर = x_Stream_बफर;
    BaseType_t xReturn = pdFAIL;
    Stream_बफर_Callback_Function_t pxSendCallback = शून्य, pxReceiveCallback = शून्य;

    #अगर ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_Stream_बफर_संख्या;
    #पूर्ण_अगर

    trace_ENTER_x_Stream_बफर_Reset( x_Stream_बफर );

    configASSERT( px_Stream_बफर );

    #अगर ( configUSE_TRACE_FACILITY == 1 )
    {
        /* Store the stream buffer number so it can be restored after the
         * reset. */
        ux_Stream_बफर_संख्या = px_Stream_बफर->ux_Stream_बफर_संख्या;
    }
    #पूर्ण_अगर

    /* Can only reset a message buffer if there are no tasks blocked on it. */
    taskENTER_CRITICAL();
    {
        अगर( ( px_Stream_बफर->xTaskWaitingToReceive == शून्य ) && ( px_Stream_बफर->xTaskWaitingToSend == शून्य ) )
        {
            #अगर ( configUSE_SB_COMPLETED_CALLBACK == 1 )
            {
                pxSendCallback = px_Stream_बफर->pxSendCompletedCallback;
                pxReceiveCallback = px_Stream_बफर->pxReceiveCompletedCallback;
            }
            #पूर्ण_अगर

            prv_Initialise_New_Stream_बफर( px_Stream_बफर,
                                          px_Stream_बफर->puc_बफर,
                                          px_Stream_बफर->x_लंबाई,
                                          px_Stream_बफर->xTriggerLevelBytes,
                                          px_Stream_बफर->ucFlags,
                                          pxSendCallback,
                                          pxReceiveCallback );

            #अगर ( configUSE_TRACE_FACILITY == 1 )
            {
                px_Stream_बफर->ux_Stream_बफर_संख्या = ux_Stream_बफर_संख्या;
            }
            #पूर्ण_अगर

            trace_STREAM_बफर_RESET( x_Stream_बफर );

            xReturn = pdPASS;
        }
    }
    taskEXIT_CRITICAL();

    trace_RETURN_x_Stream_बफर_Reset( xReturn );

    वापस xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t x_Stream_बफर_Reset_From_ISR( Stream_बफर_Handle_t x_Stream_बफर )
{
    Stream_बफर_t * स्थिर px_Stream_बफर = x_Stream_बफर;
    BaseType_t xReturn = pdFAIL;
    Stream_बफर_Callback_Function_t pxSendCallback = शून्य, pxReceiveCallback = शून्य;
    UBaseType_t uxSavedInterruptStatus;

    #अगर ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_Stream_बफर_संख्या;
    #पूर्ण_अगर

    trace_ENTER_x_Stream_बफर_Reset_From_ISR( x_Stream_बफर );

    configASSERT( px_Stream_बफर );

    #अगर ( configUSE_TRACE_FACILITY == 1 )
    {
        /* Store the stream buffer number so it can be restored after the
         * reset. */
        ux_Stream_बफर_संख्या = px_Stream_बफर->ux_Stream_बफर_संख्या;
    }
    #पूर्ण_अगर

    /* Can only reset a message buffer if there are no tasks blocked on it. */
    /* MISRA Ref 4.7.1 [Return value shall be checked] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
    /* coverity[misra_c_2012_directive_4_7_violation] */
    uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
    {
        अगर( ( px_Stream_बफर->xTaskWaitingToReceive == शून्य ) && ( px_Stream_बफर->xTaskWaitingToSend == शून्य ) )
        {
            #अगर ( configUSE_SB_COMPLETED_CALLBACK == 1 )
            {
                pxSendCallback = px_Stream_बफर->pxSendCompletedCallback;
                pxReceiveCallback = px_Stream_बफर->pxReceiveCompletedCallback;
            }
            #पूर्ण_अगर

            prv_Initialise_New_Stream_बफर( px_Stream_बफर,
                                          px_Stream_बफर->puc_बफर,
                                          px_Stream_बफर->x_लंबाई,
                                          px_Stream_बफर->xTriggerLevelBytes,
                                          px_Stream_बफर->ucFlags,
                                          pxSendCallback,
                                          pxReceiveCallback );

            #अगर ( configUSE_TRACE_FACILITY == 1 )
            {
                px_Stream_बफर->ux_Stream_बफर_संख्या = ux_Stream_बफर_संख्या;
            }
            #पूर्ण_अगर

            trace_STREAM_बफर_RESET_FROM_ISR( x_Stream_बफर );

            xReturn = pdPASS;
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( uxSavedInterruptStatus );

    trace_RETURN_x_Stream_बफर_Reset_From_ISR( xReturn );

    वापस xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t x_Stream_बफर_Set_Trigger_Level( Stream_बफर_Handle_t x_Stream_बफर,
                                         माप_प्रकार xTriggerLevel )
{
    Stream_बफर_t * स्थिर px_Stream_बफर = x_Stream_बफर;
    BaseType_t xReturn;

    trace_ENTER_x_Stream_बफर_Set_Trigger_Level( x_Stream_बफर, xTriggerLevel );

    configASSERT( px_Stream_बफर );

    /* It is not valid for the trigger level to be 0. */
    अगर( xTriggerLevel == ( माप_प्रकार ) 0 )
    {
        xTriggerLevel = ( माप_प्रकार ) 1;
    }

    /* The trigger level is the number of bytes that must be in the stream
     * buffer before a task that is waiting for data is unblocked. */
    अगर( xTriggerLevel < px_Stream_बफर->x_लंबाई )
    {
        px_Stream_बफर->xTriggerLevelBytes = xTriggerLevel;
        xReturn = pdPASS;
    }
    अन्यथा
    {
        xReturn = pdFALSE;
    }

    trace_RETURN_x_Stream_बफर_Set_Trigger_Level( xReturn );

    वापस xReturn;
}
/*-----------------------------------------------------------*/

माप_प्रकार x_Stream_बफर_Spaces_Available( Stream_बफर_Handle_t x_Stream_बफर )
{
    स्थिर Stream_बफर_t * स्थिर px_Stream_बफर = x_Stream_बफर;
    माप_प्रकार xSpace;
    माप_प्रकार x_Original_पुच्छ;

    trace_ENTER_x_Stream_बफर_Spaces_Available( x_Stream_बफर );

    configASSERT( px_Stream_बफर );

    /* The code below reads xTail and then xHead.  This is safe if the stream
     * buffer is updated once between the two reads - but not if the stream buffer
     * is updated more than once between the two reads - hence the loop. */
    करो
    {
        x_Original_पुच्छ = px_Stream_बफर->x_पुच्छ;
        xSpace = px_Stream_बफर->x_लंबाई + px_Stream_बफर->x_पुच्छ;
        xSpace -= px_Stream_बफर->x_शीर्ष;
    } जबतक( x_Original_पुच्छ != px_Stream_बफर->x_पुच्छ );

    xSpace -= ( माप_प्रकार ) 1;

    अगर( xSpace >= px_Stream_बफर->x_लंबाई )
    {
        xSpace -= px_Stream_बफर->x_लंबाई;
    }
    अन्यथा
    {
        mtCOVERAGE_TEST_MARKER();
    }

    trace_RETURN_x_Stream_बफर_Spaces_Available( xSpace );

    वापस xSpace;
}
/*-----------------------------------------------------------*/

माप_प्रकार x_Stream_बफर_Bytes_Available( Stream_बफर_Handle_t x_Stream_बफर )
{
    स्थिर Stream_बफर_t * स्थिर px_Stream_बफर = x_Stream_बफर;
    माप_प्रकार xReturn;

    trace_ENTER_x_Stream_बफर_Bytes_Available( x_Stream_बफर );

    configASSERT( px_Stream_बफर );

    xReturn = prv_Bytes_In_बफर( px_Stream_बफर );

    trace_RETURN_x_Stream_बफर_Bytes_Available( xReturn );

    वापस xReturn;
}
/*-----------------------------------------------------------*/

माप_प्रकार x_Stream_बफर_Send( Stream_बफर_Handle_t x_Stream_बफर,
                          स्थिर व्योम * pv_Tx_तथ्य,
                          माप_प्रकार x_तथ्य_लंबाई_Bytes,
                          TickType_t xTicksToWait )
{
    Stream_बफर_t * स्थिर px_Stream_बफर = x_Stream_बफर;
    माप_प्रकार xReturn, xSpace = 0;
    माप_प्रकार xRequiredSpace = x_तथ्य_लंबाई_Bytes;
    TimeOut_t xTimeOut;
    माप_प्रकार xMaxReportedSpace = 0;

    trace_ENTER_x_Stream_बफर_Send( x_Stream_बफर, pv_Tx_तथ्य, x_तथ्य_लंबाई_Bytes, xTicksToWait );

    configASSERT( pv_Tx_तथ्य );
    configASSERT( px_Stream_बफर );

    /* The maximum amount of space a stream buffer will ever report is its length
     * minus 1. */
    xMaxReportedSpace = px_Stream_बफर->x_लंबाई - ( माप_प्रकार ) 1;

    /* This send function is used to write to both message buffers and stream
     * buffers.  If this is a message buffer then the space needed must be
     * increased by the amount of bytes needed to store the length of the
     * message. */
    अगर( ( px_Stream_बफर->ucFlags & sb_FLAGS_IS_MESSAGE_बफर ) != ( uint8_t ) 0 )
    {
        xRequiredSpace += sb_BYTES_TO_STORE_MESSAGE_लंबाई;

        /* Overflow? */
        configASSERT( xRequiredSpace > x_तथ्य_लंबाई_Bytes );

        /* If this is a message buffer then it must be possible to write the
         * whole message. */
        अगर( xRequiredSpace > xMaxReportedSpace )
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
        अगर( xRequiredSpace > xMaxReportedSpace )
        {
            xRequiredSpace = xMaxReportedSpace;
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }

    अगर( xTicksToWait != ( TickType_t ) 0 )
    {
        vTaskSetTimeOutState( &xTimeOut );

        करो
        {
            /* Wait until the required number of bytes are free in the message
             * buffer. */
            taskENTER_CRITICAL();
            {
                xSpace = x_Stream_बफर_Spaces_Available( px_Stream_बफर );

                अगर( xSpace < xRequiredSpace )
                {
                    /* Clear notification state as going to wait for space. */
                    ( व्योम ) xTaskNotifyStateClearIndexed( शून्य, px_Stream_बफर->ux_Notification_सूचकांक );

                    /* Should only be one writer. */
                    configASSERT( px_Stream_बफर->xTaskWaitingToSend == शून्य );
                    px_Stream_बफर->xTaskWaitingToSend = xTaskGetCurrentTaskHandle();
                }
                अन्यथा
                {
                    taskEXIT_CRITICAL();
                    अवरोध;
                }
            }
            taskEXIT_CRITICAL();

            trace_BLOCKING_ON_STREAM_बफर_SEND( x_Stream_बफर );
            ( व्योम ) xTaskNotifyWaitIndexed( px_Stream_बफर->ux_Notification_सूचकांक, ( uint32_t ) 0, ( uint32_t ) 0, शून्य, xTicksToWait );
            px_Stream_बफर->xTaskWaitingToSend = शून्य;
        } जबतक( xTaskCheckForTimeOut( &xTimeOut, &xTicksToWait ) == pdFALSE );
    }
    अन्यथा
    {
        mtCOVERAGE_TEST_MARKER();
    }

    अगर( xSpace == ( माप_प्रकार ) 0 )
    {
        xSpace = x_Stream_बफर_Spaces_Available( px_Stream_बफर );
    }
    अन्यथा
    {
        mtCOVERAGE_TEST_MARKER();
    }

    xReturn = prv_Write_Message_To_बफर( px_Stream_बफर, pv_Tx_तथ्य, x_तथ्य_लंबाई_Bytes, xSpace, xRequiredSpace );

    अगर( xReturn > ( माप_प्रकार ) 0 )
    {
        trace_STREAM_बफर_SEND( x_Stream_बफर, xReturn );

        /* Was a task waiting for the data? */
        अगर( prv_Bytes_In_बफर( px_Stream_बफर ) >= px_Stream_बफर->xTriggerLevelBytes )
        {
            prvSEND_COMPLETED( px_Stream_बफर );
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    अन्यथा
    {
        mtCOVERAGE_TEST_MARKER();
        trace_STREAM_बफर_SEND_FAILED( x_Stream_बफर );
    }

    trace_RETURN_x_Stream_बफर_Send( xReturn );

    वापस xReturn;
}
/*-----------------------------------------------------------*/

माप_प्रकार x_Stream_बफर_Send_From_ISR( Stream_बफर_Handle_t x_Stream_बफर,
                                 स्थिर व्योम * pv_Tx_तथ्य,
                                 माप_प्रकार x_तथ्य_लंबाई_Bytes,
                                 BaseType_t * स्थिर pxHigherPriorityTaskWoken )
{
    Stream_बफर_t * स्थिर px_Stream_बफर = x_Stream_बफर;
    माप_प्रकार xReturn, xSpace;
    माप_प्रकार xRequiredSpace = x_तथ्य_लंबाई_Bytes;

    trace_ENTER_x_Stream_बफर_Send_From_ISR( x_Stream_बफर, pv_Tx_तथ्य, x_तथ्य_लंबाई_Bytes, pxHigherPriorityTaskWoken );

    configASSERT( pv_Tx_तथ्य );
    configASSERT( px_Stream_बफर );

    /* This send function is used to write to both message buffers and stream
     * buffers.  If this is a message buffer then the space needed must be
     * increased by the amount of bytes needed to store the length of the
     * message. */
    अगर( ( px_Stream_बफर->ucFlags & sb_FLAGS_IS_MESSAGE_बफर ) != ( uint8_t ) 0 )
    {
        xRequiredSpace += sb_BYTES_TO_STORE_MESSAGE_लंबाई;
    }
    अन्यथा
    {
        mtCOVERAGE_TEST_MARKER();
    }

    xSpace = x_Stream_बफर_Spaces_Available( px_Stream_बफर );
    xReturn = prv_Write_Message_To_बफर( px_Stream_बफर, pv_Tx_तथ्य, x_तथ्य_लंबाई_Bytes, xSpace, xRequiredSpace );

    अगर( xReturn > ( माप_प्रकार ) 0 )
    {
        /* Was a task waiting for the data? */
        अगर( prv_Bytes_In_बफर( px_Stream_बफर ) >= px_Stream_बफर->xTriggerLevelBytes )
        {
            /* MISRA Ref 4.7.1 [Return value shall be checked] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
            /* coverity[misra_c_2012_directive_4_7_violation] */
            prvSEND_COMPLETE_FROM_ISR( px_Stream_बफर, pxHigherPriorityTaskWoken );
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

    trace_STREAM_बफर_SEND_FROM_ISR( x_Stream_बफर, xReturn );
    trace_RETURN_x_Stream_बफर_Send_From_ISR( xReturn );

    वापस xReturn;
}
/*-----------------------------------------------------------*/

अटल माप_प्रकार prv_Write_Message_To_बफर( Stream_बफर_t * स्थिर px_Stream_बफर,
                                       स्थिर व्योम * pv_Tx_तथ्य,
                                       माप_प्रकार x_तथ्य_लंबाई_Bytes,
                                       माप_प्रकार xSpace,
                                       माप_प्रकार xRequiredSpace )
{
    माप_प्रकार x_अग्र_शीर्ष = px_Stream_बफर->x_शीर्ष;
    config_MESSAGE_बफर_लंबाई_TYPE x_Message_लंबाई;

    अगर( ( px_Stream_बफर->ucFlags & sb_FLAGS_IS_MESSAGE_बफर ) != ( uint8_t ) 0 )
    {
        /* This is a message buffer, as opposed to a stream buffer. */

        /* Convert xDataLengthBytes to the message length type. */
        x_Message_लंबाई = ( config_MESSAGE_बफर_लंबाई_TYPE ) x_तथ्य_लंबाई_Bytes;

        /* Ensure the data length given fits within configMESSAGE_BUFFER_LENGTH_TYPE. */
        configASSERT( ( माप_प्रकार ) x_Message_लंबाई == x_तथ्य_लंबाई_Bytes );

        अगर( xSpace >= xRequiredSpace )
        {
            /* There is enough space to write both the message length and the message
             * itself into the buffer.  Start by writing the length of the data, the data
             * itself will be written later in this function. */
            x_अग्र_शीर्ष = prv_Write_Bytes_To_बफर( px_Stream_बफर, ( स्थिर uint8_t * ) &( x_Message_लंबाई ), sb_BYTES_TO_STORE_MESSAGE_लंबाई, x_अग्र_शीर्ष );
        }
        अन्यथा
        {
            /* Not enough space, so do not write data to the buffer. */
            x_तथ्य_लंबाई_Bytes = 0;
        }
    }
    अन्यथा
    {
        /* This is a stream buffer, as opposed to a message buffer, so writing a
         * stream of bytes rather than discrete messages.  Plan to write as many
         * bytes as possible. */
        x_तथ्य_लंबाई_Bytes = configMIN( x_तथ्य_लंबाई_Bytes, xSpace );
    }

    अगर( x_तथ्य_लंबाई_Bytes != ( माप_प्रकार ) 0 )
    {
        /* Write the data to the buffer. */
        /* MISRA Ref 11.5.5 [Void pointer assignment] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
        /* coverity[misra_c_2012_rule_11_5_violation] */
        px_Stream_बफर->x_शीर्ष = prv_Write_Bytes_To_बफर( px_Stream_बफर, ( स्थिर uint8_t * ) pv_Tx_तथ्य, x_तथ्य_लंबाई_Bytes, x_अग्र_शीर्ष );
    }

    वापस x_तथ्य_लंबाई_Bytes;
}
/*-----------------------------------------------------------*/

माप_प्रकार x_Stream_बफर_Receive( Stream_बफर_Handle_t x_Stream_बफर,
                             व्योम * pv_Rx_तथ्य,
                             माप_प्रकार x_बफर_लंबाई_Bytes,
                             TickType_t xTicksToWait )
{
    Stream_बफर_t * स्थिर px_Stream_बफर = x_Stream_बफर;
    माप_प्रकार x_Received_लंबाई = 0, xBytesAvailable, x_Bytes_To_Store_Message_लंबाई;

    trace_ENTER_x_Stream_बफर_Receive( x_Stream_बफर, pv_Rx_तथ्य, x_बफर_लंबाई_Bytes, xTicksToWait );

    configASSERT( pv_Rx_तथ्य );
    configASSERT( px_Stream_बफर );

    /* This receive function is used by both message buffers, which store
     * discrete messages, and stream buffers, which store a continuous stream of
     * bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the
     * message. */
    अगर( ( px_Stream_बफर->ucFlags & sb_FLAGS_IS_MESSAGE_बफर ) != ( uint8_t ) 0 )
    {
        x_Bytes_To_Store_Message_लंबाई = sb_BYTES_TO_STORE_MESSAGE_लंबाई;
    }
    अन्यथा अगर( ( px_Stream_बफर->ucFlags & sb_FLAGS_IS_BATCHING_बफर ) != ( uint8_t ) 0 )
    {
        /* Force task to block if the batching buffer contains less bytes than
         * the trigger level. */
        x_Bytes_To_Store_Message_लंबाई = px_Stream_बफर->xTriggerLevelBytes;
    }
    अन्यथा
    {
        x_Bytes_To_Store_Message_लंबाई = 0;
    }

    अगर( xTicksToWait != ( TickType_t ) 0 )
    {
        /* Checking if there is data and clearing the notification state must be
         * performed atomically. */
        taskENTER_CRITICAL();
        {
            xBytesAvailable = prv_Bytes_In_बफर( px_Stream_बफर );

            /* If this function was invoked by a message buffer read then
             * xBytesToStoreMessageLength holds the number of bytes used to hold
             * the length of the next discrete message.  If this function was
             * invoked by a stream buffer read then xBytesToStoreMessageLength will
             * be 0. If this function was invoked by a stream batch buffer read
             * then xBytesToStoreMessageLength will be xTriggerLevelBytes value
             * for the buffer.*/
            अगर( xBytesAvailable <= x_Bytes_To_Store_Message_लंबाई )
            {
                /* Clear notification state as going to wait for data. */
                ( व्योम ) xTaskNotifyStateClearIndexed( शून्य, px_Stream_बफर->ux_Notification_सूचकांक );

                /* Should only be one reader. */
                configASSERT( px_Stream_बफर->xTaskWaitingToReceive == शून्य );
                px_Stream_बफर->xTaskWaitingToReceive = xTaskGetCurrentTaskHandle();
            }
            अन्यथा
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        taskEXIT_CRITICAL();

        अगर( xBytesAvailable <= x_Bytes_To_Store_Message_लंबाई )
        {
            /* Wait for data to be available. */
            trace_BLOCKING_ON_STREAM_बफर_RECEIVE( x_Stream_बफर );
            ( व्योम ) xTaskNotifyWaitIndexed( px_Stream_बफर->ux_Notification_सूचकांक, ( uint32_t ) 0, ( uint32_t ) 0, शून्य, xTicksToWait );
            px_Stream_बफर->xTaskWaitingToReceive = शून्य;

            /* Recheck the data available after blocking. */
            xBytesAvailable = prv_Bytes_In_बफर( px_Stream_बफर );
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    अन्यथा
    {
        xBytesAvailable = prv_Bytes_In_बफर( px_Stream_बफर );
    }

    /* Whether receiving a discrete message (where xBytesToStoreMessageLength
     * holds the number of bytes used to store the message length) or a stream of
     * bytes (where xBytesToStoreMessageLength is zero), the number of bytes
     * available must be greater than xBytesToStoreMessageLength to be able to
     * read bytes from the buffer. */
    अगर( xBytesAvailable > x_Bytes_To_Store_Message_लंबाई )
    {
        x_Received_लंबाई = prv_Read_Message_From_बफर( px_Stream_बफर, pv_Rx_तथ्य, x_बफर_लंबाई_Bytes, xBytesAvailable );

        /* Was a task waiting for space in the buffer? */
        अगर( x_Received_लंबाई != ( माप_प्रकार ) 0 )
        {
            trace_STREAM_बफर_RECEIVE( x_Stream_बफर, x_Received_लंबाई );
            prvRECEIVE_COMPLETED( x_Stream_बफर );
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    अन्यथा
    {
        trace_STREAM_बफर_RECEIVE_FAILED( x_Stream_बफर );
        mtCOVERAGE_TEST_MARKER();
    }

    trace_RETURN_x_Stream_बफर_Receive( x_Received_लंबाई );

    वापस x_Received_लंबाई;
}
/*-----------------------------------------------------------*/

माप_प्रकार x_Stream_बफर_अग्र_Message_लंबाई_Bytes( Stream_बफर_Handle_t x_Stream_बफर )
{
    Stream_बफर_t * स्थिर px_Stream_बफर = x_Stream_बफर;
    माप_प्रकार xReturn, xBytesAvailable;
    config_MESSAGE_बफर_लंबाई_TYPE x_अस्थायी_Return;

    trace_ENTER_x_Stream_बफर_अग्र_Message_लंबाई_Bytes( x_Stream_बफर );

    configASSERT( px_Stream_बफर );

    /* Ensure the stream buffer is being used as a message buffer. */
    अगर( ( px_Stream_बफर->ucFlags & sb_FLAGS_IS_MESSAGE_बफर ) != ( uint8_t ) 0 )
    {
        xBytesAvailable = prv_Bytes_In_बफर( px_Stream_बफर );

        अगर( xBytesAvailable > sb_BYTES_TO_STORE_MESSAGE_लंबाई )
        {
            /* The number of bytes available is greater than the number of bytes
             * required to hold the length of the next message, so another message
             * is available. */
            ( व्योम ) prv_Read_Bytes_From_बफर( px_Stream_बफर, ( uint8_t * ) &x_अस्थायी_Return, sb_BYTES_TO_STORE_MESSAGE_लंबाई, px_Stream_बफर->x_पुच्छ );
            xReturn = ( माप_प्रकार ) x_अस्थायी_Return;
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

    trace_RETURN_x_Stream_बफर_अग्र_Message_लंबाई_Bytes( xReturn );

    वापस xReturn;
}
/*-----------------------------------------------------------*/

माप_प्रकार x_Stream_बफर_Receive_From_ISR( Stream_बफर_Handle_t x_Stream_बफर,
                                    व्योम * pv_Rx_तथ्य,
                                    माप_प्रकार x_बफर_लंबाई_Bytes,
                                    BaseType_t * स्थिर pxHigherPriorityTaskWoken )
{
    Stream_बफर_t * स्थिर px_Stream_बफर = x_Stream_बफर;
    माप_प्रकार x_Received_लंबाई = 0, xBytesAvailable, x_Bytes_To_Store_Message_लंबाई;

    trace_ENTER_x_Stream_बफर_Receive_From_ISR( x_Stream_बफर, pv_Rx_तथ्य, x_बफर_लंबाई_Bytes, pxHigherPriorityTaskWoken );

    configASSERT( pv_Rx_तथ्य );
    configASSERT( px_Stream_बफर );

    /* This receive function is used by both message buffers, which store
     * discrete messages, and stream buffers, which store a continuous stream of
     * bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the
     * message. */
    अगर( ( px_Stream_बफर->ucFlags & sb_FLAGS_IS_MESSAGE_बफर ) != ( uint8_t ) 0 )
    {
        x_Bytes_To_Store_Message_लंबाई = sb_BYTES_TO_STORE_MESSAGE_लंबाई;
    }
    अन्यथा
    {
        x_Bytes_To_Store_Message_लंबाई = 0;
    }

    xBytesAvailable = prv_Bytes_In_बफर( px_Stream_बफर );

    /* Whether receiving a discrete message (where xBytesToStoreMessageLength
     * holds the number of bytes used to store the message length) or a stream of
     * bytes (where xBytesToStoreMessageLength is zero), the number of bytes
     * available must be greater than xBytesToStoreMessageLength to be able to
     * read bytes from the buffer. */
    अगर( xBytesAvailable > x_Bytes_To_Store_Message_लंबाई )
    {
        x_Received_लंबाई = prv_Read_Message_From_बफर( px_Stream_बफर, pv_Rx_तथ्य, x_बफर_लंबाई_Bytes, xBytesAvailable );

        /* Was a task waiting for space in the buffer? */
        अगर( x_Received_लंबाई != ( माप_प्रकार ) 0 )
        {
            /* MISRA Ref 4.7.1 [Return value shall be checked] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
            /* coverity[misra_c_2012_directive_4_7_violation] */
            prvRECEIVE_COMPLETED_FROM_ISR( px_Stream_बफर, pxHigherPriorityTaskWoken );
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

    trace_STREAM_बफर_RECEIVE_FROM_ISR( x_Stream_बफर, x_Received_लंबाई );
    trace_RETURN_x_Stream_बफर_Receive_From_ISR( x_Received_लंबाई );

    वापस x_Received_लंबाई;
}
/*-----------------------------------------------------------*/

अटल माप_प्रकार prv_Read_Message_From_बफर( Stream_बफर_t * px_Stream_बफर,
                                        व्योम * pv_Rx_तथ्य,
                                        माप_प्रकार x_बफर_लंबाई_Bytes,
                                        माप_प्रकार xBytesAvailable )
{
    माप_प्रकार x_गिनती, x_अग्र_Message_लंबाई;
    config_MESSAGE_बफर_लंबाई_TYPE x_अस्थायी_अग्र_Message_लंबाई;
    माप_प्रकार x_अग्र_पुच्छ = px_Stream_बफर->x_पुच्छ;

    अगर( ( px_Stream_बफर->ucFlags & sb_FLAGS_IS_MESSAGE_बफर ) != ( uint8_t ) 0 )
    {
        /* A discrete message is being received.  First receive the length
         * of the message. */
        x_अग्र_पुच्छ = prv_Read_Bytes_From_बफर( px_Stream_बफर, ( uint8_t * ) &x_अस्थायी_अग्र_Message_लंबाई, sb_BYTES_TO_STORE_MESSAGE_लंबाई, x_अग्र_पुच्छ );
        x_अग्र_Message_लंबाई = ( माप_प्रकार ) x_अस्थायी_अग्र_Message_लंबाई;

        /* Reduce the number of bytes available by the number of bytes just
         * read out. */
        xBytesAvailable -= sb_BYTES_TO_STORE_MESSAGE_लंबाई;

        /* Check there is enough space in the buffer provided by the
         * user. */
        अगर( x_अग्र_Message_लंबाई > x_बफर_लंबाई_Bytes )
        {
            /* The user has provided insufficient space to read the message. */
            x_अग्र_Message_लंबाई = 0;
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
        x_अग्र_Message_लंबाई = x_बफर_लंबाई_Bytes;
    }

    /* Use the minimum of the wanted bytes and the available bytes. */
    x_गिनती = configMIN( x_अग्र_Message_लंबाई, xBytesAvailable );

    अगर( x_गिनती != ( माप_प्रकार ) 0 )
    {
        /* Read the actual data and update the tail to mark the data as officially consumed. */
        /* MISRA Ref 11.5.5 [Void pointer assignment] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
        /* coverity[misra_c_2012_rule_11_5_violation] */
        px_Stream_बफर->x_पुच्छ = prv_Read_Bytes_From_बफर( px_Stream_बफर, ( uint8_t * ) pv_Rx_तथ्य, x_गिनती, x_अग्र_पुच्छ );
    }

    वापस x_गिनती;
}
/*-----------------------------------------------------------*/

BaseType_t x_Stream_बफर_Is_Empty( Stream_बफर_Handle_t x_Stream_बफर )
{
    स्थिर Stream_बफर_t * स्थिर px_Stream_बफर = x_Stream_बफर;
    BaseType_t xReturn;
    माप_प्रकार x_पुच्छ;

    trace_ENTER_x_Stream_बफर_Is_Empty( x_Stream_बफर );

    configASSERT( px_Stream_बफर );

    /* True if no bytes are available. */
    x_पुच्छ = px_Stream_बफर->x_पुच्छ;

    अगर( px_Stream_बफर->x_शीर्ष == x_पुच्छ )
    {
        xReturn = pdTRUE;
    }
    अन्यथा
    {
        xReturn = pdFALSE;
    }

    trace_RETURN_x_Stream_बफर_Is_Empty( xReturn );

    वापस xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t x_Stream_बफर_Is_Full( Stream_बफर_Handle_t x_Stream_बफर )
{
    BaseType_t xReturn;
    माप_प्रकार x_Bytes_To_Store_Message_लंबाई;
    स्थिर Stream_बफर_t * स्थिर px_Stream_बफर = x_Stream_बफर;

    trace_ENTER_x_Stream_बफर_Is_Full( x_Stream_बफर );

    configASSERT( px_Stream_बफर );

    /* This generic version of the receive function is used by both message
     * buffers, which store discrete messages, and stream buffers, which store a
     * continuous stream of bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the message. */
    अगर( ( px_Stream_बफर->ucFlags & sb_FLAGS_IS_MESSAGE_बफर ) != ( uint8_t ) 0 )
    {
        x_Bytes_To_Store_Message_लंबाई = sb_BYTES_TO_STORE_MESSAGE_लंबाई;
    }
    अन्यथा
    {
        x_Bytes_To_Store_Message_लंबाई = 0;
    }

    /* True if the available space equals zero. */
    अगर( x_Stream_बफर_Spaces_Available( x_Stream_बफर ) <= x_Bytes_To_Store_Message_लंबाई )
    {
        xReturn = pdTRUE;
    }
    अन्यथा
    {
        xReturn = pdFALSE;
    }

    trace_RETURN_x_Stream_बफर_Is_Full( xReturn );

    वापस xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t x_Stream_बफर_Send_Completed_From_ISR( Stream_बफर_Handle_t x_Stream_बफर,
                                              BaseType_t * pxHigherPriorityTaskWoken )
{
    Stream_बफर_t * स्थिर px_Stream_बफर = x_Stream_बफर;
    BaseType_t xReturn;
    UBaseType_t uxSavedInterruptStatus;

    trace_ENTER_x_Stream_बफर_Send_Completed_From_ISR( x_Stream_बफर, pxHigherPriorityTaskWoken );

    configASSERT( px_Stream_बफर );

    /* MISRA Ref 4.7.1 [Return value shall be checked] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
    /* coverity[misra_c_2012_directive_4_7_violation] */
    uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
    {
        अगर( ( px_Stream_बफर )->xTaskWaitingToReceive != शून्य )
        {
            ( व्योम ) xTaskNotifyIndexedFromISR( ( px_Stream_बफर )->xTaskWaitingToReceive,
                                                ( px_Stream_बफर )->ux_Notification_सूचकांक,
                                                ( uint32_t ) 0,
                                                eNoAction,
                                                pxHigherPriorityTaskWoken );
            ( px_Stream_बफर )->xTaskWaitingToReceive = शून्य;
            xReturn = pdTRUE;
        }
        अन्यथा
        {
            xReturn = pdFALSE;
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( uxSavedInterruptStatus );

    trace_RETURN_x_Stream_बफर_Send_Completed_From_ISR( xReturn );

    वापस xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t x_Stream_बफर_Receive_Completed_From_ISR( Stream_बफर_Handle_t x_Stream_बफर,
                                                 BaseType_t * pxHigherPriorityTaskWoken )
{
    Stream_बफर_t * स्थिर px_Stream_बफर = x_Stream_बफर;
    BaseType_t xReturn;
    UBaseType_t uxSavedInterruptStatus;

    trace_ENTER_x_Stream_बफर_Receive_Completed_From_ISR( x_Stream_बफर, pxHigherPriorityTaskWoken );

    configASSERT( px_Stream_बफर );

    /* MISRA Ref 4.7.1 [Return value shall be checked] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
    /* coverity[misra_c_2012_directive_4_7_violation] */
    uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
    {
        अगर( ( px_Stream_बफर )->xTaskWaitingToSend != शून्य )
        {
            ( व्योम ) xTaskNotifyIndexedFromISR( ( px_Stream_बफर )->xTaskWaitingToSend,
                                                ( px_Stream_बफर )->ux_Notification_सूचकांक,
                                                ( uint32_t ) 0,
                                                eNoAction,
                                                pxHigherPriorityTaskWoken );
            ( px_Stream_बफर )->xTaskWaitingToSend = शून्य;
            xReturn = pdTRUE;
        }
        अन्यथा
        {
            xReturn = pdFALSE;
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( uxSavedInterruptStatus );

    trace_RETURN_x_Stream_बफर_Receive_Completed_From_ISR( xReturn );

    वापस xReturn;
}
/*-----------------------------------------------------------*/

अटल माप_प्रकार prv_Write_Bytes_To_बफर( Stream_बफर_t * स्थिर px_Stream_बफर,
                                     स्थिर uint8_t * puc_तथ्य,
                                     माप_प्रकार x_गिनती,
                                     माप_प्रकार x_शीर्ष )
{
    माप_प्रकार x_First_लंबाई;

    configASSERT( x_गिनती > ( माप_प्रकार ) 0 );

    /* Calculate the number of bytes that can be added in the first write -
     * which may be less than the total number of bytes that need to be added if
     * the buffer will wrap back to the beginning. */
    x_First_लंबाई = configMIN( px_Stream_बफर->x_लंबाई - x_शीर्ष, x_गिनती );

    /* Write as many bytes as can be written in the first write. */
    configASSERT( ( x_शीर्ष + x_First_लंबाई ) <= px_Stream_बफर->x_लंबाई );
    ( व्योम ) स_नकल( ( व्योम * ) ( &( px_Stream_बफर->puc_बफर[ x_शीर्ष ] ) ), ( स्थिर व्योम * ) puc_तथ्य, x_First_लंबाई );

    /* If the number of bytes written was less than the number that could be
     * written in the first write... */
    अगर( x_गिनती > x_First_लंबाई )
    {
        /* ...then write the remaining bytes to the start of the buffer. */
        configASSERT( ( x_गिनती - x_First_लंबाई ) <= px_Stream_बफर->x_लंबाई );
        ( व्योम ) स_नकल( ( व्योम * ) px_Stream_बफर->puc_बफर, ( स्थिर व्योम * ) &( puc_तथ्य[ x_First_लंबाई ] ), x_गिनती - x_First_लंबाई );
    }
    अन्यथा
    {
        mtCOVERAGE_TEST_MARKER();
    }

    x_शीर्ष += x_गिनती;

    अगर( x_शीर्ष >= px_Stream_बफर->x_लंबाई )
    {
        x_शीर्ष -= px_Stream_बफर->x_लंबाई;
    }
    अन्यथा
    {
        mtCOVERAGE_TEST_MARKER();
    }

    वापस x_शीर्ष;
}
/*-----------------------------------------------------------*/

अटल माप_प्रकार prv_Read_Bytes_From_बफर( Stream_बफर_t * px_Stream_बफर,
                                      uint8_t * puc_तथ्य,
                                      माप_प्रकार x_गिनती,
                                      माप_प्रकार x_पुच्छ )
{
    माप_प्रकार x_First_लंबाई;

    configASSERT( x_गिनती != ( माप_प्रकार ) 0 );

    /* Calculate the number of bytes that can be read - which may be
     * less than the number wanted if the data wraps around to the start of
     * the buffer. */
    x_First_लंबाई = configMIN( px_Stream_बफर->x_लंबाई - x_पुच्छ, x_गिनती );

    /* Obtain the number of bytes it is possible to obtain in the first
     * read.  Asserts check bounds of read and write. */
    configASSERT( x_First_लंबाई <= x_गिनती );
    configASSERT( ( x_पुच्छ + x_First_लंबाई ) <= px_Stream_बफर->x_लंबाई );
    ( व्योम ) स_नकल( ( व्योम * ) puc_तथ्य, ( स्थिर व्योम * ) &( px_Stream_बफर->puc_बफर[ x_पुच्छ ] ), x_First_लंबाई );

    /* If the total number of wanted bytes is greater than the number
     * that could be read in the first read... */
    अगर( x_गिनती > x_First_लंबाई )
    {
        /* ...then read the remaining bytes from the start of the buffer. */
        ( व्योम ) स_नकल( ( व्योम * ) &( puc_तथ्य[ x_First_लंबाई ] ), ( व्योम * ) ( px_Stream_बफर->puc_बफर ), x_गिनती - x_First_लंबाई );
    }
    अन्यथा
    {
        mtCOVERAGE_TEST_MARKER();
    }

    /* Move the tail pointer to effectively remove the data read from the buffer. */
    x_पुच्छ += x_गिनती;

    अगर( x_पुच्छ >= px_Stream_बफर->x_लंबाई )
    {
        x_पुच्छ -= px_Stream_बफर->x_लंबाई;
    }

    वापस x_पुच्छ;
}
/*-----------------------------------------------------------*/

अटल माप_प्रकार prv_Bytes_In_बफर( स्थिर Stream_बफर_t * स्थिर px_Stream_बफर )
{
    /* Returns the distance between xTail and xHead. */
    माप_प्रकार x_गिनती;

    x_गिनती = px_Stream_बफर->x_लंबाई + px_Stream_बफर->x_शीर्ष;
    x_गिनती -= px_Stream_बफर->x_पुच्छ;

    अगर( x_गिनती >= px_Stream_बफर->x_लंबाई )
    {
        x_गिनती -= px_Stream_बफर->x_लंबाई;
    }
    अन्यथा
    {
        mtCOVERAGE_TEST_MARKER();
    }

    वापस x_गिनती;
}
/*-----------------------------------------------------------*/

अटल व्योम prv_Initialise_New_Stream_बफर( Stream_बफर_t * स्थिर px_Stream_बफर,
                                          uint8_t * स्थिर puc_बफर,
                                          माप_प्रकार x_बफर_आकार_Bytes,
                                          माप_प्रकार xTriggerLevelBytes,
                                          uint8_t ucFlags,
                                          Stream_बफर_Callback_Function_t pxSendCompletedCallback,
                                          Stream_बफर_Callback_Function_t pxReceiveCompletedCallback )
{
    /* Assert here is deliberately writing to the entire buffer to ensure it can
     * be written to without generating exceptions, and is setting the buffer to a
     * known value to assist in development/debugging. */
    #अगर ( configASSERT_DEFINED == 1 )
    {
        /* The value written just has to be identifiable when looking at the
         * memory.  Don't use 0xA5 as that is the stack fill value and could
         * result in confusion as to what is actually being observed. */
        #घोषणा STREAM_बफर_बफर_WRITE_मूल्य    ( 0x55 )
        configASSERT( स_रखो( puc_बफर, ( पूर्णांक ) STREAM_बफर_बफर_WRITE_मूल्य, x_बफर_आकार_Bytes ) == puc_बफर );
    }
    #पूर्ण_अगर

    ( व्योम ) स_रखो( ( व्योम * ) px_Stream_बफर, 0x00, माप( Stream_बफर_t ) );
    px_Stream_बफर->puc_बफर = puc_बफर;
    px_Stream_बफर->x_लंबाई = x_बफर_आकार_Bytes;
    px_Stream_बफर->xTriggerLevelBytes = xTriggerLevelBytes;
    px_Stream_बफर->ucFlags = ucFlags;
    px_Stream_बफर->ux_Notification_सूचकांक = tsk_DEFAULT_सूचकांक_TO_NOTIFY;
    #अगर ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    {
        px_Stream_बफर->pxSendCompletedCallback = pxSendCompletedCallback;
        px_Stream_बफर->pxReceiveCompletedCallback = pxReceiveCompletedCallback;
    }
    #अन्यथा
    {
        /* MISRA Ref 11.1.1 [Object type casting] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-111 */
        /* coverity[misra_c_2012_rule_11_1_violation] */
        ( व्योम ) pxSendCompletedCallback;

        /* MISRA Ref 11.1.1 [Object type casting] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-111 */
        /* coverity[misra_c_2012_rule_11_1_violation] */
        ( व्योम ) pxReceiveCompletedCallback;
    }
    #पूर्ण_अगर /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
}
/*-----------------------------------------------------------*/

UBaseType_t ux_Stream_बफर_Get_Stream_बफर_Notification_सूचकांक( Stream_बफर_Handle_t x_Stream_बफर )
{
    Stream_बफर_t * स्थिर px_Stream_बफर = x_Stream_बफर;

    trace_ENTER_ux_Stream_बफर_Get_Stream_बफर_Notification_सूचकांक( x_Stream_बफर );

    configASSERT( px_Stream_बफर );

    trace_RETURN_ux_Stream_बफर_Get_Stream_बफर_Notification_सूचकांक( px_Stream_बफर->ux_Notification_सूचकांक );

    वापस px_Stream_बफर->ux_Notification_सूचकांक;
}
/*-----------------------------------------------------------*/

व्योम v_Stream_बफर_Set_Stream_बफर_Notification_सूचकांक( Stream_बफर_Handle_t x_Stream_बफर,
                                                    UBaseType_t ux_Notification_सूचकांक )
{
    Stream_बफर_t * स्थिर px_Stream_बफर = x_Stream_बफर;

    trace_ENTER_v_Stream_बफर_Set_Stream_बफर_Notification_सूचकांक( x_Stream_बफर, ux_Notification_सूचकांक );

    configASSERT( px_Stream_बफर );

    /* There should be no task waiting otherwise we'd never resume them. */
    configASSERT( px_Stream_बफर->xTaskWaitingToReceive == शून्य );
    configASSERT( px_Stream_बफर->xTaskWaitingToSend == शून्य );

    /* Check that the task notification index is valid. */
    configASSERT( ux_Notification_सूचकांक < configTASK_NOTIFICATION_ARRAY_ENTRIES );

    px_Stream_बफर->ux_Notification_सूचकांक = ux_Notification_सूचकांक;

    trace_RETURN_v_Stream_बफर_Set_Stream_बफर_Notification_सूचकांक();
}
/*-----------------------------------------------------------*/

    #अगर ( configUSE_TRACE_FACILITY == 1 )

    UBaseType_t ux_Stream_बफर_Get_Stream_बफर_संख्या( Stream_बफर_Handle_t x_Stream_बफर )
    {
        trace_ENTER_ux_Stream_बफर_Get_Stream_बफर_संख्या( x_Stream_बफर );

        trace_RETURN_ux_Stream_बफर_Get_Stream_बफर_संख्या( x_Stream_बफर->ux_Stream_बफर_संख्या );

        वापस x_Stream_बफर->ux_Stream_बफर_संख्या;
    }

    #पूर्ण_अगर /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

    #अगर ( configUSE_TRACE_FACILITY == 1 )

    व्योम v_Stream_बफर_Set_Stream_बफर_संख्या( Stream_बफर_Handle_t x_Stream_बफर,
                                             UBaseType_t ux_Stream_बफर_संख्या )
    {
        trace_ENTER_v_Stream_बफर_Set_Stream_बफर_संख्या( x_Stream_बफर, ux_Stream_बफर_संख्या );

        x_Stream_बफर->ux_Stream_बफर_संख्या = ux_Stream_बफर_संख्या;

        trace_RETURN_v_Stream_बफर_Set_Stream_बफर_संख्या();
    }

    #पूर्ण_अगर /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

    #अगर ( configUSE_TRACE_FACILITY == 1 )

    uint8_t uc_Stream_बफर_Get_Stream_बफर_Type( Stream_बफर_Handle_t x_Stream_बफर )
    {
        trace_ENTER_uc_Stream_बफर_Get_Stream_बफर_Type( x_Stream_बफर );

        trace_RETURN_uc_Stream_बफर_Get_Stream_बफर_Type( ( uint8_t ) ( x_Stream_बफर->ucFlags & sb_FLAGS_IS_MESSAGE_बफर ) );

        वापस( ( uint8_t ) ( x_Stream_बफर->ucFlags & sb_FLAGS_IS_MESSAGE_बफर ) );
    }

    #पूर्ण_अगर /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

/* This entire source file will be skipped if the application is not configured
 * to include stream buffer functionality. This #if is closed at the very bottom
 * of this file. If you want to include stream buffers then ensure
 * configUSE_STREAM_BUFFERS is set to 1 in FreeRTOSConfig.h. */
#पूर्ण_अगर /* configUSE_STREAM_BUFFERS == 1 */
