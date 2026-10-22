/*
 * SPDX-FileCopyrightText: 2022-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ "esp_compiler.h"
#ಅನ್ತರ್ಭಾವ "soc/soc_caps.h"
#ಅನ್ತರ್ಭಾವ "soc/periph_defs.h"
#ಅನ್ತರ್ಭಾವ "soc/system_reg.h"
#ಅನ್ತರ್ಭಾವ "hal/crosscore_int_ll.h"
#ಅನ್ತರ್ಭಾವ "hal/systimer_hal.h"
#ಅನ್ತರ್ಭಾವ "hal/systimer_ll.h"
#ಅನ್ತರ್ಭಾವ "riscv/rvruntime-frames.h"
#ಅನ್ತರ್ಭಾವ "riscv/rv_utils.h"
#ಅನ್ತರ್ಭಾವ "riscv/interrupt.h"
#ಅನ್ತರ್ಭಾವ "esp_private/crosscore_int.h"
#ಅನ್ತರ್ಭಾವ "esp_private/esp_int_wdt.h"
#ಅನ್ತರ್ಭಾವ "esp_private/periph_ctrl.h"
#ಅನ್ತರ್ಭಾವ "esp_private/systimer.h"
#ಅನ್ತರ್ಭಾವ "esp_attr.h"
#ಅನ್ತರ್ಭಾವ "esp_system.h"
#ಅನ್ತರ್ಭಾವ "esp_heap_caps_init.h"
#ಅನ್ತರ್ಭಾವ "esp_task_wdt.h"
#ಅನ್ತರ್ಭಾವ "esp_task.h"
#ಅನ್ತರ್ಭಾವ "esp_intr_alloc.h"
#ಅನ್ತರ್ಭಾವ "esp_log.h"
#ಅನ್ತರ್ಭಾವ "FreeRTOS.h"       /* This pulls in portmacro.h */
#ಅನ್ತರ್ಭಾವ "task.h"
#ಅನ್ತರ್ಭಾವ "port_systick.h"
#ಅನ್ತರ್ಭಾವ "portmacro.h"
#ಅನ್ತರ್ಭಾವ "esp_memory_utils.h"
#ಅನ್ತರ್ಭಾವ "esp_macros.h"
#ಆದರೆ CONFIG_FREERTOS_ಸತತ_ಸಮಯ_STATS_USING_ESP_TIMER
#ಅನ್ತರ್ಭಾವ "esp_timer.h"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FREERTOS_SYSTICK_ಬಳಸುತ್ತದೆ_SYSTIMER
#ಅನ್ತರ್ಭಾವ "soc/periph_defs.h"
#ಅನ್ತರ್ಭಾವ "soc/system_reg.h"
#ಅನ್ತರ್ಭಾವ "hal/systimer_hal.h"
#ಅನ್ತರ್ಭಾವ "hal/systimer_ll.h"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_PM_TRACE
#ಅನ್ತರ್ಭಾವ "esp_private/pm_trace.h"
#ಆದರೆ_ಅಂತ್ಯ //CONFIG_PM_TRACE

_Static_assert(ಪೋರ್ಟ್_BYTE_ALIGNMENT == 16, "portBYTE_ALIGNMENT must be set to 16");
#ಆದರೆ CONFIG_ESP_SYSTEM_HW_ರಾಶಿ_GUARD
/**
 * offsetof() can not be used in asm code. Then we need make sure that
 * PORT_OFFSET_PX_STACK and PORT_OFFSET_PX_END_OF_STACK have expected values.
 * Macro used in the portasm.S instead of variables to save at least 4 instruction calls
 * which accessing DRAM memory. This optimization saves CPU time in the interrupt handling.
 */

_Static_assert(offsetof( StaticTask_t, pxDummy6 ) == ಪೋರ್ಟ್_ಸ್ಥಾನಾಂತರ_PX_ರಾಶಿ);
_Static_assert(offsetof( StaticTask_t, pxDummy8 ) == ಪೋರ್ಟ್_ಸ್ಥಾನಾಂತರ_PX_ಕೊನೆ_ದ_ರಾಶಿ);
#ಆದರೆ_ಅಂತ್ಯ // CONFIG_ESP_SYSTEM_HW_STACK_GUARD

/* ---------------------------------------------------- Variables ------------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

ತಳ_Type_t uxSchedulerRunning = 0;  // Duplicate of xSchedulerRunning, accessible to port files
ಅಸ್ಥಿರ UBaseType_t ux_ಅಡಚಣೆ_Nesting = 0;
#ಆದರೆ config_ಅಂಕೆ_CORES > 1
ಅಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೋರ್ಟ್_ux_Core_Startup_ಮುಗಿದ[ಪೋರ್ಟ್_ಅಂಕೆ_PROCESSORS] = {0};  // Indicates whether the core has completed its startup sequence
#ಆದರೆ_ಅಂತ್ಯ
ಪೋರ್ಟ್_MUX_TYPE ಪೋರ್ಟ್_x_Task_ಬೀಗ = ಪೋರ್ಟ್_MUX_INITIALIZER_UNLOCKED;
ಪೋರ್ಟ್_MUX_TYPE ಪೋರ್ಟ್_x_ISRLock = ಪೋರ್ಟ್_MUX_INITIALIZER_UNLOCKED;
ಅಸ್ಥಿರ ತಳ_Type_t x_ಪೋರ್ಟ್_Switch_Flag = 0;
__attribute__((aligned(16))) ರಾಶಿ_Type_t x_Isr_ರಾಶಿ[config_ISR_ರಾಶಿ_ಗಾತ್ರ];
ರಾಶಿ_Type_t *x_Isr_ರಾಶಿ_ತುದಿ = &x_Isr_ರಾಶಿ[0] + (config_ISR_ರಾಶಿ_ಗಾತ್ರ & (~((ಪೋರ್ಟ್_ಸೂಚಕ_ಗಾತ್ರ_TYPE)ಪೋರ್ಟ್_BYTE_ALIGNMENT_ಮುಖವಾಡ)));

// Variables used for IDF style critical sections. These are orthogonal to FreeRTOS critical sections
ಸ್ಥಿರ UBaseType_t ಪೋರ್ಟ್_ux_Critical_Nesting_IDF = 0;
ಸ್ಥಿರ UBaseType_t ಪೋರ್ಟ್_ux_Critical_Old_ಅಡಚಣೆ_State_IDF = 0;
#ಆದರೆ CONFIG_FREERTOS_ಪೋರ್ಟ್_THREAD_ಸುರಕ್ಷಿತ_CLAIM
ಅಸ್ಥಿರ bool ಪೋರ್ಟ್_x_Thread_ಸುರಕ್ಷಿತ_Claimed = false;
#ಆದರೆ_ಅಂತ್ಯ

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// ------------------ Critical Sections --------------------

#ಆದರೆ CONFIG_FREERTOS_ಪೋರ್ಟ್_THREAD_ಸುರಕ್ಷಿತ_CLAIM
ಶೂನ್ಯ x_ಪೋರ್ಟ್_Thread_ಸುರಕ್ಷಿತ_Claim(ಶೂನ್ಯ)
{
    configASSERT(!x_ಪೋರ್ಟ್_Can_Yield());
    configASSERT(!ಪೋರ್ಟ್_x_Thread_ಸುರಕ್ಷಿತ_Claimed);
    ಪೋರ್ಟ್_x_Thread_ಸುರಕ್ಷಿತ_Claimed = true;
}

ಶೂನ್ಯ x_ಪೋರ್ಟ್_Thread_ಸುರಕ್ಷಿತ_Disclaim(ಶೂನ್ಯ)
{
    configASSERT(!x_ಪೋರ್ಟ್_Can_Yield());
    configASSERT(ಪೋರ್ಟ್_x_Thread_ಸುರಕ್ಷಿತ_Claimed);
    ಪೋರ್ಟ್_x_Thread_ಸುರಕ್ಷಿತ_Claimed = false;
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Enter_Critical(ಶೂನ್ಯ)
{
#ಆದರೆ CONFIG_FREERTOS_ಪೋರ್ಟ್_THREAD_ಸುರಕ್ಷಿತ_CLAIM
    ಆದರೆ (unlikely(ಪೋರ್ಟ್_x_Thread_ಸುರಕ್ಷಿತ_Claimed)) {
        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ
    // Save current interrupt threshold and disable interrupts
    UBaseType_t old_thresh = ul_ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ();
    // Update the IDF critical nesting count
    ಪೋರ್ಟ್_ux_Critical_Nesting_IDF++;
    ಆದರೆ (ಪೋರ್ಟ್_ux_Critical_Nesting_IDF == 1) {
        // Save a copy of the old interrupt threshold
        ಪೋರ್ಟ್_ux_Critical_Old_ಅಡಚಣೆ_State_IDF = (UBaseType_t) old_thresh;
    }
}

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Exit_Critical(ಶೂನ್ಯ)
{
#ಆದರೆ CONFIG_FREERTOS_ಪೋರ್ಟ್_THREAD_ಸುರಕ್ಷಿತ_CLAIM
    ಆದರೆ (unlikely(ಪೋರ್ಟ್_x_Thread_ಸುರಕ್ಷಿತ_Claimed)) {
        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ

    /* Critical section nesting coung must never be negative */
    configASSERT( ಪೋರ್ಟ್_ux_Critical_Nesting_IDF > 0 );

    ಆದರೆ (ಪೋರ್ಟ್_ux_Critical_Nesting_IDF > 0) {
        ಪೋರ್ಟ್_ux_Critical_Nesting_IDF--;

        ಆದರೆ (ಪೋರ್ಟ್_ux_Critical_Nesting_IDF == 0) {
            // Restore the saved interrupt threshold
            v_ಪೋರ್ಟ್_Clear_ಅಡಚಣೆ_ಮುಖವಾಡ((ಪೂರ್ಣಾಂಕ)ಪೋರ್ಟ್_ux_Critical_Old_ಅಡಚಣೆ_State_IDF);
        }
    }
}

// ----------------------- System --------------------------

#ವ್ಯಾಖ್ಯಾನಿಸು ರಾಶಿ_WATCH_AREA_ಗಾತ್ರ 32
#ವ್ಯಾಖ್ಯಾನಿಸು ರಾಶಿ_WATCH_ಬಿಂದು_ಸಂಖ್ಯೆ (SOC_CPU_WATCHPOINTS_ಅಂಕೆ - 1)

ಶೂನ್ಯ v_ಪೋರ್ಟ್_ಗಣ_ರಾಶಿ_Watchpoint(ಶೂನ್ಯ *px_ರಾಶಿ_ಆರಂಭ)
{
    uint32_t addr = (uint32_t)px_ರಾಶಿ_ಆರಂಭ;
    addr = (addr + (ರಾಶಿ_WATCH_AREA_ಗಾತ್ರ - 1)) & (~(ರಾಶಿ_WATCH_AREA_ಗಾತ್ರ - 1));
    esp_cpu_ಗಣ_watchpoint(ರಾಶಿ_WATCH_ಬಿಂದು_ಸಂಖ್ಯೆ, (ಅಕ್ಷರ *)addr, ರಾಶಿ_WATCH_AREA_ಗಾತ್ರ, ESP_CPU_WATCHPOINT_STORE);
}



/* ---------------------------------------------- Port Implementations -------------------------------------------------
 * Implementations of Porting Interface functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

UBaseType_t ul_ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ(ಶೂನ್ಯ)
{
    UBaseType_t prev_int_level = 0, int_level = 0;
#ಆದರೆ !SOC_INT_CLIC_SUPPORTED
    int_level = RVHAL_EXCM_LEVEL;
#ಇಲ್ಲದಿದ್ದರೆ
    int_level = RVHAL_EXCM_LEVEL_CLIC;
#ಆದರೆ_ಅಂತ್ಯ

    prev_int_level = rv_utils_ಗಣ_intlevel_regval(int_level);
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
    ಹಿಂತಿರುಗಿ prev_int_level;
}

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Clear_ಅಡಚಣೆ_ಮುಖವಾಡ(UBaseType_t ಮುಖವಾಡ)
{
    rv_utils_restore_intlevel_regval(ಮುಖವಾಡ);
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
    asm ಅಸ್ಥಿರ ( "nop" );
    asm ಅಸ್ಥಿರ ( "nop" );
    asm ಅಸ್ಥಿರ ( "nop" );
}

ತಳ_Type_t x_ಪೋರ್ಟ್_Check_If_ಒಳ_ISR(ಶೂನ್ಯ)
{
    ಹಿಂತಿರುಗಿ ux_ಅಡಚಣೆ_Nesting;
}

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Assert_If_ಒಳ_ISR(ಶೂನ್ಯ)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(x_ಪೋರ್ಟ್_Check_If_ಒಳ_ISR() == 0);
}

// ------------------ Critical Sections --------------------

#ಆದರೆ ( config_ಸಂಖ್ಯೆ_ದ_CORES > 1 )
ಶೂನ್ಯ v_ಪೋರ್ಟ್_Take_ಬೀಗ( ಪೋರ್ಟ್_MUX_TYPE *ಬೀಗ_2 )
{
    spinlock_acquire( ಬೀಗ_2, ಪೋರ್ಟ್_MUX_NO_TIMEOUT);
}

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Release_ಬೀಗ( ಪೋರ್ಟ್_MUX_TYPE *ಬೀಗ_2 )
{
    spinlock_release( ಬೀಗ_2 );
}
#ಆದರೆ_ಅಂತ್ಯ /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Yield(ಶೂನ್ಯ)
{
    // TODO: IDF-8113
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ core_id = 0;

    ಆದರೆ (ux_ಅಡಚಣೆ_Nesting) {
        v_ಪೋರ್ಟ್_Yield_From_ISR();
    } ಇಲ್ಲದಿದ್ದರೆ {

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
        ಆಗ (uxSchedulerRunning && crosscore_int_ll_get_state(core_id) != 0) {}
    }
}

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Yield_From_ISR( ಶೂನ್ಯ )
{
    //traceISR_EXIT_TO_SCHEDULER();
    uxSchedulerRunning = 1;
    x_ಪೋರ್ಟ್_Switch_Flag = 1;
}
// ----------------------- System --------------------------

// ------------------- Run Time Stats ----------------------

// --------------------- TCB Cleanup -----------------------

#ಆದರೆ ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
ಸ್ಥಿರ ಶೂನ್ಯ v_ಪೋರ್ಟ್_TLSPointers_Del_Cb( ಶೂನ್ಯ *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pv_Thread_Local_Storage_ಸೂಚಕಗಳು_Del_Callback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_ಅಂಕೆ_THREAD_LOCAL_STORAGE_ಸೂಚಕಗಳು / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    ಗಾಗಿ ( ಪೂರ್ಣಾಂಕ x = 0; x < ( config_ಅಂಕೆ_THREAD_LOCAL_STORAGE_ಸೂಚಕಗಳು / 2 ); x++ ) {
        ಆದರೆ ( pv_Thread_Local_Storage_ಸೂಚಕಗಳು_Del_Callback[ x ] != ಖಾಲಿ ) {  //If del cb is set
            /* In case the TLSP deletion callback has been overwritten by a TLS pointer, gracefully abort. */
            ಆದರೆ ( !esp_ptr_executable( pv_Thread_Local_Storage_ಸೂಚಕಗಳು_Del_Callback[ x ] ) ) {
                ESP_EARLY_LOGE("FreeRTOS", "Fatal error: TLSP deletion callback at index %d overwritten with non-excutable pointer %p", x, pv_Thread_Local_Storage_ಸೂಚಕಗಳು_Del_Callback[ x ]);
                abort();
            }

            pv_Thread_Local_Storage_ಸೂಚಕಗಳು_Del_Callback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

ಶೂನ್ಯ v_ಪೋರ್ಟ್_TCBPre_Delete_Hook( ಶೂನ್ಯ *pxTCB )
{
    #ಆದರೆ ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        ಬಾಹ್ಯ ಶೂನ್ಯ vTaskPreDeletionHook( ಶೂನ್ಯ * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

    #ಆದರೆ ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
        /* Call TLS pointers deletion callbacks */
        v_ಪೋರ್ಟ್_TLSPointers_Del_Cb( pxTCB );
    #ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
}

/* ------------------------------------------------ FreeRTOS Portable --------------------------------------------------
 * - Provides implementation for functions required by FreeRTOS
 * - Declared in portable.h
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------- Scheduler Start/End -------------------

ತಳ_Type_t x_ಪೋರ್ಟ್_ಆರಂಭ_Scheduler(ಶೂನ್ಯ)
{
    ux_ಅಡಚಣೆ_Nesting = 0;
    ಪೋರ್ಟ್_ux_Critical_Nesting_IDF = 0;
#ಆದರೆ CONFIG_FREERTOS_ಪೋರ್ಟ್_THREAD_ಸುರಕ್ಷಿತ_CLAIM
    ಪೋರ್ಟ್_x_Thread_ಸುರಕ್ಷಿತ_Claimed = false;
#ಆದರೆ_ಅಂತ್ಯ
    uxSchedulerRunning = 0;
#ಆದರೆ config_ಅಂಕೆ_CORES > 1
    ಪೋರ್ಟ್_ux_Core_Startup_ಮುಗಿದ[x_ಪೋರ್ಟ್_Get_Core_ID()] = 0;
#ಆದರೆ_ಅಂತ್ಯ
    /* Setup the hardware to generate the tick. */
    v_ಪೋರ್ಟ್_ಸಿದ್ಧತೆ_Timer();

    rv_utils_intr_global_enable();
    esprv_int_ಗಣ_ಮಿತಿ(RVHAL_INTR_ENABLE_THRESH); /* set global interrupt masking level */

    v_ಪೋರ್ಟ್_Yield();

    /*Should not get here*/
    ಹಿಂತಿರುಗಿ pdFALSE;
}

ಶೂನ್ಯ v_ಪೋರ್ಟ್_ಕೊನೆ_Scheduler(ಶೂನ್ಯ)
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ರಾಶಿ_TLS(UBaseType_t ux_ರಾಶಿ_ಸೂಚಕ, uint32_t *ret_threadptr_reg_init)
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
    ಬಾಹ್ಯ ಅಕ್ಷರ thread_local_ದತ್ತಾಂಶ_ಆರಂಭ, thread_local_ದತ್ತಾಂಶ_ಕೊನೆ;
    ಬಾಹ್ಯ ಅಕ್ಷರ thread_local_bss_ಆರಂಭ, thread_local_bss_ಕೊನೆ;
    ಸ್ಥಿರಾಂಕ uint32_t tls_ದತ್ತಾಂಶ_ಗಾತ್ರ = (uint32_t)&thread_local_ದತ್ತಾಂಶ_ಕೊನೆ - (uint32_t)&thread_local_ದತ್ತಾಂಶ_ಆರಂಭ;
    ಸ್ಥಿರಾಂಕ uint32_t tls_bss_ಗಾತ್ರ = (uint32_t)&thread_local_bss_ಕೊನೆ - (uint32_t)&thread_local_bss_ಆರಂಭ;
    ಸ್ಥಿರಾಂಕ uint32_t tls_area_ಗಾತ್ರ = ESP_ALIGN_ಮೇಲೆ(tls_ದತ್ತಾಂಶ_ಗಾತ್ರ + tls_bss_ಗಾತ್ರ, 16);
    // TODO: check that TLS area fits the stack

    // Allocate space for the TLS area on the stack. The area must be aligned to 16-bytes
    ux_ರಾಶಿ_ಸೂಚಕ = ESP_ALIGN_ಕೆಳಕ್ಕೆ(ux_ರಾಶಿ_ಸೂಚಕ - (UBaseType_t)tls_area_ಗಾತ್ರ, 16);
    // Initialize the TLS data with the initialization values of each TLS variable
    ಸ್ಮರಣೆ_ನಕಲು((ಶೂನ್ಯ *)ux_ರಾಶಿ_ಸೂಚಕ, &thread_local_ದತ್ತಾಂಶ_ಆರಂಭ, tls_ದತ್ತಾಂಶ_ಗಾತ್ರ);
    // Initialize the TLS bss with zeroes
    memset((ಶೂನ್ಯ *)(ux_ರಾಶಿ_ಸೂಚಕ + tls_ದತ್ತಾಂಶ_ಗಾತ್ರ), 0, tls_bss_ಗಾತ್ರ);

    // Save tls start address
    *ret_threadptr_reg_init = (uint32_t)ux_ರಾಶಿ_ಸೂಚಕ;
    ಹಿಂತಿರುಗಿ ux_ರಾಶಿ_ಸೂಚಕ;
}

#ಆದರೆ CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
ಸ್ಥಿರ ಶೂನ್ಯ v_ಪೋರ್ಟ್_Task_Wrapper(TaskFunction_t px_ಸಂಕೇತ, ಶೂನ್ಯ *pvParameters)
{
    __asm__ ಅಸ್ಥಿರ(".cfi_undefined ra");  // tell to debugger that it's outermost (initial) frame
    ಬಾಹ್ಯ ಶೂನ್ಯ __attribute__((noreturn)) panic_abort(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *details);
    ಸ್ಥಿರ ಅಕ್ಷರ DRAM_ATTR ಸಂದೇಶ[80] = "FreeRTOS: FreeRTOS Task \"\0";
    px_ಸಂಕೇತ(pvParameters);
    /* FreeRTOS tasks should not return. Log the task name and abort. */
    /* We cannot use s(n)printf because it is in flash */
    strcat(ಸಂದೇಶ, pcTaskGetName(ಖಾಲಿ));
    strcat(ಸಂದೇಶ, "\" should not return, Aborting now!");
    panic_abort(ಸಂದೇಶ);
}
#ಆದರೆ_ಅಂತ್ಯ // CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER

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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ರಾಶಿ_ಚೌಕಟ್ಟು(UBaseType_t ux_ರಾಶಿ_ಸೂಚಕ, TaskFunction_t px_ಸಂಕೇತ, ಶೂನ್ಯ *pvParameters, uint32_t threadptr_reg_init)
{
    /*
    Allocate space for the task's starting interrupt stack frame.
    - The stack frame must be allocated to a 16-byte aligned address.
    - We use RV_STK_FRMSZ (instead of sizeof(RvExcFrame)) as it rounds up the total size to a multiple of 16.
    */
    ux_ರಾಶಿ_ಸೂಚಕ = ESP_ALIGN_ಕೆಳಕ್ಕೆ(ux_ರಾಶಿ_ಸೂಚಕ - RV_STK_FRMSZ, 16);

    // Clear the entire interrupt stack frame
    Rv_Exc_ಚೌಕಟ್ಟು *ಚೌಕಟ್ಟು = (Rv_Exc_ಚೌಕಟ್ಟು *)ux_ರಾಶಿ_ಸೂಚಕ;
    memset(ಚೌಕಟ್ಟು, 0, ಗಾತ್ರ(Rv_Exc_ಚೌಕಟ್ಟು));

    /* Initialize the stack frame. */
    ಬಾಹ್ಯ uint32_t __global_pointer$;
    #ಆದರೆ CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
        ಚೌಕಟ್ಟು->mepc = (UBaseType_t)v_ಪೋರ್ಟ್_Task_Wrapper;
        ಚೌಕಟ್ಟು->a0 = (UBaseType_t)px_ಸಂಕೇತ;
        ಚೌಕಟ್ಟು->a1 = (UBaseType_t)pvParameters;
    #ಇಲ್ಲದಿದ್ದರೆ
        ಚೌಕಟ್ಟು->mepc = (UBaseType_t)px_ಸಂಕೇತ;
        ಚೌಕಟ್ಟು->a0 = (UBaseType_t)pvParameters;
    #ಆದರೆ_ಅಂತ್ಯ // CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
    ಚೌಕಟ್ಟು->gp = (UBaseType_t)&__global_pointer$;
    ಚೌಕಟ್ಟು->tp = (UBaseType_t)threadptr_reg_init;

    ಹಿಂತಿರುಗಿ ux_ರಾಶಿ_ಸೂಚಕ;
}

ರಾಶಿ_Type_t *px_ಪೋರ್ಟ್_Initialise_ರಾಶಿ(ರಾಶಿ_Type_t *px_ತುದಿ_ದ_ರಾಶಿ, TaskFunction_t px_ಸಂಕೇತ, ಶೂನ್ಯ *pvParameters)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __clang_analyzer__
    // Teach clang-tidy that pxTopOfStack cannot be a pointer to const
    ಅಸ್ಥಿರ ರಾಶಿ_Type_t * px_ತಾತ್ಕಾಲಿಕ = px_ತುದಿ_ದ_ರಾಶಿ;
    px_ತುದಿ_ದ_ರಾಶಿ = px_ತಾತ್ಕಾಲಿಕ;
#ಆದರೆ_ಅಂತ್ಯ /*__clang_analyzer__ */
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

    UBaseType_t ux_ರಾಶಿ_ಸೂಚಕ = (UBaseType_t)px_ತುದಿ_ದ_ರಾಶಿ;
    configASSERT((ux_ರಾಶಿ_ಸೂಚಕ & ಪೋರ್ಟ್_BYTE_ALIGNMENT_ಮುಖವಾಡ) == 0);

    // Initialize GCC TLS area
    uint32_t threadptr_reg_init;
    ux_ರಾಶಿ_ಸೂಚಕ = ux_Initialise_ರಾಶಿ_TLS(ux_ರಾಶಿ_ಸೂಚಕ, &threadptr_reg_init);
    configASSERT((ux_ರಾಶಿ_ಸೂಚಕ & ಪೋರ್ಟ್_BYTE_ALIGNMENT_ಮುಖವಾಡ) == 0);

    // Initialize the starting interrupt stack frame
    ux_ರಾಶಿ_ಸೂಚಕ = ux_Initialise_ರಾಶಿ_ಚೌಕಟ್ಟು(ux_ರಾಶಿ_ಸೂಚಕ, px_ಸಂಕೇತ, pvParameters, threadptr_reg_init);
    configASSERT((ux_ರಾಶಿ_ಸೂಚಕ & ಪೋರ್ಟ್_BYTE_ALIGNMENT_ಮುಖವಾಡ) == 0);

    // Return the task's current stack pointer address which should point to the starting interrupt stack frame
    ಹಿಂತಿರುಗಿ (ರಾಶಿ_Type_t *)ux_ರಾಶಿ_ಸೂಚಕ;
}

// ------------------- Hook Functions ----------------------

ಶೂನ್ಯ __attribute__((weak)) v_Application_ರಾಶಿ_ಅತಿಪ್ರವಾಹ_Hook(TaskHandle_t xTask, ಅಕ್ಷರ *pcTaskName)
{
#ವ್ಯಾಖ್ಯಾನಿಸು ERR_STR1 "***ERROR*** A stack overflow in task "
#ವ್ಯಾಖ್ಯಾನಿಸು ERR_STR2 " has been detected."
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    ಅಕ್ಷರ buf[ಗಾತ್ರ(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + ಗಾತ್ರ(ERR_STR2) + 1 /* null char */] = {0};

    ಅಕ್ಷರ *dest = buf;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಗಾತ್ರ(str) / ಗಾತ್ರ(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
    esp_system_abort(buf);
}

#ಆದರೆ  (  configUSE_TICK_HOOK > 0 )
ಶೂನ್ಯ vApplicationTickHook( ಶೂನ್ಯ )
{
    esp_vApplicationTickHook();
}
#ಆದರೆ_ಅಂತ್ಯ

ಬಾಹ್ಯ ಶೂನ್ಯ esp_vApplicationIdleHook(ಶೂನ್ಯ);
#ಆದರೆ CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK
/*
By default, the port uses vApplicationPassiveIdleHook() to run IDF style idle
hooks. However, users may also want to provide their own vApplicationPassiveIdleHook().
In this case, we use to -Wl,--wrap option to wrap the user provided vApplicationPassiveIdleHook()
*/
ಬಾಹ್ಯ ಶೂನ್ಯ __real_vApplicationPassiveIdleHook( ಶೂನ್ಯ );
ಶೂನ್ಯ __wrap_vApplicationPassiveIdleHook( ಶೂನ್ಯ )
{
    esp_vApplicationIdleHook(); //Run IDF style hooks
    __real_vApplicationPassiveIdleHook(); //Call the user provided vApplicationPassiveIdleHook()
}
#ಇಲ್ಲದಿದ್ದರೆ // CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK
ಶೂನ್ಯ vApplicationPassiveIdleHook( ಶೂನ್ಯ )
{
    esp_vApplicationIdleHook(); //Run IDF style hooks
}
#ಆದರೆ_ಅಂತ್ಯ // CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK

/* ------------------------------------------------ Run Time Stats ------------------------------------------------- */

#ಆದರೆ ( CONFIG_FREERTOS_GENERATE_ಸತತ_ಸಮಯ_STATS )

config_ಸತತ_ಸಮಯ_COUNTER_TYPE x_ಪೋರ್ಟ್_Get_ಸತತ_ಸಮಯ_Counter_ಮೌಲ್ಯ( ಶೂನ್ಯ )
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FREERTOS_ಸತತ_ಸಮಯ_STATS_USING_ESP_TIMER
    ಹಿಂತಿರುಗಿ (config_ಸತತ_ಸಮಯ_COUNTER_TYPE) esp_timer_get_ಸಮಯ();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ 0;
#ಆದರೆ_ಅಂತ್ಯ // CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
}

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
