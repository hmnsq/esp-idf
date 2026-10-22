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


#جيڪڏهن_وصف_نه_ٿيل PORTMACRO_H
#وصف_ڪريو PORTMACRO_H

#ضمن <limits.h>
#ضمن <stdint.h>
#ضمن "esp_macros.h"
#ضمن "spinlock.h"

#جيڪڏهن_وصف_ٿيل __cplusplus
خارجي "C" {
#جيڪڏهن_پورو

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come before first as they are used further down the file
 * ------------------------------------------------------------------------------------------------------------------ */

/* Type definitions. */
#وصف_ڪريو پورٽ_CHAR        حرف
#وصف_ڪريو پورٽ_FLOAT       عائم
#وصف_ڪريو پورٽ_DOUBLE      مزدوج
#وصف_ڪريو پورٽ_LONG        ڊگهو
#وصف_ڪريو پورٽ_SHORT       ننڍو
#وصف_ڪريو پورٽ_ڍير_TYPE  بنانشان ڊگهو
#وصف_ڪريو پورٽ_بنياد_TYPE   ڊگهو
#وصف_ڪريو پورٽ_اشارو_سائيز_TYPE intptr_t

تعريفنوع پورٽ_ڍير_TYPE ڍير_Type_t;
تعريفنوع پورٽ_بنياد_TYPE بنياد_Type_t;
تعريفنوع بنانشان پورٽ_بنياد_TYPE UBaseType_t;

تعريفنوع بنانشان ڊگهو TickType_t;
#وصف_ڪريو پورٽ_MAX_DELAY ( TickType_t ) ULONG_MAX

#وصف_ڪريو پورٽ_TICK_TYPE_IS_ATOMIC 1

/* Task function macros as described on the FreeRTOS.org WEB site. */
#وصف_ڪريو پورٽ_TASK_FUNCTION_PROTO( vFunction, pvParameters ) فراغ vFunction( فراغ *pvParameters )
#وصف_ڪريو پورٽ_TASK_FUNCTION( vFunction, pvParameters ) فراغ vFunction( فراغ *pvParameters )
/*-----------------------------------------------------------*/

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#وصف_ڪريو پورٽ_CRITICAL_NESTING_اندريون_TCB     1
#وصف_ڪريو پورٽ_ڍير_GROWTH                ( -1 )
#وصف_ڪريو پورٽ_HAS_ڍير_حد_کان_مٿي_CHECKING ( 1 )   // Set in non-SMP POSIX port
#وصف_ڪريو پورٽ_TICK_PERIOD_MS              ( ( TickType_t ) 1000 / config_TICK_شرح_HZ )
#وصف_ڪريو پورٽ_TICK_شرح_MICROSECONDS      ( ( TickType_t ) 1000000 / config_TICK_شرح_HZ )
#وصف_ڪريو پورٽ_BYTE_ALIGNMENT              8
#وصف_ڪريو پورٽ_NOP() __asm متطاير        (" nop ")
/*-----------------------------------------------------------*/

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros need to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Spinlocks ------------------------

تعريفنوع spinlock_t                          پورٽ_MUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */
/**< Spinlock initializer */
#وصف_ڪريو پورٽ_MUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#وصف_ڪريو پورٽ_MUX_FREE_VAL                    SPINLOCK_FREE           /**< Spinlock is free. [refactor-todo] check if this is still required */
#وصف_ڪريو پورٽ_MUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#وصف_ڪريو پورٽ_MUX_TRY_تالو                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#وصف_ڪريو پورٽ_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

بنياد_Type_t x_پورٽ_Check_If_اندريون_ISR(فراغ);

// ------------------ Critical Sections --------------------

#إذا ( config_عدد_جو_CORES > 1 )
/*
These are always called with interrupts already disabled. We simply need to get/release the spinlocks
*/
خارجي پورٽ_MUX_TYPE پورٽ_x_Task_تالو;
خارجي پورٽ_MUX_TYPE پورٽ_x_ISRLock;

فراغ v_پورٽ_Take_تالو( پورٽ_MUX_TYPE *تالو );
فراغ v_پورٽ_Release_تالو( پورٽ_MUX_TYPE *تالو );
#جيڪڏهن_پورو /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

فراغ v_پورٽ_Yield( فراغ );
خارجي فراغ v_پورٽ_Yield_From_ISR( فراغ );

#وصف_ڪريو پورٽ_YIELD_FROM_ISR_CHECK(x)     ({ \
    إذا ( (x) == pdTRUE ) { \
        v_پورٽ_Yield_From_ISR(); \
    } \
})
#وصف_ڪريو پورٽ_YIELD_FROM_ISR_NO_CHECK()               v_پورٽ_Yield_From_ISR()

// ----------------------- System --------------------------

ساكن مضمن بنياد_Type_t __attribute__((always_inline)) x_پورٽ_Get_Core_ID( فراغ );

// ----------------------- TCB Cleanup --------------------------

فراغ v_پورٽ_Clean_مٿي_TCB ( فراغ *pxTCB );

/* ----------------------------------------- FreeRTOS SMP Porting Interface --------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS SMP
 * - Must come after forward declare as some porting macros map to declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

#وصف_ڪريو پورٽ_DISABLE_INTERRUPTS()                    x_پورٽ_سيٽ_مداخلت_نقاب()
#وصف_ڪريو پورٽ_ENABLE_INTERRUPTS()                     v_پورٽ_Clear_مداخلت_نقاب(0)

// ------------------ Critical Sections --------------------

#وصف_ڪريو پورٽ_ENTER_CRITICAL_SMP()                    vTaskEnterCritical();
#وصف_ڪريو پورٽ_EXIT_CRITICAL_SMP()                     vTaskExitCritical();

#إذا defined(__cplusplus) && (__cplusplus >  201703L)
#وصف_ڪريو پورٽ_ENTER_CRITICAL(...)                     چونڊيو_MACRO_VA_ARG(پورٽ_ENTER_CRITICAL_IDF, پورٽ_ENTER_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#وصف_ڪريو پورٽ_EXIT_CRITICAL(...)                      چونڊيو_MACRO_VA_ARG(پورٽ_EXIT_CRITICAL_IDF, پورٽ_EXIT_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#وإلا
#وصف_ڪريو پورٽ_ENTER_CRITICAL(...)                     چونڊيو_MACRO_VA_ARG(پورٽ_ENTER_CRITICAL_IDF, پورٽ_ENTER_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#وصف_ڪريو پورٽ_EXIT_CRITICAL(...)                      چونڊيو_MACRO_VA_ARG(پورٽ_EXIT_CRITICAL_IDF, پورٽ_EXIT_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#جيڪڏهن_پورو

خارجي بنياد_Type_t x_پورٽ_سيٽ_مداخلت_نقاب( فراغ );
خارجي فراغ v_پورٽ_Clear_مداخلت_نقاب( بنياد_Type_t x_نقاب );

#وصف_ڪريو پورٽ_سيٽ_مداخلت_نقاب_FROM_ISR() ({ \
    بنياد_Type_t cur_level; \
    cur_level = x_پورٽ_سيٽ_مداخلت_نقاب(); \
    vTaskEnterCritical(); \
    cur_level; \
})
#وصف_ڪريو پورٽ_CLEAR_مداخلت_نقاب_FROM_ISR(x) ({ \
    vTaskExitCritical(); \
    v_پورٽ_Clear_مداخلت_نقاب(x); \
})

// ---------------------- Yielding -------------------------

#وصف_ڪريو پورٽ_YIELD()                                 v_پورٽ_Yield()
#إذا defined(__cplusplus) && (__cplusplus >  201703L)
#وصف_ڪريو پورٽ_YIELD_FROM_ISR(...)                     چونڊيو_MACRO_VA_ARG(پورٽ_YIELD_FROM_ISR_CHECK, پورٽ_YIELD_FROM_ISR_NO_CHECK __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#وإلا
#وصف_ڪريو پورٽ_YIELD_FROM_ISR(...)                     چونڊيو_MACRO_VA_ARG(پورٽ_YIELD_FROM_ISR_CHECK, پورٽ_YIELD_FROM_ISR_NO_CHECK, ##__VA_ARGS__)(__VA_ARGS__)
#جيڪڏهن_پورو
// Note that the Linux port does not provide portYIELD_CORE(x) because purely single core for now.

// ----------------------- System --------------------------

#وصف_ڪريو پورٽ_GET_CORE_ID()                           x_پورٽ_Get_Core_ID()
#وصف_ڪريو پورٽ_CHECK_IF_اندريون_ISR()                       x_پورٽ_Check_If_اندريون_ISR()

// ------------------- Run Time Stats ----------------------

خارجي بنانشان ڊگهو ul_پورٽ_Get_لاڳيتو_وقت( فراغ );
#وصف_ڪريو پورٽ_CONFIGURE_TIMER_FOR_لاڳيتو_وقت_STATS() /* no-op */
#وصف_ڪريو پورٽ_GET_لاڳيتو_وقت_COUNTER_قدر()         ul_پورٽ_Get_لاڳيتو_وقت()
// TODO IDF-5723: no esp_timer implementation

// ------------------- TCB Cleanup ----------------------

#وصف_ڪريو پورٽ_CLEAN_مٿي_TCB( pxTCB )                   v_پورٽ_Clean_مٿي_TCB( pxTCB )

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
ساكن مضمن بنياد_Type_t x_پورٽ_Get_Core_ID(فراغ)
{
    أرجع (بنياد_Type_t) 0; // POSIX simulator is only single-core at the moment
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

ساكن مضمن بنياد_Type_t x_پورٽ_اندريون_Isr_Context(فراغ)
{
    //Just call the FreeRTOS port interface version
    أرجع x_پورٽ_Check_If_اندريون_ISR();
}

ساكن مضمن فراغ v_پورٽ_Assert_If_اندريون_ISR(فراغ)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(x_پورٽ_اندريون_Isr_Context() == 0);
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

فراغ v_پورٽ_Enter_Critical_IDF(فراغ);
فراغ v_پورٽ_Exit_Critical_IDF(فراغ);

// We ignore the lock and timeout argument on POSIX simulator since it is single core

//IDF task critical sections
#وصف_ڪريو پورٽ_TRY_ENTER_CRITICAL(تالو, timeout)       ({(فراغ) تالو; (فراغ) timeout; v_پورٽ_Enter_Critical_IDF(); pdPASS;})
#وصف_ڪريو پورٽ_ENTER_CRITICAL_IDF(تالو)                ({(فراغ) تالو; v_پورٽ_Enter_Critical_IDF();})
#وصف_ڪريو پورٽ_EXIT_CRITICAL_IDF(تالو)                 ({(فراغ) تالو; v_پورٽ_Exit_Critical_IDF();})
//IDF ISR critical sections
#وصف_ڪريو پورٽ_TRY_ENTER_CRITICAL_ISR(تالو, timeout)   ({(فراغ) تالو; (فراغ) timeout; v_پورٽ_Enter_Critical_IDF(); pdPASS;})
#وصف_ڪريو پورٽ_ENTER_CRITICAL_ISR(تالو)                ({(فراغ) تالو; v_پورٽ_Enter_Critical_IDF();})
#وصف_ڪريو پورٽ_EXIT_CRITICAL_ISR(تالو)                 ({(فراغ) تالو; v_پورٽ_Exit_Critical_IDF();})
//IDF safe critical sections (they're the same)
#وصف_ڪريو پورٽ_ENTER_CRITICAL_محفوظ(تالو)               ({(فراغ) تالو; v_پورٽ_Enter_Critical_IDF();})
#وصف_ڪريو پورٽ_EXIT_CRITICAL_محفوظ(تالو)                ({(فراغ) تالو; v_پورٽ_Exit_Critical_IDF();})

// ---------------------- Yielding -------------------------

خارجي فراغ v_پورٽ_Yield( فراغ );
#وصف_ڪريو پورٽ_YIELD() v_پورٽ_Yield()

// ----------------------- System --------------------------

bool پورٽ_VALID_LIST_MEM(ثابت فراغ *ptr);
bool پورٽ_VALID_TCB_MEM(ثابت فراغ *ptr);
bool پورٽ_VALID_ڍير_MEM(ثابت فراغ *ptr);

/*-----------------------------------------------------------*/

/* POSIX-specific port functions */

خارجي فراغ v_پورٽ_Thread_Dying( فراغ *px_Task_منزل_Delete, متطاير بنياد_Type_t *pxPendYield );
خارجي فراغ v_پورٽ_Cancel_Thread( فراغ *px_Task_منزل_Delete );
#وصف_ڪريو پورٽ_PRE_TASK_DELETE_HOOK( pv_Task_منزل_Delete, pxPendYield ) v_پورٽ_Thread_Dying( ( pv_Task_منزل_Delete ), ( pxPendYield ) )
/*-----------------------------------------------------------*/

/*
 * Tasks run in their own pthreads and context switches between them
 * are always a full memory barrier. ISRs are emulated as signals
 * which also imply a full memory barrier.
 *
 * Thus, only a compiler barrier is needed to prevent the compiler
 * reordering.
 */
#وصف_ڪريو پورٽ_يادگيري_BARRIER() __asm متطاير( "" ::: "memory" )

#جيڪڏهن_وصف_ٿيل __cplusplus
}
#جيڪڏهن_پورو

#جيڪڏهن_پورو /* PORTMACRO_H */
