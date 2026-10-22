/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#هدايت once

#ضمن "sdkconfig.h"
#ضمن <stdint.h>
#ضمن "spinlock.h"
#ضمن "xtensa/xtruntime.h"
#ضمن "xtensa/config/core.h"
#ضمن "xtensa/config/core-isa.h"
#ضمن "xtensa/config/xt_specreg.h"
#ضمن "xt_instr_macros.h"
#ضمن "portbenchmark.h"
#ضمن "esp_macros.h"
#ضمن "esp_cpu.h"
#ضمن "esp_private/crosscore_int.h"
#ضمن "esp_memory_utils.h"

/*
Note: We should not include any FreeRTOS headers (directly or indirectly) here as this will create a reverse dependency
- FreeRTOS pulls in "portmacro.h" via
    - source.c <- FreeRTOS.h <- portable.h <- portmacro.h

- If we need types of task.h (such as TaskHandle_t), best put the feature somewhere else
- Be careful as some parts of C Standard Library Headers will pull in FreeRTOS headers
*/

#جيڪڏهن_وصف_ٿيل __cplusplus
خارجي "C" {
#جيڪڏهن_پورو

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come before first as they are used further down the file
 * ------------------------------------------------------------------------------------------------------------------ */

#وصف_ڪريو پورٽ_CHAR                    int8_t
#وصف_ڪريو پورٽ_FLOAT                   عائم
#وصف_ڪريو پورٽ_DOUBLE                  مزدوج
#وصف_ڪريو پورٽ_LONG                    int32_t
#وصف_ڪريو پورٽ_SHORT                   int16_t
#وصف_ڪريو پورٽ_ڍير_TYPE              uint8_t
#وصف_ڪريو پورٽ_بنياد_TYPE               صحيح

تعريفنوع پورٽ_ڍير_TYPE              ڍير_Type_t;
تعريفنوع پورٽ_بنياد_TYPE               بنياد_Type_t;
تعريفنوع بنانشان پورٽ_بنياد_TYPE      UBaseType_t;

#إذا( config_USE_16_ٻٽو_انگ_TICKS == 1 )
تعريفنوع uint16_t TickType_t;
#وصف_ڪريو پورٽ_MAX_DELAY ( TickType_t ) 0xffff
#وإلا
تعريفنوع uint32_t TickType_t;
#وصف_ڪريو پورٽ_MAX_DELAY ( TickType_t ) 0xffffffffUL
#جيڪڏهن_پورو

/* Task function macros as described on the FreeRTOS.org WEB site. */
#وصف_ڪريو پورٽ_TASK_FUNCTION_PROTO( vFunction, pvParameters )  فراغ vFunction( فراغ *pvParameters )
#وصف_ڪريو پورٽ_TASK_FUNCTION( vFunction, pvParameters )        فراغ vFunction( فراغ *pvParameters )

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#وصف_ڪريو پورٽ_CRITICAL_NESTING_اندريون_TCB     1
#وصف_ڪريو پورٽ_ڍير_GROWTH                ( -1 )
#وصف_ڪريو پورٽ_TICK_PERIOD_MS              ( ( TickType_t ) 1000 / config_TICK_شرح_HZ )
#وصف_ڪريو پورٽ_BYTE_ALIGNMENT              16    // Xtensa Windowed ABI requires the stack pointer to always be 16-byte aligned. See "isa_rm.pdf 8.1.1 Windowed Register Usage and Stack Layout"
#وصف_ڪريو پورٽ_NOP()                       XT_NOP()    //Todo: Check if XT_NOP exists

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros need to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Spinlocks ------------------------

تعريفنوع spinlock_t                          پورٽ_MUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */
#وصف_ڪريو پورٽ_MUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#وصف_ڪريو پورٽ_MUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#وصف_ڪريو پورٽ_MUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#وصف_ڪريو پورٽ_MUX_TRY_تالو                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#وصف_ڪريو پورٽ_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

بنياد_Type_t x_پورٽ_Check_If_اندريون_ISR(فراغ);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortCheckIfInISR() internally
 */
فراغ v_پورٽ_Assert_If_اندريون_ISR(فراغ);

// ------------------ Critical Sections --------------------

UBaseType_t ux_پورٽ_Enter_Critical_From_ISR( فراغ );
فراغ v_پورٽ_Exit_Critical_From_ISR( UBaseType_t level );

#إذا ( config_عدد_جو_CORES > 1 )
/*
These are always called with interrupts already disabled. We simply need to get/release the spinlocks
*/
خارجي پورٽ_MUX_TYPE پورٽ_x_Task_تالو;
خارجي پورٽ_MUX_TYPE پورٽ_x_ISRLock;

فراغ v_پورٽ_Take_تالو( پورٽ_MUX_TYPE *تالو );
فراغ v_پورٽ_Release_تالو( پورٽ_MUX_TYPE *تالو );
#جيڪڏهن_پورو /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

فراغ v_پورٽ_Yield( فراغ );
ساكن مضمن فراغ __attribute__((always_inline)) v_پورٽ_Yield_Core( بنياد_Type_t xCoreID );
ساكن مضمن فراغ __attribute__((always_inline)) v_پورٽ_Yield_From_ISR( فراغ );

#وصف_ڪريو پورٽ_YIELD_FROM_ISR_CHECK(x)     ({ \
    إذا ( (x) == pdTRUE ) { \
        v_پورٽ_Yield_From_ISR(); \
    } \
})
#وصف_ڪريو پورٽ_YIELD_FROM_ISR_NO_CHECK()               v_پورٽ_Yield_From_ISR()

// ----------------------- System --------------------------

ساكن مضمن بنياد_Type_t __attribute__((always_inline)) x_پورٽ_Get_Core_ID( فراغ );

// --------------------- TCB Cleanup -----------------------

/**
 * @brief TCB cleanup hook
 *
 * The portCLEAN_UP_TCB() macro is called in prvDeleteTCB() right before a
 * deleted task's memory is freed. We map that macro to this internal function
 * so that IDF FreeRTOS ports can inject some task pre-deletion operations.
 */
فراغ v_پورٽ_TCBPre_Delete_Hook( فراغ *pxTCB );


/* ----------------------------------------- FreeRTOS SMP Porting Interface --------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS SMP
 * - Must come after forward declare as some porting macros map to declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

#وصف_ڪريو پورٽ_سيٽ_مداخلت_نقاب()            ({ \
    بنانشان صحيح prev_level = XTOS_سيٽ_INTLEVEL(XCHAL_EXCM_LEVEL); \
    portbenchmark_مداخلت_DISABLE(); \
    prev_level = ((prev_level >> XCHAL_PS_INTLEVEL_سرڪايو) & XCHAL_PS_INTLEVEL_نقاب); \
    prev_level; \
})
#وصف_ڪريو پورٽ_سيٽ_مداخلت_نقاب_FROM_ISR()   پورٽ_سيٽ_مداخلت_نقاب()
#وصف_ڪريو پورٽ_DISABLE_INTERRUPTS()            پورٽ_سيٽ_مداخلت_نقاب()

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
#وصف_ڪريو پورٽ_CLEAR_مداخلت_نقاب(x)         ({ \
    بنانشان صحيح ps_reg; \
    RSR(XT_REG_PS, ps_reg); \
    ps_reg = (ps_reg & ~XCHAL_PS_INTLEVEL_نقاب); \
    ps_reg |= ((x << XCHAL_PS_INTLEVEL_سرڪايو) & XCHAL_PS_INTLEVEL_نقاب); \
    XTOS_RESTORE_INTLEVEL(ps_reg); \
})
#وصف_ڪريو پورٽ_CLEAR_مداخلت_نقاب_FROM_ISR(x)    پورٽ_CLEAR_مداخلت_نقاب(x)
#وصف_ڪريو پورٽ_ENABLE_INTERRUPTS()     ({ \
    portbenchmark_مداخلت_RESTORE(0); \
    XTOS_سيٽ_INTLEVEL(0); \
})

/* Compatibility */
#وصف_ڪريو پورٽ_RESTORE_INTERRUPTS(x)   پورٽ_CLEAR_مداخلت_نقاب(x)

// ------------------ Critical Sections --------------------

#إذا ( config_عدد_جو_CORES > 1 )
#وصف_ڪريو پورٽ_GET_TASK_تالو()                         v_پورٽ_Take_تالو(&پورٽ_x_Task_تالو)
#وصف_ڪريو پورٽ_RELEASE_TASK_تالو()                     v_پورٽ_Release_تالو(&پورٽ_x_Task_تالو)
#وصف_ڪريو پورٽ_GET_ISR_تالو()                          v_پورٽ_Take_تالو(&پورٽ_x_ISRLock)
#وصف_ڪريو پورٽ_RELEASE_ISR_تالو()                      v_پورٽ_Release_تالو(&پورٽ_x_ISRLock)
#جيڪڏهن_پورو /* configNUMBER_OF_CORES > 1 */

//Critical sections used by FreeRTOS SMP
خارجي فراغ vTaskEnterCritical( فراغ );
خارجي فراغ vTaskExitCritical( فراغ );
#وصف_ڪريو پورٽ_ENTER_CRITICAL_SMP()                    vTaskEnterCritical();
#وصف_ڪريو پورٽ_EXIT_CRITICAL_SMP()                     vTaskExitCritical();

#إذا defined(__cplusplus) && (__cplusplus >  201703L)
#وصف_ڪريو پورٽ_ENTER_CRITICAL(...)                     چونڊيو_MACRO_VA_ARG(پورٽ_ENTER_CRITICAL_IDF, پورٽ_ENTER_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#وصف_ڪريو پورٽ_EXIT_CRITICAL(...)                      چونڊيو_MACRO_VA_ARG(پورٽ_EXIT_CRITICAL_IDF, پورٽ_EXIT_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#وإلا
#وصف_ڪريو پورٽ_ENTER_CRITICAL(...)                     چونڊيو_MACRO_VA_ARG(پورٽ_ENTER_CRITICAL_IDF, پورٽ_ENTER_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#وصف_ڪريو پورٽ_EXIT_CRITICAL(...)                      چونڊيو_MACRO_VA_ARG(پورٽ_EXIT_CRITICAL_IDF, پورٽ_EXIT_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#جيڪڏهن_پورو

خارجي UBaseType_t vTaskEnterCriticalFromISR( فراغ );
خارجي فراغ vTaskExitCriticalFromISR( UBaseType_t ux_Saved_مداخلت_حالت );
#وصف_ڪريو پورٽ_ENTER_CRITICAL_FROM_ISR() vTaskEnterCriticalFromISR()
#وصف_ڪريو پورٽ_EXIT_CRITICAL_FROM_ISR(x) vTaskExitCriticalFromISR(x)

// ---------------------- Yielding -------------------------

#وصف_ڪريو پورٽ_YIELD()                                 v_پورٽ_Yield()
#إذا defined(__cplusplus) && (__cplusplus >  201703L)
#وصف_ڪريو پورٽ_YIELD_FROM_ISR(...)                     چونڊيو_MACRO_VA_ARG(پورٽ_YIELD_FROM_ISR_CHECK, پورٽ_YIELD_FROM_ISR_NO_CHECK __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#وإلا
#وصف_ڪريو پورٽ_YIELD_FROM_ISR(...)                     چونڊيو_MACRO_VA_ARG(پورٽ_YIELD_FROM_ISR_CHECK, پورٽ_YIELD_FROM_ISR_NO_CHECK, ##__VA_ARGS__)(__VA_ARGS__)
#جيڪڏهن_پورو
#وصف_ڪريو پورٽ_YIELD_CORE(x)                           v_پورٽ_Yield_Core(x)

// ----------------------- System --------------------------

#وصف_ڪريو پورٽ_GET_CORE_ID()                           x_پورٽ_Get_Core_ID()
#وصف_ڪريو پورٽ_CHECK_IF_اندريون_ISR()                       x_پورٽ_Check_If_اندريون_ISR()

// ------------------- Run Time Stats ----------------------

//Timers are already configured, so nothing to do for configuration of run time stats timer
#وصف_ڪريو پورٽ_CONFIGURE_TIMER_FOR_لاڳيتو_وقت_STATS()
#إذا ( CONFIG_FREERTOS_GENERATE_لاڳيتو_وقت_STATS )
config_لاڳيتو_وقت_COUNTER_TYPE x_پورٽ_Get_لاڳيتو_وقت_Counter_قدر( فراغ );
#وصف_ڪريو پورٽ_GET_لاڳيتو_وقت_COUNTER_قدر()        x_پورٽ_Get_لاڳيتو_وقت_Counter_قدر()
#جيڪڏهن_پورو

// --------------------- TCB Cleanup -----------------------

#وصف_ڪريو پورٽ_CLEAN_مٿي_TCB( pxTCB )                   v_پورٽ_TCBPre_Delete_Hook( pxTCB )

/* --------------------------------------------- Inline Implementations ------------------------------------------------
 * - Implementation of inline functions of the forward declares
 * - Should come after forward declare and FreeRTOS Porting interface, as implementation may use both.
 * - For implementation of non-inlined functions, see port.c
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

// ------------------ Critical Sections --------------------

// ---------------------- Yielding -------------------------

خارجي فراغ frxt_تياري_switch( فراغ );     //Defined in portasm.S

ساكن مضمن فراغ __attribute__((always_inline)) v_پورٽ_Yield_Core( بنياد_Type_t xCoreID )
{
    esp_crosscore_int_send_yield( xCoreID );
}

ساكن مضمن فراغ __attribute__((always_inline)) v_پورٽ_Yield_From_ISR( فراغ )
{
    //traceISR_EXIT_TO_SCHEDULER();
    frxt_تياري_switch();
}

// ----------------------- System --------------------------

ساكن مضمن بنياد_Type_t __attribute__((always_inline)) x_پورٽ_Get_Core_ID( فراغ )
{
    أرجع (بنياد_Type_t) esp_cpu_get_core_id();
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

ساكن مضمن بنياد_Type_t x_پورٽ_اندريون_Isr_Context(فراغ)
{
    //Just call the FreeRTOS port interface version
    أرجع x_پورٽ_Check_If_اندريون_ISR();
}

بنياد_Type_t x_پورٽ_Interrupted_From_ISRContext(فراغ);

ساكن مضمن UBaseType_t x_پورٽ_سيٽ_مداخلت_نقاب_From_ISR(فراغ)
{
    UBaseType_t prev_int_level = XTOS_سيٽ_INTLEVEL(XCHAL_EXCM_LEVEL);
    portbenchmark_مداخلت_DISABLE();
    أرجع prev_int_level;
}

ساكن مضمن فراغ v_پورٽ_Clear_مداخلت_نقاب_From_ISR(UBaseType_t prev_level)
{
    portbenchmark_مداخلت_RESTORE(prev_level);
    XTOS_RESTORE_JUST_INTLEVEL((صحيح) prev_level);
}

// ---------------------- Spinlocks ------------------------

ساكن مضمن bool __attribute__((always_inline)) v_پورٽ_CPUAcquire_Mutex_Timeout(پورٽ_MUX_TYPE *mux, صحيح timeout)
{
    أرجع (spinlock_acquire(mux, timeout));
}

ساكن مضمن فراغ __attribute__((always_inline)) v_پورٽ_CPURelease_Mutex(پورٽ_MUX_TYPE *mux)
{
    spinlock_release(mux);
}

// ------------------ Critical Sections --------------------

#إذا CONFIG_FREERTOS_پورٽ_THREAD_محفوظ_CLAIM
/**
 * @brief Claim thread-safe region start
 *        If claimed, vPortEnterCritical/vPortExitCritical on the current core are no-ops.
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
فراغ x_پورٽ_Thread_محفوظ_Claim(فراغ);

/**
 * @brief Claim thread-safe region end
 *        Restores normal port critical behavior
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
فراغ x_پورٽ_Thread_محفوظ_Disclaim(فراغ);

خارجي متطاير bool پورٽ_x_Thread_محفوظ_Claimed;
#جيڪڏهن_پورو /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

بنياد_Type_t x_پورٽ_Enter_Critical_Timeout(پورٽ_MUX_TYPE *تالو, بنياد_Type_t timeout);

ساكن مضمن فراغ __attribute__((always_inline)) v_پورٽ_Enter_Critical_IDF(پورٽ_MUX_TYPE *تالو)
{
    x_پورٽ_Enter_Critical_Timeout(تالو, پورٽ_MUX_NO_TIMEOUT);
}

فراغ v_پورٽ_Exit_Critical_IDF(پورٽ_MUX_TYPE *تالو);

/*
In case any IDF libs called the port critical functions directly instead of through the macros.
The will just call the vPort...CriticalIDF() functions
*/
فراغ v_پورٽ_Enter_Critical(پورٽ_MUX_TYPE *تالو);
فراغ v_پورٽ_Exit_Critical(پورٽ_MUX_TYPE *تالو);

//IDF task critical sections
#وصف_ڪريو پورٽ_TRY_ENTER_CRITICAL(تالو, timeout)       x_پورٽ_Enter_Critical_Timeout(تالو, timeout)
#وصف_ڪريو پورٽ_ENTER_CRITICAL_IDF(تالو)                v_پورٽ_Enter_Critical_IDF(تالو)
#وصف_ڪريو پورٽ_EXIT_CRITICAL_IDF(تالو)                 v_پورٽ_Exit_Critical_IDF(تالو)
//IDF ISR critical sections
#وصف_ڪريو پورٽ_TRY_ENTER_CRITICAL_ISR(تالو, timeout)   x_پورٽ_Enter_Critical_Timeout(تالو, timeout)
#وصف_ڪريو پورٽ_ENTER_CRITICAL_ISR(تالو)                v_پورٽ_Enter_Critical_IDF(تالو)
#وصف_ڪريو پورٽ_EXIT_CRITICAL_ISR(تالو)                 v_پورٽ_Exit_Critical_IDF(تالو)
//IDF safe critical sections (they're the same)
#وصف_ڪريو پورٽ_ENTER_CRITICAL_محفوظ(تالو)               v_پورٽ_Enter_Critical_IDF(تالو)
#وصف_ڪريو پورٽ_EXIT_CRITICAL_محفوظ(تالو)                v_پورٽ_Exit_Critical_IDF(تالو)

// ---------------------- Yielding -------------------------

ساكن مضمن bool x_پورٽ_Can_Yield(فراغ)
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

    أرجع ((ps_reg & PS_INTLEVEL_نقاب) == 0);
}

// Defined even for configNUMBER_OF_CORES > 1 for IDF compatibility
#وصف_ڪريو پورٽ_YIELD_WITHIN_API()                      esp_crosscore_int_send_yield(x_پورٽ_Get_Core_ID())

// ----------------------- System --------------------------

فراغ v_پورٽ_سيٽ_ڍير_Watchpoint(فراغ *px_ڍير_شروعات);

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
bool x_پورٽ_Check_Valid_List_Mem(ثابت فراغ *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool x_پورٽ_Check_Valid_TCBMem(ثابت فراغ *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_ڍير_Mem(ثابت فراغ *ptr);

#وصف_ڪريو پورٽ_VALID_LIST_MEM(ptr)     x_پورٽ_Check_Valid_List_Mem(ptr)
#وصف_ڪريو پورٽ_VALID_TCB_MEM(ptr)      x_پورٽ_Check_Valid_TCBMem(ptr)
#وصف_ڪريو پورٽ_VALID_ڍير_MEM(ptr)    x_Portcheck_Valid_ڍير_Mem(ptr)

/* ------------------------------------------------------ Misc ---------------------------------------------------------
 * - Miscellaneous porting macros
 * - These are not part of the FreeRTOS porting interface, but are used by other FreeRTOS dependent components
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- App-Trace -------------------------

#إذا CONFIG_ESP_TRACE_ENABLE
خارجي متطاير بنياد_Type_t x_پورٽ_Switch_Flag;
#وصف_ڪريو os_task_switch_is_pended(_cpu_) (x_پورٽ_Switch_Flag)
#وإلا
#وصف_ڪريو os_task_switch_is_pended(_cpu_) (false)
#جيڪڏهن_پورو

// --------------- Compatibility Includes ------------------
/*
ESP-IDF currently does not have a "Include what you use" policy. A lot of files implicitly pull in API through
portmacro.h. Therefore, we need to keep these headers around for now to allow the rest of IDF to compile.

[refactor-todo] Clean up ESP-IDF inclusion dependencies and add a inclusion check.
*/

#ضمن <stdlib.h>
#ضمن <stdbool.h>
#ضمن <stdarg.h>
#ضمن <xtensa/hal.h>
#ضمن "esp_attr.h"
#ضمن "esp_newlib.h"
#ضمن "esp_heap_caps.h"
#ضمن "esp_rom_sys.h"
#ضمن "esp_system.h"
#ضمن "portbenchmark.h"
#ضمن <limits.h>
#ضمن <xtensa/config/system.h>
#ضمن <xtensa_api.h>

#جيڪڏهن_وصف_ٿيل __cplusplus
}
#جيڪڏهن_پورو
