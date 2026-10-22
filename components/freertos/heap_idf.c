/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "sdkconfig.h"

/* This file implements the heap related functions that are called by FreeRTOS.
 * ESP-IDF provides its own heap containing memory with different capabilities
 * (see esp_heap_caps.h). Thus, this file maps a subset of the ESP-IDF heap to
 * act as the FreeRTOS heap.
 *
 * All dynamic allocation done by FreeRTOS should be placed in internal 8-bit
 * accessible RAM (i.e., using the MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT flags).
 * This is due to the fact that FreeRTOS objects (e.g., task stacks, TCBs,
 * queues etc) must be accessible even if the cache is disabled. Therefore, the
 * heap that is made available to FreeRTOS for dynamic allocation is a subset of
 * the ESP-IDF heap (where all MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT memory is
 * made available to FreeRTOS for dynamic allocation).
 */

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#ضمن "FreeRTOS.h"

#undef MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#إذا ( configSUPPORT_DYNAMIC_ALLOCATION == 0 )
#error This file must not be used إذا configSUPPORT_DYNAMIC_ALLOCATION is 0
#endif

#ضمن "esp_heap_caps.h"

#إذا !CONFIG_IDF_TARGET_LINUX
/* Memory util functions are not implemented in the Linux simulator */
#ضمن "esp_memory_utils.h"
#endif /* CONFIG_IDF_TARGET_LINUX */

#define portFREERTOS_HEAP_CAPS    ( MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT )

/*-----------------------------------------------------------*/

فراغ * pvPortMalloc(size_t xWantedSize)
{
    فراغ * pvReturn = عدم;

    /* All dynamic allocation done by FreeRTOS goes through this function. If
     * users need to allocate FreeRTOS objects into external RAM, they should
     * use the "static" equivalents of FreeRTOS API to create FreeRTOS objects
     * (e.g., queues). */
    pvReturn = heap_caps_malloc(xWantedSize, portFREERTOS_HEAP_CAPS);

    أرجع pvReturn;
}
/*-----------------------------------------------------------*/

فراغ vPortFree(فراغ * pv)
{
    heap_caps_free(pv);
}
/*-----------------------------------------------------------*/

size_t xPortGetFreeHeapSize(فراغ)
{
    أرجع heap_caps_get_free_size(portFREERTOS_HEAP_CAPS);
}
/*-----------------------------------------------------------*/

size_t xPortGetMinimumEverFreeHeapSize(فراغ)
{
    أرجع heap_caps_get_minimum_free_size(portFREERTOS_HEAP_CAPS);
}
/*-----------------------------------------------------------*/

bool xPortCheckValidListMem(ثابت فراغ * ptr)
{
#إذا CONFIG_IDF_TARGET_LINUX
    أرجع true;
#وإلا /* CONFIG_IDF_TARGET_LINUX */
    أرجع esp_ptr_internal(ptr) && esp_ptr_byte_accessible(ptr);
#endif /* CONFIG_IDF_TARGET_LINUX */
}

bool xPortCheckValidTCBMem(ثابت فراغ * ptr)
{
#إذا CONFIG_IDF_TARGET_LINUX
    أرجع true;
#وإلا /* CONFIG_IDF_TARGET_LINUX */
    أرجع esp_ptr_internal(ptr) && esp_ptr_byte_accessible(ptr);
#endif /* CONFIG_IDF_TARGET_LINUX */
}

#إذا CONFIG_FREERTOS_PLACE_TASK_STACKS_IN_EXT_RAM
فراغ * pv_Port_Malloc_مكدس(size_t xWantedSize)
{
    أرجع heap_caps_malloc_prefer(xWantedSize, 2, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
}

فراغ v_Port_Free_مكدس(فراغ * pv)
{
    heap_caps_free(pv);
}
#endif /* CONFIG_FREERTOS_PLACE_TASK_STACKS_IN_EXT_RAM */

bool x_Portcheck_Valid_مكدس_Mem(ثابت فراغ * ptr)
{
#إذا CONFIG_IDF_TARGET_LINUX
    أرجع true;
#وإلا /* CONFIG_IDF_TARGET_LINUX */
#ifdef CONFIG_FREERTOS_TASK_CREATE_ALLOW_EXT_MEM
    أرجع esp_ptr_byte_accessible(ptr);
#وإلا
    أرجع esp_ptr_internal(ptr) && esp_ptr_byte_accessible(ptr);
#endif
#endif /* CONFIG_IDF_TARGET_LINUX */
}
