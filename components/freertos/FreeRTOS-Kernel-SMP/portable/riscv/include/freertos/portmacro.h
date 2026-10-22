/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#निर्देशः once

#अन्तर्भाव "sdkconfig.h"

/* Macros used instead ofsetoff() for better performance of interrupt handler */
#यदि CONFIG_FREERTOS_USE_LIST_दत्त_INTEGRITY_CHECK_अष्टकानि
/*
pxTopOfStack (4) +
xStateListItem (28) +
xEventListItem (28) +
uxPriority (4)
*/
#परिभाषय पत्तनम्_विस्थापनम्_PX_राशिः 0x40
#अन्यथा
/*
pxTopOfStack (4) +
xStateListItem (20) +
xEventListItem (20) +
uxPriority (4)
*/
#परिभाषय पत्तनम्_विस्थापनम्_PX_राशिः 0x30
#यदि_अन्तः /* #if CONFIG_FREERTOS_USE_LIST_DATA_INTEGRITY_CHECK_BYTES */

#यदि ( config_सङ्ख्या_सम्बन्धि_CORES > 1 )
#परिभाषय पत्तनम्_TCB_CORE_FIELDS_आकार 8
#अन्यथा
#परिभाषय पत्तनम्_TCB_CORE_FIELDS_आकार 0
#यदि_अन्तः

#यदि ( configUSE_TASK_PREEMPTION_DISABLE == 1 )
#परिभाषय पत्तनम्_TCB_PREEMPT_DISABLE_क्षेत्रम्_आकार 4
#अन्यथा
#परिभाषय पत्तनम्_TCB_PREEMPT_DISABLE_क्षेत्रम्_आकार 0
#यदि_अन्तः

/* Align the value up to the nearest multiple of 4 */
#परिभाषय पत्तनम्_ALIGN_ऊर्ध्वम्_गन्तव्यम्_4(मानम्) (((मानम्) + 3) & ~3)

#परिभाषय पत्तनम्_विस्थापनम्_PX_अन्तः_सम्बन्धि_राशिः ( \
    पत्तनम्_विस्थापनम्_PX_राशिः \
    + 4                                 /* StackType_t * pxStack */ \
    + पत्तनम्_TCB_CORE_FIELDS_आकार                     /* BaseType_t xDummy23 + UBaseType_t uxDummy24 */ \
    + पत्तनम्_ALIGN_ऊर्ध्वम्_गन्तव्यम्_4(configMAX_TASK_NAME_LEN)   /* pcTaskName[ configMAX_TASK_NAME_LEN ] */ \
    + पत्तनम्_TCB_PREEMPT_DISABLE_क्षेत्रम्_आकार           /* BaseType_t xDummy25 */ \
)

#यदि_अपरिभाषितम् __ASSEMBLER__

#अन्तर्भाव <stdint.h>
#अन्तर्भाव "spinlock.h"
#अन्तर्भाव "soc/interrupt_reg.h"
#अन्तर्भाव "esp_macros.h"
#अन्तर्भाव "esp_cpu.h"
#अन्तर्भाव "esp_private/crosscore_int.h"
#अन्तर्भाव "esp_memory_utils.h"

#यदि_परिभाषितम् __cplusplus
बाह्य "C" {
#यदि_अन्तः

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come first as they are used further down in this file
 * ------------------------------------------------------------------------------------------------------------------ */

#परिभाषय पत्तनम्_CHAR                    uint8_t
#परिभाषय पत्तनम्_FLOAT                   भिन्न
#परिभाषय पत्तनम्_DOUBLE                  द्विगुण
#परिभाषय पत्तनम्_LONG                    int32_t
#परिभाषय पत्तनम्_SHORT                   int16_t
#परिभाषय पत्तनम्_राशिः_TYPE              uint8_t
#परिभाषय पत्तनम्_आधारः_TYPE               पूर्णाङ्क

प्रकारनिर्देश पत्तनम्_राशिः_TYPE              राशिः_Type_t;
प्रकारनिर्देश पत्तनम्_आधारः_TYPE               आधारः_Type_t;
प्रकारनिर्देश अचिह्नित पत्तनम्_आधारः_TYPE      UBaseType_t;

#यदि ( configUSE_16_BIT_TICKS == 1 )
प्रकारनिर्देश uint16_t TickType_t;
#परिभाषय पत्तनम्_MAX_DELAY ( TickType_t )  0xffff
#अन्यथा
प्रकारनिर्देश uint32_t TickType_t;
#परिभाषय पत्तनम्_MAX_DELAY ( TickType_t )  0xffffffffUL
#यदि_अन्तः

/* Task function macros as described on the FreeRTOS.org WEB site. */
#परिभाषय पत्तनम्_TASK_FUNCTION_PROTO( vFunction, pvParameters )  शून्य vFunction( शून्य *pvParameters )
#परिभाषय पत्तनम्_TASK_FUNCTION( vFunction, pvParameters )          शून्य vFunction( शून्य *pvParameters )

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#परिभाषय पत्तनम्_CRITICAL_NESTING_अन्तर्_TCB     1
#परिभाषय पत्तनम्_राशिः_GROWTH                ( -1 )
#परिभाषय पत्तनम्_TICK_PERIOD_MS              ( ( TickType_t ) ( 1000 / config_TICK_दरः_HZ ) )
#परिभाषय पत्तनम्_BYTE_ALIGNMENT              16
#परिभाषय पत्तनम्_NOP() __asm चञ्चल        (" nop ")

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros needed to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

/* ---------------------- Spinlocks ------------------------
 - Spinlocks added to match API with SMP FreeRTOS. Single core RISC-V does not need spin locks
 * ------------------------------------------------------ */

प्रकारनिर्देश spinlock_t                          पत्तनम्_MUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */

#परिभाषय पत्तनम्_MUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#परिभाषय पत्तनम्_MUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#परिभाषय पत्तनम्_MUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#परिभाषय पत्तनम्_MUX_TRY_तालकम्                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#परिभाषय पत्तनम्_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// --------------------- Interrupts ------------------------

आधारः_Type_t x_पत्तनम्_Check_If_अन्तर्_ISR(शून्य);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortCheckIfInISR() internally
 */
शून्य v_पत्तनम्_Assert_If_अन्तर्_ISR(शून्य);

// ------------------ Critical Sections --------------------

#यदि ( config_सङ्ख्या_सम्बन्धि_CORES > 1 )
/*
These are always called with interrupts already disabled. We simply need to get/release the spinlocks
*/
बाह्य पत्तनम्_MUX_TYPE पत्तनम्_x_Task_तालकम्;
बाह्य पत्तनम्_MUX_TYPE पत्तनम्_x_ISRLock;

शून्य v_पत्तनम्_Take_तालकम्( पत्तनम्_MUX_TYPE *तालकम् );
शून्य v_पत्तनम्_Release_तालकम्( पत्तनम्_MUX_TYPE *तालकम् );
#यदि_अन्तः /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

शून्य v_पत्तनम्_Yield( शून्य );
स्थिर अन्तःस्थ शून्य __attribute__((always_inline)) v_पत्तनम्_Yield_Core( आधारः_Type_t xCoreID );

/**
 * @brief Set interrupt mask and return current interrupt enable register
 *
 * @return UBaseType_t Current interrupt enable register before set
 */
UBaseType_t ul_पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्(शून्य);

/**
 * @brief Clear current interrupt mask and set given mask
 *
 * @param mask Interrupt mask
 */
शून्य v_पत्तनम्_Clear_व्यवधानम्_आवरणम्(UBaseType_t आवरणम्);

/**
 * @brief Perform a context switch from an ISR
 */
शून्य v_पत्तनम्_Yield_From_ISR(शून्य);

#परिभाषय पत्तनम्_YIELD_FROM_ISR_CHECK(x)     ({ \
    यदि ( (x) == pdTRUE ) { \
        v_पत्तनम्_Yield_From_ISR(); \
    } \
})
#परिभाषय पत्तनम्_YIELD_FROM_ISR_NO_CHECK()               v_पत्तनम्_Yield_From_ISR()

// ----------------------- System --------------------------

स्थिर अन्तःस्थ आधारः_Type_t __attribute__((always_inline)) x_पत्तनम्_Get_Core_ID( शून्य );

// --------------------- TCB Cleanup -----------------------

/**
 * @brief TCB cleanup hook
 *
 * The portCLEAN_UP_TCB() macro is called in prvDeleteTCB() right before a
 * deleted task's memory is freed. We map that macro to this internal function
 * so that IDF FreeRTOS ports can inject some task pre-deletion operations.
 */
शून्य v_पत्तनम्_TCBPre_Delete_Hook( शून्य *pxTCB );

/* ------------------------------------------- FreeRTOS Porting Interface ----------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS
 * - Most come after forward declare as porting macros map to declared functions
 * - Maps to forward declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

#परिभाषय पत्तनम्_DISABLE_INTERRUPTS()                    ul_पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्()
#यदि !SOC_INT_CLIC_SUPPORTED
#परिभाषय पत्तनम्_ENABLE_INTERRUPTS()                     v_पत्तनम्_Clear_व्यवधानम्_आवरणम्(RVHAL_INTR_ENABLE_THRESH)
#अन्यथा
#परिभाषय पत्तनम्_ENABLE_INTERRUPTS()                     v_पत्तनम्_Clear_व्यवधानम्_आवरणम्(RVHAL_INTR_ENABLE_THRESH_CLIC)
#यदि_अन्तः /* !SOC_INT_CLIC_SUPPORTED */
#परिभाषय पत्तनम्_RESTORE_INTERRUPTS(x)                   v_पत्तनम्_Clear_व्यवधानम्_आवरणम्(x)

#परिभाषय पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्_FROM_ISR() ({           \
    अचिह्नित पूर्णाङ्क cur_level; \
    cur_level = ul_पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्(); \
    cur_level; \
})
#परिभाषय पत्तनम्_CLEAR_व्यवधानम्_आवरणम्_FROM_ISR(x)        पत्तनम्_RESTORE_INTERRUPTS(x)
#परिभाषय पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्()                    पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्_FROM_ISR()
#परिभाषय पत्तनम्_CLEAR_व्यवधानम्_आवरणम्(x)                 पत्तनम्_CLEAR_व्यवधानम्_आवरणम्_FROM_ISR(x)

/**
 * @brief Assert if in ISR context
 *
 * TODO: Enable once ISR safe version of vTaskEnter/ExitCritical() is implemented
 * for single-core SMP FreeRTOS Kernel. (IDF-10540)
 */
// #define portASSERT_IF_IN_ISR() vPortAssertIfInISR()


// ------------------ Critical Sections --------------------

#यदि ( config_सङ्ख्या_सम्बन्धि_CORES > 1 )
#परिभाषय पत्तनम्_GET_TASK_तालकम्()                         v_पत्तनम्_Take_तालकम्(&पत्तनम्_x_Task_तालकम्)
#परिभाषय पत्तनम्_RELEASE_TASK_तालकम्()                     v_पत्तनम्_Release_तालकम्(&पत्तनम्_x_Task_तालकम्)
#परिभाषय पत्तनम्_GET_ISR_तालकम्()                          v_पत्तनम्_Take_तालकम्(&पत्तनम्_x_ISRLock)
#परिभाषय पत्तनम्_RELEASE_ISR_तालकम्()                      v_पत्तनम्_Release_तालकम्(&पत्तनम्_x_ISRLock)
#यदि_अन्तः /* configNUMBER_OF_CORES > 1 */

//Critical sections used by FreeRTOS SMP
बाह्य शून्य vTaskEnterCritical( शून्य );
बाह्य शून्य vTaskExitCritical( शून्य );
#परिभाषय पत्तनम्_ENTER_CRITICAL_SMP()                    vTaskEnterCritical();
#परिभाषय पत्तनम्_EXIT_CRITICAL_SMP()                     vTaskExitCritical();

#यदि defined(__cplusplus) && (__cplusplus >  201703L)
#परिभाषय पत्तनम्_ENTER_CRITICAL(...)                     वृणु_MACRO_VA_ARG(पत्तनम्_ENTER_CRITICAL_IDF, पत्तनम्_ENTER_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#परिभाषय पत्तनम्_EXIT_CRITICAL(...)                      वृणु_MACRO_VA_ARG(पत्तनम्_EXIT_CRITICAL_IDF, पत्तनम्_EXIT_CRITICAL_SMP __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#अन्यथा
#परिभाषय पत्तनम्_ENTER_CRITICAL(...)                     वृणु_MACRO_VA_ARG(पत्तनम्_ENTER_CRITICAL_IDF, पत्तनम्_ENTER_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#परिभाषय पत्तनम्_EXIT_CRITICAL(...)                      वृणु_MACRO_VA_ARG(पत्तनम्_EXIT_CRITICAL_IDF, पत्तनम्_EXIT_CRITICAL_SMP, ##__VA_ARGS__)(__VA_ARGS__)
#यदि_अन्तः

#परिभाषय पत्तनम्_ENTER_CRITICAL_FROM_ISR() vTaskEnterCriticalFromISR()
#परिभाषय पत्तनम्_EXIT_CRITICAL_FROM_ISR(x) vTaskExitCriticalFromISR(x)

// ---------------------- Yielding -------------------------

#परिभाषय पत्तनम्_YIELD()                                 v_पत्तनम्_Yield()
#यदि defined(__cplusplus) && (__cplusplus >  201703L)
#परिभाषय पत्तनम्_YIELD_FROM_ISR(...)                     वृणु_MACRO_VA_ARG(पत्तनम्_YIELD_FROM_ISR_CHECK, पत्तनम्_YIELD_FROM_ISR_NO_CHECK __VA_OPT__(,) __VA_ARGS__)(__VA_ARGS__)
#अन्यथा
#परिभाषय पत्तनम्_YIELD_FROM_ISR(...)                     वृणु_MACRO_VA_ARG(पत्तनम्_YIELD_FROM_ISR_CHECK, पत्तनम्_YIELD_FROM_ISR_NO_CHECK, ##__VA_ARGS__)(__VA_ARGS__)
#यदि_अन्तः
#परिभाषय पत्तनम्_YIELD_CORE(x)                           v_पत्तनम्_Yield_Core(x)

// ----------------------- System --------------------------

#परिभाषय पत्तनम्_GET_CORE_ID()                           x_पत्तनम्_Get_Core_ID()
#परिभाषय पत्तनम्_CHECK_IF_अन्तर्_ISR()                       x_पत्तनम्_Check_If_अन्तर्_ISR()

// ------------------- Run Time Stats ----------------------

#परिभाषय पत्तनम्_CONFIGURE_TIMER_FOR_सततम्_TIME_STATS()
#यदि ( CONFIG_FREERTOS_GENERATE_सततम्_TIME_STATS )
config_सततम्_TIME_गणक_TYPE x_पत्तनम्_Get_सततम्_Time_गणक_मानम्( शून्य );
#परिभाषय पत्तनम्_GET_सततम्_TIME_गणक_मानम्()        x_पत्तनम्_Get_सततम्_Time_गणक_मानम्()
#यदि_अन्तः

// --------------------- TCB Cleanup -----------------------

#परिभाषय पत्तनम्_CLEAN_ऊर्ध्वम्_TCB( pxTCB )                   v_पत्तनम्_TCBPre_Delete_Hook( pxTCB )


/* --------------------------------------------- Inline Implementations ------------------------------------------------
 * - Implementation of inline functions of the forward declares
 * - Should come after forward declare and FreeRTOS Porting interface, as implementation may use both.
 * - For implementation of non-inlined functions, see port.c, port_common.c, or other assembly files
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Yielding -------------------------

स्थिर अन्तःस्थ शून्य __attribute__((always_inline)) v_पत्तनम्_Yield_Core( आधारः_Type_t xCoreID )
{
    esp_crosscore_int_send_yield( xCoreID );
}

// ----------------------- System --------------------------

स्थिर अन्तःस्थ आधारः_Type_t __attribute__((always_inline)) x_पत्तनम्_Get_Core_ID( शून्य )
{
    प्रत्यागम (आधारः_Type_t) esp_cpu_get_core_id();
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

स्थिर अन्तःस्थ आधारः_Type_t x_पत्तनम्_अन्तर्_Isr_Context(शून्य)
{
    //Just call the FreeRTOS port interface version
    प्रत्यागम x_पत्तनम्_Check_If_अन्तर्_ISR();
}

// Added for backward compatibility with IDF
#परिभाषय x_पत्तनम्_Interrupted_From_ISRContext()    x_पत्तनम्_अन्तर्_Isr_Context()

// ------------------ Critical Sections --------------------

/*
IDF style critical sections which are orthogonal to FreeRTOS critical sections. However, on single core, the IDF style
critical sections simply disable interrupts, thus we discard the lock and timeout arguments.

Note: The IDF Style critical sections are named differently to their counterparts present in the xtensa port as few IDF
examples such as esp_zigbee_gateway, when compiled for RISC-V targets, have a reference to vPortEnterCritical()
and vPortExitCritical() from precompiled libraries (.a) thereby failing linking.
*/
शून्य v_पत्तनम्_Enter_Critical(शून्य);
शून्य v_पत्तनम्_Exit_Critical(शून्य);

#यदि CONFIG_FREERTOS_पत्तनम्_THREAD_SAFE_CLAIM
/**
 * @brief Claim thread-safe region start
 *        If claimed, vPortEnterCritical/vPortExitCritical on the current core are no-ops.
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
शून्य x_पत्तनम्_Thread_Safe_Claim(शून्य);

/**
 * @brief Claim thread-safe region end
 *        Restores normal port critical behavior
 *        Only can be used in single-core running context with interrupts disabled.
 * @note !!! Caller must guarantee thread safety between Claim and Disclaim !!!
 */
शून्य x_पत्तनम्_Thread_Safe_Disclaim(शून्य);

बाह्य चञ्चल bool पत्तनम्_x_Thread_Safe_Claimed;
#यदि_अन्तः /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

//IDF task critical sections
#परिभाषय पत्तनम्_TRY_ENTER_CRITICAL(तालकम्, timeout)       ({(शून्य) तालकम्; (शून्य) timeout; v_पत्तनम्_Enter_Critical(); pdPASS;})
#परिभाषय पत्तनम्_ENTER_CRITICAL_IDF(तालकम्)                ({(शून्य) तालकम्; v_पत्तनम्_Enter_Critical();})
#परिभाषय पत्तनम्_EXIT_CRITICAL_IDF(तालकम्)                 ({(शून्य) तालकम्; v_पत्तनम्_Exit_Critical();})
//IDF ISR critical sections
#परिभाषय पत्तनम्_TRY_ENTER_CRITICAL_ISR(तालकम्, timeout)   ({(शून्य) तालकम्; (शून्य) timeout; v_पत्तनम्_Enter_Critical(); pdPASS;})
#परिभाषय पत्तनम्_ENTER_CRITICAL_ISR(तालकम्)                ({(शून्य) तालकम्; v_पत्तनम्_Enter_Critical();})
#परिभाषय पत्तनम्_EXIT_CRITICAL_ISR(तालकम्)                 ({(शून्य) तालकम्; v_पत्तनम्_Exit_Critical();})
//IDF safe critical sections (they're the same)
#परिभाषय पत्तनम्_ENTER_CRITICAL_SAFE(तालकम्)               ({(शून्य) तालकम्; v_पत्तनम्_Enter_Critical();})
#परिभाषय पत्तनम्_EXIT_CRITICAL_SAFE(तालकम्)                ({(शून्य) तालकम्; v_पत्तनम्_Exit_Critical();})

// ---------------------- Yielding -------------------------

स्थिर अन्तःस्थ bool x_पत्तनम्_Can_Yield(शून्य)
{
#यदि SOC_INT_CLIC_SUPPORTED
    /* When CLIC is supported:
     *  - The lowest interrupt threshold level is 0. Therefore, an interrupt threshold level above 0 would mean that we
     *    are in a critical section.
     *  - Since CLIC enables HW interrupt nesting, we do not have the updated interrupt level in the
     *    INTERRUPT_CURRENT_CORE_INT_THRESH_REG register when nested interrupts occur. To know the current interrupt
     *    level, we read the machine-mode interrupt level (mil) field from the mintstatus CSR. A non-zero value indicates
     *    that we are in an interrupt context.
     */
    uint32_t देहली = rv_utils_get_व्यवधानम्_देहली();
    uint32_t intr_level = rv_utils_get_व्यवधानम्_level();
    प्रत्यागम ((intr_level == 0) && (देहली == 0));
#अन्यथा/* !SOC_INT_CLIC_SUPPORTED */
    uint32_t देहली = REG_READ(व्यवधानम्_CURRENT_CORE_INT_THRESH_REG);
    /* when enter critical code, FreeRTOS will mask threshold to RVHAL_EXCM_LEVEL
     * and exit critical code, will recover threshold value (1). so threshold <= 1
     * means not in critical code
     */
    प्रत्यागम (देहली <= 1);
#यदि_अन्तः
}

// Defined even for configNUMBER_OF_CORES > 1 for IDF compatibility
#परिभाषय पत्तनम्_YIELD_WITHIN_API()                      esp_crosscore_int_send_yield(x_पत्तनम्_Get_Core_ID())

// ----------------------- System --------------------------

शून्य v_पत्तनम्_समुच्चयः_राशिः_Watchpoint(शून्य *px_राशिः_Start);

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
bool x_पत्तनम्_Check_Valid_List_Mem(ध्रुव शून्य *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool x_पत्तनम्_Check_Valid_TCBMem(ध्रुव शून्य *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_राशिः_Mem(ध्रुव शून्य *ptr);

#परिभाषय पत्तनम्_VALID_LIST_MEM(ptr)     x_पत्तनम्_Check_Valid_List_Mem(ptr)
#परिभाषय पत्तनम्_VALID_TCB_MEM(ptr)      x_पत्तनम्_Check_Valid_TCBMem(ptr)
#परिभाषय पत्तनम्_VALID_राशिः_MEM(ptr)    x_Portcheck_Valid_राशिः_Mem(ptr)

/* ------------------------------------------------------ Misc ---------------------------------------------------------
 * - Miscellaneous porting macros
 * - These are not part of the FreeRTOS porting interface, but are used by other FreeRTOS dependent components
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- App-Trace -------------------------

#यदि CONFIG_ESP_TRACE_ENABLE
बाह्य चञ्चल आधारः_Type_t x_पत्तनम्_Switch_Flag;
#परिभाषय os_task_switch_is_pended(_cpu_) (x_पत्तनम्_Switch_Flag)
#अन्यथा
#परिभाषय os_task_switch_is_pended(_cpu_) (false)
#यदि_अन्तः

// --------------- Compatibility Includes ------------------
/*
ESP-IDF currently does not have a "Include what you use" policy. A lot of files implicitly pull in API through
portmacro.h. Therefore, we need to keep these headers around for now to allow the rest of IDF to compile.

[refactor-todo] Clean up ESP-IDF inclusion dependencies and add a inclusion check.
*/

#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <stdarg.h>
#अन्तर्भाव <limits.h>
#अन्तर्भाव "esp_attr.h"
#अन्तर्भाव "esp_newlib.h"
#अन्तर्भाव "esp_heap_caps.h"
#अन्तर्भाव "esp_rom_sys.h"
#अन्तर्भाव "esp_system.h"             /* required by esp_get_...() functions in portable.h. [refactor-todo] Update portable.h */


#यदि_परिभाषितम् __cplusplus
}
#यदि_अन्तः

#यदि_अन्तः // __ASSEMBLER__
