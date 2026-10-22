/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#आशय once

#समावेश "sdkconfig.h"

/* This file gets pulled into assembly sources. Therefore, some includes need to
 * be wrapped in #ifndef __ASSEMBLER__ */

#अगर_अघोषित __ASSEMBLER__
    /* For configASSERT() */
    #समावेश <assert.h>
#पूर्ण_अगर /* def __ASSEMBLER__ */

/* ----------------------------------------------------- Helpers -------------------------------------------------------
 * - Macros that the FreeRTOS configuration macros depend on
 * ------------------------------------------------------------------------------------------------------------------ */

/* Higher stack checker modes cause overhead on each function call */
#अगर CONFIG_ढेर_CHECK_ALL || CONFIG_ढेर_CHECK_STRONG
    #घोषणा ढेर_OVERHEAD_CHECKER    256
#अन्यथा
    #घोषणा ढेर_OVERHEAD_CHECKER    0
#पूर्ण_अगर

/* with optimizations disabled, scheduler uses additional stack */
#अगर CONFIG_COMPILER_OPTIMIZATION_NONE
    #घोषणा ढेर_OVERHEAD_OPTIMIZATION    320
#अन्यथा
    #घोषणा ढेर_OVERHEAD_OPTIMIZATION    0
#पूर्ण_अगर

/* apptrace module increases minimum stack usage */
#अगर CONFIG_ESP_TRACE_TRANSPORT_APPTRACE
    #घोषणा ढेर_OVERHEAD_APPTRACE    1280
#अन्यथा
    #घोषणा ढेर_OVERHEAD_APPTRACE    0
#पूर्ण_अगर

/* Stack watchpoint decreases minimum usable stack size by up to 60 bytes.
 * See FreeRTOS FREERTOS_WATCHPOINT_END_OF_STACK option in Kconfig. */
#अगर CONFIG_FREERTOS_WATCHPOINT_END_OF_ढेर
    #घोषणा ढेर_OVERHEAD_WATCHPOINT    60
#अन्यथा
    #घोषणा ढेर_OVERHEAD_WATCHPOINT    0
#पूर्ण_अगर

#घोषणा config_ढेर_OVERHEAD_योग    \
    (                                 \
        ढेर_OVERHEAD_CHECKER +      \
        ढेर_OVERHEAD_OPTIMIZATION + \
        ढेर_OVERHEAD_APPTRACE +     \
        ढेर_OVERHEAD_WATCHPOINT     \
    )

/* The arch-specific FreeRTOSConfig_arch.h in esp_additions/arch_include/<arch>.
 * Placed here due to configSTACK_OVERHEAD_TOTAL. Todo: IDF-5712. */
#समावेश "freertos/FreeRTOSConfig_arch.h"

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

#घोषणा configUSE_PREEMPTION                         1
#घोषणा configUSE_TICKLESS_IDLE                      CONFIG_FREERTOS_USE_TICKLESS_IDLE
#अगर configUSE_TICKLESS_IDLE
    #घोषणा configEXPECTED_IDLE_TIME_BEFORE_SLEEP    CONFIG_FREERTOS_IDLE_TIME_BEFORE_SLEEP
#पूर्ण_अगर /* configUSE_TICKLESS_IDLE */
#घोषणा configCPU_CLOCK_HZ                           ( CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ * 1000000 )
#घोषणा configTICK_RATE_HZ                           CONFIG_FREERTOS_HZ
#घोषणा configMAX_PRIORITIES                         ( 25 )
#घोषणा configUSE_TIME_SLICING                       1
#घोषणा configUSE_16_BIT_TICKS                       0
#घोषणा configIDLE_SHOULD_YIELD                      0
#घोषणा configKERNEL_INTERRUPT_PRIORITY              1      /*Todo: This currently isn't used anywhere */
#घोषणा config_संख्या_OF_CORES                        CONFIG_FREERTOS_संख्या_OF_CORES
/* For compatibility */
#घोषणा config_अंक_CORES                              config_संख्या_OF_CORES

/* ------------- Synchronization Primitives ---------------- */

#घोषणा configUSE_MUTEXES                        1
#घोषणा configUSE_RECURSIVE_MUTEXES              1
#घोषणा configUSE_COUNTING_SEMAPHORES            1
#घोषणा configUSE_QUEUE_SETS                     1
#घोषणा config_QUEUE_REGISTRY_आकार                CONFIG_FREERTOS_QUEUE_REGISTRY_आकार
#घोषणा configUSE_TASK_NOTIFICATIONS             1
#घोषणा configTASK_NOTIFICATION_ARRAY_ENTRIES    CONFIG_FREERTOS_TASK_NOTIFICATION_ARRAY_ENTRIES

/* ----------------------- System -------------------------- */

#घोषणा configMAX_TASK_NAME_LEN    CONFIG_FREERTOS_MAX_TASK_NAME_LEN

/* If deletion callbacks are enabled, the number of TLSP's are doubled (i.e.,
 * the length of the TCB's pvThreadLocalStoragePointersThis array). This allows
 * the latter half of the array to store the deletion callback pointers (whereas
 * the first half stores the TLSPs themselves). */
#अगर CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
    #घोषणा config_अंक_THREAD_LOCAL_STORAGE_POINTERS    ( CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_POINTERS * 2 )
#अन्यथा /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
    #घोषणा config_अंक_THREAD_LOCAL_STORAGE_POINTERS    CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_POINTERS
#पूर्ण_अगर /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

#घोषणा config_ढेर_DEPTH_TYPE                         uint32_t
#अगर CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY
    #घोषणा configENABLE_BACKWARD_COMPATIBILITY        1
#अन्यथा /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */
    #घोषणा configENABLE_BACKWARD_COMPATIBILITY        0
#पूर्ण_अगर /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */
#घोषणा configASSERT( a )    निश्चित( a )

/* ----------------------- Memory  ------------------------- */

#घोषणा configSUPPORT_STATIC_ALLOCATION              1
#घोषणा configSUPPORT_DYNAMIC_ALLOCATION             1
#घोषणा configAPPLICATION_ALLOCATED_HEAP             1
#अगर CONFIG_FREERTOS_PLACE_TASK_STACKS_IN_EXT_RAM
    #घोषणा config_ढेर_ALLOCATION_FROM_SEPARATE_HEAP    1
#अन्यथा
    #घोषणा config_ढेर_ALLOCATION_FROM_SEPARATE_HEAP    0
#पूर्ण_अगर

/* ------------------------ Hooks -------------------------- */

#अगर CONFIG_FREERTOS_USE_IDLE_HOOK
    #घोषणा configUSE_IDLE_HOOK               1
#अन्यथा /* CONFIG_FREERTOS_USE_IDLE_HOOK */
    #घोषणा configUSE_IDLE_HOOK               0
#पूर्ण_अगर /* CONFIG_FREERTOS_USE_IDLE_HOOK */
#अगर CONFIG_FREERTOS_USE_TICK_HOOK
    #घोषणा configUSE_TICK_HOOK               1
#अन्यथा /* CONFIG_FREERTOS_USE_TICK_HOOK */
    #घोषणा configUSE_TICK_HOOK               0
#पूर्ण_अगर /* CONFIG_FREERTOS_USE_TICK_HOOK */
#अगर CONFIG_FREERTOS_CHECK_STACKOVERFLOW_NONE
    #घोषणा config_CHECK_FOR_ढेर_OVERFLOW    0
#या_अगर CONFIG_FREERTOS_CHECK_STACKOVERFLOW_PTRVAL
    #घोषणा config_CHECK_FOR_ढेर_OVERFLOW    1
#या_अगर CONFIG_FREERTOS_CHECK_STACKOVERFLOW_CANARY
    #घोषणा config_CHECK_FOR_ढेर_OVERFLOW    2
#पूर्ण_अगर /* CONFIG_FREERTOS_CHECK_STACKOVERFLOW_NONE */
#घोषणा config_RECORD_ढेर_ऊपरी_ADDRESS       1             /* This must be set as the port requires TCB.pxEndOfStack */

/* ------------------- Run-time Stats ---------------------- */

#अगर_घोषित CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS
    #घोषणा configGENERATE_RUN_TIME_STATS           1       /* Used by vTaskGetRunTimeStats() */
#पूर्ण_अगर /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
#अगर_घोषित CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS
    #घोषणा configUSE_STATS_FORMATTING_FUNCTIONS    1       /* Used by vTaskList() */
#पूर्ण_अगर /* CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS */

#अगर CONFIG_FREERTOS_RUN_TIME_गणक_TYPE_U32
    #घोषणा config_RUN_TIME_गणक_TYPE    uint32_t
#या_अगर CONFIG_FREERTOS_RUN_TIME_गणक_TYPE_U64
    #घोषणा config_RUN_TIME_गणक_TYPE    uint64_t
#पूर्ण_अगर /* CONFIG_FREERTOS_RUN_TIME_COUNTER_TYPE_U64 */

/* -------------------- Co-routines  ----------------------- */

#घोषणा configUSE_CO_ROUTINES              0              /* CO_ROUTINES are not supported in ESP-IDF */
#घोषणा configMAX_CO_ROUTINE_PRIORITIES    2

/* ------------------- Software Timer ---------------------- */

#अगर CONFIG_FREERTOS_USE_TIMERS
    #घोषणा configUSE_TIMERS                          1
    #घोषणा configTIMER_TASK_PRIORITY                 CONFIG_FREERTOS_TIMER_TASK_PRIORITY
    #घोषणा config_TIMER_QUEUE_लंबाई                  CONFIG_FREERTOS_TIMER_QUEUE_लंबाई
    #घोषणा config_TIMER_TASK_ढेर_DEPTH              CONFIG_FREERTOS_TIMER_TASK_ढेर_DEPTH
    #घोषणा configTIMER_SERVICE_TASK_NAME             CONFIG_FREERTOS_TIMER_SERVICE_TASK_NAME
    #घोषणा configTIMER_SERVICE_TASK_CORE_AFFINITY    CONFIG_FREERTOS_TIMER_SERVICE_TASK_CORE_AFFINITY
#अन्यथा
    #घोषणा configUSE_TIMERS                          0
#पूर्ण_अगर


/* ------------------------ List --------------------------- */

#घोषणा configLIST_VOLATILE    अस्थिर                     /* We define List elements as volatile to prevent the compiler from optimizing out essential code */

/* -------------------- API Includes ----------------------- */

#अगर CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY
    #घोषणा configENABLE_BACKWARD_COMPATIBILITY    1
#अन्यथा /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */
    #घोषणा configENABLE_BACKWARD_COMPATIBILITY    0
#पूर्ण_अगर /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */

#घोषणा INCLUDE_vTaskPrioritySet                   1
#घोषणा INCLUDE_uxTaskPriorityGet                  1
#घोषणा INCLUDE_vTaskDelete                        1
#घोषणा INCLUDE_vTaskSuspend                       1
#घोषणा INCLUDE_vTaskDelay                         1
#घोषणा INCLUDE_xTaskGetIdleTaskHandle             1
#घोषणा INCLUDE_xTaskAbortDelay                    1
#घोषणा INCLUDE_xSemaphoreGetMutexHolder           1
#घोषणा INCLUDE_xTaskGetHandle                     1
#घोषणा INCLUDE_ux_Task_Get_ढेर_ऊपरी_Water_Mark        1
#घोषणा INCLUDE_eTaskGetState                      1
#घोषणा INCLUDE_xTaskResumeFromISR                 1
#अगर CONFIG_FREERTOS_USE_TIMERS
  #घोषणा INCLUDE_xTimerPendFunctionCall           1
#अन्यथा
  #घोषणा INCLUDE_xTimerPendFunctionCall           0
#पूर्ण_अगर
#घोषणा INCLUDE_xTaskGetSchedulerState             1
#घोषणा INCLUDE_xTaskGetCurrentTaskHandle          1

/* -------------------- Trace Macros ----------------------- */

/*
 * For trace macros.
 * Note: Include trace macros here and not above as trace macros are dependent on some of the FreeRTOS configs
 */
#अगर_अघोषित __ASSEMBLER__
    #अगर CONFIG_ESP_TRACE_ENABLE
        #समावेश "esp_trace_freertos.h"
    #पूर्ण_अगर

    #अगर CONFIG_FREERTOS_SMP

/* Default values for trace macros added to ESP-IDF implementation of SYSVIEW
 * that is not part of Amazon SMP FreeRTOS. */
        #अगर_अघोषित traceISR_EXIT
            #घोषणा traceISR_EXIT()
        #पूर्ण_अगर
        #अगर_अघोषित traceISR_ENTER
            #घोषणा traceISR_ENTER( _n_ )
        #पूर्ण_अगर

        #अगर_अघोषित traceQUEUE_GIVE_FROM_ISR
            #घोषणा traceQUEUE_GIVE_FROM_ISR( pxQueue )
        #पूर्ण_अगर

        #अगर_अघोषित traceQUEUE_GIVE_FROM_ISR_FAILED
            #घोषणा traceQUEUE_GIVE_FROM_ISR_FAILED( pxQueue )
        #पूर्ण_अगर

        #अगर_अघोषित traceQUEUE_SEMAPHORE_RECEIVE
            #घोषणा traceQUEUE_SEMAPHORE_RECEIVE( pxQueue )
        #पूर्ण_अगर
    #पूर्ण_अगर /* CONFIG_FREERTOS_SMP */
#पूर्ण_अगर /* def __ASSEMBLER__ */

#अगर CONFIG_FREERTOS_USE_APPLICATION_TASK_TAG
    #घोषणा configUSE_APPLICATION_TASK_TAG    1
#पूर्ण_अगर // CONFIG_FREERTOS_USE_APPLICATION_TASK_TAG

/* -------------- List Data Integrity Checks --------------- */
#घोषणा config_USE_LIST_तथ्य_INTEGRITY_CHECK_BYTES    CONFIG_FREERTOS_USE_LIST_तथ्य_INTEGRITY_CHECK_BYTES

/* ----------------------------------------------- Amazon SMP FreeRTOS -------------------------------------------------
 * - All Amazon SMP FreeRTOS specific configurations
 * ------------------------------------------------------------------------------------------------------------------ */

#अगर CONFIG_FREERTOS_SMP && ( CONFIG_FREERTOS_संख्या_OF_CORES > 1 )
    #घोषणा configUSE_CORE_AFFINITY              1
    #घोषणा configRUN_MULTIPLE_PRIORITIES        1
    #घोषणा configUSE_TASK_PREEMPTION_DISABLE    1
#पूर्ण_अगर /* CONFIG_FREERTOS_SMP && ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */

/* -------------------------------------------------- IDF FreeRTOS -----------------------------------------------------
 * - All IDF FreeRTOS specific configurations
 * ------------------------------------------------------------------------------------------------------------------ */

#अगर !CONFIG_FREERTOS_SMP
    #अगर_घोषित CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID
        #घोषणा configTASKLIST_INCLUDE_COREID                  1
    #पूर्ण_अगर /* CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID */
    #अगर_घोषित CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
        #घोषणा configTHREAD_LOCAL_STORAGE_DELETE_CALLBACKS    1
    #पूर्ण_अगर /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
#पूर्ण_अगर /* !CONFIG_FREERTOS_SMP */

/* ------------------------------------------------ ESP-IDF Additions --------------------------------------------------
 * - Any other macros required by the rest of ESP-IDF
 * ------------------------------------------------------------------------------------------------------------------ */

/* portNUM_PROCESSORS is deprecated and will be removed in ESP-IDF v6.0 (IDF-8785)
 * Please use the Kconfig option CONFIG_FREERTOS_NUMBER_OF_CORES instead.
 */
#घोषणा port_अंक_PROCESSORS    config_संख्या_OF_CORES
