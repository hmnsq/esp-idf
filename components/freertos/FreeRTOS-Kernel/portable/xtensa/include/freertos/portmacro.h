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

#अगर_अघोषित PORTMACRO_H
#घोषणा PORTMACRO_H

#अगर_अघोषित __ASSEMBLER__

#समावेश "sdkconfig.h"
#समावेश <stdint.h>
#समावेश <stdlib.h>
#समावेश <stdbool.h>
#समावेश <stdarg.h>
#समावेश <xtensa/config/core.h>
#समावेश <xtensa/xtruntime.h>       /* required for XTOS_SET_INTLEVEL. [refactor-todo] add common intr functions to esp_hw_support */
#समावेश "xt_instr_macros.h"
#समावेश "spinlock.h"
#समावेश "esp_private/crosscore_int.h"
#समावेश "esp_macros.h"
#समावेश "esp_attr.h"
#समावेश "esp_cpu.h"
#समावेश "esp_memory_utils.h"
#समावेश "esp_newlib.h"             /* required for esp_reent_init() in tasks.c */
#समावेश "esp_heap_caps.h"
#समावेश "esp_rom_sys.h"
#समावेश "esp_system.h"             /* required by esp_get_...() functions in portable.h. [refactor-todo] Update portable.h */
#समावेश "portbenchmark.h"
#समावेश <limits.h>

#अगर_घोषित __cplusplus
बाह्य "C" {
#पूर्ण_अगर


/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come before first as they are used further down the file
 * ------------------------------------------------------------------------------------------------------------------ */

#घोषणा portCHAR                    int8_t
#घोषणा portFLOAT                   भग्न
#घोषणा portDOUBLE                  द्विगुन
#घोषणा portLONG                    int32_t
#घोषणा portSHORT                   int16_t
#घोषणा port_ढेर_TYPE              uint8_t
#घोषणा portBASE_TYPE               पूर्णांक

प्रकार port_ढेर_TYPE              ढेर_Type_t;
प्रकार portBASE_TYPE               BaseType_t;
प्रकार अचिन्हित portBASE_TYPE      UBaseType_t;

#अगर( configUSE_16_BIT_TICKS == 1 )
प्रकार uint16_t TickType_t;
#घोषणा portMAX_DELAY ( TickType_t ) 0xffff
#अन्यथा
प्रकार uint32_t TickType_t;
#घोषणा portMAX_DELAY ( TickType_t ) 0xffffffffUL
#पूर्ण_अगर

/* Task function macros as described on the FreeRTOS.org WEB site. */
#घोषणा portTASK_FUNCTION_PROTO( vFunction, pvParameters )  व्योम vFunction( व्योम *pvParameters )
#घोषणा portTASK_FUNCTION( vFunction, pvParameters )        व्योम vFunction( व्योम *pvParameters )



/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#घोषणा portCRITICAL_NESTING_IN_TCB     0
#घोषणा port_ढेर_GROWTH                ( -1 )
#घोषणा portTICK_PERIOD_MS              ( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#घोषणा portBYTE_ALIGNMENT              16    // Xtensa Windowed ABI requires the stack pointer to always be 16-byte aligned. See "isa_rm.pdf 8.1.1 Windowed Register Usage and Stack Layout"
#घोषणा portTICK_TYPE_IS_ATOMIC         1
#घोषणा portNOP()                       XT_NOP()



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
BaseType_t xPortInIsrContext(व्योम);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortInIsrContext() internally
 */
व्योम vPortAssertIfInISR(व्योम);

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
BaseType_t xPortInterruptedFromISRContext(व्योम);

/**
 * @brief Disable interrupts in a nested manner (meant to be called from ISRs)
 *
 * @warning Only applies to current CPU.
 * @return UBaseType_t Previous interrupt level
 */
अटल अंतरभूत UBaseType_t xPortSetInterruptMaskFromISR(व्योम);

/**
 * @brief Re-enable interrupts in a nested manner (meant to be called from ISRs)
 *
 * @warning Only applies to current CPU.
 * @param prev_level Previous interrupt level
 */
अटल अंतरभूत व्योम vPortClearInterruptMaskFromISR(UBaseType_t पूर्व_level);

/* ---------------------- Spinlocks ------------------------
 * - Modifications made to critical sections to support SMP
 * - See "Critical Sections & Disabling Interrupts" in docs/api-guides/freertos-smp.rst for more details
 * - Remark: For the ESP32, portENTER_CRITICAL and portENTER_CRITICAL_ISR both alias vPortEnterCritical, meaning that
 *           either function can be called both from ISR as well as task context. This is not standard FreeRTOS
 *           behavior; please keep this in mind if you need any compatibility with other FreeRTOS implementations.
 * @note [refactor-todo] Check if these comments are still true
 * ------------------------------------------------------ */

प्रकार spinlock_t                          portMUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */
#घोषणा portMUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#घोषणा portMUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#घोषणा portMUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#घोषणा portMUX_TRY_LOCK                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#घोषणा portMUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

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
अटल अंतरभूत व्योम __attribute__((always_inline)) vPortEnterCritical(portMUX_TYPE *mux);

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
व्योम vPortExitCritical(portMUX_TYPE *mux);

#अगर CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
/**
 * @brief Claim thread-safe region start
 *        If claimed, vPortEnterCritical/vPortExitCritical on the current core are no-ops.
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
व्योम xPortThreadSafeClaim(व्योम);

/**
 * @brief Claim thread-safe region end
 *        Restores normal port critical behavior
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
व्योम xPortThreadSafeDisclaim(व्योम);
#पूर्ण_अगर /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

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
अटल अंतरभूत व्योम __attribute__((always_inline)) vPortEnterCriticalCompliance(portMUX_TYPE *mux);

/**
 * @brief FreeRTOS compliant version of vPortExitCritical()
 *
 * Compliant version of vPortExitCritical() will ensure that this is
 * called from a task context only. An abort is called otherwise.
 *
 * @note This function is made non-inline on purpose to reduce code size
 * @param[in] mux Spinlock
 */
व्योम vPortExitCriticalCompliance(portMUX_TYPE *mux);

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
अटल अंतरभूत BaseType_t __attribute__((always_inline)) xPortEnterCriticalTimeoutSafe(portMUX_TYPE *mux, BaseType_t timeout);

/**
 * @brief Safe version of enter critical
 *
 * Safe version of enter critical will automatically select between
 * portENTER_CRITICAL() and portENTER_CRITICAL_ISR()
 *
 * @param[in] mux Spinlock
 */
अटल अंतरभूत व्योम __attribute__((always_inline)) vPortEnterCriticalSafe(portMUX_TYPE *mux);

/**
 * @brief Safe version of exit critical
 *
 * Safe version of enter critical will automatically select between
 * portEXIT_CRITICAL() and portEXIT_CRITICAL_ISR()
 *
 * @param[in] mux Spinlock
 */
अटल अंतरभूत व्योम __attribute__((always_inline)) vPortExitCriticalSafe(portMUX_TYPE *mux);

// ---------------------- Yielding -------------------------

/**
 * @brief Perform a solicited context switch
 *
 * - Defined in portasm.S
 *
 * @note [refactor-todo] The rest of ESP-IDF should call taskYield() instead
 */
व्योम vPortYield( व्योम );

/**
 * @brief Yields the other core
 *
 * - Send an interrupt to another core in order to make the task running on it yield for a higher-priority task.
 * - Can be used to yield current core as well
 *
 * @note [refactor-todo] Put this into private macros as its only called from task.c and is not public API
 * @param coreid ID of core to yield
 */
व्योम vPortYieldOtherCore(BaseType_t coreid);

/**
 * @brief Checks if the current core can yield
 *
 * - A core cannot yield if its in an ISR or in a critical section
 *
 * @note [refactor-todo] See if this can be separated from port macro
 * @return true Core can yield
 * @return false Core cannot yield
 */
FORCE_INLINE_ATTR bool xPortCanYield(व्योम);

// ------------------- Hook Functions ----------------------

/**
 * @brief Hook function called on entry to tickless idle
 *
 * - Implemented in pm_impl.c
 *
 * @param xExpectedIdleTime Expected idle time
 */
व्योम vApplicationSleep(TickType_t xExpectedIdleTime);

// ----------------------- System --------------------------

/**
 * @brief Get the tick rate per second
 *
 * @deprecated This function will be removed in IDF 7.0. Use CONFIG_FREERTOS_HZ directly instead.
 * @note [refactor-todo] Remove this function in IDF 7.0 (IDF-14115)
 * @return uint32_t Tick rate in Hz
 */
uint32_t xPortGetTickRateHz(व्योम) __attribute__((deprecated("This function will be removed in IDF 7.0. Use CONFIG_FREERTOS_HZ directly instead.")));

/**
 * @brief Set a watchpoint to watch the last 32 bytes of the stack
 *
 * Callback to set a watchpoint on the end of the stack. Called every context switch to change the stack watchpoint
 * around.
 *
 * @param pxStackStart Pointer to the start of the stack
 */
व्योम v_Port_Set_ढेर_Watchpoint( व्योम *px_ढेर_Start );

/**
 * @brief Get the current core's ID
 *
 * @note [refactor-todo] IDF should call a FreeRTOS like macro instead of port function directly
 * @return BaseType_t Core ID
 */
FORCE_INLINE_ATTR BaseType_t xPortGetCoreID(व्योम);

// --------------------- TCB Cleanup -----------------------

/**
 * @brief TCB cleanup hook
 *
 * The portCLEAN_UP_TCB() macro is called in prvDeleteTCB() right before a
 * deleted task's memory is freed. We map that macro to this internal function
 * so that IDF FreeRTOS ports can inject some task pre-deletion operations.
 */
व्योम vPortTCBPreDeleteHook( व्योम *pxTCB );


/* ------------------------------------------- FreeRTOS Porting Interface ----------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS
 * - Most come after forward declare as porting macros map to declared functions
 * - Maps to forward declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- System --------------------------

#अगर ( config_संख्या_OF_CORES > 1 )
    #घोषणा portGET_CORE_ID()       xPortGetCoreID()
#अन्यथा /* configNUMBER_OF_CORES > 1 */
    #घोषणा portGET_CORE_ID()       ((BaseType_t) 0);
#पूर्ण_अगर /* configNUMBER_OF_CORES > 1 */

// --------------------- Interrupts ------------------------

/**
 * - Only applies to current core
 * - These cannot be nested. They should be used with a lot of care and cannot be called from interrupt level.
 *
 * @note [refactor-todo] replace XTOS_SET_INTLEVEL with more efficient version, if any?
 */
#घोषणा portDISABLE_INTERRUPTS()            करो { XTOS_SET_INTLEVEL(XCHAL_EXCM_LEVEL); portbenchmarkINTERRUPT_DISABLE(); } जबतक (0)
#घोषणा portENABLE_INTERRUPTS()             करो { portbenchmarkINTERRUPT_RESTORE(0); XTOS_SET_INTLEVEL(0); } जबतक (0)

/**
 * ISR versions to enable/disable interrupts
 */
#घोषणा portSET_INTERRUPT_MASK_FROM_ISR()                   xPortSetInterruptMaskFromISR()
#घोषणा portCLEAR_INTERRUPT_MASK_FROM_ISR(पूर्व_level)       vPortClearInterruptMaskFromISR(पूर्व_level)

/**
 * @brief Assert if in ISR context
 */
#घोषणा portASSERT_IF_IN_ISR() vPortAssertIfInISR()

/**
 * @brief Used by FreeRTOS functions to call the correct version of critical section API
 */
#अगर ( config_अंक_CORES > 1 )
#घोषणा portCHECK_IF_IN_ISR()   xPortInIsrContext()
#पूर्ण_अगर

// ------------------ Critical Sections --------------------

/**
 * @brief FreeRTOS critical section macros
 *
 * - Added a spinlock argument for SMP
 * - Can be nested
 * - Compliance versions will assert if regular critical section API is used in ISR context
 * - Safe versions can be called from either contexts
 */
#अगर_घोषित CONFIG_FREERTOS_CHECK_PORT_CRITICAL_COMPLIANCE
#घोषणा portTRY_ENTER_CRITICAL(mux, timeout)        xPortEnterCriticalTimeoutCompliance(mux, timeout)
#घोषणा portENTER_CRITICAL(mux)                     vPortEnterCriticalCompliance(mux)
#घोषणा portEXIT_CRITICAL(mux)                      vPortExitCriticalCompliance(mux)
#अन्यथा
#घोषणा portTRY_ENTER_CRITICAL(mux, timeout)        xPortEnterCriticalTimeout(mux, timeout)
#घोषणा portENTER_CRITICAL(mux)                     vPortEnterCritical(mux)
#घोषणा portEXIT_CRITICAL(mux)                      vPortExitCritical(mux)
#पूर्ण_अगर /* CONFIG_FREERTOS_CHECK_PORT_CRITICAL_COMPLIANCE */

#घोषणा portTRY_ENTER_CRITICAL_ISR(mux, timeout)    xPortEnterCriticalTimeout(mux, timeout)
#घोषणा portENTER_CRITICAL_ISR(mux)                 vPortEnterCritical(mux)
#घोषणा portEXIT_CRITICAL_ISR(mux)                  vPortExitCritical(mux)

#घोषणा portTRY_ENTER_CRITICAL_SAFE(mux, timeout)   xPortEnterCriticalTimeoutSafe(mux, timeout)
#घोषणा portENTER_CRITICAL_SAFE(mux)                vPortEnterCriticalSafe(mux)
#घोषणा portEXIT_CRITICAL_SAFE(mux)                 vPortExitCriticalSafe(mux)

// ---------------------- Yielding -------------------------

#घोषणा portYIELD() vPortYield()

बाह्य व्योम _frxt_setup_switch( व्योम );     //Defined in portasm.S

#घोषणा portYIELD_FROM_ISR_NO_ARG() ({ \
    traceISR_EXIT_TO_SCHEDULER(); \
    _frxt_setup_switch(); \
})
#घोषणा portYIELD_FROM_ISR_ARG(xHigherPriorityTaskWoken) ({ \
    अगर (xHigherPriorityTaskWoken == pdTRUE) { \
        traceISR_EXIT_TO_SCHEDULER(); \
        _frxt_setup_switch(); \
    } \
})

/**
 * @note    The macro below could be used when passing a single argument, or without any argument,
 *          it was developed to support both usages of portYIELD inside of an ISR. Any other usage form
 *          might result in undesired behavior
 */
#अगर defined(__cplusplus) && (__cplusplus >  201703L)
#घोषणा portYIELD_FROM_ISR(...) CHOOSE_MACRO_VA_ARG(portYIELD_FROM_ISR_ARG, portYIELD_FROM_ISR_NO_ARG __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#अन्यथा
#घोषणा portYIELD_FROM_ISR(...) CHOOSE_MACRO_VA_ARG(portYIELD_FROM_ISR_ARG, portYIELD_FROM_ISR_NO_ARG, ##__VA_ARGS__)(__VA_ARGS__)
#पूर्ण_अगर

/* Yielding within an API call (when interrupts are off), means the yield should be delayed
   until interrupts are re-enabled.

   To do this, we use the "cross-core" interrupt as a trigger to yield on this core when interrupts are re-enabled.This
   is the same interrupt & code path which is used to trigger a yield between CPUs, although in this case the yield is
   happening on the same CPU.
*/
#घोषणा portYIELD_WITHIN_API() esp_crosscore_int_send_yield(xPortGetCoreID())

#अगर ( config_संख्या_OF_CORES > 1 )
    #घोषणा portYIELD_CORE( xCoreID )     vPortYieldOtherCore( xCoreID )
#पूर्ण_अगर /* configNUMBER_OF_CORES > 1 */

// ------------------- Hook Functions ----------------------

#घोषणा portSUPPRESS_TICKS_AND_SLEEP(idleTime) vApplicationSleep(idleTime)

// ------------------- Run Time Stats ----------------------

#घोषणा portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()

#अगर ( CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS )
config_RUN_TIME_गणक_TYPE x_Port_Get_Run_Time_गणक_मूल्य( व्योम );
#घोषणा port_GET_RUN_TIME_गणक_मूल्य()        x_Port_Get_Run_Time_गणक_मूल्य()
#पूर्ण_अगर

// --------------------- TCB Cleanup -----------------------

#घोषणा portCLEAN_UP_TCB( pxTCB ) vPortTCBPreDeleteHook( pxTCB )

// -------------- Optimized Task Selection -----------------

#अगर configUSE_PORT_OPTIMISED_TASK_SELECTION == 1
/* Check the configuration. */
#अगर( configMAX_PRIORITIES > 32 )
#त्रुटि configUSE_PORT_OPTIMISED_TASK_SELECTION can only be set to 1 when configMAX_PRIORITIES is less than or equal to 32.  It is very rare that a प्रणाली requires more than 10 to 15 different priorities as tasks that share a priority will समय slice.
#पूर्ण_अगर

/* Store/clear the ready priorities in a bit map. */
#घोषणा portRECORD_READY_PRIORITY( uxPriority, uxReadyPriorities ) ( uxReadyPriorities ) |= ( 1UL << ( uxPriority ) )
#घोषणा portRESET_READY_PRIORITY( uxPriority, uxReadyPriorities ) ( uxReadyPriorities ) &= ~( 1UL << ( uxPriority ) )
#घोषणा portGET_HIGHEST_PRIORITY( ux_शीर्ष_Priority, uxReadyPriorities ) ux_शीर्ष_Priority = ( 31 - __builtin_clz( ( uxReadyPriorities ) ) )
#पूर्ण_अगर /* configUSE_PORT_OPTIMISED_TASK_SELECTION */



/* --------------------------------------------- Inline Implementations ------------------------------------------------
 * - Implementation of inline functions of the forward declares
 * - Should come after forward declare and FreeRTOS Porting interface, as implementation may use both.
 * - For implementation of non-inlined functions, see port.c
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

अटल अंतरभूत UBaseType_t __attribute__((always_inline)) xPortSetInterruptMaskFromISR(व्योम)
{
    UBaseType_t पूर्व_int_level = XTOS_SET_INTLEVEL(XCHAL_EXCM_LEVEL);
    portbenchmarkINTERRUPT_DISABLE();
    वापस पूर्व_int_level;
}

अटल अंतरभूत व्योम __attribute__((always_inline)) vPortClearInterruptMaskFromISR(UBaseType_t पूर्व_level)
{
    portbenchmarkINTERRUPT_RESTORE(पूर्व_level);
    XTOS_RESTORE_JUST_INTLEVEL((पूर्णांक) पूर्व_level);
}

// ------------------ Critical Sections --------------------

अटल अंतरभूत व्योम __attribute__((always_inline)) vPortEnterCritical(portMUX_TYPE *mux)
{
    xPortEnterCriticalTimeout(mux, portMUX_NO_TIMEOUT);
}

अटल अंतरभूत व्योम __attribute__((always_inline)) vPortEnterCriticalCompliance(portMUX_TYPE *mux)
{
    xPortEnterCriticalTimeoutCompliance(mux, portMUX_NO_TIMEOUT);
}

अटल अंतरभूत BaseType_t __attribute__((always_inline)) xPortEnterCriticalTimeoutSafe(portMUX_TYPE *mux, BaseType_t timeout)
{
    BaseType_t ret;
    अगर (xPortInIsrContext()) {
        ret = portTRY_ENTER_CRITICAL_ISR(mux, timeout);
    } अन्यथा {
        ret = portTRY_ENTER_CRITICAL(mux, timeout);
    }
    वापस ret;
}

अटल अंतरभूत व्योम __attribute__((always_inline)) vPortEnterCriticalSafe(portMUX_TYPE *mux)
{
    xPortEnterCriticalTimeoutSafe(mux, portMUX_NO_TIMEOUT);
}

अटल अंतरभूत व्योम __attribute__((always_inline)) vPortExitCriticalSafe(portMUX_TYPE *mux)
{
    अगर (xPortInIsrContext()) {
        portEXIT_CRITICAL_ISR(mux);
    } अन्यथा {
        portEXIT_CRITICAL(mux);
    }
}

// ---------------------- Yielding -------------------------

FORCE_INLINE_ATTR bool xPortCanYield(व्योम)
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

    वापस ((ps_reg & PS_INTLEVEL_MASK) == 0);
}

// ----------------------- System --------------------------

FORCE_INLINE_ATTR BaseType_t xPortGetCoreID(व्योम)
{
    वापस (BaseType_t) esp_cpu_get_core_id();
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
bool xPortCheckValidListMem(स्थिर व्योम *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool xPortCheckValidTCBMem(स्थिर व्योम *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_ढेर_Mem(स्थिर व्योम *ptr);

#घोषणा portVALID_LIST_MEM(ptr)     xPortCheckValidListMem(ptr)
#घोषणा portVALID_TCB_MEM(ptr)      xPortCheckValidTCBMem(ptr)
#घोषणा port_VALID_ढेर_MEM(ptr)    x_Portcheck_Valid_ढेर_Mem(ptr)

// --------------------- App-Trace -------------------------

#अगर CONFIG_ESP_TRACE_ENABLE
बाह्य अस्थिर uint32_t port_switch_flag[port_अंक_PROCESSORS];
#घोषणा os_task_switch_is_pended(_cpu_) (port_switch_flag[_cpu_])
#अन्यथा
#घोषणा os_task_switch_is_pended(_cpu_) (false)
#पूर्ण_अगर

#अगर_घोषित __cplusplus
}
#पूर्ण_अगर

#पूर्ण_अगर // __ASSEMBLER__

#पूर्ण_अगर /* PORTMACRO_H */
