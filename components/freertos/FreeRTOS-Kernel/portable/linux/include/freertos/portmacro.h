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

#अन्तर्भाव <limits.h>

#ifdef __cplusplus
बाह्य "C" {
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
#define portCHAR        अक्षर
#define portFLOAT       दशांश
#define portDOUBLE      द्विगुण
#define portLONG        दीर्घ
#define portSHORT       लघु
#define port_चळत_TYPE  अचिन्हित दीर्घ
#define portBASE_TYPE   दीर्घ
#define portPOINTER_SIZE_TYPE intptr_t

प्रकारव्याख्या port_चळत_TYPE चळत_Type_t;
प्रकारव्याख्या portBASE_TYPE BaseType_t;
प्रकारव्याख्या अचिन्हित portBASE_TYPE UBaseType_t;

प्रकारव्याख्या अचिन्हित दीर्घ TickType_t;
#define portMAX_DELAY ( TickType_t ) अच_दीर्घ_उच्च

#define portTICK_TYPE_IS_ATOMIC 1

/*-----------------------------------------------------------*/

/* Architecture specifics. */
#define port_चळत_GROWTH            ( -1 )
#define port_HAS_चळत_OVERFLOW_CHECKING ( 1 )
#define portTICK_PERIOD_MS          ( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#define portTICK_RATE_MICROSECONDS  ( ( TickType_t ) 1000000 / configTICK_RATE_HZ )
#define portBYTE_ALIGNMENT          8
/*-----------------------------------------------------------*/

/* Scheduler utilities. */
बाह्य शून्य vPortYieldWithinApi( शून्य );
#define portYIELD_WITHIN_API() vPortYieldWithinApi()

बाह्य शून्य vPortYield( शून्य );
#define portYIELD() vPortYield()

#define portEND_SWITCHING_ISR( xSwitchRequired ) जर( (xSwitchRequired) != pdFALSE ) vPortYield()
#define portYIELD_FROM_ISR( x ) portEND_SWITCHING_ISR( x )
/*-----------------------------------------------------------*/

/* Critical section management. */
बाह्य शून्य vPortDisableInterrupts( शून्य );
बाह्य शून्य vPortEnableInterrupts( शून्य );
#define portSET_INTERRUPT_MASK()        ( vPortDisableInterrupts() )
#define portCLEAR_INTERRUPT_MASK()      ( vPortEnableInterrupts() )

बाह्य BaseType_t xPortSetInterruptMask( शून्य );
बाह्य शून्य vPortClearInterruptMask( BaseType_t xMask );

शून्य vPortEnterCritical( शून्य );
शून्य vPortExitCritical( शून्य );
#define portSET_INTERRUPT_MASK_FROM_ISR()       xPortSetInterruptMask()
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x)    vPortClearInterruptMask(x)
#define portDISABLE_INTERRUPTS()                portSET_INTERRUPT_MASK()
#define portENABLE_INTERRUPTS()                 portCLEAR_INTERRUPT_MASK()
#define portENTER_CRITICAL(mux)                 {(शून्य)mux;  vPortEnterCritical();}
#define portEXIT_CRITICAL(mux)                  {(शून्य)mux;  vPortExitCritical();}
#define portENTER_CRITICAL_SAFE(mux)            {(शून्य)mux;  vPortEnterCritical();}
#define portEXIT_CRITICAL_SAFE(mux)             {(शून्य)mux;  vPortExitCritical();}
#define portENTER_CRITICAL_ISR(mux)             portENTER_CRITICAL(mux)
#define portEXIT_CRITICAL_ISR(mux)              portEXIT_CRITICAL(mux)

#define prvENTER_CRITICAL_SMP_ONLY( pxLock )         portENTER_CRITICAL( pxLock )
#define prvEXIT_CRITICAL_SMP_ONLY( pxLock )          portEXIT_CRITICAL( pxLock )

बाह्य शून्य vPortSuspendScheduler(शून्य);
#define portSOFTWARE_BARRIER() vPortSuspendScheduler()

/*-----------------------------------------------------------*/

बाह्य शून्य vPortThreadDying( शून्य *pxTaskToDelete, अस्थिर BaseType_t *pxPendYield );
बाह्य शून्य vPortCancelThread( शून्य *pxTaskToDelete );
#define portPRE_TASK_DELETE_HOOK( pvTaskToDelete, pxPendYield ) vPortThreadDying( ( pvTaskToDelete ), ( pxPendYield ) )
#define portCLEAN_UP_TCB( pxTCB )   vPortCancelThread( pxTCB )
/*-----------------------------------------------------------*/

#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) शून्य vFunction( शून्य *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters ) शून्य vFunction( शून्य *pvParameters )
/*-----------------------------------------------------------*/

/*
 * Tasks run in their own pthreads and context switches between them
 * are always a full memory barrier. ISRs are emulated as signals
 * which also imply a full memory barrier.
 *
 * Thus, only a compiler barrier is needed to prevent the compiler
 * reordering.
 */
#define portMEMORY_BARRIER() __asm अस्थिर( "" ::: "memory" )

#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() /* no-op */
#जर ( CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS )
configRUN_TIME_COUNTER_TYPE x_Port_Get_Run_Time_Counter_मूल्य( शून्य );
#define port_GET_RUN_TIME_COUNTER_मूल्य()         x_Port_Get_Run_Time_Counter_मूल्य()
#endif

#ifdef __cplusplus
}
#endif

// We need additional definitions for ESP-IDF code
#अन्तर्भाव "portmacro_idf.h"

शून्य v_Port_Set_चळत_Watchpoint(शून्य *px_चळत_Start);

#endif /* PORTMACRO_H */
