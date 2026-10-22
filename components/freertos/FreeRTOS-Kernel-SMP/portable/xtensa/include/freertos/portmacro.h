/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ "spinlock.h"
#ಅನ್ತರ್ಭಾವ "xtensa/xtruntime.h"
#ಅನ್ತರ್ಭಾವ "xtensa/config/core.h"
#ಅನ್ತರ್ಭಾವ "xtensa/config/core-isa.h"
#ಅನ್ತರ್ಭಾವ "xtensa/config/xt_specreg.h"
#ಅನ್ತರ್ಭಾವ "xt_instr_macros.h"
#ಅನ್ತರ್ಭಾವ "portbenchmark.h"
#ಅನ್ತರ್ಭಾವ "esp_macros.h"
#ಅನ್ತರ್ಭಾವ "esp_cpu.h"
#ಅನ್ತರ್ಭಾವ "esp_private/crosscore_int.h"
#ಅನ್ತರ್ಭಾವ "esp_memory_utils.h"

/*
Note: We should not include any FreeRTOS headers (directly or indirectly) here as this will create a reverse dependency
- FreeRTOS pulls in "portmacro.h" via
    - source.c <- FreeRTOS.h <- portable.h <- portmacro.h

- If we need types of task.h (such as TaskHandle_t), best put the feature somewhere else
- Be careful as some parts of C Standard Library Headers will pull in FreeRTOS headers
*/

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

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_CRITICAL_NESTING_ಒಳ_TCB     1
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ರಾಶಿ_GROWTH                ( -1 )
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TICK_PERIOD_MS              ( ( TickType_t ) 1000 / config_TICK_ದರ_HZ )
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_BYTE_ALIGNMENT              16    // Xtensa Windowed ABI requires the stack pointer to always be 16-byte aligned. See "isa_rm.pdf 8.1.1 Windowed Register Usage and Stack Layout"
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_NOP()                       XT_NOP()    //Todo: Check if XT_NOP exists

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros need to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Spinlocks ------------------------

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ spinlock_t                          ಪೋರ್ಟ್_MUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_MUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_MUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_MUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_MUX_TRY_ಬೀಗ                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

ತಳ_Type_t x_ಪೋರ್ಟ್_Check_If_ಒಳ_ISR(ಶೂನ್ಯ);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortCheckIfInISR() internally
 */
ಶೂನ್ಯ v_ಪೋರ್ಟ್_Assert_If_ಒಳ_ISR(ಶೂನ್ಯ);

// ------------------ Critical Sections --------------------

UBaseType_t ux_ಪೋರ್ಟ್_Enter_Critical_From_ISR( ಶೂನ್ಯ );
ಶೂನ್ಯ v_ಪೋರ್ಟ್_Exit_Critical_From_ISR( UBaseType_t level );

#ಆದರೆ ( config_ಸಂಖ್ಯೆ_ದ_CORES > 1 )
/*
These are always called with interrupts already disabled. We simply need to get/release the spinlocks
*/
ಬಾಹ್ಯ ಪೋರ್ಟ್_MUX_TYPE ಪೋರ್ಟ್_x_Task_ಬೀಗ;
ಬಾಹ್ಯ ಪೋರ್ಟ್_MUX_TYPE ಪೋರ್ಟ್_x_ISRLock;

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Take_ಬೀಗ( ಪೋರ್ಟ್_MUX_TYPE *ಬೀಗ_2 );
ಶೂನ್ಯ v_ಪೋರ್ಟ್_Release_ಬೀಗ( ಪೋರ್ಟ್_MUX_TYPE *ಬೀಗ_2 );
#ಆದರೆ_ಅಂತ್ಯ /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Yield( ಶೂನ್ಯ );
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ __attribute__((always_inline)) v_ಪೋರ್ಟ್_Yield_Core( ತಳ_Type_t xCoreID );
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ __attribute__((always_inline)) v_ಪೋರ್ಟ್_Yield_From_ISR( ಶೂನ್ಯ );

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_YIELD_FROM_ISR_CHECK(x)     ({ \
    ಆದರೆ ( (x) == pdTRUE ) { \
        v_ಪೋರ್ಟ್_Yield_From_ISR(); \
    } \
})
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_YIELD_FROM_ISR_NO_CHECK()               v_ಪೋರ್ಟ್_Yield_From_ISR()

// ----------------------- System --------------------------

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಳ_Type_t __attribute__((always_inline)) x_ಪೋರ್ಟ್_Get_Core_ID( ಶೂನ್ಯ );

// --------------------- TCB Cleanup -----------------------

/**
 * @brief TCB cleanup hook
 *
 * The portCLEAN_UP_TCB() macro is called in prvDeleteTCB() right before a
 * deleted task's memory is freed. We map that macro to this internal function
 * so that IDF FreeRTOS ports can inject some task pre-deletion operations.
 */
ಶೂನ್ಯ v_ಪೋರ್ಟ್_TCBPre_Delete_Hook( ಶೂನ್ಯ *pxTCB );


/* ----------------------------------------- FreeRTOS SMP Porting Interface --------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS SMP
 * - Must come after forward declare as some porting macros map to declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ()            ({ \
    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ prev_level = XTOS_ಗಣ_INTLEVEL(XCHAL_EXCM_LEVEL); \
    portbenchmark_ಅಡಚಣೆ_DISABLE(); \
    prev_level = ((prev_level >> XCHAL_PS_INTLEVEL_ಸರಿಸು) & XCHAL_PS_INTLEVEL_ಮುಖವಾಡ); \
    prev_level; \
})
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR()   ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ()
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_DISABLE_INTERRUPTS()            ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ()

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
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_CLEAR_ಅಡಚಣೆ_ಮುಖವಾಡ(x)         ({ \
    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ps_reg; \
    RSR(XT_REG_PS, ps_reg); \
    ps_reg = (ps_reg & ~XCHAL_PS_INTLEVEL_ಮುಖವಾಡ); \
    ps_reg |= ((x << XCHAL_PS_INTLEVEL_ಸರಿಸು) & XCHAL_PS_INTLEVEL_ಮುಖವಾಡ); \
    XTOS_RESTORE_INTLEVEL(ps_reg); \
})
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_CLEAR_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR(x)    ಪೋರ್ಟ್_CLEAR_ಅಡಚಣೆ_ಮುಖವಾಡ(x)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ENABLE_INTERRUPTS()     ({ \
    portbenchmark_ಅಡಚಣೆ_RESTORE(0); \
    XTOS_ಗಣ_INTLEVEL(0); \
})

/* Compatibility */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_RESTORE_INTERRUPTS(x)   ಪೋರ್ಟ್_CLEAR_ಅಡಚಣೆ_ಮುಖವಾಡ(x)

// ------------------ Critical Sections --------------------

#ಆದರೆ ( config_ಸಂಖ್ಯೆ_ದ_CORES > 1 )
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_GET_TASK_ಬೀಗ()                         v_ಪೋರ್ಟ್_Take_ಬೀಗ(&ಪೋರ್ಟ್_x_Task_ಬೀಗ)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_RELEASE_TASK_ಬೀಗ()                     v_ಪೋರ್ಟ್_Release_ಬೀಗ(&ಪೋರ್ಟ್_x_Task_ಬೀಗ)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_GET_ISR_ಬೀಗ()                          v_ಪೋರ್ಟ್_Take_ಬೀಗ(&ಪೋರ್ಟ್_x_ISRLock)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_RELEASE_ISR_ಬೀಗ()                      v_ಪೋರ್ಟ್_Release_ಬೀಗ(&ಪೋರ್ಟ್_x_ISRLock)
#ಆದರೆ_ಅಂತ್ಯ /* configNUMBER_OF_CORES > 1 */

//Critical sections used by FreeRTOS SMP
ಬಾಹ್ಯ ಶೂನ್ಯ vTaskEnterCritical( ಶೂನ್ಯ );
ಬಾಹ್ಯ ಶೂನ್ಯ vTaskExitCritical( ಶೂನ್ಯ );
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ENTER_CRITICAL_SMP()                    vTaskEnterCritical();
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_EXIT_CRITICAL_SMP()                     vTaskExitCritical();

#ಆದರೆ defined(__cplusplus) && (__cplusplus >  201703L)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ENTER_CRITICAL(...)                     ಆರಿಸು_MACRO_VA_ARG(ಪೋರ್ಟ್_ENTER_CRITICAL_IDF, ಪೋರ್ಟ್_ENTER_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_EXIT_CRITICAL(...)                      ಆರಿಸು_MACRO_VA_ARG(ಪೋರ್ಟ್_EXIT_CRITICAL_IDF, ಪೋರ್ಟ್_EXIT_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ENTER_CRITICAL(...)                     ಆರಿಸು_MACRO_VA_ARG(ಪೋರ್ಟ್_ENTER_CRITICAL_IDF, ಪೋರ್ಟ್_ENTER_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_EXIT_CRITICAL(...)                      ಆರಿಸು_MACRO_VA_ARG(ಪೋರ್ಟ್_EXIT_CRITICAL_IDF, ಪೋರ್ಟ್_EXIT_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#ಆದರೆ_ಅಂತ್ಯ

ಬಾಹ್ಯ UBaseType_t vTaskEnterCriticalFromISR( ಶೂನ್ಯ );
ಬಾಹ್ಯ ಶೂನ್ಯ vTaskExitCriticalFromISR( UBaseType_t ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ );
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ENTER_CRITICAL_FROM_ISR() vTaskEnterCriticalFromISR()
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_EXIT_CRITICAL_FROM_ISR(x) vTaskExitCriticalFromISR(x)

// ---------------------- Yielding -------------------------

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_YIELD()                                 v_ಪೋರ್ಟ್_Yield()
#ಆದರೆ defined(__cplusplus) && (__cplusplus >  201703L)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_YIELD_FROM_ISR(...)                     ಆರಿಸು_MACRO_VA_ARG(ಪೋರ್ಟ್_YIELD_FROM_ISR_CHECK, ಪೋರ್ಟ್_YIELD_FROM_ISR_NO_CHECK __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_YIELD_FROM_ISR(...)                     ಆರಿಸು_MACRO_VA_ARG(ಪೋರ್ಟ್_YIELD_FROM_ISR_CHECK, ಪೋರ್ಟ್_YIELD_FROM_ISR_NO_CHECK, ##__VA_ARGS__)(__VA_ARGS__)
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_YIELD_CORE(x)                           v_ಪೋರ್ಟ್_Yield_Core(x)

// ----------------------- System --------------------------

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_GET_CORE_ID()                           x_ಪೋರ್ಟ್_Get_Core_ID()
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_CHECK_IF_ಒಳ_ISR()                       x_ಪೋರ್ಟ್_Check_If_ಒಳ_ISR()

// ------------------- Run Time Stats ----------------------

//Timers are already configured, so nothing to do for configuration of run time stats timer
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_CONFIGURE_TIMER_FOR_ಸತತ_ಸಮಯ_STATS()
#ಆದರೆ ( CONFIG_FREERTOS_GENERATE_ಸತತ_ಸಮಯ_STATS )
config_ಸತತ_ಸಮಯ_COUNTER_TYPE x_ಪೋರ್ಟ್_Get_ಸತತ_ಸಮಯ_Counter_ಮೌಲ್ಯ( ಶೂನ್ಯ );
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_GET_ಸತತ_ಸಮಯ_COUNTER_ಮೌಲ್ಯ()        x_ಪೋರ್ಟ್_Get_ಸತತ_ಸಮಯ_Counter_ಮೌಲ್ಯ()
#ಆದರೆ_ಅಂತ್ಯ

// --------------------- TCB Cleanup -----------------------

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_CLEAN_ಮೇಲೆ_TCB( pxTCB )                   v_ಪೋರ್ಟ್_TCBPre_Delete_Hook( pxTCB )

/* --------------------------------------------- Inline Implementations ------------------------------------------------
 * - Implementation of inline functions of the forward declares
 * - Should come after forward declare and FreeRTOS Porting interface, as implementation may use both.
 * - For implementation of non-inlined functions, see port.c
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

// ------------------ Critical Sections --------------------

// ---------------------- Yielding -------------------------

ಬಾಹ್ಯ ಶೂನ್ಯ frxt_ಸಿದ್ಧತೆ_switch( ಶೂನ್ಯ );     //Defined in portasm.S

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ __attribute__((always_inline)) v_ಪೋರ್ಟ್_Yield_Core( ತಳ_Type_t xCoreID )
{
    esp_crosscore_int_send_yield( xCoreID );
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ __attribute__((always_inline)) v_ಪೋರ್ಟ್_Yield_From_ISR( ಶೂನ್ಯ )
{
    //traceISR_EXIT_TO_SCHEDULER();
    frxt_ಸಿದ್ಧತೆ_switch();
}

// ----------------------- System --------------------------

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಳ_Type_t __attribute__((always_inline)) x_ಪೋರ್ಟ್_Get_Core_ID( ಶೂನ್ಯ )
{
    ಹಿಂತಿರುಗಿ (ತಳ_Type_t) esp_cpu_get_core_id();
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಳ_Type_t x_ಪೋರ್ಟ್_ಒಳ_Isr_Context(ಶೂನ್ಯ)
{
    //Just call the FreeRTOS port interface version
    ಹಿಂತಿರುಗಿ x_ಪೋರ್ಟ್_Check_If_ಒಳ_ISR();
}

ತಳ_Type_t x_ಪೋರ್ಟ್_Interrupted_From_ISRContext(ಶೂನ್ಯ);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ UBaseType_t x_ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ_From_ISR(ಶೂನ್ಯ)
{
    UBaseType_t prev_int_level = XTOS_ಗಣ_INTLEVEL(XCHAL_EXCM_LEVEL);
    portbenchmark_ಅಡಚಣೆ_DISABLE();
    ಹಿಂತಿರುಗಿ prev_int_level;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_ಪೋರ್ಟ್_Clear_ಅಡಚಣೆ_ಮುಖವಾಡ_From_ISR(UBaseType_t prev_level)
{
    portbenchmark_ಅಡಚಣೆ_RESTORE(prev_level);
    XTOS_RESTORE_JUST_INTLEVEL((ಪೂರ್ಣಾಂಕ) prev_level);
}

// ---------------------- Spinlocks ------------------------

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool __attribute__((always_inline)) v_ಪೋರ್ಟ್_CPUAcquire_Mutex_Timeout(ಪೋರ್ಟ್_MUX_TYPE *mux, ಪೂರ್ಣಾಂಕ timeout)
{
    ಹಿಂತಿರುಗಿ (spinlock_acquire(mux, timeout));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ __attribute__((always_inline)) v_ಪೋರ್ಟ್_CPURelease_Mutex(ಪೋರ್ಟ್_MUX_TYPE *mux)
{
    spinlock_release(mux);
}

// ------------------ Critical Sections --------------------

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

ಬಾಹ್ಯ ಅಸ್ಥಿರ bool ಪೋರ್ಟ್_x_Thread_ಸುರಕ್ಷಿತ_Claimed;
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

ತಳ_Type_t x_ಪೋರ್ಟ್_Enter_Critical_Timeout(ಪೋರ್ಟ್_MUX_TYPE *ಬೀಗ_2, ತಳ_Type_t timeout);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ __attribute__((always_inline)) v_ಪೋರ್ಟ್_Enter_Critical_IDF(ಪೋರ್ಟ್_MUX_TYPE *ಬೀಗ_2)
{
    x_ಪೋರ್ಟ್_Enter_Critical_Timeout(ಬೀಗ_2, ಪೋರ್ಟ್_MUX_NO_TIMEOUT);
}

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Exit_Critical_IDF(ಪೋರ್ಟ್_MUX_TYPE *ಬೀಗ_2);

/*
In case any IDF libs called the port critical functions directly instead of through the macros.
The will just call the vPort...CriticalIDF() functions
*/
ಶೂನ್ಯ v_ಪೋರ್ಟ್_Enter_Critical(ಪೋರ್ಟ್_MUX_TYPE *ಬೀಗ_2);
ಶೂನ್ಯ v_ಪೋರ್ಟ್_Exit_Critical(ಪೋರ್ಟ್_MUX_TYPE *ಬೀಗ_2);

//IDF task critical sections
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TRY_ENTER_CRITICAL(ಬೀಗ_2, timeout)       x_ಪೋರ್ಟ್_Enter_Critical_Timeout(ಬೀಗ_2, timeout)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ENTER_CRITICAL_IDF(ಬೀಗ_2)                v_ಪೋರ್ಟ್_Enter_Critical_IDF(ಬೀಗ_2)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_EXIT_CRITICAL_IDF(ಬೀಗ_2)                 v_ಪೋರ್ಟ್_Exit_Critical_IDF(ಬೀಗ_2)
//IDF ISR critical sections
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TRY_ENTER_CRITICAL_ISR(ಬೀಗ_2, timeout)   x_ಪೋರ್ಟ್_Enter_Critical_Timeout(ಬೀಗ_2, timeout)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ENTER_CRITICAL_ISR(ಬೀಗ_2)                v_ಪೋರ್ಟ್_Enter_Critical_IDF(ಬೀಗ_2)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_EXIT_CRITICAL_ISR(ಬೀಗ_2)                 v_ಪೋರ್ಟ್_Exit_Critical_IDF(ಬೀಗ_2)
//IDF safe critical sections (they're the same)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ENTER_CRITICAL_ಸುರಕ್ಷಿತ(ಬೀಗ_2)               v_ಪೋರ್ಟ್_Enter_Critical_IDF(ಬೀಗ_2)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_EXIT_CRITICAL_ಸುರಕ್ಷಿತ(ಬೀಗ_2)                v_ಪೋರ್ಟ್_Exit_Critical_IDF(ಬೀಗ_2)

// ---------------------- Yielding -------------------------

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool x_ಪೋರ್ಟ್_Can_Yield(ಶೂನ್ಯ)
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

// Defined even for configNUMBER_OF_CORES > 1 for IDF compatibility
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_YIELD_WITHIN_API()                      esp_crosscore_int_send_yield(x_ಪೋರ್ಟ್_Get_Core_ID())

// ----------------------- System --------------------------

ಶೂನ್ಯ v_ಪೋರ್ಟ್_ಗಣ_ರಾಶಿ_Watchpoint(ಶೂನ್ಯ *px_ರಾಶಿ_ಆರಂಭ);

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

/* ------------------------------------------------------ Misc ---------------------------------------------------------
 * - Miscellaneous porting macros
 * - These are not part of the FreeRTOS porting interface, but are used by other FreeRTOS dependent components
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- App-Trace -------------------------

#ಆದರೆ CONFIG_ESP_TRACE_ENABLE
ಬಾಹ್ಯ ಅಸ್ಥಿರ ತಳ_Type_t x_ಪೋರ್ಟ್_Switch_Flag;
#ವ್ಯಾಖ್ಯಾನಿಸು os_task_switch_is_pended(_cpu_) (x_ಪೋರ್ಟ್_Switch_Flag)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು os_task_switch_is_pended(_cpu_) (false)
#ಆದರೆ_ಅಂತ್ಯ

// --------------- Compatibility Includes ------------------
/*
ESP-IDF currently does not have a "Include what you use" policy. A lot of files implicitly pull in API through
portmacro.h. Therefore, we need to keep these headers around for now to allow the rest of IDF to compile.

[refactor-todo] Clean up ESP-IDF inclusion dependencies and add a inclusion check.
*/

#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <stdarg.h>
#ಅನ್ತರ್ಭಾವ <xtensa/hal.h>
#ಅನ್ತರ್ಭಾವ "esp_attr.h"
#ಅನ್ತರ್ಭಾವ "esp_newlib.h"
#ಅನ್ತರ್ಭಾವ "esp_heap_caps.h"
#ಅನ್ತರ್ಭಾವ "esp_rom_sys.h"
#ಅನ್ತರ್ಭಾವ "esp_system.h"
#ಅನ್ತರ್ಭಾವ "portbenchmark.h"
#ಅನ್ತರ್ಭಾವ <limits.h>
#ಅನ್ತರ್ಭಾವ <xtensa/config/system.h>
#ಅನ್ತರ್ಭಾವ <xtensa_api.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
}
#ಆದರೆ_ಅಂತ್ಯ
