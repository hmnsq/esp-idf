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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ PORTMACRO_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PORTMACRO_H

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __ASSEMBLER__

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdarg.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <xtensa/config/core.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <xtensa/xtruntime.h>       /* required for XTOS_SET_INTLEVEL. [refactor-todo] add common intr functions to esp_hw_support */
#ਅਨ੍ਤਰ੍ਭਾਵ "xt_instr_macros.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "spinlock.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_private/crosscore_int.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_macros.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_attr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_cpu.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_memory_utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_newlib.h"             /* required for esp_reent_init() in tasks.c */
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_heap_caps.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_rom_sys.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_system.h"             /* required by esp_get_...() functions in portable.h. [refactor-todo] Update portable.h */
#ਅਨ੍ਤਰ੍ਭਾਵ "portbenchmark.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
ਬਾਹਰੀ "C" {
#ਜੇ_ਅੰਤ


/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come before first as they are used further down the file
 * ------------------------------------------------------------------------------------------------------------------ */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_CHAR                    int8_t
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_FLOAT                   ਦਸ਼ਮਲਵ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_DOUBLE                  ਦੁੱਗਣਾ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_LONG                    int32_t
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_SHORT                   int16_t
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਢੇਰ_TYPE              uint8_t
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਅਧਾਰ_TYPE               ਪੂਰਨ

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੋਰਟ_ਢੇਰ_TYPE              ਢੇਰ_Type_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੋਰਟ_ਅਧਾਰ_TYPE               ਅਧਾਰ_Type_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੋਰਟ_ਅਧਾਰ_TYPE      UBaseType_t;

#ਜੇ( config_USE_16_ਦੋਆਧਾਰੀ_ਅੰਕ_TICKS == 1 )
ਕਿਸਮਪਰਿਭਾਸ਼ਾ uint16_t TickType_t;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_MAX_DELAY ( TickType_t ) 0xffff
#ਨਹੀਂਤਾਂ
ਕਿਸਮਪਰਿਭਾਸ਼ਾ uint32_t TickType_t;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_MAX_DELAY ( TickType_t ) 0xffffffffUL
#ਜੇ_ਅੰਤ

/* Task function macros as described on the FreeRTOS.org WEB site. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TASK_FUNCTION_PROTO( vFunction, pvParameters )  ਖਾਲੀ vFunction( ਖਾਲੀ *pvParameters )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TASK_FUNCTION( vFunction, pvParameters )        ਖਾਲੀ vFunction( ਖਾਲੀ *pvParameters )



/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_CRITICAL_NESTING_ਵਿੱਚ_TCB     0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਢੇਰ_GROWTH                ( -1 )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TICK_PERIOD_MS              ( ( TickType_t ) 1000 / config_TICK_ਦਰ_HZ )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_BYTE_ALIGNMENT              16    // Xtensa Windowed ABI requires the stack pointer to always be 16-byte aligned. See "isa_rm.pdf 8.1.1 Windowed Register Usage and Stack Layout"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TICK_TYPE_IS_ATOMIC         1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_NOP()                       XT_NOP()



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
ਅਧਾਰ_Type_t x_ਪੋਰਟ_ਵਿੱਚ_Isr_Context(ਖਾਲੀ);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortInIsrContext() internally
 */
ਖਾਲੀ v_ਪੋਰਟ_Assert_If_ਵਿੱਚ_ISR(ਖਾਲੀ);

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
ਅਧਾਰ_Type_t x_ਪੋਰਟ_Interrupted_From_ISRContext(ਖਾਲੀ);

/**
 * @brief Disable interrupts in a nested manner (meant to be called from ISRs)
 *
 * @warning Only applies to current CPU.
 * @return UBaseType_t Previous interrupt level
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ UBaseType_t x_ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ_From_ISR(ਖਾਲੀ);

/**
 * @brief Re-enable interrupts in a nested manner (meant to be called from ISRs)
 *
 * @warning Only applies to current CPU.
 * @param prev_level Previous interrupt level
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ v_ਪੋਰਟ_Clear_ਰੁਕਾਵਟ_ਨਕਾਬ_From_ISR(UBaseType_t prev_level);

/* ---------------------- Spinlocks ------------------------
 * - Modifications made to critical sections to support SMP
 * - See "Critical Sections & Disabling Interrupts" in docs/api-guides/freertos-smp.rst for more details
 * - Remark: For the ESP32, portENTER_CRITICAL and portENTER_CRITICAL_ISR both alias vPortEnterCritical, meaning that
 *           either function can be called both from ISR as well as task context. This is not standard FreeRTOS
 *           behavior; please keep this in mind if you need any compatibility with other FreeRTOS implementations.
 * @note [refactor-todo] Check if these comments are still true
 * ------------------------------------------------------ */

ਕਿਸਮਪਰਿਭਾਸ਼ਾ spinlock_t                          ਪੋਰਟ_MUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_MUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_MUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_MUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_MUX_TRY_ਤਾਲਾ                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

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
ਅਧਾਰ_Type_t x_ਪੋਰਟ_Enter_Critical_Timeout(ਪੋਰਟ_MUX_TYPE *mux, ਅਧਾਰ_Type_t timeout);

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
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ __attribute__((always_inline)) v_ਪੋਰਟ_Enter_Critical(ਪੋਰਟ_MUX_TYPE *mux);

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
ਖਾਲੀ v_ਪੋਰਟ_Exit_Critical(ਪੋਰਟ_MUX_TYPE *mux);

#ਜੇ CONFIG_FREERTOS_ਪੋਰਟ_THREAD_ਸੁਰੱਖਿਅਤ_CLAIM
/**
 * @brief Claim thread-safe region start
 *        If claimed, vPortEnterCritical/vPortExitCritical on the current core are no-ops.
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
ਖਾਲੀ x_ਪੋਰਟ_Thread_ਸੁਰੱਖਿਅਤ_Claim(ਖਾਲੀ);

/**
 * @brief Claim thread-safe region end
 *        Restores normal port critical behavior
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
ਖਾਲੀ x_ਪੋਰਟ_Thread_ਸੁਰੱਖਿਅਤ_Disclaim(ਖਾਲੀ);
#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

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
ਅਧਾਰ_Type_t x_ਪੋਰਟ_Enter_Critical_Timeout_Compliance(ਪੋਰਟ_MUX_TYPE *mux, ਅਧਾਰ_Type_t timeout);

/**
 * @brief FreeRTOS compliant version of vPortEnterCritical()
 *
 * Compliant version of vPortEnterCritical() will ensure that this is
 * called from a task context only. An abort is called otherwise.
 *
 * @param[in] mux Spinlock
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ __attribute__((always_inline)) v_ਪੋਰਟ_Enter_Critical_Compliance(ਪੋਰਟ_MUX_TYPE *mux);

/**
 * @brief FreeRTOS compliant version of vPortExitCritical()
 *
 * Compliant version of vPortExitCritical() will ensure that this is
 * called from a task context only. An abort is called otherwise.
 *
 * @note This function is made non-inline on purpose to reduce code size
 * @param[in] mux Spinlock
 */
ਖਾਲੀ v_ਪੋਰਟ_Exit_Critical_Compliance(ਪੋਰਟ_MUX_TYPE *mux);

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
ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਧਾਰ_Type_t __attribute__((always_inline)) x_ਪੋਰਟ_Enter_Critical_Timeout_ਸੁਰੱਖਿਅਤ(ਪੋਰਟ_MUX_TYPE *mux, ਅਧਾਰ_Type_t timeout);

/**
 * @brief Safe version of enter critical
 *
 * Safe version of enter critical will automatically select between
 * portENTER_CRITICAL() and portENTER_CRITICAL_ISR()
 *
 * @param[in] mux Spinlock
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ __attribute__((always_inline)) v_ਪੋਰਟ_Enter_Critical_ਸੁਰੱਖਿਅਤ(ਪੋਰਟ_MUX_TYPE *mux);

/**
 * @brief Safe version of exit critical
 *
 * Safe version of enter critical will automatically select between
 * portEXIT_CRITICAL() and portEXIT_CRITICAL_ISR()
 *
 * @param[in] mux Spinlock
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ __attribute__((always_inline)) v_ਪੋਰਟ_Exit_Critical_ਸੁਰੱਖਿਅਤ(ਪੋਰਟ_MUX_TYPE *mux);

// ---------------------- Yielding -------------------------

/**
 * @brief Perform a solicited context switch
 *
 * - Defined in portasm.S
 *
 * @note [refactor-todo] The rest of ESP-IDF should call taskYield() instead
 */
ਖਾਲੀ v_ਪੋਰਟ_Yield( ਖਾਲੀ );

/**
 * @brief Yields the other core
 *
 * - Send an interrupt to another core in order to make the task running on it yield for a higher-priority task.
 * - Can be used to yield current core as well
 *
 * @note [refactor-todo] Put this into private macros as its only called from task.c and is not public API
 * @param coreid ID of core to yield
 */
ਖਾਲੀ v_ਪੋਰਟ_Yield_Other_Core(ਅਧਾਰ_Type_t coreid);

/**
 * @brief Checks if the current core can yield
 *
 * - A core cannot yield if its in an ISR or in a critical section
 *
 * @note [refactor-todo] See if this can be separated from port macro
 * @return true Core can yield
 * @return false Core cannot yield
 */
FORCE_INLINE_ATTR bool x_ਪੋਰਟ_Can_Yield(ਖਾਲੀ);

// ------------------- Hook Functions ----------------------

/**
 * @brief Hook function called on entry to tickless idle
 *
 * - Implemented in pm_impl.c
 *
 * @param xExpectedIdleTime Expected idle time
 */
ਖਾਲੀ vApplicationSleep(TickType_t x_Expected_Idle_ਸਮਾਂ);

// ----------------------- System --------------------------

/**
 * @brief Get the tick rate per second
 *
 * @deprecated This function will be removed in IDF 7.0. Use CONFIG_FREERTOS_HZ directly instead.
 * @note [refactor-todo] Remove this function in IDF 7.0 (IDF-14115)
 * @return uint32_t Tick rate in Hz
 */
uint32_t x_ਪੋਰਟ_Get_Tick_ਦਰ_Hz(ਖਾਲੀ) __attribute__((deprecated("This function will be removed in IDF 7.0. Use CONFIG_FREERTOS_HZ directly instead.")));

/**
 * @brief Set a watchpoint to watch the last 32 bytes of the stack
 *
 * Callback to set a watchpoint on the end of the stack. Called every context switch to change the stack watchpoint
 * around.
 *
 * @param pxStackStart Pointer to the start of the stack
 */
ਖਾਲੀ v_ਪੋਰਟ_ਸੈੱਟ_ਢੇਰ_Watchpoint( ਖਾਲੀ *px_ਢੇਰ_ਸ਼ੁਰੂ );

/**
 * @brief Get the current core's ID
 *
 * @note [refactor-todo] IDF should call a FreeRTOS like macro instead of port function directly
 * @return BaseType_t Core ID
 */
FORCE_INLINE_ATTR ਅਧਾਰ_Type_t x_ਪੋਰਟ_Get_Core_ID(ਖਾਲੀ);

// --------------------- TCB Cleanup -----------------------

/**
 * @brief TCB cleanup hook
 *
 * The portCLEAN_UP_TCB() macro is called in prvDeleteTCB() right before a
 * deleted task's memory is freed. We map that macro to this internal function
 * so that IDF FreeRTOS ports can inject some task pre-deletion operations.
 */
ਖਾਲੀ v_ਪੋਰਟ_TCBPre_Delete_Hook( ਖਾਲੀ *pxTCB );


/* ------------------------------------------- FreeRTOS Porting Interface ----------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS
 * - Most come after forward declare as porting macros map to declared functions
 * - Maps to forward declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- System --------------------------

#ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 )
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_GET_CORE_ID()       x_ਪੋਰਟ_Get_Core_ID()
#ਨਹੀਂਤਾਂ /* configNUMBER_OF_CORES > 1 */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_GET_CORE_ID()       ((ਅਧਾਰ_Type_t) 0);
#ਜੇ_ਅੰਤ /* configNUMBER_OF_CORES > 1 */

// --------------------- Interrupts ------------------------

/**
 * - Only applies to current core
 * - These cannot be nested. They should be used with a lot of care and cannot be called from interrupt level.
 *
 * @note [refactor-todo] replace XTOS_SET_INTLEVEL with more efficient version, if any?
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_DISABLE_INTERRUPTS()            ਕਰੋ { XTOS_ਸੈੱਟ_INTLEVEL(XCHAL_EXCM_LEVEL); portbenchmark_ਰੁਕਾਵਟ_DISABLE(); } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENABLE_INTERRUPTS()             ਕਰੋ { portbenchmark_ਰੁਕਾਵਟ_RESTORE(0); XTOS_ਸੈੱਟ_INTLEVEL(0); } ਜਦੋਂ (0)

/**
 * ISR versions to enable/disable interrupts
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR()                   x_ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ_From_ISR()
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_CLEAR_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR(prev_level)       v_ਪੋਰਟ_Clear_ਰੁਕਾਵਟ_ਨਕਾਬ_From_ISR(prev_level)

/**
 * @brief Assert if in ISR context
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ASSERT_IF_ਵਿੱਚ_ISR() v_ਪੋਰਟ_Assert_If_ਵਿੱਚ_ISR()

/**
 * @brief Used by FreeRTOS functions to call the correct version of critical section API
 */
#ਜੇ ( config_ਅੰਕ_CORES > 1 )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_CHECK_IF_ਵਿੱਚ_ISR()   x_ਪੋਰਟ_ਵਿੱਚ_Isr_Context()
#ਜੇ_ਅੰਤ

// ------------------ Critical Sections --------------------

/**
 * @brief FreeRTOS critical section macros
 *
 * - Added a spinlock argument for SMP
 * - Can be nested
 * - Compliance versions will assert if regular critical section API is used in ISR context
 * - Safe versions can be called from either contexts
 */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FREERTOS_CHECK_ਪੋਰਟ_CRITICAL_COMPLIANCE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TRY_ENTER_CRITICAL(mux, timeout)        x_ਪੋਰਟ_Enter_Critical_Timeout_Compliance(mux, timeout)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENTER_CRITICAL(mux)                     v_ਪੋਰਟ_Enter_Critical_Compliance(mux)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_EXIT_CRITICAL(mux)                      v_ਪੋਰਟ_Exit_Critical_Compliance(mux)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TRY_ENTER_CRITICAL(mux, timeout)        x_ਪੋਰਟ_Enter_Critical_Timeout(mux, timeout)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENTER_CRITICAL(mux)                     v_ਪੋਰਟ_Enter_Critical(mux)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_EXIT_CRITICAL(mux)                      v_ਪੋਰਟ_Exit_Critical(mux)
#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_CHECK_PORT_CRITICAL_COMPLIANCE */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TRY_ENTER_CRITICAL_ISR(mux, timeout)    x_ਪੋਰਟ_Enter_Critical_Timeout(mux, timeout)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENTER_CRITICAL_ISR(mux)                 v_ਪੋਰਟ_Enter_Critical(mux)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_EXIT_CRITICAL_ISR(mux)                  v_ਪੋਰਟ_Exit_Critical(mux)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TRY_ENTER_CRITICAL_ਸੁਰੱਖਿਅਤ(mux, timeout)   x_ਪੋਰਟ_Enter_Critical_Timeout_ਸੁਰੱਖਿਅਤ(mux, timeout)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENTER_CRITICAL_ਸੁਰੱਖਿਅਤ(mux)                v_ਪੋਰਟ_Enter_Critical_ਸੁਰੱਖਿਅਤ(mux)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_EXIT_CRITICAL_ਸੁਰੱਖਿਅਤ(mux)                 v_ਪੋਰਟ_Exit_Critical_ਸੁਰੱਖਿਅਤ(mux)

// ---------------------- Yielding -------------------------

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_YIELD() v_ਪੋਰਟ_Yield()

ਬਾਹਰੀ ਖਾਲੀ frxt_ਤਿਆਰੀ_switch( ਖਾਲੀ );     //Defined in portasm.S

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_YIELD_FROM_ISR_NO_ARG() ({ \
    trace_ISR_EXIT_ਮੰਜ਼ਿਲ_SCHEDULER(); \
    frxt_ਤਿਆਰੀ_switch(); \
})
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_YIELD_FROM_ISR_ARG(x_Higher_ਤਰਜੀਹ_Task_Woken) ({ \
    ਜੇ (x_Higher_ਤਰਜੀਹ_Task_Woken == pdTRUE) { \
        trace_ISR_EXIT_ਮੰਜ਼ਿਲ_SCHEDULER(); \
        frxt_ਤਿਆਰੀ_switch(); \
    } \
})

/**
 * @note    The macro below could be used when passing a single argument, or without any argument,
 *          it was developed to support both usages of portYIELD inside of an ISR. Any other usage form
 *          might result in undesired behavior
 */
#ਜੇ defined(__cplusplus) && (__cplusplus >  201703L)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_YIELD_FROM_ISR(...) ਚੁਣੋ_MACRO_VA_ARG(ਪੋਰਟ_YIELD_FROM_ISR_ARG, ਪੋਰਟ_YIELD_FROM_ISR_NO_ARG __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_YIELD_FROM_ISR(...) ਚੁਣੋ_MACRO_VA_ARG(ਪੋਰਟ_YIELD_FROM_ISR_ARG, ਪੋਰਟ_YIELD_FROM_ISR_NO_ARG, ##__VA_ARGS__)(__VA_ARGS__)
#ਜੇ_ਅੰਤ

/* Yielding within an API call (when interrupts are off), means the yield should be delayed
   until interrupts are re-enabled.

   To do this, we use the "cross-core" interrupt as a trigger to yield on this core when interrupts are re-enabled.This
   is the same interrupt & code path which is used to trigger a yield between CPUs, although in this case the yield is
   happening on the same CPU.
*/
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_YIELD_WITHIN_API() esp_crosscore_int_send_yield(x_ਪੋਰਟ_Get_Core_ID())

#ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 )
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_YIELD_CORE( xCoreID )     v_ਪੋਰਟ_Yield_Other_Core( xCoreID )
#ਜੇ_ਅੰਤ /* configNUMBER_OF_CORES > 1 */

// ------------------- Hook Functions ----------------------

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_SUPPRESS_TICKS_AND_SLEEP(idle_ਸਮਾਂ) vApplicationSleep(idle_ਸਮਾਂ)

// ------------------- Run Time Stats ----------------------

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_CONFIGURE_TIMER_FOR_ਲੜੀਵਾਰ_ਸਮਾਂ_STATS()

#ਜੇ ( CONFIG_FREERTOS_GENERATE_ਲੜੀਵਾਰ_ਸਮਾਂ_STATS )
config_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_TYPE x_ਪੋਰਟ_Get_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter_ਮੁੱਲ( ਖਾਲੀ );
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_GET_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_ਮੁੱਲ()        x_ਪੋਰਟ_Get_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter_ਮੁੱਲ()
#ਜੇ_ਅੰਤ

// --------------------- TCB Cleanup -----------------------

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_CLEAN_ਉੱਪਰ_TCB( pxTCB ) v_ਪੋਰਟ_TCBPre_Delete_Hook( pxTCB )

// -------------- Optimized Task Selection -----------------

#ਜੇ config_USE_ਪੋਰਟ_OPTIMISED_TASK_SELECTION == 1
/* Check the configuration. */
#ਜੇ( configMAX_PRIORITIES > 32 )
#ਗਲਤੀ config_USE_ਪੋਰਟ_OPTIMISED_TASK_SELECTION can only be ਸੈੱਟ ਮੰਜ਼ਿਲ 1 when configMAX_PRIORITIES is less than or equal ਮੰਜ਼ਿਲ 32.  It is very rare that a system requires ਹੋਰ than 10 ਮੰਜ਼ਿਲ 15 different priorities as tasks that share a ਤਰਜੀਹ will time ਟੁਕੜਾ.
#ਜੇ_ਅੰਤ

/* Store/clear the ready priorities in a bit map. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਰਿਕਾਰਡ_READY_ਤਰਜੀਹ( ux_ਤਰਜੀਹ, uxReadyPriorities ) ( uxReadyPriorities ) |= ( 1UL << ( ux_ਤਰਜੀਹ ) )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_RESET_READY_ਤਰਜੀਹ( ux_ਤਰਜੀਹ, uxReadyPriorities ) ( uxReadyPriorities ) &= ~( 1UL << ( ux_ਤਰਜੀਹ ) )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_GET_HIGHEST_ਤਰਜੀਹ( ux_ਸਿਖਰ_ਤਰਜੀਹ, uxReadyPriorities ) ux_ਸਿਖਰ_ਤਰਜੀਹ = ( 31 - __builtin_clz( ( uxReadyPriorities ) ) )
#ਜੇ_ਅੰਤ /* configUSE_PORT_OPTIMISED_TASK_SELECTION */



/* --------------------------------------------- Inline Implementations ------------------------------------------------
 * - Implementation of inline functions of the forward declares
 * - Should come after forward declare and FreeRTOS Porting interface, as implementation may use both.
 * - For implementation of non-inlined functions, see port.c
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

ਸਥਿਰ ਸਤਰਅੰਦਰ UBaseType_t __attribute__((always_inline)) x_ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ_From_ISR(ਖਾਲੀ)
{
    UBaseType_t prev_int_level = XTOS_ਸੈੱਟ_INTLEVEL(XCHAL_EXCM_LEVEL);
    portbenchmark_ਰੁਕਾਵਟ_DISABLE();
    ਵਾਪਸ prev_int_level;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ __attribute__((always_inline)) v_ਪੋਰਟ_Clear_ਰੁਕਾਵਟ_ਨਕਾਬ_From_ISR(UBaseType_t prev_level)
{
    portbenchmark_ਰੁਕਾਵਟ_RESTORE(prev_level);
    XTOS_RESTORE_JUST_INTLEVEL((ਪੂਰਨ) prev_level);
}

// ------------------ Critical Sections --------------------

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ __attribute__((always_inline)) v_ਪੋਰਟ_Enter_Critical(ਪੋਰਟ_MUX_TYPE *mux)
{
    x_ਪੋਰਟ_Enter_Critical_Timeout(mux, ਪੋਰਟ_MUX_NO_TIMEOUT);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ __attribute__((always_inline)) v_ਪੋਰਟ_Enter_Critical_Compliance(ਪੋਰਟ_MUX_TYPE *mux)
{
    x_ਪੋਰਟ_Enter_Critical_Timeout_Compliance(mux, ਪੋਰਟ_MUX_NO_TIMEOUT);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਧਾਰ_Type_t __attribute__((always_inline)) x_ਪੋਰਟ_Enter_Critical_Timeout_ਸੁਰੱਖਿਅਤ(ਪੋਰਟ_MUX_TYPE *mux, ਅਧਾਰ_Type_t timeout)
{
    ਅਧਾਰ_Type_t ret;
    ਜੇ (x_ਪੋਰਟ_ਵਿੱਚ_Isr_Context()) {
        ret = ਪੋਰਟ_TRY_ENTER_CRITICAL_ISR(mux, timeout);
    } ਨਹੀਂਤਾਂ {
        ret = ਪੋਰਟ_TRY_ENTER_CRITICAL(mux, timeout);
    }
    ਵਾਪਸ ret;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ __attribute__((always_inline)) v_ਪੋਰਟ_Enter_Critical_ਸੁਰੱਖਿਅਤ(ਪੋਰਟ_MUX_TYPE *mux)
{
    x_ਪੋਰਟ_Enter_Critical_Timeout_ਸੁਰੱਖਿਅਤ(mux, ਪੋਰਟ_MUX_NO_TIMEOUT);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ __attribute__((always_inline)) v_ਪੋਰਟ_Exit_Critical_ਸੁਰੱਖਿਅਤ(ਪੋਰਟ_MUX_TYPE *mux)
{
    ਜੇ (x_ਪੋਰਟ_ਵਿੱਚ_Isr_Context()) {
        ਪੋਰਟ_EXIT_CRITICAL_ISR(mux);
    } ਨਹੀਂਤਾਂ {
        ਪੋਰਟ_EXIT_CRITICAL(mux);
    }
}

// ---------------------- Yielding -------------------------

FORCE_INLINE_ATTR bool x_ਪੋਰਟ_Can_Yield(ਖਾਲੀ)
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

    ਵਾਪਸ ((ps_reg & PS_INTLEVEL_ਨਕਾਬ) == 0);
}

// ----------------------- System --------------------------

FORCE_INLINE_ATTR ਅਧਾਰ_Type_t x_ਪੋਰਟ_Get_Core_ID(ਖਾਲੀ)
{
    ਵਾਪਸ (ਅਧਾਰ_Type_t) esp_cpu_get_core_id();
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
bool x_ਪੋਰਟ_Check_Valid_List_Mem(ਅਚਲ ਖਾਲੀ *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool x_ਪੋਰਟ_Check_Valid_TCBMem(ਅਚਲ ਖਾਲੀ *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_ਢੇਰ_Mem(ਅਚਲ ਖਾਲੀ *ptr);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_VALID_LIST_MEM(ptr)     x_ਪੋਰਟ_Check_Valid_List_Mem(ptr)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_VALID_TCB_MEM(ptr)      x_ਪੋਰਟ_Check_Valid_TCBMem(ptr)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_VALID_ਢੇਰ_MEM(ptr)    x_Portcheck_Valid_ਢੇਰ_Mem(ptr)

// --------------------- App-Trace -------------------------

#ਜੇ CONFIG_ESP_TRACE_ENABLE
ਬਾਹਰੀ ਅਸਥਿਰ uint32_t ਪੋਰਟ_switch_flag[ਪੋਰਟ_ਅੰਕ_PROCESSORS];
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ os_task_switch_is_pended(_cpu_) (ਪੋਰਟ_switch_flag[_cpu_])
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ os_task_switch_is_pended(_cpu_) (false)
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ // __ASSEMBLER__

#ਜੇ_ਅੰਤ /* PORTMACRO_H */
