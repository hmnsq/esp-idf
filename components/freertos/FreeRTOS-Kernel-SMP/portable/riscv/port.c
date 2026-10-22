/*
 * SPDX-FileCopyrightText: 2022-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#અન્તર્ભાવ "sdkconfig.h"
#અન્તર્ભાવ <stdbool.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ "esp_compiler.h"
#અન્તર્ભાવ "soc/soc_caps.h"
#અન્તર્ભાવ "soc/periph_defs.h"
#અન્તર્ભાવ "soc/system_reg.h"
#અન્તર્ભાવ "hal/crosscore_int_ll.h"
#અન્તર્ભાવ "hal/systimer_hal.h"
#અન્તર્ભાવ "hal/systimer_ll.h"
#અન્તર્ભાવ "riscv/rvruntime-frames.h"
#અન્તર્ભાવ "riscv/rv_utils.h"
#અન્તર્ભાવ "riscv/interrupt.h"
#અન્તર્ભાવ "esp_private/crosscore_int.h"
#અન્તર્ભાવ "esp_private/esp_int_wdt.h"
#અન્તર્ભાવ "esp_private/periph_ctrl.h"
#અન્તર્ભાવ "esp_private/systimer.h"
#અન્તર્ભાવ "esp_attr.h"
#અન્તર્ભાવ "esp_system.h"
#અન્તર્ભાવ "esp_heap_caps_init.h"
#અન્તર્ભાવ "esp_task_wdt.h"
#અન્તર્ભાવ "esp_task.h"
#અન્તર્ભાવ "esp_intr_alloc.h"
#અન્તર્ભાવ "esp_log.h"
#અન્તર્ભાવ "FreeRTOS.h"       /* This pulls in portmacro.h */
#અન્તર્ભાવ "task.h"
#અન્તર્ભાવ "port_systick.h"
#અન્તર્ભાવ "portmacro.h"
#અન્તર્ભાવ "esp_memory_utils.h"
#અન્તર્ભાવ "esp_macros.h"
#જો CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
#અન્તર્ભાવ "esp_timer.h"
#endif
#ifdef CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER
#અન્તર્ભાવ "soc/periph_defs.h"
#અન્તર્ભાવ "soc/system_reg.h"
#અન્તર્ભાવ "hal/systimer_hal.h"
#અન્તર્ભાવ "hal/systimer_ll.h"
#endif
#ifdef CONFIG_PM_TRACE
#અન્તર્ભાવ "esp_private/pm_trace.h"
#endif //CONFIG_PM_TRACE

_Static_assert(portBYTE_ALIGNMENT == 16, "portBYTE_ALIGNMENT must be set to 16");
#જો CONFIG_ESP_SYSTEM_HW_ઢગલો_GUARD
/**
 * offsetof() can not be used in asm code. Then we need make sure that
 * PORT_OFFSET_PX_STACK and PORT_OFFSET_PX_END_OF_STACK have expected values.
 * Macro used in the portasm.S instead of variables to save at least 4 instruction calls
 * which accessing DRAM memory. This optimization saves CPU time in the interrupt handling.
 */

_Static_assert(offsetof( StaticTask_t, pxDummy6 ) == PORT_OFFSET_PX_ઢગલો);
_Static_assert(offsetof( StaticTask_t, pxDummy8 ) == PORT_OFFSET_PX_END_OF_ઢગલો);
#endif // CONFIG_ESP_SYSTEM_HW_STACK_GUARD

/* ---------------------------------------------------- Variables ------------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

BaseType_t uxSchedulerRunning = 0;  // Duplicate of xSchedulerRunning, accessible to port files
અસ્થિર UBaseType_t uxInterruptNesting = 0;
#જો config_અંક_CORES > 1
અસ્થિર unsigned port_uxCoreStartupDone[port_અંક_PROCESSORS] = {0};  // Indicates whether the core has completed its startup sequence
#endif
portMUX_TYPE port_xTaskLock = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE port_xISRLock = portMUX_INITIALIZER_UNLOCKED;
અસ્થિર BaseType_t xPortSwitchFlag = 0;
__attribute__((aligned(16))) ઢગલો_Type_t x_Isr_ઢગલો[config_ISR_ઢગલો_SIZE];
ઢગલો_Type_t *x_Isr_ઢગલો_ટોચ = &x_Isr_ઢગલો[0] + (config_ISR_ઢગલો_SIZE & (~((portPOINTER_SIZE_TYPE)portBYTE_ALIGNMENT_MASK)));

// Variables used for IDF style critical sections. These are orthogonal to FreeRTOS critical sections
સ્થિર UBaseType_t port_uxCriticalNestingIDF = 0;
સ્થિર UBaseType_t port_uxCriticalOldInterruptStateIDF = 0;
#જો CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
અસ્થિર bool port_xThreadSafeClaimed = false;
#endif

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// ------------------ Critical Sections --------------------

#જો CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
શૂન્ય xPortThreadSafeClaim(શૂન્ય)
{
    configASSERT(!xPortCanYield());
    configASSERT(!port_xThreadSafeClaimed);
    port_xThreadSafeClaimed = true;
}

શૂન્ય xPortThreadSafeDisclaim(શૂન્ય)
{
    configASSERT(!xPortCanYield());
    configASSERT(port_xThreadSafeClaimed);
    port_xThreadSafeClaimed = false;
}
#endif /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

શૂન્ય vPortEnterCritical(શૂન્ય)
{
#જો CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
    જો (unlikely(port_xThreadSafeClaimed)) {
        પાછા;
    }
#endif
    // Save current interrupt threshold and disable interrupts
    UBaseType_t old_thresh = ulPortSetInterruptMask();
    // Update the IDF critical nesting count
    port_uxCriticalNestingIDF++;
    જો (port_uxCriticalNestingIDF == 1) {
        // Save a copy of the old interrupt threshold
        port_uxCriticalOldInterruptStateIDF = (UBaseType_t) old_thresh;
    }
}

શૂન્ય vPortExitCritical(શૂન્ય)
{
#જો CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
    જો (unlikely(port_xThreadSafeClaimed)) {
        પાછા;
    }
#endif

    /* Critical section nesting coung must never be negative */
    configASSERT( port_uxCriticalNestingIDF > 0 );

    જો (port_uxCriticalNestingIDF > 0) {
        port_uxCriticalNestingIDF--;

        જો (port_uxCriticalNestingIDF == 0) {
            // Restore the saved interrupt threshold
            vPortClearInterruptMask((પૂર્ણાંક)port_uxCriticalOldInterruptStateIDF);
        }
    }
}

// ----------------------- System --------------------------

#define ઢગલો_WATCH_AREA_SIZE 32
#define ઢગલો_WATCH_POINT_સંખ્યા (SOC_CPU_WATCHPOINTS_અંક - 1)

શૂન્ય v_Port_Set_ઢગલો_Watchpoint(શૂન્ય *px_ઢગલો_Start)
{
    uint32_t addr = (uint32_t)px_ઢગલો_Start;
    addr = (addr + (ઢગલો_WATCH_AREA_SIZE - 1)) & (~(ઢગલો_WATCH_AREA_SIZE - 1));
    esp_cpu_set_watchpoint(ઢગલો_WATCH_POINT_સંખ્યા, (અક્ષર *)addr, ઢગલો_WATCH_AREA_SIZE, ESP_CPU_WATCHPOINT_STORE);
}



/* ---------------------------------------------- Port Implementations -------------------------------------------------
 * Implementations of Porting Interface functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

UBaseType_t ulPortSetInterruptMask(શૂન્ય)
{
    UBaseType_t prev_int_level = 0, int_level = 0;
#જો !SOC_INT_CLIC_SUPPORTED
    int_level = RVHAL_EXCM_LEVEL;
#નહીંતર
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
    પાછા prev_int_level;
}

શૂન્ય vPortClearInterruptMask(UBaseType_t mask)
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
    asm અસ્થિર ( "nop" );
    asm અસ્થિર ( "nop" );
    asm અસ્થિર ( "nop" );
}

BaseType_t xPortCheckIfInISR(શૂન્ય)
{
    પાછા uxInterruptNesting;
}

શૂન્ય vPortAssertIfInISR(શૂન્ય)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(xPortCheckIfInISR() == 0);
}

// ------------------ Critical Sections --------------------

#જો ( config_સંખ્યા_OF_CORES > 1 )
શૂન્ય vPortTakeLock( portMUX_TYPE *lock )
{
    spinlock_acquire( lock, portMUX_NO_TIMEOUT);
}

શૂન્ય vPortReleaseLock( portMUX_TYPE *lock )
{
    spinlock_release( lock );
}
#endif /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

શૂન્ય vPortYield(શૂન્ય)
{
    // TODO: IDF-8113
    અચલ પૂર્ણાંક core_id = 0;

    જો (uxInterruptNesting) {
        vPortYieldFromISR();
    } નહીંતર {

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
        જ્યારે (uxSchedulerRunning && crosscore_int_ll_get_state(core_id) != 0) {}
    }
}

શૂન્ય vPortYieldFromISR( શૂન્ય )
{
    //traceISR_EXIT_TO_SCHEDULER();
    uxSchedulerRunning = 1;
    xPortSwitchFlag = 1;
}
// ----------------------- System --------------------------

// ------------------- Run Time Stats ----------------------

// --------------------- TCB Cleanup -----------------------

#જો ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
સ્થિર શૂન્ય vPortTLSPointersDelCb( શૂન્ય *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pvThreadLocalStoragePointersDelCallback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_અંક_THREAD_LOCAL_STORAGE_POINTERS / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    માટે ( પૂર્ણાંક x = 0; x < ( config_અંક_THREAD_LOCAL_STORAGE_POINTERS / 2 ); x++ ) {
        જો ( pvThreadLocalStoragePointersDelCallback[ x ] != ખાલી ) {  //If del cb is set
            /* In case the TLSP deletion callback has been overwritten by a TLS pointer, gracefully abort. */
            જો ( !esp_ptr_executable( pvThreadLocalStoragePointersDelCallback[ x ] ) ) {
                ESP_EARLY_LOGE("FreeRTOS", "Fatal error: TLSP deletion callback at index %d overwritten with non-excutable pointer %p", x, pvThreadLocalStoragePointersDelCallback[ x ]);
                abort();
            }

            pvThreadLocalStoragePointersDelCallback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#endif /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

શૂન્ય vPortTCBPreDeleteHook( શૂન્ય *pxTCB )
{
    #જો ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        બાહ્ય શૂન્ય vTaskPreDeletionHook( શૂન્ય * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #endif /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

    #જો ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
        /* Call TLS pointers deletion callbacks */
        vPortTLSPointersDelCb( pxTCB );
    #endif /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
}

/* ------------------------------------------------ FreeRTOS Portable --------------------------------------------------
 * - Provides implementation for functions required by FreeRTOS
 * - Declared in portable.h
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------- Scheduler Start/End -------------------

BaseType_t xPortStartScheduler(શૂન્ય)
{
    uxInterruptNesting = 0;
    port_uxCriticalNestingIDF = 0;
#જો CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
    port_xThreadSafeClaimed = false;
#endif
    uxSchedulerRunning = 0;
#જો config_અંક_CORES > 1
    port_uxCoreStartupDone[xPortGetCoreID()] = 0;
#endif
    /* Setup the hardware to generate the tick. */
    vPortSetupTimer();

    rv_utils_intr_global_enable();
    esprv_int_set_threshold(RVHAL_INTR_ENABLE_THRESH); /* set global interrupt masking level */

    vPortYield();

    /*Should not get here*/
    પાછા pdFALSE;
}

શૂન્ય vPortEndScheduler(શૂન્ય)
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ઢગલો_TLS(UBaseType_t ux_ઢગલો_Pointer, uint32_t *ret_threadptr_reg_init)
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
    બાહ્ય અક્ષર _thread_local_data_start, _thread_local_data_end;
    બાહ્ય અક્ષર _thread_local_bss_start, _thread_local_bss_end;
    અચલ uint32_t tls_data_size = (uint32_t)&_thread_local_data_end - (uint32_t)&_thread_local_data_start;
    અચલ uint32_t tls_bss_size = (uint32_t)&_thread_local_bss_end - (uint32_t)&_thread_local_bss_start;
    અચલ uint32_t tls_area_size = ESP_ALIGN_UP(tls_data_size + tls_bss_size, 16);
    // TODO: check that TLS area fits the stack

    // Allocate space for the TLS area on the stack. The area must be aligned to 16-bytes
    ux_ઢગલો_Pointer = ESP_ALIGN_DOWN(ux_ઢગલો_Pointer - (UBaseType_t)tls_area_size, 16);
    // Initialize the TLS data with the initialization values of each TLS variable
    memcpy((શૂન્ય *)ux_ઢગલો_Pointer, &_thread_local_data_start, tls_data_size);
    // Initialize the TLS bss with zeroes
    memset((શૂન્ય *)(ux_ઢગલો_Pointer + tls_data_size), 0, tls_bss_size);

    // Save tls start address
    *ret_threadptr_reg_init = (uint32_t)ux_ઢગલો_Pointer;
    પાછા ux_ઢગલો_Pointer;
}

#જો CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
સ્થિર શૂન્ય vPortTaskWrapper(TaskFunction_t pxCode, શૂન્ય *pvParameters)
{
    __asm__ અસ્થિર(".cfi_undefined ra");  // tell to debugger that it's outermost (initial) frame
    બાહ્ય શૂન્ય __attribute__((noreturn)) panic_abort(અચલ અક્ષર *details);
    સ્થિર અક્ષર DRAM_ATTR સંદેશ[80] = "FreeRTOS: FreeRTOS Task \"\0";
    pxCode(pvParameters);
    /* FreeRTOS tasks should not return. Log the task name and abort. */
    /* We cannot use s(n)printf because it is in flash */
    strcat(સંદેશ, pcTaskGetName(ખાલી));
    strcat(સંદેશ, "\" should not return, Aborting now!");
    panic_abort(સંદેશ);
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ઢગલો_Frame(UBaseType_t ux_ઢગલો_Pointer, TaskFunction_t pxCode, શૂન્ય *pvParameters, uint32_t threadptr_reg_init)
{
    /*
    Allocate space for the task's starting interrupt stack frame.
    - The stack frame must be allocated to a 16-byte aligned address.
    - We use RV_STK_FRMSZ (instead of sizeof(RvExcFrame)) as it rounds up the total size to a multiple of 16.
    */
    ux_ઢગલો_Pointer = ESP_ALIGN_DOWN(ux_ઢગલો_Pointer - RV_STK_FRMSZ, 16);

    // Clear the entire interrupt stack frame
    RvExcFrame *frame = (RvExcFrame *)ux_ઢગલો_Pointer;
    memset(frame, 0, કદ(RvExcFrame));

    /* Initialize the stack frame. */
    બાહ્ય uint32_t __global_pointer$;
    #જો CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
        frame->mepc = (UBaseType_t)vPortTaskWrapper;
        frame->a0 = (UBaseType_t)pxCode;
        frame->a1 = (UBaseType_t)pvParameters;
    #નહીંતર
        frame->mepc = (UBaseType_t)pxCode;
        frame->a0 = (UBaseType_t)pvParameters;
    #endif // CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
    frame->gp = (UBaseType_t)&__global_pointer$;
    frame->tp = (UBaseType_t)threadptr_reg_init;

    પાછા ux_ઢગલો_Pointer;
}

ઢગલો_Type_t *px_Port_Initialise_ઢગલો(ઢગલો_Type_t *px_ટોચ_Of_ઢગલો, TaskFunction_t pxCode, શૂન્ય *pvParameters)
{
#ifdef __clang_analyzer__
    // Teach clang-tidy that pxTopOfStack cannot be a pointer to const
    અસ્થિર ઢગલો_Type_t * pxTemp = px_ટોચ_Of_ઢગલો;
    px_ટોચ_Of_ઢગલો = pxTemp;
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

    UBaseType_t ux_ઢગલો_Pointer = (UBaseType_t)px_ટોચ_Of_ઢગલો;
    configASSERT((ux_ઢગલો_Pointer & portBYTE_ALIGNMENT_MASK) == 0);

    // Initialize GCC TLS area
    uint32_t threadptr_reg_init;
    ux_ઢગલો_Pointer = ux_Initialise_ઢગલો_TLS(ux_ઢગલો_Pointer, &threadptr_reg_init);
    configASSERT((ux_ઢગલો_Pointer & portBYTE_ALIGNMENT_MASK) == 0);

    // Initialize the starting interrupt stack frame
    ux_ઢગલો_Pointer = ux_Initialise_ઢગલો_Frame(ux_ઢગલો_Pointer, pxCode, pvParameters, threadptr_reg_init);
    configASSERT((ux_ઢગલો_Pointer & portBYTE_ALIGNMENT_MASK) == 0);

    // Return the task's current stack pointer address which should point to the starting interrupt stack frame
    પાછા (ઢગલો_Type_t *)ux_ઢગલો_Pointer;
}

// ------------------- Hook Functions ----------------------

શૂન્ય __attribute__((weak)) v_Application_ઢગલો_Overflow_Hook(TaskHandle_t xTask, અક્ષર *pcTaskName)
{
#define ERR_STR1 "***ERROR*** A stack overflow in task "
#define ERR_STR2 " has been detected."
    અચલ અક્ષર *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    અક્ષર buf[કદ(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + કદ(ERR_STR2) + 1 /* null char */] = {0};

    અક્ષર *dest = buf;
    માટે (પૂર્ણાંક i = 0; i < કદ(str) / કદ(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
    esp_system_abort(buf);
}

#જો  (  configUSE_TICK_HOOK > 0 )
શૂન્ય vApplicationTickHook( શૂન્ય )
{
    esp_vApplicationTickHook();
}
#endif

બાહ્ય શૂન્ય esp_vApplicationIdleHook(શૂન્ય);
#જો CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK
/*
By default, the port uses vApplicationPassiveIdleHook() to run IDF style idle
hooks. However, users may also want to provide their own vApplicationPassiveIdleHook().
In this case, we use to -Wl,--wrap option to wrap the user provided vApplicationPassiveIdleHook()
*/
બાહ્ય શૂન્ય __real_vApplicationPassiveIdleHook( શૂન્ય );
શૂન્ય __wrap_vApplicationPassiveIdleHook( શૂન્ય )
{
    esp_vApplicationIdleHook(); //Run IDF style hooks
    __real_vApplicationPassiveIdleHook(); //Call the user provided vApplicationPassiveIdleHook()
}
#નહીંતર // CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK
શૂન્ય vApplicationPassiveIdleHook( શૂન્ય )
{
    esp_vApplicationIdleHook(); //Run IDF style hooks
}
#endif // CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK

/* ------------------------------------------------ Run Time Stats ------------------------------------------------- */

#જો ( CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS )

configRUN_TIME_COUNTER_TYPE x_Port_Get_Run_Time_Counter_કિંમત( શૂન્ય )
{
#ifdef CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
    પાછા (configRUN_TIME_COUNTER_TYPE) esp_timer_get_time();
#નહીંતર
    પાછા 0;
#endif // CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
}

#endif /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
