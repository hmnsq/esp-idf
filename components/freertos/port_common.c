/*
 * SPDX-FileCopyrightText: 2021-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ <string.h>
#అన్తర్భావ "FreeRTOS.h"
#అన్తర్భావ "task.h"
#అన్తర్భావ "esp_system.h"
#అన్తర్భావ "esp_memory_utils.h"
#అన్తర్భావ "sdkconfig.h"

/* ----------------------------------------- Port Implementations (Common)  --------------------------------------------
 * - Common FreeRTOS port function implementations
 * - These functions are common to all FreeRTOS ports (i.e., on all architectures and all FreeRTOS implementations).
 * ------------------------------------------------------------------------------------------------------------------ */

// ------------- FreeRTOS Static Allocation ----------------

/*
These function are required by FreeRTOS when configSUPPORT_STATIC_ALLOCATION is
enabled and is used by FreeRTOS to obtain memory for its IDLE/Timer tasks.

We simply allocate the IDLE/Timer tasks memory from the FreeRTOS heap.
*/
#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )
శూన్య v_Application_Get_Idle_Task_జ్ఞాపకం(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   దొంతర_Type_t **ppx_Idle_Task_దొంతర_బఫరు,
                                   uint32_t *pul_Idle_Task_దొంతర_కొలత)
{
    StaticTask_t *px_TCBBuffer_తాత్కాలిక;
    దొంతర_Type_t *px_దొంతర_బఫరు_తాత్కాలిక;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#ఒకవేళ (పోర్టు_దొంతర_GROWTH > 0)
    {
        px_TCBBuffer_తాత్కాలిక = pv_పోర్టు_Malloc(పరిమాణం(StaticTask_t));
        px_దొంతర_బఫరు_తాత్కాలిక = pv_పోర్టు_Malloc(config_కనీస_దొంతర_కొలత);
    }
#లేకపోతే /* portSTACK_GROWTH */
    {
        px_దొంతర_బఫరు_తాత్కాలిక = pv_పోర్టు_Malloc(config_కనీస_దొంతర_కొలత);
        px_TCBBuffer_తాత్కాలిక = pv_పోర్టు_Malloc(పరిమాణం(StaticTask_t));
    }
#అయితే_ముగింపు /* portSTACK_GROWTH */

    ప్రతిజ్ఞా(px_TCBBuffer_తాత్కాలిక != ఖాళీ);
    ప్రతిజ్ఞా(px_దొంతర_బఫరు_తాత్కాలిక != ఖాళీ);
    //Write back pointers
    *ppxIdleTaskTCBBuffer = px_TCBBuffer_తాత్కాలిక;
    *ppx_Idle_Task_దొంతర_బఫరు = px_దొంతర_బఫరు_తాత్కాలిక;
    *pul_Idle_Task_దొంతర_కొలత = config_కనీస_దొంతర_కొలత;
}

#ఒకవేళ ( ( CONFIG_FREERTOS_SMP ) && ( config_సంఖ్య_యొక్క_CORES > 1 ) )
శూన్య v_Application_Get_Passive_Idle_Task_జ్ఞాపకం(StaticTask_t ** ppxIdleTaskTCBBuffer,
                                          దొంతర_Type_t ** ppx_Idle_Task_దొంతర_బఫరు,
                                          uint32_t * pul_Idle_Task_దొంతర_కొలత,
                                          పునాది_Type_t x_Passive_Idle_Task_సూచీ)
{
    v_Application_Get_Idle_Task_జ్ఞాపకం(ppxIdleTaskTCBBuffer, ppx_Idle_Task_దొంతర_బఫరు, pul_Idle_Task_దొంతర_కొలత);
}
#అయితే_ముగింపు /* ( ( CONFIG_FREERTOS_SMP ) && ( configNUMBER_OF_CORES > 1 ) ) */

#ఒకవేళ configUSE_TIMERS

శూన్య v_Application_Get_Timer_Task_జ్ఞాపకం(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    దొంతర_Type_t **ppx_Timer_Task_దొంతర_బఫరు,
                                    uint32_t *pul_Timer_Task_దొంతర_కొలత)
{
    StaticTask_t *px_TCBBuffer_తాత్కాలిక;
    దొంతర_Type_t *px_దొంతర_బఫరు_తాత్కాలిక;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#ఒకవేళ (పోర్టు_దొంతర_GROWTH > 0)
    {
        px_TCBBuffer_తాత్కాలిక = pv_పోర్టు_Malloc(పరిమాణం(StaticTask_t));
        px_దొంతర_బఫరు_తాత్కాలిక = pv_పోర్టు_Malloc(config_TIMER_TASK_దొంతర_లోతు);
    }
#లేకపోతే /* portSTACK_GROWTH */
    {
        px_దొంతర_బఫరు_తాత్కాలిక = pv_పోర్టు_Malloc(config_TIMER_TASK_దొంతర_లోతు);
        px_TCBBuffer_తాత్కాలిక = pv_పోర్టు_Malloc(పరిమాణం(StaticTask_t));
    }
#అయితే_ముగింపు /* portSTACK_GROWTH */

    ప్రతిజ్ఞా(px_TCBBuffer_తాత్కాలిక != ఖాళీ);
    ప్రతిజ్ఞా(px_దొంతర_బఫరు_తాత్కాలిక != ఖాళీ);
    //Write back pointers
    *ppxTimerTaskTCBBuffer = px_TCBBuffer_తాత్కాలిక;
    *ppx_Timer_Task_దొంతర_బఫరు = px_దొంతర_బఫరు_తాత్కాలిక;
    *pul_Timer_Task_దొంతర_కొలత = config_TIMER_TASK_దొంతర_లోతు;
}
#అయితే_ముగింపు //configUSE_TIMERS

#అయితే_ముగింపు // configSUPPORT_STATIC_ALLOCATION == 1
