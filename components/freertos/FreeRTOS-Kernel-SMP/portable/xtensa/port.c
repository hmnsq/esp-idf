/*
 * SPDX-FileCopyrightText: 2022-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <stdint.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव "esp_compiler.h"
#अन्तर्भाव "FreeRTOS.h"
#अन्तर्भाव "task.h"   //For vApplicationStackOverflowHook
#अन्तर्भाव "port_systick.h"
#अन्तर्भाव "portmacro.h"
#अन्तर्भाव "spinlock.h"
#अन्तर्भाव "xt_instr_macros.h"
#अन्तर्भाव "xtensa_context.h"
#अन्तर्भाव "xtensa/corebits.h"
#अन्तर्भाव "xtensa/config/core.h"
#अन्तर्भाव "xtensa/config/core-isa.h"
#अन्तर्भाव "xtensa/xtruntime.h"
#अन्तर्भाव "esp_private/freertos_idf_additions_priv.h"
#अन्तर्भाव "esp_private/esp_int_wdt.h"
#अन्तर्भाव "esp_private/systimer.h"
#अन्तर्भाव "esp_private/periph_ctrl.h"
#अन्तर्भाव "esp_attr.h"
#अन्तर्भाव "esp_heap_caps.h"
#अन्तर्भाव "esp_system.h"
#अन्तर्भाव "esp_task.h"
#अन्तर्भाव "esp_log.h"
#अन्तर्भाव "esp_cpu.h"
#अन्तर्भाव "esp_rom_sys.h"
#अन्तर्भाव "esp_task_wdt.h"
#अन्तर्भाव "esp_heap_caps_init.h"
#अन्तर्भाव "esp_freertos_hooks.h"
#अन्तर्भाव "esp_intr_alloc.h"
#अन्तर्भाव "esp_memory_utils.h"
#अन्तर्भाव "esp_macros.h"
#अन्तर्भाव <xtensa/hal.h>             /* required for xthal_get_ccount() */
#यदि CONFIG_FREERTOS_सततम्_TIME_STATS_USING_ESP_TIMER
#अन्तर्भाव "esp_timer.h"
#यदि_अन्तः
#यदि_परिभाषितम् CONFIG_FREERTOS_SYSTICK_उपयुङ्क्ते_SYSTIMER
#अन्तर्भाव "soc/periph_defs.h"
#अन्तर्भाव "soc/system_reg.h"
#अन्तर्भाव "hal/systimer_hal.h"
#अन्तर्भाव "hal/systimer_ll.h"
#यदि_अन्तः // CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER

_Static_assert(पत्तनम्_BYTE_ALIGNMENT == 16, "portBYTE_ALIGNMENT must be set to 16");

/* ---------------------------------------------------- Variables ------------------------------------------------------
 * - Various variables used to maintain the FreeRTOS port's state. Used from both port.c and various .S files
 * - Constant offsets are used by assembly to jump to particular TCB members or a stack area (such as the CPSA). We use
 *   C constants instead of preprocessor macros due to assembly lacking "offsetof()".
 * ------------------------------------------------------------------------------------------------------------------ */

#यदि XCHAL_CP_अङ्कः > 0
/* Offsets used to navigate to a task's CPSA on the stack */
ध्रुव DRAM_ATTR uint32_t विस्थापनम्_px_अन्तः_सम्बन्धि_राशिः = offsetof(StaticTask_t, pxDummy8);
ध्रुव DRAM_ATTR uint32_t विस्थापनम्_cpsa = XT_CP_आकार;  /* Offset to start of the CPSA area on the stack. See uxInitialiseStackCPSA(). */
#यदि config_अङ्कः_CORES > 1
/* Offset to TCB_t.uxCoreAffinityMask member. Used to pin unpinned tasks that use the FPU. */
ध्रुव DRAM_ATTR uint32_t विस्थापनम्_ux_Core_Affinity_आवरणम् = offsetof(StaticTask_t, uxDummy26);
#यदि configUSE_CORE_AFFINITY != 1
#दोषः "configUSE_CORE_AFFINITY must be 1 on multicore targets with coprocessor support"
#यदि_अन्तः
#यदि_अन्तः /* configNUM_CORES > 1 */
#यदि_अन्तः /* XCHAL_CP_NUM > 0 */

चञ्चल अचिह्नित पत्तनम्_x_Scheduler_Running[पत्तनम्_अङ्कः_PROCESSORS] = {0}; // Indicates whether scheduler is running on a per-core basis
अचिह्नित पूर्णाङ्क पत्तनम्_व्यवधानम्_Nesting[पत्तनम्_अङ्कः_PROCESSORS] = {0};  // Interrupt nesting level. Increased/decreased in portasm.c, _frxt_int_enter/_frxt_int_exit
चञ्चल अचिह्नित पत्तनम्_ux_Core_Startup_समाप्तम्[पत्तनम्_अङ्कः_PROCESSORS] = {0};  // Indicates whether the core has completed its startup sequence
#यदि ( config_सङ्ख्या_सम्बन्धि_CORES > 1 )
//FreeRTOS SMP Locks
पत्तनम्_MUX_TYPE पत्तनम्_x_Task_तालकम् = पत्तनम्_MUX_INITIALIZER_UNLOCKED;
पत्तनम्_MUX_TYPE पत्तनम्_x_ISRLock = पत्तनम्_MUX_INITIALIZER_UNLOCKED;
#यदि_अन्तः /* configNUMBER_OF_CORES > 1 */

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

आधारः_Type_t x_पत्तनम्_Interrupted_From_ISRContext(शून्य)
{
    प्रत्यागम (पत्तनम्_व्यवधानम्_Nesting[x_पत्तनम्_Get_Core_ID()] != 0);
}

// ------------------ Critical Sections --------------------

/*
Variables used by IDF critical sections only (SMP tracks critical nesting inside TCB now)
[refactor-todo] Figure out how IDF critical sections will be merged with SMP FreeRTOS critical sections
*/
आधारः_Type_t पत्तनम्_ux_Critical_Nesting_IDF[पत्तनम्_अङ्कः_PROCESSORS] = {0};
आधारः_Type_t पत्तनम्_ux_Critical_Old_व्यवधानम्_State_IDF[पत्तनम्_अङ्कः_PROCESSORS] = {0};
#यदि CONFIG_FREERTOS_पत्तनम्_THREAD_SAFE_CLAIM
चञ्चल bool पत्तनम्_x_Thread_Safe_Claimed = false;
#यदि_अन्तः

/*
*******************************************************************************
* Interrupt stack. The size of the interrupt stack is determined by the config
* parameter "configISR_STACK_SIZE" in FreeRTOSConfig.h
*******************************************************************************
*/
चञ्चल राशिः_Type_t DRAM_ATTR __attribute__((aligned(16))) पत्तनम्_Int_राशिः[पत्तनम्_अङ्कः_PROCESSORS][config_ISR_राशिः_आकार];
/* One flag for each individual CPU. */
चञ्चल uint32_t पत्तनम्_switch_flag[पत्तनम्_अङ्कः_PROCESSORS];

#यदि CONFIG_FREERTOS_पत्तनम्_THREAD_SAFE_CLAIM
शून्य x_पत्तनम्_Thread_Safe_Claim(शून्य)
{
    configASSERT(!x_पत्तनम्_Can_Yield());
    configASSERT(!पत्तनम्_x_Thread_Safe_Claimed);
    पत्तनम्_x_Thread_Safe_Claimed = true;
}

शून्य x_पत्तनम्_Thread_Safe_Disclaim(शून्य)
{
    configASSERT(!x_पत्तनम्_Can_Yield());
    configASSERT(पत्तनम्_x_Thread_Safe_Claimed);
    पत्तनम्_x_Thread_Safe_Claimed = false;
}
#यदि_अन्तः /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

आधारः_Type_t x_पत्तनम्_Enter_Critical_Timeout(पत्तनम्_MUX_TYPE *तालकम्, आधारः_Type_t timeout)
{
#यदि CONFIG_FREERTOS_पत्तनम्_THREAD_SAFE_CLAIM
    यदि (unlikely(पत्तनम्_x_Thread_Safe_Claimed)) {
        प्रत्यागम pdPASS;
    }
#यदि_अन्तः
    /* Interrupts may already be disabled (if this function is called in nested
     * manner). However, there's no atomic operation that will allow us to check,
     * thus we have to disable interrupts again anyways.
     *
     * However, if this is call is NOT nested (i.e., the first call to enter a
     * critical section), we will save the previous interrupt level so that the
     * saved level can be restored on the last call to exit the critical.
     */
    आधारः_Type_t x_Old_व्यवधानम्_Level = XTOS_समुच्चयः_INTLEVEL(XCHAL_EXCM_LEVEL);
    /* Interrupts are masked (to XCHAL_EXCM_LEVEL, the same level the spinlock uses),
     * so the core id is stable and the spinlock does not need to mask again. Read the
     * core id register once and reuse it for the owner id and the nesting index. */
    uint32_t coreOwnerId = spinlock_owner_id();
    आधारः_Type_t coreID = spinlock_core_id_from_owner_id(coreOwnerId);
    यदि (!spinlock_acquire_impl(तालकम्, timeout, coreOwnerId)) {
        //Timed out attempting to get spinlock. Restore previous interrupt level and return
        XTOS_RESTORE_JUST_INTLEVEL((पूर्णाङ्क) x_Old_व्यवधानम्_Level);
        प्रत्यागम pdFAIL;
    }
    //Spinlock acquired. Increment the IDF critical nesting count.
    आधारः_Type_t newNesting = पत्तनम्_ux_Critical_Nesting_IDF[coreID] + 1;
    पत्तनम्_ux_Critical_Nesting_IDF[coreID] = newNesting;
    //If this is the first entry to a critical section. Save the old interrupt level.
    यदि ( newNesting == 1 ) {
        पत्तनम्_ux_Critical_Old_व्यवधानम्_State_IDF[coreID] = x_Old_व्यवधानम्_Level;
    }
    प्रत्यागम pdPASS;

}

शून्य v_पत्तनम्_Exit_Critical_IDF(पत्तनम्_MUX_TYPE *तालकम्)
{
#यदि CONFIG_FREERTOS_पत्तनम्_THREAD_SAFE_CLAIM
    यदि (unlikely(पत्तनम्_x_Thread_Safe_Claimed)) {
        प्रत्यागम;
    }
#यदि_अन्तः
    /* This function may be called in a nested manner. Therefore, we only need
     * to re-enable interrupts if this is the last call to exit the critical. We
     * can use the nesting count to determine whether this is the last exit call.
     */
    /* Interrupts remain disabled for the whole critical section, so the core id is
     * stable and the spinlock does not need to mask again. Read the core id register
     * once and reuse it for the owner id and the nesting index. */
    uint32_t coreOwnerId = spinlock_owner_id();
    आधारः_Type_t coreID = spinlock_core_id_from_owner_id(coreOwnerId);
    spinlock_release_impl(तालकम्, coreOwnerId);
    आधारः_Type_t nesting = पत्तनम्_ux_Critical_Nesting_IDF[coreID];

    /* Critical section nesting count must never be negative */
    configASSERT( nesting > 0 );

    यदि (nesting > 0) {
        nesting--;
        पत्तनम्_ux_Critical_Nesting_IDF[coreID] = nesting;

        //This is the last exit call, restore the saved interrupt level
        यदि ( nesting == 0 ) {
            XTOS_RESTORE_JUST_INTLEVEL((पूर्णाङ्क) पत्तनम्_ux_Critical_Old_व्यवधानम्_State_IDF[coreID]);
        }
    }
}

/*
In case any IDF libs called the port critical functions directly instead of through the macros.
Just inline call the IDF versions
*/
शून्य v_पत्तनम्_Enter_Critical(पत्तनम्_MUX_TYPE *तालकम्)
{
    v_पत्तनम्_Enter_Critical_IDF(तालकम्);
}

शून्य v_पत्तनम्_Exit_Critical(पत्तनम्_MUX_TYPE *तालकम्)
{
    v_पत्तनम्_Exit_Critical_IDF(तालकम्);
}

// ----------------------- System --------------------------

#परिभाषय राशिः_WATCH_POINT_सङ्ख्या (SOC_CPU_WATCHPOINTS_अङ्कः - 1)

शून्य v_पत्तनम्_समुच्चयः_राशिः_Watchpoint( शून्य *px_राशिः_Start )
{
    //Set watchpoint 1 to watch the last 32 bytes of the stack.
    //Unfortunately, the Xtensa watchpoints can't set a watchpoint on a random [base - base+n] region because
    //the size works by masking off the lowest address bits. For that reason, we futz a bit and watch the lowest 32
    //bytes of the stack we can actually watch. In general, this can cause the watchpoint to be triggered at most
    //28 bytes early. The value 32 is chosen because it's larger than the stack canary, which in FreeRTOS is 20 bytes.
    //This way, we make sure we trigger before/when the stack canary is corrupted, not after.
    पूर्णाङ्क addr = (पूर्णाङ्क)px_राशिः_Start;
    addr = (addr + 31) & (~31);
    esp_cpu_समुच्चयः_watchpoint(राशिः_WATCH_POINT_सङ्ख्या, (वर्ण *)addr, 32, ESP_CPU_WATCHPOINT_STORE);
}



/* ---------------------------------------------- Port Implementations -------------------------------------------------
 * Implementations of Porting Interface functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

आधारः_Type_t x_पत्तनम्_Check_If_अन्तर्_ISR(शून्य)
{
    //Disable interrupts so that reading port_interruptNesting is atomic
    आधारः_Type_t ret;
    अचिह्नित पूर्णाङ्क पूर्व_int_level = पत्तनम्_DISABLE_INTERRUPTS();
    ret = (पत्तनम्_व्यवधानम्_Nesting[x_पत्तनम्_Get_Core_ID()] != 0) ? pdTRUE : pdFALSE;
    पत्तनम्_RESTORE_INTERRUPTS(पूर्व_int_level);
    प्रत्यागम ret;
}

शून्य v_पत्तनम्_Assert_If_अन्तर्_ISR(शून्य)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(x_पत्तनम्_Check_If_अन्तर्_ISR() == 0);
}

// ------------------ Critical Sections --------------------

#यदि ( config_सङ्ख्या_सम्बन्धि_CORES > 1 )
शून्य v_पत्तनम्_Take_तालकम्( पत्तनम्_MUX_TYPE *तालकम् )
{
    spinlock_acquire( तालकम्, पत्तनम्_MUX_NO_TIMEOUT);
}

शून्य v_पत्तनम्_Release_तालकम्( पत्तनम्_MUX_TYPE *तालकम् )
{
    spinlock_release( तालकम् );
}
#यदि_अन्तः /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

// ----------------------- System --------------------------

// ------------------- Run Time Stats ----------------------

// --------------------- TCB Cleanup -----------------------

#यदि ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
स्थिर शून्य v_पत्तनम्_TLSPointers_Del_Cb( शून्य *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pv_Thread_Local_Storage_सूचकाः_Del_Callback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_अङ्कः_THREAD_LOCAL_STORAGE_सूचकाः / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    प्रति ( पूर्णाङ्क x = 0; x < ( config_अङ्कः_THREAD_LOCAL_STORAGE_सूचकाः / 2 ); x++ ) {
        यदि ( pv_Thread_Local_Storage_सूचकाः_Del_Callback[ x ] != रिक्त ) {  //If del cb is set
            /* In case the TLSP deletion callback has been overwritten by a TLS pointer, gracefully abort. */
            यदि ( !esp_ptr_executable( pv_Thread_Local_Storage_सूचकाः_Del_Callback[ x ] ) ) {
                // We call EARLY log here as currently portCLEAN_UP_TCB() is called in a critical section
                ESP_EARLY_LOGE("FreeRTOS", "Fatal error: TLSP deletion callback at index %d overwritten with non-excutable pointer %p", x, pv_Thread_Local_Storage_सूचकाः_Del_Callback[ x ]);
                abort();
            }

            pv_Thread_Local_Storage_सूचकाः_Del_Callback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#यदि_अन्तः /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

#यदि ( XCHAL_CP_अङ्कः > 0 && configUSE_CORE_AFFINITY == 1 && config_अङ्कः_CORES > 1 )
स्थिर शून्य v_पत्तनम्_Clean_ऊर्ध्वम्_Coproc_Area( शून्य *pxTCB )
{
    UBaseType_t uxCoprocArea;
    आधारः_Type_t xTargetCoreID;

    /* Get pointer to the task's coprocessor save area from TCB->pxEndOfStack. See uxInitialiseStackCPSA() */
    uxCoprocArea = ( UBaseType_t ) ( ( ( StaticTask_t * ) pxTCB )->pxDummy8 );  /* Get TCB_t.pxEndOfStack */
    uxCoprocArea = ESP_ALIGN_अधस्तात्(uxCoprocArea - XT_CP_आकार, 16);

    /* Extract core ID from the affinity mask */
    xTargetCoreID = ( ( StaticTask_t * ) pxTCB )->uxDummy26;
    xTargetCoreID = ( आधारः_Type_t ) __builtin_ffs( ( पूर्णाङ्क ) xTargetCoreID );
    प्रतिज्ञा( xTargetCoreID >= 1 ); // __builtin_ffs always returns first set index + 1
    xTargetCoreID -= 1;

    /* If task has live floating point registers somewhere, release them */
    शून्य _xt_coproc_release(चञ्चल शून्य *coproc_sa_आधारः, आधारः_Type_t xTargetCoreID);
    _xt_coproc_release( (शून्य *)uxCoprocArea, xTargetCoreID );
}
#यदि_अन्तः /* ( XCHAL_CP_NUM > 0 && configUSE_CORE_AFFINITY == 1 && configNUM_CORES > 1 ) */

शून्य v_पत्तनम्_TCBPre_Delete_Hook( शून्य *pxTCB )
{
    #यदि ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        बाह्य शून्य vTaskPreDeletionHook( शून्य * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #यदि_अन्तः /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

    #यदि ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
        /* Call TLS pointers deletion callbacks */
        v_पत्तनम्_TLSPointers_Del_Cb( pxTCB );
    #यदि_अन्तः /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

    #यदि ( XCHAL_CP_अङ्कः > 0 && configUSE_CORE_AFFINITY == 1 && config_अङ्कः_CORES > 1 )
        /* Cleanup coproc save area */
        v_पत्तनम्_Clean_ऊर्ध्वम्_Coproc_Area( pxTCB );
    #यदि_अन्तः /* ( XCHAL_CP_NUM > 0 && configUSE_CORE_AFFINITY == 1 && configNUM_CORES > 1 ) */
}

/* ------------------------------------------------ FreeRTOS Portable --------------------------------------------------
 * - Provides implementation for functions required by FreeRTOS
 * - Declared in portable.h
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------- Scheduler Start/End -------------------

बाह्य शून्य _xt_coproc_init(शून्य);

आधारः_Type_t x_पत्तनम्_Start_Scheduler( शून्य )
{
    पत्तनम्_DISABLE_INTERRUPTS();
    // Interrupts are disabled at this point and stack contains PS with enabled interrupts when task context is restored

#यदि XCHAL_CP_अङ्कः > 0
    /* Initialize co-processor management for tasks. Leave CPENABLE alone. */
    _xt_coproc_init();
#यदि_अन्तः

    /* Setup the hardware to generate the tick. */
    v_पत्तनम्_सज्जता_Timer();

        /* Initialize all kernel state tracking variables */
    आधारः_Type_t coreID = x_पत्तनम्_Get_Core_ID();
    पत्तनम्_x_Scheduler_Running[coreID] = 1;
    पत्तनम्_ux_Core_Startup_समाप्तम्[coreID] = 0;
#यदि CONFIG_FREERTOS_पत्तनम्_THREAD_SAFE_CLAIM
    पत्तनम्_x_Thread_Safe_Claimed = false;
#यदि_अन्तः

#यदि config_अङ्कः_CORES > 1
    // Workaround for non-thread safe multi-core OS startup (see IDF-4524)
    यदि (x_पत्तनम्_Get_Core_ID() != 0) {
        prvStartSchedulerOtherCores();
    }
#यदि_अन्तः // configNUM_CORES > 1

    // Windows contain references to the startup stack which will be reclaimed by the main task
    // Spill the windows to create a clean environment to ensure we do not carry over any such references
    // to invalid SPs which will cause problems if main_task does a windowoverflow to them
    xthal_गवाक्षः_spill();

    // Cannot be directly called from C; never returns
    __asm__ चञ्चल ("call0    _frxt_dispatch\n");

    /* Should not get here. */
    प्रत्यागम pdTRUE;
}

शून्य v_पत्तनम्_अन्तः_Scheduler( शून्य )
{
    ;
}

// ------------------------ Stack --------------------------

// User exception dispatcher when exiting
शून्य _xt_user_exit(शून्य);

#यदि CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
// Wrapper to allow task functions to return (increases stack overhead by 16 bytes)
स्थिर शून्य v_पत्तनम्_Task_Wrapper(TaskFunction_t px_संहिता, शून्य *pvParameters)
{
    px_संहिता(pvParameters);
    //FreeRTOS tasks should not return. Log the task name and abort.
    वर्ण *pcTaskName = pcTaskGetName(रिक्त);
    ESP_LOGE("FreeRTOS", "FreeRTOS Task \"%s\" should not return, Aborting now!", pcTaskName);
    abort();
}
#यदि_अन्तः

#यदि XCHAL_CP_अङ्कः > 0
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_राशिः_CPSA(UBaseType_t ux_राशिः_Pointer)
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
    ux_राशिः_Pointer = ESP_ALIGN_अधस्तात्(ux_राशिः_Pointer - XT_CP_आकार, 16);
    // Initialize the coprocessor context switching flags.
    uint32_t *p = (uint32_t *)ux_राशिः_Pointer;
    p[0] = 0;   // Clear XT_CPENABLE and XT_CPSTORED
    p[1] = 0;   // Clear XT_CP_CS_ST
    // XT_CP_ASA points to the aligned start of the individual CP save areas (i.e., start of CP0 SA)
    p[2] = (uint32_t)ESP_ALIGN_ऊर्ध्वम्((uint32_t)ux_राशिः_Pointer + 12, XCHAL_योग_SA_ALIGN);
    प्रत्यागम ux_राशिः_Pointer;
}
#यदि_अन्तः /* XCHAL_CP_NUM > 0 */

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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_राशिः_TLS(UBaseType_t ux_राशिः_Pointer, uint32_t *ret_threadptr_reg_init)
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
    बाह्य पूर्णाङ्क _tls_section_alignment;
    बाह्य वर्ण thread_local_दत्त_start, thread_local_दत्त_अन्तः;
    बाह्य वर्ण _thread_local_bss_start, thread_local_bss_अन्तः;
    ध्रुव uint32_t tls_दत्त_आकार = (uint32_t)&thread_local_दत्त_अन्तः - (uint32_t)&thread_local_दत्त_start;
    ध्रुव uint32_t tls_bss_आकार = (uint32_t)&thread_local_bss_अन्तः - (uint32_t)&_thread_local_bss_start;
    ध्रुव uint32_t tls_area_आकार = ESP_ALIGN_ऊर्ध्वम्(tls_दत्त_आकार + tls_bss_आकार, 16);
    // TODO: check that TLS area fits the stack

    // Allocate space for the TLS area on the stack. The area must be allocated at a 16-byte aligned address
    ux_राशिः_Pointer = ESP_ALIGN_अधस्तात्(ux_राशिः_Pointer - (UBaseType_t)tls_area_आकार, 16);
    // Initialize the TLS data with the initialization values of each TLS variable
    स्मृतिं_प्रतिलिख((शून्य *)ux_राशिः_Pointer, &thread_local_दत्त_start, tls_दत्त_आकार);
    // Initialize the TLS bss with zeroes
    स्मृतिं_पूरय((शून्य *)(ux_राशिः_Pointer + tls_दत्त_आकार), 0, tls_bss_आकार);

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
    ध्रुव uint32_t tls_section_align = (uint32_t)&_tls_section_alignment;  // ALIGN value of .flash.tdata section
    #परिभाषय TCB_आकार 8
    ध्रुव uint32_t आधारः = ESP_ALIGN_ऊर्ध्वम्(TCB_आकार, tls_section_align);
    *ret_threadptr_reg_init = (uint32_t)ux_राशिः_Pointer - आधारः;

    प्रत्यागम ux_राशिः_Pointer;
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_राशिः_Frame(UBaseType_t ux_राशिः_Pointer, TaskFunction_t px_संहिता, शून्य *pvParameters, uint32_t threadptr_reg_init)
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
    UBaseType_t ux_राशिः_Pointer_Previous = ux_राशिः_Pointer;
    ux_राशिः_Pointer = ESP_ALIGN_अधस्तात्(ux_राशिः_Pointer - XT_STK_FRMSZ, 16);

    // Clear the entire interrupt stack frame
    स्मृतिं_पूरय((शून्य *)ux_राशिः_Pointer, 0, (size_t)(ux_राशिः_Pointer_Previous - ux_राशिः_Pointer));

    XtExcFrame *frame = (XtExcFrame *)ux_राशिः_Pointer;

    /*
    Initialize common registers
    */
    frame->a0 = 0;                                          // Set the return address to 0 terminate GDB backtrace
    frame->a1 = ux_राशिः_Pointer + XT_STK_FRMSZ;              // Saved stack pointer should point to physical top of stack frame
    frame->निर्गच्छ = (UBaseType_t) _xt_user_exit;              // User exception exit dispatcher

    /*
    Initialize the task's entry point. This will differ depending on
    - Whether the task's entry point is the wrapper function or pxCode
    - Whether Windowed ABI is used (for windowed, we mimic the task entry point being call4'd )
    */
    #यदि CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
        frame->pc = (UBaseType_t) v_पत्तनम्_Task_Wrapper;         // Task entry point is the wrapper function
        #यदि_परिभाषितम् __XTENSA_CALL0_ABI__
            frame->a2 = (UBaseType_t) px_संहिता;               // Wrapper function's argument 0 (which is the task function)
            frame->a3 = (UBaseType_t) pvParameters;         // Wrapper function's argument 1 (which is the task function's argument)
        #अन्यथा // __XTENSA_CALL0_ABI__
            frame->a6 = (UBaseType_t) px_संहिता;               // Wrapper function's argument 0 (which is the task function), passed as if we call4'd
            frame->a7 = (UBaseType_t) pvParameters;         // Wrapper function's argument 1 (which is the task function's argument), passed as if we call4'd
        #यदि_अन्तः // __XTENSA_CALL0_ABI__
    #अन्यथा
        frame->pc = (UBaseType_t) px_संहिता;                   // Task entry point is the provided task function
        #यदि_परिभाषितम् __XTENSA_CALL0_ABI__
            frame->a2 = (UBaseType_t) pvParameters;         // Task function's argument
        #अन्यथा // __XTENSA_CALL0_ABI__
            frame->a6 = (UBaseType_t) pvParameters;         // Task function's argument, passed as if we call4'd
        #यदि_अन्तः // __XTENSA_CALL0_ABI__
    #यदि_अन्तः

    /*
    Set initial PS to int level 0, EXCM disabled ('rfe' will enable), user mode.
    For windowed ABI also set WOE and CALLINC (pretend task was 'call4'd)
    */
    #यदि_परिभाषितम् __XTENSA_CALL0_ABI__
        frame->ps = PS_UM | PS_EXCM;
    #अन्यथा // __XTENSA_CALL0_ABI__
        frame->ps = PS_UM | PS_EXCM | PS_WOE | PS_CALLINC(1);
    #यदि_अन्तः // __XTENSA_CALL0_ABI__

    #यदि_परिभाषितम् XT_USE_SWPRI
        // Set the initial virtual priority mask value to all 1's.
        frame->vpri = 0xFFFFFFFF;
    #यदि_अन्तः

    // Initialize the threadptr register in the extra save area of the stack frame
    uint32_t *threadptr_reg = (uint32_t *)(ux_राशिः_Pointer + XT_STK_EXTRA);
    *threadptr_reg = threadptr_reg_init;

    प्रत्यागम ux_राशिः_Pointer;
}

#यदि ( पत्तनम्_HAS_राशिः_अतिप्रवाहः_CHECKING == 1 )
राशिः_Type_t * px_पत्तनम्_Initialise_राशिः( राशिः_Type_t * px_शिखरम्_सम्बन्धि_राशिः,
                                     राशिः_Type_t * px_अन्तः_सम्बन्धि_राशिः,
                                     TaskFunction_t px_संहिता,
                                     शून्य * pvParameters )
#अन्यथा
राशिः_Type_t * px_पत्तनम्_Initialise_राशिः( राशिः_Type_t * px_शिखरम्_सम्बन्धि_राशिः,
                                     TaskFunction_t px_संहिता,
                                     शून्य * pvParameters )
#यदि_अन्तः
{
#यदि_परिभाषितम् __clang_analyzer__
    // Teach clang-tidy that pxTopOfStack cannot be a pointer to const
    चञ्चल राशिः_Type_t * px_अस्थायी = px_शिखरम्_सम्बन्धि_राशिः;
    px_शिखरम्_सम्बन्धि_राशिः = px_अस्थायी;
#यदि_अन्तः /*__clang_analyzer__ */
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

    UBaseType_t ux_राशिः_Pointer = (UBaseType_t)px_शिखरम्_सम्बन्धि_राशिः;
    configASSERT((ux_राशिः_Pointer & पत्तनम्_BYTE_ALIGNMENT_आवरणम्) == 0);

#यदि XCHAL_CP_अङ्कः > 0
    // Initialize the coprocessor save area. THIS MUST BE THE FIRST AREA due to access from _frxt_task_coproc_state()
    ux_राशिः_Pointer = ux_Initialise_राशिः_CPSA(ux_राशिः_Pointer);
    configASSERT((ux_राशिः_Pointer & पत्तनम्_BYTE_ALIGNMENT_आवरणम्) == 0);
#यदि_अन्तः /* XCHAL_CP_NUM > 0 */

    // Initialize the GCC TLS area
    uint32_t threadptr_reg_init;
    ux_राशिः_Pointer = ux_Initialise_राशिः_TLS(ux_राशिः_Pointer, &threadptr_reg_init);
    configASSERT((ux_राशिः_Pointer & पत्तनम्_BYTE_ALIGNMENT_आवरणम्) == 0);

    // Initialize the starting interrupt stack frame
    ux_राशिः_Pointer = ux_Initialise_राशिः_Frame(ux_राशिः_Pointer, px_संहिता, pvParameters, threadptr_reg_init);
    configASSERT((ux_राशिः_Pointer & पत्तनम्_BYTE_ALIGNMENT_आवरणम्) == 0);

    // Return the task's current stack pointer address which should point to the starting interrupt stack frame
    प्रत्यागम (राशिः_Type_t *)ux_राशिः_Pointer;
}

// ------------------- Hook Functions ----------------------

#अन्तर्भाव <stdlib.h>

#यदि ( config_CHECK_FOR_राशिः_अतिप्रवाहः > 0 )
शून्य  __attribute__((weak)) v_Application_राशिः_अतिप्रवाहः_Hook( TaskHandle_t xTask, वर्ण *pcTaskName )
{
#परिभाषय ERR_STR1 "***ERROR*** A stack overflow in task "
#परिभाषय ERR_STR2 " has been detected."
    ध्रुव वर्ण *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    वर्ण buf[परिमाण(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + परिमाण(ERR_STR2) + 1 /* null char */] = { 0 };

    वर्ण *dest = buf;
    प्रति (size_t i = 0 ; i < परिमाण(str) / परिमाण(str[0]); i++) {
        dest = वाक्यं_योजय(dest, str[i]);
    }
    esp_system_abort(buf);
}
#यदि_अन्तः

बाह्य शून्य esp_vApplicationIdleHook(शून्य);
#यदि CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK
/*
By default, the port uses vApplicationPassiveIdleHook() to run IDF style idle
hooks. However, users may also want to provide their own vApplicationPassiveIdleHook().
In this case, we use to -Wl,--wrap option to wrap the user provided vApplicationPassiveIdleHook()
*/
बाह्य शून्य __real_vApplicationPassiveIdleHook( शून्य );
शून्य __wrap_vApplicationPassiveIdleHook( शून्य )
{
    esp_vApplicationIdleHook(); //Run IDF style hooks
    __real_vApplicationPassiveIdleHook(); //Call the user provided vApplicationPassiveIdleHook()
}
#अन्यथा // CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK
शून्य vApplicationPassiveIdleHook( शून्य )
{
    esp_vApplicationIdleHook(); //Run IDF style hooks
}
#यदि_अन्तः // CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK

/* ------------------------------------------------ Run Time Stats ------------------------------------------------- */

#यदि ( CONFIG_FREERTOS_GENERATE_सततम्_TIME_STATS )

config_सततम्_TIME_गणक_TYPE x_पत्तनम्_Get_सततम्_Time_गणक_मानम्( शून्य )
{
#यदि_परिभाषितम् CONFIG_FREERTOS_सततम्_TIME_STATS_USING_ESP_TIMER
    प्रत्यागम (config_सततम्_TIME_गणक_TYPE) esp_timer_get_time();
#अन्यथा // Uses CCOUNT
    प्रत्यागम (config_सततम्_TIME_गणक_TYPE) xthal_get_ccount();
#यदि_अन्तः // CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
}

#यदि_अन्तः /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
