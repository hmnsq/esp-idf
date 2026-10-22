/*
 * SPDX-FileCopyrightText: 2022-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#অন্তর্ভাব "sdkconfig.h"
#অন্তর্ভাব <stdbool.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব "esp_compiler.h"
#অন্তর্ভাব "soc/soc_caps.h"
#অন্তর্ভাব "soc/periph_defs.h"
#অন্তর্ভাব "soc/system_reg.h"
#অন্তর্ভাব "hal/crosscore_int_ll.h"
#অন্তর্ভাব "hal/systimer_hal.h"
#অন্তর্ভাব "hal/systimer_ll.h"
#অন্তর্ভাব "riscv/rvruntime-frames.h"
#অন্তর্ভাব "riscv/rv_utils.h"
#অন্তর্ভাব "riscv/interrupt.h"
#অন্তর্ভাব "esp_private/crosscore_int.h"
#অন্তর্ভাব "esp_private/esp_int_wdt.h"
#অন্তর্ভাব "esp_private/periph_ctrl.h"
#অন্তর্ভাব "esp_private/systimer.h"
#অন্তর্ভাব "esp_attr.h"
#অন্তর্ভাব "esp_system.h"
#অন্তর্ভাব "esp_heap_caps_init.h"
#অন্তর্ভাব "esp_task_wdt.h"
#অন্তর্ভাব "esp_task.h"
#অন্তর্ভাব "esp_intr_alloc.h"
#অন্তর্ভাব "esp_log.h"
#অন্তর্ভাব "FreeRTOS.h"       /* This pulls in portmacro.h */
#অন্তর্ভাব "task.h"
#অন্তর্ভাব "port_systick.h"
#অন্তর্ভাব "portmacro.h"
#অন্তর্ভাব "esp_memory_utils.h"
#অন্তর্ভাব "esp_macros.h"
#যদি CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
#অন্তর্ভাব "esp_timer.h"
#endif
#ifdef CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER
#অন্তর্ভাব "soc/periph_defs.h"
#অন্তর্ভাব "soc/system_reg.h"
#অন্তর্ভাব "hal/systimer_hal.h"
#অন্তর্ভাব "hal/systimer_ll.h"
#endif
#ifdef CONFIG_PM_TRACE
#অন্তর্ভাব "esp_private/pm_trace.h"
#endif //CONFIG_PM_TRACE

_Static_assert(portBYTE_ALIGNMENT == 16, "portBYTE_ALIGNMENT must be set to 16");
#যদি CONFIG_ESP_SYSTEM_HW_স্তূপ_GUARD
/**
 * offsetof() can not be used in asm code. Then we need make sure that
 * PORT_OFFSET_PX_STACK and PORT_OFFSET_PX_END_OF_STACK have expected values.
 * Macro used in the portasm.S instead of variables to save at least 4 instruction calls
 * which accessing DRAM memory. This optimization saves CPU time in the interrupt handling.
 */

_Static_assert(offsetof( StaticTask_t, pxDummy6 ) == PORT_OFFSET_PX_স্তূপ);
_Static_assert(offsetof( StaticTask_t, pxDummy8 ) == PORT_OFFSET_PX_END_OF_স্তূপ);
#endif // CONFIG_ESP_SYSTEM_HW_STACK_GUARD

/* ---------------------------------------------------- Variables ------------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

BaseType_t uxSchedulerRunning = 0;  // Duplicate of xSchedulerRunning, accessible to port files
অস্থির UBaseType_t uxInterruptNesting = 0;
#যদি config_অঙ্ক_CORES > 1
অস্থির unsigned port_uxCoreStartupDone[port_অঙ্ক_PROCESSORS] = {0};  // Indicates whether the core has completed its startup sequence
#endif
portMUX_TYPE port_xTaskLock = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE port_xISRLock = portMUX_INITIALIZER_UNLOCKED;
অস্থির BaseType_t xPortSwitchFlag = 0;
__attribute__((aligned(16))) স্তূপ_Type_t x_Isr_স্তূপ[config_ISR_স্তূপ_আকার];
স্তূপ_Type_t *x_Isr_স্তূপ_শীর্ষ = &x_Isr_স্তূপ[0] + (config_ISR_স্তূপ_আকার & (~((port_POINTER_আকার_TYPE)portBYTE_ALIGNMENT_MASK)));

// Variables used for IDF style critical sections. These are orthogonal to FreeRTOS critical sections
স্থির UBaseType_t port_uxCriticalNestingIDF = 0;
স্থির UBaseType_t port_uxCriticalOldInterruptStateIDF = 0;
#যদি CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
অস্থির bool port_xThreadSafeClaimed = false;
#endif

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// ------------------ Critical Sections --------------------

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

শূন্য vPortEnterCritical(শূন্য)
{
#যদি CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
    যদি (unlikely(port_xThreadSafeClaimed)) {
        ফেরত;
    }
#endif
    // Save current interrupt threshold and disable interrupts
    UBaseType_t old_thresh = ulPortSetInterruptMask();
    // Update the IDF critical nesting count
    port_uxCriticalNestingIDF++;
    যদি (port_uxCriticalNestingIDF == 1) {
        // Save a copy of the old interrupt threshold
        port_uxCriticalOldInterruptStateIDF = (UBaseType_t) old_thresh;
    }
}

শূন্য vPortExitCritical(শূন্য)
{
#যদি CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
    যদি (unlikely(port_xThreadSafeClaimed)) {
        ফেরত;
    }
#endif

    /* Critical section nesting coung must never be negative */
    configASSERT( port_uxCriticalNestingIDF > 0 );

    যদি (port_uxCriticalNestingIDF > 0) {
        port_uxCriticalNestingIDF--;

        যদি (port_uxCriticalNestingIDF == 0) {
            // Restore the saved interrupt threshold
            vPortClearInterruptMask((পূর্ণসংখ্যা)port_uxCriticalOldInterruptStateIDF);
        }
    }
}

// ----------------------- System --------------------------

#define স্তূপ_WATCH_AREA_আকার 32
#define স্তূপ_WATCH_POINT_সংখ্যা (SOC_CPU_WATCHPOINTS_অঙ্ক - 1)

শূন্য v_Port_Set_স্তূপ_Watchpoint(শূন্য *px_স্তূপ_Start)
{
    uint32_t addr = (uint32_t)px_স্তূপ_Start;
    addr = (addr + (স্তূপ_WATCH_AREA_আকার - 1)) & (~(স্তূপ_WATCH_AREA_আকার - 1));
    esp_cpu_set_watchpoint(স্তূপ_WATCH_POINT_সংখ্যা, (অক্ষর *)addr, স্তূপ_WATCH_AREA_আকার, ESP_CPU_WATCHPOINT_STORE);
}



/* ---------------------------------------------- Port Implementations -------------------------------------------------
 * Implementations of Porting Interface functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

UBaseType_t ulPortSetInterruptMask(শূন্য)
{
    UBaseType_t পূর্ব_int_level = 0, int_level = 0;
#যদি !SOC_INT_CLIC_SUPPORTED
    int_level = RVHAL_EXCM_LEVEL;
#অন্যথা
    int_level = RVHAL_EXCM_LEVEL_CLIC;
#endif

    পূর্ব_int_level = rv_utils_set_intlevel_regval(int_level);
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
    ফেরত পূর্ব_int_level;
}

শূন্য vPortClearInterruptMask(UBaseType_t mask)
{
    rv_utils_restore_intlevel_regval(mask);
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
    asm অস্থির ( "nop" );
    asm অস্থির ( "nop" );
    asm অস্থির ( "nop" );
}

BaseType_t xPortCheckIfInISR(শূন্য)
{
    ফেরত uxInterruptNesting;
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

শূন্য vPortYield(শূন্য)
{
    // TODO: IDF-8113
    ধ্রুবক পূর্ণসংখ্যা core_id = 0;

    যদি (uxInterruptNesting) {
        vPortYieldFromISR();
    } অন্যথা {

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
        যখন (uxSchedulerRunning && crosscore_int_ll_get_state(core_id) != 0) {}
    }
}

শূন্য vPortYieldFromISR( শূন্য )
{
    //traceISR_EXIT_TO_SCHEDULER();
    uxSchedulerRunning = 1;
    xPortSwitchFlag = 1;
}
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
                ESP_EARLY_LOGE("FreeRTOS", "Fatal error: TLSP deletion callback at index %d overwritten with non-excutable pointer %p", x, pvThreadLocalStoragePointersDelCallback[ x ]);
                abort();
            }

            pvThreadLocalStoragePointersDelCallback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#endif /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

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
}

/* ------------------------------------------------ FreeRTOS Portable --------------------------------------------------
 * - Provides implementation for functions required by FreeRTOS
 * - Declared in portable.h
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------- Scheduler Start/End -------------------

BaseType_t xPortStartScheduler(শূন্য)
{
    uxInterruptNesting = 0;
    port_uxCriticalNestingIDF = 0;
#যদি CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
    port_xThreadSafeClaimed = false;
#endif
    uxSchedulerRunning = 0;
#যদি config_অঙ্ক_CORES > 1
    port_uxCoreStartupDone[xPortGetCoreID()] = 0;
#endif
    /* Setup the hardware to generate the tick. */
    vPortSetupTimer();

    rv_utils_intr_global_enable();
    esprv_int_set_threshold(RVHAL_INTR_ENABLE_THRESH); /* set global interrupt masking level */

    vPortYield();

    /*Should not get here*/
    ফেরত pdFALSE;
}

শূন্য vPortEndScheduler(শূন্য)
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_স্তূপ_TLS(UBaseType_t ux_স্তূপ_Pointer, uint32_t *ret_threadptr_reg_init)
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
    বাহ্যিক অক্ষর thread_local_তথ্য_start, thread_local_তথ্য_end;
    বাহ্যিক অক্ষর _thread_local_bss_start, _thread_local_bss_end;
    ধ্রুবক uint32_t tls_তথ্য_আকার = (uint32_t)&thread_local_তথ্য_end - (uint32_t)&thread_local_তথ্য_start;
    ধ্রুবক uint32_t tls_bss_আকার = (uint32_t)&_thread_local_bss_end - (uint32_t)&_thread_local_bss_start;
    ধ্রুবক uint32_t tls_area_আকার = ESP_ALIGN_UP(tls_তথ্য_আকার + tls_bss_আকার, 16);
    // TODO: check that TLS area fits the stack

    // Allocate space for the TLS area on the stack. The area must be aligned to 16-bytes
    ux_স্তূপ_Pointer = ESP_ALIGN_DOWN(ux_স্তূপ_Pointer - (UBaseType_t)tls_area_আকার, 16);
    // Initialize the TLS data with the initialization values of each TLS variable
    memcpy((শূন্য *)ux_স্তূপ_Pointer, &thread_local_তথ্য_start, tls_তথ্য_আকার);
    // Initialize the TLS bss with zeroes
    memset((শূন্য *)(ux_স্তূপ_Pointer + tls_তথ্য_আকার), 0, tls_bss_আকার);

    // Save tls start address
    *ret_threadptr_reg_init = (uint32_t)ux_স্তূপ_Pointer;
    ফেরত ux_স্তূপ_Pointer;
}

#যদি CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
স্থির শূন্য vPortTaskWrapper(TaskFunction_t pxCode, শূন্য *pvParameters)
{
    __asm__ অস্থির(".cfi_undefined ra");  // tell to debugger that it's outermost (initial) frame
    বাহ্যিক শূন্য __attribute__((noreturn)) panic_abort(ধ্রুবক অক্ষর *details);
    স্থির অক্ষর DRAM_ATTR বার্তা[80] = "FreeRTOS: FreeRTOS Task \"\0";
    pxCode(pvParameters);
    /* FreeRTOS tasks should not return. Log the task name and abort. */
    /* We cannot use s(n)printf because it is in flash */
    strcat(বার্তা, pcTaskGetName(ফাঁকা));
    strcat(বার্তা, "\" should not return, Aborting now!");
    panic_abort(বার্তা);
}
#endif // CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER

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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_স্তূপ_Frame(UBaseType_t ux_স্তূপ_Pointer, TaskFunction_t pxCode, শূন্য *pvParameters, uint32_t threadptr_reg_init)
{
    /*
    Allocate space for the task's starting interrupt stack frame.
    - The stack frame must be allocated to a 16-byte aligned address.
    - We use RV_STK_FRMSZ (instead of sizeof(RvExcFrame)) as it rounds up the total size to a multiple of 16.
    */
    ux_স্তূপ_Pointer = ESP_ALIGN_DOWN(ux_স্তূপ_Pointer - RV_STK_FRMSZ, 16);

    // Clear the entire interrupt stack frame
    RvExcFrame *frame = (RvExcFrame *)ux_স্তূপ_Pointer;
    memset(frame, 0, আকার(RvExcFrame));

    /* Initialize the stack frame. */
    বাহ্যিক uint32_t __global_pointer$;
    #যদি CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
        frame->mepc = (UBaseType_t)vPortTaskWrapper;
        frame->a0 = (UBaseType_t)pxCode;
        frame->a1 = (UBaseType_t)pvParameters;
    #অন্যথা
        frame->mepc = (UBaseType_t)pxCode;
        frame->a0 = (UBaseType_t)pvParameters;
    #endif // CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
    frame->gp = (UBaseType_t)&__global_pointer$;
    frame->tp = (UBaseType_t)threadptr_reg_init;

    ফেরত ux_স্তূপ_Pointer;
}

স্তূপ_Type_t *px_Port_Initialise_স্তূপ(স্তূপ_Type_t *px_শীর্ষ_Of_স্তূপ, TaskFunction_t pxCode, শূন্য *pvParameters)
{
#ifdef __clang_analyzer__
    // Teach clang-tidy that pxTopOfStack cannot be a pointer to const
    অস্থির স্তূপ_Type_t * px_অস্থায়ী = px_শীর্ষ_Of_স্তূপ;
    px_শীর্ষ_Of_স্তূপ = px_অস্থায়ী;
#endif /*__clang_analyzer__ */
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

    UBaseType_t ux_স্তূপ_Pointer = (UBaseType_t)px_শীর্ষ_Of_স্তূপ;
    configASSERT((ux_স্তূপ_Pointer & portBYTE_ALIGNMENT_MASK) == 0);

    // Initialize GCC TLS area
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

শূন্য __attribute__((weak)) v_Application_স্তূপ_Overflow_Hook(TaskHandle_t xTask, অক্ষর *pcTaskName)
{
#define ERR_STR1 "***ERROR*** A stack overflow in task "
#define ERR_STR2 " has been detected."
    ধ্রুবক অক্ষর *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    অক্ষর buf[আকার(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + আকার(ERR_STR2) + 1 /* null char */] = {0};

    অক্ষর *dest = buf;
    জন্য (পূর্ণসংখ্যা i = 0; i < আকার(str) / আকার(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
    esp_system_abort(buf);
}

#যদি  (  configUSE_TICK_HOOK > 0 )
শূন্য vApplicationTickHook( শূন্য )
{
    esp_vApplicationTickHook();
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
#অন্যথা
    ফেরত 0;
#endif // CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
}

#endif /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
