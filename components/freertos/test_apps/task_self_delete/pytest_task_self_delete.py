# SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
# SPDX-License-Identifier: Apache-2.0
आनयन pytest
तः pytest_embedded आनयन Dut
तः pytest_embedded_idf.utils आनयन idf_parametrize


@pytest.mark.generic
@idf_parametrize('config', ['clang'], परोक्षम्=['config'])
@idf_parametrize('target', ['esp32', 'esp32s2', 'esp32s3'], परोक्षम्=['target'])
परिभाषा test_task_self_delete(dut: Dut) -> किमपि_न:
    dut.expect_exact('task_self_delete: PASS')
    # PASS is printed before main_task self-deletes. The heartbeat only runs
    # if that delete (and the child delete) did not panic.
    dut.expect_exact('task_self_delete: still running')
