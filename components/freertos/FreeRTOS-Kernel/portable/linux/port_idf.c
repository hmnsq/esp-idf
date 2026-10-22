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

#تعریف_کرو BACKTRACE_PC_ARRAY_سائز 20
#تعریف_کرو FREERTOS_SIM_BACKTRACE_سائز 16
#تعریف_کرو ON_SEGFAULT_پیغام "ERROR: Segmentation Fault, here's your backtrace:\n"
#تعریف_کرو ON_ABORT_پیغام "ERROR: Aborted\n"

ساکن خالی linux_پورٹ_backtrace_handler(صحیح sig)
{
    /* All calls here must be async-signal-safe (no stdio, no malloc).
     * write() and backtrace_symbols_fd() write directly to the fd. */
    خالی *بفر[FREERTOS_SIM_BACKTRACE_سائز];
    صحیح nptrs = backtrace(بفر, FREERTOS_SIM_BACKTRACE_سائز);

    ثابت حرف *name = pcTaskGetName(تہی); /* simple pointer dereference, no lock */
    ssize_t ignore __attribute__((غیر_مستعمل));
    ignore = اكتب(STDERR_FILENO, "=== Backtrace for task: ", 24);
    اگر (name) {
        size_t len = 0;
        جب (name[len] != '\0') {
            len++;
        }
        ignore = اكتب(STDERR_FILENO, name, len);
    }
    ignore = اكتب(STDERR_FILENO, " ===\n", 5);
    backtrace_symbols_fd(بفر, nptrs, STDERR_FILENO);
}

خالی linux_پورٹ_تیاری_backtrace_signal(خالی)
{
    ساخت sigaction sa;
    sa.sa_handler = linux_پورٹ_backtrace_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, تہی);
}

خالی linux_پورٹ_print_backtrace(خالی)
{
    pthread_kill(linux_پورٹ_get_scheduled_task_pthread(), SIGUSR1);
}

ESP_LOG_ATTR_لیبل(LINUX_لیبل, "port_idf_linux");
ESP_LOG_ATTR_لیبل(MAIN_لیبل, "main_task");

#اگر (defined(__APPLE__) && defined(__MACH__))
قسمتعریف sig_t sighandler_t;
#اگر_ختم

ساکن متلون UBaseType_t ux_مداخلت_Nesting = 0;
بنیاد_Type_t x_پورٹ_Check_If_اندر_ISR(خالی)
{
    واپس ux_مداخلت_Nesting;
}

#اگر CONFIG_COMPILER_OPTIMIZATION_DEBUG
#تعریف_کرو BACKTRACE_PC_ARRAY_سائز_DUMMY 1
ساکن خالی بھرو_libgcc(خالی)
{
    خالی *array[BACKTRACE_PC_ARRAY_سائز_DUMMY];
    size_t سائز = backtrace(array, BACKTRACE_PC_ARRAY_سائز_DUMMY);
    أكد(سائز == 1);
}

ساکن خالی segfault_handler(صحیح sig)
{
    خالی *array[BACKTRACE_PC_ARRAY_سائز];
    size_t سائز = backtrace(array, BACKTRACE_PC_ARRAY_سائز);
    ssize_t ignore __attribute__((غیر_مستعمل));
    ignore = اكتب(STDERR_FILENO, ON_SEGFAULT_پیغام, حجم(ON_SEGFAULT_پیغام));
    backtrace_symbols_fd(array, سائز, STDERR_FILENO);
    _exit(1);
}

ساکن خالی abort_handler(صحیح sig)
{
    // we need a raw file write here because other functions are not async-signal-safe
    ssize_t ignore __attribute__((غیر_مستعمل));
    ignore = اكتب(STDERR_FILENO, ON_ABORT_پیغام, حجم(ON_ABORT_پیغام));
    _exit(1);
}
#اگر_ختم // CONFIG_COMPILER_OPTIMIZATION_DEBUG

/*-----------------------------------------------------------
* Main FreeRTOS task
*-----------------------------------------------------------*/
خارجی خالی app_main(خالی);
ساکن خالی main_task(خالی *دلائل)
{
    (خالی)دلائل;
    ESP_LOGI(MAIN_لیبل, "Started on CPU%d", (صحیح)x_پورٹ_Get_Core_ID());

    ESP_LOGI(MAIN_لیبل, "Calling app_main()");

    app_main();

    ESP_LOGI(MAIN_لیبل, "Returned from app_main()");
    vTaskDelete(تہی);
}

خالی esp_startup_آغاز_app(خالی)
{
    setvbuf(stdout, تہی, _IONBF, 0);

#اگر CONFIG_COMPILER_OPTIMIZATION_DEBUG
    بھرو_libgcc();
    sighandler_t sig_res;

    sig_res = اشارہ(SIGSEGV, segfault_handler);
    اگر (sig_res == SIG_ERR) {
        abort();
    }

    sig_res = اشارہ(SIGABRT, abort_handler);
    اگر (sig_res == SIG_ERR) {
        abort();
    }
#اگر_ختم

    // Create main_task using FreeRTOS API
    ESP_LOGI(LINUX_لیبل, "Starting main task.");
    أكد(xTaskCreate(&main_task, "main", ESP_TASK_MAIN_ڈھیر, تہی, ESP_TASK_MAIN_PRIO, تہی) == pdTRUE);

    ESP_LOGI(LINUX_لیبل, "Starting scheduler task.");
    v_Task_آغاز_Scheduler();

    // Should never reach here
    أكد(false);
}

/*-----------------------------------------------------------
 * idle and tick hooks
 *-----------------------------------------------------------*/
#اگر (configUSE_IDLE_HOOK > 0)
خالی vApplicationIdleHook(خالی)
{
}
#اگر_ختم

#اگر (configUSE_TICK_HOOK > 0)
خالی vApplicationTickHook(خالی)
{
    خارجی خالی esp_vApplicationTickHook(خالی);
    esp_vApplicationTickHook();
}
#ورنہ
#اگر_ختم

/*-----------------------------------------------------------
 * Static allocation support
 *-----------------------------------------------------------*/
#اگر (config_SUPPORT_STATIC_تخصیص == 1)

خالی v_Application_Get_Idle_Task_حافظہ(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   ڈھیر_Type_t **ppx_Idle_Task_ڈھیر_بفر,
                                   uint32_t *pul_Idle_Task_ڈھیر_سائز)
{
    ساکن StaticTask_t xIdleTaskTCB;
    ساکن ڈھیر_Type_t ux_Idle_Task_ڈھیر[config_اقل_ڈھیر_سائز];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppx_Idle_Task_ڈھیر_بفر = ux_Idle_Task_ڈھیر;
    *pul_Idle_Task_ڈھیر_سائز = config_اقل_ڈھیر_سائز;
}

#اگر (configUSE_TIMERS == 1)
ڈھیر_Type_t ux_Timer_Task_ڈھیر[config_TIMER_TASK_ڈھیر_گہرائی];

خالی v_Application_Get_Timer_Task_حافظہ(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    ڈھیر_Type_t **ppx_Timer_Task_ڈھیر_بفر,
                                    uint32_t *pul_Timer_Task_ڈھیر_سائز)
{
    ساکن StaticTask_t xTimerTaskTCB;
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppx_Timer_Task_ڈھیر_بفر = ux_Timer_Task_ڈھیر;
    *pul_Timer_Task_ڈھیر_سائز = config_TIMER_TASK_ڈھیر_گہرائی;
}
#اگر_ختم

#اگر_ختم // configSUPPORT_STATIC_ALLOCATION

/*-----------------------------------------------------------
 * Stack overflow hook
 *-----------------------------------------------------------*/
خالی __attribute__((weak)) v_Application_ڈھیر_حد_سے_تجاوز_Hook(TaskHandle_t xTask, حرف *pcTaskName)
{
#تعریف_کرو ERR_STR1 "***ERROR*** A stack overflow in task "
#تعریف_کرو ERR_STR2 " has been detected."
    ثابت حرف *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    حرف buf[حجم(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + حجم(ERR_STR2) + 1 /* null char */] = {0};

    حرف *dest = buf;
    برائے (صحیح i = 0; i < حجم(str) / حجم(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
	لکھو("%s\n", buf);
    abort();
}
