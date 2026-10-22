/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#அந்தர்பாவ <pthread.h>

#ifdef __cplusplus
வெளி "C" {
#endif

வகைவரை கட்டமைப்பு thread *thread_hdl;

வெறுமை linux_port_setup_backtrace_signal(வெறுமை);
வெறுமை linux_port_print_backtrace(வெறுமை);
pthread_t linux_port_get_scheduled_task_pthread(வெறுமை);

#ifdef __cplusplus
}
#endif
