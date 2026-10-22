/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#شامل "sdkconfig.h"

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

#شامل "FreeRTOS.h"

#تعریف_منسوخ MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#اگر ( config_SUPPORT_DYNAMIC_تخصیص == 0 )
#غلطی This file must not be مستعمل اگر config_SUPPORT_DYNAMIC_تخصیص is 0
#اگر_ختم

#شامل "esp_heap_caps.h"

#اگر !CONFIG_IDF_ہدف_LINUX
/* Memory util functions are not implemented in the Linux simulator */
#شامل "esp_memory_utils.h"
#اگر_ختم /* CONFIG_IDF_TARGET_LINUX */

#تعریف_کرو پورٹ_FREERTOS_ڈھیر_CAPS    ( MALLOC_CAP_اندرونی | MALLOC_CAP_8_ثنائی_ہندسہ )

/*-----------------------------------------------------------*/

خالی * pv_پورٹ_Malloc(size_t x_Wanted_سائز)
{
    خالی * pvReturn = تہی;

    /* All dynamic allocation done by FreeRTOS goes through this function. If
     * users need to allocate FreeRTOS objects into external RAM, they should
     * use the "static" equivalents of FreeRTOS API to create FreeRTOS objects
     * (e.g., queues). */
    pvReturn = ڈھیر_caps_malloc(x_Wanted_سائز, پورٹ_FREERTOS_ڈھیر_CAPS);

    واپس pvReturn;
}
/*-----------------------------------------------------------*/

خالی v_پورٹ_Free(خالی * pv)
{
    ڈھیر_caps_free(pv);
}
/*-----------------------------------------------------------*/

size_t x_پورٹ_Get_Free_ڈھیر_سائز(خالی)
{
    واپس ڈھیر_caps_get_free_سائز(پورٹ_FREERTOS_ڈھیر_CAPS);
}
/*-----------------------------------------------------------*/

size_t x_پورٹ_Get_Minimum_Ever_Free_ڈھیر_سائز(خالی)
{
    واپس ڈھیر_caps_get_minimum_free_سائز(پورٹ_FREERTOS_ڈھیر_CAPS);
}
/*-----------------------------------------------------------*/

bool x_پورٹ_Check_Valid_List_Mem(ثابت خالی * ptr)
{
#اگر CONFIG_IDF_ہدف_LINUX
    واپس true;
#ورنہ /* CONFIG_IDF_TARGET_LINUX */
    واپس esp_ptr_اندرونی(ptr) && esp_ptr_byte_accessible(ptr);
#اگر_ختم /* CONFIG_IDF_TARGET_LINUX */
}

bool x_پورٹ_Check_Valid_TCBMem(ثابت خالی * ptr)
{
#اگر CONFIG_IDF_ہدف_LINUX
    واپس true;
#ورنہ /* CONFIG_IDF_TARGET_LINUX */
    واپس esp_ptr_اندرونی(ptr) && esp_ptr_byte_accessible(ptr);
#اگر_ختم /* CONFIG_IDF_TARGET_LINUX */
}

#اگر CONFIG_FREERTOS_رکھو_TASK_STACKS_اندر_EXT_RAM
خالی * pv_پورٹ_Malloc_ڈھیر(size_t x_Wanted_سائز)
{
    واپس ڈھیر_caps_malloc_prefer(x_Wanted_سائز, 2, MALLOC_CAP_SPIRAM | MALLOC_CAP_8_ثنائی_ہندسہ, MALLOC_CAP_اندرونی | MALLOC_CAP_8_ثنائی_ہندسہ);
}

خالی v_پورٹ_Free_ڈھیر(خالی * pv)
{
    ڈھیر_caps_free(pv);
}
#اگر_ختم /* CONFIG_FREERTOS_PLACE_TASK_STACKS_IN_EXT_RAM */

bool x_Portcheck_Valid_ڈھیر_Mem(ثابت خالی * ptr)
{
#اگر CONFIG_IDF_ہدف_LINUX
    واپس true;
#ورنہ /* CONFIG_IDF_TARGET_LINUX */
#اگر_تعریف_شدہ CONFIG_FREERTOS_TASK_CREATE_ALLOW_EXT_MEM
    واپس esp_ptr_byte_accessible(ptr);
#ورنہ
    واپس esp_ptr_اندرونی(ptr) && esp_ptr_byte_accessible(ptr);
#اگر_ختم
#اگر_ختم /* CONFIG_IDF_TARGET_LINUX */
}
