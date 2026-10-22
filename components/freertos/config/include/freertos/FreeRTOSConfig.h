/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#సూచన once

#అన్తర్భావ "sdkconfig.h"

/* This file gets pulled into assembly sources. Therefore, some includes need to
 * be wrapped in #ifndef __ASSEMBLER__ */

#నిర్వచితం_కాకపోతే __ASSEMBLER__
    /* For configASSERT() */
    #అన్తర్భావ <assert.h>
#అయితే_ముగింపు /* def __ASSEMBLER__ */

/* ----------------------------------------------------- Helpers -------------------------------------------------------
 * - Macros that the FreeRTOS configuration macros depend on
 * ------------------------------------------------------------------------------------------------------------------ */

/* Higher stack checker modes cause overhead on each function call */
#ఒకవేళ CONFIG_దొంతర_CHECK_ALL || CONFIG_దొంతర_CHECK_STRONG
    #నిర్వచించు దొంతర_OVERHEAD_CHECKER    256
#లేకపోతే
    #నిర్వచించు దొంతర_OVERHEAD_CHECKER    0
#అయితే_ముగింపు

/* with optimizations disabled, scheduler uses additional stack */
#ఒకవేళ CONFIG_COMPILER_OPTIMIZATION_NONE
    #నిర్వచించు దొంతర_OVERHEAD_OPTIMIZATION    320
#లేకపోతే
    #నిర్వచించు దొంతర_OVERHEAD_OPTIMIZATION    0
#అయితే_ముగింపు

/* apptrace module increases minimum stack usage */
#ఒకవేళ CONFIG_ESP_TRACE_TRANSPORT_APPTRACE
    #నిర్వచించు దొంతర_OVERHEAD_APPTRACE    1280
#లేకపోతే
    #నిర్వచించు దొంతర_OVERHEAD_APPTRACE    0
#అయితే_ముగింపు

/* Stack watchpoint decreases minimum usable stack size by up to 60 bytes.
 * See FreeRTOS FREERTOS_WATCHPOINT_END_OF_STACK option in Kconfig. */
#ఒకవేళ CONFIG_FREERTOS_WATCHPOINT_చివర_యొక్క_దొంతర
    #నిర్వచించు దొంతర_OVERHEAD_WATCHPOINT    60
#లేకపోతే
    #నిర్వచించు దొంతర_OVERHEAD_WATCHPOINT    0
#అయితే_ముగింపు

#నిర్వచించు config_దొంతర_OVERHEAD_మొత్తం    \
    (                                 \
        దొంతర_OVERHEAD_CHECKER +      \
        దొంతర_OVERHEAD_OPTIMIZATION + \
        దొంతర_OVERHEAD_APPTRACE +     \
        దొంతర_OVERHEAD_WATCHPOINT     \
    )

/* The arch-specific FreeRTOSConfig_arch.h in esp_additions/arch_include/<arch>.
 * Placed here due to configSTACK_OVERHEAD_TOTAL. Todo: IDF-5712. */
#అన్తర్భావ "freertos/FreeRTOSConfig_arch.h"

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

#నిర్వచించు configUSE_PREEMPTION                         1
#నిర్వచించు configUSE_TICKLESS_IDLE                      CONFIG_FREERTOS_USE_TICKLESS_IDLE
#ఒకవేళ configUSE_TICKLESS_IDLE
    #నిర్వచించు config_EXPECTED_IDLE_సమయం_BEFORE_SLEEP    CONFIG_FREERTOS_IDLE_సమయం_BEFORE_SLEEP
#అయితే_ముగింపు /* configUSE_TICKLESS_IDLE */
#నిర్వచించు configCPU_CLOCK_HZ                           ( CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ * 1000000 )
#నిర్వచించు config_TICK_రేటు_HZ                           CONFIG_FREERTOS_HZ
#నిర్వచించు configMAX_PRIORITIES                         ( 25 )
#నిర్వచించు config_USE_సమయం_SLICING                       1
#నిర్వచించు config_USE_16_బిట్టు_TICKS                       0
#నిర్వచించు configIDLE_SHOULD_YIELD                      0
#నిర్వచించు config_KERNEL_అంతరాయం_ప్రాధాన్యత              1      /*Todo: This currently isn't used anywhere */
#నిర్వచించు config_సంఖ్య_యొక్క_CORES                        CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES
/* For compatibility */
#నిర్వచించు config_అంకె_CORES                              config_సంఖ్య_యొక్క_CORES

/* ------------- Synchronization Primitives ---------------- */

#నిర్వచించు configUSE_MUTEXES                        1
#నిర్వచించు configUSE_RECURSIVE_MUTEXES              1
#నిర్వచించు configUSE_COUNTING_SEMAPHORES            1
#నిర్వచించు config_USE_వరుస_SETS                     1
#నిర్వచించు config_వరుస_REGISTRY_కొలత                CONFIG_FREERTOS_వరుస_REGISTRY_కొలత
#నిర్వచించు configUSE_TASK_NOTIFICATIONS             1
#నిర్వచించు configTASK_NOTIFICATION_ARRAY_ENTRIES    CONFIG_FREERTOS_TASK_NOTIFICATION_ARRAY_ENTRIES

/* ----------------------- System -------------------------- */

#నిర్వచించు configMAX_TASK_NAME_LEN    CONFIG_FREERTOS_MAX_TASK_NAME_LEN

/* If deletion callbacks are enabled, the number of TLSP's are doubled (i.e.,
 * the length of the TCB's pvThreadLocalStoragePointersThis array). This allows
 * the latter half of the array to store the deletion callback pointers (whereas
 * the first half stores the TLSPs themselves). */
#ఒకవేళ CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
    #నిర్వచించు config_అంకె_THREAD_LOCAL_STORAGE_సూచికలు    ( CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_సూచికలు * 2 )
#లేకపోతే /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
    #నిర్వచించు config_అంకె_THREAD_LOCAL_STORAGE_సూచికలు    CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_సూచికలు
#అయితే_ముగింపు /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

#నిర్వచించు config_దొంతర_లోతు_TYPE                         uint32_t
#ఒకవేళ CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY
    #నిర్వచించు configENABLE_BACKWARD_COMPATIBILITY        1
#లేకపోతే /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */
    #నిర్వచించు configENABLE_BACKWARD_COMPATIBILITY        0
#అయితే_ముగింపు /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */
#నిర్వచించు configASSERT( a )    ప్రతిజ్ఞా( a )

/* ----------------------- Memory  ------------------------- */

#నిర్వచించు config_SUPPORT_STATIC_కేటాయింపు              1
#నిర్వచించు config_SUPPORT_DYNAMIC_కేటాయింపు             1
#నిర్వచించు config_APPLICATION_ALLOCATED_కుప్ప             1
#ఒకవేళ CONFIG_FREERTOS_ఉంచు_TASK_STACKS_లోపల_EXT_RAM
    #నిర్వచించు config_దొంతర_కేటాయింపు_FROM_SEPARATE_కుప్ప    1
#లేకపోతే
    #నిర్వచించు config_దొంతర_కేటాయింపు_FROM_SEPARATE_కుప్ప    0
#అయితే_ముగింపు

/* ------------------------ Hooks -------------------------- */

#ఒకవేళ CONFIG_FREERTOS_USE_IDLE_HOOK
    #నిర్వచించు configUSE_IDLE_HOOK               1
#లేకపోతే /* CONFIG_FREERTOS_USE_IDLE_HOOK */
    #నిర్వచించు configUSE_IDLE_HOOK               0
#అయితే_ముగింపు /* CONFIG_FREERTOS_USE_IDLE_HOOK */
#ఒకవేళ CONFIG_FREERTOS_USE_TICK_HOOK
    #నిర్వచించు configUSE_TICK_HOOK               1
#లేకపోతే /* CONFIG_FREERTOS_USE_TICK_HOOK */
    #నిర్వచించు configUSE_TICK_HOOK               0
#అయితే_ముగింపు /* CONFIG_FREERTOS_USE_TICK_HOOK */
#ఒకవేళ CONFIG_FREERTOS_CHECK_STACKOVERFLOW_NONE
    #నిర్వచించు config_CHECK_FOR_దొంతర_అధికప్రవాహం    0
#లేదావేళ CONFIG_FREERTOS_CHECK_STACKOVERFLOW_PTRVAL
    #నిర్వచించు config_CHECK_FOR_దొంతర_అధికప్రవాహం    1
#లేదావేళ CONFIG_FREERTOS_CHECK_STACKOVERFLOW_CANARY
    #నిర్వచించు config_CHECK_FOR_దొంతర_అధికప్రవాహం    2
#అయితే_ముగింపు /* CONFIG_FREERTOS_CHECK_STACKOVERFLOW_NONE */
#నిర్వచించు config_రికార్డు_దొంతర_ఎగువ_చిరునామా       1             /* This must be set as the port requires TCB.pxEndOfStack */

/* ------------------- Run-time Stats ---------------------- */

#నిర్వచితమైతే CONFIG_FREERTOS_GENERATE_నిరంతరం_సమయం_STATS
    #నిర్వచించు config_GENERATE_నిరంతరం_సమయం_STATS           1       /* Used by vTaskGetRunTimeStats() */
#అయితే_ముగింపు /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
#నిర్వచితమైతే CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS
    #నిర్వచించు configUSE_STATS_FORMATTING_FUNCTIONS    1       /* Used by vTaskList() */
#అయితే_ముగింపు /* CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS */

#ఒకవేళ CONFIG_FREERTOS_నిరంతరం_సమయం_COUNTER_TYPE_U32
    #నిర్వచించు config_నిరంతరం_సమయం_COUNTER_TYPE    uint32_t
#లేదావేళ CONFIG_FREERTOS_నిరంతరం_సమయం_COUNTER_TYPE_U64
    #నిర్వచించు config_నిరంతరం_సమయం_COUNTER_TYPE    uint64_t
#అయితే_ముగింపు /* CONFIG_FREERTOS_RUN_TIME_COUNTER_TYPE_U64 */

/* -------------------- Co-routines  ----------------------- */

#నిర్వచించు configUSE_CO_ROUTINES              0              /* CO_ROUTINES are not supported in ESP-IDF */
#నిర్వచించు configMAX_CO_ROUTINE_PRIORITIES    2

/* ------------------- Software Timer ---------------------- */

#ఒకవేళ CONFIG_FREERTOS_USE_TIMERS
    #నిర్వచించు configUSE_TIMERS                          1
    #నిర్వచించు config_TIMER_TASK_ప్రాధాన్యత                 CONFIG_FREERTOS_TIMER_TASK_ప్రాధాన్యత
    #నిర్వచించు config_TIMER_వరుస_నిడివి                  CONFIG_FREERTOS_TIMER_వరుస_నిడివి
    #నిర్వచించు config_TIMER_TASK_దొంతర_లోతు              CONFIG_FREERTOS_TIMER_TASK_దొంతర_లోతు
    #నిర్వచించు configTIMER_SERVICE_TASK_NAME             CONFIG_FREERTOS_TIMER_SERVICE_TASK_NAME
    #నిర్వచించు configTIMER_SERVICE_TASK_CORE_AFFINITY    CONFIG_FREERTOS_TIMER_SERVICE_TASK_CORE_AFFINITY
#లేకపోతే
    #నిర్వచించు configUSE_TIMERS                          0
#అయితే_ముగింపు


/* ------------------------ List --------------------------- */

#నిర్వచించు configLIST_VOLATILE    అస్థిర                     /* We define List elements as volatile to prevent the compiler from optimizing out essential code */

/* -------------------- API Includes ----------------------- */

#ఒకవేళ CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY
    #నిర్వచించు configENABLE_BACKWARD_COMPATIBILITY    1
#లేకపోతే /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */
    #నిర్వచించు configENABLE_BACKWARD_COMPATIBILITY    0
#అయితే_ముగింపు /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */

#నిర్వచించు INCLUDE_v_Task_ప్రాధాన్యత_సమితి                   1
#నిర్వచించు INCLUDE_ux_Task_ప్రాధాన్యత_Get                  1
#నిర్వచించు INCLUDE_vTaskDelete                        1
#నిర్వచించు INCLUDE_vTaskSuspend                       1
#నిర్వచించు INCLUDE_vTaskDelay                         1
#నిర్వచించు INCLUDE_xTaskGetIdleTaskHandle             1
#నిర్వచించు INCLUDE_xTaskAbortDelay                    1
#నిర్వచించు INCLUDE_xSemaphoreGetMutexHolder           1
#నిర్వచించు INCLUDE_xTaskGetHandle                     1
#నిర్వచించు INCLUDE_ux_Task_Get_దొంతర_ఎగువ_Water_Mark        1
#నిర్వచించు INCLUDE_eTaskGetState                      1
#నిర్వచించు INCLUDE_xTaskResumeFromISR                 1
#ఒకవేళ CONFIG_FREERTOS_USE_TIMERS
  #నిర్వచించు INCLUDE_xTimerPendFunctionCall           1
#లేకపోతే
  #నిర్వచించు INCLUDE_xTimerPendFunctionCall           0
#అయితే_ముగింపు
#నిర్వచించు INCLUDE_xTaskGetSchedulerState             1
#నిర్వచించు INCLUDE_xTaskGetCurrentTaskHandle          1

/* -------------------- Trace Macros ----------------------- */

/*
 * For trace macros.
 * Note: Include trace macros here and not above as trace macros are dependent on some of the FreeRTOS configs
 */
#నిర్వచితం_కాకపోతే __ASSEMBLER__
    #ఒకవేళ CONFIG_ESP_TRACE_ENABLE
        #అన్తర్భావ "esp_trace_freertos.h"
    #అయితే_ముగింపు

    #ఒకవేళ CONFIG_FREERTOS_SMP

/* Default values for trace macros added to ESP-IDF implementation of SYSVIEW
 * that is not part of Amazon SMP FreeRTOS. */
        #నిర్వచితం_కాకపోతే traceISR_EXIT
            #నిర్వచించు traceISR_EXIT()
        #అయితే_ముగింపు
        #నిర్వచితం_కాకపోతే traceISR_ENTER
            #నిర్వచించు traceISR_ENTER( _n_ )
        #అయితే_ముగింపు

        #నిర్వచితం_కాకపోతే trace_వరుస_GIVE_FROM_ISR
            #నిర్వచించు trace_వరుస_GIVE_FROM_ISR( px_వరుస )
        #అయితే_ముగింపు

        #నిర్వచితం_కాకపోతే trace_వరుస_GIVE_FROM_ISR_FAILED
            #నిర్వచించు trace_వరుస_GIVE_FROM_ISR_FAILED( px_వరుస )
        #అయితే_ముగింపు

        #నిర్వచితం_కాకపోతే trace_వరుస_SEMAPHORE_RECEIVE
            #నిర్వచించు trace_వరుస_SEMAPHORE_RECEIVE( px_వరుస )
        #అయితే_ముగింపు
    #అయితే_ముగింపు /* CONFIG_FREERTOS_SMP */
#అయితే_ముగింపు /* def __ASSEMBLER__ */

#ఒకవేళ CONFIG_FREERTOS_USE_APPLICATION_TASK_ట్యాగ్
    #నిర్వచించు config_USE_APPLICATION_TASK_ట్యాగ్    1
#అయితే_ముగింపు // CONFIG_FREERTOS_USE_APPLICATION_TASK_TAG

/* -------------- List Data Integrity Checks --------------- */
#నిర్వచించు config_USE_LIST_దత్తాంశం_INTEGRITY_CHECK_బైట్లు    CONFIG_FREERTOS_USE_LIST_దత్తాంశం_INTEGRITY_CHECK_బైట్లు

/* ----------------------------------------------- Amazon SMP FreeRTOS -------------------------------------------------
 * - All Amazon SMP FreeRTOS specific configurations
 * ------------------------------------------------------------------------------------------------------------------ */

#ఒకవేళ CONFIG_FREERTOS_SMP && ( CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES > 1 )
    #నిర్వచించు configUSE_CORE_AFFINITY              1
    #నిర్వచించు config_నిరంతరం_గుణిజం_PRIORITIES        1
    #నిర్వచించు configUSE_TASK_PREEMPTION_DISABLE    1
#అయితే_ముగింపు /* CONFIG_FREERTOS_SMP && ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */

/* -------------------------------------------------- IDF FreeRTOS -----------------------------------------------------
 * - All IDF FreeRTOS specific configurations
 * ------------------------------------------------------------------------------------------------------------------ */

#ఒకవేళ !CONFIG_FREERTOS_SMP
    #నిర్వచితమైతే CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID
        #నిర్వచించు configTASKLIST_INCLUDE_COREID                  1
    #అయితే_ముగింపు /* CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID */
    #నిర్వచితమైతే CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
        #నిర్వచించు configTHREAD_LOCAL_STORAGE_DELETE_CALLBACKS    1
    #అయితే_ముగింపు /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
#అయితే_ముగింపు /* !CONFIG_FREERTOS_SMP */

/* ------------------------------------------------ ESP-IDF Additions --------------------------------------------------
 * - Any other macros required by the rest of ESP-IDF
 * ------------------------------------------------------------------------------------------------------------------ */

/* portNUM_PROCESSORS is deprecated and will be removed in ESP-IDF v6.0 (IDF-8785)
 * Please use the Kconfig option CONFIG_FREERTOS_NUMBER_OF_CORES instead.
 */
#నిర్వచించు పోర్టు_అంకె_PROCESSORS    config_సంఖ్య_యొక్క_CORES
