/*
 * FreeRTOS Kernel V10.5.1 (ESP-IDF SMP modified)
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-FileCopyrightText: 2021 Amazon.com, Inc. or its affiliates
 *
 * SPDX-License-Identifier: MIT
 *
 * SPDX-FileContributor: 2023 Espressif Systems (Shanghai) CO LTD
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


#నిర్వచితం_కాకపోతే వరుస_H
#నిర్వచించు వరుస_H

#నిర్వచితం_కాకపోతే INC_FREERTOS_H
    #దోషం "include FreeRTOS.h" must appear లోపల ఆధారం files before "include queue.h"
#అయితే_ముగింపు

/* *INDENT-OFF* */
#నిర్వచితమైతే __cplusplus
    బాహ్య "C" {
#అయితే_ముగింపు
/* *INDENT-ON* */

#అన్తర్భావ "task.h"

/**
 * Type by which queues are referenced.  For example, a call to xQueueCreate()
 * returns an QueueHandle_t variable that can then be used as a parameter to
 * xQueueSend(), xQueueReceive(), etc.
 */
నిర్మాణం వరుస_Definition; /* Using old naming convention so as not to break kernel aware debuggers. */
రకనిర్వచనం నిర్మాణం వరుస_Definition   * వరుస_Handle_t;

/**
 * Type by which queue sets are referenced.  For example, a call to
 * xQueueCreateSet() returns an xQueueSet variable that can then be used as a
 * parameter to xQueueSelectFromSet(), xQueueAddToSet(), etc.
 */
రకనిర్వచనం నిర్మాణం వరుస_Definition   * వరుస_సమితి_Handle_t;

/**
 * Queue sets can contain both queues and semaphores, so the
 * QueueSetMemberHandle_t is defined as a type to be used where a parameter or
 * return value can be either an QueueHandle_t or an SemaphoreHandle_t.
 */
రకనిర్వచనం నిర్మాణం వరుస_Definition   * వరుస_సమితి_Member_Handle_t;

/** @cond !DOC_EXCLUDE_HEADER_SECTION */

/* For internal use only. */
#నిర్వచించు వరుస_SEND_గమ్యం_వెనుక                     ( ( పునాది_Type_t ) 0 )
#నిర్వచించు వరుస_SEND_గమ్యం_FRONT                    ( ( పునాది_Type_t ) 1 )
#నిర్వచించు వరుస_OVERWRITE                        ( ( పునాది_Type_t ) 2 )

/* For internal use only.  These definitions *must* match those in queue.c. */
#నిర్వచించు వరుస_వరుస_TYPE_పునాది                  ( ( uint8_t ) 0U )
#నిర్వచించు వరుస_వరుస_TYPE_సమితి                   ( ( uint8_t ) 0U )
#నిర్వచించు వరుస_వరుస_TYPE_MUTEX                 ( ( uint8_t ) 1U )
#నిర్వచించు వరుస_వరుస_TYPE_COUNTING_SEMAPHORE    ( ( uint8_t ) 2U )
#నిర్వచించు వరుస_వరుస_TYPE_ద్విమాన_SEMAPHORE      ( ( uint8_t ) 3U )
#నిర్వచించు వరుస_వరుస_TYPE_RECURSIVE_MUTEX       ( ( uint8_t ) 4U )

/** @endcond */

/**
 *
 * Creates a new queue instance, and returns a handle by which the new queue
 * can be referenced.
 *
 * Internally, within the FreeRTOS implementation, queues use two blocks of
 * memory.  The first block is used to hold the queue's data structures.  The
 * second block is used to hold items placed into the queue.  If a queue is
 * created using xQueueCreate() then both blocks of memory are automatically
 * dynamically allocated inside the xQueueCreate() function.  (see
 * https://www.FreeRTOS.org/a00111.html).  If a queue is created using
 * xQueueCreateStatic() then the application writer must provide the memory that
 * will get used by the queue.  xQueueCreateStatic() therefore allows a queue to
 * be created without using any dynamic memory allocation.
 *
 * https://www.FreeRTOS.org/Embedded-RTOS-Queues.html
 *
 * @param uxQueueLength The maximum number of items that the queue can contain.
 *
 * @param uxItemSize The number of bytes each item in the queue will require.
 * Items are queued by copy, not by reference, so this is the number of bytes
 * that will be copied for each posted item.  Each item on the queue must be
 * the same size.
 *
 * @return If the queue is successfully create then a handle to the newly
 * created queue is returned.  If the queue cannot be created then 0 is
 * returned.
 *
 * Example usage:
 * @code{c}
 * struct AMessage
 * {
 *  char ucMessageID;
 *  char ucData[ 20 ];
 * };
 *
 * void vATask( void *pvParameters )
 * {
 * QueueHandle_t xQueue1, xQueue2;
 *
 *  // Create a queue capable of containing 10 uint32_t values.
 *  xQueue1 = xQueueCreate( 10, sizeof( uint32_t ) );
 *  if( xQueue1 == 0 )
 *  {
 *      // Queue was not created and must not be used.
 *  }
 *
 *  // Create a queue capable of containing 10 pointers to AMessage structures.
 *  // These should be passed by pointer as they contain a lot of data.
 *  xQueue2 = xQueueCreate( 10, sizeof( struct AMessage * ) );
 *  if( xQueue2 == 0 )
 *  {
 *      // Queue was not created and must not be used.
 *  }
 *
 *  // ... Rest of task code.
 * }
 * @endcode
 * \ingroup QueueManagement
 */
#ఒకవేళ ( config_SUPPORT_DYNAMIC_కేటాయింపు == 1 )
    #నిర్వచించు x_వరుస_Create( ux_వరుస_నిడివి, ux_అంశం_కొలత )    x_వరుస_Generic_Create( ( ux_వరుస_నిడివి ), ( ux_అంశం_కొలత ), ( వరుస_వరుస_TYPE_పునాది ) )
#అయితే_ముగింపు

/**
 *
 * Creates a new queue instance, and returns a handle by which the new queue
 * can be referenced.
 *
 * Internally, within the FreeRTOS implementation, queues use two blocks of
 * memory.  The first block is used to hold the queue's data structures.  The
 * second block is used to hold items placed into the queue.  If a queue is
 * created using xQueueCreate() then both blocks of memory are automatically
 * dynamically allocated inside the xQueueCreate() function.  (see
 * https://www.FreeRTOS.org/a00111.html).  If a queue is created using
 * xQueueCreateStatic() then the application writer must provide the memory that
 * will get used by the queue.  xQueueCreateStatic() therefore allows a queue to
 * be created without using any dynamic memory allocation.
 *
 * https://www.FreeRTOS.org/Embedded-RTOS-Queues.html
 *
 * @param uxQueueLength The maximum number of items that the queue can contain.
 *
 * @param uxItemSize The number of bytes each item in the queue will require.
 * Items are queued by copy, not by reference, so this is the number of bytes
 * that will be copied for each posted item.  Each item on the queue must be
 * the same size.
 *
 * @param pucQueueStorage If uxItemSize is not zero then
 * pucQueueStorage must point to a uint8_t array that is at least large
 * enough to hold the maximum number of items that can be in the queue at any
 * one time - which is ( uxQueueLength * uxItemsSize ) bytes.  If uxItemSize is
 * zero then pucQueueStorage can be NULL.
 *
 * @param pxQueueBuffer Must point to a variable of type StaticQueue_t, which
 * will be used to hold the queue's data structure.
 *
 * @return If the queue is created then a handle to the created queue is
 * returned.  If pxQueueBuffer is NULL then NULL is returned.
 *
 * Example usage:
 * @code{c}
 * struct AMessage
 * {
 *  char ucMessageID;
 *  char ucData[ 20 ];
 * };
 *
 * #define QUEUE_LENGTH 10
 * #define ITEM_SIZE sizeof( uint32_t )
 *
 * // xQueueBuffer will hold the queue structure.
 * StaticQueue_t xQueueBuffer;
 *
 * // ucQueueStorage will hold the items posted to the queue.  Must be at least
 * // [(queue length) * ( queue item size)] bytes long.
 * uint8_t ucQueueStorage[ QUEUE_LENGTH * ITEM_SIZE ];
 *
 * void vATask( void *pvParameters )
 * {
 *  QueueHandle_t xQueue1;
 *
 *  // Create a queue capable of containing 10 uint32_t values.
 *  xQueue1 = xQueueCreate( QUEUE_LENGTH, // The number of items the queue can hold.
 *                          ITEM_SIZE     // The size of each item in the queue
 *                          &( ucQueueStorage[ 0 ] ), // The buffer that will hold the items in the queue.
 *                          &xQueueBuffer ); // The buffer that will hold the queue structure.
 *
 *  // The queue is guaranteed to be created successfully as no dynamic memory
 *  // allocation is used.  Therefore xQueue1 is now a handle to a valid queue.
 *
 *  // ... Rest of task code.
 * }
 * @endcode
 * \ingroup QueueManagement
 */
#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )
    #నిర్వచించు x_వరుస_Create_Static( ux_వరుస_నిడివి, ux_అంశం_కొలత, puc_వరుస_Storage, px_వరుస_బఫరు )    x_వరుస_Generic_Create_Static( ( ux_వరుస_నిడివి ), ( ux_అంశం_కొలత ), ( puc_వరుస_Storage ), ( px_వరుస_బఫరు ), ( వరుస_వరుస_TYPE_పునాది ) )
#అయితే_ముగింపు /* configSUPPORT_STATIC_ALLOCATION */

/**
 *
 * Retrieve pointers to a statically created queue's data structure buffer
 * and storage area buffer. These are the same buffers that are supplied
 * at the time of creation.
 *
 * @param xQueue The queue for which to retrieve the buffers.
 *
 * @param ppucQueueStorage Used to return a pointer to the queue's storage
 * area buffer.
 *
 * @param ppxStaticQueue Used to return a pointer to the queue's data
 * structure buffer.
 *
 * @return pdTRUE if buffers were retrieved, pdFALSE otherwise.
 *
 * \ingroup QueueManagement
 */
#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )
    #నిర్వచించు x_వరుస_Get_Static_Buffers( x_వరుస, ppuc_వరుస_Storage, ppx_Static_వరుస )    x_వరుస_Generic_Get_Static_Buffers( ( x_వరుస ), ( ppuc_వరుస_Storage ), ( ppx_Static_వరుస ) )
#అయితే_ముగింపు /* configSUPPORT_STATIC_ALLOCATION */

/**
 *
 * Post an item to the front of a queue.  The item is queued by copy, not by
 * reference.  This function must not be called from an interrupt service
 * routine.  See xQueueSendFromISR () for an alternative which may be used
 * in an ISR.
 *
 * @param xQueue The handle to the queue on which the item is to be posted.
 *
 * @param pvItemToQueue A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvItemToQueue
 * into the queue storage area.
 *
 * @param xTicksToWait The maximum amount of time the task should block
 * waiting for space to become available on the queue, should it already
 * be full.  The call will return immediately if this is set to 0 and the
 * queue is full.  The time is defined in tick periods so the constant
 * portTICK_PERIOD_MS should be used to convert to real time if this is required.
 *
 * @return pdTRUE if the item was successfully posted, otherwise errQUEUE_FULL.
 *
 * Example usage:
 * @code{c}
 * struct AMessage
 * {
 *  char ucMessageID;
 *  char ucData[ 20 ];
 * } xMessage;
 *
 * uint32_t ulVar = 10UL;
 *
 * void vATask( void *pvParameters )
 * {
 * QueueHandle_t xQueue1, xQueue2;
 * struct AMessage *pxMessage;
 *
 *  // Create a queue capable of containing 10 uint32_t values.
 *  xQueue1 = xQueueCreate( 10, sizeof( uint32_t ) );
 *
 *  // Create a queue capable of containing 10 pointers to AMessage structures.
 *  // These should be passed by pointer as they contain a lot of data.
 *  xQueue2 = xQueueCreate( 10, sizeof( struct AMessage * ) );
 *
 *  // ...
 *
 *  if( xQueue1 != 0 )
 *  {
 *      // Send an uint32_t.  Wait for 10 ticks for space to become
 *      // available if necessary.
 *      if( xQueueSendToFront( xQueue1, ( void * ) &ulVar, ( TickType_t ) 10 ) != pdPASS )
 *      {
 *          // Failed to post the message, even after 10 ticks.
 *      }
 *  }
 *
 *  if( xQueue2 != 0 )
 *  {
 *      // Send a pointer to a struct AMessage object.  Don't block if the
 *      // queue is already full.
 *      pxMessage = & xMessage;
 *      xQueueSendToFront( xQueue2, ( void * ) &pxMessage, ( TickType_t ) 0 );
 *  }
 *
 *  // ... Rest of task code.
 * }
 * @endcode
 * \ingroup QueueManagement
 */
#నిర్వచించు x_వరుస_Send_గమ్యం_Front( x_వరుస, pv_అంశం_గమ్యం_వరుస, x_Ticks_గమ్యం_Wait ) \
    x_వరుస_Generic_Send( ( x_వరుస ), ( pv_అంశం_గమ్యం_వరుస ), ( x_Ticks_గమ్యం_Wait ), వరుస_SEND_గమ్యం_FRONT )

/**
 *
 * This is a macro that calls xQueueGenericSend().
 *
 * Post an item to the back of a queue.  The item is queued by copy, not by
 * reference.  This function must not be called from an interrupt service
 * routine.  See xQueueSendFromISR () for an alternative which may be used
 * in an ISR.
 *
 * @param xQueue The handle to the queue on which the item is to be posted.
 *
 * @param pvItemToQueue A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvItemToQueue
 * into the queue storage area.
 *
 * @param xTicksToWait The maximum amount of time the task should block
 * waiting for space to become available on the queue, should it already
 * be full.  The call will return immediately if this is set to 0 and the queue
 * is full.  The  time is defined in tick periods so the constant
 * portTICK_PERIOD_MS should be used to convert to real time if this is required.
 *
 * @return pdTRUE if the item was successfully posted, otherwise errQUEUE_FULL.
 *
 * Example usage:
 * @code{c}
 * struct AMessage
 * {
 *  char ucMessageID;
 *  char ucData[ 20 ];
 * } xMessage;
 *
 * uint32_t ulVar = 10UL;
 *
 * void vATask( void *pvParameters )
 * {
 * QueueHandle_t xQueue1, xQueue2;
 * struct AMessage *pxMessage;
 *
 *  // Create a queue capable of containing 10 uint32_t values.
 *  xQueue1 = xQueueCreate( 10, sizeof( uint32_t ) );
 *
 *  // Create a queue capable of containing 10 pointers to AMessage structures.
 *  // These should be passed by pointer as they contain a lot of data.
 *  xQueue2 = xQueueCreate( 10, sizeof( struct AMessage * ) );
 *
 *  // ...
 *
 *  if( xQueue1 != 0 )
 *  {
 *      // Send an uint32_t.  Wait for 10 ticks for space to become
 *      // available if necessary.
 *      if( xQueueSendToBack( xQueue1, ( void * ) &ulVar, ( TickType_t ) 10 ) != pdPASS )
 *      {
 *          // Failed to post the message, even after 10 ticks.
 *      }
 *  }
 *
 *  if( xQueue2 != 0 )
 *  {
 *      // Send a pointer to a struct AMessage object.  Don't block if the
 *      // queue is already full.
 *      pxMessage = & xMessage;
 *      xQueueSendToBack( xQueue2, ( void * ) &pxMessage, ( TickType_t ) 0 );
 *  }
 *
 *  // ... Rest of task code.
 * }
 * @endcode
 * \ingroup QueueManagement
 */
#నిర్వచించు x_వరుస_Send_గమ్యం_వెనుక( x_వరుస, pv_అంశం_గమ్యం_వరుస, x_Ticks_గమ్యం_Wait ) \
    x_వరుస_Generic_Send( ( x_వరుస ), ( pv_అంశం_గమ్యం_వరుస ), ( x_Ticks_గమ్యం_Wait ), వరుస_SEND_గమ్యం_వెనుక )

/**
 *
 * This is a macro that calls xQueueGenericSend().  It is included for
 * backward compatibility with versions of FreeRTOS.org that did not
 * include the xQueueSendToFront() and xQueueSendToBack() macros.  It is
 * equivalent to xQueueSendToBack().
 *
 * Post an item on a queue.  The item is queued by copy, not by reference.
 * This function must not be called from an interrupt service routine.
 * See xQueueSendFromISR () for an alternative which may be used in an ISR.
 *
 * @param xQueue The handle to the queue on which the item is to be posted.
 *
 * @param pvItemToQueue A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvItemToQueue
 * into the queue storage area.
 *
 * @param xTicksToWait The maximum amount of time the task should block
 * waiting for space to become available on the queue, should it already
 * be full.  The call will return immediately if this is set to 0 and the
 * queue is full.  The time is defined in tick periods so the constant
 * portTICK_PERIOD_MS should be used to convert to real time if this is required.
 *
 * @return pdTRUE if the item was successfully posted, otherwise errQUEUE_FULL.
 *
 * Example usage:
 * @code{c}
 * struct AMessage
 * {
 *  char ucMessageID;
 *  char ucData[ 20 ];
 * } xMessage;
 *
 * uint32_t ulVar = 10UL;
 *
 * void vATask( void *pvParameters )
 * {
 * QueueHandle_t xQueue1, xQueue2;
 * struct AMessage *pxMessage;
 *
 *  // Create a queue capable of containing 10 uint32_t values.
 *  xQueue1 = xQueueCreate( 10, sizeof( uint32_t ) );
 *
 *  // Create a queue capable of containing 10 pointers to AMessage structures.
 *  // These should be passed by pointer as they contain a lot of data.
 *  xQueue2 = xQueueCreate( 10, sizeof( struct AMessage * ) );
 *
 *  // ...
 *
 *  if( xQueue1 != 0 )
 *  {
 *      // Send an uint32_t.  Wait for 10 ticks for space to become
 *      // available if necessary.
 *      if( xQueueSend( xQueue1, ( void * ) &ulVar, ( TickType_t ) 10 ) != pdPASS )
 *      {
 *          // Failed to post the message, even after 10 ticks.
 *      }
 *  }
 *
 *  if( xQueue2 != 0 )
 *  {
 *      // Send a pointer to a struct AMessage object.  Don't block if the
 *      // queue is already full.
 *      pxMessage = & xMessage;
 *      xQueueSend( xQueue2, ( void * ) &pxMessage, ( TickType_t ) 0 );
 *  }
 *
 *  // ... Rest of task code.
 * }
 * @endcode
 * \ingroup QueueManagement
 */
#నిర్వచించు x_వరుస_Send( x_వరుస, pv_అంశం_గమ్యం_వరుస, x_Ticks_గమ్యం_Wait ) \
    x_వరుస_Generic_Send( ( x_వరుస ), ( pv_అంశం_గమ్యం_వరుస ), ( x_Ticks_గమ్యం_Wait ), వరుస_SEND_గమ్యం_వెనుక )

/**
 *
 * Only for use with queues that have a length of one - so the queue is either
 * empty or full.
 *
 * Post an item on a queue.  If the queue is already full then overwrite the
 * value held in the queue.  The item is queued by copy, not by reference.
 *
 * This function must not be called from an interrupt service routine.
 * See xQueueOverwriteFromISR () for an alternative which may be used in an ISR.
 *
 * @param xQueue The handle of the queue to which the data is being sent.
 *
 * @param pvItemToQueue A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvItemToQueue
 * into the queue storage area.
 *
 * @return xQueueOverwrite() is a macro that calls xQueueGenericSend(), and
 * therefore has the same return values as xQueueSendToFront().  However, pdPASS
 * is the only value that can be returned because xQueueOverwrite() will write
 * to the queue even when the queue is already full.
 *
 * Example usage:
 * @code{c}
 *
 * void vFunction( void *pvParameters )
 * {
 * QueueHandle_t xQueue;
 * uint32_t ulVarToSend, ulValReceived;
 *
 *  // Create a queue to hold one uint32_t value.  It is strongly
 *  // recommended *not* to use xQueueOverwrite() on queues that can
 *  // contain more than one value, and doing so will trigger an assertion
 *  // if configASSERT() is defined.
 *  xQueue = xQueueCreate( 1, sizeof( uint32_t ) );
 *
 *  // Write the value 10 to the queue using xQueueOverwrite().
 *  ulVarToSend = 10;
 *  xQueueOverwrite( xQueue, &ulVarToSend );
 *
 *  // Peeking the queue should now return 10, but leave the value 10 in
 *  // the queue.  A block time of zero is used as it is known that the
 *  // queue holds a value.
 *  ulValReceived = 0;
 *  xQueuePeek( xQueue, &ulValReceived, 0 );
 *
 *  if( ulValReceived != 10 )
 *  {
 *      // Error unless the item was removed by a different task.
 *  }
 *
 *  // The queue is still full.  Use xQueueOverwrite() to overwrite the
 *  // value held in the queue with 100.
 *  ulVarToSend = 100;
 *  xQueueOverwrite( xQueue, &ulVarToSend );
 *
 *  // This time read from the queue, leaving the queue empty once more.
 *  // A block time of 0 is used again.
 *  xQueueReceive( xQueue, &ulValReceived, 0 );
 *
 *  // The value read should be the last value written, even though the
 *  // queue was already full when the value was written.
 *  if( ulValReceived != 100 )
 *  {
 *      // Error!
 *  }
 *
 *  // ...
 * }
 * @endcode
 * \ingroup QueueManagement
 */
#నిర్వచించు x_వరుస_Overwrite( x_వరుస, pv_అంశం_గమ్యం_వరుస ) \
    x_వరుస_Generic_Send( ( x_వరుస ), ( pv_అంశం_గమ్యం_వరుస ), 0, వరుస_OVERWRITE )


/**
 *
 * It is preferred that the macros xQueueSend(), xQueueSendToFront() and
 * xQueueSendToBack() are used in place of calling this function directly.
 *
 * Post an item on a queue.  The item is queued by copy, not by reference.
 * This function must not be called from an interrupt service routine.
 * See xQueueSendFromISR () for an alternative which may be used in an ISR.
 *
 * @param xQueue The handle to the queue on which the item is to be posted.
 *
 * @param pvItemToQueue A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvItemToQueue
 * into the queue storage area.
 *
 * @param xTicksToWait The maximum amount of time the task should block
 * waiting for space to become available on the queue, should it already
 * be full.  The call will return immediately if this is set to 0 and the
 * queue is full.  The time is defined in tick periods so the constant
 * portTICK_PERIOD_MS should be used to convert to real time if this is required.
 *
 * @param xCopyPosition Can take the value queueSEND_TO_BACK to place the
 * item at the back of the queue, or queueSEND_TO_FRONT to place the item
 * at the front of the queue (for high priority messages).
 *
 * @return pdTRUE if the item was successfully posted, otherwise errQUEUE_FULL.
 *
 * Example usage:
 * @code{c}
 * struct AMessage
 * {
 *  char ucMessageID;
 *  char ucData[ 20 ];
 * } xMessage;
 *
 * uint32_t ulVar = 10UL;
 *
 * void vATask( void *pvParameters )
 * {
 * QueueHandle_t xQueue1, xQueue2;
 * struct AMessage *pxMessage;
 *
 *  // Create a queue capable of containing 10 uint32_t values.
 *  xQueue1 = xQueueCreate( 10, sizeof( uint32_t ) );
 *
 *  // Create a queue capable of containing 10 pointers to AMessage structures.
 *  // These should be passed by pointer as they contain a lot of data.
 *  xQueue2 = xQueueCreate( 10, sizeof( struct AMessage * ) );
 *
 *  // ...
 *
 *  if( xQueue1 != 0 )
 *  {
 *      // Send an uint32_t.  Wait for 10 ticks for space to become
 *      // available if necessary.
 *      if( xQueueGenericSend( xQueue1, ( void * ) &ulVar, ( TickType_t ) 10, queueSEND_TO_BACK ) != pdPASS )
 *      {
 *          // Failed to post the message, even after 10 ticks.
 *      }
 *  }
 *
 *  if( xQueue2 != 0 )
 *  {
 *      // Send a pointer to a struct AMessage object.  Don't block if the
 *      // queue is already full.
 *      pxMessage = & xMessage;
 *      xQueueGenericSend( xQueue2, ( void * ) &pxMessage, ( TickType_t ) 0, queueSEND_TO_BACK );
 *  }
 *
 *  // ... Rest of task code.
 * }
 * @endcode
 * \ingroup QueueManagement
 */
పునాది_Type_t x_వరుస_Generic_Send( వరుస_Handle_t x_వరుస,
                              స్థిరరాశి శూన్య * స్థిరరాశి pv_అంశం_గమ్యం_వరుస,
                              TickType_t x_Ticks_గమ్యం_Wait,
                              స్థిరరాశి పునాది_Type_t xCopyPosition ) PRIVILEGED_FUNCTION;

/**
 *
 * Receive an item from a queue without removing the item from the queue.
 * The item is received by copy so a buffer of adequate size must be
 * provided.  The number of bytes copied into the buffer was defined when
 * the queue was created.
 *
 * Successfully received items remain on the queue so will be returned again
 * by the next call, or a call to xQueueReceive().
 *
 * This macro must not be used in an interrupt service routine.  See
 * xQueuePeekFromISR() for an alternative that can be called from an interrupt
 * service routine.
 *
 * @param xQueue The handle to the queue from which the item is to be
 * received.
 *
 * @param pvBuffer Pointer to the buffer into which the received item will
 * be copied.
 *
 * @param xTicksToWait The maximum amount of time the task should block
 * waiting for an item to receive should the queue be empty at the time
 * of the call. The time is defined in tick periods so the constant
 * portTICK_PERIOD_MS should be used to convert to real time if this is required.
 * xQueuePeek() will return immediately if xTicksToWait is 0 and the queue
 * is empty.
 *
 * @return pdTRUE if an item was successfully received from the queue,
 * otherwise pdFALSE.
 *
 * Example usage:
 * @code{c}
 * struct AMessage
 * {
 *  char ucMessageID;
 *  char ucData[ 20 ];
 * } xMessage;
 *
 * QueueHandle_t xQueue;
 *
 * // Task to create a queue and post a value.
 * void vATask( void *pvParameters )
 * {
 * struct AMessage *pxMessage;
 *
 *  // Create a queue capable of containing 10 pointers to AMessage structures.
 *  // These should be passed by pointer as they contain a lot of data.
 *  xQueue = xQueueCreate( 10, sizeof( struct AMessage * ) );
 *  if( xQueue == 0 )
 *  {
 *      // Failed to create the queue.
 *  }
 *
 *  // ...
 *
 *  // Send a pointer to a struct AMessage object.  Don't block if the
 *  // queue is already full.
 *  pxMessage = & xMessage;
 *  xQueueSend( xQueue, ( void * ) &pxMessage, ( TickType_t ) 0 );
 *
 *  // ... Rest of task code.
 * }
 *
 * // Task to peek the data from the queue.
 * void vADifferentTask( void *pvParameters )
 * {
 * struct AMessage *pxRxedMessage;
 *
 *  if( xQueue != 0 )
 *  {
 *      // Peek a message on the created queue.  Block for 10 ticks if a
 *      // message is not immediately available.
 *      if( xQueuePeek( xQueue, &( pxRxedMessage ), ( TickType_t ) 10 ) )
 *      {
 *          // pcRxedMessage now points to the struct AMessage variable posted
 *          // by vATask, but the item still remains on the queue.
 *      }
 *  }
 *
 *  // ... Rest of task code.
 * }
 * @endcode
 * \ingroup QueueManagement
 */
పునాది_Type_t x_వరుస_Peek( వరుస_Handle_t x_వరుస,
                       శూన్య * స్థిరరాశి pv_బఫరు,
                       TickType_t x_Ticks_గమ్యం_Wait ) PRIVILEGED_FUNCTION;

/**
 *
 * A version of xQueuePeek() that can be called from an interrupt service
 * routine (ISR).
 *
 * Receive an item from a queue without removing the item from the queue.
 * The item is received by copy so a buffer of adequate size must be
 * provided.  The number of bytes copied into the buffer was defined when
 * the queue was created.
 *
 * Successfully received items remain on the queue so will be returned again
 * by the next call, or a call to xQueueReceive().
 *
 * @param xQueue The handle to the queue from which the item is to be
 * received.
 *
 * @param pvBuffer Pointer to the buffer into which the received item will
 * be copied.
 *
 * @return pdTRUE if an item was successfully received from the queue,
 * otherwise pdFALSE.
 *
 * \ingroup QueueManagement
 */
పునాది_Type_t x_వరుస_Peek_From_ISR( వరుస_Handle_t x_వరుస,
                              శూన్య * స్థిరరాశి pv_బఫరు ) PRIVILEGED_FUNCTION;

/**
 *
 * Receive an item from a queue.  The item is received by copy so a buffer of
 * adequate size must be provided.  The number of bytes copied into the buffer
 * was defined when the queue was created.
 *
 * Successfully received items are removed from the queue.
 *
 * This function must not be used in an interrupt service routine.  See
 * xQueueReceiveFromISR for an alternative that can.
 *
 * @param xQueue The handle to the queue from which the item is to be
 * received.
 *
 * @param pvBuffer Pointer to the buffer into which the received item will
 * be copied.
 *
 * @param xTicksToWait The maximum amount of time the task should block
 * waiting for an item to receive should the queue be empty at the time
 * of the call. xQueueReceive() will return immediately if xTicksToWait
 * is zero and the queue is empty.  The time is defined in tick periods so the
 * constant portTICK_PERIOD_MS should be used to convert to real time if this is
 * required.
 *
 * @return pdTRUE if an item was successfully received from the queue,
 * otherwise pdFALSE.
 *
 * Example usage:
 * @code{c}
 * struct AMessage
 * {
 *  char ucMessageID;
 *  char ucData[ 20 ];
 * } xMessage;
 *
 * QueueHandle_t xQueue;
 *
 * // Task to create a queue and post a value.
 * void vATask( void *pvParameters )
 * {
 * struct AMessage *pxMessage;
 *
 *  // Create a queue capable of containing 10 pointers to AMessage structures.
 *  // These should be passed by pointer as they contain a lot of data.
 *  xQueue = xQueueCreate( 10, sizeof( struct AMessage * ) );
 *  if( xQueue == 0 )
 *  {
 *      // Failed to create the queue.
 *  }
 *
 *  // ...
 *
 *  // Send a pointer to a struct AMessage object.  Don't block if the
 *  // queue is already full.
 *  pxMessage = & xMessage;
 *  xQueueSend( xQueue, ( void * ) &pxMessage, ( TickType_t ) 0 );
 *
 *  // ... Rest of task code.
 * }
 *
 * // Task to receive from the queue.
 * void vADifferentTask( void *pvParameters )
 * {
 * struct AMessage *pxRxedMessage;
 *
 *  if( xQueue != 0 )
 *  {
 *      // Receive a message on the created queue.  Block for 10 ticks if a
 *      // message is not immediately available.
 *      if( xQueueReceive( xQueue, &( pxRxedMessage ), ( TickType_t ) 10 ) )
 *      {
 *          // pcRxedMessage now points to the struct AMessage variable posted
 *          // by vATask.
 *      }
 *  }
 *
 *  // ... Rest of task code.
 * }
 * @endcode
 * \ingroup QueueManagement
 */
పునాది_Type_t x_వరుస_Receive( వరుస_Handle_t x_వరుస,
                          శూన్య * స్థిరరాశి pv_బఫరు,
                          TickType_t x_Ticks_గమ్యం_Wait ) PRIVILEGED_FUNCTION;

/**
 *
 * Return the number of messages stored in a queue.
 *
 * @param xQueue A handle to the queue being queried.
 *
 * @return The number of messages available in the queue.
 *
 * \ingroup QueueManagement
 */
UBaseType_t ux_వరుస_Messages_నిరీక్షణ( స్థిరరాశి వరుస_Handle_t x_వరుస ) PRIVILEGED_FUNCTION;

/**
 *
 * Return the number of free spaces available in a queue.  This is equal to the
 * number of items that can be sent to the queue before the queue becomes full
 * if no items are removed.
 *
 * @param xQueue A handle to the queue being queried.
 *
 * @return The number of spaces available in the queue.
 *
 * \ingroup QueueManagement
 */
UBaseType_t ux_వరుస_Spaces_అందుబాటు( స్థిరరాశి వరుస_Handle_t x_వరుస ) PRIVILEGED_FUNCTION;

/**
 *
 * Delete a queue - freeing all the memory allocated for storing of items
 * placed on the queue.
 *
 * @param xQueue A handle to the queue to be deleted.
 *
 * \ingroup QueueManagement
 */
శూన్య v_వరుస_Delete( వరుస_Handle_t x_వరుస ) PRIVILEGED_FUNCTION;

/**
 *
 * This is a macro that calls xQueueGenericSendFromISR().
 *
 * Post an item to the front of a queue.  It is safe to use this macro from
 * within an interrupt service routine.
 *
 * Items are queued by copy not reference so it is preferable to only
 * queue small items, especially when called from an ISR.  In most cases
 * it would be preferable to store a pointer to the item being queued.
 *
 * @param xQueue The handle to the queue on which the item is to be posted.
 *
 * @param pvItemToQueue A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvItemToQueue
 * into the queue storage area.
 *
 * @param pxHigherPriorityTaskWoken xQueueSendToFrontFromISR() will set
 * *pxHigherPriorityTaskWoken to pdTRUE if sending to the queue caused a task
 * to unblock, and the unblocked task has a priority higher than the currently
 * running task.  If xQueueSendToFromFromISR() sets this value to pdTRUE then
 * a context switch should be requested before the interrupt is exited.
 *
 * @return pdTRUE if the data was successfully sent to the queue, otherwise
 * errQUEUE_FULL.
 *
 * Example usage for buffered IO (where the ISR can obtain more than one value
 * per call):
 * @code{c}
 * void vBufferISR( void )
 * {
 * char cIn;
 * BaseType_t xHigherPriorityTaskWoken;
 *
 *  // We have not woken a task at the start of the ISR.
 *  xHigherPriorityTaskWoken = pdFALSE;
 *
 *  // Loop until the buffer is empty.
 *  do
 *  {
 *      // Obtain a byte from the buffer.
 *      cIn = portINPUT_BYTE( RX_REGISTER_ADDRESS );
 *
 *      // Post the byte.
 *      xQueueSendToFrontFromISR( xRxQueue, &cIn, &xHigherPriorityTaskWoken );
 *
 *  } while( portINPUT_BYTE( BUFFER_COUNT ) );
 *
 *  // Now the buffer is empty we can switch context if necessary.
 *  if( xHigherPriorityTaskWoken )
 *  {
 *      taskYIELD ();
 *  }
 * }
 * @endcode
 *
 * \ingroup QueueManagement
 */
#నిర్వచించు x_వరుస_Send_గమ్యం_Front_From_ISR( x_వరుస, pv_అంశం_గమ్యం_వరుస, px_Higher_ప్రాధాన్యత_Task_Woken ) \
    x_వరుస_Generic_Send_From_ISR( ( x_వరుస ), ( pv_అంశం_గమ్యం_వరుస ), ( px_Higher_ప్రాధాన్యత_Task_Woken ), వరుస_SEND_గమ్యం_FRONT )


/**
 *
 * This is a macro that calls xQueueGenericSendFromISR().
 *
 * Post an item to the back of a queue.  It is safe to use this macro from
 * within an interrupt service routine.
 *
 * Items are queued by copy not reference so it is preferable to only
 * queue small items, especially when called from an ISR.  In most cases
 * it would be preferable to store a pointer to the item being queued.
 *
 * @param xQueue The handle to the queue on which the item is to be posted.
 *
 * @param pvItemToQueue A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvItemToQueue
 * into the queue storage area.
 *
 * @param pxHigherPriorityTaskWoken xQueueSendToBackFromISR() will set
 * *pxHigherPriorityTaskWoken to pdTRUE if sending to the queue caused a task
 * to unblock, and the unblocked task has a priority higher than the currently
 * running task.  If xQueueSendToBackFromISR() sets this value to pdTRUE then
 * a context switch should be requested before the interrupt is exited.
 *
 * @return pdTRUE if the data was successfully sent to the queue, otherwise
 * errQUEUE_FULL.
 *
 * Example usage for buffered IO (where the ISR can obtain more than one value
 * per call):
 * @code{c}
 * void vBufferISR( void )
 * {
 * char cIn;
 * BaseType_t xHigherPriorityTaskWoken;
 *
 *  // We have not woken a task at the start of the ISR.
 *  xHigherPriorityTaskWoken = pdFALSE;
 *
 *  // Loop until the buffer is empty.
 *  do
 *  {
 *      // Obtain a byte from the buffer.
 *      cIn = portINPUT_BYTE( RX_REGISTER_ADDRESS );
 *
 *      // Post the byte.
 *      xQueueSendToBackFromISR( xRxQueue, &cIn, &xHigherPriorityTaskWoken );
 *
 *  } while( portINPUT_BYTE( BUFFER_COUNT ) );
 *
 *  // Now the buffer is empty we can switch context if necessary.
 *  if( xHigherPriorityTaskWoken )
 *  {
 *      taskYIELD ();
 *  }
 * }
 * @endcode
 *
 * \ingroup QueueManagement
 */
#నిర్వచించు x_వరుస_Send_గమ్యం_వెనుక_From_ISR( x_వరుస, pv_అంశం_గమ్యం_వరుస, px_Higher_ప్రాధాన్యత_Task_Woken ) \
    x_వరుస_Generic_Send_From_ISR( ( x_వరుస ), ( pv_అంశం_గమ్యం_వరుస ), ( px_Higher_ప్రాధాన్యత_Task_Woken ), వరుస_SEND_గమ్యం_వెనుక )

/**
 *
 * A version of xQueueOverwrite() that can be used in an interrupt service
 * routine (ISR).
 *
 * Only for use with queues that can hold a single item - so the queue is either
 * empty or full.
 *
 * Post an item on a queue.  If the queue is already full then overwrite the
 * value held in the queue.  The item is queued by copy, not by reference.
 *
 * @param xQueue The handle to the queue on which the item is to be posted.
 *
 * @param pvItemToQueue A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvItemToQueue
 * into the queue storage area.
 *
 * @param pxHigherPriorityTaskWoken xQueueOverwriteFromISR() will set
 * *pxHigherPriorityTaskWoken to pdTRUE if sending to the queue caused a task
 * to unblock, and the unblocked task has a priority higher than the currently
 * running task.  If xQueueOverwriteFromISR() sets this value to pdTRUE then
 * a context switch should be requested before the interrupt is exited.
 *
 * @return xQueueOverwriteFromISR() is a macro that calls
 * xQueueGenericSendFromISR(), and therefore has the same return values as
 * xQueueSendToFrontFromISR().  However, pdPASS is the only value that can be
 * returned because xQueueOverwriteFromISR() will write to the queue even when
 * the queue is already full.
 *
 * Example usage:
 * @code{c}
 *
 * QueueHandle_t xQueue;
 *
 * void vFunction( void *pvParameters )
 * {
 *  // Create a queue to hold one uint32_t value.  It is strongly
 *  // recommended *not* to use xQueueOverwriteFromISR() on queues that can
 *  // contain more than one value, and doing so will trigger an assertion
 *  // if configASSERT() is defined.
 *  xQueue = xQueueCreate( 1, sizeof( uint32_t ) );
 * }
 *
 * void vAnInterruptHandler( void )
 * {
 * // xHigherPriorityTaskWoken must be set to pdFALSE before it is used.
 * BaseType_t xHigherPriorityTaskWoken = pdFALSE;
 * uint32_t ulVarToSend, ulValReceived;
 *
 *  // Write the value 10 to the queue using xQueueOverwriteFromISR().
 *  ulVarToSend = 10;
 *  xQueueOverwriteFromISR( xQueue, &ulVarToSend, &xHigherPriorityTaskWoken );
 *
 *  // The queue is full, but calling xQueueOverwriteFromISR() again will still
 *  // pass because the value held in the queue will be overwritten with the
 *  // new value.
 *  ulVarToSend = 100;
 *  xQueueOverwriteFromISR( xQueue, &ulVarToSend, &xHigherPriorityTaskWoken );
 *
 *  // Reading from the queue will now return 100.
 *
 *  // ...
 *
 *  if( xHigherPrioritytaskWoken == pdTRUE )
 *  {
 *      // Writing to the queue caused a task to unblock and the unblocked task
 *      // has a priority higher than or equal to the priority of the currently
 *      // executing task (the task this interrupt interrupted).  Perform a context
 *      // switch so this interrupt returns directly to the unblocked task.
 *      portYIELD_FROM_ISR(); // or portEND_SWITCHING_ISR() depending on the port.
 *  }
 * }
 * @endcode
 * \ingroup QueueManagement
 */
#నిర్వచించు x_వరుస_Overwrite_From_ISR( x_వరుస, pv_అంశం_గమ్యం_వరుస, px_Higher_ప్రాధాన్యత_Task_Woken ) \
    x_వరుస_Generic_Send_From_ISR( ( x_వరుస ), ( pv_అంశం_గమ్యం_వరుస ), ( px_Higher_ప్రాధాన్యత_Task_Woken ), వరుస_OVERWRITE )

/**
 *
 * This is a macro that calls xQueueGenericSendFromISR().  It is included
 * for backward compatibility with versions of FreeRTOS.org that did not
 * include the xQueueSendToBackFromISR() and xQueueSendToFrontFromISR()
 * macros.
 *
 * Post an item to the back of a queue.  It is safe to use this function from
 * within an interrupt service routine.
 *
 * Items are queued by copy not reference so it is preferable to only
 * queue small items, especially when called from an ISR.  In most cases
 * it would be preferable to store a pointer to the item being queued.
 *
 * @param xQueue The handle to the queue on which the item is to be posted.
 *
 * @param pvItemToQueue A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvItemToQueue
 * into the queue storage area.
 *
 * @param pxHigherPriorityTaskWoken xQueueSendFromISR() will set
 * *pxHigherPriorityTaskWoken to pdTRUE if sending to the queue caused a task
 * to unblock, and the unblocked task has a priority higher than the currently
 * running task.  If xQueueSendFromISR() sets this value to pdTRUE then
 * a context switch should be requested before the interrupt is exited.
 *
 * @return pdTRUE if the data was successfully sent to the queue, otherwise
 * errQUEUE_FULL.
 *
 * Example usage for buffered IO (where the ISR can obtain more than one value
 * per call):
 * @code{c}
 * void vBufferISR( void )
 * {
 * char cIn;
 * BaseType_t xHigherPriorityTaskWoken;
 *
 *  // We have not woken a task at the start of the ISR.
 *  xHigherPriorityTaskWoken = pdFALSE;
 *
 *  // Loop until the buffer is empty.
 *  do
 *  {
 *      // Obtain a byte from the buffer.
 *      cIn = portINPUT_BYTE( RX_REGISTER_ADDRESS );
 *
 *      // Post the byte.
 *      xQueueSendFromISR( xRxQueue, &cIn, &xHigherPriorityTaskWoken );
 *
 *  } while( portINPUT_BYTE( BUFFER_COUNT ) );
 *
 *  // Now the buffer is empty we can switch context if necessary.
 *  if( xHigherPriorityTaskWoken )
 *  {
 *      // Actual macro used here is port specific.
 *      portYIELD_FROM_ISR ();
 *  }
 * }
 * @endcode
 *
 * \ingroup QueueManagement
 */
#నిర్వచించు x_వరుస_Send_From_ISR( x_వరుస, pv_అంశం_గమ్యం_వరుస, px_Higher_ప్రాధాన్యత_Task_Woken ) \
    x_వరుస_Generic_Send_From_ISR( ( x_వరుస ), ( pv_అంశం_గమ్యం_వరుస ), ( px_Higher_ప్రాధాన్యత_Task_Woken ), వరుస_SEND_గమ్యం_వెనుక )

/**
 *
 * It is preferred that the macros xQueueSendFromISR(),
 * xQueueSendToFrontFromISR() and xQueueSendToBackFromISR() be used in place
 * of calling this function directly.  xQueueGiveFromISR() is an
 * equivalent for use by semaphores that don't actually copy any data.
 *
 * Post an item on a queue.  It is safe to use this function from within an
 * interrupt service routine.
 *
 * Items are queued by copy not reference so it is preferable to only
 * queue small items, especially when called from an ISR.  In most cases
 * it would be preferable to store a pointer to the item being queued.
 *
 * @param xQueue The handle to the queue on which the item is to be posted.
 *
 * @param pvItemToQueue A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvItemToQueue
 * into the queue storage area.
 *
 * @param pxHigherPriorityTaskWoken xQueueGenericSendFromISR() will set
 * *pxHigherPriorityTaskWoken to pdTRUE if sending to the queue caused a task
 * to unblock, and the unblocked task has a priority higher than the currently
 * running task.  If xQueueGenericSendFromISR() sets this value to pdTRUE then
 * a context switch should be requested before the interrupt is exited.
 *
 * @param xCopyPosition Can take the value queueSEND_TO_BACK to place the
 * item at the back of the queue, or queueSEND_TO_FRONT to place the item
 * at the front of the queue (for high priority messages).
 *
 * @return pdTRUE if the data was successfully sent to the queue, otherwise
 * errQUEUE_FULL.
 *
 * Example usage for buffered IO (where the ISR can obtain more than one value
 * per call):
 * @code{c}
 * void vBufferISR( void )
 * {
 * char cIn;
 * BaseType_t xHigherPriorityTaskWokenByPost;
 *
 *  // We have not woken a task at the start of the ISR.
 *  xHigherPriorityTaskWokenByPost = pdFALSE;
 *
 *  // Loop until the buffer is empty.
 *  do
 *  {
 *      // Obtain a byte from the buffer.
 *      cIn = portINPUT_BYTE( RX_REGISTER_ADDRESS );
 *
 *      // Post each byte.
 *      xQueueGenericSendFromISR( xRxQueue, &cIn, &xHigherPriorityTaskWokenByPost, queueSEND_TO_BACK );
 *
 *  } while( portINPUT_BYTE( BUFFER_COUNT ) );
 *
 *  // Now the buffer is empty we can switch context if necessary.  Note that the
 *  // name of the yield function required is port specific.
 *  if( xHigherPriorityTaskWokenByPost )
 *  {
 *      portYIELD_FROM_ISR();
 *  }
 * }
 * @endcode
 *
 * \ingroup QueueManagement
 */
పునాది_Type_t x_వరుస_Generic_Send_From_ISR( వరుస_Handle_t x_వరుస,
                                     స్థిరరాశి శూన్య * స్థిరరాశి pv_అంశం_గమ్యం_వరుస,
                                     పునాది_Type_t * స్థిరరాశి px_Higher_ప్రాధాన్యత_Task_Woken,
                                     స్థిరరాశి పునాది_Type_t xCopyPosition ) PRIVILEGED_FUNCTION;
పునాది_Type_t x_వరుస_Give_From_ISR( వరుస_Handle_t x_వరుస,
                              పునాది_Type_t * స్థిరరాశి px_Higher_ప్రాధాన్యత_Task_Woken ) PRIVILEGED_FUNCTION;

/**
 *
 * Receive an item from a queue.  It is safe to use this function from within an
 * interrupt service routine.
 *
 * @param xQueue The handle to the queue from which the item is to be
 * received.
 *
 * @param pvBuffer Pointer to the buffer into which the received item will
 * be copied.
 *
 * @param pxHigherPriorityTaskWoken A task may be blocked waiting for space to
 * become available on the queue.  If xQueueReceiveFromISR causes such a task to
 * unblock *pxTaskWoken will get set to pdTRUE, otherwise *pxTaskWoken will
 * remain unchanged.
 *
 * @return pdTRUE if an item was successfully received from the queue,
 * otherwise pdFALSE.
 *
 * Example usage:
 * @code{c}
 *
 * QueueHandle_t xQueue;
 *
 * // Function to create a queue and post some values.
 * void vAFunction( void *pvParameters )
 * {
 * char cValueToPost;
 * const TickType_t xTicksToWait = ( TickType_t )0xff;
 *
 *  // Create a queue capable of containing 10 characters.
 *  xQueue = xQueueCreate( 10, sizeof( char ) );
 *  if( xQueue == 0 )
 *  {
 *      // Failed to create the queue.
 *  }
 *
 *  // ...
 *
 *  // Post some characters that will be used within an ISR.  If the queue
 *  // is full then this task will block for xTicksToWait ticks.
 *  cValueToPost = 'a';
 *  xQueueSend( xQueue, ( void * ) &cValueToPost, xTicksToWait );
 *  cValueToPost = 'b';
 *  xQueueSend( xQueue, ( void * ) &cValueToPost, xTicksToWait );
 *
 *  // ... keep posting characters ... this task may block when the queue
 *  // becomes full.
 *
 *  cValueToPost = 'c';
 *  xQueueSend( xQueue, ( void * ) &cValueToPost, xTicksToWait );
 * }
 *
 * // ISR that outputs all the characters received on the queue.
 * void vISR_Routine( void )
 * {
 * BaseType_t xTaskWokenByReceive = pdFALSE;
 * char cRxedChar;
 *
 *  while( xQueueReceiveFromISR( xQueue, ( void * ) &cRxedChar, &xTaskWokenByReceive) )
 *  {
 *      // A character was received.  Output the character now.
 *      vOutputCharacter( cRxedChar );
 *
 *      // If removing the character from the queue woke the task that was
 *      // posting onto the queue xTaskWokenByReceive will have been set to
 *      // pdTRUE.  No matter how many times this loop iterates only one
 *      // task will be woken.
 *  }
 *
 *  if( xTaskWokenByReceive != ( char ) pdFALSE;
 *  {
 *      taskYIELD ();
 *  }
 * }
 * @endcode
 * \ingroup QueueManagement
 */
పునాది_Type_t x_వరుస_Receive_From_ISR( వరుస_Handle_t x_వరుస,
                                 శూన్య * స్థిరరాశి pv_బఫరు,
                                 పునాది_Type_t * స్థిరరాశి px_Higher_ప్రాధాన్యత_Task_Woken ) PRIVILEGED_FUNCTION;

/**
 * Queries a queue to determine if the queue is empty. This function should only be used in an ISR.
 *
 * @param xQueue The handle of the queue being queried
 * @return pdFALSE if the queue is not empty, or pdTRUE if the queue is empty.
 */
పునాది_Type_t x_వరుస_Is_వరుస_ఖాళీలు_From_ISR( స్థిరరాశి వరుస_Handle_t x_వరుస ) PRIVILEGED_FUNCTION;

/**
 * Queries a queue to determine if the queue is full. This function should only be used in an ISR.
 *
 * @param xQueue The handle of the queue being queried
 * @return pdFALSE if the queue is not full, or pdTRUE if the queue is full.
 */
పునాది_Type_t x_వరుస_Is_వరుస_నిండిన_From_ISR( స్థిరరాశి వరుస_Handle_t x_వరుస ) PRIVILEGED_FUNCTION;

/**
 * A version of uxQueueMessagesWaiting() that can be called from an ISR. Return the number of messages stored in a queue.
 *
 * @param xQueue A handle to the queue being queried.
 * @return The number of messages available in the queue.
 */
UBaseType_t ux_వరుస_Messages_నిరీక్షణ_From_ISR( స్థిరరాశి వరుస_Handle_t x_వరుస ) PRIVILEGED_FUNCTION;

/** @cond !DOC_EXCLUDE_HEADER_SECTION */

/*
 * The functions defined above are for passing data to and from tasks.  The
 * functions below are the equivalents for passing data to and from
 * co-routines.
 *
 * These functions are called from the co-routine macro implementation and
 * should not be called directly from application code.  Instead use the macro
 * wrappers defined within croutine.h.
 */
పునాది_Type_t x_వరుస_CRSend_From_ISR( వరుస_Handle_t x_వరుస,
                                స్థిరరాశి శూన్య * pv_అంశం_గమ్యం_వరుస,
                                పునాది_Type_t xCoRoutinePreviouslyWoken );
పునాది_Type_t x_వరుస_CRReceive_From_ISR( వరుస_Handle_t x_వరుస,
                                   శూన్య * pv_బఫరు,
                                   పునాది_Type_t * pxTaskWoken );
పునాది_Type_t x_వరుస_CRSend( వరుస_Handle_t x_వరుస,
                         స్థిరరాశి శూన్య * pv_అంశం_గమ్యం_వరుస,
                         TickType_t x_Ticks_గమ్యం_Wait );
పునాది_Type_t x_వరుస_CRReceive( వరుస_Handle_t x_వరుస,
                            శూన్య * pv_బఫరు,
                            TickType_t x_Ticks_గమ్యం_Wait );

/*
 * For internal use only.  Use xSemaphoreCreateMutex(),
 * xSemaphoreCreateCounting() or xSemaphoreGetMutexHolder() instead of calling
 * these functions directly.
 */
వరుస_Handle_t x_వరుస_Create_Mutex( స్థిరరాశి uint8_t uc_వరుస_Type ) PRIVILEGED_FUNCTION;
వరుస_Handle_t x_వరుస_Create_Mutex_Static( స్థిరరాశి uint8_t uc_వరుస_Type,
                                       Static_వరుస_t * px_Static_వరుస ) PRIVILEGED_FUNCTION;
వరుస_Handle_t x_వరుస_Create_Counting_Semaphore( స్థిరరాశి UBaseType_t ux_Max_లెక్క,
                                             స్థిరరాశి UBaseType_t ux_Initial_లెక్క ) PRIVILEGED_FUNCTION;
వరుస_Handle_t x_వరుస_Create_Counting_Semaphore_Static( స్థిరరాశి UBaseType_t ux_Max_లెక్క,
                                                   స్థిరరాశి UBaseType_t ux_Initial_లెక్క,
                                                   Static_వరుస_t * px_Static_వరుస ) PRIVILEGED_FUNCTION;
పునాది_Type_t x_వరుస_Semaphore_Take( వరుస_Handle_t x_వరుస,
                                TickType_t x_Ticks_గమ్యం_Wait ) PRIVILEGED_FUNCTION;
TaskHandle_t x_వరుస_Get_Mutex_Holder( వరుస_Handle_t xSemaphore ) PRIVILEGED_FUNCTION;
TaskHandle_t x_వరుస_Get_Mutex_Holder_From_ISR( వరుస_Handle_t xSemaphore ) PRIVILEGED_FUNCTION;

/*
 * For internal use only.  Use xSemaphoreTakeMutexRecursive() or
 * xSemaphoreGiveMutexRecursive() instead of calling these functions directly.
 */
పునాది_Type_t x_వరుస_Take_Mutex_Recursive( వరుస_Handle_t xMutex,
                                     TickType_t x_Ticks_గమ్యం_Wait ) PRIVILEGED_FUNCTION;
పునాది_Type_t x_వరుస_Give_Mutex_Recursive( వరుస_Handle_t xMutex ) PRIVILEGED_FUNCTION;

/** @endcond */

/**
 * Reset a queue back to its original empty state.  The return value is now
 * obsolete and is always set to pdPASS.
 */
#నిర్వచించు x_వరుస_Reset( x_వరుస )    x_వరుస_Generic_Reset( ( x_వరుస ), pdFALSE )

/**
 * The registry is provided as a means for kernel aware debuggers to
 * locate queues, semaphores and mutexes.  Call vQueueAddToRegistry() add
 * a queue, semaphore or mutex handle to the registry if you want the handle
 * to be available to a kernel aware debugger.  If you are not using a kernel
 * aware debugger then this function can be ignored.
 *
 * configQUEUE_REGISTRY_SIZE defines the maximum number of handles the
 * registry can hold.  configQUEUE_REGISTRY_SIZE must be greater than 0
 * within FreeRTOSConfig.h for the registry to be available.  Its value
 * does not affect the number of queues, semaphores and mutexes that can be
 * created - just the number that the registry can hold.
 *
 * If vQueueAddToRegistry is called more than once with the same xQueue
 * parameter, the registry will store the pcQueueName parameter from the
 * most recent call to vQueueAddToRegistry.
 *
 * @param xQueue The handle of the queue being added to the registry.  This
 * is the handle returned by a call to xQueueCreate().  Semaphore and mutex
 * handles can also be passed in here.
 *
 * @param pcQueueName The name to be associated with the handle.  This is the
 * name that the kernel aware debugger will display.  The queue registry only
 * stores a pointer to the string - so the string must be persistent (global or
 * preferably in ROM/Flash), not on the stack.
 */
#ఒకవేళ ( config_వరుస_REGISTRY_కొలత > 0 )
    శూన్య v_వరుస_కూడు_గమ్యం_Registry( వరుస_Handle_t x_వరుస,
                              స్థిరరాశి అక్షరం * pc_వరుస_Name ) PRIVILEGED_FUNCTION; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
#అయితే_ముగింపు

/**
 * The registry is provided as a means for kernel aware debuggers to
 * locate queues, semaphores and mutexes.  Call vQueueAddToRegistry() add
 * a queue, semaphore or mutex handle to the registry if you want the handle
 * to be available to a kernel aware debugger, and vQueueUnregisterQueue() to
 * remove the queue, semaphore or mutex from the register.  If you are not using
 * a kernel aware debugger then this function can be ignored.
 *
 * @param xQueue The handle of the queue being removed from the registry.
 */
#ఒకవేళ ( config_వరుస_REGISTRY_కొలత > 0 )
    శూన్య v_వరుస_Unregister_వరుస( వరుస_Handle_t x_వరుస ) PRIVILEGED_FUNCTION;
#అయితే_ముగింపు

/**
 * The queue registry is provided as a means for kernel aware debuggers to
 * locate queues, semaphores and mutexes.  Call pcQueueGetName() to look
 * up and return the name of a queue in the queue registry from the queue's
 * handle.
 *
 * @param xQueue The handle of the queue the name of which will be returned.
 * @return If the queue is in the registry then a pointer to the name of the
 * queue is returned.  If the queue is not in the registry then NULL is
 * returned.
 */
#ఒకవేళ ( config_వరుస_REGISTRY_కొలత > 0 )
    స్థిరరాశి అక్షరం * pc_వరుస_Get_Name( వరుస_Handle_t x_వరుస ) PRIVILEGED_FUNCTION; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
#అయితే_ముగింపు

/** @cond !DOC_EXCLUDE_HEADER_SECTION */

/*
 * Generic version of the function used to create a queue using dynamic memory
 * allocation.  This is called by other functions and macros that create other
 * RTOS objects that use the queue structure as their base.
 */
#ఒకవేళ ( config_SUPPORT_DYNAMIC_కేటాయింపు == 1 )
    వరుస_Handle_t x_వరుస_Generic_Create( స్థిరరాశి UBaseType_t ux_వరుస_నిడివి,
                                       స్థిరరాశి UBaseType_t ux_అంశం_కొలత,
                                       స్థిరరాశి uint8_t uc_వరుస_Type ) PRIVILEGED_FUNCTION;
#అయితే_ముగింపు

/*
 * Generic version of the function used to create a queue using dynamic memory
 * allocation.  This is called by other functions and macros that create other
 * RTOS objects that use the queue structure as their base.
 */
#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )
    వరుస_Handle_t x_వరుస_Generic_Create_Static( స్థిరరాశి UBaseType_t ux_వరుస_నిడివి,
                                             స్థిరరాశి UBaseType_t ux_అంశం_కొలత,
                                             uint8_t * puc_వరుస_Storage,
                                             Static_వరుస_t * px_Static_వరుస,
                                             స్థిరరాశి uint8_t uc_వరుస_Type ) PRIVILEGED_FUNCTION;
#అయితే_ముగింపు

/*
 * Generic version of the function used to retrieve the buffers of statically
 * created queues. This is called by other functions and macros that retrieve
 * the buffers of other statically created RTOS objects that use the queue
 * structure as their base.
 */
#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )
    పునాది_Type_t x_వరుస_Generic_Get_Static_Buffers( వరుస_Handle_t x_వరుస,
                                              uint8_t ** ppuc_వరుస_Storage,
                                              Static_వరుస_t ** ppx_Static_వరుస ) PRIVILEGED_FUNCTION;
#అయితే_ముగింపు

/** @endcond */

/**
 * Queue sets provide a mechanism to allow a task to block (pend) on a read
 * operation from multiple queues or semaphores simultaneously.
 *
 * See FreeRTOS/Source/Demo/Common/Minimal/QueueSet.c for an example using this
 * function.
 *
 * A queue set must be explicitly created using a call to xQueueCreateSet()
 * before it can be used.  Once created, standard FreeRTOS queues and semaphores
 * can be added to the set using calls to xQueueAddToSet().
 * xQueueSelectFromSet() is then used to determine which, if any, of the queues
 * or semaphores contained in the set is in a state where a queue read or
 * semaphore take operation would be successful.
 *
 * Note 1:  See the documentation on https://www.freertos.org/Documentation/02-Kernel/04-API-references/07-Queue-sets/00-RTOS-queue-sets
 * for reasons why queue sets are very rarely needed in practice as there are
 * simpler methods of blocking on multiple objects.
 *
 * Note 2:  Blocking on a queue set that contains a mutex will not cause the
 * mutex holder to inherit the priority of the blocked task.
 *
 * Note 3:  An additional 4 bytes of RAM is required for each space in a every
 * queue added to a queue set.  Therefore counting semaphores that have a high
 * maximum count value should not be added to a queue set.
 *
 * Note 4:  A receive (in the case of a queue) or take (in the case of a
 * semaphore) operation must not be performed on a member of a queue set unless
 * a call to xQueueSelectFromSet() has first returned a handle to that set member.
 *
 * @param uxEventQueueLength Queue sets store events that occur on
 * the queues and semaphores contained in the set.  uxEventQueueLength specifies
 * the maximum number of events that can be queued at once.  To be absolutely
 * certain that events are not lost uxEventQueueLength should be set to the
 * total sum of the length of the queues added to the set, where binary
 * semaphores and mutexes have a length of 1, and counting semaphores have a
 * length set by their maximum count value.  Examples:
 *  + If a queue set is to hold a queue of length 5, another queue of length 12,
 *    and a binary semaphore, then uxEventQueueLength should be set to
 *    (5 + 12 + 1), or 18.
 *  + If a queue set is to hold three binary semaphores then uxEventQueueLength
 *    should be set to (1 + 1 + 1 ), or 3.
 *  + If a queue set is to hold a counting semaphore that has a maximum count of
 *    5, and a counting semaphore that has a maximum count of 3, then
 *    uxEventQueueLength should be set to (5 + 3), or 8.
 *
 * @return If the queue set is created successfully then a handle to the created
 * queue set is returned.  Otherwise NULL is returned.
 */
వరుస_సమితి_Handle_t x_వరుస_Create_సమితి( స్థిరరాశి UBaseType_t ux_Event_వరుస_నిడివి ) PRIVILEGED_FUNCTION;

/**
 * Adds a queue or semaphore to a queue set that was previously created by a
 * call to xQueueCreateSet().
 *
 * See FreeRTOS/Source/Demo/Common/Minimal/QueueSet.c for an example using this
 * function.
 *
 * Note 1:  A receive (in the case of a queue) or take (in the case of a
 * semaphore) operation must not be performed on a member of a queue set unless
 * a call to xQueueSelectFromSet() has first returned a handle to that set member.
 *
 * @param xQueueOrSemaphore The handle of the queue or semaphore being added to
 * the queue set (cast to an QueueSetMemberHandle_t type).
 *
 * @param xQueueSet The handle of the queue set to which the queue or semaphore
 * is being added.
 *
 * @return If the queue or semaphore was successfully added to the queue set
 * then pdPASS is returned.  If the queue could not be successfully added to the
 * queue set because it is already a member of a different queue set then pdFAIL
 * is returned.
 */
పునాది_Type_t x_వరుస_కూడు_గమ్యం_సమితి( వరుస_సమితి_Member_Handle_t x_వరుస_Or_Semaphore,
                           వరుస_సమితి_Handle_t x_వరుస_సమితి ) PRIVILEGED_FUNCTION;

/**
 * Removes a queue or semaphore from a queue set.  A queue or semaphore can only
 * be removed from a set if the queue or semaphore is empty.
 *
 * See FreeRTOS/Source/Demo/Common/Minimal/QueueSet.c for an example using this
 * function.
 *
 * @param xQueueOrSemaphore The handle of the queue or semaphore being removed
 * from the queue set (cast to an QueueSetMemberHandle_t type).
 *
 * @param xQueueSet The handle of the queue set in which the queue or semaphore
 * is included.
 *
 * @return If the queue or semaphore was successfully removed from the queue set
 * then pdPASS is returned.  If the queue was not in the queue set, or the
 * queue (or semaphore) was not empty, then pdFAIL is returned.
 */
పునాది_Type_t x_వరుస_Remove_From_సమితి( వరుస_సమితి_Member_Handle_t x_వరుస_Or_Semaphore,
                                వరుస_సమితి_Handle_t x_వరుస_సమితి ) PRIVILEGED_FUNCTION;

/**
 * xQueueSelectFromSet() selects from the members of a queue set a queue or
 * semaphore that either contains data (in the case of a queue) or is available
 * to take (in the case of a semaphore).  xQueueSelectFromSet() effectively
 * allows a task to block (pend) on a read operation on all the queues and
 * semaphores in a queue set simultaneously.
 *
 * See FreeRTOS/Source/Demo/Common/Minimal/QueueSet.c for an example using this
 * function.
 *
 * Note 1:  See the documentation on https://www.freertos.org/Documentation/02-Kernel/04-API-references/07-Queue-sets/00-RTOS-queue-sets
 * for reasons why queue sets are very rarely needed in practice as there are
 * simpler methods of blocking on multiple objects.
 *
 * Note 2:  Blocking on a queue set that contains a mutex will not cause the
 * mutex holder to inherit the priority of the blocked task.
 *
 * Note 3:  A receive (in the case of a queue) or take (in the case of a
 * semaphore) operation must not be performed on a member of a queue set unless
 * a call to xQueueSelectFromSet() has first returned a handle to that set member.
 *
 * @param xQueueSet The queue set on which the task will (potentially) block.
 *
 * @param xTicksToWait The maximum time, in ticks, that the calling task will
 * remain in the Blocked state (with other tasks executing) to wait for a member
 * of the queue set to be ready for a successful queue read or semaphore take
 * operation.
 *
 * @return xQueueSelectFromSet() will return the handle of a queue (cast to
 * a QueueSetMemberHandle_t type) contained in the queue set that contains data,
 * or the handle of a semaphore (cast to a QueueSetMemberHandle_t type) contained
 * in the queue set that is available, or NULL if no such queue or semaphore
 * exists before before the specified block time expires.
 */
వరుస_సమితి_Member_Handle_t x_వరుస_Select_From_సమితి( వరుస_సమితి_Handle_t x_వరుస_సమితి,
                                            స్థిరరాశి TickType_t x_Ticks_గమ్యం_Wait ) PRIVILEGED_FUNCTION;

/**
 * A version of xQueueSelectFromSet() that can be used from an ISR.
 */
వరుస_సమితి_Member_Handle_t x_వరుస_Select_From_సమితి_From_ISR( వరుస_సమితి_Handle_t x_వరుస_సమితి ) PRIVILEGED_FUNCTION;

/** @cond !DOC_EXCLUDE_HEADER_SECTION */

/* Not public API functions. */
శూన్య v_వరుస_Wait_For_సందేశం_Restricted( వరుస_Handle_t x_వరుస,
                                     TickType_t x_Ticks_గమ్యం_Wait,
                                     స్థిరరాశి పునాది_Type_t xWaitIndefinitely ) PRIVILEGED_FUNCTION;
పునాది_Type_t x_వరుస_Generic_Reset( వరుస_Handle_t x_వరుస,
                               పునాది_Type_t x_New_వరుస ) PRIVILEGED_FUNCTION;
శూన్య v_వరుస_సమితి_వరుస_సంఖ్య( వరుస_Handle_t x_వరుస,
                           UBaseType_t ux_వరుస_సంఖ్య ) PRIVILEGED_FUNCTION;
UBaseType_t ux_వరుస_Get_వరుస_సంఖ్య( వరుస_Handle_t x_వరుస ) PRIVILEGED_FUNCTION;
uint8_t uc_వరుస_Get_వరుస_Type( వరుస_Handle_t x_వరుస ) PRIVILEGED_FUNCTION;

/** @endcond */

/* *INDENT-OFF* */
#నిర్వచితమైతే __cplusplus
    }
#అయితే_ముగింపు
/* *INDENT-ON* */

#అయితే_ముగింపు /* QUEUE_H */
