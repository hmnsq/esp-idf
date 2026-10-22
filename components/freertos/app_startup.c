/*
 * SPDX-FileCopyrightText: 2022-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "sdkconfig.h"
#ضمن <stddef.h>
#ضمن <assert.h>
#ضمن "esp_task.h"
#ضمن "esp_log.h"
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "freertos/portmacro.h"
#ضمن "esp_private/esp_sys_event_internal.h"

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
#إذا CONFIG_FREERTOS_UNICORE != CONFIG_ESP_SYSTEM_SINGLE_CORE_ډېرپېښ
#تېروتنه "AMP not supported. FreeRTOS number of cores and system number of cores must be identical"
#د_که_پای

// -------------------- Declarations -----------------------

ساكن فراغ main_task(فراغ* دلیلونه);
ESP_LOG_ATTR_نښان(APP_پیل_نښان, "app_start");

ساكن فراغ پرلپسې_app_startup_event(esp_sys_event_id_t id)
{
    ESP_SYS_EVENT_FOREACH(handler, id) {
        ESP_ERROR_CHECK(handler->handler(عدم, عدم));
    }
}

// ------------------ CPU0 App Startup ---------------------

فراغ esp_startup_پیل_app(فراغ)
{
    پرلپسې_app_startup_event(ESP_SYS_EVENT_PRE_SCHEDULER);

    بنسټ_Type_t res = x_Task_Create_Pinned_منزل_Core(main_task, "main",
                                             ESP_TASK_MAIN_ډېری, عدم,
                                             ESP_TASK_MAIN_PRIO, عدم, ESP_TASK_MAIN_CORE);
    أكد(res == pdTRUE);
    (فراغ)res;

    /*
    If a particular FreeRTOS port has port/arch specific OS startup behavior, they can implement a function of type
    "void port_start_app_hook(void)" in their `port.c` files. This function will be called below, thus allowing each
    FreeRTOS port to implement port specific app startup behavior.

    Deprecated: Register a PRE_SCHEDULER handler with priority 999 to preserve this ordering.
    */
    // TODO: IDF-16135
    فراغ __attribute__((weak, deprecated("port_start_app_hook is deprecated and will be removed in IDF 7.0. Use ESP_PRE_SCHEDULER_HANDLER_REGISTER instead.")))
    پورټ_پیل_app_hook(فراغ);
#لارښوونه GCC diagnostic ټېل
#لارښوونه GCC diagnostic ignored "-Wdeprecated-declarations"
    إذا (پورټ_پیل_app_hook != عدم) {
        ESP_EARLY_LOGW(APP_پیل_نښان,
                       "port_start_app_hook is deprecated and will be removed in IDF 7.0. "
                       "Use ESP_PRE_SCHEDULER_HANDLER_REGISTER instead.");
        پورټ_پیل_app_hook();
    }
#لارښوونه GCC diagnostic راوباسه

    ESP_EARLY_LOGD(APP_پیل_نښان, "Starting scheduler on CPU0");
    v_Task_پیل_Scheduler();
}

// --------------- CPU[1:N-1] App Startup ------------------

#إذا !CONFIG_FREERTOS_UNICORE
فراغ esp_startup_پیل_app_other_cores(فراغ)
{
    // For now, we only support up to two core: 0 and 1.
    إذا (x_پورټ_Get_Core_ID() >= 2) {
        abort();
    }

    // Wait for CPU0 to start FreeRTOS before progressing
    خارجي متطاير بینښه پورټ_x_Scheduler_Running[CONFIG_FREERTOS_شمېره_د_CORES];
    طالما (پورټ_x_Scheduler_Running[0] == 0) {
        ;
    }

    پرلپسې_app_startup_event(ESP_SYS_EVENT_PRE_SCHEDULER);

    ESP_EARLY_LOGD(APP_پیل_نښان, "Starting scheduler on CPU%d", x_پورټ_Get_Core_ID());
    x_پورټ_پیل_Scheduler();
    abort(); // Only get to here if FreeRTOS somehow very broken
}
#د_که_پای // !CONFIG_FREERTOS_UNICORE

/* ---------------------------------------------------- Main Task ------------------------------------------------------
 * - main_task is a daemon task created by CPU0 before it starts FreeRTOS
 *      - Pinned to CPU(ESP_TASK_MAIN_CORE)
 *      - Priority of ESP_TASK_MAIN_PRIO
 * - Used to dispatch "void app_main(void)" provided by the application
 * - main_task will self delete if app_main returns
 * ------------------------------------------------------------------------------------------------------------------ */

ESP_LOG_ATTR_نښان(MAIN_نښان, "main_task");

/* This function has to guarantee that all CPUs have finished the FreeRTOS initialization
* and the startup stack is not in use.
*/
ساكن فراغ wait_for_all_cores_ready(فراغ)
{
#إذا !CONFIG_FREERTOS_UNICORE
    خارجي متطاير بینښه پورټ_ux_Core_Startup_بشپړ[CONFIG_FREERTOS_شمېره_د_CORES];
    bool all_cpus_has_been_started;
    افعل {
        all_cpus_has_been_started = true;
        لكل (صحيح cpu = 0; cpu < CONFIG_FREERTOS_شمېره_د_CORES; cpu++) {
            all_cpus_has_been_started &= پورټ_ux_Core_Startup_بشپړ[cpu] != 0;
        }
    } طالما (!all_cpus_has_been_started);
#د_که_پای // !CONFIG_FREERTOS_UNICORE
}

ساكن فراغ main_task(فراغ* دلیلونه)
{
    ESP_LOGI(MAIN_نښان, "Started on CPU%d", (صحيح)x_پورټ_Get_Core_ID());

    wait_for_all_cores_ready();

    پرلپسې_app_startup_event(ESP_SYS_EVENT_PRE_APP_MAIN);

    /*
    Note: Be careful when changing the "Calling app_main()" log below as multiple pytest scripts expect this log as a
    start-of-application marker.
    */
    ESP_LOGI(MAIN_نښان, "Calling app_main()");
    خارجي فراغ app_main(فراغ);
    app_main();
    ESP_LOGI(MAIN_نښان, "Returned from app_main()");
    vTaskDelete(عدم);
}
