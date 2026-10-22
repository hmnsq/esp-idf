/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#निर्देशः once

#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव <stdint.h>
#अन्तर्भाव "spinlock.h"
#अन्तर्भाव "xtensa/xtruntime.h"
#अन्तर्भाव "xtensa/config/core.h"
#अन्तर्भाव "xtensa/config/core-isa.h"
#अन्तर्भाव "xtensa/config/xt_specreg.h"
#अन्तर्भाव "xt_instr_macros.h"
#अन्तर्भाव "portbenchmark.h"
#अन्तर्भाव "esp_macros.h"
#अन्तर्भाव "esp_cpu.h"
#अन्तर्भाव "esp_private/crosscore_int.h"
#अन्तर्भाव "esp_memory_utils.h"

/*
Note: We should not include any FreeRTOS headers (directly or indirectly) here as this will create a reverse dependency
- FreeRTOS pulls in "portmacro.h" via
    - source.c <- FreeRTOS.h <- portable.h <- portmacro.h

- If we need types of task.h (such as TaskHandle_t), best put the feature somewhere else
- Be careful as some parts of C Standard Library Headers will pull in FreeRTOS headers
*/

#यदि_परिभाषितम् __cplusplus
बाह्य "C" {
#यदि_अन्तः

/* --------------------------------------------------- Port Types ------------------------------------------------------
 * - Port specific types.
 * - The settings in this file configure FreeRTOS correctly for the given hardware and compiler.
 * - These settings should not be altered.
 * - The port types must come before first as they are used further down the file
 * ------------------------------------------------------------------------------------------------------------------ */

#परिभाषय पत्तनम्_CHAR                    int8_t
#परिभाषय पत्तनम्_FLOAT                   भिन्न
#परिभाषय पत्तनम्_DOUBLE                  द्विगुण
#परिभाषय पत्तनम्_LONG                    int32_t
#परिभाषय पत्तनम्_SHORT                   int16_t
#परिभाषय पत्तनम्_राशिः_TYPE              uint8_t
#परिभाषय पत्तनम्_आधारः_TYPE               पूर्णाङ्क

प्रकारनिर्देश पत्तनम्_राशिः_TYPE              राशिः_Type_t;
प्रकारनिर्देश पत्तनम्_आधारः_TYPE               आधारः_Type_t;
प्रकारनिर्देश अचिह्नित पत्तनम्_आधारः_TYPE      UBaseType_t;

#यदि( configUSE_16_BIT_TICKS == 1 )
प्रकारनिर्देश uint16_t TickType_t;
#परिभाषय पत्तनम्_MAX_DELAY ( TickType_t ) 0xffff
#अन्यथा
प्रकारनिर्देश uint32_t TickType_t;
#परिभाषय पत्तनम्_MAX_DELAY ( TickType_t ) 0xffffffffUL
#यदि_अन्तः

/* Task function macros as described on the FreeRTOS.org WEB site. */
#परिभाषय पत्तनम्_TASK_FUNCTION_PROTO( vFunction, pvParameters )  शून्य vFunction( शून्य *pvParameters )
#परिभाषय पत्तनम्_TASK_FUNCTION( vFunction, pvParameters )        शून्य vFunction( शून्य *pvParameters )

/* ----------------------------------------------- Port Configurations -------------------------------------------------
 * - Configurations values supplied by each port
 * - Required by FreeRTOS
 * ------------------------------------------------------------------------------------------------------------------ */

#परिभाषय पत्तनम्_CRITICAL_NESTING_अन्तर्_TCB     1
#परिभाषय पत्तनम्_राशिः_GROWTH                ( -1 )
#परिभाषय पत्तनम्_TICK_PERIOD_MS              ( ( TickType_t ) 1000 / config_TICK_दरः_HZ )
#परिभाषय पत्तनम्_BYTE_ALIGNMENT              16    // Xtensa Windowed ABI requires the stack pointer to always be 16-byte aligned. See "isa_rm.pdf 8.1.1 Windowed Register Usage and Stack Layout"
#परिभाषय पत्तनम्_NOP()                       XT_NOP()    //Todo: Check if XT_NOP exists

/* ---------------------------------------------- Forward Declarations -------------------------------------------------
 * - Forward declarations of all the port functions and macros need to implement the FreeRTOS porting interface
 * - These must come before definition/declaration of the FreeRTOS porting interface
 * ------------------------------------------------------------------------------------------------------------------ */

// ---------------------- Spinlocks ------------------------

प्रकारनिर्देश spinlock_t                          पत्तनम्_MUX_TYPE;               /**< Spinlock type used by FreeRTOS critical sections */
#परिभाषय पत्तनम्_MUX_INITIALIZER_UNLOCKED        SPINLOCK_INITIALIZER        /**< Spinlock initializer */
#परिभाषय पत्तनम्_MUX_FREE_VAL                    SPINLOCK_FREE               /**< Spinlock is free. [refactor-todo] check if this is still required */
#परिभाषय पत्तनम्_MUX_NO_TIMEOUT                  SPINLOCK_WAIT_FOREVER       /**< When passed for 'timeout_cycles', spin forever if necessary. [refactor-todo] check if this is still required */
#परिभाषय पत्तनम्_MUX_TRY_तालकम्                    SPINLOCK_NO_WAIT            /**< Try to acquire the spinlock a single time only. [refactor-todo] check if this is still required */
#परिभाषय पत्तनम्_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

आधारः_Type_t x_पत्तनम्_Check_If_अन्तर्_ISR(शून्य);

/**
 * @brief Assert if in ISR context
 *
 * - Asserts on xPortCheckIfInISR() internally
 */
शून्य v_पत्तनम्_Assert_If_अन्तर्_ISR(शून्य);

// ------------------ Critical Sections --------------------

UBaseType_t ux_पत्तनम्_Enter_Critical_From_ISR( शून्य );
शून्य v_पत्तनम्_Exit_Critical_From_ISR( UBaseType_t level );

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
स्थिर अन्तःस्थ शून्य __attribute__((always_inline)) v_पत्तनम्_Yield_From_ISR( शून्य );

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


/* ----------------------------------------- FreeRTOS SMP Porting Interface --------------------------------------------
 * - Contains all the mappings of the macros required by FreeRTOS SMP
 * - Must come after forward declare as some porting macros map to declared functions
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- Memory --------------------------

// --------------------- Interrupts ------------------------

#परिभाषय पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्()            ({ \
    अचिह्नित पूर्णाङ्क पूर्व_level = XTOS_समुच्चयः_INTLEVEL(XCHAL_EXCM_LEVEL); \
    portbenchmark_व्यवधानम्_DISABLE(); \
    पूर्व_level = ((पूर्व_level >> XCHAL_PS_INTLEVEL_सरणम्) & XCHAL_PS_INTLEVEL_आवरणम्); \
    पूर्व_level; \
})
#परिभाषय पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्_FROM_ISR()   पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्()
#परिभाषय पत्तनम्_DISABLE_INTERRUPTS()            पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्()

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
#परिभाषय पत्तनम्_CLEAR_व्यवधानम्_आवरणम्(x)         ({ \
    अचिह्नित पूर्णाङ्क ps_reg; \
    RSR(XT_REG_PS, ps_reg); \
    ps_reg = (ps_reg & ~XCHAL_PS_INTLEVEL_आवरणम्); \
    ps_reg |= ((x << XCHAL_PS_INTLEVEL_सरणम्) & XCHAL_PS_INTLEVEL_आवरणम्); \
    XTOS_RESTORE_INTLEVEL(ps_reg); \
})
#परिभाषय पत्तनम्_CLEAR_व्यवधानम्_आवरणम्_FROM_ISR(x)    पत्तनम्_CLEAR_व्यवधानम्_आवरणम्(x)
#परिभाषय पत्तनम्_ENABLE_INTERRUPTS()     ({ \
    portbenchmark_व्यवधानम्_RESTORE(0); \
    XTOS_समुच्चयः_INTLEVEL(0); \
})

/* Compatibility */
#परिभाषय पत्तनम्_RESTORE_INTERRUPTS(x)   पत्तनम्_CLEAR_व्यवधानम्_आवरणम्(x)

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

बाह्य UBaseType_t vTaskEnterCriticalFromISR( शून्य );
बाह्य शून्य vTaskExitCriticalFromISR( UBaseType_t ux_Saved_व्यवधानम्_स्थितिः );
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

//Timers are already configured, so nothing to do for configuration of run time stats timer
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
 * - For implementation of non-inlined functions, see port.c
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

// ------------------ Critical Sections --------------------

// ---------------------- Yielding -------------------------

बाह्य शून्य frxt_सज्जता_switch( शून्य );     //Defined in portasm.S

स्थिर अन्तःस्थ शून्य __attribute__((always_inline)) v_पत्तनम्_Yield_Core( आधारः_Type_t xCoreID )
{
    esp_crosscore_int_send_yield( xCoreID );
}

स्थिर अन्तःस्थ शून्य __attribute__((always_inline)) v_पत्तनम्_Yield_From_ISR( शून्य )
{
    //traceISR_EXIT_TO_SCHEDULER();
    frxt_सज्जता_switch();
}

// ----------------------- System --------------------------

स्थिर अन्तःस्थ आधारः_Type_t __attribute__((always_inline)) x_पत्तनम्_Get_Core_ID( शून्य )
{
    प्रत्यागम (आधारः_Type_t) esp_cpu_get_core_id();
}

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These macros and functions need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

स्थिर अन्तःस्थ आधारः_Type_t x_पत्तनम्_अन्तर्_Isr_Context(शून्य)
{
    //Just call the FreeRTOS port interface version
    प्रत्यागम x_पत्तनम्_Check_If_अन्तर्_ISR();
}

आधारः_Type_t x_पत्तनम्_Interrupted_From_ISRContext(शून्य);

स्थिर अन्तःस्थ UBaseType_t x_पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्_From_ISR(शून्य)
{
    UBaseType_t पूर्व_int_level = XTOS_समुच्चयः_INTLEVEL(XCHAL_EXCM_LEVEL);
    portbenchmark_व्यवधानम्_DISABLE();
    प्रत्यागम पूर्व_int_level;
}

स्थिर अन्तःस्थ शून्य v_पत्तनम्_Clear_व्यवधानम्_आवरणम्_From_ISR(UBaseType_t पूर्व_level)
{
    portbenchmark_व्यवधानम्_RESTORE(पूर्व_level);
    XTOS_RESTORE_JUST_INTLEVEL((पूर्णाङ्क) पूर्व_level);
}

// ---------------------- Spinlocks ------------------------

स्थिर अन्तःस्थ bool __attribute__((always_inline)) v_पत्तनम्_CPUAcquire_Mutex_Timeout(पत्तनम्_MUX_TYPE *mux, पूर्णाङ्क timeout)
{
    प्रत्यागम (spinlock_acquire(mux, timeout));
}

स्थिर अन्तःस्थ शून्य __attribute__((always_inline)) v_पत्तनम्_CPURelease_Mutex(पत्तनम्_MUX_TYPE *mux)
{
    spinlock_release(mux);
}

// ------------------ Critical Sections --------------------

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

आधारः_Type_t x_पत्तनम्_Enter_Critical_Timeout(पत्तनम्_MUX_TYPE *तालकम्, आधारः_Type_t timeout);

स्थिर अन्तःस्थ शून्य __attribute__((always_inline)) v_पत्तनम्_Enter_Critical_IDF(पत्तनम्_MUX_TYPE *तालकम्)
{
    x_पत्तनम्_Enter_Critical_Timeout(तालकम्, पत्तनम्_MUX_NO_TIMEOUT);
}

शून्य v_पत्तनम्_Exit_Critical_IDF(पत्तनम्_MUX_TYPE *तालकम्);

/*
In case any IDF libs called the port critical functions directly instead of through the macros.
The will just call the vPort...CriticalIDF() functions
*/
शून्य v_पत्तनम्_Enter_Critical(पत्तनम्_MUX_TYPE *तालकम्);
शून्य v_पत्तनम्_Exit_Critical(पत्तनम्_MUX_TYPE *तालकम्);

//IDF task critical sections
#परिभाषय पत्तनम्_TRY_ENTER_CRITICAL(तालकम्, timeout)       x_पत्तनम्_Enter_Critical_Timeout(तालकम्, timeout)
#परिभाषय पत्तनम्_ENTER_CRITICAL_IDF(तालकम्)                v_पत्तनम्_Enter_Critical_IDF(तालकम्)
#परिभाषय पत्तनम्_EXIT_CRITICAL_IDF(तालकम्)                 v_पत्तनम्_Exit_Critical_IDF(तालकम्)
//IDF ISR critical sections
#परिभाषय पत्तनम्_TRY_ENTER_CRITICAL_ISR(तालकम्, timeout)   x_पत्तनम्_Enter_Critical_Timeout(तालकम्, timeout)
#परिभाषय पत्तनम्_ENTER_CRITICAL_ISR(तालकम्)                v_पत्तनम्_Enter_Critical_IDF(तालकम्)
#परिभाषय पत्तनम्_EXIT_CRITICAL_ISR(तालकम्)                 v_पत्तनम्_Exit_Critical_IDF(तालकम्)
//IDF safe critical sections (they're the same)
#परिभाषय पत्तनम्_ENTER_CRITICAL_SAFE(तालकम्)               v_पत्तनम्_Enter_Critical_IDF(तालकम्)
#परिभाषय पत्तनम्_EXIT_CRITICAL_SAFE(तालकम्)                v_पत्तनम्_Exit_Critical_IDF(तालकम्)

// ---------------------- Yielding -------------------------

स्थिर अन्तःस्थ bool x_पत्तनम्_Can_Yield(शून्य)
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

    प्रत्यागम ((ps_reg & PS_INTLEVEL_आवरणम्) == 0);
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
#अन्तर्भाव <xtensa/hal.h>
#अन्तर्भाव "esp_attr.h"
#अन्तर्भाव "esp_newlib.h"
#अन्तर्भाव "esp_heap_caps.h"
#अन्तर्भाव "esp_rom_sys.h"
#अन्तर्भाव "esp_system.h"
#अन्तर्भाव "portbenchmark.h"
#अन्तर्भाव <limits.h>
#अन्तर्भाव <xtensa/config/system.h>
#अन्तर्भाव <xtensa_api.h>

#यदि_परिभाषितम् __cplusplus
}
#यदि_अन्तः
