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
#ضمن <array>
#ضمن <cstdio>

#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"

نطاق {

StaticTask_t s_self_tcb;
StaticTask_t s_heartbeat_tcb;
std::array<ډېری_Type_t, 4096> s_self_ډېری;
std::array<ډېری_Type_t, 4096> s_heartbeat_ډېری;

فراغ self_delete_must_not_return(فراغ *arg)
{
    (فراغ)arg;
    vTaskDelete(تش_اشاره);
}

فراغ heartbeat(فراغ *arg)
{
    (فراغ)arg;
    لكل (;;) {
        std::چاپ_کړه("task_self_delete: still running\n");
        vTaskDelay(pd_MS_منزل_TICKS(100));
    }
}

} // namespace

خارجي "C" فراغ app_main(فراغ)
{
    TaskHandle_t self_del = xTaskCreateStatic(self_delete_must_not_return,
                                              "self_del",
                                              s_self_ډېری.size(),
                                              تش_اشاره,
                                              configMAX_PRIORITIES - 1,
                                              s_self_ډېری.data(),
                                              &s_self_tcb);
    إذا (self_del == تش_اشاره) {
        std::چاپ_کړه("task_self_delete: FAIL create\n");
        أرجع;
    }

    /* Child was higher priority, so it has already run. Surviving to here
     * means it did not fall off its entry after vTaskDelete(). */
    std::چاپ_کړه("task_self_delete: PASS\n");

    TaskHandle_t hb = xTaskCreateStatic(heartbeat,
                                        "hb",
                                        s_heartbeat_ډېری.size(),
                                        تش_اشاره,
                                        tsk_IDLE_لومړیتوب + 1,
                                        s_heartbeat_ډېری.data(),
                                        &s_heartbeat_tcb);
    إذا (hb == تش_اشاره) {
        std::چاپ_کړه("task_self_delete: FAIL create\n");
        أرجع;
    }
}
