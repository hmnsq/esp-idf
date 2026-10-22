/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"

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
#ವ್ಯಾಖ್ಯಾನಿಸು MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#ಅನ್ತರ್ಭಾವ "FreeRTOS.h"

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#ಆದರೆ ( config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ == 0 )
#ದೋಷ This file must not be ಬಳಸಿದ ಆದರೆ config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ is 0
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "esp_heap_caps.h"

#ಆದರೆ !CONFIG_IDF_ಗುರಿ_LINUX
/* Memory util functions are not implemented in the Linux simulator */
#ಅನ್ತರ್ಭಾವ "esp_memory_utils.h"
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_IDF_TARGET_LINUX */

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_FREERTOS_ರಾಶಿ_CAPS    ( MALLOC_CAP_ಆಂತರಿಕ | MALLOC_CAP_8_ಬಿಟ್ )

/*-----------------------------------------------------------*/

ಶೂನ್ಯ * pv_ಪೋರ್ಟ್_Malloc(size_t x_Wanted_ಗಾತ್ರ)
{
    ಶೂನ್ಯ * pvReturn = ಖಾಲಿ;

    /* All dynamic allocation done by FreeRTOS goes through this function. If
     * users need to allocate FreeRTOS objects into external RAM, they should
     * use the "static" equivalents of FreeRTOS API to create FreeRTOS objects
     * (e.g., queues). */
    pvReturn = ರಾಶಿ_caps_malloc(x_Wanted_ಗಾತ್ರ, ಪೋರ್ಟ್_FREERTOS_ರಾಶಿ_CAPS);

    ಹಿಂತಿರುಗಿ pvReturn;
}
/*-----------------------------------------------------------*/

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Free(ಶೂನ್ಯ * pv)
{
    ರಾಶಿ_caps_free(pv);
}
/*-----------------------------------------------------------*/

size_t x_ಪೋರ್ಟ್_Get_Free_ರಾಶಿ_ಗಾತ್ರ(ಶೂನ್ಯ)
{
    ಹಿಂತಿರುಗಿ ರಾಶಿ_caps_get_free_ಗಾತ್ರ(ಪೋರ್ಟ್_FREERTOS_ರಾಶಿ_CAPS);
}
/*-----------------------------------------------------------*/

size_t x_ಪೋರ್ಟ್_Get_Minimum_Ever_Free_ರಾಶಿ_ಗಾತ್ರ(ಶೂನ್ಯ)
{
    ಹಿಂತಿರುಗಿ ರಾಶಿ_caps_get_minimum_free_ಗಾತ್ರ(ಪೋರ್ಟ್_FREERTOS_ರಾಶಿ_CAPS);
}
/*-----------------------------------------------------------*/

bool x_ಪೋರ್ಟ್_Check_Valid_List_Mem(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * ptr)
{
#ಆದರೆ CONFIG_IDF_ಗುರಿ_LINUX
    ಹಿಂತಿರುಗಿ true;
#ಇಲ್ಲದಿದ್ದರೆ /* CONFIG_IDF_TARGET_LINUX */
    ಹಿಂತಿರುಗಿ esp_ptr_ಆಂತರಿಕ(ptr) && esp_ptr_byte_accessible(ptr);
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_IDF_TARGET_LINUX */
}

bool x_ಪೋರ್ಟ್_Check_Valid_TCBMem(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * ptr)
{
#ಆದರೆ CONFIG_IDF_ಗುರಿ_LINUX
    ಹಿಂತಿರುಗಿ true;
#ಇಲ್ಲದಿದ್ದರೆ /* CONFIG_IDF_TARGET_LINUX */
    ಹಿಂತಿರುಗಿ esp_ptr_ಆಂತರಿಕ(ptr) && esp_ptr_byte_accessible(ptr);
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_IDF_TARGET_LINUX */
}

#ಆದರೆ CONFIG_FREERTOS_ಇರಿಸು_TASK_STACKS_ಒಳ_EXT_RAM
ಶೂನ್ಯ * pv_ಪೋರ್ಟ್_Malloc_ರಾಶಿ(size_t x_Wanted_ಗಾತ್ರ)
{
    ಹಿಂತಿರುಗಿ ರಾಶಿ_caps_malloc_prefer(x_Wanted_ಗಾತ್ರ, 2, MALLOC_CAP_SPIRAM | MALLOC_CAP_8_ಬಿಟ್, MALLOC_CAP_ಆಂತರಿಕ | MALLOC_CAP_8_ಬಿಟ್);
}

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Free_ರಾಶಿ(ಶೂನ್ಯ * pv)
{
    ರಾಶಿ_caps_free(pv);
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_PLACE_TASK_STACKS_IN_EXT_RAM */

bool x_Portcheck_Valid_ರಾಶಿ_Mem(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * ptr)
{
#ಆದರೆ CONFIG_IDF_ಗುರಿ_LINUX
    ಹಿಂತಿರುಗಿ true;
#ಇಲ್ಲದಿದ್ದರೆ /* CONFIG_IDF_TARGET_LINUX */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FREERTOS_TASK_CREATE_ALLOW_EXT_MEM
    ಹಿಂತಿರುಗಿ esp_ptr_byte_accessible(ptr);
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ esp_ptr_ಆಂತರಿಕ(ptr) && esp_ptr_byte_accessible(ptr);
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_IDF_TARGET_LINUX */
}
