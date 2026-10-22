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

static ssize_t  (*real_readv)(ကိန်းပြည့်, const sa_nracanaa iovec *, ကိန်းပြည့်);
static ssize_t  (*real_writev)(ကိန်းပြည့်, const sa_nracanaa iovec *, ကိန်းပြည့်);
static ssize_t  (*real_recv)(ကိန်းပြည့်, void *, size_t, ကိန်းပြည့်);
static ssize_t  (*real_send)(ကိန်းပြည့်, const void *, size_t, ကိန်းပြည့်);
static ssize_t  (*real_recvfrom)(ကိန်းပြည့်, void *, size_t, ကိန်းပြည့်,
                                 sa_nracanaa sockaddr *, socklen_t *);
static ssize_t  (*real_sendto)(ကိန်းပြည့်, const void *, size_t, ကိန်းပြည့်,
                                const sa_nracanaa sockaddr *, socklen_t);
static ssize_t  (*real_recvmsg)(ကိန်းပြည့်, sa_nracanaa msghdr *, ကိန်းပြည့်);
static ssize_t  (*real_sendmsg)(ကိန်းပြည့်, const sa_nracanaa msghdr *, ကိန်းပြည့်);
static ကိန်းပြည့်      (*real_connect)(ကိန်းပြည့်, const sa_nracanaa sockaddr *, socklen_t);
static ကိန်းပြည့်      (*real_accept)(ကိန်းပြည့်, sa_nracanaa sockaddr *, socklen_t *);
static ကိန်းပြည့်      (*real_pselect)(ကိန်းပြည့်, fd_set *, fd_set *, fd_set *,
                                const sa_nracanaa timespec *, const sigset_t *);
static ကိန်းပြည့်      (*real_poll)(sa_nracanaa pollfd *, nfds_t, ကိန်းပြည့်);
static ကိန်းပြည့်      (*real_socket)(ကိန်းပြည့်, ကိန်းပြည့်, ကိန်းပြည့်);
static ကိန်းပြည့်      (*real_socketpair)(ကိန်းပြည့်, ကိန်းပြည့်, ကိန်းပြည့်, ကိန်းပြည့်[2]);
static ကိန်းပြည့်      (*real_pipe)(ကိန်းပြည့်[2]);
static ကိန်းပြည့်      (*real_pipe2)(ကိန်းပြည့်[2], ကိန်းပြည့်);
static ကိန်းပြည့်      (*real_dup)(ကိန်းပြည့်);
static ကိန်းပြည့်      (*real_dup2)(ကိန်းပြည့်, ကိန်းပြည့်);
static ကိန်းပြည့်      (*real_nanosleep)(const sa_nracanaa timespec *, sa_nracanaa timespec *);
static ssize_t  (*real_read)(ကိန်းပြည့်, void *, size_t);
static ssize_t  (*real_write)(ကိန်းပြည့်, const void *, size_t);
static ssize_t  (*real_pread)(ကိန်းပြည့်, void *, size_t, off_t);
static ssize_t  (*real_pwrite)(ကိန်းပြည့်, const void *, size_t, off_t);
static ကိန်းပြည့်      (*real_close)(ကိန်းပြည့်);
static ကိန်းပြည့်      (*real_select)(ကိန်းပြည့်, fd_set *, fd_set *, fd_set *,
                                sa_nracanaa timeval *);

typedef ကိန်းပြည့် (*linux_coop_fcntl_fn_t)(ကိန်းပြည့်, ကိန်းပြည့်, ...);
static linux_coop_fcntl_fn_t real_fcntl;

typedef ကိန်းပြည့် (*linux_coop_open_fn_t)(const var_na *, ကိန်းပြည့်, mode_t);
static linux_coop_open_fn_t real_open;

typedef sa_nracanaa linux_coop_fd_state {
    ကိန်းပြည့် fd;
    ကိန်းပြည့် user_flags;
    sa_nracanaa linux_coop_fd_state *next;
} linux_coop_fd_state_t;

static linux_coop_fd_state_t *s_fd_state_list;
static pthread_mutex_t s_fd_state_lock = PTHREAD_MUTEX_INITIALIZER;

static linux_coop_fd_state_t *linux_coop_find_fd_locked(ကိန်းပြည့် fd)
{
    linux_coop_fd_state_t *node = s_fd_state_list;
    နေစဉ် (node != NULL) {
        အကယ်၍ (node->fd == fd) {
            ပြန်ပေး node;
        }
        node = node->next;
    }
    ပြန်ပေး NULL;
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

void linux_coop_set_nonblocking(ကိန်းပြည့် fd)
{
    အကယ်၍ (fd >= 0) {
        ကိန်းပြည့် flags = real_fcntl(fd, F_GETFL, 0);
        အကယ်၍ (flags >= 0) {
            real_fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
    }
}

void linux_coop_track_fd(ကိန်းပြည့် fd, ကိန်းပြည့် user_flags)
{
    အကယ်၍ (fd < 0) {
        ပြန်ပေး;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *node = linux_coop_find_fd_locked(fd);
    အကယ်၍ (node != NULL) {
        node->user_flags = user_flags;
        pthread_mutex_unlock(&s_fd_state_lock);
        ပြန်ပေး;
    }

    node = calloc(1, parimaa_na(*node));
    အကယ်၍ (node != NULL) {
        node->fd = fd;
        node->user_flags = user_flags;
        node->next = s_fd_state_list;
        s_fd_state_list = node;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

void linux_coop_untrack_fd(ကိန်းပြည့် fd)
{
    အကယ်၍ (fd < 0) {
        ပြန်ပေး;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *prev = NULL;
    linux_coop_fd_state_t *node = s_fd_state_list;
    နေစဉ် (node != NULL) {
        အကယ်၍ (node->fd == fd) {
            အကယ်၍ (prev == NULL) {
                s_fd_state_list = node->next;
            } မဟုတ်ပါက {
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

void linux_coop_set_user_flags(ကိန်းပြည့် fd, ကိန်းပြည့် user_flags)
{
    linux_coop_track_fd(fd, user_flags);
}

bool linux_coop_get_user_flags(ကိန်းပြည့် fd, ကိန်းပြည့် *flags)
{
    အကယ်၍ (fd < 0 || flags == NULL) {
        ပြန်ပေး false;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *node = linux_coop_find_fd_locked(fd);
    အကယ်၍ (node == NULL) {
        pthread_mutex_unlock(&s_fd_state_lock);
        ပြန်ပေး false;
    }
    *flags = node->user_flags;
    pthread_mutex_unlock(&s_fd_state_lock);
    ပြန်ပေး true;
}

bool linux_coop_fd_user_nonblocking(ကိန်းပြည့် fd)
{
    ကိန်းပြည့် user_flags = 0;
    အကယ်၍ (linux_coop_get_user_flags(fd, &user_flags)) {
        ပြန်ပေး (user_flags & O_NONBLOCK) != 0;
    }
    ပြန်ပေး false;
}


ssize_t freertos_linux_coop_read(ကိန်းပြည့် fd, void *buf, size_t count)
{
    LINUX_COOP_IO_LOOP(fd, real_read(fd, buf, count))
}

ssize_t freertos_linux_coop_write(ကိန်းပြည့် fd, const void *buf, size_t count)
{
    LINUX_COOP_IO_LOOP(fd, real_write(fd, buf, count))
}

ssize_t freertos_linux_coop_pread(ကိန်းပြည့် fd, void *buf, size_t count, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pread(fd, buf, count, offset))
}

ssize_t freertos_linux_coop_pwrite(ကိန်းပြည့် fd, const void *buf, size_t count, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pwrite(fd, buf, count, offset))
}

ကိန်းပြည့် freertos_linux_coop_open(const var_na *path, ကိန်းပြည့် flags, ကိန်းပြည့် mode)
{
    ကိန်းပြည့် fd = real_open(path, flags, (mode_t)mode);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, flags & O_NONBLOCK);
    ပြန်ပေး fd;
}

ကိန်းပြည့် freertos_linux_coop_close(ကိန်းပြည့် fd)
{
    ကိန်းပြည့် ret = real_close(fd);
    အကယ်၍ (ret == 0 || errno == EINTR) {
        /* On Linux, close() always releases the fd even when returning
         * EINTR.  Retrying would risk closing a fd reused by another
         * thread.  Treat EINTR as success. */
        linux_coop_untrack_fd(fd);
        ပြန်ပေး 0;
    }
    ပြန်ပေး -1;
}

ကိန်းပြည့် freertos_linux_coop_fcntl(ကိန်းပြည့် fd, ကိန်းပြည့် cmd, ကိန်းပြည့် arg)
{
    အကယ်၍ (cmd == F_GETFL) {
        ကိန်းပြည့် real_flags = real_fcntl(fd, F_GETFL, 0);
        အကယ်၍ (real_flags < 0) {
            ပြန်ပေး -1;
        }
        ကိန်းပြည့် user_flags = 0;
        အကယ်၍ (linux_coop_get_user_flags(fd, &user_flags)) {
            real_flags &= ~O_NONBLOCK;
            real_flags |= (user_flags & O_NONBLOCK);
        }
        ပြန်ပေး real_flags;
    }
    အကယ်၍ (cmd == F_SETFL) {
        linux_coop_set_user_flags(fd, arg);
        ကိန်းပြည့် internal_flags = arg | O_NONBLOCK;
        ပြန်ပေး real_fcntl(fd, F_SETFL, internal_flags);
    }
    ပြန်ပေး real_fcntl(fd, cmd, arg);
}

ကိန်းပြည့် freertos_linux_coop_select(ကိန်းပြည့် nfds, fd_set *readfds, fd_set *writefds,
                      fd_set *exceptfds, sa_nracanaa timeval *timeout)
{
    long timeout_ms = -1;
    အကယ်၍ (timeout != NULL) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_usec + 999) / 1000;
        အကယ်၍ (timeout_ms == 0) {
            ပြန်ပေး real_select(nfds, readfds, writefds, exceptfds, timeout);
        }
    }

    /* real_select() mutates the fd_sets in place (clears them, then sets bits
     * for ready descriptors). To keep polling the same descriptors across
     * iterations we must snapshot the caller's sets and restore them on every
     * loop. */
    fd_set rfds_in, wfds_in, efds_in;
    အကယ်၍ (readfds != NULL) {
        rfds_in = *readfds;
    }
    အကယ်၍ (writefds != NULL) {
        wfds_in = *writefds;
    }
    အကယ်၍ (exceptfds != NULL) {
        efds_in = *exceptfds;
    }

    long waited_ms = 0;
    နေစဉ် (1) {
        အကယ်၍ (readfds != NULL) {
            *readfds = rfds_in;
        }
        အကယ်၍ (writefds != NULL) {
            *writefds = wfds_in;
        }
        အကယ်၍ (exceptfds != NULL) {
            *exceptfds = efds_in;
        }

        sa_nracanaa timeval zero_tv = {0, 0};
        ကိန်းပြည့် ret = real_select(nfds, readfds, writefds, exceptfds, &zero_tv);
        အကယ်၍ (ret != 0) {
            ပြန်ပေး ret;
        }
        အကယ်၍ (timeout_ms == 0) {
            ပြန်ပေး 0;
        }
        အကယ်၍ (timeout_ms > 0 && waited_ms >= timeout_ms) {
            ပြန်ပေး 0;
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

__attribute__((weak)) ssize_t read(ကိန်းပြည့် fd, void *buf, size_t count)
{
    ပြန်ပေး freertos_linux_coop_read(fd, buf, count);
}

__attribute__((weak)) ssize_t write(ကိန်းပြည့် fd, const void *buf, size_t count)
{
    ပြန်ပေး freertos_linux_coop_write(fd, buf, count);
}

__attribute__((weak)) ssize_t pread(ကိန်းပြည့် fd, void *buf, size_t count, off_t offset)
{
    ပြန်ပေး freertos_linux_coop_pread(fd, buf, count, offset);
}

__attribute__((weak)) ssize_t pwrite(ကိန်းပြည့် fd, const void *buf, size_t count, off_t offset)
{
    ပြန်ပေး freertos_linux_coop_pwrite(fd, buf, count, offset);
}

__attribute__((weak)) ကိန်းပြည့် open(const var_na *path, ကိန်းပြည့် flags, ...)
{
    ကိန်းပြည့် mode = 0;
    အကယ်၍ (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        mode = va_arg(ap, ကိန်းပြည့်);
        va_end(ap);
    }
    ပြန်ပေး freertos_linux_coop_open(path, flags, mode);
}

__attribute__((weak)) ကိန်းပြည့် close(ကိန်းပြည့် fd)
{
    ပြန်ပေး freertos_linux_coop_close(fd);
}

__attribute__((weak)) ကိန်းပြည့် fcntl(ကိန်းပြည့် fd, ကိန်းပြည့် cmd, ...)
{
    ကိန်းပြည့် arg = 0;
    အကယ်၍ (cmd != F_GETFL) {
        va_list list;
        va_start(list, cmd);
        arg = va_arg(list, ကိန်းပြည့်);
        va_end(list);
    }
    ပြန်ပေး freertos_linux_coop_fcntl(fd, cmd, arg);
}

__attribute__((weak)) ကိန်းပြည့် select(ကိန်းပြည့် nfds, fd_set *readfds, fd_set *writefds,
                                 fd_set *exceptfds, sa_nracanaa timeval *timeout)
{
    ပြန်ပေး freertos_linux_coop_select(nfds, readfds, writefds, exceptfds, timeout);
}

ssize_t freertos_linux_coop_readv(ကိန်းပြည့် fd, const sa_nracanaa iovec *iov, ကိန်းပြည့် iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_readv(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_writev(ကိန်းပြည့် fd, const sa_nracanaa iovec *iov, ကိန်းပြည့် iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_writev(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_recv(ကိန်းပြည့် sockfd, void *buf, size_t len, ကိန်းပြည့် flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recv(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_send(ကိန်းပြည့် sockfd, const void *buf, size_t len, ကိန်းပြည့် flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_send(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_recvfrom(ကိန်းပြည့် sockfd, void *buf, size_t len, ကိန်းပြည့် flags,
                 sa_nracanaa sockaddr *src_addr, socklen_t *addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvfrom(sockfd, buf, len, flags | MSG_DONTWAIT,
                            src_addr, addrlen))
}

ssize_t freertos_linux_coop_sendto(ကိန်းပြည့် sockfd, const void *buf, size_t len, ကိန်းပြည့် flags,
               const sa_nracanaa sockaddr *dest_addr, socklen_t addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendto(sockfd, buf, len, flags | MSG_DONTWAIT,
                          dest_addr, addrlen))
}

ssize_t freertos_linux_coop_recvmsg(ကိန်းပြည့် sockfd, sa_nracanaa msghdr *msg, ကိန်းပြည့် flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvmsg(sockfd, msg, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_sendmsg(ကိန်းပြည့် sockfd, const sa_nracanaa msghdr *msg, ကိန်းပြည့် flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendmsg(sockfd, msg, flags | MSG_DONTWAIT))
}

__attribute__((weak)) ssize_t readv(ကိန်းပြည့် fd, const sa_nracanaa iovec *iov, ကိန်းပြည့် iovcnt)
{
    ပြန်ပေး freertos_linux_coop_readv(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t writev(ကိန်းပြည့် fd, const sa_nracanaa iovec *iov, ကိန်းပြည့် iovcnt)
{
    ပြန်ပေး freertos_linux_coop_writev(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t recv(ကိန်းပြည့် sockfd, void *buf, size_t len, ကိန်းပြည့် flags)
{
    ပြန်ပေး freertos_linux_coop_recv(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t send(ကိန်းပြည့် sockfd, const void *buf, size_t len, ကိန်းပြည့် flags)
{
    ပြန်ပေး freertos_linux_coop_send(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t recvfrom(ကိန်းပြည့် sockfd, void *buf, size_t len, ကိန်းပြည့် flags,
                 sa_nracanaa sockaddr *src_addr, socklen_t *addrlen)
{
    ပြန်ပေး freertos_linux_coop_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

__attribute__((weak)) ssize_t sendto(ကိန်းပြည့် sockfd, const void *buf, size_t len, ကိန်းပြည့် flags,
               const sa_nracanaa sockaddr *dest_addr, socklen_t addrlen)
{
    ပြန်ပေး freertos_linux_coop_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

__attribute__((weak)) ssize_t recvmsg(ကိန်းပြည့် sockfd, sa_nracanaa msghdr *msg, ကိန်းပြည့် flags)
{
    ပြန်ပေး freertos_linux_coop_recvmsg(sockfd, msg, flags);
}

__attribute__((weak)) ssize_t sendmsg(ကိန်းပြည့် sockfd, const sa_nracanaa msghdr *msg, ကိန်းပြည့် flags)
{
    ပြန်ပေး freertos_linux_coop_sendmsg(sockfd, msg, flags);
}

ကိန်းပြည့် freertos_linux_coop_connect(ကိန်းပြည့် sockfd, const sa_nracanaa sockaddr *addr, socklen_t addrlen)
{
    နေစဉ် (1)
    {
        ကိန်းပြည့် ret = real_connect(sockfd, addr, addrlen);
        အကယ်၍ (ret == 0) {
            ပြန်ပေး ret;
        }

        အကယ်၍ (errno == EINPROGRESS || errno == EALREADY) {
            sa_nracanaa pollfd pfd;
            pfd.fd = sockfd;
            pfd.events = POLLOUT;
            pfd.revents = 0;

            ကိန်းပြည့် poll_retries = 0;
            နေစဉ် (1)
            {
                ကိန်းပြည့် press = real_poll(&pfd, 1, 0);
                အကယ်၍ (press > 0) {
                    ကိန်းပြည့် so_err = 0;
                    socklen_t len = parimaa_na(so_err);
                    အကယ်၍ (getsockopt(sockfd, SOL_SOCKET, SO_ERROR,
                                   &so_err, &len) < 0) {
                        ပြန်ပေး -1;
                    }
                    အကယ်၍ (so_err == 0) {
                        ပြန်ပေး 0;
                    } မဟုတ်ပါက {
                        errno = so_err;
                        ပြန်ပေး -1;
                    }
                } မဟုတ်ပါက အကယ်၍ (press == 0) {
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    continue;
                } မဟုတ်ပါက {
                    အကယ်၍ (errno == EINTR) {
                        continue;
                    }
                    /* Non-EINTR poll error; bail out after a few retries */
                    အကယ်၍ (++poll_retries > 3) {
                        ပြန်ပေး -1;
                    }
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    continue;
                }
            }
        }

        အကယ်၍ (errno == EINTR) {
            ပြန်ပေး -1;
        }

        ပြန်ပေး -1;
    }
}

ကိန်းပြည့် freertos_linux_coop_accept(ကိန်းပြည့် sockfd, sa_nracanaa sockaddr *addr, socklen_t *addrlen)
{
    နေစဉ် (1)
    {
        ကိန်းပြည့် fd = real_accept(sockfd, addr, addrlen);
        အကယ်၍ (fd >= 0) {
            linux_coop_set_nonblocking(fd);
            linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(sockfd) ? O_NONBLOCK : 0);
            ပြန်ပေး fd;
        }
        အကယ်၍ (errno == EAGAIN || errno == EWOULDBLOCK) {
            အကယ်၍ (linux_coop_fd_user_nonblocking(sockfd)) {
                ပြန်ပေး -1;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            continue;
        }
        ပြန်ပေး -1;
    }
}

ကိန်းပြည့် freertos_linux_coop_pselect(ကိန်းပြည့် nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, const sa_nracanaa timespec *timeout,
            const sigset_t *sigmask)
{
    long timeout_ms = -1;
    အကယ်၍ (timeout != NULL) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_nsec + 999999) / 1000000;
        အကယ်၍ (timeout_ms == 0) {
            ပြန်ပေး real_pselect(nfds, readfds, writefds, exceptfds,
                                timeout, sigmask);
        }
    }

    long waited_ms = 0;
    နေစဉ် (1)
    {
        sa_nracanaa timespec zero_ts = {0, 0};
        ကိန်းပြည့် ret = real_pselect(nfds, readfds, writefds, exceptfds,
                               &zero_ts, sigmask);
        အကယ်၍ (ret != 0) {
            ပြန်ပေး ret;
        }

        အကယ်၍ (timeout_ms == 0 ||
            (timeout_ms > 0 && waited_ms >= timeout_ms)) {
            ပြန်ပေး 0;
        }

        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

ကိန်းပြည့် freertos_linux_coop_poll(sa_nracanaa pollfd *fds, nfds_t nfds, ကိန်းပြည့် timeout)
{
    အကယ်၍ (timeout == 0) {
        ပြန်ပေး real_poll(fds, nfds, 0);
    }

    long timeout_ms = -1;
    အကယ်၍ (timeout > 0) {
        timeout_ms = timeout;
    }

    long waited_ms = 0;

    နေစဉ် (1)
    {
        ကိန်းပြည့် ret = real_poll(fds, nfds, 0);
        အကယ်၍ (ret > 0) {
            ပြန်ပေး ret;
        } မဟုတ်ပါက အကယ်၍ (ret == 0) {
            အကယ်၍ (timeout_ms == 0 ||
                (timeout_ms > 0 && waited_ms >= timeout_ms)) {
                ပြန်ပေး 0;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            waited_ms += LINUX_COOP_TICK_MS;
            continue;
        } မဟုတ်ပါက {
            အကယ်၍ (errno == EINTR) {
                continue;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            အကယ်၍ (timeout_ms > 0 && waited_ms >= timeout_ms) {
                ပြန်ပေး -1;
            }
            waited_ms += LINUX_COOP_TICK_MS;
        }
    }
}

ကိန်းပြည့် nanosleep(const sa_nracanaa timespec *req, sa_nracanaa timespec *rem)
{
    အကယ်၍ (req == NULL) {
        errno = EFAULT;
        ပြန်ပေး -1;
    }
    အကယ်၍ (!linux_port_in_freertos_task()) {
        ပြန်ပေး real_nanosleep(req, rem);
    }
    long ms = req->tv_sec * 1000 + (req->tv_nsec + 999999) / 1000000;
    အကယ်၍ (ms <= 0) {
        ms = 1;
    }
    vTaskDelay(ms);
    အကယ်၍ (rem) {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }
    ပြန်ပေး 0;
}

unsigned ကိန်းပြည့် sleep(unsigned ကိန်းပြည့် seconds)
{
    linux_coop_yield(seconds * 1000);
    ပြန်ပေး 0;
}

ကိန်းပြည့် usleep(useconds_t usec)
{
    linux_coop_yield(usec / 1000);
    ပြန်ပေး 0;
}

ကိန်းပြည့် freertos_linux_coop_socket(ကိန်းပြည့် domain, ကိန်းပြည့် type, ကိန်းပြည့် protocol)
{
    ကိန်းပြည့် fd = real_socket(domain, type, protocol);
    linux_coop_set_nonblocking(fd);
#ifdef SOCK_NONBLOCK
    const ကိန်းပြည့် user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#မဟုတ်ပါက
    const ကိန်းပြည့် user_flags = 0;
#endif
    linux_coop_track_fd(fd, user_flags);
    ပြန်ပေး fd;
}

ကိန်းပြည့် freertos_linux_coop_socketpair(ကိန်းပြည့် domain, ကိန်းပြည့် type, ကိန်းပြည့် protocol, ကိန်းပြည့် *sv)
{
    ကိန်းပြည့် ret = real_socketpair(domain, type, protocol, sv);
    အကယ်၍ (ret == 0) {
        linux_coop_set_nonblocking(sv[0]);
        linux_coop_set_nonblocking(sv[1]);
#ifdef SOCK_NONBLOCK
        const ကိန်းပြည့် user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#မဟုတ်ပါက
        const ကိန်းပြည့် user_flags = 0;
#endif
        linux_coop_track_fd(sv[0], user_flags);
        linux_coop_track_fd(sv[1], user_flags);
    }
    ပြန်ပေး ret;
}

ကိန်းပြည့် freertos_linux_coop_pipe(ကိန်းပြည့် *fds)
{
    ကိန်းပြည့် ret = real_pipe(fds);
    အကယ်၍ (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        linux_coop_track_fd(fds[0], 0);
        linux_coop_track_fd(fds[1], 0);
    }
    ပြန်ပေး ret;
}

ကိန်းပြည့် freertos_linux_coop_pipe2(ကိန်းပြည့် *fds, ကိန်းပြည့် flags)
{
    ကိန်းပြည့် ret = real_pipe2(fds, flags);
    အကယ်၍ (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        const ကိန်းပြည့် user_flags = (flags & O_NONBLOCK) ? O_NONBLOCK : 0;
        linux_coop_track_fd(fds[0], user_flags);
        linux_coop_track_fd(fds[1], user_flags);
    }
    ပြန်ပေး ret;
}

ကိန်းပြည့် freertos_linux_coop_dup(ကိန်းပြည့် oldfd)
{
    ကိန်းပြည့် fd = real_dup(oldfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    ပြန်ပေး fd;
}

ကိန်းပြည့် freertos_linux_coop_dup2(ကိန်းပြည့် oldfd, ကိန်းပြည့် newfd)
{
    ကိန်းပြည့် fd = real_dup2(oldfd, newfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    ပြန်ပေး fd;
}

__attribute__((weak)) ကိန်းပြည့် connect(ကိန်းပြည့် sockfd, const sa_nracanaa sockaddr *addr, socklen_t addrlen)
{
    ပြန်ပေး freertos_linux_coop_connect(sockfd, addr, addrlen);
}

__attribute__((weak)) ကိန်းပြည့် accept(ကိန်းပြည့် sockfd, sa_nracanaa sockaddr *addr, socklen_t *addrlen)
{
    ပြန်ပေး freertos_linux_coop_accept(sockfd, addr, addrlen);
}

__attribute__((weak)) ကိန်းပြည့် pselect(ကိန်းပြည့် nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, const sa_nracanaa timespec *timeout,
            const sigset_t *sigmask)
{
    ပြန်ပေး freertos_linux_coop_pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}

__attribute__((weak)) ကိန်းပြည့် poll(sa_nracanaa pollfd *fds, nfds_t nfds, ကိန်းပြည့် timeout)
{
    ပြန်ပေး freertos_linux_coop_poll(fds, nfds, timeout);
}

__attribute__((weak)) ကိန်းပြည့် socket(ကိန်းပြည့် domain, ကိန်းပြည့် type, ကိန်းပြည့် protocol)
{
    ပြန်ပေး freertos_linux_coop_socket(domain, type, protocol);
}

__attribute__((weak)) ကိန်းပြည့် socketpair(ကိန်းပြည့် domain, ကိန်းပြည့် type, ကိန်းပြည့် protocol, ကိန်းပြည့် sv[2])
{
    ပြန်ပေး freertos_linux_coop_socketpair(domain, type, protocol, sv);
}

__attribute__((weak)) ကိန်းပြည့် pipe(ကိန်းပြည့် fds[2])
{
    ပြန်ပေး freertos_linux_coop_pipe(fds);
}

__attribute__((weak)) ကိန်းပြည့် pipe2(ကိန်းပြည့် fds[2], ကိန်းပြည့် flags)
{
    ပြန်ပေး freertos_linux_coop_pipe2(fds, flags);
}

__attribute__((weak)) ကိန်းပြည့် dup(ကိန်းပြည့် oldfd)
{
    ပြန်ပေး freertos_linux_coop_dup(oldfd);
}

__attribute__((weak)) ကိန်းပြည့် dup2(ကိန်းပြည့် oldfd, ကိန်းပြည့် newfd)
{
    ပြန်ပေး freertos_linux_coop_dup2(oldfd, newfd);
}
