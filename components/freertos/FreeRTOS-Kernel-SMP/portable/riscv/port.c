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
#إذا CONFIG_FREERTOS_لاڳيتو_وقت_STATS_USING_ESP_TIMER
#ضمن "esp_timer.h"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل CONFIG_FREERTOS_SYSTICK_استعمال_ڪري_SYSTIMER
#ضمن "soc/periph_defs.h"
#ضمن "soc/system_reg.h"
#ضمن "hal/systimer_hal.h"
#ضمن "hal/systimer_ll.h"
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل CONFIG_PM_TRACE
#ضمن "esp_private/pm_trace.h"
#جيڪڏهن_پورو //CONFIG_PM_TRACE

_Static_assert(پورٽ_BYTE_ALIGNMENT == 16, "portBYTE_ALIGNMENT must be set to 16");
#إذا CONFIG_ESP_SYSTEM_HW_ڍير_GUARD
/**
 * offsetof() can not be used in asm code. Then we need make sure that
 * PORT_OFFSET_PX_STACK and PORT_OFFSET_PX_END_OF_STACK have expected values.
 * Macro used in the portasm.S instead of variables to save at least 4 instruction calls
 * which accessing DRAM memory. This optimization saves CPU time in the interrupt handling.
 */

_Static_assert(offsetof( StaticTask_t, pxDummy6 ) == پورٽ_وٿي_PX_ڍير);
_Static_assert(offsetof( StaticTask_t, pxDummy8 ) == پورٽ_وٿي_PX_پڇاڙي_جو_ڍير);
#جيڪڏهن_پورو // CONFIG_ESP_SYSTEM_HW_STACK_GUARD

/* ---------------------------------------------------- Variables ------------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

بنياد_Type_t uxSchedulerRunning = 0;  // Duplicate of xSchedulerRunning, accessible to port files
متطاير UBaseType_t ux_مداخلت_Nesting = 0;
#إذا config_انگ_CORES > 1
متطاير بنانشان پورٽ_ux_Core_Startup_پورو[پورٽ_انگ_PROCESSORS] = {0};  // Indicates whether the core has completed its startup sequence
#جيڪڏهن_پورو
پورٽ_MUX_TYPE پورٽ_x_Task_تالو = پورٽ_MUX_INITIALIZER_UNLOCKED;
پورٽ_MUX_TYPE پورٽ_x_ISRLock = پورٽ_MUX_INITIALIZER_UNLOCKED;
متطاير بنياد_Type_t x_پورٽ_Switch_Flag = 0;
__attribute__((aligned(16))) ڍير_Type_t x_Isr_ڍير[config_ISR_ڍير_سائيز];
ڍير_Type_t *x_Isr_ڍير_چوٽي = &x_Isr_ڍير[0] + (config_ISR_ڍير_سائيز & (~((پورٽ_اشارو_سائيز_TYPE)پورٽ_BYTE_ALIGNMENT_نقاب)));

// Variables used for IDF style critical sections. These are orthogonal to FreeRTOS critical sections
ساكن UBaseType_t پورٽ_ux_Critical_Nesting_IDF = 0;
ساكن UBaseType_t پورٽ_ux_Critical_Old_مداخلت_State_IDF = 0;
#إذا CONFIG_FREERTOS_پورٽ_THREAD_محفوظ_CLAIM
متطاير bool پورٽ_x_Thread_محفوظ_Claimed = false;
#جيڪڏهن_پورو

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// ------------------ Critical Sections --------------------

#إذا CONFIG_FREERTOS_پورٽ_THREAD_محفوظ_CLAIM
فراغ x_پورٽ_Thread_محفوظ_Claim(فراغ)
{
    configASSERT(!x_پورٽ_Can_Yield());
    configASSERT(!پورٽ_x_Thread_محفوظ_Claimed);
    پورٽ_x_Thread_محفوظ_Claimed = true;
}

فراغ x_پورٽ_Thread_محفوظ_Disclaim(فراغ)
{
    configASSERT(!x_پورٽ_Can_Yield());
    configASSERT(پورٽ_x_Thread_محفوظ_Claimed);
    پورٽ_x_Thread_محفوظ_Claimed = false;
}
#جيڪڏهن_پورو /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

فراغ v_پورٽ_Enter_Critical(فراغ)
{
#إذا CONFIG_FREERTOS_پورٽ_THREAD_محفوظ_CLAIM
    إذا (unlikely(پورٽ_x_Thread_محفوظ_Claimed)) {
        أرجع;
    }
#جيڪڏهن_پورو
    // Save current interrupt threshold and disable interrupts
    UBaseType_t old_thresh = ul_پورٽ_سيٽ_مداخلت_نقاب();
    // Update the IDF critical nesting count
    پورٽ_ux_Critical_Nesting_IDF++;
    إذا (پورٽ_ux_Critical_Nesting_IDF == 1) {
        // Save a copy of the old interrupt threshold
        پورٽ_ux_Critical_Old_مداخلت_State_IDF = (UBaseType_t) old_thresh;
    }
}

فراغ v_پورٽ_Exit_Critical(فراغ)
{
#إذا CONFIG_FREERTOS_پورٽ_THREAD_محفوظ_CLAIM
    إذا (unlikely(پورٽ_x_Thread_محفوظ_Claimed)) {
        أرجع;
    }
#جيڪڏهن_پورو

    /* Critical section nesting coung must never be negative */
    configASSERT( پورٽ_ux_Critical_Nesting_IDF > 0 );

    إذا (پورٽ_ux_Critical_Nesting_IDF > 0) {
        پورٽ_ux_Critical_Nesting_IDF--;

        إذا (پورٽ_ux_Critical_Nesting_IDF == 0) {
            // Restore the saved interrupt threshold
            v_پورٽ_Clear_مداخلت_نقاب((صحيح)پورٽ_ux_Critical_Old_مداخلت_State_IDF);
        }
    }
}

// ----------------------- System --------------------------

#وصف_ڪريو ڍير_WATCH_AREA_سائيز 32
#وصف_ڪريو ڍير_WATCH_نقطو_عدد (SOC_CPU_WATCHPOINTS_انگ - 1)

فراغ v_پورٽ_سيٽ_ڍير_Watchpoint(فراغ *px_ڍير_شروعات)
{
    uint32_t addr = (uint32_t)px_ڍير_شروعات;
    addr = (addr + (ڍير_WATCH_AREA_سائيز - 1)) & (~(ڍير_WATCH_AREA_سائيز - 1));
    esp_cpu_سيٽ_watchpoint(ڍير_WATCH_نقطو_عدد, (حرف *)addr, ڍير_WATCH_AREA_سائيز, ESP_CPU_WATCHPOINT_STORE);
}



/* ---------------------------------------------- Port Implementations -------------------------------------------------
 * Implementations of Porting Interface functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

UBaseType_t ul_پورٽ_سيٽ_مداخلت_نقاب(فراغ)
{
    UBaseType_t prev_int_level = 0, int_level = 0;
#إذا !SOC_INT_CLIC_SUPPORTED
    int_level = RVHAL_EXCM_LEVEL;
#وإلا
    int_level = RVHAL_EXCM_LEVEL_CLIC;
#جيڪڏهن_پورو

    prev_int_level = rv_utils_سيٽ_intlevel_regval(int_level);
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

فراغ v_پورٽ_Clear_مداخلت_نقاب(UBaseType_t نقاب)
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

بنياد_Type_t x_پورٽ_Check_If_اندريون_ISR(فراغ)
{
    أرجع ux_مداخلت_Nesting;
}

فراغ v_پورٽ_Assert_If_اندريون_ISR(فراغ)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(x_پورٽ_Check_If_اندريون_ISR() == 0);
}

// ------------------ Critical Sections --------------------

#إذا ( config_عدد_جو_CORES > 1 )
فراغ v_پورٽ_Take_تالو( پورٽ_MUX_TYPE *تالو )
{
    spinlock_acquire( تالو, پورٽ_MUX_NO_TIMEOUT);
}

فراغ v_پورٽ_Release_تالو( پورٽ_MUX_TYPE *تالو )
{
    spinlock_release( تالو );
}
#جيڪڏهن_پورو /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

فراغ v_پورٽ_Yield(فراغ)
{
    // TODO: IDF-8113
    ثابت صحيح core_id = 0;

    إذا (ux_مداخلت_Nesting) {
        v_پورٽ_Yield_From_ISR();
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

فراغ v_پورٽ_Yield_From_ISR( فراغ )
{
    //traceISR_EXIT_TO_SCHEDULER();
    uxSchedulerRunning = 1;
    x_پورٽ_Switch_Flag = 1;
}
// ----------------------- System --------------------------

// ------------------- Run Time Stats ----------------------

// --------------------- TCB Cleanup -----------------------

#إذا ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
ساكن فراغ v_پورٽ_TLSPointers_Del_Cb( فراغ *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pv_Thread_Local_Storage_اشارا_Del_Callback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_انگ_THREAD_LOCAL_STORAGE_اشارا / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    لكل ( صحيح x = 0; x < ( config_انگ_THREAD_LOCAL_STORAGE_اشارا / 2 ); x++ ) {
        إذا ( pv_Thread_Local_Storage_اشارا_Del_Callback[ x ] != عدم ) {  //If del cb is set
            /* In case the TLSP deletion callback has been overwritten by a TLS pointer, gracefully abort. */
            إذا ( !esp_ptr_executable( pv_Thread_Local_Storage_اشارا_Del_Callback[ x ] ) ) {
                ESP_EARLY_LOGE("FreeRTOS", "Fatal error: TLSP deletion callback at index %d overwritten with non-excutable pointer %p", x, pv_Thread_Local_Storage_اشارا_Del_Callback[ x ]);
                abort();
            }

            pv_Thread_Local_Storage_اشارا_Del_Callback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#جيڪڏهن_پورو /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

فراغ v_پورٽ_TCBPre_Delete_Hook( فراغ *pxTCB )
{
    #إذا ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        خارجي فراغ vTaskPreDeletionHook( فراغ * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #جيڪڏهن_پورو /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

    #إذا ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
        /* Call TLS pointers deletion callbacks */
        v_پورٽ_TLSPointers_Del_Cb( pxTCB );
    #جيڪڏهن_پورو /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
}

/* ------------------------------------------------ FreeRTOS Portable --------------------------------------------------
 * - Provides implementation for functions required by FreeRTOS
 * - Declared in portable.h
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------- Scheduler Start/End -------------------

بنياد_Type_t x_پورٽ_شروعات_Scheduler(فراغ)
{
    ux_مداخلت_Nesting = 0;
    پورٽ_ux_Critical_Nesting_IDF = 0;
#إذا CONFIG_FREERTOS_پورٽ_THREAD_محفوظ_CLAIM
    پورٽ_x_Thread_محفوظ_Claimed = false;
#جيڪڏهن_پورو
    uxSchedulerRunning = 0;
#إذا config_انگ_CORES > 1
    پورٽ_ux_Core_Startup_پورو[x_پورٽ_Get_Core_ID()] = 0;
#جيڪڏهن_پورو
    /* Setup the hardware to generate the tick. */
    v_پورٽ_تياري_Timer();

    rv_utils_intr_global_enable();
    esprv_int_سيٽ_حد(RVHAL_INTR_ENABLE_THRESH); /* set global interrupt masking level */

    v_پورٽ_Yield();

    /*Should not get here*/
    أرجع pdFALSE;
}

فراغ v_پورٽ_پڇاڙي_Scheduler(فراغ)
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ڍير_TLS(UBaseType_t ux_ڍير_اشارو, uint32_t *ret_threadptr_reg_init)
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
    خارجي حرف thread_local_معلومات_شروعات, thread_local_معلومات_پڇاڙي;
    خارجي حرف thread_local_bss_شروعات, thread_local_bss_پڇاڙي;
    ثابت uint32_t tls_معلومات_سائيز = (uint32_t)&thread_local_معلومات_پڇاڙي - (uint32_t)&thread_local_معلومات_شروعات;
    ثابت uint32_t tls_bss_سائيز = (uint32_t)&thread_local_bss_پڇاڙي - (uint32_t)&thread_local_bss_شروعات;
    ثابت uint32_t tls_area_سائيز = ESP_ALIGN_مٿي(tls_معلومات_سائيز + tls_bss_سائيز, 16);
    // TODO: check that TLS area fits the stack

    // Allocate space for the TLS area on the stack. The area must be aligned to 16-bytes
    ux_ڍير_اشارو = ESP_ALIGN_هيٺ_ڏانهن(ux_ڍير_اشارو - (UBaseType_t)tls_area_سائيز, 16);
    // Initialize the TLS data with the initialization values of each TLS variable
    يادگيري_نقل((فراغ *)ux_ڍير_اشارو, &thread_local_معلومات_شروعات, tls_معلومات_سائيز);
    // Initialize the TLS bss with zeroes
    memset((فراغ *)(ux_ڍير_اشارو + tls_معلومات_سائيز), 0, tls_bss_سائيز);

    // Save tls start address
    *ret_threadptr_reg_init = (uint32_t)ux_ڍير_اشارو;
    أرجع ux_ڍير_اشارو;
}

#إذا CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
ساكن فراغ v_پورٽ_Task_Wrapper(TaskFunction_t px_ڪوڊ, فراغ *pvParameters)
{
    __asm__ متطاير(".cfi_undefined ra");  // tell to debugger that it's outermost (initial) frame
    خارجي فراغ __attribute__((noreturn)) panic_abort(ثابت حرف *details);
    ساكن حرف DRAM_ATTR نياپو[80] = "FreeRTOS: FreeRTOS Task \"\0";
    px_ڪوڊ(pvParameters);
    /* FreeRTOS tasks should not return. Log the task name and abort. */
    /* We cannot use s(n)printf because it is in flash */
    strcat(نياپو, pcTaskGetName(عدم));
    strcat(نياپو, "\" should not return, Aborting now!");
    panic_abort(نياپو);
}
#جيڪڏهن_پورو // CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER

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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ڍير_چوکٺو(UBaseType_t ux_ڍير_اشارو, TaskFunction_t px_ڪوڊ, فراغ *pvParameters, uint32_t threadptr_reg_init)
{
    /*
    Allocate space for the task's starting interrupt stack frame.
    - The stack frame must be allocated to a 16-byte aligned address.
    - We use RV_STK_FRMSZ (instead of sizeof(RvExcFrame)) as it rounds up the total size to a multiple of 16.
    */
    ux_ڍير_اشارو = ESP_ALIGN_هيٺ_ڏانهن(ux_ڍير_اشارو - RV_STK_FRMSZ, 16);

    // Clear the entire interrupt stack frame
    Rv_Exc_چوکٺو *چوکٺو = (Rv_Exc_چوکٺو *)ux_ڍير_اشارو;
    memset(چوکٺو, 0, حجم(Rv_Exc_چوکٺو));

    /* Initialize the stack frame. */
    خارجي uint32_t __global_pointer$;
    #إذا CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
        چوکٺو->mepc = (UBaseType_t)v_پورٽ_Task_Wrapper;
        چوکٺو->a0 = (UBaseType_t)px_ڪوڊ;
        چوکٺو->a1 = (UBaseType_t)pvParameters;
    #وإلا
        چوکٺو->mepc = (UBaseType_t)px_ڪوڊ;
        چوکٺو->a0 = (UBaseType_t)pvParameters;
    #جيڪڏهن_پورو // CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
    چوکٺو->gp = (UBaseType_t)&__global_pointer$;
    چوکٺو->tp = (UBaseType_t)threadptr_reg_init;

    أرجع ux_ڍير_اشارو;
}

ڍير_Type_t *px_پورٽ_Initialise_ڍير(ڍير_Type_t *px_چوٽي_جو_ڍير, TaskFunction_t px_ڪوڊ, فراغ *pvParameters)
{
#جيڪڏهن_وصف_ٿيل __clang_analyzer__
    // Teach clang-tidy that pxTopOfStack cannot be a pointer to const
    متطاير ڍير_Type_t * px_عارضي = px_چوٽي_جو_ڍير;
    px_چوٽي_جو_ڍير = px_عارضي;
#جيڪڏهن_پورو /*__clang_analyzer__ */
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

    UBaseType_t ux_ڍير_اشارو = (UBaseType_t)px_چوٽي_جو_ڍير;
    configASSERT((ux_ڍير_اشارو & پورٽ_BYTE_ALIGNMENT_نقاب) == 0);

    // Initialize GCC TLS area
    uint32_t threadptr_reg_init;
    ux_ڍير_اشارو = ux_Initialise_ڍير_TLS(ux_ڍير_اشارو, &threadptr_reg_init);
    configASSERT((ux_ڍير_اشارو & پورٽ_BYTE_ALIGNMENT_نقاب) == 0);

    // Initialize the starting interrupt stack frame
    ux_ڍير_اشارو = ux_Initialise_ڍير_چوکٺو(ux_ڍير_اشارو, px_ڪوڊ, pvParameters, threadptr_reg_init);
    configASSERT((ux_ڍير_اشارو & پورٽ_BYTE_ALIGNMENT_نقاب) == 0);

    // Return the task's current stack pointer address which should point to the starting interrupt stack frame
    أرجع (ڍير_Type_t *)ux_ڍير_اشارو;
}

// ------------------- Hook Functions ----------------------

فراغ __attribute__((weak)) v_Application_ڍير_حد_کان_مٿي_Hook(TaskHandle_t xTask, حرف *pcTaskName)
{
#وصف_ڪريو ERR_STR1 "***ERROR*** A stack overflow in task "
#وصف_ڪريو ERR_STR2 " has been detected."
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
#جيڪڏهن_پورو

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
#جيڪڏهن_پورو // CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK

/* ------------------------------------------------ Run Time Stats ------------------------------------------------- */

#إذا ( CONFIG_FREERTOS_GENERATE_لاڳيتو_وقت_STATS )

config_لاڳيتو_وقت_COUNTER_TYPE x_پورٽ_Get_لاڳيتو_وقت_Counter_قدر( فراغ )
{
#جيڪڏهن_وصف_ٿيل CONFIG_FREERTOS_لاڳيتو_وقت_STATS_USING_ESP_TIMER
    أرجع (config_لاڳيتو_وقت_COUNTER_TYPE) esp_timer_get_وقت();
#وإلا
    أرجع 0;
#جيڪڏهن_پورو // CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
}

#جيڪڏهن_پورو /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
