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
#شامل <array>
#شامل <cstdio>

#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"

نامخلا {

StaticTask_t s_self_tcb;
StaticTask_t s_heartbeat_tcb;
std::array<ڈھیر_Type_t, 4096> s_self_ڈھیر;
std::array<ڈھیر_Type_t, 4096> s_heartbeat_ڈھیر;

خالی self_delete_must_not_return(خالی *arg)
{
    (خالی)arg;
    vTaskDelete(خالی_اشاریہ);
}

خالی heartbeat(خالی *arg)
{
    (خالی)arg;
    برائے (;;) {
        std::لکھو("task_self_delete: still running\n");
        vTaskDelay(pd_MS_مقصد_TICKS(100));
    }
}

} // namespace

خارجی "C" خالی app_main(خالی)
{
    TaskHandle_t self_del = xTaskCreateStatic(self_delete_must_not_return,
                                              "self_del",
                                              s_self_ڈھیر.size(),
                                              خالی_اشاریہ,
                                              configMAX_PRIORITIES - 1,
                                              s_self_ڈھیر.data(),
                                              &s_self_tcb);
    اگر (self_del == خالی_اشاریہ) {
        std::لکھو("task_self_delete: FAIL create\n");
        واپس;
    }

    /* Child was higher priority, so it has already run. Surviving to here
     * means it did not fall off its entry after vTaskDelete(). */
    std::لکھو("task_self_delete: PASS\n");

    TaskHandle_t hb = xTaskCreateStatic(heartbeat,
                                        "hb",
                                        s_heartbeat_ڈھیر.size(),
                                        خالی_اشاریہ,
                                        tsk_IDLE_ترجیح + 1,
                                        s_heartbeat_ڈھیر.data(),
                                        &s_heartbeat_tcb);
    اگر (hb == خالی_اشاریہ) {
        std::لکھو("task_self_delete: FAIL create\n");
        واپس;
    }
}
