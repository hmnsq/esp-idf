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


#جے_بنا_تعریف PORTMACRO_H
#تعریف_کرو PORTMACRO_H

#ضمن <limits.h>

#جے_تعریف_شدہ __cplusplus
خارجي "C" {
#جے_انت

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
#تعریف_کرو پورٹ_CHAR        حرف
#تعریف_کرو پورٹ_FLOAT       عائم
#تعریف_کرو پورٹ_DOUBLE      مزدوج
#تعریف_کرو پورٹ_LONG        لما
#تعریف_کرو پورٹ_SHORT       چھوٹا
#تعریف_کرو پورٹ_ڈھیر_TYPE  بنانشانی لما
#تعریف_کرو پورٹ_ادھار_TYPE   لما
#تعریف_کرو پورٹ_اشارہ_گر_ناپ_TYPE intptr_t

تعريفنوع پورٹ_ڈھیر_TYPE ڈھیر_Type_t;
تعريفنوع پورٹ_ادھار_TYPE ادھار_Type_t;
تعريفنوع بنانشانی پورٹ_ادھار_TYPE UBaseType_t;

تعريفنوع بنانشانی لما TickType_t;
#تعریف_کرو پورٹ_MAX_DELAY ( TickType_t ) ULONG_MAX

#تعریف_کرو پورٹ_TICK_TYPE_IS_ATOMIC 1

/*-----------------------------------------------------------*/

/* Architecture specifics. */
#تعریف_کرو پورٹ_ڈھیر_GROWTH            ( -1 )
#تعریف_کرو پورٹ_HAS_ڈھیر_حد_توں_ودھ_CHECKING ( 1 )
#تعریف_کرو پورٹ_TICK_PERIOD_MS          ( ( TickType_t ) 1000 / config_TICK_شرح_HZ )
#تعریف_کرو پورٹ_TICK_شرح_MICROSECONDS  ( ( TickType_t ) 1000000 / config_TICK_شرح_HZ )
#تعریف_کرو پورٹ_BYTE_ALIGNMENT          8
/*-----------------------------------------------------------*/

/* Scheduler utilities. */
خارجي فراغ v_پورٹ_Yield_Within_Api( فراغ );
#تعریف_کرو پورٹ_YIELD_WITHIN_API() v_پورٹ_Yield_Within_Api()

خارجي فراغ v_پورٹ_Yield( فراغ );
#تعریف_کرو پورٹ_YIELD() v_پورٹ_Yield()

#تعریف_کرو پورٹ_سرا_SWITCHING_ISR( xSwitchRequired ) إذا( (xSwitchRequired) != pdFALSE ) v_پورٹ_Yield()
#تعریف_کرو پورٹ_YIELD_FROM_ISR( x ) پورٹ_سرا_SWITCHING_ISR( x )
/*-----------------------------------------------------------*/

/* Critical section management. */
خارجي فراغ v_پورٹ_Disable_Interrupts( فراغ );
خارجي فراغ v_پورٹ_Enable_Interrupts( فراغ );
#تعریف_کرو پورٹ_سیٹ_رکاوٹ_نقاب()        ( v_پورٹ_Disable_Interrupts() )
#تعریف_کرو پورٹ_CLEAR_رکاوٹ_نقاب()      ( v_پورٹ_Enable_Interrupts() )

خارجي ادھار_Type_t x_پورٹ_سیٹ_رکاوٹ_نقاب( فراغ );
خارجي فراغ v_پورٹ_Clear_رکاوٹ_نقاب( ادھار_Type_t x_نقاب );

فراغ v_پورٹ_Enter_Critical( فراغ );
فراغ v_پورٹ_Exit_Critical( فراغ );
#تعریف_کرو پورٹ_سیٹ_رکاوٹ_نقاب_FROM_ISR()       x_پورٹ_سیٹ_رکاوٹ_نقاب()
#تعریف_کرو پورٹ_CLEAR_رکاوٹ_نقاب_FROM_ISR(x)    v_پورٹ_Clear_رکاوٹ_نقاب(x)
#تعریف_کرو پورٹ_DISABLE_INTERRUPTS()                پورٹ_سیٹ_رکاوٹ_نقاب()
#تعریف_کرو پورٹ_ENABLE_INTERRUPTS()                 پورٹ_CLEAR_رکاوٹ_نقاب()
#تعریف_کرو پورٹ_ENTER_CRITICAL(mux)                 {(فراغ)mux;  v_پورٹ_Enter_Critical();}
#تعریف_کرو پورٹ_EXIT_CRITICAL(mux)                  {(فراغ)mux;  v_پورٹ_Exit_Critical();}
#تعریف_کرو پورٹ_ENTER_CRITICAL_محفوظ(mux)            {(فراغ)mux;  v_پورٹ_Enter_Critical();}
#تعریف_کرو پورٹ_EXIT_CRITICAL_محفوظ(mux)             {(فراغ)mux;  v_پورٹ_Exit_Critical();}
#تعریف_کرو پورٹ_ENTER_CRITICAL_ISR(mux)             پورٹ_ENTER_CRITICAL(mux)
#تعریف_کرو پورٹ_EXIT_CRITICAL_ISR(mux)              پورٹ_EXIT_CRITICAL(mux)

#تعریف_کرو prvENTER_CRITICAL_SMP_ONLY( px_جندرا )         پورٹ_ENTER_CRITICAL( px_جندرا )
#تعریف_کرو prvEXIT_CRITICAL_SMP_ONLY( px_جندرا )          پورٹ_EXIT_CRITICAL( px_جندرا )

خارجي فراغ v_پورٹ_Suspend_Scheduler(فراغ);
#تعریف_کرو پورٹ_SOFTWARE_BARRIER() v_پورٹ_Suspend_Scheduler()

/*-----------------------------------------------------------*/

خارجي فراغ v_پورٹ_Thread_Dying( فراغ *px_Task_منزل_Delete, متطاير ادھار_Type_t *pxPendYield );
خارجي فراغ v_پورٹ_Cancel_Thread( فراغ *px_Task_منزل_Delete );
#تعریف_کرو پورٹ_PRE_TASK_DELETE_HOOK( pv_Task_منزل_Delete, pxPendYield ) v_پورٹ_Thread_Dying( ( pv_Task_منزل_Delete ), ( pxPendYield ) )
#تعریف_کرو پورٹ_CLEAN_اتے_TCB( pxTCB )   v_پورٹ_Cancel_Thread( pxTCB )
/*-----------------------------------------------------------*/

#تعریف_کرو پورٹ_TASK_FUNCTION_PROTO( vFunction, pvParameters ) فراغ vFunction( فراغ *pvParameters )
#تعریف_کرو پورٹ_TASK_FUNCTION( vFunction, pvParameters ) فراغ vFunction( فراغ *pvParameters )
/*-----------------------------------------------------------*/

/*
 * Tasks run in their own pthreads and context switches between them
 * are always a full memory barrier. ISRs are emulated as signals
 * which also imply a full memory barrier.
 *
 * Thus, only a compiler barrier is needed to prevent the compiler
 * reordering.
 */
#تعریف_کرو پورٹ_حافظہ_BARRIER() __asm متطاير( "" ::: "memory" )

#تعریف_کرو پورٹ_CONFIGURE_TIMER_FOR_لگاتار_ویلا_STATS() /* no-op */
#إذا ( CONFIG_FREERTOS_GENERATE_لگاتار_ویلا_STATS )
config_لگاتار_ویلا_COUNTER_TYPE x_پورٹ_Get_لگاتار_ویلا_Counter_قدر( فراغ );
#تعریف_کرو پورٹ_GET_لگاتار_ویلا_COUNTER_قدر()         x_پورٹ_Get_لگاتار_ویلا_Counter_قدر()
#جے_انت

#جے_تعریف_شدہ __cplusplus
}
#جے_انت

// We need additional definitions for ESP-IDF code
#ضمن "portmacro_idf.h"

فراغ v_پورٹ_سیٹ_ڈھیر_Watchpoint(فراغ *px_ڈھیر_شروع);

#جے_انت /* PORTMACRO_H */
