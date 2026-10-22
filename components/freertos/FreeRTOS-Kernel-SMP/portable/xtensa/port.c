/*
 * SPDX-FileCopyrightText: 2022-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "sdkconfig.h"
#ضمن <stdbool.h>
#ضمن <stdint.h>
#ضمن <string.h>
#ضمن "esp_compiler.h"
#ضمن "FreeRTOS.h"
#ضمن "task.h"   //For vApplicationStackOverflowHook
#ضمن "port_systick.h"
#ضمن "portmacro.h"
#ضمن "spinlock.h"
#ضمن "xt_instr_macros.h"
#ضمن "xtensa_context.h"
#ضمن "xtensa/corebits.h"
#ضمن "xtensa/config/core.h"
#ضمن "xtensa/config/core-isa.h"
#ضمن "xtensa/xtruntime.h"
#ضمن "esp_private/freertos_idf_additions_priv.h"
#ضمن "esp_private/esp_int_wdt.h"
#ضمن "esp_private/systimer.h"
#ضمن "esp_private/periph_ctrl.h"
#ضمن "esp_attr.h"
#ضمن "esp_heap_caps.h"
#ضمن "esp_system.h"
#ضمن "esp_task.h"
#ضمن "esp_log.h"
#ضمن "esp_cpu.h"
#ضمن "esp_rom_sys.h"
#ضمن "esp_task_wdt.h"
#ضمن "esp_heap_caps_init.h"
#ضمن "esp_freertos_hooks.h"
#ضمن "esp_intr_alloc.h"
#ضمن "esp_memory_utils.h"
#ضمن "esp_macros.h"
#ضمن <xtensa/hal.h>             /* required for xthal_get_ccount() */
#إذا CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
#ضمن "esp_timer.h"
#endif
#ifdef CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER
#ضمن "soc/periph_defs.h"
#ضمن "soc/system_reg.h"
#ضمن "hal/systimer_hal.h"
#ضمن "hal/systimer_ll.h"
#endif // CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER

_Static_assert(portBYTE_ALIGNMENT == 16, "portBYTE_ALIGNMENT must be set to 16");

/* ---------------------------------------------------- Variables ------------------------------------------------------
 * - Various variables used to maintain the FreeRTOS port's state. Used from both port.c and various .S files
 * - Constant offsets are used by assembly to jump to particular TCB members or a stack area (such as the CPSA). We use
 *   C constants instead of preprocessor macros due to assembly lacking "offsetof()".
 * ------------------------------------------------------------------------------------------------------------------ */

#إذا XCHAL_CP_رەقەم > 0
/* Offsets used to navigate to a task's CPSA on the stack */
ثابت DRAM_ATTR uint32_t offset_px_End_Of_دۆۋە = offsetof(StaticTask_t, pxDummy8);
ثابت DRAM_ATTR uint32_t offset_cpsa = XT_CP_SIZE;  /* Offset to start of the CPSA area on the stack. See uxInitialiseStackCPSA(). */
#إذا config_رەقەم_CORES > 1
/* Offset to TCB_t.uxCoreAffinityMask member. Used to pin unpinned tasks that use the FPU. */
ثابت DRAM_ATTR uint32_t offset_uxCoreAffinityMask = offsetof(StaticTask_t, uxDummy26);
#إذا configUSE_CORE_AFFINITY != 1
#error "configUSE_CORE_AFFINITY must be 1 on multicore targets with coprocessor support"
#endif
#endif /* configNUM_CORES > 1 */
#endif /* XCHAL_CP_NUM > 0 */

متطاير unsigned port_xSchedulerRunning[port_رەقەم_PROCESSORS] = {0}; // Indicates whether scheduler is running on a per-core basis
unsigned صحيح port_interruptNesting[port_رەقەم_PROCESSORS] = {0};  // Interrupt nesting level. Increased/decreased in portasm.c, _frxt_int_enter/_frxt_int_exit
متطاير unsigned port_uxCoreStartupDone[port_رەقەم_PROCESSORS] = {0};  // Indicates whether the core has completed its startup sequence
#إذا ( config_سان_OF_CORES > 1 )
//FreeRTOS SMP Locks
portMUX_TYPE port_xTaskLock = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE port_xISRLock = portMUX_INITIALIZER_UNLOCKED;
#endif /* configNUMBER_OF_CORES > 1 */

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

BaseType_t xPortInterruptedFromISRContext(فراغ)
{
    أرجع (port_interruptNesting[xPortGetCoreID()] != 0);
}

// ------------------ Critical Sections --------------------

/*
Variables used by IDF critical sections only (SMP tracks critical nesting inside TCB now)
[refactor-todo] Figure out how IDF critical sections will be merged with SMP FreeRTOS critical sections
*/
BaseType_t port_uxCriticalNestingIDF[port_رەقەم_PROCESSORS] = {0};
BaseType_t port_uxCriticalOldInterruptStateIDF[port_رەقەم_PROCESSORS] = {0};
#إذا CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
متطاير bool port_xThreadSafeClaimed = false;
#endif

/*
*******************************************************************************
* Interrupt stack. The size of the interrupt stack is determined by the config
* parameter "configISR_STACK_SIZE" in FreeRTOSConfig.h
*******************************************************************************
*/
متطاير دۆۋە_Type_t DRAM_ATTR __attribute__((aligned(16))) port_Int_دۆۋە[port_رەقەم_PROCESSORS][config_ISR_دۆۋە_SIZE];
/* One flag for each individual CPU. */
متطاير uint32_t port_switch_flag[port_رەقەم_PROCESSORS];

#إذا CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
فراغ xPortThreadSafeClaim(فراغ)
{
    configASSERT(!xPortCanYield());
    configASSERT(!port_xThreadSafeClaimed);
    port_xThreadSafeClaimed = true;
}

فراغ xPortThreadSafeDisclaim(فراغ)
{
    configASSERT(!xPortCanYield());
    configASSERT(port_xThreadSafeClaimed);
    port_xThreadSafeClaimed = false;
}
#endif /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

BaseType_t xPortEnterCriticalTimeout(portMUX_TYPE *lock, BaseType_t timeout)
{
#إذا CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
    إذا (unlikely(port_xThreadSafeClaimed)) {
        أرجع pdPASS;
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
    إذا (!spinlock_acquire_impl(lock, timeout, coreOwnerId)) {
        //Timed out attempting to get spinlock. Restore previous interrupt level and return
        XTOS_RESTORE_JUST_INTLEVEL((صحيح) xOldInterruptLevel);
        أرجع pdFAIL;
    }
    //Spinlock acquired. Increment the IDF critical nesting count.
    BaseType_t newNesting = port_uxCriticalNestingIDF[coreID] + 1;
    port_uxCriticalNestingIDF[coreID] = newNesting;
    //If this is the first entry to a critical section. Save the old interrupt level.
    إذا ( newNesting == 1 ) {
        port_uxCriticalOldInterruptStateIDF[coreID] = xOldInterruptLevel;
    }
    أرجع pdPASS;

}

فراغ vPortExitCriticalIDF(portMUX_TYPE *lock)
{
#إذا CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
    إذا (unlikely(port_xThreadSafeClaimed)) {
        أرجع;
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

    إذا (nesting > 0) {
        nesting--;
        port_uxCriticalNestingIDF[coreID] = nesting;

        //This is the last exit call, restore the saved interrupt level
        إذا ( nesting == 0 ) {
            XTOS_RESTORE_JUST_INTLEVEL((صحيح) port_uxCriticalOldInterruptStateIDF[coreID]);
        }
    }
}

/*
In case any IDF libs called the port critical functions directly instead of through the macros.
Just inline call the IDF versions
*/
فراغ vPortEnterCritical(portMUX_TYPE *lock)
{
    vPortEnterCriticalIDF(lock);
}

فراغ vPortExitCritical(portMUX_TYPE *lock)
{
    vPortExitCriticalIDF(lock);
}

// ----------------------- System --------------------------

#define دۆۋە_WATCH_POINT_سان (SOC_CPU_WATCHPOINTS_رەقەم - 1)

فراغ v_Port_Set_دۆۋە_Watchpoint( فراغ *px_دۆۋە_Start )
{
    //Set watchpoint 1 to watch the last 32 bytes of the stack.
    //Unfortunately, the Xtensa watchpoints can't set a watchpoint on a random [base - base+n] region because
    //the size works by masking off the lowest address bits. For that reason, we futz a bit and watch the lowest 32
    //bytes of the stack we can actually watch. In general, this can cause the watchpoint to be triggered at most
    //28 bytes early. The value 32 is chosen because it's larger than the stack canary, which in FreeRTOS is 20 bytes.
    //This way, we make sure we trigger before/when the stack canary is corrupted, not after.
    صحيح addr = (صحيح)px_دۆۋە_Start;
    addr = (addr + 31) & (~31);
    esp_cpu_set_watchpoint(دۆۋە_WATCH_POINT_سان, (حرف *)addr, 32, ESP_CPU_WATCHPOINT_STORE);
}



/* ---------------------------------------------- Port Implementations -------------------------------------------------
 * Implementations of Porting Interface functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

BaseType_t xPortCheckIfInISR(فراغ)
{
    //Disable interrupts so that reading port_interruptNesting is atomic
    BaseType_t ret;
    unsigned صحيح prev_int_level = portDISABLE_INTERRUPTS();
    ret = (port_interruptNesting[xPortGetCoreID()] != 0) ? pdTRUE : pdFALSE;
    portRESTORE_INTERRUPTS(prev_int_level);
    أرجع ret;
}

فراغ vPortAssertIfInISR(فراغ)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(xPortCheckIfInISR() == 0);
}

// ------------------ Critical Sections --------------------

#إذا ( config_سان_OF_CORES > 1 )
فراغ vPortTakeLock( portMUX_TYPE *lock )
{
    spinlock_acquire( lock, portMUX_NO_TIMEOUT);
}

فراغ vPortReleaseLock( portMUX_TYPE *lock )
{
    spinlock_release( lock );
}
#endif /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

// ----------------------- System --------------------------

// ------------------- Run Time Stats ----------------------

// --------------------- TCB Cleanup -----------------------

#إذا ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
ساكن فراغ vPortTLSPointersDelCb( فراغ *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pvThreadLocalStoragePointersDelCallback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_رەقەم_THREAD_LOCAL_STORAGE_POINTERS / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    لكل ( صحيح x = 0; x < ( config_رەقەم_THREAD_LOCAL_STORAGE_POINTERS / 2 ); x++ ) {
        إذا ( pvThreadLocalStoragePointersDelCallback[ x ] != عدم ) {  //If del cb is set
            /* In case the TLSP deletion callback has been overwritten by a TLS pointer, gracefully abort. */
            إذا ( !esp_ptr_executable( pvThreadLocalStoragePointersDelCallback[ x ] ) ) {
                // We call EARLY log here as currently portCLEAN_UP_TCB() is called in a critical section
                ESP_EARLY_LOGE("FreeRTOS", "Fatal error: TLSP deletion callback at index %d overwritten with non-excutable pointer %p", x, pvThreadLocalStoragePointersDelCallback[ x ]);
                abort();
            }

            pvThreadLocalStoragePointersDelCallback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#endif /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

#إذا ( XCHAL_CP_رەقەم > 0 && configUSE_CORE_AFFINITY == 1 && config_رەقەم_CORES > 1 )
ساكن فراغ vPortCleanUpCoprocArea( فراغ *pxTCB )
{
    UBaseType_t uxCoprocArea;
    BaseType_t xTargetCoreID;

    /* Get pointer to the task's coprocessor save area from TCB->pxEndOfStack. See uxInitialiseStackCPSA() */
    uxCoprocArea = ( UBaseType_t ) ( ( ( StaticTask_t * ) pxTCB )->pxDummy8 );  /* Get TCB_t.pxEndOfStack */
    uxCoprocArea = ESP_ALIGN_DOWN(uxCoprocArea - XT_CP_SIZE, 16);

    /* Extract core ID from the affinity mask */
    xTargetCoreID = ( ( StaticTask_t * ) pxTCB )->uxDummy26;
    xTargetCoreID = ( BaseType_t ) __builtin_ffs( ( صحيح ) xTargetCoreID );
    أكد( xTargetCoreID >= 1 ); // __builtin_ffs always returns first set index + 1
    xTargetCoreID -= 1;

    /* If task has live floating point registers somewhere, release them */
    فراغ _xt_coproc_release(متطاير فراغ *coproc_sa_base, BaseType_t xTargetCoreID);
    _xt_coproc_release( (فراغ *)uxCoprocArea, xTargetCoreID );
}
#endif /* ( XCHAL_CP_NUM > 0 && configUSE_CORE_AFFINITY == 1 && configNUM_CORES > 1 ) */

فراغ vPortTCBPreDeleteHook( فراغ *pxTCB )
{
    #إذا ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        خارجي فراغ vTaskPreDeletionHook( فراغ * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #endif /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

    #إذا ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
        /* Call TLS pointers deletion callbacks */
        vPortTLSPointersDelCb( pxTCB );
    #endif /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

    #إذا ( XCHAL_CP_رەقەم > 0 && configUSE_CORE_AFFINITY == 1 && config_رەقەم_CORES > 1 )
        /* Cleanup coproc save area */
        vPortCleanUpCoprocArea( pxTCB );
    #endif /* ( XCHAL_CP_NUM > 0 && configUSE_CORE_AFFINITY == 1 && configNUM_CORES > 1 ) */
}

/* ------------------------------------------------ FreeRTOS Portable --------------------------------------------------
 * - Provides implementation for functions required by FreeRTOS
 * - Declared in portable.h
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------- Scheduler Start/End -------------------

خارجي فراغ _xt_coproc_init(فراغ);

BaseType_t xPortStartScheduler( فراغ )
{
    portDISABLE_INTERRUPTS();
    // Interrupts are disabled at this point and stack contains PS with enabled interrupts when task context is restored

#إذا XCHAL_CP_رەقەم > 0
    /* Initialize co-processor management for tasks. Leave CPENABLE alone. */
    _xt_coproc_init();
#endif

    /* Setup the hardware to generate the tick. */
    vPortSetupTimer();

        /* Initialize all kernel state tracking variables */
    BaseType_t coreID = xPortGetCoreID();
    port_xSchedulerRunning[coreID] = 1;
    port_uxCoreStartupDone[coreID] = 0;
#إذا CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
    port_xThreadSafeClaimed = false;
#endif

#إذا config_رەقەم_CORES > 1
    // Workaround for non-thread safe multi-core OS startup (see IDF-4524)
    إذا (xPortGetCoreID() != 0) {
        prvStartSchedulerOtherCores();
    }
#endif // configNUM_CORES > 1

    // Windows contain references to the startup stack which will be reclaimed by the main task
    // Spill the windows to create a clean environment to ensure we do not carry over any such references
    // to invalid SPs which will cause problems if main_task does a windowoverflow to them
    xthal_window_spill();

    // Cannot be directly called from C; never returns
    __asm__ متطاير ("call0    _frxt_dispatch\n");

    /* Should not get here. */
    أرجع pdTRUE;
}

فراغ vPortEndScheduler( فراغ )
{
    ;
}

// ------------------------ Stack --------------------------

// User exception dispatcher when exiting
فراغ _xt_user_exit(فراغ);

#إذا CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
// Wrapper to allow task functions to return (increases stack overhead by 16 bytes)
ساكن فراغ vPortTaskWrapper(TaskFunction_t pxCode, فراغ *pvParameters)
{
    pxCode(pvParameters);
    //FreeRTOS tasks should not return. Log the task name and abort.
    حرف *pcTaskName = pcTaskGetName(عدم);
    ESP_LOGE("FreeRTOS", "FreeRTOS Task \"%s\" should not return, Aborting now!", pcTaskName);
    abort();
}
#endif

#إذا XCHAL_CP_رەقەم > 0
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_دۆۋە_CPSA(UBaseType_t ux_دۆۋە_Pointer)
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
    ux_دۆۋە_Pointer = ESP_ALIGN_DOWN(ux_دۆۋە_Pointer - XT_CP_SIZE, 16);
    // Initialize the coprocessor context switching flags.
    uint32_t *p = (uint32_t *)ux_دۆۋە_Pointer;
    p[0] = 0;   // Clear XT_CPENABLE and XT_CPSTORED
    p[1] = 0;   // Clear XT_CP_CS_ST
    // XT_CP_ASA points to the aligned start of the individual CP save areas (i.e., start of CP0 SA)
    p[2] = (uint32_t)ESP_ALIGN_UP((uint32_t)ux_دۆۋە_Pointer + 12, XCHAL_TOTAL_SA_ALIGN);
    أرجع ux_دۆۋە_Pointer;
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_دۆۋە_TLS(UBaseType_t ux_دۆۋە_Pointer, uint32_t *ret_threadptr_reg_init)
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
    خارجي صحيح _tls_section_alignment;
    خارجي حرف _thread_local_data_start, _thread_local_data_end;
    خارجي حرف _thread_local_bss_start, _thread_local_bss_end;
    ثابت uint32_t tls_data_size = (uint32_t)&_thread_local_data_end - (uint32_t)&_thread_local_data_start;
    ثابت uint32_t tls_bss_size = (uint32_t)&_thread_local_bss_end - (uint32_t)&_thread_local_bss_start;
    ثابت uint32_t tls_area_size = ESP_ALIGN_UP(tls_data_size + tls_bss_size, 16);
    // TODO: check that TLS area fits the stack

    // Allocate space for the TLS area on the stack. The area must be allocated at a 16-byte aligned address
    ux_دۆۋە_Pointer = ESP_ALIGN_DOWN(ux_دۆۋە_Pointer - (UBaseType_t)tls_area_size, 16);
    // Initialize the TLS data with the initialization values of each TLS variable
    memcpy((فراغ *)ux_دۆۋە_Pointer, &_thread_local_data_start, tls_data_size);
    // Initialize the TLS bss with zeroes
    memset((فراغ *)(ux_دۆۋە_Pointer + tls_data_size), 0, tls_bss_size);

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
    ثابت uint32_t tls_section_align = (uint32_t)&_tls_section_alignment;  // ALIGN value of .flash.tdata section
    #define TCB_SIZE 8
    ثابت uint32_t base = ESP_ALIGN_UP(TCB_SIZE, tls_section_align);
    *ret_threadptr_reg_init = (uint32_t)ux_دۆۋە_Pointer - base;

    أرجع ux_دۆۋە_Pointer;
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_دۆۋە_Frame(UBaseType_t ux_دۆۋە_Pointer, TaskFunction_t pxCode, فراغ *pvParameters, uint32_t threadptr_reg_init)
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
    UBaseType_t ux_دۆۋە_Pointer_Previous = ux_دۆۋە_Pointer;
    ux_دۆۋە_Pointer = ESP_ALIGN_DOWN(ux_دۆۋە_Pointer - XT_STK_FRMSZ, 16);

    // Clear the entire interrupt stack frame
    memset((فراغ *)ux_دۆۋە_Pointer, 0, (size_t)(ux_دۆۋە_Pointer_Previous - ux_دۆۋە_Pointer));

    XtExcFrame *frame = (XtExcFrame *)ux_دۆۋە_Pointer;

    /*
    Initialize common registers
    */
    frame->a0 = 0;                                          // Set the return address to 0 terminate GDB backtrace
    frame->a1 = ux_دۆۋە_Pointer + XT_STK_FRMSZ;              // Saved stack pointer should point to physical top of stack frame
    frame->exit = (UBaseType_t) _xt_user_exit;              // User exception exit dispatcher

    /*
    Initialize the task's entry point. This will differ depending on
    - Whether the task's entry point is the wrapper function or pxCode
    - Whether Windowed ABI is used (for windowed, we mimic the task entry point being call4'd )
    */
    #إذا CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
        frame->pc = (UBaseType_t) vPortTaskWrapper;         // Task entry point is the wrapper function
        #ifdef __XTENSA_CALL0_ABI__
            frame->a2 = (UBaseType_t) pxCode;               // Wrapper function's argument 0 (which is the task function)
            frame->a3 = (UBaseType_t) pvParameters;         // Wrapper function's argument 1 (which is the task function's argument)
        #وإلا // __XTENSA_CALL0_ABI__
            frame->a6 = (UBaseType_t) pxCode;               // Wrapper function's argument 0 (which is the task function), passed as if we call4'd
            frame->a7 = (UBaseType_t) pvParameters;         // Wrapper function's argument 1 (which is the task function's argument), passed as if we call4'd
        #endif // __XTENSA_CALL0_ABI__
    #وإلا
        frame->pc = (UBaseType_t) pxCode;                   // Task entry point is the provided task function
        #ifdef __XTENSA_CALL0_ABI__
            frame->a2 = (UBaseType_t) pvParameters;         // Task function's argument
        #وإلا // __XTENSA_CALL0_ABI__
            frame->a6 = (UBaseType_t) pvParameters;         // Task function's argument, passed as if we call4'd
        #endif // __XTENSA_CALL0_ABI__
    #endif

    /*
    Set initial PS to int level 0, EXCM disabled ('rfe' will enable), user mode.
    For windowed ABI also set WOE and CALLINC (pretend task was 'call4'd)
    */
    #ifdef __XTENSA_CALL0_ABI__
        frame->ps = PS_UM | PS_EXCM;
    #وإلا // __XTENSA_CALL0_ABI__
        frame->ps = PS_UM | PS_EXCM | PS_WOE | PS_CALLINC(1);
    #endif // __XTENSA_CALL0_ABI__

    #ifdef XT_USE_SWPRI
        // Set the initial virtual priority mask value to all 1's.
        frame->vpri = 0xFFFFFFFF;
    #endif

    // Initialize the threadptr register in the extra save area of the stack frame
    uint32_t *threadptr_reg = (uint32_t *)(ux_دۆۋە_Pointer + XT_STK_EXTRA);
    *threadptr_reg = threadptr_reg_init;

    أرجع ux_دۆۋە_Pointer;
}

#إذا ( port_HAS_دۆۋە_OVERFLOW_CHECKING == 1 )
دۆۋە_Type_t * px_Port_Initialise_دۆۋە( دۆۋە_Type_t * px_چوققا_Of_دۆۋە,
                                     دۆۋە_Type_t * px_End_Of_دۆۋە,
                                     TaskFunction_t pxCode,
                                     فراغ * pvParameters )
#وإلا
دۆۋە_Type_t * px_Port_Initialise_دۆۋە( دۆۋە_Type_t * px_چوققا_Of_دۆۋە,
                                     TaskFunction_t pxCode,
                                     فراغ * pvParameters )
#endif
{
#ifdef __clang_analyzer__
    // Teach clang-tidy that pxTopOfStack cannot be a pointer to const
    متطاير دۆۋە_Type_t * pxTemp = px_چوققا_Of_دۆۋە;
    px_چوققا_Of_دۆۋە = pxTemp;
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

    UBaseType_t ux_دۆۋە_Pointer = (UBaseType_t)px_چوققا_Of_دۆۋە;
    configASSERT((ux_دۆۋە_Pointer & portBYTE_ALIGNMENT_MASK) == 0);

#إذا XCHAL_CP_رەقەم > 0
    // Initialize the coprocessor save area. THIS MUST BE THE FIRST AREA due to access from _frxt_task_coproc_state()
    ux_دۆۋە_Pointer = ux_Initialise_دۆۋە_CPSA(ux_دۆۋە_Pointer);
    configASSERT((ux_دۆۋە_Pointer & portBYTE_ALIGNMENT_MASK) == 0);
#endif /* XCHAL_CP_NUM > 0 */

    // Initialize the GCC TLS area
    uint32_t threadptr_reg_init;
    ux_دۆۋە_Pointer = ux_Initialise_دۆۋە_TLS(ux_دۆۋە_Pointer, &threadptr_reg_init);
    configASSERT((ux_دۆۋە_Pointer & portBYTE_ALIGNMENT_MASK) == 0);

    // Initialize the starting interrupt stack frame
    ux_دۆۋە_Pointer = ux_Initialise_دۆۋە_Frame(ux_دۆۋە_Pointer, pxCode, pvParameters, threadptr_reg_init);
    configASSERT((ux_دۆۋە_Pointer & portBYTE_ALIGNMENT_MASK) == 0);

    // Return the task's current stack pointer address which should point to the starting interrupt stack frame
    أرجع (دۆۋە_Type_t *)ux_دۆۋە_Pointer;
}

// ------------------- Hook Functions ----------------------

#ضمن <stdlib.h>

#إذا ( config_CHECK_FOR_دۆۋە_OVERFLOW > 0 )
فراغ  __attribute__((weak)) v_Application_دۆۋە_Overflow_Hook( TaskHandle_t xTask, حرف *pcTaskName )
{
#define ERR_STR1 "***ERROR*** A stack overflow in task "
#define ERR_STR2 " has been detected."
    ثابت حرف *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    حرف buf[حجم(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + حجم(ERR_STR2) + 1 /* null char */] = { 0 };

    حرف *dest = buf;
    لكل (size_t i = 0 ; i < حجم(str) / حجم(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
    esp_system_abort(buf);
}
#endif

خارجي فراغ esp_vApplicationIdleHook(فراغ);
#إذا CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK
/*
By default, the port uses vApplicationPassiveIdleHook() to run IDF style idle
hooks. However, users may also want to provide their own vApplicationPassiveIdleHook().
In this case, we use to -Wl,--wrap option to wrap the user provided vApplicationPassiveIdleHook()
*/
خارجي فراغ __real_vApplicationPassiveIdleHook( فراغ );
فراغ __wrap_vApplicationPassiveIdleHook( فراغ )
{
    esp_vApplicationIdleHook(); //Run IDF style hooks
    __real_vApplicationPassiveIdleHook(); //Call the user provided vApplicationPassiveIdleHook()
}
#وإلا // CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK
فراغ vApplicationPassiveIdleHook( فراغ )
{
    esp_vApplicationIdleHook(); //Run IDF style hooks
}
#endif // CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK

/* ------------------------------------------------ Run Time Stats ------------------------------------------------- */

#إذا ( CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS )

configRUN_TIME_COUNTER_TYPE x_Port_Get_Run_Time_Counter_قىممەت( فراغ )
{
#ifdef CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
    أرجع (configRUN_TIME_COUNTER_TYPE) esp_timer_get_time();
#وإلا // Uses CCOUNT
    أرجع (configRUN_TIME_COUNTER_TYPE) xthal_get_ccount();
#endif // CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
}

#endif /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
