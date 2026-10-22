/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ہدایت once

#شامل "sdkconfig.h"
#شامل <stdint.h>
#شامل "spinlock.h"
#شامل "xtensa/xtruntime.h"
#شامل "xtensa/config/core.h"
#شامل "xtensa/config/core-isa.h"
#شامل "xtensa/config/xt_specreg.h"
#شامل "xt_instr_macros.h"
#شامل "portbenchmark.h"
#شامل "esp_macros.h"
#شامل "esp_cpu.h"
#شامل "esp_private/crosscore_int.h"
#شامل "esp_memory_utils.h"

/*
Note: We should not include any FreeRTOS headers (directly or indirectly) here as this will create a reverse dependency
- FreeRTOS pulls in "portmacro.h" via
    - source.c <- FreeRTOS.h <- portable.h <- portmacro.h

- If we need types of task.h (such as TaskHandle_t), best put the feature somewhere else
- Be careful as some parts of C Standard Library Headers will pull in FreeRTOS headers
*/

#اگر_تعریف_شدہ __cplusplus
خارجی "C" {
#اگر_ختم

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come before first as they are used further down the file
 * ------------------------------------------------------------------------------------------------------------------ */

#تعریف_کرو پورٹ_CHAR                    int8_t
#تعریف_کرو پورٹ_FLOAT                   اعشاری
#تعریف_کرو پورٹ_DOUBLE                  دوہرا
#تعریف_کرو پورٹ_LONG                    int32_t
#تعریف_کرو پورٹ_SHORT                   int16_t
#تعریف_کرو پورٹ_ڈھیر_TYPE              uint8_t
#تعریف_کرو پورٹ_بنیاد_TYPE               صحیح

قسمتعریف پورٹ_ڈھیر_TYPE              ڈھیر_Type_t;
قسمتعریف پورٹ_بنیاد_TYPE               بنیاد_Type_t;
قسمتعریف غیرعلامتی پورٹ_بنیاد_TYPE      UBaseType_t;

#اگر( config_USE_16_ثنائی_ہندسہ_TICKS == 1 )
قسمتعریف uint16_t TickType_t;
#تعریف_کرو پورٹ_MAX_DELAY ( TickType_t ) 0xffff
#ورنہ
قسمتعریف uint32_t TickType_t;
#تعریف_کرو پورٹ_MAX_DELAY ( TickType_t ) 0xffffffffUL
#اگر_ختم

/* Task function macros as described on the FreeRTOS.org WEB site. */
#تعریف_کرو پورٹ_TASK_FUNCTION_PROTO( vFunction, pvParameters )  خالی vFunction( خالی *pvParameters )
#تعریف_کرو پورٹ_TASK_FUNCTION( vFunction, pvParameters )        خالی vFunction( خالی *pvParameters )

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#تعریف_کرو پورٹ_CRITICAL_NESTING_اندر_TCB     1
#تعریف_کرو پورٹ_ڈھیر_GROWTH                ( -1 )
#تعریف_کرو پورٹ_TICK_PERIOD_MS              ( ( TickType_t ) 1000 / config_TICK_شرح_HZ )
#تعریف_کرو پورٹ_BYTE_ALIGNMENT              16    // Xtensa Windowed ABI requires the stack pointer to always be 16-byte aligned. See "isa_rm.pdf 8.1.1 Windowed Register Usage and Stack Layout"
#تعریف_کرو پورٹ_NOP()                       XT_NOP()    //Todo: Check if XT_NOP exists

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros need to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Spinlocks ------------------------

قسمتعریف spinlock_t                          پورٹ_MUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */
#تعریف_کرو پورٹ_MUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#تعریف_کرو پورٹ_MUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#تعریف_کرو پورٹ_MUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#تعریف_کرو پورٹ_MUX_TRY_تالا                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#تعریف_کرو پورٹ_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

بنیاد_Type_t x_پورٹ_Check_If_اندر_ISR(خالی);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortCheckIfInISR() internally
 */
خالی v_پورٹ_Assert_If_اندر_ISR(خالی);

// ------------------ Critical Sections --------------------

UBaseType_t ux_پورٹ_Enter_Critical_From_ISR( خالی );
خالی v_پورٹ_Exit_Critical_From_ISR( UBaseType_t level );

#اگر ( config_نمبر_کا_CORES > 1 )
/*
These are always called with interrupts already disabled. We simply need to get/release the spinlocks
*/
خارجی پورٹ_MUX_TYPE پورٹ_x_Task_تالا;
خارجی پورٹ_MUX_TYPE پورٹ_x_ISRLock;

خالی v_پورٹ_Take_تالا( پورٹ_MUX_TYPE *تالا_2 );
خالی v_پورٹ_Release_تالا( پورٹ_MUX_TYPE *تالا_2 );
#اگر_ختم /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

خالی v_پورٹ_Yield( خالی );
ساکن اندرونی خالی __attribute__((always_inline)) v_پورٹ_Yield_Core( بنیاد_Type_t xCoreID );
ساکن اندرونی خالی __attribute__((always_inline)) v_پورٹ_Yield_From_ISR( خالی );

#تعریف_کرو پورٹ_YIELD_FROM_ISR_CHECK(x)     ({ \
    اگر ( (x) == pdTRUE ) { \
        v_پورٹ_Yield_From_ISR(); \
    } \
})
#تعریف_کرو پورٹ_YIELD_FROM_ISR_NO_CHECK()               v_پورٹ_Yield_From_ISR()

// ----------------------- System --------------------------

ساکن اندرونی بنیاد_Type_t __attribute__((always_inline)) x_پورٹ_Get_Core_ID( خالی );

// --------------------- TCB Cleanup -----------------------

/**
 * @brief TCB cleanup hook
 *
 * The portCLEAN_UP_TCB() macro is called in prvDeleteTCB() right before a
 * deleted task's memory is freed. We map that macro to this internal function
 * so that IDF FreeRTOS ports can inject some task pre-deletion operations.
 */
خالی v_پورٹ_TCBPre_Delete_Hook( خالی *pxTCB );


/* ----------------------------------------- FreeRTOS SMP Porting Interface --------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS SMP
 * - Must come after forward declare as some porting macros map to declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

#تعریف_کرو پورٹ_سیٹ_مداخلت_نقاب()            ({ \
    غیرعلامتی صحیح پچھلا_level = XTOS_سیٹ_INTLEVEL(XCHAL_EXCM_LEVEL); \
    portbenchmark_مداخلت_DISABLE(); \
    پچھلا_level = ((پچھلا_level >> XCHAL_PS_INTLEVEL_سرکاؤ) & XCHAL_PS_INTLEVEL_نقاب); \
    پچھلا_level; \
})
#تعریف_کرو پورٹ_سیٹ_مداخلت_نقاب_FROM_ISR()   پورٹ_سیٹ_مداخلت_نقاب()
#تعریف_کرو پورٹ_DISABLE_INTERRUPTS()            پورٹ_سیٹ_مداخلت_نقاب()

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
#تعریف_کرو پورٹ_CLEAR_مداخلت_نقاب(x)         ({ \
    غیرعلامتی صحیح ps_reg; \
    RSR(XT_REG_PS, ps_reg); \
    ps_reg = (ps_reg & ~XCHAL_PS_INTLEVEL_نقاب); \
    ps_reg |= ((x << XCHAL_PS_INTLEVEL_سرکاؤ) & XCHAL_PS_INTLEVEL_نقاب); \
    XTOS_RESTORE_INTLEVEL(ps_reg); \
})
#تعریف_کرو پورٹ_CLEAR_مداخلت_نقاب_FROM_ISR(x)    پورٹ_CLEAR_مداخلت_نقاب(x)
#تعریف_کرو پورٹ_ENABLE_INTERRUPTS()     ({ \
    portbenchmark_مداخلت_RESTORE(0); \
    XTOS_سیٹ_INTLEVEL(0); \
})

/* Compatibility */
#تعریف_کرو پورٹ_RESTORE_INTERRUPTS(x)   پورٹ_CLEAR_مداخلت_نقاب(x)

// ------------------ Critical Sections --------------------

#اگر ( config_نمبر_کا_CORES > 1 )
#تعریف_کرو پورٹ_GET_TASK_تالا()                         v_پورٹ_Take_تالا(&پورٹ_x_Task_تالا)
#تعریف_کرو پورٹ_RELEASE_TASK_تالا()                     v_پورٹ_Release_تالا(&پورٹ_x_Task_تالا)
#تعریف_کرو پورٹ_GET_ISR_تالا()                          v_پورٹ_Take_تالا(&پورٹ_x_ISRLock)
#تعریف_کرو پورٹ_RELEASE_ISR_تالا()                      v_پورٹ_Release_تالا(&پورٹ_x_ISRLock)
#اگر_ختم /* configNUMBER_OF_CORES > 1 */

//Critical sections used by FreeRTOS SMP
خارجی خالی vTaskEnterCritical( خالی );
خارجی خالی vTaskExitCritical( خالی );
#تعریف_کرو پورٹ_ENTER_CRITICAL_SMP()                    vTaskEnterCritical();
#تعریف_کرو پورٹ_EXIT_CRITICAL_SMP()                     vTaskExitCritical();

#اگر defined(__cplusplus) && (__cplusplus >  201703L)
#تعریف_کرو پورٹ_ENTER_CRITICAL(...)                     انتخاب_MACRO_VA_ARG(پورٹ_ENTER_CRITICAL_IDF, پورٹ_ENTER_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#تعریف_کرو پورٹ_EXIT_CRITICAL(...)                      انتخاب_MACRO_VA_ARG(پورٹ_EXIT_CRITICAL_IDF, پورٹ_EXIT_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#ورنہ
#تعریف_کرو پورٹ_ENTER_CRITICAL(...)                     انتخاب_MACRO_VA_ARG(پورٹ_ENTER_CRITICAL_IDF, پورٹ_ENTER_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#تعریف_کرو پورٹ_EXIT_CRITICAL(...)                      انتخاب_MACRO_VA_ARG(پورٹ_EXIT_CRITICAL_IDF, پورٹ_EXIT_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#اگر_ختم

خارجی UBaseType_t vTaskEnterCriticalFromISR( خالی );
خارجی خالی vTaskExitCriticalFromISR( UBaseType_t ux_Saved_مداخلت_حالت );
#تعریف_کرو پورٹ_ENTER_CRITICAL_FROM_ISR() vTaskEnterCriticalFromISR()
#تعریف_کرو پورٹ_EXIT_CRITICAL_FROM_ISR(x) vTaskExitCriticalFromISR(x)

// ---------------------- Yielding -------------------------

#تعریف_کرو پورٹ_YIELD()                                 v_پورٹ_Yield()
#اگر defined(__cplusplus) && (__cplusplus >  201703L)
#تعریف_کرو پورٹ_YIELD_FROM_ISR(...)                     انتخاب_MACRO_VA_ARG(پورٹ_YIELD_FROM_ISR_CHECK, پورٹ_YIELD_FROM_ISR_NO_CHECK __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#ورنہ
#تعریف_کرو پورٹ_YIELD_FROM_ISR(...)                     انتخاب_MACRO_VA_ARG(پورٹ_YIELD_FROM_ISR_CHECK, پورٹ_YIELD_FROM_ISR_NO_CHECK, ##__VA_ARGS__)(__VA_ARGS__)
#اگر_ختم
#تعریف_کرو پورٹ_YIELD_CORE(x)                           v_پورٹ_Yield_Core(x)

// ----------------------- System --------------------------

#تعریف_کرو پورٹ_GET_CORE_ID()                           x_پورٹ_Get_Core_ID()
#تعریف_کرو پورٹ_CHECK_IF_اندر_ISR()                       x_پورٹ_Check_If_اندر_ISR()

// ------------------- Run Time Stats ----------------------

//Timers are already configured, so nothing to do for configuration of run time stats timer
#تعریف_کرو پورٹ_CONFIGURE_TIMER_FOR_تسلسل_وقت_STATS()
#اگر ( CONFIG_FREERTOS_GENERATE_تسلسل_وقت_STATS )
config_تسلسل_وقت_شمار_TYPE x_پورٹ_Get_تسلسل_وقت_شمار_قدر( خالی );
#تعریف_کرو پورٹ_GET_تسلسل_وقت_شمار_قدر()        x_پورٹ_Get_تسلسل_وقت_شمار_قدر()
#اگر_ختم

// --------------------- TCB Cleanup -----------------------

#تعریف_کرو پورٹ_CLEAN_اوپر_TCB( pxTCB )                   v_پورٹ_TCBPre_Delete_Hook( pxTCB )

/* --------------------------------------------- Inline Implementations ------------------------------------------------
 * - Implementation of inline functions of the forward declares
 * - Should come after forward declare and FreeRTOS Porting interface, as implementation may use both.
 * - For implementation of non-inlined functions, see port.c
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

// ------------------ Critical Sections --------------------

// ---------------------- Yielding -------------------------

خارجی خالی frxt_تیاری_switch( خالی );     //Defined in portasm.S

ساکن اندرونی خالی __attribute__((always_inline)) v_پورٹ_Yield_Core( بنیاد_Type_t xCoreID )
{
    esp_crosscore_int_send_yield( xCoreID );
}

ساکن اندرونی خالی __attribute__((always_inline)) v_پورٹ_Yield_From_ISR( خالی )
{
    //traceISR_EXIT_TO_SCHEDULER();
    frxt_تیاری_switch();
}

// ----------------------- System --------------------------

ساکن اندرونی بنیاد_Type_t __attribute__((always_inline)) x_پورٹ_Get_Core_ID( خالی )
{
    واپس (بنیاد_Type_t) esp_cpu_get_core_id();
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

ساکن اندرونی بنیاد_Type_t x_پورٹ_اندر_Isr_Context(خالی)
{
    //Just call the FreeRTOS port interface version
    واپس x_پورٹ_Check_If_اندر_ISR();
}

بنیاد_Type_t x_پورٹ_Interrupted_From_ISRContext(خالی);

ساکن اندرونی UBaseType_t x_پورٹ_سیٹ_مداخلت_نقاب_From_ISR(خالی)
{
    UBaseType_t پچھلا_int_level = XTOS_سیٹ_INTLEVEL(XCHAL_EXCM_LEVEL);
    portbenchmark_مداخلت_DISABLE();
    واپس پچھلا_int_level;
}

ساکن اندرونی خالی v_پورٹ_Clear_مداخلت_نقاب_From_ISR(UBaseType_t پچھلا_level)
{
    portbenchmark_مداخلت_RESTORE(پچھلا_level);
    XTOS_RESTORE_JUST_INTLEVEL((صحیح) پچھلا_level);
}

// ---------------------- Spinlocks ------------------------

ساکن اندرونی bool __attribute__((always_inline)) v_پورٹ_CPUAcquire_Mutex_Timeout(پورٹ_MUX_TYPE *mux, صحیح timeout)
{
    واپس (spinlock_acquire(mux, timeout));
}

ساکن اندرونی خالی __attribute__((always_inline)) v_پورٹ_CPURelease_Mutex(پورٹ_MUX_TYPE *mux)
{
    spinlock_release(mux);
}

// ------------------ Critical Sections --------------------

#اگر CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
/**
 * @brief Claim thread-safe region start
 *        If claimed, vPortEnterCritical/vPortExitCritical on the current core are no-ops.
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
خالی x_پورٹ_Thread_محفوظ_Claim(خالی);

/**
 * @brief Claim thread-safe region end
 *        Restores normal port critical behavior
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
خالی x_پورٹ_Thread_محفوظ_Disclaim(خالی);

خارجی متلون bool پورٹ_x_Thread_محفوظ_Claimed;
#اگر_ختم /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

بنیاد_Type_t x_پورٹ_Enter_Critical_Timeout(پورٹ_MUX_TYPE *تالا_2, بنیاد_Type_t timeout);

ساکن اندرونی خالی __attribute__((always_inline)) v_پورٹ_Enter_Critical_IDF(پورٹ_MUX_TYPE *تالا_2)
{
    x_پورٹ_Enter_Critical_Timeout(تالا_2, پورٹ_MUX_NO_TIMEOUT);
}

خالی v_پورٹ_Exit_Critical_IDF(پورٹ_MUX_TYPE *تالا_2);

/*
In case any IDF libs called the port critical functions directly instead of through the macros.
The will just call the vPort...CriticalIDF() functions
*/
خالی v_پورٹ_Enter_Critical(پورٹ_MUX_TYPE *تالا_2);
خالی v_پورٹ_Exit_Critical(پورٹ_MUX_TYPE *تالا_2);

//IDF task critical sections
#تعریف_کرو پورٹ_TRY_ENTER_CRITICAL(تالا_2, timeout)       x_پورٹ_Enter_Critical_Timeout(تالا_2, timeout)
#تعریف_کرو پورٹ_ENTER_CRITICAL_IDF(تالا_2)                v_پورٹ_Enter_Critical_IDF(تالا_2)
#تعریف_کرو پورٹ_EXIT_CRITICAL_IDF(تالا_2)                 v_پورٹ_Exit_Critical_IDF(تالا_2)
//IDF ISR critical sections
#تعریف_کرو پورٹ_TRY_ENTER_CRITICAL_ISR(تالا_2, timeout)   x_پورٹ_Enter_Critical_Timeout(تالا_2, timeout)
#تعریف_کرو پورٹ_ENTER_CRITICAL_ISR(تالا_2)                v_پورٹ_Enter_Critical_IDF(تالا_2)
#تعریف_کرو پورٹ_EXIT_CRITICAL_ISR(تالا_2)                 v_پورٹ_Exit_Critical_IDF(تالا_2)
//IDF safe critical sections (they're the same)
#تعریف_کرو پورٹ_ENTER_CRITICAL_محفوظ(تالا_2)               v_پورٹ_Enter_Critical_IDF(تالا_2)
#تعریف_کرو پورٹ_EXIT_CRITICAL_محفوظ(تالا_2)                v_پورٹ_Exit_Critical_IDF(تالا_2)

// ---------------------- Yielding -------------------------

ساکن اندرونی bool x_پورٹ_Can_Yield(خالی)
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

    واپس ((ps_reg & PS_INTLEVEL_نقاب) == 0);
}

// Defined even for configNUMBER_OF_CORES > 1 for IDF compatibility
#تعریف_کرو پورٹ_YIELD_WITHIN_API()                      esp_crosscore_int_send_yield(x_پورٹ_Get_Core_ID())

// ----------------------- System --------------------------

خالی v_پورٹ_سیٹ_ڈھیر_Watchpoint(خالی *px_ڈھیر_آغاز);

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
bool x_پورٹ_Check_Valid_List_Mem(ثابت خالی *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool x_پورٹ_Check_Valid_TCBMem(ثابت خالی *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_ڈھیر_Mem(ثابت خالی *ptr);

#تعریف_کرو پورٹ_VALID_LIST_MEM(ptr)     x_پورٹ_Check_Valid_List_Mem(ptr)
#تعریف_کرو پورٹ_VALID_TCB_MEM(ptr)      x_پورٹ_Check_Valid_TCBMem(ptr)
#تعریف_کرو پورٹ_VALID_ڈھیر_MEM(ptr)    x_Portcheck_Valid_ڈھیر_Mem(ptr)

/* ------------------------------------------------------ Misc ---------------------------------------------------------
 * - Miscellaneous porting macros
 * - These are not part of the FreeRTOS porting interface, but are used by other FreeRTOS dependent components
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- App-Trace -------------------------

#اگر CONFIG_ESP_TRACE_ENABLE
خارجی متلون بنیاد_Type_t x_پورٹ_Switch_Flag;
#تعریف_کرو os_task_switch_is_pended(_cpu_) (x_پورٹ_Switch_Flag)
#ورنہ
#تعریف_کرو os_task_switch_is_pended(_cpu_) (false)
#اگر_ختم

// --------------- Compatibility Includes ------------------
/*
ESP-IDF currently does not have a "Include what you use" policy. A lot of files implicitly pull in API through
portmacro.h. Therefore, we need to keep these headers around for now to allow the rest of IDF to compile.

[refactor-todo] Clean up ESP-IDF inclusion dependencies and add a inclusion check.
*/

#شامل <stdlib.h>
#شامل <stdbool.h>
#شامل <stdarg.h>
#شامل <xtensa/hal.h>
#شامل "esp_attr.h"
#شامل "esp_newlib.h"
#شامل "esp_heap_caps.h"
#شامل "esp_rom_sys.h"
#شامل "esp_system.h"
#شامل "portbenchmark.h"
#شامل <limits.h>
#شامل <xtensa/config/system.h>
#شامل <xtensa_api.h>

#اگر_تعریف_شدہ __cplusplus
}
#اگر_ختم
