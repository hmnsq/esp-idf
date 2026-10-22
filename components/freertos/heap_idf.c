/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#समावेश "sdkconfig.h"

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
#घोषणा MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#समावेश "FreeRTOS.h"

#अघोषित MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#अगर ( configSUPPORT_DYNAMIC_ALLOCATION == 0 )
#त्रुटि This file must not be used अगर configSUPPORT_DYNAMIC_ALLOCATION is 0
#पूर्ण_अगर

#समावेश "esp_heap_caps.h"

#अगर !CONFIG_IDF_TARGET_LINUX
/* Memory util functions are not implemented in the Linux simulator */
#समावेश "esp_memory_utils.h"
#पूर्ण_अगर /* CONFIG_IDF_TARGET_LINUX */

#घोषणा portFREERTOS_HEAP_CAPS    ( MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT )

/*-----------------------------------------------------------*/

व्योम * pvPortMalloc(माप_प्रकार x_Wanted_आकार)
{
    व्योम * pvReturn = शून्य;

    /* All dynamic allocation done by FreeRTOS goes through this function. If
     * users need to allocate FreeRTOS objects into external RAM, they should
     * use the "static" equivalents of FreeRTOS API to create FreeRTOS objects
     * (e.g., queues). */
    pvReturn = heap_caps_malloc(x_Wanted_आकार, portFREERTOS_HEAP_CAPS);

    वापस pvReturn;
}
/*-----------------------------------------------------------*/

व्योम vPortFree(व्योम * pv)
{
    heap_caps_free(pv);
}
/*-----------------------------------------------------------*/

माप_प्रकार x_Port_Get_Free_Heap_आकार(व्योम)
{
    वापस heap_caps_get_free_आकार(portFREERTOS_HEAP_CAPS);
}
/*-----------------------------------------------------------*/

माप_प्रकार x_Port_Get_Minimum_Ever_Free_Heap_आकार(व्योम)
{
    वापस heap_caps_get_minimum_free_आकार(portFREERTOS_HEAP_CAPS);
}
/*-----------------------------------------------------------*/

bool xPortCheckValidListMem(स्थिर व्योम * ptr)
{
#अगर CONFIG_IDF_TARGET_LINUX
    वापस true;
#अन्यथा /* CONFIG_IDF_TARGET_LINUX */
    वापस esp_ptr_internal(ptr) && esp_ptr_byte_accessible(ptr);
#पूर्ण_अगर /* CONFIG_IDF_TARGET_LINUX */
}

bool xPortCheckValidTCBMem(स्थिर व्योम * ptr)
{
#अगर CONFIG_IDF_TARGET_LINUX
    वापस true;
#अन्यथा /* CONFIG_IDF_TARGET_LINUX */
    वापस esp_ptr_internal(ptr) && esp_ptr_byte_accessible(ptr);
#पूर्ण_अगर /* CONFIG_IDF_TARGET_LINUX */
}

#अगर CONFIG_FREERTOS_PLACE_TASK_STACKS_IN_EXT_RAM
व्योम * pv_Port_Malloc_ढेर(माप_प्रकार x_Wanted_आकार)
{
    वापस heap_caps_malloc_prefer(x_Wanted_आकार, 2, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
}

व्योम v_Port_Free_ढेर(व्योम * pv)
{
    heap_caps_free(pv);
}
#पूर्ण_अगर /* CONFIG_FREERTOS_PLACE_TASK_STACKS_IN_EXT_RAM */

bool x_Portcheck_Valid_ढेर_Mem(स्थिर व्योम * ptr)
{
#अगर CONFIG_IDF_TARGET_LINUX
    वापस true;
#अन्यथा /* CONFIG_IDF_TARGET_LINUX */
#अगर_घोषित CONFIG_FREERTOS_TASK_CREATE_ALLOW_EXT_MEM
    वापस esp_ptr_byte_accessible(ptr);
#अन्यथा
    वापस esp_ptr_internal(ptr) && esp_ptr_byte_accessible(ptr);
#पूर्ण_अगर
#पूर्ण_अगर /* CONFIG_IDF_TARGET_LINUX */
}
