/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"

/* This file gets pulled into assembly sources. Therefore, some includes need to
 * be wrapped in #ifndef __ASSEMBLER__ */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __ASSEMBLER__
    /* For configASSERT() */
    #ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਜੇ_ਅੰਤ /* def __ASSEMBLER__ */

/* ----------------------------------------------------- Helpers -------------------------------------------------------
 * - Macros that the FreeRTOS configuration macros depend on
 * ------------------------------------------------------------------------------------------------------------------ */

/* Higher stack checker modes cause overhead on each function call */
#ਜੇ CONFIG_ਢੇਰ_CHECK_ALL || CONFIG_ਢੇਰ_CHECK_STRONG
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਢੇਰ_OVERHEAD_CHECKER    256
#ਨਹੀਂਤਾਂ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਢੇਰ_OVERHEAD_CHECKER    0
#ਜੇ_ਅੰਤ

/* with optimizations disabled, scheduler uses additional stack */
#ਜੇ CONFIG_COMPILER_OPTIMIZATION_NONE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਢੇਰ_OVERHEAD_OPTIMIZATION    320
#ਨਹੀਂਤਾਂ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਢੇਰ_OVERHEAD_OPTIMIZATION    0
#ਜੇ_ਅੰਤ

/* apptrace module increases minimum stack usage */
#ਜੇ CONFIG_ESP_TRACE_TRANSPORT_APPTRACE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਢੇਰ_OVERHEAD_APPTRACE    1280
#ਨਹੀਂਤਾਂ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਢੇਰ_OVERHEAD_APPTRACE    0
#ਜੇ_ਅੰਤ

/* Stack watchpoint decreases minimum usable stack size by up to 60 bytes.
 * See FreeRTOS FREERTOS_WATCHPOINT_END_OF_STACK option in Kconfig. */
#ਜੇ CONFIG_FREERTOS_WATCHPOINT_ਸਿਰਾ_ਦਾ_ਢੇਰ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਢੇਰ_OVERHEAD_WATCHPOINT    60
#ਨਹੀਂਤਾਂ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਢੇਰ_OVERHEAD_WATCHPOINT    0
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਢੇਰ_OVERHEAD_ਕੁੱਲ    \
    (                                 \
        ਢੇਰ_OVERHEAD_CHECKER +      \
        ਢੇਰ_OVERHEAD_OPTIMIZATION + \
        ਢੇਰ_OVERHEAD_APPTRACE +     \
        ਢੇਰ_OVERHEAD_WATCHPOINT     \
    )

/* The arch-specific FreeRTOSConfig_arch.h in esp_additions/arch_include/<arch>.
 * Placed here due to configSTACK_OVERHEAD_TOTAL. Todo: IDF-5712. */
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOSConfig_arch.h"

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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_PREEMPTION                         1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_TICKLESS_IDLE                      CONFIG_FREERTOS_USE_TICKLESS_IDLE
#ਜੇ configUSE_TICKLESS_IDLE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_EXPECTED_IDLE_ਸਮਾਂ_BEFORE_SLEEP    CONFIG_FREERTOS_IDLE_ਸਮਾਂ_BEFORE_SLEEP
#ਜੇ_ਅੰਤ /* configUSE_TICKLESS_IDLE */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configCPU_CLOCK_HZ                           ( CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ * 1000000 )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_TICK_ਦਰ_HZ                           CONFIG_FREERTOS_HZ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configMAX_PRIORITIES                         ( 25 )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_USE_ਸਮਾਂ_SLICING                       1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_USE_16_ਦੋਆਧਾਰੀ_ਅੰਕ_TICKS                       0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configIDLE_SHOULD_YIELD                      0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_KERNEL_ਰੁਕਾਵਟ_ਤਰਜੀਹ              1      /*Todo: This currently isn't used anywhere */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਸੰਖਿਆ_ਦਾ_CORES                        CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES
/* For compatibility */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਅੰਕ_CORES                              config_ਸੰਖਿਆ_ਦਾ_CORES

/* ------------- Synchronization Primitives ---------------- */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_MUTEXES                        1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_RECURSIVE_MUTEXES              1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_COUNTING_SEMAPHORES            1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_USE_ਕਤਾਰ_SETS                     1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਕਤਾਰ_REGISTRY_ਮਾਪ                CONFIG_FREERTOS_ਕਤਾਰ_REGISTRY_ਮਾਪ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_TASK_NOTIFICATIONS             1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configTASK_NOTIFICATION_ARRAY_ENTRIES    CONFIG_FREERTOS_TASK_NOTIFICATION_ARRAY_ENTRIES

/* ----------------------- System -------------------------- */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configMAX_TASK_NAME_LEN    CONFIG_FREERTOS_MAX_TASK_NAME_LEN

/* If deletion callbacks are enabled, the number of TLSP's are doubled (i.e.,
 * the length of the TCB's pvThreadLocalStoragePointersThis array). This allows
 * the latter half of the array to store the deletion callback pointers (whereas
 * the first half stores the TLSPs themselves). */
#ਜੇ CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਅੰਕ_THREAD_LOCAL_STORAGE_ਸੂਚਕ_ਸਾਰੇ    ( CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_ਸੂਚਕ_ਸਾਰੇ * 2 )
#ਨਹੀਂਤਾਂ /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਅੰਕ_THREAD_LOCAL_STORAGE_ਸੂਚਕ_ਸਾਰੇ    CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_ਸੂਚਕ_ਸਾਰੇ
#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਢੇਰ_ਡੂੰਘਾਈ_TYPE                         uint32_t
#ਜੇ CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configENABLE_BACKWARD_COMPATIBILITY        1
#ਨਹੀਂਤਾਂ /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configENABLE_BACKWARD_COMPATIBILITY        0
#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configASSERT( a )    ਪ੍ਰਤਿਜ੍ਞਾ( a )

/* ----------------------- Memory  ------------------------- */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_SUPPORT_STATIC_ਵੰਡ              1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_SUPPORT_DYNAMIC_ਵੰਡ             1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_APPLICATION_ALLOCATED_ਢੇਰ             1
#ਜੇ CONFIG_FREERTOS_ਰੱਖੋ_TASK_STACKS_ਵਿੱਚ_EXT_RAM
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਢੇਰ_ਵੰਡ_FROM_SEPARATE_ਢੇਰ    1
#ਨਹੀਂਤਾਂ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਢੇਰ_ਵੰਡ_FROM_SEPARATE_ਢੇਰ    0
#ਜੇ_ਅੰਤ

/* ------------------------ Hooks -------------------------- */

#ਜੇ CONFIG_FREERTOS_USE_IDLE_HOOK
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_IDLE_HOOK               1
#ਨਹੀਂਤਾਂ /* CONFIG_FREERTOS_USE_IDLE_HOOK */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_IDLE_HOOK               0
#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_USE_IDLE_HOOK */
#ਜੇ CONFIG_FREERTOS_USE_TICK_HOOK
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_TICK_HOOK               1
#ਨਹੀਂਤਾਂ /* CONFIG_FREERTOS_USE_TICK_HOOK */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_TICK_HOOK               0
#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_USE_TICK_HOOK */
#ਜੇ CONFIG_FREERTOS_CHECK_STACKOVERFLOW_NONE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_CHECK_FOR_ਢੇਰ_ਉਛਾਲ    0
#ਜਾਂਜੇ CONFIG_FREERTOS_CHECK_STACKOVERFLOW_PTRVAL
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_CHECK_FOR_ਢੇਰ_ਉਛਾਲ    1
#ਜਾਂਜੇ CONFIG_FREERTOS_CHECK_STACKOVERFLOW_CANARY
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_CHECK_FOR_ਢੇਰ_ਉਛਾਲ    2
#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_CHECK_STACKOVERFLOW_NONE */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਰਿਕਾਰਡ_ਢੇਰ_ਉੱਚਾ_ਪਤਾ       1             /* This must be set as the port requires TCB.pxEndOfStack */

/* ------------------- Run-time Stats ---------------------- */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FREERTOS_GENERATE_ਲੜੀਵਾਰ_ਸਮਾਂ_STATS
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_GENERATE_ਲੜੀਵਾਰ_ਸਮਾਂ_STATS           1       /* Used by vTaskGetRunTimeStats() */
#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_STATS_FORMATTING_FUNCTIONS    1       /* Used by vTaskList() */
#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS */

#ਜੇ CONFIG_FREERTOS_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_TYPE_U32
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_TYPE    uint32_t
#ਜਾਂਜੇ CONFIG_FREERTOS_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_TYPE_U64
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_TYPE    uint64_t
#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_RUN_TIME_COUNTER_TYPE_U64 */

/* -------------------- Co-routines  ----------------------- */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_CO_ROUTINES              0              /* CO_ROUTINES are not supported in ESP-IDF */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configMAX_CO_ROUTINE_PRIORITIES    2

/* ------------------- Software Timer ---------------------- */

#ਜੇ CONFIG_FREERTOS_USE_TIMERS
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_TIMERS                          1
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_TIMER_TASK_ਤਰਜੀਹ                 CONFIG_FREERTOS_TIMER_TASK_ਤਰਜੀਹ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_TIMER_ਕਤਾਰ_ਲੰਮਾਈ                  CONFIG_FREERTOS_TIMER_ਕਤਾਰ_ਲੰਮਾਈ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_TIMER_TASK_ਢੇਰ_ਡੂੰਘਾਈ              CONFIG_FREERTOS_TIMER_TASK_ਢੇਰ_ਡੂੰਘਾਈ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configTIMER_SERVICE_TASK_NAME             CONFIG_FREERTOS_TIMER_SERVICE_TASK_NAME
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configTIMER_SERVICE_TASK_CORE_AFFINITY    CONFIG_FREERTOS_TIMER_SERVICE_TASK_CORE_AFFINITY
#ਨਹੀਂਤਾਂ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_TIMERS                          0
#ਜੇ_ਅੰਤ


/* ------------------------ List --------------------------- */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configLIST_VOLATILE    ਅਸਥਿਰ                     /* We define List elements as volatile to prevent the compiler from optimizing out essential code */

/* -------------------- API Includes ----------------------- */

#ਜੇ CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configENABLE_BACKWARD_COMPATIBILITY    1
#ਨਹੀਂਤਾਂ /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configENABLE_BACKWARD_COMPATIBILITY    0
#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_v_Task_ਤਰਜੀਹ_ਸੈੱਟ                   1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_ux_Task_ਤਰਜੀਹ_Get                  1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_vTaskDelete                        1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_vTaskSuspend                       1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_vTaskDelay                         1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_xTaskGetIdleTaskHandle             1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_xTaskAbortDelay                    1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_xSemaphoreGetMutexHolder           1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_xTaskGetHandle                     1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_ux_Task_Get_ਢੇਰ_ਉੱਚਾ_Water_Mark        1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_eTaskGetState                      1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_xTaskResumeFromISR                 1
#ਜੇ CONFIG_FREERTOS_USE_TIMERS
  #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_xTimerPendFunctionCall           1
#ਨਹੀਂਤਾਂ
  #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_xTimerPendFunctionCall           0
#ਜੇ_ਅੰਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_xTaskGetSchedulerState             1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_xTaskGetCurrentTaskHandle          1

/* -------------------- Trace Macros ----------------------- */

/*
 * For trace macros.
 * Note: Include trace macros here and not above as trace macros are dependent on some of the FreeRTOS configs
 */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __ASSEMBLER__
    #ਜੇ CONFIG_ESP_TRACE_ENABLE
        #ਅਨ੍ਤਰ੍ਭਾਵ "esp_trace_freertos.h"
    #ਜੇ_ਅੰਤ

    #ਜੇ CONFIG_FREERTOS_SMP

/* Default values for trace macros added to ESP-IDF implementation of SYSVIEW
 * that is not part of Amazon SMP FreeRTOS. */
        #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceISR_EXIT
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceISR_EXIT()
        #ਜੇ_ਅੰਤ
        #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceISR_ENTER
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceISR_ENTER( _n_ )
        #ਜੇ_ਅੰਤ

        #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਕਤਾਰ_GIVE_FROM_ISR
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਕਤਾਰ_GIVE_FROM_ISR( px_ਕਤਾਰ )
        #ਜੇ_ਅੰਤ

        #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਕਤਾਰ_GIVE_FROM_ISR_FAILED
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਕਤਾਰ_GIVE_FROM_ISR_FAILED( px_ਕਤਾਰ )
        #ਜੇ_ਅੰਤ

        #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਕਤਾਰ_SEMAPHORE_RECEIVE
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਕਤਾਰ_SEMAPHORE_RECEIVE( px_ਕਤਾਰ )
        #ਜੇ_ਅੰਤ
    #ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_SMP */
#ਜੇ_ਅੰਤ /* def __ASSEMBLER__ */

#ਜੇ CONFIG_FREERTOS_USE_APPLICATION_TASK_ਟੈਗ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_USE_APPLICATION_TASK_ਟੈਗ    1
#ਜੇ_ਅੰਤ // CONFIG_FREERTOS_USE_APPLICATION_TASK_TAG

/* -------------- List Data Integrity Checks --------------- */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_USE_LIST_ਅੰਕੜੇ_INTEGRITY_CHECK_ਬਾਈਟਾਂ    CONFIG_FREERTOS_USE_LIST_ਅੰਕੜੇ_INTEGRITY_CHECK_ਬਾਈਟਾਂ

/* ----------------------------------------------- Amazon SMP FreeRTOS -------------------------------------------------
 * - All Amazon SMP FreeRTOS specific configurations
 * ------------------------------------------------------------------------------------------------------------------ */

#ਜੇ CONFIG_FREERTOS_SMP && ( CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES > 1 )
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_CORE_AFFINITY              1
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਲੜੀਵਾਰ_ਗੁਣਜ_PRIORITIES        1
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_TASK_PREEMPTION_DISABLE    1
#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_SMP && ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */

/* -------------------------------------------------- IDF FreeRTOS -----------------------------------------------------
 * - All IDF FreeRTOS specific configurations
 * ------------------------------------------------------------------------------------------------------------------ */

#ਜੇ !CONFIG_FREERTOS_SMP
    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configTASKLIST_INCLUDE_COREID                  1
    #ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID */
    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configTHREAD_LOCAL_STORAGE_DELETE_CALLBACKS    1
    #ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
#ਜੇ_ਅੰਤ /* !CONFIG_FREERTOS_SMP */

/* ------------------------------------------------ ESP-IDF Additions --------------------------------------------------
 * - Any other macros required by the rest of ESP-IDF
 * ------------------------------------------------------------------------------------------------------------------ */

/* portNUM_PROCESSORS is deprecated and will be removed in ESP-IDF v6.0 (IDF-8785)
 * Please use the Kconfig option CONFIG_FREERTOS_NUMBER_OF_CORES instead.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਅੰਕ_PROCESSORS    config_ਸੰਖਿਆ_ਦਾ_CORES
