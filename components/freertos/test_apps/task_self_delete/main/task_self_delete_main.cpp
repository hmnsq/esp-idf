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
#అన్తర్భావ <array>
#అన్తర్భావ <cstdio>

#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"

నామాకాశ {

StaticTask_t s_self_tcb;
StaticTask_t s_heartbeat_tcb;
std::array<దొంతర_Type_t, 4096> s_self_దొంతర;
std::array<దొంతర_Type_t, 4096> s_heartbeat_దొంతర;

శూన్య self_delete_must_not_return(శూన్య *arg)
{
    (శూన్య)arg;
    vTaskDelete(శూన్యసూచిక);
}

శూన్య heartbeat(శూన్య *arg)
{
    (శూన్య)arg;
    కొరకు (;;) {
        std::రాయి("task_self_delete: still running\n");
        vTaskDelay(pd_MS_గమ్యం_TICKS(100));
    }
}

} // namespace

బాహ్య "C" శూన్య app_main(శూన్య)
{
    TaskHandle_t self_del = xTaskCreateStatic(self_delete_must_not_return,
                                              "self_del",
                                              s_self_దొంతర.size(),
                                              శూన్యసూచిక,
                                              configMAX_PRIORITIES - 1,
                                              s_self_దొంతర.data(),
                                              &s_self_tcb);
    ఒకవేళ (self_del == శూన్యసూచిక) {
        std::రాయి("task_self_delete: FAIL create\n");
        తిరిగి;
    }

    /* Child was higher priority, so it has already run. Surviving to here
     * means it did not fall off its entry after vTaskDelete(). */
    std::రాయి("task_self_delete: PASS\n");

    TaskHandle_t hb = xTaskCreateStatic(heartbeat,
                                        "hb",
                                        s_heartbeat_దొంతర.size(),
                                        శూన్యసూచిక,
                                        tsk_IDLE_ప్రాధాన్యత + 1,
                                        s_heartbeat_దొంతర.data(),
                                        &s_heartbeat_tcb);
    ఒకవేళ (hb == శూన్యసూచిక) {
        std::రాయి("task_self_delete: FAIL create\n");
        తిరిగి;
    }
}
