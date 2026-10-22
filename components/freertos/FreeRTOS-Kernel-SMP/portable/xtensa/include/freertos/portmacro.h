/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "spinlock.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xtensa/xtruntime.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xtensa/config/core.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xtensa/config/core-isa.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xtensa/config/xt_specreg.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xt_instr_macros.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "portbenchmark.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_macros.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_cpu.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_private/crosscore_int.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_memory_utils.h"

/*
Note: We should not include any FreeRTOS headers (directly or indirectly) here as this will create a reverse dependency
- FreeRTOS pulls in "portmacro.h" via
    - source.c <- FreeRTOS.h <- portable.h <- portmacro.h

- If we need types of task.h (such as TaskHandle_t), best put the feature somewhere else
- Be careful as some parts of C Standard Library Headers will pull in FreeRTOS headers
*/

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
ਬਾਹਰੀ "C" {
#ਜੇ_ਅੰਤ

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come before first as they are used further down the file
 * ------------------------------------------------------------------------------------------------------------------ */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_CHAR                    int8_t
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_FLOAT                   ਦਸ਼ਮਲਵ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_DOUBLE                  ਦੁੱਗਣਾ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_LONG                    int32_t
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_SHORT                   int16_t
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਢੇਰ_TYPE              uint8_t
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਅਧਾਰ_TYPE               ਪੂਰਨ

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੋਰਟ_ਢੇਰ_TYPE              ਢੇਰ_Type_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੋਰਟ_ਅਧਾਰ_TYPE               ਅਧਾਰ_Type_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੋਰਟ_ਅਧਾਰ_TYPE      UBaseType_t;

#ਜੇ( config_USE_16_ਦੋਆਧਾਰੀ_ਅੰਕ_TICKS == 1 )
ਕਿਸਮਪਰਿਭਾਸ਼ਾ uint16_t TickType_t;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_MAX_DELAY ( TickType_t ) 0xffff
#ਨਹੀਂਤਾਂ
ਕਿਸਮਪਰਿਭਾਸ਼ਾ uint32_t TickType_t;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_MAX_DELAY ( TickType_t ) 0xffffffffUL
#ਜੇ_ਅੰਤ

/* Task function macros as described on the FreeRTOS.org WEB site. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TASK_FUNCTION_PROTO( vFunction, pvParameters )  ਖਾਲੀ vFunction( ਖਾਲੀ *pvParameters )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TASK_FUNCTION( vFunction, pvParameters )        ਖਾਲੀ vFunction( ਖਾਲੀ *pvParameters )

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_CRITICAL_NESTING_ਵਿੱਚ_TCB     1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਢੇਰ_GROWTH                ( -1 )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TICK_PERIOD_MS              ( ( TickType_t ) 1000 / config_TICK_ਦਰ_HZ )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_BYTE_ALIGNMENT              16    // Xtensa Windowed ABI requires the stack pointer to always be 16-byte aligned. See "isa_rm.pdf 8.1.1 Windowed Register Usage and Stack Layout"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_NOP()                       XT_NOP()    //Todo: Check if XT_NOP exists

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros need to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Spinlocks ------------------------

ਕਿਸਮਪਰਿਭਾਸ਼ਾ spinlock_t                          ਪੋਰਟ_MUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_MUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_MUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_MUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_MUX_TRY_ਤਾਲਾ                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

ਅਧਾਰ_Type_t x_ਪੋਰਟ_Check_If_ਵਿੱਚ_ISR(ਖਾਲੀ);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortCheckIfInISR() internally
 */
ਖਾਲੀ v_ਪੋਰਟ_Assert_If_ਵਿੱਚ_ISR(ਖਾਲੀ);

// ------------------ Critical Sections --------------------

UBaseType_t ux_ਪੋਰਟ_Enter_Critical_From_ISR( ਖਾਲੀ );
ਖਾਲੀ v_ਪੋਰਟ_Exit_Critical_From_ISR( UBaseType_t level );

#ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 )
/*
These are always called with interrupts already disabled. We simply need to get/release the spinlocks
*/
ਬਾਹਰੀ ਪੋਰਟ_MUX_TYPE ਪੋਰਟ_x_Task_ਤਾਲਾ;
ਬਾਹਰੀ ਪੋਰਟ_MUX_TYPE ਪੋਰਟ_x_ISRLock;

ਖਾਲੀ v_ਪੋਰਟ_Take_ਤਾਲਾ( ਪੋਰਟ_MUX_TYPE *ਤਾਲਾ );
ਖਾਲੀ v_ਪੋਰਟ_Release_ਤਾਲਾ( ਪੋਰਟ_MUX_TYPE *ਤਾਲਾ );
#ਜੇ_ਅੰਤ /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

ਖਾਲੀ v_ਪੋਰਟ_Yield( ਖਾਲੀ );
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ __attribute__((always_inline)) v_ਪੋਰਟ_Yield_Core( ਅਧਾਰ_Type_t xCoreID );
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ __attribute__((always_inline)) v_ਪੋਰਟ_Yield_From_ISR( ਖਾਲੀ );

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_YIELD_FROM_ISR_CHECK(x)     ({ \
    ਜੇ ( (x) == pdTRUE ) { \
        v_ਪੋਰਟ_Yield_From_ISR(); \
    } \
})
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_YIELD_FROM_ISR_NO_CHECK()               v_ਪੋਰਟ_Yield_From_ISR()

// ----------------------- System --------------------------

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਧਾਰ_Type_t __attribute__((always_inline)) x_ਪੋਰਟ_Get_Core_ID( ਖਾਲੀ );

// --------------------- TCB Cleanup -----------------------

/**
 * @brief TCB cleanup hook
 *
 * The portCLEAN_UP_TCB() macro is called in prvDeleteTCB() right before a
 * deleted task's memory is freed. We map that macro to this internal function
 * so that IDF FreeRTOS ports can inject some task pre-deletion operations.
 */
ਖਾਲੀ v_ਪੋਰਟ_TCBPre_Delete_Hook( ਖਾਲੀ *pxTCB );


/* ----------------------------------------- FreeRTOS SMP Porting Interface --------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS SMP
 * - Must come after forward declare as some porting macros map to declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ()            ({ \
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ prev_level = XTOS_ਸੈੱਟ_INTLEVEL(XCHAL_EXCM_LEVEL); \
    portbenchmark_ਰੁਕਾਵਟ_DISABLE(); \
    prev_level = ((prev_level >> XCHAL_PS_INTLEVEL_ਖਿਸਕਾਓ) & XCHAL_PS_INTLEVEL_ਨਕਾਬ); \
    prev_level; \
})
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR()   ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ()
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_DISABLE_INTERRUPTS()            ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ()

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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_CLEAR_ਰੁਕਾਵਟ_ਨਕਾਬ(x)         ({ \
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ps_reg; \
    RSR(XT_REG_PS, ps_reg); \
    ps_reg = (ps_reg & ~XCHAL_PS_INTLEVEL_ਨਕਾਬ); \
    ps_reg |= ((x << XCHAL_PS_INTLEVEL_ਖਿਸਕਾਓ) & XCHAL_PS_INTLEVEL_ਨਕਾਬ); \
    XTOS_RESTORE_INTLEVEL(ps_reg); \
})
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_CLEAR_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR(x)    ਪੋਰਟ_CLEAR_ਰੁਕਾਵਟ_ਨਕਾਬ(x)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENABLE_INTERRUPTS()     ({ \
    portbenchmark_ਰੁਕਾਵਟ_RESTORE(0); \
    XTOS_ਸੈੱਟ_INTLEVEL(0); \
})

/* Compatibility */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_RESTORE_INTERRUPTS(x)   ਪੋਰਟ_CLEAR_ਰੁਕਾਵਟ_ਨਕਾਬ(x)

// ------------------ Critical Sections --------------------

#ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_GET_TASK_ਤਾਲਾ()                         v_ਪੋਰਟ_Take_ਤਾਲਾ(&ਪੋਰਟ_x_Task_ਤਾਲਾ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_RELEASE_TASK_ਤਾਲਾ()                     v_ਪੋਰਟ_Release_ਤਾਲਾ(&ਪੋਰਟ_x_Task_ਤਾਲਾ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_GET_ISR_ਤਾਲਾ()                          v_ਪੋਰਟ_Take_ਤਾਲਾ(&ਪੋਰਟ_x_ISRLock)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_RELEASE_ISR_ਤਾਲਾ()                      v_ਪੋਰਟ_Release_ਤਾਲਾ(&ਪੋਰਟ_x_ISRLock)
#ਜੇ_ਅੰਤ /* configNUMBER_OF_CORES > 1 */

//Critical sections used by FreeRTOS SMP
ਬਾਹਰੀ ਖਾਲੀ vTaskEnterCritical( ਖਾਲੀ );
ਬਾਹਰੀ ਖਾਲੀ vTaskExitCritical( ਖਾਲੀ );
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENTER_CRITICAL_SMP()                    vTaskEnterCritical();
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_EXIT_CRITICAL_SMP()                     vTaskExitCritical();

#ਜੇ defined(__cplusplus) && (__cplusplus >  201703L)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENTER_CRITICAL(...)                     ਚੁਣੋ_MACRO_VA_ARG(ਪੋਰਟ_ENTER_CRITICAL_IDF, ਪੋਰਟ_ENTER_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_EXIT_CRITICAL(...)                      ਚੁਣੋ_MACRO_VA_ARG(ਪੋਰਟ_EXIT_CRITICAL_IDF, ਪੋਰਟ_EXIT_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENTER_CRITICAL(...)                     ਚੁਣੋ_MACRO_VA_ARG(ਪੋਰਟ_ENTER_CRITICAL_IDF, ਪੋਰਟ_ENTER_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_EXIT_CRITICAL(...)                      ਚੁਣੋ_MACRO_VA_ARG(ਪੋਰਟ_EXIT_CRITICAL_IDF, ਪੋਰਟ_EXIT_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#ਜੇ_ਅੰਤ

ਬਾਹਰੀ UBaseType_t vTaskEnterCriticalFromISR( ਖਾਲੀ );
ਬਾਹਰੀ ਖਾਲੀ vTaskExitCriticalFromISR( UBaseType_t ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ );
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENTER_CRITICAL_FROM_ISR() vTaskEnterCriticalFromISR()
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_EXIT_CRITICAL_FROM_ISR(x) vTaskExitCriticalFromISR(x)

// ---------------------- Yielding -------------------------

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_YIELD()                                 v_ਪੋਰਟ_Yield()
#ਜੇ defined(__cplusplus) && (__cplusplus >  201703L)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_YIELD_FROM_ISR(...)                     ਚੁਣੋ_MACRO_VA_ARG(ਪੋਰਟ_YIELD_FROM_ISR_CHECK, ਪੋਰਟ_YIELD_FROM_ISR_NO_CHECK __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_YIELD_FROM_ISR(...)                     ਚੁਣੋ_MACRO_VA_ARG(ਪੋਰਟ_YIELD_FROM_ISR_CHECK, ਪੋਰਟ_YIELD_FROM_ISR_NO_CHECK, ##__VA_ARGS__)(__VA_ARGS__)
#ਜੇ_ਅੰਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_YIELD_CORE(x)                           v_ਪੋਰਟ_Yield_Core(x)

// ----------------------- System --------------------------

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_GET_CORE_ID()                           x_ਪੋਰਟ_Get_Core_ID()
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_CHECK_IF_ਵਿੱਚ_ISR()                       x_ਪੋਰਟ_Check_If_ਵਿੱਚ_ISR()

// ------------------- Run Time Stats ----------------------

//Timers are already configured, so nothing to do for configuration of run time stats timer
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_CONFIGURE_TIMER_FOR_ਲੜੀਵਾਰ_ਸਮਾਂ_STATS()
#ਜੇ ( CONFIG_FREERTOS_GENERATE_ਲੜੀਵਾਰ_ਸਮਾਂ_STATS )
config_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_TYPE x_ਪੋਰਟ_Get_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter_ਮੁੱਲ( ਖਾਲੀ );
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_GET_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_ਮੁੱਲ()        x_ਪੋਰਟ_Get_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter_ਮੁੱਲ()
#ਜੇ_ਅੰਤ

// --------------------- TCB Cleanup -----------------------

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_CLEAN_ਉੱਪਰ_TCB( pxTCB )                   v_ਪੋਰਟ_TCBPre_Delete_Hook( pxTCB )

/* --------------------------------------------- Inline Implementations ------------------------------------------------
 * - Implementation of inline functions of the forward declares
 * - Should come after forward declare and FreeRTOS Porting interface, as implementation may use both.
 * - For implementation of non-inlined functions, see port.c
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

// ------------------ Critical Sections --------------------

// ---------------------- Yielding -------------------------

ਬਾਹਰੀ ਖਾਲੀ frxt_ਤਿਆਰੀ_switch( ਖਾਲੀ );     //Defined in portasm.S

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ __attribute__((always_inline)) v_ਪੋਰਟ_Yield_Core( ਅਧਾਰ_Type_t xCoreID )
{
    esp_crosscore_int_send_yield( xCoreID );
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ __attribute__((always_inline)) v_ਪੋਰਟ_Yield_From_ISR( ਖਾਲੀ )
{
    //traceISR_EXIT_TO_SCHEDULER();
    frxt_ਤਿਆਰੀ_switch();
}

// ----------------------- System --------------------------

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਧਾਰ_Type_t __attribute__((always_inline)) x_ਪੋਰਟ_Get_Core_ID( ਖਾਲੀ )
{
    ਵਾਪਸ (ਅਧਾਰ_Type_t) esp_cpu_get_core_id();
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਧਾਰ_Type_t x_ਪੋਰਟ_ਵਿੱਚ_Isr_Context(ਖਾਲੀ)
{
    //Just call the FreeRTOS port interface version
    ਵਾਪਸ x_ਪੋਰਟ_Check_If_ਵਿੱਚ_ISR();
}

ਅਧਾਰ_Type_t x_ਪੋਰਟ_Interrupted_From_ISRContext(ਖਾਲੀ);

ਸਥਿਰ ਸਤਰਅੰਦਰ UBaseType_t x_ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ_From_ISR(ਖਾਲੀ)
{
    UBaseType_t prev_int_level = XTOS_ਸੈੱਟ_INTLEVEL(XCHAL_EXCM_LEVEL);
    portbenchmark_ਰੁਕਾਵਟ_DISABLE();
    ਵਾਪਸ prev_int_level;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ v_ਪੋਰਟ_Clear_ਰੁਕਾਵਟ_ਨਕਾਬ_From_ISR(UBaseType_t prev_level)
{
    portbenchmark_ਰੁਕਾਵਟ_RESTORE(prev_level);
    XTOS_RESTORE_JUST_INTLEVEL((ਪੂਰਨ) prev_level);
}

// ---------------------- Spinlocks ------------------------

ਸਥਿਰ ਸਤਰਅੰਦਰ bool __attribute__((always_inline)) v_ਪੋਰਟ_CPUAcquire_Mutex_Timeout(ਪੋਰਟ_MUX_TYPE *mux, ਪੂਰਨ timeout)
{
    ਵਾਪਸ (spinlock_acquire(mux, timeout));
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ __attribute__((always_inline)) v_ਪੋਰਟ_CPURelease_Mutex(ਪੋਰਟ_MUX_TYPE *mux)
{
    spinlock_release(mux);
}

// ------------------ Critical Sections --------------------

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

ਬਾਹਰੀ ਅਸਥਿਰ bool ਪੋਰਟ_x_Thread_ਸੁਰੱਖਿਅਤ_Claimed;
#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

ਅਧਾਰ_Type_t x_ਪੋਰਟ_Enter_Critical_Timeout(ਪੋਰਟ_MUX_TYPE *ਤਾਲਾ, ਅਧਾਰ_Type_t timeout);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ __attribute__((always_inline)) v_ਪੋਰਟ_Enter_Critical_IDF(ਪੋਰਟ_MUX_TYPE *ਤਾਲਾ)
{
    x_ਪੋਰਟ_Enter_Critical_Timeout(ਤਾਲਾ, ਪੋਰਟ_MUX_NO_TIMEOUT);
}

ਖਾਲੀ v_ਪੋਰਟ_Exit_Critical_IDF(ਪੋਰਟ_MUX_TYPE *ਤਾਲਾ);

/*
In case any IDF libs called the port critical functions directly instead of through the macros.
The will just call the vPort...CriticalIDF() functions
*/
ਖਾਲੀ v_ਪੋਰਟ_Enter_Critical(ਪੋਰਟ_MUX_TYPE *ਤਾਲਾ);
ਖਾਲੀ v_ਪੋਰਟ_Exit_Critical(ਪੋਰਟ_MUX_TYPE *ਤਾਲਾ);

//IDF task critical sections
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TRY_ENTER_CRITICAL(ਤਾਲਾ, timeout)       x_ਪੋਰਟ_Enter_Critical_Timeout(ਤਾਲਾ, timeout)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENTER_CRITICAL_IDF(ਤਾਲਾ)                v_ਪੋਰਟ_Enter_Critical_IDF(ਤਾਲਾ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_EXIT_CRITICAL_IDF(ਤਾਲਾ)                 v_ਪੋਰਟ_Exit_Critical_IDF(ਤਾਲਾ)
//IDF ISR critical sections
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TRY_ENTER_CRITICAL_ISR(ਤਾਲਾ, timeout)   x_ਪੋਰਟ_Enter_Critical_Timeout(ਤਾਲਾ, timeout)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENTER_CRITICAL_ISR(ਤਾਲਾ)                v_ਪੋਰਟ_Enter_Critical_IDF(ਤਾਲਾ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_EXIT_CRITICAL_ISR(ਤਾਲਾ)                 v_ਪੋਰਟ_Exit_Critical_IDF(ਤਾਲਾ)
//IDF safe critical sections (they're the same)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENTER_CRITICAL_ਸੁਰੱਖਿਅਤ(ਤਾਲਾ)               v_ਪੋਰਟ_Enter_Critical_IDF(ਤਾਲਾ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_EXIT_CRITICAL_ਸੁਰੱਖਿਅਤ(ਤਾਲਾ)                v_ਪੋਰਟ_Exit_Critical_IDF(ਤਾਲਾ)

// ---------------------- Yielding -------------------------

ਸਥਿਰ ਸਤਰਅੰਦਰ bool x_ਪੋਰਟ_Can_Yield(ਖਾਲੀ)
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

    ਵਾਪਸ ((ps_reg & PS_INTLEVEL_ਨਕਾਬ) == 0);
}

// Defined even for configNUMBER_OF_CORES > 1 for IDF compatibility
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_YIELD_WITHIN_API()                      esp_crosscore_int_send_yield(x_ਪੋਰਟ_Get_Core_ID())

// ----------------------- System --------------------------

ਖਾਲੀ v_ਪੋਰਟ_ਸੈੱਟ_ਢੇਰ_Watchpoint(ਖਾਲੀ *px_ਢੇਰ_ਸ਼ੁਰੂ);

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

/* ------------------------------------------------------ Misc ---------------------------------------------------------
 * - Miscellaneous porting macros
 * - These are not part of the FreeRTOS porting interface, but are used by other FreeRTOS dependent components
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- App-Trace -------------------------

#ਜੇ CONFIG_ESP_TRACE_ENABLE
ਬਾਹਰੀ ਅਸਥਿਰ ਅਧਾਰ_Type_t x_ਪੋਰਟ_Switch_Flag;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ os_task_switch_is_pended(_cpu_) (x_ਪੋਰਟ_Switch_Flag)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ os_task_switch_is_pended(_cpu_) (false)
#ਜੇ_ਅੰਤ

// --------------- Compatibility Includes ------------------
/*
ESP-IDF currently does not have a "Include what you use" policy. A lot of files implicitly pull in API through
portmacro.h. Therefore, we need to keep these headers around for now to allow the rest of IDF to compile.

[refactor-todo] Clean up ESP-IDF inclusion dependencies and add a inclusion check.
*/

#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdarg.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <xtensa/hal.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_attr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_newlib.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_heap_caps.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_rom_sys.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "portbenchmark.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <xtensa/config/system.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <xtensa_api.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
}
#ਜੇ_ਅੰਤ
