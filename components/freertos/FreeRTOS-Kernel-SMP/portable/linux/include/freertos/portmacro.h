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


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ PORTMACRO_H
#ವ್ಯಾಖ್ಯಾನಿಸು PORTMACRO_H

#ಅನ್ತರ್ಭಾವ <limits.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ "esp_macros.h"
#ಅನ್ತರ್ಭಾವ "spinlock.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
ಬಾಹ್ಯ "C" {
#ಆದರೆ_ಅಂತ್ಯ

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come before first as they are used further down the file
 * ------------------------------------------------------------------------------------------------------------------ */

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

/* Task function macros as described on the FreeRTOS.org WEB site. */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TASK_FUNCTION_PROTO( vFunction, pvParameters ) ಶೂನ್ಯ vFunction( ಶೂನ್ಯ *pvParameters )
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TASK_FUNCTION( vFunction, pvParameters ) ಶೂನ್ಯ vFunction( ಶೂನ್ಯ *pvParameters )
/*-----------------------------------------------------------*/

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_CRITICAL_NESTING_ಒಳ_TCB     1
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ರಾಶಿ_GROWTH                ( -1 )
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_HAS_ರಾಶಿ_ಅತಿಪ್ರವಾಹ_CHECKING ( 1 )   // Set in non-SMP POSIX port
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TICK_PERIOD_MS              ( ( TickType_t ) 1000 / config_TICK_ದರ_HZ )
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TICK_ದರ_MICROSECONDS      ( ( TickType_t ) 1000000 / config_TICK_ದರ_HZ )
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_BYTE_ALIGNMENT              8
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_NOP() __asm ಅಸ್ಥಿರ        (" nop ")
/*-----------------------------------------------------------*/

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros need to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Spinlocks ------------------------

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ spinlock_t                          ಪೋರ್ಟ್_MUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */
/**< Spinlock initializer */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_MUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_MUX_FREE_VAL                    SPINLOCK_FREE           /**< Spinlock is free. [refactor-todo] check if this is still required */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_MUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_MUX_TRY_ಬೀಗ                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

ತಳ_Type_t x_ಪೋರ್ಟ್_Check_If_ಒಳ_ISR(ಶೂನ್ಯ);

// ------------------ Critical Sections --------------------

#ಆದರೆ ( config_ಸಂಖ್ಯೆ_ದ_CORES > 1 )
/*
These are always called with interrupts already disabled. We simply need to get/release the spinlocks
*/
ಬಾಹ್ಯ ಪೋರ್ಟ್_MUX_TYPE ಪೋರ್ಟ್_x_Task_ಬೀಗ;
ಬಾಹ್ಯ ಪೋರ್ಟ್_MUX_TYPE ಪೋರ್ಟ್_x_ISRLock;

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Take_ಬೀಗ( ಪೋರ್ಟ್_MUX_TYPE *ಬೀಗ_2 );
ಶೂನ್ಯ v_ಪೋರ್ಟ್_Release_ಬೀಗ( ಪೋರ್ಟ್_MUX_TYPE *ಬೀಗ_2 );
#ಆದರೆ_ಅಂತ್ಯ /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Yield( ಶೂನ್ಯ );
ಬಾಹ್ಯ ಶೂನ್ಯ v_ಪೋರ್ಟ್_Yield_From_ISR( ಶೂನ್ಯ );

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_YIELD_FROM_ISR_CHECK(x)     ({ \
    ಆದರೆ ( (x) == pdTRUE ) { \
        v_ಪೋರ್ಟ್_Yield_From_ISR(); \
    } \
})
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_YIELD_FROM_ISR_NO_CHECK()               v_ಪೋರ್ಟ್_Yield_From_ISR()

// ----------------------- System --------------------------

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಳ_Type_t __attribute__((always_inline)) x_ಪೋರ್ಟ್_Get_Core_ID( ಶೂನ್ಯ );

// ----------------------- TCB Cleanup --------------------------

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Clean_ಮೇಲೆ_TCB ( ಶೂನ್ಯ *pxTCB );

/* ----------------------------------------- FreeRTOS SMP Porting Interface --------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS SMP
 * - Must come after forward declare as some porting macros map to declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_DISABLE_INTERRUPTS()                    x_ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ()
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ENABLE_INTERRUPTS()                     v_ಪೋರ್ಟ್_Clear_ಅಡಚಣೆ_ಮುಖವಾಡ(0)

// ------------------ Critical Sections --------------------

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ENTER_CRITICAL_SMP()                    vTaskEnterCritical();
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_EXIT_CRITICAL_SMP()                     vTaskExitCritical();

#ಆದರೆ defined(__cplusplus) && (__cplusplus >  201703L)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ENTER_CRITICAL(...)                     ಆರಿಸು_MACRO_VA_ARG(ಪೋರ್ಟ್_ENTER_CRITICAL_IDF, ಪೋರ್ಟ್_ENTER_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_EXIT_CRITICAL(...)                      ಆರಿಸು_MACRO_VA_ARG(ಪೋರ್ಟ್_EXIT_CRITICAL_IDF, ಪೋರ್ಟ್_EXIT_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ENTER_CRITICAL(...)                     ಆರಿಸು_MACRO_VA_ARG(ಪೋರ್ಟ್_ENTER_CRITICAL_IDF, ಪೋರ್ಟ್_ENTER_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_EXIT_CRITICAL(...)                      ಆರಿಸು_MACRO_VA_ARG(ಪೋರ್ಟ್_EXIT_CRITICAL_IDF, ಪೋರ್ಟ್_EXIT_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#ಆದರೆ_ಅಂತ್ಯ

ಬಾಹ್ಯ ತಳ_Type_t x_ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ( ಶೂನ್ಯ );
ಬಾಹ್ಯ ಶೂನ್ಯ v_ಪೋರ್ಟ್_Clear_ಅಡಚಣೆ_ಮುಖವಾಡ( ತಳ_Type_t x_ಮುಖವಾಡ );

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR() ({ \
    ತಳ_Type_t cur_level; \
    cur_level = x_ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ(); \
    vTaskEnterCritical(); \
    cur_level; \
})
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_CLEAR_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR(x) ({ \
    vTaskExitCritical(); \
    v_ಪೋರ್ಟ್_Clear_ಅಡಚಣೆ_ಮುಖವಾಡ(x); \
})

// ---------------------- Yielding -------------------------

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_YIELD()                                 v_ಪೋರ್ಟ್_Yield()
#ಆದರೆ defined(__cplusplus) && (__cplusplus >  201703L)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_YIELD_FROM_ISR(...)                     ಆರಿಸು_MACRO_VA_ARG(ಪೋರ್ಟ್_YIELD_FROM_ISR_CHECK, ಪೋರ್ಟ್_YIELD_FROM_ISR_NO_CHECK __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_YIELD_FROM_ISR(...)                     ಆರಿಸು_MACRO_VA_ARG(ಪೋರ್ಟ್_YIELD_FROM_ISR_CHECK, ಪೋರ್ಟ್_YIELD_FROM_ISR_NO_CHECK, ##__VA_ARGS__)(__VA_ARGS__)
#ಆದರೆ_ಅಂತ್ಯ
// Note that the Linux port does not provide portYIELD_CORE(x) because purely single core for now.

// ----------------------- System --------------------------

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_GET_CORE_ID()                           x_ಪೋರ್ಟ್_Get_Core_ID()
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_CHECK_IF_ಒಳ_ISR()                       x_ಪೋರ್ಟ್_Check_If_ಒಳ_ISR()

// ------------------- Run Time Stats ----------------------

ಬಾಹ್ಯ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ul_ಪೋರ್ಟ್_Get_ಸತತ_ಸಮಯ( ಶೂನ್ಯ );
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_CONFIGURE_TIMER_FOR_ಸತತ_ಸಮಯ_STATS() /* no-op */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_GET_ಸತತ_ಸಮಯ_COUNTER_ಮೌಲ್ಯ()         ul_ಪೋರ್ಟ್_Get_ಸತತ_ಸಮಯ()
// TODO IDF-5723: no esp_timer implementation

// ------------------- TCB Cleanup ----------------------

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_CLEAN_ಮೇಲೆ_TCB( pxTCB )                   v_ಪೋರ್ಟ್_Clean_ಮೇಲೆ_TCB( pxTCB )

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
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಳ_Type_t x_ಪೋರ್ಟ್_Get_Core_ID(ಶೂನ್ಯ)
{
    ಹಿಂತಿರುಗಿ (ತಳ_Type_t) 0; // POSIX simulator is only single-core at the moment
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಳ_Type_t x_ಪೋರ್ಟ್_ಒಳ_Isr_Context(ಶೂನ್ಯ)
{
    //Just call the FreeRTOS port interface version
    ಹಿಂತಿರುಗಿ x_ಪೋರ್ಟ್_Check_If_ಒಳ_ISR();
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_ಪೋರ್ಟ್_Assert_If_ಒಳ_ISR(ಶೂನ್ಯ)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(x_ಪೋರ್ಟ್_ಒಳ_Isr_Context() == 0);
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

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Enter_Critical_IDF(ಶೂನ್ಯ);
ಶೂನ್ಯ v_ಪೋರ್ಟ್_Exit_Critical_IDF(ಶೂನ್ಯ);

// We ignore the lock and timeout argument on POSIX simulator since it is single core

//IDF task critical sections
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TRY_ENTER_CRITICAL(ಬೀಗ_2, timeout)       ({(ಶೂನ್ಯ) ಬೀಗ_2; (ಶೂನ್ಯ) timeout; v_ಪೋರ್ಟ್_Enter_Critical_IDF(); pdPASS;})
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ENTER_CRITICAL_IDF(ಬೀಗ_2)                ({(ಶೂನ್ಯ) ಬೀಗ_2; v_ಪೋರ್ಟ್_Enter_Critical_IDF();})
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_EXIT_CRITICAL_IDF(ಬೀಗ_2)                 ({(ಶೂನ್ಯ) ಬೀಗ_2; v_ಪೋರ್ಟ್_Exit_Critical_IDF();})
//IDF ISR critical sections
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TRY_ENTER_CRITICAL_ISR(ಬೀಗ_2, timeout)   ({(ಶೂನ್ಯ) ಬೀಗ_2; (ಶೂನ್ಯ) timeout; v_ಪೋರ್ಟ್_Enter_Critical_IDF(); pdPASS;})
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ENTER_CRITICAL_ISR(ಬೀಗ_2)                ({(ಶೂನ್ಯ) ಬೀಗ_2; v_ಪೋರ್ಟ್_Enter_Critical_IDF();})
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_EXIT_CRITICAL_ISR(ಬೀಗ_2)                 ({(ಶೂನ್ಯ) ಬೀಗ_2; v_ಪೋರ್ಟ್_Exit_Critical_IDF();})
//IDF safe critical sections (they're the same)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ENTER_CRITICAL_ಸುರಕ್ಷಿತ(ಬೀಗ_2)               ({(ಶೂನ್ಯ) ಬೀಗ_2; v_ಪೋರ್ಟ್_Enter_Critical_IDF();})
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_EXIT_CRITICAL_ಸುರಕ್ಷಿತ(ಬೀಗ_2)                ({(ಶೂನ್ಯ) ಬೀಗ_2; v_ಪೋರ್ಟ್_Exit_Critical_IDF();})

// ---------------------- Yielding -------------------------

ಬಾಹ್ಯ ಶೂನ್ಯ v_ಪೋರ್ಟ್_Yield( ಶೂನ್ಯ );
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_YIELD() v_ಪೋರ್ಟ್_Yield()

// ----------------------- System --------------------------

bool ಪೋರ್ಟ್_VALID_LIST_MEM(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ptr);
bool ಪೋರ್ಟ್_VALID_TCB_MEM(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ptr);
bool ಪೋರ್ಟ್_VALID_ರಾಶಿ_MEM(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ptr);

/*-----------------------------------------------------------*/

/* POSIX-specific port functions */

ಬಾಹ್ಯ ಶೂನ್ಯ v_ಪೋರ್ಟ್_Thread_Dying( ಶೂನ್ಯ *px_Task_ಗಮ್ಯ_Delete, ಅಸ್ಥಿರ ತಳ_Type_t *pxPendYield );
ಬಾಹ್ಯ ಶೂನ್ಯ v_ಪೋರ್ಟ್_Cancel_Thread( ಶೂನ್ಯ *px_Task_ಗಮ್ಯ_Delete );
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_PRE_TASK_DELETE_HOOK( pv_Task_ಗಮ್ಯ_Delete, pxPendYield ) v_ಪೋರ್ಟ್_Thread_Dying( ( pv_Task_ಗಮ್ಯ_Delete ), ( pxPendYield ) )
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

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ /* PORTMACRO_H */
