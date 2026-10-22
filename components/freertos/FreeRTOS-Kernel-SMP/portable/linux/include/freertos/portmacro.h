/*
 * FreeRTOS Kernel V10.4.3
 * Copyright 2020 Cambridge Consultants Ltd.
 *
 * SPDX-FileCopyrightText: 2020 Cambridge Consultants Ltd.
 *
 * SPDX-License-Identifier: MIT
 *
 * SPDX-FileContributor: 2023-2024 Espressif Systems (Shanghai) CO LTD
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


#اگر_غیر_تعریف_شدہ PORTMACRO_H
#تعریف_کرو PORTMACRO_H

#شامل <limits.h>
#شامل <stdint.h>
#شامل "esp_macros.h"
#شامل "spinlock.h"

#اگر_تعریف_شدہ __cplusplus
خارجی "C" {
#اگر_ختم

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come before first as they are used further down the file
 * ------------------------------------------------------------------------------------------------------------------ */

/* Type definitions. */
#تعریف_کرو پورٹ_CHAR        حرف
#تعریف_کرو پورٹ_FLOAT       اعشاری
#تعریف_کرو پورٹ_DOUBLE      دوہرا
#تعریف_کرو پورٹ_LONG        طویل
#تعریف_کرو پورٹ_SHORT       مختصر
#تعریف_کرو پورٹ_ڈھیر_TYPE  غیرعلامتی طویل
#تعریف_کرو پورٹ_بنیاد_TYPE   طویل
#تعریف_کرو پورٹ_اشارہ_گر_سائز_TYPE intptr_t

قسمتعریف پورٹ_ڈھیر_TYPE ڈھیر_Type_t;
قسمتعریف پورٹ_بنیاد_TYPE بنیاد_Type_t;
قسمتعریف غیرعلامتی پورٹ_بنیاد_TYPE UBaseType_t;

قسمتعریف غیرعلامتی طویل TickType_t;
#تعریف_کرو پورٹ_MAX_DELAY ( TickType_t ) ULONG_MAX

#تعریف_کرو پورٹ_TICK_TYPE_IS_ATOMIC 1

/* Task function macros as described on the FreeRTOS.org WEB site. */
#تعریف_کرو پورٹ_TASK_FUNCTION_PROTO( vFunction, pvParameters ) خالی vFunction( خالی *pvParameters )
#تعریف_کرو پورٹ_TASK_FUNCTION( vFunction, pvParameters ) خالی vFunction( خالی *pvParameters )
/*-----------------------------------------------------------*/

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#تعریف_کرو پورٹ_CRITICAL_NESTING_اندر_TCB     1
#تعریف_کرو پورٹ_ڈھیر_GROWTH                ( -1 )
#تعریف_کرو پورٹ_HAS_ڈھیر_حد_سے_تجاوز_CHECKING ( 1 )   // Set in non-SMP POSIX port
#تعریف_کرو پورٹ_TICK_PERIOD_MS              ( ( TickType_t ) 1000 / config_TICK_شرح_HZ )
#تعریف_کرو پورٹ_TICK_شرح_MICROSECONDS      ( ( TickType_t ) 1000000 / config_TICK_شرح_HZ )
#تعریف_کرو پورٹ_BYTE_ALIGNMENT              8
#تعریف_کرو پورٹ_NOP() __asm متلون        (" nop ")
/*-----------------------------------------------------------*/

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros need to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Spinlocks ------------------------

قسمتعریف spinlock_t                          پورٹ_MUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */
/**< Spinlock initializer */
#تعریف_کرو پورٹ_MUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#تعریف_کرو پورٹ_MUX_FREE_VAL                    SPINLOCK_FREE           /**< Spinlock is free. [refactor-todo] check if this is still required */
#تعریف_کرو پورٹ_MUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#تعریف_کرو پورٹ_MUX_TRY_تالا                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#تعریف_کرو پورٹ_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

بنیاد_Type_t x_پورٹ_Check_If_اندر_ISR(خالی);

// ------------------ Critical Sections --------------------

#اگر ( config_نمبر_کا_CORES > 1 )
/*
These are always called with interrupts already disabled. We simply need to get/release the spinlocks
*/
خارجی پورٹ_MUX_TYPE پورٹ_x_Task_تالا;
خارجی پورٹ_MUX_TYPE پورٹ_x_ISRLock;

خالی v_پورٹ_Take_تالا( پورٹ_MUX_TYPE *تالا_2 );
خالی v_پورٹ_Release_تالا( پورٹ_MUX_TYPE *تالا_2 );
#اگر_ختم /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

خالی v_پورٹ_Yield( خالی );
خارجی خالی v_پورٹ_Yield_From_ISR( خالی );

#تعریف_کرو پورٹ_YIELD_FROM_ISR_CHECK(x)     ({ \
    اگر ( (x) == pdTRUE ) { \
        v_پورٹ_Yield_From_ISR(); \
    } \
})
#تعریف_کرو پورٹ_YIELD_FROM_ISR_NO_CHECK()               v_پورٹ_Yield_From_ISR()

// ----------------------- System --------------------------

ساکن اندرونی بنیاد_Type_t __attribute__((always_inline)) x_پورٹ_Get_Core_ID( خالی );

// ----------------------- TCB Cleanup --------------------------

خالی v_پورٹ_Clean_اوپر_TCB ( خالی *pxTCB );

/* ----------------------------------------- FreeRTOS SMP Porting Interface --------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS SMP
 * - Must come after forward declare as some porting macros map to declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

#تعریف_کرو پورٹ_DISABLE_INTERRUPTS()                    x_پورٹ_سیٹ_مداخلت_نقاب()
#تعریف_کرو پورٹ_ENABLE_INTERRUPTS()                     v_پورٹ_Clear_مداخلت_نقاب(0)

// ------------------ Critical Sections --------------------

#تعریف_کرو پورٹ_ENTER_CRITICAL_SMP()                    vTaskEnterCritical();
#تعریف_کرو پورٹ_EXIT_CRITICAL_SMP()                     vTaskExitCritical();

#اگر defined(__cplusplus) && (__cplusplus >  201703L)
#تعریف_کرو پورٹ_ENTER_CRITICAL(...)                     انتخاب_MACRO_VA_ARG(پورٹ_ENTER_CRITICAL_IDF, پورٹ_ENTER_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#تعریف_کرو پورٹ_EXIT_CRITICAL(...)                      انتخاب_MACRO_VA_ARG(پورٹ_EXIT_CRITICAL_IDF, پورٹ_EXIT_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#ورنہ
#تعریف_کرو پورٹ_ENTER_CRITICAL(...)                     انتخاب_MACRO_VA_ARG(پورٹ_ENTER_CRITICAL_IDF, پورٹ_ENTER_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#تعریف_کرو پورٹ_EXIT_CRITICAL(...)                      انتخاب_MACRO_VA_ARG(پورٹ_EXIT_CRITICAL_IDF, پورٹ_EXIT_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#اگر_ختم

خارجی بنیاد_Type_t x_پورٹ_سیٹ_مداخلت_نقاب( خالی );
خارجی خالی v_پورٹ_Clear_مداخلت_نقاب( بنیاد_Type_t x_نقاب );

#تعریف_کرو پورٹ_سیٹ_مداخلت_نقاب_FROM_ISR() ({ \
    بنیاد_Type_t cur_level; \
    cur_level = x_پورٹ_سیٹ_مداخلت_نقاب(); \
    vTaskEnterCritical(); \
    cur_level; \
})
#تعریف_کرو پورٹ_CLEAR_مداخلت_نقاب_FROM_ISR(x) ({ \
    vTaskExitCritical(); \
    v_پورٹ_Clear_مداخلت_نقاب(x); \
})

// ---------------------- Yielding -------------------------

#تعریف_کرو پورٹ_YIELD()                                 v_پورٹ_Yield()
#اگر defined(__cplusplus) && (__cplusplus >  201703L)
#تعریف_کرو پورٹ_YIELD_FROM_ISR(...)                     انتخاب_MACRO_VA_ARG(پورٹ_YIELD_FROM_ISR_CHECK, پورٹ_YIELD_FROM_ISR_NO_CHECK __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#ورنہ
#تعریف_کرو پورٹ_YIELD_FROM_ISR(...)                     انتخاب_MACRO_VA_ARG(پورٹ_YIELD_FROM_ISR_CHECK, پورٹ_YIELD_FROM_ISR_NO_CHECK, ##__VA_ARGS__)(__VA_ARGS__)
#اگر_ختم
// Note that the Linux port does not provide portYIELD_CORE(x) because purely single core for now.

// ----------------------- System --------------------------

#تعریف_کرو پورٹ_GET_CORE_ID()                           x_پورٹ_Get_Core_ID()
#تعریف_کرو پورٹ_CHECK_IF_اندر_ISR()                       x_پورٹ_Check_If_اندر_ISR()

// ------------------- Run Time Stats ----------------------

خارجی غیرعلامتی طویل ul_پورٹ_Get_تسلسل_وقت( خالی );
#تعریف_کرو پورٹ_CONFIGURE_TIMER_FOR_تسلسل_وقت_STATS() /* no-op */
#تعریف_کرو پورٹ_GET_تسلسل_وقت_شمار_قدر()         ul_پورٹ_Get_تسلسل_وقت()
// TODO IDF-5723: no esp_timer implementation

// ------------------- TCB Cleanup ----------------------

#تعریف_کرو پورٹ_CLEAN_اوپر_TCB( pxTCB )                   v_پورٹ_Clean_اوپر_TCB( pxTCB )

/* --------------------------------------------- Inline Implementations ------------------------------------------------
 * - Implementation of inline functions of the forward declares
 * - Should come after forward declare and FreeRTOS Porting interface, as implementation may use both.
 * - For implementation of non-inlined functions, see port.c
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

// ------------------ Critical Sections --------------------

// ---------------------- Yielding -------------------------

// NOTE: vPortYieldCore( BaseType_t xCoreID ) is currently not implemented as the simulator is single core

// ----------------------- System --------------------------
/**
 * @brief Get the current core's ID
 *
 * @note dummy function for freertos simulator, always returns 0.
 @ return BaseType_t 0
 */
ساکن اندرونی بنیاد_Type_t x_پورٹ_Get_Core_ID(خالی)
{
    واپس (بنیاد_Type_t) 0; // POSIX simulator is only single-core at the moment
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

ساکن اندرونی بنیاد_Type_t x_پورٹ_اندر_Isr_Context(خالی)
{
    //Just call the FreeRTOS port interface version
    واپس x_پورٹ_Check_If_اندر_ISR();
}

ساکن اندرونی خالی v_پورٹ_Assert_If_اندر_ISR(خالی)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(x_پورٹ_اندر_Isr_Context() == 0);
}

// xPortInterruptedFromISRContext() is only used in panic handler and core dump,
// both probably not relevant on POSIX sim.
//BaseType_t xPortInterruptedFromISRContext(void);

// Following functions seem to not be used in SMP:
//static inline UBaseType_t xPortSetInterruptMaskFromISR(void)
//static inline void vPortClearInterruptMaskFromISR(UBaseType_t prev_level)

// Following functions should have been removed according to
// docs/en/migration-guides/release-5.x/5.0/system.rst
//static inline bool __attribute__((always_inline)) vPortCPUAcquireMutexTimeout(portMUX_TYPE *mux, int timeout)
//static inline void __attribute__((always_inline)) vPortCPUReleaseMutex(portMUX_TYPE *mux)

// ------------------ Critical Sections --------------------

خالی v_پورٹ_Enter_Critical_IDF(خالی);
خالی v_پورٹ_Exit_Critical_IDF(خالی);

// We ignore the lock and timeout argument on POSIX simulator since it is single core

//IDF task critical sections
#تعریف_کرو پورٹ_TRY_ENTER_CRITICAL(تالا_2, timeout)       ({(خالی) تالا_2; (خالی) timeout; v_پورٹ_Enter_Critical_IDF(); pdPASS;})
#تعریف_کرو پورٹ_ENTER_CRITICAL_IDF(تالا_2)                ({(خالی) تالا_2; v_پورٹ_Enter_Critical_IDF();})
#تعریف_کرو پورٹ_EXIT_CRITICAL_IDF(تالا_2)                 ({(خالی) تالا_2; v_پورٹ_Exit_Critical_IDF();})
//IDF ISR critical sections
#تعریف_کرو پورٹ_TRY_ENTER_CRITICAL_ISR(تالا_2, timeout)   ({(خالی) تالا_2; (خالی) timeout; v_پورٹ_Enter_Critical_IDF(); pdPASS;})
#تعریف_کرو پورٹ_ENTER_CRITICAL_ISR(تالا_2)                ({(خالی) تالا_2; v_پورٹ_Enter_Critical_IDF();})
#تعریف_کرو پورٹ_EXIT_CRITICAL_ISR(تالا_2)                 ({(خالی) تالا_2; v_پورٹ_Exit_Critical_IDF();})
//IDF safe critical sections (they're the same)
#تعریف_کرو پورٹ_ENTER_CRITICAL_محفوظ(تالا_2)               ({(خالی) تالا_2; v_پورٹ_Enter_Critical_IDF();})
#تعریف_کرو پورٹ_EXIT_CRITICAL_محفوظ(تالا_2)                ({(خالی) تالا_2; v_پورٹ_Exit_Critical_IDF();})

// ---------------------- Yielding -------------------------

خارجی خالی v_پورٹ_Yield( خالی );
#تعریف_کرو پورٹ_YIELD() v_پورٹ_Yield()

// ----------------------- System --------------------------

bool پورٹ_VALID_LIST_MEM(ثابت خالی *ptr);
bool پورٹ_VALID_TCB_MEM(ثابت خالی *ptr);
bool پورٹ_VALID_ڈھیر_MEM(ثابت خالی *ptr);

/*-----------------------------------------------------------*/

/* POSIX-specific port functions */

خارجی خالی v_پورٹ_Thread_Dying( خالی *px_Task_مقصد_Delete, متلون بنیاد_Type_t *pxPendYield );
خارجی خالی v_پورٹ_Cancel_Thread( خالی *px_Task_مقصد_Delete );
#تعریف_کرو پورٹ_PRE_TASK_DELETE_HOOK( pv_Task_مقصد_Delete, pxPendYield ) v_پورٹ_Thread_Dying( ( pv_Task_مقصد_Delete ), ( pxPendYield ) )
/*-----------------------------------------------------------*/

/*
 * Tasks run in their own pthreads and context switches between them
 * are always a full memory barrier. ISRs are emulated as signals
 * which also imply a full memory barrier.
 *
 * Thus, only a compiler barrier is needed to prevent the compiler
 * reordering.
 */
#تعریف_کرو پورٹ_حافظہ_BARRIER() __asm متلون( "" ::: "memory" )

#اگر_تعریف_شدہ __cplusplus
}
#اگر_ختم

#اگر_ختم /* PORTMACRO_H */
