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


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ PORTMACRO_H
#ವ್ಯಾಖ್ಯಾನಿಸು PORTMACRO_H

#ಅನ್ತರ್ಭಾವ <limits.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
ಬಾಹ್ಯ "C" {
#ಆದರೆ_ಅಂತ್ಯ

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
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_CHAR        ಅಕ್ಷರ
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_FLOAT       ದಶಮಾಂಶ
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_DOUBLE      ದ್ವಿಗುಣ
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_LONG        ದೀರ್ಘ
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_SHORT       ಗಿಡ್ಡ
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ರಾಶಿ_TYPE  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ತಳ_TYPE   ದೀರ್ಘ
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ಸೂಚಕ_ಗಾತ್ರ_TYPE intptr_t

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೋರ್ಟ್_ರಾಶಿ_TYPE ರಾಶಿ_Type_t;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೋರ್ಟ್_ತಳ_TYPE ತಳ_Type_t;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಪೋರ್ಟ್_ತಳ_TYPE UBaseType_t;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ TickType_t;
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_MAX_DELAY ( TickType_t ) ULONG_MAX

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TICK_TYPE_IS_ATOMIC 1

/*-----------------------------------------------------------*/

/* Architecture specifics. */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ರಾಶಿ_GROWTH            ( -1 )
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_HAS_ರಾಶಿ_ಅತಿಪ್ರವಾಹ_CHECKING ( 1 )
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TICK_PERIOD_MS          ( ( TickType_t ) 1000 / config_TICK_ದರ_HZ )
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TICK_ದರ_MICROSECONDS  ( ( TickType_t ) 1000000 / config_TICK_ದರ_HZ )
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_BYTE_ALIGNMENT          8
/*-----------------------------------------------------------*/

/* Scheduler utilities. */
ಬಾಹ್ಯ ಶೂನ್ಯ v_ಪೋರ್ಟ್_Yield_Within_Api( ಶೂನ್ಯ );
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_YIELD_WITHIN_API() v_ಪೋರ್ಟ್_Yield_Within_Api()

ಬಾಹ್ಯ ಶೂನ್ಯ v_ಪೋರ್ಟ್_Yield( ಶೂನ್ಯ );
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_YIELD() v_ಪೋರ್ಟ್_Yield()

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ಕೊನೆ_SWITCHING_ISR( xSwitchRequired ) ಆದರೆ( (xSwitchRequired) != pdFALSE ) v_ಪೋರ್ಟ್_Yield()
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_YIELD_FROM_ISR( x ) ಪೋರ್ಟ್_ಕೊನೆ_SWITCHING_ISR( x )
/*-----------------------------------------------------------*/

/* Critical section management. */
ಬಾಹ್ಯ ಶೂನ್ಯ v_ಪೋರ್ಟ್_Disable_Interrupts( ಶೂನ್ಯ );
ಬಾಹ್ಯ ಶೂನ್ಯ v_ಪೋರ್ಟ್_Enable_Interrupts( ಶೂನ್ಯ );
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ()        ( v_ಪೋರ್ಟ್_Disable_Interrupts() )
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_CLEAR_ಅಡಚಣೆ_ಮುಖವಾಡ()      ( v_ಪೋರ್ಟ್_Enable_Interrupts() )

ಬಾಹ್ಯ ತಳ_Type_t x_ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ( ಶೂನ್ಯ );
ಬಾಹ್ಯ ಶೂನ್ಯ v_ಪೋರ್ಟ್_Clear_ಅಡಚಣೆ_ಮುಖವಾಡ( ತಳ_Type_t x_ಮುಖವಾಡ );

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Enter_Critical( ಶೂನ್ಯ );
ಶೂನ್ಯ v_ಪೋರ್ಟ್_Exit_Critical( ಶೂನ್ಯ );
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR()       x_ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ()
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_CLEAR_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR(x)    v_ಪೋರ್ಟ್_Clear_ಅಡಚಣೆ_ಮುಖವಾಡ(x)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_DISABLE_INTERRUPTS()                ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ()
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ENABLE_INTERRUPTS()                 ಪೋರ್ಟ್_CLEAR_ಅಡಚಣೆ_ಮುಖವಾಡ()
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ENTER_CRITICAL(mux)                 {(ಶೂನ್ಯ)mux;  v_ಪೋರ್ಟ್_Enter_Critical();}
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_EXIT_CRITICAL(mux)                  {(ಶೂನ್ಯ)mux;  v_ಪೋರ್ಟ್_Exit_Critical();}
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ENTER_CRITICAL_ಸುರಕ್ಷಿತ(mux)            {(ಶೂನ್ಯ)mux;  v_ಪೋರ್ಟ್_Enter_Critical();}
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_EXIT_CRITICAL_ಸುರಕ್ಷಿತ(mux)             {(ಶೂನ್ಯ)mux;  v_ಪೋರ್ಟ್_Exit_Critical();}
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ENTER_CRITICAL_ISR(mux)             ಪೋರ್ಟ್_ENTER_CRITICAL(mux)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_EXIT_CRITICAL_ISR(mux)              ಪೋರ್ಟ್_EXIT_CRITICAL(mux)

#ವ್ಯಾಖ್ಯಾನಿಸು prvENTER_CRITICAL_SMP_ONLY( px_ಬೀಗ )         ಪೋರ್ಟ್_ENTER_CRITICAL( px_ಬೀಗ )
#ವ್ಯಾಖ್ಯಾನಿಸು prvEXIT_CRITICAL_SMP_ONLY( px_ಬೀಗ )          ಪೋರ್ಟ್_EXIT_CRITICAL( px_ಬೀಗ )

ಬಾಹ್ಯ ಶೂನ್ಯ v_ಪೋರ್ಟ್_Suspend_Scheduler(ಶೂನ್ಯ);
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_SOFTWARE_BARRIER() v_ಪೋರ್ಟ್_Suspend_Scheduler()

/*-----------------------------------------------------------*/

ಬಾಹ್ಯ ಶೂನ್ಯ v_ಪೋರ್ಟ್_Thread_Dying( ಶೂನ್ಯ *px_Task_ಗಮ್ಯ_Delete, ಅಸ್ಥಿರ ತಳ_Type_t *pxPendYield );
ಬಾಹ್ಯ ಶೂನ್ಯ v_ಪೋರ್ಟ್_Cancel_Thread( ಶೂನ್ಯ *px_Task_ಗಮ್ಯ_Delete );
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_PRE_TASK_DELETE_HOOK( pv_Task_ಗಮ್ಯ_Delete, pxPendYield ) v_ಪೋರ್ಟ್_Thread_Dying( ( pv_Task_ಗಮ್ಯ_Delete ), ( pxPendYield ) )
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_CLEAN_ಮೇಲೆ_TCB( pxTCB )   v_ಪೋರ್ಟ್_Cancel_Thread( pxTCB )
/*-----------------------------------------------------------*/

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TASK_FUNCTION_PROTO( vFunction, pvParameters ) ಶೂನ್ಯ vFunction( ಶೂನ್ಯ *pvParameters )
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TASK_FUNCTION( vFunction, pvParameters ) ಶೂನ್ಯ vFunction( ಶೂನ್ಯ *pvParameters )
/*-----------------------------------------------------------*/

/*
 * Tasks run in their own pthreads and context switches between them
 * are always a full memory barrier. ISRs are emulated as signals
 * which also imply a full memory barrier.
 *
 * Thus, only a compiler barrier is needed to prevent the compiler
 * reordering.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ಸ್ಮರಣೆ_BARRIER() __asm ಅಸ್ಥಿರ( "" ::: "memory" )

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_CONFIGURE_TIMER_FOR_ಸತತ_ಸಮಯ_STATS() /* no-op */
#ಆದರೆ ( CONFIG_FREERTOS_GENERATE_ಸತತ_ಸಮಯ_STATS )
config_ಸತತ_ಸಮಯ_COUNTER_TYPE x_ಪೋರ್ಟ್_Get_ಸತತ_ಸಮಯ_Counter_ಮೌಲ್ಯ( ಶೂನ್ಯ );
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_GET_ಸತತ_ಸಮಯ_COUNTER_ಮೌಲ್ಯ()         x_ಪೋರ್ಟ್_Get_ಸತತ_ಸಮಯ_Counter_ಮೌಲ್ಯ()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
}
#ಆದರೆ_ಅಂತ್ಯ

// We need additional definitions for ESP-IDF code
#ಅನ್ತರ್ಭಾವ "portmacro_idf.h"

ಶೂನ್ಯ v_ಪೋರ್ಟ್_ಗಣ_ರಾಶಿ_Watchpoint(ಶೂನ್ಯ *px_ರಾಶಿ_ಆರಂಭ);

#ಆದರೆ_ಅಂತ್ಯ /* PORTMACRO_H */
