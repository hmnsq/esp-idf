/*
 * SPDX-FileCopyrightText: 2025-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#شامل <string.h>
#شامل <stdlib.h>
#شامل <stdio.h>
#شامل <assert.h>
#شامل <unistd.h>
#شامل <execinfo.h>
#شامل <signal.h>
#شامل <pthread.h>

#شامل "FreeRTOS.h"
#شامل "task.h"
#شامل "esp_log.h"
#شامل "utils/linux_port_utils.h"

#define BACKTRACE_PC_ARRAY_SIZE 20
#define FREERTOS_SIM_BACKTRACE_SIZE 16
#define ON_SEGFAULT_MESSAGE "ERROR: Segmentation Fault, here's your backtrace:\n"
#define ON_ABORT_MESSAGE "ERROR: Aborted\n"

ایستا تهی linux_port_backtrace_handler(صحیح sig)
{
    /* All calls here must be async-signal-safe (no stdio, no malloc).
     * write() and backtrace_symbols_fd() write directly to the fd. */
    تهی *buffer[FREERTOS_SIM_BACKTRACE_SIZE];
    صحیح nptrs = backtrace(buffer, FREERTOS_SIM_BACKTRACE_SIZE);

    ثابت نویسه *name = pcTaskGetName(پوچ); /* simple pointer dereference, no lock */
    ssize_t ignore __attribute__((unused));
    ignore = اكتب(STDERR_FILENO, "=== Backtrace for task: ", 24);
    اگر (name) {
        size_t len = 0;
        وقتی (name[len] != '\0') {
            len++;
        }
        ignore = اكتب(STDERR_FILENO, name, len);
    }
    ignore = اكتب(STDERR_FILENO, " ===\n", 5);
    backtrace_symbols_fd(buffer, nptrs, STDERR_FILENO);
}

تهی linux_port_setup_backtrace_signal(تهی)
{
    ساختار sigaction sa;
    sa.sa_handler = linux_port_backtrace_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, پوچ);
}

تهی linux_port_print_backtrace(تهی)
{
    pthread_kill(linux_port_get_scheduled_task_pthread(), SIGUSR1);
}

ESP_LOG_ATTR_TAG(LINUX_TAG, "port_idf_linux");
ESP_LOG_ATTR_TAG(MAIN_TAG, "main_task");

#اگر (defined(__APPLE__) && defined(__MACH__))
تعریفنوع sig_t sighandler_t;
#endif

ایستا فرار UBaseType_t uxInterruptNesting = 0;
BaseType_t xPortCheckIfInISR(تهی)
{
    بازگشت uxInterruptNesting;
}

#اگر CONFIG_COMPILER_OPTIMIZATION_DEBUG
#define BACKTRACE_PC_ARRAY_SIZE_DUMMY 1
ایستا تهی load_libgcc(تهی)
{
    تهی *array[BACKTRACE_PC_ARRAY_SIZE_DUMMY];
    size_t size = backtrace(array, BACKTRACE_PC_ARRAY_SIZE_DUMMY);
    أكد(size == 1);
}

ایستا تهی segfault_handler(صحیح sig)
{
    تهی *array[BACKTRACE_PC_ARRAY_SIZE];
    size_t size = backtrace(array, BACKTRACE_PC_ARRAY_SIZE);
    ssize_t ignore __attribute__((unused));
    ignore = اكتب(STDERR_FILENO, ON_SEGFAULT_MESSAGE, اندازه(ON_SEGFAULT_MESSAGE));
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    _exit(1);
}

ایستا تهی abort_handler(صحیح sig)
{
    // we need a raw file write here because other functions are not async-signal-safe
    ssize_t ignore __attribute__((unused));
    ignore = اكتب(STDERR_FILENO, ON_ABORT_MESSAGE, اندازه(ON_ABORT_MESSAGE));
    _exit(1);
}
#endif // CONFIG_COMPILER_OPTIMIZATION_DEBUG

/*-----------------------------------------------------------
* Main FreeRTOS task
*-----------------------------------------------------------*/
بیرونی تهی app_main(تهی);
ایستا تهی main_task(تهی *args)
{
    (تهی)args;
    ESP_LOGI(MAIN_TAG, "Started on CPU%d", (صحیح)xPortGetCoreID());

    ESP_LOGI(MAIN_TAG, "Calling app_main()");

    app_main();

    ESP_LOGI(MAIN_TAG, "Returned from app_main()");
    vTaskDelete(پوچ);
}

تهی esp_startup_start_app(تهی)
{
    setvbuf(stdout, پوچ, _IONBF, 0);

#اگر CONFIG_COMPILER_OPTIMIZATION_DEBUG
    load_libgcc();
    sighandler_t sig_res;

    sig_res = سیگنال(SIGSEGV, segfault_handler);
    اگر (sig_res == SIG_ERR) {
        abort();
    }

    sig_res = سیگنال(SIGABRT, abort_handler);
    اگر (sig_res == SIG_ERR) {
        abort();
    }
#endif

    // Create main_task using FreeRTOS API
    ESP_LOGI(LINUX_TAG, "Starting main task.");
    أكد(xTaskCreate(&main_task, "main", ESP_TASK_MAIN_پشته, پوچ, ESP_TASK_MAIN_PRIO, پوچ) == pdTRUE);

    ESP_LOGI(LINUX_TAG, "Starting scheduler task.");
    vTaskStartScheduler();

    // Should never reach here
    أكد(false);
}

/*-----------------------------------------------------------
 * idle and tick hooks
 *-----------------------------------------------------------*/
#اگر (configUSE_IDLE_HOOK > 0)
تهی vApplicationIdleHook(تهی)
{
}
#endif

#اگر (configUSE_TICK_HOOK > 0)
تهی vApplicationTickHook(تهی)
{
    بیرونی تهی esp_vApplicationTickHook(تهی);
    esp_vApplicationTickHook();
}
#وگرنه
#endif

/*-----------------------------------------------------------
 * Static allocation support
 *-----------------------------------------------------------*/
#اگر (configSUPPORT_STATIC_ALLOCATION == 1)

تهی vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   پشته_Type_t **ppx_Idle_Task_پشته_Buffer,
                                   uint32_t *pul_Idle_Task_پشته_Size)
{
    ایستا StaticTask_t xIdleTaskTCB;
    ایستا پشته_Type_t ux_Idle_Task_پشته[config_MINIMAL_پشته_SIZE];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppx_Idle_Task_پشته_Buffer = ux_Idle_Task_پشته;
    *pul_Idle_Task_پشته_Size = config_MINIMAL_پشته_SIZE;
}

#اگر (configUSE_TIMERS == 1)
پشته_Type_t ux_Timer_Task_پشته[config_TIMER_TASK_پشته_DEPTH];

تهی vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    پشته_Type_t **ppx_Timer_Task_پشته_Buffer,
                                    uint32_t *pul_Timer_Task_پشته_Size)
{
    ایستا StaticTask_t xTimerTaskTCB;
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppx_Timer_Task_پشته_Buffer = ux_Timer_Task_پشته;
    *pul_Timer_Task_پشته_Size = config_TIMER_TASK_پشته_DEPTH;
}
#endif

#endif // configSUPPORT_STATIC_ALLOCATION

/*-----------------------------------------------------------
 * Stack overflow hook
 *-----------------------------------------------------------*/
تهی __attribute__((weak)) v_Application_پشته_Overflow_Hook(TaskHandle_t xTask, نویسه *pcTaskName)
{
#define ERR_STR1 "***ERROR*** A stack overflow in task "
#define ERR_STR2 " has been detected."
    ثابت نویسه *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    نویسه buf[اندازه(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + اندازه(ERR_STR2) + 1 /* null char */] = {0};

    نویسه *dest = buf;
    برای (صحیح i = 0; i < اندازه(str) / اندازه(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
	printf("%s\n", buf);
    abort();
}
