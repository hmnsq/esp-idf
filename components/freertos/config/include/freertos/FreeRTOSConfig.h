/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"

/* This file gets pulled into assembly sources. Therefore, some includes need to
 * be wrapped in #ifndef __ASSEMBLER__ */

#ifndef __ASSEMBLER__
    /* For configASSERT() */
    #ಅನ್ತರ್ಭಾವ <assert.h>
#endif /* def __ASSEMBLER__ */

/* ----------------------------------------------------- Helpers -------------------------------------------------------
 * - Macros that the FreeRTOS configuration macros depend on
 * ------------------------------------------------------------------------------------------------------------------ */

/* Higher stack checker modes cause overhead on each function call */
#ಯದಿ CONFIG_ರಾಶಿ_CHECK_ALL || CONFIG_ರಾಶಿ_CHECK_STRONG
    #define ರಾಶಿ_OVERHEAD_CHECKER    256
#ಅನ್ಯಥಾ
    #define ರಾಶಿ_OVERHEAD_CHECKER    0
#endif

/* with optimizations disabled, scheduler uses additional stack */
#ಯದಿ CONFIG_COMPILER_OPTIMIZATION_NONE
    #define ರಾಶಿ_OVERHEAD_OPTIMIZATION    320
#ಅನ್ಯಥಾ
    #define ರಾಶಿ_OVERHEAD_OPTIMIZATION    0
#endif

/* apptrace module increases minimum stack usage */
#ಯದಿ CONFIG_ESP_TRACE_TRANSPORT_APPTRACE
    #define ರಾಶಿ_OVERHEAD_APPTRACE    1280
#ಅನ್ಯಥಾ
    #define ರಾಶಿ_OVERHEAD_APPTRACE    0
#endif

/* Stack watchpoint decreases minimum usable stack size by up to 60 bytes.
 * See FreeRTOS FREERTOS_WATCHPOINT_END_OF_STACK option in Kconfig. */
#ಯದಿ CONFIG_FREERTOS_WATCHPOINT_END_OF_ರಾಶಿ
    #define ರಾಶಿ_OVERHEAD_WATCHPOINT    60
#ಅನ್ಯಥಾ
    #define ರಾಶಿ_OVERHEAD_WATCHPOINT    0
#endif

#define config_ರಾಶಿ_OVERHEAD_TOTAL    \
    (                                 \
        ರಾಶಿ_OVERHEAD_CHECKER +      \
        ರಾಶಿ_OVERHEAD_OPTIMIZATION + \
        ರಾಶಿ_OVERHEAD_APPTRACE +     \
        ರಾಶಿ_OVERHEAD_WATCHPOINT     \
    )

/* The arch-specific FreeRTOSConfig_arch.h in esp_additions/arch_include/<arch>.
 * Placed here due to configSTACK_OVERHEAD_TOTAL. Todo: IDF-5712. */
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOSConfig_arch.h"

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

#define configUSE_PREEMPTION                         1
#define configUSE_TICKLESS_IDLE                      CONFIG_FREERTOS_USE_TICKLESS_IDLE
#ಯದಿ configUSE_TICKLESS_IDLE
    #define configEXPECTED_IDLE_TIME_BEFORE_SLEEP    CONFIG_FREERTOS_IDLE_TIME_BEFORE_SLEEP
#endif /* configUSE_TICKLESS_IDLE */
#define configCPU_CLOCK_HZ                           ( CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ * 1000000 )
#define configTICK_RATE_HZ                           CONFIG_FREERTOS_HZ
#define configMAX_PRIORITIES                         ( 25 )
#define configUSE_TIME_SLICING                       1
#define configUSE_16_BIT_TICKS                       0
#define configIDLE_SHOULD_YIELD                      0
#define configKERNEL_INTERRUPT_PRIORITY              1      /*Todo: This currently isn't used anywhere */
#define config_ಸಂಖ್ಯೆ_OF_CORES                        CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES
/* For compatibility */
#define config_ಅಂಕೆ_CORES                              config_ಸಂಖ್ಯೆ_OF_CORES

/* ------------- Synchronization Primitives ---------------- */

#define configUSE_MUTEXES                        1
#define configUSE_RECURSIVE_MUTEXES              1
#define configUSE_COUNTING_SEMAPHORES            1
#define configUSE_QUEUE_SETS                     1
#define configQUEUE_REGISTRY_SIZE                CONFIG_FREERTOS_QUEUE_REGISTRY_SIZE
#define configUSE_TASK_NOTIFICATIONS             1
#define configTASK_NOTIFICATION_ARRAY_ENTRIES    CONFIG_FREERTOS_TASK_NOTIFICATION_ARRAY_ENTRIES

/* ----------------------- System -------------------------- */

#define configMAX_TASK_NAME_LEN    CONFIG_FREERTOS_MAX_TASK_NAME_LEN

/* If deletion callbacks are enabled, the number of TLSP's are doubled (i.e.,
 * the length of the TCB's pvThreadLocalStoragePointersThis array). This allows
 * the latter half of the array to store the deletion callback pointers (whereas
 * the first half stores the TLSPs themselves). */
#ಯದಿ CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
    #define config_ಅಂಕೆ_THREAD_LOCAL_STORAGE_POINTERS    ( CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_POINTERS * 2 )
#ಅನ್ಯಥಾ /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
    #define config_ಅಂಕೆ_THREAD_LOCAL_STORAGE_POINTERS    CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_POINTERS
#endif /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

#define config_ರಾಶಿ_DEPTH_TYPE                         uint32_t
#ಯದಿ CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY
    #define configENABLE_BACKWARD_COMPATIBILITY        1
#ಅನ್ಯಥಾ /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */
    #define configENABLE_BACKWARD_COMPATIBILITY        0
#endif /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */
#define configASSERT( a )    ಪ್ರತಿಜ್ಞಾ( a )

/* ----------------------- Memory  ------------------------- */

#define configSUPPORT_STATIC_ALLOCATION              1
#define configSUPPORT_DYNAMIC_ALLOCATION             1
#define configAPPLICATION_ALLOCATED_HEAP             1
#ಯದಿ CONFIG_FREERTOS_PLACE_TASK_STACKS_IN_EXT_RAM
    #define config_ರಾಶಿ_ALLOCATION_FROM_SEPARATE_HEAP    1
#ಅನ್ಯಥಾ
    #define config_ರಾಶಿ_ALLOCATION_FROM_SEPARATE_HEAP    0
#endif

/* ------------------------ Hooks -------------------------- */

#ಯದಿ CONFIG_FREERTOS_USE_IDLE_HOOK
    #define configUSE_IDLE_HOOK               1
#ಅನ್ಯಥಾ /* CONFIG_FREERTOS_USE_IDLE_HOOK */
    #define configUSE_IDLE_HOOK               0
#endif /* CONFIG_FREERTOS_USE_IDLE_HOOK */
#ಯದಿ CONFIG_FREERTOS_USE_TICK_HOOK
    #define configUSE_TICK_HOOK               1
#ಅನ್ಯಥಾ /* CONFIG_FREERTOS_USE_TICK_HOOK */
    #define configUSE_TICK_HOOK               0
#endif /* CONFIG_FREERTOS_USE_TICK_HOOK */
#ಯದಿ CONFIG_FREERTOS_CHECK_STACKOVERFLOW_NONE
    #define config_CHECK_FOR_ರಾಶಿ_OVERFLOW    0
#ನೋಚೇದ್ಯದಿ CONFIG_FREERTOS_CHECK_STACKOVERFLOW_PTRVAL
    #define config_CHECK_FOR_ರಾಶಿ_OVERFLOW    1
#ನೋಚೇದ್ಯದಿ CONFIG_FREERTOS_CHECK_STACKOVERFLOW_CANARY
    #define config_CHECK_FOR_ರಾಶಿ_OVERFLOW    2
#endif /* CONFIG_FREERTOS_CHECK_STACKOVERFLOW_NONE */
#define config_RECORD_ರಾಶಿ_ಮೇಲಿನ_ADDRESS       1             /* This must be set as the port requires TCB.pxEndOfStack */

/* ------------------- Run-time Stats ---------------------- */

#ifdef CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS
    #define configGENERATE_RUN_TIME_STATS           1       /* Used by vTaskGetRunTimeStats() */
#endif /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
#ifdef CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS
    #define configUSE_STATS_FORMATTING_FUNCTIONS    1       /* Used by vTaskList() */
#endif /* CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS */

#ಯದಿ CONFIG_FREERTOS_RUN_TIME_COUNTER_TYPE_U32
    #define configRUN_TIME_COUNTER_TYPE    uint32_t
#ನೋಚೇದ್ಯದಿ CONFIG_FREERTOS_RUN_TIME_COUNTER_TYPE_U64
    #define configRUN_TIME_COUNTER_TYPE    uint64_t
#endif /* CONFIG_FREERTOS_RUN_TIME_COUNTER_TYPE_U64 */

/* -------------------- Co-routines  ----------------------- */

#define configUSE_CO_ROUTINES              0              /* CO_ROUTINES are not supported in ESP-IDF */
#define configMAX_CO_ROUTINE_PRIORITIES    2

/* ------------------- Software Timer ---------------------- */

#ಯದಿ CONFIG_FREERTOS_USE_TIMERS
    #define configUSE_TIMERS                          1
    #define configTIMER_TASK_PRIORITY                 CONFIG_FREERTOS_TIMER_TASK_PRIORITY
    #define configTIMER_QUEUE_LENGTH                  CONFIG_FREERTOS_TIMER_QUEUE_LENGTH
    #define config_TIMER_TASK_ರಾಶಿ_DEPTH              CONFIG_FREERTOS_TIMER_TASK_ರಾಶಿ_DEPTH
    #define configTIMER_SERVICE_TASK_NAME             CONFIG_FREERTOS_TIMER_SERVICE_TASK_NAME
    #define configTIMER_SERVICE_TASK_CORE_AFFINITY    CONFIG_FREERTOS_TIMER_SERVICE_TASK_CORE_AFFINITY
#ಅನ್ಯಥಾ
    #define configUSE_TIMERS                          0
#endif


/* ------------------------ List --------------------------- */

#define configLIST_VOLATILE    ಚಞ್ಚಲ                     /* We define List elements as volatile to prevent the compiler from optimizing out essential code */

/* -------------------- API Includes ----------------------- */

#ಯದಿ CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY
    #define configENABLE_BACKWARD_COMPATIBILITY    1
#ಅನ್ಯಥಾ /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */
    #define configENABLE_BACKWARD_COMPATIBILITY    0
#endif /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */

#define INCLUDE_vTaskPrioritySet                   1
#define INCLUDE_uxTaskPriorityGet                  1
#define INCLUDE_vTaskDelete                        1
#define INCLUDE_vTaskSuspend                       1
#define INCLUDE_vTaskDelay                         1
#define INCLUDE_xTaskGetIdleTaskHandle             1
#define INCLUDE_xTaskAbortDelay                    1
#define INCLUDE_xSemaphoreGetMutexHolder           1
#define INCLUDE_xTaskGetHandle                     1
#define INCLUDE_ux_Task_Get_ರಾಶಿ_ಮೇಲಿನ_Water_Mark        1
#define INCLUDE_eTaskGetState                      1
#define INCLUDE_xTaskResumeFromISR                 1
#ಯದಿ CONFIG_FREERTOS_USE_TIMERS
  #define INCLUDE_xTimerPendFunctionCall           1
#ಅನ್ಯಥಾ
  #define INCLUDE_xTimerPendFunctionCall           0
#endif
#define INCLUDE_xTaskGetSchedulerState             1
#define INCLUDE_xTaskGetCurrentTaskHandle          1

/* -------------------- Trace Macros ----------------------- */

/*
 * For trace macros.
 * Note: Include trace macros here and not above as trace macros are dependent on some of the FreeRTOS configs
 */
#ifndef __ASSEMBLER__
    #ಯದಿ CONFIG_ESP_TRACE_ENABLE
        #ಅನ್ತರ್ಭಾವ "esp_trace_freertos.h"
    #endif

    #ಯದಿ CONFIG_FREERTOS_SMP

/* Default values for trace macros added to ESP-IDF implementation of SYSVIEW
 * that is not part of Amazon SMP FreeRTOS. */
        #ifndef traceISR_EXIT
            #define traceISR_EXIT()
        #endif
        #ifndef traceISR_ENTER
            #define traceISR_ENTER( _n_ )
        #endif

        #ifndef traceQUEUE_GIVE_FROM_ISR
            #define traceQUEUE_GIVE_FROM_ISR( pxQueue )
        #endif

        #ifndef traceQUEUE_GIVE_FROM_ISR_FAILED
            #define traceQUEUE_GIVE_FROM_ISR_FAILED( pxQueue )
        #endif

        #ifndef traceQUEUE_SEMAPHORE_RECEIVE
            #define traceQUEUE_SEMAPHORE_RECEIVE( pxQueue )
        #endif
    #endif /* CONFIG_FREERTOS_SMP */
#endif /* def __ASSEMBLER__ */

#ಯದಿ CONFIG_FREERTOS_USE_APPLICATION_TASK_TAG
    #define configUSE_APPLICATION_TASK_TAG    1
#endif // CONFIG_FREERTOS_USE_APPLICATION_TASK_TAG

/* -------------- List Data Integrity Checks --------------- */
#define configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES    CONFIG_FREERTOS_USE_LIST_DATA_INTEGRITY_CHECK_BYTES

/* ----------------------------------------------- Amazon SMP FreeRTOS -------------------------------------------------
 * - All Amazon SMP FreeRTOS specific configurations
 * ------------------------------------------------------------------------------------------------------------------ */

#ಯದಿ CONFIG_FREERTOS_SMP && ( CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES > 1 )
    #define configUSE_CORE_AFFINITY              1
    #define configRUN_MULTIPLE_PRIORITIES        1
    #define configUSE_TASK_PREEMPTION_DISABLE    1
#endif /* CONFIG_FREERTOS_SMP && ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */

/* -------------------------------------------------- IDF FreeRTOS -----------------------------------------------------
 * - All IDF FreeRTOS specific configurations
 * ------------------------------------------------------------------------------------------------------------------ */

#ಯದಿ !CONFIG_FREERTOS_SMP
    #ifdef CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID
        #define configTASKLIST_INCLUDE_COREID                  1
    #endif /* CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID */
    #ifdef CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
        #define configTHREAD_LOCAL_STORAGE_DELETE_CALLBACKS    1
    #endif /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
#endif /* !CONFIG_FREERTOS_SMP */

/* ------------------------------------------------ ESP-IDF Additions --------------------------------------------------
 * - Any other macros required by the rest of ESP-IDF
 * ------------------------------------------------------------------------------------------------------------------ */

/* portNUM_PROCESSORS is deprecated and will be removed in ESP-IDF v6.0 (IDF-8785)
 * Please use the Kconfig option CONFIG_FREERTOS_NUMBER_OF_CORES instead.
 */
#define port_ಅಂಕೆ_PROCESSORS    config_ಸಂಖ್ಯೆ_OF_CORES
