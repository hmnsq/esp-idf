/*
 * SPDX-FileCopyrightText: 2022-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "sdkconfig.h"
#ضمن <stdbool.h>
#ضمن <string.h>
#ضمن "esp_compiler.h"
#ضمن "soc/soc_caps.h"
#ضمن "soc/periph_defs.h"
#ضمن "soc/system_reg.h"
#ضمن "hal/crosscore_int_ll.h"
#ضمن "hal/systimer_hal.h"
#ضمن "hal/systimer_ll.h"
#ضمن "riscv/rvruntime-frames.h"
#ضمن "riscv/rv_utils.h"
#ضمن "riscv/interrupt.h"
#ضمن "esp_private/crosscore_int.h"
#ضمن "esp_private/esp_int_wdt.h"
#ضمن "esp_private/periph_ctrl.h"
#ضمن "esp_private/systimer.h"
#ضمن "esp_attr.h"
#ضمن "esp_system.h"
#ضمن "esp_heap_caps_init.h"
#ضمن "esp_task_wdt.h"
#ضمن "esp_task.h"
#ضمن "esp_intr_alloc.h"
#ضمن "esp_log.h"
#ضمن "FreeRTOS.h"       /* This pulls in portmacro.h */
#ضمن "task.h"
#ضمن "port_systick.h"
#ضمن "portmacro.h"
#ضمن "esp_memory_utils.h"
#ضمن "esp_macros.h"
#إذا CONFIG_FREERTOS_لگاتار_ویلا_STATS_USING_ESP_TIMER
#ضمن "esp_timer.h"
#جے_انت
#جے_تعریف_شدہ CONFIG_FREERTOS_SYSTICK_ورتدا_SYSTIMER
#ضمن "soc/periph_defs.h"
#ضمن "soc/system_reg.h"
#ضمن "hal/systimer_hal.h"
#ضمن "hal/systimer_ll.h"
#جے_انت
#جے_تعریف_شدہ CONFIG_PM_TRACE
#ضمن "esp_private/pm_trace.h"
#جے_انت //CONFIG_PM_TRACE

_Static_assert(پورٹ_BYTE_ALIGNMENT == 16, "portBYTE_ALIGNMENT must be set to 16");
#إذا CONFIG_ESP_SYSTEM_HW_ڈھیر_GUARD
/**
 * offsetof() can not be used in asm code. Then we need make sure that
 * PORT_OFFSET_PX_STACK and PORT_OFFSET_PX_END_OF_STACK have expected values.
 * Macro used in the portasm.S instead of variables to save at least 4 instruction calls
 * which accessing DRAM memory. This optimization saves CPU time in the interrupt handling.
 */

_Static_assert(offsetof( StaticTask_t, pxDummy6 ) == پورٹ_وتھ_PX_ڈھیر);
_Static_assert(offsetof( StaticTask_t, pxDummy8 ) == پورٹ_وتھ_PX_سرا_دا_ڈھیر);
#جے_انت // CONFIG_ESP_SYSTEM_HW_STACK_GUARD

/* ---------------------------------------------------- Variables ------------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

ادھار_Type_t uxSchedulerRunning = 0;  // Duplicate of xSchedulerRunning, accessible to port files
متطاير UBaseType_t ux_رکاوٹ_Nesting = 0;
#إذا config_عدد_CORES > 1
متطاير بنانشانی پورٹ_ux_Core_Startup_پورا[پورٹ_عدد_PROCESSORS] = {0};  // Indicates whether the core has completed its startup sequence
#جے_انت
پورٹ_MUX_TYPE پورٹ_x_Task_جندرا = پورٹ_MUX_INITIALIZER_UNLOCKED;
پورٹ_MUX_TYPE پورٹ_x_ISRLock = پورٹ_MUX_INITIALIZER_UNLOCKED;
متطاير ادھار_Type_t x_پورٹ_Switch_Flag = 0;
__attribute__((aligned(16))) ڈھیر_Type_t x_Isr_ڈھیر[config_ISR_ڈھیر_ناپ];
ڈھیر_Type_t *x_Isr_ڈھیر_چوٹی = &x_Isr_ڈھیر[0] + (config_ISR_ڈھیر_ناپ & (~((پورٹ_اشارہ_گر_ناپ_TYPE)پورٹ_BYTE_ALIGNMENT_نقاب)));

// Variables used for IDF style critical sections. These are orthogonal to FreeRTOS critical sections
ساكن UBaseType_t پورٹ_ux_Critical_Nesting_IDF = 0;
ساكن UBaseType_t پورٹ_ux_Critical_Old_رکاوٹ_State_IDF = 0;
#إذا CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
متطاير bool پورٹ_x_Thread_محفوظ_Claimed = false;
#جے_انت

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// ------------------ Critical Sections --------------------

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

فراغ v_پورٹ_Enter_Critical(فراغ)
{
#إذا CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
    إذا (unlikely(پورٹ_x_Thread_محفوظ_Claimed)) {
        أرجع;
    }
#جے_انت
    // Save current interrupt threshold and disable interrupts
    UBaseType_t old_thresh = ul_پورٹ_سیٹ_رکاوٹ_نقاب();
    // Update the IDF critical nesting count
    پورٹ_ux_Critical_Nesting_IDF++;
    إذا (پورٹ_ux_Critical_Nesting_IDF == 1) {
        // Save a copy of the old interrupt threshold
        پورٹ_ux_Critical_Old_رکاوٹ_State_IDF = (UBaseType_t) old_thresh;
    }
}

فراغ v_پورٹ_Exit_Critical(فراغ)
{
#إذا CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
    إذا (unlikely(پورٹ_x_Thread_محفوظ_Claimed)) {
        أرجع;
    }
#جے_انت

    /* Critical section nesting coung must never be negative */
    configASSERT( پورٹ_ux_Critical_Nesting_IDF > 0 );

    إذا (پورٹ_ux_Critical_Nesting_IDF > 0) {
        پورٹ_ux_Critical_Nesting_IDF--;

        إذا (پورٹ_ux_Critical_Nesting_IDF == 0) {
            // Restore the saved interrupt threshold
            v_پورٹ_Clear_رکاوٹ_نقاب((صحيح)پورٹ_ux_Critical_Old_رکاوٹ_State_IDF);
        }
    }
}

// ----------------------- System --------------------------

#تعریف_کرو ڈھیر_WATCH_AREA_ناپ 32
#تعریف_کرو ڈھیر_WATCH_نقطہ_نمبر (SOC_CPU_WATCHPOINTS_عدد - 1)

فراغ v_پورٹ_سیٹ_ڈھیر_Watchpoint(فراغ *px_ڈھیر_شروع)
{
    uint32_t addr = (uint32_t)px_ڈھیر_شروع;
    addr = (addr + (ڈھیر_WATCH_AREA_ناپ - 1)) & (~(ڈھیر_WATCH_AREA_ناپ - 1));
    esp_cpu_سیٹ_watchpoint(ڈھیر_WATCH_نقطہ_نمبر, (حرف *)addr, ڈھیر_WATCH_AREA_ناپ, ESP_CPU_WATCHPOINT_STORE);
}



/* ---------------------------------------------- Port Implementations -------------------------------------------------
 * Implementations of Porting Interface functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

UBaseType_t ul_پورٹ_سیٹ_رکاوٹ_نقاب(فراغ)
{
    UBaseType_t prev_int_level = 0, int_level = 0;
#إذا !SOC_INT_CLIC_SUPPORTED
    int_level = RVHAL_EXCM_LEVEL;
#وإلا
    int_level = RVHAL_EXCM_LEVEL_CLIC;
#جے_انت

    prev_int_level = rv_utils_سیٹ_intlevel_regval(int_level);
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
    أرجع prev_int_level;
}

فراغ v_پورٹ_Clear_رکاوٹ_نقاب(UBaseType_t نقاب)
{
    rv_utils_restore_intlevel_regval(نقاب);
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
    asm متطاير ( "nop" );
    asm متطاير ( "nop" );
    asm متطاير ( "nop" );
}

ادھار_Type_t x_پورٹ_Check_If_وچ_ISR(فراغ)
{
    أرجع ux_رکاوٹ_Nesting;
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

فراغ v_پورٹ_Yield(فراغ)
{
    // TODO: IDF-8113
    ثابت صحيح core_id = 0;

    إذا (ux_رکاوٹ_Nesting) {
        v_پورٹ_Yield_From_ISR();
    } وإلا {

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
        طالما (uxSchedulerRunning && crosscore_int_ll_get_state(core_id) != 0) {}
    }
}

فراغ v_پورٹ_Yield_From_ISR( فراغ )
{
    //traceISR_EXIT_TO_SCHEDULER();
    uxSchedulerRunning = 1;
    x_پورٹ_Switch_Flag = 1;
}
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
                ESP_EARLY_LOGE("FreeRTOS", "Fatal error: TLSP deletion callback at index %d overwritten with non-excutable pointer %p", x, pv_Thread_Local_Storage_سوچکاں_Del_Callback[ x ]);
                abort();
            }

            pv_Thread_Local_Storage_سوچکاں_Del_Callback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#جے_انت /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

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
}

/* ------------------------------------------------ FreeRTOS Portable --------------------------------------------------
 * - Provides implementation for functions required by FreeRTOS
 * - Declared in portable.h
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------- Scheduler Start/End -------------------

ادھار_Type_t x_پورٹ_شروع_Scheduler(فراغ)
{
    ux_رکاوٹ_Nesting = 0;
    پورٹ_ux_Critical_Nesting_IDF = 0;
#إذا CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
    پورٹ_x_Thread_محفوظ_Claimed = false;
#جے_انت
    uxSchedulerRunning = 0;
#إذا config_عدد_CORES > 1
    پورٹ_ux_Core_Startup_پورا[x_پورٹ_Get_Core_ID()] = 0;
#جے_انت
    /* Setup the hardware to generate the tick. */
    v_پورٹ_تیاری_Timer();

    rv_utils_intr_global_enable();
    esprv_int_سیٹ_حد(RVHAL_INTR_ENABLE_THRESH); /* set global interrupt masking level */

    v_پورٹ_Yield();

    /*Should not get here*/
    أرجع pdFALSE;
}

فراغ v_پورٹ_سرا_Scheduler(فراغ)
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ڈھیر_TLS(UBaseType_t ux_ڈھیر_اشارہ_گر, uint32_t *ret_threadptr_reg_init)
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
    خارجي حرف thread_local_انکڑے_شروع, thread_local_انکڑے_سرا;
    خارجي حرف thread_local_bss_شروع, thread_local_bss_سرا;
    ثابت uint32_t tls_انکڑے_ناپ = (uint32_t)&thread_local_انکڑے_سرا - (uint32_t)&thread_local_انکڑے_شروع;
    ثابت uint32_t tls_bss_ناپ = (uint32_t)&thread_local_bss_سرا - (uint32_t)&thread_local_bss_شروع;
    ثابت uint32_t tls_area_ناپ = ESP_ALIGN_اتے(tls_انکڑے_ناپ + tls_bss_ناپ, 16);
    // TODO: check that TLS area fits the stack

    // Allocate space for the TLS area on the stack. The area must be aligned to 16-bytes
    ux_ڈھیر_اشارہ_گر = ESP_ALIGN_تھلے(ux_ڈھیر_اشارہ_گر - (UBaseType_t)tls_area_ناپ, 16);
    // Initialize the TLS data with the initialization values of each TLS variable
    حافظہ_نقل((فراغ *)ux_ڈھیر_اشارہ_گر, &thread_local_انکڑے_شروع, tls_انکڑے_ناپ);
    // Initialize the TLS bss with zeroes
    memset((فراغ *)(ux_ڈھیر_اشارہ_گر + tls_انکڑے_ناپ), 0, tls_bss_ناپ);

    // Save tls start address
    *ret_threadptr_reg_init = (uint32_t)ux_ڈھیر_اشارہ_گر;
    أرجع ux_ڈھیر_اشارہ_گر;
}

#إذا CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
ساكن فراغ v_پورٹ_Task_Wrapper(TaskFunction_t px_کوڈ, فراغ *pvParameters)
{
    __asm__ متطاير(".cfi_undefined ra");  // tell to debugger that it's outermost (initial) frame
    خارجي فراغ __attribute__((noreturn)) panic_abort(ثابت حرف *details);
    ساكن حرف DRAM_ATTR پیغام[80] = "FreeRTOS: FreeRTOS Task \"\0";
    px_کوڈ(pvParameters);
    /* FreeRTOS tasks should not return. Log the task name and abort. */
    /* We cannot use s(n)printf because it is in flash */
    strcat(پیغام, pcTaskGetName(عدم));
    strcat(پیغام, "\" should not return, Aborting now!");
    panic_abort(پیغام);
}
#جے_انت // CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER

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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ڈھیر_چوکھٹا(UBaseType_t ux_ڈھیر_اشارہ_گر, TaskFunction_t px_کوڈ, فراغ *pvParameters, uint32_t threadptr_reg_init)
{
    /*
    Allocate space for the task's starting interrupt stack frame.
    - The stack frame must be allocated to a 16-byte aligned address.
    - We use RV_STK_FRMSZ (instead of sizeof(RvExcFrame)) as it rounds up the total size to a multiple of 16.
    */
    ux_ڈھیر_اشارہ_گر = ESP_ALIGN_تھلے(ux_ڈھیر_اشارہ_گر - RV_STK_FRMSZ, 16);

    // Clear the entire interrupt stack frame
    Rv_Exc_چوکھٹا *چوکھٹا = (Rv_Exc_چوکھٹا *)ux_ڈھیر_اشارہ_گر;
    memset(چوکھٹا, 0, حجم(Rv_Exc_چوکھٹا));

    /* Initialize the stack frame. */
    خارجي uint32_t __global_pointer$;
    #إذا CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
        چوکھٹا->mepc = (UBaseType_t)v_پورٹ_Task_Wrapper;
        چوکھٹا->a0 = (UBaseType_t)px_کوڈ;
        چوکھٹا->a1 = (UBaseType_t)pvParameters;
    #وإلا
        چوکھٹا->mepc = (UBaseType_t)px_کوڈ;
        چوکھٹا->a0 = (UBaseType_t)pvParameters;
    #جے_انت // CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
    چوکھٹا->gp = (UBaseType_t)&__global_pointer$;
    چوکھٹا->tp = (UBaseType_t)threadptr_reg_init;

    أرجع ux_ڈھیر_اشارہ_گر;
}

ڈھیر_Type_t *px_پورٹ_Initialise_ڈھیر(ڈھیر_Type_t *px_چوٹی_دا_ڈھیر, TaskFunction_t px_کوڈ, فراغ *pvParameters)
{
#جے_تعریف_شدہ __clang_analyzer__
    // Teach clang-tidy that pxTopOfStack cannot be a pointer to const
    متطاير ڈھیر_Type_t * px_عارضی = px_چوٹی_دا_ڈھیر;
    px_چوٹی_دا_ڈھیر = px_عارضی;
#جے_انت /*__clang_analyzer__ */
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

    UBaseType_t ux_ڈھیر_اشارہ_گر = (UBaseType_t)px_چوٹی_دا_ڈھیر;
    configASSERT((ux_ڈھیر_اشارہ_گر & پورٹ_BYTE_ALIGNMENT_نقاب) == 0);

    // Initialize GCC TLS area
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

فراغ __attribute__((weak)) v_Application_ڈھیر_حد_توں_ودھ_Hook(TaskHandle_t xTask, حرف *pcTaskName)
{
#تعریف_کرو ERR_STR1 "***ERROR*** A stack overflow in task "
#تعریف_کرو ERR_STR2 " has been detected."
    ثابت حرف *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    حرف buf[حجم(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + حجم(ERR_STR2) + 1 /* null char */] = {0};

    حرف *dest = buf;
    لكل (صحيح i = 0; i < حجم(str) / حجم(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
    esp_system_abort(buf);
}

#إذا  (  configUSE_TICK_HOOK > 0 )
فراغ vApplicationTickHook( فراغ )
{
    esp_vApplicationTickHook();
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
#وإلا
    أرجع 0;
#جے_انت // CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
}

#جے_انت /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
