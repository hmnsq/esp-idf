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


#నిర్వచితం_కాకపోతే PORTMACRO_H
#నిర్వచించు PORTMACRO_H

#అన్తర్భావ <limits.h>

#నిర్వచితమైతే __cplusplus
బాహ్య "C" {
#అయితే_ముగింపు

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
#నిర్వచించు పోర్టు_CHAR        అక్షరం
#నిర్వచించు పోర్టు_FLOAT       దశాంశం
#నిర్వచించు పోర్టు_DOUBLE      ద్విగుణ
#నిర్వచించు పోర్టు_LONG        దీర్ఘ
#నిర్వచించు పోర్టు_SHORT       పొట్టి
#నిర్వచించు పోర్టు_దొంతర_TYPE  గుర్తులేని దీర్ఘ
#నిర్వచించు పోర్టు_పునాది_TYPE   దీర్ఘ
#నిర్వచించు పోర్టు_సూచిక_కొలత_TYPE intptr_t

రకనిర్వచనం పోర్టు_దొంతర_TYPE దొంతర_Type_t;
రకనిర్వచనం పోర్టు_పునాది_TYPE పునాది_Type_t;
రకనిర్వచనం గుర్తులేని పోర్టు_పునాది_TYPE UBaseType_t;

రకనిర్వచనం గుర్తులేని దీర్ఘ TickType_t;
#నిర్వచించు పోర్టు_MAX_DELAY ( TickType_t ) ULONG_MAX

#నిర్వచించు పోర్టు_TICK_TYPE_IS_ATOMIC 1

/*-----------------------------------------------------------*/

/* Architecture specifics. */
#నిర్వచించు పోర్టు_దొంతర_GROWTH            ( -1 )
#నిర్వచించు పోర్టు_HAS_దొంతర_అధికప్రవాహం_CHECKING ( 1 )
#నిర్వచించు పోర్టు_TICK_PERIOD_MS          ( ( TickType_t ) 1000 / config_TICK_రేటు_HZ )
#నిర్వచించు పోర్టు_TICK_రేటు_MICROSECONDS  ( ( TickType_t ) 1000000 / config_TICK_రేటు_HZ )
#నిర్వచించు పోర్టు_BYTE_ALIGNMENT          8
/*-----------------------------------------------------------*/

/* Scheduler utilities. */
బాహ్య శూన్య v_పోర్టు_Yield_Within_Api( శూన్య );
#నిర్వచించు పోర్టు_YIELD_WITHIN_API() v_పోర్టు_Yield_Within_Api()

బాహ్య శూన్య v_పోర్టు_Yield( శూన్య );
#నిర్వచించు పోర్టు_YIELD() v_పోర్టు_Yield()

#నిర్వచించు పోర్టు_చివర_SWITCHING_ISR( xSwitchRequired ) ఒకవేళ( (xSwitchRequired) != pdFALSE ) v_పోర్టు_Yield()
#నిర్వచించు పోర్టు_YIELD_FROM_ISR( x ) పోర్టు_చివర_SWITCHING_ISR( x )
/*-----------------------------------------------------------*/

/* Critical section management. */
బాహ్య శూన్య v_పోర్టు_Disable_Interrupts( శూన్య );
బాహ్య శూన్య v_పోర్టు_Enable_Interrupts( శూన్య );
#నిర్వచించు పోర్టు_సమితి_అంతరాయం_ముసుగు()        ( v_పోర్టు_Disable_Interrupts() )
#నిర్వచించు పోర్టు_CLEAR_అంతరాయం_ముసుగు()      ( v_పోర్టు_Enable_Interrupts() )

బాహ్య పునాది_Type_t x_పోర్టు_సమితి_అంతరాయం_ముసుగు( శూన్య );
బాహ్య శూన్య v_పోర్టు_Clear_అంతరాయం_ముసుగు( పునాది_Type_t x_ముసుగు );

శూన్య v_పోర్టు_Enter_Critical( శూన్య );
శూన్య v_పోర్టు_Exit_Critical( శూన్య );
#నిర్వచించు పోర్టు_సమితి_అంతరాయం_ముసుగు_FROM_ISR()       x_పోర్టు_సమితి_అంతరాయం_ముసుగు()
#నిర్వచించు పోర్టు_CLEAR_అంతరాయం_ముసుగు_FROM_ISR(x)    v_పోర్టు_Clear_అంతరాయం_ముసుగు(x)
#నిర్వచించు పోర్టు_DISABLE_INTERRUPTS()                పోర్టు_సమితి_అంతరాయం_ముసుగు()
#నిర్వచించు పోర్టు_ENABLE_INTERRUPTS()                 పోర్టు_CLEAR_అంతరాయం_ముసుగు()
#నిర్వచించు పోర్టు_ENTER_CRITICAL(mux)                 {(శూన్య)mux;  v_పోర్టు_Enter_Critical();}
#నిర్వచించు పోర్టు_EXIT_CRITICAL(mux)                  {(శూన్య)mux;  v_పోర్టు_Exit_Critical();}
#నిర్వచించు పోర్టు_ENTER_CRITICAL_సురక్షిత(mux)            {(శూన్య)mux;  v_పోర్టు_Enter_Critical();}
#నిర్వచించు పోర్టు_EXIT_CRITICAL_సురక్షిత(mux)             {(శూన్య)mux;  v_పోర్టు_Exit_Critical();}
#నిర్వచించు పోర్టు_ENTER_CRITICAL_ISR(mux)             పోర్టు_ENTER_CRITICAL(mux)
#నిర్వచించు పోర్టు_EXIT_CRITICAL_ISR(mux)              పోర్టు_EXIT_CRITICAL(mux)

#నిర్వచించు prvENTER_CRITICAL_SMP_ONLY( px_తాళం )         పోర్టు_ENTER_CRITICAL( px_తాళం )
#నిర్వచించు prvEXIT_CRITICAL_SMP_ONLY( px_తాళం )          పోర్టు_EXIT_CRITICAL( px_తాళం )

బాహ్య శూన్య v_పోర్టు_Suspend_Scheduler(శూన్య);
#నిర్వచించు పోర్టు_SOFTWARE_BARRIER() v_పోర్టు_Suspend_Scheduler()

/*-----------------------------------------------------------*/

బాహ్య శూన్య v_పోర్టు_Thread_Dying( శూన్య *px_Task_గమ్యం_Delete, అస్థిర పునాది_Type_t *pxPendYield );
బాహ్య శూన్య v_పోర్టు_Cancel_Thread( శూన్య *px_Task_గమ్యం_Delete );
#నిర్వచించు పోర్టు_PRE_TASK_DELETE_HOOK( pv_Task_గమ్యం_Delete, pxPendYield ) v_పోర్టు_Thread_Dying( ( pv_Task_గమ్యం_Delete ), ( pxPendYield ) )
#నిర్వచించు పోర్టు_CLEAN_పైకి_TCB( pxTCB )   v_పోర్టు_Cancel_Thread( pxTCB )
/*-----------------------------------------------------------*/

#నిర్వచించు పోర్టు_TASK_FUNCTION_PROTO( vFunction, pvParameters ) శూన్య vFunction( శూన్య *pvParameters )
#నిర్వచించు పోర్టు_TASK_FUNCTION( vFunction, pvParameters ) శూన్య vFunction( శూన్య *pvParameters )
/*-----------------------------------------------------------*/

/*
 * Tasks run in their own pthreads and context switches between them
 * are always a full memory barrier. ISRs are emulated as signals
 * which also imply a full memory barrier.
 *
 * Thus, only a compiler barrier is needed to prevent the compiler
 * reordering.
 */
#నిర్వచించు పోర్టు_జ్ఞాపకం_BARRIER() __asm అస్థిర( "" ::: "memory" )

#నిర్వచించు పోర్టు_CONFIGURE_TIMER_FOR_నిరంతరం_సమయం_STATS() /* no-op */
#ఒకవేళ ( CONFIG_FREERTOS_GENERATE_నిరంతరం_సమయం_STATS )
config_నిరంతరం_సమయం_COUNTER_TYPE x_పోర్టు_Get_నిరంతరం_సమయం_Counter_విలువ( శూన్య );
#నిర్వచించు పోర్టు_GET_నిరంతరం_సమయం_COUNTER_విలువ()         x_పోర్టు_Get_నిరంతరం_సమయం_Counter_విలువ()
#అయితే_ముగింపు

#నిర్వచితమైతే __cplusplus
}
#అయితే_ముగింపు

// We need additional definitions for ESP-IDF code
#అన్తర్భావ "portmacro_idf.h"

శూన్య v_పోర్టు_సమితి_దొంతర_Watchpoint(శూన్య *px_దొంతర_ప్రారంభం);

#అయితే_ముగింపు /* PORTMACRO_H */
