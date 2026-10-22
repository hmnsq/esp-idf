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

#ifndef PORTMACRO_H
#define PORTMACRO_H

#ifndef __ASSEMBLER__

#അന്തര്ഭാവ "sdkconfig.h"
#അന്തര്ഭാവ <stdint.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <stdbool.h>
#അന്തര്ഭാവ <stdarg.h>
#അന്തര്ഭാവ <xtensa/config/core.h>
#അന്തര്ഭാവ <xtensa/xtruntime.h>       /* required for XTOS_SET_INTLEVEL. [refactor-todo] add common intr functions to esp_hw_support */
#അന്തര്ഭാവ "xt_instr_macros.h"
#അന്തര്ഭാവ "spinlock.h"
#അന്തര്ഭാവ "esp_private/crosscore_int.h"
#അന്തര്ഭാവ "esp_macros.h"
#അന്തര്ഭാവ "esp_attr.h"
#അന്തര്ഭാവ "esp_cpu.h"
#അന്തര്ഭാവ "esp_memory_utils.h"
#അന്തര്ഭാവ "esp_newlib.h"             /* required for esp_reent_init() in tasks.c */
#അന്തര്ഭാവ "esp_heap_caps.h"
#അന്തര്ഭാവ "esp_rom_sys.h"
#അന്തര്ഭാവ "esp_system.h"             /* required by esp_get_...() functions in portable.h. [refactor-todo] Update portable.h */
#അന്തര്ഭാവ "portbenchmark.h"
#അന്തര്ഭാവ <limits.h>

#ifdef __cplusplus
ബാഹ്യ "C" {
#endif


/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come before first as they are used further down the file
 * ------------------------------------------------------------------------------------------------------------------ */

#define portCHAR                    int8_t
#define portFLOAT                   ദശാംശം
#define portDOUBLE                  ഇരട്ട
#define portLONG                    int32_t
#define portSHORT                   int16_t
#define port_അട്ടി_TYPE              uint8_t
#define portBASE_TYPE               പൂർണ്ണസംഖ്യ

തരനിർവചനം port_അട്ടി_TYPE              അട്ടി_Type_t;
തരനിർവചനം portBASE_TYPE               BaseType_t;
തരനിർവചനം unsigned portBASE_TYPE      UBaseType_t;

#എങ്കിൽ( configUSE_16_BIT_TICKS == 1 )
തരനിർവചനം uint16_t TickType_t;
#define portMAX_DELAY ( TickType_t ) 0xffff
#അല്ലെങ്കിൽ
തരനിർവചനം uint32_t TickType_t;
#define portMAX_DELAY ( TickType_t ) 0xffffffffUL
#endif

/* Task function macros as described on the FreeRTOS.org WEB site. */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters )  ശൂന്യം vFunction( ശൂന്യം *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters )        ശൂന്യം vFunction( ശൂന്യം *pvParameters )



/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#define portCRITICAL_NESTING_IN_TCB     0
#define port_അട്ടി_GROWTH                ( -1 )
#define portTICK_PERIOD_MS              ( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#define portBYTE_ALIGNMENT              16    // Xtensa Windowed ABI requires the stack pointer to always be 16-byte aligned. See "isa_rm.pdf 8.1.1 Windowed Register Usage and Stack Layout"
#define portTICK_TYPE_IS_ATOMIC         1
#define portNOP()                       XT_NOP()



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
BaseType_t xPortInIsrContext(ശൂന്യം);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortInIsrContext() internally
 */
ശൂന്യം vPortAssertIfInISR(ശൂന്യം);

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
BaseType_t xPortInterruptedFromISRContext(ശൂന്യം);

/**
 * @brief Disable interrupts in a nested manner (meant to be called from ISRs)
 *
 * @warning Only applies to current CPU.
 * @return UBaseType_t Previous interrupt level
 */
സ്ഥിതം വരിയിൽ UBaseType_t xPortSetInterruptMaskFromISR(ശൂന്യം);

/**
 * @brief Re-enable interrupts in a nested manner (meant to be called from ISRs)
 *
 * @warning Only applies to current CPU.
 * @param prev_level Previous interrupt level
 */
സ്ഥിതം വരിയിൽ ശൂന്യം vPortClearInterruptMaskFromISR(UBaseType_t prev_level);

/* ---------------------- Spinlocks ------------------------
 * - Modifications made to critical sections to support SMP
 * - See "Critical Sections & Disabling Interrupts" in docs/api-guides/freertos-smp.rst for more details
 * - Remark: For the ESP32, portENTER_CRITICAL and portENTER_CRITICAL_ISR both alias vPortEnterCritical, meaning that
 *           either function can be called both from ISR as well as task context. This is not standard FreeRTOS
 *           behavior; please keep this in mind if you need any compatibility with other FreeRTOS implementations.
 * @note [refactor-todo] Check if these comments are still true
 * ------------------------------------------------------ */

തരനിർവചനം spinlock_t                          portMUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */
#define portMUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#define portMUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#define portMUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#define portMUX_TRY_LOCK                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#define portMUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

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
BaseType_t xPortEnterCriticalTimeout(portMUX_TYPE *mux, BaseType_t timeout);

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
സ്ഥിതം വരിയിൽ ശൂന്യം __attribute__((always_inline)) vPortEnterCritical(portMUX_TYPE *mux);

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
ശൂന്യം vPortExitCritical(portMUX_TYPE *mux);

#എങ്കിൽ CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
/**
 * @brief Claim thread-safe region start
 *        If claimed, vPortEnterCritical/vPortExitCritical on the current core are no-ops.
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
ശൂന്യം xPortThreadSafeClaim(ശൂന്യം);

/**
 * @brief Claim thread-safe region end
 *        Restores normal port critical behavior
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
ശൂന്യം xPortThreadSafeDisclaim(ശൂന്യം);
#endif /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

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
BaseType_t xPortEnterCriticalTimeoutCompliance(portMUX_TYPE *mux, BaseType_t timeout);

/**
 * @brief FreeRTOS compliant version of vPortEnterCritical()
 *
 * Compliant version of vPortEnterCritical() will ensure that this is
 * called from a task context only. An abort is called otherwise.
 *
 * @param[in] mux Spinlock
 */
സ്ഥിതം വരിയിൽ ശൂന്യം __attribute__((always_inline)) vPortEnterCriticalCompliance(portMUX_TYPE *mux);

/**
 * @brief FreeRTOS compliant version of vPortExitCritical()
 *
 * Compliant version of vPortExitCritical() will ensure that this is
 * called from a task context only. An abort is called otherwise.
 *
 * @note This function is made non-inline on purpose to reduce code size
 * @param[in] mux Spinlock
 */
ശൂന്യം vPortExitCriticalCompliance(portMUX_TYPE *mux);

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
സ്ഥിതം വരിയിൽ BaseType_t __attribute__((always_inline)) xPortEnterCriticalTimeoutSafe(portMUX_TYPE *mux, BaseType_t timeout);

/**
 * @brief Safe version of enter critical
 *
 * Safe version of enter critical will automatically select between
 * portENTER_CRITICAL() and portENTER_CRITICAL_ISR()
 *
 * @param[in] mux Spinlock
 */
സ്ഥിതം വരിയിൽ ശൂന്യം __attribute__((always_inline)) vPortEnterCriticalSafe(portMUX_TYPE *mux);

/**
 * @brief Safe version of exit critical
 *
 * Safe version of enter critical will automatically select between
 * portEXIT_CRITICAL() and portEXIT_CRITICAL_ISR()
 *
 * @param[in] mux Spinlock
 */
സ്ഥിതം വരിയിൽ ശൂന്യം __attribute__((always_inline)) vPortExitCriticalSafe(portMUX_TYPE *mux);

// ---------------------- Yielding -------------------------

/**
 * @brief Perform a solicited context switch
 *
 * - Defined in portasm.S
 *
 * @note [refactor-todo] The rest of ESP-IDF should call taskYield() instead
 */
ശൂന്യം vPortYield( ശൂന്യം );

/**
 * @brief Yields the other core
 *
 * - Send an interrupt to another core in order to make the task running on it yield for a higher-priority task.
 * - Can be used to yield current core as well
 *
 * @note [refactor-todo] Put this into private macros as its only called from task.c and is not public API
 * @param coreid ID of core to yield
 */
ശൂന്യം vPortYieldOtherCore(BaseType_t coreid);

/**
 * @brief Checks if the current core can yield
 *
 * - A core cannot yield if its in an ISR or in a critical section
 *
 * @note [refactor-todo] See if this can be separated from port macro
 * @return true Core can yield
 * @return false Core cannot yield
 */
FORCE_INLINE_ATTR bool xPortCanYield(ശൂന്യം);

// ------------------- Hook Functions ----------------------

/**
 * @brief Hook function called on entry to tickless idle
 *
 * - Implemented in pm_impl.c
 *
 * @param xExpectedIdleTime Expected idle time
 */
ശൂന്യം vApplicationSleep(TickType_t xExpectedIdleTime);

// ----------------------- System --------------------------

/**
 * @brief Get the tick rate per second
 *
 * @deprecated This function will be removed in IDF 7.0. Use CONFIG_FREERTOS_HZ directly instead.
 * @note [refactor-todo] Remove this function in IDF 7.0 (IDF-14115)
 * @return uint32_t Tick rate in Hz
 */
uint32_t xPortGetTickRateHz(ശൂന്യം) __attribute__((deprecated("This function will be removed in IDF 7.0. Use CONFIG_FREERTOS_HZ directly instead.")));

/**
 * @brief Set a watchpoint to watch the last 32 bytes of the stack
 *
 * Callback to set a watchpoint on the end of the stack. Called every context switch to change the stack watchpoint
 * around.
 *
 * @param pxStackStart Pointer to the start of the stack
 */
ശൂന്യം v_Port_Set_അട്ടി_Watchpoint( ശൂന്യം *px_അട്ടി_Start );

/**
 * @brief Get the current core's ID
 *
 * @note [refactor-todo] IDF should call a FreeRTOS like macro instead of port function directly
 * @return BaseType_t Core ID
 */
FORCE_INLINE_ATTR BaseType_t xPortGetCoreID(ശൂന്യം);

// --------------------- TCB Cleanup -----------------------

/**
 * @brief TCB cleanup hook
 *
 * The portCLEAN_UP_TCB() macro is called in prvDeleteTCB() right before a
 * deleted task's memory is freed. We map that macro to this internal function
 * so that IDF FreeRTOS ports can inject some task pre-deletion operations.
 */
ശൂന്യം vPortTCBPreDeleteHook( ശൂന്യം *pxTCB );


/* ------------------------------------------- FreeRTOS Porting Interface ----------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS
 * - Most come after forward declare as porting macros map to declared functions
 * - Maps to forward declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- System --------------------------

#എങ്കിൽ ( config_സംഖ്യ_OF_CORES > 1 )
    #define portGET_CORE_ID()       xPortGetCoreID()
#അല്ലെങ്കിൽ /* configNUMBER_OF_CORES > 1 */
    #define portGET_CORE_ID()       ((BaseType_t) 0);
#endif /* configNUMBER_OF_CORES > 1 */

// --------------------- Interrupts ------------------------

/**
 * - Only applies to current core
 * - These cannot be nested. They should be used with a lot of care and cannot be called from interrupt level.
 *
 * @note [refactor-todo] replace XTOS_SET_INTLEVEL with more efficient version, if any?
 */
#define portDISABLE_INTERRUPTS()            ചെയ്യുക { XTOS_SET_INTLEVEL(XCHAL_EXCM_LEVEL); portbenchmarkINTERRUPT_DISABLE(); } ആകുമ്പോൾ (0)
#define portENABLE_INTERRUPTS()             ചെയ്യുക { portbenchmarkINTERRUPT_RESTORE(0); XTOS_SET_INTLEVEL(0); } ആകുമ്പോൾ (0)

/**
 * ISR versions to enable/disable interrupts
 */
#define portSET_INTERRUPT_MASK_FROM_ISR()                   xPortSetInterruptMaskFromISR()
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(prev_level)       vPortClearInterruptMaskFromISR(prev_level)

/**
 * @brief Assert if in ISR context
 */
#define portASSERT_IF_IN_ISR() vPortAssertIfInISR()

/**
 * @brief Used by FreeRTOS functions to call the correct version of critical section API
 */
#എങ്കിൽ ( config_അക്കം_CORES > 1 )
#define portCHECK_IF_IN_ISR()   xPortInIsrContext()
#endif

// ------------------ Critical Sections --------------------

/**
 * @brief FreeRTOS critical section macros
 *
 * - Added a spinlock argument for SMP
 * - Can be nested
 * - Compliance versions will assert if regular critical section API is used in ISR context
 * - Safe versions can be called from either contexts
 */
#ifdef CONFIG_FREERTOS_CHECK_PORT_CRITICAL_COMPLIANCE
#define portTRY_ENTER_CRITICAL(mux, timeout)        xPortEnterCriticalTimeoutCompliance(mux, timeout)
#define portENTER_CRITICAL(mux)                     vPortEnterCriticalCompliance(mux)
#define portEXIT_CRITICAL(mux)                      vPortExitCriticalCompliance(mux)
#അല്ലെങ്കിൽ
#define portTRY_ENTER_CRITICAL(mux, timeout)        xPortEnterCriticalTimeout(mux, timeout)
#define portENTER_CRITICAL(mux)                     vPortEnterCritical(mux)
#define portEXIT_CRITICAL(mux)                      vPortExitCritical(mux)
#endif /* CONFIG_FREERTOS_CHECK_PORT_CRITICAL_COMPLIANCE */

#define portTRY_ENTER_CRITICAL_ISR(mux, timeout)    xPortEnterCriticalTimeout(mux, timeout)
#define portENTER_CRITICAL_ISR(mux)                 vPortEnterCritical(mux)
#define portEXIT_CRITICAL_ISR(mux)                  vPortExitCritical(mux)

#define portTRY_ENTER_CRITICAL_SAFE(mux, timeout)   xPortEnterCriticalTimeoutSafe(mux, timeout)
#define portENTER_CRITICAL_SAFE(mux)                vPortEnterCriticalSafe(mux)
#define portEXIT_CRITICAL_SAFE(mux)                 vPortExitCriticalSafe(mux)

// ---------------------- Yielding -------------------------

#define portYIELD() vPortYield()

ബാഹ്യ ശൂന്യം _frxt_setup_switch( ശൂന്യം );     //Defined in portasm.S

#define portYIELD_FROM_ISR_NO_ARG() ({ \
    traceISR_EXIT_TO_SCHEDULER(); \
    _frxt_setup_switch(); \
})
#define portYIELD_FROM_ISR_ARG(xHigherPriorityTaskWoken) ({ \
    എങ്കിൽ (xHigherPriorityTaskWoken == pdTRUE) { \
        traceISR_EXIT_TO_SCHEDULER(); \
        _frxt_setup_switch(); \
    } \
})

/**
 * @note    The macro below could be used when passing a single argument, or without any argument,
 *          it was developed to support both usages of portYIELD inside of an ISR. Any other usage form
 *          might result in undesired behavior
 */
#എങ്കിൽ defined(__cplusplus) && (__cplusplus >  201703L)
#define portYIELD_FROM_ISR(...) CHOOSE_MACRO_VA_ARG(portYIELD_FROM_ISR_ARG, portYIELD_FROM_ISR_NO_ARG __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#അല്ലെങ്കിൽ
#define portYIELD_FROM_ISR(...) CHOOSE_MACRO_VA_ARG(portYIELD_FROM_ISR_ARG, portYIELD_FROM_ISR_NO_ARG, ##__VA_ARGS__)(__VA_ARGS__)
#endif

/* Yielding within an API call (when interrupts are off), means the yield should be delayed
   until interrupts are re-enabled.

   To do this, we use the "cross-core" interrupt as a trigger to yield on this core when interrupts are re-enabled.This
   is the same interrupt & code path which is used to trigger a yield between CPUs, although in this case the yield is
   happening on the same CPU.
*/
#define portYIELD_WITHIN_API() esp_crosscore_int_send_yield(xPortGetCoreID())

#എങ്കിൽ ( config_സംഖ്യ_OF_CORES > 1 )
    #define portYIELD_CORE( xCoreID )     vPortYieldOtherCore( xCoreID )
#endif /* configNUMBER_OF_CORES > 1 */

// ------------------- Hook Functions ----------------------

#define portSUPPRESS_TICKS_AND_SLEEP(idleTime) vApplicationSleep(idleTime)

// ------------------- Run Time Stats ----------------------

#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()

#എങ്കിൽ ( CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS )
configRUN_TIME_COUNTER_TYPE x_Port_Get_Run_Time_Counter_മൂല്യം( ശൂന്യം );
#define port_GET_RUN_TIME_COUNTER_മൂല്യം()        x_Port_Get_Run_Time_Counter_മൂല്യം()
#endif

// --------------------- TCB Cleanup -----------------------

#define portCLEAN_UP_TCB( pxTCB ) vPortTCBPreDeleteHook( pxTCB )

// -------------- Optimized Task Selection -----------------

#എങ്കിൽ configUSE_PORT_OPTIMISED_TASK_SELECTION == 1
/* Check the configuration. */
#എങ്കിൽ( configMAX_PRIORITIES > 32 )
#error configUSE_PORT_OPTIMISED_TASK_SELECTION can only be set to 1 when configMAX_PRIORITIES is less than or equal to 32.  It is very rare that a system requires more than 10 to 15 different priorities as tasks that share a priority will time slice.
#endif

/* Store/clear the ready priorities in a bit map. */
#define portRECORD_READY_PRIORITY( uxPriority, uxReadyPriorities ) ( uxReadyPriorities ) |= ( 1UL << ( uxPriority ) )
#define portRESET_READY_PRIORITY( uxPriority, uxReadyPriorities ) ( uxReadyPriorities ) &= ~( 1UL << ( uxPriority ) )
#define portGET_HIGHEST_PRIORITY( ux_മുകൾ_Priority, uxReadyPriorities ) ux_മുകൾ_Priority = ( 31 - __builtin_clz( ( uxReadyPriorities ) ) )
#endif /* configUSE_PORT_OPTIMISED_TASK_SELECTION */



/* --------------------------------------------- Inline Implementations ------------------------------------------------
 * - Implementation of inline functions of the forward declares
 * - Should come after forward declare and FreeRTOS Porting interface, as implementation may use both.
 * - For implementation of non-inlined functions, see port.c
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

സ്ഥിതം വരിയിൽ UBaseType_t __attribute__((always_inline)) xPortSetInterruptMaskFromISR(ശൂന്യം)
{
    UBaseType_t prev_int_level = XTOS_SET_INTLEVEL(XCHAL_EXCM_LEVEL);
    portbenchmarkINTERRUPT_DISABLE();
    തിരികെ prev_int_level;
}

സ്ഥിതം വരിയിൽ ശൂന്യം __attribute__((always_inline)) vPortClearInterruptMaskFromISR(UBaseType_t prev_level)
{
    portbenchmarkINTERRUPT_RESTORE(prev_level);
    XTOS_RESTORE_JUST_INTLEVEL((പൂർണ്ണസംഖ്യ) prev_level);
}

// ------------------ Critical Sections --------------------

സ്ഥിതം വരിയിൽ ശൂന്യം __attribute__((always_inline)) vPortEnterCritical(portMUX_TYPE *mux)
{
    xPortEnterCriticalTimeout(mux, portMUX_NO_TIMEOUT);
}

സ്ഥിതം വരിയിൽ ശൂന്യം __attribute__((always_inline)) vPortEnterCriticalCompliance(portMUX_TYPE *mux)
{
    xPortEnterCriticalTimeoutCompliance(mux, portMUX_NO_TIMEOUT);
}

സ്ഥിതം വരിയിൽ BaseType_t __attribute__((always_inline)) xPortEnterCriticalTimeoutSafe(portMUX_TYPE *mux, BaseType_t timeout)
{
    BaseType_t ret;
    എങ്കിൽ (xPortInIsrContext()) {
        ret = portTRY_ENTER_CRITICAL_ISR(mux, timeout);
    } അല്ലെങ്കിൽ {
        ret = portTRY_ENTER_CRITICAL(mux, timeout);
    }
    തിരികെ ret;
}

സ്ഥിതം വരിയിൽ ശൂന്യം __attribute__((always_inline)) vPortEnterCriticalSafe(portMUX_TYPE *mux)
{
    xPortEnterCriticalTimeoutSafe(mux, portMUX_NO_TIMEOUT);
}

സ്ഥിതം വരിയിൽ ശൂന്യം __attribute__((always_inline)) vPortExitCriticalSafe(portMUX_TYPE *mux)
{
    എങ്കിൽ (xPortInIsrContext()) {
        portEXIT_CRITICAL_ISR(mux);
    } അല്ലെങ്കിൽ {
        portEXIT_CRITICAL(mux);
    }
}

// ---------------------- Yielding -------------------------

FORCE_INLINE_ATTR bool xPortCanYield(ശൂന്യം)
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

    തിരികെ ((ps_reg & PS_INTLEVEL_MASK) == 0);
}

// ----------------------- System --------------------------

FORCE_INLINE_ATTR BaseType_t xPortGetCoreID(ശൂന്യം)
{
    തിരികെ (BaseType_t) esp_cpu_get_core_id();
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
bool xPortCheckValidListMem(സ്ഥിരം ശൂന്യം *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool xPortCheckValidTCBMem(സ്ഥിരം ശൂന്യം *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_അട്ടി_Mem(സ്ഥിരം ശൂന്യം *ptr);

#define portVALID_LIST_MEM(ptr)     xPortCheckValidListMem(ptr)
#define portVALID_TCB_MEM(ptr)      xPortCheckValidTCBMem(ptr)
#define port_VALID_അട്ടി_MEM(ptr)    x_Portcheck_Valid_അട്ടി_Mem(ptr)

// --------------------- App-Trace -------------------------

#എങ്കിൽ CONFIG_ESP_TRACE_ENABLE
ബാഹ്യ അസ്ഥിര uint32_t port_switch_flag[port_അക്കം_PROCESSORS];
#define os_task_switch_is_pended(_cpu_) (port_switch_flag[_cpu_])
#അല്ലെങ്കിൽ
#define os_task_switch_is_pended(_cpu_) (false)
#endif

#ifdef __cplusplus
}
#endif

#endif // __ASSEMBLER__

#endif /* PORTMACRO_H */
