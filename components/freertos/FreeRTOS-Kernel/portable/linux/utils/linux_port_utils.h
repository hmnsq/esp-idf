/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 * SPDX-License-Identifier: Apache-2.0
 */

#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ <pthread.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
ਬਾਹਰੀ "C" {
#ਜੇ_ਅੰਤ

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ thread *thread_hdl;

ਖਾਲੀ linux_ਪੋਰਟ_ਤਿਆਰੀ_backtrace_signal(ਖਾਲੀ);
ਖਾਲੀ linux_ਪੋਰਟ_print_backtrace(ਖਾਲੀ);
pthread_t linux_ਪੋਰਟ_get_scheduled_task_pthread(ਖਾਲੀ);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
}
#ਜੇ_ਅੰਤ
