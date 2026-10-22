# SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
# SPDX-License-Identifier: Apache-2.0
ਆਯਾਤ pytest
ਤਃ pytest_embedded ਆਯਾਤ Dut
ਤਃ pytest_embedded_idf.utils ਆਯਾਤ idf_parametrize


@pytest.mark.generic
@idf_parametrize('config', ['clang'], ਅਸਿੱਧਾ=['config'])
@idf_parametrize('target', ['esp32', 'esp32s2', 'esp32s3'], ਅਸਿੱਧਾ=['target'])
ਪਰਿਭਾਸ਼ਾ test_task_self_delete(dut: Dut) -> ਕੁਝ_ਨਹੀਂ:
    dut.expect_exact('task_self_delete: PASS')
    # PASS is printed before main_task self-deletes. The heartbeat only runs
    # if that delete (and the child delete) did not panic.
    dut.expect_exact('task_self_delete: still running')
