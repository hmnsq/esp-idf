/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#ଅନ୍ତର୍ଭାଵ "sdkconfig.h"

/* Macros used instead ofsetoff() for better performance of interrupt handler */
#ଯଦି CONFIG_FREERTOS_USE_LIST_DATA_INTEGRITY_CHECK_BYTES
/*
pxTopOfStack (4) +
xStateListItem (28) +
xEventListItem (28) +
uxPriority (4)
*/
#define PORT_OFFSET_PX_ଗଦା 0x40
#ଅନ୍ୟଥା
/*
pxTopOfStack (4) +
xStateListItem (20) +
xEventListItem (20) +
uxPriority (4)
*/
#define PORT_OFFSET_PX_ଗଦା 0x30
#endif /* #if CONFIG_FREERTOS_USE_LIST_DATA_INTEGRITY_CHECK_BYTES */

#ଯଦି ( config_ସଂଖ୍ୟା_OF_CORES > 1 )
#define PORT_TCB_CORE_FIELDS_SIZE 8
#ଅନ୍ୟଥା
#define PORT_TCB_CORE_FIELDS_SIZE 0
#endif

#ଯଦି ( configUSE_TASK_PREEMPTION_DISABLE == 1 )
#define PORT_TCB_PREEMPT_DISABLE_FIELD_SIZE 4
#ଅନ୍ୟଥା
#define PORT_TCB_PREEMPT_DISABLE_FIELD_SIZE 0
#endif

/* Align the value up to the nearest multiple of 4 */
#define PORT_ALIGN_UP_TO_4(ମୂଲ୍ୟ) (((ମୂଲ୍ୟ) + 3) & ~3)

#define PORT_OFFSET_PX_END_OF_ଗଦା ( \
    PORT_OFFSET_PX_ଗଦା \
    + 4                                 /* StackType_t * pxStack */ \
    + PORT_TCB_CORE_FIELDS_SIZE                     /* BaseType_t xDummy23 + UBaseType_t uxDummy24 */ \
    + PORT_ALIGN_UP_TO_4(configMAX_TASK_NAME_LEN)   /* pcTaskName[ configMAX_TASK_NAME_LEN ] */ \
    + PORT_TCB_PREEMPT_DISABLE_FIELD_SIZE           /* BaseType_t xDummy25 */ \
)

#ifndef __ASSEMBLER__

#ଅନ୍ତର୍ଭାଵ <stdint.h>
#ଅନ୍ତର୍ଭାଵ "spinlock.h"
#ଅନ୍ତର୍ଭାଵ "soc/interrupt_reg.h"
#ଅନ୍ତର୍ଭାଵ "esp_macros.h"
#ଅନ୍ତର୍ଭାଵ "esp_cpu.h"
#ଅନ୍ତର୍ଭାଵ "esp_private/crosscore_int.h"
#ଅନ୍ତର୍ଭାଵ "esp_memory_utils.h"

#ifdef __cplusplus
ବାହ୍ୟ "C" {
#endif

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come first as they are used further down in this file
 * ------------------------------------------------------------------------------------------------------------------ */

#define portCHAR                    uint8_t
#define portFLOAT                   ଦଶମିକ
#define portDOUBLE                  ଦ୍ୱିଗୁଣ
#define portLONG                    int32_t
#define portSHORT                   int16_t
#define port_ଗଦା_TYPE              uint8_t
#define portBASE_TYPE               ପୂର୍ଣ୍ଣସଂଖ୍ୟା

ପ୍ରକାରବ୍ୟାଖ୍ୟା port_ଗଦା_TYPE              ଗଦା_Type_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା portBASE_TYPE               BaseType_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା unsigned portBASE_TYPE      UBaseType_t;

#ଯଦି ( configUSE_16_BIT_TICKS == 1 )
ପ୍ରକାରବ୍ୟାଖ୍ୟା uint16_t TickType_t;
#define portMAX_DELAY ( TickType_t )  0xffff
#ଅନ୍ୟଥା
ପ୍ରକାରବ୍ୟାଖ୍ୟା uint32_t TickType_t;
#define portMAX_DELAY ( TickType_t )  0xffffffffUL
#endif

/* Task function macros as described on the FreeRTOS.org WEB site. */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters )  ଶୂନ୍ୟ vFunction( ଶୂନ୍ୟ *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters )          ଶୂନ୍ୟ vFunction( ଶୂନ୍ୟ *pvParameters )

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#define portCRITICAL_NESTING_IN_TCB     1
#define port_ଗଦା_GROWTH                ( -1 )
#define portTICK_PERIOD_MS              ( ( TickType_t ) ( 1000 / configTICK_RATE_HZ ) )
#define portBYTE_ALIGNMENT              16
#define portNOP() __asm ଅସ୍ଥିର        (" nop ")

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros needed to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

/* ---------------------- Spinlocks ------------------------
 - Spinlocks added to match API with SMP FreeRTOS. Single core RISC-V does not need spin locks
 * ------------------------------------------------------ */

ପ୍ରକାରବ୍ୟାଖ୍ୟା spinlock_t                          portMUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */

#define portMUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#define portMUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#define portMUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#define portMUX_TRY_LOCK                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#define portMUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// --------------------- Interrupts ------------------------

BaseType_t xPortCheckIfInISR(ଶୂନ୍ୟ);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortCheckIfInISR() internally
 */
ଶୂନ୍ୟ vPortAssertIfInISR(ଶୂନ୍ୟ);

// ------------------ Critical Sections --------------------

#ଯଦି ( config_ସଂଖ୍ୟା_OF_CORES > 1 )
/*
These are always called with interrupts already disabled. We simply need to get/release the spinlocks
*/
ବାହ୍ୟ portMUX_TYPE port_xTaskLock;
ବାହ୍ୟ portMUX_TYPE port_xISRLock;

ଶୂନ୍ୟ vPortTakeLock( portMUX_TYPE *lock );
ଶୂନ୍ୟ vPortReleaseLock( portMUX_TYPE *lock );
#endif /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

ଶୂନ୍ୟ vPortYield( ଶୂନ୍ୟ );
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ __attribute__((always_inline)) vPortYieldCore( BaseType_t xCoreID );

/**
 * @brief Set interrupt mask and return current interrupt enable register
 *
 * @return UBaseType_t Current interrupt enable register before set
 */
UBaseType_t ulPortSetInterruptMask(ଶୂନ୍ୟ);

/**
 * @brief Clear current interrupt mask and set given mask
 *
 * @param mask Interrupt mask
 */
ଶୂନ୍ୟ vPortClearInterruptMask(UBaseType_t mask);

/**
 * @brief Perform a context switch from an ISR
 */
ଶୂନ୍ୟ vPortYieldFromISR(ଶୂନ୍ୟ);

#define portYIELD_FROM_ISR_CHECK(x)     ({ \
    ଯଦି ( (x) == pdTRUE ) { \
        vPortYieldFromISR(); \
    } \
})
#define portYIELD_FROM_ISR_NO_CHECK()               vPortYieldFromISR()

// ----------------------- System --------------------------

ସ୍ଥିର ଧାଡ଼ିଭିତରେ BaseType_t __attribute__((always_inline)) xPortGetCoreID( ଶୂନ୍ୟ );

// --------------------- TCB Cleanup -----------------------

/**
 * @brief TCB cleanup hook
 *
 * The portCLEAN_UP_TCB() macro is called in prvDeleteTCB() right before a
 * deleted task's memory is freed. We map that macro to this internal function
 * so that IDF FreeRTOS ports can inject some task pre-deletion operations.
 */
ଶୂନ୍ୟ vPortTCBPreDeleteHook( ଶୂନ୍ୟ *pxTCB );

/* ------------------------------------------- FreeRTOS Porting Interface ----------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS
 * - Most come after forward declare as porting macros map to declared functions
 * - Maps to forward declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

#define portDISABLE_INTERRUPTS()                    ulPortSetInterruptMask()
#ଯଦି !SOC_INT_CLIC_SUPPORTED
#define portENABLE_INTERRUPTS()                     vPortClearInterruptMask(RVHAL_INTR_ENABLE_THRESH)
#ଅନ୍ୟଥା
#define portENABLE_INTERRUPTS()                     vPortClearInterruptMask(RVHAL_INTR_ENABLE_THRESH_CLIC)
#endif /* !SOC_INT_CLIC_SUPPORTED */
#define portRESTORE_INTERRUPTS(x)                   vPortClearInterruptMask(x)

#define portSET_INTERRUPT_MASK_FROM_ISR() ({           \
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cur_level; \
    cur_level = ulPortSetInterruptMask(); \
    cur_level; \
})
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x)        portRESTORE_INTERRUPTS(x)
#define portSET_INTERRUPT_MASK()                    portSET_INTERRUPT_MASK_FROM_ISR()
#define portCLEAR_INTERRUPT_MASK(x)                 portCLEAR_INTERRUPT_MASK_FROM_ISR(x)

/**
 * @brief Assert if in ISR context
 *
 * TODO: Enable once ISR safe version of vTaskEnter/ExitCritical() is implemented
 * for single-core SMP FreeRTOS Kernel. (IDF-10540)
 */
// #define portASSERT_IF_IN_ISR() vPortAssertIfInISR()


// ------------------ Critical Sections --------------------

#ଯଦି ( config_ସଂଖ୍ୟା_OF_CORES > 1 )
#define portGET_TASK_LOCK()                         vPortTakeLock(&port_xTaskLock)
#define portRELEASE_TASK_LOCK()                     vPortReleaseLock(&port_xTaskLock)
#define portGET_ISR_LOCK()                          vPortTakeLock(&port_xISRLock)
#define portRELEASE_ISR_LOCK()                      vPortReleaseLock(&port_xISRLock)
#endif /* configNUMBER_OF_CORES > 1 */

//Critical sections used by FreeRTOS SMP
ବାହ୍ୟ ଶୂନ୍ୟ vTaskEnterCritical( ଶୂନ୍ୟ );
ବାହ୍ୟ ଶୂନ୍ୟ vTaskExitCritical( ଶୂନ୍ୟ );
#define portENTER_CRITICAL_SMP()                    vTaskEnterCritical();
#define portEXIT_CRITICAL_SMP()                     vTaskExitCritical();

#ଯଦି defined(__cplusplus) && (__cplusplus >  201703L)
#define portENTER_CRITICAL(...)                     CHOOSE_MACRO_VA_ARG(portENTER_CRITICAL_IDF, portENTER_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#define portEXIT_CRITICAL(...)                      CHOOSE_MACRO_VA_ARG(portEXIT_CRITICAL_IDF, portEXIT_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#ଅନ୍ୟଥା
#define portENTER_CRITICAL(...)                     CHOOSE_MACRO_VA_ARG(portENTER_CRITICAL_IDF, portENTER_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#define portEXIT_CRITICAL(...)                      CHOOSE_MACRO_VA_ARG(portEXIT_CRITICAL_IDF, portEXIT_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#endif

#define portENTER_CRITICAL_FROM_ISR() vTaskEnterCriticalFromISR()
#define portEXIT_CRITICAL_FROM_ISR(x) vTaskExitCriticalFromISR(x)

// ---------------------- Yielding -------------------------

#define portYIELD()                                 vPortYield()
#ଯଦି defined(__cplusplus) && (__cplusplus >  201703L)
#define portYIELD_FROM_ISR(...)                     CHOOSE_MACRO_VA_ARG(portYIELD_FROM_ISR_CHECK, portYIELD_FROM_ISR_NO_CHECK __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#ଅନ୍ୟଥା
#define portYIELD_FROM_ISR(...)                     CHOOSE_MACRO_VA_ARG(portYIELD_FROM_ISR_CHECK, portYIELD_FROM_ISR_NO_CHECK, ##__VA_ARGS__)(__VA_ARGS__)
#endif
#define portYIELD_CORE(x)                           vPortYieldCore(x)

// ----------------------- System --------------------------

#define portGET_CORE_ID()                           xPortGetCoreID()
#define portCHECK_IF_IN_ISR()                       xPortCheckIfInISR()

// ------------------- Run Time Stats ----------------------

#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()
#ଯଦି ( CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS )
configRUN_TIME_COUNTER_TYPE x_Port_Get_Run_Time_Counter_ମୂଲ୍ୟ( ଶୂନ୍ୟ );
#define port_GET_RUN_TIME_COUNTER_ମୂଲ୍ୟ()        x_Port_Get_Run_Time_Counter_ମୂଲ୍ୟ()
#endif

// --------------------- TCB Cleanup -----------------------

#define portCLEAN_UP_TCB( pxTCB )                   vPortTCBPreDeleteHook( pxTCB )


/* --------------------------------------------- Inline Implementations ------------------------------------------------
 * - Implementation of inline functions of the forward declares
 * - Should come after forward declare and FreeRTOS Porting interface, as implementation may use both.
 * - For implementation of non-inlined functions, see port.c, port_common.c, or other assembly files
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Yielding -------------------------

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ __attribute__((always_inline)) vPortYieldCore( BaseType_t xCoreID )
{
    esp_crosscore_int_send_yield( xCoreID );
}

// ----------------------- System --------------------------

ସ୍ଥିର ଧାଡ଼ିଭିତରେ BaseType_t __attribute__((always_inline)) xPortGetCoreID( ଶୂନ୍ୟ )
{
    ଫେରନ୍ତୁ (BaseType_t) esp_cpu_get_core_id();
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ BaseType_t xPortInIsrContext(ଶୂନ୍ୟ)
{
    //Just call the FreeRTOS port interface version
    ଫେରନ୍ତୁ xPortCheckIfInISR();
}

// Added for backward compatibility with IDF
#define xPortInterruptedFromISRContext()    xPortInIsrContext()

// ------------------ Critical Sections --------------------

/*
IDF style critical sections which are orthogonal to FreeRTOS critical sections. However, on single core, the IDF style
critical sections simply disable interrupts, thus we discard the lock and timeout arguments.

Note: The IDF Style critical sections are named differently to their counterparts present in the xtensa port as few IDF
examples such as esp_zigbee_gateway, when compiled for RISC-V targets, have a reference to vPortEnterCritical()
and vPortExitCritical() from precompiled libraries (.a) thereby failing linking.
*/
ଶୂନ୍ୟ vPortEnterCritical(ଶୂନ୍ୟ);
ଶୂନ୍ୟ vPortExitCritical(ଶୂନ୍ୟ);

#ଯଦି CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
/**
 * @brief Claim thread-safe region start
 *        If claimed, vPortEnterCritical/vPortExitCritical on the current core are no-ops.
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
ଶୂନ୍ୟ xPortThreadSafeClaim(ଶୂନ୍ୟ);

/**
 * @brief Claim thread-safe region end
 *        Restores normal port critical behavior
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
ଶୂନ୍ୟ xPortThreadSafeDisclaim(ଶୂନ୍ୟ);

ବାହ୍ୟ ଅସ୍ଥିର bool port_xThreadSafeClaimed;
#endif /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

//IDF task critical sections
#define portTRY_ENTER_CRITICAL(lock, timeout)       ({(ଶୂନ୍ୟ) lock; (ଶୂନ୍ୟ) timeout; vPortEnterCritical(); pdPASS;})
#define portENTER_CRITICAL_IDF(lock)                ({(ଶୂନ୍ୟ) lock; vPortEnterCritical();})
#define portEXIT_CRITICAL_IDF(lock)                 ({(ଶୂନ୍ୟ) lock; vPortExitCritical();})
//IDF ISR critical sections
#define portTRY_ENTER_CRITICAL_ISR(lock, timeout)   ({(ଶୂନ୍ୟ) lock; (ଶୂନ୍ୟ) timeout; vPortEnterCritical(); pdPASS;})
#define portENTER_CRITICAL_ISR(lock)                ({(ଶୂନ୍ୟ) lock; vPortEnterCritical();})
#define portEXIT_CRITICAL_ISR(lock)                 ({(ଶୂନ୍ୟ) lock; vPortExitCritical();})
//IDF safe critical sections (they're the same)
#define portENTER_CRITICAL_SAFE(lock)               ({(ଶୂନ୍ୟ) lock; vPortEnterCritical();})
#define portEXIT_CRITICAL_SAFE(lock)                ({(ଶୂନ୍ୟ) lock; vPortExitCritical();})

// ---------------------- Yielding -------------------------

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool xPortCanYield(ଶୂନ୍ୟ)
{
#ଯଦି SOC_INT_CLIC_SUPPORTED
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
    ଫେରନ୍ତୁ ((intr_level == 0) && (threshold == 0));
#ଅନ୍ୟଥା/* !SOC_INT_CLIC_SUPPORTED */
    uint32_t threshold = REG_READ(INTERRUPT_CURRENT_CORE_INT_THRESH_REG);
    /* when enter critical code, FreeRTOS will mask threshold to RVHAL_EXCM_LEVEL
     * and exit critical code, will recover threshold value (1). so threshold <= 1
     * means not in critical code
     */
    ଫେରନ୍ତୁ (threshold <= 1);
#endif
}

// Defined even for configNUMBER_OF_CORES > 1 for IDF compatibility
#define portYIELD_WITHIN_API()                      esp_crosscore_int_send_yield(xPortGetCoreID())

// ----------------------- System --------------------------

ଶୂନ୍ୟ v_Port_Set_ଗଦା_Watchpoint(ଶୂନ୍ୟ *px_ଗଦା_Start);

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
bool xPortCheckValidListMem(ଅଚଳ ଶୂନ୍ୟ *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool xPortCheckValidTCBMem(ଅଚଳ ଶୂନ୍ୟ *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_ଗଦା_Mem(ଅଚଳ ଶୂନ୍ୟ *ptr);

#define portVALID_LIST_MEM(ptr)     xPortCheckValidListMem(ptr)
#define portVALID_TCB_MEM(ptr)      xPortCheckValidTCBMem(ptr)
#define port_VALID_ଗଦା_MEM(ptr)    x_Portcheck_Valid_ଗଦା_Mem(ptr)

/* ------------------------------------------------------ Misc ---------------------------------------------------------
 * - Miscellaneous porting macros
 * - These are not part of the FreeRTOS porting interface, but are used by other FreeRTOS dependent components
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- App-Trace -------------------------

#ଯଦି CONFIG_ESP_TRACE_ENABLE
ବାହ୍ୟ ଅସ୍ଥିର BaseType_t xPortSwitchFlag;
#define os_task_switch_is_pended(_cpu_) (xPortSwitchFlag)
#ଅନ୍ୟଥା
#define os_task_switch_is_pended(_cpu_) (false)
#endif

// --------------- Compatibility Includes ------------------
/*
ESP-IDF currently does not have a "Include what you use" policy. A lot of files implicitly pull in API through
portmacro.h. Therefore, we need to keep these headers around for now to allow the rest of IDF to compile.

[refactor-todo] Clean up ESP-IDF inclusion dependencies and add a inclusion check.
*/

#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <stdbool.h>
#ଅନ୍ତର୍ଭାଵ <stdarg.h>
#ଅନ୍ତର୍ଭାଵ <limits.h>
#ଅନ୍ତର୍ଭାଵ "esp_attr.h"
#ଅନ୍ତର୍ଭାଵ "esp_newlib.h"
#ଅନ୍ତର୍ଭାଵ "esp_heap_caps.h"
#ଅନ୍ତର୍ଭାଵ "esp_rom_sys.h"
#ଅନ୍ତର୍ଭାଵ "esp_system.h"             /* required by esp_get_...() functions in portable.h. [refactor-todo] Update portable.h */


#ifdef __cplusplus
}
#endif

#endif // __ASSEMBLER__
