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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ EVENT_GROUPS_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EVENT_GROUPS_H

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ INC_FREERTOS_H
    #ਗਲਤੀ "include FreeRTOS.h" must appear ਵਿੱਚ ਸਰੋਤ files before "include event_groups.h"
#ਜੇ_ਅੰਤ

/* FreeRTOS includes. */
#ਅਨ੍ਤਰ੍ਭਾਵ "timers.h"

/* *INDENT-OFF* */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
    ਬਾਹਰੀ "C" {
#ਜੇ_ਅੰਤ
/* *INDENT-ON* */

/**
 * An event group is a collection of bits to which an application can assign a
 * meaning.  For example, an application may create an event group to convey
 * the status of various CAN bus related events in which bit 0 might mean "A CAN
 * message has been received and is ready for processing", bit 1 might mean "The
 * application has queued a message that is ready for sending onto the CAN
 * network", and bit 2 might mean "It is time to send a SYNC message onto the
 * CAN network" etc.  A task can then test the bit values to see which events
 * are active, and optionally enter the Blocked state to wait for a specified
 * bit or a group of specified bits to be active.  To continue the CAN bus
 * example, a CAN controlling task can enter the Blocked state (and therefore
 * not consume any processing time) until either bit 0, bit 1 or bit 2 are
 * active, at which time the bit that was actually active would inform the task
 * which action it had to take (process a received message, send a message, or
 * send a SYNC).
 *
 * The event groups implementation contains intelligence to avoid race
 * conditions that would otherwise occur were an application to use a simple
 * variable for the same purpose.  This is particularly important with respect
 * to when a bit within an event group is to be cleared, and when bits have to
 * be set and then tested atomically - as is the case where event groups are
 * used to create a synchronisation point between multiple tasks (a
 * 'rendezvous').
 */



/**
 *
 * Type by which event groups are referenced.  For example, a call to
 * xEventGroupCreate() returns an EventGroupHandle_t variable that can then
 * be used as a parameter to other event group functions.
 *
 * \ingroup EventGroup
 */
ਬਣਤਰ Event_ਸਮੂਹ_Def_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ Event_ਸਮੂਹ_Def_t   * Event_ਸਮੂਹ_Handle_t;

/*
 * The type that holds event bits always matches TickType_t - therefore the
 * number of bits it holds is set by configUSE_16_BIT_TICKS (16 bits if set to 1,
 * 32 bits if set to 0.
 *
 * \ingroup EventGroup
 */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ TickType_t               Event_ਬਿੱਟ_t;

/**
 *
 * Create a new event group.
 *
 * Internally, within the FreeRTOS implementation, event groups use a [small]
 * block of memory, in which the event group's structure is stored.  If an event
 * groups is created using xEventGroupCreate() then the required memory is
 * automatically dynamically allocated inside the xEventGroupCreate() function.
 * (see https://www.FreeRTOS.org/a00111.html).  If an event group is created
 * using xEventGroupCreateStatic() then the application writer must instead
 * provide the memory that will get used by the event group.
 * xEventGroupCreateStatic() therefore allows an event group to be created
 * without using any dynamic memory allocation.
 *
 * Although event groups are not related to ticks, for internal implementation
 * reasons the number of bits available for use in an event group is dependent
 * on the configUSE_16_BIT_TICKS setting in FreeRTOSConfig.h.  If
 * configUSE_16_BIT_TICKS is 1 then each event group contains 8 usable bits (bit
 * 0 to bit 7).  If configUSE_16_BIT_TICKS is set to 0 then each event group has
 * 24 usable bits (bit 0 to bit 23).  The EventBits_t type is used to store
 * event bits within an event group.
 *
 * @return If the event group was created then a handle to the event group is
 * returned.  If there was insufficient FreeRTOS heap available to create the
 * event group then NULL is returned.  See https://www.FreeRTOS.org/a00111.html
 *
 * Example usage:
 * @code{c}
 *  // Declare a variable to hold the created event group.
 *  EventGroupHandle_t xCreatedEventGroup;
 *
 *  // Attempt to create the event group.
 *  xCreatedEventGroup = xEventGroupCreate();
 *
 *  // Was the event group created successfully?
 *  if( xCreatedEventGroup == NULL )
 *  {
 *      // The event group was not created because there was insufficient
 *      // FreeRTOS heap available.
 *  }
 *  else
 *  {
 *      // The event group was created.
 *  }
 * @endcode
 * \ingroup EventGroup
 */
#ਜੇ ( config_SUPPORT_DYNAMIC_ਵੰਡ == 1 )
    Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ_Create( ਖਾਲੀ ) PRIVILEGED_FUNCTION;
#ਜੇ_ਅੰਤ

/**
 *
 * Create a new event group.
 *
 * Internally, within the FreeRTOS implementation, event groups use a [small]
 * block of memory, in which the event group's structure is stored.  If an event
 * groups is created using xEventGroupCreate() then the required memory is
 * automatically dynamically allocated inside the xEventGroupCreate() function.
 * (see https://www.FreeRTOS.org/a00111.html).  If an event group is created
 * using xEventGroupCreateStatic() then the application writer must instead
 * provide the memory that will get used by the event group.
 * xEventGroupCreateStatic() therefore allows an event group to be created
 * without using any dynamic memory allocation.
 *
 * Although event groups are not related to ticks, for internal implementation
 * reasons the number of bits available for use in an event group is dependent
 * on the configUSE_16_BIT_TICKS setting in FreeRTOSConfig.h.  If
 * configUSE_16_BIT_TICKS is 1 then each event group contains 8 usable bits (bit
 * 0 to bit 7).  If configUSE_16_BIT_TICKS is set to 0 then each event group has
 * 24 usable bits (bit 0 to bit 23).  The EventBits_t type is used to store
 * event bits within an event group.
 *
 * @param pxEventGroupBuffer pxEventGroupBuffer must point to a variable of type
 * StaticEventGroup_t, which will be then be used to hold the event group's data
 * structures, removing the need for the memory to be allocated dynamically.
 *
 * @return If the event group was created then a handle to the event group is
 * returned.  If pxEventGroupBuffer was NULL then NULL is returned.
 *
 * Example usage:
 * @code{c}
 *  // StaticEventGroup_t is a publicly accessible structure that has the same
 *  // size and alignment requirements as the real event group structure.  It is
 *  // provided as a mechanism for applications to know the size of the event
 *  // group (which is dependent on the architecture and configuration file
 *  // settings) without breaking the strict data hiding policy by exposing the
 *  // real event group internals.  This StaticEventGroup_t variable is passed
 *  // into the xSemaphoreCreateEventGroupStatic() function and is used to store
 *  // the event group's data structures
 *  StaticEventGroup_t xEventGroupBuffer;
 *
 *  // Create the event group without dynamically allocating any memory.
 *  xEventGroup = xEventGroupCreateStatic( &xEventGroupBuffer );
 * @endcode
 */
#ਜੇ ( config_SUPPORT_STATIC_ਵੰਡ == 1 )
    Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ_Create_Static( Static_Event_ਸਮੂਹ_t * px_Event_ਸਮੂਹ_ਬਫਰ ) PRIVILEGED_FUNCTION;
#ਜੇ_ਅੰਤ

/**
 *
 * [Potentially] block to wait for one or more bits to be set within a
 * previously created event group.
 *
 * This function cannot be called from an interrupt.
 *
 * @param xEventGroup The event group in which the bits are being tested.  The
 * event group must have previously been created using a call to
 * xEventGroupCreate().
 *
 * @param uxBitsToWaitFor A bitwise value that indicates the bit or bits to test
 * inside the event group.  For example, to wait for bit 0 and/or bit 2 set
 * uxBitsToWaitFor to 0x05.  To wait for bits 0 and/or bit 1 and/or bit 2 set
 * uxBitsToWaitFor to 0x07.  Etc.
 *
 * @param xClearOnExit If xClearOnExit is set to pdTRUE then any bits within
 * uxBitsToWaitFor that are set within the event group will be cleared before
 * xEventGroupWaitBits() returns if the wait condition was met (if the function
 * returns for a reason other than a timeout).  If xClearOnExit is set to
 * pdFALSE then the bits set in the event group are not altered when the call to
 * xEventGroupWaitBits() returns.
 *
 * @param xWaitForAllBits If xWaitForAllBits is set to pdTRUE then
 * xEventGroupWaitBits() will return when either all the bits in uxBitsToWaitFor
 * are set or the specified block time expires.  If xWaitForAllBits is set to
 * pdFALSE then xEventGroupWaitBits() will return when any one of the bits set
 * in uxBitsToWaitFor is set or the specified block time expires.  The block
 * time is specified by the xTicksToWait parameter.
 *
 * @param xTicksToWait The maximum amount of time (specified in 'ticks') to wait
 * for one/all (depending on the xWaitForAllBits value) of the bits specified by
 * uxBitsToWaitFor to become set. A value of portMAX_DELAY can be used to block
 * indefinitely (provided INCLUDE_vTaskSuspend is set to 1 in FreeRTOSConfig.h).
 *
 * @return The value of the event group at the time either the bits being waited
 * for became set, or the block time expired.  Test the return value to know
 * which bits were set.  If xEventGroupWaitBits() returned because its timeout
 * expired then not all the bits being waited for will be set.  If
 * xEventGroupWaitBits() returned because the bits it was waiting for were set
 * then the returned value is the event group value before any bits were
 * automatically cleared in the case that xClearOnExit parameter was set to
 * pdTRUE.
 *
 * Example usage:
 * @code{c}
 * #define BIT_0 ( 1 << 0 )
 * #define BIT_4 ( 1 << 4 )
 *
 * void aFunction( EventGroupHandle_t xEventGroup )
 * {
 * EventBits_t uxBits;
 * const TickType_t xTicksToWait = 100 / portTICK_PERIOD_MS;
 *
 *      // Wait a maximum of 100ms for either bit 0 or bit 4 to be set within
 *      // the event group.  Clear the bits before exiting.
 *      uxBits = xEventGroupWaitBits(
 *                  xEventGroup,    // The event group being tested.
 *                  BIT_0 | BIT_4,  // The bits within the event group to wait for.
 *                  pdTRUE,         // BIT_0 and BIT_4 should be cleared before returning.
 *                  pdFALSE,        // Don't wait for both bits, either bit will do.
 *                  xTicksToWait ); // Wait a maximum of 100ms for either bit to be set.
 *
 *      if( ( uxBits & ( BIT_0 | BIT_4 ) ) == ( BIT_0 | BIT_4 ) )
 *      {
 *          // xEventGroupWaitBits() returned because both bits were set.
 *      }
 *      else if( ( uxBits & BIT_0 ) != 0 )
 *      {
 *          // xEventGroupWaitBits() returned because just BIT_0 was set.
 *      }
 *      else if( ( uxBits & BIT_4 ) != 0 )
 *      {
 *          // xEventGroupWaitBits() returned because just BIT_4 was set.
 *      }
 *      else
 *      {
 *          // xEventGroupWaitBits() returned because xTicksToWait ticks passed
 *          // without either BIT_0 or BIT_4 becoming set.
 *      }
 * }
 * @endcode
 * \ingroup EventGroup
 */
Event_ਬਿੱਟ_t x_Event_ਸਮੂਹ_Wait_ਬਿੱਟ( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ,
                                 ਅਚਲ Event_ਬਿੱਟ_t ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For,
                                 ਅਚਲ ਅਧਾਰ_Type_t xClearOnExit,
                                 ਅਚਲ ਅਧਾਰ_Type_t x_Wait_For_All_ਬਿੱਟ,
                                 TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait ) PRIVILEGED_FUNCTION;

/**
 *
 * Clear bits within an event group.  This function cannot be called from an
 * interrupt.
 *
 * @param xEventGroup The event group in which the bits are to be cleared.
 *
 * @param uxBitsToClear A bitwise value that indicates the bit or bits to clear
 * in the event group.  For example, to clear bit 3 only, set uxBitsToClear to
 * 0x08.  To clear bit 3 and bit 0 set uxBitsToClear to 0x09.
 *
 * @return The value of the event group before the specified bits were cleared.
 *
 * Example usage:
 * @code{c}
 * #define BIT_0 ( 1 << 0 )
 * #define BIT_4 ( 1 << 4 )
 *
 * void aFunction( EventGroupHandle_t xEventGroup )
 * {
 * EventBits_t uxBits;
 *
 *      // Clear bit 0 and bit 4 in xEventGroup.
 *      uxBits = xEventGroupClearBits(
 *                              xEventGroup,    // The event group being updated.
 *                              BIT_0 | BIT_4 );// The bits being cleared.
 *
 *      if( ( uxBits & ( BIT_0 | BIT_4 ) ) == ( BIT_0 | BIT_4 ) )
 *      {
 *          // Both bit 0 and bit 4 were set before xEventGroupClearBits() was
 *          // called.  Both will now be clear (not set).
 *      }
 *      else if( ( uxBits & BIT_0 ) != 0 )
 *      {
 *          // Bit 0 was set before xEventGroupClearBits() was called.  It will
 *          // now be clear.
 *      }
 *      else if( ( uxBits & BIT_4 ) != 0 )
 *      {
 *          // Bit 4 was set before xEventGroupClearBits() was called.  It will
 *          // now be clear.
 *      }
 *      else
 *      {
 *          // Neither bit 0 nor bit 4 were set in the first place.
 *      }
 * }
 * @endcode
 * \ingroup EventGroup
 */
Event_ਬਿੱਟ_t x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ,
                                  ਅਚਲ Event_ਬਿੱਟ_t ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear ) PRIVILEGED_FUNCTION;

/**
 *
 * A version of xEventGroupClearBits() that can be called from an interrupt.
 *
 * Setting bits in an event group is not a deterministic operation because there
 * are an unknown number of tasks that may be waiting for the bit or bits being
 * set.  FreeRTOS does not allow nondeterministic operations to be performed
 * while interrupts are disabled, so protects event groups that are accessed
 * from tasks by suspending the scheduler rather than disabling interrupts.  As
 * a result event groups cannot be accessed directly from an interrupt service
 * routine.  Therefore xEventGroupClearBitsFromISR() sends a message to the
 * timer task to have the clear operation performed in the context of the timer
 * task.
 *
 * @note If this function returns pdPASS then the timer task is ready to run
 * and a portYIELD_FROM_ISR(pdTRUE) should be executed to perform the needed
 * clear on the event group.  This behavior is different from
 * xEventGroupSetBitsFromISR because the parameter xHigherPriorityTaskWoken is
 * not present.
 *
 * @param xEventGroup The event group in which the bits are to be cleared.
 *
 * @param uxBitsToClear A bitwise value that indicates the bit or bits to clear.
 * For example, to clear bit 3 only, set uxBitsToClear to 0x08.  To clear bit 3
 * and bit 0 set uxBitsToClear to 0x09.
 *
 * @return If the request to execute the function was posted successfully then
 * pdPASS is returned, otherwise pdFALSE is returned.  pdFALSE will be returned
 * if the timer service queue was full.
 *
 * Example usage:
 * @code{c}
 * #define BIT_0 ( 1 << 0 )
 * #define BIT_4 ( 1 << 4 )
 *
 * // An event group which it is assumed has already been created by a call to
 * // xEventGroupCreate().
 * EventGroupHandle_t xEventGroup;
 *
 * void anInterruptHandler( void )
 * {
 *      // Clear bit 0 and bit 4 in xEventGroup.
 *      xResult = xEventGroupClearBitsFromISR(
 *                          xEventGroup,     // The event group being updated.
 *                          BIT_0 | BIT_4 ); // The bits being set.
 *
 *      if( xResult == pdPASS )
 *      {
 *          // The message was posted successfully.
 *          portYIELD_FROM_ISR(pdTRUE);
 *      }
 * }
 * @endcode
 * \ingroup EventGroup
 */
#ਜੇ ( configUSE_TRACE_FACILITY == 1 )
    ਅਧਾਰ_Type_t x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ_From_ISR( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ,
                                            ਅਚਲ Event_ਬਿੱਟ_t ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear ) PRIVILEGED_FUNCTION;
#ਨਹੀਂਤਾਂ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ_From_ISR( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear ) \
    xTimerPendFunctionCallFromISR( v_Event_ਸਮੂਹ_Clear_ਬਿੱਟ_Callback, ( ਖਾਲੀ * ) ( x_Event_ਸਮੂਹ ), ( uint32_t ) ( ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear ), ਨਿਰਾ )
#ਜੇ_ਅੰਤ

/**
 *
 * Set bits within an event group.
 * This function cannot be called from an interrupt.  xEventGroupSetBitsFromISR()
 * is a version that can be called from an interrupt.
 *
 * Setting bits in an event group will automatically unblock tasks that are
 * blocked waiting for the bits.
 *
 * @param xEventGroup The event group in which the bits are to be set.
 *
 * @param uxBitsToSet A bitwise value that indicates the bit or bits to set.
 * For example, to set bit 3 only, set uxBitsToSet to 0x08.  To set bit 3
 * and bit 0 set uxBitsToSet to 0x09.
 *
 * @return The value of the event group at the time the call to
 * xEventGroupSetBits() returns.  There are two reasons why the returned value
 * might have the bits specified by the uxBitsToSet parameter cleared.  First,
 * if setting a bit results in a task that was waiting for the bit leaving the
 * blocked state then it is possible the bit will be cleared automatically
 * (see the xClearBitOnExit parameter of xEventGroupWaitBits()).  Second, any
 * unblocked (or otherwise Ready state) task that has a priority above that of
 * the task that called xEventGroupSetBits() will execute and may change the
 * event group value before the call to xEventGroupSetBits() returns.
 *
 * Example usage:
 * @code{c}
 * #define BIT_0 ( 1 << 0 )
 * #define BIT_4 ( 1 << 4 )
 *
 * void aFunction( EventGroupHandle_t xEventGroup )
 * {
 * EventBits_t uxBits;
 *
 *      // Set bit 0 and bit 4 in xEventGroup.
 *      uxBits = xEventGroupSetBits(
 *                          xEventGroup,    // The event group being updated.
 *                          BIT_0 | BIT_4 );// The bits being set.
 *
 *      if( ( uxBits & ( BIT_0 | BIT_4 ) ) == ( BIT_0 | BIT_4 ) )
 *      {
 *          // Both bit 0 and bit 4 remained set when the function returned.
 *      }
 *      else if( ( uxBits & BIT_0 ) != 0 )
 *      {
 *          // Bit 0 remained set when the function returned, but bit 4 was
 *          // cleared.  It might be that bit 4 was cleared automatically as a
 *          // task that was waiting for bit 4 was removed from the Blocked
 *          // state.
 *      }
 *      else if( ( uxBits & BIT_4 ) != 0 )
 *      {
 *          // Bit 4 remained set when the function returned, but bit 0 was
 *          // cleared.  It might be that bit 0 was cleared automatically as a
 *          // task that was waiting for bit 0 was removed from the Blocked
 *          // state.
 *      }
 *      else
 *      {
 *          // Neither bit 0 nor bit 4 remained set.  It might be that a task
 *          // was waiting for both of the bits to be set, and the bits were
 *          // cleared as the task left the Blocked state.
 *      }
 * }
 * @endcode
 * \ingroup EventGroup
 */
Event_ਬਿੱਟ_t x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ,
                                ਅਚਲ Event_ਬਿੱਟ_t ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ ) PRIVILEGED_FUNCTION;

/**
 *
 * A version of xEventGroupSetBits() that can be called from an interrupt.
 *
 * Setting bits in an event group is not a deterministic operation because there
 * are an unknown number of tasks that may be waiting for the bit or bits being
 * set.  FreeRTOS does not allow nondeterministic operations to be performed in
 * interrupts or from critical sections.  Therefore xEventGroupSetBitsFromISR()
 * sends a message to the timer task to have the set operation performed in the
 * context of the timer task - where a scheduler lock is used in place of a
 * critical section.
 *
 * @param xEventGroup The event group in which the bits are to be set.
 *
 * @param uxBitsToSet A bitwise value that indicates the bit or bits to set.
 * For example, to set bit 3 only, set uxBitsToSet to 0x08.  To set bit 3
 * and bit 0 set uxBitsToSet to 0x09.
 *
 * @param pxHigherPriorityTaskWoken As mentioned above, calling this function
 * will result in a message being sent to the timer daemon task.  If the
 * priority of the timer daemon task is higher than the priority of the
 * currently running task (the task the interrupt interrupted) then
 * *pxHigherPriorityTaskWoken will be set to pdTRUE by
 * xEventGroupSetBitsFromISR(), indicating that a context switch should be
 * requested before the interrupt exits.  For that reason
 * *pxHigherPriorityTaskWoken must be initialised to pdFALSE.  See the
 * example code below.
 *
 * @return If the request to execute the function was posted successfully then
 * pdPASS is returned, otherwise pdFALSE is returned.  pdFALSE will be returned
 * if the timer service queue was full.
 *
 * Example usage:
 * @code{c}
 * #define BIT_0 ( 1 << 0 )
 * #define BIT_4 ( 1 << 4 )
 *
 * // An event group which it is assumed has already been created by a call to
 * // xEventGroupCreate().
 * EventGroupHandle_t xEventGroup;
 *
 * void anInterruptHandler( void )
 * {
 * BaseType_t xHigherPriorityTaskWoken, xResult;
 *
 *      // xHigherPriorityTaskWoken must be initialised to pdFALSE.
 *      xHigherPriorityTaskWoken = pdFALSE;
 *
 *      // Set bit 0 and bit 4 in xEventGroup.
 *      xResult = xEventGroupSetBitsFromISR(
 *                          xEventGroup,    // The event group being updated.
 *                          BIT_0 | BIT_4   // The bits being set.
 *                          &xHigherPriorityTaskWoken );
 *
 *      // Was the message posted successfully?
 *      if( xResult == pdPASS )
 *      {
 *          // If xHigherPriorityTaskWoken is now set to pdTRUE then a context
 *          // switch should be requested.  The macro used is port specific and
 *          // will be either portYIELD_FROM_ISR() or portEND_SWITCHING_ISR() -
 *          // refer to the documentation page for the port being used.
 *          portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
 *      }
 * }
 * @endcode
 * \ingroup EventGroup
 */
#ਜੇ ( configUSE_TRACE_FACILITY == 1 )
    ਅਧਾਰ_Type_t x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ_From_ISR( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ,
                                          ਅਚਲ Event_ਬਿੱਟ_t ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ,
                                          ਅਧਾਰ_Type_t * px_Higher_ਤਰਜੀਹ_Task_Woken ) PRIVILEGED_FUNCTION;
#ਨਹੀਂਤਾਂ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ_From_ISR( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ, px_Higher_ਤਰਜੀਹ_Task_Woken ) \
    xTimerPendFunctionCallFromISR( v_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ_Callback, ( ਖਾਲੀ * ) ( x_Event_ਸਮੂਹ ), ( uint32_t ) ( ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ ), ( px_Higher_ਤਰਜੀਹ_Task_Woken ) )
#ਜੇ_ਅੰਤ

/**
 *
 * Atomically set bits within an event group, then wait for a combination of
 * bits to be set within the same event group.  This functionality is typically
 * used to synchronise multiple tasks, where each task has to wait for the other
 * tasks to reach a synchronisation point before proceeding.
 *
 * This function cannot be used from an interrupt.
 *
 * The function will return before its block time expires if the bits specified
 * by the uxBitsToWait parameter are set, or become set within that time.  In
 * this case all the bits specified by uxBitsToWait will be automatically
 * cleared before the function returns.
 *
 * @param xEventGroup The event group in which the bits are being tested.  The
 * event group must have previously been created using a call to
 * xEventGroupCreate().
 *
 * @param uxBitsToSet The bits to set in the event group before determining
 * if, and possibly waiting for, all the bits specified by the uxBitsToWait
 * parameter are set.
 *
 * @param uxBitsToWaitFor A bitwise value that indicates the bit or bits to test
 * inside the event group.  For example, to wait for bit 0 and bit 2 set
 * uxBitsToWaitFor to 0x05.  To wait for bits 0 and bit 1 and bit 2 set
 * uxBitsToWaitFor to 0x07.  Etc.
 *
 * @param xTicksToWait The maximum amount of time (specified in 'ticks') to wait
 * for all of the bits specified by uxBitsToWaitFor to become set.
 *
 * @return The value of the event group at the time either the bits being waited
 * for became set, or the block time expired.  Test the return value to know
 * which bits were set.  If xEventGroupSync() returned because its timeout
 * expired then not all the bits being waited for will be set.  If
 * xEventGroupSync() returned because all the bits it was waiting for were
 * set then the returned value is the event group value before any bits were
 * automatically cleared.
 *
 * Example usage:
 * @code{c}
 * // Bits used by the three tasks.
 * #define TASK_0_BIT     ( 1 << 0 )
 * #define TASK_1_BIT     ( 1 << 1 )
 * #define TASK_2_BIT     ( 1 << 2 )
 *
 * #define ALL_SYNC_BITS ( TASK_0_BIT | TASK_1_BIT | TASK_2_BIT )
 *
 * // Use an event group to synchronise three tasks.  It is assumed this event
 * // group has already been created elsewhere.
 * EventGroupHandle_t xEventBits;
 *
 * void vTask0( void *pvParameters )
 * {
 * EventBits_t uxReturn;
 * TickType_t xTicksToWait = 100 / portTICK_PERIOD_MS;
 *
 *   for( ;; )
 *   {
 *      // Perform task functionality here.
 *
 *      // Set bit 0 in the event flag to note this task has reached the
 *      // sync point.  The other two tasks will set the other two bits defined
 *      // by ALL_SYNC_BITS.  All three tasks have reached the synchronisation
 *      // point when all the ALL_SYNC_BITS are set.  Wait a maximum of 100ms
 *      // for this to happen.
 *      uxReturn = xEventGroupSync( xEventBits, TASK_0_BIT, ALL_SYNC_BITS, xTicksToWait );
 *
 *      if( ( uxReturn & ALL_SYNC_BITS ) == ALL_SYNC_BITS )
 *      {
 *          // All three tasks reached the synchronisation point before the call
 *          // to xEventGroupSync() timed out.
 *      }
 *  }
 * }
 *
 * void vTask1( void *pvParameters )
 * {
 *   for( ;; )
 *   {
 *      // Perform task functionality here.
 *
 *      // Set bit 1 in the event flag to note this task has reached the
 *      // synchronisation point.  The other two tasks will set the other two
 *      // bits defined by ALL_SYNC_BITS.  All three tasks have reached the
 *      // synchronisation point when all the ALL_SYNC_BITS are set.  Wait
 *      // indefinitely for this to happen.
 *      xEventGroupSync( xEventBits, TASK_1_BIT, ALL_SYNC_BITS, portMAX_DELAY );
 *
 *      // xEventGroupSync() was called with an indefinite block time, so
 *      // this task will only reach here if the synchronisation was made by all
 *      // three tasks, so there is no need to test the return value.
 *   }
 * }
 *
 * void vTask2( void *pvParameters )
 * {
 *   for( ;; )
 *   {
 *      // Perform task functionality here.
 *
 *      // Set bit 2 in the event flag to note this task has reached the
 *      // synchronisation point.  The other two tasks will set the other two
 *      // bits defined by ALL_SYNC_BITS.  All three tasks have reached the
 *      // synchronisation point when all the ALL_SYNC_BITS are set.  Wait
 *      // indefinitely for this to happen.
 *      xEventGroupSync( xEventBits, TASK_2_BIT, ALL_SYNC_BITS, portMAX_DELAY );
 *
 *      // xEventGroupSync() was called with an indefinite block time, so
 *      // this task will only reach here if the synchronisation was made by all
 *      // three tasks, so there is no need to test the return value.
 *  }
 * }
 *
 * @endcode
 * \ingroup EventGroup
 */
Event_ਬਿੱਟ_t x_Event_ਸਮੂਹ_Sync( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ,
                             ਅਚਲ Event_ਬਿੱਟ_t ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ,
                             ਅਚਲ Event_ਬਿੱਟ_t ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For,
                             TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait ) PRIVILEGED_FUNCTION;


/**
 *
 * Returns the current value of the bits in an event group.  This function
 * cannot be used from an interrupt.
 *
 * @param xEventGroup The event group being queried.
 *
 * @return The event group bits at the time xEventGroupGetBits() was called.
 *
 * \ingroup EventGroup
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Event_ਸਮੂਹ_Get_ਬਿੱਟ( x_Event_ਸਮੂਹ )    x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ( ( x_Event_ਸਮੂਹ ), 0 )

/**
 *
 * A version of xEventGroupGetBits() that can be called from an ISR.
 *
 * @param xEventGroup The event group being queried.
 *
 * @return The event group bits at the time xEventGroupGetBitsFromISR() was called.
 *
 * \ingroup EventGroup
 */
Event_ਬਿੱਟ_t x_Event_ਸਮੂਹ_Get_ਬਿੱਟ_From_ISR( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ ) PRIVILEGED_FUNCTION;

/**
 *
 * Delete an event group that was previously created by a call to
 * xEventGroupCreate().  Tasks that are blocked on the event group will be
 * unblocked and obtain 0 as the event group's value.
 *
 * @param xEventGroup The event group being deleted.
 */
ਖਾਲੀ v_Event_ਸਮੂਹ_Delete( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ ) PRIVILEGED_FUNCTION;

/**
 *
 * Retrieve a pointer to a statically created event groups's data structure
 * buffer. It is the same buffer that is supplied at the time of creation.
 *
 * @param xEventGroup The event group for which to retrieve the buffer.
 *
 * @param ppxEventGroupBuffer Used to return a pointer to the event groups's
 * data structure buffer.
 *
 * @return pdTRUE if the buffer was retrieved, pdFALSE otherwise.
 */
#ਜੇ ( config_SUPPORT_STATIC_ਵੰਡ == 1 )
    ਅਧਾਰ_Type_t x_Event_ਸਮੂਹ_Get_Static_ਬਫਰ( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ,
                                           Static_Event_ਸਮੂਹ_t ** ppx_Event_ਸਮੂਹ_ਬਫਰ ) PRIVILEGED_FUNCTION;
#ਜੇ_ਅੰਤ /* configSUPPORT_STATIC_ALLOCATION */

/** @cond !DOC_EXCLUDE_HEADER_SECTION */

/* For internal use only. */
ਖਾਲੀ v_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ_Callback( ਖਾਲੀ * pv_Event_ਸਮੂਹ,
                                 ਅਚਲ uint32_t ul_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ ) PRIVILEGED_FUNCTION;
ਖਾਲੀ v_Event_ਸਮੂਹ_Clear_ਬਿੱਟ_Callback( ਖਾਲੀ * pv_Event_ਸਮੂਹ,
                                   ਅਚਲ uint32_t ul_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear ) PRIVILEGED_FUNCTION;


#ਜੇ ( configUSE_TRACE_FACILITY == 1 )
    UBaseType_t ux_Event_ਸਮੂਹ_Get_ਸੰਖਿਆ( ਖਾਲੀ * x_Event_ਸਮੂਹ ) PRIVILEGED_FUNCTION;
    ਖਾਲੀ v_Event_ਸਮੂਹ_ਸੈੱਟ_ਸੰਖਿਆ( ਖਾਲੀ * x_Event_ਸਮੂਹ,
                               UBaseType_t ux_Event_ਸਮੂਹ_ਸੰਖਿਆ ) PRIVILEGED_FUNCTION;
#ਜੇ_ਅੰਤ

/** @endcond */

/* *INDENT-OFF* */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
    }
#ਜੇ_ਅੰਤ
/* *INDENT-ON* */

#ਜੇ_ਅੰਤ /* EVENT_GROUPS_H */
