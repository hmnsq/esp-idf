/*
 * SPDX-FileCopyrightText: 2022-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#समावेश "sdkconfig.h"
#समावेश <stdbool.h>
#समावेश <string.h>
#समावेश "esp_compiler.h"
#समावेश "soc/soc_caps.h"
#समावेश "soc/periph_defs.h"
#समावेश "soc/system_reg.h"
#समावेश "hal/crosscore_int_ll.h"
#समावेश "hal/systimer_hal.h"
#समावेश "hal/systimer_ll.h"
#समावेश "riscv/rvruntime-frames.h"
#समावेश "riscv/rv_utils.h"
#समावेश "riscv/interrupt.h"
#समावेश "esp_private/crosscore_int.h"
#समावेश "esp_private/esp_int_wdt.h"
#समावेश "esp_private/periph_ctrl.h"
#समावेश "esp_private/systimer.h"
#समावेश "esp_attr.h"
#समावेश "esp_system.h"
#समावेश "esp_heap_caps_init.h"
#समावेश "esp_task_wdt.h"
#समावेश "esp_task.h"
#समावेश "esp_intr_alloc.h"
#समावेश "esp_log.h"
#समावेश "FreeRTOS.h"       /* This pulls in portmacro.h */
#समावेश "task.h"
#समावेश "port_systick.h"
#समावेश "portmacro.h"
#समावेश "esp_memory_utils.h"
#समावेश "esp_macros.h"
#अगर CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
#समावेश "esp_timer.h"
#पूर्ण_अगर
#अगर_घोषित CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER
#समावेश "soc/periph_defs.h"
#समावेश "soc/system_reg.h"
#समावेश "hal/systimer_hal.h"
#समावेश "hal/systimer_ll.h"
#पूर्ण_अगर
#अगर_घोषित CONFIG_PM_TRACE
#समावेश "esp_private/pm_trace.h"
#पूर्ण_अगर //CONFIG_PM_TRACE

_Static_assert(portBYTE_ALIGNMENT == 16, "portBYTE_ALIGNMENT must be set to 16");
#अगर CONFIG_ESP_SYSTEM_HW_ढेर_GUARD
/**
 * offsetof() can not be used in asm code. Then we need make sure that
 * PORT_OFFSET_PX_STACK and PORT_OFFSET_PX_END_OF_STACK have expected values.
 * Macro used in the portasm.S instead of variables to save at least 4 instruction calls
 * which accessing DRAM memory. This optimization saves CPU time in the interrupt handling.
 */

_Static_assert(दुरत्व( StaticTask_t, pxDummy6 ) == PORT_OFFSET_PX_ढेर);
_Static_assert(दुरत्व( StaticTask_t, pxDummy8 ) == PORT_OFFSET_PX_END_OF_ढेर);
#पूर्ण_अगर // CONFIG_ESP_SYSTEM_HW_STACK_GUARD

/* ---------------------------------------------------- Variables ------------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

BaseType_t uxSchedulerRunning = 0;  // Duplicate of xSchedulerRunning, accessible to port files
अस्थिर UBaseType_t uxInterruptNesting = 0;
#अगर config_अंक_CORES > 1
अस्थिर अचिन्हित port_uxCoreStartupDone[port_अंक_PROCESSORS] = {0};  // Indicates whether the core has completed its startup sequence
#पूर्ण_अगर
portMUX_TYPE port_xTaskLock = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE port_xISRLock = portMUX_INITIALIZER_UNLOCKED;
अस्थिर BaseType_t xPortSwitchFlag = 0;
__attribute__((aligned(16))) ढेर_Type_t x_Isr_ढेर[config_ISR_ढेर_आकार];
ढेर_Type_t *x_Isr_ढेर_शीर्ष = &x_Isr_ढेर[0] + (config_ISR_ढेर_आकार & (~((port_POINTER_आकार_TYPE)portBYTE_ALIGNMENT_MASK)));

// Variables used for IDF style critical sections. These are orthogonal to FreeRTOS critical sections
अटल UBaseType_t port_uxCriticalNestingIDF = 0;
अटल UBaseType_t port_uxCriticalOldInterruptStateIDF = 0;
#अगर CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
अस्थिर bool port_xThreadSafeClaimed = false;
#पूर्ण_अगर

/* ------------------------------------------------ IDF Compatibility --------------------------------------------------
 * - These need to be defined for IDF to compile
 * ------------------------------------------------------------------------------------------------------------------ */

// ------------------ Critical Sections --------------------

#अगर CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
व्योम xPortThreadSafeClaim(व्योम)
{
    configASSERT(!xPortCanYield());
    configASSERT(!port_xThreadSafeClaimed);
    port_xThreadSafeClaimed = true;
}

व्योम xPortThreadSafeDisclaim(व्योम)
{
    configASSERT(!xPortCanYield());
    configASSERT(port_xThreadSafeClaimed);
    port_xThreadSafeClaimed = false;
}
#पूर्ण_अगर /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

व्योम vPortEnterCritical(व्योम)
{
#अगर CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
    अगर (unlikely(port_xThreadSafeClaimed)) {
        वापस;
    }
#पूर्ण_अगर
    // Save current interrupt threshold and disable interrupts
    UBaseType_t old_thresh = ulPortSetInterruptMask();
    // Update the IDF critical nesting count
    port_uxCriticalNestingIDF++;
    अगर (port_uxCriticalNestingIDF == 1) {
        // Save a copy of the old interrupt threshold
        port_uxCriticalOldInterruptStateIDF = (UBaseType_t) old_thresh;
    }
}

व्योम vPortExitCritical(व्योम)
{
#अगर CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
    अगर (unlikely(port_xThreadSafeClaimed)) {
        वापस;
    }
#पूर्ण_अगर

    /* Critical section nesting coung must never be negative */
    configASSERT( port_uxCriticalNestingIDF > 0 );

    अगर (port_uxCriticalNestingIDF > 0) {
        port_uxCriticalNestingIDF--;

        अगर (port_uxCriticalNestingIDF == 0) {
            // Restore the saved interrupt threshold
            vPortClearInterruptMask((पूर्णांक)port_uxCriticalOldInterruptStateIDF);
        }
    }
}

// ----------------------- System --------------------------

#घोषणा ढेर_WATCH_AREA_आकार 32
#घोषणा ढेर_WATCH_POINT_संख्या (SOC_CPU_WATCHPOINTS_अंक - 1)

व्योम v_Port_Set_ढेर_Watchpoint(व्योम *px_ढेर_Start)
{
    uint32_t addr = (uint32_t)px_ढेर_Start;
    addr = (addr + (ढेर_WATCH_AREA_आकार - 1)) & (~(ढेर_WATCH_AREA_आकार - 1));
    esp_cpu_set_watchpoint(ढेर_WATCH_POINT_संख्या, (अक्षर *)addr, ढेर_WATCH_AREA_आकार, ESP_CPU_WATCHPOINT_STORE);
}



/* ---------------------------------------------- Port Implementations -------------------------------------------------
 * Implementations of Porting Interface functions
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

UBaseType_t ulPortSetInterruptMask(व्योम)
{
    UBaseType_t पूर्व_int_level = 0, int_level = 0;
#अगर !SOC_INT_CLIC_SUPPORTED
    int_level = RVHAL_EXCM_LEVEL;
#अन्यथा
    int_level = RVHAL_EXCM_LEVEL_CLIC;
#पूर्ण_अगर

    पूर्व_int_level = rv_utils_set_intlevel_regval(int_level);
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
    वापस पूर्व_int_level;
}

व्योम vPortClearInterruptMask(UBaseType_t mask)
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
    यंत्र अस्थिर ( "nop" );
    यंत्र अस्थिर ( "nop" );
    यंत्र अस्थिर ( "nop" );
}

BaseType_t xPortCheckIfInISR(व्योम)
{
    वापस uxInterruptNesting;
}

व्योम vPortAssertIfInISR(व्योम)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(xPortCheckIfInISR() == 0);
}

// ------------------ Critical Sections --------------------

#अगर ( config_संख्या_OF_CORES > 1 )
व्योम vPortTakeLock( portMUX_TYPE *lock )
{
    spinlock_acquire( lock, portMUX_NO_TIMEOUT);
}

व्योम vPortReleaseLock( portMUX_TYPE *lock )
{
    spinlock_release( lock );
}
#पूर्ण_अगर /* configNUMBER_OF_CORES > 1 */

// ---------------------- Yielding -------------------------

व्योम vPortYield(व्योम)
{
    // TODO: IDF-8113
    स्थिर पूर्णांक core_id = 0;

    अगर (uxInterruptNesting) {
        vPortYieldFromISR();
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
        जबतक (uxSchedulerRunning && crosscore_int_ll_get_state(core_id) != 0) {}
    }
}

व्योम vPortYieldFromISR( व्योम )
{
    //traceISR_EXIT_TO_SCHEDULER();
    uxSchedulerRunning = 1;
    xPortSwitchFlag = 1;
}
// ----------------------- System --------------------------

// ------------------- Run Time Stats ----------------------

// --------------------- TCB Cleanup -----------------------

#अगर ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
अटल व्योम vPortTLSPointersDelCb( व्योम *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pvThreadLocalStoragePointersDelCallback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_अंक_THREAD_LOCAL_STORAGE_POINTERS / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    क्रम ( पूर्णांक x = 0; x < ( config_अंक_THREAD_LOCAL_STORAGE_POINTERS / 2 ); x++ ) {
        अगर ( pvThreadLocalStoragePointersDelCallback[ x ] != शून्य ) {  //If del cb is set
            /* In case the TLSP deletion callback has been overwritten by a TLS pointer, gracefully abort. */
            अगर ( !esp_ptr_executable( pvThreadLocalStoragePointersDelCallback[ x ] ) ) {
                ESP_EARLY_LOGE("FreeRTOS", "Fatal error: TLSP deletion callback at index %d overwritten with non-excutable pointer %p", x, pvThreadLocalStoragePointersDelCallback[ x ]);
                पात();
            }

            pvThreadLocalStoragePointersDelCallback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#पूर्ण_अगर /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

व्योम vPortTCBPreDeleteHook( व्योम *pxTCB )
{
    #अगर ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        बाह्य व्योम vTaskPreDeletionHook( व्योम * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #पूर्ण_अगर /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

    #अगर ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
        /* Call TLS pointers deletion callbacks */
        vPortTLSPointersDelCb( pxTCB );
    #पूर्ण_अगर /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
}

/* ------------------------------------------------ FreeRTOS Portable --------------------------------------------------
 * - Provides implementation for functions required by FreeRTOS
 * - Declared in portable.h
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------- Scheduler Start/End -------------------

BaseType_t xPortStartScheduler(व्योम)
{
    uxInterruptNesting = 0;
    port_uxCriticalNestingIDF = 0;
#अगर CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
    port_xThreadSafeClaimed = false;
#पूर्ण_अगर
    uxSchedulerRunning = 0;
#अगर config_अंक_CORES > 1
    port_uxCoreStartupDone[xPortGetCoreID()] = 0;
#पूर्ण_अगर
    /* Setup the hardware to generate the tick. */
    vPortSetupTimer();

    rv_utils_intr_global_enable();
    esprv_int_set_threshold(RVHAL_INTR_ENABLE_THRESH); /* set global interrupt masking level */

    vPortYield();

    /*Should not get here*/
    वापस pdFALSE;
}

व्योम vPortEndScheduler(व्योम)
{
    /* very unlikely this function will be called, so just trap here */
    पात();
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ढेर_TLS(UBaseType_t ux_ढेर_Pointer, uint32_t *ret_threadptr_reg_init)
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
    बाह्य अक्षर thread_local_तथ्य_start, thread_local_तथ्य_end;
    बाह्य अक्षर _thread_local_bss_start, _thread_local_bss_end;
    स्थिर uint32_t tls_तथ्य_आकार = (uint32_t)&thread_local_तथ्य_end - (uint32_t)&thread_local_तथ्य_start;
    स्थिर uint32_t tls_bss_आकार = (uint32_t)&_thread_local_bss_end - (uint32_t)&_thread_local_bss_start;
    स्थिर uint32_t tls_area_आकार = ESP_ALIGN_UP(tls_तथ्य_आकार + tls_bss_आकार, 16);
    // TODO: check that TLS area fits the stack

    // Allocate space for the TLS area on the stack. The area must be aligned to 16-bytes
    ux_ढेर_Pointer = ESP_ALIGN_DOWN(ux_ढेर_Pointer - (UBaseType_t)tls_area_आकार, 16);
    // Initialize the TLS data with the initialization values of each TLS variable
    स_नकल((व्योम *)ux_ढेर_Pointer, &thread_local_तथ्य_start, tls_तथ्य_आकार);
    // Initialize the TLS bss with zeroes
    स_रखो((व्योम *)(ux_ढेर_Pointer + tls_तथ्य_आकार), 0, tls_bss_आकार);

    // Save tls start address
    *ret_threadptr_reg_init = (uint32_t)ux_ढेर_Pointer;
    वापस ux_ढेर_Pointer;
}

#अगर CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
अटल व्योम vPortTaskWrapper(TaskFunction_t pxCode, व्योम *pvParameters)
{
    __asm__ अस्थिर(".cfi_undefined ra");  // tell to debugger that it's outermost (initial) frame
    बाह्य व्योम __attribute__((noreturn)) panic_abort(स्थिर अक्षर *details);
    अटल अक्षर DRAM_ATTR संदेश[80] = "FreeRTOS: FreeRTOS Task \"\0";
    pxCode(pvParameters);
    /* FreeRTOS tasks should not return. Log the task name and abort. */
    /* We cannot use s(n)printf because it is in flash */
    म_जोड़ो(संदेश, pcTaskGetName(शून्य));
    म_जोड़ो(संदेश, "\" should not return, Aborting now!");
    panic_abort(संदेश);
}
#पूर्ण_अगर // CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER

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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ढेर_Frame(UBaseType_t ux_ढेर_Pointer, TaskFunction_t pxCode, व्योम *pvParameters, uint32_t threadptr_reg_init)
{
    /*
    Allocate space for the task's starting interrupt stack frame.
    - The stack frame must be allocated to a 16-byte aligned address.
    - We use RV_STK_FRMSZ (instead of sizeof(RvExcFrame)) as it rounds up the total size to a multiple of 16.
    */
    ux_ढेर_Pointer = ESP_ALIGN_DOWN(ux_ढेर_Pointer - RV_STK_FRMSZ, 16);

    // Clear the entire interrupt stack frame
    RvExcFrame *frame = (RvExcFrame *)ux_ढेर_Pointer;
    स_रखो(frame, 0, माप(RvExcFrame));

    /* Initialize the stack frame. */
    बाह्य uint32_t __global_pointer$;
    #अगर CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
        frame->mepc = (UBaseType_t)vPortTaskWrapper;
        frame->a0 = (UBaseType_t)pxCode;
        frame->a1 = (UBaseType_t)pvParameters;
    #अन्यथा
        frame->mepc = (UBaseType_t)pxCode;
        frame->a0 = (UBaseType_t)pvParameters;
    #पूर्ण_अगर // CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
    frame->gp = (UBaseType_t)&__global_pointer$;
    frame->tp = (UBaseType_t)threadptr_reg_init;

    वापस ux_ढेर_Pointer;
}

ढेर_Type_t *px_Port_Initialise_ढेर(ढेर_Type_t *px_शीर्ष_Of_ढेर, TaskFunction_t pxCode, व्योम *pvParameters)
{
#अगर_घोषित __clang_analyzer__
    // Teach clang-tidy that pxTopOfStack cannot be a pointer to const
    अस्थिर ढेर_Type_t * px_अस्थायी = px_शीर्ष_Of_ढेर;
    px_शीर्ष_Of_ढेर = px_अस्थायी;
#पूर्ण_अगर /*__clang_analyzer__ */
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

    UBaseType_t ux_ढेर_Pointer = (UBaseType_t)px_शीर्ष_Of_ढेर;
    configASSERT((ux_ढेर_Pointer & portBYTE_ALIGNMENT_MASK) == 0);

    // Initialize GCC TLS area
    uint32_t threadptr_reg_init;
    ux_ढेर_Pointer = ux_Initialise_ढेर_TLS(ux_ढेर_Pointer, &threadptr_reg_init);
    configASSERT((ux_ढेर_Pointer & portBYTE_ALIGNMENT_MASK) == 0);

    // Initialize the starting interrupt stack frame
    ux_ढेर_Pointer = ux_Initialise_ढेर_Frame(ux_ढेर_Pointer, pxCode, pvParameters, threadptr_reg_init);
    configASSERT((ux_ढेर_Pointer & portBYTE_ALIGNMENT_MASK) == 0);

    // Return the task's current stack pointer address which should point to the starting interrupt stack frame
    वापस (ढेर_Type_t *)ux_ढेर_Pointer;
}

// ------------------- Hook Functions ----------------------

व्योम __attribute__((weak)) v_Application_ढेर_Overflow_Hook(TaskHandle_t xTask, अक्षर *pcTaskName)
{
#घोषणा ERR_STR1 "***ERROR*** A stack overflow in task "
#घोषणा ERR_STR2 " has been detected."
    स्थिर अक्षर *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    अक्षर buf[माप(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + माप(ERR_STR2) + 1 /* null char */] = {0};

    अक्षर *dest = buf;
    क्रम (पूर्णांक i = 0; i < माप(str) / माप(str[0]); i++) {
        dest = म_जोड़ो(dest, str[i]);
    }
    esp_system_abort(buf);
}

#अगर  (  configUSE_TICK_HOOK > 0 )
व्योम vApplicationTickHook( व्योम )
{
    esp_vApplicationTickHook();
}
#पूर्ण_अगर

बाह्य व्योम esp_vApplicationIdleHook(व्योम);
#अगर CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK
/*
By default, the port uses vApplicationPassiveIdleHook() to run IDF style idle
hooks. However, users may also want to provide their own vApplicationPassiveIdleHook().
In this case, we use to -Wl,--wrap option to wrap the user provided vApplicationPassiveIdleHook()
*/
बाह्य व्योम __real_vApplicationPassiveIdleHook( व्योम );
व्योम __wrap_vApplicationPassiveIdleHook( व्योम )
{
    esp_vApplicationIdleHook(); //Run IDF style hooks
    __real_vApplicationPassiveIdleHook(); //Call the user provided vApplicationPassiveIdleHook()
}
#अन्यथा // CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK
व्योम vApplicationPassiveIdleHook( व्योम )
{
    esp_vApplicationIdleHook(); //Run IDF style hooks
}
#पूर्ण_अगर // CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK

/* ------------------------------------------------ Run Time Stats ------------------------------------------------- */

#अगर ( CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS )

config_RUN_TIME_गणक_TYPE x_Port_Get_Run_Time_गणक_मूल्य( व्योम )
{
#अगर_घोषित CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
    वापस (config_RUN_TIME_गणक_TYPE) esp_timer_get_time();
#अन्यथा
    वापस 0;
#पूर्ण_अगर // CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
}

#पूर्ण_अगर /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
