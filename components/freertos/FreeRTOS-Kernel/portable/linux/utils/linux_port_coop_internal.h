/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Internal header for FreeRTOS Linux cooperative syscall wrappers.
 * Not for use outside the FreeRTOS portable layer.
 */

#pragma once

#অন্তর্ভাৱ <errno.h>
#অন্তর্ভাৱ <time.h>
#অন্তর্ভাৱ <dlfcn.h>
#অন্তর্ভাৱ "freertos/FreeRTOS.h"
#অন্তর্ভাৱ "freertos/task.h"

#ifdef __cplusplus
বাহ্য "C" {
#endif

/**
 * @brief Cooperative tick period in milliseconds.
 */
#define LINUX_COOP_TICK_MS   (1000 / CONFIG_FREERTOS_HZ)

/**
 * @brief Resolve real libc symbol into a file-static function pointer.
 *
 * Use inside a constructor function.  real_<name> must be declared as
 * a file-static variable of the correct function-pointer type.
 *
 * @param name Name of the libc symbol to resolve.
 */
#define LINUX_COOP_RESOLVE(name) real_##name = dlsym(RTLD_NEXT, #name)

/**
 * @brief Check whether the calling thread is a FreeRTOS task pthread.
 *
 * @return true if called from a FreeRTOS task, false otherwise.
 */
bool linux_port_in_freertos_task(শূন্য);

/**
 * @brief Set a kernel file descriptor to O_NONBLOCK mode.
 *
 * @param fd File descriptor to make non-blocking.
 */
শূন্য linux_coop_set_nonblocking(পূর্ণাঙ্ক fd);

/**
 * @brief Track a file descriptor in the cooperative mode table.
 *
 * Real kernel FDs are kept non-blocking internally. This function stores the
 * user-visible file status flags so wrappers can preserve POSIX blocking/
 * non-blocking semantics.
 *
 * @param fd         File descriptor to track.
 * @param user_flags User-visible status flags (as passed to fcntl F_SETFL).
 */
শূন্য linux_coop_track_fd(পূর্ণাঙ্ক fd, পূর্ণাঙ্ক user_flags);

/**
 * @brief Stop tracking a file descriptor in the cooperative mode table.
 *
 * @param fd File descriptor to untrack.
 */
শূন্য linux_coop_untrack_fd(পূর্ণাঙ্ক fd);

/**
 * @brief Update user-visible status flags for a tracked file descriptor.
 *
 * @param fd         File descriptor to update.
 * @param user_flags New user-visible status flags.
 */
শূন্য linux_coop_set_user_flags(পূর্ণাঙ্ক fd, পূর্ণাঙ্ক user_flags);

/**
 * @brief Query whether user-visible mode is non-blocking for a file descriptor.
 *
 * @param fd File descriptor to query.
 * @return true if user-visible mode is non-blocking, false otherwise.
 */
bool linux_coop_fd_user_nonblocking(পূর্ণাঙ্ক fd);

/**
 * @brief Get tracked user-visible status flags for a file descriptor.
 *
 * @param[in]  fd    File descriptor to query.
 * @param[out] flags Output for tracked user-visible flags.
 * @return true if the fd is tracked and flags were written, false otherwise.
 */
bool linux_coop_get_user_flags(পূর্ণাঙ্ক fd, পূর্ণাঙ্ক *flags);

/**
 * @brief Cooperatively yield for @p ms milliseconds.
 *
 * Maps to vTaskDelay() inside a FreeRTOS task, nanosleep() otherwise.
 *
 * @param ms Number of milliseconds to yield.
 */
স্থির অন্তঃস্থ __attribute__((always_inline))
শূন্য linux_coop_yield(পূর্ণাঙ্ক ms)
{
    যদি (linux_port_in_freertos_task()) {
        vTaskDelay(pdMS_TO_TICKS(ms));
    } অন্যথা {
        সংরচনা timespec ts;
        ts.tv_sec  = ms / 1000;
        ts.tv_nsec = (ms % 1000) * 1000000L;
        যেতিয়ালৈ (nanosleep(&ts, &ts) == -1 && errno == EINTR) {
            /* retry with remaining time */
        }
    }
}

/**
 * @brief Cooperative I/O retry loop.
 *
 * If user-visible mode is non-blocking, returns EAGAIN/EWOULDBLOCK as-is.
 * Otherwise yields and retries until success or a real error.
 *
 * @param fd   File descriptor used to determine user-visible blocking mode.
 * @param expr Expression to evaluate in the retry loop (must yield ssize_t).
 */
#define LINUX_COOP_IO_LOOP(fd, ৰাশিমালা)                           \
    যেতিয়ালৈ (1)                                                  \
    {                                                          \
        ssize_t _n = (ৰাশিমালা);                                   \
        যদি (_n >= 0) {                                         \
            ঘূৰাই _n;                                         \
        }                                                      \
        যদি (errno == EAGAIN || errno == EWOULDBLOCK) {         \
            যদি (linux_coop_fd_user_nonblocking(fd)) {          \
                ঘূৰাই -1;                                     \
            }                                                  \
            linux_coop_yield(LINUX_COOP_TICK_MS);              \
            অনুৱর্ত;                                          \
        }                                                      \
        ঘূৰাই -1;                                             \
    }

#ifdef __cplusplus
}
#endif
