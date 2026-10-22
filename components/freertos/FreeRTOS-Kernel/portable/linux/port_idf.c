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

#ವ್ಯಾಖ್ಯಾನಿಸು BACKTRACE_PC_ARRAY_ಗಾತ್ರ 20
#ವ್ಯಾಖ್ಯಾನಿಸು FREERTOS_SIM_BACKTRACE_ಗಾತ್ರ 16
#ವ್ಯಾಖ್ಯಾನಿಸು ON_SEGFAULT_ಸಂದೇಶ "ERROR: Segmentation Fault, here's your backtrace:\n"
#ವ್ಯಾಖ್ಯಾನಿಸು ON_ABORT_ಸಂದೇಶ "ERROR: Aborted\n"

ಸ್ಥಿರ ಶೂನ್ಯ linux_ಪೋರ್ಟ್_backtrace_handler(ಪೂರ್ಣಾಂಕ sig)
{
    /* All calls here must be async-signal-safe (no stdio, no malloc).
     * write() and backtrace_symbols_fd() write directly to the fd. */
    ಶೂನ್ಯ *ಬಫರ್[FREERTOS_SIM_BACKTRACE_ಗಾತ್ರ];
    ಪೂರ್ಣಾಂಕ nptrs = backtrace(ಬಫರ್, FREERTOS_SIM_BACKTRACE_ಗಾತ್ರ);

    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name = pcTaskGetName(ಖಾಲಿ); /* simple pointer dereference, no lock */
    ssize_t ignore __attribute__((ಬಳಸದ));
    ignore = ಲಿಖ(STDERR_FILENO, "=== Backtrace for task: ", 24);
    ಆದರೆ (name) {
        size_t len = 0;
        ಆಗ (name[len] != '\0') {
            len++;
        }
        ignore = ಲಿಖ(STDERR_FILENO, name, len);
    }
    ignore = ಲಿಖ(STDERR_FILENO, " ===\n", 5);
    backtrace_symbols_fd(ಬಫರ್, nptrs, STDERR_FILENO);
}

ಶೂನ್ಯ linux_ಪೋರ್ಟ್_ಸಿದ್ಧತೆ_backtrace_signal(ಶೂನ್ಯ)
{
    ರಚನೆ sigaction sa;
    sa.sa_handler = linux_ಪೋರ್ಟ್_backtrace_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, ಖಾಲಿ);
}

ಶೂನ್ಯ linux_ಪೋರ್ಟ್_print_backtrace(ಶೂನ್ಯ)
{
    pthread_kill(linux_ಪೋರ್ಟ್_get_scheduled_task_pthread(), SIGUSR1);
}

ESP_LOG_ATTR_ಟ್ಯಾಗ್(LINUX_ಟ್ಯಾಗ್, "port_idf_linux");
ESP_LOG_ATTR_ಟ್ಯಾಗ್(MAIN_ಟ್ಯಾಗ್, "main_task");

#ಆದರೆ (defined(__APPLE__) && defined(__MACH__))
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ sig_t sighandler_t;
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಅಸ್ಥಿರ UBaseType_t ux_ಅಡಚಣೆ_Nesting = 0;
ತಳ_Type_t x_ಪೋರ್ಟ್_Check_If_ಒಳ_ISR(ಶೂನ್ಯ)
{
    ಹಿಂತಿರುಗಿ ux_ಅಡಚಣೆ_Nesting;
}

#ಆದರೆ CONFIG_COMPILER_OPTIMIZATION_DEBUG
#ವ್ಯಾಖ್ಯಾನಿಸು BACKTRACE_PC_ARRAY_ಗಾತ್ರ_DUMMY 1
ಸ್ಥಿರ ಶೂನ್ಯ ತುಂಬು_libgcc(ಶೂನ್ಯ)
{
    ಶೂನ್ಯ *array[BACKTRACE_PC_ARRAY_ಗಾತ್ರ_DUMMY];
    size_t ಗಾತ್ರ_2 = backtrace(array, BACKTRACE_PC_ARRAY_ಗಾತ್ರ_DUMMY);
    ಪ್ರತಿಜ್ಞಾ(ಗಾತ್ರ_2 == 1);
}

ಸ್ಥಿರ ಶೂನ್ಯ segfault_handler(ಪೂರ್ಣಾಂಕ sig)
{
    ಶೂನ್ಯ *array[BACKTRACE_PC_ARRAY_ಗಾತ್ರ];
    size_t ಗಾತ್ರ_2 = backtrace(array, BACKTRACE_PC_ARRAY_ಗಾತ್ರ);
    ssize_t ignore __attribute__((ಬಳಸದ));
    ignore = ಲಿಖ(STDERR_FILENO, ON_SEGFAULT_ಸಂದೇಶ, ಗಾತ್ರ(ON_SEGFAULT_ಸಂದೇಶ));
    backtrace_symbols_fd(array, ಗಾತ್ರ_2, STDERR_FILENO);
    _exit(1);
}

ಸ್ಥಿರ ಶೂನ್ಯ abort_handler(ಪೂರ್ಣಾಂಕ sig)
{
    // we need a raw file write here because other functions are not async-signal-safe
    ssize_t ignore __attribute__((ಬಳಸದ));
    ignore = ಲಿಖ(STDERR_FILENO, ON_ABORT_ಸಂದೇಶ, ಗಾತ್ರ(ON_ABORT_ಸಂದೇಶ));
    _exit(1);
}
#ಆದರೆ_ಅಂತ್ಯ // CONFIG_COMPILER_OPTIMIZATION_DEBUG

/*-----------------------------------------------------------
* Main FreeRTOS task
*-----------------------------------------------------------*/
ಬಾಹ್ಯ ಶೂನ್ಯ app_main(ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ main_task(ಶೂನ್ಯ *ವಾದಗಳು)
{
    (ಶೂನ್ಯ)ವಾದಗಳು;
    ESP_LOGI(MAIN_ಟ್ಯಾಗ್, "Started on CPU%d", (ಪೂರ್ಣಾಂಕ)x_ಪೋರ್ಟ್_Get_Core_ID());

    ESP_LOGI(MAIN_ಟ್ಯಾಗ್, "Calling app_main()");

    app_main();

    ESP_LOGI(MAIN_ಟ್ಯಾಗ್, "Returned from app_main()");
    vTaskDelete(ಖಾಲಿ);
}

ಶೂನ್ಯ esp_startup_ಆರಂಭ_app(ಶೂನ್ಯ)
{
    setvbuf(stdout, ಖಾಲಿ, _IONBF, 0);

#ಆದರೆ CONFIG_COMPILER_OPTIMIZATION_DEBUG
    ತುಂಬು_libgcc();
    sighandler_t sig_res;

    sig_res = ಸಂಕೇತ(SIGSEGV, segfault_handler);
    ಆದರೆ (sig_res == SIG_ERR) {
        abort();
    }

    sig_res = ಸಂಕೇತ(SIGABRT, abort_handler);
    ಆದರೆ (sig_res == SIG_ERR) {
        abort();
    }
#ಆದರೆ_ಅಂತ್ಯ

    // Create main_task using FreeRTOS API
    ESP_LOGI(LINUX_ಟ್ಯಾಗ್, "Starting main task.");
    ಪ್ರತಿಜ್ಞಾ(xTaskCreate(&main_task, "main", ESP_TASK_MAIN_ರಾಶಿ, ಖಾಲಿ, ESP_TASK_MAIN_PRIO, ಖಾಲಿ) == pdTRUE);

    ESP_LOGI(LINUX_ಟ್ಯಾಗ್, "Starting scheduler task.");
    v_Task_ಆರಂಭ_Scheduler();

    // Should never reach here
    ಪ್ರತಿಜ್ಞಾ(false);
}

/*-----------------------------------------------------------
 * idle and tick hooks
 *-----------------------------------------------------------*/
#ಆದರೆ (configUSE_IDLE_HOOK > 0)
ಶೂನ್ಯ vApplicationIdleHook(ಶೂನ್ಯ)
{
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ (configUSE_TICK_HOOK > 0)
ಶೂನ್ಯ vApplicationTickHook(ಶೂನ್ಯ)
{
    ಬಾಹ್ಯ ಶೂನ್ಯ esp_vApplicationTickHook(ಶೂನ್ಯ);
    esp_vApplicationTickHook();
}
#ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ

/*-----------------------------------------------------------
 * Static allocation support
 *-----------------------------------------------------------*/
#ಆದರೆ (config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1)

ಶೂನ್ಯ v_Application_Get_Idle_Task_ಸ್ಮರಣೆ(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   ರಾಶಿ_Type_t **ppx_Idle_Task_ರಾಶಿ_ಬಫರ್,
                                   uint32_t *pul_Idle_Task_ರಾಶಿ_ಗಾತ್ರ)
{
    ಸ್ಥಿರ StaticTask_t xIdleTaskTCB;
    ಸ್ಥಿರ ರಾಶಿ_Type_t ux_Idle_Task_ರಾಶಿ[config_ಕನಿಷ್ಠತಮ_ರಾಶಿ_ಗಾತ್ರ];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppx_Idle_Task_ರಾಶಿ_ಬಫರ್ = ux_Idle_Task_ರಾಶಿ;
    *pul_Idle_Task_ರಾಶಿ_ಗಾತ್ರ = config_ಕನಿಷ್ಠತಮ_ರಾಶಿ_ಗಾತ್ರ;
}

#ಆದರೆ (configUSE_TIMERS == 1)
ರಾಶಿ_Type_t ux_Timer_Task_ರಾಶಿ[config_TIMER_TASK_ರಾಶಿ_ಆಳ];

ಶೂನ್ಯ v_Application_Get_Timer_Task_ಸ್ಮರಣೆ(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    ರಾಶಿ_Type_t **ppx_Timer_Task_ರಾಶಿ_ಬಫರ್,
                                    uint32_t *pul_Timer_Task_ರಾಶಿ_ಗಾತ್ರ)
{
    ಸ್ಥಿರ StaticTask_t xTimerTaskTCB;
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppx_Timer_Task_ರಾಶಿ_ಬಫರ್ = ux_Timer_Task_ರಾಶಿ;
    *pul_Timer_Task_ರಾಶಿ_ಗಾತ್ರ = config_TIMER_TASK_ರಾಶಿ_ಆಳ;
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ // configSUPPORT_STATIC_ALLOCATION

/*-----------------------------------------------------------
 * Stack overflow hook
 *-----------------------------------------------------------*/
ಶೂನ್ಯ __attribute__((weak)) v_Application_ರಾಶಿ_ಅತಿಪ್ರವಾಹ_Hook(TaskHandle_t xTask, ಅಕ್ಷರ *pcTaskName)
{
#ವ್ಯಾಖ್ಯಾನಿಸು ERR_STR1 "***ERROR*** A stack overflow in task "
#ವ್ಯಾಖ್ಯಾನಿಸು ERR_STR2 " has been detected."
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    ಅಕ್ಷರ buf[ಗಾತ್ರ(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + ಗಾತ್ರ(ERR_STR2) + 1 /* null char */] = {0};

    ಅಕ್ಷರ *dest = buf;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಗಾತ್ರ(str) / ಗಾತ್ರ(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
	ಬರೆ("%s\n", buf);
    abort();
}
