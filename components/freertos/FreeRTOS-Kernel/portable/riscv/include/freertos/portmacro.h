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

#جے_بنا_تعریف PORTMACRO_H
#تعریف_کرو PORTMACRO_H

#ضمن "sdkconfig.h"
#ضمن "freertos/FreeRTOSConfig.h"

/* Macros used instead ofsetoff() for better performance of interrupt handler */
#إذا CONFIG_FREERTOS_USE_LIST_انکڑے_INTEGRITY_CHECK_بائٹاں
#تعریف_کرو پورٹ_وتھ_PX_ڈھیر 0x40
#وإلا
#تعریف_کرو پورٹ_وتھ_PX_ڈھیر 0x30
#جے_انت /* #if CONFIG_FREERTOS_USE_LIST_DATA_INTEGRITY_CHECK_BYTES */

#إذا CONFIG_FREERTOS_UNICORE
#تعریف_کرو CORE_ID_ناپ        0
#وإلا
#تعریف_کرو CORE_ID_ناپ        4
#جے_انت

/* Align the value up to the nearest multiple of 4 */
#تعریف_کرو پورٹ_ALIGN_اتے_منزل_4(قدر) (((قدر) + 3) & ~3)

#تعریف_کرو پورٹ_وتھ_PX_سرا_دا_ڈھیر ( \
    پورٹ_وتھ_PX_ڈھیر \
    + 4                                 /* void * pxDummy6 */ \
    + پورٹ_ALIGN_اتے_منزل_4(configMAX_TASK_NAME_LEN) /* uint8_t ucDummy7[ configMAX_TASK_NAME_LEN ] */ \
    + CORE_ID_ناپ                      /* BaseType_t xDummyCoreID */ \
)

#جے_بنا_تعریف __ASSEMBLER__

#ضمن <stdint.h>
#ضمن <stdlib.h>
#ضمن <stdbool.h>
#ضمن <stdio.h>
#ضمن "spinlock.h"
#ضمن "soc/interrupt_reg.h"
#ضمن "esp_macros.h"
#ضمن "esp_attr.h"
#ضمن "esp_cpu.h"
#ضمن "esp_rom_sys.h"
#ضمن "esp_heap_caps.h"
#ضمن "esp_system.h"             /* required by esp_get_...() functions in portable.h. [refactor-todo] Update portable.h */
#ضمن "esp_newlib.h"
#ضمن <limits.h>

#جے_تعریف_شدہ __cplusplus
خارجي "C" {
#جے_انت

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come first as they are used further down in this file
 * ------------------------------------------------------------------------------------------------------------------ */

#تعریف_کرو پورٹ_CHAR                    uint8_t
#تعریف_کرو پورٹ_FLOAT                   عائم
#تعریف_کرو پورٹ_DOUBLE                  مزدوج
#تعریف_کرو پورٹ_LONG                    int32_t
#تعریف_کرو پورٹ_SHORT                   int16_t
#تعریف_کرو پورٹ_ڈھیر_TYPE              uint8_t
#تعریف_کرو پورٹ_ادھار_TYPE               صحيح

تعريفنوع پورٹ_ڈھیر_TYPE              ڈھیر_Type_t;
تعريفنوع پورٹ_ادھار_TYPE               ادھار_Type_t;
تعريفنوع بنانشانی پورٹ_ادھار_TYPE      UBaseType_t;

#إذا (config_USE_16_ثنائی_ہندسہ_TICKS == 1)
تعريفنوع uint16_t TickType_t;
#تعریف_کرو پورٹ_MAX_DELAY (TickType_t)  0xffff
#وإلا
تعريفنوع uint32_t TickType_t;
#تعریف_کرو پورٹ_MAX_DELAY (TickType_t)  0xffffffffUL
#جے_انت

/* Task function macros as described on the FreeRTOS.org WEB site. */
#تعریف_کرو پورٹ_TASK_FUNCTION_PROTO(vFunction, pvParameters) فراغ vFunction(فراغ *pvParameters)
#تعریف_کرو پورٹ_TASK_FUNCTION(vFunction, pvParameters) فراغ vFunction(فراغ *pvParameters)


/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#تعریف_کرو پورٹ_CRITICAL_NESTING_وچ_TCB     0
#تعریف_کرو پورٹ_ڈھیر_GROWTH                (-1)
#تعریف_کرو پورٹ_TICK_PERIOD_MS              ((TickType_t) (1000 / config_TICK_شرح_HZ))
#تعریف_کرو پورٹ_BYTE_ALIGNMENT              16
#تعریف_کرو پورٹ_TICK_TYPE_IS_ATOMIC         1
#تعریف_کرو پورٹ_NOP() __asm متطاير        (" nop ")



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
UBaseType_t x_پورٹ_سیٹ_رکاوٹ_نقاب_From_ISR(فراغ);

/**
 * @brief Re-enable interrupts in a nested manner (meant to be called from ISRs)
 *
 * @warning Only applies to current CPU.
 * @param prev_int_level Previous interrupt level
 */
فراغ v_پورٹ_Clear_رکاوٹ_نقاب_From_ISR(UBaseType_t prev_int_level);

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
ادھار_Type_t x_پورٹ_وچ_Isr_Context(فراغ);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortInIsrContext() internally
 */
فراغ v_پورٹ_Assert_If_وچ_ISR(فراغ);

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
ادھار_Type_t x_پورٹ_Interrupted_From_ISRContext(فراغ);

/* ---------------------- Spinlocks ------------------------
 * - Modifications made to critical sections to support SMP
 * - See "Critical Sections & Disabling Interrupts" in docs/api-guides/freertos-smp.rst for more details
 * - Remark: For the ESP32, portENTER_CRITICAL and portENTER_CRITICAL_ISR both alias vPortEnterCritical, meaning that
 *           either function can be called both from ISR as well as task context. This is not standard FreeRTOS
 *           behavior; please keep this in mind if you need any compatibility with other FreeRTOS implementations.
 * @note [refactor-todo] Check if these comments are still true
 * ------------------------------------------------------ */

تعريفنوع spinlock_t                          پورٹ_MUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */

#تعریف_کرو پورٹ_MUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#تعریف_کرو پورٹ_MUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#تعریف_کرو پورٹ_MUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#تعریف_کرو پورٹ_MUX_TRY_جندرا                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#تعریف_کرو پورٹ_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

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
فراغ v_پورٹ_Enter_Critical(فراغ);

/**
 * @brief Exit a critical section
 *
 * - Reenables interrupts
 * - Can be nested
 */
فراغ v_پورٹ_Exit_Critical(فراغ);

#إذا CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
/**
 * @brief Claim thread-safe region start
 *        If claimed, vPortEnterCritical/vPortExitCritical on the current core are no-ops.
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
فراغ x_پورٹ_Thread_محفوظ_Claim(فراغ);

/**
 * @brief Claim thread-safe region end
 *        Restores normal port critical behavior
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
فراغ x_پورٹ_Thread_محفوظ_Disclaim(فراغ);
#جے_انت /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

#إذا (config_عدد_CORES > 1)
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
ادھار_Type_t x_پورٹ_Enter_Critical_Timeout(پورٹ_MUX_TYPE *mux, ادھار_Type_t timeout);

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
ساكن مضمن فراغ __attribute__((always_inline)) v_پورٹ_Enter_Critical_Multi_Core(پورٹ_MUX_TYPE *mux);

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
فراغ v_پورٹ_Exit_Critical_Multi_Core(پورٹ_MUX_TYPE *mux);

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
ادھار_Type_t x_پورٹ_Enter_Critical_Timeout_Compliance(پورٹ_MUX_TYPE *mux, ادھار_Type_t timeout);

/**
 * @brief FreeRTOS compliant version of vPortEnterCritical()
 *
 * Compliant version of vPortEnterCritical() will ensure that this is
 * called from a task context only. An abort is called otherwise.
 *
 * @param[in] mux Spinlock
 */
ساكن مضمن فراغ __attribute__((always_inline)) v_پورٹ_Enter_Critical_Compliance(پورٹ_MUX_TYPE *mux);

/**
 * @brief FreeRTOS compliant version of vPortExitCritical()
 *
 * Compliant version of vPortExitCritical() will ensure that this is
 * called from a task context only. An abort is called otherwise.
 *
 * @note This function is made non-inline on purpose to reduce code size
 * @param[in] mux Spinlock
 */
فراغ v_پورٹ_Exit_Critical_Compliance(پورٹ_MUX_TYPE *mux);

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
ساكن مضمن ادھار_Type_t __attribute__((always_inline)) x_پورٹ_Enter_Critical_Timeout_محفوظ(پورٹ_MUX_TYPE *mux, ادھار_Type_t timeout);

/**
 * @brief Safe version of enter critical
 *
 * Safe version of enter critical will automatically select between
 * portENTER_CRITICAL() and portENTER_CRITICAL_ISR()
 *
 * @param[in] mux Spinlock
 */
ساكن مضمن فراغ __attribute__((always_inline)) v_پورٹ_Enter_Critical_محفوظ(پورٹ_MUX_TYPE *mux);

/**
 * @brief Safe version of exit critical
 *
 * Safe version of enter critical will automatically select between
 * portEXIT_CRITICAL() and portEXIT_CRITICAL_ISR()
 *
 * @param[in] mux Spinlock
 */
ساكن مضمن فراغ __attribute__((always_inline)) v_پورٹ_Exit_Critical_محفوظ(پورٹ_MUX_TYPE *mux);
#جے_انت /* (configNUM_CORES > 1) */

// ---------------------- Yielding -------------------------

/**
 * @brief Set interrupt mask and return current interrupt enable register
 *
 * @note [refactor-todo] Check if this function should be renamed (due to int return type)
 * @return int Current interrupt enable register before set
 */
صحيح v_پورٹ_سیٹ_رکاوٹ_نقاب(فراغ);

/**
 * @brief Clear current interrupt mask and set given mask
 *
 * @param mask Interrupt mask
 */
فراغ v_پورٹ_Clear_رکاوٹ_نقاب(صحيح نقاب);

/**
 * @brief Perform a context switch from a task
 *
 * @note [refactor-todo] The rest of ESP-IDF should call taskYield() instead
 */
فراغ v_پورٹ_Yield(فراغ);

/**
 * @brief Perform a context switch from an ISR
 */
فراغ v_پورٹ_Yield_From_ISR(فراغ);

/**
 * @brief Yields the other core
 *
 * @note Added to be compatible with SMP API
 * @note [refactor-todo] Put this into private macros as its only called from task.c and is not public API
 * @param coreid ID of core to yield
 */
فراغ v_پورٹ_Yield_Other_Core(ادھار_Type_t coreid);

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
FORCE_INLINE_ATTR bool x_پورٹ_Can_Yield(فراغ);

// ------------------- Hook Functions ----------------------

/**
 * @brief Hook function called on entry to tickless idle
 *
 * - Implemented in pm_impl.c
 *
 * @param xExpectedIdleTime Expected idle time
 */
فراغ vApplicationSleep(TickType_t x_Expected_Idle_ویلا);

// ----------------------- System --------------------------

/**
 * @brief Get the tick rate per second
 *
 * @deprecated This function will be removed in IDF 7.0. Use CONFIG_FREERTOS_HZ directly instead.
 * @note [refactor-todo] Remove this function in IDF 7.0 (IDF-14115)
 * @return uint32_t Tick rate in Hz
 */
uint32_t x_پورٹ_Get_Tick_شرح_Hz(فراغ) __attribute__((deprecated("This function will be removed in IDF 7.0. Use CONFIG_FREERTOS_HZ directly instead.")));

/**
 * @brief Set a watchpoint to watch the last 32 bytes of the stack
 *
 * Callback to set a watchpoint on the end of the stack. Called every context switch to change the stack watchpoint
 * around.
 *
 * @param pxStackStart Pointer to the start of the stack
 */
فراغ v_پورٹ_سیٹ_ڈھیر_Watchpoint(فراغ *px_ڈھیر_شروع);

/**
 * @brief Get the current core's ID
 *
 * @note Added to be compatible with SMP API
 * @note [refactor-todo] IDF should call a FreeRTOS like macro instead of port function directly
 * @return BaseType_t Core ID
 */
FORCE_INLINE_ATTR ادھار_Type_t x_پورٹ_Get_Core_ID(فراغ)
{
    أرجع (ادھار_Type_t) esp_cpu_get_core_id();
}

// --------------------- TCB Cleanup -----------------------

/**
 * @brief TCB cleanup hook
 *
 * The portCLEAN_UP_TCB() macro is called in prvDeleteTCB() right before a
 * deleted task's memory is freed. We map that macro to this internal function
 * so that IDF FreeRTOS ports can inject some task pre-deletion operations.
 */
فراغ v_پورٹ_TCBPre_Delete_Hook( فراغ *pxTCB );

/* ------------------------------------------- FreeRTOS Porting Interface ----------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS
 * - Most come after forward declare as porting macros map to declared functions
 * - Maps to forward declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- System --------------------------

#إذا ( config_نمبر_دا_CORES > 1 )
    #تعریف_کرو پورٹ_GET_CORE_ID()       x_پورٹ_Get_Core_ID()
#وإلا /* configNUMBER_OF_CORES > 1 */
    #تعریف_کرو پورٹ_GET_CORE_ID()       ((ادھار_Type_t) 0);
#جے_انت /* configNUMBER_OF_CORES > 1 */

// --------------------- Interrupts ------------------------

#تعریف_کرو پورٹ_DISABLE_INTERRUPTS()            پورٹ_سیٹ_رکاوٹ_نقاب_FROM_ISR()
#إذا !SOC_INT_CLIC_SUPPORTED
#تعریف_کرو پورٹ_ENABLE_INTERRUPTS()             پورٹ_CLEAR_رکاوٹ_نقاب_FROM_ISR(RVHAL_INTR_ENABLE_THRESH)
#وإلا
#تعریف_کرو پورٹ_ENABLE_INTERRUPTS()             پورٹ_CLEAR_رکاوٹ_نقاب_FROM_ISR(RVHAL_INTR_ENABLE_THRESH_CLIC)
#جے_انت /* !SOC_INT_CLIC_SUPPORTED */

/**
 * ISR versions to enable/disable interrupts
 */
#تعریف_کرو پورٹ_سیٹ_رکاوٹ_نقاب_FROM_ISR()                   x_پورٹ_سیٹ_رکاوٹ_نقاب_From_ISR()
#تعریف_کرو پورٹ_CLEAR_رکاوٹ_نقاب_FROM_ISR(prev_level)       v_پورٹ_Clear_رکاوٹ_نقاب_From_ISR(prev_level)

/**
 * @brief Assert if in ISR context
 */
#تعریف_کرو پورٹ_ASSERT_IF_وچ_ISR() v_پورٹ_Assert_If_وچ_ISR()

/**
 * @brief Used by FreeRTOS functions to call the correct version of critical section API
 */
#إذا ( config_عدد_CORES > 1 )
#تعریف_کرو پورٹ_CHECK_IF_وچ_ISR()   x_پورٹ_وچ_Isr_Context()
#جے_انت

// ------------------ Critical Sections --------------------

#إذا (config_عدد_CORES > 1)
/**
 * @brief FreeRTOS critical section macros
 *
 * - Added a spinlock argument for SMP
 * - Can be nested
 * - Compliance versions will assert if regular critical section API is used in ISR context
 * - Safe versions can be called from either contexts
 */
#جے_تعریف_شدہ CONFIG_FREERTOS_CHECK_پورٹ_CRITICAL_COMPLIANCE
#تعریف_کرو پورٹ_TRY_ENTER_CRITICAL(mux, timeout)        x_پورٹ_Enter_Critical_Timeout_Compliance(mux, timeout)
#تعریف_کرو پورٹ_ENTER_CRITICAL(mux)                     v_پورٹ_Enter_Critical_Compliance(mux)
#تعریف_کرو پورٹ_EXIT_CRITICAL(mux)                      v_پورٹ_Exit_Critical_Compliance(mux)
#وإلا
#تعریف_کرو پورٹ_TRY_ENTER_CRITICAL(mux, timeout)        x_پورٹ_Enter_Critical_Timeout(mux, timeout)
#تعریف_کرو پورٹ_ENTER_CRITICAL(mux)                     v_پورٹ_Enter_Critical_Multi_Core(mux)
#تعریف_کرو پورٹ_EXIT_CRITICAL(mux)                      v_پورٹ_Exit_Critical_Multi_Core(mux)
#جے_انت /* CONFIG_FREERTOS_CHECK_PORT_CRITICAL_COMPLIANCE */

#تعریف_کرو پورٹ_TRY_ENTER_CRITICAL_ISR(mux, timeout)    x_پورٹ_Enter_Critical_Timeout(mux, timeout)
#تعریف_کرو پورٹ_ENTER_CRITICAL_ISR(mux)                 v_پورٹ_Enter_Critical_Multi_Core(mux)
#تعریف_کرو پورٹ_EXIT_CRITICAL_ISR(mux)                  v_پورٹ_Exit_Critical_Multi_Core(mux)

#تعریف_کرو پورٹ_TRY_ENTER_CRITICAL_محفوظ(mux, timeout)   x_پورٹ_Enter_Critical_Timeout_محفوظ(mux, timeout)
#تعریف_کرو پورٹ_ENTER_CRITICAL_محفوظ(mux)                v_پورٹ_Enter_Critical_محفوظ(mux)
#تعریف_کرو پورٹ_EXIT_CRITICAL_محفوظ(mux)                 v_پورٹ_Exit_Critical_محفوظ(mux)
#وإلا
/* Single-core variants of the critical section macros */
#تعریف_کرو پورٹ_ENTER_CRITICAL(mux)                 {(فراغ)mux;  v_پورٹ_Enter_Critical();}
#تعریف_کرو پورٹ_EXIT_CRITICAL(mux)                  {(فراغ)mux;  v_پورٹ_Exit_Critical();}
#تعریف_کرو پورٹ_TRY_ENTER_CRITICAL(mux, timeout)    ({  \
    (فراغ)mux; (فراغ)timeout;                       \
    v_پورٹ_Enter_Critical();                           \
    ادھار_Type_t ret = pdPASS;                        \
    ret;                                            \
})

//In single-core RISC-V, we can use the same critical section API
#تعریف_کرو پورٹ_ENTER_CRITICAL_ISR(mux)                 پورٹ_ENTER_CRITICAL(mux)
#تعریف_کرو پورٹ_EXIT_CRITICAL_ISR(mux)                  پورٹ_EXIT_CRITICAL(mux)
#تعریف_کرو پورٹ_TRY_ENTER_CRITICAL_ISR(mux, timeout)    پورٹ_TRY_ENTER_CRITICAL(mux, timeout)

/* [refactor-todo] on RISC-V, both ISR and non-ISR cases result in the same call. We can redefine this macro */
#تعریف_کرو پورٹ_ENTER_CRITICAL_محفوظ(mux)    ({  \
    إذا (x_پورٹ_وچ_Isr_Context()) {              \
        پورٹ_ENTER_CRITICAL_ISR(mux);        \
    } وإلا {                                \
        پورٹ_ENTER_CRITICAL(mux);            \
    }                                       \
})
#تعریف_کرو پورٹ_EXIT_CRITICAL_محفوظ(mux)     ({  \
    إذا (x_پورٹ_وچ_Isr_Context()) {              \
        پورٹ_EXIT_CRITICAL_ISR(mux);         \
    } وإلا {                                \
        پورٹ_EXIT_CRITICAL(mux);             \
    }                                       \
})
#تعریف_کرو پورٹ_TRY_ENTER_CRITICAL_محفوظ(mux, timeout)   ({  \
    (فراغ)timeout;                                      \
    پورٹ_ENTER_CRITICAL_محفوظ(mux);                       \
    ادھار_Type_t ret = pdPASS;                            \
    ret;                                                \
})

#جے_انت /* (configNUM_CORES > 1) */

// ---------------------- Yielding -------------------------

#تعریف_کرو پورٹ_YIELD() v_پورٹ_Yield()
#تعریف_کرو پورٹ_YIELD_FROM_ISR_NO_ARG() v_پورٹ_Yield_From_ISR()
#تعریف_کرو پورٹ_YIELD_FROM_ISR_ARG(x_Higher_ترجیح_Task_Woken) ({ \
    إذا (x_Higher_ترجیح_Task_Woken == pdTRUE) { \
        v_پورٹ_Yield_From_ISR(); \
    } \
})
/**
 * @note    The macro below could be used when passing a single argument, or without any argument,
 *          it was developed to support both usages of portYIELD inside of an ISR. Any other usage form
 *          might result in undesired behavior
 */
#إذا defined(__cplusplus) && (__cplusplus >  201703L)
#تعریف_کرو پورٹ_YIELD_FROM_ISR(...) چنو_MACRO_VA_ARG(پورٹ_YIELD_FROM_ISR_ARG, پورٹ_YIELD_FROM_ISR_NO_ARG __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#وإلا
#تعریف_کرو پورٹ_YIELD_FROM_ISR(...) چنو_MACRO_VA_ARG(پورٹ_YIELD_FROM_ISR_ARG, پورٹ_YIELD_FROM_ISR_NO_ARG, ##__VA_ARGS__)(__VA_ARGS__)
#جے_انت


#تعریف_کرو پورٹ_سرا_SWITCHING_ISR(xSwitchRequired) إذا(xSwitchRequired) v_پورٹ_Yield()
/* Yielding within an API call (when interrupts are off), means the yield should be delayed
   until interrupts are re-enabled.
   To do this, we use the "cross-core" interrupt as a trigger to yield on this core when interrupts are re-enabled.This
   is the same interrupt & code path which is used to trigger a yield between CPUs, although in this case the yield is
   happening on the same CPU.
*/
#تعریف_کرو پورٹ_YIELD_WITHIN_API() پورٹ_YIELD()

#إذا ( config_نمبر_دا_CORES > 1 )
    #تعریف_کرو پورٹ_YIELD_CORE( xCoreID )     v_پورٹ_Yield_Other_Core( xCoreID )
#جے_انت /* configNUMBER_OF_CORES > 1 */

// ------------------- Hook Functions ----------------------

#تعریف_کرو پورٹ_SUPPRESS_TICKS_AND_SLEEP(idle_ویلا) vApplicationSleep(idle_ویلا)

// ------------------- Run Time Stats ----------------------

#تعریف_کرو پورٹ_CONFIGURE_TIMER_FOR_لگاتار_ویلا_STATS()
#إذا ( CONFIG_FREERTOS_GENERATE_لگاتار_ویلا_STATS )
config_لگاتار_ویلا_COUNTER_TYPE x_پورٹ_Get_لگاتار_ویلا_Counter_قدر( فراغ );
#تعریف_کرو پورٹ_GET_لگاتار_ویلا_COUNTER_قدر()        x_پورٹ_Get_لگاتار_ویلا_Counter_قدر()
#جے_انت

// --------------------- TCB Cleanup -----------------------

#تعریف_کرو پورٹ_CLEAN_اتے_TCB( pxTCB ) v_پورٹ_TCBPre_Delete_Hook( pxTCB )

// -------------- Optimized Task Selection -----------------

#إذا config_USE_پورٹ_OPTIMISED_TASK_SELECTION == 1
/* Check the configuration. */
#إذا( configMAX_PRIORITIES > 32 )
#غلطی config_USE_پورٹ_OPTIMISED_TASK_SELECTION can only be سیٹ منزل 1 when configMAX_PRIORITIES is less than or equal منزل 32.  It is very rare that a system requires ہور than 10 منزل 15 different priorities as tasks that share a ترجیح will time ٹکڑا.
#جے_انت

/* Store/clear the ready priorities in a bit map. */
#تعریف_کرو پورٹ_ریکارڈ_READY_ترجیح( ux_ترجیح, uxReadyPriorities ) ( uxReadyPriorities ) |= ( 1UL << ( ux_ترجیح ) )
#تعریف_کرو پورٹ_RESET_READY_ترجیح( ux_ترجیح, uxReadyPriorities ) ( uxReadyPriorities ) &= ~( 1UL << ( ux_ترجیح ) )
#تعریف_کرو پورٹ_GET_HIGHEST_ترجیح( ux_چوٹی_ترجیح, uxReadyPriorities ) ux_چوٹی_ترجیح = ( 31 - __builtin_clz( ( uxReadyPriorities ) ) )
#جے_انت /* configUSE_PORT_OPTIMISED_TASK_SELECTION */


/* --------------------------------------------- Inline Implementations ------------------------------------------------
 * - Implementation of inline functions of the forward declares
 * - Should come after forward declare and FreeRTOS Porting interface, as implementation may use both.
 * - For implementation of non-inlined functions, see port.c, port_common.c, or other assembly files
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

// ------------------ Critical Sections --------------------

#إذا (config_عدد_CORES > 1)
ساكن مضمن فراغ __attribute__((always_inline)) v_پورٹ_Enter_Critical_Multi_Core(پورٹ_MUX_TYPE *mux)
{
    x_پورٹ_Enter_Critical_Timeout(mux, پورٹ_MUX_NO_TIMEOUT);
}

ساكن مضمن فراغ __attribute__((always_inline)) v_پورٹ_Enter_Critical_Compliance(پورٹ_MUX_TYPE *mux)
{
    x_پورٹ_Enter_Critical_Timeout_Compliance(mux, پورٹ_MUX_NO_TIMEOUT);
}

ساكن مضمن ادھار_Type_t __attribute__((always_inline)) x_پورٹ_Enter_Critical_Timeout_محفوظ(پورٹ_MUX_TYPE *mux, ادھار_Type_t timeout)
{
    ادھار_Type_t ret;
    إذا (x_پورٹ_وچ_Isr_Context()) {
        ret = پورٹ_TRY_ENTER_CRITICAL_ISR(mux, timeout);
    } وإلا {
        ret = پورٹ_TRY_ENTER_CRITICAL(mux, timeout);
    }
    أرجع ret;
}

ساكن مضمن فراغ __attribute__((always_inline)) v_پورٹ_Enter_Critical_محفوظ(پورٹ_MUX_TYPE *mux)
{
    x_پورٹ_Enter_Critical_Timeout_محفوظ(mux, پورٹ_MUX_NO_TIMEOUT);
}

ساكن مضمن فراغ __attribute__((always_inline)) v_پورٹ_Exit_Critical_محفوظ(پورٹ_MUX_TYPE *mux)
{
    إذا (x_پورٹ_وچ_Isr_Context()) {
        پورٹ_EXIT_CRITICAL_ISR(mux);
    } وإلا {
        پورٹ_EXIT_CRITICAL(mux);
    }
}
#جے_انت /* (configNUM_CORES > 1) */

// ---------------------- Yielding -------------------------

FORCE_INLINE_ATTR bool x_پورٹ_Can_Yield(فراغ)
{

#إذا SOC_INT_CLIC_SUPPORTED
    /* When CLIC is supported:
     *  - The lowest interrupt threshold level is 0. Therefore, an interrupt threshold level above 0 would mean that we
     *    are in a critical section.
     *  - Since CLIC enables HW interrupt nesting, we do not have the updated interrupt level in the
     *    INTERRUPT_CURRENT_CORE_INT_THRESH_REG register when nested interrupts occur. To know the current interrupt
     *    level, we read the machine-mode interrupt level (mil) field from the mintstatus CSR. A non-zero value indicates
     *    that we are in an interrupt context.
     */
    uint32_t حد = rv_utils_get_رکاوٹ_حد();
    uint32_t intr_level = rv_utils_get_رکاوٹ_level();
    أرجع ((intr_level == 0) && (حد == 0));
#وإلا/* !SOC_INT_CLIC_SUPPORTED */
    uint32_t حد = REG_READ(رکاوٹ_CURRENT_CORE_INT_THRESH_REG);
    /* when enter critical code, FreeRTOS will mask threshold to RVHAL_EXCM_LEVEL
     * and exit critical code, will recover threshold value (1). so threshold <= 1
     * means not in critical code
     */
    أرجع (حد <= 1);
#جے_انت
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
bool x_پورٹ_Check_Valid_List_Mem(ثابت فراغ *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool x_پورٹ_Check_Valid_TCBMem(ثابت فراغ *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_ڈھیر_Mem(ثابت فراغ *ptr);

#تعریف_کرو پورٹ_VALID_LIST_MEM(ptr)     x_پورٹ_Check_Valid_List_Mem(ptr)
#تعریف_کرو پورٹ_VALID_TCB_MEM(ptr)      x_پورٹ_Check_Valid_TCBMem(ptr)
#تعریف_کرو پورٹ_VALID_ڈھیر_MEM(ptr)    x_Portcheck_Valid_ڈھیر_Mem(ptr)

// --------------------- App-Trace -------------------------

#إذا CONFIG_ESP_TRACE_ENABLE
خارجي متطاير UBaseType_t x_پورٹ_Switch_Flag[پورٹ_عدد_PROCESSORS];
#تعریف_کرو os_task_switch_is_pended(_cpu_) (x_پورٹ_Switch_Flag[_cpu_])
#وإلا
#تعریف_کرو os_task_switch_is_pended(_cpu_) (false)
#جے_انت

// -------------- FPU softerware retention ------------------
#إذا (SOC_CPU_COPROC_عدد > 0) && SOC_CPU_HAS_FPU && SOC_PM_FPU_RETENTION_BY_SW
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
ادھار_Type_t x_پورٹ_FPUContext_Is_Dirty(ادھار_Type_t core_id);
#جے_انت

#جے_تعریف_شدہ __cplusplus
}
#جے_انت

#جے_انت //__ASSEMBLER__

#جے_انت /* PORTMACRO_H */
