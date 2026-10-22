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
#అన్తర్భావ <stdint.h>
#అన్తర్భావ <string.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#నిర్వచించు MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* FreeRTOS includes. */
#అన్తర్భావ "FreeRTOS.h"
#అన్తర్భావ "task.h"
#అన్తర్భావ "stream_buffer.h"
/* Include private IDF API additions for critical thread safety macros */
#అన్తర్భావ "esp_private/freertos_idf_additions_priv.h"

#ఒకవేళ ( configUSE_TASK_NOTIFICATIONS != 1 )
    #దోషం configUSE_TASK_NOTIFICATIONS must be సమితి గమ్యం 1 గమ్యం build ప్రవాహం_బఫరు.c
#అయితే_ముగింపు

#ఒకవేళ ( INCLUDE_xTaskGetCurrentTaskHandle != 1 )
    #దోషం INCLUDE_xTaskGetCurrentTaskHandle must be సమితి గమ్యం 1 గమ్యం build ప్రవాహం_బఫరు.c
#అయితే_ముగింపు

/* Lint e961, e9021 and e750 are suppressed as a MISRA exception justified
 * because the MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#నిర్వచనం_రద్దు MPU_WRAPPERS_INCLUDED_FROM_API_FILE /*lint !e961 !e750 !e9021. */

/* If the user has not provided application specific Rx notification macros,
 * or #defined the notification macros away, then provide default implementations
 * that uses task notifications. */
/*lint -save -e9026 Function like macros allowed and needed here so they can be overridden. */
#నిర్వచితం_కాకపోతే sbRECEIVE_COMPLETED
    #నిర్వచించు sbRECEIVE_COMPLETED( px_ప్రవాహం_బఫరు )                               \
    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( px_ప్రవాహం_బఫరు->x_ప్రవాహం_బఫరు_తాళం ) ); \
    {                                                                           \
        ఒకవేళ( ( px_ప్రవాహం_బఫరు )->x_Task_నిరీక్షణ_గమ్యం_Send != ఖాళీ )                    \
        {                                                                       \
            ( శూన్య ) xTaskNotify( ( px_ప్రవాహం_బఫరు )->x_Task_నిరీక్షణ_గమ్యం_Send,       \
                                  ( uint32_t ) 0,                               \
                                  eNoAction );                                  \
            ( px_ప్రవాహం_బఫరు )->x_Task_నిరీక్షణ_గమ్యం_Send = ఖాళీ;                      \
        }                                                                       \
    }                                                                           \
    ( శూన్య ) prvEXIT_CRITICAL_OR_RESUME_ALL( &( px_ప్రవాహం_బఫరు->x_ప్రవాహం_బఫరు_తాళం ) );
#అయితే_ముగింపు /* sbRECEIVE_COMPLETED */

/* If user has provided a per-instance receive complete callback, then
 * invoke the callback else use the receive complete macro which is provided by default for all instances.
 */
#ఒకవేళ ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #నిర్వచించు prvRECEIVE_COMPLETED( px_ప్రవాహం_బఫరు )                                               \
    {                                                                                            \
        ఒకవేళ( ( px_ప్రవాహం_బఫరు )->pxReceiveCompletedCallback != ఖాళీ )                             \
        {                                                                                        \
            ( px_ప్రవాహం_బఫరు )->pxReceiveCompletedCallback( ( px_ప్రవాహం_బఫరు ), pdFALSE, ఖాళీ ); \
        }                                                                                        \
        లేకపోతే                                                                                     \
        {                                                                                        \
            sbRECEIVE_COMPLETED( ( px_ప్రవాహం_బఫరు ) );                                           \
        }                                                                                        \
    }
#లేకపోతే /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #నిర్వచించు prvRECEIVE_COMPLETED( px_ప్రవాహం_బఫరు )    sbRECEIVE_COMPLETED( ( px_ప్రవాహం_బఫరు ) )
#అయితే_ముగింపు /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

#నిర్వచితం_కాకపోతే sbRECEIVE_COMPLETED_FROM_ISR
    #నిర్వచించు sbRECEIVE_COMPLETED_FROM_ISR( px_ప్రవాహం_బఫరు,                            \
                                          px_Higher_ప్రాధాన్యత_Task_Woken )                \
    {                                                                                \
        UBaseType_t ux_Saved_అంతరాయం_స్థితి;                                          \
                                                                                     \
        ux_Saved_అంతరాయం_స్థితి = ( UBaseType_t ) పోర్టు_సమితి_అంతరాయం_ముసుగు_FROM_ISR();  \
        {                                                                            \
            ఒకవేళ( ( px_ప్రవాహం_బఫరు )->x_Task_నిరీక్షణ_గమ్యం_Send != ఖాళీ )                     \
            {                                                                        \
                ( శూన్య ) xTaskNotifyFromISR( ( px_ప్రవాహం_బఫరు )->x_Task_నిరీక్షణ_గమ్యం_Send, \
                                             ( uint32_t ) 0,                         \
                                             eNoAction,                              \
                                             ( px_Higher_ప్రాధాన్యత_Task_Woken ) );        \
                ( px_ప్రవాహం_బఫరు )->x_Task_నిరీక్షణ_గమ్యం_Send = ఖాళీ;                       \
            }                                                                        \
        }                                                                            \
        పోర్టు_CLEAR_అంతరాయం_ముసుగు_FROM_ISR( ux_Saved_అంతరాయం_స్థితి );                 \
    }
#అయితే_ముగింపు /* sbRECEIVE_COMPLETED_FROM_ISR */

#ఒకవేళ ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #నిర్వచించు prvRECEIVE_COMPLETED_FROM_ISR( px_ప్రవాహం_బఫరు,                                                               \
                                           px_Higher_ప్రాధాన్యత_Task_Woken )                                                   \
    {                                                                                                                    \
        ఒకవేళ( ( px_ప్రవాహం_బఫరు )->pxReceiveCompletedCallback != ఖాళీ )                                                     \
        {                                                                                                                \
            ( px_ప్రవాహం_బఫరు )->pxReceiveCompletedCallback( ( px_ప్రవాహం_బఫరు ), pdTRUE, ( px_Higher_ప్రాధాన్యత_Task_Woken ) ); \
        }                                                                                                                \
        లేకపోతే                                                                                                             \
        {                                                                                                                \
            sbRECEIVE_COMPLETED_FROM_ISR( ( px_ప్రవాహం_బఫరు ), ( px_Higher_ప్రాధాన్యత_Task_Woken ) );                           \
        }                                                                                                                \
    }
#లేకపోతే /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #నిర్వచించు prvRECEIVE_COMPLETED_FROM_ISR( px_ప్రవాహం_బఫరు, px_Higher_ప్రాధాన్యత_Task_Woken ) \
    sbRECEIVE_COMPLETED_FROM_ISR( ( px_ప్రవాహం_బఫరు ), ( px_Higher_ప్రాధాన్యత_Task_Woken ) )
#అయితే_ముగింపు /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

/* If the user has not provided an application specific Tx notification macro,
 * or #defined the notification macro away, then provide a default
 * implementation that uses task notifications.
 */
#నిర్వచితం_కాకపోతే sbSEND_COMPLETED
    #నిర్వచించు sbSEND_COMPLETED( px_ప్రవాహం_బఫరు )                                  \
    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( px_ప్రవాహం_బఫరు->x_ప్రవాహం_బఫరు_తాళం ) ); \
    {                                                                           \
        ఒకవేళ( ( px_ప్రవాహం_బఫరు )->x_Task_నిరీక్షణ_గమ్యం_Receive != ఖాళీ )                 \
        {                                                                       \
            ( శూన్య ) xTaskNotify( ( px_ప్రవాహం_బఫరు )->x_Task_నిరీక్షణ_గమ్యం_Receive,    \
                                  ( uint32_t ) 0,                               \
                                  eNoAction );                                  \
            ( px_ప్రవాహం_బఫరు )->x_Task_నిరీక్షణ_గమ్యం_Receive = ఖాళీ;                   \
        }                                                                       \
    }                                                                           \
    ( శూన్య ) prvEXIT_CRITICAL_OR_RESUME_ALL( &( px_ప్రవాహం_బఫరు->x_ప్రవాహం_బఫరు_తాళం ) );
#అయితే_ముగింపు /* sbSEND_COMPLETED */

/* If user has provided a per-instance send completed callback, then
 * invoke the callback else use the send complete macro which is provided by default for all instances.
 */
#ఒకవేళ ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #నిర్వచించు prvSEND_COMPLETED( px_ప్రవాహం_బఫరు )                                           \
    {                                                                                     \
        ఒకవేళ( ( px_ప్రవాహం_బఫరు )->pxSendCompletedCallback != ఖాళీ )                         \
        {                                                                                 \
            px_ప్రవాహం_బఫరు->pxSendCompletedCallback( ( px_ప్రవాహం_బఫరు ), pdFALSE, ఖాళీ ); \
        }                                                                                 \
        లేకపోతే                                                                              \
        {                                                                                 \
            sbSEND_COMPLETED( ( px_ప్రవాహం_బఫరు ) );                                       \
        }                                                                                 \
    }
#లేకపోతే /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #నిర్వచించు prvSEND_COMPLETED( px_ప్రవాహం_బఫరు )    sbSEND_COMPLETED( ( px_ప్రవాహం_బఫరు ) )
#అయితే_ముగింపు /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */


#నిర్వచితం_కాకపోతే sbSEND_COMPLETE_FROM_ISR
    #నిర్వచించు sbSEND_COMPLETE_FROM_ISR( px_ప్రవాహం_బఫరు, px_Higher_ప్రాధాన్యత_Task_Woken )       \
    {                                                                                   \
        UBaseType_t ux_Saved_అంతరాయం_స్థితి;                                             \
                                                                                        \
        ux_Saved_అంతరాయం_స్థితి = ( UBaseType_t ) పోర్టు_సమితి_అంతరాయం_ముసుగు_FROM_ISR();     \
        {                                                                               \
            ఒకవేళ( ( px_ప్రవాహం_బఫరు )->x_Task_నిరీక్షణ_గమ్యం_Receive != ఖాళీ )                     \
            {                                                                           \
                ( శూన్య ) xTaskNotifyFromISR( ( px_ప్రవాహం_బఫరు )->x_Task_నిరీక్షణ_గమ్యం_Receive, \
                                             ( uint32_t ) 0,                            \
                                             eNoAction,                                 \
                                             ( px_Higher_ప్రాధాన్యత_Task_Woken ) );           \
                ( px_ప్రవాహం_బఫరు )->x_Task_నిరీక్షణ_గమ్యం_Receive = ఖాళీ;                       \
            }                                                                           \
        }                                                                               \
        పోర్టు_CLEAR_అంతరాయం_ముసుగు_FROM_ISR( ux_Saved_అంతరాయం_స్థితి );                    \
    }
#అయితే_ముగింపు /* sbSEND_COMPLETE_FROM_ISR */


#ఒకవేళ ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    #నిర్వచించు prvSEND_COMPLETE_FROM_ISR( px_ప్రవాహం_బఫరు, px_Higher_ప్రాధాన్యత_Task_Woken )                                    \
    {                                                                                                                 \
        ఒకవేళ( ( px_ప్రవాహం_బఫరు )->pxSendCompletedCallback != ఖాళీ )                                                     \
        {                                                                                                             \
            ( px_ప్రవాహం_బఫరు )->pxSendCompletedCallback( ( px_ప్రవాహం_బఫరు ), pdTRUE, ( px_Higher_ప్రాధాన్యత_Task_Woken ) ); \
        }                                                                                                             \
        లేకపోతే                                                                                                          \
        {                                                                                                             \
            sbSEND_COMPLETE_FROM_ISR( ( px_ప్రవాహం_బఫరు ), ( px_Higher_ప్రాధాన్యత_Task_Woken ) );                            \
        }                                                                                                             \
    }
#లేకపోతే /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */
    #నిర్వచించు prvSEND_COMPLETE_FROM_ISR( px_ప్రవాహం_బఫరు, px_Higher_ప్రాధాన్యత_Task_Woken ) \
    sbSEND_COMPLETE_FROM_ISR( ( px_ప్రవాహం_బఫరు ), ( px_Higher_ప్రాధాన్యత_Task_Woken ) )
#అయితే_ముగింపు /* if ( configUSE_SB_COMPLETED_CALLBACK == 1 ) */

/*lint -restore (9026) */

/* The number of bytes used to hold the length of a message in the buffer. */
#నిర్వచించు sb_బైట్లు_గమ్యం_STORE_సందేశం_నిడివి    ( పరిమాణం( config_సందేశం_బఫరు_నిడివి_TYPE ) )

/* Bits stored in the ucFlags field of the stream buffer. */
#నిర్వచించు sb_FLAGS_IS_సందేశం_బఫరు          ( ( uint8_t ) 1 ) /* Set if the stream buffer was created as a message buffer, in which case it holds discrete messages rather than a stream. */
#నిర్వచించు sbFLAGS_IS_STATICALLY_ALLOCATED    ( ( uint8_t ) 2 ) /* Set if the stream buffer was created using statically allocated memory. */

/*-----------------------------------------------------------*/

/* Structure that hold state information on the buffer. */
రకనిర్వచనం నిర్మాణం ప్రవాహం_బఫరు_Def_t                 /*lint !e9058 Style convention uses tag. */
{
    అస్థిర size_t x_తోక;                       /* Index to the next item to read within the buffer. */
    అస్థిర size_t x_తల;                       /* Index to the next item to write within the buffer. */
    size_t x_నిడివి;                              /* The length of the buffer pointed to by pucBuffer. */
    size_t x_Trigger_Level_బైట్లు;                   /* The number of bytes that must be in the stream buffer before a task that is waiting for data is unblocked. */
    అస్థిర TaskHandle_t x_Task_నిరీక్షణ_గమ్యం_Receive; /* Holds the handle of a task waiting for data, or NULL if no tasks are waiting. */
    అస్థిర TaskHandle_t x_Task_నిరీక్షణ_గమ్యం_Send;    /* Holds the handle of a task waiting to send data to a message buffer that is full. */
    uint8_t * puc_బఫరు;                         /* Points to the buffer itself - that is - the RAM that stores the data passed through the buffer. */
    uint8_t ucFlags;

    #ఒకవేళ ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_ప్రవాహం_బఫరు_సంఖ్య; /* Used for tracing purposes. */
    #అయితే_ముగింపు

    #ఒకవేళ ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        ప్రవాహం_బఫరు_Callback_Function_t pxSendCompletedCallback;    /* Optional callback called on send complete. sbSEND_COMPLETED is called if this is NULL. */
        ప్రవాహం_బఫరు_Callback_Function_t pxReceiveCompletedCallback; /* Optional callback called on receive complete.  sbRECEIVE_COMPLETED is called if this is NULL. */
    #అయితే_ముగింపు

    పోర్టు_MUX_TYPE x_ప్రవాహం_బఫరు_తాళం; /* Spinlock required for SMP critical sections */
} ప్రవాహం_బఫరు_t;

/*
 * The number of bytes available to be read from the buffer.
 */
స్థిర size_t prv_బైట్లు_లోపల_బఫరు( స్థిరరాశి ప్రవాహం_బఫరు_t * స్థిరరాశి px_ప్రవాహం_బఫరు ) PRIVILEGED_FUNCTION;

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
స్థిర size_t prv_Write_బైట్లు_గమ్యం_బఫరు( ప్రవాహం_బఫరు_t * స్థిరరాశి px_ప్రవాహం_బఫరు,
                                     స్థిరరాశి uint8_t * puc_దత్తాంశం,
                                     size_t x_లెక్క,
                                     size_t x_తల ) PRIVILEGED_FUNCTION;

/*
 * If the stream buffer is being used as a message buffer, then reads an entire
 * message out of the buffer.  If the stream buffer is being used as a stream
 * buffer then read as many bytes as possible from the buffer.
 * prvReadBytesFromBuffer() is called to actually extract the bytes from the
 * buffer's data storage area.
 */
స్థిర size_t prv_Read_సందేశం_From_బఫరు( ప్రవాహం_బఫరు_t * px_ప్రవాహం_బఫరు,
                                        శూన్య * pv_Rx_దత్తాంశం,
                                        size_t x_బఫరు_నిడివి_బైట్లు,
                                        size_t x_బైట్లు_అందుబాటు ) PRIVILEGED_FUNCTION;

/*
 * If the stream buffer is being used as a message buffer, then writes an entire
 * message to the buffer.  If the stream buffer is being used as a stream
 * buffer then write as many bytes as possible to the buffer.
 * prvWriteBytestoBuffer() is called to actually send the bytes to the buffer's
 * data storage area.
 */
స్థిర size_t prv_Write_సందేశం_గమ్యం_బఫరు( ప్రవాహం_బఫరు_t * స్థిరరాశి px_ప్రవాహం_బఫరు,
                                       స్థిరరాశి శూన్య * pv_Tx_దత్తాంశం,
                                       size_t x_దత్తాంశం_నిడివి_బైట్లు,
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
స్థిర size_t prv_Read_బైట్లు_From_బఫరు( ప్రవాహం_బఫరు_t * px_ప్రవాహం_బఫరు,
                                      uint8_t * puc_దత్తాంశం,
                                      size_t x_లెక్క,
                                      size_t x_తోక ) PRIVILEGED_FUNCTION;

/*
 * Called by both pxStreamBufferCreate() and pxStreamBufferCreateStatic() to
 * initialise the members of the newly created stream buffer structure.
 */
స్థిర శూన్య prv_Initialise_New_ప్రవాహం_బఫరు( ప్రవాహం_బఫరు_t * స్థిరరాశి px_ప్రవాహం_బఫరు,
                                          uint8_t * స్థిరరాశి puc_బఫరు,
                                          size_t x_బఫరు_కొలత_బైట్లు,
                                          size_t x_Trigger_Level_బైట్లు,
                                          uint8_t ucFlags,
                                          ప్రవాహం_బఫరు_Callback_Function_t pxSendCompletedCallback,
                                          ప్రవాహం_బఫరు_Callback_Function_t pxReceiveCompletedCallback ) PRIVILEGED_FUNCTION;

/*-----------------------------------------------------------*/
#ఒకవేళ ( config_SUPPORT_DYNAMIC_కేటాయింపు == 1 )
    ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు_Generic_Create( size_t x_బఫరు_కొలత_బైట్లు,
                                                     size_t x_Trigger_Level_బైట్లు,
                                                     పునాది_Type_t x_Is_సందేశం_బఫరు,
                                                     ప్రవాహం_బఫరు_Callback_Function_t pxSendCompletedCallback,
                                                     ప్రవాహం_బఫరు_Callback_Function_t pxReceiveCompletedCallback )
    {
        uint8_t * puc_Allocated_జ్ఞాపకం;
        uint8_t ucFlags;

        /* In case the stream buffer is going to be used as a message buffer
         * (that is, it will hold discrete messages with a little meta data that
         * says how big the next message is) check the buffer will be large enough
         * to hold at least one message. */
        ఒకవేళ( x_Is_సందేశం_బఫరు == pdTRUE )
        {
            /* Is a message buffer but not statically allocated. */
            ucFlags = sb_FLAGS_IS_సందేశం_బఫరు;
            configASSERT( x_బఫరు_కొలత_బైట్లు > sb_బైట్లు_గమ్యం_STORE_సందేశం_నిడివి );
        }
        లేకపోతే
        {
            /* Not a message buffer and not statically allocated. */
            ucFlags = 0;
            configASSERT( x_బఫరు_కొలత_బైట్లు > 0 );
        }

        configASSERT( x_Trigger_Level_బైట్లు <= x_బఫరు_కొలత_బైట్లు );

        /* A trigger level of 0 would cause a waiting task to unblock even when
         * the buffer was empty. */
        ఒకవేళ( x_Trigger_Level_బైట్లు == ( size_t ) 0 )
        {
            x_Trigger_Level_బైట్లు = ( size_t ) 1;
        }

        /* A stream buffer requires a StreamBuffer_t structure and a buffer.
         * Both are allocated in a single call to pvPortMalloc().  The
         * StreamBuffer_t structure is placed at the start of the allocated memory
         * and the buffer follows immediately after.  The requested size is
         * incremented so the free space is returned as the user would expect -
         * this is a quirk of the implementation that means otherwise the free
         * space would be reported as one byte smaller than would be logically
         * expected. */
        ఒకవేళ( x_బఫరు_కొలత_బైట్లు < ( x_బఫరు_కొలత_బైట్లు + 1 + పరిమాణం( ప్రవాహం_బఫరు_t ) ) )
        {
            x_బఫరు_కొలత_బైట్లు++;
            puc_Allocated_జ్ఞాపకం = ( uint8_t * ) pv_పోర్టు_Malloc( x_బఫరు_కొలత_బైట్లు + పరిమాణం( ప్రవాహం_బఫరు_t ) ); /*lint !e9079 malloc() only returns void*. */
        }
        లేకపోతే
        {
            puc_Allocated_జ్ఞాపకం = ఖాళీ;
        }

        ఒకవేళ( puc_Allocated_జ్ఞాపకం != ఖాళీ )
        {
            prv_Initialise_New_ప్రవాహం_బఫరు( ( ప్రవాహం_బఫరు_t * ) puc_Allocated_జ్ఞాపకం,       /* Structure at the start of the allocated memory. */ /*lint !e9087 Safe cast as allocated memory is aligned. */ /*lint !e826 Area is not too small and alignment is guaranteed provided malloc() behaves as expected and returns aligned buffer. */
                                          puc_Allocated_జ్ఞాపకం + పరిమాణం( ప్రవాహం_బఫరు_t ), /* Storage area follows. */ /*lint !e9016 Indexing past structure valid for uint8_t pointer, also storage area has no alignment requirement. */
                                          x_బఫరు_కొలత_బైట్లు,
                                          x_Trigger_Level_బైట్లు,
                                          ucFlags,
                                          pxSendCompletedCallback,
                                          pxReceiveCompletedCallback );

            /* Initialize the stream buffer's spinlock separately, as
             * prvInitialiseNewStreamBuffer() is also called from
             * xStreamBufferReset(). */
            పోర్టు_MUX_INITIALIZE( &( ( ( ప్రవాహం_బఫరు_t * ) puc_Allocated_జ్ఞాపకం )->x_ప్రవాహం_బఫరు_తాళం ) );

            trace_ప్రవాహం_బఫరు_CREATE( ( ( ప్రవాహం_బఫరు_t * ) puc_Allocated_జ్ఞాపకం ), x_Is_సందేశం_బఫరు );
        }
        లేకపోతే
        {
            trace_ప్రవాహం_బఫరు_CREATE_FAILED( x_Is_సందేశం_బఫరు );
        }

        తిరిగి ( ప్రవాహం_బఫరు_Handle_t ) puc_Allocated_జ్ఞాపకం; /*lint !e9087 !e826 Safe cast as allocated memory is aligned. */
    }
#అయితే_ముగింపు /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

    ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు_Generic_Create_Static( size_t x_బఫరు_కొలత_బైట్లు,
                                                           size_t x_Trigger_Level_బైట్లు,
                                                           పునాది_Type_t x_Is_సందేశం_బఫరు,
                                                           uint8_t * స్థిరరాశి puc_ప్రవాహం_బఫరు_Storage_Area,
                                                           Static_ప్రవాహం_బఫరు_t * స్థిరరాశి px_Static_ప్రవాహం_బఫరు,
                                                           ప్రవాహం_బఫరు_Callback_Function_t pxSendCompletedCallback,
                                                           ప్రవాహం_బఫరు_Callback_Function_t pxReceiveCompletedCallback )
    {
        ప్రవాహం_బఫరు_t * స్థిరరాశి px_ప్రవాహం_బఫరు = ( ప్రవాహం_బఫరు_t * ) px_Static_ప్రవాహం_బఫరు; /*lint !e740 !e9087 Safe cast as StaticStreamBuffer_t is opaque Streambuffer_t. */
        ప్రవాహం_బఫరు_Handle_t xReturn;
        uint8_t ucFlags;

        configASSERT( puc_ప్రవాహం_బఫరు_Storage_Area );
        configASSERT( px_Static_ప్రవాహం_బఫరు );
        configASSERT( x_Trigger_Level_బైట్లు <= x_బఫరు_కొలత_బైట్లు );

        /* A trigger level of 0 would cause a waiting task to unblock even when
         * the buffer was empty. */
        ఒకవేళ( x_Trigger_Level_బైట్లు == ( size_t ) 0 )
        {
            x_Trigger_Level_బైట్లు = ( size_t ) 1;
        }

        ఒకవేళ( x_Is_సందేశం_బఫరు != pdFALSE )
        {
            /* Statically allocated message buffer. */
            ucFlags = sb_FLAGS_IS_సందేశం_బఫరు | sbFLAGS_IS_STATICALLY_ALLOCATED;
        }
        లేకపోతే
        {
            /* Statically allocated stream buffer. */
            ucFlags = sbFLAGS_IS_STATICALLY_ALLOCATED;
        }

        /* In case the stream buffer is going to be used as a message buffer
         * (that is, it will hold discrete messages with a little meta data that
         * says how big the next message is) check the buffer will be large enough
         * to hold at least one message. */
        configASSERT( x_బఫరు_కొలత_బైట్లు > sb_బైట్లు_గమ్యం_STORE_సందేశం_నిడివి );

        #ఒకవేళ ( configASSERT_DEFINED == 1 )
        {
            /* Sanity check that the size of the structure used to declare a
             * variable of type StaticStreamBuffer_t equals the size of the real
             * message buffer structure. */
            అస్థిర size_t x_కొలత = పరిమాణం( Static_ప్రవాహం_బఫరు_t );
            configASSERT( x_కొలత == పరిమాణం( ప్రవాహం_బఫరు_t ) );
            ( శూన్య ) x_కొలత; /* Prevent unused variable warning when configASSERT() is not used. */
        } /*lint !e529 xSize is referenced is configASSERT() is defined. */
        #అయితే_ముగింపు /* configASSERT_DEFINED */

        ఒకవేళ( ( puc_ప్రవాహం_బఫరు_Storage_Area != ఖాళీ ) && ( px_Static_ప్రవాహం_బఫరు != ఖాళీ ) )
        {
            prv_Initialise_New_ప్రవాహం_బఫరు( px_ప్రవాహం_బఫరు,
                                          puc_ప్రవాహం_బఫరు_Storage_Area,
                                          x_బఫరు_కొలత_బైట్లు,
                                          x_Trigger_Level_బైట్లు,
                                          ucFlags,
                                          pxSendCompletedCallback,
                                          pxReceiveCompletedCallback );

            /* Remember this was statically allocated in case it is ever deleted
             * again. */
            px_ప్రవాహం_బఫరు->ucFlags |= sbFLAGS_IS_STATICALLY_ALLOCATED;

            /* Initialize the stream buffer's spinlock separately, as
             * prvInitialiseNewStreamBuffer() is also called from
             * xStreamBufferReset(). */
            పోర్టు_MUX_INITIALIZE( &( px_ప్రవాహం_బఫరు->x_ప్రవాహం_బఫరు_తాళం ) );

            trace_ప్రవాహం_బఫరు_CREATE( px_ప్రవాహం_బఫరు, x_Is_సందేశం_బఫరు );

            xReturn = ( ప్రవాహం_బఫరు_Handle_t ) px_Static_ప్రవాహం_బఫరు; /*lint !e9087 Data hiding requires cast to opaque type. */
        }
        లేకపోతే
        {
            xReturn = ఖాళీ;
            trace_ప్రవాహం_బఫరు_CREATE_STATIC_FAILED( xReturn, x_Is_సందేశం_బఫరు );
        }

        తిరిగి xReturn;
    }
#అయితే_ముగింపు /* ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*-----------------------------------------------------------*/

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )
    పునాది_Type_t x_ప్రవాహం_బఫరు_Get_Static_Buffers( ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు,
                                              uint8_t ** ppuc_ప్రవాహం_బఫరు_Storage_Area,
                                              Static_ప్రవాహం_బఫరు_t ** ppx_Static_ప్రవాహం_బఫరు )
    {
        పునాది_Type_t xReturn;
        స్థిరరాశి ప్రవాహం_బఫరు_t * స్థిరరాశి px_ప్రవాహం_బఫరు = x_ప్రవాహం_బఫరు;

        configASSERT( px_ప్రవాహం_బఫరు );
        configASSERT( ppuc_ప్రవాహం_బఫరు_Storage_Area );
        configASSERT( ppx_Static_ప్రవాహం_బఫరు );

        ఒకవేళ( ( px_ప్రవాహం_బఫరు->ucFlags & sbFLAGS_IS_STATICALLY_ALLOCATED ) != ( uint8_t ) 0 )
        {
            *ppuc_ప్రవాహం_బఫరు_Storage_Area = px_ప్రవాహం_బఫరు->puc_బఫరు;
            *ppx_Static_ప్రవాహం_బఫరు = ( Static_ప్రవాహం_బఫరు_t * ) px_ప్రవాహం_బఫరు;
            xReturn = pdTRUE;
        }
        లేకపోతే
        {
            xReturn = pdFALSE;
        }

        తిరిగి xReturn;
    }
#అయితే_ముగింపు /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

శూన్య v_ప్రవాహం_బఫరు_Delete( ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు )
{
    ప్రవాహం_బఫరు_t * px_ప్రవాహం_బఫరు = x_ప్రవాహం_బఫరు;

    configASSERT( px_ప్రవాహం_బఫరు );

    trace_ప్రవాహం_బఫరు_DELETE( x_ప్రవాహం_బఫరు );

    ఒకవేళ( ( px_ప్రవాహం_బఫరు->ucFlags & sbFLAGS_IS_STATICALLY_ALLOCATED ) == ( uint8_t ) pdFALSE )
    {
        #ఒకవేళ ( config_SUPPORT_DYNAMIC_కేటాయింపు == 1 )
        {
            /* Both the structure and the buffer were allocated using a single call
            * to pvPortMalloc(), hence only one call to vPortFree() is required. */
            v_పోర్టు_Free( ( శూన్య * ) px_ప్రవాహం_బఫరు ); /*lint !e9087 Standard free() semantics require void *, plus pxStreamBuffer was allocated by pvPortMalloc(). */
        }
        #లేకపోతే
        {
            /* Should not be possible to get here, ucFlags must be corrupt.
             * Force an assert. */
            configASSERT( x_ప్రవాహం_బఫరు == ( ప్రవాహం_బఫరు_Handle_t ) ~0 );
        }
        #అయితే_ముగింపు
    }
    లేకపోతే
    {
        /* The structure and buffer were not allocated dynamically and cannot be
         * freed - just scrub the structure so future use will assert. */
        ( శూన్య ) memset( px_ప్రవాహం_బఫరు, 0x00, పరిమాణం( ప్రవాహం_బఫరు_t ) );
    }
}
/*-----------------------------------------------------------*/

పునాది_Type_t x_ప్రవాహం_బఫరు_Reset( ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు )
{
    ప్రవాహం_బఫరు_t * స్థిరరాశి px_ప్రవాహం_బఫరు = x_ప్రవాహం_బఫరు;
    పునాది_Type_t xReturn = pdFAIL;
    ప్రవాహం_బఫరు_Callback_Function_t pxSendCallback = ఖాళీ, pxReceiveCallback = ఖాళీ;

    #ఒకవేళ ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_ప్రవాహం_బఫరు_సంఖ్య;
    #అయితే_ముగింపు

    configASSERT( px_ప్రవాహం_బఫరు );

    #ఒకవేళ ( configUSE_TRACE_FACILITY == 1 )
    {
        /* Store the stream buffer number so it can be restored after the
         * reset. */
        ux_ప్రవాహం_బఫరు_సంఖ్య = px_ప్రవాహం_బఫరు->ux_ప్రవాహం_బఫరు_సంఖ్య;
    }
    #అయితే_ముగింపు

    /* Can only reset a message buffer if there are no tasks blocked on it. */
    taskENTER_CRITICAL( &( px_ప్రవాహం_బఫరు->x_ప్రవాహం_బఫరు_తాళం ) );
    {
        ఒకవేళ( ( px_ప్రవాహం_బఫరు->x_Task_నిరీక్షణ_గమ్యం_Receive == ఖాళీ ) && ( px_ప్రవాహం_బఫరు->x_Task_నిరీక్షణ_గమ్యం_Send == ఖాళీ ) )
        {
            #ఒకవేళ ( configUSE_SB_COMPLETED_CALLBACK == 1 )
            {
                pxSendCallback = px_ప్రవాహం_బఫరు->pxSendCompletedCallback;
                pxReceiveCallback = px_ప్రవాహం_బఫరు->pxReceiveCompletedCallback;
            }
            #అయితే_ముగింపు

            prv_Initialise_New_ప్రవాహం_బఫరు( px_ప్రవాహం_బఫరు,
                                          px_ప్రవాహం_బఫరు->puc_బఫరు,
                                          px_ప్రవాహం_బఫరు->x_నిడివి,
                                          px_ప్రవాహం_బఫరు->x_Trigger_Level_బైట్లు,
                                          px_ప్రవాహం_బఫరు->ucFlags,
                                          pxSendCallback,
                                          pxReceiveCallback );

            #ఒకవేళ ( configUSE_TRACE_FACILITY == 1 )
            {
                px_ప్రవాహం_బఫరు->ux_ప్రవాహం_బఫరు_సంఖ్య = ux_ప్రవాహం_బఫరు_సంఖ్య;
            }
            #అయితే_ముగింపు

            trace_ప్రవాహం_బఫరు_RESET( x_ప్రవాహం_బఫరు );

            xReturn = pdPASS;
        }
    }
    taskEXIT_CRITICAL( &( px_ప్రవాహం_బఫరు->x_ప్రవాహం_బఫరు_తాళం ) );

    తిరిగి xReturn;
}
/*-----------------------------------------------------------*/

పునాది_Type_t x_ప్రవాహం_బఫరు_సమితి_Trigger_Level( ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు,
                                         size_t xTriggerLevel )
{
    ప్రవాహం_బఫరు_t * స్థిరరాశి px_ప్రవాహం_బఫరు = x_ప్రవాహం_బఫరు;
    పునాది_Type_t xReturn;

    configASSERT( px_ప్రవాహం_బఫరు );

    /* It is not valid for the trigger level to be 0. */
    ఒకవేళ( xTriggerLevel == ( size_t ) 0 )
    {
        xTriggerLevel = ( size_t ) 1;
    }

    /* The trigger level is the number of bytes that must be in the stream
     * buffer before a task that is waiting for data is unblocked. */
    ఒకవేళ( xTriggerLevel < px_ప్రవాహం_బఫరు->x_నిడివి )
    {
        px_ప్రవాహం_బఫరు->x_Trigger_Level_బైట్లు = xTriggerLevel;
        xReturn = pdPASS;
    }
    లేకపోతే
    {
        xReturn = pdFALSE;
    }

    తిరిగి xReturn;
}
/*-----------------------------------------------------------*/

size_t x_ప్రవాహం_బఫరు_Spaces_అందుబాటు( ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు )
{
    స్థిరరాశి ప్రవాహం_బఫరు_t * స్థిరరాశి px_ప్రవాహం_బఫరు = x_ప్రవాహం_బఫరు;
    size_t xSpace;
    size_t x_Original_తోక;

    configASSERT( px_ప్రవాహం_బఫరు );

    /* The code below reads xTail and then xHead.  This is safe if the stream
     * buffer is updated once between the two reads - but not if the stream buffer
     * is updated more than once between the two reads - hence the loop. */
    చేయి
    {
        x_Original_తోక = px_ప్రవాహం_బఫరు->x_తోక;
        xSpace = px_ప్రవాహం_బఫరు->x_నిడివి + px_ప్రవాహం_బఫరు->x_తోక;
        xSpace -= px_ప్రవాహం_బఫరు->x_తల;
    } అయితే( x_Original_తోక != px_ప్రవాహం_బఫరు->x_తోక );

    xSpace -= ( size_t ) 1;

    ఒకవేళ( xSpace >= px_ప్రవాహం_బఫరు->x_నిడివి )
    {
        xSpace -= px_ప్రవాహం_బఫరు->x_నిడివి;
    }
    లేకపోతే
    {
        mtCOVERAGE_TEST_MARKER();
    }

    తిరిగి xSpace;
}
/*-----------------------------------------------------------*/

size_t x_ప్రవాహం_బఫరు_బైట్లు_అందుబాటు( ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు )
{
    స్థిరరాశి ప్రవాహం_బఫరు_t * స్థిరరాశి px_ప్రవాహం_బఫరు = x_ప్రవాహం_బఫరు;
    size_t xReturn;

    configASSERT( px_ప్రవాహం_బఫరు );

    xReturn = prv_బైట్లు_లోపల_బఫరు( px_ప్రవాహం_బఫరు );
    తిరిగి xReturn;
}
/*-----------------------------------------------------------*/

size_t x_ప్రవాహం_బఫరు_Send( ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు,
                          స్థిరరాశి శూన్య * pv_Tx_దత్తాంశం,
                          size_t x_దత్తాంశం_నిడివి_బైట్లు,
                          TickType_t x_Ticks_గమ్యం_Wait )
{
    ప్రవాహం_బఫరు_t * స్థిరరాశి px_ప్రవాహం_బఫరు = x_ప్రవాహం_బఫరు;
    size_t xReturn, xSpace = 0;
    size_t xRequiredSpace = x_దత్తాంశం_నిడివి_బైట్లు;
    సమయం_బహిర్గతం_t x_సమయం_బహిర్గతం_2;
    size_t xMaxReportedSpace = 0;

    configASSERT( pv_Tx_దత్తాంశం );
    configASSERT( px_ప్రవాహం_బఫరు );

    /* The maximum amount of space a stream buffer will ever report is its length
     * minus 1. */
    xMaxReportedSpace = px_ప్రవాహం_బఫరు->x_నిడివి - ( size_t ) 1;

    /* This send function is used to write to both message buffers and stream
     * buffers.  If this is a message buffer then the space needed must be
     * increased by the amount of bytes needed to store the length of the
     * message. */
    ఒకవేళ( ( px_ప్రవాహం_బఫరు->ucFlags & sb_FLAGS_IS_సందేశం_బఫరు ) != ( uint8_t ) 0 )
    {
        xRequiredSpace += sb_బైట్లు_గమ్యం_STORE_సందేశం_నిడివి;

        /* Overflow? */
        configASSERT( xRequiredSpace > x_దత్తాంశం_నిడివి_బైట్లు );

        /* If this is a message buffer then it must be possible to write the
         * whole message. */
        ఒకవేళ( xRequiredSpace > xMaxReportedSpace )
        {
            /* The message would not fit even if the entire buffer was empty,
             * so don't wait for space. */
            x_Ticks_గమ్యం_Wait = ( TickType_t ) 0;
        }
        లేకపోతే
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    లేకపోతే
    {
        /* If this is a stream buffer then it is acceptable to write only part
         * of the message to the buffer.  Cap the length to the total length of
         * the buffer. */
        ఒకవేళ( xRequiredSpace > xMaxReportedSpace )
        {
            xRequiredSpace = xMaxReportedSpace;
        }
        లేకపోతే
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }

    ఒకవేళ( x_Ticks_గమ్యం_Wait != ( TickType_t ) 0 )
    {
        v_Task_సమితి_సమయం_బహిర్గతం_State( &x_సమయం_బహిర్గతం_2 );

        చేయి
        {
            /* Wait until the required number of bytes are free in the message
             * buffer. */
            taskENTER_CRITICAL( &( px_ప్రవాహం_బఫరు->x_ప్రవాహం_బఫరు_తాళం ) );
            {
                xSpace = x_ప్రవాహం_బఫరు_Spaces_అందుబాటు( px_ప్రవాహం_బఫరు );

                ఒకవేళ( xSpace < xRequiredSpace )
                {
                    /* Clear notification state as going to wait for space. */
                    ( శూన్య ) xTaskNotifyStateClear( ఖాళీ );

                    /* Should only be one writer. */
                    configASSERT( px_ప్రవాహం_బఫరు->x_Task_నిరీక్షణ_గమ్యం_Send == ఖాళీ );
                    px_ప్రవాహం_బఫరు->x_Task_నిరీక్షణ_గమ్యం_Send = xTaskGetCurrentTaskHandle();
                }
                లేకపోతే
                {
                    taskEXIT_CRITICAL( &( px_ప్రవాహం_బఫరు->x_ప్రవాహం_బఫరు_తాళం ) );
                    ఆపు;
                }
            }
            taskEXIT_CRITICAL( &( px_ప్రవాహం_బఫరు->x_ప్రవాహం_బఫరు_తాళం ) );

            trace_BLOCKING_ON_ప్రవాహం_బఫరు_SEND( x_ప్రవాహం_బఫరు );
            ( శూన్య ) xTaskNotifyWait( ( uint32_t ) 0, ( uint32_t ) 0, ఖాళీ, x_Ticks_గమ్యం_Wait );
            /* In SMP mode, the task may have been woken and scheduled on
             * another core. Hence, we must clear the xTaskWaitingToSend
             * handle in a critical section. */
            #ఒకవేళ ( config_సంఖ్య_యొక్క_CORES > 1 )
                taskENTER_CRITICAL( &( px_ప్రవాహం_బఫరు->x_ప్రవాహం_బఫరు_తాళం ) );
            #అయితే_ముగింపు /* configNUMBER_OF_CORES > 1 */
            {
                px_ప్రవాహం_బఫరు->x_Task_నిరీక్షణ_గమ్యం_Send = ఖాళీ;
            }
            #ఒకవేళ ( config_సంఖ్య_యొక్క_CORES > 1 )
                taskEXIT_CRITICAL( &( px_ప్రవాహం_బఫరు->x_ప్రవాహం_బఫరు_తాళం ) );
            #అయితే_ముగింపు /* configNUMBER_OF_CORES > 1 */
        } అయితే( x_Task_Check_For_సమయం_బహిర్గతం( &x_సమయం_బహిర్గతం_2, &x_Ticks_గమ్యం_Wait ) == pdFALSE );
    }
    లేకపోతే
    {
        mtCOVERAGE_TEST_MARKER();
    }

    ఒకవేళ( xSpace == ( size_t ) 0 )
    {
        xSpace = x_ప్రవాహం_బఫరు_Spaces_అందుబాటు( px_ప్రవాహం_బఫరు );
    }
    లేకపోతే
    {
        mtCOVERAGE_TEST_MARKER();
    }

    xReturn = prv_Write_సందేశం_గమ్యం_బఫరు( px_ప్రవాహం_బఫరు, pv_Tx_దత్తాంశం, x_దత్తాంశం_నిడివి_బైట్లు, xSpace, xRequiredSpace );

    ఒకవేళ( xReturn > ( size_t ) 0 )
    {
        trace_ప్రవాహం_బఫరు_SEND( x_ప్రవాహం_బఫరు, xReturn );

        /* Was a task waiting for the data? */
        ఒకవేళ( prv_బైట్లు_లోపల_బఫరు( px_ప్రవాహం_బఫరు ) >= px_ప్రవాహం_బఫరు->x_Trigger_Level_బైట్లు )
        {
            prvSEND_COMPLETED( px_ప్రవాహం_బఫరు );
        }
        లేకపోతే
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    లేకపోతే
    {
        mtCOVERAGE_TEST_MARKER();
        trace_ప్రవాహం_బఫరు_SEND_FAILED( x_ప్రవాహం_బఫరు );
    }

    తిరిగి xReturn;
}
/*-----------------------------------------------------------*/

size_t x_ప్రవాహం_బఫరు_Send_From_ISR( ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు,
                                 స్థిరరాశి శూన్య * pv_Tx_దత్తాంశం,
                                 size_t x_దత్తాంశం_నిడివి_బైట్లు,
                                 పునాది_Type_t * స్థిరరాశి px_Higher_ప్రాధాన్యత_Task_Woken )
{
    ప్రవాహం_బఫరు_t * స్థిరరాశి px_ప్రవాహం_బఫరు = x_ప్రవాహం_బఫరు;
    size_t xReturn, xSpace;
    size_t xRequiredSpace = x_దత్తాంశం_నిడివి_బైట్లు;

    configASSERT( pv_Tx_దత్తాంశం );
    configASSERT( px_ప్రవాహం_బఫరు );

    /* This send function is used to write to both message buffers and stream
     * buffers.  If this is a message buffer then the space needed must be
     * increased by the amount of bytes needed to store the length of the
     * message. */
    ఒకవేళ( ( px_ప్రవాహం_బఫరు->ucFlags & sb_FLAGS_IS_సందేశం_బఫరు ) != ( uint8_t ) 0 )
    {
        xRequiredSpace += sb_బైట్లు_గమ్యం_STORE_సందేశం_నిడివి;
    }
    లేకపోతే
    {
        mtCOVERAGE_TEST_MARKER();
    }

    xSpace = x_ప్రవాహం_బఫరు_Spaces_అందుబాటు( px_ప్రవాహం_బఫరు );
    xReturn = prv_Write_సందేశం_గమ్యం_బఫరు( px_ప్రవాహం_బఫరు, pv_Tx_దత్తాంశం, x_దత్తాంశం_నిడివి_బైట్లు, xSpace, xRequiredSpace );

    ఒకవేళ( xReturn > ( size_t ) 0 )
    {
        /* Was a task waiting for the data? */
        ఒకవేళ( prv_బైట్లు_లోపల_బఫరు( px_ప్రవాహం_బఫరు ) >= px_ప్రవాహం_బఫరు->x_Trigger_Level_బైట్లు )
        {
            prvSEND_COMPLETE_FROM_ISR( px_ప్రవాహం_బఫరు, px_Higher_ప్రాధాన్యత_Task_Woken );
        }
        లేకపోతే
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    లేకపోతే
    {
        mtCOVERAGE_TEST_MARKER();
    }

    trace_ప్రవాహం_బఫరు_SEND_FROM_ISR( x_ప్రవాహం_బఫరు, xReturn );

    తిరిగి xReturn;
}
/*-----------------------------------------------------------*/

స్థిర size_t prv_Write_సందేశం_గమ్యం_బఫరు( ప్రవాహం_బఫరు_t * స్థిరరాశి px_ప్రవాహం_బఫరు,
                                       స్థిరరాశి శూన్య * pv_Tx_దత్తాంశం,
                                       size_t x_దత్తాంశం_నిడివి_బైట్లు,
                                       size_t xSpace,
                                       size_t xRequiredSpace )
{
    size_t x_తదుపరి_తల = px_ప్రవాహం_బఫరు->x_తల;
    config_సందేశం_బఫరు_నిడివి_TYPE x_సందేశం_నిడివి;

    ఒకవేళ( ( px_ప్రవాహం_బఫరు->ucFlags & sb_FLAGS_IS_సందేశం_బఫరు ) != ( uint8_t ) 0 )
    {
        /* This is a message buffer, as opposed to a stream buffer. */

        /* Convert xDataLengthBytes to the message length type. */
        x_సందేశం_నిడివి = ( config_సందేశం_బఫరు_నిడివి_TYPE ) x_దత్తాంశం_నిడివి_బైట్లు;

        /* Ensure the data length given fits within configMESSAGE_BUFFER_LENGTH_TYPE. */
        configASSERT( ( size_t ) x_సందేశం_నిడివి == x_దత్తాంశం_నిడివి_బైట్లు );

        ఒకవేళ( xSpace >= xRequiredSpace )
        {
            /* There is enough space to write both the message length and the message
             * itself into the buffer.  Start by writing the length of the data, the data
             * itself will be written later in this function. */
            x_తదుపరి_తల = prv_Write_బైట్లు_గమ్యం_బఫరు( px_ప్రవాహం_బఫరు, ( స్థిరరాశి uint8_t * ) &( x_సందేశం_నిడివి ), sb_బైట్లు_గమ్యం_STORE_సందేశం_నిడివి, x_తదుపరి_తల );
        }
        లేకపోతే
        {
            /* Not enough space, so do not write data to the buffer. */
            x_దత్తాంశం_నిడివి_బైట్లు = 0;
        }
    }
    లేకపోతే
    {
        /* This is a stream buffer, as opposed to a message buffer, so writing a
         * stream of bytes rather than discrete messages.  Plan to write as many
         * bytes as possible. */
        x_దత్తాంశం_నిడివి_బైట్లు = configMIN( x_దత్తాంశం_నిడివి_బైట్లు, xSpace );
    }

    ఒకవేళ( x_దత్తాంశం_నిడివి_బైట్లు != ( size_t ) 0 )
    {
        /* Write the data to the buffer. */
        px_ప్రవాహం_బఫరు->x_తల = prv_Write_బైట్లు_గమ్యం_బఫరు( px_ప్రవాహం_బఫరు, ( స్థిరరాశి uint8_t * ) pv_Tx_దత్తాంశం, x_దత్తాంశం_నిడివి_బైట్లు, x_తదుపరి_తల ); /*lint !e9079 Storage buffer is implemented as uint8_t for ease of sizing, alignment and access. */
    }

    తిరిగి x_దత్తాంశం_నిడివి_బైట్లు;
}
/*-----------------------------------------------------------*/

size_t x_ప్రవాహం_బఫరు_Receive( ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు,
                             శూన్య * pv_Rx_దత్తాంశం,
                             size_t x_బఫరు_నిడివి_బైట్లు,
                             TickType_t x_Ticks_గమ్యం_Wait )
{
    ప్రవాహం_బఫరు_t * స్థిరరాశి px_ప్రవాహం_బఫరు = x_ప్రవాహం_బఫరు;
    size_t x_Received_నిడివి = 0, x_బైట్లు_అందుబాటు, x_బైట్లు_గమ్యం_Store_సందేశం_నిడివి;

    configASSERT( pv_Rx_దత్తాంశం );
    configASSERT( px_ప్రవాహం_బఫరు );

    /* This receive function is used by both message buffers, which store
     * discrete messages, and stream buffers, which store a continuous stream of
     * bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the
     * message. */
    ఒకవేళ( ( px_ప్రవాహం_బఫరు->ucFlags & sb_FLAGS_IS_సందేశం_బఫరు ) != ( uint8_t ) 0 )
    {
        x_బైట్లు_గమ్యం_Store_సందేశం_నిడివి = sb_బైట్లు_గమ్యం_STORE_సందేశం_నిడివి;
    }
    లేకపోతే
    {
        x_బైట్లు_గమ్యం_Store_సందేశం_నిడివి = 0;
    }

    ఒకవేళ( x_Ticks_గమ్యం_Wait != ( TickType_t ) 0 )
    {
        /* Checking if there is data and clearing the notification state must be
         * performed atomically. */
        taskENTER_CRITICAL( &( px_ప్రవాహం_బఫరు->x_ప్రవాహం_బఫరు_తాళం ) );
        {
            x_బైట్లు_అందుబాటు = prv_బైట్లు_లోపల_బఫరు( px_ప్రవాహం_బఫరు );

            /* If this function was invoked by a message buffer read then
             * xBytesToStoreMessageLength holds the number of bytes used to hold
             * the length of the next discrete message.  If this function was
             * invoked by a stream buffer read then xBytesToStoreMessageLength will
             * be 0. */
            ఒకవేళ( x_బైట్లు_అందుబాటు <= x_బైట్లు_గమ్యం_Store_సందేశం_నిడివి )
            {
                /* Clear notification state as going to wait for data. */
                ( శూన్య ) xTaskNotifyStateClear( ఖాళీ );

                /* Should only be one reader. */
                configASSERT( px_ప్రవాహం_బఫరు->x_Task_నిరీక్షణ_గమ్యం_Receive == ఖాళీ );
                px_ప్రవాహం_బఫరు->x_Task_నిరీక్షణ_గమ్యం_Receive = xTaskGetCurrentTaskHandle();
            }
            లేకపోతే
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        taskEXIT_CRITICAL( &( px_ప్రవాహం_బఫరు->x_ప్రవాహం_బఫరు_తాళం ) );

        ఒకవేళ( x_బైట్లు_అందుబాటు <= x_బైట్లు_గమ్యం_Store_సందేశం_నిడివి )
        {
            /* Wait for data to be available. */
            trace_BLOCKING_ON_ప్రవాహం_బఫరు_RECEIVE( x_ప్రవాహం_బఫరు );
            ( శూన్య ) xTaskNotifyWait( ( uint32_t ) 0, ( uint32_t ) 0, ఖాళీ, x_Ticks_గమ్యం_Wait );

            /* In SMP mode, the task may have been woken and scheduled on
             * another core. Hence, we must clear the xTaskWaitingToReceive
             * handle in a critical section. */
            #ఒకవేళ ( config_సంఖ్య_యొక్క_CORES > 1 )
                taskENTER_CRITICAL( &( px_ప్రవాహం_బఫరు->x_ప్రవాహం_బఫరు_తాళం ) );
            #అయితే_ముగింపు /* configNUMBER_OF_CORES > 1 */
            {
                px_ప్రవాహం_బఫరు->x_Task_నిరీక్షణ_గమ్యం_Receive = ఖాళీ;
            }
            #ఒకవేళ ( config_సంఖ్య_యొక్క_CORES > 1 )
                taskEXIT_CRITICAL( &( px_ప్రవాహం_బఫరు->x_ప్రవాహం_బఫరు_తాళం ) );
            #అయితే_ముగింపు /* configNUMBER_OF_CORES > 1 */

            /* Recheck the data available after blocking. */
            x_బైట్లు_అందుబాటు = prv_బైట్లు_లోపల_బఫరు( px_ప్రవాహం_బఫరు );
        }
        లేకపోతే
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    లేకపోతే
    {
        x_బైట్లు_అందుబాటు = prv_బైట్లు_లోపల_బఫరు( px_ప్రవాహం_బఫరు );
    }

    /* Whether receiving a discrete message (where xBytesToStoreMessageLength
     * holds the number of bytes used to store the message length) or a stream of
     * bytes (where xBytesToStoreMessageLength is zero), the number of bytes
     * available must be greater than xBytesToStoreMessageLength to be able to
     * read bytes from the buffer. */
    ఒకవేళ( x_బైట్లు_అందుబాటు > x_బైట్లు_గమ్యం_Store_సందేశం_నిడివి )
    {
        x_Received_నిడివి = prv_Read_సందేశం_From_బఫరు( px_ప్రవాహం_బఫరు, pv_Rx_దత్తాంశం, x_బఫరు_నిడివి_బైట్లు, x_బైట్లు_అందుబాటు );

        /* Was a task waiting for space in the buffer? */
        ఒకవేళ( x_Received_నిడివి != ( size_t ) 0 )
        {
            trace_ప్రవాహం_బఫరు_RECEIVE( x_ప్రవాహం_బఫరు, x_Received_నిడివి );
            prvRECEIVE_COMPLETED( x_ప్రవాహం_బఫరు );
        }
        లేకపోతే
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    లేకపోతే
    {
        trace_ప్రవాహం_బఫరు_RECEIVE_FAILED( x_ప్రవాహం_బఫరు );
        mtCOVERAGE_TEST_MARKER();
    }

    తిరిగి x_Received_నిడివి;
}
/*-----------------------------------------------------------*/

size_t x_ప్రవాహం_బఫరు_తదుపరి_సందేశం_నిడివి_బైట్లు( ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు )
{
    ప్రవాహం_బఫరు_t * స్థిరరాశి px_ప్రవాహం_బఫరు = x_ప్రవాహం_బఫరు;
    size_t xReturn, x_బైట్లు_అందుబాటు;
    config_సందేశం_బఫరు_నిడివి_TYPE x_తాత్కాలిక_Return;

    configASSERT( px_ప్రవాహం_బఫరు );

    /* Ensure the stream buffer is being used as a message buffer. */
    ఒకవేళ( ( px_ప్రవాహం_బఫరు->ucFlags & sb_FLAGS_IS_సందేశం_బఫరు ) != ( uint8_t ) 0 )
    {
        x_బైట్లు_అందుబాటు = prv_బైట్లు_లోపల_బఫరు( px_ప్రవాహం_బఫరు );

        ఒకవేళ( x_బైట్లు_అందుబాటు > sb_బైట్లు_గమ్యం_STORE_సందేశం_నిడివి )
        {
            /* The number of bytes available is greater than the number of bytes
             * required to hold the length of the next message, so another message
             * is available. */
            ( శూన్య ) prv_Read_బైట్లు_From_బఫరు( px_ప్రవాహం_బఫరు, ( uint8_t * ) &x_తాత్కాలిక_Return, sb_బైట్లు_గమ్యం_STORE_సందేశం_నిడివి, px_ప్రవాహం_బఫరు->x_తోక );
            xReturn = ( size_t ) x_తాత్కాలిక_Return;
        }
        లేకపోతే
        {
            /* The minimum amount of bytes in a message buffer is
             * ( sbBYTES_TO_STORE_MESSAGE_LENGTH + 1 ), so if xBytesAvailable is
             * less than sbBYTES_TO_STORE_MESSAGE_LENGTH the only other valid
             * value is 0. */
            configASSERT( x_బైట్లు_అందుబాటు == 0 );
            xReturn = 0;
        }
    }
    లేకపోతే
    {
        xReturn = 0;
    }

    తిరిగి xReturn;
}
/*-----------------------------------------------------------*/

size_t x_ప్రవాహం_బఫరు_Receive_From_ISR( ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు,
                                    శూన్య * pv_Rx_దత్తాంశం,
                                    size_t x_బఫరు_నిడివి_బైట్లు,
                                    పునాది_Type_t * స్థిరరాశి px_Higher_ప్రాధాన్యత_Task_Woken )
{
    ప్రవాహం_బఫరు_t * స్థిరరాశి px_ప్రవాహం_బఫరు = x_ప్రవాహం_బఫరు;
    size_t x_Received_నిడివి = 0, x_బైట్లు_అందుబాటు, x_బైట్లు_గమ్యం_Store_సందేశం_నిడివి;

    configASSERT( pv_Rx_దత్తాంశం );
    configASSERT( px_ప్రవాహం_బఫరు );

    /* This receive function is used by both message buffers, which store
     * discrete messages, and stream buffers, which store a continuous stream of
     * bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the
     * message. */
    ఒకవేళ( ( px_ప్రవాహం_బఫరు->ucFlags & sb_FLAGS_IS_సందేశం_బఫరు ) != ( uint8_t ) 0 )
    {
        x_బైట్లు_గమ్యం_Store_సందేశం_నిడివి = sb_బైట్లు_గమ్యం_STORE_సందేశం_నిడివి;
    }
    లేకపోతే
    {
        x_బైట్లు_గమ్యం_Store_సందేశం_నిడివి = 0;
    }

    x_బైట్లు_అందుబాటు = prv_బైట్లు_లోపల_బఫరు( px_ప్రవాహం_బఫరు );

    /* Whether receiving a discrete message (where xBytesToStoreMessageLength
     * holds the number of bytes used to store the message length) or a stream of
     * bytes (where xBytesToStoreMessageLength is zero), the number of bytes
     * available must be greater than xBytesToStoreMessageLength to be able to
     * read bytes from the buffer. */
    ఒకవేళ( x_బైట్లు_అందుబాటు > x_బైట్లు_గమ్యం_Store_సందేశం_నిడివి )
    {
        x_Received_నిడివి = prv_Read_సందేశం_From_బఫరు( px_ప్రవాహం_బఫరు, pv_Rx_దత్తాంశం, x_బఫరు_నిడివి_బైట్లు, x_బైట్లు_అందుబాటు );

        /* Was a task waiting for space in the buffer? */
        ఒకవేళ( x_Received_నిడివి != ( size_t ) 0 )
        {
            prvRECEIVE_COMPLETED_FROM_ISR( px_ప్రవాహం_బఫరు, px_Higher_ప్రాధాన్యత_Task_Woken );
        }
        లేకపోతే
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    లేకపోతే
    {
        mtCOVERAGE_TEST_MARKER();
    }

    trace_ప్రవాహం_బఫరు_RECEIVE_FROM_ISR( x_ప్రవాహం_బఫరు, x_Received_నిడివి );

    తిరిగి x_Received_నిడివి;
}
/*-----------------------------------------------------------*/

స్థిర size_t prv_Read_సందేశం_From_బఫరు( ప్రవాహం_బఫరు_t * px_ప్రవాహం_బఫరు,
                                        శూన్య * pv_Rx_దత్తాంశం,
                                        size_t x_బఫరు_నిడివి_బైట్లు,
                                        size_t x_బైట్లు_అందుబాటు )
{
    size_t x_లెక్క, x_తదుపరి_సందేశం_నిడివి;
    config_సందేశం_బఫరు_నిడివి_TYPE x_తాత్కాలిక_తదుపరి_సందేశం_నిడివి;
    size_t x_తదుపరి_తోక = px_ప్రవాహం_బఫరు->x_తోక;

    ఒకవేళ( ( px_ప్రవాహం_బఫరు->ucFlags & sb_FLAGS_IS_సందేశం_బఫరు ) != ( uint8_t ) 0 )
    {
        /* A discrete message is being received.  First receive the length
         * of the message. */
        x_తదుపరి_తోక = prv_Read_బైట్లు_From_బఫరు( px_ప్రవాహం_బఫరు, ( uint8_t * ) &x_తాత్కాలిక_తదుపరి_సందేశం_నిడివి, sb_బైట్లు_గమ్యం_STORE_సందేశం_నిడివి, x_తదుపరి_తోక );
        x_తదుపరి_సందేశం_నిడివి = ( size_t ) x_తాత్కాలిక_తదుపరి_సందేశం_నిడివి;

        /* Reduce the number of bytes available by the number of bytes just
         * read out. */
        x_బైట్లు_అందుబాటు -= sb_బైట్లు_గమ్యం_STORE_సందేశం_నిడివి;

        /* Check there is enough space in the buffer provided by the
         * user. */
        ఒకవేళ( x_తదుపరి_సందేశం_నిడివి > x_బఫరు_నిడివి_బైట్లు )
        {
            /* The user has provided insufficient space to read the message. */
            x_తదుపరి_సందేశం_నిడివి = 0;
        }
        లేకపోతే
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    లేకపోతే
    {
        /* A stream of bytes is being received (as opposed to a discrete
         * message), so read as many bytes as possible. */
        x_తదుపరి_సందేశం_నిడివి = x_బఫరు_నిడివి_బైట్లు;
    }

    /* Use the minimum of the wanted bytes and the available bytes. */
    x_లెక్క = configMIN( x_తదుపరి_సందేశం_నిడివి, x_బైట్లు_అందుబాటు );

    ఒకవేళ( x_లెక్క != ( size_t ) 0 )
    {
        /* Read the actual data and update the tail to mark the data as officially consumed. */
        px_ప్రవాహం_బఫరు->x_తోక = prv_Read_బైట్లు_From_బఫరు( px_ప్రవాహం_బఫరు, ( uint8_t * ) pv_Rx_దత్తాంశం, x_లెక్క, x_తదుపరి_తోక ); /*lint !e9079 Data storage area is implemented as uint8_t array for ease of sizing, indexing and alignment. */
    }

    తిరిగి x_లెక్క;
}
/*-----------------------------------------------------------*/

పునాది_Type_t x_ప్రవాహం_బఫరు_Is_ఖాళీలు( ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు )
{
    స్థిరరాశి ప్రవాహం_బఫరు_t * స్థిరరాశి px_ప్రవాహం_బఫరు = x_ప్రవాహం_బఫరు;
    పునాది_Type_t xReturn;
    size_t x_తోక;

    configASSERT( px_ప్రవాహం_బఫరు );

    /* True if no bytes are available. */
    x_తోక = px_ప్రవాహం_బఫరు->x_తోక;

    ఒకవేళ( px_ప్రవాహం_బఫరు->x_తల == x_తోక )
    {
        xReturn = pdTRUE;
    }
    లేకపోతే
    {
        xReturn = pdFALSE;
    }

    తిరిగి xReturn;
}
/*-----------------------------------------------------------*/

పునాది_Type_t x_ప్రవాహం_బఫరు_Is_నిండిన( ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు )
{
    పునాది_Type_t xReturn;
    size_t x_బైట్లు_గమ్యం_Store_సందేశం_నిడివి;
    స్థిరరాశి ప్రవాహం_బఫరు_t * స్థిరరాశి px_ప్రవాహం_బఫరు = x_ప్రవాహం_బఫరు;

    configASSERT( px_ప్రవాహం_బఫరు );

    /* This generic version of the receive function is used by both message
     * buffers, which store discrete messages, and stream buffers, which store a
     * continuous stream of bytes.  Discrete messages include an additional
     * sbBYTES_TO_STORE_MESSAGE_LENGTH bytes that hold the length of the message. */
    ఒకవేళ( ( px_ప్రవాహం_బఫరు->ucFlags & sb_FLAGS_IS_సందేశం_బఫరు ) != ( uint8_t ) 0 )
    {
        x_బైట్లు_గమ్యం_Store_సందేశం_నిడివి = sb_బైట్లు_గమ్యం_STORE_సందేశం_నిడివి;
    }
    లేకపోతే
    {
        x_బైట్లు_గమ్యం_Store_సందేశం_నిడివి = 0;
    }

    /* True if the available space equals zero. */
    ఒకవేళ( x_ప్రవాహం_బఫరు_Spaces_అందుబాటు( x_ప్రవాహం_బఫరు ) <= x_బైట్లు_గమ్యం_Store_సందేశం_నిడివి )
    {
        xReturn = pdTRUE;
    }
    లేకపోతే
    {
        xReturn = pdFALSE;
    }

    తిరిగి xReturn;
}
/*-----------------------------------------------------------*/

పునాది_Type_t x_ప్రవాహం_బఫరు_Send_Completed_From_ISR( ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు,
                                              పునాది_Type_t * px_Higher_ప్రాధాన్యత_Task_Woken )
{
    ప్రవాహం_బఫరు_t * స్థిరరాశి px_ప్రవాహం_బఫరు = x_ప్రవాహం_బఫరు;
    పునాది_Type_t xReturn;
    UBaseType_t ux_Saved_అంతరాయం_స్థితి;

    configASSERT( px_ప్రవాహం_బఫరు );

    ux_Saved_అంతరాయం_స్థితి = ( UBaseType_t ) పోర్టు_సమితి_అంతరాయం_ముసుగు_FROM_ISR();
    {
        ఒకవేళ( ( px_ప్రవాహం_బఫరు )->x_Task_నిరీక్షణ_గమ్యం_Receive != ఖాళీ )
        {
            ( శూన్య ) xTaskNotifyFromISR( ( px_ప్రవాహం_బఫరు )->x_Task_నిరీక్షణ_గమ్యం_Receive,
                                         ( uint32_t ) 0,
                                         eNoAction,
                                         px_Higher_ప్రాధాన్యత_Task_Woken );
            ( px_ప్రవాహం_బఫరు )->x_Task_నిరీక్షణ_గమ్యం_Receive = ఖాళీ;
            xReturn = pdTRUE;
        }
        లేకపోతే
        {
            xReturn = pdFALSE;
        }
    }
    పోర్టు_CLEAR_అంతరాయం_ముసుగు_FROM_ISR( ux_Saved_అంతరాయం_స్థితి );

    తిరిగి xReturn;
}
/*-----------------------------------------------------------*/

పునాది_Type_t x_ప్రవాహం_బఫరు_Receive_Completed_From_ISR( ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు,
                                                 పునాది_Type_t * px_Higher_ప్రాధాన్యత_Task_Woken )
{
    ప్రవాహం_బఫరు_t * స్థిరరాశి px_ప్రవాహం_బఫరు = x_ప్రవాహం_బఫరు;
    పునాది_Type_t xReturn;
    UBaseType_t ux_Saved_అంతరాయం_స్థితి;

    configASSERT( px_ప్రవాహం_బఫరు );

    ux_Saved_అంతరాయం_స్థితి = ( UBaseType_t ) పోర్టు_సమితి_అంతరాయం_ముసుగు_FROM_ISR();
    {
        ఒకవేళ( ( px_ప్రవాహం_బఫరు )->x_Task_నిరీక్షణ_గమ్యం_Send != ఖాళీ )
        {
            ( శూన్య ) xTaskNotifyFromISR( ( px_ప్రవాహం_బఫరు )->x_Task_నిరీక్షణ_గమ్యం_Send,
                                         ( uint32_t ) 0,
                                         eNoAction,
                                         px_Higher_ప్రాధాన్యత_Task_Woken );
            ( px_ప్రవాహం_బఫరు )->x_Task_నిరీక్షణ_గమ్యం_Send = ఖాళీ;
            xReturn = pdTRUE;
        }
        లేకపోతే
        {
            xReturn = pdFALSE;
        }
    }
    పోర్టు_CLEAR_అంతరాయం_ముసుగు_FROM_ISR( ux_Saved_అంతరాయం_స్థితి );

    తిరిగి xReturn;
}
/*-----------------------------------------------------------*/

స్థిర size_t prv_Write_బైట్లు_గమ్యం_బఫరు( ప్రవాహం_బఫరు_t * స్థిరరాశి px_ప్రవాహం_బఫరు,
                                     స్థిరరాశి uint8_t * puc_దత్తాంశం,
                                     size_t x_లెక్క,
                                     size_t x_తల )
{
    size_t x_మొదటి_నిడివి;

    configASSERT( x_లెక్క > ( size_t ) 0 );

    /* Calculate the number of bytes that can be added in the first write -
     * which may be less than the total number of bytes that need to be added if
     * the buffer will wrap back to the beginning. */
    x_మొదటి_నిడివి = configMIN( px_ప్రవాహం_బఫరు->x_నిడివి - x_తల, x_లెక్క );

    /* Write as many bytes as can be written in the first write. */
    configASSERT( ( x_తల + x_మొదటి_నిడివి ) <= px_ప్రవాహం_బఫరు->x_నిడివి );
    ( శూన్య ) జ్ఞాపకం_నకలు( ( శూన్య * ) ( &( px_ప్రవాహం_బఫరు->puc_బఫరు[ x_తల ] ) ), ( స్థిరరాశి శూన్య * ) puc_దత్తాంశం, x_మొదటి_నిడివి ); /*lint !e9087 memcpy() requires void *. */

    /* If the number of bytes written was less than the number that could be
     * written in the first write... */
    ఒకవేళ( x_లెక్క > x_మొదటి_నిడివి )
    {
        /* ...then write the remaining bytes to the start of the buffer. */
        configASSERT( ( x_లెక్క - x_మొదటి_నిడివి ) <= px_ప్రవాహం_బఫరు->x_నిడివి );
        ( శూన్య ) జ్ఞాపకం_నకలు( ( శూన్య * ) px_ప్రవాహం_బఫరు->puc_బఫరు, ( స్థిరరాశి శూన్య * ) &( puc_దత్తాంశం[ x_మొదటి_నిడివి ] ), x_లెక్క - x_మొదటి_నిడివి ); /*lint !e9087 memcpy() requires void *. */
    }
    లేకపోతే
    {
        mtCOVERAGE_TEST_MARKER();
    }

    x_తల += x_లెక్క;

    ఒకవేళ( x_తల >= px_ప్రవాహం_బఫరు->x_నిడివి )
    {
        x_తల -= px_ప్రవాహం_బఫరు->x_నిడివి;
    }
    లేకపోతే
    {
        mtCOVERAGE_TEST_MARKER();
    }

    తిరిగి x_తల;
}
/*-----------------------------------------------------------*/

స్థిర size_t prv_Read_బైట్లు_From_బఫరు( ప్రవాహం_బఫరు_t * px_ప్రవాహం_బఫరు,
                                      uint8_t * puc_దత్తాంశం,
                                      size_t x_లెక్క,
                                      size_t x_తోక )
{
    size_t x_మొదటి_నిడివి;

    configASSERT( x_లెక్క != ( size_t ) 0 );

    /* Calculate the number of bytes that can be read - which may be
     * less than the number wanted if the data wraps around to the start of
     * the buffer. */
    x_మొదటి_నిడివి = configMIN( px_ప్రవాహం_బఫరు->x_నిడివి - x_తోక, x_లెక్క );

    /* Obtain the number of bytes it is possible to obtain in the first
     * read.  Asserts check bounds of read and write. */
    configASSERT( x_మొదటి_నిడివి <= x_లెక్క );
    configASSERT( ( x_తోక + x_మొదటి_నిడివి ) <= px_ప్రవాహం_బఫరు->x_నిడివి );
    ( శూన్య ) జ్ఞాపకం_నకలు( ( శూన్య * ) puc_దత్తాంశం, ( స్థిరరాశి శూన్య * ) &( px_ప్రవాహం_బఫరు->puc_బఫరు[ x_తోక ] ), x_మొదటి_నిడివి ); /*lint !e9087 memcpy() requires void *. */

    /* If the total number of wanted bytes is greater than the number
     * that could be read in the first read... */
    ఒకవేళ( x_లెక్క > x_మొదటి_నిడివి )
    {
        /* ...then read the remaining bytes from the start of the buffer. */
        ( శూన్య ) జ్ఞాపకం_నకలు( ( శూన్య * ) &( puc_దత్తాంశం[ x_మొదటి_నిడివి ] ), ( శూన్య * ) ( px_ప్రవాహం_బఫరు->puc_బఫరు ), x_లెక్క - x_మొదటి_నిడివి ); /*lint !e9087 memcpy() requires void *. */
    }
    లేకపోతే
    {
        mtCOVERAGE_TEST_MARKER();
    }

    /* Move the tail pointer to effectively remove the data read from the buffer. */
    x_తోక += x_లెక్క;

    ఒకవేళ( x_తోక >= px_ప్రవాహం_బఫరు->x_నిడివి )
    {
        x_తోక -= px_ప్రవాహం_బఫరు->x_నిడివి;
    }

    తిరిగి x_తోక;
}
/*-----------------------------------------------------------*/

స్థిర size_t prv_బైట్లు_లోపల_బఫరు( స్థిరరాశి ప్రవాహం_బఫరు_t * స్థిరరాశి px_ప్రవాహం_బఫరు )
{
/* Returns the distance between xTail and xHead. */
    size_t x_లెక్క;

    x_లెక్క = px_ప్రవాహం_బఫరు->x_నిడివి + px_ప్రవాహం_బఫరు->x_తల;
    x_లెక్క -= px_ప్రవాహం_బఫరు->x_తోక;

    ఒకవేళ( x_లెక్క >= px_ప్రవాహం_బఫరు->x_నిడివి )
    {
        x_లెక్క -= px_ప్రవాహం_బఫరు->x_నిడివి;
    }
    లేకపోతే
    {
        mtCOVERAGE_TEST_MARKER();
    }

    తిరిగి x_లెక్క;
}
/*-----------------------------------------------------------*/

స్థిర శూన్య prv_Initialise_New_ప్రవాహం_బఫరు( ప్రవాహం_బఫరు_t * స్థిరరాశి px_ప్రవాహం_బఫరు,
                                          uint8_t * స్థిరరాశి puc_బఫరు,
                                          size_t x_బఫరు_కొలత_బైట్లు,
                                          size_t x_Trigger_Level_బైట్లు,
                                          uint8_t ucFlags,
                                          ప్రవాహం_బఫరు_Callback_Function_t pxSendCompletedCallback,
                                          ప్రవాహం_బఫరు_Callback_Function_t pxReceiveCompletedCallback )
{
    /* Assert here is deliberately writing to the entire buffer to ensure it can
     * be written to without generating exceptions, and is setting the buffer to a
     * known value to assist in development/debugging. */
    #ఒకవేళ ( configASSERT_DEFINED == 1 )
    {
        /* The value written just has to be identifiable when looking at the
         * memory.  Don't use 0xA5 as that is the stack fill value and could
         * result in confusion as to what is actually being observed. */
        స్థిరరాశి పునాది_Type_t x_Write_విలువ = 0x55;
        configASSERT( memset( puc_బఫరు, ( పూర్ణాంకం ) x_Write_విలువ, x_బఫరు_కొలత_బైట్లు ) == puc_బఫరు );
        ( శూన్య ) x_Write_విలువ;
    } /*lint !e529 !e438 xWriteValue is only used if configASSERT() is defined. */
    #అయితే_ముగింపు

    /* This function could be called from xStreamBufferReset(), so we reset the
     * stream buffer fields manually in order to avoid clearing
     * xStreamBufferLock. The xStreamBufferLock is initialized separately on
     * stream buffer creation. */
    px_ప్రవాహం_బఫరు->x_తోక = ( size_t ) 0;
    px_ప్రవాహం_బఫరు->x_తల = ( size_t ) 0;
    px_ప్రవాహం_బఫరు->x_Task_నిరీక్షణ_గమ్యం_Receive = ( TaskHandle_t ) 0;
    px_ప్రవాహం_బఫరు->x_Task_నిరీక్షణ_గమ్యం_Send = ( TaskHandle_t ) 0;
    #ఒకవేళ ( configUSE_TRACE_FACILITY == 1 )
        px_ప్రవాహం_బఫరు->ux_ప్రవాహం_బఫరు_సంఖ్య = ( UBaseType_t ) 0;
    #అయితే_ముగింపు
    px_ప్రవాహం_బఫరు->puc_బఫరు = puc_బఫరు;
    px_ప్రవాహం_బఫరు->x_నిడివి = x_బఫరు_కొలత_బైట్లు;
    px_ప్రవాహం_బఫరు->x_Trigger_Level_బైట్లు = x_Trigger_Level_బైట్లు;
    px_ప్రవాహం_బఫరు->ucFlags = ucFlags;
    #ఒకవేళ ( configUSE_SB_COMPLETED_CALLBACK == 1 )
    {
        px_ప్రవాహం_బఫరు->pxSendCompletedCallback = pxSendCompletedCallback;
        px_ప్రవాహం_బఫరు->pxReceiveCompletedCallback = pxReceiveCompletedCallback;
    }
    #లేకపోతే
    {
        ( శూన్య ) pxSendCompletedCallback;
        ( శూన్య ) pxReceiveCompletedCallback;
    }
    #అయితే_ముగింపు
}

#ఒకవేళ ( configUSE_TRACE_FACILITY == 1 )

    UBaseType_t ux_ప్రవాహం_బఫరు_Get_ప్రవాహం_బఫరు_సంఖ్య( ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు )
    {
        తిరిగి x_ప్రవాహం_బఫరు->ux_ప్రవాహం_బఫరు_సంఖ్య;
    }

#అయితే_ముగింపు /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#ఒకవేళ ( configUSE_TRACE_FACILITY == 1 )

    శూన్య v_ప్రవాహం_బఫరు_సమితి_ప్రవాహం_బఫరు_సంఖ్య( ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు,
                                             UBaseType_t ux_ప్రవాహం_బఫరు_సంఖ్య )
    {
        x_ప్రవాహం_బఫరు->ux_ప్రవాహం_బఫరు_సంఖ్య = ux_ప్రవాహం_బఫరు_సంఖ్య;
    }

#అయితే_ముగింపు /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#ఒకవేళ ( configUSE_TRACE_FACILITY == 1 )

    uint8_t uc_ప్రవాహం_బఫరు_Get_ప్రవాహం_బఫరు_Type( ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు )
    {
        తిరిగి( x_ప్రవాహం_బఫరు->ucFlags & sb_FLAGS_IS_సందేశం_బఫరు );
    }

#అయితే_ముగింపు /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/
