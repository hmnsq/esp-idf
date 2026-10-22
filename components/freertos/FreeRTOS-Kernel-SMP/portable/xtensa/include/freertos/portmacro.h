/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#அந்தர்பாவ "sdkconfig.h"
#அந்தர்பாவ <stdint.h>
#அந்தர்பாவ "spinlock.h"
#அந்தர்பாவ "xtensa/xtruntime.h"
#அந்தர்பாவ "xtensa/config/core.h"
#அந்தர்பாவ "xtensa/config/core-isa.h"
#அந்தர்பாவ "xtensa/config/xt_specreg.h"
#அந்தர்பாவ "xt_instr_macros.h"
#அந்தர்பாவ "portbenchmark.h"
#அந்தர்பாவ "esp_macros.h"
#அந்தர்பாவ "esp_cpu.h"
#அந்தர்பாவ "esp_private/crosscore_int.h"
#அந்தர்பாவ "esp_memory_utils.h"

/*
Note: We should not include any FreeRTOS headers (directly or indirectly) here as this will create a reverse dependency
- FreeRTOS pulls in "portmacro.h" via
    - source.c <- FreeRTOS.h <- portable.h <- portmacro.h

- If we need types of task.h (such as TaskHandle_t), best put the feature somewhere else
- Be careful as some parts of C Standard Library Headers will pull in FreeRTOS headers
*/

#ifdef __cplusplus
வெளி "C" {
#endif

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come before first as they are used further down the file
 * ------------------------------------------------------------------------------------------------------------------ */

#define portCHAR                    int8_t
#define portFLOAT                   தசம
#define portDOUBLE                  இரட்டை
#define portLONG                    int32_t
#define portSHORT                   int16_t
#define port_அடுக்கு_TYPE              uint8_t
#define portBASE_TYPE               முழு

வகைவரை port_அடுக்கு_TYPE              அடுக்கு_Type_t;
வகைவரை portBASE_TYPE               BaseType_t;
வகைவரை unsigned portBASE_TYPE      UBaseType_t;

#எனில்( configUSE_16_BIT_TICKS == 1 )
வகைவரை uint16_t TickType_t;
#define portMAX_DELAY ( TickType_t ) 0xffff
#இல்லையெனில்
வகைவரை uint32_t TickType_t;
#define portMAX_DELAY ( TickType_t ) 0xffffffffUL
#endif

/* Task function macros as described on the FreeRTOS.org WEB site. */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters )  வெறுமை vFunction( வெறுமை *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters )        வெறுமை vFunction( வெறுமை *pvParameters )

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#define portCRITICAL_NESTING_IN_TCB     1
#define port_அடுக்கு_GROWTH                ( -1 )
#define portTICK_PERIOD_MS              ( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#define portBYTE_ALIGNMENT              16    // Xtensa Windowed ABI requires the stack pointer to always be 16-byte aligned. See "isa_rm.pdf 8.1.1 Windowed Register Usage and Stack Layout"
#define portNOP()                       XT_NOP()    //Todo: Check if XT_NOP exists

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros need to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Spinlocks ------------------------

வகைவரை spinlock_t                          portMUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */
#define portMUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#define portMUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#define portMUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#define portMUX_TRY_LOCK                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#define portMUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

BaseType_t xPortCheckIfInISR(வெறுமை);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortCheckIfInISR() internally
 */
வெறுமை vPortAssertIfInISR(வெறுமை);

// ------------------ Critical Sections --------------------

UBaseType_t uxPortEnterCriticalFromISR( வெறுமை );
வெறுமை vPortExitCriticalFromISR( UBaseType_t level );

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
நிலையான உள்வரி வெறுமை __attribute__((always_inline)) vPortYieldFromISR( வெறுமை );

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


/* ----------------------------------------- FreeRTOS SMP Porting Interface --------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS SMP
 * - Must come after forward declare as some porting macros map to declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

#define portSET_INTERRUPT_MASK()            ({ \
    unsigned முழு prev_level = XTOS_SET_INTLEVEL(XCHAL_EXCM_LEVEL); \
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
    unsigned முழு ps_reg; \
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

வெளி UBaseType_t vTaskEnterCriticalFromISR( வெறுமை );
வெளி வெறுமை vTaskExitCriticalFromISR( UBaseType_t uxSavedInterruptStatus );
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

//Timers are already configured, so nothing to do for configuration of run time stats timer
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
 * - For implementation of non-inlined functions, see port.c
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

// ------------------ Critical Sections --------------------

// ---------------------- Yielding -------------------------

வெளி வெறுமை _frxt_setup_switch( வெறுமை );     //Defined in portasm.S

நிலையான உள்வரி வெறுமை __attribute__((always_inline)) vPortYieldCore( BaseType_t xCoreID )
{
    esp_crosscore_int_send_yield( xCoreID );
}

நிலையான உள்வரி வெறுமை __attribute__((always_inline)) vPortYieldFromISR( வெறுமை )
{
    //traceISR_EXIT_TO_SCHEDULER();
    _frxt_setup_switch();
}

// ----------------------- System --------------------------

நிலையான உள்வரி BaseType_t __attribute__((always_inline)) xPortGetCoreID( வெறுமை )
{
    திரும்பு (BaseType_t) esp_cpu_get_core_id();
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

நிலையான உள்வரி BaseType_t xPortInIsrContext(வெறுமை)
{
    //Just call the FreeRTOS port interface version
    திரும்பு xPortCheckIfInISR();
}

BaseType_t xPortInterruptedFromISRContext(வெறுமை);

நிலையான உள்வரி UBaseType_t xPortSetInterruptMaskFromISR(வெறுமை)
{
    UBaseType_t prev_int_level = XTOS_SET_INTLEVEL(XCHAL_EXCM_LEVEL);
    portbenchmarkINTERRUPT_DISABLE();
    திரும்பு prev_int_level;
}

நிலையான உள்வரி வெறுமை vPortClearInterruptMaskFromISR(UBaseType_t prev_level)
{
    portbenchmarkINTERRUPT_RESTORE(prev_level);
    XTOS_RESTORE_JUST_INTLEVEL((முழு) prev_level);
}

// ---------------------- Spinlocks ------------------------

நிலையான உள்வரி bool __attribute__((always_inline)) vPortCPUAcquireMutexTimeout(portMUX_TYPE *mux, முழு timeout)
{
    திரும்பு (spinlock_acquire(mux, timeout));
}

நிலையான உள்வரி வெறுமை __attribute__((always_inline)) vPortCPUReleaseMutex(portMUX_TYPE *mux)
{
    spinlock_release(mux);
}

// ------------------ Critical Sections --------------------

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

BaseType_t xPortEnterCriticalTimeout(portMUX_TYPE *lock, BaseType_t timeout);

நிலையான உள்வரி வெறுமை __attribute__((always_inline)) vPortEnterCriticalIDF(portMUX_TYPE *lock)
{
    xPortEnterCriticalTimeout(lock, portMUX_NO_TIMEOUT);
}

வெறுமை vPortExitCriticalIDF(portMUX_TYPE *lock);

/*
In case any IDF libs called the port critical functions directly instead of through the macros.
The will just call the vPort...CriticalIDF() functions
*/
வெறுமை vPortEnterCritical(portMUX_TYPE *lock);
வெறுமை vPortExitCritical(portMUX_TYPE *lock);

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

நிலையான உள்வரி bool xPortCanYield(வெறுமை)
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

    திரும்பு ((ps_reg & PS_INTLEVEL_MASK) == 0);
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
#அந்தர்பாவ <xtensa/hal.h>
#அந்தர்பாவ "esp_attr.h"
#அந்தர்பாவ "esp_newlib.h"
#அந்தர்பாவ "esp_heap_caps.h"
#அந்தர்பாவ "esp_rom_sys.h"
#அந்தர்பாவ "esp_system.h"
#அந்தர்பாவ "portbenchmark.h"
#அந்தர்பாவ <limits.h>
#அந்தர்பாவ <xtensa/config/system.h>
#அந்தர்பாவ <xtensa_api.h>

#ifdef __cplusplus
}
#endif
