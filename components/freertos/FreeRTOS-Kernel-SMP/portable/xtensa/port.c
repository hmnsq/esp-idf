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
#إذا CONFIG_FREERTOS_پرلپسې_وخت_STATS_USING_ESP_TIMER
#ضمن "esp_timer.h"
#د_که_پای
#که_تعریف_شوی CONFIG_FREERTOS_SYSTICK_کاروي_SYSTIMER
#ضمن "soc/periph_defs.h"
#ضمن "soc/system_reg.h"
#ضمن "hal/systimer_hal.h"
#ضمن "hal/systimer_ll.h"
#د_که_پای // CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER

_Static_assert(پورټ_BYTE_ALIGNMENT == 16, "portBYTE_ALIGNMENT must be set to 16");

/* ---------------------------------------------------- Variables ------------------------------------------------------
 * - Various variables used to maintain the FreeRTOS port's state. Used from both port.c and various .S files
 * - Constant offsets are used by assembly to jump to particular TCB members or a stack area (such as the CPSA). We use
 *   C constants instead of preprocessor macros due to assembly lacking "offsetof()".
 * ------------------------------------------------------------------------------------------------------------------ */

#إذا XCHAL_CP_عدد > 0
/* Offsets used to navigate to a task's CPSA on the stack */
ثابت DRAM_ATTR uint32_t دننه_واټن_px_پای_ټکی_د_ډېری = offsetof(StaticTask_t, pxDummy8);
ثابت DRAM_ATTR uint32_t دننه_واټن_cpsa = XT_CP_اندازه;  /* Offset to start of the CPSA area on the stack. See uxInitialiseStackCPSA(). */
#إذا config_عدد_CORES > 1
/* Offset to TCB_t.uxCoreAffinityMask member. Used to pin unpinned tasks that use the FPU. */
ثابت DRAM_ATTR uint32_t دننه_واټن_ux_Core_Affinity_ماسک = offsetof(StaticTask_t, uxDummy26);
#إذا configUSE_CORE_AFFINITY != 1
#تېروتنه "configUSE_CORE_AFFINITY must be 1 on multicore targets with coprocessor support"
#د_که_پای
#د_که_پای /* configNUM_CORES > 1 */
#د_که_پای /* XCHAL_CP_NUM > 0 */

متطاير بینښه پورټ_x_Scheduler_Running[پورټ_عدد_PROCESSORS] = {0}; // Indicates whether scheduler is running on a per-core basis
بینښه صحيح پورټ_مداخله_Nesting[پورټ_عدد_PROCESSORS] = {0};  // Interrupt nesting level. Increased/decreased in portasm.c, _frxt_int_enter/_frxt_int_exit
متطاير بینښه پورټ_ux_Core_Startup_بشپړ[پورټ_عدد_PROCESSORS] = {0};  // Indicates whether the core has completed its startup sequence
#إذا ( config_شمېره_د_CORES > 1 )
//FreeRTOS SMP Locks
پورټ_MUX_TYPE پورټ_x_Task_قلف = پورټ_MUX_INITIALIZER_UNLOCKED;
پورټ_MUX_TYPE پورټ_x_ISRLock = پورټ_MUX_INITIALIZER_UNLOCKED;
#د_که_پای /* configNUMBER_OF_CORES > 1 */

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

بنسټ_Type_t x_پورټ_Interrupted_From_ISRContext(فراغ)
{
    أرجع (پورټ_مداخله_Nesting[x_پورټ_Get_Core_ID()] != 0);
}

// ------------------ Critical Sections --------------------

/*
Variables used by IDF critical sections only (SMP tracks critical nesting inside TCB now)
[refactor-todo] Figure out how IDF critical sections will be merged with SMP FreeRTOS critical sections
*/
بنسټ_Type_t پورټ_ux_Critical_Nesting_IDF[پورټ_عدد_PROCESSORS] = {0};
بنسټ_Type_t پورټ_ux_Critical_Old_مداخله_State_IDF[پورټ_عدد_PROCESSORS] = {0};
#إذا CONFIG_FREERTOS_پورټ_THREAD_خوندي_CLAIM
متطاير bool پورټ_x_Thread_خوندي_Claimed = false;
#د_که_پای

/*
*******************************************************************************
* Interrupt stack. The size of the interrupt stack is determined by the config
* parameter "configISR_STACK_SIZE" in FreeRTOSConfig.h
*******************************************************************************
*/
متطاير ډېری_Type_t DRAM_ATTR __attribute__((aligned(16))) پورټ_Int_ډېری[پورټ_عدد_PROCESSORS][config_ISR_ډېری_اندازه];
/* One flag for each individual CPU. */
متطاير uint32_t پورټ_switch_flag[پورټ_عدد_PROCESSORS];

#إذا CONFIG_FREERTOS_پورټ_THREAD_خوندي_CLAIM
فراغ x_پورټ_Thread_خوندي_Claim(فراغ)
{
    configASSERT(!x_پورټ_Can_Yield());
    configASSERT(!پورټ_x_Thread_خوندي_Claimed);
    پورټ_x_Thread_خوندي_Claimed = true;
}

فراغ x_پورټ_Thread_خوندي_Disclaim(فراغ)
{
    configASSERT(!x_پورټ_Can_Yield());
    configASSERT(پورټ_x_Thread_خوندي_Claimed);
    پورټ_x_Thread_خوندي_Claimed = false;
}
#د_که_پای /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

بنسټ_Type_t x_پورټ_Enter_Critical_Timeout(پورټ_MUX_TYPE *قلف, بنسټ_Type_t timeout)
{
#إذا CONFIG_FREERTOS_پورټ_THREAD_خوندي_CLAIM
    إذا (unlikely(پورټ_x_Thread_خوندي_Claimed)) {
        أرجع pdPASS;
    }
#د_که_پای
    /* Interrupts may already be disabled (if this function is called in nested
     * manner). However, there's no atomic operation that will allow us to check,
     * thus we have to disable interrupts again anyways.
     *
     * However, if this is call is NOT nested (i.e., the first call to enter a
     * critical section), we will save the previous interrupt level so that the
     * saved level can be restored on the last call to exit the critical.
     */
    بنسټ_Type_t x_Old_مداخله_Level = XTOS_سېټ_INTLEVEL(XCHAL_EXCM_LEVEL);
    /* Interrupts are masked (to XCHAL_EXCM_LEVEL, the same level the spinlock uses),
     * so the core id is stable and the spinlock does not need to mask again. Read the
     * core id register once and reuse it for the owner id and the nesting index. */
    uint32_t coreOwnerId = spinlock_owner_id();
    بنسټ_Type_t coreID = spinlock_core_id_from_owner_id(coreOwnerId);
    إذا (!spinlock_acquire_impl(قلف, timeout, coreOwnerId)) {
        //Timed out attempting to get spinlock. Restore previous interrupt level and return
        XTOS_RESTORE_JUST_INTLEVEL((صحيح) x_Old_مداخله_Level);
        أرجع pdFAIL;
    }
    //Spinlock acquired. Increment the IDF critical nesting count.
    بنسټ_Type_t newNesting = پورټ_ux_Critical_Nesting_IDF[coreID] + 1;
    پورټ_ux_Critical_Nesting_IDF[coreID] = newNesting;
    //If this is the first entry to a critical section. Save the old interrupt level.
    إذا ( newNesting == 1 ) {
        پورټ_ux_Critical_Old_مداخله_State_IDF[coreID] = x_Old_مداخله_Level;
    }
    أرجع pdPASS;

}

فراغ v_پورټ_Exit_Critical_IDF(پورټ_MUX_TYPE *قلف)
{
#إذا CONFIG_FREERTOS_پورټ_THREAD_خوندي_CLAIM
    إذا (unlikely(پورټ_x_Thread_خوندي_Claimed)) {
        أرجع;
    }
#د_که_پای
    /* This function may be called in a nested manner. Therefore, we only need
     * to re-enable interrupts if this is the last call to exit the critical. We
     * can use the nesting count to determine whether this is the last exit call.
     */
    /* Interrupts remain disabled for the whole critical section, so the core id is
     * stable and the spinlock does not need to mask again. Read the core id register
     * once and reuse it for the owner id and the nesting index. */
    uint32_t coreOwnerId = spinlock_owner_id();
    بنسټ_Type_t coreID = spinlock_core_id_from_owner_id(coreOwnerId);
    spinlock_release_impl(قلف, coreOwnerId);
    بنسټ_Type_t nesting = پورټ_ux_Critical_Nesting_IDF[coreID];

    /* Critical section nesting count must never be negative */
    configASSERT( nesting > 0 );

    إذا (nesting > 0) {
        nesting--;
        پورټ_ux_Critical_Nesting_IDF[coreID] = nesting;

        //This is the last exit call, restore the saved interrupt level
        إذا ( nesting == 0 ) {
            XTOS_RESTORE_JUST_INTLEVEL((صحيح) پورټ_ux_Critical_Old_مداخله_State_IDF[coreID]);
        }
    }
}

/*
In case any IDF libs called the port critical functions directly instead of through the macros.
Just inline call the IDF versions
*/
فراغ v_پورټ_Enter_Critical(پورټ_MUX_TYPE *قلف)
{
    v_پورټ_Enter_Critical_IDF(قلف);
}

فراغ v_پورټ_Exit_Critical(پورټ_MUX_TYPE *قلف)
{
    v_پورټ_Exit_Critical_IDF(قلف);
}

// ----------------------- System --------------------------

#تعریف_کړه ډېری_WATCH_ټکی_شمېره (SOC_CPU_WATCHPOINTS_عدد - 1)

فراغ v_پورټ_سېټ_ډېری_Watchpoint( فراغ *px_ډېری_پیل )
{
    //Set watchpoint 1 to watch the last 32 bytes of the stack.
    //Unfortunately, the Xtensa watchpoints can't set a watchpoint on a random [base - base+n] region because
    //the size works by masking off the lowest address bits. For that reason, we futz a bit and watch the lowest 32
    //bytes of the stack we can actually watch. In general, this can cause the watchpoint to be triggered at most
    //28 bytes early. The value 32 is chosen because it's larger than the stack canary, which in FreeRTOS is 20 bytes.
    //This way, we make sure we trigger before/when the stack canary is corrupted, not after.
    صحيح addr = (صحيح)px_ډېری_پیل;
    addr = (addr + 31) & (~31);
    esp_cpu_سېټ_watchpoint(ډېری_WATCH_ټکی_شمېره, (حرف *)addr, 32, ESP_CPU_WATCHPOINT_STORE);
}



/* ---------------------------------------------- Port Implementations -------------------------------------------------
 * Implementations of Porting Interface functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

بنسټ_Type_t x_پورټ_Check_If_دننه_ISR(فراغ)
{
    //Disable interrupts so that reading port_interruptNesting is atomic
    بنسټ_Type_t ret;
    بینښه صحيح prev_int_level = پورټ_DISABLE_INTERRUPTS();
    ret = (پورټ_مداخله_Nesting[x_پورټ_Get_Core_ID()] != 0) ? pdTRUE : pdFALSE;
    پورټ_RESTORE_INTERRUPTS(prev_int_level);
    أرجع ret;
}

فراغ v_پورټ_Assert_If_دننه_ISR(فراغ)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(x_پورټ_Check_If_دننه_ISR() == 0);
}

// ------------------ Critical Sections --------------------

#إذا ( config_شمېره_د_CORES > 1 )
فراغ v_پورټ_Take_قلف( پورټ_MUX_TYPE *قلف )
{
    spinlock_acquire( قلف, پورټ_MUX_NO_TIMEOUT);
}

فراغ v_پورټ_Release_قلف( پورټ_MUX_TYPE *قلف )
{
    spinlock_release( قلف );
}
#د_که_پای /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

// ----------------------- System --------------------------

// ------------------- Run Time Stats ----------------------

// --------------------- TCB Cleanup -----------------------

#إذا ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
ساكن فراغ v_پورټ_TLSPointers_Del_Cb( فراغ *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pv_Thread_Local_Storage_اشارې_Del_Callback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_عدد_THREAD_LOCAL_STORAGE_اشارې / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    لكل ( صحيح x = 0; x < ( config_عدد_THREAD_LOCAL_STORAGE_اشارې / 2 ); x++ ) {
        إذا ( pv_Thread_Local_Storage_اشارې_Del_Callback[ x ] != عدم ) {  //If del cb is set
            /* In case the TLSP deletion callback has been overwritten by a TLS pointer, gracefully abort. */
            إذا ( !esp_ptr_executable( pv_Thread_Local_Storage_اشارې_Del_Callback[ x ] ) ) {
                // We call EARLY log here as currently portCLEAN_UP_TCB() is called in a critical section
                ESP_EARLY_LOGE("FreeRTOS", "Fatal error: TLSP deletion callback at index %d overwritten with non-excutable pointer %p", x, pv_Thread_Local_Storage_اشارې_Del_Callback[ x ]);
                abort();
            }

            pv_Thread_Local_Storage_اشارې_Del_Callback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#د_که_پای /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

#إذا ( XCHAL_CP_عدد > 0 && configUSE_CORE_AFFINITY == 1 && config_عدد_CORES > 1 )
ساكن فراغ v_پورټ_Clean_پورته_Coproc_Area( فراغ *pxTCB )
{
    UBaseType_t uxCoprocArea;
    بنسټ_Type_t x_موخه_Core_ID;

    /* Get pointer to the task's coprocessor save area from TCB->pxEndOfStack. See uxInitialiseStackCPSA() */
    uxCoprocArea = ( UBaseType_t ) ( ( ( StaticTask_t * ) pxTCB )->pxDummy8 );  /* Get TCB_t.pxEndOfStack */
    uxCoprocArea = ESP_ALIGN_ښکته(uxCoprocArea - XT_CP_اندازه, 16);

    /* Extract core ID from the affinity mask */
    x_موخه_Core_ID = ( ( StaticTask_t * ) pxTCB )->uxDummy26;
    x_موخه_Core_ID = ( بنسټ_Type_t ) __builtin_ffs( ( صحيح ) x_موخه_Core_ID );
    أكد( x_موخه_Core_ID >= 1 ); // __builtin_ffs always returns first set index + 1
    x_موخه_Core_ID -= 1;

    /* If task has live floating point registers somewhere, release them */
    فراغ _xt_coproc_release(متطاير فراغ *coproc_sa_بنسټ, بنسټ_Type_t x_موخه_Core_ID);
    _xt_coproc_release( (فراغ *)uxCoprocArea, x_موخه_Core_ID );
}
#د_که_پای /* ( XCHAL_CP_NUM > 0 && configUSE_CORE_AFFINITY == 1 && configNUM_CORES > 1 ) */

فراغ v_پورټ_TCBPre_Delete_Hook( فراغ *pxTCB )
{
    #إذا ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        خارجي فراغ vTaskPreDeletionHook( فراغ * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #د_که_پای /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

    #إذا ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
        /* Call TLS pointers deletion callbacks */
        v_پورټ_TLSPointers_Del_Cb( pxTCB );
    #د_که_پای /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

    #إذا ( XCHAL_CP_عدد > 0 && configUSE_CORE_AFFINITY == 1 && config_عدد_CORES > 1 )
        /* Cleanup coproc save area */
        v_پورټ_Clean_پورته_Coproc_Area( pxTCB );
    #د_که_پای /* ( XCHAL_CP_NUM > 0 && configUSE_CORE_AFFINITY == 1 && configNUM_CORES > 1 ) */
}

/* ------------------------------------------------ FreeRTOS Portable --------------------------------------------------
 * - Provides implementation for functions required by FreeRTOS
 * - Declared in portable.h
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------- Scheduler Start/End -------------------

خارجي فراغ _xt_coproc_init(فراغ);

بنسټ_Type_t x_پورټ_پیل_Scheduler( فراغ )
{
    پورټ_DISABLE_INTERRUPTS();
    // Interrupts are disabled at this point and stack contains PS with enabled interrupts when task context is restored

#إذا XCHAL_CP_عدد > 0
    /* Initialize co-processor management for tasks. Leave CPENABLE alone. */
    _xt_coproc_init();
#د_که_پای

    /* Setup the hardware to generate the tick. */
    v_پورټ_تیاری_Timer();

        /* Initialize all kernel state tracking variables */
    بنسټ_Type_t coreID = x_پورټ_Get_Core_ID();
    پورټ_x_Scheduler_Running[coreID] = 1;
    پورټ_ux_Core_Startup_بشپړ[coreID] = 0;
#إذا CONFIG_FREERTOS_پورټ_THREAD_خوندي_CLAIM
    پورټ_x_Thread_خوندي_Claimed = false;
#د_که_پای

#إذا config_عدد_CORES > 1
    // Workaround for non-thread safe multi-core OS startup (see IDF-4524)
    إذا (x_پورټ_Get_Core_ID() != 0) {
        prv_پیل_Scheduler_Other_Cores();
    }
#د_که_پای // configNUM_CORES > 1

    // Windows contain references to the startup stack which will be reclaimed by the main task
    // Spill the windows to create a clean environment to ensure we do not carry over any such references
    // to invalid SPs which will cause problems if main_task does a windowoverflow to them
    xthal_کړکۍ_spill();

    // Cannot be directly called from C; never returns
    __asm__ متطاير ("call0    _frxt_dispatch\n");

    /* Should not get here. */
    أرجع pdTRUE;
}

فراغ v_پورټ_پای_ټکی_Scheduler( فراغ )
{
    ;
}

// ------------------------ Stack --------------------------

// User exception dispatcher when exiting
فراغ _xt_user_exit(فراغ);

#إذا CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
// Wrapper to allow task functions to return (increases stack overhead by 16 bytes)
ساكن فراغ v_پورټ_Task_Wrapper(TaskFunction_t px_کوډ, فراغ *pvParameters)
{
    px_کوډ(pvParameters);
    //FreeRTOS tasks should not return. Log the task name and abort.
    حرف *pcTaskName = pcTaskGetName(عدم);
    ESP_LOGE("FreeRTOS", "FreeRTOS Task \"%s\" should not return, Aborting now!", pcTaskName);
    abort();
}
#د_که_پای

#إذا XCHAL_CP_عدد > 0
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ډېری_CPSA(UBaseType_t ux_ډېری_اشاره_ګر)
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
    ux_ډېری_اشاره_ګر = ESP_ALIGN_ښکته(ux_ډېری_اشاره_ګر - XT_CP_اندازه, 16);
    // Initialize the coprocessor context switching flags.
    uint32_t *p = (uint32_t *)ux_ډېری_اشاره_ګر;
    p[0] = 0;   // Clear XT_CPENABLE and XT_CPSTORED
    p[1] = 0;   // Clear XT_CP_CS_ST
    // XT_CP_ASA points to the aligned start of the individual CP save areas (i.e., start of CP0 SA)
    p[2] = (uint32_t)ESP_ALIGN_پورته((uint32_t)ux_ډېری_اشاره_ګر + 12, XCHAL_ټول_SA_ALIGN);
    أرجع ux_ډېری_اشاره_ګر;
}
#د_که_پای /* XCHAL_CP_NUM > 0 */

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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ډېری_TLS(UBaseType_t ux_ډېری_اشاره_ګر, uint32_t *ret_threadptr_reg_init)
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
    خارجي حرف thread_local_معلومات_پیل, thread_local_معلومات_پای_ټکی;
    خارجي حرف thread_local_bss_پیل, thread_local_bss_پای_ټکی;
    ثابت uint32_t tls_معلومات_اندازه = (uint32_t)&thread_local_معلومات_پای_ټکی - (uint32_t)&thread_local_معلومات_پیل;
    ثابت uint32_t tls_bss_اندازه = (uint32_t)&thread_local_bss_پای_ټکی - (uint32_t)&thread_local_bss_پیل;
    ثابت uint32_t tls_area_اندازه = ESP_ALIGN_پورته(tls_معلومات_اندازه + tls_bss_اندازه, 16);
    // TODO: check that TLS area fits the stack

    // Allocate space for the TLS area on the stack. The area must be allocated at a 16-byte aligned address
    ux_ډېری_اشاره_ګر = ESP_ALIGN_ښکته(ux_ډېری_اشاره_ګر - (UBaseType_t)tls_area_اندازه, 16);
    // Initialize the TLS data with the initialization values of each TLS variable
    حافظه_نقل((فراغ *)ux_ډېری_اشاره_ګر, &thread_local_معلومات_پیل, tls_معلومات_اندازه);
    // Initialize the TLS bss with zeroes
    memset((فراغ *)(ux_ډېری_اشاره_ګر + tls_معلومات_اندازه), 0, tls_bss_اندازه);

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
    #تعریف_کړه TCB_اندازه 8
    ثابت uint32_t بنسټ = ESP_ALIGN_پورته(TCB_اندازه, tls_section_align);
    *ret_threadptr_reg_init = (uint32_t)ux_ډېری_اشاره_ګر - بنسټ;

    أرجع ux_ډېری_اشاره_ګر;
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ډېری_چوکاټ(UBaseType_t ux_ډېری_اشاره_ګر, TaskFunction_t px_کوډ, فراغ *pvParameters, uint32_t threadptr_reg_init)
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
    UBaseType_t ux_ډېری_اشاره_ګر_پخوانی = ux_ډېری_اشاره_ګر;
    ux_ډېری_اشاره_ګر = ESP_ALIGN_ښکته(ux_ډېری_اشاره_ګر - XT_STK_FRMSZ, 16);

    // Clear the entire interrupt stack frame
    memset((فراغ *)ux_ډېری_اشاره_ګر, 0, (size_t)(ux_ډېری_اشاره_ګر_پخوانی - ux_ډېری_اشاره_ګر));

    Xt_Exc_چوکاټ *چوکاټ = (Xt_Exc_چوکاټ *)ux_ډېری_اشاره_ګر;

    /*
    Initialize common registers
    */
    چوکاټ->a0 = 0;                                          // Set the return address to 0 terminate GDB backtrace
    چوکاټ->a1 = ux_ډېری_اشاره_ګر + XT_STK_FRMSZ;              // Saved stack pointer should point to physical top of stack frame
    چوکاټ->وتل = (UBaseType_t) _xt_user_exit;              // User exception exit dispatcher

    /*
    Initialize the task's entry point. This will differ depending on
    - Whether the task's entry point is the wrapper function or pxCode
    - Whether Windowed ABI is used (for windowed, we mimic the task entry point being call4'd )
    */
    #إذا CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
        چوکاټ->pc = (UBaseType_t) v_پورټ_Task_Wrapper;         // Task entry point is the wrapper function
        #که_تعریف_شوی __XTENSA_CALL0_ABI__
            چوکاټ->a2 = (UBaseType_t) px_کوډ;               // Wrapper function's argument 0 (which is the task function)
            چوکاټ->a3 = (UBaseType_t) pvParameters;         // Wrapper function's argument 1 (which is the task function's argument)
        #وإلا // __XTENSA_CALL0_ABI__
            چوکاټ->a6 = (UBaseType_t) px_کوډ;               // Wrapper function's argument 0 (which is the task function), passed as if we call4'd
            چوکاټ->a7 = (UBaseType_t) pvParameters;         // Wrapper function's argument 1 (which is the task function's argument), passed as if we call4'd
        #د_که_پای // __XTENSA_CALL0_ABI__
    #وإلا
        چوکاټ->pc = (UBaseType_t) px_کوډ;                   // Task entry point is the provided task function
        #که_تعریف_شوی __XTENSA_CALL0_ABI__
            چوکاټ->a2 = (UBaseType_t) pvParameters;         // Task function's argument
        #وإلا // __XTENSA_CALL0_ABI__
            چوکاټ->a6 = (UBaseType_t) pvParameters;         // Task function's argument, passed as if we call4'd
        #د_که_پای // __XTENSA_CALL0_ABI__
    #د_که_پای

    /*
    Set initial PS to int level 0, EXCM disabled ('rfe' will enable), user mode.
    For windowed ABI also set WOE and CALLINC (pretend task was 'call4'd)
    */
    #که_تعریف_شوی __XTENSA_CALL0_ABI__
        چوکاټ->ps = PS_UM | PS_EXCM;
    #وإلا // __XTENSA_CALL0_ABI__
        چوکاټ->ps = PS_UM | PS_EXCM | PS_WOE | PS_CALLINC(1);
    #د_که_پای // __XTENSA_CALL0_ABI__

    #که_تعریف_شوی XT_USE_SWPRI
        // Set the initial virtual priority mask value to all 1's.
        چوکاټ->vpri = 0xFFFFFFFF;
    #د_که_پای

    // Initialize the threadptr register in the extra save area of the stack frame
    uint32_t *threadptr_reg = (uint32_t *)(ux_ډېری_اشاره_ګر + XT_STK_EXTRA);
    *threadptr_reg = threadptr_reg_init;

    أرجع ux_ډېری_اشاره_ګر;
}

#إذا ( پورټ_HAS_ډېری_ډکېدنه_CHECKING == 1 )
ډېری_Type_t * px_پورټ_Initialise_ډېری( ډېری_Type_t * px_سر_د_ډېری,
                                     ډېری_Type_t * px_پای_ټکی_د_ډېری,
                                     TaskFunction_t px_کوډ,
                                     فراغ * pvParameters )
#وإلا
ډېری_Type_t * px_پورټ_Initialise_ډېری( ډېری_Type_t * px_سر_د_ډېری,
                                     TaskFunction_t px_کوډ,
                                     فراغ * pvParameters )
#د_که_پای
{
#که_تعریف_شوی __clang_analyzer__
    // Teach clang-tidy that pxTopOfStack cannot be a pointer to const
    متطاير ډېری_Type_t * px_لنډمهاله = px_سر_د_ډېری;
    px_سر_د_ډېری = px_لنډمهاله;
#د_که_پای /*__clang_analyzer__ */
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

    UBaseType_t ux_ډېری_اشاره_ګر = (UBaseType_t)px_سر_د_ډېری;
    configASSERT((ux_ډېری_اشاره_ګر & پورټ_BYTE_ALIGNMENT_ماسک) == 0);

#إذا XCHAL_CP_عدد > 0
    // Initialize the coprocessor save area. THIS MUST BE THE FIRST AREA due to access from _frxt_task_coproc_state()
    ux_ډېری_اشاره_ګر = ux_Initialise_ډېری_CPSA(ux_ډېری_اشاره_ګر);
    configASSERT((ux_ډېری_اشاره_ګر & پورټ_BYTE_ALIGNMENT_ماسک) == 0);
#د_که_پای /* XCHAL_CP_NUM > 0 */

    // Initialize the GCC TLS area
    uint32_t threadptr_reg_init;
    ux_ډېری_اشاره_ګر = ux_Initialise_ډېری_TLS(ux_ډېری_اشاره_ګر, &threadptr_reg_init);
    configASSERT((ux_ډېری_اشاره_ګر & پورټ_BYTE_ALIGNMENT_ماسک) == 0);

    // Initialize the starting interrupt stack frame
    ux_ډېری_اشاره_ګر = ux_Initialise_ډېری_چوکاټ(ux_ډېری_اشاره_ګر, px_کوډ, pvParameters, threadptr_reg_init);
    configASSERT((ux_ډېری_اشاره_ګر & پورټ_BYTE_ALIGNMENT_ماسک) == 0);

    // Return the task's current stack pointer address which should point to the starting interrupt stack frame
    أرجع (ډېری_Type_t *)ux_ډېری_اشاره_ګر;
}

// ------------------- Hook Functions ----------------------

#ضمن <stdlib.h>

#إذا ( config_CHECK_FOR_ډېری_ډکېدنه > 0 )
فراغ  __attribute__((weak)) v_Application_ډېری_ډکېدنه_Hook( TaskHandle_t xTask, حرف *pcTaskName )
{
#تعریف_کړه ERR_STR1 "***ERROR*** A stack overflow in task "
#تعریف_کړه ERR_STR2 " has been detected."
    ثابت حرف *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    حرف buf[حجم(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + حجم(ERR_STR2) + 1 /* null char */] = { 0 };

    حرف *dest = buf;
    لكل (size_t i = 0 ; i < حجم(str) / حجم(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
    esp_system_abort(buf);
}
#د_که_پای

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
#د_که_پای // CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK

/* ------------------------------------------------ Run Time Stats ------------------------------------------------- */

#إذا ( CONFIG_FREERTOS_GENERATE_پرلپسې_وخت_STATS )

config_پرلپسې_وخت_COUNTER_TYPE x_پورټ_Get_پرلپسې_وخت_Counter_ارزښت( فراغ )
{
#که_تعریف_شوی CONFIG_FREERTOS_پرلپسې_وخت_STATS_USING_ESP_TIMER
    أرجع (config_پرلپسې_وخت_COUNTER_TYPE) esp_timer_get_وخت();
#وإلا // Uses CCOUNT
    أرجع (config_پرلپسې_وخت_COUNTER_TYPE) xthal_get_ccount();
#د_که_پای // CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
}

#د_که_پای /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
