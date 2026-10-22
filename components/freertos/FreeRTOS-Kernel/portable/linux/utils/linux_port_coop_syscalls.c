/*
 * SPDX-FileCopyrightText: 2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Cooperative syscall interposition for Linux/macOS FreeRTOS simulator.
 *
 * This file provides:
 *  - Shadow FD state table (user-visible blocking/non-blocking tracking)
 *  - Cooperative I/O primitives (freertos_linux_coop_read/write/open/close/…)
 *  - Weak POSIX symbol definitions (read/write/open/close/fcntl/select/…)
 *    that VFS can override with strong definitions
 *  - Socket, scatter/gather, multiplexing, sleep wrappers
 *
 * Real libc entry points are resolved via dlsym(RTLD_NEXT, …).
 */

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <stdarg.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <sys/uio.h>
#ಅನ್ತರ್ಭಾವ <sys/socket.h>
#ಅನ್ತರ್ಭಾವ <sys/select.h>
#ಅನ್ತರ್ಭಾವ <sys/time.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ <poll.h>
#ಅನ್ತರ್ಭಾವ <dlfcn.h>
#ಅನ್ತರ್ಭಾವ <pthread.h>

#ಅನ್ತರ್ಭಾವ "linux_port_coop_internal.h"
#ಅನ್ತರ್ಭಾವ "esp_private/freertos_linux_coop_syscalls.h"

ಸ್ಥಿರ ssize_t  (*real_readv)(ಪೂರ್ಣಾಙ್ಕ, ಧ್ರುವ ಸಂರಚನಾ iovec *, ಪೂರ್ಣಾಙ್ಕ);
ಸ್ಥಿರ ssize_t  (*real_writev)(ಪೂರ್ಣಾಙ್ಕ, ಧ್ರುವ ಸಂರಚನಾ iovec *, ಪೂರ್ಣಾಙ್ಕ);
ಸ್ಥಿರ ssize_t  (*real_recv)(ಪೂರ್ಣಾಙ್ಕ, ಶೂನ್ಯ *, size_t, ಪೂರ್ಣಾಙ್ಕ);
ಸ್ಥಿರ ssize_t  (*real_send)(ಪೂರ್ಣಾಙ್ಕ, ಧ್ರುವ ಶೂನ್ಯ *, size_t, ಪೂರ್ಣಾಙ್ಕ);
ಸ್ಥಿರ ssize_t  (*real_recvfrom)(ಪೂರ್ಣಾಙ್ಕ, ಶೂನ್ಯ *, size_t, ಪೂರ್ಣಾಙ್ಕ,
                                 ಸಂರಚನಾ sockaddr *, socklen_t *);
ಸ್ಥಿರ ssize_t  (*real_sendto)(ಪೂರ್ಣಾಙ್ಕ, ಧ್ರುವ ಶೂನ್ಯ *, size_t, ಪೂರ್ಣಾಙ್ಕ,
                                ಧ್ರುವ ಸಂರಚನಾ sockaddr *, socklen_t);
ಸ್ಥಿರ ssize_t  (*real_recvmsg)(ಪೂರ್ಣಾಙ್ಕ, ಸಂರಚನಾ msghdr *, ಪೂರ್ಣಾಙ್ಕ);
ಸ್ಥಿರ ssize_t  (*real_sendmsg)(ಪೂರ್ಣಾಙ್ಕ, ಧ್ರುವ ಸಂರಚನಾ msghdr *, ಪೂರ್ಣಾಙ್ಕ);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ      (*real_connect)(ಪೂರ್ಣಾಙ್ಕ, ಧ್ರುವ ಸಂರಚನಾ sockaddr *, socklen_t);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ      (*real_accept)(ಪೂರ್ಣಾಙ್ಕ, ಸಂರಚನಾ sockaddr *, socklen_t *);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ      (*real_pselect)(ಪೂರ್ಣಾಙ್ಕ, fd_set *, fd_set *, fd_set *,
                                ಧ್ರುವ ಸಂರಚನಾ timespec *, ಧ್ರುವ sigset_t *);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ      (*real_poll)(ಸಂರಚನಾ pollfd *, nfds_t, ಪೂರ್ಣಾಙ್ಕ);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ      (*real_socket)(ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ      (*real_socketpair)(ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ[2]);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ      (*real_pipe)(ಪೂರ್ಣಾಙ್ಕ[2]);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ      (*real_pipe2)(ಪೂರ್ಣಾಙ್ಕ[2], ಪೂರ್ಣಾಙ್ಕ);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ      (*real_dup)(ಪೂರ್ಣಾಙ್ಕ);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ      (*real_dup2)(ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ      (*real_nanosleep)(ಧ್ರುವ ಸಂರಚನಾ timespec *, ಸಂರಚನಾ timespec *);
ಸ್ಥಿರ ssize_t  (*real_read)(ಪೂರ್ಣಾಙ್ಕ, ಶೂನ್ಯ *, size_t);
ಸ್ಥಿರ ssize_t  (*real_write)(ಪೂರ್ಣಾಙ್ಕ, ಧ್ರುವ ಶೂನ್ಯ *, size_t);
ಸ್ಥಿರ ssize_t  (*real_pread)(ಪೂರ್ಣಾಙ್ಕ, ಶೂನ್ಯ *, size_t, off_t);
ಸ್ಥಿರ ssize_t  (*real_pwrite)(ಪೂರ್ಣಾಙ್ಕ, ಧ್ರುವ ಶೂನ್ಯ *, size_t, off_t);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ      (*real_close)(ಪೂರ್ಣಾಙ್ಕ);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ      (*real_select)(ಪೂರ್ಣಾಙ್ಕ, fd_set *, fd_set *, fd_set *,
                                ಸಂರಚನಾ timeval *);

ಪ್ರಕಾರನಿರ್ದೇಶ ಪೂರ್ಣಾಙ್ಕ (*linux_coop_fcntl_fn_t)(ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ, ...);
ಸ್ಥಿರ linux_coop_fcntl_fn_t real_fcntl;

ಪ್ರಕಾರನಿರ್ದೇಶ ಪೂರ್ಣಾಙ್ಕ (*linux_coop_open_fn_t)(ಧ್ರುವ ವರ್ಣ *, ಪೂರ್ಣಾಙ್ಕ, mode_t);
ಸ್ಥಿರ linux_coop_open_fn_t real_open;

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ linux_coop_fd_state {
    ಪೂರ್ಣಾಙ್ಕ fd;
    ಪೂರ್ಣಾಙ್ಕ user_flags;
    ಸಂರಚನಾ linux_coop_fd_state *next;
} linux_coop_fd_state_t;

ಸ್ಥಿರ linux_coop_fd_state_t *s_fd_state_list;
ಸ್ಥಿರ pthread_mutex_t s_fd_state_lock = PTHREAD_MUTEX_INITIALIZER;

ಸ್ಥಿರ linux_coop_fd_state_t *linux_coop_find_fd_locked(ಪೂರ್ಣಾಙ್ಕ fd)
{
    linux_coop_fd_state_t *ಗಂಟು = s_fd_state_list;
    ಯಾವತ್ (ಗಂಟು != ರಿಕ್ತ) {
        ಯದಿ (ಗಂಟು->fd == fd) {
            ಪ್ರತ್ಯಾಗಮ ಗಂಟು;
        }
        ಗಂಟು = ಗಂಟು->next;
    }
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ಶೂನ್ಯ __attribute__((constructor)) linux_coop_resolve_all(ಶೂನ್ಯ)
{
    LINUX_COOP_RESOLVE(readv);
    LINUX_COOP_RESOLVE(writev);
    LINUX_COOP_RESOLVE(recv);
    LINUX_COOP_RESOLVE(send);
    LINUX_COOP_RESOLVE(recvfrom);
    LINUX_COOP_RESOLVE(sendto);
    LINUX_COOP_RESOLVE(recvmsg);
    LINUX_COOP_RESOLVE(sendmsg);
    LINUX_COOP_RESOLVE(connect);
    LINUX_COOP_RESOLVE(accept);
    LINUX_COOP_RESOLVE(pselect);
    LINUX_COOP_RESOLVE(poll);
    LINUX_COOP_RESOLVE(socket);
    LINUX_COOP_RESOLVE(socketpair);
    LINUX_COOP_RESOLVE(pipe);
    LINUX_COOP_RESOLVE(pipe2);     /* may be NULL on macOS */
    LINUX_COOP_RESOLVE(dup);
    LINUX_COOP_RESOLVE(dup2);
    LINUX_COOP_RESOLVE(nanosleep);
    LINUX_COOP_RESOLVE(fcntl);
    LINUX_COOP_RESOLVE(ಪಠ);
    LINUX_COOP_RESOLVE(ಲಿಖ);
    LINUX_COOP_RESOLVE(pread);
    LINUX_COOP_RESOLVE(pwrite);
    LINUX_COOP_RESOLVE(close);
    LINUX_COOP_RESOLVE(select);
    LINUX_COOP_RESOLVE(open);
}

ಶೂನ್ಯ linux_coop_set_nonblocking(ಪೂರ್ಣಾಙ್ಕ fd)
{
    ಯದಿ (fd >= 0) {
        ಪೂರ್ಣಾಙ್ಕ flags = real_fcntl(fd, F_GETFL, 0);
        ಯದಿ (flags >= 0) {
            real_fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
    }
}

ಶೂನ್ಯ linux_coop_track_fd(ಪೂರ್ಣಾಙ್ಕ fd, ಪೂರ್ಣಾಙ್ಕ user_flags)
{
    ಯದಿ (fd < 0) {
        ಪ್ರತ್ಯಾಗಮ;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *ಗಂಟು = linux_coop_find_fd_locked(fd);
    ಯದಿ (ಗಂಟು != ರಿಕ್ತ) {
        ಗಂಟು->user_flags = user_flags;
        pthread_mutex_unlock(&s_fd_state_lock);
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಗಂಟು = calloc(1, ಪರಿಮಾಣ(*ಗಂಟು));
    ಯದಿ (ಗಂಟು != ರಿಕ್ತ) {
        ಗಂಟು->fd = fd;
        ಗಂಟು->user_flags = user_flags;
        ಗಂಟು->next = s_fd_state_list;
        s_fd_state_list = ಗಂಟು;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

ಶೂನ್ಯ linux_coop_untrack_fd(ಪೂರ್ಣಾಙ್ಕ fd)
{
    ಯದಿ (fd < 0) {
        ಪ್ರತ್ಯಾಗಮ;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *prev = ರಿಕ್ತ;
    linux_coop_fd_state_t *ಗಂಟು = s_fd_state_list;
    ಯಾವತ್ (ಗಂಟು != ರಿಕ್ತ) {
        ಯದಿ (ಗಂಟು->fd == fd) {
            ಯದಿ (prev == ರಿಕ್ತ) {
                s_fd_state_list = ಗಂಟು->next;
            } ಅನ್ಯಥಾ {
                prev->next = ಗಂಟು->next;
            }
            ಮುಕ್ತ(ಗಂಟು);
            ಭಙ್ಗ;
        }
        prev = ಗಂಟು;
        ಗಂಟು = ಗಂಟು->next;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

ಶೂನ್ಯ linux_coop_set_user_flags(ಪೂರ್ಣಾಙ್ಕ fd, ಪೂರ್ಣಾಙ್ಕ user_flags)
{
    linux_coop_track_fd(fd, user_flags);
}

bool linux_coop_get_user_flags(ಪೂರ್ಣಾಙ್ಕ fd, ಪೂರ್ಣಾಙ್ಕ *flags)
{
    ಯದಿ (fd < 0 || flags == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ false;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *ಗಂಟು = linux_coop_find_fd_locked(fd);
    ಯದಿ (ಗಂಟು == ರಿಕ್ತ) {
        pthread_mutex_unlock(&s_fd_state_lock);
        ಪ್ರತ್ಯಾಗಮ false;
    }
    *flags = ಗಂಟು->user_flags;
    pthread_mutex_unlock(&s_fd_state_lock);
    ಪ್ರತ್ಯಾಗಮ true;
}

bool linux_coop_fd_user_nonblocking(ಪೂರ್ಣಾಙ್ಕ fd)
{
    ಪೂರ್ಣಾಙ್ಕ user_flags = 0;
    ಯದಿ (linux_coop_get_user_flags(fd, &user_flags)) {
        ಪ್ರತ್ಯಾಗಮ (user_flags & O_NONBLOCK) != 0;
    }
    ಪ್ರತ್ಯಾಗಮ false;
}


ssize_t freertos_linux_coop_read(ಪೂರ್ಣಾಙ್ಕ fd, ಶೂನ್ಯ *buf, size_t ಎಣಿಕೆ)
{
    LINUX_COOP_IO_LOOP(fd, real_read(fd, buf, ಎಣಿಕೆ))
}

ssize_t freertos_linux_coop_write(ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ಶೂನ್ಯ *buf, size_t ಎಣಿಕೆ)
{
    LINUX_COOP_IO_LOOP(fd, real_write(fd, buf, ಎಣಿಕೆ))
}

ssize_t freertos_linux_coop_pread(ಪೂರ್ಣಾಙ್ಕ fd, ಶೂನ್ಯ *buf, size_t ಎಣಿಕೆ, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pread(fd, buf, ಎಣಿಕೆ, offset))
}

ssize_t freertos_linux_coop_pwrite(ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ಶೂನ್ಯ *buf, size_t ಎಣಿಕೆ, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pwrite(fd, buf, ಎಣಿಕೆ, offset))
}

ಪೂರ್ಣಾಙ್ಕ freertos_linux_coop_open(ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ flags, ಪೂರ್ಣಾಙ್ಕ mode)
{
    ಪೂರ್ಣಾಙ್ಕ fd = real_open(path, flags, (mode_t)mode);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, flags & O_NONBLOCK);
    ಪ್ರತ್ಯಾಗಮ fd;
}

ಪೂರ್ಣಾಙ್ಕ freertos_linux_coop_close(ಪೂರ್ಣಾಙ್ಕ fd)
{
    ಪೂರ್ಣಾಙ್ಕ ret = real_close(fd);
    ಯದಿ (ret == 0 || errno == EINTR) {
        /* On Linux, close() always releases the fd even when returning
         * EINTR.  Retrying would risk closing a fd reused by another
         * thread.  Treat EINTR as success. */
        linux_coop_untrack_fd(fd);
        ಪ್ರತ್ಯಾಗಮ 0;
    }
    ಪ್ರತ್ಯಾಗಮ -1;
}

ಪೂರ್ಣಾಙ್ಕ freertos_linux_coop_fcntl(ಪೂರ್ಣಾಙ್ಕ fd, ಪೂರ್ಣಾಙ್ಕ cmd, ಪೂರ್ಣಾಙ್ಕ arg)
{
    ಯದಿ (cmd == F_GETFL) {
        ಪೂರ್ಣಾಙ್ಕ real_flags = real_fcntl(fd, F_GETFL, 0);
        ಯದಿ (real_flags < 0) {
            ಪ್ರತ್ಯಾಗಮ -1;
        }
        ಪೂರ್ಣಾಙ್ಕ user_flags = 0;
        ಯದಿ (linux_coop_get_user_flags(fd, &user_flags)) {
            real_flags &= ~O_NONBLOCK;
            real_flags |= (user_flags & O_NONBLOCK);
        }
        ಪ್ರತ್ಯಾಗಮ real_flags;
    }
    ಯದಿ (cmd == F_SETFL) {
        linux_coop_set_user_flags(fd, arg);
        ಪೂರ್ಣಾಙ್ಕ internal_flags = arg | O_NONBLOCK;
        ಪ್ರತ್ಯಾಗಮ real_fcntl(fd, F_SETFL, internal_flags);
    }
    ಪ್ರತ್ಯಾಗಮ real_fcntl(fd, cmd, arg);
}

ಪೂರ್ಣಾಙ್ಕ freertos_linux_coop_select(ಪೂರ್ಣಾಙ್ಕ nfds, fd_set *readfds, fd_set *writefds,
                      fd_set *exceptfds, ಸಂರಚನಾ timeval *timeout)
{
    long timeout_ms = -1;
    ಯದಿ (timeout != ರಿಕ್ತ) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_usec + 999) / 1000;
        ಯದಿ (timeout_ms == 0) {
            ಪ್ರತ್ಯಾಗಮ real_select(nfds, readfds, writefds, exceptfds, timeout);
        }
    }

    /* real_select() mutates the fd_sets in place (clears them, then sets bits
     * for ready descriptors). To keep polling the same descriptors across
     * iterations we must snapshot the caller's sets and restore them on every
     * loop. */
    fd_set rfds_in, wfds_in, efds_in;
    ಯದಿ (readfds != ರಿಕ್ತ) {
        rfds_in = *readfds;
    }
    ಯದಿ (writefds != ರಿಕ್ತ) {
        wfds_in = *writefds;
    }
    ಯದಿ (exceptfds != ರಿಕ್ತ) {
        efds_in = *exceptfds;
    }

    long waited_ms = 0;
    ಯಾವತ್ (1) {
        ಯದಿ (readfds != ರಿಕ್ತ) {
            *readfds = rfds_in;
        }
        ಯದಿ (writefds != ರಿಕ್ತ) {
            *writefds = wfds_in;
        }
        ಯದಿ (exceptfds != ರಿಕ್ತ) {
            *exceptfds = efds_in;
        }

        ಸಂರಚನಾ timeval zero_tv = {0, 0};
        ಪೂರ್ಣಾಙ್ಕ ret = real_select(nfds, readfds, writefds, exceptfds, &zero_tv);
        ಯದಿ (ret != 0) {
            ಪ್ರತ್ಯಾಗಮ ret;
        }
        ಯದಿ (timeout_ms == 0) {
            ಪ್ರತ್ಯಾಗಮ 0;
        }
        ಯದಿ (timeout_ms > 0 && waited_ms >= timeout_ms) {
            ಪ್ರತ್ಯಾಗಮ 0;
        }
        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

ಶೂನ್ಯ freertos_linux_coop_syscalls_init(ಶೂನ್ಯ)
{
    linux_coop_set_nonblocking(STDIN_FILENO);
    linux_coop_set_nonblocking(STDOUT_FILENO);
    linux_coop_set_nonblocking(STDERR_FILENO);
    linux_coop_track_fd(STDIN_FILENO, 0);
    linux_coop_track_fd(STDOUT_FILENO, 0);
    linux_coop_track_fd(STDERR_FILENO, 0);
}

/* These are overridden by strong definitions in VFS when that component
 * is linked.  When VFS is absent the weak versions provide cooperative
 * behaviour directly via real libc. */

__attribute__((weak)) ssize_t ಪಠ(ಪೂರ್ಣಾಙ್ಕ fd, ಶೂನ್ಯ *buf, size_t ಎಣಿಕೆ)
{
    ಪ್ರತ್ಯಾಗಮ freertos_linux_coop_read(fd, buf, ಎಣಿಕೆ);
}

__attribute__((weak)) ssize_t ಲಿಖ(ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ಶೂನ್ಯ *buf, size_t ಎಣಿಕೆ)
{
    ಪ್ರತ್ಯಾಗಮ freertos_linux_coop_write(fd, buf, ಎಣಿಕೆ);
}

__attribute__((weak)) ssize_t pread(ಪೂರ್ಣಾಙ್ಕ fd, ಶೂನ್ಯ *buf, size_t ಎಣಿಕೆ, off_t offset)
{
    ಪ್ರತ್ಯಾಗಮ freertos_linux_coop_pread(fd, buf, ಎಣಿಕೆ, offset);
}

__attribute__((weak)) ssize_t pwrite(ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ಶೂನ್ಯ *buf, size_t ಎಣಿಕೆ, off_t offset)
{
    ಪ್ರತ್ಯಾಗಮ freertos_linux_coop_pwrite(fd, buf, ಎಣಿಕೆ, offset);
}

__attribute__((weak)) ಪೂರ್ಣಾಙ್ಕ open(ಧ್ರುವ ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ flags, ...)
{
    ಪೂರ್ಣಾಙ್ಕ mode = 0;
    ಯದಿ (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        mode = va_arg(ap, ಪೂರ್ಣಾಙ್ಕ);
        va_end(ap);
    }
    ಪ್ರತ್ಯಾಗಮ freertos_linux_coop_open(path, flags, mode);
}

__attribute__((weak)) ಪೂರ್ಣಾಙ್ಕ close(ಪೂರ್ಣಾಙ್ಕ fd)
{
    ಪ್ರತ್ಯಾಗಮ freertos_linux_coop_close(fd);
}

__attribute__((weak)) ಪೂರ್ಣಾಙ್ಕ fcntl(ಪೂರ್ಣಾಙ್ಕ fd, ಪೂರ್ಣಾಙ್ಕ cmd, ...)
{
    ಪೂರ್ಣಾಙ್ಕ arg = 0;
    ಯದಿ (cmd != F_GETFL) {
        va_list list;
        va_start(list, cmd);
        arg = va_arg(list, ಪೂರ್ಣಾಙ್ಕ);
        va_end(list);
    }
    ಪ್ರತ್ಯಾಗಮ freertos_linux_coop_fcntl(fd, cmd, arg);
}

__attribute__((weak)) ಪೂರ್ಣಾಙ್ಕ select(ಪೂರ್ಣಾಙ್ಕ nfds, fd_set *readfds, fd_set *writefds,
                                 fd_set *exceptfds, ಸಂರಚನಾ timeval *timeout)
{
    ಪ್ರತ್ಯಾಗಮ freertos_linux_coop_select(nfds, readfds, writefds, exceptfds, timeout);
}

ssize_t freertos_linux_coop_readv(ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ಸಂರಚನಾ iovec *iov, ಪೂರ್ಣಾಙ್ಕ iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_readv(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_writev(ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ಸಂರಚನಾ iovec *iov, ಪೂರ್ಣಾಙ್ಕ iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_writev(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_recv(ಪೂರ್ಣಾಙ್ಕ sockfd, ಶೂನ್ಯ *buf, size_t len, ಪೂರ್ಣಾಙ್ಕ flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recv(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_send(ಪೂರ್ಣಾಙ್ಕ sockfd, ಧ್ರುವ ಶೂನ್ಯ *buf, size_t len, ಪೂರ್ಣಾಙ್ಕ flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_send(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_recvfrom(ಪೂರ್ಣಾಙ್ಕ sockfd, ಶೂನ್ಯ *buf, size_t len, ಪೂರ್ಣಾಙ್ಕ flags,
                 ಸಂರಚನಾ sockaddr *src_addr, socklen_t *addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvfrom(sockfd, buf, len, flags | MSG_DONTWAIT,
                            src_addr, addrlen))
}

ssize_t freertos_linux_coop_sendto(ಪೂರ್ಣಾಙ್ಕ sockfd, ಧ್ರುವ ಶೂನ್ಯ *buf, size_t len, ಪೂರ್ಣಾಙ್ಕ flags,
               ಧ್ರುವ ಸಂರಚನಾ sockaddr *dest_addr, socklen_t addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendto(sockfd, buf, len, flags | MSG_DONTWAIT,
                          dest_addr, addrlen))
}

ssize_t freertos_linux_coop_recvmsg(ಪೂರ್ಣಾಙ್ಕ sockfd, ಸಂರಚನಾ msghdr *ಸಂದೇಶ, ಪೂರ್ಣಾಙ್ಕ flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvmsg(sockfd, ಸಂದೇಶ, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_sendmsg(ಪೂರ್ಣಾಙ್ಕ sockfd, ಧ್ರುವ ಸಂರಚನಾ msghdr *ಸಂದೇಶ, ಪೂರ್ಣಾಙ್ಕ flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendmsg(sockfd, ಸಂದೇಶ, flags | MSG_DONTWAIT))
}

__attribute__((weak)) ssize_t readv(ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ಸಂರಚನಾ iovec *iov, ಪೂರ್ಣಾಙ್ಕ iovcnt)
{
    ಪ್ರತ್ಯಾಗಮ freertos_linux_coop_readv(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t writev(ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ಸಂರಚನಾ iovec *iov, ಪೂರ್ಣಾಙ್ಕ iovcnt)
{
    ಪ್ರತ್ಯಾಗಮ freertos_linux_coop_writev(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t recv(ಪೂರ್ಣಾಙ್ಕ sockfd, ಶೂನ್ಯ *buf, size_t len, ಪೂರ್ಣಾಙ್ಕ flags)
{
    ಪ್ರತ್ಯಾಗಮ freertos_linux_coop_recv(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t send(ಪೂರ್ಣಾಙ್ಕ sockfd, ಧ್ರುವ ಶೂನ್ಯ *buf, size_t len, ಪೂರ್ಣಾಙ್ಕ flags)
{
    ಪ್ರತ್ಯಾಗಮ freertos_linux_coop_send(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t recvfrom(ಪೂರ್ಣಾಙ್ಕ sockfd, ಶೂನ್ಯ *buf, size_t len, ಪೂರ್ಣಾಙ್ಕ flags,
                 ಸಂರಚನಾ sockaddr *src_addr, socklen_t *addrlen)
{
    ಪ್ರತ್ಯಾಗಮ freertos_linux_coop_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

__attribute__((weak)) ssize_t sendto(ಪೂರ್ಣಾಙ್ಕ sockfd, ಧ್ರುವ ಶೂನ್ಯ *buf, size_t len, ಪೂರ್ಣಾಙ್ಕ flags,
               ಧ್ರುವ ಸಂರಚನಾ sockaddr *dest_addr, socklen_t addrlen)
{
    ಪ್ರತ್ಯಾಗಮ freertos_linux_coop_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

__attribute__((weak)) ssize_t recvmsg(ಪೂರ್ಣಾಙ್ಕ sockfd, ಸಂರಚನಾ msghdr *ಸಂದೇಶ, ಪೂರ್ಣಾಙ್ಕ flags)
{
    ಪ್ರತ್ಯಾಗಮ freertos_linux_coop_recvmsg(sockfd, ಸಂದೇಶ, flags);
}

__attribute__((weak)) ssize_t sendmsg(ಪೂರ್ಣಾಙ್ಕ sockfd, ಧ್ರುವ ಸಂರಚನಾ msghdr *ಸಂದೇಶ, ಪೂರ್ಣಾಙ್ಕ flags)
{
    ಪ್ರತ್ಯಾಗಮ freertos_linux_coop_sendmsg(sockfd, ಸಂದೇಶ, flags);
}

ಪೂರ್ಣಾಙ್ಕ freertos_linux_coop_connect(ಪೂರ್ಣಾಙ್ಕ sockfd, ಧ್ರುವ ಸಂರಚನಾ sockaddr *addr, socklen_t addrlen)
{
    ಯಾವತ್ (1)
    {
        ಪೂರ್ಣಾಙ್ಕ ret = real_connect(sockfd, addr, addrlen);
        ಯದಿ (ret == 0) {
            ಪ್ರತ್ಯಾಗಮ ret;
        }

        ಯದಿ (errno == EINPROGRESS || errno == EALREADY) {
            ಸಂರಚನಾ pollfd pfd;
            pfd.fd = sockfd;
            pfd.events = POLLOUT;
            pfd.revents = 0;

            ಪೂರ್ಣಾಙ್ಕ poll_retries = 0;
            ಯಾವತ್ (1)
            {
                ಪೂರ್ಣಾಙ್ಕ press = real_poll(&pfd, 1, 0);
                ಯದಿ (press > 0) {
                    ಪೂರ್ಣಾಙ್ಕ so_err = 0;
                    socklen_t len = ಪರಿಮಾಣ(so_err);
                    ಯದಿ (getsockopt(sockfd, SOL_SOCKET, SO_ERROR,
                                   &so_err, &len) < 0) {
                        ಪ್ರತ್ಯಾಗಮ -1;
                    }
                    ಯದಿ (so_err == 0) {
                        ಪ್ರತ್ಯಾಗಮ 0;
                    } ಅನ್ಯಥಾ {
                        errno = so_err;
                        ಪ್ರತ್ಯಾಗಮ -1;
                    }
                } ಅನ್ಯಥಾ ಯದಿ (press == 0) {
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    ಅನುವರ್ತ;
                } ಅನ್ಯಥಾ {
                    ಯದಿ (errno == EINTR) {
                        ಅನುವರ್ತ;
                    }
                    /* Non-EINTR poll error; bail out after a few retries */
                    ಯದಿ (++poll_retries > 3) {
                        ಪ್ರತ್ಯಾಗಮ -1;
                    }
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    ಅನುವರ್ತ;
                }
            }
        }

        ಯದಿ (errno == EINTR) {
            ಪ್ರತ್ಯಾಗಮ -1;
        }

        ಪ್ರತ್ಯಾಗಮ -1;
    }
}

ಪೂರ್ಣಾಙ್ಕ freertos_linux_coop_accept(ಪೂರ್ಣಾಙ್ಕ sockfd, ಸಂರಚನಾ sockaddr *addr, socklen_t *addrlen)
{
    ಯಾವತ್ (1)
    {
        ಪೂರ್ಣಾಙ್ಕ fd = real_accept(sockfd, addr, addrlen);
        ಯದಿ (fd >= 0) {
            linux_coop_set_nonblocking(fd);
            linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(sockfd) ? O_NONBLOCK : 0);
            ಪ್ರತ್ಯಾಗಮ fd;
        }
        ಯದಿ (errno == EAGAIN || errno == EWOULDBLOCK) {
            ಯದಿ (linux_coop_fd_user_nonblocking(sockfd)) {
                ಪ್ರತ್ಯಾಗಮ -1;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            ಅನುವರ್ತ;
        }
        ಪ್ರತ್ಯಾಗಮ -1;
    }
}

ಪೂರ್ಣಾಙ್ಕ freertos_linux_coop_pselect(ಪೂರ್ಣಾಙ್ಕ nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, ಧ್ರುವ ಸಂರಚನಾ timespec *timeout,
            ಧ್ರುವ sigset_t *sigmask)
{
    long timeout_ms = -1;
    ಯದಿ (timeout != ರಿಕ್ತ) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_nsec + 999999) / 1000000;
        ಯದಿ (timeout_ms == 0) {
            ಪ್ರತ್ಯಾಗಮ real_pselect(nfds, readfds, writefds, exceptfds,
                                timeout, sigmask);
        }
    }

    long waited_ms = 0;
    ಯಾವತ್ (1)
    {
        ಸಂರಚನಾ timespec zero_ts = {0, 0};
        ಪೂರ್ಣಾಙ್ಕ ret = real_pselect(nfds, readfds, writefds, exceptfds,
                               &zero_ts, sigmask);
        ಯದಿ (ret != 0) {
            ಪ್ರತ್ಯಾಗಮ ret;
        }

        ಯದಿ (timeout_ms == 0 ||
            (timeout_ms > 0 && waited_ms >= timeout_ms)) {
            ಪ್ರತ್ಯಾಗಮ 0;
        }

        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

ಪೂರ್ಣಾಙ್ಕ freertos_linux_coop_poll(ಸಂರಚನಾ pollfd *fds, nfds_t nfds, ಪೂರ್ಣಾಙ್ಕ timeout)
{
    ಯದಿ (timeout == 0) {
        ಪ್ರತ್ಯಾಗಮ real_poll(fds, nfds, 0);
    }

    long timeout_ms = -1;
    ಯದಿ (timeout > 0) {
        timeout_ms = timeout;
    }

    long waited_ms = 0;

    ಯಾವತ್ (1)
    {
        ಪೂರ್ಣಾಙ್ಕ ret = real_poll(fds, nfds, 0);
        ಯದಿ (ret > 0) {
            ಪ್ರತ್ಯಾಗಮ ret;
        } ಅನ್ಯಥಾ ಯದಿ (ret == 0) {
            ಯದಿ (timeout_ms == 0 ||
                (timeout_ms > 0 && waited_ms >= timeout_ms)) {
                ಪ್ರತ್ಯಾಗಮ 0;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            waited_ms += LINUX_COOP_TICK_MS;
            ಅನುವರ್ತ;
        } ಅನ್ಯಥಾ {
            ಯದಿ (errno == EINTR) {
                ಅನುವರ್ತ;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            ಯದಿ (timeout_ms > 0 && waited_ms >= timeout_ms) {
                ಪ್ರತ್ಯಾಗಮ -1;
            }
            waited_ms += LINUX_COOP_TICK_MS;
        }
    }
}

ಪೂರ್ಣಾಙ್ಕ nanosleep(ಧ್ರುವ ಸಂರಚನಾ timespec *req, ಸಂರಚನಾ timespec *rem)
{
    ಯದಿ (req == ರಿಕ್ತ) {
        errno = EFAULT;
        ಪ್ರತ್ಯಾಗಮ -1;
    }
    ಯದಿ (!linux_port_in_freertos_task()) {
        ಪ್ರತ್ಯಾಗಮ real_nanosleep(req, rem);
    }
    long ms = req->tv_sec * 1000 + (req->tv_nsec + 999999) / 1000000;
    ಯದಿ (ms <= 0) {
        ms = 1;
    }
    vTaskDelay(ms);
    ಯದಿ (rem) {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }
    ಪ್ರತ್ಯಾಗಮ 0;
}

unsigned ಪೂರ್ಣಾಙ್ಕ sleep(unsigned ಪೂರ್ಣಾಙ್ಕ seconds)
{
    linux_coop_yield(seconds * 1000);
    ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ usleep(useconds_t usec)
{
    linux_coop_yield(usec / 1000);
    ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ freertos_linux_coop_socket(ಪೂರ್ಣಾಙ್ಕ domain, ಪೂರ್ಣಾಙ್ಕ type, ಪೂರ್ಣಾಙ್ಕ protocol)
{
    ಪೂರ್ಣಾಙ್ಕ fd = real_socket(domain, type, protocol);
    linux_coop_set_nonblocking(fd);
#ifdef SOCK_NONBLOCK
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#ಅನ್ಯಥಾ
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ user_flags = 0;
#endif
    linux_coop_track_fd(fd, user_flags);
    ಪ್ರತ್ಯಾಗಮ fd;
}

ಪೂರ್ಣಾಙ್ಕ freertos_linux_coop_socketpair(ಪೂರ್ಣಾಙ್ಕ domain, ಪೂರ್ಣಾಙ್ಕ type, ಪೂರ್ಣಾಙ್ಕ protocol, ಪೂರ್ಣಾಙ್ಕ *sv)
{
    ಪೂರ್ಣಾಙ್ಕ ret = real_socketpair(domain, type, protocol, sv);
    ಯದಿ (ret == 0) {
        linux_coop_set_nonblocking(sv[0]);
        linux_coop_set_nonblocking(sv[1]);
#ifdef SOCK_NONBLOCK
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#ಅನ್ಯಥಾ
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ user_flags = 0;
#endif
        linux_coop_track_fd(sv[0], user_flags);
        linux_coop_track_fd(sv[1], user_flags);
    }
    ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ freertos_linux_coop_pipe(ಪೂರ್ಣಾಙ್ಕ *fds)
{
    ಪೂರ್ಣಾಙ್ಕ ret = real_pipe(fds);
    ಯದಿ (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        linux_coop_track_fd(fds[0], 0);
        linux_coop_track_fd(fds[1], 0);
    }
    ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ freertos_linux_coop_pipe2(ಪೂರ್ಣಾಙ್ಕ *fds, ಪೂರ್ಣಾಙ್ಕ flags)
{
    ಪೂರ್ಣಾಙ್ಕ ret = real_pipe2(fds, flags);
    ಯದಿ (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ user_flags = (flags & O_NONBLOCK) ? O_NONBLOCK : 0;
        linux_coop_track_fd(fds[0], user_flags);
        linux_coop_track_fd(fds[1], user_flags);
    }
    ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ freertos_linux_coop_dup(ಪೂರ್ಣಾಙ್ಕ oldfd)
{
    ಪೂರ್ಣಾಙ್ಕ fd = real_dup(oldfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    ಪ್ರತ್ಯಾಗಮ fd;
}

ಪೂರ್ಣಾಙ್ಕ freertos_linux_coop_dup2(ಪೂರ್ಣಾಙ್ಕ oldfd, ಪೂರ್ಣಾಙ್ಕ newfd)
{
    ಪೂರ್ಣಾಙ್ಕ fd = real_dup2(oldfd, newfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    ಪ್ರತ್ಯಾಗಮ fd;
}

__attribute__((weak)) ಪೂರ್ಣಾಙ್ಕ connect(ಪೂರ್ಣಾಙ್ಕ sockfd, ಧ್ರುವ ಸಂರಚನಾ sockaddr *addr, socklen_t addrlen)
{
    ಪ್ರತ್ಯಾಗಮ freertos_linux_coop_connect(sockfd, addr, addrlen);
}

__attribute__((weak)) ಪೂರ್ಣಾಙ್ಕ accept(ಪೂರ್ಣಾಙ್ಕ sockfd, ಸಂರಚನಾ sockaddr *addr, socklen_t *addrlen)
{
    ಪ್ರತ್ಯಾಗಮ freertos_linux_coop_accept(sockfd, addr, addrlen);
}

__attribute__((weak)) ಪೂರ್ಣಾಙ್ಕ pselect(ಪೂರ್ಣಾಙ್ಕ nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, ಧ್ರುವ ಸಂರಚನಾ timespec *timeout,
            ಧ್ರುವ sigset_t *sigmask)
{
    ಪ್ರತ್ಯಾಗಮ freertos_linux_coop_pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}

__attribute__((weak)) ಪೂರ್ಣಾಙ್ಕ poll(ಸಂರಚನಾ pollfd *fds, nfds_t nfds, ಪೂರ್ಣಾಙ್ಕ timeout)
{
    ಪ್ರತ್ಯಾಗಮ freertos_linux_coop_poll(fds, nfds, timeout);
}

__attribute__((weak)) ಪೂರ್ಣಾಙ್ಕ socket(ಪೂರ್ಣಾಙ್ಕ domain, ಪೂರ್ಣಾಙ್ಕ type, ಪೂರ್ಣಾಙ್ಕ protocol)
{
    ಪ್ರತ್ಯಾಗಮ freertos_linux_coop_socket(domain, type, protocol);
}

__attribute__((weak)) ಪೂರ್ಣಾಙ್ಕ socketpair(ಪೂರ್ಣಾಙ್ಕ domain, ಪೂರ್ಣಾಙ್ಕ type, ಪೂರ್ಣಾಙ್ಕ protocol, ಪೂರ್ಣಾಙ್ಕ sv[2])
{
    ಪ್ರತ್ಯಾಗಮ freertos_linux_coop_socketpair(domain, type, protocol, sv);
}

__attribute__((weak)) ಪೂರ್ಣಾಙ್ಕ pipe(ಪೂರ್ಣಾಙ್ಕ fds[2])
{
    ಪ್ರತ್ಯಾಗಮ freertos_linux_coop_pipe(fds);
}

__attribute__((weak)) ಪೂರ್ಣಾಙ್ಕ pipe2(ಪೂರ್ಣಾಙ್ಕ fds[2], ಪೂರ್ಣಾಙ್ಕ flags)
{
    ಪ್ರತ್ಯಾಗಮ freertos_linux_coop_pipe2(fds, flags);
}

__attribute__((weak)) ಪೂರ್ಣಾಙ್ಕ dup(ಪೂರ್ಣಾಙ್ಕ oldfd)
{
    ಪ್ರತ್ಯಾಗಮ freertos_linux_coop_dup(oldfd);
}

__attribute__((weak)) ಪೂರ್ಣಾಙ್ಕ dup2(ಪೂರ್ಣಾಙ್ಕ oldfd, ಪೂರ್ಣಾಙ್ಕ newfd)
{
    ಪ್ರತ್ಯಾಗಮ freertos_linux_coop_dup2(oldfd, newfd);
}
