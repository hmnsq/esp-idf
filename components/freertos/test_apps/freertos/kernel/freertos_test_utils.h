/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "FreeRTOS.h"

#ਜੇ ( CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES > 1 )

/**
 * @brief Prototype for test function.
 *
 * A test function can be passed to vTestOnAllCores() which will run the test function from a task on each core.
 */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (* TestFunction_t)(ਖਾਲੀ *);

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
ਖਾਲੀ vTestOnAllCores(TestFunction_t px_Test_ਕੋਡ, ਖਾਲੀ * pv_Test_ਕੋਡ_Arg, uint32_t ul_ਢੇਰ_ਡੂੰਘਾਈ, UBaseType_t ux_ਤਰਜੀਹ);

#ਜੇ_ਅੰਤ /* ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */
