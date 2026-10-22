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

#अगर_अघोषित PORTMACRO_H
#घोषणा PORTMACRO_H

#समावेश "sdkconfig.h"
#समावेश "freertos/FreeRTOSConfig.h"

/* Macros used instead ofsetoff() for better performance of interrupt handler */
#अगर CONFIG_FREERTOS_USE_LIST_तथ्य_INTEGRITY_CHECK_BYTES
#घोषणा PORT_OFFSET_PX_ढेर 0x40
#अन्यथा
#घोषणा PORT_OFFSET_PX_ढेर 0x30
#पूर्ण_अगर /* #if CONFIG_FREERTOS_USE_LIST_DATA_INTEGRITY_CHECK_BYTES */

#अगर CONFIG_FREERTOS_UNICORE
#घोषणा CORE_ID_आकार        0
#अन्यथा
#घोषणा CORE_ID_आकार        4
#पूर्ण_अगर

/* Align the value up to the nearest multiple of 4 */
#घोषणा PORT_ALIGN_UP_TO_4(मूल्य) (((मूल्य) + 3) & ~3)

#घोषणा PORT_OFFSET_PX_END_OF_ढेर ( \
    PORT_OFFSET_PX_ढेर \
    + 4                                 /* void * pxDummy6 */ \
    + PORT_ALIGN_UP_TO_4(configMAX_TASK_NAME_LEN) /* uint8_t ucDummy7[ configMAX_TASK_NAME_LEN ] */ \
    + CORE_ID_आकार                      /* BaseType_t xDummyCoreID */ \
)

#अगर_अघोषित __ASSEMBLER__

#समावेश <stdint.h>
#समावेश <stdlib.h>
#समावेश <stdbool.h>
#समावेश <stdio.h>
#समावेश "spinlock.h"
#समावेश "soc/interrupt_reg.h"
#समावेश "esp_macros.h"
#समावेश "esp_attr.h"
#समावेश "esp_cpu.h"
#समावेश "esp_rom_sys.h"
#समावेश "esp_heap_caps.h"
#समावेश "esp_system.h"             /* required by esp_get_...() functions in portable.h. [refactor-todo] Update portable.h */
#समावेश "esp_newlib.h"
#समावेश <limits.h>

#अगर_घोषित __cplusplus
बाह्य "C" {
#पूर्ण_अगर

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come first as they are used further down in this file
 * ------------------------------------------------------------------------------------------------------------------ */

#घोषणा portCHAR                    uint8_t
#घोषणा portFLOAT                   भग्न
#घोषणा portDOUBLE                  द्विगुन
#घोषणा portLONG                    int32_t
#घोषणा portSHORT                   int16_t
#घोषणा port_ढेर_TYPE              uint8_t
#घोषणा portBASE_TYPE               पूर्णांक

प्रकार port_ढेर_TYPE              ढेर_Type_t;
प्रकार portBASE_TYPE               BaseType_t;
प्रकार अचिन्हित portBASE_TYPE      UBaseType_t;

#अगर (configUSE_16_BIT_TICKS == 1)
प्रकार uint16_t TickType_t;
#घोषणा portMAX_DELAY (TickType_t)  0xffff
#अन्यथा
प्रकार uint32_t TickType_t;
#घोषणा portMAX_DELAY (TickType_t)  0xffffffffUL
#पूर्ण_अगर

/* Task function macros as described on the FreeRTOS.org WEB site. */
#घोषणा portTASK_FUNCTION_PROTO(vFunction, pvParameters) व्योम vFunction(व्योम *pvParameters)
#घोषणा portTASK_FUNCTION(vFunction, pvParameters) व्योम vFunction(व्योम *pvParameters)


/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#घोषणा portCRITICAL_NESTING_IN_TCB     0
#घोषणा port_ढेर_GROWTH                (-1)
#घोषणा portTICK_PERIOD_MS              ((TickType_t) (1000 / configTICK_RATE_HZ))
#घोषणा portBYTE_ALIGNMENT              16
#घोषणा portTICK_TYPE_IS_ATOMIC         1
#घोषणा portNOP() __asm अस्थिर        (" nop ")



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
UBaseType_t xPortSetInterruptMaskFromISR(व्योम);

/**
 * @brief Re-enable interrupts in a nested manner (meant to be called from ISRs)
 *
 * @warning Only applies to current CPU.
 * @param prev_int_level Previous interrupt level
 */
व्योम vPortClearInterruptMaskFromISR(UBaseType_t पूर्व_int_level);

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
व्योम vPortEnterCritical(व्योम);

/**
 * @brief Exit a critical section
 *
 * - Reenables interrupts
 * - Can be nested
 */
व्योम vPortExitCritical(व्योम);

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

#अगर (config_अंक_CORES > 1)
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
BaseType_t xPortEnterCriticalTimeout(portMUX_TYPE *mux, BaseType_t timeout);

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
अटल अंतरभूत व्योम __attribute__((always_inline)) vPortEnterCriticalMultiCore(portMUX_TYPE *mux);

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
व्योम vPortExitCriticalMultiCore(portMUX_TYPE *mux);

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
#पूर्ण_अगर /* (configNUM_CORES > 1) */

// ---------------------- Yielding -------------------------

/**
 * @brief Set interrupt mask and return current interrupt enable register
 *
 * @note [refactor-todo] Check if this function should be renamed (due to int return type)
 * @return int Current interrupt enable register before set
 */
पूर्णांक vPortSetInterruptMask(व्योम);

/**
 * @brief Clear current interrupt mask and set given mask
 *
 * @param mask Interrupt mask
 */
व्योम vPortClearInterruptMask(पूर्णांक mask);

/**
 * @brief Perform a context switch from a task
 *
 * @note [refactor-todo] The rest of ESP-IDF should call taskYield() instead
 */
व्योम vPortYield(व्योम);

/**
 * @brief Perform a context switch from an ISR
 */
व्योम vPortYieldFromISR(व्योम);

/**
 * @brief Yields the other core
 *
 * @note Added to be compatible with SMP API
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
 * @note [refactor-todo] Check if this function should be renamed (due to bool return type)
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
व्योम v_Port_Set_ढेर_Watchpoint(व्योम *px_ढेर_Start);

/**
 * @brief Get the current core's ID
 *
 * @note Added to be compatible with SMP API
 * @note [refactor-todo] IDF should call a FreeRTOS like macro instead of port function directly
 * @return BaseType_t Core ID
 */
FORCE_INLINE_ATTR BaseType_t xPortGetCoreID(व्योम)
{
    वापस (BaseType_t) esp_cpu_get_core_id();
}

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

#घोषणा portDISABLE_INTERRUPTS()            portSET_INTERRUPT_MASK_FROM_ISR()
#अगर !SOC_INT_CLIC_SUPPORTED
#घोषणा portENABLE_INTERRUPTS()             portCLEAR_INTERRUPT_MASK_FROM_ISR(RVHAL_INTR_ENABLE_THRESH)
#अन्यथा
#घोषणा portENABLE_INTERRUPTS()             portCLEAR_INTERRUPT_MASK_FROM_ISR(RVHAL_INTR_ENABLE_THRESH_CLIC)
#पूर्ण_अगर /* !SOC_INT_CLIC_SUPPORTED */

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

#अगर (config_अंक_CORES > 1)
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
#घोषणा portENTER_CRITICAL(mux)                     vPortEnterCriticalMultiCore(mux)
#घोषणा portEXIT_CRITICAL(mux)                      vPortExitCriticalMultiCore(mux)
#पूर्ण_अगर /* CONFIG_FREERTOS_CHECK_PORT_CRITICAL_COMPLIANCE */

#घोषणा portTRY_ENTER_CRITICAL_ISR(mux, timeout)    xPortEnterCriticalTimeout(mux, timeout)
#घोषणा portENTER_CRITICAL_ISR(mux)                 vPortEnterCriticalMultiCore(mux)
#घोषणा portEXIT_CRITICAL_ISR(mux)                  vPortExitCriticalMultiCore(mux)

#घोषणा portTRY_ENTER_CRITICAL_SAFE(mux, timeout)   xPortEnterCriticalTimeoutSafe(mux, timeout)
#घोषणा portENTER_CRITICAL_SAFE(mux)                vPortEnterCriticalSafe(mux)
#घोषणा portEXIT_CRITICAL_SAFE(mux)                 vPortExitCriticalSafe(mux)
#अन्यथा
/* Single-core variants of the critical section macros */
#घोषणा portENTER_CRITICAL(mux)                 {(व्योम)mux;  vPortEnterCritical();}
#घोषणा portEXIT_CRITICAL(mux)                  {(व्योम)mux;  vPortExitCritical();}
#घोषणा portTRY_ENTER_CRITICAL(mux, timeout)    ({  \
    (व्योम)mux; (व्योम)timeout;                       \
    vPortEnterCritical();                           \
    BaseType_t ret = pdPASS;                        \
    ret;                                            \
})

//In single-core RISC-V, we can use the same critical section API
#घोषणा portENTER_CRITICAL_ISR(mux)                 portENTER_CRITICAL(mux)
#घोषणा portEXIT_CRITICAL_ISR(mux)                  portEXIT_CRITICAL(mux)
#घोषणा portTRY_ENTER_CRITICAL_ISR(mux, timeout)    portTRY_ENTER_CRITICAL(mux, timeout)

/* [refactor-todo] on RISC-V, both ISR and non-ISR cases result in the same call. We can redefine this macro */
#घोषणा portENTER_CRITICAL_SAFE(mux)    ({  \
    अगर (xPortInIsrContext()) {              \
        portENTER_CRITICAL_ISR(mux);        \
    } अन्यथा {                                \
        portENTER_CRITICAL(mux);            \
    }                                       \
})
#घोषणा portEXIT_CRITICAL_SAFE(mux)     ({  \
    अगर (xPortInIsrContext()) {              \
        portEXIT_CRITICAL_ISR(mux);         \
    } अन्यथा {                                \
        portEXIT_CRITICAL(mux);             \
    }                                       \
})
#घोषणा portTRY_ENTER_CRITICAL_SAFE(mux, timeout)   ({  \
    (व्योम)timeout;                                      \
    portENTER_CRITICAL_SAFE(mux);                       \
    BaseType_t ret = pdPASS;                            \
    ret;                                                \
})

#पूर्ण_अगर /* (configNUM_CORES > 1) */

// ---------------------- Yielding -------------------------

#घोषणा portYIELD() vPortYield()
#घोषणा portYIELD_FROM_ISR_NO_ARG() vPortYieldFromISR()
#घोषणा portYIELD_FROM_ISR_ARG(xHigherPriorityTaskWoken) ({ \
    अगर (xHigherPriorityTaskWoken == pdTRUE) { \
        vPortYieldFromISR(); \
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


#घोषणा portEND_SWITCHING_ISR(xSwitchRequired) अगर(xSwitchRequired) vPortYield()
/* Yielding within an API call (when interrupts are off), means the yield should be delayed
   until interrupts are re-enabled.
   To do this, we use the "cross-core" interrupt as a trigger to yield on this core when interrupts are re-enabled.This
   is the same interrupt & code path which is used to trigger a yield between CPUs, although in this case the yield is
   happening on the same CPU.
*/
#घोषणा portYIELD_WITHIN_API() portYIELD()

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
 * - For implementation of non-inlined functions, see port.c, port_common.c, or other assembly files
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

// ------------------ Critical Sections --------------------

#अगर (config_अंक_CORES > 1)
अटल अंतरभूत व्योम __attribute__((always_inline)) vPortEnterCriticalMultiCore(portMUX_TYPE *mux)
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
#पूर्ण_अगर /* (configNUM_CORES > 1) */

// ---------------------- Yielding -------------------------

FORCE_INLINE_ATTR bool xPortCanYield(व्योम)
{

#अगर SOC_INT_CLIC_SUPPORTED
    /* When CLIC is supported:
     *  - The lowest interrupt threshold level is 0. Therefore, an interrupt threshold level above 0 would mean that we
     *    are in a critical section.
     *  - Since CLIC enables HW interrupt nesting, we do not have the updated interrupt level in the
     *    INTERRUPT_CURRENT_CORE_INT_THRESH_REG register when nested interrupts occur. To know the current interrupt
     *    level, we read the machine-mode interrupt level (mil) field from the mintstatus CSR. A non-zero value indicates
     *    that we are in an interrupt context.
     */
    uint32_t threshold = rv_utils_get_interrupt_threshold();
    uint32_t intr_level = rv_utils_get_interrupt_level();
    वापस ((intr_level == 0) && (threshold == 0));
#अन्यथा/* !SOC_INT_CLIC_SUPPORTED */
    uint32_t threshold = REG_READ(INTERRUPT_CURRENT_CORE_INT_THRESH_REG);
    /* when enter critical code, FreeRTOS will mask threshold to RVHAL_EXCM_LEVEL
     * and exit critical code, will recover threshold value (1). so threshold <= 1
     * means not in critical code
     */
    वापस (threshold <= 1);
#पूर्ण_अगर
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
बाह्य अस्थिर UBaseType_t xPortSwitchFlag[port_अंक_PROCESSORS];
#घोषणा os_task_switch_is_pended(_cpu_) (xPortSwitchFlag[_cpu_])
#अन्यथा
#घोषणा os_task_switch_is_pended(_cpu_) (false)
#पूर्ण_अगर

// -------------- FPU softerware retention ------------------
#अगर (SOC_CPU_COPROC_अंक > 0) && SOC_CPU_HAS_FPU && SOC_PM_FPU_RETENTION_BY_SW
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
BaseType_t xPortFPUContextIsDirty(BaseType_t core_id);
#पूर्ण_अगर

#अगर_घोषित __cplusplus
}
#पूर्ण_अगर

#पूर्ण_अगर //__ASSEMBLER__

#पूर्ण_अगर /* PORTMACRO_H */
