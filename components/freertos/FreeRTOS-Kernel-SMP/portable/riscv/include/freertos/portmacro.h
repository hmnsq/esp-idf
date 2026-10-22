/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"

/* Macros used instead ofsetoff() for better performance of interrupt handler */
#ಆದರೆ CONFIG_FREERTOS_USE_LIST_ದತ್ತಾಂಶ_INTEGRITY_CHECK_ಬೈಟುಗಳು
/*
pxTopOfStack (4) +
xStateListItem (28) +
xEventListItem (28) +
uxPriority (4)
*/
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ಸ್ಥಾನಾಂತರ_PX_ರಾಶಿ 0x40
#ಇಲ್ಲದಿದ್ದರೆ
/*
pxTopOfStack (4) +
xStateListItem (20) +
xEventListItem (20) +
uxPriority (4)
*/
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ಸ್ಥಾನಾಂತರ_PX_ರಾಶಿ 0x30
#ಆದರೆ_ಅಂತ್ಯ /* #if CONFIG_FREERTOS_USE_LIST_DATA_INTEGRITY_CHECK_BYTES */

#ಆದರೆ ( config_ಸಂಖ್ಯೆ_ದ_CORES > 1 )
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TCB_CORE_FIELDS_ಗಾತ್ರ 8
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TCB_CORE_FIELDS_ಗಾತ್ರ 0
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ( configUSE_TASK_PREEMPTION_DISABLE == 1 )
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TCB_PREEMPT_DISABLE_ಕ್ಷೇತ್ರ_ಗಾತ್ರ 4
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TCB_PREEMPT_DISABLE_ಕ್ಷೇತ್ರ_ಗಾತ್ರ 0
#ಆದರೆ_ಅಂತ್ಯ

/* Align the value up to the nearest multiple of 4 */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ALIGN_ಮೇಲೆ_ಗಮ್ಯ_4(ಮೌಲ್ಯ) (((ಮೌಲ್ಯ) + 3) & ~3)

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ಸ್ಥಾನಾಂತರ_PX_ಕೊನೆ_ದ_ರಾಶಿ ( \
    ಪೋರ್ಟ್_ಸ್ಥಾನಾಂತರ_PX_ರಾಶಿ \
    + 4                                 /* StackType_t * pxStack */ \
    + ಪೋರ್ಟ್_TCB_CORE_FIELDS_ಗಾತ್ರ                     /* BaseType_t xDummy23 + UBaseType_t uxDummy24 */ \
    + ಪೋರ್ಟ್_ALIGN_ಮೇಲೆ_ಗಮ್ಯ_4(configMAX_TASK_NAME_LEN)   /* pcTaskName[ configMAX_TASK_NAME_LEN ] */ \
    + ಪೋರ್ಟ್_TCB_PREEMPT_DISABLE_ಕ್ಷೇತ್ರ_ಗಾತ್ರ           /* BaseType_t xDummy25 */ \
)

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __ASSEMBLER__

#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ "spinlock.h"
#ಅನ್ತರ್ಭಾವ "soc/interrupt_reg.h"
#ಅನ್ತರ್ಭಾವ "esp_macros.h"
#ಅನ್ತರ್ಭಾವ "esp_cpu.h"
#ಅನ್ತರ್ಭಾವ "esp_private/crosscore_int.h"
#ಅನ್ತರ್ಭಾವ "esp_memory_utils.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
ಬಾಹ್ಯ "C" {
#ಆದರೆ_ಅಂತ್ಯ

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come first as they are used further down in this file
 * ------------------------------------------------------------------------------------------------------------------ */

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_CHAR                    uint8_t
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_FLOAT                   ದಶಮಾಂಶ
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_DOUBLE                  ದ್ವಿಗುಣ
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_LONG                    int32_t
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_SHORT                   int16_t
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ರಾಶಿ_TYPE              uint8_t
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ತಳ_TYPE               ಪೂರ್ಣಾಂಕ

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೋರ್ಟ್_ರಾಶಿ_TYPE              ರಾಶಿ_Type_t;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೋರ್ಟ್_ತಳ_TYPE               ತಳ_Type_t;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಪೋರ್ಟ್_ತಳ_TYPE      UBaseType_t;

#ಆದರೆ ( config_USE_16_ಬಿಟ್_TICKS == 1 )
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ uint16_t TickType_t;
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_MAX_DELAY ( TickType_t )  0xffff
#ಇಲ್ಲದಿದ್ದರೆ
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ uint32_t TickType_t;
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_MAX_DELAY ( TickType_t )  0xffffffffUL
#ಆದರೆ_ಅಂತ್ಯ

/* Task function macros as described on the FreeRTOS.org WEB site. */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TASK_FUNCTION_PROTO( vFunction, pvParameters )  ಶೂನ್ಯ vFunction( ಶೂನ್ಯ *pvParameters )
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TASK_FUNCTION( vFunction, pvParameters )          ಶೂನ್ಯ vFunction( ಶೂನ್ಯ *pvParameters )

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_CRITICAL_NESTING_ಒಳ_TCB     1
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ರಾಶಿ_GROWTH                ( -1 )
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TICK_PERIOD_MS              ( ( TickType_t ) ( 1000 / config_TICK_ದರ_HZ ) )
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_BYTE_ALIGNMENT              16
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_NOP() __asm ಅಸ್ಥಿರ        (" nop ")

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros needed to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

/* ---------------------- Spinlocks ------------------------
 - Spinlocks added to match API with SMP FreeRTOS. Single core RISC-V does not need spin locks
 * ------------------------------------------------------ */

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ spinlock_t                          ಪೋರ್ಟ್_MUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_MUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_MUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_MUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_MUX_TRY_ಬೀಗ                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// --------------------- Interrupts ------------------------

ತಳ_Type_t x_ಪೋರ್ಟ್_Check_If_ಒಳ_ISR(ಶೂನ್ಯ);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortCheckIfInISR() internally
 */
ಶೂನ್ಯ v_ಪೋರ್ಟ್_Assert_If_ಒಳ_ISR(ಶೂನ್ಯ);

// ------------------ Critical Sections --------------------

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

/**
 * @brief Set interrupt mask and return current interrupt enable register
 *
 * @return UBaseType_t Current interrupt enable register before set
 */
UBaseType_t ul_ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ(ಶೂನ್ಯ);

/**
 * @brief Clear current interrupt mask and set given mask
 *
 * @param mask Interrupt mask
 */
ಶೂನ್ಯ v_ಪೋರ್ಟ್_Clear_ಅಡಚಣೆ_ಮುಖವಾಡ(UBaseType_t ಮುಖವಾಡ);

/**
 * @brief Perform a context switch from an ISR
 */
ಶೂನ್ಯ v_ಪೋರ್ಟ್_Yield_From_ISR(ಶೂನ್ಯ);

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

/* ------------------------------------------- FreeRTOS Porting Interface ----------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS
 * - Most come after forward declare as porting macros map to declared functions
 * - Maps to forward declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_DISABLE_INTERRUPTS()                    ul_ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ()
#ಆದರೆ !SOC_INT_CLIC_SUPPORTED
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ENABLE_INTERRUPTS()                     v_ಪೋರ್ಟ್_Clear_ಅಡಚಣೆ_ಮುಖವಾಡ(RVHAL_INTR_ENABLE_THRESH)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ENABLE_INTERRUPTS()                     v_ಪೋರ್ಟ್_Clear_ಅಡಚಣೆ_ಮುಖವಾಡ(RVHAL_INTR_ENABLE_THRESH_CLIC)
#ಆದರೆ_ಅಂತ್ಯ /* !SOC_INT_CLIC_SUPPORTED */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_RESTORE_INTERRUPTS(x)                   v_ಪೋರ್ಟ್_Clear_ಅಡಚಣೆ_ಮುಖವಾಡ(x)

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR() ({           \
    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cur_level; \
    cur_level = ul_ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ(); \
    cur_level; \
})
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_CLEAR_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR(x)        ಪೋರ್ಟ್_RESTORE_INTERRUPTS(x)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ()                    ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR()
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_CLEAR_ಅಡಚಣೆ_ಮುಖವಾಡ(x)                 ಪೋರ್ಟ್_CLEAR_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR(x)

/**
 * @brief Assert if in ISR context
 *
 * TODO: Enable once ISR safe version of vTaskEnter/ExitCritical() is implemented
 * for single-core SMP FreeRTOS Kernel. (IDF-10540)
 */
// #define portASSERT_IF_IN_ISR() vPortAssertIfInISR()


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
 * - For implementation of non-inlined functions, see port.c, port_common.c, or other assembly files
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Yielding -------------------------

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ __attribute__((always_inline)) v_ಪೋರ್ಟ್_Yield_Core( ತಳ_Type_t xCoreID )
{
    esp_crosscore_int_send_yield( xCoreID );
}

// ----------------------- System --------------------------

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಳ_Type_t __attribute__((always_inline)) x_ಪೋರ್ಟ್_Get_Core_ID( ಶೂನ್ಯ )
{
    ಹಿಂತಿರುಗಿ (ತಳ_Type_t) esp_cpu_get_core_id();
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಳ_Type_t x_ಪೋರ್ಟ್_ಒಳ_Isr_Context(ಶೂನ್ಯ)
{
    //Just call the FreeRTOS port interface version
    ಹಿಂತಿರುಗಿ x_ಪೋರ್ಟ್_Check_If_ಒಳ_ISR();
}

// Added for backward compatibility with IDF
#ವ್ಯಾಖ್ಯಾನಿಸು x_ಪೋರ್ಟ್_Interrupted_From_ISRContext()    x_ಪೋರ್ಟ್_ಒಳ_Isr_Context()

// ------------------ Critical Sections --------------------

/*
IDF style critical sections which are orthogonal to FreeRTOS critical sections. However, on single core, the IDF style
critical sections simply disable interrupts, thus we discard the lock and timeout arguments.

Note: The IDF Style critical sections are named differently to their counterparts present in the xtensa port as few IDF
examples such as esp_zigbee_gateway, when compiled for RISC-V targets, have a reference to vPortEnterCritical()
and vPortExitCritical() from precompiled libraries (.a) thereby failing linking.
*/
ಶೂನ್ಯ v_ಪೋರ್ಟ್_Enter_Critical(ಶೂನ್ಯ);
ಶೂನ್ಯ v_ಪೋರ್ಟ್_Exit_Critical(ಶೂನ್ಯ);

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

//IDF task critical sections
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TRY_ENTER_CRITICAL(ಬೀಗ_2, timeout)       ({(ಶೂನ್ಯ) ಬೀಗ_2; (ಶೂನ್ಯ) timeout; v_ಪೋರ್ಟ್_Enter_Critical(); pdPASS;})
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ENTER_CRITICAL_IDF(ಬೀಗ_2)                ({(ಶೂನ್ಯ) ಬೀಗ_2; v_ಪೋರ್ಟ್_Enter_Critical();})
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_EXIT_CRITICAL_IDF(ಬೀಗ_2)                 ({(ಶೂನ್ಯ) ಬೀಗ_2; v_ಪೋರ್ಟ್_Exit_Critical();})
//IDF ISR critical sections
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TRY_ENTER_CRITICAL_ISR(ಬೀಗ_2, timeout)   ({(ಶೂನ್ಯ) ಬೀಗ_2; (ಶೂನ್ಯ) timeout; v_ಪೋರ್ಟ್_Enter_Critical(); pdPASS;})
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ENTER_CRITICAL_ISR(ಬೀಗ_2)                ({(ಶೂನ್ಯ) ಬೀಗ_2; v_ಪೋರ್ಟ್_Enter_Critical();})
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_EXIT_CRITICAL_ISR(ಬೀಗ_2)                 ({(ಶೂನ್ಯ) ಬೀಗ_2; v_ಪೋರ್ಟ್_Exit_Critical();})
//IDF safe critical sections (they're the same)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ENTER_CRITICAL_ಸುರಕ್ಷಿತ(ಬೀಗ_2)               ({(ಶೂನ್ಯ) ಬೀಗ_2; v_ಪೋರ್ಟ್_Enter_Critical();})
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_EXIT_CRITICAL_ಸುರಕ್ಷಿತ(ಬೀಗ_2)                ({(ಶೂನ್ಯ) ಬೀಗ_2; v_ಪೋರ್ಟ್_Exit_Critical();})

// ---------------------- Yielding -------------------------

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool x_ಪೋರ್ಟ್_Can_Yield(ಶೂನ್ಯ)
{
#ಆದರೆ SOC_INT_CLIC_SUPPORTED
    /* When CLIC is supported:
     *  - The lowest interrupt threshold level is 0. Therefore, an interrupt threshold level above 0 would mean that we
     *    are in a critical section.
     *  - Since CLIC enables HW interrupt nesting, we do not have the updated interrupt level in the
     *    INTERRUPT_CURRENT_CORE_INT_THRESH_REG register when nested interrupts occur. To know the current interrupt
     *    level, we read the machine-mode interrupt level (mil) field from the mintstatus CSR. A non-zero value indicates
     *    that we are in an interrupt context.
     */
    uint32_t ಮಿತಿ = rv_utils_get_ಅಡಚಣೆ_ಮಿತಿ();
    uint32_t intr_level = rv_utils_get_ಅಡಚಣೆ_level();
    ಹಿಂತಿರುಗಿ ((intr_level == 0) && (ಮಿತಿ == 0));
#ಇಲ್ಲದಿದ್ದರೆ/* !SOC_INT_CLIC_SUPPORTED */
    uint32_t ಮಿತಿ = REG_READ(ಅಡಚಣೆ_CURRENT_CORE_INT_THRESH_REG);
    /* when enter critical code, FreeRTOS will mask threshold to RVHAL_EXCM_LEVEL
     * and exit critical code, will recover threshold value (1). so threshold <= 1
     * means not in critical code
     */
    ಹಿಂತಿರುಗಿ (ಮಿತಿ <= 1);
#ಆದರೆ_ಅಂತ್ಯ
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
#ಅನ್ತರ್ಭಾವ <limits.h>
#ಅನ್ತರ್ಭಾವ "esp_attr.h"
#ಅನ್ತರ್ಭಾವ "esp_newlib.h"
#ಅನ್ತರ್ಭಾವ "esp_heap_caps.h"
#ಅನ್ತರ್ಭಾವ "esp_rom_sys.h"
#ಅನ್ತರ್ಭಾವ "esp_system.h"             /* required by esp_get_...() functions in portable.h. [refactor-todo] Update portable.h */


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ // __ASSEMBLER__
