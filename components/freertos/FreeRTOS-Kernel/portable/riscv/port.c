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

#شامل "sdkconfig.h"
#شامل <stdbool.h>
#شامل <string.h>
#شامل "soc/soc_caps.h"
#شامل "soc/periph_defs.h"
#شامل "soc/system_reg.h"
#شامل "soc/interrupt_reg.h"
#شامل "hal/systimer_hal.h"
#شامل "hal/systimer_ll.h"
#شامل "riscv/rvruntime-frames.h"
#شامل "riscv/rv_utils.h"
#شامل "riscv/interrupt.h"
#شامل "esp_private/crosscore_int.h"
#شامل "hal/crosscore_int_ll.h"
#شامل "esp_attr.h"
#شامل "esp_compiler.h"
#شامل "esp_system.h"
#شامل "esp_intr_alloc.h"
#شامل "esp_log.h"
#شامل "FreeRTOS.h"       /* This pulls in portmacro.h */
#شامل "task.h"
#اگر CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST
#شامل "freertos/idf_additions.h"
#اگر_ختم
#شامل "portmacro.h"
#شامل "port_systick.h"
#شامل "esp_memory_utils.h"
#شامل "esp_macros.h"
#اگر CONFIG_FREERTOS_تسلسل_وقت_STATS_USING_ESP_TIMER
#شامل "esp_timer.h"
#اگر_ختم

#اگر SOC_CPU_HAS_HWLOOP
#شامل "riscv/csr.h"
#شامل "riscv/csr_hwlp.h"
#اگر_ختم

#اگر ( SOC_CPU_COPROC_عدد > 0 )

#شامل "esp_private/panic_internal.h"

/* Since `portFORCE_INLINE` is not defined in `portmacro.h`, we must define it here since it is
 * used by `atomic.h`. */
#تعریف_کرو پورٹ_FORCE_INLINE    اندرونی
#شامل "freertos/atomic.h"

#اگر_ختم // ( SOC_CPU_COPROC_NUM > 0 )


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

متلون UBaseType_t پورٹ_x_Scheduler_Running[پورٹ_عدد_PROCESSORS] = {0}; // Indicates whether scheduler is running on a per-core basis
متلون UBaseType_t پورٹ_ux_مداخلت_Nesting[پورٹ_عدد_PROCESSORS] = {0};  // Interrupt nesting level. Increased/decreased in portasm.c
متلون UBaseType_t پورٹ_ux_Critical_Nesting[پورٹ_عدد_PROCESSORS] = {0};
#اگر CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
متلون bool پورٹ_x_Thread_محفوظ_Claimed = false;
#اگر_ختم
متلون UBaseType_t پورٹ_ux_Old_مداخلت_State[پورٹ_عدد_PROCESSORS] = {0};
متلون UBaseType_t x_پورٹ_Switch_Flag[پورٹ_عدد_PROCESSORS] = {0};
متلون UBaseType_t پورٹ_ux_Core_Startup_مکمل[پورٹ_عدد_PROCESSORS] = {0};  // Indicates whether the core has completed its startup sequence

#اگر ( SOC_CPU_COPROC_عدد > 0 )

/* Current owner of the coprocessors for each core */
StaticTask_t* پورٹ_ux_Coproc_Owner[پورٹ_عدد_PROCESSORS][SOC_CPU_COPROC_عدد];

#اگر_ختم /* SOC_CPU_COPROC_NUM > 0 */

/*
*******************************************************************************
* Interrupt stack. The size of the interrupt stack is determined by the config
* parameter "configISR_STACK_SIZE" in FreeRTOSConfig.h
*******************************************************************************
*/
__attribute__((aligned(16))) ڈھیر_Type_t x_Isr_ڈھیر[پورٹ_عدد_PROCESSORS][config_ISR_ڈھیر_سائز];
ڈھیر_Type_t *x_Isr_ڈھیر_چوٹی[پورٹ_عدد_PROCESSORS] = {0};

#اگر CONFIG_ESP_SYSTEM_HW_ڈھیر_GUARD
ڈھیر_Type_t *x_Isr_ڈھیر_Bottom[پورٹ_عدد_PROCESSORS] = {0};
#اگر_ختم

/* ------------------------------------------------ FreeRTOS Portable --------------------------------------------------
 * - Provides implementation for functions required by FreeRTOS
 * - Declared in portable.h
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------- Scheduler Start/End -------------------

بنیاد_Type_t x_پورٹ_آغاز_Scheduler(خالی)
{
#اگر ( SOC_CPU_COPROC_عدد > 0 )

#اگر SOC_CPU_HAS_FPU
    /* Disable FPU so that the first task to use it will trigger an exception */
    rv_utils_disable_fpu();
#اگر_ختم /* SOC_CPU_HAS_FPU */

#اگر SOC_CPU_HAS_PIE
    /* Similarly, disable PIE */
    rv_utils_disable_pie();
#اگر_ختم /* SOC_CPU_HAS_PIE */

#اگر SOC_CPU_HAS_DSP
    rv_utils_disable_dsp();
#اگر_ختم /* SOC_CPU_HAS_DSP */

#اگر SOC_CPU_HAS_HWLOOP
    /* Initialize the Hardware loop feature */
    RV_WRITE_CSR(CSR_HWLP_STATE_REG, HWLP_INITIAL_STATE);
#اگر_ختم /* SOC_CPU_HAS_HWLOOP */
#اگر_ختم /* ( SOC_CPU_COPROC_NUM > 0 ) */

    /* Initialize all kernel state tracking variables */
    بنیاد_Type_t coreID = x_پورٹ_Get_Core_ID();
    پورٹ_ux_مداخلت_Nesting[coreID] = 0;
    پورٹ_ux_Critical_Nesting[coreID] = 0;
#اگر CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
    پورٹ_x_Thread_محفوظ_Claimed = false;
#اگر_ختم
    پورٹ_x_Scheduler_Running[coreID] = 0;
    پورٹ_ux_Core_Startup_مکمل[coreID] = 0;

    /* Initialize ISR Stack(s) */
    برائے (صحیح i = 0; i < پورٹ_عدد_PROCESSORS; i++) {
        x_Isr_ڈھیر_چوٹی[i] = &x_Isr_ڈھیر[i][0] + (config_ISR_ڈھیر_سائز & (~((پورٹ_اشارہ_گر_سائز_TYPE)پورٹ_BYTE_ALIGNMENT_نقاب)));
#اگر CONFIG_ESP_SYSTEM_HW_ڈھیر_GUARD
        x_Isr_ڈھیر_Bottom[i] = &x_Isr_ڈھیر[i][0];
#اگر_ختم
    }

    /* Setup the hardware to generate the tick. */
    v_پورٹ_تیاری_Timer();

    rv_utils_intr_global_enable();
    esprv_int_سیٹ_حد(RVHAL_INTR_ENABLE_THRESH); /* set global interrupt masking level */

    v_پورٹ_Yield();

    /* Should not get here */
    واپس pdTRUE;
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


#اگر ( SOC_CPU_COPROC_عدد > 0 )

/**
 * @brief Retrieve or allocate coprocessors save area from the given pxTopOfStack address.
 *
 * @param pxTopOfStack End of the stack address. This represents the highest address of a Task's stack.
 */
FORCE_INLINE_ATTR RvCoprocSaveArea* px_Retrieve_Coproc_Save_Area_From_ڈھیر_اشارہ_گر(UBaseType_t px_چوٹی_کا_ڈھیر)
{
    واپس (RvCoprocSaveArea*) ESP_ALIGN_نیچے_کو(px_چوٹی_کا_ڈھیر - حجم(RvCoprocSaveArea), 16);
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
    واپس (UBaseType_t) sa;
}


ساکن خالی v_پورٹ_Clean_اوپر_Coproc_Area(خالی *pvTCB)
{
    StaticTask_t* task = (StaticTask_t*) pvTCB;

    /* Get a pointer to the task's coprocessor save area */
    ثابت UBaseType_t bottomstack = (UBaseType_t) task->pxDummy8;
    RvCoprocSaveArea* sa = px_Retrieve_Coproc_Save_Area_From_ڈھیر_اشارہ_گر(bottomstack);

    /* If the Task ever saved the original stack pointer, restore it before returning */
    اگر (sa->sa_allocator != 0) {
        /* Restore the original lowest address of the stack in the TCB */
        task->pxDummy6 = sa->sa_tcbstack;

        /* Get the core the task is pinned on */
        #اگر ( config_عدد_CORES > 1 )
            ثابت بنیاد_Type_t coreID = task->xDummyCoreID;
            /* If the task is not pinned on any core, it didn't use any coprocessor than need to be freed (FPU or PIE).
             * If it used the HWLP coprocessor, it has nothing to clear since there is no "owner" for it. */
            اگر (coreID == tskNO_AFFINITY) {
                واپس;
            }
        #ورنہ /* configNUM_CORES > 1 */
            ثابت بنیاد_Type_t coreID = 0;
        #اگر_ختم /* configNUM_CORES > 1 */

        برائے (صحیح i = 0; i < SOC_CPU_COPROC_عدد; i++) {
            StaticTask_t** owner = &پورٹ_ux_Coproc_Owner[coreID][i];
            /* If the owner is `task`, replace it with NULL atomically */
            Atomic_Compare_And_تبادلہ_اشارے_p32((خالی**) owner, تہی, task);
        }
    }
}
#اگر_ختم /* SOC_CPU_COPROC_NUM > 0 */


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
    - We use RV_STK_FRMSZ as it rounds up the total size to a multiple of 16.
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

    UBaseType_t ux_ڈھیر_اشارہ_گر = (UBaseType_t)px_چوٹی_کا_ڈھیر;
    configASSERT((ux_ڈھیر_اشارہ_گر & پورٹ_BYTE_ALIGNMENT_نقاب) == 0);

#اگر ( SOC_CPU_COPROC_عدد > 0 )
    // Initialize the coprocessors save area
    ux_ڈھیر_اشارہ_گر = uxInitialiseCoprocSaveArea(ux_ڈھیر_اشارہ_گر);
    configASSERT((ux_ڈھیر_اشارہ_گر & پورٹ_BYTE_ALIGNMENT_نقاب) == 0);
#اگر_ختم // SOC_CPU_COPROC_NUM > 0

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



/* ---------------------------------------------- Port Implementations -------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

بنیاد_Type_t x_پورٹ_اندر_Isr_Context(خالی)
{
#اگر (config_عدد_CORES > 1)
    غیرعلامتی صحیح irq_حالت;
    بنیاد_Type_t ret;

    /* Disable interrupts to fetch the coreID atomically */
    irq_حالت = پورٹ_سیٹ_مداخلت_نقاب_FROM_ISR();

    /* Return the interrupt nesting counter for this core */
    ret = پورٹ_ux_مداخلت_Nesting[x_پورٹ_Get_Core_ID()];

    /* Restore interrupts */
    پورٹ_CLEAR_مداخلت_نقاب_FROM_ISR(irq_حالت);

    واپس ret;
#ورنہ
    /* Optimize the call for single-core targets */
    واپس پورٹ_ux_مداخلت_Nesting[0];
#اگر_ختم /* (configNUM_CORES > 1) */
}

خالی v_پورٹ_Assert_If_اندر_ISR(خالی)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(x_پورٹ_اندر_Isr_Context() == 0);
}

بنیاد_Type_t x_پورٹ_Interrupted_From_ISRContext(خالی)
{
    /* Return the interrupt nesting counter for this core */
    واپس پورٹ_ux_مداخلت_Nesting[x_پورٹ_Get_Core_ID()];
}

UBaseType_t x_پورٹ_سیٹ_مداخلت_نقاب_From_ISR(خالی)
{
    UBaseType_t پچھلا_int_level = 0, int_level = 0;
#اگر !SOC_INT_CLIC_SUPPORTED
    int_level = RVHAL_EXCM_LEVEL;
#ورنہ
    int_level = RVHAL_EXCM_LEVEL_CLIC;
#اگر_ختم

    پچھلا_int_level = rv_utils_سیٹ_intlevel_regval(int_level);
    واپس پچھلا_int_level;
}

خالی v_پورٹ_Clear_مداخلت_نقاب_From_ISR(UBaseType_t پچھلا_int_level)
{
    rv_utils_restore_intlevel_regval(پچھلا_int_level);
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

// ------------------ Critical Sections --------------------

#اگر (config_عدد_CORES > 1)
بنیاد_Type_t __attribute__((optimize("-O3"))) x_پورٹ_Enter_Critical_Timeout(پورٹ_MUX_TYPE *mux, بنیاد_Type_t timeout)
{
#اگر CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
    اگر (unlikely(پورٹ_x_Thread_محفوظ_Claimed)) {
        واپس pdPASS;
    }
#اگر_ختم
    /* Interrupts may already be disabled (if this function is called in nested
     * manner). However, there's no atomic operation that will allow us to check,
     * thus we have to disable interrupts again anyways.
     *
     * However, if this is call is NOT nested (i.e., the first call to enter a
     * critical section), we will save the previous interrupt level so that the
     * saved level can be restored on the last call to exit the critical.
     */
    بنیاد_Type_t x_Old_مداخلت_Level = پورٹ_سیٹ_مداخلت_نقاب_FROM_ISR();
    /* Interrupts are masked, so the core id is stable. Read it once and reuse it for
     * both the spinlock owner id and the per-core critical nesting state. Interrupts
     * stay masked for the whole critical section (to the same level the spinlock would
     * use), so the spinlock does not need to disable them again. */
    بنیاد_Type_t coreID = x_پورٹ_Get_Core_ID();
    اگر (!spinlock_acquire_impl(mux, timeout, spinlock_owner_id_for_core(coreID))) {
        //Timed out attempting to get spinlock. Restore previous interrupt level and return
        پورٹ_CLEAR_مداخلت_نقاب_FROM_ISR(x_Old_مداخلت_Level);
        واپس pdFAIL;
    }
    //Spinlock acquired. Increment the critical nesting count.
    بنیاد_Type_t newNesting = پورٹ_ux_Critical_Nesting[coreID] + 1;
    پورٹ_ux_Critical_Nesting[coreID] = newNesting;
    //If this is the first entry to a critical section. Save the old interrupt level.
    اگر ( newNesting == 1 ) {
        پورٹ_ux_Old_مداخلت_State[coreID] = x_Old_مداخلت_Level;
    }
    واپس pdPASS;
}

خالی __attribute__((optimize("-O3"))) v_پورٹ_Exit_Critical_Multi_Core(پورٹ_MUX_TYPE *mux)
{
#اگر CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
    اگر (unlikely(پورٹ_x_Thread_محفوظ_Claimed)) {
        واپس;
    }
#اگر_ختم
    /* This function may be called in a nested manner. Therefore, we only need
     * to re-enable interrupts if this is the last call to exit the critical. We
     * can use the nesting count to determine whether this is the last exit call.
     */
    /* Interrupts remain disabled for the whole critical section, so the core id is
     * stable and the spinlock does not need to mask them again. Read the core id
     * once and reuse it for both the release owner id and the per-core nesting state. */
    بنیاد_Type_t coreID = x_پورٹ_Get_Core_ID();
    spinlock_release_impl(mux, spinlock_owner_id_for_core(coreID));
    بنیاد_Type_t nesting = پورٹ_ux_Critical_Nesting[coreID];

    /* Critical section nesting count must never be negative */
    configASSERT( nesting > 0 );

    اگر (nesting > 0) {
        nesting--;
        پورٹ_ux_Critical_Nesting[coreID] = nesting;

        //This is the last exit call, restore the saved interrupt level
        اگر ( nesting == 0 ) {
            پورٹ_CLEAR_مداخلت_نقاب_FROM_ISR(پورٹ_ux_Old_مداخلت_State[coreID]);
        }
    }
}


بنیاد_Type_t x_پورٹ_Enter_Critical_Timeout_Compliance(پورٹ_MUX_TYPE *mux, بنیاد_Type_t timeout)
{
    بنیاد_Type_t ret;
    اگر (!x_پورٹ_اندر_Isr_Context()) {
        ret = x_پورٹ_Enter_Critical_Timeout(mux, timeout);
    } ورنہ {
        esp_rom_printf("port*_CRITICAL called from ISR context. Aborting!\n");
        abort();
        ret = pdFAIL;
    }
    واپس ret;
}

خالی v_پورٹ_Exit_Critical_Compliance(پورٹ_MUX_TYPE *mux)
{
    اگر (!x_پورٹ_اندر_Isr_Context()) {
        v_پورٹ_Exit_Critical_Multi_Core(mux);
    } ورنہ {
        esp_rom_printf("port*_CRITICAL called from ISR context. Aborting!\n");
        abort();
    }
}
#اگر_ختم /* (configNUM_CORES > 1) */

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
#اگر (config_عدد_CORES > 1)
        esp_rom_printf("vPortEnterCritical(void) is not supported on multi-core targets. Please use vPortEnterCriticalMultiCore(portMUX_TYPE *mux) instead.\n");
        abort();
#اگر_ختم /* (configNUM_CORES > 1) */
#اگر CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
    اگر (unlikely(پورٹ_x_Thread_محفوظ_Claimed)) {
        واپس;
    }
#اگر_ختم
    بنیاد_Type_t state = پورٹ_سیٹ_مداخلت_نقاب_FROM_ISR();
    پورٹ_ux_Critical_Nesting[0]++;

    اگر (پورٹ_ux_Critical_Nesting[0] == 1) {
        پورٹ_ux_Old_مداخلت_State[0] = state;
    }
}

خالی v_پورٹ_Exit_Critical(خالی)
{
#اگر (config_عدد_CORES > 1)
        esp_rom_printf("vPortExitCritical(void) is not supported on multi-core targets. Please use vPortExitCriticalMultiCore(portMUX_TYPE *mux) instead.\n");
        abort();
#اگر_ختم /* (configNUM_CORES > 1) */
#اگر CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
    اگر (unlikely(پورٹ_x_Thread_محفوظ_Claimed)) {
        واپس;
    }
#اگر_ختم

    /* Critical section nesting count must never be negative */
    configASSERT( پورٹ_ux_Critical_Nesting[0] > 0 );

    اگر (پورٹ_ux_Critical_Nesting[0] > 0) {
        پورٹ_ux_Critical_Nesting[0]--;

        اگر (پورٹ_ux_Critical_Nesting[0] == 0) {
            پورٹ_CLEAR_مداخلت_نقاب_FROM_ISR(پورٹ_ux_Old_مداخلت_State[0]);
        }
    }
}

// ---------------------- Yielding -------------------------

خالی v_پورٹ_Yield(خالی)
{
    بنیاد_Type_t coreID = x_پورٹ_Get_Core_ID();

    اگر (پورٹ_ux_مداخلت_Nesting[coreID]) {
        v_پورٹ_Yield_From_ISR();
    } ورنہ {
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
        جب (پورٹ_x_Scheduler_Running[coreID] && پورٹ_ux_Critical_Nesting[coreID] == 0 && crosscore_int_ll_get_state(coreID) != 0) {}
    }
}

خالی v_پورٹ_Yield_From_ISR( خالی )
{
    trace_ISR_EXIT_مقصد_SCHEDULER();
    بنیاد_Type_t coreID = x_پورٹ_Get_Core_ID();
    پورٹ_x_Scheduler_Running[coreID] = 1;
    x_پورٹ_Switch_Flag[coreID] = 1;
}

خالی v_پورٹ_Yield_Other_Core(بنیاد_Type_t coreid)
{
    esp_crosscore_int_send_yield(coreid);
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

// ----------------------- System --------------------------

uint32_t x_پورٹ_Get_Tick_شرح_Hz(خالی)
{
    واپس (uint32_t)config_TICK_شرح_HZ;
}

#تعریف_کرو ڈھیر_WATCH_AREA_سائز 32
#تعریف_کرو ڈھیر_WATCH_نقطہ_نمبر (SOC_CPU_WATCHPOINTS_عدد - 1)

خالی v_پورٹ_سیٹ_ڈھیر_Watchpoint(خالی *px_ڈھیر_آغاز)
{
    uint32_t addr = (uint32_t)px_ڈھیر_آغاز;
    addr = (addr + (ڈھیر_WATCH_AREA_سائز - 1)) & (~(ڈھیر_WATCH_AREA_سائز - 1));
    esp_cpu_سیٹ_watchpoint(ڈھیر_WATCH_نقطہ_نمبر, (حرف *)addr, ڈھیر_WATCH_AREA_سائز, ESP_CPU_WATCHPOINT_STORE);
}

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

    #اگر ( CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST )
        اگر (pxTCB != تہی) {
            v_Task_سیٹ_Pie_Blacklisted((TaskHandle_t)pxTCB, pdFALSE);
        }
    #اگر_ختم /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */

    #اگر ( SOC_CPU_COPROC_عدد > 0 )
        /* Cleanup coproc save area */
        v_پورٹ_Clean_اوپر_Coproc_Area( pxTCB );
    #اگر_ختم /* SOC_CPU_COPROC_NUM > 0 */
}


#اگر ( SOC_CPU_COPROC_عدد > 0 )

// ----------------------- Coprocessors --------------------------

/**
 * @brief Pin the given task to the given core
 *
 * This function is called when a task uses a coprocessor. Since the coprocessors registers
 * are saved lazily, as soon as a task starts using one, it must always be scheduled on the core
 * it is currently executing on.
 */
#اگر ( config_عدد_CORES > 1 )
خالی v_پورٹ_Task_Pin_مقصد_Core(StaticTask_t* task, صحیح coreid)
{
    task->xDummyCoreID = coreid;
}
#اگر_ختم /* configNUM_CORES > 1 */


/**
 * @brief Function to call to simulate an `abort()` occurring in a different context than the one it's called from.
 */
خارجی خالی xt_unhandled_exception(خالی *چوکھٹا);

/**
 * @brief Get coprocessor save area out of the given task. If the coprocessor area is not created,
 *        it shall be allocated.
 *
 * @param task Task to get the coprocessor save area of
 * @param allocate When true, memory will be allocated for the coprocessor if it hasn't been allocated yet.
 *                 When false, the coprocessor memory will be left as NULL if not allocated.
 * @param coproc Coprocessor number to allocate memory for
 */
RvCoprocSaveArea* px_پورٹ_Get_Coproc_Area(StaticTask_t* task, bool allocate, صحیح coproc)
{
    أكد(coproc < SOC_CPU_COPROC_عدد);

    ثابت UBaseType_t bottomstack = (UBaseType_t) task->pxDummy8;
    RvCoprocSaveArea* sa = px_Retrieve_Coproc_Save_Area_From_ڈھیر_اشارہ_گر(bottomstack);
    /* Check if the allocator is NULL. Since we don't have a way to get the end of the stack
     * during its initialization, we have to do this here */
    اگر (sa->sa_allocator == 0) {
        /* Since the lowest stack address shall not be used as `sp` anymore, we will modify it */
        sa->sa_tcbstack = task->pxDummy6;
        sa->sa_allocator = (UBaseType_t) task->pxDummy6;
    }

    /* Check if coprocessor area is allocated */
    اگر (allocate && sa->sa_coprocs[coproc] == تہی) {
        ثابت uint32_t coproc_sa_sizes[] = {
            RV_COPROC0_سائز, RV_COPROC1_سائز, RV_COPROC2_سائز
        };
        ثابت uint32_t coproc_sa_align[] = {
            RV_COPROC0_ALIGN, RV_COPROC1_ALIGN, RV_COPROC2_ALIGN
        };
        /* The allocator points to a usable part of the stack, use it for the coprocessor.
         * Align it up to the coprocessor save area requirement */
        sa->sa_allocator = (sa->sa_allocator + coproc_sa_align[coproc] - 1) & ~(coproc_sa_align[coproc] - 1);
        sa->sa_coprocs[coproc] = (خالی*) (sa->sa_allocator);
        sa->sa_allocator += coproc_sa_sizes[coproc];
        /* Update the lowest address of the stack to prevent FreeRTOS performing overflow/watermark checks on the coprocessors contexts */
        task->pxDummy6 = (خالی*) (sa->sa_allocator);
        /* Make sure the Task stack pointer is not pointing to the coprocessor context area, in other words, make
         * sure we don't have a stack overflow */
        خالی* task_sp = task->pxDummy1;
        اگر (task_sp <= task->pxDummy6) {
            /* In theory we need to call vApplicationStackOverflowHook to trigger the stack overflow callback,
             * but in practice, since we are already in an exception handler, this won't work, so let's manually
             * trigger an exception with the previous coprocessor owner's TCB */
            g_panic_abort = true;
            g_panic_abort_details = (حرف *) "ERROR: Stack overflow while saving coprocessor context!\n";
            xt_unhandled_exception(task_sp);
        }
    }
    واپس sa;
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
RvCoprocSaveArea* px_پورٹ_Update_Coproc_Owner(صحیح coreid, صحیح coproc, StaticTask_t* owner)
{
    RvCoprocSaveArea* sa = تہی;
    /* Address of coprocessor owner */
    StaticTask_t** owner_addr = &پورٹ_ux_Coproc_Owner[ coreid ][ coproc ];
    /* Atomically exchange former owner with the new one */
    StaticTask_t* former = Atomic_تبادلہ_اشارے_p32((خالی**) owner_addr, owner);
    /* Get the save area of former owner. small optimization here, if the former owner is the new owner,
     * return -1. This will simplify the assembly code while making it faster. */
    اگر (former == owner) {
        sa = (خالی*) -1;
    } ورنہ اگر (former != تہی) {
        /* Allocate coprocessor memory if not available yet */
        sa = px_پورٹ_Get_Coproc_Area(former, true, coproc);
    }
    واپس sa;
}


/**
 * @brief Aborts execution when a coprocessor was used in an ISR context
 */
خالی v_پورٹ_Coproc_مستعمل_اندر_ISR(خالی* چوکھٹا)
{
    /* Since this function is called from an exception handler, the interrupts are disabled,
     * as such, it is not possible to trigger another exception as would `abort` do.
     * Simulate an abort without actually triggering an exception. */
    g_panic_abort = true;
    g_panic_abort_details = (حرف *) "ERROR: Coprocessors must not be used in ISRs!\n";
    xt_unhandled_exception(چوکھٹا);
}

#اگر CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST
/**
 * @brief Abort if the given task is blacklisted from using the PIE coprocessor.
 *
 * Called from the PIE illegal-instruction path, so this must not trigger another
 * exception via abort().
 */
خالی v_پورٹ_Abort_If_Task_Pie_Blacklisted(StaticTask_t *task, خالی *چوکھٹا)
{
    اگر (task != تہی && xTaskGetPieBlacklisted((TaskHandle_t)task) != pdFALSE) {
        g_panic_abort = true;
        g_panic_abort_details = (حرف *) "ERROR: Blacklisted task executed a PIE instruction!\n";
        xt_unhandled_exception(چوکھٹا);
    }
}
#اگر_ختم /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */

#اگر CONFIG_IDF_ہدف_ESP32_S31
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
خالی* v_پورٹ_Task_مستعمل_PIEOn_CPU0(StaticTask_t* task, خالی* چوکھٹا)
{
#اگر CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST
    v_پورٹ_Abort_If_Task_Pie_Blacklisted(task, چوکھٹا);
#اگر_ختم
#اگر CONFIG_FREERTOS_UNICORE
    g_panic_abort = true;
    g_panic_abort_details = (حرف *) "ERROR: PIE coprocessor is not supported in unicore configuration!\n";
    xt_unhandled_exception(چوکھٹا);
    واپس تہی;
#ورنہ
    خالی* context_مقصد_save = تہی;
    /* Make sure we are not in an interrupt context nor in a critical section */
    اگر (x_پورٹ_اندر_Isr_Context()) {
        /* We are in an interrupt context, abort */
        v_پورٹ_Coproc_مستعمل_اندر_ISR(چوکھٹا);
    }
    /* Since we count on crosscore interrupt to reschedule the current task, we must not be in a
     * critical section. In other words, we must be able to yield */
    اگر (!x_پورٹ_Can_Yield()) {
        g_panic_abort = true;
        g_panic_abort_details = (حرف *) "ERROR: PIE coprocessor must not be used in critical sections!\n";
        xt_unhandled_exception(چوکھٹا);
    }

    /* Check if the current task is the owner of the FPU on the current core. No need to make the following two instructions
     * atomic since we are in an exception context, we can't be interrupted by an interrupt. */
    اگر (پورٹ_ux_Coproc_Owner[0][FPU_COPROC_IDX] == task) {
        /* Task is the owner of the FPU on the current core, set the new owner to NULL and return the save area to fill */
        RvCoprocSaveArea* sa = px_پورٹ_Update_Coproc_Owner(0, FPU_COPROC_IDX, تہی);
        /* `sa` is not NULL here for sure */
        context_مقصد_save = sa->sa_coprocs[FPU_COPROC_IDX];
    }
    /* Migrate the task to core 1. NOTE: This will override any existing pinning of the task */
    v_پورٹ_Task_Pin_مقصد_Core(task, 1);
    /* Raised an error if the scheduler is NOT running on core 0 */
    اگر (!پورٹ_x_Scheduler_Running[0]) {
        /* Scheduler is not running on core 0, raise an error */
        g_panic_abort = true;
        g_panic_abort_details = (حرف *) "ERROR: Scheduler is not running on core 0, task must migrate to core 1!\n";
        xt_unhandled_exception(چوکھٹا);
    }
    /* Send a cross-core interrupt on the current core, it won't be triggered until we return from the exception handler */
    esp_crosscore_int_send_yield(0);
    واپس context_مقصد_save;
#اگر_ختم /* CONFIG_FREERTOS_UNICORE */
}

#اگر_ختم /* CONFIG_IDF_TARGET_ESP32S31 */


#اگر_ختم /* SOC_CPU_COPROC_NUM > 0 */

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

/* ---------------------------------------------- Misc Implementations -------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */
#اگر (SOC_CPU_COPROC_عدد > 0) && SOC_CPU_HAS_FPU && SOC_PM_FPU_RETENTION_BY_SW
بنیاد_Type_t x_پورٹ_FPUContext_Is_Dirty(بنیاد_Type_t core_id)
{
    واپس (بنیاد_Type_t)(پورٹ_ux_Coproc_Owner[core_id][FPU_COPROC_IDX] != تہی);
}
#اگر_ختم /* (SOC_CPU_COPROC_NUM > 0) && SOC_CPU_HAS_FPU && SOC_PM_FPU_RETENTION_BY_SW */
