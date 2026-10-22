/*
 * SPDX-FileCopyrightText: 2025-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <execinfo.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <signal.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <pthread.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_log.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils/linux_port_utils.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BACKTRACE_PC_ARRAY_ਮਾਪ 20
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FREERTOS_SIM_BACKTRACE_ਮਾਪ 16
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ON_SEGFAULT_ਸੁਨੇਹਾ "ERROR: Segmentation Fault, here's your backtrace:\n"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ON_ABORT_ਸੁਨੇਹਾ "ERROR: Aborted\n"

ਸਥਿਰ ਖਾਲੀ linux_ਪੋਰਟ_backtrace_handler(ਪੂਰਨ sig)
{
    /* All calls here must be async-signal-safe (no stdio, no malloc).
     * write() and backtrace_symbols_fd() write directly to the fd. */
    ਖਾਲੀ *ਬਫਰ[FREERTOS_SIM_BACKTRACE_ਮਾਪ];
    ਪੂਰਨ nptrs = backtrace(ਬਫਰ, FREERTOS_SIM_BACKTRACE_ਮਾਪ);

    ਅਚਲ ਅੱਖਰ *name = pcTaskGetName(ਨਿਰਾ); /* simple pointer dereference, no lock */
    ssize_t ignore __attribute__((ਅਣਵਰਤਿਆ));
    ignore = ਲਿਖ(STDERR_FILENO, "=== Backtrace for task: ", 24);
    ਜੇ (name) {
        size_t len = 0;
        ਜਦੋਂ (name[len] != '\0') {
            len++;
        }
        ignore = ਲਿਖ(STDERR_FILENO, name, len);
    }
    ignore = ਲਿਖ(STDERR_FILENO, " ===\n", 5);
    backtrace_symbols_fd(ਬਫਰ, nptrs, STDERR_FILENO);
}

ਖਾਲੀ linux_ਪੋਰਟ_ਤਿਆਰੀ_backtrace_signal(ਖਾਲੀ)
{
    ਬਣਤਰ sigaction sa;
    sa.sa_handler = linux_ਪੋਰਟ_backtrace_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, ਨਿਰਾ);
}

ਖਾਲੀ linux_ਪੋਰਟ_print_backtrace(ਖਾਲੀ)
{
    pthread_kill(linux_ਪੋਰਟ_get_scheduled_task_pthread(), SIGUSR1);
}

ESP_LOG_ATTR_ਟੈਗ(LINUX_ਟੈਗ, "port_idf_linux");
ESP_LOG_ATTR_ਟੈਗ(MAIN_ਟੈਗ, "main_task");

#ਜੇ (defined(__APPLE__) && defined(__MACH__))
ਕਿਸਮਪਰਿਭਾਸ਼ਾ sig_t sighandler_t;
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਅਸਥਿਰ UBaseType_t ux_ਰੁਕਾਵਟ_Nesting = 0;
ਅਧਾਰ_Type_t x_ਪੋਰਟ_Check_If_ਵਿੱਚ_ISR(ਖਾਲੀ)
{
    ਵਾਪਸ ux_ਰੁਕਾਵਟ_Nesting;
}

#ਜੇ CONFIG_COMPILER_OPTIMIZATION_DEBUG
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BACKTRACE_PC_ARRAY_ਮਾਪ_DUMMY 1
ਸਥਿਰ ਖਾਲੀ ਲੱਦੋ_libgcc(ਖਾਲੀ)
{
    ਖਾਲੀ *array[BACKTRACE_PC_ARRAY_ਮਾਪ_DUMMY];
    size_t ਮਾਪ = backtrace(array, BACKTRACE_PC_ARRAY_ਮਾਪ_DUMMY);
    ਪ੍ਰਤਿਜ੍ਞਾ(ਮਾਪ == 1);
}

ਸਥਿਰ ਖਾਲੀ segfault_handler(ਪੂਰਨ sig)
{
    ਖਾਲੀ *array[BACKTRACE_PC_ARRAY_ਮਾਪ];
    size_t ਮਾਪ = backtrace(array, BACKTRACE_PC_ARRAY_ਮਾਪ);
    ssize_t ignore __attribute__((ਅਣਵਰਤਿਆ));
    ignore = ਲਿਖ(STDERR_FILENO, ON_SEGFAULT_ਸੁਨੇਹਾ, ਆਕਾਰ(ON_SEGFAULT_ਸੁਨੇਹਾ));
    backtrace_symbols_fd(array, ਮਾਪ, STDERR_FILENO);
    _exit(1);
}

ਸਥਿਰ ਖਾਲੀ abort_handler(ਪੂਰਨ sig)
{
    // we need a raw file write here because other functions are not async-signal-safe
    ssize_t ignore __attribute__((ਅਣਵਰਤਿਆ));
    ignore = ਲਿਖ(STDERR_FILENO, ON_ABORT_ਸੁਨੇਹਾ, ਆਕਾਰ(ON_ABORT_ਸੁਨੇਹਾ));
    _exit(1);
}
#ਜੇ_ਅੰਤ // CONFIG_COMPILER_OPTIMIZATION_DEBUG

/*-----------------------------------------------------------
* Main FreeRTOS task
*-----------------------------------------------------------*/
ਬਾਹਰੀ ਖਾਲੀ app_main(ਖਾਲੀ);
ਸਥਿਰ ਖਾਲੀ main_task(ਖਾਲੀ *ਦਲੀਲਾਂ)
{
    (ਖਾਲੀ)ਦਲੀਲਾਂ;
    ESP_LOGI(MAIN_ਟੈਗ, "Started on CPU%d", (ਪੂਰਨ)x_ਪੋਰਟ_Get_Core_ID());

    ESP_LOGI(MAIN_ਟੈਗ, "Calling app_main()");

    app_main();

    ESP_LOGI(MAIN_ਟੈਗ, "Returned from app_main()");
    vTaskDelete(ਨਿਰਾ);
}

ਖਾਲੀ esp_startup_ਸ਼ੁਰੂ_app(ਖਾਲੀ)
{
    setvbuf(stdout, ਨਿਰਾ, _IONBF, 0);

#ਜੇ CONFIG_COMPILER_OPTIMIZATION_DEBUG
    ਲੱਦੋ_libgcc();
    sighandler_t sig_res;

    sig_res = ਸੰਕੇਤ(SIGSEGV, segfault_handler);
    ਜੇ (sig_res == SIG_ERR) {
        abort();
    }

    sig_res = ਸੰਕੇਤ(SIGABRT, abort_handler);
    ਜੇ (sig_res == SIG_ERR) {
        abort();
    }
#ਜੇ_ਅੰਤ

    // Create main_task using FreeRTOS API
    ESP_LOGI(LINUX_ਟੈਗ, "Starting main task.");
    ਪ੍ਰਤਿਜ੍ਞਾ(xTaskCreate(&main_task, "main", ESP_TASK_MAIN_ਢੇਰ, ਨਿਰਾ, ESP_TASK_MAIN_PRIO, ਨਿਰਾ) == pdTRUE);

    ESP_LOGI(LINUX_ਟੈਗ, "Starting scheduler task.");
    v_Task_ਸ਼ੁਰੂ_Scheduler();

    // Should never reach here
    ਪ੍ਰਤਿਜ੍ਞਾ(false);
}

/*-----------------------------------------------------------
 * idle and tick hooks
 *-----------------------------------------------------------*/
#ਜੇ (configUSE_IDLE_HOOK > 0)
ਖਾਲੀ vApplicationIdleHook(ਖਾਲੀ)
{
}
#ਜੇ_ਅੰਤ

#ਜੇ (configUSE_TICK_HOOK > 0)
ਖਾਲੀ vApplicationTickHook(ਖਾਲੀ)
{
    ਬਾਹਰੀ ਖਾਲੀ esp_vApplicationTickHook(ਖਾਲੀ);
    esp_vApplicationTickHook();
}
#ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ

/*-----------------------------------------------------------
 * Static allocation support
 *-----------------------------------------------------------*/
#ਜੇ (config_SUPPORT_STATIC_ਵੰਡ == 1)

ਖਾਲੀ v_Application_Get_Idle_Task_ਸਮ੍ਰਿਤੀ(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   ਢੇਰ_Type_t **ppx_Idle_Task_ਢੇਰ_ਬਫਰ,
                                   uint32_t *pul_Idle_Task_ਢੇਰ_ਮਾਪ)
{
    ਸਥਿਰ StaticTask_t xIdleTaskTCB;
    ਸਥਿਰ ਢੇਰ_Type_t ux_Idle_Task_ਢੇਰ[config_ਨਿਊਨਤਮ_ਢੇਰ_ਮਾਪ];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppx_Idle_Task_ਢੇਰ_ਬਫਰ = ux_Idle_Task_ਢੇਰ;
    *pul_Idle_Task_ਢੇਰ_ਮਾਪ = config_ਨਿਊਨਤਮ_ਢੇਰ_ਮਾਪ;
}

#ਜੇ (configUSE_TIMERS == 1)
ਢੇਰ_Type_t ux_Timer_Task_ਢੇਰ[config_TIMER_TASK_ਢੇਰ_ਡੂੰਘਾਈ];

ਖਾਲੀ v_Application_Get_Timer_Task_ਸਮ੍ਰਿਤੀ(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    ਢੇਰ_Type_t **ppx_Timer_Task_ਢੇਰ_ਬਫਰ,
                                    uint32_t *pul_Timer_Task_ਢੇਰ_ਮਾਪ)
{
    ਸਥਿਰ StaticTask_t xTimerTaskTCB;
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppx_Timer_Task_ਢੇਰ_ਬਫਰ = ux_Timer_Task_ਢੇਰ;
    *pul_Timer_Task_ਢੇਰ_ਮਾਪ = config_TIMER_TASK_ਢੇਰ_ਡੂੰਘਾਈ;
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ // configSUPPORT_STATIC_ALLOCATION

/*-----------------------------------------------------------
 * Stack overflow hook
 *-----------------------------------------------------------*/
ਖਾਲੀ __attribute__((weak)) v_Application_ਢੇਰ_ਉਛਾਲ_Hook(TaskHandle_t xTask, ਅੱਖਰ *pcTaskName)
{
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ERR_STR1 "***ERROR*** A stack overflow in task "
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ERR_STR2 " has been detected."
    ਅਚਲ ਅੱਖਰ *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    ਅੱਖਰ buf[ਆਕਾਰ(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + ਆਕਾਰ(ERR_STR2) + 1 /* null char */] = {0};

    ਅੱਖਰ *dest = buf;
    ਲਈ (ਪੂਰਨ i = 0; i < ਆਕਾਰ(str) / ਆਕਾਰ(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
	ਲਿਖੋ("%s\n", buf);
    abort();
}
