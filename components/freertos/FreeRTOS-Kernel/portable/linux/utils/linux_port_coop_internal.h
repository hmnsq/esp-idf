/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Internal header for FreeRTOS Linux cooperative syscall wrappers.
 * Not for use outside the FreeRTOS portable layer.
 */

#निर्देशः once

#अन्तर्भाव <errno.h>
#अन्तर्भाव <time.h>
#अन्तर्भाव <dlfcn.h>
#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"

#यदि_परिभाषितम् __cplusplus
बाह्य "C" {
#यदि_अन्तः

/**
 * @brief Cooperative tick period in milliseconds.
 */
#परिभाषय LINUX_COOP_TICK_MS   (1000 / CONFIG_FREERTOS_HZ)

/**
 * @brief Resolve real libc symbol into a file-static function pointer.
 *
 * Use inside a constructor function.  real_<name> must be declared as
 * a file-static variable of the correct function-pointer type.
 *
 * @param name Name of the libc symbol to resolve.
 */
#परिभाषय LINUX_COOP_RESOLVE(name) real_##name = dlsym(RTLD_NEXT, #name)

/**
 * @brief Check whether the calling thread is a FreeRTOS task pthread.
 *
 * @return true if called from a FreeRTOS task, false otherwise.
 */
bool linux_पत्तनम्_अन्तर्_freertos_task(शून्य);

/**
 * @brief Set a kernel file descriptor to O_NONBLOCK mode.
 *
 * @param fd File descriptor to make non-blocking.
 */
शून्य linux_coop_समुच्चयः_nonblocking(पूर्णाङ्क fd);

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
शून्य linux_coop_track_fd(पूर्णाङ्क fd, पूर्णाङ्क user_flags);

/**
 * @brief Stop tracking a file descriptor in the cooperative mode table.
 *
 * @param fd File descriptor to untrack.
 */
शून्य linux_coop_untrack_fd(पूर्णाङ्क fd);

/**
 * @brief Update user-visible status flags for a tracked file descriptor.
 *
 * @param fd         File descriptor to update.
 * @param user_flags New user-visible status flags.
 */
शून्य linux_coop_समुच्चयः_user_flags(पूर्णाङ्क fd, पूर्णाङ्क user_flags);

/**
 * @brief Query whether user-visible mode is non-blocking for a file descriptor.
 *
 * @param fd File descriptor to query.
 * @return true if user-visible mode is non-blocking, false otherwise.
 */
bool linux_coop_fd_user_nonblocking(पूर्णाङ्क fd);

/**
 * @brief Get tracked user-visible status flags for a file descriptor.
 *
 * @param[in]  fd    File descriptor to query.
 * @param[out] flags Output for tracked user-visible flags.
 * @return true if the fd is tracked and flags were written, false otherwise.
 */
bool linux_coop_get_user_flags(पूर्णाङ्क fd, पूर्णाङ्क *flags);

/**
 * @brief Cooperatively yield for @p ms milliseconds.
 *
 * Maps to vTaskDelay() inside a FreeRTOS task, nanosleep() otherwise.
 *
 * @param ms Number of milliseconds to yield.
 */
स्थिर अन्तःस्थ __attribute__((always_inline))
शून्य linux_coop_yield(पूर्णाङ्क ms)
{
    यदि (linux_पत्तनम्_अन्तर्_freertos_task()) {
        vTaskDelay(pd_MS_गन्तव्यम्_TICKS(ms));
    } अन्यथा {
        संरचना timespec ts;
        ts.tv_sec  = ms / 1000;
        ts.tv_nsec = (ms % 1000) * 1000000L;
        यावत् (nanosleep(&ts, &ts) == -1 && errno == EINTR) {
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
#परिभाषय LINUX_COOP_IO_LOOP(fd, व्यञ्जकम्)                           \
    यावत् (1)                                                  \
    {                                                          \
        ssize_t _n = (व्यञ्जकम्);                                   \
        यदि (_n >= 0) {                                         \
            प्रत्यागम _n;                                         \
        }                                                      \
        यदि (errno == EAGAIN || errno == EWOULDBLOCK) {         \
            यदि (linux_coop_fd_user_nonblocking(fd)) {          \
                प्रत्यागम -1;                                     \
            }                                                  \
            linux_coop_yield(LINUX_COOP_TICK_MS);              \
            अनुवर्त;                                          \
        }                                                      \
        प्रत्यागम -1;                                             \
    }

#यदि_परिभाषितम् __cplusplus
}
#यदि_अन्तः
