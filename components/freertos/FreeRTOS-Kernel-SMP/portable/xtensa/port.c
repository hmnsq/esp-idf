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
#यदि CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
#अन्तर्भाव "esp_timer.h"
#endif
#ifdef CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER
#अन्तर्भाव "soc/periph_defs.h"
#अन्तर्भाव "soc/system_reg.h"
#अन्तर्भाव "hal/systimer_hal.h"
#अन्तर्भाव "hal/systimer_ll.h"
#endif // CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER

_Static_assert(portBYTE_ALIGNMENT == 16, "portBYTE_ALIGNMENT must be set to 16");

/* ---------------------------------------------------- Variables ------------------------------------------------------
 * - Various variables used to maintain the FreeRTOS port's state. Used from both port.c and various .S files
 * - Constant offsets are used by assembly to jump to particular TCB members or a stack area (such as the CPSA). We use
 *   C constants instead of preprocessor macros due to assembly lacking "offsetof()".
 * ------------------------------------------------------------------------------------------------------------------ */

#यदि XCHAL_CP_अंक > 0
/* Offsets used to navigate to a task's CPSA on the stack */
ध्रुव DRAM_ATTR uint32_t offset_px_End_Of_चळत = offsetof(StaticTask_t, pxDummy8);
ध्रुव DRAM_ATTR uint32_t offset_cpsa = XT_CP_SIZE;  /* Offset to start of the CPSA area on the stack. See uxInitialiseStackCPSA(). */
#यदि config_अंक_CORES > 1
/* Offset to TCB_t.uxCoreAffinityMask member. Used to pin unpinned tasks that use the FPU. */
ध्रुव DRAM_ATTR uint32_t offset_uxCoreAffinityMask = offsetof(StaticTask_t, uxDummy26);
#यदि configUSE_CORE_AFFINITY != 1
#error "configUSE_CORE_AFFINITY must be 1 on multicore targets with coprocessor support"
#endif
#endif /* configNUM_CORES > 1 */
#endif /* XCHAL_CP_NUM > 0 */

चञ्चल unsigned port_xSchedulerRunning[port_अंक_PROCESSORS] = {0}; // Indicates whether scheduler is running on a per-core basis
unsigned पूर्णाङ्क port_interruptNesting[port_अंक_PROCESSORS] = {0};  // Interrupt nesting level. Increased/decreased in portasm.c, _frxt_int_enter/_frxt_int_exit
चञ्चल unsigned port_uxCoreStartupDone[port_अंक_PROCESSORS] = {0};  // Indicates whether the core has completed its startup sequence
#यदि ( config_संख्या_OF_CORES > 1 )
//FreeRTOS SMP Locks
portMUX_TYPE port_xTaskLock = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE port_xISRLock = portMUX_INITIALIZER_UNLOCKED;
#endif /* configNUMBER_OF_CORES > 1 */

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

BaseType_t xPortInterruptedFromISRContext(शून्य)
{
    प्रत्यागम (port_interruptNesting[xPortGetCoreID()] != 0);
}

// ------------------ Critical Sections --------------------

/*
Variables used by IDF critical sections only (SMP tracks critical nesting inside TCB now)
[refactor-todo] Figure out how IDF critical sections will be merged with SMP FreeRTOS critical sections
*/
BaseType_t port_uxCriticalNestingIDF[port_अंक_PROCESSORS] = {0};
BaseType_t port_uxCriticalOldInterruptStateIDF[port_अंक_PROCESSORS] = {0};
#यदि CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
चञ्चल bool port_xThreadSafeClaimed = false;
#endif

/*
*******************************************************************************
* Interrupt stack. The size of the interrupt stack is determined by the config
* parameter "configISR_STACK_SIZE" in FreeRTOSConfig.h
*******************************************************************************
*/
चञ्चल चळत_Type_t DRAM_ATTR __attribute__((aligned(16))) port_Int_चळत[port_अंक_PROCESSORS][config_ISR_चळत_SIZE];
/* One flag for each individual CPU. */
चञ्चल uint32_t port_switch_flag[port_अंक_PROCESSORS];

#यदि CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
शून्य xPortThreadSafeClaim(शून्य)
{
    configASSERT(!xPortCanYield());
    configASSERT(!port_xThreadSafeClaimed);
    port_xThreadSafeClaimed = true;
}

शून्य xPortThreadSafeDisclaim(शून्य)
{
    configASSERT(!xPortCanYield());
    configASSERT(port_xThreadSafeClaimed);
    port_xThreadSafeClaimed = false;
}
#endif /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

BaseType_t xPortEnterCriticalTimeout(portMUX_TYPE *lock, BaseType_t timeout)
{
#यदि CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
    यदि (unlikely(port_xThreadSafeClaimed)) {
        प्रत्यागम pdPASS;
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
    यदि (!spinlock_acquire_impl(lock, timeout, coreOwnerId)) {
        //Timed out attempting to get spinlock. Restore previous interrupt level and return
        XTOS_RESTORE_JUST_INTLEVEL((पूर्णाङ्क) xOldInterruptLevel);
        प्रत्यागम pdFAIL;
    }
    //Spinlock acquired. Increment the IDF critical nesting count.
    BaseType_t newNesting = port_uxCriticalNestingIDF[coreID] + 1;
    port_uxCriticalNestingIDF[coreID] = newNesting;
    //If this is the first entry to a critical section. Save the old interrupt level.
    यदि ( newNesting == 1 ) {
        port_uxCriticalOldInterruptStateIDF[coreID] = xOldInterruptLevel;
    }
    प्रत्यागम pdPASS;

}

शून्य vPortExitCriticalIDF(portMUX_TYPE *lock)
{
#यदि CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
    यदि (unlikely(port_xThreadSafeClaimed)) {
        प्रत्यागम;
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

    यदि (nesting > 0) {
        nesting--;
        port_uxCriticalNestingIDF[coreID] = nesting;

        //This is the last exit call, restore the saved interrupt level
        यदि ( nesting == 0 ) {
            XTOS_RESTORE_JUST_INTLEVEL((पूर्णाङ्क) port_uxCriticalOldInterruptStateIDF[coreID]);
        }
    }
}

/*
In case any IDF libs called the port critical functions directly instead of through the macros.
Just inline call the IDF versions
*/
शून्य vPortEnterCritical(portMUX_TYPE *lock)
{
    vPortEnterCriticalIDF(lock);
}

शून्य vPortExitCritical(portMUX_TYPE *lock)
{
    vPortExitCriticalIDF(lock);
}

// ----------------------- System --------------------------

#define चळत_WATCH_POINT_संख्या (SOC_CPU_WATCHPOINTS_अंक - 1)

शून्य v_Port_Set_चळत_Watchpoint( शून्य *px_चळत_Start )
{
    //Set watchpoint 1 to watch the last 32 bytes of the stack.
    //Unfortunately, the Xtensa watchpoints can't set a watchpoint on a random [base - base+n] region because
    //the size works by masking off the lowest address bits. For that reason, we futz a bit and watch the lowest 32
    //bytes of the stack we can actually watch. In general, this can cause the watchpoint to be triggered at most
    //28 bytes early. The value 32 is chosen because it's larger than the stack canary, which in FreeRTOS is 20 bytes.
    //This way, we make sure we trigger before/when the stack canary is corrupted, not after.
    पूर्णाङ्क addr = (पूर्णाङ्क)px_चळत_Start;
    addr = (addr + 31) & (~31);
    esp_cpu_set_watchpoint(चळत_WATCH_POINT_संख्या, (वर्ण *)addr, 32, ESP_CPU_WATCHPOINT_STORE);
}



/* ---------------------------------------------- Port Implementations -------------------------------------------------
 * Implementations of Porting Interface functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

BaseType_t xPortCheckIfInISR(शून्य)
{
    //Disable interrupts so that reading port_interruptNesting is atomic
    BaseType_t ret;
    unsigned पूर्णाङ्क prev_int_level = portDISABLE_INTERRUPTS();
    ret = (port_interruptNesting[xPortGetCoreID()] != 0) ? pdTRUE : pdFALSE;
    portRESTORE_INTERRUPTS(prev_int_level);
    प्रत्यागम ret;
}

शून्य vPortAssertIfInISR(शून्य)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(xPortCheckIfInISR() == 0);
}

// ------------------ Critical Sections --------------------

#यदि ( config_संख्या_OF_CORES > 1 )
शून्य vPortTakeLock( portMUX_TYPE *lock )
{
    spinlock_acquire( lock, portMUX_NO_TIMEOUT);
}

शून्य vPortReleaseLock( portMUX_TYPE *lock )
{
    spinlock_release( lock );
}
#endif /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

// ----------------------- System --------------------------

// ------------------- Run Time Stats ----------------------

// --------------------- TCB Cleanup -----------------------

#यदि ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
स्थिर शून्य vPortTLSPointersDelCb( शून्य *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pvThreadLocalStoragePointersDelCallback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_अंक_THREAD_LOCAL_STORAGE_POINTERS / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    प्रति ( पूर्णाङ्क x = 0; x < ( config_अंक_THREAD_LOCAL_STORAGE_POINTERS / 2 ); x++ ) {
        यदि ( pvThreadLocalStoragePointersDelCallback[ x ] != रिक्त ) {  //If del cb is set
            /* In case the TLSP deletion callback has been overwritten by a TLS pointer, gracefully abort. */
            यदि ( !esp_ptr_executable( pvThreadLocalStoragePointersDelCallback[ x ] ) ) {
                // We call EARLY log here as currently portCLEAN_UP_TCB() is called in a critical section
                ESP_EARLY_LOGE("FreeRTOS", "Fatal error: TLSP deletion callback at index %d overwritten with non-excutable pointer %p", x, pvThreadLocalStoragePointersDelCallback[ x ]);
                abort();
            }

            pvThreadLocalStoragePointersDelCallback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#endif /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

#यदि ( XCHAL_CP_अंक > 0 && configUSE_CORE_AFFINITY == 1 && config_अंक_CORES > 1 )
स्थिर शून्य vPortCleanUpCoprocArea( शून्य *pxTCB )
{
    UBaseType_t uxCoprocArea;
    BaseType_t xTargetCoreID;

    /* Get pointer to the task's coprocessor save area from TCB->pxEndOfStack. See uxInitialiseStackCPSA() */
    uxCoprocArea = ( UBaseType_t ) ( ( ( StaticTask_t * ) pxTCB )->pxDummy8 );  /* Get TCB_t.pxEndOfStack */
    uxCoprocArea = ESP_ALIGN_DOWN(uxCoprocArea - XT_CP_SIZE, 16);

    /* Extract core ID from the affinity mask */
    xTargetCoreID = ( ( StaticTask_t * ) pxTCB )->uxDummy26;
    xTargetCoreID = ( BaseType_t ) __builtin_ffs( ( पूर्णाङ्क ) xTargetCoreID );
    प्रतिज्ञा( xTargetCoreID >= 1 ); // __builtin_ffs always returns first set index + 1
    xTargetCoreID -= 1;

    /* If task has live floating point registers somewhere, release them */
    शून्य _xt_coproc_release(चञ्चल शून्य *coproc_sa_base, BaseType_t xTargetCoreID);
    _xt_coproc_release( (शून्य *)uxCoprocArea, xTargetCoreID );
}
#endif /* ( XCHAL_CP_NUM > 0 && configUSE_CORE_AFFINITY == 1 && configNUM_CORES > 1 ) */

शून्य vPortTCBPreDeleteHook( शून्य *pxTCB )
{
    #यदि ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        बाह्य शून्य vTaskPreDeletionHook( शून्य * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #endif /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

    #यदि ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
        /* Call TLS pointers deletion callbacks */
        vPortTLSPointersDelCb( pxTCB );
    #endif /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

    #यदि ( XCHAL_CP_अंक > 0 && configUSE_CORE_AFFINITY == 1 && config_अंक_CORES > 1 )
        /* Cleanup coproc save area */
        vPortCleanUpCoprocArea( pxTCB );
    #endif /* ( XCHAL_CP_NUM > 0 && configUSE_CORE_AFFINITY == 1 && configNUM_CORES > 1 ) */
}

/* ------------------------------------------------ FreeRTOS Portable --------------------------------------------------
 * - Provides implementation for functions required by FreeRTOS
 * - Declared in portable.h
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------- Scheduler Start/End -------------------

बाह्य शून्य _xt_coproc_init(शून्य);

BaseType_t xPortStartScheduler( शून्य )
{
    portDISABLE_INTERRUPTS();
    // Interrupts are disabled at this point and stack contains PS with enabled interrupts when task context is restored

#यदि XCHAL_CP_अंक > 0
    /* Initialize co-processor management for tasks. Leave CPENABLE alone. */
    _xt_coproc_init();
#endif

    /* Setup the hardware to generate the tick. */
    vPortSetupTimer();

        /* Initialize all kernel state tracking variables */
    BaseType_t coreID = xPortGetCoreID();
    port_xSchedulerRunning[coreID] = 1;
    port_uxCoreStartupDone[coreID] = 0;
#यदि CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
    port_xThreadSafeClaimed = false;
#endif

#यदि config_अंक_CORES > 1
    // Workaround for non-thread safe multi-core OS startup (see IDF-4524)
    यदि (xPortGetCoreID() != 0) {
        prvStartSchedulerOtherCores();
    }
#endif // configNUM_CORES > 1

    // Windows contain references to the startup stack which will be reclaimed by the main task
    // Spill the windows to create a clean environment to ensure we do not carry over any such references
    // to invalid SPs which will cause problems if main_task does a windowoverflow to them
    xthal_window_spill();

    // Cannot be directly called from C; never returns
    __asm__ चञ्चल ("call0    _frxt_dispatch\n");

    /* Should not get here. */
    प्रत्यागम pdTRUE;
}

शून्य vPortEndScheduler( शून्य )
{
    ;
}

// ------------------------ Stack --------------------------

// User exception dispatcher when exiting
शून्य _xt_user_exit(शून्य);

#यदि CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
// Wrapper to allow task functions to return (increases stack overhead by 16 bytes)
स्थिर शून्य vPortTaskWrapper(TaskFunction_t pxCode, शून्य *pvParameters)
{
    pxCode(pvParameters);
    //FreeRTOS tasks should not return. Log the task name and abort.
    वर्ण *pcTaskName = pcTaskGetName(रिक्त);
    ESP_LOGE("FreeRTOS", "FreeRTOS Task \"%s\" should not return, Aborting now!", pcTaskName);
    abort();
}
#endif

#यदि XCHAL_CP_अंक > 0
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_चळत_CPSA(UBaseType_t ux_चळत_Pointer)
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
    ux_चळत_Pointer = ESP_ALIGN_DOWN(ux_चळत_Pointer - XT_CP_SIZE, 16);
    // Initialize the coprocessor context switching flags.
    uint32_t *p = (uint32_t *)ux_चळत_Pointer;
    p[0] = 0;   // Clear XT_CPENABLE and XT_CPSTORED
    p[1] = 0;   // Clear XT_CP_CS_ST
    // XT_CP_ASA points to the aligned start of the individual CP save areas (i.e., start of CP0 SA)
    p[2] = (uint32_t)ESP_ALIGN_UP((uint32_t)ux_चळत_Pointer + 12, XCHAL_TOTAL_SA_ALIGN);
    प्रत्यागम ux_चळत_Pointer;
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_चळत_TLS(UBaseType_t ux_चळत_Pointer, uint32_t *ret_threadptr_reg_init)
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
    बाह्य वर्ण _thread_local_data_start, _thread_local_data_end;
    बाह्य वर्ण _thread_local_bss_start, _thread_local_bss_end;
    ध्रुव uint32_t tls_data_size = (uint32_t)&_thread_local_data_end - (uint32_t)&_thread_local_data_start;
    ध्रुव uint32_t tls_bss_size = (uint32_t)&_thread_local_bss_end - (uint32_t)&_thread_local_bss_start;
    ध्रुव uint32_t tls_area_size = ESP_ALIGN_UP(tls_data_size + tls_bss_size, 16);
    // TODO: check that TLS area fits the stack

    // Allocate space for the TLS area on the stack. The area must be allocated at a 16-byte aligned address
    ux_चळत_Pointer = ESP_ALIGN_DOWN(ux_चळत_Pointer - (UBaseType_t)tls_area_size, 16);
    // Initialize the TLS data with the initialization values of each TLS variable
    memcpy((शून्य *)ux_चळत_Pointer, &_thread_local_data_start, tls_data_size);
    // Initialize the TLS bss with zeroes
    memset((शून्य *)(ux_चळत_Pointer + tls_data_size), 0, tls_bss_size);

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
    #define TCB_SIZE 8
    ध्रुव uint32_t base = ESP_ALIGN_UP(TCB_SIZE, tls_section_align);
    *ret_threadptr_reg_init = (uint32_t)ux_चळत_Pointer - base;

    प्रत्यागम ux_चळत_Pointer;
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_चळत_Frame(UBaseType_t ux_चळत_Pointer, TaskFunction_t pxCode, शून्य *pvParameters, uint32_t threadptr_reg_init)
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
    UBaseType_t ux_चळत_Pointer_Previous = ux_चळत_Pointer;
    ux_चळत_Pointer = ESP_ALIGN_DOWN(ux_चळत_Pointer - XT_STK_FRMSZ, 16);

    // Clear the entire interrupt stack frame
    memset((शून्य *)ux_चळत_Pointer, 0, (size_t)(ux_चळत_Pointer_Previous - ux_चळत_Pointer));

    XtExcFrame *frame = (XtExcFrame *)ux_चळत_Pointer;

    /*
    Initialize common registers
    */
    frame->a0 = 0;                                          // Set the return address to 0 terminate GDB backtrace
    frame->a1 = ux_चळत_Pointer + XT_STK_FRMSZ;              // Saved stack pointer should point to physical top of stack frame
    frame->exit = (UBaseType_t) _xt_user_exit;              // User exception exit dispatcher

    /*
    Initialize the task's entry point. This will differ depending on
    - Whether the task's entry point is the wrapper function or pxCode
    - Whether Windowed ABI is used (for windowed, we mimic the task entry point being call4'd )
    */
    #यदि CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
        frame->pc = (UBaseType_t) vPortTaskWrapper;         // Task entry point is the wrapper function
        #ifdef __XTENSA_CALL0_ABI__
            frame->a2 = (UBaseType_t) pxCode;               // Wrapper function's argument 0 (which is the task function)
            frame->a3 = (UBaseType_t) pvParameters;         // Wrapper function's argument 1 (which is the task function's argument)
        #अन्यथा // __XTENSA_CALL0_ABI__
            frame->a6 = (UBaseType_t) pxCode;               // Wrapper function's argument 0 (which is the task function), passed as if we call4'd
            frame->a7 = (UBaseType_t) pvParameters;         // Wrapper function's argument 1 (which is the task function's argument), passed as if we call4'd
        #endif // __XTENSA_CALL0_ABI__
    #अन्यथा
        frame->pc = (UBaseType_t) pxCode;                   // Task entry point is the provided task function
        #ifdef __XTENSA_CALL0_ABI__
            frame->a2 = (UBaseType_t) pvParameters;         // Task function's argument
        #अन्यथा // __XTENSA_CALL0_ABI__
            frame->a6 = (UBaseType_t) pvParameters;         // Task function's argument, passed as if we call4'd
        #endif // __XTENSA_CALL0_ABI__
    #endif

    /*
    Set initial PS to int level 0, EXCM disabled ('rfe' will enable), user mode.
    For windowed ABI also set WOE and CALLINC (pretend task was 'call4'd)
    */
    #ifdef __XTENSA_CALL0_ABI__
        frame->ps = PS_UM | PS_EXCM;
    #अन्यथा // __XTENSA_CALL0_ABI__
        frame->ps = PS_UM | PS_EXCM | PS_WOE | PS_CALLINC(1);
    #endif // __XTENSA_CALL0_ABI__

    #ifdef XT_USE_SWPRI
        // Set the initial virtual priority mask value to all 1's.
        frame->vpri = 0xFFFFFFFF;
    #endif

    // Initialize the threadptr register in the extra save area of the stack frame
    uint32_t *threadptr_reg = (uint32_t *)(ux_चळत_Pointer + XT_STK_EXTRA);
    *threadptr_reg = threadptr_reg_init;

    प्रत्यागम ux_चळत_Pointer;
}

#यदि ( port_HAS_चळत_OVERFLOW_CHECKING == 1 )
चळत_Type_t * px_Port_Initialise_चळत( चळत_Type_t * px_शीर्ष_Of_चळत,
                                     चळत_Type_t * px_End_Of_चळत,
                                     TaskFunction_t pxCode,
                                     शून्य * pvParameters )
#अन्यथा
चळत_Type_t * px_Port_Initialise_चळत( चळत_Type_t * px_शीर्ष_Of_चळत,
                                     TaskFunction_t pxCode,
                                     शून्य * pvParameters )
#endif
{
#ifdef __clang_analyzer__
    // Teach clang-tidy that pxTopOfStack cannot be a pointer to const
    चञ्चल चळत_Type_t * pxTemp = px_शीर्ष_Of_चळत;
    px_शीर्ष_Of_चळत = pxTemp;
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

    UBaseType_t ux_चळत_Pointer = (UBaseType_t)px_शीर्ष_Of_चळत;
    configASSERT((ux_चळत_Pointer & portBYTE_ALIGNMENT_MASK) == 0);

#यदि XCHAL_CP_अंक > 0
    // Initialize the coprocessor save area. THIS MUST BE THE FIRST AREA due to access from _frxt_task_coproc_state()
    ux_चळत_Pointer = ux_Initialise_चळत_CPSA(ux_चळत_Pointer);
    configASSERT((ux_चळत_Pointer & portBYTE_ALIGNMENT_MASK) == 0);
#endif /* XCHAL_CP_NUM > 0 */

    // Initialize the GCC TLS area
    uint32_t threadptr_reg_init;
    ux_चळत_Pointer = ux_Initialise_चळत_TLS(ux_चळत_Pointer, &threadptr_reg_init);
    configASSERT((ux_चळत_Pointer & portBYTE_ALIGNMENT_MASK) == 0);

    // Initialize the starting interrupt stack frame
    ux_चळत_Pointer = ux_Initialise_चळत_Frame(ux_चळत_Pointer, pxCode, pvParameters, threadptr_reg_init);
    configASSERT((ux_चळत_Pointer & portBYTE_ALIGNMENT_MASK) == 0);

    // Return the task's current stack pointer address which should point to the starting interrupt stack frame
    प्रत्यागम (चळत_Type_t *)ux_चळत_Pointer;
}

// ------------------- Hook Functions ----------------------

#अन्तर्भाव <stdlib.h>

#यदि ( config_CHECK_FOR_चळत_OVERFLOW > 0 )
शून्य  __attribute__((weak)) v_Application_चळत_Overflow_Hook( TaskHandle_t xTask, वर्ण *pcTaskName )
{
#define ERR_STR1 "***ERROR*** A stack overflow in task "
#define ERR_STR2 " has been detected."
    ध्रुव वर्ण *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    वर्ण buf[परिमाण(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + परिमाण(ERR_STR2) + 1 /* null char */] = { 0 };

    वर्ण *dest = buf;
    प्रति (size_t i = 0 ; i < परिमाण(str) / परिमाण(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
    esp_system_abort(buf);
}
#endif

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
#endif // CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK

/* ------------------------------------------------ Run Time Stats ------------------------------------------------- */

#यदि ( CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS )

configRUN_TIME_COUNTER_TYPE x_Port_Get_Run_Time_Counter_मूल्य( शून्य )
{
#ifdef CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
    प्रत्यागम (configRUN_TIME_COUNTER_TYPE) esp_timer_get_time();
#अन्यथा // Uses CCOUNT
    प्रत्यागम (configRUN_TIME_COUNTER_TYPE) xthal_get_ccount();
#endif // CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
}

#endif /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
