/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ہدایت once

#ضمن "sdkconfig.h"

/* Macros used instead ofsetoff() for better performance of interrupt handler */
#إذا CONFIG_FREERTOS_USE_LIST_انکڑے_INTEGRITY_CHECK_بائٹاں
/*
pxTopOfStack (4) +
xStateListItem (28) +
xEventListItem (28) +
uxPriority (4)
*/
#تعریف_کرو پورٹ_وتھ_PX_ڈھیر 0x40
#وإلا
/*
pxTopOfStack (4) +
xStateListItem (20) +
xEventListItem (20) +
uxPriority (4)
*/
#تعریف_کرو پورٹ_وتھ_PX_ڈھیر 0x30
#جے_انت /* #if CONFIG_FREERTOS_USE_LIST_DATA_INTEGRITY_CHECK_BYTES */

#إذا ( config_نمبر_دا_CORES > 1 )
#تعریف_کرو پورٹ_TCB_CORE_FIELDS_ناپ 8
#وإلا
#تعریف_کرو پورٹ_TCB_CORE_FIELDS_ناپ 0
#جے_انت

#إذا ( configUSE_TASK_PREEMPTION_DISABLE == 1 )
#تعریف_کرو پورٹ_TCB_PREEMPT_DISABLE_میدان_ناپ 4
#وإلا
#تعریف_کرو پورٹ_TCB_PREEMPT_DISABLE_میدان_ناپ 0
#جے_انت

/* Align the value up to the nearest multiple of 4 */
#تعریف_کرو پورٹ_ALIGN_اتے_منزل_4(قدر) (((قدر) + 3) & ~3)

#تعریف_کرو پورٹ_وتھ_PX_سرا_دا_ڈھیر ( \
    پورٹ_وتھ_PX_ڈھیر \
    + 4                                 /* StackType_t * pxStack */ \
    + پورٹ_TCB_CORE_FIELDS_ناپ                     /* BaseType_t xDummy23 + UBaseType_t uxDummy24 */ \
    + پورٹ_ALIGN_اتے_منزل_4(configMAX_TASK_NAME_LEN)   /* pcTaskName[ configMAX_TASK_NAME_LEN ] */ \
    + پورٹ_TCB_PREEMPT_DISABLE_میدان_ناپ           /* BaseType_t xDummy25 */ \
)

#جے_بنا_تعریف __ASSEMBLER__

#ضمن <stdint.h>
#ضمن "spinlock.h"
#ضمن "soc/interrupt_reg.h"
#ضمن "esp_macros.h"
#ضمن "esp_cpu.h"
#ضمن "esp_private/crosscore_int.h"
#ضمن "esp_memory_utils.h"

#جے_تعریف_شدہ __cplusplus
خارجي "C" {
#جے_انت

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come first as they are used further down in this file
 * ------------------------------------------------------------------------------------------------------------------ */

#تعریف_کرو پورٹ_CHAR                    uint8_t
#تعریف_کرو پورٹ_FLOAT                   عائم
#تعریف_کرو پورٹ_DOUBLE                  مزدوج
#تعریف_کرو پورٹ_LONG                    int32_t
#تعریف_کرو پورٹ_SHORT                   int16_t
#تعریف_کرو پورٹ_ڈھیر_TYPE              uint8_t
#تعریف_کرو پورٹ_ادھار_TYPE               صحيح

تعريفنوع پورٹ_ڈھیر_TYPE              ڈھیر_Type_t;
تعريفنوع پورٹ_ادھار_TYPE               ادھار_Type_t;
تعريفنوع بنانشانی پورٹ_ادھار_TYPE      UBaseType_t;

#إذا ( config_USE_16_ثنائی_ہندسہ_TICKS == 1 )
تعريفنوع uint16_t TickType_t;
#تعریف_کرو پورٹ_MAX_DELAY ( TickType_t )  0xffff
#وإلا
تعريفنوع uint32_t TickType_t;
#تعریف_کرو پورٹ_MAX_DELAY ( TickType_t )  0xffffffffUL
#جے_انت

/* Task function macros as described on the FreeRTOS.org WEB site. */
#تعریف_کرو پورٹ_TASK_FUNCTION_PROTO( vFunction, pvParameters )  فراغ vFunction( فراغ *pvParameters )
#تعریف_کرو پورٹ_TASK_FUNCTION( vFunction, pvParameters )          فراغ vFunction( فراغ *pvParameters )

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#تعریف_کرو پورٹ_CRITICAL_NESTING_وچ_TCB     1
#تعریف_کرو پورٹ_ڈھیر_GROWTH                ( -1 )
#تعریف_کرو پورٹ_TICK_PERIOD_MS              ( ( TickType_t ) ( 1000 / config_TICK_شرح_HZ ) )
#تعریف_کرو پورٹ_BYTE_ALIGNMENT              16
#تعریف_کرو پورٹ_NOP() __asm متطاير        (" nop ")

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros needed to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

/* ---------------------- Spinlocks ------------------------
 - Spinlocks added to match API with SMP FreeRTOS. Single core RISC-V does not need spin locks
 * ------------------------------------------------------ */

تعريفنوع spinlock_t                          پورٹ_MUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */

#تعریف_کرو پورٹ_MUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#تعریف_کرو پورٹ_MUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#تعریف_کرو پورٹ_MUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#تعریف_کرو پورٹ_MUX_TRY_جندرا                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#تعریف_کرو پورٹ_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// --------------------- Interrupts ------------------------

ادھار_Type_t x_پورٹ_Check_If_وچ_ISR(فراغ);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortCheckIfInISR() internally
 */
فراغ v_پورٹ_Assert_If_وچ_ISR(فراغ);

// ------------------ Critical Sections --------------------

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

/**
 * @brief Set interrupt mask and return current interrupt enable register
 *
 * @return UBaseType_t Current interrupt enable register before set
 */
UBaseType_t ul_پورٹ_سیٹ_رکاوٹ_نقاب(فراغ);

/**
 * @brief Clear current interrupt mask and set given mask
 *
 * @param mask Interrupt mask
 */
فراغ v_پورٹ_Clear_رکاوٹ_نقاب(UBaseType_t نقاب);

/**
 * @brief Perform a context switch from an ISR
 */
فراغ v_پورٹ_Yield_From_ISR(فراغ);

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

/* ------------------------------------------- FreeRTOS Porting Interface ----------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS
 * - Most come after forward declare as porting macros map to declared functions
 * - Maps to forward declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

#تعریف_کرو پورٹ_DISABLE_INTERRUPTS()                    ul_پورٹ_سیٹ_رکاوٹ_نقاب()
#إذا !SOC_INT_CLIC_SUPPORTED
#تعریف_کرو پورٹ_ENABLE_INTERRUPTS()                     v_پورٹ_Clear_رکاوٹ_نقاب(RVHAL_INTR_ENABLE_THRESH)
#وإلا
#تعریف_کرو پورٹ_ENABLE_INTERRUPTS()                     v_پورٹ_Clear_رکاوٹ_نقاب(RVHAL_INTR_ENABLE_THRESH_CLIC)
#جے_انت /* !SOC_INT_CLIC_SUPPORTED */
#تعریف_کرو پورٹ_RESTORE_INTERRUPTS(x)                   v_پورٹ_Clear_رکاوٹ_نقاب(x)

#تعریف_کرو پورٹ_سیٹ_رکاوٹ_نقاب_FROM_ISR() ({           \
    بنانشانی صحيح cur_level; \
    cur_level = ul_پورٹ_سیٹ_رکاوٹ_نقاب(); \
    cur_level; \
})
#تعریف_کرو پورٹ_CLEAR_رکاوٹ_نقاب_FROM_ISR(x)        پورٹ_RESTORE_INTERRUPTS(x)
#تعریف_کرو پورٹ_سیٹ_رکاوٹ_نقاب()                    پورٹ_سیٹ_رکاوٹ_نقاب_FROM_ISR()
#تعریف_کرو پورٹ_CLEAR_رکاوٹ_نقاب(x)                 پورٹ_CLEAR_رکاوٹ_نقاب_FROM_ISR(x)

/**
 * @brief Assert if in ISR context
 *
 * TODO: Enable once ISR safe version of vTaskEnter/ExitCritical() is implemented
 * for single-core SMP FreeRTOS Kernel. (IDF-10540)
 */
// #define portASSERT_IF_IN_ISR() vPortAssertIfInISR()


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
 * - For implementation of non-inlined functions, see port.c, port_common.c, or other assembly files
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Yielding -------------------------

ساكن مضمن فراغ __attribute__((always_inline)) v_پورٹ_Yield_Core( ادھار_Type_t xCoreID )
{
    esp_crosscore_int_send_yield( xCoreID );
}

// ----------------------- System --------------------------

ساكن مضمن ادھار_Type_t __attribute__((always_inline)) x_پورٹ_Get_Core_ID( فراغ )
{
    أرجع (ادھار_Type_t) esp_cpu_get_core_id();
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

ساكن مضمن ادھار_Type_t x_پورٹ_وچ_Isr_Context(فراغ)
{
    //Just call the FreeRTOS port interface version
    أرجع x_پورٹ_Check_If_وچ_ISR();
}

// Added for backward compatibility with IDF
#تعریف_کرو x_پورٹ_Interrupted_From_ISRContext()    x_پورٹ_وچ_Isr_Context()

// ------------------ Critical Sections --------------------

/*
IDF style critical sections which are orthogonal to FreeRTOS critical sections. However, on single core, the IDF style
critical sections simply disable interrupts, thus we discard the lock and timeout arguments.

Note: The IDF Style critical sections are named differently to their counterparts present in the xtensa port as few IDF
examples such as esp_zigbee_gateway, when compiled for RISC-V targets, have a reference to vPortEnterCritical()
and vPortExitCritical() from precompiled libraries (.a) thereby failing linking.
*/
فراغ v_پورٹ_Enter_Critical(فراغ);
فراغ v_پورٹ_Exit_Critical(فراغ);

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

//IDF task critical sections
#تعریف_کرو پورٹ_TRY_ENTER_CRITICAL(جندرا, timeout)       ({(فراغ) جندرا; (فراغ) timeout; v_پورٹ_Enter_Critical(); pdPASS;})
#تعریف_کرو پورٹ_ENTER_CRITICAL_IDF(جندرا)                ({(فراغ) جندرا; v_پورٹ_Enter_Critical();})
#تعریف_کرو پورٹ_EXIT_CRITICAL_IDF(جندرا)                 ({(فراغ) جندرا; v_پورٹ_Exit_Critical();})
//IDF ISR critical sections
#تعریف_کرو پورٹ_TRY_ENTER_CRITICAL_ISR(جندرا, timeout)   ({(فراغ) جندرا; (فراغ) timeout; v_پورٹ_Enter_Critical(); pdPASS;})
#تعریف_کرو پورٹ_ENTER_CRITICAL_ISR(جندرا)                ({(فراغ) جندرا; v_پورٹ_Enter_Critical();})
#تعریف_کرو پورٹ_EXIT_CRITICAL_ISR(جندرا)                 ({(فراغ) جندرا; v_پورٹ_Exit_Critical();})
//IDF safe critical sections (they're the same)
#تعریف_کرو پورٹ_ENTER_CRITICAL_محفوظ(جندرا)               ({(فراغ) جندرا; v_پورٹ_Enter_Critical();})
#تعریف_کرو پورٹ_EXIT_CRITICAL_محفوظ(جندرا)                ({(فراغ) جندرا; v_پورٹ_Exit_Critical();})

// ---------------------- Yielding -------------------------

ساكن مضمن bool x_پورٹ_Can_Yield(فراغ)
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
    uint32_t حد = rv_utils_get_رکاوٹ_حد();
    uint32_t intr_level = rv_utils_get_رکاوٹ_level();
    أرجع ((intr_level == 0) && (حد == 0));
#وإلا/* !SOC_INT_CLIC_SUPPORTED */
    uint32_t حد = REG_READ(رکاوٹ_CURRENT_CORE_INT_THRESH_REG);
    /* when enter critical code, FreeRTOS will mask threshold to RVHAL_EXCM_LEVEL
     * and exit critical code, will recover threshold value (1). so threshold <= 1
     * means not in critical code
     */
    أرجع (حد <= 1);
#جے_انت
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
#ضمن <limits.h>
#ضمن "esp_attr.h"
#ضمن "esp_newlib.h"
#ضمن "esp_heap_caps.h"
#ضمن "esp_rom_sys.h"
#ضمن "esp_system.h"             /* required by esp_get_...() functions in portable.h. [refactor-todo] Update portable.h */


#جے_تعریف_شدہ __cplusplus
}
#جے_انت

#جے_انت // __ASSEMBLER__
