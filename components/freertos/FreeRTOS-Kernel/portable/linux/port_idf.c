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

#تعریف_کرو BACKTRACE_PC_ARRAY_ناپ 20
#تعریف_کرو FREERTOS_SIM_BACKTRACE_ناپ 16
#تعریف_کرو ON_SEGFAULT_سنیہا "ERROR: Segmentation Fault, here's your backtrace:\n"
#تعریف_کرو ON_ABORT_سنیہا "ERROR: Aborted\n"

ساكن فراغ linux_پورٹ_backtrace_handler(صحيح sig)
{
    /* All calls here must be async-signal-safe (no stdio, no malloc).
     * write() and backtrace_symbols_fd() write directly to the fd. */
    فراغ *بفر[FREERTOS_SIM_BACKTRACE_ناپ];
    صحيح nptrs = backtrace(بفر, FREERTOS_SIM_BACKTRACE_ناپ);

    ثابت حرف *name = pcTaskGetName(عدم); /* simple pointer dereference, no lock */
    ssize_t ignore __attribute__((بن_ورتیا));
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

فراغ linux_پورٹ_تیاری_backtrace_signal(فراغ)
{
    بنية sigaction sa;
    sa.sa_handler = linux_پورٹ_backtrace_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, عدم);
}

فراغ linux_پورٹ_print_backtrace(فراغ)
{
    pthread_kill(linux_پورٹ_get_scheduled_task_pthread(), SIGUSR1);
}

ESP_LOG_ATTR_ٹیگ(LINUX_ٹیگ, "port_idf_linux");
ESP_LOG_ATTR_ٹیگ(MAIN_ٹیگ, "main_task");

#إذا (defined(__APPLE__) && defined(__MACH__))
تعريفنوع sig_t sighandler_t;
#جے_انت

ساكن متطاير UBaseType_t ux_رکاوٹ_Nesting = 0;
ادھار_Type_t x_پورٹ_Check_If_وچ_ISR(فراغ)
{
    أرجع ux_رکاوٹ_Nesting;
}

#إذا CONFIG_COMPILER_OPTIMIZATION_DEBUG
#تعریف_کرو BACKTRACE_PC_ARRAY_ناپ_DUMMY 1
ساكن فراغ لدو_libgcc(فراغ)
{
    فراغ *array[BACKTRACE_PC_ARRAY_ناپ_DUMMY];
    size_t ناپ = backtrace(array, BACKTRACE_PC_ARRAY_ناپ_DUMMY);
    أكد(ناپ == 1);
}

ساكن فراغ segfault_handler(صحيح sig)
{
    فراغ *array[BACKTRACE_PC_ARRAY_ناپ];
    size_t ناپ = backtrace(array, BACKTRACE_PC_ARRAY_ناپ);
    ssize_t ignore __attribute__((بن_ورتیا));
    ignore = اكتب(STDERR_FILENO, ON_SEGFAULT_سنیہا, حجم(ON_SEGFAULT_سنیہا));
    backtrace_symbols_fd(array, ناپ, STDERR_FILENO);
    _exit(1);
}

ساكن فراغ abort_handler(صحيح sig)
{
    // we need a raw file write here because other functions are not async-signal-safe
    ssize_t ignore __attribute__((بن_ورتیا));
    ignore = اكتب(STDERR_FILENO, ON_ABORT_سنیہا, حجم(ON_ABORT_سنیہا));
    _exit(1);
}
#جے_انت // CONFIG_COMPILER_OPTIMIZATION_DEBUG

/*-----------------------------------------------------------
* Main FreeRTOS task
*-----------------------------------------------------------*/
خارجي فراغ app_main(فراغ);
ساكن فراغ main_task(فراغ *دلیلاں)
{
    (فراغ)دلیلاں;
    ESP_LOGI(MAIN_ٹیگ, "Started on CPU%d", (صحيح)x_پورٹ_Get_Core_ID());

    ESP_LOGI(MAIN_ٹیگ, "Calling app_main()");

    app_main();

    ESP_LOGI(MAIN_ٹیگ, "Returned from app_main()");
    vTaskDelete(عدم);
}

فراغ esp_startup_شروع_app(فراغ)
{
    setvbuf(stdout, عدم, _IONBF, 0);

#إذا CONFIG_COMPILER_OPTIMIZATION_DEBUG
    لدو_libgcc();
    sighandler_t sig_res;

    sig_res = إشارة(SIGSEGV, segfault_handler);
    إذا (sig_res == SIG_ERR) {
        abort();
    }

    sig_res = إشارة(SIGABRT, abort_handler);
    إذا (sig_res == SIG_ERR) {
        abort();
    }
#جے_انت

    // Create main_task using FreeRTOS API
    ESP_LOGI(LINUX_ٹیگ, "Starting main task.");
    أكد(xTaskCreate(&main_task, "main", ESP_TASK_MAIN_ڈھیر, عدم, ESP_TASK_MAIN_PRIO, عدم) == pdTRUE);

    ESP_LOGI(LINUX_ٹیگ, "Starting scheduler task.");
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
#جے_انت

#إذا (configUSE_TICK_HOOK > 0)
فراغ vApplicationTickHook(فراغ)
{
    خارجي فراغ esp_vApplicationTickHook(فراغ);
    esp_vApplicationTickHook();
}
#وإلا
#جے_انت

/*-----------------------------------------------------------
 * Static allocation support
 *-----------------------------------------------------------*/
#إذا (config_SUPPORT_STATIC_ونڈ == 1)

فراغ v_Application_Get_Idle_Task_حافظہ(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   ڈھیر_Type_t **ppx_Idle_Task_ڈھیر_بفر,
                                   uint32_t *pul_Idle_Task_ڈھیر_ناپ)
{
    ساكن StaticTask_t xIdleTaskTCB;
    ساكن ڈھیر_Type_t ux_Idle_Task_ڈھیر[config_اقل_ڈھیر_ناپ];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppx_Idle_Task_ڈھیر_بفر = ux_Idle_Task_ڈھیر;
    *pul_Idle_Task_ڈھیر_ناپ = config_اقل_ڈھیر_ناپ;
}

#إذا (configUSE_TIMERS == 1)
ڈھیر_Type_t ux_Timer_Task_ڈھیر[config_TIMER_TASK_ڈھیر_ڈونگھائی];

فراغ v_Application_Get_Timer_Task_حافظہ(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    ڈھیر_Type_t **ppx_Timer_Task_ڈھیر_بفر,
                                    uint32_t *pul_Timer_Task_ڈھیر_ناپ)
{
    ساكن StaticTask_t xTimerTaskTCB;
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppx_Timer_Task_ڈھیر_بفر = ux_Timer_Task_ڈھیر;
    *pul_Timer_Task_ڈھیر_ناپ = config_TIMER_TASK_ڈھیر_ڈونگھائی;
}
#جے_انت

#جے_انت // configSUPPORT_STATIC_ALLOCATION

/*-----------------------------------------------------------
 * Stack overflow hook
 *-----------------------------------------------------------*/
فراغ __attribute__((weak)) v_Application_ڈھیر_حد_توں_ودھ_Hook(TaskHandle_t xTask, حرف *pcTaskName)
{
#تعریف_کرو ERR_STR1 "***ERROR*** A stack overflow in task "
#تعریف_کرو ERR_STR2 " has been detected."
    ثابت حرف *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    حرف buf[حجم(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + حجم(ERR_STR2) + 1 /* null char */] = {0};

    حرف *dest = buf;
    لكل (صحيح i = 0; i < حجم(str) / حجم(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
	لکھو("%s\n", buf);
    abort();
}
