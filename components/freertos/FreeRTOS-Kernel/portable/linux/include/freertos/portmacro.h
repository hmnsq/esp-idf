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


#اگر_تعریف_نشده PORTMACRO_H
#تعریف_کن PORTMACRO_H

#ضمن <limits.h>

#اگر_تعریف_شده __cplusplus
خارجي "C" {
#پایان_اگر

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
#تعریف_کن پورت_CHAR        حرف
#تعریف_کن پورت_FLOAT       عائم
#تعریف_کن پورت_DOUBLE      مزدوج
#تعریف_کن پورت_LONG        دراز
#تعریف_کن پورت_SHORT       کوتاه
#تعریف_کن پورت_پشته_TYPE  بیعلامت دراز
#تعریف_کن پورت_پایه_TYPE   دراز
#تعریف_کن پورت_اشاره_گر_اندازه_TYPE intptr_t

تعريفنوع پورت_پشته_TYPE پشته_Type_t;
تعريفنوع پورت_پایه_TYPE پایه_Type_t;
تعريفنوع بیعلامت پورت_پایه_TYPE UBaseType_t;

تعريفنوع بیعلامت دراز TickType_t;
#تعریف_کن پورت_MAX_DELAY ( TickType_t ) ULONG_MAX

#تعریف_کن پورت_TICK_TYPE_IS_ATOMIC 1

/*-----------------------------------------------------------*/

/* Architecture specifics. */
#تعریف_کن پورت_پشته_GROWTH            ( -1 )
#تعریف_کن پورت_HAS_پشته_سرریز_CHECKING ( 1 )
#تعریف_کن پورت_TICK_PERIOD_MS          ( ( TickType_t ) 1000 / config_TICK_نرخ_HZ )
#تعریف_کن پورت_TICK_نرخ_MICROSECONDS  ( ( TickType_t ) 1000000 / config_TICK_نرخ_HZ )
#تعریف_کن پورت_BYTE_ALIGNMENT          8
/*-----------------------------------------------------------*/

/* Scheduler utilities. */
خارجي فراغ v_پورت_Yield_Within_Api( فراغ );
#تعریف_کن پورت_YIELD_WITHIN_API() v_پورت_Yield_Within_Api()

خارجي فراغ v_پورت_Yield( فراغ );
#تعریف_کن پورت_YIELD() v_پورت_Yield()

#تعریف_کن پورت_انتها_SWITCHING_ISR( xSwitchRequired ) إذا( (xSwitchRequired) != pdFALSE ) v_پورت_Yield()
#تعریف_کن پورت_YIELD_FROM_ISR( x ) پورت_انتها_SWITCHING_ISR( x )
/*-----------------------------------------------------------*/

/* Critical section management. */
خارجي فراغ v_پورت_Disable_Interrupts( فراغ );
خارجي فراغ v_پورت_Enable_Interrupts( فراغ );
#تعریف_کن پورت_مجموعه_وقفه_ماسک()        ( v_پورت_Disable_Interrupts() )
#تعریف_کن پورت_CLEAR_وقفه_ماسک()      ( v_پورت_Enable_Interrupts() )

خارجي پایه_Type_t x_پورت_مجموعه_وقفه_ماسک( فراغ );
خارجي فراغ v_پورت_Clear_وقفه_ماسک( پایه_Type_t x_ماسک );

فراغ v_پورت_Enter_Critical( فراغ );
فراغ v_پورت_Exit_Critical( فراغ );
#تعریف_کن پورت_مجموعه_وقفه_ماسک_FROM_ISR()       x_پورت_مجموعه_وقفه_ماسک()
#تعریف_کن پورت_CLEAR_وقفه_ماسک_FROM_ISR(x)    v_پورت_Clear_وقفه_ماسک(x)
#تعریف_کن پورت_DISABLE_INTERRUPTS()                پورت_مجموعه_وقفه_ماسک()
#تعریف_کن پورت_ENABLE_INTERRUPTS()                 پورت_CLEAR_وقفه_ماسک()
#تعریف_کن پورت_ENTER_CRITICAL(mux)                 {(فراغ)mux;  v_پورت_Enter_Critical();}
#تعریف_کن پورت_EXIT_CRITICAL(mux)                  {(فراغ)mux;  v_پورت_Exit_Critical();}
#تعریف_کن پورت_ENTER_CRITICAL_امن(mux)            {(فراغ)mux;  v_پورت_Enter_Critical();}
#تعریف_کن پورت_EXIT_CRITICAL_امن(mux)             {(فراغ)mux;  v_پورت_Exit_Critical();}
#تعریف_کن پورت_ENTER_CRITICAL_ISR(mux)             پورت_ENTER_CRITICAL(mux)
#تعریف_کن پورت_EXIT_CRITICAL_ISR(mux)              پورت_EXIT_CRITICAL(mux)

#تعریف_کن prvENTER_CRITICAL_SMP_ONLY( px_قفل )         پورت_ENTER_CRITICAL( px_قفل )
#تعریف_کن prvEXIT_CRITICAL_SMP_ONLY( px_قفل )          پورت_EXIT_CRITICAL( px_قفل )

خارجي فراغ v_پورت_Suspend_Scheduler(فراغ);
#تعریف_کن پورت_SOFTWARE_BARRIER() v_پورت_Suspend_Scheduler()

/*-----------------------------------------------------------*/

خارجي فراغ v_پورت_Thread_Dying( فراغ *px_Task_مقصد_Delete, متطاير پایه_Type_t *pxPendYield );
خارجي فراغ v_پورت_Cancel_Thread( فراغ *px_Task_مقصد_Delete );
#تعریف_کن پورت_PRE_TASK_DELETE_HOOK( pv_Task_مقصد_Delete, pxPendYield ) v_پورت_Thread_Dying( ( pv_Task_مقصد_Delete ), ( pxPendYield ) )
#تعریف_کن پورت_CLEAN_به_بالا_TCB( pxTCB )   v_پورت_Cancel_Thread( pxTCB )
/*-----------------------------------------------------------*/

#تعریف_کن پورت_TASK_FUNCTION_PROTO( vFunction, pvParameters ) فراغ vFunction( فراغ *pvParameters )
#تعریف_کن پورت_TASK_FUNCTION( vFunction, pvParameters ) فراغ vFunction( فراغ *pvParameters )
/*-----------------------------------------------------------*/

/*
 * Tasks run in their own pthreads and context switches between them
 * are always a full memory barrier. ISRs are emulated as signals
 * which also imply a full memory barrier.
 *
 * Thus, only a compiler barrier is needed to prevent the compiler
 * reordering.
 */
#تعریف_کن پورت_حافظه_BARRIER() __asm متطاير( "" ::: "memory" )

#تعریف_کن پورت_CONFIGURE_TIMER_FOR_پیاپی_زمان_STATS() /* no-op */
#إذا ( CONFIG_FREERTOS_GENERATE_پیاپی_زمان_STATS )
config_پیاپی_زمان_COUNTER_TYPE x_پورت_Get_پیاپی_زمان_Counter_مقدار( فراغ );
#تعریف_کن پورت_GET_پیاپی_زمان_COUNTER_مقدار()         x_پورت_Get_پیاپی_زمان_Counter_مقدار()
#پایان_اگر

#اگر_تعریف_شده __cplusplus
}
#پایان_اگر

// We need additional definitions for ESP-IDF code
#ضمن "portmacro_idf.h"

فراغ v_پورت_مجموعه_پشته_Watchpoint(فراغ *px_پشته_شروع);

#پایان_اگر /* PORTMACRO_H */
