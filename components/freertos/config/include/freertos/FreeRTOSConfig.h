/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"

/* This file gets pulled into assembly sources. Therefore, some includes need to
 * be wrapped in #ifndef __ASSEMBLER__ */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __ASSEMBLER__
    /* For configASSERT() */
    #ಅನ್ತರ್ಭಾವ <assert.h>
#ಆದರೆ_ಅಂತ್ಯ /* def __ASSEMBLER__ */

/* ----------------------------------------------------- Helpers -------------------------------------------------------
 * - Macros that the FreeRTOS configuration macros depend on
 * ------------------------------------------------------------------------------------------------------------------ */

/* Higher stack checker modes cause overhead on each function call */
#ಆದರೆ CONFIG_ರಾಶಿ_CHECK_ALL || CONFIG_ರಾಶಿ_CHECK_STRONG
    #ವ್ಯಾಖ್ಯಾನಿಸು ರಾಶಿ_OVERHEAD_CHECKER    256
#ಇಲ್ಲದಿದ್ದರೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು ರಾಶಿ_OVERHEAD_CHECKER    0
#ಆದರೆ_ಅಂತ್ಯ

/* with optimizations disabled, scheduler uses additional stack */
#ಆದರೆ CONFIG_COMPILER_OPTIMIZATION_NONE
    #ವ್ಯಾಖ್ಯಾನಿಸು ರಾಶಿ_OVERHEAD_OPTIMIZATION    320
#ಇಲ್ಲದಿದ್ದರೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು ರಾಶಿ_OVERHEAD_OPTIMIZATION    0
#ಆದರೆ_ಅಂತ್ಯ

/* apptrace module increases minimum stack usage */
#ಆದರೆ CONFIG_ESP_TRACE_TRANSPORT_APPTRACE
    #ವ್ಯಾಖ್ಯಾನಿಸು ರಾಶಿ_OVERHEAD_APPTRACE    1280
#ಇಲ್ಲದಿದ್ದರೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು ರಾಶಿ_OVERHEAD_APPTRACE    0
#ಆದರೆ_ಅಂತ್ಯ

/* Stack watchpoint decreases minimum usable stack size by up to 60 bytes.
 * See FreeRTOS FREERTOS_WATCHPOINT_END_OF_STACK option in Kconfig. */
#ಆದರೆ CONFIG_FREERTOS_WATCHPOINT_ಕೊನೆ_ದ_ರಾಶಿ
    #ವ್ಯಾಖ್ಯಾನಿಸು ರಾಶಿ_OVERHEAD_WATCHPOINT    60
#ಇಲ್ಲದಿದ್ದರೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು ರಾಶಿ_OVERHEAD_WATCHPOINT    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು config_ರಾಶಿ_OVERHEAD_ಒಟ್ಟು    \
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

#ವ್ಯಾಖ್ಯಾನಿಸು configUSE_PREEMPTION                         1
#ವ್ಯಾಖ್ಯಾನಿಸು configUSE_TICKLESS_IDLE                      CONFIG_FREERTOS_USE_TICKLESS_IDLE
#ಆದರೆ configUSE_TICKLESS_IDLE
    #ವ್ಯಾಖ್ಯಾನಿಸು config_EXPECTED_IDLE_ಸಮಯ_BEFORE_SLEEP    CONFIG_FREERTOS_IDLE_ಸಮಯ_BEFORE_SLEEP
#ಆದರೆ_ಅಂತ್ಯ /* configUSE_TICKLESS_IDLE */
#ವ್ಯಾಖ್ಯಾನಿಸು configCPU_CLOCK_HZ                           ( CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ * 1000000 )
#ವ್ಯಾಖ್ಯಾನಿಸು config_TICK_ದರ_HZ                           CONFIG_FREERTOS_HZ
#ವ್ಯಾಖ್ಯಾನಿಸು configMAX_PRIORITIES                         ( 25 )
#ವ್ಯಾಖ್ಯಾನಿಸು config_USE_ಸಮಯ_SLICING                       1
#ವ್ಯಾಖ್ಯಾನಿಸು config_USE_16_ಬಿಟ್_TICKS                       0
#ವ್ಯಾಖ್ಯಾನಿಸು configIDLE_SHOULD_YIELD                      0
#ವ್ಯಾಖ್ಯಾನಿಸು config_KERNEL_ಅಡಚಣೆ_ಆದ್ಯತೆ              1      /*Todo: This currently isn't used anywhere */
#ವ್ಯಾಖ್ಯಾನಿಸು config_ಸಂಖ್ಯೆ_ದ_CORES                        CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES
/* For compatibility */
#ವ್ಯಾಖ್ಯಾನಿಸು config_ಅಂಕೆ_CORES                              config_ಸಂಖ್ಯೆ_ದ_CORES

/* ------------- Synchronization Primitives ---------------- */

#ವ್ಯಾಖ್ಯಾನಿಸು configUSE_MUTEXES                        1
#ವ್ಯಾಖ್ಯಾನಿಸು configUSE_RECURSIVE_MUTEXES              1
#ವ್ಯಾಖ್ಯಾನಿಸು configUSE_COUNTING_SEMAPHORES            1
#ವ್ಯಾಖ್ಯಾನಿಸು config_USE_ಸರತಿ_SETS                     1
#ವ್ಯಾಖ್ಯಾನಿಸು config_ಸರತಿ_REGISTRY_ಗಾತ್ರ                CONFIG_FREERTOS_ಸರತಿ_REGISTRY_ಗಾತ್ರ
#ವ್ಯಾಖ್ಯಾನಿಸು configUSE_TASK_NOTIFICATIONS             1
#ವ್ಯಾಖ್ಯಾನಿಸು configTASK_NOTIFICATION_ARRAY_ENTRIES    CONFIG_FREERTOS_TASK_NOTIFICATION_ARRAY_ENTRIES

/* ----------------------- System -------------------------- */

#ವ್ಯಾಖ್ಯಾನಿಸು configMAX_TASK_NAME_LEN    CONFIG_FREERTOS_MAX_TASK_NAME_LEN

/* If deletion callbacks are enabled, the number of TLSP's are doubled (i.e.,
 * the length of the TCB's pvThreadLocalStoragePointersThis array). This allows
 * the latter half of the array to store the deletion callback pointers (whereas
 * the first half stores the TLSPs themselves). */
#ಆದರೆ CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
    #ವ್ಯಾಖ್ಯಾನಿಸು config_ಅಂಕೆ_THREAD_LOCAL_STORAGE_ಸೂಚಕಗಳು    ( CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_ಸೂಚಕಗಳು * 2 )
#ಇಲ್ಲದಿದ್ದರೆ /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
    #ವ್ಯಾಖ್ಯಾನಿಸು config_ಅಂಕೆ_THREAD_LOCAL_STORAGE_ಸೂಚಕಗಳು    CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_ಸೂಚಕಗಳು
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

#ವ್ಯಾಖ್ಯಾನಿಸು config_ರಾಶಿ_ಆಳ_TYPE                         uint32_t
#ಆದರೆ CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY
    #ವ್ಯಾಖ್ಯಾನಿಸು configENABLE_BACKWARD_COMPATIBILITY        1
#ಇಲ್ಲದಿದ್ದರೆ /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */
    #ವ್ಯಾಖ್ಯಾನಿಸು configENABLE_BACKWARD_COMPATIBILITY        0
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */
#ವ್ಯಾಖ್ಯಾನಿಸು configASSERT( a )    ಪ್ರತಿಜ್ಞಾ( a )

/* ----------------------- Memory  ------------------------- */

#ವ್ಯಾಖ್ಯಾನಿಸು config_SUPPORT_STATIC_ಹಂಚಿಕೆ              1
#ವ್ಯಾಖ್ಯಾನಿಸು config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ             1
#ವ್ಯಾಖ್ಯಾನಿಸು config_APPLICATION_ALLOCATED_ರಾಶಿ             1
#ಆದರೆ CONFIG_FREERTOS_ಇರಿಸು_TASK_STACKS_ಒಳ_EXT_RAM
    #ವ್ಯಾಖ್ಯಾನಿಸು config_ರಾಶಿ_ಹಂಚಿಕೆ_FROM_SEPARATE_ರಾಶಿ    1
#ಇಲ್ಲದಿದ್ದರೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು config_ರಾಶಿ_ಹಂಚಿಕೆ_FROM_SEPARATE_ರಾಶಿ    0
#ಆದರೆ_ಅಂತ್ಯ

/* ------------------------ Hooks -------------------------- */

#ಆದರೆ CONFIG_FREERTOS_USE_IDLE_HOOK
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_IDLE_HOOK               1
#ಇಲ್ಲದಿದ್ದರೆ /* CONFIG_FREERTOS_USE_IDLE_HOOK */
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_IDLE_HOOK               0
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_USE_IDLE_HOOK */
#ಆದರೆ CONFIG_FREERTOS_USE_TICK_HOOK
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_TICK_HOOK               1
#ಇಲ್ಲದಿದ್ದರೆ /* CONFIG_FREERTOS_USE_TICK_HOOK */
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_TICK_HOOK               0
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_USE_TICK_HOOK */
#ಆದರೆ CONFIG_FREERTOS_CHECK_STACKOVERFLOW_NONE
    #ವ್ಯಾಖ್ಯಾನಿಸು config_CHECK_FOR_ರಾಶಿ_ಅತಿಪ್ರವಾಹ    0
#ಅಥವಾಆದರೆ CONFIG_FREERTOS_CHECK_STACKOVERFLOW_PTRVAL
    #ವ್ಯಾಖ್ಯಾನಿಸು config_CHECK_FOR_ರಾಶಿ_ಅತಿಪ್ರವಾಹ    1
#ಅಥವಾಆದರೆ CONFIG_FREERTOS_CHECK_STACKOVERFLOW_CANARY
    #ವ್ಯಾಖ್ಯಾನಿಸು config_CHECK_FOR_ರಾಶಿ_ಅತಿಪ್ರವಾಹ    2
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_CHECK_STACKOVERFLOW_NONE */
#ವ್ಯಾಖ್ಯಾನಿಸು config_ದಾಖಲೆ_ರಾಶಿ_ಮೇಲಿನ_ವಿಳಾಸ       1             /* This must be set as the port requires TCB.pxEndOfStack */

/* ------------------- Run-time Stats ---------------------- */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FREERTOS_GENERATE_ಸತತ_ಸಮಯ_STATS
    #ವ್ಯಾಖ್ಯಾನಿಸು config_GENERATE_ಸತತ_ಸಮಯ_STATS           1       /* Used by vTaskGetRunTimeStats() */
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_STATS_FORMATTING_FUNCTIONS    1       /* Used by vTaskList() */
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS */

#ಆದರೆ CONFIG_FREERTOS_ಸತತ_ಸಮಯ_COUNTER_TYPE_U32
    #ವ್ಯಾಖ್ಯಾನಿಸು config_ಸತತ_ಸಮಯ_COUNTER_TYPE    uint32_t
#ಅಥವಾಆದರೆ CONFIG_FREERTOS_ಸತತ_ಸಮಯ_COUNTER_TYPE_U64
    #ವ್ಯಾಖ್ಯಾನಿಸು config_ಸತತ_ಸಮಯ_COUNTER_TYPE    uint64_t
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_RUN_TIME_COUNTER_TYPE_U64 */

/* -------------------- Co-routines  ----------------------- */

#ವ್ಯಾಖ್ಯಾನಿಸು configUSE_CO_ROUTINES              0              /* CO_ROUTINES are not supported in ESP-IDF */
#ವ್ಯಾಖ್ಯಾನಿಸು configMAX_CO_ROUTINE_PRIORITIES    2

/* ------------------- Software Timer ---------------------- */

#ಆದರೆ CONFIG_FREERTOS_USE_TIMERS
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_TIMERS                          1
    #ವ್ಯಾಖ್ಯಾನಿಸು config_TIMER_TASK_ಆದ್ಯತೆ                 CONFIG_FREERTOS_TIMER_TASK_ಆದ್ಯತೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು config_TIMER_ಸರತಿ_ಉದ್ದಳತೆ                  CONFIG_FREERTOS_TIMER_ಸರತಿ_ಉದ್ದಳತೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು config_TIMER_TASK_ರಾಶಿ_ಆಳ              CONFIG_FREERTOS_TIMER_TASK_ರಾಶಿ_ಆಳ
    #ವ್ಯಾಖ್ಯಾನಿಸು configTIMER_SERVICE_TASK_NAME             CONFIG_FREERTOS_TIMER_SERVICE_TASK_NAME
    #ವ್ಯಾಖ್ಯಾನಿಸು configTIMER_SERVICE_TASK_CORE_AFFINITY    CONFIG_FREERTOS_TIMER_SERVICE_TASK_CORE_AFFINITY
#ಇಲ್ಲದಿದ್ದರೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_TIMERS                          0
#ಆದರೆ_ಅಂತ್ಯ


/* ------------------------ List --------------------------- */

#ವ್ಯಾಖ್ಯಾನಿಸು configLIST_VOLATILE    ಅಸ್ಥಿರ                     /* We define List elements as volatile to prevent the compiler from optimizing out essential code */

/* -------------------- API Includes ----------------------- */

#ಆದರೆ CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY
    #ವ್ಯಾಖ್ಯಾನಿಸು configENABLE_BACKWARD_COMPATIBILITY    1
#ಇಲ್ಲದಿದ್ದರೆ /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */
    #ವ್ಯಾಖ್ಯಾನಿಸು configENABLE_BACKWARD_COMPATIBILITY    0
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */

#ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_v_Task_ಆದ್ಯತೆ_ಗಣ                   1
#ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_ux_Task_ಆದ್ಯತೆ_Get                  1
#ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_vTaskDelete                        1
#ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_vTaskSuspend                       1
#ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_vTaskDelay                         1
#ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_xTaskGetIdleTaskHandle             1
#ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_xTaskAbortDelay                    1
#ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_xSemaphoreGetMutexHolder           1
#ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_xTaskGetHandle                     1
#ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_ux_Task_Get_ರಾಶಿ_ಮೇಲಿನ_Water_Mark        1
#ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_eTaskGetState                      1
#ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_xTaskResumeFromISR                 1
#ಆದರೆ CONFIG_FREERTOS_USE_TIMERS
  #ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_xTimerPendFunctionCall           1
#ಇಲ್ಲದಿದ್ದರೆ
  #ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_xTimerPendFunctionCall           0
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_xTaskGetSchedulerState             1
#ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_xTaskGetCurrentTaskHandle          1

/* -------------------- Trace Macros ----------------------- */

/*
 * For trace macros.
 * Note: Include trace macros here and not above as trace macros are dependent on some of the FreeRTOS configs
 */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __ASSEMBLER__
    #ಆದರೆ CONFIG_ESP_TRACE_ENABLE
        #ಅನ್ತರ್ಭಾವ "esp_trace_freertos.h"
    #ಆದರೆ_ಅಂತ್ಯ

    #ಆದರೆ CONFIG_FREERTOS_SMP

/* Default values for trace macros added to ESP-IDF implementation of SYSVIEW
 * that is not part of Amazon SMP FreeRTOS. */
        #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceISR_EXIT
            #ವ್ಯಾಖ್ಯಾನಿಸು traceISR_EXIT()
        #ಆದರೆ_ಅಂತ್ಯ
        #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceISR_ENTER
            #ವ್ಯಾಖ್ಯಾನಿಸು traceISR_ENTER( _n_ )
        #ಆದರೆ_ಅಂತ್ಯ

        #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಸರತಿ_GIVE_FROM_ISR
            #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಸರತಿ_GIVE_FROM_ISR( px_ಸರತಿ )
        #ಆದರೆ_ಅಂತ್ಯ

        #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಸರತಿ_GIVE_FROM_ISR_FAILED
            #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಸರತಿ_GIVE_FROM_ISR_FAILED( px_ಸರತಿ )
        #ಆದರೆ_ಅಂತ್ಯ

        #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಸರತಿ_SEMAPHORE_RECEIVE
            #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಸರತಿ_SEMAPHORE_RECEIVE( px_ಸರತಿ )
        #ಆದರೆ_ಅಂತ್ಯ
    #ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_SMP */
#ಆದರೆ_ಅಂತ್ಯ /* def __ASSEMBLER__ */

#ಆದರೆ CONFIG_FREERTOS_USE_APPLICATION_TASK_ಟ್ಯಾಗ್
    #ವ್ಯಾಖ್ಯಾನಿಸು config_USE_APPLICATION_TASK_ಟ್ಯಾಗ್    1
#ಆದರೆ_ಅಂತ್ಯ // CONFIG_FREERTOS_USE_APPLICATION_TASK_TAG

/* -------------- List Data Integrity Checks --------------- */
#ವ್ಯಾಖ್ಯಾನಿಸು config_USE_LIST_ದತ್ತಾಂಶ_INTEGRITY_CHECK_ಬೈಟುಗಳು    CONFIG_FREERTOS_USE_LIST_ದತ್ತಾಂಶ_INTEGRITY_CHECK_ಬೈಟುಗಳು

/* ----------------------------------------------- Amazon SMP FreeRTOS -------------------------------------------------
 * - All Amazon SMP FreeRTOS specific configurations
 * ------------------------------------------------------------------------------------------------------------------ */

#ಆದರೆ CONFIG_FREERTOS_SMP && ( CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES > 1 )
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_CORE_AFFINITY              1
    #ವ್ಯಾಖ್ಯಾನಿಸು config_ಸತತ_ಅಪವರ್ತ್ಯ_PRIORITIES        1
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_TASK_PREEMPTION_DISABLE    1
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_SMP && ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */

/* -------------------------------------------------- IDF FreeRTOS -----------------------------------------------------
 * - All IDF FreeRTOS specific configurations
 * ------------------------------------------------------------------------------------------------------------------ */

#ಆದರೆ !CONFIG_FREERTOS_SMP
    #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID
        #ವ್ಯಾಖ್ಯಾನಿಸು configTASKLIST_INCLUDE_COREID                  1
    #ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID */
    #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
        #ವ್ಯಾಖ್ಯಾನಿಸು configTHREAD_LOCAL_STORAGE_DELETE_CALLBACKS    1
    #ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_FREERTOS_SMP */

/* ------------------------------------------------ ESP-IDF Additions --------------------------------------------------
 * - Any other macros required by the rest of ESP-IDF
 * ------------------------------------------------------------------------------------------------------------------ */

/* portNUM_PROCESSORS is deprecated and will be removed in ESP-IDF v6.0 (IDF-8785)
 * Please use the Kconfig option CONFIG_FREERTOS_NUMBER_OF_CORES instead.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ಅಂಕೆ_PROCESSORS    config_ಸಂಖ್ಯೆ_ದ_CORES
