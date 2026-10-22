/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#رهنمود once

#ضمن "sdkconfig.h"

/* Macros used instead ofsetoff() for better performance of interrupt handler */
#إذا CONFIG_FREERTOS_USE_LIST_داده_INTEGRITY_CHECK_بایتها
/*
pxTopOfStack (4) +
xStateListItem (28) +
xEventListItem (28) +
uxPriority (4)
*/
#تعریف_کن پورت_جابه_جایی_PX_پشته 0x40
#وإلا
/*
pxTopOfStack (4) +
xStateListItem (20) +
xEventListItem (20) +
uxPriority (4)
*/
#تعریف_کن پورت_جابه_جایی_PX_پشته 0x30
#پایان_اگر /* #if CONFIG_FREERTOS_USE_LIST_DATA_INTEGRITY_CHECK_BYTES */

#إذا ( config_عدد_از_CORES > 1 )
#تعریف_کن پورت_TCB_CORE_FIELDS_اندازه 8
#وإلا
#تعریف_کن پورت_TCB_CORE_FIELDS_اندازه 0
#پایان_اگر

#إذا ( configUSE_TASK_PREEMPTION_DISABLE == 1 )
#تعریف_کن پورت_TCB_PREEMPT_DISABLE_فیلد_اندازه 4
#وإلا
#تعریف_کن پورت_TCB_PREEMPT_DISABLE_فیلد_اندازه 0
#پایان_اگر

/* Align the value up to the nearest multiple of 4 */
#تعریف_کن پورت_ALIGN_به_بالا_مقصد_4(مقدار) (((مقدار) + 3) & ~3)

#تعریف_کن پورت_جابه_جایی_PX_انتها_از_پشته ( \
    پورت_جابه_جایی_PX_پشته \
    + 4                                 /* StackType_t * pxStack */ \
    + پورت_TCB_CORE_FIELDS_اندازه                     /* BaseType_t xDummy23 + UBaseType_t uxDummy24 */ \
    + پورت_ALIGN_به_بالا_مقصد_4(configMAX_TASK_NAME_LEN)   /* pcTaskName[ configMAX_TASK_NAME_LEN ] */ \
    + پورت_TCB_PREEMPT_DISABLE_فیلد_اندازه           /* BaseType_t xDummy25 */ \
)

#اگر_تعریف_نشده __ASSEMBLER__

#ضمن <stdint.h>
#ضمن "spinlock.h"
#ضمن "soc/interrupt_reg.h"
#ضمن "esp_macros.h"
#ضمن "esp_cpu.h"
#ضمن "esp_private/crosscore_int.h"
#ضمن "esp_memory_utils.h"

#اگر_تعریف_شده __cplusplus
خارجي "C" {
#پایان_اگر

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come first as they are used further down in this file
 * ------------------------------------------------------------------------------------------------------------------ */

#تعریف_کن پورت_CHAR                    uint8_t
#تعریف_کن پورت_FLOAT                   عائم
#تعریف_کن پورت_DOUBLE                  مزدوج
#تعریف_کن پورت_LONG                    int32_t
#تعریف_کن پورت_SHORT                   int16_t
#تعریف_کن پورت_پشته_TYPE              uint8_t
#تعریف_کن پورت_پایه_TYPE               صحيح

تعريفنوع پورت_پشته_TYPE              پشته_Type_t;
تعريفنوع پورت_پایه_TYPE               پایه_Type_t;
تعريفنوع بیعلامت پورت_پایه_TYPE      UBaseType_t;

#إذا ( config_USE_16_بیت_TICKS == 1 )
تعريفنوع uint16_t TickType_t;
#تعریف_کن پورت_MAX_DELAY ( TickType_t )  0xffff
#وإلا
تعريفنوع uint32_t TickType_t;
#تعریف_کن پورت_MAX_DELAY ( TickType_t )  0xffffffffUL
#پایان_اگر

/* Task function macros as described on the FreeRTOS.org WEB site. */
#تعریف_کن پورت_TASK_FUNCTION_PROTO( vFunction, pvParameters )  فراغ vFunction( فراغ *pvParameters )
#تعریف_کن پورت_TASK_FUNCTION( vFunction, pvParameters )          فراغ vFunction( فراغ *pvParameters )

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#تعریف_کن پورت_CRITICAL_NESTING_درون_TCB     1
#تعریف_کن پورت_پشته_GROWTH                ( -1 )
#تعریف_کن پورت_TICK_PERIOD_MS              ( ( TickType_t ) ( 1000 / config_TICK_نرخ_HZ ) )
#تعریف_کن پورت_BYTE_ALIGNMENT              16
#تعریف_کن پورت_NOP() __asm متطاير        (" nop ")

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros needed to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

/* ---------------------- Spinlocks ------------------------
 - Spinlocks added to match API with SMP FreeRTOS. Single core RISC-V does not need spin locks
 * ------------------------------------------------------ */

تعريفنوع spinlock_t                          پورت_MUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */

#تعریف_کن پورت_MUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#تعریف_کن پورت_MUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#تعریف_کن پورت_MUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#تعریف_کن پورت_MUX_TRY_قفل                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#تعریف_کن پورت_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// --------------------- Interrupts ------------------------

پایه_Type_t x_پورت_Check_If_درون_ISR(فراغ);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortCheckIfInISR() internally
 */
فراغ v_پورت_Assert_If_درون_ISR(فراغ);

// ------------------ Critical Sections --------------------

#إذا ( config_عدد_از_CORES > 1 )
/*
These are always called with interrupts already disabled. We simply need to get/release the spinlocks
*/
خارجي پورت_MUX_TYPE پورت_x_Task_قفل;
خارجي پورت_MUX_TYPE پورت_x_ISRLock;

فراغ v_پورت_Take_قفل( پورت_MUX_TYPE *قفل_2 );
فراغ v_پورت_Release_قفل( پورت_MUX_TYPE *قفل_2 );
#پایان_اگر /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

فراغ v_پورت_Yield( فراغ );
ساكن مضمن فراغ __attribute__((always_inline)) v_پورت_Yield_Core( پایه_Type_t xCoreID );

/**
 * @brief Set interrupt mask and return current interrupt enable register
 *
 * @return UBaseType_t Current interrupt enable register before set
 */
UBaseType_t ul_پورت_مجموعه_وقفه_ماسک(فراغ);

/**
 * @brief Clear current interrupt mask and set given mask
 *
 * @param mask Interrupt mask
 */
فراغ v_پورت_Clear_وقفه_ماسک(UBaseType_t ماسک);

/**
 * @brief Perform a context switch from an ISR
 */
فراغ v_پورت_Yield_From_ISR(فراغ);

#تعریف_کن پورت_YIELD_FROM_ISR_CHECK(x)     ({ \
    إذا ( (x) == pdTRUE ) { \
        v_پورت_Yield_From_ISR(); \
    } \
})
#تعریف_کن پورت_YIELD_FROM_ISR_NO_CHECK()               v_پورت_Yield_From_ISR()

// ----------------------- System --------------------------

ساكن مضمن پایه_Type_t __attribute__((always_inline)) x_پورت_Get_Core_ID( فراغ );

// --------------------- TCB Cleanup -----------------------

/**
 * @brief TCB cleanup hook
 *
 * The portCLEAN_UP_TCB() macro is called in prvDeleteTCB() right before a
 * deleted task's memory is freed. We map that macro to this internal function
 * so that IDF FreeRTOS ports can inject some task pre-deletion operations.
 */
فراغ v_پورت_TCBPre_Delete_Hook( فراغ *pxTCB );

/* ------------------------------------------- FreeRTOS Porting Interface ----------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS
 * - Most come after forward declare as porting macros map to declared functions
 * - Maps to forward declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

#تعریف_کن پورت_DISABLE_INTERRUPTS()                    ul_پورت_مجموعه_وقفه_ماسک()
#إذا !SOC_INT_CLIC_SUPPORTED
#تعریف_کن پورت_ENABLE_INTERRUPTS()                     v_پورت_Clear_وقفه_ماسک(RVHAL_INTR_ENABLE_THRESH)
#وإلا
#تعریف_کن پورت_ENABLE_INTERRUPTS()                     v_پورت_Clear_وقفه_ماسک(RVHAL_INTR_ENABLE_THRESH_CLIC)
#پایان_اگر /* !SOC_INT_CLIC_SUPPORTED */
#تعریف_کن پورت_RESTORE_INTERRUPTS(x)                   v_پورت_Clear_وقفه_ماسک(x)

#تعریف_کن پورت_مجموعه_وقفه_ماسک_FROM_ISR() ({           \
    بیعلامت صحيح cur_level; \
    cur_level = ul_پورت_مجموعه_وقفه_ماسک(); \
    cur_level; \
})
#تعریف_کن پورت_CLEAR_وقفه_ماسک_FROM_ISR(x)        پورت_RESTORE_INTERRUPTS(x)
#تعریف_کن پورت_مجموعه_وقفه_ماسک()                    پورت_مجموعه_وقفه_ماسک_FROM_ISR()
#تعریف_کن پورت_CLEAR_وقفه_ماسک(x)                 پورت_CLEAR_وقفه_ماسک_FROM_ISR(x)

/**
 * @brief Assert if in ISR context
 *
 * TODO: Enable once ISR safe version of vTaskEnter/ExitCritical() is implemented
 * for single-core SMP FreeRTOS Kernel. (IDF-10540)
 */
// #define portASSERT_IF_IN_ISR() vPortAssertIfInISR()


// ------------------ Critical Sections --------------------

#إذا ( config_عدد_از_CORES > 1 )
#تعریف_کن پورت_GET_TASK_قفل()                         v_پورت_Take_قفل(&پورت_x_Task_قفل)
#تعریف_کن پورت_RELEASE_TASK_قفل()                     v_پورت_Release_قفل(&پورت_x_Task_قفل)
#تعریف_کن پورت_GET_ISR_قفل()                          v_پورت_Take_قفل(&پورت_x_ISRLock)
#تعریف_کن پورت_RELEASE_ISR_قفل()                      v_پورت_Release_قفل(&پورت_x_ISRLock)
#پایان_اگر /* configNUMBER_OF_CORES > 1 */

//Critical sections used by FreeRTOS SMP
خارجي فراغ vTaskEnterCritical( فراغ );
خارجي فراغ vTaskExitCritical( فراغ );
#تعریف_کن پورت_ENTER_CRITICAL_SMP()                    vTaskEnterCritical();
#تعریف_کن پورت_EXIT_CRITICAL_SMP()                     vTaskExitCritical();

#إذا defined(__cplusplus) && (__cplusplus >  201703L)
#تعریف_کن پورت_ENTER_CRITICAL(...)                     انتخاب_MACRO_VA_ARG(پورت_ENTER_CRITICAL_IDF, پورت_ENTER_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#تعریف_کن پورت_EXIT_CRITICAL(...)                      انتخاب_MACRO_VA_ARG(پورت_EXIT_CRITICAL_IDF, پورت_EXIT_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#وإلا
#تعریف_کن پورت_ENTER_CRITICAL(...)                     انتخاب_MACRO_VA_ARG(پورت_ENTER_CRITICAL_IDF, پورت_ENTER_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#تعریف_کن پورت_EXIT_CRITICAL(...)                      انتخاب_MACRO_VA_ARG(پورت_EXIT_CRITICAL_IDF, پورت_EXIT_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#پایان_اگر

#تعریف_کن پورت_ENTER_CRITICAL_FROM_ISR() vTaskEnterCriticalFromISR()
#تعریف_کن پورت_EXIT_CRITICAL_FROM_ISR(x) vTaskExitCriticalFromISR(x)

// ---------------------- Yielding -------------------------

#تعریف_کن پورت_YIELD()                                 v_پورت_Yield()
#إذا defined(__cplusplus) && (__cplusplus >  201703L)
#تعریف_کن پورت_YIELD_FROM_ISR(...)                     انتخاب_MACRO_VA_ARG(پورت_YIELD_FROM_ISR_CHECK, پورت_YIELD_FROM_ISR_NO_CHECK __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#وإلا
#تعریف_کن پورت_YIELD_FROM_ISR(...)                     انتخاب_MACRO_VA_ARG(پورت_YIELD_FROM_ISR_CHECK, پورت_YIELD_FROM_ISR_NO_CHECK, ##__VA_ARGS__)(__VA_ARGS__)
#پایان_اگر
#تعریف_کن پورت_YIELD_CORE(x)                           v_پورت_Yield_Core(x)

// ----------------------- System --------------------------

#تعریف_کن پورت_GET_CORE_ID()                           x_پورت_Get_Core_ID()
#تعریف_کن پورت_CHECK_IF_درون_ISR()                       x_پورت_Check_If_درون_ISR()

// ------------------- Run Time Stats ----------------------

#تعریف_کن پورت_CONFIGURE_TIMER_FOR_پیاپی_زمان_STATS()
#إذا ( CONFIG_FREERTOS_GENERATE_پیاپی_زمان_STATS )
config_پیاپی_زمان_COUNTER_TYPE x_پورت_Get_پیاپی_زمان_Counter_مقدار( فراغ );
#تعریف_کن پورت_GET_پیاپی_زمان_COUNTER_مقدار()        x_پورت_Get_پیاپی_زمان_Counter_مقدار()
#پایان_اگر

// --------------------- TCB Cleanup -----------------------

#تعریف_کن پورت_CLEAN_به_بالا_TCB( pxTCB )                   v_پورت_TCBPre_Delete_Hook( pxTCB )


/* --------------------------------------------- Inline Implementations ------------------------------------------------
 * - Implementation of inline functions of the forward declares
 * - Should come after forward declare and FreeRTOS Porting interface, as implementation may use both.
 * - For implementation of non-inlined functions, see port.c, port_common.c, or other assembly files
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Yielding -------------------------

ساكن مضمن فراغ __attribute__((always_inline)) v_پورت_Yield_Core( پایه_Type_t xCoreID )
{
    esp_crosscore_int_send_yield( xCoreID );
}

// ----------------------- System --------------------------

ساكن مضمن پایه_Type_t __attribute__((always_inline)) x_پورت_Get_Core_ID( فراغ )
{
    أرجع (پایه_Type_t) esp_cpu_get_core_id();
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

ساكن مضمن پایه_Type_t x_پورت_درون_Isr_Context(فراغ)
{
    //Just call the FreeRTOS port interface version
    أرجع x_پورت_Check_If_درون_ISR();
}

// Added for backward compatibility with IDF
#تعریف_کن x_پورت_Interrupted_From_ISRContext()    x_پورت_درون_Isr_Context()

// ------------------ Critical Sections --------------------

/*
IDF style critical sections which are orthogonal to FreeRTOS critical sections. However, on single core, the IDF style
critical sections simply disable interrupts, thus we discard the lock and timeout arguments.

Note: The IDF Style critical sections are named differently to their counterparts present in the xtensa port as few IDF
examples such as esp_zigbee_gateway, when compiled for RISC-V targets, have a reference to vPortEnterCritical()
and vPortExitCritical() from precompiled libraries (.a) thereby failing linking.
*/
فراغ v_پورت_Enter_Critical(فراغ);
فراغ v_پورت_Exit_Critical(فراغ);

#إذا CONFIG_FREERTOS_پورت_THREAD_امن_CLAIM
/**
 * @brief Claim thread-safe region start
 *        If claimed, vPortEnterCritical/vPortExitCritical on the current core are no-ops.
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
فراغ x_پورت_Thread_امن_Claim(فراغ);

/**
 * @brief Claim thread-safe region end
 *        Restores normal port critical behavior
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
فراغ x_پورت_Thread_امن_Disclaim(فراغ);

خارجي متطاير bool پورت_x_Thread_امن_Claimed;
#پایان_اگر /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

//IDF task critical sections
#تعریف_کن پورت_TRY_ENTER_CRITICAL(قفل_2, timeout)       ({(فراغ) قفل_2; (فراغ) timeout; v_پورت_Enter_Critical(); pdPASS;})
#تعریف_کن پورت_ENTER_CRITICAL_IDF(قفل_2)                ({(فراغ) قفل_2; v_پورت_Enter_Critical();})
#تعریف_کن پورت_EXIT_CRITICAL_IDF(قفل_2)                 ({(فراغ) قفل_2; v_پورت_Exit_Critical();})
//IDF ISR critical sections
#تعریف_کن پورت_TRY_ENTER_CRITICAL_ISR(قفل_2, timeout)   ({(فراغ) قفل_2; (فراغ) timeout; v_پورت_Enter_Critical(); pdPASS;})
#تعریف_کن پورت_ENTER_CRITICAL_ISR(قفل_2)                ({(فراغ) قفل_2; v_پورت_Enter_Critical();})
#تعریف_کن پورت_EXIT_CRITICAL_ISR(قفل_2)                 ({(فراغ) قفل_2; v_پورت_Exit_Critical();})
//IDF safe critical sections (they're the same)
#تعریف_کن پورت_ENTER_CRITICAL_امن(قفل_2)               ({(فراغ) قفل_2; v_پورت_Enter_Critical();})
#تعریف_کن پورت_EXIT_CRITICAL_امن(قفل_2)                ({(فراغ) قفل_2; v_پورت_Exit_Critical();})

// ---------------------- Yielding -------------------------

ساكن مضمن bool x_پورت_Can_Yield(فراغ)
{
#إذا SOC_INT_CLIC_SUPPORTED
    /* When CLIC is supported:
     *  - The lowest interrupt threshold level is 0. Therefore, an interrupt threshold level above 0 would mean that we
     *    are in a critical section.
     *  - Since CLIC enables HW interrupt nesting, we do not have the updated interrupt level in the
     *    INTERRUPT_CURRENT_CORE_INT_THRESH_REG register when nested interrupts occur. To know the current interrupt
     *    level, we read the machine-mode interrupt level (mil) field from the mintstatus CSR. A non-zero value indicates
     *    that we are in an interrupt context.
     */
    uint32_t آستانه = rv_utils_get_وقفه_آستانه();
    uint32_t intr_level = rv_utils_get_وقفه_level();
    أرجع ((intr_level == 0) && (آستانه == 0));
#وإلا/* !SOC_INT_CLIC_SUPPORTED */
    uint32_t آستانه = REG_READ(وقفه_CURRENT_CORE_INT_THRESH_REG);
    /* when enter critical code, FreeRTOS will mask threshold to RVHAL_EXCM_LEVEL
     * and exit critical code, will recover threshold value (1). so threshold <= 1
     * means not in critical code
     */
    أرجع (آستانه <= 1);
#پایان_اگر
}

// Defined even for configNUMBER_OF_CORES > 1 for IDF compatibility
#تعریف_کن پورت_YIELD_WITHIN_API()                      esp_crosscore_int_send_yield(x_پورت_Get_Core_ID())

// ----------------------- System --------------------------

فراغ v_پورت_مجموعه_پشته_Watchpoint(فراغ *px_پشته_شروع);

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
bool x_پورت_Check_Valid_List_Mem(ثابت فراغ *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool x_پورت_Check_Valid_TCBMem(ثابت فراغ *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_پشته_Mem(ثابت فراغ *ptr);

#تعریف_کن پورت_VALID_LIST_MEM(ptr)     x_پورت_Check_Valid_List_Mem(ptr)
#تعریف_کن پورت_VALID_TCB_MEM(ptr)      x_پورت_Check_Valid_TCBMem(ptr)
#تعریف_کن پورت_VALID_پشته_MEM(ptr)    x_Portcheck_Valid_پشته_Mem(ptr)

/* ------------------------------------------------------ Misc ---------------------------------------------------------
 * - Miscellaneous porting macros
 * - These are not part of the FreeRTOS porting interface, but are used by other FreeRTOS dependent components
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- App-Trace -------------------------

#إذا CONFIG_ESP_TRACE_ENABLE
خارجي متطاير پایه_Type_t x_پورت_Switch_Flag;
#تعریف_کن os_task_switch_is_pended(_cpu_) (x_پورت_Switch_Flag)
#وإلا
#تعریف_کن os_task_switch_is_pended(_cpu_) (false)
#پایان_اگر

// --------------- Compatibility Includes ------------------
/*
ESP-IDF currently does not have a "Include what you use" policy. A lot of files implicitly pull in API through
portmacro.h. Therefore, we need to keep these headers around for now to allow the rest of IDF to compile.

[refactor-todo] Clean up ESP-IDF inclusion dependencies and add a inclusion check.
*/

#ضمن <stdlib.h>
#ضمن <stdbool.h>
#ضمن <stdarg.h>
#ضمن <limits.h>
#ضمن "esp_attr.h"
#ضمن "esp_newlib.h"
#ضمن "esp_heap_caps.h"
#ضمن "esp_rom_sys.h"
#ضمن "esp_system.h"             /* required by esp_get_...() functions in portable.h. [refactor-todo] Update portable.h */


#اگر_تعریف_شده __cplusplus
}
#پایان_اگر

#پایان_اگر // __ASSEMBLER__
