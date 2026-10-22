/*
 * SPDX-FileCopyrightText: 2022-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#شامل "sdkconfig.h"
#شامل <stdbool.h>
#شامل <string.h>
#شامل "esp_compiler.h"
#شامل "soc/soc_caps.h"
#شامل "soc/periph_defs.h"
#شامل "soc/system_reg.h"
#شامل "hal/crosscore_int_ll.h"
#شامل "hal/systimer_hal.h"
#شامل "hal/systimer_ll.h"
#شامل "riscv/rvruntime-frames.h"
#شامل "riscv/rv_utils.h"
#شامل "riscv/interrupt.h"
#شامل "esp_private/crosscore_int.h"
#شامل "esp_private/esp_int_wdt.h"
#شامل "esp_private/periph_ctrl.h"
#شامل "esp_private/systimer.h"
#شامل "esp_attr.h"
#شامل "esp_system.h"
#شامل "esp_heap_caps_init.h"
#شامل "esp_task_wdt.h"
#شامل "esp_task.h"
#شامل "esp_intr_alloc.h"
#شامل "esp_log.h"
#شامل "FreeRTOS.h"       /* This pulls in portmacro.h */
#شامل "task.h"
#شامل "port_systick.h"
#شامل "portmacro.h"
#شامل "esp_memory_utils.h"
#شامل "esp_macros.h"
#اگر CONFIG_FREERTOS_تسلسل_وقت_STATS_USING_ESP_TIMER
#شامل "esp_timer.h"
#اگر_ختم
#اگر_تعریف_شدہ CONFIG_FREERTOS_SYSTICK_استعمال_کرتا_SYSTIMER
#شامل "soc/periph_defs.h"
#شامل "soc/system_reg.h"
#شامل "hal/systimer_hal.h"
#شامل "hal/systimer_ll.h"
#اگر_ختم
#اگر_تعریف_شدہ CONFIG_PM_TRACE
#شامل "esp_private/pm_trace.h"
#اگر_ختم //CONFIG_PM_TRACE

_Static_assert(پورٹ_BYTE_ALIGNMENT == 16, "portBYTE_ALIGNMENT must be set to 16");
#اگر CONFIG_ESP_SYSTEM_HW_ڈھیر_GUARD
/**
 * offsetof() can not be used in asm code. Then we need make sure that
 * PORT_OFFSET_PX_STACK and PORT_OFFSET_PX_END_OF_STACK have expected values.
 * Macro used in the portasm.S instead of variables to save at least 4 instruction calls
 * which accessing DRAM memory. This optimization saves CPU time in the interrupt handling.
 */

_Static_assert(offsetof( StaticTask_t, pxDummy6 ) == پورٹ_انحراف_PX_ڈھیر);
_Static_assert(offsetof( StaticTask_t, pxDummy8 ) == پورٹ_انحراف_PX_انتہا_کا_ڈھیر);
#اگر_ختم // CONFIG_ESP_SYSTEM_HW_STACK_GUARD

/* ---------------------------------------------------- Variables ------------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

بنیاد_Type_t uxSchedulerRunning = 0;  // Duplicate of xSchedulerRunning, accessible to port files
متلون UBaseType_t ux_مداخلت_Nesting = 0;
#اگر config_عدد_CORES > 1
متلون غیرعلامتی پورٹ_ux_Core_Startup_مکمل[پورٹ_عدد_PROCESSORS] = {0};  // Indicates whether the core has completed its startup sequence
#اگر_ختم
پورٹ_MUX_TYPE پورٹ_x_Task_تالا = پورٹ_MUX_INITIALIZER_UNLOCKED;
پورٹ_MUX_TYPE پورٹ_x_ISRLock = پورٹ_MUX_INITIALIZER_UNLOCKED;
متلون بنیاد_Type_t x_پورٹ_Switch_Flag = 0;
__attribute__((aligned(16))) ڈھیر_Type_t x_Isr_ڈھیر[config_ISR_ڈھیر_سائز];
ڈھیر_Type_t *x_Isr_ڈھیر_چوٹی = &x_Isr_ڈھیر[0] + (config_ISR_ڈھیر_سائز & (~((پورٹ_اشارہ_گر_سائز_TYPE)پورٹ_BYTE_ALIGNMENT_نقاب)));

// Variables used for IDF style critical sections. These are orthogonal to FreeRTOS critical sections
ساکن UBaseType_t پورٹ_ux_Critical_Nesting_IDF = 0;
ساکن UBaseType_t پورٹ_ux_Critical_Old_مداخلت_State_IDF = 0;
#اگر CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
متلون bool پورٹ_x_Thread_محفوظ_Claimed = false;
#اگر_ختم

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// ------------------ Critical Sections --------------------

#اگر CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
خالی x_پورٹ_Thread_محفوظ_Claim(خالی)
{
    configASSERT(!x_پورٹ_Can_Yield());
    configASSERT(!پورٹ_x_Thread_محفوظ_Claimed);
    پورٹ_x_Thread_محفوظ_Claimed = true;
}

خالی x_پورٹ_Thread_محفوظ_Disclaim(خالی)
{
    configASSERT(!x_پورٹ_Can_Yield());
    configASSERT(پورٹ_x_Thread_محفوظ_Claimed);
    پورٹ_x_Thread_محفوظ_Claimed = false;
}
#اگر_ختم /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

خالی v_پورٹ_Enter_Critical(خالی)
{
#اگر CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
    اگر (unlikely(پورٹ_x_Thread_محفوظ_Claimed)) {
        واپس;
    }
#اگر_ختم
    // Save current interrupt threshold and disable interrupts
    UBaseType_t old_thresh = ul_پورٹ_سیٹ_مداخلت_نقاب();
    // Update the IDF critical nesting count
    پورٹ_ux_Critical_Nesting_IDF++;
    اگر (پورٹ_ux_Critical_Nesting_IDF == 1) {
        // Save a copy of the old interrupt threshold
        پورٹ_ux_Critical_Old_مداخلت_State_IDF = (UBaseType_t) old_thresh;
    }
}

خالی v_پورٹ_Exit_Critical(خالی)
{
#اگر CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
    اگر (unlikely(پورٹ_x_Thread_محفوظ_Claimed)) {
        واپس;
    }
#اگر_ختم

    /* Critical section nesting coung must never be negative */
    configASSERT( پورٹ_ux_Critical_Nesting_IDF > 0 );

    اگر (پورٹ_ux_Critical_Nesting_IDF > 0) {
        پورٹ_ux_Critical_Nesting_IDF--;

        اگر (پورٹ_ux_Critical_Nesting_IDF == 0) {
            // Restore the saved interrupt threshold
            v_پورٹ_Clear_مداخلت_نقاب((صحیح)پورٹ_ux_Critical_Old_مداخلت_State_IDF);
        }
    }
}

// ----------------------- System --------------------------

#تعریف_کرو ڈھیر_WATCH_AREA_سائز 32
#تعریف_کرو ڈھیر_WATCH_نقطہ_نمبر (SOC_CPU_WATCHPOINTS_عدد - 1)

خالی v_پورٹ_سیٹ_ڈھیر_Watchpoint(خالی *px_ڈھیر_آغاز)
{
    uint32_t addr = (uint32_t)px_ڈھیر_آغاز;
    addr = (addr + (ڈھیر_WATCH_AREA_سائز - 1)) & (~(ڈھیر_WATCH_AREA_سائز - 1));
    esp_cpu_سیٹ_watchpoint(ڈھیر_WATCH_نقطہ_نمبر, (حرف *)addr, ڈھیر_WATCH_AREA_سائز, ESP_CPU_WATCHPOINT_STORE);
}



/* ---------------------------------------------- Port Implementations -------------------------------------------------
 * Implementations of Porting Interface functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

UBaseType_t ul_پورٹ_سیٹ_مداخلت_نقاب(خالی)
{
    UBaseType_t پچھلا_int_level = 0, int_level = 0;
#اگر !SOC_INT_CLIC_SUPPORTED
    int_level = RVHAL_EXCM_LEVEL;
#ورنہ
    int_level = RVHAL_EXCM_LEVEL_CLIC;
#اگر_ختم

    پچھلا_int_level = rv_utils_سیٹ_intlevel_regval(int_level);
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
    واپس پچھلا_int_level;
}

خالی v_پورٹ_Clear_مداخلت_نقاب(UBaseType_t نقاب)
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
    asm متلون ( "nop" );
    asm متلون ( "nop" );
    asm متلون ( "nop" );
}

بنیاد_Type_t x_پورٹ_Check_If_اندر_ISR(خالی)
{
    واپس ux_مداخلت_Nesting;
}

خالی v_پورٹ_Assert_If_اندر_ISR(خالی)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(x_پورٹ_Check_If_اندر_ISR() == 0);
}

// ------------------ Critical Sections --------------------

#اگر ( config_نمبر_کا_CORES > 1 )
خالی v_پورٹ_Take_تالا( پورٹ_MUX_TYPE *تالا_2 )
{
    spinlock_acquire( تالا_2, پورٹ_MUX_NO_TIMEOUT);
}

خالی v_پورٹ_Release_تالا( پورٹ_MUX_TYPE *تالا_2 )
{
    spinlock_release( تالا_2 );
}
#اگر_ختم /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

خالی v_پورٹ_Yield(خالی)
{
    // TODO: IDF-8113
    ثابت صحیح core_id = 0;

    اگر (ux_مداخلت_Nesting) {
        v_پورٹ_Yield_From_ISR();
    } ورنہ {

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
        جب (uxSchedulerRunning && crosscore_int_ll_get_state(core_id) != 0) {}
    }
}

خالی v_پورٹ_Yield_From_ISR( خالی )
{
    //traceISR_EXIT_TO_SCHEDULER();
    uxSchedulerRunning = 1;
    x_پورٹ_Switch_Flag = 1;
}
// ----------------------- System --------------------------

// ------------------- Run Time Stats ----------------------

// --------------------- TCB Cleanup -----------------------

#اگر ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
ساکن خالی v_پورٹ_TLSPointers_Del_Cb( خالی *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pv_Thread_Local_Storage_اشارے_Del_Callback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_عدد_THREAD_LOCAL_STORAGE_اشارے / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    برائے ( صحیح x = 0; x < ( config_عدد_THREAD_LOCAL_STORAGE_اشارے / 2 ); x++ ) {
        اگر ( pv_Thread_Local_Storage_اشارے_Del_Callback[ x ] != تہی ) {  //If del cb is set
            /* In case the TLSP deletion callback has been overwritten by a TLS pointer, gracefully abort. */
            اگر ( !esp_ptr_executable( pv_Thread_Local_Storage_اشارے_Del_Callback[ x ] ) ) {
                ESP_EARLY_LOGE("FreeRTOS", "Fatal error: TLSP deletion callback at index %d overwritten with non-excutable pointer %p", x, pv_Thread_Local_Storage_اشارے_Del_Callback[ x ]);
                abort();
            }

            pv_Thread_Local_Storage_اشارے_Del_Callback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#اگر_ختم /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

خالی v_پورٹ_TCBPre_Delete_Hook( خالی *pxTCB )
{
    #اگر ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        خارجی خالی vTaskPreDeletionHook( خالی * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #اگر_ختم /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

    #اگر ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
        /* Call TLS pointers deletion callbacks */
        v_پورٹ_TLSPointers_Del_Cb( pxTCB );
    #اگر_ختم /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
}

/* ------------------------------------------------ FreeRTOS Portable --------------------------------------------------
 * - Provides implementation for functions required by FreeRTOS
 * - Declared in portable.h
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------- Scheduler Start/End -------------------

بنیاد_Type_t x_پورٹ_آغاز_Scheduler(خالی)
{
    ux_مداخلت_Nesting = 0;
    پورٹ_ux_Critical_Nesting_IDF = 0;
#اگر CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
    پورٹ_x_Thread_محفوظ_Claimed = false;
#اگر_ختم
    uxSchedulerRunning = 0;
#اگر config_عدد_CORES > 1
    پورٹ_ux_Core_Startup_مکمل[x_پورٹ_Get_Core_ID()] = 0;
#اگر_ختم
    /* Setup the hardware to generate the tick. */
    v_پورٹ_تیاری_Timer();

    rv_utils_intr_global_enable();
    esprv_int_سیٹ_حد(RVHAL_INTR_ENABLE_THRESH); /* set global interrupt masking level */

    v_پورٹ_Yield();

    /*Should not get here*/
    واپس pdFALSE;
}

خالی v_پورٹ_انتہا_Scheduler(خالی)
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
    خارجی حرف thread_local_ڈیٹا_آغاز, thread_local_ڈیٹا_انتہا;
    خارجی حرف thread_local_bss_آغاز, thread_local_bss_انتہا;
    ثابت uint32_t tls_ڈیٹا_سائز = (uint32_t)&thread_local_ڈیٹا_انتہا - (uint32_t)&thread_local_ڈیٹا_آغاز;
    ثابت uint32_t tls_bss_سائز = (uint32_t)&thread_local_bss_انتہا - (uint32_t)&thread_local_bss_آغاز;
    ثابت uint32_t tls_area_سائز = ESP_ALIGN_اوپر(tls_ڈیٹا_سائز + tls_bss_سائز, 16);
    // TODO: check that TLS area fits the stack

    // Allocate space for the TLS area on the stack. The area must be aligned to 16-bytes
    ux_ڈھیر_اشارہ_گر = ESP_ALIGN_نیچے_کو(ux_ڈھیر_اشارہ_گر - (UBaseType_t)tls_area_سائز, 16);
    // Initialize the TLS data with the initialization values of each TLS variable
    حافظہ_نقل((خالی *)ux_ڈھیر_اشارہ_گر, &thread_local_ڈیٹا_آغاز, tls_ڈیٹا_سائز);
    // Initialize the TLS bss with zeroes
    memset((خالی *)(ux_ڈھیر_اشارہ_گر + tls_ڈیٹا_سائز), 0, tls_bss_سائز);

    // Save tls start address
    *ret_threadptr_reg_init = (uint32_t)ux_ڈھیر_اشارہ_گر;
    واپس ux_ڈھیر_اشارہ_گر;
}

#اگر CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
ساکن خالی v_پورٹ_Task_Wrapper(TaskFunction_t px_رمز, خالی *pvParameters)
{
    __asm__ متلون(".cfi_undefined ra");  // tell to debugger that it's outermost (initial) frame
    خارجی خالی __attribute__((noreturn)) panic_abort(ثابت حرف *details);
    ساکن حرف DRAM_ATTR پیغام[80] = "FreeRTOS: FreeRTOS Task \"\0";
    px_رمز(pvParameters);
    /* FreeRTOS tasks should not return. Log the task name and abort. */
    /* We cannot use s(n)printf because it is in flash */
    strcat(پیغام, pcTaskGetName(تہی));
    strcat(پیغام, "\" should not return, Aborting now!");
    panic_abort(پیغام);
}
#اگر_ختم // CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER

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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ڈھیر_چوکھٹا(UBaseType_t ux_ڈھیر_اشارہ_گر, TaskFunction_t px_رمز, خالی *pvParameters, uint32_t threadptr_reg_init)
{
    /*
    Allocate space for the task's starting interrupt stack frame.
    - The stack frame must be allocated to a 16-byte aligned address.
    - We use RV_STK_FRMSZ (instead of sizeof(RvExcFrame)) as it rounds up the total size to a multiple of 16.
    */
    ux_ڈھیر_اشارہ_گر = ESP_ALIGN_نیچے_کو(ux_ڈھیر_اشارہ_گر - RV_STK_FRMSZ, 16);

    // Clear the entire interrupt stack frame
    Rv_Exc_چوکھٹا *چوکھٹا = (Rv_Exc_چوکھٹا *)ux_ڈھیر_اشارہ_گر;
    memset(چوکھٹا, 0, حجم(Rv_Exc_چوکھٹا));

    /* Initialize the stack frame. */
    خارجی uint32_t __global_pointer$;
    #اگر CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
        چوکھٹا->mepc = (UBaseType_t)v_پورٹ_Task_Wrapper;
        چوکھٹا->a0 = (UBaseType_t)px_رمز;
        چوکھٹا->a1 = (UBaseType_t)pvParameters;
    #ورنہ
        چوکھٹا->mepc = (UBaseType_t)px_رمز;
        چوکھٹا->a0 = (UBaseType_t)pvParameters;
    #اگر_ختم // CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
    چوکھٹا->gp = (UBaseType_t)&__global_pointer$;
    چوکھٹا->tp = (UBaseType_t)threadptr_reg_init;

    واپس ux_ڈھیر_اشارہ_گر;
}

ڈھیر_Type_t *px_پورٹ_Initialise_ڈھیر(ڈھیر_Type_t *px_چوٹی_کا_ڈھیر, TaskFunction_t px_رمز, خالی *pvParameters)
{
#اگر_تعریف_شدہ __clang_analyzer__
    // Teach clang-tidy that pxTopOfStack cannot be a pointer to const
    متلون ڈھیر_Type_t * px_عارضی = px_چوٹی_کا_ڈھیر;
    px_چوٹی_کا_ڈھیر = px_عارضی;
#اگر_ختم /*__clang_analyzer__ */
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

    UBaseType_t ux_ڈھیر_اشارہ_گر = (UBaseType_t)px_چوٹی_کا_ڈھیر;
    configASSERT((ux_ڈھیر_اشارہ_گر & پورٹ_BYTE_ALIGNMENT_نقاب) == 0);

    // Initialize GCC TLS area
    uint32_t threadptr_reg_init;
    ux_ڈھیر_اشارہ_گر = ux_Initialise_ڈھیر_TLS(ux_ڈھیر_اشارہ_گر, &threadptr_reg_init);
    configASSERT((ux_ڈھیر_اشارہ_گر & پورٹ_BYTE_ALIGNMENT_نقاب) == 0);

    // Initialize the starting interrupt stack frame
    ux_ڈھیر_اشارہ_گر = ux_Initialise_ڈھیر_چوکھٹا(ux_ڈھیر_اشارہ_گر, px_رمز, pvParameters, threadptr_reg_init);
    configASSERT((ux_ڈھیر_اشارہ_گر & پورٹ_BYTE_ALIGNMENT_نقاب) == 0);

    // Return the task's current stack pointer address which should point to the starting interrupt stack frame
    واپس (ڈھیر_Type_t *)ux_ڈھیر_اشارہ_گر;
}

// ------------------- Hook Functions ----------------------

خالی __attribute__((weak)) v_Application_ڈھیر_حد_سے_تجاوز_Hook(TaskHandle_t xTask, حرف *pcTaskName)
{
#تعریف_کرو ERR_STR1 "***ERROR*** A stack overflow in task "
#تعریف_کرو ERR_STR2 " has been detected."
    ثابت حرف *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    حرف buf[حجم(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + حجم(ERR_STR2) + 1 /* null char */] = {0};

    حرف *dest = buf;
    برائے (صحیح i = 0; i < حجم(str) / حجم(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
    esp_system_abort(buf);
}

#اگر  (  configUSE_TICK_HOOK > 0 )
خالی vApplicationTickHook( خالی )
{
    esp_vApplicationTickHook();
}
#اگر_ختم

خارجی خالی esp_vApplicationIdleHook(خالی);
#اگر CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK
/*
By default, the port uses vApplicationPassiveIdleHook() to run IDF style idle
hooks. However, users may also want to provide their own vApplicationPassiveIdleHook().
In this case, we use to -Wl,--wrap option to wrap the user provided vApplicationPassiveIdleHook()
*/
خارجی خالی __real_vApplicationPassiveIdleHook( خالی );
خالی __wrap_vApplicationPassiveIdleHook( خالی )
{
    esp_vApplicationIdleHook(); //Run IDF style hooks
    __real_vApplicationPassiveIdleHook(); //Call the user provided vApplicationPassiveIdleHook()
}
#ورنہ // CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK
خالی vApplicationPassiveIdleHook( خالی )
{
    esp_vApplicationIdleHook(); //Run IDF style hooks
}
#اگر_ختم // CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK

/* ------------------------------------------------ Run Time Stats ------------------------------------------------- */

#اگر ( CONFIG_FREERTOS_GENERATE_تسلسل_وقت_STATS )

config_تسلسل_وقت_شمار_TYPE x_پورٹ_Get_تسلسل_وقت_شمار_قدر( خالی )
{
#اگر_تعریف_شدہ CONFIG_FREERTOS_تسلسل_وقت_STATS_USING_ESP_TIMER
    واپس (config_تسلسل_وقت_شمار_TYPE) esp_timer_get_وقت();
#ورنہ
    واپس 0;
#اگر_ختم // CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
}

#اگر_ختم /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
