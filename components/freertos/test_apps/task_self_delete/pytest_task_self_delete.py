# SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
# SPDX-License-Identifier: Apache-2.0
درآمد pytest
سے pytest_embedded درآمد Dut
سے pytest_embedded_idf.utils درآمد idf_parametrize


@pytest.mark.generic
@idf_parametrize('config', ['clang'], بالواسطہ=['config'])
@idf_parametrize('target', ['esp32', 'esp32s2', 'esp32s3'], بالواسطہ=['target'])
تعریف test_task_self_delete(dut: Dut) -> کچھ_نہیں:
    dut.expect_exact('task_self_delete: PASS')
    # PASS is printed before main_task self-deletes. The heartbeat only runs
    # if that delete (and the child delete) did not panic.
    dut.expect_exact('task_self_delete: still running')
