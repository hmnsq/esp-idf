/*
 * FreeRTOS Kernel V11.1.0
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * SPDX-FileCopyrightText: 2021 Amazon.com, Inc. or its affiliates
 *
 * SPDX-License-Identifier: MIT
 *
 * SPDX-FileContributor: 2023-2024 Espressif Systems (Shanghai) CO LTD
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

#यदि_अपरिभाषितम् PORTABLE_H
#परिभाषय PORTABLE_H

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
#अन्तर्भाव "deprecated_definitions.h"

/* If portENTER_CRITICAL is not defined then including deprecated_definitions.h
 * did not result in a portmacro.h header file being included - and it should be
 * included here.  In this case the path to the correct portmacro.h header file
 * must be set in the compiler's include path. */
#यदि_अपरिभाषितम् पत्तनम्_ENTER_CRITICAL
    #अन्तर्भाव "portmacro.h"
#यदि_अन्तः

#यदि पत्तनम्_BYTE_ALIGNMENT == 32
    #परिभाषय पत्तनम्_BYTE_ALIGNMENT_आवरणम्    ( 0x001f )
#नोचेद्यदि पत्तनम्_BYTE_ALIGNMENT == 16
    #परिभाषय पत्तनम्_BYTE_ALIGNMENT_आवरणम्    ( 0x000f )
#नोचेद्यदि पत्तनम्_BYTE_ALIGNMENT == 8
    #परिभाषय पत्तनम्_BYTE_ALIGNMENT_आवरणम्    ( 0x0007 )
#नोचेद्यदि पत्तनम्_BYTE_ALIGNMENT == 4
    #परिभाषय पत्तनम्_BYTE_ALIGNMENT_आवरणम्    ( 0x0003 )
#नोचेद्यदि पत्तनम्_BYTE_ALIGNMENT == 2
    #परिभाषय पत्तनम्_BYTE_ALIGNMENT_आवरणम्    ( 0x0001 )
#नोचेद्यदि पत्तनम्_BYTE_ALIGNMENT == 1
    #परिभाषय पत्तनम्_BYTE_ALIGNMENT_आवरणम्    ( 0x0000 )
#अन्यथा /* if portBYTE_ALIGNMENT == 32 */
    #दोषः "Invalid portBYTE_ALIGNMENT definition"
#यदि_अन्तः /* if portBYTE_ALIGNMENT == 32 */

#यदि_अपरिभाषितम् पत्तनम्_USING_MPU_WRAPPERS
    #परिभाषय पत्तनम्_USING_MPU_WRAPPERS    0
#यदि_अन्तः

#यदि_अपरिभाषितम् पत्तनम्_अङ्कः_CONFIGURABLE_REGIONS
    #परिभाषय पत्तनम्_अङ्कः_CONFIGURABLE_REGIONS    1
#यदि_अन्तः

#यदि_अपरिभाषितम् पत्तनम्_HAS_राशिः_अतिप्रवाहः_CHECKING
    #परिभाषय पत्तनम्_HAS_राशिः_अतिप्रवाहः_CHECKING    0
#यदि_अन्तः

#यदि_अपरिभाषितम् पत्तनम्_ARCH_NAME
    #परिभाषय पत्तनम्_ARCH_NAME    रिक्त
#यदि_अन्तः

#यदि_अपरिभाषितम् config_राशिः_गभीरता_TYPE
    #परिभाषय config_राशिः_गभीरता_TYPE    राशिः_Type_t
#यदि_अन्तः

#यदि_अपरिभाषितम् config_राशिः_आवण्टनम्_FROM_SEPARATE_HEAP
    /* Defaults to 0 for backward compatibility. */
    #परिभाषय config_राशिः_आवण्टनम्_FROM_SEPARATE_HEAP    0
#यदि_अन्तः

/* *INDENT-OFF* */
#यदि_परिभाषितम् __cplusplus
    बाह्य "C" {
#यदि_अन्तः
/* *INDENT-ON* */

#अन्तर्भाव "mpu_wrappers.h"

/*
 * Setup the stack of a new task so it is ready to be placed under the
 * scheduler control.  The registers have to be placed on the stack in
 * the order that the port expects to find them.
 *
 */
#यदि ( पत्तनम्_USING_MPU_WRAPPERS == 1 )
    #यदि ( पत्तनम्_HAS_राशिः_अतिप्रवाहः_CHECKING == 1 )
        राशिः_Type_t * px_पत्तनम्_Initialise_राशिः( राशिः_Type_t * px_शिखरम्_सम्बन्धि_राशिः,
                                             राशिः_Type_t * px_अन्तः_सम्बन्धि_राशिः,
                                             TaskFunction_t px_संहिता,
                                             शून्य * pvParameters,
                                             आधारः_Type_t x_सततम्_Privileged,
                                             xMPU_SETTINGS * xMPUSettings ) PRIVILEGED_FUNCTION;
    #अन्यथा
        राशिः_Type_t * px_पत्तनम्_Initialise_राशिः( राशिः_Type_t * px_शिखरम्_सम्बन्धि_राशिः,
                                             TaskFunction_t px_संहिता,
                                             शून्य * pvParameters,
                                             आधारः_Type_t x_सततम्_Privileged,
                                             xMPU_SETTINGS * xMPUSettings ) PRIVILEGED_FUNCTION;
    #यदि_अन्तः /* if ( portHAS_STACK_OVERFLOW_CHECKING == 1 ) */
#अन्यथा /* if ( portUSING_MPU_WRAPPERS == 1 ) */
    #यदि ( पत्तनम्_HAS_राशिः_अतिप्रवाहः_CHECKING == 1 )
        राशिः_Type_t * px_पत्तनम्_Initialise_राशिः( राशिः_Type_t * px_शिखरम्_सम्बन्धि_राशिः,
                                             राशिः_Type_t * px_अन्तः_सम्बन्धि_राशिः,
                                             TaskFunction_t px_संहिता,
                                             शून्य * pvParameters ) PRIVILEGED_FUNCTION;
    #अन्यथा
        राशिः_Type_t * px_पत्तनम्_Initialise_राशिः( राशिः_Type_t * px_शिखरम्_सम्बन्धि_राशिः,
                                             TaskFunction_t px_संहिता,
                                             शून्य * pvParameters ) PRIVILEGED_FUNCTION;
    #यदि_अन्तः
#यदि_अन्तः /* if ( portUSING_MPU_WRAPPERS == 1 ) */

/* Used by heap_5.c to define the start address and size of each memory region
 * that together comprise the total FreeRTOS heap space. */
प्रकारनिर्देश संरचना HeapRegion
{
    uint8_t * puc_Start_स्थानाङ्कः;
    size_t x_आकार_अन्तर्_अष्टकानि;
} HeapRegion_t;

/* Used to pass information about the heap out of vPortGetHeapStats(). */
प्रकारनिर्देश संरचना xHeapStats
{
    size_t x_उपलब्धम्_Heap_Space_अन्तर्_अष्टकानि;      /* The total heap size currently available - this is the sum of all the free blocks, not the largest block that can be allocated. */
    size_t x_आकार_सम्बन्धि_Largest_Free_खण्डम्_अन्तर्_अष्टकानि;  /* The maximum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t x_आकार_सम्बन्धि_Smallest_Free_खण्डम्_अन्तर्_अष्टकानि; /* The minimum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t x_सङ्ख्या_सम्बन्धि_Free_खण्डानि;             /* The number of free memory blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t x_Minimum_Ever_Free_अष्टकानि_Remaining;  /* The minimum amount of total free memory (sum of all free blocks) there has been in the heap since the system booted. */
    size_t x_सङ्ख्या_सम्बन्धि_Successful_Allocations;  /* The number of calls to pvPortMalloc() that have returned a valid memory block. */
    size_t x_सङ्ख्या_सम्बन्धि_Successful_Frees;        /* The number of calls to vPortFree() that has successfully freed a block of memory. */
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
शून्य v_पत्तनम्_Define_Heap_Regions( ध्रुव HeapRegion_t * ध्रुव pxHeapRegions ) PRIVILEGED_FUNCTION;

/*
 * Returns a HeapStats_t structure filled with information about the current
 * heap state.
 */
शून्य v_पत्तनम्_Get_Heap_Stats( HeapStats_t * pxHeapStats );

/*
 * Map to the memory management routines required for the port.
 */
शून्य * pv_पत्तनम्_Malloc( size_t x_Wanted_आकार ) PRIVILEGED_FUNCTION;
शून्य * pv_पत्तनम्_Calloc( size_t x_अङ्कः,
                     size_t x_आकार ) PRIVILEGED_FUNCTION;
शून्य v_पत्तनम्_Free( शून्य * pv ) PRIVILEGED_FUNCTION;
शून्य v_पत्तनम्_Initialise_खण्डानि( शून्य ) PRIVILEGED_FUNCTION;
size_t x_पत्तनम्_Get_Free_Heap_आकार( शून्य ) PRIVILEGED_FUNCTION;
size_t x_पत्तनम्_Get_Minimum_Ever_Free_Heap_आकार( शून्य ) PRIVILEGED_FUNCTION;

#यदि ( config_राशिः_आवण्टनम्_FROM_SEPARATE_HEAP == 1 )
    शून्य * pv_पत्तनम्_Malloc_राशिः( size_t x_आकार ) PRIVILEGED_FUNCTION;
    शून्य v_पत्तनम्_Free_राशिः( शून्य * pv ) PRIVILEGED_FUNCTION;
#अन्यथा
    #परिभाषय pv_पत्तनम्_Malloc_राशिः    pv_पत्तनम्_Malloc
    #परिभाषय v_पत्तनम्_Free_राशिः       v_पत्तनम्_Free
#यदि_अन्तः

/*
 * This function resets the internal state of the heap module. It must be called
 * by the application before restarting the scheduler.
 */
शून्य v_पत्तनम्_Heap_Reset_State( शून्य ) PRIVILEGED_FUNCTION;

#यदि ( configUSE_MALLOC_FAILED_HOOK == 1 )

/**
 * task.h
 * @code{c}
 * void vApplicationMallocFailedHook( void )
 * @endcode
 *
 * This hook function is called when allocation failed.
 */
    शून्य vApplicationMallocFailedHook( शून्य );
#यदि_अन्तः

/*
 * Setup the hardware ready for the scheduler to take control.  This generally
 * sets up a tick interrupt and sets timers for the correct tick frequency.
 */
आधारः_Type_t x_पत्तनम्_Start_Scheduler( शून्य ) PRIVILEGED_FUNCTION;

/*
 * Undo any hardware/ISR setup that was performed by xPortStartScheduler() so
 * the hardware is left in its original condition after the scheduler stops
 * executing.
 */
शून्य v_पत्तनम्_अन्तः_Scheduler( शून्य ) PRIVILEGED_FUNCTION;

/*
 * The structures and methods of manipulating the MPU are contained within the
 * port layer.
 *
 * Fills the xMPUSettings structure with the memory region information
 * contained in xRegions.
 */
#यदि ( पत्तनम्_USING_MPU_WRAPPERS == 1 )
    संरचना x_स्मृतिः_REGION;
    शून्य v_पत्तनम्_Store_Task_MPUSettings( xMPU_SETTINGS * xMPUSettings,
                                    ध्रुव संरचना x_स्मृतिः_REGION * ध्रुव xRegions,
                                    राशिः_Type_t * px_Bottom_सम्बन्धि_राशिः,
                                    config_राशिः_गभीरता_TYPE ux_राशिः_गभीरता ) PRIVILEGED_FUNCTION;
#यदि_अन्तः

/**
 * @brief Checks if the calling task is authorized to access the given buffer.
 *
 * @param pvBuffer The buffer which the calling task wants to access.
 * @param ulBufferLength The length of the pvBuffer.
 * @param ulAccessRequested The permissions that the calling task wants.
 *
 * @return pdTRUE if the calling task is authorized to access the buffer,
 *         pdFALSE otherwise.
 */
#यदि ( पत्तनम्_USING_MPU_WRAPPERS == 1 )
    आधारः_Type_t x_पत्तनम्_Is_Authorized_गन्तव्यम्_Access_बफर( ध्रुव शून्य * pv_बफर,
                                                uint32_t ul_बफर_दीर्घता,
                                                uint32_t ulAccessRequested ) PRIVILEGED_FUNCTION;
#यदि_अन्तः

/**
 * @brief Checks if the calling task is authorized to access the given kernel object.
 *
 * @param lInternalIndexOfKernelObject The index of the kernel object in the kernel
 *                                     object handle pool.
 *
 * @return pdTRUE if the calling task is authorized to access the kernel object,
 *         pdFALSE otherwise.
 */
#यदि ( ( पत्तनम्_USING_MPU_WRAPPERS == 1 ) && ( configUSE_MPU_WRAPPERS_V1 == 0 ) )

    आधारः_Type_t x_पत्तनम्_Is_Authorized_गन्तव्यम्_Access_Kernel_Object( int32_t l_आन्तरिकम्_सूचकांक_सम्बन्धि_Kernel_Object ) PRIVILEGED_FUNCTION;

#यदि_अन्तः

/* *INDENT-OFF* */
#यदि_परिभाषितम् __cplusplus
    }
#यदि_अन्तः
/* *INDENT-ON* */

#यदि_अन्तः /* PORTABLE_H */
