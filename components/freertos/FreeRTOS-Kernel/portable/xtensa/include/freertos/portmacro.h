/*
 * FreeRTOS Kernel V10.5.1
 * Copyright (C) 2015-2019 Cadence Design Systems, Inc.
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-FileCopyrightText: 2015-2019 Cadence Design Systems, Inc
 * SPDX-FileCopyrightText: 2021 Amazon.com, Inc. or its affiliates
 *
 * SPDX-License-Identifier: MIT
 *
 * SPDX-FileContributor: 2023-2026 Espressif Systems (Shanghai) CO LTD
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
 *
 */

#اگر_غیر_تعریف_شدہ PORTMACRO_H
#تعریف_کرو PORTMACRO_H

#اگر_غیر_تعریف_شدہ __ASSEMBLER__

#شامل "sdkconfig.h"
#شامل <stdint.h>
#شامل <stdlib.h>
#شامل <stdbool.h>
#شامل <stdarg.h>
#شامل <xtensa/config/core.h>
#شامل <xtensa/xtruntime.h>       /* required for XTOS_SET_INTLEVEL. [refactor-todo] add common intr functions to esp_hw_support */
#شامل "xt_instr_macros.h"
#شامل "spinlock.h"
#شامل "esp_private/crosscore_int.h"
#شامل "esp_macros.h"
#شامل "esp_attr.h"
#شامل "esp_cpu.h"
#شامل "esp_memory_utils.h"
#شامل "esp_newlib.h"             /* required for esp_reent_init() in tasks.c */
#شامل "esp_heap_caps.h"
#شامل "esp_rom_sys.h"
#شامل "esp_system.h"             /* required by esp_get_...() functions in portable.h. [refactor-todo] Update portable.h */
#شامل "portbenchmark.h"
#شامل <limits.h>

#اگر_تعریف_شدہ __cplusplus
خارجی "C" {
#اگر_ختم


/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come before first as they are used further down the file
 * ------------------------------------------------------------------------------------------------------------------ */

#تعریف_کرو پورٹ_CHAR                    int8_t
#تعریف_کرو پورٹ_FLOAT                   اعشاری
#تعریف_کرو پورٹ_DOUBLE                  دوہرا
#تعریف_کرو پورٹ_LONG                    int32_t
#تعریف_کرو پورٹ_SHORT                   int16_t
#تعریف_کرو پورٹ_ڈھیر_TYPE              uint8_t
#تعریف_کرو پورٹ_بنیاد_TYPE               صحیح

قسمتعریف پورٹ_ڈھیر_TYPE              ڈھیر_Type_t;
قسمتعریف پورٹ_بنیاد_TYPE               بنیاد_Type_t;
قسمتعریف غیرعلامتی پورٹ_بنیاد_TYPE      UBaseType_t;

#اگر( config_USE_16_ثنائی_ہندسہ_TICKS == 1 )
قسمتعریف uint16_t TickType_t;
#تعریف_کرو پورٹ_MAX_DELAY ( TickType_t ) 0xffff
#ورنہ
قسمتعریف uint32_t TickType_t;
#تعریف_کرو پورٹ_MAX_DELAY ( TickType_t ) 0xffffffffUL
#اگر_ختم

/* Task function macros as described on the FreeRTOS.org WEB site. */
#تعریف_کرو پورٹ_TASK_FUNCTION_PROTO( vFunction, pvParameters )  خالی vFunction( خالی *pvParameters )
#تعریف_کرو پورٹ_TASK_FUNCTION( vFunction, pvParameters )        خالی vFunction( خالی *pvParameters )



/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#تعریف_کرو پورٹ_CRITICAL_NESTING_اندر_TCB     0
#تعریف_کرو پورٹ_ڈھیر_GROWTH                ( -1 )
#تعریف_کرو پورٹ_TICK_PERIOD_MS              ( ( TickType_t ) 1000 / config_TICK_شرح_HZ )
#تعریف_کرو پورٹ_BYTE_ALIGNMENT              16    // Xtensa Windowed ABI requires the stack pointer to always be 16-byte aligned. See "isa_rm.pdf 8.1.1 Windowed Register Usage and Stack Layout"
#تعریف_کرو پورٹ_TICK_TYPE_IS_ATOMIC         1
#تعریف_کرو پورٹ_NOP()                       XT_NOP()



/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros need to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

/**
 * @brief Checks if the current core is in an ISR context
 *
 * - ISR context consist of Low/Mid priority ISR, or time tick ISR
 * - High priority ISRs aren't detected here, but they normally cannot call C code, so that should not be an issue anyway.
 *
 * @note [refactor-todo] Check if this should be inlined
 * @return
 *  - pdTRUE if in ISR
 *  - pdFALSE otherwise
 */
بنیاد_Type_t x_پورٹ_اندر_Isr_Context(خالی);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortInIsrContext() internally
 */
خالی v_پورٹ_Assert_If_اندر_ISR(خالی);

/**
 * @brief Check if in ISR context from High priority ISRs
 *
 * - Called from High priority ISR
 * - Checks if the previous context (before high priority interrupt) was in ISR context (meaning low/med priority)
 *
 * @note [refactor-todo] Check if this should be inlined
 * @return
 *  - pdTRUE if in previous in ISR context
 *  - pdFALSE otherwise
 */
بنیاد_Type_t x_پورٹ_Interrupted_From_ISRContext(خالی);

/**
 * @brief Disable interrupts in a nested manner (meant to be called from ISRs)
 *
 * @warning Only applies to current CPU.
 * @return UBaseType_t Previous interrupt level
 */
ساکن اندرونی UBaseType_t x_پورٹ_سیٹ_مداخلت_نقاب_From_ISR(خالی);

/**
 * @brief Re-enable interrupts in a nested manner (meant to be called from ISRs)
 *
 * @warning Only applies to current CPU.
 * @param prev_level Previous interrupt level
 */
ساکن اندرونی خالی v_پورٹ_Clear_مداخلت_نقاب_From_ISR(UBaseType_t پچھلا_level);

/* ---------------------- Spinlocks ------------------------
 * - Modifications made to critical sections to support SMP
 * - See "Critical Sections & Disabling Interrupts" in docs/api-guides/freertos-smp.rst for more details
 * - Remark: For the ESP32, portENTER_CRITICAL and portENTER_CRITICAL_ISR both alias vPortEnterCritical, meaning that
 *           either function can be called both from ISR as well as task context. This is not standard FreeRTOS
 *           behavior; please keep this in mind if you need any compatibility with other FreeRTOS implementations.
 * @note [refactor-todo] Check if these comments are still true
 * ------------------------------------------------------ */

قسمتعریف spinlock_t                          پورٹ_MUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */
#تعریف_کرو پورٹ_MUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#تعریف_کرو پورٹ_MUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#تعریف_کرو پورٹ_MUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#تعریف_کرو پورٹ_MUX_TRY_تالا                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#تعریف_کرو پورٹ_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// ------------------ Critical Sections --------------------

/**
 * @brief Enter a SMP critical section with a timeout
 *
 * This function enters an SMP critical section by disabling interrupts then
 * taking a spinlock with a specified timeout.
 *
 * This function can be called in a nested manner.
 *
 * @note This function is made non-inline on purpose to reduce code size
 * @param mux Spinlock
 * @param timeout Timeout to wait for spinlock in number of CPU cycles.
 *                Use portMUX_NO_TIMEOUT to wait indefinitely
 *                Use portMUX_TRY_LOCK to only getting the spinlock a single time
 * @retval pdPASS Critical section entered (spinlock taken)
 * @retval pdFAIL If timed out waiting for spinlock (will not occur if using portMUX_NO_TIMEOUT)
 */
بنیاد_Type_t x_پورٹ_Enter_Critical_Timeout(پورٹ_MUX_TYPE *mux, بنیاد_Type_t timeout);

/**
 * @brief Enter a SMP critical section
 *
 * This function enters an SMP critical section by disabling interrupts then
 * taking a spinlock with an unlimited timeout.
 *
 * This function can be called in a nested manner
 *
 * @param[in] mux Spinlock
 */
ساکن اندرونی خالی __attribute__((always_inline)) v_پورٹ_Enter_Critical(پورٹ_MUX_TYPE *mux);

/**
 * @brief Exit a SMP critical section
 *
 * This function can be called in a nested manner. On the outer most level of nesting, this function will:
 *
 * - Release the spinlock
 * - Restore the previous interrupt level before the critical section was entered
 *
 * If still nesting, this function simply decrements a critical nesting count
 *
 * @note This function is made non-inline on purpose to reduce code size
 * @param[in] mux Spinlock
 */
خالی v_پورٹ_Exit_Critical(پورٹ_MUX_TYPE *mux);

#اگر CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
/**
 * @brief Claim thread-safe region start
 *        If claimed, vPortEnterCritical/vPortExitCritical on the current core are no-ops.
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
خالی x_پورٹ_Thread_محفوظ_Claim(خالی);

/**
 * @brief Claim thread-safe region end
 *        Restores normal port critical behavior
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
خالی x_پورٹ_Thread_محفوظ_Disclaim(خالی);
#اگر_ختم /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

/**
 * @brief FreeRTOS Compliant version of xPortEnterCriticalTimeout()
 *
 * Compliant version of xPortEnterCriticalTimeout() will ensure that this is
 * called from a task context only. An abort is called otherwise.
 *
 * @note This function is made non-inline on purpose to reduce code size
 *
 * @param mux Spinlock
 * @param timeout Timeout
 * @return BaseType_t
 */
بنیاد_Type_t x_پورٹ_Enter_Critical_Timeout_Compliance(پورٹ_MUX_TYPE *mux, بنیاد_Type_t timeout);

/**
 * @brief FreeRTOS compliant version of vPortEnterCritical()
 *
 * Compliant version of vPortEnterCritical() will ensure that this is
 * called from a task context only. An abort is called otherwise.
 *
 * @param[in] mux Spinlock
 */
ساکن اندرونی خالی __attribute__((always_inline)) v_پورٹ_Enter_Critical_Compliance(پورٹ_MUX_TYPE *mux);

/**
 * @brief FreeRTOS compliant version of vPortExitCritical()
 *
 * Compliant version of vPortExitCritical() will ensure that this is
 * called from a task context only. An abort is called otherwise.
 *
 * @note This function is made non-inline on purpose to reduce code size
 * @param[in] mux Spinlock
 */
خالی v_پورٹ_Exit_Critical_Compliance(پورٹ_MUX_TYPE *mux);

/**
 * @brief Safe version of enter critical timeout
 *
 * Safe version of enter critical will automatically select between
 * portTRY_ENTER_CRITICAL() and portTRY_ENTER_CRITICAL_ISR()
 *
 * @param mux Spinlock
 * @param timeout Timeout
 * @return BaseType_t
 */
ساکن اندرونی بنیاد_Type_t __attribute__((always_inline)) x_پورٹ_Enter_Critical_Timeout_محفوظ(پورٹ_MUX_TYPE *mux, بنیاد_Type_t timeout);

/**
 * @brief Safe version of enter critical
 *
 * Safe version of enter critical will automatically select between
 * portENTER_CRITICAL() and portENTER_CRITICAL_ISR()
 *
 * @param[in] mux Spinlock
 */
ساکن اندرونی خالی __attribute__((always_inline)) v_پورٹ_Enter_Critical_محفوظ(پورٹ_MUX_TYPE *mux);

/**
 * @brief Safe version of exit critical
 *
 * Safe version of enter critical will automatically select between
 * portEXIT_CRITICAL() and portEXIT_CRITICAL_ISR()
 *
 * @param[in] mux Spinlock
 */
ساکن اندرونی خالی __attribute__((always_inline)) v_پورٹ_Exit_Critical_محفوظ(پورٹ_MUX_TYPE *mux);

// ---------------------- Yielding -------------------------

/**
 * @brief Perform a solicited context switch
 *
 * - Defined in portasm.S
 *
 * @note [refactor-todo] The rest of ESP-IDF should call taskYield() instead
 */
خالی v_پورٹ_Yield( خالی );

/**
 * @brief Yields the other core
 *
 * - Send an interrupt to another core in order to make the task running on it yield for a higher-priority task.
 * - Can be used to yield current core as well
 *
 * @note [refactor-todo] Put this into private macros as its only called from task.c and is not public API
 * @param coreid ID of core to yield
 */
خالی v_پورٹ_Yield_Other_Core(بنیاد_Type_t coreid);

/**
 * @brief Checks if the current core can yield
 *
 * - A core cannot yield if its in an ISR or in a critical section
 *
 * @note [refactor-todo] See if this can be separated from port macro
 * @return true Core can yield
 * @return false Core cannot yield
 */
FORCE_INLINE_ATTR bool x_پورٹ_Can_Yield(خالی);

// ------------------- Hook Functions ----------------------

/**
 * @brief Hook function called on entry to tickless idle
 *
 * - Implemented in pm_impl.c
 *
 * @param xExpectedIdleTime Expected idle time
 */
خالی vApplicationSleep(TickType_t x_Expected_Idle_وقت);

// ----------------------- System --------------------------

/**
 * @brief Get the tick rate per second
 *
 * @deprecated This function will be removed in IDF 7.0. Use CONFIG_FREERTOS_HZ directly instead.
 * @note [refactor-todo] Remove this function in IDF 7.0 (IDF-14115)
 * @return uint32_t Tick rate in Hz
 */
uint32_t x_پورٹ_Get_Tick_شرح_Hz(خالی) __attribute__((deprecated("This function will be removed in IDF 7.0. Use CONFIG_FREERTOS_HZ directly instead.")));

/**
 * @brief Set a watchpoint to watch the last 32 bytes of the stack
 *
 * Callback to set a watchpoint on the end of the stack. Called every context switch to change the stack watchpoint
 * around.
 *
 * @param pxStackStart Pointer to the start of the stack
 */
خالی v_پورٹ_سیٹ_ڈھیر_Watchpoint( خالی *px_ڈھیر_آغاز );

/**
 * @brief Get the current core's ID
 *
 * @note [refactor-todo] IDF should call a FreeRTOS like macro instead of port function directly
 * @return BaseType_t Core ID
 */
FORCE_INLINE_ATTR بنیاد_Type_t x_پورٹ_Get_Core_ID(خالی);

// --------------------- TCB Cleanup -----------------------

/**
 * @brief TCB cleanup hook
 *
 * The portCLEAN_UP_TCB() macro is called in prvDeleteTCB() right before a
 * deleted task's memory is freed. We map that macro to this internal function
 * so that IDF FreeRTOS ports can inject some task pre-deletion operations.
 */
خالی v_پورٹ_TCBPre_Delete_Hook( خالی *pxTCB );


/* ------------------------------------------- FreeRTOS Porting Interface ----------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS
 * - Most come after forward declare as porting macros map to declared functions
 * - Maps to forward declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- System --------------------------

#اگر ( config_نمبر_کا_CORES > 1 )
    #تعریف_کرو پورٹ_GET_CORE_ID()       x_پورٹ_Get_Core_ID()
#ورنہ /* configNUMBER_OF_CORES > 1 */
    #تعریف_کرو پورٹ_GET_CORE_ID()       ((بنیاد_Type_t) 0);
#اگر_ختم /* configNUMBER_OF_CORES > 1 */

// --------------------- Interrupts ------------------------

/**
 * - Only applies to current core
 * - These cannot be nested. They should be used with a lot of care and cannot be called from interrupt level.
 *
 * @note [refactor-todo] replace XTOS_SET_INTLEVEL with more efficient version, if any?
 */
#تعریف_کرو پورٹ_DISABLE_INTERRUPTS()            کرو { XTOS_سیٹ_INTLEVEL(XCHAL_EXCM_LEVEL); portbenchmark_مداخلت_DISABLE(); } جب (0)
#تعریف_کرو پورٹ_ENABLE_INTERRUPTS()             کرو { portbenchmark_مداخلت_RESTORE(0); XTOS_سیٹ_INTLEVEL(0); } جب (0)

/**
 * ISR versions to enable/disable interrupts
 */
#تعریف_کرو پورٹ_سیٹ_مداخلت_نقاب_FROM_ISR()                   x_پورٹ_سیٹ_مداخلت_نقاب_From_ISR()
#تعریف_کرو پورٹ_CLEAR_مداخلت_نقاب_FROM_ISR(پچھلا_level)       v_پورٹ_Clear_مداخلت_نقاب_From_ISR(پچھلا_level)

/**
 * @brief Assert if in ISR context
 */
#تعریف_کرو پورٹ_ASSERT_IF_اندر_ISR() v_پورٹ_Assert_If_اندر_ISR()

/**
 * @brief Used by FreeRTOS functions to call the correct version of critical section API
 */
#اگر ( config_عدد_CORES > 1 )
#تعریف_کرو پورٹ_CHECK_IF_اندر_ISR()   x_پورٹ_اندر_Isr_Context()
#اگر_ختم

// ------------------ Critical Sections --------------------

/**
 * @brief FreeRTOS critical section macros
 *
 * - Added a spinlock argument for SMP
 * - Can be nested
 * - Compliance versions will assert if regular critical section API is used in ISR context
 * - Safe versions can be called from either contexts
 */
#اگر_تعریف_شدہ CONFIG_FREERTOS_CHECK_پورٹ_CRITICAL_COMPLIANCE
#تعریف_کرو پورٹ_TRY_ENTER_CRITICAL(mux, timeout)        x_پورٹ_Enter_Critical_Timeout_Compliance(mux, timeout)
#تعریف_کرو پورٹ_ENTER_CRITICAL(mux)                     v_پورٹ_Enter_Critical_Compliance(mux)
#تعریف_کرو پورٹ_EXIT_CRITICAL(mux)                      v_پورٹ_Exit_Critical_Compliance(mux)
#ورنہ
#تعریف_کرو پورٹ_TRY_ENTER_CRITICAL(mux, timeout)        x_پورٹ_Enter_Critical_Timeout(mux, timeout)
#تعریف_کرو پورٹ_ENTER_CRITICAL(mux)                     v_پورٹ_Enter_Critical(mux)
#تعریف_کرو پورٹ_EXIT_CRITICAL(mux)                      v_پورٹ_Exit_Critical(mux)
#اگر_ختم /* CONFIG_FREERTOS_CHECK_PORT_CRITICAL_COMPLIANCE */

#تعریف_کرو پورٹ_TRY_ENTER_CRITICAL_ISR(mux, timeout)    x_پورٹ_Enter_Critical_Timeout(mux, timeout)
#تعریف_کرو پورٹ_ENTER_CRITICAL_ISR(mux)                 v_پورٹ_Enter_Critical(mux)
#تعریف_کرو پورٹ_EXIT_CRITICAL_ISR(mux)                  v_پورٹ_Exit_Critical(mux)

#تعریف_کرو پورٹ_TRY_ENTER_CRITICAL_محفوظ(mux, timeout)   x_پورٹ_Enter_Critical_Timeout_محفوظ(mux, timeout)
#تعریف_کرو پورٹ_ENTER_CRITICAL_محفوظ(mux)                v_پورٹ_Enter_Critical_محفوظ(mux)
#تعریف_کرو پورٹ_EXIT_CRITICAL_محفوظ(mux)                 v_پورٹ_Exit_Critical_محفوظ(mux)

// ---------------------- Yielding -------------------------

#تعریف_کرو پورٹ_YIELD() v_پورٹ_Yield()

خارجی خالی frxt_تیاری_switch( خالی );     //Defined in portasm.S

#تعریف_کرو پورٹ_YIELD_FROM_ISR_NO_ARG() ({ \
    trace_ISR_EXIT_مقصد_SCHEDULER(); \
    frxt_تیاری_switch(); \
})
#تعریف_کرو پورٹ_YIELD_FROM_ISR_ARG(x_Higher_ترجیح_Task_Woken) ({ \
    اگر (x_Higher_ترجیح_Task_Woken == pdTRUE) { \
        trace_ISR_EXIT_مقصد_SCHEDULER(); \
        frxt_تیاری_switch(); \
    } \
})

/**
 * @note    The macro below could be used when passing a single argument, or without any argument,
 *          it was developed to support both usages of portYIELD inside of an ISR. Any other usage form
 *          might result in undesired behavior
 */
#اگر defined(__cplusplus) && (__cplusplus >  201703L)
#تعریف_کرو پورٹ_YIELD_FROM_ISR(...) انتخاب_MACRO_VA_ARG(پورٹ_YIELD_FROM_ISR_ARG, پورٹ_YIELD_FROM_ISR_NO_ARG __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#ورنہ
#تعریف_کرو پورٹ_YIELD_FROM_ISR(...) انتخاب_MACRO_VA_ARG(پورٹ_YIELD_FROM_ISR_ARG, پورٹ_YIELD_FROM_ISR_NO_ARG, ##__VA_ARGS__)(__VA_ARGS__)
#اگر_ختم

/* Yielding within an API call (when interrupts are off), means the yield should be delayed
   until interrupts are re-enabled.

   To do this, we use the "cross-core" interrupt as a trigger to yield on this core when interrupts are re-enabled.This
   is the same interrupt & code path which is used to trigger a yield between CPUs, although in this case the yield is
   happening on the same CPU.
*/
#تعریف_کرو پورٹ_YIELD_WITHIN_API() esp_crosscore_int_send_yield(x_پورٹ_Get_Core_ID())

#اگر ( config_نمبر_کا_CORES > 1 )
    #تعریف_کرو پورٹ_YIELD_CORE( xCoreID )     v_پورٹ_Yield_Other_Core( xCoreID )
#اگر_ختم /* configNUMBER_OF_CORES > 1 */

// ------------------- Hook Functions ----------------------

#تعریف_کرو پورٹ_SUPPRESS_TICKS_AND_SLEEP(idle_وقت) vApplicationSleep(idle_وقت)

// ------------------- Run Time Stats ----------------------

#تعریف_کرو پورٹ_CONFIGURE_TIMER_FOR_تسلسل_وقت_STATS()

#اگر ( CONFIG_FREERTOS_GENERATE_تسلسل_وقت_STATS )
config_تسلسل_وقت_شمار_TYPE x_پورٹ_Get_تسلسل_وقت_شمار_قدر( خالی );
#تعریف_کرو پورٹ_GET_تسلسل_وقت_شمار_قدر()        x_پورٹ_Get_تسلسل_وقت_شمار_قدر()
#اگر_ختم

// --------------------- TCB Cleanup -----------------------

#تعریف_کرو پورٹ_CLEAN_اوپر_TCB( pxTCB ) v_پورٹ_TCBPre_Delete_Hook( pxTCB )

// -------------- Optimized Task Selection -----------------

#اگر config_USE_پورٹ_OPTIMISED_TASK_SELECTION == 1
/* Check the configuration. */
#اگر( configMAX_PRIORITIES > 32 )
#غلطی config_USE_پورٹ_OPTIMISED_TASK_SELECTION can only be سیٹ مقصد 1 when configMAX_PRIORITIES is less than or equal مقصد 32.  It is very rare that a system requires مزید than 10 مقصد 15 different priorities as tasks that share a ترجیح will time قاش.
#اگر_ختم

/* Store/clear the ready priorities in a bit map. */
#تعریف_کرو پورٹ_ریکارڈ_READY_ترجیح( ux_ترجیح, uxReadyPriorities ) ( uxReadyPriorities ) |= ( 1UL << ( ux_ترجیح ) )
#تعریف_کرو پورٹ_RESET_READY_ترجیح( ux_ترجیح, uxReadyPriorities ) ( uxReadyPriorities ) &= ~( 1UL << ( ux_ترجیح ) )
#تعریف_کرو پورٹ_GET_HIGHEST_ترجیح( ux_چوٹی_ترجیح, uxReadyPriorities ) ux_چوٹی_ترجیح = ( 31 - __builtin_clz( ( uxReadyPriorities ) ) )
#اگر_ختم /* configUSE_PORT_OPTIMISED_TASK_SELECTION */



/* --------------------------------------------- Inline Implementations ------------------------------------------------
 * - Implementation of inline functions of the forward declares
 * - Should come after forward declare and FreeRTOS Porting interface, as implementation may use both.
 * - For implementation of non-inlined functions, see port.c
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

ساکن اندرونی UBaseType_t __attribute__((always_inline)) x_پورٹ_سیٹ_مداخلت_نقاب_From_ISR(خالی)
{
    UBaseType_t پچھلا_int_level = XTOS_سیٹ_INTLEVEL(XCHAL_EXCM_LEVEL);
    portbenchmark_مداخلت_DISABLE();
    واپس پچھلا_int_level;
}

ساکن اندرونی خالی __attribute__((always_inline)) v_پورٹ_Clear_مداخلت_نقاب_From_ISR(UBaseType_t پچھلا_level)
{
    portbenchmark_مداخلت_RESTORE(پچھلا_level);
    XTOS_RESTORE_JUST_INTLEVEL((صحیح) پچھلا_level);
}

// ------------------ Critical Sections --------------------

ساکن اندرونی خالی __attribute__((always_inline)) v_پورٹ_Enter_Critical(پورٹ_MUX_TYPE *mux)
{
    x_پورٹ_Enter_Critical_Timeout(mux, پورٹ_MUX_NO_TIMEOUT);
}

ساکن اندرونی خالی __attribute__((always_inline)) v_پورٹ_Enter_Critical_Compliance(پورٹ_MUX_TYPE *mux)
{
    x_پورٹ_Enter_Critical_Timeout_Compliance(mux, پورٹ_MUX_NO_TIMEOUT);
}

ساکن اندرونی بنیاد_Type_t __attribute__((always_inline)) x_پورٹ_Enter_Critical_Timeout_محفوظ(پورٹ_MUX_TYPE *mux, بنیاد_Type_t timeout)
{
    بنیاد_Type_t ret;
    اگر (x_پورٹ_اندر_Isr_Context()) {
        ret = پورٹ_TRY_ENTER_CRITICAL_ISR(mux, timeout);
    } ورنہ {
        ret = پورٹ_TRY_ENTER_CRITICAL(mux, timeout);
    }
    واپس ret;
}

ساکن اندرونی خالی __attribute__((always_inline)) v_پورٹ_Enter_Critical_محفوظ(پورٹ_MUX_TYPE *mux)
{
    x_پورٹ_Enter_Critical_Timeout_محفوظ(mux, پورٹ_MUX_NO_TIMEOUT);
}

ساکن اندرونی خالی __attribute__((always_inline)) v_پورٹ_Exit_Critical_محفوظ(پورٹ_MUX_TYPE *mux)
{
    اگر (x_پورٹ_اندر_Isr_Context()) {
        پورٹ_EXIT_CRITICAL_ISR(mux);
    } ورنہ {
        پورٹ_EXIT_CRITICAL(mux);
    }
}

// ---------------------- Yielding -------------------------

FORCE_INLINE_ATTR bool x_پورٹ_Can_Yield(خالی)
{
    uint32_t ps_reg = 0;

    //Get the current value of PS (processor status) register
    RSR(XT_REG_PS, ps_reg);

    /*
     * intlevel = (ps_reg & 0xf);
     * excm  = (ps_reg >> 4) & 0x1;
     * CINTLEVEL is max(excm * EXCMLEVEL, INTLEVEL), where EXCMLEVEL is 3.
     * However, just return true, only intlevel is zero.
     */

    واپس ((ps_reg & PS_INTLEVEL_نقاب) == 0);
}

// ----------------------- System --------------------------

FORCE_INLINE_ATTR بنیاد_Type_t x_پورٹ_Get_Core_ID(خالی)
{
    واپس (بنیاد_Type_t) esp_cpu_get_core_id();
}



/* ------------------------------------------------------ Misc ---------------------------------------------------------
 * - Miscellaneous porting macros
 * - These are not part of the FreeRTOS porting interface, but are used by other FreeRTOS dependent components
 * ------------------------------------------------------------------------------------------------------------------ */

// -------------------- Heap Related -----------------------

/**
 * @brief Checks if a given piece of memory can be used to store a FreeRTOS list
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a List
 * @return false Otherwise
 */
bool x_پورٹ_Check_Valid_List_Mem(ثابت خالی *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool x_پورٹ_Check_Valid_TCBMem(ثابت خالی *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_ڈھیر_Mem(ثابت خالی *ptr);

#تعریف_کرو پورٹ_VALID_LIST_MEM(ptr)     x_پورٹ_Check_Valid_List_Mem(ptr)
#تعریف_کرو پورٹ_VALID_TCB_MEM(ptr)      x_پورٹ_Check_Valid_TCBMem(ptr)
#تعریف_کرو پورٹ_VALID_ڈھیر_MEM(ptr)    x_Portcheck_Valid_ڈھیر_Mem(ptr)

// --------------------- App-Trace -------------------------

#اگر CONFIG_ESP_TRACE_ENABLE
خارجی متلون uint32_t پورٹ_switch_flag[پورٹ_عدد_PROCESSORS];
#تعریف_کرو os_task_switch_is_pended(_cpu_) (پورٹ_switch_flag[_cpu_])
#ورنہ
#تعریف_کرو os_task_switch_is_pended(_cpu_) (false)
#اگر_ختم

#اگر_تعریف_شدہ __cplusplus
}
#اگر_ختم

#اگر_ختم // __ASSEMBLER__

#اگر_ختم /* PORTMACRO_H */
