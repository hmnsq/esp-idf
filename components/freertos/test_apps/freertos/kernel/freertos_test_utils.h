/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#समावेश "sdkconfig.h"
#समावेश <stdint.h>
#समावेश "FreeRTOS.h"

#अगर ( CONFIG_FREERTOS_संख्या_OF_CORES > 1 )

/**
 * @brief Prototype for test function.
 *
 * A test function can be passed to vTestOnAllCores() which will run the test function from a task on each core.
 */
प्रकार व्योम (* TestFunction_t)(व्योम *);

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
व्योम vTestOnAllCores(TestFunction_t pxTestCode, व्योम * pvTestCodeArg, uint32_t ul_ढेर_Depth, UBaseType_t uxPriority);

#पूर्ण_अगर /* ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */
