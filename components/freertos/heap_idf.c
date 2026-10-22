/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#अन्तर्भाव "sdkconfig.h"

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

#अन्तर्भाव "FreeRTOS.h"

#undef MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#जर ( configSUPPORT_DYNAMIC_ALLOCATION == 0 )
#error This file must not be used जर configSUPPORT_DYNAMIC_ALLOCATION is 0
#endif

#अन्तर्भाव "esp_heap_caps.h"

#जर !CONFIG_IDF_TARGET_LINUX
/* Memory util functions are not implemented in the Linux simulator */
#अन्तर्भाव "esp_memory_utils.h"
#endif /* CONFIG_IDF_TARGET_LINUX */

#define portFREERTOS_HEAP_CAPS    ( MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT )

/*-----------------------------------------------------------*/

शून्य * pvPortMalloc(माप_प्रकार xWantedSize)
{
    शून्य * pvReturn = रिक्त;

    /* All dynamic allocation done by FreeRTOS goes through this function. If
     * users need to allocate FreeRTOS objects into external RAM, they should
     * use the "static" equivalents of FreeRTOS API to create FreeRTOS objects
     * (e.g., queues). */
    pvReturn = heap_caps_malloc(xWantedSize, portFREERTOS_HEAP_CAPS);

    परत pvReturn;
}
/*-----------------------------------------------------------*/

शून्य vPortFree(शून्य * pv)
{
    heap_caps_free(pv);
}
/*-----------------------------------------------------------*/

माप_प्रकार xPortGetFreeHeapSize(शून्य)
{
    परत heap_caps_get_free_size(portFREERTOS_HEAP_CAPS);
}
/*-----------------------------------------------------------*/

माप_प्रकार xPortGetMinimumEverFreeHeapSize(शून्य)
{
    परत heap_caps_get_minimum_free_size(portFREERTOS_HEAP_CAPS);
}
/*-----------------------------------------------------------*/

bool xPortCheckValidListMem(स्थिरांक शून्य * ptr)
{
#जर CONFIG_IDF_TARGET_LINUX
    परत true;
#अन्यथा /* CONFIG_IDF_TARGET_LINUX */
    परत esp_ptr_internal(ptr) && esp_ptr_byte_accessible(ptr);
#endif /* CONFIG_IDF_TARGET_LINUX */
}

bool xPortCheckValidTCBMem(स्थिरांक शून्य * ptr)
{
#जर CONFIG_IDF_TARGET_LINUX
    परत true;
#अन्यथा /* CONFIG_IDF_TARGET_LINUX */
    परत esp_ptr_internal(ptr) && esp_ptr_byte_accessible(ptr);
#endif /* CONFIG_IDF_TARGET_LINUX */
}

#जर CONFIG_FREERTOS_PLACE_TASK_STACKS_IN_EXT_RAM
शून्य * pv_Port_Malloc_चळत(माप_प्रकार xWantedSize)
{
    परत heap_caps_malloc_prefer(xWantedSize, 2, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
}

शून्य v_Port_Free_चळत(शून्य * pv)
{
    heap_caps_free(pv);
}
#endif /* CONFIG_FREERTOS_PLACE_TASK_STACKS_IN_EXT_RAM */

bool x_Portcheck_Valid_चळत_Mem(स्थिरांक शून्य * ptr)
{
#जर CONFIG_IDF_TARGET_LINUX
    परत true;
#अन्यथा /* CONFIG_IDF_TARGET_LINUX */
#ifdef CONFIG_FREERTOS_TASK_CREATE_ALLOW_EXT_MEM
    परत esp_ptr_byte_accessible(ptr);
#अन्यथा
    परत esp_ptr_internal(ptr) && esp_ptr_byte_accessible(ptr);
#endif
#endif /* CONFIG_IDF_TARGET_LINUX */
}
