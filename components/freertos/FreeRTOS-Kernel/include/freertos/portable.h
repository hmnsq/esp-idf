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

#अगर_अघोषित PORTABLE_H
#घोषणा PORTABLE_H

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
#समावेश "deprecated_definitions.h"

/* If portENTER_CRITICAL is not defined then including deprecated_definitions.h
 * did not result in a portmacro.h header file being included - and it should be
 * included here.  In this case the path to the correct portmacro.h header file
 * must be set in the compiler's include path. */
#अगर_अघोषित portENTER_CRITICAL
    #समावेश "portmacro.h"
#पूर्ण_अगर

#अगर portBYTE_ALIGNMENT == 32
    #घोषणा portBYTE_ALIGNMENT_MASK    ( 0x001f )
#या_अगर portBYTE_ALIGNMENT == 16
    #घोषणा portBYTE_ALIGNMENT_MASK    ( 0x000f )
#या_अगर portBYTE_ALIGNMENT == 8
    #घोषणा portBYTE_ALIGNMENT_MASK    ( 0x0007 )
#या_अगर portBYTE_ALIGNMENT == 4
    #घोषणा portBYTE_ALIGNMENT_MASK    ( 0x0003 )
#या_अगर portBYTE_ALIGNMENT == 2
    #घोषणा portBYTE_ALIGNMENT_MASK    ( 0x0001 )
#या_अगर portBYTE_ALIGNMENT == 1
    #घोषणा portBYTE_ALIGNMENT_MASK    ( 0x0000 )
#अन्यथा /* if portBYTE_ALIGNMENT == 32 */
    #त्रुटि "Invalid portBYTE_ALIGNMENT definition"
#पूर्ण_अगर /* if portBYTE_ALIGNMENT == 32 */

#अगर_अघोषित portUSING_MPU_WRAPPERS
    #घोषणा portUSING_MPU_WRAPPERS    0
#पूर्ण_अगर

#अगर_अघोषित port_अंक_CONFIGURABLE_REGIONS
    #घोषणा port_अंक_CONFIGURABLE_REGIONS    1
#पूर्ण_अगर

#अगर_अघोषित port_HAS_ढेर_OVERFLOW_CHECKING
    #घोषणा port_HAS_ढेर_OVERFLOW_CHECKING    0
#पूर्ण_अगर

#अगर_अघोषित portARCH_NAME
    #घोषणा portARCH_NAME    शून्य
#पूर्ण_अगर

#अगर_अघोषित config_ढेर_ALLOCATION_FROM_SEPARATE_HEAP
    /* Defaults to 0 for backward compatibility. */
    #घोषणा config_ढेर_ALLOCATION_FROM_SEPARATE_HEAP    0
#पूर्ण_अगर

/* *INDENT-OFF* */
#अगर_घोषित __cplusplus
    बाह्य "C" {
#पूर्ण_अगर
/* *INDENT-ON* */

#समावेश "mpu_wrappers.h"

/*
 * Setup the stack of a new task so it is ready to be placed under the
 * scheduler control.  The registers have to be placed on the stack in
 * the order that the port expects to find them.
 *
 */
#अगर ( portUSING_MPU_WRAPPERS == 1 )
    #अगर ( port_HAS_ढेर_OVERFLOW_CHECKING == 1 )
        ढेर_Type_t * px_Port_Initialise_ढेर( ढेर_Type_t * px_शीर्ष_Of_ढेर,
                                             ढेर_Type_t * px_End_Of_ढेर,
                                             TaskFunction_t pxCode,
                                             व्योम * pvParameters,
                                             BaseType_t xRunPrivileged ) PRIVILEGED_FUNCTION;
    #अन्यथा
        ढेर_Type_t * px_Port_Initialise_ढेर( ढेर_Type_t * px_शीर्ष_Of_ढेर,
                                             TaskFunction_t pxCode,
                                             व्योम * pvParameters,
                                             BaseType_t xRunPrivileged ) PRIVILEGED_FUNCTION;
    #पूर्ण_अगर
#अन्यथा /* if ( portUSING_MPU_WRAPPERS == 1 ) */
    #अगर ( port_HAS_ढेर_OVERFLOW_CHECKING == 1 )
        ढेर_Type_t * px_Port_Initialise_ढेर( ढेर_Type_t * px_शीर्ष_Of_ढेर,
                                             ढेर_Type_t * px_End_Of_ढेर,
                                             TaskFunction_t pxCode,
                                             व्योम * pvParameters ) PRIVILEGED_FUNCTION;
    #अन्यथा
        ढेर_Type_t * px_Port_Initialise_ढेर( ढेर_Type_t * px_शीर्ष_Of_ढेर,
                                             TaskFunction_t pxCode,
                                             व्योम * pvParameters ) PRIVILEGED_FUNCTION;
    #पूर्ण_अगर
#पूर्ण_अगर /* if ( portUSING_MPU_WRAPPERS == 1 ) */

/* Used by heap_5.c to define the start address and size of each memory region
 * that together comprise the total FreeRTOS heap space. */
प्रकार काष्ठा HeapRegion
{
    uint8_t * pucStartAddress;
    माप_प्रकार x_आकार_In_Bytes;
} HeapRegion_t;

/* Used to pass information about the heap out of vPortGetHeapStats(). */
प्रकार काष्ठा xHeapStats
{
    माप_प्रकार xAvailableHeapSpaceInBytes;      /* The total heap size currently available - this is the sum of all the free blocks, not the largest block that can be allocated. */
    माप_प्रकार x_आकार_Of_Largest_Free_Block_In_Bytes;  /* The maximum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
    माप_प्रकार x_आकार_Of_Smallest_Free_Block_In_Bytes; /* The minimum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
    माप_प्रकार x_संख्या_Of_Free_Blocks;             /* The number of free memory blocks within the heap at the time vPortGetHeapStats() is called. */
    माप_प्रकार xMinimumEverFreeBytesRemaining;  /* The minimum amount of total free memory (sum of all free blocks) there has been in the heap since the system booted. */
    माप_प्रकार x_संख्या_Of_Successful_Allocations;  /* The number of calls to pvPortMalloc() that have returned a valid memory block. */
    माप_प्रकार x_संख्या_Of_Successful_Frees;        /* The number of calls to vPortFree() that has successfully freed a block of memory. */
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
व्योम vPortDefineHeapRegions( स्थिर HeapRegion_t * स्थिर pxHeapRegions ) PRIVILEGED_FUNCTION;

/*
 * Returns a HeapStats_t structure filled with information about the current
 * heap state.
 */
व्योम vPortGetHeapStats( HeapStats_t * pxHeapStats );

/*
 * Map to the memory management routines required for the port.
 */
व्योम * pvPortMalloc( माप_प्रकार x_आकार ) PRIVILEGED_FUNCTION;
व्योम * pvPortCalloc( माप_प्रकार x_अंक,
                     माप_प्रकार x_आकार ) PRIVILEGED_FUNCTION;
व्योम vPortFree( व्योम * pv ) PRIVILEGED_FUNCTION;
व्योम vPortInitialiseBlocks( व्योम ) PRIVILEGED_FUNCTION;
माप_प्रकार x_Port_Get_Free_Heap_आकार( व्योम ) PRIVILEGED_FUNCTION;
माप_प्रकार x_Port_Get_Minimum_Ever_Free_Heap_आकार( व्योम ) PRIVILEGED_FUNCTION;

#अगर ( config_ढेर_ALLOCATION_FROM_SEPARATE_HEAP == 1 )
    व्योम * pv_Port_Malloc_ढेर( माप_प्रकार x_आकार ) PRIVILEGED_FUNCTION;
    व्योम v_Port_Free_ढेर( व्योम * pv ) PRIVILEGED_FUNCTION;
#अन्यथा
    #घोषणा pv_Port_Malloc_ढेर    pvPortMalloc
    #घोषणा v_Port_Free_ढेर       vPortFree
#पूर्ण_अगर

#अगर ( configUSE_MALLOC_FAILED_HOOK == 1 )

/**
 * task.h
 * @code{c}
 * void vApplicationMallocFailedHook( void )
 * @endcode
 *
 * This hook function is called when allocation failed.
 */
    व्योम vApplicationMallocFailedHook( व्योम ); /*lint !e526 Symbol not defined as it is an application callback. */
#पूर्ण_अगर

/*
 * Setup the hardware ready for the scheduler to take control.  This generally
 * sets up a tick interrupt and sets timers for the correct tick frequency.
 */
BaseType_t xPortStartScheduler( व्योम ) PRIVILEGED_FUNCTION;

/*
 * Undo any hardware/ISR setup that was performed by xPortStartScheduler() so
 * the hardware is left in its original condition after the scheduler stops
 * executing.
 */
व्योम vPortEndScheduler( व्योम ) PRIVILEGED_FUNCTION;

/*
 * The structures and methods of manipulating the MPU are contained within the
 * port layer.
 *
 * Fills the xMPUSettings structure with the memory region information
 * contained in xRegions.
 */
#अगर ( portUSING_MPU_WRAPPERS == 1 )
    काष्ठा xMEMORY_REGION;
    व्योम vPortStoreTaskMPUSettings( xMPU_SETTINGS * xMPUSettings,
                                    स्थिर काष्ठा xMEMORY_REGION * स्थिर xRegions,
                                    ढेर_Type_t * px_Bottom_Of_ढेर,
                                    uint32_t ul_ढेर_Depth ) PRIVILEGED_FUNCTION;
#पूर्ण_अगर

/* *INDENT-OFF* */
#अगर_घोषित __cplusplus
    }
#पूर्ण_अगर
/* *INDENT-ON* */

#पूर्ण_अगर /* PORTABLE_H */
