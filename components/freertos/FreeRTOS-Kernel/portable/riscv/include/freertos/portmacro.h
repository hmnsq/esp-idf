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

#اگر_تعریف_نشده PORTMACRO_H
#تعریف_کن PORTMACRO_H

#ضمن "sdkconfig.h"
#ضمن "freertos/FreeRTOSConfig.h"

/* Macros used instead ofsetoff() for better performance of interrupt handler */
#إذا CONFIG_FREERTOS_USE_LIST_داده_INTEGRITY_CHECK_بایتها
#تعریف_کن پورت_جابه_جایی_PX_پشته 0x40
#وإلا
#تعریف_کن پورت_جابه_جایی_PX_پشته 0x30
#پایان_اگر /* #if CONFIG_FREERTOS_USE_LIST_DATA_INTEGRITY_CHECK_BYTES */

#إذا CONFIG_FREERTOS_UNICORE
#تعریف_کن CORE_ID_اندازه        0
#وإلا
#تعریف_کن CORE_ID_اندازه        4
#پایان_اگر

/* Align the value up to the nearest multiple of 4 */
#تعریف_کن پورت_ALIGN_به_بالا_مقصد_4(مقدار) (((مقدار) + 3) & ~3)

#تعریف_کن پورت_جابه_جایی_PX_انتها_از_پشته ( \
    پورت_جابه_جایی_PX_پشته \
    + 4                                 /* void * pxDummy6 */ \
    + پورت_ALIGN_به_بالا_مقصد_4(configMAX_TASK_NAME_LEN) /* uint8_t ucDummy7[ configMAX_TASK_NAME_LEN ] */ \
    + CORE_ID_اندازه                      /* BaseType_t xDummyCoreID */ \
)

#اگر_تعریف_نشده __ASSEMBLER__

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

#اگر_تعریف_شده __cplusplus
خارجي "C" {
#پایان_اگر

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come first as they are used further down in this file
 * ------------------------------------------------------------------------------------------------------------------ */

#تعریف_کن پورت_CHAR                    uint8_t
#تعریف_کن پورت_FLOAT                   عائم
#تعریف_کن پورت_DOUBLE                  مزدوج
#تعریف_کن پورت_LONG                    int32_t
#تعریف_کن پورت_SHORT                   int16_t
#تعریف_کن پورت_پشته_TYPE              uint8_t
#تعریف_کن پورت_پایه_TYPE               صحيح

تعريفنوع پورت_پشته_TYPE              پشته_Type_t;
تعريفنوع پورت_پایه_TYPE               پایه_Type_t;
تعريفنوع بیعلامت پورت_پایه_TYPE      UBaseType_t;

#إذا (config_USE_16_بیت_TICKS == 1)
تعريفنوع uint16_t TickType_t;
#تعریف_کن پورت_MAX_DELAY (TickType_t)  0xffff
#وإلا
تعريفنوع uint32_t TickType_t;
#تعریف_کن پورت_MAX_DELAY (TickType_t)  0xffffffffUL
#پایان_اگر

/* Task function macros as described on the FreeRTOS.org WEB site. */
#تعریف_کن پورت_TASK_FUNCTION_PROTO(vFunction, pvParameters) فراغ vFunction(فراغ *pvParameters)
#تعریف_کن پورت_TASK_FUNCTION(vFunction, pvParameters) فراغ vFunction(فراغ *pvParameters)


/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#تعریف_کن پورت_CRITICAL_NESTING_درون_TCB     0
#تعریف_کن پورت_پشته_GROWTH                (-1)
#تعریف_کن پورت_TICK_PERIOD_MS              ((TickType_t) (1000 / config_TICK_نرخ_HZ))
#تعریف_کن پورت_BYTE_ALIGNMENT              16
#تعریف_کن پورت_TICK_TYPE_IS_ATOMIC         1
#تعریف_کن پورت_NOP() __asm متطاير        (" nop ")



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
UBaseType_t x_پورت_مجموعه_وقفه_ماسک_From_ISR(فراغ);

/**
 * @brief Re-enable interrupts in a nested manner (meant to be called from ISRs)
 *
 * @warning Only applies to current CPU.
 * @param prev_int_level Previous interrupt level
 */
فراغ v_پورت_Clear_وقفه_ماسک_From_ISR(UBaseType_t prev_int_level);

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
پایه_Type_t x_پورت_درون_Isr_Context(فراغ);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortInIsrContext() internally
 */
فراغ v_پورت_Assert_If_درون_ISR(فراغ);

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
پایه_Type_t x_پورت_Interrupted_From_ISRContext(فراغ);

/* ---------------------- Spinlocks ------------------------
 * - Modifications made to critical sections to support SMP
 * - See "Critical Sections & Disabling Interrupts" in docs/api-guides/freertos-smp.rst for more details
 * - Remark: For the ESP32, portENTER_CRITICAL and portENTER_CRITICAL_ISR both alias vPortEnterCritical, meaning that
 *           either function can be called both from ISR as well as task context. This is not standard FreeRTOS
 *           behavior; please keep this in mind if you need any compatibility with other FreeRTOS implementations.
 * @note [refactor-todo] Check if these comments are still true
 * ------------------------------------------------------ */

تعريفنوع spinlock_t                          پورت_MUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */

#تعریف_کن پورت_MUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#تعریف_کن پورت_MUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#تعریف_کن پورت_MUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#تعریف_کن پورت_MUX_TRY_قفل                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#تعریف_کن پورت_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

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
فراغ v_پورت_Enter_Critical(فراغ);

/**
 * @brief Exit a critical section
 *
 * - Reenables interrupts
 * - Can be nested
 */
فراغ v_پورت_Exit_Critical(فراغ);

#إذا CONFIG_FREERTOS_پورت_THREAD_امن_CLAIM
/**
 * @brief Claim thread-safe region start
 *        If claimed, vPortEnterCritical/vPortExitCritical on the current core are no-ops.
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
فراغ x_پورت_Thread_امن_Claim(فراغ);

/**
 * @brief Claim thread-safe region end
 *        Restores normal port critical behavior
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
فراغ x_پورت_Thread_امن_Disclaim(فراغ);
#پایان_اگر /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

#إذا (config_رقم_CORES > 1)
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
پایه_Type_t x_پورت_Enter_Critical_Timeout(پورت_MUX_TYPE *mux, پایه_Type_t timeout);

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
ساكن مضمن فراغ __attribute__((always_inline)) v_پورت_Enter_Critical_Multi_Core(پورت_MUX_TYPE *mux);

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
فراغ v_پورت_Exit_Critical_Multi_Core(پورت_MUX_TYPE *mux);

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
پایه_Type_t x_پورت_Enter_Critical_Timeout_Compliance(پورت_MUX_TYPE *mux, پایه_Type_t timeout);

/**
 * @brief FreeRTOS compliant version of vPortEnterCritical()
 *
 * Compliant version of vPortEnterCritical() will ensure that this is
 * called from a task context only. An abort is called otherwise.
 *
 * @param[in] mux Spinlock
 */
ساكن مضمن فراغ __attribute__((always_inline)) v_پورت_Enter_Critical_Compliance(پورت_MUX_TYPE *mux);

/**
 * @brief FreeRTOS compliant version of vPortExitCritical()
 *
 * Compliant version of vPortExitCritical() will ensure that this is
 * called from a task context only. An abort is called otherwise.
 *
 * @note This function is made non-inline on purpose to reduce code size
 * @param[in] mux Spinlock
 */
فراغ v_پورت_Exit_Critical_Compliance(پورت_MUX_TYPE *mux);

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
ساكن مضمن پایه_Type_t __attribute__((always_inline)) x_پورت_Enter_Critical_Timeout_امن(پورت_MUX_TYPE *mux, پایه_Type_t timeout);

/**
 * @brief Safe version of enter critical
 *
 * Safe version of enter critical will automatically select between
 * portENTER_CRITICAL() and portENTER_CRITICAL_ISR()
 *
 * @param[in] mux Spinlock
 */
ساكن مضمن فراغ __attribute__((always_inline)) v_پورت_Enter_Critical_امن(پورت_MUX_TYPE *mux);

/**
 * @brief Safe version of exit critical
 *
 * Safe version of enter critical will automatically select between
 * portEXIT_CRITICAL() and portEXIT_CRITICAL_ISR()
 *
 * @param[in] mux Spinlock
 */
ساكن مضمن فراغ __attribute__((always_inline)) v_پورت_Exit_Critical_امن(پورت_MUX_TYPE *mux);
#پایان_اگر /* (configNUM_CORES > 1) */

// ---------------------- Yielding -------------------------

/**
 * @brief Set interrupt mask and return current interrupt enable register
 *
 * @note [refactor-todo] Check if this function should be renamed (due to int return type)
 * @return int Current interrupt enable register before set
 */
صحيح v_پورت_مجموعه_وقفه_ماسک(فراغ);

/**
 * @brief Clear current interrupt mask and set given mask
 *
 * @param mask Interrupt mask
 */
فراغ v_پورت_Clear_وقفه_ماسک(صحيح ماسک);

/**
 * @brief Perform a context switch from a task
 *
 * @note [refactor-todo] The rest of ESP-IDF should call taskYield() instead
 */
فراغ v_پورت_Yield(فراغ);

/**
 * @brief Perform a context switch from an ISR
 */
فراغ v_پورت_Yield_From_ISR(فراغ);

/**
 * @brief Yields the other core
 *
 * @note Added to be compatible with SMP API
 * @note [refactor-todo] Put this into private macros as its only called from task.c and is not public API
 * @param coreid ID of core to yield
 */
فراغ v_پورت_Yield_Other_Core(پایه_Type_t coreid);

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
FORCE_INLINE_ATTR bool x_پورت_Can_Yield(فراغ);

// ------------------- Hook Functions ----------------------

/**
 * @brief Hook function called on entry to tickless idle
 *
 * - Implemented in pm_impl.c
 *
 * @param xExpectedIdleTime Expected idle time
 */
فراغ vApplicationSleep(TickType_t x_Expected_Idle_زمان);

// ----------------------- System --------------------------

/**
 * @brief Get the tick rate per second
 *
 * @deprecated This function will be removed in IDF 7.0. Use CONFIG_FREERTOS_HZ directly instead.
 * @note [refactor-todo] Remove this function in IDF 7.0 (IDF-14115)
 * @return uint32_t Tick rate in Hz
 */
uint32_t x_پورت_Get_Tick_نرخ_Hz(فراغ) __attribute__((deprecated("This function will be removed in IDF 7.0. Use CONFIG_FREERTOS_HZ directly instead.")));

/**
 * @brief Set a watchpoint to watch the last 32 bytes of the stack
 *
 * Callback to set a watchpoint on the end of the stack. Called every context switch to change the stack watchpoint
 * around.
 *
 * @param pxStackStart Pointer to the start of the stack
 */
فراغ v_پورت_مجموعه_پشته_Watchpoint(فراغ *px_پشته_شروع);

/**
 * @brief Get the current core's ID
 *
 * @note Added to be compatible with SMP API
 * @note [refactor-todo] IDF should call a FreeRTOS like macro instead of port function directly
 * @return BaseType_t Core ID
 */
FORCE_INLINE_ATTR پایه_Type_t x_پورت_Get_Core_ID(فراغ)
{
    أرجع (پایه_Type_t) esp_cpu_get_core_id();
}

// --------------------- TCB Cleanup -----------------------

/**
 * @brief TCB cleanup hook
 *
 * The portCLEAN_UP_TCB() macro is called in prvDeleteTCB() right before a
 * deleted task's memory is freed. We map that macro to this internal function
 * so that IDF FreeRTOS ports can inject some task pre-deletion operations.
 */
فراغ v_پورت_TCBPre_Delete_Hook( فراغ *pxTCB );

/* ------------------------------------------- FreeRTOS Porting Interface ----------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS
 * - Most come after forward declare as porting macros map to declared functions
 * - Maps to forward declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- System --------------------------

#إذا ( config_عدد_از_CORES > 1 )
    #تعریف_کن پورت_GET_CORE_ID()       x_پورت_Get_Core_ID()
#وإلا /* configNUMBER_OF_CORES > 1 */
    #تعریف_کن پورت_GET_CORE_ID()       ((پایه_Type_t) 0);
#پایان_اگر /* configNUMBER_OF_CORES > 1 */

// --------------------- Interrupts ------------------------

#تعریف_کن پورت_DISABLE_INTERRUPTS()            پورت_مجموعه_وقفه_ماسک_FROM_ISR()
#إذا !SOC_INT_CLIC_SUPPORTED
#تعریف_کن پورت_ENABLE_INTERRUPTS()             پورت_CLEAR_وقفه_ماسک_FROM_ISR(RVHAL_INTR_ENABLE_THRESH)
#وإلا
#تعریف_کن پورت_ENABLE_INTERRUPTS()             پورت_CLEAR_وقفه_ماسک_FROM_ISR(RVHAL_INTR_ENABLE_THRESH_CLIC)
#پایان_اگر /* !SOC_INT_CLIC_SUPPORTED */

/**
 * ISR versions to enable/disable interrupts
 */
#تعریف_کن پورت_مجموعه_وقفه_ماسک_FROM_ISR()                   x_پورت_مجموعه_وقفه_ماسک_From_ISR()
#تعریف_کن پورت_CLEAR_وقفه_ماسک_FROM_ISR(prev_level)       v_پورت_Clear_وقفه_ماسک_From_ISR(prev_level)

/**
 * @brief Assert if in ISR context
 */
#تعریف_کن پورت_ASSERT_IF_درون_ISR() v_پورت_Assert_If_درون_ISR()

/**
 * @brief Used by FreeRTOS functions to call the correct version of critical section API
 */
#إذا ( config_رقم_CORES > 1 )
#تعریف_کن پورت_CHECK_IF_درون_ISR()   x_پورت_درون_Isr_Context()
#پایان_اگر

// ------------------ Critical Sections --------------------

#إذا (config_رقم_CORES > 1)
/**
 * @brief FreeRTOS critical section macros
 *
 * - Added a spinlock argument for SMP
 * - Can be nested
 * - Compliance versions will assert if regular critical section API is used in ISR context
 * - Safe versions can be called from either contexts
 */
#اگر_تعریف_شده CONFIG_FREERTOS_CHECK_پورت_CRITICAL_COMPLIANCE
#تعریف_کن پورت_TRY_ENTER_CRITICAL(mux, timeout)        x_پورت_Enter_Critical_Timeout_Compliance(mux, timeout)
#تعریف_کن پورت_ENTER_CRITICAL(mux)                     v_پورت_Enter_Critical_Compliance(mux)
#تعریف_کن پورت_EXIT_CRITICAL(mux)                      v_پورت_Exit_Critical_Compliance(mux)
#وإلا
#تعریف_کن پورت_TRY_ENTER_CRITICAL(mux, timeout)        x_پورت_Enter_Critical_Timeout(mux, timeout)
#تعریف_کن پورت_ENTER_CRITICAL(mux)                     v_پورت_Enter_Critical_Multi_Core(mux)
#تعریف_کن پورت_EXIT_CRITICAL(mux)                      v_پورت_Exit_Critical_Multi_Core(mux)
#پایان_اگر /* CONFIG_FREERTOS_CHECK_PORT_CRITICAL_COMPLIANCE */

#تعریف_کن پورت_TRY_ENTER_CRITICAL_ISR(mux, timeout)    x_پورت_Enter_Critical_Timeout(mux, timeout)
#تعریف_کن پورت_ENTER_CRITICAL_ISR(mux)                 v_پورت_Enter_Critical_Multi_Core(mux)
#تعریف_کن پورت_EXIT_CRITICAL_ISR(mux)                  v_پورت_Exit_Critical_Multi_Core(mux)

#تعریف_کن پورت_TRY_ENTER_CRITICAL_امن(mux, timeout)   x_پورت_Enter_Critical_Timeout_امن(mux, timeout)
#تعریف_کن پورت_ENTER_CRITICAL_امن(mux)                v_پورت_Enter_Critical_امن(mux)
#تعریف_کن پورت_EXIT_CRITICAL_امن(mux)                 v_پورت_Exit_Critical_امن(mux)
#وإلا
/* Single-core variants of the critical section macros */
#تعریف_کن پورت_ENTER_CRITICAL(mux)                 {(فراغ)mux;  v_پورت_Enter_Critical();}
#تعریف_کن پورت_EXIT_CRITICAL(mux)                  {(فراغ)mux;  v_پورت_Exit_Critical();}
#تعریف_کن پورت_TRY_ENTER_CRITICAL(mux, timeout)    ({  \
    (فراغ)mux; (فراغ)timeout;                       \
    v_پورت_Enter_Critical();                           \
    پایه_Type_t ret = pdPASS;                        \
    ret;                                            \
})

//In single-core RISC-V, we can use the same critical section API
#تعریف_کن پورت_ENTER_CRITICAL_ISR(mux)                 پورت_ENTER_CRITICAL(mux)
#تعریف_کن پورت_EXIT_CRITICAL_ISR(mux)                  پورت_EXIT_CRITICAL(mux)
#تعریف_کن پورت_TRY_ENTER_CRITICAL_ISR(mux, timeout)    پورت_TRY_ENTER_CRITICAL(mux, timeout)

/* [refactor-todo] on RISC-V, both ISR and non-ISR cases result in the same call. We can redefine this macro */
#تعریف_کن پورت_ENTER_CRITICAL_امن(mux)    ({  \
    إذا (x_پورت_درون_Isr_Context()) {              \
        پورت_ENTER_CRITICAL_ISR(mux);        \
    } وإلا {                                \
        پورت_ENTER_CRITICAL(mux);            \
    }                                       \
})
#تعریف_کن پورت_EXIT_CRITICAL_امن(mux)     ({  \
    إذا (x_پورت_درون_Isr_Context()) {              \
        پورت_EXIT_CRITICAL_ISR(mux);         \
    } وإلا {                                \
        پورت_EXIT_CRITICAL(mux);             \
    }                                       \
})
#تعریف_کن پورت_TRY_ENTER_CRITICAL_امن(mux, timeout)   ({  \
    (فراغ)timeout;                                      \
    پورت_ENTER_CRITICAL_امن(mux);                       \
    پایه_Type_t ret = pdPASS;                            \
    ret;                                                \
})

#پایان_اگر /* (configNUM_CORES > 1) */

// ---------------------- Yielding -------------------------

#تعریف_کن پورت_YIELD() v_پورت_Yield()
#تعریف_کن پورت_YIELD_FROM_ISR_NO_ARG() v_پورت_Yield_From_ISR()
#تعریف_کن پورت_YIELD_FROM_ISR_ARG(x_Higher_اولویت_Task_Woken) ({ \
    إذا (x_Higher_اولویت_Task_Woken == pdTRUE) { \
        v_پورت_Yield_From_ISR(); \
    } \
})
/**
 * @note    The macro below could be used when passing a single argument, or without any argument,
 *          it was developed to support both usages of portYIELD inside of an ISR. Any other usage form
 *          might result in undesired behavior
 */
#إذا defined(__cplusplus) && (__cplusplus >  201703L)
#تعریف_کن پورت_YIELD_FROM_ISR(...) انتخاب_MACRO_VA_ARG(پورت_YIELD_FROM_ISR_ARG, پورت_YIELD_FROM_ISR_NO_ARG __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#وإلا
#تعریف_کن پورت_YIELD_FROM_ISR(...) انتخاب_MACRO_VA_ARG(پورت_YIELD_FROM_ISR_ARG, پورت_YIELD_FROM_ISR_NO_ARG, ##__VA_ARGS__)(__VA_ARGS__)
#پایان_اگر


#تعریف_کن پورت_انتها_SWITCHING_ISR(xSwitchRequired) إذا(xSwitchRequired) v_پورت_Yield()
/* Yielding within an API call (when interrupts are off), means the yield should be delayed
   until interrupts are re-enabled.
   To do this, we use the "cross-core" interrupt as a trigger to yield on this core when interrupts are re-enabled.This
   is the same interrupt & code path which is used to trigger a yield between CPUs, although in this case the yield is
   happening on the same CPU.
*/
#تعریف_کن پورت_YIELD_WITHIN_API() پورت_YIELD()

#إذا ( config_عدد_از_CORES > 1 )
    #تعریف_کن پورت_YIELD_CORE( xCoreID )     v_پورت_Yield_Other_Core( xCoreID )
#پایان_اگر /* configNUMBER_OF_CORES > 1 */

// ------------------- Hook Functions ----------------------

#تعریف_کن پورت_SUPPRESS_TICKS_AND_SLEEP(idle_زمان) vApplicationSleep(idle_زمان)

// ------------------- Run Time Stats ----------------------

#تعریف_کن پورت_CONFIGURE_TIMER_FOR_پیاپی_زمان_STATS()
#إذا ( CONFIG_FREERTOS_GENERATE_پیاپی_زمان_STATS )
config_پیاپی_زمان_COUNTER_TYPE x_پورت_Get_پیاپی_زمان_Counter_مقدار( فراغ );
#تعریف_کن پورت_GET_پیاپی_زمان_COUNTER_مقدار()        x_پورت_Get_پیاپی_زمان_Counter_مقدار()
#پایان_اگر

// --------------------- TCB Cleanup -----------------------

#تعریف_کن پورت_CLEAN_به_بالا_TCB( pxTCB ) v_پورت_TCBPre_Delete_Hook( pxTCB )

// -------------- Optimized Task Selection -----------------

#إذا config_USE_پورت_OPTIMISED_TASK_SELECTION == 1
/* Check the configuration. */
#إذا( configMAX_PRIORITIES > 32 )
#خطا config_USE_پورت_OPTIMISED_TASK_SELECTION can only be مجموعه مقصد 1 when configMAX_PRIORITIES is less than or equal مقصد 32.  It is very rare that a system requires بیشتر than 10 مقصد 15 different priorities as tasks that share a اولویت will time برش.
#پایان_اگر

/* Store/clear the ready priorities in a bit map. */
#تعریف_کن پورت_رکورد_READY_اولویت( ux_اولویت, uxReadyPriorities ) ( uxReadyPriorities ) |= ( 1UL << ( ux_اولویت ) )
#تعریف_کن پورت_RESET_READY_اولویت( ux_اولویت, uxReadyPriorities ) ( uxReadyPriorities ) &= ~( 1UL << ( ux_اولویت ) )
#تعریف_کن پورت_GET_HIGHEST_اولویت( ux_بالاترین_اولویت, uxReadyPriorities ) ux_بالاترین_اولویت = ( 31 - __builtin_clz( ( uxReadyPriorities ) ) )
#پایان_اگر /* configUSE_PORT_OPTIMISED_TASK_SELECTION */


/* --------------------------------------------- Inline Implementations ------------------------------------------------
 * - Implementation of inline functions of the forward declares
 * - Should come after forward declare and FreeRTOS Porting interface, as implementation may use both.
 * - For implementation of non-inlined functions, see port.c, port_common.c, or other assembly files
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

// ------------------ Critical Sections --------------------

#إذا (config_رقم_CORES > 1)
ساكن مضمن فراغ __attribute__((always_inline)) v_پورت_Enter_Critical_Multi_Core(پورت_MUX_TYPE *mux)
{
    x_پورت_Enter_Critical_Timeout(mux, پورت_MUX_NO_TIMEOUT);
}

ساكن مضمن فراغ __attribute__((always_inline)) v_پورت_Enter_Critical_Compliance(پورت_MUX_TYPE *mux)
{
    x_پورت_Enter_Critical_Timeout_Compliance(mux, پورت_MUX_NO_TIMEOUT);
}

ساكن مضمن پایه_Type_t __attribute__((always_inline)) x_پورت_Enter_Critical_Timeout_امن(پورت_MUX_TYPE *mux, پایه_Type_t timeout)
{
    پایه_Type_t ret;
    إذا (x_پورت_درون_Isr_Context()) {
        ret = پورت_TRY_ENTER_CRITICAL_ISR(mux, timeout);
    } وإلا {
        ret = پورت_TRY_ENTER_CRITICAL(mux, timeout);
    }
    أرجع ret;
}

ساكن مضمن فراغ __attribute__((always_inline)) v_پورت_Enter_Critical_امن(پورت_MUX_TYPE *mux)
{
    x_پورت_Enter_Critical_Timeout_امن(mux, پورت_MUX_NO_TIMEOUT);
}

ساكن مضمن فراغ __attribute__((always_inline)) v_پورت_Exit_Critical_امن(پورت_MUX_TYPE *mux)
{
    إذا (x_پورت_درون_Isr_Context()) {
        پورت_EXIT_CRITICAL_ISR(mux);
    } وإلا {
        پورت_EXIT_CRITICAL(mux);
    }
}
#پایان_اگر /* (configNUM_CORES > 1) */

// ---------------------- Yielding -------------------------

FORCE_INLINE_ATTR bool x_پورت_Can_Yield(فراغ)
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
    uint32_t آستانه = rv_utils_get_وقفه_آستانه();
    uint32_t intr_level = rv_utils_get_وقفه_level();
    أرجع ((intr_level == 0) && (آستانه == 0));
#وإلا/* !SOC_INT_CLIC_SUPPORTED */
    uint32_t آستانه = REG_READ(وقفه_CURRENT_CORE_INT_THRESH_REG);
    /* when enter critical code, FreeRTOS will mask threshold to RVHAL_EXCM_LEVEL
     * and exit critical code, will recover threshold value (1). so threshold <= 1
     * means not in critical code
     */
    أرجع (آستانه <= 1);
#پایان_اگر
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
bool x_پورت_Check_Valid_List_Mem(ثابت فراغ *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool x_پورت_Check_Valid_TCBMem(ثابت فراغ *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_پشته_Mem(ثابت فراغ *ptr);

#تعریف_کن پورت_VALID_LIST_MEM(ptr)     x_پورت_Check_Valid_List_Mem(ptr)
#تعریف_کن پورت_VALID_TCB_MEM(ptr)      x_پورت_Check_Valid_TCBMem(ptr)
#تعریف_کن پورت_VALID_پشته_MEM(ptr)    x_Portcheck_Valid_پشته_Mem(ptr)

// --------------------- App-Trace -------------------------

#إذا CONFIG_ESP_TRACE_ENABLE
خارجي متطاير UBaseType_t x_پورت_Switch_Flag[پورت_رقم_PROCESSORS];
#تعریف_کن os_task_switch_is_pended(_cpu_) (x_پورت_Switch_Flag[_cpu_])
#وإلا
#تعریف_کن os_task_switch_is_pended(_cpu_) (false)
#پایان_اگر

// -------------- FPU softerware retention ------------------
#إذا (SOC_CPU_COPROC_رقم > 0) && SOC_CPU_HAS_FPU && SOC_PM_FPU_RETENTION_BY_SW
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
پایه_Type_t x_پورت_FPUContext_Is_Dirty(پایه_Type_t core_id);
#پایان_اگر

#اگر_تعریف_شده __cplusplus
}
#پایان_اگر

#پایان_اگر //__ASSEMBLER__

#پایان_اگر /* PORTMACRO_H */
