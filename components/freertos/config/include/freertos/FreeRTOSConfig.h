/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#لارښوونه once

#ضمن "sdkconfig.h"

/* This file gets pulled into assembly sources. Therefore, some includes need to
 * be wrapped in #ifndef __ASSEMBLER__ */

#که_نه_تعریف_شوی __ASSEMBLER__
    /* For configASSERT() */
    #ضمن <assert.h>
#د_که_پای /* def __ASSEMBLER__ */

/* ----------------------------------------------------- Helpers -------------------------------------------------------
 * - Macros that the FreeRTOS configuration macros depend on
 * ------------------------------------------------------------------------------------------------------------------ */

/* Higher stack checker modes cause overhead on each function call */
#إذا CONFIG_ډېری_CHECK_ALL || CONFIG_ډېری_CHECK_STRONG
    #تعریف_کړه ډېری_OVERHEAD_CHECKER    256
#وإلا
    #تعریف_کړه ډېری_OVERHEAD_CHECKER    0
#د_که_پای

/* with optimizations disabled, scheduler uses additional stack */
#إذا CONFIG_COMPILER_OPTIMIZATION_NONE
    #تعریف_کړه ډېری_OVERHEAD_OPTIMIZATION    320
#وإلا
    #تعریف_کړه ډېری_OVERHEAD_OPTIMIZATION    0
#د_که_پای

/* apptrace module increases minimum stack usage */
#إذا CONFIG_ESP_TRACE_TRANSPORT_APPTRACE
    #تعریف_کړه ډېری_OVERHEAD_APPTRACE    1280
#وإلا
    #تعریف_کړه ډېری_OVERHEAD_APPTRACE    0
#د_که_پای

/* Stack watchpoint decreases minimum usable stack size by up to 60 bytes.
 * See FreeRTOS FREERTOS_WATCHPOINT_END_OF_STACK option in Kconfig. */
#إذا CONFIG_FREERTOS_WATCHPOINT_پای_ټکی_د_ډېری
    #تعریف_کړه ډېری_OVERHEAD_WATCHPOINT    60
#وإلا
    #تعریف_کړه ډېری_OVERHEAD_WATCHPOINT    0
#د_که_پای

#تعریف_کړه config_ډېری_OVERHEAD_ټول    \
    (                                 \
        ډېری_OVERHEAD_CHECKER +      \
        ډېری_OVERHEAD_OPTIMIZATION + \
        ډېری_OVERHEAD_APPTRACE +     \
        ډېری_OVERHEAD_WATCHPOINT     \
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

#تعریف_کړه configUSE_PREEMPTION                         1
#تعریف_کړه configUSE_TICKLESS_IDLE                      CONFIG_FREERTOS_USE_TICKLESS_IDLE
#إذا configUSE_TICKLESS_IDLE
    #تعریف_کړه config_EXPECTED_IDLE_وخت_BEFORE_SLEEP    CONFIG_FREERTOS_IDLE_وخت_BEFORE_SLEEP
#د_که_پای /* configUSE_TICKLESS_IDLE */
#تعریف_کړه configCPU_CLOCK_HZ                           ( CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ * 1000000 )
#تعریف_کړه config_TICK_شرح_HZ                           CONFIG_FREERTOS_HZ
#تعریف_کړه configMAX_PRIORITIES                         ( 25 )
#تعریف_کړه config_USE_وخت_SLICING                       1
#تعریف_کړه config_USE_16_بټ_TICKS                       0
#تعریف_کړه configIDLE_SHOULD_YIELD                      0
#تعریف_کړه config_KERNEL_مداخله_لومړیتوب              1      /*Todo: This currently isn't used anywhere */
#تعریف_کړه config_شمېره_د_CORES                        CONFIG_FREERTOS_شمېره_د_CORES
/* For compatibility */
#تعریف_کړه config_عدد_CORES                              config_شمېره_د_CORES

/* ------------- Synchronization Primitives ---------------- */

#تعریف_کړه configUSE_MUTEXES                        1
#تعریف_کړه configUSE_RECURSIVE_MUTEXES              1
#تعریف_کړه configUSE_COUNTING_SEMAPHORES            1
#تعریف_کړه config_USE_کتار_SETS                     1
#تعریف_کړه config_کتار_REGISTRY_اندازه                CONFIG_FREERTOS_کتار_REGISTRY_اندازه
#تعریف_کړه configUSE_TASK_NOTIFICATIONS             1
#تعریف_کړه configTASK_NOTIFICATION_ARRAY_ENTRIES    CONFIG_FREERTOS_TASK_NOTIFICATION_ARRAY_ENTRIES

/* ----------------------- System -------------------------- */

#تعریف_کړه configMAX_TASK_NAME_LEN    CONFIG_FREERTOS_MAX_TASK_NAME_LEN

/* If deletion callbacks are enabled, the number of TLSP's are doubled (i.e.,
 * the length of the TCB's pvThreadLocalStoragePointersThis array). This allows
 * the latter half of the array to store the deletion callback pointers (whereas
 * the first half stores the TLSPs themselves). */
#إذا CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
    #تعریف_کړه config_عدد_THREAD_LOCAL_STORAGE_اشارې    ( CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_اشارې * 2 )
#وإلا /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
    #تعریف_کړه config_عدد_THREAD_LOCAL_STORAGE_اشارې    CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_اشارې
#د_که_پای /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

#تعریف_کړه config_ډېری_ژوروالی_TYPE                         uint32_t
#إذا CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY
    #تعریف_کړه configENABLE_BACKWARD_COMPATIBILITY        1
#وإلا /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */
    #تعریف_کړه configENABLE_BACKWARD_COMPATIBILITY        0
#د_که_پای /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */
#تعریف_کړه configASSERT( a )    أكد( a )

/* ----------------------- Memory  ------------------------- */

#تعریف_کړه config_SUPPORT_STATIC_ویش              1
#تعریف_کړه config_SUPPORT_DYNAMIC_ویش             1
#تعریف_کړه config_APPLICATION_ALLOCATED_ډېرۍ             1
#إذا CONFIG_FREERTOS_ځای_پرځای_کړه_TASK_STACKS_دننه_EXT_RAM
    #تعریف_کړه config_ډېری_ویش_FROM_SEPARATE_ډېرۍ    1
#وإلا
    #تعریف_کړه config_ډېری_ویش_FROM_SEPARATE_ډېرۍ    0
#د_که_پای

/* ------------------------ Hooks -------------------------- */

#إذا CONFIG_FREERTOS_USE_IDLE_HOOK
    #تعریف_کړه configUSE_IDLE_HOOK               1
#وإلا /* CONFIG_FREERTOS_USE_IDLE_HOOK */
    #تعریف_کړه configUSE_IDLE_HOOK               0
#د_که_پای /* CONFIG_FREERTOS_USE_IDLE_HOOK */
#إذا CONFIG_FREERTOS_USE_TICK_HOOK
    #تعریف_کړه configUSE_TICK_HOOK               1
#وإلا /* CONFIG_FREERTOS_USE_TICK_HOOK */
    #تعریف_کړه configUSE_TICK_HOOK               0
#د_که_پای /* CONFIG_FREERTOS_USE_TICK_HOOK */
#إذا CONFIG_FREERTOS_CHECK_STACKOVERFLOW_NONE
    #تعریف_کړه config_CHECK_FOR_ډېری_ډکېدنه    0
#وإلاإذا CONFIG_FREERTOS_CHECK_STACKOVERFLOW_PTRVAL
    #تعریف_کړه config_CHECK_FOR_ډېری_ډکېدنه    1
#وإلاإذا CONFIG_FREERTOS_CHECK_STACKOVERFLOW_CANARY
    #تعریف_کړه config_CHECK_FOR_ډېری_ډکېدنه    2
#د_که_پای /* CONFIG_FREERTOS_CHECK_STACKOVERFLOW_NONE */
#تعریف_کړه config_ریکارډ_ډېری_لوړ_پته       1             /* This must be set as the port requires TCB.pxEndOfStack */

/* ------------------- Run-time Stats ---------------------- */

#که_تعریف_شوی CONFIG_FREERTOS_GENERATE_پرلپسې_وخت_STATS
    #تعریف_کړه config_GENERATE_پرلپسې_وخت_STATS           1       /* Used by vTaskGetRunTimeStats() */
#د_که_پای /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
#که_تعریف_شوی CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS
    #تعریف_کړه configUSE_STATS_FORMATTING_FUNCTIONS    1       /* Used by vTaskList() */
#د_که_پای /* CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS */

#إذا CONFIG_FREERTOS_پرلپسې_وخت_COUNTER_TYPE_U32
    #تعریف_کړه config_پرلپسې_وخت_COUNTER_TYPE    uint32_t
#وإلاإذا CONFIG_FREERTOS_پرلپسې_وخت_COUNTER_TYPE_U64
    #تعریف_کړه config_پرلپسې_وخت_COUNTER_TYPE    uint64_t
#د_که_پای /* CONFIG_FREERTOS_RUN_TIME_COUNTER_TYPE_U64 */

/* -------------------- Co-routines  ----------------------- */

#تعریف_کړه configUSE_CO_ROUTINES              0              /* CO_ROUTINES are not supported in ESP-IDF */
#تعریف_کړه configMAX_CO_ROUTINE_PRIORITIES    2

/* ------------------- Software Timer ---------------------- */

#إذا CONFIG_FREERTOS_USE_TIMERS
    #تعریف_کړه configUSE_TIMERS                          1
    #تعریف_کړه config_TIMER_TASK_لومړیتوب                 CONFIG_FREERTOS_TIMER_TASK_لومړیتوب
    #تعریف_کړه config_TIMER_کتار_اوږدتیا                  CONFIG_FREERTOS_TIMER_کتار_اوږدتیا
    #تعریف_کړه config_TIMER_TASK_ډېری_ژوروالی              CONFIG_FREERTOS_TIMER_TASK_ډېری_ژوروالی
    #تعریف_کړه configTIMER_SERVICE_TASK_NAME             CONFIG_FREERTOS_TIMER_SERVICE_TASK_NAME
    #تعریف_کړه configTIMER_SERVICE_TASK_CORE_AFFINITY    CONFIG_FREERTOS_TIMER_SERVICE_TASK_CORE_AFFINITY
#وإلا
    #تعریف_کړه configUSE_TIMERS                          0
#د_که_پای


/* ------------------------ List --------------------------- */

#تعریف_کړه configLIST_VOLATILE    متطاير                     /* We define List elements as volatile to prevent the compiler from optimizing out essential code */

/* -------------------- API Includes ----------------------- */

#إذا CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY
    #تعریف_کړه configENABLE_BACKWARD_COMPATIBILITY    1
#وإلا /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */
    #تعریف_کړه configENABLE_BACKWARD_COMPATIBILITY    0
#د_که_پای /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */

#تعریف_کړه INCLUDE_v_Task_لومړیتوب_سېټ                   1
#تعریف_کړه INCLUDE_ux_Task_لومړیتوب_Get                  1
#تعریف_کړه INCLUDE_vTaskDelete                        1
#تعریف_کړه INCLUDE_vTaskSuspend                       1
#تعریف_کړه INCLUDE_vTaskDelay                         1
#تعریف_کړه INCLUDE_xTaskGetIdleTaskHandle             1
#تعریف_کړه INCLUDE_xTaskAbortDelay                    1
#تعریف_کړه INCLUDE_xSemaphoreGetMutexHolder           1
#تعریف_کړه INCLUDE_xTaskGetHandle                     1
#تعریف_کړه INCLUDE_ux_Task_Get_ډېری_لوړ_Water_Mark        1
#تعریف_کړه INCLUDE_eTaskGetState                      1
#تعریف_کړه INCLUDE_xTaskResumeFromISR                 1
#إذا CONFIG_FREERTOS_USE_TIMERS
  #تعریف_کړه INCLUDE_xTimerPendFunctionCall           1
#وإلا
  #تعریف_کړه INCLUDE_xTimerPendFunctionCall           0
#د_که_پای
#تعریف_کړه INCLUDE_xTaskGetSchedulerState             1
#تعریف_کړه INCLUDE_xTaskGetCurrentTaskHandle          1

/* -------------------- Trace Macros ----------------------- */

/*
 * For trace macros.
 * Note: Include trace macros here and not above as trace macros are dependent on some of the FreeRTOS configs
 */
#که_نه_تعریف_شوی __ASSEMBLER__
    #إذا CONFIG_ESP_TRACE_ENABLE
        #ضمن "esp_trace_freertos.h"
    #د_که_پای

    #إذا CONFIG_FREERTOS_SMP

/* Default values for trace macros added to ESP-IDF implementation of SYSVIEW
 * that is not part of Amazon SMP FreeRTOS. */
        #که_نه_تعریف_شوی traceISR_EXIT
            #تعریف_کړه traceISR_EXIT()
        #د_که_پای
        #که_نه_تعریف_شوی traceISR_ENTER
            #تعریف_کړه traceISR_ENTER( _n_ )
        #د_که_پای

        #که_نه_تعریف_شوی trace_کتار_GIVE_FROM_ISR
            #تعریف_کړه trace_کتار_GIVE_FROM_ISR( px_کتار )
        #د_که_پای

        #که_نه_تعریف_شوی trace_کتار_GIVE_FROM_ISR_FAILED
            #تعریف_کړه trace_کتار_GIVE_FROM_ISR_FAILED( px_کتار )
        #د_که_پای

        #که_نه_تعریف_شوی trace_کتار_SEMAPHORE_RECEIVE
            #تعریف_کړه trace_کتار_SEMAPHORE_RECEIVE( px_کتار )
        #د_که_پای
    #د_که_پای /* CONFIG_FREERTOS_SMP */
#د_که_پای /* def __ASSEMBLER__ */

#إذا CONFIG_FREERTOS_USE_APPLICATION_TASK_نښان
    #تعریف_کړه config_USE_APPLICATION_TASK_نښان    1
#د_که_پای // CONFIG_FREERTOS_USE_APPLICATION_TASK_TAG

/* -------------- List Data Integrity Checks --------------- */
#تعریف_کړه config_USE_LIST_معلومات_INTEGRITY_CHECK_بایټونه    CONFIG_FREERTOS_USE_LIST_معلومات_INTEGRITY_CHECK_بایټونه

/* ----------------------------------------------- Amazon SMP FreeRTOS -------------------------------------------------
 * - All Amazon SMP FreeRTOS specific configurations
 * ------------------------------------------------------------------------------------------------------------------ */

#إذا CONFIG_FREERTOS_SMP && ( CONFIG_FREERTOS_شمېره_د_CORES > 1 )
    #تعریف_کړه configUSE_CORE_AFFINITY              1
    #تعریف_کړه config_پرلپسې_مضرب_PRIORITIES        1
    #تعریف_کړه configUSE_TASK_PREEMPTION_DISABLE    1
#د_که_پای /* CONFIG_FREERTOS_SMP && ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */

/* -------------------------------------------------- IDF FreeRTOS -----------------------------------------------------
 * - All IDF FreeRTOS specific configurations
 * ------------------------------------------------------------------------------------------------------------------ */

#إذا !CONFIG_FREERTOS_SMP
    #که_تعریف_شوی CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID
        #تعریف_کړه configTASKLIST_INCLUDE_COREID                  1
    #د_که_پای /* CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID */
    #که_تعریف_شوی CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
        #تعریف_کړه configTHREAD_LOCAL_STORAGE_DELETE_CALLBACKS    1
    #د_که_پای /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
#د_که_پای /* !CONFIG_FREERTOS_SMP */

/* ------------------------------------------------ ESP-IDF Additions --------------------------------------------------
 * - Any other macros required by the rest of ESP-IDF
 * ------------------------------------------------------------------------------------------------------------------ */

/* portNUM_PROCESSORS is deprecated and will be removed in ESP-IDF v6.0 (IDF-8785)
 * Please use the Kconfig option CONFIG_FREERTOS_NUMBER_OF_CORES instead.
 */
#تعریف_کړه پورټ_عدد_PROCESSORS    config_شمېره_د_CORES
