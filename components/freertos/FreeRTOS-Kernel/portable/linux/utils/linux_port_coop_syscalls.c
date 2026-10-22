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

static ssize_t  (*real_readv)(số_nguyên, const struct iovec *, số_nguyên);
static ssize_t  (*real_writev)(số_nguyên, const struct iovec *, số_nguyên);
static ssize_t  (*real_recv)(số_nguyên, void *, size_t, số_nguyên);
static ssize_t  (*real_send)(số_nguyên, const void *, size_t, số_nguyên);
static ssize_t  (*real_recvfrom)(số_nguyên, void *, size_t, số_nguyên,
                                 struct sockaddr *, socklen_t *);
static ssize_t  (*real_sendto)(số_nguyên, const void *, size_t, số_nguyên,
                                const struct sockaddr *, socklen_t);
static ssize_t  (*real_recvmsg)(số_nguyên, struct msghdr *, số_nguyên);
static ssize_t  (*real_sendmsg)(số_nguyên, const struct msghdr *, số_nguyên);
static số_nguyên      (*real_connect)(số_nguyên, const struct sockaddr *, socklen_t);
static số_nguyên      (*real_accept)(số_nguyên, struct sockaddr *, socklen_t *);
static số_nguyên      (*real_pselect)(số_nguyên, fd_set *, fd_set *, fd_set *,
                                const struct timespec *, const sigset_t *);
static số_nguyên      (*real_poll)(struct pollfd *, nfds_t, số_nguyên);
static số_nguyên      (*real_socket)(số_nguyên, số_nguyên, số_nguyên);
static số_nguyên      (*real_socketpair)(số_nguyên, số_nguyên, số_nguyên, số_nguyên[2]);
static số_nguyên      (*real_pipe)(số_nguyên[2]);
static số_nguyên      (*real_pipe2)(số_nguyên[2], số_nguyên);
static số_nguyên      (*real_dup)(số_nguyên);
static số_nguyên      (*real_dup2)(số_nguyên, số_nguyên);
static số_nguyên      (*real_nanosleep)(const struct timespec *, struct timespec *);
static ssize_t  (*real_read)(số_nguyên, void *, size_t);
static ssize_t  (*real_write)(số_nguyên, const void *, size_t);
static ssize_t  (*real_pread)(số_nguyên, void *, size_t, off_t);
static ssize_t  (*real_pwrite)(số_nguyên, const void *, size_t, off_t);
static số_nguyên      (*real_close)(số_nguyên);
static số_nguyên      (*real_select)(số_nguyên, fd_set *, fd_set *, fd_set *,
                                struct timeval *);

typedef số_nguyên (*linux_coop_fcntl_fn_t)(số_nguyên, số_nguyên, ...);
static linux_coop_fcntl_fn_t real_fcntl;

typedef số_nguyên (*linux_coop_open_fn_t)(const char *, số_nguyên, mode_t);
static linux_coop_open_fn_t real_open;

typedef struct linux_coop_fd_state {
    số_nguyên fd;
    số_nguyên user_flags;
    struct linux_coop_fd_state *next;
} linux_coop_fd_state_t;

static linux_coop_fd_state_t *s_fd_state_list;
static pthread_mutex_t s_fd_state_lock = PTHREAD_MUTEX_INITIALIZER;

static linux_coop_fd_state_t *linux_coop_find_fd_locked(số_nguyên fd)
{
    linux_coop_fd_state_t *node = s_fd_state_list;
    trong_khi (node != NULL) {
        nếu (node->fd == fd) {
            trả_về node;
        }
        node = node->next;
    }
    trả_về NULL;
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

void linux_coop_set_nonblocking(số_nguyên fd)
{
    nếu (fd >= 0) {
        số_nguyên flags = real_fcntl(fd, F_GETFL, 0);
        nếu (flags >= 0) {
            real_fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
    }
}

void linux_coop_track_fd(số_nguyên fd, số_nguyên user_flags)
{
    nếu (fd < 0) {
        trả_về;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *node = linux_coop_find_fd_locked(fd);
    nếu (node != NULL) {
        node->user_flags = user_flags;
        pthread_mutex_unlock(&s_fd_state_lock);
        trả_về;
    }

    node = calloc(1, sizeof(*node));
    nếu (node != NULL) {
        node->fd = fd;
        node->user_flags = user_flags;
        node->next = s_fd_state_list;
        s_fd_state_list = node;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

void linux_coop_untrack_fd(số_nguyên fd)
{
    nếu (fd < 0) {
        trả_về;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *prev = NULL;
    linux_coop_fd_state_t *node = s_fd_state_list;
    trong_khi (node != NULL) {
        nếu (node->fd == fd) {
            nếu (prev == NULL) {
                s_fd_state_list = node->next;
            } không_thì {
                prev->next = node->next;
            }
            free(node);
            break;
        }
        prev = node;
        node = node->next;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

void linux_coop_set_user_flags(số_nguyên fd, số_nguyên user_flags)
{
    linux_coop_track_fd(fd, user_flags);
}

bool linux_coop_get_user_flags(số_nguyên fd, số_nguyên *flags)
{
    nếu (fd < 0 || flags == NULL) {
        trả_về false;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *node = linux_coop_find_fd_locked(fd);
    nếu (node == NULL) {
        pthread_mutex_unlock(&s_fd_state_lock);
        trả_về false;
    }
    *flags = node->user_flags;
    pthread_mutex_unlock(&s_fd_state_lock);
    trả_về true;
}

bool linux_coop_fd_user_nonblocking(số_nguyên fd)
{
    số_nguyên user_flags = 0;
    nếu (linux_coop_get_user_flags(fd, &user_flags)) {
        trả_về (user_flags & O_NONBLOCK) != 0;
    }
    trả_về false;
}


ssize_t freertos_linux_coop_read(số_nguyên fd, void *buf, size_t count)
{
    LINUX_COOP_IO_LOOP(fd, real_read(fd, buf, count))
}

ssize_t freertos_linux_coop_write(số_nguyên fd, const void *buf, size_t count)
{
    LINUX_COOP_IO_LOOP(fd, real_write(fd, buf, count))
}

ssize_t freertos_linux_coop_pread(số_nguyên fd, void *buf, size_t count, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pread(fd, buf, count, offset))
}

ssize_t freertos_linux_coop_pwrite(số_nguyên fd, const void *buf, size_t count, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pwrite(fd, buf, count, offset))
}

số_nguyên freertos_linux_coop_open(const char *path, số_nguyên flags, số_nguyên mode)
{
    số_nguyên fd = real_open(path, flags, (mode_t)mode);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, flags & O_NONBLOCK);
    trả_về fd;
}

số_nguyên freertos_linux_coop_close(số_nguyên fd)
{
    số_nguyên ret = real_close(fd);
    nếu (ret == 0 || errno == EINTR) {
        /* On Linux, close() always releases the fd even when returning
         * EINTR.  Retrying would risk closing a fd reused by another
         * thread.  Treat EINTR as success. */
        linux_coop_untrack_fd(fd);
        trả_về 0;
    }
    trả_về -1;
}

số_nguyên freertos_linux_coop_fcntl(số_nguyên fd, số_nguyên cmd, số_nguyên arg)
{
    nếu (cmd == F_GETFL) {
        số_nguyên real_flags = real_fcntl(fd, F_GETFL, 0);
        nếu (real_flags < 0) {
            trả_về -1;
        }
        số_nguyên user_flags = 0;
        nếu (linux_coop_get_user_flags(fd, &user_flags)) {
            real_flags &= ~O_NONBLOCK;
            real_flags |= (user_flags & O_NONBLOCK);
        }
        trả_về real_flags;
    }
    nếu (cmd == F_SETFL) {
        linux_coop_set_user_flags(fd, arg);
        số_nguyên internal_flags = arg | O_NONBLOCK;
        trả_về real_fcntl(fd, F_SETFL, internal_flags);
    }
    trả_về real_fcntl(fd, cmd, arg);
}

số_nguyên freertos_linux_coop_select(số_nguyên nfds, fd_set *readfds, fd_set *writefds,
                      fd_set *exceptfds, struct timeval *timeout)
{
    long timeout_ms = -1;
    nếu (timeout != NULL) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_usec + 999) / 1000;
        nếu (timeout_ms == 0) {
            trả_về real_select(nfds, readfds, writefds, exceptfds, timeout);
        }
    }

    /* real_select() mutates the fd_sets in place (clears them, then sets bits
     * for ready descriptors). To keep polling the same descriptors across
     * iterations we must snapshot the caller's sets and restore them on every
     * loop. */
    fd_set rfds_in, wfds_in, efds_in;
    nếu (readfds != NULL) {
        rfds_in = *readfds;
    }
    nếu (writefds != NULL) {
        wfds_in = *writefds;
    }
    nếu (exceptfds != NULL) {
        efds_in = *exceptfds;
    }

    long waited_ms = 0;
    trong_khi (1) {
        nếu (readfds != NULL) {
            *readfds = rfds_in;
        }
        nếu (writefds != NULL) {
            *writefds = wfds_in;
        }
        nếu (exceptfds != NULL) {
            *exceptfds = efds_in;
        }

        struct timeval zero_tv = {0, 0};
        số_nguyên ret = real_select(nfds, readfds, writefds, exceptfds, &zero_tv);
        nếu (ret != 0) {
            trả_về ret;
        }
        nếu (timeout_ms == 0) {
            trả_về 0;
        }
        nếu (timeout_ms > 0 && waited_ms >= timeout_ms) {
            trả_về 0;
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

__attribute__((weak)) ssize_t read(số_nguyên fd, void *buf, size_t count)
{
    trả_về freertos_linux_coop_read(fd, buf, count);
}

__attribute__((weak)) ssize_t write(số_nguyên fd, const void *buf, size_t count)
{
    trả_về freertos_linux_coop_write(fd, buf, count);
}

__attribute__((weak)) ssize_t pread(số_nguyên fd, void *buf, size_t count, off_t offset)
{
    trả_về freertos_linux_coop_pread(fd, buf, count, offset);
}

__attribute__((weak)) ssize_t pwrite(số_nguyên fd, const void *buf, size_t count, off_t offset)
{
    trả_về freertos_linux_coop_pwrite(fd, buf, count, offset);
}

__attribute__((weak)) số_nguyên open(const char *path, số_nguyên flags, ...)
{
    số_nguyên mode = 0;
    nếu (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        mode = va_arg(ap, số_nguyên);
        va_end(ap);
    }
    trả_về freertos_linux_coop_open(path, flags, mode);
}

__attribute__((weak)) số_nguyên close(số_nguyên fd)
{
    trả_về freertos_linux_coop_close(fd);
}

__attribute__((weak)) số_nguyên fcntl(số_nguyên fd, số_nguyên cmd, ...)
{
    số_nguyên arg = 0;
    nếu (cmd != F_GETFL) {
        va_list list;
        va_start(list, cmd);
        arg = va_arg(list, số_nguyên);
        va_end(list);
    }
    trả_về freertos_linux_coop_fcntl(fd, cmd, arg);
}

__attribute__((weak)) số_nguyên select(số_nguyên nfds, fd_set *readfds, fd_set *writefds,
                                 fd_set *exceptfds, struct timeval *timeout)
{
    trả_về freertos_linux_coop_select(nfds, readfds, writefds, exceptfds, timeout);
}

ssize_t freertos_linux_coop_readv(số_nguyên fd, const struct iovec *iov, số_nguyên iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_readv(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_writev(số_nguyên fd, const struct iovec *iov, số_nguyên iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_writev(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_recv(số_nguyên sockfd, void *buf, size_t len, số_nguyên flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recv(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_send(số_nguyên sockfd, const void *buf, size_t len, số_nguyên flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_send(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_recvfrom(số_nguyên sockfd, void *buf, size_t len, số_nguyên flags,
                 struct sockaddr *src_addr, socklen_t *addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvfrom(sockfd, buf, len, flags | MSG_DONTWAIT,
                            src_addr, addrlen))
}

ssize_t freertos_linux_coop_sendto(số_nguyên sockfd, const void *buf, size_t len, số_nguyên flags,
               const struct sockaddr *dest_addr, socklen_t addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendto(sockfd, buf, len, flags | MSG_DONTWAIT,
                          dest_addr, addrlen))
}

ssize_t freertos_linux_coop_recvmsg(số_nguyên sockfd, struct msghdr *msg, số_nguyên flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvmsg(sockfd, msg, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_sendmsg(số_nguyên sockfd, const struct msghdr *msg, số_nguyên flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendmsg(sockfd, msg, flags | MSG_DONTWAIT))
}

__attribute__((weak)) ssize_t readv(số_nguyên fd, const struct iovec *iov, số_nguyên iovcnt)
{
    trả_về freertos_linux_coop_readv(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t writev(số_nguyên fd, const struct iovec *iov, số_nguyên iovcnt)
{
    trả_về freertos_linux_coop_writev(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t recv(số_nguyên sockfd, void *buf, size_t len, số_nguyên flags)
{
    trả_về freertos_linux_coop_recv(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t send(số_nguyên sockfd, const void *buf, size_t len, số_nguyên flags)
{
    trả_về freertos_linux_coop_send(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t recvfrom(số_nguyên sockfd, void *buf, size_t len, số_nguyên flags,
                 struct sockaddr *src_addr, socklen_t *addrlen)
{
    trả_về freertos_linux_coop_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

__attribute__((weak)) ssize_t sendto(số_nguyên sockfd, const void *buf, size_t len, số_nguyên flags,
               const struct sockaddr *dest_addr, socklen_t addrlen)
{
    trả_về freertos_linux_coop_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

__attribute__((weak)) ssize_t recvmsg(số_nguyên sockfd, struct msghdr *msg, số_nguyên flags)
{
    trả_về freertos_linux_coop_recvmsg(sockfd, msg, flags);
}

__attribute__((weak)) ssize_t sendmsg(số_nguyên sockfd, const struct msghdr *msg, số_nguyên flags)
{
    trả_về freertos_linux_coop_sendmsg(sockfd, msg, flags);
}

số_nguyên freertos_linux_coop_connect(số_nguyên sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    trong_khi (1)
    {
        số_nguyên ret = real_connect(sockfd, addr, addrlen);
        nếu (ret == 0) {
            trả_về ret;
        }

        nếu (errno == EINPROGRESS || errno == EALREADY) {
            struct pollfd pfd;
            pfd.fd = sockfd;
            pfd.events = POLLOUT;
            pfd.revents = 0;

            số_nguyên poll_retries = 0;
            trong_khi (1)
            {
                số_nguyên press = real_poll(&pfd, 1, 0);
                nếu (press > 0) {
                    số_nguyên so_err = 0;
                    socklen_t len = sizeof(so_err);
                    nếu (getsockopt(sockfd, SOL_SOCKET, SO_ERROR,
                                   &so_err, &len) < 0) {
                        trả_về -1;
                    }
                    nếu (so_err == 0) {
                        trả_về 0;
                    } không_thì {
                        errno = so_err;
                        trả_về -1;
                    }
                } không_thì nếu (press == 0) {
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    continue;
                } không_thì {
                    nếu (errno == EINTR) {
                        continue;
                    }
                    /* Non-EINTR poll error; bail out after a few retries */
                    nếu (++poll_retries > 3) {
                        trả_về -1;
                    }
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    continue;
                }
            }
        }

        nếu (errno == EINTR) {
            trả_về -1;
        }

        trả_về -1;
    }
}

số_nguyên freertos_linux_coop_accept(số_nguyên sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    trong_khi (1)
    {
        số_nguyên fd = real_accept(sockfd, addr, addrlen);
        nếu (fd >= 0) {
            linux_coop_set_nonblocking(fd);
            linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(sockfd) ? O_NONBLOCK : 0);
            trả_về fd;
        }
        nếu (errno == EAGAIN || errno == EWOULDBLOCK) {
            nếu (linux_coop_fd_user_nonblocking(sockfd)) {
                trả_về -1;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            continue;
        }
        trả_về -1;
    }
}

số_nguyên freertos_linux_coop_pselect(số_nguyên nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, const struct timespec *timeout,
            const sigset_t *sigmask)
{
    long timeout_ms = -1;
    nếu (timeout != NULL) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_nsec + 999999) / 1000000;
        nếu (timeout_ms == 0) {
            trả_về real_pselect(nfds, readfds, writefds, exceptfds,
                                timeout, sigmask);
        }
    }

    long waited_ms = 0;
    trong_khi (1)
    {
        struct timespec zero_ts = {0, 0};
        số_nguyên ret = real_pselect(nfds, readfds, writefds, exceptfds,
                               &zero_ts, sigmask);
        nếu (ret != 0) {
            trả_về ret;
        }

        nếu (timeout_ms == 0 ||
            (timeout_ms > 0 && waited_ms >= timeout_ms)) {
            trả_về 0;
        }

        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

số_nguyên freertos_linux_coop_poll(struct pollfd *fds, nfds_t nfds, số_nguyên timeout)
{
    nếu (timeout == 0) {
        trả_về real_poll(fds, nfds, 0);
    }

    long timeout_ms = -1;
    nếu (timeout > 0) {
        timeout_ms = timeout;
    }

    long waited_ms = 0;

    trong_khi (1)
    {
        số_nguyên ret = real_poll(fds, nfds, 0);
        nếu (ret > 0) {
            trả_về ret;
        } không_thì nếu (ret == 0) {
            nếu (timeout_ms == 0 ||
                (timeout_ms > 0 && waited_ms >= timeout_ms)) {
                trả_về 0;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            waited_ms += LINUX_COOP_TICK_MS;
            continue;
        } không_thì {
            nếu (errno == EINTR) {
                continue;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            nếu (timeout_ms > 0 && waited_ms >= timeout_ms) {
                trả_về -1;
            }
            waited_ms += LINUX_COOP_TICK_MS;
        }
    }
}

số_nguyên nanosleep(const struct timespec *req, struct timespec *rem)
{
    nếu (req == NULL) {
        errno = EFAULT;
        trả_về -1;
    }
    nếu (!linux_port_in_freertos_task()) {
        trả_về real_nanosleep(req, rem);
    }
    long ms = req->tv_sec * 1000 + (req->tv_nsec + 999999) / 1000000;
    nếu (ms <= 0) {
        ms = 1;
    }
    vTaskDelay(ms);
    nếu (rem) {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }
    trả_về 0;
}

unsigned số_nguyên sleep(unsigned số_nguyên seconds)
{
    linux_coop_yield(seconds * 1000);
    trả_về 0;
}

số_nguyên usleep(useconds_t usec)
{
    linux_coop_yield(usec / 1000);
    trả_về 0;
}

số_nguyên freertos_linux_coop_socket(số_nguyên domain, số_nguyên type, số_nguyên protocol)
{
    số_nguyên fd = real_socket(domain, type, protocol);
    linux_coop_set_nonblocking(fd);
#ifdef SOCK_NONBLOCK
    const số_nguyên user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#không_thì
    const số_nguyên user_flags = 0;
#endif
    linux_coop_track_fd(fd, user_flags);
    trả_về fd;
}

số_nguyên freertos_linux_coop_socketpair(số_nguyên domain, số_nguyên type, số_nguyên protocol, số_nguyên *sv)
{
    số_nguyên ret = real_socketpair(domain, type, protocol, sv);
    nếu (ret == 0) {
        linux_coop_set_nonblocking(sv[0]);
        linux_coop_set_nonblocking(sv[1]);
#ifdef SOCK_NONBLOCK
        const số_nguyên user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#không_thì
        const số_nguyên user_flags = 0;
#endif
        linux_coop_track_fd(sv[0], user_flags);
        linux_coop_track_fd(sv[1], user_flags);
    }
    trả_về ret;
}

số_nguyên freertos_linux_coop_pipe(số_nguyên *fds)
{
    số_nguyên ret = real_pipe(fds);
    nếu (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        linux_coop_track_fd(fds[0], 0);
        linux_coop_track_fd(fds[1], 0);
    }
    trả_về ret;
}

số_nguyên freertos_linux_coop_pipe2(số_nguyên *fds, số_nguyên flags)
{
    số_nguyên ret = real_pipe2(fds, flags);
    nếu (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        const số_nguyên user_flags = (flags & O_NONBLOCK) ? O_NONBLOCK : 0;
        linux_coop_track_fd(fds[0], user_flags);
        linux_coop_track_fd(fds[1], user_flags);
    }
    trả_về ret;
}

số_nguyên freertos_linux_coop_dup(số_nguyên oldfd)
{
    số_nguyên fd = real_dup(oldfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    trả_về fd;
}

số_nguyên freertos_linux_coop_dup2(số_nguyên oldfd, số_nguyên newfd)
{
    số_nguyên fd = real_dup2(oldfd, newfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    trả_về fd;
}

__attribute__((weak)) số_nguyên connect(số_nguyên sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    trả_về freertos_linux_coop_connect(sockfd, addr, addrlen);
}

__attribute__((weak)) số_nguyên accept(số_nguyên sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    trả_về freertos_linux_coop_accept(sockfd, addr, addrlen);
}

__attribute__((weak)) số_nguyên pselect(số_nguyên nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, const struct timespec *timeout,
            const sigset_t *sigmask)
{
    trả_về freertos_linux_coop_pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}

__attribute__((weak)) số_nguyên poll(struct pollfd *fds, nfds_t nfds, số_nguyên timeout)
{
    trả_về freertos_linux_coop_poll(fds, nfds, timeout);
}

__attribute__((weak)) số_nguyên socket(số_nguyên domain, số_nguyên type, số_nguyên protocol)
{
    trả_về freertos_linux_coop_socket(domain, type, protocol);
}

__attribute__((weak)) số_nguyên socketpair(số_nguyên domain, số_nguyên type, số_nguyên protocol, số_nguyên sv[2])
{
    trả_về freertos_linux_coop_socketpair(domain, type, protocol, sv);
}

__attribute__((weak)) số_nguyên pipe(số_nguyên fds[2])
{
    trả_về freertos_linux_coop_pipe(fds);
}

__attribute__((weak)) số_nguyên pipe2(số_nguyên fds[2], số_nguyên flags)
{
    trả_về freertos_linux_coop_pipe2(fds, flags);
}

__attribute__((weak)) số_nguyên dup(số_nguyên oldfd)
{
    trả_về freertos_linux_coop_dup(oldfd);
}

__attribute__((weak)) số_nguyên dup2(số_nguyên oldfd, số_nguyên newfd)
{
    trả_về freertos_linux_coop_dup2(oldfd, newfd);
}
