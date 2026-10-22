/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#సూచన once

#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ <stdint.h>
#అన్తర్భావ "spinlock.h"
#అన్తర్భావ "xtensa/xtruntime.h"
#అన్తర్భావ "xtensa/config/core.h"
#అన్తర్భావ "xtensa/config/core-isa.h"
#అన్తర్భావ "xtensa/config/xt_specreg.h"
#అన్తర్భావ "xt_instr_macros.h"
#అన్తర్భావ "portbenchmark.h"
#అన్తర్భావ "esp_macros.h"
#అన్తర్భావ "esp_cpu.h"
#అన్తర్భావ "esp_private/crosscore_int.h"
#అన్తర్భావ "esp_memory_utils.h"

/*
Note: We should not include any FreeRTOS headers (directly or indirectly) here as this will create a reverse dependency
- FreeRTOS pulls in "portmacro.h" via
    - source.c <- FreeRTOS.h <- portable.h <- portmacro.h

- If we need types of task.h (such as TaskHandle_t), best put the feature somewhere else
- Be careful as some parts of C Standard Library Headers will pull in FreeRTOS headers
*/

#నిర్వచితమైతే __cplusplus
బాహ్య "C" {
#అయితే_ముగింపు

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come before first as they are used further down the file
 * ------------------------------------------------------------------------------------------------------------------ */

#నిర్వచించు పోర్టు_CHAR                    int8_t
#నిర్వచించు పోర్టు_FLOAT                   దశాంశం
#నిర్వచించు పోర్టు_DOUBLE                  ద్విగుణ
#నిర్వచించు పోర్టు_LONG                    int32_t
#నిర్వచించు పోర్టు_SHORT                   int16_t
#నిర్వచించు పోర్టు_దొంతర_TYPE              uint8_t
#నిర్వచించు పోర్టు_పునాది_TYPE               పూర్ణాంకం

రకనిర్వచనం పోర్టు_దొంతర_TYPE              దొంతర_Type_t;
రకనిర్వచనం పోర్టు_పునాది_TYPE               పునాది_Type_t;
రకనిర్వచనం గుర్తులేని పోర్టు_పునాది_TYPE      UBaseType_t;

#ఒకవేళ( config_USE_16_బిట్టు_TICKS == 1 )
రకనిర్వచనం uint16_t TickType_t;
#నిర్వచించు పోర్టు_MAX_DELAY ( TickType_t ) 0xffff
#లేకపోతే
రకనిర్వచనం uint32_t TickType_t;
#నిర్వచించు పోర్టు_MAX_DELAY ( TickType_t ) 0xffffffffUL
#అయితే_ముగింపు

/* Task function macros as described on the FreeRTOS.org WEB site. */
#నిర్వచించు పోర్టు_TASK_FUNCTION_PROTO( vFunction, pvParameters )  శూన్య vFunction( శూన్య *pvParameters )
#నిర్వచించు పోర్టు_TASK_FUNCTION( vFunction, pvParameters )        శూన్య vFunction( శూన్య *pvParameters )

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#నిర్వచించు పోర్టు_CRITICAL_NESTING_లోపల_TCB     1
#నిర్వచించు పోర్టు_దొంతర_GROWTH                ( -1 )
#నిర్వచించు పోర్టు_TICK_PERIOD_MS              ( ( TickType_t ) 1000 / config_TICK_రేటు_HZ )
#నిర్వచించు పోర్టు_BYTE_ALIGNMENT              16    // Xtensa Windowed ABI requires the stack pointer to always be 16-byte aligned. See "isa_rm.pdf 8.1.1 Windowed Register Usage and Stack Layout"
#నిర్వచించు పోర్టు_NOP()                       XT_NOP()    //Todo: Check if XT_NOP exists

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros need to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Spinlocks ------------------------

రకనిర్వచనం spinlock_t                          పోర్టు_MUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */
#నిర్వచించు పోర్టు_MUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#నిర్వచించు పోర్టు_MUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#నిర్వచించు పోర్టు_MUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#నిర్వచించు పోర్టు_MUX_TRY_తాళం                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#నిర్వచించు పోర్టు_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

పునాది_Type_t x_పోర్టు_Check_If_లోపల_ISR(శూన్య);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortCheckIfInISR() internally
 */
శూన్య v_పోర్టు_Assert_If_లోపల_ISR(శూన్య);

// ------------------ Critical Sections --------------------

UBaseType_t ux_పోర్టు_Enter_Critical_From_ISR( శూన్య );
శూన్య v_పోర్టు_Exit_Critical_From_ISR( UBaseType_t level );

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
స్థిర అంతఃవరుస శూన్య __attribute__((always_inline)) v_పోర్టు_Yield_Core( పునాది_Type_t xCoreID );
స్థిర అంతఃవరుస శూన్య __attribute__((always_inline)) v_పోర్టు_Yield_From_ISR( శూన్య );

#నిర్వచించు పోర్టు_YIELD_FROM_ISR_CHECK(x)     ({ \
    ఒకవేళ ( (x) == pdTRUE ) { \
        v_పోర్టు_Yield_From_ISR(); \
    } \
})
#నిర్వచించు పోర్టు_YIELD_FROM_ISR_NO_CHECK()               v_పోర్టు_Yield_From_ISR()

// ----------------------- System --------------------------

స్థిర అంతఃవరుస పునాది_Type_t __attribute__((always_inline)) x_పోర్టు_Get_Core_ID( శూన్య );

// --------------------- TCB Cleanup -----------------------

/**
 * @brief TCB cleanup hook
 *
 * The portCLEAN_UP_TCB() macro is called in prvDeleteTCB() right before a
 * deleted task's memory is freed. We map that macro to this internal function
 * so that IDF FreeRTOS ports can inject some task pre-deletion operations.
 */
శూన్య v_పోర్టు_TCBPre_Delete_Hook( శూన్య *pxTCB );


/* ----------------------------------------- FreeRTOS SMP Porting Interface --------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS SMP
 * - Must come after forward declare as some porting macros map to declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

#నిర్వచించు పోర్టు_సమితి_అంతరాయం_ముసుగు()            ({ \
    గుర్తులేని పూర్ణాంకం prev_level = XTOS_సమితి_INTLEVEL(XCHAL_EXCM_LEVEL); \
    portbenchmark_అంతరాయం_DISABLE(); \
    prev_level = ((prev_level >> XCHAL_PS_INTLEVEL_జరుపు) & XCHAL_PS_INTLEVEL_ముసుగు); \
    prev_level; \
})
#నిర్వచించు పోర్టు_సమితి_అంతరాయం_ముసుగు_FROM_ISR()   పోర్టు_సమితి_అంతరాయం_ముసుగు()
#నిర్వచించు పోర్టు_DISABLE_INTERRUPTS()            పోర్టు_సమితి_అంతరాయం_ముసుగు()

/**
 * @brief Assert if in ISR context
 *
 * TODO: Enable once ISR safe version of vTaskEnter/ExitCritical() is implemented
 * for single-core SMP FreeRTOS Kernel. (IDF-10540)
 */
// #define portASSERT_IF_IN_ISR() vPortAssertIfInISR()

/*
Note: XTOS_RESTORE_INTLEVEL() will overwrite entire PS register on XEA2. So we need to set the value of the INTLEVEL field ourselves
*/
#నిర్వచించు పోర్టు_CLEAR_అంతరాయం_ముసుగు(x)         ({ \
    గుర్తులేని పూర్ణాంకం ps_reg; \
    RSR(XT_REG_PS, ps_reg); \
    ps_reg = (ps_reg & ~XCHAL_PS_INTLEVEL_ముసుగు); \
    ps_reg |= ((x << XCHAL_PS_INTLEVEL_జరుపు) & XCHAL_PS_INTLEVEL_ముసుగు); \
    XTOS_RESTORE_INTLEVEL(ps_reg); \
})
#నిర్వచించు పోర్టు_CLEAR_అంతరాయం_ముసుగు_FROM_ISR(x)    పోర్టు_CLEAR_అంతరాయం_ముసుగు(x)
#నిర్వచించు పోర్టు_ENABLE_INTERRUPTS()     ({ \
    portbenchmark_అంతరాయం_RESTORE(0); \
    XTOS_సమితి_INTLEVEL(0); \
})

/* Compatibility */
#నిర్వచించు పోర్టు_RESTORE_INTERRUPTS(x)   పోర్టు_CLEAR_అంతరాయం_ముసుగు(x)

// ------------------ Critical Sections --------------------

#ఒకవేళ ( config_సంఖ్య_యొక్క_CORES > 1 )
#నిర్వచించు పోర్టు_GET_TASK_తాళం()                         v_పోర్టు_Take_తాళం(&పోర్టు_x_Task_తాళం)
#నిర్వచించు పోర్టు_RELEASE_TASK_తాళం()                     v_పోర్టు_Release_తాళం(&పోర్టు_x_Task_తాళం)
#నిర్వచించు పోర్టు_GET_ISR_తాళం()                          v_పోర్టు_Take_తాళం(&పోర్టు_x_ISRLock)
#నిర్వచించు పోర్టు_RELEASE_ISR_తాళం()                      v_పోర్టు_Release_తాళం(&పోర్టు_x_ISRLock)
#అయితే_ముగింపు /* configNUMBER_OF_CORES > 1 */

//Critical sections used by FreeRTOS SMP
బాహ్య శూన్య vTaskEnterCritical( శూన్య );
బాహ్య శూన్య vTaskExitCritical( శూన్య );
#నిర్వచించు పోర్టు_ENTER_CRITICAL_SMP()                    vTaskEnterCritical();
#నిర్వచించు పోర్టు_EXIT_CRITICAL_SMP()                     vTaskExitCritical();

#ఒకవేళ defined(__cplusplus) && (__cplusplus >  201703L)
#నిర్వచించు పోర్టు_ENTER_CRITICAL(...)                     ఎంచుకో_MACRO_VA_ARG(పోర్టు_ENTER_CRITICAL_IDF, పోర్టు_ENTER_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#నిర్వచించు పోర్టు_EXIT_CRITICAL(...)                      ఎంచుకో_MACRO_VA_ARG(పోర్టు_EXIT_CRITICAL_IDF, పోర్టు_EXIT_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#లేకపోతే
#నిర్వచించు పోర్టు_ENTER_CRITICAL(...)                     ఎంచుకో_MACRO_VA_ARG(పోర్టు_ENTER_CRITICAL_IDF, పోర్టు_ENTER_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#నిర్వచించు పోర్టు_EXIT_CRITICAL(...)                      ఎంచుకో_MACRO_VA_ARG(పోర్టు_EXIT_CRITICAL_IDF, పోర్టు_EXIT_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#అయితే_ముగింపు

బాహ్య UBaseType_t vTaskEnterCriticalFromISR( శూన్య );
బాహ్య శూన్య vTaskExitCriticalFromISR( UBaseType_t ux_Saved_అంతరాయం_స్థితి );
#నిర్వచించు పోర్టు_ENTER_CRITICAL_FROM_ISR() vTaskEnterCriticalFromISR()
#నిర్వచించు పోర్టు_EXIT_CRITICAL_FROM_ISR(x) vTaskExitCriticalFromISR(x)

// ---------------------- Yielding -------------------------

#నిర్వచించు పోర్టు_YIELD()                                 v_పోర్టు_Yield()
#ఒకవేళ defined(__cplusplus) && (__cplusplus >  201703L)
#నిర్వచించు పోర్టు_YIELD_FROM_ISR(...)                     ఎంచుకో_MACRO_VA_ARG(పోర్టు_YIELD_FROM_ISR_CHECK, పోర్టు_YIELD_FROM_ISR_NO_CHECK __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#లేకపోతే
#నిర్వచించు పోర్టు_YIELD_FROM_ISR(...)                     ఎంచుకో_MACRO_VA_ARG(పోర్టు_YIELD_FROM_ISR_CHECK, పోర్టు_YIELD_FROM_ISR_NO_CHECK, ##__VA_ARGS__)(__VA_ARGS__)
#అయితే_ముగింపు
#నిర్వచించు పోర్టు_YIELD_CORE(x)                           v_పోర్టు_Yield_Core(x)

// ----------------------- System --------------------------

#నిర్వచించు పోర్టు_GET_CORE_ID()                           x_పోర్టు_Get_Core_ID()
#నిర్వచించు పోర్టు_CHECK_IF_లోపల_ISR()                       x_పోర్టు_Check_If_లోపల_ISR()

// ------------------- Run Time Stats ----------------------

//Timers are already configured, so nothing to do for configuration of run time stats timer
#నిర్వచించు పోర్టు_CONFIGURE_TIMER_FOR_నిరంతరం_సమయం_STATS()
#ఒకవేళ ( CONFIG_FREERTOS_GENERATE_నిరంతరం_సమయం_STATS )
config_నిరంతరం_సమయం_COUNTER_TYPE x_పోర్టు_Get_నిరంతరం_సమయం_Counter_విలువ( శూన్య );
#నిర్వచించు పోర్టు_GET_నిరంతరం_సమయం_COUNTER_విలువ()        x_పోర్టు_Get_నిరంతరం_సమయం_Counter_విలువ()
#అయితే_ముగింపు

// --------------------- TCB Cleanup -----------------------

#నిర్వచించు పోర్టు_CLEAN_పైకి_TCB( pxTCB )                   v_పోర్టు_TCBPre_Delete_Hook( pxTCB )

/* --------------------------------------------- Inline Implementations ------------------------------------------------
 * - Implementation of inline functions of the forward declares
 * - Should come after forward declare and FreeRTOS Porting interface, as implementation may use both.
 * - For implementation of non-inlined functions, see port.c
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

// ------------------ Critical Sections --------------------

// ---------------------- Yielding -------------------------

బాహ్య శూన్య frxt_అమరిక_switch( శూన్య );     //Defined in portasm.S

స్థిర అంతఃవరుస శూన్య __attribute__((always_inline)) v_పోర్టు_Yield_Core( పునాది_Type_t xCoreID )
{
    esp_crosscore_int_send_yield( xCoreID );
}

స్థిర అంతఃవరుస శూన్య __attribute__((always_inline)) v_పోర్టు_Yield_From_ISR( శూన్య )
{
    //traceISR_EXIT_TO_SCHEDULER();
    frxt_అమరిక_switch();
}

// ----------------------- System --------------------------

స్థిర అంతఃవరుస పునాది_Type_t __attribute__((always_inline)) x_పోర్టు_Get_Core_ID( శూన్య )
{
    తిరిగి (పునాది_Type_t) esp_cpu_get_core_id();
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

పునాది_Type_t x_పోర్టు_Interrupted_From_ISRContext(శూన్య);

స్థిర అంతఃవరుస UBaseType_t x_పోర్టు_సమితి_అంతరాయం_ముసుగు_From_ISR(శూన్య)
{
    UBaseType_t prev_int_level = XTOS_సమితి_INTLEVEL(XCHAL_EXCM_LEVEL);
    portbenchmark_అంతరాయం_DISABLE();
    తిరిగి prev_int_level;
}

స్థిర అంతఃవరుస శూన్య v_పోర్టు_Clear_అంతరాయం_ముసుగు_From_ISR(UBaseType_t prev_level)
{
    portbenchmark_అంతరాయం_RESTORE(prev_level);
    XTOS_RESTORE_JUST_INTLEVEL((పూర్ణాంకం) prev_level);
}

// ---------------------- Spinlocks ------------------------

స్థిర అంతఃవరుస bool __attribute__((always_inline)) v_పోర్టు_CPUAcquire_Mutex_Timeout(పోర్టు_MUX_TYPE *mux, పూర్ణాంకం timeout)
{
    తిరిగి (spinlock_acquire(mux, timeout));
}

స్థిర అంతఃవరుస శూన్య __attribute__((always_inline)) v_పోర్టు_CPURelease_Mutex(పోర్టు_MUX_TYPE *mux)
{
    spinlock_release(mux);
}

// ------------------ Critical Sections --------------------

#ఒకవేళ CONFIG_FREERTOS_పోర్టు_THREAD_సురక్షిత_CLAIM
/**
 * @brief Claim thread-safe region start
 *        If claimed, vPortEnterCritical/vPortExitCritical on the current core are no-ops.
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
శూన్య x_పోర్టు_Thread_సురక్షిత_Claim(శూన్య);

/**
 * @brief Claim thread-safe region end
 *        Restores normal port critical behavior
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
శూన్య x_పోర్టు_Thread_సురక్షిత_Disclaim(శూన్య);

బాహ్య అస్థిర bool పోర్టు_x_Thread_సురక్షిత_Claimed;
#అయితే_ముగింపు /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

పునాది_Type_t x_పోర్టు_Enter_Critical_Timeout(పోర్టు_MUX_TYPE *తాళం_2, పునాది_Type_t timeout);

స్థిర అంతఃవరుస శూన్య __attribute__((always_inline)) v_పోర్టు_Enter_Critical_IDF(పోర్టు_MUX_TYPE *తాళం_2)
{
    x_పోర్టు_Enter_Critical_Timeout(తాళం_2, పోర్టు_MUX_NO_TIMEOUT);
}

శూన్య v_పోర్టు_Exit_Critical_IDF(పోర్టు_MUX_TYPE *తాళం_2);

/*
In case any IDF libs called the port critical functions directly instead of through the macros.
The will just call the vPort...CriticalIDF() functions
*/
శూన్య v_పోర్టు_Enter_Critical(పోర్టు_MUX_TYPE *తాళం_2);
శూన్య v_పోర్టు_Exit_Critical(పోర్టు_MUX_TYPE *తాళం_2);

//IDF task critical sections
#నిర్వచించు పోర్టు_TRY_ENTER_CRITICAL(తాళం_2, timeout)       x_పోర్టు_Enter_Critical_Timeout(తాళం_2, timeout)
#నిర్వచించు పోర్టు_ENTER_CRITICAL_IDF(తాళం_2)                v_పోర్టు_Enter_Critical_IDF(తాళం_2)
#నిర్వచించు పోర్టు_EXIT_CRITICAL_IDF(తాళం_2)                 v_పోర్టు_Exit_Critical_IDF(తాళం_2)
//IDF ISR critical sections
#నిర్వచించు పోర్టు_TRY_ENTER_CRITICAL_ISR(తాళం_2, timeout)   x_పోర్టు_Enter_Critical_Timeout(తాళం_2, timeout)
#నిర్వచించు పోర్టు_ENTER_CRITICAL_ISR(తాళం_2)                v_పోర్టు_Enter_Critical_IDF(తాళం_2)
#నిర్వచించు పోర్టు_EXIT_CRITICAL_ISR(తాళం_2)                 v_పోర్టు_Exit_Critical_IDF(తాళం_2)
//IDF safe critical sections (they're the same)
#నిర్వచించు పోర్టు_ENTER_CRITICAL_సురక్షిత(తాళం_2)               v_పోర్టు_Enter_Critical_IDF(తాళం_2)
#నిర్వచించు పోర్టు_EXIT_CRITICAL_సురక్షిత(తాళం_2)                v_పోర్టు_Exit_Critical_IDF(తాళం_2)

// ---------------------- Yielding -------------------------

స్థిర అంతఃవరుస bool x_పోర్టు_Can_Yield(శూన్య)
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

    తిరిగి ((ps_reg & PS_INTLEVEL_ముసుగు) == 0);
}

// Defined even for configNUMBER_OF_CORES > 1 for IDF compatibility
#నిర్వచించు పోర్టు_YIELD_WITHIN_API()                      esp_crosscore_int_send_yield(x_పోర్టు_Get_Core_ID())

// ----------------------- System --------------------------

శూన్య v_పోర్టు_సమితి_దొంతర_Watchpoint(శూన్య *px_దొంతర_ప్రారంభం);

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
bool x_పోర్టు_Check_Valid_List_Mem(స్థిరరాశి శూన్య *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool x_పోర్టు_Check_Valid_TCBMem(స్థిరరాశి శూన్య *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_దొంతర_Mem(స్థిరరాశి శూన్య *ptr);

#నిర్వచించు పోర్టు_VALID_LIST_MEM(ptr)     x_పోర్టు_Check_Valid_List_Mem(ptr)
#నిర్వచించు పోర్టు_VALID_TCB_MEM(ptr)      x_పోర్టు_Check_Valid_TCBMem(ptr)
#నిర్వచించు పోర్టు_VALID_దొంతర_MEM(ptr)    x_Portcheck_Valid_దొంతర_Mem(ptr)

/* ------------------------------------------------------ Misc ---------------------------------------------------------
 * - Miscellaneous porting macros
 * - These are not part of the FreeRTOS porting interface, but are used by other FreeRTOS dependent components
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- App-Trace -------------------------

#ఒకవేళ CONFIG_ESP_TRACE_ENABLE
బాహ్య అస్థిర పునాది_Type_t x_పోర్టు_Switch_Flag;
#నిర్వచించు os_task_switch_is_pended(_cpu_) (x_పోర్టు_Switch_Flag)
#లేకపోతే
#నిర్వచించు os_task_switch_is_pended(_cpu_) (false)
#అయితే_ముగింపు

// --------------- Compatibility Includes ------------------
/*
ESP-IDF currently does not have a "Include what you use" policy. A lot of files implicitly pull in API through
portmacro.h. Therefore, we need to keep these headers around for now to allow the rest of IDF to compile.

[refactor-todo] Clean up ESP-IDF inclusion dependencies and add a inclusion check.
*/

#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <stdbool.h>
#అన్తర్భావ <stdarg.h>
#అన్తర్భావ <xtensa/hal.h>
#అన్తర్భావ "esp_attr.h"
#అన్తర్భావ "esp_newlib.h"
#అన్తర్భావ "esp_heap_caps.h"
#అన్తర్భావ "esp_rom_sys.h"
#అన్తర్భావ "esp_system.h"
#అన్తర్భావ "portbenchmark.h"
#అన్తర్భావ <limits.h>
#అన్తర్భావ <xtensa/config/system.h>
#అన్తర్భావ <xtensa_api.h>

#నిర్వచితమైతే __cplusplus
}
#అయితే_ముగింపు
