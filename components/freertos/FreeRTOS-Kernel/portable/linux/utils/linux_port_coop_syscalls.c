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

static ssize_t  (*real_readv)(ཧྲིལ, const sa_nracanaa iovec *, ཧྲིལ);
static ssize_t  (*real_writev)(ཧྲིལ, const sa_nracanaa iovec *, ཧྲིལ);
static ssize_t  (*real_recv)(ཧྲིལ, void *, size_t, ཧྲིལ);
static ssize_t  (*real_send)(ཧྲིལ, const void *, size_t, ཧྲིལ);
static ssize_t  (*real_recvfrom)(ཧྲིལ, void *, size_t, ཧྲིལ,
                                 sa_nracanaa sockaddr *, socklen_t *);
static ssize_t  (*real_sendto)(ཧྲིལ, const void *, size_t, ཧྲིལ,
                                const sa_nracanaa sockaddr *, socklen_t);
static ssize_t  (*real_recvmsg)(ཧྲིལ, sa_nracanaa msghdr *, ཧྲིལ);
static ssize_t  (*real_sendmsg)(ཧྲིལ, const sa_nracanaa msghdr *, ཧྲིལ);
static ཧྲིལ      (*real_connect)(ཧྲིལ, const sa_nracanaa sockaddr *, socklen_t);
static ཧྲིལ      (*real_accept)(ཧྲིལ, sa_nracanaa sockaddr *, socklen_t *);
static ཧྲིལ      (*real_pselect)(ཧྲིལ, fd_set *, fd_set *, fd_set *,
                                const sa_nracanaa timespec *, const sigset_t *);
static ཧྲིལ      (*real_poll)(sa_nracanaa pollfd *, nfds_t, ཧྲིལ);
static ཧྲིལ      (*real_socket)(ཧྲིལ, ཧྲིལ, ཧྲིལ);
static ཧྲིལ      (*real_socketpair)(ཧྲིལ, ཧྲིལ, ཧྲིལ, ཧྲིལ[2]);
static ཧྲིལ      (*real_pipe)(ཧྲིལ[2]);
static ཧྲིལ      (*real_pipe2)(ཧྲིལ[2], ཧྲིལ);
static ཧྲིལ      (*real_dup)(ཧྲིལ);
static ཧྲིལ      (*real_dup2)(ཧྲིལ, ཧྲིལ);
static ཧྲིལ      (*real_nanosleep)(const sa_nracanaa timespec *, sa_nracanaa timespec *);
static ssize_t  (*real_read)(ཧྲིལ, void *, size_t);
static ssize_t  (*real_write)(ཧྲིལ, const void *, size_t);
static ssize_t  (*real_pread)(ཧྲིལ, void *, size_t, off_t);
static ssize_t  (*real_pwrite)(ཧྲིལ, const void *, size_t, off_t);
static ཧྲིལ      (*real_close)(ཧྲིལ);
static ཧྲིལ      (*real_select)(ཧྲིལ, fd_set *, fd_set *, fd_set *,
                                sa_nracanaa timeval *);

typedef ཧྲིལ (*linux_coop_fcntl_fn_t)(ཧྲིལ, ཧྲིལ, ...);
static linux_coop_fcntl_fn_t real_fcntl;

typedef ཧྲིལ (*linux_coop_open_fn_t)(const var_na *, ཧྲིལ, mode_t);
static linux_coop_open_fn_t real_open;

typedef sa_nracanaa linux_coop_fd_state {
    ཧྲིལ fd;
    ཧྲིལ user_flags;
    sa_nracanaa linux_coop_fd_state *next;
} linux_coop_fd_state_t;

static linux_coop_fd_state_t *s_fd_state_list;
static pthread_mutex_t s_fd_state_lock = PTHREAD_MUTEX_INITIALIZER;

static linux_coop_fd_state_t *linux_coop_find_fd_locked(ཧྲིལ fd)
{
    linux_coop_fd_state_t *node = s_fd_state_list;
    སྐབས (node != NULL) {
        གལ་སྲིད (node->fd == fd) {
            ལོག node;
        }
        node = node->next;
    }
    ལོག NULL;
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

void linux_coop_set_nonblocking(ཧྲིལ fd)
{
    གལ་སྲིད (fd >= 0) {
        ཧྲིལ flags = real_fcntl(fd, F_GETFL, 0);
        གལ་སྲིད (flags >= 0) {
            real_fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
    }
}

void linux_coop_track_fd(ཧྲིལ fd, ཧྲིལ user_flags)
{
    གལ་སྲིད (fd < 0) {
        ལོག;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *node = linux_coop_find_fd_locked(fd);
    གལ་སྲིད (node != NULL) {
        node->user_flags = user_flags;
        pthread_mutex_unlock(&s_fd_state_lock);
        ལོག;
    }

    node = calloc(1, parimaa_na(*node));
    གལ་སྲིད (node != NULL) {
        node->fd = fd;
        node->user_flags = user_flags;
        node->next = s_fd_state_list;
        s_fd_state_list = node;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

void linux_coop_untrack_fd(ཧྲིལ fd)
{
    གལ་སྲིད (fd < 0) {
        ལོག;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *prev = NULL;
    linux_coop_fd_state_t *node = s_fd_state_list;
    སྐབས (node != NULL) {
        གལ་སྲིད (node->fd == fd) {
            གལ་སྲིད (prev == NULL) {
                s_fd_state_list = node->next;
            } དེ་མིན {
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

void linux_coop_set_user_flags(ཧྲིལ fd, ཧྲིལ user_flags)
{
    linux_coop_track_fd(fd, user_flags);
}

bool linux_coop_get_user_flags(ཧྲིལ fd, ཧྲིལ *flags)
{
    གལ་སྲིད (fd < 0 || flags == NULL) {
        ལོག false;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *node = linux_coop_find_fd_locked(fd);
    གལ་སྲིད (node == NULL) {
        pthread_mutex_unlock(&s_fd_state_lock);
        ལོག false;
    }
    *flags = node->user_flags;
    pthread_mutex_unlock(&s_fd_state_lock);
    ལོག true;
}

bool linux_coop_fd_user_nonblocking(ཧྲིལ fd)
{
    ཧྲིལ user_flags = 0;
    གལ་སྲིད (linux_coop_get_user_flags(fd, &user_flags)) {
        ལོག (user_flags & O_NONBLOCK) != 0;
    }
    ལོག false;
}


ssize_t freertos_linux_coop_read(ཧྲིལ fd, void *buf, size_t count)
{
    LINUX_COOP_IO_LOOP(fd, real_read(fd, buf, count))
}

ssize_t freertos_linux_coop_write(ཧྲིལ fd, const void *buf, size_t count)
{
    LINUX_COOP_IO_LOOP(fd, real_write(fd, buf, count))
}

ssize_t freertos_linux_coop_pread(ཧྲིལ fd, void *buf, size_t count, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pread(fd, buf, count, offset))
}

ssize_t freertos_linux_coop_pwrite(ཧྲིལ fd, const void *buf, size_t count, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pwrite(fd, buf, count, offset))
}

ཧྲིལ freertos_linux_coop_open(const var_na *path, ཧྲིལ flags, ཧྲིལ mode)
{
    ཧྲིལ fd = real_open(path, flags, (mode_t)mode);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, flags & O_NONBLOCK);
    ལོག fd;
}

ཧྲིལ freertos_linux_coop_close(ཧྲིལ fd)
{
    ཧྲིལ ret = real_close(fd);
    གལ་སྲིད (ret == 0 || errno == EINTR) {
        /* On Linux, close() always releases the fd even when returning
         * EINTR.  Retrying would risk closing a fd reused by another
         * thread.  Treat EINTR as success. */
        linux_coop_untrack_fd(fd);
        ལོག 0;
    }
    ལོག -1;
}

ཧྲིལ freertos_linux_coop_fcntl(ཧྲིལ fd, ཧྲིལ cmd, ཧྲིལ arg)
{
    གལ་སྲིད (cmd == F_GETFL) {
        ཧྲིལ real_flags = real_fcntl(fd, F_GETFL, 0);
        གལ་སྲིད (real_flags < 0) {
            ལོག -1;
        }
        ཧྲིལ user_flags = 0;
        གལ་སྲིད (linux_coop_get_user_flags(fd, &user_flags)) {
            real_flags &= ~O_NONBLOCK;
            real_flags |= (user_flags & O_NONBLOCK);
        }
        ལོག real_flags;
    }
    གལ་སྲིད (cmd == F_SETFL) {
        linux_coop_set_user_flags(fd, arg);
        ཧྲིལ internal_flags = arg | O_NONBLOCK;
        ལོག real_fcntl(fd, F_SETFL, internal_flags);
    }
    ལོག real_fcntl(fd, cmd, arg);
}

ཧྲིལ freertos_linux_coop_select(ཧྲིལ nfds, fd_set *readfds, fd_set *writefds,
                      fd_set *exceptfds, sa_nracanaa timeval *timeout)
{
    long timeout_ms = -1;
    གལ་སྲིད (timeout != NULL) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_usec + 999) / 1000;
        གལ་སྲིད (timeout_ms == 0) {
            ལོག real_select(nfds, readfds, writefds, exceptfds, timeout);
        }
    }

    /* real_select() mutates the fd_sets in place (clears them, then sets bits
     * for ready descriptors). To keep polling the same descriptors across
     * iterations we must snapshot the caller's sets and restore them on every
     * loop. */
    fd_set rfds_in, wfds_in, efds_in;
    གལ་སྲིད (readfds != NULL) {
        rfds_in = *readfds;
    }
    གལ་སྲིད (writefds != NULL) {
        wfds_in = *writefds;
    }
    གལ་སྲིད (exceptfds != NULL) {
        efds_in = *exceptfds;
    }

    long waited_ms = 0;
    སྐབས (1) {
        གལ་སྲིད (readfds != NULL) {
            *readfds = rfds_in;
        }
        གལ་སྲིད (writefds != NULL) {
            *writefds = wfds_in;
        }
        གལ་སྲིད (exceptfds != NULL) {
            *exceptfds = efds_in;
        }

        sa_nracanaa timeval zero_tv = {0, 0};
        ཧྲིལ ret = real_select(nfds, readfds, writefds, exceptfds, &zero_tv);
        གལ་སྲིད (ret != 0) {
            ལོག ret;
        }
        གལ་སྲིད (timeout_ms == 0) {
            ལོག 0;
        }
        གལ་སྲིད (timeout_ms > 0 && waited_ms >= timeout_ms) {
            ལོག 0;
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

__attribute__((weak)) ssize_t read(ཧྲིལ fd, void *buf, size_t count)
{
    ལོག freertos_linux_coop_read(fd, buf, count);
}

__attribute__((weak)) ssize_t write(ཧྲིལ fd, const void *buf, size_t count)
{
    ལོག freertos_linux_coop_write(fd, buf, count);
}

__attribute__((weak)) ssize_t pread(ཧྲིལ fd, void *buf, size_t count, off_t offset)
{
    ལོག freertos_linux_coop_pread(fd, buf, count, offset);
}

__attribute__((weak)) ssize_t pwrite(ཧྲིལ fd, const void *buf, size_t count, off_t offset)
{
    ལོག freertos_linux_coop_pwrite(fd, buf, count, offset);
}

__attribute__((weak)) ཧྲིལ open(const var_na *path, ཧྲིལ flags, ...)
{
    ཧྲིལ mode = 0;
    གལ་སྲིད (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        mode = va_arg(ap, ཧྲིལ);
        va_end(ap);
    }
    ལོག freertos_linux_coop_open(path, flags, mode);
}

__attribute__((weak)) ཧྲིལ close(ཧྲིལ fd)
{
    ལོག freertos_linux_coop_close(fd);
}

__attribute__((weak)) ཧྲིལ fcntl(ཧྲིལ fd, ཧྲིལ cmd, ...)
{
    ཧྲིལ arg = 0;
    གལ་སྲིད (cmd != F_GETFL) {
        va_list list;
        va_start(list, cmd);
        arg = va_arg(list, ཧྲིལ);
        va_end(list);
    }
    ལོག freertos_linux_coop_fcntl(fd, cmd, arg);
}

__attribute__((weak)) ཧྲིལ select(ཧྲིལ nfds, fd_set *readfds, fd_set *writefds,
                                 fd_set *exceptfds, sa_nracanaa timeval *timeout)
{
    ལོག freertos_linux_coop_select(nfds, readfds, writefds, exceptfds, timeout);
}

ssize_t freertos_linux_coop_readv(ཧྲིལ fd, const sa_nracanaa iovec *iov, ཧྲིལ iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_readv(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_writev(ཧྲིལ fd, const sa_nracanaa iovec *iov, ཧྲིལ iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_writev(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_recv(ཧྲིལ sockfd, void *buf, size_t len, ཧྲིལ flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recv(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_send(ཧྲིལ sockfd, const void *buf, size_t len, ཧྲིལ flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_send(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_recvfrom(ཧྲིལ sockfd, void *buf, size_t len, ཧྲིལ flags,
                 sa_nracanaa sockaddr *src_addr, socklen_t *addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvfrom(sockfd, buf, len, flags | MSG_DONTWAIT,
                            src_addr, addrlen))
}

ssize_t freertos_linux_coop_sendto(ཧྲིལ sockfd, const void *buf, size_t len, ཧྲིལ flags,
               const sa_nracanaa sockaddr *dest_addr, socklen_t addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendto(sockfd, buf, len, flags | MSG_DONTWAIT,
                          dest_addr, addrlen))
}

ssize_t freertos_linux_coop_recvmsg(ཧྲིལ sockfd, sa_nracanaa msghdr *msg, ཧྲིལ flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvmsg(sockfd, msg, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_sendmsg(ཧྲིལ sockfd, const sa_nracanaa msghdr *msg, ཧྲིལ flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendmsg(sockfd, msg, flags | MSG_DONTWAIT))
}

__attribute__((weak)) ssize_t readv(ཧྲིལ fd, const sa_nracanaa iovec *iov, ཧྲིལ iovcnt)
{
    ལོག freertos_linux_coop_readv(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t writev(ཧྲིལ fd, const sa_nracanaa iovec *iov, ཧྲིལ iovcnt)
{
    ལོག freertos_linux_coop_writev(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t recv(ཧྲིལ sockfd, void *buf, size_t len, ཧྲིལ flags)
{
    ལོག freertos_linux_coop_recv(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t send(ཧྲིལ sockfd, const void *buf, size_t len, ཧྲིལ flags)
{
    ལོག freertos_linux_coop_send(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t recvfrom(ཧྲིལ sockfd, void *buf, size_t len, ཧྲིལ flags,
                 sa_nracanaa sockaddr *src_addr, socklen_t *addrlen)
{
    ལོག freertos_linux_coop_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

__attribute__((weak)) ssize_t sendto(ཧྲིལ sockfd, const void *buf, size_t len, ཧྲིལ flags,
               const sa_nracanaa sockaddr *dest_addr, socklen_t addrlen)
{
    ལོག freertos_linux_coop_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

__attribute__((weak)) ssize_t recvmsg(ཧྲིལ sockfd, sa_nracanaa msghdr *msg, ཧྲིལ flags)
{
    ལོག freertos_linux_coop_recvmsg(sockfd, msg, flags);
}

__attribute__((weak)) ssize_t sendmsg(ཧྲིལ sockfd, const sa_nracanaa msghdr *msg, ཧྲིལ flags)
{
    ལོག freertos_linux_coop_sendmsg(sockfd, msg, flags);
}

ཧྲིལ freertos_linux_coop_connect(ཧྲིལ sockfd, const sa_nracanaa sockaddr *addr, socklen_t addrlen)
{
    སྐབས (1)
    {
        ཧྲིལ ret = real_connect(sockfd, addr, addrlen);
        གལ་སྲིད (ret == 0) {
            ལོག ret;
        }

        གལ་སྲིད (errno == EINPROGRESS || errno == EALREADY) {
            sa_nracanaa pollfd pfd;
            pfd.fd = sockfd;
            pfd.events = POLLOUT;
            pfd.revents = 0;

            ཧྲིལ poll_retries = 0;
            སྐབས (1)
            {
                ཧྲིལ press = real_poll(&pfd, 1, 0);
                གལ་སྲིད (press > 0) {
                    ཧྲིལ so_err = 0;
                    socklen_t len = parimaa_na(so_err);
                    གལ་སྲིད (getsockopt(sockfd, SOL_SOCKET, SO_ERROR,
                                   &so_err, &len) < 0) {
                        ལོག -1;
                    }
                    གལ་སྲིད (so_err == 0) {
                        ལོག 0;
                    } དེ་མིན {
                        errno = so_err;
                        ལོག -1;
                    }
                } དེ་མིན གལ་སྲིད (press == 0) {
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    continue;
                } དེ་མིན {
                    གལ་སྲིད (errno == EINTR) {
                        continue;
                    }
                    /* Non-EINTR poll error; bail out after a few retries */
                    གལ་སྲིད (++poll_retries > 3) {
                        ལོག -1;
                    }
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    continue;
                }
            }
        }

        གལ་སྲིད (errno == EINTR) {
            ལོག -1;
        }

        ལོག -1;
    }
}

ཧྲིལ freertos_linux_coop_accept(ཧྲིལ sockfd, sa_nracanaa sockaddr *addr, socklen_t *addrlen)
{
    སྐབས (1)
    {
        ཧྲིལ fd = real_accept(sockfd, addr, addrlen);
        གལ་སྲིད (fd >= 0) {
            linux_coop_set_nonblocking(fd);
            linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(sockfd) ? O_NONBLOCK : 0);
            ལོག fd;
        }
        གལ་སྲིད (errno == EAGAIN || errno == EWOULDBLOCK) {
            གལ་སྲིད (linux_coop_fd_user_nonblocking(sockfd)) {
                ལོག -1;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            continue;
        }
        ལོག -1;
    }
}

ཧྲིལ freertos_linux_coop_pselect(ཧྲིལ nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, const sa_nracanaa timespec *timeout,
            const sigset_t *sigmask)
{
    long timeout_ms = -1;
    གལ་སྲིད (timeout != NULL) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_nsec + 999999) / 1000000;
        གལ་སྲིད (timeout_ms == 0) {
            ལོག real_pselect(nfds, readfds, writefds, exceptfds,
                                timeout, sigmask);
        }
    }

    long waited_ms = 0;
    སྐབས (1)
    {
        sa_nracanaa timespec zero_ts = {0, 0};
        ཧྲིལ ret = real_pselect(nfds, readfds, writefds, exceptfds,
                               &zero_ts, sigmask);
        གལ་སྲིད (ret != 0) {
            ལོག ret;
        }

        གལ་སྲིད (timeout_ms == 0 ||
            (timeout_ms > 0 && waited_ms >= timeout_ms)) {
            ལོག 0;
        }

        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

ཧྲིལ freertos_linux_coop_poll(sa_nracanaa pollfd *fds, nfds_t nfds, ཧྲིལ timeout)
{
    གལ་སྲིད (timeout == 0) {
        ལོག real_poll(fds, nfds, 0);
    }

    long timeout_ms = -1;
    གལ་སྲིད (timeout > 0) {
        timeout_ms = timeout;
    }

    long waited_ms = 0;

    སྐབས (1)
    {
        ཧྲིལ ret = real_poll(fds, nfds, 0);
        གལ་སྲིད (ret > 0) {
            ལོག ret;
        } དེ་མིན གལ་སྲིད (ret == 0) {
            གལ་སྲིད (timeout_ms == 0 ||
                (timeout_ms > 0 && waited_ms >= timeout_ms)) {
                ལོག 0;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            waited_ms += LINUX_COOP_TICK_MS;
            continue;
        } དེ་མིན {
            གལ་སྲིད (errno == EINTR) {
                continue;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            གལ་སྲིད (timeout_ms > 0 && waited_ms >= timeout_ms) {
                ལོག -1;
            }
            waited_ms += LINUX_COOP_TICK_MS;
        }
    }
}

ཧྲིལ nanosleep(const sa_nracanaa timespec *req, sa_nracanaa timespec *rem)
{
    གལ་སྲིད (req == NULL) {
        errno = EFAULT;
        ལོག -1;
    }
    གལ་སྲིད (!linux_port_in_freertos_task()) {
        ལོག real_nanosleep(req, rem);
    }
    long ms = req->tv_sec * 1000 + (req->tv_nsec + 999999) / 1000000;
    གལ་སྲིད (ms <= 0) {
        ms = 1;
    }
    vTaskDelay(ms);
    གལ་སྲིད (rem) {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }
    ལོག 0;
}

unsigned ཧྲིལ sleep(unsigned ཧྲིལ seconds)
{
    linux_coop_yield(seconds * 1000);
    ལོག 0;
}

ཧྲིལ usleep(useconds_t usec)
{
    linux_coop_yield(usec / 1000);
    ལོག 0;
}

ཧྲིལ freertos_linux_coop_socket(ཧྲིལ domain, ཧྲིལ type, ཧྲིལ protocol)
{
    ཧྲིལ fd = real_socket(domain, type, protocol);
    linux_coop_set_nonblocking(fd);
#ifdef SOCK_NONBLOCK
    const ཧྲིལ user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#དེ་མིན
    const ཧྲིལ user_flags = 0;
#endif
    linux_coop_track_fd(fd, user_flags);
    ལོག fd;
}

ཧྲིལ freertos_linux_coop_socketpair(ཧྲིལ domain, ཧྲིལ type, ཧྲིལ protocol, ཧྲིལ *sv)
{
    ཧྲིལ ret = real_socketpair(domain, type, protocol, sv);
    གལ་སྲིད (ret == 0) {
        linux_coop_set_nonblocking(sv[0]);
        linux_coop_set_nonblocking(sv[1]);
#ifdef SOCK_NONBLOCK
        const ཧྲིལ user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#དེ་མིན
        const ཧྲིལ user_flags = 0;
#endif
        linux_coop_track_fd(sv[0], user_flags);
        linux_coop_track_fd(sv[1], user_flags);
    }
    ལོག ret;
}

ཧྲིལ freertos_linux_coop_pipe(ཧྲིལ *fds)
{
    ཧྲིལ ret = real_pipe(fds);
    གལ་སྲིད (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        linux_coop_track_fd(fds[0], 0);
        linux_coop_track_fd(fds[1], 0);
    }
    ལོག ret;
}

ཧྲིལ freertos_linux_coop_pipe2(ཧྲིལ *fds, ཧྲིལ flags)
{
    ཧྲིལ ret = real_pipe2(fds, flags);
    གལ་སྲིད (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        const ཧྲིལ user_flags = (flags & O_NONBLOCK) ? O_NONBLOCK : 0;
        linux_coop_track_fd(fds[0], user_flags);
        linux_coop_track_fd(fds[1], user_flags);
    }
    ལོག ret;
}

ཧྲིལ freertos_linux_coop_dup(ཧྲིལ oldfd)
{
    ཧྲིལ fd = real_dup(oldfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    ལོག fd;
}

ཧྲིལ freertos_linux_coop_dup2(ཧྲིལ oldfd, ཧྲིལ newfd)
{
    ཧྲིལ fd = real_dup2(oldfd, newfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    ལོག fd;
}

__attribute__((weak)) ཧྲིལ connect(ཧྲིལ sockfd, const sa_nracanaa sockaddr *addr, socklen_t addrlen)
{
    ལོག freertos_linux_coop_connect(sockfd, addr, addrlen);
}

__attribute__((weak)) ཧྲིལ accept(ཧྲིལ sockfd, sa_nracanaa sockaddr *addr, socklen_t *addrlen)
{
    ལོག freertos_linux_coop_accept(sockfd, addr, addrlen);
}

__attribute__((weak)) ཧྲིལ pselect(ཧྲིལ nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, const sa_nracanaa timespec *timeout,
            const sigset_t *sigmask)
{
    ལོག freertos_linux_coop_pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}

__attribute__((weak)) ཧྲིལ poll(sa_nracanaa pollfd *fds, nfds_t nfds, ཧྲིལ timeout)
{
    ལོག freertos_linux_coop_poll(fds, nfds, timeout);
}

__attribute__((weak)) ཧྲིལ socket(ཧྲིལ domain, ཧྲིལ type, ཧྲིལ protocol)
{
    ལོག freertos_linux_coop_socket(domain, type, protocol);
}

__attribute__((weak)) ཧྲིལ socketpair(ཧྲིལ domain, ཧྲིལ type, ཧྲིལ protocol, ཧྲིལ sv[2])
{
    ལོག freertos_linux_coop_socketpair(domain, type, protocol, sv);
}

__attribute__((weak)) ཧྲིལ pipe(ཧྲིལ fds[2])
{
    ལོག freertos_linux_coop_pipe(fds);
}

__attribute__((weak)) ཧྲིལ pipe2(ཧྲིལ fds[2], ཧྲིལ flags)
{
    ལོག freertos_linux_coop_pipe2(fds, flags);
}

__attribute__((weak)) ཧྲིལ dup(ཧྲིལ oldfd)
{
    ལོག freertos_linux_coop_dup(oldfd);
}

__attribute__((weak)) ཧྲིལ dup2(ཧྲིལ oldfd, ཧྲིལ newfd)
{
    ལོག freertos_linux_coop_dup2(oldfd, newfd);
}
