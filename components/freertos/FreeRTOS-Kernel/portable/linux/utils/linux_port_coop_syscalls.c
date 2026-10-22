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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <time.h>
#include <poll.h>
#include <dlfcn.h>
#include <pthread.h>

#include "linux_port_coop_internal.h"
#include "esp_private/freertos_linux_coop_syscalls.h"

static ssize_t  (*real_readv)(puur_naa_nka, const sa_nracanaa iovec *, puur_naa_nka);
static ssize_t  (*real_writev)(puur_naa_nka, const sa_nracanaa iovec *, puur_naa_nka);
static ssize_t  (*real_recv)(puur_naa_nka, void *, size_t, puur_naa_nka);
static ssize_t  (*real_send)(puur_naa_nka, const void *, size_t, puur_naa_nka);
static ssize_t  (*real_recvfrom)(puur_naa_nka, void *, size_t, puur_naa_nka,
                                 sa_nracanaa sockaddr *, socklen_t *);
static ssize_t  (*real_sendto)(puur_naa_nka, const void *, size_t, puur_naa_nka,
                                const sa_nracanaa sockaddr *, socklen_t);
static ssize_t  (*real_recvmsg)(puur_naa_nka, sa_nracanaa msghdr *, puur_naa_nka);
static ssize_t  (*real_sendmsg)(puur_naa_nka, const sa_nracanaa msghdr *, puur_naa_nka);
static puur_naa_nka      (*real_connect)(puur_naa_nka, const sa_nracanaa sockaddr *, socklen_t);
static puur_naa_nka      (*real_accept)(puur_naa_nka, sa_nracanaa sockaddr *, socklen_t *);
static puur_naa_nka      (*real_pselect)(puur_naa_nka, fd_set *, fd_set *, fd_set *,
                                const sa_nracanaa timespec *, const sigset_t *);
static puur_naa_nka      (*real_poll)(sa_nracanaa pollfd *, nfds_t, puur_naa_nka);
static puur_naa_nka      (*real_socket)(puur_naa_nka, puur_naa_nka, puur_naa_nka);
static puur_naa_nka      (*real_socketpair)(puur_naa_nka, puur_naa_nka, puur_naa_nka, puur_naa_nka[2]);
static puur_naa_nka      (*real_pipe)(puur_naa_nka[2]);
static puur_naa_nka      (*real_pipe2)(puur_naa_nka[2], puur_naa_nka);
static puur_naa_nka      (*real_dup)(puur_naa_nka);
static puur_naa_nka      (*real_dup2)(puur_naa_nka, puur_naa_nka);
static puur_naa_nka      (*real_nanosleep)(const sa_nracanaa timespec *, sa_nracanaa timespec *);
static ssize_t  (*real_read)(puur_naa_nka, void *, size_t);
static ssize_t  (*real_write)(puur_naa_nka, const void *, size_t);
static ssize_t  (*real_pread)(puur_naa_nka, void *, size_t, off_t);
static ssize_t  (*real_pwrite)(puur_naa_nka, const void *, size_t, off_t);
static puur_naa_nka      (*real_close)(puur_naa_nka);
static puur_naa_nka      (*real_select)(puur_naa_nka, fd_set *, fd_set *, fd_set *,
                                sa_nracanaa timeval *);

typedef puur_naa_nka (*linux_coop_fcntl_fn_t)(puur_naa_nka, puur_naa_nka, ...);
static linux_coop_fcntl_fn_t real_fcntl;

typedef puur_naa_nka (*linux_coop_open_fn_t)(const var_na *, puur_naa_nka, mode_t);
static linux_coop_open_fn_t real_open;

typedef sa_nracanaa linux_coop_fd_state {
    puur_naa_nka fd;
    puur_naa_nka user_flags;
    sa_nracanaa linux_coop_fd_state *next;
} linux_coop_fd_state_t;

static linux_coop_fd_state_t *s_fd_state_list;
static pthread_mutex_t s_fd_state_lock = PTHREAD_MUTEX_INITIALIZER;

static linux_coop_fd_state_t *linux_coop_find_fd_locked(puur_naa_nka fd)
{
    linux_coop_fd_state_t *node = s_fd_state_list;
    ᯀᯬᯝ (node != NULL) {
        ᯀᯮᯝ (node->fd == fd) {
            ᯀᯮᯞ node;
        }
        node = node->next;
    }
    ᯀᯮᯞ NULL;
}

static void __attribute__((constructor)) linux_coop_resolve_all(void)
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
    LINUX_COOP_RESOLVE(read);
    LINUX_COOP_RESOLVE(write);
    LINUX_COOP_RESOLVE(pread);
    LINUX_COOP_RESOLVE(pwrite);
    LINUX_COOP_RESOLVE(close);
    LINUX_COOP_RESOLVE(select);
    LINUX_COOP_RESOLVE(open);
}

void linux_coop_set_nonblocking(puur_naa_nka fd)
{
    ᯀᯮᯝ (fd >= 0) {
        puur_naa_nka flags = real_fcntl(fd, F_GETFL, 0);
        ᯀᯮᯝ (flags >= 0) {
            real_fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
    }
}

void linux_coop_track_fd(puur_naa_nka fd, puur_naa_nka user_flags)
{
    ᯀᯮᯝ (fd < 0) {
        ᯀᯮᯞ;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *node = linux_coop_find_fd_locked(fd);
    ᯀᯮᯝ (node != NULL) {
        node->user_flags = user_flags;
        pthread_mutex_unlock(&s_fd_state_lock);
        ᯀᯮᯞ;
    }

    node = calloc(1, parimaa_na(*node));
    ᯀᯮᯝ (node != NULL) {
        node->fd = fd;
        node->user_flags = user_flags;
        node->next = s_fd_state_list;
        s_fd_state_list = node;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

void linux_coop_untrack_fd(puur_naa_nka fd)
{
    ᯀᯮᯝ (fd < 0) {
        ᯀᯮᯞ;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *prev = NULL;
    linux_coop_fd_state_t *node = s_fd_state_list;
    ᯀᯬᯝ (node != NULL) {
        ᯀᯮᯝ (node->fd == fd) {
            ᯀᯮᯝ (prev == NULL) {
                s_fd_state_list = node->next;
            } ᯀᯮᯝᯂᯪ {
                prev->next = node->next;
            }
            free(node);
            bha_nga;
        }
        prev = node;
        node = node->next;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

void linux_coop_set_user_flags(puur_naa_nka fd, puur_naa_nka user_flags)
{
    linux_coop_track_fd(fd, user_flags);
}

bool linux_coop_get_user_flags(puur_naa_nka fd, puur_naa_nka *flags)
{
    ᯀᯮᯝ (fd < 0 || flags == NULL) {
        ᯀᯮᯞ false;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *node = linux_coop_find_fd_locked(fd);
    ᯀᯮᯝ (node == NULL) {
        pthread_mutex_unlock(&s_fd_state_lock);
        ᯀᯮᯞ false;
    }
    *flags = node->user_flags;
    pthread_mutex_unlock(&s_fd_state_lock);
    ᯀᯮᯞ true;
}

bool linux_coop_fd_user_nonblocking(puur_naa_nka fd)
{
    puur_naa_nka user_flags = 0;
    ᯀᯮᯝ (linux_coop_get_user_flags(fd, &user_flags)) {
        ᯀᯮᯞ (user_flags & O_NONBLOCK) != 0;
    }
    ᯀᯮᯞ false;
}


ssize_t freertos_linux_coop_read(puur_naa_nka fd, void *buf, size_t count)
{
    LINUX_COOP_IO_LOOP(fd, real_read(fd, buf, count))
}

ssize_t freertos_linux_coop_write(puur_naa_nka fd, const void *buf, size_t count)
{
    LINUX_COOP_IO_LOOP(fd, real_write(fd, buf, count))
}

ssize_t freertos_linux_coop_pread(puur_naa_nka fd, void *buf, size_t count, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pread(fd, buf, count, offset))
}

ssize_t freertos_linux_coop_pwrite(puur_naa_nka fd, const void *buf, size_t count, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pwrite(fd, buf, count, offset))
}

puur_naa_nka freertos_linux_coop_open(const var_na *path, puur_naa_nka flags, puur_naa_nka mode)
{
    puur_naa_nka fd = real_open(path, flags, (mode_t)mode);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, flags & O_NONBLOCK);
    ᯀᯮᯞ fd;
}

puur_naa_nka freertos_linux_coop_close(puur_naa_nka fd)
{
    puur_naa_nka ret = real_close(fd);
    ᯀᯮᯝ (ret == 0 || errno == EINTR) {
        /* On Linux, close() always releases the fd even when returning
         * EINTR.  Retrying would risk closing a fd reused by another
         * thread.  Treat EINTR as success. */
        linux_coop_untrack_fd(fd);
        ᯀᯮᯞ 0;
    }
    ᯀᯮᯞ -1;
}

puur_naa_nka freertos_linux_coop_fcntl(puur_naa_nka fd, puur_naa_nka cmd, puur_naa_nka arg)
{
    ᯀᯮᯝ (cmd == F_GETFL) {
        puur_naa_nka real_flags = real_fcntl(fd, F_GETFL, 0);
        ᯀᯮᯝ (real_flags < 0) {
            ᯀᯮᯞ -1;
        }
        puur_naa_nka user_flags = 0;
        ᯀᯮᯝ (linux_coop_get_user_flags(fd, &user_flags)) {
            real_flags &= ~O_NONBLOCK;
            real_flags |= (user_flags & O_NONBLOCK);
        }
        ᯀᯮᯞ real_flags;
    }
    ᯀᯮᯝ (cmd == F_SETFL) {
        linux_coop_set_user_flags(fd, arg);
        puur_naa_nka internal_flags = arg | O_NONBLOCK;
        ᯀᯮᯞ real_fcntl(fd, F_SETFL, internal_flags);
    }
    ᯀᯮᯞ real_fcntl(fd, cmd, arg);
}

puur_naa_nka freertos_linux_coop_select(puur_naa_nka nfds, fd_set *readfds, fd_set *writefds,
                      fd_set *exceptfds, sa_nracanaa timeval *timeout)
{
    long timeout_ms = -1;
    ᯀᯮᯝ (timeout != NULL) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_usec + 999) / 1000;
        ᯀᯮᯝ (timeout_ms == 0) {
            ᯀᯮᯞ real_select(nfds, readfds, writefds, exceptfds, timeout);
        }
    }

    /* real_select() mutates the fd_sets in place (clears them, then sets bits
     * for ready descriptors). To keep polling the same descriptors across
     * iterations we must snapshot the caller's sets and restore them on every
     * loop. */
    fd_set rfds_in, wfds_in, efds_in;
    ᯀᯮᯝ (readfds != NULL) {
        rfds_in = *readfds;
    }
    ᯀᯮᯝ (writefds != NULL) {
        wfds_in = *writefds;
    }
    ᯀᯮᯝ (exceptfds != NULL) {
        efds_in = *exceptfds;
    }

    long waited_ms = 0;
    ᯀᯬᯝ (1) {
        ᯀᯮᯝ (readfds != NULL) {
            *readfds = rfds_in;
        }
        ᯀᯮᯝ (writefds != NULL) {
            *writefds = wfds_in;
        }
        ᯀᯮᯝ (exceptfds != NULL) {
            *exceptfds = efds_in;
        }

        sa_nracanaa timeval zero_tv = {0, 0};
        puur_naa_nka ret = real_select(nfds, readfds, writefds, exceptfds, &zero_tv);
        ᯀᯮᯝ (ret != 0) {
            ᯀᯮᯞ ret;
        }
        ᯀᯮᯝ (timeout_ms == 0) {
            ᯀᯮᯞ 0;
        }
        ᯀᯮᯝ (timeout_ms > 0 && waited_ms >= timeout_ms) {
            ᯀᯮᯞ 0;
        }
        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

void freertos_linux_coop_syscalls_init(void)
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

__attribute__((weak)) ssize_t read(puur_naa_nka fd, void *buf, size_t count)
{
    ᯀᯮᯞ freertos_linux_coop_read(fd, buf, count);
}

__attribute__((weak)) ssize_t write(puur_naa_nka fd, const void *buf, size_t count)
{
    ᯀᯮᯞ freertos_linux_coop_write(fd, buf, count);
}

__attribute__((weak)) ssize_t pread(puur_naa_nka fd, void *buf, size_t count, off_t offset)
{
    ᯀᯮᯞ freertos_linux_coop_pread(fd, buf, count, offset);
}

__attribute__((weak)) ssize_t pwrite(puur_naa_nka fd, const void *buf, size_t count, off_t offset)
{
    ᯀᯮᯞ freertos_linux_coop_pwrite(fd, buf, count, offset);
}

__attribute__((weak)) puur_naa_nka open(const var_na *path, puur_naa_nka flags, ...)
{
    puur_naa_nka mode = 0;
    ᯀᯮᯝ (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        mode = va_arg(ap, puur_naa_nka);
        va_end(ap);
    }
    ᯀᯮᯞ freertos_linux_coop_open(path, flags, mode);
}

__attribute__((weak)) puur_naa_nka close(puur_naa_nka fd)
{
    ᯀᯮᯞ freertos_linux_coop_close(fd);
}

__attribute__((weak)) puur_naa_nka fcntl(puur_naa_nka fd, puur_naa_nka cmd, ...)
{
    puur_naa_nka arg = 0;
    ᯀᯮᯝ (cmd != F_GETFL) {
        va_list list;
        va_start(list, cmd);
        arg = va_arg(list, puur_naa_nka);
        va_end(list);
    }
    ᯀᯮᯞ freertos_linux_coop_fcntl(fd, cmd, arg);
}

__attribute__((weak)) puur_naa_nka select(puur_naa_nka nfds, fd_set *readfds, fd_set *writefds,
                                 fd_set *exceptfds, sa_nracanaa timeval *timeout)
{
    ᯀᯮᯞ freertos_linux_coop_select(nfds, readfds, writefds, exceptfds, timeout);
}

ssize_t freertos_linux_coop_readv(puur_naa_nka fd, const sa_nracanaa iovec *iov, puur_naa_nka iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_readv(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_writev(puur_naa_nka fd, const sa_nracanaa iovec *iov, puur_naa_nka iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_writev(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_recv(puur_naa_nka sockfd, void *buf, size_t len, puur_naa_nka flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recv(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_send(puur_naa_nka sockfd, const void *buf, size_t len, puur_naa_nka flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_send(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_recvfrom(puur_naa_nka sockfd, void *buf, size_t len, puur_naa_nka flags,
                 sa_nracanaa sockaddr *src_addr, socklen_t *addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvfrom(sockfd, buf, len, flags | MSG_DONTWAIT,
                            src_addr, addrlen))
}

ssize_t freertos_linux_coop_sendto(puur_naa_nka sockfd, const void *buf, size_t len, puur_naa_nka flags,
               const sa_nracanaa sockaddr *dest_addr, socklen_t addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendto(sockfd, buf, len, flags | MSG_DONTWAIT,
                          dest_addr, addrlen))
}

ssize_t freertos_linux_coop_recvmsg(puur_naa_nka sockfd, sa_nracanaa msghdr *msg, puur_naa_nka flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvmsg(sockfd, msg, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_sendmsg(puur_naa_nka sockfd, const sa_nracanaa msghdr *msg, puur_naa_nka flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendmsg(sockfd, msg, flags | MSG_DONTWAIT))
}

__attribute__((weak)) ssize_t readv(puur_naa_nka fd, const sa_nracanaa iovec *iov, puur_naa_nka iovcnt)
{
    ᯀᯮᯞ freertos_linux_coop_readv(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t writev(puur_naa_nka fd, const sa_nracanaa iovec *iov, puur_naa_nka iovcnt)
{
    ᯀᯮᯞ freertos_linux_coop_writev(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t recv(puur_naa_nka sockfd, void *buf, size_t len, puur_naa_nka flags)
{
    ᯀᯮᯞ freertos_linux_coop_recv(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t send(puur_naa_nka sockfd, const void *buf, size_t len, puur_naa_nka flags)
{
    ᯀᯮᯞ freertos_linux_coop_send(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t recvfrom(puur_naa_nka sockfd, void *buf, size_t len, puur_naa_nka flags,
                 sa_nracanaa sockaddr *src_addr, socklen_t *addrlen)
{
    ᯀᯮᯞ freertos_linux_coop_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

__attribute__((weak)) ssize_t sendto(puur_naa_nka sockfd, const void *buf, size_t len, puur_naa_nka flags,
               const sa_nracanaa sockaddr *dest_addr, socklen_t addrlen)
{
    ᯀᯮᯞ freertos_linux_coop_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

__attribute__((weak)) ssize_t recvmsg(puur_naa_nka sockfd, sa_nracanaa msghdr *msg, puur_naa_nka flags)
{
    ᯀᯮᯞ freertos_linux_coop_recvmsg(sockfd, msg, flags);
}

__attribute__((weak)) ssize_t sendmsg(puur_naa_nka sockfd, const sa_nracanaa msghdr *msg, puur_naa_nka flags)
{
    ᯀᯮᯞ freertos_linux_coop_sendmsg(sockfd, msg, flags);
}

puur_naa_nka freertos_linux_coop_connect(puur_naa_nka sockfd, const sa_nracanaa sockaddr *addr, socklen_t addrlen)
{
    ᯀᯬᯝ (1)
    {
        puur_naa_nka ret = real_connect(sockfd, addr, addrlen);
        ᯀᯮᯝ (ret == 0) {
            ᯀᯮᯞ ret;
        }

        ᯀᯮᯝ (errno == EINPROGRESS || errno == EALREADY) {
            sa_nracanaa pollfd pfd;
            pfd.fd = sockfd;
            pfd.events = POLLOUT;
            pfd.revents = 0;

            puur_naa_nka poll_retries = 0;
            ᯀᯬᯝ (1)
            {
                puur_naa_nka press = real_poll(&pfd, 1, 0);
                ᯀᯮᯝ (press > 0) {
                    puur_naa_nka so_err = 0;
                    socklen_t len = parimaa_na(so_err);
                    ᯀᯮᯝ (getsockopt(sockfd, SOL_SOCKET, SO_ERROR,
                                   &so_err, &len) < 0) {
                        ᯀᯮᯞ -1;
                    }
                    ᯀᯮᯝ (so_err == 0) {
                        ᯀᯮᯞ 0;
                    } ᯀᯮᯝᯂᯪ {
                        errno = so_err;
                        ᯀᯮᯞ -1;
                    }
                } ᯀᯮᯝᯂᯪ ᯀᯮᯝ (press == 0) {
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    continue;
                } ᯀᯮᯝᯂᯪ {
                    ᯀᯮᯝ (errno == EINTR) {
                        continue;
                    }
                    /* Non-EINTR poll error; bail out after a few retries */
                    ᯀᯮᯝ (++poll_retries > 3) {
                        ᯀᯮᯞ -1;
                    }
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    continue;
                }
            }
        }

        ᯀᯮᯝ (errno == EINTR) {
            ᯀᯮᯞ -1;
        }

        ᯀᯮᯞ -1;
    }
}

puur_naa_nka freertos_linux_coop_accept(puur_naa_nka sockfd, sa_nracanaa sockaddr *addr, socklen_t *addrlen)
{
    ᯀᯬᯝ (1)
    {
        puur_naa_nka fd = real_accept(sockfd, addr, addrlen);
        ᯀᯮᯝ (fd >= 0) {
            linux_coop_set_nonblocking(fd);
            linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(sockfd) ? O_NONBLOCK : 0);
            ᯀᯮᯞ fd;
        }
        ᯀᯮᯝ (errno == EAGAIN || errno == EWOULDBLOCK) {
            ᯀᯮᯝ (linux_coop_fd_user_nonblocking(sockfd)) {
                ᯀᯮᯞ -1;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            continue;
        }
        ᯀᯮᯞ -1;
    }
}

puur_naa_nka freertos_linux_coop_pselect(puur_naa_nka nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, const sa_nracanaa timespec *timeout,
            const sigset_t *sigmask)
{
    long timeout_ms = -1;
    ᯀᯮᯝ (timeout != NULL) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_nsec + 999999) / 1000000;
        ᯀᯮᯝ (timeout_ms == 0) {
            ᯀᯮᯞ real_pselect(nfds, readfds, writefds, exceptfds,
                                timeout, sigmask);
        }
    }

    long waited_ms = 0;
    ᯀᯬᯝ (1)
    {
        sa_nracanaa timespec zero_ts = {0, 0};
        puur_naa_nka ret = real_pselect(nfds, readfds, writefds, exceptfds,
                               &zero_ts, sigmask);
        ᯀᯮᯝ (ret != 0) {
            ᯀᯮᯞ ret;
        }

        ᯀᯮᯝ (timeout_ms == 0 ||
            (timeout_ms > 0 && waited_ms >= timeout_ms)) {
            ᯀᯮᯞ 0;
        }

        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

puur_naa_nka freertos_linux_coop_poll(sa_nracanaa pollfd *fds, nfds_t nfds, puur_naa_nka timeout)
{
    ᯀᯮᯝ (timeout == 0) {
        ᯀᯮᯞ real_poll(fds, nfds, 0);
    }

    long timeout_ms = -1;
    ᯀᯮᯝ (timeout > 0) {
        timeout_ms = timeout;
    }

    long waited_ms = 0;

    ᯀᯬᯝ (1)
    {
        puur_naa_nka ret = real_poll(fds, nfds, 0);
        ᯀᯮᯝ (ret > 0) {
            ᯀᯮᯞ ret;
        } ᯀᯮᯝᯂᯪ ᯀᯮᯝ (ret == 0) {
            ᯀᯮᯝ (timeout_ms == 0 ||
                (timeout_ms > 0 && waited_ms >= timeout_ms)) {
                ᯀᯮᯞ 0;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            waited_ms += LINUX_COOP_TICK_MS;
            continue;
        } ᯀᯮᯝᯂᯪ {
            ᯀᯮᯝ (errno == EINTR) {
                continue;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            ᯀᯮᯝ (timeout_ms > 0 && waited_ms >= timeout_ms) {
                ᯀᯮᯞ -1;
            }
            waited_ms += LINUX_COOP_TICK_MS;
        }
    }
}

puur_naa_nka nanosleep(const sa_nracanaa timespec *req, sa_nracanaa timespec *rem)
{
    ᯀᯮᯝ (req == NULL) {
        errno = EFAULT;
        ᯀᯮᯞ -1;
    }
    ᯀᯮᯝ (!linux_port_in_freertos_task()) {
        ᯀᯮᯞ real_nanosleep(req, rem);
    }
    long ms = req->tv_sec * 1000 + (req->tv_nsec + 999999) / 1000000;
    ᯀᯮᯝ (ms <= 0) {
        ms = 1;
    }
    vTaskDelay(ms);
    ᯀᯮᯝ (rem) {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }
    ᯀᯮᯞ 0;
}

unsigned puur_naa_nka sleep(unsigned puur_naa_nka seconds)
{
    linux_coop_yield(seconds * 1000);
    ᯀᯮᯞ 0;
}

puur_naa_nka usleep(useconds_t usec)
{
    linux_coop_yield(usec / 1000);
    ᯀᯮᯞ 0;
}

puur_naa_nka freertos_linux_coop_socket(puur_naa_nka domain, puur_naa_nka type, puur_naa_nka protocol)
{
    puur_naa_nka fd = real_socket(domain, type, protocol);
    linux_coop_set_nonblocking(fd);
#ifdef SOCK_NONBLOCK
    const puur_naa_nka user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#ᯀᯮᯝᯂᯪ
    const puur_naa_nka user_flags = 0;
#endif
    linux_coop_track_fd(fd, user_flags);
    ᯀᯮᯞ fd;
}

puur_naa_nka freertos_linux_coop_socketpair(puur_naa_nka domain, puur_naa_nka type, puur_naa_nka protocol, puur_naa_nka *sv)
{
    puur_naa_nka ret = real_socketpair(domain, type, protocol, sv);
    ᯀᯮᯝ (ret == 0) {
        linux_coop_set_nonblocking(sv[0]);
        linux_coop_set_nonblocking(sv[1]);
#ifdef SOCK_NONBLOCK
        const puur_naa_nka user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#ᯀᯮᯝᯂᯪ
        const puur_naa_nka user_flags = 0;
#endif
        linux_coop_track_fd(sv[0], user_flags);
        linux_coop_track_fd(sv[1], user_flags);
    }
    ᯀᯮᯞ ret;
}

puur_naa_nka freertos_linux_coop_pipe(puur_naa_nka *fds)
{
    puur_naa_nka ret = real_pipe(fds);
    ᯀᯮᯝ (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        linux_coop_track_fd(fds[0], 0);
        linux_coop_track_fd(fds[1], 0);
    }
    ᯀᯮᯞ ret;
}

puur_naa_nka freertos_linux_coop_pipe2(puur_naa_nka *fds, puur_naa_nka flags)
{
    puur_naa_nka ret = real_pipe2(fds, flags);
    ᯀᯮᯝ (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        const puur_naa_nka user_flags = (flags & O_NONBLOCK) ? O_NONBLOCK : 0;
        linux_coop_track_fd(fds[0], user_flags);
        linux_coop_track_fd(fds[1], user_flags);
    }
    ᯀᯮᯞ ret;
}

puur_naa_nka freertos_linux_coop_dup(puur_naa_nka oldfd)
{
    puur_naa_nka fd = real_dup(oldfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    ᯀᯮᯞ fd;
}

puur_naa_nka freertos_linux_coop_dup2(puur_naa_nka oldfd, puur_naa_nka newfd)
{
    puur_naa_nka fd = real_dup2(oldfd, newfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    ᯀᯮᯞ fd;
}

__attribute__((weak)) puur_naa_nka connect(puur_naa_nka sockfd, const sa_nracanaa sockaddr *addr, socklen_t addrlen)
{
    ᯀᯮᯞ freertos_linux_coop_connect(sockfd, addr, addrlen);
}

__attribute__((weak)) puur_naa_nka accept(puur_naa_nka sockfd, sa_nracanaa sockaddr *addr, socklen_t *addrlen)
{
    ᯀᯮᯞ freertos_linux_coop_accept(sockfd, addr, addrlen);
}

__attribute__((weak)) puur_naa_nka pselect(puur_naa_nka nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, const sa_nracanaa timespec *timeout,
            const sigset_t *sigmask)
{
    ᯀᯮᯞ freertos_linux_coop_pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}

__attribute__((weak)) puur_naa_nka poll(sa_nracanaa pollfd *fds, nfds_t nfds, puur_naa_nka timeout)
{
    ᯀᯮᯞ freertos_linux_coop_poll(fds, nfds, timeout);
}

__attribute__((weak)) puur_naa_nka socket(puur_naa_nka domain, puur_naa_nka type, puur_naa_nka protocol)
{
    ᯀᯮᯞ freertos_linux_coop_socket(domain, type, protocol);
}

__attribute__((weak)) puur_naa_nka socketpair(puur_naa_nka domain, puur_naa_nka type, puur_naa_nka protocol, puur_naa_nka sv[2])
{
    ᯀᯮᯞ freertos_linux_coop_socketpair(domain, type, protocol, sv);
}

__attribute__((weak)) puur_naa_nka pipe(puur_naa_nka fds[2])
{
    ᯀᯮᯞ freertos_linux_coop_pipe(fds);
}

__attribute__((weak)) puur_naa_nka pipe2(puur_naa_nka fds[2], puur_naa_nka flags)
{
    ᯀᯮᯞ freertos_linux_coop_pipe2(fds, flags);
}

__attribute__((weak)) puur_naa_nka dup(puur_naa_nka oldfd)
{
    ᯀᯮᯞ freertos_linux_coop_dup(oldfd);
}

__attribute__((weak)) puur_naa_nka dup2(puur_naa_nka oldfd, puur_naa_nka newfd)
{
    ᯀᯮᯞ freertos_linux_coop_dup2(oldfd, newfd);
}
