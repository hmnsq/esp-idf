/*
 * SPDX-FileCopyrightText: 2025-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#অন্তর্ভাব <string.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <assert.h>
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <execinfo.h>
#অন্তর্ভাব <signal.h>
#অন্তর্ভাব <pthread.h>

#অন্তর্ভাব "FreeRTOS.h"
#অন্তর্ভাব "task.h"
#অন্তর্ভাব "esp_log.h"
#অন্তর্ভাব "utils/linux_port_utils.h"

#define BACKTRACE_PC_ARRAY_আকার 20
#define FREERTOS_SIM_BACKTRACE_আকার 16
#define ON_SEGFAULT_MESSAGE "ERROR: Segmentation Fault, here's your backtrace:\n"
#define ON_ABORT_MESSAGE "ERROR: Aborted\n"

স্থির শূন্য linux_port_backtrace_handler(পূর্ণসংখ্যা sig)
{
    /* All calls here must be async-signal-safe (no stdio, no malloc).
     * write() and backtrace_symbols_fd() write directly to the fd. */
    শূন্য *বাফার[FREERTOS_SIM_BACKTRACE_আকার];
    পূর্ণসংখ্যা nptrs = backtrace(বাফার, FREERTOS_SIM_BACKTRACE_আকার);

    ধ্রুবক অক্ষর *name = pcTaskGetName(ফাঁকা); /* simple pointer dereference, no lock */
    ssize_t ignore __attribute__((unused));
    ignore = লিখ(STDERR_FILENO, "=== Backtrace for task: ", 24);
    যদি (name) {
        size_t len = 0;
        যখন (name[len] != '\0') {
            len++;
        }
        ignore = লিখ(STDERR_FILENO, name, len);
    }
    ignore = লিখ(STDERR_FILENO, " ===\n", 5);
    backtrace_symbols_fd(বাফার, nptrs, STDERR_FILENO);
}

শূন্য linux_port_setup_backtrace_signal(শূন্য)
{
    কাঠামো sigaction sa;
    sa.sa_handler = linux_port_backtrace_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, ফাঁকা);
}

শূন্য linux_port_print_backtrace(শূন্য)
{
    pthread_kill(linux_port_get_scheduled_task_pthread(), SIGUSR1);
}

ESP_LOG_ATTR_TAG(LINUX_TAG, "port_idf_linux");
ESP_LOG_ATTR_TAG(MAIN_TAG, "main_task");

#যদি (defined(__APPLE__) && defined(__MACH__))
প্রকারলেখ sig_t sighandler_t;
#endif

স্থির অস্থির UBaseType_t uxInterruptNesting = 0;
BaseType_t xPortCheckIfInISR(শূন্য)
{
    ফেরত uxInterruptNesting;
}

#যদি CONFIG_COMPILER_OPTIMIZATION_DEBUG
#define BACKTRACE_PC_ARRAY_আকার_DUMMY 1
স্থির শূন্য load_libgcc(শূন্য)
{
    শূন্য *array[BACKTRACE_PC_ARRAY_আকার_DUMMY];
    size_t আকার_2 = backtrace(array, BACKTRACE_PC_ARRAY_আকার_DUMMY);
    প্রতিজ্ঞা(আকার_2 == 1);
}

স্থির শূন্য segfault_handler(পূর্ণসংখ্যা sig)
{
    শূন্য *array[BACKTRACE_PC_ARRAY_আকার];
    size_t আকার_2 = backtrace(array, BACKTRACE_PC_ARRAY_আকার);
    ssize_t ignore __attribute__((unused));
    ignore = লিখ(STDERR_FILENO, ON_SEGFAULT_MESSAGE, আকার(ON_SEGFAULT_MESSAGE));
    backtrace_symbols_fd(array, আকার_2, STDERR_FILENO);
    _exit(1);
}

স্থির শূন্য abort_handler(পূর্ণসংখ্যা sig)
{
    // we need a raw file write here because other functions are not async-signal-safe
    ssize_t ignore __attribute__((unused));
    ignore = লিখ(STDERR_FILENO, ON_ABORT_MESSAGE, আকার(ON_ABORT_MESSAGE));
    _exit(1);
}
#endif // CONFIG_COMPILER_OPTIMIZATION_DEBUG

/*-----------------------------------------------------------
* Main FreeRTOS task
*-----------------------------------------------------------*/
বাহ্যিক শূন্য app_main(শূন্য);
স্থির শূন্য main_task(শূন্য *args)
{
    (শূন্য)args;
    ESP_LOGI(MAIN_TAG, "Started on CPU%d", (পূর্ণসংখ্যা)xPortGetCoreID());

    ESP_LOGI(MAIN_TAG, "Calling app_main()");

    app_main();

    ESP_LOGI(MAIN_TAG, "Returned from app_main()");
    vTaskDelete(ফাঁকা);
}

শূন্য esp_startup_start_app(শূন্য)
{
    setvbuf(stdout, ফাঁকা, _IONBF, 0);

#যদি CONFIG_COMPILER_OPTIMIZATION_DEBUG
    load_libgcc();
    sighandler_t sig_res;

    sig_res = সংকেত(SIGSEGV, segfault_handler);
    যদি (sig_res == SIG_ERR) {
        abort();
    }

    sig_res = সংকেত(SIGABRT, abort_handler);
    যদি (sig_res == SIG_ERR) {
        abort();
    }
#endif

    // Create main_task using FreeRTOS API
    ESP_LOGI(LINUX_TAG, "Starting main task.");
    প্রতিজ্ঞা(xTaskCreate(&main_task, "main", ESP_TASK_MAIN_স্তূপ, ফাঁকা, ESP_TASK_MAIN_PRIO, ফাঁকা) == pdTRUE);

    ESP_LOGI(LINUX_TAG, "Starting scheduler task.");
    vTaskStartScheduler();

    // Should never reach here
    প্রতিজ্ঞা(false);
}

/*-----------------------------------------------------------
 * idle and tick hooks
 *-----------------------------------------------------------*/
#যদি (configUSE_IDLE_HOOK > 0)
শূন্য vApplicationIdleHook(শূন্য)
{
}
#endif

#যদি (configUSE_TICK_HOOK > 0)
শূন্য vApplicationTickHook(শূন্য)
{
    বাহ্যিক শূন্য esp_vApplicationTickHook(শূন্য);
    esp_vApplicationTickHook();
}
#অন্যথা
#endif

/*-----------------------------------------------------------
 * Static allocation support
 *-----------------------------------------------------------*/
#যদি (configSUPPORT_STATIC_ALLOCATION == 1)

শূন্য vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   স্তূপ_Type_t **ppx_Idle_Task_স্তূপ_বাফার,
                                   uint32_t *pul_Idle_Task_স্তূপ_আকার)
{
    স্থির StaticTask_t xIdleTaskTCB;
    স্থির স্তূপ_Type_t ux_Idle_Task_স্তূপ[config_MINIMAL_স্তূপ_আকার];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppx_Idle_Task_স্তূপ_বাফার = ux_Idle_Task_স্তূপ;
    *pul_Idle_Task_স্তূপ_আকার = config_MINIMAL_স্তূপ_আকার;
}

#যদি (configUSE_TIMERS == 1)
স্তূপ_Type_t ux_Timer_Task_স্তূপ[config_TIMER_TASK_স্তূপ_DEPTH];

শূন্য vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    স্তূপ_Type_t **ppx_Timer_Task_স্তূপ_বাফার,
                                    uint32_t *pul_Timer_Task_স্তূপ_আকার)
{
    স্থির StaticTask_t xTimerTaskTCB;
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppx_Timer_Task_স্তূপ_বাফার = ux_Timer_Task_স্তূপ;
    *pul_Timer_Task_স্তূপ_আকার = config_TIMER_TASK_স্তূপ_DEPTH;
}
#endif

#endif // configSUPPORT_STATIC_ALLOCATION

/*-----------------------------------------------------------
 * Stack overflow hook
 *-----------------------------------------------------------*/
শূন্য __attribute__((weak)) v_Application_স্তূপ_Overflow_Hook(TaskHandle_t xTask, অক্ষর *pcTaskName)
{
#define ERR_STR1 "***ERROR*** A stack overflow in task "
#define ERR_STR2 " has been detected."
    ধ্রুবক অক্ষর *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    অক্ষর buf[আকার(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + আকার(ERR_STR2) + 1 /* null char */] = {0};

    অক্ষর *dest = buf;
    জন্য (পূর্ণসংখ্যা i = 0; i < আকার(str) / আকার(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
	printf("%s\n", buf);
    abort();
}
