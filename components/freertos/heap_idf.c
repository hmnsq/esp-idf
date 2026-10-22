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
#وصف_ڪريو MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#ضمن "FreeRTOS.h"

#وصف_رد MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#إذا ( config_SUPPORT_DYNAMIC_ورهاست == 0 )
#غلطي This file must not be استعمال_ٿيل إذا config_SUPPORT_DYNAMIC_ورهاست is 0
#جيڪڏهن_پورو

#ضمن "esp_heap_caps.h"

#إذا !CONFIG_IDF_هدف_LINUX
/* Memory util functions are not implemented in the Linux simulator */
#ضمن "esp_memory_utils.h"
#جيڪڏهن_پورو /* CONFIG_IDF_TARGET_LINUX */

#وصف_ڪريو پورٽ_FREERTOS_ڍير_CAPS    ( MALLOC_CAP_اندروني | MALLOC_CAP_8_ٻٽو_انگ )

/*-----------------------------------------------------------*/

فراغ * pv_پورٽ_Malloc(size_t x_Wanted_سائيز)
{
    فراغ * pvReturn = عدم;

    /* All dynamic allocation done by FreeRTOS goes through this function. If
     * users need to allocate FreeRTOS objects into external RAM, they should
     * use the "static" equivalents of FreeRTOS API to create FreeRTOS objects
     * (e.g., queues). */
    pvReturn = ڍير_caps_malloc(x_Wanted_سائيز, پورٽ_FREERTOS_ڍير_CAPS);

    أرجع pvReturn;
}
/*-----------------------------------------------------------*/

فراغ v_پورٽ_Free(فراغ * pv)
{
    ڍير_caps_free(pv);
}
/*-----------------------------------------------------------*/

size_t x_پورٽ_Get_Free_ڍير_سائيز(فراغ)
{
    أرجع ڍير_caps_get_free_سائيز(پورٽ_FREERTOS_ڍير_CAPS);
}
/*-----------------------------------------------------------*/

size_t x_پورٽ_Get_Minimum_Ever_Free_ڍير_سائيز(فراغ)
{
    أرجع ڍير_caps_get_minimum_free_سائيز(پورٽ_FREERTOS_ڍير_CAPS);
}
/*-----------------------------------------------------------*/

bool x_پورٽ_Check_Valid_List_Mem(ثابت فراغ * ptr)
{
#إذا CONFIG_IDF_هدف_LINUX
    أرجع true;
#وإلا /* CONFIG_IDF_TARGET_LINUX */
    أرجع esp_ptr_اندروني(ptr) && esp_ptr_byte_accessible(ptr);
#جيڪڏهن_پورو /* CONFIG_IDF_TARGET_LINUX */
}

bool x_پورٽ_Check_Valid_TCBMem(ثابت فراغ * ptr)
{
#إذا CONFIG_IDF_هدف_LINUX
    أرجع true;
#وإلا /* CONFIG_IDF_TARGET_LINUX */
    أرجع esp_ptr_اندروني(ptr) && esp_ptr_byte_accessible(ptr);
#جيڪڏهن_پورو /* CONFIG_IDF_TARGET_LINUX */
}

#إذا CONFIG_FREERTOS_رکو_TASK_STACKS_اندريون_EXT_RAM
فراغ * pv_پورٽ_Malloc_ڍير(size_t x_Wanted_سائيز)
{
    أرجع ڍير_caps_malloc_prefer(x_Wanted_سائيز, 2, MALLOC_CAP_SPIRAM | MALLOC_CAP_8_ٻٽو_انگ, MALLOC_CAP_اندروني | MALLOC_CAP_8_ٻٽو_انگ);
}

فراغ v_پورٽ_Free_ڍير(فراغ * pv)
{
    ڍير_caps_free(pv);
}
#جيڪڏهن_پورو /* CONFIG_FREERTOS_PLACE_TASK_STACKS_IN_EXT_RAM */

bool x_Portcheck_Valid_ڍير_Mem(ثابت فراغ * ptr)
{
#إذا CONFIG_IDF_هدف_LINUX
    أرجع true;
#وإلا /* CONFIG_IDF_TARGET_LINUX */
#جيڪڏهن_وصف_ٿيل CONFIG_FREERTOS_TASK_CREATE_ALLOW_EXT_MEM
    أرجع esp_ptr_byte_accessible(ptr);
#وإلا
    أرجع esp_ptr_اندروني(ptr) && esp_ptr_byte_accessible(ptr);
#جيڪڏهن_پورو
#جيڪڏهن_پورو /* CONFIG_IDF_TARGET_LINUX */
}
