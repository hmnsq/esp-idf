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


#यदि_अपरिभाषितम् PORTMACRO_H
#परिभाषय PORTMACRO_H

#अन्तर्भाव <limits.h>
#अन्तर्भाव <stdint.h>
#अन्तर्भाव "esp_macros.h"
#अन्तर्भाव "spinlock.h"

#यदि_परिभाषितम् __cplusplus
बाह्य "C" {
#यदि_अन्तः

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come before first as they are used further down the file
 * ------------------------------------------------------------------------------------------------------------------ */

/* Type definitions. */
#परिभाषय पत्तनम्_CHAR        वर्ण
#परिभाषय पत्तनम्_FLOAT       भिन्न
#परिभाषय पत्तनम्_DOUBLE      द्विगुण
#परिभाषय पत्तनम्_LONG        दीर्घ
#परिभाषय पत्तनम्_SHORT       ह्रस्व
#परिभाषय पत्तनम्_राशिः_TYPE  अचिह्नित दीर्घ
#परिभाषय पत्तनम्_आधारः_TYPE   दीर्घ
#परिभाषय पत्तनम्_POINTER_आकार_TYPE intptr_t

प्रकारनिर्देश पत्तनम्_राशिः_TYPE राशिः_Type_t;
प्रकारनिर्देश पत्तनम्_आधारः_TYPE आधारः_Type_t;
प्रकारनिर्देश अचिह्नित पत्तनम्_आधारः_TYPE UBaseType_t;

प्रकारनिर्देश अचिह्नित दीर्घ TickType_t;
#परिभाषय पत्तनम्_MAX_DELAY ( TickType_t ) ULONG_MAX

#परिभाषय पत्तनम्_TICK_TYPE_IS_ATOMIC 1

/* Task function macros as described on the FreeRTOS.org WEB site. */
#परिभाषय पत्तनम्_TASK_FUNCTION_PROTO( vFunction, pvParameters ) शून्य vFunction( शून्य *pvParameters )
#परिभाषय पत्तनम्_TASK_FUNCTION( vFunction, pvParameters ) शून्य vFunction( शून्य *pvParameters )
/*-----------------------------------------------------------*/

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#परिभाषय पत्तनम्_CRITICAL_NESTING_अन्तर्_TCB     1
#परिभाषय पत्तनम्_राशिः_GROWTH                ( -1 )
#परिभाषय पत्तनम्_HAS_राशिः_अतिप्रवाहः_CHECKING ( 1 )   // Set in non-SMP POSIX port
#परिभाषय पत्तनम्_TICK_PERIOD_MS              ( ( TickType_t ) 1000 / config_TICK_दरः_HZ )
#परिभाषय पत्तनम्_TICK_दरः_MICROSECONDS      ( ( TickType_t ) 1000000 / config_TICK_दरः_HZ )
#परिभाषय पत्तनम्_BYTE_ALIGNMENT              8
#परिभाषय पत्तनम्_NOP() __asm चञ्चल        (" nop ")
/*-----------------------------------------------------------*/

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros need to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Spinlocks ------------------------

प्रकारनिर्देश spinlock_t                          पत्तनम्_MUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */
/**< Spinlock initializer */
#परिभाषय पत्तनम्_MUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#परिभाषय पत्तनम्_MUX_FREE_VAL                    SPINLOCK_FREE           /**< Spinlock is free. [refactor-todo] check if this is still required */
#परिभाषय पत्तनम्_MUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#परिभाषय पत्तनम्_MUX_TRY_तालकम्                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#परिभाषय पत्तनम्_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

आधारः_Type_t x_पत्तनम्_Check_If_अन्तर्_ISR(शून्य);

// ------------------ Critical Sections --------------------

#यदि ( config_सङ्ख्या_सम्बन्धि_CORES > 1 )
/*
These are always called with interrupts already disabled. We simply need to get/release the spinlocks
*/
बाह्य पत्तनम्_MUX_TYPE पत्तनम्_x_Task_तालकम्;
बाह्य पत्तनम्_MUX_TYPE पत्तनम्_x_ISRLock;

शून्य v_पत्तनम्_Take_तालकम्( पत्तनम्_MUX_TYPE *तालकम् );
शून्य v_पत्तनम्_Release_तालकम्( पत्तनम्_MUX_TYPE *तालकम् );
#यदि_अन्तः /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

शून्य v_पत्तनम्_Yield( शून्य );
बाह्य शून्य v_पत्तनम्_Yield_From_ISR( शून्य );

#परिभाषय पत्तनम्_YIELD_FROM_ISR_CHECK(x)     ({ \
    यदि ( (x) == pdTRUE ) { \
        v_पत्तनम्_Yield_From_ISR(); \
    } \
})
#परिभाषय पत्तनम्_YIELD_FROM_ISR_NO_CHECK()               v_पत्तनम्_Yield_From_ISR()

// ----------------------- System --------------------------

स्थिर अन्तःस्थ आधारः_Type_t __attribute__((always_inline)) x_पत्तनम्_Get_Core_ID( शून्य );

// ----------------------- TCB Cleanup --------------------------

शून्य v_पत्तनम्_Clean_ऊर्ध्वम्_TCB ( शून्य *pxTCB );

/* ----------------------------------------- FreeRTOS SMP Porting Interface --------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS SMP
 * - Must come after forward declare as some porting macros map to declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

#परिभाषय पत्तनम्_DISABLE_INTERRUPTS()                    x_पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्()
#परिभाषय पत्तनम्_ENABLE_INTERRUPTS()                     v_पत्तनम्_Clear_व्यवधानम्_आवरणम्(0)

// ------------------ Critical Sections --------------------

#परिभाषय पत्तनम्_ENTER_CRITICAL_SMP()                    vTaskEnterCritical();
#परिभाषय पत्तनम्_EXIT_CRITICAL_SMP()                     vTaskExitCritical();

#यदि defined(__cplusplus) && (__cplusplus >  201703L)
#परिभाषय पत्तनम्_ENTER_CRITICAL(...)                     वृणु_MACRO_VA_ARG(पत्तनम्_ENTER_CRITICAL_IDF, पत्तनम्_ENTER_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#परिभाषय पत्तनम्_EXIT_CRITICAL(...)                      वृणु_MACRO_VA_ARG(पत्तनम्_EXIT_CRITICAL_IDF, पत्तनम्_EXIT_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#अन्यथा
#परिभाषय पत्तनम्_ENTER_CRITICAL(...)                     वृणु_MACRO_VA_ARG(पत्तनम्_ENTER_CRITICAL_IDF, पत्तनम्_ENTER_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#परिभाषय पत्तनम्_EXIT_CRITICAL(...)                      वृणु_MACRO_VA_ARG(पत्तनम्_EXIT_CRITICAL_IDF, पत्तनम्_EXIT_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#यदि_अन्तः

बाह्य आधारः_Type_t x_पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्( शून्य );
बाह्य शून्य v_पत्तनम्_Clear_व्यवधानम्_आवरणम्( आधारः_Type_t x_आवरणम् );

#परिभाषय पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्_FROM_ISR() ({ \
    आधारः_Type_t cur_level; \
    cur_level = x_पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्(); \
    vTaskEnterCritical(); \
    cur_level; \
})
#परिभाषय पत्तनम्_CLEAR_व्यवधानम्_आवरणम्_FROM_ISR(x) ({ \
    vTaskExitCritical(); \
    v_पत्तनम्_Clear_व्यवधानम्_आवरणम्(x); \
})

// ---------------------- Yielding -------------------------

#परिभाषय पत्तनम्_YIELD()                                 v_पत्तनम्_Yield()
#यदि defined(__cplusplus) && (__cplusplus >  201703L)
#परिभाषय पत्तनम्_YIELD_FROM_ISR(...)                     वृणु_MACRO_VA_ARG(पत्तनम्_YIELD_FROM_ISR_CHECK, पत्तनम्_YIELD_FROM_ISR_NO_CHECK __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#अन्यथा
#परिभाषय पत्तनम्_YIELD_FROM_ISR(...)                     वृणु_MACRO_VA_ARG(पत्तनम्_YIELD_FROM_ISR_CHECK, पत्तनम्_YIELD_FROM_ISR_NO_CHECK, ##__VA_ARGS__)(__VA_ARGS__)
#यदि_अन्तः
// Note that the Linux port does not provide portYIELD_CORE(x) because purely single core for now.

// ----------------------- System --------------------------

#परिभाषय पत्तनम्_GET_CORE_ID()                           x_पत्तनम्_Get_Core_ID()
#परिभाषय पत्तनम्_CHECK_IF_अन्तर्_ISR()                       x_पत्तनम्_Check_If_अन्तर्_ISR()

// ------------------- Run Time Stats ----------------------

बाह्य अचिह्नित दीर्घ ul_पत्तनम्_Get_सततम्_Time( शून्य );
#परिभाषय पत्तनम्_CONFIGURE_TIMER_FOR_सततम्_TIME_STATS() /* no-op */
#परिभाषय पत्तनम्_GET_सततम्_TIME_गणक_मानम्()         ul_पत्तनम्_Get_सततम्_Time()
// TODO IDF-5723: no esp_timer implementation

// ------------------- TCB Cleanup ----------------------

#परिभाषय पत्तनम्_CLEAN_ऊर्ध्वम्_TCB( pxTCB )                   v_पत्तनम्_Clean_ऊर्ध्वम्_TCB( pxTCB )

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
स्थिर अन्तःस्थ आधारः_Type_t x_पत्तनम्_Get_Core_ID(शून्य)
{
    प्रत्यागम (आधारः_Type_t) 0; // POSIX simulator is only single-core at the moment
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

स्थिर अन्तःस्थ आधारः_Type_t x_पत्तनम्_अन्तर्_Isr_Context(शून्य)
{
    //Just call the FreeRTOS port interface version
    प्रत्यागम x_पत्तनम्_Check_If_अन्तर्_ISR();
}

स्थिर अन्तःस्थ शून्य v_पत्तनम्_Assert_If_अन्तर्_ISR(शून्य)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(x_पत्तनम्_अन्तर्_Isr_Context() == 0);
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

शून्य v_पत्तनम्_Enter_Critical_IDF(शून्य);
शून्य v_पत्तनम्_Exit_Critical_IDF(शून्य);

// We ignore the lock and timeout argument on POSIX simulator since it is single core

//IDF task critical sections
#परिभाषय पत्तनम्_TRY_ENTER_CRITICAL(तालकम्, timeout)       ({(शून्य) तालकम्; (शून्य) timeout; v_पत्तनम्_Enter_Critical_IDF(); pdPASS;})
#परिभाषय पत्तनम्_ENTER_CRITICAL_IDF(तालकम्)                ({(शून्य) तालकम्; v_पत्तनम्_Enter_Critical_IDF();})
#परिभाषय पत्तनम्_EXIT_CRITICAL_IDF(तालकम्)                 ({(शून्य) तालकम्; v_पत्तनम्_Exit_Critical_IDF();})
//IDF ISR critical sections
#परिभाषय पत्तनम्_TRY_ENTER_CRITICAL_ISR(तालकम्, timeout)   ({(शून्य) तालकम्; (शून्य) timeout; v_पत्तनम्_Enter_Critical_IDF(); pdPASS;})
#परिभाषय पत्तनम्_ENTER_CRITICAL_ISR(तालकम्)                ({(शून्य) तालकम्; v_पत्तनम्_Enter_Critical_IDF();})
#परिभाषय पत्तनम्_EXIT_CRITICAL_ISR(तालकम्)                 ({(शून्य) तालकम्; v_पत्तनम्_Exit_Critical_IDF();})
//IDF safe critical sections (they're the same)
#परिभाषय पत्तनम्_ENTER_CRITICAL_SAFE(तालकम्)               ({(शून्य) तालकम्; v_पत्तनम्_Enter_Critical_IDF();})
#परिभाषय पत्तनम्_EXIT_CRITICAL_SAFE(तालकम्)                ({(शून्य) तालकम्; v_पत्तनम्_Exit_Critical_IDF();})

// ---------------------- Yielding -------------------------

बाह्य शून्य v_पत्तनम्_Yield( शून्य );
#परिभाषय पत्तनम्_YIELD() v_पत्तनम्_Yield()

// ----------------------- System --------------------------

bool पत्तनम्_VALID_LIST_MEM(ध्रुव शून्य *ptr);
bool पत्तनम्_VALID_TCB_MEM(ध्रुव शून्य *ptr);
bool पत्तनम्_VALID_राशिः_MEM(ध्रुव शून्य *ptr);

/*-----------------------------------------------------------*/

/* POSIX-specific port functions */

बाह्य शून्य v_पत्तनम्_Thread_Dying( शून्य *px_Task_गन्तव्यम्_Delete, चञ्चल आधारः_Type_t *pxPendYield );
बाह्य शून्य v_पत्तनम्_Cancel_Thread( शून्य *px_Task_गन्तव्यम्_Delete );
#परिभाषय पत्तनम्_PRE_TASK_DELETE_HOOK( pv_Task_गन्तव्यम्_Delete, pxPendYield ) v_पत्तनम्_Thread_Dying( ( pv_Task_गन्तव्यम्_Delete ), ( pxPendYield ) )
/*-----------------------------------------------------------*/

/*
 * Tasks run in their own pthreads and context switches between them
 * are always a full memory barrier. ISRs are emulated as signals
 * which also imply a full memory barrier.
 *
 * Thus, only a compiler barrier is needed to prevent the compiler
 * reordering.
 */
#परिभाषय पत्तनम्_स्मृतिः_BARRIER() __asm चञ्चल( "" ::: "memory" )

#यदि_परिभाषितम् __cplusplus
}
#यदि_अन्तः

#यदि_अन्तः /* PORTMACRO_H */
