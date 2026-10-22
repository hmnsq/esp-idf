/*
 * SPDX-FileCopyrightText: 2022-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ <stddef.h>
#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ "esp_task.h"
#ಅನ್ತರ್ಭಾವ "esp_log.h"
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "freertos/portmacro.h"
#ಅನ್ತರ್ಭಾವ "esp_private/esp_sys_event_internal.h"

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
#ಆದರೆ CONFIG_FREERTOS_UNICORE != CONFIG_ESP_SYSTEM_SINGLE_CORE_ಬಹುಲಕ
#ದೋಷ "AMP not supported. FreeRTOS number of cores and system number of cores must be identical"
#ಆದರೆ_ಅಂತ್ಯ

// -------------------- Declarations -----------------------

ಸ್ಥಿರ ಶೂನ್ಯ main_task(ಶೂನ್ಯ* ವಾದಗಳು);
ESP_LOG_ATTR_ಟ್ಯಾಗ್(APP_ಆರಂಭ_ಟ್ಯಾಗ್, "app_start");

ಸ್ಥಿರ ಶೂನ್ಯ ಸತತ_app_startup_event(esp_sys_event_id_t id)
{
    ESP_SYS_EVENT_FOREACH(handler, id) {
        ESP_ERROR_CHECK(handler->handler(ಖಾಲಿ, ಖಾಲಿ));
    }
}

// ------------------ CPU0 App Startup ---------------------

ಶೂನ್ಯ esp_startup_ಆರಂಭ_app(ಶೂನ್ಯ)
{
    ಸತತ_app_startup_event(ESP_SYS_EVENT_PRE_SCHEDULER);

    ತಳ_Type_t res = x_Task_Create_Pinned_ಗಮ್ಯ_Core(main_task, "main",
                                             ESP_TASK_MAIN_ರಾಶಿ, ಖಾಲಿ,
                                             ESP_TASK_MAIN_PRIO, ಖಾಲಿ, ESP_TASK_MAIN_CORE);
    ಪ್ರತಿಜ್ಞಾ(res == pdTRUE);
    (ಶೂನ್ಯ)res;

    /*
    If a particular FreeRTOS port has port/arch specific OS startup behavior, they can implement a function of type
    "void port_start_app_hook(void)" in their `port.c` files. This function will be called below, thus allowing each
    FreeRTOS port to implement port specific app startup behavior.

    Deprecated: Register a PRE_SCHEDULER handler with priority 999 to preserve this ordering.
    */
    // TODO: IDF-16135
    ಶೂನ್ಯ __attribute__((weak, deprecated("port_start_app_hook is deprecated and will be removed in IDF 7.0. Use ESP_PRE_SCHEDULER_HANDLER_REGISTER instead.")))
    ಪೋರ್ಟ್_ಆರಂಭ_app_hook(ಶೂನ್ಯ);
#ಸೂಚನೆ GCC diagnostic ತಳ್ಳು
#ಸೂಚನೆ GCC diagnostic ignored "-Wdeprecated-declarations"
    ಆದರೆ (ಪೋರ್ಟ್_ಆರಂಭ_app_hook != ಖಾಲಿ) {
        ESP_EARLY_LOGW(APP_ಆರಂಭ_ಟ್ಯಾಗ್,
                       "port_start_app_hook is deprecated and will be removed in IDF 7.0. "
                       "Use ESP_PRE_SCHEDULER_HANDLER_REGISTER instead.");
        ಪೋರ್ಟ್_ಆರಂಭ_app_hook();
    }
#ಸೂಚನೆ GCC diagnostic ತೆಗೆ

    ESP_EARLY_LOGD(APP_ಆರಂಭ_ಟ್ಯಾಗ್, "Starting scheduler on CPU0");
    v_Task_ಆರಂಭ_Scheduler();
}

// --------------- CPU[1:N-1] App Startup ------------------

#ಆದರೆ !CONFIG_FREERTOS_UNICORE
ಶೂನ್ಯ esp_startup_ಆರಂಭ_app_other_cores(ಶೂನ್ಯ)
{
    // For now, we only support up to two core: 0 and 1.
    ಆದರೆ (x_ಪೋರ್ಟ್_Get_Core_ID() >= 2) {
        abort();
    }

    // Wait for CPU0 to start FreeRTOS before progressing
    ಬಾಹ್ಯ ಅಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೋರ್ಟ್_x_Scheduler_Running[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES];
    ಆಗ (ಪೋರ್ಟ್_x_Scheduler_Running[0] == 0) {
        ;
    }

    ಸತತ_app_startup_event(ESP_SYS_EVENT_PRE_SCHEDULER);

    ESP_EARLY_LOGD(APP_ಆರಂಭ_ಟ್ಯಾಗ್, "Starting scheduler on CPU%d", x_ಪೋರ್ಟ್_Get_Core_ID());
    x_ಪೋರ್ಟ್_ಆರಂಭ_Scheduler();
    abort(); // Only get to here if FreeRTOS somehow very broken
}
#ಆದರೆ_ಅಂತ್ಯ // !CONFIG_FREERTOS_UNICORE

/* ---------------------------------------------------- Main Task ------------------------------------------------------
 * - main_task is a daemon task created by CPU0 before it starts FreeRTOS
 *      - Pinned to CPU(ESP_TASK_MAIN_CORE)
 *      - Priority of ESP_TASK_MAIN_PRIO
 * - Used to dispatch "void app_main(void)" provided by the application
 * - main_task will self delete if app_main returns
 * ------------------------------------------------------------------------------------------------------------------ */

ESP_LOG_ATTR_ಟ್ಯಾಗ್(MAIN_ಟ್ಯಾಗ್, "main_task");

/* This function has to guarantee that all CPUs have finished the FreeRTOS initialization
* and the startup stack is not in use.
*/
ಸ್ಥಿರ ಶೂನ್ಯ wait_for_all_cores_ready(ಶೂನ್ಯ)
{
#ಆದರೆ !CONFIG_FREERTOS_UNICORE
    ಬಾಹ್ಯ ಅಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೋರ್ಟ್_ux_Core_Startup_ಮುಗಿದ[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES];
    bool all_cpus_has_been_started;
    ಮಾಡು {
        all_cpus_has_been_started = true;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ cpu = 0; cpu < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; cpu++) {
            all_cpus_has_been_started &= ಪೋರ್ಟ್_ux_Core_Startup_ಮುಗಿದ[cpu] != 0;
        }
    } ಆಗ (!all_cpus_has_been_started);
#ಆದರೆ_ಅಂತ್ಯ // !CONFIG_FREERTOS_UNICORE
}

ಸ್ಥಿರ ಶೂನ್ಯ main_task(ಶೂನ್ಯ* ವಾದಗಳು)
{
    ESP_LOGI(MAIN_ಟ್ಯಾಗ್, "Started on CPU%d", (ಪೂರ್ಣಾಂಕ)x_ಪೋರ್ಟ್_Get_Core_ID());

    wait_for_all_cores_ready();

    ಸತತ_app_startup_event(ESP_SYS_EVENT_PRE_APP_MAIN);

    /*
    Note: Be careful when changing the "Calling app_main()" log below as multiple pytest scripts expect this log as a
    start-of-application marker.
    */
    ESP_LOGI(MAIN_ಟ್ಯಾಗ್, "Calling app_main()");
    ಬಾಹ್ಯ ಶೂನ್ಯ app_main(ಶೂನ್ಯ);
    app_main();
    ESP_LOGI(MAIN_ಟ್ಯಾಗ್, "Returned from app_main()");
    vTaskDelete(ಖಾಲಿ);
}
