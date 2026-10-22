/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#لارښوونه once

#ضمن "sdkconfig.h"

/* Macros used instead ofsetoff() for better performance of interrupt handler */
#إذا CONFIG_FREERTOS_USE_LIST_معلومات_INTEGRITY_CHECK_بایټونه
/*
pxTopOfStack (4) +
xStateListItem (28) +
xEventListItem (28) +
uxPriority (4)
*/
#تعریف_کړه پورټ_دننه_واټن_PX_ډېری 0x40
#وإلا
/*
pxTopOfStack (4) +
xStateListItem (20) +
xEventListItem (20) +
uxPriority (4)
*/
#تعریف_کړه پورټ_دننه_واټن_PX_ډېری 0x30
#د_که_پای /* #if CONFIG_FREERTOS_USE_LIST_DATA_INTEGRITY_CHECK_BYTES */

#إذا ( config_شمېره_د_CORES > 1 )
#تعریف_کړه پورټ_TCB_CORE_FIELDS_اندازه 8
#وإلا
#تعریف_کړه پورټ_TCB_CORE_FIELDS_اندازه 0
#د_که_پای

#إذا ( configUSE_TASK_PREEMPTION_DISABLE == 1 )
#تعریف_کړه پورټ_TCB_PREEMPT_DISABLE_ډګر_اندازه 4
#وإلا
#تعریف_کړه پورټ_TCB_PREEMPT_DISABLE_ډګر_اندازه 0
#د_که_پای

/* Align the value up to the nearest multiple of 4 */
#تعریف_کړه پورټ_ALIGN_پورته_منزل_4(ارزښت) (((ارزښت) + 3) & ~3)

#تعریف_کړه پورټ_دننه_واټن_PX_پای_ټکی_د_ډېری ( \
    پورټ_دننه_واټن_PX_ډېری \
    + 4                                 /* StackType_t * pxStack */ \
    + پورټ_TCB_CORE_FIELDS_اندازه                     /* BaseType_t xDummy23 + UBaseType_t uxDummy24 */ \
    + پورټ_ALIGN_پورته_منزل_4(configMAX_TASK_NAME_LEN)   /* pcTaskName[ configMAX_TASK_NAME_LEN ] */ \
    + پورټ_TCB_PREEMPT_DISABLE_ډګر_اندازه           /* BaseType_t xDummy25 */ \
)

#که_نه_تعریف_شوی __ASSEMBLER__

#ضمن <stdint.h>
#ضمن "spinlock.h"
#ضمن "soc/interrupt_reg.h"
#ضمن "esp_macros.h"
#ضمن "esp_cpu.h"
#ضمن "esp_private/crosscore_int.h"
#ضمن "esp_memory_utils.h"

#که_تعریف_شوی __cplusplus
خارجي "C" {
#د_که_پای

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come first as they are used further down in this file
 * ------------------------------------------------------------------------------------------------------------------ */

#تعریف_کړه پورټ_CHAR                    uint8_t
#تعریف_کړه پورټ_FLOAT                   عائم
#تعریف_کړه پورټ_DOUBLE                  مزدوج
#تعریف_کړه پورټ_LONG                    int32_t
#تعریف_کړه پورټ_SHORT                   int16_t
#تعریف_کړه پورټ_ډېری_TYPE              uint8_t
#تعریف_کړه پورټ_بنسټ_TYPE               صحيح

تعريفنوع پورټ_ډېری_TYPE              ډېری_Type_t;
تعريفنوع پورټ_بنسټ_TYPE               بنسټ_Type_t;
تعريفنوع بینښه پورټ_بنسټ_TYPE      UBaseType_t;

#إذا ( config_USE_16_بټ_TICKS == 1 )
تعريفنوع uint16_t TickType_t;
#تعریف_کړه پورټ_MAX_DELAY ( TickType_t )  0xffff
#وإلا
تعريفنوع uint32_t TickType_t;
#تعریف_کړه پورټ_MAX_DELAY ( TickType_t )  0xffffffffUL
#د_که_پای

/* Task function macros as described on the FreeRTOS.org WEB site. */
#تعریف_کړه پورټ_TASK_FUNCTION_PROTO( vFunction, pvParameters )  فراغ vFunction( فراغ *pvParameters )
#تعریف_کړه پورټ_TASK_FUNCTION( vFunction, pvParameters )          فراغ vFunction( فراغ *pvParameters )

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#تعریف_کړه پورټ_CRITICAL_NESTING_دننه_TCB     1
#تعریف_کړه پورټ_ډېری_GROWTH                ( -1 )
#تعریف_کړه پورټ_TICK_PERIOD_MS              ( ( TickType_t ) ( 1000 / config_TICK_شرح_HZ ) )
#تعریف_کړه پورټ_BYTE_ALIGNMENT              16
#تعریف_کړه پورټ_NOP() __asm متطاير        (" nop ")

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros needed to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

/* ---------------------- Spinlocks ------------------------
 - Spinlocks added to match API with SMP FreeRTOS. Single core RISC-V does not need spin locks
 * ------------------------------------------------------ */

تعريفنوع spinlock_t                          پورټ_MUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */

#تعریف_کړه پورټ_MUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#تعریف_کړه پورټ_MUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#تعریف_کړه پورټ_MUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#تعریف_کړه پورټ_MUX_TRY_قلف                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#تعریف_کړه پورټ_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// --------------------- Interrupts ------------------------

بنسټ_Type_t x_پورټ_Check_If_دننه_ISR(فراغ);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortCheckIfInISR() internally
 */
فراغ v_پورټ_Assert_If_دننه_ISR(فراغ);

// ------------------ Critical Sections --------------------

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

/**
 * @brief Set interrupt mask and return current interrupt enable register
 *
 * @return UBaseType_t Current interrupt enable register before set
 */
UBaseType_t ul_پورټ_سېټ_مداخله_ماسک(فراغ);

/**
 * @brief Clear current interrupt mask and set given mask
 *
 * @param mask Interrupt mask
 */
فراغ v_پورټ_Clear_مداخله_ماسک(UBaseType_t ماسک);

/**
 * @brief Perform a context switch from an ISR
 */
فراغ v_پورټ_Yield_From_ISR(فراغ);

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

/* ------------------------------------------- FreeRTOS Porting Interface ----------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS
 * - Most come after forward declare as porting macros map to declared functions
 * - Maps to forward declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

#تعریف_کړه پورټ_DISABLE_INTERRUPTS()                    ul_پورټ_سېټ_مداخله_ماسک()
#إذا !SOC_INT_CLIC_SUPPORTED
#تعریف_کړه پورټ_ENABLE_INTERRUPTS()                     v_پورټ_Clear_مداخله_ماسک(RVHAL_INTR_ENABLE_THRESH)
#وإلا
#تعریف_کړه پورټ_ENABLE_INTERRUPTS()                     v_پورټ_Clear_مداخله_ماسک(RVHAL_INTR_ENABLE_THRESH_CLIC)
#د_که_پای /* !SOC_INT_CLIC_SUPPORTED */
#تعریف_کړه پورټ_RESTORE_INTERRUPTS(x)                   v_پورټ_Clear_مداخله_ماسک(x)

#تعریف_کړه پورټ_سېټ_مداخله_ماسک_FROM_ISR() ({           \
    بینښه صحيح cur_level; \
    cur_level = ul_پورټ_سېټ_مداخله_ماسک(); \
    cur_level; \
})
#تعریف_کړه پورټ_CLEAR_مداخله_ماسک_FROM_ISR(x)        پورټ_RESTORE_INTERRUPTS(x)
#تعریف_کړه پورټ_سېټ_مداخله_ماسک()                    پورټ_سېټ_مداخله_ماسک_FROM_ISR()
#تعریف_کړه پورټ_CLEAR_مداخله_ماسک(x)                 پورټ_CLEAR_مداخله_ماسک_FROM_ISR(x)

/**
 * @brief Assert if in ISR context
 *
 * TODO: Enable once ISR safe version of vTaskEnter/ExitCritical() is implemented
 * for single-core SMP FreeRTOS Kernel. (IDF-10540)
 */
// #define portASSERT_IF_IN_ISR() vPortAssertIfInISR()


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
 * - For implementation of non-inlined functions, see port.c, port_common.c, or other assembly files
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Yielding -------------------------

ساكن مضمن فراغ __attribute__((always_inline)) v_پورټ_Yield_Core( بنسټ_Type_t xCoreID )
{
    esp_crosscore_int_send_yield( xCoreID );
}

// ----------------------- System --------------------------

ساكن مضمن بنسټ_Type_t __attribute__((always_inline)) x_پورټ_Get_Core_ID( فراغ )
{
    أرجع (بنسټ_Type_t) esp_cpu_get_core_id();
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

ساكن مضمن بنسټ_Type_t x_پورټ_دننه_Isr_Context(فراغ)
{
    //Just call the FreeRTOS port interface version
    أرجع x_پورټ_Check_If_دننه_ISR();
}

// Added for backward compatibility with IDF
#تعریف_کړه x_پورټ_Interrupted_From_ISRContext()    x_پورټ_دننه_Isr_Context()

// ------------------ Critical Sections --------------------

/*
IDF style critical sections which are orthogonal to FreeRTOS critical sections. However, on single core, the IDF style
critical sections simply disable interrupts, thus we discard the lock and timeout arguments.

Note: The IDF Style critical sections are named differently to their counterparts present in the xtensa port as few IDF
examples such as esp_zigbee_gateway, when compiled for RISC-V targets, have a reference to vPortEnterCritical()
and vPortExitCritical() from precompiled libraries (.a) thereby failing linking.
*/
فراغ v_پورټ_Enter_Critical(فراغ);
فراغ v_پورټ_Exit_Critical(فراغ);

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

//IDF task critical sections
#تعریف_کړه پورټ_TRY_ENTER_CRITICAL(قلف, timeout)       ({(فراغ) قلف; (فراغ) timeout; v_پورټ_Enter_Critical(); pdPASS;})
#تعریف_کړه پورټ_ENTER_CRITICAL_IDF(قلف)                ({(فراغ) قلف; v_پورټ_Enter_Critical();})
#تعریف_کړه پورټ_EXIT_CRITICAL_IDF(قلف)                 ({(فراغ) قلف; v_پورټ_Exit_Critical();})
//IDF ISR critical sections
#تعریف_کړه پورټ_TRY_ENTER_CRITICAL_ISR(قلف, timeout)   ({(فراغ) قلف; (فراغ) timeout; v_پورټ_Enter_Critical(); pdPASS;})
#تعریف_کړه پورټ_ENTER_CRITICAL_ISR(قلف)                ({(فراغ) قلف; v_پورټ_Enter_Critical();})
#تعریف_کړه پورټ_EXIT_CRITICAL_ISR(قلف)                 ({(فراغ) قلف; v_پورټ_Exit_Critical();})
//IDF safe critical sections (they're the same)
#تعریف_کړه پورټ_ENTER_CRITICAL_خوندي(قلف)               ({(فراغ) قلف; v_پورټ_Enter_Critical();})
#تعریف_کړه پورټ_EXIT_CRITICAL_خوندي(قلف)                ({(فراغ) قلف; v_پورټ_Exit_Critical();})

// ---------------------- Yielding -------------------------

ساكن مضمن bool x_پورټ_Can_Yield(فراغ)
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
    uint32_t پوله = rv_utils_get_مداخله_پوله();
    uint32_t intr_level = rv_utils_get_مداخله_level();
    أرجع ((intr_level == 0) && (پوله == 0));
#وإلا/* !SOC_INT_CLIC_SUPPORTED */
    uint32_t پوله = REG_READ(مداخله_CURRENT_CORE_INT_THRESH_REG);
    /* when enter critical code, FreeRTOS will mask threshold to RVHAL_EXCM_LEVEL
     * and exit critical code, will recover threshold value (1). so threshold <= 1
     * means not in critical code
     */
    أرجع (پوله <= 1);
#د_که_پای
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
#ضمن <limits.h>
#ضمن "esp_attr.h"
#ضمن "esp_newlib.h"
#ضمن "esp_heap_caps.h"
#ضمن "esp_rom_sys.h"
#ضمن "esp_system.h"             /* required by esp_get_...() functions in portable.h. [refactor-todo] Update portable.h */


#که_تعریف_شوی __cplusplus
}
#د_که_پای

#د_که_پای // __ASSEMBLER__
