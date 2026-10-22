# SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
# SPDX-License-Identifier: Apache-2.0
దిగుమతి pytest
తః pytest_embedded దిగుమతి Dut
తః pytest_embedded_idf.utils దిగుమతి idf_parametrize


@pytest.mark.generic
@idf_parametrize('config', ['clang'], పరోక్ష=['config'])
@idf_parametrize('target', ['esp32', 'esp32s2', 'esp32s3'], పరోక్ష=['target'])
నిర్వచనం test_task_self_delete(dut: Dut) -> ఏమీలేదు:
    dut.expect_exact('task_self_delete: PASS')
    # PASS is printed before main_task self-deletes. The heartbeat only runs
    # if that delete (and the child delete) did not panic.
    dut.expect_exact('task_self_delete: still running')
