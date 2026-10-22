/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#અન્તર્ભાવ "sdkconfig.h"
#અન્તર્ભાવ <stdint.h>
#અન્તર્ભાવ "FreeRTOS.h"

#જો ( CONFIG_FREERTOS_સંખ્યા_OF_CORES > 1 )

/**
 * @brief Prototype for test function.
 *
 * A test function can be passed to vTestOnAllCores() which will run the test function from a task on each core.
 */
પ્રકારવ્યાખ્યા શૂન્ય (* TestFunction_t)(શૂન્ય *);

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
શૂન્ય vTestOnAllCores(TestFunction_t pxTestCode, શૂન્ય * pvTestCodeArg, uint32_t ul_ઢગલો_Depth, UBaseType_t uxPriority);

#endif /* ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */
