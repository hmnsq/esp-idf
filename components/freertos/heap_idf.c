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
#تعریف_کرو MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#ضمن "FreeRTOS.h"

#تعریف_رد MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#إذا ( config_SUPPORT_DYNAMIC_ونڈ == 0 )
#غلطی This file must not be ورتیا إذا config_SUPPORT_DYNAMIC_ونڈ is 0
#جے_انت

#ضمن "esp_heap_caps.h"

#إذا !CONFIG_IDF_نشانہ_LINUX
/* Memory util functions are not implemented in the Linux simulator */
#ضمن "esp_memory_utils.h"
#جے_انت /* CONFIG_IDF_TARGET_LINUX */

#تعریف_کرو پورٹ_FREERTOS_ڈھیر_CAPS    ( MALLOC_CAP_اندرونی | MALLOC_CAP_8_ثنائی_ہندسہ )

/*-----------------------------------------------------------*/

فراغ * pv_پورٹ_Malloc(size_t x_Wanted_ناپ)
{
    فراغ * pvReturn = عدم;

    /* All dynamic allocation done by FreeRTOS goes through this function. If
     * users need to allocate FreeRTOS objects into external RAM, they should
     * use the "static" equivalents of FreeRTOS API to create FreeRTOS objects
     * (e.g., queues). */
    pvReturn = ڈھیر_caps_malloc(x_Wanted_ناپ, پورٹ_FREERTOS_ڈھیر_CAPS);

    أرجع pvReturn;
}
/*-----------------------------------------------------------*/

فراغ v_پورٹ_Free(فراغ * pv)
{
    ڈھیر_caps_free(pv);
}
/*-----------------------------------------------------------*/

size_t x_پورٹ_Get_Free_ڈھیر_ناپ(فراغ)
{
    أرجع ڈھیر_caps_get_free_ناپ(پورٹ_FREERTOS_ڈھیر_CAPS);
}
/*-----------------------------------------------------------*/

size_t x_پورٹ_Get_Minimum_Ever_Free_ڈھیر_ناپ(فراغ)
{
    أرجع ڈھیر_caps_get_minimum_free_ناپ(پورٹ_FREERTOS_ڈھیر_CAPS);
}
/*-----------------------------------------------------------*/

bool x_پورٹ_Check_Valid_List_Mem(ثابت فراغ * ptr)
{
#إذا CONFIG_IDF_نشانہ_LINUX
    أرجع true;
#وإلا /* CONFIG_IDF_TARGET_LINUX */
    أرجع esp_ptr_اندرونی(ptr) && esp_ptr_byte_accessible(ptr);
#جے_انت /* CONFIG_IDF_TARGET_LINUX */
}

bool x_پورٹ_Check_Valid_TCBMem(ثابت فراغ * ptr)
{
#إذا CONFIG_IDF_نشانہ_LINUX
    أرجع true;
#وإلا /* CONFIG_IDF_TARGET_LINUX */
    أرجع esp_ptr_اندرونی(ptr) && esp_ptr_byte_accessible(ptr);
#جے_انت /* CONFIG_IDF_TARGET_LINUX */
}

#إذا CONFIG_FREERTOS_رکھو_TASK_STACKS_وچ_EXT_RAM
فراغ * pv_پورٹ_Malloc_ڈھیر(size_t x_Wanted_ناپ)
{
    أرجع ڈھیر_caps_malloc_prefer(x_Wanted_ناپ, 2, MALLOC_CAP_SPIRAM | MALLOC_CAP_8_ثنائی_ہندسہ, MALLOC_CAP_اندرونی | MALLOC_CAP_8_ثنائی_ہندسہ);
}

فراغ v_پورٹ_Free_ڈھیر(فراغ * pv)
{
    ڈھیر_caps_free(pv);
}
#جے_انت /* CONFIG_FREERTOS_PLACE_TASK_STACKS_IN_EXT_RAM */

bool x_Portcheck_Valid_ڈھیر_Mem(ثابت فراغ * ptr)
{
#إذا CONFIG_IDF_نشانہ_LINUX
    أرجع true;
#وإلا /* CONFIG_IDF_TARGET_LINUX */
#جے_تعریف_شدہ CONFIG_FREERTOS_TASK_CREATE_ALLOW_EXT_MEM
    أرجع esp_ptr_byte_accessible(ptr);
#وإلا
    أرجع esp_ptr_اندرونی(ptr) && esp_ptr_byte_accessible(ptr);
#جے_انت
#جے_انت /* CONFIG_IDF_TARGET_LINUX */
}
