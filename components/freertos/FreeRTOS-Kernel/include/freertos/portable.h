/*
 * FreeRTOS Kernel V10.5.1 (ESP-IDF SMP modified)
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-FileCopyrightText: 2021 Amazon.com, Inc. or its affiliates
 *
 * SPDX-License-Identifier: MIT
 *
 * SPDX-FileContributor: 2023 Espressif Systems (Shanghai) CO LTD
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

/*-----------------------------------------------------------
* Portable layer API.  Each function must be defined for each port.
*----------------------------------------------------------*/

#اگر_تعریف_نشده PORTABLE_H
#تعریف_کن PORTABLE_H

/* Each FreeRTOS port has a unique portmacro.h header file.  Originally a
 * pre-processor definition was used to ensure the pre-processor found the correct
 * portmacro.h file for the port being used.  That scheme was deprecated in favour
 * of setting the compiler's include path such that it found the correct
 * portmacro.h file - removing the need for the constant and allowing the
 * portmacro.h file to be located anywhere in relation to the port being used.
 * Purely for reasons of backward compatibility the old method is still valid, but
 * to make it clear that new projects should not use it, support for the port
 * specific constants has been moved into the deprecated_definitions.h header
 * file. */
#ضمن "deprecated_definitions.h"

/* If portENTER_CRITICAL is not defined then including deprecated_definitions.h
 * did not result in a portmacro.h header file being included - and it should be
 * included here.  In this case the path to the correct portmacro.h header file
 * must be set in the compiler's include path. */
#اگر_تعریف_نشده پورت_ENTER_CRITICAL
    #ضمن "portmacro.h"
#پایان_اگر

#إذا پورت_BYTE_ALIGNMENT == 32
    #تعریف_کن پورت_BYTE_ALIGNMENT_ماسک    ( 0x001f )
#وإلاإذا پورت_BYTE_ALIGNMENT == 16
    #تعریف_کن پورت_BYTE_ALIGNMENT_ماسک    ( 0x000f )
#وإلاإذا پورت_BYTE_ALIGNMENT == 8
    #تعریف_کن پورت_BYTE_ALIGNMENT_ماسک    ( 0x0007 )
#وإلاإذا پورت_BYTE_ALIGNMENT == 4
    #تعریف_کن پورت_BYTE_ALIGNMENT_ماسک    ( 0x0003 )
#وإلاإذا پورت_BYTE_ALIGNMENT == 2
    #تعریف_کن پورت_BYTE_ALIGNMENT_ماسک    ( 0x0001 )
#وإلاإذا پورت_BYTE_ALIGNMENT == 1
    #تعریف_کن پورت_BYTE_ALIGNMENT_ماسک    ( 0x0000 )
#وإلا /* if portBYTE_ALIGNMENT == 32 */
    #خطا "Invalid portBYTE_ALIGNMENT definition"
#پایان_اگر /* if portBYTE_ALIGNMENT == 32 */

#اگر_تعریف_نشده پورت_USING_MPU_WRAPPERS
    #تعریف_کن پورت_USING_MPU_WRAPPERS    0
#پایان_اگر

#اگر_تعریف_نشده پورت_رقم_CONFIGURABLE_REGIONS
    #تعریف_کن پورت_رقم_CONFIGURABLE_REGIONS    1
#پایان_اگر

#اگر_تعریف_نشده پورت_HAS_پشته_سرریز_CHECKING
    #تعریف_کن پورت_HAS_پشته_سرریز_CHECKING    0
#پایان_اگر

#اگر_تعریف_نشده پورت_ARCH_NAME
    #تعریف_کن پورت_ARCH_NAME    عدم
#پایان_اگر

#اگر_تعریف_نشده config_پشته_تخصیص_FROM_SEPARATE_انباشته
    /* Defaults to 0 for backward compatibility. */
    #تعریف_کن config_پشته_تخصیص_FROM_SEPARATE_انباشته    0
#پایان_اگر

/* *INDENT-OFF* */
#اگر_تعریف_شده __cplusplus
    خارجي "C" {
#پایان_اگر
/* *INDENT-ON* */

#ضمن "mpu_wrappers.h"

/*
 * Setup the stack of a new task so it is ready to be placed under the
 * scheduler control.  The registers have to be placed on the stack in
 * the order that the port expects to find them.
 *
 */
#إذا ( پورت_USING_MPU_WRAPPERS == 1 )
    #إذا ( پورت_HAS_پشته_سرریز_CHECKING == 1 )
        پشته_Type_t * px_پورت_Initialise_پشته( پشته_Type_t * px_بالاترین_از_پشته,
                                             پشته_Type_t * px_انتها_از_پشته,
                                             TaskFunction_t px_کد,
                                             فراغ * pvParameters,
                                             پایه_Type_t x_پیاپی_Privileged ) PRIVILEGED_FUNCTION;
    #وإلا
        پشته_Type_t * px_پورت_Initialise_پشته( پشته_Type_t * px_بالاترین_از_پشته,
                                             TaskFunction_t px_کد,
                                             فراغ * pvParameters,
                                             پایه_Type_t x_پیاپی_Privileged ) PRIVILEGED_FUNCTION;
    #پایان_اگر
#وإلا /* if ( portUSING_MPU_WRAPPERS == 1 ) */
    #إذا ( پورت_HAS_پشته_سرریز_CHECKING == 1 )
        پشته_Type_t * px_پورت_Initialise_پشته( پشته_Type_t * px_بالاترین_از_پشته,
                                             پشته_Type_t * px_انتها_از_پشته,
                                             TaskFunction_t px_کد,
                                             فراغ * pvParameters ) PRIVILEGED_FUNCTION;
    #وإلا
        پشته_Type_t * px_پورت_Initialise_پشته( پشته_Type_t * px_بالاترین_از_پشته,
                                             TaskFunction_t px_کد,
                                             فراغ * pvParameters ) PRIVILEGED_FUNCTION;
    #پایان_اگر
#پایان_اگر /* if ( portUSING_MPU_WRAPPERS == 1 ) */

/* Used by heap_5.c to define the start address and size of each memory region
 * that together comprise the total FreeRTOS heap space. */
تعريفنوع بنية انباشته_Region
{
    uint8_t * puc_شروع_آدرس;
    size_t x_اندازه_درون_بایتها;
} انباشته_Region_t;

/* Used to pass information about the heap out of vPortGetHeapStats(). */
تعريفنوع بنية x_انباشته_Stats
{
    size_t x_موجود_انباشته_Space_درون_بایتها;      /* The total heap size currently available - this is the sum of all the free blocks, not the largest block that can be allocated. */
    size_t x_اندازه_از_بزرگترین_Free_بلوک_درون_بایتها;  /* The maximum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t x_اندازه_از_کوچکترین_Free_بلوک_درون_بایتها; /* The minimum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t x_عدد_از_Free_بلوکها;             /* The number of free memory blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t x_Minimum_Ever_Free_بایتها_مانده;  /* The minimum amount of total free memory (sum of all free blocks) there has been in the heap since the system booted. */
    size_t x_عدد_از_Successful_Allocations;  /* The number of calls to pvPortMalloc() that have returned a valid memory block. */
    size_t x_عدد_از_Successful_Frees;        /* The number of calls to vPortFree() that has successfully freed a block of memory. */
} انباشته_Stats_t;

/*
 * Used to define multiple heap regions for use by heap_5.c.  This function
 * must be called before any calls to pvPortMalloc() - not creating a task,
 * queue, semaphore, mutex, software timer, event group, etc. will result in
 * pvPortMalloc being called.
 *
 * pxHeapRegions passes in an array of HeapRegion_t structures - each of which
 * defines a region of memory that can be used as the heap.  The array is
 * terminated by a HeapRegions_t structure that has a size of 0.  The region
 * with the lowest start address must appear first in the array.
 */
فراغ v_پورت_Define_انباشته_Regions( ثابت انباشته_Region_t * ثابت px_انباشته_Regions ) PRIVILEGED_FUNCTION;

/*
 * Returns a HeapStats_t structure filled with information about the current
 * heap state.
 */
فراغ v_پورت_Get_انباشته_Stats( انباشته_Stats_t * px_انباشته_Stats );

/*
 * Map to the memory management routines required for the port.
 */
فراغ * pv_پورت_Malloc( size_t x_اندازه ) PRIVILEGED_FUNCTION;
فراغ * pv_پورت_Calloc( size_t x_رقم,
                     size_t x_اندازه ) PRIVILEGED_FUNCTION;
فراغ v_پورت_Free( فراغ * pv ) PRIVILEGED_FUNCTION;
فراغ v_پورت_Initialise_بلوکها( فراغ ) PRIVILEGED_FUNCTION;
size_t x_پورت_Get_Free_انباشته_اندازه( فراغ ) PRIVILEGED_FUNCTION;
size_t x_پورت_Get_Minimum_Ever_Free_انباشته_اندازه( فراغ ) PRIVILEGED_FUNCTION;

#إذا ( config_پشته_تخصیص_FROM_SEPARATE_انباشته == 1 )
    فراغ * pv_پورت_Malloc_پشته( size_t x_اندازه ) PRIVILEGED_FUNCTION;
    فراغ v_پورت_Free_پشته( فراغ * pv ) PRIVILEGED_FUNCTION;
#وإلا
    #تعریف_کن pv_پورت_Malloc_پشته    pv_پورت_Malloc
    #تعریف_کن v_پورت_Free_پشته       v_پورت_Free
#پایان_اگر

#إذا ( configUSE_MALLOC_FAILED_HOOK == 1 )

/**
 * task.h
 * @code{c}
 * void vApplicationMallocFailedHook( void )
 * @endcode
 *
 * This hook function is called when allocation failed.
 */
    فراغ vApplicationMallocFailedHook( فراغ ); /*lint !e526 Symbol not defined as it is an application callback. */
#پایان_اگر

/*
 * Setup the hardware ready for the scheduler to take control.  This generally
 * sets up a tick interrupt and sets timers for the correct tick frequency.
 */
پایه_Type_t x_پورت_شروع_Scheduler( فراغ ) PRIVILEGED_FUNCTION;

/*
 * Undo any hardware/ISR setup that was performed by xPortStartScheduler() so
 * the hardware is left in its original condition after the scheduler stops
 * executing.
 */
فراغ v_پورت_انتها_Scheduler( فراغ ) PRIVILEGED_FUNCTION;

/*
 * The structures and methods of manipulating the MPU are contained within the
 * port layer.
 *
 * Fills the xMPUSettings structure with the memory region information
 * contained in xRegions.
 */
#إذا ( پورت_USING_MPU_WRAPPERS == 1 )
    بنية x_حافظه_REGION;
    فراغ v_پورت_Store_Task_MPUSettings( xMPU_SETTINGS * xMPUSettings,
                                    ثابت بنية x_حافظه_REGION * ثابت xRegions,
                                    پشته_Type_t * px_Bottom_از_پشته,
                                    uint32_t ul_پشته_عمق ) PRIVILEGED_FUNCTION;
#پایان_اگر

/* *INDENT-OFF* */
#اگر_تعریف_شده __cplusplus
    }
#پایان_اگر
/* *INDENT-ON* */

#پایان_اگر /* PORTABLE_H */
