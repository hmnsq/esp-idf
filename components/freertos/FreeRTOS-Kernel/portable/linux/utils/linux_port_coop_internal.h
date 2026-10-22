/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Internal header for FreeRTOS Linux cooperative syscall wrappers.
 * Not for use outside the FreeRTOS portable layer.
 */

#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <dlfcn.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
ਬਾਹਰੀ "C" {
#ਜੇ_ਅੰਤ

/**
 * @brief Cooperative tick period in milliseconds.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_COOP_TICK_MS   (1000 / CONFIG_FREERTOS_HZ)

/**
 * @brief Resolve real libc symbol into a file-static function pointer.
 *
 * Use inside a constructor function.  real_<name> must be declared as
 * a file-static variable of the correct function-pointer type.
 *
 * @param name Name of the libc symbol to resolve.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_COOP_RESOLVE(name) real_##name = dlsym(RTLD_NEXT, #name)

/**
 * @brief Check whether the calling thread is a FreeRTOS task pthread.
 *
 * @return true if called from a FreeRTOS task, false otherwise.
 */
bool linux_ਪੋਰਟ_ਵਿੱਚ_freertos_task(ਖਾਲੀ);

/**
 * @brief Set a kernel file descriptor to O_NONBLOCK mode.
 *
 * @param fd File descriptor to make non-blocking.
 */
ਖਾਲੀ linux_coop_ਸੈੱਟ_nonblocking(ਪੂਰਨ fd);

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
ਖਾਲੀ linux_coop_track_fd(ਪੂਰਨ fd, ਪੂਰਨ user_flags);

/**
 * @brief Stop tracking a file descriptor in the cooperative mode table.
 *
 * @param fd File descriptor to untrack.
 */
ਖਾਲੀ linux_coop_untrack_fd(ਪੂਰਨ fd);

/**
 * @brief Update user-visible status flags for a tracked file descriptor.
 *
 * @param fd         File descriptor to update.
 * @param user_flags New user-visible status flags.
 */
ਖਾਲੀ linux_coop_ਸੈੱਟ_user_flags(ਪੂਰਨ fd, ਪੂਰਨ user_flags);

/**
 * @brief Query whether user-visible mode is non-blocking for a file descriptor.
 *
 * @param fd File descriptor to query.
 * @return true if user-visible mode is non-blocking, false otherwise.
 */
bool linux_coop_fd_user_nonblocking(ਪੂਰਨ fd);

/**
 * @brief Get tracked user-visible status flags for a file descriptor.
 *
 * @param[in]  fd    File descriptor to query.
 * @param[out] flags Output for tracked user-visible flags.
 * @return true if the fd is tracked and flags were written, false otherwise.
 */
bool linux_coop_get_user_flags(ਪੂਰਨ fd, ਪੂਰਨ *flags);

/**
 * @brief Cooperatively yield for @p ms milliseconds.
 *
 * Maps to vTaskDelay() inside a FreeRTOS task, nanosleep() otherwise.
 *
 * @param ms Number of milliseconds to yield.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ __attribute__((always_inline))
ਖਾਲੀ linux_coop_yield(ਪੂਰਨ ms)
{
    ਜੇ (linux_ਪੋਰਟ_ਵਿੱਚ_freertos_task()) {
        vTaskDelay(pd_MS_ਮੰਜ਼ਿਲ_TICKS(ms));
    } ਨਹੀਂਤਾਂ {
        ਬਣਤਰ timespec ts;
        ts.tv_sec  = ms / 1000;
        ts.tv_nsec = (ms % 1000) * 1000000L;
        ਜਦੋਂ (nanosleep(&ts, &ts) == -1 && errno == EINTR) {
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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_COOP_IO_LOOP(fd, ਵਿਅੰਜਕ)                           \
    ਜਦੋਂ (1)                                                  \
    {                                                          \
        ssize_t _n = (ਵਿਅੰਜਕ);                                   \
        ਜੇ (_n >= 0) {                                         \
            ਵਾਪਸ _n;                                         \
        }                                                      \
        ਜੇ (errno == EAGAIN || errno == EWOULDBLOCK) {         \
            ਜੇ (linux_coop_fd_user_nonblocking(fd)) {          \
                ਵਾਪਸ -1;                                     \
            }                                                  \
            linux_coop_yield(LINUX_COOP_TICK_MS);              \
            ਜਾਰੀ;                                          \
        }                                                      \
        ਵਾਪਸ -1;                                             \
    }

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
}
#ਜੇ_ਅੰਤ
