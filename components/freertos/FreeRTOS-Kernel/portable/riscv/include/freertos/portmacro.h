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

#که_نه_تعریف_شوی PORTMACRO_H
#تعریف_کړه PORTMACRO_H

#ضمن "sdkconfig.h"
#ضمن "freertos/FreeRTOSConfig.h"

/* Macros used instead ofsetoff() for better performance of interrupt handler */
#إذا CONFIG_FREERTOS_USE_LIST_معلومات_INTEGRITY_CHECK_بایټونه
#تعریف_کړه پورټ_دننه_واټن_PX_ډېری 0x40
#وإلا
#تعریف_کړه پورټ_دننه_واټن_PX_ډېری 0x30
#د_که_پای /* #if CONFIG_FREERTOS_USE_LIST_DATA_INTEGRITY_CHECK_BYTES */

#إذا CONFIG_FREERTOS_UNICORE
#تعریف_کړه CORE_ID_اندازه        0
#وإلا
#تعریف_کړه CORE_ID_اندازه        4
#د_که_پای

/* Align the value up to the nearest multiple of 4 */
#تعریف_کړه پورټ_ALIGN_پورته_منزل_4(ارزښت) (((ارزښت) + 3) & ~3)

#تعریف_کړه پورټ_دننه_واټن_PX_پای_ټکی_د_ډېری ( \
    پورټ_دننه_واټن_PX_ډېری \
    + 4                                 /* void * pxDummy6 */ \
    + پورټ_ALIGN_پورته_منزل_4(configMAX_TASK_NAME_LEN) /* uint8_t ucDummy7[ configMAX_TASK_NAME_LEN ] */ \
    + CORE_ID_اندازه                      /* BaseType_t xDummyCoreID */ \
)

#که_نه_تعریف_شوی __ASSEMBLER__

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

#که_تعریف_شوی __cplusplus
خارجي "C" {
#د_که_پای

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come first as they are used further down in this file
 * ------------------------------------------------------------------------------------------------------------------ */

#تعریف_کړه پورټ_CHAR                    uint8_t
#تعریف_کړه پورټ_FLOAT                   عائم
#تعریف_کړه پورټ_DOUBLE                  مزدوج
#تعریف_کړه پورټ_LONG                    int32_t
#تعریف_کړه پورټ_SHORT                   int16_t
#تعریف_کړه پورټ_ډېری_TYPE              uint8_t
#تعریف_کړه پورټ_بنسټ_TYPE               صحيح

تعريفنوع پورټ_ډېری_TYPE              ډېری_Type_t;
تعريفنوع پورټ_بنسټ_TYPE               بنسټ_Type_t;
تعريفنوع بینښه پورټ_بنسټ_TYPE      UBaseType_t;

#إذا (config_USE_16_بټ_TICKS == 1)
تعريفنوع uint16_t TickType_t;
#تعریف_کړه پورټ_MAX_DELAY (TickType_t)  0xffff
#وإلا
تعريفنوع uint32_t TickType_t;
#تعریف_کړه پورټ_MAX_DELAY (TickType_t)  0xffffffffUL
#د_که_پای

/* Task function macros as described on the FreeRTOS.org WEB site. */
#تعریف_کړه پورټ_TASK_FUNCTION_PROTO(vFunction, pvParameters) فراغ vFunction(فراغ *pvParameters)
#تعریف_کړه پورټ_TASK_FUNCTION(vFunction, pvParameters) فراغ vFunction(فراغ *pvParameters)


/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#تعریف_کړه پورټ_CRITICAL_NESTING_دننه_TCB     0
#تعریف_کړه پورټ_ډېری_GROWTH                (-1)
#تعریف_کړه پورټ_TICK_PERIOD_MS              ((TickType_t) (1000 / config_TICK_شرح_HZ))
#تعریف_کړه پورټ_BYTE_ALIGNMENT              16
#تعریف_کړه پورټ_TICK_TYPE_IS_ATOMIC         1
#تعریف_کړه پورټ_NOP() __asm متطاير        (" nop ")



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
UBaseType_t x_پورټ_سېټ_مداخله_ماسک_From_ISR(فراغ);

/**
 * @brief Re-enable interrupts in a nested manner (meant to be called from ISRs)
 *
 * @warning Only applies to current CPU.
 * @param prev_int_level Previous interrupt level
 */
فراغ v_پورټ_Clear_مداخله_ماسک_From_ISR(UBaseType_t prev_int_level);

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
بنسټ_Type_t x_پورټ_دننه_Isr_Context(فراغ);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortInIsrContext() internally
 */
فراغ v_پورټ_Assert_If_دننه_ISR(فراغ);

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
بنسټ_Type_t x_پورټ_Interrupted_From_ISRContext(فراغ);

/* ---------------------- Spinlocks ------------------------
 * - Modifications made to critical sections to support SMP
 * - See "Critical Sections & Disabling Interrupts" in docs/api-guides/freertos-smp.rst for more details
 * - Remark: For the ESP32, portENTER_CRITICAL and portENTER_CRITICAL_ISR both alias vPortEnterCritical, meaning that
 *           either function can be called both from ISR as well as task context. This is not standard FreeRTOS
 *           behavior; please keep this in mind if you need any compatibility with other FreeRTOS implementations.
 * @note [refactor-todo] Check if these comments are still true
 * ------------------------------------------------------ */

تعريفنوع spinlock_t                          پورټ_MUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */

#تعریف_کړه پورټ_MUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#تعریف_کړه پورټ_MUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#تعریف_کړه پورټ_MUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#تعریف_کړه پورټ_MUX_TRY_قلف                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#تعریف_کړه پورټ_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

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
فراغ v_پورټ_Enter_Critical(فراغ);

/**
 * @brief Exit a critical section
 *
 * - Reenables interrupts
 * - Can be nested
 */
فراغ v_پورټ_Exit_Critical(فراغ);

#إذا CONFIG_FREERTOS_پورټ_THREAD_خوندي_CLAIM
/**
 * @brief Claim thread-safe region start
 *        If claimed, vPortEnterCritical/vPortExitCritical on the current core are no-ops.
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
فراغ x_پورټ_Thread_خوندي_Claim(فراغ);

/**
 * @brief Claim thread-safe region end
 *        Restores normal port critical behavior
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
فراغ x_پورټ_Thread_خوندي_Disclaim(فراغ);
#د_که_پای /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

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
بنسټ_Type_t x_پورټ_Enter_Critical_Timeout(پورټ_MUX_TYPE *mux, بنسټ_Type_t timeout);

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
ساكن مضمن فراغ __attribute__((always_inline)) v_پورټ_Enter_Critical_Multi_Core(پورټ_MUX_TYPE *mux);

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
فراغ v_پورټ_Exit_Critical_Multi_Core(پورټ_MUX_TYPE *mux);

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
بنسټ_Type_t x_پورټ_Enter_Critical_Timeout_Compliance(پورټ_MUX_TYPE *mux, بنسټ_Type_t timeout);

/**
 * @brief FreeRTOS compliant version of vPortEnterCritical()
 *
 * Compliant version of vPortEnterCritical() will ensure that this is
 * called from a task context only. An abort is called otherwise.
 *
 * @param[in] mux Spinlock
 */
ساكن مضمن فراغ __attribute__((always_inline)) v_پورټ_Enter_Critical_Compliance(پورټ_MUX_TYPE *mux);

/**
 * @brief FreeRTOS compliant version of vPortExitCritical()
 *
 * Compliant version of vPortExitCritical() will ensure that this is
 * called from a task context only. An abort is called otherwise.
 *
 * @note This function is made non-inline on purpose to reduce code size
 * @param[in] mux Spinlock
 */
فراغ v_پورټ_Exit_Critical_Compliance(پورټ_MUX_TYPE *mux);

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
ساكن مضمن بنسټ_Type_t __attribute__((always_inline)) x_پورټ_Enter_Critical_Timeout_خوندي(پورټ_MUX_TYPE *mux, بنسټ_Type_t timeout);

/**
 * @brief Safe version of enter critical
 *
 * Safe version of enter critical will automatically select between
 * portENTER_CRITICAL() and portENTER_CRITICAL_ISR()
 *
 * @param[in] mux Spinlock
 */
ساكن مضمن فراغ __attribute__((always_inline)) v_پورټ_Enter_Critical_خوندي(پورټ_MUX_TYPE *mux);

/**
 * @brief Safe version of exit critical
 *
 * Safe version of enter critical will automatically select between
 * portEXIT_CRITICAL() and portEXIT_CRITICAL_ISR()
 *
 * @param[in] mux Spinlock
 */
ساكن مضمن فراغ __attribute__((always_inline)) v_پورټ_Exit_Critical_خوندي(پورټ_MUX_TYPE *mux);
#د_که_پای /* (configNUM_CORES > 1) */

// ---------------------- Yielding -------------------------

/**
 * @brief Set interrupt mask and return current interrupt enable register
 *
 * @note [refactor-todo] Check if this function should be renamed (due to int return type)
 * @return int Current interrupt enable register before set
 */
صحيح v_پورټ_سېټ_مداخله_ماسک(فراغ);

/**
 * @brief Clear current interrupt mask and set given mask
 *
 * @param mask Interrupt mask
 */
فراغ v_پورټ_Clear_مداخله_ماسک(صحيح ماسک);

/**
 * @brief Perform a context switch from a task
 *
 * @note [refactor-todo] The rest of ESP-IDF should call taskYield() instead
 */
فراغ v_پورټ_Yield(فراغ);

/**
 * @brief Perform a context switch from an ISR
 */
فراغ v_پورټ_Yield_From_ISR(فراغ);

/**
 * @brief Yields the other core
 *
 * @note Added to be compatible with SMP API
 * @note [refactor-todo] Put this into private macros as its only called from task.c and is not public API
 * @param coreid ID of core to yield
 */
فراغ v_پورټ_Yield_Other_Core(بنسټ_Type_t coreid);

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
FORCE_INLINE_ATTR bool x_پورټ_Can_Yield(فراغ);

// ------------------- Hook Functions ----------------------

/**
 * @brief Hook function called on entry to tickless idle
 *
 * - Implemented in pm_impl.c
 *
 * @param xExpectedIdleTime Expected idle time
 */
فراغ vApplicationSleep(TickType_t x_Expected_Idle_وخت);

// ----------------------- System --------------------------

/**
 * @brief Get the tick rate per second
 *
 * @deprecated This function will be removed in IDF 7.0. Use CONFIG_FREERTOS_HZ directly instead.
 * @note [refactor-todo] Remove this function in IDF 7.0 (IDF-14115)
 * @return uint32_t Tick rate in Hz
 */
uint32_t x_پورټ_Get_Tick_شرح_Hz(فراغ) __attribute__((deprecated("This function will be removed in IDF 7.0. Use CONFIG_FREERTOS_HZ directly instead.")));

/**
 * @brief Set a watchpoint to watch the last 32 bytes of the stack
 *
 * Callback to set a watchpoint on the end of the stack. Called every context switch to change the stack watchpoint
 * around.
 *
 * @param pxStackStart Pointer to the start of the stack
 */
فراغ v_پورټ_سېټ_ډېری_Watchpoint(فراغ *px_ډېری_پیل);

/**
 * @brief Get the current core's ID
 *
 * @note Added to be compatible with SMP API
 * @note [refactor-todo] IDF should call a FreeRTOS like macro instead of port function directly
 * @return BaseType_t Core ID
 */
FORCE_INLINE_ATTR بنسټ_Type_t x_پورټ_Get_Core_ID(فراغ)
{
    أرجع (بنسټ_Type_t) esp_cpu_get_core_id();
}

// --------------------- TCB Cleanup -----------------------

/**
 * @brief TCB cleanup hook
 *
 * The portCLEAN_UP_TCB() macro is called in prvDeleteTCB() right before a
 * deleted task's memory is freed. We map that macro to this internal function
 * so that IDF FreeRTOS ports can inject some task pre-deletion operations.
 */
فراغ v_پورټ_TCBPre_Delete_Hook( فراغ *pxTCB );

/* ------------------------------------------- FreeRTOS Porting Interface ----------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS
 * - Most come after forward declare as porting macros map to declared functions
 * - Maps to forward declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- System --------------------------

#إذا ( config_شمېره_د_CORES > 1 )
    #تعریف_کړه پورټ_GET_CORE_ID()       x_پورټ_Get_Core_ID()
#وإلا /* configNUMBER_OF_CORES > 1 */
    #تعریف_کړه پورټ_GET_CORE_ID()       ((بنسټ_Type_t) 0);
#د_که_پای /* configNUMBER_OF_CORES > 1 */

// --------------------- Interrupts ------------------------

#تعریف_کړه پورټ_DISABLE_INTERRUPTS()            پورټ_سېټ_مداخله_ماسک_FROM_ISR()
#إذا !SOC_INT_CLIC_SUPPORTED
#تعریف_کړه پورټ_ENABLE_INTERRUPTS()             پورټ_CLEAR_مداخله_ماسک_FROM_ISR(RVHAL_INTR_ENABLE_THRESH)
#وإلا
#تعریف_کړه پورټ_ENABLE_INTERRUPTS()             پورټ_CLEAR_مداخله_ماسک_FROM_ISR(RVHAL_INTR_ENABLE_THRESH_CLIC)
#د_که_پای /* !SOC_INT_CLIC_SUPPORTED */

/**
 * ISR versions to enable/disable interrupts
 */
#تعریف_کړه پورټ_سېټ_مداخله_ماسک_FROM_ISR()                   x_پورټ_سېټ_مداخله_ماسک_From_ISR()
#تعریف_کړه پورټ_CLEAR_مداخله_ماسک_FROM_ISR(prev_level)       v_پورټ_Clear_مداخله_ماسک_From_ISR(prev_level)

/**
 * @brief Assert if in ISR context
 */
#تعریف_کړه پورټ_ASSERT_IF_دننه_ISR() v_پورټ_Assert_If_دننه_ISR()

/**
 * @brief Used by FreeRTOS functions to call the correct version of critical section API
 */
#إذا ( config_عدد_CORES > 1 )
#تعریف_کړه پورټ_CHECK_IF_دننه_ISR()   x_پورټ_دننه_Isr_Context()
#د_که_پای

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
#که_تعریف_شوی CONFIG_FREERTOS_CHECK_پورټ_CRITICAL_COMPLIANCE
#تعریف_کړه پورټ_TRY_ENTER_CRITICAL(mux, timeout)        x_پورټ_Enter_Critical_Timeout_Compliance(mux, timeout)
#تعریف_کړه پورټ_ENTER_CRITICAL(mux)                     v_پورټ_Enter_Critical_Compliance(mux)
#تعریف_کړه پورټ_EXIT_CRITICAL(mux)                      v_پورټ_Exit_Critical_Compliance(mux)
#وإلا
#تعریف_کړه پورټ_TRY_ENTER_CRITICAL(mux, timeout)        x_پورټ_Enter_Critical_Timeout(mux, timeout)
#تعریف_کړه پورټ_ENTER_CRITICAL(mux)                     v_پورټ_Enter_Critical_Multi_Core(mux)
#تعریف_کړه پورټ_EXIT_CRITICAL(mux)                      v_پورټ_Exit_Critical_Multi_Core(mux)
#د_که_پای /* CONFIG_FREERTOS_CHECK_PORT_CRITICAL_COMPLIANCE */

#تعریف_کړه پورټ_TRY_ENTER_CRITICAL_ISR(mux, timeout)    x_پورټ_Enter_Critical_Timeout(mux, timeout)
#تعریف_کړه پورټ_ENTER_CRITICAL_ISR(mux)                 v_پورټ_Enter_Critical_Multi_Core(mux)
#تعریف_کړه پورټ_EXIT_CRITICAL_ISR(mux)                  v_پورټ_Exit_Critical_Multi_Core(mux)

#تعریف_کړه پورټ_TRY_ENTER_CRITICAL_خوندي(mux, timeout)   x_پورټ_Enter_Critical_Timeout_خوندي(mux, timeout)
#تعریف_کړه پورټ_ENTER_CRITICAL_خوندي(mux)                v_پورټ_Enter_Critical_خوندي(mux)
#تعریف_کړه پورټ_EXIT_CRITICAL_خوندي(mux)                 v_پورټ_Exit_Critical_خوندي(mux)
#وإلا
/* Single-core variants of the critical section macros */
#تعریف_کړه پورټ_ENTER_CRITICAL(mux)                 {(فراغ)mux;  v_پورټ_Enter_Critical();}
#تعریف_کړه پورټ_EXIT_CRITICAL(mux)                  {(فراغ)mux;  v_پورټ_Exit_Critical();}
#تعریف_کړه پورټ_TRY_ENTER_CRITICAL(mux, timeout)    ({  \
    (فراغ)mux; (فراغ)timeout;                       \
    v_پورټ_Enter_Critical();                           \
    بنسټ_Type_t ret = pdPASS;                        \
    ret;                                            \
})

//In single-core RISC-V, we can use the same critical section API
#تعریف_کړه پورټ_ENTER_CRITICAL_ISR(mux)                 پورټ_ENTER_CRITICAL(mux)
#تعریف_کړه پورټ_EXIT_CRITICAL_ISR(mux)                  پورټ_EXIT_CRITICAL(mux)
#تعریف_کړه پورټ_TRY_ENTER_CRITICAL_ISR(mux, timeout)    پورټ_TRY_ENTER_CRITICAL(mux, timeout)

/* [refactor-todo] on RISC-V, both ISR and non-ISR cases result in the same call. We can redefine this macro */
#تعریف_کړه پورټ_ENTER_CRITICAL_خوندي(mux)    ({  \
    إذا (x_پورټ_دننه_Isr_Context()) {              \
        پورټ_ENTER_CRITICAL_ISR(mux);        \
    } وإلا {                                \
        پورټ_ENTER_CRITICAL(mux);            \
    }                                       \
})
#تعریف_کړه پورټ_EXIT_CRITICAL_خوندي(mux)     ({  \
    إذا (x_پورټ_دننه_Isr_Context()) {              \
        پورټ_EXIT_CRITICAL_ISR(mux);         \
    } وإلا {                                \
        پورټ_EXIT_CRITICAL(mux);             \
    }                                       \
})
#تعریف_کړه پورټ_TRY_ENTER_CRITICAL_خوندي(mux, timeout)   ({  \
    (فراغ)timeout;                                      \
    پورټ_ENTER_CRITICAL_خوندي(mux);                       \
    بنسټ_Type_t ret = pdPASS;                            \
    ret;                                                \
})

#د_که_پای /* (configNUM_CORES > 1) */

// ---------------------- Yielding -------------------------

#تعریف_کړه پورټ_YIELD() v_پورټ_Yield()
#تعریف_کړه پورټ_YIELD_FROM_ISR_NO_ARG() v_پورټ_Yield_From_ISR()
#تعریف_کړه پورټ_YIELD_FROM_ISR_ARG(x_Higher_لومړیتوب_Task_Woken) ({ \
    إذا (x_Higher_لومړیتوب_Task_Woken == pdTRUE) { \
        v_پورټ_Yield_From_ISR(); \
    } \
})
/**
 * @note    The macro below could be used when passing a single argument, or without any argument,
 *          it was developed to support both usages of portYIELD inside of an ISR. Any other usage form
 *          might result in undesired behavior
 */
#إذا defined(__cplusplus) && (__cplusplus >  201703L)
#تعریف_کړه پورټ_YIELD_FROM_ISR(...) ټاکنه_MACRO_VA_ARG(پورټ_YIELD_FROM_ISR_ARG, پورټ_YIELD_FROM_ISR_NO_ARG __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#وإلا
#تعریف_کړه پورټ_YIELD_FROM_ISR(...) ټاکنه_MACRO_VA_ARG(پورټ_YIELD_FROM_ISR_ARG, پورټ_YIELD_FROM_ISR_NO_ARG, ##__VA_ARGS__)(__VA_ARGS__)
#د_که_پای


#تعریف_کړه پورټ_پای_ټکی_SWITCHING_ISR(xSwitchRequired) إذا(xSwitchRequired) v_پورټ_Yield()
/* Yielding within an API call (when interrupts are off), means the yield should be delayed
   until interrupts are re-enabled.
   To do this, we use the "cross-core" interrupt as a trigger to yield on this core when interrupts are re-enabled.This
   is the same interrupt & code path which is used to trigger a yield between CPUs, although in this case the yield is
   happening on the same CPU.
*/
#تعریف_کړه پورټ_YIELD_WITHIN_API() پورټ_YIELD()

#إذا ( config_شمېره_د_CORES > 1 )
    #تعریف_کړه پورټ_YIELD_CORE( xCoreID )     v_پورټ_Yield_Other_Core( xCoreID )
#د_که_پای /* configNUMBER_OF_CORES > 1 */

// ------------------- Hook Functions ----------------------

#تعریف_کړه پورټ_SUPPRESS_TICKS_AND_SLEEP(idle_وخت) vApplicationSleep(idle_وخت)

// ------------------- Run Time Stats ----------------------

#تعریف_کړه پورټ_CONFIGURE_TIMER_FOR_پرلپسې_وخت_STATS()
#إذا ( CONFIG_FREERTOS_GENERATE_پرلپسې_وخت_STATS )
config_پرلپسې_وخت_COUNTER_TYPE x_پورټ_Get_پرلپسې_وخت_Counter_ارزښت( فراغ );
#تعریف_کړه پورټ_GET_پرلپسې_وخت_COUNTER_ارزښت()        x_پورټ_Get_پرلپسې_وخت_Counter_ارزښت()
#د_که_پای

// --------------------- TCB Cleanup -----------------------

#تعریف_کړه پورټ_CLEAN_پورته_TCB( pxTCB ) v_پورټ_TCBPre_Delete_Hook( pxTCB )

// -------------- Optimized Task Selection -----------------

#إذا config_USE_پورټ_OPTIMISED_TASK_SELECTION == 1
/* Check the configuration. */
#إذا( configMAX_PRIORITIES > 32 )
#تېروتنه config_USE_پورټ_OPTIMISED_TASK_SELECTION can only be سېټ منزل 1 when configMAX_PRIORITIES is less than or equal منزل 32.  It is very rare that a system requires نور than 10 منزل 15 different priorities as tasks that share a لومړیتوب will time ټوټه.
#د_که_پای

/* Store/clear the ready priorities in a bit map. */
#تعریف_کړه پورټ_ریکارډ_READY_لومړیتوب( ux_لومړیتوب, uxReadyPriorities ) ( uxReadyPriorities ) |= ( 1UL << ( ux_لومړیتوب ) )
#تعریف_کړه پورټ_RESET_READY_لومړیتوب( ux_لومړیتوب, uxReadyPriorities ) ( uxReadyPriorities ) &= ~( 1UL << ( ux_لومړیتوب ) )
#تعریف_کړه پورټ_GET_HIGHEST_لومړیتوب( ux_سر_لومړیتوب, uxReadyPriorities ) ux_سر_لومړیتوب = ( 31 - __builtin_clz( ( uxReadyPriorities ) ) )
#د_که_پای /* configUSE_PORT_OPTIMISED_TASK_SELECTION */


/* --------------------------------------------- Inline Implementations ------------------------------------------------
 * - Implementation of inline functions of the forward declares
 * - Should come after forward declare and FreeRTOS Porting interface, as implementation may use both.
 * - For implementation of non-inlined functions, see port.c, port_common.c, or other assembly files
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

// ------------------ Critical Sections --------------------

#إذا (config_عدد_CORES > 1)
ساكن مضمن فراغ __attribute__((always_inline)) v_پورټ_Enter_Critical_Multi_Core(پورټ_MUX_TYPE *mux)
{
    x_پورټ_Enter_Critical_Timeout(mux, پورټ_MUX_NO_TIMEOUT);
}

ساكن مضمن فراغ __attribute__((always_inline)) v_پورټ_Enter_Critical_Compliance(پورټ_MUX_TYPE *mux)
{
    x_پورټ_Enter_Critical_Timeout_Compliance(mux, پورټ_MUX_NO_TIMEOUT);
}

ساكن مضمن بنسټ_Type_t __attribute__((always_inline)) x_پورټ_Enter_Critical_Timeout_خوندي(پورټ_MUX_TYPE *mux, بنسټ_Type_t timeout)
{
    بنسټ_Type_t ret;
    إذا (x_پورټ_دننه_Isr_Context()) {
        ret = پورټ_TRY_ENTER_CRITICAL_ISR(mux, timeout);
    } وإلا {
        ret = پورټ_TRY_ENTER_CRITICAL(mux, timeout);
    }
    أرجع ret;
}

ساكن مضمن فراغ __attribute__((always_inline)) v_پورټ_Enter_Critical_خوندي(پورټ_MUX_TYPE *mux)
{
    x_پورټ_Enter_Critical_Timeout_خوندي(mux, پورټ_MUX_NO_TIMEOUT);
}

ساكن مضمن فراغ __attribute__((always_inline)) v_پورټ_Exit_Critical_خوندي(پورټ_MUX_TYPE *mux)
{
    إذا (x_پورټ_دننه_Isr_Context()) {
        پورټ_EXIT_CRITICAL_ISR(mux);
    } وإلا {
        پورټ_EXIT_CRITICAL(mux);
    }
}
#د_که_پای /* (configNUM_CORES > 1) */

// ---------------------- Yielding -------------------------

FORCE_INLINE_ATTR bool x_پورټ_Can_Yield(فراغ)
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
    uint32_t پوله = rv_utils_get_مداخله_پوله();
    uint32_t intr_level = rv_utils_get_مداخله_level();
    أرجع ((intr_level == 0) && (پوله == 0));
#وإلا/* !SOC_INT_CLIC_SUPPORTED */
    uint32_t پوله = REG_READ(مداخله_CURRENT_CORE_INT_THRESH_REG);
    /* when enter critical code, FreeRTOS will mask threshold to RVHAL_EXCM_LEVEL
     * and exit critical code, will recover threshold value (1). so threshold <= 1
     * means not in critical code
     */
    أرجع (پوله <= 1);
#د_که_پای
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
bool x_پورټ_Check_Valid_List_Mem(ثابت فراغ *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool x_پورټ_Check_Valid_TCBMem(ثابت فراغ *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_ډېری_Mem(ثابت فراغ *ptr);

#تعریف_کړه پورټ_VALID_LIST_MEM(ptr)     x_پورټ_Check_Valid_List_Mem(ptr)
#تعریف_کړه پورټ_VALID_TCB_MEM(ptr)      x_پورټ_Check_Valid_TCBMem(ptr)
#تعریف_کړه پورټ_VALID_ډېری_MEM(ptr)    x_Portcheck_Valid_ډېری_Mem(ptr)

// --------------------- App-Trace -------------------------

#إذا CONFIG_ESP_TRACE_ENABLE
خارجي متطاير UBaseType_t x_پورټ_Switch_Flag[پورټ_عدد_PROCESSORS];
#تعریف_کړه os_task_switch_is_pended(_cpu_) (x_پورټ_Switch_Flag[_cpu_])
#وإلا
#تعریف_کړه os_task_switch_is_pended(_cpu_) (false)
#د_که_پای

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
بنسټ_Type_t x_پورټ_FPUContext_Is_Dirty(بنسټ_Type_t core_id);
#د_که_پای

#که_تعریف_شوی __cplusplus
}
#د_که_پای

#د_که_پای //__ASSEMBLER__

#د_که_پای /* PORTMACRO_H */
