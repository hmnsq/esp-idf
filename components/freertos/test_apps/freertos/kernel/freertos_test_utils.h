/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#অন্তর্ভাব "sdkconfig.h"
#অন্তর্ভাব <stdint.h>
#অন্তর্ভাব "FreeRTOS.h"

#যদি ( CONFIG_FREERTOS_সংখ্যা_OF_CORES > 1 )

/**
 * @brief Prototype for test function.
 *
 * A test function can be passed to vTestOnAllCores() which will run the test function from a task on each core.
 */
প্রকারলেখ শূন্য (* TestFunction_t)(শূন্য *);

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
শূন্য vTestOnAllCores(TestFunction_t pxTestCode, শূন্য * pvTestCodeArg, uint32_t ul_স্তূপ_Depth, UBaseType_t uxPriority);

#endif /* ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */
