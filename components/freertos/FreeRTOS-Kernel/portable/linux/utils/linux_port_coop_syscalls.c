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

#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <errno.h>
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <stdarg.h>
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <sys/uio.h>
#അന്തര്ഭാവ <sys/socket.h>
#അന്തര്ഭാവ <sys/select.h>
#അന്തര്ഭാവ <sys/time.h>
#അന്തര്ഭാവ <time.h>
#അന്തര്ഭാവ <poll.h>
#അന്തര്ഭാവ <dlfcn.h>
#അന്തര്ഭാവ <pthread.h>

#അന്തര്ഭാവ "linux_port_coop_internal.h"
#അന്തര്ഭാവ "esp_private/freertos_linux_coop_syscalls.h"

സ്ഥിതം ssize_t  (*real_readv)(പൂർണ്ണസംഖ്യ, സ്ഥിരം ഘടന iovec *, പൂർണ്ണസംഖ്യ);
സ്ഥിതം ssize_t  (*real_writev)(പൂർണ്ണസംഖ്യ, സ്ഥിരം ഘടന iovec *, പൂർണ്ണസംഖ്യ);
സ്ഥിതം ssize_t  (*real_recv)(പൂർണ്ണസംഖ്യ, ശൂന്യം *, size_t, പൂർണ്ണസംഖ്യ);
സ്ഥിതം ssize_t  (*real_send)(പൂർണ്ണസംഖ്യ, സ്ഥിരം ശൂന്യം *, size_t, പൂർണ്ണസംഖ്യ);
സ്ഥിതം ssize_t  (*real_recvfrom)(പൂർണ്ണസംഖ്യ, ശൂന്യം *, size_t, പൂർണ്ണസംഖ്യ,
                                 ഘടന sockaddr *, socklen_t *);
സ്ഥിതം ssize_t  (*real_sendto)(പൂർണ്ണസംഖ്യ, സ്ഥിരം ശൂന്യം *, size_t, പൂർണ്ണസംഖ്യ,
                                സ്ഥിരം ഘടന sockaddr *, socklen_t);
സ്ഥിതം ssize_t  (*real_recvmsg)(പൂർണ്ണസംഖ്യ, ഘടന msghdr *, പൂർണ്ണസംഖ്യ);
സ്ഥിതം ssize_t  (*real_sendmsg)(പൂർണ്ണസംഖ്യ, സ്ഥിരം ഘടന msghdr *, പൂർണ്ണസംഖ്യ);
സ്ഥിതം പൂർണ്ണസംഖ്യ      (*real_connect)(പൂർണ്ണസംഖ്യ, സ്ഥിരം ഘടന sockaddr *, socklen_t);
സ്ഥിതം പൂർണ്ണസംഖ്യ      (*real_accept)(പൂർണ്ണസംഖ്യ, ഘടന sockaddr *, socklen_t *);
സ്ഥിതം പൂർണ്ണസംഖ്യ      (*real_pselect)(പൂർണ്ണസംഖ്യ, fd_set *, fd_set *, fd_set *,
                                സ്ഥിരം ഘടന timespec *, സ്ഥിരം sigset_t *);
സ്ഥിതം പൂർണ്ണസംഖ്യ      (*real_poll)(ഘടന pollfd *, nfds_t, പൂർണ്ണസംഖ്യ);
സ്ഥിതം പൂർണ്ണസംഖ്യ      (*real_socket)(പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ);
സ്ഥിതം പൂർണ്ണസംഖ്യ      (*real_socketpair)(പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ[2]);
സ്ഥിതം പൂർണ്ണസംഖ്യ      (*real_pipe)(പൂർണ്ണസംഖ്യ[2]);
സ്ഥിതം പൂർണ്ണസംഖ്യ      (*real_pipe2)(പൂർണ്ണസംഖ്യ[2], പൂർണ്ണസംഖ്യ);
സ്ഥിതം പൂർണ്ണസംഖ്യ      (*real_dup)(പൂർണ്ണസംഖ്യ);
സ്ഥിതം പൂർണ്ണസംഖ്യ      (*real_dup2)(പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ);
സ്ഥിതം പൂർണ്ണസംഖ്യ      (*real_nanosleep)(സ്ഥിരം ഘടന timespec *, ഘടന timespec *);
സ്ഥിതം ssize_t  (*real_read)(പൂർണ്ണസംഖ്യ, ശൂന്യം *, size_t);
സ്ഥിതം ssize_t  (*real_write)(പൂർണ്ണസംഖ്യ, സ്ഥിരം ശൂന്യം *, size_t);
സ്ഥിതം ssize_t  (*real_pread)(പൂർണ്ണസംഖ്യ, ശൂന്യം *, size_t, off_t);
സ്ഥിതം ssize_t  (*real_pwrite)(പൂർണ്ണസംഖ്യ, സ്ഥിരം ശൂന്യം *, size_t, off_t);
സ്ഥിതം പൂർണ്ണസംഖ്യ      (*real_close)(പൂർണ്ണസംഖ്യ);
സ്ഥിതം പൂർണ്ണസംഖ്യ      (*real_select)(പൂർണ്ണസംഖ്യ, fd_set *, fd_set *, fd_set *,
                                ഘടന timeval *);

തരനിർവചനം പൂർണ്ണസംഖ്യ (*linux_coop_fcntl_fn_t)(പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ, ...);
സ്ഥിതം linux_coop_fcntl_fn_t real_fcntl;

തരനിർവചനം പൂർണ്ണസംഖ്യ (*linux_coop_open_fn_t)(സ്ഥിരം അക്ഷരം *, പൂർണ്ണസംഖ്യ, mode_t);
സ്ഥിതം linux_coop_open_fn_t real_open;

തരനിർവചനം ഘടന linux_coop_fd_state {
    പൂർണ്ണസംഖ്യ fd;
    പൂർണ്ണസംഖ്യ user_flags;
    ഘടന linux_coop_fd_state *next;
} linux_coop_fd_state_t;

സ്ഥിതം linux_coop_fd_state_t *s_fd_state_list;
സ്ഥിതം pthread_mutex_t s_fd_state_lock = PTHREAD_MUTEX_INITIALIZER;

സ്ഥിതം linux_coop_fd_state_t *linux_coop_find_fd_locked(പൂർണ്ണസംഖ്യ fd)
{
    linux_coop_fd_state_t *കണ്ണി = s_fd_state_list;
    ആകുമ്പോൾ (കണ്ണി != ശൂന്യ) {
        എങ്കിൽ (കണ്ണി->fd == fd) {
            തിരികെ കണ്ണി;
        }
        കണ്ണി = കണ്ണി->next;
    }
    തിരികെ ശൂന്യ;
}

സ്ഥിതം ശൂന്യം __attribute__((constructor)) linux_coop_resolve_all(ശൂന്യം)
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
    LINUX_COOP_RESOLVE(പഠ);
    LINUX_COOP_RESOLVE(ലിഖ);
    LINUX_COOP_RESOLVE(pread);
    LINUX_COOP_RESOLVE(pwrite);
    LINUX_COOP_RESOLVE(close);
    LINUX_COOP_RESOLVE(select);
    LINUX_COOP_RESOLVE(open);
}

ശൂന്യം linux_coop_set_nonblocking(പൂർണ്ണസംഖ്യ fd)
{
    എങ്കിൽ (fd >= 0) {
        പൂർണ്ണസംഖ്യ flags = real_fcntl(fd, F_GETFL, 0);
        എങ്കിൽ (flags >= 0) {
            real_fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
    }
}

ശൂന്യം linux_coop_track_fd(പൂർണ്ണസംഖ്യ fd, പൂർണ്ണസംഖ്യ user_flags)
{
    എങ്കിൽ (fd < 0) {
        തിരികെ;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *കണ്ണി = linux_coop_find_fd_locked(fd);
    എങ്കിൽ (കണ്ണി != ശൂന്യ) {
        കണ്ണി->user_flags = user_flags;
        pthread_mutex_unlock(&s_fd_state_lock);
        തിരികെ;
    }

    കണ്ണി = calloc(1, വലുപ്പം(*കണ്ണി));
    എങ്കിൽ (കണ്ണി != ശൂന്യ) {
        കണ്ണി->fd = fd;
        കണ്ണി->user_flags = user_flags;
        കണ്ണി->next = s_fd_state_list;
        s_fd_state_list = കണ്ണി;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

ശൂന്യം linux_coop_untrack_fd(പൂർണ്ണസംഖ്യ fd)
{
    എങ്കിൽ (fd < 0) {
        തിരികെ;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *prev = ശൂന്യ;
    linux_coop_fd_state_t *കണ്ണി = s_fd_state_list;
    ആകുമ്പോൾ (കണ്ണി != ശൂന്യ) {
        എങ്കിൽ (കണ്ണി->fd == fd) {
            എങ്കിൽ (prev == ശൂന്യ) {
                s_fd_state_list = കണ്ണി->next;
            } അല്ലെങ്കിൽ {
                prev->next = കണ്ണി->next;
            }
            സ്വതന്ത്രം(കണ്ണി);
            നിർത്തുക;
        }
        prev = കണ്ണി;
        കണ്ണി = കണ്ണി->next;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

ശൂന്യം linux_coop_set_user_flags(പൂർണ്ണസംഖ്യ fd, പൂർണ്ണസംഖ്യ user_flags)
{
    linux_coop_track_fd(fd, user_flags);
}

bool linux_coop_get_user_flags(പൂർണ്ണസംഖ്യ fd, പൂർണ്ണസംഖ്യ *flags)
{
    എങ്കിൽ (fd < 0 || flags == ശൂന്യ) {
        തിരികെ false;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *കണ്ണി = linux_coop_find_fd_locked(fd);
    എങ്കിൽ (കണ്ണി == ശൂന്യ) {
        pthread_mutex_unlock(&s_fd_state_lock);
        തിരികെ false;
    }
    *flags = കണ്ണി->user_flags;
    pthread_mutex_unlock(&s_fd_state_lock);
    തിരികെ true;
}

bool linux_coop_fd_user_nonblocking(പൂർണ്ണസംഖ്യ fd)
{
    പൂർണ്ണസംഖ്യ user_flags = 0;
    എങ്കിൽ (linux_coop_get_user_flags(fd, &user_flags)) {
        തിരികെ (user_flags & O_NONBLOCK) != 0;
    }
    തിരികെ false;
}


ssize_t freertos_linux_coop_read(പൂർണ്ണസംഖ്യ fd, ശൂന്യം *buf, size_t എണ്ണം)
{
    LINUX_COOP_IO_LOOP(fd, real_read(fd, buf, എണ്ണം))
}

ssize_t freertos_linux_coop_write(പൂർണ്ണസംഖ്യ fd, സ്ഥിരം ശൂന്യം *buf, size_t എണ്ണം)
{
    LINUX_COOP_IO_LOOP(fd, real_write(fd, buf, എണ്ണം))
}

ssize_t freertos_linux_coop_pread(പൂർണ്ണസംഖ്യ fd, ശൂന്യം *buf, size_t എണ്ണം, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pread(fd, buf, എണ്ണം, offset))
}

ssize_t freertos_linux_coop_pwrite(പൂർണ്ണസംഖ്യ fd, സ്ഥിരം ശൂന്യം *buf, size_t എണ്ണം, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pwrite(fd, buf, എണ്ണം, offset))
}

പൂർണ്ണസംഖ്യ freertos_linux_coop_open(സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ flags, പൂർണ്ണസംഖ്യ mode)
{
    പൂർണ്ണസംഖ്യ fd = real_open(path, flags, (mode_t)mode);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, flags & O_NONBLOCK);
    തിരികെ fd;
}

പൂർണ്ണസംഖ്യ freertos_linux_coop_close(പൂർണ്ണസംഖ്യ fd)
{
    പൂർണ്ണസംഖ്യ ret = real_close(fd);
    എങ്കിൽ (ret == 0 || errno == EINTR) {
        /* On Linux, close() always releases the fd even when returning
         * EINTR.  Retrying would risk closing a fd reused by another
         * thread.  Treat EINTR as success. */
        linux_coop_untrack_fd(fd);
        തിരികെ 0;
    }
    തിരികെ -1;
}

പൂർണ്ണസംഖ്യ freertos_linux_coop_fcntl(പൂർണ്ണസംഖ്യ fd, പൂർണ്ണസംഖ്യ cmd, പൂർണ്ണസംഖ്യ arg)
{
    എങ്കിൽ (cmd == F_GETFL) {
        പൂർണ്ണസംഖ്യ real_flags = real_fcntl(fd, F_GETFL, 0);
        എങ്കിൽ (real_flags < 0) {
            തിരികെ -1;
        }
        പൂർണ്ണസംഖ്യ user_flags = 0;
        എങ്കിൽ (linux_coop_get_user_flags(fd, &user_flags)) {
            real_flags &= ~O_NONBLOCK;
            real_flags |= (user_flags & O_NONBLOCK);
        }
        തിരികെ real_flags;
    }
    എങ്കിൽ (cmd == F_SETFL) {
        linux_coop_set_user_flags(fd, arg);
        പൂർണ്ണസംഖ്യ internal_flags = arg | O_NONBLOCK;
        തിരികെ real_fcntl(fd, F_SETFL, internal_flags);
    }
    തിരികെ real_fcntl(fd, cmd, arg);
}

പൂർണ്ണസംഖ്യ freertos_linux_coop_select(പൂർണ്ണസംഖ്യ nfds, fd_set *readfds, fd_set *writefds,
                      fd_set *exceptfds, ഘടന timeval *timeout)
{
    long timeout_ms = -1;
    എങ്കിൽ (timeout != ശൂന്യ) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_usec + 999) / 1000;
        എങ്കിൽ (timeout_ms == 0) {
            തിരികെ real_select(nfds, readfds, writefds, exceptfds, timeout);
        }
    }

    /* real_select() mutates the fd_sets in place (clears them, then sets bits
     * for ready descriptors). To keep polling the same descriptors across
     * iterations we must snapshot the caller's sets and restore them on every
     * loop. */
    fd_set rfds_in, wfds_in, efds_in;
    എങ്കിൽ (readfds != ശൂന്യ) {
        rfds_in = *readfds;
    }
    എങ്കിൽ (writefds != ശൂന്യ) {
        wfds_in = *writefds;
    }
    എങ്കിൽ (exceptfds != ശൂന്യ) {
        efds_in = *exceptfds;
    }

    long waited_ms = 0;
    ആകുമ്പോൾ (1) {
        എങ്കിൽ (readfds != ശൂന്യ) {
            *readfds = rfds_in;
        }
        എങ്കിൽ (writefds != ശൂന്യ) {
            *writefds = wfds_in;
        }
        എങ്കിൽ (exceptfds != ശൂന്യ) {
            *exceptfds = efds_in;
        }

        ഘടന timeval zero_tv = {0, 0};
        പൂർണ്ണസംഖ്യ ret = real_select(nfds, readfds, writefds, exceptfds, &zero_tv);
        എങ്കിൽ (ret != 0) {
            തിരികെ ret;
        }
        എങ്കിൽ (timeout_ms == 0) {
            തിരികെ 0;
        }
        എങ്കിൽ (timeout_ms > 0 && waited_ms >= timeout_ms) {
            തിരികെ 0;
        }
        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

ശൂന്യം freertos_linux_coop_syscalls_init(ശൂന്യം)
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

__attribute__((weak)) ssize_t പഠ(പൂർണ്ണസംഖ്യ fd, ശൂന്യം *buf, size_t എണ്ണം)
{
    തിരികെ freertos_linux_coop_read(fd, buf, എണ്ണം);
}

__attribute__((weak)) ssize_t ലിഖ(പൂർണ്ണസംഖ്യ fd, സ്ഥിരം ശൂന്യം *buf, size_t എണ്ണം)
{
    തിരികെ freertos_linux_coop_write(fd, buf, എണ്ണം);
}

__attribute__((weak)) ssize_t pread(പൂർണ്ണസംഖ്യ fd, ശൂന്യം *buf, size_t എണ്ണം, off_t offset)
{
    തിരികെ freertos_linux_coop_pread(fd, buf, എണ്ണം, offset);
}

__attribute__((weak)) ssize_t pwrite(പൂർണ്ണസംഖ്യ fd, സ്ഥിരം ശൂന്യം *buf, size_t എണ്ണം, off_t offset)
{
    തിരികെ freertos_linux_coop_pwrite(fd, buf, എണ്ണം, offset);
}

__attribute__((weak)) പൂർണ്ണസംഖ്യ open(സ്ഥിരം അക്ഷരം *path, പൂർണ്ണസംഖ്യ flags, ...)
{
    പൂർണ്ണസംഖ്യ mode = 0;
    എങ്കിൽ (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        mode = va_arg(ap, പൂർണ്ണസംഖ്യ);
        va_end(ap);
    }
    തിരികെ freertos_linux_coop_open(path, flags, mode);
}

__attribute__((weak)) പൂർണ്ണസംഖ്യ close(പൂർണ്ണസംഖ്യ fd)
{
    തിരികെ freertos_linux_coop_close(fd);
}

__attribute__((weak)) പൂർണ്ണസംഖ്യ fcntl(പൂർണ്ണസംഖ്യ fd, പൂർണ്ണസംഖ്യ cmd, ...)
{
    പൂർണ്ണസംഖ്യ arg = 0;
    എങ്കിൽ (cmd != F_GETFL) {
        va_list list;
        va_start(list, cmd);
        arg = va_arg(list, പൂർണ്ണസംഖ്യ);
        va_end(list);
    }
    തിരികെ freertos_linux_coop_fcntl(fd, cmd, arg);
}

__attribute__((weak)) പൂർണ്ണസംഖ്യ select(പൂർണ്ണസംഖ്യ nfds, fd_set *readfds, fd_set *writefds,
                                 fd_set *exceptfds, ഘടന timeval *timeout)
{
    തിരികെ freertos_linux_coop_select(nfds, readfds, writefds, exceptfds, timeout);
}

ssize_t freertos_linux_coop_readv(പൂർണ്ണസംഖ്യ fd, സ്ഥിരം ഘടന iovec *iov, പൂർണ്ണസംഖ്യ iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_readv(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_writev(പൂർണ്ണസംഖ്യ fd, സ്ഥിരം ഘടന iovec *iov, പൂർണ്ണസംഖ്യ iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_writev(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_recv(പൂർണ്ണസംഖ്യ sockfd, ശൂന്യം *buf, size_t len, പൂർണ്ണസംഖ്യ flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recv(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_send(പൂർണ്ണസംഖ്യ sockfd, സ്ഥിരം ശൂന്യം *buf, size_t len, പൂർണ്ണസംഖ്യ flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_send(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_recvfrom(പൂർണ്ണസംഖ്യ sockfd, ശൂന്യം *buf, size_t len, പൂർണ്ണസംഖ്യ flags,
                 ഘടന sockaddr *src_addr, socklen_t *addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvfrom(sockfd, buf, len, flags | MSG_DONTWAIT,
                            src_addr, addrlen))
}

ssize_t freertos_linux_coop_sendto(പൂർണ്ണസംഖ്യ sockfd, സ്ഥിരം ശൂന്യം *buf, size_t len, പൂർണ്ണസംഖ്യ flags,
               സ്ഥിരം ഘടന sockaddr *dest_addr, socklen_t addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendto(sockfd, buf, len, flags | MSG_DONTWAIT,
                          dest_addr, addrlen))
}

ssize_t freertos_linux_coop_recvmsg(പൂർണ്ണസംഖ്യ sockfd, ഘടന msghdr *സന്ദേശം, പൂർണ്ണസംഖ്യ flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvmsg(sockfd, സന്ദേശം, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_sendmsg(പൂർണ്ണസംഖ്യ sockfd, സ്ഥിരം ഘടന msghdr *സന്ദേശം, പൂർണ്ണസംഖ്യ flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendmsg(sockfd, സന്ദേശം, flags | MSG_DONTWAIT))
}

__attribute__((weak)) ssize_t readv(പൂർണ്ണസംഖ്യ fd, സ്ഥിരം ഘടന iovec *iov, പൂർണ്ണസംഖ്യ iovcnt)
{
    തിരികെ freertos_linux_coop_readv(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t writev(പൂർണ്ണസംഖ്യ fd, സ്ഥിരം ഘടന iovec *iov, പൂർണ്ണസംഖ്യ iovcnt)
{
    തിരികെ freertos_linux_coop_writev(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t recv(പൂർണ്ണസംഖ്യ sockfd, ശൂന്യം *buf, size_t len, പൂർണ്ണസംഖ്യ flags)
{
    തിരികെ freertos_linux_coop_recv(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t send(പൂർണ്ണസംഖ്യ sockfd, സ്ഥിരം ശൂന്യം *buf, size_t len, പൂർണ്ണസംഖ്യ flags)
{
    തിരികെ freertos_linux_coop_send(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t recvfrom(പൂർണ്ണസംഖ്യ sockfd, ശൂന്യം *buf, size_t len, പൂർണ്ണസംഖ്യ flags,
                 ഘടന sockaddr *src_addr, socklen_t *addrlen)
{
    തിരികെ freertos_linux_coop_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

__attribute__((weak)) ssize_t sendto(പൂർണ്ണസംഖ്യ sockfd, സ്ഥിരം ശൂന്യം *buf, size_t len, പൂർണ്ണസംഖ്യ flags,
               സ്ഥിരം ഘടന sockaddr *dest_addr, socklen_t addrlen)
{
    തിരികെ freertos_linux_coop_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

__attribute__((weak)) ssize_t recvmsg(പൂർണ്ണസംഖ്യ sockfd, ഘടന msghdr *സന്ദേശം, പൂർണ്ണസംഖ്യ flags)
{
    തിരികെ freertos_linux_coop_recvmsg(sockfd, സന്ദേശം, flags);
}

__attribute__((weak)) ssize_t sendmsg(പൂർണ്ണസംഖ്യ sockfd, സ്ഥിരം ഘടന msghdr *സന്ദേശം, പൂർണ്ണസംഖ്യ flags)
{
    തിരികെ freertos_linux_coop_sendmsg(sockfd, സന്ദേശം, flags);
}

പൂർണ്ണസംഖ്യ freertos_linux_coop_connect(പൂർണ്ണസംഖ്യ sockfd, സ്ഥിരം ഘടന sockaddr *addr, socklen_t addrlen)
{
    ആകുമ്പോൾ (1)
    {
        പൂർണ്ണസംഖ്യ ret = real_connect(sockfd, addr, addrlen);
        എങ്കിൽ (ret == 0) {
            തിരികെ ret;
        }

        എങ്കിൽ (errno == EINPROGRESS || errno == EALREADY) {
            ഘടന pollfd pfd;
            pfd.fd = sockfd;
            pfd.events = POLLOUT;
            pfd.revents = 0;

            പൂർണ്ണസംഖ്യ poll_retries = 0;
            ആകുമ്പോൾ (1)
            {
                പൂർണ്ണസംഖ്യ press = real_poll(&pfd, 1, 0);
                എങ്കിൽ (press > 0) {
                    പൂർണ്ണസംഖ്യ so_err = 0;
                    socklen_t len = വലുപ്പം(so_err);
                    എങ്കിൽ (getsockopt(sockfd, SOL_SOCKET, SO_ERROR,
                                   &so_err, &len) < 0) {
                        തിരികെ -1;
                    }
                    എങ്കിൽ (so_err == 0) {
                        തിരികെ 0;
                    } അല്ലെങ്കിൽ {
                        errno = so_err;
                        തിരികെ -1;
                    }
                } അല്ലെങ്കിൽ എങ്കിൽ (press == 0) {
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    തുടരുക;
                } അല്ലെങ്കിൽ {
                    എങ്കിൽ (errno == EINTR) {
                        തുടരുക;
                    }
                    /* Non-EINTR poll error; bail out after a few retries */
                    എങ്കിൽ (++poll_retries > 3) {
                        തിരികെ -1;
                    }
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    തുടരുക;
                }
            }
        }

        എങ്കിൽ (errno == EINTR) {
            തിരികെ -1;
        }

        തിരികെ -1;
    }
}

പൂർണ്ണസംഖ്യ freertos_linux_coop_accept(പൂർണ്ണസംഖ്യ sockfd, ഘടന sockaddr *addr, socklen_t *addrlen)
{
    ആകുമ്പോൾ (1)
    {
        പൂർണ്ണസംഖ്യ fd = real_accept(sockfd, addr, addrlen);
        എങ്കിൽ (fd >= 0) {
            linux_coop_set_nonblocking(fd);
            linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(sockfd) ? O_NONBLOCK : 0);
            തിരികെ fd;
        }
        എങ്കിൽ (errno == EAGAIN || errno == EWOULDBLOCK) {
            എങ്കിൽ (linux_coop_fd_user_nonblocking(sockfd)) {
                തിരികെ -1;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            തുടരുക;
        }
        തിരികെ -1;
    }
}

പൂർണ്ണസംഖ്യ freertos_linux_coop_pselect(പൂർണ്ണസംഖ്യ nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, സ്ഥിരം ഘടന timespec *timeout,
            സ്ഥിരം sigset_t *sigmask)
{
    long timeout_ms = -1;
    എങ്കിൽ (timeout != ശൂന്യ) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_nsec + 999999) / 1000000;
        എങ്കിൽ (timeout_ms == 0) {
            തിരികെ real_pselect(nfds, readfds, writefds, exceptfds,
                                timeout, sigmask);
        }
    }

    long waited_ms = 0;
    ആകുമ്പോൾ (1)
    {
        ഘടന timespec zero_ts = {0, 0};
        പൂർണ്ണസംഖ്യ ret = real_pselect(nfds, readfds, writefds, exceptfds,
                               &zero_ts, sigmask);
        എങ്കിൽ (ret != 0) {
            തിരികെ ret;
        }

        എങ്കിൽ (timeout_ms == 0 ||
            (timeout_ms > 0 && waited_ms >= timeout_ms)) {
            തിരികെ 0;
        }

        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

പൂർണ്ണസംഖ്യ freertos_linux_coop_poll(ഘടന pollfd *fds, nfds_t nfds, പൂർണ്ണസംഖ്യ timeout)
{
    എങ്കിൽ (timeout == 0) {
        തിരികെ real_poll(fds, nfds, 0);
    }

    long timeout_ms = -1;
    എങ്കിൽ (timeout > 0) {
        timeout_ms = timeout;
    }

    long waited_ms = 0;

    ആകുമ്പോൾ (1)
    {
        പൂർണ്ണസംഖ്യ ret = real_poll(fds, nfds, 0);
        എങ്കിൽ (ret > 0) {
            തിരികെ ret;
        } അല്ലെങ്കിൽ എങ്കിൽ (ret == 0) {
            എങ്കിൽ (timeout_ms == 0 ||
                (timeout_ms > 0 && waited_ms >= timeout_ms)) {
                തിരികെ 0;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            waited_ms += LINUX_COOP_TICK_MS;
            തുടരുക;
        } അല്ലെങ്കിൽ {
            എങ്കിൽ (errno == EINTR) {
                തുടരുക;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            എങ്കിൽ (timeout_ms > 0 && waited_ms >= timeout_ms) {
                തിരികെ -1;
            }
            waited_ms += LINUX_COOP_TICK_MS;
        }
    }
}

പൂർണ്ണസംഖ്യ nanosleep(സ്ഥിരം ഘടന timespec *req, ഘടന timespec *rem)
{
    എങ്കിൽ (req == ശൂന്യ) {
        errno = EFAULT;
        തിരികെ -1;
    }
    എങ്കിൽ (!linux_port_in_freertos_task()) {
        തിരികെ real_nanosleep(req, rem);
    }
    long ms = req->tv_sec * 1000 + (req->tv_nsec + 999999) / 1000000;
    എങ്കിൽ (ms <= 0) {
        ms = 1;
    }
    vTaskDelay(ms);
    എങ്കിൽ (rem) {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }
    തിരികെ 0;
}

unsigned പൂർണ്ണസംഖ്യ sleep(unsigned പൂർണ്ണസംഖ്യ seconds)
{
    linux_coop_yield(seconds * 1000);
    തിരികെ 0;
}

പൂർണ്ണസംഖ്യ usleep(useconds_t usec)
{
    linux_coop_yield(usec / 1000);
    തിരികെ 0;
}

പൂർണ്ണസംഖ്യ freertos_linux_coop_socket(പൂർണ്ണസംഖ്യ domain, പൂർണ്ണസംഖ്യ type, പൂർണ്ണസംഖ്യ protocol)
{
    പൂർണ്ണസംഖ്യ fd = real_socket(domain, type, protocol);
    linux_coop_set_nonblocking(fd);
#ifdef SOCK_NONBLOCK
    സ്ഥിരം പൂർണ്ണസംഖ്യ user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#അല്ലെങ്കിൽ
    സ്ഥിരം പൂർണ്ണസംഖ്യ user_flags = 0;
#endif
    linux_coop_track_fd(fd, user_flags);
    തിരികെ fd;
}

പൂർണ്ണസംഖ്യ freertos_linux_coop_socketpair(പൂർണ്ണസംഖ്യ domain, പൂർണ്ണസംഖ്യ type, പൂർണ്ണസംഖ്യ protocol, പൂർണ്ണസംഖ്യ *sv)
{
    പൂർണ്ണസംഖ്യ ret = real_socketpair(domain, type, protocol, sv);
    എങ്കിൽ (ret == 0) {
        linux_coop_set_nonblocking(sv[0]);
        linux_coop_set_nonblocking(sv[1]);
#ifdef SOCK_NONBLOCK
        സ്ഥിരം പൂർണ്ണസംഖ്യ user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#അല്ലെങ്കിൽ
        സ്ഥിരം പൂർണ്ണസംഖ്യ user_flags = 0;
#endif
        linux_coop_track_fd(sv[0], user_flags);
        linux_coop_track_fd(sv[1], user_flags);
    }
    തിരികെ ret;
}

പൂർണ്ണസംഖ്യ freertos_linux_coop_pipe(പൂർണ്ണസംഖ്യ *fds)
{
    പൂർണ്ണസംഖ്യ ret = real_pipe(fds);
    എങ്കിൽ (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        linux_coop_track_fd(fds[0], 0);
        linux_coop_track_fd(fds[1], 0);
    }
    തിരികെ ret;
}

പൂർണ്ണസംഖ്യ freertos_linux_coop_pipe2(പൂർണ്ണസംഖ്യ *fds, പൂർണ്ണസംഖ്യ flags)
{
    പൂർണ്ണസംഖ്യ ret = real_pipe2(fds, flags);
    എങ്കിൽ (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        സ്ഥിരം പൂർണ്ണസംഖ്യ user_flags = (flags & O_NONBLOCK) ? O_NONBLOCK : 0;
        linux_coop_track_fd(fds[0], user_flags);
        linux_coop_track_fd(fds[1], user_flags);
    }
    തിരികെ ret;
}

പൂർണ്ണസംഖ്യ freertos_linux_coop_dup(പൂർണ്ണസംഖ്യ oldfd)
{
    പൂർണ്ണസംഖ്യ fd = real_dup(oldfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    തിരികെ fd;
}

പൂർണ്ണസംഖ്യ freertos_linux_coop_dup2(പൂർണ്ണസംഖ്യ oldfd, പൂർണ്ണസംഖ്യ newfd)
{
    പൂർണ്ണസംഖ്യ fd = real_dup2(oldfd, newfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    തിരികെ fd;
}

__attribute__((weak)) പൂർണ്ണസംഖ്യ connect(പൂർണ്ണസംഖ്യ sockfd, സ്ഥിരം ഘടന sockaddr *addr, socklen_t addrlen)
{
    തിരികെ freertos_linux_coop_connect(sockfd, addr, addrlen);
}

__attribute__((weak)) പൂർണ്ണസംഖ്യ accept(പൂർണ്ണസംഖ്യ sockfd, ഘടന sockaddr *addr, socklen_t *addrlen)
{
    തിരികെ freertos_linux_coop_accept(sockfd, addr, addrlen);
}

__attribute__((weak)) പൂർണ്ണസംഖ്യ pselect(പൂർണ്ണസംഖ്യ nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, സ്ഥിരം ഘടന timespec *timeout,
            സ്ഥിരം sigset_t *sigmask)
{
    തിരികെ freertos_linux_coop_pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}

__attribute__((weak)) പൂർണ്ണസംഖ്യ poll(ഘടന pollfd *fds, nfds_t nfds, പൂർണ്ണസംഖ്യ timeout)
{
    തിരികെ freertos_linux_coop_poll(fds, nfds, timeout);
}

__attribute__((weak)) പൂർണ്ണസംഖ്യ socket(പൂർണ്ണസംഖ്യ domain, പൂർണ്ണസംഖ്യ type, പൂർണ്ണസംഖ്യ protocol)
{
    തിരികെ freertos_linux_coop_socket(domain, type, protocol);
}

__attribute__((weak)) പൂർണ്ണസംഖ്യ socketpair(പൂർണ്ണസംഖ്യ domain, പൂർണ്ണസംഖ്യ type, പൂർണ്ണസംഖ്യ protocol, പൂർണ്ണസംഖ്യ sv[2])
{
    തിരികെ freertos_linux_coop_socketpair(domain, type, protocol, sv);
}

__attribute__((weak)) പൂർണ്ണസംഖ്യ pipe(പൂർണ്ണസംഖ്യ fds[2])
{
    തിരികെ freertos_linux_coop_pipe(fds);
}

__attribute__((weak)) പൂർണ്ണസംഖ്യ pipe2(പൂർണ്ണസംഖ്യ fds[2], പൂർണ്ണസംഖ്യ flags)
{
    തിരികെ freertos_linux_coop_pipe2(fds, flags);
}

__attribute__((weak)) പൂർണ്ണസംഖ്യ dup(പൂർണ്ണസംഖ്യ oldfd)
{
    തിരികെ freertos_linux_coop_dup(oldfd);
}

__attribute__((weak)) പൂർണ്ണസംഖ്യ dup2(പൂർണ്ണസംഖ്യ oldfd, പൂർണ്ണസംഖ്യ newfd)
{
    തിരികെ freertos_linux_coop_dup2(oldfd, newfd);
}
