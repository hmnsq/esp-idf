/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#निर्देशः once

#अन्तर्भाव "sdkconfig.h"

/* This file gets pulled into assembly sources. Therefore, some includes need to
 * be wrapped in #ifndef __ASSEMBLER__ */

#यदि_अपरिभाषितम् __ASSEMBLER__
    /* For configASSERT() */
    #अन्तर्भाव <assert.h>
#यदि_अन्तः /* def __ASSEMBLER__ */

/* ----------------------------------------------------- Helpers -------------------------------------------------------
 * - Macros that the FreeRTOS configuration macros depend on
 * ------------------------------------------------------------------------------------------------------------------ */

/* Higher stack checker modes cause overhead on each function call */
#यदि CONFIG_राशिः_CHECK_ALL || CONFIG_राशिः_CHECK_STRONG
    #परिभाषय राशिः_OVERHEAD_CHECKER    256
#अन्यथा
    #परिभाषय राशिः_OVERHEAD_CHECKER    0
#यदि_अन्तः

/* with optimizations disabled, scheduler uses additional stack */
#यदि CONFIG_COMPILER_OPTIMIZATION_NONE
    #परिभाषय राशिः_OVERHEAD_OPTIMIZATION    320
#अन्यथा
    #परिभाषय राशिः_OVERHEAD_OPTIMIZATION    0
#यदि_अन्तः

/* apptrace module increases minimum stack usage */
#यदि CONFIG_ESP_TRACE_TRANSPORT_APPTRACE
    #परिभाषय राशिः_OVERHEAD_APPTRACE    1280
#अन्यथा
    #परिभाषय राशिः_OVERHEAD_APPTRACE    0
#यदि_अन्तः

/* Stack watchpoint decreases minimum usable stack size by up to 60 bytes.
 * See FreeRTOS FREERTOS_WATCHPOINT_END_OF_STACK option in Kconfig. */
#यदि CONFIG_FREERTOS_WATCHPOINT_अन्तः_सम्बन्धि_राशिः
    #परिभाषय राशिः_OVERHEAD_WATCHPOINT    60
#अन्यथा
    #परिभाषय राशिः_OVERHEAD_WATCHPOINT    0
#यदि_अन्तः

#परिभाषय config_राशिः_OVERHEAD_योग    \
    (                                 \
        राशिः_OVERHEAD_CHECKER +      \
        राशिः_OVERHEAD_OPTIMIZATION + \
        राशिः_OVERHEAD_APPTRACE +     \
        राशिः_OVERHEAD_WATCHPOINT     \
    )

/* The arch-specific FreeRTOSConfig_arch.h in esp_additions/arch_include/<arch>.
 * Placed here due to configSTACK_OVERHEAD_TOTAL. Todo: IDF-5712. */
#अन्तर्भाव "freertos/FreeRTOSConfig_arch.h"

/* ------------------------------------------------- FreeRTOS Config ---------------------------------------------------
 * - All Vanilla FreeRTOS configuration goes into this section
 * - Keep this section in-sync with the corresponding version of single-core upstream version of FreeRTOS
 * - Don't put any Amazon SMP FreeRTOS or IDF FreeRTOS configurations here. Those go into the next section
 * - Not all FreeRTOS configuration are listed. Some configurations have default values set in FreeRTOS.h thus don't
 *   need to be explicitly defined.
 * ------------------------------------------------------------------------------------------------------------------ */

/*-----------------------------------------------------------
* Application specific definitions.
*
* These definitions should be adjusted for your particular hardware and
* application requirements.
*
* THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
* FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
*
* See http://www.freertos.org/a00110.html
*----------------------------------------------------------*/

/* ------------------ Scheduler Related -------------------- */

#परिभाषय configUSE_PREEMPTION                         1
#परिभाषय configUSE_TICKLESS_IDLE                      CONFIG_FREERTOS_USE_TICKLESS_IDLE
#यदि configUSE_TICKLESS_IDLE
    #परिभाषय configEXPECTED_IDLE_TIME_BEFORE_SLEEP    CONFIG_FREERTOS_IDLE_TIME_BEFORE_SLEEP
#यदि_अन्तः /* configUSE_TICKLESS_IDLE */
#परिभाषय configCPU_CLOCK_HZ                           ( CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ * 1000000 )
#परिभाषय config_TICK_दरः_HZ                           CONFIG_FREERTOS_HZ
#परिभाषय configMAX_PRIORITIES                         ( 25 )
#परिभाषय configUSE_TIME_SLICING                       1
#परिभाषय configUSE_16_BIT_TICKS                       0
#परिभाषय configIDLE_SHOULD_YIELD                      0
#परिभाषय config_KERNEL_व्यवधानम्_प्राथमिकता              1      /*Todo: This currently isn't used anywhere */
#परिभाषय config_सङ्ख्या_सम्बन्धि_CORES                        CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES
/* For compatibility */
#परिभाषय config_अङ्कः_CORES                              config_सङ्ख्या_सम्बन्धि_CORES

/* ------------- Synchronization Primitives ---------------- */

#परिभाषय configUSE_MUTEXES                        1
#परिभाषय configUSE_RECURSIVE_MUTEXES              1
#परिभाषय configUSE_COUNTING_SEMAPHORES            1
#परिभाषय configUSE_QUEUE_SETS                     1
#परिभाषय config_QUEUE_REGISTRY_आकार                CONFIG_FREERTOS_QUEUE_REGISTRY_आकार
#परिभाषय configUSE_TASK_NOTIFICATIONS             1
#परिभाषय configTASK_NOTIFICATION_ARRAY_ENTRIES    CONFIG_FREERTOS_TASK_NOTIFICATION_ARRAY_ENTRIES

/* ----------------------- System -------------------------- */

#परिभाषय configMAX_TASK_NAME_LEN    CONFIG_FREERTOS_MAX_TASK_NAME_LEN

/* If deletion callbacks are enabled, the number of TLSP's are doubled (i.e.,
 * the length of the TCB's pvThreadLocalStoragePointersThis array). This allows
 * the latter half of the array to store the deletion callback pointers (whereas
 * the first half stores the TLSPs themselves). */
#यदि CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
    #परिभाषय config_अङ्कः_THREAD_LOCAL_STORAGE_सूचकाः    ( CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_सूचकाः * 2 )
#अन्यथा /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
    #परिभाषय config_अङ्कः_THREAD_LOCAL_STORAGE_सूचकाः    CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_सूचकाः
#यदि_अन्तः /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

#परिभाषय config_राशिः_गभीरता_TYPE                         uint32_t
#यदि CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY
    #परिभाषय configENABLE_BACKWARD_COMPATIBILITY        1
#अन्यथा /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */
    #परिभाषय configENABLE_BACKWARD_COMPATIBILITY        0
#यदि_अन्तः /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */
#परिभाषय configASSERT( a )    प्रतिज्ञा( a )

/* ----------------------- Memory  ------------------------- */

#परिभाषय config_SUPPORT_STATIC_आवण्टनम्              1
#परिभाषय config_SUPPORT_DYNAMIC_आवण्टनम्             1
#परिभाषय configAPPLICATION_ALLOCATED_HEAP             1
#यदि CONFIG_FREERTOS_PLACE_TASK_STACKS_अन्तर्_EXT_RAM
    #परिभाषय config_राशिः_आवण्टनम्_FROM_SEPARATE_HEAP    1
#अन्यथा
    #परिभाषय config_राशिः_आवण्टनम्_FROM_SEPARATE_HEAP    0
#यदि_अन्तः

/* ------------------------ Hooks -------------------------- */

#यदि CONFIG_FREERTOS_USE_IDLE_HOOK
    #परिभाषय configUSE_IDLE_HOOK               1
#अन्यथा /* CONFIG_FREERTOS_USE_IDLE_HOOK */
    #परिभाषय configUSE_IDLE_HOOK               0
#यदि_अन्तः /* CONFIG_FREERTOS_USE_IDLE_HOOK */
#यदि CONFIG_FREERTOS_USE_TICK_HOOK
    #परिभाषय configUSE_TICK_HOOK               1
#अन्यथा /* CONFIG_FREERTOS_USE_TICK_HOOK */
    #परिभाषय configUSE_TICK_HOOK               0
#यदि_अन्तः /* CONFIG_FREERTOS_USE_TICK_HOOK */
#यदि CONFIG_FREERTOS_CHECK_STACKOVERFLOW_NONE
    #परिभाषय config_CHECK_FOR_राशिः_अतिप्रवाहः    0
#नोचेद्यदि CONFIG_FREERTOS_CHECK_STACKOVERFLOW_PTRVAL
    #परिभाषय config_CHECK_FOR_राशिः_अतिप्रवाहः    1
#नोचेद्यदि CONFIG_FREERTOS_CHECK_STACKOVERFLOW_CANARY
    #परिभाषय config_CHECK_FOR_राशिः_अतिप्रवाहः    2
#यदि_अन्तः /* CONFIG_FREERTOS_CHECK_STACKOVERFLOW_NONE */
#परिभाषय config_लेखः_राशिः_उच्चः_स्थानाङ्कः       1             /* This must be set as the port requires TCB.pxEndOfStack */

/* ------------------- Run-time Stats ---------------------- */

#यदि_परिभाषितम् CONFIG_FREERTOS_GENERATE_सततम्_TIME_STATS
    #परिभाषय config_GENERATE_सततम्_TIME_STATS           1       /* Used by vTaskGetRunTimeStats() */
#यदि_अन्तः /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
#यदि_परिभाषितम् CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS
    #परिभाषय configUSE_STATS_FORMATTING_FUNCTIONS    1       /* Used by vTaskList() */
#यदि_अन्तः /* CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS */

#यदि CONFIG_FREERTOS_सततम्_TIME_गणक_TYPE_U32
    #परिभाषय config_सततम्_TIME_गणक_TYPE    uint32_t
#नोचेद्यदि CONFIG_FREERTOS_सततम्_TIME_गणक_TYPE_U64
    #परिभाषय config_सततम्_TIME_गणक_TYPE    uint64_t
#यदि_अन्तः /* CONFIG_FREERTOS_RUN_TIME_COUNTER_TYPE_U64 */

/* -------------------- Co-routines  ----------------------- */

#परिभाषय configUSE_CO_ROUTINES              0              /* CO_ROUTINES are not supported in ESP-IDF */
#परिभाषय configMAX_CO_ROUTINE_PRIORITIES    2

/* ------------------- Software Timer ---------------------- */

#यदि CONFIG_FREERTOS_USE_TIMERS
    #परिभाषय configUSE_TIMERS                          1
    #परिभाषय config_TIMER_TASK_प्राथमिकता                 CONFIG_FREERTOS_TIMER_TASK_प्राथमिकता
    #परिभाषय config_TIMER_QUEUE_दीर्घता                  CONFIG_FREERTOS_TIMER_QUEUE_दीर्घता
    #परिभाषय config_TIMER_TASK_राशिः_गभीरता              CONFIG_FREERTOS_TIMER_TASK_राशिः_गभीरता
    #परिभाषय configTIMER_SERVICE_TASK_NAME             CONFIG_FREERTOS_TIMER_SERVICE_TASK_NAME
    #परिभाषय configTIMER_SERVICE_TASK_CORE_AFFINITY    CONFIG_FREERTOS_TIMER_SERVICE_TASK_CORE_AFFINITY
#अन्यथा
    #परिभाषय configUSE_TIMERS                          0
#यदि_अन्तः


/* ------------------------ List --------------------------- */

#परिभाषय configLIST_VOLATILE    चञ्चल                     /* We define List elements as volatile to prevent the compiler from optimizing out essential code */

/* -------------------- API Includes ----------------------- */

#यदि CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY
    #परिभाषय configENABLE_BACKWARD_COMPATIBILITY    1
#अन्यथा /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */
    #परिभाषय configENABLE_BACKWARD_COMPATIBILITY    0
#यदि_अन्तः /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */

#परिभाषय INCLUDE_v_Task_प्राथमिकता_समुच्चयः                   1
#परिभाषय INCLUDE_ux_Task_प्राथमिकता_Get                  1
#परिभाषय INCLUDE_vTaskDelete                        1
#परिभाषय INCLUDE_vTaskSuspend                       1
#परिभाषय INCLUDE_vTaskDelay                         1
#परिभाषय INCLUDE_xTaskGetIdleTaskHandle             1
#परिभाषय INCLUDE_xTaskAbortDelay                    1
#परिभाषय INCLUDE_xSemaphoreGetMutexHolder           1
#परिभाषय INCLUDE_xTaskGetHandle                     1
#परिभाषय INCLUDE_ux_Task_Get_राशिः_उच्चः_Water_Mark        1
#परिभाषय INCLUDE_eTaskGetState                      1
#परिभाषय INCLUDE_xTaskResumeFromISR                 1
#यदि CONFIG_FREERTOS_USE_TIMERS
  #परिभाषय INCLUDE_xTimerPendFunctionCall           1
#अन्यथा
  #परिभाषय INCLUDE_xTimerPendFunctionCall           0
#यदि_अन्तः
#परिभाषय INCLUDE_xTaskGetSchedulerState             1
#परिभाषय INCLUDE_xTaskGetCurrentTaskHandle          1

/* -------------------- Trace Macros ----------------------- */

/*
 * For trace macros.
 * Note: Include trace macros here and not above as trace macros are dependent on some of the FreeRTOS configs
 */
#यदि_अपरिभाषितम् __ASSEMBLER__
    #यदि CONFIG_ESP_TRACE_ENABLE
        #अन्तर्भाव "esp_trace_freertos.h"
    #यदि_अन्तः

    #यदि CONFIG_FREERTOS_SMP

/* Default values for trace macros added to ESP-IDF implementation of SYSVIEW
 * that is not part of Amazon SMP FreeRTOS. */
        #यदि_अपरिभाषितम् traceISR_EXIT
            #परिभाषय traceISR_EXIT()
        #यदि_अन्तः
        #यदि_अपरिभाषितम् traceISR_ENTER
            #परिभाषय traceISR_ENTER( _n_ )
        #यदि_अन्तः

        #यदि_अपरिभाषितम् traceQUEUE_GIVE_FROM_ISR
            #परिभाषय traceQUEUE_GIVE_FROM_ISR( pxQueue )
        #यदि_अन्तः

        #यदि_अपरिभाषितम् traceQUEUE_GIVE_FROM_ISR_FAILED
            #परिभाषय traceQUEUE_GIVE_FROM_ISR_FAILED( pxQueue )
        #यदि_अन्तः

        #यदि_अपरिभाषितम् traceQUEUE_SEMAPHORE_RECEIVE
            #परिभाषय traceQUEUE_SEMAPHORE_RECEIVE( pxQueue )
        #यदि_अन्तः
    #यदि_अन्तः /* CONFIG_FREERTOS_SMP */
#यदि_अन्तः /* def __ASSEMBLER__ */

#यदि CONFIG_FREERTOS_USE_APPLICATION_TASK_चिह्नकम्
    #परिभाषय config_USE_APPLICATION_TASK_चिह्नकम्    1
#यदि_अन्तः // CONFIG_FREERTOS_USE_APPLICATION_TASK_TAG

/* -------------- List Data Integrity Checks --------------- */
#परिभाषय config_USE_LIST_दत्त_INTEGRITY_CHECK_अष्टकानि    CONFIG_FREERTOS_USE_LIST_दत्त_INTEGRITY_CHECK_अष्टकानि

/* ----------------------------------------------- Amazon SMP FreeRTOS -------------------------------------------------
 * - All Amazon SMP FreeRTOS specific configurations
 * ------------------------------------------------------------------------------------------------------------------ */

#यदि CONFIG_FREERTOS_SMP && ( CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES > 1 )
    #परिभाषय configUSE_CORE_AFFINITY              1
    #परिभाषय config_सततम्_गुणजः_PRIORITIES        1
    #परिभाषय configUSE_TASK_PREEMPTION_DISABLE    1
#यदि_अन्तः /* CONFIG_FREERTOS_SMP && ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */

/* -------------------------------------------------- IDF FreeRTOS -----------------------------------------------------
 * - All IDF FreeRTOS specific configurations
 * ------------------------------------------------------------------------------------------------------------------ */

#यदि !CONFIG_FREERTOS_SMP
    #यदि_परिभाषितम् CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID
        #परिभाषय configTASKLIST_INCLUDE_COREID                  1
    #यदि_अन्तः /* CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID */
    #यदि_परिभाषितम् CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
        #परिभाषय configTHREAD_LOCAL_STORAGE_DELETE_CALLBACKS    1
    #यदि_अन्तः /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
#यदि_अन्तः /* !CONFIG_FREERTOS_SMP */

/* ------------------------------------------------ ESP-IDF Additions --------------------------------------------------
 * - Any other macros required by the rest of ESP-IDF
 * ------------------------------------------------------------------------------------------------------------------ */

/* portNUM_PROCESSORS is deprecated and will be removed in ESP-IDF v6.0 (IDF-8785)
 * Please use the Kconfig option CONFIG_FREERTOS_NUMBER_OF_CORES instead.
 */
#परिभाषय पत्तनम्_अङ्कः_PROCESSORS    config_सङ्ख्या_सम्बन्धि_CORES
