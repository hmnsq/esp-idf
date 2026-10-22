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

ಸ್ಥಿರ ssize_t  (*real_readv)(ಪೂರ್ಣಾಂಕ, ಸ್ಥಿರಾಂಕ ರಚನೆ iovec *, ಪೂರ್ಣಾಂಕ);
ಸ್ಥಿರ ssize_t  (*real_writev)(ಪೂರ್ಣಾಂಕ, ಸ್ಥಿರಾಂಕ ರಚನೆ iovec *, ಪೂರ್ಣಾಂಕ);
ಸ್ಥಿರ ssize_t  (*real_recv)(ಪೂರ್ಣಾಂಕ, ಶೂನ್ಯ *, size_t, ಪೂರ್ಣಾಂಕ);
ಸ್ಥಿರ ssize_t  (*real_send)(ಪೂರ್ಣಾಂಕ, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *, size_t, ಪೂರ್ಣಾಂಕ);
ಸ್ಥಿರ ssize_t  (*real_recvfrom)(ಪೂರ್ಣಾಂಕ, ಶೂನ್ಯ *, size_t, ಪೂರ್ಣಾಂಕ,
                                 ರಚನೆ sockaddr *, socklen_t *);
ಸ್ಥಿರ ssize_t  (*real_sendto)(ಪೂರ್ಣಾಂಕ, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *, size_t, ಪೂರ್ಣಾಂಕ,
                                ಸ್ಥಿರಾಂಕ ರಚನೆ sockaddr *, socklen_t);
ಸ್ಥಿರ ssize_t  (*real_recvmsg)(ಪೂರ್ಣಾಂಕ, ರಚನೆ msghdr *, ಪೂರ್ಣಾಂಕ);
ಸ್ಥಿರ ssize_t  (*real_sendmsg)(ಪೂರ್ಣಾಂಕ, ಸ್ಥಿರಾಂಕ ರಚನೆ msghdr *, ಪೂರ್ಣಾಂಕ);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      (*real_connect)(ಪೂರ್ಣಾಂಕ, ಸ್ಥಿರಾಂಕ ರಚನೆ sockaddr *, socklen_t);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      (*real_accept)(ಪೂರ್ಣಾಂಕ, ರಚನೆ sockaddr *, socklen_t *);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      (*real_pselect)(ಪೂರ್ಣಾಂಕ, fd_set *, fd_set *, fd_set *,
                                ಸ್ಥಿರಾಂಕ ರಚನೆ timespec *, ಸ್ಥಿರಾಂಕ sigset_t *);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      (*real_poll)(ರಚನೆ pollfd *, nfds_t, ಪೂರ್ಣಾಂಕ);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      (*real_socket)(ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      (*real_socketpair)(ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ[2]);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      (*real_pipe)(ಪೂರ್ಣಾಂಕ[2]);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      (*real_pipe2)(ಪೂರ್ಣಾಂಕ[2], ಪೂರ್ಣಾಂಕ);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      (*real_dup)(ಪೂರ್ಣಾಂಕ);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      (*real_dup2)(ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      (*real_nanosleep)(ಸ್ಥಿರಾಂಕ ರಚನೆ timespec *, ರಚನೆ timespec *);
ಸ್ಥಿರ ssize_t  (*real_read)(ಪೂರ್ಣಾಂಕ, ಶೂನ್ಯ *, size_t);
ಸ್ಥಿರ ssize_t  (*real_write)(ಪೂರ್ಣಾಂಕ, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *, size_t);
ಸ್ಥಿರ ssize_t  (*real_pread)(ಪೂರ್ಣಾಂಕ, ಶೂನ್ಯ *, size_t, off_t);
ಸ್ಥಿರ ssize_t  (*real_pwrite)(ಪೂರ್ಣಾಂಕ, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *, size_t, off_t);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      (*real_close)(ಪೂರ್ಣಾಂಕ);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ      (*real_select)(ಪೂರ್ಣಾಂಕ, fd_set *, fd_set *, fd_set *,
                                ರಚನೆ timeval *);

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ (*linux_coop_fcntl_fn_t)(ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ, ...);
ಸ್ಥಿರ linux_coop_fcntl_fn_t real_fcntl;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ (*linux_coop_open_fn_t)(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *, ಪೂರ್ಣಾಂಕ, mode_t);
ಸ್ಥಿರ linux_coop_open_fn_t real_open;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ linux_coop_fd_state {
    ಪೂರ್ಣಾಂಕ fd;
    ಪೂರ್ಣಾಂಕ user_flags;
    ರಚನೆ linux_coop_fd_state *ಮುಂದಿನ;
} linux_coop_fd_state_t;

ಸ್ಥಿರ linux_coop_fd_state_t *s_fd_state_list;
ಸ್ಥಿರ pthread_mutex_t s_fd_state_ಬೀಗ = PTHREAD_MUTEX_INITIALIZER;

ಸ್ಥಿರ linux_coop_fd_state_t *linux_coop_ಕಂಡುಹಿಡಿ_fd_locked(ಪೂರ್ಣಾಂಕ fd)
{
    linux_coop_fd_state_t *ಗಂಟು = s_fd_state_list;
    ಆಗ (ಗಂಟು != ಖಾಲಿ) {
        ಆದರೆ (ಗಂಟು->fd == fd) {
            ಹಿಂತಿರುಗಿ ಗಂಟು;
        }
        ಗಂಟು = ಗಂಟು->ಮುಂದಿನ;
    }
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
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

ಶೂನ್ಯ linux_coop_ಗಣ_nonblocking(ಪೂರ್ಣಾಂಕ fd)
{
    ಆದರೆ (fd >= 0) {
        ಪೂರ್ಣಾಂಕ flags = real_fcntl(fd, F_GETFL, 0);
        ಆದರೆ (flags >= 0) {
            real_fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
    }
}

ಶೂನ್ಯ linux_coop_track_fd(ಪೂರ್ಣಾಂಕ fd, ಪೂರ್ಣಾಂಕ user_flags)
{
    ಆದರೆ (fd < 0) {
        ಹಿಂತಿರುಗಿ;
    }

    pthread_mutex_lock(&s_fd_state_ಬೀಗ);
    linux_coop_fd_state_t *ಗಂಟು = linux_coop_ಕಂಡುಹಿಡಿ_fd_locked(fd);
    ಆದರೆ (ಗಂಟು != ಖಾಲಿ) {
        ಗಂಟು->user_flags = user_flags;
        pthread_mutex_unlock(&s_fd_state_ಬೀಗ);
        ಹಿಂತಿರುಗಿ;
    }

    ಗಂಟು = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(*ಗಂಟು));
    ಆದರೆ (ಗಂಟು != ಖಾಲಿ) {
        ಗಂಟು->fd = fd;
        ಗಂಟು->user_flags = user_flags;
        ಗಂಟು->ಮುಂದಿನ = s_fd_state_list;
        s_fd_state_list = ಗಂಟು;
    }
    pthread_mutex_unlock(&s_fd_state_ಬೀಗ);
}

ಶೂನ್ಯ linux_coop_untrack_fd(ಪೂರ್ಣಾಂಕ fd)
{
    ಆದರೆ (fd < 0) {
        ಹಿಂತಿರುಗಿ;
    }

    pthread_mutex_lock(&s_fd_state_ಬೀಗ);
    linux_coop_fd_state_t *prev = ಖಾಲಿ;
    linux_coop_fd_state_t *ಗಂಟು = s_fd_state_list;
    ಆಗ (ಗಂಟು != ಖಾಲಿ) {
        ಆದರೆ (ಗಂಟು->fd == fd) {
            ಆದರೆ (prev == ಖಾಲಿ) {
                s_fd_state_list = ಗಂಟು->ಮುಂದಿನ;
            } ಇಲ್ಲದಿದ್ದರೆ {
                prev->ಮುಂದಿನ = ಗಂಟು->ಮುಂದಿನ;
            }
            ಬಿಡುಗಡೆ(ಗಂಟು);
            ನಿಲ್ಲಿಸು;
        }
        prev = ಗಂಟು;
        ಗಂಟು = ಗಂಟು->ಮುಂದಿನ;
    }
    pthread_mutex_unlock(&s_fd_state_ಬೀಗ);
}

ಶೂನ್ಯ linux_coop_ಗಣ_user_flags(ಪೂರ್ಣಾಂಕ fd, ಪೂರ್ಣಾಂಕ user_flags)
{
    linux_coop_track_fd(fd, user_flags);
}

bool linux_coop_get_user_flags(ಪೂರ್ಣಾಂಕ fd, ಪೂರ್ಣಾಂಕ *flags)
{
    ಆದರೆ (fd < 0 || flags == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ false;
    }

    pthread_mutex_lock(&s_fd_state_ಬೀಗ);
    linux_coop_fd_state_t *ಗಂಟು = linux_coop_ಕಂಡುಹಿಡಿ_fd_locked(fd);
    ಆದರೆ (ಗಂಟು == ಖಾಲಿ) {
        pthread_mutex_unlock(&s_fd_state_ಬೀಗ);
        ಹಿಂತಿರುಗಿ false;
    }
    *flags = ಗಂಟು->user_flags;
    pthread_mutex_unlock(&s_fd_state_ಬೀಗ);
    ಹಿಂತಿರುಗಿ true;
}

bool linux_coop_fd_user_nonblocking(ಪೂರ್ಣಾಂಕ fd)
{
    ಪೂರ್ಣಾಂಕ user_flags = 0;
    ಆದರೆ (linux_coop_get_user_flags(fd, &user_flags)) {
        ಹಿಂತಿರುಗಿ (user_flags & O_NONBLOCK) != 0;
    }
    ಹಿಂತಿರುಗಿ false;
}


ssize_t freertos_linux_coop_read(ಪೂರ್ಣಾಂಕ fd, ಶೂನ್ಯ *buf, size_t ಎಣಿಕೆ_2)
{
    LINUX_COOP_IO_LOOP(fd, real_read(fd, buf, ಎಣಿಕೆ_2))
}

ssize_t freertos_linux_coop_write(ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf, size_t ಎಣಿಕೆ_2)
{
    LINUX_COOP_IO_LOOP(fd, real_write(fd, buf, ಎಣಿಕೆ_2))
}

ssize_t freertos_linux_coop_pread(ಪೂರ್ಣಾಂಕ fd, ಶೂನ್ಯ *buf, size_t ಎಣಿಕೆ_2, off_t ಸ್ಥಾನಾಂತರ)
{
    LINUX_COOP_IO_LOOP(fd, real_pread(fd, buf, ಎಣಿಕೆ_2, ಸ್ಥಾನಾಂತರ))
}

ssize_t freertos_linux_coop_pwrite(ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf, size_t ಎಣಿಕೆ_2, off_t ಸ್ಥಾನಾಂತರ)
{
    LINUX_COOP_IO_LOOP(fd, real_pwrite(fd, buf, ಎಣಿಕೆ_2, ಸ್ಥಾನಾಂತರ))
}

ಪೂರ್ಣಾಂಕ freertos_linux_coop_open(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ ಬಹುಲಕ)
{
    ಪೂರ್ಣಾಂಕ fd = real_open(ಮಾರ್ಗ, flags, (mode_t)ಬಹುಲಕ);
    linux_coop_ಗಣ_nonblocking(fd);
    linux_coop_track_fd(fd, flags & O_NONBLOCK);
    ಹಿಂತಿರುಗಿ fd;
}

ಪೂರ್ಣಾಂಕ freertos_linux_coop_close(ಪೂರ್ಣಾಂಕ fd)
{
    ಪೂರ್ಣಾಂಕ ret = real_close(fd);
    ಆದರೆ (ret == 0 || errno == EINTR) {
        /* On Linux, close() always releases the fd even when returning
         * EINTR.  Retrying would risk closing a fd reused by another
         * thread.  Treat EINTR as success. */
        linux_coop_untrack_fd(fd);
        ಹಿಂತಿರುಗಿ 0;
    }
    ಹಿಂತಿರುಗಿ -1;
}

ಪೂರ್ಣಾಂಕ freertos_linux_coop_fcntl(ಪೂರ್ಣಾಂಕ fd, ಪೂರ್ಣಾಂಕ cmd, ಪೂರ್ಣಾಂಕ arg)
{
    ಆದರೆ (cmd == F_GETFL) {
        ಪೂರ್ಣಾಂಕ real_flags = real_fcntl(fd, F_GETFL, 0);
        ಆದರೆ (real_flags < 0) {
            ಹಿಂತಿರುಗಿ -1;
        }
        ಪೂರ್ಣಾಂಕ user_flags = 0;
        ಆದರೆ (linux_coop_get_user_flags(fd, &user_flags)) {
            real_flags &= ~O_NONBLOCK;
            real_flags |= (user_flags & O_NONBLOCK);
        }
        ಹಿಂತಿರುಗಿ real_flags;
    }
    ಆದರೆ (cmd == F_SETFL) {
        linux_coop_ಗಣ_user_flags(fd, arg);
        ಪೂರ್ಣಾಂಕ ಆಂತರಿಕ_flags = arg | O_NONBLOCK;
        ಹಿಂತಿರುಗಿ real_fcntl(fd, F_SETFL, ಆಂತರಿಕ_flags);
    }
    ಹಿಂತಿರುಗಿ real_fcntl(fd, cmd, arg);
}

ಪೂರ್ಣಾಂಕ freertos_linux_coop_select(ಪೂರ್ಣಾಂಕ nfds, fd_set *readfds, fd_set *writefds,
                      fd_set *exceptfds, ರಚನೆ timeval *timeout)
{
    ದೀರ್ಘ timeout_ms = -1;
    ಆದರೆ (timeout != ಖಾಲಿ) {
        timeout_ms = (ದೀರ್ಘ)timeout->tv_sec * 1000 +
                     (timeout->tv_usec + 999) / 1000;
        ಆದರೆ (timeout_ms == 0) {
            ಹಿಂತಿರುಗಿ real_select(nfds, readfds, writefds, exceptfds, timeout);
        }
    }

    /* real_select() mutates the fd_sets in place (clears them, then sets bits
     * for ready descriptors). To keep polling the same descriptors across
     * iterations we must snapshot the caller's sets and restore them on every
     * loop. */
    fd_set rfds_ಒಳ, wfds_ಒಳ, efds_ಒಳ;
    ಆದರೆ (readfds != ಖಾಲಿ) {
        rfds_ಒಳ = *readfds;
    }
    ಆದರೆ (writefds != ಖಾಲಿ) {
        wfds_ಒಳ = *writefds;
    }
    ಆದರೆ (exceptfds != ಖಾಲಿ) {
        efds_ಒಳ = *exceptfds;
    }

    ದೀರ್ಘ waited_ms = 0;
    ಆಗ (1) {
        ಆದರೆ (readfds != ಖಾಲಿ) {
            *readfds = rfds_ಒಳ;
        }
        ಆದರೆ (writefds != ಖಾಲಿ) {
            *writefds = wfds_ಒಳ;
        }
        ಆದರೆ (exceptfds != ಖಾಲಿ) {
            *exceptfds = efds_ಒಳ;
        }

        ರಚನೆ timeval zero_tv = {0, 0};
        ಪೂರ್ಣಾಂಕ ret = real_select(nfds, readfds, writefds, exceptfds, &zero_tv);
        ಆದರೆ (ret != 0) {
            ಹಿಂತಿರುಗಿ ret;
        }
        ಆದರೆ (timeout_ms == 0) {
            ಹಿಂತಿರುಗಿ 0;
        }
        ಆದರೆ (timeout_ms > 0 && waited_ms >= timeout_ms) {
            ಹಿಂತಿರುಗಿ 0;
        }
        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

ಶೂನ್ಯ freertos_linux_coop_syscalls_init(ಶೂನ್ಯ)
{
    linux_coop_ಗಣ_nonblocking(STDIN_FILENO);
    linux_coop_ಗಣ_nonblocking(STDOUT_FILENO);
    linux_coop_ಗಣ_nonblocking(STDERR_FILENO);
    linux_coop_track_fd(STDIN_FILENO, 0);
    linux_coop_track_fd(STDOUT_FILENO, 0);
    linux_coop_track_fd(STDERR_FILENO, 0);
}

/* These are overridden by strong definitions in VFS when that component
 * is linked.  When VFS is absent the weak versions provide cooperative
 * behaviour directly via real libc. */

__attribute__((weak)) ssize_t ಪಠ(ಪೂರ್ಣಾಂಕ fd, ಶೂನ್ಯ *buf, size_t ಎಣಿಕೆ_2)
{
    ಹಿಂತಿರುಗಿ freertos_linux_coop_read(fd, buf, ಎಣಿಕೆ_2);
}

__attribute__((weak)) ssize_t ಲಿಖ(ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf, size_t ಎಣಿಕೆ_2)
{
    ಹಿಂತಿರುಗಿ freertos_linux_coop_write(fd, buf, ಎಣಿಕೆ_2);
}

__attribute__((weak)) ssize_t pread(ಪೂರ್ಣಾಂಕ fd, ಶೂನ್ಯ *buf, size_t ಎಣಿಕೆ_2, off_t ಸ್ಥಾನಾಂತರ)
{
    ಹಿಂತಿರುಗಿ freertos_linux_coop_pread(fd, buf, ಎಣಿಕೆ_2, ಸ್ಥಾನಾಂತರ);
}

__attribute__((weak)) ssize_t pwrite(ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf, size_t ಎಣಿಕೆ_2, off_t ಸ್ಥಾನಾಂತರ)
{
    ಹಿಂತಿರುಗಿ freertos_linux_coop_pwrite(fd, buf, ಎಣಿಕೆ_2, ಸ್ಥಾನಾಂತರ);
}

__attribute__((weak)) ಪೂರ್ಣಾಂಕ open(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ flags, ...)
{
    ಪೂರ್ಣಾಂಕ ಬಹುಲಕ = 0;
    ಆದರೆ (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        ಬಹುಲಕ = va_arg(ap, ಪೂರ್ಣಾಂಕ);
        va_end(ap);
    }
    ಹಿಂತಿರುಗಿ freertos_linux_coop_open(ಮಾರ್ಗ, flags, ಬಹುಲಕ);
}

__attribute__((weak)) ಪೂರ್ಣಾಂಕ close(ಪೂರ್ಣಾಂಕ fd)
{
    ಹಿಂತಿರುಗಿ freertos_linux_coop_close(fd);
}

__attribute__((weak)) ಪೂರ್ಣಾಂಕ fcntl(ಪೂರ್ಣಾಂಕ fd, ಪೂರ್ಣಾಂಕ cmd, ...)
{
    ಪೂರ್ಣಾಂಕ arg = 0;
    ಆದರೆ (cmd != F_GETFL) {
        va_list list;
        va_start(list, cmd);
        arg = va_arg(list, ಪೂರ್ಣಾಂಕ);
        va_end(list);
    }
    ಹಿಂತಿರುಗಿ freertos_linux_coop_fcntl(fd, cmd, arg);
}

__attribute__((weak)) ಪೂರ್ಣಾಂಕ select(ಪೂರ್ಣಾಂಕ nfds, fd_set *readfds, fd_set *writefds,
                                 fd_set *exceptfds, ರಚನೆ timeval *timeout)
{
    ಹಿಂತಿರುಗಿ freertos_linux_coop_select(nfds, readfds, writefds, exceptfds, timeout);
}

ssize_t freertos_linux_coop_readv(ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ರಚನೆ iovec *iov, ಪೂರ್ಣಾಂಕ iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_readv(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_writev(ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ರಚನೆ iovec *iov, ಪೂರ್ಣಾಂಕ iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_writev(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_recv(ಪೂರ್ಣಾಂಕ sockfd, ಶೂನ್ಯ *buf, size_t len, ಪೂರ್ಣಾಂಕ flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recv(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_send(ಪೂರ್ಣಾಂಕ sockfd, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf, size_t len, ಪೂರ್ಣಾಂಕ flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_send(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_recvfrom(ಪೂರ್ಣಾಂಕ sockfd, ಶೂನ್ಯ *buf, size_t len, ಪೂರ್ಣಾಂಕ flags,
                 ರಚನೆ sockaddr *src_addr, socklen_t *addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvfrom(sockfd, buf, len, flags | MSG_DONTWAIT,
                            src_addr, addrlen))
}

ssize_t freertos_linux_coop_sendto(ಪೂರ್ಣಾಂಕ sockfd, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf, size_t len, ಪೂರ್ಣಾಂಕ flags,
               ಸ್ಥಿರಾಂಕ ರಚನೆ sockaddr *dest_addr, socklen_t addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendto(sockfd, buf, len, flags | MSG_DONTWAIT,
                          dest_addr, addrlen))
}

ssize_t freertos_linux_coop_recvmsg(ಪೂರ್ಣಾಂಕ sockfd, ರಚನೆ msghdr *ಸಂದೇಶ, ಪೂರ್ಣಾಂಕ flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvmsg(sockfd, ಸಂದೇಶ, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_sendmsg(ಪೂರ್ಣಾಂಕ sockfd, ಸ್ಥಿರಾಂಕ ರಚನೆ msghdr *ಸಂದೇಶ, ಪೂರ್ಣಾಂಕ flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendmsg(sockfd, ಸಂದೇಶ, flags | MSG_DONTWAIT))
}

__attribute__((weak)) ssize_t readv(ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ರಚನೆ iovec *iov, ಪೂರ್ಣಾಂಕ iovcnt)
{
    ಹಿಂತಿರುಗಿ freertos_linux_coop_readv(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t writev(ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ರಚನೆ iovec *iov, ಪೂರ್ಣಾಂಕ iovcnt)
{
    ಹಿಂತಿರುಗಿ freertos_linux_coop_writev(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t recv(ಪೂರ್ಣಾಂಕ sockfd, ಶೂನ್ಯ *buf, size_t len, ಪೂರ್ಣಾಂಕ flags)
{
    ಹಿಂತಿರುಗಿ freertos_linux_coop_recv(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t send(ಪೂರ್ಣಾಂಕ sockfd, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf, size_t len, ಪೂರ್ಣಾಂಕ flags)
{
    ಹಿಂತಿರುಗಿ freertos_linux_coop_send(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t recvfrom(ಪೂರ್ಣಾಂಕ sockfd, ಶೂನ್ಯ *buf, size_t len, ಪೂರ್ಣಾಂಕ flags,
                 ರಚನೆ sockaddr *src_addr, socklen_t *addrlen)
{
    ಹಿಂತಿರುಗಿ freertos_linux_coop_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

__attribute__((weak)) ssize_t sendto(ಪೂರ್ಣಾಂಕ sockfd, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf, size_t len, ಪೂರ್ಣಾಂಕ flags,
               ಸ್ಥಿರಾಂಕ ರಚನೆ sockaddr *dest_addr, socklen_t addrlen)
{
    ಹಿಂತಿರುಗಿ freertos_linux_coop_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

__attribute__((weak)) ssize_t recvmsg(ಪೂರ್ಣಾಂಕ sockfd, ರಚನೆ msghdr *ಸಂದೇಶ, ಪೂರ್ಣಾಂಕ flags)
{
    ಹಿಂತಿರುಗಿ freertos_linux_coop_recvmsg(sockfd, ಸಂದೇಶ, flags);
}

__attribute__((weak)) ssize_t sendmsg(ಪೂರ್ಣಾಂಕ sockfd, ಸ್ಥಿರಾಂಕ ರಚನೆ msghdr *ಸಂದೇಶ, ಪೂರ್ಣಾಂಕ flags)
{
    ಹಿಂತಿರುಗಿ freertos_linux_coop_sendmsg(sockfd, ಸಂದೇಶ, flags);
}

ಪೂರ್ಣಾಂಕ freertos_linux_coop_connect(ಪೂರ್ಣಾಂಕ sockfd, ಸ್ಥಿರಾಂಕ ರಚನೆ sockaddr *addr, socklen_t addrlen)
{
    ಆಗ (1)
    {
        ಪೂರ್ಣಾಂಕ ret = real_connect(sockfd, addr, addrlen);
        ಆದರೆ (ret == 0) {
            ಹಿಂತಿರುಗಿ ret;
        }

        ಆದರೆ (errno == EINPROGRESS || errno == EALREADY) {
            ರಚನೆ pollfd pfd;
            pfd.fd = sockfd;
            pfd.events = POLLOUT;
            pfd.revents = 0;

            ಪೂರ್ಣಾಂಕ poll_retries = 0;
            ಆಗ (1)
            {
                ಪೂರ್ಣಾಂಕ press = real_poll(&pfd, 1, 0);
                ಆದರೆ (press > 0) {
                    ಪೂರ್ಣಾಂಕ so_err = 0;
                    socklen_t len = ಗಾತ್ರ(so_err);
                    ಆದರೆ (getsockopt(sockfd, SOL_SOCKET, SO_ERROR,
                                   &so_err, &len) < 0) {
                        ಹಿಂತಿರುಗಿ -1;
                    }
                    ಆದರೆ (so_err == 0) {
                        ಹಿಂತಿರುಗಿ 0;
                    } ಇಲ್ಲದಿದ್ದರೆ {
                        errno = so_err;
                        ಹಿಂತಿರುಗಿ -1;
                    }
                } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (press == 0) {
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    ಮುಂದುವರಿ;
                } ಇಲ್ಲದಿದ್ದರೆ {
                    ಆದರೆ (errno == EINTR) {
                        ಮುಂದುವರಿ;
                    }
                    /* Non-EINTR poll error; bail out after a few retries */
                    ಆದರೆ (++poll_retries > 3) {
                        ಹಿಂತಿರುಗಿ -1;
                    }
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    ಮುಂದುವರಿ;
                }
            }
        }

        ಆದರೆ (errno == EINTR) {
            ಹಿಂತಿರುಗಿ -1;
        }

        ಹಿಂತಿರುಗಿ -1;
    }
}

ಪೂರ್ಣಾಂಕ freertos_linux_coop_accept(ಪೂರ್ಣಾಂಕ sockfd, ರಚನೆ sockaddr *addr, socklen_t *addrlen)
{
    ಆಗ (1)
    {
        ಪೂರ್ಣಾಂಕ fd = real_accept(sockfd, addr, addrlen);
        ಆದರೆ (fd >= 0) {
            linux_coop_ಗಣ_nonblocking(fd);
            linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(sockfd) ? O_NONBLOCK : 0);
            ಹಿಂತಿರುಗಿ fd;
        }
        ಆದರೆ (errno == EAGAIN || errno == EWOULDBLOCK) {
            ಆದರೆ (linux_coop_fd_user_nonblocking(sockfd)) {
                ಹಿಂತಿರುಗಿ -1;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            ಮುಂದುವರಿ;
        }
        ಹಿಂತಿರುಗಿ -1;
    }
}

ಪೂರ್ಣಾಂಕ freertos_linux_coop_pselect(ಪೂರ್ಣಾಂಕ nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, ಸ್ಥಿರಾಂಕ ರಚನೆ timespec *timeout,
            ಸ್ಥಿರಾಂಕ sigset_t *sigmask)
{
    ದೀರ್ಘ timeout_ms = -1;
    ಆದರೆ (timeout != ಖಾಲಿ) {
        timeout_ms = (ದೀರ್ಘ)timeout->tv_sec * 1000 +
                     (timeout->tv_nsec + 999999) / 1000000;
        ಆದರೆ (timeout_ms == 0) {
            ಹಿಂತಿರುಗಿ real_pselect(nfds, readfds, writefds, exceptfds,
                                timeout, sigmask);
        }
    }

    ದೀರ್ಘ waited_ms = 0;
    ಆಗ (1)
    {
        ರಚನೆ timespec zero_ts = {0, 0};
        ಪೂರ್ಣಾಂಕ ret = real_pselect(nfds, readfds, writefds, exceptfds,
                               &zero_ts, sigmask);
        ಆದರೆ (ret != 0) {
            ಹಿಂತಿರುಗಿ ret;
        }

        ಆದರೆ (timeout_ms == 0 ||
            (timeout_ms > 0 && waited_ms >= timeout_ms)) {
            ಹಿಂತಿರುಗಿ 0;
        }

        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

ಪೂರ್ಣಾಂಕ freertos_linux_coop_poll(ರಚನೆ pollfd *fds, nfds_t nfds, ಪೂರ್ಣಾಂಕ timeout)
{
    ಆದರೆ (timeout == 0) {
        ಹಿಂತಿರುಗಿ real_poll(fds, nfds, 0);
    }

    ದೀರ್ಘ timeout_ms = -1;
    ಆದರೆ (timeout > 0) {
        timeout_ms = timeout;
    }

    ದೀರ್ಘ waited_ms = 0;

    ಆಗ (1)
    {
        ಪೂರ್ಣಾಂಕ ret = real_poll(fds, nfds, 0);
        ಆದರೆ (ret > 0) {
            ಹಿಂತಿರುಗಿ ret;
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ret == 0) {
            ಆದರೆ (timeout_ms == 0 ||
                (timeout_ms > 0 && waited_ms >= timeout_ms)) {
                ಹಿಂತಿರುಗಿ 0;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            waited_ms += LINUX_COOP_TICK_MS;
            ಮುಂದುವರಿ;
        } ಇಲ್ಲದಿದ್ದರೆ {
            ಆದರೆ (errno == EINTR) {
                ಮುಂದುವರಿ;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            ಆದರೆ (timeout_ms > 0 && waited_ms >= timeout_ms) {
                ಹಿಂತಿರುಗಿ -1;
            }
            waited_ms += LINUX_COOP_TICK_MS;
        }
    }
}

ಪೂರ್ಣಾಂಕ nanosleep(ಸ್ಥಿರಾಂಕ ರಚನೆ timespec *req, ರಚನೆ timespec *rem)
{
    ಆದರೆ (req == ಖಾಲಿ) {
        errno = EFAULT;
        ಹಿಂತಿರುಗಿ -1;
    }
    ಆದರೆ (!linux_ಪೋರ್ಟ್_ಒಳ_freertos_task()) {
        ಹಿಂತಿರುಗಿ real_nanosleep(req, rem);
    }
    ದೀರ್ಘ ms = req->tv_sec * 1000 + (req->tv_nsec + 999999) / 1000000;
    ಆದರೆ (ms <= 0) {
        ms = 1;
    }
    vTaskDelay(ms);
    ಆದರೆ (rem) {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }
    ಹಿಂತಿರುಗಿ 0;
}

ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ sleep(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ seconds)
{
    linux_coop_yield(seconds * 1000);
    ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ usleep(useconds_t usec)
{
    linux_coop_yield(usec / 1000);
    ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ freertos_linux_coop_socket(ಪೂರ್ಣಾಂಕ domain, ಪೂರ್ಣಾಂಕ type, ಪೂರ್ಣಾಂಕ protocol)
{
    ಪೂರ್ಣಾಂಕ fd = real_socket(domain, type, protocol);
    linux_coop_ಗಣ_nonblocking(fd);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ SOCK_NONBLOCK
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#ಇಲ್ಲದಿದ್ದರೆ
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ user_flags = 0;
#ಆದರೆ_ಅಂತ್ಯ
    linux_coop_track_fd(fd, user_flags);
    ಹಿಂತಿರುಗಿ fd;
}

ಪೂರ್ಣಾಂಕ freertos_linux_coop_socketpair(ಪೂರ್ಣಾಂಕ domain, ಪೂರ್ಣಾಂಕ type, ಪೂರ್ಣಾಂಕ protocol, ಪೂರ್ಣಾಂಕ *sv)
{
    ಪೂರ್ಣಾಂಕ ret = real_socketpair(domain, type, protocol, sv);
    ಆದರೆ (ret == 0) {
        linux_coop_ಗಣ_nonblocking(sv[0]);
        linux_coop_ಗಣ_nonblocking(sv[1]);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ SOCK_NONBLOCK
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#ಇಲ್ಲದಿದ್ದರೆ
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ user_flags = 0;
#ಆದರೆ_ಅಂತ್ಯ
        linux_coop_track_fd(sv[0], user_flags);
        linux_coop_track_fd(sv[1], user_flags);
    }
    ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ freertos_linux_coop_pipe(ಪೂರ್ಣಾಂಕ *fds)
{
    ಪೂರ್ಣಾಂಕ ret = real_pipe(fds);
    ಆದರೆ (ret == 0) {
        linux_coop_ಗಣ_nonblocking(fds[0]);
        linux_coop_ಗಣ_nonblocking(fds[1]);
        linux_coop_track_fd(fds[0], 0);
        linux_coop_track_fd(fds[1], 0);
    }
    ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ freertos_linux_coop_pipe2(ಪೂರ್ಣಾಂಕ *fds, ಪೂರ್ಣಾಂಕ flags)
{
    ಪೂರ್ಣಾಂಕ ret = real_pipe2(fds, flags);
    ಆದರೆ (ret == 0) {
        linux_coop_ಗಣ_nonblocking(fds[0]);
        linux_coop_ಗಣ_nonblocking(fds[1]);
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ user_flags = (flags & O_NONBLOCK) ? O_NONBLOCK : 0;
        linux_coop_track_fd(fds[0], user_flags);
        linux_coop_track_fd(fds[1], user_flags);
    }
    ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ freertos_linux_coop_dup(ಪೂರ್ಣಾಂಕ oldfd)
{
    ಪೂರ್ಣಾಂಕ fd = real_dup(oldfd);
    linux_coop_ಗಣ_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    ಹಿಂತಿರುಗಿ fd;
}

ಪೂರ್ಣಾಂಕ freertos_linux_coop_dup2(ಪೂರ್ಣಾಂಕ oldfd, ಪೂರ್ಣಾಂಕ newfd)
{
    ಪೂರ್ಣಾಂಕ fd = real_dup2(oldfd, newfd);
    linux_coop_ಗಣ_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    ಹಿಂತಿರುಗಿ fd;
}

__attribute__((weak)) ಪೂರ್ಣಾಂಕ connect(ಪೂರ್ಣಾಂಕ sockfd, ಸ್ಥಿರಾಂಕ ರಚನೆ sockaddr *addr, socklen_t addrlen)
{
    ಹಿಂತಿರುಗಿ freertos_linux_coop_connect(sockfd, addr, addrlen);
}

__attribute__((weak)) ಪೂರ್ಣಾಂಕ accept(ಪೂರ್ಣಾಂಕ sockfd, ರಚನೆ sockaddr *addr, socklen_t *addrlen)
{
    ಹಿಂತಿರುಗಿ freertos_linux_coop_accept(sockfd, addr, addrlen);
}

__attribute__((weak)) ಪೂರ್ಣಾಂಕ pselect(ಪೂರ್ಣಾಂಕ nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, ಸ್ಥಿರಾಂಕ ರಚನೆ timespec *timeout,
            ಸ್ಥಿರಾಂಕ sigset_t *sigmask)
{
    ಹಿಂತಿರುಗಿ freertos_linux_coop_pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}

__attribute__((weak)) ಪೂರ್ಣಾಂಕ poll(ರಚನೆ pollfd *fds, nfds_t nfds, ಪೂರ್ಣಾಂಕ timeout)
{
    ಹಿಂತಿರುಗಿ freertos_linux_coop_poll(fds, nfds, timeout);
}

__attribute__((weak)) ಪೂರ್ಣಾಂಕ socket(ಪೂರ್ಣಾಂಕ domain, ಪೂರ್ಣಾಂಕ type, ಪೂರ್ಣಾಂಕ protocol)
{
    ಹಿಂತಿರುಗಿ freertos_linux_coop_socket(domain, type, protocol);
}

__attribute__((weak)) ಪೂರ್ಣಾಂಕ socketpair(ಪೂರ್ಣಾಂಕ domain, ಪೂರ್ಣಾಂಕ type, ಪೂರ್ಣಾಂಕ protocol, ಪೂರ್ಣಾಂಕ sv[2])
{
    ಹಿಂತಿರುಗಿ freertos_linux_coop_socketpair(domain, type, protocol, sv);
}

__attribute__((weak)) ಪೂರ್ಣಾಂಕ pipe(ಪೂರ್ಣಾಂಕ fds[2])
{
    ಹಿಂತಿರುಗಿ freertos_linux_coop_pipe(fds);
}

__attribute__((weak)) ಪೂರ್ಣಾಂಕ pipe2(ಪೂರ್ಣಾಂಕ fds[2], ಪೂರ್ಣಾಂಕ flags)
{
    ಹಿಂತಿರುಗಿ freertos_linux_coop_pipe2(fds, flags);
}

__attribute__((weak)) ಪೂರ್ಣಾಂಕ dup(ಪೂರ್ಣಾಂಕ oldfd)
{
    ಹಿಂತಿರುಗಿ freertos_linux_coop_dup(oldfd);
}

__attribute__((weak)) ಪೂರ್ಣಾಂಕ dup2(ಪೂರ್ಣಾಂಕ oldfd, ಪೂರ್ಣಾಂಕ newfd)
{
    ಹಿಂತಿರುಗಿ freertos_linux_coop_dup2(oldfd, newfd);
}
