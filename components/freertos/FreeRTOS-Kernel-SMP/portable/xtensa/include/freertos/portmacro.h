/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ہدایت once

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

#جے_تعریف_شدہ __cplusplus
خارجي "C" {
#جے_انت

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come before first as they are used further down the file
 * ------------------------------------------------------------------------------------------------------------------ */

#تعریف_کرو پورٹ_CHAR                    int8_t
#تعریف_کرو پورٹ_FLOAT                   عائم
#تعریف_کرو پورٹ_DOUBLE                  مزدوج
#تعریف_کرو پورٹ_LONG                    int32_t
#تعریف_کرو پورٹ_SHORT                   int16_t
#تعریف_کرو پورٹ_ڈھیر_TYPE              uint8_t
#تعریف_کرو پورٹ_ادھار_TYPE               صحيح

تعريفنوع پورٹ_ڈھیر_TYPE              ڈھیر_Type_t;
تعريفنوع پورٹ_ادھار_TYPE               ادھار_Type_t;
تعريفنوع بنانشانی پورٹ_ادھار_TYPE      UBaseType_t;

#إذا( config_USE_16_ثنائی_ہندسہ_TICKS == 1 )
تعريفنوع uint16_t TickType_t;
#تعریف_کرو پورٹ_MAX_DELAY ( TickType_t ) 0xffff
#وإلا
تعريفنوع uint32_t TickType_t;
#تعریف_کرو پورٹ_MAX_DELAY ( TickType_t ) 0xffffffffUL
#جے_انت

/* Task function macros as described on the FreeRTOS.org WEB site. */
#تعریف_کرو پورٹ_TASK_FUNCTION_PROTO( vFunction, pvParameters )  فراغ vFunction( فراغ *pvParameters )
#تعریف_کرو پورٹ_TASK_FUNCTION( vFunction, pvParameters )        فراغ vFunction( فراغ *pvParameters )

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#تعریف_کرو پورٹ_CRITICAL_NESTING_وچ_TCB     1
#تعریف_کرو پورٹ_ڈھیر_GROWTH                ( -1 )
#تعریف_کرو پورٹ_TICK_PERIOD_MS              ( ( TickType_t ) 1000 / config_TICK_شرح_HZ )
#تعریف_کرو پورٹ_BYTE_ALIGNMENT              16    // Xtensa Windowed ABI requires the stack pointer to always be 16-byte aligned. See "isa_rm.pdf 8.1.1 Windowed Register Usage and Stack Layout"
#تعریف_کرو پورٹ_NOP()                       XT_NOP()    //Todo: Check if XT_NOP exists

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros need to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Spinlocks ------------------------

تعريفنوع spinlock_t                          پورٹ_MUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */
#تعریف_کرو پورٹ_MUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#تعریف_کرو پورٹ_MUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#تعریف_کرو پورٹ_MUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#تعریف_کرو پورٹ_MUX_TRY_جندرا                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#تعریف_کرو پورٹ_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

ادھار_Type_t x_پورٹ_Check_If_وچ_ISR(فراغ);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortCheckIfInISR() internally
 */
فراغ v_پورٹ_Assert_If_وچ_ISR(فراغ);

// ------------------ Critical Sections --------------------

UBaseType_t ux_پورٹ_Enter_Critical_From_ISR( فراغ );
فراغ v_پورٹ_Exit_Critical_From_ISR( UBaseType_t level );

#إذا ( config_نمبر_دا_CORES > 1 )
/*
These are always called with interrupts already disabled. We simply need to get/release the spinlocks
*/
خارجي پورٹ_MUX_TYPE پورٹ_x_Task_جندرا;
خارجي پورٹ_MUX_TYPE پورٹ_x_ISRLock;

فراغ v_پورٹ_Take_جندرا( پورٹ_MUX_TYPE *جندرا );
فراغ v_پورٹ_Release_جندرا( پورٹ_MUX_TYPE *جندرا );
#جے_انت /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

فراغ v_پورٹ_Yield( فراغ );
ساكن مضمن فراغ __attribute__((always_inline)) v_پورٹ_Yield_Core( ادھار_Type_t xCoreID );
ساكن مضمن فراغ __attribute__((always_inline)) v_پورٹ_Yield_From_ISR( فراغ );

#تعریف_کرو پورٹ_YIELD_FROM_ISR_CHECK(x)     ({ \
    إذا ( (x) == pdTRUE ) { \
        v_پورٹ_Yield_From_ISR(); \
    } \
})
#تعریف_کرو پورٹ_YIELD_FROM_ISR_NO_CHECK()               v_پورٹ_Yield_From_ISR()

// ----------------------- System --------------------------

ساكن مضمن ادھار_Type_t __attribute__((always_inline)) x_پورٹ_Get_Core_ID( فراغ );

// --------------------- TCB Cleanup -----------------------

/**
 * @brief TCB cleanup hook
 *
 * The portCLEAN_UP_TCB() macro is called in prvDeleteTCB() right before a
 * deleted task's memory is freed. We map that macro to this internal function
 * so that IDF FreeRTOS ports can inject some task pre-deletion operations.
 */
فراغ v_پورٹ_TCBPre_Delete_Hook( فراغ *pxTCB );


/* ----------------------------------------- FreeRTOS SMP Porting Interface --------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS SMP
 * - Must come after forward declare as some porting macros map to declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

#تعریف_کرو پورٹ_سیٹ_رکاوٹ_نقاب()            ({ \
    بنانشانی صحيح prev_level = XTOS_سیٹ_INTLEVEL(XCHAL_EXCM_LEVEL); \
    portbenchmark_رکاوٹ_DISABLE(); \
    prev_level = ((prev_level >> XCHAL_PS_INTLEVEL_کھسکاؤ) & XCHAL_PS_INTLEVEL_نقاب); \
    prev_level; \
})
#تعریف_کرو پورٹ_سیٹ_رکاوٹ_نقاب_FROM_ISR()   پورٹ_سیٹ_رکاوٹ_نقاب()
#تعریف_کرو پورٹ_DISABLE_INTERRUPTS()            پورٹ_سیٹ_رکاوٹ_نقاب()

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
#تعریف_کرو پورٹ_CLEAR_رکاوٹ_نقاب(x)         ({ \
    بنانشانی صحيح ps_reg; \
    RSR(XT_REG_PS, ps_reg); \
    ps_reg = (ps_reg & ~XCHAL_PS_INTLEVEL_نقاب); \
    ps_reg |= ((x << XCHAL_PS_INTLEVEL_کھسکاؤ) & XCHAL_PS_INTLEVEL_نقاب); \
    XTOS_RESTORE_INTLEVEL(ps_reg); \
})
#تعریف_کرو پورٹ_CLEAR_رکاوٹ_نقاب_FROM_ISR(x)    پورٹ_CLEAR_رکاوٹ_نقاب(x)
#تعریف_کرو پورٹ_ENABLE_INTERRUPTS()     ({ \
    portbenchmark_رکاوٹ_RESTORE(0); \
    XTOS_سیٹ_INTLEVEL(0); \
})

/* Compatibility */
#تعریف_کرو پورٹ_RESTORE_INTERRUPTS(x)   پورٹ_CLEAR_رکاوٹ_نقاب(x)

// ------------------ Critical Sections --------------------

#إذا ( config_نمبر_دا_CORES > 1 )
#تعریف_کرو پورٹ_GET_TASK_جندرا()                         v_پورٹ_Take_جندرا(&پورٹ_x_Task_جندرا)
#تعریف_کرو پورٹ_RELEASE_TASK_جندرا()                     v_پورٹ_Release_جندرا(&پورٹ_x_Task_جندرا)
#تعریف_کرو پورٹ_GET_ISR_جندرا()                          v_پورٹ_Take_جندرا(&پورٹ_x_ISRLock)
#تعریف_کرو پورٹ_RELEASE_ISR_جندرا()                      v_پورٹ_Release_جندرا(&پورٹ_x_ISRLock)
#جے_انت /* configNUMBER_OF_CORES > 1 */

//Critical sections used by FreeRTOS SMP
خارجي فراغ vTaskEnterCritical( فراغ );
خارجي فراغ vTaskExitCritical( فراغ );
#تعریف_کرو پورٹ_ENTER_CRITICAL_SMP()                    vTaskEnterCritical();
#تعریف_کرو پورٹ_EXIT_CRITICAL_SMP()                     vTaskExitCritical();

#إذا defined(__cplusplus) && (__cplusplus >  201703L)
#تعریف_کرو پورٹ_ENTER_CRITICAL(...)                     چنو_MACRO_VA_ARG(پورٹ_ENTER_CRITICAL_IDF, پورٹ_ENTER_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#تعریف_کرو پورٹ_EXIT_CRITICAL(...)                      چنو_MACRO_VA_ARG(پورٹ_EXIT_CRITICAL_IDF, پورٹ_EXIT_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#وإلا
#تعریف_کرو پورٹ_ENTER_CRITICAL(...)                     چنو_MACRO_VA_ARG(پورٹ_ENTER_CRITICAL_IDF, پورٹ_ENTER_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#تعریف_کرو پورٹ_EXIT_CRITICAL(...)                      چنو_MACRO_VA_ARG(پورٹ_EXIT_CRITICAL_IDF, پورٹ_EXIT_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#جے_انت

خارجي UBaseType_t vTaskEnterCriticalFromISR( فراغ );
خارجي فراغ vTaskExitCriticalFromISR( UBaseType_t ux_Saved_رکاوٹ_حالت );
#تعریف_کرو پورٹ_ENTER_CRITICAL_FROM_ISR() vTaskEnterCriticalFromISR()
#تعریف_کرو پورٹ_EXIT_CRITICAL_FROM_ISR(x) vTaskExitCriticalFromISR(x)

// ---------------------- Yielding -------------------------

#تعریف_کرو پورٹ_YIELD()                                 v_پورٹ_Yield()
#إذا defined(__cplusplus) && (__cplusplus >  201703L)
#تعریف_کرو پورٹ_YIELD_FROM_ISR(...)                     چنو_MACRO_VA_ARG(پورٹ_YIELD_FROM_ISR_CHECK, پورٹ_YIELD_FROM_ISR_NO_CHECK __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#وإلا
#تعریف_کرو پورٹ_YIELD_FROM_ISR(...)                     چنو_MACRO_VA_ARG(پورٹ_YIELD_FROM_ISR_CHECK, پورٹ_YIELD_FROM_ISR_NO_CHECK, ##__VA_ARGS__)(__VA_ARGS__)
#جے_انت
#تعریف_کرو پورٹ_YIELD_CORE(x)                           v_پورٹ_Yield_Core(x)

// ----------------------- System --------------------------

#تعریف_کرو پورٹ_GET_CORE_ID()                           x_پورٹ_Get_Core_ID()
#تعریف_کرو پورٹ_CHECK_IF_وچ_ISR()                       x_پورٹ_Check_If_وچ_ISR()

// ------------------- Run Time Stats ----------------------

//Timers are already configured, so nothing to do for configuration of run time stats timer
#تعریف_کرو پورٹ_CONFIGURE_TIMER_FOR_لگاتار_ویلا_STATS()
#إذا ( CONFIG_FREERTOS_GENERATE_لگاتار_ویلا_STATS )
config_لگاتار_ویلا_COUNTER_TYPE x_پورٹ_Get_لگاتار_ویلا_Counter_قدر( فراغ );
#تعریف_کرو پورٹ_GET_لگاتار_ویلا_COUNTER_قدر()        x_پورٹ_Get_لگاتار_ویلا_Counter_قدر()
#جے_انت

// --------------------- TCB Cleanup -----------------------

#تعریف_کرو پورٹ_CLEAN_اتے_TCB( pxTCB )                   v_پورٹ_TCBPre_Delete_Hook( pxTCB )

/* --------------------------------------------- Inline Implementations ------------------------------------------------
 * - Implementation of inline functions of the forward declares
 * - Should come after forward declare and FreeRTOS Porting interface, as implementation may use both.
 * - For implementation of non-inlined functions, see port.c
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

// ------------------ Critical Sections --------------------

// ---------------------- Yielding -------------------------

خارجي فراغ frxt_تیاری_switch( فراغ );     //Defined in portasm.S

ساكن مضمن فراغ __attribute__((always_inline)) v_پورٹ_Yield_Core( ادھار_Type_t xCoreID )
{
    esp_crosscore_int_send_yield( xCoreID );
}

ساكن مضمن فراغ __attribute__((always_inline)) v_پورٹ_Yield_From_ISR( فراغ )
{
    //traceISR_EXIT_TO_SCHEDULER();
    frxt_تیاری_switch();
}

// ----------------------- System --------------------------

ساكن مضمن ادھار_Type_t __attribute__((always_inline)) x_پورٹ_Get_Core_ID( فراغ )
{
    أرجع (ادھار_Type_t) esp_cpu_get_core_id();
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

ساكن مضمن ادھار_Type_t x_پورٹ_وچ_Isr_Context(فراغ)
{
    //Just call the FreeRTOS port interface version
    أرجع x_پورٹ_Check_If_وچ_ISR();
}

ادھار_Type_t x_پورٹ_Interrupted_From_ISRContext(فراغ);

ساكن مضمن UBaseType_t x_پورٹ_سیٹ_رکاوٹ_نقاب_From_ISR(فراغ)
{
    UBaseType_t prev_int_level = XTOS_سیٹ_INTLEVEL(XCHAL_EXCM_LEVEL);
    portbenchmark_رکاوٹ_DISABLE();
    أرجع prev_int_level;
}

ساكن مضمن فراغ v_پورٹ_Clear_رکاوٹ_نقاب_From_ISR(UBaseType_t prev_level)
{
    portbenchmark_رکاوٹ_RESTORE(prev_level);
    XTOS_RESTORE_JUST_INTLEVEL((صحيح) prev_level);
}

// ---------------------- Spinlocks ------------------------

ساكن مضمن bool __attribute__((always_inline)) v_پورٹ_CPUAcquire_Mutex_Timeout(پورٹ_MUX_TYPE *mux, صحيح timeout)
{
    أرجع (spinlock_acquire(mux, timeout));
}

ساكن مضمن فراغ __attribute__((always_inline)) v_پورٹ_CPURelease_Mutex(پورٹ_MUX_TYPE *mux)
{
    spinlock_release(mux);
}

// ------------------ Critical Sections --------------------

#إذا CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
/**
 * @brief Claim thread-safe region start
 *        If claimed, vPortEnterCritical/vPortExitCritical on the current core are no-ops.
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
فراغ x_پورٹ_Thread_محفوظ_Claim(فراغ);

/**
 * @brief Claim thread-safe region end
 *        Restores normal port critical behavior
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
فراغ x_پورٹ_Thread_محفوظ_Disclaim(فراغ);

خارجي متطاير bool پورٹ_x_Thread_محفوظ_Claimed;
#جے_انت /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

ادھار_Type_t x_پورٹ_Enter_Critical_Timeout(پورٹ_MUX_TYPE *جندرا, ادھار_Type_t timeout);

ساكن مضمن فراغ __attribute__((always_inline)) v_پورٹ_Enter_Critical_IDF(پورٹ_MUX_TYPE *جندرا)
{
    x_پورٹ_Enter_Critical_Timeout(جندرا, پورٹ_MUX_NO_TIMEOUT);
}

فراغ v_پورٹ_Exit_Critical_IDF(پورٹ_MUX_TYPE *جندرا);

/*
In case any IDF libs called the port critical functions directly instead of through the macros.
The will just call the vPort...CriticalIDF() functions
*/
فراغ v_پورٹ_Enter_Critical(پورٹ_MUX_TYPE *جندرا);
فراغ v_پورٹ_Exit_Critical(پورٹ_MUX_TYPE *جندرا);

//IDF task critical sections
#تعریف_کرو پورٹ_TRY_ENTER_CRITICAL(جندرا, timeout)       x_پورٹ_Enter_Critical_Timeout(جندرا, timeout)
#تعریف_کرو پورٹ_ENTER_CRITICAL_IDF(جندرا)                v_پورٹ_Enter_Critical_IDF(جندرا)
#تعریف_کرو پورٹ_EXIT_CRITICAL_IDF(جندرا)                 v_پورٹ_Exit_Critical_IDF(جندرا)
//IDF ISR critical sections
#تعریف_کرو پورٹ_TRY_ENTER_CRITICAL_ISR(جندرا, timeout)   x_پورٹ_Enter_Critical_Timeout(جندرا, timeout)
#تعریف_کرو پورٹ_ENTER_CRITICAL_ISR(جندرا)                v_پورٹ_Enter_Critical_IDF(جندرا)
#تعریف_کرو پورٹ_EXIT_CRITICAL_ISR(جندرا)                 v_پورٹ_Exit_Critical_IDF(جندرا)
//IDF safe critical sections (they're the same)
#تعریف_کرو پورٹ_ENTER_CRITICAL_محفوظ(جندرا)               v_پورٹ_Enter_Critical_IDF(جندرا)
#تعریف_کرو پورٹ_EXIT_CRITICAL_محفوظ(جندرا)                v_پورٹ_Exit_Critical_IDF(جندرا)

// ---------------------- Yielding -------------------------

ساكن مضمن bool x_پورٹ_Can_Yield(فراغ)
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
#تعریف_کرو پورٹ_YIELD_WITHIN_API()                      esp_crosscore_int_send_yield(x_پورٹ_Get_Core_ID())

// ----------------------- System --------------------------

فراغ v_پورٹ_سیٹ_ڈھیر_Watchpoint(فراغ *px_ڈھیر_شروع);

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
bool x_پورٹ_Check_Valid_List_Mem(ثابت فراغ *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool x_پورٹ_Check_Valid_TCBMem(ثابت فراغ *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_ڈھیر_Mem(ثابت فراغ *ptr);

#تعریف_کرو پورٹ_VALID_LIST_MEM(ptr)     x_پورٹ_Check_Valid_List_Mem(ptr)
#تعریف_کرو پورٹ_VALID_TCB_MEM(ptr)      x_پورٹ_Check_Valid_TCBMem(ptr)
#تعریف_کرو پورٹ_VALID_ڈھیر_MEM(ptr)    x_Portcheck_Valid_ڈھیر_Mem(ptr)

/* ------------------------------------------------------ Misc ---------------------------------------------------------
 * - Miscellaneous porting macros
 * - These are not part of the FreeRTOS porting interface, but are used by other FreeRTOS dependent components
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- App-Trace -------------------------

#إذا CONFIG_ESP_TRACE_ENABLE
خارجي متطاير ادھار_Type_t x_پورٹ_Switch_Flag;
#تعریف_کرو os_task_switch_is_pended(_cpu_) (x_پورٹ_Switch_Flag)
#وإلا
#تعریف_کرو os_task_switch_is_pended(_cpu_) (false)
#جے_انت

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

#جے_تعریف_شدہ __cplusplus
}
#جے_انت
