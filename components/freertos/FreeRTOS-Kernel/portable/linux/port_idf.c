/*
 * SPDX-FileCopyrightText: 2025-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <assert.h>
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <execinfo.h>
#અન્તર્ભાવ <signal.h>
#અન્તર્ભાવ <pthread.h>

#અન્તર્ભાવ "FreeRTOS.h"
#અન્તર્ભાવ "task.h"
#અન્તર્ભાવ "esp_log.h"
#અન્તર્ભાવ "utils/linux_port_utils.h"

#define BACKTRACE_PC_ARRAY_SIZE 20
#define FREERTOS_SIM_BACKTRACE_SIZE 16
#define ON_SEGFAULT_MESSAGE "ERROR: Segmentation Fault, here's your backtrace:\n"
#define ON_ABORT_MESSAGE "ERROR: Aborted\n"

સ્થિર શૂન્ય linux_port_backtrace_handler(પૂર્ણાંક sig)
{
    /* All calls here must be async-signal-safe (no stdio, no malloc).
     * write() and backtrace_symbols_fd() write directly to the fd. */
    શૂન્ય *buffer[FREERTOS_SIM_BACKTRACE_SIZE];
    પૂર્ણાંક nptrs = backtrace(buffer, FREERTOS_SIM_BACKTRACE_SIZE);

    અચલ અક્ષર *name = pcTaskGetName(ખાલી); /* simple pointer dereference, no lock */
    ssize_t ignore __attribute__((unused));
    ignore = લિખ(STDERR_FILENO, "=== Backtrace for task: ", 24);
    જો (name) {
        size_t len = 0;
        જ્યારે (name[len] != '\0') {
            len++;
        }
        ignore = લિખ(STDERR_FILENO, name, len);
    }
    ignore = લિખ(STDERR_FILENO, " ===\n", 5);
    backtrace_symbols_fd(buffer, nptrs, STDERR_FILENO);
}

શૂન્ય linux_port_setup_backtrace_signal(શૂન્ય)
{
    સંરચના sigaction sa;
    sa.sa_handler = linux_port_backtrace_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, ખાલી);
}

શૂન્ય linux_port_print_backtrace(શૂન્ય)
{
    pthread_kill(linux_port_get_scheduled_task_pthread(), SIGUSR1);
}

ESP_LOG_ATTR_TAG(LINUX_TAG, "port_idf_linux");
ESP_LOG_ATTR_TAG(MAIN_TAG, "main_task");

#જો (defined(__APPLE__) && defined(__MACH__))
પ્રકારવ્યાખ્યા sig_t sighandler_t;
#endif

સ્થિર અસ્થિર UBaseType_t uxInterruptNesting = 0;
BaseType_t xPortCheckIfInISR(શૂન્ય)
{
    પાછા uxInterruptNesting;
}

#જો CONFIG_COMPILER_OPTIMIZATION_DEBUG
#define BACKTRACE_PC_ARRAY_SIZE_DUMMY 1
સ્થિર શૂન્ય load_libgcc(શૂન્ય)
{
    શૂન્ય *array[BACKTRACE_PC_ARRAY_SIZE_DUMMY];
    size_t size = backtrace(array, BACKTRACE_PC_ARRAY_SIZE_DUMMY);
    પ્રતિજ્ઞા(size == 1);
}

સ્થિર શૂન્ય segfault_handler(પૂર્ણાંક sig)
{
    શૂન્ય *array[BACKTRACE_PC_ARRAY_SIZE];
    size_t size = backtrace(array, BACKTRACE_PC_ARRAY_SIZE);
    ssize_t ignore __attribute__((unused));
    ignore = લિખ(STDERR_FILENO, ON_SEGFAULT_MESSAGE, કદ(ON_SEGFAULT_MESSAGE));
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    _exit(1);
}

સ્થિર શૂન્ય abort_handler(પૂર્ણાંક sig)
{
    // we need a raw file write here because other functions are not async-signal-safe
    ssize_t ignore __attribute__((unused));
    ignore = લિખ(STDERR_FILENO, ON_ABORT_MESSAGE, કદ(ON_ABORT_MESSAGE));
    _exit(1);
}
#endif // CONFIG_COMPILER_OPTIMIZATION_DEBUG

/*-----------------------------------------------------------
* Main FreeRTOS task
*-----------------------------------------------------------*/
બાહ્ય શૂન્ય app_main(શૂન્ય);
સ્થિર શૂન્ય main_task(શૂન્ય *args)
{
    (શૂન્ય)args;
    ESP_LOGI(MAIN_TAG, "Started on CPU%d", (પૂર્ણાંક)xPortGetCoreID());

    ESP_LOGI(MAIN_TAG, "Calling app_main()");

    app_main();

    ESP_LOGI(MAIN_TAG, "Returned from app_main()");
    vTaskDelete(ખાલી);
}

શૂન્ય esp_startup_start_app(શૂન્ય)
{
    setvbuf(stdout, ખાલી, _IONBF, 0);

#જો CONFIG_COMPILER_OPTIMIZATION_DEBUG
    load_libgcc();
    sighandler_t sig_res;

    sig_res = સંકેત(SIGSEGV, segfault_handler);
    જો (sig_res == SIG_ERR) {
        abort();
    }

    sig_res = સંકેત(SIGABRT, abort_handler);
    જો (sig_res == SIG_ERR) {
        abort();
    }
#endif

    // Create main_task using FreeRTOS API
    ESP_LOGI(LINUX_TAG, "Starting main task.");
    પ્રતિજ્ઞા(xTaskCreate(&main_task, "main", ESP_TASK_MAIN_ઢગલો, ખાલી, ESP_TASK_MAIN_PRIO, ખાલી) == pdTRUE);

    ESP_LOGI(LINUX_TAG, "Starting scheduler task.");
    vTaskStartScheduler();

    // Should never reach here
    પ્રતિજ્ઞા(false);
}

/*-----------------------------------------------------------
 * idle and tick hooks
 *-----------------------------------------------------------*/
#જો (configUSE_IDLE_HOOK > 0)
શૂન્ય vApplicationIdleHook(શૂન્ય)
{
}
#endif

#જો (configUSE_TICK_HOOK > 0)
શૂન્ય vApplicationTickHook(શૂન્ય)
{
    બાહ્ય શૂન્ય esp_vApplicationTickHook(શૂન્ય);
    esp_vApplicationTickHook();
}
#નહીંતર
#endif

/*-----------------------------------------------------------
 * Static allocation support
 *-----------------------------------------------------------*/
#જો (configSUPPORT_STATIC_ALLOCATION == 1)

શૂન્ય vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   ઢગલો_Type_t **ppx_Idle_Task_ઢગલો_Buffer,
                                   uint32_t *pul_Idle_Task_ઢગલો_Size)
{
    સ્થિર StaticTask_t xIdleTaskTCB;
    સ્થિર ઢગલો_Type_t ux_Idle_Task_ઢગલો[config_MINIMAL_ઢગલો_SIZE];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppx_Idle_Task_ઢગલો_Buffer = ux_Idle_Task_ઢગલો;
    *pul_Idle_Task_ઢગલો_Size = config_MINIMAL_ઢગલો_SIZE;
}

#જો (configUSE_TIMERS == 1)
ઢગલો_Type_t ux_Timer_Task_ઢગલો[config_TIMER_TASK_ઢગલો_DEPTH];

શૂન્ય vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    ઢગલો_Type_t **ppx_Timer_Task_ઢગલો_Buffer,
                                    uint32_t *pul_Timer_Task_ઢગલો_Size)
{
    સ્થિર StaticTask_t xTimerTaskTCB;
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppx_Timer_Task_ઢગલો_Buffer = ux_Timer_Task_ઢગલો;
    *pul_Timer_Task_ઢગલો_Size = config_TIMER_TASK_ઢગલો_DEPTH;
}
#endif

#endif // configSUPPORT_STATIC_ALLOCATION

/*-----------------------------------------------------------
 * Stack overflow hook
 *-----------------------------------------------------------*/
શૂન્ય __attribute__((weak)) v_Application_ઢગલો_Overflow_Hook(TaskHandle_t xTask, અક્ષર *pcTaskName)
{
#define ERR_STR1 "***ERROR*** A stack overflow in task "
#define ERR_STR2 " has been detected."
    અચલ અક્ષર *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    અક્ષર buf[કદ(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + કદ(ERR_STR2) + 1 /* null char */] = {0};

    અક્ષર *dest = buf;
    માટે (પૂર્ણાંક i = 0; i < કદ(str) / કદ(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
	printf("%s\n", buf);
    abort();
}
