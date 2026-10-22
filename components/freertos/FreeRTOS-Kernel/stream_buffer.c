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
#تعریف_کن MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* FreeRTOS includes. */
#ضمن "FreeRTOS.h"
#ضمن "task.h"
#ضمن "stream_buffer.h"
/* Include private IDF API additions for critical thread safety macros */
#ضمن "esp_private/freertos_idf_additions_priv.h"

#إذا ( configUSE_TASK_NOTIFICATIONS != 1 )
    #خطا configUSE_TASK_NOTIFICATIONS must be مجموعه مقصد 1 مقصد build جریان_میانگیر.c
#پایان_اگر

#إذا ( INCLUDE_xTaskGetCurrentTaskHandle != 1 )
    #خطا INCLUDE_xTaskGetCurrentTaskHandle must be مجموعه مقصد 1 مقصد build جریان_میانگیر.c
#پایان_اگر

/* Lint e961, e9021 and e750 are suppressed as a MISRA exception justified
 * because the MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#لغو_تعریف MPU_WRAPPERS_INCLUDED_FROM_API_FILE /*lint !e961 !e750 !e9021. */

/* If the user has not provided application specific Rx notification macros,
 * or #defined the notification macros away, then provide default implementations
 * that uses task notifications. */
/*lint -save -e9026 Function like macros allowed and needed here so they can be overridden. */
#اگر_تعریف_نشده sbRECEIVE_COMPLETED
    #تعریف_کن sbRECEIVE_COMPLETED( px_جریان_میانگیر )                               \
    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( px_جریان_میانگیر->x_جریان_میانگیر_قفل ) ); \
    {                                                                           \
        إذا( ( px_جریان_میانگیر )->x_Task_انتظار_مقصد_Send != عدم )                    \
        {                                                                       \
            ( فراغ ) xTaskNotify( ( px_جریان_میانگیر )->x_Task_انتظار_مقصد_Send,       \
                                  ( uint32_t ) 0,                               \
                                  eNoAction );                                  \
            ( px_جریان_میانگیر )->x_Task_انتظار_مقصد_Send = عدم;                      \
        }                                                                       \
    }                                                                           \
    ( فراغ ) prvEXIT_CRITICAL_OR_RESUME_ALL( &( px_جریان_میانگیر->x_جریان_میانگیر_قفل ) );
#پایان_اگر /* sbRECEIVE_COMPLETED */

/* If user has provided a per-instance receive complete callback, then
 * invoke the callback else use the receive complete macro which is provided by default for all instances.
 */
#إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #تعریف_کن prvRECEIVE_COMPLETED( px_جریان_میانگیر )                                               \
    {                                                                                            \
        إذا( ( px_جریان_میانگیر )->pxReceiveCompletedCallback != عدم )                             \
        {                                                                                        \
            ( px_جریان_میانگیر )->pxReceiveCompletedCallback( ( px_جریان_میانگیر ), pdFALSE, عدم ); \
        }                                                                                        \
        وإلا                                                                                     \
        {                                                                                        \
            sbRECEIVE_COMPLETED( ( px_جریان_میانگیر ) );                                           \
        }                                                                                        \
    }
#وإلا /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #تعریف_کن prvRECEIVE_COMPLETED( px_جریان_میانگیر )    sbRECEIVE_COMPLETED( ( px_جریان_میانگیر ) )
#پایان_اگر /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

#اگر_تعریف_نشده sbRECEIVE_COMPLETED_FROM_ISR
    #تعریف_کن sbRECEIVE_COMPLETED_FROM_ISR( px_جریان_میانگیر,                            \
                                          px_Higher_اولویت_Task_Woken )                \
    {                                                                                \
        UBaseType_t ux_Saved_وقفه_وضعیت;                                          \
                                                                                     \
        ux_Saved_وقفه_وضعیت = ( UBaseType_t ) پورت_مجموعه_وقفه_ماسک_FROM_ISR();  \
        {                                                                            \
            إذا( ( px_جریان_میانگیر )->x_Task_انتظار_مقصد_Send != عدم )                     \
            {                                                                        \
                ( فراغ ) xTaskNotifyFromISR( ( px_جریان_میانگیر )->x_Task_انتظار_مقصد_Send, \
                                             ( uint32_t ) 0,                         \
                                             eNoAction,                              \
                                             ( px_Higher_اولویت_Task_Woken ) );        \
                ( px_جریان_میانگیر )->x_Task_انتظار_مقصد_Send = عدم;                       \
            }                                                                        \
        }                                                                            \
        پورت_CLEAR_وقفه_ماسک_FROM_ISR( ux_Saved_وقفه_وضعیت );                 \
    }
#پایان_اگر /* sbRECEIVE_COMPLETED_FROM_ISR */

#إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #تعریف_کن prvRECEIVE_COMPLETED_FROM_ISR( px_جریان_میانگیر,                                                               \
                                           px_Higher_اولویت_Task_Woken )                                                   \
    {                                                                                                                    \
        إذا( ( px_جریان_میانگیر )->pxReceiveCompletedCallback != عدم )                                                     \
        {                                                                                                                \
            ( px_جریان_میانگیر )->pxReceiveCompletedCallback( ( px_جریان_میانگیر ), pdTRUE, ( px_Higher_اولویت_Task_Woken ) ); \
        }                                                                                                                \
        وإلا                                                                                                             \
        {                                                                                                                \
            sbRECEIVE_COMPLETED_FROM_ISR( ( px_جریان_میانگیر ), ( px_Higher_اولویت_Task_Woken ) );                           \
        }                                                                                                                \
    }
#وإلا /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #تعریف_کن prvRECEIVE_COMPLETED_FROM_ISR( px_جریان_میانگیر, px_Higher_اولویت_Task_Woken ) \
    sbRECEIVE_COMPLETED_FROM_ISR( ( px_جریان_میانگیر ), ( px_Higher_اولویت_Task_Woken ) )
#پایان_اگر /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

/* If the user has not provided an application specific Tx notification macro,
 * or #defined the notification macro away, then provide a default
 * implementation that uses task notifications.
 */
#اگر_تعریف_نشده sbSEND_COMPLETED
    #تعریف_کن sbSEND_COMPLETED( px_جریان_میانگیر )                                  \
    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( px_جریان_میانگیر->x_جریان_میانگیر_قفل ) ); \
    {                                                                           \
        إذا( ( px_جریان_میانگیر )->x_Task_انتظار_مقصد_Receive != عدم )                 \
        {                                                                       \
            ( فراغ ) xTaskNotify( ( px_جریان_میانگیر )->x_Task_انتظار_مقصد_Receive,    \
                                  ( uint32_t ) 0,                               \
                                  eNoAction );                                  \
            ( px_جریان_میانگیر )->x_Task_انتظار_مقصد_Receive = عدم;                   \
        }                                                                       \
    }                                                                           \
    ( فراغ ) prvEXIT_CRITICAL_OR_RESUME_ALL( &( px_جریان_میانگیر->x_جریان_میانگیر_قفل ) );
#پایان_اگر /* sbSEND_COMPLETED */

/* If user has provided a per-instance send completed callback, then
 * invoke the callback else use the send complete macro which is provided by default for all instances.
 */
#إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #تعریف_کن prvSEND_COMPLETED( px_جریان_میانگیر )                                           \
    {                                                                                     \
        إذا( ( px_جریان_میانگیر )->pxSendCompletedCallback != عدم )                         \
        {                                                                                 \
            px_جریان_میانگیر->pxSendCompletedCallback( ( px_جریان_میانگیر ), pdFALSE, عدم ); \
        }                                                                                 \
        وإلا                                                                              \
        {                                                                                 \
            sbSEND_COMPLETED( ( px_جریان_میانگیر ) );                                       \
        }                                                                                 \
    }
#وإلا /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #تعریف_کن prvSEND_COMPLETED( px_جریان_میانگیر )    sbSEND_COMPLETED( ( px_جریان_میانگیر ) )
#پایان_اگر /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */


#اگر_تعریف_نشده sbSEND_COMPLETE_FROM_ISR
    #تعریف_کن sbSEND_COMPLETE_FROM_ISR( px_جریان_میانگیر, px_Higher_اولویت_Task_Woken )       \
    {                                                                                   \
        UBaseType_t ux_Saved_وقفه_وضعیت;                                             \
                                                                                        \
        ux_Saved_وقفه_وضعیت = ( UBaseType_t ) پورت_مجموعه_وقفه_ماسک_FROM_ISR();     \
        {                                                                               \
            إذا( ( px_جریان_میانگیر )->x_Task_انتظار_مقصد_Receive != عدم )                     \
            {                                                                           \
                ( فراغ ) xTaskNotifyFromISR( ( px_جریان_میانگیر )->x_Task_انتظار_مقصد_Receive, \
                                             ( uint32_t ) 0,                            \
                                             eNoAction,                                 \
                                             ( px_Higher_اولویت_Task_Woken ) );           \
                ( px_جریان_میانگیر )->x_Task_انتظار_مقصد_Receive = عدم;                       \
            }                                                                           \
        }                                                                               \
        پورت_CLEAR_وقفه_ماسک_FROM_ISR( ux_Saved_وقفه_وضعیت );                    \
    }
#پایان_اگر /* sbSEND_COMPLETE_FROM_ISR */


#إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #تعریف_کن prvSEND_COMPLETE_FROM_ISR( px_جریان_میانگیر, px_Higher_اولویت_Task_Woken )                                    \
    {                                                                                                                 \
        إذا( ( px_جریان_میانگیر )->pxSendCompletedCallback != عدم )                                                     \
        {                                                                                                             \
            ( px_جریان_میانگیر )->pxSendCompletedCallback( ( px_جریان_میانگیر ), pdTRUE, ( px_Higher_اولویت_Task_Woken ) ); \
        }                                                                                                             \
        وإلا                                                                                                          \
        {                                                                                                             \
            sbSEND_COMPLETE_FROM_ISR( ( px_جریان_میانگیر ), ( px_Higher_اولویت_Task_Woken ) );                            \
        }                                                                                                             \
    }
#وإلا /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #تعریف_کن prvSEND_COMPLETE_FROM_ISR( px_جریان_میانگیر, px_Higher_اولویت_Task_Woken ) \
    sbSEND_COMPLETE_FROM_ISR( ( px_جریان_میانگیر ), ( px_Higher_اولویت_Task_Woken ) )
#پایان_اگر /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

/*lint -restore (9026) */

/* The number of bytes used to hold the length of a message in the buffer. */
#تعریف_کن sb_بایتها_مقصد_STORE_پیام_درازی    ( حجم( config_پیام_میانگیر_درازی_TYPE ) )

/* Bits stored in the ucFlags field of the stream buffer. */
#تعریف_کن sb_FLAGS_IS_پیام_میانگیر          ( ( uint8_t ) 1 ) /* Set if the stream buffer was created as a message buffer, in which case it holds discrete messages rather than a stream. */
#تعریف_کن sbFLAGS_IS_STATICALLY_ALLOCATED    ( ( uint8_t ) 2 ) /* Set if the stream buffer was created using statically allocated memory. */

/*-----------------------------------------------------------*/

/* Structure that hold state information on the buffer. */
تعريفنوع بنية جریان_میانگیر_Def_t                 /*lint !e9058 Style convention uses tag. */
{
    متطاير size_t x_دم;                       /* Index to the next item to read within the buffer. */
    متطاير size_t x_سر;                       /* Index to the next item to write within the buffer. */
    size_t x_درازی;                              /* The length of the buffer pointed to by pucBuffer. */
    size_t x_Trigger_Level_بایتها;                   /* The number of bytes that must be in the stream buffer before a task that is waiting for data is unblocked. */
    متطاير TaskHandle_t x_Task_انتظار_مقصد_Receive; /* Holds the handle of a task waiting for data, or NULL if no tasks are waiting. */
    متطاير TaskHandle_t x_Task_انتظار_مقصد_Send;    /* Holds the handle of a task waiting to send data to a message buffer that is full. */
    uint8_t * puc_میانگیر;                         /* Points to the buffer itself - that is - the RAM that stores the data passed through the buffer. */
    uint8_t ucFlags;

    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_جریان_میانگیر_عدد; /* Used for tracing purposes. */
    #پایان_اگر

    #إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        جریان_میانگیر_Callback_Function_t pxSendCompletedCallback;    /* Optional callback called on send complete. sbSEND_COMPLETED is called if this is NULL. */
        جریان_میانگیر_Callback_Function_t pxReceiveCompletedCallback; /* Optional callback called on receive complete.  sbRECEIVE_COMPLETED is called if this is NULL. */
    #پایان_اگر

    پورت_MUX_TYPE x_جریان_میانگیر_قفل; /* Spinlock required for SMP critical sections */
} جریان_میانگیر_t;

/*
 * The number of bytes available to be read from the buffer.
 */
ساكن size_t prv_بایتها_درون_میانگیر( ثابت جریان_میانگیر_t * ثابت px_جریان_میانگیر ) PRIVILEGED_FUNCTION;

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
ساكن size_t prv_Write_بایتها_مقصد_میانگیر( جریان_میانگیر_t * ثابت px_جریان_میانگیر,
                                     ثابت uint8_t * puc_داده,
                                     size_t x_شمار,
                                     size_t x_سر ) PRIVILEGED_FUNCTION;

/*
 * If the stream buffer is being used as a message buffer, then reads an entire
 * message out of the buffer.  If the stream buffer is being used as a stream
 * buffer then read as many bytes as possible from the buffer.
 * prvReadBytesFromBuffer() is called to actually extract the bytes from the
 * buffer's data storage area.
 */
ساكن size_t prv_Read_پیام_From_میانگیر( جریان_میانگیر_t * px_جریان_میانگیر,
                                        فراغ * pv_Rx_داده,
                                        size_t x_میانگیر_درازی_بایتها,
                                        size_t x_بایتها_موجود ) PRIVILEGED_FUNCTION;

/*
 * If the stream buffer is being used as a message buffer, then writes an entire
 * message to the buffer.  If the stream buffer is being used as a stream
 * buffer then write as many bytes as possible to the buffer.
 * prvWriteBytestoBuffer() is called to actually send the bytes to the buffer's
 * data storage area.
 */
ساكن size_t prv_Write_پیام_مقصد_میانگیر( جریان_میانگیر_t * ثابت px_جریان_میانگیر,
                                       ثابت فراغ * pv_Tx_داده,
                                       size_t x_داده_درازی_بایتها,
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
ساكن size_t prv_Read_بایتها_From_میانگیر( جریان_میانگیر_t * px_جریان_میانگیر,
                                      uint8_t * puc_داده,
                                      size_t x_شمار,
                                      size_t x_دم ) PRIVILEGED_FUNCTION;

/*
 * Called by both pxStreamBufferCreate() and pxStreamBufferCreateStatic() to
 * initialise the members of the newly created stream buffer structure.
 */
ساكن فراغ prv_Initialise_New_جریان_میانگیر( جریان_میانگیر_t * ثابت px_جریان_میانگیر,
                                          uint8_t * ثابت puc_میانگیر,
                                          size_t x_میانگیر_اندازه_بایتها,
                                          size_t x_Trigger_Level_بایتها,
                                          uint8_t ucFlags,
                                          جریان_میانگیر_Callback_Function_t pxSendCompletedCallback,
                                          جریان_میانگیر_Callback_Function_t pxReceiveCompletedCallback ) PRIVILEGED_FUNCTION;

/*-----------------------------------------------------------*/
#إذا ( config_SUPPORT_DYNAMIC_تخصیص == 1 )
    جریان_میانگیر_Handle_t x_جریان_میانگیر_Generic_Create( size_t x_میانگیر_اندازه_بایتها,
                                                     size_t x_Trigger_Level_بایتها,
                                                     پایه_Type_t x_Is_پیام_میانگیر,
                                                     جریان_میانگیر_Callback_Function_t pxSendCompletedCallback,
                                                     جریان_میانگیر_Callback_Function_t pxReceiveCompletedCallback )
    {
        uint8_t * puc_Allocated_حافظه;
        uint8_t ucFlags;

        /* In case the stream buffer is going to be used as a message buffer
         * (that is, it will hold discrete messages with a little meta data that
         * says how big the next message is) check the buffer will be large enough
         * to hold at least one message. */
        إذا( x_Is_پیام_میانگیر == pdTRUE )
        {
            /* Is a message buffer but not statically allocated. */
            ucFlags = sb_FLAGS_IS_پیام_میانگیر;
            configASSERT( x_میانگیر_اندازه_بایتها > sb_بایتها_مقصد_STORE_پیام_درازی );
        }
        وإلا
        {
            /* Not a message buffer and not statically allocated. */
            ucFlags = 0;
            configASSERT( x_میانگیر_اندازه_بایتها > 0 );
        }

        configASSERT( x_Trigger_Level_بایتها <= x_میانگیر_اندازه_بایتها );

        /* A trigger level of 0 would cause a waiting task to unblock even when
         * the buffer was empty. */
        إذا( x_Trigger_Level_بایتها == ( size_t ) 0 )
        {
            x_Trigger_Level_بایتها = ( size_t ) 1;
        }

        /* A stream buffer requires a StreamBuffer_t structure and a buffer.
         * Both are allocated in a single call to pvPortMalloc().  The
         * StreamBuffer_t structure is placed at the start of the allocated memory
         * and the buffer follows immediately after.  The requested size is
         * incremented so the free space is returned as the user would expect -
         * this is a quirk of the implementation that means otherwise the free
         * space would be reported as one byte smaller than would be logically
         * expected. */
        إذا( x_میانگیر_اندازه_بایتها < ( x_میانگیر_اندازه_بایتها + 1 + حجم( جریان_میانگیر_t ) ) )
        {
            x_میانگیر_اندازه_بایتها++;
            puc_Allocated_حافظه = ( uint8_t * ) pv_پورت_Malloc( x_میانگیر_اندازه_بایتها + حجم( جریان_میانگیر_t ) ); /*lint !e9079 malloc() only returns void*. */
        }
        وإلا
        {
            puc_Allocated_حافظه = عدم;
        }

        إذا( puc_Allocated_حافظه != عدم )
        {
            prv_Initialise_New_جریان_میانگیر( ( جریان_میانگیر_t * ) puc_Allocated_حافظه,       /* Structure at the start of the allocated memory. */ /*lint !e9087 Safe cast as allocated memory is aligned. */ /*lint !e826 Area is not too small and alignment is guaranteed provided malloc() behaves as expected and returns aligned buffer. */
                                          puc_Allocated_حافظه + حجم( جریان_میانگیر_t ), /* Storage area follows. */ /*lint !e9016 Indexing past structure valid for uint8_t pointer, also storage area has no alignment requirement. */
                                          x_میانگیر_اندازه_بایتها,
                                          x_Trigger_Level_بایتها,
                                          ucFlags,
                                          pxSendCompletedCallback,
                                          pxReceiveCompletedCallback );

            /* Initialize the stream buffer's spinlock separately, as
             * prvInitialiseNewStreamBuffer() is also called from
             * xStreamBufferReset(). */
            پورت_MUX_INITIALIZE( &( ( ( جریان_میانگیر_t * ) puc_Allocated_حافظه )->x_جریان_میانگیر_قفل ) );

            trace_جریان_میانگیر_CREATE( ( ( جریان_میانگیر_t * ) puc_Allocated_حافظه ), x_Is_پیام_میانگیر );
        }
        وإلا
        {
            trace_جریان_میانگیر_CREATE_FAILED( x_Is_پیام_میانگیر );
        }

        أرجع ( جریان_میانگیر_Handle_t ) puc_Allocated_حافظه; /*lint !e9087 !e826 Safe cast as allocated memory is aligned. */
    }
#پایان_اگر /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_تخصیص == 1 )

    جریان_میانگیر_Handle_t x_جریان_میانگیر_Generic_Create_Static( size_t x_میانگیر_اندازه_بایتها,
                                                           size_t x_Trigger_Level_بایتها,
                                                           پایه_Type_t x_Is_پیام_میانگیر,
                                                           uint8_t * ثابت puc_جریان_میانگیر_Storage_Area,
                                                           Static_جریان_میانگیر_t * ثابت px_Static_جریان_میانگیر,
                                                           جریان_میانگیر_Callback_Function_t pxSendCompletedCallback,
                                                           جریان_میانگیر_Callback_Function_t pxReceiveCompletedCallback )
    {
        جریان_میانگیر_t * ثابت px_جریان_میانگیر = ( جریان_میانگیر_t * ) px_Static_جریان_میانگیر; /*lint !e740 !e9087 Safe cast as StaticStreamBuffer_t is opaque Streambuffer_t. */
        جریان_میانگیر_Handle_t xReturn;
        uint8_t ucFlags;

        configASSERT( puc_جریان_میانگیر_Storage_Area );
        configASSERT( px_Static_جریان_میانگیر );
        configASSERT( x_Trigger_Level_بایتها <= x_میانگیر_اندازه_بایتها );

        /* A trigger level of 0 would cause a waiting task to unblock even when
         * the buffer was empty. */
        إذا( x_Trigger_Level_بایتها == ( size_t ) 0 )
        {
            x_Trigger_Level_بایتها = ( size_t ) 1;
        }

        إذا( x_Is_پیام_میانگیر != pdFALSE )
        {
            /* Statically allocated message buffer. */
            ucFlags = sb_FLAGS_IS_پیام_میانگیر | sbFLAGS_IS_STATICALLY_ALLOCATED;
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
        configASSERT( x_میانگیر_اندازه_بایتها > sb_بایتها_مقصد_STORE_پیام_درازی );

        #إذا ( configASSERT_DEFINED == 1 )
        {
            /* Sanity check that the size of the structure used to declare a
             * variable of type StaticStreamBuffer_t equals the size of the real
             * message buffer structure. */
            متطاير size_t x_اندازه = حجم( Static_جریان_میانگیر_t );
            configASSERT( x_اندازه == حجم( جریان_میانگیر_t ) );
            ( فراغ ) x_اندازه; /* Prevent unused variable warning when configASSERT() is not used. */
        } /*lint !e529 xSize is referenced is configASSERT() is defined. */
        #پایان_اگر /* configASSERT_DEFINED */

        إذا( ( puc_جریان_میانگیر_Storage_Area != عدم ) && ( px_Static_جریان_میانگیر != عدم ) )
        {
            prv_Initialise_New_جریان_میانگیر( px_جریان_میانگیر,
                                          puc_جریان_میانگیر_Storage_Area,
                                          x_میانگیر_اندازه_بایتها,
                                          x_Trigger_Level_بایتها,
                                          ucFlags,
                                          pxSendCompletedCallback,
                                          pxReceiveCompletedCallback );

            /* Remember this was statically allocated in case it is ever deleted
             * again. */
            px_جریان_میانگیر->ucFlags |= sbFLAGS_IS_STATICALLY_ALLOCATED;

            /* Initialize the stream buffer's spinlock separately, as
             * prvInitialiseNewStreamBuffer() is also called from
             * xStreamBufferReset(). */
            پورت_MUX_INITIALIZE( &( px_جریان_میانگیر->x_جریان_میانگیر_قفل ) );

            trace_جریان_میانگیر_CREATE( px_جریان_میانگیر, x_Is_پیام_میانگیر );

            xReturn = ( جریان_میانگیر_Handle_t ) px_Static_جریان_میانگیر; /*lint !e9087 Data hiding requires cast to opaque type. */
        }
        وإلا
        {
            xReturn = عدم;
            trace_جریان_میانگیر_CREATE_STATIC_FAILED( xReturn, x_Is_پیام_میانگیر );
        }

        أرجع xReturn;
    }
#پایان_اگر /* ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*-----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_تخصیص == 1 )
    پایه_Type_t x_جریان_میانگیر_Get_Static_Buffers( جریان_میانگیر_Handle_t x_جریان_میانگیر,
                                              uint8_t ** ppuc_جریان_میانگیر_Storage_Area,
                                              Static_جریان_میانگیر_t ** ppx_Static_جریان_میانگیر )
    {
        پایه_Type_t xReturn;
        ثابت جریان_میانگیر_t * ثابت px_جریان_میانگیر = x_جریان_میانگیر;

        configASSERT( px_جریان_میانگیر );
        configASSERT( ppuc_جریان_میانگیر_Storage_Area );
        configASSERT( ppx_Static_جریان_میانگیر );

        إذا( ( px_جریان_میانگیر->ucFlags & sbFLAGS_IS_STATICALLY_ALLOCATED ) != ( uint8_t ) 0 )
        {
            *ppuc_جریان_میانگیر_Storage_Area = px_جریان_میانگیر->puc_میانگیر;
            *ppx_Static_جریان_میانگیر = ( Static_جریان_میانگیر_t * ) px_جریان_میانگیر;
            xReturn = pdTRUE;
        }
        وإلا
        {
            xReturn = pdFALSE;
        }

        أرجع xReturn;
    }
#پایان_اگر /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

فراغ v_جریان_میانگیر_Delete( جریان_میانگیر_Handle_t x_جریان_میانگیر )
{
    جریان_میانگیر_t * px_جریان_میانگیر = x_جریان_میانگیر;

    configASSERT( px_جریان_میانگیر );

    trace_جریان_میانگیر_DELETE( x_جریان_میانگیر );

    إذا( ( px_جریان_میانگیر->ucFlags & sbFLAGS_IS_STATICALLY_ALLOCATED ) == ( uint8_t ) pdFALSE )
    {
        #إذا ( config_SUPPORT_DYNAMIC_تخصیص == 1 )
        {
            /* Both the structure and the buffer were allocated using a single call
            * to pvPortMalloc(), hence only one call to vPortFree() is required. */
            v_پورت_Free( ( فراغ * ) px_جریان_میانگیر ); /*lint !e9087 Standard free() semantics require void *, plus pxStreamBuffer was allocated by pvPortMalloc(). */
        }
        #وإلا
        {
            /* Should not be possible to get here, ucFlags must be corrupt.
             * Force an assert. */
            configASSERT( x_جریان_میانگیر == ( جریان_میانگیر_Handle_t ) ~0 );
        }
        #پایان_اگر
    }
    وإلا
    {
        /* The structure and buffer were not allocated dynamically and cannot be
         * freed - just scrub the structure so future use will assert. */
        ( فراغ ) memset( px_جریان_میانگیر, 0x00, حجم( جریان_میانگیر_t ) );
    }
}
/*-----------------------------------------------------------*/

پایه_Type_t x_جریان_میانگیر_Reset( جریان_میانگیر_Handle_t x_جریان_میانگیر )
{
    جریان_میانگیر_t * ثابت px_جریان_میانگیر = x_جریان_میانگیر;
    پایه_Type_t xReturn = pdFAIL;
    جریان_میانگیر_Callback_Function_t pxSendCallback = عدم, pxReceiveCallback = عدم;

    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_جریان_میانگیر_عدد;
    #پایان_اگر

    configASSERT( px_جریان_میانگیر );

    #إذا ( configUSE_TRACE_FACILITY == 1 )
    {
        /* Store the stream buffer number so it can be restored after the
         * reset. */
        ux_جریان_میانگیر_عدد = px_جریان_میانگیر->ux_جریان_میانگیر_عدد;
    }
    #پایان_اگر

    /* Can only reset a message buffer if there are no tasks blocked on it. */
    taskENTER_CRITICAL( &( px_جریان_میانگیر->x_جریان_میانگیر_قفل ) );
    {
        إذا( ( px_جریان_میانگیر->x_Task_انتظار_مقصد_Receive == عدم ) && ( px_جریان_میانگیر->x_Task_انتظار_مقصد_Send == عدم ) )
        {
            #إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
            {
                pxSendCallback = px_جریان_میانگیر->pxSendCompletedCallback;
                pxReceiveCallback = px_جریان_میانگیر->pxReceiveCompletedCallback;
            }
            #پایان_اگر

            prv_Initialise_New_جریان_میانگیر( px_جریان_میانگیر,
                                          px_جریان_میانگیر->puc_میانگیر,
                                          px_جریان_میانگیر->x_درازی,
                                          px_جریان_میانگیر->x_Trigger_Level_بایتها,
                                          px_جریان_میانگیر->ucFlags,
                                          pxSendCallback,
                                          pxReceiveCallback );

            #إذا ( configUSE_TRACE_FACILITY == 1 )
            {
                px_جریان_میانگیر->ux_جریان_میانگیر_عدد = ux_جریان_میانگیر_عدد;
            }
            #پایان_اگر

            trace_جریان_میانگیر_RESET( x_جریان_میانگیر );

            xReturn = pdPASS;
        }
    }
    taskEXIT_CRITICAL( &( px_جریان_میانگیر->x_جریان_میانگیر_قفل ) );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

پایه_Type_t x_جریان_میانگیر_مجموعه_Trigger_Level( جریان_میانگیر_Handle_t x_جریان_میانگیر,
                                         size_t xTriggerLevel )
{
    جریان_میانگیر_t * ثابت px_جریان_میانگیر = x_جریان_میانگیر;
    پایه_Type_t xReturn;

    configASSERT( px_جریان_میانگیر );

    /* It is not valid for the trigger level to be 0. */
    إذا( xTriggerLevel == ( size_t ) 0 )
    {
        xTriggerLevel = ( size_t ) 1;
    }

    /* The trigger level is the number of bytes that must be in the stream
     * buffer before a task that is waiting for data is unblocked. */
    إذا( xTriggerLevel < px_جریان_میانگیر->x_درازی )
    {
        px_جریان_میانگیر->x_Trigger_Level_بایتها = xTriggerLevel;
        xReturn = pdPASS;
    }
    وإلا
    {
        xReturn = pdFALSE;
    }

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

size_t x_جریان_میانگیر_Spaces_موجود( جریان_میانگیر_Handle_t x_جریان_میانگیر )
{
    ثابت جریان_میانگیر_t * ثابت px_جریان_میانگیر = x_جریان_میانگیر;
    size_t xSpace;
    size_t x_Original_دم;

    configASSERT( px_جریان_میانگیر );

    /* The code below reads xTail and then xHead.  This is safe if the stream
     * buffer is updated once between the two reads - but not if the stream buffer
     * is updated more than once between the two reads - hence the loop. */
    افعل
    {
        x_Original_دم = px_جریان_میانگیر->x_دم;
        xSpace = px_جریان_میانگیر->x_درازی + px_جریان_میانگیر->x_دم;
        xSpace -= px_جریان_میانگیر->x_سر;
    } طالما( x_Original_دم != px_جریان_میانگیر->x_دم );

    xSpace -= ( size_t ) 1;

    إذا( xSpace >= px_جریان_میانگیر->x_درازی )
    {
        xSpace -= px_جریان_میانگیر->x_درازی;
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    أرجع xSpace;
}
/*-----------------------------------------------------------*/

size_t x_جریان_میانگیر_بایتها_موجود( جریان_میانگیر_Handle_t x_جریان_میانگیر )
{
    ثابت جریان_میانگیر_t * ثابت px_جریان_میانگیر = x_جریان_میانگیر;
    size_t xReturn;

    configASSERT( px_جریان_میانگیر );

    xReturn = prv_بایتها_درون_میانگیر( px_جریان_میانگیر );
    أرجع xReturn;
}
/*-----------------------------------------------------------*/

size_t x_جریان_میانگیر_Send( جریان_میانگیر_Handle_t x_جریان_میانگیر,
                          ثابت فراغ * pv_Tx_داده,
                          size_t x_داده_درازی_بایتها,
                          TickType_t x_Ticks_مقصد_Wait )
{
    جریان_میانگیر_t * ثابت px_جریان_میانگیر = x_جریان_میانگیر;
    size_t xReturn, xSpace = 0;
    size_t xRequiredSpace = x_داده_درازی_بایتها;
    زمان_بیرون_t x_زمان_بیرون_2;
    size_t xMaxReportedSpace = 0;

    configASSERT( pv_Tx_داده );
    configASSERT( px_جریان_میانگیر );

    /* The maximum amount of space a stream buffer will ever report is its length
     * minus 1. */
    xMaxReportedSpace = px_جریان_میانگیر->x_درازی - ( size_t ) 1;

    /* This send function is used to write to both message buffers and stream
     * buffers.  If this is a message buffer then the space needed must be
     * increased by the amount of bytes needed to store the length of the
     * message. */
    إذا( ( px_جریان_میانگیر->ucFlags & sb_FLAGS_IS_پیام_میانگیر ) != ( uint8_t ) 0 )
    {
        xRequiredSpace += sb_بایتها_مقصد_STORE_پیام_درازی;

        /* Overflow? */
        configASSERT( xRequiredSpace > x_داده_درازی_بایتها );

        /* If this is a message buffer then it must be possible to write the
         * whole message. */
        إذا( xRequiredSpace > xMaxReportedSpace )
        {
            /* The message would not fit even if the entire buffer was empty,
             * so don't wait for space. */
            x_Ticks_مقصد_Wait = ( TickType_t ) 0;
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

    إذا( x_Ticks_مقصد_Wait != ( TickType_t ) 0 )
    {
        v_Task_مجموعه_زمان_بیرون_State( &x_زمان_بیرون_2 );

        افعل
        {
            /* Wait until the required number of bytes are free in the message
             * buffer. */
            taskENTER_CRITICAL( &( px_جریان_میانگیر->x_جریان_میانگیر_قفل ) );
            {
                xSpace = x_جریان_میانگیر_Spaces_موجود( px_جریان_میانگیر );

                إذا( xSpace < xRequiredSpace )
                {
                    /* Clear notification state as going to wait for space. */
                    ( فراغ ) xTaskNotifyStateClear( عدم );

                    /* Should only be one writer. */
                    configASSERT( px_جریان_میانگیر->x_Task_انتظار_مقصد_Send == عدم );
                    px_جریان_میانگیر->x_Task_انتظار_مقصد_Send = xTaskGetCurrentTaskHandle();
                }
                وإلا
                {
                    taskEXIT_CRITICAL( &( px_جریان_میانگیر->x_جریان_میانگیر_قفل ) );
                    اكسر;
                }
            }
            taskEXIT_CRITICAL( &( px_جریان_میانگیر->x_جریان_میانگیر_قفل ) );

            trace_BLOCKING_ON_جریان_میانگیر_SEND( x_جریان_میانگیر );
            ( فراغ ) xTaskNotifyWait( ( uint32_t ) 0, ( uint32_t ) 0, عدم, x_Ticks_مقصد_Wait );
            /* In SMP mode, the task may have been woken and scheduled on
             * another core. Hence, we must clear the xTaskWaitingToSend
             * handle in a critical section. */
            #إذا ( config_عدد_از_CORES > 1 )
                taskENTER_CRITICAL( &( px_جریان_میانگیر->x_جریان_میانگیر_قفل ) );
            #پایان_اگر /* configNUMBER_OF_CORES > 1 */
            {
                px_جریان_میانگیر->x_Task_انتظار_مقصد_Send = عدم;
            }
            #إذا ( config_عدد_از_CORES > 1 )
                taskEXIT_CRITICAL( &( px_جریان_میانگیر->x_جریان_میانگیر_قفل ) );
            #پایان_اگر /* configNUMBER_OF_CORES > 1 */
        } طالما( x_Task_Check_For_زمان_بیرون( &x_زمان_بیرون_2, &x_Ticks_مقصد_Wait ) == pdFALSE );
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    إذا( xSpace == ( size_t ) 0 )
    {
        xSpace = x_جریان_میانگیر_Spaces_موجود( px_جریان_میانگیر );
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    xReturn = prv_Write_پیام_مقصد_میانگیر( px_جریان_میانگیر, pv_Tx_داده, x_داده_درازی_بایتها, xSpace, xRequiredSpace );

    إذا( xReturn > ( size_t ) 0 )
    {
        trace_جریان_میانگیر_SEND( x_جریان_میانگیر, xReturn );

        /* Was a task waiting for the data? */
        إذا( prv_بایتها_درون_میانگیر( px_جریان_میانگیر ) >= px_جریان_میانگیر->x_Trigger_Level_بایتها )
        {
            prvSEND_COMPLETED( px_جریان_میانگیر );
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
        trace_جریان_میانگیر_SEND_FAILED( x_جریان_میانگیر );
    }

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

size_t x_جریان_میانگیر_Send_From_ISR( جریان_میانگیر_Handle_t x_جریان_میانگیر,
                                 ثابت فراغ * pv_Tx_داده,
                                 size_t x_داده_درازی_بایتها,
                                 پایه_Type_t * ثابت px_Higher_اولویت_Task_Woken )
{
    جریان_میانگیر_t * ثابت px_جریان_میانگیر = x_جریان_میانگیر;
    size_t xReturn, xSpace;
    size_t xRequiredSpace = x_داده_درازی_بایتها;

    configASSERT( pv_Tx_داده );
    configASSERT( px_جریان_میانگیر );

    /* This send function is used to write to both message buffers and stream
     * buffers.  If this is a message buffer then the space needed must be
     * increased by the amount of bytes needed to store the length of the
     * message. */
    إذا( ( px_جریان_میانگیر->ucFlags & sb_FLAGS_IS_پیام_میانگیر ) != ( uint8_t ) 0 )
    {
        xRequiredSpace += sb_بایتها_مقصد_STORE_پیام_درازی;
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    xSpace = x_جریان_میانگیر_Spaces_موجود( px_جریان_میانگیر );
    xReturn = prv_Write_پیام_مقصد_میانگیر( px_جریان_میانگیر, pv_Tx_داده, x_داده_درازی_بایتها, xSpace, xRequiredSpace );

    إذا( xReturn > ( size_t ) 0 )
    {
        /* Was a task waiting for the data? */
        إذا( prv_بایتها_درون_میانگیر( px_جریان_میانگیر ) >= px_جریان_میانگیر->x_Trigger_Level_بایتها )
        {
            prvSEND_COMPLETE_FROM_ISR( px_جریان_میانگیر, px_Higher_اولویت_Task_Woken );
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

    trace_جریان_میانگیر_SEND_FROM_ISR( x_جریان_میانگیر, xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

ساكن size_t prv_Write_پیام_مقصد_میانگیر( جریان_میانگیر_t * ثابت px_جریان_میانگیر,
                                       ثابت فراغ * pv_Tx_داده,
                                       size_t x_داده_درازی_بایتها,
                                       size_t xSpace,
                                       size_t xRequiredSpace )
{
    size_t x_بعدی_سر = px_جریان_میانگیر->x_سر;
    config_پیام_میانگیر_درازی_TYPE x_پیام_درازی;

    إذا( ( px_جریان_میانگیر->ucFlags & sb_FLAGS_IS_پیام_میانگیر ) != ( uint8_t ) 0 )
    {
        /* This is a message buffer, as opposed to a stream buffer. */

        /* Convert xDataLengthBytes to the message length type. */
        x_پیام_درازی = ( config_پیام_میانگیر_درازی_TYPE ) x_داده_درازی_بایتها;

        /* Ensure the data length given fits within configMESSAGE_BUFFER_LENGTH_TYPE. */
        configASSERT( ( size_t ) x_پیام_درازی == x_داده_درازی_بایتها );

        إذا( xSpace >= xRequiredSpace )
        {
            /* There is enough space to write both the message length and the message
             * itself into the buffer.  Start by writing the length of the data, the data
             * itself will be written later in this function. */
            x_بعدی_سر = prv_Write_بایتها_مقصد_میانگیر( px_جریان_میانگیر, ( ثابت uint8_t * ) &( x_پیام_درازی ), sb_بایتها_مقصد_STORE_پیام_درازی, x_بعدی_سر );
        }
        وإلا
        {
            /* Not enough space, so do not write data to the buffer. */
            x_داده_درازی_بایتها = 0;
        }
    }
    وإلا
    {
        /* This is a stream buffer, as opposed to a message buffer, so writing a
         * stream of bytes rather than discrete messages.  Plan to write as many
         * bytes as possible. */
        x_داده_درازی_بایتها = configMIN( x_داده_درازی_بایتها, xSpace );
    }

    إذا( x_داده_درازی_بایتها != ( size_t ) 0 )
    {
        /* Write the data to the buffer. */
        px_جریان_میانگیر->x_سر = prv_Write_بایتها_مقصد_میانگیر( px_جریان_میانگیر, ( ثابت uint8_t * ) pv_Tx_داده, x_داده_درازی_بایتها, x_بعدی_سر ); /*lint !e9079 Storage buffer is implemented as uint8_t for ease of sizing, alignment and access. */
    }

    أرجع x_داده_درازی_بایتها;
}
/*-----------------------------------------------------------*/

size_t x_جریان_میانگیر_Receive( جریان_میانگیر_Handle_t x_جریان_میانگیر,
                             فراغ * pv_Rx_داده,
                             size_t x_میانگیر_درازی_بایتها,
                             TickType_t x_Ticks_مقصد_Wait )
{
    جریان_میانگیر_t * ثابت px_جریان_میانگیر = x_جریان_میانگیر;
    size_t x_Received_درازی = 0, x_بایتها_موجود, x_بایتها_مقصد_Store_پیام_درازی;

    configASSERT( pv_Rx_داده );
    configASSERT( px_جریان_میانگیر );

    /* This receive function is used by both message buffers, which store
     * discrete messages, and stream buffers, which store a continuous stream of
     * bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the
     * message. */
    إذا( ( px_جریان_میانگیر->ucFlags & sb_FLAGS_IS_پیام_میانگیر ) != ( uint8_t ) 0 )
    {
        x_بایتها_مقصد_Store_پیام_درازی = sb_بایتها_مقصد_STORE_پیام_درازی;
    }
    وإلا
    {
        x_بایتها_مقصد_Store_پیام_درازی = 0;
    }

    إذا( x_Ticks_مقصد_Wait != ( TickType_t ) 0 )
    {
        /* Checking if there is data and clearing the notification state must be
         * performed atomically. */
        taskENTER_CRITICAL( &( px_جریان_میانگیر->x_جریان_میانگیر_قفل ) );
        {
            x_بایتها_موجود = prv_بایتها_درون_میانگیر( px_جریان_میانگیر );

            /* If this function was invoked by a message buffer read then
             * xBytesToStoreMessageLength holds the number of bytes used to hold
             * the length of the next discrete message.  If this function was
             * invoked by a stream buffer read then xBytesToStoreMessageLength will
             * be 0. */
            إذا( x_بایتها_موجود <= x_بایتها_مقصد_Store_پیام_درازی )
            {
                /* Clear notification state as going to wait for data. */
                ( فراغ ) xTaskNotifyStateClear( عدم );

                /* Should only be one reader. */
                configASSERT( px_جریان_میانگیر->x_Task_انتظار_مقصد_Receive == عدم );
                px_جریان_میانگیر->x_Task_انتظار_مقصد_Receive = xTaskGetCurrentTaskHandle();
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        taskEXIT_CRITICAL( &( px_جریان_میانگیر->x_جریان_میانگیر_قفل ) );

        إذا( x_بایتها_موجود <= x_بایتها_مقصد_Store_پیام_درازی )
        {
            /* Wait for data to be available. */
            trace_BLOCKING_ON_جریان_میانگیر_RECEIVE( x_جریان_میانگیر );
            ( فراغ ) xTaskNotifyWait( ( uint32_t ) 0, ( uint32_t ) 0, عدم, x_Ticks_مقصد_Wait );

            /* In SMP mode, the task may have been woken and scheduled on
             * another core. Hence, we must clear the xTaskWaitingToReceive
             * handle in a critical section. */
            #إذا ( config_عدد_از_CORES > 1 )
                taskENTER_CRITICAL( &( px_جریان_میانگیر->x_جریان_میانگیر_قفل ) );
            #پایان_اگر /* configNUMBER_OF_CORES > 1 */
            {
                px_جریان_میانگیر->x_Task_انتظار_مقصد_Receive = عدم;
            }
            #إذا ( config_عدد_از_CORES > 1 )
                taskEXIT_CRITICAL( &( px_جریان_میانگیر->x_جریان_میانگیر_قفل ) );
            #پایان_اگر /* configNUMBER_OF_CORES > 1 */

            /* Recheck the data available after blocking. */
            x_بایتها_موجود = prv_بایتها_درون_میانگیر( px_جریان_میانگیر );
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    وإلا
    {
        x_بایتها_موجود = prv_بایتها_درون_میانگیر( px_جریان_میانگیر );
    }

    /* Whether receiving a discrete message (where xBytesToStoreMessageLength
     * holds the number of bytes used to store the message length) or a stream of
     * bytes (where xBytesToStoreMessageLength is zero), the number of bytes
     * available must be greater than xBytesToStoreMessageLength to be able to
     * read bytes from the buffer. */
    إذا( x_بایتها_موجود > x_بایتها_مقصد_Store_پیام_درازی )
    {
        x_Received_درازی = prv_Read_پیام_From_میانگیر( px_جریان_میانگیر, pv_Rx_داده, x_میانگیر_درازی_بایتها, x_بایتها_موجود );

        /* Was a task waiting for space in the buffer? */
        إذا( x_Received_درازی != ( size_t ) 0 )
        {
            trace_جریان_میانگیر_RECEIVE( x_جریان_میانگیر, x_Received_درازی );
            prvRECEIVE_COMPLETED( x_جریان_میانگیر );
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    وإلا
    {
        trace_جریان_میانگیر_RECEIVE_FAILED( x_جریان_میانگیر );
        mtCOVERAGE_TEST_MARKER();
    }

    أرجع x_Received_درازی;
}
/*-----------------------------------------------------------*/

size_t x_جریان_میانگیر_بعدی_پیام_درازی_بایتها( جریان_میانگیر_Handle_t x_جریان_میانگیر )
{
    جریان_میانگیر_t * ثابت px_جریان_میانگیر = x_جریان_میانگیر;
    size_t xReturn, x_بایتها_موجود;
    config_پیام_میانگیر_درازی_TYPE x_موقت_Return;

    configASSERT( px_جریان_میانگیر );

    /* Ensure the stream buffer is being used as a message buffer. */
    إذا( ( px_جریان_میانگیر->ucFlags & sb_FLAGS_IS_پیام_میانگیر ) != ( uint8_t ) 0 )
    {
        x_بایتها_موجود = prv_بایتها_درون_میانگیر( px_جریان_میانگیر );

        إذا( x_بایتها_موجود > sb_بایتها_مقصد_STORE_پیام_درازی )
        {
            /* The number of bytes available is greater than the number of bytes
             * required to hold the length of the next message, so another message
             * is available. */
            ( فراغ ) prv_Read_بایتها_From_میانگیر( px_جریان_میانگیر, ( uint8_t * ) &x_موقت_Return, sb_بایتها_مقصد_STORE_پیام_درازی, px_جریان_میانگیر->x_دم );
            xReturn = ( size_t ) x_موقت_Return;
        }
        وإلا
        {
            /* The minimum amount of bytes in a message buffer is
             * ( sbBYTES_TO_STORE_MESSAGE_LENGTH + 1 ), so if xBytesAvailable is
             * less than sbBYTES_TO_STORE_MESSAGE_LENGTH the only other valid
             * value is 0. */
            configASSERT( x_بایتها_موجود == 0 );
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

size_t x_جریان_میانگیر_Receive_From_ISR( جریان_میانگیر_Handle_t x_جریان_میانگیر,
                                    فراغ * pv_Rx_داده,
                                    size_t x_میانگیر_درازی_بایتها,
                                    پایه_Type_t * ثابت px_Higher_اولویت_Task_Woken )
{
    جریان_میانگیر_t * ثابت px_جریان_میانگیر = x_جریان_میانگیر;
    size_t x_Received_درازی = 0, x_بایتها_موجود, x_بایتها_مقصد_Store_پیام_درازی;

    configASSERT( pv_Rx_داده );
    configASSERT( px_جریان_میانگیر );

    /* This receive function is used by both message buffers, which store
     * discrete messages, and stream buffers, which store a continuous stream of
     * bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the
     * message. */
    إذا( ( px_جریان_میانگیر->ucFlags & sb_FLAGS_IS_پیام_میانگیر ) != ( uint8_t ) 0 )
    {
        x_بایتها_مقصد_Store_پیام_درازی = sb_بایتها_مقصد_STORE_پیام_درازی;
    }
    وإلا
    {
        x_بایتها_مقصد_Store_پیام_درازی = 0;
    }

    x_بایتها_موجود = prv_بایتها_درون_میانگیر( px_جریان_میانگیر );

    /* Whether receiving a discrete message (where xBytesToStoreMessageLength
     * holds the number of bytes used to store the message length) or a stream of
     * bytes (where xBytesToStoreMessageLength is zero), the number of bytes
     * available must be greater than xBytesToStoreMessageLength to be able to
     * read bytes from the buffer. */
    إذا( x_بایتها_موجود > x_بایتها_مقصد_Store_پیام_درازی )
    {
        x_Received_درازی = prv_Read_پیام_From_میانگیر( px_جریان_میانگیر, pv_Rx_داده, x_میانگیر_درازی_بایتها, x_بایتها_موجود );

        /* Was a task waiting for space in the buffer? */
        إذا( x_Received_درازی != ( size_t ) 0 )
        {
            prvRECEIVE_COMPLETED_FROM_ISR( px_جریان_میانگیر, px_Higher_اولویت_Task_Woken );
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

    trace_جریان_میانگیر_RECEIVE_FROM_ISR( x_جریان_میانگیر, x_Received_درازی );

    أرجع x_Received_درازی;
}
/*-----------------------------------------------------------*/

ساكن size_t prv_Read_پیام_From_میانگیر( جریان_میانگیر_t * px_جریان_میانگیر,
                                        فراغ * pv_Rx_داده,
                                        size_t x_میانگیر_درازی_بایتها,
                                        size_t x_بایتها_موجود )
{
    size_t x_شمار, x_بعدی_پیام_درازی;
    config_پیام_میانگیر_درازی_TYPE x_موقت_بعدی_پیام_درازی;
    size_t x_بعدی_دم = px_جریان_میانگیر->x_دم;

    إذا( ( px_جریان_میانگیر->ucFlags & sb_FLAGS_IS_پیام_میانگیر ) != ( uint8_t ) 0 )
    {
        /* A discrete message is being received.  First receive the length
         * of the message. */
        x_بعدی_دم = prv_Read_بایتها_From_میانگیر( px_جریان_میانگیر, ( uint8_t * ) &x_موقت_بعدی_پیام_درازی, sb_بایتها_مقصد_STORE_پیام_درازی, x_بعدی_دم );
        x_بعدی_پیام_درازی = ( size_t ) x_موقت_بعدی_پیام_درازی;

        /* Reduce the number of bytes available by the number of bytes just
         * read out. */
        x_بایتها_موجود -= sb_بایتها_مقصد_STORE_پیام_درازی;

        /* Check there is enough space in the buffer provided by the
         * user. */
        إذا( x_بعدی_پیام_درازی > x_میانگیر_درازی_بایتها )
        {
            /* The user has provided insufficient space to read the message. */
            x_بعدی_پیام_درازی = 0;
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
        x_بعدی_پیام_درازی = x_میانگیر_درازی_بایتها;
    }

    /* Use the minimum of the wanted bytes and the available bytes. */
    x_شمار = configMIN( x_بعدی_پیام_درازی, x_بایتها_موجود );

    إذا( x_شمار != ( size_t ) 0 )
    {
        /* Read the actual data and update the tail to mark the data as officially consumed. */
        px_جریان_میانگیر->x_دم = prv_Read_بایتها_From_میانگیر( px_جریان_میانگیر, ( uint8_t * ) pv_Rx_داده, x_شمار, x_بعدی_دم ); /*lint !e9079 Data storage area is implemented as uint8_t array for ease of sizing, indexing and alignment. */
    }

    أرجع x_شمار;
}
/*-----------------------------------------------------------*/

پایه_Type_t x_جریان_میانگیر_Is_خالی( جریان_میانگیر_Handle_t x_جریان_میانگیر )
{
    ثابت جریان_میانگیر_t * ثابت px_جریان_میانگیر = x_جریان_میانگیر;
    پایه_Type_t xReturn;
    size_t x_دم;

    configASSERT( px_جریان_میانگیر );

    /* True if no bytes are available. */
    x_دم = px_جریان_میانگیر->x_دم;

    إذا( px_جریان_میانگیر->x_سر == x_دم )
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

پایه_Type_t x_جریان_میانگیر_Is_پر( جریان_میانگیر_Handle_t x_جریان_میانگیر )
{
    پایه_Type_t xReturn;
    size_t x_بایتها_مقصد_Store_پیام_درازی;
    ثابت جریان_میانگیر_t * ثابت px_جریان_میانگیر = x_جریان_میانگیر;

    configASSERT( px_جریان_میانگیر );

    /* This generic version of the receive function is used by both message
     * buffers, which store discrete messages, and stream buffers, which store a
     * continuous stream of bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the message. */
    إذا( ( px_جریان_میانگیر->ucFlags & sb_FLAGS_IS_پیام_میانگیر ) != ( uint8_t ) 0 )
    {
        x_بایتها_مقصد_Store_پیام_درازی = sb_بایتها_مقصد_STORE_پیام_درازی;
    }
    وإلا
    {
        x_بایتها_مقصد_Store_پیام_درازی = 0;
    }

    /* True if the available space equals zero. */
    إذا( x_جریان_میانگیر_Spaces_موجود( x_جریان_میانگیر ) <= x_بایتها_مقصد_Store_پیام_درازی )
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

پایه_Type_t x_جریان_میانگیر_Send_Completed_From_ISR( جریان_میانگیر_Handle_t x_جریان_میانگیر,
                                              پایه_Type_t * px_Higher_اولویت_Task_Woken )
{
    جریان_میانگیر_t * ثابت px_جریان_میانگیر = x_جریان_میانگیر;
    پایه_Type_t xReturn;
    UBaseType_t ux_Saved_وقفه_وضعیت;

    configASSERT( px_جریان_میانگیر );

    ux_Saved_وقفه_وضعیت = ( UBaseType_t ) پورت_مجموعه_وقفه_ماسک_FROM_ISR();
    {
        إذا( ( px_جریان_میانگیر )->x_Task_انتظار_مقصد_Receive != عدم )
        {
            ( فراغ ) xTaskNotifyFromISR( ( px_جریان_میانگیر )->x_Task_انتظار_مقصد_Receive,
                                         ( uint32_t ) 0,
                                         eNoAction,
                                         px_Higher_اولویت_Task_Woken );
            ( px_جریان_میانگیر )->x_Task_انتظار_مقصد_Receive = عدم;
            xReturn = pdTRUE;
        }
        وإلا
        {
            xReturn = pdFALSE;
        }
    }
    پورت_CLEAR_وقفه_ماسک_FROM_ISR( ux_Saved_وقفه_وضعیت );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

پایه_Type_t x_جریان_میانگیر_Receive_Completed_From_ISR( جریان_میانگیر_Handle_t x_جریان_میانگیر,
                                                 پایه_Type_t * px_Higher_اولویت_Task_Woken )
{
    جریان_میانگیر_t * ثابت px_جریان_میانگیر = x_جریان_میانگیر;
    پایه_Type_t xReturn;
    UBaseType_t ux_Saved_وقفه_وضعیت;

    configASSERT( px_جریان_میانگیر );

    ux_Saved_وقفه_وضعیت = ( UBaseType_t ) پورت_مجموعه_وقفه_ماسک_FROM_ISR();
    {
        إذا( ( px_جریان_میانگیر )->x_Task_انتظار_مقصد_Send != عدم )
        {
            ( فراغ ) xTaskNotifyFromISR( ( px_جریان_میانگیر )->x_Task_انتظار_مقصد_Send,
                                         ( uint32_t ) 0,
                                         eNoAction,
                                         px_Higher_اولویت_Task_Woken );
            ( px_جریان_میانگیر )->x_Task_انتظار_مقصد_Send = عدم;
            xReturn = pdTRUE;
        }
        وإلا
        {
            xReturn = pdFALSE;
        }
    }
    پورت_CLEAR_وقفه_ماسک_FROM_ISR( ux_Saved_وقفه_وضعیت );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

ساكن size_t prv_Write_بایتها_مقصد_میانگیر( جریان_میانگیر_t * ثابت px_جریان_میانگیر,
                                     ثابت uint8_t * puc_داده,
                                     size_t x_شمار,
                                     size_t x_سر )
{
    size_t x_اولی_درازی;

    configASSERT( x_شمار > ( size_t ) 0 );

    /* Calculate the number of bytes that can be added in the first write -
     * which may be less than the total number of bytes that need to be added if
     * the buffer will wrap back to the beginning. */
    x_اولی_درازی = configMIN( px_جریان_میانگیر->x_درازی - x_سر, x_شمار );

    /* Write as many bytes as can be written in the first write. */
    configASSERT( ( x_سر + x_اولی_درازی ) <= px_جریان_میانگیر->x_درازی );
    ( فراغ ) کپی_حافظه( ( فراغ * ) ( &( px_جریان_میانگیر->puc_میانگیر[ x_سر ] ) ), ( ثابت فراغ * ) puc_داده, x_اولی_درازی ); /*lint !e9087 memcpy() requires void *. */

    /* If the number of bytes written was less than the number that could be
     * written in the first write... */
    إذا( x_شمار > x_اولی_درازی )
    {
        /* ...then write the remaining bytes to the start of the buffer. */
        configASSERT( ( x_شمار - x_اولی_درازی ) <= px_جریان_میانگیر->x_درازی );
        ( فراغ ) کپی_حافظه( ( فراغ * ) px_جریان_میانگیر->puc_میانگیر, ( ثابت فراغ * ) &( puc_داده[ x_اولی_درازی ] ), x_شمار - x_اولی_درازی ); /*lint !e9087 memcpy() requires void *. */
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    x_سر += x_شمار;

    إذا( x_سر >= px_جریان_میانگیر->x_درازی )
    {
        x_سر -= px_جریان_میانگیر->x_درازی;
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    أرجع x_سر;
}
/*-----------------------------------------------------------*/

ساكن size_t prv_Read_بایتها_From_میانگیر( جریان_میانگیر_t * px_جریان_میانگیر,
                                      uint8_t * puc_داده,
                                      size_t x_شمار,
                                      size_t x_دم )
{
    size_t x_اولی_درازی;

    configASSERT( x_شمار != ( size_t ) 0 );

    /* Calculate the number of bytes that can be read - which may be
     * less than the number wanted if the data wraps around to the start of
     * the buffer. */
    x_اولی_درازی = configMIN( px_جریان_میانگیر->x_درازی - x_دم, x_شمار );

    /* Obtain the number of bytes it is possible to obtain in the first
     * read.  Asserts check bounds of read and write. */
    configASSERT( x_اولی_درازی <= x_شمار );
    configASSERT( ( x_دم + x_اولی_درازی ) <= px_جریان_میانگیر->x_درازی );
    ( فراغ ) کپی_حافظه( ( فراغ * ) puc_داده, ( ثابت فراغ * ) &( px_جریان_میانگیر->puc_میانگیر[ x_دم ] ), x_اولی_درازی ); /*lint !e9087 memcpy() requires void *. */

    /* If the total number of wanted bytes is greater than the number
     * that could be read in the first read... */
    إذا( x_شمار > x_اولی_درازی )
    {
        /* ...then read the remaining bytes from the start of the buffer. */
        ( فراغ ) کپی_حافظه( ( فراغ * ) &( puc_داده[ x_اولی_درازی ] ), ( فراغ * ) ( px_جریان_میانگیر->puc_میانگیر ), x_شمار - x_اولی_درازی ); /*lint !e9087 memcpy() requires void *. */
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    /* Move the tail pointer to effectively remove the data read from the buffer. */
    x_دم += x_شمار;

    إذا( x_دم >= px_جریان_میانگیر->x_درازی )
    {
        x_دم -= px_جریان_میانگیر->x_درازی;
    }

    أرجع x_دم;
}
/*-----------------------------------------------------------*/

ساكن size_t prv_بایتها_درون_میانگیر( ثابت جریان_میانگیر_t * ثابت px_جریان_میانگیر )
{
/* Returns the distance between xTail and xHead. */
    size_t x_شمار;

    x_شمار = px_جریان_میانگیر->x_درازی + px_جریان_میانگیر->x_سر;
    x_شمار -= px_جریان_میانگیر->x_دم;

    إذا( x_شمار >= px_جریان_میانگیر->x_درازی )
    {
        x_شمار -= px_جریان_میانگیر->x_درازی;
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    أرجع x_شمار;
}
/*-----------------------------------------------------------*/

ساكن فراغ prv_Initialise_New_جریان_میانگیر( جریان_میانگیر_t * ثابت px_جریان_میانگیر,
                                          uint8_t * ثابت puc_میانگیر,
                                          size_t x_میانگیر_اندازه_بایتها,
                                          size_t x_Trigger_Level_بایتها,
                                          uint8_t ucFlags,
                                          جریان_میانگیر_Callback_Function_t pxSendCompletedCallback,
                                          جریان_میانگیر_Callback_Function_t pxReceiveCompletedCallback )
{
    /* Assert here is deliberately writing to the entire buffer to ensure it can
     * be written to without generating exceptions, and is setting the buffer to a
     * known value to assist in development/debugging. */
    #إذا ( configASSERT_DEFINED == 1 )
    {
        /* The value written just has to be identifiable when looking at the
         * memory.  Don't use 0xA5 as that is the stack fill value and could
         * result in confusion as to what is actually being observed. */
        ثابت پایه_Type_t x_Write_مقدار = 0x55;
        configASSERT( memset( puc_میانگیر, ( صحيح ) x_Write_مقدار, x_میانگیر_اندازه_بایتها ) == puc_میانگیر );
        ( فراغ ) x_Write_مقدار;
    } /*lint !e529 !e438 xWriteValue is only used if configASSERT() is defined. */
    #پایان_اگر

    /* This function could be called from xStreamBufferReset(), so we reset the
     * stream buffer fields manually in order to avoid clearing
     * xStreamBufferLock. The xStreamBufferLock is initialized separately on
     * stream buffer creation. */
    px_جریان_میانگیر->x_دم = ( size_t ) 0;
    px_جریان_میانگیر->x_سر = ( size_t ) 0;
    px_جریان_میانگیر->x_Task_انتظار_مقصد_Receive = ( TaskHandle_t ) 0;
    px_جریان_میانگیر->x_Task_انتظار_مقصد_Send = ( TaskHandle_t ) 0;
    #إذا ( configUSE_TRACE_FACILITY == 1 )
        px_جریان_میانگیر->ux_جریان_میانگیر_عدد = ( UBaseType_t ) 0;
    #پایان_اگر
    px_جریان_میانگیر->puc_میانگیر = puc_میانگیر;
    px_جریان_میانگیر->x_درازی = x_میانگیر_اندازه_بایتها;
    px_جریان_میانگیر->x_Trigger_Level_بایتها = x_Trigger_Level_بایتها;
    px_جریان_میانگیر->ucFlags = ucFlags;
    #إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    {
        px_جریان_میانگیر->pxSendCompletedCallback = pxSendCompletedCallback;
        px_جریان_میانگیر->pxReceiveCompletedCallback = pxReceiveCompletedCallback;
    }
    #وإلا
    {
        ( فراغ ) pxSendCompletedCallback;
        ( فراغ ) pxReceiveCompletedCallback;
    }
    #پایان_اگر
}

#إذا ( configUSE_TRACE_FACILITY == 1 )

    UBaseType_t ux_جریان_میانگیر_Get_جریان_میانگیر_عدد( جریان_میانگیر_Handle_t x_جریان_میانگیر )
    {
        أرجع x_جریان_میانگیر->ux_جریان_میانگیر_عدد;
    }

#پایان_اگر /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#إذا ( configUSE_TRACE_FACILITY == 1 )

    فراغ v_جریان_میانگیر_مجموعه_جریان_میانگیر_عدد( جریان_میانگیر_Handle_t x_جریان_میانگیر,
                                             UBaseType_t ux_جریان_میانگیر_عدد )
    {
        x_جریان_میانگیر->ux_جریان_میانگیر_عدد = ux_جریان_میانگیر_عدد;
    }

#پایان_اگر /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#إذا ( configUSE_TRACE_FACILITY == 1 )

    uint8_t uc_جریان_میانگیر_Get_جریان_میانگیر_Type( جریان_میانگیر_Handle_t x_جریان_میانگیر )
    {
        أرجع( x_جریان_میانگیر->ucFlags & sb_FLAGS_IS_پیام_میانگیر );
    }

#پایان_اگر /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/
