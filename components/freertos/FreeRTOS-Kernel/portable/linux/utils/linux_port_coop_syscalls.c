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

static ssize_t  (*real_readv)(ចំនួនគត់, const sa_nracanaa iovec *, ចំនួនគត់);
static ssize_t  (*real_writev)(ចំនួនគត់, const sa_nracanaa iovec *, ចំនួនគត់);
static ssize_t  (*real_recv)(ចំនួនគត់, void *, size_t, ចំនួនគត់);
static ssize_t  (*real_send)(ចំនួនគត់, const void *, size_t, ចំនួនគត់);
static ssize_t  (*real_recvfrom)(ចំនួនគត់, void *, size_t, ចំនួនគត់,
                                 sa_nracanaa sockaddr *, socklen_t *);
static ssize_t  (*real_sendto)(ចំនួនគត់, const void *, size_t, ចំនួនគត់,
                                const sa_nracanaa sockaddr *, socklen_t);
static ssize_t  (*real_recvmsg)(ចំនួនគត់, sa_nracanaa msghdr *, ចំនួនគត់);
static ssize_t  (*real_sendmsg)(ចំនួនគត់, const sa_nracanaa msghdr *, ចំនួនគត់);
static ចំនួនគត់      (*real_connect)(ចំនួនគត់, const sa_nracanaa sockaddr *, socklen_t);
static ចំនួនគត់      (*real_accept)(ចំនួនគត់, sa_nracanaa sockaddr *, socklen_t *);
static ចំនួនគត់      (*real_pselect)(ចំនួនគត់, fd_set *, fd_set *, fd_set *,
                                const sa_nracanaa timespec *, const sigset_t *);
static ចំនួនគត់      (*real_poll)(sa_nracanaa pollfd *, nfds_t, ចំនួនគត់);
static ចំនួនគត់      (*real_socket)(ចំនួនគត់, ចំនួនគត់, ចំនួនគត់);
static ចំនួនគត់      (*real_socketpair)(ចំនួនគត់, ចំនួនគត់, ចំនួនគត់, ចំនួនគត់[2]);
static ចំនួនគត់      (*real_pipe)(ចំនួនគត់[2]);
static ចំនួនគត់      (*real_pipe2)(ចំនួនគត់[2], ចំនួនគត់);
static ចំនួនគត់      (*real_dup)(ចំនួនគត់);
static ចំនួនគត់      (*real_dup2)(ចំនួនគត់, ចំនួនគត់);
static ចំនួនគត់      (*real_nanosleep)(const sa_nracanaa timespec *, sa_nracanaa timespec *);
static ssize_t  (*real_read)(ចំនួនគត់, void *, size_t);
static ssize_t  (*real_write)(ចំនួនគត់, const void *, size_t);
static ssize_t  (*real_pread)(ចំនួនគត់, void *, size_t, off_t);
static ssize_t  (*real_pwrite)(ចំនួនគត់, const void *, size_t, off_t);
static ចំនួនគត់      (*real_close)(ចំនួនគត់);
static ចំនួនគត់      (*real_select)(ចំនួនគត់, fd_set *, fd_set *, fd_set *,
                                sa_nracanaa timeval *);

typedef ចំនួនគត់ (*linux_coop_fcntl_fn_t)(ចំនួនគត់, ចំនួនគត់, ...);
static linux_coop_fcntl_fn_t real_fcntl;

typedef ចំនួនគត់ (*linux_coop_open_fn_t)(const var_na *, ចំនួនគត់, mode_t);
static linux_coop_open_fn_t real_open;

typedef sa_nracanaa linux_coop_fd_state {
    ចំនួនគត់ fd;
    ចំនួនគត់ user_flags;
    sa_nracanaa linux_coop_fd_state *next;
} linux_coop_fd_state_t;

static linux_coop_fd_state_t *s_fd_state_list;
static pthread_mutex_t s_fd_state_lock = PTHREAD_MUTEX_INITIALIZER;

static linux_coop_fd_state_t *linux_coop_find_fd_locked(ចំនួនគត់ fd)
{
    linux_coop_fd_state_t *node = s_fd_state_list;
    ខណៈពេល (node != NULL) {
        ប្រសិនបើ (node->fd == fd) {
            ត្រឡប់ node;
        }
        node = node->next;
    }
    ត្រឡប់ NULL;
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

void linux_coop_set_nonblocking(ចំនួនគត់ fd)
{
    ប្រសិនបើ (fd >= 0) {
        ចំនួនគត់ flags = real_fcntl(fd, F_GETFL, 0);
        ប្រសិនបើ (flags >= 0) {
            real_fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
    }
}

void linux_coop_track_fd(ចំនួនគត់ fd, ចំនួនគត់ user_flags)
{
    ប្រសិនបើ (fd < 0) {
        ត្រឡប់;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *node = linux_coop_find_fd_locked(fd);
    ប្រសិនបើ (node != NULL) {
        node->user_flags = user_flags;
        pthread_mutex_unlock(&s_fd_state_lock);
        ត្រឡប់;
    }

    node = calloc(1, parimaa_na(*node));
    ប្រសិនបើ (node != NULL) {
        node->fd = fd;
        node->user_flags = user_flags;
        node->next = s_fd_state_list;
        s_fd_state_list = node;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

void linux_coop_untrack_fd(ចំនួនគត់ fd)
{
    ប្រសិនបើ (fd < 0) {
        ត្រឡប់;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *prev = NULL;
    linux_coop_fd_state_t *node = s_fd_state_list;
    ខណៈពេល (node != NULL) {
        ប្រសិនបើ (node->fd == fd) {
            ប្រសិនបើ (prev == NULL) {
                s_fd_state_list = node->next;
            } បើមិនដូច្នេះ {
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

void linux_coop_set_user_flags(ចំនួនគត់ fd, ចំនួនគត់ user_flags)
{
    linux_coop_track_fd(fd, user_flags);
}

bool linux_coop_get_user_flags(ចំនួនគត់ fd, ចំនួនគត់ *flags)
{
    ប្រសិនបើ (fd < 0 || flags == NULL) {
        ត្រឡប់ false;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *node = linux_coop_find_fd_locked(fd);
    ប្រសិនបើ (node == NULL) {
        pthread_mutex_unlock(&s_fd_state_lock);
        ត្រឡប់ false;
    }
    *flags = node->user_flags;
    pthread_mutex_unlock(&s_fd_state_lock);
    ត្រឡប់ true;
}

bool linux_coop_fd_user_nonblocking(ចំនួនគត់ fd)
{
    ចំនួនគត់ user_flags = 0;
    ប្រសិនបើ (linux_coop_get_user_flags(fd, &user_flags)) {
        ត្រឡប់ (user_flags & O_NONBLOCK) != 0;
    }
    ត្រឡប់ false;
}


ssize_t freertos_linux_coop_read(ចំនួនគត់ fd, void *buf, size_t count)
{
    LINUX_COOP_IO_LOOP(fd, real_read(fd, buf, count))
}

ssize_t freertos_linux_coop_write(ចំនួនគត់ fd, const void *buf, size_t count)
{
    LINUX_COOP_IO_LOOP(fd, real_write(fd, buf, count))
}

ssize_t freertos_linux_coop_pread(ចំនួនគត់ fd, void *buf, size_t count, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pread(fd, buf, count, offset))
}

ssize_t freertos_linux_coop_pwrite(ចំនួនគត់ fd, const void *buf, size_t count, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pwrite(fd, buf, count, offset))
}

ចំនួនគត់ freertos_linux_coop_open(const var_na *path, ចំនួនគត់ flags, ចំនួនគត់ mode)
{
    ចំនួនគត់ fd = real_open(path, flags, (mode_t)mode);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, flags & O_NONBLOCK);
    ត្រឡប់ fd;
}

ចំនួនគត់ freertos_linux_coop_close(ចំនួនគត់ fd)
{
    ចំនួនគត់ ret = real_close(fd);
    ប្រសិនបើ (ret == 0 || errno == EINTR) {
        /* On Linux, close() always releases the fd even when returning
         * EINTR.  Retrying would risk closing a fd reused by another
         * thread.  Treat EINTR as success. */
        linux_coop_untrack_fd(fd);
        ត្រឡប់ 0;
    }
    ត្រឡប់ -1;
}

ចំនួនគត់ freertos_linux_coop_fcntl(ចំនួនគត់ fd, ចំនួនគត់ cmd, ចំនួនគត់ arg)
{
    ប្រសិនបើ (cmd == F_GETFL) {
        ចំនួនគត់ real_flags = real_fcntl(fd, F_GETFL, 0);
        ប្រសិនបើ (real_flags < 0) {
            ត្រឡប់ -1;
        }
        ចំនួនគត់ user_flags = 0;
        ប្រសិនបើ (linux_coop_get_user_flags(fd, &user_flags)) {
            real_flags &= ~O_NONBLOCK;
            real_flags |= (user_flags & O_NONBLOCK);
        }
        ត្រឡប់ real_flags;
    }
    ប្រសិនបើ (cmd == F_SETFL) {
        linux_coop_set_user_flags(fd, arg);
        ចំនួនគត់ internal_flags = arg | O_NONBLOCK;
        ត្រឡប់ real_fcntl(fd, F_SETFL, internal_flags);
    }
    ត្រឡប់ real_fcntl(fd, cmd, arg);
}

ចំនួនគត់ freertos_linux_coop_select(ចំនួនគត់ nfds, fd_set *readfds, fd_set *writefds,
                      fd_set *exceptfds, sa_nracanaa timeval *timeout)
{
    long timeout_ms = -1;
    ប្រសិនបើ (timeout != NULL) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_usec + 999) / 1000;
        ប្រសិនបើ (timeout_ms == 0) {
            ត្រឡប់ real_select(nfds, readfds, writefds, exceptfds, timeout);
        }
    }

    /* real_select() mutates the fd_sets in place (clears them, then sets bits
     * for ready descriptors). To keep polling the same descriptors across
     * iterations we must snapshot the caller's sets and restore them on every
     * loop. */
    fd_set rfds_in, wfds_in, efds_in;
    ប្រសិនបើ (readfds != NULL) {
        rfds_in = *readfds;
    }
    ប្រសិនបើ (writefds != NULL) {
        wfds_in = *writefds;
    }
    ប្រសិនបើ (exceptfds != NULL) {
        efds_in = *exceptfds;
    }

    long waited_ms = 0;
    ខណៈពេល (1) {
        ប្រសិនបើ (readfds != NULL) {
            *readfds = rfds_in;
        }
        ប្រសិនបើ (writefds != NULL) {
            *writefds = wfds_in;
        }
        ប្រសិនបើ (exceptfds != NULL) {
            *exceptfds = efds_in;
        }

        sa_nracanaa timeval zero_tv = {0, 0};
        ចំនួនគត់ ret = real_select(nfds, readfds, writefds, exceptfds, &zero_tv);
        ប្រសិនបើ (ret != 0) {
            ត្រឡប់ ret;
        }
        ប្រសិនបើ (timeout_ms == 0) {
            ត្រឡប់ 0;
        }
        ប្រសិនបើ (timeout_ms > 0 && waited_ms >= timeout_ms) {
            ត្រឡប់ 0;
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

__attribute__((weak)) ssize_t read(ចំនួនគត់ fd, void *buf, size_t count)
{
    ត្រឡប់ freertos_linux_coop_read(fd, buf, count);
}

__attribute__((weak)) ssize_t write(ចំនួនគត់ fd, const void *buf, size_t count)
{
    ត្រឡប់ freertos_linux_coop_write(fd, buf, count);
}

__attribute__((weak)) ssize_t pread(ចំនួនគត់ fd, void *buf, size_t count, off_t offset)
{
    ត្រឡប់ freertos_linux_coop_pread(fd, buf, count, offset);
}

__attribute__((weak)) ssize_t pwrite(ចំនួនគត់ fd, const void *buf, size_t count, off_t offset)
{
    ត្រឡប់ freertos_linux_coop_pwrite(fd, buf, count, offset);
}

__attribute__((weak)) ចំនួនគត់ open(const var_na *path, ចំនួនគត់ flags, ...)
{
    ចំនួនគត់ mode = 0;
    ប្រសិនបើ (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        mode = va_arg(ap, ចំនួនគត់);
        va_end(ap);
    }
    ត្រឡប់ freertos_linux_coop_open(path, flags, mode);
}

__attribute__((weak)) ចំនួនគត់ close(ចំនួនគត់ fd)
{
    ត្រឡប់ freertos_linux_coop_close(fd);
}

__attribute__((weak)) ចំនួនគត់ fcntl(ចំនួនគត់ fd, ចំនួនគត់ cmd, ...)
{
    ចំនួនគត់ arg = 0;
    ប្រសិនបើ (cmd != F_GETFL) {
        va_list list;
        va_start(list, cmd);
        arg = va_arg(list, ចំនួនគត់);
        va_end(list);
    }
    ត្រឡប់ freertos_linux_coop_fcntl(fd, cmd, arg);
}

__attribute__((weak)) ចំនួនគត់ select(ចំនួនគត់ nfds, fd_set *readfds, fd_set *writefds,
                                 fd_set *exceptfds, sa_nracanaa timeval *timeout)
{
    ត្រឡប់ freertos_linux_coop_select(nfds, readfds, writefds, exceptfds, timeout);
}

ssize_t freertos_linux_coop_readv(ចំនួនគត់ fd, const sa_nracanaa iovec *iov, ចំនួនគត់ iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_readv(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_writev(ចំនួនគត់ fd, const sa_nracanaa iovec *iov, ចំនួនគត់ iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_writev(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_recv(ចំនួនគត់ sockfd, void *buf, size_t len, ចំនួនគត់ flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recv(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_send(ចំនួនគត់ sockfd, const void *buf, size_t len, ចំនួនគត់ flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_send(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_recvfrom(ចំនួនគត់ sockfd, void *buf, size_t len, ចំនួនគត់ flags,
                 sa_nracanaa sockaddr *src_addr, socklen_t *addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvfrom(sockfd, buf, len, flags | MSG_DONTWAIT,
                            src_addr, addrlen))
}

ssize_t freertos_linux_coop_sendto(ចំនួនគត់ sockfd, const void *buf, size_t len, ចំនួនគត់ flags,
               const sa_nracanaa sockaddr *dest_addr, socklen_t addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendto(sockfd, buf, len, flags | MSG_DONTWAIT,
                          dest_addr, addrlen))
}

ssize_t freertos_linux_coop_recvmsg(ចំនួនគត់ sockfd, sa_nracanaa msghdr *msg, ចំនួនគត់ flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvmsg(sockfd, msg, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_sendmsg(ចំនួនគត់ sockfd, const sa_nracanaa msghdr *msg, ចំនួនគត់ flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendmsg(sockfd, msg, flags | MSG_DONTWAIT))
}

__attribute__((weak)) ssize_t readv(ចំនួនគត់ fd, const sa_nracanaa iovec *iov, ចំនួនគត់ iovcnt)
{
    ត្រឡប់ freertos_linux_coop_readv(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t writev(ចំនួនគត់ fd, const sa_nracanaa iovec *iov, ចំនួនគត់ iovcnt)
{
    ត្រឡប់ freertos_linux_coop_writev(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t recv(ចំនួនគត់ sockfd, void *buf, size_t len, ចំនួនគត់ flags)
{
    ត្រឡប់ freertos_linux_coop_recv(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t send(ចំនួនគត់ sockfd, const void *buf, size_t len, ចំនួនគត់ flags)
{
    ត្រឡប់ freertos_linux_coop_send(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t recvfrom(ចំនួនគត់ sockfd, void *buf, size_t len, ចំនួនគត់ flags,
                 sa_nracanaa sockaddr *src_addr, socklen_t *addrlen)
{
    ត្រឡប់ freertos_linux_coop_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

__attribute__((weak)) ssize_t sendto(ចំនួនគត់ sockfd, const void *buf, size_t len, ចំនួនគត់ flags,
               const sa_nracanaa sockaddr *dest_addr, socklen_t addrlen)
{
    ត្រឡប់ freertos_linux_coop_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

__attribute__((weak)) ssize_t recvmsg(ចំនួនគត់ sockfd, sa_nracanaa msghdr *msg, ចំនួនគត់ flags)
{
    ត្រឡប់ freertos_linux_coop_recvmsg(sockfd, msg, flags);
}

__attribute__((weak)) ssize_t sendmsg(ចំនួនគត់ sockfd, const sa_nracanaa msghdr *msg, ចំនួនគត់ flags)
{
    ត្រឡប់ freertos_linux_coop_sendmsg(sockfd, msg, flags);
}

ចំនួនគត់ freertos_linux_coop_connect(ចំនួនគត់ sockfd, const sa_nracanaa sockaddr *addr, socklen_t addrlen)
{
    ខណៈពេល (1)
    {
        ចំនួនគត់ ret = real_connect(sockfd, addr, addrlen);
        ប្រសិនបើ (ret == 0) {
            ត្រឡប់ ret;
        }

        ប្រសិនបើ (errno == EINPROGRESS || errno == EALREADY) {
            sa_nracanaa pollfd pfd;
            pfd.fd = sockfd;
            pfd.events = POLLOUT;
            pfd.revents = 0;

            ចំនួនគត់ poll_retries = 0;
            ខណៈពេល (1)
            {
                ចំនួនគត់ press = real_poll(&pfd, 1, 0);
                ប្រសិនបើ (press > 0) {
                    ចំនួនគត់ so_err = 0;
                    socklen_t len = parimaa_na(so_err);
                    ប្រសិនបើ (getsockopt(sockfd, SOL_SOCKET, SO_ERROR,
                                   &so_err, &len) < 0) {
                        ត្រឡប់ -1;
                    }
                    ប្រសិនបើ (so_err == 0) {
                        ត្រឡប់ 0;
                    } បើមិនដូច្នេះ {
                        errno = so_err;
                        ត្រឡប់ -1;
                    }
                } បើមិនដូច្នេះ ប្រសិនបើ (press == 0) {
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    continue;
                } បើមិនដូច្នេះ {
                    ប្រសិនបើ (errno == EINTR) {
                        continue;
                    }
                    /* Non-EINTR poll error; bail out after a few retries */
                    ប្រសិនបើ (++poll_retries > 3) {
                        ត្រឡប់ -1;
                    }
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    continue;
                }
            }
        }

        ប្រសិនបើ (errno == EINTR) {
            ត្រឡប់ -1;
        }

        ត្រឡប់ -1;
    }
}

ចំនួនគត់ freertos_linux_coop_accept(ចំនួនគត់ sockfd, sa_nracanaa sockaddr *addr, socklen_t *addrlen)
{
    ខណៈពេល (1)
    {
        ចំនួនគត់ fd = real_accept(sockfd, addr, addrlen);
        ប្រសិនបើ (fd >= 0) {
            linux_coop_set_nonblocking(fd);
            linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(sockfd) ? O_NONBLOCK : 0);
            ត្រឡប់ fd;
        }
        ប្រសិនបើ (errno == EAGAIN || errno == EWOULDBLOCK) {
            ប្រសិនបើ (linux_coop_fd_user_nonblocking(sockfd)) {
                ត្រឡប់ -1;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            continue;
        }
        ត្រឡប់ -1;
    }
}

ចំនួនគត់ freertos_linux_coop_pselect(ចំនួនគត់ nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, const sa_nracanaa timespec *timeout,
            const sigset_t *sigmask)
{
    long timeout_ms = -1;
    ប្រសិនបើ (timeout != NULL) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_nsec + 999999) / 1000000;
        ប្រសិនបើ (timeout_ms == 0) {
            ត្រឡប់ real_pselect(nfds, readfds, writefds, exceptfds,
                                timeout, sigmask);
        }
    }

    long waited_ms = 0;
    ខណៈពេល (1)
    {
        sa_nracanaa timespec zero_ts = {0, 0};
        ចំនួនគត់ ret = real_pselect(nfds, readfds, writefds, exceptfds,
                               &zero_ts, sigmask);
        ប្រសិនបើ (ret != 0) {
            ត្រឡប់ ret;
        }

        ប្រសិនបើ (timeout_ms == 0 ||
            (timeout_ms > 0 && waited_ms >= timeout_ms)) {
            ត្រឡប់ 0;
        }

        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

ចំនួនគត់ freertos_linux_coop_poll(sa_nracanaa pollfd *fds, nfds_t nfds, ចំនួនគត់ timeout)
{
    ប្រសិនបើ (timeout == 0) {
        ត្រឡប់ real_poll(fds, nfds, 0);
    }

    long timeout_ms = -1;
    ប្រសិនបើ (timeout > 0) {
        timeout_ms = timeout;
    }

    long waited_ms = 0;

    ខណៈពេល (1)
    {
        ចំនួនគត់ ret = real_poll(fds, nfds, 0);
        ប្រសិនបើ (ret > 0) {
            ត្រឡប់ ret;
        } បើមិនដូច្នេះ ប្រសិនបើ (ret == 0) {
            ប្រសិនបើ (timeout_ms == 0 ||
                (timeout_ms > 0 && waited_ms >= timeout_ms)) {
                ត្រឡប់ 0;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            waited_ms += LINUX_COOP_TICK_MS;
            continue;
        } បើមិនដូច្នេះ {
            ប្រសិនបើ (errno == EINTR) {
                continue;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            ប្រសិនបើ (timeout_ms > 0 && waited_ms >= timeout_ms) {
                ត្រឡប់ -1;
            }
            waited_ms += LINUX_COOP_TICK_MS;
        }
    }
}

ចំនួនគត់ nanosleep(const sa_nracanaa timespec *req, sa_nracanaa timespec *rem)
{
    ប្រសិនបើ (req == NULL) {
        errno = EFAULT;
        ត្រឡប់ -1;
    }
    ប្រសិនបើ (!linux_port_in_freertos_task()) {
        ត្រឡប់ real_nanosleep(req, rem);
    }
    long ms = req->tv_sec * 1000 + (req->tv_nsec + 999999) / 1000000;
    ប្រសិនបើ (ms <= 0) {
        ms = 1;
    }
    vTaskDelay(ms);
    ប្រសិនបើ (rem) {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }
    ត្រឡប់ 0;
}

unsigned ចំនួនគត់ sleep(unsigned ចំនួនគត់ seconds)
{
    linux_coop_yield(seconds * 1000);
    ត្រឡប់ 0;
}

ចំនួនគត់ usleep(useconds_t usec)
{
    linux_coop_yield(usec / 1000);
    ត្រឡប់ 0;
}

ចំនួនគត់ freertos_linux_coop_socket(ចំនួនគត់ domain, ចំនួនគត់ type, ចំនួនគត់ protocol)
{
    ចំនួនគត់ fd = real_socket(domain, type, protocol);
    linux_coop_set_nonblocking(fd);
#ifdef SOCK_NONBLOCK
    const ចំនួនគត់ user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#បើមិនដូច្នេះ
    const ចំនួនគត់ user_flags = 0;
#endif
    linux_coop_track_fd(fd, user_flags);
    ត្រឡប់ fd;
}

ចំនួនគត់ freertos_linux_coop_socketpair(ចំនួនគត់ domain, ចំនួនគត់ type, ចំនួនគត់ protocol, ចំនួនគត់ *sv)
{
    ចំនួនគត់ ret = real_socketpair(domain, type, protocol, sv);
    ប្រសិនបើ (ret == 0) {
        linux_coop_set_nonblocking(sv[0]);
        linux_coop_set_nonblocking(sv[1]);
#ifdef SOCK_NONBLOCK
        const ចំនួនគត់ user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#បើមិនដូច្នេះ
        const ចំនួនគត់ user_flags = 0;
#endif
        linux_coop_track_fd(sv[0], user_flags);
        linux_coop_track_fd(sv[1], user_flags);
    }
    ត្រឡប់ ret;
}

ចំនួនគត់ freertos_linux_coop_pipe(ចំនួនគត់ *fds)
{
    ចំនួនគត់ ret = real_pipe(fds);
    ប្រសិនបើ (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        linux_coop_track_fd(fds[0], 0);
        linux_coop_track_fd(fds[1], 0);
    }
    ត្រឡប់ ret;
}

ចំនួនគត់ freertos_linux_coop_pipe2(ចំនួនគត់ *fds, ចំនួនគត់ flags)
{
    ចំនួនគត់ ret = real_pipe2(fds, flags);
    ប្រសិនបើ (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        const ចំនួនគត់ user_flags = (flags & O_NONBLOCK) ? O_NONBLOCK : 0;
        linux_coop_track_fd(fds[0], user_flags);
        linux_coop_track_fd(fds[1], user_flags);
    }
    ត្រឡប់ ret;
}

ចំនួនគត់ freertos_linux_coop_dup(ចំនួនគត់ oldfd)
{
    ចំនួនគត់ fd = real_dup(oldfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    ត្រឡប់ fd;
}

ចំនួនគត់ freertos_linux_coop_dup2(ចំនួនគត់ oldfd, ចំនួនគត់ newfd)
{
    ចំនួនគត់ fd = real_dup2(oldfd, newfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    ត្រឡប់ fd;
}

__attribute__((weak)) ចំនួនគត់ connect(ចំនួនគត់ sockfd, const sa_nracanaa sockaddr *addr, socklen_t addrlen)
{
    ត្រឡប់ freertos_linux_coop_connect(sockfd, addr, addrlen);
}

__attribute__((weak)) ចំនួនគត់ accept(ចំនួនគត់ sockfd, sa_nracanaa sockaddr *addr, socklen_t *addrlen)
{
    ត្រឡប់ freertos_linux_coop_accept(sockfd, addr, addrlen);
}

__attribute__((weak)) ចំនួនគត់ pselect(ចំនួនគត់ nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, const sa_nracanaa timespec *timeout,
            const sigset_t *sigmask)
{
    ត្រឡប់ freertos_linux_coop_pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}

__attribute__((weak)) ចំនួនគត់ poll(sa_nracanaa pollfd *fds, nfds_t nfds, ចំនួនគត់ timeout)
{
    ត្រឡប់ freertos_linux_coop_poll(fds, nfds, timeout);
}

__attribute__((weak)) ចំនួនគត់ socket(ចំនួនគត់ domain, ចំនួនគត់ type, ចំនួនគត់ protocol)
{
    ត្រឡប់ freertos_linux_coop_socket(domain, type, protocol);
}

__attribute__((weak)) ចំនួនគត់ socketpair(ចំនួនគត់ domain, ចំនួនគត់ type, ចំនួនគត់ protocol, ចំនួនគត់ sv[2])
{
    ត្រឡប់ freertos_linux_coop_socketpair(domain, type, protocol, sv);
}

__attribute__((weak)) ចំនួនគត់ pipe(ចំនួនគត់ fds[2])
{
    ត្រឡប់ freertos_linux_coop_pipe(fds);
}

__attribute__((weak)) ចំនួនគត់ pipe2(ចំនួនគត់ fds[2], ចំនួនគត់ flags)
{
    ត្រឡប់ freertos_linux_coop_pipe2(fds, flags);
}

__attribute__((weak)) ចំនួនគត់ dup(ចំនួនគត់ oldfd)
{
    ត្រឡប់ freertos_linux_coop_dup(oldfd);
}

__attribute__((weak)) ចំនួនគត់ dup2(ចំនួនគត់ oldfd, ចំនួនគត់ newfd)
{
    ត្រឡប់ freertos_linux_coop_dup2(oldfd, newfd);
}
