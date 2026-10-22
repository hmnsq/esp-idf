/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#هدايت once

#ضمن "sdkconfig.h"

/* This file gets pulled into assembly sources. Therefore, some includes need to
 * be wrapped in #ifndef __ASSEMBLER__ */

#جيڪڏهن_وصف_نه_ٿيل __ASSEMBLER__
    /* For configASSERT() */
    #ضمن <assert.h>
#جيڪڏهن_پورو /* def __ASSEMBLER__ */

/* ----------------------------------------------------- Helpers -------------------------------------------------------
 * - Macros that the FreeRTOS configuration macros depend on
 * ------------------------------------------------------------------------------------------------------------------ */

/* Higher stack checker modes cause overhead on each function call */
#إذا CONFIG_ڍير_CHECK_ALL || CONFIG_ڍير_CHECK_STRONG
    #وصف_ڪريو ڍير_OVERHEAD_CHECKER    256
#وإلا
    #وصف_ڪريو ڍير_OVERHEAD_CHECKER    0
#جيڪڏهن_پورو

/* with optimizations disabled, scheduler uses additional stack */
#إذا CONFIG_COMPILER_OPTIMIZATION_NONE
    #وصف_ڪريو ڍير_OVERHEAD_OPTIMIZATION    320
#وإلا
    #وصف_ڪريو ڍير_OVERHEAD_OPTIMIZATION    0
#جيڪڏهن_پورو

/* apptrace module increases minimum stack usage */
#إذا CONFIG_ESP_TRACE_TRANSPORT_APPTRACE
    #وصف_ڪريو ڍير_OVERHEAD_APPTRACE    1280
#وإلا
    #وصف_ڪريو ڍير_OVERHEAD_APPTRACE    0
#جيڪڏهن_پورو

/* Stack watchpoint decreases minimum usable stack size by up to 60 bytes.
 * See FreeRTOS FREERTOS_WATCHPOINT_END_OF_STACK option in Kconfig. */
#إذا CONFIG_FREERTOS_WATCHPOINT_پڇاڙي_جو_ڍير
    #وصف_ڪريو ڍير_OVERHEAD_WATCHPOINT    60
#وإلا
    #وصف_ڪريو ڍير_OVERHEAD_WATCHPOINT    0
#جيڪڏهن_پورو

#وصف_ڪريو config_ڍير_OVERHEAD_ڪل    \
    (                                 \
        ڍير_OVERHEAD_CHECKER +      \
        ڍير_OVERHEAD_OPTIMIZATION + \
        ڍير_OVERHEAD_APPTRACE +     \
        ڍير_OVERHEAD_WATCHPOINT     \
    )

/* The arch-specific FreeRTOSConfig_arch.h in esp_additions/arch_include/<arch>.
 * Placed here due to configSTACK_OVERHEAD_TOTAL. Todo: IDF-5712. */
#ضمن "freertos/FreeRTOSConfig_arch.h"

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

#وصف_ڪريو configUSE_PREEMPTION                         1
#وصف_ڪريو configUSE_TICKLESS_IDLE                      CONFIG_FREERTOS_USE_TICKLESS_IDLE
#إذا configUSE_TICKLESS_IDLE
    #وصف_ڪريو config_EXPECTED_IDLE_وقت_BEFORE_SLEEP    CONFIG_FREERTOS_IDLE_وقت_BEFORE_SLEEP
#جيڪڏهن_پورو /* configUSE_TICKLESS_IDLE */
#وصف_ڪريو configCPU_CLOCK_HZ                           ( CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ * 1000000 )
#وصف_ڪريو config_TICK_شرح_HZ                           CONFIG_FREERTOS_HZ
#وصف_ڪريو configMAX_PRIORITIES                         ( 25 )
#وصف_ڪريو config_USE_وقت_SLICING                       1
#وصف_ڪريو config_USE_16_ٻٽو_انگ_TICKS                       0
#وصف_ڪريو configIDLE_SHOULD_YIELD                      0
#وصف_ڪريو config_KERNEL_مداخلت_ترجيح              1      /*Todo: This currently isn't used anywhere */
#وصف_ڪريو config_عدد_جو_CORES                        CONFIG_FREERTOS_عدد_جو_CORES
/* For compatibility */
#وصف_ڪريو config_انگ_CORES                              config_عدد_جو_CORES

/* ------------- Synchronization Primitives ---------------- */

#وصف_ڪريو configUSE_MUTEXES                        1
#وصف_ڪريو configUSE_RECURSIVE_MUTEXES              1
#وصف_ڪريو configUSE_COUNTING_SEMAPHORES            1
#وصف_ڪريو config_USE_قطار_SETS                     1
#وصف_ڪريو config_قطار_REGISTRY_سائيز                CONFIG_FREERTOS_قطار_REGISTRY_سائيز
#وصف_ڪريو configUSE_TASK_NOTIFICATIONS             1
#وصف_ڪريو configTASK_NOTIFICATION_ARRAY_ENTRIES    CONFIG_FREERTOS_TASK_NOTIFICATION_ARRAY_ENTRIES

/* ----------------------- System -------------------------- */

#وصف_ڪريو configMAX_TASK_NAME_LEN    CONFIG_FREERTOS_MAX_TASK_NAME_LEN

/* If deletion callbacks are enabled, the number of TLSP's are doubled (i.e.,
 * the length of the TCB's pvThreadLocalStoragePointersThis array). This allows
 * the latter half of the array to store the deletion callback pointers (whereas
 * the first half stores the TLSPs themselves). */
#إذا CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
    #وصف_ڪريو config_انگ_THREAD_LOCAL_STORAGE_اشارا    ( CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_اشارا * 2 )
#وإلا /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
    #وصف_ڪريو config_انگ_THREAD_LOCAL_STORAGE_اشارا    CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_اشارا
#جيڪڏهن_پورو /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

#وصف_ڪريو config_ڍير_اونهائي_TYPE                         uint32_t
#إذا CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY
    #وصف_ڪريو configENABLE_BACKWARD_COMPATIBILITY        1
#وإلا /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */
    #وصف_ڪريو configENABLE_BACKWARD_COMPATIBILITY        0
#جيڪڏهن_پورو /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */
#وصف_ڪريو configASSERT( a )    أكد( a )

/* ----------------------- Memory  ------------------------- */

#وصف_ڪريو config_SUPPORT_STATIC_ورهاست              1
#وصف_ڪريو config_SUPPORT_DYNAMIC_ورهاست             1
#وصف_ڪريو config_APPLICATION_ALLOCATED_ڍير             1
#إذا CONFIG_FREERTOS_رکو_TASK_STACKS_اندريون_EXT_RAM
    #وصف_ڪريو config_ڍير_ورهاست_FROM_SEPARATE_ڍير    1
#وإلا
    #وصف_ڪريو config_ڍير_ورهاست_FROM_SEPARATE_ڍير    0
#جيڪڏهن_پورو

/* ------------------------ Hooks -------------------------- */

#إذا CONFIG_FREERTOS_USE_IDLE_HOOK
    #وصف_ڪريو configUSE_IDLE_HOOK               1
#وإلا /* CONFIG_FREERTOS_USE_IDLE_HOOK */
    #وصف_ڪريو configUSE_IDLE_HOOK               0
#جيڪڏهن_پورو /* CONFIG_FREERTOS_USE_IDLE_HOOK */
#إذا CONFIG_FREERTOS_USE_TICK_HOOK
    #وصف_ڪريو configUSE_TICK_HOOK               1
#وإلا /* CONFIG_FREERTOS_USE_TICK_HOOK */
    #وصف_ڪريو configUSE_TICK_HOOK               0
#جيڪڏهن_پورو /* CONFIG_FREERTOS_USE_TICK_HOOK */
#إذا CONFIG_FREERTOS_CHECK_STACKOVERFLOW_NONE
    #وصف_ڪريو config_CHECK_FOR_ڍير_حد_کان_مٿي    0
#وإلاإذا CONFIG_FREERTOS_CHECK_STACKOVERFLOW_PTRVAL
    #وصف_ڪريو config_CHECK_FOR_ڍير_حد_کان_مٿي    1
#وإلاإذا CONFIG_FREERTOS_CHECK_STACKOVERFLOW_CANARY
    #وصف_ڪريو config_CHECK_FOR_ڍير_حد_کان_مٿي    2
#جيڪڏهن_پورو /* CONFIG_FREERTOS_CHECK_STACKOVERFLOW_NONE */
#وصف_ڪريو config_رڪارڊ_ڍير_مٿاهون_پتو       1             /* This must be set as the port requires TCB.pxEndOfStack */

/* ------------------- Run-time Stats ---------------------- */

#جيڪڏهن_وصف_ٿيل CONFIG_FREERTOS_GENERATE_لاڳيتو_وقت_STATS
    #وصف_ڪريو config_GENERATE_لاڳيتو_وقت_STATS           1       /* Used by vTaskGetRunTimeStats() */
#جيڪڏهن_پورو /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
#جيڪڏهن_وصف_ٿيل CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS
    #وصف_ڪريو configUSE_STATS_FORMATTING_FUNCTIONS    1       /* Used by vTaskList() */
#جيڪڏهن_پورو /* CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS */

#إذا CONFIG_FREERTOS_لاڳيتو_وقت_COUNTER_TYPE_U32
    #وصف_ڪريو config_لاڳيتو_وقت_COUNTER_TYPE    uint32_t
#وإلاإذا CONFIG_FREERTOS_لاڳيتو_وقت_COUNTER_TYPE_U64
    #وصف_ڪريو config_لاڳيتو_وقت_COUNTER_TYPE    uint64_t
#جيڪڏهن_پورو /* CONFIG_FREERTOS_RUN_TIME_COUNTER_TYPE_U64 */

/* -------------------- Co-routines  ----------------------- */

#وصف_ڪريو configUSE_CO_ROUTINES              0              /* CO_ROUTINES are not supported in ESP-IDF */
#وصف_ڪريو configMAX_CO_ROUTINE_PRIORITIES    2

/* ------------------- Software Timer ---------------------- */

#إذا CONFIG_FREERTOS_USE_TIMERS
    #وصف_ڪريو configUSE_TIMERS                          1
    #وصف_ڪريو config_TIMER_TASK_ترجيح                 CONFIG_FREERTOS_TIMER_TASK_ترجيح
    #وصف_ڪريو config_TIMER_قطار_ڊيگهائي                  CONFIG_FREERTOS_TIMER_قطار_ڊيگهائي
    #وصف_ڪريو config_TIMER_TASK_ڍير_اونهائي              CONFIG_FREERTOS_TIMER_TASK_ڍير_اونهائي
    #وصف_ڪريو configTIMER_SERVICE_TASK_NAME             CONFIG_FREERTOS_TIMER_SERVICE_TASK_NAME
    #وصف_ڪريو configTIMER_SERVICE_TASK_CORE_AFFINITY    CONFIG_FREERTOS_TIMER_SERVICE_TASK_CORE_AFFINITY
#وإلا
    #وصف_ڪريو configUSE_TIMERS                          0
#جيڪڏهن_پورو


/* ------------------------ List --------------------------- */

#وصف_ڪريو configLIST_VOLATILE    متطاير                     /* We define List elements as volatile to prevent the compiler from optimizing out essential code */

/* -------------------- API Includes ----------------------- */

#إذا CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY
    #وصف_ڪريو configENABLE_BACKWARD_COMPATIBILITY    1
#وإلا /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */
    #وصف_ڪريو configENABLE_BACKWARD_COMPATIBILITY    0
#جيڪڏهن_پورو /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */

#وصف_ڪريو INCLUDE_v_Task_ترجيح_سيٽ                   1
#وصف_ڪريو INCLUDE_ux_Task_ترجيح_Get                  1
#وصف_ڪريو INCLUDE_vTaskDelete                        1
#وصف_ڪريو INCLUDE_vTaskSuspend                       1
#وصف_ڪريو INCLUDE_vTaskDelay                         1
#وصف_ڪريو INCLUDE_xTaskGetIdleTaskHandle             1
#وصف_ڪريو INCLUDE_xTaskAbortDelay                    1
#وصف_ڪريو INCLUDE_xSemaphoreGetMutexHolder           1
#وصف_ڪريو INCLUDE_xTaskGetHandle                     1
#وصف_ڪريو INCLUDE_ux_Task_Get_ڍير_مٿاهون_Water_Mark        1
#وصف_ڪريو INCLUDE_eTaskGetState                      1
#وصف_ڪريو INCLUDE_xTaskResumeFromISR                 1
#إذا CONFIG_FREERTOS_USE_TIMERS
  #وصف_ڪريو INCLUDE_xTimerPendFunctionCall           1
#وإلا
  #وصف_ڪريو INCLUDE_xTimerPendFunctionCall           0
#جيڪڏهن_پورو
#وصف_ڪريو INCLUDE_xTaskGetSchedulerState             1
#وصف_ڪريو INCLUDE_xTaskGetCurrentTaskHandle          1

/* -------------------- Trace Macros ----------------------- */

/*
 * For trace macros.
 * Note: Include trace macros here and not above as trace macros are dependent on some of the FreeRTOS configs
 */
#جيڪڏهن_وصف_نه_ٿيل __ASSEMBLER__
    #إذا CONFIG_ESP_TRACE_ENABLE
        #ضمن "esp_trace_freertos.h"
    #جيڪڏهن_پورو

    #إذا CONFIG_FREERTOS_SMP

/* Default values for trace macros added to ESP-IDF implementation of SYSVIEW
 * that is not part of Amazon SMP FreeRTOS. */
        #جيڪڏهن_وصف_نه_ٿيل traceISR_EXIT
            #وصف_ڪريو traceISR_EXIT()
        #جيڪڏهن_پورو
        #جيڪڏهن_وصف_نه_ٿيل traceISR_ENTER
            #وصف_ڪريو traceISR_ENTER( _n_ )
        #جيڪڏهن_پورو

        #جيڪڏهن_وصف_نه_ٿيل trace_قطار_GIVE_FROM_ISR
            #وصف_ڪريو trace_قطار_GIVE_FROM_ISR( px_قطار )
        #جيڪڏهن_پورو

        #جيڪڏهن_وصف_نه_ٿيل trace_قطار_GIVE_FROM_ISR_FAILED
            #وصف_ڪريو trace_قطار_GIVE_FROM_ISR_FAILED( px_قطار )
        #جيڪڏهن_پورو

        #جيڪڏهن_وصف_نه_ٿيل trace_قطار_SEMAPHORE_RECEIVE
            #وصف_ڪريو trace_قطار_SEMAPHORE_RECEIVE( px_قطار )
        #جيڪڏهن_پورو
    #جيڪڏهن_پورو /* CONFIG_FREERTOS_SMP */
#جيڪڏهن_پورو /* def __ASSEMBLER__ */

#إذا CONFIG_FREERTOS_USE_APPLICATION_TASK_ٽيگ
    #وصف_ڪريو config_USE_APPLICATION_TASK_ٽيگ    1
#جيڪڏهن_پورو // CONFIG_FREERTOS_USE_APPLICATION_TASK_TAG

/* -------------- List Data Integrity Checks --------------- */
#وصف_ڪريو config_USE_LIST_معلومات_INTEGRITY_CHECK_بائيٽون    CONFIG_FREERTOS_USE_LIST_معلومات_INTEGRITY_CHECK_بائيٽون

/* ----------------------------------------------- Amazon SMP FreeRTOS -------------------------------------------------
 * - All Amazon SMP FreeRTOS specific configurations
 * ------------------------------------------------------------------------------------------------------------------ */

#إذا CONFIG_FREERTOS_SMP && ( CONFIG_FREERTOS_عدد_جو_CORES > 1 )
    #وصف_ڪريو configUSE_CORE_AFFINITY              1
    #وصف_ڪريو config_لاڳيتو_گڻو_PRIORITIES        1
    #وصف_ڪريو configUSE_TASK_PREEMPTION_DISABLE    1
#جيڪڏهن_پورو /* CONFIG_FREERTOS_SMP && ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */

/* -------------------------------------------------- IDF FreeRTOS -----------------------------------------------------
 * - All IDF FreeRTOS specific configurations
 * ------------------------------------------------------------------------------------------------------------------ */

#إذا !CONFIG_FREERTOS_SMP
    #جيڪڏهن_وصف_ٿيل CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID
        #وصف_ڪريو configTASKLIST_INCLUDE_COREID                  1
    #جيڪڏهن_پورو /* CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID */
    #جيڪڏهن_وصف_ٿيل CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
        #وصف_ڪريو configTHREAD_LOCAL_STORAGE_DELETE_CALLBACKS    1
    #جيڪڏهن_پورو /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
#جيڪڏهن_پورو /* !CONFIG_FREERTOS_SMP */

/* ------------------------------------------------ ESP-IDF Additions --------------------------------------------------
 * - Any other macros required by the rest of ESP-IDF
 * ------------------------------------------------------------------------------------------------------------------ */

/* portNUM_PROCESSORS is deprecated and will be removed in ESP-IDF v6.0 (IDF-8785)
 * Please use the Kconfig option CONFIG_FREERTOS_NUMBER_OF_CORES instead.
 */
#وصف_ڪريو پورٽ_انگ_PROCESSORS    config_عدد_جو_CORES
