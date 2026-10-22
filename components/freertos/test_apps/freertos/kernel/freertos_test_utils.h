/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#شامل "sdkconfig.h"
#شامل <stdint.h>
#شامل "FreeRTOS.h"

#اگر ( CONFIG_FREERTOS_نمبر_کا_CORES > 1 )

/**
 * @brief Prototype for test function.
 *
 * A test function can be passed to vTestOnAllCores() which will run the test function from a task on each core.
 */
قسمتعریف خالی (* TestFunction_t)(خالی *);

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
خالی vTestOnAllCores(TestFunction_t px_Test_رمز, خالی * pv_Test_رمز_Arg, uint32_t ul_ڈھیر_گہرائی, UBaseType_t ux_ترجیح);

#اگر_ختم /* ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */
