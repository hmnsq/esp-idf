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


#که_نه_تعریف_شوی PORTMACRO_H
#تعریف_کړه PORTMACRO_H

#ضمن <limits.h>
#ضمن <stdint.h>
#ضمن "esp_macros.h"
#ضمن "spinlock.h"

#که_تعریف_شوی __cplusplus
خارجي "C" {
#د_که_پای

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come before first as they are used further down the file
 * ------------------------------------------------------------------------------------------------------------------ */

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

/* Task function macros as described on the FreeRTOS.org WEB site. */
#تعریف_کړه پورټ_TASK_FUNCTION_PROTO( vFunction, pvParameters ) فراغ vFunction( فراغ *pvParameters )
#تعریف_کړه پورټ_TASK_FUNCTION( vFunction, pvParameters ) فراغ vFunction( فراغ *pvParameters )
/*-----------------------------------------------------------*/

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#تعریف_کړه پورټ_CRITICAL_NESTING_دننه_TCB     1
#تعریف_کړه پورټ_ډېری_GROWTH                ( -1 )
#تعریف_کړه پورټ_HAS_ډېری_ډکېدنه_CHECKING ( 1 )   // Set in non-SMP POSIX port
#تعریف_کړه پورټ_TICK_PERIOD_MS              ( ( TickType_t ) 1000 / config_TICK_شرح_HZ )
#تعریف_کړه پورټ_TICK_شرح_MICROSECONDS      ( ( TickType_t ) 1000000 / config_TICK_شرح_HZ )
#تعریف_کړه پورټ_BYTE_ALIGNMENT              8
#تعریف_کړه پورټ_NOP() __asm متطاير        (" nop ")
/*-----------------------------------------------------------*/

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros need to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Spinlocks ------------------------

تعريفنوع spinlock_t                          پورټ_MUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */
/**< Spinlock initializer */
#تعریف_کړه پورټ_MUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#تعریف_کړه پورټ_MUX_FREE_VAL                    SPINLOCK_FREE           /**< Spinlock is free. [refactor-todo] check if this is still required */
#تعریف_کړه پورټ_MUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#تعریف_کړه پورټ_MUX_TRY_قلف                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#تعریف_کړه پورټ_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

بنسټ_Type_t x_پورټ_Check_If_دننه_ISR(فراغ);

// ------------------ Critical Sections --------------------

#إذا ( config_شمېره_د_CORES > 1 )
/*
These are always called with interrupts already disabled. We simply need to get/release the spinlocks
*/
خارجي پورټ_MUX_TYPE پورټ_x_Task_قلف;
خارجي پورټ_MUX_TYPE پورټ_x_ISRLock;

فراغ v_پورټ_Take_قلف( پورټ_MUX_TYPE *قلف );
فراغ v_پورټ_Release_قلف( پورټ_MUX_TYPE *قلف );
#د_که_پای /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

فراغ v_پورټ_Yield( فراغ );
خارجي فراغ v_پورټ_Yield_From_ISR( فراغ );

#تعریف_کړه پورټ_YIELD_FROM_ISR_CHECK(x)     ({ \
    إذا ( (x) == pdTRUE ) { \
        v_پورټ_Yield_From_ISR(); \
    } \
})
#تعریف_کړه پورټ_YIELD_FROM_ISR_NO_CHECK()               v_پورټ_Yield_From_ISR()

// ----------------------- System --------------------------

ساكن مضمن بنسټ_Type_t __attribute__((always_inline)) x_پورټ_Get_Core_ID( فراغ );

// ----------------------- TCB Cleanup --------------------------

فراغ v_پورټ_Clean_پورته_TCB ( فراغ *pxTCB );

/* ----------------------------------------- FreeRTOS SMP Porting Interface --------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS SMP
 * - Must come after forward declare as some porting macros map to declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

#تعریف_کړه پورټ_DISABLE_INTERRUPTS()                    x_پورټ_سېټ_مداخله_ماسک()
#تعریف_کړه پورټ_ENABLE_INTERRUPTS()                     v_پورټ_Clear_مداخله_ماسک(0)

// ------------------ Critical Sections --------------------

#تعریف_کړه پورټ_ENTER_CRITICAL_SMP()                    vTaskEnterCritical();
#تعریف_کړه پورټ_EXIT_CRITICAL_SMP()                     vTaskExitCritical();

#إذا defined(__cplusplus) && (__cplusplus >  201703L)
#تعریف_کړه پورټ_ENTER_CRITICAL(...)                     ټاکنه_MACRO_VA_ARG(پورټ_ENTER_CRITICAL_IDF, پورټ_ENTER_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#تعریف_کړه پورټ_EXIT_CRITICAL(...)                      ټاکنه_MACRO_VA_ARG(پورټ_EXIT_CRITICAL_IDF, پورټ_EXIT_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#وإلا
#تعریف_کړه پورټ_ENTER_CRITICAL(...)                     ټاکنه_MACRO_VA_ARG(پورټ_ENTER_CRITICAL_IDF, پورټ_ENTER_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#تعریف_کړه پورټ_EXIT_CRITICAL(...)                      ټاکنه_MACRO_VA_ARG(پورټ_EXIT_CRITICAL_IDF, پورټ_EXIT_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#د_که_پای

خارجي بنسټ_Type_t x_پورټ_سېټ_مداخله_ماسک( فراغ );
خارجي فراغ v_پورټ_Clear_مداخله_ماسک( بنسټ_Type_t x_ماسک );

#تعریف_کړه پورټ_سېټ_مداخله_ماسک_FROM_ISR() ({ \
    بنسټ_Type_t cur_level; \
    cur_level = x_پورټ_سېټ_مداخله_ماسک(); \
    vTaskEnterCritical(); \
    cur_level; \
})
#تعریف_کړه پورټ_CLEAR_مداخله_ماسک_FROM_ISR(x) ({ \
    vTaskExitCritical(); \
    v_پورټ_Clear_مداخله_ماسک(x); \
})

// ---------------------- Yielding -------------------------

#تعریف_کړه پورټ_YIELD()                                 v_پورټ_Yield()
#إذا defined(__cplusplus) && (__cplusplus >  201703L)
#تعریف_کړه پورټ_YIELD_FROM_ISR(...)                     ټاکنه_MACRO_VA_ARG(پورټ_YIELD_FROM_ISR_CHECK, پورټ_YIELD_FROM_ISR_NO_CHECK __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#وإلا
#تعریف_کړه پورټ_YIELD_FROM_ISR(...)                     ټاکنه_MACRO_VA_ARG(پورټ_YIELD_FROM_ISR_CHECK, پورټ_YIELD_FROM_ISR_NO_CHECK, ##__VA_ARGS__)(__VA_ARGS__)
#د_که_پای
// Note that the Linux port does not provide portYIELD_CORE(x) because purely single core for now.

// ----------------------- System --------------------------

#تعریف_کړه پورټ_GET_CORE_ID()                           x_پورټ_Get_Core_ID()
#تعریف_کړه پورټ_CHECK_IF_دننه_ISR()                       x_پورټ_Check_If_دننه_ISR()

// ------------------- Run Time Stats ----------------------

خارجي بینښه اوږد ul_پورټ_Get_پرلپسې_وخت( فراغ );
#تعریف_کړه پورټ_CONFIGURE_TIMER_FOR_پرلپسې_وخت_STATS() /* no-op */
#تعریف_کړه پورټ_GET_پرلپسې_وخت_COUNTER_ارزښت()         ul_پورټ_Get_پرلپسې_وخت()
// TODO IDF-5723: no esp_timer implementation

// ------------------- TCB Cleanup ----------------------

#تعریف_کړه پورټ_CLEAN_پورته_TCB( pxTCB )                   v_پورټ_Clean_پورته_TCB( pxTCB )

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
ساكن مضمن بنسټ_Type_t x_پورټ_Get_Core_ID(فراغ)
{
    أرجع (بنسټ_Type_t) 0; // POSIX simulator is only single-core at the moment
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

ساكن مضمن بنسټ_Type_t x_پورټ_دننه_Isr_Context(فراغ)
{
    //Just call the FreeRTOS port interface version
    أرجع x_پورټ_Check_If_دننه_ISR();
}

ساكن مضمن فراغ v_پورټ_Assert_If_دننه_ISR(فراغ)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(x_پورټ_دننه_Isr_Context() == 0);
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

فراغ v_پورټ_Enter_Critical_IDF(فراغ);
فراغ v_پورټ_Exit_Critical_IDF(فراغ);

// We ignore the lock and timeout argument on POSIX simulator since it is single core

//IDF task critical sections
#تعریف_کړه پورټ_TRY_ENTER_CRITICAL(قلف, timeout)       ({(فراغ) قلف; (فراغ) timeout; v_پورټ_Enter_Critical_IDF(); pdPASS;})
#تعریف_کړه پورټ_ENTER_CRITICAL_IDF(قلف)                ({(فراغ) قلف; v_پورټ_Enter_Critical_IDF();})
#تعریف_کړه پورټ_EXIT_CRITICAL_IDF(قلف)                 ({(فراغ) قلف; v_پورټ_Exit_Critical_IDF();})
//IDF ISR critical sections
#تعریف_کړه پورټ_TRY_ENTER_CRITICAL_ISR(قلف, timeout)   ({(فراغ) قلف; (فراغ) timeout; v_پورټ_Enter_Critical_IDF(); pdPASS;})
#تعریف_کړه پورټ_ENTER_CRITICAL_ISR(قلف)                ({(فراغ) قلف; v_پورټ_Enter_Critical_IDF();})
#تعریف_کړه پورټ_EXIT_CRITICAL_ISR(قلف)                 ({(فراغ) قلف; v_پورټ_Exit_Critical_IDF();})
//IDF safe critical sections (they're the same)
#تعریف_کړه پورټ_ENTER_CRITICAL_خوندي(قلف)               ({(فراغ) قلف; v_پورټ_Enter_Critical_IDF();})
#تعریف_کړه پورټ_EXIT_CRITICAL_خوندي(قلف)                ({(فراغ) قلف; v_پورټ_Exit_Critical_IDF();})

// ---------------------- Yielding -------------------------

خارجي فراغ v_پورټ_Yield( فراغ );
#تعریف_کړه پورټ_YIELD() v_پورټ_Yield()

// ----------------------- System --------------------------

bool پورټ_VALID_LIST_MEM(ثابت فراغ *ptr);
bool پورټ_VALID_TCB_MEM(ثابت فراغ *ptr);
bool پورټ_VALID_ډېری_MEM(ثابت فراغ *ptr);

/*-----------------------------------------------------------*/

/* POSIX-specific port functions */

خارجي فراغ v_پورټ_Thread_Dying( فراغ *px_Task_منزل_Delete, متطاير بنسټ_Type_t *pxPendYield );
خارجي فراغ v_پورټ_Cancel_Thread( فراغ *px_Task_منزل_Delete );
#تعریف_کړه پورټ_PRE_TASK_DELETE_HOOK( pv_Task_منزل_Delete, pxPendYield ) v_پورټ_Thread_Dying( ( pv_Task_منزل_Delete ), ( pxPendYield ) )
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

#که_تعریف_شوی __cplusplus
}
#د_که_پای

#د_که_پای /* PORTMACRO_H */
