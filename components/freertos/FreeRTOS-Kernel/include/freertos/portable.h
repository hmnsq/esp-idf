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

#جے_بنا_تعریف PORTABLE_H
#تعریف_کرو PORTABLE_H

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
#جے_بنا_تعریف پورٹ_ENTER_CRITICAL
    #ضمن "portmacro.h"
#جے_انت

#إذا پورٹ_BYTE_ALIGNMENT == 32
    #تعریف_کرو پورٹ_BYTE_ALIGNMENT_نقاب    ( 0x001f )
#وإلاإذا پورٹ_BYTE_ALIGNMENT == 16
    #تعریف_کرو پورٹ_BYTE_ALIGNMENT_نقاب    ( 0x000f )
#وإلاإذا پورٹ_BYTE_ALIGNMENT == 8
    #تعریف_کرو پورٹ_BYTE_ALIGNMENT_نقاب    ( 0x0007 )
#وإلاإذا پورٹ_BYTE_ALIGNMENT == 4
    #تعریف_کرو پورٹ_BYTE_ALIGNMENT_نقاب    ( 0x0003 )
#وإلاإذا پورٹ_BYTE_ALIGNMENT == 2
    #تعریف_کرو پورٹ_BYTE_ALIGNMENT_نقاب    ( 0x0001 )
#وإلاإذا پورٹ_BYTE_ALIGNMENT == 1
    #تعریف_کرو پورٹ_BYTE_ALIGNMENT_نقاب    ( 0x0000 )
#وإلا /* if portBYTE_ALIGNMENT == 32 */
    #غلطی "Invalid portBYTE_ALIGNMENT definition"
#جے_انت /* if portBYTE_ALIGNMENT == 32 */

#جے_بنا_تعریف پورٹ_USING_MPU_WRAPPERS
    #تعریف_کرو پورٹ_USING_MPU_WRAPPERS    0
#جے_انت

#جے_بنا_تعریف پورٹ_عدد_CONFIGURABLE_REGIONS
    #تعریف_کرو پورٹ_عدد_CONFIGURABLE_REGIONS    1
#جے_انت

#جے_بنا_تعریف پورٹ_HAS_ڈھیر_حد_توں_ودھ_CHECKING
    #تعریف_کرو پورٹ_HAS_ڈھیر_حد_توں_ودھ_CHECKING    0
#جے_انت

#جے_بنا_تعریف پورٹ_ARCH_NAME
    #تعریف_کرو پورٹ_ARCH_NAME    عدم
#جے_انت

#جے_بنا_تعریف config_ڈھیر_ونڈ_FROM_SEPARATE_ڈھیر
    /* Defaults to 0 for backward compatibility. */
    #تعریف_کرو config_ڈھیر_ونڈ_FROM_SEPARATE_ڈھیر    0
#جے_انت

/* *INDENT-OFF* */
#جے_تعریف_شدہ __cplusplus
    خارجي "C" {
#جے_انت
/* *INDENT-ON* */

#ضمن "mpu_wrappers.h"

/*
 * Setup the stack of a new task so it is ready to be placed under the
 * scheduler control.  The registers have to be placed on the stack in
 * the order that the port expects to find them.
 *
 */
#إذا ( پورٹ_USING_MPU_WRAPPERS == 1 )
    #إذا ( پورٹ_HAS_ڈھیر_حد_توں_ودھ_CHECKING == 1 )
        ڈھیر_Type_t * px_پورٹ_Initialise_ڈھیر( ڈھیر_Type_t * px_چوٹی_دا_ڈھیر,
                                             ڈھیر_Type_t * px_سرا_دا_ڈھیر,
                                             TaskFunction_t px_کوڈ,
                                             فراغ * pvParameters,
                                             ادھار_Type_t x_لگاتار_Privileged ) PRIVILEGED_FUNCTION;
    #وإلا
        ڈھیر_Type_t * px_پورٹ_Initialise_ڈھیر( ڈھیر_Type_t * px_چوٹی_دا_ڈھیر,
                                             TaskFunction_t px_کوڈ,
                                             فراغ * pvParameters,
                                             ادھار_Type_t x_لگاتار_Privileged ) PRIVILEGED_FUNCTION;
    #جے_انت
#وإلا /* if ( portUSING_MPU_WRAPPERS == 1 ) */
    #إذا ( پورٹ_HAS_ڈھیر_حد_توں_ودھ_CHECKING == 1 )
        ڈھیر_Type_t * px_پورٹ_Initialise_ڈھیر( ڈھیر_Type_t * px_چوٹی_دا_ڈھیر,
                                             ڈھیر_Type_t * px_سرا_دا_ڈھیر,
                                             TaskFunction_t px_کوڈ,
                                             فراغ * pvParameters ) PRIVILEGED_FUNCTION;
    #وإلا
        ڈھیر_Type_t * px_پورٹ_Initialise_ڈھیر( ڈھیر_Type_t * px_چوٹی_دا_ڈھیر,
                                             TaskFunction_t px_کوڈ,
                                             فراغ * pvParameters ) PRIVILEGED_FUNCTION;
    #جے_انت
#جے_انت /* if ( portUSING_MPU_WRAPPERS == 1 ) */

/* Used by heap_5.c to define the start address and size of each memory region
 * that together comprise the total FreeRTOS heap space. */
تعريفنوع بنية ڈھیر_Region
{
    uint8_t * puc_شروع_پتہ;
    size_t x_ناپ_وچ_بائٹاں;
} ڈھیر_Region_t;

/* Used to pass information about the heap out of vPortGetHeapStats(). */
تعريفنوع بنية x_ڈھیر_Stats
{
    size_t x_دستیاب_ڈھیر_Space_وچ_بائٹاں;      /* The total heap size currently available - this is the sum of all the free blocks, not the largest block that can be allocated. */
    size_t x_ناپ_دا_سبھ_توں_وڈا_Free_بلاک_وچ_بائٹاں;  /* The maximum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t x_ناپ_دا_سبھ_توں_چھوٹا_Free_بلاک_وچ_بائٹاں; /* The minimum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t x_نمبر_دا_Free_بلاکاں;             /* The number of free memory blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t x_Minimum_Ever_Free_بائٹاں_بچیا;  /* The minimum amount of total free memory (sum of all free blocks) there has been in the heap since the system booted. */
    size_t x_نمبر_دا_Successful_Allocations;  /* The number of calls to pvPortMalloc() that have returned a valid memory block. */
    size_t x_نمبر_دا_Successful_Frees;        /* The number of calls to vPortFree() that has successfully freed a block of memory. */
} ڈھیر_Stats_t;

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
فراغ v_پورٹ_Define_ڈھیر_Regions( ثابت ڈھیر_Region_t * ثابت px_ڈھیر_Regions ) PRIVILEGED_FUNCTION;

/*
 * Returns a HeapStats_t structure filled with information about the current
 * heap state.
 */
فراغ v_پورٹ_Get_ڈھیر_Stats( ڈھیر_Stats_t * px_ڈھیر_Stats );

/*
 * Map to the memory management routines required for the port.
 */
فراغ * pv_پورٹ_Malloc( size_t x_ناپ ) PRIVILEGED_FUNCTION;
فراغ * pv_پورٹ_Calloc( size_t x_عدد,
                     size_t x_ناپ ) PRIVILEGED_FUNCTION;
فراغ v_پورٹ_Free( فراغ * pv ) PRIVILEGED_FUNCTION;
فراغ v_پورٹ_Initialise_بلاکاں( فراغ ) PRIVILEGED_FUNCTION;
size_t x_پورٹ_Get_Free_ڈھیر_ناپ( فراغ ) PRIVILEGED_FUNCTION;
size_t x_پورٹ_Get_Minimum_Ever_Free_ڈھیر_ناپ( فراغ ) PRIVILEGED_FUNCTION;

#إذا ( config_ڈھیر_ونڈ_FROM_SEPARATE_ڈھیر == 1 )
    فراغ * pv_پورٹ_Malloc_ڈھیر( size_t x_ناپ ) PRIVILEGED_FUNCTION;
    فراغ v_پورٹ_Free_ڈھیر( فراغ * pv ) PRIVILEGED_FUNCTION;
#وإلا
    #تعریف_کرو pv_پورٹ_Malloc_ڈھیر    pv_پورٹ_Malloc
    #تعریف_کرو v_پورٹ_Free_ڈھیر       v_پورٹ_Free
#جے_انت

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
#جے_انت

/*
 * Setup the hardware ready for the scheduler to take control.  This generally
 * sets up a tick interrupt and sets timers for the correct tick frequency.
 */
ادھار_Type_t x_پورٹ_شروع_Scheduler( فراغ ) PRIVILEGED_FUNCTION;

/*
 * Undo any hardware/ISR setup that was performed by xPortStartScheduler() so
 * the hardware is left in its original condition after the scheduler stops
 * executing.
 */
فراغ v_پورٹ_سرا_Scheduler( فراغ ) PRIVILEGED_FUNCTION;

/*
 * The structures and methods of manipulating the MPU are contained within the
 * port layer.
 *
 * Fills the xMPUSettings structure with the memory region information
 * contained in xRegions.
 */
#إذا ( پورٹ_USING_MPU_WRAPPERS == 1 )
    بنية x_حافظہ_REGION;
    فراغ v_پورٹ_Store_Task_MPUSettings( xMPU_SETTINGS * xMPUSettings,
                                    ثابت بنية x_حافظہ_REGION * ثابت xRegions,
                                    ڈھیر_Type_t * px_Bottom_دا_ڈھیر,
                                    uint32_t ul_ڈھیر_ڈونگھائی ) PRIVILEGED_FUNCTION;
#جے_انت

/* *INDENT-OFF* */
#جے_تعریف_شدہ __cplusplus
    }
#جے_انت
/* *INDENT-ON* */

#جے_انت /* PORTABLE_H */
