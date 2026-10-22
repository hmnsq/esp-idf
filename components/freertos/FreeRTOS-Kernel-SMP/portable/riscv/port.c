/*
 * SPDX-FileCopyrightText: 2022-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_compiler.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "soc/soc_caps.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "soc/periph_defs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "soc/system_reg.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hal/crosscore_int_ll.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hal/systimer_hal.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hal/systimer_ll.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "riscv/rvruntime-frames.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "riscv/rv_utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "riscv/interrupt.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_private/crosscore_int.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_private/esp_int_wdt.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_private/periph_ctrl.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_private/systimer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_attr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_heap_caps_init.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_task_wdt.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_intr_alloc.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_log.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "FreeRTOS.h"       /* This pulls in portmacro.h */
#ਅਨ੍ਤਰ੍ਭਾਵ "task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "port_systick.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "portmacro.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_memory_utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_macros.h"
#ਜੇ CONFIG_FREERTOS_ਲੜੀਵਾਰ_ਸਮਾਂ_STATS_USING_ESP_TIMER
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_timer.h"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FREERTOS_SYSTICK_ਵਰਤਦਾ_SYSTIMER
#ਅਨ੍ਤਰ੍ਭਾਵ "soc/periph_defs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "soc/system_reg.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hal/systimer_hal.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hal/systimer_ll.h"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_PM_TRACE
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_private/pm_trace.h"
#ਜੇ_ਅੰਤ //CONFIG_PM_TRACE

_Static_assert(ਪੋਰਟ_BYTE_ALIGNMENT == 16, "portBYTE_ALIGNMENT must be set to 16");
#ਜੇ CONFIG_ESP_SYSTEM_HW_ਢੇਰ_GUARD
/**
 * offsetof() can not be used in asm code. Then we need make sure that
 * PORT_OFFSET_PX_STACK and PORT_OFFSET_PX_END_OF_STACK have expected values.
 * Macro used in the portasm.S instead of variables to save at least 4 instruction calls
 * which accessing DRAM memory. This optimization saves CPU time in the interrupt handling.
 */

_Static_assert(offsetof( StaticTask_t, pxDummy6 ) == ਪੋਰਟ_ਵਿੱਥ_PX_ਢੇਰ);
_Static_assert(offsetof( StaticTask_t, pxDummy8 ) == ਪੋਰਟ_ਵਿੱਥ_PX_ਸਿਰਾ_ਦਾ_ਢੇਰ);
#ਜੇ_ਅੰਤ // CONFIG_ESP_SYSTEM_HW_STACK_GUARD

/* ---------------------------------------------------- Variables ------------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

ਅਧਾਰ_Type_t uxSchedulerRunning = 0;  // Duplicate of xSchedulerRunning, accessible to port files
ਅਸਥਿਰ UBaseType_t ux_ਰੁਕਾਵਟ_Nesting = 0;
#ਜੇ config_ਅੰਕ_CORES > 1
ਅਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੋਰਟ_ux_Core_Startup_ਪੂਰਾ[ਪੋਰਟ_ਅੰਕ_PROCESSORS] = {0};  // Indicates whether the core has completed its startup sequence
#ਜੇ_ਅੰਤ
ਪੋਰਟ_MUX_TYPE ਪੋਰਟ_x_Task_ਤਾਲਾ = ਪੋਰਟ_MUX_INITIALIZER_UNLOCKED;
ਪੋਰਟ_MUX_TYPE ਪੋਰਟ_x_ISRLock = ਪੋਰਟ_MUX_INITIALIZER_UNLOCKED;
ਅਸਥਿਰ ਅਧਾਰ_Type_t x_ਪੋਰਟ_Switch_Flag = 0;
__attribute__((aligned(16))) ਢੇਰ_Type_t x_Isr_ਢੇਰ[config_ISR_ਢੇਰ_ਮਾਪ];
ਢੇਰ_Type_t *x_Isr_ਢੇਰ_ਸਿਖਰ = &x_Isr_ਢੇਰ[0] + (config_ISR_ਢੇਰ_ਮਾਪ & (~((ਪੋਰਟ_ਸੂਚਕ_ਮਾਪ_TYPE)ਪੋਰਟ_BYTE_ALIGNMENT_ਨਕਾਬ)));

// Variables used for IDF style critical sections. These are orthogonal to FreeRTOS critical sections
ਸਥਿਰ UBaseType_t ਪੋਰਟ_ux_Critical_Nesting_IDF = 0;
ਸਥਿਰ UBaseType_t ਪੋਰਟ_ux_Critical_Old_ਰੁਕਾਵਟ_State_IDF = 0;
#ਜੇ CONFIG_FREERTOS_ਪੋਰਟ_THREAD_ਸੁਰੱਖਿਅਤ_CLAIM
ਅਸਥਿਰ bool ਪੋਰਟ_x_Thread_ਸੁਰੱਖਿਅਤ_Claimed = false;
#ਜੇ_ਅੰਤ

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// ------------------ Critical Sections --------------------

#ਜੇ CONFIG_FREERTOS_ਪੋਰਟ_THREAD_ਸੁਰੱਖਿਅਤ_CLAIM
ਖਾਲੀ x_ਪੋਰਟ_Thread_ਸੁਰੱਖਿਅਤ_Claim(ਖਾਲੀ)
{
    configASSERT(!x_ਪੋਰਟ_Can_Yield());
    configASSERT(!ਪੋਰਟ_x_Thread_ਸੁਰੱਖਿਅਤ_Claimed);
    ਪੋਰਟ_x_Thread_ਸੁਰੱਖਿਅਤ_Claimed = true;
}

ਖਾਲੀ x_ਪੋਰਟ_Thread_ਸੁਰੱਖਿਅਤ_Disclaim(ਖਾਲੀ)
{
    configASSERT(!x_ਪੋਰਟ_Can_Yield());
    configASSERT(ਪੋਰਟ_x_Thread_ਸੁਰੱਖਿਅਤ_Claimed);
    ਪੋਰਟ_x_Thread_ਸੁਰੱਖਿਅਤ_Claimed = false;
}
#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

ਖਾਲੀ v_ਪੋਰਟ_Enter_Critical(ਖਾਲੀ)
{
#ਜੇ CONFIG_FREERTOS_ਪੋਰਟ_THREAD_ਸੁਰੱਖਿਅਤ_CLAIM
    ਜੇ (unlikely(ਪੋਰਟ_x_Thread_ਸੁਰੱਖਿਅਤ_Claimed)) {
        ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ
    // Save current interrupt threshold and disable interrupts
    UBaseType_t old_thresh = ul_ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ();
    // Update the IDF critical nesting count
    ਪੋਰਟ_ux_Critical_Nesting_IDF++;
    ਜੇ (ਪੋਰਟ_ux_Critical_Nesting_IDF == 1) {
        // Save a copy of the old interrupt threshold
        ਪੋਰਟ_ux_Critical_Old_ਰੁਕਾਵਟ_State_IDF = (UBaseType_t) old_thresh;
    }
}

ਖਾਲੀ v_ਪੋਰਟ_Exit_Critical(ਖਾਲੀ)
{
#ਜੇ CONFIG_FREERTOS_ਪੋਰਟ_THREAD_ਸੁਰੱਖਿਅਤ_CLAIM
    ਜੇ (unlikely(ਪੋਰਟ_x_Thread_ਸੁਰੱਖਿਅਤ_Claimed)) {
        ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ

    /* Critical section nesting coung must never be negative */
    configASSERT( ਪੋਰਟ_ux_Critical_Nesting_IDF > 0 );

    ਜੇ (ਪੋਰਟ_ux_Critical_Nesting_IDF > 0) {
        ਪੋਰਟ_ux_Critical_Nesting_IDF--;

        ਜੇ (ਪੋਰਟ_ux_Critical_Nesting_IDF == 0) {
            // Restore the saved interrupt threshold
            v_ਪੋਰਟ_Clear_ਰੁਕਾਵਟ_ਨਕਾਬ((ਪੂਰਨ)ਪੋਰਟ_ux_Critical_Old_ਰੁਕਾਵਟ_State_IDF);
        }
    }
}

// ----------------------- System --------------------------

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਢੇਰ_WATCH_AREA_ਮਾਪ 32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਢੇਰ_WATCH_ਬਿੰਦੂ_ਸੰਖਿਆ (SOC_CPU_WATCHPOINTS_ਅੰਕ - 1)

ਖਾਲੀ v_ਪੋਰਟ_ਸੈੱਟ_ਢੇਰ_Watchpoint(ਖਾਲੀ *px_ਢੇਰ_ਸ਼ੁਰੂ)
{
    uint32_t addr = (uint32_t)px_ਢੇਰ_ਸ਼ੁਰੂ;
    addr = (addr + (ਢੇਰ_WATCH_AREA_ਮਾਪ - 1)) & (~(ਢੇਰ_WATCH_AREA_ਮਾਪ - 1));
    esp_cpu_ਸੈੱਟ_watchpoint(ਢੇਰ_WATCH_ਬਿੰਦੂ_ਸੰਖਿਆ, (ਅੱਖਰ *)addr, ਢੇਰ_WATCH_AREA_ਮਾਪ, ESP_CPU_WATCHPOINT_STORE);
}



/* ---------------------------------------------- Port Implementations -------------------------------------------------
 * Implementations of Porting Interface functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

UBaseType_t ul_ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ(ਖਾਲੀ)
{
    UBaseType_t prev_int_level = 0, int_level = 0;
#ਜੇ !SOC_INT_CLIC_SUPPORTED
    int_level = RVHAL_EXCM_LEVEL;
#ਨਹੀਂਤਾਂ
    int_level = RVHAL_EXCM_LEVEL_CLIC;
#ਜੇ_ਅੰਤ

    prev_int_level = rv_utils_ਸੈੱਟ_intlevel_regval(int_level);
    /**
     * In theory, this function should not return immediately as there is a
     * delay between the moment we mask the interrupt threshold register and
     * the moment a potential lower-priority interrupt is triggered (as said
     * above), it should have a delay of 2 machine cycles/instructions.
     *
     * However, in practice, this function has an epilogue of one instruction,
     * thus the instruction masking the interrupt threshold register is
     * followed by two instructions: `ret` and `csrrs` (RV_SET_CSR).
     * That's why we don't need any additional nop instructions here.
     */
    ਵਾਪਸ prev_int_level;
}

ਖਾਲੀ v_ਪੋਰਟ_Clear_ਰੁਕਾਵਟ_ਨਕਾਬ(UBaseType_t ਨਕਾਬ)
{
    rv_utils_restore_intlevel_regval(ਨਕਾਬ);
    /**
     * The delay between the moment we unmask the interrupt threshold register
     * and the moment the potential requested interrupt is triggered is not
     * null: up to three machine cycles/instructions can be executed.
     *
     * When compilation size optimization is enabled, this function and its
     * callers returning void will have NO epilogue, thus the instruction
     * following these calls will be executed.
     *
     * If the requested interrupt is a context switch to a higher priority
     * task then the one currently running, we MUST NOT execute any instruction
     * before the interrupt effectively happens.
     * In order to prevent this, force this routine to have a 3-instruction
     * delay before exiting.
     */
    asm ਅਸਥਿਰ ( "nop" );
    asm ਅਸਥਿਰ ( "nop" );
    asm ਅਸਥਿਰ ( "nop" );
}

ਅਧਾਰ_Type_t x_ਪੋਰਟ_Check_If_ਵਿੱਚ_ISR(ਖਾਲੀ)
{
    ਵਾਪਸ ux_ਰੁਕਾਵਟ_Nesting;
}

ਖਾਲੀ v_ਪੋਰਟ_Assert_If_ਵਿੱਚ_ISR(ਖਾਲੀ)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(x_ਪੋਰਟ_Check_If_ਵਿੱਚ_ISR() == 0);
}

// ------------------ Critical Sections --------------------

#ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 )
ਖਾਲੀ v_ਪੋਰਟ_Take_ਤਾਲਾ( ਪੋਰਟ_MUX_TYPE *ਤਾਲਾ )
{
    spinlock_acquire( ਤਾਲਾ, ਪੋਰਟ_MUX_NO_TIMEOUT);
}

ਖਾਲੀ v_ਪੋਰਟ_Release_ਤਾਲਾ( ਪੋਰਟ_MUX_TYPE *ਤਾਲਾ )
{
    spinlock_release( ਤਾਲਾ );
}
#ਜੇ_ਅੰਤ /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

ਖਾਲੀ v_ਪੋਰਟ_Yield(ਖਾਲੀ)
{
    // TODO: IDF-8113
    ਅਚਲ ਪੂਰਨ core_id = 0;

    ਜੇ (ux_ਰੁਕਾਵਟ_Nesting) {
        v_ਪੋਰਟ_Yield_From_ISR();
    } ਨਹੀਂਤਾਂ {

        esp_crosscore_int_send_yield(0);
        /* There are 3-4 instructions of latency between triggering the software
           interrupt and the CPU interrupt happening. Make sure it happened before
           we return, otherwise vTaskDelay() may return and execute 1-2
           instructions before the delay actually happens.

           (We could use the WFI instruction here, but there is a chance that
           the interrupt will happen while evaluating the other two conditions
           for an instant yield, and if that happens then the WFI would be
           waiting for the next interrupt to occur...)
        */
        ਜਦੋਂ (uxSchedulerRunning && crosscore_int_ll_get_state(core_id) != 0) {}
    }
}

ਖਾਲੀ v_ਪੋਰਟ_Yield_From_ISR( ਖਾਲੀ )
{
    //traceISR_EXIT_TO_SCHEDULER();
    uxSchedulerRunning = 1;
    x_ਪੋਰਟ_Switch_Flag = 1;
}
// ----------------------- System --------------------------

// ------------------- Run Time Stats ----------------------

// --------------------- TCB Cleanup -----------------------

#ਜੇ ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
ਸਥਿਰ ਖਾਲੀ v_ਪੋਰਟ_TLSPointers_Del_Cb( ਖਾਲੀ *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pv_Thread_Local_Storage_ਸੂਚਕ_ਸਾਰੇ_Del_Callback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_ਅੰਕ_THREAD_LOCAL_STORAGE_ਸੂਚਕ_ਸਾਰੇ / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    ਲਈ ( ਪੂਰਨ x = 0; x < ( config_ਅੰਕ_THREAD_LOCAL_STORAGE_ਸੂਚਕ_ਸਾਰੇ / 2 ); x++ ) {
        ਜੇ ( pv_Thread_Local_Storage_ਸੂਚਕ_ਸਾਰੇ_Del_Callback[ x ] != ਨਿਰਾ ) {  //If del cb is set
            /* In case the TLSP deletion callback has been overwritten by a TLS pointer, gracefully abort. */
            ਜੇ ( !esp_ptr_executable( pv_Thread_Local_Storage_ਸੂਚਕ_ਸਾਰੇ_Del_Callback[ x ] ) ) {
                ESP_EARLY_LOGE("FreeRTOS", "Fatal error: TLSP deletion callback at index %d overwritten with non-excutable pointer %p", x, pv_Thread_Local_Storage_ਸੂਚਕ_ਸਾਰੇ_Del_Callback[ x ]);
                abort();
            }

            pv_Thread_Local_Storage_ਸੂਚਕ_ਸਾਰੇ_Del_Callback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

ਖਾਲੀ v_ਪੋਰਟ_TCBPre_Delete_Hook( ਖਾਲੀ *pxTCB )
{
    #ਜੇ ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        ਬਾਹਰੀ ਖਾਲੀ vTaskPreDeletionHook( ਖਾਲੀ * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

    #ਜੇ ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
        /* Call TLS pointers deletion callbacks */
        v_ਪੋਰਟ_TLSPointers_Del_Cb( pxTCB );
    #ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
}

/* ------------------------------------------------ FreeRTOS Portable --------------------------------------------------
 * - Provides implementation for functions required by FreeRTOS
 * - Declared in portable.h
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------- Scheduler Start/End -------------------

ਅਧਾਰ_Type_t x_ਪੋਰਟ_ਸ਼ੁਰੂ_Scheduler(ਖਾਲੀ)
{
    ux_ਰੁਕਾਵਟ_Nesting = 0;
    ਪੋਰਟ_ux_Critical_Nesting_IDF = 0;
#ਜੇ CONFIG_FREERTOS_ਪੋਰਟ_THREAD_ਸੁਰੱਖਿਅਤ_CLAIM
    ਪੋਰਟ_x_Thread_ਸੁਰੱਖਿਅਤ_Claimed = false;
#ਜੇ_ਅੰਤ
    uxSchedulerRunning = 0;
#ਜੇ config_ਅੰਕ_CORES > 1
    ਪੋਰਟ_ux_Core_Startup_ਪੂਰਾ[x_ਪੋਰਟ_Get_Core_ID()] = 0;
#ਜੇ_ਅੰਤ
    /* Setup the hardware to generate the tick. */
    v_ਪੋਰਟ_ਤਿਆਰੀ_Timer();

    rv_utils_intr_global_enable();
    esprv_int_ਸੈੱਟ_ਹੱਦ(RVHAL_INTR_ENABLE_THRESH); /* set global interrupt masking level */

    v_ਪੋਰਟ_Yield();

    /*Should not get here*/
    ਵਾਪਸ pdFALSE;
}

ਖਾਲੀ v_ਪੋਰਟ_ਸਿਰਾ_Scheduler(ਖਾਲੀ)
{
    /* very unlikely this function will be called, so just trap here */
    abort();
}

// ------------------------ Stack --------------------------



/**
 * @brief Allocate and initialize GCC TLS area
 *
 * This function allocates and initializes the area on the stack used to store GCC TLS (Thread Local Storage) variables.
 * - The area's size is derived from the TLS section's linker variables, and rounded up to a multiple of 16 bytes
 * - The allocated area is aligned to a 16-byte aligned address
 * - The TLS variables in the area are then initialized
 *
 * Each task access the TLS variables using the THREADPTR register plus an offset to obtain the address of the variable.
 * The value for the THREADPTR register is also calculated by this function, and that value should be use to initialize
 * the THREADPTR register.
 *
 * @param[in] uxStackPointer Current stack pointer address
 * @param[out] ret_threadptr_reg_init Calculated THREADPTR register initialization value
 * @return Stack pointer that points to the TLS area
 */
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ਢੇਰ_TLS(UBaseType_t ux_ਢੇਰ_ਸੂਚਕ, uint32_t *ret_threadptr_reg_init)
{
    /*
    LOW ADDRESS
            |---------------------------|   Linker Symbols
            | Section                   |   --------------
            | .flash.tdata              |
         0x0|---------------------------| <- _thread_local_data_start  ^
            | .flash.tdata              |                              |
            | int var_1 = 1;            |                              |
            |                           | <- _thread_local_data_end    |
            |                           | <- _thread_local_bss_start   | tls_area_size
            |                           |                              |
            | .flash.tbss (NOLOAD)      |                              |
            | int var_2;                |                              |
            |---------------------------| <- _thread_local_bss_end     V
            | Other data                |
            | ...                       |
            |---------------------------|
    HIGH ADDRESS
    */
    // Calculate TLS area size and round up to multiple of 16 bytes.
    ਬਾਹਰੀ ਅੱਖਰ thread_local_ਅੰਕੜੇ_ਸ਼ੁਰੂ, thread_local_ਅੰਕੜੇ_ਸਿਰਾ;
    ਬਾਹਰੀ ਅੱਖਰ thread_local_bss_ਸ਼ੁਰੂ, thread_local_bss_ਸਿਰਾ;
    ਅਚਲ uint32_t tls_ਅੰਕੜੇ_ਮਾਪ = (uint32_t)&thread_local_ਅੰਕੜੇ_ਸਿਰਾ - (uint32_t)&thread_local_ਅੰਕੜੇ_ਸ਼ੁਰੂ;
    ਅਚਲ uint32_t tls_bss_ਮਾਪ = (uint32_t)&thread_local_bss_ਸਿਰਾ - (uint32_t)&thread_local_bss_ਸ਼ੁਰੂ;
    ਅਚਲ uint32_t tls_area_ਮਾਪ = ESP_ALIGN_ਉੱਪਰ(tls_ਅੰਕੜੇ_ਮਾਪ + tls_bss_ਮਾਪ, 16);
    // TODO: check that TLS area fits the stack

    // Allocate space for the TLS area on the stack. The area must be aligned to 16-bytes
    ux_ਢੇਰ_ਸੂਚਕ = ESP_ALIGN_ਥੱਲੇ(ux_ਢੇਰ_ਸੂਚਕ - (UBaseType_t)tls_area_ਮਾਪ, 16);
    // Initialize the TLS data with the initialization values of each TLS variable
    ਸਮ੍ਰਿਤੀ_ਨਕਲ((ਖਾਲੀ *)ux_ਢੇਰ_ਸੂਚਕ, &thread_local_ਅੰਕੜੇ_ਸ਼ੁਰੂ, tls_ਅੰਕੜੇ_ਮਾਪ);
    // Initialize the TLS bss with zeroes
    memset((ਖਾਲੀ *)(ux_ਢੇਰ_ਸੂਚਕ + tls_ਅੰਕੜੇ_ਮਾਪ), 0, tls_bss_ਮਾਪ);

    // Save tls start address
    *ret_threadptr_reg_init = (uint32_t)ux_ਢੇਰ_ਸੂਚਕ;
    ਵਾਪਸ ux_ਢੇਰ_ਸੂਚਕ;
}

#ਜੇ CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
ਸਥਿਰ ਖਾਲੀ v_ਪੋਰਟ_Task_Wrapper(TaskFunction_t px_ਕੋਡ, ਖਾਲੀ *pvParameters)
{
    __asm__ ਅਸਥਿਰ(".cfi_undefined ra");  // tell to debugger that it's outermost (initial) frame
    ਬਾਹਰੀ ਖਾਲੀ __attribute__((noreturn)) panic_abort(ਅਚਲ ਅੱਖਰ *details);
    ਸਥਿਰ ਅੱਖਰ DRAM_ATTR ਸੁਨੇਹਾ[80] = "FreeRTOS: FreeRTOS Task \"\0";
    px_ਕੋਡ(pvParameters);
    /* FreeRTOS tasks should not return. Log the task name and abort. */
    /* We cannot use s(n)printf because it is in flash */
    strcat(ਸੁਨੇਹਾ, pcTaskGetName(ਨਿਰਾ));
    strcat(ਸੁਨੇਹਾ, "\" should not return, Aborting now!");
    panic_abort(ਸੁਨੇਹਾ);
}
#ਜੇ_ਅੰਤ // CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER

/**
 * @brief Initialize the task's starting interrupt stack frame
 *
 * This function initializes the task's starting interrupt stack frame. The dispatcher will use this stack frame in a
 * context restore routine. Therefore, the starting stack frame must be initialized as if the task was interrupted right
 * before its first instruction is called.
 *
 * - The stack frame is allocated to a 16-byte aligned address
 *
 * @param[in] uxStackPointer Current stack pointer address
 * @param[in] pxCode Task function
 * @param[in] pvParameters Task function's parameter
 * @param[in] threadptr_reg_init THREADPTR register initialization value
 * @return Stack pointer that points to the stack frame
 */
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ਢੇਰ_ਚੌਖਟਾ(UBaseType_t ux_ਢੇਰ_ਸੂਚਕ, TaskFunction_t px_ਕੋਡ, ਖਾਲੀ *pvParameters, uint32_t threadptr_reg_init)
{
    /*
    Allocate space for the task's starting interrupt stack frame.
    - The stack frame must be allocated to a 16-byte aligned address.
    - We use RV_STK_FRMSZ (instead of sizeof(RvExcFrame)) as it rounds up the total size to a multiple of 16.
    */
    ux_ਢੇਰ_ਸੂਚਕ = ESP_ALIGN_ਥੱਲੇ(ux_ਢੇਰ_ਸੂਚਕ - RV_STK_FRMSZ, 16);

    // Clear the entire interrupt stack frame
    Rv_Exc_ਚੌਖਟਾ *ਚੌਖਟਾ = (Rv_Exc_ਚੌਖਟਾ *)ux_ਢੇਰ_ਸੂਚਕ;
    memset(ਚੌਖਟਾ, 0, ਆਕਾਰ(Rv_Exc_ਚੌਖਟਾ));

    /* Initialize the stack frame. */
    ਬਾਹਰੀ uint32_t __global_pointer$;
    #ਜੇ CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
        ਚੌਖਟਾ->mepc = (UBaseType_t)v_ਪੋਰਟ_Task_Wrapper;
        ਚੌਖਟਾ->a0 = (UBaseType_t)px_ਕੋਡ;
        ਚੌਖਟਾ->a1 = (UBaseType_t)pvParameters;
    #ਨਹੀਂਤਾਂ
        ਚੌਖਟਾ->mepc = (UBaseType_t)px_ਕੋਡ;
        ਚੌਖਟਾ->a0 = (UBaseType_t)pvParameters;
    #ਜੇ_ਅੰਤ // CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
    ਚੌਖਟਾ->gp = (UBaseType_t)&__global_pointer$;
    ਚੌਖਟਾ->tp = (UBaseType_t)threadptr_reg_init;

    ਵਾਪਸ ux_ਢੇਰ_ਸੂਚਕ;
}

ਢੇਰ_Type_t *px_ਪੋਰਟ_Initialise_ਢੇਰ(ਢੇਰ_Type_t *px_ਸਿਖਰ_ਦਾ_ਢੇਰ, TaskFunction_t px_ਕੋਡ, ਖਾਲੀ *pvParameters)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __clang_analyzer__
    // Teach clang-tidy that pxTopOfStack cannot be a pointer to const
    ਅਸਥਿਰ ਢੇਰ_Type_t * px_ਆਰਜ਼ੀ = px_ਸਿਖਰ_ਦਾ_ਢੇਰ;
    px_ਸਿਖਰ_ਦਾ_ਢੇਰ = px_ਆਰਜ਼ੀ;
#ਜੇ_ਅੰਤ /*__clang_analyzer__ */
    /*
    HIGH ADDRESS
    |---------------------------| <- pxTopOfStack on entry
    | TLS Variables             |
    | ------------------------- | <- Start of usable stack
    | Starting stack frame      |
    | ------------------------- | <- pxTopOfStack on return (which is the tasks current SP)
    |             |             |
    |             |             |
    |             V             |
    ----------------------------- <- Bottom of stack
    LOW ADDRESS

    - All stack areas are aligned to 16 byte boundary
    - We use UBaseType_t for all of stack area initialization functions for more convenient pointer arithmetic
    */

    UBaseType_t ux_ਢੇਰ_ਸੂਚਕ = (UBaseType_t)px_ਸਿਖਰ_ਦਾ_ਢੇਰ;
    configASSERT((ux_ਢੇਰ_ਸੂਚਕ & ਪੋਰਟ_BYTE_ALIGNMENT_ਨਕਾਬ) == 0);

    // Initialize GCC TLS area
    uint32_t threadptr_reg_init;
    ux_ਢੇਰ_ਸੂਚਕ = ux_Initialise_ਢੇਰ_TLS(ux_ਢੇਰ_ਸੂਚਕ, &threadptr_reg_init);
    configASSERT((ux_ਢੇਰ_ਸੂਚਕ & ਪੋਰਟ_BYTE_ALIGNMENT_ਨਕਾਬ) == 0);

    // Initialize the starting interrupt stack frame
    ux_ਢੇਰ_ਸੂਚਕ = ux_Initialise_ਢੇਰ_ਚੌਖਟਾ(ux_ਢੇਰ_ਸੂਚਕ, px_ਕੋਡ, pvParameters, threadptr_reg_init);
    configASSERT((ux_ਢੇਰ_ਸੂਚਕ & ਪੋਰਟ_BYTE_ALIGNMENT_ਨਕਾਬ) == 0);

    // Return the task's current stack pointer address which should point to the starting interrupt stack frame
    ਵਾਪਸ (ਢੇਰ_Type_t *)ux_ਢੇਰ_ਸੂਚਕ;
}

// ------------------- Hook Functions ----------------------

ਖਾਲੀ __attribute__((weak)) v_Application_ਢੇਰ_ਉਛਾਲ_Hook(TaskHandle_t xTask, ਅੱਖਰ *pcTaskName)
{
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ERR_STR1 "***ERROR*** A stack overflow in task "
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ERR_STR2 " has been detected."
    ਅਚਲ ਅੱਖਰ *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    ਅੱਖਰ buf[ਆਕਾਰ(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + ਆਕਾਰ(ERR_STR2) + 1 /* null char */] = {0};

    ਅੱਖਰ *dest = buf;
    ਲਈ (ਪੂਰਨ i = 0; i < ਆਕਾਰ(str) / ਆਕਾਰ(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
    esp_system_abort(buf);
}

#ਜੇ  (  configUSE_TICK_HOOK > 0 )
ਖਾਲੀ vApplicationTickHook( ਖਾਲੀ )
{
    esp_vApplicationTickHook();
}
#ਜੇ_ਅੰਤ

ਬਾਹਰੀ ਖਾਲੀ esp_vApplicationIdleHook(ਖਾਲੀ);
#ਜੇ CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK
/*
By default, the port uses vApplicationPassiveIdleHook() to run IDF style idle
hooks. However, users may also want to provide their own vApplicationPassiveIdleHook().
In this case, we use to -Wl,--wrap option to wrap the user provided vApplicationPassiveIdleHook()
*/
ਬਾਹਰੀ ਖਾਲੀ __real_vApplicationPassiveIdleHook( ਖਾਲੀ );
ਖਾਲੀ __wrap_vApplicationPassiveIdleHook( ਖਾਲੀ )
{
    esp_vApplicationIdleHook(); //Run IDF style hooks
    __real_vApplicationPassiveIdleHook(); //Call the user provided vApplicationPassiveIdleHook()
}
#ਨਹੀਂਤਾਂ // CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK
ਖਾਲੀ vApplicationPassiveIdleHook( ਖਾਲੀ )
{
    esp_vApplicationIdleHook(); //Run IDF style hooks
}
#ਜੇ_ਅੰਤ // CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK

/* ------------------------------------------------ Run Time Stats ------------------------------------------------- */

#ਜੇ ( CONFIG_FREERTOS_GENERATE_ਲੜੀਵਾਰ_ਸਮਾਂ_STATS )

config_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_TYPE x_ਪੋਰਟ_Get_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter_ਮੁੱਲ( ਖਾਲੀ )
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FREERTOS_ਲੜੀਵਾਰ_ਸਮਾਂ_STATS_USING_ESP_TIMER
    ਵਾਪਸ (config_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_TYPE) esp_timer_get_ਸਮਾਂ();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ 0;
#ਜੇ_ਅੰਤ // CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
}

#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
