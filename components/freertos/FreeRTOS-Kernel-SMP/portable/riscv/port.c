/*
 * SPDX-FileCopyrightText: 2022-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव "esp_compiler.h"
#अन्तर्भाव "soc/soc_caps.h"
#अन्तर्भाव "soc/periph_defs.h"
#अन्तर्भाव "soc/system_reg.h"
#अन्तर्भाव "hal/crosscore_int_ll.h"
#अन्तर्भाव "hal/systimer_hal.h"
#अन्तर्भाव "hal/systimer_ll.h"
#अन्तर्भाव "riscv/rvruntime-frames.h"
#अन्तर्भाव "riscv/rv_utils.h"
#अन्तर्भाव "riscv/interrupt.h"
#अन्तर्भाव "esp_private/crosscore_int.h"
#अन्तर्भाव "esp_private/esp_int_wdt.h"
#अन्तर्भाव "esp_private/periph_ctrl.h"
#अन्तर्भाव "esp_private/systimer.h"
#अन्तर्भाव "esp_attr.h"
#अन्तर्भाव "esp_system.h"
#अन्तर्भाव "esp_heap_caps_init.h"
#अन्तर्भाव "esp_task_wdt.h"
#अन्तर्भाव "esp_task.h"
#अन्तर्भाव "esp_intr_alloc.h"
#अन्तर्भाव "esp_log.h"
#अन्तर्भाव "FreeRTOS.h"       /* This pulls in portmacro.h */
#अन्तर्भाव "task.h"
#अन्तर्भाव "port_systick.h"
#अन्तर्भाव "portmacro.h"
#अन्तर्भाव "esp_memory_utils.h"
#अन्तर्भाव "esp_macros.h"
#यदि CONFIG_FREERTOS_सततम्_TIME_STATS_USING_ESP_TIMER
#अन्तर्भाव "esp_timer.h"
#यदि_अन्तः
#यदि_परिभाषितम् CONFIG_FREERTOS_SYSTICK_उपयुङ्क्ते_SYSTIMER
#अन्तर्भाव "soc/periph_defs.h"
#अन्तर्भाव "soc/system_reg.h"
#अन्तर्भाव "hal/systimer_hal.h"
#अन्तर्भाव "hal/systimer_ll.h"
#यदि_अन्तः
#यदि_परिभाषितम् CONFIG_PM_TRACE
#अन्तर्भाव "esp_private/pm_trace.h"
#यदि_अन्तः //CONFIG_PM_TRACE

_Static_assert(पत्तनम्_BYTE_ALIGNMENT == 16, "portBYTE_ALIGNMENT must be set to 16");
#यदि CONFIG_ESP_SYSTEM_HW_राशिः_GUARD
/**
 * offsetof() can not be used in asm code. Then we need make sure that
 * PORT_OFFSET_PX_STACK and PORT_OFFSET_PX_END_OF_STACK have expected values.
 * Macro used in the portasm.S instead of variables to save at least 4 instruction calls
 * which accessing DRAM memory. This optimization saves CPU time in the interrupt handling.
 */

_Static_assert(offsetof( StaticTask_t, pxDummy6 ) == पत्तनम्_विस्थापनम्_PX_राशिः);
_Static_assert(offsetof( StaticTask_t, pxDummy8 ) == पत्तनम्_विस्थापनम्_PX_अन्तः_सम्बन्धि_राशिः);
#यदि_अन्तः // CONFIG_ESP_SYSTEM_HW_STACK_GUARD

/* ---------------------------------------------------- Variables ------------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

आधारः_Type_t uxSchedulerRunning = 0;  // Duplicate of xSchedulerRunning, accessible to port files
चञ्चल UBaseType_t ux_व्यवधानम्_Nesting = 0;
#यदि config_अङ्कः_CORES > 1
चञ्चल अचिह्नित पत्तनम्_ux_Core_Startup_समाप्तम्[पत्तनम्_अङ्कः_PROCESSORS] = {0};  // Indicates whether the core has completed its startup sequence
#यदि_अन्तः
पत्तनम्_MUX_TYPE पत्तनम्_x_Task_तालकम् = पत्तनम्_MUX_INITIALIZER_UNLOCKED;
पत्तनम्_MUX_TYPE पत्तनम्_x_ISRLock = पत्तनम्_MUX_INITIALIZER_UNLOCKED;
चञ्चल आधारः_Type_t x_पत्तनम्_Switch_Flag = 0;
__attribute__((aligned(16))) राशिः_Type_t x_Isr_राशिः[config_ISR_राशिः_आकार];
राशिः_Type_t *x_Isr_राशिः_शिखरम् = &x_Isr_राशिः[0] + (config_ISR_राशिः_आकार & (~((पत्तनम्_POINTER_आकार_TYPE)पत्तनम्_BYTE_ALIGNMENT_आवरणम्)));

// Variables used for IDF style critical sections. These are orthogonal to FreeRTOS critical sections
स्थिर UBaseType_t पत्तनम्_ux_Critical_Nesting_IDF = 0;
स्थिर UBaseType_t पत्तनम्_ux_Critical_Old_व्यवधानम्_State_IDF = 0;
#यदि CONFIG_FREERTOS_पत्तनम्_THREAD_SAFE_CLAIM
चञ्चल bool पत्तनम्_x_Thread_Safe_Claimed = false;
#यदि_अन्तः

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// ------------------ Critical Sections --------------------

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

शून्य v_पत्तनम्_Enter_Critical(शून्य)
{
#यदि CONFIG_FREERTOS_पत्तनम्_THREAD_SAFE_CLAIM
    यदि (unlikely(पत्तनम्_x_Thread_Safe_Claimed)) {
        प्रत्यागम;
    }
#यदि_अन्तः
    // Save current interrupt threshold and disable interrupts
    UBaseType_t old_thresh = ul_पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्();
    // Update the IDF critical nesting count
    पत्तनम्_ux_Critical_Nesting_IDF++;
    यदि (पत्तनम्_ux_Critical_Nesting_IDF == 1) {
        // Save a copy of the old interrupt threshold
        पत्तनम्_ux_Critical_Old_व्यवधानम्_State_IDF = (UBaseType_t) old_thresh;
    }
}

शून्य v_पत्तनम्_Exit_Critical(शून्य)
{
#यदि CONFIG_FREERTOS_पत्तनम्_THREAD_SAFE_CLAIM
    यदि (unlikely(पत्तनम्_x_Thread_Safe_Claimed)) {
        प्रत्यागम;
    }
#यदि_अन्तः

    /* Critical section nesting coung must never be negative */
    configASSERT( पत्तनम्_ux_Critical_Nesting_IDF > 0 );

    यदि (पत्तनम्_ux_Critical_Nesting_IDF > 0) {
        पत्तनम्_ux_Critical_Nesting_IDF--;

        यदि (पत्तनम्_ux_Critical_Nesting_IDF == 0) {
            // Restore the saved interrupt threshold
            v_पत्तनम्_Clear_व्यवधानम्_आवरणम्((पूर्णाङ्क)पत्तनम्_ux_Critical_Old_व्यवधानम्_State_IDF);
        }
    }
}

// ----------------------- System --------------------------

#परिभाषय राशिः_WATCH_AREA_आकार 32
#परिभाषय राशिः_WATCH_POINT_सङ्ख्या (SOC_CPU_WATCHPOINTS_अङ्कः - 1)

शून्य v_पत्तनम्_समुच्चयः_राशिः_Watchpoint(शून्य *px_राशिः_Start)
{
    uint32_t addr = (uint32_t)px_राशिः_Start;
    addr = (addr + (राशिः_WATCH_AREA_आकार - 1)) & (~(राशिः_WATCH_AREA_आकार - 1));
    esp_cpu_समुच्चयः_watchpoint(राशिः_WATCH_POINT_सङ्ख्या, (वर्ण *)addr, राशिः_WATCH_AREA_आकार, ESP_CPU_WATCHPOINT_STORE);
}



/* ---------------------------------------------- Port Implementations -------------------------------------------------
 * Implementations of Porting Interface functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

UBaseType_t ul_पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्(शून्य)
{
    UBaseType_t पूर्व_int_level = 0, int_level = 0;
#यदि !SOC_INT_CLIC_SUPPORTED
    int_level = RVHAL_EXCM_LEVEL;
#अन्यथा
    int_level = RVHAL_EXCM_LEVEL_CLIC;
#यदि_अन्तः

    पूर्व_int_level = rv_utils_समुच्चयः_intlevel_regval(int_level);
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
    प्रत्यागम पूर्व_int_level;
}

शून्य v_पत्तनम्_Clear_व्यवधानम्_आवरणम्(UBaseType_t आवरणम्)
{
    rv_utils_restore_intlevel_regval(आवरणम्);
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
    asm चञ्चल ( "nop" );
    asm चञ्चल ( "nop" );
    asm चञ्चल ( "nop" );
}

आधारः_Type_t x_पत्तनम्_Check_If_अन्तर्_ISR(शून्य)
{
    प्रत्यागम ux_व्यवधानम्_Nesting;
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

शून्य v_पत्तनम्_Yield(शून्य)
{
    // TODO: IDF-8113
    ध्रुव पूर्णाङ्क core_id = 0;

    यदि (ux_व्यवधानम्_Nesting) {
        v_पत्तनम्_Yield_From_ISR();
    } अन्यथा {

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
        यावत् (uxSchedulerRunning && crosscore_int_ll_get_state(core_id) != 0) {}
    }
}

शून्य v_पत्तनम्_Yield_From_ISR( शून्य )
{
    //traceISR_EXIT_TO_SCHEDULER();
    uxSchedulerRunning = 1;
    x_पत्तनम्_Switch_Flag = 1;
}
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
                ESP_EARLY_LOGE("FreeRTOS", "Fatal error: TLSP deletion callback at index %d overwritten with non-excutable pointer %p", x, pv_Thread_Local_Storage_सूचकाः_Del_Callback[ x ]);
                abort();
            }

            pv_Thread_Local_Storage_सूचकाः_Del_Callback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#यदि_अन्तः /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

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
}

/* ------------------------------------------------ FreeRTOS Portable --------------------------------------------------
 * - Provides implementation for functions required by FreeRTOS
 * - Declared in portable.h
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------- Scheduler Start/End -------------------

आधारः_Type_t x_पत्तनम्_Start_Scheduler(शून्य)
{
    ux_व्यवधानम्_Nesting = 0;
    पत्तनम्_ux_Critical_Nesting_IDF = 0;
#यदि CONFIG_FREERTOS_पत्तनम्_THREAD_SAFE_CLAIM
    पत्तनम्_x_Thread_Safe_Claimed = false;
#यदि_अन्तः
    uxSchedulerRunning = 0;
#यदि config_अङ्कः_CORES > 1
    पत्तनम्_ux_Core_Startup_समाप्तम्[x_पत्तनम्_Get_Core_ID()] = 0;
#यदि_अन्तः
    /* Setup the hardware to generate the tick. */
    v_पत्तनम्_सज्जता_Timer();

    rv_utils_intr_global_enable();
    esprv_int_समुच्चयः_देहली(RVHAL_INTR_ENABLE_THRESH); /* set global interrupt masking level */

    v_पत्तनम्_Yield();

    /*Should not get here*/
    प्रत्यागम pdFALSE;
}

शून्य v_पत्तनम्_अन्तः_Scheduler(शून्य)
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_राशिः_TLS(UBaseType_t ux_राशिः_Pointer, uint32_t *ret_threadptr_reg_init)
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
    बाह्य वर्ण thread_local_दत्त_start, thread_local_दत्त_अन्तः;
    बाह्य वर्ण _thread_local_bss_start, thread_local_bss_अन्तः;
    ध्रुव uint32_t tls_दत्त_आकार = (uint32_t)&thread_local_दत्त_अन्तः - (uint32_t)&thread_local_दत्त_start;
    ध्रुव uint32_t tls_bss_आकार = (uint32_t)&thread_local_bss_अन्तः - (uint32_t)&_thread_local_bss_start;
    ध्रुव uint32_t tls_area_आकार = ESP_ALIGN_ऊर्ध्वम्(tls_दत्त_आकार + tls_bss_आकार, 16);
    // TODO: check that TLS area fits the stack

    // Allocate space for the TLS area on the stack. The area must be aligned to 16-bytes
    ux_राशिः_Pointer = ESP_ALIGN_अधस्तात्(ux_राशिः_Pointer - (UBaseType_t)tls_area_आकार, 16);
    // Initialize the TLS data with the initialization values of each TLS variable
    स्मृतिं_प्रतिलिख((शून्य *)ux_राशिः_Pointer, &thread_local_दत्त_start, tls_दत्त_आकार);
    // Initialize the TLS bss with zeroes
    स्मृतिं_पूरय((शून्य *)(ux_राशिः_Pointer + tls_दत्त_आकार), 0, tls_bss_आकार);

    // Save tls start address
    *ret_threadptr_reg_init = (uint32_t)ux_राशिः_Pointer;
    प्रत्यागम ux_राशिः_Pointer;
}

#यदि CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
स्थिर शून्य v_पत्तनम्_Task_Wrapper(TaskFunction_t px_संहिता, शून्य *pvParameters)
{
    __asm__ चञ्चल(".cfi_undefined ra");  // tell to debugger that it's outermost (initial) frame
    बाह्य शून्य __attribute__((noreturn)) panic_abort(ध्रुव वर्ण *details);
    स्थिर वर्ण DRAM_ATTR सन्देशः[80] = "FreeRTOS: FreeRTOS Task \"\0";
    px_संहिता(pvParameters);
    /* FreeRTOS tasks should not return. Log the task name and abort. */
    /* We cannot use s(n)printf because it is in flash */
    वाक्यं_योजय(सन्देशः, pcTaskGetName(रिक्त));
    वाक्यं_योजय(सन्देशः, "\" should not return, Aborting now!");
    panic_abort(सन्देशः);
}
#यदि_अन्तः // CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER

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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_राशिः_Frame(UBaseType_t ux_राशिः_Pointer, TaskFunction_t px_संहिता, शून्य *pvParameters, uint32_t threadptr_reg_init)
{
    /*
    Allocate space for the task's starting interrupt stack frame.
    - The stack frame must be allocated to a 16-byte aligned address.
    - We use RV_STK_FRMSZ (instead of sizeof(RvExcFrame)) as it rounds up the total size to a multiple of 16.
    */
    ux_राशिः_Pointer = ESP_ALIGN_अधस्तात्(ux_राशिः_Pointer - RV_STK_FRMSZ, 16);

    // Clear the entire interrupt stack frame
    RvExcFrame *frame = (RvExcFrame *)ux_राशिः_Pointer;
    स्मृतिं_पूरय(frame, 0, परिमाण(RvExcFrame));

    /* Initialize the stack frame. */
    बाह्य uint32_t __global_pointer$;
    #यदि CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
        frame->mepc = (UBaseType_t)v_पत्तनम्_Task_Wrapper;
        frame->a0 = (UBaseType_t)px_संहिता;
        frame->a1 = (UBaseType_t)pvParameters;
    #अन्यथा
        frame->mepc = (UBaseType_t)px_संहिता;
        frame->a0 = (UBaseType_t)pvParameters;
    #यदि_अन्तः // CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
    frame->gp = (UBaseType_t)&__global_pointer$;
    frame->tp = (UBaseType_t)threadptr_reg_init;

    प्रत्यागम ux_राशिः_Pointer;
}

राशिः_Type_t *px_पत्तनम्_Initialise_राशिः(राशिः_Type_t *px_शिखरम्_सम्बन्धि_राशिः, TaskFunction_t px_संहिता, शून्य *pvParameters)
{
#यदि_परिभाषितम् __clang_analyzer__
    // Teach clang-tidy that pxTopOfStack cannot be a pointer to const
    चञ्चल राशिः_Type_t * px_अस्थायी = px_शिखरम्_सम्बन्धि_राशिः;
    px_शिखरम्_सम्बन्धि_राशिः = px_अस्थायी;
#यदि_अन्तः /*__clang_analyzer__ */
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

    UBaseType_t ux_राशिः_Pointer = (UBaseType_t)px_शिखरम्_सम्बन्धि_राशिः;
    configASSERT((ux_राशिः_Pointer & पत्तनम्_BYTE_ALIGNMENT_आवरणम्) == 0);

    // Initialize GCC TLS area
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

शून्य __attribute__((weak)) v_Application_राशिः_अतिप्रवाहः_Hook(TaskHandle_t xTask, वर्ण *pcTaskName)
{
#परिभाषय ERR_STR1 "***ERROR*** A stack overflow in task "
#परिभाषय ERR_STR2 " has been detected."
    ध्रुव वर्ण *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    वर्ण buf[परिमाण(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + परिमाण(ERR_STR2) + 1 /* null char */] = {0};

    वर्ण *dest = buf;
    प्रति (पूर्णाङ्क i = 0; i < परिमाण(str) / परिमाण(str[0]); i++) {
        dest = वाक्यं_योजय(dest, str[i]);
    }
    esp_system_abort(buf);
}

#यदि  (  configUSE_TICK_HOOK > 0 )
शून्य vApplicationTickHook( शून्य )
{
    esp_vApplicationTickHook();
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
#अन्यथा
    प्रत्यागम 0;
#यदि_अन्तः // CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
}

#यदि_अन्तः /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
