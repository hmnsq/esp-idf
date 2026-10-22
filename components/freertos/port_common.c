/*
 * SPDX-FileCopyrightText: 2021-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#अन्तर्भाव <string.h>
#अन्तर्भाव "FreeRTOS.h"
#अन्तर्भाव "task.h"
#अन्तर्भाव "esp_system.h"
#अन्तर्भाव "esp_memory_utils.h"
#अन्तर्भाव "sdkconfig.h"

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
#यदि ( config_SUPPORT_STATIC_आवण्टनम् == 1 )
शून्य v_Application_Get_Idle_Task_स्मृतिः(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   राशिः_Type_t **ppx_Idle_Task_राशिः_बफर,
                                   uint32_t *pul_Idle_Task_राशिः_आकार)
{
    StaticTask_t *px_TCBBuffer_अस्थायी;
    राशिः_Type_t *px_राशिः_बफर_अस्थायी;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#यदि (पत्तनम्_राशिः_GROWTH > 0)
    {
        px_TCBBuffer_अस्थायी = pv_पत्तनम्_Malloc(परिमाण(StaticTask_t));
        px_राशिः_बफर_अस्थायी = pv_पत्तनम्_Malloc(config_अल्पिष्ठम्_राशिः_आकार);
    }
#अन्यथा /* portSTACK_GROWTH */
    {
        px_राशिः_बफर_अस्थायी = pv_पत्तनम्_Malloc(config_अल्पिष्ठम्_राशिः_आकार);
        px_TCBBuffer_अस्थायी = pv_पत्तनम्_Malloc(परिमाण(StaticTask_t));
    }
#यदि_अन्तः /* portSTACK_GROWTH */

    प्रतिज्ञा(px_TCBBuffer_अस्थायी != रिक्त);
    प्रतिज्ञा(px_राशिः_बफर_अस्थायी != रिक्त);
    //Write back pointers
    *ppxIdleTaskTCBBuffer = px_TCBBuffer_अस्थायी;
    *ppx_Idle_Task_राशिः_बफर = px_राशिः_बफर_अस्थायी;
    *pul_Idle_Task_राशिः_आकार = config_अल्पिष्ठम्_राशिः_आकार;
}

#यदि ( ( CONFIG_FREERTOS_SMP ) && ( config_सङ्ख्या_सम्बन्धि_CORES > 1 ) )
शून्य v_Application_Get_Passive_Idle_Task_स्मृतिः(StaticTask_t ** ppxIdleTaskTCBBuffer,
                                          राशिः_Type_t ** ppx_Idle_Task_राशिः_बफर,
                                          uint32_t * pul_Idle_Task_राशिः_आकार,
                                          आधारः_Type_t x_Passive_Idle_Task_सूचकांक)
{
    v_Application_Get_Idle_Task_स्मृतिः(ppxIdleTaskTCBBuffer, ppx_Idle_Task_राशिः_बफर, pul_Idle_Task_राशिः_आकार);
}
#यदि_अन्तः /* ( ( CONFIG_FREERTOS_SMP ) && ( configNUMBER_OF_CORES > 1 ) ) */

#यदि configUSE_TIMERS

शून्य v_Application_Get_Timer_Task_स्मृतिः(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    राशिः_Type_t **ppx_Timer_Task_राशिः_बफर,
                                    uint32_t *pul_Timer_Task_राशिः_आकार)
{
    StaticTask_t *px_TCBBuffer_अस्थायी;
    राशिः_Type_t *px_राशिः_बफर_अस्थायी;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#यदि (पत्तनम्_राशिः_GROWTH > 0)
    {
        px_TCBBuffer_अस्थायी = pv_पत्तनम्_Malloc(परिमाण(StaticTask_t));
        px_राशिः_बफर_अस्थायी = pv_पत्तनम्_Malloc(config_TIMER_TASK_राशिः_गभीरता);
    }
#अन्यथा /* portSTACK_GROWTH */
    {
        px_राशिः_बफर_अस्थायी = pv_पत्तनम्_Malloc(config_TIMER_TASK_राशिः_गभीरता);
        px_TCBBuffer_अस्थायी = pv_पत्तनम्_Malloc(परिमाण(StaticTask_t));
    }
#यदि_अन्तः /* portSTACK_GROWTH */

    प्रतिज्ञा(px_TCBBuffer_अस्थायी != रिक्त);
    प्रतिज्ञा(px_राशिः_बफर_अस्थायी != रिक्त);
    //Write back pointers
    *ppxTimerTaskTCBBuffer = px_TCBBuffer_अस्थायी;
    *ppx_Timer_Task_राशिः_बफर = px_राशिः_बफर_अस्थायी;
    *pul_Timer_Task_राशिः_आकार = config_TIMER_TASK_राशिः_गभीरता;
}
#यदि_अन्तः //configUSE_TIMERS

#यदि_अन्तः // configSUPPORT_STATIC_ALLOCATION == 1
