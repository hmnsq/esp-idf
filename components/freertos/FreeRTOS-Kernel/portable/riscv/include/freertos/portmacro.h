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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ PORTMACRO_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PORTMACRO_H

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOSConfig.h"

/* Macros used instead ofsetoff() for better performance of interrupt handler */
#ਜੇ CONFIG_FREERTOS_USE_LIST_ਅੰਕੜੇ_INTEGRITY_CHECK_ਬਾਈਟਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਵਿੱਥ_PX_ਢੇਰ 0x40
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਵਿੱਥ_PX_ਢੇਰ 0x30
#ਜੇ_ਅੰਤ /* #if CONFIG_FREERTOS_USE_LIST_DATA_INTEGRITY_CHECK_BYTES */

#ਜੇ CONFIG_FREERTOS_UNICORE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CORE_ID_ਮਾਪ        0
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CORE_ID_ਮਾਪ        4
#ਜੇ_ਅੰਤ

/* Align the value up to the nearest multiple of 4 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ALIGN_ਉੱਪਰ_ਮੰਜ਼ਿਲ_4(ਮੁੱਲ) (((ਮੁੱਲ) + 3) & ~3)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਵਿੱਥ_PX_ਸਿਰਾ_ਦਾ_ਢੇਰ ( \
    ਪੋਰਟ_ਵਿੱਥ_PX_ਢੇਰ \
    + 4                                 /* void * pxDummy6 */ \
    + ਪੋਰਟ_ALIGN_ਉੱਪਰ_ਮੰਜ਼ਿਲ_4(configMAX_TASK_NAME_LEN) /* uint8_t ucDummy7[ configMAX_TASK_NAME_LEN ] */ \
    + CORE_ID_ਮਾਪ                      /* BaseType_t xDummyCoreID */ \
)

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __ASSEMBLER__

#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "spinlock.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "soc/interrupt_reg.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_macros.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_attr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_cpu.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_rom_sys.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_heap_caps.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_system.h"             /* required by esp_get_...() functions in portable.h. [refactor-todo] Update portable.h */
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_newlib.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
ਬਾਹਰੀ "C" {
#ਜੇ_ਅੰਤ

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come first as they are used further down in this file
 * ------------------------------------------------------------------------------------------------------------------ */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_CHAR                    uint8_t
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_FLOAT                   ਦਸ਼ਮਲਵ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_DOUBLE                  ਦੁੱਗਣਾ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_LONG                    int32_t
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_SHORT                   int16_t
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਢੇਰ_TYPE              uint8_t
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਅਧਾਰ_TYPE               ਪੂਰਨ

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੋਰਟ_ਢੇਰ_TYPE              ਢੇਰ_Type_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੋਰਟ_ਅਧਾਰ_TYPE               ਅਧਾਰ_Type_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੋਰਟ_ਅਧਾਰ_TYPE      UBaseType_t;

#ਜੇ (config_USE_16_ਦੋਆਧਾਰੀ_ਅੰਕ_TICKS == 1)
ਕਿਸਮਪਰਿਭਾਸ਼ਾ uint16_t TickType_t;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_MAX_DELAY (TickType_t)  0xffff
#ਨਹੀਂਤਾਂ
ਕਿਸਮਪਰਿਭਾਸ਼ਾ uint32_t TickType_t;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_MAX_DELAY (TickType_t)  0xffffffffUL
#ਜੇ_ਅੰਤ

/* Task function macros as described on the FreeRTOS.org WEB site. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TASK_FUNCTION_PROTO(vFunction, pvParameters) ਖਾਲੀ vFunction(ਖਾਲੀ *pvParameters)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TASK_FUNCTION(vFunction, pvParameters) ਖਾਲੀ vFunction(ਖਾਲੀ *pvParameters)


/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_CRITICAL_NESTING_ਵਿੱਚ_TCB     0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਢੇਰ_GROWTH                (-1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TICK_PERIOD_MS              ((TickType_t) (1000 / config_TICK_ਦਰ_HZ))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_BYTE_ALIGNMENT              16
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TICK_TYPE_IS_ATOMIC         1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_NOP() __asm ਅਸਥਿਰ        (" nop ")



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
UBaseType_t x_ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ_From_ISR(ਖਾਲੀ);

/**
 * @brief Re-enable interrupts in a nested manner (meant to be called from ISRs)
 *
 * @warning Only applies to current CPU.
 * @param prev_int_level Previous interrupt level
 */
ਖਾਲੀ v_ਪੋਰਟ_Clear_ਰੁਕਾਵਟ_ਨਕਾਬ_From_ISR(UBaseType_t prev_int_level);

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
ਖਾਲੀ v_ਪੋਰਟ_Enter_Critical(ਖਾਲੀ);

/**
 * @brief Exit a critical section
 *
 * - Reenables interrupts
 * - Can be nested
 */
ਖਾਲੀ v_ਪੋਰਟ_Exit_Critical(ਖਾਲੀ);

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

#ਜੇ (config_ਅੰਕ_CORES > 1)
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
ਅਧਾਰ_Type_t x_ਪੋਰਟ_Enter_Critical_Timeout(ਪੋਰਟ_MUX_TYPE *mux, ਅਧਾਰ_Type_t timeout);

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
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ __attribute__((always_inline)) v_ਪੋਰਟ_Enter_Critical_Multi_Core(ਪੋਰਟ_MUX_TYPE *mux);

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
ਖਾਲੀ v_ਪੋਰਟ_Exit_Critical_Multi_Core(ਪੋਰਟ_MUX_TYPE *mux);

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
#ਜੇ_ਅੰਤ /* (configNUM_CORES > 1) */

// ---------------------- Yielding -------------------------

/**
 * @brief Set interrupt mask and return current interrupt enable register
 *
 * @note [refactor-todo] Check if this function should be renamed (due to int return type)
 * @return int Current interrupt enable register before set
 */
ਪੂਰਨ v_ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ(ਖਾਲੀ);

/**
 * @brief Clear current interrupt mask and set given mask
 *
 * @param mask Interrupt mask
 */
ਖਾਲੀ v_ਪੋਰਟ_Clear_ਰੁਕਾਵਟ_ਨਕਾਬ(ਪੂਰਨ ਨਕਾਬ);

/**
 * @brief Perform a context switch from a task
 *
 * @note [refactor-todo] The rest of ESP-IDF should call taskYield() instead
 */
ਖਾਲੀ v_ਪੋਰਟ_Yield(ਖਾਲੀ);

/**
 * @brief Perform a context switch from an ISR
 */
ਖਾਲੀ v_ਪੋਰਟ_Yield_From_ISR(ਖਾਲੀ);

/**
 * @brief Yields the other core
 *
 * @note Added to be compatible with SMP API
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
 * @note [refactor-todo] Check if this function should be renamed (due to bool return type)
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
ਖਾਲੀ v_ਪੋਰਟ_ਸੈੱਟ_ਢੇਰ_Watchpoint(ਖਾਲੀ *px_ਢੇਰ_ਸ਼ੁਰੂ);

/**
 * @brief Get the current core's ID
 *
 * @note Added to be compatible with SMP API
 * @note [refactor-todo] IDF should call a FreeRTOS like macro instead of port function directly
 * @return BaseType_t Core ID
 */
FORCE_INLINE_ATTR ਅਧਾਰ_Type_t x_ਪੋਰਟ_Get_Core_ID(ਖਾਲੀ)
{
    ਵਾਪਸ (ਅਧਾਰ_Type_t) esp_cpu_get_core_id();
}

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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_DISABLE_INTERRUPTS()            ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR()
#ਜੇ !SOC_INT_CLIC_SUPPORTED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENABLE_INTERRUPTS()             ਪੋਰਟ_CLEAR_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR(RVHAL_INTR_ENABLE_THRESH)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENABLE_INTERRUPTS()             ਪੋਰਟ_CLEAR_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR(RVHAL_INTR_ENABLE_THRESH_CLIC)
#ਜੇ_ਅੰਤ /* !SOC_INT_CLIC_SUPPORTED */

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

#ਜੇ (config_ਅੰਕ_CORES > 1)
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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENTER_CRITICAL(mux)                     v_ਪੋਰਟ_Enter_Critical_Multi_Core(mux)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_EXIT_CRITICAL(mux)                      v_ਪੋਰਟ_Exit_Critical_Multi_Core(mux)
#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_CHECK_PORT_CRITICAL_COMPLIANCE */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TRY_ENTER_CRITICAL_ISR(mux, timeout)    x_ਪੋਰਟ_Enter_Critical_Timeout(mux, timeout)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENTER_CRITICAL_ISR(mux)                 v_ਪੋਰਟ_Enter_Critical_Multi_Core(mux)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_EXIT_CRITICAL_ISR(mux)                  v_ਪੋਰਟ_Exit_Critical_Multi_Core(mux)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TRY_ENTER_CRITICAL_ਸੁਰੱਖਿਅਤ(mux, timeout)   x_ਪੋਰਟ_Enter_Critical_Timeout_ਸੁਰੱਖਿਅਤ(mux, timeout)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENTER_CRITICAL_ਸੁਰੱਖਿਅਤ(mux)                v_ਪੋਰਟ_Enter_Critical_ਸੁਰੱਖਿਅਤ(mux)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_EXIT_CRITICAL_ਸੁਰੱਖਿਅਤ(mux)                 v_ਪੋਰਟ_Exit_Critical_ਸੁਰੱਖਿਅਤ(mux)
#ਨਹੀਂਤਾਂ
/* Single-core variants of the critical section macros */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENTER_CRITICAL(mux)                 {(ਖਾਲੀ)mux;  v_ਪੋਰਟ_Enter_Critical();}
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_EXIT_CRITICAL(mux)                  {(ਖਾਲੀ)mux;  v_ਪੋਰਟ_Exit_Critical();}
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TRY_ENTER_CRITICAL(mux, timeout)    ({  \
    (ਖਾਲੀ)mux; (ਖਾਲੀ)timeout;                       \
    v_ਪੋਰਟ_Enter_Critical();                           \
    ਅਧਾਰ_Type_t ret = pdPASS;                        \
    ret;                                            \
})

//In single-core RISC-V, we can use the same critical section API
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENTER_CRITICAL_ISR(mux)                 ਪੋਰਟ_ENTER_CRITICAL(mux)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_EXIT_CRITICAL_ISR(mux)                  ਪੋਰਟ_EXIT_CRITICAL(mux)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TRY_ENTER_CRITICAL_ISR(mux, timeout)    ਪੋਰਟ_TRY_ENTER_CRITICAL(mux, timeout)

/* [refactor-todo] on RISC-V, both ISR and non-ISR cases result in the same call. We can redefine this macro */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENTER_CRITICAL_ਸੁਰੱਖਿਅਤ(mux)    ({  \
    ਜੇ (x_ਪੋਰਟ_ਵਿੱਚ_Isr_Context()) {              \
        ਪੋਰਟ_ENTER_CRITICAL_ISR(mux);        \
    } ਨਹੀਂਤਾਂ {                                \
        ਪੋਰਟ_ENTER_CRITICAL(mux);            \
    }                                       \
})
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_EXIT_CRITICAL_ਸੁਰੱਖਿਅਤ(mux)     ({  \
    ਜੇ (x_ਪੋਰਟ_ਵਿੱਚ_Isr_Context()) {              \
        ਪੋਰਟ_EXIT_CRITICAL_ISR(mux);         \
    } ਨਹੀਂਤਾਂ {                                \
        ਪੋਰਟ_EXIT_CRITICAL(mux);             \
    }                                       \
})
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TRY_ENTER_CRITICAL_ਸੁਰੱਖਿਅਤ(mux, timeout)   ({  \
    (ਖਾਲੀ)timeout;                                      \
    ਪੋਰਟ_ENTER_CRITICAL_ਸੁਰੱਖਿਅਤ(mux);                       \
    ਅਧਾਰ_Type_t ret = pdPASS;                            \
    ret;                                                \
})

#ਜੇ_ਅੰਤ /* (configNUM_CORES > 1) */

// ---------------------- Yielding -------------------------

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_YIELD() v_ਪੋਰਟ_Yield()
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_YIELD_FROM_ISR_NO_ARG() v_ਪੋਰਟ_Yield_From_ISR()
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_YIELD_FROM_ISR_ARG(x_Higher_ਤਰਜੀਹ_Task_Woken) ({ \
    ਜੇ (x_Higher_ਤਰਜੀਹ_Task_Woken == pdTRUE) { \
        v_ਪੋਰਟ_Yield_From_ISR(); \
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


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਸਿਰਾ_SWITCHING_ISR(xSwitchRequired) ਜੇ(xSwitchRequired) v_ਪੋਰਟ_Yield()
/* Yielding within an API call (when interrupts are off), means the yield should be delayed
   until interrupts are re-enabled.
   To do this, we use the "cross-core" interrupt as a trigger to yield on this core when interrupts are re-enabled.This
   is the same interrupt & code path which is used to trigger a yield between CPUs, although in this case the yield is
   happening on the same CPU.
*/
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_YIELD_WITHIN_API() ਪੋਰਟ_YIELD()

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
 * - For implementation of non-inlined functions, see port.c, port_common.c, or other assembly files
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

// ------------------ Critical Sections --------------------

#ਜੇ (config_ਅੰਕ_CORES > 1)
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ __attribute__((always_inline)) v_ਪੋਰਟ_Enter_Critical_Multi_Core(ਪੋਰਟ_MUX_TYPE *mux)
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
#ਜੇ_ਅੰਤ /* (configNUM_CORES > 1) */

// ---------------------- Yielding -------------------------

FORCE_INLINE_ATTR bool x_ਪੋਰਟ_Can_Yield(ਖਾਲੀ)
{

#ਜੇ SOC_INT_CLIC_SUPPORTED
    /* When CLIC is supported:
     *  - The lowest interrupt threshold level is 0. Therefore, an interrupt threshold level above 0 would mean that we
     *    are in a critical section.
     *  - Since CLIC enables HW interrupt nesting, we do not have the updated interrupt level in the
     *    INTERRUPT_CURRENT_CORE_INT_THRESH_REG register when nested interrupts occur. To know the current interrupt
     *    level, we read the machine-mode interrupt level (mil) field from the mintstatus CSR. A non-zero value indicates
     *    that we are in an interrupt context.
     */
    uint32_t ਹੱਦ = rv_utils_get_ਰੁਕਾਵਟ_ਹੱਦ();
    uint32_t intr_level = rv_utils_get_ਰੁਕਾਵਟ_level();
    ਵਾਪਸ ((intr_level == 0) && (ਹੱਦ == 0));
#ਨਹੀਂਤਾਂ/* !SOC_INT_CLIC_SUPPORTED */
    uint32_t ਹੱਦ = REG_READ(ਰੁਕਾਵਟ_CURRENT_CORE_INT_THRESH_REG);
    /* when enter critical code, FreeRTOS will mask threshold to RVHAL_EXCM_LEVEL
     * and exit critical code, will recover threshold value (1). so threshold <= 1
     * means not in critical code
     */
    ਵਾਪਸ (ਹੱਦ <= 1);
#ਜੇ_ਅੰਤ
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
ਬਾਹਰੀ ਅਸਥਿਰ UBaseType_t x_ਪੋਰਟ_Switch_Flag[ਪੋਰਟ_ਅੰਕ_PROCESSORS];
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ os_task_switch_is_pended(_cpu_) (x_ਪੋਰਟ_Switch_Flag[_cpu_])
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ os_task_switch_is_pended(_cpu_) (false)
#ਜੇ_ਅੰਤ

// -------------- FPU softerware retention ------------------
#ਜੇ (SOC_CPU_COPROC_ਅੰਕ > 0) && SOC_CPU_HAS_FPU && SOC_PM_FPU_RETENTION_BY_SW
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
ਅਧਾਰ_Type_t x_ਪੋਰਟ_FPUContext_Is_Dirty(ਅਧਾਰ_Type_t core_id);
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ //__ASSEMBLER__

#ਜੇ_ਅੰਤ /* PORTMACRO_H */
