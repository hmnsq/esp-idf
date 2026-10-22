/*
 * SPDX-FileCopyrightText: 2022-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ <stdbool.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ "esp_compiler.h"
#అన్తర్భావ "soc/soc_caps.h"
#అన్తర్భావ "soc/periph_defs.h"
#అన్తర్భావ "soc/system_reg.h"
#అన్తర్భావ "hal/crosscore_int_ll.h"
#అన్తర్భావ "hal/systimer_hal.h"
#అన్తర్భావ "hal/systimer_ll.h"
#అన్తర్భావ "riscv/rvruntime-frames.h"
#అన్తర్భావ "riscv/rv_utils.h"
#అన్తర్భావ "riscv/interrupt.h"
#అన్తర్భావ "esp_private/crosscore_int.h"
#అన్తర్భావ "esp_private/esp_int_wdt.h"
#అన్తర్భావ "esp_private/periph_ctrl.h"
#అన్తర్భావ "esp_private/systimer.h"
#అన్తర్భావ "esp_attr.h"
#అన్తర్భావ "esp_system.h"
#అన్తర్భావ "esp_heap_caps_init.h"
#అన్తర్భావ "esp_task_wdt.h"
#అన్తర్భావ "esp_task.h"
#అన్తర్భావ "esp_intr_alloc.h"
#అన్తర్భావ "esp_log.h"
#అన్తర్భావ "FreeRTOS.h"       /* This pulls in portmacro.h */
#అన్తర్భావ "task.h"
#అన్తర్భావ "port_systick.h"
#అన్తర్భావ "portmacro.h"
#అన్తర్భావ "esp_memory_utils.h"
#అన్తర్భావ "esp_macros.h"
#ఒకవేళ CONFIG_FREERTOS_నిరంతరం_సమయం_STATS_USING_ESP_TIMER
#అన్తర్భావ "esp_timer.h"
#అయితే_ముగింపు
#నిర్వచితమైతే CONFIG_FREERTOS_SYSTICK_ఉపయోగిస్తుంది_SYSTIMER
#అన్తర్భావ "soc/periph_defs.h"
#అన్తర్భావ "soc/system_reg.h"
#అన్తర్భావ "hal/systimer_hal.h"
#అన్తర్భావ "hal/systimer_ll.h"
#అయితే_ముగింపు
#నిర్వచితమైతే CONFIG_PM_TRACE
#అన్తర్భావ "esp_private/pm_trace.h"
#అయితే_ముగింపు //CONFIG_PM_TRACE

_Static_assert(పోర్టు_BYTE_ALIGNMENT == 16, "portBYTE_ALIGNMENT must be set to 16");
#ఒకవేళ CONFIG_ESP_SYSTEM_HW_దొంతర_GUARD
/**
 * offsetof() can not be used in asm code. Then we need make sure that
 * PORT_OFFSET_PX_STACK and PORT_OFFSET_PX_END_OF_STACK have expected values.
 * Macro used in the portasm.S instead of variables to save at least 4 instruction calls
 * which accessing DRAM memory. This optimization saves CPU time in the interrupt handling.
 */

_Static_assert(offsetof( StaticTask_t, pxDummy6 ) == పోర్టు_స్థానభ్రంశం_PX_దొంతర);
_Static_assert(offsetof( StaticTask_t, pxDummy8 ) == పోర్టు_స్థానభ్రంశం_PX_చివర_యొక్క_దొంతర);
#అయితే_ముగింపు // CONFIG_ESP_SYSTEM_HW_STACK_GUARD

/* ---------------------------------------------------- Variables ------------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

పునాది_Type_t uxSchedulerRunning = 0;  // Duplicate of xSchedulerRunning, accessible to port files
అస్థిర UBaseType_t ux_అంతరాయం_Nesting = 0;
#ఒకవేళ config_అంకె_CORES > 1
అస్థిర గుర్తులేని పోర్టు_ux_Core_Startup_పూర్తి[పోర్టు_అంకె_PROCESSORS] = {0};  // Indicates whether the core has completed its startup sequence
#అయితే_ముగింపు
పోర్టు_MUX_TYPE పోర్టు_x_Task_తాళం = పోర్టు_MUX_INITIALIZER_UNLOCKED;
పోర్టు_MUX_TYPE పోర్టు_x_ISRLock = పోర్టు_MUX_INITIALIZER_UNLOCKED;
అస్థిర పునాది_Type_t x_పోర్టు_Switch_Flag = 0;
__attribute__((aligned(16))) దొంతర_Type_t x_Isr_దొంతర[config_ISR_దొంతర_కొలత];
దొంతర_Type_t *x_Isr_దొంతర_శిఖరం = &x_Isr_దొంతర[0] + (config_ISR_దొంతర_కొలత & (~((పోర్టు_సూచిక_కొలత_TYPE)పోర్టు_BYTE_ALIGNMENT_ముసుగు)));

// Variables used for IDF style critical sections. These are orthogonal to FreeRTOS critical sections
స్థిర UBaseType_t పోర్టు_ux_Critical_Nesting_IDF = 0;
స్థిర UBaseType_t పోర్టు_ux_Critical_Old_అంతరాయం_State_IDF = 0;
#ఒకవేళ CONFIG_FREERTOS_పోర్టు_THREAD_సురక్షిత_CLAIM
అస్థిర bool పోర్టు_x_Thread_సురక్షిత_Claimed = false;
#అయితే_ముగింపు

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// ------------------ Critical Sections --------------------

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

శూన్య v_పోర్టు_Enter_Critical(శూన్య)
{
#ఒకవేళ CONFIG_FREERTOS_పోర్టు_THREAD_సురక్షిత_CLAIM
    ఒకవేళ (unlikely(పోర్టు_x_Thread_సురక్షిత_Claimed)) {
        తిరిగి;
    }
#అయితే_ముగింపు
    // Save current interrupt threshold and disable interrupts
    UBaseType_t old_thresh = ul_పోర్టు_సమితి_అంతరాయం_ముసుగు();
    // Update the IDF critical nesting count
    పోర్టు_ux_Critical_Nesting_IDF++;
    ఒకవేళ (పోర్టు_ux_Critical_Nesting_IDF == 1) {
        // Save a copy of the old interrupt threshold
        పోర్టు_ux_Critical_Old_అంతరాయం_State_IDF = (UBaseType_t) old_thresh;
    }
}

శూన్య v_పోర్టు_Exit_Critical(శూన్య)
{
#ఒకవేళ CONFIG_FREERTOS_పోర్టు_THREAD_సురక్షిత_CLAIM
    ఒకవేళ (unlikely(పోర్టు_x_Thread_సురక్షిత_Claimed)) {
        తిరిగి;
    }
#అయితే_ముగింపు

    /* Critical section nesting coung must never be negative */
    configASSERT( పోర్టు_ux_Critical_Nesting_IDF > 0 );

    ఒకవేళ (పోర్టు_ux_Critical_Nesting_IDF > 0) {
        పోర్టు_ux_Critical_Nesting_IDF--;

        ఒకవేళ (పోర్టు_ux_Critical_Nesting_IDF == 0) {
            // Restore the saved interrupt threshold
            v_పోర్టు_Clear_అంతరాయం_ముసుగు((పూర్ణాంకం)పోర్టు_ux_Critical_Old_అంతరాయం_State_IDF);
        }
    }
}

// ----------------------- System --------------------------

#నిర్వచించు దొంతర_WATCH_AREA_కొలత 32
#నిర్వచించు దొంతర_WATCH_బిందువు_సంఖ్య (SOC_CPU_WATCHPOINTS_అంకె - 1)

శూన్య v_పోర్టు_సమితి_దొంతర_Watchpoint(శూన్య *px_దొంతర_ప్రారంభం)
{
    uint32_t addr = (uint32_t)px_దొంతర_ప్రారంభం;
    addr = (addr + (దొంతర_WATCH_AREA_కొలత - 1)) & (~(దొంతర_WATCH_AREA_కొలత - 1));
    esp_cpu_సమితి_watchpoint(దొంతర_WATCH_బిందువు_సంఖ్య, (అక్షరం *)addr, దొంతర_WATCH_AREA_కొలత, ESP_CPU_WATCHPOINT_STORE);
}



/* ---------------------------------------------- Port Implementations -------------------------------------------------
 * Implementations of Porting Interface functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

UBaseType_t ul_పోర్టు_సమితి_అంతరాయం_ముసుగు(శూన్య)
{
    UBaseType_t prev_int_level = 0, int_level = 0;
#ఒకవేళ !SOC_INT_CLIC_SUPPORTED
    int_level = RVHAL_EXCM_LEVEL;
#లేకపోతే
    int_level = RVHAL_EXCM_LEVEL_CLIC;
#అయితే_ముగింపు

    prev_int_level = rv_utils_సమితి_intlevel_regval(int_level);
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
    తిరిగి prev_int_level;
}

శూన్య v_పోర్టు_Clear_అంతరాయం_ముసుగు(UBaseType_t ముసుగు)
{
    rv_utils_restore_intlevel_regval(ముసుగు);
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
    asm అస్థిర ( "nop" );
    asm అస్థిర ( "nop" );
    asm అస్థిర ( "nop" );
}

పునాది_Type_t x_పోర్టు_Check_If_లోపల_ISR(శూన్య)
{
    తిరిగి ux_అంతరాయం_Nesting;
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

శూన్య v_పోర్టు_Yield(శూన్య)
{
    // TODO: IDF-8113
    స్థిరరాశి పూర్ణాంకం core_id = 0;

    ఒకవేళ (ux_అంతరాయం_Nesting) {
        v_పోర్టు_Yield_From_ISR();
    } లేకపోతే {

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
        అయితే (uxSchedulerRunning && crosscore_int_ll_get_state(core_id) != 0) {}
    }
}

శూన్య v_పోర్టు_Yield_From_ISR( శూన్య )
{
    //traceISR_EXIT_TO_SCHEDULER();
    uxSchedulerRunning = 1;
    x_పోర్టు_Switch_Flag = 1;
}
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
                ESP_EARLY_LOGE("FreeRTOS", "Fatal error: TLSP deletion callback at index %d overwritten with non-excutable pointer %p", x, pv_Thread_Local_Storage_సూచికలు_Del_Callback[ x ]);
                abort();
            }

            pv_Thread_Local_Storage_సూచికలు_Del_Callback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#అయితే_ముగింపు /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

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
}

/* ------------------------------------------------ FreeRTOS Portable --------------------------------------------------
 * - Provides implementation for functions required by FreeRTOS
 * - Declared in portable.h
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------- Scheduler Start/End -------------------

పునాది_Type_t x_పోర్టు_ప్రారంభం_Scheduler(శూన్య)
{
    ux_అంతరాయం_Nesting = 0;
    పోర్టు_ux_Critical_Nesting_IDF = 0;
#ఒకవేళ CONFIG_FREERTOS_పోర్టు_THREAD_సురక్షిత_CLAIM
    పోర్టు_x_Thread_సురక్షిత_Claimed = false;
#అయితే_ముగింపు
    uxSchedulerRunning = 0;
#ఒకవేళ config_అంకె_CORES > 1
    పోర్టు_ux_Core_Startup_పూర్తి[x_పోర్టు_Get_Core_ID()] = 0;
#అయితే_ముగింపు
    /* Setup the hardware to generate the tick. */
    v_పోర్టు_అమరిక_Timer();

    rv_utils_intr_global_enable();
    esprv_int_సమితి_పరిమితి(RVHAL_INTR_ENABLE_THRESH); /* set global interrupt masking level */

    v_పోర్టు_Yield();

    /*Should not get here*/
    తిరిగి pdFALSE;
}

శూన్య v_పోర్టు_చివర_Scheduler(శూన్య)
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_దొంతర_TLS(UBaseType_t ux_దొంతర_సూచిక, uint32_t *ret_threadptr_reg_init)
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
    బాహ్య అక్షరం thread_local_దత్తాంశం_ప్రారంభం, thread_local_దత్తాంశం_చివర;
    బాహ్య అక్షరం thread_local_bss_ప్రారంభం, thread_local_bss_చివర;
    స్థిరరాశి uint32_t tls_దత్తాంశం_కొలత = (uint32_t)&thread_local_దత్తాంశం_చివర - (uint32_t)&thread_local_దత్తాంశం_ప్రారంభం;
    స్థిరరాశి uint32_t tls_bss_కొలత = (uint32_t)&thread_local_bss_చివర - (uint32_t)&thread_local_bss_ప్రారంభం;
    స్థిరరాశి uint32_t tls_area_కొలత = ESP_ALIGN_పైకి(tls_దత్తాంశం_కొలత + tls_bss_కొలత, 16);
    // TODO: check that TLS area fits the stack

    // Allocate space for the TLS area on the stack. The area must be aligned to 16-bytes
    ux_దొంతర_సూచిక = ESP_ALIGN_కిందికి(ux_దొంతర_సూచిక - (UBaseType_t)tls_area_కొలత, 16);
    // Initialize the TLS data with the initialization values of each TLS variable
    జ్ఞాపకం_నకలు((శూన్య *)ux_దొంతర_సూచిక, &thread_local_దత్తాంశం_ప్రారంభం, tls_దత్తాంశం_కొలత);
    // Initialize the TLS bss with zeroes
    memset((శూన్య *)(ux_దొంతర_సూచిక + tls_దత్తాంశం_కొలత), 0, tls_bss_కొలత);

    // Save tls start address
    *ret_threadptr_reg_init = (uint32_t)ux_దొంతర_సూచిక;
    తిరిగి ux_దొంతర_సూచిక;
}

#ఒకవేళ CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
స్థిర శూన్య v_పోర్టు_Task_Wrapper(TaskFunction_t px_సంకేతం, శూన్య *pvParameters)
{
    __asm__ అస్థిర(".cfi_undefined ra");  // tell to debugger that it's outermost (initial) frame
    బాహ్య శూన్య __attribute__((noreturn)) panic_abort(స్థిరరాశి అక్షరం *details);
    స్థిర అక్షరం DRAM_ATTR సందేశం[80] = "FreeRTOS: FreeRTOS Task \"\0";
    px_సంకేతం(pvParameters);
    /* FreeRTOS tasks should not return. Log the task name and abort. */
    /* We cannot use s(n)printf because it is in flash */
    strcat(సందేశం, pcTaskGetName(ఖాళీ));
    strcat(సందేశం, "\" should not return, Aborting now!");
    panic_abort(సందేశం);
}
#అయితే_ముగింపు // CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER

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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_దొంతర_చట్రం(UBaseType_t ux_దొంతర_సూచిక, TaskFunction_t px_సంకేతం, శూన్య *pvParameters, uint32_t threadptr_reg_init)
{
    /*
    Allocate space for the task's starting interrupt stack frame.
    - The stack frame must be allocated to a 16-byte aligned address.
    - We use RV_STK_FRMSZ (instead of sizeof(RvExcFrame)) as it rounds up the total size to a multiple of 16.
    */
    ux_దొంతర_సూచిక = ESP_ALIGN_కిందికి(ux_దొంతర_సూచిక - RV_STK_FRMSZ, 16);

    // Clear the entire interrupt stack frame
    Rv_Exc_చట్రం *చట్రం = (Rv_Exc_చట్రం *)ux_దొంతర_సూచిక;
    memset(చట్రం, 0, పరిమాణం(Rv_Exc_చట్రం));

    /* Initialize the stack frame. */
    బాహ్య uint32_t __global_pointer$;
    #ఒకవేళ CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
        చట్రం->mepc = (UBaseType_t)v_పోర్టు_Task_Wrapper;
        చట్రం->a0 = (UBaseType_t)px_సంకేతం;
        చట్రం->a1 = (UBaseType_t)pvParameters;
    #లేకపోతే
        చట్రం->mepc = (UBaseType_t)px_సంకేతం;
        చట్రం->a0 = (UBaseType_t)pvParameters;
    #అయితే_ముగింపు // CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
    చట్రం->gp = (UBaseType_t)&__global_pointer$;
    చట్రం->tp = (UBaseType_t)threadptr_reg_init;

    తిరిగి ux_దొంతర_సూచిక;
}

దొంతర_Type_t *px_పోర్టు_Initialise_దొంతర(దొంతర_Type_t *px_శిఖరం_యొక్క_దొంతర, TaskFunction_t px_సంకేతం, శూన్య *pvParameters)
{
#నిర్వచితమైతే __clang_analyzer__
    // Teach clang-tidy that pxTopOfStack cannot be a pointer to const
    అస్థిర దొంతర_Type_t * px_తాత్కాలిక = px_శిఖరం_యొక్క_దొంతర;
    px_శిఖరం_యొక్క_దొంతర = px_తాత్కాలిక;
#అయితే_ముగింపు /*__clang_analyzer__ */
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

    UBaseType_t ux_దొంతర_సూచిక = (UBaseType_t)px_శిఖరం_యొక్క_దొంతర;
    configASSERT((ux_దొంతర_సూచిక & పోర్టు_BYTE_ALIGNMENT_ముసుగు) == 0);

    // Initialize GCC TLS area
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

శూన్య __attribute__((weak)) v_Application_దొంతర_అధికప్రవాహం_Hook(TaskHandle_t xTask, అక్షరం *pcTaskName)
{
#నిర్వచించు ERR_STR1 "***ERROR*** A stack overflow in task "
#నిర్వచించు ERR_STR2 " has been detected."
    స్థిరరాశి అక్షరం *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    అక్షరం buf[పరిమాణం(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + పరిమాణం(ERR_STR2) + 1 /* null char */] = {0};

    అక్షరం *dest = buf;
    కొరకు (పూర్ణాంకం i = 0; i < పరిమాణం(str) / పరిమాణం(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
    esp_system_abort(buf);
}

#ఒకవేళ  (  configUSE_TICK_HOOK > 0 )
శూన్య vApplicationTickHook( శూన్య )
{
    esp_vApplicationTickHook();
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
#లేకపోతే
    తిరిగి 0;
#అయితే_ముగింపు // CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
}

#అయితే_ముగింపు /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
