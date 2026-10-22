/*
 * SPDX-FileCopyrightText: 2022-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ <stdbool.h>
#అన్తర్భావ <stdint.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ "esp_compiler.h"
#అన్తర్భావ "FreeRTOS.h"
#అన్తర్భావ "task.h"   //For vApplicationStackOverflowHook
#అన్తర్భావ "port_systick.h"
#అన్తర్భావ "portmacro.h"
#అన్తర్భావ "spinlock.h"
#అన్తర్భావ "xt_instr_macros.h"
#అన్తర్భావ "xtensa_context.h"
#అన్తర్భావ "xtensa/corebits.h"
#అన్తర్భావ "xtensa/config/core.h"
#అన్తర్భావ "xtensa/config/core-isa.h"
#అన్తర్భావ "xtensa/xtruntime.h"
#అన్తర్భావ "esp_private/freertos_idf_additions_priv.h"
#అన్తర్భావ "esp_private/esp_int_wdt.h"
#అన్తర్భావ "esp_private/systimer.h"
#అన్తర్భావ "esp_private/periph_ctrl.h"
#అన్తర్భావ "esp_attr.h"
#అన్తర్భావ "esp_heap_caps.h"
#అన్తర్భావ "esp_system.h"
#అన్తర్భావ "esp_task.h"
#అన్తర్భావ "esp_log.h"
#అన్తర్భావ "esp_cpu.h"
#అన్తర్భావ "esp_rom_sys.h"
#అన్తర్భావ "esp_task_wdt.h"
#అన్తర్భావ "esp_heap_caps_init.h"
#అన్తర్భావ "esp_freertos_hooks.h"
#అన్తర్భావ "esp_intr_alloc.h"
#అన్తర్భావ "esp_memory_utils.h"
#అన్తర్భావ "esp_macros.h"
#అన్తర్భావ <xtensa/hal.h>             /* required for xthal_get_ccount() */
#ఒకవేళ CONFIG_FREERTOS_నిరంతరం_సమయం_STATS_USING_ESP_TIMER
#అన్తర్భావ "esp_timer.h"
#అయితే_ముగింపు
#నిర్వచితమైతే CONFIG_FREERTOS_SYSTICK_ఉపయోగిస్తుంది_SYSTIMER
#అన్తర్భావ "soc/periph_defs.h"
#అన్తర్భావ "soc/system_reg.h"
#అన్తర్భావ "hal/systimer_hal.h"
#అన్తర్భావ "hal/systimer_ll.h"
#అయితే_ముగింపు // CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER

_Static_assert(పోర్టు_BYTE_ALIGNMENT == 16, "portBYTE_ALIGNMENT must be set to 16");

/* ---------------------------------------------------- Variables ------------------------------------------------------
 * - Various variables used to maintain the FreeRTOS port's state. Used from both port.c and various .S files
 * - Constant offsets are used by assembly to jump to particular TCB members or a stack area (such as the CPSA). We use
 *   C constants instead of preprocessor macros due to assembly lacking "offsetof()".
 * ------------------------------------------------------------------------------------------------------------------ */

#ఒకవేళ XCHAL_CP_అంకె > 0
/* Offsets used to navigate to a task's CPSA on the stack */
స్థిరరాశి DRAM_ATTR uint32_t స్థానభ్రంశం_px_చివర_యొక్క_దొంతర = offsetof(StaticTask_t, pxDummy8);
స్థిరరాశి DRAM_ATTR uint32_t స్థానభ్రంశం_cpsa = XT_CP_కొలత;  /* Offset to start of the CPSA area on the stack. See uxInitialiseStackCPSA(). */
#ఒకవేళ config_అంకె_CORES > 1
/* Offset to TCB_t.uxCoreAffinityMask member. Used to pin unpinned tasks that use the FPU. */
స్థిరరాశి DRAM_ATTR uint32_t స్థానభ్రంశం_ux_Core_Affinity_ముసుగు = offsetof(StaticTask_t, uxDummy26);
#ఒకవేళ configUSE_CORE_AFFINITY != 1
#దోషం "configUSE_CORE_AFFINITY must be 1 on multicore targets with coprocessor support"
#అయితే_ముగింపు
#అయితే_ముగింపు /* configNUM_CORES > 1 */
#అయితే_ముగింపు /* XCHAL_CP_NUM > 0 */

అస్థిర గుర్తులేని పోర్టు_x_Scheduler_Running[పోర్టు_అంకె_PROCESSORS] = {0}; // Indicates whether scheduler is running on a per-core basis
గుర్తులేని పూర్ణాంకం పోర్టు_అంతరాయం_Nesting[పోర్టు_అంకె_PROCESSORS] = {0};  // Interrupt nesting level. Increased/decreased in portasm.c, _frxt_int_enter/_frxt_int_exit
అస్థిర గుర్తులేని పోర్టు_ux_Core_Startup_పూర్తి[పోర్టు_అంకె_PROCESSORS] = {0};  // Indicates whether the core has completed its startup sequence
#ఒకవేళ ( config_సంఖ్య_యొక్క_CORES > 1 )
//FreeRTOS SMP Locks
పోర్టు_MUX_TYPE పోర్టు_x_Task_తాళం = పోర్టు_MUX_INITIALIZER_UNLOCKED;
పోర్టు_MUX_TYPE పోర్టు_x_ISRLock = పోర్టు_MUX_INITIALIZER_UNLOCKED;
#అయితే_ముగింపు /* configNUMBER_OF_CORES > 1 */

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

పునాది_Type_t x_పోర్టు_Interrupted_From_ISRContext(శూన్య)
{
    తిరిగి (పోర్టు_అంతరాయం_Nesting[x_పోర్టు_Get_Core_ID()] != 0);
}

// ------------------ Critical Sections --------------------

/*
Variables used by IDF critical sections only (SMP tracks critical nesting inside TCB now)
[refactor-todo] Figure out how IDF critical sections will be merged with SMP FreeRTOS critical sections
*/
పునాది_Type_t పోర్టు_ux_Critical_Nesting_IDF[పోర్టు_అంకె_PROCESSORS] = {0};
పునాది_Type_t పోర్టు_ux_Critical_Old_అంతరాయం_State_IDF[పోర్టు_అంకె_PROCESSORS] = {0};
#ఒకవేళ CONFIG_FREERTOS_పోర్టు_THREAD_సురక్షిత_CLAIM
అస్థిర bool పోర్టు_x_Thread_సురక్షిత_Claimed = false;
#అయితే_ముగింపు

/*
*******************************************************************************
* Interrupt stack. The size of the interrupt stack is determined by the config
* parameter "configISR_STACK_SIZE" in FreeRTOSConfig.h
*******************************************************************************
*/
అస్థిర దొంతర_Type_t DRAM_ATTR __attribute__((aligned(16))) పోర్టు_Int_దొంతర[పోర్టు_అంకె_PROCESSORS][config_ISR_దొంతర_కొలత];
/* One flag for each individual CPU. */
అస్థిర uint32_t పోర్టు_switch_flag[పోర్టు_అంకె_PROCESSORS];

#ఒకవేళ CONFIG_FREERTOS_పోర్టు_THREAD_సురక్షిత_CLAIM
శూన్య x_పోర్టు_Thread_సురక్షిత_Claim(శూన్య)
{
    configASSERT(!x_పోర్టు_Can_Yield());
    configASSERT(!పోర్టు_x_Thread_సురక్షిత_Claimed);
    పోర్టు_x_Thread_సురక్షిత_Claimed = true;
}

శూన్య x_పోర్టు_Thread_సురక్షిత_Disclaim(శూన్య)
{
    configASSERT(!x_పోర్టు_Can_Yield());
    configASSERT(పోర్టు_x_Thread_సురక్షిత_Claimed);
    పోర్టు_x_Thread_సురక్షిత_Claimed = false;
}
#అయితే_ముగింపు /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

పునాది_Type_t x_పోర్టు_Enter_Critical_Timeout(పోర్టు_MUX_TYPE *తాళం_2, పునాది_Type_t timeout)
{
#ఒకవేళ CONFIG_FREERTOS_పోర్టు_THREAD_సురక్షిత_CLAIM
    ఒకవేళ (unlikely(పోర్టు_x_Thread_సురక్షిత_Claimed)) {
        తిరిగి pdPASS;
    }
#అయితే_ముగింపు
    /* Interrupts may already be disabled (if this function is called in nested
     * manner). However, there's no atomic operation that will allow us to check,
     * thus we have to disable interrupts again anyways.
     *
     * However, if this is call is NOT nested (i.e., the first call to enter a
     * critical section), we will save the previous interrupt level so that the
     * saved level can be restored on the last call to exit the critical.
     */
    పునాది_Type_t x_Old_అంతరాయం_Level = XTOS_సమితి_INTLEVEL(XCHAL_EXCM_LEVEL);
    /* Interrupts are masked (to XCHAL_EXCM_LEVEL, the same level the spinlock uses),
     * so the core id is stable and the spinlock does not need to mask again. Read the
     * core id register once and reuse it for the owner id and the nesting index. */
    uint32_t coreOwnerId = spinlock_owner_id();
    పునాది_Type_t coreID = spinlock_core_id_from_owner_id(coreOwnerId);
    ఒకవేళ (!spinlock_acquire_impl(తాళం_2, timeout, coreOwnerId)) {
        //Timed out attempting to get spinlock. Restore previous interrupt level and return
        XTOS_RESTORE_JUST_INTLEVEL((పూర్ణాంకం) x_Old_అంతరాయం_Level);
        తిరిగి pdFAIL;
    }
    //Spinlock acquired. Increment the IDF critical nesting count.
    పునాది_Type_t newNesting = పోర్టు_ux_Critical_Nesting_IDF[coreID] + 1;
    పోర్టు_ux_Critical_Nesting_IDF[coreID] = newNesting;
    //If this is the first entry to a critical section. Save the old interrupt level.
    ఒకవేళ ( newNesting == 1 ) {
        పోర్టు_ux_Critical_Old_అంతరాయం_State_IDF[coreID] = x_Old_అంతరాయం_Level;
    }
    తిరిగి pdPASS;

}

శూన్య v_పోర్టు_Exit_Critical_IDF(పోర్టు_MUX_TYPE *తాళం_2)
{
#ఒకవేళ CONFIG_FREERTOS_పోర్టు_THREAD_సురక్షిత_CLAIM
    ఒకవేళ (unlikely(పోర్టు_x_Thread_సురక్షిత_Claimed)) {
        తిరిగి;
    }
#అయితే_ముగింపు
    /* This function may be called in a nested manner. Therefore, we only need
     * to re-enable interrupts if this is the last call to exit the critical. We
     * can use the nesting count to determine whether this is the last exit call.
     */
    /* Interrupts remain disabled for the whole critical section, so the core id is
     * stable and the spinlock does not need to mask again. Read the core id register
     * once and reuse it for the owner id and the nesting index. */
    uint32_t coreOwnerId = spinlock_owner_id();
    పునాది_Type_t coreID = spinlock_core_id_from_owner_id(coreOwnerId);
    spinlock_release_impl(తాళం_2, coreOwnerId);
    పునాది_Type_t nesting = పోర్టు_ux_Critical_Nesting_IDF[coreID];

    /* Critical section nesting count must never be negative */
    configASSERT( nesting > 0 );

    ఒకవేళ (nesting > 0) {
        nesting--;
        పోర్టు_ux_Critical_Nesting_IDF[coreID] = nesting;

        //This is the last exit call, restore the saved interrupt level
        ఒకవేళ ( nesting == 0 ) {
            XTOS_RESTORE_JUST_INTLEVEL((పూర్ణాంకం) పోర్టు_ux_Critical_Old_అంతరాయం_State_IDF[coreID]);
        }
    }
}

/*
In case any IDF libs called the port critical functions directly instead of through the macros.
Just inline call the IDF versions
*/
శూన్య v_పోర్టు_Enter_Critical(పోర్టు_MUX_TYPE *తాళం_2)
{
    v_పోర్టు_Enter_Critical_IDF(తాళం_2);
}

శూన్య v_పోర్టు_Exit_Critical(పోర్టు_MUX_TYPE *తాళం_2)
{
    v_పోర్టు_Exit_Critical_IDF(తాళం_2);
}

// ----------------------- System --------------------------

#నిర్వచించు దొంతర_WATCH_బిందువు_సంఖ్య (SOC_CPU_WATCHPOINTS_అంకె - 1)

శూన్య v_పోర్టు_సమితి_దొంతర_Watchpoint( శూన్య *px_దొంతర_ప్రారంభం )
{
    //Set watchpoint 1 to watch the last 32 bytes of the stack.
    //Unfortunately, the Xtensa watchpoints can't set a watchpoint on a random [base - base+n] region because
    //the size works by masking off the lowest address bits. For that reason, we futz a bit and watch the lowest 32
    //bytes of the stack we can actually watch. In general, this can cause the watchpoint to be triggered at most
    //28 bytes early. The value 32 is chosen because it's larger than the stack canary, which in FreeRTOS is 20 bytes.
    //This way, we make sure we trigger before/when the stack canary is corrupted, not after.
    పూర్ణాంకం addr = (పూర్ణాంకం)px_దొంతర_ప్రారంభం;
    addr = (addr + 31) & (~31);
    esp_cpu_సమితి_watchpoint(దొంతర_WATCH_బిందువు_సంఖ్య, (అక్షరం *)addr, 32, ESP_CPU_WATCHPOINT_STORE);
}



/* ---------------------------------------------- Port Implementations -------------------------------------------------
 * Implementations of Porting Interface functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

పునాది_Type_t x_పోర్టు_Check_If_లోపల_ISR(శూన్య)
{
    //Disable interrupts so that reading port_interruptNesting is atomic
    పునాది_Type_t ret;
    గుర్తులేని పూర్ణాంకం prev_int_level = పోర్టు_DISABLE_INTERRUPTS();
    ret = (పోర్టు_అంతరాయం_Nesting[x_పోర్టు_Get_Core_ID()] != 0) ? pdTRUE : pdFALSE;
    పోర్టు_RESTORE_INTERRUPTS(prev_int_level);
    తిరిగి ret;
}

శూన్య v_పోర్టు_Assert_If_లోపల_ISR(శూన్య)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(x_పోర్టు_Check_If_లోపల_ISR() == 0);
}

// ------------------ Critical Sections --------------------

#ఒకవేళ ( config_సంఖ్య_యొక్క_CORES > 1 )
శూన్య v_పోర్టు_Take_తాళం( పోర్టు_MUX_TYPE *తాళం_2 )
{
    spinlock_acquire( తాళం_2, పోర్టు_MUX_NO_TIMEOUT);
}

శూన్య v_పోర్టు_Release_తాళం( పోర్టు_MUX_TYPE *తాళం_2 )
{
    spinlock_release( తాళం_2 );
}
#అయితే_ముగింపు /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

// ----------------------- System --------------------------

// ------------------- Run Time Stats ----------------------

// --------------------- TCB Cleanup -----------------------

#ఒకవేళ ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
స్థిర శూన్య v_పోర్టు_TLSPointers_Del_Cb( శూన్య *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pv_Thread_Local_Storage_సూచికలు_Del_Callback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_అంకె_THREAD_LOCAL_STORAGE_సూచికలు / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    కొరకు ( పూర్ణాంకం x = 0; x < ( config_అంకె_THREAD_LOCAL_STORAGE_సూచికలు / 2 ); x++ ) {
        ఒకవేళ ( pv_Thread_Local_Storage_సూచికలు_Del_Callback[ x ] != ఖాళీ ) {  //If del cb is set
            /* In case the TLSP deletion callback has been overwritten by a TLS pointer, gracefully abort. */
            ఒకవేళ ( !esp_ptr_executable( pv_Thread_Local_Storage_సూచికలు_Del_Callback[ x ] ) ) {
                // We call EARLY log here as currently portCLEAN_UP_TCB() is called in a critical section
                ESP_EARLY_LOGE("FreeRTOS", "Fatal error: TLSP deletion callback at index %d overwritten with non-excutable pointer %p", x, pv_Thread_Local_Storage_సూచికలు_Del_Callback[ x ]);
                abort();
            }

            pv_Thread_Local_Storage_సూచికలు_Del_Callback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#అయితే_ముగింపు /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

#ఒకవేళ ( XCHAL_CP_అంకె > 0 && configUSE_CORE_AFFINITY == 1 && config_అంకె_CORES > 1 )
స్థిర శూన్య v_పోర్టు_Clean_పైకి_Coproc_Area( శూన్య *pxTCB )
{
    UBaseType_t uxCoprocArea;
    పునాది_Type_t x_లక్ష్యం_Core_ID;

    /* Get pointer to the task's coprocessor save area from TCB->pxEndOfStack. See uxInitialiseStackCPSA() */
    uxCoprocArea = ( UBaseType_t ) ( ( ( StaticTask_t * ) pxTCB )->pxDummy8 );  /* Get TCB_t.pxEndOfStack */
    uxCoprocArea = ESP_ALIGN_కిందికి(uxCoprocArea - XT_CP_కొలత, 16);

    /* Extract core ID from the affinity mask */
    x_లక్ష్యం_Core_ID = ( ( StaticTask_t * ) pxTCB )->uxDummy26;
    x_లక్ష్యం_Core_ID = ( పునాది_Type_t ) __builtin_ffs( ( పూర్ణాంకం ) x_లక్ష్యం_Core_ID );
    ప్రతిజ్ఞా( x_లక్ష్యం_Core_ID >= 1 ); // __builtin_ffs always returns first set index + 1
    x_లక్ష్యం_Core_ID -= 1;

    /* If task has live floating point registers somewhere, release them */
    శూన్య _xt_coproc_release(అస్థిర శూన్య *coproc_sa_పునాది, పునాది_Type_t x_లక్ష్యం_Core_ID);
    _xt_coproc_release( (శూన్య *)uxCoprocArea, x_లక్ష్యం_Core_ID );
}
#అయితే_ముగింపు /* ( XCHAL_CP_NUM > 0 && configUSE_CORE_AFFINITY == 1 && configNUM_CORES > 1 ) */

శూన్య v_పోర్టు_TCBPre_Delete_Hook( శూన్య *pxTCB )
{
    #ఒకవేళ ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        బాహ్య శూన్య vTaskPreDeletionHook( శూన్య * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #అయితే_ముగింపు /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

    #ఒకవేళ ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
        /* Call TLS pointers deletion callbacks */
        v_పోర్టు_TLSPointers_Del_Cb( pxTCB );
    #అయితే_ముగింపు /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

    #ఒకవేళ ( XCHAL_CP_అంకె > 0 && configUSE_CORE_AFFINITY == 1 && config_అంకె_CORES > 1 )
        /* Cleanup coproc save area */
        v_పోర్టు_Clean_పైకి_Coproc_Area( pxTCB );
    #అయితే_ముగింపు /* ( XCHAL_CP_NUM > 0 && configUSE_CORE_AFFINITY == 1 && configNUM_CORES > 1 ) */
}

/* ------------------------------------------------ FreeRTOS Portable --------------------------------------------------
 * - Provides implementation for functions required by FreeRTOS
 * - Declared in portable.h
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------- Scheduler Start/End -------------------

బాహ్య శూన్య _xt_coproc_init(శూన్య);

పునాది_Type_t x_పోర్టు_ప్రారంభం_Scheduler( శూన్య )
{
    పోర్టు_DISABLE_INTERRUPTS();
    // Interrupts are disabled at this point and stack contains PS with enabled interrupts when task context is restored

#ఒకవేళ XCHAL_CP_అంకె > 0
    /* Initialize co-processor management for tasks. Leave CPENABLE alone. */
    _xt_coproc_init();
#అయితే_ముగింపు

    /* Setup the hardware to generate the tick. */
    v_పోర్టు_అమరిక_Timer();

        /* Initialize all kernel state tracking variables */
    పునాది_Type_t coreID = x_పోర్టు_Get_Core_ID();
    పోర్టు_x_Scheduler_Running[coreID] = 1;
    పోర్టు_ux_Core_Startup_పూర్తి[coreID] = 0;
#ఒకవేళ CONFIG_FREERTOS_పోర్టు_THREAD_సురక్షిత_CLAIM
    పోర్టు_x_Thread_సురక్షిత_Claimed = false;
#అయితే_ముగింపు

#ఒకవేళ config_అంకె_CORES > 1
    // Workaround for non-thread safe multi-core OS startup (see IDF-4524)
    ఒకవేళ (x_పోర్టు_Get_Core_ID() != 0) {
        prv_ప్రారంభం_Scheduler_Other_Cores();
    }
#అయితే_ముగింపు // configNUM_CORES > 1

    // Windows contain references to the startup stack which will be reclaimed by the main task
    // Spill the windows to create a clean environment to ensure we do not carry over any such references
    // to invalid SPs which will cause problems if main_task does a windowoverflow to them
    xthal_కిటికీ_spill();

    // Cannot be directly called from C; never returns
    __asm__ అస్థిర ("call0    _frxt_dispatch\n");

    /* Should not get here. */
    తిరిగి pdTRUE;
}

శూన్య v_పోర్టు_చివర_Scheduler( శూన్య )
{
    ;
}

// ------------------------ Stack --------------------------

// User exception dispatcher when exiting
శూన్య _xt_user_exit(శూన్య);

#ఒకవేళ CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
// Wrapper to allow task functions to return (increases stack overhead by 16 bytes)
స్థిర శూన్య v_పోర్టు_Task_Wrapper(TaskFunction_t px_సంకేతం, శూన్య *pvParameters)
{
    px_సంకేతం(pvParameters);
    //FreeRTOS tasks should not return. Log the task name and abort.
    అక్షరం *pcTaskName = pcTaskGetName(ఖాళీ);
    ESP_LOGE("FreeRTOS", "FreeRTOS Task \"%s\" should not return, Aborting now!", pcTaskName);
    abort();
}
#అయితే_ముగింపు

#ఒకవేళ XCHAL_CP_అంకె > 0
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_దొంతర_CPSA(UBaseType_t ux_దొంతర_సూచిక)
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
    ux_దొంతర_సూచిక = ESP_ALIGN_కిందికి(ux_దొంతర_సూచిక - XT_CP_కొలత, 16);
    // Initialize the coprocessor context switching flags.
    uint32_t *p = (uint32_t *)ux_దొంతర_సూచిక;
    p[0] = 0;   // Clear XT_CPENABLE and XT_CPSTORED
    p[1] = 0;   // Clear XT_CP_CS_ST
    // XT_CP_ASA points to the aligned start of the individual CP save areas (i.e., start of CP0 SA)
    p[2] = (uint32_t)ESP_ALIGN_పైకి((uint32_t)ux_దొంతర_సూచిక + 12, XCHAL_మొత్తం_SA_ALIGN);
    తిరిగి ux_దొంతర_సూచిక;
}
#అయితే_ముగింపు /* XCHAL_CP_NUM > 0 */

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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_దొంతర_TLS(UBaseType_t ux_దొంతర_సూచిక, uint32_t *ret_threadptr_reg_init)
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
    బాహ్య పూర్ణాంకం _tls_section_alignment;
    బాహ్య అక్షరం thread_local_దత్తాంశం_ప్రారంభం, thread_local_దత్తాంశం_చివర;
    బాహ్య అక్షరం thread_local_bss_ప్రారంభం, thread_local_bss_చివర;
    స్థిరరాశి uint32_t tls_దత్తాంశం_కొలత = (uint32_t)&thread_local_దత్తాంశం_చివర - (uint32_t)&thread_local_దత్తాంశం_ప్రారంభం;
    స్థిరరాశి uint32_t tls_bss_కొలత = (uint32_t)&thread_local_bss_చివర - (uint32_t)&thread_local_bss_ప్రారంభం;
    స్థిరరాశి uint32_t tls_area_కొలత = ESP_ALIGN_పైకి(tls_దత్తాంశం_కొలత + tls_bss_కొలత, 16);
    // TODO: check that TLS area fits the stack

    // Allocate space for the TLS area on the stack. The area must be allocated at a 16-byte aligned address
    ux_దొంతర_సూచిక = ESP_ALIGN_కిందికి(ux_దొంతర_సూచిక - (UBaseType_t)tls_area_కొలత, 16);
    // Initialize the TLS data with the initialization values of each TLS variable
    జ్ఞాపకం_నకలు((శూన్య *)ux_దొంతర_సూచిక, &thread_local_దత్తాంశం_ప్రారంభం, tls_దత్తాంశం_కొలత);
    // Initialize the TLS bss with zeroes
    memset((శూన్య *)(ux_దొంతర_సూచిక + tls_దత్తాంశం_కొలత), 0, tls_bss_కొలత);

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
    స్థిరరాశి uint32_t tls_section_align = (uint32_t)&_tls_section_alignment;  // ALIGN value of .flash.tdata section
    #నిర్వచించు TCB_కొలత 8
    స్థిరరాశి uint32_t పునాది = ESP_ALIGN_పైకి(TCB_కొలత, tls_section_align);
    *ret_threadptr_reg_init = (uint32_t)ux_దొంతర_సూచిక - పునాది;

    తిరిగి ux_దొంతర_సూచిక;
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_దొంతర_చట్రం(UBaseType_t ux_దొంతర_సూచిక, TaskFunction_t px_సంకేతం, శూన్య *pvParameters, uint32_t threadptr_reg_init)
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
    UBaseType_t ux_దొంతర_సూచిక_మునుపటి = ux_దొంతర_సూచిక;
    ux_దొంతర_సూచిక = ESP_ALIGN_కిందికి(ux_దొంతర_సూచిక - XT_STK_FRMSZ, 16);

    // Clear the entire interrupt stack frame
    memset((శూన్య *)ux_దొంతర_సూచిక, 0, (size_t)(ux_దొంతర_సూచిక_మునుపటి - ux_దొంతర_సూచిక));

    Xt_Exc_చట్రం *చట్రం = (Xt_Exc_చట్రం *)ux_దొంతర_సూచిక;

    /*
    Initialize common registers
    */
    చట్రం->a0 = 0;                                          // Set the return address to 0 terminate GDB backtrace
    చట్రం->a1 = ux_దొంతర_సూచిక + XT_STK_FRMSZ;              // Saved stack pointer should point to physical top of stack frame
    చట్రం->నిష్క్రమించు = (UBaseType_t) _xt_user_exit;              // User exception exit dispatcher

    /*
    Initialize the task's entry point. This will differ depending on
    - Whether the task's entry point is the wrapper function or pxCode
    - Whether Windowed ABI is used (for windowed, we mimic the task entry point being call4'd )
    */
    #ఒకవేళ CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
        చట్రం->pc = (UBaseType_t) v_పోర్టు_Task_Wrapper;         // Task entry point is the wrapper function
        #నిర్వచితమైతే __XTENSA_CALL0_ABI__
            చట్రం->a2 = (UBaseType_t) px_సంకేతం;               // Wrapper function's argument 0 (which is the task function)
            చట్రం->a3 = (UBaseType_t) pvParameters;         // Wrapper function's argument 1 (which is the task function's argument)
        #లేకపోతే // __XTENSA_CALL0_ABI__
            చట్రం->a6 = (UBaseType_t) px_సంకేతం;               // Wrapper function's argument 0 (which is the task function), passed as if we call4'd
            చట్రం->a7 = (UBaseType_t) pvParameters;         // Wrapper function's argument 1 (which is the task function's argument), passed as if we call4'd
        #అయితే_ముగింపు // __XTENSA_CALL0_ABI__
    #లేకపోతే
        చట్రం->pc = (UBaseType_t) px_సంకేతం;                   // Task entry point is the provided task function
        #నిర్వచితమైతే __XTENSA_CALL0_ABI__
            చట్రం->a2 = (UBaseType_t) pvParameters;         // Task function's argument
        #లేకపోతే // __XTENSA_CALL0_ABI__
            చట్రం->a6 = (UBaseType_t) pvParameters;         // Task function's argument, passed as if we call4'd
        #అయితే_ముగింపు // __XTENSA_CALL0_ABI__
    #అయితే_ముగింపు

    /*
    Set initial PS to int level 0, EXCM disabled ('rfe' will enable), user mode.
    For windowed ABI also set WOE and CALLINC (pretend task was 'call4'd)
    */
    #నిర్వచితమైతే __XTENSA_CALL0_ABI__
        చట్రం->ps = PS_UM | PS_EXCM;
    #లేకపోతే // __XTENSA_CALL0_ABI__
        చట్రం->ps = PS_UM | PS_EXCM | PS_WOE | PS_CALLINC(1);
    #అయితే_ముగింపు // __XTENSA_CALL0_ABI__

    #నిర్వచితమైతే XT_USE_SWPRI
        // Set the initial virtual priority mask value to all 1's.
        చట్రం->vpri = 0xFFFFFFFF;
    #అయితే_ముగింపు

    // Initialize the threadptr register in the extra save area of the stack frame
    uint32_t *threadptr_reg = (uint32_t *)(ux_దొంతర_సూచిక + XT_STK_EXTRA);
    *threadptr_reg = threadptr_reg_init;

    తిరిగి ux_దొంతర_సూచిక;
}

#ఒకవేళ ( పోర్టు_HAS_దొంతర_అధికప్రవాహం_CHECKING == 1 )
దొంతర_Type_t * px_పోర్టు_Initialise_దొంతర( దొంతర_Type_t * px_శిఖరం_యొక్క_దొంతర,
                                     దొంతర_Type_t * px_చివర_యొక్క_దొంతర,
                                     TaskFunction_t px_సంకేతం,
                                     శూన్య * pvParameters )
#లేకపోతే
దొంతర_Type_t * px_పోర్టు_Initialise_దొంతర( దొంతర_Type_t * px_శిఖరం_యొక్క_దొంతర,
                                     TaskFunction_t px_సంకేతం,
                                     శూన్య * pvParameters )
#అయితే_ముగింపు
{
#నిర్వచితమైతే __clang_analyzer__
    // Teach clang-tidy that pxTopOfStack cannot be a pointer to const
    అస్థిర దొంతర_Type_t * px_తాత్కాలిక = px_శిఖరం_యొక్క_దొంతర;
    px_శిఖరం_యొక్క_దొంతర = px_తాత్కాలిక;
#అయితే_ముగింపు /*__clang_analyzer__ */
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

    UBaseType_t ux_దొంతర_సూచిక = (UBaseType_t)px_శిఖరం_యొక్క_దొంతర;
    configASSERT((ux_దొంతర_సూచిక & పోర్టు_BYTE_ALIGNMENT_ముసుగు) == 0);

#ఒకవేళ XCHAL_CP_అంకె > 0
    // Initialize the coprocessor save area. THIS MUST BE THE FIRST AREA due to access from _frxt_task_coproc_state()
    ux_దొంతర_సూచిక = ux_Initialise_దొంతర_CPSA(ux_దొంతర_సూచిక);
    configASSERT((ux_దొంతర_సూచిక & పోర్టు_BYTE_ALIGNMENT_ముసుగు) == 0);
#అయితే_ముగింపు /* XCHAL_CP_NUM > 0 */

    // Initialize the GCC TLS area
    uint32_t threadptr_reg_init;
    ux_దొంతర_సూచిక = ux_Initialise_దొంతర_TLS(ux_దొంతర_సూచిక, &threadptr_reg_init);
    configASSERT((ux_దొంతర_సూచిక & పోర్టు_BYTE_ALIGNMENT_ముసుగు) == 0);

    // Initialize the starting interrupt stack frame
    ux_దొంతర_సూచిక = ux_Initialise_దొంతర_చట్రం(ux_దొంతర_సూచిక, px_సంకేతం, pvParameters, threadptr_reg_init);
    configASSERT((ux_దొంతర_సూచిక & పోర్టు_BYTE_ALIGNMENT_ముసుగు) == 0);

    // Return the task's current stack pointer address which should point to the starting interrupt stack frame
    తిరిగి (దొంతర_Type_t *)ux_దొంతర_సూచిక;
}

// ------------------- Hook Functions ----------------------

#అన్తర్భావ <stdlib.h>

#ఒకవేళ ( config_CHECK_FOR_దొంతర_అధికప్రవాహం > 0 )
శూన్య  __attribute__((weak)) v_Application_దొంతర_అధికప్రవాహం_Hook( TaskHandle_t xTask, అక్షరం *pcTaskName )
{
#నిర్వచించు ERR_STR1 "***ERROR*** A stack overflow in task "
#నిర్వచించు ERR_STR2 " has been detected."
    స్థిరరాశి అక్షరం *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    అక్షరం buf[పరిమాణం(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + పరిమాణం(ERR_STR2) + 1 /* null char */] = { 0 };

    అక్షరం *dest = buf;
    కొరకు (size_t i = 0 ; i < పరిమాణం(str) / పరిమాణం(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
    esp_system_abort(buf);
}
#అయితే_ముగింపు

బాహ్య శూన్య esp_vApplicationIdleHook(శూన్య);
#ఒకవేళ CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK
/*
By default, the port uses vApplicationPassiveIdleHook() to run IDF style idle
hooks. However, users may also want to provide their own vApplicationPassiveIdleHook().
In this case, we use to -Wl,--wrap option to wrap the user provided vApplicationPassiveIdleHook()
*/
బాహ్య శూన్య __real_vApplicationPassiveIdleHook( శూన్య );
శూన్య __wrap_vApplicationPassiveIdleHook( శూన్య )
{
    esp_vApplicationIdleHook(); //Run IDF style hooks
    __real_vApplicationPassiveIdleHook(); //Call the user provided vApplicationPassiveIdleHook()
}
#లేకపోతే // CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK
శూన్య vApplicationPassiveIdleHook( శూన్య )
{
    esp_vApplicationIdleHook(); //Run IDF style hooks
}
#అయితే_ముగింపు // CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK

/* ------------------------------------------------ Run Time Stats ------------------------------------------------- */

#ఒకవేళ ( CONFIG_FREERTOS_GENERATE_నిరంతరం_సమయం_STATS )

config_నిరంతరం_సమయం_COUNTER_TYPE x_పోర్టు_Get_నిరంతరం_సమయం_Counter_విలువ( శూన్య )
{
#నిర్వచితమైతే CONFIG_FREERTOS_నిరంతరం_సమయం_STATS_USING_ESP_TIMER
    తిరిగి (config_నిరంతరం_సమయం_COUNTER_TYPE) esp_timer_get_సమయం();
#లేకపోతే // Uses CCOUNT
    తిరిగి (config_నిరంతరం_సమయం_COUNTER_TYPE) xthal_get_ccount();
#అయితే_ముగింపు // CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
}

#అయితే_ముగింపు /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
