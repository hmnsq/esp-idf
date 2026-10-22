/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ଅନ୍ତର୍ଭାଵ "sdkconfig.h"

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

#ଅନ୍ତର୍ଭାଵ "FreeRTOS.h"

#undef MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#ଯଦି ( configSUPPORT_DYNAMIC_ALLOCATION == 0 )
#error This file must not be used ଯଦି configSUPPORT_DYNAMIC_ALLOCATION is 0
#endif

#ଅନ୍ତର୍ଭାଵ "esp_heap_caps.h"

#ଯଦି !CONFIG_IDF_TARGET_LINUX
/* Memory util functions are not implemented in the Linux simulator */
#ଅନ୍ତର୍ଭାଵ "esp_memory_utils.h"
#endif /* CONFIG_IDF_TARGET_LINUX */

#define portFREERTOS_HEAP_CAPS    ( MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT )

/*-----------------------------------------------------------*/

ଶୂନ୍ୟ * pvPortMalloc(size_t xWantedSize)
{
    ଶୂନ୍ୟ * pvReturn = ଖାଲି;

    /* All dynamic allocation done by FreeRTOS goes through this function. If
     * users need to allocate FreeRTOS objects into external RAM, they should
     * use the "static" equivalents of FreeRTOS API to create FreeRTOS objects
     * (e.g., queues). */
    pvReturn = heap_caps_malloc(xWantedSize, portFREERTOS_HEAP_CAPS);

    ଫେରନ୍ତୁ pvReturn;
}
/*-----------------------------------------------------------*/

ଶୂନ୍ୟ vPortFree(ଶୂନ୍ୟ * pv)
{
    heap_caps_free(pv);
}
/*-----------------------------------------------------------*/

size_t xPortGetFreeHeapSize(ଶୂନ୍ୟ)
{
    ଫେରନ୍ତୁ heap_caps_get_free_size(portFREERTOS_HEAP_CAPS);
}
/*-----------------------------------------------------------*/

size_t xPortGetMinimumEverFreeHeapSize(ଶୂନ୍ୟ)
{
    ଫେରନ୍ତୁ heap_caps_get_minimum_free_size(portFREERTOS_HEAP_CAPS);
}
/*-----------------------------------------------------------*/

bool xPortCheckValidListMem(ଅଚଳ ଶୂନ୍ୟ * ptr)
{
#ଯଦି CONFIG_IDF_TARGET_LINUX
    ଫେରନ୍ତୁ true;
#ଅନ୍ୟଥା /* CONFIG_IDF_TARGET_LINUX */
    ଫେରନ୍ତୁ esp_ptr_internal(ptr) && esp_ptr_byte_accessible(ptr);
#endif /* CONFIG_IDF_TARGET_LINUX */
}

bool xPortCheckValidTCBMem(ଅଚଳ ଶୂନ୍ୟ * ptr)
{
#ଯଦି CONFIG_IDF_TARGET_LINUX
    ଫେରନ୍ତୁ true;
#ଅନ୍ୟଥା /* CONFIG_IDF_TARGET_LINUX */
    ଫେରନ୍ତୁ esp_ptr_internal(ptr) && esp_ptr_byte_accessible(ptr);
#endif /* CONFIG_IDF_TARGET_LINUX */
}

#ଯଦି CONFIG_FREERTOS_PLACE_TASK_STACKS_IN_EXT_RAM
ଶୂନ୍ୟ * pv_Port_Malloc_ଗଦା(size_t xWantedSize)
{
    ଫେରନ୍ତୁ heap_caps_malloc_prefer(xWantedSize, 2, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
}

ଶୂନ୍ୟ v_Port_Free_ଗଦା(ଶୂନ୍ୟ * pv)
{
    heap_caps_free(pv);
}
#endif /* CONFIG_FREERTOS_PLACE_TASK_STACKS_IN_EXT_RAM */

bool x_Portcheck_Valid_ଗଦା_Mem(ଅଚଳ ଶୂନ୍ୟ * ptr)
{
#ଯଦି CONFIG_IDF_TARGET_LINUX
    ଫେରନ୍ତୁ true;
#ଅନ୍ୟଥା /* CONFIG_IDF_TARGET_LINUX */
#ifdef CONFIG_FREERTOS_TASK_CREATE_ALLOW_EXT_MEM
    ଫେରନ୍ତୁ esp_ptr_byte_accessible(ptr);
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ esp_ptr_internal(ptr) && esp_ptr_byte_accessible(ptr);
#endif
#endif /* CONFIG_IDF_TARGET_LINUX */
}
