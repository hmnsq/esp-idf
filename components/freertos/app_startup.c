/*
 * SPDX-FileCopyrightText: 2022-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#شامل "sdkconfig.h"
#شامل <stddef.h>
#شامل <assert.h>
#شامل "esp_task.h"
#شامل "esp_log.h"
#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل "freertos/portmacro.h"
#شامل "esp_private/esp_sys_event_internal.h"

/* ------------------------------------------------- App/OS Startup ----------------------------------------------------
 * - Functions related to application and FreeRTOS startup
 * - This startup is common to all architectures (e.g. RISC-V and Xtensa) and all FreeRTOS implementations (i.e., IDF
 *   FreeRTOS and Amazon SMP FreeRTOS).
 * - Application startup flow as follows:
 *      - For CPU 0
 *          - CPU0 completes CPU startup (in startup.c), then calls esp_startup_start_app()
 *          - esp_startup_start_app() registers some daemon services for CPU0 then starts FreeRTOS
 *      - For CPUx (1 to N-1)
 *          - CPUx completes CPU startup in startup.c, then calls esp_startup_start_app_other_cores()
 *          - esp_startup_start_app_other_cores(), registers some daemon services for CPUx, waits for CPU0 to start
 *            FreeRTOS, then yields (via xPortStartScheduler()) to schedule a task.
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------------- Checks --------------------------

/*
For now, AMP is not supported (i.e., running FreeRTOS on one core and a bare metal/other OS on the other). Therefore,
CONFIG_FREERTOS_UNICORE and CONFIG_ESP_SYSTEM_SINGLE_CORE_MODE should be identical. We add a check for this here.
*/
#اگر CONFIG_FREERTOS_UNICORE != CONFIG_ESP_SYSTEM_SINGLE_CORE_منوال
#غلطی "AMP not supported. FreeRTOS number of cores and system number of cores must be identical"
#اگر_ختم

// -------------------- Declarations -----------------------

ساکن خالی main_task(خالی* دلائل);
ESP_LOG_ATTR_لیبل(APP_آغاز_لیبل, "app_start");

ساکن خالی تسلسل_app_startup_event(esp_sys_event_id_t id)
{
    ESP_SYS_EVENT_FOREACH(handler, id) {
        ESP_ERROR_CHECK(handler->handler(تہی, تہی));
    }
}

// ------------------ CPU0 App Startup ---------------------

خالی esp_startup_آغاز_app(خالی)
{
    تسلسل_app_startup_event(ESP_SYS_EVENT_PRE_SCHEDULER);

    بنیاد_Type_t res = x_Task_Create_Pinned_مقصد_Core(main_task, "main",
                                             ESP_TASK_MAIN_ڈھیر, تہی,
                                             ESP_TASK_MAIN_PRIO, تہی, ESP_TASK_MAIN_CORE);
    أكد(res == pdTRUE);
    (خالی)res;

    /*
    If a particular FreeRTOS port has port/arch specific OS startup behavior, they can implement a function of type
    "void port_start_app_hook(void)" in their `port.c` files. This function will be called below, thus allowing each
    FreeRTOS port to implement port specific app startup behavior.

    Deprecated: Register a PRE_SCHEDULER handler with priority 999 to preserve this ordering.
    */
    // TODO: IDF-16135
    خالی __attribute__((weak, deprecated("port_start_app_hook is deprecated and will be removed in IDF 7.0. Use ESP_PRE_SCHEDULER_HANDLER_REGISTER instead.")))
    پورٹ_آغاز_app_hook(خالی);
#ہدایت GCC diagnostic ڈالو
#ہدایت GCC diagnostic ignored "-Wdeprecated-declarations"
    اگر (پورٹ_آغاز_app_hook != تہی) {
        ESP_EARLY_LOGW(APP_آغاز_لیبل,
                       "port_start_app_hook is deprecated and will be removed in IDF 7.0. "
                       "Use ESP_PRE_SCHEDULER_HANDLER_REGISTER instead.");
        پورٹ_آغاز_app_hook();
    }
#ہدایت GCC diagnostic نکالو

    ESP_EARLY_LOGD(APP_آغاز_لیبل, "Starting scheduler on CPU0");
    v_Task_آغاز_Scheduler();
}

// --------------- CPU[1:N-1] App Startup ------------------

#اگر !CONFIG_FREERTOS_UNICORE
خالی esp_startup_آغاز_app_other_cores(خالی)
{
    // For now, we only support up to two core: 0 and 1.
    اگر (x_پورٹ_Get_Core_ID() >= 2) {
        abort();
    }

    // Wait for CPU0 to start FreeRTOS before progressing
    خارجی متلون غیرعلامتی پورٹ_x_Scheduler_Running[CONFIG_FREERTOS_نمبر_کا_CORES];
    جب (پورٹ_x_Scheduler_Running[0] == 0) {
        ;
    }

    تسلسل_app_startup_event(ESP_SYS_EVENT_PRE_SCHEDULER);

    ESP_EARLY_LOGD(APP_آغاز_لیبل, "Starting scheduler on CPU%d", x_پورٹ_Get_Core_ID());
    x_پورٹ_آغاز_Scheduler();
    abort(); // Only get to here if FreeRTOS somehow very broken
}
#اگر_ختم // !CONFIG_FREERTOS_UNICORE

/* ---------------------------------------------------- Main Task ------------------------------------------------------
 * - main_task is a daemon task created by CPU0 before it starts FreeRTOS
 *      - Pinned to CPU(ESP_TASK_MAIN_CORE)
 *      - Priority of ESP_TASK_MAIN_PRIO
 * - Used to dispatch "void app_main(void)" provided by the application
 * - main_task will self delete if app_main returns
 * ------------------------------------------------------------------------------------------------------------------ */

ESP_LOG_ATTR_لیبل(MAIN_لیبل, "main_task");

/* This function has to guarantee that all CPUs have finished the FreeRTOS initialization
* and the startup stack is not in use.
*/
ساکن خالی wait_for_all_cores_ready(خالی)
{
#اگر !CONFIG_FREERTOS_UNICORE
    خارجی متلون غیرعلامتی پورٹ_ux_Core_Startup_مکمل[CONFIG_FREERTOS_نمبر_کا_CORES];
    bool all_cpus_has_been_started;
    کرو {
        all_cpus_has_been_started = true;
        برائے (صحیح cpu = 0; cpu < CONFIG_FREERTOS_نمبر_کا_CORES; cpu++) {
            all_cpus_has_been_started &= پورٹ_ux_Core_Startup_مکمل[cpu] != 0;
        }
    } جب (!all_cpus_has_been_started);
#اگر_ختم // !CONFIG_FREERTOS_UNICORE
}

ساکن خالی main_task(خالی* دلائل)
{
    ESP_LOGI(MAIN_لیبل, "Started on CPU%d", (صحیح)x_پورٹ_Get_Core_ID());

    wait_for_all_cores_ready();

    تسلسل_app_startup_event(ESP_SYS_EVENT_PRE_APP_MAIN);

    /*
    Note: Be careful when changing the "Calling app_main()" log below as multiple pytest scripts expect this log as a
    start-of-application marker.
    */
    ESP_LOGI(MAIN_لیبل, "Calling app_main()");
    خارجی خالی app_main(خالی);
    app_main();
    ESP_LOGI(MAIN_لیبل, "Returned from app_main()");
    vTaskDelete(تہی);
}
