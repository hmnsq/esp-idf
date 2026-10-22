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


#यदि_अपरिभाषितम् PORTMACRO_H
#परिभाषय PORTMACRO_H

#अन्तर्भाव <limits.h>

#यदि_परिभाषितम् __cplusplus
बाह्य "C" {
#यदि_अन्तः

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
#परिभाषय पत्तनम्_CHAR        वर्ण
#परिभाषय पत्तनम्_FLOAT       भिन्न
#परिभाषय पत्तनम्_DOUBLE      द्विगुण
#परिभाषय पत्तनम्_LONG        दीर्घ
#परिभाषय पत्तनम्_SHORT       ह्रस्व
#परिभाषय पत्तनम्_राशिः_TYPE  अचिह्नित दीर्घ
#परिभाषय पत्तनम्_आधारः_TYPE   दीर्घ
#परिभाषय पत्तनम्_POINTER_आकार_TYPE intptr_t

प्रकारनिर्देश पत्तनम्_राशिः_TYPE राशिः_Type_t;
प्रकारनिर्देश पत्तनम्_आधारः_TYPE आधारः_Type_t;
प्रकारनिर्देश अचिह्नित पत्तनम्_आधारः_TYPE UBaseType_t;

प्रकारनिर्देश अचिह्नित दीर्घ TickType_t;
#परिभाषय पत्तनम्_MAX_DELAY ( TickType_t ) ULONG_MAX

#परिभाषय पत्तनम्_TICK_TYPE_IS_ATOMIC 1

/*-----------------------------------------------------------*/

/* Architecture specifics. */
#परिभाषय पत्तनम्_राशिः_GROWTH            ( -1 )
#परिभाषय पत्तनम्_HAS_राशिः_अतिप्रवाहः_CHECKING ( 1 )
#परिभाषय पत्तनम्_TICK_PERIOD_MS          ( ( TickType_t ) 1000 / config_TICK_दरः_HZ )
#परिभाषय पत्तनम्_TICK_दरः_MICROSECONDS  ( ( TickType_t ) 1000000 / config_TICK_दरः_HZ )
#परिभाषय पत्तनम्_BYTE_ALIGNMENT          8
/*-----------------------------------------------------------*/

/* Scheduler utilities. */
बाह्य शून्य v_पत्तनम्_Yield_Within_Api( शून्य );
#परिभाषय पत्तनम्_YIELD_WITHIN_API() v_पत्तनम्_Yield_Within_Api()

बाह्य शून्य v_पत्तनम्_Yield( शून्य );
#परिभाषय पत्तनम्_YIELD() v_पत्तनम्_Yield()

#परिभाषय पत्तनम्_अन्तः_SWITCHING_ISR( xSwitchRequired ) यदि( (xSwitchRequired) != pdFALSE ) v_पत्तनम्_Yield()
#परिभाषय पत्तनम्_YIELD_FROM_ISR( x ) पत्तनम्_अन्तः_SWITCHING_ISR( x )
/*-----------------------------------------------------------*/

/* Critical section management. */
बाह्य शून्य v_पत्तनम्_Disable_Interrupts( शून्य );
बाह्य शून्य v_पत्तनम्_Enable_Interrupts( शून्य );
#परिभाषय पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्()        ( v_पत्तनम्_Disable_Interrupts() )
#परिभाषय पत्तनम्_CLEAR_व्यवधानम्_आवरणम्()      ( v_पत्तनम्_Enable_Interrupts() )

बाह्य आधारः_Type_t x_पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्( शून्य );
बाह्य शून्य v_पत्तनम्_Clear_व्यवधानम्_आवरणम्( आधारः_Type_t x_आवरणम् );

शून्य v_पत्तनम्_Enter_Critical( शून्य );
शून्य v_पत्तनम्_Exit_Critical( शून्य );
#परिभाषय पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्_FROM_ISR()       x_पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्()
#परिभाषय पत्तनम्_CLEAR_व्यवधानम्_आवरणम्_FROM_ISR(x)    v_पत्तनम्_Clear_व्यवधानम्_आवरणम्(x)
#परिभाषय पत्तनम्_DISABLE_INTERRUPTS()                पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्()
#परिभाषय पत्तनम्_ENABLE_INTERRUPTS()                 पत्तनम्_CLEAR_व्यवधानम्_आवरणम्()
#परिभाषय पत्तनम्_ENTER_CRITICAL(mux)                 {(शून्य)mux;  v_पत्तनम्_Enter_Critical();}
#परिभाषय पत्तनम्_EXIT_CRITICAL(mux)                  {(शून्य)mux;  v_पत्तनम्_Exit_Critical();}
#परिभाषय पत्तनम्_ENTER_CRITICAL_SAFE(mux)            {(शून्य)mux;  v_पत्तनम्_Enter_Critical();}
#परिभाषय पत्तनम्_EXIT_CRITICAL_SAFE(mux)             {(शून्य)mux;  v_पत्तनम्_Exit_Critical();}
#परिभाषय पत्तनम्_ENTER_CRITICAL_ISR(mux)             पत्तनम्_ENTER_CRITICAL(mux)
#परिभाषय पत्तनम्_EXIT_CRITICAL_ISR(mux)              पत्तनम्_EXIT_CRITICAL(mux)

#परिभाषय prvENTER_CRITICAL_SMP_ONLY( px_तालकम् )         पत्तनम्_ENTER_CRITICAL( px_तालकम् )
#परिभाषय prvEXIT_CRITICAL_SMP_ONLY( px_तालकम् )          पत्तनम्_EXIT_CRITICAL( px_तालकम् )

बाह्य शून्य v_पत्तनम्_Suspend_Scheduler(शून्य);
#परिभाषय पत्तनम्_SOFTWARE_BARRIER() v_पत्तनम्_Suspend_Scheduler()

/*-----------------------------------------------------------*/

बाह्य शून्य v_पत्तनम्_Thread_Dying( शून्य *px_Task_गन्तव्यम्_Delete, चञ्चल आधारः_Type_t *pxPendYield );
बाह्य शून्य v_पत्तनम्_Cancel_Thread( शून्य *px_Task_गन्तव्यम्_Delete );
#परिभाषय पत्तनम्_PRE_TASK_DELETE_HOOK( pv_Task_गन्तव्यम्_Delete, pxPendYield ) v_पत्तनम्_Thread_Dying( ( pv_Task_गन्तव्यम्_Delete ), ( pxPendYield ) )
#परिभाषय पत्तनम्_CLEAN_ऊर्ध्वम्_TCB( pxTCB )   v_पत्तनम्_Cancel_Thread( pxTCB )
/*-----------------------------------------------------------*/

#परिभाषय पत्तनम्_TASK_FUNCTION_PROTO( vFunction, pvParameters ) शून्य vFunction( शून्य *pvParameters )
#परिभाषय पत्तनम्_TASK_FUNCTION( vFunction, pvParameters ) शून्य vFunction( शून्य *pvParameters )
/*-----------------------------------------------------------*/

/*
 * Tasks run in their own pthreads and context switches between them
 * are always a full memory barrier. ISRs are emulated as signals
 * which also imply a full memory barrier.
 *
 * Thus, only a compiler barrier is needed to prevent the compiler
 * reordering.
 */
#परिभाषय पत्तनम्_स्मृतिः_BARRIER() __asm चञ्चल( "" ::: "memory" )

#परिभाषय पत्तनम्_CONFIGURE_TIMER_FOR_सततम्_TIME_STATS() /* no-op */
#यदि ( CONFIG_FREERTOS_GENERATE_सततम्_TIME_STATS )
config_सततम्_TIME_गणक_TYPE x_पत्तनम्_Get_सततम्_Time_गणक_मानम्( शून्य );
#परिभाषय पत्तनम्_GET_सततम्_TIME_गणक_मानम्()         x_पत्तनम्_Get_सततम्_Time_गणक_मानम्()
#यदि_अन्तः

#यदि_परिभाषितम् __cplusplus
}
#यदि_अन्तः

// We need additional definitions for ESP-IDF code
#अन्तर्भाव "portmacro_idf.h"

शून्य v_पत्तनम्_समुच्चयः_राशिः_Watchpoint(शून्य *px_राशिः_Start);

#यदि_अन्तः /* PORTMACRO_H */
