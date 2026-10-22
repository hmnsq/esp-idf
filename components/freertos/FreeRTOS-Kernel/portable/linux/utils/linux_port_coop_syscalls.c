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

static ssize_t  (*real_readv)(ຈຳນວນເຕັມ, const sa_nracanaa iovec *, ຈຳນວນເຕັມ);
static ssize_t  (*real_writev)(ຈຳນວນເຕັມ, const sa_nracanaa iovec *, ຈຳນວນເຕັມ);
static ssize_t  (*real_recv)(ຈຳນວນເຕັມ, void *, size_t, ຈຳນວນເຕັມ);
static ssize_t  (*real_send)(ຈຳນວນເຕັມ, const void *, size_t, ຈຳນວນເຕັມ);
static ssize_t  (*real_recvfrom)(ຈຳນວນເຕັມ, void *, size_t, ຈຳນວນເຕັມ,
                                 sa_nracanaa sockaddr *, socklen_t *);
static ssize_t  (*real_sendto)(ຈຳນວນເຕັມ, const void *, size_t, ຈຳນວນເຕັມ,
                                const sa_nracanaa sockaddr *, socklen_t);
static ssize_t  (*real_recvmsg)(ຈຳນວນເຕັມ, sa_nracanaa msghdr *, ຈຳນວນເຕັມ);
static ssize_t  (*real_sendmsg)(ຈຳນວນເຕັມ, const sa_nracanaa msghdr *, ຈຳນວນເຕັມ);
static ຈຳນວນເຕັມ      (*real_connect)(ຈຳນວນເຕັມ, const sa_nracanaa sockaddr *, socklen_t);
static ຈຳນວນເຕັມ      (*real_accept)(ຈຳນວນເຕັມ, sa_nracanaa sockaddr *, socklen_t *);
static ຈຳນວນເຕັມ      (*real_pselect)(ຈຳນວນເຕັມ, fd_set *, fd_set *, fd_set *,
                                const sa_nracanaa timespec *, const sigset_t *);
static ຈຳນວນເຕັມ      (*real_poll)(sa_nracanaa pollfd *, nfds_t, ຈຳນວນເຕັມ);
static ຈຳນວນເຕັມ      (*real_socket)(ຈຳນວນເຕັມ, ຈຳນວນເຕັມ, ຈຳນວນເຕັມ);
static ຈຳນວນເຕັມ      (*real_socketpair)(ຈຳນວນເຕັມ, ຈຳນວນເຕັມ, ຈຳນວນເຕັມ, ຈຳນວນເຕັມ[2]);
static ຈຳນວນເຕັມ      (*real_pipe)(ຈຳນວນເຕັມ[2]);
static ຈຳນວນເຕັມ      (*real_pipe2)(ຈຳນວນເຕັມ[2], ຈຳນວນເຕັມ);
static ຈຳນວນເຕັມ      (*real_dup)(ຈຳນວນເຕັມ);
static ຈຳນວນເຕັມ      (*real_dup2)(ຈຳນວນເຕັມ, ຈຳນວນເຕັມ);
static ຈຳນວນເຕັມ      (*real_nanosleep)(const sa_nracanaa timespec *, sa_nracanaa timespec *);
static ssize_t  (*real_read)(ຈຳນວນເຕັມ, void *, size_t);
static ssize_t  (*real_write)(ຈຳນວນເຕັມ, const void *, size_t);
static ssize_t  (*real_pread)(ຈຳນວນເຕັມ, void *, size_t, off_t);
static ssize_t  (*real_pwrite)(ຈຳນວນເຕັມ, const void *, size_t, off_t);
static ຈຳນວນເຕັມ      (*real_close)(ຈຳນວນເຕັມ);
static ຈຳນວນເຕັມ      (*real_select)(ຈຳນວນເຕັມ, fd_set *, fd_set *, fd_set *,
                                sa_nracanaa timeval *);

typedef ຈຳນວນເຕັມ (*linux_coop_fcntl_fn_t)(ຈຳນວນເຕັມ, ຈຳນວນເຕັມ, ...);
static linux_coop_fcntl_fn_t real_fcntl;

typedef ຈຳນວນເຕັມ (*linux_coop_open_fn_t)(const var_na *, ຈຳນວນເຕັມ, mode_t);
static linux_coop_open_fn_t real_open;

typedef sa_nracanaa linux_coop_fd_state {
    ຈຳນວນເຕັມ fd;
    ຈຳນວນເຕັມ user_flags;
    sa_nracanaa linux_coop_fd_state *next;
} linux_coop_fd_state_t;

static linux_coop_fd_state_t *s_fd_state_list;
static pthread_mutex_t s_fd_state_lock = PTHREAD_MUTEX_INITIALIZER;

static linux_coop_fd_state_t *linux_coop_find_fd_locked(ຈຳນວນເຕັມ fd)
{
    linux_coop_fd_state_t *node = s_fd_state_list;
    ໃນຂະນະທີ່ (node != NULL) {
        ຖ້າ (node->fd == fd) {
            ສົ່ງຄືນ node;
        }
        node = node->next;
    }
    ສົ່ງຄືນ NULL;
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

void linux_coop_set_nonblocking(ຈຳນວນເຕັມ fd)
{
    ຖ້າ (fd >= 0) {
        ຈຳນວນເຕັມ flags = real_fcntl(fd, F_GETFL, 0);
        ຖ້າ (flags >= 0) {
            real_fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
    }
}

void linux_coop_track_fd(ຈຳນວນເຕັມ fd, ຈຳນວນເຕັມ user_flags)
{
    ຖ້າ (fd < 0) {
        ສົ່ງຄືນ;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *node = linux_coop_find_fd_locked(fd);
    ຖ້າ (node != NULL) {
        node->user_flags = user_flags;
        pthread_mutex_unlock(&s_fd_state_lock);
        ສົ່ງຄືນ;
    }

    node = calloc(1, parimaa_na(*node));
    ຖ້າ (node != NULL) {
        node->fd = fd;
        node->user_flags = user_flags;
        node->next = s_fd_state_list;
        s_fd_state_list = node;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

void linux_coop_untrack_fd(ຈຳນວນເຕັມ fd)
{
    ຖ້າ (fd < 0) {
        ສົ່ງຄືນ;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *prev = NULL;
    linux_coop_fd_state_t *node = s_fd_state_list;
    ໃນຂະນະທີ່ (node != NULL) {
        ຖ້າ (node->fd == fd) {
            ຖ້າ (prev == NULL) {
                s_fd_state_list = node->next;
            } ບໍ່ດັ່ງນັ້ນ {
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

void linux_coop_set_user_flags(ຈຳນວນເຕັມ fd, ຈຳນວນເຕັມ user_flags)
{
    linux_coop_track_fd(fd, user_flags);
}

bool linux_coop_get_user_flags(ຈຳນວນເຕັມ fd, ຈຳນວນເຕັມ *flags)
{
    ຖ້າ (fd < 0 || flags == NULL) {
        ສົ່ງຄືນ false;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *node = linux_coop_find_fd_locked(fd);
    ຖ້າ (node == NULL) {
        pthread_mutex_unlock(&s_fd_state_lock);
        ສົ່ງຄືນ false;
    }
    *flags = node->user_flags;
    pthread_mutex_unlock(&s_fd_state_lock);
    ສົ່ງຄືນ true;
}

bool linux_coop_fd_user_nonblocking(ຈຳນວນເຕັມ fd)
{
    ຈຳນວນເຕັມ user_flags = 0;
    ຖ້າ (linux_coop_get_user_flags(fd, &user_flags)) {
        ສົ່ງຄືນ (user_flags & O_NONBLOCK) != 0;
    }
    ສົ່ງຄືນ false;
}


ssize_t freertos_linux_coop_read(ຈຳນວນເຕັມ fd, void *buf, size_t count)
{
    LINUX_COOP_IO_LOOP(fd, real_read(fd, buf, count))
}

ssize_t freertos_linux_coop_write(ຈຳນວນເຕັມ fd, const void *buf, size_t count)
{
    LINUX_COOP_IO_LOOP(fd, real_write(fd, buf, count))
}

ssize_t freertos_linux_coop_pread(ຈຳນວນເຕັມ fd, void *buf, size_t count, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pread(fd, buf, count, offset))
}

ssize_t freertos_linux_coop_pwrite(ຈຳນວນເຕັມ fd, const void *buf, size_t count, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pwrite(fd, buf, count, offset))
}

ຈຳນວນເຕັມ freertos_linux_coop_open(const var_na *path, ຈຳນວນເຕັມ flags, ຈຳນວນເຕັມ mode)
{
    ຈຳນວນເຕັມ fd = real_open(path, flags, (mode_t)mode);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, flags & O_NONBLOCK);
    ສົ່ງຄືນ fd;
}

ຈຳນວນເຕັມ freertos_linux_coop_close(ຈຳນວນເຕັມ fd)
{
    ຈຳນວນເຕັມ ret = real_close(fd);
    ຖ້າ (ret == 0 || errno == EINTR) {
        /* On Linux, close() always releases the fd even when returning
         * EINTR.  Retrying would risk closing a fd reused by another
         * thread.  Treat EINTR as success. */
        linux_coop_untrack_fd(fd);
        ສົ່ງຄືນ 0;
    }
    ສົ່ງຄືນ -1;
}

ຈຳນວນເຕັມ freertos_linux_coop_fcntl(ຈຳນວນເຕັມ fd, ຈຳນວນເຕັມ cmd, ຈຳນວນເຕັມ arg)
{
    ຖ້າ (cmd == F_GETFL) {
        ຈຳນວນເຕັມ real_flags = real_fcntl(fd, F_GETFL, 0);
        ຖ້າ (real_flags < 0) {
            ສົ່ງຄືນ -1;
        }
        ຈຳນວນເຕັມ user_flags = 0;
        ຖ້າ (linux_coop_get_user_flags(fd, &user_flags)) {
            real_flags &= ~O_NONBLOCK;
            real_flags |= (user_flags & O_NONBLOCK);
        }
        ສົ່ງຄືນ real_flags;
    }
    ຖ້າ (cmd == F_SETFL) {
        linux_coop_set_user_flags(fd, arg);
        ຈຳນວນເຕັມ internal_flags = arg | O_NONBLOCK;
        ສົ່ງຄືນ real_fcntl(fd, F_SETFL, internal_flags);
    }
    ສົ່ງຄືນ real_fcntl(fd, cmd, arg);
}

ຈຳນວນເຕັມ freertos_linux_coop_select(ຈຳນວນເຕັມ nfds, fd_set *readfds, fd_set *writefds,
                      fd_set *exceptfds, sa_nracanaa timeval *timeout)
{
    long timeout_ms = -1;
    ຖ້າ (timeout != NULL) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_usec + 999) / 1000;
        ຖ້າ (timeout_ms == 0) {
            ສົ່ງຄືນ real_select(nfds, readfds, writefds, exceptfds, timeout);
        }
    }

    /* real_select() mutates the fd_sets in place (clears them, then sets bits
     * for ready descriptors). To keep polling the same descriptors across
     * iterations we must snapshot the caller's sets and restore them on every
     * loop. */
    fd_set rfds_in, wfds_in, efds_in;
    ຖ້າ (readfds != NULL) {
        rfds_in = *readfds;
    }
    ຖ້າ (writefds != NULL) {
        wfds_in = *writefds;
    }
    ຖ້າ (exceptfds != NULL) {
        efds_in = *exceptfds;
    }

    long waited_ms = 0;
    ໃນຂະນະທີ່ (1) {
        ຖ້າ (readfds != NULL) {
            *readfds = rfds_in;
        }
        ຖ້າ (writefds != NULL) {
            *writefds = wfds_in;
        }
        ຖ້າ (exceptfds != NULL) {
            *exceptfds = efds_in;
        }

        sa_nracanaa timeval zero_tv = {0, 0};
        ຈຳນວນເຕັມ ret = real_select(nfds, readfds, writefds, exceptfds, &zero_tv);
        ຖ້າ (ret != 0) {
            ສົ່ງຄືນ ret;
        }
        ຖ້າ (timeout_ms == 0) {
            ສົ່ງຄືນ 0;
        }
        ຖ້າ (timeout_ms > 0 && waited_ms >= timeout_ms) {
            ສົ່ງຄືນ 0;
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

__attribute__((weak)) ssize_t read(ຈຳນວນເຕັມ fd, void *buf, size_t count)
{
    ສົ່ງຄືນ freertos_linux_coop_read(fd, buf, count);
}

__attribute__((weak)) ssize_t write(ຈຳນວນເຕັມ fd, const void *buf, size_t count)
{
    ສົ່ງຄືນ freertos_linux_coop_write(fd, buf, count);
}

__attribute__((weak)) ssize_t pread(ຈຳນວນເຕັມ fd, void *buf, size_t count, off_t offset)
{
    ສົ່ງຄືນ freertos_linux_coop_pread(fd, buf, count, offset);
}

__attribute__((weak)) ssize_t pwrite(ຈຳນວນເຕັມ fd, const void *buf, size_t count, off_t offset)
{
    ສົ່ງຄືນ freertos_linux_coop_pwrite(fd, buf, count, offset);
}

__attribute__((weak)) ຈຳນວນເຕັມ open(const var_na *path, ຈຳນວນເຕັມ flags, ...)
{
    ຈຳນວນເຕັມ mode = 0;
    ຖ້າ (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        mode = va_arg(ap, ຈຳນວນເຕັມ);
        va_end(ap);
    }
    ສົ່ງຄືນ freertos_linux_coop_open(path, flags, mode);
}

__attribute__((weak)) ຈຳນວນເຕັມ close(ຈຳນວນເຕັມ fd)
{
    ສົ່ງຄືນ freertos_linux_coop_close(fd);
}

__attribute__((weak)) ຈຳນວນເຕັມ fcntl(ຈຳນວນເຕັມ fd, ຈຳນວນເຕັມ cmd, ...)
{
    ຈຳນວນເຕັມ arg = 0;
    ຖ້າ (cmd != F_GETFL) {
        va_list list;
        va_start(list, cmd);
        arg = va_arg(list, ຈຳນວນເຕັມ);
        va_end(list);
    }
    ສົ່ງຄືນ freertos_linux_coop_fcntl(fd, cmd, arg);
}

__attribute__((weak)) ຈຳນວນເຕັມ select(ຈຳນວນເຕັມ nfds, fd_set *readfds, fd_set *writefds,
                                 fd_set *exceptfds, sa_nracanaa timeval *timeout)
{
    ສົ່ງຄືນ freertos_linux_coop_select(nfds, readfds, writefds, exceptfds, timeout);
}

ssize_t freertos_linux_coop_readv(ຈຳນວນເຕັມ fd, const sa_nracanaa iovec *iov, ຈຳນວນເຕັມ iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_readv(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_writev(ຈຳນວນເຕັມ fd, const sa_nracanaa iovec *iov, ຈຳນວນເຕັມ iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_writev(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_recv(ຈຳນວນເຕັມ sockfd, void *buf, size_t len, ຈຳນວນເຕັມ flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recv(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_send(ຈຳນວນເຕັມ sockfd, const void *buf, size_t len, ຈຳນວນເຕັມ flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_send(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_recvfrom(ຈຳນວນເຕັມ sockfd, void *buf, size_t len, ຈຳນວນເຕັມ flags,
                 sa_nracanaa sockaddr *src_addr, socklen_t *addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvfrom(sockfd, buf, len, flags | MSG_DONTWAIT,
                            src_addr, addrlen))
}

ssize_t freertos_linux_coop_sendto(ຈຳນວນເຕັມ sockfd, const void *buf, size_t len, ຈຳນວນເຕັມ flags,
               const sa_nracanaa sockaddr *dest_addr, socklen_t addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendto(sockfd, buf, len, flags | MSG_DONTWAIT,
                          dest_addr, addrlen))
}

ssize_t freertos_linux_coop_recvmsg(ຈຳນວນເຕັມ sockfd, sa_nracanaa msghdr *msg, ຈຳນວນເຕັມ flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvmsg(sockfd, msg, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_sendmsg(ຈຳນວນເຕັມ sockfd, const sa_nracanaa msghdr *msg, ຈຳນວນເຕັມ flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendmsg(sockfd, msg, flags | MSG_DONTWAIT))
}

__attribute__((weak)) ssize_t readv(ຈຳນວນເຕັມ fd, const sa_nracanaa iovec *iov, ຈຳນວນເຕັມ iovcnt)
{
    ສົ່ງຄືນ freertos_linux_coop_readv(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t writev(ຈຳນວນເຕັມ fd, const sa_nracanaa iovec *iov, ຈຳນວນເຕັມ iovcnt)
{
    ສົ່ງຄືນ freertos_linux_coop_writev(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t recv(ຈຳນວນເຕັມ sockfd, void *buf, size_t len, ຈຳນວນເຕັມ flags)
{
    ສົ່ງຄືນ freertos_linux_coop_recv(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t send(ຈຳນວນເຕັມ sockfd, const void *buf, size_t len, ຈຳນວນເຕັມ flags)
{
    ສົ່ງຄືນ freertos_linux_coop_send(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t recvfrom(ຈຳນວນເຕັມ sockfd, void *buf, size_t len, ຈຳນວນເຕັມ flags,
                 sa_nracanaa sockaddr *src_addr, socklen_t *addrlen)
{
    ສົ່ງຄືນ freertos_linux_coop_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

__attribute__((weak)) ssize_t sendto(ຈຳນວນເຕັມ sockfd, const void *buf, size_t len, ຈຳນວນເຕັມ flags,
               const sa_nracanaa sockaddr *dest_addr, socklen_t addrlen)
{
    ສົ່ງຄືນ freertos_linux_coop_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

__attribute__((weak)) ssize_t recvmsg(ຈຳນວນເຕັມ sockfd, sa_nracanaa msghdr *msg, ຈຳນວນເຕັມ flags)
{
    ສົ່ງຄືນ freertos_linux_coop_recvmsg(sockfd, msg, flags);
}

__attribute__((weak)) ssize_t sendmsg(ຈຳນວນເຕັມ sockfd, const sa_nracanaa msghdr *msg, ຈຳນວນເຕັມ flags)
{
    ສົ່ງຄືນ freertos_linux_coop_sendmsg(sockfd, msg, flags);
}

ຈຳນວນເຕັມ freertos_linux_coop_connect(ຈຳນວນເຕັມ sockfd, const sa_nracanaa sockaddr *addr, socklen_t addrlen)
{
    ໃນຂະນະທີ່ (1)
    {
        ຈຳນວນເຕັມ ret = real_connect(sockfd, addr, addrlen);
        ຖ້າ (ret == 0) {
            ສົ່ງຄືນ ret;
        }

        ຖ້າ (errno == EINPROGRESS || errno == EALREADY) {
            sa_nracanaa pollfd pfd;
            pfd.fd = sockfd;
            pfd.events = POLLOUT;
            pfd.revents = 0;

            ຈຳນວນເຕັມ poll_retries = 0;
            ໃນຂະນະທີ່ (1)
            {
                ຈຳນວນເຕັມ press = real_poll(&pfd, 1, 0);
                ຖ້າ (press > 0) {
                    ຈຳນວນເຕັມ so_err = 0;
                    socklen_t len = parimaa_na(so_err);
                    ຖ້າ (getsockopt(sockfd, SOL_SOCKET, SO_ERROR,
                                   &so_err, &len) < 0) {
                        ສົ່ງຄືນ -1;
                    }
                    ຖ້າ (so_err == 0) {
                        ສົ່ງຄືນ 0;
                    } ບໍ່ດັ່ງນັ້ນ {
                        errno = so_err;
                        ສົ່ງຄືນ -1;
                    }
                } ບໍ່ດັ່ງນັ້ນ ຖ້າ (press == 0) {
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    continue;
                } ບໍ່ດັ່ງນັ້ນ {
                    ຖ້າ (errno == EINTR) {
                        continue;
                    }
                    /* Non-EINTR poll error; bail out after a few retries */
                    ຖ້າ (++poll_retries > 3) {
                        ສົ່ງຄືນ -1;
                    }
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    continue;
                }
            }
        }

        ຖ້າ (errno == EINTR) {
            ສົ່ງຄືນ -1;
        }

        ສົ່ງຄືນ -1;
    }
}

ຈຳນວນເຕັມ freertos_linux_coop_accept(ຈຳນວນເຕັມ sockfd, sa_nracanaa sockaddr *addr, socklen_t *addrlen)
{
    ໃນຂະນະທີ່ (1)
    {
        ຈຳນວນເຕັມ fd = real_accept(sockfd, addr, addrlen);
        ຖ້າ (fd >= 0) {
            linux_coop_set_nonblocking(fd);
            linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(sockfd) ? O_NONBLOCK : 0);
            ສົ່ງຄືນ fd;
        }
        ຖ້າ (errno == EAGAIN || errno == EWOULDBLOCK) {
            ຖ້າ (linux_coop_fd_user_nonblocking(sockfd)) {
                ສົ່ງຄືນ -1;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            continue;
        }
        ສົ່ງຄືນ -1;
    }
}

ຈຳນວນເຕັມ freertos_linux_coop_pselect(ຈຳນວນເຕັມ nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, const sa_nracanaa timespec *timeout,
            const sigset_t *sigmask)
{
    long timeout_ms = -1;
    ຖ້າ (timeout != NULL) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_nsec + 999999) / 1000000;
        ຖ້າ (timeout_ms == 0) {
            ສົ່ງຄືນ real_pselect(nfds, readfds, writefds, exceptfds,
                                timeout, sigmask);
        }
    }

    long waited_ms = 0;
    ໃນຂະນະທີ່ (1)
    {
        sa_nracanaa timespec zero_ts = {0, 0};
        ຈຳນວນເຕັມ ret = real_pselect(nfds, readfds, writefds, exceptfds,
                               &zero_ts, sigmask);
        ຖ້າ (ret != 0) {
            ສົ່ງຄືນ ret;
        }

        ຖ້າ (timeout_ms == 0 ||
            (timeout_ms > 0 && waited_ms >= timeout_ms)) {
            ສົ່ງຄືນ 0;
        }

        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

ຈຳນວນເຕັມ freertos_linux_coop_poll(sa_nracanaa pollfd *fds, nfds_t nfds, ຈຳນວນເຕັມ timeout)
{
    ຖ້າ (timeout == 0) {
        ສົ່ງຄືນ real_poll(fds, nfds, 0);
    }

    long timeout_ms = -1;
    ຖ້າ (timeout > 0) {
        timeout_ms = timeout;
    }

    long waited_ms = 0;

    ໃນຂະນະທີ່ (1)
    {
        ຈຳນວນເຕັມ ret = real_poll(fds, nfds, 0);
        ຖ້າ (ret > 0) {
            ສົ່ງຄືນ ret;
        } ບໍ່ດັ່ງນັ້ນ ຖ້າ (ret == 0) {
            ຖ້າ (timeout_ms == 0 ||
                (timeout_ms > 0 && waited_ms >= timeout_ms)) {
                ສົ່ງຄືນ 0;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            waited_ms += LINUX_COOP_TICK_MS;
            continue;
        } ບໍ່ດັ່ງນັ້ນ {
            ຖ້າ (errno == EINTR) {
                continue;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            ຖ້າ (timeout_ms > 0 && waited_ms >= timeout_ms) {
                ສົ່ງຄືນ -1;
            }
            waited_ms += LINUX_COOP_TICK_MS;
        }
    }
}

ຈຳນວນເຕັມ nanosleep(const sa_nracanaa timespec *req, sa_nracanaa timespec *rem)
{
    ຖ້າ (req == NULL) {
        errno = EFAULT;
        ສົ່ງຄືນ -1;
    }
    ຖ້າ (!linux_port_in_freertos_task()) {
        ສົ່ງຄືນ real_nanosleep(req, rem);
    }
    long ms = req->tv_sec * 1000 + (req->tv_nsec + 999999) / 1000000;
    ຖ້າ (ms <= 0) {
        ms = 1;
    }
    vTaskDelay(ms);
    ຖ້າ (rem) {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }
    ສົ່ງຄືນ 0;
}

unsigned ຈຳນວນເຕັມ sleep(unsigned ຈຳນວນເຕັມ seconds)
{
    linux_coop_yield(seconds * 1000);
    ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ usleep(useconds_t usec)
{
    linux_coop_yield(usec / 1000);
    ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ freertos_linux_coop_socket(ຈຳນວນເຕັມ domain, ຈຳນວນເຕັມ type, ຈຳນວນເຕັມ protocol)
{
    ຈຳນວນເຕັມ fd = real_socket(domain, type, protocol);
    linux_coop_set_nonblocking(fd);
#ifdef SOCK_NONBLOCK
    const ຈຳນວນເຕັມ user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#ບໍ່ດັ່ງນັ້ນ
    const ຈຳນວນເຕັມ user_flags = 0;
#endif
    linux_coop_track_fd(fd, user_flags);
    ສົ່ງຄືນ fd;
}

ຈຳນວນເຕັມ freertos_linux_coop_socketpair(ຈຳນວນເຕັມ domain, ຈຳນວນເຕັມ type, ຈຳນວນເຕັມ protocol, ຈຳນວນເຕັມ *sv)
{
    ຈຳນວນເຕັມ ret = real_socketpair(domain, type, protocol, sv);
    ຖ້າ (ret == 0) {
        linux_coop_set_nonblocking(sv[0]);
        linux_coop_set_nonblocking(sv[1]);
#ifdef SOCK_NONBLOCK
        const ຈຳນວນເຕັມ user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#ບໍ່ດັ່ງນັ້ນ
        const ຈຳນວນເຕັມ user_flags = 0;
#endif
        linux_coop_track_fd(sv[0], user_flags);
        linux_coop_track_fd(sv[1], user_flags);
    }
    ສົ່ງຄືນ ret;
}

ຈຳນວນເຕັມ freertos_linux_coop_pipe(ຈຳນວນເຕັມ *fds)
{
    ຈຳນວນເຕັມ ret = real_pipe(fds);
    ຖ້າ (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        linux_coop_track_fd(fds[0], 0);
        linux_coop_track_fd(fds[1], 0);
    }
    ສົ່ງຄືນ ret;
}

ຈຳນວນເຕັມ freertos_linux_coop_pipe2(ຈຳນວນເຕັມ *fds, ຈຳນວນເຕັມ flags)
{
    ຈຳນວນເຕັມ ret = real_pipe2(fds, flags);
    ຖ້າ (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        const ຈຳນວນເຕັມ user_flags = (flags & O_NONBLOCK) ? O_NONBLOCK : 0;
        linux_coop_track_fd(fds[0], user_flags);
        linux_coop_track_fd(fds[1], user_flags);
    }
    ສົ່ງຄືນ ret;
}

ຈຳນວນເຕັມ freertos_linux_coop_dup(ຈຳນວນເຕັມ oldfd)
{
    ຈຳນວນເຕັມ fd = real_dup(oldfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    ສົ່ງຄືນ fd;
}

ຈຳນວນເຕັມ freertos_linux_coop_dup2(ຈຳນວນເຕັມ oldfd, ຈຳນວນເຕັມ newfd)
{
    ຈຳນວນເຕັມ fd = real_dup2(oldfd, newfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    ສົ່ງຄືນ fd;
}

__attribute__((weak)) ຈຳນວນເຕັມ connect(ຈຳນວນເຕັມ sockfd, const sa_nracanaa sockaddr *addr, socklen_t addrlen)
{
    ສົ່ງຄືນ freertos_linux_coop_connect(sockfd, addr, addrlen);
}

__attribute__((weak)) ຈຳນວນເຕັມ accept(ຈຳນວນເຕັມ sockfd, sa_nracanaa sockaddr *addr, socklen_t *addrlen)
{
    ສົ່ງຄືນ freertos_linux_coop_accept(sockfd, addr, addrlen);
}

__attribute__((weak)) ຈຳນວນເຕັມ pselect(ຈຳນວນເຕັມ nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, const sa_nracanaa timespec *timeout,
            const sigset_t *sigmask)
{
    ສົ່ງຄືນ freertos_linux_coop_pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}

__attribute__((weak)) ຈຳນວນເຕັມ poll(sa_nracanaa pollfd *fds, nfds_t nfds, ຈຳນວນເຕັມ timeout)
{
    ສົ່ງຄືນ freertos_linux_coop_poll(fds, nfds, timeout);
}

__attribute__((weak)) ຈຳນວນເຕັມ socket(ຈຳນວນເຕັມ domain, ຈຳນວນເຕັມ type, ຈຳນວນເຕັມ protocol)
{
    ສົ່ງຄືນ freertos_linux_coop_socket(domain, type, protocol);
}

__attribute__((weak)) ຈຳນວນເຕັມ socketpair(ຈຳນວນເຕັມ domain, ຈຳນວນເຕັມ type, ຈຳນວນເຕັມ protocol, ຈຳນວນເຕັມ sv[2])
{
    ສົ່ງຄືນ freertos_linux_coop_socketpair(domain, type, protocol, sv);
}

__attribute__((weak)) ຈຳນວນເຕັມ pipe(ຈຳນວນເຕັມ fds[2])
{
    ສົ່ງຄືນ freertos_linux_coop_pipe(fds);
}

__attribute__((weak)) ຈຳນວນເຕັມ pipe2(ຈຳນວນເຕັມ fds[2], ຈຳນວນເຕັມ flags)
{
    ສົ່ງຄືນ freertos_linux_coop_pipe2(fds, flags);
}

__attribute__((weak)) ຈຳນວນເຕັມ dup(ຈຳນວນເຕັມ oldfd)
{
    ສົ່ງຄືນ freertos_linux_coop_dup(oldfd);
}

__attribute__((weak)) ຈຳນວນເຕັມ dup2(ຈຳນວນເຕັມ oldfd, ຈຳນວນເຕັມ newfd)
{
    ສົ່ງຄືນ freertos_linux_coop_dup2(oldfd, newfd);
}
