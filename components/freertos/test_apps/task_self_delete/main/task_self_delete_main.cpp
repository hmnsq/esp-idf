/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * On Xtensa unicore, portYIELD_WITHIN_API() only pends a yield and returns.
 * vTaskDelete(NULL) must not come back to the deleted task: under Clang -O2
 * that continuation can fall off the task entry and panic with
 * IllegalInstruction. See https://github.com/espressif/esp-idf/issues/18460
 *
 * The child body is only vTaskDelete(), matching the original repro. Extra
 * code after vTaskDelete() (a flag store or busy-loop) is enough work that
 * the deferred yield interrupt often wins, so those checks miss the bug.
 *
 * app_main then returns so main_task also self-deletes (same kernel path).
 * A lower-priority heartbeat is created after PASS and must keep printing
 * if that second delete did not panic.
 */
#ਅਨ੍ਤਰ੍ਭਾਵ <array>
#ਅਨ੍ਤਰ੍ਭਾਵ <cstdio>

#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"

ਨਾਮਾਕਾਸ਼ {

StaticTask_t s_self_tcb;
StaticTask_t s_heartbeat_tcb;
std::array<ਢੇਰ_Type_t, 4096> s_self_ਢੇਰ;
std::array<ਢੇਰ_Type_t, 4096> s_heartbeat_ਢੇਰ;

ਖਾਲੀ self_delete_must_not_return(ਖਾਲੀ *arg)
{
    (ਖਾਲੀ)arg;
    vTaskDelete(ਖਾਲੀ_ਸੂਚਕ);
}

ਖਾਲੀ heartbeat(ਖਾਲੀ *arg)
{
    (ਖਾਲੀ)arg;
    ਲਈ (;;) {
        std::ਲਿਖੋ("task_self_delete: still running\n");
        vTaskDelay(pd_MS_ਮੰਜ਼ਿਲ_TICKS(100));
    }
}

} // namespace

ਬਾਹਰੀ "C" ਖਾਲੀ app_main(ਖਾਲੀ)
{
    TaskHandle_t self_del = xTaskCreateStatic(self_delete_must_not_return,
                                              "self_del",
                                              s_self_ਢੇਰ.size(),
                                              ਖਾਲੀ_ਸੂਚਕ,
                                              configMAX_PRIORITIES - 1,
                                              s_self_ਢੇਰ.data(),
                                              &s_self_tcb);
    ਜੇ (self_del == ਖਾਲੀ_ਸੂਚਕ) {
        std::ਲਿਖੋ("task_self_delete: FAIL create\n");
        ਵਾਪਸ;
    }

    /* Child was higher priority, so it has already run. Surviving to here
     * means it did not fall off its entry after vTaskDelete(). */
    std::ਲਿਖੋ("task_self_delete: PASS\n");

    TaskHandle_t hb = xTaskCreateStatic(heartbeat,
                                        "hb",
                                        s_heartbeat_ਢੇਰ.size(),
                                        ਖਾਲੀ_ਸੂਚਕ,
                                        tsk_IDLE_ਤਰਜੀਹ + 1,
                                        s_heartbeat_ਢੇਰ.data(),
                                        &s_heartbeat_tcb);
    ਜੇ (hb == ਖਾਲੀ_ਸੂਚਕ) {
        std::ਲਿਖੋ("task_self_delete: FAIL create\n");
        ਵਾਪਸ;
    }
}
