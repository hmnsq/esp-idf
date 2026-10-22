/*
 * FreeRTOS Kernel V10.5.1 (ESP-IDF SMP modified)
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-FileCopyrightText: 2021 Amazon.com, Inc. or its affiliates
 *
 * SPDX-License-Identifier: MIT
 *
 * SPDX-FileContributor: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/*-----------------------------------------------------------------------
 * Implementation of functions defined in portable.h for the RISC-V port.
 *----------------------------------------------------------------------*/

#ضمن "sdkconfig.h"
#ضمن <stdbool.h>
#ضمن <string.h>
#ضمن "soc/soc_caps.h"
#ضمن "soc/periph_defs.h"
#ضمن "soc/system_reg.h"
#ضمن "soc/interrupt_reg.h"
#ضمن "hal/systimer_hal.h"
#ضمن "hal/systimer_ll.h"
#ضمن "riscv/rvruntime-frames.h"
#ضمن "riscv/rv_utils.h"
#ضمن "riscv/interrupt.h"
#ضمن "esp_private/crosscore_int.h"
#ضمن "hal/crosscore_int_ll.h"
#ضمن "esp_attr.h"
#ضمن "esp_compiler.h"
#ضمن "esp_system.h"
#ضمن "esp_intr_alloc.h"
#ضمن "esp_log.h"
#ضمن "FreeRTOS.h"       /* This pulls in portmacro.h */
#ضمن "task.h"
#إذا CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST
#ضمن "freertos/idf_additions.h"
#جے_انت
#ضمن "portmacro.h"
#ضمن "port_systick.h"
#ضمن "esp_memory_utils.h"
#ضمن "esp_macros.h"
#إذا CONFIG_FREERTOS_لگاتار_ویلا_STATS_USING_ESP_TIMER
#ضمن "esp_timer.h"
#جے_انت

#إذا SOC_CPU_HAS_HWLOOP
#ضمن "riscv/csr.h"
#ضمن "riscv/csr_hwlp.h"
#جے_انت

#إذا ( SOC_CPU_COPROC_عدد > 0 )

#ضمن "esp_private/panic_internal.h"

/* Since `portFORCE_INLINE` is not defined in `portmacro.h`, we must define it here since it is
 * used by `atomic.h`. */
#تعریف_کرو پورٹ_FORCE_INLINE    مضمن
#ضمن "freertos/atomic.h"

#جے_انت // ( SOC_CPU_COPROC_NUM > 0 )


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

متطاير UBaseType_t پورٹ_x_Scheduler_Running[پورٹ_عدد_PROCESSORS] = {0}; // Indicates whether scheduler is running on a per-core basis
متطاير UBaseType_t پورٹ_ux_رکاوٹ_Nesting[پورٹ_عدد_PROCESSORS] = {0};  // Interrupt nesting level. Increased/decreased in portasm.c
متطاير UBaseType_t پورٹ_ux_Critical_Nesting[پورٹ_عدد_PROCESSORS] = {0};
#إذا CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
متطاير bool پورٹ_x_Thread_محفوظ_Claimed = false;
#جے_انت
متطاير UBaseType_t پورٹ_ux_Old_رکاوٹ_State[پورٹ_عدد_PROCESSORS] = {0};
متطاير UBaseType_t x_پورٹ_Switch_Flag[پورٹ_عدد_PROCESSORS] = {0};
متطاير UBaseType_t پورٹ_ux_Core_Startup_پورا[پورٹ_عدد_PROCESSORS] = {0};  // Indicates whether the core has completed its startup sequence

#إذا ( SOC_CPU_COPROC_عدد > 0 )

/* Current owner of the coprocessors for each core */
StaticTask_t* پورٹ_ux_Coproc_Owner[پورٹ_عدد_PROCESSORS][SOC_CPU_COPROC_عدد];

#جے_انت /* SOC_CPU_COPROC_NUM > 0 */

/*
*******************************************************************************
* Interrupt stack. The size of the interrupt stack is determined by the config
* parameter "configISR_STACK_SIZE" in FreeRTOSConfig.h
*******************************************************************************
*/
__attribute__((aligned(16))) ڈھیر_Type_t x_Isr_ڈھیر[پورٹ_عدد_PROCESSORS][config_ISR_ڈھیر_ناپ];
ڈھیر_Type_t *x_Isr_ڈھیر_چوٹی[پورٹ_عدد_PROCESSORS] = {0};

#إذا CONFIG_ESP_SYSTEM_HW_ڈھیر_GUARD
ڈھیر_Type_t *x_Isr_ڈھیر_Bottom[پورٹ_عدد_PROCESSORS] = {0};
#جے_انت

/* ------------------------------------------------ FreeRTOS Portable --------------------------------------------------
 * - Provides implementation for functions required by FreeRTOS
 * - Declared in portable.h
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------- Scheduler Start/End -------------------

ادھار_Type_t x_پورٹ_شروع_Scheduler(فراغ)
{
#إذا ( SOC_CPU_COPROC_عدد > 0 )

#إذا SOC_CPU_HAS_FPU
    /* Disable FPU so that the first task to use it will trigger an exception */
    rv_utils_disable_fpu();
#جے_انت /* SOC_CPU_HAS_FPU */

#إذا SOC_CPU_HAS_PIE
    /* Similarly, disable PIE */
    rv_utils_disable_pie();
#جے_انت /* SOC_CPU_HAS_PIE */

#إذا SOC_CPU_HAS_DSP
    rv_utils_disable_dsp();
#جے_انت /* SOC_CPU_HAS_DSP */

#إذا SOC_CPU_HAS_HWLOOP
    /* Initialize the Hardware loop feature */
    RV_WRITE_CSR(CSR_HWLP_STATE_REG, HWLP_INITIAL_STATE);
#جے_انت /* SOC_CPU_HAS_HWLOOP */
#جے_انت /* ( SOC_CPU_COPROC_NUM > 0 ) */

    /* Initialize all kernel state tracking variables */
    ادھار_Type_t coreID = x_پورٹ_Get_Core_ID();
    پورٹ_ux_رکاوٹ_Nesting[coreID] = 0;
    پورٹ_ux_Critical_Nesting[coreID] = 0;
#إذا CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
    پورٹ_x_Thread_محفوظ_Claimed = false;
#جے_انت
    پورٹ_x_Scheduler_Running[coreID] = 0;
    پورٹ_ux_Core_Startup_پورا[coreID] = 0;

    /* Initialize ISR Stack(s) */
    لكل (صحيح i = 0; i < پورٹ_عدد_PROCESSORS; i++) {
        x_Isr_ڈھیر_چوٹی[i] = &x_Isr_ڈھیر[i][0] + (config_ISR_ڈھیر_ناپ & (~((پورٹ_اشارہ_گر_ناپ_TYPE)پورٹ_BYTE_ALIGNMENT_نقاب)));
#إذا CONFIG_ESP_SYSTEM_HW_ڈھیر_GUARD
        x_Isr_ڈھیر_Bottom[i] = &x_Isr_ڈھیر[i][0];
#جے_انت
    }

    /* Setup the hardware to generate the tick. */
    v_پورٹ_تیاری_Timer();

    rv_utils_intr_global_enable();
    esprv_int_سیٹ_حد(RVHAL_INTR_ENABLE_THRESH); /* set global interrupt masking level */

    v_پورٹ_Yield();

    /* Should not get here */
    أرجع pdTRUE;
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


#إذا ( SOC_CPU_COPROC_عدد > 0 )

/**
 * @brief Retrieve or allocate coprocessors save area from the given pxTopOfStack address.
 *
 * @param pxTopOfStack End of the stack address. This represents the highest address of a Task's stack.
 */
FORCE_INLINE_ATTR RvCoprocSaveArea* px_Retrieve_Coproc_Save_Area_From_ڈھیر_اشارہ_گر(UBaseType_t px_چوٹی_دا_ڈھیر)
{
    أرجع (RvCoprocSaveArea*) ESP_ALIGN_تھلے(px_چوٹی_دا_ڈھیر - حجم(RvCoprocSaveArea), 16);
}

/**
 * @brief Allocate and initialize the coprocessors save area on the stack
 *
 * @param[in] uxStackPointer Current stack pointer address
 *
 * @return Stack pointer that points to allocated and initialized the coprocessor save area
 */
FORCE_INLINE_ATTR UBaseType_t uxInitialiseCoprocSaveArea(UBaseType_t ux_ڈھیر_اشارہ_گر)
{
    RvCoprocSaveArea* sa = px_Retrieve_Coproc_Save_Area_From_ڈھیر_اشارہ_گر(ux_ڈھیر_اشارہ_گر);
    memset(sa, 0, حجم(RvCoprocSaveArea));
    أرجع (UBaseType_t) sa;
}


ساكن فراغ v_پورٹ_Clean_اتے_Coproc_Area(فراغ *pvTCB)
{
    StaticTask_t* task = (StaticTask_t*) pvTCB;

    /* Get a pointer to the task's coprocessor save area */
    ثابت UBaseType_t bottomstack = (UBaseType_t) task->pxDummy8;
    RvCoprocSaveArea* sa = px_Retrieve_Coproc_Save_Area_From_ڈھیر_اشارہ_گر(bottomstack);

    /* If the Task ever saved the original stack pointer, restore it before returning */
    إذا (sa->sa_allocator != 0) {
        /* Restore the original lowest address of the stack in the TCB */
        task->pxDummy6 = sa->sa_tcbstack;

        /* Get the core the task is pinned on */
        #إذا ( config_عدد_CORES > 1 )
            ثابت ادھار_Type_t coreID = task->xDummyCoreID;
            /* If the task is not pinned on any core, it didn't use any coprocessor than need to be freed (FPU or PIE).
             * If it used the HWLP coprocessor, it has nothing to clear since there is no "owner" for it. */
            إذا (coreID == tskNO_AFFINITY) {
                أرجع;
            }
        #وإلا /* configNUM_CORES > 1 */
            ثابت ادھار_Type_t coreID = 0;
        #جے_انت /* configNUM_CORES > 1 */

        لكل (صحيح i = 0; i < SOC_CPU_COPROC_عدد; i++) {
            StaticTask_t** owner = &پورٹ_ux_Coproc_Owner[coreID][i];
            /* If the owner is `task`, replace it with NULL atomically */
            Atomic_Compare_And_وٹاندرا_سوچکاں_p32((فراغ**) owner, عدم, task);
        }
    }
}
#جے_انت /* SOC_CPU_COPROC_NUM > 0 */


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
    - We use RV_STK_FRMSZ as it rounds up the total size to a multiple of 16.
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

    In the case of targets that have coprocessors, the stack is presented as follows:
    HIGH ADDRESS
    |---------------------------| <- pxTopOfStack on entry
    | Coproc. Save Area         | <- RvCoprocSaveArea
    | ------------------------- |
    | TLS Variables             |
    | ------------------------- | <- Start of usable stack
    | Starting stack frame      |
    | ------------------------- | <- pxTopOfStack on return (which is the tasks current SP)
    |             |             |
    |             |             |
    |             V             |
    |---------------------------|
    | Coproc. m Saved Context   | <- Coprocessor context save area after allocation
    |---------------------------|
    | Coproc. n Saved Context   | <- Another coprocessor context save area after allocation
    ----------------------------- <- Bottom of stack
    LOW ADDRESS

    Where m != n, n < SOC_CPU_COPROC_NUM, m < SOC_CPU_COPROC_NUM

    */

    UBaseType_t ux_ڈھیر_اشارہ_گر = (UBaseType_t)px_چوٹی_دا_ڈھیر;
    configASSERT((ux_ڈھیر_اشارہ_گر & پورٹ_BYTE_ALIGNMENT_نقاب) == 0);

#إذا ( SOC_CPU_COPROC_عدد > 0 )
    // Initialize the coprocessors save area
    ux_ڈھیر_اشارہ_گر = uxInitialiseCoprocSaveArea(ux_ڈھیر_اشارہ_گر);
    configASSERT((ux_ڈھیر_اشارہ_گر & پورٹ_BYTE_ALIGNMENT_نقاب) == 0);
#جے_انت // SOC_CPU_COPROC_NUM > 0

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



/* ---------------------------------------------- Port Implementations -------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

ادھار_Type_t x_پورٹ_وچ_Isr_Context(فراغ)
{
#إذا (config_عدد_CORES > 1)
    بنانشانی صحيح irq_حالت;
    ادھار_Type_t ret;

    /* Disable interrupts to fetch the coreID atomically */
    irq_حالت = پورٹ_سیٹ_رکاوٹ_نقاب_FROM_ISR();

    /* Return the interrupt nesting counter for this core */
    ret = پورٹ_ux_رکاوٹ_Nesting[x_پورٹ_Get_Core_ID()];

    /* Restore interrupts */
    پورٹ_CLEAR_رکاوٹ_نقاب_FROM_ISR(irq_حالت);

    أرجع ret;
#وإلا
    /* Optimize the call for single-core targets */
    أرجع پورٹ_ux_رکاوٹ_Nesting[0];
#جے_انت /* (configNUM_CORES > 1) */
}

فراغ v_پورٹ_Assert_If_وچ_ISR(فراغ)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(x_پورٹ_وچ_Isr_Context() == 0);
}

ادھار_Type_t x_پورٹ_Interrupted_From_ISRContext(فراغ)
{
    /* Return the interrupt nesting counter for this core */
    أرجع پورٹ_ux_رکاوٹ_Nesting[x_پورٹ_Get_Core_ID()];
}

UBaseType_t x_پورٹ_سیٹ_رکاوٹ_نقاب_From_ISR(فراغ)
{
    UBaseType_t prev_int_level = 0, int_level = 0;
#إذا !SOC_INT_CLIC_SUPPORTED
    int_level = RVHAL_EXCM_LEVEL;
#وإلا
    int_level = RVHAL_EXCM_LEVEL_CLIC;
#جے_انت

    prev_int_level = rv_utils_سیٹ_intlevel_regval(int_level);
    أرجع prev_int_level;
}

فراغ v_پورٹ_Clear_رکاوٹ_نقاب_From_ISR(UBaseType_t prev_int_level)
{
    rv_utils_restore_intlevel_regval(prev_int_level);
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

// ------------------ Critical Sections --------------------

#إذا (config_عدد_CORES > 1)
ادھار_Type_t __attribute__((optimize("-O3"))) x_پورٹ_Enter_Critical_Timeout(پورٹ_MUX_TYPE *mux, ادھار_Type_t timeout)
{
#إذا CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
    إذا (unlikely(پورٹ_x_Thread_محفوظ_Claimed)) {
        أرجع pdPASS;
    }
#جے_انت
    /* Interrupts may already be disabled (if this function is called in nested
     * manner). However, there's no atomic operation that will allow us to check,
     * thus we have to disable interrupts again anyways.
     *
     * However, if this is call is NOT nested (i.e., the first call to enter a
     * critical section), we will save the previous interrupt level so that the
     * saved level can be restored on the last call to exit the critical.
     */
    ادھار_Type_t x_Old_رکاوٹ_Level = پورٹ_سیٹ_رکاوٹ_نقاب_FROM_ISR();
    /* Interrupts are masked, so the core id is stable. Read it once and reuse it for
     * both the spinlock owner id and the per-core critical nesting state. Interrupts
     * stay masked for the whole critical section (to the same level the spinlock would
     * use), so the spinlock does not need to disable them again. */
    ادھار_Type_t coreID = x_پورٹ_Get_Core_ID();
    إذا (!spinlock_acquire_impl(mux, timeout, spinlock_owner_id_for_core(coreID))) {
        //Timed out attempting to get spinlock. Restore previous interrupt level and return
        پورٹ_CLEAR_رکاوٹ_نقاب_FROM_ISR(x_Old_رکاوٹ_Level);
        أرجع pdFAIL;
    }
    //Spinlock acquired. Increment the critical nesting count.
    ادھار_Type_t newNesting = پورٹ_ux_Critical_Nesting[coreID] + 1;
    پورٹ_ux_Critical_Nesting[coreID] = newNesting;
    //If this is the first entry to a critical section. Save the old interrupt level.
    إذا ( newNesting == 1 ) {
        پورٹ_ux_Old_رکاوٹ_State[coreID] = x_Old_رکاوٹ_Level;
    }
    أرجع pdPASS;
}

فراغ __attribute__((optimize("-O3"))) v_پورٹ_Exit_Critical_Multi_Core(پورٹ_MUX_TYPE *mux)
{
#إذا CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
    إذا (unlikely(پورٹ_x_Thread_محفوظ_Claimed)) {
        أرجع;
    }
#جے_انت
    /* This function may be called in a nested manner. Therefore, we only need
     * to re-enable interrupts if this is the last call to exit the critical. We
     * can use the nesting count to determine whether this is the last exit call.
     */
    /* Interrupts remain disabled for the whole critical section, so the core id is
     * stable and the spinlock does not need to mask them again. Read the core id
     * once and reuse it for both the release owner id and the per-core nesting state. */
    ادھار_Type_t coreID = x_پورٹ_Get_Core_ID();
    spinlock_release_impl(mux, spinlock_owner_id_for_core(coreID));
    ادھار_Type_t nesting = پورٹ_ux_Critical_Nesting[coreID];

    /* Critical section nesting count must never be negative */
    configASSERT( nesting > 0 );

    إذا (nesting > 0) {
        nesting--;
        پورٹ_ux_Critical_Nesting[coreID] = nesting;

        //This is the last exit call, restore the saved interrupt level
        إذا ( nesting == 0 ) {
            پورٹ_CLEAR_رکاوٹ_نقاب_FROM_ISR(پورٹ_ux_Old_رکاوٹ_State[coreID]);
        }
    }
}


ادھار_Type_t x_پورٹ_Enter_Critical_Timeout_Compliance(پورٹ_MUX_TYPE *mux, ادھار_Type_t timeout)
{
    ادھار_Type_t ret;
    إذا (!x_پورٹ_وچ_Isr_Context()) {
        ret = x_پورٹ_Enter_Critical_Timeout(mux, timeout);
    } وإلا {
        esp_rom_printf("port*_CRITICAL called from ISR context. Aborting!\n");
        abort();
        ret = pdFAIL;
    }
    أرجع ret;
}

فراغ v_پورٹ_Exit_Critical_Compliance(پورٹ_MUX_TYPE *mux)
{
    إذا (!x_پورٹ_وچ_Isr_Context()) {
        v_پورٹ_Exit_Critical_Multi_Core(mux);
    } وإلا {
        esp_rom_printf("port*_CRITICAL called from ISR context. Aborting!\n");
        abort();
    }
}
#جے_انت /* (configNUM_CORES > 1) */

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
#إذا (config_عدد_CORES > 1)
        esp_rom_printf("vPortEnterCritical(void) is not supported on multi-core targets. Please use vPortEnterCriticalMultiCore(portMUX_TYPE *mux) instead.\n");
        abort();
#جے_انت /* (configNUM_CORES > 1) */
#إذا CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
    إذا (unlikely(پورٹ_x_Thread_محفوظ_Claimed)) {
        أرجع;
    }
#جے_انت
    ادھار_Type_t state = پورٹ_سیٹ_رکاوٹ_نقاب_FROM_ISR();
    پورٹ_ux_Critical_Nesting[0]++;

    إذا (پورٹ_ux_Critical_Nesting[0] == 1) {
        پورٹ_ux_Old_رکاوٹ_State[0] = state;
    }
}

فراغ v_پورٹ_Exit_Critical(فراغ)
{
#إذا (config_عدد_CORES > 1)
        esp_rom_printf("vPortExitCritical(void) is not supported on multi-core targets. Please use vPortExitCriticalMultiCore(portMUX_TYPE *mux) instead.\n");
        abort();
#جے_انت /* (configNUM_CORES > 1) */
#إذا CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
    إذا (unlikely(پورٹ_x_Thread_محفوظ_Claimed)) {
        أرجع;
    }
#جے_انت

    /* Critical section nesting count must never be negative */
    configASSERT( پورٹ_ux_Critical_Nesting[0] > 0 );

    إذا (پورٹ_ux_Critical_Nesting[0] > 0) {
        پورٹ_ux_Critical_Nesting[0]--;

        إذا (پورٹ_ux_Critical_Nesting[0] == 0) {
            پورٹ_CLEAR_رکاوٹ_نقاب_FROM_ISR(پورٹ_ux_Old_رکاوٹ_State[0]);
        }
    }
}

// ---------------------- Yielding -------------------------

فراغ v_پورٹ_Yield(فراغ)
{
    ادھار_Type_t coreID = x_پورٹ_Get_Core_ID();

    إذا (پورٹ_ux_رکاوٹ_Nesting[coreID]) {
        v_پورٹ_Yield_From_ISR();
    } وإلا {
        esp_crosscore_int_send_yield(coreID);
        /* There are 3-4 instructions of latency between triggering the software
           interrupt and the CPU interrupt happening. Make sure it happened before
           we return, otherwise vTaskDelay() may return and execute 1-2
           instructions before the delay actually happens.

           (We could use the WFI instruction here, but there is a chance that
           the interrupt will happen while evaluating the other two conditions
           for an instant yield, and if that happens then the WFI would be
           waiting for the next interrupt to occur...)
        */
        طالما (پورٹ_x_Scheduler_Running[coreID] && پورٹ_ux_Critical_Nesting[coreID] == 0 && crosscore_int_ll_get_state(coreID) != 0) {}
    }
}

فراغ v_پورٹ_Yield_From_ISR( فراغ )
{
    trace_ISR_EXIT_منزل_SCHEDULER();
    ادھار_Type_t coreID = x_پورٹ_Get_Core_ID();
    پورٹ_x_Scheduler_Running[coreID] = 1;
    x_پورٹ_Switch_Flag[coreID] = 1;
}

فراغ v_پورٹ_Yield_Other_Core(ادھار_Type_t coreid)
{
    esp_crosscore_int_send_yield(coreid);
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

// ----------------------- System --------------------------

uint32_t x_پورٹ_Get_Tick_شرح_Hz(فراغ)
{
    أرجع (uint32_t)config_TICK_شرح_HZ;
}

#تعریف_کرو ڈھیر_WATCH_AREA_ناپ 32
#تعریف_کرو ڈھیر_WATCH_نقطہ_نمبر (SOC_CPU_WATCHPOINTS_عدد - 1)

فراغ v_پورٹ_سیٹ_ڈھیر_Watchpoint(فراغ *px_ڈھیر_شروع)
{
    uint32_t addr = (uint32_t)px_ڈھیر_شروع;
    addr = (addr + (ڈھیر_WATCH_AREA_ناپ - 1)) & (~(ڈھیر_WATCH_AREA_ناپ - 1));
    esp_cpu_سیٹ_watchpoint(ڈھیر_WATCH_نقطہ_نمبر, (حرف *)addr, ڈھیر_WATCH_AREA_ناپ, ESP_CPU_WATCHPOINT_STORE);
}

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

    #إذا ( CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST )
        إذا (pxTCB != عدم) {
            v_Task_سیٹ_Pie_Blacklisted((TaskHandle_t)pxTCB, pdFALSE);
        }
    #جے_انت /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */

    #إذا ( SOC_CPU_COPROC_عدد > 0 )
        /* Cleanup coproc save area */
        v_پورٹ_Clean_اتے_Coproc_Area( pxTCB );
    #جے_انت /* SOC_CPU_COPROC_NUM > 0 */
}


#إذا ( SOC_CPU_COPROC_عدد > 0 )

// ----------------------- Coprocessors --------------------------

/**
 * @brief Pin the given task to the given core
 *
 * This function is called when a task uses a coprocessor. Since the coprocessors registers
 * are saved lazily, as soon as a task starts using one, it must always be scheduled on the core
 * it is currently executing on.
 */
#إذا ( config_عدد_CORES > 1 )
فراغ v_پورٹ_Task_Pin_منزل_Core(StaticTask_t* task, صحيح coreid)
{
    task->xDummyCoreID = coreid;
}
#جے_انت /* configNUM_CORES > 1 */


/**
 * @brief Function to call to simulate an `abort()` occurring in a different context than the one it's called from.
 */
خارجي فراغ xt_unhandled_exception(فراغ *چوکھٹا);

/**
 * @brief Get coprocessor save area out of the given task. If the coprocessor area is not created,
 *        it shall be allocated.
 *
 * @param task Task to get the coprocessor save area of
 * @param allocate When true, memory will be allocated for the coprocessor if it hasn't been allocated yet.
 *                 When false, the coprocessor memory will be left as NULL if not allocated.
 * @param coproc Coprocessor number to allocate memory for
 */
RvCoprocSaveArea* px_پورٹ_Get_Coproc_Area(StaticTask_t* task, bool allocate, صحيح coproc)
{
    أكد(coproc < SOC_CPU_COPROC_عدد);

    ثابت UBaseType_t bottomstack = (UBaseType_t) task->pxDummy8;
    RvCoprocSaveArea* sa = px_Retrieve_Coproc_Save_Area_From_ڈھیر_اشارہ_گر(bottomstack);
    /* Check if the allocator is NULL. Since we don't have a way to get the end of the stack
     * during its initialization, we have to do this here */
    إذا (sa->sa_allocator == 0) {
        /* Since the lowest stack address shall not be used as `sp` anymore, we will modify it */
        sa->sa_tcbstack = task->pxDummy6;
        sa->sa_allocator = (UBaseType_t) task->pxDummy6;
    }

    /* Check if coprocessor area is allocated */
    إذا (allocate && sa->sa_coprocs[coproc] == عدم) {
        ثابت uint32_t coproc_sa_sizes[] = {
            RV_COPROC0_ناپ, RV_COPROC1_ناپ, RV_COPROC2_ناپ
        };
        ثابت uint32_t coproc_sa_align[] = {
            RV_COPROC0_ALIGN, RV_COPROC1_ALIGN, RV_COPROC2_ALIGN
        };
        /* The allocator points to a usable part of the stack, use it for the coprocessor.
         * Align it up to the coprocessor save area requirement */
        sa->sa_allocator = (sa->sa_allocator + coproc_sa_align[coproc] - 1) & ~(coproc_sa_align[coproc] - 1);
        sa->sa_coprocs[coproc] = (فراغ*) (sa->sa_allocator);
        sa->sa_allocator += coproc_sa_sizes[coproc];
        /* Update the lowest address of the stack to prevent FreeRTOS performing overflow/watermark checks on the coprocessors contexts */
        task->pxDummy6 = (فراغ*) (sa->sa_allocator);
        /* Make sure the Task stack pointer is not pointing to the coprocessor context area, in other words, make
         * sure we don't have a stack overflow */
        فراغ* task_sp = task->pxDummy1;
        إذا (task_sp <= task->pxDummy6) {
            /* In theory we need to call vApplicationStackOverflowHook to trigger the stack overflow callback,
             * but in practice, since we are already in an exception handler, this won't work, so let's manually
             * trigger an exception with the previous coprocessor owner's TCB */
            g_panic_abort = true;
            g_panic_abort_details = (حرف *) "ERROR: Stack overflow while saving coprocessor context!\n";
            xt_unhandled_exception(task_sp);
        }
    }
    أرجع sa;
}


/**
 * @brief Update given coprocessor owner and get the address of former owner's save area.
 *
 * This function is called when the current running task has poked a coprocessor's register which
 * was used by a previous task. We have to save the coprocessor context (registers) inside the
 * current owner's save area and change the ownership. The coprocessor will be marked as used in
 * the new owner's coprocessor save area.
 *
 * @param coreid    Current core
 * @param coproc    Coprocessor to save context of
 * @param owner     New owner of the coprocessor. Can be NULL to clear the owner.
 *
 * @returns Coprocessor former owner's save area, can be NULL if there was no owner yet, can be -1 if
 *          the former owner is the same as the new owner.
 */
RvCoprocSaveArea* px_پورٹ_Update_Coproc_Owner(صحيح coreid, صحيح coproc, StaticTask_t* owner)
{
    RvCoprocSaveArea* sa = عدم;
    /* Address of coprocessor owner */
    StaticTask_t** owner_addr = &پورٹ_ux_Coproc_Owner[ coreid ][ coproc ];
    /* Atomically exchange former owner with the new one */
    StaticTask_t* former = Atomic_وٹاندرا_سوچکاں_p32((فراغ**) owner_addr, owner);
    /* Get the save area of former owner. small optimization here, if the former owner is the new owner,
     * return -1. This will simplify the assembly code while making it faster. */
    إذا (former == owner) {
        sa = (فراغ*) -1;
    } وإلا إذا (former != عدم) {
        /* Allocate coprocessor memory if not available yet */
        sa = px_پورٹ_Get_Coproc_Area(former, true, coproc);
    }
    أرجع sa;
}


/**
 * @brief Aborts execution when a coprocessor was used in an ISR context
 */
فراغ v_پورٹ_Coproc_ورتیا_وچ_ISR(فراغ* چوکھٹا)
{
    /* Since this function is called from an exception handler, the interrupts are disabled,
     * as such, it is not possible to trigger another exception as would `abort` do.
     * Simulate an abort without actually triggering an exception. */
    g_panic_abort = true;
    g_panic_abort_details = (حرف *) "ERROR: Coprocessors must not be used in ISRs!\n";
    xt_unhandled_exception(چوکھٹا);
}

#إذا CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST
/**
 * @brief Abort if the given task is blacklisted from using the PIE coprocessor.
 *
 * Called from the PIE illegal-instruction path, so this must not trigger another
 * exception via abort().
 */
فراغ v_پورٹ_Abort_If_Task_Pie_Blacklisted(StaticTask_t *task, فراغ *چوکھٹا)
{
    إذا (task != عدم && xTaskGetPieBlacklisted((TaskHandle_t)task) != pdFALSE) {
        g_panic_abort = true;
        g_panic_abort_details = (حرف *) "ERROR: Blacklisted task executed a PIE instruction!\n";
        xt_unhandled_exception(چوکھٹا);
    }
}
#جے_انت /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */

#إذا CONFIG_IDF_نشانہ_ESP32_S31
/* On the ESP32-S31, the PIE is only available on core 1, so we need to perform a few checks when core 0 uses a PIE instruction.
 * the functions here will help us with that. */

/**
 * @brief Called when a task uses a PIE instruction on core 0.
 *
 * @param task Task that used the PIE instruction
 * @param frame Frame of the PIE instruction
 *
 * @returns The context to save with the current FPU context when the current task was the FPU owner,
 *          NULL if the task is not the owner of the FPU on the current core.
 */
فراغ* v_پورٹ_Task_ورتیا_PIEOn_CPU0(StaticTask_t* task, فراغ* چوکھٹا)
{
#إذا CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST
    v_پورٹ_Abort_If_Task_Pie_Blacklisted(task, چوکھٹا);
#جے_انت
#إذا CONFIG_FREERTOS_UNICORE
    g_panic_abort = true;
    g_panic_abort_details = (حرف *) "ERROR: PIE coprocessor is not supported in unicore configuration!\n";
    xt_unhandled_exception(چوکھٹا);
    أرجع عدم;
#وإلا
    فراغ* context_منزل_save = عدم;
    /* Make sure we are not in an interrupt context nor in a critical section */
    إذا (x_پورٹ_وچ_Isr_Context()) {
        /* We are in an interrupt context, abort */
        v_پورٹ_Coproc_ورتیا_وچ_ISR(چوکھٹا);
    }
    /* Since we count on crosscore interrupt to reschedule the current task, we must not be in a
     * critical section. In other words, we must be able to yield */
    إذا (!x_پورٹ_Can_Yield()) {
        g_panic_abort = true;
        g_panic_abort_details = (حرف *) "ERROR: PIE coprocessor must not be used in critical sections!\n";
        xt_unhandled_exception(چوکھٹا);
    }

    /* Check if the current task is the owner of the FPU on the current core. No need to make the following two instructions
     * atomic since we are in an exception context, we can't be interrupted by an interrupt. */
    إذا (پورٹ_ux_Coproc_Owner[0][FPU_COPROC_IDX] == task) {
        /* Task is the owner of the FPU on the current core, set the new owner to NULL and return the save area to fill */
        RvCoprocSaveArea* sa = px_پورٹ_Update_Coproc_Owner(0, FPU_COPROC_IDX, عدم);
        /* `sa` is not NULL here for sure */
        context_منزل_save = sa->sa_coprocs[FPU_COPROC_IDX];
    }
    /* Migrate the task to core 1. NOTE: This will override any existing pinning of the task */
    v_پورٹ_Task_Pin_منزل_Core(task, 1);
    /* Raised an error if the scheduler is NOT running on core 0 */
    إذا (!پورٹ_x_Scheduler_Running[0]) {
        /* Scheduler is not running on core 0, raise an error */
        g_panic_abort = true;
        g_panic_abort_details = (حرف *) "ERROR: Scheduler is not running on core 0, task must migrate to core 1!\n";
        xt_unhandled_exception(چوکھٹا);
    }
    /* Send a cross-core interrupt on the current core, it won't be triggered until we return from the exception handler */
    esp_crosscore_int_send_yield(0);
    أرجع context_منزل_save;
#جے_انت /* CONFIG_FREERTOS_UNICORE */
}

#جے_انت /* CONFIG_IDF_TARGET_ESP32S31 */


#جے_انت /* SOC_CPU_COPROC_NUM > 0 */

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

/* ---------------------------------------------- Misc Implementations -------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */
#إذا (SOC_CPU_COPROC_عدد > 0) && SOC_CPU_HAS_FPU && SOC_PM_FPU_RETENTION_BY_SW
ادھار_Type_t x_پورٹ_FPUContext_Is_Dirty(ادھار_Type_t core_id)
{
    أرجع (ادھار_Type_t)(پورٹ_ux_Coproc_Owner[core_id][FPU_COPROC_IDX] != عدم);
}
#جے_انت /* (SOC_CPU_COPROC_NUM > 0) && SOC_CPU_HAS_FPU && SOC_PM_FPU_RETENTION_BY_SW */
