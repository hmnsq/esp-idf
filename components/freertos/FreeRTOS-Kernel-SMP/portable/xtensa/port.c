/*
 * SPDX-FileCopyrightText: 2022-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#অন্তর্ভাব "sdkconfig.h"
#অন্তর্ভাব <stdbool.h>
#অন্তর্ভাব <stdint.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব "esp_compiler.h"
#অন্তর্ভাব "FreeRTOS.h"
#অন্তর্ভাব "task.h"   //For vApplicationStackOverflowHook
#অন্তর্ভাব "port_systick.h"
#অন্তর্ভাব "portmacro.h"
#অন্তর্ভাব "spinlock.h"
#অন্তর্ভাব "xt_instr_macros.h"
#অন্তর্ভাব "xtensa_context.h"
#অন্তর্ভাব "xtensa/corebits.h"
#অন্তর্ভাব "xtensa/config/core.h"
#অন্তর্ভাব "xtensa/config/core-isa.h"
#অন্তর্ভাব "xtensa/xtruntime.h"
#অন্তর্ভাব "esp_private/freertos_idf_additions_priv.h"
#অন্তর্ভাব "esp_private/esp_int_wdt.h"
#অন্তর্ভাব "esp_private/systimer.h"
#অন্তর্ভাব "esp_private/periph_ctrl.h"
#অন্তর্ভাব "esp_attr.h"
#অন্তর্ভাব "esp_heap_caps.h"
#অন্তর্ভাব "esp_system.h"
#অন্তর্ভাব "esp_task.h"
#অন্তর্ভাব "esp_log.h"
#অন্তর্ভাব "esp_cpu.h"
#অন্তর্ভাব "esp_rom_sys.h"
#অন্তর্ভাব "esp_task_wdt.h"
#অন্তর্ভাব "esp_heap_caps_init.h"
#অন্তর্ভাব "esp_freertos_hooks.h"
#অন্তর্ভাব "esp_intr_alloc.h"
#অন্তর্ভাব "esp_memory_utils.h"
#অন্তর্ভাব "esp_macros.h"
#অন্তর্ভাব <xtensa/hal.h>             /* required for xthal_get_ccount() */
#যদি CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
#অন্তর্ভাব "esp_timer.h"
#endif
#ifdef CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER
#অন্তর্ভাব "soc/periph_defs.h"
#অন্তর্ভাব "soc/system_reg.h"
#অন্তর্ভাব "hal/systimer_hal.h"
#অন্তর্ভাব "hal/systimer_ll.h"
#endif // CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER

_Static_assert(portBYTE_ALIGNMENT == 16, "portBYTE_ALIGNMENT must be set to 16");

/* ---------------------------------------------------- Variables ------------------------------------------------------
 * - Various variables used to maintain the FreeRTOS port's state. Used from both port.c and various .S files
 * - Constant offsets are used by assembly to jump to particular TCB members or a stack area (such as the CPSA). We use
 *   C constants instead of preprocessor macros due to assembly lacking "offsetof()".
 * ------------------------------------------------------------------------------------------------------------------ */

#যদি XCHAL_CP_অঙ্ক > 0
/* Offsets used to navigate to a task's CPSA on the stack */
ধ্রুবক DRAM_ATTR uint32_t offset_px_End_Of_স্তূপ = offsetof(StaticTask_t, pxDummy8);
ধ্রুবক DRAM_ATTR uint32_t offset_cpsa = XT_CP_আকার;  /* Offset to start of the CPSA area on the stack. See uxInitialiseStackCPSA(). */
#যদি config_অঙ্ক_CORES > 1
/* Offset to TCB_t.uxCoreAffinityMask member. Used to pin unpinned tasks that use the FPU. */
ধ্রুবক DRAM_ATTR uint32_t offset_uxCoreAffinityMask = offsetof(StaticTask_t, uxDummy26);
#যদি configUSE_CORE_AFFINITY != 1
#error "configUSE_CORE_AFFINITY must be 1 on multicore targets with coprocessor support"
#endif
#endif /* configNUM_CORES > 1 */
#endif /* XCHAL_CP_NUM > 0 */

অস্থির unsigned port_xSchedulerRunning[port_অঙ্ক_PROCESSORS] = {0}; // Indicates whether scheduler is running on a per-core basis
unsigned পূর্ণসংখ্যা port_interruptNesting[port_অঙ্ক_PROCESSORS] = {0};  // Interrupt nesting level. Increased/decreased in portasm.c, _frxt_int_enter/_frxt_int_exit
অস্থির unsigned port_uxCoreStartupDone[port_অঙ্ক_PROCESSORS] = {0};  // Indicates whether the core has completed its startup sequence
#যদি ( config_সংখ্যা_OF_CORES > 1 )
//FreeRTOS SMP Locks
portMUX_TYPE port_xTaskLock = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE port_xISRLock = portMUX_INITIALIZER_UNLOCKED;
#endif /* configNUMBER_OF_CORES > 1 */

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

BaseType_t xPortInterruptedFromISRContext(শূন্য)
{
    ফেরত (port_interruptNesting[xPortGetCoreID()] != 0);
}

// ------------------ Critical Sections --------------------

/*
Variables used by IDF critical sections only (SMP tracks critical nesting inside TCB now)
[refactor-todo] Figure out how IDF critical sections will be merged with SMP FreeRTOS critical sections
*/
BaseType_t port_uxCriticalNestingIDF[port_অঙ্ক_PROCESSORS] = {0};
BaseType_t port_uxCriticalOldInterruptStateIDF[port_অঙ্ক_PROCESSORS] = {0};
#যদি CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
অস্থির bool port_xThreadSafeClaimed = false;
#endif

/*
*******************************************************************************
* Interrupt stack. The size of the interrupt stack is determined by the config
* parameter "configISR_STACK_SIZE" in FreeRTOSConfig.h
*******************************************************************************
*/
অস্থির স্তূপ_Type_t DRAM_ATTR __attribute__((aligned(16))) port_Int_স্তূপ[port_অঙ্ক_PROCESSORS][config_ISR_স্তূপ_আকার];
/* One flag for each individual CPU. */
অস্থির uint32_t port_switch_flag[port_অঙ্ক_PROCESSORS];

#যদি CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
শূন্য xPortThreadSafeClaim(শূন্য)
{
    configASSERT(!xPortCanYield());
    configASSERT(!port_xThreadSafeClaimed);
    port_xThreadSafeClaimed = true;
}

শূন্য xPortThreadSafeDisclaim(শূন্য)
{
    configASSERT(!xPortCanYield());
    configASSERT(port_xThreadSafeClaimed);
    port_xThreadSafeClaimed = false;
}
#endif /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

BaseType_t xPortEnterCriticalTimeout(portMUX_TYPE *lock, BaseType_t timeout)
{
#যদি CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
    যদি (unlikely(port_xThreadSafeClaimed)) {
        ফেরত pdPASS;
    }
#endif
    /* Interrupts may already be disabled (if this function is called in nested
     * manner). However, there's no atomic operation that will allow us to check,
     * thus we have to disable interrupts again anyways.
     *
     * However, if this is call is NOT nested (i.e., the first call to enter a
     * critical section), we will save the previous interrupt level so that the
     * saved level can be restored on the last call to exit the critical.
     */
    BaseType_t xOldInterruptLevel = XTOS_SET_INTLEVEL(XCHAL_EXCM_LEVEL);
    /* Interrupts are masked (to XCHAL_EXCM_LEVEL, the same level the spinlock uses),
     * so the core id is stable and the spinlock does not need to mask again. Read the
     * core id register once and reuse it for the owner id and the nesting index. */
    uint32_t coreOwnerId = spinlock_owner_id();
    BaseType_t coreID = spinlock_core_id_from_owner_id(coreOwnerId);
    যদি (!spinlock_acquire_impl(lock, timeout, coreOwnerId)) {
        //Timed out attempting to get spinlock. Restore previous interrupt level and return
        XTOS_RESTORE_JUST_INTLEVEL((পূর্ণসংখ্যা) xOldInterruptLevel);
        ফেরত pdFAIL;
    }
    //Spinlock acquired. Increment the IDF critical nesting count.
    BaseType_t newNesting = port_uxCriticalNestingIDF[coreID] + 1;
    port_uxCriticalNestingIDF[coreID] = newNesting;
    //If this is the first entry to a critical section. Save the old interrupt level.
    যদি ( newNesting == 1 ) {
        port_uxCriticalOldInterruptStateIDF[coreID] = xOldInterruptLevel;
    }
    ফেরত pdPASS;

}

শূন্য vPortExitCriticalIDF(portMUX_TYPE *lock)
{
#যদি CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
    যদি (unlikely(port_xThreadSafeClaimed)) {
        ফেরত;
    }
#endif
    /* This function may be called in a nested manner. Therefore, we only need
     * to re-enable interrupts if this is the last call to exit the critical. We
     * can use the nesting count to determine whether this is the last exit call.
     */
    /* Interrupts remain disabled for the whole critical section, so the core id is
     * stable and the spinlock does not need to mask again. Read the core id register
     * once and reuse it for the owner id and the nesting index. */
    uint32_t coreOwnerId = spinlock_owner_id();
    BaseType_t coreID = spinlock_core_id_from_owner_id(coreOwnerId);
    spinlock_release_impl(lock, coreOwnerId);
    BaseType_t nesting = port_uxCriticalNestingIDF[coreID];

    /* Critical section nesting count must never be negative */
    configASSERT( nesting > 0 );

    যদি (nesting > 0) {
        nesting--;
        port_uxCriticalNestingIDF[coreID] = nesting;

        //This is the last exit call, restore the saved interrupt level
        যদি ( nesting == 0 ) {
            XTOS_RESTORE_JUST_INTLEVEL((পূর্ণসংখ্যা) port_uxCriticalOldInterruptStateIDF[coreID]);
        }
    }
}

/*
In case any IDF libs called the port critical functions directly instead of through the macros.
Just inline call the IDF versions
*/
শূন্য vPortEnterCritical(portMUX_TYPE *lock)
{
    vPortEnterCriticalIDF(lock);
}

শূন্য vPortExitCritical(portMUX_TYPE *lock)
{
    vPortExitCriticalIDF(lock);
}

// ----------------------- System --------------------------

#define স্তূপ_WATCH_POINT_সংখ্যা (SOC_CPU_WATCHPOINTS_অঙ্ক - 1)

শূন্য v_Port_Set_স্তূপ_Watchpoint( শূন্য *px_স্তূপ_Start )
{
    //Set watchpoint 1 to watch the last 32 bytes of the stack.
    //Unfortunately, the Xtensa watchpoints can't set a watchpoint on a random [base - base+n] region because
    //the size works by masking off the lowest address bits. For that reason, we futz a bit and watch the lowest 32
    //bytes of the stack we can actually watch. In general, this can cause the watchpoint to be triggered at most
    //28 bytes early. The value 32 is chosen because it's larger than the stack canary, which in FreeRTOS is 20 bytes.
    //This way, we make sure we trigger before/when the stack canary is corrupted, not after.
    পূর্ণসংখ্যা addr = (পূর্ণসংখ্যা)px_স্তূপ_Start;
    addr = (addr + 31) & (~31);
    esp_cpu_set_watchpoint(স্তূপ_WATCH_POINT_সংখ্যা, (অক্ষর *)addr, 32, ESP_CPU_WATCHPOINT_STORE);
}



/* ---------------------------------------------- Port Implementations -------------------------------------------------
 * Implementations of Porting Interface functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

BaseType_t xPortCheckIfInISR(শূন্য)
{
    //Disable interrupts so that reading port_interruptNesting is atomic
    BaseType_t ret;
    unsigned পূর্ণসংখ্যা পূর্ব_int_level = portDISABLE_INTERRUPTS();
    ret = (port_interruptNesting[xPortGetCoreID()] != 0) ? pdTRUE : pdFALSE;
    portRESTORE_INTERRUPTS(পূর্ব_int_level);
    ফেরত ret;
}

শূন্য vPortAssertIfInISR(শূন্য)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(xPortCheckIfInISR() == 0);
}

// ------------------ Critical Sections --------------------

#যদি ( config_সংখ্যা_OF_CORES > 1 )
শূন্য vPortTakeLock( portMUX_TYPE *lock )
{
    spinlock_acquire( lock, portMUX_NO_TIMEOUT);
}

শূন্য vPortReleaseLock( portMUX_TYPE *lock )
{
    spinlock_release( lock );
}
#endif /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

// ----------------------- System --------------------------

// ------------------- Run Time Stats ----------------------

// --------------------- TCB Cleanup -----------------------

#যদি ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
স্থির শূন্য vPortTLSPointersDelCb( শূন্য *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pvThreadLocalStoragePointersDelCallback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_অঙ্ক_THREAD_LOCAL_STORAGE_POINTERS / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    জন্য ( পূর্ণসংখ্যা x = 0; x < ( config_অঙ্ক_THREAD_LOCAL_STORAGE_POINTERS / 2 ); x++ ) {
        যদি ( pvThreadLocalStoragePointersDelCallback[ x ] != ফাঁকা ) {  //If del cb is set
            /* In case the TLSP deletion callback has been overwritten by a TLS pointer, gracefully abort. */
            যদি ( !esp_ptr_executable( pvThreadLocalStoragePointersDelCallback[ x ] ) ) {
                // We call EARLY log here as currently portCLEAN_UP_TCB() is called in a critical section
                ESP_EARLY_LOGE("FreeRTOS", "Fatal error: TLSP deletion callback at index %d overwritten with non-excutable pointer %p", x, pvThreadLocalStoragePointersDelCallback[ x ]);
                abort();
            }

            pvThreadLocalStoragePointersDelCallback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#endif /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

#যদি ( XCHAL_CP_অঙ্ক > 0 && configUSE_CORE_AFFINITY == 1 && config_অঙ্ক_CORES > 1 )
স্থির শূন্য vPortCleanUpCoprocArea( শূন্য *pxTCB )
{
    UBaseType_t uxCoprocArea;
    BaseType_t xTargetCoreID;

    /* Get pointer to the task's coprocessor save area from TCB->pxEndOfStack. See uxInitialiseStackCPSA() */
    uxCoprocArea = ( UBaseType_t ) ( ( ( StaticTask_t * ) pxTCB )->pxDummy8 );  /* Get TCB_t.pxEndOfStack */
    uxCoprocArea = ESP_ALIGN_DOWN(uxCoprocArea - XT_CP_আকার, 16);

    /* Extract core ID from the affinity mask */
    xTargetCoreID = ( ( StaticTask_t * ) pxTCB )->uxDummy26;
    xTargetCoreID = ( BaseType_t ) __builtin_ffs( ( পূর্ণসংখ্যা ) xTargetCoreID );
    প্রতিজ্ঞা( xTargetCoreID >= 1 ); // __builtin_ffs always returns first set index + 1
    xTargetCoreID -= 1;

    /* If task has live floating point registers somewhere, release them */
    শূন্য _xt_coproc_release(অস্থির শূন্য *coproc_sa_base, BaseType_t xTargetCoreID);
    _xt_coproc_release( (শূন্য *)uxCoprocArea, xTargetCoreID );
}
#endif /* ( XCHAL_CP_NUM > 0 && configUSE_CORE_AFFINITY == 1 && configNUM_CORES > 1 ) */

শূন্য vPortTCBPreDeleteHook( শূন্য *pxTCB )
{
    #যদি ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        বাহ্যিক শূন্য vTaskPreDeletionHook( শূন্য * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #endif /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

    #যদি ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
        /* Call TLS pointers deletion callbacks */
        vPortTLSPointersDelCb( pxTCB );
    #endif /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

    #যদি ( XCHAL_CP_অঙ্ক > 0 && configUSE_CORE_AFFINITY == 1 && config_অঙ্ক_CORES > 1 )
        /* Cleanup coproc save area */
        vPortCleanUpCoprocArea( pxTCB );
    #endif /* ( XCHAL_CP_NUM > 0 && configUSE_CORE_AFFINITY == 1 && configNUM_CORES > 1 ) */
}

/* ------------------------------------------------ FreeRTOS Portable --------------------------------------------------
 * - Provides implementation for functions required by FreeRTOS
 * - Declared in portable.h
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------- Scheduler Start/End -------------------

বাহ্যিক শূন্য _xt_coproc_init(শূন্য);

BaseType_t xPortStartScheduler( শূন্য )
{
    portDISABLE_INTERRUPTS();
    // Interrupts are disabled at this point and stack contains PS with enabled interrupts when task context is restored

#যদি XCHAL_CP_অঙ্ক > 0
    /* Initialize co-processor management for tasks. Leave CPENABLE alone. */
    _xt_coproc_init();
#endif

    /* Setup the hardware to generate the tick. */
    vPortSetupTimer();

        /* Initialize all kernel state tracking variables */
    BaseType_t coreID = xPortGetCoreID();
    port_xSchedulerRunning[coreID] = 1;
    port_uxCoreStartupDone[coreID] = 0;
#যদি CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
    port_xThreadSafeClaimed = false;
#endif

#যদি config_অঙ্ক_CORES > 1
    // Workaround for non-thread safe multi-core OS startup (see IDF-4524)
    যদি (xPortGetCoreID() != 0) {
        prvStartSchedulerOtherCores();
    }
#endif // configNUM_CORES > 1

    // Windows contain references to the startup stack which will be reclaimed by the main task
    // Spill the windows to create a clean environment to ensure we do not carry over any such references
    // to invalid SPs which will cause problems if main_task does a windowoverflow to them
    xthal_window_spill();

    // Cannot be directly called from C; never returns
    __asm__ অস্থির ("call0    _frxt_dispatch\n");

    /* Should not get here. */
    ফেরত pdTRUE;
}

শূন্য vPortEndScheduler( শূন্য )
{
    ;
}

// ------------------------ Stack --------------------------

// User exception dispatcher when exiting
শূন্য _xt_user_exit(শূন্য);

#যদি CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
// Wrapper to allow task functions to return (increases stack overhead by 16 bytes)
স্থির শূন্য vPortTaskWrapper(TaskFunction_t pxCode, শূন্য *pvParameters)
{
    pxCode(pvParameters);
    //FreeRTOS tasks should not return. Log the task name and abort.
    অক্ষর *pcTaskName = pcTaskGetName(ফাঁকা);
    ESP_LOGE("FreeRTOS", "FreeRTOS Task \"%s\" should not return, Aborting now!", pcTaskName);
    abort();
}
#endif

#যদি XCHAL_CP_অঙ্ক > 0
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_স্তূপ_CPSA(UBaseType_t ux_স্তূপ_Pointer)
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
    ux_স্তূপ_Pointer = ESP_ALIGN_DOWN(ux_স্তূপ_Pointer - XT_CP_আকার, 16);
    // Initialize the coprocessor context switching flags.
    uint32_t *p = (uint32_t *)ux_স্তূপ_Pointer;
    p[0] = 0;   // Clear XT_CPENABLE and XT_CPSTORED
    p[1] = 0;   // Clear XT_CP_CS_ST
    // XT_CP_ASA points to the aligned start of the individual CP save areas (i.e., start of CP0 SA)
    p[2] = (uint32_t)ESP_ALIGN_UP((uint32_t)ux_স্তূপ_Pointer + 12, XCHAL_যোগ_SA_ALIGN);
    ফেরত ux_স্তূপ_Pointer;
}
#endif /* XCHAL_CP_NUM > 0 */

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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_স্তূপ_TLS(UBaseType_t ux_স্তূপ_Pointer, uint32_t *ret_threadptr_reg_init)
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
    বাহ্যিক পূর্ণসংখ্যা _tls_section_alignment;
    বাহ্যিক অক্ষর thread_local_তথ্য_start, thread_local_তথ্য_end;
    বাহ্যিক অক্ষর _thread_local_bss_start, _thread_local_bss_end;
    ধ্রুবক uint32_t tls_তথ্য_আকার = (uint32_t)&thread_local_তথ্য_end - (uint32_t)&thread_local_তথ্য_start;
    ধ্রুবক uint32_t tls_bss_আকার = (uint32_t)&_thread_local_bss_end - (uint32_t)&_thread_local_bss_start;
    ধ্রুবক uint32_t tls_area_আকার = ESP_ALIGN_UP(tls_তথ্য_আকার + tls_bss_আকার, 16);
    // TODO: check that TLS area fits the stack

    // Allocate space for the TLS area on the stack. The area must be allocated at a 16-byte aligned address
    ux_স্তূপ_Pointer = ESP_ALIGN_DOWN(ux_স্তূপ_Pointer - (UBaseType_t)tls_area_আকার, 16);
    // Initialize the TLS data with the initialization values of each TLS variable
    memcpy((শূন্য *)ux_স্তূপ_Pointer, &thread_local_তথ্য_start, tls_তথ্য_আকার);
    // Initialize the TLS bss with zeroes
    memset((শূন্য *)(ux_স্তূপ_Pointer + tls_তথ্য_আকার), 0, tls_bss_আকার);

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
    ধ্রুবক uint32_t tls_section_align = (uint32_t)&_tls_section_alignment;  // ALIGN value of .flash.tdata section
    #define TCB_আকার 8
    ধ্রুবক uint32_t base = ESP_ALIGN_UP(TCB_আকার, tls_section_align);
    *ret_threadptr_reg_init = (uint32_t)ux_স্তূপ_Pointer - base;

    ফেরত ux_স্তূপ_Pointer;
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_স্তূপ_Frame(UBaseType_t ux_স্তূপ_Pointer, TaskFunction_t pxCode, শূন্য *pvParameters, uint32_t threadptr_reg_init)
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
    UBaseType_t ux_স্তূপ_Pointer_Previous = ux_স্তূপ_Pointer;
    ux_স্তূপ_Pointer = ESP_ALIGN_DOWN(ux_স্তূপ_Pointer - XT_STK_FRMSZ, 16);

    // Clear the entire interrupt stack frame
    memset((শূন্য *)ux_স্তূপ_Pointer, 0, (size_t)(ux_স্তূপ_Pointer_Previous - ux_স্তূপ_Pointer));

    XtExcFrame *frame = (XtExcFrame *)ux_স্তূপ_Pointer;

    /*
    Initialize common registers
    */
    frame->a0 = 0;                                          // Set the return address to 0 terminate GDB backtrace
    frame->a1 = ux_স্তূপ_Pointer + XT_STK_FRMSZ;              // Saved stack pointer should point to physical top of stack frame
    frame->exit = (UBaseType_t) _xt_user_exit;              // User exception exit dispatcher

    /*
    Initialize the task's entry point. This will differ depending on
    - Whether the task's entry point is the wrapper function or pxCode
    - Whether Windowed ABI is used (for windowed, we mimic the task entry point being call4'd )
    */
    #যদি CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
        frame->pc = (UBaseType_t) vPortTaskWrapper;         // Task entry point is the wrapper function
        #ifdef __XTENSA_CALL0_ABI__
            frame->a2 = (UBaseType_t) pxCode;               // Wrapper function's argument 0 (which is the task function)
            frame->a3 = (UBaseType_t) pvParameters;         // Wrapper function's argument 1 (which is the task function's argument)
        #অন্যথা // __XTENSA_CALL0_ABI__
            frame->a6 = (UBaseType_t) pxCode;               // Wrapper function's argument 0 (which is the task function), passed as if we call4'd
            frame->a7 = (UBaseType_t) pvParameters;         // Wrapper function's argument 1 (which is the task function's argument), passed as if we call4'd
        #endif // __XTENSA_CALL0_ABI__
    #অন্যথা
        frame->pc = (UBaseType_t) pxCode;                   // Task entry point is the provided task function
        #ifdef __XTENSA_CALL0_ABI__
            frame->a2 = (UBaseType_t) pvParameters;         // Task function's argument
        #অন্যথা // __XTENSA_CALL0_ABI__
            frame->a6 = (UBaseType_t) pvParameters;         // Task function's argument, passed as if we call4'd
        #endif // __XTENSA_CALL0_ABI__
    #endif

    /*
    Set initial PS to int level 0, EXCM disabled ('rfe' will enable), user mode.
    For windowed ABI also set WOE and CALLINC (pretend task was 'call4'd)
    */
    #ifdef __XTENSA_CALL0_ABI__
        frame->ps = PS_UM | PS_EXCM;
    #অন্যথা // __XTENSA_CALL0_ABI__
        frame->ps = PS_UM | PS_EXCM | PS_WOE | PS_CALLINC(1);
    #endif // __XTENSA_CALL0_ABI__

    #ifdef XT_USE_SWPRI
        // Set the initial virtual priority mask value to all 1's.
        frame->vpri = 0xFFFFFFFF;
    #endif

    // Initialize the threadptr register in the extra save area of the stack frame
    uint32_t *threadptr_reg = (uint32_t *)(ux_স্তূপ_Pointer + XT_STK_EXTRA);
    *threadptr_reg = threadptr_reg_init;

    ফেরত ux_স্তূপ_Pointer;
}

#যদি ( port_HAS_স্তূপ_OVERFLOW_CHECKING == 1 )
স্তূপ_Type_t * px_Port_Initialise_স্তূপ( স্তূপ_Type_t * px_শীর্ষ_Of_স্তূপ,
                                     স্তূপ_Type_t * px_End_Of_স্তূপ,
                                     TaskFunction_t pxCode,
                                     শূন্য * pvParameters )
#অন্যথা
স্তূপ_Type_t * px_Port_Initialise_স্তূপ( স্তূপ_Type_t * px_শীর্ষ_Of_স্তূপ,
                                     TaskFunction_t pxCode,
                                     শূন্য * pvParameters )
#endif
{
#ifdef __clang_analyzer__
    // Teach clang-tidy that pxTopOfStack cannot be a pointer to const
    অস্থির স্তূপ_Type_t * px_অস্থায়ী = px_শীর্ষ_Of_স্তূপ;
    px_শীর্ষ_Of_স্তূপ = px_অস্থায়ী;
#endif /*__clang_analyzer__ */
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

    UBaseType_t ux_স্তূপ_Pointer = (UBaseType_t)px_শীর্ষ_Of_স্তূপ;
    configASSERT((ux_স্তূপ_Pointer & portBYTE_ALIGNMENT_MASK) == 0);

#যদি XCHAL_CP_অঙ্ক > 0
    // Initialize the coprocessor save area. THIS MUST BE THE FIRST AREA due to access from _frxt_task_coproc_state()
    ux_স্তূপ_Pointer = ux_Initialise_স্তূপ_CPSA(ux_স্তূপ_Pointer);
    configASSERT((ux_স্তূপ_Pointer & portBYTE_ALIGNMENT_MASK) == 0);
#endif /* XCHAL_CP_NUM > 0 */

    // Initialize the GCC TLS area
    uint32_t threadptr_reg_init;
    ux_স্তূপ_Pointer = ux_Initialise_স্তূপ_TLS(ux_স্তূপ_Pointer, &threadptr_reg_init);
    configASSERT((ux_স্তূপ_Pointer & portBYTE_ALIGNMENT_MASK) == 0);

    // Initialize the starting interrupt stack frame
    ux_স্তূপ_Pointer = ux_Initialise_স্তূপ_Frame(ux_স্তূপ_Pointer, pxCode, pvParameters, threadptr_reg_init);
    configASSERT((ux_স্তূপ_Pointer & portBYTE_ALIGNMENT_MASK) == 0);

    // Return the task's current stack pointer address which should point to the starting interrupt stack frame
    ফেরত (স্তূপ_Type_t *)ux_স্তূপ_Pointer;
}

// ------------------- Hook Functions ----------------------

#অন্তর্ভাব <stdlib.h>

#যদি ( config_CHECK_FOR_স্তূপ_OVERFLOW > 0 )
শূন্য  __attribute__((weak)) v_Application_স্তূপ_Overflow_Hook( TaskHandle_t xTask, অক্ষর *pcTaskName )
{
#define ERR_STR1 "***ERROR*** A stack overflow in task "
#define ERR_STR2 " has been detected."
    ধ্রুবক অক্ষর *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    অক্ষর buf[আকার(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + আকার(ERR_STR2) + 1 /* null char */] = { 0 };

    অক্ষর *dest = buf;
    জন্য (size_t i = 0 ; i < আকার(str) / আকার(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
    esp_system_abort(buf);
}
#endif

বাহ্যিক শূন্য esp_vApplicationIdleHook(শূন্য);
#যদি CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK
/*
By default, the port uses vApplicationPassiveIdleHook() to run IDF style idle
hooks. However, users may also want to provide their own vApplicationPassiveIdleHook().
In this case, we use to -Wl,--wrap option to wrap the user provided vApplicationPassiveIdleHook()
*/
বাহ্যিক শূন্য __real_vApplicationPassiveIdleHook( শূন্য );
শূন্য __wrap_vApplicationPassiveIdleHook( শূন্য )
{
    esp_vApplicationIdleHook(); //Run IDF style hooks
    __real_vApplicationPassiveIdleHook(); //Call the user provided vApplicationPassiveIdleHook()
}
#অন্যথা // CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK
শূন্য vApplicationPassiveIdleHook( শূন্য )
{
    esp_vApplicationIdleHook(); //Run IDF style hooks
}
#endif // CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK

/* ------------------------------------------------ Run Time Stats ------------------------------------------------- */

#যদি ( CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS )

config_RUN_TIME_গণক_TYPE x_Port_Get_Run_Time_গণক_মান( শূন্য )
{
#ifdef CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
    ফেরত (config_RUN_TIME_গণক_TYPE) esp_timer_get_time();
#অন্যথা // Uses CCOUNT
    ফেরত (config_RUN_TIME_গণক_TYPE) xthal_get_ccount();
#endif // CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
}

#endif /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
