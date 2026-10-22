/*
 * FreeRTOS Kernel V10.4.3
 * Copyright 2020 Cambridge Consultants Ltd.
 *
 * SPDX-FileCopyrightText: 2020 Cambridge Consultants Ltd.
 *
 * SPDX-License-Identifier: MIT
 *
 * SPDX-FileContributor: 2023-2024 Espressif Systems (Shanghai) CO LTD
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
 */


#ifndef PORTMACRO_H
#define PORTMACRO_H

#ಅನ್ತರ್ಭಾವ <limits.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ "esp_macros.h"
#ಅನ್ತರ್ಭಾವ "spinlock.h"

#ifdef __cplusplus
ಬಾಹ್ಯ "C" {
#endif

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come before first as they are used further down the file
 * ------------------------------------------------------------------------------------------------------------------ */

/* Type definitions. */
#define portCHAR        ವರ್ಣ
#define portFLOAT       ಭಿನ್ನ
#define portDOUBLE      ದ್ವಿಗುಣ
#define portLONG        long
#define portSHORT       short
#define port_ರಾಶಿ_TYPE  unsigned long
#define portBASE_TYPE   long
#define portPOINTER_SIZE_TYPE intptr_t

ಪ್ರಕಾರನಿರ್ದೇಶ port_ರಾಶಿ_TYPE ರಾಶಿ_Type_t;
ಪ್ರಕಾರನಿರ್ದೇಶ portBASE_TYPE BaseType_t;
ಪ್ರಕಾರನಿರ್ದೇಶ unsigned portBASE_TYPE UBaseType_t;

ಪ್ರಕಾರನಿರ್ದೇಶ unsigned long TickType_t;
#define portMAX_DELAY ( TickType_t ) ULONG_MAX

#define portTICK_TYPE_IS_ATOMIC 1

/* Task function macros as described on the FreeRTOS.org WEB site. */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) ಶೂನ್ಯ vFunction( ಶೂನ್ಯ *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters ) ಶೂನ್ಯ vFunction( ಶೂನ್ಯ *pvParameters )
/*-----------------------------------------------------------*/

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#define portCRITICAL_NESTING_IN_TCB     1
#define port_ರಾಶಿ_GROWTH                ( -1 )
#define port_HAS_ರಾಶಿ_OVERFLOW_CHECKING ( 1 )   // Set in non-SMP POSIX port
#define portTICK_PERIOD_MS              ( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#define portTICK_RATE_MICROSECONDS      ( ( TickType_t ) 1000000 / configTICK_RATE_HZ )
#define portBYTE_ALIGNMENT              8
#define portNOP() __asm ಚಞ್ಚಲ        (" nop ")
/*-----------------------------------------------------------*/

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros need to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Spinlocks ------------------------

ಪ್ರಕಾರನಿರ್ದೇಶ spinlock_t                          portMUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */
/**< Spinlock initializer */
#define portMUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#define portMUX_FREE_VAL                    SPINLOCK_FREE           /**< Spinlock is free. [refactor-todo] check if this is still required */
#define portMUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#define portMUX_TRY_LOCK                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#define portMUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

BaseType_t xPortCheckIfInISR(ಶೂನ್ಯ);

// ------------------ Critical Sections --------------------

#ಯದಿ ( config_ಸಂಖ್ಯೆ_OF_CORES > 1 )
/*
These are always called with interrupts already disabled. We simply need to get/release the spinlocks
*/
ಬಾಹ್ಯ portMUX_TYPE port_xTaskLock;
ಬಾಹ್ಯ portMUX_TYPE port_xISRLock;

ಶೂನ್ಯ vPortTakeLock( portMUX_TYPE *lock );
ಶೂನ್ಯ vPortReleaseLock( portMUX_TYPE *lock );
#endif /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

ಶೂನ್ಯ vPortYield( ಶೂನ್ಯ );
ಬಾಹ್ಯ ಶೂನ್ಯ vPortYieldFromISR( ಶೂನ್ಯ );

#define portYIELD_FROM_ISR_CHECK(x)     ({ \
    ಯದಿ ( (x) == pdTRUE ) { \
        vPortYieldFromISR(); \
    } \
})
#define portYIELD_FROM_ISR_NO_CHECK()               vPortYieldFromISR()

// ----------------------- System --------------------------

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ BaseType_t __attribute__((always_inline)) xPortGetCoreID( ಶೂನ್ಯ );

// ----------------------- TCB Cleanup --------------------------

ಶೂನ್ಯ vPortCleanUpTCB ( ಶೂನ್ಯ *pxTCB );

/* ----------------------------------------- FreeRTOS SMP Porting Interface --------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS SMP
 * - Must come after forward declare as some porting macros map to declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

#define portDISABLE_INTERRUPTS()                    xPortSetInterruptMask()
#define portENABLE_INTERRUPTS()                     vPortClearInterruptMask(0)

// ------------------ Critical Sections --------------------

#define portENTER_CRITICAL_SMP()                    vTaskEnterCritical();
#define portEXIT_CRITICAL_SMP()                     vTaskExitCritical();

#ಯದಿ defined(__cplusplus) && (__cplusplus >  201703L)
#define portENTER_CRITICAL(...)                     CHOOSE_MACRO_VA_ARG(portENTER_CRITICAL_IDF, portENTER_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#define portEXIT_CRITICAL(...)                      CHOOSE_MACRO_VA_ARG(portEXIT_CRITICAL_IDF, portEXIT_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#ಅನ್ಯಥಾ
#define portENTER_CRITICAL(...)                     CHOOSE_MACRO_VA_ARG(portENTER_CRITICAL_IDF, portENTER_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#define portEXIT_CRITICAL(...)                      CHOOSE_MACRO_VA_ARG(portEXIT_CRITICAL_IDF, portEXIT_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#endif

ಬಾಹ್ಯ BaseType_t xPortSetInterruptMask( ಶೂನ್ಯ );
ಬಾಹ್ಯ ಶೂನ್ಯ vPortClearInterruptMask( BaseType_t xMask );

#define portSET_INTERRUPT_MASK_FROM_ISR() ({ \
    BaseType_t cur_level; \
    cur_level = xPortSetInterruptMask(); \
    vTaskEnterCritical(); \
    cur_level; \
})
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x) ({ \
    vTaskExitCritical(); \
    vPortClearInterruptMask(x); \
})

// ---------------------- Yielding -------------------------

#define portYIELD()                                 vPortYield()
#ಯದಿ defined(__cplusplus) && (__cplusplus >  201703L)
#define portYIELD_FROM_ISR(...)                     CHOOSE_MACRO_VA_ARG(portYIELD_FROM_ISR_CHECK, portYIELD_FROM_ISR_NO_CHECK __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#ಅನ್ಯಥಾ
#define portYIELD_FROM_ISR(...)                     CHOOSE_MACRO_VA_ARG(portYIELD_FROM_ISR_CHECK, portYIELD_FROM_ISR_NO_CHECK, ##__VA_ARGS__)(__VA_ARGS__)
#endif
// Note that the Linux port does not provide portYIELD_CORE(x) because purely single core for now.

// ----------------------- System --------------------------

#define portGET_CORE_ID()                           xPortGetCoreID()
#define portCHECK_IF_IN_ISR()                       xPortCheckIfInISR()

// ------------------- Run Time Stats ----------------------

ಬಾಹ್ಯ unsigned long ulPortGetRunTime( ಶೂನ್ಯ );
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() /* no-op */
#define port_GET_RUN_TIME_COUNTER_ಮೌಲ್ಯ()         ulPortGetRunTime()
// TODO IDF-5723: no esp_timer implementation

// ------------------- TCB Cleanup ----------------------

#define portCLEAN_UP_TCB( pxTCB )                   vPortCleanUpTCB( pxTCB )

/* --------------------------------------------- Inline Implementations ------------------------------------------------
 * - Implementation of inline functions of the forward declares
 * - Should come after forward declare and FreeRTOS Porting interface, as implementation may use both.
 * - For implementation of non-inlined functions, see port.c
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

// ------------------ Critical Sections --------------------

// ---------------------- Yielding -------------------------

// NOTE: vPortYieldCore( BaseType_t xCoreID ) is currently not implemented as the simulator is single core

// ----------------------- System --------------------------
/**
 * @brief Get the current core's ID
 *
 * @note dummy function for freertos simulator, always returns 0.
 @ return BaseType_t 0
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ BaseType_t xPortGetCoreID(ಶೂನ್ಯ)
{
    ಪ್ರತ್ಯಾಗಮ (BaseType_t) 0; // POSIX simulator is only single-core at the moment
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ BaseType_t xPortInIsrContext(ಶೂನ್ಯ)
{
    //Just call the FreeRTOS port interface version
    ಪ್ರತ್ಯಾಗಮ xPortCheckIfInISR();
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ vPortAssertIfInISR(ಶೂನ್ಯ)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(xPortInIsrContext() == 0);
}

// xPortInterruptedFromISRContext() is only used in panic handler and core dump,
// both probably not relevant on POSIX sim.
//BaseType_t xPortInterruptedFromISRContext(void);

// Following functions seem to not be used in SMP:
//static inline UBaseType_t xPortSetInterruptMaskFromISR(void)
//static inline void vPortClearInterruptMaskFromISR(UBaseType_t prev_level)

// Following functions should have been removed according to
// docs/en/migration-guides/release-5.x/5.0/system.rst
//static inline bool __attribute__((always_inline)) vPortCPUAcquireMutexTimeout(portMUX_TYPE *mux, int timeout)
//static inline void __attribute__((always_inline)) vPortCPUReleaseMutex(portMUX_TYPE *mux)

// ------------------ Critical Sections --------------------

ಶೂನ್ಯ vPortEnterCriticalIDF(ಶೂನ್ಯ);
ಶೂನ್ಯ vPortExitCriticalIDF(ಶೂನ್ಯ);

// We ignore the lock and timeout argument on POSIX simulator since it is single core

//IDF task critical sections
#define portTRY_ENTER_CRITICAL(lock, timeout)       ({(ಶೂನ್ಯ) lock; (ಶೂನ್ಯ) timeout; vPortEnterCriticalIDF(); pdPASS;})
#define portENTER_CRITICAL_IDF(lock)                ({(ಶೂನ್ಯ) lock; vPortEnterCriticalIDF();})
#define portEXIT_CRITICAL_IDF(lock)                 ({(ಶೂನ್ಯ) lock; vPortExitCriticalIDF();})
//IDF ISR critical sections
#define portTRY_ENTER_CRITICAL_ISR(lock, timeout)   ({(ಶೂನ್ಯ) lock; (ಶೂನ್ಯ) timeout; vPortEnterCriticalIDF(); pdPASS;})
#define portENTER_CRITICAL_ISR(lock)                ({(ಶೂನ್ಯ) lock; vPortEnterCriticalIDF();})
#define portEXIT_CRITICAL_ISR(lock)                 ({(ಶೂನ್ಯ) lock; vPortExitCriticalIDF();})
//IDF safe critical sections (they're the same)
#define portENTER_CRITICAL_SAFE(lock)               ({(ಶೂನ್ಯ) lock; vPortEnterCriticalIDF();})
#define portEXIT_CRITICAL_SAFE(lock)                ({(ಶೂನ್ಯ) lock; vPortExitCriticalIDF();})

// ---------------------- Yielding -------------------------

ಬಾಹ್ಯ ಶೂನ್ಯ vPortYield( ಶೂನ್ಯ );
#define portYIELD() vPortYield()

// ----------------------- System --------------------------

bool portVALID_LIST_MEM(ಧ್ರುವ ಶೂನ್ಯ *ptr);
bool portVALID_TCB_MEM(ಧ್ರುವ ಶೂನ್ಯ *ptr);
bool port_VALID_ರಾಶಿ_MEM(ಧ್ರುವ ಶೂನ್ಯ *ptr);

/*-----------------------------------------------------------*/

/* POSIX-specific port functions */

ಬಾಹ್ಯ ಶೂನ್ಯ vPortThreadDying( ಶೂನ್ಯ *pxTaskToDelete, ಚಞ್ಚಲ BaseType_t *pxPendYield );
ಬಾಹ್ಯ ಶೂನ್ಯ vPortCancelThread( ಶೂನ್ಯ *pxTaskToDelete );
#define portPRE_TASK_DELETE_HOOK( pvTaskToDelete, pxPendYield ) vPortThreadDying( ( pvTaskToDelete ), ( pxPendYield ) )
/*-----------------------------------------------------------*/

/*
 * Tasks run in their own pthreads and context switches between them
 * are always a full memory barrier. ISRs are emulated as signals
 * which also imply a full memory barrier.
 *
 * Thus, only a compiler barrier is needed to prevent the compiler
 * reordering.
 */
#define portMEMORY_BARRIER() __asm ಚಞ್ಚಲ( "" ::: "memory" )

#ifdef __cplusplus
}
#endif

#endif /* PORTMACRO_H */
