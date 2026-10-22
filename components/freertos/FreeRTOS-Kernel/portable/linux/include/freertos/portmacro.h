/*
 * FreeRTOS Kernel V10.4.6
 * Copyright 2020 Cambridge Consultants Ltd.
 *
 * SPDX-FileCopyrightText: 2020 Cambridge Consultants Ltd.
 *
 * SPDX-License-Identifier: MIT
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
 */


#ifndef PORTMACRO_H
#define PORTMACRO_H

#ଅନ୍ତର୍ଭାଵ <limits.h>

#ifdef __cplusplus
ବାହ୍ୟ "C" {
#endif

/*-----------------------------------------------------------
 * Port specific definitions.
 *
 * The settings in this file configure FreeRTOS correctly for the
 * given hardware and compiler.
 *
 * These settings should not be altered.
 *-----------------------------------------------------------
 */

/* Type definitions. */
#define portCHAR        ଅକ୍ଷର
#define portFLOAT       ଦଶମିକ
#define portDOUBLE      ଦ୍ୱିଗୁଣ
#define portLONG        long
#define portSHORT       short
#define port_ଗଦା_TYPE  unsigned long
#define portBASE_TYPE   long
#define portPOINTER_SIZE_TYPE intptr_t

ପ୍ରକାରବ୍ୟାଖ୍ୟା port_ଗଦା_TYPE ଗଦା_Type_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା portBASE_TYPE BaseType_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା unsigned portBASE_TYPE UBaseType_t;

ପ୍ରକାରବ୍ୟାଖ୍ୟା unsigned long TickType_t;
#define portMAX_DELAY ( TickType_t ) ULONG_MAX

#define portTICK_TYPE_IS_ATOMIC 1

/*-----------------------------------------------------------*/

/* Architecture specifics. */
#define port_ଗଦା_GROWTH            ( -1 )
#define port_HAS_ଗଦା_OVERFLOW_CHECKING ( 1 )
#define portTICK_PERIOD_MS          ( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#define portTICK_RATE_MICROSECONDS  ( ( TickType_t ) 1000000 / configTICK_RATE_HZ )
#define portBYTE_ALIGNMENT          8
/*-----------------------------------------------------------*/

/* Scheduler utilities. */
ବାହ୍ୟ ଶୂନ୍ୟ vPortYieldWithinApi( ଶୂନ୍ୟ );
#define portYIELD_WITHIN_API() vPortYieldWithinApi()

ବାହ୍ୟ ଶୂନ୍ୟ vPortYield( ଶୂନ୍ୟ );
#define portYIELD() vPortYield()

#define portEND_SWITCHING_ISR( xSwitchRequired ) ଯଦି( (xSwitchRequired) != pdFALSE ) vPortYield()
#define portYIELD_FROM_ISR( x ) portEND_SWITCHING_ISR( x )
/*-----------------------------------------------------------*/

/* Critical section management. */
ବାହ୍ୟ ଶୂନ୍ୟ vPortDisableInterrupts( ଶୂନ୍ୟ );
ବାହ୍ୟ ଶୂନ୍ୟ vPortEnableInterrupts( ଶୂନ୍ୟ );
#define portSET_INTERRUPT_MASK()        ( vPortDisableInterrupts() )
#define portCLEAR_INTERRUPT_MASK()      ( vPortEnableInterrupts() )

ବାହ୍ୟ BaseType_t xPortSetInterruptMask( ଶୂନ୍ୟ );
ବାହ୍ୟ ଶୂନ୍ୟ vPortClearInterruptMask( BaseType_t xMask );

ଶୂନ୍ୟ vPortEnterCritical( ଶୂନ୍ୟ );
ଶୂନ୍ୟ vPortExitCritical( ଶୂନ୍ୟ );
#define portSET_INTERRUPT_MASK_FROM_ISR()       xPortSetInterruptMask()
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x)    vPortClearInterruptMask(x)
#define portDISABLE_INTERRUPTS()                portSET_INTERRUPT_MASK()
#define portENABLE_INTERRUPTS()                 portCLEAR_INTERRUPT_MASK()
#define portENTER_CRITICAL(mux)                 {(ଶୂନ୍ୟ)mux;  vPortEnterCritical();}
#define portEXIT_CRITICAL(mux)                  {(ଶୂନ୍ୟ)mux;  vPortExitCritical();}
#define portENTER_CRITICAL_SAFE(mux)            {(ଶୂନ୍ୟ)mux;  vPortEnterCritical();}
#define portEXIT_CRITICAL_SAFE(mux)             {(ଶୂନ୍ୟ)mux;  vPortExitCritical();}
#define portENTER_CRITICAL_ISR(mux)             portENTER_CRITICAL(mux)
#define portEXIT_CRITICAL_ISR(mux)              portEXIT_CRITICAL(mux)

#define prvENTER_CRITICAL_SMP_ONLY( pxLock )         portENTER_CRITICAL( pxLock )
#define prvEXIT_CRITICAL_SMP_ONLY( pxLock )          portEXIT_CRITICAL( pxLock )

ବାହ୍ୟ ଶୂନ୍ୟ vPortSuspendScheduler(ଶୂନ୍ୟ);
#define portSOFTWARE_BARRIER() vPortSuspendScheduler()

/*-----------------------------------------------------------*/

ବାହ୍ୟ ଶୂନ୍ୟ vPortThreadDying( ଶୂନ୍ୟ *pxTaskToDelete, ଅସ୍ଥିର BaseType_t *pxPendYield );
ବାହ୍ୟ ଶୂନ୍ୟ vPortCancelThread( ଶୂନ୍ୟ *pxTaskToDelete );
#define portPRE_TASK_DELETE_HOOK( pvTaskToDelete, pxPendYield ) vPortThreadDying( ( pvTaskToDelete ), ( pxPendYield ) )
#define portCLEAN_UP_TCB( pxTCB )   vPortCancelThread( pxTCB )
/*-----------------------------------------------------------*/

#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) ଶୂନ୍ୟ vFunction( ଶୂନ୍ୟ *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters ) ଶୂନ୍ୟ vFunction( ଶୂନ୍ୟ *pvParameters )
/*-----------------------------------------------------------*/

/*
 * Tasks run in their own pthreads and context switches between them
 * are always a full memory barrier. ISRs are emulated as signals
 * which also imply a full memory barrier.
 *
 * Thus, only a compiler barrier is needed to prevent the compiler
 * reordering.
 */
#define portMEMORY_BARRIER() __asm ଅସ୍ଥିର( "" ::: "memory" )

#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() /* no-op */
#ଯଦି ( CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS )
configRUN_TIME_COUNTER_TYPE x_Port_Get_Run_Time_Counter_ମୂଲ୍ୟ( ଶୂନ୍ୟ );
#define port_GET_RUN_TIME_COUNTER_ମୂଲ୍ୟ()         x_Port_Get_Run_Time_Counter_ମୂଲ୍ୟ()
#endif

#ifdef __cplusplus
}
#endif

// We need additional definitions for ESP-IDF code
#ଅନ୍ତର୍ଭାଵ "portmacro_idf.h"

ଶୂନ୍ୟ v_Port_Set_ଗଦା_Watchpoint(ଶୂନ୍ୟ *px_ଗଦା_Start);

#endif /* PORTMACRO_H */
