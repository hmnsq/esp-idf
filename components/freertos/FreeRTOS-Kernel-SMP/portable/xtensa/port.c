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
#إذا CONFIG_FREERTOS_لگاتار_ویلا_STATS_USING_ESP_TIMER
#ضمن "esp_timer.h"
#جے_انت
#جے_تعریف_شدہ CONFIG_FREERTOS_SYSTICK_ورتدا_SYSTIMER
#ضمن "soc/periph_defs.h"
#ضمن "soc/system_reg.h"
#ضمن "hal/systimer_hal.h"
#ضمن "hal/systimer_ll.h"
#جے_انت // CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER

_Static_assert(پورٹ_BYTE_ALIGNMENT == 16, "portBYTE_ALIGNMENT must be set to 16");

/* ---------------------------------------------------- Variables ------------------------------------------------------
 * - Various variables used to maintain the FreeRTOS port's state. Used from both port.c and various .S files
 * - Constant offsets are used by assembly to jump to particular TCB members or a stack area (such as the CPSA). We use
 *   C constants instead of preprocessor macros due to assembly lacking "offsetof()".
 * ------------------------------------------------------------------------------------------------------------------ */

#إذا XCHAL_CP_عدد > 0
/* Offsets used to navigate to a task's CPSA on the stack */
ثابت DRAM_ATTR uint32_t وتھ_px_سرا_دا_ڈھیر = offsetof(StaticTask_t, pxDummy8);
ثابت DRAM_ATTR uint32_t وتھ_cpsa = XT_CP_ناپ;  /* Offset to start of the CPSA area on the stack. See uxInitialiseStackCPSA(). */
#إذا config_عدد_CORES > 1
/* Offset to TCB_t.uxCoreAffinityMask member. Used to pin unpinned tasks that use the FPU. */
ثابت DRAM_ATTR uint32_t وتھ_ux_Core_Affinity_نقاب = offsetof(StaticTask_t, uxDummy26);
#إذا configUSE_CORE_AFFINITY != 1
#غلطی "configUSE_CORE_AFFINITY must be 1 on multicore targets with coprocessor support"
#جے_انت
#جے_انت /* configNUM_CORES > 1 */
#جے_انت /* XCHAL_CP_NUM > 0 */

متطاير بنانشانی پورٹ_x_Scheduler_Running[پورٹ_عدد_PROCESSORS] = {0}; // Indicates whether scheduler is running on a per-core basis
بنانشانی صحيح پورٹ_رکاوٹ_Nesting[پورٹ_عدد_PROCESSORS] = {0};  // Interrupt nesting level. Increased/decreased in portasm.c, _frxt_int_enter/_frxt_int_exit
متطاير بنانشانی پورٹ_ux_Core_Startup_پورا[پورٹ_عدد_PROCESSORS] = {0};  // Indicates whether the core has completed its startup sequence
#إذا ( config_نمبر_دا_CORES > 1 )
//FreeRTOS SMP Locks
پورٹ_MUX_TYPE پورٹ_x_Task_جندرا = پورٹ_MUX_INITIALIZER_UNLOCKED;
پورٹ_MUX_TYPE پورٹ_x_ISRLock = پورٹ_MUX_INITIALIZER_UNLOCKED;
#جے_انت /* configNUMBER_OF_CORES > 1 */

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

ادھار_Type_t x_پورٹ_Interrupted_From_ISRContext(فراغ)
{
    أرجع (پورٹ_رکاوٹ_Nesting[x_پورٹ_Get_Core_ID()] != 0);
}

// ------------------ Critical Sections --------------------

/*
Variables used by IDF critical sections only (SMP tracks critical nesting inside TCB now)
[refactor-todo] Figure out how IDF critical sections will be merged with SMP FreeRTOS critical sections
*/
ادھار_Type_t پورٹ_ux_Critical_Nesting_IDF[پورٹ_عدد_PROCESSORS] = {0};
ادھار_Type_t پورٹ_ux_Critical_Old_رکاوٹ_State_IDF[پورٹ_عدد_PROCESSORS] = {0};
#إذا CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
متطاير bool پورٹ_x_Thread_محفوظ_Claimed = false;
#جے_انت

/*
*******************************************************************************
* Interrupt stack. The size of the interrupt stack is determined by the config
* parameter "configISR_STACK_SIZE" in FreeRTOSConfig.h
*******************************************************************************
*/
متطاير ڈھیر_Type_t DRAM_ATTR __attribute__((aligned(16))) پورٹ_Int_ڈھیر[پورٹ_عدد_PROCESSORS][config_ISR_ڈھیر_ناپ];
/* One flag for each individual CPU. */
متطاير uint32_t پورٹ_switch_flag[پورٹ_عدد_PROCESSORS];

#إذا CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
فراغ x_پورٹ_Thread_محفوظ_Claim(فراغ)
{
    configASSERT(!x_پورٹ_Can_Yield());
    configASSERT(!پورٹ_x_Thread_محفوظ_Claimed);
    پورٹ_x_Thread_محفوظ_Claimed = true;
}

فراغ x_پورٹ_Thread_محفوظ_Disclaim(فراغ)
{
    configASSERT(!x_پورٹ_Can_Yield());
    configASSERT(پورٹ_x_Thread_محفوظ_Claimed);
    پورٹ_x_Thread_محفوظ_Claimed = false;
}
#جے_انت /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

ادھار_Type_t x_پورٹ_Enter_Critical_Timeout(پورٹ_MUX_TYPE *جندرا, ادھار_Type_t timeout)
{
#إذا CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
    إذا (unlikely(پورٹ_x_Thread_محفوظ_Claimed)) {
        أرجع pdPASS;
    }
#جے_انت
    /* Interrupts may already be disabled (if this function is called in nested
     * manner). However, there's no atomic operation that will allow us to check,
     * thus we have to disable interrupts again anyways.
     *
     * However, if this is call is NOT nested (i.e., the first call to enter a
     * critical section), we will save the previous interrupt level so that the
     * saved level can be restored on the last call to exit the critical.
     */
    ادھار_Type_t x_Old_رکاوٹ_Level = XTOS_سیٹ_INTLEVEL(XCHAL_EXCM_LEVEL);
    /* Interrupts are masked (to XCHAL_EXCM_LEVEL, the same level the spinlock uses),
     * so the core id is stable and the spinlock does not need to mask again. Read the
     * core id register once and reuse it for the owner id and the nesting index. */
    uint32_t coreOwnerId = spinlock_owner_id();
    ادھار_Type_t coreID = spinlock_core_id_from_owner_id(coreOwnerId);
    إذا (!spinlock_acquire_impl(جندرا, timeout, coreOwnerId)) {
        //Timed out attempting to get spinlock. Restore previous interrupt level and return
        XTOS_RESTORE_JUST_INTLEVEL((صحيح) x_Old_رکاوٹ_Level);
        أرجع pdFAIL;
    }
    //Spinlock acquired. Increment the IDF critical nesting count.
    ادھار_Type_t newNesting = پورٹ_ux_Critical_Nesting_IDF[coreID] + 1;
    پورٹ_ux_Critical_Nesting_IDF[coreID] = newNesting;
    //If this is the first entry to a critical section. Save the old interrupt level.
    إذا ( newNesting == 1 ) {
        پورٹ_ux_Critical_Old_رکاوٹ_State_IDF[coreID] = x_Old_رکاوٹ_Level;
    }
    أرجع pdPASS;

}

فراغ v_پورٹ_Exit_Critical_IDF(پورٹ_MUX_TYPE *جندرا)
{
#إذا CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
    إذا (unlikely(پورٹ_x_Thread_محفوظ_Claimed)) {
        أرجع;
    }
#جے_انت
    /* This function may be called in a nested manner. Therefore, we only need
     * to re-enable interrupts if this is the last call to exit the critical. We
     * can use the nesting count to determine whether this is the last exit call.
     */
    /* Interrupts remain disabled for the whole critical section, so the core id is
     * stable and the spinlock does not need to mask again. Read the core id register
     * once and reuse it for the owner id and the nesting index. */
    uint32_t coreOwnerId = spinlock_owner_id();
    ادھار_Type_t coreID = spinlock_core_id_from_owner_id(coreOwnerId);
    spinlock_release_impl(جندرا, coreOwnerId);
    ادھار_Type_t nesting = پورٹ_ux_Critical_Nesting_IDF[coreID];

    /* Critical section nesting count must never be negative */
    configASSERT( nesting > 0 );

    إذا (nesting > 0) {
        nesting--;
        پورٹ_ux_Critical_Nesting_IDF[coreID] = nesting;

        //This is the last exit call, restore the saved interrupt level
        إذا ( nesting == 0 ) {
            XTOS_RESTORE_JUST_INTLEVEL((صحيح) پورٹ_ux_Critical_Old_رکاوٹ_State_IDF[coreID]);
        }
    }
}

/*
In case any IDF libs called the port critical functions directly instead of through the macros.
Just inline call the IDF versions
*/
فراغ v_پورٹ_Enter_Critical(پورٹ_MUX_TYPE *جندرا)
{
    v_پورٹ_Enter_Critical_IDF(جندرا);
}

فراغ v_پورٹ_Exit_Critical(پورٹ_MUX_TYPE *جندرا)
{
    v_پورٹ_Exit_Critical_IDF(جندرا);
}

// ----------------------- System --------------------------

#تعریف_کرو ڈھیر_WATCH_نقطہ_نمبر (SOC_CPU_WATCHPOINTS_عدد - 1)

فراغ v_پورٹ_سیٹ_ڈھیر_Watchpoint( فراغ *px_ڈھیر_شروع )
{
    //Set watchpoint 1 to watch the last 32 bytes of the stack.
    //Unfortunately, the Xtensa watchpoints can't set a watchpoint on a random [base - base+n] region because
    //the size works by masking off the lowest address bits. For that reason, we futz a bit and watch the lowest 32
    //bytes of the stack we can actually watch. In general, this can cause the watchpoint to be triggered at most
    //28 bytes early. The value 32 is chosen because it's larger than the stack canary, which in FreeRTOS is 20 bytes.
    //This way, we make sure we trigger before/when the stack canary is corrupted, not after.
    صحيح addr = (صحيح)px_ڈھیر_شروع;
    addr = (addr + 31) & (~31);
    esp_cpu_سیٹ_watchpoint(ڈھیر_WATCH_نقطہ_نمبر, (حرف *)addr, 32, ESP_CPU_WATCHPOINT_STORE);
}



/* ---------------------------------------------- Port Implementations -------------------------------------------------
 * Implementations of Porting Interface functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

ادھار_Type_t x_پورٹ_Check_If_وچ_ISR(فراغ)
{
    //Disable interrupts so that reading port_interruptNesting is atomic
    ادھار_Type_t ret;
    بنانشانی صحيح prev_int_level = پورٹ_DISABLE_INTERRUPTS();
    ret = (پورٹ_رکاوٹ_Nesting[x_پورٹ_Get_Core_ID()] != 0) ? pdTRUE : pdFALSE;
    پورٹ_RESTORE_INTERRUPTS(prev_int_level);
    أرجع ret;
}

فراغ v_پورٹ_Assert_If_وچ_ISR(فراغ)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(x_پورٹ_Check_If_وچ_ISR() == 0);
}

// ------------------ Critical Sections --------------------

#إذا ( config_نمبر_دا_CORES > 1 )
فراغ v_پورٹ_Take_جندرا( پورٹ_MUX_TYPE *جندرا )
{
    spinlock_acquire( جندرا, پورٹ_MUX_NO_TIMEOUT);
}

فراغ v_پورٹ_Release_جندرا( پورٹ_MUX_TYPE *جندرا )
{
    spinlock_release( جندرا );
}
#جے_انت /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

// ----------------------- System --------------------------

// ------------------- Run Time Stats ----------------------

// --------------------- TCB Cleanup -----------------------

#إذا ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
ساكن فراغ v_پورٹ_TLSPointers_Del_Cb( فراغ *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pv_Thread_Local_Storage_سوچکاں_Del_Callback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_عدد_THREAD_LOCAL_STORAGE_سوچکاں / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    لكل ( صحيح x = 0; x < ( config_عدد_THREAD_LOCAL_STORAGE_سوچکاں / 2 ); x++ ) {
        إذا ( pv_Thread_Local_Storage_سوچکاں_Del_Callback[ x ] != عدم ) {  //If del cb is set
            /* In case the TLSP deletion callback has been overwritten by a TLS pointer, gracefully abort. */
            إذا ( !esp_ptr_executable( pv_Thread_Local_Storage_سوچکاں_Del_Callback[ x ] ) ) {
                // We call EARLY log here as currently portCLEAN_UP_TCB() is called in a critical section
                ESP_EARLY_LOGE("FreeRTOS", "Fatal error: TLSP deletion callback at index %d overwritten with non-excutable pointer %p", x, pv_Thread_Local_Storage_سوچکاں_Del_Callback[ x ]);
                abort();
            }

            pv_Thread_Local_Storage_سوچکاں_Del_Callback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#جے_انت /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

#إذا ( XCHAL_CP_عدد > 0 && configUSE_CORE_AFFINITY == 1 && config_عدد_CORES > 1 )
ساكن فراغ v_پورٹ_Clean_اتے_Coproc_Area( فراغ *pxTCB )
{
    UBaseType_t uxCoprocArea;
    ادھار_Type_t x_نشانہ_Core_ID;

    /* Get pointer to the task's coprocessor save area from TCB->pxEndOfStack. See uxInitialiseStackCPSA() */
    uxCoprocArea = ( UBaseType_t ) ( ( ( StaticTask_t * ) pxTCB )->pxDummy8 );  /* Get TCB_t.pxEndOfStack */
    uxCoprocArea = ESP_ALIGN_تھلے(uxCoprocArea - XT_CP_ناپ, 16);

    /* Extract core ID from the affinity mask */
    x_نشانہ_Core_ID = ( ( StaticTask_t * ) pxTCB )->uxDummy26;
    x_نشانہ_Core_ID = ( ادھار_Type_t ) __builtin_ffs( ( صحيح ) x_نشانہ_Core_ID );
    أكد( x_نشانہ_Core_ID >= 1 ); // __builtin_ffs always returns first set index + 1
    x_نشانہ_Core_ID -= 1;

    /* If task has live floating point registers somewhere, release them */
    فراغ _xt_coproc_release(متطاير فراغ *coproc_sa_ادھار, ادھار_Type_t x_نشانہ_Core_ID);
    _xt_coproc_release( (فراغ *)uxCoprocArea, x_نشانہ_Core_ID );
}
#جے_انت /* ( XCHAL_CP_NUM > 0 && configUSE_CORE_AFFINITY == 1 && configNUM_CORES > 1 ) */

فراغ v_پورٹ_TCBPre_Delete_Hook( فراغ *pxTCB )
{
    #إذا ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        خارجي فراغ vTaskPreDeletionHook( فراغ * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #جے_انت /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

    #إذا ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
        /* Call TLS pointers deletion callbacks */
        v_پورٹ_TLSPointers_Del_Cb( pxTCB );
    #جے_انت /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

    #إذا ( XCHAL_CP_عدد > 0 && configUSE_CORE_AFFINITY == 1 && config_عدد_CORES > 1 )
        /* Cleanup coproc save area */
        v_پورٹ_Clean_اتے_Coproc_Area( pxTCB );
    #جے_انت /* ( XCHAL_CP_NUM > 0 && configUSE_CORE_AFFINITY == 1 && configNUM_CORES > 1 ) */
}

/* ------------------------------------------------ FreeRTOS Portable --------------------------------------------------
 * - Provides implementation for functions required by FreeRTOS
 * - Declared in portable.h
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------- Scheduler Start/End -------------------

خارجي فراغ _xt_coproc_init(فراغ);

ادھار_Type_t x_پورٹ_شروع_Scheduler( فراغ )
{
    پورٹ_DISABLE_INTERRUPTS();
    // Interrupts are disabled at this point and stack contains PS with enabled interrupts when task context is restored

#إذا XCHAL_CP_عدد > 0
    /* Initialize co-processor management for tasks. Leave CPENABLE alone. */
    _xt_coproc_init();
#جے_انت

    /* Setup the hardware to generate the tick. */
    v_پورٹ_تیاری_Timer();

        /* Initialize all kernel state tracking variables */
    ادھار_Type_t coreID = x_پورٹ_Get_Core_ID();
    پورٹ_x_Scheduler_Running[coreID] = 1;
    پورٹ_ux_Core_Startup_پورا[coreID] = 0;
#إذا CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
    پورٹ_x_Thread_محفوظ_Claimed = false;
#جے_انت

#إذا config_عدد_CORES > 1
    // Workaround for non-thread safe multi-core OS startup (see IDF-4524)
    إذا (x_پورٹ_Get_Core_ID() != 0) {
        prv_شروع_Scheduler_Other_Cores();
    }
#جے_انت // configNUM_CORES > 1

    // Windows contain references to the startup stack which will be reclaimed by the main task
    // Spill the windows to create a clean environment to ensure we do not carry over any such references
    // to invalid SPs which will cause problems if main_task does a windowoverflow to them
    xthal_کھڑکی_spill();

    // Cannot be directly called from C; never returns
    __asm__ متطاير ("call0    _frxt_dispatch\n");

    /* Should not get here. */
    أرجع pdTRUE;
}

فراغ v_پورٹ_سرا_Scheduler( فراغ )
{
    ;
}

// ------------------------ Stack --------------------------

// User exception dispatcher when exiting
فراغ _xt_user_exit(فراغ);

#إذا CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
// Wrapper to allow task functions to return (increases stack overhead by 16 bytes)
ساكن فراغ v_پورٹ_Task_Wrapper(TaskFunction_t px_کوڈ, فراغ *pvParameters)
{
    px_کوڈ(pvParameters);
    //FreeRTOS tasks should not return. Log the task name and abort.
    حرف *pcTaskName = pcTaskGetName(عدم);
    ESP_LOGE("FreeRTOS", "FreeRTOS Task \"%s\" should not return, Aborting now!", pcTaskName);
    abort();
}
#جے_انت

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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ڈھیر_CPSA(UBaseType_t ux_ڈھیر_اشارہ_گر)
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
    ux_ڈھیر_اشارہ_گر = ESP_ALIGN_تھلے(ux_ڈھیر_اشارہ_گر - XT_CP_ناپ, 16);
    // Initialize the coprocessor context switching flags.
    uint32_t *p = (uint32_t *)ux_ڈھیر_اشارہ_گر;
    p[0] = 0;   // Clear XT_CPENABLE and XT_CPSTORED
    p[1] = 0;   // Clear XT_CP_CS_ST
    // XT_CP_ASA points to the aligned start of the individual CP save areas (i.e., start of CP0 SA)
    p[2] = (uint32_t)ESP_ALIGN_اتے((uint32_t)ux_ڈھیر_اشارہ_گر + 12, XCHAL_کل_SA_ALIGN);
    أرجع ux_ڈھیر_اشارہ_گر;
}
#جے_انت /* XCHAL_CP_NUM > 0 */

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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ڈھیر_TLS(UBaseType_t ux_ڈھیر_اشارہ_گر, uint32_t *ret_threadptr_reg_init)
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
    خارجي حرف thread_local_انکڑے_شروع, thread_local_انکڑے_سرا;
    خارجي حرف thread_local_bss_شروع, thread_local_bss_سرا;
    ثابت uint32_t tls_انکڑے_ناپ = (uint32_t)&thread_local_انکڑے_سرا - (uint32_t)&thread_local_انکڑے_شروع;
    ثابت uint32_t tls_bss_ناپ = (uint32_t)&thread_local_bss_سرا - (uint32_t)&thread_local_bss_شروع;
    ثابت uint32_t tls_area_ناپ = ESP_ALIGN_اتے(tls_انکڑے_ناپ + tls_bss_ناپ, 16);
    // TODO: check that TLS area fits the stack

    // Allocate space for the TLS area on the stack. The area must be allocated at a 16-byte aligned address
    ux_ڈھیر_اشارہ_گر = ESP_ALIGN_تھلے(ux_ڈھیر_اشارہ_گر - (UBaseType_t)tls_area_ناپ, 16);
    // Initialize the TLS data with the initialization values of each TLS variable
    حافظہ_نقل((فراغ *)ux_ڈھیر_اشارہ_گر, &thread_local_انکڑے_شروع, tls_انکڑے_ناپ);
    // Initialize the TLS bss with zeroes
    memset((فراغ *)(ux_ڈھیر_اشارہ_گر + tls_انکڑے_ناپ), 0, tls_bss_ناپ);

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
    #تعریف_کرو TCB_ناپ 8
    ثابت uint32_t ادھار = ESP_ALIGN_اتے(TCB_ناپ, tls_section_align);
    *ret_threadptr_reg_init = (uint32_t)ux_ڈھیر_اشارہ_گر - ادھار;

    أرجع ux_ڈھیر_اشارہ_گر;
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ڈھیر_چوکھٹا(UBaseType_t ux_ڈھیر_اشارہ_گر, TaskFunction_t px_کوڈ, فراغ *pvParameters, uint32_t threadptr_reg_init)
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
    UBaseType_t ux_ڈھیر_اشارہ_گر_پچھلا = ux_ڈھیر_اشارہ_گر;
    ux_ڈھیر_اشارہ_گر = ESP_ALIGN_تھلے(ux_ڈھیر_اشارہ_گر - XT_STK_FRMSZ, 16);

    // Clear the entire interrupt stack frame
    memset((فراغ *)ux_ڈھیر_اشارہ_گر, 0, (size_t)(ux_ڈھیر_اشارہ_گر_پچھلا - ux_ڈھیر_اشارہ_گر));

    Xt_Exc_چوکھٹا *چوکھٹا = (Xt_Exc_چوکھٹا *)ux_ڈھیر_اشارہ_گر;

    /*
    Initialize common registers
    */
    چوکھٹا->a0 = 0;                                          // Set the return address to 0 terminate GDB backtrace
    چوکھٹا->a1 = ux_ڈھیر_اشارہ_گر + XT_STK_FRMSZ;              // Saved stack pointer should point to physical top of stack frame
    چوکھٹا->باہر_نکلو = (UBaseType_t) _xt_user_exit;              // User exception exit dispatcher

    /*
    Initialize the task's entry point. This will differ depending on
    - Whether the task's entry point is the wrapper function or pxCode
    - Whether Windowed ABI is used (for windowed, we mimic the task entry point being call4'd )
    */
    #إذا CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
        چوکھٹا->pc = (UBaseType_t) v_پورٹ_Task_Wrapper;         // Task entry point is the wrapper function
        #جے_تعریف_شدہ __XTENSA_CALL0_ABI__
            چوکھٹا->a2 = (UBaseType_t) px_کوڈ;               // Wrapper function's argument 0 (which is the task function)
            چوکھٹا->a3 = (UBaseType_t) pvParameters;         // Wrapper function's argument 1 (which is the task function's argument)
        #وإلا // __XTENSA_CALL0_ABI__
            چوکھٹا->a6 = (UBaseType_t) px_کوڈ;               // Wrapper function's argument 0 (which is the task function), passed as if we call4'd
            چوکھٹا->a7 = (UBaseType_t) pvParameters;         // Wrapper function's argument 1 (which is the task function's argument), passed as if we call4'd
        #جے_انت // __XTENSA_CALL0_ABI__
    #وإلا
        چوکھٹا->pc = (UBaseType_t) px_کوڈ;                   // Task entry point is the provided task function
        #جے_تعریف_شدہ __XTENSA_CALL0_ABI__
            چوکھٹا->a2 = (UBaseType_t) pvParameters;         // Task function's argument
        #وإلا // __XTENSA_CALL0_ABI__
            چوکھٹا->a6 = (UBaseType_t) pvParameters;         // Task function's argument, passed as if we call4'd
        #جے_انت // __XTENSA_CALL0_ABI__
    #جے_انت

    /*
    Set initial PS to int level 0, EXCM disabled ('rfe' will enable), user mode.
    For windowed ABI also set WOE and CALLINC (pretend task was 'call4'd)
    */
    #جے_تعریف_شدہ __XTENSA_CALL0_ABI__
        چوکھٹا->ps = PS_UM | PS_EXCM;
    #وإلا // __XTENSA_CALL0_ABI__
        چوکھٹا->ps = PS_UM | PS_EXCM | PS_WOE | PS_CALLINC(1);
    #جے_انت // __XTENSA_CALL0_ABI__

    #جے_تعریف_شدہ XT_USE_SWPRI
        // Set the initial virtual priority mask value to all 1's.
        چوکھٹا->vpri = 0xFFFFFFFF;
    #جے_انت

    // Initialize the threadptr register in the extra save area of the stack frame
    uint32_t *threadptr_reg = (uint32_t *)(ux_ڈھیر_اشارہ_گر + XT_STK_EXTRA);
    *threadptr_reg = threadptr_reg_init;

    أرجع ux_ڈھیر_اشارہ_گر;
}

#إذا ( پورٹ_HAS_ڈھیر_حد_توں_ودھ_CHECKING == 1 )
ڈھیر_Type_t * px_پورٹ_Initialise_ڈھیر( ڈھیر_Type_t * px_چوٹی_دا_ڈھیر,
                                     ڈھیر_Type_t * px_سرا_دا_ڈھیر,
                                     TaskFunction_t px_کوڈ,
                                     فراغ * pvParameters )
#وإلا
ڈھیر_Type_t * px_پورٹ_Initialise_ڈھیر( ڈھیر_Type_t * px_چوٹی_دا_ڈھیر,
                                     TaskFunction_t px_کوڈ,
                                     فراغ * pvParameters )
#جے_انت
{
#جے_تعریف_شدہ __clang_analyzer__
    // Teach clang-tidy that pxTopOfStack cannot be a pointer to const
    متطاير ڈھیر_Type_t * px_عارضی = px_چوٹی_دا_ڈھیر;
    px_چوٹی_دا_ڈھیر = px_عارضی;
#جے_انت /*__clang_analyzer__ */
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

    UBaseType_t ux_ڈھیر_اشارہ_گر = (UBaseType_t)px_چوٹی_دا_ڈھیر;
    configASSERT((ux_ڈھیر_اشارہ_گر & پورٹ_BYTE_ALIGNMENT_نقاب) == 0);

#إذا XCHAL_CP_عدد > 0
    // Initialize the coprocessor save area. THIS MUST BE THE FIRST AREA due to access from _frxt_task_coproc_state()
    ux_ڈھیر_اشارہ_گر = ux_Initialise_ڈھیر_CPSA(ux_ڈھیر_اشارہ_گر);
    configASSERT((ux_ڈھیر_اشارہ_گر & پورٹ_BYTE_ALIGNMENT_نقاب) == 0);
#جے_انت /* XCHAL_CP_NUM > 0 */

    // Initialize the GCC TLS area
    uint32_t threadptr_reg_init;
    ux_ڈھیر_اشارہ_گر = ux_Initialise_ڈھیر_TLS(ux_ڈھیر_اشارہ_گر, &threadptr_reg_init);
    configASSERT((ux_ڈھیر_اشارہ_گر & پورٹ_BYTE_ALIGNMENT_نقاب) == 0);

    // Initialize the starting interrupt stack frame
    ux_ڈھیر_اشارہ_گر = ux_Initialise_ڈھیر_چوکھٹا(ux_ڈھیر_اشارہ_گر, px_کوڈ, pvParameters, threadptr_reg_init);
    configASSERT((ux_ڈھیر_اشارہ_گر & پورٹ_BYTE_ALIGNMENT_نقاب) == 0);

    // Return the task's current stack pointer address which should point to the starting interrupt stack frame
    أرجع (ڈھیر_Type_t *)ux_ڈھیر_اشارہ_گر;
}

// ------------------- Hook Functions ----------------------

#ضمن <stdlib.h>

#إذا ( config_CHECK_FOR_ڈھیر_حد_توں_ودھ > 0 )
فراغ  __attribute__((weak)) v_Application_ڈھیر_حد_توں_ودھ_Hook( TaskHandle_t xTask, حرف *pcTaskName )
{
#تعریف_کرو ERR_STR1 "***ERROR*** A stack overflow in task "
#تعریف_کرو ERR_STR2 " has been detected."
    ثابت حرف *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    حرف buf[حجم(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + حجم(ERR_STR2) + 1 /* null char */] = { 0 };

    حرف *dest = buf;
    لكل (size_t i = 0 ; i < حجم(str) / حجم(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
    esp_system_abort(buf);
}
#جے_انت

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
#جے_انت // CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK

/* ------------------------------------------------ Run Time Stats ------------------------------------------------- */

#إذا ( CONFIG_FREERTOS_GENERATE_لگاتار_ویلا_STATS )

config_لگاتار_ویلا_COUNTER_TYPE x_پورٹ_Get_لگاتار_ویلا_Counter_قدر( فراغ )
{
#جے_تعریف_شدہ CONFIG_FREERTOS_لگاتار_ویلا_STATS_USING_ESP_TIMER
    أرجع (config_لگاتار_ویلا_COUNTER_TYPE) esp_timer_get_ویلا();
#وإلا // Uses CCOUNT
    أرجع (config_لگاتار_ویلا_COUNTER_TYPE) xthal_get_ccount();
#جے_انت // CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
}

#جے_انت /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
