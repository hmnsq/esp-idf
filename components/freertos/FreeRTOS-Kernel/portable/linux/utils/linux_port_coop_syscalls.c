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

static ssize_t  (*real_readv)(පූර්ණ_සංඛ්‍යා, const sa_nracanaa iovec *, පූර්ණ_සංඛ්‍යා);
static ssize_t  (*real_writev)(පූර්ණ_සංඛ්‍යා, const sa_nracanaa iovec *, පූර්ණ_සංඛ්‍යා);
static ssize_t  (*real_recv)(පූර්ණ_සංඛ්‍යා, void *, size_t, පූර්ණ_සංඛ්‍යා);
static ssize_t  (*real_send)(පූර්ණ_සංඛ්‍යා, const void *, size_t, පූර්ණ_සංඛ්‍යා);
static ssize_t  (*real_recvfrom)(පූර්ණ_සංඛ්‍යා, void *, size_t, පූර්ණ_සංඛ්‍යා,
                                 sa_nracanaa sockaddr *, socklen_t *);
static ssize_t  (*real_sendto)(පූර්ණ_සංඛ්‍යා, const void *, size_t, පූර්ණ_සංඛ්‍යා,
                                const sa_nracanaa sockaddr *, socklen_t);
static ssize_t  (*real_recvmsg)(පූර්ණ_සංඛ්‍යා, sa_nracanaa msghdr *, පූර්ණ_සංඛ්‍යා);
static ssize_t  (*real_sendmsg)(පූර්ණ_සංඛ්‍යා, const sa_nracanaa msghdr *, පූර්ණ_සංඛ්‍යා);
static පූර්ණ_සංඛ්‍යා      (*real_connect)(පූර්ණ_සංඛ්‍යා, const sa_nracanaa sockaddr *, socklen_t);
static පූර්ණ_සංඛ්‍යා      (*real_accept)(පූර්ණ_සංඛ්‍යා, sa_nracanaa sockaddr *, socklen_t *);
static පූර්ණ_සංඛ්‍යා      (*real_pselect)(පූර්ණ_සංඛ්‍යා, fd_set *, fd_set *, fd_set *,
                                const sa_nracanaa timespec *, const sigset_t *);
static පූර්ණ_සංඛ්‍යා      (*real_poll)(sa_nracanaa pollfd *, nfds_t, පූර්ණ_සංඛ්‍යා);
static පූර්ණ_සංඛ්‍යා      (*real_socket)(පූර්ණ_සංඛ්‍යා, පූර්ණ_සංඛ්‍යා, පූර්ණ_සංඛ්‍යා);
static පූර්ණ_සංඛ්‍යා      (*real_socketpair)(පූර්ණ_සංඛ්‍යා, පූර්ණ_සංඛ්‍යා, පූර්ණ_සංඛ්‍යා, පූර්ණ_සංඛ්‍යා[2]);
static පූර්ණ_සංඛ්‍යා      (*real_pipe)(පූර්ණ_සංඛ්‍යා[2]);
static පූර්ණ_සංඛ්‍යා      (*real_pipe2)(පූර්ණ_සංඛ්‍යා[2], පූර්ණ_සංඛ්‍යා);
static පූර්ණ_සංඛ්‍යා      (*real_dup)(පූර්ණ_සංඛ්‍යා);
static පූර්ණ_සංඛ්‍යා      (*real_dup2)(පූර්ණ_සංඛ්‍යා, පූර්ණ_සංඛ්‍යා);
static පූර්ණ_සංඛ්‍යා      (*real_nanosleep)(const sa_nracanaa timespec *, sa_nracanaa timespec *);
static ssize_t  (*real_read)(පූර්ණ_සංඛ්‍යා, void *, size_t);
static ssize_t  (*real_write)(පූර්ණ_සංඛ්‍යා, const void *, size_t);
static ssize_t  (*real_pread)(පූර්ණ_සංඛ්‍යා, void *, size_t, off_t);
static ssize_t  (*real_pwrite)(පූර්ණ_සංඛ්‍යා, const void *, size_t, off_t);
static පූර්ණ_සංඛ්‍යා      (*real_close)(පූර්ණ_සංඛ්‍යා);
static පූර්ණ_සංඛ්‍යා      (*real_select)(පූර්ණ_සංඛ්‍යා, fd_set *, fd_set *, fd_set *,
                                sa_nracanaa timeval *);

typedef පූර්ණ_සංඛ්‍යා (*linux_coop_fcntl_fn_t)(පූර්ණ_සංඛ්‍යා, පූර්ණ_සංඛ්‍යා, ...);
static linux_coop_fcntl_fn_t real_fcntl;

typedef පූර්ණ_සංඛ්‍යා (*linux_coop_open_fn_t)(const var_na *, පූර්ණ_සංඛ්‍යා, mode_t);
static linux_coop_open_fn_t real_open;

typedef sa_nracanaa linux_coop_fd_state {
    පූර්ණ_සංඛ්‍යා fd;
    පූර්ණ_සංඛ්‍යා user_flags;
    sa_nracanaa linux_coop_fd_state *next;
} linux_coop_fd_state_t;

static linux_coop_fd_state_t *s_fd_state_list;
static pthread_mutex_t s_fd_state_lock = PTHREAD_MUTEX_INITIALIZER;

static linux_coop_fd_state_t *linux_coop_find_fd_locked(පූර්ණ_සංඛ්‍යා fd)
{
    linux_coop_fd_state_t *node = s_fd_state_list;
    අතරතුර (node != NULL) {
        නම් (node->fd == fd) {
            ආපසු node;
        }
        node = node->next;
    }
    ආපසු NULL;
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

void linux_coop_set_nonblocking(පූර්ණ_සංඛ්‍යා fd)
{
    නම් (fd >= 0) {
        පූර්ණ_සංඛ්‍යා flags = real_fcntl(fd, F_GETFL, 0);
        නම් (flags >= 0) {
            real_fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
    }
}

void linux_coop_track_fd(පූර්ණ_සංඛ්‍යා fd, පූර්ණ_සංඛ්‍යා user_flags)
{
    නම් (fd < 0) {
        ආපසු;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *node = linux_coop_find_fd_locked(fd);
    නම් (node != NULL) {
        node->user_flags = user_flags;
        pthread_mutex_unlock(&s_fd_state_lock);
        ආපසු;
    }

    node = calloc(1, parimaa_na(*node));
    නම් (node != NULL) {
        node->fd = fd;
        node->user_flags = user_flags;
        node->next = s_fd_state_list;
        s_fd_state_list = node;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

void linux_coop_untrack_fd(පූර්ණ_සංඛ්‍යා fd)
{
    නම් (fd < 0) {
        ආපසු;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *prev = NULL;
    linux_coop_fd_state_t *node = s_fd_state_list;
    අතරතුර (node != NULL) {
        නම් (node->fd == fd) {
            නම් (prev == NULL) {
                s_fd_state_list = node->next;
            } නැතහොත් {
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

void linux_coop_set_user_flags(පූර්ණ_සංඛ්‍යා fd, පූර්ණ_සංඛ්‍යා user_flags)
{
    linux_coop_track_fd(fd, user_flags);
}

bool linux_coop_get_user_flags(පූර්ණ_සංඛ්‍යා fd, පූර්ණ_සංඛ්‍යා *flags)
{
    නම් (fd < 0 || flags == NULL) {
        ආපසු false;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *node = linux_coop_find_fd_locked(fd);
    නම් (node == NULL) {
        pthread_mutex_unlock(&s_fd_state_lock);
        ආපසු false;
    }
    *flags = node->user_flags;
    pthread_mutex_unlock(&s_fd_state_lock);
    ආපසු true;
}

bool linux_coop_fd_user_nonblocking(පූර්ණ_සංඛ්‍යා fd)
{
    පූර්ණ_සංඛ්‍යා user_flags = 0;
    නම් (linux_coop_get_user_flags(fd, &user_flags)) {
        ආපසු (user_flags & O_NONBLOCK) != 0;
    }
    ආපසු false;
}


ssize_t freertos_linux_coop_read(පූර්ණ_සංඛ්‍යා fd, void *buf, size_t count)
{
    LINUX_COOP_IO_LOOP(fd, real_read(fd, buf, count))
}

ssize_t freertos_linux_coop_write(පූර්ණ_සංඛ්‍යා fd, const void *buf, size_t count)
{
    LINUX_COOP_IO_LOOP(fd, real_write(fd, buf, count))
}

ssize_t freertos_linux_coop_pread(පූර්ණ_සංඛ්‍යා fd, void *buf, size_t count, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pread(fd, buf, count, offset))
}

ssize_t freertos_linux_coop_pwrite(පූර්ණ_සංඛ්‍යා fd, const void *buf, size_t count, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pwrite(fd, buf, count, offset))
}

පූර්ණ_සංඛ්‍යා freertos_linux_coop_open(const var_na *path, පූර්ණ_සංඛ්‍යා flags, පූර්ණ_සංඛ්‍යා mode)
{
    පූර්ණ_සංඛ්‍යා fd = real_open(path, flags, (mode_t)mode);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, flags & O_NONBLOCK);
    ආපසු fd;
}

පූර්ණ_සංඛ්‍යා freertos_linux_coop_close(පූර්ණ_සංඛ්‍යා fd)
{
    පූර්ණ_සංඛ්‍යා ret = real_close(fd);
    නම් (ret == 0 || errno == EINTR) {
        /* On Linux, close() always releases the fd even when returning
         * EINTR.  Retrying would risk closing a fd reused by another
         * thread.  Treat EINTR as success. */
        linux_coop_untrack_fd(fd);
        ආපසු 0;
    }
    ආපසු -1;
}

පූර්ණ_සංඛ්‍යා freertos_linux_coop_fcntl(පූර්ණ_සංඛ්‍යා fd, පූර්ණ_සංඛ්‍යා cmd, පූර්ණ_සංඛ්‍යා arg)
{
    නම් (cmd == F_GETFL) {
        පූර්ණ_සංඛ්‍යා real_flags = real_fcntl(fd, F_GETFL, 0);
        නම් (real_flags < 0) {
            ආපසු -1;
        }
        පූර්ණ_සංඛ්‍යා user_flags = 0;
        නම් (linux_coop_get_user_flags(fd, &user_flags)) {
            real_flags &= ~O_NONBLOCK;
            real_flags |= (user_flags & O_NONBLOCK);
        }
        ආපසු real_flags;
    }
    නම් (cmd == F_SETFL) {
        linux_coop_set_user_flags(fd, arg);
        පූර්ණ_සංඛ්‍යා internal_flags = arg | O_NONBLOCK;
        ආපසු real_fcntl(fd, F_SETFL, internal_flags);
    }
    ආපසු real_fcntl(fd, cmd, arg);
}

පූර්ණ_සංඛ්‍යා freertos_linux_coop_select(පූර්ණ_සංඛ්‍යා nfds, fd_set *readfds, fd_set *writefds,
                      fd_set *exceptfds, sa_nracanaa timeval *timeout)
{
    long timeout_ms = -1;
    නම් (timeout != NULL) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_usec + 999) / 1000;
        නම් (timeout_ms == 0) {
            ආපසු real_select(nfds, readfds, writefds, exceptfds, timeout);
        }
    }

    /* real_select() mutates the fd_sets in place (clears them, then sets bits
     * for ready descriptors). To keep polling the same descriptors across
     * iterations we must snapshot the caller's sets and restore them on every
     * loop. */
    fd_set rfds_in, wfds_in, efds_in;
    නම් (readfds != NULL) {
        rfds_in = *readfds;
    }
    නම් (writefds != NULL) {
        wfds_in = *writefds;
    }
    නම් (exceptfds != NULL) {
        efds_in = *exceptfds;
    }

    long waited_ms = 0;
    අතරතුර (1) {
        නම් (readfds != NULL) {
            *readfds = rfds_in;
        }
        නම් (writefds != NULL) {
            *writefds = wfds_in;
        }
        නම් (exceptfds != NULL) {
            *exceptfds = efds_in;
        }

        sa_nracanaa timeval zero_tv = {0, 0};
        පූර්ණ_සංඛ්‍යා ret = real_select(nfds, readfds, writefds, exceptfds, &zero_tv);
        නම් (ret != 0) {
            ආපසු ret;
        }
        නම් (timeout_ms == 0) {
            ආපසු 0;
        }
        නම් (timeout_ms > 0 && waited_ms >= timeout_ms) {
            ආපසු 0;
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

__attribute__((weak)) ssize_t read(පූර්ණ_සංඛ්‍යා fd, void *buf, size_t count)
{
    ආපසු freertos_linux_coop_read(fd, buf, count);
}

__attribute__((weak)) ssize_t write(පූර්ණ_සංඛ්‍යා fd, const void *buf, size_t count)
{
    ආපසු freertos_linux_coop_write(fd, buf, count);
}

__attribute__((weak)) ssize_t pread(පූර්ණ_සංඛ්‍යා fd, void *buf, size_t count, off_t offset)
{
    ආපසු freertos_linux_coop_pread(fd, buf, count, offset);
}

__attribute__((weak)) ssize_t pwrite(පූර්ණ_සංඛ්‍යා fd, const void *buf, size_t count, off_t offset)
{
    ආපසු freertos_linux_coop_pwrite(fd, buf, count, offset);
}

__attribute__((weak)) පූර්ණ_සංඛ්‍යා open(const var_na *path, පූර්ණ_සංඛ්‍යා flags, ...)
{
    පූර්ණ_සංඛ්‍යා mode = 0;
    නම් (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        mode = va_arg(ap, පූර්ණ_සංඛ්‍යා);
        va_end(ap);
    }
    ආපසු freertos_linux_coop_open(path, flags, mode);
}

__attribute__((weak)) පූර්ණ_සංඛ්‍යා close(පූර්ණ_සංඛ්‍යා fd)
{
    ආපසු freertos_linux_coop_close(fd);
}

__attribute__((weak)) පූර්ණ_සංඛ්‍යා fcntl(පූර්ණ_සංඛ්‍යා fd, පූර්ණ_සංඛ්‍යා cmd, ...)
{
    පූර්ණ_සංඛ්‍යා arg = 0;
    නම් (cmd != F_GETFL) {
        va_list list;
        va_start(list, cmd);
        arg = va_arg(list, පූර්ණ_සංඛ්‍යා);
        va_end(list);
    }
    ආපසු freertos_linux_coop_fcntl(fd, cmd, arg);
}

__attribute__((weak)) පූර්ණ_සංඛ්‍යා select(පූර්ණ_සංඛ්‍යා nfds, fd_set *readfds, fd_set *writefds,
                                 fd_set *exceptfds, sa_nracanaa timeval *timeout)
{
    ආපසු freertos_linux_coop_select(nfds, readfds, writefds, exceptfds, timeout);
}

ssize_t freertos_linux_coop_readv(පූර්ණ_සංඛ්‍යා fd, const sa_nracanaa iovec *iov, පූර්ණ_සංඛ්‍යා iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_readv(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_writev(පූර්ණ_සංඛ්‍යා fd, const sa_nracanaa iovec *iov, පූර්ණ_සංඛ්‍යා iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_writev(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_recv(පූර්ණ_සංඛ්‍යා sockfd, void *buf, size_t len, පූර්ණ_සංඛ්‍යා flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recv(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_send(පූර්ණ_සංඛ්‍යා sockfd, const void *buf, size_t len, පූර්ණ_සංඛ්‍යා flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_send(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_recvfrom(පූර්ණ_සංඛ්‍යා sockfd, void *buf, size_t len, පූර්ණ_සංඛ්‍යා flags,
                 sa_nracanaa sockaddr *src_addr, socklen_t *addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvfrom(sockfd, buf, len, flags | MSG_DONTWAIT,
                            src_addr, addrlen))
}

ssize_t freertos_linux_coop_sendto(පූර්ණ_සංඛ්‍යා sockfd, const void *buf, size_t len, පූර්ණ_සංඛ්‍යා flags,
               const sa_nracanaa sockaddr *dest_addr, socklen_t addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendto(sockfd, buf, len, flags | MSG_DONTWAIT,
                          dest_addr, addrlen))
}

ssize_t freertos_linux_coop_recvmsg(පූර්ණ_සංඛ්‍යා sockfd, sa_nracanaa msghdr *msg, පූර්ණ_සංඛ්‍යා flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvmsg(sockfd, msg, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_sendmsg(පූර්ණ_සංඛ්‍යා sockfd, const sa_nracanaa msghdr *msg, පූර්ණ_සංඛ්‍යා flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendmsg(sockfd, msg, flags | MSG_DONTWAIT))
}

__attribute__((weak)) ssize_t readv(පූර්ණ_සංඛ්‍යා fd, const sa_nracanaa iovec *iov, පූර්ණ_සංඛ්‍යා iovcnt)
{
    ආපසු freertos_linux_coop_readv(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t writev(පූර්ණ_සංඛ්‍යා fd, const sa_nracanaa iovec *iov, පූර්ණ_සංඛ්‍යා iovcnt)
{
    ආපසු freertos_linux_coop_writev(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t recv(පූර්ණ_සංඛ්‍යා sockfd, void *buf, size_t len, පූර්ණ_සංඛ්‍යා flags)
{
    ආපසු freertos_linux_coop_recv(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t send(පූර්ණ_සංඛ්‍යා sockfd, const void *buf, size_t len, පූර්ණ_සංඛ්‍යා flags)
{
    ආපසු freertos_linux_coop_send(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t recvfrom(පූර්ණ_සංඛ්‍යා sockfd, void *buf, size_t len, පූර්ණ_සංඛ්‍යා flags,
                 sa_nracanaa sockaddr *src_addr, socklen_t *addrlen)
{
    ආපසු freertos_linux_coop_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

__attribute__((weak)) ssize_t sendto(පූර්ණ_සංඛ්‍යා sockfd, const void *buf, size_t len, පූර්ණ_සංඛ්‍යා flags,
               const sa_nracanaa sockaddr *dest_addr, socklen_t addrlen)
{
    ආපසු freertos_linux_coop_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

__attribute__((weak)) ssize_t recvmsg(පූර්ණ_සංඛ්‍යා sockfd, sa_nracanaa msghdr *msg, පූර්ණ_සංඛ්‍යා flags)
{
    ආපසු freertos_linux_coop_recvmsg(sockfd, msg, flags);
}

__attribute__((weak)) ssize_t sendmsg(පූර්ණ_සංඛ්‍යා sockfd, const sa_nracanaa msghdr *msg, පූර්ණ_සංඛ්‍යා flags)
{
    ආපසු freertos_linux_coop_sendmsg(sockfd, msg, flags);
}

පූර්ණ_සංඛ්‍යා freertos_linux_coop_connect(පූර්ණ_සංඛ්‍යා sockfd, const sa_nracanaa sockaddr *addr, socklen_t addrlen)
{
    අතරතුර (1)
    {
        පූර්ණ_සංඛ්‍යා ret = real_connect(sockfd, addr, addrlen);
        නම් (ret == 0) {
            ආපසු ret;
        }

        නම් (errno == EINPROGRESS || errno == EALREADY) {
            sa_nracanaa pollfd pfd;
            pfd.fd = sockfd;
            pfd.events = POLLOUT;
            pfd.revents = 0;

            පූර්ණ_සංඛ්‍යා poll_retries = 0;
            අතරතුර (1)
            {
                පූර්ණ_සංඛ්‍යා press = real_poll(&pfd, 1, 0);
                නම් (press > 0) {
                    පූර්ණ_සංඛ්‍යා so_err = 0;
                    socklen_t len = parimaa_na(so_err);
                    නම් (getsockopt(sockfd, SOL_SOCKET, SO_ERROR,
                                   &so_err, &len) < 0) {
                        ආපසු -1;
                    }
                    නම් (so_err == 0) {
                        ආපසු 0;
                    } නැතහොත් {
                        errno = so_err;
                        ආපසු -1;
                    }
                } නැතහොත් නම් (press == 0) {
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    continue;
                } නැතහොත් {
                    නම් (errno == EINTR) {
                        continue;
                    }
                    /* Non-EINTR poll error; bail out after a few retries */
                    නම් (++poll_retries > 3) {
                        ආපසු -1;
                    }
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    continue;
                }
            }
        }

        නම් (errno == EINTR) {
            ආපසු -1;
        }

        ආපසු -1;
    }
}

පූර්ණ_සංඛ්‍යා freertos_linux_coop_accept(පූර්ණ_සංඛ්‍යා sockfd, sa_nracanaa sockaddr *addr, socklen_t *addrlen)
{
    අතරතුර (1)
    {
        පූර්ණ_සංඛ්‍යා fd = real_accept(sockfd, addr, addrlen);
        නම් (fd >= 0) {
            linux_coop_set_nonblocking(fd);
            linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(sockfd) ? O_NONBLOCK : 0);
            ආපසු fd;
        }
        නම් (errno == EAGAIN || errno == EWOULDBLOCK) {
            නම් (linux_coop_fd_user_nonblocking(sockfd)) {
                ආපසු -1;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            continue;
        }
        ආපසු -1;
    }
}

පූර්ණ_සංඛ්‍යා freertos_linux_coop_pselect(පූර්ණ_සංඛ්‍යා nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, const sa_nracanaa timespec *timeout,
            const sigset_t *sigmask)
{
    long timeout_ms = -1;
    නම් (timeout != NULL) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_nsec + 999999) / 1000000;
        නම් (timeout_ms == 0) {
            ආපසු real_pselect(nfds, readfds, writefds, exceptfds,
                                timeout, sigmask);
        }
    }

    long waited_ms = 0;
    අතරතුර (1)
    {
        sa_nracanaa timespec zero_ts = {0, 0};
        පූර්ණ_සංඛ්‍යා ret = real_pselect(nfds, readfds, writefds, exceptfds,
                               &zero_ts, sigmask);
        නම් (ret != 0) {
            ආපසු ret;
        }

        නම් (timeout_ms == 0 ||
            (timeout_ms > 0 && waited_ms >= timeout_ms)) {
            ආපසු 0;
        }

        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

පූර්ණ_සංඛ්‍යා freertos_linux_coop_poll(sa_nracanaa pollfd *fds, nfds_t nfds, පූර්ණ_සංඛ්‍යා timeout)
{
    නම් (timeout == 0) {
        ආපසු real_poll(fds, nfds, 0);
    }

    long timeout_ms = -1;
    නම් (timeout > 0) {
        timeout_ms = timeout;
    }

    long waited_ms = 0;

    අතරතුර (1)
    {
        පූර්ණ_සංඛ්‍යා ret = real_poll(fds, nfds, 0);
        නම් (ret > 0) {
            ආපසු ret;
        } නැතහොත් නම් (ret == 0) {
            නම් (timeout_ms == 0 ||
                (timeout_ms > 0 && waited_ms >= timeout_ms)) {
                ආපසු 0;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            waited_ms += LINUX_COOP_TICK_MS;
            continue;
        } නැතහොත් {
            නම් (errno == EINTR) {
                continue;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            නම් (timeout_ms > 0 && waited_ms >= timeout_ms) {
                ආපසු -1;
            }
            waited_ms += LINUX_COOP_TICK_MS;
        }
    }
}

පූර්ණ_සංඛ්‍යා nanosleep(const sa_nracanaa timespec *req, sa_nracanaa timespec *rem)
{
    නම් (req == NULL) {
        errno = EFAULT;
        ආපසු -1;
    }
    නම් (!linux_port_in_freertos_task()) {
        ආපසු real_nanosleep(req, rem);
    }
    long ms = req->tv_sec * 1000 + (req->tv_nsec + 999999) / 1000000;
    නම් (ms <= 0) {
        ms = 1;
    }
    vTaskDelay(ms);
    නම් (rem) {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }
    ආපසු 0;
}

unsigned පූර්ණ_සංඛ්‍යා sleep(unsigned පූර්ණ_සංඛ්‍යා seconds)
{
    linux_coop_yield(seconds * 1000);
    ආපසු 0;
}

පූර්ණ_සංඛ්‍යා usleep(useconds_t usec)
{
    linux_coop_yield(usec / 1000);
    ආපසු 0;
}

පූර්ණ_සංඛ්‍යා freertos_linux_coop_socket(පූර්ණ_සංඛ්‍යා domain, පූර්ණ_සංඛ්‍යා type, පූර්ණ_සංඛ්‍යා protocol)
{
    පූර්ණ_සංඛ්‍යා fd = real_socket(domain, type, protocol);
    linux_coop_set_nonblocking(fd);
#ifdef SOCK_NONBLOCK
    const පූර්ණ_සංඛ්‍යා user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#නැතහොත්
    const පූර්ණ_සංඛ්‍යා user_flags = 0;
#endif
    linux_coop_track_fd(fd, user_flags);
    ආපසු fd;
}

පූර්ණ_සංඛ්‍යා freertos_linux_coop_socketpair(පූර්ණ_සංඛ්‍යා domain, පූර්ණ_සංඛ්‍යා type, පූර්ණ_සංඛ්‍යා protocol, පූර්ණ_සංඛ්‍යා *sv)
{
    පූර්ණ_සංඛ්‍යා ret = real_socketpair(domain, type, protocol, sv);
    නම් (ret == 0) {
        linux_coop_set_nonblocking(sv[0]);
        linux_coop_set_nonblocking(sv[1]);
#ifdef SOCK_NONBLOCK
        const පූර්ණ_සංඛ්‍යා user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#නැතහොත්
        const පූර්ණ_සංඛ්‍යා user_flags = 0;
#endif
        linux_coop_track_fd(sv[0], user_flags);
        linux_coop_track_fd(sv[1], user_flags);
    }
    ආපසු ret;
}

පූර්ණ_සංඛ්‍යා freertos_linux_coop_pipe(පූර්ණ_සංඛ්‍යා *fds)
{
    පූර්ණ_සංඛ්‍යා ret = real_pipe(fds);
    නම් (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        linux_coop_track_fd(fds[0], 0);
        linux_coop_track_fd(fds[1], 0);
    }
    ආපසු ret;
}

පූර්ණ_සංඛ්‍යා freertos_linux_coop_pipe2(පූර්ණ_සංඛ්‍යා *fds, පූර්ණ_සංඛ්‍යා flags)
{
    පූර්ණ_සංඛ්‍යා ret = real_pipe2(fds, flags);
    නම් (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        const පූර්ණ_සංඛ්‍යා user_flags = (flags & O_NONBLOCK) ? O_NONBLOCK : 0;
        linux_coop_track_fd(fds[0], user_flags);
        linux_coop_track_fd(fds[1], user_flags);
    }
    ආපසු ret;
}

පූර්ණ_සංඛ්‍යා freertos_linux_coop_dup(පූර්ණ_සංඛ්‍යා oldfd)
{
    පූර්ණ_සංඛ්‍යා fd = real_dup(oldfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    ආපසු fd;
}

පූර්ණ_සංඛ්‍යා freertos_linux_coop_dup2(පූර්ණ_සංඛ්‍යා oldfd, පූර්ණ_සංඛ්‍යා newfd)
{
    පූර්ණ_සංඛ්‍යා fd = real_dup2(oldfd, newfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    ආපසු fd;
}

__attribute__((weak)) පූර්ණ_සංඛ්‍යා connect(පූර්ණ_සංඛ්‍යා sockfd, const sa_nracanaa sockaddr *addr, socklen_t addrlen)
{
    ආපසු freertos_linux_coop_connect(sockfd, addr, addrlen);
}

__attribute__((weak)) පූර්ණ_සංඛ්‍යා accept(පූර්ණ_සංඛ්‍යා sockfd, sa_nracanaa sockaddr *addr, socklen_t *addrlen)
{
    ආපසු freertos_linux_coop_accept(sockfd, addr, addrlen);
}

__attribute__((weak)) පූර්ණ_සංඛ්‍යා pselect(පූර්ණ_සංඛ්‍යා nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, const sa_nracanaa timespec *timeout,
            const sigset_t *sigmask)
{
    ආපසු freertos_linux_coop_pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}

__attribute__((weak)) පූර්ණ_සංඛ්‍යා poll(sa_nracanaa pollfd *fds, nfds_t nfds, පූර්ණ_සංඛ්‍යා timeout)
{
    ආපසු freertos_linux_coop_poll(fds, nfds, timeout);
}

__attribute__((weak)) පූර්ණ_සංඛ්‍යා socket(පූර්ණ_සංඛ්‍යා domain, පූර්ණ_සංඛ්‍යා type, පූර්ණ_සංඛ්‍යා protocol)
{
    ආපසු freertos_linux_coop_socket(domain, type, protocol);
}

__attribute__((weak)) පූර්ණ_සංඛ්‍යා socketpair(පූර්ණ_සංඛ්‍යා domain, පූර්ණ_සංඛ්‍යා type, පූර්ණ_සංඛ්‍යා protocol, පූර්ණ_සංඛ්‍යා sv[2])
{
    ආපසු freertos_linux_coop_socketpair(domain, type, protocol, sv);
}

__attribute__((weak)) පූර්ණ_සංඛ්‍යා pipe(පූර්ණ_සංඛ්‍යා fds[2])
{
    ආපසු freertos_linux_coop_pipe(fds);
}

__attribute__((weak)) පූර්ණ_සංඛ්‍යා pipe2(පූර්ණ_සංඛ්‍යා fds[2], පූර්ණ_සංඛ්‍යා flags)
{
    ආපසු freertos_linux_coop_pipe2(fds, flags);
}

__attribute__((weak)) පූර්ණ_සංඛ්‍යා dup(පූර්ණ_සංඛ්‍යා oldfd)
{
    ආපසු freertos_linux_coop_dup(oldfd);
}

__attribute__((weak)) පූර්ණ_සංඛ්‍යා dup2(පූර්ණ_සංඛ්‍යා oldfd, පූර්ණ_සංඛ්‍යා newfd)
{
    ආපසු freertos_linux_coop_dup2(oldfd, newfd);
}
