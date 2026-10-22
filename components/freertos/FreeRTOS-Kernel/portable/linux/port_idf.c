/*
 * SPDX-FileCopyrightText: 2025-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن <string.h>
#ضمن <stdlib.h>
#ضمن <stdio.h>
#ضمن <assert.h>
#ضمن <unistd.h>
#ضمن <execinfo.h>
#ضمن <signal.h>
#ضمن <pthread.h>

#ضمن "FreeRTOS.h"
#ضمن "task.h"
#ضمن "esp_log.h"
#ضمن "utils/linux_port_utils.h"

#define BACKTRACE_PC_ARRAY_SIZE 20
#define FREERTOS_SIM_BACKTRACE_SIZE 16
#define ON_SEGFAULT_MESSAGE "ERROR: Segmentation Fault, here's your backtrace:\n"
#define ON_ABORT_MESSAGE "ERROR: Aborted\n"

ساكن فراغ linux_port_backtrace_handler(صحيح sig)
{
    /* All calls here must be async-signal-safe (no stdio, no malloc).
     * write() and backtrace_symbols_fd() write directly to the fd. */
    فراغ *buffer[FREERTOS_SIM_BACKTRACE_SIZE];
    صحيح nptrs = backtrace(buffer, FREERTOS_SIM_BACKTRACE_SIZE);

    ثابت حرف *name = pcTaskGetName(عدم); /* simple pointer dereference, no lock */
    ssize_t ignore __attribute__((unused));
    ignore = اكتب(STDERR_FILENO, "=== Backtrace for task: ", 24);
    𐡄𐡍 (name) {
        size_t len = 0;
        طالما (name[len] != '\0') {
            len++;
        }
        ignore = اكتب(STDERR_FILENO, name, len);
    }
    ignore = اكتب(STDERR_FILENO, " ===\n", 5);
    backtrace_symbols_fd(buffer, nptrs, STDERR_FILENO);
}

فراغ linux_port_setup_backtrace_signal(فراغ)
{
    بنية sigaction sa;
    sa.sa_handler = linux_port_backtrace_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, عدم);
}

فراغ linux_port_print_backtrace(فراغ)
{
    pthread_kill(linux_port_get_scheduled_task_pthread(), SIGUSR1);
}

ESP_LOG_ATTR_TAG(LINUX_TAG, "port_idf_linux");
ESP_LOG_ATTR_TAG(MAIN_TAG, "main_task");

#𐡄𐡍 (defined(__APPLE__) && defined(__MACH__))
تعريفنوع sig_t sighandler_t;
#endif

ساكن متطاير UBaseType_t uxInterruptNesting = 0;
BaseType_t xPortCheckIfInISR(فراغ)
{
    أرجع uxInterruptNesting;
}

#𐡄𐡍 CONFIG_COMPILER_OPTIMIZATION_DEBUG
#define BACKTRACE_PC_ARRAY_SIZE_DUMMY 1
ساكن فراغ load_libgcc(فراغ)
{
    فراغ *array[BACKTRACE_PC_ARRAY_SIZE_DUMMY];
    size_t size = backtrace(array, BACKTRACE_PC_ARRAY_SIZE_DUMMY);
    أكد(size == 1);
}

ساكن فراغ segfault_handler(صحيح sig)
{
    فراغ *array[BACKTRACE_PC_ARRAY_SIZE];
    size_t size = backtrace(array, BACKTRACE_PC_ARRAY_SIZE);
    ssize_t ignore __attribute__((unused));
    ignore = اكتب(STDERR_FILENO, ON_SEGFAULT_MESSAGE, حجم(ON_SEGFAULT_MESSAGE));
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    _exit(1);
}

ساكن فراغ abort_handler(صحيح sig)
{
    // we need a raw file write here because other functions are not async-signal-safe
    ssize_t ignore __attribute__((unused));
    ignore = اكتب(STDERR_FILENO, ON_ABORT_MESSAGE, حجم(ON_ABORT_MESSAGE));
    _exit(1);
}
#endif // CONFIG_COMPILER_OPTIMIZATION_DEBUG

/*-----------------------------------------------------------
* Main FreeRTOS task
*-----------------------------------------------------------*/
خارجي فراغ app_main(فراغ);
ساكن فراغ main_task(فراغ *args)
{
    (فراغ)args;
    ESP_LOGI(MAIN_TAG, "Started on CPU%d", (صحيح)xPortGetCoreID());

    ESP_LOGI(MAIN_TAG, "Calling app_main()");

    app_main();

    ESP_LOGI(MAIN_TAG, "Returned from app_main()");
    vTaskDelete(عدم);
}

فراغ esp_startup_start_app(فراغ)
{
    setvbuf(stdout, عدم, _IONBF, 0);

#𐡄𐡍 CONFIG_COMPILER_OPTIMIZATION_DEBUG
    load_libgcc();
    sighandler_t sig_res;

    sig_res = إشارة(SIGSEGV, segfault_handler);
    𐡄𐡍 (sig_res == SIG_ERR) {
        abort();
    }

    sig_res = إشارة(SIGABRT, abort_handler);
    𐡄𐡍 (sig_res == SIG_ERR) {
        abort();
    }
#endif

    // Create main_task using FreeRTOS API
    ESP_LOGI(LINUX_TAG, "Starting main task.");
    أكد(xTaskCreate(&main_task, "main", ESP_TASK_MAIN_STACK, عدم, ESP_TASK_MAIN_PRIO, عدم) == pdTRUE);

    ESP_LOGI(LINUX_TAG, "Starting scheduler task.");
    vTaskStartScheduler();

    // Should never reach here
    أكد(false);
}

/*-----------------------------------------------------------
 * idle and tick hooks
 *-----------------------------------------------------------*/
#𐡄𐡍 (configUSE_IDLE_HOOK > 0)
فراغ vApplicationIdleHook(فراغ)
{
}
#endif

#𐡄𐡍 (configUSE_TICK_HOOK > 0)
فراغ vApplicationTickHook(فراغ)
{
    خارجي فراغ esp_vApplicationTickHook(فراغ);
    esp_vApplicationTickHook();
}
#وإلا
#endif

/*-----------------------------------------------------------
 * Static allocation support
 *-----------------------------------------------------------*/
#𐡄𐡍 (configSUPPORT_STATIC_ALLOCATION == 1)

فراغ vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    ساكن StaticTask_t xIdleTaskTCB;
    ساكن StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

#𐡄𐡍 (configUSE_TIMERS == 1)
StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

فراغ vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    ساكن StaticTask_t xTimerTaskTCB;
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
#endif

#endif // configSUPPORT_STATIC_ALLOCATION

/*-----------------------------------------------------------
 * Stack overflow hook
 *-----------------------------------------------------------*/
فراغ __attribute__((weak)) vApplicationStackOverflowHook(TaskHandle_t xTask, حرف *pcTaskName)
{
#define ERR_STR1 "***ERROR*** A stack overflow in task "
#define ERR_STR2 " has been detected."
    ثابت حرف *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    حرف buf[حجم(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + حجم(ERR_STR2) + 1 /* null char */] = {0};

    حرف *dest = buf;
    لكل (صحيح i = 0; i < حجم(str) / حجم(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
	printf("%s\n", buf);
    abort();
}
