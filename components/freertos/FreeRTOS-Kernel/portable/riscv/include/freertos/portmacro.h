/*
 * FreeRTOS Kernel V10.5.1 (ESP-IDF SMP modified)
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
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

#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ "freertos/FreeRTOSConfig.h"

/* Macros used instead ofsetoff() for better performance of interrupt handler */
#ఒకవేళ CONFIG_FREERTOS_USE_LIST_దత్తాంశం_INTEGRITY_CHECK_బైట్లు
#నిర్వచించు పోర్టు_స్థానభ్రంశం_PX_దొంతర 0x40
#లేకపోతే
#నిర్వచించు పోర్టు_స్థానభ్రంశం_PX_దొంతర 0x30
#అయితే_ముగింపు /* #if CONFIG_FREERTOS_USE_LIST_DATA_INTEGRITY_CHECK_BYTES */

#ఒకవేళ CONFIG_FREERTOS_UNICORE
#నిర్వచించు CORE_ID_కొలత        0
#లేకపోతే
#నిర్వచించు CORE_ID_కొలత        4
#అయితే_ముగింపు

/* Align the value up to the nearest multiple of 4 */
#నిర్వచించు పోర్టు_ALIGN_పైకి_గమ్యం_4(విలువ) (((విలువ) + 3) & ~3)

#నిర్వచించు పోర్టు_స్థానభ్రంశం_PX_చివర_యొక్క_దొంతర ( \
    పోర్టు_స్థానభ్రంశం_PX_దొంతర \
    + 4                                 /* void * pxDummy6 */ \
    + పోర్టు_ALIGN_పైకి_గమ్యం_4(configMAX_TASK_NAME_LEN) /* uint8_t ucDummy7[ configMAX_TASK_NAME_LEN ] */ \
    + CORE_ID_కొలత                      /* BaseType_t xDummyCoreID */ \
)

#నిర్వచితం_కాకపోతే __ASSEMBLER__

#అన్తర్భావ <stdint.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <stdbool.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ "spinlock.h"
#అన్తర్భావ "soc/interrupt_reg.h"
#అన్తర్భావ "esp_macros.h"
#అన్తర్భావ "esp_attr.h"
#అన్తర్భావ "esp_cpu.h"
#అన్తర్భావ "esp_rom_sys.h"
#అన్తర్భావ "esp_heap_caps.h"
#అన్తర్భావ "esp_system.h"             /* required by esp_get_...() functions in portable.h. [refactor-todo] Update portable.h */
#అన్తర్భావ "esp_newlib.h"
#అన్తర్భావ <limits.h>

#నిర్వచితమైతే __cplusplus
బాహ్య "C" {
#అయితే_ముగింపు

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come first as they are used further down in this file
 * ------------------------------------------------------------------------------------------------------------------ */

#నిర్వచించు పోర్టు_CHAR                    uint8_t
#నిర్వచించు పోర్టు_FLOAT                   దశాంశం
#నిర్వచించు పోర్టు_DOUBLE                  ద్విగుణ
#నిర్వచించు పోర్టు_LONG                    int32_t
#నిర్వచించు పోర్టు_SHORT                   int16_t
#నిర్వచించు పోర్టు_దొంతర_TYPE              uint8_t
#నిర్వచించు పోర్టు_పునాది_TYPE               పూర్ణాంకం

రకనిర్వచనం పోర్టు_దొంతర_TYPE              దొంతర_Type_t;
రకనిర్వచనం పోర్టు_పునాది_TYPE               పునాది_Type_t;
రకనిర్వచనం గుర్తులేని పోర్టు_పునాది_TYPE      UBaseType_t;

#ఒకవేళ (config_USE_16_బిట్టు_TICKS == 1)
రకనిర్వచనం uint16_t TickType_t;
#నిర్వచించు పోర్టు_MAX_DELAY (TickType_t)  0xffff
#లేకపోతే
రకనిర్వచనం uint32_t TickType_t;
#నిర్వచించు పోర్టు_MAX_DELAY (TickType_t)  0xffffffffUL
#అయితే_ముగింపు

/* Task function macros as described on the FreeRTOS.org WEB site. */
#నిర్వచించు పోర్టు_TASK_FUNCTION_PROTO(vFunction, pvParameters) శూన్య vFunction(శూన్య *pvParameters)
#నిర్వచించు పోర్టు_TASK_FUNCTION(vFunction, pvParameters) శూన్య vFunction(శూన్య *pvParameters)


/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#నిర్వచించు పోర్టు_CRITICAL_NESTING_లోపల_TCB     0
#నిర్వచించు పోర్టు_దొంతర_GROWTH                (-1)
#నిర్వచించు పోర్టు_TICK_PERIOD_MS              ((TickType_t) (1000 / config_TICK_రేటు_HZ))
#నిర్వచించు పోర్టు_BYTE_ALIGNMENT              16
#నిర్వచించు పోర్టు_TICK_TYPE_IS_ATOMIC         1
#నిర్వచించు పోర్టు_NOP() __asm అస్థిర        (" nop ")



/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros need to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

/**
 * @brief Disable interrupts in a nested manner (meant to be called from ISRs)
 *
 * @warning Only applies to current CPU.
 * @return UBaseType_t Previous interrupt level
 */
UBaseType_t x_పోర్టు_సమితి_అంతరాయం_ముసుగు_From_ISR(శూన్య);

/**
 * @brief Re-enable interrupts in a nested manner (meant to be called from ISRs)
 *
 * @warning Only applies to current CPU.
 * @param prev_int_level Previous interrupt level
 */
శూన్య v_పోర్టు_Clear_అంతరాయం_ముసుగు_From_ISR(UBaseType_t prev_int_level);

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

/*
This RISC-V port provides two kinds of critical section APIs, viz., one those take a spinlock argument and one those do
not -

These sets of APIs are -
1. vPortEnterCritical(void) and vPortExitCritical(void)
2. vPortEnterCriticalMultiCore(portMUX_TYPE *mux) and vPortExitCriticalMultiCore(portMUX_TYPE *MUX)

This is primarily done to be compatible with some IDF examples such as esp_zigbee_gateway which have a reference
to vPortEnterCritical(void) and vPortExitCritical(void) from precompiled libraries (.a).
TODO: IDF-8089
*/

/**
 * @brief Enter a critical section
 *
 * - Simply disable interrupts
 * - Can be nested
 */
శూన్య v_పోర్టు_Enter_Critical(శూన్య);

/**
 * @brief Exit a critical section
 *
 * - Reenables interrupts
 * - Can be nested
 */
శూన్య v_పోర్టు_Exit_Critical(శూన్య);

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

#ఒకవేళ (config_అంకె_CORES > 1)
/**
 * @brief Enter an SMP critical section with a timeout
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
 * @brief Enter an SMP critical section
 *
 * This function enters an SMP critical section by disabling interrupts then
 * taking a spinlock with an unlimited timeout.
 *
 * This function can be called in a nested manner
 *
 * @param[in] mux Spinlock
 */
స్థిర అంతఃవరుస శూన్య __attribute__((always_inline)) v_పోర్టు_Enter_Critical_Multi_Core(పోర్టు_MUX_TYPE *mux);

/**
 * @brief Exit an SMP critical section
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
శూన్య v_పోర్టు_Exit_Critical_Multi_Core(పోర్టు_MUX_TYPE *mux);

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
#అయితే_ముగింపు /* (configNUM_CORES > 1) */

// ---------------------- Yielding -------------------------

/**
 * @brief Set interrupt mask and return current interrupt enable register
 *
 * @note [refactor-todo] Check if this function should be renamed (due to int return type)
 * @return int Current interrupt enable register before set
 */
పూర్ణాంకం v_పోర్టు_సమితి_అంతరాయం_ముసుగు(శూన్య);

/**
 * @brief Clear current interrupt mask and set given mask
 *
 * @param mask Interrupt mask
 */
శూన్య v_పోర్టు_Clear_అంతరాయం_ముసుగు(పూర్ణాంకం ముసుగు);

/**
 * @brief Perform a context switch from a task
 *
 * @note [refactor-todo] The rest of ESP-IDF should call taskYield() instead
 */
శూన్య v_పోర్టు_Yield(శూన్య);

/**
 * @brief Perform a context switch from an ISR
 */
శూన్య v_పోర్టు_Yield_From_ISR(శూన్య);

/**
 * @brief Yields the other core
 *
 * @note Added to be compatible with SMP API
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
 * @note [refactor-todo] Check if this function should be renamed (due to bool return type)
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
శూన్య v_పోర్టు_సమితి_దొంతర_Watchpoint(శూన్య *px_దొంతర_ప్రారంభం);

/**
 * @brief Get the current core's ID
 *
 * @note Added to be compatible with SMP API
 * @note [refactor-todo] IDF should call a FreeRTOS like macro instead of port function directly
 * @return BaseType_t Core ID
 */
FORCE_INLINE_ATTR పునాది_Type_t x_పోర్టు_Get_Core_ID(శూన్య)
{
    తిరిగి (పునాది_Type_t) esp_cpu_get_core_id();
}

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

#నిర్వచించు పోర్టు_DISABLE_INTERRUPTS()            పోర్టు_సమితి_అంతరాయం_ముసుగు_FROM_ISR()
#ఒకవేళ !SOC_INT_CLIC_SUPPORTED
#నిర్వచించు పోర్టు_ENABLE_INTERRUPTS()             పోర్టు_CLEAR_అంతరాయం_ముసుగు_FROM_ISR(RVHAL_INTR_ENABLE_THRESH)
#లేకపోతే
#నిర్వచించు పోర్టు_ENABLE_INTERRUPTS()             పోర్టు_CLEAR_అంతరాయం_ముసుగు_FROM_ISR(RVHAL_INTR_ENABLE_THRESH_CLIC)
#అయితే_ముగింపు /* !SOC_INT_CLIC_SUPPORTED */

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

#ఒకవేళ (config_అంకె_CORES > 1)
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
#నిర్వచించు పోర్టు_ENTER_CRITICAL(mux)                     v_పోర్టు_Enter_Critical_Multi_Core(mux)
#నిర్వచించు పోర్టు_EXIT_CRITICAL(mux)                      v_పోర్టు_Exit_Critical_Multi_Core(mux)
#అయితే_ముగింపు /* CONFIG_FREERTOS_CHECK_PORT_CRITICAL_COMPLIANCE */

#నిర్వచించు పోర్టు_TRY_ENTER_CRITICAL_ISR(mux, timeout)    x_పోర్టు_Enter_Critical_Timeout(mux, timeout)
#నిర్వచించు పోర్టు_ENTER_CRITICAL_ISR(mux)                 v_పోర్టు_Enter_Critical_Multi_Core(mux)
#నిర్వచించు పోర్టు_EXIT_CRITICAL_ISR(mux)                  v_పోర్టు_Exit_Critical_Multi_Core(mux)

#నిర్వచించు పోర్టు_TRY_ENTER_CRITICAL_సురక్షిత(mux, timeout)   x_పోర్టు_Enter_Critical_Timeout_సురక్షిత(mux, timeout)
#నిర్వచించు పోర్టు_ENTER_CRITICAL_సురక్షిత(mux)                v_పోర్టు_Enter_Critical_సురక్షిత(mux)
#నిర్వచించు పోర్టు_EXIT_CRITICAL_సురక్షిత(mux)                 v_పోర్టు_Exit_Critical_సురక్షిత(mux)
#లేకపోతే
/* Single-core variants of the critical section macros */
#నిర్వచించు పోర్టు_ENTER_CRITICAL(mux)                 {(శూన్య)mux;  v_పోర్టు_Enter_Critical();}
#నిర్వచించు పోర్టు_EXIT_CRITICAL(mux)                  {(శూన్య)mux;  v_పోర్టు_Exit_Critical();}
#నిర్వచించు పోర్టు_TRY_ENTER_CRITICAL(mux, timeout)    ({  \
    (శూన్య)mux; (శూన్య)timeout;                       \
    v_పోర్టు_Enter_Critical();                           \
    పునాది_Type_t ret = pdPASS;                        \
    ret;                                            \
})

//In single-core RISC-V, we can use the same critical section API
#నిర్వచించు పోర్టు_ENTER_CRITICAL_ISR(mux)                 పోర్టు_ENTER_CRITICAL(mux)
#నిర్వచించు పోర్టు_EXIT_CRITICAL_ISR(mux)                  పోర్టు_EXIT_CRITICAL(mux)
#నిర్వచించు పోర్టు_TRY_ENTER_CRITICAL_ISR(mux, timeout)    పోర్టు_TRY_ENTER_CRITICAL(mux, timeout)

/* [refactor-todo] on RISC-V, both ISR and non-ISR cases result in the same call. We can redefine this macro */
#నిర్వచించు పోర్టు_ENTER_CRITICAL_సురక్షిత(mux)    ({  \
    ఒకవేళ (x_పోర్టు_లోపల_Isr_Context()) {              \
        పోర్టు_ENTER_CRITICAL_ISR(mux);        \
    } లేకపోతే {                                \
        పోర్టు_ENTER_CRITICAL(mux);            \
    }                                       \
})
#నిర్వచించు పోర్టు_EXIT_CRITICAL_సురక్షిత(mux)     ({  \
    ఒకవేళ (x_పోర్టు_లోపల_Isr_Context()) {              \
        పోర్టు_EXIT_CRITICAL_ISR(mux);         \
    } లేకపోతే {                                \
        పోర్టు_EXIT_CRITICAL(mux);             \
    }                                       \
})
#నిర్వచించు పోర్టు_TRY_ENTER_CRITICAL_సురక్షిత(mux, timeout)   ({  \
    (శూన్య)timeout;                                      \
    పోర్టు_ENTER_CRITICAL_సురక్షిత(mux);                       \
    పునాది_Type_t ret = pdPASS;                            \
    ret;                                                \
})

#అయితే_ముగింపు /* (configNUM_CORES > 1) */

// ---------------------- Yielding -------------------------

#నిర్వచించు పోర్టు_YIELD() v_పోర్టు_Yield()
#నిర్వచించు పోర్టు_YIELD_FROM_ISR_NO_ARG() v_పోర్టు_Yield_From_ISR()
#నిర్వచించు పోర్టు_YIELD_FROM_ISR_ARG(x_Higher_ప్రాధాన్యత_Task_Woken) ({ \
    ఒకవేళ (x_Higher_ప్రాధాన్యత_Task_Woken == pdTRUE) { \
        v_పోర్టు_Yield_From_ISR(); \
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


#నిర్వచించు పోర్టు_చివర_SWITCHING_ISR(xSwitchRequired) ఒకవేళ(xSwitchRequired) v_పోర్టు_Yield()
/* Yielding within an API call (when interrupts are off), means the yield should be delayed
   until interrupts are re-enabled.
   To do this, we use the "cross-core" interrupt as a trigger to yield on this core when interrupts are re-enabled.This
   is the same interrupt & code path which is used to trigger a yield between CPUs, although in this case the yield is
   happening on the same CPU.
*/
#నిర్వచించు పోర్టు_YIELD_WITHIN_API() పోర్టు_YIELD()

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
 * - For implementation of non-inlined functions, see port.c, port_common.c, or other assembly files
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

// ------------------ Critical Sections --------------------

#ఒకవేళ (config_అంకె_CORES > 1)
స్థిర అంతఃవరుస శూన్య __attribute__((always_inline)) v_పోర్టు_Enter_Critical_Multi_Core(పోర్టు_MUX_TYPE *mux)
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
#అయితే_ముగింపు /* (configNUM_CORES > 1) */

// ---------------------- Yielding -------------------------

FORCE_INLINE_ATTR bool x_పోర్టు_Can_Yield(శూన్య)
{

#ఒకవేళ SOC_INT_CLIC_SUPPORTED
    /* When CLIC is supported:
     *  - The lowest interrupt threshold level is 0. Therefore, an interrupt threshold level above 0 would mean that we
     *    are in a critical section.
     *  - Since CLIC enables HW interrupt nesting, we do not have the updated interrupt level in the
     *    INTERRUPT_CURRENT_CORE_INT_THRESH_REG register when nested interrupts occur. To know the current interrupt
     *    level, we read the machine-mode interrupt level (mil) field from the mintstatus CSR. A non-zero value indicates
     *    that we are in an interrupt context.
     */
    uint32_t పరిమితి = rv_utils_get_అంతరాయం_పరిమితి();
    uint32_t intr_level = rv_utils_get_అంతరాయం_level();
    తిరిగి ((intr_level == 0) && (పరిమితి == 0));
#లేకపోతే/* !SOC_INT_CLIC_SUPPORTED */
    uint32_t పరిమితి = REG_READ(అంతరాయం_CURRENT_CORE_INT_THRESH_REG);
    /* when enter critical code, FreeRTOS will mask threshold to RVHAL_EXCM_LEVEL
     * and exit critical code, will recover threshold value (1). so threshold <= 1
     * means not in critical code
     */
    తిరిగి (పరిమితి <= 1);
#అయితే_ముగింపు
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
బాహ్య అస్థిర UBaseType_t x_పోర్టు_Switch_Flag[పోర్టు_అంకె_PROCESSORS];
#నిర్వచించు os_task_switch_is_pended(_cpu_) (x_పోర్టు_Switch_Flag[_cpu_])
#లేకపోతే
#నిర్వచించు os_task_switch_is_pended(_cpu_) (false)
#అయితే_ముగింపు

// -------------- FPU softerware retention ------------------
#ఒకవేళ (SOC_CPU_COPROC_అంకె > 0) && SOC_CPU_HAS_FPU && SOC_PM_FPU_RETENTION_BY_SW
/**
 * @brief Whether the FPU context is dirty on the given core.
 *
 * Returns non-zero if any task has used the FPU, such context should be
 *                  saved during sleep retention.
 *
 * @param core_id Core id
 * @return pdTRUE  FPU context is dirty
 * @return pdFALSE FPU was not used
 */
పునాది_Type_t x_పోర్టు_FPUContext_Is_Dirty(పునాది_Type_t core_id);
#అయితే_ముగింపు

#నిర్వచితమైతే __cplusplus
}
#అయితే_ముగింపు

#అయితే_ముగింపు //__ASSEMBLER__

#అయితే_ముగింపు /* PORTMACRO_H */
