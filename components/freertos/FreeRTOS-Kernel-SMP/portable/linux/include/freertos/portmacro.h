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


#اگر_تعریف_نشده PORTMACRO_H
#تعریف_کن PORTMACRO_H

#ضمن <limits.h>
#ضمن <stdint.h>
#ضمن "esp_macros.h"
#ضمن "spinlock.h"

#اگر_تعریف_شده __cplusplus
خارجي "C" {
#پایان_اگر

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come before first as they are used further down the file
 * ------------------------------------------------------------------------------------------------------------------ */

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

/* Task function macros as described on the FreeRTOS.org WEB site. */
#تعریف_کن پورت_TASK_FUNCTION_PROTO( vFunction, pvParameters ) فراغ vFunction( فراغ *pvParameters )
#تعریف_کن پورت_TASK_FUNCTION( vFunction, pvParameters ) فراغ vFunction( فراغ *pvParameters )
/*-----------------------------------------------------------*/

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#تعریف_کن پورت_CRITICAL_NESTING_درون_TCB     1
#تعریف_کن پورت_پشته_GROWTH                ( -1 )
#تعریف_کن پورت_HAS_پشته_سرریز_CHECKING ( 1 )   // Set in non-SMP POSIX port
#تعریف_کن پورت_TICK_PERIOD_MS              ( ( TickType_t ) 1000 / config_TICK_نرخ_HZ )
#تعریف_کن پورت_TICK_نرخ_MICROSECONDS      ( ( TickType_t ) 1000000 / config_TICK_نرخ_HZ )
#تعریف_کن پورت_BYTE_ALIGNMENT              8
#تعریف_کن پورت_NOP() __asm متطاير        (" nop ")
/*-----------------------------------------------------------*/

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros need to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Spinlocks ------------------------

تعريفنوع spinlock_t                          پورت_MUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */
/**< Spinlock initializer */
#تعریف_کن پورت_MUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#تعریف_کن پورت_MUX_FREE_VAL                    SPINLOCK_FREE           /**< Spinlock is free. [refactor-todo] check if this is still required */
#تعریف_کن پورت_MUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#تعریف_کن پورت_MUX_TRY_قفل                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#تعریف_کن پورت_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

پایه_Type_t x_پورت_Check_If_درون_ISR(فراغ);

// ------------------ Critical Sections --------------------

#إذا ( config_عدد_از_CORES > 1 )
/*
These are always called with interrupts already disabled. We simply need to get/release the spinlocks
*/
خارجي پورت_MUX_TYPE پورت_x_Task_قفل;
خارجي پورت_MUX_TYPE پورت_x_ISRLock;

فراغ v_پورت_Take_قفل( پورت_MUX_TYPE *قفل_2 );
فراغ v_پورت_Release_قفل( پورت_MUX_TYPE *قفل_2 );
#پایان_اگر /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

فراغ v_پورت_Yield( فراغ );
خارجي فراغ v_پورت_Yield_From_ISR( فراغ );

#تعریف_کن پورت_YIELD_FROM_ISR_CHECK(x)     ({ \
    إذا ( (x) == pdTRUE ) { \
        v_پورت_Yield_From_ISR(); \
    } \
})
#تعریف_کن پورت_YIELD_FROM_ISR_NO_CHECK()               v_پورت_Yield_From_ISR()

// ----------------------- System --------------------------

ساكن مضمن پایه_Type_t __attribute__((always_inline)) x_پورت_Get_Core_ID( فراغ );

// ----------------------- TCB Cleanup --------------------------

فراغ v_پورت_Clean_به_بالا_TCB ( فراغ *pxTCB );

/* ----------------------------------------- FreeRTOS SMP Porting Interface --------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS SMP
 * - Must come after forward declare as some porting macros map to declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

#تعریف_کن پورت_DISABLE_INTERRUPTS()                    x_پورت_مجموعه_وقفه_ماسک()
#تعریف_کن پورت_ENABLE_INTERRUPTS()                     v_پورت_Clear_وقفه_ماسک(0)

// ------------------ Critical Sections --------------------

#تعریف_کن پورت_ENTER_CRITICAL_SMP()                    vTaskEnterCritical();
#تعریف_کن پورت_EXIT_CRITICAL_SMP()                     vTaskExitCritical();

#إذا defined(__cplusplus) && (__cplusplus >  201703L)
#تعریف_کن پورت_ENTER_CRITICAL(...)                     انتخاب_MACRO_VA_ARG(پورت_ENTER_CRITICAL_IDF, پورت_ENTER_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#تعریف_کن پورت_EXIT_CRITICAL(...)                      انتخاب_MACRO_VA_ARG(پورت_EXIT_CRITICAL_IDF, پورت_EXIT_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#وإلا
#تعریف_کن پورت_ENTER_CRITICAL(...)                     انتخاب_MACRO_VA_ARG(پورت_ENTER_CRITICAL_IDF, پورت_ENTER_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#تعریف_کن پورت_EXIT_CRITICAL(...)                      انتخاب_MACRO_VA_ARG(پورت_EXIT_CRITICAL_IDF, پورت_EXIT_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#پایان_اگر

خارجي پایه_Type_t x_پورت_مجموعه_وقفه_ماسک( فراغ );
خارجي فراغ v_پورت_Clear_وقفه_ماسک( پایه_Type_t x_ماسک );

#تعریف_کن پورت_مجموعه_وقفه_ماسک_FROM_ISR() ({ \
    پایه_Type_t cur_level; \
    cur_level = x_پورت_مجموعه_وقفه_ماسک(); \
    vTaskEnterCritical(); \
    cur_level; \
})
#تعریف_کن پورت_CLEAR_وقفه_ماسک_FROM_ISR(x) ({ \
    vTaskExitCritical(); \
    v_پورت_Clear_وقفه_ماسک(x); \
})

// ---------------------- Yielding -------------------------

#تعریف_کن پورت_YIELD()                                 v_پورت_Yield()
#إذا defined(__cplusplus) && (__cplusplus >  201703L)
#تعریف_کن پورت_YIELD_FROM_ISR(...)                     انتخاب_MACRO_VA_ARG(پورت_YIELD_FROM_ISR_CHECK, پورت_YIELD_FROM_ISR_NO_CHECK __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#وإلا
#تعریف_کن پورت_YIELD_FROM_ISR(...)                     انتخاب_MACRO_VA_ARG(پورت_YIELD_FROM_ISR_CHECK, پورت_YIELD_FROM_ISR_NO_CHECK, ##__VA_ARGS__)(__VA_ARGS__)
#پایان_اگر
// Note that the Linux port does not provide portYIELD_CORE(x) because purely single core for now.

// ----------------------- System --------------------------

#تعریف_کن پورت_GET_CORE_ID()                           x_پورت_Get_Core_ID()
#تعریف_کن پورت_CHECK_IF_درون_ISR()                       x_پورت_Check_If_درون_ISR()

// ------------------- Run Time Stats ----------------------

خارجي بیعلامت دراز ul_پورت_Get_پیاپی_زمان( فراغ );
#تعریف_کن پورت_CONFIGURE_TIMER_FOR_پیاپی_زمان_STATS() /* no-op */
#تعریف_کن پورت_GET_پیاپی_زمان_COUNTER_مقدار()         ul_پورت_Get_پیاپی_زمان()
// TODO IDF-5723: no esp_timer implementation

// ------------------- TCB Cleanup ----------------------

#تعریف_کن پورت_CLEAN_به_بالا_TCB( pxTCB )                   v_پورت_Clean_به_بالا_TCB( pxTCB )

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
ساكن مضمن پایه_Type_t x_پورت_Get_Core_ID(فراغ)
{
    أرجع (پایه_Type_t) 0; // POSIX simulator is only single-core at the moment
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

ساكن مضمن پایه_Type_t x_پورت_درون_Isr_Context(فراغ)
{
    //Just call the FreeRTOS port interface version
    أرجع x_پورت_Check_If_درون_ISR();
}

ساكن مضمن فراغ v_پورت_Assert_If_درون_ISR(فراغ)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(x_پورت_درون_Isr_Context() == 0);
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

فراغ v_پورت_Enter_Critical_IDF(فراغ);
فراغ v_پورت_Exit_Critical_IDF(فراغ);

// We ignore the lock and timeout argument on POSIX simulator since it is single core

//IDF task critical sections
#تعریف_کن پورت_TRY_ENTER_CRITICAL(قفل_2, timeout)       ({(فراغ) قفل_2; (فراغ) timeout; v_پورت_Enter_Critical_IDF(); pdPASS;})
#تعریف_کن پورت_ENTER_CRITICAL_IDF(قفل_2)                ({(فراغ) قفل_2; v_پورت_Enter_Critical_IDF();})
#تعریف_کن پورت_EXIT_CRITICAL_IDF(قفل_2)                 ({(فراغ) قفل_2; v_پورت_Exit_Critical_IDF();})
//IDF ISR critical sections
#تعریف_کن پورت_TRY_ENTER_CRITICAL_ISR(قفل_2, timeout)   ({(فراغ) قفل_2; (فراغ) timeout; v_پورت_Enter_Critical_IDF(); pdPASS;})
#تعریف_کن پورت_ENTER_CRITICAL_ISR(قفل_2)                ({(فراغ) قفل_2; v_پورت_Enter_Critical_IDF();})
#تعریف_کن پورت_EXIT_CRITICAL_ISR(قفل_2)                 ({(فراغ) قفل_2; v_پورت_Exit_Critical_IDF();})
//IDF safe critical sections (they're the same)
#تعریف_کن پورت_ENTER_CRITICAL_امن(قفل_2)               ({(فراغ) قفل_2; v_پورت_Enter_Critical_IDF();})
#تعریف_کن پورت_EXIT_CRITICAL_امن(قفل_2)                ({(فراغ) قفل_2; v_پورت_Exit_Critical_IDF();})

// ---------------------- Yielding -------------------------

خارجي فراغ v_پورت_Yield( فراغ );
#تعریف_کن پورت_YIELD() v_پورت_Yield()

// ----------------------- System --------------------------

bool پورت_VALID_LIST_MEM(ثابت فراغ *ptr);
bool پورت_VALID_TCB_MEM(ثابت فراغ *ptr);
bool پورت_VALID_پشته_MEM(ثابت فراغ *ptr);

/*-----------------------------------------------------------*/

/* POSIX-specific port functions */

خارجي فراغ v_پورت_Thread_Dying( فراغ *px_Task_مقصد_Delete, متطاير پایه_Type_t *pxPendYield );
خارجي فراغ v_پورت_Cancel_Thread( فراغ *px_Task_مقصد_Delete );
#تعریف_کن پورت_PRE_TASK_DELETE_HOOK( pv_Task_مقصد_Delete, pxPendYield ) v_پورت_Thread_Dying( ( pv_Task_مقصد_Delete ), ( pxPendYield ) )
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

#اگر_تعریف_شده __cplusplus
}
#پایان_اگر

#پایان_اگر /* PORTMACRO_H */
