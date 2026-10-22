/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#आशय once

#समावेश "sdkconfig.h"
#समावेश <stdint.h>
#समावेश "spinlock.h"
#समावेश "xtensa/xtruntime.h"
#समावेश "xtensa/config/core.h"
#समावेश "xtensa/config/core-isa.h"
#समावेश "xtensa/config/xt_specreg.h"
#समावेश "xt_instr_macros.h"
#समावेश "portbenchmark.h"
#समावेश "esp_macros.h"
#समावेश "esp_cpu.h"
#समावेश "esp_private/crosscore_int.h"
#समावेश "esp_memory_utils.h"

/*
Note: We should not include any FreeRTOS headers (directly or indirectly) here as this will create a reverse dependency
- FreeRTOS pulls in "portmacro.h" via
    - source.c <- FreeRTOS.h <- portable.h <- portmacro.h

- If we need types of task.h (such as TaskHandle_t), best put the feature somewhere else
- Be careful as some parts of C Standard Library Headers will pull in FreeRTOS headers
*/

#अगर_घोषित __cplusplus
बाह्य "C" {
#पूर्ण_अगर

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come before first as they are used further down the file
 * ------------------------------------------------------------------------------------------------------------------ */

#घोषणा portCHAR                    int8_t
#घोषणा portFLOAT                   भग्न
#घोषणा portDOUBLE                  द्विगुन
#घोषणा portLONG                    int32_t
#घोषणा portSHORT                   int16_t
#घोषणा port_ढेर_TYPE              uint8_t
#घोषणा portBASE_TYPE               पूर्णांक

प्रकार port_ढेर_TYPE              ढेर_Type_t;
प्रकार portBASE_TYPE               BaseType_t;
प्रकार अचिन्हित portBASE_TYPE      UBaseType_t;

#अगर( configUSE_16_BIT_TICKS == 1 )
प्रकार uint16_t TickType_t;
#घोषणा portMAX_DELAY ( TickType_t ) 0xffff
#अन्यथा
प्रकार uint32_t TickType_t;
#घोषणा portMAX_DELAY ( TickType_t ) 0xffffffffUL
#पूर्ण_अगर

/* Task function macros as described on the FreeRTOS.org WEB site. */
#घोषणा portTASK_FUNCTION_PROTO( vFunction, pvParameters )  व्योम vFunction( व्योम *pvParameters )
#घोषणा portTASK_FUNCTION( vFunction, pvParameters )        व्योम vFunction( व्योम *pvParameters )

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#घोषणा portCRITICAL_NESTING_IN_TCB     1
#घोषणा port_ढेर_GROWTH                ( -1 )
#घोषणा portTICK_PERIOD_MS              ( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#घोषणा portBYTE_ALIGNMENT              16    // Xtensa Windowed ABI requires the stack pointer to always be 16-byte aligned. See "isa_rm.pdf 8.1.1 Windowed Register Usage and Stack Layout"
#घोषणा portNOP()                       XT_NOP()    //Todo: Check if XT_NOP exists

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros need to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Spinlocks ------------------------

प्रकार spinlock_t                          portMUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */
#घोषणा portMUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#घोषणा portMUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#घोषणा portMUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#घोषणा portMUX_TRY_LOCK                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#घोषणा portMUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

BaseType_t xPortCheckIfInISR(व्योम);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortCheckIfInISR() internally
 */
व्योम vPortAssertIfInISR(व्योम);

// ------------------ Critical Sections --------------------

UBaseType_t uxPortEnterCriticalFromISR( व्योम );
व्योम vPortExitCriticalFromISR( UBaseType_t level );

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
अटल अंतरभूत व्योम __attribute__((always_inline)) vPortYieldFromISR( व्योम );

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


/* ----------------------------------------- FreeRTOS SMP Porting Interface --------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS SMP
 * - Must come after forward declare as some porting macros map to declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

#घोषणा portSET_INTERRUPT_MASK()            ({ \
    अचिन्हित पूर्णांक पूर्व_level = XTOS_SET_INTLEVEL(XCHAL_EXCM_LEVEL); \
    portbenchmarkINTERRUPT_DISABLE(); \
    पूर्व_level = ((पूर्व_level >> XCHAL_PS_INTLEVEL_SHIFT) & XCHAL_PS_INTLEVEL_MASK); \
    पूर्व_level; \
})
#घोषणा portSET_INTERRUPT_MASK_FROM_ISR()   portSET_INTERRUPT_MASK()
#घोषणा portDISABLE_INTERRUPTS()            portSET_INTERRUPT_MASK()

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
#घोषणा portCLEAR_INTERRUPT_MASK(x)         ({ \
    अचिन्हित पूर्णांक ps_reg; \
    RSR(XT_REG_PS, ps_reg); \
    ps_reg = (ps_reg & ~XCHAL_PS_INTLEVEL_MASK); \
    ps_reg |= ((x << XCHAL_PS_INTLEVEL_SHIFT) & XCHAL_PS_INTLEVEL_MASK); \
    XTOS_RESTORE_INTLEVEL(ps_reg); \
})
#घोषणा portCLEAR_INTERRUPT_MASK_FROM_ISR(x)    portCLEAR_INTERRUPT_MASK(x)
#घोषणा portENABLE_INTERRUPTS()     ({ \
    portbenchmarkINTERRUPT_RESTORE(0); \
    XTOS_SET_INTLEVEL(0); \
})

/* Compatibility */
#घोषणा portRESTORE_INTERRUPTS(x)   portCLEAR_INTERRUPT_MASK(x)

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

बाह्य UBaseType_t vTaskEnterCriticalFromISR( व्योम );
बाह्य व्योम vTaskExitCriticalFromISR( UBaseType_t uxSavedInterruptStatus );
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

//Timers are already configured, so nothing to do for configuration of run time stats timer
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
 * - For implementation of non-inlined functions, see port.c
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

// ------------------ Critical Sections --------------------

// ---------------------- Yielding -------------------------

बाह्य व्योम _frxt_setup_switch( व्योम );     //Defined in portasm.S

अटल अंतरभूत व्योम __attribute__((always_inline)) vPortYieldCore( BaseType_t xCoreID )
{
    esp_crosscore_int_send_yield( xCoreID );
}

अटल अंतरभूत व्योम __attribute__((always_inline)) vPortYieldFromISR( व्योम )
{
    //traceISR_EXIT_TO_SCHEDULER();
    _frxt_setup_switch();
}

// ----------------------- System --------------------------

अटल अंतरभूत BaseType_t __attribute__((always_inline)) xPortGetCoreID( व्योम )
{
    वापस (BaseType_t) esp_cpu_get_core_id();
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

अटल अंतरभूत BaseType_t xPortInIsrContext(व्योम)
{
    //Just call the FreeRTOS port interface version
    वापस xPortCheckIfInISR();
}

BaseType_t xPortInterruptedFromISRContext(व्योम);

अटल अंतरभूत UBaseType_t xPortSetInterruptMaskFromISR(व्योम)
{
    UBaseType_t पूर्व_int_level = XTOS_SET_INTLEVEL(XCHAL_EXCM_LEVEL);
    portbenchmarkINTERRUPT_DISABLE();
    वापस पूर्व_int_level;
}

अटल अंतरभूत व्योम vPortClearInterruptMaskFromISR(UBaseType_t पूर्व_level)
{
    portbenchmarkINTERRUPT_RESTORE(पूर्व_level);
    XTOS_RESTORE_JUST_INTLEVEL((पूर्णांक) पूर्व_level);
}

// ---------------------- Spinlocks ------------------------

अटल अंतरभूत bool __attribute__((always_inline)) vPortCPUAcquireMutexTimeout(portMUX_TYPE *mux, पूर्णांक timeout)
{
    वापस (spinlock_acquire(mux, timeout));
}

अटल अंतरभूत व्योम __attribute__((always_inline)) vPortCPUReleaseMutex(portMUX_TYPE *mux)
{
    spinlock_release(mux);
}

// ------------------ Critical Sections --------------------

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

BaseType_t xPortEnterCriticalTimeout(portMUX_TYPE *lock, BaseType_t timeout);

अटल अंतरभूत व्योम __attribute__((always_inline)) vPortEnterCriticalIDF(portMUX_TYPE *lock)
{
    xPortEnterCriticalTimeout(lock, portMUX_NO_TIMEOUT);
}

व्योम vPortExitCriticalIDF(portMUX_TYPE *lock);

/*
In case any IDF libs called the port critical functions directly instead of through the macros.
The will just call the vPort...CriticalIDF() functions
*/
व्योम vPortEnterCritical(portMUX_TYPE *lock);
व्योम vPortExitCritical(portMUX_TYPE *lock);

//IDF task critical sections
#घोषणा portTRY_ENTER_CRITICAL(lock, timeout)       xPortEnterCriticalTimeout(lock, timeout)
#घोषणा portENTER_CRITICAL_IDF(lock)                vPortEnterCriticalIDF(lock)
#घोषणा portEXIT_CRITICAL_IDF(lock)                 vPortExitCriticalIDF(lock)
//IDF ISR critical sections
#घोषणा portTRY_ENTER_CRITICAL_ISR(lock, timeout)   xPortEnterCriticalTimeout(lock, timeout)
#घोषणा portENTER_CRITICAL_ISR(lock)                vPortEnterCriticalIDF(lock)
#घोषणा portEXIT_CRITICAL_ISR(lock)                 vPortExitCriticalIDF(lock)
//IDF safe critical sections (they're the same)
#घोषणा portENTER_CRITICAL_SAFE(lock)               vPortEnterCriticalIDF(lock)
#घोषणा portEXIT_CRITICAL_SAFE(lock)                vPortExitCriticalIDF(lock)

// ---------------------- Yielding -------------------------

अटल अंतरभूत bool xPortCanYield(व्योम)
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

    वापस ((ps_reg & PS_INTLEVEL_MASK) == 0);
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
#समावेश <xtensa/hal.h>
#समावेश "esp_attr.h"
#समावेश "esp_newlib.h"
#समावेश "esp_heap_caps.h"
#समावेश "esp_rom_sys.h"
#समावेश "esp_system.h"
#समावेश "portbenchmark.h"
#समावेश <limits.h>
#समावेश <xtensa/config/system.h>
#समावेश <xtensa_api.h>

#अगर_घोषित __cplusplus
}
#पूर्ण_अगर
