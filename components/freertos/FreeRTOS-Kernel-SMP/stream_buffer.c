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
#ضمن <string.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#تعریف_کرو MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* FreeRTOS includes. */
#ضمن "FreeRTOS.h"
#ضمن "task.h"
#ضمن "stream_buffer.h"

#إذا ( configUSE_TASK_NOTIFICATIONS != 1 )
    #غلطی configUSE_TASK_NOTIFICATIONS must be سیٹ منزل 1 منزل build دھارا_بفر.c
#جے_انت

#إذا ( INCLUDE_xTaskGetCurrentTaskHandle != 1 )
    #غلطی INCLUDE_xTaskGetCurrentTaskHandle must be سیٹ منزل 1 منزل build دھارا_بفر.c
#جے_انت

/* The MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#تعریف_رد MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* This entire source file will be skipped if the application is not configured
 * to include stream buffer functionality. This #if is closed at the very bottom
 * of this file. If you want to include stream buffers then ensure
 * configUSE_STREAM_BUFFERS is set to 1 in FreeRTOSConfig.h. */
#إذا ( config_USE_دھارا_BUFFERS == 1 )

/* If the user has not provided application specific Rx notification macros,
 * or #defined the notification macros away, then provide default implementations
 * that uses task notifications. */
    #جے_بنا_تعریف sbRECEIVE_COMPLETED
        #تعریف_کرو sbRECEIVE_COMPLETED( px_دھارا_بفر )                                 \
    افعل                                                                                \
    {                                                                                 \
        vTaskSuspendAll();                                                            \
        {                                                                             \
            إذا( ( px_دھارا_بفر )->x_Task_اڈیک_منزل_Send != عدم )                      \
            {                                                                         \
                ( فراغ ) x_Task_Notify_اشاریہ_دار( ( px_دھارا_بفر )->x_Task_اڈیک_منزل_Send,  \
                                             ( px_دھارا_بفر )->ux_Notification_اشاریہ, \
                                             ( uint32_t ) 0,                          \
                                             eNoAction );                             \
                ( px_دھارا_بفر )->x_Task_اڈیک_منزل_Send = عدم;                        \
            }                                                                         \
        }                                                                             \
        ( فراغ ) xTaskResumeAll();                                                    \
    } طالما( 0 )
    #جے_انت /* sbRECEIVE_COMPLETED */

/* If user has provided a per-instance receive complete callback, then
 * invoke the callback else use the receive complete macro which is provided by default for all instances.
 */
    #إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        #تعریف_کرو prvRECEIVE_COMPLETED( px_دھارا_بفر )                                           \
    افعل {                                                                                         \
        إذا( ( px_دھارا_بفر )->pxReceiveCompletedCallback != عدم )                             \
        {                                                                                        \
            ( px_دھارا_بفر )->pxReceiveCompletedCallback( ( px_دھارا_بفر ), pdFALSE, عدم ); \
        }                                                                                        \
        وإلا                                                                                     \
        {                                                                                        \
            sbRECEIVE_COMPLETED( ( px_دھارا_بفر ) );                                           \
        }                                                                                        \
    } طالما( 0 )
    #وإلا /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
        #تعریف_کرو prvRECEIVE_COMPLETED( px_دھارا_بفر )    sbRECEIVE_COMPLETED( ( px_دھارا_بفر ) )
    #جے_انت /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

    #جے_بنا_تعریف sbRECEIVE_COMPLETED_FROM_ISR
        #تعریف_کرو sbRECEIVE_COMPLETED_FROM_ISR( px_دھارا_بفر,                                \
                                              px_Higher_ترجیح_Task_Woken )                    \
    افعل {                                                                                     \
        UBaseType_t ux_Saved_رکاوٹ_حالت;                                                  \
                                                                                             \
        ux_Saved_رکاوٹ_حالت = taskENTER_CRITICAL_FROM_ISR();                              \
        {                                                                                    \
            إذا( ( px_دھارا_بفر )->x_Task_اڈیک_منزل_Send != عدم )                             \
            {                                                                                \
                ( فراغ ) x_Task_Notify_اشاریہ_دار_From_ISR( ( px_دھارا_بفر )->x_Task_اڈیک_منزل_Send,  \
                                                    ( px_دھارا_بفر )->ux_Notification_اشاریہ, \
                                                    ( uint32_t ) 0,                          \
                                                    eNoAction,                               \
                                                    ( px_Higher_ترجیح_Task_Woken ) );         \
                ( px_دھارا_بفر )->x_Task_اڈیک_منزل_Send = عدم;                               \
            }                                                                                \
        }                                                                                    \
        taskEXIT_CRITICAL_FROM_ISR( ux_Saved_رکاوٹ_حالت );                                \
    } طالما( 0 )
    #جے_انت /* sbRECEIVE_COMPLETED_FROM_ISR */

    #إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        #تعریف_کرو prvRECEIVE_COMPLETED_FROM_ISR( px_دھارا_بفر,                                                           \
                                               px_Higher_ترجیح_Task_Woken )                                               \
    افعل {                                                                                                                 \
        إذا( ( px_دھارا_بفر )->pxReceiveCompletedCallback != عدم )                                                     \
        {                                                                                                                \
            ( px_دھارا_بفر )->pxReceiveCompletedCallback( ( px_دھارا_بفر ), pdTRUE, ( px_Higher_ترجیح_Task_Woken ) ); \
        }                                                                                                                \
        وإلا                                                                                                             \
        {                                                                                                                \
            sbRECEIVE_COMPLETED_FROM_ISR( ( px_دھارا_بفر ), ( px_Higher_ترجیح_Task_Woken ) );                           \
        }                                                                                                                \
    } طالما( 0 )
    #وإلا /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
        #تعریف_کرو prvRECEIVE_COMPLETED_FROM_ISR( px_دھارا_بفر, px_Higher_ترجیح_Task_Woken ) \
    sbRECEIVE_COMPLETED_FROM_ISR( ( px_دھارا_بفر ), ( px_Higher_ترجیح_Task_Woken ) )
    #جے_انت /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

/* If the user has not provided an application specific Tx notification macro,
 * or #defined the notification macro away, then provide a default
 * implementation that uses task notifications.
 */
    #جے_بنا_تعریف sbSEND_COMPLETED
        #تعریف_کرو sbSEND_COMPLETED( px_دھارا_بفر )                                  \
    vTaskSuspendAll();                                                              \
    {                                                                               \
        إذا( ( px_دھارا_بفر )->x_Task_اڈیک_منزل_Receive != عدم )                     \
        {                                                                           \
            ( فراغ ) x_Task_Notify_اشاریہ_دار( ( px_دھارا_بفر )->x_Task_اڈیک_منزل_Receive, \
                                         ( px_دھارا_بفر )->ux_Notification_اشاریہ,   \
                                         ( uint32_t ) 0,                            \
                                         eNoAction );                               \
            ( px_دھارا_بفر )->x_Task_اڈیک_منزل_Receive = عدم;                       \
        }                                                                           \
    }                                                                               \
    ( فراغ ) xTaskResumeAll()
    #جے_انت /* sbSEND_COMPLETED */

/* If user has provided a per-instance send completed callback, then
 * invoke the callback else use the send complete macro which is provided by default for all instances.
 */
    #إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        #تعریف_کرو prvSEND_COMPLETED( px_دھارا_بفر )                                           \
    افعل {                                                                                      \
        إذا( ( px_دھارا_بفر )->pxSendCompletedCallback != عدم )                             \
        {                                                                                     \
            ( px_دھارا_بفر )->pxSendCompletedCallback( ( px_دھارا_بفر ), pdFALSE, عدم ); \
        }                                                                                     \
        وإلا                                                                                  \
        {                                                                                     \
            sbSEND_COMPLETED( ( px_دھارا_بفر ) );                                           \
        }                                                                                     \
    } طالما( 0 )
    #وإلا /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
        #تعریف_کرو prvSEND_COMPLETED( px_دھارا_بفر )    sbSEND_COMPLETED( ( px_دھارا_بفر ) )
    #جے_انت /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */


    #جے_بنا_تعریف sbSEND_COMPLETE_FROM_ISR
        #تعریف_کرو sbSEND_COMPLETE_FROM_ISR( px_دھارا_بفر, px_Higher_ترجیح_Task_Woken )          \
    افعل {                                                                                       \
        UBaseType_t ux_Saved_رکاوٹ_حالت;                                                    \
                                                                                               \
        ux_Saved_رکاوٹ_حالت = taskENTER_CRITICAL_FROM_ISR();                                \
        {                                                                                      \
            إذا( ( px_دھارا_بفر )->x_Task_اڈیک_منزل_Receive != عدم )                            \
            {                                                                                  \
                ( فراغ ) x_Task_Notify_اشاریہ_دار_From_ISR( ( px_دھارا_بفر )->x_Task_اڈیک_منزل_Receive, \
                                                    ( px_دھارا_بفر )->ux_Notification_اشاریہ,   \
                                                    ( uint32_t ) 0,                            \
                                                    eNoAction,                                 \
                                                    ( px_Higher_ترجیح_Task_Woken ) );           \
                ( px_دھارا_بفر )->x_Task_اڈیک_منزل_Receive = عدم;                              \
            }                                                                                  \
        }                                                                                      \
        taskEXIT_CRITICAL_FROM_ISR( ux_Saved_رکاوٹ_حالت );                                  \
    } طالما( 0 )
    #جے_انت /* sbSEND_COMPLETE_FROM_ISR */


    #إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        #تعریف_کرو prvSEND_COMPLETE_FROM_ISR( px_دھارا_بفر, px_Higher_ترجیح_Task_Woken )                                \
    افعل {                                                                                                              \
        إذا( ( px_دھارا_بفر )->pxSendCompletedCallback != عدم )                                                     \
        {                                                                                                             \
            ( px_دھارا_بفر )->pxSendCompletedCallback( ( px_دھارا_بفر ), pdTRUE, ( px_Higher_ترجیح_Task_Woken ) ); \
        }                                                                                                             \
        وإلا                                                                                                          \
        {                                                                                                             \
            sbSEND_COMPLETE_FROM_ISR( ( px_دھارا_بفر ), ( px_Higher_ترجیح_Task_Woken ) );                            \
        }                                                                                                             \
    } طالما( 0 )
    #وإلا /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
        #تعریف_کرو prvSEND_COMPLETE_FROM_ISR( px_دھارا_بفر, px_Higher_ترجیح_Task_Woken ) \
    sbSEND_COMPLETE_FROM_ISR( ( px_دھارا_بفر ), ( px_Higher_ترجیح_Task_Woken ) )
    #جے_انت /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

/* The number of bytes used to hold the length of a message in the buffer. */
    #تعریف_کرو sb_بائٹاں_منزل_STORE_سنیہا_لمائی    ( حجم( config_سنیہا_بفر_لمائی_TYPE ) )

/* Bits stored in the ucFlags field of the stream buffer. */
    #تعریف_کرو sb_FLAGS_IS_سنیہا_بفر          ( ( uint8_t ) 1 ) /* Set if the stream buffer was created as a message buffer, in which case it holds discrete messages rather than a stream. */
    #تعریف_کرو sbFLAGS_IS_STATICALLY_ALLOCATED    ( ( uint8_t ) 2 ) /* Set if the stream buffer was created using statically allocated memory. */
    #تعریف_کرو sb_FLAGS_IS_BATCHING_بفر         ( ( uint8_t ) 4 ) /* Set if the stream buffer was created as a batching buffer, meaning the receiver task will only unblock when the trigger level exceededs. */

/*-----------------------------------------------------------*/

/* Structure that hold state information on the buffer. */
تعريفنوع بنية دھارا_بفر_Def_t
{
    متطاير size_t x_پوچھ;                       /* Index to the next item to read within the buffer. */
    متطاير size_t x_سر;                       /* Index to the next item to write within the buffer. */
    size_t x_لمائی;                              /* The length of the buffer pointed to by pucBuffer. */
    size_t x_Trigger_Level_بائٹاں;                   /* The number of bytes that must be in the stream buffer before a task that is waiting for data is unblocked. */
    متطاير TaskHandle_t x_Task_اڈیک_منزل_Receive; /* Holds the handle of a task waiting for data, or NULL if no tasks are waiting. */
    متطاير TaskHandle_t x_Task_اڈیک_منزل_Send;    /* Holds the handle of a task waiting to send data to a message buffer that is full. */
    uint8_t * puc_بفر;                         /* Points to the buffer itself - that is - the RAM that stores the data passed through the buffer. */
    uint8_t ucFlags;

    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_دھارا_بفر_نمبر; /* Used for tracing purposes. */
    #جے_انت

    #إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        دھارا_بفر_Callback_Function_t pxSendCompletedCallback;    /* Optional callback called on send complete. sbSEND_COMPLETED is called if this is NULL. */
        دھارا_بفر_Callback_Function_t pxReceiveCompletedCallback; /* Optional callback called on receive complete.  sbRECEIVE_COMPLETED is called if this is NULL. */
    #جے_انت
    UBaseType_t ux_Notification_اشاریہ;                               /* The index we are using for notification, by default tskDEFAULT_INDEX_TO_NOTIFY. */
} دھارا_بفر_t;

/*
 * The number of bytes available to be read from the buffer.
 */
ساكن size_t prv_بائٹاں_وچ_بفر( ثابت دھارا_بفر_t * ثابت px_دھارا_بفر ) PRIVILEGED_FUNCTION;

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
ساكن size_t prv_Write_بائٹاں_منزل_بفر( دھارا_بفر_t * ثابت px_دھارا_بفر,
                                     ثابت uint8_t * puc_انکڑے,
                                     size_t x_گنتی,
                                     size_t x_سر ) PRIVILEGED_FUNCTION;

/*
 * If the stream buffer is being used as a message buffer, then reads an entire
 * message out of the buffer.  If the stream buffer is being used as a stream
 * buffer then read as many bytes as possible from the buffer.
 * prvReadBytesFromBuffer() is called to actually extract the bytes from the
 * buffer's data storage area.
 */
ساكن size_t prv_Read_سنیہا_From_بفر( دھارا_بفر_t * px_دھارا_بفر,
                                        فراغ * pv_Rx_انکڑے,
                                        size_t x_بفر_لمائی_بائٹاں,
                                        size_t x_بائٹاں_دستیاب ) PRIVILEGED_FUNCTION;

/*
 * If the stream buffer is being used as a message buffer, then writes an entire
 * message to the buffer.  If the stream buffer is being used as a stream
 * buffer then write as many bytes as possible to the buffer.
 * prvWriteBytestoBuffer() is called to actually send the bytes to the buffer's
 * data storage area.
 */
ساكن size_t prv_Write_سنیہا_منزل_بفر( دھارا_بفر_t * ثابت px_دھارا_بفر,
                                       ثابت فراغ * pv_Tx_انکڑے,
                                       size_t x_انکڑے_لمائی_بائٹاں,
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
ساكن size_t prv_Read_بائٹاں_From_بفر( دھارا_بفر_t * px_دھارا_بفر,
                                      uint8_t * puc_انکڑے,
                                      size_t x_گنتی,
                                      size_t x_پوچھ ) PRIVILEGED_FUNCTION;

/*
 * Called by both pxStreamBufferCreate() and pxStreamBufferCreateStatic() to
 * initialise the members of the newly created stream buffer structure.
 */
ساكن فراغ prv_Initialise_New_دھارا_بفر( دھارا_بفر_t * ثابت px_دھارا_بفر,
                                          uint8_t * ثابت puc_بفر,
                                          size_t x_بفر_ناپ_بائٹاں,
                                          size_t x_Trigger_Level_بائٹاں,
                                          uint8_t ucFlags,
                                          دھارا_بفر_Callback_Function_t pxSendCompletedCallback,
                                          دھارا_بفر_Callback_Function_t pxReceiveCompletedCallback ) PRIVILEGED_FUNCTION;

/*-----------------------------------------------------------*/
    #إذا ( config_SUPPORT_DYNAMIC_ونڈ == 1 )
    دھارا_بفر_Handle_t x_دھارا_بفر_Generic_Create( size_t x_بفر_ناپ_بائٹاں,
                                                     size_t x_Trigger_Level_بائٹاں,
                                                     ادھار_Type_t x_دھارا_بفر_Type,
                                                     دھارا_بفر_Callback_Function_t pxSendCompletedCallback,
                                                     دھارا_بفر_Callback_Function_t pxReceiveCompletedCallback )
    {
        فراغ * pv_Allocated_حافظہ;
        uint8_t ucFlags;

        trace_ENTER_x_دھارا_بفر_Generic_Create( x_بفر_ناپ_بائٹاں, x_Trigger_Level_بائٹاں, x_دھارا_بفر_Type, pxSendCompletedCallback, pxReceiveCompletedCallback );

        /* In case the stream buffer is going to be used as a message buffer
         * (that is, it will hold discrete messages with a little meta data that
         * says how big the next message is) check the buffer will be large enough
         * to hold at least one message. */
        إذا( x_دھارا_بفر_Type == sb_TYPE_سنیہا_بفر )
        {
            /* Is a message buffer but not statically allocated. */
            ucFlags = sb_FLAGS_IS_سنیہا_بفر;
            configASSERT( x_بفر_ناپ_بائٹاں > sb_بائٹاں_منزل_STORE_سنیہا_لمائی );
        }
        وإلا إذا( x_دھارا_بفر_Type == sb_TYPE_دھارا_BATCHING_بفر )
        {
            /* Is a batching buffer but not statically allocated. */
            ucFlags = sb_FLAGS_IS_BATCHING_بفر;
            configASSERT( x_بفر_ناپ_بائٹاں > 0 );
        }
        وإلا
        {
            /* Not a message buffer and not statically allocated. */
            ucFlags = 0;
            configASSERT( x_بفر_ناپ_بائٹاں > 0 );
        }

        configASSERT( x_Trigger_Level_بائٹاں <= x_بفر_ناپ_بائٹاں );

        /* A trigger level of 0 would cause a waiting task to unblock even when
         * the buffer was empty. */
        إذا( x_Trigger_Level_بائٹاں == ( size_t ) 0 )
        {
            x_Trigger_Level_بائٹاں = ( size_t ) 1;
        }

        /* A stream buffer requires a StreamBuffer_t structure and a buffer.
         * Both are allocated in a single call to pvPortMalloc().  The
         * StreamBuffer_t structure is placed at the start of the allocated memory
         * and the buffer follows immediately after.  The requested size is
         * incremented so the free space is returned as the user would expect -
         * this is a quirk of the implementation that means otherwise the free
         * space would be reported as one byte smaller than would be logically
         * expected. */
        إذا( x_بفر_ناپ_بائٹاں < ( x_بفر_ناپ_بائٹاں + 1U + حجم( دھارا_بفر_t ) ) )
        {
            x_بفر_ناپ_بائٹاں++;
            pv_Allocated_حافظہ = pv_پورٹ_Malloc( x_بفر_ناپ_بائٹاں + حجم( دھارا_بفر_t ) );
        }
        وإلا
        {
            pv_Allocated_حافظہ = عدم;
        }

        إذا( pv_Allocated_حافظہ != عدم )
        {
            /* MISRA Ref 11.5.1 [Malloc memory assignment] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
            /* coverity[misra_c_2012_rule_11_5_violation] */
            prv_Initialise_New_دھارا_بفر( ( دھارا_بفر_t * ) pv_Allocated_حافظہ,                         /* Structure at the start of the allocated memory. */
                                                                                                          /* MISRA Ref 11.5.1 [Malloc memory assignment] */
                                                                                                          /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
                                                                                                          /* coverity[misra_c_2012_rule_11_5_violation] */
                                          ( ( uint8_t * ) pv_Allocated_حافظہ ) + حجم( دھارا_بفر_t ), /* Storage area follows. */
                                          x_بفر_ناپ_بائٹاں,
                                          x_Trigger_Level_بائٹاں,
                                          ucFlags,
                                          pxSendCompletedCallback,
                                          pxReceiveCompletedCallback );

            trace_دھارا_بفر_CREATE( ( ( دھارا_بفر_t * ) pv_Allocated_حافظہ ), x_دھارا_بفر_Type );
        }
        وإلا
        {
            trace_دھارا_بفر_CREATE_FAILED( x_دھارا_بفر_Type );
        }

        trace_RETURN_x_دھارا_بفر_Generic_Create( pv_Allocated_حافظہ );

        /* MISRA Ref 11.5.1 [Malloc memory assignment] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
        /* coverity[misra_c_2012_rule_11_5_violation] */
        أرجع ( دھارا_بفر_Handle_t ) pv_Allocated_حافظہ;
    }
    #جے_انت /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

    #إذا ( config_SUPPORT_STATIC_ونڈ == 1 )

    دھارا_بفر_Handle_t x_دھارا_بفر_Generic_Create_Static( size_t x_بفر_ناپ_بائٹاں,
                                                           size_t x_Trigger_Level_بائٹاں,
                                                           ادھار_Type_t x_دھارا_بفر_Type,
                                                           uint8_t * ثابت puc_دھارا_بفر_Storage_Area,
                                                           Static_دھارا_بفر_t * ثابت px_Static_دھارا_بفر,
                                                           دھارا_بفر_Callback_Function_t pxSendCompletedCallback,
                                                           دھارا_بفر_Callback_Function_t pxReceiveCompletedCallback )
    {
        /* MISRA Ref 11.3.1 [Misaligned access] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
        /* coverity[misra_c_2012_rule_11_3_violation] */
        دھارا_بفر_t * ثابت px_دھارا_بفر = ( دھارا_بفر_t * ) px_Static_دھارا_بفر;
        دھارا_بفر_Handle_t xReturn;
        uint8_t ucFlags;

        trace_ENTER_x_دھارا_بفر_Generic_Create_Static( x_بفر_ناپ_بائٹاں, x_Trigger_Level_بائٹاں, x_دھارا_بفر_Type, puc_دھارا_بفر_Storage_Area, px_Static_دھارا_بفر, pxSendCompletedCallback, pxReceiveCompletedCallback );

        configASSERT( puc_دھارا_بفر_Storage_Area );
        configASSERT( px_Static_دھارا_بفر );
        configASSERT( x_Trigger_Level_بائٹاں <= x_بفر_ناپ_بائٹاں );

        /* A trigger level of 0 would cause a waiting task to unblock even when
         * the buffer was empty. */
        إذا( x_Trigger_Level_بائٹاں == ( size_t ) 0 )
        {
            x_Trigger_Level_بائٹاں = ( size_t ) 1;
        }

        /* In case the stream buffer is going to be used as a message buffer
         * (that is, it will hold discrete messages with a little meta data that
         * says how big the next message is) check the buffer will be large enough
         * to hold at least one message. */

        إذا( x_دھارا_بفر_Type == sb_TYPE_سنیہا_بفر )
        {
            /* Statically allocated message buffer. */
            ucFlags = sb_FLAGS_IS_سنیہا_بفر | sbFLAGS_IS_STATICALLY_ALLOCATED;
            configASSERT( x_بفر_ناپ_بائٹاں > sb_بائٹاں_منزل_STORE_سنیہا_لمائی );
        }
        وإلا إذا( x_دھارا_بفر_Type == sb_TYPE_دھارا_BATCHING_بفر )
        {
            /* Statically allocated batching buffer. */
            ucFlags = sb_FLAGS_IS_BATCHING_بفر | sbFLAGS_IS_STATICALLY_ALLOCATED;
            configASSERT( x_بفر_ناپ_بائٹاں > 0 );
        }
        وإلا
        {
            /* Statically allocated stream buffer. */
            ucFlags = sbFLAGS_IS_STATICALLY_ALLOCATED;
        }

        #إذا ( configASSERT_DEFINED == 1 )
        {
            /* Sanity check that the size of the structure used to declare a
             * variable of type StaticStreamBuffer_t equals the size of the real
             * message buffer structure. */
            متطاير size_t x_ناپ = حجم( Static_دھارا_بفر_t );
            configASSERT( x_ناپ == حجم( دھارا_بفر_t ) );
        }
        #جے_انت /* configASSERT_DEFINED */

        إذا( ( puc_دھارا_بفر_Storage_Area != عدم ) && ( px_Static_دھارا_بفر != عدم ) )
        {
            prv_Initialise_New_دھارا_بفر( px_دھارا_بفر,
                                          puc_دھارا_بفر_Storage_Area,
                                          x_بفر_ناپ_بائٹاں,
                                          x_Trigger_Level_بائٹاں,
                                          ucFlags,
                                          pxSendCompletedCallback,
                                          pxReceiveCompletedCallback );

            /* Remember this was statically allocated in case it is ever deleted
             * again. */
            px_دھارا_بفر->ucFlags |= sbFLAGS_IS_STATICALLY_ALLOCATED;

            trace_دھارا_بفر_CREATE( px_دھارا_بفر, x_دھارا_بفر_Type );

            /* MISRA Ref 11.3.1 [Misaligned access] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
            /* coverity[misra_c_2012_rule_11_3_violation] */
            xReturn = ( دھارا_بفر_Handle_t ) px_Static_دھارا_بفر;
        }
        وإلا
        {
            xReturn = عدم;
            trace_دھارا_بفر_CREATE_STATIC_FAILED( xReturn, x_دھارا_بفر_Type );
        }

        trace_RETURN_x_دھارا_بفر_Generic_Create_Static( xReturn );

        أرجع xReturn;
    }
    #جے_انت /* ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*-----------------------------------------------------------*/

    #إذا ( config_SUPPORT_STATIC_ونڈ == 1 )
    ادھار_Type_t x_دھارا_بفر_Get_Static_Buffers( دھارا_بفر_Handle_t x_دھارا_بفر,
                                              uint8_t ** ppuc_دھارا_بفر_Storage_Area,
                                              Static_دھارا_بفر_t ** ppx_Static_دھارا_بفر )
    {
        ادھار_Type_t xReturn;
        دھارا_بفر_t * ثابت px_دھارا_بفر = x_دھارا_بفر;

        trace_ENTER_x_دھارا_بفر_Get_Static_Buffers( x_دھارا_بفر, ppuc_دھارا_بفر_Storage_Area, ppx_Static_دھارا_بفر );

        configASSERT( px_دھارا_بفر );
        configASSERT( ppuc_دھارا_بفر_Storage_Area );
        configASSERT( ppx_Static_دھارا_بفر );

        إذا( ( px_دھارا_بفر->ucFlags & sbFLAGS_IS_STATICALLY_ALLOCATED ) != ( uint8_t ) 0 )
        {
            *ppuc_دھارا_بفر_Storage_Area = px_دھارا_بفر->puc_بفر;
            /* MISRA Ref 11.3.1 [Misaligned access] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
            /* coverity[misra_c_2012_rule_11_3_violation] */
            *ppx_Static_دھارا_بفر = ( Static_دھارا_بفر_t * ) px_دھارا_بفر;
            xReturn = pdTRUE;
        }
        وإلا
        {
            xReturn = pdFALSE;
        }

        trace_RETURN_x_دھارا_بفر_Get_Static_Buffers( xReturn );

        أرجع xReturn;
    }
    #جے_انت /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

فراغ v_دھارا_بفر_Delete( دھارا_بفر_Handle_t x_دھارا_بفر )
{
    دھارا_بفر_t * px_دھارا_بفر = x_دھارا_بفر;

    trace_ENTER_v_دھارا_بفر_Delete( x_دھارا_بفر );

    configASSERT( px_دھارا_بفر );

    trace_دھارا_بفر_DELETE( x_دھارا_بفر );

    إذا( ( px_دھارا_بفر->ucFlags & sbFLAGS_IS_STATICALLY_ALLOCATED ) == ( uint8_t ) pdFALSE )
    {
        #إذا ( config_SUPPORT_DYNAMIC_ونڈ == 1 )
        {
            /* Both the structure and the buffer were allocated using a single call
            * to pvPortMalloc(), hence only one call to vPortFree() is required. */
            v_پورٹ_Free( ( فراغ * ) px_دھارا_بفر );
        }
        #وإلا
        {
            /* Should not be possible to get here, ucFlags must be corrupt.
             * Force an assert. */
            configASSERT( x_دھارا_بفر == ( دھارا_بفر_Handle_t ) ~0 );
        }
        #جے_انت
    }
    وإلا
    {
        /* The structure and buffer were not allocated dynamically and cannot be
         * freed - just scrub the structure so future use will assert. */
        ( فراغ ) memset( px_دھارا_بفر, 0x00, حجم( دھارا_بفر_t ) );
    }

    trace_RETURN_v_دھارا_بفر_Delete();
}
/*-----------------------------------------------------------*/

ادھار_Type_t x_دھارا_بفر_Reset( دھارا_بفر_Handle_t x_دھارا_بفر )
{
    دھارا_بفر_t * ثابت px_دھارا_بفر = x_دھارا_بفر;
    ادھار_Type_t xReturn = pdFAIL;
    دھارا_بفر_Callback_Function_t pxSendCallback = عدم, pxReceiveCallback = عدم;

    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_دھارا_بفر_نمبر;
    #جے_انت

    trace_ENTER_x_دھارا_بفر_Reset( x_دھارا_بفر );

    configASSERT( px_دھارا_بفر );

    #إذا ( configUSE_TRACE_FACILITY == 1 )
    {
        /* Store the stream buffer number so it can be restored after the
         * reset. */
        ux_دھارا_بفر_نمبر = px_دھارا_بفر->ux_دھارا_بفر_نمبر;
    }
    #جے_انت

    /* Can only reset a message buffer if there are no tasks blocked on it. */
    taskENTER_CRITICAL();
    {
        إذا( ( px_دھارا_بفر->x_Task_اڈیک_منزل_Receive == عدم ) && ( px_دھارا_بفر->x_Task_اڈیک_منزل_Send == عدم ) )
        {
            #إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
            {
                pxSendCallback = px_دھارا_بفر->pxSendCompletedCallback;
                pxReceiveCallback = px_دھارا_بفر->pxReceiveCompletedCallback;
            }
            #جے_انت

            prv_Initialise_New_دھارا_بفر( px_دھارا_بفر,
                                          px_دھارا_بفر->puc_بفر,
                                          px_دھارا_بفر->x_لمائی,
                                          px_دھارا_بفر->x_Trigger_Level_بائٹاں,
                                          px_دھارا_بفر->ucFlags,
                                          pxSendCallback,
                                          pxReceiveCallback );

            #إذا ( configUSE_TRACE_FACILITY == 1 )
            {
                px_دھارا_بفر->ux_دھارا_بفر_نمبر = ux_دھارا_بفر_نمبر;
            }
            #جے_انت

            trace_دھارا_بفر_RESET( x_دھارا_بفر );

            xReturn = pdPASS;
        }
    }
    taskEXIT_CRITICAL();

    trace_RETURN_x_دھارا_بفر_Reset( xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

ادھار_Type_t x_دھارا_بفر_Reset_From_ISR( دھارا_بفر_Handle_t x_دھارا_بفر )
{
    دھارا_بفر_t * ثابت px_دھارا_بفر = x_دھارا_بفر;
    ادھار_Type_t xReturn = pdFAIL;
    دھارا_بفر_Callback_Function_t pxSendCallback = عدم, pxReceiveCallback = عدم;
    UBaseType_t ux_Saved_رکاوٹ_حالت;

    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_دھارا_بفر_نمبر;
    #جے_انت

    trace_ENTER_x_دھارا_بفر_Reset_From_ISR( x_دھارا_بفر );

    configASSERT( px_دھارا_بفر );

    #إذا ( configUSE_TRACE_FACILITY == 1 )
    {
        /* Store the stream buffer number so it can be restored after the
         * reset. */
        ux_دھارا_بفر_نمبر = px_دھارا_بفر->ux_دھارا_بفر_نمبر;
    }
    #جے_انت

    /* Can only reset a message buffer if there are no tasks blocked on it. */
    /* MISRA Ref 4.7.1 [Return value shall be checked] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
    /* coverity[misra_c_2012_directive_4_7_violation] */
    ux_Saved_رکاوٹ_حالت = taskENTER_CRITICAL_FROM_ISR();
    {
        إذا( ( px_دھارا_بفر->x_Task_اڈیک_منزل_Receive == عدم ) && ( px_دھارا_بفر->x_Task_اڈیک_منزل_Send == عدم ) )
        {
            #إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
            {
                pxSendCallback = px_دھارا_بفر->pxSendCompletedCallback;
                pxReceiveCallback = px_دھارا_بفر->pxReceiveCompletedCallback;
            }
            #جے_انت

            prv_Initialise_New_دھارا_بفر( px_دھارا_بفر,
                                          px_دھارا_بفر->puc_بفر,
                                          px_دھارا_بفر->x_لمائی,
                                          px_دھارا_بفر->x_Trigger_Level_بائٹاں,
                                          px_دھارا_بفر->ucFlags,
                                          pxSendCallback,
                                          pxReceiveCallback );

            #إذا ( configUSE_TRACE_FACILITY == 1 )
            {
                px_دھارا_بفر->ux_دھارا_بفر_نمبر = ux_دھارا_بفر_نمبر;
            }
            #جے_انت

            trace_دھارا_بفر_RESET_FROM_ISR( x_دھارا_بفر );

            xReturn = pdPASS;
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( ux_Saved_رکاوٹ_حالت );

    trace_RETURN_x_دھارا_بفر_Reset_From_ISR( xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

ادھار_Type_t x_دھارا_بفر_سیٹ_Trigger_Level( دھارا_بفر_Handle_t x_دھارا_بفر,
                                         size_t xTriggerLevel )
{
    دھارا_بفر_t * ثابت px_دھارا_بفر = x_دھارا_بفر;
    ادھار_Type_t xReturn;

    trace_ENTER_x_دھارا_بفر_سیٹ_Trigger_Level( x_دھارا_بفر, xTriggerLevel );

    configASSERT( px_دھارا_بفر );

    /* It is not valid for the trigger level to be 0. */
    إذا( xTriggerLevel == ( size_t ) 0 )
    {
        xTriggerLevel = ( size_t ) 1;
    }

    /* The trigger level is the number of bytes that must be in the stream
     * buffer before a task that is waiting for data is unblocked. */
    إذا( xTriggerLevel < px_دھارا_بفر->x_لمائی )
    {
        px_دھارا_بفر->x_Trigger_Level_بائٹاں = xTriggerLevel;
        xReturn = pdPASS;
    }
    وإلا
    {
        xReturn = pdFALSE;
    }

    trace_RETURN_x_دھارا_بفر_سیٹ_Trigger_Level( xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

size_t x_دھارا_بفر_Spaces_دستیاب( دھارا_بفر_Handle_t x_دھارا_بفر )
{
    ثابت دھارا_بفر_t * ثابت px_دھارا_بفر = x_دھارا_بفر;
    size_t xSpace;
    size_t x_Original_پوچھ;

    trace_ENTER_x_دھارا_بفر_Spaces_دستیاب( x_دھارا_بفر );

    configASSERT( px_دھارا_بفر );

    /* The code below reads xTail and then xHead.  This is safe if the stream
     * buffer is updated once between the two reads - but not if the stream buffer
     * is updated more than once between the two reads - hence the loop. */
    افعل
    {
        x_Original_پوچھ = px_دھارا_بفر->x_پوچھ;
        xSpace = px_دھارا_بفر->x_لمائی + px_دھارا_بفر->x_پوچھ;
        xSpace -= px_دھارا_بفر->x_سر;
    } طالما( x_Original_پوچھ != px_دھارا_بفر->x_پوچھ );

    xSpace -= ( size_t ) 1;

    إذا( xSpace >= px_دھارا_بفر->x_لمائی )
    {
        xSpace -= px_دھارا_بفر->x_لمائی;
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    trace_RETURN_x_دھارا_بفر_Spaces_دستیاب( xSpace );

    أرجع xSpace;
}
/*-----------------------------------------------------------*/

size_t x_دھارا_بفر_بائٹاں_دستیاب( دھارا_بفر_Handle_t x_دھارا_بفر )
{
    ثابت دھارا_بفر_t * ثابت px_دھارا_بفر = x_دھارا_بفر;
    size_t xReturn;

    trace_ENTER_x_دھارا_بفر_بائٹاں_دستیاب( x_دھارا_بفر );

    configASSERT( px_دھارا_بفر );

    xReturn = prv_بائٹاں_وچ_بفر( px_دھارا_بفر );

    trace_RETURN_x_دھارا_بفر_بائٹاں_دستیاب( xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

size_t x_دھارا_بفر_Send( دھارا_بفر_Handle_t x_دھارا_بفر,
                          ثابت فراغ * pv_Tx_انکڑے,
                          size_t x_انکڑے_لمائی_بائٹاں,
                          TickType_t x_Ticks_منزل_Wait )
{
    دھارا_بفر_t * ثابت px_دھارا_بفر = x_دھارا_بفر;
    size_t xReturn, xSpace = 0;
    size_t xRequiredSpace = x_انکڑے_لمائی_بائٹاں;
    ویلا_باہرلا_t x_ویلا_باہرلا_2;
    size_t xMaxReportedSpace = 0;

    trace_ENTER_x_دھارا_بفر_Send( x_دھارا_بفر, pv_Tx_انکڑے, x_انکڑے_لمائی_بائٹاں, x_Ticks_منزل_Wait );

    configASSERT( pv_Tx_انکڑے );
    configASSERT( px_دھارا_بفر );

    /* The maximum amount of space a stream buffer will ever report is its length
     * minus 1. */
    xMaxReportedSpace = px_دھارا_بفر->x_لمائی - ( size_t ) 1;

    /* This send function is used to write to both message buffers and stream
     * buffers.  If this is a message buffer then the space needed must be
     * increased by the amount of bytes needed to store the length of the
     * message. */
    إذا( ( px_دھارا_بفر->ucFlags & sb_FLAGS_IS_سنیہا_بفر ) != ( uint8_t ) 0 )
    {
        xRequiredSpace += sb_بائٹاں_منزل_STORE_سنیہا_لمائی;

        /* Overflow? */
        configASSERT( xRequiredSpace > x_انکڑے_لمائی_بائٹاں );

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
        v_Task_سیٹ_ویلا_باہرلا_State( &x_ویلا_باہرلا_2 );

        افعل
        {
            /* Wait until the required number of bytes are free in the message
             * buffer. */
            taskENTER_CRITICAL();
            {
                xSpace = x_دھارا_بفر_Spaces_دستیاب( px_دھارا_بفر );

                إذا( xSpace < xRequiredSpace )
                {
                    /* Clear notification state as going to wait for space. */
                    ( فراغ ) x_Task_Notify_State_Clear_اشاریہ_دار( عدم, px_دھارا_بفر->ux_Notification_اشاریہ );

                    /* Should only be one writer. */
                    configASSERT( px_دھارا_بفر->x_Task_اڈیک_منزل_Send == عدم );
                    px_دھارا_بفر->x_Task_اڈیک_منزل_Send = xTaskGetCurrentTaskHandle();
                }
                وإلا
                {
                    taskEXIT_CRITICAL();
                    اكسر;
                }
            }
            taskEXIT_CRITICAL();

            trace_BLOCKING_ON_دھارا_بفر_SEND( x_دھارا_بفر );
            ( فراغ ) x_Task_Notify_Wait_اشاریہ_دار( px_دھارا_بفر->ux_Notification_اشاریہ, ( uint32_t ) 0, ( uint32_t ) 0, عدم, x_Ticks_منزل_Wait );
            px_دھارا_بفر->x_Task_اڈیک_منزل_Send = عدم;
        } طالما( x_Task_Check_For_ویلا_باہرلا( &x_ویلا_باہرلا_2, &x_Ticks_منزل_Wait ) == pdFALSE );
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    إذا( xSpace == ( size_t ) 0 )
    {
        xSpace = x_دھارا_بفر_Spaces_دستیاب( px_دھارا_بفر );
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    xReturn = prv_Write_سنیہا_منزل_بفر( px_دھارا_بفر, pv_Tx_انکڑے, x_انکڑے_لمائی_بائٹاں, xSpace, xRequiredSpace );

    إذا( xReturn > ( size_t ) 0 )
    {
        trace_دھارا_بفر_SEND( x_دھارا_بفر, xReturn );

        /* Was a task waiting for the data? */
        إذا( prv_بائٹاں_وچ_بفر( px_دھارا_بفر ) >= px_دھارا_بفر->x_Trigger_Level_بائٹاں )
        {
            prvSEND_COMPLETED( px_دھارا_بفر );
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
        trace_دھارا_بفر_SEND_FAILED( x_دھارا_بفر );
    }

    trace_RETURN_x_دھارا_بفر_Send( xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

size_t x_دھارا_بفر_Send_From_ISR( دھارا_بفر_Handle_t x_دھارا_بفر,
                                 ثابت فراغ * pv_Tx_انکڑے,
                                 size_t x_انکڑے_لمائی_بائٹاں,
                                 ادھار_Type_t * ثابت px_Higher_ترجیح_Task_Woken )
{
    دھارا_بفر_t * ثابت px_دھارا_بفر = x_دھارا_بفر;
    size_t xReturn, xSpace;
    size_t xRequiredSpace = x_انکڑے_لمائی_بائٹاں;

    trace_ENTER_x_دھارا_بفر_Send_From_ISR( x_دھارا_بفر, pv_Tx_انکڑے, x_انکڑے_لمائی_بائٹاں, px_Higher_ترجیح_Task_Woken );

    configASSERT( pv_Tx_انکڑے );
    configASSERT( px_دھارا_بفر );

    /* This send function is used to write to both message buffers and stream
     * buffers.  If this is a message buffer then the space needed must be
     * increased by the amount of bytes needed to store the length of the
     * message. */
    إذا( ( px_دھارا_بفر->ucFlags & sb_FLAGS_IS_سنیہا_بفر ) != ( uint8_t ) 0 )
    {
        xRequiredSpace += sb_بائٹاں_منزل_STORE_سنیہا_لمائی;
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    xSpace = x_دھارا_بفر_Spaces_دستیاب( px_دھارا_بفر );
    xReturn = prv_Write_سنیہا_منزل_بفر( px_دھارا_بفر, pv_Tx_انکڑے, x_انکڑے_لمائی_بائٹاں, xSpace, xRequiredSpace );

    إذا( xReturn > ( size_t ) 0 )
    {
        /* Was a task waiting for the data? */
        إذا( prv_بائٹاں_وچ_بفر( px_دھارا_بفر ) >= px_دھارا_بفر->x_Trigger_Level_بائٹاں )
        {
            /* MISRA Ref 4.7.1 [Return value shall be checked] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
            /* coverity[misra_c_2012_directive_4_7_violation] */
            prvSEND_COMPLETE_FROM_ISR( px_دھارا_بفر, px_Higher_ترجیح_Task_Woken );
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

    trace_دھارا_بفر_SEND_FROM_ISR( x_دھارا_بفر, xReturn );
    trace_RETURN_x_دھارا_بفر_Send_From_ISR( xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

ساكن size_t prv_Write_سنیہا_منزل_بفر( دھارا_بفر_t * ثابت px_دھارا_بفر,
                                       ثابت فراغ * pv_Tx_انکڑے,
                                       size_t x_انکڑے_لمائی_بائٹاں,
                                       size_t xSpace,
                                       size_t xRequiredSpace )
{
    size_t x_اگلا_سر = px_دھارا_بفر->x_سر;
    config_سنیہا_بفر_لمائی_TYPE x_سنیہا_لمائی;

    إذا( ( px_دھارا_بفر->ucFlags & sb_FLAGS_IS_سنیہا_بفر ) != ( uint8_t ) 0 )
    {
        /* This is a message buffer, as opposed to a stream buffer. */

        /* Convert xDataLengthBytes to the message length type. */
        x_سنیہا_لمائی = ( config_سنیہا_بفر_لمائی_TYPE ) x_انکڑے_لمائی_بائٹاں;

        /* Ensure the data length given fits within configMESSAGE_BUFFER_LENGTH_TYPE. */
        configASSERT( ( size_t ) x_سنیہا_لمائی == x_انکڑے_لمائی_بائٹاں );

        إذا( xSpace >= xRequiredSpace )
        {
            /* There is enough space to write both the message length and the message
             * itself into the buffer.  Start by writing the length of the data, the data
             * itself will be written later in this function. */
            x_اگلا_سر = prv_Write_بائٹاں_منزل_بفر( px_دھارا_بفر, ( ثابت uint8_t * ) &( x_سنیہا_لمائی ), sb_بائٹاں_منزل_STORE_سنیہا_لمائی, x_اگلا_سر );
        }
        وإلا
        {
            /* Not enough space, so do not write data to the buffer. */
            x_انکڑے_لمائی_بائٹاں = 0;
        }
    }
    وإلا
    {
        /* This is a stream buffer, as opposed to a message buffer, so writing a
         * stream of bytes rather than discrete messages.  Plan to write as many
         * bytes as possible. */
        x_انکڑے_لمائی_بائٹاں = configMIN( x_انکڑے_لمائی_بائٹاں, xSpace );
    }

    إذا( x_انکڑے_لمائی_بائٹاں != ( size_t ) 0 )
    {
        /* Write the data to the buffer. */
        /* MISRA Ref 11.5.5 [Void pointer assignment] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
        /* coverity[misra_c_2012_rule_11_5_violation] */
        px_دھارا_بفر->x_سر = prv_Write_بائٹاں_منزل_بفر( px_دھارا_بفر, ( ثابت uint8_t * ) pv_Tx_انکڑے, x_انکڑے_لمائی_بائٹاں, x_اگلا_سر );
    }

    أرجع x_انکڑے_لمائی_بائٹاں;
}
/*-----------------------------------------------------------*/

size_t x_دھارا_بفر_Receive( دھارا_بفر_Handle_t x_دھارا_بفر,
                             فراغ * pv_Rx_انکڑے,
                             size_t x_بفر_لمائی_بائٹاں,
                             TickType_t x_Ticks_منزل_Wait )
{
    دھارا_بفر_t * ثابت px_دھارا_بفر = x_دھارا_بفر;
    size_t x_Received_لمائی = 0, x_بائٹاں_دستیاب, x_بائٹاں_منزل_Store_سنیہا_لمائی;

    trace_ENTER_x_دھارا_بفر_Receive( x_دھارا_بفر, pv_Rx_انکڑے, x_بفر_لمائی_بائٹاں, x_Ticks_منزل_Wait );

    configASSERT( pv_Rx_انکڑے );
    configASSERT( px_دھارا_بفر );

    /* This receive function is used by both message buffers, which store
     * discrete messages, and stream buffers, which store a continuous stream of
     * bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the
     * message. */
    إذا( ( px_دھارا_بفر->ucFlags & sb_FLAGS_IS_سنیہا_بفر ) != ( uint8_t ) 0 )
    {
        x_بائٹاں_منزل_Store_سنیہا_لمائی = sb_بائٹاں_منزل_STORE_سنیہا_لمائی;
    }
    وإلا إذا( ( px_دھارا_بفر->ucFlags & sb_FLAGS_IS_BATCHING_بفر ) != ( uint8_t ) 0 )
    {
        /* Force task to block if the batching buffer contains less bytes than
         * the trigger level. */
        x_بائٹاں_منزل_Store_سنیہا_لمائی = px_دھارا_بفر->x_Trigger_Level_بائٹاں;
    }
    وإلا
    {
        x_بائٹاں_منزل_Store_سنیہا_لمائی = 0;
    }

    إذا( x_Ticks_منزل_Wait != ( TickType_t ) 0 )
    {
        /* Checking if there is data and clearing the notification state must be
         * performed atomically. */
        taskENTER_CRITICAL();
        {
            x_بائٹاں_دستیاب = prv_بائٹاں_وچ_بفر( px_دھارا_بفر );

            /* If this function was invoked by a message buffer read then
             * xBytesToStoreMessageLength holds the number of bytes used to hold
             * the length of the next discrete message.  If this function was
             * invoked by a stream buffer read then xBytesToStoreMessageLength will
             * be 0. If this function was invoked by a stream batch buffer read
             * then xBytesToStoreMessageLength will be xTriggerLevelBytes value
             * for the buffer.*/
            إذا( x_بائٹاں_دستیاب <= x_بائٹاں_منزل_Store_سنیہا_لمائی )
            {
                /* Clear notification state as going to wait for data. */
                ( فراغ ) x_Task_Notify_State_Clear_اشاریہ_دار( عدم, px_دھارا_بفر->ux_Notification_اشاریہ );

                /* Should only be one reader. */
                configASSERT( px_دھارا_بفر->x_Task_اڈیک_منزل_Receive == عدم );
                px_دھارا_بفر->x_Task_اڈیک_منزل_Receive = xTaskGetCurrentTaskHandle();
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        taskEXIT_CRITICAL();

        إذا( x_بائٹاں_دستیاب <= x_بائٹاں_منزل_Store_سنیہا_لمائی )
        {
            /* Wait for data to be available. */
            trace_BLOCKING_ON_دھارا_بفر_RECEIVE( x_دھارا_بفر );
            ( فراغ ) x_Task_Notify_Wait_اشاریہ_دار( px_دھارا_بفر->ux_Notification_اشاریہ, ( uint32_t ) 0, ( uint32_t ) 0, عدم, x_Ticks_منزل_Wait );
            px_دھارا_بفر->x_Task_اڈیک_منزل_Receive = عدم;

            /* Recheck the data available after blocking. */
            x_بائٹاں_دستیاب = prv_بائٹاں_وچ_بفر( px_دھارا_بفر );
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    وإلا
    {
        x_بائٹاں_دستیاب = prv_بائٹاں_وچ_بفر( px_دھارا_بفر );
    }

    /* Whether receiving a discrete message (where xBytesToStoreMessageLength
     * holds the number of bytes used to store the message length) or a stream of
     * bytes (where xBytesToStoreMessageLength is zero), the number of bytes
     * available must be greater than xBytesToStoreMessageLength to be able to
     * read bytes from the buffer. */
    إذا( x_بائٹاں_دستیاب > x_بائٹاں_منزل_Store_سنیہا_لمائی )
    {
        x_Received_لمائی = prv_Read_سنیہا_From_بفر( px_دھارا_بفر, pv_Rx_انکڑے, x_بفر_لمائی_بائٹاں, x_بائٹاں_دستیاب );

        /* Was a task waiting for space in the buffer? */
        إذا( x_Received_لمائی != ( size_t ) 0 )
        {
            trace_دھارا_بفر_RECEIVE( x_دھارا_بفر, x_Received_لمائی );
            prvRECEIVE_COMPLETED( x_دھارا_بفر );
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    وإلا
    {
        trace_دھارا_بفر_RECEIVE_FAILED( x_دھارا_بفر );
        mtCOVERAGE_TEST_MARKER();
    }

    trace_RETURN_x_دھارا_بفر_Receive( x_Received_لمائی );

    أرجع x_Received_لمائی;
}
/*-----------------------------------------------------------*/

size_t x_دھارا_بفر_اگلا_سنیہا_لمائی_بائٹاں( دھارا_بفر_Handle_t x_دھارا_بفر )
{
    دھارا_بفر_t * ثابت px_دھارا_بفر = x_دھارا_بفر;
    size_t xReturn, x_بائٹاں_دستیاب;
    config_سنیہا_بفر_لمائی_TYPE x_عارضی_Return;

    trace_ENTER_x_دھارا_بفر_اگلا_سنیہا_لمائی_بائٹاں( x_دھارا_بفر );

    configASSERT( px_دھارا_بفر );

    /* Ensure the stream buffer is being used as a message buffer. */
    إذا( ( px_دھارا_بفر->ucFlags & sb_FLAGS_IS_سنیہا_بفر ) != ( uint8_t ) 0 )
    {
        x_بائٹاں_دستیاب = prv_بائٹاں_وچ_بفر( px_دھارا_بفر );

        إذا( x_بائٹاں_دستیاب > sb_بائٹاں_منزل_STORE_سنیہا_لمائی )
        {
            /* The number of bytes available is greater than the number of bytes
             * required to hold the length of the next message, so another message
             * is available. */
            ( فراغ ) prv_Read_بائٹاں_From_بفر( px_دھارا_بفر, ( uint8_t * ) &x_عارضی_Return, sb_بائٹاں_منزل_STORE_سنیہا_لمائی, px_دھارا_بفر->x_پوچھ );
            xReturn = ( size_t ) x_عارضی_Return;
        }
        وإلا
        {
            /* The minimum amount of bytes in a message buffer is
             * ( sbBYTES_TO_STORE_MESSAGE_LENGTH + 1 ), so if xBytesAvailable is
             * less than sbBYTES_TO_STORE_MESSAGE_LENGTH the only other valid
             * value is 0. */
            configASSERT( x_بائٹاں_دستیاب == 0 );
            xReturn = 0;
        }
    }
    وإلا
    {
        xReturn = 0;
    }

    trace_RETURN_x_دھارا_بفر_اگلا_سنیہا_لمائی_بائٹاں( xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

size_t x_دھارا_بفر_Receive_From_ISR( دھارا_بفر_Handle_t x_دھارا_بفر,
                                    فراغ * pv_Rx_انکڑے,
                                    size_t x_بفر_لمائی_بائٹاں,
                                    ادھار_Type_t * ثابت px_Higher_ترجیح_Task_Woken )
{
    دھارا_بفر_t * ثابت px_دھارا_بفر = x_دھارا_بفر;
    size_t x_Received_لمائی = 0, x_بائٹاں_دستیاب, x_بائٹاں_منزل_Store_سنیہا_لمائی;

    trace_ENTER_x_دھارا_بفر_Receive_From_ISR( x_دھارا_بفر, pv_Rx_انکڑے, x_بفر_لمائی_بائٹاں, px_Higher_ترجیح_Task_Woken );

    configASSERT( pv_Rx_انکڑے );
    configASSERT( px_دھارا_بفر );

    /* This receive function is used by both message buffers, which store
     * discrete messages, and stream buffers, which store a continuous stream of
     * bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the
     * message. */
    إذا( ( px_دھارا_بفر->ucFlags & sb_FLAGS_IS_سنیہا_بفر ) != ( uint8_t ) 0 )
    {
        x_بائٹاں_منزل_Store_سنیہا_لمائی = sb_بائٹاں_منزل_STORE_سنیہا_لمائی;
    }
    وإلا
    {
        x_بائٹاں_منزل_Store_سنیہا_لمائی = 0;
    }

    x_بائٹاں_دستیاب = prv_بائٹاں_وچ_بفر( px_دھارا_بفر );

    /* Whether receiving a discrete message (where xBytesToStoreMessageLength
     * holds the number of bytes used to store the message length) or a stream of
     * bytes (where xBytesToStoreMessageLength is zero), the number of bytes
     * available must be greater than xBytesToStoreMessageLength to be able to
     * read bytes from the buffer. */
    إذا( x_بائٹاں_دستیاب > x_بائٹاں_منزل_Store_سنیہا_لمائی )
    {
        x_Received_لمائی = prv_Read_سنیہا_From_بفر( px_دھارا_بفر, pv_Rx_انکڑے, x_بفر_لمائی_بائٹاں, x_بائٹاں_دستیاب );

        /* Was a task waiting for space in the buffer? */
        إذا( x_Received_لمائی != ( size_t ) 0 )
        {
            /* MISRA Ref 4.7.1 [Return value shall be checked] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
            /* coverity[misra_c_2012_directive_4_7_violation] */
            prvRECEIVE_COMPLETED_FROM_ISR( px_دھارا_بفر, px_Higher_ترجیح_Task_Woken );
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

    trace_دھارا_بفر_RECEIVE_FROM_ISR( x_دھارا_بفر, x_Received_لمائی );
    trace_RETURN_x_دھارا_بفر_Receive_From_ISR( x_Received_لمائی );

    أرجع x_Received_لمائی;
}
/*-----------------------------------------------------------*/

ساكن size_t prv_Read_سنیہا_From_بفر( دھارا_بفر_t * px_دھارا_بفر,
                                        فراغ * pv_Rx_انکڑے,
                                        size_t x_بفر_لمائی_بائٹاں,
                                        size_t x_بائٹاں_دستیاب )
{
    size_t x_گنتی, x_اگلا_سنیہا_لمائی;
    config_سنیہا_بفر_لمائی_TYPE x_عارضی_اگلا_سنیہا_لمائی;
    size_t x_اگلا_پوچھ = px_دھارا_بفر->x_پوچھ;

    إذا( ( px_دھارا_بفر->ucFlags & sb_FLAGS_IS_سنیہا_بفر ) != ( uint8_t ) 0 )
    {
        /* A discrete message is being received.  First receive the length
         * of the message. */
        x_اگلا_پوچھ = prv_Read_بائٹاں_From_بفر( px_دھارا_بفر, ( uint8_t * ) &x_عارضی_اگلا_سنیہا_لمائی, sb_بائٹاں_منزل_STORE_سنیہا_لمائی, x_اگلا_پوچھ );
        x_اگلا_سنیہا_لمائی = ( size_t ) x_عارضی_اگلا_سنیہا_لمائی;

        /* Reduce the number of bytes available by the number of bytes just
         * read out. */
        x_بائٹاں_دستیاب -= sb_بائٹاں_منزل_STORE_سنیہا_لمائی;

        /* Check there is enough space in the buffer provided by the
         * user. */
        إذا( x_اگلا_سنیہا_لمائی > x_بفر_لمائی_بائٹاں )
        {
            /* The user has provided insufficient space to read the message. */
            x_اگلا_سنیہا_لمائی = 0;
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
        x_اگلا_سنیہا_لمائی = x_بفر_لمائی_بائٹاں;
    }

    /* Use the minimum of the wanted bytes and the available bytes. */
    x_گنتی = configMIN( x_اگلا_سنیہا_لمائی, x_بائٹاں_دستیاب );

    إذا( x_گنتی != ( size_t ) 0 )
    {
        /* Read the actual data and update the tail to mark the data as officially consumed. */
        /* MISRA Ref 11.5.5 [Void pointer assignment] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
        /* coverity[misra_c_2012_rule_11_5_violation] */
        px_دھارا_بفر->x_پوچھ = prv_Read_بائٹاں_From_بفر( px_دھارا_بفر, ( uint8_t * ) pv_Rx_انکڑے, x_گنتی, x_اگلا_پوچھ );
    }

    أرجع x_گنتی;
}
/*-----------------------------------------------------------*/

ادھار_Type_t x_دھارا_بفر_Is_خالی_تھاواں( دھارا_بفر_Handle_t x_دھارا_بفر )
{
    ثابت دھارا_بفر_t * ثابت px_دھارا_بفر = x_دھارا_بفر;
    ادھار_Type_t xReturn;
    size_t x_پوچھ;

    trace_ENTER_x_دھارا_بفر_Is_خالی_تھاواں( x_دھارا_بفر );

    configASSERT( px_دھارا_بفر );

    /* True if no bytes are available. */
    x_پوچھ = px_دھارا_بفر->x_پوچھ;

    إذا( px_دھارا_بفر->x_سر == x_پوچھ )
    {
        xReturn = pdTRUE;
    }
    وإلا
    {
        xReturn = pdFALSE;
    }

    trace_RETURN_x_دھارا_بفر_Is_خالی_تھاواں( xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

ادھار_Type_t x_دھارا_بفر_Is_بھریا( دھارا_بفر_Handle_t x_دھارا_بفر )
{
    ادھار_Type_t xReturn;
    size_t x_بائٹاں_منزل_Store_سنیہا_لمائی;
    ثابت دھارا_بفر_t * ثابت px_دھارا_بفر = x_دھارا_بفر;

    trace_ENTER_x_دھارا_بفر_Is_بھریا( x_دھارا_بفر );

    configASSERT( px_دھارا_بفر );

    /* This generic version of the receive function is used by both message
     * buffers, which store discrete messages, and stream buffers, which store a
     * continuous stream of bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the message. */
    إذا( ( px_دھارا_بفر->ucFlags & sb_FLAGS_IS_سنیہا_بفر ) != ( uint8_t ) 0 )
    {
        x_بائٹاں_منزل_Store_سنیہا_لمائی = sb_بائٹاں_منزل_STORE_سنیہا_لمائی;
    }
    وإلا
    {
        x_بائٹاں_منزل_Store_سنیہا_لمائی = 0;
    }

    /* True if the available space equals zero. */
    إذا( x_دھارا_بفر_Spaces_دستیاب( x_دھارا_بفر ) <= x_بائٹاں_منزل_Store_سنیہا_لمائی )
    {
        xReturn = pdTRUE;
    }
    وإلا
    {
        xReturn = pdFALSE;
    }

    trace_RETURN_x_دھارا_بفر_Is_بھریا( xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

ادھار_Type_t x_دھارا_بفر_Send_Completed_From_ISR( دھارا_بفر_Handle_t x_دھارا_بفر,
                                              ادھار_Type_t * px_Higher_ترجیح_Task_Woken )
{
    دھارا_بفر_t * ثابت px_دھارا_بفر = x_دھارا_بفر;
    ادھار_Type_t xReturn;
    UBaseType_t ux_Saved_رکاوٹ_حالت;

    trace_ENTER_x_دھارا_بفر_Send_Completed_From_ISR( x_دھارا_بفر, px_Higher_ترجیح_Task_Woken );

    configASSERT( px_دھارا_بفر );

    /* MISRA Ref 4.7.1 [Return value shall be checked] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
    /* coverity[misra_c_2012_directive_4_7_violation] */
    ux_Saved_رکاوٹ_حالت = taskENTER_CRITICAL_FROM_ISR();
    {
        إذا( ( px_دھارا_بفر )->x_Task_اڈیک_منزل_Receive != عدم )
        {
            ( فراغ ) x_Task_Notify_اشاریہ_دار_From_ISR( ( px_دھارا_بفر )->x_Task_اڈیک_منزل_Receive,
                                                ( px_دھارا_بفر )->ux_Notification_اشاریہ,
                                                ( uint32_t ) 0,
                                                eNoAction,
                                                px_Higher_ترجیح_Task_Woken );
            ( px_دھارا_بفر )->x_Task_اڈیک_منزل_Receive = عدم;
            xReturn = pdTRUE;
        }
        وإلا
        {
            xReturn = pdFALSE;
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( ux_Saved_رکاوٹ_حالت );

    trace_RETURN_x_دھارا_بفر_Send_Completed_From_ISR( xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

ادھار_Type_t x_دھارا_بفر_Receive_Completed_From_ISR( دھارا_بفر_Handle_t x_دھارا_بفر,
                                                 ادھار_Type_t * px_Higher_ترجیح_Task_Woken )
{
    دھارا_بفر_t * ثابت px_دھارا_بفر = x_دھارا_بفر;
    ادھار_Type_t xReturn;
    UBaseType_t ux_Saved_رکاوٹ_حالت;

    trace_ENTER_x_دھارا_بفر_Receive_Completed_From_ISR( x_دھارا_بفر, px_Higher_ترجیح_Task_Woken );

    configASSERT( px_دھارا_بفر );

    /* MISRA Ref 4.7.1 [Return value shall be checked] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
    /* coverity[misra_c_2012_directive_4_7_violation] */
    ux_Saved_رکاوٹ_حالت = taskENTER_CRITICAL_FROM_ISR();
    {
        إذا( ( px_دھارا_بفر )->x_Task_اڈیک_منزل_Send != عدم )
        {
            ( فراغ ) x_Task_Notify_اشاریہ_دار_From_ISR( ( px_دھارا_بفر )->x_Task_اڈیک_منزل_Send,
                                                ( px_دھارا_بفر )->ux_Notification_اشاریہ,
                                                ( uint32_t ) 0,
                                                eNoAction,
                                                px_Higher_ترجیح_Task_Woken );
            ( px_دھارا_بفر )->x_Task_اڈیک_منزل_Send = عدم;
            xReturn = pdTRUE;
        }
        وإلا
        {
            xReturn = pdFALSE;
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( ux_Saved_رکاوٹ_حالت );

    trace_RETURN_x_دھارا_بفر_Receive_Completed_From_ISR( xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

ساكن size_t prv_Write_بائٹاں_منزل_بفر( دھارا_بفر_t * ثابت px_دھارا_بفر,
                                     ثابت uint8_t * puc_انکڑے,
                                     size_t x_گنتی,
                                     size_t x_سر )
{
    size_t x_پہلا_لمائی;

    configASSERT( x_گنتی > ( size_t ) 0 );

    /* Calculate the number of bytes that can be added in the first write -
     * which may be less than the total number of bytes that need to be added if
     * the buffer will wrap back to the beginning. */
    x_پہلا_لمائی = configMIN( px_دھارا_بفر->x_لمائی - x_سر, x_گنتی );

    /* Write as many bytes as can be written in the first write. */
    configASSERT( ( x_سر + x_پہلا_لمائی ) <= px_دھارا_بفر->x_لمائی );
    ( فراغ ) حافظہ_نقل( ( فراغ * ) ( &( px_دھارا_بفر->puc_بفر[ x_سر ] ) ), ( ثابت فراغ * ) puc_انکڑے, x_پہلا_لمائی );

    /* If the number of bytes written was less than the number that could be
     * written in the first write... */
    إذا( x_گنتی > x_پہلا_لمائی )
    {
        /* ...then write the remaining bytes to the start of the buffer. */
        configASSERT( ( x_گنتی - x_پہلا_لمائی ) <= px_دھارا_بفر->x_لمائی );
        ( فراغ ) حافظہ_نقل( ( فراغ * ) px_دھارا_بفر->puc_بفر, ( ثابت فراغ * ) &( puc_انکڑے[ x_پہلا_لمائی ] ), x_گنتی - x_پہلا_لمائی );
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    x_سر += x_گنتی;

    إذا( x_سر >= px_دھارا_بفر->x_لمائی )
    {
        x_سر -= px_دھارا_بفر->x_لمائی;
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    أرجع x_سر;
}
/*-----------------------------------------------------------*/

ساكن size_t prv_Read_بائٹاں_From_بفر( دھارا_بفر_t * px_دھارا_بفر,
                                      uint8_t * puc_انکڑے,
                                      size_t x_گنتی,
                                      size_t x_پوچھ )
{
    size_t x_پہلا_لمائی;

    configASSERT( x_گنتی != ( size_t ) 0 );

    /* Calculate the number of bytes that can be read - which may be
     * less than the number wanted if the data wraps around to the start of
     * the buffer. */
    x_پہلا_لمائی = configMIN( px_دھارا_بفر->x_لمائی - x_پوچھ, x_گنتی );

    /* Obtain the number of bytes it is possible to obtain in the first
     * read.  Asserts check bounds of read and write. */
    configASSERT( x_پہلا_لمائی <= x_گنتی );
    configASSERT( ( x_پوچھ + x_پہلا_لمائی ) <= px_دھارا_بفر->x_لمائی );
    ( فراغ ) حافظہ_نقل( ( فراغ * ) puc_انکڑے, ( ثابت فراغ * ) &( px_دھارا_بفر->puc_بفر[ x_پوچھ ] ), x_پہلا_لمائی );

    /* If the total number of wanted bytes is greater than the number
     * that could be read in the first read... */
    إذا( x_گنتی > x_پہلا_لمائی )
    {
        /* ...then read the remaining bytes from the start of the buffer. */
        ( فراغ ) حافظہ_نقل( ( فراغ * ) &( puc_انکڑے[ x_پہلا_لمائی ] ), ( فراغ * ) ( px_دھارا_بفر->puc_بفر ), x_گنتی - x_پہلا_لمائی );
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    /* Move the tail pointer to effectively remove the data read from the buffer. */
    x_پوچھ += x_گنتی;

    إذا( x_پوچھ >= px_دھارا_بفر->x_لمائی )
    {
        x_پوچھ -= px_دھارا_بفر->x_لمائی;
    }

    أرجع x_پوچھ;
}
/*-----------------------------------------------------------*/

ساكن size_t prv_بائٹاں_وچ_بفر( ثابت دھارا_بفر_t * ثابت px_دھارا_بفر )
{
    /* Returns the distance between xTail and xHead. */
    size_t x_گنتی;

    x_گنتی = px_دھارا_بفر->x_لمائی + px_دھارا_بفر->x_سر;
    x_گنتی -= px_دھارا_بفر->x_پوچھ;

    إذا( x_گنتی >= px_دھارا_بفر->x_لمائی )
    {
        x_گنتی -= px_دھارا_بفر->x_لمائی;
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    أرجع x_گنتی;
}
/*-----------------------------------------------------------*/

ساكن فراغ prv_Initialise_New_دھارا_بفر( دھارا_بفر_t * ثابت px_دھارا_بفر,
                                          uint8_t * ثابت puc_بفر,
                                          size_t x_بفر_ناپ_بائٹاں,
                                          size_t x_Trigger_Level_بائٹاں,
                                          uint8_t ucFlags,
                                          دھارا_بفر_Callback_Function_t pxSendCompletedCallback,
                                          دھارا_بفر_Callback_Function_t pxReceiveCompletedCallback )
{
    /* Assert here is deliberately writing to the entire buffer to ensure it can
     * be written to without generating exceptions, and is setting the buffer to a
     * known value to assist in development/debugging. */
    #إذا ( configASSERT_DEFINED == 1 )
    {
        /* The value written just has to be identifiable when looking at the
         * memory.  Don't use 0xA5 as that is the stack fill value and could
         * result in confusion as to what is actually being observed. */
        #تعریف_کرو دھارا_بفر_بفر_WRITE_قدر    ( 0x55 )
        configASSERT( memset( puc_بفر, ( صحيح ) دھارا_بفر_بفر_WRITE_قدر, x_بفر_ناپ_بائٹاں ) == puc_بفر );
    }
    #جے_انت

    ( فراغ ) memset( ( فراغ * ) px_دھارا_بفر, 0x00, حجم( دھارا_بفر_t ) );
    px_دھارا_بفر->puc_بفر = puc_بفر;
    px_دھارا_بفر->x_لمائی = x_بفر_ناپ_بائٹاں;
    px_دھارا_بفر->x_Trigger_Level_بائٹاں = x_Trigger_Level_بائٹاں;
    px_دھارا_بفر->ucFlags = ucFlags;
    px_دھارا_بفر->ux_Notification_اشاریہ = tsk_DEFAULT_اشاریہ_منزل_NOTIFY;
    #إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    {
        px_دھارا_بفر->pxSendCompletedCallback = pxSendCompletedCallback;
        px_دھارا_بفر->pxReceiveCompletedCallback = pxReceiveCompletedCallback;
    }
    #وإلا
    {
        /* MISRA Ref 11.1.1 [Object type casting] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-111 */
        /* coverity[misra_c_2012_rule_11_1_violation] */
        ( فراغ ) pxSendCompletedCallback;

        /* MISRA Ref 11.1.1 [Object type casting] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-111 */
        /* coverity[misra_c_2012_rule_11_1_violation] */
        ( فراغ ) pxReceiveCompletedCallback;
    }
    #جے_انت /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
}
/*-----------------------------------------------------------*/

UBaseType_t ux_دھارا_بفر_Get_دھارا_بفر_Notification_اشاریہ( دھارا_بفر_Handle_t x_دھارا_بفر )
{
    دھارا_بفر_t * ثابت px_دھارا_بفر = x_دھارا_بفر;

    trace_ENTER_ux_دھارا_بفر_Get_دھارا_بفر_Notification_اشاریہ( x_دھارا_بفر );

    configASSERT( px_دھارا_بفر );

    trace_RETURN_ux_دھارا_بفر_Get_دھارا_بفر_Notification_اشاریہ( px_دھارا_بفر->ux_Notification_اشاریہ );

    أرجع px_دھارا_بفر->ux_Notification_اشاریہ;
}
/*-----------------------------------------------------------*/

فراغ v_دھارا_بفر_سیٹ_دھارا_بفر_Notification_اشاریہ( دھارا_بفر_Handle_t x_دھارا_بفر,
                                                    UBaseType_t ux_Notification_اشاریہ )
{
    دھارا_بفر_t * ثابت px_دھارا_بفر = x_دھارا_بفر;

    trace_ENTER_v_دھارا_بفر_سیٹ_دھارا_بفر_Notification_اشاریہ( x_دھارا_بفر, ux_Notification_اشاریہ );

    configASSERT( px_دھارا_بفر );

    /* There should be no task waiting otherwise we'd never resume them. */
    configASSERT( px_دھارا_بفر->x_Task_اڈیک_منزل_Receive == عدم );
    configASSERT( px_دھارا_بفر->x_Task_اڈیک_منزل_Send == عدم );

    /* Check that the task notification index is valid. */
    configASSERT( ux_Notification_اشاریہ < configTASK_NOTIFICATION_ARRAY_ENTRIES );

    px_دھارا_بفر->ux_Notification_اشاریہ = ux_Notification_اشاریہ;

    trace_RETURN_v_دھارا_بفر_سیٹ_دھارا_بفر_Notification_اشاریہ();
}
/*-----------------------------------------------------------*/

    #إذا ( configUSE_TRACE_FACILITY == 1 )

    UBaseType_t ux_دھارا_بفر_Get_دھارا_بفر_نمبر( دھارا_بفر_Handle_t x_دھارا_بفر )
    {
        trace_ENTER_ux_دھارا_بفر_Get_دھارا_بفر_نمبر( x_دھارا_بفر );

        trace_RETURN_ux_دھارا_بفر_Get_دھارا_بفر_نمبر( x_دھارا_بفر->ux_دھارا_بفر_نمبر );

        أرجع x_دھارا_بفر->ux_دھارا_بفر_نمبر;
    }

    #جے_انت /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

    #إذا ( configUSE_TRACE_FACILITY == 1 )

    فراغ v_دھارا_بفر_سیٹ_دھارا_بفر_نمبر( دھارا_بفر_Handle_t x_دھارا_بفر,
                                             UBaseType_t ux_دھارا_بفر_نمبر )
    {
        trace_ENTER_v_دھارا_بفر_سیٹ_دھارا_بفر_نمبر( x_دھارا_بفر, ux_دھارا_بفر_نمبر );

        x_دھارا_بفر->ux_دھارا_بفر_نمبر = ux_دھارا_بفر_نمبر;

        trace_RETURN_v_دھارا_بفر_سیٹ_دھارا_بفر_نمبر();
    }

    #جے_انت /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

    #إذا ( configUSE_TRACE_FACILITY == 1 )

    uint8_t uc_دھارا_بفر_Get_دھارا_بفر_Type( دھارا_بفر_Handle_t x_دھارا_بفر )
    {
        trace_ENTER_uc_دھارا_بفر_Get_دھارا_بفر_Type( x_دھارا_بفر );

        trace_RETURN_uc_دھارا_بفر_Get_دھارا_بفر_Type( ( uint8_t ) ( x_دھارا_بفر->ucFlags & sb_FLAGS_IS_سنیہا_بفر ) );

        أرجع( ( uint8_t ) ( x_دھارا_بفر->ucFlags & sb_FLAGS_IS_سنیہا_بفر ) );
    }

    #جے_انت /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

/* This entire source file will be skipped if the application is not configured
 * to include stream buffer functionality. This #if is closed at the very bottom
 * of this file. If you want to include stream buffers then ensure
 * configUSE_STREAM_BUFFERS is set to 1 in FreeRTOSConfig.h. */
#جے_انت /* configUSE_STREAM_BUFFERS == 1 */
