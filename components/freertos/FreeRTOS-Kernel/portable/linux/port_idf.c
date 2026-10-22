/*
 * SPDX-FileCopyrightText: 2025-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <execinfo.h>
#ಅನ್ತರ್ಭಾವ <signal.h>
#ಅನ್ತರ್ಭಾವ <pthread.h>

#ಅನ್ತರ್ಭಾವ "FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "task.h"
#ಅನ್ತರ್ಭಾವ "esp_log.h"
#ಅನ್ತರ್ಭಾವ "utils/linux_port_utils.h"

#define BACKTRACE_PC_ARRAY_SIZE 20
#define FREERTOS_SIM_BACKTRACE_SIZE 16
#define ON_SEGFAULT_MESSAGE "ERROR: Segmentation Fault, here's your backtrace:\n"
#define ON_ABORT_MESSAGE "ERROR: Aborted\n"

ಸ್ಥಿರ ಶೂನ್ಯ linux_port_backtrace_handler(ಪೂರ್ಣಾಙ್ಕ sig)
{
    /* All calls here must be async-signal-safe (no stdio, no malloc).
     * write() and backtrace_symbols_fd() write directly to the fd. */
    ಶೂನ್ಯ *buffer[FREERTOS_SIM_BACKTRACE_SIZE];
    ಪೂರ್ಣಾಙ್ಕ nptrs = backtrace(buffer, FREERTOS_SIM_BACKTRACE_SIZE);

    ಧ್ರುವ ವರ್ಣ *name = pcTaskGetName(ರಿಕ್ತ); /* simple pointer dereference, no lock */
    ssize_t ignore __attribute__((unused));
    ignore = ಲಿಖ(STDERR_FILENO, "=== Backtrace for task: ", 24);
    ಯದಿ (name) {
        size_t len = 0;
        ಯಾವತ್ (name[len] != '\0') {
            len++;
        }
        ignore = ಲಿಖ(STDERR_FILENO, name, len);
    }
    ignore = ಲಿಖ(STDERR_FILENO, " ===\n", 5);
    backtrace_symbols_fd(buffer, nptrs, STDERR_FILENO);
}

ಶೂನ್ಯ linux_port_setup_backtrace_signal(ಶೂನ್ಯ)
{
    ಸಂರಚನಾ sigaction sa;
    sa.sa_handler = linux_port_backtrace_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, ರಿಕ್ತ);
}

ಶೂನ್ಯ linux_port_print_backtrace(ಶೂನ್ಯ)
{
    pthread_kill(linux_port_get_scheduled_task_pthread(), SIGUSR1);
}

ESP_LOG_ATTR_TAG(LINUX_TAG, "port_idf_linux");
ESP_LOG_ATTR_TAG(MAIN_TAG, "main_task");

#ಯದಿ (defined(__APPLE__) && defined(__MACH__))
ಪ್ರಕಾರನಿರ್ದೇಶ sig_t sighandler_t;
#endif

ಸ್ಥಿರ ಚಞ್ಚಲ UBaseType_t uxInterruptNesting = 0;
BaseType_t xPortCheckIfInISR(ಶೂನ್ಯ)
{
    ಪ್ರತ್ಯಾಗಮ uxInterruptNesting;
}

#ಯದಿ CONFIG_COMPILER_OPTIMIZATION_DEBUG
#define BACKTRACE_PC_ARRAY_SIZE_DUMMY 1
ಸ್ಥಿರ ಶೂನ್ಯ load_libgcc(ಶೂನ್ಯ)
{
    ಶೂನ್ಯ *array[BACKTRACE_PC_ARRAY_SIZE_DUMMY];
    size_t size = backtrace(array, BACKTRACE_PC_ARRAY_SIZE_DUMMY);
    ಪ್ರತಿಜ್ಞಾ(size == 1);
}

ಸ್ಥಿರ ಶೂನ್ಯ segfault_handler(ಪೂರ್ಣಾಙ್ಕ sig)
{
    ಶೂನ್ಯ *array[BACKTRACE_PC_ARRAY_SIZE];
    size_t size = backtrace(array, BACKTRACE_PC_ARRAY_SIZE);
    ssize_t ignore __attribute__((unused));
    ignore = ಲಿಖ(STDERR_FILENO, ON_SEGFAULT_MESSAGE, ಪರಿಮಾಣ(ON_SEGFAULT_MESSAGE));
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    _exit(1);
}

ಸ್ಥಿರ ಶೂನ್ಯ abort_handler(ಪೂರ್ಣಾಙ್ಕ sig)
{
    // we need a raw file write here because other functions are not async-signal-safe
    ssize_t ignore __attribute__((unused));
    ignore = ಲಿಖ(STDERR_FILENO, ON_ABORT_MESSAGE, ಪರಿಮಾಣ(ON_ABORT_MESSAGE));
    _exit(1);
}
#endif // CONFIG_COMPILER_OPTIMIZATION_DEBUG

/*-----------------------------------------------------------
* Main FreeRTOS task
*-----------------------------------------------------------*/
ಬಾಹ್ಯ ಶೂನ್ಯ app_main(ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ main_task(ಶೂನ್ಯ *args)
{
    (ಶೂನ್ಯ)args;
    ESP_LOGI(MAIN_TAG, "Started on CPU%d", (ಪೂರ್ಣಾಙ್ಕ)xPortGetCoreID());

    ESP_LOGI(MAIN_TAG, "Calling app_main()");

    app_main();

    ESP_LOGI(MAIN_TAG, "Returned from app_main()");
    vTaskDelete(ರಿಕ್ತ);
}

ಶೂನ್ಯ esp_startup_start_app(ಶೂನ್ಯ)
{
    setvbuf(stdout, ರಿಕ್ತ, _IONBF, 0);

#ಯದಿ CONFIG_COMPILER_OPTIMIZATION_DEBUG
    load_libgcc();
    sighandler_t sig_res;

    sig_res = ಸಙ್ಕೇತ(SIGSEGV, segfault_handler);
    ಯದಿ (sig_res == SIG_ERR) {
        abort();
    }

    sig_res = ಸಙ್ಕೇತ(SIGABRT, abort_handler);
    ಯದಿ (sig_res == SIG_ERR) {
        abort();
    }
#endif

    // Create main_task using FreeRTOS API
    ESP_LOGI(LINUX_TAG, "Starting main task.");
    ಪ್ರತಿಜ್ಞಾ(xTaskCreate(&main_task, "main", ESP_TASK_MAIN_ರಾಶಿ, ರಿಕ್ತ, ESP_TASK_MAIN_PRIO, ರಿಕ್ತ) == pdTRUE);

    ESP_LOGI(LINUX_TAG, "Starting scheduler task.");
    vTaskStartScheduler();

    // Should never reach here
    ಪ್ರತಿಜ್ಞಾ(false);
}

/*-----------------------------------------------------------
 * idle and tick hooks
 *-----------------------------------------------------------*/
#ಯದಿ (configUSE_IDLE_HOOK > 0)
ಶೂನ್ಯ vApplicationIdleHook(ಶೂನ್ಯ)
{
}
#endif

#ಯದಿ (configUSE_TICK_HOOK > 0)
ಶೂನ್ಯ vApplicationTickHook(ಶೂನ್ಯ)
{
    ಬಾಹ್ಯ ಶೂನ್ಯ esp_vApplicationTickHook(ಶೂನ್ಯ);
    esp_vApplicationTickHook();
}
#ಅನ್ಯಥಾ
#endif

/*-----------------------------------------------------------
 * Static allocation support
 *-----------------------------------------------------------*/
#ಯದಿ (configSUPPORT_STATIC_ALLOCATION == 1)

ಶೂನ್ಯ vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   ರಾಶಿ_Type_t **ppx_Idle_Task_ರಾಶಿ_Buffer,
                                   uint32_t *pul_Idle_Task_ರಾಶಿ_Size)
{
    ಸ್ಥಿರ StaticTask_t xIdleTaskTCB;
    ಸ್ಥಿರ ರಾಶಿ_Type_t ux_Idle_Task_ರಾಶಿ[config_MINIMAL_ರಾಶಿ_SIZE];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppx_Idle_Task_ರಾಶಿ_Buffer = ux_Idle_Task_ರಾಶಿ;
    *pul_Idle_Task_ರಾಶಿ_Size = config_MINIMAL_ರಾಶಿ_SIZE;
}

#ಯದಿ (configUSE_TIMERS == 1)
ರಾಶಿ_Type_t ux_Timer_Task_ರಾಶಿ[config_TIMER_TASK_ರಾಶಿ_DEPTH];

ಶೂನ್ಯ vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    ರಾಶಿ_Type_t **ppx_Timer_Task_ರಾಶಿ_Buffer,
                                    uint32_t *pul_Timer_Task_ರಾಶಿ_Size)
{
    ಸ್ಥಿರ StaticTask_t xTimerTaskTCB;
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppx_Timer_Task_ರಾಶಿ_Buffer = ux_Timer_Task_ರಾಶಿ;
    *pul_Timer_Task_ರಾಶಿ_Size = config_TIMER_TASK_ರಾಶಿ_DEPTH;
}
#endif

#endif // configSUPPORT_STATIC_ALLOCATION

/*-----------------------------------------------------------
 * Stack overflow hook
 *-----------------------------------------------------------*/
ಶೂನ್ಯ __attribute__((weak)) v_Application_ರಾಶಿ_Overflow_Hook(TaskHandle_t xTask, ವರ್ಣ *pcTaskName)
{
#define ERR_STR1 "***ERROR*** A stack overflow in task "
#define ERR_STR2 " has been detected."
    ಧ್ರುವ ವರ್ಣ *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    ವರ್ಣ buf[ಪರಿಮಾಣ(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + ಪರಿಮಾಣ(ERR_STR2) + 1 /* null char */] = {0};

    ವರ್ಣ *dest = buf;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಪರಿಮಾಣ(str) / ಪರಿಮಾಣ(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
	printf("%s\n", buf);
    abort();
}
