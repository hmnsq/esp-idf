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

#నిర్వచితం_కాకపోతే PORTABLE_H
#నిర్వచించు PORTABLE_H

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
#అన్తర్భావ "deprecated_definitions.h"

/* If portENTER_CRITICAL is not defined then including deprecated_definitions.h
 * did not result in a portmacro.h header file being included - and it should be
 * included here.  In this case the path to the correct portmacro.h header file
 * must be set in the compiler's include path. */
#నిర్వచితం_కాకపోతే పోర్టు_ENTER_CRITICAL
    #అన్తర్భావ "portmacro.h"
#అయితే_ముగింపు

#ఒకవేళ పోర్టు_BYTE_ALIGNMENT == 32
    #నిర్వచించు పోర్టు_BYTE_ALIGNMENT_ముసుగు    ( 0x001f )
#లేదావేళ పోర్టు_BYTE_ALIGNMENT == 16
    #నిర్వచించు పోర్టు_BYTE_ALIGNMENT_ముసుగు    ( 0x000f )
#లేదావేళ పోర్టు_BYTE_ALIGNMENT == 8
    #నిర్వచించు పోర్టు_BYTE_ALIGNMENT_ముసుగు    ( 0x0007 )
#లేదావేళ పోర్టు_BYTE_ALIGNMENT == 4
    #నిర్వచించు పోర్టు_BYTE_ALIGNMENT_ముసుగు    ( 0x0003 )
#లేదావేళ పోర్టు_BYTE_ALIGNMENT == 2
    #నిర్వచించు పోర్టు_BYTE_ALIGNMENT_ముసుగు    ( 0x0001 )
#లేదావేళ పోర్టు_BYTE_ALIGNMENT == 1
    #నిర్వచించు పోర్టు_BYTE_ALIGNMENT_ముసుగు    ( 0x0000 )
#లేకపోతే /* if portBYTE_ALIGNMENT == 32 */
    #దోషం "Invalid portBYTE_ALIGNMENT definition"
#అయితే_ముగింపు /* if portBYTE_ALIGNMENT == 32 */

#నిర్వచితం_కాకపోతే పోర్టు_USING_MPU_WRAPPERS
    #నిర్వచించు పోర్టు_USING_MPU_WRAPPERS    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే పోర్టు_అంకె_CONFIGURABLE_REGIONS
    #నిర్వచించు పోర్టు_అంకె_CONFIGURABLE_REGIONS    1
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే పోర్టు_HAS_దొంతర_అధికప్రవాహం_CHECKING
    #నిర్వచించు పోర్టు_HAS_దొంతర_అధికప్రవాహం_CHECKING    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే పోర్టు_ARCH_NAME
    #నిర్వచించు పోర్టు_ARCH_NAME    ఖాళీ
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే config_దొంతర_కేటాయింపు_FROM_SEPARATE_కుప్ప
    /* Defaults to 0 for backward compatibility. */
    #నిర్వచించు config_దొంతర_కేటాయింపు_FROM_SEPARATE_కుప్ప    0
#అయితే_ముగింపు

/* *INDENT-OFF* */
#నిర్వచితమైతే __cplusplus
    బాహ్య "C" {
#అయితే_ముగింపు
/* *INDENT-ON* */

#అన్తర్భావ "mpu_wrappers.h"

/*
 * Setup the stack of a new task so it is ready to be placed under the
 * scheduler control.  The registers have to be placed on the stack in
 * the order that the port expects to find them.
 *
 */
#ఒకవేళ ( పోర్టు_USING_MPU_WRAPPERS == 1 )
    #ఒకవేళ ( పోర్టు_HAS_దొంతర_అధికప్రవాహం_CHECKING == 1 )
        దొంతర_Type_t * px_పోర్టు_Initialise_దొంతర( దొంతర_Type_t * px_శిఖరం_యొక్క_దొంతర,
                                             దొంతర_Type_t * px_చివర_యొక్క_దొంతర,
                                             TaskFunction_t px_సంకేతం,
                                             శూన్య * pvParameters,
                                             పునాది_Type_t x_నిరంతరం_Privileged ) PRIVILEGED_FUNCTION;
    #లేకపోతే
        దొంతర_Type_t * px_పోర్టు_Initialise_దొంతర( దొంతర_Type_t * px_శిఖరం_యొక్క_దొంతర,
                                             TaskFunction_t px_సంకేతం,
                                             శూన్య * pvParameters,
                                             పునాది_Type_t x_నిరంతరం_Privileged ) PRIVILEGED_FUNCTION;
    #అయితే_ముగింపు
#లేకపోతే /* if ( portUSING_MPU_WRAPPERS == 1 ) */
    #ఒకవేళ ( పోర్టు_HAS_దొంతర_అధికప్రవాహం_CHECKING == 1 )
        దొంతర_Type_t * px_పోర్టు_Initialise_దొంతర( దొంతర_Type_t * px_శిఖరం_యొక్క_దొంతర,
                                             దొంతర_Type_t * px_చివర_యొక్క_దొంతర,
                                             TaskFunction_t px_సంకేతం,
                                             శూన్య * pvParameters ) PRIVILEGED_FUNCTION;
    #లేకపోతే
        దొంతర_Type_t * px_పోర్టు_Initialise_దొంతర( దొంతర_Type_t * px_శిఖరం_యొక్క_దొంతర,
                                             TaskFunction_t px_సంకేతం,
                                             శూన్య * pvParameters ) PRIVILEGED_FUNCTION;
    #అయితే_ముగింపు
#అయితే_ముగింపు /* if ( portUSING_MPU_WRAPPERS == 1 ) */

/* Used by heap_5.c to define the start address and size of each memory region
 * that together comprise the total FreeRTOS heap space. */
రకనిర్వచనం నిర్మాణం కుప్ప_Region
{
    uint8_t * puc_ప్రారంభం_చిరునామా;
    size_t x_కొలత_లోపల_బైట్లు;
} కుప్ప_Region_t;

/* Used to pass information about the heap out of vPortGetHeapStats(). */
రకనిర్వచనం నిర్మాణం x_కుప్ప_Stats
{
    size_t x_అందుబాటు_కుప్ప_Space_లోపల_బైట్లు;      /* The total heap size currently available - this is the sum of all the free blocks, not the largest block that can be allocated. */
    size_t x_కొలత_యొక్క_అతిపెద్ద_Free_ఖండం_లోపల_బైట్లు;  /* The maximum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t x_కొలత_యొక్క_అతిచిన్న_Free_ఖండం_లోపల_బైట్లు; /* The minimum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t x_సంఖ్య_యొక్క_Free_ఖండాలు;             /* The number of free memory blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t x_Minimum_Ever_Free_బైట్లు_మిగిలిన;  /* The minimum amount of total free memory (sum of all free blocks) there has been in the heap since the system booted. */
    size_t x_సంఖ్య_యొక్క_Successful_Allocations;  /* The number of calls to pvPortMalloc() that have returned a valid memory block. */
    size_t x_సంఖ్య_యొక్క_Successful_Frees;        /* The number of calls to vPortFree() that has successfully freed a block of memory. */
} కుప్ప_Stats_t;

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
శూన్య v_పోర్టు_Define_కుప్ప_Regions( స్థిరరాశి కుప్ప_Region_t * స్థిరరాశి px_కుప్ప_Regions ) PRIVILEGED_FUNCTION;

/*
 * Returns a HeapStats_t structure filled with information about the current
 * heap state.
 */
శూన్య v_పోర్టు_Get_కుప్ప_Stats( కుప్ప_Stats_t * px_కుప్ప_Stats );

/*
 * Map to the memory management routines required for the port.
 */
శూన్య * pv_పోర్టు_Malloc( size_t x_కొలత ) PRIVILEGED_FUNCTION;
శూన్య * pv_పోర్టు_Calloc( size_t x_అంకె,
                     size_t x_కొలత ) PRIVILEGED_FUNCTION;
శూన్య v_పోర్టు_Free( శూన్య * pv ) PRIVILEGED_FUNCTION;
శూన్య v_పోర్టు_Initialise_ఖండాలు( శూన్య ) PRIVILEGED_FUNCTION;
size_t x_పోర్టు_Get_Free_కుప్ప_కొలత( శూన్య ) PRIVILEGED_FUNCTION;
size_t x_పోర్టు_Get_Minimum_Ever_Free_కుప్ప_కొలత( శూన్య ) PRIVILEGED_FUNCTION;

#ఒకవేళ ( config_దొంతర_కేటాయింపు_FROM_SEPARATE_కుప్ప == 1 )
    శూన్య * pv_పోర్టు_Malloc_దొంతర( size_t x_కొలత ) PRIVILEGED_FUNCTION;
    శూన్య v_పోర్టు_Free_దొంతర( శూన్య * pv ) PRIVILEGED_FUNCTION;
#లేకపోతే
    #నిర్వచించు pv_పోర్టు_Malloc_దొంతర    pv_పోర్టు_Malloc
    #నిర్వచించు v_పోర్టు_Free_దొంతర       v_పోర్టు_Free
#అయితే_ముగింపు

#ఒకవేళ ( configUSE_MALLOC_FAILED_HOOK == 1 )

/**
 * task.h
 * @code{c}
 * void vApplicationMallocFailedHook( void )
 * @endcode
 *
 * This hook function is called when allocation failed.
 */
    శూన్య vApplicationMallocFailedHook( శూన్య ); /*lint !e526 Symbol not defined as it is an application callback. */
#అయితే_ముగింపు

/*
 * Setup the hardware ready for the scheduler to take control.  This generally
 * sets up a tick interrupt and sets timers for the correct tick frequency.
 */
పునాది_Type_t x_పోర్టు_ప్రారంభం_Scheduler( శూన్య ) PRIVILEGED_FUNCTION;

/*
 * Undo any hardware/ISR setup that was performed by xPortStartScheduler() so
 * the hardware is left in its original condition after the scheduler stops
 * executing.
 */
శూన్య v_పోర్టు_చివర_Scheduler( శూన్య ) PRIVILEGED_FUNCTION;

/*
 * The structures and methods of manipulating the MPU are contained within the
 * port layer.
 *
 * Fills the xMPUSettings structure with the memory region information
 * contained in xRegions.
 */
#ఒకవేళ ( పోర్టు_USING_MPU_WRAPPERS == 1 )
    నిర్మాణం x_జ్ఞాపకం_REGION;
    శూన్య v_పోర్టు_Store_Task_MPUSettings( xMPU_SETTINGS * xMPUSettings,
                                    స్థిరరాశి నిర్మాణం x_జ్ఞాపకం_REGION * స్థిరరాశి xRegions,
                                    దొంతర_Type_t * px_Bottom_యొక్క_దొంతర,
                                    uint32_t ul_దొంతర_లోతు ) PRIVILEGED_FUNCTION;
#అయితే_ముగింపు

/* *INDENT-OFF* */
#నిర్వచితమైతే __cplusplus
    }
#అయితే_ముగింపు
/* *INDENT-ON* */

#అయితే_ముగింపు /* PORTABLE_H */
