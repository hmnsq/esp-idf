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

#నిర్వచితం_కాకపోతే PORTMACRO_H
#నిర్వచించు PORTMACRO_H

#నిర్వచితం_కాకపోతే __ASSEMBLER__

#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ <stdint.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <stdbool.h>
#అన్తర్భావ <stdarg.h>
#అన్తర్భావ <xtensa/config/core.h>
#అన్తర్భావ <xtensa/xtruntime.h>       /* required for XTOS_SET_INTLEVEL. [refactor-todo] add common intr functions to esp_hw_support */
#అన్తర్భావ "xt_instr_macros.h"
#అన్తర్భావ "spinlock.h"
#అన్తర్భావ "esp_private/crosscore_int.h"
#అన్తర్భావ "esp_macros.h"
#అన్తర్భావ "esp_attr.h"
#అన్తర్భావ "esp_cpu.h"
#అన్తర్భావ "esp_memory_utils.h"
#అన్తర్భావ "esp_newlib.h"             /* required for esp_reent_init() in tasks.c */
#అన్తర్భావ "esp_heap_caps.h"
#అన్తర్భావ "esp_rom_sys.h"
#అన్తర్భావ "esp_system.h"             /* required by esp_get_...() functions in portable.h. [refactor-todo] Update portable.h */
#అన్తర్భావ "portbenchmark.h"
#అన్తర్భావ <limits.h>

#నిర్వచితమైతే __cplusplus
బాహ్య "C" {
#అయితే_ముగింపు


/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come before first as they are used further down the file
 * ------------------------------------------------------------------------------------------------------------------ */

#నిర్వచించు పోర్టు_CHAR                    int8_t
#నిర్వచించు పోర్టు_FLOAT                   దశాంశం
#నిర్వచించు పోర్టు_DOUBLE                  ద్విగుణ
#నిర్వచించు పోర్టు_LONG                    int32_t
#నిర్వచించు పోర్టు_SHORT                   int16_t
#నిర్వచించు పోర్టు_దొంతర_TYPE              uint8_t
#నిర్వచించు పోర్టు_పునాది_TYPE               పూర్ణాంకం

రకనిర్వచనం పోర్టు_దొంతర_TYPE              దొంతర_Type_t;
రకనిర్వచనం పోర్టు_పునాది_TYPE               పునాది_Type_t;
రకనిర్వచనం గుర్తులేని పోర్టు_పునాది_TYPE      UBaseType_t;

#ఒకవేళ( config_USE_16_బిట్టు_TICKS == 1 )
రకనిర్వచనం uint16_t TickType_t;
#నిర్వచించు పోర్టు_MAX_DELAY ( TickType_t ) 0xffff
#లేకపోతే
రకనిర్వచనం uint32_t TickType_t;
#నిర్వచించు పోర్టు_MAX_DELAY ( TickType_t ) 0xffffffffUL
#అయితే_ముగింపు

/* Task function macros as described on the FreeRTOS.org WEB site. */
#నిర్వచించు పోర్టు_TASK_FUNCTION_PROTO( vFunction, pvParameters )  శూన్య vFunction( శూన్య *pvParameters )
#నిర్వచించు పోర్టు_TASK_FUNCTION( vFunction, pvParameters )        శూన్య vFunction( శూన్య *pvParameters )



/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#నిర్వచించు పోర్టు_CRITICAL_NESTING_లోపల_TCB     0
#నిర్వచించు పోర్టు_దొంతర_GROWTH                ( -1 )
#నిర్వచించు పోర్టు_TICK_PERIOD_MS              ( ( TickType_t ) 1000 / config_TICK_రేటు_HZ )
#నిర్వచించు పోర్టు_BYTE_ALIGNMENT              16    // Xtensa Windowed ABI requires the stack pointer to always be 16-byte aligned. See "isa_rm.pdf 8.1.1 Windowed Register Usage and Stack Layout"
#నిర్వచించు పోర్టు_TICK_TYPE_IS_ATOMIC         1
#నిర్వచించు పోర్టు_NOP()                       XT_NOP()



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
పునాది_Type_t x_పోర్టు_లోపల_Isr_Context(శూన్య);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortInIsrContext() internally
 */
శూన్య v_పోర్టు_Assert_If_లోపల_ISR(శూన్య);

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
పునాది_Type_t x_పోర్టు_Interrupted_From_ISRContext(శూన్య);

/**
 * @brief Disable interrupts in a nested manner (meant to be called from ISRs)
 *
 * @warning Only applies to current CPU.
 * @return UBaseType_t Previous interrupt level
 */
స్థిర అంతఃవరుస UBaseType_t x_పోర్టు_సమితి_అంతరాయం_ముసుగు_From_ISR(శూన్య);

/**
 * @brief Re-enable interrupts in a nested manner (meant to be called from ISRs)
 *
 * @warning Only applies to current CPU.
 * @param prev_level Previous interrupt level
 */
స్థిర అంతఃవరుస శూన్య v_పోర్టు_Clear_అంతరాయం_ముసుగు_From_ISR(UBaseType_t prev_level);

/* ---------------------- Spinlocks ------------------------
 * - Modifications made to critical sections to support SMP
 * - See "Critical Sections & Disabling Interrupts" in docs/api-guides/freertos-smp.rst for more details
 * - Remark: For the ESP32, portENTER_CRITICAL and portENTER_CRITICAL_ISR both alias vPortEnterCritical, meaning that
 *           either function can be called both from ISR as well as task context. This is not standard FreeRTOS
 *           behavior; please keep this in mind if you need any compatibility with other FreeRTOS implementations.
 * @note [refactor-todo] Check if these comments are still true
 * ------------------------------------------------------ */

రకనిర్వచనం spinlock_t                          పోర్టు_MUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */
#నిర్వచించు పోర్టు_MUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#నిర్వచించు పోర్టు_MUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#నిర్వచించు పోర్టు_MUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#నిర్వచించు పోర్టు_MUX_TRY_తాళం                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#నిర్వచించు పోర్టు_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

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
పునాది_Type_t x_పోర్టు_Enter_Critical_Timeout(పోర్టు_MUX_TYPE *mux, పునాది_Type_t timeout);

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
స్థిర అంతఃవరుస శూన్య __attribute__((always_inline)) v_పోర్టు_Enter_Critical(పోర్టు_MUX_TYPE *mux);

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
శూన్య v_పోర్టు_Exit_Critical(పోర్టు_MUX_TYPE *mux);

#ఒకవేళ CONFIG_FREERTOS_పోర్టు_THREAD_సురక్షిత_CLAIM
/**
 * @brief Claim thread-safe region start
 *        If claimed, vPortEnterCritical/vPortExitCritical on the current core are no-ops.
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
శూన్య x_పోర్టు_Thread_సురక్షిత_Claim(శూన్య);

/**
 * @brief Claim thread-safe region end
 *        Restores normal port critical behavior
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
శూన్య x_పోర్టు_Thread_సురక్షిత_Disclaim(శూన్య);
#అయితే_ముగింపు /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

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
పునాది_Type_t x_పోర్టు_Enter_Critical_Timeout_Compliance(పోర్టు_MUX_TYPE *mux, పునాది_Type_t timeout);

/**
 * @brief FreeRTOS compliant version of vPortEnterCritical()
 *
 * Compliant version of vPortEnterCritical() will ensure that this is
 * called from a task context only. An abort is called otherwise.
 *
 * @param[in] mux Spinlock
 */
స్థిర అంతఃవరుస శూన్య __attribute__((always_inline)) v_పోర్టు_Enter_Critical_Compliance(పోర్టు_MUX_TYPE *mux);

/**
 * @brief FreeRTOS compliant version of vPortExitCritical()
 *
 * Compliant version of vPortExitCritical() will ensure that this is
 * called from a task context only. An abort is called otherwise.
 *
 * @note This function is made non-inline on purpose to reduce code size
 * @param[in] mux Spinlock
 */
శూన్య v_పోర్టు_Exit_Critical_Compliance(పోర్టు_MUX_TYPE *mux);

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
స్థిర అంతఃవరుస పునాది_Type_t __attribute__((always_inline)) x_పోర్టు_Enter_Critical_Timeout_సురక్షిత(పోర్టు_MUX_TYPE *mux, పునాది_Type_t timeout);

/**
 * @brief Safe version of enter critical
 *
 * Safe version of enter critical will automatically select between
 * portENTER_CRITICAL() and portENTER_CRITICAL_ISR()
 *
 * @param[in] mux Spinlock
 */
స్థిర అంతఃవరుస శూన్య __attribute__((always_inline)) v_పోర్టు_Enter_Critical_సురక్షిత(పోర్టు_MUX_TYPE *mux);

/**
 * @brief Safe version of exit critical
 *
 * Safe version of enter critical will automatically select between
 * portEXIT_CRITICAL() and portEXIT_CRITICAL_ISR()
 *
 * @param[in] mux Spinlock
 */
స్థిర అంతఃవరుస శూన్య __attribute__((always_inline)) v_పోర్టు_Exit_Critical_సురక్షిత(పోర్టు_MUX_TYPE *mux);

// ---------------------- Yielding -------------------------

/**
 * @brief Perform a solicited context switch
 *
 * - Defined in portasm.S
 *
 * @note [refactor-todo] The rest of ESP-IDF should call taskYield() instead
 */
శూన్య v_పోర్టు_Yield( శూన్య );

/**
 * @brief Yields the other core
 *
 * - Send an interrupt to another core in order to make the task running on it yield for a higher-priority task.
 * - Can be used to yield current core as well
 *
 * @note [refactor-todo] Put this into private macros as its only called from task.c and is not public API
 * @param coreid ID of core to yield
 */
శూన్య v_పోర్టు_Yield_Other_Core(పునాది_Type_t coreid);

/**
 * @brief Checks if the current core can yield
 *
 * - A core cannot yield if its in an ISR or in a critical section
 *
 * @note [refactor-todo] See if this can be separated from port macro
 * @return true Core can yield
 * @return false Core cannot yield
 */
FORCE_INLINE_ATTR bool x_పోర్టు_Can_Yield(శూన్య);

// ------------------- Hook Functions ----------------------

/**
 * @brief Hook function called on entry to tickless idle
 *
 * - Implemented in pm_impl.c
 *
 * @param xExpectedIdleTime Expected idle time
 */
శూన్య vApplicationSleep(TickType_t x_Expected_Idle_సమయం);

// ----------------------- System --------------------------

/**
 * @brief Get the tick rate per second
 *
 * @deprecated This function will be removed in IDF 7.0. Use CONFIG_FREERTOS_HZ directly instead.
 * @note [refactor-todo] Remove this function in IDF 7.0 (IDF-14115)
 * @return uint32_t Tick rate in Hz
 */
uint32_t x_పోర్టు_Get_Tick_రేటు_Hz(శూన్య) __attribute__((deprecated("This function will be removed in IDF 7.0. Use CONFIG_FREERTOS_HZ directly instead.")));

/**
 * @brief Set a watchpoint to watch the last 32 bytes of the stack
 *
 * Callback to set a watchpoint on the end of the stack. Called every context switch to change the stack watchpoint
 * around.
 *
 * @param pxStackStart Pointer to the start of the stack
 */
శూన్య v_పోర్టు_సమితి_దొంతర_Watchpoint( శూన్య *px_దొంతర_ప్రారంభం );

/**
 * @brief Get the current core's ID
 *
 * @note [refactor-todo] IDF should call a FreeRTOS like macro instead of port function directly
 * @return BaseType_t Core ID
 */
FORCE_INLINE_ATTR పునాది_Type_t x_పోర్టు_Get_Core_ID(శూన్య);

// --------------------- TCB Cleanup -----------------------

/**
 * @brief TCB cleanup hook
 *
 * The portCLEAN_UP_TCB() macro is called in prvDeleteTCB() right before a
 * deleted task's memory is freed. We map that macro to this internal function
 * so that IDF FreeRTOS ports can inject some task pre-deletion operations.
 */
శూన్య v_పోర్టు_TCBPre_Delete_Hook( శూన్య *pxTCB );


/* ------------------------------------------- FreeRTOS Porting Interface ----------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS
 * - Most come after forward declare as porting macros map to declared functions
 * - Maps to forward declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- System --------------------------

#ఒకవేళ ( config_సంఖ్య_యొక్క_CORES > 1 )
    #నిర్వచించు పోర్టు_GET_CORE_ID()       x_పోర్టు_Get_Core_ID()
#లేకపోతే /* configNUMBER_OF_CORES > 1 */
    #నిర్వచించు పోర్టు_GET_CORE_ID()       ((పునాది_Type_t) 0);
#అయితే_ముగింపు /* configNUMBER_OF_CORES > 1 */

// --------------------- Interrupts ------------------------

/**
 * - Only applies to current core
 * - These cannot be nested. They should be used with a lot of care and cannot be called from interrupt level.
 *
 * @note [refactor-todo] replace XTOS_SET_INTLEVEL with more efficient version, if any?
 */
#నిర్వచించు పోర్టు_DISABLE_INTERRUPTS()            చేయి { XTOS_సమితి_INTLEVEL(XCHAL_EXCM_LEVEL); portbenchmark_అంతరాయం_DISABLE(); } అయితే (0)
#నిర్వచించు పోర్టు_ENABLE_INTERRUPTS()             చేయి { portbenchmark_అంతరాయం_RESTORE(0); XTOS_సమితి_INTLEVEL(0); } అయితే (0)

/**
 * ISR versions to enable/disable interrupts
 */
#నిర్వచించు పోర్టు_సమితి_అంతరాయం_ముసుగు_FROM_ISR()                   x_పోర్టు_సమితి_అంతరాయం_ముసుగు_From_ISR()
#నిర్వచించు పోర్టు_CLEAR_అంతరాయం_ముసుగు_FROM_ISR(prev_level)       v_పోర్టు_Clear_అంతరాయం_ముసుగు_From_ISR(prev_level)

/**
 * @brief Assert if in ISR context
 */
#నిర్వచించు పోర్టు_ASSERT_IF_లోపల_ISR() v_పోర్టు_Assert_If_లోపల_ISR()

/**
 * @brief Used by FreeRTOS functions to call the correct version of critical section API
 */
#ఒకవేళ ( config_అంకె_CORES > 1 )
#నిర్వచించు పోర్టు_CHECK_IF_లోపల_ISR()   x_పోర్టు_లోపల_Isr_Context()
#అయితే_ముగింపు

// ------------------ Critical Sections --------------------

/**
 * @brief FreeRTOS critical section macros
 *
 * - Added a spinlock argument for SMP
 * - Can be nested
 * - Compliance versions will assert if regular critical section API is used in ISR context
 * - Safe versions can be called from either contexts
 */
#నిర్వచితమైతే CONFIG_FREERTOS_CHECK_పోర్టు_CRITICAL_COMPLIANCE
#నిర్వచించు పోర్టు_TRY_ENTER_CRITICAL(mux, timeout)        x_పోర్టు_Enter_Critical_Timeout_Compliance(mux, timeout)
#నిర్వచించు పోర్టు_ENTER_CRITICAL(mux)                     v_పోర్టు_Enter_Critical_Compliance(mux)
#నిర్వచించు పోర్టు_EXIT_CRITICAL(mux)                      v_పోర్టు_Exit_Critical_Compliance(mux)
#లేకపోతే
#నిర్వచించు పోర్టు_TRY_ENTER_CRITICAL(mux, timeout)        x_పోర్టు_Enter_Critical_Timeout(mux, timeout)
#నిర్వచించు పోర్టు_ENTER_CRITICAL(mux)                     v_పోర్టు_Enter_Critical(mux)
#నిర్వచించు పోర్టు_EXIT_CRITICAL(mux)                      v_పోర్టు_Exit_Critical(mux)
#అయితే_ముగింపు /* CONFIG_FREERTOS_CHECK_PORT_CRITICAL_COMPLIANCE */

#నిర్వచించు పోర్టు_TRY_ENTER_CRITICAL_ISR(mux, timeout)    x_పోర్టు_Enter_Critical_Timeout(mux, timeout)
#నిర్వచించు పోర్టు_ENTER_CRITICAL_ISR(mux)                 v_పోర్టు_Enter_Critical(mux)
#నిర్వచించు పోర్టు_EXIT_CRITICAL_ISR(mux)                  v_పోర్టు_Exit_Critical(mux)

#నిర్వచించు పోర్టు_TRY_ENTER_CRITICAL_సురక్షిత(mux, timeout)   x_పోర్టు_Enter_Critical_Timeout_సురక్షిత(mux, timeout)
#నిర్వచించు పోర్టు_ENTER_CRITICAL_సురక్షిత(mux)                v_పోర్టు_Enter_Critical_సురక్షిత(mux)
#నిర్వచించు పోర్టు_EXIT_CRITICAL_సురక్షిత(mux)                 v_పోర్టు_Exit_Critical_సురక్షిత(mux)

// ---------------------- Yielding -------------------------

#నిర్వచించు పోర్టు_YIELD() v_పోర్టు_Yield()

బాహ్య శూన్య frxt_అమరిక_switch( శూన్య );     //Defined in portasm.S

#నిర్వచించు పోర్టు_YIELD_FROM_ISR_NO_ARG() ({ \
    trace_ISR_EXIT_గమ్యం_SCHEDULER(); \
    frxt_అమరిక_switch(); \
})
#నిర్వచించు పోర్టు_YIELD_FROM_ISR_ARG(x_Higher_ప్రాధాన్యత_Task_Woken) ({ \
    ఒకవేళ (x_Higher_ప్రాధాన్యత_Task_Woken == pdTRUE) { \
        trace_ISR_EXIT_గమ్యం_SCHEDULER(); \
        frxt_అమరిక_switch(); \
    } \
})

/**
 * @note    The macro below could be used when passing a single argument, or without any argument,
 *          it was developed to support both usages of portYIELD inside of an ISR. Any other usage form
 *          might result in undesired behavior
 */
#ఒకవేళ defined(__cplusplus) && (__cplusplus >  201703L)
#నిర్వచించు పోర్టు_YIELD_FROM_ISR(...) ఎంచుకో_MACRO_VA_ARG(పోర్టు_YIELD_FROM_ISR_ARG, పోర్టు_YIELD_FROM_ISR_NO_ARG __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#లేకపోతే
#నిర్వచించు పోర్టు_YIELD_FROM_ISR(...) ఎంచుకో_MACRO_VA_ARG(పోర్టు_YIELD_FROM_ISR_ARG, పోర్టు_YIELD_FROM_ISR_NO_ARG, ##__VA_ARGS__)(__VA_ARGS__)
#అయితే_ముగింపు

/* Yielding within an API call (when interrupts are off), means the yield should be delayed
   until interrupts are re-enabled.

   To do this, we use the "cross-core" interrupt as a trigger to yield on this core when interrupts are re-enabled.This
   is the same interrupt & code path which is used to trigger a yield between CPUs, although in this case the yield is
   happening on the same CPU.
*/
#నిర్వచించు పోర్టు_YIELD_WITHIN_API() esp_crosscore_int_send_yield(x_పోర్టు_Get_Core_ID())

#ఒకవేళ ( config_సంఖ్య_యొక్క_CORES > 1 )
    #నిర్వచించు పోర్టు_YIELD_CORE( xCoreID )     v_పోర్టు_Yield_Other_Core( xCoreID )
#అయితే_ముగింపు /* configNUMBER_OF_CORES > 1 */

// ------------------- Hook Functions ----------------------

#నిర్వచించు పోర్టు_SUPPRESS_TICKS_AND_SLEEP(idle_సమయం) vApplicationSleep(idle_సమయం)

// ------------------- Run Time Stats ----------------------

#నిర్వచించు పోర్టు_CONFIGURE_TIMER_FOR_నిరంతరం_సమయం_STATS()

#ఒకవేళ ( CONFIG_FREERTOS_GENERATE_నిరంతరం_సమయం_STATS )
config_నిరంతరం_సమయం_COUNTER_TYPE x_పోర్టు_Get_నిరంతరం_సమయం_Counter_విలువ( శూన్య );
#నిర్వచించు పోర్టు_GET_నిరంతరం_సమయం_COUNTER_విలువ()        x_పోర్టు_Get_నిరంతరం_సమయం_Counter_విలువ()
#అయితే_ముగింపు

// --------------------- TCB Cleanup -----------------------

#నిర్వచించు పోర్టు_CLEAN_పైకి_TCB( pxTCB ) v_పోర్టు_TCBPre_Delete_Hook( pxTCB )

// -------------- Optimized Task Selection -----------------

#ఒకవేళ config_USE_పోర్టు_OPTIMISED_TASK_SELECTION == 1
/* Check the configuration. */
#ఒకవేళ( configMAX_PRIORITIES > 32 )
#దోషం config_USE_పోర్టు_OPTIMISED_TASK_SELECTION can only be సమితి గమ్యం 1 when configMAX_PRIORITIES is less than or equal గమ్యం 32.  It is very rare that a system requires మరిన్ని than 10 గమ్యం 15 different priorities as tasks that share a ప్రాధాన్యత will time ముక్క.
#అయితే_ముగింపు

/* Store/clear the ready priorities in a bit map. */
#నిర్వచించు పోర్టు_రికార్డు_READY_ప్రాధాన్యత( ux_ప్రాధాన్యత, uxReadyPriorities ) ( uxReadyPriorities ) |= ( 1UL << ( ux_ప్రాధాన్యత ) )
#నిర్వచించు పోర్టు_RESET_READY_ప్రాధాన్యత( ux_ప్రాధాన్యత, uxReadyPriorities ) ( uxReadyPriorities ) &= ~( 1UL << ( ux_ప్రాధాన్యత ) )
#నిర్వచించు పోర్టు_GET_HIGHEST_ప్రాధాన్యత( ux_శిఖరం_ప్రాధాన్యత, uxReadyPriorities ) ux_శిఖరం_ప్రాధాన్యత = ( 31 - __builtin_clz( ( uxReadyPriorities ) ) )
#అయితే_ముగింపు /* configUSE_PORT_OPTIMISED_TASK_SELECTION */



/* --------------------------------------------- Inline Implementations ------------------------------------------------
 * - Implementation of inline functions of the forward declares
 * - Should come after forward declare and FreeRTOS Porting interface, as implementation may use both.
 * - For implementation of non-inlined functions, see port.c
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

స్థిర అంతఃవరుస UBaseType_t __attribute__((always_inline)) x_పోర్టు_సమితి_అంతరాయం_ముసుగు_From_ISR(శూన్య)
{
    UBaseType_t prev_int_level = XTOS_సమితి_INTLEVEL(XCHAL_EXCM_LEVEL);
    portbenchmark_అంతరాయం_DISABLE();
    తిరిగి prev_int_level;
}

స్థిర అంతఃవరుస శూన్య __attribute__((always_inline)) v_పోర్టు_Clear_అంతరాయం_ముసుగు_From_ISR(UBaseType_t prev_level)
{
    portbenchmark_అంతరాయం_RESTORE(prev_level);
    XTOS_RESTORE_JUST_INTLEVEL((పూర్ణాంకం) prev_level);
}

// ------------------ Critical Sections --------------------

స్థిర అంతఃవరుస శూన్య __attribute__((always_inline)) v_పోర్టు_Enter_Critical(పోర్టు_MUX_TYPE *mux)
{
    x_పోర్టు_Enter_Critical_Timeout(mux, పోర్టు_MUX_NO_TIMEOUT);
}

స్థిర అంతఃవరుస శూన్య __attribute__((always_inline)) v_పోర్టు_Enter_Critical_Compliance(పోర్టు_MUX_TYPE *mux)
{
    x_పోర్టు_Enter_Critical_Timeout_Compliance(mux, పోర్టు_MUX_NO_TIMEOUT);
}

స్థిర అంతఃవరుస పునాది_Type_t __attribute__((always_inline)) x_పోర్టు_Enter_Critical_Timeout_సురక్షిత(పోర్టు_MUX_TYPE *mux, పునాది_Type_t timeout)
{
    పునాది_Type_t ret;
    ఒకవేళ (x_పోర్టు_లోపల_Isr_Context()) {
        ret = పోర్టు_TRY_ENTER_CRITICAL_ISR(mux, timeout);
    } లేకపోతే {
        ret = పోర్టు_TRY_ENTER_CRITICAL(mux, timeout);
    }
    తిరిగి ret;
}

స్థిర అంతఃవరుస శూన్య __attribute__((always_inline)) v_పోర్టు_Enter_Critical_సురక్షిత(పోర్టు_MUX_TYPE *mux)
{
    x_పోర్టు_Enter_Critical_Timeout_సురక్షిత(mux, పోర్టు_MUX_NO_TIMEOUT);
}

స్థిర అంతఃవరుస శూన్య __attribute__((always_inline)) v_పోర్టు_Exit_Critical_సురక్షిత(పోర్టు_MUX_TYPE *mux)
{
    ఒకవేళ (x_పోర్టు_లోపల_Isr_Context()) {
        పోర్టు_EXIT_CRITICAL_ISR(mux);
    } లేకపోతే {
        పోర్టు_EXIT_CRITICAL(mux);
    }
}

// ---------------------- Yielding -------------------------

FORCE_INLINE_ATTR bool x_పోర్టు_Can_Yield(శూన్య)
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

    తిరిగి ((ps_reg & PS_INTLEVEL_ముసుగు) == 0);
}

// ----------------------- System --------------------------

FORCE_INLINE_ATTR పునాది_Type_t x_పోర్టు_Get_Core_ID(శూన్య)
{
    తిరిగి (పునాది_Type_t) esp_cpu_get_core_id();
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
bool x_పోర్టు_Check_Valid_List_Mem(స్థిరరాశి శూన్య *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool x_పోర్టు_Check_Valid_TCBMem(స్థిరరాశి శూన్య *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_దొంతర_Mem(స్థిరరాశి శూన్య *ptr);

#నిర్వచించు పోర్టు_VALID_LIST_MEM(ptr)     x_పోర్టు_Check_Valid_List_Mem(ptr)
#నిర్వచించు పోర్టు_VALID_TCB_MEM(ptr)      x_పోర్టు_Check_Valid_TCBMem(ptr)
#నిర్వచించు పోర్టు_VALID_దొంతర_MEM(ptr)    x_Portcheck_Valid_దొంతర_Mem(ptr)

// --------------------- App-Trace -------------------------

#ఒకవేళ CONFIG_ESP_TRACE_ENABLE
బాహ్య అస్థిర uint32_t పోర్టు_switch_flag[పోర్టు_అంకె_PROCESSORS];
#నిర్వచించు os_task_switch_is_pended(_cpu_) (పోర్టు_switch_flag[_cpu_])
#లేకపోతే
#నిర్వచించు os_task_switch_is_pended(_cpu_) (false)
#అయితే_ముగింపు

#నిర్వచితమైతే __cplusplus
}
#అయితే_ముగింపు

#అయితే_ముగింపు // __ASSEMBLER__

#అయితే_ముగింపు /* PORTMACRO_H */
