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
#परिभाषय MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#अन्तर्भाव "FreeRTOS.h"

#अपरिभाषय MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#यदि ( config_SUPPORT_DYNAMIC_आवण्टनम् == 0 )
#दोषः This file must not be प्रयुक्तम् यदि config_SUPPORT_DYNAMIC_आवण्टनम् is 0
#यदि_अन्तः

#अन्तर्भाव "esp_heap_caps.h"

#यदि !CONFIG_IDF_TARGET_LINUX
/* Memory util functions are not implemented in the Linux simulator */
#अन्तर्भाव "esp_memory_utils.h"
#यदि_अन्तः /* CONFIG_IDF_TARGET_LINUX */

#परिभाषय पत्तनम्_FREERTOS_HEAP_CAPS    ( MALLOC_CAP_आन्तरिकम् | MALLOC_CAP_8BIT )

/*-----------------------------------------------------------*/

शून्य * pv_पत्तनम्_Malloc(size_t x_Wanted_आकार)
{
    शून्य * pvReturn = रिक्त;

    /* All dynamic allocation done by FreeRTOS goes through this function. If
     * users need to allocate FreeRTOS objects into external RAM, they should
     * use the "static" equivalents of FreeRTOS API to create FreeRTOS objects
     * (e.g., queues). */
    pvReturn = heap_caps_malloc(x_Wanted_आकार, पत्तनम्_FREERTOS_HEAP_CAPS);

    प्रत्यागम pvReturn;
}
/*-----------------------------------------------------------*/

शून्य v_पत्तनम्_Free(शून्य * pv)
{
    heap_caps_free(pv);
}
/*-----------------------------------------------------------*/

size_t x_पत्तनम्_Get_Free_Heap_आकार(शून्य)
{
    प्रत्यागम heap_caps_get_free_आकार(पत्तनम्_FREERTOS_HEAP_CAPS);
}
/*-----------------------------------------------------------*/

size_t x_पत्तनम्_Get_Minimum_Ever_Free_Heap_आकार(शून्य)
{
    प्रत्यागम heap_caps_get_minimum_free_आकार(पत्तनम्_FREERTOS_HEAP_CAPS);
}
/*-----------------------------------------------------------*/

bool x_पत्तनम्_Check_Valid_List_Mem(ध्रुव शून्य * ptr)
{
#यदि CONFIG_IDF_TARGET_LINUX
    प्रत्यागम true;
#अन्यथा /* CONFIG_IDF_TARGET_LINUX */
    प्रत्यागम esp_ptr_आन्तरिकम्(ptr) && esp_ptr_byte_accessible(ptr);
#यदि_अन्तः /* CONFIG_IDF_TARGET_LINUX */
}

bool x_पत्तनम्_Check_Valid_TCBMem(ध्रुव शून्य * ptr)
{
#यदि CONFIG_IDF_TARGET_LINUX
    प्रत्यागम true;
#अन्यथा /* CONFIG_IDF_TARGET_LINUX */
    प्रत्यागम esp_ptr_आन्तरिकम्(ptr) && esp_ptr_byte_accessible(ptr);
#यदि_अन्तः /* CONFIG_IDF_TARGET_LINUX */
}

#यदि CONFIG_FREERTOS_PLACE_TASK_STACKS_अन्तर्_EXT_RAM
शून्य * pv_पत्तनम्_Malloc_राशिः(size_t x_Wanted_आकार)
{
    प्रत्यागम heap_caps_malloc_prefer(x_Wanted_आकार, 2, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT, MALLOC_CAP_आन्तरिकम् | MALLOC_CAP_8BIT);
}

शून्य v_पत्तनम्_Free_राशिः(शून्य * pv)
{
    heap_caps_free(pv);
}
#यदि_अन्तः /* CONFIG_FREERTOS_PLACE_TASK_STACKS_IN_EXT_RAM */

bool x_Portcheck_Valid_राशिः_Mem(ध्रुव शून्य * ptr)
{
#यदि CONFIG_IDF_TARGET_LINUX
    प्रत्यागम true;
#अन्यथा /* CONFIG_IDF_TARGET_LINUX */
#यदि_परिभाषितम् CONFIG_FREERTOS_TASK_CREATE_ALLOW_EXT_MEM
    प्रत्यागम esp_ptr_byte_accessible(ptr);
#अन्यथा
    प्रत्यागम esp_ptr_आन्तरिकम्(ptr) && esp_ptr_byte_accessible(ptr);
#यदि_अन्तः
#यदि_अन्तः /* CONFIG_IDF_TARGET_LINUX */
}
