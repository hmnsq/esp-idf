# SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
# SPDX-License-Identifier: Apache-2.0
ಆಮದು pytest
ತಃ pytest_embedded ಆಮದು Dut
ತಃ pytest_embedded_idf.utils ಆಮದು idf_parametrize


@pytest.mark.generic
@idf_parametrize('config', ['clang'], ಪರೋಕ್ಷ=['config'])
@idf_parametrize('target', ['esp32', 'esp32s2', 'esp32s3'], ಪರೋಕ್ಷ=['target'])
ವ್ಯಾಖ್ಯಾನ test_task_self_delete(dut: Dut) -> ಏನೂಇಲ್ಲ:
    dut.expect_exact('task_self_delete: PASS')
    # PASS is printed before main_task self-deletes. The heartbeat only runs
    # if that delete (and the child delete) did not panic.
    dut.expect_exact('task_self_delete: still running')
