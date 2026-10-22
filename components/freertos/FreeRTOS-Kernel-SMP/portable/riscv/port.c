/*
 * SPDX-FileCopyrightText: 2022-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ "esp_compiler.h"
#ಅನ್ತರ್ಭಾವ "soc/soc_caps.h"
#ಅನ್ತರ್ಭಾವ "soc/periph_defs.h"
#ಅನ್ತರ್ಭಾವ "soc/system_reg.h"
#ಅನ್ತರ್ಭಾವ "hal/crosscore_int_ll.h"
#ಅನ್ತರ್ಭಾವ "hal/systimer_hal.h"
#ಅನ್ತರ್ಭಾವ "hal/systimer_ll.h"
#ಅನ್ತರ್ಭಾವ "riscv/rvruntime-frames.h"
#ಅನ್ತರ್ಭಾವ "riscv/rv_utils.h"
#ಅನ್ತರ್ಭಾವ "riscv/interrupt.h"
#ಅನ್ತರ್ಭಾವ "esp_private/crosscore_int.h"
#ಅನ್ತರ್ಭಾವ "esp_private/esp_int_wdt.h"
#ಅನ್ತರ್ಭಾವ "esp_private/periph_ctrl.h"
#ಅನ್ತರ್ಭಾವ "esp_private/systimer.h"
#ಅನ್ತರ್ಭಾವ "esp_attr.h"
#ಅನ್ತರ್ಭಾವ "esp_system.h"
#ಅನ್ತರ್ಭಾವ "esp_heap_caps_init.h"
#ಅನ್ತರ್ಭಾವ "esp_task_wdt.h"
#ಅನ್ತರ್ಭಾವ "esp_task.h"
#ಅನ್ತರ್ಭಾವ "esp_intr_alloc.h"
#ಅನ್ತರ್ಭಾವ "esp_log.h"
#ಅನ್ತರ್ಭಾವ "FreeRTOS.h"       /* This pulls in portmacro.h */
#ಅನ್ತರ್ಭಾವ "task.h"
#ಅನ್ತರ್ಭಾವ "port_systick.h"
#ಅನ್ತರ್ಭಾವ "portmacro.h"
#ಅನ್ತರ್ಭಾವ "esp_memory_utils.h"
#ಅನ್ತರ್ಭಾವ "esp_macros.h"
#ಯದಿ CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
#ಅನ್ತರ್ಭಾವ "esp_timer.h"
#endif
#ifdef CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER
#ಅನ್ತರ್ಭಾವ "soc/periph_defs.h"
#ಅನ್ತರ್ಭಾವ "soc/system_reg.h"
#ಅನ್ತರ್ಭಾವ "hal/systimer_hal.h"
#ಅನ್ತರ್ಭಾವ "hal/systimer_ll.h"
#endif
#ifdef CONFIG_PM_TRACE
#ಅನ್ತರ್ಭಾವ "esp_private/pm_trace.h"
#endif //CONFIG_PM_TRACE

_Static_assert(portBYTE_ALIGNMENT == 16, "portBYTE_ALIGNMENT must be set to 16");
#ಯದಿ CONFIG_ESP_SYSTEM_HW_ರಾಶಿ_GUARD
/**
 * offsetof() can not be used in asm code. Then we need make sure that
 * PORT_OFFSET_PX_STACK and PORT_OFFSET_PX_END_OF_STACK have expected values.
 * Macro used in the portasm.S instead of variables to save at least 4 instruction calls
 * which accessing DRAM memory. This optimization saves CPU time in the interrupt handling.
 */

_Static_assert(offsetof( StaticTask_t, pxDummy6 ) == PORT_OFFSET_PX_ರಾಶಿ);
_Static_assert(offsetof( StaticTask_t, pxDummy8 ) == PORT_OFFSET_PX_END_OF_ರಾಶಿ);
#endif // CONFIG_ESP_SYSTEM_HW_STACK_GUARD

/* ---------------------------------------------------- Variables ------------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

BaseType_t uxSchedulerRunning = 0;  // Duplicate of xSchedulerRunning, accessible to port files
ಚಞ್ಚಲ UBaseType_t uxInterruptNesting = 0;
#ಯದಿ config_ಅಂಕೆ_CORES > 1
ಚಞ್ಚಲ unsigned port_uxCoreStartupDone[port_ಅಂಕೆ_PROCESSORS] = {0};  // Indicates whether the core has completed its startup sequence
#endif
portMUX_TYPE port_xTaskLock = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE port_xISRLock = portMUX_INITIALIZER_UNLOCKED;
ಚಞ್ಚಲ BaseType_t xPortSwitchFlag = 0;
__attribute__((aligned(16))) ರಾಶಿ_Type_t x_Isr_ರಾಶಿ[config_ISR_ರಾಶಿ_SIZE];
ರಾಶಿ_Type_t *x_Isr_ರಾಶಿ_ತುದಿ = &x_Isr_ರಾಶಿ[0] + (config_ISR_ರಾಶಿ_SIZE & (~((portPOINTER_SIZE_TYPE)portBYTE_ALIGNMENT_MASK)));

// Variables used for IDF style critical sections. These are orthogonal to FreeRTOS critical sections
ಸ್ಥಿರ UBaseType_t port_uxCriticalNestingIDF = 0;
ಸ್ಥಿರ UBaseType_t port_uxCriticalOldInterruptStateIDF = 0;
#ಯದಿ CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
ಚಞ್ಚಲ bool port_xThreadSafeClaimed = false;
#endif

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// ------------------ Critical Sections --------------------

#ಯದಿ CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
ಶೂನ್ಯ xPortThreadSafeClaim(ಶೂನ್ಯ)
{
    configASSERT(!xPortCanYield());
    configASSERT(!port_xThreadSafeClaimed);
    port_xThreadSafeClaimed = true;
}

ಶೂನ್ಯ xPortThreadSafeDisclaim(ಶೂನ್ಯ)
{
    configASSERT(!xPortCanYield());
    configASSERT(port_xThreadSafeClaimed);
    port_xThreadSafeClaimed = false;
}
#endif /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

ಶೂನ್ಯ vPortEnterCritical(ಶೂನ್ಯ)
{
#ಯದಿ CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
    ಯದಿ (unlikely(port_xThreadSafeClaimed)) {
        ಪ್ರತ್ಯಾಗಮ;
    }
#endif
    // Save current interrupt threshold and disable interrupts
    UBaseType_t old_thresh = ulPortSetInterruptMask();
    // Update the IDF critical nesting count
    port_uxCriticalNestingIDF++;
    ಯದಿ (port_uxCriticalNestingIDF == 1) {
        // Save a copy of the old interrupt threshold
        port_uxCriticalOldInterruptStateIDF = (UBaseType_t) old_thresh;
    }
}

ಶೂನ್ಯ vPortExitCritical(ಶೂನ್ಯ)
{
#ಯದಿ CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
    ಯದಿ (unlikely(port_xThreadSafeClaimed)) {
        ಪ್ರತ್ಯಾಗಮ;
    }
#endif

    /* Critical section nesting coung must never be negative */
    configASSERT( port_uxCriticalNestingIDF > 0 );

    ಯದಿ (port_uxCriticalNestingIDF > 0) {
        port_uxCriticalNestingIDF--;

        ಯದಿ (port_uxCriticalNestingIDF == 0) {
            // Restore the saved interrupt threshold
            vPortClearInterruptMask((ಪೂರ್ಣಾಙ್ಕ)port_uxCriticalOldInterruptStateIDF);
        }
    }
}

// ----------------------- System --------------------------

#define ರಾಶಿ_WATCH_AREA_SIZE 32
#define ರಾಶಿ_WATCH_POINT_ಸಂಖ್ಯೆ (SOC_CPU_WATCHPOINTS_ಅಂಕೆ - 1)

ಶೂನ್ಯ v_Port_Set_ರಾಶಿ_Watchpoint(ಶೂನ್ಯ *px_ರಾಶಿ_Start)
{
    uint32_t addr = (uint32_t)px_ರಾಶಿ_Start;
    addr = (addr + (ರಾಶಿ_WATCH_AREA_SIZE - 1)) & (~(ರಾಶಿ_WATCH_AREA_SIZE - 1));
    esp_cpu_set_watchpoint(ರಾಶಿ_WATCH_POINT_ಸಂಖ್ಯೆ, (ವರ್ಣ *)addr, ರಾಶಿ_WATCH_AREA_SIZE, ESP_CPU_WATCHPOINT_STORE);
}



/* ---------------------------------------------- Port Implementations -------------------------------------------------
 * Implementations of Porting Interface functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

UBaseType_t ulPortSetInterruptMask(ಶೂನ್ಯ)
{
    UBaseType_t prev_int_level = 0, int_level = 0;
#ಯದಿ !SOC_INT_CLIC_SUPPORTED
    int_level = RVHAL_EXCM_LEVEL;
#ಅನ್ಯಥಾ
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
    ಪ್ರತ್ಯಾಗಮ prev_int_level;
}

ಶೂನ್ಯ vPortClearInterruptMask(UBaseType_t mask)
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
    asm ಚಞ್ಚಲ ( "nop" );
    asm ಚಞ್ಚಲ ( "nop" );
    asm ಚಞ್ಚಲ ( "nop" );
}

BaseType_t xPortCheckIfInISR(ಶೂನ್ಯ)
{
    ಪ್ರತ್ಯಾಗಮ uxInterruptNesting;
}

ಶೂನ್ಯ vPortAssertIfInISR(ಶೂನ್ಯ)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(xPortCheckIfInISR() == 0);
}

// ------------------ Critical Sections --------------------

#ಯದಿ ( config_ಸಂಖ್ಯೆ_OF_CORES > 1 )
ಶೂನ್ಯ vPortTakeLock( portMUX_TYPE *lock )
{
    spinlock_acquire( lock, portMUX_NO_TIMEOUT);
}

ಶೂನ್ಯ vPortReleaseLock( portMUX_TYPE *lock )
{
    spinlock_release( lock );
}
#endif /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

ಶೂನ್ಯ vPortYield(ಶೂನ್ಯ)
{
    // TODO: IDF-8113
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ core_id = 0;

    ಯದಿ (uxInterruptNesting) {
        vPortYieldFromISR();
    } ಅನ್ಯಥಾ {

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
        ಯಾವತ್ (uxSchedulerRunning && crosscore_int_ll_get_state(core_id) != 0) {}
    }
}

ಶೂನ್ಯ vPortYieldFromISR( ಶೂನ್ಯ )
{
    //traceISR_EXIT_TO_SCHEDULER();
    uxSchedulerRunning = 1;
    xPortSwitchFlag = 1;
}
// ----------------------- System --------------------------

// ------------------- Run Time Stats ----------------------

// --------------------- TCB Cleanup -----------------------

#ಯದಿ ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
ಸ್ಥಿರ ಶೂನ್ಯ vPortTLSPointersDelCb( ಶೂನ್ಯ *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pvThreadLocalStoragePointersDelCallback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_ಅಂಕೆ_THREAD_LOCAL_STORAGE_POINTERS / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    ಪ್ರತಿ ( ಪೂರ್ಣಾಙ್ಕ x = 0; x < ( config_ಅಂಕೆ_THREAD_LOCAL_STORAGE_POINTERS / 2 ); x++ ) {
        ಯದಿ ( pvThreadLocalStoragePointersDelCallback[ x ] != ರಿಕ್ತ ) {  //If del cb is set
            /* In case the TLSP deletion callback has been overwritten by a TLS pointer, gracefully abort. */
            ಯದಿ ( !esp_ptr_executable( pvThreadLocalStoragePointersDelCallback[ x ] ) ) {
                ESP_EARLY_LOGE("FreeRTOS", "Fatal error: TLSP deletion callback at index %d overwritten with non-excutable pointer %p", x, pvThreadLocalStoragePointersDelCallback[ x ]);
                abort();
            }

            pvThreadLocalStoragePointersDelCallback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#endif /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

ಶೂನ್ಯ vPortTCBPreDeleteHook( ಶೂನ್ಯ *pxTCB )
{
    #ಯದಿ ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        ಬಾಹ್ಯ ಶೂನ್ಯ vTaskPreDeletionHook( ಶೂನ್ಯ * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #endif /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

    #ಯದಿ ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
        /* Call TLS pointers deletion callbacks */
        vPortTLSPointersDelCb( pxTCB );
    #endif /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
}

/* ------------------------------------------------ FreeRTOS Portable --------------------------------------------------
 * - Provides implementation for functions required by FreeRTOS
 * - Declared in portable.h
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------- Scheduler Start/End -------------------

BaseType_t xPortStartScheduler(ಶೂನ್ಯ)
{
    uxInterruptNesting = 0;
    port_uxCriticalNestingIDF = 0;
#ಯದಿ CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
    port_xThreadSafeClaimed = false;
#endif
    uxSchedulerRunning = 0;
#ಯದಿ config_ಅಂಕೆ_CORES > 1
    port_uxCoreStartupDone[xPortGetCoreID()] = 0;
#endif
    /* Setup the hardware to generate the tick. */
    vPortSetupTimer();

    rv_utils_intr_global_enable();
    esprv_int_set_threshold(RVHAL_INTR_ENABLE_THRESH); /* set global interrupt masking level */

    vPortYield();

    /*Should not get here*/
    ಪ್ರತ್ಯಾಗಮ pdFALSE;
}

ಶೂನ್ಯ vPortEndScheduler(ಶೂನ್ಯ)
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ರಾಶಿ_TLS(UBaseType_t ux_ರಾಶಿ_Pointer, uint32_t *ret_threadptr_reg_init)
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
    ಬಾಹ್ಯ ವರ್ಣ _thread_local_data_start, _thread_local_data_end;
    ಬಾಹ್ಯ ವರ್ಣ _thread_local_bss_start, _thread_local_bss_end;
    ಧ್ರುವ uint32_t tls_data_size = (uint32_t)&_thread_local_data_end - (uint32_t)&_thread_local_data_start;
    ಧ್ರುವ uint32_t tls_bss_size = (uint32_t)&_thread_local_bss_end - (uint32_t)&_thread_local_bss_start;
    ಧ್ರುವ uint32_t tls_area_size = ESP_ALIGN_UP(tls_data_size + tls_bss_size, 16);
    // TODO: check that TLS area fits the stack

    // Allocate space for the TLS area on the stack. The area must be aligned to 16-bytes
    ux_ರಾಶಿ_Pointer = ESP_ALIGN_DOWN(ux_ರಾಶಿ_Pointer - (UBaseType_t)tls_area_size, 16);
    // Initialize the TLS data with the initialization values of each TLS variable
    memcpy((ಶೂನ್ಯ *)ux_ರಾಶಿ_Pointer, &_thread_local_data_start, tls_data_size);
    // Initialize the TLS bss with zeroes
    memset((ಶೂನ್ಯ *)(ux_ರಾಶಿ_Pointer + tls_data_size), 0, tls_bss_size);

    // Save tls start address
    *ret_threadptr_reg_init = (uint32_t)ux_ರಾಶಿ_Pointer;
    ಪ್ರತ್ಯಾಗಮ ux_ರಾಶಿ_Pointer;
}

#ಯದಿ CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
ಸ್ಥಿರ ಶೂನ್ಯ vPortTaskWrapper(TaskFunction_t pxCode, ಶೂನ್ಯ *pvParameters)
{
    __asm__ ಚಞ್ಚಲ(".cfi_undefined ra");  // tell to debugger that it's outermost (initial) frame
    ಬಾಹ್ಯ ಶೂನ್ಯ __attribute__((noreturn)) panic_abort(ಧ್ರುವ ವರ್ಣ *details);
    ಸ್ಥಿರ ವರ್ಣ DRAM_ATTR ಸಂದೇಶ[80] = "FreeRTOS: FreeRTOS Task \"\0";
    pxCode(pvParameters);
    /* FreeRTOS tasks should not return. Log the task name and abort. */
    /* We cannot use s(n)printf because it is in flash */
    strcat(ಸಂದೇಶ, pcTaskGetName(ರಿಕ್ತ));
    strcat(ಸಂದೇಶ, "\" should not return, Aborting now!");
    panic_abort(ಸಂದೇಶ);
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ರಾಶಿ_Frame(UBaseType_t ux_ರಾಶಿ_Pointer, TaskFunction_t pxCode, ಶೂನ್ಯ *pvParameters, uint32_t threadptr_reg_init)
{
    /*
    Allocate space for the task's starting interrupt stack frame.
    - The stack frame must be allocated to a 16-byte aligned address.
    - We use RV_STK_FRMSZ (instead of sizeof(RvExcFrame)) as it rounds up the total size to a multiple of 16.
    */
    ux_ರಾಶಿ_Pointer = ESP_ALIGN_DOWN(ux_ರಾಶಿ_Pointer - RV_STK_FRMSZ, 16);

    // Clear the entire interrupt stack frame
    RvExcFrame *frame = (RvExcFrame *)ux_ರಾಶಿ_Pointer;
    memset(frame, 0, ಪರಿಮಾಣ(RvExcFrame));

    /* Initialize the stack frame. */
    ಬಾಹ್ಯ uint32_t __global_pointer$;
    #ಯದಿ CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
        frame->mepc = (UBaseType_t)vPortTaskWrapper;
        frame->a0 = (UBaseType_t)pxCode;
        frame->a1 = (UBaseType_t)pvParameters;
    #ಅನ್ಯಥಾ
        frame->mepc = (UBaseType_t)pxCode;
        frame->a0 = (UBaseType_t)pvParameters;
    #endif // CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
    frame->gp = (UBaseType_t)&__global_pointer$;
    frame->tp = (UBaseType_t)threadptr_reg_init;

    ಪ್ರತ್ಯಾಗಮ ux_ರಾಶಿ_Pointer;
}

ರಾಶಿ_Type_t *px_Port_Initialise_ರಾಶಿ(ರಾಶಿ_Type_t *px_ತುದಿ_Of_ರಾಶಿ, TaskFunction_t pxCode, ಶೂನ್ಯ *pvParameters)
{
#ifdef __clang_analyzer__
    // Teach clang-tidy that pxTopOfStack cannot be a pointer to const
    ಚಞ್ಚಲ ರಾಶಿ_Type_t * pxTemp = px_ತುದಿ_Of_ರಾಶಿ;
    px_ತುದಿ_Of_ರಾಶಿ = pxTemp;
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

    UBaseType_t ux_ರಾಶಿ_Pointer = (UBaseType_t)px_ತುದಿ_Of_ರಾಶಿ;
    configASSERT((ux_ರಾಶಿ_Pointer & portBYTE_ALIGNMENT_MASK) == 0);

    // Initialize GCC TLS area
    uint32_t threadptr_reg_init;
    ux_ರಾಶಿ_Pointer = ux_Initialise_ರಾಶಿ_TLS(ux_ರಾಶಿ_Pointer, &threadptr_reg_init);
    configASSERT((ux_ರಾಶಿ_Pointer & portBYTE_ALIGNMENT_MASK) == 0);

    // Initialize the starting interrupt stack frame
    ux_ರಾಶಿ_Pointer = ux_Initialise_ರಾಶಿ_Frame(ux_ರಾಶಿ_Pointer, pxCode, pvParameters, threadptr_reg_init);
    configASSERT((ux_ರಾಶಿ_Pointer & portBYTE_ALIGNMENT_MASK) == 0);

    // Return the task's current stack pointer address which should point to the starting interrupt stack frame
    ಪ್ರತ್ಯಾಗಮ (ರಾಶಿ_Type_t *)ux_ರಾಶಿ_Pointer;
}

// ------------------- Hook Functions ----------------------

ಶೂನ್ಯ __attribute__((weak)) v_Application_ರಾಶಿ_Overflow_Hook(TaskHandle_t xTask, ವರ್ಣ *pcTaskName)
{
#define ERR_STR1 "***ERROR*** A stack overflow in task "
#define ERR_STR2 " has been detected."
    ಧ್ರುವ ವರ್ಣ *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    ವರ್ಣ buf[ಪರಿಮಾಣ(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + ಪರಿಮಾಣ(ERR_STR2) + 1 /* null char */] = {0};

    ವರ್ಣ *dest = buf;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಪರಿಮಾಣ(str) / ಪರಿಮಾಣ(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
    esp_system_abort(buf);
}

#ಯದಿ  (  configUSE_TICK_HOOK > 0 )
ಶೂನ್ಯ vApplicationTickHook( ಶೂನ್ಯ )
{
    esp_vApplicationTickHook();
}
#endif

ಬಾಹ್ಯ ಶೂನ್ಯ esp_vApplicationIdleHook(ಶೂನ್ಯ);
#ಯದಿ CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK
/*
By default, the port uses vApplicationPassiveIdleHook() to run IDF style idle
hooks. However, users may also want to provide their own vApplicationPassiveIdleHook().
In this case, we use to -Wl,--wrap option to wrap the user provided vApplicationPassiveIdleHook()
*/
ಬಾಹ್ಯ ಶೂನ್ಯ __real_vApplicationPassiveIdleHook( ಶೂನ್ಯ );
ಶೂನ್ಯ __wrap_vApplicationPassiveIdleHook( ಶೂನ್ಯ )
{
    esp_vApplicationIdleHook(); //Run IDF style hooks
    __real_vApplicationPassiveIdleHook(); //Call the user provided vApplicationPassiveIdleHook()
}
#ಅನ್ಯಥಾ // CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK
ಶೂನ್ಯ vApplicationPassiveIdleHook( ಶೂನ್ಯ )
{
    esp_vApplicationIdleHook(); //Run IDF style hooks
}
#endif // CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK

/* ------------------------------------------------ Run Time Stats ------------------------------------------------- */

#ಯದಿ ( CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS )

configRUN_TIME_COUNTER_TYPE x_Port_Get_Run_Time_Counter_ಮೌಲ್ಯ( ಶೂನ್ಯ )
{
#ifdef CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
    ಪ್ರತ್ಯಾಗಮ (configRUN_TIME_COUNTER_TYPE) esp_timer_get_time();
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ 0;
#endif // CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
}

#endif /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
