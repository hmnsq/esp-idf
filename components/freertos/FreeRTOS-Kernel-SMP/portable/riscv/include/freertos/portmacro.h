/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#அந்தர்பாவ "sdkconfig.h"

/* Macros used instead ofsetoff() for better performance of interrupt handler */
#எனில் CONFIG_FREERTOS_USE_LIST_DATA_INTEGRITY_CHECK_BYTES
/*
pxTopOfStack (4) +
xStateListItem (28) +
xEventListItem (28) +
uxPriority (4)
*/
#define PORT_OFFSET_PX_அடுக்கு 0x40
#இல்லையெனில்
/*
pxTopOfStack (4) +
xStateListItem (20) +
xEventListItem (20) +
uxPriority (4)
*/
#define PORT_OFFSET_PX_அடுக்கு 0x30
#endif /* #if CONFIG_FREERTOS_USE_LIST_DATA_INTEGRITY_CHECK_BYTES */

#எனில் ( config_எண்_OF_CORES > 1 )
#define PORT_TCB_CORE_FIELDS_SIZE 8
#இல்லையெனில்
#define PORT_TCB_CORE_FIELDS_SIZE 0
#endif

#எனில் ( configUSE_TASK_PREEMPTION_DISABLE == 1 )
#define PORT_TCB_PREEMPT_DISABLE_FIELD_SIZE 4
#இல்லையெனில்
#define PORT_TCB_PREEMPT_DISABLE_FIELD_SIZE 0
#endif

/* Align the value up to the nearest multiple of 4 */
#define PORT_ALIGN_UP_TO_4(மதிப்பு) (((மதிப்பு) + 3) & ~3)

#define PORT_OFFSET_PX_END_OF_அடுக்கு ( \
    PORT_OFFSET_PX_அடுக்கு \
    + 4                                 /* StackType_t * pxStack */ \
    + PORT_TCB_CORE_FIELDS_SIZE                     /* BaseType_t xDummy23 + UBaseType_t uxDummy24 */ \
    + PORT_ALIGN_UP_TO_4(configMAX_TASK_NAME_LEN)   /* pcTaskName[ configMAX_TASK_NAME_LEN ] */ \
    + PORT_TCB_PREEMPT_DISABLE_FIELD_SIZE           /* BaseType_t xDummy25 */ \
)

#ifndef __ASSEMBLER__

#அந்தர்பாவ <stdint.h>
#அந்தர்பாவ "spinlock.h"
#அந்தர்பாவ "soc/interrupt_reg.h"
#அந்தர்பாவ "esp_macros.h"
#அந்தர்பாவ "esp_cpu.h"
#அந்தர்பாவ "esp_private/crosscore_int.h"
#அந்தர்பாவ "esp_memory_utils.h"

#ifdef __cplusplus
வெளி "C" {
#endif

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come first as they are used further down in this file
 * ------------------------------------------------------------------------------------------------------------------ */

#define portCHAR                    uint8_t
#define portFLOAT                   தசம
#define portDOUBLE                  இரட்டை
#define portLONG                    int32_t
#define portSHORT                   int16_t
#define port_அடுக்கு_TYPE              uint8_t
#define portBASE_TYPE               முழு

வகைவரை port_அடுக்கு_TYPE              அடுக்கு_Type_t;
வகைவரை portBASE_TYPE               BaseType_t;
வகைவரை unsigned portBASE_TYPE      UBaseType_t;

#எனில் ( configUSE_16_BIT_TICKS == 1 )
வகைவரை uint16_t TickType_t;
#define portMAX_DELAY ( TickType_t )  0xffff
#இல்லையெனில்
வகைவரை uint32_t TickType_t;
#define portMAX_DELAY ( TickType_t )  0xffffffffUL
#endif

/* Task function macros as described on the FreeRTOS.org WEB site. */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters )  வெறுமை vFunction( வெறுமை *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters )          வெறுமை vFunction( வெறுமை *pvParameters )

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#define portCRITICAL_NESTING_IN_TCB     1
#define port_அடுக்கு_GROWTH                ( -1 )
#define portTICK_PERIOD_MS              ( ( TickType_t ) ( 1000 / configTICK_RATE_HZ ) )
#define portBYTE_ALIGNMENT              16
#define portNOP() __asm நிலையற்ற        (" nop ")

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros needed to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

/* ---------------------- Spinlocks ------------------------
 - Spinlocks added to match API with SMP FreeRTOS. Single core RISC-V does not need spin locks
 * ------------------------------------------------------ */

வகைவரை spinlock_t                          portMUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */

#define portMUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#define portMUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#define portMUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#define portMUX_TRY_LOCK                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#define portMUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// --------------------- Interrupts ------------------------

BaseType_t xPortCheckIfInISR(வெறுமை);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortCheckIfInISR() internally
 */
வெறுமை vPortAssertIfInISR(வெறுமை);

// ------------------ Critical Sections --------------------

#எனில் ( config_எண்_OF_CORES > 1 )
/*
These are always called with interrupts already disabled. We simply need to get/release the spinlocks
*/
வெளி portMUX_TYPE port_xTaskLock;
வெளி portMUX_TYPE port_xISRLock;

வெறுமை vPortTakeLock( portMUX_TYPE *lock );
வெறுமை vPortReleaseLock( portMUX_TYPE *lock );
#endif /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

வெறுமை vPortYield( வெறுமை );
நிலையான உள்வரி வெறுமை __attribute__((always_inline)) vPortYieldCore( BaseType_t xCoreID );

/**
 * @brief Set interrupt mask and return current interrupt enable register
 *
 * @return UBaseType_t Current interrupt enable register before set
 */
UBaseType_t ulPortSetInterruptMask(வெறுமை);

/**
 * @brief Clear current interrupt mask and set given mask
 *
 * @param mask Interrupt mask
 */
வெறுமை vPortClearInterruptMask(UBaseType_t mask);

/**
 * @brief Perform a context switch from an ISR
 */
வெறுமை vPortYieldFromISR(வெறுமை);

#define portYIELD_FROM_ISR_CHECK(x)     ({ \
    எனில் ( (x) == pdTRUE ) { \
        vPortYieldFromISR(); \
    } \
})
#define portYIELD_FROM_ISR_NO_CHECK()               vPortYieldFromISR()

// ----------------------- System --------------------------

நிலையான உள்வரி BaseType_t __attribute__((always_inline)) xPortGetCoreID( வெறுமை );

// --------------------- TCB Cleanup -----------------------

/**
 * @brief TCB cleanup hook
 *
 * The portCLEAN_UP_TCB() macro is called in prvDeleteTCB() right before a
 * deleted task's memory is freed. We map that macro to this internal function
 * so that IDF FreeRTOS ports can inject some task pre-deletion operations.
 */
வெறுமை vPortTCBPreDeleteHook( வெறுமை *pxTCB );

/* ------------------------------------------- FreeRTOS Porting Interface ----------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS
 * - Most come after forward declare as porting macros map to declared functions
 * - Maps to forward declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

#define portDISABLE_INTERRUPTS()                    ulPortSetInterruptMask()
#எனில் !SOC_INT_CLIC_SUPPORTED
#define portENABLE_INTERRUPTS()                     vPortClearInterruptMask(RVHAL_INTR_ENABLE_THRESH)
#இல்லையெனில்
#define portENABLE_INTERRUPTS()                     vPortClearInterruptMask(RVHAL_INTR_ENABLE_THRESH_CLIC)
#endif /* !SOC_INT_CLIC_SUPPORTED */
#define portRESTORE_INTERRUPTS(x)                   vPortClearInterruptMask(x)

#define portSET_INTERRUPT_MASK_FROM_ISR() ({           \
    unsigned முழு cur_level; \
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

#எனில் ( config_எண்_OF_CORES > 1 )
#define portGET_TASK_LOCK()                         vPortTakeLock(&port_xTaskLock)
#define portRELEASE_TASK_LOCK()                     vPortReleaseLock(&port_xTaskLock)
#define portGET_ISR_LOCK()                          vPortTakeLock(&port_xISRLock)
#define portRELEASE_ISR_LOCK()                      vPortReleaseLock(&port_xISRLock)
#endif /* configNUMBER_OF_CORES > 1 */

//Critical sections used by FreeRTOS SMP
வெளி வெறுமை vTaskEnterCritical( வெறுமை );
வெளி வெறுமை vTaskExitCritical( வெறுமை );
#define portENTER_CRITICAL_SMP()                    vTaskEnterCritical();
#define portEXIT_CRITICAL_SMP()                     vTaskExitCritical();

#எனில் defined(__cplusplus) && (__cplusplus >  201703L)
#define portENTER_CRITICAL(...)                     CHOOSE_MACRO_VA_ARG(portENTER_CRITICAL_IDF, portENTER_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#define portEXIT_CRITICAL(...)                      CHOOSE_MACRO_VA_ARG(portEXIT_CRITICAL_IDF, portEXIT_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#இல்லையெனில்
#define portENTER_CRITICAL(...)                     CHOOSE_MACRO_VA_ARG(portENTER_CRITICAL_IDF, portENTER_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#define portEXIT_CRITICAL(...)                      CHOOSE_MACRO_VA_ARG(portEXIT_CRITICAL_IDF, portEXIT_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#endif

#define portENTER_CRITICAL_FROM_ISR() vTaskEnterCriticalFromISR()
#define portEXIT_CRITICAL_FROM_ISR(x) vTaskExitCriticalFromISR(x)

// ---------------------- Yielding -------------------------

#define portYIELD()                                 vPortYield()
#எனில் defined(__cplusplus) && (__cplusplus >  201703L)
#define portYIELD_FROM_ISR(...)                     CHOOSE_MACRO_VA_ARG(portYIELD_FROM_ISR_CHECK, portYIELD_FROM_ISR_NO_CHECK __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#இல்லையெனில்
#define portYIELD_FROM_ISR(...)                     CHOOSE_MACRO_VA_ARG(portYIELD_FROM_ISR_CHECK, portYIELD_FROM_ISR_NO_CHECK, ##__VA_ARGS__)(__VA_ARGS__)
#endif
#define portYIELD_CORE(x)                           vPortYieldCore(x)

// ----------------------- System --------------------------

#define portGET_CORE_ID()                           xPortGetCoreID()
#define portCHECK_IF_IN_ISR()                       xPortCheckIfInISR()

// ------------------- Run Time Stats ----------------------

#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()
#எனில் ( CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS )
configRUN_TIME_COUNTER_TYPE x_Port_Get_Run_Time_Counter_மதிப்பு( வெறுமை );
#define port_GET_RUN_TIME_COUNTER_மதிப்பு()        x_Port_Get_Run_Time_Counter_மதிப்பு()
#endif

// --------------------- TCB Cleanup -----------------------

#define portCLEAN_UP_TCB( pxTCB )                   vPortTCBPreDeleteHook( pxTCB )


/* --------------------------------------------- Inline Implementations ------------------------------------------------
 * - Implementation of inline functions of the forward declares
 * - Should come after forward declare and FreeRTOS Porting interface, as implementation may use both.
 * - For implementation of non-inlined functions, see port.c, port_common.c, or other assembly files
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Yielding -------------------------

நிலையான உள்வரி வெறுமை __attribute__((always_inline)) vPortYieldCore( BaseType_t xCoreID )
{
    esp_crosscore_int_send_yield( xCoreID );
}

// ----------------------- System --------------------------

நிலையான உள்வரி BaseType_t __attribute__((always_inline)) xPortGetCoreID( வெறுமை )
{
    திரும்பு (BaseType_t) esp_cpu_get_core_id();
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

நிலையான உள்வரி BaseType_t xPortInIsrContext(வெறுமை)
{
    //Just call the FreeRTOS port interface version
    திரும்பு xPortCheckIfInISR();
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
வெறுமை vPortEnterCritical(வெறுமை);
வெறுமை vPortExitCritical(வெறுமை);

#எனில் CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
/**
 * @brief Claim thread-safe region start
 *        If claimed, vPortEnterCritical/vPortExitCritical on the current core are no-ops.
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
வெறுமை xPortThreadSafeClaim(வெறுமை);

/**
 * @brief Claim thread-safe region end
 *        Restores normal port critical behavior
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
வெறுமை xPortThreadSafeDisclaim(வெறுமை);

வெளி நிலையற்ற bool port_xThreadSafeClaimed;
#endif /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

//IDF task critical sections
#define portTRY_ENTER_CRITICAL(lock, timeout)       ({(வெறுமை) lock; (வெறுமை) timeout; vPortEnterCritical(); pdPASS;})
#define portENTER_CRITICAL_IDF(lock)                ({(வெறுமை) lock; vPortEnterCritical();})
#define portEXIT_CRITICAL_IDF(lock)                 ({(வெறுமை) lock; vPortExitCritical();})
//IDF ISR critical sections
#define portTRY_ENTER_CRITICAL_ISR(lock, timeout)   ({(வெறுமை) lock; (வெறுமை) timeout; vPortEnterCritical(); pdPASS;})
#define portENTER_CRITICAL_ISR(lock)                ({(வெறுமை) lock; vPortEnterCritical();})
#define portEXIT_CRITICAL_ISR(lock)                 ({(வெறுமை) lock; vPortExitCritical();})
//IDF safe critical sections (they're the same)
#define portENTER_CRITICAL_SAFE(lock)               ({(வெறுமை) lock; vPortEnterCritical();})
#define portEXIT_CRITICAL_SAFE(lock)                ({(வெறுமை) lock; vPortExitCritical();})

// ---------------------- Yielding -------------------------

நிலையான உள்வரி bool xPortCanYield(வெறுமை)
{
#எனில் SOC_INT_CLIC_SUPPORTED
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
    திரும்பு ((intr_level == 0) && (threshold == 0));
#இல்லையெனில்/* !SOC_INT_CLIC_SUPPORTED */
    uint32_t threshold = REG_READ(INTERRUPT_CURRENT_CORE_INT_THRESH_REG);
    /* when enter critical code, FreeRTOS will mask threshold to RVHAL_EXCM_LEVEL
     * and exit critical code, will recover threshold value (1). so threshold <= 1
     * means not in critical code
     */
    திரும்பு (threshold <= 1);
#endif
}

// Defined even for configNUMBER_OF_CORES > 1 for IDF compatibility
#define portYIELD_WITHIN_API()                      esp_crosscore_int_send_yield(xPortGetCoreID())

// ----------------------- System --------------------------

வெறுமை v_Port_Set_அடுக்கு_Watchpoint(வெறுமை *px_அடுக்கு_Start);

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
bool xPortCheckValidListMem(மாறிலி வெறுமை *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool xPortCheckValidTCBMem(மாறிலி வெறுமை *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_அடுக்கு_Mem(மாறிலி வெறுமை *ptr);

#define portVALID_LIST_MEM(ptr)     xPortCheckValidListMem(ptr)
#define portVALID_TCB_MEM(ptr)      xPortCheckValidTCBMem(ptr)
#define port_VALID_அடுக்கு_MEM(ptr)    x_Portcheck_Valid_அடுக்கு_Mem(ptr)

/* ------------------------------------------------------ Misc ---------------------------------------------------------
 * - Miscellaneous porting macros
 * - These are not part of the FreeRTOS porting interface, but are used by other FreeRTOS dependent components
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- App-Trace -------------------------

#எனில் CONFIG_ESP_TRACE_ENABLE
வெளி நிலையற்ற BaseType_t xPortSwitchFlag;
#define os_task_switch_is_pended(_cpu_) (xPortSwitchFlag)
#இல்லையெனில்
#define os_task_switch_is_pended(_cpu_) (false)
#endif

// --------------- Compatibility Includes ------------------
/*
ESP-IDF currently does not have a "Include what you use" policy. A lot of files implicitly pull in API through
portmacro.h. Therefore, we need to keep these headers around for now to allow the rest of IDF to compile.

[refactor-todo] Clean up ESP-IDF inclusion dependencies and add a inclusion check.
*/

#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <stdbool.h>
#அந்தர்பாவ <stdarg.h>
#அந்தர்பாவ <limits.h>
#அந்தர்பாவ "esp_attr.h"
#அந்தர்பாவ "esp_newlib.h"
#அந்தர்பாவ "esp_heap_caps.h"
#அந்தர்பாவ "esp_rom_sys.h"
#அந்தர்பாவ "esp_system.h"             /* required by esp_get_...() functions in portable.h. [refactor-todo] Update portable.h */


#ifdef __cplusplus
}
#endif

#endif // __ASSEMBLER__
