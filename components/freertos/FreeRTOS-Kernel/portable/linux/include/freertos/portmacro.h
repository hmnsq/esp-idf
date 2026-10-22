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

#અન્તર્ભાવ <limits.h>

#ifdef __cplusplus
બાહ્ય "C" {
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
#define portCHAR        અક્ષર
#define portFLOAT       દશાંશ
#define portDOUBLE      દ્વિગુણ
#define portLONG        long
#define portSHORT       short
#define port_ઢગલો_TYPE  unsigned long
#define portBASE_TYPE   long
#define portPOINTER_SIZE_TYPE intptr_t

પ્રકારવ્યાખ્યા port_ઢગલો_TYPE ઢગલો_Type_t;
પ્રકારવ્યાખ્યા portBASE_TYPE BaseType_t;
પ્રકારવ્યાખ્યા unsigned portBASE_TYPE UBaseType_t;

પ્રકારવ્યાખ્યા unsigned long TickType_t;
#define portMAX_DELAY ( TickType_t ) ULONG_MAX

#define portTICK_TYPE_IS_ATOMIC 1

/*-----------------------------------------------------------*/

/* Architecture specifics. */
#define port_ઢગલો_GROWTH            ( -1 )
#define port_HAS_ઢગલો_OVERFLOW_CHECKING ( 1 )
#define portTICK_PERIOD_MS          ( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#define portTICK_RATE_MICROSECONDS  ( ( TickType_t ) 1000000 / configTICK_RATE_HZ )
#define portBYTE_ALIGNMENT          8
/*-----------------------------------------------------------*/

/* Scheduler utilities. */
બાહ્ય શૂન્ય vPortYieldWithinApi( શૂન્ય );
#define portYIELD_WITHIN_API() vPortYieldWithinApi()

બાહ્ય શૂન્ય vPortYield( શૂન્ય );
#define portYIELD() vPortYield()

#define portEND_SWITCHING_ISR( xSwitchRequired ) જો( (xSwitchRequired) != pdFALSE ) vPortYield()
#define portYIELD_FROM_ISR( x ) portEND_SWITCHING_ISR( x )
/*-----------------------------------------------------------*/

/* Critical section management. */
બાહ્ય શૂન્ય vPortDisableInterrupts( શૂન્ય );
બાહ્ય શૂન્ય vPortEnableInterrupts( શૂન્ય );
#define portSET_INTERRUPT_MASK()        ( vPortDisableInterrupts() )
#define portCLEAR_INTERRUPT_MASK()      ( vPortEnableInterrupts() )

બાહ્ય BaseType_t xPortSetInterruptMask( શૂન્ય );
બાહ્ય શૂન્ય vPortClearInterruptMask( BaseType_t xMask );

શૂન્ય vPortEnterCritical( શૂન્ય );
શૂન્ય vPortExitCritical( શૂન્ય );
#define portSET_INTERRUPT_MASK_FROM_ISR()       xPortSetInterruptMask()
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x)    vPortClearInterruptMask(x)
#define portDISABLE_INTERRUPTS()                portSET_INTERRUPT_MASK()
#define portENABLE_INTERRUPTS()                 portCLEAR_INTERRUPT_MASK()
#define portENTER_CRITICAL(mux)                 {(શૂન્ય)mux;  vPortEnterCritical();}
#define portEXIT_CRITICAL(mux)                  {(શૂન્ય)mux;  vPortExitCritical();}
#define portENTER_CRITICAL_SAFE(mux)            {(શૂન્ય)mux;  vPortEnterCritical();}
#define portEXIT_CRITICAL_SAFE(mux)             {(શૂન્ય)mux;  vPortExitCritical();}
#define portENTER_CRITICAL_ISR(mux)             portENTER_CRITICAL(mux)
#define portEXIT_CRITICAL_ISR(mux)              portEXIT_CRITICAL(mux)

#define prvENTER_CRITICAL_SMP_ONLY( pxLock )         portENTER_CRITICAL( pxLock )
#define prvEXIT_CRITICAL_SMP_ONLY( pxLock )          portEXIT_CRITICAL( pxLock )

બાહ્ય શૂન્ય vPortSuspendScheduler(શૂન્ય);
#define portSOFTWARE_BARRIER() vPortSuspendScheduler()

/*-----------------------------------------------------------*/

બાહ્ય શૂન્ય vPortThreadDying( શૂન્ય *pxTaskToDelete, અસ્થિર BaseType_t *pxPendYield );
બાહ્ય શૂન્ય vPortCancelThread( શૂન્ય *pxTaskToDelete );
#define portPRE_TASK_DELETE_HOOK( pvTaskToDelete, pxPendYield ) vPortThreadDying( ( pvTaskToDelete ), ( pxPendYield ) )
#define portCLEAN_UP_TCB( pxTCB )   vPortCancelThread( pxTCB )
/*-----------------------------------------------------------*/

#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) શૂન્ય vFunction( શૂન્ય *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters ) શૂન્ય vFunction( શૂન્ય *pvParameters )
/*-----------------------------------------------------------*/

/*
 * Tasks run in their own pthreads and context switches between them
 * are always a full memory barrier. ISRs are emulated as signals
 * which also imply a full memory barrier.
 *
 * Thus, only a compiler barrier is needed to prevent the compiler
 * reordering.
 */
#define portMEMORY_BARRIER() __asm અસ્થિર( "" ::: "memory" )

#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() /* no-op */
#જો ( CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS )
configRUN_TIME_COUNTER_TYPE x_Port_Get_Run_Time_Counter_કિંમત( શૂન્ય );
#define port_GET_RUN_TIME_COUNTER_કિંમત()         x_Port_Get_Run_Time_Counter_કિંમત()
#endif

#ifdef __cplusplus
}
#endif

// We need additional definitions for ESP-IDF code
#અન્તર્ભાવ "portmacro_idf.h"

શૂન્ય v_Port_Set_ઢગલો_Watchpoint(શૂન્ય *px_ઢગલો_Start);

#endif /* PORTMACRO_H */
