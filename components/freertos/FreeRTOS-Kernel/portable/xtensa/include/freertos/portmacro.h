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

#यदि_अपरिभाषितम् PORTMACRO_H
#परिभाषय PORTMACRO_H

#यदि_अपरिभाषितम् __ASSEMBLER__

#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव <stdint.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <stdarg.h>
#अन्तर्भाव <xtensa/config/core.h>
#अन्तर्भाव <xtensa/xtruntime.h>       /* required for XTOS_SET_INTLEVEL. [refactor-todo] add common intr functions to esp_hw_support */
#अन्तर्भाव "xt_instr_macros.h"
#अन्तर्भाव "spinlock.h"
#अन्तर्भाव "esp_private/crosscore_int.h"
#अन्तर्भाव "esp_macros.h"
#अन्तर्भाव "esp_attr.h"
#अन्तर्भाव "esp_cpu.h"
#अन्तर्भाव "esp_memory_utils.h"
#अन्तर्भाव "esp_newlib.h"             /* required for esp_reent_init() in tasks.c */
#अन्तर्भाव "esp_heap_caps.h"
#अन्तर्भाव "esp_rom_sys.h"
#अन्तर्भाव "esp_system.h"             /* required by esp_get_...() functions in portable.h. [refactor-todo] Update portable.h */
#अन्तर्भाव "portbenchmark.h"
#अन्तर्भाव <limits.h>

#यदि_परिभाषितम् __cplusplus
बाह्य "C" {
#यदि_अन्तः


/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come before first as they are used further down the file
 * ------------------------------------------------------------------------------------------------------------------ */

#परिभाषय पत्तनम्_CHAR                    int8_t
#परिभाषय पत्तनम्_FLOAT                   भिन्न
#परिभाषय पत्तनम्_DOUBLE                  द्विगुण
#परिभाषय पत्तनम्_LONG                    int32_t
#परिभाषय पत्तनम्_SHORT                   int16_t
#परिभाषय पत्तनम्_राशिः_TYPE              uint8_t
#परिभाषय पत्तनम्_आधारः_TYPE               पूर्णाङ्क

प्रकारनिर्देश पत्तनम्_राशिः_TYPE              राशिः_Type_t;
प्रकारनिर्देश पत्तनम्_आधारः_TYPE               आधारः_Type_t;
प्रकारनिर्देश अचिह्नित पत्तनम्_आधारः_TYPE      UBaseType_t;

#यदि( configUSE_16_BIT_TICKS == 1 )
प्रकारनिर्देश uint16_t TickType_t;
#परिभाषय पत्तनम्_MAX_DELAY ( TickType_t ) 0xffff
#अन्यथा
प्रकारनिर्देश uint32_t TickType_t;
#परिभाषय पत्तनम्_MAX_DELAY ( TickType_t ) 0xffffffffUL
#यदि_अन्तः

/* Task function macros as described on the FreeRTOS.org WEB site. */
#परिभाषय पत्तनम्_TASK_FUNCTION_PROTO( vFunction, pvParameters )  शून्य vFunction( शून्य *pvParameters )
#परिभाषय पत्तनम्_TASK_FUNCTION( vFunction, pvParameters )        शून्य vFunction( शून्य *pvParameters )



/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#परिभाषय पत्तनम्_CRITICAL_NESTING_अन्तर्_TCB     0
#परिभाषय पत्तनम्_राशिः_GROWTH                ( -1 )
#परिभाषय पत्तनम्_TICK_PERIOD_MS              ( ( TickType_t ) 1000 / config_TICK_दरः_HZ )
#परिभाषय पत्तनम्_BYTE_ALIGNMENT              16    // Xtensa Windowed ABI requires the stack pointer to always be 16-byte aligned. See "isa_rm.pdf 8.1.1 Windowed Register Usage and Stack Layout"
#परिभाषय पत्तनम्_TICK_TYPE_IS_ATOMIC         1
#परिभाषय पत्तनम्_NOP()                       XT_NOP()



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
आधारः_Type_t x_पत्तनम्_अन्तर्_Isr_Context(शून्य);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortInIsrContext() internally
 */
शून्य v_पत्तनम्_Assert_If_अन्तर्_ISR(शून्य);

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
आधारः_Type_t x_पत्तनम्_Interrupted_From_ISRContext(शून्य);

/**
 * @brief Disable interrupts in a nested manner (meant to be called from ISRs)
 *
 * @warning Only applies to current CPU.
 * @return UBaseType_t Previous interrupt level
 */
स्थिर अन्तःस्थ UBaseType_t x_पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्_From_ISR(शून्य);

/**
 * @brief Re-enable interrupts in a nested manner (meant to be called from ISRs)
 *
 * @warning Only applies to current CPU.
 * @param prev_level Previous interrupt level
 */
स्थिर अन्तःस्थ शून्य v_पत्तनम्_Clear_व्यवधानम्_आवरणम्_From_ISR(UBaseType_t पूर्व_level);

/* ---------------------- Spinlocks ------------------------
 * - Modifications made to critical sections to support SMP
 * - See "Critical Sections & Disabling Interrupts" in docs/api-guides/freertos-smp.rst for more details
 * - Remark: For the ESP32, portENTER_CRITICAL and portENTER_CRITICAL_ISR both alias vPortEnterCritical, meaning that
 *           either function can be called both from ISR as well as task context. This is not standard FreeRTOS
 *           behavior; please keep this in mind if you need any compatibility with other FreeRTOS implementations.
 * @note [refactor-todo] Check if these comments are still true
 * ------------------------------------------------------ */

प्रकारनिर्देश spinlock_t                          पत्तनम्_MUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */
#परिभाषय पत्तनम्_MUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#परिभाषय पत्तनम्_MUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#परिभाषय पत्तनम्_MUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#परिभाषय पत्तनम्_MUX_TRY_तालकम्                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#परिभाषय पत्तनम्_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

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
आधारः_Type_t x_पत्तनम्_Enter_Critical_Timeout(पत्तनम्_MUX_TYPE *mux, आधारः_Type_t timeout);

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
स्थिर अन्तःस्थ शून्य __attribute__((always_inline)) v_पत्तनम्_Enter_Critical(पत्तनम्_MUX_TYPE *mux);

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
शून्य v_पत्तनम्_Exit_Critical(पत्तनम्_MUX_TYPE *mux);

#यदि CONFIG_FREERTOS_पत्तनम्_THREAD_SAFE_CLAIM
/**
 * @brief Claim thread-safe region start
 *        If claimed, vPortEnterCritical/vPortExitCritical on the current core are no-ops.
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
शून्य x_पत्तनम्_Thread_Safe_Claim(शून्य);

/**
 * @brief Claim thread-safe region end
 *        Restores normal port critical behavior
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
शून्य x_पत्तनम्_Thread_Safe_Disclaim(शून्य);
#यदि_अन्तः /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

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
आधारः_Type_t x_पत्तनम्_Enter_Critical_Timeout_Compliance(पत्तनम्_MUX_TYPE *mux, आधारः_Type_t timeout);

/**
 * @brief FreeRTOS compliant version of vPortEnterCritical()
 *
 * Compliant version of vPortEnterCritical() will ensure that this is
 * called from a task context only. An abort is called otherwise.
 *
 * @param[in] mux Spinlock
 */
स्थिर अन्तःस्थ शून्य __attribute__((always_inline)) v_पत्तनम्_Enter_Critical_Compliance(पत्तनम्_MUX_TYPE *mux);

/**
 * @brief FreeRTOS compliant version of vPortExitCritical()
 *
 * Compliant version of vPortExitCritical() will ensure that this is
 * called from a task context only. An abort is called otherwise.
 *
 * @note This function is made non-inline on purpose to reduce code size
 * @param[in] mux Spinlock
 */
शून्य v_पत्तनम्_Exit_Critical_Compliance(पत्तनम्_MUX_TYPE *mux);

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
स्थिर अन्तःस्थ आधारः_Type_t __attribute__((always_inline)) x_पत्तनम्_Enter_Critical_Timeout_Safe(पत्तनम्_MUX_TYPE *mux, आधारः_Type_t timeout);

/**
 * @brief Safe version of enter critical
 *
 * Safe version of enter critical will automatically select between
 * portENTER_CRITICAL() and portENTER_CRITICAL_ISR()
 *
 * @param[in] mux Spinlock
 */
स्थिर अन्तःस्थ शून्य __attribute__((always_inline)) v_पत्तनम्_Enter_Critical_Safe(पत्तनम्_MUX_TYPE *mux);

/**
 * @brief Safe version of exit critical
 *
 * Safe version of enter critical will automatically select between
 * portEXIT_CRITICAL() and portEXIT_CRITICAL_ISR()
 *
 * @param[in] mux Spinlock
 */
स्थिर अन्तःस्थ शून्य __attribute__((always_inline)) v_पत्तनम्_Exit_Critical_Safe(पत्तनम्_MUX_TYPE *mux);

// ---------------------- Yielding -------------------------

/**
 * @brief Perform a solicited context switch
 *
 * - Defined in portasm.S
 *
 * @note [refactor-todo] The rest of ESP-IDF should call taskYield() instead
 */
शून्य v_पत्तनम्_Yield( शून्य );

/**
 * @brief Yields the other core
 *
 * - Send an interrupt to another core in order to make the task running on it yield for a higher-priority task.
 * - Can be used to yield current core as well
 *
 * @note [refactor-todo] Put this into private macros as its only called from task.c and is not public API
 * @param coreid ID of core to yield
 */
शून्य v_पत्तनम्_Yield_Other_Core(आधारः_Type_t coreid);

/**
 * @brief Checks if the current core can yield
 *
 * - A core cannot yield if its in an ISR or in a critical section
 *
 * @note [refactor-todo] See if this can be separated from port macro
 * @return true Core can yield
 * @return false Core cannot yield
 */
FORCE_INLINE_ATTR bool x_पत्तनम्_Can_Yield(शून्य);

// ------------------- Hook Functions ----------------------

/**
 * @brief Hook function called on entry to tickless idle
 *
 * - Implemented in pm_impl.c
 *
 * @param xExpectedIdleTime Expected idle time
 */
शून्य vApplicationSleep(TickType_t xExpectedIdleTime);

// ----------------------- System --------------------------

/**
 * @brief Get the tick rate per second
 *
 * @deprecated This function will be removed in IDF 7.0. Use CONFIG_FREERTOS_HZ directly instead.
 * @note [refactor-todo] Remove this function in IDF 7.0 (IDF-14115)
 * @return uint32_t Tick rate in Hz
 */
uint32_t x_पत्तनम्_Get_Tick_दरः_Hz(शून्य) __attribute__((deprecated("This function will be removed in IDF 7.0. Use CONFIG_FREERTOS_HZ directly instead.")));

/**
 * @brief Set a watchpoint to watch the last 32 bytes of the stack
 *
 * Callback to set a watchpoint on the end of the stack. Called every context switch to change the stack watchpoint
 * around.
 *
 * @param pxStackStart Pointer to the start of the stack
 */
शून्य v_पत्तनम्_समुच्चयः_राशिः_Watchpoint( शून्य *px_राशिः_Start );

/**
 * @brief Get the current core's ID
 *
 * @note [refactor-todo] IDF should call a FreeRTOS like macro instead of port function directly
 * @return BaseType_t Core ID
 */
FORCE_INLINE_ATTR आधारः_Type_t x_पत्तनम्_Get_Core_ID(शून्य);

// --------------------- TCB Cleanup -----------------------

/**
 * @brief TCB cleanup hook
 *
 * The portCLEAN_UP_TCB() macro is called in prvDeleteTCB() right before a
 * deleted task's memory is freed. We map that macro to this internal function
 * so that IDF FreeRTOS ports can inject some task pre-deletion operations.
 */
शून्य v_पत्तनम्_TCBPre_Delete_Hook( शून्य *pxTCB );


/* ------------------------------------------- FreeRTOS Porting Interface ----------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS
 * - Most come after forward declare as porting macros map to declared functions
 * - Maps to forward declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- System --------------------------

#यदि ( config_सङ्ख्या_सम्बन्धि_CORES > 1 )
    #परिभाषय पत्तनम्_GET_CORE_ID()       x_पत्तनम्_Get_Core_ID()
#अन्यथा /* configNUMBER_OF_CORES > 1 */
    #परिभाषय पत्तनम्_GET_CORE_ID()       ((आधारः_Type_t) 0);
#यदि_अन्तः /* configNUMBER_OF_CORES > 1 */

// --------------------- Interrupts ------------------------

/**
 * - Only applies to current core
 * - These cannot be nested. They should be used with a lot of care and cannot be called from interrupt level.
 *
 * @note [refactor-todo] replace XTOS_SET_INTLEVEL with more efficient version, if any?
 */
#परिभाषय पत्तनम्_DISABLE_INTERRUPTS()            कुरु { XTOS_समुच्चयः_INTLEVEL(XCHAL_EXCM_LEVEL); portbenchmark_व्यवधानम्_DISABLE(); } यावत् (0)
#परिभाषय पत्तनम्_ENABLE_INTERRUPTS()             कुरु { portbenchmark_व्यवधानम्_RESTORE(0); XTOS_समुच्चयः_INTLEVEL(0); } यावत् (0)

/**
 * ISR versions to enable/disable interrupts
 */
#परिभाषय पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्_FROM_ISR()                   x_पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्_From_ISR()
#परिभाषय पत्तनम्_CLEAR_व्यवधानम्_आवरणम्_FROM_ISR(पूर्व_level)       v_पत्तनम्_Clear_व्यवधानम्_आवरणम्_From_ISR(पूर्व_level)

/**
 * @brief Assert if in ISR context
 */
#परिभाषय पत्तनम्_ASSERT_IF_अन्तर्_ISR() v_पत्तनम्_Assert_If_अन्तर्_ISR()

/**
 * @brief Used by FreeRTOS functions to call the correct version of critical section API
 */
#यदि ( config_अङ्कः_CORES > 1 )
#परिभाषय पत्तनम्_CHECK_IF_अन्तर्_ISR()   x_पत्तनम्_अन्तर्_Isr_Context()
#यदि_अन्तः

// ------------------ Critical Sections --------------------

/**
 * @brief FreeRTOS critical section macros
 *
 * - Added a spinlock argument for SMP
 * - Can be nested
 * - Compliance versions will assert if regular critical section API is used in ISR context
 * - Safe versions can be called from either contexts
 */
#यदि_परिभाषितम् CONFIG_FREERTOS_CHECK_पत्तनम्_CRITICAL_COMPLIANCE
#परिभाषय पत्तनम्_TRY_ENTER_CRITICAL(mux, timeout)        x_पत्तनम्_Enter_Critical_Timeout_Compliance(mux, timeout)
#परिभाषय पत्तनम्_ENTER_CRITICAL(mux)                     v_पत्तनम्_Enter_Critical_Compliance(mux)
#परिभाषय पत्तनम्_EXIT_CRITICAL(mux)                      v_पत्तनम्_Exit_Critical_Compliance(mux)
#अन्यथा
#परिभाषय पत्तनम्_TRY_ENTER_CRITICAL(mux, timeout)        x_पत्तनम्_Enter_Critical_Timeout(mux, timeout)
#परिभाषय पत्तनम्_ENTER_CRITICAL(mux)                     v_पत्तनम्_Enter_Critical(mux)
#परिभाषय पत्तनम्_EXIT_CRITICAL(mux)                      v_पत्तनम्_Exit_Critical(mux)
#यदि_अन्तः /* CONFIG_FREERTOS_CHECK_PORT_CRITICAL_COMPLIANCE */

#परिभाषय पत्तनम्_TRY_ENTER_CRITICAL_ISR(mux, timeout)    x_पत्तनम्_Enter_Critical_Timeout(mux, timeout)
#परिभाषय पत्तनम्_ENTER_CRITICAL_ISR(mux)                 v_पत्तनम्_Enter_Critical(mux)
#परिभाषय पत्तनम्_EXIT_CRITICAL_ISR(mux)                  v_पत्तनम्_Exit_Critical(mux)

#परिभाषय पत्तनम्_TRY_ENTER_CRITICAL_SAFE(mux, timeout)   x_पत्तनम्_Enter_Critical_Timeout_Safe(mux, timeout)
#परिभाषय पत्तनम्_ENTER_CRITICAL_SAFE(mux)                v_पत्तनम्_Enter_Critical_Safe(mux)
#परिभाषय पत्तनम्_EXIT_CRITICAL_SAFE(mux)                 v_पत्तनम्_Exit_Critical_Safe(mux)

// ---------------------- Yielding -------------------------

#परिभाषय पत्तनम्_YIELD() v_पत्तनम्_Yield()

बाह्य शून्य frxt_सज्जता_switch( शून्य );     //Defined in portasm.S

#परिभाषय पत्तनम्_YIELD_FROM_ISR_NO_ARG() ({ \
    trace_ISR_EXIT_गन्तव्यम्_SCHEDULER(); \
    frxt_सज्जता_switch(); \
})
#परिभाषय पत्तनम्_YIELD_FROM_ISR_ARG(x_Higher_प्राथमिकता_Task_Woken) ({ \
    यदि (x_Higher_प्राथमिकता_Task_Woken == pdTRUE) { \
        trace_ISR_EXIT_गन्तव्यम्_SCHEDULER(); \
        frxt_सज्जता_switch(); \
    } \
})

/**
 * @note    The macro below could be used when passing a single argument, or without any argument,
 *          it was developed to support both usages of portYIELD inside of an ISR. Any other usage form
 *          might result in undesired behavior
 */
#यदि defined(__cplusplus) && (__cplusplus >  201703L)
#परिभाषय पत्तनम्_YIELD_FROM_ISR(...) वृणु_MACRO_VA_ARG(पत्तनम्_YIELD_FROM_ISR_ARG, पत्तनम्_YIELD_FROM_ISR_NO_ARG __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#अन्यथा
#परिभाषय पत्तनम्_YIELD_FROM_ISR(...) वृणु_MACRO_VA_ARG(पत्तनम्_YIELD_FROM_ISR_ARG, पत्तनम्_YIELD_FROM_ISR_NO_ARG, ##__VA_ARGS__)(__VA_ARGS__)
#यदि_अन्तः

/* Yielding within an API call (when interrupts are off), means the yield should be delayed
   until interrupts are re-enabled.

   To do this, we use the "cross-core" interrupt as a trigger to yield on this core when interrupts are re-enabled.This
   is the same interrupt & code path which is used to trigger a yield between CPUs, although in this case the yield is
   happening on the same CPU.
*/
#परिभाषय पत्तनम्_YIELD_WITHIN_API() esp_crosscore_int_send_yield(x_पत्तनम्_Get_Core_ID())

#यदि ( config_सङ्ख्या_सम्बन्धि_CORES > 1 )
    #परिभाषय पत्तनम्_YIELD_CORE( xCoreID )     v_पत्तनम्_Yield_Other_Core( xCoreID )
#यदि_अन्तः /* configNUMBER_OF_CORES > 1 */

// ------------------- Hook Functions ----------------------

#परिभाषय पत्तनम्_SUPPRESS_TICKS_AND_SLEEP(idleTime) vApplicationSleep(idleTime)

// ------------------- Run Time Stats ----------------------

#परिभाषय पत्तनम्_CONFIGURE_TIMER_FOR_सततम्_TIME_STATS()

#यदि ( CONFIG_FREERTOS_GENERATE_सततम्_TIME_STATS )
config_सततम्_TIME_गणक_TYPE x_पत्तनम्_Get_सततम्_Time_गणक_मानम्( शून्य );
#परिभाषय पत्तनम्_GET_सततम्_TIME_गणक_मानम्()        x_पत्तनम्_Get_सततम्_Time_गणक_मानम्()
#यदि_अन्तः

// --------------------- TCB Cleanup -----------------------

#परिभाषय पत्तनम्_CLEAN_ऊर्ध्वम्_TCB( pxTCB ) v_पत्तनम्_TCBPre_Delete_Hook( pxTCB )

// -------------- Optimized Task Selection -----------------

#यदि config_USE_पत्तनम्_OPTIMISED_TASK_SELECTION == 1
/* Check the configuration. */
#यदि( configMAX_PRIORITIES > 32 )
#दोषः config_USE_पत्तनम्_OPTIMISED_TASK_SELECTION can only be समुच्चयः गन्तव्यम् 1 when configMAX_PRIORITIES is less than or equal गन्तव्यम् 32.  It is very rare that a system requires अधिकम् than 10 गन्तव्यम् 15 different priorities as tasks that share a प्राथमिकता will time खण्डिका.
#यदि_अन्तः

/* Store/clear the ready priorities in a bit map. */
#परिभाषय पत्तनम्_लेखः_READY_प्राथमिकता( ux_प्राथमिकता, uxReadyPriorities ) ( uxReadyPriorities ) |= ( 1UL << ( ux_प्राथमिकता ) )
#परिभाषय पत्तनम्_RESET_READY_प्राथमिकता( ux_प्राथमिकता, uxReadyPriorities ) ( uxReadyPriorities ) &= ~( 1UL << ( ux_प्राथमिकता ) )
#परिभाषय पत्तनम्_GET_HIGHEST_प्राथमिकता( ux_शिखरम्_प्राथमिकता, uxReadyPriorities ) ux_शिखरम्_प्राथमिकता = ( 31 - __builtin_clz( ( uxReadyPriorities ) ) )
#यदि_अन्तः /* configUSE_PORT_OPTIMISED_TASK_SELECTION */



/* --------------------------------------------- Inline Implementations ------------------------------------------------
 * - Implementation of inline functions of the forward declares
 * - Should come after forward declare and FreeRTOS Porting interface, as implementation may use both.
 * - For implementation of non-inlined functions, see port.c
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

स्थिर अन्तःस्थ UBaseType_t __attribute__((always_inline)) x_पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्_From_ISR(शून्य)
{
    UBaseType_t पूर्व_int_level = XTOS_समुच्चयः_INTLEVEL(XCHAL_EXCM_LEVEL);
    portbenchmark_व्यवधानम्_DISABLE();
    प्रत्यागम पूर्व_int_level;
}

स्थिर अन्तःस्थ शून्य __attribute__((always_inline)) v_पत्तनम्_Clear_व्यवधानम्_आवरणम्_From_ISR(UBaseType_t पूर्व_level)
{
    portbenchmark_व्यवधानम्_RESTORE(पूर्व_level);
    XTOS_RESTORE_JUST_INTLEVEL((पूर्णाङ्क) पूर्व_level);
}

// ------------------ Critical Sections --------------------

स्थिर अन्तःस्थ शून्य __attribute__((always_inline)) v_पत्तनम्_Enter_Critical(पत्तनम्_MUX_TYPE *mux)
{
    x_पत्तनम्_Enter_Critical_Timeout(mux, पत्तनम्_MUX_NO_TIMEOUT);
}

स्थिर अन्तःस्थ शून्य __attribute__((always_inline)) v_पत्तनम्_Enter_Critical_Compliance(पत्तनम्_MUX_TYPE *mux)
{
    x_पत्तनम्_Enter_Critical_Timeout_Compliance(mux, पत्तनम्_MUX_NO_TIMEOUT);
}

स्थिर अन्तःस्थ आधारः_Type_t __attribute__((always_inline)) x_पत्तनम्_Enter_Critical_Timeout_Safe(पत्तनम्_MUX_TYPE *mux, आधारः_Type_t timeout)
{
    आधारः_Type_t ret;
    यदि (x_पत्तनम्_अन्तर्_Isr_Context()) {
        ret = पत्तनम्_TRY_ENTER_CRITICAL_ISR(mux, timeout);
    } अन्यथा {
        ret = पत्तनम्_TRY_ENTER_CRITICAL(mux, timeout);
    }
    प्रत्यागम ret;
}

स्थिर अन्तःस्थ शून्य __attribute__((always_inline)) v_पत्तनम्_Enter_Critical_Safe(पत्तनम्_MUX_TYPE *mux)
{
    x_पत्तनम्_Enter_Critical_Timeout_Safe(mux, पत्तनम्_MUX_NO_TIMEOUT);
}

स्थिर अन्तःस्थ शून्य __attribute__((always_inline)) v_पत्तनम्_Exit_Critical_Safe(पत्तनम्_MUX_TYPE *mux)
{
    यदि (x_पत्तनम्_अन्तर्_Isr_Context()) {
        पत्तनम्_EXIT_CRITICAL_ISR(mux);
    } अन्यथा {
        पत्तनम्_EXIT_CRITICAL(mux);
    }
}

// ---------------------- Yielding -------------------------

FORCE_INLINE_ATTR bool x_पत्तनम्_Can_Yield(शून्य)
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

    प्रत्यागम ((ps_reg & PS_INTLEVEL_आवरणम्) == 0);
}

// ----------------------- System --------------------------

FORCE_INLINE_ATTR आधारः_Type_t x_पत्तनम्_Get_Core_ID(शून्य)
{
    प्रत्यागम (आधारः_Type_t) esp_cpu_get_core_id();
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
bool x_पत्तनम्_Check_Valid_List_Mem(ध्रुव शून्य *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool x_पत्तनम्_Check_Valid_TCBMem(ध्रुव शून्य *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_राशिः_Mem(ध्रुव शून्य *ptr);

#परिभाषय पत्तनम्_VALID_LIST_MEM(ptr)     x_पत्तनम्_Check_Valid_List_Mem(ptr)
#परिभाषय पत्तनम्_VALID_TCB_MEM(ptr)      x_पत्तनम्_Check_Valid_TCBMem(ptr)
#परिभाषय पत्तनम्_VALID_राशिः_MEM(ptr)    x_Portcheck_Valid_राशिः_Mem(ptr)

// --------------------- App-Trace -------------------------

#यदि CONFIG_ESP_TRACE_ENABLE
बाह्य चञ्चल uint32_t पत्तनम्_switch_flag[पत्तनम्_अङ्कः_PROCESSORS];
#परिभाषय os_task_switch_is_pended(_cpu_) (पत्तनम्_switch_flag[_cpu_])
#अन्यथा
#परिभाषय os_task_switch_is_pended(_cpu_) (false)
#यदि_अन्तः

#यदि_परिभाषितम् __cplusplus
}
#यदि_अन्तः

#यदि_अन्तः // __ASSEMBLER__

#यदि_अन्तः /* PORTMACRO_H */
