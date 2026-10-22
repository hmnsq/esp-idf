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

#അന്തര്ഭാവ <limits.h>

#ifdef __cplusplus
ബാഹ്യ "C" {
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
#define portCHAR        അക്ഷരം
#define portFLOAT       ദശാംശം
#define portDOUBLE      ഇരട്ട
#define portLONG        long
#define portSHORT       short
#define port_അട്ടി_TYPE  unsigned long
#define portBASE_TYPE   long
#define portPOINTER_SIZE_TYPE intptr_t

തരനിർവചനം port_അട്ടി_TYPE അട്ടി_Type_t;
തരനിർവചനം portBASE_TYPE BaseType_t;
തരനിർവചനം unsigned portBASE_TYPE UBaseType_t;

തരനിർവചനം unsigned long TickType_t;
#define portMAX_DELAY ( TickType_t ) ULONG_MAX

#define portTICK_TYPE_IS_ATOMIC 1

/*-----------------------------------------------------------*/

/* Architecture specifics. */
#define port_അട്ടി_GROWTH            ( -1 )
#define port_HAS_അട്ടി_OVERFLOW_CHECKING ( 1 )
#define portTICK_PERIOD_MS          ( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#define portTICK_RATE_MICROSECONDS  ( ( TickType_t ) 1000000 / configTICK_RATE_HZ )
#define portBYTE_ALIGNMENT          8
/*-----------------------------------------------------------*/

/* Scheduler utilities. */
ബാഹ്യ ശൂന്യം vPortYieldWithinApi( ശൂന്യം );
#define portYIELD_WITHIN_API() vPortYieldWithinApi()

ബാഹ്യ ശൂന്യം vPortYield( ശൂന്യം );
#define portYIELD() vPortYield()

#define portEND_SWITCHING_ISR( xSwitchRequired ) എങ്കിൽ( (xSwitchRequired) != pdFALSE ) vPortYield()
#define portYIELD_FROM_ISR( x ) portEND_SWITCHING_ISR( x )
/*-----------------------------------------------------------*/

/* Critical section management. */
ബാഹ്യ ശൂന്യം vPortDisableInterrupts( ശൂന്യം );
ബാഹ്യ ശൂന്യം vPortEnableInterrupts( ശൂന്യം );
#define portSET_INTERRUPT_MASK()        ( vPortDisableInterrupts() )
#define portCLEAR_INTERRUPT_MASK()      ( vPortEnableInterrupts() )

ബാഹ്യ BaseType_t xPortSetInterruptMask( ശൂന്യം );
ബാഹ്യ ശൂന്യം vPortClearInterruptMask( BaseType_t xMask );

ശൂന്യം vPortEnterCritical( ശൂന്യം );
ശൂന്യം vPortExitCritical( ശൂന്യം );
#define portSET_INTERRUPT_MASK_FROM_ISR()       xPortSetInterruptMask()
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x)    vPortClearInterruptMask(x)
#define portDISABLE_INTERRUPTS()                portSET_INTERRUPT_MASK()
#define portENABLE_INTERRUPTS()                 portCLEAR_INTERRUPT_MASK()
#define portENTER_CRITICAL(mux)                 {(ശൂന്യം)mux;  vPortEnterCritical();}
#define portEXIT_CRITICAL(mux)                  {(ശൂന്യം)mux;  vPortExitCritical();}
#define portENTER_CRITICAL_SAFE(mux)            {(ശൂന്യം)mux;  vPortEnterCritical();}
#define portEXIT_CRITICAL_SAFE(mux)             {(ശൂന്യം)mux;  vPortExitCritical();}
#define portENTER_CRITICAL_ISR(mux)             portENTER_CRITICAL(mux)
#define portEXIT_CRITICAL_ISR(mux)              portEXIT_CRITICAL(mux)

#define prvENTER_CRITICAL_SMP_ONLY( pxLock )         portENTER_CRITICAL( pxLock )
#define prvEXIT_CRITICAL_SMP_ONLY( pxLock )          portEXIT_CRITICAL( pxLock )

ബാഹ്യ ശൂന്യം vPortSuspendScheduler(ശൂന്യം);
#define portSOFTWARE_BARRIER() vPortSuspendScheduler()

/*-----------------------------------------------------------*/

ബാഹ്യ ശൂന്യം vPortThreadDying( ശൂന്യം *pxTaskToDelete, അസ്ഥിര BaseType_t *pxPendYield );
ബാഹ്യ ശൂന്യം vPortCancelThread( ശൂന്യം *pxTaskToDelete );
#define portPRE_TASK_DELETE_HOOK( pvTaskToDelete, pxPendYield ) vPortThreadDying( ( pvTaskToDelete ), ( pxPendYield ) )
#define portCLEAN_UP_TCB( pxTCB )   vPortCancelThread( pxTCB )
/*-----------------------------------------------------------*/

#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) ശൂന്യം vFunction( ശൂന്യം *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters ) ശൂന്യം vFunction( ശൂന്യം *pvParameters )
/*-----------------------------------------------------------*/

/*
 * Tasks run in their own pthreads and context switches between them
 * are always a full memory barrier. ISRs are emulated as signals
 * which also imply a full memory barrier.
 *
 * Thus, only a compiler barrier is needed to prevent the compiler
 * reordering.
 */
#define portMEMORY_BARRIER() __asm അസ്ഥിര( "" ::: "memory" )

#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() /* no-op */
#എങ്കിൽ ( CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS )
configRUN_TIME_COUNTER_TYPE x_Port_Get_Run_Time_Counter_മൂല്യം( ശൂന്യം );
#define port_GET_RUN_TIME_COUNTER_മൂല്യം()         x_Port_Get_Run_Time_Counter_മൂല്യം()
#endif

#ifdef __cplusplus
}
#endif

// We need additional definitions for ESP-IDF code
#അന്തര്ഭാവ "portmacro_idf.h"

ശൂന്യം v_Port_Set_അട്ടി_Watchpoint(ശൂന്യം *px_അട്ടി_Start);

#endif /* PORTMACRO_H */
