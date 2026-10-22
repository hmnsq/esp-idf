/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ہدایت once

#شامل "sdkconfig.h"

/* This file gets pulled into assembly sources. Therefore, some includes need to
 * be wrapped in #ifndef __ASSEMBLER__ */

#اگر_غیر_تعریف_شدہ __ASSEMBLER__
    /* For configASSERT() */
    #شامل <assert.h>
#اگر_ختم /* def __ASSEMBLER__ */

/* ----------------------------------------------------- Helpers -------------------------------------------------------
 * - Macros that the FreeRTOS configuration macros depend on
 * ------------------------------------------------------------------------------------------------------------------ */

/* Higher stack checker modes cause overhead on each function call */
#اگر CONFIG_ڈھیر_CHECK_ALL || CONFIG_ڈھیر_CHECK_STRONG
    #تعریف_کرو ڈھیر_OVERHEAD_CHECKER    256
#ورنہ
    #تعریف_کرو ڈھیر_OVERHEAD_CHECKER    0
#اگر_ختم

/* with optimizations disabled, scheduler uses additional stack */
#اگر CONFIG_COMPILER_OPTIMIZATION_NONE
    #تعریف_کرو ڈھیر_OVERHEAD_OPTIMIZATION    320
#ورنہ
    #تعریف_کرو ڈھیر_OVERHEAD_OPTIMIZATION    0
#اگر_ختم

/* apptrace module increases minimum stack usage */
#اگر CONFIG_ESP_TRACE_TRANSPORT_APPTRACE
    #تعریف_کرو ڈھیر_OVERHEAD_APPTRACE    1280
#ورنہ
    #تعریف_کرو ڈھیر_OVERHEAD_APPTRACE    0
#اگر_ختم

/* Stack watchpoint decreases minimum usable stack size by up to 60 bytes.
 * See FreeRTOS FREERTOS_WATCHPOINT_END_OF_STACK option in Kconfig. */
#اگر CONFIG_FREERTOS_WATCHPOINT_انتہا_کا_ڈھیر
    #تعریف_کرو ڈھیر_OVERHEAD_WATCHPOINT    60
#ورنہ
    #تعریف_کرو ڈھیر_OVERHEAD_WATCHPOINT    0
#اگر_ختم

#تعریف_کرو config_ڈھیر_OVERHEAD_کل    \
    (                                 \
        ڈھیر_OVERHEAD_CHECKER +      \
        ڈھیر_OVERHEAD_OPTIMIZATION + \
        ڈھیر_OVERHEAD_APPTRACE +     \
        ڈھیر_OVERHEAD_WATCHPOINT     \
    )

/* The arch-specific FreeRTOSConfig_arch.h in esp_additions/arch_include/<arch>.
 * Placed here due to configSTACK_OVERHEAD_TOTAL. Todo: IDF-5712. */
#شامل "freertos/FreeRTOSConfig_arch.h"

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

#تعریف_کرو configUSE_PREEMPTION                         1
#تعریف_کرو configUSE_TICKLESS_IDLE                      CONFIG_FREERTOS_USE_TICKLESS_IDLE
#اگر configUSE_TICKLESS_IDLE
    #تعریف_کرو config_EXPECTED_IDLE_وقت_BEFORE_SLEEP    CONFIG_FREERTOS_IDLE_وقت_BEFORE_SLEEP
#اگر_ختم /* configUSE_TICKLESS_IDLE */
#تعریف_کرو configCPU_CLOCK_HZ                           ( CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ * 1000000 )
#تعریف_کرو config_TICK_شرح_HZ                           CONFIG_FREERTOS_HZ
#تعریف_کرو configMAX_PRIORITIES                         ( 25 )
#تعریف_کرو config_USE_وقت_SLICING                       1
#تعریف_کرو config_USE_16_ثنائی_ہندسہ_TICKS                       0
#تعریف_کرو configIDLE_SHOULD_YIELD                      0
#تعریف_کرو config_KERNEL_مداخلت_ترجیح              1      /*Todo: This currently isn't used anywhere */
#تعریف_کرو config_نمبر_کا_CORES                        CONFIG_FREERTOS_نمبر_کا_CORES
/* For compatibility */
#تعریف_کرو config_عدد_CORES                              config_نمبر_کا_CORES

/* ------------- Synchronization Primitives ---------------- */

#تعریف_کرو configUSE_MUTEXES                        1
#تعریف_کرو configUSE_RECURSIVE_MUTEXES              1
#تعریف_کرو configUSE_COUNTING_SEMAPHORES            1
#تعریف_کرو config_USE_قطار_SETS                     1
#تعریف_کرو config_قطار_REGISTRY_سائز                CONFIG_FREERTOS_قطار_REGISTRY_سائز
#تعریف_کرو configUSE_TASK_NOTIFICATIONS             1
#تعریف_کرو configTASK_NOTIFICATION_ARRAY_ENTRIES    CONFIG_FREERTOS_TASK_NOTIFICATION_ARRAY_ENTRIES

/* ----------------------- System -------------------------- */

#تعریف_کرو configMAX_TASK_NAME_LEN    CONFIG_FREERTOS_MAX_TASK_NAME_LEN

/* If deletion callbacks are enabled, the number of TLSP's are doubled (i.e.,
 * the length of the TCB's pvThreadLocalStoragePointersThis array). This allows
 * the latter half of the array to store the deletion callback pointers (whereas
 * the first half stores the TLSPs themselves). */
#اگر CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
    #تعریف_کرو config_عدد_THREAD_LOCAL_STORAGE_اشارے    ( CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_اشارے * 2 )
#ورنہ /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
    #تعریف_کرو config_عدد_THREAD_LOCAL_STORAGE_اشارے    CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_اشارے
#اگر_ختم /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

#تعریف_کرو config_ڈھیر_گہرائی_TYPE                         uint32_t
#اگر CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY
    #تعریف_کرو configENABLE_BACKWARD_COMPATIBILITY        1
#ورنہ /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */
    #تعریف_کرو configENABLE_BACKWARD_COMPATIBILITY        0
#اگر_ختم /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */
#تعریف_کرو configASSERT( a )    أكد( a )

/* ----------------------- Memory  ------------------------- */

#تعریف_کرو config_SUPPORT_STATIC_تخصیص              1
#تعریف_کرو config_SUPPORT_DYNAMIC_تخصیص             1
#تعریف_کرو config_APPLICATION_ALLOCATED_ڈھیر             1
#اگر CONFIG_FREERTOS_رکھو_TASK_STACKS_اندر_EXT_RAM
    #تعریف_کرو config_ڈھیر_تخصیص_FROM_SEPARATE_ڈھیر    1
#ورنہ
    #تعریف_کرو config_ڈھیر_تخصیص_FROM_SEPARATE_ڈھیر    0
#اگر_ختم

/* ------------------------ Hooks -------------------------- */

#اگر CONFIG_FREERTOS_USE_IDLE_HOOK
    #تعریف_کرو configUSE_IDLE_HOOK               1
#ورنہ /* CONFIG_FREERTOS_USE_IDLE_HOOK */
    #تعریف_کرو configUSE_IDLE_HOOK               0
#اگر_ختم /* CONFIG_FREERTOS_USE_IDLE_HOOK */
#اگر CONFIG_FREERTOS_USE_TICK_HOOK
    #تعریف_کرو configUSE_TICK_HOOK               1
#ورنہ /* CONFIG_FREERTOS_USE_TICK_HOOK */
    #تعریف_کرو configUSE_TICK_HOOK               0
#اگر_ختم /* CONFIG_FREERTOS_USE_TICK_HOOK */
#اگر CONFIG_FREERTOS_CHECK_STACKOVERFLOW_NONE
    #تعریف_کرو config_CHECK_FOR_ڈھیر_حد_سے_تجاوز    0
#وگرنہ_اگر CONFIG_FREERTOS_CHECK_STACKOVERFLOW_PTRVAL
    #تعریف_کرو config_CHECK_FOR_ڈھیر_حد_سے_تجاوز    1
#وگرنہ_اگر CONFIG_FREERTOS_CHECK_STACKOVERFLOW_CANARY
    #تعریف_کرو config_CHECK_FOR_ڈھیر_حد_سے_تجاوز    2
#اگر_ختم /* CONFIG_FREERTOS_CHECK_STACKOVERFLOW_NONE */
#تعریف_کرو config_ریکارڈ_ڈھیر_بلند_پتہ       1             /* This must be set as the port requires TCB.pxEndOfStack */

/* ------------------- Run-time Stats ---------------------- */

#اگر_تعریف_شدہ CONFIG_FREERTOS_GENERATE_تسلسل_وقت_STATS
    #تعریف_کرو config_GENERATE_تسلسل_وقت_STATS           1       /* Used by vTaskGetRunTimeStats() */
#اگر_ختم /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
#اگر_تعریف_شدہ CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS
    #تعریف_کرو configUSE_STATS_FORMATTING_FUNCTIONS    1       /* Used by vTaskList() */
#اگر_ختم /* CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS */

#اگر CONFIG_FREERTOS_تسلسل_وقت_شمار_TYPE_U32
    #تعریف_کرو config_تسلسل_وقت_شمار_TYPE    uint32_t
#وگرنہ_اگر CONFIG_FREERTOS_تسلسل_وقت_شمار_TYPE_U64
    #تعریف_کرو config_تسلسل_وقت_شمار_TYPE    uint64_t
#اگر_ختم /* CONFIG_FREERTOS_RUN_TIME_COUNTER_TYPE_U64 */

/* -------------------- Co-routines  ----------------------- */

#تعریف_کرو configUSE_CO_ROUTINES              0              /* CO_ROUTINES are not supported in ESP-IDF */
#تعریف_کرو configMAX_CO_ROUTINE_PRIORITIES    2

/* ------------------- Software Timer ---------------------- */

#اگر CONFIG_FREERTOS_USE_TIMERS
    #تعریف_کرو configUSE_TIMERS                          1
    #تعریف_کرو config_TIMER_TASK_ترجیح                 CONFIG_FREERTOS_TIMER_TASK_ترجیح
    #تعریف_کرو config_TIMER_قطار_لمبائی                  CONFIG_FREERTOS_TIMER_قطار_لمبائی
    #تعریف_کرو config_TIMER_TASK_ڈھیر_گہرائی              CONFIG_FREERTOS_TIMER_TASK_ڈھیر_گہرائی
    #تعریف_کرو configTIMER_SERVICE_TASK_NAME             CONFIG_FREERTOS_TIMER_SERVICE_TASK_NAME
    #تعریف_کرو configTIMER_SERVICE_TASK_CORE_AFFINITY    CONFIG_FREERTOS_TIMER_SERVICE_TASK_CORE_AFFINITY
#ورنہ
    #تعریف_کرو configUSE_TIMERS                          0
#اگر_ختم


/* ------------------------ List --------------------------- */

#تعریف_کرو configLIST_VOLATILE    متلون                     /* We define List elements as volatile to prevent the compiler from optimizing out essential code */

/* -------------------- API Includes ----------------------- */

#اگر CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY
    #تعریف_کرو configENABLE_BACKWARD_COMPATIBILITY    1
#ورنہ /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */
    #تعریف_کرو configENABLE_BACKWARD_COMPATIBILITY    0
#اگر_ختم /* CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY */

#تعریف_کرو INCLUDE_v_Task_ترجیح_سیٹ                   1
#تعریف_کرو INCLUDE_ux_Task_ترجیح_Get                  1
#تعریف_کرو INCLUDE_vTaskDelete                        1
#تعریف_کرو INCLUDE_vTaskSuspend                       1
#تعریف_کرو INCLUDE_vTaskDelay                         1
#تعریف_کرو INCLUDE_xTaskGetIdleTaskHandle             1
#تعریف_کرو INCLUDE_xTaskAbortDelay                    1
#تعریف_کرو INCLUDE_xSemaphoreGetMutexHolder           1
#تعریف_کرو INCLUDE_xTaskGetHandle                     1
#تعریف_کرو INCLUDE_ux_Task_Get_ڈھیر_بلند_Water_Mark        1
#تعریف_کرو INCLUDE_eTaskGetState                      1
#تعریف_کرو INCLUDE_xTaskResumeFromISR                 1
#اگر CONFIG_FREERTOS_USE_TIMERS
  #تعریف_کرو INCLUDE_xTimerPendFunctionCall           1
#ورنہ
  #تعریف_کرو INCLUDE_xTimerPendFunctionCall           0
#اگر_ختم
#تعریف_کرو INCLUDE_xTaskGetSchedulerState             1
#تعریف_کرو INCLUDE_xTaskGetCurrentTaskHandle          1

/* -------------------- Trace Macros ----------------------- */

/*
 * For trace macros.
 * Note: Include trace macros here and not above as trace macros are dependent on some of the FreeRTOS configs
 */
#اگر_غیر_تعریف_شدہ __ASSEMBLER__
    #اگر CONFIG_ESP_TRACE_ENABLE
        #شامل "esp_trace_freertos.h"
    #اگر_ختم

    #اگر CONFIG_FREERTOS_SMP

/* Default values for trace macros added to ESP-IDF implementation of SYSVIEW
 * that is not part of Amazon SMP FreeRTOS. */
        #اگر_غیر_تعریف_شدہ traceISR_EXIT
            #تعریف_کرو traceISR_EXIT()
        #اگر_ختم
        #اگر_غیر_تعریف_شدہ traceISR_ENTER
            #تعریف_کرو traceISR_ENTER( _n_ )
        #اگر_ختم

        #اگر_غیر_تعریف_شدہ trace_قطار_GIVE_FROM_ISR
            #تعریف_کرو trace_قطار_GIVE_FROM_ISR( px_قطار )
        #اگر_ختم

        #اگر_غیر_تعریف_شدہ trace_قطار_GIVE_FROM_ISR_FAILED
            #تعریف_کرو trace_قطار_GIVE_FROM_ISR_FAILED( px_قطار )
        #اگر_ختم

        #اگر_غیر_تعریف_شدہ trace_قطار_SEMAPHORE_RECEIVE
            #تعریف_کرو trace_قطار_SEMAPHORE_RECEIVE( px_قطار )
        #اگر_ختم
    #اگر_ختم /* CONFIG_FREERTOS_SMP */
#اگر_ختم /* def __ASSEMBLER__ */

#اگر CONFIG_FREERTOS_USE_APPLICATION_TASK_لیبل
    #تعریف_کرو config_USE_APPLICATION_TASK_لیبل    1
#اگر_ختم // CONFIG_FREERTOS_USE_APPLICATION_TASK_TAG

/* -------------- List Data Integrity Checks --------------- */
#تعریف_کرو config_USE_LIST_ڈیٹا_INTEGRITY_CHECK_بائٹیں    CONFIG_FREERTOS_USE_LIST_ڈیٹا_INTEGRITY_CHECK_بائٹیں

/* ----------------------------------------------- Amazon SMP FreeRTOS -------------------------------------------------
 * - All Amazon SMP FreeRTOS specific configurations
 * ------------------------------------------------------------------------------------------------------------------ */

#اگر CONFIG_FREERTOS_SMP && ( CONFIG_FREERTOS_نمبر_کا_CORES > 1 )
    #تعریف_کرو configUSE_CORE_AFFINITY              1
    #تعریف_کرو config_تسلسل_مضرب_PRIORITIES        1
    #تعریف_کرو configUSE_TASK_PREEMPTION_DISABLE    1
#اگر_ختم /* CONFIG_FREERTOS_SMP && ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */

/* -------------------------------------------------- IDF FreeRTOS -----------------------------------------------------
 * - All IDF FreeRTOS specific configurations
 * ------------------------------------------------------------------------------------------------------------------ */

#اگر !CONFIG_FREERTOS_SMP
    #اگر_تعریف_شدہ CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID
        #تعریف_کرو configTASKLIST_INCLUDE_COREID                  1
    #اگر_ختم /* CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID */
    #اگر_تعریف_شدہ CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
        #تعریف_کرو configTHREAD_LOCAL_STORAGE_DELETE_CALLBACKS    1
    #اگر_ختم /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
#اگر_ختم /* !CONFIG_FREERTOS_SMP */

/* ------------------------------------------------ ESP-IDF Additions --------------------------------------------------
 * - Any other macros required by the rest of ESP-IDF
 * ------------------------------------------------------------------------------------------------------------------ */

/* portNUM_PROCESSORS is deprecated and will be removed in ESP-IDF v6.0 (IDF-8785)
 * Please use the Kconfig option CONFIG_FREERTOS_NUMBER_OF_CORES instead.
 */
#تعریف_کرو پورٹ_عدد_PROCESSORS    config_نمبر_کا_CORES
