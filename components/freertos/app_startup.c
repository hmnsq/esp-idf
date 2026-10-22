/*
 * SPDX-FileCopyrightText: 2022-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ <stddef.h>
#అన్తర్భావ <assert.h>
#అన్తర్భావ "esp_task.h"
#అన్తర్భావ "esp_log.h"
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "freertos/portmacro.h"
#అన్తర్భావ "esp_private/esp_sys_event_internal.h"

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
#ఒకవేళ CONFIG_FREERTOS_UNICORE != CONFIG_ESP_SYSTEM_SINGLE_CORE_బహుళకం
#దోషం "AMP not supported. FreeRTOS number of cores and system number of cores must be identical"
#అయితే_ముగింపు

// -------------------- Declarations -----------------------

స్థిర శూన్య main_task(శూన్య* వాదనలు);
ESP_LOG_ATTR_ట్యాగ్(APP_ప్రారంభం_ట్యాగ్, "app_start");

స్థిర శూన్య నిరంతరం_app_startup_event(esp_sys_event_id_t id)
{
    ESP_SYS_EVENT_FOREACH(handler, id) {
        ESP_ERROR_CHECK(handler->handler(ఖాళీ, ఖాళీ));
    }
}

// ------------------ CPU0 App Startup ---------------------

శూన్య esp_startup_ప్రారంభం_app(శూన్య)
{
    నిరంతరం_app_startup_event(ESP_SYS_EVENT_PRE_SCHEDULER);

    పునాది_Type_t res = x_Task_Create_Pinned_గమ్యం_Core(main_task, "main",
                                             ESP_TASK_MAIN_దొంతర, ఖాళీ,
                                             ESP_TASK_MAIN_PRIO, ఖాళీ, ESP_TASK_MAIN_CORE);
    ప్రతిజ్ఞా(res == pdTRUE);
    (శూన్య)res;

    /*
    If a particular FreeRTOS port has port/arch specific OS startup behavior, they can implement a function of type
    "void port_start_app_hook(void)" in their `port.c` files. This function will be called below, thus allowing each
    FreeRTOS port to implement port specific app startup behavior.

    Deprecated: Register a PRE_SCHEDULER handler with priority 999 to preserve this ordering.
    */
    // TODO: IDF-16135
    శూన్య __attribute__((weak, deprecated("port_start_app_hook is deprecated and will be removed in IDF 7.0. Use ESP_PRE_SCHEDULER_HANDLER_REGISTER instead.")))
    పోర్టు_ప్రారంభం_app_hook(శూన్య);
#సూచన GCC diagnostic నెట్టు
#సూచన GCC diagnostic ignored "-Wdeprecated-declarations"
    ఒకవేళ (పోర్టు_ప్రారంభం_app_hook != ఖాళీ) {
        ESP_EARLY_LOGW(APP_ప్రారంభం_ట్యాగ్,
                       "port_start_app_hook is deprecated and will be removed in IDF 7.0. "
                       "Use ESP_PRE_SCHEDULER_HANDLER_REGISTER instead.");
        పోర్టు_ప్రారంభం_app_hook();
    }
#సూచన GCC diagnostic తీయి

    ESP_EARLY_LOGD(APP_ప్రారంభం_ట్యాగ్, "Starting scheduler on CPU0");
    v_Task_ప్రారంభం_Scheduler();
}

// --------------- CPU[1:N-1] App Startup ------------------

#ఒకవేళ !CONFIG_FREERTOS_UNICORE
శూన్య esp_startup_ప్రారంభం_app_other_cores(శూన్య)
{
    // For now, we only support up to two core: 0 and 1.
    ఒకవేళ (x_పోర్టు_Get_Core_ID() >= 2) {
        abort();
    }

    // Wait for CPU0 to start FreeRTOS before progressing
    బాహ్య అస్థిర గుర్తులేని పోర్టు_x_Scheduler_Running[CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES];
    అయితే (పోర్టు_x_Scheduler_Running[0] == 0) {
        ;
    }

    నిరంతరం_app_startup_event(ESP_SYS_EVENT_PRE_SCHEDULER);

    ESP_EARLY_LOGD(APP_ప్రారంభం_ట్యాగ్, "Starting scheduler on CPU%d", x_పోర్టు_Get_Core_ID());
    x_పోర్టు_ప్రారంభం_Scheduler();
    abort(); // Only get to here if FreeRTOS somehow very broken
}
#అయితే_ముగింపు // !CONFIG_FREERTOS_UNICORE

/* ---------------------------------------------------- Main Task ------------------------------------------------------
 * - main_task is a daemon task created by CPU0 before it starts FreeRTOS
 *      - Pinned to CPU(ESP_TASK_MAIN_CORE)
 *      - Priority of ESP_TASK_MAIN_PRIO
 * - Used to dispatch "void app_main(void)" provided by the application
 * - main_task will self delete if app_main returns
 * ------------------------------------------------------------------------------------------------------------------ */

ESP_LOG_ATTR_ట్యాగ్(MAIN_ట్యాగ్, "main_task");

/* This function has to guarantee that all CPUs have finished the FreeRTOS initialization
* and the startup stack is not in use.
*/
స్థిర శూన్య wait_for_all_cores_ready(శూన్య)
{
#ఒకవేళ !CONFIG_FREERTOS_UNICORE
    బాహ్య అస్థిర గుర్తులేని పోర్టు_ux_Core_Startup_పూర్తి[CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES];
    bool all_cpus_has_been_started;
    చేయి {
        all_cpus_has_been_started = true;
        కొరకు (పూర్ణాంకం cpu = 0; cpu < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; cpu++) {
            all_cpus_has_been_started &= పోర్టు_ux_Core_Startup_పూర్తి[cpu] != 0;
        }
    } అయితే (!all_cpus_has_been_started);
#అయితే_ముగింపు // !CONFIG_FREERTOS_UNICORE
}

స్థిర శూన్య main_task(శూన్య* వాదనలు)
{
    ESP_LOGI(MAIN_ట్యాగ్, "Started on CPU%d", (పూర్ణాంకం)x_పోర్టు_Get_Core_ID());

    wait_for_all_cores_ready();

    నిరంతరం_app_startup_event(ESP_SYS_EVENT_PRE_APP_MAIN);

    /*
    Note: Be careful when changing the "Calling app_main()" log below as multiple pytest scripts expect this log as a
    start-of-application marker.
    */
    ESP_LOGI(MAIN_ట్యాగ్, "Calling app_main()");
    బాహ్య శూన్య app_main(శూన్య);
    app_main();
    ESP_LOGI(MAIN_ట్యాగ్, "Returned from app_main()");
    vTaskDelete(ఖాళీ);
}
