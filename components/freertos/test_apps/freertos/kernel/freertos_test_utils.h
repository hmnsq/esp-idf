/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ "FreeRTOS.h"

#ಆದರೆ ( CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES > 1 )

/**
 * @brief Prototype for test function.
 *
 * A test function can be passed to vTestOnAllCores() which will run the test function from a task on each core.
 */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (* TestFunction_t)(ಶೂನ್ಯ *);

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
ಶೂನ್ಯ vTestOnAllCores(TestFunction_t px_Test_ಸಂಕೇತ, ಶೂನ್ಯ * pv_Test_ಸಂಕೇತ_Arg, uint32_t ul_ರಾಶಿ_ಆಳ, UBaseType_t ux_ಆದ್ಯತೆ);

#ಆದರೆ_ಅಂತ್ಯ /* ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */
