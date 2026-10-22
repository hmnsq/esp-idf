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


#که_نه_تعریف_شوی PORTMACRO_H
#تعریف_کړه PORTMACRO_H

#ضمن <limits.h>

#که_تعریف_شوی __cplusplus
خارجي "C" {
#د_که_پای

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
#تعریف_کړه پورټ_CHAR        حرف
#تعریف_کړه پورټ_FLOAT       عائم
#تعریف_کړه پورټ_DOUBLE      مزدوج
#تعریف_کړه پورټ_LONG        اوږد
#تعریف_کړه پورټ_SHORT       لنډ
#تعریف_کړه پورټ_ډېری_TYPE  بینښه اوږد
#تعریف_کړه پورټ_بنسټ_TYPE   اوږد
#تعریف_کړه پورټ_اشاره_ګر_اندازه_TYPE intptr_t

تعريفنوع پورټ_ډېری_TYPE ډېری_Type_t;
تعريفنوع پورټ_بنسټ_TYPE بنسټ_Type_t;
تعريفنوع بینښه پورټ_بنسټ_TYPE UBaseType_t;

تعريفنوع بینښه اوږد TickType_t;
#تعریف_کړه پورټ_MAX_DELAY ( TickType_t ) ULONG_MAX

#تعریف_کړه پورټ_TICK_TYPE_IS_ATOMIC 1

/*-----------------------------------------------------------*/

/* Architecture specifics. */
#تعریف_کړه پورټ_ډېری_GROWTH            ( -1 )
#تعریف_کړه پورټ_HAS_ډېری_ډکېدنه_CHECKING ( 1 )
#تعریف_کړه پورټ_TICK_PERIOD_MS          ( ( TickType_t ) 1000 / config_TICK_شرح_HZ )
#تعریف_کړه پورټ_TICK_شرح_MICROSECONDS  ( ( TickType_t ) 1000000 / config_TICK_شرح_HZ )
#تعریف_کړه پورټ_BYTE_ALIGNMENT          8
/*-----------------------------------------------------------*/

/* Scheduler utilities. */
خارجي فراغ v_پورټ_Yield_Within_Api( فراغ );
#تعریف_کړه پورټ_YIELD_WITHIN_API() v_پورټ_Yield_Within_Api()

خارجي فراغ v_پورټ_Yield( فراغ );
#تعریف_کړه پورټ_YIELD() v_پورټ_Yield()

#تعریف_کړه پورټ_پای_ټکی_SWITCHING_ISR( xSwitchRequired ) إذا( (xSwitchRequired) != pdFALSE ) v_پورټ_Yield()
#تعریف_کړه پورټ_YIELD_FROM_ISR( x ) پورټ_پای_ټکی_SWITCHING_ISR( x )
/*-----------------------------------------------------------*/

/* Critical section management. */
خارجي فراغ v_پورټ_Disable_Interrupts( فراغ );
خارجي فراغ v_پورټ_Enable_Interrupts( فراغ );
#تعریف_کړه پورټ_سېټ_مداخله_ماسک()        ( v_پورټ_Disable_Interrupts() )
#تعریف_کړه پورټ_CLEAR_مداخله_ماسک()      ( v_پورټ_Enable_Interrupts() )

خارجي بنسټ_Type_t x_پورټ_سېټ_مداخله_ماسک( فراغ );
خارجي فراغ v_پورټ_Clear_مداخله_ماسک( بنسټ_Type_t x_ماسک );

فراغ v_پورټ_Enter_Critical( فراغ );
فراغ v_پورټ_Exit_Critical( فراغ );
#تعریف_کړه پورټ_سېټ_مداخله_ماسک_FROM_ISR()       x_پورټ_سېټ_مداخله_ماسک()
#تعریف_کړه پورټ_CLEAR_مداخله_ماسک_FROM_ISR(x)    v_پورټ_Clear_مداخله_ماسک(x)
#تعریف_کړه پورټ_DISABLE_INTERRUPTS()                پورټ_سېټ_مداخله_ماسک()
#تعریف_کړه پورټ_ENABLE_INTERRUPTS()                 پورټ_CLEAR_مداخله_ماسک()
#تعریف_کړه پورټ_ENTER_CRITICAL(mux)                 {(فراغ)mux;  v_پورټ_Enter_Critical();}
#تعریف_کړه پورټ_EXIT_CRITICAL(mux)                  {(فراغ)mux;  v_پورټ_Exit_Critical();}
#تعریف_کړه پورټ_ENTER_CRITICAL_خوندي(mux)            {(فراغ)mux;  v_پورټ_Enter_Critical();}
#تعریف_کړه پورټ_EXIT_CRITICAL_خوندي(mux)             {(فراغ)mux;  v_پورټ_Exit_Critical();}
#تعریف_کړه پورټ_ENTER_CRITICAL_ISR(mux)             پورټ_ENTER_CRITICAL(mux)
#تعریف_کړه پورټ_EXIT_CRITICAL_ISR(mux)              پورټ_EXIT_CRITICAL(mux)

#تعریف_کړه prvENTER_CRITICAL_SMP_ONLY( px_قلف )         پورټ_ENTER_CRITICAL( px_قلف )
#تعریف_کړه prvEXIT_CRITICAL_SMP_ONLY( px_قلف )          پورټ_EXIT_CRITICAL( px_قلف )

خارجي فراغ v_پورټ_Suspend_Scheduler(فراغ);
#تعریف_کړه پورټ_SOFTWARE_BARRIER() v_پورټ_Suspend_Scheduler()

/*-----------------------------------------------------------*/

خارجي فراغ v_پورټ_Thread_Dying( فراغ *px_Task_منزل_Delete, متطاير بنسټ_Type_t *pxPendYield );
خارجي فراغ v_پورټ_Cancel_Thread( فراغ *px_Task_منزل_Delete );
#تعریف_کړه پورټ_PRE_TASK_DELETE_HOOK( pv_Task_منزل_Delete, pxPendYield ) v_پورټ_Thread_Dying( ( pv_Task_منزل_Delete ), ( pxPendYield ) )
#تعریف_کړه پورټ_CLEAN_پورته_TCB( pxTCB )   v_پورټ_Cancel_Thread( pxTCB )
/*-----------------------------------------------------------*/

#تعریف_کړه پورټ_TASK_FUNCTION_PROTO( vFunction, pvParameters ) فراغ vFunction( فراغ *pvParameters )
#تعریف_کړه پورټ_TASK_FUNCTION( vFunction, pvParameters ) فراغ vFunction( فراغ *pvParameters )
/*-----------------------------------------------------------*/

/*
 * Tasks run in their own pthreads and context switches between them
 * are always a full memory barrier. ISRs are emulated as signals
 * which also imply a full memory barrier.
 *
 * Thus, only a compiler barrier is needed to prevent the compiler
 * reordering.
 */
#تعریف_کړه پورټ_حافظه_BARRIER() __asm متطاير( "" ::: "memory" )

#تعریف_کړه پورټ_CONFIGURE_TIMER_FOR_پرلپسې_وخت_STATS() /* no-op */
#إذا ( CONFIG_FREERTOS_GENERATE_پرلپسې_وخت_STATS )
config_پرلپسې_وخت_COUNTER_TYPE x_پورټ_Get_پرلپسې_وخت_Counter_ارزښت( فراغ );
#تعریف_کړه پورټ_GET_پرلپسې_وخت_COUNTER_ارزښت()         x_پورټ_Get_پرلپسې_وخت_Counter_ارزښت()
#د_که_پای

#که_تعریف_شوی __cplusplus
}
#د_که_پای

// We need additional definitions for ESP-IDF code
#ضمن "portmacro_idf.h"

فراغ v_پورټ_سېټ_ډېری_Watchpoint(فراغ *px_ډېری_پیل);

#د_که_پای /* PORTMACRO_H */
