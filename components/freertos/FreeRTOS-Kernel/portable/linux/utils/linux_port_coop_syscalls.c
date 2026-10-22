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

static ssize_t  (*real_readv)(ціле, const struct iovec *, ціле);
static ssize_t  (*real_writev)(ціле, const struct iovec *, ціле);
static ssize_t  (*real_recv)(ціле, void *, size_t, ціле);
static ssize_t  (*real_send)(ціле, const void *, size_t, ціле);
static ssize_t  (*real_recvfrom)(ціле, void *, size_t, ціле,
                                 struct sockaddr *, socklen_t *);
static ssize_t  (*real_sendto)(ціле, const void *, size_t, ціле,
                                const struct sockaddr *, socklen_t);
static ssize_t  (*real_recvmsg)(ціле, struct msghdr *, ціле);
static ssize_t  (*real_sendmsg)(ціле, const struct msghdr *, ціле);
static ціле      (*real_connect)(ціле, const struct sockaddr *, socklen_t);
static ціле      (*real_accept)(ціле, struct sockaddr *, socklen_t *);
static ціле      (*real_pselect)(ціле, fd_set *, fd_set *, fd_set *,
                                const struct timespec *, const sigset_t *);
static ціле      (*real_poll)(struct pollfd *, nfds_t, ціле);
static ціле      (*real_socket)(ціле, ціле, ціле);
static ціле      (*real_socketpair)(ціле, ціле, ціле, ціле[2]);
static ціле      (*real_pipe)(ціле[2]);
static ціле      (*real_pipe2)(ціле[2], ціле);
static ціле      (*real_dup)(ціле);
static ціле      (*real_dup2)(ціле, ціле);
static ціле      (*real_nanosleep)(const struct timespec *, struct timespec *);
static ssize_t  (*real_read)(ціле, void *, size_t);
static ssize_t  (*real_write)(ціле, const void *, size_t);
static ssize_t  (*real_pread)(ціле, void *, size_t, off_t);
static ssize_t  (*real_pwrite)(ціле, const void *, size_t, off_t);
static ціле      (*real_close)(ціле);
static ціле      (*real_select)(ціле, fd_set *, fd_set *, fd_set *,
                                struct timeval *);

typedef ціле (*linux_coop_fcntl_fn_t)(ціле, ціле, ...);
static linux_coop_fcntl_fn_t real_fcntl;

typedef ціле (*linux_coop_open_fn_t)(const char *, ціле, mode_t);
static linux_coop_open_fn_t real_open;

typedef struct linux_coop_fd_state {
    ціле fd;
    ціле user_flags;
    struct linux_coop_fd_state *next;
} linux_coop_fd_state_t;

static linux_coop_fd_state_t *s_fd_state_list;
static pthread_mutex_t s_fd_state_lock = PTHREAD_MUTEX_INITIALIZER;

static linux_coop_fd_state_t *linux_coop_find_fd_locked(ціле fd)
{
    linux_coop_fd_state_t *node = s_fd_state_list;
    поки (node != NULL) {
        якщо (node->fd == fd) {
            повернути node;
        }
        node = node->next;
    }
    повернути NULL;
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

void linux_coop_set_nonblocking(ціле fd)
{
    якщо (fd >= 0) {
        ціле flags = real_fcntl(fd, F_GETFL, 0);
        якщо (flags >= 0) {
            real_fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
    }
}

void linux_coop_track_fd(ціле fd, ціле user_flags)
{
    якщо (fd < 0) {
        повернути;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *node = linux_coop_find_fd_locked(fd);
    якщо (node != NULL) {
        node->user_flags = user_flags;
        pthread_mutex_unlock(&s_fd_state_lock);
        повернути;
    }

    node = calloc(1, sizeof(*node));
    якщо (node != NULL) {
        node->fd = fd;
        node->user_flags = user_flags;
        node->next = s_fd_state_list;
        s_fd_state_list = node;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

void linux_coop_untrack_fd(ціле fd)
{
    якщо (fd < 0) {
        повернути;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *prev = NULL;
    linux_coop_fd_state_t *node = s_fd_state_list;
    поки (node != NULL) {
        якщо (node->fd == fd) {
            якщо (prev == NULL) {
                s_fd_state_list = node->next;
            } інакше {
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

void linux_coop_set_user_flags(ціле fd, ціле user_flags)
{
    linux_coop_track_fd(fd, user_flags);
}

bool linux_coop_get_user_flags(ціле fd, ціле *flags)
{
    якщо (fd < 0 || flags == NULL) {
        повернути false;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *node = linux_coop_find_fd_locked(fd);
    якщо (node == NULL) {
        pthread_mutex_unlock(&s_fd_state_lock);
        повернути false;
    }
    *flags = node->user_flags;
    pthread_mutex_unlock(&s_fd_state_lock);
    повернути true;
}

bool linux_coop_fd_user_nonblocking(ціле fd)
{
    ціле user_flags = 0;
    якщо (linux_coop_get_user_flags(fd, &user_flags)) {
        повернути (user_flags & O_NONBLOCK) != 0;
    }
    повернути false;
}


ssize_t freertos_linux_coop_read(ціле fd, void *buf, size_t count)
{
    LINUX_COOP_IO_LOOP(fd, real_read(fd, buf, count))
}

ssize_t freertos_linux_coop_write(ціле fd, const void *buf, size_t count)
{
    LINUX_COOP_IO_LOOP(fd, real_write(fd, buf, count))
}

ssize_t freertos_linux_coop_pread(ціле fd, void *buf, size_t count, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pread(fd, buf, count, offset))
}

ssize_t freertos_linux_coop_pwrite(ціле fd, const void *buf, size_t count, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pwrite(fd, buf, count, offset))
}

ціле freertos_linux_coop_open(const char *path, ціле flags, ціле mode)
{
    ціле fd = real_open(path, flags, (mode_t)mode);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, flags & O_NONBLOCK);
    повернути fd;
}

ціле freertos_linux_coop_close(ціле fd)
{
    ціле ret = real_close(fd);
    якщо (ret == 0 || errno == EINTR) {
        /* On Linux, close() always releases the fd even when returning
         * EINTR.  Retrying would risk closing a fd reused by another
         * thread.  Treat EINTR as success. */
        linux_coop_untrack_fd(fd);
        повернути 0;
    }
    повернути -1;
}

ціле freertos_linux_coop_fcntl(ціле fd, ціле cmd, ціле arg)
{
    якщо (cmd == F_GETFL) {
        ціле real_flags = real_fcntl(fd, F_GETFL, 0);
        якщо (real_flags < 0) {
            повернути -1;
        }
        ціле user_flags = 0;
        якщо (linux_coop_get_user_flags(fd, &user_flags)) {
            real_flags &= ~O_NONBLOCK;
            real_flags |= (user_flags & O_NONBLOCK);
        }
        повернути real_flags;
    }
    якщо (cmd == F_SETFL) {
        linux_coop_set_user_flags(fd, arg);
        ціле internal_flags = arg | O_NONBLOCK;
        повернути real_fcntl(fd, F_SETFL, internal_flags);
    }
    повернути real_fcntl(fd, cmd, arg);
}

ціле freertos_linux_coop_select(ціле nfds, fd_set *readfds, fd_set *writefds,
                      fd_set *exceptfds, struct timeval *timeout)
{
    long timeout_ms = -1;
    якщо (timeout != NULL) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_usec + 999) / 1000;
        якщо (timeout_ms == 0) {
            повернути real_select(nfds, readfds, writefds, exceptfds, timeout);
        }
    }

    /* real_select() mutates the fd_sets in place (clears them, then sets bits
     * for ready descriptors). To keep polling the same descriptors across
     * iterations we must snapshot the caller's sets and restore them on every
     * loop. */
    fd_set rfds_in, wfds_in, efds_in;
    якщо (readfds != NULL) {
        rfds_in = *readfds;
    }
    якщо (writefds != NULL) {
        wfds_in = *writefds;
    }
    якщо (exceptfds != NULL) {
        efds_in = *exceptfds;
    }

    long waited_ms = 0;
    поки (1) {
        якщо (readfds != NULL) {
            *readfds = rfds_in;
        }
        якщо (writefds != NULL) {
            *writefds = wfds_in;
        }
        якщо (exceptfds != NULL) {
            *exceptfds = efds_in;
        }

        struct timeval zero_tv = {0, 0};
        ціле ret = real_select(nfds, readfds, writefds, exceptfds, &zero_tv);
        якщо (ret != 0) {
            повернути ret;
        }
        якщо (timeout_ms == 0) {
            повернути 0;
        }
        якщо (timeout_ms > 0 && waited_ms >= timeout_ms) {
            повернути 0;
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

__attribute__((weak)) ssize_t read(ціле fd, void *buf, size_t count)
{
    повернути freertos_linux_coop_read(fd, buf, count);
}

__attribute__((weak)) ssize_t write(ціле fd, const void *buf, size_t count)
{
    повернути freertos_linux_coop_write(fd, buf, count);
}

__attribute__((weak)) ssize_t pread(ціле fd, void *buf, size_t count, off_t offset)
{
    повернути freertos_linux_coop_pread(fd, buf, count, offset);
}

__attribute__((weak)) ssize_t pwrite(ціле fd, const void *buf, size_t count, off_t offset)
{
    повернути freertos_linux_coop_pwrite(fd, buf, count, offset);
}

__attribute__((weak)) ціле open(const char *path, ціле flags, ...)
{
    ціле mode = 0;
    якщо (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        mode = va_arg(ap, ціле);
        va_end(ap);
    }
    повернути freertos_linux_coop_open(path, flags, mode);
}

__attribute__((weak)) ціле close(ціле fd)
{
    повернути freertos_linux_coop_close(fd);
}

__attribute__((weak)) ціле fcntl(ціле fd, ціле cmd, ...)
{
    ціле arg = 0;
    якщо (cmd != F_GETFL) {
        va_list list;
        va_start(list, cmd);
        arg = va_arg(list, ціле);
        va_end(list);
    }
    повернути freertos_linux_coop_fcntl(fd, cmd, arg);
}

__attribute__((weak)) ціле select(ціле nfds, fd_set *readfds, fd_set *writefds,
                                 fd_set *exceptfds, struct timeval *timeout)
{
    повернути freertos_linux_coop_select(nfds, readfds, writefds, exceptfds, timeout);
}

ssize_t freertos_linux_coop_readv(ціле fd, const struct iovec *iov, ціле iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_readv(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_writev(ціле fd, const struct iovec *iov, ціле iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_writev(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_recv(ціле sockfd, void *buf, size_t len, ціле flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recv(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_send(ціле sockfd, const void *buf, size_t len, ціле flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_send(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_recvfrom(ціле sockfd, void *buf, size_t len, ціле flags,
                 struct sockaddr *src_addr, socklen_t *addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvfrom(sockfd, buf, len, flags | MSG_DONTWAIT,
                            src_addr, addrlen))
}

ssize_t freertos_linux_coop_sendto(ціле sockfd, const void *buf, size_t len, ціле flags,
               const struct sockaddr *dest_addr, socklen_t addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendto(sockfd, buf, len, flags | MSG_DONTWAIT,
                          dest_addr, addrlen))
}

ssize_t freertos_linux_coop_recvmsg(ціле sockfd, struct msghdr *msg, ціле flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvmsg(sockfd, msg, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_sendmsg(ціле sockfd, const struct msghdr *msg, ціле flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendmsg(sockfd, msg, flags | MSG_DONTWAIT))
}

__attribute__((weak)) ssize_t readv(ціле fd, const struct iovec *iov, ціле iovcnt)
{
    повернути freertos_linux_coop_readv(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t writev(ціле fd, const struct iovec *iov, ціле iovcnt)
{
    повернути freertos_linux_coop_writev(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t recv(ціле sockfd, void *buf, size_t len, ціле flags)
{
    повернути freertos_linux_coop_recv(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t send(ціле sockfd, const void *buf, size_t len, ціле flags)
{
    повернути freertos_linux_coop_send(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t recvfrom(ціле sockfd, void *buf, size_t len, ціле flags,
                 struct sockaddr *src_addr, socklen_t *addrlen)
{
    повернути freertos_linux_coop_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

__attribute__((weak)) ssize_t sendto(ціле sockfd, const void *buf, size_t len, ціле flags,
               const struct sockaddr *dest_addr, socklen_t addrlen)
{
    повернути freertos_linux_coop_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

__attribute__((weak)) ssize_t recvmsg(ціле sockfd, struct msghdr *msg, ціле flags)
{
    повернути freertos_linux_coop_recvmsg(sockfd, msg, flags);
}

__attribute__((weak)) ssize_t sendmsg(ціле sockfd, const struct msghdr *msg, ціле flags)
{
    повернути freertos_linux_coop_sendmsg(sockfd, msg, flags);
}

ціле freertos_linux_coop_connect(ціле sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    поки (1)
    {
        ціле ret = real_connect(sockfd, addr, addrlen);
        якщо (ret == 0) {
            повернути ret;
        }

        якщо (errno == EINPROGRESS || errno == EALREADY) {
            struct pollfd pfd;
            pfd.fd = sockfd;
            pfd.events = POLLOUT;
            pfd.revents = 0;

            ціле poll_retries = 0;
            поки (1)
            {
                ціле press = real_poll(&pfd, 1, 0);
                якщо (press > 0) {
                    ціле so_err = 0;
                    socklen_t len = sizeof(so_err);
                    якщо (getsockopt(sockfd, SOL_SOCKET, SO_ERROR,
                                   &so_err, &len) < 0) {
                        повернути -1;
                    }
                    якщо (so_err == 0) {
                        повернути 0;
                    } інакше {
                        errno = so_err;
                        повернути -1;
                    }
                } інакше якщо (press == 0) {
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    continue;
                } інакше {
                    якщо (errno == EINTR) {
                        continue;
                    }
                    /* Non-EINTR poll error; bail out after a few retries */
                    якщо (++poll_retries > 3) {
                        повернути -1;
                    }
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    continue;
                }
            }
        }

        якщо (errno == EINTR) {
            повернути -1;
        }

        повернути -1;
    }
}

ціле freertos_linux_coop_accept(ціле sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    поки (1)
    {
        ціле fd = real_accept(sockfd, addr, addrlen);
        якщо (fd >= 0) {
            linux_coop_set_nonblocking(fd);
            linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(sockfd) ? O_NONBLOCK : 0);
            повернути fd;
        }
        якщо (errno == EAGAIN || errno == EWOULDBLOCK) {
            якщо (linux_coop_fd_user_nonblocking(sockfd)) {
                повернути -1;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            continue;
        }
        повернути -1;
    }
}

ціле freertos_linux_coop_pselect(ціле nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, const struct timespec *timeout,
            const sigset_t *sigmask)
{
    long timeout_ms = -1;
    якщо (timeout != NULL) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_nsec + 999999) / 1000000;
        якщо (timeout_ms == 0) {
            повернути real_pselect(nfds, readfds, writefds, exceptfds,
                                timeout, sigmask);
        }
    }

    long waited_ms = 0;
    поки (1)
    {
        struct timespec zero_ts = {0, 0};
        ціле ret = real_pselect(nfds, readfds, writefds, exceptfds,
                               &zero_ts, sigmask);
        якщо (ret != 0) {
            повернути ret;
        }

        якщо (timeout_ms == 0 ||
            (timeout_ms > 0 && waited_ms >= timeout_ms)) {
            повернути 0;
        }

        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

ціле freertos_linux_coop_poll(struct pollfd *fds, nfds_t nfds, ціле timeout)
{
    якщо (timeout == 0) {
        повернути real_poll(fds, nfds, 0);
    }

    long timeout_ms = -1;
    якщо (timeout > 0) {
        timeout_ms = timeout;
    }

    long waited_ms = 0;

    поки (1)
    {
        ціле ret = real_poll(fds, nfds, 0);
        якщо (ret > 0) {
            повернути ret;
        } інакше якщо (ret == 0) {
            якщо (timeout_ms == 0 ||
                (timeout_ms > 0 && waited_ms >= timeout_ms)) {
                повернути 0;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            waited_ms += LINUX_COOP_TICK_MS;
            continue;
        } інакше {
            якщо (errno == EINTR) {
                continue;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            якщо (timeout_ms > 0 && waited_ms >= timeout_ms) {
                повернути -1;
            }
            waited_ms += LINUX_COOP_TICK_MS;
        }
    }
}

ціле nanosleep(const struct timespec *req, struct timespec *rem)
{
    якщо (req == NULL) {
        errno = EFAULT;
        повернути -1;
    }
    якщо (!linux_port_in_freertos_task()) {
        повернути real_nanosleep(req, rem);
    }
    long ms = req->tv_sec * 1000 + (req->tv_nsec + 999999) / 1000000;
    якщо (ms <= 0) {
        ms = 1;
    }
    vTaskDelay(ms);
    якщо (rem) {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }
    повернути 0;
}

unsigned ціле sleep(unsigned ціле seconds)
{
    linux_coop_yield(seconds * 1000);
    повернути 0;
}

ціле usleep(useconds_t usec)
{
    linux_coop_yield(usec / 1000);
    повернути 0;
}

ціле freertos_linux_coop_socket(ціле domain, ціле type, ціле protocol)
{
    ціле fd = real_socket(domain, type, protocol);
    linux_coop_set_nonblocking(fd);
#ifdef SOCK_NONBLOCK
    const ціле user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#інакше
    const ціле user_flags = 0;
#endif
    linux_coop_track_fd(fd, user_flags);
    повернути fd;
}

ціле freertos_linux_coop_socketpair(ціле domain, ціле type, ціле protocol, ціле *sv)
{
    ціле ret = real_socketpair(domain, type, protocol, sv);
    якщо (ret == 0) {
        linux_coop_set_nonblocking(sv[0]);
        linux_coop_set_nonblocking(sv[1]);
#ifdef SOCK_NONBLOCK
        const ціле user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#інакше
        const ціле user_flags = 0;
#endif
        linux_coop_track_fd(sv[0], user_flags);
        linux_coop_track_fd(sv[1], user_flags);
    }
    повернути ret;
}

ціле freertos_linux_coop_pipe(ціле *fds)
{
    ціле ret = real_pipe(fds);
    якщо (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        linux_coop_track_fd(fds[0], 0);
        linux_coop_track_fd(fds[1], 0);
    }
    повернути ret;
}

ціле freertos_linux_coop_pipe2(ціле *fds, ціле flags)
{
    ціле ret = real_pipe2(fds, flags);
    якщо (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        const ціле user_flags = (flags & O_NONBLOCK) ? O_NONBLOCK : 0;
        linux_coop_track_fd(fds[0], user_flags);
        linux_coop_track_fd(fds[1], user_flags);
    }
    повернути ret;
}

ціле freertos_linux_coop_dup(ціле oldfd)
{
    ціле fd = real_dup(oldfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    повернути fd;
}

ціле freertos_linux_coop_dup2(ціле oldfd, ціле newfd)
{
    ціле fd = real_dup2(oldfd, newfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    повернути fd;
}

__attribute__((weak)) ціле connect(ціле sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    повернути freertos_linux_coop_connect(sockfd, addr, addrlen);
}

__attribute__((weak)) ціле accept(ціле sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    повернути freertos_linux_coop_accept(sockfd, addr, addrlen);
}

__attribute__((weak)) ціле pselect(ціле nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, const struct timespec *timeout,
            const sigset_t *sigmask)
{
    повернути freertos_linux_coop_pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}

__attribute__((weak)) ціле poll(struct pollfd *fds, nfds_t nfds, ціле timeout)
{
    повернути freertos_linux_coop_poll(fds, nfds, timeout);
}

__attribute__((weak)) ціле socket(ціле domain, ціле type, ціле protocol)
{
    повернути freertos_linux_coop_socket(domain, type, protocol);
}

__attribute__((weak)) ціле socketpair(ціле domain, ціле type, ціле protocol, ціле sv[2])
{
    повернути freertos_linux_coop_socketpair(domain, type, protocol, sv);
}

__attribute__((weak)) ціле pipe(ціле fds[2])
{
    повернути freertos_linux_coop_pipe(fds);
}

__attribute__((weak)) ціле pipe2(ціле fds[2], ціле flags)
{
    повернути freertos_linux_coop_pipe2(fds, flags);
}

__attribute__((weak)) ціле dup(ціле oldfd)
{
    повернути freertos_linux_coop_dup(oldfd);
}

__attribute__((weak)) ціле dup2(ціле oldfd, ціле newfd)
{
    повернути freertos_linux_coop_dup2(oldfd, newfd);
}
