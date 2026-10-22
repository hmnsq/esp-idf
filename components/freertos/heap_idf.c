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
#تعریف_کن MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#ضمن "FreeRTOS.h"

#لغو_تعریف MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#إذا ( config_SUPPORT_DYNAMIC_تخصیص == 0 )
#خطا This file must not be استفاده_شده إذا config_SUPPORT_DYNAMIC_تخصیص is 0
#پایان_اگر

#ضمن "esp_heap_caps.h"

#إذا !CONFIG_IDF_هدف_LINUX
/* Memory util functions are not implemented in the Linux simulator */
#ضمن "esp_memory_utils.h"
#پایان_اگر /* CONFIG_IDF_TARGET_LINUX */

#تعریف_کن پورت_FREERTOS_انباشته_CAPS    ( MALLOC_CAP_داخلی | MALLOC_CAP_8_بیت )

/*-----------------------------------------------------------*/

فراغ * pv_پورت_Malloc(size_t x_Wanted_اندازه)
{
    فراغ * pvReturn = عدم;

    /* All dynamic allocation done by FreeRTOS goes through this function. If
     * users need to allocate FreeRTOS objects into external RAM, they should
     * use the "static" equivalents of FreeRTOS API to create FreeRTOS objects
     * (e.g., queues). */
    pvReturn = انباشته_caps_malloc(x_Wanted_اندازه, پورت_FREERTOS_انباشته_CAPS);

    أرجع pvReturn;
}
/*-----------------------------------------------------------*/

فراغ v_پورت_Free(فراغ * pv)
{
    انباشته_caps_free(pv);
}
/*-----------------------------------------------------------*/

size_t x_پورت_Get_Free_انباشته_اندازه(فراغ)
{
    أرجع انباشته_caps_get_free_اندازه(پورت_FREERTOS_انباشته_CAPS);
}
/*-----------------------------------------------------------*/

size_t x_پورت_Get_Minimum_Ever_Free_انباشته_اندازه(فراغ)
{
    أرجع انباشته_caps_get_minimum_free_اندازه(پورت_FREERTOS_انباشته_CAPS);
}
/*-----------------------------------------------------------*/

bool x_پورت_Check_Valid_List_Mem(ثابت فراغ * ptr)
{
#إذا CONFIG_IDF_هدف_LINUX
    أرجع true;
#وإلا /* CONFIG_IDF_TARGET_LINUX */
    أرجع esp_ptr_داخلی(ptr) && esp_ptr_byte_accessible(ptr);
#پایان_اگر /* CONFIG_IDF_TARGET_LINUX */
}

bool x_پورت_Check_Valid_TCBMem(ثابت فراغ * ptr)
{
#إذا CONFIG_IDF_هدف_LINUX
    أرجع true;
#وإلا /* CONFIG_IDF_TARGET_LINUX */
    أرجع esp_ptr_داخلی(ptr) && esp_ptr_byte_accessible(ptr);
#پایان_اگر /* CONFIG_IDF_TARGET_LINUX */
}

#إذا CONFIG_FREERTOS_قراربده_TASK_STACKS_درون_EXT_RAM
فراغ * pv_پورت_Malloc_پشته(size_t x_Wanted_اندازه)
{
    أرجع انباشته_caps_malloc_prefer(x_Wanted_اندازه, 2, MALLOC_CAP_SPIRAM | MALLOC_CAP_8_بیت, MALLOC_CAP_داخلی | MALLOC_CAP_8_بیت);
}

فراغ v_پورت_Free_پشته(فراغ * pv)
{
    انباشته_caps_free(pv);
}
#پایان_اگر /* CONFIG_FREERTOS_PLACE_TASK_STACKS_IN_EXT_RAM */

bool x_Portcheck_Valid_پشته_Mem(ثابت فراغ * ptr)
{
#إذا CONFIG_IDF_هدف_LINUX
    أرجع true;
#وإلا /* CONFIG_IDF_TARGET_LINUX */
#اگر_تعریف_شده CONFIG_FREERTOS_TASK_CREATE_ALLOW_EXT_MEM
    أرجع esp_ptr_byte_accessible(ptr);
#وإلا
    أرجع esp_ptr_داخلی(ptr) && esp_ptr_byte_accessible(ptr);
#پایان_اگر
#پایان_اگر /* CONFIG_IDF_TARGET_LINUX */
}
