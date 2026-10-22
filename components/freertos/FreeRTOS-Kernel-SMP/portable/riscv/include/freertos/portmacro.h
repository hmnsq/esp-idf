/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"

/* Macros used instead ofsetoff() for better performance of interrupt handler */
#ਜੇ CONFIG_FREERTOS_USE_LIST_ਅੰਕੜੇ_INTEGRITY_CHECK_ਬਾਈਟਾਂ
/*
pxTopOfStack (4) +
xStateListItem (28) +
xEventListItem (28) +
uxPriority (4)
*/
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਵਿੱਥ_PX_ਢੇਰ 0x40
#ਨਹੀਂਤਾਂ
/*
pxTopOfStack (4) +
xStateListItem (20) +
xEventListItem (20) +
uxPriority (4)
*/
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਵਿੱਥ_PX_ਢੇਰ 0x30
#ਜੇ_ਅੰਤ /* #if CONFIG_FREERTOS_USE_LIST_DATA_INTEGRITY_CHECK_BYTES */

#ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TCB_CORE_FIELDS_ਮਾਪ 8
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TCB_CORE_FIELDS_ਮਾਪ 0
#ਜੇ_ਅੰਤ

#ਜੇ ( configUSE_TASK_PREEMPTION_DISABLE == 1 )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TCB_PREEMPT_DISABLE_ਖੇਤਰ_ਮਾਪ 4
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TCB_PREEMPT_DISABLE_ਖੇਤਰ_ਮਾਪ 0
#ਜੇ_ਅੰਤ

/* Align the value up to the nearest multiple of 4 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ALIGN_ਉੱਪਰ_ਮੰਜ਼ਿਲ_4(ਮੁੱਲ) (((ਮੁੱਲ) + 3) & ~3)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਵਿੱਥ_PX_ਸਿਰਾ_ਦਾ_ਢੇਰ ( \
    ਪੋਰਟ_ਵਿੱਥ_PX_ਢੇਰ \
    + 4                                 /* StackType_t * pxStack */ \
    + ਪੋਰਟ_TCB_CORE_FIELDS_ਮਾਪ                     /* BaseType_t xDummy23 + UBaseType_t uxDummy24 */ \
    + ਪੋਰਟ_ALIGN_ਉੱਪਰ_ਮੰਜ਼ਿਲ_4(configMAX_TASK_NAME_LEN)   /* pcTaskName[ configMAX_TASK_NAME_LEN ] */ \
    + ਪੋਰਟ_TCB_PREEMPT_DISABLE_ਖੇਤਰ_ਮਾਪ           /* BaseType_t xDummy25 */ \
)

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __ASSEMBLER__

#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "spinlock.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "soc/interrupt_reg.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_macros.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_cpu.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_private/crosscore_int.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_memory_utils.h"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
ਬਾਹਰੀ "C" {
#ਜੇ_ਅੰਤ

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come first as they are used further down in this file
 * ------------------------------------------------------------------------------------------------------------------ */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_CHAR                    uint8_t
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_FLOAT                   ਦਸ਼ਮਲਵ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_DOUBLE                  ਦੁੱਗਣਾ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_LONG                    int32_t
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_SHORT                   int16_t
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਢੇਰ_TYPE              uint8_t
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਅਧਾਰ_TYPE               ਪੂਰਨ

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੋਰਟ_ਢੇਰ_TYPE              ਢੇਰ_Type_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੋਰਟ_ਅਧਾਰ_TYPE               ਅਧਾਰ_Type_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੋਰਟ_ਅਧਾਰ_TYPE      UBaseType_t;

#ਜੇ ( config_USE_16_ਦੋਆਧਾਰੀ_ਅੰਕ_TICKS == 1 )
ਕਿਸਮਪਰਿਭਾਸ਼ਾ uint16_t TickType_t;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_MAX_DELAY ( TickType_t )  0xffff
#ਨਹੀਂਤਾਂ
ਕਿਸਮਪਰਿਭਾਸ਼ਾ uint32_t TickType_t;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_MAX_DELAY ( TickType_t )  0xffffffffUL
#ਜੇ_ਅੰਤ

/* Task function macros as described on the FreeRTOS.org WEB site. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TASK_FUNCTION_PROTO( vFunction, pvParameters )  ਖਾਲੀ vFunction( ਖਾਲੀ *pvParameters )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TASK_FUNCTION( vFunction, pvParameters )          ਖਾਲੀ vFunction( ਖਾਲੀ *pvParameters )

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_CRITICAL_NESTING_ਵਿੱਚ_TCB     1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਢੇਰ_GROWTH                ( -1 )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TICK_PERIOD_MS              ( ( TickType_t ) ( 1000 / config_TICK_ਦਰ_HZ ) )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_BYTE_ALIGNMENT              16
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_NOP() __asm ਅਸਥਿਰ        (" nop ")

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros needed to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

/* ---------------------- Spinlocks ------------------------
 - Spinlocks added to match API with SMP FreeRTOS. Single core RISC-V does not need spin locks
 * ------------------------------------------------------ */

ਕਿਸਮਪਰਿਭਾਸ਼ਾ spinlock_t                          ਪੋਰਟ_MUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_MUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_MUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_MUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_MUX_TRY_ਤਾਲਾ                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// --------------------- Interrupts ------------------------

ਅਧਾਰ_Type_t x_ਪੋਰਟ_Check_If_ਵਿੱਚ_ISR(ਖਾਲੀ);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortCheckIfInISR() internally
 */
ਖਾਲੀ v_ਪੋਰਟ_Assert_If_ਵਿੱਚ_ISR(ਖਾਲੀ);

// ------------------ Critical Sections --------------------

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

/**
 * @brief Set interrupt mask and return current interrupt enable register
 *
 * @return UBaseType_t Current interrupt enable register before set
 */
UBaseType_t ul_ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ(ਖਾਲੀ);

/**
 * @brief Clear current interrupt mask and set given mask
 *
 * @param mask Interrupt mask
 */
ਖਾਲੀ v_ਪੋਰਟ_Clear_ਰੁਕਾਵਟ_ਨਕਾਬ(UBaseType_t ਨਕਾਬ);

/**
 * @brief Perform a context switch from an ISR
 */
ਖਾਲੀ v_ਪੋਰਟ_Yield_From_ISR(ਖਾਲੀ);

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

/* ------------------------------------------- FreeRTOS Porting Interface ----------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS
 * - Most come after forward declare as porting macros map to declared functions
 * - Maps to forward declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_DISABLE_INTERRUPTS()                    ul_ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ()
#ਜੇ !SOC_INT_CLIC_SUPPORTED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENABLE_INTERRUPTS()                     v_ਪੋਰਟ_Clear_ਰੁਕਾਵਟ_ਨਕਾਬ(RVHAL_INTR_ENABLE_THRESH)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENABLE_INTERRUPTS()                     v_ਪੋਰਟ_Clear_ਰੁਕਾਵਟ_ਨਕਾਬ(RVHAL_INTR_ENABLE_THRESH_CLIC)
#ਜੇ_ਅੰਤ /* !SOC_INT_CLIC_SUPPORTED */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_RESTORE_INTERRUPTS(x)                   v_ਪੋਰਟ_Clear_ਰੁਕਾਵਟ_ਨਕਾਬ(x)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR() ({           \
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cur_level; \
    cur_level = ul_ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ(); \
    cur_level; \
})
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_CLEAR_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR(x)        ਪੋਰਟ_RESTORE_INTERRUPTS(x)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ()                    ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR()
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_CLEAR_ਰੁਕਾਵਟ_ਨਕਾਬ(x)                 ਪੋਰਟ_CLEAR_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR(x)

/**
 * @brief Assert if in ISR context
 *
 * TODO: Enable once ISR safe version of vTaskEnter/ExitCritical() is implemented
 * for single-core SMP FreeRTOS Kernel. (IDF-10540)
 */
// #define portASSERT_IF_IN_ISR() vPortAssertIfInISR()


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
 * - For implementation of non-inlined functions, see port.c, port_common.c, or other assembly files
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Yielding -------------------------

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ __attribute__((always_inline)) v_ਪੋਰਟ_Yield_Core( ਅਧਾਰ_Type_t xCoreID )
{
    esp_crosscore_int_send_yield( xCoreID );
}

// ----------------------- System --------------------------

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਧਾਰ_Type_t __attribute__((always_inline)) x_ਪੋਰਟ_Get_Core_ID( ਖਾਲੀ )
{
    ਵਾਪਸ (ਅਧਾਰ_Type_t) esp_cpu_get_core_id();
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਧਾਰ_Type_t x_ਪੋਰਟ_ਵਿੱਚ_Isr_Context(ਖਾਲੀ)
{
    //Just call the FreeRTOS port interface version
    ਵਾਪਸ x_ਪੋਰਟ_Check_If_ਵਿੱਚ_ISR();
}

// Added for backward compatibility with IDF
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਪੋਰਟ_Interrupted_From_ISRContext()    x_ਪੋਰਟ_ਵਿੱਚ_Isr_Context()

// ------------------ Critical Sections --------------------

/*
IDF style critical sections which are orthogonal to FreeRTOS critical sections. However, on single core, the IDF style
critical sections simply disable interrupts, thus we discard the lock and timeout arguments.

Note: The IDF Style critical sections are named differently to their counterparts present in the xtensa port as few IDF
examples such as esp_zigbee_gateway, when compiled for RISC-V targets, have a reference to vPortEnterCritical()
and vPortExitCritical() from precompiled libraries (.a) thereby failing linking.
*/
ਖਾਲੀ v_ਪੋਰਟ_Enter_Critical(ਖਾਲੀ);
ਖਾਲੀ v_ਪੋਰਟ_Exit_Critical(ਖਾਲੀ);

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

//IDF task critical sections
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TRY_ENTER_CRITICAL(ਤਾਲਾ, timeout)       ({(ਖਾਲੀ) ਤਾਲਾ; (ਖਾਲੀ) timeout; v_ਪੋਰਟ_Enter_Critical(); pdPASS;})
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENTER_CRITICAL_IDF(ਤਾਲਾ)                ({(ਖਾਲੀ) ਤਾਲਾ; v_ਪੋਰਟ_Enter_Critical();})
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_EXIT_CRITICAL_IDF(ਤਾਲਾ)                 ({(ਖਾਲੀ) ਤਾਲਾ; v_ਪੋਰਟ_Exit_Critical();})
//IDF ISR critical sections
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TRY_ENTER_CRITICAL_ISR(ਤਾਲਾ, timeout)   ({(ਖਾਲੀ) ਤਾਲਾ; (ਖਾਲੀ) timeout; v_ਪੋਰਟ_Enter_Critical(); pdPASS;})
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENTER_CRITICAL_ISR(ਤਾਲਾ)                ({(ਖਾਲੀ) ਤਾਲਾ; v_ਪੋਰਟ_Enter_Critical();})
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_EXIT_CRITICAL_ISR(ਤਾਲਾ)                 ({(ਖਾਲੀ) ਤਾਲਾ; v_ਪੋਰਟ_Exit_Critical();})
//IDF safe critical sections (they're the same)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ENTER_CRITICAL_ਸੁਰੱਖਿਅਤ(ਤਾਲਾ)               ({(ਖਾਲੀ) ਤਾਲਾ; v_ਪੋਰਟ_Enter_Critical();})
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_EXIT_CRITICAL_ਸੁਰੱਖਿਅਤ(ਤਾਲਾ)                ({(ਖਾਲੀ) ਤਾਲਾ; v_ਪੋਰਟ_Exit_Critical();})

// ---------------------- Yielding -------------------------

ਸਥਿਰ ਸਤਰਅੰਦਰ bool x_ਪੋਰਟ_Can_Yield(ਖਾਲੀ)
{
#ਜੇ SOC_INT_CLIC_SUPPORTED
    /* When CLIC is supported:
     *  - The lowest interrupt threshold level is 0. Therefore, an interrupt threshold level above 0 would mean that we
     *    are in a critical section.
     *  - Since CLIC enables HW interrupt nesting, we do not have the updated interrupt level in the
     *    INTERRUPT_CURRENT_CORE_INT_THRESH_REG register when nested interrupts occur. To know the current interrupt
     *    level, we read the machine-mode interrupt level (mil) field from the mintstatus CSR. A non-zero value indicates
     *    that we are in an interrupt context.
     */
    uint32_t ਹੱਦ = rv_utils_get_ਰੁਕਾਵਟ_ਹੱਦ();
    uint32_t intr_level = rv_utils_get_ਰੁਕਾਵਟ_level();
    ਵਾਪਸ ((intr_level == 0) && (ਹੱਦ == 0));
#ਨਹੀਂਤਾਂ/* !SOC_INT_CLIC_SUPPORTED */
    uint32_t ਹੱਦ = REG_READ(ਰੁਕਾਵਟ_CURRENT_CORE_INT_THRESH_REG);
    /* when enter critical code, FreeRTOS will mask threshold to RVHAL_EXCM_LEVEL
     * and exit critical code, will recover threshold value (1). so threshold <= 1
     * means not in critical code
     */
    ਵਾਪਸ (ਹੱਦ <= 1);
#ਜੇ_ਅੰਤ
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
#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_attr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_newlib.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_heap_caps.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_rom_sys.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_system.h"             /* required by esp_get_...() functions in portable.h. [refactor-todo] Update portable.h */


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ // __ASSEMBLER__
