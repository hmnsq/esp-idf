/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#അന്തര്ഭാവ "sdkconfig.h"
#അന്തര്ഭാവ <stdint.h>
#അന്തര്ഭാവ "FreeRTOS.h"

#എങ്കിൽ ( CONFIG_FREERTOS_സംഖ്യ_OF_CORES > 1 )

/**
 * @brief Prototype for test function.
 *
 * A test function can be passed to vTestOnAllCores() which will run the test function from a task on each core.
 */
തരനിർവചനം ശൂന്യം (* TestFunction_t)(ശൂന്യം *);

/**
 * @brief Run a test function on each core
 *
 * This function will internally create a task pinned to each core, where each task will call the provided test
 * function. This function will block until all cores finish executing the test function.
 *
 * @param pxTestCode Test function
 * @param pvTestCodeArg Argument provided to test function
 * @param ulStackDepth Stack depth of the created tasks
 * @param uxPriority Priority of the created tasks
 */
ശൂന്യം vTestOnAllCores(TestFunction_t pxTestCode, ശൂന്യം * pvTestCodeArg, uint32_t ul_അട്ടി_Depth, UBaseType_t uxPriority);

#endif /* ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */
