/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#അന്തര്ഭാവ "sdkconfig.h"
#അന്തര്ഭാവ <stdint.h>
#അന്തര്ഭാവ "spinlock.h"
#അന്തര്ഭാവ "xtensa/xtruntime.h"
#അന്തര്ഭാവ "xtensa/config/core.h"
#അന്തര്ഭാവ "xtensa/config/core-isa.h"
#അന്തര്ഭാവ "xtensa/config/xt_specreg.h"
#അന്തര്ഭാവ "xt_instr_macros.h"
#അന്തര്ഭാവ "portbenchmark.h"
#അന്തര്ഭാവ "esp_macros.h"
#അന്തര്ഭാവ "esp_cpu.h"
#അന്തര്ഭാവ "esp_private/crosscore_int.h"
#അന്തര്ഭാവ "esp_memory_utils.h"

/*
Note: We should not include any FreeRTOS headers (directly or indirectly) here as this will create a reverse dependency
- FreeRTOS pulls in "portmacro.h" via
    - source.c <- FreeRTOS.h <- portable.h <- portmacro.h

- If we need types of task.h (such as TaskHandle_t), best put the feature somewhere else
- Be careful as some parts of C Standard Library Headers will pull in FreeRTOS headers
*/

#ifdef __cplusplus
ബാഹ്യ "C" {
#endif

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come before first as they are used further down the file
 * ------------------------------------------------------------------------------------------------------------------ */

#define portCHAR                    int8_t
#define portFLOAT                   ദശാംശം
#define portDOUBLE                  ഇരട്ട
#define portLONG                    int32_t
#define portSHORT                   int16_t
#define port_അട്ടി_TYPE              uint8_t
#define portBASE_TYPE               പൂർണ്ണസംഖ്യ

തരനിർവചനം port_അട്ടി_TYPE              അട്ടി_Type_t;
തരനിർവചനം portBASE_TYPE               BaseType_t;
തരനിർവചനം unsigned portBASE_TYPE      UBaseType_t;

#എങ്കിൽ( configUSE_16_BIT_TICKS == 1 )
തരനിർവചനം uint16_t TickType_t;
#define portMAX_DELAY ( TickType_t ) 0xffff
#അല്ലെങ്കിൽ
തരനിർവചനം uint32_t TickType_t;
#define portMAX_DELAY ( TickType_t ) 0xffffffffUL
#endif

/* Task function macros as described on the FreeRTOS.org WEB site. */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters )  ശൂന്യം vFunction( ശൂന്യം *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters )        ശൂന്യം vFunction( ശൂന്യം *pvParameters )

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#define portCRITICAL_NESTING_IN_TCB     1
#define port_അട്ടി_GROWTH                ( -1 )
#define portTICK_PERIOD_MS              ( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#define portBYTE_ALIGNMENT              16    // Xtensa Windowed ABI requires the stack pointer to always be 16-byte aligned. See "isa_rm.pdf 8.1.1 Windowed Register Usage and Stack Layout"
#define portNOP()                       XT_NOP()    //Todo: Check if XT_NOP exists

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros need to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Spinlocks ------------------------

തരനിർവചനം spinlock_t                          portMUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */
#define portMUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#define portMUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#define portMUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#define portMUX_TRY_LOCK                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#define portMUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

BaseType_t xPortCheckIfInISR(ശൂന്യം);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortCheckIfInISR() internally
 */
ശൂന്യം vPortAssertIfInISR(ശൂന്യം);

// ------------------ Critical Sections --------------------

UBaseType_t uxPortEnterCriticalFromISR( ശൂന്യം );
ശൂന്യം vPortExitCriticalFromISR( UBaseType_t level );

#എങ്കിൽ ( config_സംഖ്യ_OF_CORES > 1 )
/*
These are always called with interrupts already disabled. We simply need to get/release the spinlocks
*/
ബാഹ്യ portMUX_TYPE port_xTaskLock;
ബാഹ്യ portMUX_TYPE port_xISRLock;

ശൂന്യം vPortTakeLock( portMUX_TYPE *lock );
ശൂന്യം vPortReleaseLock( portMUX_TYPE *lock );
#endif /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

ശൂന്യം vPortYield( ശൂന്യം );
സ്ഥിതം വരിയിൽ ശൂന്യം __attribute__((always_inline)) vPortYieldCore( BaseType_t xCoreID );
സ്ഥിതം വരിയിൽ ശൂന്യം __attribute__((always_inline)) vPortYieldFromISR( ശൂന്യം );

#define portYIELD_FROM_ISR_CHECK(x)     ({ \
    എങ്കിൽ ( (x) == pdTRUE ) { \
        vPortYieldFromISR(); \
    } \
})
#define portYIELD_FROM_ISR_NO_CHECK()               vPortYieldFromISR()

// ----------------------- System --------------------------

സ്ഥിതം വരിയിൽ BaseType_t __attribute__((always_inline)) xPortGetCoreID( ശൂന്യം );

// --------------------- TCB Cleanup -----------------------

/**
 * @brief TCB cleanup hook
 *
 * The portCLEAN_UP_TCB() macro is called in prvDeleteTCB() right before a
 * deleted task's memory is freed. We map that macro to this internal function
 * so that IDF FreeRTOS ports can inject some task pre-deletion operations.
 */
ശൂന്യം vPortTCBPreDeleteHook( ശൂന്യം *pxTCB );


/* ----------------------------------------- FreeRTOS SMP Porting Interface --------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS SMP
 * - Must come after forward declare as some porting macros map to declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

#define portSET_INTERRUPT_MASK()            ({ \
    unsigned പൂർണ്ണസംഖ്യ prev_level = XTOS_SET_INTLEVEL(XCHAL_EXCM_LEVEL); \
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
    unsigned പൂർണ്ണസംഖ്യ ps_reg; \
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

#എങ്കിൽ ( config_സംഖ്യ_OF_CORES > 1 )
#define portGET_TASK_LOCK()                         vPortTakeLock(&port_xTaskLock)
#define portRELEASE_TASK_LOCK()                     vPortReleaseLock(&port_xTaskLock)
#define portGET_ISR_LOCK()                          vPortTakeLock(&port_xISRLock)
#define portRELEASE_ISR_LOCK()                      vPortReleaseLock(&port_xISRLock)
#endif /* configNUMBER_OF_CORES > 1 */

//Critical sections used by FreeRTOS SMP
ബാഹ്യ ശൂന്യം vTaskEnterCritical( ശൂന്യം );
ബാഹ്യ ശൂന്യം vTaskExitCritical( ശൂന്യം );
#define portENTER_CRITICAL_SMP()                    vTaskEnterCritical();
#define portEXIT_CRITICAL_SMP()                     vTaskExitCritical();

#എങ്കിൽ defined(__cplusplus) && (__cplusplus >  201703L)
#define portENTER_CRITICAL(...)                     CHOOSE_MACRO_VA_ARG(portENTER_CRITICAL_IDF, portENTER_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#define portEXIT_CRITICAL(...)                      CHOOSE_MACRO_VA_ARG(portEXIT_CRITICAL_IDF, portEXIT_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#അല്ലെങ്കിൽ
#define portENTER_CRITICAL(...)                     CHOOSE_MACRO_VA_ARG(portENTER_CRITICAL_IDF, portENTER_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#define portEXIT_CRITICAL(...)                      CHOOSE_MACRO_VA_ARG(portEXIT_CRITICAL_IDF, portEXIT_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#endif

ബാഹ്യ UBaseType_t vTaskEnterCriticalFromISR( ശൂന്യം );
ബാഹ്യ ശൂന്യം vTaskExitCriticalFromISR( UBaseType_t uxSavedInterruptStatus );
#define portENTER_CRITICAL_FROM_ISR() vTaskEnterCriticalFromISR()
#define portEXIT_CRITICAL_FROM_ISR(x) vTaskExitCriticalFromISR(x)

// ---------------------- Yielding -------------------------

#define portYIELD()                                 vPortYield()
#എങ്കിൽ defined(__cplusplus) && (__cplusplus >  201703L)
#define portYIELD_FROM_ISR(...)                     CHOOSE_MACRO_VA_ARG(portYIELD_FROM_ISR_CHECK, portYIELD_FROM_ISR_NO_CHECK __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#അല്ലെങ്കിൽ
#define portYIELD_FROM_ISR(...)                     CHOOSE_MACRO_VA_ARG(portYIELD_FROM_ISR_CHECK, portYIELD_FROM_ISR_NO_CHECK, ##__VA_ARGS__)(__VA_ARGS__)
#endif
#define portYIELD_CORE(x)                           vPortYieldCore(x)

// ----------------------- System --------------------------

#define portGET_CORE_ID()                           xPortGetCoreID()
#define portCHECK_IF_IN_ISR()                       xPortCheckIfInISR()

// ------------------- Run Time Stats ----------------------

//Timers are already configured, so nothing to do for configuration of run time stats timer
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()
#എങ്കിൽ ( CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS )
configRUN_TIME_COUNTER_TYPE x_Port_Get_Run_Time_Counter_മൂല്യം( ശൂന്യം );
#define port_GET_RUN_TIME_COUNTER_മൂല്യം()        x_Port_Get_Run_Time_Counter_മൂല്യം()
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

ബാഹ്യ ശൂന്യം _frxt_setup_switch( ശൂന്യം );     //Defined in portasm.S

സ്ഥിതം വരിയിൽ ശൂന്യം __attribute__((always_inline)) vPortYieldCore( BaseType_t xCoreID )
{
    esp_crosscore_int_send_yield( xCoreID );
}

സ്ഥിതം വരിയിൽ ശൂന്യം __attribute__((always_inline)) vPortYieldFromISR( ശൂന്യം )
{
    //traceISR_EXIT_TO_SCHEDULER();
    _frxt_setup_switch();
}

// ----------------------- System --------------------------

സ്ഥിതം വരിയിൽ BaseType_t __attribute__((always_inline)) xPortGetCoreID( ശൂന്യം )
{
    തിരികെ (BaseType_t) esp_cpu_get_core_id();
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

സ്ഥിതം വരിയിൽ BaseType_t xPortInIsrContext(ശൂന്യം)
{
    //Just call the FreeRTOS port interface version
    തിരികെ xPortCheckIfInISR();
}

BaseType_t xPortInterruptedFromISRContext(ശൂന്യം);

സ്ഥിതം വരിയിൽ UBaseType_t xPortSetInterruptMaskFromISR(ശൂന്യം)
{
    UBaseType_t prev_int_level = XTOS_SET_INTLEVEL(XCHAL_EXCM_LEVEL);
    portbenchmarkINTERRUPT_DISABLE();
    തിരികെ prev_int_level;
}

സ്ഥിതം വരിയിൽ ശൂന്യം vPortClearInterruptMaskFromISR(UBaseType_t prev_level)
{
    portbenchmarkINTERRUPT_RESTORE(prev_level);
    XTOS_RESTORE_JUST_INTLEVEL((പൂർണ്ണസംഖ്യ) prev_level);
}

// ---------------------- Spinlocks ------------------------

സ്ഥിതം വരിയിൽ bool __attribute__((always_inline)) vPortCPUAcquireMutexTimeout(portMUX_TYPE *mux, പൂർണ്ണസംഖ്യ timeout)
{
    തിരികെ (spinlock_acquire(mux, timeout));
}

സ്ഥിതം വരിയിൽ ശൂന്യം __attribute__((always_inline)) vPortCPUReleaseMutex(portMUX_TYPE *mux)
{
    spinlock_release(mux);
}

// ------------------ Critical Sections --------------------

#എങ്കിൽ CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
/**
 * @brief Claim thread-safe region start
 *        If claimed, vPortEnterCritical/vPortExitCritical on the current core are no-ops.
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
ശൂന്യം xPortThreadSafeClaim(ശൂന്യം);

/**
 * @brief Claim thread-safe region end
 *        Restores normal port critical behavior
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
ശൂന്യം xPortThreadSafeDisclaim(ശൂന്യം);

ബാഹ്യ അസ്ഥിര bool port_xThreadSafeClaimed;
#endif /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

BaseType_t xPortEnterCriticalTimeout(portMUX_TYPE *lock, BaseType_t timeout);

സ്ഥിതം വരിയിൽ ശൂന്യം __attribute__((always_inline)) vPortEnterCriticalIDF(portMUX_TYPE *lock)
{
    xPortEnterCriticalTimeout(lock, portMUX_NO_TIMEOUT);
}

ശൂന്യം vPortExitCriticalIDF(portMUX_TYPE *lock);

/*
In case any IDF libs called the port critical functions directly instead of through the macros.
The will just call the vPort...CriticalIDF() functions
*/
ശൂന്യം vPortEnterCritical(portMUX_TYPE *lock);
ശൂന്യം vPortExitCritical(portMUX_TYPE *lock);

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

സ്ഥിതം വരിയിൽ bool xPortCanYield(ശൂന്യം)
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

    തിരികെ ((ps_reg & PS_INTLEVEL_MASK) == 0);
}

// Defined even for configNUMBER_OF_CORES > 1 for IDF compatibility
#define portYIELD_WITHIN_API()                      esp_crosscore_int_send_yield(xPortGetCoreID())

// ----------------------- System --------------------------

ശൂന്യം v_Port_Set_അട്ടി_Watchpoint(ശൂന്യം *px_അട്ടി_Start);

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
bool xPortCheckValidListMem(സ്ഥിരം ശൂന്യം *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool xPortCheckValidTCBMem(സ്ഥിരം ശൂന്യം *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_അട്ടി_Mem(സ്ഥിരം ശൂന്യം *ptr);

#define portVALID_LIST_MEM(ptr)     xPortCheckValidListMem(ptr)
#define portVALID_TCB_MEM(ptr)      xPortCheckValidTCBMem(ptr)
#define port_VALID_അട്ടി_MEM(ptr)    x_Portcheck_Valid_അട്ടി_Mem(ptr)

/* ------------------------------------------------------ Misc ---------------------------------------------------------
 * - Miscellaneous porting macros
 * - These are not part of the FreeRTOS porting interface, but are used by other FreeRTOS dependent components
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- App-Trace -------------------------

#എങ്കിൽ CONFIG_ESP_TRACE_ENABLE
ബാഹ്യ അസ്ഥിര BaseType_t xPortSwitchFlag;
#define os_task_switch_is_pended(_cpu_) (xPortSwitchFlag)
#അല്ലെങ്കിൽ
#define os_task_switch_is_pended(_cpu_) (false)
#endif

// --------------- Compatibility Includes ------------------
/*
ESP-IDF currently does not have a "Include what you use" policy. A lot of files implicitly pull in API through
portmacro.h. Therefore, we need to keep these headers around for now to allow the rest of IDF to compile.

[refactor-todo] Clean up ESP-IDF inclusion dependencies and add a inclusion check.
*/

#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <stdbool.h>
#അന്തര്ഭാവ <stdarg.h>
#അന്തര്ഭാവ <xtensa/hal.h>
#അന്തര്ഭാവ "esp_attr.h"
#അന്തര്ഭാവ "esp_newlib.h"
#അന്തര്ഭാവ "esp_heap_caps.h"
#അന്തര്ഭാവ "esp_rom_sys.h"
#അന്തര്ഭാവ "esp_system.h"
#അന്തര്ഭാവ "portbenchmark.h"
#അന്തര്ഭാവ <limits.h>
#അന്തര്ഭാവ <xtensa/config/system.h>
#അന്തര്ഭാവ <xtensa_api.h>

#ifdef __cplusplus
}
#endif
