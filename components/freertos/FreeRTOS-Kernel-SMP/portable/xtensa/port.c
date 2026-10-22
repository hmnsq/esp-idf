/*
 * SPDX-FileCopyrightText: 2022-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_compiler.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "task.h"   //For vApplicationStackOverflowHook
#ਅਨ੍ਤਰ੍ਭਾਵ "port_systick.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "portmacro.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "spinlock.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xt_instr_macros.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xtensa_context.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xtensa/corebits.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xtensa/config/core.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xtensa/config/core-isa.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xtensa/xtruntime.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_private/freertos_idf_additions_priv.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_private/esp_int_wdt.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_private/systimer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_private/periph_ctrl.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_attr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_heap_caps.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_log.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_cpu.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_rom_sys.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_task_wdt.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_heap_caps_init.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_freertos_hooks.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_intr_alloc.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_memory_utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_macros.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <xtensa/hal.h>             /* required for xthal_get_ccount() */
#ਜੇ CONFIG_FREERTOS_ਲੜੀਵਾਰ_ਸਮਾਂ_STATS_USING_ESP_TIMER
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_timer.h"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FREERTOS_SYSTICK_ਵਰਤਦਾ_SYSTIMER
#ਅਨ੍ਤਰ੍ਭਾਵ "soc/periph_defs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "soc/system_reg.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hal/systimer_hal.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hal/systimer_ll.h"
#ਜੇ_ਅੰਤ // CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER

_Static_assert(ਪੋਰਟ_BYTE_ALIGNMENT == 16, "portBYTE_ALIGNMENT must be set to 16");

/* ---------------------------------------------------- Variables ------------------------------------------------------
 * - Various variables used to maintain the FreeRTOS port's state. Used from both port.c and various .S files
 * - Constant offsets are used by assembly to jump to particular TCB members or a stack area (such as the CPSA). We use
 *   C constants instead of preprocessor macros due to assembly lacking "offsetof()".
 * ------------------------------------------------------------------------------------------------------------------ */

#ਜੇ XCHAL_CP_ਅੰਕ > 0
/* Offsets used to navigate to a task's CPSA on the stack */
ਅਚਲ DRAM_ATTR uint32_t ਵਿੱਥ_px_ਸਿਰਾ_ਦਾ_ਢੇਰ = offsetof(StaticTask_t, pxDummy8);
ਅਚਲ DRAM_ATTR uint32_t ਵਿੱਥ_cpsa = XT_CP_ਮਾਪ;  /* Offset to start of the CPSA area on the stack. See uxInitialiseStackCPSA(). */
#ਜੇ config_ਅੰਕ_CORES > 1
/* Offset to TCB_t.uxCoreAffinityMask member. Used to pin unpinned tasks that use the FPU. */
ਅਚਲ DRAM_ATTR uint32_t ਵਿੱਥ_ux_Core_Affinity_ਨਕਾਬ = offsetof(StaticTask_t, uxDummy26);
#ਜੇ configUSE_CORE_AFFINITY != 1
#ਗਲਤੀ "configUSE_CORE_AFFINITY must be 1 on multicore targets with coprocessor support"
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ /* configNUM_CORES > 1 */
#ਜੇ_ਅੰਤ /* XCHAL_CP_NUM > 0 */

ਅਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੋਰਟ_x_Scheduler_Running[ਪੋਰਟ_ਅੰਕ_PROCESSORS] = {0}; // Indicates whether scheduler is running on a per-core basis
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਪੋਰਟ_ਰੁਕਾਵਟ_Nesting[ਪੋਰਟ_ਅੰਕ_PROCESSORS] = {0};  // Interrupt nesting level. Increased/decreased in portasm.c, _frxt_int_enter/_frxt_int_exit
ਅਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੋਰਟ_ux_Core_Startup_ਪੂਰਾ[ਪੋਰਟ_ਅੰਕ_PROCESSORS] = {0};  // Indicates whether the core has completed its startup sequence
#ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 )
//FreeRTOS SMP Locks
ਪੋਰਟ_MUX_TYPE ਪੋਰਟ_x_Task_ਤਾਲਾ = ਪੋਰਟ_MUX_INITIALIZER_UNLOCKED;
ਪੋਰਟ_MUX_TYPE ਪੋਰਟ_x_ISRLock = ਪੋਰਟ_MUX_INITIALIZER_UNLOCKED;
#ਜੇ_ਅੰਤ /* configNUMBER_OF_CORES > 1 */

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

ਅਧਾਰ_Type_t x_ਪੋਰਟ_Interrupted_From_ISRContext(ਖਾਲੀ)
{
    ਵਾਪਸ (ਪੋਰਟ_ਰੁਕਾਵਟ_Nesting[x_ਪੋਰਟ_Get_Core_ID()] != 0);
}

// ------------------ Critical Sections --------------------

/*
Variables used by IDF critical sections only (SMP tracks critical nesting inside TCB now)
[refactor-todo] Figure out how IDF critical sections will be merged with SMP FreeRTOS critical sections
*/
ਅਧਾਰ_Type_t ਪੋਰਟ_ux_Critical_Nesting_IDF[ਪੋਰਟ_ਅੰਕ_PROCESSORS] = {0};
ਅਧਾਰ_Type_t ਪੋਰਟ_ux_Critical_Old_ਰੁਕਾਵਟ_State_IDF[ਪੋਰਟ_ਅੰਕ_PROCESSORS] = {0};
#ਜੇ CONFIG_FREERTOS_ਪੋਰਟ_THREAD_ਸੁਰੱਖਿਅਤ_CLAIM
ਅਸਥਿਰ bool ਪੋਰਟ_x_Thread_ਸੁਰੱਖਿਅਤ_Claimed = false;
#ਜੇ_ਅੰਤ

/*
*******************************************************************************
* Interrupt stack. The size of the interrupt stack is determined by the config
* parameter "configISR_STACK_SIZE" in FreeRTOSConfig.h
*******************************************************************************
*/
ਅਸਥਿਰ ਢੇਰ_Type_t DRAM_ATTR __attribute__((aligned(16))) ਪੋਰਟ_Int_ਢੇਰ[ਪੋਰਟ_ਅੰਕ_PROCESSORS][config_ISR_ਢੇਰ_ਮਾਪ];
/* One flag for each individual CPU. */
ਅਸਥਿਰ uint32_t ਪੋਰਟ_switch_flag[ਪੋਰਟ_ਅੰਕ_PROCESSORS];

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

ਅਧਾਰ_Type_t x_ਪੋਰਟ_Enter_Critical_Timeout(ਪੋਰਟ_MUX_TYPE *ਤਾਲਾ, ਅਧਾਰ_Type_t timeout)
{
#ਜੇ CONFIG_FREERTOS_ਪੋਰਟ_THREAD_ਸੁਰੱਖਿਅਤ_CLAIM
    ਜੇ (unlikely(ਪੋਰਟ_x_Thread_ਸੁਰੱਖਿਅਤ_Claimed)) {
        ਵਾਪਸ pdPASS;
    }
#ਜੇ_ਅੰਤ
    /* Interrupts may already be disabled (if this function is called in nested
     * manner). However, there's no atomic operation that will allow us to check,
     * thus we have to disable interrupts again anyways.
     *
     * However, if this is call is NOT nested (i.e., the first call to enter a
     * critical section), we will save the previous interrupt level so that the
     * saved level can be restored on the last call to exit the critical.
     */
    ਅਧਾਰ_Type_t x_Old_ਰੁਕਾਵਟ_Level = XTOS_ਸੈੱਟ_INTLEVEL(XCHAL_EXCM_LEVEL);
    /* Interrupts are masked (to XCHAL_EXCM_LEVEL, the same level the spinlock uses),
     * so the core id is stable and the spinlock does not need to mask again. Read the
     * core id register once and reuse it for the owner id and the nesting index. */
    uint32_t coreOwnerId = spinlock_owner_id();
    ਅਧਾਰ_Type_t coreID = spinlock_core_id_from_owner_id(coreOwnerId);
    ਜੇ (!spinlock_acquire_impl(ਤਾਲਾ, timeout, coreOwnerId)) {
        //Timed out attempting to get spinlock. Restore previous interrupt level and return
        XTOS_RESTORE_JUST_INTLEVEL((ਪੂਰਨ) x_Old_ਰੁਕਾਵਟ_Level);
        ਵਾਪਸ pdFAIL;
    }
    //Spinlock acquired. Increment the IDF critical nesting count.
    ਅਧਾਰ_Type_t newNesting = ਪੋਰਟ_ux_Critical_Nesting_IDF[coreID] + 1;
    ਪੋਰਟ_ux_Critical_Nesting_IDF[coreID] = newNesting;
    //If this is the first entry to a critical section. Save the old interrupt level.
    ਜੇ ( newNesting == 1 ) {
        ਪੋਰਟ_ux_Critical_Old_ਰੁਕਾਵਟ_State_IDF[coreID] = x_Old_ਰੁਕਾਵਟ_Level;
    }
    ਵਾਪਸ pdPASS;

}

ਖਾਲੀ v_ਪੋਰਟ_Exit_Critical_IDF(ਪੋਰਟ_MUX_TYPE *ਤਾਲਾ)
{
#ਜੇ CONFIG_FREERTOS_ਪੋਰਟ_THREAD_ਸੁਰੱਖਿਅਤ_CLAIM
    ਜੇ (unlikely(ਪੋਰਟ_x_Thread_ਸੁਰੱਖਿਅਤ_Claimed)) {
        ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ
    /* This function may be called in a nested manner. Therefore, we only need
     * to re-enable interrupts if this is the last call to exit the critical. We
     * can use the nesting count to determine whether this is the last exit call.
     */
    /* Interrupts remain disabled for the whole critical section, so the core id is
     * stable and the spinlock does not need to mask again. Read the core id register
     * once and reuse it for the owner id and the nesting index. */
    uint32_t coreOwnerId = spinlock_owner_id();
    ਅਧਾਰ_Type_t coreID = spinlock_core_id_from_owner_id(coreOwnerId);
    spinlock_release_impl(ਤਾਲਾ, coreOwnerId);
    ਅਧਾਰ_Type_t nesting = ਪੋਰਟ_ux_Critical_Nesting_IDF[coreID];

    /* Critical section nesting count must never be negative */
    configASSERT( nesting > 0 );

    ਜੇ (nesting > 0) {
        nesting--;
        ਪੋਰਟ_ux_Critical_Nesting_IDF[coreID] = nesting;

        //This is the last exit call, restore the saved interrupt level
        ਜੇ ( nesting == 0 ) {
            XTOS_RESTORE_JUST_INTLEVEL((ਪੂਰਨ) ਪੋਰਟ_ux_Critical_Old_ਰੁਕਾਵਟ_State_IDF[coreID]);
        }
    }
}

/*
In case any IDF libs called the port critical functions directly instead of through the macros.
Just inline call the IDF versions
*/
ਖਾਲੀ v_ਪੋਰਟ_Enter_Critical(ਪੋਰਟ_MUX_TYPE *ਤਾਲਾ)
{
    v_ਪੋਰਟ_Enter_Critical_IDF(ਤਾਲਾ);
}

ਖਾਲੀ v_ਪੋਰਟ_Exit_Critical(ਪੋਰਟ_MUX_TYPE *ਤਾਲਾ)
{
    v_ਪੋਰਟ_Exit_Critical_IDF(ਤਾਲਾ);
}

// ----------------------- System --------------------------

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਢੇਰ_WATCH_ਬਿੰਦੂ_ਸੰਖਿਆ (SOC_CPU_WATCHPOINTS_ਅੰਕ - 1)

ਖਾਲੀ v_ਪੋਰਟ_ਸੈੱਟ_ਢੇਰ_Watchpoint( ਖਾਲੀ *px_ਢੇਰ_ਸ਼ੁਰੂ )
{
    //Set watchpoint 1 to watch the last 32 bytes of the stack.
    //Unfortunately, the Xtensa watchpoints can't set a watchpoint on a random [base - base+n] region because
    //the size works by masking off the lowest address bits. For that reason, we futz a bit and watch the lowest 32
    //bytes of the stack we can actually watch. In general, this can cause the watchpoint to be triggered at most
    //28 bytes early. The value 32 is chosen because it's larger than the stack canary, which in FreeRTOS is 20 bytes.
    //This way, we make sure we trigger before/when the stack canary is corrupted, not after.
    ਪੂਰਨ addr = (ਪੂਰਨ)px_ਢੇਰ_ਸ਼ੁਰੂ;
    addr = (addr + 31) & (~31);
    esp_cpu_ਸੈੱਟ_watchpoint(ਢੇਰ_WATCH_ਬਿੰਦੂ_ਸੰਖਿਆ, (ਅੱਖਰ *)addr, 32, ESP_CPU_WATCHPOINT_STORE);
}



/* ---------------------------------------------- Port Implementations -------------------------------------------------
 * Implementations of Porting Interface functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

ਅਧਾਰ_Type_t x_ਪੋਰਟ_Check_If_ਵਿੱਚ_ISR(ਖਾਲੀ)
{
    //Disable interrupts so that reading port_interruptNesting is atomic
    ਅਧਾਰ_Type_t ret;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ prev_int_level = ਪੋਰਟ_DISABLE_INTERRUPTS();
    ret = (ਪੋਰਟ_ਰੁਕਾਵਟ_Nesting[x_ਪੋਰਟ_Get_Core_ID()] != 0) ? pdTRUE : pdFALSE;
    ਪੋਰਟ_RESTORE_INTERRUPTS(prev_int_level);
    ਵਾਪਸ ret;
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
                // We call EARLY log here as currently portCLEAN_UP_TCB() is called in a critical section
                ESP_EARLY_LOGE("FreeRTOS", "Fatal error: TLSP deletion callback at index %d overwritten with non-excutable pointer %p", x, pv_Thread_Local_Storage_ਸੂਚਕ_ਸਾਰੇ_Del_Callback[ x ]);
                abort();
            }

            pv_Thread_Local_Storage_ਸੂਚਕ_ਸਾਰੇ_Del_Callback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

#ਜੇ ( XCHAL_CP_ਅੰਕ > 0 && configUSE_CORE_AFFINITY == 1 && config_ਅੰਕ_CORES > 1 )
ਸਥਿਰ ਖਾਲੀ v_ਪੋਰਟ_Clean_ਉੱਪਰ_Coproc_Area( ਖਾਲੀ *pxTCB )
{
    UBaseType_t uxCoprocArea;
    ਅਧਾਰ_Type_t x_ਨਿਸ਼ਾਨਾ_Core_ID;

    /* Get pointer to the task's coprocessor save area from TCB->pxEndOfStack. See uxInitialiseStackCPSA() */
    uxCoprocArea = ( UBaseType_t ) ( ( ( StaticTask_t * ) pxTCB )->pxDummy8 );  /* Get TCB_t.pxEndOfStack */
    uxCoprocArea = ESP_ALIGN_ਥੱਲੇ(uxCoprocArea - XT_CP_ਮਾਪ, 16);

    /* Extract core ID from the affinity mask */
    x_ਨਿਸ਼ਾਨਾ_Core_ID = ( ( StaticTask_t * ) pxTCB )->uxDummy26;
    x_ਨਿਸ਼ਾਨਾ_Core_ID = ( ਅਧਾਰ_Type_t ) __builtin_ffs( ( ਪੂਰਨ ) x_ਨਿਸ਼ਾਨਾ_Core_ID );
    ਪ੍ਰਤਿਜ੍ਞਾ( x_ਨਿਸ਼ਾਨਾ_Core_ID >= 1 ); // __builtin_ffs always returns first set index + 1
    x_ਨਿਸ਼ਾਨਾ_Core_ID -= 1;

    /* If task has live floating point registers somewhere, release them */
    ਖਾਲੀ _xt_coproc_release(ਅਸਥਿਰ ਖਾਲੀ *coproc_sa_ਅਧਾਰ, ਅਧਾਰ_Type_t x_ਨਿਸ਼ਾਨਾ_Core_ID);
    _xt_coproc_release( (ਖਾਲੀ *)uxCoprocArea, x_ਨਿਸ਼ਾਨਾ_Core_ID );
}
#ਜੇ_ਅੰਤ /* ( XCHAL_CP_NUM > 0 && configUSE_CORE_AFFINITY == 1 && configNUM_CORES > 1 ) */

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

    #ਜੇ ( XCHAL_CP_ਅੰਕ > 0 && configUSE_CORE_AFFINITY == 1 && config_ਅੰਕ_CORES > 1 )
        /* Cleanup coproc save area */
        v_ਪੋਰਟ_Clean_ਉੱਪਰ_Coproc_Area( pxTCB );
    #ਜੇ_ਅੰਤ /* ( XCHAL_CP_NUM > 0 && configUSE_CORE_AFFINITY == 1 && configNUM_CORES > 1 ) */
}

/* ------------------------------------------------ FreeRTOS Portable --------------------------------------------------
 * - Provides implementation for functions required by FreeRTOS
 * - Declared in portable.h
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------- Scheduler Start/End -------------------

ਬਾਹਰੀ ਖਾਲੀ _xt_coproc_init(ਖਾਲੀ);

ਅਧਾਰ_Type_t x_ਪੋਰਟ_ਸ਼ੁਰੂ_Scheduler( ਖਾਲੀ )
{
    ਪੋਰਟ_DISABLE_INTERRUPTS();
    // Interrupts are disabled at this point and stack contains PS with enabled interrupts when task context is restored

#ਜੇ XCHAL_CP_ਅੰਕ > 0
    /* Initialize co-processor management for tasks. Leave CPENABLE alone. */
    _xt_coproc_init();
#ਜੇ_ਅੰਤ

    /* Setup the hardware to generate the tick. */
    v_ਪੋਰਟ_ਤਿਆਰੀ_Timer();

        /* Initialize all kernel state tracking variables */
    ਅਧਾਰ_Type_t coreID = x_ਪੋਰਟ_Get_Core_ID();
    ਪੋਰਟ_x_Scheduler_Running[coreID] = 1;
    ਪੋਰਟ_ux_Core_Startup_ਪੂਰਾ[coreID] = 0;
#ਜੇ CONFIG_FREERTOS_ਪੋਰਟ_THREAD_ਸੁਰੱਖਿਅਤ_CLAIM
    ਪੋਰਟ_x_Thread_ਸੁਰੱਖਿਅਤ_Claimed = false;
#ਜੇ_ਅੰਤ

#ਜੇ config_ਅੰਕ_CORES > 1
    // Workaround for non-thread safe multi-core OS startup (see IDF-4524)
    ਜੇ (x_ਪੋਰਟ_Get_Core_ID() != 0) {
        prv_ਸ਼ੁਰੂ_Scheduler_Other_Cores();
    }
#ਜੇ_ਅੰਤ // configNUM_CORES > 1

    // Windows contain references to the startup stack which will be reclaimed by the main task
    // Spill the windows to create a clean environment to ensure we do not carry over any such references
    // to invalid SPs which will cause problems if main_task does a windowoverflow to them
    xthal_ਖਿੜਕੀ_spill();

    // Cannot be directly called from C; never returns
    __asm__ ਅਸਥਿਰ ("call0    _frxt_dispatch\n");

    /* Should not get here. */
    ਵਾਪਸ pdTRUE;
}

ਖਾਲੀ v_ਪੋਰਟ_ਸਿਰਾ_Scheduler( ਖਾਲੀ )
{
    ;
}

// ------------------------ Stack --------------------------

// User exception dispatcher when exiting
ਖਾਲੀ _xt_user_exit(ਖਾਲੀ);

#ਜੇ CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
// Wrapper to allow task functions to return (increases stack overhead by 16 bytes)
ਸਥਿਰ ਖਾਲੀ v_ਪੋਰਟ_Task_Wrapper(TaskFunction_t px_ਕੋਡ, ਖਾਲੀ *pvParameters)
{
    px_ਕੋਡ(pvParameters);
    //FreeRTOS tasks should not return. Log the task name and abort.
    ਅੱਖਰ *pcTaskName = pcTaskGetName(ਨਿਰਾ);
    ESP_LOGE("FreeRTOS", "FreeRTOS Task \"%s\" should not return, Aborting now!", pcTaskName);
    abort();
}
#ਜੇ_ਅੰਤ

#ਜੇ XCHAL_CP_ਅੰਕ > 0
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ਢੇਰ_CPSA(UBaseType_t ux_ਢੇਰ_ਸੂਚਕ)
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
    ux_ਢੇਰ_ਸੂਚਕ = ESP_ALIGN_ਥੱਲੇ(ux_ਢੇਰ_ਸੂਚਕ - XT_CP_ਮਾਪ, 16);
    // Initialize the coprocessor context switching flags.
    uint32_t *p = (uint32_t *)ux_ਢੇਰ_ਸੂਚਕ;
    p[0] = 0;   // Clear XT_CPENABLE and XT_CPSTORED
    p[1] = 0;   // Clear XT_CP_CS_ST
    // XT_CP_ASA points to the aligned start of the individual CP save areas (i.e., start of CP0 SA)
    p[2] = (uint32_t)ESP_ALIGN_ਉੱਪਰ((uint32_t)ux_ਢੇਰ_ਸੂਚਕ + 12, XCHAL_ਕੁੱਲ_SA_ALIGN);
    ਵਾਪਸ ux_ਢੇਰ_ਸੂਚਕ;
}
#ਜੇ_ਅੰਤ /* XCHAL_CP_NUM > 0 */

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
    ਬਾਹਰੀ ਪੂਰਨ _tls_section_alignment;
    ਬਾਹਰੀ ਅੱਖਰ thread_local_ਅੰਕੜੇ_ਸ਼ੁਰੂ, thread_local_ਅੰਕੜੇ_ਸਿਰਾ;
    ਬਾਹਰੀ ਅੱਖਰ thread_local_bss_ਸ਼ੁਰੂ, thread_local_bss_ਸਿਰਾ;
    ਅਚਲ uint32_t tls_ਅੰਕੜੇ_ਮਾਪ = (uint32_t)&thread_local_ਅੰਕੜੇ_ਸਿਰਾ - (uint32_t)&thread_local_ਅੰਕੜੇ_ਸ਼ੁਰੂ;
    ਅਚਲ uint32_t tls_bss_ਮਾਪ = (uint32_t)&thread_local_bss_ਸਿਰਾ - (uint32_t)&thread_local_bss_ਸ਼ੁਰੂ;
    ਅਚਲ uint32_t tls_area_ਮਾਪ = ESP_ALIGN_ਉੱਪਰ(tls_ਅੰਕੜੇ_ਮਾਪ + tls_bss_ਮਾਪ, 16);
    // TODO: check that TLS area fits the stack

    // Allocate space for the TLS area on the stack. The area must be allocated at a 16-byte aligned address
    ux_ਢੇਰ_ਸੂਚਕ = ESP_ALIGN_ਥੱਲੇ(ux_ਢੇਰ_ਸੂਚਕ - (UBaseType_t)tls_area_ਮਾਪ, 16);
    // Initialize the TLS data with the initialization values of each TLS variable
    ਸਮ੍ਰਿਤੀ_ਨਕਲ((ਖਾਲੀ *)ux_ਢੇਰ_ਸੂਚਕ, &thread_local_ਅੰਕੜੇ_ਸ਼ੁਰੂ, tls_ਅੰਕੜੇ_ਮਾਪ);
    // Initialize the TLS bss with zeroes
    memset((ਖਾਲੀ *)(ux_ਢੇਰ_ਸੂਚਕ + tls_ਅੰਕੜੇ_ਮਾਪ), 0, tls_bss_ਮਾਪ);

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
    ਅਚਲ uint32_t tls_section_align = (uint32_t)&_tls_section_alignment;  // ALIGN value of .flash.tdata section
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TCB_ਮਾਪ 8
    ਅਚਲ uint32_t ਅਧਾਰ = ESP_ALIGN_ਉੱਪਰ(TCB_ਮਾਪ, tls_section_align);
    *ret_threadptr_reg_init = (uint32_t)ux_ਢੇਰ_ਸੂਚਕ - ਅਧਾਰ;

    ਵਾਪਸ ux_ਢੇਰ_ਸੂਚਕ;
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ਢੇਰ_ਚੌਖਟਾ(UBaseType_t ux_ਢੇਰ_ਸੂਚਕ, TaskFunction_t px_ਕੋਡ, ਖਾਲੀ *pvParameters, uint32_t threadptr_reg_init)
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
    UBaseType_t ux_ਢੇਰ_ਸੂਚਕ_ਪਿਛਲਾ = ux_ਢੇਰ_ਸੂਚਕ;
    ux_ਢੇਰ_ਸੂਚਕ = ESP_ALIGN_ਥੱਲੇ(ux_ਢੇਰ_ਸੂਚਕ - XT_STK_FRMSZ, 16);

    // Clear the entire interrupt stack frame
    memset((ਖਾਲੀ *)ux_ਢੇਰ_ਸੂਚਕ, 0, (size_t)(ux_ਢੇਰ_ਸੂਚਕ_ਪਿਛਲਾ - ux_ਢੇਰ_ਸੂਚਕ));

    Xt_Exc_ਚੌਖਟਾ *ਚੌਖਟਾ = (Xt_Exc_ਚੌਖਟਾ *)ux_ਢੇਰ_ਸੂਚਕ;

    /*
    Initialize common registers
    */
    ਚੌਖਟਾ->a0 = 0;                                          // Set the return address to 0 terminate GDB backtrace
    ਚੌਖਟਾ->a1 = ux_ਢੇਰ_ਸੂਚਕ + XT_STK_FRMSZ;              // Saved stack pointer should point to physical top of stack frame
    ਚੌਖਟਾ->ਬਾਹਰ_ਨਿਕਲੋ = (UBaseType_t) _xt_user_exit;              // User exception exit dispatcher

    /*
    Initialize the task's entry point. This will differ depending on
    - Whether the task's entry point is the wrapper function or pxCode
    - Whether Windowed ABI is used (for windowed, we mimic the task entry point being call4'd )
    */
    #ਜੇ CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
        ਚੌਖਟਾ->pc = (UBaseType_t) v_ਪੋਰਟ_Task_Wrapper;         // Task entry point is the wrapper function
        #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __XTENSA_CALL0_ABI__
            ਚੌਖਟਾ->a2 = (UBaseType_t) px_ਕੋਡ;               // Wrapper function's argument 0 (which is the task function)
            ਚੌਖਟਾ->a3 = (UBaseType_t) pvParameters;         // Wrapper function's argument 1 (which is the task function's argument)
        #ਨਹੀਂਤਾਂ // __XTENSA_CALL0_ABI__
            ਚੌਖਟਾ->a6 = (UBaseType_t) px_ਕੋਡ;               // Wrapper function's argument 0 (which is the task function), passed as if we call4'd
            ਚੌਖਟਾ->a7 = (UBaseType_t) pvParameters;         // Wrapper function's argument 1 (which is the task function's argument), passed as if we call4'd
        #ਜੇ_ਅੰਤ // __XTENSA_CALL0_ABI__
    #ਨਹੀਂਤਾਂ
        ਚੌਖਟਾ->pc = (UBaseType_t) px_ਕੋਡ;                   // Task entry point is the provided task function
        #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __XTENSA_CALL0_ABI__
            ਚੌਖਟਾ->a2 = (UBaseType_t) pvParameters;         // Task function's argument
        #ਨਹੀਂਤਾਂ // __XTENSA_CALL0_ABI__
            ਚੌਖਟਾ->a6 = (UBaseType_t) pvParameters;         // Task function's argument, passed as if we call4'd
        #ਜੇ_ਅੰਤ // __XTENSA_CALL0_ABI__
    #ਜੇ_ਅੰਤ

    /*
    Set initial PS to int level 0, EXCM disabled ('rfe' will enable), user mode.
    For windowed ABI also set WOE and CALLINC (pretend task was 'call4'd)
    */
    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __XTENSA_CALL0_ABI__
        ਚੌਖਟਾ->ps = PS_UM | PS_EXCM;
    #ਨਹੀਂਤਾਂ // __XTENSA_CALL0_ABI__
        ਚੌਖਟਾ->ps = PS_UM | PS_EXCM | PS_WOE | PS_CALLINC(1);
    #ਜੇ_ਅੰਤ // __XTENSA_CALL0_ABI__

    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ XT_USE_SWPRI
        // Set the initial virtual priority mask value to all 1's.
        ਚੌਖਟਾ->vpri = 0xFFFFFFFF;
    #ਜੇ_ਅੰਤ

    // Initialize the threadptr register in the extra save area of the stack frame
    uint32_t *threadptr_reg = (uint32_t *)(ux_ਢੇਰ_ਸੂਚਕ + XT_STK_EXTRA);
    *threadptr_reg = threadptr_reg_init;

    ਵਾਪਸ ux_ਢੇਰ_ਸੂਚਕ;
}

#ਜੇ ( ਪੋਰਟ_HAS_ਢੇਰ_ਉਛਾਲ_CHECKING == 1 )
ਢੇਰ_Type_t * px_ਪੋਰਟ_Initialise_ਢੇਰ( ਢੇਰ_Type_t * px_ਸਿਖਰ_ਦਾ_ਢੇਰ,
                                     ਢੇਰ_Type_t * px_ਸਿਰਾ_ਦਾ_ਢੇਰ,
                                     TaskFunction_t px_ਕੋਡ,
                                     ਖਾਲੀ * pvParameters )
#ਨਹੀਂਤਾਂ
ਢੇਰ_Type_t * px_ਪੋਰਟ_Initialise_ਢੇਰ( ਢੇਰ_Type_t * px_ਸਿਖਰ_ਦਾ_ਢੇਰ,
                                     TaskFunction_t px_ਕੋਡ,
                                     ਖਾਲੀ * pvParameters )
#ਜੇ_ਅੰਤ
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __clang_analyzer__
    // Teach clang-tidy that pxTopOfStack cannot be a pointer to const
    ਅਸਥਿਰ ਢੇਰ_Type_t * px_ਆਰਜ਼ੀ = px_ਸਿਖਰ_ਦਾ_ਢੇਰ;
    px_ਸਿਖਰ_ਦਾ_ਢੇਰ = px_ਆਰਜ਼ੀ;
#ਜੇ_ਅੰਤ /*__clang_analyzer__ */
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

    UBaseType_t ux_ਢੇਰ_ਸੂਚਕ = (UBaseType_t)px_ਸਿਖਰ_ਦਾ_ਢੇਰ;
    configASSERT((ux_ਢੇਰ_ਸੂਚਕ & ਪੋਰਟ_BYTE_ALIGNMENT_ਨਕਾਬ) == 0);

#ਜੇ XCHAL_CP_ਅੰਕ > 0
    // Initialize the coprocessor save area. THIS MUST BE THE FIRST AREA due to access from _frxt_task_coproc_state()
    ux_ਢੇਰ_ਸੂਚਕ = ux_Initialise_ਢੇਰ_CPSA(ux_ਢੇਰ_ਸੂਚਕ);
    configASSERT((ux_ਢੇਰ_ਸੂਚਕ & ਪੋਰਟ_BYTE_ALIGNMENT_ਨਕਾਬ) == 0);
#ਜੇ_ਅੰਤ /* XCHAL_CP_NUM > 0 */

    // Initialize the GCC TLS area
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

#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>

#ਜੇ ( config_CHECK_FOR_ਢੇਰ_ਉਛਾਲ > 0 )
ਖਾਲੀ  __attribute__((weak)) v_Application_ਢੇਰ_ਉਛਾਲ_Hook( TaskHandle_t xTask, ਅੱਖਰ *pcTaskName )
{
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ERR_STR1 "***ERROR*** A stack overflow in task "
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ERR_STR2 " has been detected."
    ਅਚਲ ਅੱਖਰ *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    ਅੱਖਰ buf[ਆਕਾਰ(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + ਆਕਾਰ(ERR_STR2) + 1 /* null char */] = { 0 };

    ਅੱਖਰ *dest = buf;
    ਲਈ (size_t i = 0 ; i < ਆਕਾਰ(str) / ਆਕਾਰ(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
    esp_system_abort(buf);
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
#ਨਹੀਂਤਾਂ // Uses CCOUNT
    ਵਾਪਸ (config_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_TYPE) xthal_get_ccount();
#ਜੇ_ਅੰਤ // CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
}

#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
