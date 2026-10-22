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

#تعریف_کن BACKTRACE_PC_ARRAY_اندازه 20
#تعریف_کن FREERTOS_SIM_BACKTRACE_اندازه 16
#تعریف_کن ON_SEGFAULT_پیام "ERROR: Segmentation Fault, here's your backtrace:\n"
#تعریف_کن ON_ABORT_پیام "ERROR: Aborted\n"

ساكن فراغ linux_پورت_backtrace_handler(صحيح sig)
{
    /* All calls here must be async-signal-safe (no stdio, no malloc).
     * write() and backtrace_symbols_fd() write directly to the fd. */
    فراغ *میانگیر[FREERTOS_SIM_BACKTRACE_اندازه];
    صحيح nptrs = backtrace(میانگیر, FREERTOS_SIM_BACKTRACE_اندازه);

    ثابت حرف *name = pcTaskGetName(عدم); /* simple pointer dereference, no lock */
    ssize_t ignore __attribute__((استفادهنشده));
    ignore = اكتب(STDERR_FILENO, "=== Backtrace for task: ", 24);
    إذا (name) {
        size_t len = 0;
        طالما (name[len] != '\0') {
            len++;
        }
        ignore = اكتب(STDERR_FILENO, name, len);
    }
    ignore = اكتب(STDERR_FILENO, " ===\n", 5);
    backtrace_symbols_fd(میانگیر, nptrs, STDERR_FILENO);
}

فراغ linux_پورت_راهاندازی_backtrace_signal(فراغ)
{
    بنية sigaction sa;
    sa.sa_handler = linux_پورت_backtrace_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, عدم);
}

فراغ linux_پورت_print_backtrace(فراغ)
{
    pthread_kill(linux_پورت_get_scheduled_task_pthread(), SIGUSR1);
}

ESP_LOG_ATTR_برچسب(LINUX_برچسب, "port_idf_linux");
ESP_LOG_ATTR_برچسب(MAIN_برچسب, "main_task");

#إذا (defined(__APPLE__) && defined(__MACH__))
تعريفنوع sig_t sighandler_t;
#پایان_اگر

ساكن متطاير UBaseType_t ux_وقفه_Nesting = 0;
پایه_Type_t x_پورت_Check_If_درون_ISR(فراغ)
{
    أرجع ux_وقفه_Nesting;
}

#إذا CONFIG_COMPILER_OPTIMIZATION_DEBUG
#تعریف_کن BACKTRACE_PC_ARRAY_اندازه_DUMMY 1
ساكن فراغ بارگیری_libgcc(فراغ)
{
    فراغ *array[BACKTRACE_PC_ARRAY_اندازه_DUMMY];
    size_t اندازه = backtrace(array, BACKTRACE_PC_ARRAY_اندازه_DUMMY);
    أكد(اندازه == 1);
}

ساكن فراغ segfault_handler(صحيح sig)
{
    فراغ *array[BACKTRACE_PC_ARRAY_اندازه];
    size_t اندازه = backtrace(array, BACKTRACE_PC_ARRAY_اندازه);
    ssize_t ignore __attribute__((استفادهنشده));
    ignore = اكتب(STDERR_FILENO, ON_SEGFAULT_پیام, حجم(ON_SEGFAULT_پیام));
    backtrace_symbols_fd(array, اندازه, STDERR_FILENO);
    _exit(1);
}

ساكن فراغ abort_handler(صحيح sig)
{
    // we need a raw file write here because other functions are not async-signal-safe
    ssize_t ignore __attribute__((استفادهنشده));
    ignore = اكتب(STDERR_FILENO, ON_ABORT_پیام, حجم(ON_ABORT_پیام));
    _exit(1);
}
#پایان_اگر // CONFIG_COMPILER_OPTIMIZATION_DEBUG

/*-----------------------------------------------------------
* Main FreeRTOS task
*-----------------------------------------------------------*/
خارجي فراغ app_main(فراغ);
ساكن فراغ main_task(فراغ *آرگومانها)
{
    (فراغ)آرگومانها;
    ESP_LOGI(MAIN_برچسب, "Started on CPU%d", (صحيح)x_پورت_Get_Core_ID());

    ESP_LOGI(MAIN_برچسب, "Calling app_main()");

    app_main();

    ESP_LOGI(MAIN_برچسب, "Returned from app_main()");
    vTaskDelete(عدم);
}

فراغ esp_startup_شروع_app(فراغ)
{
    setvbuf(stdout, عدم, _IONBF, 0);

#إذا CONFIG_COMPILER_OPTIMIZATION_DEBUG
    بارگیری_libgcc();
    sighandler_t sig_res;

    sig_res = إشارة(SIGSEGV, segfault_handler);
    إذا (sig_res == SIG_ERR) {
        abort();
    }

    sig_res = إشارة(SIGABRT, abort_handler);
    إذا (sig_res == SIG_ERR) {
        abort();
    }
#پایان_اگر

    // Create main_task using FreeRTOS API
    ESP_LOGI(LINUX_برچسب, "Starting main task.");
    أكد(xTaskCreate(&main_task, "main", ESP_TASK_MAIN_پشته, عدم, ESP_TASK_MAIN_PRIO, عدم) == pdTRUE);

    ESP_LOGI(LINUX_برچسب, "Starting scheduler task.");
    v_Task_شروع_Scheduler();

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
#پایان_اگر

#إذا (configUSE_TICK_HOOK > 0)
فراغ vApplicationTickHook(فراغ)
{
    خارجي فراغ esp_vApplicationTickHook(فراغ);
    esp_vApplicationTickHook();
}
#وإلا
#پایان_اگر

/*-----------------------------------------------------------
 * Static allocation support
 *-----------------------------------------------------------*/
#إذا (config_SUPPORT_STATIC_تخصیص == 1)

فراغ v_Application_Get_Idle_Task_حافظه(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   پشته_Type_t **ppx_Idle_Task_پشته_میانگیر,
                                   uint32_t *pul_Idle_Task_پشته_اندازه)
{
    ساكن StaticTask_t xIdleTaskTCB;
    ساكن پشته_Type_t ux_Idle_Task_پشته[config_کمینه_پشته_اندازه];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppx_Idle_Task_پشته_میانگیر = ux_Idle_Task_پشته;
    *pul_Idle_Task_پشته_اندازه = config_کمینه_پشته_اندازه;
}

#إذا (configUSE_TIMERS == 1)
پشته_Type_t ux_Timer_Task_پشته[config_TIMER_TASK_پشته_عمق];

فراغ v_Application_Get_Timer_Task_حافظه(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    پشته_Type_t **ppx_Timer_Task_پشته_میانگیر,
                                    uint32_t *pul_Timer_Task_پشته_اندازه)
{
    ساكن StaticTask_t xTimerTaskTCB;
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppx_Timer_Task_پشته_میانگیر = ux_Timer_Task_پشته;
    *pul_Timer_Task_پشته_اندازه = config_TIMER_TASK_پشته_عمق;
}
#پایان_اگر

#پایان_اگر // configSUPPORT_STATIC_ALLOCATION

/*-----------------------------------------------------------
 * Stack overflow hook
 *-----------------------------------------------------------*/
فراغ __attribute__((weak)) v_Application_پشته_سرریز_Hook(TaskHandle_t xTask, حرف *pcTaskName)
{
#تعریف_کن ERR_STR1 "***ERROR*** A stack overflow in task "
#تعریف_کن ERR_STR2 " has been detected."
    ثابت حرف *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    حرف buf[حجم(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + حجم(ERR_STR2) + 1 /* null char */] = {0};

    حرف *dest = buf;
    لكل (صحيح i = 0; i < حجم(str) / حجم(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
	چاپ_کن("%s\n", buf);
    abort();
}
