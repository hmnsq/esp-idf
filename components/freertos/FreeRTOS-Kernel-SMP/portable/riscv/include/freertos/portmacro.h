/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#ضمن "sdkconfig.h"

/* Macros used instead ofsetoff() for better performance of interrupt handler */
#إذا CONFIG_FREERTOS_USE_LIST_DATA_INTEGRITY_CHECK_BYTES
/*
pxTopOfStack (4) +
xStateListItem (28) +
xEventListItem (28) +
uxPriority (4)
*/
#define PORT_OFFSET_PX_دۆۋە 0x40
#وإلا
/*
pxTopOfStack (4) +
xStateListItem (20) +
xEventListItem (20) +
uxPriority (4)
*/
#define PORT_OFFSET_PX_دۆۋە 0x30
#endif /* #if CONFIG_FREERTOS_USE_LIST_DATA_INTEGRITY_CHECK_BYTES */

#إذا ( config_سان_OF_CORES > 1 )
#define PORT_TCB_CORE_FIELDS_SIZE 8
#وإلا
#define PORT_TCB_CORE_FIELDS_SIZE 0
#endif

#إذا ( configUSE_TASK_PREEMPTION_DISABLE == 1 )
#define PORT_TCB_PREEMPT_DISABLE_FIELD_SIZE 4
#وإلا
#define PORT_TCB_PREEMPT_DISABLE_FIELD_SIZE 0
#endif

/* Align the value up to the nearest multiple of 4 */
#define PORT_ALIGN_UP_TO_4(قىممەت) (((قىممەت) + 3) & ~3)

#define PORT_OFFSET_PX_END_OF_دۆۋە ( \
    PORT_OFFSET_PX_دۆۋە \
    + 4                                 /* StackType_t * pxStack */ \
    + PORT_TCB_CORE_FIELDS_SIZE                     /* BaseType_t xDummy23 + UBaseType_t uxDummy24 */ \
    + PORT_ALIGN_UP_TO_4(configMAX_TASK_NAME_LEN)   /* pcTaskName[ configMAX_TASK_NAME_LEN ] */ \
    + PORT_TCB_PREEMPT_DISABLE_FIELD_SIZE           /* BaseType_t xDummy25 */ \
)

#ifndef __ASSEMBLER__

#ضمن <stdint.h>
#ضمن "spinlock.h"
#ضمن "soc/interrupt_reg.h"
#ضمن "esp_macros.h"
#ضمن "esp_cpu.h"
#ضمن "esp_private/crosscore_int.h"
#ضمن "esp_memory_utils.h"

#ifdef __cplusplus
خارجي "C" {
#endif

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come first as they are used further down in this file
 * ------------------------------------------------------------------------------------------------------------------ */

#define portCHAR                    uint8_t
#define portFLOAT                   عائم
#define portDOUBLE                  مزدوج
#define portLONG                    int32_t
#define portSHORT                   int16_t
#define port_دۆۋە_TYPE              uint8_t
#define portBASE_TYPE               صحيح

تعريفنوع port_دۆۋە_TYPE              دۆۋە_Type_t;
تعريفنوع portBASE_TYPE               BaseType_t;
تعريفنوع unsigned portBASE_TYPE      UBaseType_t;

#إذا ( configUSE_16_BIT_TICKS == 1 )
تعريفنوع uint16_t TickType_t;
#define portMAX_DELAY ( TickType_t )  0xffff
#وإلا
تعريفنوع uint32_t TickType_t;
#define portMAX_DELAY ( TickType_t )  0xffffffffUL
#endif

/* Task function macros as described on the FreeRTOS.org WEB site. */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters )  فراغ vFunction( فراغ *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters )          فراغ vFunction( فراغ *pvParameters )

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#define portCRITICAL_NESTING_IN_TCB     1
#define port_دۆۋە_GROWTH                ( -1 )
#define portTICK_PERIOD_MS              ( ( TickType_t ) ( 1000 / configTICK_RATE_HZ ) )
#define portBYTE_ALIGNMENT              16
#define portNOP() __asm متطاير        (" nop ")

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros needed to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

/* ---------------------- Spinlocks ------------------------
 - Spinlocks added to match API with SMP FreeRTOS. Single core RISC-V does not need spin locks
 * ------------------------------------------------------ */

تعريفنوع spinlock_t                          portMUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */

#define portMUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#define portMUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#define portMUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#define portMUX_TRY_LOCK                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#define portMUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// --------------------- Interrupts ------------------------

BaseType_t xPortCheckIfInISR(فراغ);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortCheckIfInISR() internally
 */
فراغ vPortAssertIfInISR(فراغ);

// ------------------ Critical Sections --------------------

#إذا ( config_سان_OF_CORES > 1 )
/*
These are always called with interrupts already disabled. We simply need to get/release the spinlocks
*/
خارجي portMUX_TYPE port_xTaskLock;
خارجي portMUX_TYPE port_xISRLock;

فراغ vPortTakeLock( portMUX_TYPE *lock );
فراغ vPortReleaseLock( portMUX_TYPE *lock );
#endif /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

فراغ vPortYield( فراغ );
ساكن مضمن فراغ __attribute__((always_inline)) vPortYieldCore( BaseType_t xCoreID );

/**
 * @brief Set interrupt mask and return current interrupt enable register
 *
 * @return UBaseType_t Current interrupt enable register before set
 */
UBaseType_t ulPortSetInterruptMask(فراغ);

/**
 * @brief Clear current interrupt mask and set given mask
 *
 * @param mask Interrupt mask
 */
فراغ vPortClearInterruptMask(UBaseType_t mask);

/**
 * @brief Perform a context switch from an ISR
 */
فراغ vPortYieldFromISR(فراغ);

#define portYIELD_FROM_ISR_CHECK(x)     ({ \
    إذا ( (x) == pdTRUE ) { \
        vPortYieldFromISR(); \
    } \
})
#define portYIELD_FROM_ISR_NO_CHECK()               vPortYieldFromISR()

// ----------------------- System --------------------------

ساكن مضمن BaseType_t __attribute__((always_inline)) xPortGetCoreID( فراغ );

// --------------------- TCB Cleanup -----------------------

/**
 * @brief TCB cleanup hook
 *
 * The portCLEAN_UP_TCB() macro is called in prvDeleteTCB() right before a
 * deleted task's memory is freed. We map that macro to this internal function
 * so that IDF FreeRTOS ports can inject some task pre-deletion operations.
 */
فراغ vPortTCBPreDeleteHook( فراغ *pxTCB );

/* ------------------------------------------- FreeRTOS Porting Interface ----------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS
 * - Most come after forward declare as porting macros map to declared functions
 * - Maps to forward declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

#define portDISABLE_INTERRUPTS()                    ulPortSetInterruptMask()
#إذا !SOC_INT_CLIC_SUPPORTED
#define portENABLE_INTERRUPTS()                     vPortClearInterruptMask(RVHAL_INTR_ENABLE_THRESH)
#وإلا
#define portENABLE_INTERRUPTS()                     vPortClearInterruptMask(RVHAL_INTR_ENABLE_THRESH_CLIC)
#endif /* !SOC_INT_CLIC_SUPPORTED */
#define portRESTORE_INTERRUPTS(x)                   vPortClearInterruptMask(x)

#define portSET_INTERRUPT_MASK_FROM_ISR() ({           \
    unsigned صحيح cur_level; \
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

#إذا ( config_سان_OF_CORES > 1 )
#define portGET_TASK_LOCK()                         vPortTakeLock(&port_xTaskLock)
#define portRELEASE_TASK_LOCK()                     vPortReleaseLock(&port_xTaskLock)
#define portGET_ISR_LOCK()                          vPortTakeLock(&port_xISRLock)
#define portRELEASE_ISR_LOCK()                      vPortReleaseLock(&port_xISRLock)
#endif /* configNUMBER_OF_CORES > 1 */

//Critical sections used by FreeRTOS SMP
خارجي فراغ vTaskEnterCritical( فراغ );
خارجي فراغ vTaskExitCritical( فراغ );
#define portENTER_CRITICAL_SMP()                    vTaskEnterCritical();
#define portEXIT_CRITICAL_SMP()                     vTaskExitCritical();

#إذا defined(__cplusplus) && (__cplusplus >  201703L)
#define portENTER_CRITICAL(...)                     CHOOSE_MACRO_VA_ARG(portENTER_CRITICAL_IDF, portENTER_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#define portEXIT_CRITICAL(...)                      CHOOSE_MACRO_VA_ARG(portEXIT_CRITICAL_IDF, portEXIT_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#وإلا
#define portENTER_CRITICAL(...)                     CHOOSE_MACRO_VA_ARG(portENTER_CRITICAL_IDF, portENTER_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#define portEXIT_CRITICAL(...)                      CHOOSE_MACRO_VA_ARG(portEXIT_CRITICAL_IDF, portEXIT_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#endif

#define portENTER_CRITICAL_FROM_ISR() vTaskEnterCriticalFromISR()
#define portEXIT_CRITICAL_FROM_ISR(x) vTaskExitCriticalFromISR(x)

// ---------------------- Yielding -------------------------

#define portYIELD()                                 vPortYield()
#إذا defined(__cplusplus) && (__cplusplus >  201703L)
#define portYIELD_FROM_ISR(...)                     CHOOSE_MACRO_VA_ARG(portYIELD_FROM_ISR_CHECK, portYIELD_FROM_ISR_NO_CHECK __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#وإلا
#define portYIELD_FROM_ISR(...)                     CHOOSE_MACRO_VA_ARG(portYIELD_FROM_ISR_CHECK, portYIELD_FROM_ISR_NO_CHECK, ##__VA_ARGS__)(__VA_ARGS__)
#endif
#define portYIELD_CORE(x)                           vPortYieldCore(x)

// ----------------------- System --------------------------

#define portGET_CORE_ID()                           xPortGetCoreID()
#define portCHECK_IF_IN_ISR()                       xPortCheckIfInISR()

// ------------------- Run Time Stats ----------------------

#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()
#إذا ( CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS )
configRUN_TIME_COUNTER_TYPE x_Port_Get_Run_Time_Counter_قىممەت( فراغ );
#define port_GET_RUN_TIME_COUNTER_قىممەت()        x_Port_Get_Run_Time_Counter_قىممەت()
#endif

// --------------------- TCB Cleanup -----------------------

#define portCLEAN_UP_TCB( pxTCB )                   vPortTCBPreDeleteHook( pxTCB )


/* --------------------------------------------- Inline Implementations ------------------------------------------------
 * - Implementation of inline functions of the forward declares
 * - Should come after forward declare and FreeRTOS Porting interface, as implementation may use both.
 * - For implementation of non-inlined functions, see port.c, port_common.c, or other assembly files
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Yielding -------------------------

ساكن مضمن فراغ __attribute__((always_inline)) vPortYieldCore( BaseType_t xCoreID )
{
    esp_crosscore_int_send_yield( xCoreID );
}

// ----------------------- System --------------------------

ساكن مضمن BaseType_t __attribute__((always_inline)) xPortGetCoreID( فراغ )
{
    أرجع (BaseType_t) esp_cpu_get_core_id();
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

ساكن مضمن BaseType_t xPortInIsrContext(فراغ)
{
    //Just call the FreeRTOS port interface version
    أرجع xPortCheckIfInISR();
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
فراغ vPortEnterCritical(فراغ);
فراغ vPortExitCritical(فراغ);

#إذا CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
/**
 * @brief Claim thread-safe region start
 *        If claimed, vPortEnterCritical/vPortExitCritical on the current core are no-ops.
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
فراغ xPortThreadSafeClaim(فراغ);

/**
 * @brief Claim thread-safe region end
 *        Restores normal port critical behavior
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
فراغ xPortThreadSafeDisclaim(فراغ);

خارجي متطاير bool port_xThreadSafeClaimed;
#endif /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

//IDF task critical sections
#define portTRY_ENTER_CRITICAL(lock, timeout)       ({(فراغ) lock; (فراغ) timeout; vPortEnterCritical(); pdPASS;})
#define portENTER_CRITICAL_IDF(lock)                ({(فراغ) lock; vPortEnterCritical();})
#define portEXIT_CRITICAL_IDF(lock)                 ({(فراغ) lock; vPortExitCritical();})
//IDF ISR critical sections
#define portTRY_ENTER_CRITICAL_ISR(lock, timeout)   ({(فراغ) lock; (فراغ) timeout; vPortEnterCritical(); pdPASS;})
#define portENTER_CRITICAL_ISR(lock)                ({(فراغ) lock; vPortEnterCritical();})
#define portEXIT_CRITICAL_ISR(lock)                 ({(فراغ) lock; vPortExitCritical();})
//IDF safe critical sections (they're the same)
#define portENTER_CRITICAL_SAFE(lock)               ({(فراغ) lock; vPortEnterCritical();})
#define portEXIT_CRITICAL_SAFE(lock)                ({(فراغ) lock; vPortExitCritical();})

// ---------------------- Yielding -------------------------

ساكن مضمن bool xPortCanYield(فراغ)
{
#إذا SOC_INT_CLIC_SUPPORTED
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
    أرجع ((intr_level == 0) && (threshold == 0));
#وإلا/* !SOC_INT_CLIC_SUPPORTED */
    uint32_t threshold = REG_READ(INTERRUPT_CURRENT_CORE_INT_THRESH_REG);
    /* when enter critical code, FreeRTOS will mask threshold to RVHAL_EXCM_LEVEL
     * and exit critical code, will recover threshold value (1). so threshold <= 1
     * means not in critical code
     */
    أرجع (threshold <= 1);
#endif
}

// Defined even for configNUMBER_OF_CORES > 1 for IDF compatibility
#define portYIELD_WITHIN_API()                      esp_crosscore_int_send_yield(xPortGetCoreID())

// ----------------------- System --------------------------

فراغ v_Port_Set_دۆۋە_Watchpoint(فراغ *px_دۆۋە_Start);

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
bool xPortCheckValidListMem(ثابت فراغ *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool xPortCheckValidTCBMem(ثابت فراغ *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_دۆۋە_Mem(ثابت فراغ *ptr);

#define portVALID_LIST_MEM(ptr)     xPortCheckValidListMem(ptr)
#define portVALID_TCB_MEM(ptr)      xPortCheckValidTCBMem(ptr)
#define port_VALID_دۆۋە_MEM(ptr)    x_Portcheck_Valid_دۆۋە_Mem(ptr)

/* ------------------------------------------------------ Misc ---------------------------------------------------------
 * - Miscellaneous porting macros
 * - These are not part of the FreeRTOS porting interface, but are used by other FreeRTOS dependent components
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- App-Trace -------------------------

#إذا CONFIG_ESP_TRACE_ENABLE
خارجي متطاير BaseType_t xPortSwitchFlag;
#define os_task_switch_is_pended(_cpu_) (xPortSwitchFlag)
#وإلا
#define os_task_switch_is_pended(_cpu_) (false)
#endif

// --------------- Compatibility Includes ------------------
/*
ESP-IDF currently does not have a "Include what you use" policy. A lot of files implicitly pull in API through
portmacro.h. Therefore, we need to keep these headers around for now to allow the rest of IDF to compile.

[refactor-todo] Clean up ESP-IDF inclusion dependencies and add a inclusion check.
*/

#ضمن <stdlib.h>
#ضمن <stdbool.h>
#ضمن <stdarg.h>
#ضمن <limits.h>
#ضمن "esp_attr.h"
#ضمن "esp_newlib.h"
#ضمن "esp_heap_caps.h"
#ضمن "esp_rom_sys.h"
#ضمن "esp_system.h"             /* required by esp_get_...() functions in portable.h. [refactor-todo] Update portable.h */


#ifdef __cplusplus
}
#endif

#endif // __ASSEMBLER__
