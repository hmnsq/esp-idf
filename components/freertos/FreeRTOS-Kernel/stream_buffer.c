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
#परिभाषय MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* FreeRTOS includes. */
#अन्तर्भाव "FreeRTOS.h"
#अन्तर्भाव "task.h"
#अन्तर्भाव "stream_buffer.h"
/* Include private IDF API additions for critical thread safety macros */
#अन्तर्भाव "esp_private/freertos_idf_additions_priv.h"

#यदि ( configUSE_TASK_NOTIFICATIONS != 1 )
    #दोषः configUSE_TASK_NOTIFICATIONS must be समुच्चयः गन्तव्यम् 1 गन्तव्यम् build प्रवाहः_बफर.c
#यदि_अन्तः

#यदि ( INCLUDE_xTaskGetCurrentTaskHandle != 1 )
    #दोषः INCLUDE_xTaskGetCurrentTaskHandle must be समुच्चयः गन्तव्यम् 1 गन्तव्यम् build प्रवाहः_बफर.c
#यदि_अन्तः

/* Lint e961, e9021 and e750 are suppressed as a MISRA exception justified
 * because the MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#अपरिभाषय MPU_WRAPPERS_INCLUDED_FROM_API_FILE /*lint !e961 !e750 !e9021. */

/* If the user has not provided application specific Rx notification macros,
 * or #defined the notification macros away, then provide default implementations
 * that uses task notifications. */
/*lint -save -e9026 Function like macros allowed and needed here so they can be overridden. */
#यदि_अपरिभाषितम् sbRECEIVE_COMPLETED
    #परिभाषय sbRECEIVE_COMPLETED( px_प्रवाहः_बफर )                               \
    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( px_प्रवाहः_बफर->x_प्रवाहः_बफर_तालकम् ) ); \
    {                                                                           \
        यदि( ( px_प्रवाहः_बफर )->x_Task_प्रतीक्षा_गन्तव्यम्_Send != रिक्त )                    \
        {                                                                       \
            ( शून्य ) xTaskNotify( ( px_प्रवाहः_बफर )->x_Task_प्रतीक्षा_गन्तव्यम्_Send,       \
                                  ( uint32_t ) 0,                               \
                                  eNoAction );                                  \
            ( px_प्रवाहः_बफर )->x_Task_प्रतीक्षा_गन्तव्यम्_Send = रिक्त;                      \
        }                                                                       \
    }                                                                           \
    ( शून्य ) prvEXIT_CRITICAL_OR_RESUME_ALL( &( px_प्रवाहः_बफर->x_प्रवाहः_बफर_तालकम् ) );
#यदि_अन्तः /* sbRECEIVE_COMPLETED */

/* If user has provided a per-instance receive complete callback, then
 * invoke the callback else use the receive complete macro which is provided by default for all instances.
 */
#यदि ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #परिभाषय prvRECEIVE_COMPLETED( px_प्रवाहः_बफर )                                               \
    {                                                                                            \
        यदि( ( px_प्रवाहः_बफर )->pxReceiveCompletedCallback != रिक्त )                             \
        {                                                                                        \
            ( px_प्रवाहः_बफर )->pxReceiveCompletedCallback( ( px_प्रवाहः_बफर ), pdFALSE, रिक्त ); \
        }                                                                                        \
        अन्यथा                                                                                     \
        {                                                                                        \
            sbRECEIVE_COMPLETED( ( px_प्रवाहः_बफर ) );                                           \
        }                                                                                        \
    }
#अन्यथा /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #परिभाषय prvRECEIVE_COMPLETED( px_प्रवाहः_बफर )    sbRECEIVE_COMPLETED( ( px_प्रवाहः_बफर ) )
#यदि_अन्तः /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

#यदि_अपरिभाषितम् sbRECEIVE_COMPLETED_FROM_ISR
    #परिभाषय sbRECEIVE_COMPLETED_FROM_ISR( px_प्रवाहः_बफर,                            \
                                          px_Higher_प्राथमिकता_Task_Woken )                \
    {                                                                                \
        UBaseType_t ux_Saved_व्यवधानम्_स्थितिः;                                          \
                                                                                     \
        ux_Saved_व्यवधानम्_स्थितिः = ( UBaseType_t ) पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्_FROM_ISR();  \
        {                                                                            \
            यदि( ( px_प्रवाहः_बफर )->x_Task_प्रतीक्षा_गन्तव्यम्_Send != रिक्त )                     \
            {                                                                        \
                ( शून्य ) xTaskNotifyFromISR( ( px_प्रवाहः_बफर )->x_Task_प्रतीक्षा_गन्तव्यम्_Send, \
                                             ( uint32_t ) 0,                         \
                                             eNoAction,                              \
                                             ( px_Higher_प्राथमिकता_Task_Woken ) );        \
                ( px_प्रवाहः_बफर )->x_Task_प्रतीक्षा_गन्तव्यम्_Send = रिक्त;                       \
            }                                                                        \
        }                                                                            \
        पत्तनम्_CLEAR_व्यवधानम्_आवरणम्_FROM_ISR( ux_Saved_व्यवधानम्_स्थितिः );                 \
    }
#यदि_अन्तः /* sbRECEIVE_COMPLETED_FROM_ISR */

#यदि ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #परिभाषय prvRECEIVE_COMPLETED_FROM_ISR( px_प्रवाहः_बफर,                                                               \
                                           px_Higher_प्राथमिकता_Task_Woken )                                                   \
    {                                                                                                                    \
        यदि( ( px_प्रवाहः_बफर )->pxReceiveCompletedCallback != रिक्त )                                                     \
        {                                                                                                                \
            ( px_प्रवाहः_बफर )->pxReceiveCompletedCallback( ( px_प्रवाहः_बफर ), pdTRUE, ( px_Higher_प्राथमिकता_Task_Woken ) ); \
        }                                                                                                                \
        अन्यथा                                                                                                             \
        {                                                                                                                \
            sbRECEIVE_COMPLETED_FROM_ISR( ( px_प्रवाहः_बफर ), ( px_Higher_प्राथमिकता_Task_Woken ) );                           \
        }                                                                                                                \
    }
#अन्यथा /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #परिभाषय prvRECEIVE_COMPLETED_FROM_ISR( px_प्रवाहः_बफर, px_Higher_प्राथमिकता_Task_Woken ) \
    sbRECEIVE_COMPLETED_FROM_ISR( ( px_प्रवाहः_बफर ), ( px_Higher_प्राथमिकता_Task_Woken ) )
#यदि_अन्तः /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

/* If the user has not provided an application specific Tx notification macro,
 * or #defined the notification macro away, then provide a default
 * implementation that uses task notifications.
 */
#यदि_अपरिभाषितम् sbSEND_COMPLETED
    #परिभाषय sbSEND_COMPLETED( px_प्रवाहः_बफर )                                  \
    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( px_प्रवाहः_बफर->x_प्रवाहः_बफर_तालकम् ) ); \
    {                                                                           \
        यदि( ( px_प्रवाहः_बफर )->x_Task_प्रतीक्षा_गन्तव्यम्_Receive != रिक्त )                 \
        {                                                                       \
            ( शून्य ) xTaskNotify( ( px_प्रवाहः_बफर )->x_Task_प्रतीक्षा_गन्तव्यम्_Receive,    \
                                  ( uint32_t ) 0,                               \
                                  eNoAction );                                  \
            ( px_प्रवाहः_बफर )->x_Task_प्रतीक्षा_गन्तव्यम्_Receive = रिक्त;                   \
        }                                                                       \
    }                                                                           \
    ( शून्य ) prvEXIT_CRITICAL_OR_RESUME_ALL( &( px_प्रवाहः_बफर->x_प्रवाहः_बफर_तालकम् ) );
#यदि_अन्तः /* sbSEND_COMPLETED */

/* If user has provided a per-instance send completed callback, then
 * invoke the callback else use the send complete macro which is provided by default for all instances.
 */
#यदि ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #परिभाषय prvSEND_COMPLETED( px_प्रवाहः_बफर )                                           \
    {                                                                                     \
        यदि( ( px_प्रवाहः_बफर )->pxSendCompletedCallback != रिक्त )                         \
        {                                                                                 \
            px_प्रवाहः_बफर->pxSendCompletedCallback( ( px_प्रवाहः_बफर ), pdFALSE, रिक्त ); \
        }                                                                                 \
        अन्यथा                                                                              \
        {                                                                                 \
            sbSEND_COMPLETED( ( px_प्रवाहः_बफर ) );                                       \
        }                                                                                 \
    }
#अन्यथा /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #परिभाषय prvSEND_COMPLETED( px_प्रवाहः_बफर )    sbSEND_COMPLETED( ( px_प्रवाहः_बफर ) )
#यदि_अन्तः /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */


#यदि_अपरिभाषितम् sbSEND_COMPLETE_FROM_ISR
    #परिभाषय sbSEND_COMPLETE_FROM_ISR( px_प्रवाहः_बफर, px_Higher_प्राथमिकता_Task_Woken )       \
    {                                                                                   \
        UBaseType_t ux_Saved_व्यवधानम्_स्थितिः;                                             \
                                                                                        \
        ux_Saved_व्यवधानम्_स्थितिः = ( UBaseType_t ) पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्_FROM_ISR();     \
        {                                                                               \
            यदि( ( px_प्रवाहः_बफर )->x_Task_प्रतीक्षा_गन्तव्यम्_Receive != रिक्त )                     \
            {                                                                           \
                ( शून्य ) xTaskNotifyFromISR( ( px_प्रवाहः_बफर )->x_Task_प्रतीक्षा_गन्तव्यम्_Receive, \
                                             ( uint32_t ) 0,                            \
                                             eNoAction,                                 \
                                             ( px_Higher_प्राथमिकता_Task_Woken ) );           \
                ( px_प्रवाहः_बफर )->x_Task_प्रतीक्षा_गन्तव्यम्_Receive = रिक्त;                       \
            }                                                                           \
        }                                                                               \
        पत्तनम्_CLEAR_व्यवधानम्_आवरणम्_FROM_ISR( ux_Saved_व्यवधानम्_स्थितिः );                    \
    }
#यदि_अन्तः /* sbSEND_COMPLETE_FROM_ISR */


#यदि ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #परिभाषय prvSEND_COMPLETE_FROM_ISR( px_प्रवाहः_बफर, px_Higher_प्राथमिकता_Task_Woken )                                    \
    {                                                                                                                 \
        यदि( ( px_प्रवाहः_बफर )->pxSendCompletedCallback != रिक्त )                                                     \
        {                                                                                                             \
            ( px_प्रवाहः_बफर )->pxSendCompletedCallback( ( px_प्रवाहः_बफर ), pdTRUE, ( px_Higher_प्राथमिकता_Task_Woken ) ); \
        }                                                                                                             \
        अन्यथा                                                                                                          \
        {                                                                                                             \
            sbSEND_COMPLETE_FROM_ISR( ( px_प्रवाहः_बफर ), ( px_Higher_प्राथमिकता_Task_Woken ) );                            \
        }                                                                                                             \
    }
#अन्यथा /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #परिभाषय prvSEND_COMPLETE_FROM_ISR( px_प्रवाहः_बफर, px_Higher_प्राथमिकता_Task_Woken ) \
    sbSEND_COMPLETE_FROM_ISR( ( px_प्रवाहः_बफर ), ( px_Higher_प्राथमिकता_Task_Woken ) )
#यदि_अन्तः /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

/*lint -restore (9026) */

/* The number of bytes used to hold the length of a message in the buffer. */
#परिभाषय sb_अष्टकानि_गन्तव्यम्_STORE_सन्देशः_दीर्घता    ( परिमाण( config_सन्देशः_बफर_दीर्घता_TYPE ) )

/* Bits stored in the ucFlags field of the stream buffer. */
#परिभाषय sb_FLAGS_IS_सन्देशः_बफर          ( ( uint8_t ) 1 ) /* Set if the stream buffer was created as a message buffer, in which case it holds discrete messages rather than a stream. */
#परिभाषय sbFLAGS_IS_STATICALLY_ALLOCATED    ( ( uint8_t ) 2 ) /* Set if the stream buffer was created using statically allocated memory. */

/*-----------------------------------------------------------*/

/* Structure that hold state information on the buffer. */
प्रकारनिर्देश संरचना प्रवाहः_बफर_Def_t                 /*lint !e9058 Style convention uses tag. */
{
    चञ्चल size_t x_पुच्छ;                       /* Index to the next item to read within the buffer. */
    चञ्चल size_t x_शीर्ष;                       /* Index to the next item to write within the buffer. */
    size_t x_दीर्घता;                              /* The length of the buffer pointed to by pucBuffer. */
    size_t x_Trigger_Level_अष्टकानि;                   /* The number of bytes that must be in the stream buffer before a task that is waiting for data is unblocked. */
    चञ्चल TaskHandle_t x_Task_प्रतीक्षा_गन्तव्यम्_Receive; /* Holds the handle of a task waiting for data, or NULL if no tasks are waiting. */
    चञ्चल TaskHandle_t x_Task_प्रतीक्षा_गन्तव्यम्_Send;    /* Holds the handle of a task waiting to send data to a message buffer that is full. */
    uint8_t * puc_बफर;                         /* Points to the buffer itself - that is - the RAM that stores the data passed through the buffer. */
    uint8_t ucFlags;

    #यदि ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_प्रवाहः_बफर_सङ्ख्या; /* Used for tracing purposes. */
    #यदि_अन्तः

    #यदि ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        प्रवाहः_बफर_Callback_Function_t pxSendCompletedCallback;    /* Optional callback called on send complete. sbSEND_COMPLETED is called if this is NULL. */
        प्रवाहः_बफर_Callback_Function_t pxReceiveCompletedCallback; /* Optional callback called on receive complete.  sbRECEIVE_COMPLETED is called if this is NULL. */
    #यदि_अन्तः

    पत्तनम्_MUX_TYPE x_प्रवाहः_बफर_तालकम्; /* Spinlock required for SMP critical sections */
} प्रवाहः_बफर_t;

/*
 * The number of bytes available to be read from the buffer.
 */
स्थिर size_t prv_अष्टकानि_अन्तर्_बफर( ध्रुव प्रवाहः_बफर_t * ध्रुव px_प्रवाहः_बफर ) PRIVILEGED_FUNCTION;

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
स्थिर size_t prv_Write_अष्टकानि_गन्तव्यम्_बफर( प्रवाहः_बफर_t * ध्रुव px_प्रवाहः_बफर,
                                     ध्रुव uint8_t * puc_दत्त,
                                     size_t x_गणनम्,
                                     size_t x_शीर्ष ) PRIVILEGED_FUNCTION;

/*
 * If the stream buffer is being used as a message buffer, then reads an entire
 * message out of the buffer.  If the stream buffer is being used as a stream
 * buffer then read as many bytes as possible from the buffer.
 * prvReadBytesFromBuffer() is called to actually extract the bytes from the
 * buffer's data storage area.
 */
स्थिर size_t prv_Read_सन्देशः_From_बफर( प्रवाहः_बफर_t * px_प्रवाहः_बफर,
                                        शून्य * pv_Rx_दत्त,
                                        size_t x_बफर_दीर्घता_अष्टकानि,
                                        size_t x_अष्टकानि_उपलब्धम् ) PRIVILEGED_FUNCTION;

/*
 * If the stream buffer is being used as a message buffer, then writes an entire
 * message to the buffer.  If the stream buffer is being used as a stream
 * buffer then write as many bytes as possible to the buffer.
 * prvWriteBytestoBuffer() is called to actually send the bytes to the buffer's
 * data storage area.
 */
स्थिर size_t prv_Write_सन्देशः_गन्तव्यम्_बफर( प्रवाहः_बफर_t * ध्रुव px_प्रवाहः_बफर,
                                       ध्रुव शून्य * pv_Tx_दत्त,
                                       size_t x_दत्त_दीर्घता_अष्टकानि,
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
स्थिर size_t prv_Read_अष्टकानि_From_बफर( प्रवाहः_बफर_t * px_प्रवाहः_बफर,
                                      uint8_t * puc_दत्त,
                                      size_t x_गणनम्,
                                      size_t x_पुच्छ ) PRIVILEGED_FUNCTION;

/*
 * Called by both pxStreamBufferCreate() and pxStreamBufferCreateStatic() to
 * initialise the members of the newly created stream buffer structure.
 */
स्थिर शून्य prv_Initialise_New_प्रवाहः_बफर( प्रवाहः_बफर_t * ध्रुव px_प्रवाहः_बफर,
                                          uint8_t * ध्रुव puc_बफर,
                                          size_t x_बफर_आकार_अष्टकानि,
                                          size_t x_Trigger_Level_अष्टकानि,
                                          uint8_t ucFlags,
                                          प्रवाहः_बफर_Callback_Function_t pxSendCompletedCallback,
                                          प्रवाहः_बफर_Callback_Function_t pxReceiveCompletedCallback ) PRIVILEGED_FUNCTION;

/*-----------------------------------------------------------*/
#यदि ( config_SUPPORT_DYNAMIC_आवण्टनम् == 1 )
    प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर_Generic_Create( size_t x_बफर_आकार_अष्टकानि,
                                                     size_t x_Trigger_Level_अष्टकानि,
                                                     आधारः_Type_t x_Is_सन्देशः_बफर,
                                                     प्रवाहः_बफर_Callback_Function_t pxSendCompletedCallback,
                                                     प्रवाहः_बफर_Callback_Function_t pxReceiveCompletedCallback )
    {
        uint8_t * puc_Allocated_स्मृतिः;
        uint8_t ucFlags;

        /* In case the stream buffer is going to be used as a message buffer
         * (that is, it will hold discrete messages with a little meta data that
         * says how big the next message is) check the buffer will be large enough
         * to hold at least one message. */
        यदि( x_Is_सन्देशः_बफर == pdTRUE )
        {
            /* Is a message buffer but not statically allocated. */
            ucFlags = sb_FLAGS_IS_सन्देशः_बफर;
            configASSERT( x_बफर_आकार_अष्टकानि > sb_अष्टकानि_गन्तव्यम्_STORE_सन्देशः_दीर्घता );
        }
        अन्यथा
        {
            /* Not a message buffer and not statically allocated. */
            ucFlags = 0;
            configASSERT( x_बफर_आकार_अष्टकानि > 0 );
        }

        configASSERT( x_Trigger_Level_अष्टकानि <= x_बफर_आकार_अष्टकानि );

        /* A trigger level of 0 would cause a waiting task to unblock even when
         * the buffer was empty. */
        यदि( x_Trigger_Level_अष्टकानि == ( size_t ) 0 )
        {
            x_Trigger_Level_अष्टकानि = ( size_t ) 1;
        }

        /* A stream buffer requires a StreamBuffer_t structure and a buffer.
         * Both are allocated in a single call to pvPortMalloc().  The
         * StreamBuffer_t structure is placed at the start of the allocated memory
         * and the buffer follows immediately after.  The requested size is
         * incremented so the free space is returned as the user would expect -
         * this is a quirk of the implementation that means otherwise the free
         * space would be reported as one byte smaller than would be logically
         * expected. */
        यदि( x_बफर_आकार_अष्टकानि < ( x_बफर_आकार_अष्टकानि + 1 + परिमाण( प्रवाहः_बफर_t ) ) )
        {
            x_बफर_आकार_अष्टकानि++;
            puc_Allocated_स्मृतिः = ( uint8_t * ) pv_पत्तनम्_Malloc( x_बफर_आकार_अष्टकानि + परिमाण( प्रवाहः_बफर_t ) ); /*lint !e9079 malloc() only returns void*. */
        }
        अन्यथा
        {
            puc_Allocated_स्मृतिः = रिक्त;
        }

        यदि( puc_Allocated_स्मृतिः != रिक्त )
        {
            prv_Initialise_New_प्रवाहः_बफर( ( प्रवाहः_बफर_t * ) puc_Allocated_स्मृतिः,       /* Structure at the start of the allocated memory. */ /*lint !e9087 Safe cast as allocated memory is aligned. */ /*lint !e826 Area is not too small and alignment is guaranteed provided malloc() behaves as expected and returns aligned buffer. */
                                          puc_Allocated_स्मृतिः + परिमाण( प्रवाहः_बफर_t ), /* Storage area follows. */ /*lint !e9016 Indexing past structure valid for uint8_t pointer, also storage area has no alignment requirement. */
                                          x_बफर_आकार_अष्टकानि,
                                          x_Trigger_Level_अष्टकानि,
                                          ucFlags,
                                          pxSendCompletedCallback,
                                          pxReceiveCompletedCallback );

            /* Initialize the stream buffer's spinlock separately, as
             * prvInitialiseNewStreamBuffer() is also called from
             * xStreamBufferReset(). */
            पत्तनम्_MUX_INITIALIZE( &( ( ( प्रवाहः_बफर_t * ) puc_Allocated_स्मृतिः )->x_प्रवाहः_बफर_तालकम् ) );

            trace_प्रवाहः_बफर_CREATE( ( ( प्रवाहः_बफर_t * ) puc_Allocated_स्मृतिः ), x_Is_सन्देशः_बफर );
        }
        अन्यथा
        {
            trace_प्रवाहः_बफर_CREATE_FAILED( x_Is_सन्देशः_बफर );
        }

        प्रत्यागम ( प्रवाहः_बफर_Handle_t ) puc_Allocated_स्मृतिः; /*lint !e9087 !e826 Safe cast as allocated memory is aligned. */
    }
#यदि_अन्तः /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

#यदि ( config_SUPPORT_STATIC_आवण्टनम् == 1 )

    प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर_Generic_Create_Static( size_t x_बफर_आकार_अष्टकानि,
                                                           size_t x_Trigger_Level_अष्टकानि,
                                                           आधारः_Type_t x_Is_सन्देशः_बफर,
                                                           uint8_t * ध्रुव puc_प्रवाहः_बफर_Storage_Area,
                                                           Static_प्रवाहः_बफर_t * ध्रुव px_Static_प्रवाहः_बफर,
                                                           प्रवाहः_बफर_Callback_Function_t pxSendCompletedCallback,
                                                           प्रवाहः_बफर_Callback_Function_t pxReceiveCompletedCallback )
    {
        प्रवाहः_बफर_t * ध्रुव px_प्रवाहः_बफर = ( प्रवाहः_बफर_t * ) px_Static_प्रवाहः_बफर; /*lint !e740 !e9087 Safe cast as StaticStreamBuffer_t is opaque Streambuffer_t. */
        प्रवाहः_बफर_Handle_t xReturn;
        uint8_t ucFlags;

        configASSERT( puc_प्रवाहः_बफर_Storage_Area );
        configASSERT( px_Static_प्रवाहः_बफर );
        configASSERT( x_Trigger_Level_अष्टकानि <= x_बफर_आकार_अष्टकानि );

        /* A trigger level of 0 would cause a waiting task to unblock even when
         * the buffer was empty. */
        यदि( x_Trigger_Level_अष्टकानि == ( size_t ) 0 )
        {
            x_Trigger_Level_अष्टकानि = ( size_t ) 1;
        }

        यदि( x_Is_सन्देशः_बफर != pdFALSE )
        {
            /* Statically allocated message buffer. */
            ucFlags = sb_FLAGS_IS_सन्देशः_बफर | sbFLAGS_IS_STATICALLY_ALLOCATED;
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
        configASSERT( x_बफर_आकार_अष्टकानि > sb_अष्टकानि_गन्तव्यम्_STORE_सन्देशः_दीर्घता );

        #यदि ( configASSERT_DEFINED == 1 )
        {
            /* Sanity check that the size of the structure used to declare a
             * variable of type StaticStreamBuffer_t equals the size of the real
             * message buffer structure. */
            चञ्चल size_t x_आकार = परिमाण( Static_प्रवाहः_बफर_t );
            configASSERT( x_आकार == परिमाण( प्रवाहः_बफर_t ) );
            ( शून्य ) x_आकार; /* Prevent unused variable warning when configASSERT() is not used. */
        } /*lint !e529 xSize is referenced is configASSERT() is defined. */
        #यदि_अन्तः /* configASSERT_DEFINED */

        यदि( ( puc_प्रवाहः_बफर_Storage_Area != रिक्त ) && ( px_Static_प्रवाहः_बफर != रिक्त ) )
        {
            prv_Initialise_New_प्रवाहः_बफर( px_प्रवाहः_बफर,
                                          puc_प्रवाहः_बफर_Storage_Area,
                                          x_बफर_आकार_अष्टकानि,
                                          x_Trigger_Level_अष्टकानि,
                                          ucFlags,
                                          pxSendCompletedCallback,
                                          pxReceiveCompletedCallback );

            /* Remember this was statically allocated in case it is ever deleted
             * again. */
            px_प्रवाहः_बफर->ucFlags |= sbFLAGS_IS_STATICALLY_ALLOCATED;

            /* Initialize the stream buffer's spinlock separately, as
             * prvInitialiseNewStreamBuffer() is also called from
             * xStreamBufferReset(). */
            पत्तनम्_MUX_INITIALIZE( &( px_प्रवाहः_बफर->x_प्रवाहः_बफर_तालकम् ) );

            trace_प्रवाहः_बफर_CREATE( px_प्रवाहः_बफर, x_Is_सन्देशः_बफर );

            xReturn = ( प्रवाहः_बफर_Handle_t ) px_Static_प्रवाहः_बफर; /*lint !e9087 Data hiding requires cast to opaque type. */
        }
        अन्यथा
        {
            xReturn = रिक्त;
            trace_प्रवाहः_बफर_CREATE_STATIC_FAILED( xReturn, x_Is_सन्देशः_बफर );
        }

        प्रत्यागम xReturn;
    }
#यदि_अन्तः /* ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*-----------------------------------------------------------*/

#यदि ( config_SUPPORT_STATIC_आवण्टनम् == 1 )
    आधारः_Type_t x_प्रवाहः_बफर_Get_Static_Buffers( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर,
                                              uint8_t ** ppuc_प्रवाहः_बफर_Storage_Area,
                                              Static_प्रवाहः_बफर_t ** ppx_Static_प्रवाहः_बफर )
    {
        आधारः_Type_t xReturn;
        ध्रुव प्रवाहः_बफर_t * ध्रुव px_प्रवाहः_बफर = x_प्रवाहः_बफर;

        configASSERT( px_प्रवाहः_बफर );
        configASSERT( ppuc_प्रवाहः_बफर_Storage_Area );
        configASSERT( ppx_Static_प्रवाहः_बफर );

        यदि( ( px_प्रवाहः_बफर->ucFlags & sbFLAGS_IS_STATICALLY_ALLOCATED ) != ( uint8_t ) 0 )
        {
            *ppuc_प्रवाहः_बफर_Storage_Area = px_प्रवाहः_बफर->puc_बफर;
            *ppx_Static_प्रवाहः_बफर = ( Static_प्रवाहः_बफर_t * ) px_प्रवाहः_बफर;
            xReturn = pdTRUE;
        }
        अन्यथा
        {
            xReturn = pdFALSE;
        }

        प्रत्यागम xReturn;
    }
#यदि_अन्तः /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

शून्य v_प्रवाहः_बफर_Delete( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर )
{
    प्रवाहः_बफर_t * px_प्रवाहः_बफर = x_प्रवाहः_बफर;

    configASSERT( px_प्रवाहः_बफर );

    trace_प्रवाहः_बफर_DELETE( x_प्रवाहः_बफर );

    यदि( ( px_प्रवाहः_बफर->ucFlags & sbFLAGS_IS_STATICALLY_ALLOCATED ) == ( uint8_t ) pdFALSE )
    {
        #यदि ( config_SUPPORT_DYNAMIC_आवण्टनम् == 1 )
        {
            /* Both the structure and the buffer were allocated using a single call
            * to pvPortMalloc(), hence only one call to vPortFree() is required. */
            v_पत्तनम्_Free( ( शून्य * ) px_प्रवाहः_बफर ); /*lint !e9087 Standard free() semantics require void *, plus pxStreamBuffer was allocated by pvPortMalloc(). */
        }
        #अन्यथा
        {
            /* Should not be possible to get here, ucFlags must be corrupt.
             * Force an assert. */
            configASSERT( x_प्रवाहः_बफर == ( प्रवाहः_बफर_Handle_t ) ~0 );
        }
        #यदि_अन्तः
    }
    अन्यथा
    {
        /* The structure and buffer were not allocated dynamically and cannot be
         * freed - just scrub the structure so future use will assert. */
        ( शून्य ) स्मृतिं_पूरय( px_प्रवाहः_बफर, 0x00, परिमाण( प्रवाहः_बफर_t ) );
    }
}
/*-----------------------------------------------------------*/

आधारः_Type_t x_प्रवाहः_बफर_Reset( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर )
{
    प्रवाहः_बफर_t * ध्रुव px_प्रवाहः_बफर = x_प्रवाहः_बफर;
    आधारः_Type_t xReturn = pdFAIL;
    प्रवाहः_बफर_Callback_Function_t pxSendCallback = रिक्त, pxReceiveCallback = रिक्त;

    #यदि ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_प्रवाहः_बफर_सङ्ख्या;
    #यदि_अन्तः

    configASSERT( px_प्रवाहः_बफर );

    #यदि ( configUSE_TRACE_FACILITY == 1 )
    {
        /* Store the stream buffer number so it can be restored after the
         * reset. */
        ux_प्रवाहः_बफर_सङ्ख्या = px_प्रवाहः_बफर->ux_प्रवाहः_बफर_सङ्ख्या;
    }
    #यदि_अन्तः

    /* Can only reset a message buffer if there are no tasks blocked on it. */
    taskENTER_CRITICAL( &( px_प्रवाहः_बफर->x_प्रवाहः_बफर_तालकम् ) );
    {
        यदि( ( px_प्रवाहः_बफर->x_Task_प्रतीक्षा_गन्तव्यम्_Receive == रिक्त ) && ( px_प्रवाहः_बफर->x_Task_प्रतीक्षा_गन्तव्यम्_Send == रिक्त ) )
        {
            #यदि ( configUSE_SB_COMPLETED_CALLBACK == 1 )
            {
                pxSendCallback = px_प्रवाहः_बफर->pxSendCompletedCallback;
                pxReceiveCallback = px_प्रवाहः_बफर->pxReceiveCompletedCallback;
            }
            #यदि_अन्तः

            prv_Initialise_New_प्रवाहः_बफर( px_प्रवाहः_बफर,
                                          px_प्रवाहः_बफर->puc_बफर,
                                          px_प्रवाहः_बफर->x_दीर्घता,
                                          px_प्रवाहः_बफर->x_Trigger_Level_अष्टकानि,
                                          px_प्रवाहः_बफर->ucFlags,
                                          pxSendCallback,
                                          pxReceiveCallback );

            #यदि ( configUSE_TRACE_FACILITY == 1 )
            {
                px_प्रवाहः_बफर->ux_प्रवाहः_बफर_सङ्ख्या = ux_प्रवाहः_बफर_सङ्ख्या;
            }
            #यदि_अन्तः

            trace_प्रवाहः_बफर_RESET( x_प्रवाहः_बफर );

            xReturn = pdPASS;
        }
    }
    taskEXIT_CRITICAL( &( px_प्रवाहः_बफर->x_प्रवाहः_बफर_तालकम् ) );

    प्रत्यागम xReturn;
}
/*-----------------------------------------------------------*/

आधारः_Type_t x_प्रवाहः_बफर_समुच्चयः_Trigger_Level( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर,
                                         size_t xTriggerLevel )
{
    प्रवाहः_बफर_t * ध्रुव px_प्रवाहः_बफर = x_प्रवाहः_बफर;
    आधारः_Type_t xReturn;

    configASSERT( px_प्रवाहः_बफर );

    /* It is not valid for the trigger level to be 0. */
    यदि( xTriggerLevel == ( size_t ) 0 )
    {
        xTriggerLevel = ( size_t ) 1;
    }

    /* The trigger level is the number of bytes that must be in the stream
     * buffer before a task that is waiting for data is unblocked. */
    यदि( xTriggerLevel < px_प्रवाहः_बफर->x_दीर्घता )
    {
        px_प्रवाहः_बफर->x_Trigger_Level_अष्टकानि = xTriggerLevel;
        xReturn = pdPASS;
    }
    अन्यथा
    {
        xReturn = pdFALSE;
    }

    प्रत्यागम xReturn;
}
/*-----------------------------------------------------------*/

size_t x_प्रवाहः_बफर_Spaces_उपलब्धम्( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर )
{
    ध्रुव प्रवाहः_बफर_t * ध्रुव px_प्रवाहः_बफर = x_प्रवाहः_बफर;
    size_t xSpace;
    size_t x_Original_पुच्छ;

    configASSERT( px_प्रवाहः_बफर );

    /* The code below reads xTail and then xHead.  This is safe if the stream
     * buffer is updated once between the two reads - but not if the stream buffer
     * is updated more than once between the two reads - hence the loop. */
    कुरु
    {
        x_Original_पुच्छ = px_प्रवाहः_बफर->x_पुच्छ;
        xSpace = px_प्रवाहः_बफर->x_दीर्घता + px_प्रवाहः_बफर->x_पुच्छ;
        xSpace -= px_प्रवाहः_बफर->x_शीर्ष;
    } यावत्( x_Original_पुच्छ != px_प्रवाहः_बफर->x_पुच्छ );

    xSpace -= ( size_t ) 1;

    यदि( xSpace >= px_प्रवाहः_बफर->x_दीर्घता )
    {
        xSpace -= px_प्रवाहः_बफर->x_दीर्घता;
    }
    अन्यथा
    {
        mtCOVERAGE_TEST_MARKER();
    }

    प्रत्यागम xSpace;
}
/*-----------------------------------------------------------*/

size_t x_प्रवाहः_बफर_अष्टकानि_उपलब्धम्( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर )
{
    ध्रुव प्रवाहः_बफर_t * ध्रुव px_प्रवाहः_बफर = x_प्रवाहः_बफर;
    size_t xReturn;

    configASSERT( px_प्रवाहः_बफर );

    xReturn = prv_अष्टकानि_अन्तर्_बफर( px_प्रवाहः_बफर );
    प्रत्यागम xReturn;
}
/*-----------------------------------------------------------*/

size_t x_प्रवाहः_बफर_Send( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर,
                          ध्रुव शून्य * pv_Tx_दत्त,
                          size_t x_दत्त_दीर्घता_अष्टकानि,
                          TickType_t x_Ticks_गन्तव्यम्_Wait )
{
    प्रवाहः_बफर_t * ध्रुव px_प्रवाहः_बफर = x_प्रवाहः_बफर;
    size_t xReturn, xSpace = 0;
    size_t xRequiredSpace = x_दत्त_दीर्घता_अष्टकानि;
    Time_बहिर्गमनम्_t x_Time_बहिर्गमनम्;
    size_t xMaxReportedSpace = 0;

    configASSERT( pv_Tx_दत्त );
    configASSERT( px_प्रवाहः_बफर );

    /* The maximum amount of space a stream buffer will ever report is its length
     * minus 1. */
    xMaxReportedSpace = px_प्रवाहः_बफर->x_दीर्घता - ( size_t ) 1;

    /* This send function is used to write to both message buffers and stream
     * buffers.  If this is a message buffer then the space needed must be
     * increased by the amount of bytes needed to store the length of the
     * message. */
    यदि( ( px_प्रवाहः_बफर->ucFlags & sb_FLAGS_IS_सन्देशः_बफर ) != ( uint8_t ) 0 )
    {
        xRequiredSpace += sb_अष्टकानि_गन्तव्यम्_STORE_सन्देशः_दीर्घता;

        /* Overflow? */
        configASSERT( xRequiredSpace > x_दत्त_दीर्घता_अष्टकानि );

        /* If this is a message buffer then it must be possible to write the
         * whole message. */
        यदि( xRequiredSpace > xMaxReportedSpace )
        {
            /* The message would not fit even if the entire buffer was empty,
             * so don't wait for space. */
            x_Ticks_गन्तव्यम्_Wait = ( TickType_t ) 0;
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
        यदि( xRequiredSpace > xMaxReportedSpace )
        {
            xRequiredSpace = xMaxReportedSpace;
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }

    यदि( x_Ticks_गन्तव्यम्_Wait != ( TickType_t ) 0 )
    {
        v_Task_समुच्चयः_Time_बहिर्गमनम्_State( &x_Time_बहिर्गमनम् );

        कुरु
        {
            /* Wait until the required number of bytes are free in the message
             * buffer. */
            taskENTER_CRITICAL( &( px_प्रवाहः_बफर->x_प्रवाहः_बफर_तालकम् ) );
            {
                xSpace = x_प्रवाहः_बफर_Spaces_उपलब्धम्( px_प्रवाहः_बफर );

                यदि( xSpace < xRequiredSpace )
                {
                    /* Clear notification state as going to wait for space. */
                    ( शून्य ) xTaskNotifyStateClear( रिक्त );

                    /* Should only be one writer. */
                    configASSERT( px_प्रवाहः_बफर->x_Task_प्रतीक्षा_गन्तव्यम्_Send == रिक्त );
                    px_प्रवाहः_बफर->x_Task_प्रतीक्षा_गन्तव्यम्_Send = xTaskGetCurrentTaskHandle();
                }
                अन्यथा
                {
                    taskEXIT_CRITICAL( &( px_प्रवाहः_बफर->x_प्रवाहः_बफर_तालकम् ) );
                    भङ्ग;
                }
            }
            taskEXIT_CRITICAL( &( px_प्रवाहः_बफर->x_प्रवाहः_बफर_तालकम् ) );

            trace_BLOCKING_ON_प्रवाहः_बफर_SEND( x_प्रवाहः_बफर );
            ( शून्य ) xTaskNotifyWait( ( uint32_t ) 0, ( uint32_t ) 0, रिक्त, x_Ticks_गन्तव्यम्_Wait );
            /* In SMP mode, the task may have been woken and scheduled on
             * another core. Hence, we must clear the xTaskWaitingToSend
             * handle in a critical section. */
            #यदि ( config_सङ्ख्या_सम्बन्धि_CORES > 1 )
                taskENTER_CRITICAL( &( px_प्रवाहः_बफर->x_प्रवाहः_बफर_तालकम् ) );
            #यदि_अन्तः /* configNUMBER_OF_CORES > 1 */
            {
                px_प्रवाहः_बफर->x_Task_प्रतीक्षा_गन्तव्यम्_Send = रिक्त;
            }
            #यदि ( config_सङ्ख्या_सम्बन्धि_CORES > 1 )
                taskEXIT_CRITICAL( &( px_प्रवाहः_बफर->x_प्रवाहः_बफर_तालकम् ) );
            #यदि_अन्तः /* configNUMBER_OF_CORES > 1 */
        } यावत्( x_Task_Check_For_Time_बहिर्गमनम्( &x_Time_बहिर्गमनम्, &x_Ticks_गन्तव्यम्_Wait ) == pdFALSE );
    }
    अन्यथा
    {
        mtCOVERAGE_TEST_MARKER();
    }

    यदि( xSpace == ( size_t ) 0 )
    {
        xSpace = x_प्रवाहः_बफर_Spaces_उपलब्धम्( px_प्रवाहः_बफर );
    }
    अन्यथा
    {
        mtCOVERAGE_TEST_MARKER();
    }

    xReturn = prv_Write_सन्देशः_गन्तव्यम्_बफर( px_प्रवाहः_बफर, pv_Tx_दत्त, x_दत्त_दीर्घता_अष्टकानि, xSpace, xRequiredSpace );

    यदि( xReturn > ( size_t ) 0 )
    {
        trace_प्रवाहः_बफर_SEND( x_प्रवाहः_बफर, xReturn );

        /* Was a task waiting for the data? */
        यदि( prv_अष्टकानि_अन्तर्_बफर( px_प्रवाहः_बफर ) >= px_प्रवाहः_बफर->x_Trigger_Level_अष्टकानि )
        {
            prvSEND_COMPLETED( px_प्रवाहः_बफर );
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    अन्यथा
    {
        mtCOVERAGE_TEST_MARKER();
        trace_प्रवाहः_बफर_SEND_FAILED( x_प्रवाहः_बफर );
    }

    प्रत्यागम xReturn;
}
/*-----------------------------------------------------------*/

size_t x_प्रवाहः_बफर_Send_From_ISR( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर,
                                 ध्रुव शून्य * pv_Tx_दत्त,
                                 size_t x_दत्त_दीर्घता_अष्टकानि,
                                 आधारः_Type_t * ध्रुव px_Higher_प्राथमिकता_Task_Woken )
{
    प्रवाहः_बफर_t * ध्रुव px_प्रवाहः_बफर = x_प्रवाहः_बफर;
    size_t xReturn, xSpace;
    size_t xRequiredSpace = x_दत्त_दीर्घता_अष्टकानि;

    configASSERT( pv_Tx_दत्त );
    configASSERT( px_प्रवाहः_बफर );

    /* This send function is used to write to both message buffers and stream
     * buffers.  If this is a message buffer then the space needed must be
     * increased by the amount of bytes needed to store the length of the
     * message. */
    यदि( ( px_प्रवाहः_बफर->ucFlags & sb_FLAGS_IS_सन्देशः_बफर ) != ( uint8_t ) 0 )
    {
        xRequiredSpace += sb_अष्टकानि_गन्तव्यम्_STORE_सन्देशः_दीर्घता;
    }
    अन्यथा
    {
        mtCOVERAGE_TEST_MARKER();
    }

    xSpace = x_प्रवाहः_बफर_Spaces_उपलब्धम्( px_प्रवाहः_बफर );
    xReturn = prv_Write_सन्देशः_गन्तव्यम्_बफर( px_प्रवाहः_बफर, pv_Tx_दत्त, x_दत्त_दीर्घता_अष्टकानि, xSpace, xRequiredSpace );

    यदि( xReturn > ( size_t ) 0 )
    {
        /* Was a task waiting for the data? */
        यदि( prv_अष्टकानि_अन्तर्_बफर( px_प्रवाहः_बफर ) >= px_प्रवाहः_बफर->x_Trigger_Level_अष्टकानि )
        {
            prvSEND_COMPLETE_FROM_ISR( px_प्रवाहः_बफर, px_Higher_प्राथमिकता_Task_Woken );
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

    trace_प्रवाहः_बफर_SEND_FROM_ISR( x_प्रवाहः_बफर, xReturn );

    प्रत्यागम xReturn;
}
/*-----------------------------------------------------------*/

स्थिर size_t prv_Write_सन्देशः_गन्तव्यम्_बफर( प्रवाहः_बफर_t * ध्रुव px_प्रवाहः_बफर,
                                       ध्रुव शून्य * pv_Tx_दत्त,
                                       size_t x_दत्त_दीर्घता_अष्टकानि,
                                       size_t xSpace,
                                       size_t xRequiredSpace )
{
    size_t x_अग्र_शीर्ष = px_प्रवाहः_बफर->x_शीर्ष;
    config_सन्देशः_बफर_दीर्घता_TYPE x_सन्देशः_दीर्घता;

    यदि( ( px_प्रवाहः_बफर->ucFlags & sb_FLAGS_IS_सन्देशः_बफर ) != ( uint8_t ) 0 )
    {
        /* This is a message buffer, as opposed to a stream buffer. */

        /* Convert xDataLengthBytes to the message length type. */
        x_सन्देशः_दीर्घता = ( config_सन्देशः_बफर_दीर्घता_TYPE ) x_दत्त_दीर्घता_अष्टकानि;

        /* Ensure the data length given fits within configMESSAGE_BUFFER_LENGTH_TYPE. */
        configASSERT( ( size_t ) x_सन्देशः_दीर्घता == x_दत्त_दीर्घता_अष्टकानि );

        यदि( xSpace >= xRequiredSpace )
        {
            /* There is enough space to write both the message length and the message
             * itself into the buffer.  Start by writing the length of the data, the data
             * itself will be written later in this function. */
            x_अग्र_शीर्ष = prv_Write_अष्टकानि_गन्तव्यम्_बफर( px_प्रवाहः_बफर, ( ध्रुव uint8_t * ) &( x_सन्देशः_दीर्घता ), sb_अष्टकानि_गन्तव्यम्_STORE_सन्देशः_दीर्घता, x_अग्र_शीर्ष );
        }
        अन्यथा
        {
            /* Not enough space, so do not write data to the buffer. */
            x_दत्त_दीर्घता_अष्टकानि = 0;
        }
    }
    अन्यथा
    {
        /* This is a stream buffer, as opposed to a message buffer, so writing a
         * stream of bytes rather than discrete messages.  Plan to write as many
         * bytes as possible. */
        x_दत्त_दीर्घता_अष्टकानि = configMIN( x_दत्त_दीर्घता_अष्टकानि, xSpace );
    }

    यदि( x_दत्त_दीर्घता_अष्टकानि != ( size_t ) 0 )
    {
        /* Write the data to the buffer. */
        px_प्रवाहः_बफर->x_शीर्ष = prv_Write_अष्टकानि_गन्तव्यम्_बफर( px_प्रवाहः_बफर, ( ध्रुव uint8_t * ) pv_Tx_दत्त, x_दत्त_दीर्घता_अष्टकानि, x_अग्र_शीर्ष ); /*lint !e9079 Storage buffer is implemented as uint8_t for ease of sizing, alignment and access. */
    }

    प्रत्यागम x_दत्त_दीर्घता_अष्टकानि;
}
/*-----------------------------------------------------------*/

size_t x_प्रवाहः_बफर_Receive( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर,
                             शून्य * pv_Rx_दत्त,
                             size_t x_बफर_दीर्घता_अष्टकानि,
                             TickType_t x_Ticks_गन्तव्यम्_Wait )
{
    प्रवाहः_बफर_t * ध्रुव px_प्रवाहः_बफर = x_प्रवाहः_बफर;
    size_t x_Received_दीर्घता = 0, x_अष्टकानि_उपलब्धम्, x_अष्टकानि_गन्तव्यम्_Store_सन्देशः_दीर्घता;

    configASSERT( pv_Rx_दत्त );
    configASSERT( px_प्रवाहः_बफर );

    /* This receive function is used by both message buffers, which store
     * discrete messages, and stream buffers, which store a continuous stream of
     * bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the
     * message. */
    यदि( ( px_प्रवाहः_बफर->ucFlags & sb_FLAGS_IS_सन्देशः_बफर ) != ( uint8_t ) 0 )
    {
        x_अष्टकानि_गन्तव्यम्_Store_सन्देशः_दीर्घता = sb_अष्टकानि_गन्तव्यम्_STORE_सन्देशः_दीर्घता;
    }
    अन्यथा
    {
        x_अष्टकानि_गन्तव्यम्_Store_सन्देशः_दीर्घता = 0;
    }

    यदि( x_Ticks_गन्तव्यम्_Wait != ( TickType_t ) 0 )
    {
        /* Checking if there is data and clearing the notification state must be
         * performed atomically. */
        taskENTER_CRITICAL( &( px_प्रवाहः_बफर->x_प्रवाहः_बफर_तालकम् ) );
        {
            x_अष्टकानि_उपलब्धम् = prv_अष्टकानि_अन्तर्_बफर( px_प्रवाहः_बफर );

            /* If this function was invoked by a message buffer read then
             * xBytesToStoreMessageLength holds the number of bytes used to hold
             * the length of the next discrete message.  If this function was
             * invoked by a stream buffer read then xBytesToStoreMessageLength will
             * be 0. */
            यदि( x_अष्टकानि_उपलब्धम् <= x_अष्टकानि_गन्तव्यम्_Store_सन्देशः_दीर्घता )
            {
                /* Clear notification state as going to wait for data. */
                ( शून्य ) xTaskNotifyStateClear( रिक्त );

                /* Should only be one reader. */
                configASSERT( px_प्रवाहः_बफर->x_Task_प्रतीक्षा_गन्तव्यम्_Receive == रिक्त );
                px_प्रवाहः_बफर->x_Task_प्रतीक्षा_गन्तव्यम्_Receive = xTaskGetCurrentTaskHandle();
            }
            अन्यथा
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        taskEXIT_CRITICAL( &( px_प्रवाहः_बफर->x_प्रवाहः_बफर_तालकम् ) );

        यदि( x_अष्टकानि_उपलब्धम् <= x_अष्टकानि_गन्तव्यम्_Store_सन्देशः_दीर्घता )
        {
            /* Wait for data to be available. */
            trace_BLOCKING_ON_प्रवाहः_बफर_RECEIVE( x_प्रवाहः_बफर );
            ( शून्य ) xTaskNotifyWait( ( uint32_t ) 0, ( uint32_t ) 0, रिक्त, x_Ticks_गन्तव्यम्_Wait );

            /* In SMP mode, the task may have been woken and scheduled on
             * another core. Hence, we must clear the xTaskWaitingToReceive
             * handle in a critical section. */
            #यदि ( config_सङ्ख्या_सम्बन्धि_CORES > 1 )
                taskENTER_CRITICAL( &( px_प्रवाहः_बफर->x_प्रवाहः_बफर_तालकम् ) );
            #यदि_अन्तः /* configNUMBER_OF_CORES > 1 */
            {
                px_प्रवाहः_बफर->x_Task_प्रतीक्षा_गन्तव्यम्_Receive = रिक्त;
            }
            #यदि ( config_सङ्ख्या_सम्बन्धि_CORES > 1 )
                taskEXIT_CRITICAL( &( px_प्रवाहः_बफर->x_प्रवाहः_बफर_तालकम् ) );
            #यदि_अन्तः /* configNUMBER_OF_CORES > 1 */

            /* Recheck the data available after blocking. */
            x_अष्टकानि_उपलब्धम् = prv_अष्टकानि_अन्तर्_बफर( px_प्रवाहः_बफर );
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    अन्यथा
    {
        x_अष्टकानि_उपलब्धम् = prv_अष्टकानि_अन्तर्_बफर( px_प्रवाहः_बफर );
    }

    /* Whether receiving a discrete message (where xBytesToStoreMessageLength
     * holds the number of bytes used to store the message length) or a stream of
     * bytes (where xBytesToStoreMessageLength is zero), the number of bytes
     * available must be greater than xBytesToStoreMessageLength to be able to
     * read bytes from the buffer. */
    यदि( x_अष्टकानि_उपलब्धम् > x_अष्टकानि_गन्तव्यम्_Store_सन्देशः_दीर्घता )
    {
        x_Received_दीर्घता = prv_Read_सन्देशः_From_बफर( px_प्रवाहः_बफर, pv_Rx_दत्त, x_बफर_दीर्घता_अष्टकानि, x_अष्टकानि_उपलब्धम् );

        /* Was a task waiting for space in the buffer? */
        यदि( x_Received_दीर्घता != ( size_t ) 0 )
        {
            trace_प्रवाहः_बफर_RECEIVE( x_प्रवाहः_बफर, x_Received_दीर्घता );
            prvRECEIVE_COMPLETED( x_प्रवाहः_बफर );
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    अन्यथा
    {
        trace_प्रवाहः_बफर_RECEIVE_FAILED( x_प्रवाहः_बफर );
        mtCOVERAGE_TEST_MARKER();
    }

    प्रत्यागम x_Received_दीर्घता;
}
/*-----------------------------------------------------------*/

size_t x_प्रवाहः_बफर_अग्र_सन्देशः_दीर्घता_अष्टकानि( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर )
{
    प्रवाहः_बफर_t * ध्रुव px_प्रवाहः_बफर = x_प्रवाहः_बफर;
    size_t xReturn, x_अष्टकानि_उपलब्धम्;
    config_सन्देशः_बफर_दीर्घता_TYPE x_अस्थायी_Return;

    configASSERT( px_प्रवाहः_बफर );

    /* Ensure the stream buffer is being used as a message buffer. */
    यदि( ( px_प्रवाहः_बफर->ucFlags & sb_FLAGS_IS_सन्देशः_बफर ) != ( uint8_t ) 0 )
    {
        x_अष्टकानि_उपलब्धम् = prv_अष्टकानि_अन्तर्_बफर( px_प्रवाहः_बफर );

        यदि( x_अष्टकानि_उपलब्धम् > sb_अष्टकानि_गन्तव्यम्_STORE_सन्देशः_दीर्घता )
        {
            /* The number of bytes available is greater than the number of bytes
             * required to hold the length of the next message, so another message
             * is available. */
            ( शून्य ) prv_Read_अष्टकानि_From_बफर( px_प्रवाहः_बफर, ( uint8_t * ) &x_अस्थायी_Return, sb_अष्टकानि_गन्तव्यम्_STORE_सन्देशः_दीर्घता, px_प्रवाहः_बफर->x_पुच्छ );
            xReturn = ( size_t ) x_अस्थायी_Return;
        }
        अन्यथा
        {
            /* The minimum amount of bytes in a message buffer is
             * ( sbBYTES_TO_STORE_MESSAGE_LENGTH + 1 ), so if xBytesAvailable is
             * less than sbBYTES_TO_STORE_MESSAGE_LENGTH the only other valid
             * value is 0. */
            configASSERT( x_अष्टकानि_उपलब्धम् == 0 );
            xReturn = 0;
        }
    }
    अन्यथा
    {
        xReturn = 0;
    }

    प्रत्यागम xReturn;
}
/*-----------------------------------------------------------*/

size_t x_प्रवाहः_बफर_Receive_From_ISR( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर,
                                    शून्य * pv_Rx_दत्त,
                                    size_t x_बफर_दीर्घता_अष्टकानि,
                                    आधारः_Type_t * ध्रुव px_Higher_प्राथमिकता_Task_Woken )
{
    प्रवाहः_बफर_t * ध्रुव px_प्रवाहः_बफर = x_प्रवाहः_बफर;
    size_t x_Received_दीर्घता = 0, x_अष्टकानि_उपलब्धम्, x_अष्टकानि_गन्तव्यम्_Store_सन्देशः_दीर्घता;

    configASSERT( pv_Rx_दत्त );
    configASSERT( px_प्रवाहः_बफर );

    /* This receive function is used by both message buffers, which store
     * discrete messages, and stream buffers, which store a continuous stream of
     * bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the
     * message. */
    यदि( ( px_प्रवाहः_बफर->ucFlags & sb_FLAGS_IS_सन्देशः_बफर ) != ( uint8_t ) 0 )
    {
        x_अष्टकानि_गन्तव्यम्_Store_सन्देशः_दीर्घता = sb_अष्टकानि_गन्तव्यम्_STORE_सन्देशः_दीर्घता;
    }
    अन्यथा
    {
        x_अष्टकानि_गन्तव्यम्_Store_सन्देशः_दीर्घता = 0;
    }

    x_अष्टकानि_उपलब्धम् = prv_अष्टकानि_अन्तर्_बफर( px_प्रवाहः_बफर );

    /* Whether receiving a discrete message (where xBytesToStoreMessageLength
     * holds the number of bytes used to store the message length) or a stream of
     * bytes (where xBytesToStoreMessageLength is zero), the number of bytes
     * available must be greater than xBytesToStoreMessageLength to be able to
     * read bytes from the buffer. */
    यदि( x_अष्टकानि_उपलब्धम् > x_अष्टकानि_गन्तव्यम्_Store_सन्देशः_दीर्घता )
    {
        x_Received_दीर्घता = prv_Read_सन्देशः_From_बफर( px_प्रवाहः_बफर, pv_Rx_दत्त, x_बफर_दीर्घता_अष्टकानि, x_अष्टकानि_उपलब्धम् );

        /* Was a task waiting for space in the buffer? */
        यदि( x_Received_दीर्घता != ( size_t ) 0 )
        {
            prvRECEIVE_COMPLETED_FROM_ISR( px_प्रवाहः_बफर, px_Higher_प्राथमिकता_Task_Woken );
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

    trace_प्रवाहः_बफर_RECEIVE_FROM_ISR( x_प्रवाहः_बफर, x_Received_दीर्घता );

    प्रत्यागम x_Received_दीर्घता;
}
/*-----------------------------------------------------------*/

स्थिर size_t prv_Read_सन्देशः_From_बफर( प्रवाहः_बफर_t * px_प्रवाहः_बफर,
                                        शून्य * pv_Rx_दत्त,
                                        size_t x_बफर_दीर्घता_अष्टकानि,
                                        size_t x_अष्टकानि_उपलब्धम् )
{
    size_t x_गणनम्, x_अग्र_सन्देशः_दीर्घता;
    config_सन्देशः_बफर_दीर्घता_TYPE x_अस्थायी_अग्र_सन्देशः_दीर्घता;
    size_t x_अग्र_पुच्छ = px_प्रवाहः_बफर->x_पुच्छ;

    यदि( ( px_प्रवाहः_बफर->ucFlags & sb_FLAGS_IS_सन्देशः_बफर ) != ( uint8_t ) 0 )
    {
        /* A discrete message is being received.  First receive the length
         * of the message. */
        x_अग्र_पुच्छ = prv_Read_अष्टकानि_From_बफर( px_प्रवाहः_बफर, ( uint8_t * ) &x_अस्थायी_अग्र_सन्देशः_दीर्घता, sb_अष्टकानि_गन्तव्यम्_STORE_सन्देशः_दीर्घता, x_अग्र_पुच्छ );
        x_अग्र_सन्देशः_दीर्घता = ( size_t ) x_अस्थायी_अग्र_सन्देशः_दीर्घता;

        /* Reduce the number of bytes available by the number of bytes just
         * read out. */
        x_अष्टकानि_उपलब्धम् -= sb_अष्टकानि_गन्तव्यम्_STORE_सन्देशः_दीर्घता;

        /* Check there is enough space in the buffer provided by the
         * user. */
        यदि( x_अग्र_सन्देशः_दीर्घता > x_बफर_दीर्घता_अष्टकानि )
        {
            /* The user has provided insufficient space to read the message. */
            x_अग्र_सन्देशः_दीर्घता = 0;
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
        x_अग्र_सन्देशः_दीर्घता = x_बफर_दीर्घता_अष्टकानि;
    }

    /* Use the minimum of the wanted bytes and the available bytes. */
    x_गणनम् = configMIN( x_अग्र_सन्देशः_दीर्घता, x_अष्टकानि_उपलब्धम् );

    यदि( x_गणनम् != ( size_t ) 0 )
    {
        /* Read the actual data and update the tail to mark the data as officially consumed. */
        px_प्रवाहः_बफर->x_पुच्छ = prv_Read_अष्टकानि_From_बफर( px_प्रवाहः_बफर, ( uint8_t * ) pv_Rx_दत्त, x_गणनम्, x_अग्र_पुच्छ ); /*lint !e9079 Data storage area is implemented as uint8_t array for ease of sizing, indexing and alignment. */
    }

    प्रत्यागम x_गणनम्;
}
/*-----------------------------------------------------------*/

आधारः_Type_t x_प्रवाहः_बफर_Is_रिक्तानि( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर )
{
    ध्रुव प्रवाहः_बफर_t * ध्रुव px_प्रवाहः_बफर = x_प्रवाहः_बफर;
    आधारः_Type_t xReturn;
    size_t x_पुच्छ;

    configASSERT( px_प्रवाहः_बफर );

    /* True if no bytes are available. */
    x_पुच्छ = px_प्रवाहः_बफर->x_पुच्छ;

    यदि( px_प्रवाहः_बफर->x_शीर्ष == x_पुच्छ )
    {
        xReturn = pdTRUE;
    }
    अन्यथा
    {
        xReturn = pdFALSE;
    }

    प्रत्यागम xReturn;
}
/*-----------------------------------------------------------*/

आधारः_Type_t x_प्रवाहः_बफर_Is_पूर्णम्( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर )
{
    आधारः_Type_t xReturn;
    size_t x_अष्टकानि_गन्तव्यम्_Store_सन्देशः_दीर्घता;
    ध्रुव प्रवाहः_बफर_t * ध्रुव px_प्रवाहः_बफर = x_प्रवाहः_बफर;

    configASSERT( px_प्रवाहः_बफर );

    /* This generic version of the receive function is used by both message
     * buffers, which store discrete messages, and stream buffers, which store a
     * continuous stream of bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the message. */
    यदि( ( px_प्रवाहः_बफर->ucFlags & sb_FLAGS_IS_सन्देशः_बफर ) != ( uint8_t ) 0 )
    {
        x_अष्टकानि_गन्तव्यम्_Store_सन्देशः_दीर्घता = sb_अष्टकानि_गन्तव्यम्_STORE_सन्देशः_दीर्घता;
    }
    अन्यथा
    {
        x_अष्टकानि_गन्तव्यम्_Store_सन्देशः_दीर्घता = 0;
    }

    /* True if the available space equals zero. */
    यदि( x_प्रवाहः_बफर_Spaces_उपलब्धम्( x_प्रवाहः_बफर ) <= x_अष्टकानि_गन्तव्यम्_Store_सन्देशः_दीर्घता )
    {
        xReturn = pdTRUE;
    }
    अन्यथा
    {
        xReturn = pdFALSE;
    }

    प्रत्यागम xReturn;
}
/*-----------------------------------------------------------*/

आधारः_Type_t x_प्रवाहः_बफर_Send_Completed_From_ISR( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर,
                                              आधारः_Type_t * px_Higher_प्राथमिकता_Task_Woken )
{
    प्रवाहः_बफर_t * ध्रुव px_प्रवाहः_बफर = x_प्रवाहः_बफर;
    आधारः_Type_t xReturn;
    UBaseType_t ux_Saved_व्यवधानम्_स्थितिः;

    configASSERT( px_प्रवाहः_बफर );

    ux_Saved_व्यवधानम्_स्थितिः = ( UBaseType_t ) पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्_FROM_ISR();
    {
        यदि( ( px_प्रवाहः_बफर )->x_Task_प्रतीक्षा_गन्तव्यम्_Receive != रिक्त )
        {
            ( शून्य ) xTaskNotifyFromISR( ( px_प्रवाहः_बफर )->x_Task_प्रतीक्षा_गन्तव्यम्_Receive,
                                         ( uint32_t ) 0,
                                         eNoAction,
                                         px_Higher_प्राथमिकता_Task_Woken );
            ( px_प्रवाहः_बफर )->x_Task_प्रतीक्षा_गन्तव्यम्_Receive = रिक्त;
            xReturn = pdTRUE;
        }
        अन्यथा
        {
            xReturn = pdFALSE;
        }
    }
    पत्तनम्_CLEAR_व्यवधानम्_आवरणम्_FROM_ISR( ux_Saved_व्यवधानम्_स्थितिः );

    प्रत्यागम xReturn;
}
/*-----------------------------------------------------------*/

आधारः_Type_t x_प्रवाहः_बफर_Receive_Completed_From_ISR( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर,
                                                 आधारः_Type_t * px_Higher_प्राथमिकता_Task_Woken )
{
    प्रवाहः_बफर_t * ध्रुव px_प्रवाहः_बफर = x_प्रवाहः_बफर;
    आधारः_Type_t xReturn;
    UBaseType_t ux_Saved_व्यवधानम्_स्थितिः;

    configASSERT( px_प्रवाहः_बफर );

    ux_Saved_व्यवधानम्_स्थितिः = ( UBaseType_t ) पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्_FROM_ISR();
    {
        यदि( ( px_प्रवाहः_बफर )->x_Task_प्रतीक्षा_गन्तव्यम्_Send != रिक्त )
        {
            ( शून्य ) xTaskNotifyFromISR( ( px_प्रवाहः_बफर )->x_Task_प्रतीक्षा_गन्तव्यम्_Send,
                                         ( uint32_t ) 0,
                                         eNoAction,
                                         px_Higher_प्राथमिकता_Task_Woken );
            ( px_प्रवाहः_बफर )->x_Task_प्रतीक्षा_गन्तव्यम्_Send = रिक्त;
            xReturn = pdTRUE;
        }
        अन्यथा
        {
            xReturn = pdFALSE;
        }
    }
    पत्तनम्_CLEAR_व्यवधानम्_आवरणम्_FROM_ISR( ux_Saved_व्यवधानम्_स्थितिः );

    प्रत्यागम xReturn;
}
/*-----------------------------------------------------------*/

स्थिर size_t prv_Write_अष्टकानि_गन्तव्यम्_बफर( प्रवाहः_बफर_t * ध्रुव px_प्रवाहः_बफर,
                                     ध्रुव uint8_t * puc_दत्त,
                                     size_t x_गणनम्,
                                     size_t x_शीर्ष )
{
    size_t x_First_दीर्घता;

    configASSERT( x_गणनम् > ( size_t ) 0 );

    /* Calculate the number of bytes that can be added in the first write -
     * which may be less than the total number of bytes that need to be added if
     * the buffer will wrap back to the beginning. */
    x_First_दीर्घता = configMIN( px_प्रवाहः_बफर->x_दीर्घता - x_शीर्ष, x_गणनम् );

    /* Write as many bytes as can be written in the first write. */
    configASSERT( ( x_शीर्ष + x_First_दीर्घता ) <= px_प्रवाहः_बफर->x_दीर्घता );
    ( शून्य ) स्मृतिं_प्रतिलिख( ( शून्य * ) ( &( px_प्रवाहः_बफर->puc_बफर[ x_शीर्ष ] ) ), ( ध्रुव शून्य * ) puc_दत्त, x_First_दीर्घता ); /*lint !e9087 memcpy() requires void *. */

    /* If the number of bytes written was less than the number that could be
     * written in the first write... */
    यदि( x_गणनम् > x_First_दीर्घता )
    {
        /* ...then write the remaining bytes to the start of the buffer. */
        configASSERT( ( x_गणनम् - x_First_दीर्घता ) <= px_प्रवाहः_बफर->x_दीर्घता );
        ( शून्य ) स्मृतिं_प्रतिलिख( ( शून्य * ) px_प्रवाहः_बफर->puc_बफर, ( ध्रुव शून्य * ) &( puc_दत्त[ x_First_दीर्घता ] ), x_गणनम् - x_First_दीर्घता ); /*lint !e9087 memcpy() requires void *. */
    }
    अन्यथा
    {
        mtCOVERAGE_TEST_MARKER();
    }

    x_शीर्ष += x_गणनम्;

    यदि( x_शीर्ष >= px_प्रवाहः_बफर->x_दीर्घता )
    {
        x_शीर्ष -= px_प्रवाहः_बफर->x_दीर्घता;
    }
    अन्यथा
    {
        mtCOVERAGE_TEST_MARKER();
    }

    प्रत्यागम x_शीर्ष;
}
/*-----------------------------------------------------------*/

स्थिर size_t prv_Read_अष्टकानि_From_बफर( प्रवाहः_बफर_t * px_प्रवाहः_बफर,
                                      uint8_t * puc_दत्त,
                                      size_t x_गणनम्,
                                      size_t x_पुच्छ )
{
    size_t x_First_दीर्घता;

    configASSERT( x_गणनम् != ( size_t ) 0 );

    /* Calculate the number of bytes that can be read - which may be
     * less than the number wanted if the data wraps around to the start of
     * the buffer. */
    x_First_दीर्घता = configMIN( px_प्रवाहः_बफर->x_दीर्घता - x_पुच्छ, x_गणनम् );

    /* Obtain the number of bytes it is possible to obtain in the first
     * read.  Asserts check bounds of read and write. */
    configASSERT( x_First_दीर्घता <= x_गणनम् );
    configASSERT( ( x_पुच्छ + x_First_दीर्घता ) <= px_प्रवाहः_बफर->x_दीर्घता );
    ( शून्य ) स्मृतिं_प्रतिलिख( ( शून्य * ) puc_दत्त, ( ध्रुव शून्य * ) &( px_प्रवाहः_बफर->puc_बफर[ x_पुच्छ ] ), x_First_दीर्घता ); /*lint !e9087 memcpy() requires void *. */

    /* If the total number of wanted bytes is greater than the number
     * that could be read in the first read... */
    यदि( x_गणनम् > x_First_दीर्घता )
    {
        /* ...then read the remaining bytes from the start of the buffer. */
        ( शून्य ) स्मृतिं_प्रतिलिख( ( शून्य * ) &( puc_दत्त[ x_First_दीर्घता ] ), ( शून्य * ) ( px_प्रवाहः_बफर->puc_बफर ), x_गणनम् - x_First_दीर्घता ); /*lint !e9087 memcpy() requires void *. */
    }
    अन्यथा
    {
        mtCOVERAGE_TEST_MARKER();
    }

    /* Move the tail pointer to effectively remove the data read from the buffer. */
    x_पुच्छ += x_गणनम्;

    यदि( x_पुच्छ >= px_प्रवाहः_बफर->x_दीर्घता )
    {
        x_पुच्छ -= px_प्रवाहः_बफर->x_दीर्घता;
    }

    प्रत्यागम x_पुच्छ;
}
/*-----------------------------------------------------------*/

स्थिर size_t prv_अष्टकानि_अन्तर्_बफर( ध्रुव प्रवाहः_बफर_t * ध्रुव px_प्रवाहः_बफर )
{
/* Returns the distance between xTail and xHead. */
    size_t x_गणनम्;

    x_गणनम् = px_प्रवाहः_बफर->x_दीर्घता + px_प्रवाहः_बफर->x_शीर्ष;
    x_गणनम् -= px_प्रवाहः_बफर->x_पुच्छ;

    यदि( x_गणनम् >= px_प्रवाहः_बफर->x_दीर्घता )
    {
        x_गणनम् -= px_प्रवाहः_बफर->x_दीर्घता;
    }
    अन्यथा
    {
        mtCOVERAGE_TEST_MARKER();
    }

    प्रत्यागम x_गणनम्;
}
/*-----------------------------------------------------------*/

स्थिर शून्य prv_Initialise_New_प्रवाहः_बफर( प्रवाहः_बफर_t * ध्रुव px_प्रवाहः_बफर,
                                          uint8_t * ध्रुव puc_बफर,
                                          size_t x_बफर_आकार_अष्टकानि,
                                          size_t x_Trigger_Level_अष्टकानि,
                                          uint8_t ucFlags,
                                          प्रवाहः_बफर_Callback_Function_t pxSendCompletedCallback,
                                          प्रवाहः_बफर_Callback_Function_t pxReceiveCompletedCallback )
{
    /* Assert here is deliberately writing to the entire buffer to ensure it can
     * be written to without generating exceptions, and is setting the buffer to a
     * known value to assist in development/debugging. */
    #यदि ( configASSERT_DEFINED == 1 )
    {
        /* The value written just has to be identifiable when looking at the
         * memory.  Don't use 0xA5 as that is the stack fill value and could
         * result in confusion as to what is actually being observed. */
        ध्रुव आधारः_Type_t x_Write_मानम् = 0x55;
        configASSERT( स्मृतिं_पूरय( puc_बफर, ( पूर्णाङ्क ) x_Write_मानम्, x_बफर_आकार_अष्टकानि ) == puc_बफर );
        ( शून्य ) x_Write_मानम्;
    } /*lint !e529 !e438 xWriteValue is only used if configASSERT() is defined. */
    #यदि_अन्तः

    /* This function could be called from xStreamBufferReset(), so we reset the
     * stream buffer fields manually in order to avoid clearing
     * xStreamBufferLock. The xStreamBufferLock is initialized separately on
     * stream buffer creation. */
    px_प्रवाहः_बफर->x_पुच्छ = ( size_t ) 0;
    px_प्रवाहः_बफर->x_शीर्ष = ( size_t ) 0;
    px_प्रवाहः_बफर->x_Task_प्रतीक्षा_गन्तव्यम्_Receive = ( TaskHandle_t ) 0;
    px_प्रवाहः_बफर->x_Task_प्रतीक्षा_गन्तव्यम्_Send = ( TaskHandle_t ) 0;
    #यदि ( configUSE_TRACE_FACILITY == 1 )
        px_प्रवाहः_बफर->ux_प्रवाहः_बफर_सङ्ख्या = ( UBaseType_t ) 0;
    #यदि_अन्तः
    px_प्रवाहः_बफर->puc_बफर = puc_बफर;
    px_प्रवाहः_बफर->x_दीर्घता = x_बफर_आकार_अष्टकानि;
    px_प्रवाहः_बफर->x_Trigger_Level_अष्टकानि = x_Trigger_Level_अष्टकानि;
    px_प्रवाहः_बफर->ucFlags = ucFlags;
    #यदि ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    {
        px_प्रवाहः_बफर->pxSendCompletedCallback = pxSendCompletedCallback;
        px_प्रवाहः_बफर->pxReceiveCompletedCallback = pxReceiveCompletedCallback;
    }
    #अन्यथा
    {
        ( शून्य ) pxSendCompletedCallback;
        ( शून्य ) pxReceiveCompletedCallback;
    }
    #यदि_अन्तः
}

#यदि ( configUSE_TRACE_FACILITY == 1 )

    UBaseType_t ux_प्रवाहः_बफर_Get_प्रवाहः_बफर_सङ्ख्या( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर )
    {
        प्रत्यागम x_प्रवाहः_बफर->ux_प्रवाहः_बफर_सङ्ख्या;
    }

#यदि_अन्तः /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#यदि ( configUSE_TRACE_FACILITY == 1 )

    शून्य v_प्रवाहः_बफर_समुच्चयः_प्रवाहः_बफर_सङ्ख्या( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर,
                                             UBaseType_t ux_प्रवाहः_बफर_सङ्ख्या )
    {
        x_प्रवाहः_बफर->ux_प्रवाहः_बफर_सङ्ख्या = ux_प्रवाहः_बफर_सङ्ख्या;
    }

#यदि_अन्तः /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#यदि ( configUSE_TRACE_FACILITY == 1 )

    uint8_t uc_प्रवाहः_बफर_Get_प्रवाहः_बफर_Type( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर )
    {
        प्रत्यागम( x_प्रवाहः_बफर->ucFlags & sb_FLAGS_IS_सन्देशः_बफर );
    }

#यदि_अन्तः /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/
