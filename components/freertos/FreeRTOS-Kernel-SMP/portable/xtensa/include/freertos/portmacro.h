/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#لارښوونه once

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

#که_تعریف_شوی __cplusplus
خارجي "C" {
#د_که_پای

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come before first as they are used further down the file
 * ------------------------------------------------------------------------------------------------------------------ */

#تعریف_کړه پورټ_CHAR                    int8_t
#تعریف_کړه پورټ_FLOAT                   عائم
#تعریف_کړه پورټ_DOUBLE                  مزدوج
#تعریف_کړه پورټ_LONG                    int32_t
#تعریف_کړه پورټ_SHORT                   int16_t
#تعریف_کړه پورټ_ډېری_TYPE              uint8_t
#تعریف_کړه پورټ_بنسټ_TYPE               صحيح

تعريفنوع پورټ_ډېری_TYPE              ډېری_Type_t;
تعريفنوع پورټ_بنسټ_TYPE               بنسټ_Type_t;
تعريفنوع بینښه پورټ_بنسټ_TYPE      UBaseType_t;

#إذا( config_USE_16_بټ_TICKS == 1 )
تعريفنوع uint16_t TickType_t;
#تعریف_کړه پورټ_MAX_DELAY ( TickType_t ) 0xffff
#وإلا
تعريفنوع uint32_t TickType_t;
#تعریف_کړه پورټ_MAX_DELAY ( TickType_t ) 0xffffffffUL
#د_که_پای

/* Task function macros as described on the FreeRTOS.org WEB site. */
#تعریف_کړه پورټ_TASK_FUNCTION_PROTO( vFunction, pvParameters )  فراغ vFunction( فراغ *pvParameters )
#تعریف_کړه پورټ_TASK_FUNCTION( vFunction, pvParameters )        فراغ vFunction( فراغ *pvParameters )

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#تعریف_کړه پورټ_CRITICAL_NESTING_دننه_TCB     1
#تعریف_کړه پورټ_ډېری_GROWTH                ( -1 )
#تعریف_کړه پورټ_TICK_PERIOD_MS              ( ( TickType_t ) 1000 / config_TICK_شرح_HZ )
#تعریف_کړه پورټ_BYTE_ALIGNMENT              16    // Xtensa Windowed ABI requires the stack pointer to always be 16-byte aligned. See "isa_rm.pdf 8.1.1 Windowed Register Usage and Stack Layout"
#تعریف_کړه پورټ_NOP()                       XT_NOP()    //Todo: Check if XT_NOP exists

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros need to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Spinlocks ------------------------

تعريفنوع spinlock_t                          پورټ_MUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */
#تعریف_کړه پورټ_MUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#تعریف_کړه پورټ_MUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#تعریف_کړه پورټ_MUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#تعریف_کړه پورټ_MUX_TRY_قلف                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#تعریف_کړه پورټ_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

بنسټ_Type_t x_پورټ_Check_If_دننه_ISR(فراغ);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortCheckIfInISR() internally
 */
فراغ v_پورټ_Assert_If_دننه_ISR(فراغ);

// ------------------ Critical Sections --------------------

UBaseType_t ux_پورټ_Enter_Critical_From_ISR( فراغ );
فراغ v_پورټ_Exit_Critical_From_ISR( UBaseType_t level );

#إذا ( config_شمېره_د_CORES > 1 )
/*
These are always called with interrupts already disabled. We simply need to get/release the spinlocks
*/
خارجي پورټ_MUX_TYPE پورټ_x_Task_قلف;
خارجي پورټ_MUX_TYPE پورټ_x_ISRLock;

فراغ v_پورټ_Take_قلف( پورټ_MUX_TYPE *قلف );
فراغ v_پورټ_Release_قلف( پورټ_MUX_TYPE *قلف );
#د_که_پای /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

فراغ v_پورټ_Yield( فراغ );
ساكن مضمن فراغ __attribute__((always_inline)) v_پورټ_Yield_Core( بنسټ_Type_t xCoreID );
ساكن مضمن فراغ __attribute__((always_inline)) v_پورټ_Yield_From_ISR( فراغ );

#تعریف_کړه پورټ_YIELD_FROM_ISR_CHECK(x)     ({ \
    إذا ( (x) == pdTRUE ) { \
        v_پورټ_Yield_From_ISR(); \
    } \
})
#تعریف_کړه پورټ_YIELD_FROM_ISR_NO_CHECK()               v_پورټ_Yield_From_ISR()

// ----------------------- System --------------------------

ساكن مضمن بنسټ_Type_t __attribute__((always_inline)) x_پورټ_Get_Core_ID( فراغ );

// --------------------- TCB Cleanup -----------------------

/**
 * @brief TCB cleanup hook
 *
 * The portCLEAN_UP_TCB() macro is called in prvDeleteTCB() right before a
 * deleted task's memory is freed. We map that macro to this internal function
 * so that IDF FreeRTOS ports can inject some task pre-deletion operations.
 */
فراغ v_پورټ_TCBPre_Delete_Hook( فراغ *pxTCB );


/* ----------------------------------------- FreeRTOS SMP Porting Interface --------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS SMP
 * - Must come after forward declare as some porting macros map to declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

#تعریف_کړه پورټ_سېټ_مداخله_ماسک()            ({ \
    بینښه صحيح prev_level = XTOS_سېټ_INTLEVEL(XCHAL_EXCM_LEVEL); \
    portbenchmark_مداخله_DISABLE(); \
    prev_level = ((prev_level >> XCHAL_PS_INTLEVEL_ښويدنه) & XCHAL_PS_INTLEVEL_ماسک); \
    prev_level; \
})
#تعریف_کړه پورټ_سېټ_مداخله_ماسک_FROM_ISR()   پورټ_سېټ_مداخله_ماسک()
#تعریف_کړه پورټ_DISABLE_INTERRUPTS()            پورټ_سېټ_مداخله_ماسک()

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
#تعریف_کړه پورټ_CLEAR_مداخله_ماسک(x)         ({ \
    بینښه صحيح ps_reg; \
    RSR(XT_REG_PS, ps_reg); \
    ps_reg = (ps_reg & ~XCHAL_PS_INTLEVEL_ماسک); \
    ps_reg |= ((x << XCHAL_PS_INTLEVEL_ښويدنه) & XCHAL_PS_INTLEVEL_ماسک); \
    XTOS_RESTORE_INTLEVEL(ps_reg); \
})
#تعریف_کړه پورټ_CLEAR_مداخله_ماسک_FROM_ISR(x)    پورټ_CLEAR_مداخله_ماسک(x)
#تعریف_کړه پورټ_ENABLE_INTERRUPTS()     ({ \
    portbenchmark_مداخله_RESTORE(0); \
    XTOS_سېټ_INTLEVEL(0); \
})

/* Compatibility */
#تعریف_کړه پورټ_RESTORE_INTERRUPTS(x)   پورټ_CLEAR_مداخله_ماسک(x)

// ------------------ Critical Sections --------------------

#إذا ( config_شمېره_د_CORES > 1 )
#تعریف_کړه پورټ_GET_TASK_قلف()                         v_پورټ_Take_قلف(&پورټ_x_Task_قلف)
#تعریف_کړه پورټ_RELEASE_TASK_قلف()                     v_پورټ_Release_قلف(&پورټ_x_Task_قلف)
#تعریف_کړه پورټ_GET_ISR_قلف()                          v_پورټ_Take_قلف(&پورټ_x_ISRLock)
#تعریف_کړه پورټ_RELEASE_ISR_قلف()                      v_پورټ_Release_قلف(&پورټ_x_ISRLock)
#د_که_پای /* configNUMBER_OF_CORES > 1 */

//Critical sections used by FreeRTOS SMP
خارجي فراغ vTaskEnterCritical( فراغ );
خارجي فراغ vTaskExitCritical( فراغ );
#تعریف_کړه پورټ_ENTER_CRITICAL_SMP()                    vTaskEnterCritical();
#تعریف_کړه پورټ_EXIT_CRITICAL_SMP()                     vTaskExitCritical();

#إذا defined(__cplusplus) && (__cplusplus >  201703L)
#تعریف_کړه پورټ_ENTER_CRITICAL(...)                     ټاکنه_MACRO_VA_ARG(پورټ_ENTER_CRITICAL_IDF, پورټ_ENTER_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#تعریف_کړه پورټ_EXIT_CRITICAL(...)                      ټاکنه_MACRO_VA_ARG(پورټ_EXIT_CRITICAL_IDF, پورټ_EXIT_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#وإلا
#تعریف_کړه پورټ_ENTER_CRITICAL(...)                     ټاکنه_MACRO_VA_ARG(پورټ_ENTER_CRITICAL_IDF, پورټ_ENTER_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#تعریف_کړه پورټ_EXIT_CRITICAL(...)                      ټاکنه_MACRO_VA_ARG(پورټ_EXIT_CRITICAL_IDF, پورټ_EXIT_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#د_که_پای

خارجي UBaseType_t vTaskEnterCriticalFromISR( فراغ );
خارجي فراغ vTaskExitCriticalFromISR( UBaseType_t ux_Saved_مداخله_حالت );
#تعریف_کړه پورټ_ENTER_CRITICAL_FROM_ISR() vTaskEnterCriticalFromISR()
#تعریف_کړه پورټ_EXIT_CRITICAL_FROM_ISR(x) vTaskExitCriticalFromISR(x)

// ---------------------- Yielding -------------------------

#تعریف_کړه پورټ_YIELD()                                 v_پورټ_Yield()
#إذا defined(__cplusplus) && (__cplusplus >  201703L)
#تعریف_کړه پورټ_YIELD_FROM_ISR(...)                     ټاکنه_MACRO_VA_ARG(پورټ_YIELD_FROM_ISR_CHECK, پورټ_YIELD_FROM_ISR_NO_CHECK __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#وإلا
#تعریف_کړه پورټ_YIELD_FROM_ISR(...)                     ټاکنه_MACRO_VA_ARG(پورټ_YIELD_FROM_ISR_CHECK, پورټ_YIELD_FROM_ISR_NO_CHECK, ##__VA_ARGS__)(__VA_ARGS__)
#د_که_پای
#تعریف_کړه پورټ_YIELD_CORE(x)                           v_پورټ_Yield_Core(x)

// ----------------------- System --------------------------

#تعریف_کړه پورټ_GET_CORE_ID()                           x_پورټ_Get_Core_ID()
#تعریف_کړه پورټ_CHECK_IF_دننه_ISR()                       x_پورټ_Check_If_دننه_ISR()

// ------------------- Run Time Stats ----------------------

//Timers are already configured, so nothing to do for configuration of run time stats timer
#تعریف_کړه پورټ_CONFIGURE_TIMER_FOR_پرلپسې_وخت_STATS()
#إذا ( CONFIG_FREERTOS_GENERATE_پرلپسې_وخت_STATS )
config_پرلپسې_وخت_COUNTER_TYPE x_پورټ_Get_پرلپسې_وخت_Counter_ارزښت( فراغ );
#تعریف_کړه پورټ_GET_پرلپسې_وخت_COUNTER_ارزښت()        x_پورټ_Get_پرلپسې_وخت_Counter_ارزښت()
#د_که_پای

// --------------------- TCB Cleanup -----------------------

#تعریف_کړه پورټ_CLEAN_پورته_TCB( pxTCB )                   v_پورټ_TCBPre_Delete_Hook( pxTCB )

/* --------------------------------------------- Inline Implementations ------------------------------------------------
 * - Implementation of inline functions of the forward declares
 * - Should come after forward declare and FreeRTOS Porting interface, as implementation may use both.
 * - For implementation of non-inlined functions, see port.c
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

// ------------------ Critical Sections --------------------

// ---------------------- Yielding -------------------------

خارجي فراغ frxt_تیاری_switch( فراغ );     //Defined in portasm.S

ساكن مضمن فراغ __attribute__((always_inline)) v_پورټ_Yield_Core( بنسټ_Type_t xCoreID )
{
    esp_crosscore_int_send_yield( xCoreID );
}

ساكن مضمن فراغ __attribute__((always_inline)) v_پورټ_Yield_From_ISR( فراغ )
{
    //traceISR_EXIT_TO_SCHEDULER();
    frxt_تیاری_switch();
}

// ----------------------- System --------------------------

ساكن مضمن بنسټ_Type_t __attribute__((always_inline)) x_پورټ_Get_Core_ID( فراغ )
{
    أرجع (بنسټ_Type_t) esp_cpu_get_core_id();
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

ساكن مضمن بنسټ_Type_t x_پورټ_دننه_Isr_Context(فراغ)
{
    //Just call the FreeRTOS port interface version
    أرجع x_پورټ_Check_If_دننه_ISR();
}

بنسټ_Type_t x_پورټ_Interrupted_From_ISRContext(فراغ);

ساكن مضمن UBaseType_t x_پورټ_سېټ_مداخله_ماسک_From_ISR(فراغ)
{
    UBaseType_t prev_int_level = XTOS_سېټ_INTLEVEL(XCHAL_EXCM_LEVEL);
    portbenchmark_مداخله_DISABLE();
    أرجع prev_int_level;
}

ساكن مضمن فراغ v_پورټ_Clear_مداخله_ماسک_From_ISR(UBaseType_t prev_level)
{
    portbenchmark_مداخله_RESTORE(prev_level);
    XTOS_RESTORE_JUST_INTLEVEL((صحيح) prev_level);
}

// ---------------------- Spinlocks ------------------------

ساكن مضمن bool __attribute__((always_inline)) v_پورټ_CPUAcquire_Mutex_Timeout(پورټ_MUX_TYPE *mux, صحيح timeout)
{
    أرجع (spinlock_acquire(mux, timeout));
}

ساكن مضمن فراغ __attribute__((always_inline)) v_پورټ_CPURelease_Mutex(پورټ_MUX_TYPE *mux)
{
    spinlock_release(mux);
}

// ------------------ Critical Sections --------------------

#إذا CONFIG_FREERTOS_پورټ_THREAD_خوندي_CLAIM
/**
 * @brief Claim thread-safe region start
 *        If claimed, vPortEnterCritical/vPortExitCritical on the current core are no-ops.
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
فراغ x_پورټ_Thread_خوندي_Claim(فراغ);

/**
 * @brief Claim thread-safe region end
 *        Restores normal port critical behavior
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
فراغ x_پورټ_Thread_خوندي_Disclaim(فراغ);

خارجي متطاير bool پورټ_x_Thread_خوندي_Claimed;
#د_که_پای /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

بنسټ_Type_t x_پورټ_Enter_Critical_Timeout(پورټ_MUX_TYPE *قلف, بنسټ_Type_t timeout);

ساكن مضمن فراغ __attribute__((always_inline)) v_پورټ_Enter_Critical_IDF(پورټ_MUX_TYPE *قلف)
{
    x_پورټ_Enter_Critical_Timeout(قلف, پورټ_MUX_NO_TIMEOUT);
}

فراغ v_پورټ_Exit_Critical_IDF(پورټ_MUX_TYPE *قلف);

/*
In case any IDF libs called the port critical functions directly instead of through the macros.
The will just call the vPort...CriticalIDF() functions
*/
فراغ v_پورټ_Enter_Critical(پورټ_MUX_TYPE *قلف);
فراغ v_پورټ_Exit_Critical(پورټ_MUX_TYPE *قلف);

//IDF task critical sections
#تعریف_کړه پورټ_TRY_ENTER_CRITICAL(قلف, timeout)       x_پورټ_Enter_Critical_Timeout(قلف, timeout)
#تعریف_کړه پورټ_ENTER_CRITICAL_IDF(قلف)                v_پورټ_Enter_Critical_IDF(قلف)
#تعریف_کړه پورټ_EXIT_CRITICAL_IDF(قلف)                 v_پورټ_Exit_Critical_IDF(قلف)
//IDF ISR critical sections
#تعریف_کړه پورټ_TRY_ENTER_CRITICAL_ISR(قلف, timeout)   x_پورټ_Enter_Critical_Timeout(قلف, timeout)
#تعریف_کړه پورټ_ENTER_CRITICAL_ISR(قلف)                v_پورټ_Enter_Critical_IDF(قلف)
#تعریف_کړه پورټ_EXIT_CRITICAL_ISR(قلف)                 v_پورټ_Exit_Critical_IDF(قلف)
//IDF safe critical sections (they're the same)
#تعریف_کړه پورټ_ENTER_CRITICAL_خوندي(قلف)               v_پورټ_Enter_Critical_IDF(قلف)
#تعریف_کړه پورټ_EXIT_CRITICAL_خوندي(قلف)                v_پورټ_Exit_Critical_IDF(قلف)

// ---------------------- Yielding -------------------------

ساكن مضمن bool x_پورټ_Can_Yield(فراغ)
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

    أرجع ((ps_reg & PS_INTLEVEL_ماسک) == 0);
}

// Defined even for configNUMBER_OF_CORES > 1 for IDF compatibility
#تعریف_کړه پورټ_YIELD_WITHIN_API()                      esp_crosscore_int_send_yield(x_پورټ_Get_Core_ID())

// ----------------------- System --------------------------

فراغ v_پورټ_سېټ_ډېری_Watchpoint(فراغ *px_ډېری_پیل);

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
bool x_پورټ_Check_Valid_List_Mem(ثابت فراغ *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool x_پورټ_Check_Valid_TCBMem(ثابت فراغ *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_ډېری_Mem(ثابت فراغ *ptr);

#تعریف_کړه پورټ_VALID_LIST_MEM(ptr)     x_پورټ_Check_Valid_List_Mem(ptr)
#تعریف_کړه پورټ_VALID_TCB_MEM(ptr)      x_پورټ_Check_Valid_TCBMem(ptr)
#تعریف_کړه پورټ_VALID_ډېری_MEM(ptr)    x_Portcheck_Valid_ډېری_Mem(ptr)

/* ------------------------------------------------------ Misc ---------------------------------------------------------
 * - Miscellaneous porting macros
 * - These are not part of the FreeRTOS porting interface, but are used by other FreeRTOS dependent components
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- App-Trace -------------------------

#إذا CONFIG_ESP_TRACE_ENABLE
خارجي متطاير بنسټ_Type_t x_پورټ_Switch_Flag;
#تعریف_کړه os_task_switch_is_pended(_cpu_) (x_پورټ_Switch_Flag)
#وإلا
#تعریف_کړه os_task_switch_is_pended(_cpu_) (false)
#د_که_پای

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

#که_تعریف_شوی __cplusplus
}
#د_که_پای
