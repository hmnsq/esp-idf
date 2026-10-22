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

#ಅನ್ತರ್ಭಾವ <limits.h>

#ifdef __cplusplus
ಬಾಹ್ಯ "C" {
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
#define portCHAR        ವರ್ಣ
#define portFLOAT       ಭಿನ್ನ
#define portDOUBLE      ದ್ವಿಗುಣ
#define portLONG        long
#define portSHORT       short
#define port_ರಾಶಿ_TYPE  unsigned long
#define portBASE_TYPE   long
#define portPOINTER_SIZE_TYPE intptr_t

ಪ್ರಕಾರನಿರ್ದೇಶ port_ರಾಶಿ_TYPE ರಾಶಿ_Type_t;
ಪ್ರಕಾರನಿರ್ದೇಶ portBASE_TYPE BaseType_t;
ಪ್ರಕಾರನಿರ್ದೇಶ unsigned portBASE_TYPE UBaseType_t;

ಪ್ರಕಾರನಿರ್ದೇಶ unsigned long TickType_t;
#define portMAX_DELAY ( TickType_t ) ULONG_MAX

#define portTICK_TYPE_IS_ATOMIC 1

/*-----------------------------------------------------------*/

/* Architecture specifics. */
#define port_ರಾಶಿ_GROWTH            ( -1 )
#define port_HAS_ರಾಶಿ_OVERFLOW_CHECKING ( 1 )
#define portTICK_PERIOD_MS          ( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#define portTICK_RATE_MICROSECONDS  ( ( TickType_t ) 1000000 / configTICK_RATE_HZ )
#define portBYTE_ALIGNMENT          8
/*-----------------------------------------------------------*/

/* Scheduler utilities. */
ಬಾಹ್ಯ ಶೂನ್ಯ vPortYieldWithinApi( ಶೂನ್ಯ );
#define portYIELD_WITHIN_API() vPortYieldWithinApi()

ಬಾಹ್ಯ ಶೂನ್ಯ vPortYield( ಶೂನ್ಯ );
#define portYIELD() vPortYield()

#define portEND_SWITCHING_ISR( xSwitchRequired ) ಯದಿ( (xSwitchRequired) != pdFALSE ) vPortYield()
#define portYIELD_FROM_ISR( x ) portEND_SWITCHING_ISR( x )
/*-----------------------------------------------------------*/

/* Critical section management. */
ಬಾಹ್ಯ ಶೂನ್ಯ vPortDisableInterrupts( ಶೂನ್ಯ );
ಬಾಹ್ಯ ಶೂನ್ಯ vPortEnableInterrupts( ಶೂನ್ಯ );
#define portSET_INTERRUPT_MASK()        ( vPortDisableInterrupts() )
#define portCLEAR_INTERRUPT_MASK()      ( vPortEnableInterrupts() )

ಬಾಹ್ಯ BaseType_t xPortSetInterruptMask( ಶೂನ್ಯ );
ಬಾಹ್ಯ ಶೂನ್ಯ vPortClearInterruptMask( BaseType_t xMask );

ಶೂನ್ಯ vPortEnterCritical( ಶೂನ್ಯ );
ಶೂನ್ಯ vPortExitCritical( ಶೂನ್ಯ );
#define portSET_INTERRUPT_MASK_FROM_ISR()       xPortSetInterruptMask()
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x)    vPortClearInterruptMask(x)
#define portDISABLE_INTERRUPTS()                portSET_INTERRUPT_MASK()
#define portENABLE_INTERRUPTS()                 portCLEAR_INTERRUPT_MASK()
#define portENTER_CRITICAL(mux)                 {(ಶೂನ್ಯ)mux;  vPortEnterCritical();}
#define portEXIT_CRITICAL(mux)                  {(ಶೂನ್ಯ)mux;  vPortExitCritical();}
#define portENTER_CRITICAL_SAFE(mux)            {(ಶೂನ್ಯ)mux;  vPortEnterCritical();}
#define portEXIT_CRITICAL_SAFE(mux)             {(ಶೂನ್ಯ)mux;  vPortExitCritical();}
#define portENTER_CRITICAL_ISR(mux)             portENTER_CRITICAL(mux)
#define portEXIT_CRITICAL_ISR(mux)              portEXIT_CRITICAL(mux)

#define prvENTER_CRITICAL_SMP_ONLY( pxLock )         portENTER_CRITICAL( pxLock )
#define prvEXIT_CRITICAL_SMP_ONLY( pxLock )          portEXIT_CRITICAL( pxLock )

ಬಾಹ್ಯ ಶೂನ್ಯ vPortSuspendScheduler(ಶೂನ್ಯ);
#define portSOFTWARE_BARRIER() vPortSuspendScheduler()

/*-----------------------------------------------------------*/

ಬಾಹ್ಯ ಶೂನ್ಯ vPortThreadDying( ಶೂನ್ಯ *pxTaskToDelete, ಚಞ್ಚಲ BaseType_t *pxPendYield );
ಬಾಹ್ಯ ಶೂನ್ಯ vPortCancelThread( ಶೂನ್ಯ *pxTaskToDelete );
#define portPRE_TASK_DELETE_HOOK( pvTaskToDelete, pxPendYield ) vPortThreadDying( ( pvTaskToDelete ), ( pxPendYield ) )
#define portCLEAN_UP_TCB( pxTCB )   vPortCancelThread( pxTCB )
/*-----------------------------------------------------------*/

#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) ಶೂನ್ಯ vFunction( ಶೂನ್ಯ *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters ) ಶೂನ್ಯ vFunction( ಶೂನ್ಯ *pvParameters )
/*-----------------------------------------------------------*/

/*
 * Tasks run in their own pthreads and context switches between them
 * are always a full memory barrier. ISRs are emulated as signals
 * which also imply a full memory barrier.
 *
 * Thus, only a compiler barrier is needed to prevent the compiler
 * reordering.
 */
#define portMEMORY_BARRIER() __asm ಚಞ್ಚಲ( "" ::: "memory" )

#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() /* no-op */
#ಯದಿ ( CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS )
configRUN_TIME_COUNTER_TYPE x_Port_Get_Run_Time_Counter_ಮೌಲ್ಯ( ಶೂನ್ಯ );
#define port_GET_RUN_TIME_COUNTER_ಮೌಲ್ಯ()         x_Port_Get_Run_Time_Counter_ಮೌಲ್ಯ()
#endif

#ifdef __cplusplus
}
#endif

// We need additional definitions for ESP-IDF code
#ಅನ್ತರ್ಭಾವ "portmacro_idf.h"

ಶೂನ್ಯ v_Port_Set_ರಾಶಿ_Watchpoint(ಶೂನ್ಯ *px_ರಾಶಿ_Start);

#endif /* PORTMACRO_H */
