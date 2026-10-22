/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ہدایت once

#شامل "sdkconfig.h"

/* Macros used instead ofsetoff() for better performance of interrupt handler */
#اگر CONFIG_FREERTOS_USE_LIST_ڈیٹا_INTEGRITY_CHECK_بائٹیں
/*
pxTopOfStack (4) +
xStateListItem (28) +
xEventListItem (28) +
uxPriority (4)
*/
#تعریف_کرو پورٹ_انحراف_PX_ڈھیر 0x40
#ورنہ
/*
pxTopOfStack (4) +
xStateListItem (20) +
xEventListItem (20) +
uxPriority (4)
*/
#تعریف_کرو پورٹ_انحراف_PX_ڈھیر 0x30
#اگر_ختم /* #if CONFIG_FREERTOS_USE_LIST_DATA_INTEGRITY_CHECK_BYTES */

#اگر ( config_نمبر_کا_CORES > 1 )
#تعریف_کرو پورٹ_TCB_CORE_FIELDS_سائز 8
#ورنہ
#تعریف_کرو پورٹ_TCB_CORE_FIELDS_سائز 0
#اگر_ختم

#اگر ( configUSE_TASK_PREEMPTION_DISABLE == 1 )
#تعریف_کرو پورٹ_TCB_PREEMPT_DISABLE_میدان_سائز 4
#ورنہ
#تعریف_کرو پورٹ_TCB_PREEMPT_DISABLE_میدان_سائز 0
#اگر_ختم

/* Align the value up to the nearest multiple of 4 */
#تعریف_کرو پورٹ_ALIGN_اوپر_مقصد_4(قدر) (((قدر) + 3) & ~3)

#تعریف_کرو پورٹ_انحراف_PX_انتہا_کا_ڈھیر ( \
    پورٹ_انحراف_PX_ڈھیر \
    + 4                                 /* StackType_t * pxStack */ \
    + پورٹ_TCB_CORE_FIELDS_سائز                     /* BaseType_t xDummy23 + UBaseType_t uxDummy24 */ \
    + پورٹ_ALIGN_اوپر_مقصد_4(configMAX_TASK_NAME_LEN)   /* pcTaskName[ configMAX_TASK_NAME_LEN ] */ \
    + پورٹ_TCB_PREEMPT_DISABLE_میدان_سائز           /* BaseType_t xDummy25 */ \
)

#اگر_غیر_تعریف_شدہ __ASSEMBLER__

#شامل <stdint.h>
#شامل "spinlock.h"
#شامل "soc/interrupt_reg.h"
#شامل "esp_macros.h"
#شامل "esp_cpu.h"
#شامل "esp_private/crosscore_int.h"
#شامل "esp_memory_utils.h"

#اگر_تعریف_شدہ __cplusplus
خارجی "C" {
#اگر_ختم

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come first as they are used further down in this file
 * ------------------------------------------------------------------------------------------------------------------ */

#تعریف_کرو پورٹ_CHAR                    uint8_t
#تعریف_کرو پورٹ_FLOAT                   اعشاری
#تعریف_کرو پورٹ_DOUBLE                  دوہرا
#تعریف_کرو پورٹ_LONG                    int32_t
#تعریف_کرو پورٹ_SHORT                   int16_t
#تعریف_کرو پورٹ_ڈھیر_TYPE              uint8_t
#تعریف_کرو پورٹ_بنیاد_TYPE               صحیح

قسمتعریف پورٹ_ڈھیر_TYPE              ڈھیر_Type_t;
قسمتعریف پورٹ_بنیاد_TYPE               بنیاد_Type_t;
قسمتعریف غیرعلامتی پورٹ_بنیاد_TYPE      UBaseType_t;

#اگر ( config_USE_16_ثنائی_ہندسہ_TICKS == 1 )
قسمتعریف uint16_t TickType_t;
#تعریف_کرو پورٹ_MAX_DELAY ( TickType_t )  0xffff
#ورنہ
قسمتعریف uint32_t TickType_t;
#تعریف_کرو پورٹ_MAX_DELAY ( TickType_t )  0xffffffffUL
#اگر_ختم

/* Task function macros as described on the FreeRTOS.org WEB site. */
#تعریف_کرو پورٹ_TASK_FUNCTION_PROTO( vFunction, pvParameters )  خالی vFunction( خالی *pvParameters )
#تعریف_کرو پورٹ_TASK_FUNCTION( vFunction, pvParameters )          خالی vFunction( خالی *pvParameters )

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#تعریف_کرو پورٹ_CRITICAL_NESTING_اندر_TCB     1
#تعریف_کرو پورٹ_ڈھیر_GROWTH                ( -1 )
#تعریف_کرو پورٹ_TICK_PERIOD_MS              ( ( TickType_t ) ( 1000 / config_TICK_شرح_HZ ) )
#تعریف_کرو پورٹ_BYTE_ALIGNMENT              16
#تعریف_کرو پورٹ_NOP() __asm متلون        (" nop ")

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros needed to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

/* ---------------------- Spinlocks ------------------------
 - Spinlocks added to match API with SMP FreeRTOS. Single core RISC-V does not need spin locks
 * ------------------------------------------------------ */

قسمتعریف spinlock_t                          پورٹ_MUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */

#تعریف_کرو پورٹ_MUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#تعریف_کرو پورٹ_MUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#تعریف_کرو پورٹ_MUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#تعریف_کرو پورٹ_MUX_TRY_تالا                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#تعریف_کرو پورٹ_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// --------------------- Interrupts ------------------------

بنیاد_Type_t x_پورٹ_Check_If_اندر_ISR(خالی);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortCheckIfInISR() internally
 */
خالی v_پورٹ_Assert_If_اندر_ISR(خالی);

// ------------------ Critical Sections --------------------

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

/**
 * @brief Set interrupt mask and return current interrupt enable register
 *
 * @return UBaseType_t Current interrupt enable register before set
 */
UBaseType_t ul_پورٹ_سیٹ_مداخلت_نقاب(خالی);

/**
 * @brief Clear current interrupt mask and set given mask
 *
 * @param mask Interrupt mask
 */
خالی v_پورٹ_Clear_مداخلت_نقاب(UBaseType_t نقاب);

/**
 * @brief Perform a context switch from an ISR
 */
خالی v_پورٹ_Yield_From_ISR(خالی);

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

/* ------------------------------------------- FreeRTOS Porting Interface ----------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS
 * - Most come after forward declare as porting macros map to declared functions
 * - Maps to forward declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

#تعریف_کرو پورٹ_DISABLE_INTERRUPTS()                    ul_پورٹ_سیٹ_مداخلت_نقاب()
#اگر !SOC_INT_CLIC_SUPPORTED
#تعریف_کرو پورٹ_ENABLE_INTERRUPTS()                     v_پورٹ_Clear_مداخلت_نقاب(RVHAL_INTR_ENABLE_THRESH)
#ورنہ
#تعریف_کرو پورٹ_ENABLE_INTERRUPTS()                     v_پورٹ_Clear_مداخلت_نقاب(RVHAL_INTR_ENABLE_THRESH_CLIC)
#اگر_ختم /* !SOC_INT_CLIC_SUPPORTED */
#تعریف_کرو پورٹ_RESTORE_INTERRUPTS(x)                   v_پورٹ_Clear_مداخلت_نقاب(x)

#تعریف_کرو پورٹ_سیٹ_مداخلت_نقاب_FROM_ISR() ({           \
    غیرعلامتی صحیح cur_level; \
    cur_level = ul_پورٹ_سیٹ_مداخلت_نقاب(); \
    cur_level; \
})
#تعریف_کرو پورٹ_CLEAR_مداخلت_نقاب_FROM_ISR(x)        پورٹ_RESTORE_INTERRUPTS(x)
#تعریف_کرو پورٹ_سیٹ_مداخلت_نقاب()                    پورٹ_سیٹ_مداخلت_نقاب_FROM_ISR()
#تعریف_کرو پورٹ_CLEAR_مداخلت_نقاب(x)                 پورٹ_CLEAR_مداخلت_نقاب_FROM_ISR(x)

/**
 * @brief Assert if in ISR context
 *
 * TODO: Enable once ISR safe version of vTaskEnter/ExitCritical() is implemented
 * for single-core SMP FreeRTOS Kernel. (IDF-10540)
 */
// #define portASSERT_IF_IN_ISR() vPortAssertIfInISR()


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
 * - For implementation of non-inlined functions, see port.c, port_common.c, or other assembly files
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Yielding -------------------------

ساکن اندرونی خالی __attribute__((always_inline)) v_پورٹ_Yield_Core( بنیاد_Type_t xCoreID )
{
    esp_crosscore_int_send_yield( xCoreID );
}

// ----------------------- System --------------------------

ساکن اندرونی بنیاد_Type_t __attribute__((always_inline)) x_پورٹ_Get_Core_ID( خالی )
{
    واپس (بنیاد_Type_t) esp_cpu_get_core_id();
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

ساکن اندرونی بنیاد_Type_t x_پورٹ_اندر_Isr_Context(خالی)
{
    //Just call the FreeRTOS port interface version
    واپس x_پورٹ_Check_If_اندر_ISR();
}

// Added for backward compatibility with IDF
#تعریف_کرو x_پورٹ_Interrupted_From_ISRContext()    x_پورٹ_اندر_Isr_Context()

// ------------------ Critical Sections --------------------

/*
IDF style critical sections which are orthogonal to FreeRTOS critical sections. However, on single core, the IDF style
critical sections simply disable interrupts, thus we discard the lock and timeout arguments.

Note: The IDF Style critical sections are named differently to their counterparts present in the xtensa port as few IDF
examples such as esp_zigbee_gateway, when compiled for RISC-V targets, have a reference to vPortEnterCritical()
and vPortExitCritical() from precompiled libraries (.a) thereby failing linking.
*/
خالی v_پورٹ_Enter_Critical(خالی);
خالی v_پورٹ_Exit_Critical(خالی);

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

//IDF task critical sections
#تعریف_کرو پورٹ_TRY_ENTER_CRITICAL(تالا_2, timeout)       ({(خالی) تالا_2; (خالی) timeout; v_پورٹ_Enter_Critical(); pdPASS;})
#تعریف_کرو پورٹ_ENTER_CRITICAL_IDF(تالا_2)                ({(خالی) تالا_2; v_پورٹ_Enter_Critical();})
#تعریف_کرو پورٹ_EXIT_CRITICAL_IDF(تالا_2)                 ({(خالی) تالا_2; v_پورٹ_Exit_Critical();})
//IDF ISR critical sections
#تعریف_کرو پورٹ_TRY_ENTER_CRITICAL_ISR(تالا_2, timeout)   ({(خالی) تالا_2; (خالی) timeout; v_پورٹ_Enter_Critical(); pdPASS;})
#تعریف_کرو پورٹ_ENTER_CRITICAL_ISR(تالا_2)                ({(خالی) تالا_2; v_پورٹ_Enter_Critical();})
#تعریف_کرو پورٹ_EXIT_CRITICAL_ISR(تالا_2)                 ({(خالی) تالا_2; v_پورٹ_Exit_Critical();})
//IDF safe critical sections (they're the same)
#تعریف_کرو پورٹ_ENTER_CRITICAL_محفوظ(تالا_2)               ({(خالی) تالا_2; v_پورٹ_Enter_Critical();})
#تعریف_کرو پورٹ_EXIT_CRITICAL_محفوظ(تالا_2)                ({(خالی) تالا_2; v_پورٹ_Exit_Critical();})

// ---------------------- Yielding -------------------------

ساکن اندرونی bool x_پورٹ_Can_Yield(خالی)
{
#اگر SOC_INT_CLIC_SUPPORTED
    /* When CLIC is supported:
     *  - The lowest interrupt threshold level is 0. Therefore, an interrupt threshold level above 0 would mean that we
     *    are in a critical section.
     *  - Since CLIC enables HW interrupt nesting, we do not have the updated interrupt level in the
     *    INTERRUPT_CURRENT_CORE_INT_THRESH_REG register when nested interrupts occur. To know the current interrupt
     *    level, we read the machine-mode interrupt level (mil) field from the mintstatus CSR. A non-zero value indicates
     *    that we are in an interrupt context.
     */
    uint32_t حد = rv_utils_get_مداخلت_حد();
    uint32_t intr_level = rv_utils_get_مداخلت_level();
    واپس ((intr_level == 0) && (حد == 0));
#ورنہ/* !SOC_INT_CLIC_SUPPORTED */
    uint32_t حد = REG_READ(مداخلت_CURRENT_CORE_INT_THRESH_REG);
    /* when enter critical code, FreeRTOS will mask threshold to RVHAL_EXCM_LEVEL
     * and exit critical code, will recover threshold value (1). so threshold <= 1
     * means not in critical code
     */
    واپس (حد <= 1);
#اگر_ختم
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
#شامل <limits.h>
#شامل "esp_attr.h"
#شامل "esp_newlib.h"
#شامل "esp_heap_caps.h"
#شامل "esp_rom_sys.h"
#شامل "esp_system.h"             /* required by esp_get_...() functions in portable.h. [refactor-todo] Update portable.h */


#اگر_تعریف_شدہ __cplusplus
}
#اگر_ختم

#اگر_ختم // __ASSEMBLER__
