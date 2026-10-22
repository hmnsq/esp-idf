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

#تعریف_کړه BACKTRACE_PC_ARRAY_اندازه 20
#تعریف_کړه FREERTOS_SIM_BACKTRACE_اندازه 16
#تعریف_کړه ON_SEGFAULT_پیغام "ERROR: Segmentation Fault, here's your backtrace:\n"
#تعریف_کړه ON_ABORT_پیغام "ERROR: Aborted\n"

ساكن فراغ linux_پورټ_backtrace_handler(صحيح sig)
{
    /* All calls here must be async-signal-safe (no stdio, no malloc).
     * write() and backtrace_symbols_fd() write directly to the fd. */
    فراغ *بفر[FREERTOS_SIM_BACKTRACE_اندازه];
    صحيح nptrs = backtrace(بفر, FREERTOS_SIM_BACKTRACE_اندازه);

    ثابت حرف *name = pcTaskGetName(عدم); /* simple pointer dereference, no lock */
    ssize_t ignore __attribute__((ناکارول_شوی));
    ignore = اكتب(STDERR_FILENO, "=== Backtrace for task: ", 24);
    إذا (name) {
        size_t len = 0;
        طالما (name[len] != '\0') {
            len++;
        }
        ignore = اكتب(STDERR_FILENO, name, len);
    }
    ignore = اكتب(STDERR_FILENO, " ===\n", 5);
    backtrace_symbols_fd(بفر, nptrs, STDERR_FILENO);
}

فراغ linux_پورټ_تیاری_backtrace_signal(فراغ)
{
    بنية sigaction sa;
    sa.sa_handler = linux_پورټ_backtrace_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, عدم);
}

فراغ linux_پورټ_print_backtrace(فراغ)
{
    pthread_kill(linux_پورټ_get_scheduled_task_pthread(), SIGUSR1);
}

ESP_LOG_ATTR_نښان(LINUX_نښان, "port_idf_linux");
ESP_LOG_ATTR_نښان(MAIN_نښان, "main_task");

#إذا (defined(__APPLE__) && defined(__MACH__))
تعريفنوع sig_t sighandler_t;
#د_که_پای

ساكن متطاير UBaseType_t ux_مداخله_Nesting = 0;
بنسټ_Type_t x_پورټ_Check_If_دننه_ISR(فراغ)
{
    أرجع ux_مداخله_Nesting;
}

#إذا CONFIG_COMPILER_OPTIMIZATION_DEBUG
#تعریف_کړه BACKTRACE_PC_ARRAY_اندازه_DUMMY 1
ساكن فراغ بارول_libgcc(فراغ)
{
    فراغ *array[BACKTRACE_PC_ARRAY_اندازه_DUMMY];
    size_t اندازه = backtrace(array, BACKTRACE_PC_ARRAY_اندازه_DUMMY);
    أكد(اندازه == 1);
}

ساكن فراغ segfault_handler(صحيح sig)
{
    فراغ *array[BACKTRACE_PC_ARRAY_اندازه];
    size_t اندازه = backtrace(array, BACKTRACE_PC_ARRAY_اندازه);
    ssize_t ignore __attribute__((ناکارول_شوی));
    ignore = اكتب(STDERR_FILENO, ON_SEGFAULT_پیغام, حجم(ON_SEGFAULT_پیغام));
    backtrace_symbols_fd(array, اندازه, STDERR_FILENO);
    _exit(1);
}

ساكن فراغ abort_handler(صحيح sig)
{
    // we need a raw file write here because other functions are not async-signal-safe
    ssize_t ignore __attribute__((ناکارول_شوی));
    ignore = اكتب(STDERR_FILENO, ON_ABORT_پیغام, حجم(ON_ABORT_پیغام));
    _exit(1);
}
#د_که_پای // CONFIG_COMPILER_OPTIMIZATION_DEBUG

/*-----------------------------------------------------------
* Main FreeRTOS task
*-----------------------------------------------------------*/
خارجي فراغ app_main(فراغ);
ساكن فراغ main_task(فراغ *دلیلونه)
{
    (فراغ)دلیلونه;
    ESP_LOGI(MAIN_نښان, "Started on CPU%d", (صحيح)x_پورټ_Get_Core_ID());

    ESP_LOGI(MAIN_نښان, "Calling app_main()");

    app_main();

    ESP_LOGI(MAIN_نښان, "Returned from app_main()");
    vTaskDelete(عدم);
}

فراغ esp_startup_پیل_app(فراغ)
{
    setvbuf(stdout, عدم, _IONBF, 0);

#إذا CONFIG_COMPILER_OPTIMIZATION_DEBUG
    بارول_libgcc();
    sighandler_t sig_res;

    sig_res = إشارة(SIGSEGV, segfault_handler);
    إذا (sig_res == SIG_ERR) {
        abort();
    }

    sig_res = إشارة(SIGABRT, abort_handler);
    إذا (sig_res == SIG_ERR) {
        abort();
    }
#د_که_پای

    // Create main_task using FreeRTOS API
    ESP_LOGI(LINUX_نښان, "Starting main task.");
    أكد(xTaskCreate(&main_task, "main", ESP_TASK_MAIN_ډېری, عدم, ESP_TASK_MAIN_PRIO, عدم) == pdTRUE);

    ESP_LOGI(LINUX_نښان, "Starting scheduler task.");
    v_Task_پیل_Scheduler();

    // Should never reach here
    أكد(false);
}

/*-----------------------------------------------------------
 * idle and tick hooks
 *-----------------------------------------------------------*/
#إذا (configUSE_IDLE_HOOK > 0)
فراغ vApplicationIdleHook(فراغ)
{
}
#د_که_پای

#إذا (configUSE_TICK_HOOK > 0)
فراغ vApplicationTickHook(فراغ)
{
    خارجي فراغ esp_vApplicationTickHook(فراغ);
    esp_vApplicationTickHook();
}
#وإلا
#د_که_پای

/*-----------------------------------------------------------
 * Static allocation support
 *-----------------------------------------------------------*/
#إذا (config_SUPPORT_STATIC_ویش == 1)

فراغ v_Application_Get_Idle_Task_حافظه(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   ډېری_Type_t **ppx_Idle_Task_ډېری_بفر,
                                   uint32_t *pul_Idle_Task_ډېری_اندازه)
{
    ساكن StaticTask_t xIdleTaskTCB;
    ساكن ډېری_Type_t ux_Idle_Task_ډېری[config_اصغري_ډېری_اندازه];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppx_Idle_Task_ډېری_بفر = ux_Idle_Task_ډېری;
    *pul_Idle_Task_ډېری_اندازه = config_اصغري_ډېری_اندازه;
}

#إذا (configUSE_TIMERS == 1)
ډېری_Type_t ux_Timer_Task_ډېری[config_TIMER_TASK_ډېری_ژوروالی];

فراغ v_Application_Get_Timer_Task_حافظه(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    ډېری_Type_t **ppx_Timer_Task_ډېری_بفر,
                                    uint32_t *pul_Timer_Task_ډېری_اندازه)
{
    ساكن StaticTask_t xTimerTaskTCB;
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppx_Timer_Task_ډېری_بفر = ux_Timer_Task_ډېری;
    *pul_Timer_Task_ډېری_اندازه = config_TIMER_TASK_ډېری_ژوروالی;
}
#د_که_پای

#د_که_پای // configSUPPORT_STATIC_ALLOCATION

/*-----------------------------------------------------------
 * Stack overflow hook
 *-----------------------------------------------------------*/
فراغ __attribute__((weak)) v_Application_ډېری_ډکېدنه_Hook(TaskHandle_t xTask, حرف *pcTaskName)
{
#تعریف_کړه ERR_STR1 "***ERROR*** A stack overflow in task "
#تعریف_کړه ERR_STR2 " has been detected."
    ثابت حرف *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    حرف buf[حجم(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + حجم(ERR_STR2) + 1 /* null char */] = {0};

    حرف *dest = buf;
    لكل (صحيح i = 0; i < حجم(str) / حجم(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
	چاپ_کړه("%s\n", buf);
    abort();
}
