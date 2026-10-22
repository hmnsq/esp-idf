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

#جيڪڏهن_وصف_نه_ٿيل PORTABLE_H
#وصف_ڪريو PORTABLE_H

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
#جيڪڏهن_وصف_نه_ٿيل پورٽ_ENTER_CRITICAL
    #ضمن "portmacro.h"
#جيڪڏهن_پورو

#إذا پورٽ_BYTE_ALIGNMENT == 32
    #وصف_ڪريو پورٽ_BYTE_ALIGNMENT_نقاب    ( 0x001f )
#وإلاإذا پورٽ_BYTE_ALIGNMENT == 16
    #وصف_ڪريو پورٽ_BYTE_ALIGNMENT_نقاب    ( 0x000f )
#وإلاإذا پورٽ_BYTE_ALIGNMENT == 8
    #وصف_ڪريو پورٽ_BYTE_ALIGNMENT_نقاب    ( 0x0007 )
#وإلاإذا پورٽ_BYTE_ALIGNMENT == 4
    #وصف_ڪريو پورٽ_BYTE_ALIGNMENT_نقاب    ( 0x0003 )
#وإلاإذا پورٽ_BYTE_ALIGNMENT == 2
    #وصف_ڪريو پورٽ_BYTE_ALIGNMENT_نقاب    ( 0x0001 )
#وإلاإذا پورٽ_BYTE_ALIGNMENT == 1
    #وصف_ڪريو پورٽ_BYTE_ALIGNMENT_نقاب    ( 0x0000 )
#وإلا /* if portBYTE_ALIGNMENT == 32 */
    #غلطي "Invalid portBYTE_ALIGNMENT definition"
#جيڪڏهن_پورو /* if portBYTE_ALIGNMENT == 32 */

#جيڪڏهن_وصف_نه_ٿيل پورٽ_USING_MPU_WRAPPERS
    #وصف_ڪريو پورٽ_USING_MPU_WRAPPERS    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل پورٽ_انگ_CONFIGURABLE_REGIONS
    #وصف_ڪريو پورٽ_انگ_CONFIGURABLE_REGIONS    1
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل پورٽ_HAS_ڍير_حد_کان_مٿي_CHECKING
    #وصف_ڪريو پورٽ_HAS_ڍير_حد_کان_مٿي_CHECKING    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل پورٽ_ARCH_NAME
    #وصف_ڪريو پورٽ_ARCH_NAME    عدم
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل config_ڍير_ورهاست_FROM_SEPARATE_ڍير
    /* Defaults to 0 for backward compatibility. */
    #وصف_ڪريو config_ڍير_ورهاست_FROM_SEPARATE_ڍير    0
#جيڪڏهن_پورو

/* *INDENT-OFF* */
#جيڪڏهن_وصف_ٿيل __cplusplus
    خارجي "C" {
#جيڪڏهن_پورو
/* *INDENT-ON* */

#ضمن "mpu_wrappers.h"

/*
 * Setup the stack of a new task so it is ready to be placed under the
 * scheduler control.  The registers have to be placed on the stack in
 * the order that the port expects to find them.
 *
 */
#إذا ( پورٽ_USING_MPU_WRAPPERS == 1 )
    #إذا ( پورٽ_HAS_ڍير_حد_کان_مٿي_CHECKING == 1 )
        ڍير_Type_t * px_پورٽ_Initialise_ڍير( ڍير_Type_t * px_چوٽي_جو_ڍير,
                                             ڍير_Type_t * px_پڇاڙي_جو_ڍير,
                                             TaskFunction_t px_ڪوڊ,
                                             فراغ * pvParameters,
                                             بنياد_Type_t x_لاڳيتو_Privileged ) PRIVILEGED_FUNCTION;
    #وإلا
        ڍير_Type_t * px_پورٽ_Initialise_ڍير( ڍير_Type_t * px_چوٽي_جو_ڍير,
                                             TaskFunction_t px_ڪوڊ,
                                             فراغ * pvParameters,
                                             بنياد_Type_t x_لاڳيتو_Privileged ) PRIVILEGED_FUNCTION;
    #جيڪڏهن_پورو
#وإلا /* if ( portUSING_MPU_WRAPPERS == 1 ) */
    #إذا ( پورٽ_HAS_ڍير_حد_کان_مٿي_CHECKING == 1 )
        ڍير_Type_t * px_پورٽ_Initialise_ڍير( ڍير_Type_t * px_چوٽي_جو_ڍير,
                                             ڍير_Type_t * px_پڇاڙي_جو_ڍير,
                                             TaskFunction_t px_ڪوڊ,
                                             فراغ * pvParameters ) PRIVILEGED_FUNCTION;
    #وإلا
        ڍير_Type_t * px_پورٽ_Initialise_ڍير( ڍير_Type_t * px_چوٽي_جو_ڍير,
                                             TaskFunction_t px_ڪوڊ,
                                             فراغ * pvParameters ) PRIVILEGED_FUNCTION;
    #جيڪڏهن_پورو
#جيڪڏهن_پورو /* if ( portUSING_MPU_WRAPPERS == 1 ) */

/* Used by heap_5.c to define the start address and size of each memory region
 * that together comprise the total FreeRTOS heap space. */
تعريفنوع بنية ڍير_Region
{
    uint8_t * puc_شروعات_پتو;
    size_t x_سائيز_اندريون_بائيٽون;
} ڍير_Region_t;

/* Used to pass information about the heap out of vPortGetHeapStats(). */
تعريفنوع بنية x_ڍير_Stats
{
    size_t x_موجود_ڍير_Space_اندريون_بائيٽون;      /* The total heap size currently available - this is the sum of all the free blocks, not the largest block that can be allocated. */
    size_t x_سائيز_جو_سڀ_کان_وڏو_Free_بلاڪ_اندريون_بائيٽون;  /* The maximum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t x_سائيز_جو_سڀ_کان_ننڍو_Free_بلاڪ_اندريون_بائيٽون; /* The minimum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t x_عدد_جو_Free_بلاڪون;             /* The number of free memory blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t x_Minimum_Ever_Free_بائيٽون_بچيل;  /* The minimum amount of total free memory (sum of all free blocks) there has been in the heap since the system booted. */
    size_t x_عدد_جو_Successful_Allocations;  /* The number of calls to pvPortMalloc() that have returned a valid memory block. */
    size_t x_عدد_جو_Successful_Frees;        /* The number of calls to vPortFree() that has successfully freed a block of memory. */
} ڍير_Stats_t;

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
فراغ v_پورٽ_Define_ڍير_Regions( ثابت ڍير_Region_t * ثابت px_ڍير_Regions ) PRIVILEGED_FUNCTION;

/*
 * Returns a HeapStats_t structure filled with information about the current
 * heap state.
 */
فراغ v_پورٽ_Get_ڍير_Stats( ڍير_Stats_t * px_ڍير_Stats );

/*
 * Map to the memory management routines required for the port.
 */
فراغ * pv_پورٽ_Malloc( size_t x_سائيز ) PRIVILEGED_FUNCTION;
فراغ * pv_پورٽ_Calloc( size_t x_انگ,
                     size_t x_سائيز ) PRIVILEGED_FUNCTION;
فراغ v_پورٽ_Free( فراغ * pv ) PRIVILEGED_FUNCTION;
فراغ v_پورٽ_Initialise_بلاڪون( فراغ ) PRIVILEGED_FUNCTION;
size_t x_پورٽ_Get_Free_ڍير_سائيز( فراغ ) PRIVILEGED_FUNCTION;
size_t x_پورٽ_Get_Minimum_Ever_Free_ڍير_سائيز( فراغ ) PRIVILEGED_FUNCTION;

#إذا ( config_ڍير_ورهاست_FROM_SEPARATE_ڍير == 1 )
    فراغ * pv_پورٽ_Malloc_ڍير( size_t x_سائيز ) PRIVILEGED_FUNCTION;
    فراغ v_پورٽ_Free_ڍير( فراغ * pv ) PRIVILEGED_FUNCTION;
#وإلا
    #وصف_ڪريو pv_پورٽ_Malloc_ڍير    pv_پورٽ_Malloc
    #وصف_ڪريو v_پورٽ_Free_ڍير       v_پورٽ_Free
#جيڪڏهن_پورو

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
#جيڪڏهن_پورو

/*
 * Setup the hardware ready for the scheduler to take control.  This generally
 * sets up a tick interrupt and sets timers for the correct tick frequency.
 */
بنياد_Type_t x_پورٽ_شروعات_Scheduler( فراغ ) PRIVILEGED_FUNCTION;

/*
 * Undo any hardware/ISR setup that was performed by xPortStartScheduler() so
 * the hardware is left in its original condition after the scheduler stops
 * executing.
 */
فراغ v_پورٽ_پڇاڙي_Scheduler( فراغ ) PRIVILEGED_FUNCTION;

/*
 * The structures and methods of manipulating the MPU are contained within the
 * port layer.
 *
 * Fills the xMPUSettings structure with the memory region information
 * contained in xRegions.
 */
#إذا ( پورٽ_USING_MPU_WRAPPERS == 1 )
    بنية x_يادگيري_REGION;
    فراغ v_پورٽ_Store_Task_MPUSettings( xMPU_SETTINGS * xMPUSettings,
                                    ثابت بنية x_يادگيري_REGION * ثابت xRegions,
                                    ڍير_Type_t * px_Bottom_جو_ڍير,
                                    uint32_t ul_ڍير_اونهائي ) PRIVILEGED_FUNCTION;
#جيڪڏهن_پورو

/* *INDENT-OFF* */
#جيڪڏهن_وصف_ٿيل __cplusplus
    }
#جيڪڏهن_پورو
/* *INDENT-ON* */

#جيڪڏهن_پورو /* PORTABLE_H */
