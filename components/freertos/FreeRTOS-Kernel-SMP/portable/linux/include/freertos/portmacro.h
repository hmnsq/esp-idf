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


#నిర్వచితం_కాకపోతే PORTMACRO_H
#నిర్వచించు PORTMACRO_H

#అన్తర్భావ <limits.h>
#అన్తర్భావ <stdint.h>
#అన్తర్భావ "esp_macros.h"
#అన్తర్భావ "spinlock.h"

#నిర్వచితమైతే __cplusplus
బాహ్య "C" {
#అయితే_ముగింపు

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come before first as they are used further down the file
 * ------------------------------------------------------------------------------------------------------------------ */

/* Type definitions. */
#నిర్వచించు పోర్టు_CHAR        అక్షరం
#నిర్వచించు పోర్టు_FLOAT       దశాంశం
#నిర్వచించు పోర్టు_DOUBLE      ద్విగుణ
#నిర్వచించు పోర్టు_LONG        దీర్ఘ
#నిర్వచించు పోర్టు_SHORT       పొట్టి
#నిర్వచించు పోర్టు_దొంతర_TYPE  గుర్తులేని దీర్ఘ
#నిర్వచించు పోర్టు_పునాది_TYPE   దీర్ఘ
#నిర్వచించు పోర్టు_సూచిక_కొలత_TYPE intptr_t

రకనిర్వచనం పోర్టు_దొంతర_TYPE దొంతర_Type_t;
రకనిర్వచనం పోర్టు_పునాది_TYPE పునాది_Type_t;
రకనిర్వచనం గుర్తులేని పోర్టు_పునాది_TYPE UBaseType_t;

రకనిర్వచనం గుర్తులేని దీర్ఘ TickType_t;
#నిర్వచించు పోర్టు_MAX_DELAY ( TickType_t ) ULONG_MAX

#నిర్వచించు పోర్టు_TICK_TYPE_IS_ATOMIC 1

/* Task function macros as described on the FreeRTOS.org WEB site. */
#నిర్వచించు పోర్టు_TASK_FUNCTION_PROTO( vFunction, pvParameters ) శూన్య vFunction( శూన్య *pvParameters )
#నిర్వచించు పోర్టు_TASK_FUNCTION( vFunction, pvParameters ) శూన్య vFunction( శూన్య *pvParameters )
/*-----------------------------------------------------------*/

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#నిర్వచించు పోర్టు_CRITICAL_NESTING_లోపల_TCB     1
#నిర్వచించు పోర్టు_దొంతర_GROWTH                ( -1 )
#నిర్వచించు పోర్టు_HAS_దొంతర_అధికప్రవాహం_CHECKING ( 1 )   // Set in non-SMP POSIX port
#నిర్వచించు పోర్టు_TICK_PERIOD_MS              ( ( TickType_t ) 1000 / config_TICK_రేటు_HZ )
#నిర్వచించు పోర్టు_TICK_రేటు_MICROSECONDS      ( ( TickType_t ) 1000000 / config_TICK_రేటు_HZ )
#నిర్వచించు పోర్టు_BYTE_ALIGNMENT              8
#నిర్వచించు పోర్టు_NOP() __asm అస్థిర        (" nop ")
/*-----------------------------------------------------------*/

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros need to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Spinlocks ------------------------

రకనిర్వచనం spinlock_t                          పోర్టు_MUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */
/**< Spinlock initializer */
#నిర్వచించు పోర్టు_MUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#నిర్వచించు పోర్టు_MUX_FREE_VAL                    SPINLOCK_FREE           /**< Spinlock is free. [refactor-todo] check if this is still required */
#నిర్వచించు పోర్టు_MUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#నిర్వచించు పోర్టు_MUX_TRY_తాళం                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#నిర్వచించు పోర్టు_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

పునాది_Type_t x_పోర్టు_Check_If_లోపల_ISR(శూన్య);

// ------------------ Critical Sections --------------------

#ఒకవేళ ( config_సంఖ్య_యొక్క_CORES > 1 )
/*
These are always called with interrupts already disabled. We simply need to get/release the spinlocks
*/
బాహ్య పోర్టు_MUX_TYPE పోర్టు_x_Task_తాళం;
బాహ్య పోర్టు_MUX_TYPE పోర్టు_x_ISRLock;

శూన్య v_పోర్టు_Take_తాళం( పోర్టు_MUX_TYPE *తాళం_2 );
శూన్య v_పోర్టు_Release_తాళం( పోర్టు_MUX_TYPE *తాళం_2 );
#అయితే_ముగింపు /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

శూన్య v_పోర్టు_Yield( శూన్య );
బాహ్య శూన్య v_పోర్టు_Yield_From_ISR( శూన్య );

#నిర్వచించు పోర్టు_YIELD_FROM_ISR_CHECK(x)     ({ \
    ఒకవేళ ( (x) == pdTRUE ) { \
        v_పోర్టు_Yield_From_ISR(); \
    } \
})
#నిర్వచించు పోర్టు_YIELD_FROM_ISR_NO_CHECK()               v_పోర్టు_Yield_From_ISR()

// ----------------------- System --------------------------

స్థిర అంతఃవరుస పునాది_Type_t __attribute__((always_inline)) x_పోర్టు_Get_Core_ID( శూన్య );

// ----------------------- TCB Cleanup --------------------------

శూన్య v_పోర్టు_Clean_పైకి_TCB ( శూన్య *pxTCB );

/* ----------------------------------------- FreeRTOS SMP Porting Interface --------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS SMP
 * - Must come after forward declare as some porting macros map to declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

#నిర్వచించు పోర్టు_DISABLE_INTERRUPTS()                    x_పోర్టు_సమితి_అంతరాయం_ముసుగు()
#నిర్వచించు పోర్టు_ENABLE_INTERRUPTS()                     v_పోర్టు_Clear_అంతరాయం_ముసుగు(0)

// ------------------ Critical Sections --------------------

#నిర్వచించు పోర్టు_ENTER_CRITICAL_SMP()                    vTaskEnterCritical();
#నిర్వచించు పోర్టు_EXIT_CRITICAL_SMP()                     vTaskExitCritical();

#ఒకవేళ defined(__cplusplus) && (__cplusplus >  201703L)
#నిర్వచించు పోర్టు_ENTER_CRITICAL(...)                     ఎంచుకో_MACRO_VA_ARG(పోర్టు_ENTER_CRITICAL_IDF, పోర్టు_ENTER_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#నిర్వచించు పోర్టు_EXIT_CRITICAL(...)                      ఎంచుకో_MACRO_VA_ARG(పోర్టు_EXIT_CRITICAL_IDF, పోర్టు_EXIT_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#లేకపోతే
#నిర్వచించు పోర్టు_ENTER_CRITICAL(...)                     ఎంచుకో_MACRO_VA_ARG(పోర్టు_ENTER_CRITICAL_IDF, పోర్టు_ENTER_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#నిర్వచించు పోర్టు_EXIT_CRITICAL(...)                      ఎంచుకో_MACRO_VA_ARG(పోర్టు_EXIT_CRITICAL_IDF, పోర్టు_EXIT_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#అయితే_ముగింపు

బాహ్య పునాది_Type_t x_పోర్టు_సమితి_అంతరాయం_ముసుగు( శూన్య );
బాహ్య శూన్య v_పోర్టు_Clear_అంతరాయం_ముసుగు( పునాది_Type_t x_ముసుగు );

#నిర్వచించు పోర్టు_సమితి_అంతరాయం_ముసుగు_FROM_ISR() ({ \
    పునాది_Type_t cur_level; \
    cur_level = x_పోర్టు_సమితి_అంతరాయం_ముసుగు(); \
    vTaskEnterCritical(); \
    cur_level; \
})
#నిర్వచించు పోర్టు_CLEAR_అంతరాయం_ముసుగు_FROM_ISR(x) ({ \
    vTaskExitCritical(); \
    v_పోర్టు_Clear_అంతరాయం_ముసుగు(x); \
})

// ---------------------- Yielding -------------------------

#నిర్వచించు పోర్టు_YIELD()                                 v_పోర్టు_Yield()
#ఒకవేళ defined(__cplusplus) && (__cplusplus >  201703L)
#నిర్వచించు పోర్టు_YIELD_FROM_ISR(...)                     ఎంచుకో_MACRO_VA_ARG(పోర్టు_YIELD_FROM_ISR_CHECK, పోర్టు_YIELD_FROM_ISR_NO_CHECK __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#లేకపోతే
#నిర్వచించు పోర్టు_YIELD_FROM_ISR(...)                     ఎంచుకో_MACRO_VA_ARG(పోర్టు_YIELD_FROM_ISR_CHECK, పోర్టు_YIELD_FROM_ISR_NO_CHECK, ##__VA_ARGS__)(__VA_ARGS__)
#అయితే_ముగింపు
// Note that the Linux port does not provide portYIELD_CORE(x) because purely single core for now.

// ----------------------- System --------------------------

#నిర్వచించు పోర్టు_GET_CORE_ID()                           x_పోర్టు_Get_Core_ID()
#నిర్వచించు పోర్టు_CHECK_IF_లోపల_ISR()                       x_పోర్టు_Check_If_లోపల_ISR()

// ------------------- Run Time Stats ----------------------

బాహ్య గుర్తులేని దీర్ఘ ul_పోర్టు_Get_నిరంతరం_సమయం( శూన్య );
#నిర్వచించు పోర్టు_CONFIGURE_TIMER_FOR_నిరంతరం_సమయం_STATS() /* no-op */
#నిర్వచించు పోర్టు_GET_నిరంతరం_సమయం_COUNTER_విలువ()         ul_పోర్టు_Get_నిరంతరం_సమయం()
// TODO IDF-5723: no esp_timer implementation

// ------------------- TCB Cleanup ----------------------

#నిర్వచించు పోర్టు_CLEAN_పైకి_TCB( pxTCB )                   v_పోర్టు_Clean_పైకి_TCB( pxTCB )

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
స్థిర అంతఃవరుస పునాది_Type_t x_పోర్టు_Get_Core_ID(శూన్య)
{
    తిరిగి (పునాది_Type_t) 0; // POSIX simulator is only single-core at the moment
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

స్థిర అంతఃవరుస పునాది_Type_t x_పోర్టు_లోపల_Isr_Context(శూన్య)
{
    //Just call the FreeRTOS port interface version
    తిరిగి x_పోర్టు_Check_If_లోపల_ISR();
}

స్థిర అంతఃవరుస శూన్య v_పోర్టు_Assert_If_లోపల_ISR(శూన్య)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(x_పోర్టు_లోపల_Isr_Context() == 0);
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

శూన్య v_పోర్టు_Enter_Critical_IDF(శూన్య);
శూన్య v_పోర్టు_Exit_Critical_IDF(శూన్య);

// We ignore the lock and timeout argument on POSIX simulator since it is single core

//IDF task critical sections
#నిర్వచించు పోర్టు_TRY_ENTER_CRITICAL(తాళం_2, timeout)       ({(శూన్య) తాళం_2; (శూన్య) timeout; v_పోర్టు_Enter_Critical_IDF(); pdPASS;})
#నిర్వచించు పోర్టు_ENTER_CRITICAL_IDF(తాళం_2)                ({(శూన్య) తాళం_2; v_పోర్టు_Enter_Critical_IDF();})
#నిర్వచించు పోర్టు_EXIT_CRITICAL_IDF(తాళం_2)                 ({(శూన్య) తాళం_2; v_పోర్టు_Exit_Critical_IDF();})
//IDF ISR critical sections
#నిర్వచించు పోర్టు_TRY_ENTER_CRITICAL_ISR(తాళం_2, timeout)   ({(శూన్య) తాళం_2; (శూన్య) timeout; v_పోర్టు_Enter_Critical_IDF(); pdPASS;})
#నిర్వచించు పోర్టు_ENTER_CRITICAL_ISR(తాళం_2)                ({(శూన్య) తాళం_2; v_పోర్టు_Enter_Critical_IDF();})
#నిర్వచించు పోర్టు_EXIT_CRITICAL_ISR(తాళం_2)                 ({(శూన్య) తాళం_2; v_పోర్టు_Exit_Critical_IDF();})
//IDF safe critical sections (they're the same)
#నిర్వచించు పోర్టు_ENTER_CRITICAL_సురక్షిత(తాళం_2)               ({(శూన్య) తాళం_2; v_పోర్టు_Enter_Critical_IDF();})
#నిర్వచించు పోర్టు_EXIT_CRITICAL_సురక్షిత(తాళం_2)                ({(శూన్య) తాళం_2; v_పోర్టు_Exit_Critical_IDF();})

// ---------------------- Yielding -------------------------

బాహ్య శూన్య v_పోర్టు_Yield( శూన్య );
#నిర్వచించు పోర్టు_YIELD() v_పోర్టు_Yield()

// ----------------------- System --------------------------

bool పోర్టు_VALID_LIST_MEM(స్థిరరాశి శూన్య *ptr);
bool పోర్టు_VALID_TCB_MEM(స్థిరరాశి శూన్య *ptr);
bool పోర్టు_VALID_దొంతర_MEM(స్థిరరాశి శూన్య *ptr);

/*-----------------------------------------------------------*/

/* POSIX-specific port functions */

బాహ్య శూన్య v_పోర్టు_Thread_Dying( శూన్య *px_Task_గమ్యం_Delete, అస్థిర పునాది_Type_t *pxPendYield );
బాహ్య శూన్య v_పోర్టు_Cancel_Thread( శూన్య *px_Task_గమ్యం_Delete );
#నిర్వచించు పోర్టు_PRE_TASK_DELETE_HOOK( pv_Task_గమ్యం_Delete, pxPendYield ) v_పోర్టు_Thread_Dying( ( pv_Task_గమ్యం_Delete ), ( pxPendYield ) )
/*-----------------------------------------------------------*/

/*
 * Tasks run in their own pthreads and context switches between them
 * are always a full memory barrier. ISRs are emulated as signals
 * which also imply a full memory barrier.
 *
 * Thus, only a compiler barrier is needed to prevent the compiler
 * reordering.
 */
#నిర్వచించు పోర్టు_జ్ఞాపకం_BARRIER() __asm అస్థిర( "" ::: "memory" )

#నిర్వచితమైతే __cplusplus
}
#అయితే_ముగింపు

#అయితే_ముగింపు /* PORTMACRO_H */
