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

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdarg.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/uio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/socket.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/select.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <poll.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <dlfcn.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <pthread.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "linux_port_coop_internal.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_private/freertos_linux_coop_syscalls.h"

ਸਥਿਰ ssize_t  (*real_readv)(ਪੂਰਨ, ਅਚਲ ਬਣਤਰ iovec *, ਪੂਰਨ);
ਸਥਿਰ ssize_t  (*real_writev)(ਪੂਰਨ, ਅਚਲ ਬਣਤਰ iovec *, ਪੂਰਨ);
ਸਥਿਰ ssize_t  (*real_recv)(ਪੂਰਨ, ਖਾਲੀ *, size_t, ਪੂਰਨ);
ਸਥਿਰ ssize_t  (*real_send)(ਪੂਰਨ, ਅਚਲ ਖਾਲੀ *, size_t, ਪੂਰਨ);
ਸਥਿਰ ssize_t  (*real_recvfrom)(ਪੂਰਨ, ਖਾਲੀ *, size_t, ਪੂਰਨ,
                                 ਬਣਤਰ sockaddr *, socklen_t *);
ਸਥਿਰ ssize_t  (*real_sendto)(ਪੂਰਨ, ਅਚਲ ਖਾਲੀ *, size_t, ਪੂਰਨ,
                                ਅਚਲ ਬਣਤਰ sockaddr *, socklen_t);
ਸਥਿਰ ssize_t  (*real_recvmsg)(ਪੂਰਨ, ਬਣਤਰ msghdr *, ਪੂਰਨ);
ਸਥਿਰ ssize_t  (*real_sendmsg)(ਪੂਰਨ, ਅਚਲ ਬਣਤਰ msghdr *, ਪੂਰਨ);
ਸਥਿਰ ਪੂਰਨ      (*real_connect)(ਪੂਰਨ, ਅਚਲ ਬਣਤਰ sockaddr *, socklen_t);
ਸਥਿਰ ਪੂਰਨ      (*real_accept)(ਪੂਰਨ, ਬਣਤਰ sockaddr *, socklen_t *);
ਸਥਿਰ ਪੂਰਨ      (*real_pselect)(ਪੂਰਨ, fd_set *, fd_set *, fd_set *,
                                ਅਚਲ ਬਣਤਰ timespec *, ਅਚਲ sigset_t *);
ਸਥਿਰ ਪੂਰਨ      (*real_poll)(ਬਣਤਰ pollfd *, nfds_t, ਪੂਰਨ);
ਸਥਿਰ ਪੂਰਨ      (*real_socket)(ਪੂਰਨ, ਪੂਰਨ, ਪੂਰਨ);
ਸਥਿਰ ਪੂਰਨ      (*real_socketpair)(ਪੂਰਨ, ਪੂਰਨ, ਪੂਰਨ, ਪੂਰਨ[2]);
ਸਥਿਰ ਪੂਰਨ      (*real_pipe)(ਪੂਰਨ[2]);
ਸਥਿਰ ਪੂਰਨ      (*real_pipe2)(ਪੂਰਨ[2], ਪੂਰਨ);
ਸਥਿਰ ਪੂਰਨ      (*real_dup)(ਪੂਰਨ);
ਸਥਿਰ ਪੂਰਨ      (*real_dup2)(ਪੂਰਨ, ਪੂਰਨ);
ਸਥਿਰ ਪੂਰਨ      (*real_nanosleep)(ਅਚਲ ਬਣਤਰ timespec *, ਬਣਤਰ timespec *);
ਸਥਿਰ ssize_t  (*real_read)(ਪੂਰਨ, ਖਾਲੀ *, size_t);
ਸਥਿਰ ssize_t  (*real_write)(ਪੂਰਨ, ਅਚਲ ਖਾਲੀ *, size_t);
ਸਥਿਰ ssize_t  (*real_pread)(ਪੂਰਨ, ਖਾਲੀ *, size_t, off_t);
ਸਥਿਰ ssize_t  (*real_pwrite)(ਪੂਰਨ, ਅਚਲ ਖਾਲੀ *, size_t, off_t);
ਸਥਿਰ ਪੂਰਨ      (*real_close)(ਪੂਰਨ);
ਸਥਿਰ ਪੂਰਨ      (*real_select)(ਪੂਰਨ, fd_set *, fd_set *, fd_set *,
                                ਬਣਤਰ timeval *);

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ (*linux_coop_fcntl_fn_t)(ਪੂਰਨ, ਪੂਰਨ, ...);
ਸਥਿਰ linux_coop_fcntl_fn_t real_fcntl;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ (*linux_coop_open_fn_t)(ਅਚਲ ਅੱਖਰ *, ਪੂਰਨ, mode_t);
ਸਥਿਰ linux_coop_open_fn_t real_open;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ linux_coop_fd_state {
    ਪੂਰਨ fd;
    ਪੂਰਨ user_flags;
    ਬਣਤਰ linux_coop_fd_state *ਅਗਲਾ;
} linux_coop_fd_state_t;

ਸਥਿਰ linux_coop_fd_state_t *s_fd_state_list;
ਸਥਿਰ pthread_mutex_t s_fd_state_ਤਾਲਾ = PTHREAD_MUTEX_INITIALIZER;

ਸਥਿਰ linux_coop_fd_state_t *linux_coop_ਲੱਭੋ_fd_locked(ਪੂਰਨ fd)
{
    linux_coop_fd_state_t *ਗੰਢ = s_fd_state_list;
    ਜਦੋਂ (ਗੰਢ != ਨਿਰਾ) {
        ਜੇ (ਗੰਢ->fd == fd) {
            ਵਾਪਸ ਗੰਢ;
        }
        ਗੰਢ = ਗੰਢ->ਅਗਲਾ;
    }
    ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਖਾਲੀ __attribute__((constructor)) linux_coop_resolve_all(ਖਾਲੀ)
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
    LINUX_COOP_RESOLVE(ਪਠ);
    LINUX_COOP_RESOLVE(ਲਿਖ);
    LINUX_COOP_RESOLVE(pread);
    LINUX_COOP_RESOLVE(pwrite);
    LINUX_COOP_RESOLVE(close);
    LINUX_COOP_RESOLVE(select);
    LINUX_COOP_RESOLVE(open);
}

ਖਾਲੀ linux_coop_ਸੈੱਟ_nonblocking(ਪੂਰਨ fd)
{
    ਜੇ (fd >= 0) {
        ਪੂਰਨ flags = real_fcntl(fd, F_GETFL, 0);
        ਜੇ (flags >= 0) {
            real_fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
    }
}

ਖਾਲੀ linux_coop_track_fd(ਪੂਰਨ fd, ਪੂਰਨ user_flags)
{
    ਜੇ (fd < 0) {
        ਵਾਪਸ;
    }

    pthread_mutex_lock(&s_fd_state_ਤਾਲਾ);
    linux_coop_fd_state_t *ਗੰਢ = linux_coop_ਲੱਭੋ_fd_locked(fd);
    ਜੇ (ਗੰਢ != ਨਿਰਾ) {
        ਗੰਢ->user_flags = user_flags;
        pthread_mutex_unlock(&s_fd_state_ਤਾਲਾ);
        ਵਾਪਸ;
    }

    ਗੰਢ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(*ਗੰਢ));
    ਜੇ (ਗੰਢ != ਨਿਰਾ) {
        ਗੰਢ->fd = fd;
        ਗੰਢ->user_flags = user_flags;
        ਗੰਢ->ਅਗਲਾ = s_fd_state_list;
        s_fd_state_list = ਗੰਢ;
    }
    pthread_mutex_unlock(&s_fd_state_ਤਾਲਾ);
}

ਖਾਲੀ linux_coop_untrack_fd(ਪੂਰਨ fd)
{
    ਜੇ (fd < 0) {
        ਵਾਪਸ;
    }

    pthread_mutex_lock(&s_fd_state_ਤਾਲਾ);
    linux_coop_fd_state_t *prev = ਨਿਰਾ;
    linux_coop_fd_state_t *ਗੰਢ = s_fd_state_list;
    ਜਦੋਂ (ਗੰਢ != ਨਿਰਾ) {
        ਜੇ (ਗੰਢ->fd == fd) {
            ਜੇ (prev == ਨਿਰਾ) {
                s_fd_state_list = ਗੰਢ->ਅਗਲਾ;
            } ਨਹੀਂਤਾਂ {
                prev->ਅਗਲਾ = ਗੰਢ->ਅਗਲਾ;
            }
            ਮੁਕਤ(ਗੰਢ);
            ਰੋਕੋ;
        }
        prev = ਗੰਢ;
        ਗੰਢ = ਗੰਢ->ਅਗਲਾ;
    }
    pthread_mutex_unlock(&s_fd_state_ਤਾਲਾ);
}

ਖਾਲੀ linux_coop_ਸੈੱਟ_user_flags(ਪੂਰਨ fd, ਪੂਰਨ user_flags)
{
    linux_coop_track_fd(fd, user_flags);
}

bool linux_coop_get_user_flags(ਪੂਰਨ fd, ਪੂਰਨ *flags)
{
    ਜੇ (fd < 0 || flags == ਨਿਰਾ) {
        ਵਾਪਸ false;
    }

    pthread_mutex_lock(&s_fd_state_ਤਾਲਾ);
    linux_coop_fd_state_t *ਗੰਢ = linux_coop_ਲੱਭੋ_fd_locked(fd);
    ਜੇ (ਗੰਢ == ਨਿਰਾ) {
        pthread_mutex_unlock(&s_fd_state_ਤਾਲਾ);
        ਵਾਪਸ false;
    }
    *flags = ਗੰਢ->user_flags;
    pthread_mutex_unlock(&s_fd_state_ਤਾਲਾ);
    ਵਾਪਸ true;
}

bool linux_coop_fd_user_nonblocking(ਪੂਰਨ fd)
{
    ਪੂਰਨ user_flags = 0;
    ਜੇ (linux_coop_get_user_flags(fd, &user_flags)) {
        ਵਾਪਸ (user_flags & O_NONBLOCK) != 0;
    }
    ਵਾਪਸ false;
}


ssize_t freertos_linux_coop_read(ਪੂਰਨ fd, ਖਾਲੀ *buf, size_t ਗਿਣਤੀ_2)
{
    LINUX_COOP_IO_LOOP(fd, real_read(fd, buf, ਗਿਣਤੀ_2))
}

ssize_t freertos_linux_coop_write(ਪੂਰਨ fd, ਅਚਲ ਖਾਲੀ *buf, size_t ਗਿਣਤੀ_2)
{
    LINUX_COOP_IO_LOOP(fd, real_write(fd, buf, ਗਿਣਤੀ_2))
}

ssize_t freertos_linux_coop_pread(ਪੂਰਨ fd, ਖਾਲੀ *buf, size_t ਗਿਣਤੀ_2, off_t ਵਿੱਥ)
{
    LINUX_COOP_IO_LOOP(fd, real_pread(fd, buf, ਗਿਣਤੀ_2, ਵਿੱਥ))
}

ssize_t freertos_linux_coop_pwrite(ਪੂਰਨ fd, ਅਚਲ ਖਾਲੀ *buf, size_t ਗਿਣਤੀ_2, off_t ਵਿੱਥ)
{
    LINUX_COOP_IO_LOOP(fd, real_pwrite(fd, buf, ਗਿਣਤੀ_2, ਵਿੱਥ))
}

ਪੂਰਨ freertos_linux_coop_open(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ flags, ਪੂਰਨ ਬਹੁਲਕ)
{
    ਪੂਰਨ fd = real_open(ਰਸਤਾ, flags, (mode_t)ਬਹੁਲਕ);
    linux_coop_ਸੈੱਟ_nonblocking(fd);
    linux_coop_track_fd(fd, flags & O_NONBLOCK);
    ਵਾਪਸ fd;
}

ਪੂਰਨ freertos_linux_coop_close(ਪੂਰਨ fd)
{
    ਪੂਰਨ ret = real_close(fd);
    ਜੇ (ret == 0 || errno == EINTR) {
        /* On Linux, close() always releases the fd even when returning
         * EINTR.  Retrying would risk closing a fd reused by another
         * thread.  Treat EINTR as success. */
        linux_coop_untrack_fd(fd);
        ਵਾਪਸ 0;
    }
    ਵਾਪਸ -1;
}

ਪੂਰਨ freertos_linux_coop_fcntl(ਪੂਰਨ fd, ਪੂਰਨ cmd, ਪੂਰਨ arg)
{
    ਜੇ (cmd == F_GETFL) {
        ਪੂਰਨ real_flags = real_fcntl(fd, F_GETFL, 0);
        ਜੇ (real_flags < 0) {
            ਵਾਪਸ -1;
        }
        ਪੂਰਨ user_flags = 0;
        ਜੇ (linux_coop_get_user_flags(fd, &user_flags)) {
            real_flags &= ~O_NONBLOCK;
            real_flags |= (user_flags & O_NONBLOCK);
        }
        ਵਾਪਸ real_flags;
    }
    ਜੇ (cmd == F_SETFL) {
        linux_coop_ਸੈੱਟ_user_flags(fd, arg);
        ਪੂਰਨ ਅੰਦਰੂਨੀ_flags = arg | O_NONBLOCK;
        ਵਾਪਸ real_fcntl(fd, F_SETFL, ਅੰਦਰੂਨੀ_flags);
    }
    ਵਾਪਸ real_fcntl(fd, cmd, arg);
}

ਪੂਰਨ freertos_linux_coop_select(ਪੂਰਨ nfds, fd_set *readfds, fd_set *writefds,
                      fd_set *exceptfds, ਬਣਤਰ timeval *timeout)
{
    ਲੰਮਾ timeout_ms = -1;
    ਜੇ (timeout != ਨਿਰਾ) {
        timeout_ms = (ਲੰਮਾ)timeout->tv_sec * 1000 +
                     (timeout->tv_usec + 999) / 1000;
        ਜੇ (timeout_ms == 0) {
            ਵਾਪਸ real_select(nfds, readfds, writefds, exceptfds, timeout);
        }
    }

    /* real_select() mutates the fd_sets in place (clears them, then sets bits
     * for ready descriptors). To keep polling the same descriptors across
     * iterations we must snapshot the caller's sets and restore them on every
     * loop. */
    fd_set rfds_ਵਿੱਚ, wfds_ਵਿੱਚ, efds_ਵਿੱਚ;
    ਜੇ (readfds != ਨਿਰਾ) {
        rfds_ਵਿੱਚ = *readfds;
    }
    ਜੇ (writefds != ਨਿਰਾ) {
        wfds_ਵਿੱਚ = *writefds;
    }
    ਜੇ (exceptfds != ਨਿਰਾ) {
        efds_ਵਿੱਚ = *exceptfds;
    }

    ਲੰਮਾ waited_ms = 0;
    ਜਦੋਂ (1) {
        ਜੇ (readfds != ਨਿਰਾ) {
            *readfds = rfds_ਵਿੱਚ;
        }
        ਜੇ (writefds != ਨਿਰਾ) {
            *writefds = wfds_ਵਿੱਚ;
        }
        ਜੇ (exceptfds != ਨਿਰਾ) {
            *exceptfds = efds_ਵਿੱਚ;
        }

        ਬਣਤਰ timeval zero_tv = {0, 0};
        ਪੂਰਨ ret = real_select(nfds, readfds, writefds, exceptfds, &zero_tv);
        ਜੇ (ret != 0) {
            ਵਾਪਸ ret;
        }
        ਜੇ (timeout_ms == 0) {
            ਵਾਪਸ 0;
        }
        ਜੇ (timeout_ms > 0 && waited_ms >= timeout_ms) {
            ਵਾਪਸ 0;
        }
        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

ਖਾਲੀ freertos_linux_coop_syscalls_init(ਖਾਲੀ)
{
    linux_coop_ਸੈੱਟ_nonblocking(STDIN_FILENO);
    linux_coop_ਸੈੱਟ_nonblocking(STDOUT_FILENO);
    linux_coop_ਸੈੱਟ_nonblocking(STDERR_FILENO);
    linux_coop_track_fd(STDIN_FILENO, 0);
    linux_coop_track_fd(STDOUT_FILENO, 0);
    linux_coop_track_fd(STDERR_FILENO, 0);
}

/* These are overridden by strong definitions in VFS when that component
 * is linked.  When VFS is absent the weak versions provide cooperative
 * behaviour directly via real libc. */

__attribute__((weak)) ssize_t ਪਠ(ਪੂਰਨ fd, ਖਾਲੀ *buf, size_t ਗਿਣਤੀ_2)
{
    ਵਾਪਸ freertos_linux_coop_read(fd, buf, ਗਿਣਤੀ_2);
}

__attribute__((weak)) ssize_t ਲਿਖ(ਪੂਰਨ fd, ਅਚਲ ਖਾਲੀ *buf, size_t ਗਿਣਤੀ_2)
{
    ਵਾਪਸ freertos_linux_coop_write(fd, buf, ਗਿਣਤੀ_2);
}

__attribute__((weak)) ssize_t pread(ਪੂਰਨ fd, ਖਾਲੀ *buf, size_t ਗਿਣਤੀ_2, off_t ਵਿੱਥ)
{
    ਵਾਪਸ freertos_linux_coop_pread(fd, buf, ਗਿਣਤੀ_2, ਵਿੱਥ);
}

__attribute__((weak)) ssize_t pwrite(ਪੂਰਨ fd, ਅਚਲ ਖਾਲੀ *buf, size_t ਗਿਣਤੀ_2, off_t ਵਿੱਥ)
{
    ਵਾਪਸ freertos_linux_coop_pwrite(fd, buf, ਗਿਣਤੀ_2, ਵਿੱਥ);
}

__attribute__((weak)) ਪੂਰਨ open(ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ flags, ...)
{
    ਪੂਰਨ ਬਹੁਲਕ = 0;
    ਜੇ (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        ਬਹੁਲਕ = va_arg(ap, ਪੂਰਨ);
        va_end(ap);
    }
    ਵਾਪਸ freertos_linux_coop_open(ਰਸਤਾ, flags, ਬਹੁਲਕ);
}

__attribute__((weak)) ਪੂਰਨ close(ਪੂਰਨ fd)
{
    ਵਾਪਸ freertos_linux_coop_close(fd);
}

__attribute__((weak)) ਪੂਰਨ fcntl(ਪੂਰਨ fd, ਪੂਰਨ cmd, ...)
{
    ਪੂਰਨ arg = 0;
    ਜੇ (cmd != F_GETFL) {
        va_list list;
        va_start(list, cmd);
        arg = va_arg(list, ਪੂਰਨ);
        va_end(list);
    }
    ਵਾਪਸ freertos_linux_coop_fcntl(fd, cmd, arg);
}

__attribute__((weak)) ਪੂਰਨ select(ਪੂਰਨ nfds, fd_set *readfds, fd_set *writefds,
                                 fd_set *exceptfds, ਬਣਤਰ timeval *timeout)
{
    ਵਾਪਸ freertos_linux_coop_select(nfds, readfds, writefds, exceptfds, timeout);
}

ssize_t freertos_linux_coop_readv(ਪੂਰਨ fd, ਅਚਲ ਬਣਤਰ iovec *iov, ਪੂਰਨ iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_readv(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_writev(ਪੂਰਨ fd, ਅਚਲ ਬਣਤਰ iovec *iov, ਪੂਰਨ iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_writev(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_recv(ਪੂਰਨ sockfd, ਖਾਲੀ *buf, size_t len, ਪੂਰਨ flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recv(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_send(ਪੂਰਨ sockfd, ਅਚਲ ਖਾਲੀ *buf, size_t len, ਪੂਰਨ flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_send(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_recvfrom(ਪੂਰਨ sockfd, ਖਾਲੀ *buf, size_t len, ਪੂਰਨ flags,
                 ਬਣਤਰ sockaddr *src_addr, socklen_t *addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvfrom(sockfd, buf, len, flags | MSG_DONTWAIT,
                            src_addr, addrlen))
}

ssize_t freertos_linux_coop_sendto(ਪੂਰਨ sockfd, ਅਚਲ ਖਾਲੀ *buf, size_t len, ਪੂਰਨ flags,
               ਅਚਲ ਬਣਤਰ sockaddr *dest_addr, socklen_t addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendto(sockfd, buf, len, flags | MSG_DONTWAIT,
                          dest_addr, addrlen))
}

ssize_t freertos_linux_coop_recvmsg(ਪੂਰਨ sockfd, ਬਣਤਰ msghdr *ਸੁਨੇਹਾ, ਪੂਰਨ flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvmsg(sockfd, ਸੁਨੇਹਾ, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_sendmsg(ਪੂਰਨ sockfd, ਅਚਲ ਬਣਤਰ msghdr *ਸੁਨੇਹਾ, ਪੂਰਨ flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendmsg(sockfd, ਸੁਨੇਹਾ, flags | MSG_DONTWAIT))
}

__attribute__((weak)) ssize_t readv(ਪੂਰਨ fd, ਅਚਲ ਬਣਤਰ iovec *iov, ਪੂਰਨ iovcnt)
{
    ਵਾਪਸ freertos_linux_coop_readv(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t writev(ਪੂਰਨ fd, ਅਚਲ ਬਣਤਰ iovec *iov, ਪੂਰਨ iovcnt)
{
    ਵਾਪਸ freertos_linux_coop_writev(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t recv(ਪੂਰਨ sockfd, ਖਾਲੀ *buf, size_t len, ਪੂਰਨ flags)
{
    ਵਾਪਸ freertos_linux_coop_recv(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t send(ਪੂਰਨ sockfd, ਅਚਲ ਖਾਲੀ *buf, size_t len, ਪੂਰਨ flags)
{
    ਵਾਪਸ freertos_linux_coop_send(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t recvfrom(ਪੂਰਨ sockfd, ਖਾਲੀ *buf, size_t len, ਪੂਰਨ flags,
                 ਬਣਤਰ sockaddr *src_addr, socklen_t *addrlen)
{
    ਵਾਪਸ freertos_linux_coop_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

__attribute__((weak)) ssize_t sendto(ਪੂਰਨ sockfd, ਅਚਲ ਖਾਲੀ *buf, size_t len, ਪੂਰਨ flags,
               ਅਚਲ ਬਣਤਰ sockaddr *dest_addr, socklen_t addrlen)
{
    ਵਾਪਸ freertos_linux_coop_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

__attribute__((weak)) ssize_t recvmsg(ਪੂਰਨ sockfd, ਬਣਤਰ msghdr *ਸੁਨੇਹਾ, ਪੂਰਨ flags)
{
    ਵਾਪਸ freertos_linux_coop_recvmsg(sockfd, ਸੁਨੇਹਾ, flags);
}

__attribute__((weak)) ssize_t sendmsg(ਪੂਰਨ sockfd, ਅਚਲ ਬਣਤਰ msghdr *ਸੁਨੇਹਾ, ਪੂਰਨ flags)
{
    ਵਾਪਸ freertos_linux_coop_sendmsg(sockfd, ਸੁਨੇਹਾ, flags);
}

ਪੂਰਨ freertos_linux_coop_connect(ਪੂਰਨ sockfd, ਅਚਲ ਬਣਤਰ sockaddr *addr, socklen_t addrlen)
{
    ਜਦੋਂ (1)
    {
        ਪੂਰਨ ret = real_connect(sockfd, addr, addrlen);
        ਜੇ (ret == 0) {
            ਵਾਪਸ ret;
        }

        ਜੇ (errno == EINPROGRESS || errno == EALREADY) {
            ਬਣਤਰ pollfd pfd;
            pfd.fd = sockfd;
            pfd.events = POLLOUT;
            pfd.revents = 0;

            ਪੂਰਨ poll_retries = 0;
            ਜਦੋਂ (1)
            {
                ਪੂਰਨ press = real_poll(&pfd, 1, 0);
                ਜੇ (press > 0) {
                    ਪੂਰਨ so_err = 0;
                    socklen_t len = ਆਕਾਰ(so_err);
                    ਜੇ (getsockopt(sockfd, SOL_SOCKET, SO_ERROR,
                                   &so_err, &len) < 0) {
                        ਵਾਪਸ -1;
                    }
                    ਜੇ (so_err == 0) {
                        ਵਾਪਸ 0;
                    } ਨਹੀਂਤਾਂ {
                        errno = so_err;
                        ਵਾਪਸ -1;
                    }
                } ਨਹੀਂਤਾਂ ਜੇ (press == 0) {
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    ਜਾਰੀ;
                } ਨਹੀਂਤਾਂ {
                    ਜੇ (errno == EINTR) {
                        ਜਾਰੀ;
                    }
                    /* Non-EINTR poll error; bail out after a few retries */
                    ਜੇ (++poll_retries > 3) {
                        ਵਾਪਸ -1;
                    }
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    ਜਾਰੀ;
                }
            }
        }

        ਜੇ (errno == EINTR) {
            ਵਾਪਸ -1;
        }

        ਵਾਪਸ -1;
    }
}

ਪੂਰਨ freertos_linux_coop_accept(ਪੂਰਨ sockfd, ਬਣਤਰ sockaddr *addr, socklen_t *addrlen)
{
    ਜਦੋਂ (1)
    {
        ਪੂਰਨ fd = real_accept(sockfd, addr, addrlen);
        ਜੇ (fd >= 0) {
            linux_coop_ਸੈੱਟ_nonblocking(fd);
            linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(sockfd) ? O_NONBLOCK : 0);
            ਵਾਪਸ fd;
        }
        ਜੇ (errno == EAGAIN || errno == EWOULDBLOCK) {
            ਜੇ (linux_coop_fd_user_nonblocking(sockfd)) {
                ਵਾਪਸ -1;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            ਜਾਰੀ;
        }
        ਵਾਪਸ -1;
    }
}

ਪੂਰਨ freertos_linux_coop_pselect(ਪੂਰਨ nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, ਅਚਲ ਬਣਤਰ timespec *timeout,
            ਅਚਲ sigset_t *sigmask)
{
    ਲੰਮਾ timeout_ms = -1;
    ਜੇ (timeout != ਨਿਰਾ) {
        timeout_ms = (ਲੰਮਾ)timeout->tv_sec * 1000 +
                     (timeout->tv_nsec + 999999) / 1000000;
        ਜੇ (timeout_ms == 0) {
            ਵਾਪਸ real_pselect(nfds, readfds, writefds, exceptfds,
                                timeout, sigmask);
        }
    }

    ਲੰਮਾ waited_ms = 0;
    ਜਦੋਂ (1)
    {
        ਬਣਤਰ timespec zero_ts = {0, 0};
        ਪੂਰਨ ret = real_pselect(nfds, readfds, writefds, exceptfds,
                               &zero_ts, sigmask);
        ਜੇ (ret != 0) {
            ਵਾਪਸ ret;
        }

        ਜੇ (timeout_ms == 0 ||
            (timeout_ms > 0 && waited_ms >= timeout_ms)) {
            ਵਾਪਸ 0;
        }

        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

ਪੂਰਨ freertos_linux_coop_poll(ਬਣਤਰ pollfd *fds, nfds_t nfds, ਪੂਰਨ timeout)
{
    ਜੇ (timeout == 0) {
        ਵਾਪਸ real_poll(fds, nfds, 0);
    }

    ਲੰਮਾ timeout_ms = -1;
    ਜੇ (timeout > 0) {
        timeout_ms = timeout;
    }

    ਲੰਮਾ waited_ms = 0;

    ਜਦੋਂ (1)
    {
        ਪੂਰਨ ret = real_poll(fds, nfds, 0);
        ਜੇ (ret > 0) {
            ਵਾਪਸ ret;
        } ਨਹੀਂਤਾਂ ਜੇ (ret == 0) {
            ਜੇ (timeout_ms == 0 ||
                (timeout_ms > 0 && waited_ms >= timeout_ms)) {
                ਵਾਪਸ 0;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            waited_ms += LINUX_COOP_TICK_MS;
            ਜਾਰੀ;
        } ਨਹੀਂਤਾਂ {
            ਜੇ (errno == EINTR) {
                ਜਾਰੀ;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            ਜੇ (timeout_ms > 0 && waited_ms >= timeout_ms) {
                ਵਾਪਸ -1;
            }
            waited_ms += LINUX_COOP_TICK_MS;
        }
    }
}

ਪੂਰਨ nanosleep(ਅਚਲ ਬਣਤਰ timespec *req, ਬਣਤਰ timespec *rem)
{
    ਜੇ (req == ਨਿਰਾ) {
        errno = EFAULT;
        ਵਾਪਸ -1;
    }
    ਜੇ (!linux_ਪੋਰਟ_ਵਿੱਚ_freertos_task()) {
        ਵਾਪਸ real_nanosleep(req, rem);
    }
    ਲੰਮਾ ms = req->tv_sec * 1000 + (req->tv_nsec + 999999) / 1000000;
    ਜੇ (ms <= 0) {
        ms = 1;
    }
    vTaskDelay(ms);
    ਜੇ (rem) {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }
    ਵਾਪਸ 0;
}

ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sleep(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ seconds)
{
    linux_coop_yield(seconds * 1000);
    ਵਾਪਸ 0;
}

ਪੂਰਨ usleep(useconds_t usec)
{
    linux_coop_yield(usec / 1000);
    ਵਾਪਸ 0;
}

ਪੂਰਨ freertos_linux_coop_socket(ਪੂਰਨ domain, ਪੂਰਨ type, ਪੂਰਨ protocol)
{
    ਪੂਰਨ fd = real_socket(domain, type, protocol);
    linux_coop_ਸੈੱਟ_nonblocking(fd);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ SOCK_NONBLOCK
    ਅਚਲ ਪੂਰਨ user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#ਨਹੀਂਤਾਂ
    ਅਚਲ ਪੂਰਨ user_flags = 0;
#ਜੇ_ਅੰਤ
    linux_coop_track_fd(fd, user_flags);
    ਵਾਪਸ fd;
}

ਪੂਰਨ freertos_linux_coop_socketpair(ਪੂਰਨ domain, ਪੂਰਨ type, ਪੂਰਨ protocol, ਪੂਰਨ *sv)
{
    ਪੂਰਨ ret = real_socketpair(domain, type, protocol, sv);
    ਜੇ (ret == 0) {
        linux_coop_ਸੈੱਟ_nonblocking(sv[0]);
        linux_coop_ਸੈੱਟ_nonblocking(sv[1]);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ SOCK_NONBLOCK
        ਅਚਲ ਪੂਰਨ user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#ਨਹੀਂਤਾਂ
        ਅਚਲ ਪੂਰਨ user_flags = 0;
#ਜੇ_ਅੰਤ
        linux_coop_track_fd(sv[0], user_flags);
        linux_coop_track_fd(sv[1], user_flags);
    }
    ਵਾਪਸ ret;
}

ਪੂਰਨ freertos_linux_coop_pipe(ਪੂਰਨ *fds)
{
    ਪੂਰਨ ret = real_pipe(fds);
    ਜੇ (ret == 0) {
        linux_coop_ਸੈੱਟ_nonblocking(fds[0]);
        linux_coop_ਸੈੱਟ_nonblocking(fds[1]);
        linux_coop_track_fd(fds[0], 0);
        linux_coop_track_fd(fds[1], 0);
    }
    ਵਾਪਸ ret;
}

ਪੂਰਨ freertos_linux_coop_pipe2(ਪੂਰਨ *fds, ਪੂਰਨ flags)
{
    ਪੂਰਨ ret = real_pipe2(fds, flags);
    ਜੇ (ret == 0) {
        linux_coop_ਸੈੱਟ_nonblocking(fds[0]);
        linux_coop_ਸੈੱਟ_nonblocking(fds[1]);
        ਅਚਲ ਪੂਰਨ user_flags = (flags & O_NONBLOCK) ? O_NONBLOCK : 0;
        linux_coop_track_fd(fds[0], user_flags);
        linux_coop_track_fd(fds[1], user_flags);
    }
    ਵਾਪਸ ret;
}

ਪੂਰਨ freertos_linux_coop_dup(ਪੂਰਨ oldfd)
{
    ਪੂਰਨ fd = real_dup(oldfd);
    linux_coop_ਸੈੱਟ_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    ਵਾਪਸ fd;
}

ਪੂਰਨ freertos_linux_coop_dup2(ਪੂਰਨ oldfd, ਪੂਰਨ newfd)
{
    ਪੂਰਨ fd = real_dup2(oldfd, newfd);
    linux_coop_ਸੈੱਟ_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    ਵਾਪਸ fd;
}

__attribute__((weak)) ਪੂਰਨ connect(ਪੂਰਨ sockfd, ਅਚਲ ਬਣਤਰ sockaddr *addr, socklen_t addrlen)
{
    ਵਾਪਸ freertos_linux_coop_connect(sockfd, addr, addrlen);
}

__attribute__((weak)) ਪੂਰਨ accept(ਪੂਰਨ sockfd, ਬਣਤਰ sockaddr *addr, socklen_t *addrlen)
{
    ਵਾਪਸ freertos_linux_coop_accept(sockfd, addr, addrlen);
}

__attribute__((weak)) ਪੂਰਨ pselect(ਪੂਰਨ nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, ਅਚਲ ਬਣਤਰ timespec *timeout,
            ਅਚਲ sigset_t *sigmask)
{
    ਵਾਪਸ freertos_linux_coop_pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}

__attribute__((weak)) ਪੂਰਨ poll(ਬਣਤਰ pollfd *fds, nfds_t nfds, ਪੂਰਨ timeout)
{
    ਵਾਪਸ freertos_linux_coop_poll(fds, nfds, timeout);
}

__attribute__((weak)) ਪੂਰਨ socket(ਪੂਰਨ domain, ਪੂਰਨ type, ਪੂਰਨ protocol)
{
    ਵਾਪਸ freertos_linux_coop_socket(domain, type, protocol);
}

__attribute__((weak)) ਪੂਰਨ socketpair(ਪੂਰਨ domain, ਪੂਰਨ type, ਪੂਰਨ protocol, ਪੂਰਨ sv[2])
{
    ਵਾਪਸ freertos_linux_coop_socketpair(domain, type, protocol, sv);
}

__attribute__((weak)) ਪੂਰਨ pipe(ਪੂਰਨ fds[2])
{
    ਵਾਪਸ freertos_linux_coop_pipe(fds);
}

__attribute__((weak)) ਪੂਰਨ pipe2(ਪੂਰਨ fds[2], ਪੂਰਨ flags)
{
    ਵਾਪਸ freertos_linux_coop_pipe2(fds, flags);
}

__attribute__((weak)) ਪੂਰਨ dup(ਪੂਰਨ oldfd)
{
    ਵਾਪਸ freertos_linux_coop_dup(oldfd);
}

__attribute__((weak)) ਪੂਰਨ dup2(ਪੂਰਨ oldfd, ਪੂਰਨ newfd)
{
    ਵਾਪਸ freertos_linux_coop_dup2(oldfd, newfd);
}
