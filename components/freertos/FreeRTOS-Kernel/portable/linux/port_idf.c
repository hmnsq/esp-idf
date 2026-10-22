/*
 * SPDX-FileCopyrightText: 2025-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <execinfo.h>
#ଅନ୍ତର୍ଭାଵ <signal.h>
#ଅନ୍ତର୍ଭାଵ <pthread.h>

#ଅନ୍ତର୍ଭାଵ "FreeRTOS.h"
#ଅନ୍ତର୍ଭାଵ "task.h"
#ଅନ୍ତର୍ଭାଵ "esp_log.h"
#ଅନ୍ତର୍ଭାଵ "utils/linux_port_utils.h"

#define BACKTRACE_PC_ARRAY_SIZE 20
#define FREERTOS_SIM_BACKTRACE_SIZE 16
#define ON_SEGFAULT_MESSAGE "ERROR: Segmentation Fault, here's your backtrace:\n"
#define ON_ABORT_MESSAGE "ERROR: Aborted\n"

ସ୍ଥିର ଶୂନ୍ୟ linux_port_backtrace_handler(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sig)
{
    /* All calls here must be async-signal-safe (no stdio, no malloc).
     * write() and backtrace_symbols_fd() write directly to the fd. */
    ଶୂନ୍ୟ *buffer[FREERTOS_SIM_BACKTRACE_SIZE];
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା nptrs = backtrace(buffer, FREERTOS_SIM_BACKTRACE_SIZE);

    ଅଚଳ ଅକ୍ଷର *name = pcTaskGetName(ଖାଲି); /* simple pointer dereference, no lock */
    ssize_t ignore __attribute__((unused));
    ignore = ଲିଖ(STDERR_FILENO, "=== Backtrace for task: ", 24);
    ଯଦି (name) {
        size_t len = 0;
        ଯେତେବେଳେ (name[len] != '\0') {
            len++;
        }
        ignore = ଲିଖ(STDERR_FILENO, name, len);
    }
    ignore = ଲିଖ(STDERR_FILENO, " ===\n", 5);
    backtrace_symbols_fd(buffer, nptrs, STDERR_FILENO);
}

ଶୂନ୍ୟ linux_port_setup_backtrace_signal(ଶୂନ୍ୟ)
{
    ସଂରଚନା sigaction sa;
    sa.sa_handler = linux_port_backtrace_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, ଖାଲି);
}

ଶୂନ୍ୟ linux_port_print_backtrace(ଶୂନ୍ୟ)
{
    pthread_kill(linux_port_get_scheduled_task_pthread(), SIGUSR1);
}

ESP_LOG_ATTR_TAG(LINUX_TAG, "port_idf_linux");
ESP_LOG_ATTR_TAG(MAIN_TAG, "main_task");

#ଯଦି (defined(__APPLE__) && defined(__MACH__))
ପ୍ରକାରବ୍ୟାଖ୍ୟା sig_t sighandler_t;
#endif

ସ୍ଥିର ଅସ୍ଥିର UBaseType_t uxInterruptNesting = 0;
BaseType_t xPortCheckIfInISR(ଶୂନ୍ୟ)
{
    ଫେରନ୍ତୁ uxInterruptNesting;
}

#ଯଦି CONFIG_COMPILER_OPTIMIZATION_DEBUG
#define BACKTRACE_PC_ARRAY_SIZE_DUMMY 1
ସ୍ଥିର ଶୂନ୍ୟ load_libgcc(ଶୂନ୍ୟ)
{
    ଶୂନ୍ୟ *array[BACKTRACE_PC_ARRAY_SIZE_DUMMY];
    size_t size = backtrace(array, BACKTRACE_PC_ARRAY_SIZE_DUMMY);
    ପ୍ରତିଜ୍ଞା(size == 1);
}

ସ୍ଥିର ଶୂନ୍ୟ segfault_handler(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sig)
{
    ଶୂନ୍ୟ *array[BACKTRACE_PC_ARRAY_SIZE];
    size_t size = backtrace(array, BACKTRACE_PC_ARRAY_SIZE);
    ssize_t ignore __attribute__((unused));
    ignore = ଲିଖ(STDERR_FILENO, ON_SEGFAULT_MESSAGE, ଆକାର(ON_SEGFAULT_MESSAGE));
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    _exit(1);
}

ସ୍ଥିର ଶୂନ୍ୟ abort_handler(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sig)
{
    // we need a raw file write here because other functions are not async-signal-safe
    ssize_t ignore __attribute__((unused));
    ignore = ଲିଖ(STDERR_FILENO, ON_ABORT_MESSAGE, ଆକାର(ON_ABORT_MESSAGE));
    _exit(1);
}
#endif // CONFIG_COMPILER_OPTIMIZATION_DEBUG

/*-----------------------------------------------------------
* Main FreeRTOS task
*-----------------------------------------------------------*/
ବାହ୍ୟ ଶୂନ୍ୟ app_main(ଶୂନ୍ୟ);
ସ୍ଥିର ଶୂନ୍ୟ main_task(ଶୂନ୍ୟ *args)
{
    (ଶୂନ୍ୟ)args;
    ESP_LOGI(MAIN_TAG, "Started on CPU%d", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)xPortGetCoreID());

    ESP_LOGI(MAIN_TAG, "Calling app_main()");

    app_main();

    ESP_LOGI(MAIN_TAG, "Returned from app_main()");
    vTaskDelete(ଖାଲି);
}

ଶୂନ୍ୟ esp_startup_start_app(ଶୂନ୍ୟ)
{
    setvbuf(stdout, ଖାଲି, _IONBF, 0);

#ଯଦି CONFIG_COMPILER_OPTIMIZATION_DEBUG
    load_libgcc();
    sighandler_t sig_res;

    sig_res = ସଙ୍କେତ(SIGSEGV, segfault_handler);
    ଯଦି (sig_res == SIG_ERR) {
        abort();
    }

    sig_res = ସଙ୍କେତ(SIGABRT, abort_handler);
    ଯଦି (sig_res == SIG_ERR) {
        abort();
    }
#endif

    // Create main_task using FreeRTOS API
    ESP_LOGI(LINUX_TAG, "Starting main task.");
    ପ୍ରତିଜ୍ଞା(xTaskCreate(&main_task, "main", ESP_TASK_MAIN_ଗଦା, ଖାଲି, ESP_TASK_MAIN_PRIO, ଖାଲି) == pdTRUE);

    ESP_LOGI(LINUX_TAG, "Starting scheduler task.");
    vTaskStartScheduler();

    // Should never reach here
    ପ୍ରତିଜ୍ଞା(false);
}

/*-----------------------------------------------------------
 * idle and tick hooks
 *-----------------------------------------------------------*/
#ଯଦି (configUSE_IDLE_HOOK > 0)
ଶୂନ୍ୟ vApplicationIdleHook(ଶୂନ୍ୟ)
{
}
#endif

#ଯଦି (configUSE_TICK_HOOK > 0)
ଶୂନ୍ୟ vApplicationTickHook(ଶୂନ୍ୟ)
{
    ବାହ୍ୟ ଶୂନ୍ୟ esp_vApplicationTickHook(ଶୂନ୍ୟ);
    esp_vApplicationTickHook();
}
#ଅନ୍ୟଥା
#endif

/*-----------------------------------------------------------
 * Static allocation support
 *-----------------------------------------------------------*/
#ଯଦି (configSUPPORT_STATIC_ALLOCATION == 1)

ଶୂନ୍ୟ vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   ଗଦା_Type_t **ppx_Idle_Task_ଗଦା_Buffer,
                                   uint32_t *pul_Idle_Task_ଗଦା_Size)
{
    ସ୍ଥିର StaticTask_t xIdleTaskTCB;
    ସ୍ଥିର ଗଦା_Type_t ux_Idle_Task_ଗଦା[config_MINIMAL_ଗଦା_SIZE];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppx_Idle_Task_ଗଦା_Buffer = ux_Idle_Task_ଗଦା;
    *pul_Idle_Task_ଗଦା_Size = config_MINIMAL_ଗଦା_SIZE;
}

#ଯଦି (configUSE_TIMERS == 1)
ଗଦା_Type_t ux_Timer_Task_ଗଦା[config_TIMER_TASK_ଗଦା_DEPTH];

ଶୂନ୍ୟ vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    ଗଦା_Type_t **ppx_Timer_Task_ଗଦା_Buffer,
                                    uint32_t *pul_Timer_Task_ଗଦା_Size)
{
    ସ୍ଥିର StaticTask_t xTimerTaskTCB;
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppx_Timer_Task_ଗଦା_Buffer = ux_Timer_Task_ଗଦା;
    *pul_Timer_Task_ଗଦା_Size = config_TIMER_TASK_ଗଦା_DEPTH;
}
#endif

#endif // configSUPPORT_STATIC_ALLOCATION

/*-----------------------------------------------------------
 * Stack overflow hook
 *-----------------------------------------------------------*/
ଶୂନ୍ୟ __attribute__((weak)) v_Application_ଗଦା_Overflow_Hook(TaskHandle_t xTask, ଅକ୍ଷର *pcTaskName)
{
#define ERR_STR1 "***ERROR*** A stack overflow in task "
#define ERR_STR2 " has been detected."
    ଅଚଳ ଅକ୍ଷର *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    ଅକ୍ଷର buf[ଆକାର(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + ଆକାର(ERR_STR2) + 1 /* null char */] = {0};

    ଅକ୍ଷର *dest = buf;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଆକାର(str) / ଆକାର(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
	printf("%s\n", buf);
    abort();
}
