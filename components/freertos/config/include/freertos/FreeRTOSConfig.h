/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#رهنمود once

#ضمن "sdkconfig.h"

/* This file gets pulled into assembly sources. Therefore, some includes need to
 * be wrapped in #ifndef __ASSEMBLER__ */

#اگر_تعریف_نشده __ASSEMBLER__
    /* For configASSERT() */
    #ضمن <assert.h>
#پایان_اگر /* def __ASSEMBLER__ */

/* ----------------------------------------------------- Helpers -------------------------------------------------------
 * - Macros that the FreeRTOS configuration macros depend on
 * ------------------------------------------------------------------------------------------------------------------ */

/* Higher stack checker modes cause overhead on each function call */
#إذا CONFIG_پشته_CHECK_ALL || CONFIG_پشته_CHECK_STRONG
    #تعریف_کن پشته_OVERHEAD_CHECKER    256
#وإلا
    #تعریف_کن پشته_OVERHEAD_CHECKER    0
#پایان_اگر

/* with optimizations disabled, scheduler uses additional stack */
#إذا CONFIG_COMPILER_OPTIMIZATION_NONE
    #تعریف_کن پشته_OVERHEAD_OPTIMIZATION    320
#وإلا
    #تعریف_کن پشته_OVERHEAD_OPTIMIZATION    0
#پایان_اگر

/* apptrace module increases minimum stack usage */
#إذا CONFIG_ESP_TRACE_TRANSPORT_APPTRACE
    #تعریف_کن پشته_OVERHEAD_APPTRACE    1280
#وإلا
    #تعریف_کن پشته_OVERHEAD_APPTRACE    0
#پایان_اگر

/* Stack watchpoint decreases minimum usable stack size by up to 60 bytes.
 * See FreeRTOS FREERTOS_WATCHPOINT_END_OF_STACK option in Kconfig. */
#إذا CONFIG_FREERTOS_WATCHPOINT_انتها_از_پشته
    #تعریف_کن پشته_OVERHEAD_WATCHPOINT    60
#وإلا
    #تعریف_کن پشته_OVERHEAD_WATCHPOINT    0
#پایان_اگر

#تعریف_کن config_پشته_OVERHEAD_مجموع    \
    (                                 \
        پشته_OVERHEAD_CHECKER +      \
        پشته_OVERHEAD_OPTIMIZATION + \
        پشته_OVERHEAD_APPTRACE +     \
        پشته_OVERHEAD_WATCHPOINT     \
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

#تعریف_کن configUSE_PREEMPTION                         1
#تعریف_کن configUSE_TICKLESS_IDLE                      CONFIG_FREERTOS_USE_TICKLESS_IDLE
#إذا configUSE_TICKLESS_IDLE
    #تعریف_کن config_EXPECTED_IDLE_زمان_BEFORE_SLEEP    CONFIG_FREERTOS_IDLE_زمان_BEFORE_SLEEP
#پایان_اگر /* configUSE_TICKLESS_IDLE */
#تعریف_کن configCPU_CLOCK_HZ                           ( CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ * 1000000 )
#تعریف_کن config_TICK_نرخ_HZ                           CONFIG_FREERTOS_HZ
#تعریف_کن configMAX_PRIORITIES                         ( 25 )
#تعریف_کن config_USE_زمان_SLICING                       1
#تعریف_کن config_USE_16_بیت_TICKS                       0
#تعریف_کن configIDLE_SHOULD_YIELD                      0
#تعریف_کن config_KERNEL_وقفه_اولویت              1      /*Todo: This currently isn't used anywhere */
#تعریف_کن config_عدد_از_CORES                        CONFIG_FREERTOS_عدد_از_CORES
/* For compatibility */
#تعریف_کن config_رقم_CORES                              config_عدد_از_CORES

/* ------------- Synchronization Primitives ---------------- */

#تعریف_کن configUSE_MUTEXES                        1
#تعریف_کن configUSE_RECURSIVE_MUTEXES              1
#تعریف_کن configUSE_COUNTING_SEMAPHORES            1
#تعریف_کن config_USE_صف_SETS                     1
#تعریف_کن config_صف_REGISTRY_اندازه                CONFIG_FREERTOS_صف_REGISTRY_اندازه
#تعریف_کن configUSE_TASK_NOTIFICATIONS             1
#تعریف_کن configTASK_NOTIFICATION_ARRAY_ENTRIES    CONFIG_FREERTOS_TASK_NOTIFICATION_ARRAY_ENTRIES

/* ----------------------- System -------------------------- */

#تعریف_کن configMAX_TASK_NAME_LEN    CONFIG_FREERTOS_MAX_TASK_NAME_LEN

/* If deletion callbacks are enabled, the number of TLSP's are doubled (i.e.,
 * the length of the TCB's pvThreadLocalStoragePointersThis array). This allows
 * the latter half of the array to store the deletion callback pointers (whereas
 * the first half stores the TLSPs themselves). */
#إذا CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
    #تعریف_کن config_رقم_THREAD_LOCAL_STORAGE_اشارهگرها    ( CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_اشارهگرها * 2 )
#وإلا /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
    #تعریف_کن config_رقم_THREAD_LOCAL_STORAGE_اشارهگرها    CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_اشارهگرها
#پایان_اگر /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

#تعریف_کن config_پشته_عمق_TYPE                         uint32_t
#إذا CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY
    #تعریف_کن configENABLE_BACKWARD_COMPATIBILITY        1
#وإلا /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */
    #تعریف_کن configENABLE_BACKWARD_COMPATIBILITY        0
#پایان_اگر /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */
#تعریف_کن configASSERT( a )    أكد( a )

/* ----------------------- Memory  ------------------------- */

#تعریف_کن config_SUPPORT_STATIC_تخصیص              1
#تعریف_کن config_SUPPORT_DYNAMIC_تخصیص             1
#تعریف_کن config_APPLICATION_ALLOCATED_انباشته             1
#إذا CONFIG_FREERTOS_قراربده_TASK_STACKS_درون_EXT_RAM
    #تعریف_کن config_پشته_تخصیص_FROM_SEPARATE_انباشته    1
#وإلا
    #تعریف_کن config_پشته_تخصیص_FROM_SEPARATE_انباشته    0
#پایان_اگر

/* ------------------------ Hooks -------------------------- */

#إذا CONFIG_FREERTOS_USE_IDLE_HOOK
    #تعریف_کن configUSE_IDLE_HOOK               1
#وإلا /* CONFIG_FREERTOS_USE_IDLE_HOOK */
    #تعریف_کن configUSE_IDLE_HOOK               0
#پایان_اگر /* CONFIG_FREERTOS_USE_IDLE_HOOK */
#إذا CONFIG_FREERTOS_USE_TICK_HOOK
    #تعریف_کن configUSE_TICK_HOOK               1
#وإلا /* CONFIG_FREERTOS_USE_TICK_HOOK */
    #تعریف_کن configUSE_TICK_HOOK               0
#پایان_اگر /* CONFIG_FREERTOS_USE_TICK_HOOK */
#إذا CONFIG_FREERTOS_CHECK_STACKOVERFLOW_NONE
    #تعریف_کن config_CHECK_FOR_پشته_سرریز    0
#وإلاإذا CONFIG_FREERTOS_CHECK_STACKOVERFLOW_PTRVAL
    #تعریف_کن config_CHECK_FOR_پشته_سرریز    1
#وإلاإذا CONFIG_FREERTOS_CHECK_STACKOVERFLOW_CANARY
    #تعریف_کن config_CHECK_FOR_پشته_سرریز    2
#پایان_اگر /* CONFIG_FREERTOS_CHECK_STACKOVERFLOW_NONE */
#تعریف_کن config_رکورد_پشته_بالا_آدرس       1             /* This must be set as the port requires TCB.pxEndOfStack */

/* ------------------- Run-time Stats ---------------------- */

#اگر_تعریف_شده CONFIG_FREERTOS_GENERATE_پیاپی_زمان_STATS
    #تعریف_کن config_GENERATE_پیاپی_زمان_STATS           1       /* Used by vTaskGetRunTimeStats() */
#پایان_اگر /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
#اگر_تعریف_شده CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS
    #تعریف_کن configUSE_STATS_FORMATTING_FUNCTIONS    1       /* Used by vTaskList() */
#پایان_اگر /* CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS */

#إذا CONFIG_FREERTOS_پیاپی_زمان_COUNTER_TYPE_U32
    #تعریف_کن config_پیاپی_زمان_COUNTER_TYPE    uint32_t
#وإلاإذا CONFIG_FREERTOS_پیاپی_زمان_COUNTER_TYPE_U64
    #تعریف_کن config_پیاپی_زمان_COUNTER_TYPE    uint64_t
#پایان_اگر /* CONFIG_FREERTOS_RUN_TIME_COUNTER_TYPE_U64 */

/* -------------------- Co-routines  ----------------------- */

#تعریف_کن configUSE_CO_ROUTINES              0              /* CO_ROUTINES are not supported in ESP-IDF */
#تعریف_کن configMAX_CO_ROUTINE_PRIORITIES    2

/* ------------------- Software Timer ---------------------- */

#إذا CONFIG_FREERTOS_USE_TIMERS
    #تعریف_کن configUSE_TIMERS                          1
    #تعریف_کن config_TIMER_TASK_اولویت                 CONFIG_FREERTOS_TIMER_TASK_اولویت
    #تعریف_کن config_TIMER_صف_درازی                  CONFIG_FREERTOS_TIMER_صف_درازی
    #تعریف_کن config_TIMER_TASK_پشته_عمق              CONFIG_FREERTOS_TIMER_TASK_پشته_عمق
    #تعریف_کن configTIMER_SERVICE_TASK_NAME             CONFIG_FREERTOS_TIMER_SERVICE_TASK_NAME
    #تعریف_کن configTIMER_SERVICE_TASK_CORE_AFFINITY    CONFIG_FREERTOS_TIMER_SERVICE_TASK_CORE_AFFINITY
#وإلا
    #تعریف_کن configUSE_TIMERS                          0
#پایان_اگر


/* ------------------------ List --------------------------- */

#تعریف_کن configLIST_VOLATILE    متطاير                     /* We define List elements as volatile to prevent the compiler from optimizing out essential code */

/* -------------------- API Includes ----------------------- */

#إذا CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY
    #تعریف_کن configENABLE_BACKWARD_COMPATIBILITY    1
#وإلا /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */
    #تعریف_کن configENABLE_BACKWARD_COMPATIBILITY    0
#پایان_اگر /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */

#تعریف_کن INCLUDE_v_Task_اولویت_مجموعه                   1
#تعریف_کن INCLUDE_ux_Task_اولویت_Get                  1
#تعریف_کن INCLUDE_vTaskDelete                        1
#تعریف_کن INCLUDE_vTaskSuspend                       1
#تعریف_کن INCLUDE_vTaskDelay                         1
#تعریف_کن INCLUDE_xTaskGetIdleTaskHandle             1
#تعریف_کن INCLUDE_xTaskAbortDelay                    1
#تعریف_کن INCLUDE_xSemaphoreGetMutexHolder           1
#تعریف_کن INCLUDE_xTaskGetHandle                     1
#تعریف_کن INCLUDE_ux_Task_Get_پشته_بالا_Water_Mark        1
#تعریف_کن INCLUDE_eTaskGetState                      1
#تعریف_کن INCLUDE_xTaskResumeFromISR                 1
#إذا CONFIG_FREERTOS_USE_TIMERS
  #تعریف_کن INCLUDE_xTimerPendFunctionCall           1
#وإلا
  #تعریف_کن INCLUDE_xTimerPendFunctionCall           0
#پایان_اگر
#تعریف_کن INCLUDE_xTaskGetSchedulerState             1
#تعریف_کن INCLUDE_xTaskGetCurrentTaskHandle          1

/* -------------------- Trace Macros ----------------------- */

/*
 * For trace macros.
 * Note: Include trace macros here and not above as trace macros are dependent on some of the FreeRTOS configs
 */
#اگر_تعریف_نشده __ASSEMBLER__
    #إذا CONFIG_ESP_TRACE_ENABLE
        #ضمن "esp_trace_freertos.h"
    #پایان_اگر

    #إذا CONFIG_FREERTOS_SMP

/* Default values for trace macros added to ESP-IDF implementation of SYSVIEW
 * that is not part of Amazon SMP FreeRTOS. */
        #اگر_تعریف_نشده traceISR_EXIT
            #تعریف_کن traceISR_EXIT()
        #پایان_اگر
        #اگر_تعریف_نشده traceISR_ENTER
            #تعریف_کن traceISR_ENTER( _n_ )
        #پایان_اگر

        #اگر_تعریف_نشده trace_صف_GIVE_FROM_ISR
            #تعریف_کن trace_صف_GIVE_FROM_ISR( px_صف )
        #پایان_اگر

        #اگر_تعریف_نشده trace_صف_GIVE_FROM_ISR_FAILED
            #تعریف_کن trace_صف_GIVE_FROM_ISR_FAILED( px_صف )
        #پایان_اگر

        #اگر_تعریف_نشده trace_صف_SEMAPHORE_RECEIVE
            #تعریف_کن trace_صف_SEMAPHORE_RECEIVE( px_صف )
        #پایان_اگر
    #پایان_اگر /* CONFIG_FREERTOS_SMP */
#پایان_اگر /* def __ASSEMBLER__ */

#إذا CONFIG_FREERTOS_USE_APPLICATION_TASK_برچسب
    #تعریف_کن config_USE_APPLICATION_TASK_برچسب    1
#پایان_اگر // CONFIG_FREERTOS_USE_APPLICATION_TASK_TAG

/* -------------- List Data Integrity Checks --------------- */
#تعریف_کن config_USE_LIST_داده_INTEGRITY_CHECK_بایتها    CONFIG_FREERTOS_USE_LIST_داده_INTEGRITY_CHECK_بایتها

/* ----------------------------------------------- Amazon SMP FreeRTOS -------------------------------------------------
 * - All Amazon SMP FreeRTOS specific configurations
 * ------------------------------------------------------------------------------------------------------------------ */

#إذا CONFIG_FREERTOS_SMP && ( CONFIG_FREERTOS_عدد_از_CORES > 1 )
    #تعریف_کن configUSE_CORE_AFFINITY              1
    #تعریف_کن config_پیاپی_مضرب_PRIORITIES        1
    #تعریف_کن configUSE_TASK_PREEMPTION_DISABLE    1
#پایان_اگر /* CONFIG_FREERTOS_SMP && ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */

/* -------------------------------------------------- IDF FreeRTOS -----------------------------------------------------
 * - All IDF FreeRTOS specific configurations
 * ------------------------------------------------------------------------------------------------------------------ */

#إذا !CONFIG_FREERTOS_SMP
    #اگر_تعریف_شده CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID
        #تعریف_کن configTASKLIST_INCLUDE_COREID                  1
    #پایان_اگر /* CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID */
    #اگر_تعریف_شده CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
        #تعریف_کن configTHREAD_LOCAL_STORAGE_DELETE_CALLBACKS    1
    #پایان_اگر /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
#پایان_اگر /* !CONFIG_FREERTOS_SMP */

/* ------------------------------------------------ ESP-IDF Additions --------------------------------------------------
 * - Any other macros required by the rest of ESP-IDF
 * ------------------------------------------------------------------------------------------------------------------ */

/* portNUM_PROCESSORS is deprecated and will be removed in ESP-IDF v6.0 (IDF-8785)
 * Please use the Kconfig option CONFIG_FREERTOS_NUMBER_OF_CORES instead.
 */
#تعریف_کن پورت_رقم_PROCESSORS    config_عدد_از_CORES
