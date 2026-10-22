/*
 * SPDX-FileCopyrightText: 2022-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ଅନ୍ତର୍ଭାଵ "sdkconfig.h"
#ଅନ୍ତର୍ଭାଵ <stdbool.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ "esp_compiler.h"
#ଅନ୍ତର୍ଭାଵ "soc/soc_caps.h"
#ଅନ୍ତର୍ଭାଵ "soc/periph_defs.h"
#ଅନ୍ତର୍ଭାଵ "soc/system_reg.h"
#ଅନ୍ତର୍ଭାଵ "hal/crosscore_int_ll.h"
#ଅନ୍ତର୍ଭାଵ "hal/systimer_hal.h"
#ଅନ୍ତର୍ଭାଵ "hal/systimer_ll.h"
#ଅନ୍ତର୍ଭାଵ "riscv/rvruntime-frames.h"
#ଅନ୍ତର୍ଭାଵ "riscv/rv_utils.h"
#ଅନ୍ତର୍ଭାଵ "riscv/interrupt.h"
#ଅନ୍ତର୍ଭାଵ "esp_private/crosscore_int.h"
#ଅନ୍ତର୍ଭାଵ "esp_private/esp_int_wdt.h"
#ଅନ୍ତର୍ଭାଵ "esp_private/periph_ctrl.h"
#ଅନ୍ତର୍ଭାଵ "esp_private/systimer.h"
#ଅନ୍ତର୍ଭାଵ "esp_attr.h"
#ଅନ୍ତର୍ଭାଵ "esp_system.h"
#ଅନ୍ତର୍ଭାଵ "esp_heap_caps_init.h"
#ଅନ୍ତର୍ଭାଵ "esp_task_wdt.h"
#ଅନ୍ତର୍ଭାଵ "esp_task.h"
#ଅନ୍ତର୍ଭାଵ "esp_intr_alloc.h"
#ଅନ୍ତର୍ଭାଵ "esp_log.h"
#ଅନ୍ତର୍ଭାଵ "FreeRTOS.h"       /* This pulls in portmacro.h */
#ଅନ୍ତର୍ଭାଵ "task.h"
#ଅନ୍ତର୍ଭାଵ "port_systick.h"
#ଅନ୍ତର୍ଭାଵ "portmacro.h"
#ଅନ୍ତର୍ଭାଵ "esp_memory_utils.h"
#ଅନ୍ତର୍ଭାଵ "esp_macros.h"
#ଯଦି CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
#ଅନ୍ତର୍ଭାଵ "esp_timer.h"
#endif
#ifdef CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER
#ଅନ୍ତର୍ଭାଵ "soc/periph_defs.h"
#ଅନ୍ତର୍ଭାଵ "soc/system_reg.h"
#ଅନ୍ତର୍ଭାଵ "hal/systimer_hal.h"
#ଅନ୍ତର୍ଭାଵ "hal/systimer_ll.h"
#endif
#ifdef CONFIG_PM_TRACE
#ଅନ୍ତର୍ଭାଵ "esp_private/pm_trace.h"
#endif //CONFIG_PM_TRACE

_Static_assert(portBYTE_ALIGNMENT == 16, "portBYTE_ALIGNMENT must be set to 16");
#ଯଦି CONFIG_ESP_SYSTEM_HW_ଗଦା_GUARD
/**
 * offsetof() can not be used in asm code. Then we need make sure that
 * PORT_OFFSET_PX_STACK and PORT_OFFSET_PX_END_OF_STACK have expected values.
 * Macro used in the portasm.S instead of variables to save at least 4 instruction calls
 * which accessing DRAM memory. This optimization saves CPU time in the interrupt handling.
 */

_Static_assert(offsetof( StaticTask_t, pxDummy6 ) == PORT_OFFSET_PX_ଗଦା);
_Static_assert(offsetof( StaticTask_t, pxDummy8 ) == PORT_OFFSET_PX_END_OF_ଗଦା);
#endif // CONFIG_ESP_SYSTEM_HW_STACK_GUARD

/* ---------------------------------------------------- Variables ------------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

BaseType_t uxSchedulerRunning = 0;  // Duplicate of xSchedulerRunning, accessible to port files
ଅସ୍ଥିର UBaseType_t uxInterruptNesting = 0;
#ଯଦି config_ଅଙ୍କ_CORES > 1
ଅସ୍ଥିର unsigned port_uxCoreStartupDone[port_ଅଙ୍କ_PROCESSORS] = {0};  // Indicates whether the core has completed its startup sequence
#endif
portMUX_TYPE port_xTaskLock = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE port_xISRLock = portMUX_INITIALIZER_UNLOCKED;
ଅସ୍ଥିର BaseType_t xPortSwitchFlag = 0;
__attribute__((aligned(16))) ଗଦା_Type_t x_Isr_ଗଦା[config_ISR_ଗଦା_SIZE];
ଗଦା_Type_t *x_Isr_ଗଦା_ଶୀର୍ଷ = &x_Isr_ଗଦା[0] + (config_ISR_ଗଦା_SIZE & (~((portPOINTER_SIZE_TYPE)portBYTE_ALIGNMENT_MASK)));

// Variables used for IDF style critical sections. These are orthogonal to FreeRTOS critical sections
ସ୍ଥିର UBaseType_t port_uxCriticalNestingIDF = 0;
ସ୍ଥିର UBaseType_t port_uxCriticalOldInterruptStateIDF = 0;
#ଯଦି CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
ଅସ୍ଥିର bool port_xThreadSafeClaimed = false;
#endif

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// ------------------ Critical Sections --------------------

#ଯଦି CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
ଶୂନ୍ୟ xPortThreadSafeClaim(ଶୂନ୍ୟ)
{
    configASSERT(!xPortCanYield());
    configASSERT(!port_xThreadSafeClaimed);
    port_xThreadSafeClaimed = true;
}

ଶୂନ୍ୟ xPortThreadSafeDisclaim(ଶୂନ୍ୟ)
{
    configASSERT(!xPortCanYield());
    configASSERT(port_xThreadSafeClaimed);
    port_xThreadSafeClaimed = false;
}
#endif /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

ଶୂନ୍ୟ vPortEnterCritical(ଶୂନ୍ୟ)
{
#ଯଦି CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
    ଯଦି (unlikely(port_xThreadSafeClaimed)) {
        ଫେରନ୍ତୁ;
    }
#endif
    // Save current interrupt threshold and disable interrupts
    UBaseType_t old_thresh = ulPortSetInterruptMask();
    // Update the IDF critical nesting count
    port_uxCriticalNestingIDF++;
    ଯଦି (port_uxCriticalNestingIDF == 1) {
        // Save a copy of the old interrupt threshold
        port_uxCriticalOldInterruptStateIDF = (UBaseType_t) old_thresh;
    }
}

ଶୂନ୍ୟ vPortExitCritical(ଶୂନ୍ୟ)
{
#ଯଦି CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
    ଯଦି (unlikely(port_xThreadSafeClaimed)) {
        ଫେରନ୍ତୁ;
    }
#endif

    /* Critical section nesting coung must never be negative */
    configASSERT( port_uxCriticalNestingIDF > 0 );

    ଯଦି (port_uxCriticalNestingIDF > 0) {
        port_uxCriticalNestingIDF--;

        ଯଦି (port_uxCriticalNestingIDF == 0) {
            // Restore the saved interrupt threshold
            vPortClearInterruptMask((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)port_uxCriticalOldInterruptStateIDF);
        }
    }
}

// ----------------------- System --------------------------

#define ଗଦା_WATCH_AREA_SIZE 32
#define ଗଦା_WATCH_POINT_ସଂଖ୍ୟା (SOC_CPU_WATCHPOINTS_ଅଙ୍କ - 1)

ଶୂନ୍ୟ v_Port_Set_ଗଦା_Watchpoint(ଶୂନ୍ୟ *px_ଗଦା_Start)
{
    uint32_t addr = (uint32_t)px_ଗଦା_Start;
    addr = (addr + (ଗଦା_WATCH_AREA_SIZE - 1)) & (~(ଗଦା_WATCH_AREA_SIZE - 1));
    esp_cpu_set_watchpoint(ଗଦା_WATCH_POINT_ସଂଖ୍ୟା, (ଅକ୍ଷର *)addr, ଗଦା_WATCH_AREA_SIZE, ESP_CPU_WATCHPOINT_STORE);
}



/* ---------------------------------------------- Port Implementations -------------------------------------------------
 * Implementations of Porting Interface functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

UBaseType_t ulPortSetInterruptMask(ଶୂନ୍ୟ)
{
    UBaseType_t prev_int_level = 0, int_level = 0;
#ଯଦି !SOC_INT_CLIC_SUPPORTED
    int_level = RVHAL_EXCM_LEVEL;
#ଅନ୍ୟଥା
    int_level = RVHAL_EXCM_LEVEL_CLIC;
#endif

    prev_int_level = rv_utils_set_intlevel_regval(int_level);
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
    ଫେରନ୍ତୁ prev_int_level;
}

ଶୂନ୍ୟ vPortClearInterruptMask(UBaseType_t mask)
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
    asm ଅସ୍ଥିର ( "nop" );
    asm ଅସ୍ଥିର ( "nop" );
    asm ଅସ୍ଥିର ( "nop" );
}

BaseType_t xPortCheckIfInISR(ଶୂନ୍ୟ)
{
    ଫେରନ୍ତୁ uxInterruptNesting;
}

ଶୂନ୍ୟ vPortAssertIfInISR(ଶୂନ୍ୟ)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(xPortCheckIfInISR() == 0);
}

// ------------------ Critical Sections --------------------

#ଯଦି ( config_ସଂଖ୍ୟା_OF_CORES > 1 )
ଶୂନ୍ୟ vPortTakeLock( portMUX_TYPE *lock )
{
    spinlock_acquire( lock, portMUX_NO_TIMEOUT);
}

ଶୂନ୍ୟ vPortReleaseLock( portMUX_TYPE *lock )
{
    spinlock_release( lock );
}
#endif /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

ଶୂନ୍ୟ vPortYield(ଶୂନ୍ୟ)
{
    // TODO: IDF-8113
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା core_id = 0;

    ଯଦି (uxInterruptNesting) {
        vPortYieldFromISR();
    } ଅନ୍ୟଥା {

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
        ଯେତେବେଳେ (uxSchedulerRunning && crosscore_int_ll_get_state(core_id) != 0) {}
    }
}

ଶୂନ୍ୟ vPortYieldFromISR( ଶୂନ୍ୟ )
{
    //traceISR_EXIT_TO_SCHEDULER();
    uxSchedulerRunning = 1;
    xPortSwitchFlag = 1;
}
// ----------------------- System --------------------------

// ------------------- Run Time Stats ----------------------

// --------------------- TCB Cleanup -----------------------

#ଯଦି ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
ସ୍ଥିର ଶୂନ୍ୟ vPortTLSPointersDelCb( ଶୂନ୍ୟ *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pvThreadLocalStoragePointersDelCallback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_ଅଙ୍କ_THREAD_LOCAL_STORAGE_POINTERS / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    ପାଇଁ ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0; x < ( config_ଅଙ୍କ_THREAD_LOCAL_STORAGE_POINTERS / 2 ); x++ ) {
        ଯଦି ( pvThreadLocalStoragePointersDelCallback[ x ] != ଖାଲି ) {  //If del cb is set
            /* In case the TLSP deletion callback has been overwritten by a TLS pointer, gracefully abort. */
            ଯଦି ( !esp_ptr_executable( pvThreadLocalStoragePointersDelCallback[ x ] ) ) {
                ESP_EARLY_LOGE("FreeRTOS", "Fatal error: TLSP deletion callback at index %d overwritten with non-excutable pointer %p", x, pvThreadLocalStoragePointersDelCallback[ x ]);
                abort();
            }

            pvThreadLocalStoragePointersDelCallback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#endif /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

ଶୂନ୍ୟ vPortTCBPreDeleteHook( ଶୂନ୍ୟ *pxTCB )
{
    #ଯଦି ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        ବାହ୍ୟ ଶୂନ୍ୟ vTaskPreDeletionHook( ଶୂନ୍ୟ * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #endif /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

    #ଯଦି ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
        /* Call TLS pointers deletion callbacks */
        vPortTLSPointersDelCb( pxTCB );
    #endif /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
}

/* ------------------------------------------------ FreeRTOS Portable --------------------------------------------------
 * - Provides implementation for functions required by FreeRTOS
 * - Declared in portable.h
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------- Scheduler Start/End -------------------

BaseType_t xPortStartScheduler(ଶୂନ୍ୟ)
{
    uxInterruptNesting = 0;
    port_uxCriticalNestingIDF = 0;
#ଯଦି CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
    port_xThreadSafeClaimed = false;
#endif
    uxSchedulerRunning = 0;
#ଯଦି config_ଅଙ୍କ_CORES > 1
    port_uxCoreStartupDone[xPortGetCoreID()] = 0;
#endif
    /* Setup the hardware to generate the tick. */
    vPortSetupTimer();

    rv_utils_intr_global_enable();
    esprv_int_set_threshold(RVHAL_INTR_ENABLE_THRESH); /* set global interrupt masking level */

    vPortYield();

    /*Should not get here*/
    ଫେରନ୍ତୁ pdFALSE;
}

ଶୂନ୍ୟ vPortEndScheduler(ଶୂନ୍ୟ)
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ଗଦା_TLS(UBaseType_t ux_ଗଦା_Pointer, uint32_t *ret_threadptr_reg_init)
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
    ବାହ୍ୟ ଅକ୍ଷର _thread_local_data_start, _thread_local_data_end;
    ବାହ୍ୟ ଅକ୍ଷର _thread_local_bss_start, _thread_local_bss_end;
    ଅଚଳ uint32_t tls_data_size = (uint32_t)&_thread_local_data_end - (uint32_t)&_thread_local_data_start;
    ଅଚଳ uint32_t tls_bss_size = (uint32_t)&_thread_local_bss_end - (uint32_t)&_thread_local_bss_start;
    ଅଚଳ uint32_t tls_area_size = ESP_ALIGN_UP(tls_data_size + tls_bss_size, 16);
    // TODO: check that TLS area fits the stack

    // Allocate space for the TLS area on the stack. The area must be aligned to 16-bytes
    ux_ଗଦା_Pointer = ESP_ALIGN_DOWN(ux_ଗଦା_Pointer - (UBaseType_t)tls_area_size, 16);
    // Initialize the TLS data with the initialization values of each TLS variable
    memcpy((ଶୂନ୍ୟ *)ux_ଗଦା_Pointer, &_thread_local_data_start, tls_data_size);
    // Initialize the TLS bss with zeroes
    memset((ଶୂନ୍ୟ *)(ux_ଗଦା_Pointer + tls_data_size), 0, tls_bss_size);

    // Save tls start address
    *ret_threadptr_reg_init = (uint32_t)ux_ଗଦା_Pointer;
    ଫେରନ୍ତୁ ux_ଗଦା_Pointer;
}

#ଯଦି CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
ସ୍ଥିର ଶୂନ୍ୟ vPortTaskWrapper(TaskFunction_t pxCode, ଶୂନ୍ୟ *pvParameters)
{
    __asm__ ଅସ୍ଥିର(".cfi_undefined ra");  // tell to debugger that it's outermost (initial) frame
    ବାହ୍ୟ ଶୂନ୍ୟ __attribute__((noreturn)) panic_abort(ଅଚଳ ଅକ୍ଷର *details);
    ସ୍ଥିର ଅକ୍ଷର DRAM_ATTR ସନ୍ଦେଶ[80] = "FreeRTOS: FreeRTOS Task \"\0";
    pxCode(pvParameters);
    /* FreeRTOS tasks should not return. Log the task name and abort. */
    /* We cannot use s(n)printf because it is in flash */
    strcat(ସନ୍ଦେଶ, pcTaskGetName(ଖାଲି));
    strcat(ସନ୍ଦେଶ, "\" should not return, Aborting now!");
    panic_abort(ସନ୍ଦେଶ);
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ଗଦା_Frame(UBaseType_t ux_ଗଦା_Pointer, TaskFunction_t pxCode, ଶୂନ୍ୟ *pvParameters, uint32_t threadptr_reg_init)
{
    /*
    Allocate space for the task's starting interrupt stack frame.
    - The stack frame must be allocated to a 16-byte aligned address.
    - We use RV_STK_FRMSZ (instead of sizeof(RvExcFrame)) as it rounds up the total size to a multiple of 16.
    */
    ux_ଗଦା_Pointer = ESP_ALIGN_DOWN(ux_ଗଦା_Pointer - RV_STK_FRMSZ, 16);

    // Clear the entire interrupt stack frame
    RvExcFrame *frame = (RvExcFrame *)ux_ଗଦା_Pointer;
    memset(frame, 0, ଆକାର(RvExcFrame));

    /* Initialize the stack frame. */
    ବାହ୍ୟ uint32_t __global_pointer$;
    #ଯଦି CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
        frame->mepc = (UBaseType_t)vPortTaskWrapper;
        frame->a0 = (UBaseType_t)pxCode;
        frame->a1 = (UBaseType_t)pvParameters;
    #ଅନ୍ୟଥା
        frame->mepc = (UBaseType_t)pxCode;
        frame->a0 = (UBaseType_t)pvParameters;
    #endif // CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
    frame->gp = (UBaseType_t)&__global_pointer$;
    frame->tp = (UBaseType_t)threadptr_reg_init;

    ଫେରନ୍ତୁ ux_ଗଦା_Pointer;
}

ଗଦା_Type_t *px_Port_Initialise_ଗଦା(ଗଦା_Type_t *px_ଶୀର୍ଷ_Of_ଗଦା, TaskFunction_t pxCode, ଶୂନ୍ୟ *pvParameters)
{
#ifdef __clang_analyzer__
    // Teach clang-tidy that pxTopOfStack cannot be a pointer to const
    ଅସ୍ଥିର ଗଦା_Type_t * pxTemp = px_ଶୀର୍ଷ_Of_ଗଦା;
    px_ଶୀର୍ଷ_Of_ଗଦା = pxTemp;
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

    UBaseType_t ux_ଗଦା_Pointer = (UBaseType_t)px_ଶୀର୍ଷ_Of_ଗଦା;
    configASSERT((ux_ଗଦା_Pointer & portBYTE_ALIGNMENT_MASK) == 0);

    // Initialize GCC TLS area
    uint32_t threadptr_reg_init;
    ux_ଗଦା_Pointer = ux_Initialise_ଗଦା_TLS(ux_ଗଦା_Pointer, &threadptr_reg_init);
    configASSERT((ux_ଗଦା_Pointer & portBYTE_ALIGNMENT_MASK) == 0);

    // Initialize the starting interrupt stack frame
    ux_ଗଦା_Pointer = ux_Initialise_ଗଦା_Frame(ux_ଗଦା_Pointer, pxCode, pvParameters, threadptr_reg_init);
    configASSERT((ux_ଗଦା_Pointer & portBYTE_ALIGNMENT_MASK) == 0);

    // Return the task's current stack pointer address which should point to the starting interrupt stack frame
    ଫେରନ୍ତୁ (ଗଦା_Type_t *)ux_ଗଦା_Pointer;
}

// ------------------- Hook Functions ----------------------

ଶୂନ୍ୟ __attribute__((weak)) v_Application_ଗଦା_Overflow_Hook(TaskHandle_t xTask, ଅକ୍ଷର *pcTaskName)
{
#define ERR_STR1 "***ERROR*** A stack overflow in task "
#define ERR_STR2 " has been detected."
    ଅଚଳ ଅକ୍ଷର *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    ଅକ୍ଷର buf[ଆକାର(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + ଆକାର(ERR_STR2) + 1 /* null char */] = {0};

    ଅକ୍ଷର *dest = buf;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଆକାର(str) / ଆକାର(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
    esp_system_abort(buf);
}

#ଯଦି  (  configUSE_TICK_HOOK > 0 )
ଶୂନ୍ୟ vApplicationTickHook( ଶୂନ୍ୟ )
{
    esp_vApplicationTickHook();
}
#endif

ବାହ୍ୟ ଶୂନ୍ୟ esp_vApplicationIdleHook(ଶୂନ୍ୟ);
#ଯଦି CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK
/*
By default, the port uses vApplicationPassiveIdleHook() to run IDF style idle
hooks. However, users may also want to provide their own vApplicationPassiveIdleHook().
In this case, we use to -Wl,--wrap option to wrap the user provided vApplicationPassiveIdleHook()
*/
ବାହ୍ୟ ଶୂନ୍ୟ __real_vApplicationPassiveIdleHook( ଶୂନ୍ୟ );
ଶୂନ୍ୟ __wrap_vApplicationPassiveIdleHook( ଶୂନ୍ୟ )
{
    esp_vApplicationIdleHook(); //Run IDF style hooks
    __real_vApplicationPassiveIdleHook(); //Call the user provided vApplicationPassiveIdleHook()
}
#ଅନ୍ୟଥା // CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK
ଶୂନ୍ୟ vApplicationPassiveIdleHook( ଶୂନ୍ୟ )
{
    esp_vApplicationIdleHook(); //Run IDF style hooks
}
#endif // CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK

/* ------------------------------------------------ Run Time Stats ------------------------------------------------- */

#ଯଦି ( CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS )

configRUN_TIME_COUNTER_TYPE x_Port_Get_Run_Time_Counter_ମୂଲ୍ୟ( ଶୂନ୍ୟ )
{
#ifdef CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
    ଫେରନ୍ତୁ (configRUN_TIME_COUNTER_TYPE) esp_timer_get_time();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ 0;
#endif // CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
}

#endif /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
