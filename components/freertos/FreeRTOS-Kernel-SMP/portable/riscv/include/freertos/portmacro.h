/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#आशय once

#समावेश "sdkconfig.h"

/* Macros used instead ofsetoff() for better performance of interrupt handler */
#अगर CONFIG_FREERTOS_USE_LIST_तथ्य_INTEGRITY_CHECK_BYTES
/*
pxTopOfStack (4) +
xStateListItem (28) +
xEventListItem (28) +
uxPriority (4)
*/
#घोषणा PORT_OFFSET_PX_ढेर 0x40
#अन्यथा
/*
pxTopOfStack (4) +
xStateListItem (20) +
xEventListItem (20) +
uxPriority (4)
*/
#घोषणा PORT_OFFSET_PX_ढेर 0x30
#पूर्ण_अगर /* #if CONFIG_FREERTOS_USE_LIST_DATA_INTEGRITY_CHECK_BYTES */

#अगर ( config_संख्या_OF_CORES > 1 )
#घोषणा PORT_TCB_CORE_FIELDS_आकार 8
#अन्यथा
#घोषणा PORT_TCB_CORE_FIELDS_आकार 0
#पूर्ण_अगर

#अगर ( configUSE_TASK_PREEMPTION_DISABLE == 1 )
#घोषणा PORT_TCB_PREEMPT_DISABLE_FIELD_आकार 4
#अन्यथा
#घोषणा PORT_TCB_PREEMPT_DISABLE_FIELD_आकार 0
#पूर्ण_अगर

/* Align the value up to the nearest multiple of 4 */
#घोषणा PORT_ALIGN_UP_TO_4(मूल्य) (((मूल्य) + 3) & ~3)

#घोषणा PORT_OFFSET_PX_END_OF_ढेर ( \
    PORT_OFFSET_PX_ढेर \
    + 4                                 /* StackType_t * pxStack */ \
    + PORT_TCB_CORE_FIELDS_आकार                     /* BaseType_t xDummy23 + UBaseType_t uxDummy24 */ \
    + PORT_ALIGN_UP_TO_4(configMAX_TASK_NAME_LEN)   /* pcTaskName[ configMAX_TASK_NAME_LEN ] */ \
    + PORT_TCB_PREEMPT_DISABLE_FIELD_आकार           /* BaseType_t xDummy25 */ \
)

#अगर_अघोषित __ASSEMBLER__

#समावेश <stdint.h>
#समावेश "spinlock.h"
#समावेश "soc/interrupt_reg.h"
#समावेश "esp_macros.h"
#समावेश "esp_cpu.h"
#समावेश "esp_private/crosscore_int.h"
#समावेश "esp_memory_utils.h"

#अगर_घोषित __cplusplus
बाह्य "C" {
#पूर्ण_अगर

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come first as they are used further down in this file
 * ------------------------------------------------------------------------------------------------------------------ */

#घोषणा portCHAR                    uint8_t
#घोषणा portFLOAT                   भग्न
#घोषणा portDOUBLE                  द्विगुन
#घोषणा portLONG                    int32_t
#घोषणा portSHORT                   int16_t
#घोषणा port_ढेर_TYPE              uint8_t
#घोषणा portBASE_TYPE               पूर्णांक

प्रकार port_ढेर_TYPE              ढेर_Type_t;
प्रकार portBASE_TYPE               BaseType_t;
प्रकार अचिन्हित portBASE_TYPE      UBaseType_t;

#अगर ( configUSE_16_BIT_TICKS == 1 )
प्रकार uint16_t TickType_t;
#घोषणा portMAX_DELAY ( TickType_t )  0xffff
#अन्यथा
प्रकार uint32_t TickType_t;
#घोषणा portMAX_DELAY ( TickType_t )  0xffffffffUL
#पूर्ण_अगर

/* Task function macros as described on the FreeRTOS.org WEB site. */
#घोषणा portTASK_FUNCTION_PROTO( vFunction, pvParameters )  व्योम vFunction( व्योम *pvParameters )
#घोषणा portTASK_FUNCTION( vFunction, pvParameters )          व्योम vFunction( व्योम *pvParameters )

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#घोषणा portCRITICAL_NESTING_IN_TCB     1
#घोषणा port_ढेर_GROWTH                ( -1 )
#घोषणा portTICK_PERIOD_MS              ( ( TickType_t ) ( 1000 / configTICK_RATE_HZ ) )
#घोषणा portBYTE_ALIGNMENT              16
#घोषणा portNOP() __asm अस्थिर        (" nop ")

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros needed to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

/* ---------------------- Spinlocks ------------------------
 - Spinlocks added to match API with SMP FreeRTOS. Single core RISC-V does not need spin locks
 * ------------------------------------------------------ */

प्रकार spinlock_t                          portMUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */

#घोषणा portMUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#घोषणा portMUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#घोषणा portMUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#घोषणा portMUX_TRY_LOCK                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#घोषणा portMUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// --------------------- Interrupts ------------------------

BaseType_t xPortCheckIfInISR(व्योम);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortCheckIfInISR() internally
 */
व्योम vPortAssertIfInISR(व्योम);

// ------------------ Critical Sections --------------------

#अगर ( config_संख्या_OF_CORES > 1 )
/*
These are always called with interrupts already disabled. We simply need to get/release the spinlocks
*/
बाह्य portMUX_TYPE port_xTaskLock;
बाह्य portMUX_TYPE port_xISRLock;

व्योम vPortTakeLock( portMUX_TYPE *lock );
व्योम vPortReleaseLock( portMUX_TYPE *lock );
#पूर्ण_अगर /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

व्योम vPortYield( व्योम );
अटल अंतरभूत व्योम __attribute__((always_inline)) vPortYieldCore( BaseType_t xCoreID );

/**
 * @brief Set interrupt mask and return current interrupt enable register
 *
 * @return UBaseType_t Current interrupt enable register before set
 */
UBaseType_t ulPortSetInterruptMask(व्योम);

/**
 * @brief Clear current interrupt mask and set given mask
 *
 * @param mask Interrupt mask
 */
व्योम vPortClearInterruptMask(UBaseType_t mask);

/**
 * @brief Perform a context switch from an ISR
 */
व्योम vPortYieldFromISR(व्योम);

#घोषणा portYIELD_FROM_ISR_CHECK(x)     ({ \
    अगर ( (x) == pdTRUE ) { \
        vPortYieldFromISR(); \
    } \
})
#घोषणा portYIELD_FROM_ISR_NO_CHECK()               vPortYieldFromISR()

// ----------------------- System --------------------------

अटल अंतरभूत BaseType_t __attribute__((always_inline)) xPortGetCoreID( व्योम );

// --------------------- TCB Cleanup -----------------------

/**
 * @brief TCB cleanup hook
 *
 * The portCLEAN_UP_TCB() macro is called in prvDeleteTCB() right before a
 * deleted task's memory is freed. We map that macro to this internal function
 * so that IDF FreeRTOS ports can inject some task pre-deletion operations.
 */
व्योम vPortTCBPreDeleteHook( व्योम *pxTCB );

/* ------------------------------------------- FreeRTOS Porting Interface ----------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS
 * - Most come after forward declare as porting macros map to declared functions
 * - Maps to forward declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

#घोषणा portDISABLE_INTERRUPTS()                    ulPortSetInterruptMask()
#अगर !SOC_INT_CLIC_SUPPORTED
#घोषणा portENABLE_INTERRUPTS()                     vPortClearInterruptMask(RVHAL_INTR_ENABLE_THRESH)
#अन्यथा
#घोषणा portENABLE_INTERRUPTS()                     vPortClearInterruptMask(RVHAL_INTR_ENABLE_THRESH_CLIC)
#पूर्ण_अगर /* !SOC_INT_CLIC_SUPPORTED */
#घोषणा portRESTORE_INTERRUPTS(x)                   vPortClearInterruptMask(x)

#घोषणा portSET_INTERRUPT_MASK_FROM_ISR() ({           \
    अचिन्हित पूर्णांक cur_level; \
    cur_level = ulPortSetInterruptMask(); \
    cur_level; \
})
#घोषणा portCLEAR_INTERRUPT_MASK_FROM_ISR(x)        portRESTORE_INTERRUPTS(x)
#घोषणा portSET_INTERRUPT_MASK()                    portSET_INTERRUPT_MASK_FROM_ISR()
#घोषणा portCLEAR_INTERRUPT_MASK(x)                 portCLEAR_INTERRUPT_MASK_FROM_ISR(x)

/**
 * @brief Assert if in ISR context
 *
 * TODO: Enable once ISR safe version of vTaskEnter/ExitCritical() is implemented
 * for single-core SMP FreeRTOS Kernel. (IDF-10540)
 */
// #define portASSERT_IF_IN_ISR() vPortAssertIfInISR()


// ------------------ Critical Sections --------------------

#अगर ( config_संख्या_OF_CORES > 1 )
#घोषणा portGET_TASK_LOCK()                         vPortTakeLock(&port_xTaskLock)
#घोषणा portRELEASE_TASK_LOCK()                     vPortReleaseLock(&port_xTaskLock)
#घोषणा portGET_ISR_LOCK()                          vPortTakeLock(&port_xISRLock)
#घोषणा portRELEASE_ISR_LOCK()                      vPortReleaseLock(&port_xISRLock)
#पूर्ण_अगर /* configNUMBER_OF_CORES > 1 */

//Critical sections used by FreeRTOS SMP
बाह्य व्योम vTaskEnterCritical( व्योम );
बाह्य व्योम vTaskExitCritical( व्योम );
#घोषणा portENTER_CRITICAL_SMP()                    vTaskEnterCritical();
#घोषणा portEXIT_CRITICAL_SMP()                     vTaskExitCritical();

#अगर defined(__cplusplus) && (__cplusplus >  201703L)
#घोषणा portENTER_CRITICAL(...)                     CHOOSE_MACRO_VA_ARG(portENTER_CRITICAL_IDF, portENTER_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#घोषणा portEXIT_CRITICAL(...)                      CHOOSE_MACRO_VA_ARG(portEXIT_CRITICAL_IDF, portEXIT_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#अन्यथा
#घोषणा portENTER_CRITICAL(...)                     CHOOSE_MACRO_VA_ARG(portENTER_CRITICAL_IDF, portENTER_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#घोषणा portEXIT_CRITICAL(...)                      CHOOSE_MACRO_VA_ARG(portEXIT_CRITICAL_IDF, portEXIT_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#पूर्ण_अगर

#घोषणा portENTER_CRITICAL_FROM_ISR() vTaskEnterCriticalFromISR()
#घोषणा portEXIT_CRITICAL_FROM_ISR(x) vTaskExitCriticalFromISR(x)

// ---------------------- Yielding -------------------------

#घोषणा portYIELD()                                 vPortYield()
#अगर defined(__cplusplus) && (__cplusplus >  201703L)
#घोषणा portYIELD_FROM_ISR(...)                     CHOOSE_MACRO_VA_ARG(portYIELD_FROM_ISR_CHECK, portYIELD_FROM_ISR_NO_CHECK __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#अन्यथा
#घोषणा portYIELD_FROM_ISR(...)                     CHOOSE_MACRO_VA_ARG(portYIELD_FROM_ISR_CHECK, portYIELD_FROM_ISR_NO_CHECK, ##__VA_ARGS__)(__VA_ARGS__)
#पूर्ण_अगर
#घोषणा portYIELD_CORE(x)                           vPortYieldCore(x)

// ----------------------- System --------------------------

#घोषणा portGET_CORE_ID()                           xPortGetCoreID()
#घोषणा portCHECK_IF_IN_ISR()                       xPortCheckIfInISR()

// ------------------- Run Time Stats ----------------------

#घोषणा portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()
#अगर ( CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS )
config_RUN_TIME_गणक_TYPE x_Port_Get_Run_Time_गणक_मूल्य( व्योम );
#घोषणा port_GET_RUN_TIME_गणक_मूल्य()        x_Port_Get_Run_Time_गणक_मूल्य()
#पूर्ण_अगर

// --------------------- TCB Cleanup -----------------------

#घोषणा portCLEAN_UP_TCB( pxTCB )                   vPortTCBPreDeleteHook( pxTCB )


/* --------------------------------------------- Inline Implementations ------------------------------------------------
 * - Implementation of inline functions of the forward declares
 * - Should come after forward declare and FreeRTOS Porting interface, as implementation may use both.
 * - For implementation of non-inlined functions, see port.c, port_common.c, or other assembly files
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Yielding -------------------------

अटल अंतरभूत व्योम __attribute__((always_inline)) vPortYieldCore( BaseType_t xCoreID )
{
    esp_crosscore_int_send_yield( xCoreID );
}

// ----------------------- System --------------------------

अटल अंतरभूत BaseType_t __attribute__((always_inline)) xPortGetCoreID( व्योम )
{
    वापस (BaseType_t) esp_cpu_get_core_id();
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

अटल अंतरभूत BaseType_t xPortInIsrContext(व्योम)
{
    //Just call the FreeRTOS port interface version
    वापस xPortCheckIfInISR();
}

// Added for backward compatibility with IDF
#घोषणा xPortInterruptedFromISRContext()    xPortInIsrContext()

// ------------------ Critical Sections --------------------

/*
IDF style critical sections which are orthogonal to FreeRTOS critical sections. However, on single core, the IDF style
critical sections simply disable interrupts, thus we discard the lock and timeout arguments.

Note: The IDF Style critical sections are named differently to their counterparts present in the xtensa port as few IDF
examples such as esp_zigbee_gateway, when compiled for RISC-V targets, have a reference to vPortEnterCritical()
and vPortExitCritical() from precompiled libraries (.a) thereby failing linking.
*/
व्योम vPortEnterCritical(व्योम);
व्योम vPortExitCritical(व्योम);

#अगर CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
/**
 * @brief Claim thread-safe region start
 *        If claimed, vPortEnterCritical/vPortExitCritical on the current core are no-ops.
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
व्योम xPortThreadSafeClaim(व्योम);

/**
 * @brief Claim thread-safe region end
 *        Restores normal port critical behavior
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
व्योम xPortThreadSafeDisclaim(व्योम);

बाह्य अस्थिर bool port_xThreadSafeClaimed;
#पूर्ण_अगर /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

//IDF task critical sections
#घोषणा portTRY_ENTER_CRITICAL(lock, timeout)       ({(व्योम) lock; (व्योम) timeout; vPortEnterCritical(); pdPASS;})
#घोषणा portENTER_CRITICAL_IDF(lock)                ({(व्योम) lock; vPortEnterCritical();})
#घोषणा portEXIT_CRITICAL_IDF(lock)                 ({(व्योम) lock; vPortExitCritical();})
//IDF ISR critical sections
#घोषणा portTRY_ENTER_CRITICAL_ISR(lock, timeout)   ({(व्योम) lock; (व्योम) timeout; vPortEnterCritical(); pdPASS;})
#घोषणा portENTER_CRITICAL_ISR(lock)                ({(व्योम) lock; vPortEnterCritical();})
#घोषणा portEXIT_CRITICAL_ISR(lock)                 ({(व्योम) lock; vPortExitCritical();})
//IDF safe critical sections (they're the same)
#घोषणा portENTER_CRITICAL_SAFE(lock)               ({(व्योम) lock; vPortEnterCritical();})
#घोषणा portEXIT_CRITICAL_SAFE(lock)                ({(व्योम) lock; vPortExitCritical();})

// ---------------------- Yielding -------------------------

अटल अंतरभूत bool xPortCanYield(व्योम)
{
#अगर SOC_INT_CLIC_SUPPORTED
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
    वापस ((intr_level == 0) && (threshold == 0));
#अन्यथा/* !SOC_INT_CLIC_SUPPORTED */
    uint32_t threshold = REG_READ(INTERRUPT_CURRENT_CORE_INT_THRESH_REG);
    /* when enter critical code, FreeRTOS will mask threshold to RVHAL_EXCM_LEVEL
     * and exit critical code, will recover threshold value (1). so threshold <= 1
     * means not in critical code
     */
    वापस (threshold <= 1);
#पूर्ण_अगर
}

// Defined even for configNUMBER_OF_CORES > 1 for IDF compatibility
#घोषणा portYIELD_WITHIN_API()                      esp_crosscore_int_send_yield(xPortGetCoreID())

// ----------------------- System --------------------------

व्योम v_Port_Set_ढेर_Watchpoint(व्योम *px_ढेर_Start);

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
bool xPortCheckValidListMem(स्थिर व्योम *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool xPortCheckValidTCBMem(स्थिर व्योम *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_ढेर_Mem(स्थिर व्योम *ptr);

#घोषणा portVALID_LIST_MEM(ptr)     xPortCheckValidListMem(ptr)
#घोषणा portVALID_TCB_MEM(ptr)      xPortCheckValidTCBMem(ptr)
#घोषणा port_VALID_ढेर_MEM(ptr)    x_Portcheck_Valid_ढेर_Mem(ptr)

/* ------------------------------------------------------ Misc ---------------------------------------------------------
 * - Miscellaneous porting macros
 * - These are not part of the FreeRTOS porting interface, but are used by other FreeRTOS dependent components
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- App-Trace -------------------------

#अगर CONFIG_ESP_TRACE_ENABLE
बाह्य अस्थिर BaseType_t xPortSwitchFlag;
#घोषणा os_task_switch_is_pended(_cpu_) (xPortSwitchFlag)
#अन्यथा
#घोषणा os_task_switch_is_pended(_cpu_) (false)
#पूर्ण_अगर

// --------------- Compatibility Includes ------------------
/*
ESP-IDF currently does not have a "Include what you use" policy. A lot of files implicitly pull in API through
portmacro.h. Therefore, we need to keep these headers around for now to allow the rest of IDF to compile.

[refactor-todo] Clean up ESP-IDF inclusion dependencies and add a inclusion check.
*/

#समावेश <stdlib.h>
#समावेश <stdbool.h>
#समावेश <stdarg.h>
#समावेश <limits.h>
#समावेश "esp_attr.h"
#समावेश "esp_newlib.h"
#समावेश "esp_heap_caps.h"
#समावेश "esp_rom_sys.h"
#समावेश "esp_system.h"             /* required by esp_get_...() functions in portable.h. [refactor-todo] Update portable.h */


#अगर_घोषित __cplusplus
}
#पूर्ण_अगर

#पूर्ण_अगर // __ASSEMBLER__
