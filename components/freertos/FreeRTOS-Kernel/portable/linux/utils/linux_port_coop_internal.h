/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Internal header for FreeRTOS Linux cooperative syscall wrappers.
 * Not for use outside the FreeRTOS portable layer.
 */

#సూచన once

#అన్తర్భావ <errno.h>
#అన్తర్భావ <time.h>
#అన్తర్భావ <dlfcn.h>
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"

#నిర్వచితమైతే __cplusplus
బాహ్య "C" {
#అయితే_ముగింపు

/**
 * @brief Cooperative tick period in milliseconds.
 */
#నిర్వచించు LINUX_COOP_TICK_MS   (1000 / CONFIG_FREERTOS_HZ)

/**
 * @brief Resolve real libc symbol into a file-static function pointer.
 *
 * Use inside a constructor function.  real_<name> must be declared as
 * a file-static variable of the correct function-pointer type.
 *
 * @param name Name of the libc symbol to resolve.
 */
#నిర్వచించు LINUX_COOP_RESOLVE(name) real_##name = dlsym(RTLD_NEXT, #name)

/**
 * @brief Check whether the calling thread is a FreeRTOS task pthread.
 *
 * @return true if called from a FreeRTOS task, false otherwise.
 */
bool linux_పోర్టు_లోపల_freertos_task(శూన్య);

/**
 * @brief Set a kernel file descriptor to O_NONBLOCK mode.
 *
 * @param fd File descriptor to make non-blocking.
 */
శూన్య linux_coop_సమితి_nonblocking(పూర్ణాంకం fd);

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
శూన్య linux_coop_track_fd(పూర్ణాంకం fd, పూర్ణాంకం user_flags);

/**
 * @brief Stop tracking a file descriptor in the cooperative mode table.
 *
 * @param fd File descriptor to untrack.
 */
శూన్య linux_coop_untrack_fd(పూర్ణాంకం fd);

/**
 * @brief Update user-visible status flags for a tracked file descriptor.
 *
 * @param fd         File descriptor to update.
 * @param user_flags New user-visible status flags.
 */
శూన్య linux_coop_సమితి_user_flags(పూర్ణాంకం fd, పూర్ణాంకం user_flags);

/**
 * @brief Query whether user-visible mode is non-blocking for a file descriptor.
 *
 * @param fd File descriptor to query.
 * @return true if user-visible mode is non-blocking, false otherwise.
 */
bool linux_coop_fd_user_nonblocking(పూర్ణాంకం fd);

/**
 * @brief Get tracked user-visible status flags for a file descriptor.
 *
 * @param[in]  fd    File descriptor to query.
 * @param[out] flags Output for tracked user-visible flags.
 * @return true if the fd is tracked and flags were written, false otherwise.
 */
bool linux_coop_get_user_flags(పూర్ణాంకం fd, పూర్ణాంకం *flags);

/**
 * @brief Cooperatively yield for @p ms milliseconds.
 *
 * Maps to vTaskDelay() inside a FreeRTOS task, nanosleep() otherwise.
 *
 * @param ms Number of milliseconds to yield.
 */
స్థిర అంతఃవరుస __attribute__((always_inline))
శూన్య linux_coop_yield(పూర్ణాంకం ms)
{
    ఒకవేళ (linux_పోర్టు_లోపల_freertos_task()) {
        vTaskDelay(pd_MS_గమ్యం_TICKS(ms));
    } లేకపోతే {
        నిర్మాణం timespec ts;
        ts.tv_sec  = ms / 1000;
        ts.tv_nsec = (ms % 1000) * 1000000L;
        అయితే (nanosleep(&ts, &ts) == -1 && errno == EINTR) {
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
#నిర్వచించు LINUX_COOP_IO_LOOP(fd, సమాసం)                           \
    అయితే (1)                                                  \
    {                                                          \
        ssize_t _n = (సమాసం);                                   \
        ఒకవేళ (_n >= 0) {                                         \
            తిరిగి _n;                                         \
        }                                                      \
        ఒకవేళ (errno == EAGAIN || errno == EWOULDBLOCK) {         \
            ఒకవేళ (linux_coop_fd_user_nonblocking(fd)) {          \
                తిరిగి -1;                                     \
            }                                                  \
            linux_coop_yield(LINUX_COOP_TICK_MS);              \
            కొనసాగు;                                          \
        }                                                      \
        తిరిగి -1;                                             \
    }

#నిర్వచితమైతే __cplusplus
}
#అయితే_ముగింపు
