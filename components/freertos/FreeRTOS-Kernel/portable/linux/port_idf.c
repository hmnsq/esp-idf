/*
 * SPDX-FileCopyrightText: 2025-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <execinfo.h>
#അന്തര്ഭാവ <signal.h>
#അന്തര്ഭാവ <pthread.h>

#അന്തര്ഭാവ "FreeRTOS.h"
#അന്തര്ഭാവ "task.h"
#അന്തര്ഭാവ "esp_log.h"
#അന്തര്ഭാവ "utils/linux_port_utils.h"

#define BACKTRACE_PC_ARRAY_SIZE 20
#define FREERTOS_SIM_BACKTRACE_SIZE 16
#define ON_SEGFAULT_MESSAGE "ERROR: Segmentation Fault, here's your backtrace:\n"
#define ON_ABORT_MESSAGE "ERROR: Aborted\n"

സ്ഥിതം ശൂന്യം linux_port_backtrace_handler(പൂർണ്ണസംഖ്യ sig)
{
    /* All calls here must be async-signal-safe (no stdio, no malloc).
     * write() and backtrace_symbols_fd() write directly to the fd. */
    ശൂന്യം *buffer[FREERTOS_SIM_BACKTRACE_SIZE];
    പൂർണ്ണസംഖ്യ nptrs = backtrace(buffer, FREERTOS_SIM_BACKTRACE_SIZE);

    സ്ഥിരം അക്ഷരം *name = pcTaskGetName(ശൂന്യ); /* simple pointer dereference, no lock */
    ssize_t ignore __attribute__((unused));
    ignore = ലിഖ(STDERR_FILENO, "=== Backtrace for task: ", 24);
    എങ്കിൽ (name) {
        size_t len = 0;
        ആകുമ്പോൾ (name[len] != '\0') {
            len++;
        }
        ignore = ലിഖ(STDERR_FILENO, name, len);
    }
    ignore = ലിഖ(STDERR_FILENO, " ===\n", 5);
    backtrace_symbols_fd(buffer, nptrs, STDERR_FILENO);
}

ശൂന്യം linux_port_setup_backtrace_signal(ശൂന്യം)
{
    ഘടന sigaction sa;
    sa.sa_handler = linux_port_backtrace_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, ശൂന്യ);
}

ശൂന്യം linux_port_print_backtrace(ശൂന്യം)
{
    pthread_kill(linux_port_get_scheduled_task_pthread(), SIGUSR1);
}

ESP_LOG_ATTR_TAG(LINUX_TAG, "port_idf_linux");
ESP_LOG_ATTR_TAG(MAIN_TAG, "main_task");

#എങ്കിൽ (defined(__APPLE__) && defined(__MACH__))
തരനിർവചനം sig_t sighandler_t;
#endif

സ്ഥിതം അസ്ഥിര UBaseType_t uxInterruptNesting = 0;
BaseType_t xPortCheckIfInISR(ശൂന്യം)
{
    തിരികെ uxInterruptNesting;
}

#എങ്കിൽ CONFIG_COMPILER_OPTIMIZATION_DEBUG
#define BACKTRACE_PC_ARRAY_SIZE_DUMMY 1
സ്ഥിതം ശൂന്യം load_libgcc(ശൂന്യം)
{
    ശൂന്യം *array[BACKTRACE_PC_ARRAY_SIZE_DUMMY];
    size_t size = backtrace(array, BACKTRACE_PC_ARRAY_SIZE_DUMMY);
    പ്രതിജ്ഞാ(size == 1);
}

സ്ഥിതം ശൂന്യം segfault_handler(പൂർണ്ണസംഖ്യ sig)
{
    ശൂന്യം *array[BACKTRACE_PC_ARRAY_SIZE];
    size_t size = backtrace(array, BACKTRACE_PC_ARRAY_SIZE);
    ssize_t ignore __attribute__((unused));
    ignore = ലിഖ(STDERR_FILENO, ON_SEGFAULT_MESSAGE, വലുപ്പം(ON_SEGFAULT_MESSAGE));
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    _exit(1);
}

സ്ഥിതം ശൂന്യം abort_handler(പൂർണ്ണസംഖ്യ sig)
{
    // we need a raw file write here because other functions are not async-signal-safe
    ssize_t ignore __attribute__((unused));
    ignore = ലിഖ(STDERR_FILENO, ON_ABORT_MESSAGE, വലുപ്പം(ON_ABORT_MESSAGE));
    _exit(1);
}
#endif // CONFIG_COMPILER_OPTIMIZATION_DEBUG

/*-----------------------------------------------------------
* Main FreeRTOS task
*-----------------------------------------------------------*/
ബാഹ്യ ശൂന്യം app_main(ശൂന്യം);
സ്ഥിതം ശൂന്യം main_task(ശൂന്യം *args)
{
    (ശൂന്യം)args;
    ESP_LOGI(MAIN_TAG, "Started on CPU%d", (പൂർണ്ണസംഖ്യ)xPortGetCoreID());

    ESP_LOGI(MAIN_TAG, "Calling app_main()");

    app_main();

    ESP_LOGI(MAIN_TAG, "Returned from app_main()");
    vTaskDelete(ശൂന്യ);
}

ശൂന്യം esp_startup_start_app(ശൂന്യം)
{
    setvbuf(stdout, ശൂന്യ, _IONBF, 0);

#എങ്കിൽ CONFIG_COMPILER_OPTIMIZATION_DEBUG
    load_libgcc();
    sighandler_t sig_res;

    sig_res = സൂചന(SIGSEGV, segfault_handler);
    എങ്കിൽ (sig_res == SIG_ERR) {
        abort();
    }

    sig_res = സൂചന(SIGABRT, abort_handler);
    എങ്കിൽ (sig_res == SIG_ERR) {
        abort();
    }
#endif

    // Create main_task using FreeRTOS API
    ESP_LOGI(LINUX_TAG, "Starting main task.");
    പ്രതിജ്ഞാ(xTaskCreate(&main_task, "main", ESP_TASK_MAIN_അട്ടി, ശൂന്യ, ESP_TASK_MAIN_PRIO, ശൂന്യ) == pdTRUE);

    ESP_LOGI(LINUX_TAG, "Starting scheduler task.");
    vTaskStartScheduler();

    // Should never reach here
    പ്രതിജ്ഞാ(false);
}

/*-----------------------------------------------------------
 * idle and tick hooks
 *-----------------------------------------------------------*/
#എങ്കിൽ (configUSE_IDLE_HOOK > 0)
ശൂന്യം vApplicationIdleHook(ശൂന്യം)
{
}
#endif

#എങ്കിൽ (configUSE_TICK_HOOK > 0)
ശൂന്യം vApplicationTickHook(ശൂന്യം)
{
    ബാഹ്യ ശൂന്യം esp_vApplicationTickHook(ശൂന്യം);
    esp_vApplicationTickHook();
}
#അല്ലെങ്കിൽ
#endif

/*-----------------------------------------------------------
 * Static allocation support
 *-----------------------------------------------------------*/
#എങ്കിൽ (configSUPPORT_STATIC_ALLOCATION == 1)

ശൂന്യം vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   അട്ടി_Type_t **ppx_Idle_Task_അട്ടി_Buffer,
                                   uint32_t *pul_Idle_Task_അട്ടി_Size)
{
    സ്ഥിതം StaticTask_t xIdleTaskTCB;
    സ്ഥിതം അട്ടി_Type_t ux_Idle_Task_അട്ടി[config_MINIMAL_അട്ടി_SIZE];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppx_Idle_Task_അട്ടി_Buffer = ux_Idle_Task_അട്ടി;
    *pul_Idle_Task_അട്ടി_Size = config_MINIMAL_അട്ടി_SIZE;
}

#എങ്കിൽ (configUSE_TIMERS == 1)
അട്ടി_Type_t ux_Timer_Task_അട്ടി[config_TIMER_TASK_അട്ടി_DEPTH];

ശൂന്യം vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    അട്ടി_Type_t **ppx_Timer_Task_അട്ടി_Buffer,
                                    uint32_t *pul_Timer_Task_അട്ടി_Size)
{
    സ്ഥിതം StaticTask_t xTimerTaskTCB;
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppx_Timer_Task_അട്ടി_Buffer = ux_Timer_Task_അട്ടി;
    *pul_Timer_Task_അട്ടി_Size = config_TIMER_TASK_അട്ടി_DEPTH;
}
#endif

#endif // configSUPPORT_STATIC_ALLOCATION

/*-----------------------------------------------------------
 * Stack overflow hook
 *-----------------------------------------------------------*/
ശൂന്യം __attribute__((weak)) v_Application_അട്ടി_Overflow_Hook(TaskHandle_t xTask, അക്ഷരം *pcTaskName)
{
#define ERR_STR1 "***ERROR*** A stack overflow in task "
#define ERR_STR2 " has been detected."
    സ്ഥിരം അക്ഷരം *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    അക്ഷരം buf[വലുപ്പം(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + വലുപ്പം(ERR_STR2) + 1 /* null char */] = {0};

    അക്ഷരം *dest = buf;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < വലുപ്പം(str) / വലുപ്പം(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
	printf("%s\n", buf);
    abort();
}
