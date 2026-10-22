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
#تعریف_کړه MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* FreeRTOS includes. */
#ضمن "FreeRTOS.h"
#ضمن "task.h"
#ضمن "stream_buffer.h"
/* Include private IDF API additions for critical thread safety macros */
#ضمن "esp_private/freertos_idf_additions_priv.h"

#إذا ( configUSE_TASK_NOTIFICATIONS != 1 )
    #تېروتنه configUSE_TASK_NOTIFICATIONS must be سېټ منزل 1 منزل build بهیر_بفر.c
#د_که_پای

#إذا ( INCLUDE_xTaskGetCurrentTaskHandle != 1 )
    #تېروتنه INCLUDE_xTaskGetCurrentTaskHandle must be سېټ منزل 1 منزل build بهیر_بفر.c
#د_که_پای

/* Lint e961, e9021 and e750 are suppressed as a MISRA exception justified
 * because the MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#تعریف_لغوه MPU_WRAPPERS_INCLUDED_FROM_API_FILE /*lint !e961 !e750 !e9021. */

/* If the user has not provided application specific Rx notification macros,
 * or #defined the notification macros away, then provide default implementations
 * that uses task notifications. */
/*lint -save -e9026 Function like macros allowed and needed here so they can be overridden. */
#که_نه_تعریف_شوی sbRECEIVE_COMPLETED
    #تعریف_کړه sbRECEIVE_COMPLETED( px_بهیر_بفر )                               \
    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( px_بهیر_بفر->x_بهیر_بفر_قلف ) ); \
    {                                                                           \
        إذا( ( px_بهیر_بفر )->x_Task_انتظار_منزل_Send != عدم )                    \
        {                                                                       \
            ( فراغ ) xTaskNotify( ( px_بهیر_بفر )->x_Task_انتظار_منزل_Send,       \
                                  ( uint32_t ) 0,                               \
                                  eNoAction );                                  \
            ( px_بهیر_بفر )->x_Task_انتظار_منزل_Send = عدم;                      \
        }                                                                       \
    }                                                                           \
    ( فراغ ) prvEXIT_CRITICAL_OR_RESUME_ALL( &( px_بهیر_بفر->x_بهیر_بفر_قلف ) );
#د_که_پای /* sbRECEIVE_COMPLETED */

/* If user has provided a per-instance receive complete callback, then
 * invoke the callback else use the receive complete macro which is provided by default for all instances.
 */
#إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #تعریف_کړه prvRECEIVE_COMPLETED( px_بهیر_بفر )                                               \
    {                                                                                            \
        إذا( ( px_بهیر_بفر )->pxReceiveCompletedCallback != عدم )                             \
        {                                                                                        \
            ( px_بهیر_بفر )->pxReceiveCompletedCallback( ( px_بهیر_بفر ), pdFALSE, عدم ); \
        }                                                                                        \
        وإلا                                                                                     \
        {                                                                                        \
            sbRECEIVE_COMPLETED( ( px_بهیر_بفر ) );                                           \
        }                                                                                        \
    }
#وإلا /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #تعریف_کړه prvRECEIVE_COMPLETED( px_بهیر_بفر )    sbRECEIVE_COMPLETED( ( px_بهیر_بفر ) )
#د_که_پای /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

#که_نه_تعریف_شوی sbRECEIVE_COMPLETED_FROM_ISR
    #تعریف_کړه sbRECEIVE_COMPLETED_FROM_ISR( px_بهیر_بفر,                            \
                                          px_Higher_لومړیتوب_Task_Woken )                \
    {                                                                                \
        UBaseType_t ux_Saved_مداخله_حالت;                                          \
                                                                                     \
        ux_Saved_مداخله_حالت = ( UBaseType_t ) پورټ_سېټ_مداخله_ماسک_FROM_ISR();  \
        {                                                                            \
            إذا( ( px_بهیر_بفر )->x_Task_انتظار_منزل_Send != عدم )                     \
            {                                                                        \
                ( فراغ ) xTaskNotifyFromISR( ( px_بهیر_بفر )->x_Task_انتظار_منزل_Send, \
                                             ( uint32_t ) 0,                         \
                                             eNoAction,                              \
                                             ( px_Higher_لومړیتوب_Task_Woken ) );        \
                ( px_بهیر_بفر )->x_Task_انتظار_منزل_Send = عدم;                       \
            }                                                                        \
        }                                                                            \
        پورټ_CLEAR_مداخله_ماسک_FROM_ISR( ux_Saved_مداخله_حالت );                 \
    }
#د_که_پای /* sbRECEIVE_COMPLETED_FROM_ISR */

#إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #تعریف_کړه prvRECEIVE_COMPLETED_FROM_ISR( px_بهیر_بفر,                                                               \
                                           px_Higher_لومړیتوب_Task_Woken )                                                   \
    {                                                                                                                    \
        إذا( ( px_بهیر_بفر )->pxReceiveCompletedCallback != عدم )                                                     \
        {                                                                                                                \
            ( px_بهیر_بفر )->pxReceiveCompletedCallback( ( px_بهیر_بفر ), pdTRUE, ( px_Higher_لومړیتوب_Task_Woken ) ); \
        }                                                                                                                \
        وإلا                                                                                                             \
        {                                                                                                                \
            sbRECEIVE_COMPLETED_FROM_ISR( ( px_بهیر_بفر ), ( px_Higher_لومړیتوب_Task_Woken ) );                           \
        }                                                                                                                \
    }
#وإلا /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #تعریف_کړه prvRECEIVE_COMPLETED_FROM_ISR( px_بهیر_بفر, px_Higher_لومړیتوب_Task_Woken ) \
    sbRECEIVE_COMPLETED_FROM_ISR( ( px_بهیر_بفر ), ( px_Higher_لومړیتوب_Task_Woken ) )
#د_که_پای /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

/* If the user has not provided an application specific Tx notification macro,
 * or #defined the notification macro away, then provide a default
 * implementation that uses task notifications.
 */
#که_نه_تعریف_شوی sbSEND_COMPLETED
    #تعریف_کړه sbSEND_COMPLETED( px_بهیر_بفر )                                  \
    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( px_بهیر_بفر->x_بهیر_بفر_قلف ) ); \
    {                                                                           \
        إذا( ( px_بهیر_بفر )->x_Task_انتظار_منزل_Receive != عدم )                 \
        {                                                                       \
            ( فراغ ) xTaskNotify( ( px_بهیر_بفر )->x_Task_انتظار_منزل_Receive,    \
                                  ( uint32_t ) 0,                               \
                                  eNoAction );                                  \
            ( px_بهیر_بفر )->x_Task_انتظار_منزل_Receive = عدم;                   \
        }                                                                       \
    }                                                                           \
    ( فراغ ) prvEXIT_CRITICAL_OR_RESUME_ALL( &( px_بهیر_بفر->x_بهیر_بفر_قلف ) );
#د_که_پای /* sbSEND_COMPLETED */

/* If user has provided a per-instance send completed callback, then
 * invoke the callback else use the send complete macro which is provided by default for all instances.
 */
#إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #تعریف_کړه prvSEND_COMPLETED( px_بهیر_بفر )                                           \
    {                                                                                     \
        إذا( ( px_بهیر_بفر )->pxSendCompletedCallback != عدم )                         \
        {                                                                                 \
            px_بهیر_بفر->pxSendCompletedCallback( ( px_بهیر_بفر ), pdFALSE, عدم ); \
        }                                                                                 \
        وإلا                                                                              \
        {                                                                                 \
            sbSEND_COMPLETED( ( px_بهیر_بفر ) );                                       \
        }                                                                                 \
    }
#وإلا /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #تعریف_کړه prvSEND_COMPLETED( px_بهیر_بفر )    sbSEND_COMPLETED( ( px_بهیر_بفر ) )
#د_که_پای /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */


#که_نه_تعریف_شوی sbSEND_COMPLETE_FROM_ISR
    #تعریف_کړه sbSEND_COMPLETE_FROM_ISR( px_بهیر_بفر, px_Higher_لومړیتوب_Task_Woken )       \
    {                                                                                   \
        UBaseType_t ux_Saved_مداخله_حالت;                                             \
                                                                                        \
        ux_Saved_مداخله_حالت = ( UBaseType_t ) پورټ_سېټ_مداخله_ماسک_FROM_ISR();     \
        {                                                                               \
            إذا( ( px_بهیر_بفر )->x_Task_انتظار_منزل_Receive != عدم )                     \
            {                                                                           \
                ( فراغ ) xTaskNotifyFromISR( ( px_بهیر_بفر )->x_Task_انتظار_منزل_Receive, \
                                             ( uint32_t ) 0,                            \
                                             eNoAction,                                 \
                                             ( px_Higher_لومړیتوب_Task_Woken ) );           \
                ( px_بهیر_بفر )->x_Task_انتظار_منزل_Receive = عدم;                       \
            }                                                                           \
        }                                                                               \
        پورټ_CLEAR_مداخله_ماسک_FROM_ISR( ux_Saved_مداخله_حالت );                    \
    }
#د_که_پای /* sbSEND_COMPLETE_FROM_ISR */


#إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #تعریف_کړه prvSEND_COMPLETE_FROM_ISR( px_بهیر_بفر, px_Higher_لومړیتوب_Task_Woken )                                    \
    {                                                                                                                 \
        إذا( ( px_بهیر_بفر )->pxSendCompletedCallback != عدم )                                                     \
        {                                                                                                             \
            ( px_بهیر_بفر )->pxSendCompletedCallback( ( px_بهیر_بفر ), pdTRUE, ( px_Higher_لومړیتوب_Task_Woken ) ); \
        }                                                                                                             \
        وإلا                                                                                                          \
        {                                                                                                             \
            sbSEND_COMPLETE_FROM_ISR( ( px_بهیر_بفر ), ( px_Higher_لومړیتوب_Task_Woken ) );                            \
        }                                                                                                             \
    }
#وإلا /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #تعریف_کړه prvSEND_COMPLETE_FROM_ISR( px_بهیر_بفر, px_Higher_لومړیتوب_Task_Woken ) \
    sbSEND_COMPLETE_FROM_ISR( ( px_بهیر_بفر ), ( px_Higher_لومړیتوب_Task_Woken ) )
#د_که_پای /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

/*lint -restore (9026) */

/* The number of bytes used to hold the length of a message in the buffer. */
#تعریف_کړه sb_بایټونه_منزل_STORE_پیغام_اوږدتیا    ( حجم( config_پیغام_بفر_اوږدتیا_TYPE ) )

/* Bits stored in the ucFlags field of the stream buffer. */
#تعریف_کړه sb_FLAGS_IS_پیغام_بفر          ( ( uint8_t ) 1 ) /* Set if the stream buffer was created as a message buffer, in which case it holds discrete messages rather than a stream. */
#تعریف_کړه sbFLAGS_IS_STATICALLY_ALLOCATED    ( ( uint8_t ) 2 ) /* Set if the stream buffer was created using statically allocated memory. */

/*-----------------------------------------------------------*/

/* Structure that hold state information on the buffer. */
تعريفنوع بنية بهیر_بفر_Def_t                 /*lint !e9058 Style convention uses tag. */
{
    متطاير size_t x_لکۍ;                       /* Index to the next item to read within the buffer. */
    متطاير size_t x_سر;                       /* Index to the next item to write within the buffer. */
    size_t x_اوږدتیا;                              /* The length of the buffer pointed to by pucBuffer. */
    size_t x_Trigger_Level_بایټونه;                   /* The number of bytes that must be in the stream buffer before a task that is waiting for data is unblocked. */
    متطاير TaskHandle_t x_Task_انتظار_منزل_Receive; /* Holds the handle of a task waiting for data, or NULL if no tasks are waiting. */
    متطاير TaskHandle_t x_Task_انتظار_منزل_Send;    /* Holds the handle of a task waiting to send data to a message buffer that is full. */
    uint8_t * puc_بفر;                         /* Points to the buffer itself - that is - the RAM that stores the data passed through the buffer. */
    uint8_t ucFlags;

    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_بهیر_بفر_شمېره; /* Used for tracing purposes. */
    #د_که_پای

    #إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        بهیر_بفر_Callback_Function_t pxSendCompletedCallback;    /* Optional callback called on send complete. sbSEND_COMPLETED is called if this is NULL. */
        بهیر_بفر_Callback_Function_t pxReceiveCompletedCallback; /* Optional callback called on receive complete.  sbRECEIVE_COMPLETED is called if this is NULL. */
    #د_که_پای

    پورټ_MUX_TYPE x_بهیر_بفر_قلف; /* Spinlock required for SMP critical sections */
} بهیر_بفر_t;

/*
 * The number of bytes available to be read from the buffer.
 */
ساكن size_t prv_بایټونه_دننه_بفر( ثابت بهیر_بفر_t * ثابت px_بهیر_بفر ) PRIVILEGED_FUNCTION;

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
ساكن size_t prv_Write_بایټونه_منزل_بفر( بهیر_بفر_t * ثابت px_بهیر_بفر,
                                     ثابت uint8_t * puc_معلومات,
                                     size_t x_شمېر,
                                     size_t x_سر ) PRIVILEGED_FUNCTION;

/*
 * If the stream buffer is being used as a message buffer, then reads an entire
 * message out of the buffer.  If the stream buffer is being used as a stream
 * buffer then read as many bytes as possible from the buffer.
 * prvReadBytesFromBuffer() is called to actually extract the bytes from the
 * buffer's data storage area.
 */
ساكن size_t prv_Read_پیغام_From_بفر( بهیر_بفر_t * px_بهیر_بفر,
                                        فراغ * pv_Rx_معلومات,
                                        size_t x_بفر_اوږدتیا_بایټونه,
                                        size_t x_بایټونه_شته ) PRIVILEGED_FUNCTION;

/*
 * If the stream buffer is being used as a message buffer, then writes an entire
 * message to the buffer.  If the stream buffer is being used as a stream
 * buffer then write as many bytes as possible to the buffer.
 * prvWriteBytestoBuffer() is called to actually send the bytes to the buffer's
 * data storage area.
 */
ساكن size_t prv_Write_پیغام_منزل_بفر( بهیر_بفر_t * ثابت px_بهیر_بفر,
                                       ثابت فراغ * pv_Tx_معلومات,
                                       size_t x_معلومات_اوږدتیا_بایټونه,
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
ساكن size_t prv_Read_بایټونه_From_بفر( بهیر_بفر_t * px_بهیر_بفر,
                                      uint8_t * puc_معلومات,
                                      size_t x_شمېر,
                                      size_t x_لکۍ ) PRIVILEGED_FUNCTION;

/*
 * Called by both pxStreamBufferCreate() and pxStreamBufferCreateStatic() to
 * initialise the members of the newly created stream buffer structure.
 */
ساكن فراغ prv_Initialise_New_بهیر_بفر( بهیر_بفر_t * ثابت px_بهیر_بفر,
                                          uint8_t * ثابت puc_بفر,
                                          size_t x_بفر_اندازه_بایټونه,
                                          size_t x_Trigger_Level_بایټونه,
                                          uint8_t ucFlags,
                                          بهیر_بفر_Callback_Function_t pxSendCompletedCallback,
                                          بهیر_بفر_Callback_Function_t pxReceiveCompletedCallback ) PRIVILEGED_FUNCTION;

/*-----------------------------------------------------------*/
#إذا ( config_SUPPORT_DYNAMIC_ویش == 1 )
    بهیر_بفر_Handle_t x_بهیر_بفر_Generic_Create( size_t x_بفر_اندازه_بایټونه,
                                                     size_t x_Trigger_Level_بایټونه,
                                                     بنسټ_Type_t x_Is_پیغام_بفر,
                                                     بهیر_بفر_Callback_Function_t pxSendCompletedCallback,
                                                     بهیر_بفر_Callback_Function_t pxReceiveCompletedCallback )
    {
        uint8_t * puc_Allocated_حافظه;
        uint8_t ucFlags;

        /* In case the stream buffer is going to be used as a message buffer
         * (that is, it will hold discrete messages with a little meta data that
         * says how big the next message is) check the buffer will be large enough
         * to hold at least one message. */
        إذا( x_Is_پیغام_بفر == pdTRUE )
        {
            /* Is a message buffer but not statically allocated. */
            ucFlags = sb_FLAGS_IS_پیغام_بفر;
            configASSERT( x_بفر_اندازه_بایټونه > sb_بایټونه_منزل_STORE_پیغام_اوږدتیا );
        }
        وإلا
        {
            /* Not a message buffer and not statically allocated. */
            ucFlags = 0;
            configASSERT( x_بفر_اندازه_بایټونه > 0 );
        }

        configASSERT( x_Trigger_Level_بایټونه <= x_بفر_اندازه_بایټونه );

        /* A trigger level of 0 would cause a waiting task to unblock even when
         * the buffer was empty. */
        إذا( x_Trigger_Level_بایټونه == ( size_t ) 0 )
        {
            x_Trigger_Level_بایټونه = ( size_t ) 1;
        }

        /* A stream buffer requires a StreamBuffer_t structure and a buffer.
         * Both are allocated in a single call to pvPortMalloc().  The
         * StreamBuffer_t structure is placed at the start of the allocated memory
         * and the buffer follows immediately after.  The requested size is
         * incremented so the free space is returned as the user would expect -
         * this is a quirk of the implementation that means otherwise the free
         * space would be reported as one byte smaller than would be logically
         * expected. */
        إذا( x_بفر_اندازه_بایټونه < ( x_بفر_اندازه_بایټونه + 1 + حجم( بهیر_بفر_t ) ) )
        {
            x_بفر_اندازه_بایټونه++;
            puc_Allocated_حافظه = ( uint8_t * ) pv_پورټ_Malloc( x_بفر_اندازه_بایټونه + حجم( بهیر_بفر_t ) ); /*lint !e9079 malloc() only returns void*. */
        }
        وإلا
        {
            puc_Allocated_حافظه = عدم;
        }

        إذا( puc_Allocated_حافظه != عدم )
        {
            prv_Initialise_New_بهیر_بفر( ( بهیر_بفر_t * ) puc_Allocated_حافظه,       /* Structure at the start of the allocated memory. */ /*lint !e9087 Safe cast as allocated memory is aligned. */ /*lint !e826 Area is not too small and alignment is guaranteed provided malloc() behaves as expected and returns aligned buffer. */
                                          puc_Allocated_حافظه + حجم( بهیر_بفر_t ), /* Storage area follows. */ /*lint !e9016 Indexing past structure valid for uint8_t pointer, also storage area has no alignment requirement. */
                                          x_بفر_اندازه_بایټونه,
                                          x_Trigger_Level_بایټونه,
                                          ucFlags,
                                          pxSendCompletedCallback,
                                          pxReceiveCompletedCallback );

            /* Initialize the stream buffer's spinlock separately, as
             * prvInitialiseNewStreamBuffer() is also called from
             * xStreamBufferReset(). */
            پورټ_MUX_INITIALIZE( &( ( ( بهیر_بفر_t * ) puc_Allocated_حافظه )->x_بهیر_بفر_قلف ) );

            trace_بهیر_بفر_CREATE( ( ( بهیر_بفر_t * ) puc_Allocated_حافظه ), x_Is_پیغام_بفر );
        }
        وإلا
        {
            trace_بهیر_بفر_CREATE_FAILED( x_Is_پیغام_بفر );
        }

        أرجع ( بهیر_بفر_Handle_t ) puc_Allocated_حافظه; /*lint !e9087 !e826 Safe cast as allocated memory is aligned. */
    }
#د_که_پای /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_ویش == 1 )

    بهیر_بفر_Handle_t x_بهیر_بفر_Generic_Create_Static( size_t x_بفر_اندازه_بایټونه,
                                                           size_t x_Trigger_Level_بایټونه,
                                                           بنسټ_Type_t x_Is_پیغام_بفر,
                                                           uint8_t * ثابت puc_بهیر_بفر_Storage_Area,
                                                           Static_بهیر_بفر_t * ثابت px_Static_بهیر_بفر,
                                                           بهیر_بفر_Callback_Function_t pxSendCompletedCallback,
                                                           بهیر_بفر_Callback_Function_t pxReceiveCompletedCallback )
    {
        بهیر_بفر_t * ثابت px_بهیر_بفر = ( بهیر_بفر_t * ) px_Static_بهیر_بفر; /*lint !e740 !e9087 Safe cast as StaticStreamBuffer_t is opaque Streambuffer_t. */
        بهیر_بفر_Handle_t xReturn;
        uint8_t ucFlags;

        configASSERT( puc_بهیر_بفر_Storage_Area );
        configASSERT( px_Static_بهیر_بفر );
        configASSERT( x_Trigger_Level_بایټونه <= x_بفر_اندازه_بایټونه );

        /* A trigger level of 0 would cause a waiting task to unblock even when
         * the buffer was empty. */
        إذا( x_Trigger_Level_بایټونه == ( size_t ) 0 )
        {
            x_Trigger_Level_بایټونه = ( size_t ) 1;
        }

        إذا( x_Is_پیغام_بفر != pdFALSE )
        {
            /* Statically allocated message buffer. */
            ucFlags = sb_FLAGS_IS_پیغام_بفر | sbFLAGS_IS_STATICALLY_ALLOCATED;
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
        configASSERT( x_بفر_اندازه_بایټونه > sb_بایټونه_منزل_STORE_پیغام_اوږدتیا );

        #إذا ( configASSERT_DEFINED == 1 )
        {
            /* Sanity check that the size of the structure used to declare a
             * variable of type StaticStreamBuffer_t equals the size of the real
             * message buffer structure. */
            متطاير size_t x_اندازه = حجم( Static_بهیر_بفر_t );
            configASSERT( x_اندازه == حجم( بهیر_بفر_t ) );
            ( فراغ ) x_اندازه; /* Prevent unused variable warning when configASSERT() is not used. */
        } /*lint !e529 xSize is referenced is configASSERT() is defined. */
        #د_که_پای /* configASSERT_DEFINED */

        إذا( ( puc_بهیر_بفر_Storage_Area != عدم ) && ( px_Static_بهیر_بفر != عدم ) )
        {
            prv_Initialise_New_بهیر_بفر( px_بهیر_بفر,
                                          puc_بهیر_بفر_Storage_Area,
                                          x_بفر_اندازه_بایټونه,
                                          x_Trigger_Level_بایټونه,
                                          ucFlags,
                                          pxSendCompletedCallback,
                                          pxReceiveCompletedCallback );

            /* Remember this was statically allocated in case it is ever deleted
             * again. */
            px_بهیر_بفر->ucFlags |= sbFLAGS_IS_STATICALLY_ALLOCATED;

            /* Initialize the stream buffer's spinlock separately, as
             * prvInitialiseNewStreamBuffer() is also called from
             * xStreamBufferReset(). */
            پورټ_MUX_INITIALIZE( &( px_بهیر_بفر->x_بهیر_بفر_قلف ) );

            trace_بهیر_بفر_CREATE( px_بهیر_بفر, x_Is_پیغام_بفر );

            xReturn = ( بهیر_بفر_Handle_t ) px_Static_بهیر_بفر; /*lint !e9087 Data hiding requires cast to opaque type. */
        }
        وإلا
        {
            xReturn = عدم;
            trace_بهیر_بفر_CREATE_STATIC_FAILED( xReturn, x_Is_پیغام_بفر );
        }

        أرجع xReturn;
    }
#د_که_پای /* ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*-----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_ویش == 1 )
    بنسټ_Type_t x_بهیر_بفر_Get_Static_Buffers( بهیر_بفر_Handle_t x_بهیر_بفر,
                                              uint8_t ** ppuc_بهیر_بفر_Storage_Area,
                                              Static_بهیر_بفر_t ** ppx_Static_بهیر_بفر )
    {
        بنسټ_Type_t xReturn;
        ثابت بهیر_بفر_t * ثابت px_بهیر_بفر = x_بهیر_بفر;

        configASSERT( px_بهیر_بفر );
        configASSERT( ppuc_بهیر_بفر_Storage_Area );
        configASSERT( ppx_Static_بهیر_بفر );

        إذا( ( px_بهیر_بفر->ucFlags & sbFLAGS_IS_STATICALLY_ALLOCATED ) != ( uint8_t ) 0 )
        {
            *ppuc_بهیر_بفر_Storage_Area = px_بهیر_بفر->puc_بفر;
            *ppx_Static_بهیر_بفر = ( Static_بهیر_بفر_t * ) px_بهیر_بفر;
            xReturn = pdTRUE;
        }
        وإلا
        {
            xReturn = pdFALSE;
        }

        أرجع xReturn;
    }
#د_که_پای /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

فراغ v_بهیر_بفر_Delete( بهیر_بفر_Handle_t x_بهیر_بفر )
{
    بهیر_بفر_t * px_بهیر_بفر = x_بهیر_بفر;

    configASSERT( px_بهیر_بفر );

    trace_بهیر_بفر_DELETE( x_بهیر_بفر );

    إذا( ( px_بهیر_بفر->ucFlags & sbFLAGS_IS_STATICALLY_ALLOCATED ) == ( uint8_t ) pdFALSE )
    {
        #إذا ( config_SUPPORT_DYNAMIC_ویش == 1 )
        {
            /* Both the structure and the buffer were allocated using a single call
            * to pvPortMalloc(), hence only one call to vPortFree() is required. */
            v_پورټ_Free( ( فراغ * ) px_بهیر_بفر ); /*lint !e9087 Standard free() semantics require void *, plus pxStreamBuffer was allocated by pvPortMalloc(). */
        }
        #وإلا
        {
            /* Should not be possible to get here, ucFlags must be corrupt.
             * Force an assert. */
            configASSERT( x_بهیر_بفر == ( بهیر_بفر_Handle_t ) ~0 );
        }
        #د_که_پای
    }
    وإلا
    {
        /* The structure and buffer were not allocated dynamically and cannot be
         * freed - just scrub the structure so future use will assert. */
        ( فراغ ) memset( px_بهیر_بفر, 0x00, حجم( بهیر_بفر_t ) );
    }
}
/*-----------------------------------------------------------*/

بنسټ_Type_t x_بهیر_بفر_Reset( بهیر_بفر_Handle_t x_بهیر_بفر )
{
    بهیر_بفر_t * ثابت px_بهیر_بفر = x_بهیر_بفر;
    بنسټ_Type_t xReturn = pdFAIL;
    بهیر_بفر_Callback_Function_t pxSendCallback = عدم, pxReceiveCallback = عدم;

    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_بهیر_بفر_شمېره;
    #د_که_پای

    configASSERT( px_بهیر_بفر );

    #إذا ( configUSE_TRACE_FACILITY == 1 )
    {
        /* Store the stream buffer number so it can be restored after the
         * reset. */
        ux_بهیر_بفر_شمېره = px_بهیر_بفر->ux_بهیر_بفر_شمېره;
    }
    #د_که_پای

    /* Can only reset a message buffer if there are no tasks blocked on it. */
    taskENTER_CRITICAL( &( px_بهیر_بفر->x_بهیر_بفر_قلف ) );
    {
        إذا( ( px_بهیر_بفر->x_Task_انتظار_منزل_Receive == عدم ) && ( px_بهیر_بفر->x_Task_انتظار_منزل_Send == عدم ) )
        {
            #إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
            {
                pxSendCallback = px_بهیر_بفر->pxSendCompletedCallback;
                pxReceiveCallback = px_بهیر_بفر->pxReceiveCompletedCallback;
            }
            #د_که_پای

            prv_Initialise_New_بهیر_بفر( px_بهیر_بفر,
                                          px_بهیر_بفر->puc_بفر,
                                          px_بهیر_بفر->x_اوږدتیا,
                                          px_بهیر_بفر->x_Trigger_Level_بایټونه,
                                          px_بهیر_بفر->ucFlags,
                                          pxSendCallback,
                                          pxReceiveCallback );

            #إذا ( configUSE_TRACE_FACILITY == 1 )
            {
                px_بهیر_بفر->ux_بهیر_بفر_شمېره = ux_بهیر_بفر_شمېره;
            }
            #د_که_پای

            trace_بهیر_بفر_RESET( x_بهیر_بفر );

            xReturn = pdPASS;
        }
    }
    taskEXIT_CRITICAL( &( px_بهیر_بفر->x_بهیر_بفر_قلف ) );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

بنسټ_Type_t x_بهیر_بفر_سېټ_Trigger_Level( بهیر_بفر_Handle_t x_بهیر_بفر,
                                         size_t xTriggerLevel )
{
    بهیر_بفر_t * ثابت px_بهیر_بفر = x_بهیر_بفر;
    بنسټ_Type_t xReturn;

    configASSERT( px_بهیر_بفر );

    /* It is not valid for the trigger level to be 0. */
    إذا( xTriggerLevel == ( size_t ) 0 )
    {
        xTriggerLevel = ( size_t ) 1;
    }

    /* The trigger level is the number of bytes that must be in the stream
     * buffer before a task that is waiting for data is unblocked. */
    إذا( xTriggerLevel < px_بهیر_بفر->x_اوږدتیا )
    {
        px_بهیر_بفر->x_Trigger_Level_بایټونه = xTriggerLevel;
        xReturn = pdPASS;
    }
    وإلا
    {
        xReturn = pdFALSE;
    }

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

size_t x_بهیر_بفر_Spaces_شته( بهیر_بفر_Handle_t x_بهیر_بفر )
{
    ثابت بهیر_بفر_t * ثابت px_بهیر_بفر = x_بهیر_بفر;
    size_t xSpace;
    size_t x_Original_لکۍ;

    configASSERT( px_بهیر_بفر );

    /* The code below reads xTail and then xHead.  This is safe if the stream
     * buffer is updated once between the two reads - but not if the stream buffer
     * is updated more than once between the two reads - hence the loop. */
    افعل
    {
        x_Original_لکۍ = px_بهیر_بفر->x_لکۍ;
        xSpace = px_بهیر_بفر->x_اوږدتیا + px_بهیر_بفر->x_لکۍ;
        xSpace -= px_بهیر_بفر->x_سر;
    } طالما( x_Original_لکۍ != px_بهیر_بفر->x_لکۍ );

    xSpace -= ( size_t ) 1;

    إذا( xSpace >= px_بهیر_بفر->x_اوږدتیا )
    {
        xSpace -= px_بهیر_بفر->x_اوږدتیا;
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    أرجع xSpace;
}
/*-----------------------------------------------------------*/

size_t x_بهیر_بفر_بایټونه_شته( بهیر_بفر_Handle_t x_بهیر_بفر )
{
    ثابت بهیر_بفر_t * ثابت px_بهیر_بفر = x_بهیر_بفر;
    size_t xReturn;

    configASSERT( px_بهیر_بفر );

    xReturn = prv_بایټونه_دننه_بفر( px_بهیر_بفر );
    أرجع xReturn;
}
/*-----------------------------------------------------------*/

size_t x_بهیر_بفر_Send( بهیر_بفر_Handle_t x_بهیر_بفر,
                          ثابت فراغ * pv_Tx_معلومات,
                          size_t x_معلومات_اوږدتیا_بایټونه,
                          TickType_t x_Ticks_منزل_Wait )
{
    بهیر_بفر_t * ثابت px_بهیر_بفر = x_بهیر_بفر;
    size_t xReturn, xSpace = 0;
    size_t xRequiredSpace = x_معلومات_اوږدتیا_بایټونه;
    وخت_بهر_t x_وخت_بهر_2;
    size_t xMaxReportedSpace = 0;

    configASSERT( pv_Tx_معلومات );
    configASSERT( px_بهیر_بفر );

    /* The maximum amount of space a stream buffer will ever report is its length
     * minus 1. */
    xMaxReportedSpace = px_بهیر_بفر->x_اوږدتیا - ( size_t ) 1;

    /* This send function is used to write to both message buffers and stream
     * buffers.  If this is a message buffer then the space needed must be
     * increased by the amount of bytes needed to store the length of the
     * message. */
    إذا( ( px_بهیر_بفر->ucFlags & sb_FLAGS_IS_پیغام_بفر ) != ( uint8_t ) 0 )
    {
        xRequiredSpace += sb_بایټونه_منزل_STORE_پیغام_اوږدتیا;

        /* Overflow? */
        configASSERT( xRequiredSpace > x_معلومات_اوږدتیا_بایټونه );

        /* If this is a message buffer then it must be possible to write the
         * whole message. */
        إذا( xRequiredSpace > xMaxReportedSpace )
        {
            /* The message would not fit even if the entire buffer was empty,
             * so don't wait for space. */
            x_Ticks_منزل_Wait = ( TickType_t ) 0;
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

    إذا( x_Ticks_منزل_Wait != ( TickType_t ) 0 )
    {
        v_Task_سېټ_وخت_بهر_State( &x_وخت_بهر_2 );

        افعل
        {
            /* Wait until the required number of bytes are free in the message
             * buffer. */
            taskENTER_CRITICAL( &( px_بهیر_بفر->x_بهیر_بفر_قلف ) );
            {
                xSpace = x_بهیر_بفر_Spaces_شته( px_بهیر_بفر );

                إذا( xSpace < xRequiredSpace )
                {
                    /* Clear notification state as going to wait for space. */
                    ( فراغ ) xTaskNotifyStateClear( عدم );

                    /* Should only be one writer. */
                    configASSERT( px_بهیر_بفر->x_Task_انتظار_منزل_Send == عدم );
                    px_بهیر_بفر->x_Task_انتظار_منزل_Send = xTaskGetCurrentTaskHandle();
                }
                وإلا
                {
                    taskEXIT_CRITICAL( &( px_بهیر_بفر->x_بهیر_بفر_قلف ) );
                    اكسر;
                }
            }
            taskEXIT_CRITICAL( &( px_بهیر_بفر->x_بهیر_بفر_قلف ) );

            trace_BLOCKING_ON_بهیر_بفر_SEND( x_بهیر_بفر );
            ( فراغ ) xTaskNotifyWait( ( uint32_t ) 0, ( uint32_t ) 0, عدم, x_Ticks_منزل_Wait );
            /* In SMP mode, the task may have been woken and scheduled on
             * another core. Hence, we must clear the xTaskWaitingToSend
             * handle in a critical section. */
            #إذا ( config_شمېره_د_CORES > 1 )
                taskENTER_CRITICAL( &( px_بهیر_بفر->x_بهیر_بفر_قلف ) );
            #د_که_پای /* configNUMBER_OF_CORES > 1 */
            {
                px_بهیر_بفر->x_Task_انتظار_منزل_Send = عدم;
            }
            #إذا ( config_شمېره_د_CORES > 1 )
                taskEXIT_CRITICAL( &( px_بهیر_بفر->x_بهیر_بفر_قلف ) );
            #د_که_پای /* configNUMBER_OF_CORES > 1 */
        } طالما( x_Task_Check_For_وخت_بهر( &x_وخت_بهر_2, &x_Ticks_منزل_Wait ) == pdFALSE );
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    إذا( xSpace == ( size_t ) 0 )
    {
        xSpace = x_بهیر_بفر_Spaces_شته( px_بهیر_بفر );
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    xReturn = prv_Write_پیغام_منزل_بفر( px_بهیر_بفر, pv_Tx_معلومات, x_معلومات_اوږدتیا_بایټونه, xSpace, xRequiredSpace );

    إذا( xReturn > ( size_t ) 0 )
    {
        trace_بهیر_بفر_SEND( x_بهیر_بفر, xReturn );

        /* Was a task waiting for the data? */
        إذا( prv_بایټونه_دننه_بفر( px_بهیر_بفر ) >= px_بهیر_بفر->x_Trigger_Level_بایټونه )
        {
            prvSEND_COMPLETED( px_بهیر_بفر );
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
        trace_بهیر_بفر_SEND_FAILED( x_بهیر_بفر );
    }

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

size_t x_بهیر_بفر_Send_From_ISR( بهیر_بفر_Handle_t x_بهیر_بفر,
                                 ثابت فراغ * pv_Tx_معلومات,
                                 size_t x_معلومات_اوږدتیا_بایټونه,
                                 بنسټ_Type_t * ثابت px_Higher_لومړیتوب_Task_Woken )
{
    بهیر_بفر_t * ثابت px_بهیر_بفر = x_بهیر_بفر;
    size_t xReturn, xSpace;
    size_t xRequiredSpace = x_معلومات_اوږدتیا_بایټونه;

    configASSERT( pv_Tx_معلومات );
    configASSERT( px_بهیر_بفر );

    /* This send function is used to write to both message buffers and stream
     * buffers.  If this is a message buffer then the space needed must be
     * increased by the amount of bytes needed to store the length of the
     * message. */
    إذا( ( px_بهیر_بفر->ucFlags & sb_FLAGS_IS_پیغام_بفر ) != ( uint8_t ) 0 )
    {
        xRequiredSpace += sb_بایټونه_منزل_STORE_پیغام_اوږدتیا;
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    xSpace = x_بهیر_بفر_Spaces_شته( px_بهیر_بفر );
    xReturn = prv_Write_پیغام_منزل_بفر( px_بهیر_بفر, pv_Tx_معلومات, x_معلومات_اوږدتیا_بایټونه, xSpace, xRequiredSpace );

    إذا( xReturn > ( size_t ) 0 )
    {
        /* Was a task waiting for the data? */
        إذا( prv_بایټونه_دننه_بفر( px_بهیر_بفر ) >= px_بهیر_بفر->x_Trigger_Level_بایټونه )
        {
            prvSEND_COMPLETE_FROM_ISR( px_بهیر_بفر, px_Higher_لومړیتوب_Task_Woken );
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

    trace_بهیر_بفر_SEND_FROM_ISR( x_بهیر_بفر, xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

ساكن size_t prv_Write_پیغام_منزل_بفر( بهیر_بفر_t * ثابت px_بهیر_بفر,
                                       ثابت فراغ * pv_Tx_معلومات,
                                       size_t x_معلومات_اوږدتیا_بایټونه,
                                       size_t xSpace,
                                       size_t xRequiredSpace )
{
    size_t x_بل_سر = px_بهیر_بفر->x_سر;
    config_پیغام_بفر_اوږدتیا_TYPE x_پیغام_اوږدتیا;

    إذا( ( px_بهیر_بفر->ucFlags & sb_FLAGS_IS_پیغام_بفر ) != ( uint8_t ) 0 )
    {
        /* This is a message buffer, as opposed to a stream buffer. */

        /* Convert xDataLengthBytes to the message length type. */
        x_پیغام_اوږدتیا = ( config_پیغام_بفر_اوږدتیا_TYPE ) x_معلومات_اوږدتیا_بایټونه;

        /* Ensure the data length given fits within configMESSAGE_BUFFER_LENGTH_TYPE. */
        configASSERT( ( size_t ) x_پیغام_اوږدتیا == x_معلومات_اوږدتیا_بایټونه );

        إذا( xSpace >= xRequiredSpace )
        {
            /* There is enough space to write both the message length and the message
             * itself into the buffer.  Start by writing the length of the data, the data
             * itself will be written later in this function. */
            x_بل_سر = prv_Write_بایټونه_منزل_بفر( px_بهیر_بفر, ( ثابت uint8_t * ) &( x_پیغام_اوږدتیا ), sb_بایټونه_منزل_STORE_پیغام_اوږدتیا, x_بل_سر );
        }
        وإلا
        {
            /* Not enough space, so do not write data to the buffer. */
            x_معلومات_اوږدتیا_بایټونه = 0;
        }
    }
    وإلا
    {
        /* This is a stream buffer, as opposed to a message buffer, so writing a
         * stream of bytes rather than discrete messages.  Plan to write as many
         * bytes as possible. */
        x_معلومات_اوږدتیا_بایټونه = configMIN( x_معلومات_اوږدتیا_بایټونه, xSpace );
    }

    إذا( x_معلومات_اوږدتیا_بایټونه != ( size_t ) 0 )
    {
        /* Write the data to the buffer. */
        px_بهیر_بفر->x_سر = prv_Write_بایټونه_منزل_بفر( px_بهیر_بفر, ( ثابت uint8_t * ) pv_Tx_معلومات, x_معلومات_اوږدتیا_بایټونه, x_بل_سر ); /*lint !e9079 Storage buffer is implemented as uint8_t for ease of sizing, alignment and access. */
    }

    أرجع x_معلومات_اوږدتیا_بایټونه;
}
/*-----------------------------------------------------------*/

size_t x_بهیر_بفر_Receive( بهیر_بفر_Handle_t x_بهیر_بفر,
                             فراغ * pv_Rx_معلومات,
                             size_t x_بفر_اوږدتیا_بایټونه,
                             TickType_t x_Ticks_منزل_Wait )
{
    بهیر_بفر_t * ثابت px_بهیر_بفر = x_بهیر_بفر;
    size_t x_Received_اوږدتیا = 0, x_بایټونه_شته, x_بایټونه_منزل_Store_پیغام_اوږدتیا;

    configASSERT( pv_Rx_معلومات );
    configASSERT( px_بهیر_بفر );

    /* This receive function is used by both message buffers, which store
     * discrete messages, and stream buffers, which store a continuous stream of
     * bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the
     * message. */
    إذا( ( px_بهیر_بفر->ucFlags & sb_FLAGS_IS_پیغام_بفر ) != ( uint8_t ) 0 )
    {
        x_بایټونه_منزل_Store_پیغام_اوږدتیا = sb_بایټونه_منزل_STORE_پیغام_اوږدتیا;
    }
    وإلا
    {
        x_بایټونه_منزل_Store_پیغام_اوږدتیا = 0;
    }

    إذا( x_Ticks_منزل_Wait != ( TickType_t ) 0 )
    {
        /* Checking if there is data and clearing the notification state must be
         * performed atomically. */
        taskENTER_CRITICAL( &( px_بهیر_بفر->x_بهیر_بفر_قلف ) );
        {
            x_بایټونه_شته = prv_بایټونه_دننه_بفر( px_بهیر_بفر );

            /* If this function was invoked by a message buffer read then
             * xBytesToStoreMessageLength holds the number of bytes used to hold
             * the length of the next discrete message.  If this function was
             * invoked by a stream buffer read then xBytesToStoreMessageLength will
             * be 0. */
            إذا( x_بایټونه_شته <= x_بایټونه_منزل_Store_پیغام_اوږدتیا )
            {
                /* Clear notification state as going to wait for data. */
                ( فراغ ) xTaskNotifyStateClear( عدم );

                /* Should only be one reader. */
                configASSERT( px_بهیر_بفر->x_Task_انتظار_منزل_Receive == عدم );
                px_بهیر_بفر->x_Task_انتظار_منزل_Receive = xTaskGetCurrentTaskHandle();
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        taskEXIT_CRITICAL( &( px_بهیر_بفر->x_بهیر_بفر_قلف ) );

        إذا( x_بایټونه_شته <= x_بایټونه_منزل_Store_پیغام_اوږدتیا )
        {
            /* Wait for data to be available. */
            trace_BLOCKING_ON_بهیر_بفر_RECEIVE( x_بهیر_بفر );
            ( فراغ ) xTaskNotifyWait( ( uint32_t ) 0, ( uint32_t ) 0, عدم, x_Ticks_منزل_Wait );

            /* In SMP mode, the task may have been woken and scheduled on
             * another core. Hence, we must clear the xTaskWaitingToReceive
             * handle in a critical section. */
            #إذا ( config_شمېره_د_CORES > 1 )
                taskENTER_CRITICAL( &( px_بهیر_بفر->x_بهیر_بفر_قلف ) );
            #د_که_پای /* configNUMBER_OF_CORES > 1 */
            {
                px_بهیر_بفر->x_Task_انتظار_منزل_Receive = عدم;
            }
            #إذا ( config_شمېره_د_CORES > 1 )
                taskEXIT_CRITICAL( &( px_بهیر_بفر->x_بهیر_بفر_قلف ) );
            #د_که_پای /* configNUMBER_OF_CORES > 1 */

            /* Recheck the data available after blocking. */
            x_بایټونه_شته = prv_بایټونه_دننه_بفر( px_بهیر_بفر );
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    وإلا
    {
        x_بایټونه_شته = prv_بایټونه_دننه_بفر( px_بهیر_بفر );
    }

    /* Whether receiving a discrete message (where xBytesToStoreMessageLength
     * holds the number of bytes used to store the message length) or a stream of
     * bytes (where xBytesToStoreMessageLength is zero), the number of bytes
     * available must be greater than xBytesToStoreMessageLength to be able to
     * read bytes from the buffer. */
    إذا( x_بایټونه_شته > x_بایټونه_منزل_Store_پیغام_اوږدتیا )
    {
        x_Received_اوږدتیا = prv_Read_پیغام_From_بفر( px_بهیر_بفر, pv_Rx_معلومات, x_بفر_اوږدتیا_بایټونه, x_بایټونه_شته );

        /* Was a task waiting for space in the buffer? */
        إذا( x_Received_اوږدتیا != ( size_t ) 0 )
        {
            trace_بهیر_بفر_RECEIVE( x_بهیر_بفر, x_Received_اوږدتیا );
            prvRECEIVE_COMPLETED( x_بهیر_بفر );
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    وإلا
    {
        trace_بهیر_بفر_RECEIVE_FAILED( x_بهیر_بفر );
        mtCOVERAGE_TEST_MARKER();
    }

    أرجع x_Received_اوږدتیا;
}
/*-----------------------------------------------------------*/

size_t x_بهیر_بفر_بل_پیغام_اوږدتیا_بایټونه( بهیر_بفر_Handle_t x_بهیر_بفر )
{
    بهیر_بفر_t * ثابت px_بهیر_بفر = x_بهیر_بفر;
    size_t xReturn, x_بایټونه_شته;
    config_پیغام_بفر_اوږدتیا_TYPE x_لنډمهاله_Return;

    configASSERT( px_بهیر_بفر );

    /* Ensure the stream buffer is being used as a message buffer. */
    إذا( ( px_بهیر_بفر->ucFlags & sb_FLAGS_IS_پیغام_بفر ) != ( uint8_t ) 0 )
    {
        x_بایټونه_شته = prv_بایټونه_دننه_بفر( px_بهیر_بفر );

        إذا( x_بایټونه_شته > sb_بایټونه_منزل_STORE_پیغام_اوږدتیا )
        {
            /* The number of bytes available is greater than the number of bytes
             * required to hold the length of the next message, so another message
             * is available. */
            ( فراغ ) prv_Read_بایټونه_From_بفر( px_بهیر_بفر, ( uint8_t * ) &x_لنډمهاله_Return, sb_بایټونه_منزل_STORE_پیغام_اوږدتیا, px_بهیر_بفر->x_لکۍ );
            xReturn = ( size_t ) x_لنډمهاله_Return;
        }
        وإلا
        {
            /* The minimum amount of bytes in a message buffer is
             * ( sbBYTES_TO_STORE_MESSAGE_LENGTH + 1 ), so if xBytesAvailable is
             * less than sbBYTES_TO_STORE_MESSAGE_LENGTH the only other valid
             * value is 0. */
            configASSERT( x_بایټونه_شته == 0 );
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

size_t x_بهیر_بفر_Receive_From_ISR( بهیر_بفر_Handle_t x_بهیر_بفر,
                                    فراغ * pv_Rx_معلومات,
                                    size_t x_بفر_اوږدتیا_بایټونه,
                                    بنسټ_Type_t * ثابت px_Higher_لومړیتوب_Task_Woken )
{
    بهیر_بفر_t * ثابت px_بهیر_بفر = x_بهیر_بفر;
    size_t x_Received_اوږدتیا = 0, x_بایټونه_شته, x_بایټونه_منزل_Store_پیغام_اوږدتیا;

    configASSERT( pv_Rx_معلومات );
    configASSERT( px_بهیر_بفر );

    /* This receive function is used by both message buffers, which store
     * discrete messages, and stream buffers, which store a continuous stream of
     * bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the
     * message. */
    إذا( ( px_بهیر_بفر->ucFlags & sb_FLAGS_IS_پیغام_بفر ) != ( uint8_t ) 0 )
    {
        x_بایټونه_منزل_Store_پیغام_اوږدتیا = sb_بایټونه_منزل_STORE_پیغام_اوږدتیا;
    }
    وإلا
    {
        x_بایټونه_منزل_Store_پیغام_اوږدتیا = 0;
    }

    x_بایټونه_شته = prv_بایټونه_دننه_بفر( px_بهیر_بفر );

    /* Whether receiving a discrete message (where xBytesToStoreMessageLength
     * holds the number of bytes used to store the message length) or a stream of
     * bytes (where xBytesToStoreMessageLength is zero), the number of bytes
     * available must be greater than xBytesToStoreMessageLength to be able to
     * read bytes from the buffer. */
    إذا( x_بایټونه_شته > x_بایټونه_منزل_Store_پیغام_اوږدتیا )
    {
        x_Received_اوږدتیا = prv_Read_پیغام_From_بفر( px_بهیر_بفر, pv_Rx_معلومات, x_بفر_اوږدتیا_بایټونه, x_بایټونه_شته );

        /* Was a task waiting for space in the buffer? */
        إذا( x_Received_اوږدتیا != ( size_t ) 0 )
        {
            prvRECEIVE_COMPLETED_FROM_ISR( px_بهیر_بفر, px_Higher_لومړیتوب_Task_Woken );
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

    trace_بهیر_بفر_RECEIVE_FROM_ISR( x_بهیر_بفر, x_Received_اوږدتیا );

    أرجع x_Received_اوږدتیا;
}
/*-----------------------------------------------------------*/

ساكن size_t prv_Read_پیغام_From_بفر( بهیر_بفر_t * px_بهیر_بفر,
                                        فراغ * pv_Rx_معلومات,
                                        size_t x_بفر_اوږدتیا_بایټونه,
                                        size_t x_بایټونه_شته )
{
    size_t x_شمېر, x_بل_پیغام_اوږدتیا;
    config_پیغام_بفر_اوږدتیا_TYPE x_لنډمهاله_بل_پیغام_اوږدتیا;
    size_t x_بل_لکۍ = px_بهیر_بفر->x_لکۍ;

    إذا( ( px_بهیر_بفر->ucFlags & sb_FLAGS_IS_پیغام_بفر ) != ( uint8_t ) 0 )
    {
        /* A discrete message is being received.  First receive the length
         * of the message. */
        x_بل_لکۍ = prv_Read_بایټونه_From_بفر( px_بهیر_بفر, ( uint8_t * ) &x_لنډمهاله_بل_پیغام_اوږدتیا, sb_بایټونه_منزل_STORE_پیغام_اوږدتیا, x_بل_لکۍ );
        x_بل_پیغام_اوږدتیا = ( size_t ) x_لنډمهاله_بل_پیغام_اوږدتیا;

        /* Reduce the number of bytes available by the number of bytes just
         * read out. */
        x_بایټونه_شته -= sb_بایټونه_منزل_STORE_پیغام_اوږدتیا;

        /* Check there is enough space in the buffer provided by the
         * user. */
        إذا( x_بل_پیغام_اوږدتیا > x_بفر_اوږدتیا_بایټونه )
        {
            /* The user has provided insufficient space to read the message. */
            x_بل_پیغام_اوږدتیا = 0;
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
        x_بل_پیغام_اوږدتیا = x_بفر_اوږدتیا_بایټونه;
    }

    /* Use the minimum of the wanted bytes and the available bytes. */
    x_شمېر = configMIN( x_بل_پیغام_اوږدتیا, x_بایټونه_شته );

    إذا( x_شمېر != ( size_t ) 0 )
    {
        /* Read the actual data and update the tail to mark the data as officially consumed. */
        px_بهیر_بفر->x_لکۍ = prv_Read_بایټونه_From_بفر( px_بهیر_بفر, ( uint8_t * ) pv_Rx_معلومات, x_شمېر, x_بل_لکۍ ); /*lint !e9079 Data storage area is implemented as uint8_t array for ease of sizing, indexing and alignment. */
    }

    أرجع x_شمېر;
}
/*-----------------------------------------------------------*/

بنسټ_Type_t x_بهیر_بفر_Is_تشه( بهیر_بفر_Handle_t x_بهیر_بفر )
{
    ثابت بهیر_بفر_t * ثابت px_بهیر_بفر = x_بهیر_بفر;
    بنسټ_Type_t xReturn;
    size_t x_لکۍ;

    configASSERT( px_بهیر_بفر );

    /* True if no bytes are available. */
    x_لکۍ = px_بهیر_بفر->x_لکۍ;

    إذا( px_بهیر_بفر->x_سر == x_لکۍ )
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

بنسټ_Type_t x_بهیر_بفر_Is_ډک( بهیر_بفر_Handle_t x_بهیر_بفر )
{
    بنسټ_Type_t xReturn;
    size_t x_بایټونه_منزل_Store_پیغام_اوږدتیا;
    ثابت بهیر_بفر_t * ثابت px_بهیر_بفر = x_بهیر_بفر;

    configASSERT( px_بهیر_بفر );

    /* This generic version of the receive function is used by both message
     * buffers, which store discrete messages, and stream buffers, which store a
     * continuous stream of bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the message. */
    إذا( ( px_بهیر_بفر->ucFlags & sb_FLAGS_IS_پیغام_بفر ) != ( uint8_t ) 0 )
    {
        x_بایټونه_منزل_Store_پیغام_اوږدتیا = sb_بایټونه_منزل_STORE_پیغام_اوږدتیا;
    }
    وإلا
    {
        x_بایټونه_منزل_Store_پیغام_اوږدتیا = 0;
    }

    /* True if the available space equals zero. */
    إذا( x_بهیر_بفر_Spaces_شته( x_بهیر_بفر ) <= x_بایټونه_منزل_Store_پیغام_اوږدتیا )
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

بنسټ_Type_t x_بهیر_بفر_Send_Completed_From_ISR( بهیر_بفر_Handle_t x_بهیر_بفر,
                                              بنسټ_Type_t * px_Higher_لومړیتوب_Task_Woken )
{
    بهیر_بفر_t * ثابت px_بهیر_بفر = x_بهیر_بفر;
    بنسټ_Type_t xReturn;
    UBaseType_t ux_Saved_مداخله_حالت;

    configASSERT( px_بهیر_بفر );

    ux_Saved_مداخله_حالت = ( UBaseType_t ) پورټ_سېټ_مداخله_ماسک_FROM_ISR();
    {
        إذا( ( px_بهیر_بفر )->x_Task_انتظار_منزل_Receive != عدم )
        {
            ( فراغ ) xTaskNotifyFromISR( ( px_بهیر_بفر )->x_Task_انتظار_منزل_Receive,
                                         ( uint32_t ) 0,
                                         eNoAction,
                                         px_Higher_لومړیتوب_Task_Woken );
            ( px_بهیر_بفر )->x_Task_انتظار_منزل_Receive = عدم;
            xReturn = pdTRUE;
        }
        وإلا
        {
            xReturn = pdFALSE;
        }
    }
    پورټ_CLEAR_مداخله_ماسک_FROM_ISR( ux_Saved_مداخله_حالت );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

بنسټ_Type_t x_بهیر_بفر_Receive_Completed_From_ISR( بهیر_بفر_Handle_t x_بهیر_بفر,
                                                 بنسټ_Type_t * px_Higher_لومړیتوب_Task_Woken )
{
    بهیر_بفر_t * ثابت px_بهیر_بفر = x_بهیر_بفر;
    بنسټ_Type_t xReturn;
    UBaseType_t ux_Saved_مداخله_حالت;

    configASSERT( px_بهیر_بفر );

    ux_Saved_مداخله_حالت = ( UBaseType_t ) پورټ_سېټ_مداخله_ماسک_FROM_ISR();
    {
        إذا( ( px_بهیر_بفر )->x_Task_انتظار_منزل_Send != عدم )
        {
            ( فراغ ) xTaskNotifyFromISR( ( px_بهیر_بفر )->x_Task_انتظار_منزل_Send,
                                         ( uint32_t ) 0,
                                         eNoAction,
                                         px_Higher_لومړیتوب_Task_Woken );
            ( px_بهیر_بفر )->x_Task_انتظار_منزل_Send = عدم;
            xReturn = pdTRUE;
        }
        وإلا
        {
            xReturn = pdFALSE;
        }
    }
    پورټ_CLEAR_مداخله_ماسک_FROM_ISR( ux_Saved_مداخله_حالت );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

ساكن size_t prv_Write_بایټونه_منزل_بفر( بهیر_بفر_t * ثابت px_بهیر_بفر,
                                     ثابت uint8_t * puc_معلومات,
                                     size_t x_شمېر,
                                     size_t x_سر )
{
    size_t x_لومړی_اوږدتیا;

    configASSERT( x_شمېر > ( size_t ) 0 );

    /* Calculate the number of bytes that can be added in the first write -
     * which may be less than the total number of bytes that need to be added if
     * the buffer will wrap back to the beginning. */
    x_لومړی_اوږدتیا = configMIN( px_بهیر_بفر->x_اوږدتیا - x_سر, x_شمېر );

    /* Write as many bytes as can be written in the first write. */
    configASSERT( ( x_سر + x_لومړی_اوږدتیا ) <= px_بهیر_بفر->x_اوږدتیا );
    ( فراغ ) حافظه_نقل( ( فراغ * ) ( &( px_بهیر_بفر->puc_بفر[ x_سر ] ) ), ( ثابت فراغ * ) puc_معلومات, x_لومړی_اوږدتیا ); /*lint !e9087 memcpy() requires void *. */

    /* If the number of bytes written was less than the number that could be
     * written in the first write... */
    إذا( x_شمېر > x_لومړی_اوږدتیا )
    {
        /* ...then write the remaining bytes to the start of the buffer. */
        configASSERT( ( x_شمېر - x_لومړی_اوږدتیا ) <= px_بهیر_بفر->x_اوږدتیا );
        ( فراغ ) حافظه_نقل( ( فراغ * ) px_بهیر_بفر->puc_بفر, ( ثابت فراغ * ) &( puc_معلومات[ x_لومړی_اوږدتیا ] ), x_شمېر - x_لومړی_اوږدتیا ); /*lint !e9087 memcpy() requires void *. */
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    x_سر += x_شمېر;

    إذا( x_سر >= px_بهیر_بفر->x_اوږدتیا )
    {
        x_سر -= px_بهیر_بفر->x_اوږدتیا;
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    أرجع x_سر;
}
/*-----------------------------------------------------------*/

ساكن size_t prv_Read_بایټونه_From_بفر( بهیر_بفر_t * px_بهیر_بفر,
                                      uint8_t * puc_معلومات,
                                      size_t x_شمېر,
                                      size_t x_لکۍ )
{
    size_t x_لومړی_اوږدتیا;

    configASSERT( x_شمېر != ( size_t ) 0 );

    /* Calculate the number of bytes that can be read - which may be
     * less than the number wanted if the data wraps around to the start of
     * the buffer. */
    x_لومړی_اوږدتیا = configMIN( px_بهیر_بفر->x_اوږدتیا - x_لکۍ, x_شمېر );

    /* Obtain the number of bytes it is possible to obtain in the first
     * read.  Asserts check bounds of read and write. */
    configASSERT( x_لومړی_اوږدتیا <= x_شمېر );
    configASSERT( ( x_لکۍ + x_لومړی_اوږدتیا ) <= px_بهیر_بفر->x_اوږدتیا );
    ( فراغ ) حافظه_نقل( ( فراغ * ) puc_معلومات, ( ثابت فراغ * ) &( px_بهیر_بفر->puc_بفر[ x_لکۍ ] ), x_لومړی_اوږدتیا ); /*lint !e9087 memcpy() requires void *. */

    /* If the total number of wanted bytes is greater than the number
     * that could be read in the first read... */
    إذا( x_شمېر > x_لومړی_اوږدتیا )
    {
        /* ...then read the remaining bytes from the start of the buffer. */
        ( فراغ ) حافظه_نقل( ( فراغ * ) &( puc_معلومات[ x_لومړی_اوږدتیا ] ), ( فراغ * ) ( px_بهیر_بفر->puc_بفر ), x_شمېر - x_لومړی_اوږدتیا ); /*lint !e9087 memcpy() requires void *. */
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    /* Move the tail pointer to effectively remove the data read from the buffer. */
    x_لکۍ += x_شمېر;

    إذا( x_لکۍ >= px_بهیر_بفر->x_اوږدتیا )
    {
        x_لکۍ -= px_بهیر_بفر->x_اوږدتیا;
    }

    أرجع x_لکۍ;
}
/*-----------------------------------------------------------*/

ساكن size_t prv_بایټونه_دننه_بفر( ثابت بهیر_بفر_t * ثابت px_بهیر_بفر )
{
/* Returns the distance between xTail and xHead. */
    size_t x_شمېر;

    x_شمېر = px_بهیر_بفر->x_اوږدتیا + px_بهیر_بفر->x_سر;
    x_شمېر -= px_بهیر_بفر->x_لکۍ;

    إذا( x_شمېر >= px_بهیر_بفر->x_اوږدتیا )
    {
        x_شمېر -= px_بهیر_بفر->x_اوږدتیا;
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    أرجع x_شمېر;
}
/*-----------------------------------------------------------*/

ساكن فراغ prv_Initialise_New_بهیر_بفر( بهیر_بفر_t * ثابت px_بهیر_بفر,
                                          uint8_t * ثابت puc_بفر,
                                          size_t x_بفر_اندازه_بایټونه,
                                          size_t x_Trigger_Level_بایټونه,
                                          uint8_t ucFlags,
                                          بهیر_بفر_Callback_Function_t pxSendCompletedCallback,
                                          بهیر_بفر_Callback_Function_t pxReceiveCompletedCallback )
{
    /* Assert here is deliberately writing to the entire buffer to ensure it can
     * be written to without generating exceptions, and is setting the buffer to a
     * known value to assist in development/debugging. */
    #إذا ( configASSERT_DEFINED == 1 )
    {
        /* The value written just has to be identifiable when looking at the
         * memory.  Don't use 0xA5 as that is the stack fill value and could
         * result in confusion as to what is actually being observed. */
        ثابت بنسټ_Type_t x_Write_ارزښت = 0x55;
        configASSERT( memset( puc_بفر, ( صحيح ) x_Write_ارزښت, x_بفر_اندازه_بایټونه ) == puc_بفر );
        ( فراغ ) x_Write_ارزښت;
    } /*lint !e529 !e438 xWriteValue is only used if configASSERT() is defined. */
    #د_که_پای

    /* This function could be called from xStreamBufferReset(), so we reset the
     * stream buffer fields manually in order to avoid clearing
     * xStreamBufferLock. The xStreamBufferLock is initialized separately on
     * stream buffer creation. */
    px_بهیر_بفر->x_لکۍ = ( size_t ) 0;
    px_بهیر_بفر->x_سر = ( size_t ) 0;
    px_بهیر_بفر->x_Task_انتظار_منزل_Receive = ( TaskHandle_t ) 0;
    px_بهیر_بفر->x_Task_انتظار_منزل_Send = ( TaskHandle_t ) 0;
    #إذا ( configUSE_TRACE_FACILITY == 1 )
        px_بهیر_بفر->ux_بهیر_بفر_شمېره = ( UBaseType_t ) 0;
    #د_که_پای
    px_بهیر_بفر->puc_بفر = puc_بفر;
    px_بهیر_بفر->x_اوږدتیا = x_بفر_اندازه_بایټونه;
    px_بهیر_بفر->x_Trigger_Level_بایټونه = x_Trigger_Level_بایټونه;
    px_بهیر_بفر->ucFlags = ucFlags;
    #إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    {
        px_بهیر_بفر->pxSendCompletedCallback = pxSendCompletedCallback;
        px_بهیر_بفر->pxReceiveCompletedCallback = pxReceiveCompletedCallback;
    }
    #وإلا
    {
        ( فراغ ) pxSendCompletedCallback;
        ( فراغ ) pxReceiveCompletedCallback;
    }
    #د_که_پای
}

#إذا ( configUSE_TRACE_FACILITY == 1 )

    UBaseType_t ux_بهیر_بفر_Get_بهیر_بفر_شمېره( بهیر_بفر_Handle_t x_بهیر_بفر )
    {
        أرجع x_بهیر_بفر->ux_بهیر_بفر_شمېره;
    }

#د_که_پای /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#إذا ( configUSE_TRACE_FACILITY == 1 )

    فراغ v_بهیر_بفر_سېټ_بهیر_بفر_شمېره( بهیر_بفر_Handle_t x_بهیر_بفر,
                                             UBaseType_t ux_بهیر_بفر_شمېره )
    {
        x_بهیر_بفر->ux_بهیر_بفر_شمېره = ux_بهیر_بفر_شمېره;
    }

#د_که_پای /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#إذا ( configUSE_TRACE_FACILITY == 1 )

    uint8_t uc_بهیر_بفر_Get_بهیر_بفر_Type( بهیر_بفر_Handle_t x_بهیر_بفر )
    {
        أرجع( x_بهیر_بفر->ucFlags & sb_FLAGS_IS_پیغام_بفر );
    }

#د_که_پای /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/
