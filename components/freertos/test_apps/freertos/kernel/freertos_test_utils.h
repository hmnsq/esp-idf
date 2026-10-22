/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ <stdint.h>
#అన్తర్భావ "FreeRTOS.h"

#ఒకవేళ ( CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES > 1 )

/**
 * @brief Prototype for test function.
 *
 * A test function can be passed to vTestOnAllCores() which will run the test function from a task on each core.
 */
రకనిర్వచనం శూన్య (* TestFunction_t)(శూన్య *);

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
శూన్య vTestOnAllCores(TestFunction_t px_Test_సంకేతం, శూన్య * pv_Test_సంకేతం_Arg, uint32_t ul_దొంతర_లోతు, UBaseType_t ux_ప్రాధాన్యత);

#అయితే_ముగింపు /* ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */
