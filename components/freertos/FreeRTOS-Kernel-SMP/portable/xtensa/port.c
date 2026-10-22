/*
 * SPDX-FileCopyrightText: 2022-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ "esp_compiler.h"
#ಅನ್ತರ್ಭಾವ "FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "task.h"   //For vApplicationStackOverflowHook
#ಅನ್ತರ್ಭಾವ "port_systick.h"
#ಅನ್ತರ್ಭಾವ "portmacro.h"
#ಅನ್ತರ್ಭಾವ "spinlock.h"
#ಅನ್ತರ್ಭಾವ "xt_instr_macros.h"
#ಅನ್ತರ್ಭಾವ "xtensa_context.h"
#ಅನ್ತರ್ಭಾವ "xtensa/corebits.h"
#ಅನ್ತರ್ಭಾವ "xtensa/config/core.h"
#ಅನ್ತರ್ಭಾವ "xtensa/config/core-isa.h"
#ಅನ್ತರ್ಭಾವ "xtensa/xtruntime.h"
#ಅನ್ತರ್ಭಾವ "esp_private/freertos_idf_additions_priv.h"
#ಅನ್ತರ್ಭಾವ "esp_private/esp_int_wdt.h"
#ಅನ್ತರ್ಭಾವ "esp_private/systimer.h"
#ಅನ್ತರ್ಭಾವ "esp_private/periph_ctrl.h"
#ಅನ್ತರ್ಭಾವ "esp_attr.h"
#ಅನ್ತರ್ಭಾವ "esp_heap_caps.h"
#ಅನ್ತರ್ಭಾವ "esp_system.h"
#ಅನ್ತರ್ಭಾವ "esp_task.h"
#ಅನ್ತರ್ಭಾವ "esp_log.h"
#ಅನ್ತರ್ಭಾವ "esp_cpu.h"
#ಅನ್ತರ್ಭಾವ "esp_rom_sys.h"
#ಅನ್ತರ್ಭಾವ "esp_task_wdt.h"
#ಅನ್ತರ್ಭಾವ "esp_heap_caps_init.h"
#ಅನ್ತರ್ಭಾವ "esp_freertos_hooks.h"
#ಅನ್ತರ್ಭಾವ "esp_intr_alloc.h"
#ಅನ್ತರ್ಭಾವ "esp_memory_utils.h"
#ಅನ್ತರ್ಭಾವ "esp_macros.h"
#ಅನ್ತರ್ಭಾವ <xtensa/hal.h>             /* required for xthal_get_ccount() */
#ಆದರೆ CONFIG_FREERTOS_ಸತತ_ಸಮಯ_STATS_USING_ESP_TIMER
#ಅನ್ತರ್ಭಾವ "esp_timer.h"
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FREERTOS_SYSTICK_ಬಳಸುತ್ತದೆ_SYSTIMER
#ಅನ್ತರ್ಭಾವ "soc/periph_defs.h"
#ಅನ್ತರ್ಭಾವ "soc/system_reg.h"
#ಅನ್ತರ್ಭಾವ "hal/systimer_hal.h"
#ಅನ್ತರ್ಭಾವ "hal/systimer_ll.h"
#ಆದರೆ_ಅಂತ್ಯ // CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER

_Static_assert(ಪೋರ್ಟ್_BYTE_ALIGNMENT == 16, "portBYTE_ALIGNMENT must be set to 16");

/* ---------------------------------------------------- Variables ------------------------------------------------------
 * - Various variables used to maintain the FreeRTOS port's state. Used from both port.c and various .S files
 * - Constant offsets are used by assembly to jump to particular TCB members or a stack area (such as the CPSA). We use
 *   C constants instead of preprocessor macros due to assembly lacking "offsetof()".
 * ------------------------------------------------------------------------------------------------------------------ */

#ಆದರೆ XCHAL_CP_ಅಂಕೆ > 0
/* Offsets used to navigate to a task's CPSA on the stack */
ಸ್ಥಿರಾಂಕ DRAM_ATTR uint32_t ಸ್ಥಾನಾಂತರ_px_ಕೊನೆ_ದ_ರಾಶಿ = offsetof(StaticTask_t, pxDummy8);
ಸ್ಥಿರಾಂಕ DRAM_ATTR uint32_t ಸ್ಥಾನಾಂತರ_cpsa = XT_CP_ಗಾತ್ರ;  /* Offset to start of the CPSA area on the stack. See uxInitialiseStackCPSA(). */
#ಆದರೆ config_ಅಂಕೆ_CORES > 1
/* Offset to TCB_t.uxCoreAffinityMask member. Used to pin unpinned tasks that use the FPU. */
ಸ್ಥಿರಾಂಕ DRAM_ATTR uint32_t ಸ್ಥಾನಾಂತರ_ux_Core_Affinity_ಮುಖವಾಡ = offsetof(StaticTask_t, uxDummy26);
#ಆದರೆ configUSE_CORE_AFFINITY != 1
#ದೋಷ "configUSE_CORE_AFFINITY must be 1 on multicore targets with coprocessor support"
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ /* configNUM_CORES > 1 */
#ಆದರೆ_ಅಂತ್ಯ /* XCHAL_CP_NUM > 0 */

ಅಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೋರ್ಟ್_x_Scheduler_Running[ಪೋರ್ಟ್_ಅಂಕೆ_PROCESSORS] = {0}; // Indicates whether scheduler is running on a per-core basis
ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಪೋರ್ಟ್_ಅಡಚಣೆ_Nesting[ಪೋರ್ಟ್_ಅಂಕೆ_PROCESSORS] = {0};  // Interrupt nesting level. Increased/decreased in portasm.c, _frxt_int_enter/_frxt_int_exit
ಅಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೋರ್ಟ್_ux_Core_Startup_ಮುಗಿದ[ಪೋರ್ಟ್_ಅಂಕೆ_PROCESSORS] = {0};  // Indicates whether the core has completed its startup sequence
#ಆದರೆ ( config_ಸಂಖ್ಯೆ_ದ_CORES > 1 )
//FreeRTOS SMP Locks
ಪೋರ್ಟ್_MUX_TYPE ಪೋರ್ಟ್_x_Task_ಬೀಗ = ಪೋರ್ಟ್_MUX_INITIALIZER_UNLOCKED;
ಪೋರ್ಟ್_MUX_TYPE ಪೋರ್ಟ್_x_ISRLock = ಪೋರ್ಟ್_MUX_INITIALIZER_UNLOCKED;
#ಆದರೆ_ಅಂತ್ಯ /* configNUMBER_OF_CORES > 1 */

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

ತಳ_Type_t x_ಪೋರ್ಟ್_Interrupted_From_ISRContext(ಶೂನ್ಯ)
{
    ಹಿಂತಿರುಗಿ (ಪೋರ್ಟ್_ಅಡಚಣೆ_Nesting[x_ಪೋರ್ಟ್_Get_Core_ID()] != 0);
}

// ------------------ Critical Sections --------------------

/*
Variables used by IDF critical sections only (SMP tracks critical nesting inside TCB now)
[refactor-todo] Figure out how IDF critical sections will be merged with SMP FreeRTOS critical sections
*/
ತಳ_Type_t ಪೋರ್ಟ್_ux_Critical_Nesting_IDF[ಪೋರ್ಟ್_ಅಂಕೆ_PROCESSORS] = {0};
ತಳ_Type_t ಪೋರ್ಟ್_ux_Critical_Old_ಅಡಚಣೆ_State_IDF[ಪೋರ್ಟ್_ಅಂಕೆ_PROCESSORS] = {0};
#ಆದರೆ CONFIG_FREERTOS_ಪೋರ್ಟ್_THREAD_ಸುರಕ್ಷಿತ_CLAIM
ಅಸ್ಥಿರ bool ಪೋರ್ಟ್_x_Thread_ಸುರಕ್ಷಿತ_Claimed = false;
#ಆದರೆ_ಅಂತ್ಯ

/*
*******************************************************************************
* Interrupt stack. The size of the interrupt stack is determined by the config
* parameter "configISR_STACK_SIZE" in FreeRTOSConfig.h
*******************************************************************************
*/
ಅಸ್ಥಿರ ರಾಶಿ_Type_t DRAM_ATTR __attribute__((aligned(16))) ಪೋರ್ಟ್_Int_ರಾಶಿ[ಪೋರ್ಟ್_ಅಂಕೆ_PROCESSORS][config_ISR_ರಾಶಿ_ಗಾತ್ರ];
/* One flag for each individual CPU. */
ಅಸ್ಥಿರ uint32_t ಪೋರ್ಟ್_switch_flag[ಪೋರ್ಟ್_ಅಂಕೆ_PROCESSORS];

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

ತಳ_Type_t x_ಪೋರ್ಟ್_Enter_Critical_Timeout(ಪೋರ್ಟ್_MUX_TYPE *ಬೀಗ_2, ತಳ_Type_t timeout)
{
#ಆದರೆ CONFIG_FREERTOS_ಪೋರ್ಟ್_THREAD_ಸುರಕ್ಷಿತ_CLAIM
    ಆದರೆ (unlikely(ಪೋರ್ಟ್_x_Thread_ಸುರಕ್ಷಿತ_Claimed)) {
        ಹಿಂತಿರುಗಿ pdPASS;
    }
#ಆದರೆ_ಅಂತ್ಯ
    /* Interrupts may already be disabled (if this function is called in nested
     * manner). However, there's no atomic operation that will allow us to check,
     * thus we have to disable interrupts again anyways.
     *
     * However, if this is call is NOT nested (i.e., the first call to enter a
     * critical section), we will save the previous interrupt level so that the
     * saved level can be restored on the last call to exit the critical.
     */
    ತಳ_Type_t x_Old_ಅಡಚಣೆ_Level = XTOS_ಗಣ_INTLEVEL(XCHAL_EXCM_LEVEL);
    /* Interrupts are masked (to XCHAL_EXCM_LEVEL, the same level the spinlock uses),
     * so the core id is stable and the spinlock does not need to mask again. Read the
     * core id register once and reuse it for the owner id and the nesting index. */
    uint32_t coreOwnerId = spinlock_owner_id();
    ತಳ_Type_t coreID = spinlock_core_id_from_owner_id(coreOwnerId);
    ಆದರೆ (!spinlock_acquire_impl(ಬೀಗ_2, timeout, coreOwnerId)) {
        //Timed out attempting to get spinlock. Restore previous interrupt level and return
        XTOS_RESTORE_JUST_INTLEVEL((ಪೂರ್ಣಾಂಕ) x_Old_ಅಡಚಣೆ_Level);
        ಹಿಂತಿರುಗಿ pdFAIL;
    }
    //Spinlock acquired. Increment the IDF critical nesting count.
    ತಳ_Type_t newNesting = ಪೋರ್ಟ್_ux_Critical_Nesting_IDF[coreID] + 1;
    ಪೋರ್ಟ್_ux_Critical_Nesting_IDF[coreID] = newNesting;
    //If this is the first entry to a critical section. Save the old interrupt level.
    ಆದರೆ ( newNesting == 1 ) {
        ಪೋರ್ಟ್_ux_Critical_Old_ಅಡಚಣೆ_State_IDF[coreID] = x_Old_ಅಡಚಣೆ_Level;
    }
    ಹಿಂತಿರುಗಿ pdPASS;

}

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Exit_Critical_IDF(ಪೋರ್ಟ್_MUX_TYPE *ಬೀಗ_2)
{
#ಆದರೆ CONFIG_FREERTOS_ಪೋರ್ಟ್_THREAD_ಸುರಕ್ಷಿತ_CLAIM
    ಆದರೆ (unlikely(ಪೋರ್ಟ್_x_Thread_ಸುರಕ್ಷಿತ_Claimed)) {
        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ
    /* This function may be called in a nested manner. Therefore, we only need
     * to re-enable interrupts if this is the last call to exit the critical. We
     * can use the nesting count to determine whether this is the last exit call.
     */
    /* Interrupts remain disabled for the whole critical section, so the core id is
     * stable and the spinlock does not need to mask again. Read the core id register
     * once and reuse it for the owner id and the nesting index. */
    uint32_t coreOwnerId = spinlock_owner_id();
    ತಳ_Type_t coreID = spinlock_core_id_from_owner_id(coreOwnerId);
    spinlock_release_impl(ಬೀಗ_2, coreOwnerId);
    ತಳ_Type_t nesting = ಪೋರ್ಟ್_ux_Critical_Nesting_IDF[coreID];

    /* Critical section nesting count must never be negative */
    configASSERT( nesting > 0 );

    ಆದರೆ (nesting > 0) {
        nesting--;
        ಪೋರ್ಟ್_ux_Critical_Nesting_IDF[coreID] = nesting;

        //This is the last exit call, restore the saved interrupt level
        ಆದರೆ ( nesting == 0 ) {
            XTOS_RESTORE_JUST_INTLEVEL((ಪೂರ್ಣಾಂಕ) ಪೋರ್ಟ್_ux_Critical_Old_ಅಡಚಣೆ_State_IDF[coreID]);
        }
    }
}

/*
In case any IDF libs called the port critical functions directly instead of through the macros.
Just inline call the IDF versions
*/
ಶೂನ್ಯ v_ಪೋರ್ಟ್_Enter_Critical(ಪೋರ್ಟ್_MUX_TYPE *ಬೀಗ_2)
{
    v_ಪೋರ್ಟ್_Enter_Critical_IDF(ಬೀಗ_2);
}

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Exit_Critical(ಪೋರ್ಟ್_MUX_TYPE *ಬೀಗ_2)
{
    v_ಪೋರ್ಟ್_Exit_Critical_IDF(ಬೀಗ_2);
}

// ----------------------- System --------------------------

#ವ್ಯಾಖ್ಯಾನಿಸು ರಾಶಿ_WATCH_ಬಿಂದು_ಸಂಖ್ಯೆ (SOC_CPU_WATCHPOINTS_ಅಂಕೆ - 1)

ಶೂನ್ಯ v_ಪೋರ್ಟ್_ಗಣ_ರಾಶಿ_Watchpoint( ಶೂನ್ಯ *px_ರಾಶಿ_ಆರಂಭ )
{
    //Set watchpoint 1 to watch the last 32 bytes of the stack.
    //Unfortunately, the Xtensa watchpoints can't set a watchpoint on a random [base - base+n] region because
    //the size works by masking off the lowest address bits. For that reason, we futz a bit and watch the lowest 32
    //bytes of the stack we can actually watch. In general, this can cause the watchpoint to be triggered at most
    //28 bytes early. The value 32 is chosen because it's larger than the stack canary, which in FreeRTOS is 20 bytes.
    //This way, we make sure we trigger before/when the stack canary is corrupted, not after.
    ಪೂರ್ಣಾಂಕ addr = (ಪೂರ್ಣಾಂಕ)px_ರಾಶಿ_ಆರಂಭ;
    addr = (addr + 31) & (~31);
    esp_cpu_ಗಣ_watchpoint(ರಾಶಿ_WATCH_ಬಿಂದು_ಸಂಖ್ಯೆ, (ಅಕ್ಷರ *)addr, 32, ESP_CPU_WATCHPOINT_STORE);
}



/* ---------------------------------------------- Port Implementations -------------------------------------------------
 * Implementations of Porting Interface functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

ತಳ_Type_t x_ಪೋರ್ಟ್_Check_If_ಒಳ_ISR(ಶೂನ್ಯ)
{
    //Disable interrupts so that reading port_interruptNesting is atomic
    ತಳ_Type_t ret;
    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ prev_int_level = ಪೋರ್ಟ್_DISABLE_INTERRUPTS();
    ret = (ಪೋರ್ಟ್_ಅಡಚಣೆ_Nesting[x_ಪೋರ್ಟ್_Get_Core_ID()] != 0) ? pdTRUE : pdFALSE;
    ಪೋರ್ಟ್_RESTORE_INTERRUPTS(prev_int_level);
    ಹಿಂತಿರುಗಿ ret;
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
                // We call EARLY log here as currently portCLEAN_UP_TCB() is called in a critical section
                ESP_EARLY_LOGE("FreeRTOS", "Fatal error: TLSP deletion callback at index %d overwritten with non-excutable pointer %p", x, pv_Thread_Local_Storage_ಸೂಚಕಗಳು_Del_Callback[ x ]);
                abort();
            }

            pv_Thread_Local_Storage_ಸೂಚಕಗಳು_Del_Callback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

#ಆದರೆ ( XCHAL_CP_ಅಂಕೆ > 0 && configUSE_CORE_AFFINITY == 1 && config_ಅಂಕೆ_CORES > 1 )
ಸ್ಥಿರ ಶೂನ್ಯ v_ಪೋರ್ಟ್_Clean_ಮೇಲೆ_Coproc_Area( ಶೂನ್ಯ *pxTCB )
{
    UBaseType_t uxCoprocArea;
    ತಳ_Type_t x_ಗುರಿ_Core_ID;

    /* Get pointer to the task's coprocessor save area from TCB->pxEndOfStack. See uxInitialiseStackCPSA() */
    uxCoprocArea = ( UBaseType_t ) ( ( ( StaticTask_t * ) pxTCB )->pxDummy8 );  /* Get TCB_t.pxEndOfStack */
    uxCoprocArea = ESP_ALIGN_ಕೆಳಕ್ಕೆ(uxCoprocArea - XT_CP_ಗಾತ್ರ, 16);

    /* Extract core ID from the affinity mask */
    x_ಗುರಿ_Core_ID = ( ( StaticTask_t * ) pxTCB )->uxDummy26;
    x_ಗುರಿ_Core_ID = ( ತಳ_Type_t ) __builtin_ffs( ( ಪೂರ್ಣಾಂಕ ) x_ಗುರಿ_Core_ID );
    ಪ್ರತಿಜ್ಞಾ( x_ಗುರಿ_Core_ID >= 1 ); // __builtin_ffs always returns first set index + 1
    x_ಗುರಿ_Core_ID -= 1;

    /* If task has live floating point registers somewhere, release them */
    ಶೂನ್ಯ _xt_coproc_release(ಅಸ್ಥಿರ ಶೂನ್ಯ *coproc_sa_ತಳ, ತಳ_Type_t x_ಗುರಿ_Core_ID);
    _xt_coproc_release( (ಶೂನ್ಯ *)uxCoprocArea, x_ಗುರಿ_Core_ID );
}
#ಆದರೆ_ಅಂತ್ಯ /* ( XCHAL_CP_NUM > 0 && configUSE_CORE_AFFINITY == 1 && configNUM_CORES > 1 ) */

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

    #ಆದರೆ ( XCHAL_CP_ಅಂಕೆ > 0 && configUSE_CORE_AFFINITY == 1 && config_ಅಂಕೆ_CORES > 1 )
        /* Cleanup coproc save area */
        v_ಪೋರ್ಟ್_Clean_ಮೇಲೆ_Coproc_Area( pxTCB );
    #ಆದರೆ_ಅಂತ್ಯ /* ( XCHAL_CP_NUM > 0 && configUSE_CORE_AFFINITY == 1 && configNUM_CORES > 1 ) */
}

/* ------------------------------------------------ FreeRTOS Portable --------------------------------------------------
 * - Provides implementation for functions required by FreeRTOS
 * - Declared in portable.h
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------- Scheduler Start/End -------------------

ಬಾಹ್ಯ ಶೂನ್ಯ _xt_coproc_init(ಶೂನ್ಯ);

ತಳ_Type_t x_ಪೋರ್ಟ್_ಆರಂಭ_Scheduler( ಶೂನ್ಯ )
{
    ಪೋರ್ಟ್_DISABLE_INTERRUPTS();
    // Interrupts are disabled at this point and stack contains PS with enabled interrupts when task context is restored

#ಆದರೆ XCHAL_CP_ಅಂಕೆ > 0
    /* Initialize co-processor management for tasks. Leave CPENABLE alone. */
    _xt_coproc_init();
#ಆದರೆ_ಅಂತ್ಯ

    /* Setup the hardware to generate the tick. */
    v_ಪೋರ್ಟ್_ಸಿದ್ಧತೆ_Timer();

        /* Initialize all kernel state tracking variables */
    ತಳ_Type_t coreID = x_ಪೋರ್ಟ್_Get_Core_ID();
    ಪೋರ್ಟ್_x_Scheduler_Running[coreID] = 1;
    ಪೋರ್ಟ್_ux_Core_Startup_ಮುಗಿದ[coreID] = 0;
#ಆದರೆ CONFIG_FREERTOS_ಪೋರ್ಟ್_THREAD_ಸುರಕ್ಷಿತ_CLAIM
    ಪೋರ್ಟ್_x_Thread_ಸುರಕ್ಷಿತ_Claimed = false;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ config_ಅಂಕೆ_CORES > 1
    // Workaround for non-thread safe multi-core OS startup (see IDF-4524)
    ಆದರೆ (x_ಪೋರ್ಟ್_Get_Core_ID() != 0) {
        prv_ಆರಂಭ_Scheduler_Other_Cores();
    }
#ಆದರೆ_ಅಂತ್ಯ // configNUM_CORES > 1

    // Windows contain references to the startup stack which will be reclaimed by the main task
    // Spill the windows to create a clean environment to ensure we do not carry over any such references
    // to invalid SPs which will cause problems if main_task does a windowoverflow to them
    xthal_ಕಿಟಕಿ_spill();

    // Cannot be directly called from C; never returns
    __asm__ ಅಸ್ಥಿರ ("call0    _frxt_dispatch\n");

    /* Should not get here. */
    ಹಿಂತಿರುಗಿ pdTRUE;
}

ಶೂನ್ಯ v_ಪೋರ್ಟ್_ಕೊನೆ_Scheduler( ಶೂನ್ಯ )
{
    ;
}

// ------------------------ Stack --------------------------

// User exception dispatcher when exiting
ಶೂನ್ಯ _xt_user_exit(ಶೂನ್ಯ);

#ಆದರೆ CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
// Wrapper to allow task functions to return (increases stack overhead by 16 bytes)
ಸ್ಥಿರ ಶೂನ್ಯ v_ಪೋರ್ಟ್_Task_Wrapper(TaskFunction_t px_ಸಂಕೇತ, ಶೂನ್ಯ *pvParameters)
{
    px_ಸಂಕೇತ(pvParameters);
    //FreeRTOS tasks should not return. Log the task name and abort.
    ಅಕ್ಷರ *pcTaskName = pcTaskGetName(ಖಾಲಿ);
    ESP_LOGE("FreeRTOS", "FreeRTOS Task \"%s\" should not return, Aborting now!", pcTaskName);
    abort();
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ XCHAL_CP_ಅಂಕೆ > 0
/**
 * @brief Allocate and initialize coprocessor save area on the stack
 *
 * This function allocates the coprocessor save area on the stack (sized XT_CP_SIZE) which includes...
 *  - Individual save areas for each coprocessor (size XT_CPx_SA, inclusive of each area's alignment)
 *  - Coprocessor context switching flags (e.g., XT_CPENABLE, XT_CPSTORED, XT_CP_CS_ST, XT_CP_ASA).
 *
 * The coprocessor save area is aligned to a 16-byte boundary.
 * The coprocessor context switching flags are then initialized
 *
 * @param[in] uxStackPointer Current stack pointer address
 * @return Stack pointer that points to allocated and initialized the coprocessor save area
 */
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ರಾಶಿ_CPSA(UBaseType_t ux_ರಾಶಿ_ಸೂಚಕ)
{
    /*
    HIGH ADDRESS
    |-------------------|      XT_CP_SIZE
    | CPn SA            |           ^
    | ...               |           |
    | CP0 SA            |           |
    | ----------------- |           |       ---- XCHAL_TOTAL_SA_ALIGN aligned
    |-------------------|           |   12 bytes
    | XT_CP_ASA         |           |       ^
    | XT_CP_CS_ST       |           |       |
    | XT_CPSTORED       |           |       |
    | XT_CPENABLE       |           |       |
    |-------------------| ---------------------- 16 byte aligned
    LOW ADDRESS
    */

    // Allocate overall coprocessor save area, aligned down to 16 byte boundary
    ux_ರಾಶಿ_ಸೂಚಕ = ESP_ALIGN_ಕೆಳಕ್ಕೆ(ux_ರಾಶಿ_ಸೂಚಕ - XT_CP_ಗಾತ್ರ, 16);
    // Initialize the coprocessor context switching flags.
    uint32_t *p = (uint32_t *)ux_ರಾಶಿ_ಸೂಚಕ;
    p[0] = 0;   // Clear XT_CPENABLE and XT_CPSTORED
    p[1] = 0;   // Clear XT_CP_CS_ST
    // XT_CP_ASA points to the aligned start of the individual CP save areas (i.e., start of CP0 SA)
    p[2] = (uint32_t)ESP_ALIGN_ಮೇಲೆ((uint32_t)ux_ರಾಶಿ_ಸೂಚಕ + 12, XCHAL_ಒಟ್ಟು_SA_ALIGN);
    ಹಿಂತಿರುಗಿ ux_ರಾಶಿ_ಸೂಚಕ;
}
#ಆದರೆ_ಅಂತ್ಯ /* XCHAL_CP_NUM > 0 */

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
    TLS layout at link-time, where 0xNNN is the offset that the linker calculates to a particular TLS variable.

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
    // Calculate the TLS area's size (rounded up to multiple of 16 bytes).
    ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ _tls_section_alignment;
    ಬಾಹ್ಯ ಅಕ್ಷರ thread_local_ದತ್ತಾಂಶ_ಆರಂಭ, thread_local_ದತ್ತಾಂಶ_ಕೊನೆ;
    ಬಾಹ್ಯ ಅಕ್ಷರ thread_local_bss_ಆರಂಭ, thread_local_bss_ಕೊನೆ;
    ಸ್ಥಿರಾಂಕ uint32_t tls_ದತ್ತಾಂಶ_ಗಾತ್ರ = (uint32_t)&thread_local_ದತ್ತಾಂಶ_ಕೊನೆ - (uint32_t)&thread_local_ದತ್ತಾಂಶ_ಆರಂಭ;
    ಸ್ಥಿರಾಂಕ uint32_t tls_bss_ಗಾತ್ರ = (uint32_t)&thread_local_bss_ಕೊನೆ - (uint32_t)&thread_local_bss_ಆರಂಭ;
    ಸ್ಥಿರಾಂಕ uint32_t tls_area_ಗಾತ್ರ = ESP_ALIGN_ಮೇಲೆ(tls_ದತ್ತಾಂಶ_ಗಾತ್ರ + tls_bss_ಗಾತ್ರ, 16);
    // TODO: check that TLS area fits the stack

    // Allocate space for the TLS area on the stack. The area must be allocated at a 16-byte aligned address
    ux_ರಾಶಿ_ಸೂಚಕ = ESP_ALIGN_ಕೆಳಕ್ಕೆ(ux_ರಾಶಿ_ಸೂಚಕ - (UBaseType_t)tls_area_ಗಾತ್ರ, 16);
    // Initialize the TLS data with the initialization values of each TLS variable
    ಸ್ಮರಣೆ_ನಕಲು((ಶೂನ್ಯ *)ux_ರಾಶಿ_ಸೂಚಕ, &thread_local_ದತ್ತಾಂಶ_ಆರಂಭ, tls_ದತ್ತಾಂಶ_ಗಾತ್ರ);
    // Initialize the TLS bss with zeroes
    memset((ಶೂನ್ಯ *)(ux_ರಾಶಿ_ಸೂಚಕ + tls_ದತ್ತಾಂಶ_ಗಾತ್ರ), 0, tls_bss_ಗಾತ್ರ);

    /*
    Calculate the THREADPTR register's initialization value based on the link-time offset and the TLS area allocated on
    the stack.

    HIGH ADDRESS
            |---------------------------|
            | .tdata (*)                |
          ^ | int example;              |
          | |                           |
          | | .tbss (*)                 |
          | |---------------------------| <- uxStackPointer (start of TLS area)
    0xNNN | |                           | ^
          | |                           | |
          |             ...               | (_thread_local_start - _flash_rodata_start) + align_up(TCB_SIZE, tls_section_alignment)
          | |                           | |
          | |                           | V
          V |                           | <- threadptr register's value

    LOW ADDRESS

    Note: Xtensa is slightly different compared to the RISC-V port as there is an implicit aligned TCB_SIZE added to
    the offset. (search for 'tpoff' in elf32-xtensa.c in BFD):
        - "offset = address - tls_section_vma + align_up(TCB_SIZE, tls_section_alignment)"
        - TCB_SIZE is hardcoded to 8
    */
    ಸ್ಥಿರಾಂಕ uint32_t tls_section_align = (uint32_t)&_tls_section_alignment;  // ALIGN value of .flash.tdata section
    #ವ್ಯಾಖ್ಯಾನಿಸು TCB_ಗಾತ್ರ 8
    ಸ್ಥಿರಾಂಕ uint32_t ತಳ = ESP_ALIGN_ಮೇಲೆ(TCB_ಗಾತ್ರ, tls_section_align);
    *ret_threadptr_reg_init = (uint32_t)ux_ರಾಶಿ_ಸೂಚಕ - ತಳ;

    ಹಿಂತಿರುಗಿ ux_ರಾಶಿ_ಸೂಚಕ;
}

/**
 * @brief Initialize the task's starting interrupt stack frame
 *
 * This function initializes the task's starting interrupt stack frame. The dispatcher will use this stack frame in a
 * context restore routine. Therefore, the starting stack frame must be initialized as if the task was interrupted right
 * before its first instruction is called.
 *
 * - The stack frame is allocated to a 16-byte aligned address
 * - The THREADPTR register is saved in the extra storage area of the stack frame. This is also initialized
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
    HIGH ADDRESS
    |---------------------------|       ^ XT_STK_FRMSZ
    |                           |       |
    | Stack Frame Extra Storage |       |
    |                           |       |
    | ------------------------- |       |   ^ XT_STK_EXTRA
    |                           |       |   |
    | Intr/Exc Stack Frame      |       |   |
    |                           |       V   V
    | ------------------------- | ---------------------- 16 byte aligned
    LOW ADDRESS
    */

    /*
    Allocate space for the task's starting interrupt stack frame.
    - The stack frame must be allocated to a 16-byte aligned address.
    - We use XT_STK_FRMSZ (instead of sizeof(XtExcFrame)) as it...
        - includes the size of the extra storage area
        - includes the size for a base save area before the stack frame
        - rounds up the total size to a multiple of 16
    */
    UBaseType_t ux_ರಾಶಿ_ಸೂಚಕ_ಹಿಂದಿನ = ux_ರಾಶಿ_ಸೂಚಕ;
    ux_ರಾಶಿ_ಸೂಚಕ = ESP_ALIGN_ಕೆಳಕ್ಕೆ(ux_ರಾಶಿ_ಸೂಚಕ - XT_STK_FRMSZ, 16);

    // Clear the entire interrupt stack frame
    memset((ಶೂನ್ಯ *)ux_ರಾಶಿ_ಸೂಚಕ, 0, (size_t)(ux_ರಾಶಿ_ಸೂಚಕ_ಹಿಂದಿನ - ux_ರಾಶಿ_ಸೂಚಕ));

    Xt_Exc_ಚೌಕಟ್ಟು *ಚೌಕಟ್ಟು = (Xt_Exc_ಚೌಕಟ್ಟು *)ux_ರಾಶಿ_ಸೂಚಕ;

    /*
    Initialize common registers
    */
    ಚೌಕಟ್ಟು->a0 = 0;                                          // Set the return address to 0 terminate GDB backtrace
    ಚೌಕಟ್ಟು->a1 = ux_ರಾಶಿ_ಸೂಚಕ + XT_STK_FRMSZ;              // Saved stack pointer should point to physical top of stack frame
    ಚೌಕಟ್ಟು->ನಿರ್ಗಮಿಸು = (UBaseType_t) _xt_user_exit;              // User exception exit dispatcher

    /*
    Initialize the task's entry point. This will differ depending on
    - Whether the task's entry point is the wrapper function or pxCode
    - Whether Windowed ABI is used (for windowed, we mimic the task entry point being call4'd )
    */
    #ಆದರೆ CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
        ಚೌಕಟ್ಟು->pc = (UBaseType_t) v_ಪೋರ್ಟ್_Task_Wrapper;         // Task entry point is the wrapper function
        #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __XTENSA_CALL0_ABI__
            ಚೌಕಟ್ಟು->a2 = (UBaseType_t) px_ಸಂಕೇತ;               // Wrapper function's argument 0 (which is the task function)
            ಚೌಕಟ್ಟು->a3 = (UBaseType_t) pvParameters;         // Wrapper function's argument 1 (which is the task function's argument)
        #ಇಲ್ಲದಿದ್ದರೆ // __XTENSA_CALL0_ABI__
            ಚೌಕಟ್ಟು->a6 = (UBaseType_t) px_ಸಂಕೇತ;               // Wrapper function's argument 0 (which is the task function), passed as if we call4'd
            ಚೌಕಟ್ಟು->a7 = (UBaseType_t) pvParameters;         // Wrapper function's argument 1 (which is the task function's argument), passed as if we call4'd
        #ಆದರೆ_ಅಂತ್ಯ // __XTENSA_CALL0_ABI__
    #ಇಲ್ಲದಿದ್ದರೆ
        ಚೌಕಟ್ಟು->pc = (UBaseType_t) px_ಸಂಕೇತ;                   // Task entry point is the provided task function
        #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __XTENSA_CALL0_ABI__
            ಚೌಕಟ್ಟು->a2 = (UBaseType_t) pvParameters;         // Task function's argument
        #ಇಲ್ಲದಿದ್ದರೆ // __XTENSA_CALL0_ABI__
            ಚೌಕಟ್ಟು->a6 = (UBaseType_t) pvParameters;         // Task function's argument, passed as if we call4'd
        #ಆದರೆ_ಅಂತ್ಯ // __XTENSA_CALL0_ABI__
    #ಆದರೆ_ಅಂತ್ಯ

    /*
    Set initial PS to int level 0, EXCM disabled ('rfe' will enable), user mode.
    For windowed ABI also set WOE and CALLINC (pretend task was 'call4'd)
    */
    #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __XTENSA_CALL0_ABI__
        ಚೌಕಟ್ಟು->ps = PS_UM | PS_EXCM;
    #ಇಲ್ಲದಿದ್ದರೆ // __XTENSA_CALL0_ABI__
        ಚೌಕಟ್ಟು->ps = PS_UM | PS_EXCM | PS_WOE | PS_CALLINC(1);
    #ಆದರೆ_ಅಂತ್ಯ // __XTENSA_CALL0_ABI__

    #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ XT_USE_SWPRI
        // Set the initial virtual priority mask value to all 1's.
        ಚೌಕಟ್ಟು->vpri = 0xFFFFFFFF;
    #ಆದರೆ_ಅಂತ್ಯ

    // Initialize the threadptr register in the extra save area of the stack frame
    uint32_t *threadptr_reg = (uint32_t *)(ux_ರಾಶಿ_ಸೂಚಕ + XT_STK_EXTRA);
    *threadptr_reg = threadptr_reg_init;

    ಹಿಂತಿರುಗಿ ux_ರಾಶಿ_ಸೂಚಕ;
}

#ಆದರೆ ( ಪೋರ್ಟ್_HAS_ರಾಶಿ_ಅತಿಪ್ರವಾಹ_CHECKING == 1 )
ರಾಶಿ_Type_t * px_ಪೋರ್ಟ್_Initialise_ರಾಶಿ( ರಾಶಿ_Type_t * px_ತುದಿ_ದ_ರಾಶಿ,
                                     ರಾಶಿ_Type_t * px_ಕೊನೆ_ದ_ರಾಶಿ,
                                     TaskFunction_t px_ಸಂಕೇತ,
                                     ಶೂನ್ಯ * pvParameters )
#ಇಲ್ಲದಿದ್ದರೆ
ರಾಶಿ_Type_t * px_ಪೋರ್ಟ್_Initialise_ರಾಶಿ( ರಾಶಿ_Type_t * px_ತುದಿ_ದ_ರಾಶಿ,
                                     TaskFunction_t px_ಸಂಕೇತ,
                                     ಶೂನ್ಯ * pvParameters )
#ಆದರೆ_ಅಂತ್ಯ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __clang_analyzer__
    // Teach clang-tidy that pxTopOfStack cannot be a pointer to const
    ಅಸ್ಥಿರ ರಾಶಿ_Type_t * px_ತಾತ್ಕಾಲಿಕ = px_ತುದಿ_ದ_ರಾಶಿ;
    px_ತುದಿ_ದ_ರಾಶಿ = px_ತಾತ್ಕಾಲಿಕ;
#ಆದರೆ_ಅಂತ್ಯ /*__clang_analyzer__ */
    /*
    HIGH ADDRESS
    |---------------------------| <- pxTopOfStack on entry
    | Coproc Save Area          | (CPSA MUST BE FIRST)
    | ------------------------- |
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

#ಆದರೆ XCHAL_CP_ಅಂಕೆ > 0
    // Initialize the coprocessor save area. THIS MUST BE THE FIRST AREA due to access from _frxt_task_coproc_state()
    ux_ರಾಶಿ_ಸೂಚಕ = ux_Initialise_ರಾಶಿ_CPSA(ux_ರಾಶಿ_ಸೂಚಕ);
    configASSERT((ux_ರಾಶಿ_ಸೂಚಕ & ಪೋರ್ಟ್_BYTE_ALIGNMENT_ಮುಖವಾಡ) == 0);
#ಆದರೆ_ಅಂತ್ಯ /* XCHAL_CP_NUM > 0 */

    // Initialize the GCC TLS area
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

#ಅನ್ತರ್ಭಾವ <stdlib.h>

#ಆದರೆ ( config_CHECK_FOR_ರಾಶಿ_ಅತಿಪ್ರವಾಹ > 0 )
ಶೂನ್ಯ  __attribute__((weak)) v_Application_ರಾಶಿ_ಅತಿಪ್ರವಾಹ_Hook( TaskHandle_t xTask, ಅಕ್ಷರ *pcTaskName )
{
#ವ್ಯಾಖ್ಯಾನಿಸು ERR_STR1 "***ERROR*** A stack overflow in task "
#ವ್ಯಾಖ್ಯಾನಿಸು ERR_STR2 " has been detected."
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    ಅಕ್ಷರ buf[ಗಾತ್ರ(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + ಗಾತ್ರ(ERR_STR2) + 1 /* null char */] = { 0 };

    ಅಕ್ಷರ *dest = buf;
    ಗಾಗಿ (size_t i = 0 ; i < ಗಾತ್ರ(str) / ಗಾತ್ರ(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
    esp_system_abort(buf);
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
#ಇಲ್ಲದಿದ್ದರೆ // Uses CCOUNT
    ಹಿಂತಿರುಗಿ (config_ಸತತ_ಸಮಯ_COUNTER_TYPE) xthal_get_ccount();
#ಆದರೆ_ಅಂತ್ಯ // CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
}

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
