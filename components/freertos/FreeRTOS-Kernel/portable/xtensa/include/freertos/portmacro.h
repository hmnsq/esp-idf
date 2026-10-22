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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ PORTMACRO_H
#ವ್ಯಾಖ್ಯಾನಿಸು PORTMACRO_H

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __ASSEMBLER__

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <stdarg.h>
#ಅನ್ತರ್ಭಾವ <xtensa/config/core.h>
#ಅನ್ತರ್ಭಾವ <xtensa/xtruntime.h>       /* required for XTOS_SET_INTLEVEL. [refactor-todo] add common intr functions to esp_hw_support */
#ಅನ್ತರ್ಭಾವ "xt_instr_macros.h"
#ಅನ್ತರ್ಭಾವ "spinlock.h"
#ಅನ್ತರ್ಭಾವ "esp_private/crosscore_int.h"
#ಅನ್ತರ್ಭಾವ "esp_macros.h"
#ಅನ್ತರ್ಭಾವ "esp_attr.h"
#ಅನ್ತರ್ಭಾವ "esp_cpu.h"
#ಅನ್ತರ್ಭಾವ "esp_memory_utils.h"
#ಅನ್ತರ್ಭಾವ "esp_newlib.h"             /* required for esp_reent_init() in tasks.c */
#ಅನ್ತರ್ಭಾವ "esp_heap_caps.h"
#ಅನ್ತರ್ಭಾವ "esp_rom_sys.h"
#ಅನ್ತರ್ಭಾವ "esp_system.h"             /* required by esp_get_...() functions in portable.h. [refactor-todo] Update portable.h */
#ಅನ್ತರ್ಭಾವ "portbenchmark.h"
#ಅನ್ತರ್ಭಾವ <limits.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
ಬಾಹ್ಯ "C" {
#ಆದರೆ_ಅಂತ್ಯ


/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come before first as they are used further down the file
 * ------------------------------------------------------------------------------------------------------------------ */

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_CHAR                    int8_t
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_FLOAT                   ದಶಮಾಂಶ
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_DOUBLE                  ದ್ವಿಗುಣ
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_LONG                    int32_t
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_SHORT                   int16_t
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ರಾಶಿ_TYPE              uint8_t
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ತಳ_TYPE               ಪೂರ್ಣಾಂಕ

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೋರ್ಟ್_ರಾಶಿ_TYPE              ರಾಶಿ_Type_t;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೋರ್ಟ್_ತಳ_TYPE               ತಳ_Type_t;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಪೋರ್ಟ್_ತಳ_TYPE      UBaseType_t;

#ಆದರೆ( config_USE_16_ಬಿಟ್_TICKS == 1 )
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ uint16_t TickType_t;
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_MAX_DELAY ( TickType_t ) 0xffff
#ಇಲ್ಲದಿದ್ದರೆ
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ uint32_t TickType_t;
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_MAX_DELAY ( TickType_t ) 0xffffffffUL
#ಆದರೆ_ಅಂತ್ಯ

/* Task function macros as described on the FreeRTOS.org WEB site. */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TASK_FUNCTION_PROTO( vFunction, pvParameters )  ಶೂನ್ಯ vFunction( ಶೂನ್ಯ *pvParameters )
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TASK_FUNCTION( vFunction, pvParameters )        ಶೂನ್ಯ vFunction( ಶೂನ್ಯ *pvParameters )



/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_CRITICAL_NESTING_ಒಳ_TCB     0
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ರಾಶಿ_GROWTH                ( -1 )
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TICK_PERIOD_MS              ( ( TickType_t ) 1000 / config_TICK_ದರ_HZ )
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_BYTE_ALIGNMENT              16    // Xtensa Windowed ABI requires the stack pointer to always be 16-byte aligned. See "isa_rm.pdf 8.1.1 Windowed Register Usage and Stack Layout"
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TICK_TYPE_IS_ATOMIC         1
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_NOP()                       XT_NOP()



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
ತಳ_Type_t x_ಪೋರ್ಟ್_ಒಳ_Isr_Context(ಶೂನ್ಯ);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortInIsrContext() internally
 */
ಶೂನ್ಯ v_ಪೋರ್ಟ್_Assert_If_ಒಳ_ISR(ಶೂನ್ಯ);

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
ತಳ_Type_t x_ಪೋರ್ಟ್_Interrupted_From_ISRContext(ಶೂನ್ಯ);

/**
 * @brief Disable interrupts in a nested manner (meant to be called from ISRs)
 *
 * @warning Only applies to current CPU.
 * @return UBaseType_t Previous interrupt level
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ UBaseType_t x_ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ_From_ISR(ಶೂನ್ಯ);

/**
 * @brief Re-enable interrupts in a nested manner (meant to be called from ISRs)
 *
 * @warning Only applies to current CPU.
 * @param prev_level Previous interrupt level
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_ಪೋರ್ಟ್_Clear_ಅಡಚಣೆ_ಮುಖವಾಡ_From_ISR(UBaseType_t prev_level);

/* ---------------------- Spinlocks ------------------------
 * - Modifications made to critical sections to support SMP
 * - See "Critical Sections & Disabling Interrupts" in docs/api-guides/freertos-smp.rst for more details
 * - Remark: For the ESP32, portENTER_CRITICAL and portENTER_CRITICAL_ISR both alias vPortEnterCritical, meaning that
 *           either function can be called both from ISR as well as task context. This is not standard FreeRTOS
 *           behavior; please keep this in mind if you need any compatibility with other FreeRTOS implementations.
 * @note [refactor-todo] Check if these comments are still true
 * ------------------------------------------------------ */

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ spinlock_t                          ಪೋರ್ಟ್_MUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_MUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_MUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_MUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_MUX_TRY_ಬೀಗ                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

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
ತಳ_Type_t x_ಪೋರ್ಟ್_Enter_Critical_Timeout(ಪೋರ್ಟ್_MUX_TYPE *mux, ತಳ_Type_t timeout);

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
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ __attribute__((always_inline)) v_ಪೋರ್ಟ್_Enter_Critical(ಪೋರ್ಟ್_MUX_TYPE *mux);

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
ಶೂನ್ಯ v_ಪೋರ್ಟ್_Exit_Critical(ಪೋರ್ಟ್_MUX_TYPE *mux);

#ಆದರೆ CONFIG_FREERTOS_ಪೋರ್ಟ್_THREAD_ಸುರಕ್ಷಿತ_CLAIM
/**
 * @brief Claim thread-safe region start
 *        If claimed, vPortEnterCritical/vPortExitCritical on the current core are no-ops.
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
ಶೂನ್ಯ x_ಪೋರ್ಟ್_Thread_ಸುರಕ್ಷಿತ_Claim(ಶೂನ್ಯ);

/**
 * @brief Claim thread-safe region end
 *        Restores normal port critical behavior
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
ಶೂನ್ಯ x_ಪೋರ್ಟ್_Thread_ಸುರಕ್ಷಿತ_Disclaim(ಶೂನ್ಯ);
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

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
ತಳ_Type_t x_ಪೋರ್ಟ್_Enter_Critical_Timeout_Compliance(ಪೋರ್ಟ್_MUX_TYPE *mux, ತಳ_Type_t timeout);

/**
 * @brief FreeRTOS compliant version of vPortEnterCritical()
 *
 * Compliant version of vPortEnterCritical() will ensure that this is
 * called from a task context only. An abort is called otherwise.
 *
 * @param[in] mux Spinlock
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ __attribute__((always_inline)) v_ಪೋರ್ಟ್_Enter_Critical_Compliance(ಪೋರ್ಟ್_MUX_TYPE *mux);

/**
 * @brief FreeRTOS compliant version of vPortExitCritical()
 *
 * Compliant version of vPortExitCritical() will ensure that this is
 * called from a task context only. An abort is called otherwise.
 *
 * @note This function is made non-inline on purpose to reduce code size
 * @param[in] mux Spinlock
 */
ಶೂನ್ಯ v_ಪೋರ್ಟ್_Exit_Critical_Compliance(ಪೋರ್ಟ್_MUX_TYPE *mux);

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
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಳ_Type_t __attribute__((always_inline)) x_ಪೋರ್ಟ್_Enter_Critical_Timeout_ಸುರಕ್ಷಿತ(ಪೋರ್ಟ್_MUX_TYPE *mux, ತಳ_Type_t timeout);

/**
 * @brief Safe version of enter critical
 *
 * Safe version of enter critical will automatically select between
 * portENTER_CRITICAL() and portENTER_CRITICAL_ISR()
 *
 * @param[in] mux Spinlock
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ __attribute__((always_inline)) v_ಪೋರ್ಟ್_Enter_Critical_ಸುರಕ್ಷಿತ(ಪೋರ್ಟ್_MUX_TYPE *mux);

/**
 * @brief Safe version of exit critical
 *
 * Safe version of enter critical will automatically select between
 * portEXIT_CRITICAL() and portEXIT_CRITICAL_ISR()
 *
 * @param[in] mux Spinlock
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ __attribute__((always_inline)) v_ಪೋರ್ಟ್_Exit_Critical_ಸುರಕ್ಷಿತ(ಪೋರ್ಟ್_MUX_TYPE *mux);

// ---------------------- Yielding -------------------------

/**
 * @brief Perform a solicited context switch
 *
 * - Defined in portasm.S
 *
 * @note [refactor-todo] The rest of ESP-IDF should call taskYield() instead
 */
ಶೂನ್ಯ v_ಪೋರ್ಟ್_Yield( ಶೂನ್ಯ );

/**
 * @brief Yields the other core
 *
 * - Send an interrupt to another core in order to make the task running on it yield for a higher-priority task.
 * - Can be used to yield current core as well
 *
 * @note [refactor-todo] Put this into private macros as its only called from task.c and is not public API
 * @param coreid ID of core to yield
 */
ಶೂನ್ಯ v_ಪೋರ್ಟ್_Yield_Other_Core(ತಳ_Type_t coreid);

/**
 * @brief Checks if the current core can yield
 *
 * - A core cannot yield if its in an ISR or in a critical section
 *
 * @note [refactor-todo] See if this can be separated from port macro
 * @return true Core can yield
 * @return false Core cannot yield
 */
FORCE_INLINE_ATTR bool x_ಪೋರ್ಟ್_Can_Yield(ಶೂನ್ಯ);

// ------------------- Hook Functions ----------------------

/**
 * @brief Hook function called on entry to tickless idle
 *
 * - Implemented in pm_impl.c
 *
 * @param xExpectedIdleTime Expected idle time
 */
ಶೂನ್ಯ vApplicationSleep(TickType_t x_Expected_Idle_ಸಮಯ);

// ----------------------- System --------------------------

/**
 * @brief Get the tick rate per second
 *
 * @deprecated This function will be removed in IDF 7.0. Use CONFIG_FREERTOS_HZ directly instead.
 * @note [refactor-todo] Remove this function in IDF 7.0 (IDF-14115)
 * @return uint32_t Tick rate in Hz
 */
uint32_t x_ಪೋರ್ಟ್_Get_Tick_ದರ_Hz(ಶೂನ್ಯ) __attribute__((deprecated("This function will be removed in IDF 7.0. Use CONFIG_FREERTOS_HZ directly instead.")));

/**
 * @brief Set a watchpoint to watch the last 32 bytes of the stack
 *
 * Callback to set a watchpoint on the end of the stack. Called every context switch to change the stack watchpoint
 * around.
 *
 * @param pxStackStart Pointer to the start of the stack
 */
ಶೂನ್ಯ v_ಪೋರ್ಟ್_ಗಣ_ರಾಶಿ_Watchpoint( ಶೂನ್ಯ *px_ರಾಶಿ_ಆರಂಭ );

/**
 * @brief Get the current core's ID
 *
 * @note [refactor-todo] IDF should call a FreeRTOS like macro instead of port function directly
 * @return BaseType_t Core ID
 */
FORCE_INLINE_ATTR ತಳ_Type_t x_ಪೋರ್ಟ್_Get_Core_ID(ಶೂನ್ಯ);

// --------------------- TCB Cleanup -----------------------

/**
 * @brief TCB cleanup hook
 *
 * The portCLEAN_UP_TCB() macro is called in prvDeleteTCB() right before a
 * deleted task's memory is freed. We map that macro to this internal function
 * so that IDF FreeRTOS ports can inject some task pre-deletion operations.
 */
ಶೂನ್ಯ v_ಪೋರ್ಟ್_TCBPre_Delete_Hook( ಶೂನ್ಯ *pxTCB );


/* ------------------------------------------- FreeRTOS Porting Interface ----------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS
 * - Most come after forward declare as porting macros map to declared functions
 * - Maps to forward declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- System --------------------------

#ಆದರೆ ( config_ಸಂಖ್ಯೆ_ದ_CORES > 1 )
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_GET_CORE_ID()       x_ಪೋರ್ಟ್_Get_Core_ID()
#ಇಲ್ಲದಿದ್ದರೆ /* configNUMBER_OF_CORES > 1 */
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_GET_CORE_ID()       ((ತಳ_Type_t) 0);
#ಆದರೆ_ಅಂತ್ಯ /* configNUMBER_OF_CORES > 1 */

// --------------------- Interrupts ------------------------

/**
 * - Only applies to current core
 * - These cannot be nested. They should be used with a lot of care and cannot be called from interrupt level.
 *
 * @note [refactor-todo] replace XTOS_SET_INTLEVEL with more efficient version, if any?
 */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_DISABLE_INTERRUPTS()            ಮಾಡು { XTOS_ಗಣ_INTLEVEL(XCHAL_EXCM_LEVEL); portbenchmark_ಅಡಚಣೆ_DISABLE(); } ಆಗ (0)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ENABLE_INTERRUPTS()             ಮಾಡು { portbenchmark_ಅಡಚಣೆ_RESTORE(0); XTOS_ಗಣ_INTLEVEL(0); } ಆಗ (0)

/**
 * ISR versions to enable/disable interrupts
 */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR()                   x_ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ_From_ISR()
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_CLEAR_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR(prev_level)       v_ಪೋರ್ಟ್_Clear_ಅಡಚಣೆ_ಮುಖವಾಡ_From_ISR(prev_level)

/**
 * @brief Assert if in ISR context
 */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ASSERT_IF_ಒಳ_ISR() v_ಪೋರ್ಟ್_Assert_If_ಒಳ_ISR()

/**
 * @brief Used by FreeRTOS functions to call the correct version of critical section API
 */
#ಆದರೆ ( config_ಅಂಕೆ_CORES > 1 )
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_CHECK_IF_ಒಳ_ISR()   x_ಪೋರ್ಟ್_ಒಳ_Isr_Context()
#ಆದರೆ_ಅಂತ್ಯ

// ------------------ Critical Sections --------------------

/**
 * @brief FreeRTOS critical section macros
 *
 * - Added a spinlock argument for SMP
 * - Can be nested
 * - Compliance versions will assert if regular critical section API is used in ISR context
 * - Safe versions can be called from either contexts
 */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FREERTOS_CHECK_ಪೋರ್ಟ್_CRITICAL_COMPLIANCE
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TRY_ENTER_CRITICAL(mux, timeout)        x_ಪೋರ್ಟ್_Enter_Critical_Timeout_Compliance(mux, timeout)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ENTER_CRITICAL(mux)                     v_ಪೋರ್ಟ್_Enter_Critical_Compliance(mux)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_EXIT_CRITICAL(mux)                      v_ಪೋರ್ಟ್_Exit_Critical_Compliance(mux)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TRY_ENTER_CRITICAL(mux, timeout)        x_ಪೋರ್ಟ್_Enter_Critical_Timeout(mux, timeout)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ENTER_CRITICAL(mux)                     v_ಪೋರ್ಟ್_Enter_Critical(mux)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_EXIT_CRITICAL(mux)                      v_ಪೋರ್ಟ್_Exit_Critical(mux)
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_CHECK_PORT_CRITICAL_COMPLIANCE */

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TRY_ENTER_CRITICAL_ISR(mux, timeout)    x_ಪೋರ್ಟ್_Enter_Critical_Timeout(mux, timeout)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ENTER_CRITICAL_ISR(mux)                 v_ಪೋರ್ಟ್_Enter_Critical(mux)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_EXIT_CRITICAL_ISR(mux)                  v_ಪೋರ್ಟ್_Exit_Critical(mux)

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TRY_ENTER_CRITICAL_ಸುರಕ್ಷಿತ(mux, timeout)   x_ಪೋರ್ಟ್_Enter_Critical_Timeout_ಸುರಕ್ಷಿತ(mux, timeout)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ENTER_CRITICAL_ಸುರಕ್ಷಿತ(mux)                v_ಪೋರ್ಟ್_Enter_Critical_ಸುರಕ್ಷಿತ(mux)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_EXIT_CRITICAL_ಸುರಕ್ಷಿತ(mux)                 v_ಪೋರ್ಟ್_Exit_Critical_ಸುರಕ್ಷಿತ(mux)

// ---------------------- Yielding -------------------------

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_YIELD() v_ಪೋರ್ಟ್_Yield()

ಬಾಹ್ಯ ಶೂನ್ಯ frxt_ಸಿದ್ಧತೆ_switch( ಶೂನ್ಯ );     //Defined in portasm.S

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_YIELD_FROM_ISR_NO_ARG() ({ \
    trace_ISR_EXIT_ಗಮ್ಯ_SCHEDULER(); \
    frxt_ಸಿದ್ಧತೆ_switch(); \
})
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_YIELD_FROM_ISR_ARG(x_Higher_ಆದ್ಯತೆ_Task_Woken) ({ \
    ಆದರೆ (x_Higher_ಆದ್ಯತೆ_Task_Woken == pdTRUE) { \
        trace_ISR_EXIT_ಗಮ್ಯ_SCHEDULER(); \
        frxt_ಸಿದ್ಧತೆ_switch(); \
    } \
})

/**
 * @note    The macro below could be used when passing a single argument, or without any argument,
 *          it was developed to support both usages of portYIELD inside of an ISR. Any other usage form
 *          might result in undesired behavior
 */
#ಆದರೆ defined(__cplusplus) && (__cplusplus >  201703L)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_YIELD_FROM_ISR(...) ಆರಿಸು_MACRO_VA_ARG(ಪೋರ್ಟ್_YIELD_FROM_ISR_ARG, ಪೋರ್ಟ್_YIELD_FROM_ISR_NO_ARG __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_YIELD_FROM_ISR(...) ಆರಿಸು_MACRO_VA_ARG(ಪೋರ್ಟ್_YIELD_FROM_ISR_ARG, ಪೋರ್ಟ್_YIELD_FROM_ISR_NO_ARG, ##__VA_ARGS__)(__VA_ARGS__)
#ಆದರೆ_ಅಂತ್ಯ

/* Yielding within an API call (when interrupts are off), means the yield should be delayed
   until interrupts are re-enabled.

   To do this, we use the "cross-core" interrupt as a trigger to yield on this core when interrupts are re-enabled.This
   is the same interrupt & code path which is used to trigger a yield between CPUs, although in this case the yield is
   happening on the same CPU.
*/
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_YIELD_WITHIN_API() esp_crosscore_int_send_yield(x_ಪೋರ್ಟ್_Get_Core_ID())

#ಆದರೆ ( config_ಸಂಖ್ಯೆ_ದ_CORES > 1 )
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_YIELD_CORE( xCoreID )     v_ಪೋರ್ಟ್_Yield_Other_Core( xCoreID )
#ಆದರೆ_ಅಂತ್ಯ /* configNUMBER_OF_CORES > 1 */

// ------------------- Hook Functions ----------------------

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_SUPPRESS_TICKS_AND_SLEEP(idle_ಸಮಯ) vApplicationSleep(idle_ಸಮಯ)

// ------------------- Run Time Stats ----------------------

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_CONFIGURE_TIMER_FOR_ಸತತ_ಸಮಯ_STATS()

#ಆದರೆ ( CONFIG_FREERTOS_GENERATE_ಸತತ_ಸಮಯ_STATS )
config_ಸತತ_ಸಮಯ_COUNTER_TYPE x_ಪೋರ್ಟ್_Get_ಸತತ_ಸಮಯ_Counter_ಮೌಲ್ಯ( ಶೂನ್ಯ );
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_GET_ಸತತ_ಸಮಯ_COUNTER_ಮೌಲ್ಯ()        x_ಪೋರ್ಟ್_Get_ಸತತ_ಸಮಯ_Counter_ಮೌಲ್ಯ()
#ಆದರೆ_ಅಂತ್ಯ

// --------------------- TCB Cleanup -----------------------

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_CLEAN_ಮೇಲೆ_TCB( pxTCB ) v_ಪೋರ್ಟ್_TCBPre_Delete_Hook( pxTCB )

// -------------- Optimized Task Selection -----------------

#ಆದರೆ config_USE_ಪೋರ್ಟ್_OPTIMISED_TASK_SELECTION == 1
/* Check the configuration. */
#ಆದರೆ( configMAX_PRIORITIES > 32 )
#ದೋಷ config_USE_ಪೋರ್ಟ್_OPTIMISED_TASK_SELECTION can only be ಗಣ ಗಮ್ಯ 1 when configMAX_PRIORITIES is less than or equal ಗಮ್ಯ 32.  It is very rare that a system requires ಇನ್ನಷ್ಟು than 10 ಗಮ್ಯ 15 different priorities as tasks that share a ಆದ್ಯತೆ will time ತುಂಡು.
#ಆದರೆ_ಅಂತ್ಯ

/* Store/clear the ready priorities in a bit map. */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ದಾಖಲೆ_READY_ಆದ್ಯತೆ( ux_ಆದ್ಯತೆ, uxReadyPriorities ) ( uxReadyPriorities ) |= ( 1UL << ( ux_ಆದ್ಯತೆ ) )
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_RESET_READY_ಆದ್ಯತೆ( ux_ಆದ್ಯತೆ, uxReadyPriorities ) ( uxReadyPriorities ) &= ~( 1UL << ( ux_ಆದ್ಯತೆ ) )
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_GET_HIGHEST_ಆದ್ಯತೆ( ux_ತುದಿ_ಆದ್ಯತೆ, uxReadyPriorities ) ux_ತುದಿ_ಆದ್ಯತೆ = ( 31 - __builtin_clz( ( uxReadyPriorities ) ) )
#ಆದರೆ_ಅಂತ್ಯ /* configUSE_PORT_OPTIMISED_TASK_SELECTION */



/* --------------------------------------------- Inline Implementations ------------------------------------------------
 * - Implementation of inline functions of the forward declares
 * - Should come after forward declare and FreeRTOS Porting interface, as implementation may use both.
 * - For implementation of non-inlined functions, see port.c
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

ಸ್ಥಿರ ಸಾಲೊಳಗೆ UBaseType_t __attribute__((always_inline)) x_ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ_From_ISR(ಶೂನ್ಯ)
{
    UBaseType_t prev_int_level = XTOS_ಗಣ_INTLEVEL(XCHAL_EXCM_LEVEL);
    portbenchmark_ಅಡಚಣೆ_DISABLE();
    ಹಿಂತಿರುಗಿ prev_int_level;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ __attribute__((always_inline)) v_ಪೋರ್ಟ್_Clear_ಅಡಚಣೆ_ಮುಖವಾಡ_From_ISR(UBaseType_t prev_level)
{
    portbenchmark_ಅಡಚಣೆ_RESTORE(prev_level);
    XTOS_RESTORE_JUST_INTLEVEL((ಪೂರ್ಣಾಂಕ) prev_level);
}

// ------------------ Critical Sections --------------------

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ __attribute__((always_inline)) v_ಪೋರ್ಟ್_Enter_Critical(ಪೋರ್ಟ್_MUX_TYPE *mux)
{
    x_ಪೋರ್ಟ್_Enter_Critical_Timeout(mux, ಪೋರ್ಟ್_MUX_NO_TIMEOUT);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ __attribute__((always_inline)) v_ಪೋರ್ಟ್_Enter_Critical_Compliance(ಪೋರ್ಟ್_MUX_TYPE *mux)
{
    x_ಪೋರ್ಟ್_Enter_Critical_Timeout_Compliance(mux, ಪೋರ್ಟ್_MUX_NO_TIMEOUT);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಳ_Type_t __attribute__((always_inline)) x_ಪೋರ್ಟ್_Enter_Critical_Timeout_ಸುರಕ್ಷಿತ(ಪೋರ್ಟ್_MUX_TYPE *mux, ತಳ_Type_t timeout)
{
    ತಳ_Type_t ret;
    ಆದರೆ (x_ಪೋರ್ಟ್_ಒಳ_Isr_Context()) {
        ret = ಪೋರ್ಟ್_TRY_ENTER_CRITICAL_ISR(mux, timeout);
    } ಇಲ್ಲದಿದ್ದರೆ {
        ret = ಪೋರ್ಟ್_TRY_ENTER_CRITICAL(mux, timeout);
    }
    ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ __attribute__((always_inline)) v_ಪೋರ್ಟ್_Enter_Critical_ಸುರಕ್ಷಿತ(ಪೋರ್ಟ್_MUX_TYPE *mux)
{
    x_ಪೋರ್ಟ್_Enter_Critical_Timeout_ಸುರಕ್ಷಿತ(mux, ಪೋರ್ಟ್_MUX_NO_TIMEOUT);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ __attribute__((always_inline)) v_ಪೋರ್ಟ್_Exit_Critical_ಸುರಕ್ಷಿತ(ಪೋರ್ಟ್_MUX_TYPE *mux)
{
    ಆದರೆ (x_ಪೋರ್ಟ್_ಒಳ_Isr_Context()) {
        ಪೋರ್ಟ್_EXIT_CRITICAL_ISR(mux);
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಪೋರ್ಟ್_EXIT_CRITICAL(mux);
    }
}

// ---------------------- Yielding -------------------------

FORCE_INLINE_ATTR bool x_ಪೋರ್ಟ್_Can_Yield(ಶೂನ್ಯ)
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

    ಹಿಂತಿರುಗಿ ((ps_reg & PS_INTLEVEL_ಮುಖವಾಡ) == 0);
}

// ----------------------- System --------------------------

FORCE_INLINE_ATTR ತಳ_Type_t x_ಪೋರ್ಟ್_Get_Core_ID(ಶೂನ್ಯ)
{
    ಹಿಂತಿರುಗಿ (ತಳ_Type_t) esp_cpu_get_core_id();
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
bool x_ಪೋರ್ಟ್_Check_Valid_List_Mem(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool x_ಪೋರ್ಟ್_Check_Valid_TCBMem(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_ರಾಶಿ_Mem(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ptr);

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_VALID_LIST_MEM(ptr)     x_ಪೋರ್ಟ್_Check_Valid_List_Mem(ptr)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_VALID_TCB_MEM(ptr)      x_ಪೋರ್ಟ್_Check_Valid_TCBMem(ptr)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_VALID_ರಾಶಿ_MEM(ptr)    x_Portcheck_Valid_ರಾಶಿ_Mem(ptr)

// --------------------- App-Trace -------------------------

#ಆದರೆ CONFIG_ESP_TRACE_ENABLE
ಬಾಹ್ಯ ಅಸ್ಥಿರ uint32_t ಪೋರ್ಟ್_switch_flag[ಪೋರ್ಟ್_ಅಂಕೆ_PROCESSORS];
#ವ್ಯಾಖ್ಯಾನಿಸು os_task_switch_is_pended(_cpu_) (ಪೋರ್ಟ್_switch_flag[_cpu_])
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು os_task_switch_is_pended(_cpu_) (false)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ // __ASSEMBLER__

#ಆದರೆ_ಅಂತ್ಯ /* PORTMACRO_H */
