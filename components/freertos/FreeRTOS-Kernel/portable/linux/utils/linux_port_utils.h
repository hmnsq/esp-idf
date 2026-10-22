/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#અન્તર્ભાવ <pthread.h>

#ifdef __cplusplus
બાહ્ય "C" {
#endif

પ્રકારવ્યાખ્યા સંરચના thread *thread_hdl;

શૂન્ય linux_port_setup_backtrace_signal(શૂન્ય);
શૂન્ય linux_port_print_backtrace(શૂન્ય);
pthread_t linux_port_get_scheduled_task_pthread(શૂન્ય);

#ifdef __cplusplus
}
#endif
