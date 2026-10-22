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


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ PORTMACRO_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PORTMACRO_H

#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_macros.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "spinlock.h"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
ਬਾਹਰੀ "C" {
#ਜੇ_ਅੰਤ

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come before first as they are used further down the file
 * ------------------------------------------------------------------------------------------------------------------ */

/* Type definitions. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_CHAR        ਅੱਖਰ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_FLOAT       ਦਸ਼ਮਲਵ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_DOUBLE      ਦੁੱਗਣਾ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_LONG        ਲੰਮਾ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_SHORT       ਛੋਟਾ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਢੇਰ_TYPE  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਅਧਾਰ_TYPE   ਲੰਮਾ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਸੂਚਕ_ਮਾਪ_TYPE intptr_t

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੋਰਟ_ਢੇਰ_TYPE ਢੇਰ_Type_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੋਰਟ_ਅਧਾਰ_TYPE ਅਧਾਰ_Type_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੋਰਟ_ਅਧਾਰ_TYPE UBaseType_t;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ TickType_t;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_MAX_DELAY ( TickType_t ) ULONG_MAX

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TICK_TYPE_IS_ATOMIC 1

/* Task function macros as described on the FreeRTOS.org WEB site. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TASK_FUNCTION_PROTO( vFunction, pvParameters ) ਖਾਲੀ vFunction( ਖਾਲੀ *pvParameters )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TASK_FUNCTION( vFunction, pvParameters ) ਖਾਲੀ vFunction( ਖਾਲੀ *pvParameters )
/*-----------------------------------------------------------*/

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_CRITICAL_NESTING_ਵਿੱਚ_TCB     1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਢੇਰ_GROWTH                ( -1 )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_HAS_ਢੇਰ_ਉਛਾਲ_CHECKING ( 1 )   // Set in non-SMP POSIX port
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TICK_PERIOD_MS              ( ( TickType_t ) 1000 / config_TICK_ਦਰ_HZ )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TICK_ਦਰ_MICROSECONDS      ( ( TickType_t ) 1000000 / config_TICK_ਦਰ_HZ )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_BYTE_ALIGNMENT              8
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_NOP() __asm ਅਸਥਿਰ        (" nop ")
/*-----------------------------------------------------------*/

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros need to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Spinlocks ------------------------

ਕਿਸਮਪਰਿਭਾਸ਼ਾ spinlock_t                          ਪੋਰਟ_MUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */
/**< Spinlock initializer */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_MUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_MUX_FREE_VAL                    SPINLOCK_FREE           /**< Spinlock is free. [refactor-todo] check if this is still required */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_MUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_MUX_TRY_ਤਾਲਾ                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

ਅਧਾਰ_Type_t x_ਪੋਰਟ_Check_If_ਵਿੱਚ_ISR(ਖਾਲੀ);

// ------------------ Critical Sections --------------------

#ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 )
/*
These are always called with interrupts already disabled. We simply need to get/release the spinlocks
*/
ਬਾਹਰੀ ਪੋਰਟ_MUX_TYPE ਪੋਰਟ_x_Task_ਤਾਲਾ;
ਬਾਹਰੀ ਪੋਰਟ_MUX_TYPE ਪੋਰਟ_x_ISRLock;

ਖਾਲੀ v_ਪੋਰਟ_Take_ਤਾਲਾ( ਪੋਰਟ_MUX_TYPE *ਤਾਲਾ );
ਖਾਲੀ v_ਪੋਰਟ_Release_ਤਾਲਾ( ਪੋਰਟ_MUX_TYPE *ਤਾਲਾ );
#ਜੇ_ਅੰਤ /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

ਖਾਲੀ v_ਪੋਰਟ_Yield( ਖਾਲੀ );
ਬਾਹਰੀ ਖਾਲੀ v_ਪੋਰਟ_Yield_From_ISR( ਖਾਲੀ );

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_YIELD_FROM_ISR_CHECK(x)     ({ \
    ਜੇ ( (x) == pdTRUE ) { \
        v_ਪੋਰਟ_Yield_From_ISR(); \
    } \
})
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_YIELD_FROM_ISR_NO_CHECK()               v_ਪੋਰਟ_Yield_From_ISR()

// ----------------------- System --------------------------

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਧਾਰ_Type_t __attribute__((always_inline)) x_ਪੋਰਟ_Get_Core_ID( ਖਾਲੀ );

// ----------------------- TCB Cleanup --------------------------

ਖਾਲੀ v_ਪੋਰਟ_Clean_ਉੱਪਰ_TCB ( ਖਾਲੀ *pxTCB );

/* ----------------------------------------- FreeRTOS SMP Porting Interface --------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS SMP
 * - Must come after forward declare as some porting macros map to declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_DISABLE_INTERRUPTS()                    x_ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ()
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENABLE_INTERRUPTS()                     v_ਪੋਰਟ_Clear_ਰੁਕਾਵਟ_ਨਕਾਬ(0)

// ------------------ Critical Sections --------------------

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENTER_CRITICAL_SMP()                    vTaskEnterCritical();
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_EXIT_CRITICAL_SMP()                     vTaskExitCritical();

#ਜੇ defined(__cplusplus) && (__cplusplus >  201703L)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENTER_CRITICAL(...)                     ਚੁਣੋ_MACRO_VA_ARG(ਪੋਰਟ_ENTER_CRITICAL_IDF, ਪੋਰਟ_ENTER_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_EXIT_CRITICAL(...)                      ਚੁਣੋ_MACRO_VA_ARG(ਪੋਰਟ_EXIT_CRITICAL_IDF, ਪੋਰਟ_EXIT_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENTER_CRITICAL(...)                     ਚੁਣੋ_MACRO_VA_ARG(ਪੋਰਟ_ENTER_CRITICAL_IDF, ਪੋਰਟ_ENTER_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_EXIT_CRITICAL(...)                      ਚੁਣੋ_MACRO_VA_ARG(ਪੋਰਟ_EXIT_CRITICAL_IDF, ਪੋਰਟ_EXIT_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#ਜੇ_ਅੰਤ

ਬਾਹਰੀ ਅਧਾਰ_Type_t x_ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ( ਖਾਲੀ );
ਬਾਹਰੀ ਖਾਲੀ v_ਪੋਰਟ_Clear_ਰੁਕਾਵਟ_ਨਕਾਬ( ਅਧਾਰ_Type_t x_ਨਕਾਬ );

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR() ({ \
    ਅਧਾਰ_Type_t cur_level; \
    cur_level = x_ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ(); \
    vTaskEnterCritical(); \
    cur_level; \
})
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_CLEAR_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR(x) ({ \
    vTaskExitCritical(); \
    v_ਪੋਰਟ_Clear_ਰੁਕਾਵਟ_ਨਕਾਬ(x); \
})

// ---------------------- Yielding -------------------------

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_YIELD()                                 v_ਪੋਰਟ_Yield()
#ਜੇ defined(__cplusplus) && (__cplusplus >  201703L)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_YIELD_FROM_ISR(...)                     ਚੁਣੋ_MACRO_VA_ARG(ਪੋਰਟ_YIELD_FROM_ISR_CHECK, ਪੋਰਟ_YIELD_FROM_ISR_NO_CHECK __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_YIELD_FROM_ISR(...)                     ਚੁਣੋ_MACRO_VA_ARG(ਪੋਰਟ_YIELD_FROM_ISR_CHECK, ਪੋਰਟ_YIELD_FROM_ISR_NO_CHECK, ##__VA_ARGS__)(__VA_ARGS__)
#ਜੇ_ਅੰਤ
// Note that the Linux port does not provide portYIELD_CORE(x) because purely single core for now.

// ----------------------- System --------------------------

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_GET_CORE_ID()                           x_ਪੋਰਟ_Get_Core_ID()
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_CHECK_IF_ਵਿੱਚ_ISR()                       x_ਪੋਰਟ_Check_If_ਵਿੱਚ_ISR()

// ------------------- Run Time Stats ----------------------

ਬਾਹਰੀ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ul_ਪੋਰਟ_Get_ਲੜੀਵਾਰ_ਸਮਾਂ( ਖਾਲੀ );
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_CONFIGURE_TIMER_FOR_ਲੜੀਵਾਰ_ਸਮਾਂ_STATS() /* no-op */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_GET_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_ਮੁੱਲ()         ul_ਪੋਰਟ_Get_ਲੜੀਵਾਰ_ਸਮਾਂ()
// TODO IDF-5723: no esp_timer implementation

// ------------------- TCB Cleanup ----------------------

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_CLEAN_ਉੱਪਰ_TCB( pxTCB )                   v_ਪੋਰਟ_Clean_ਉੱਪਰ_TCB( pxTCB )

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
ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਧਾਰ_Type_t x_ਪੋਰਟ_Get_Core_ID(ਖਾਲੀ)
{
    ਵਾਪਸ (ਅਧਾਰ_Type_t) 0; // POSIX simulator is only single-core at the moment
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਧਾਰ_Type_t x_ਪੋਰਟ_ਵਿੱਚ_Isr_Context(ਖਾਲੀ)
{
    //Just call the FreeRTOS port interface version
    ਵਾਪਸ x_ਪੋਰਟ_Check_If_ਵਿੱਚ_ISR();
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ v_ਪੋਰਟ_Assert_If_ਵਿੱਚ_ISR(ਖਾਲੀ)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(x_ਪੋਰਟ_ਵਿੱਚ_Isr_Context() == 0);
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

ਖਾਲੀ v_ਪੋਰਟ_Enter_Critical_IDF(ਖਾਲੀ);
ਖਾਲੀ v_ਪੋਰਟ_Exit_Critical_IDF(ਖਾਲੀ);

// We ignore the lock and timeout argument on POSIX simulator since it is single core

//IDF task critical sections
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TRY_ENTER_CRITICAL(ਤਾਲਾ, timeout)       ({(ਖਾਲੀ) ਤਾਲਾ; (ਖਾਲੀ) timeout; v_ਪੋਰਟ_Enter_Critical_IDF(); pdPASS;})
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENTER_CRITICAL_IDF(ਤਾਲਾ)                ({(ਖਾਲੀ) ਤਾਲਾ; v_ਪੋਰਟ_Enter_Critical_IDF();})
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_EXIT_CRITICAL_IDF(ਤਾਲਾ)                 ({(ਖਾਲੀ) ਤਾਲਾ; v_ਪੋਰਟ_Exit_Critical_IDF();})
//IDF ISR critical sections
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TRY_ENTER_CRITICAL_ISR(ਤਾਲਾ, timeout)   ({(ਖਾਲੀ) ਤਾਲਾ; (ਖਾਲੀ) timeout; v_ਪੋਰਟ_Enter_Critical_IDF(); pdPASS;})
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENTER_CRITICAL_ISR(ਤਾਲਾ)                ({(ਖਾਲੀ) ਤਾਲਾ; v_ਪੋਰਟ_Enter_Critical_IDF();})
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_EXIT_CRITICAL_ISR(ਤਾਲਾ)                 ({(ਖਾਲੀ) ਤਾਲਾ; v_ਪੋਰਟ_Exit_Critical_IDF();})
//IDF safe critical sections (they're the same)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENTER_CRITICAL_ਸੁਰੱਖਿਅਤ(ਤਾਲਾ)               ({(ਖਾਲੀ) ਤਾਲਾ; v_ਪੋਰਟ_Enter_Critical_IDF();})
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_EXIT_CRITICAL_ਸੁਰੱਖਿਅਤ(ਤਾਲਾ)                ({(ਖਾਲੀ) ਤਾਲਾ; v_ਪੋਰਟ_Exit_Critical_IDF();})

// ---------------------- Yielding -------------------------

ਬਾਹਰੀ ਖਾਲੀ v_ਪੋਰਟ_Yield( ਖਾਲੀ );
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_YIELD() v_ਪੋਰਟ_Yield()

// ----------------------- System --------------------------

bool ਪੋਰਟ_VALID_LIST_MEM(ਅਚਲ ਖਾਲੀ *ptr);
bool ਪੋਰਟ_VALID_TCB_MEM(ਅਚਲ ਖਾਲੀ *ptr);
bool ਪੋਰਟ_VALID_ਢੇਰ_MEM(ਅਚਲ ਖਾਲੀ *ptr);

/*-----------------------------------------------------------*/

/* POSIX-specific port functions */

ਬਾਹਰੀ ਖਾਲੀ v_ਪੋਰਟ_Thread_Dying( ਖਾਲੀ *px_Task_ਮੰਜ਼ਿਲ_Delete, ਅਸਥਿਰ ਅਧਾਰ_Type_t *pxPendYield );
ਬਾਹਰੀ ਖਾਲੀ v_ਪੋਰਟ_Cancel_Thread( ਖਾਲੀ *px_Task_ਮੰਜ਼ਿਲ_Delete );
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_PRE_TASK_DELETE_HOOK( pv_Task_ਮੰਜ਼ਿਲ_Delete, pxPendYield ) v_ਪੋਰਟ_Thread_Dying( ( pv_Task_ਮੰਜ਼ਿਲ_Delete ), ( pxPendYield ) )
/*-----------------------------------------------------------*/

/*
 * Tasks run in their own pthreads and context switches between them
 * are always a full memory barrier. ISRs are emulated as signals
 * which also imply a full memory barrier.
 *
 * Thus, only a compiler barrier is needed to prevent the compiler
 * reordering.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਸਮ੍ਰਿਤੀ_BARRIER() __asm ਅਸਥਿਰ( "" ::: "memory" )

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /* PORTMACRO_H */
