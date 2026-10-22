/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ "sdkconfig.h"

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
#నిర్వచించు MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#అన్తర్భావ "FreeRTOS.h"

#నిర్వచనం_రద్దు MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#ఒకవేళ ( config_SUPPORT_DYNAMIC_కేటాయింపు == 0 )
#దోషం This file must not be వాడిన ఒకవేళ config_SUPPORT_DYNAMIC_కేటాయింపు is 0
#అయితే_ముగింపు

#అన్తర్భావ "esp_heap_caps.h"

#ఒకవేళ !CONFIG_IDF_లక్ష్యం_LINUX
/* Memory util functions are not implemented in the Linux simulator */
#అన్తర్భావ "esp_memory_utils.h"
#అయితే_ముగింపు /* CONFIG_IDF_TARGET_LINUX */

#నిర్వచించు పోర్టు_FREERTOS_కుప్ప_CAPS    ( MALLOC_CAP_అంతర్గత | MALLOC_CAP_8_బిట్టు )

/*-----------------------------------------------------------*/

శూన్య * pv_పోర్టు_Malloc(size_t x_Wanted_కొలత)
{
    శూన్య * pvReturn = ఖాళీ;

    /* All dynamic allocation done by FreeRTOS goes through this function. If
     * users need to allocate FreeRTOS objects into external RAM, they should
     * use the "static" equivalents of FreeRTOS API to create FreeRTOS objects
     * (e.g., queues). */
    pvReturn = కుప్ప_caps_malloc(x_Wanted_కొలత, పోర్టు_FREERTOS_కుప్ప_CAPS);

    తిరిగి pvReturn;
}
/*-----------------------------------------------------------*/

శూన్య v_పోర్టు_Free(శూన్య * pv)
{
    కుప్ప_caps_free(pv);
}
/*-----------------------------------------------------------*/

size_t x_పోర్టు_Get_Free_కుప్ప_కొలత(శూన్య)
{
    తిరిగి కుప్ప_caps_get_free_కొలత(పోర్టు_FREERTOS_కుప్ప_CAPS);
}
/*-----------------------------------------------------------*/

size_t x_పోర్టు_Get_Minimum_Ever_Free_కుప్ప_కొలత(శూన్య)
{
    తిరిగి కుప్ప_caps_get_minimum_free_కొలత(పోర్టు_FREERTOS_కుప్ప_CAPS);
}
/*-----------------------------------------------------------*/

bool x_పోర్టు_Check_Valid_List_Mem(స్థిరరాశి శూన్య * ptr)
{
#ఒకవేళ CONFIG_IDF_లక్ష్యం_LINUX
    తిరిగి true;
#లేకపోతే /* CONFIG_IDF_TARGET_LINUX */
    తిరిగి esp_ptr_అంతర్గత(ptr) && esp_ptr_byte_accessible(ptr);
#అయితే_ముగింపు /* CONFIG_IDF_TARGET_LINUX */
}

bool x_పోర్టు_Check_Valid_TCBMem(స్థిరరాశి శూన్య * ptr)
{
#ఒకవేళ CONFIG_IDF_లక్ష్యం_LINUX
    తిరిగి true;
#లేకపోతే /* CONFIG_IDF_TARGET_LINUX */
    తిరిగి esp_ptr_అంతర్గత(ptr) && esp_ptr_byte_accessible(ptr);
#అయితే_ముగింపు /* CONFIG_IDF_TARGET_LINUX */
}

#ఒకవేళ CONFIG_FREERTOS_ఉంచు_TASK_STACKS_లోపల_EXT_RAM
శూన్య * pv_పోర్టు_Malloc_దొంతర(size_t x_Wanted_కొలత)
{
    తిరిగి కుప్ప_caps_malloc_prefer(x_Wanted_కొలత, 2, MALLOC_CAP_SPIRAM | MALLOC_CAP_8_బిట్టు, MALLOC_CAP_అంతర్గత | MALLOC_CAP_8_బిట్టు);
}

శూన్య v_పోర్టు_Free_దొంతర(శూన్య * pv)
{
    కుప్ప_caps_free(pv);
}
#అయితే_ముగింపు /* CONFIG_FREERTOS_PLACE_TASK_STACKS_IN_EXT_RAM */

bool x_Portcheck_Valid_దొంతర_Mem(స్థిరరాశి శూన్య * ptr)
{
#ఒకవేళ CONFIG_IDF_లక్ష్యం_LINUX
    తిరిగి true;
#లేకపోతే /* CONFIG_IDF_TARGET_LINUX */
#నిర్వచితమైతే CONFIG_FREERTOS_TASK_CREATE_ALLOW_EXT_MEM
    తిరిగి esp_ptr_byte_accessible(ptr);
#లేకపోతే
    తిరిగి esp_ptr_అంతర్గత(ptr) && esp_ptr_byte_accessible(ptr);
#అయితే_ముగింపు
#అయితే_ముగింపు /* CONFIG_IDF_TARGET_LINUX */
}
