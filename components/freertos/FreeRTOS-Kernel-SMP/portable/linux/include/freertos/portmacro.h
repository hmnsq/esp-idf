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


#अगर_अघोषित PORTMACRO_H
#घोषणा PORTMACRO_H

#समावेश <limits.h>
#समावेश <stdint.h>
#समावेश "esp_macros.h"
#समावेश "spinlock.h"

#अगर_घोषित __cplusplus
बाह्य "C" {
#पूर्ण_अगर

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come before first as they are used further down the file
 * ------------------------------------------------------------------------------------------------------------------ */

/* Type definitions. */
#घोषणा portCHAR        अक्षर
#घोषणा portFLOAT       भग्न
#घोषणा portDOUBLE      द्विगुन
#घोषणा portLONG        दीर्घ
#घोषणा portSHORT       लघु
#घोषणा port_ढेर_TYPE  अचिन्हित दीर्घ
#घोषणा portBASE_TYPE   दीर्घ
#घोषणा port_POINTER_आकार_TYPE intptr_t

प्रकार port_ढेर_TYPE ढेर_Type_t;
प्रकार portBASE_TYPE BaseType_t;
प्रकार अचिन्हित portBASE_TYPE UBaseType_t;

प्रकार अचिन्हित दीर्घ TickType_t;
#घोषणा portMAX_DELAY ( TickType_t ) अच_दीर्घ_उच्च

#घोषणा portTICK_TYPE_IS_ATOMIC 1

/* Task function macros as described on the FreeRTOS.org WEB site. */
#घोषणा portTASK_FUNCTION_PROTO( vFunction, pvParameters ) व्योम vFunction( व्योम *pvParameters )
#घोषणा portTASK_FUNCTION( vFunction, pvParameters ) व्योम vFunction( व्योम *pvParameters )
/*-----------------------------------------------------------*/

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#घोषणा portCRITICAL_NESTING_IN_TCB     1
#घोषणा port_ढेर_GROWTH                ( -1 )
#घोषणा port_HAS_ढेर_OVERFLOW_CHECKING ( 1 )   // Set in non-SMP POSIX port
#घोषणा portTICK_PERIOD_MS              ( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#घोषणा portTICK_RATE_MICROSECONDS      ( ( TickType_t ) 1000000 / configTICK_RATE_HZ )
#घोषणा portBYTE_ALIGNMENT              8
#घोषणा portNOP() __asm अस्थिर        (" nop ")
/*-----------------------------------------------------------*/

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros need to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Spinlocks ------------------------

प्रकार spinlock_t                          portMUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */
/**< Spinlock initializer */
#घोषणा portMUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#घोषणा portMUX_FREE_VAL                    SPINLOCK_FREE           /**< Spinlock is free. [refactor-todo] check if this is still required */
#घोषणा portMUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#घोषणा portMUX_TRY_LOCK                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#घोषणा portMUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

BaseType_t xPortCheckIfInISR(व्योम);

// ------------------ Critical Sections --------------------

#अगर ( config_संख्या_OF_CORES > 1 )
/*
These are always called with interrupts already disabled. We simply need to get/release the spinlocks
*/
बाह्य portMUX_TYPE port_xTaskLock;
बाह्य portMUX_TYPE port_xISRLock;

व्योम vPortTakeLock( portMUX_TYPE *lock );
व्योम vPortReleaseLock( portMUX_TYPE *lock );
#पूर्ण_अगर /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

व्योम vPortYield( व्योम );
बाह्य व्योम vPortYieldFromISR( व्योम );

#घोषणा portYIELD_FROM_ISR_CHECK(x)     ({ \
    अगर ( (x) == pdTRUE ) { \
        vPortYieldFromISR(); \
    } \
})
#घोषणा portYIELD_FROM_ISR_NO_CHECK()               vPortYieldFromISR()

// ----------------------- System --------------------------

अटल अंतरभूत BaseType_t __attribute__((always_inline)) xPortGetCoreID( व्योम );

// ----------------------- TCB Cleanup --------------------------

व्योम vPortCleanUpTCB ( व्योम *pxTCB );

/* ----------------------------------------- FreeRTOS SMP Porting Interface --------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS SMP
 * - Must come after forward declare as some porting macros map to declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

#घोषणा portDISABLE_INTERRUPTS()                    xPortSetInterruptMask()
#घोषणा portENABLE_INTERRUPTS()                     vPortClearInterruptMask(0)

// ------------------ Critical Sections --------------------

#घोषणा portENTER_CRITICAL_SMP()                    vTaskEnterCritical();
#घोषणा portEXIT_CRITICAL_SMP()                     vTaskExitCritical();

#अगर defined(__cplusplus) && (__cplusplus >  201703L)
#घोषणा portENTER_CRITICAL(...)                     CHOOSE_MACRO_VA_ARG(portENTER_CRITICAL_IDF, portENTER_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#घोषणा portEXIT_CRITICAL(...)                      CHOOSE_MACRO_VA_ARG(portEXIT_CRITICAL_IDF, portEXIT_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#अन्यथा
#घोषणा portENTER_CRITICAL(...)                     CHOOSE_MACRO_VA_ARG(portENTER_CRITICAL_IDF, portENTER_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#घोषणा portEXIT_CRITICAL(...)                      CHOOSE_MACRO_VA_ARG(portEXIT_CRITICAL_IDF, portEXIT_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#पूर्ण_अगर

बाह्य BaseType_t xPortSetInterruptMask( व्योम );
बाह्य व्योम vPortClearInterruptMask( BaseType_t xMask );

#घोषणा portSET_INTERRUPT_MASK_FROM_ISR() ({ \
    BaseType_t cur_level; \
    cur_level = xPortSetInterruptMask(); \
    vTaskEnterCritical(); \
    cur_level; \
})
#घोषणा portCLEAR_INTERRUPT_MASK_FROM_ISR(x) ({ \
    vTaskExitCritical(); \
    vPortClearInterruptMask(x); \
})

// ---------------------- Yielding -------------------------

#घोषणा portYIELD()                                 vPortYield()
#अगर defined(__cplusplus) && (__cplusplus >  201703L)
#घोषणा portYIELD_FROM_ISR(...)                     CHOOSE_MACRO_VA_ARG(portYIELD_FROM_ISR_CHECK, portYIELD_FROM_ISR_NO_CHECK __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#अन्यथा
#घोषणा portYIELD_FROM_ISR(...)                     CHOOSE_MACRO_VA_ARG(portYIELD_FROM_ISR_CHECK, portYIELD_FROM_ISR_NO_CHECK, ##__VA_ARGS__)(__VA_ARGS__)
#पूर्ण_अगर
// Note that the Linux port does not provide portYIELD_CORE(x) because purely single core for now.

// ----------------------- System --------------------------

#घोषणा portGET_CORE_ID()                           xPortGetCoreID()
#घोषणा portCHECK_IF_IN_ISR()                       xPortCheckIfInISR()

// ------------------- Run Time Stats ----------------------

बाह्य अचिन्हित दीर्घ ulPortGetRunTime( व्योम );
#घोषणा portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() /* no-op */
#घोषणा port_GET_RUN_TIME_गणक_मूल्य()         ulPortGetRunTime()
// TODO IDF-5723: no esp_timer implementation

// ------------------- TCB Cleanup ----------------------

#घोषणा portCLEAN_UP_TCB( pxTCB )                   vPortCleanUpTCB( pxTCB )

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
अटल अंतरभूत BaseType_t xPortGetCoreID(व्योम)
{
    वापस (BaseType_t) 0; // POSIX simulator is only single-core at the moment
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

अटल अंतरभूत BaseType_t xPortInIsrContext(व्योम)
{
    //Just call the FreeRTOS port interface version
    वापस xPortCheckIfInISR();
}

अटल अंतरभूत व्योम vPortAssertIfInISR(व्योम)
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

व्योम vPortEnterCriticalIDF(व्योम);
व्योम vPortExitCriticalIDF(व्योम);

// We ignore the lock and timeout argument on POSIX simulator since it is single core

//IDF task critical sections
#घोषणा portTRY_ENTER_CRITICAL(lock, timeout)       ({(व्योम) lock; (व्योम) timeout; vPortEnterCriticalIDF(); pdPASS;})
#घोषणा portENTER_CRITICAL_IDF(lock)                ({(व्योम) lock; vPortEnterCriticalIDF();})
#घोषणा portEXIT_CRITICAL_IDF(lock)                 ({(व्योम) lock; vPortExitCriticalIDF();})
//IDF ISR critical sections
#घोषणा portTRY_ENTER_CRITICAL_ISR(lock, timeout)   ({(व्योम) lock; (व्योम) timeout; vPortEnterCriticalIDF(); pdPASS;})
#घोषणा portENTER_CRITICAL_ISR(lock)                ({(व्योम) lock; vPortEnterCriticalIDF();})
#घोषणा portEXIT_CRITICAL_ISR(lock)                 ({(व्योम) lock; vPortExitCriticalIDF();})
//IDF safe critical sections (they're the same)
#घोषणा portENTER_CRITICAL_SAFE(lock)               ({(व्योम) lock; vPortEnterCriticalIDF();})
#घोषणा portEXIT_CRITICAL_SAFE(lock)                ({(व्योम) lock; vPortExitCriticalIDF();})

// ---------------------- Yielding -------------------------

बाह्य व्योम vPortYield( व्योम );
#घोषणा portYIELD() vPortYield()

// ----------------------- System --------------------------

bool portVALID_LIST_MEM(स्थिर व्योम *ptr);
bool portVALID_TCB_MEM(स्थिर व्योम *ptr);
bool port_VALID_ढेर_MEM(स्थिर व्योम *ptr);

/*-----------------------------------------------------------*/

/* POSIX-specific port functions */

बाह्य व्योम vPortThreadDying( व्योम *pxTaskToDelete, अस्थिर BaseType_t *pxPendYield );
बाह्य व्योम vPortCancelThread( व्योम *pxTaskToDelete );
#घोषणा portPRE_TASK_DELETE_HOOK( pvTaskToDelete, pxPendYield ) vPortThreadDying( ( pvTaskToDelete ), ( pxPendYield ) )
/*-----------------------------------------------------------*/

/*
 * Tasks run in their own pthreads and context switches between them
 * are always a full memory barrier. ISRs are emulated as signals
 * which also imply a full memory barrier.
 *
 * Thus, only a compiler barrier is needed to prevent the compiler
 * reordering.
 */
#घोषणा portMEMORY_BARRIER() __asm अस्थिर( "" ::: "memory" )

#अगर_घोषित __cplusplus
}
#पूर्ण_अगर

#पूर्ण_अगर /* PORTMACRO_H */
