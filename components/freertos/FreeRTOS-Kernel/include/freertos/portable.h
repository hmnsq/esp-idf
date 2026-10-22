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

#ifndef PORTABLE_H
#define PORTABLE_H

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
#અન્તર્ભાવ "deprecated_definitions.h"

/* If portENTER_CRITICAL is not defined then including deprecated_definitions.h
 * did not result in a portmacro.h header file being included - and it should be
 * included here.  In this case the path to the correct portmacro.h header file
 * must be set in the compiler's include path. */
#ifndef portENTER_CRITICAL
    #અન્તર્ભાવ "portmacro.h"
#endif

#જો portBYTE_ALIGNMENT == 32
    #define portBYTE_ALIGNMENT_MASK    ( 0x001f )
#અથવાજો portBYTE_ALIGNMENT == 16
    #define portBYTE_ALIGNMENT_MASK    ( 0x000f )
#અથવાજો portBYTE_ALIGNMENT == 8
    #define portBYTE_ALIGNMENT_MASK    ( 0x0007 )
#અથવાજો portBYTE_ALIGNMENT == 4
    #define portBYTE_ALIGNMENT_MASK    ( 0x0003 )
#અથવાજો portBYTE_ALIGNMENT == 2
    #define portBYTE_ALIGNMENT_MASK    ( 0x0001 )
#અથવાજો portBYTE_ALIGNMENT == 1
    #define portBYTE_ALIGNMENT_MASK    ( 0x0000 )
#નહીંતર /* if portBYTE_ALIGNMENT == 32 */
    #error "Invalid portBYTE_ALIGNMENT definition"
#endif /* if portBYTE_ALIGNMENT == 32 */

#ifndef portUSING_MPU_WRAPPERS
    #define portUSING_MPU_WRAPPERS    0
#endif

#ifndef port_અંક_CONFIGURABLE_REGIONS
    #define port_અંક_CONFIGURABLE_REGIONS    1
#endif

#ifndef port_HAS_ઢગલો_OVERFLOW_CHECKING
    #define port_HAS_ઢગલો_OVERFLOW_CHECKING    0
#endif

#ifndef portARCH_NAME
    #define portARCH_NAME    ખાલી
#endif

#ifndef config_ઢગલો_ALLOCATION_FROM_SEPARATE_HEAP
    /* Defaults to 0 for backward compatibility. */
    #define config_ઢગલો_ALLOCATION_FROM_SEPARATE_HEAP    0
#endif

/* *INDENT-OFF* */
#ifdef __cplusplus
    બાહ્ય "C" {
#endif
/* *INDENT-ON* */

#અન્તર્ભાવ "mpu_wrappers.h"

/*
 * Setup the stack of a new task so it is ready to be placed under the
 * scheduler control.  The registers have to be placed on the stack in
 * the order that the port expects to find them.
 *
 */
#જો ( portUSING_MPU_WRAPPERS == 1 )
    #જો ( port_HAS_ઢગલો_OVERFLOW_CHECKING == 1 )
        ઢગલો_Type_t * px_Port_Initialise_ઢગલો( ઢગલો_Type_t * px_ટોચ_Of_ઢગલો,
                                             ઢગલો_Type_t * px_End_Of_ઢગલો,
                                             TaskFunction_t pxCode,
                                             શૂન્ય * pvParameters,
                                             BaseType_t xRunPrivileged ) PRIVILEGED_FUNCTION;
    #નહીંતર
        ઢગલો_Type_t * px_Port_Initialise_ઢગલો( ઢગલો_Type_t * px_ટોચ_Of_ઢગલો,
                                             TaskFunction_t pxCode,
                                             શૂન્ય * pvParameters,
                                             BaseType_t xRunPrivileged ) PRIVILEGED_FUNCTION;
    #endif
#નહીંતર /* if ( portUSING_MPU_WRAPPERS == 1 ) */
    #જો ( port_HAS_ઢગલો_OVERFLOW_CHECKING == 1 )
        ઢગલો_Type_t * px_Port_Initialise_ઢગલો( ઢગલો_Type_t * px_ટોચ_Of_ઢગલો,
                                             ઢગલો_Type_t * px_End_Of_ઢગલો,
                                             TaskFunction_t pxCode,
                                             શૂન્ય * pvParameters ) PRIVILEGED_FUNCTION;
    #નહીંતર
        ઢગલો_Type_t * px_Port_Initialise_ઢગલો( ઢગલો_Type_t * px_ટોચ_Of_ઢગલો,
                                             TaskFunction_t pxCode,
                                             શૂન્ય * pvParameters ) PRIVILEGED_FUNCTION;
    #endif
#endif /* if ( portUSING_MPU_WRAPPERS == 1 ) */

/* Used by heap_5.c to define the start address and size of each memory region
 * that together comprise the total FreeRTOS heap space. */
પ્રકારવ્યાખ્યા સંરચના HeapRegion
{
    uint8_t * pucStartAddress;
    size_t xSizeInBytes;
} HeapRegion_t;

/* Used to pass information about the heap out of vPortGetHeapStats(). */
પ્રકારવ્યાખ્યા સંરચના xHeapStats
{
    size_t xAvailableHeapSpaceInBytes;      /* The total heap size currently available - this is the sum of all the free blocks, not the largest block that can be allocated. */
    size_t xSizeOfLargestFreeBlockInBytes;  /* The maximum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t xSizeOfSmallestFreeBlockInBytes; /* The minimum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t x_સંખ્યા_Of_Free_Blocks;             /* The number of free memory blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t xMinimumEverFreeBytesRemaining;  /* The minimum amount of total free memory (sum of all free blocks) there has been in the heap since the system booted. */
    size_t x_સંખ્યા_Of_Successful_Allocations;  /* The number of calls to pvPortMalloc() that have returned a valid memory block. */
    size_t x_સંખ્યા_Of_Successful_Frees;        /* The number of calls to vPortFree() that has successfully freed a block of memory. */
} HeapStats_t;

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
શૂન્ય vPortDefineHeapRegions( અચલ HeapRegion_t * અચલ pxHeapRegions ) PRIVILEGED_FUNCTION;

/*
 * Returns a HeapStats_t structure filled with information about the current
 * heap state.
 */
શૂન્ય vPortGetHeapStats( HeapStats_t * pxHeapStats );

/*
 * Map to the memory management routines required for the port.
 */
શૂન્ય * pvPortMalloc( size_t xSize ) PRIVILEGED_FUNCTION;
શૂન્ય * pvPortCalloc( size_t x_અંક,
                     size_t xSize ) PRIVILEGED_FUNCTION;
શૂન્ય vPortFree( શૂન્ય * pv ) PRIVILEGED_FUNCTION;
શૂન્ય vPortInitialiseBlocks( શૂન્ય ) PRIVILEGED_FUNCTION;
size_t xPortGetFreeHeapSize( શૂન્ય ) PRIVILEGED_FUNCTION;
size_t xPortGetMinimumEverFreeHeapSize( શૂન્ય ) PRIVILEGED_FUNCTION;

#જો ( config_ઢગલો_ALLOCATION_FROM_SEPARATE_HEAP == 1 )
    શૂન્ય * pv_Port_Malloc_ઢગલો( size_t xSize ) PRIVILEGED_FUNCTION;
    શૂન્ય v_Port_Free_ઢગલો( શૂન્ય * pv ) PRIVILEGED_FUNCTION;
#નહીંતર
    #define pv_Port_Malloc_ઢગલો    pvPortMalloc
    #define v_Port_Free_ઢગલો       vPortFree
#endif

#જો ( configUSE_MALLOC_FAILED_HOOK == 1 )

/**
 * task.h
 * @code{c}
 * void vApplicationMallocFailedHook( void )
 * @endcode
 *
 * This hook function is called when allocation failed.
 */
    શૂન્ય vApplicationMallocFailedHook( શૂન્ય ); /*lint !e526 Symbol not defined as it is an application callback. */
#endif

/*
 * Setup the hardware ready for the scheduler to take control.  This generally
 * sets up a tick interrupt and sets timers for the correct tick frequency.
 */
BaseType_t xPortStartScheduler( શૂન્ય ) PRIVILEGED_FUNCTION;

/*
 * Undo any hardware/ISR setup that was performed by xPortStartScheduler() so
 * the hardware is left in its original condition after the scheduler stops
 * executing.
 */
શૂન્ય vPortEndScheduler( શૂન્ય ) PRIVILEGED_FUNCTION;

/*
 * The structures and methods of manipulating the MPU are contained within the
 * port layer.
 *
 * Fills the xMPUSettings structure with the memory region information
 * contained in xRegions.
 */
#જો ( portUSING_MPU_WRAPPERS == 1 )
    સંરચના xMEMORY_REGION;
    શૂન્ય vPortStoreTaskMPUSettings( xMPU_SETTINGS * xMPUSettings,
                                    અચલ સંરચના xMEMORY_REGION * અચલ xRegions,
                                    ઢગલો_Type_t * px_Bottom_Of_ઢગલો,
                                    uint32_t ul_ઢગલો_Depth ) PRIVILEGED_FUNCTION;
#endif

/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */

#endif /* PORTABLE_H */
