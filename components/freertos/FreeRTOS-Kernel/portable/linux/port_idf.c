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

#وصف_ڪريو BACKTRACE_PC_ARRAY_سائيز 20
#وصف_ڪريو FREERTOS_SIM_BACKTRACE_سائيز 16
#وصف_ڪريو ON_SEGFAULT_پيغام "ERROR: Segmentation Fault, here's your backtrace:\n"
#وصف_ڪريو ON_ABORT_پيغام "ERROR: Aborted\n"

ساكن فراغ linux_پورٽ_backtrace_handler(صحيح sig)
{
    /* All calls here must be async-signal-safe (no stdio, no malloc).
     * write() and backtrace_symbols_fd() write directly to the fd. */
    فراغ *بفر[FREERTOS_SIM_BACKTRACE_سائيز];
    صحيح nptrs = backtrace(بفر, FREERTOS_SIM_BACKTRACE_سائيز);

    ثابت حرف *name = pcTaskGetName(عدم); /* simple pointer dereference, no lock */
    ssize_t ignore __attribute__((اڻ_استعمال_ٿيل));
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

فراغ linux_پورٽ_تياري_backtrace_signal(فراغ)
{
    بنية sigaction sa;
    sa.sa_handler = linux_پورٽ_backtrace_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, عدم);
}

فراغ linux_پورٽ_print_backtrace(فراغ)
{
    pthread_kill(linux_پورٽ_get_scheduled_task_pthread(), SIGUSR1);
}

ESP_LOG_ATTR_ٽيگ(LINUX_ٽيگ, "port_idf_linux");
ESP_LOG_ATTR_ٽيگ(MAIN_ٽيگ, "main_task");

#إذا (defined(__APPLE__) && defined(__MACH__))
تعريفنوع sig_t sighandler_t;
#جيڪڏهن_پورو

ساكن متطاير UBaseType_t ux_مداخلت_Nesting = 0;
بنياد_Type_t x_پورٽ_Check_If_اندريون_ISR(فراغ)
{
    أرجع ux_مداخلت_Nesting;
}

#إذا CONFIG_COMPILER_OPTIMIZATION_DEBUG
#وصف_ڪريو BACKTRACE_PC_ARRAY_سائيز_DUMMY 1
ساكن فراغ ڀريو_libgcc(فراغ)
{
    فراغ *array[BACKTRACE_PC_ARRAY_سائيز_DUMMY];
    size_t سائيز = backtrace(array, BACKTRACE_PC_ARRAY_سائيز_DUMMY);
    أكد(سائيز == 1);
}

ساكن فراغ segfault_handler(صحيح sig)
{
    فراغ *array[BACKTRACE_PC_ARRAY_سائيز];
    size_t سائيز = backtrace(array, BACKTRACE_PC_ARRAY_سائيز);
    ssize_t ignore __attribute__((اڻ_استعمال_ٿيل));
    ignore = اكتب(STDERR_FILENO, ON_SEGFAULT_پيغام, حجم(ON_SEGFAULT_پيغام));
    backtrace_symbols_fd(array, سائيز, STDERR_FILENO);
    _exit(1);
}

ساكن فراغ abort_handler(صحيح sig)
{
    // we need a raw file write here because other functions are not async-signal-safe
    ssize_t ignore __attribute__((اڻ_استعمال_ٿيل));
    ignore = اكتب(STDERR_FILENO, ON_ABORT_پيغام, حجم(ON_ABORT_پيغام));
    _exit(1);
}
#جيڪڏهن_پورو // CONFIG_COMPILER_OPTIMIZATION_DEBUG

/*-----------------------------------------------------------
* Main FreeRTOS task
*-----------------------------------------------------------*/
خارجي فراغ app_main(فراغ);
ساكن فراغ main_task(فراغ *دليلون)
{
    (فراغ)دليلون;
    ESP_LOGI(MAIN_ٽيگ, "Started on CPU%d", (صحيح)x_پورٽ_Get_Core_ID());

    ESP_LOGI(MAIN_ٽيگ, "Calling app_main()");

    app_main();

    ESP_LOGI(MAIN_ٽيگ, "Returned from app_main()");
    vTaskDelete(عدم);
}

فراغ esp_startup_شروعات_app(فراغ)
{
    setvbuf(stdout, عدم, _IONBF, 0);

#إذا CONFIG_COMPILER_OPTIMIZATION_DEBUG
    ڀريو_libgcc();
    sighandler_t sig_res;

    sig_res = إشارة(SIGSEGV, segfault_handler);
    إذا (sig_res == SIG_ERR) {
        abort();
    }

    sig_res = إشارة(SIGABRT, abort_handler);
    إذا (sig_res == SIG_ERR) {
        abort();
    }
#جيڪڏهن_پورو

    // Create main_task using FreeRTOS API
    ESP_LOGI(LINUX_ٽيگ, "Starting main task.");
    أكد(xTaskCreate(&main_task, "main", ESP_TASK_MAIN_ڍير, عدم, ESP_TASK_MAIN_PRIO, عدم) == pdTRUE);

    ESP_LOGI(LINUX_ٽيگ, "Starting scheduler task.");
    v_Task_شروعات_Scheduler();

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
#جيڪڏهن_پورو

#إذا (configUSE_TICK_HOOK > 0)
فراغ vApplicationTickHook(فراغ)
{
    خارجي فراغ esp_vApplicationTickHook(فراغ);
    esp_vApplicationTickHook();
}
#وإلا
#جيڪڏهن_پورو

/*-----------------------------------------------------------
 * Static allocation support
 *-----------------------------------------------------------*/
#إذا (config_SUPPORT_STATIC_ورهاست == 1)

فراغ v_Application_Get_Idle_Task_يادگيري(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   ڍير_Type_t **ppx_Idle_Task_ڍير_بفر,
                                   uint32_t *pul_Idle_Task_ڍير_سائيز)
{
    ساكن StaticTask_t xIdleTaskTCB;
    ساكن ڍير_Type_t ux_Idle_Task_ڍير[config_گهٽتر_ڍير_سائيز];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppx_Idle_Task_ڍير_بفر = ux_Idle_Task_ڍير;
    *pul_Idle_Task_ڍير_سائيز = config_گهٽتر_ڍير_سائيز;
}

#إذا (configUSE_TIMERS == 1)
ڍير_Type_t ux_Timer_Task_ڍير[config_TIMER_TASK_ڍير_اونهائي];

فراغ v_Application_Get_Timer_Task_يادگيري(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    ڍير_Type_t **ppx_Timer_Task_ڍير_بفر,
                                    uint32_t *pul_Timer_Task_ڍير_سائيز)
{
    ساكن StaticTask_t xTimerTaskTCB;
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppx_Timer_Task_ڍير_بفر = ux_Timer_Task_ڍير;
    *pul_Timer_Task_ڍير_سائيز = config_TIMER_TASK_ڍير_اونهائي;
}
#جيڪڏهن_پورو

#جيڪڏهن_پورو // configSUPPORT_STATIC_ALLOCATION

/*-----------------------------------------------------------
 * Stack overflow hook
 *-----------------------------------------------------------*/
فراغ __attribute__((weak)) v_Application_ڍير_حد_کان_مٿي_Hook(TaskHandle_t xTask, حرف *pcTaskName)
{
#وصف_ڪريو ERR_STR1 "***ERROR*** A stack overflow in task "
#وصف_ڪريو ERR_STR2 " has been detected."
    ثابت حرف *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    حرف buf[حجم(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + حجم(ERR_STR2) + 1 /* null char */] = {0};

    حرف *dest = buf;
    لكل (صحيح i = 0; i < حجم(str) / حجم(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
	لکو("%s\n", buf);
    abort();
}
