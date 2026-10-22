/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"

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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#ਅਨ੍ਤਰ੍ਭਾਵ "FreeRTOS.h"

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#ਜੇ ( config_SUPPORT_DYNAMIC_ਵੰਡ == 0 )
#ਗਲਤੀ This file must not be ਵਰਤਿਆ ਜੇ config_SUPPORT_DYNAMIC_ਵੰਡ is 0
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "esp_heap_caps.h"

#ਜੇ !CONFIG_IDF_ਨਿਸ਼ਾਨਾ_LINUX
/* Memory util functions are not implemented in the Linux simulator */
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_memory_utils.h"
#ਜੇ_ਅੰਤ /* CONFIG_IDF_TARGET_LINUX */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_FREERTOS_ਢੇਰ_CAPS    ( MALLOC_CAP_ਅੰਦਰੂਨੀ | MALLOC_CAP_8_ਦੋਆਧਾਰੀ_ਅੰਕ )

/*-----------------------------------------------------------*/

ਖਾਲੀ * pv_ਪੋਰਟ_Malloc(size_t x_Wanted_ਮਾਪ)
{
    ਖਾਲੀ * pvReturn = ਨਿਰਾ;

    /* All dynamic allocation done by FreeRTOS goes through this function. If
     * users need to allocate FreeRTOS objects into external RAM, they should
     * use the "static" equivalents of FreeRTOS API to create FreeRTOS objects
     * (e.g., queues). */
    pvReturn = ਢੇਰ_caps_malloc(x_Wanted_ਮਾਪ, ਪੋਰਟ_FREERTOS_ਢੇਰ_CAPS);

    ਵਾਪਸ pvReturn;
}
/*-----------------------------------------------------------*/

ਖਾਲੀ v_ਪੋਰਟ_Free(ਖਾਲੀ * pv)
{
    ਢੇਰ_caps_free(pv);
}
/*-----------------------------------------------------------*/

size_t x_ਪੋਰਟ_Get_Free_ਢੇਰ_ਮਾਪ(ਖਾਲੀ)
{
    ਵਾਪਸ ਢੇਰ_caps_get_free_ਮਾਪ(ਪੋਰਟ_FREERTOS_ਢੇਰ_CAPS);
}
/*-----------------------------------------------------------*/

size_t x_ਪੋਰਟ_Get_Minimum_Ever_Free_ਢੇਰ_ਮਾਪ(ਖਾਲੀ)
{
    ਵਾਪਸ ਢੇਰ_caps_get_minimum_free_ਮਾਪ(ਪੋਰਟ_FREERTOS_ਢੇਰ_CAPS);
}
/*-----------------------------------------------------------*/

bool x_ਪੋਰਟ_Check_Valid_List_Mem(ਅਚਲ ਖਾਲੀ * ptr)
{
#ਜੇ CONFIG_IDF_ਨਿਸ਼ਾਨਾ_LINUX
    ਵਾਪਸ true;
#ਨਹੀਂਤਾਂ /* CONFIG_IDF_TARGET_LINUX */
    ਵਾਪਸ esp_ptr_ਅੰਦਰੂਨੀ(ptr) && esp_ptr_byte_accessible(ptr);
#ਜੇ_ਅੰਤ /* CONFIG_IDF_TARGET_LINUX */
}

bool x_ਪੋਰਟ_Check_Valid_TCBMem(ਅਚਲ ਖਾਲੀ * ptr)
{
#ਜੇ CONFIG_IDF_ਨਿਸ਼ਾਨਾ_LINUX
    ਵਾਪਸ true;
#ਨਹੀਂਤਾਂ /* CONFIG_IDF_TARGET_LINUX */
    ਵਾਪਸ esp_ptr_ਅੰਦਰੂਨੀ(ptr) && esp_ptr_byte_accessible(ptr);
#ਜੇ_ਅੰਤ /* CONFIG_IDF_TARGET_LINUX */
}

#ਜੇ CONFIG_FREERTOS_ਰੱਖੋ_TASK_STACKS_ਵਿੱਚ_EXT_RAM
ਖਾਲੀ * pv_ਪੋਰਟ_Malloc_ਢੇਰ(size_t x_Wanted_ਮਾਪ)
{
    ਵਾਪਸ ਢੇਰ_caps_malloc_prefer(x_Wanted_ਮਾਪ, 2, MALLOC_CAP_SPIRAM | MALLOC_CAP_8_ਦੋਆਧਾਰੀ_ਅੰਕ, MALLOC_CAP_ਅੰਦਰੂਨੀ | MALLOC_CAP_8_ਦੋਆਧਾਰੀ_ਅੰਕ);
}

ਖਾਲੀ v_ਪੋਰਟ_Free_ਢੇਰ(ਖਾਲੀ * pv)
{
    ਢੇਰ_caps_free(pv);
}
#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_PLACE_TASK_STACKS_IN_EXT_RAM */

bool x_Portcheck_Valid_ਢੇਰ_Mem(ਅਚਲ ਖਾਲੀ * ptr)
{
#ਜੇ CONFIG_IDF_ਨਿਸ਼ਾਨਾ_LINUX
    ਵਾਪਸ true;
#ਨਹੀਂਤਾਂ /* CONFIG_IDF_TARGET_LINUX */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FREERTOS_TASK_CREATE_ALLOW_EXT_MEM
    ਵਾਪਸ esp_ptr_byte_accessible(ptr);
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ esp_ptr_ਅੰਦਰੂਨੀ(ptr) && esp_ptr_byte_accessible(ptr);
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ /* CONFIG_IDF_TARGET_LINUX */
}
