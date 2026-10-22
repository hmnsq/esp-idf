/*
 * SPDX-FileCopyrightText: 2025-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#அந்தர்பாவ <string.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <assert.h>
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <execinfo.h>
#அந்தர்பாவ <signal.h>
#அந்தர்பாவ <pthread.h>

#அந்தர்பாவ "FreeRTOS.h"
#அந்தர்பாவ "task.h"
#அந்தர்பாவ "esp_log.h"
#அந்தர்பாவ "utils/linux_port_utils.h"

#define BACKTRACE_PC_ARRAY_SIZE 20
#define FREERTOS_SIM_BACKTRACE_SIZE 16
#define ON_SEGFAULT_MESSAGE "ERROR: Segmentation Fault, here's your backtrace:\n"
#define ON_ABORT_MESSAGE "ERROR: Aborted\n"

நிலையான வெறுமை linux_port_backtrace_handler(முழு sig)
{
    /* All calls here must be async-signal-safe (no stdio, no malloc).
     * write() and backtrace_symbols_fd() write directly to the fd. */
    வெறுமை *buffer[FREERTOS_SIM_BACKTRACE_SIZE];
    முழு nptrs = backtrace(buffer, FREERTOS_SIM_BACKTRACE_SIZE);

    மாறிலி எழுத்து *name = pcTaskGetName(வெற்று); /* simple pointer dereference, no lock */
    ssize_t ignore __attribute__((unused));
    ignore = லிக(STDERR_FILENO, "=== Backtrace for task: ", 24);
    எனில் (name) {
        size_t len = 0;
        போது (name[len] != '\0') {
            len++;
        }
        ignore = லிக(STDERR_FILENO, name, len);
    }
    ignore = லிக(STDERR_FILENO, " ===\n", 5);
    backtrace_symbols_fd(buffer, nptrs, STDERR_FILENO);
}

வெறுமை linux_port_setup_backtrace_signal(வெறுமை)
{
    கட்டமைப்பு sigaction sa;
    sa.sa_handler = linux_port_backtrace_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, வெற்று);
}

வெறுமை linux_port_print_backtrace(வெறுமை)
{
    pthread_kill(linux_port_get_scheduled_task_pthread(), SIGUSR1);
}

ESP_LOG_ATTR_TAG(LINUX_TAG, "port_idf_linux");
ESP_LOG_ATTR_TAG(MAIN_TAG, "main_task");

#எனில் (defined(__APPLE__) && defined(__MACH__))
வகைவரை sig_t sighandler_t;
#endif

நிலையான நிலையற்ற UBaseType_t uxInterruptNesting = 0;
BaseType_t xPortCheckIfInISR(வெறுமை)
{
    திரும்பு uxInterruptNesting;
}

#எனில் CONFIG_COMPILER_OPTIMIZATION_DEBUG
#define BACKTRACE_PC_ARRAY_SIZE_DUMMY 1
நிலையான வெறுமை load_libgcc(வெறுமை)
{
    வெறுமை *array[BACKTRACE_PC_ARRAY_SIZE_DUMMY];
    size_t size = backtrace(array, BACKTRACE_PC_ARRAY_SIZE_DUMMY);
    ப்ரதிஜ்ஞா(size == 1);
}

நிலையான வெறுமை segfault_handler(முழு sig)
{
    வெறுமை *array[BACKTRACE_PC_ARRAY_SIZE];
    size_t size = backtrace(array, BACKTRACE_PC_ARRAY_SIZE);
    ssize_t ignore __attribute__((unused));
    ignore = லிக(STDERR_FILENO, ON_SEGFAULT_MESSAGE, அளவு(ON_SEGFAULT_MESSAGE));
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    _exit(1);
}

நிலையான வெறுமை abort_handler(முழு sig)
{
    // we need a raw file write here because other functions are not async-signal-safe
    ssize_t ignore __attribute__((unused));
    ignore = லிக(STDERR_FILENO, ON_ABORT_MESSAGE, அளவு(ON_ABORT_MESSAGE));
    _exit(1);
}
#endif // CONFIG_COMPILER_OPTIMIZATION_DEBUG

/*-----------------------------------------------------------
* Main FreeRTOS task
*-----------------------------------------------------------*/
வெளி வெறுமை app_main(வெறுமை);
நிலையான வெறுமை main_task(வெறுமை *args)
{
    (வெறுமை)args;
    ESP_LOGI(MAIN_TAG, "Started on CPU%d", (முழு)xPortGetCoreID());

    ESP_LOGI(MAIN_TAG, "Calling app_main()");

    app_main();

    ESP_LOGI(MAIN_TAG, "Returned from app_main()");
    vTaskDelete(வெற்று);
}

வெறுமை esp_startup_start_app(வெறுமை)
{
    setvbuf(stdout, வெற்று, _IONBF, 0);

#எனில் CONFIG_COMPILER_OPTIMIZATION_DEBUG
    load_libgcc();
    sighandler_t sig_res;

    sig_res = சமிக்ஞை(SIGSEGV, segfault_handler);
    எனில் (sig_res == SIG_ERR) {
        abort();
    }

    sig_res = சமிக்ஞை(SIGABRT, abort_handler);
    எனில் (sig_res == SIG_ERR) {
        abort();
    }
#endif

    // Create main_task using FreeRTOS API
    ESP_LOGI(LINUX_TAG, "Starting main task.");
    ப்ரதிஜ்ஞா(xTaskCreate(&main_task, "main", ESP_TASK_MAIN_அடுக்கு, வெற்று, ESP_TASK_MAIN_PRIO, வெற்று) == pdTRUE);

    ESP_LOGI(LINUX_TAG, "Starting scheduler task.");
    vTaskStartScheduler();

    // Should never reach here
    ப்ரதிஜ்ஞா(false);
}

/*-----------------------------------------------------------
 * idle and tick hooks
 *-----------------------------------------------------------*/
#எனில் (configUSE_IDLE_HOOK > 0)
வெறுமை vApplicationIdleHook(வெறுமை)
{
}
#endif

#எனில் (configUSE_TICK_HOOK > 0)
வெறுமை vApplicationTickHook(வெறுமை)
{
    வெளி வெறுமை esp_vApplicationTickHook(வெறுமை);
    esp_vApplicationTickHook();
}
#இல்லையெனில்
#endif

/*-----------------------------------------------------------
 * Static allocation support
 *-----------------------------------------------------------*/
#எனில் (configSUPPORT_STATIC_ALLOCATION == 1)

வெறுமை vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   அடுக்கு_Type_t **ppx_Idle_Task_அடுக்கு_Buffer,
                                   uint32_t *pul_Idle_Task_அடுக்கு_Size)
{
    நிலையான StaticTask_t xIdleTaskTCB;
    நிலையான அடுக்கு_Type_t ux_Idle_Task_அடுக்கு[config_MINIMAL_அடுக்கு_SIZE];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppx_Idle_Task_அடுக்கு_Buffer = ux_Idle_Task_அடுக்கு;
    *pul_Idle_Task_அடுக்கு_Size = config_MINIMAL_அடுக்கு_SIZE;
}

#எனில் (configUSE_TIMERS == 1)
அடுக்கு_Type_t ux_Timer_Task_அடுக்கு[config_TIMER_TASK_அடுக்கு_DEPTH];

வெறுமை vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    அடுக்கு_Type_t **ppx_Timer_Task_அடுக்கு_Buffer,
                                    uint32_t *pul_Timer_Task_அடுக்கு_Size)
{
    நிலையான StaticTask_t xTimerTaskTCB;
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppx_Timer_Task_அடுக்கு_Buffer = ux_Timer_Task_அடுக்கு;
    *pul_Timer_Task_அடுக்கு_Size = config_TIMER_TASK_அடுக்கு_DEPTH;
}
#endif

#endif // configSUPPORT_STATIC_ALLOCATION

/*-----------------------------------------------------------
 * Stack overflow hook
 *-----------------------------------------------------------*/
வெறுமை __attribute__((weak)) v_Application_அடுக்கு_Overflow_Hook(TaskHandle_t xTask, எழுத்து *pcTaskName)
{
#define ERR_STR1 "***ERROR*** A stack overflow in task "
#define ERR_STR2 " has been detected."
    மாறிலி எழுத்து *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    எழுத்து buf[அளவு(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + அளவு(ERR_STR2) + 1 /* null char */] = {0};

    எழுத்து *dest = buf;
    க்கு (முழு i = 0; i < அளவு(str) / அளவு(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
	printf("%s\n", buf);
    abort();
}
