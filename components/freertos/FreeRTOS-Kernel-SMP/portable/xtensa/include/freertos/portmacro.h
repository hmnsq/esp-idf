/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव <stdint.h>
#अन्तर्भाव "spinlock.h"
#अन्तर्भाव "xtensa/xtruntime.h"
#अन्तर्भाव "xtensa/config/core.h"
#अन्तर्भाव "xtensa/config/core-isa.h"
#अन्तर्भाव "xtensa/config/xt_specreg.h"
#अन्तर्भाव "xt_instr_macros.h"
#अन्तर्भाव "portbenchmark.h"
#अन्तर्भाव "esp_macros.h"
#अन्तर्भाव "esp_cpu.h"
#अन्तर्भाव "esp_private/crosscore_int.h"
#अन्तर्भाव "esp_memory_utils.h"

/*
Note: We should not include any FreeRTOS headers (directly or indirectly) here as this will create a reverse dependency
- FreeRTOS pulls in "portmacro.h" via
    - source.c <- FreeRTOS.h <- portable.h <- portmacro.h

- If we need types of task.h (such as TaskHandle_t), best put the feature somewhere else
- Be careful as some parts of C Standard Library Headers will pull in FreeRTOS headers
*/

#ifdef __cplusplus
बाह्य "C" {
#endif

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come before first as they are used further down the file
 * ------------------------------------------------------------------------------------------------------------------ */

#define portCHAR                    int8_t
#define portFLOAT                   भिन्न
#define portDOUBLE                  द्विगुण
#define portLONG                    int32_t
#define portSHORT                   int16_t
#define port_चळत_TYPE              uint8_t
#define portBASE_TYPE               पूर्णाङ्क

प्रकारनिर्देश port_चळत_TYPE              चळत_Type_t;
प्रकारनिर्देश portBASE_TYPE               BaseType_t;
प्रकारनिर्देश unsigned portBASE_TYPE      UBaseType_t;

#यदि( configUSE_16_BIT_TICKS == 1 )
प्रकारनिर्देश uint16_t TickType_t;
#define portMAX_DELAY ( TickType_t ) 0xffff
#अन्यथा
प्रकारनिर्देश uint32_t TickType_t;
#define portMAX_DELAY ( TickType_t ) 0xffffffffUL
#endif

/* Task function macros as described on the FreeRTOS.org WEB site. */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters )  शून्य vFunction( शून्य *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters )        शून्य vFunction( शून्य *pvParameters )

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#define portCRITICAL_NESTING_IN_TCB     1
#define port_चळत_GROWTH                ( -1 )
#define portTICK_PERIOD_MS              ( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#define portBYTE_ALIGNMENT              16    // Xtensa Windowed ABI requires the stack pointer to always be 16-byte aligned. See "isa_rm.pdf 8.1.1 Windowed Register Usage and Stack Layout"
#define portNOP()                       XT_NOP()    //Todo: Check if XT_NOP exists

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros need to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Spinlocks ------------------------

प्रकारनिर्देश spinlock_t                          portMUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */
#define portMUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#define portMUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#define portMUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#define portMUX_TRY_LOCK                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#define portMUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

BaseType_t xPortCheckIfInISR(शून्य);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortCheckIfInISR() internally
 */
शून्य vPortAssertIfInISR(शून्य);

// ------------------ Critical Sections --------------------

UBaseType_t uxPortEnterCriticalFromISR( शून्य );
शून्य vPortExitCriticalFromISR( UBaseType_t level );

#यदि ( config_संख्या_OF_CORES > 1 )
/*
These are always called with interrupts already disabled. We simply need to get/release the spinlocks
*/
बाह्य portMUX_TYPE port_xTaskLock;
बाह्य portMUX_TYPE port_xISRLock;

शून्य vPortTakeLock( portMUX_TYPE *lock );
शून्य vPortReleaseLock( portMUX_TYPE *lock );
#endif /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

शून्य vPortYield( शून्य );
स्थिर अन्तःस्थ शून्य __attribute__((always_inline)) vPortYieldCore( BaseType_t xCoreID );
स्थिर अन्तःस्थ शून्य __attribute__((always_inline)) vPortYieldFromISR( शून्य );

#define portYIELD_FROM_ISR_CHECK(x)     ({ \
    यदि ( (x) == pdTRUE ) { \
        vPortYieldFromISR(); \
    } \
})
#define portYIELD_FROM_ISR_NO_CHECK()               vPortYieldFromISR()

// ----------------------- System --------------------------

स्थिर अन्तःस्थ BaseType_t __attribute__((always_inline)) xPortGetCoreID( शून्य );

// --------------------- TCB Cleanup -----------------------

/**
 * @brief TCB cleanup hook
 *
 * The portCLEAN_UP_TCB() macro is called in prvDeleteTCB() right before a
 * deleted task's memory is freed. We map that macro to this internal function
 * so that IDF FreeRTOS ports can inject some task pre-deletion operations.
 */
शून्य vPortTCBPreDeleteHook( शून्य *pxTCB );


/* ----------------------------------------- FreeRTOS SMP Porting Interface --------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS SMP
 * - Must come after forward declare as some porting macros map to declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

#define portSET_INTERRUPT_MASK()            ({ \
    unsigned पूर्णाङ्क prev_level = XTOS_SET_INTLEVEL(XCHAL_EXCM_LEVEL); \
    portbenchmarkINTERRUPT_DISABLE(); \
    prev_level = ((prev_level >> XCHAL_PS_INTLEVEL_SHIFT) & XCHAL_PS_INTLEVEL_MASK); \
    prev_level; \
})
#define portSET_INTERRUPT_MASK_FROM_ISR()   portSET_INTERRUPT_MASK()
#define portDISABLE_INTERRUPTS()            portSET_INTERRUPT_MASK()

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
#define portCLEAR_INTERRUPT_MASK(x)         ({ \
    unsigned पूर्णाङ्क ps_reg; \
    RSR(XT_REG_PS, ps_reg); \
    ps_reg = (ps_reg & ~XCHAL_PS_INTLEVEL_MASK); \
    ps_reg |= ((x << XCHAL_PS_INTLEVEL_SHIFT) & XCHAL_PS_INTLEVEL_MASK); \
    XTOS_RESTORE_INTLEVEL(ps_reg); \
})
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x)    portCLEAR_INTERRUPT_MASK(x)
#define portENABLE_INTERRUPTS()     ({ \
    portbenchmarkINTERRUPT_RESTORE(0); \
    XTOS_SET_INTLEVEL(0); \
})

/* Compatibility */
#define portRESTORE_INTERRUPTS(x)   portCLEAR_INTERRUPT_MASK(x)

// ------------------ Critical Sections --------------------

#यदि ( config_संख्या_OF_CORES > 1 )
#define portGET_TASK_LOCK()                         vPortTakeLock(&port_xTaskLock)
#define portRELEASE_TASK_LOCK()                     vPortReleaseLock(&port_xTaskLock)
#define portGET_ISR_LOCK()                          vPortTakeLock(&port_xISRLock)
#define portRELEASE_ISR_LOCK()                      vPortReleaseLock(&port_xISRLock)
#endif /* configNUMBER_OF_CORES > 1 */

//Critical sections used by FreeRTOS SMP
बाह्य शून्य vTaskEnterCritical( शून्य );
बाह्य शून्य vTaskExitCritical( शून्य );
#define portENTER_CRITICAL_SMP()                    vTaskEnterCritical();
#define portEXIT_CRITICAL_SMP()                     vTaskExitCritical();

#यदि defined(__cplusplus) && (__cplusplus >  201703L)
#define portENTER_CRITICAL(...)                     CHOOSE_MACRO_VA_ARG(portENTER_CRITICAL_IDF, portENTER_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#define portEXIT_CRITICAL(...)                      CHOOSE_MACRO_VA_ARG(portEXIT_CRITICAL_IDF, portEXIT_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#अन्यथा
#define portENTER_CRITICAL(...)                     CHOOSE_MACRO_VA_ARG(portENTER_CRITICAL_IDF, portENTER_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#define portEXIT_CRITICAL(...)                      CHOOSE_MACRO_VA_ARG(portEXIT_CRITICAL_IDF, portEXIT_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#endif

बाह्य UBaseType_t vTaskEnterCriticalFromISR( शून्य );
बाह्य शून्य vTaskExitCriticalFromISR( UBaseType_t uxSavedInterruptStatus );
#define portENTER_CRITICAL_FROM_ISR() vTaskEnterCriticalFromISR()
#define portEXIT_CRITICAL_FROM_ISR(x) vTaskExitCriticalFromISR(x)

// ---------------------- Yielding -------------------------

#define portYIELD()                                 vPortYield()
#यदि defined(__cplusplus) && (__cplusplus >  201703L)
#define portYIELD_FROM_ISR(...)                     CHOOSE_MACRO_VA_ARG(portYIELD_FROM_ISR_CHECK, portYIELD_FROM_ISR_NO_CHECK __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#अन्यथा
#define portYIELD_FROM_ISR(...)                     CHOOSE_MACRO_VA_ARG(portYIELD_FROM_ISR_CHECK, portYIELD_FROM_ISR_NO_CHECK, ##__VA_ARGS__)(__VA_ARGS__)
#endif
#define portYIELD_CORE(x)                           vPortYieldCore(x)

// ----------------------- System --------------------------

#define portGET_CORE_ID()                           xPortGetCoreID()
#define portCHECK_IF_IN_ISR()                       xPortCheckIfInISR()

// ------------------- Run Time Stats ----------------------

//Timers are already configured, so nothing to do for configuration of run time stats timer
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()
#यदि ( CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS )
configRUN_TIME_COUNTER_TYPE x_Port_Get_Run_Time_Counter_मूल्य( शून्य );
#define port_GET_RUN_TIME_COUNTER_मूल्य()        x_Port_Get_Run_Time_Counter_मूल्य()
#endif

// --------------------- TCB Cleanup -----------------------

#define portCLEAN_UP_TCB( pxTCB )                   vPortTCBPreDeleteHook( pxTCB )

/* --------------------------------------------- Inline Implementations ------------------------------------------------
 * - Implementation of inline functions of the forward declares
 * - Should come after forward declare and FreeRTOS Porting interface, as implementation may use both.
 * - For implementation of non-inlined functions, see port.c
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

// ------------------ Critical Sections --------------------

// ---------------------- Yielding -------------------------

बाह्य शून्य _frxt_setup_switch( शून्य );     //Defined in portasm.S

स्थिर अन्तःस्थ शून्य __attribute__((always_inline)) vPortYieldCore( BaseType_t xCoreID )
{
    esp_crosscore_int_send_yield( xCoreID );
}

स्थिर अन्तःस्थ शून्य __attribute__((always_inline)) vPortYieldFromISR( शून्य )
{
    //traceISR_EXIT_TO_SCHEDULER();
    _frxt_setup_switch();
}

// ----------------------- System --------------------------

स्थिर अन्तःस्थ BaseType_t __attribute__((always_inline)) xPortGetCoreID( शून्य )
{
    प्रत्यागम (BaseType_t) esp_cpu_get_core_id();
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

स्थिर अन्तःस्थ BaseType_t xPortInIsrContext(शून्य)
{
    //Just call the FreeRTOS port interface version
    प्रत्यागम xPortCheckIfInISR();
}

BaseType_t xPortInterruptedFromISRContext(शून्य);

स्थिर अन्तःस्थ UBaseType_t xPortSetInterruptMaskFromISR(शून्य)
{
    UBaseType_t prev_int_level = XTOS_SET_INTLEVEL(XCHAL_EXCM_LEVEL);
    portbenchmarkINTERRUPT_DISABLE();
    प्रत्यागम prev_int_level;
}

स्थिर अन्तःस्थ शून्य vPortClearInterruptMaskFromISR(UBaseType_t prev_level)
{
    portbenchmarkINTERRUPT_RESTORE(prev_level);
    XTOS_RESTORE_JUST_INTLEVEL((पूर्णाङ्क) prev_level);
}

// ---------------------- Spinlocks ------------------------

स्थिर अन्तःस्थ bool __attribute__((always_inline)) vPortCPUAcquireMutexTimeout(portMUX_TYPE *mux, पूर्णाङ्क timeout)
{
    प्रत्यागम (spinlock_acquire(mux, timeout));
}

स्थिर अन्तःस्थ शून्य __attribute__((always_inline)) vPortCPUReleaseMutex(portMUX_TYPE *mux)
{
    spinlock_release(mux);
}

// ------------------ Critical Sections --------------------

#यदि CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
/**
 * @brief Claim thread-safe region start
 *        If claimed, vPortEnterCritical/vPortExitCritical on the current core are no-ops.
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
शून्य xPortThreadSafeClaim(शून्य);

/**
 * @brief Claim thread-safe region end
 *        Restores normal port critical behavior
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
शून्य xPortThreadSafeDisclaim(शून्य);

बाह्य चञ्चल bool port_xThreadSafeClaimed;
#endif /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

BaseType_t xPortEnterCriticalTimeout(portMUX_TYPE *lock, BaseType_t timeout);

स्थिर अन्तःस्थ शून्य __attribute__((always_inline)) vPortEnterCriticalIDF(portMUX_TYPE *lock)
{
    xPortEnterCriticalTimeout(lock, portMUX_NO_TIMEOUT);
}

शून्य vPortExitCriticalIDF(portMUX_TYPE *lock);

/*
In case any IDF libs called the port critical functions directly instead of through the macros.
The will just call the vPort...CriticalIDF() functions
*/
शून्य vPortEnterCritical(portMUX_TYPE *lock);
शून्य vPortExitCritical(portMUX_TYPE *lock);

//IDF task critical sections
#define portTRY_ENTER_CRITICAL(lock, timeout)       xPortEnterCriticalTimeout(lock, timeout)
#define portENTER_CRITICAL_IDF(lock)                vPortEnterCriticalIDF(lock)
#define portEXIT_CRITICAL_IDF(lock)                 vPortExitCriticalIDF(lock)
//IDF ISR critical sections
#define portTRY_ENTER_CRITICAL_ISR(lock, timeout)   xPortEnterCriticalTimeout(lock, timeout)
#define portENTER_CRITICAL_ISR(lock)                vPortEnterCriticalIDF(lock)
#define portEXIT_CRITICAL_ISR(lock)                 vPortExitCriticalIDF(lock)
//IDF safe critical sections (they're the same)
#define portENTER_CRITICAL_SAFE(lock)               vPortEnterCriticalIDF(lock)
#define portEXIT_CRITICAL_SAFE(lock)                vPortExitCriticalIDF(lock)

// ---------------------- Yielding -------------------------

स्थिर अन्तःस्थ bool xPortCanYield(शून्य)
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

    प्रत्यागम ((ps_reg & PS_INTLEVEL_MASK) == 0);
}

// Defined even for configNUMBER_OF_CORES > 1 for IDF compatibility
#define portYIELD_WITHIN_API()                      esp_crosscore_int_send_yield(xPortGetCoreID())

// ----------------------- System --------------------------

शून्य v_Port_Set_चळत_Watchpoint(शून्य *px_चळत_Start);

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
bool xPortCheckValidListMem(ध्रुव शून्य *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool xPortCheckValidTCBMem(ध्रुव शून्य *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_चळत_Mem(ध्रुव शून्य *ptr);

#define portVALID_LIST_MEM(ptr)     xPortCheckValidListMem(ptr)
#define portVALID_TCB_MEM(ptr)      xPortCheckValidTCBMem(ptr)
#define port_VALID_चळत_MEM(ptr)    x_Portcheck_Valid_चळत_Mem(ptr)

/* ------------------------------------------------------ Misc ---------------------------------------------------------
 * - Miscellaneous porting macros
 * - These are not part of the FreeRTOS porting interface, but are used by other FreeRTOS dependent components
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- App-Trace -------------------------

#यदि CONFIG_ESP_TRACE_ENABLE
बाह्य चञ्चल BaseType_t xPortSwitchFlag;
#define os_task_switch_is_pended(_cpu_) (xPortSwitchFlag)
#अन्यथा
#define os_task_switch_is_pended(_cpu_) (false)
#endif

// --------------- Compatibility Includes ------------------
/*
ESP-IDF currently does not have a "Include what you use" policy. A lot of files implicitly pull in API through
portmacro.h. Therefore, we need to keep these headers around for now to allow the rest of IDF to compile.

[refactor-todo] Clean up ESP-IDF inclusion dependencies and add a inclusion check.
*/

#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <stdarg.h>
#अन्तर्भाव <xtensa/hal.h>
#अन्तर्भाव "esp_attr.h"
#अन्तर्भाव "esp_newlib.h"
#अन्तर्भाव "esp_heap_caps.h"
#अन्तर्भाव "esp_rom_sys.h"
#अन्तर्भाव "esp_system.h"
#अन्तर्भाव "portbenchmark.h"
#अन्तर्भाव <limits.h>
#अन्तर्भाव <xtensa/config/system.h>
#अन्तर्भाव <xtensa_api.h>

#ifdef __cplusplus
}
#endif
