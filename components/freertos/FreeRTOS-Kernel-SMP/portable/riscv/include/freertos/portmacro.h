/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#هدايت once

#ضمن "sdkconfig.h"

/* Macros used instead ofsetoff() for better performance of interrupt handler */
#إذا CONFIG_FREERTOS_USE_LIST_معلومات_INTEGRITY_CHECK_بائيٽون
/*
pxTopOfStack (4) +
xStateListItem (28) +
xEventListItem (28) +
uxPriority (4)
*/
#وصف_ڪريو پورٽ_وٿي_PX_ڍير 0x40
#وإلا
/*
pxTopOfStack (4) +
xStateListItem (20) +
xEventListItem (20) +
uxPriority (4)
*/
#وصف_ڪريو پورٽ_وٿي_PX_ڍير 0x30
#جيڪڏهن_پورو /* #if CONFIG_FREERTOS_USE_LIST_DATA_INTEGRITY_CHECK_BYTES */

#إذا ( config_عدد_جو_CORES > 1 )
#وصف_ڪريو پورٽ_TCB_CORE_FIELDS_سائيز 8
#وإلا
#وصف_ڪريو پورٽ_TCB_CORE_FIELDS_سائيز 0
#جيڪڏهن_پورو

#إذا ( configUSE_TASK_PREEMPTION_DISABLE == 1 )
#وصف_ڪريو پورٽ_TCB_PREEMPT_DISABLE_ميدان_سائيز 4
#وإلا
#وصف_ڪريو پورٽ_TCB_PREEMPT_DISABLE_ميدان_سائيز 0
#جيڪڏهن_پورو

/* Align the value up to the nearest multiple of 4 */
#وصف_ڪريو پورٽ_ALIGN_مٿي_منزل_4(قدر) (((قدر) + 3) & ~3)

#وصف_ڪريو پورٽ_وٿي_PX_پڇاڙي_جو_ڍير ( \
    پورٽ_وٿي_PX_ڍير \
    + 4                                 /* StackType_t * pxStack */ \
    + پورٽ_TCB_CORE_FIELDS_سائيز                     /* BaseType_t xDummy23 + UBaseType_t uxDummy24 */ \
    + پورٽ_ALIGN_مٿي_منزل_4(configMAX_TASK_NAME_LEN)   /* pcTaskName[ configMAX_TASK_NAME_LEN ] */ \
    + پورٽ_TCB_PREEMPT_DISABLE_ميدان_سائيز           /* BaseType_t xDummy25 */ \
)

#جيڪڏهن_وصف_نه_ٿيل __ASSEMBLER__

#ضمن <stdint.h>
#ضمن "spinlock.h"
#ضمن "soc/interrupt_reg.h"
#ضمن "esp_macros.h"
#ضمن "esp_cpu.h"
#ضمن "esp_private/crosscore_int.h"
#ضمن "esp_memory_utils.h"

#جيڪڏهن_وصف_ٿيل __cplusplus
خارجي "C" {
#جيڪڏهن_پورو

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come first as they are used further down in this file
 * ------------------------------------------------------------------------------------------------------------------ */

#وصف_ڪريو پورٽ_CHAR                    uint8_t
#وصف_ڪريو پورٽ_FLOAT                   عائم
#وصف_ڪريو پورٽ_DOUBLE                  مزدوج
#وصف_ڪريو پورٽ_LONG                    int32_t
#وصف_ڪريو پورٽ_SHORT                   int16_t
#وصف_ڪريو پورٽ_ڍير_TYPE              uint8_t
#وصف_ڪريو پورٽ_بنياد_TYPE               صحيح

تعريفنوع پورٽ_ڍير_TYPE              ڍير_Type_t;
تعريفنوع پورٽ_بنياد_TYPE               بنياد_Type_t;
تعريفنوع بنانشان پورٽ_بنياد_TYPE      UBaseType_t;

#إذا ( config_USE_16_ٻٽو_انگ_TICKS == 1 )
تعريفنوع uint16_t TickType_t;
#وصف_ڪريو پورٽ_MAX_DELAY ( TickType_t )  0xffff
#وإلا
تعريفنوع uint32_t TickType_t;
#وصف_ڪريو پورٽ_MAX_DELAY ( TickType_t )  0xffffffffUL
#جيڪڏهن_پورو

/* Task function macros as described on the FreeRTOS.org WEB site. */
#وصف_ڪريو پورٽ_TASK_FUNCTION_PROTO( vFunction, pvParameters )  فراغ vFunction( فراغ *pvParameters )
#وصف_ڪريو پورٽ_TASK_FUNCTION( vFunction, pvParameters )          فراغ vFunction( فراغ *pvParameters )

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#وصف_ڪريو پورٽ_CRITICAL_NESTING_اندريون_TCB     1
#وصف_ڪريو پورٽ_ڍير_GROWTH                ( -1 )
#وصف_ڪريو پورٽ_TICK_PERIOD_MS              ( ( TickType_t ) ( 1000 / config_TICK_شرح_HZ ) )
#وصف_ڪريو پورٽ_BYTE_ALIGNMENT              16
#وصف_ڪريو پورٽ_NOP() __asm متطاير        (" nop ")

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros needed to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

/* ---------------------- Spinlocks ------------------------
 - Spinlocks added to match API with SMP FreeRTOS. Single core RISC-V does not need spin locks
 * ------------------------------------------------------ */

تعريفنوع spinlock_t                          پورٽ_MUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */

#وصف_ڪريو پورٽ_MUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#وصف_ڪريو پورٽ_MUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#وصف_ڪريو پورٽ_MUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#وصف_ڪريو پورٽ_MUX_TRY_تالو                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#وصف_ڪريو پورٽ_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// --------------------- Interrupts ------------------------

بنياد_Type_t x_پورٽ_Check_If_اندريون_ISR(فراغ);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortCheckIfInISR() internally
 */
فراغ v_پورٽ_Assert_If_اندريون_ISR(فراغ);

// ------------------ Critical Sections --------------------

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

/**
 * @brief Set interrupt mask and return current interrupt enable register
 *
 * @return UBaseType_t Current interrupt enable register before set
 */
UBaseType_t ul_پورٽ_سيٽ_مداخلت_نقاب(فراغ);

/**
 * @brief Clear current interrupt mask and set given mask
 *
 * @param mask Interrupt mask
 */
فراغ v_پورٽ_Clear_مداخلت_نقاب(UBaseType_t نقاب);

/**
 * @brief Perform a context switch from an ISR
 */
فراغ v_پورٽ_Yield_From_ISR(فراغ);

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

/* ------------------------------------------- FreeRTOS Porting Interface ----------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS
 * - Most come after forward declare as porting macros map to declared functions
 * - Maps to forward declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

#وصف_ڪريو پورٽ_DISABLE_INTERRUPTS()                    ul_پورٽ_سيٽ_مداخلت_نقاب()
#إذا !SOC_INT_CLIC_SUPPORTED
#وصف_ڪريو پورٽ_ENABLE_INTERRUPTS()                     v_پورٽ_Clear_مداخلت_نقاب(RVHAL_INTR_ENABLE_THRESH)
#وإلا
#وصف_ڪريو پورٽ_ENABLE_INTERRUPTS()                     v_پورٽ_Clear_مداخلت_نقاب(RVHAL_INTR_ENABLE_THRESH_CLIC)
#جيڪڏهن_پورو /* !SOC_INT_CLIC_SUPPORTED */
#وصف_ڪريو پورٽ_RESTORE_INTERRUPTS(x)                   v_پورٽ_Clear_مداخلت_نقاب(x)

#وصف_ڪريو پورٽ_سيٽ_مداخلت_نقاب_FROM_ISR() ({           \
    بنانشان صحيح cur_level; \
    cur_level = ul_پورٽ_سيٽ_مداخلت_نقاب(); \
    cur_level; \
})
#وصف_ڪريو پورٽ_CLEAR_مداخلت_نقاب_FROM_ISR(x)        پورٽ_RESTORE_INTERRUPTS(x)
#وصف_ڪريو پورٽ_سيٽ_مداخلت_نقاب()                    پورٽ_سيٽ_مداخلت_نقاب_FROM_ISR()
#وصف_ڪريو پورٽ_CLEAR_مداخلت_نقاب(x)                 پورٽ_CLEAR_مداخلت_نقاب_FROM_ISR(x)

/**
 * @brief Assert if in ISR context
 *
 * TODO: Enable once ISR safe version of vTaskEnter/ExitCritical() is implemented
 * for single-core SMP FreeRTOS Kernel. (IDF-10540)
 */
// #define portASSERT_IF_IN_ISR() vPortAssertIfInISR()


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
 * - For implementation of non-inlined functions, see port.c, port_common.c, or other assembly files
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Yielding -------------------------

ساكن مضمن فراغ __attribute__((always_inline)) v_پورٽ_Yield_Core( بنياد_Type_t xCoreID )
{
    esp_crosscore_int_send_yield( xCoreID );
}

// ----------------------- System --------------------------

ساكن مضمن بنياد_Type_t __attribute__((always_inline)) x_پورٽ_Get_Core_ID( فراغ )
{
    أرجع (بنياد_Type_t) esp_cpu_get_core_id();
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

ساكن مضمن بنياد_Type_t x_پورٽ_اندريون_Isr_Context(فراغ)
{
    //Just call the FreeRTOS port interface version
    أرجع x_پورٽ_Check_If_اندريون_ISR();
}

// Added for backward compatibility with IDF
#وصف_ڪريو x_پورٽ_Interrupted_From_ISRContext()    x_پورٽ_اندريون_Isr_Context()

// ------------------ Critical Sections --------------------

/*
IDF style critical sections which are orthogonal to FreeRTOS critical sections. However, on single core, the IDF style
critical sections simply disable interrupts, thus we discard the lock and timeout arguments.

Note: The IDF Style critical sections are named differently to their counterparts present in the xtensa port as few IDF
examples such as esp_zigbee_gateway, when compiled for RISC-V targets, have a reference to vPortEnterCritical()
and vPortExitCritical() from precompiled libraries (.a) thereby failing linking.
*/
فراغ v_پورٽ_Enter_Critical(فراغ);
فراغ v_پورٽ_Exit_Critical(فراغ);

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

//IDF task critical sections
#وصف_ڪريو پورٽ_TRY_ENTER_CRITICAL(تالو, timeout)       ({(فراغ) تالو; (فراغ) timeout; v_پورٽ_Enter_Critical(); pdPASS;})
#وصف_ڪريو پورٽ_ENTER_CRITICAL_IDF(تالو)                ({(فراغ) تالو; v_پورٽ_Enter_Critical();})
#وصف_ڪريو پورٽ_EXIT_CRITICAL_IDF(تالو)                 ({(فراغ) تالو; v_پورٽ_Exit_Critical();})
//IDF ISR critical sections
#وصف_ڪريو پورٽ_TRY_ENTER_CRITICAL_ISR(تالو, timeout)   ({(فراغ) تالو; (فراغ) timeout; v_پورٽ_Enter_Critical(); pdPASS;})
#وصف_ڪريو پورٽ_ENTER_CRITICAL_ISR(تالو)                ({(فراغ) تالو; v_پورٽ_Enter_Critical();})
#وصف_ڪريو پورٽ_EXIT_CRITICAL_ISR(تالو)                 ({(فراغ) تالو; v_پورٽ_Exit_Critical();})
//IDF safe critical sections (they're the same)
#وصف_ڪريو پورٽ_ENTER_CRITICAL_محفوظ(تالو)               ({(فراغ) تالو; v_پورٽ_Enter_Critical();})
#وصف_ڪريو پورٽ_EXIT_CRITICAL_محفوظ(تالو)                ({(فراغ) تالو; v_پورٽ_Exit_Critical();})

// ---------------------- Yielding -------------------------

ساكن مضمن bool x_پورٽ_Can_Yield(فراغ)
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
    uint32_t حد = rv_utils_get_مداخلت_حد();
    uint32_t intr_level = rv_utils_get_مداخلت_level();
    أرجع ((intr_level == 0) && (حد == 0));
#وإلا/* !SOC_INT_CLIC_SUPPORTED */
    uint32_t حد = REG_READ(مداخلت_CURRENT_CORE_INT_THRESH_REG);
    /* when enter critical code, FreeRTOS will mask threshold to RVHAL_EXCM_LEVEL
     * and exit critical code, will recover threshold value (1). so threshold <= 1
     * means not in critical code
     */
    أرجع (حد <= 1);
#جيڪڏهن_پورو
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
#ضمن <limits.h>
#ضمن "esp_attr.h"
#ضمن "esp_newlib.h"
#ضمن "esp_heap_caps.h"
#ضمن "esp_rom_sys.h"
#ضمن "esp_system.h"             /* required by esp_get_...() functions in portable.h. [refactor-todo] Update portable.h */


#جيڪڏهن_وصف_ٿيل __cplusplus
}
#جيڪڏهن_پورو

#جيڪڏهن_پورو // __ASSEMBLER__
