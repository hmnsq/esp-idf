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

static ssize_t  (*real_readv)(цяло, const struct iovec *, цяло);
static ssize_t  (*real_writev)(цяло, const struct iovec *, цяло);
static ssize_t  (*real_recv)(цяло, void *, size_t, цяло);
static ssize_t  (*real_send)(цяло, const void *, size_t, цяло);
static ssize_t  (*real_recvfrom)(цяло, void *, size_t, цяло,
                                 struct sockaddr *, socklen_t *);
static ssize_t  (*real_sendto)(цяло, const void *, size_t, цяло,
                                const struct sockaddr *, socklen_t);
static ssize_t  (*real_recvmsg)(цяло, struct msghdr *, цяло);
static ssize_t  (*real_sendmsg)(цяло, const struct msghdr *, цяло);
static цяло      (*real_connect)(цяло, const struct sockaddr *, socklen_t);
static цяло      (*real_accept)(цяло, struct sockaddr *, socklen_t *);
static цяло      (*real_pselect)(цяло, fd_set *, fd_set *, fd_set *,
                                const struct timespec *, const sigset_t *);
static цяло      (*real_poll)(struct pollfd *, nfds_t, цяло);
static цяло      (*real_socket)(цяло, цяло, цяло);
static цяло      (*real_socketpair)(цяло, цяло, цяло, цяло[2]);
static цяло      (*real_pipe)(цяло[2]);
static цяло      (*real_pipe2)(цяло[2], цяло);
static цяло      (*real_dup)(цяло);
static цяло      (*real_dup2)(цяло, цяло);
static цяло      (*real_nanosleep)(const struct timespec *, struct timespec *);
static ssize_t  (*real_read)(цяло, void *, size_t);
static ssize_t  (*real_write)(цяло, const void *, size_t);
static ssize_t  (*real_pread)(цяло, void *, size_t, off_t);
static ssize_t  (*real_pwrite)(цяло, const void *, size_t, off_t);
static цяло      (*real_close)(цяло);
static цяло      (*real_select)(цяло, fd_set *, fd_set *, fd_set *,
                                struct timeval *);

typedef цяло (*linux_coop_fcntl_fn_t)(цяло, цяло, ...);
static linux_coop_fcntl_fn_t real_fcntl;

typedef цяло (*linux_coop_open_fn_t)(const char *, цяло, mode_t);
static linux_coop_open_fn_t real_open;

typedef struct linux_coop_fd_state {
    цяло fd;
    цяло user_flags;
    struct linux_coop_fd_state *next;
} linux_coop_fd_state_t;

static linux_coop_fd_state_t *s_fd_state_list;
static pthread_mutex_t s_fd_state_lock = PTHREAD_MUTEX_INITIALIZER;

static linux_coop_fd_state_t *linux_coop_find_fd_locked(цяло fd)
{
    linux_coop_fd_state_t *node = s_fd_state_list;
    докато (node != NULL) {
        ако (node->fd == fd) {
            върни node;
        }
        node = node->next;
    }
    върни NULL;
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

void linux_coop_set_nonblocking(цяло fd)
{
    ако (fd >= 0) {
        цяло flags = real_fcntl(fd, F_GETFL, 0);
        ако (flags >= 0) {
            real_fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
    }
}

void linux_coop_track_fd(цяло fd, цяло user_flags)
{
    ако (fd < 0) {
        върни;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *node = linux_coop_find_fd_locked(fd);
    ако (node != NULL) {
        node->user_flags = user_flags;
        pthread_mutex_unlock(&s_fd_state_lock);
        върни;
    }

    node = calloc(1, sizeof(*node));
    ако (node != NULL) {
        node->fd = fd;
        node->user_flags = user_flags;
        node->next = s_fd_state_list;
        s_fd_state_list = node;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

void linux_coop_untrack_fd(цяло fd)
{
    ако (fd < 0) {
        върни;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *prev = NULL;
    linux_coop_fd_state_t *node = s_fd_state_list;
    докато (node != NULL) {
        ако (node->fd == fd) {
            ако (prev == NULL) {
                s_fd_state_list = node->next;
            } иначе {
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

void linux_coop_set_user_flags(цяло fd, цяло user_flags)
{
    linux_coop_track_fd(fd, user_flags);
}

bool linux_coop_get_user_flags(цяло fd, цяло *flags)
{
    ако (fd < 0 || flags == NULL) {
        върни false;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *node = linux_coop_find_fd_locked(fd);
    ако (node == NULL) {
        pthread_mutex_unlock(&s_fd_state_lock);
        върни false;
    }
    *flags = node->user_flags;
    pthread_mutex_unlock(&s_fd_state_lock);
    върни true;
}

bool linux_coop_fd_user_nonblocking(цяло fd)
{
    цяло user_flags = 0;
    ако (linux_coop_get_user_flags(fd, &user_flags)) {
        върни (user_flags & O_NONBLOCK) != 0;
    }
    върни false;
}


ssize_t freertos_linux_coop_read(цяло fd, void *buf, size_t count)
{
    LINUX_COOP_IO_LOOP(fd, real_read(fd, buf, count))
}

ssize_t freertos_linux_coop_write(цяло fd, const void *buf, size_t count)
{
    LINUX_COOP_IO_LOOP(fd, real_write(fd, buf, count))
}

ssize_t freertos_linux_coop_pread(цяло fd, void *buf, size_t count, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pread(fd, buf, count, offset))
}

ssize_t freertos_linux_coop_pwrite(цяло fd, const void *buf, size_t count, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pwrite(fd, buf, count, offset))
}

цяло freertos_linux_coop_open(const char *path, цяло flags, цяло mode)
{
    цяло fd = real_open(path, flags, (mode_t)mode);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, flags & O_NONBLOCK);
    върни fd;
}

цяло freertos_linux_coop_close(цяло fd)
{
    цяло ret = real_close(fd);
    ако (ret == 0 || errno == EINTR) {
        /* On Linux, close() always releases the fd even when returning
         * EINTR.  Retrying would risk closing a fd reused by another
         * thread.  Treat EINTR as success. */
        linux_coop_untrack_fd(fd);
        върни 0;
    }
    върни -1;
}

цяло freertos_linux_coop_fcntl(цяло fd, цяло cmd, цяло arg)
{
    ако (cmd == F_GETFL) {
        цяло real_flags = real_fcntl(fd, F_GETFL, 0);
        ако (real_flags < 0) {
            върни -1;
        }
        цяло user_flags = 0;
        ако (linux_coop_get_user_flags(fd, &user_flags)) {
            real_flags &= ~O_NONBLOCK;
            real_flags |= (user_flags & O_NONBLOCK);
        }
        върни real_flags;
    }
    ако (cmd == F_SETFL) {
        linux_coop_set_user_flags(fd, arg);
        цяло internal_flags = arg | O_NONBLOCK;
        върни real_fcntl(fd, F_SETFL, internal_flags);
    }
    върни real_fcntl(fd, cmd, arg);
}

цяло freertos_linux_coop_select(цяло nfds, fd_set *readfds, fd_set *writefds,
                      fd_set *exceptfds, struct timeval *timeout)
{
    long timeout_ms = -1;
    ако (timeout != NULL) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_usec + 999) / 1000;
        ако (timeout_ms == 0) {
            върни real_select(nfds, readfds, writefds, exceptfds, timeout);
        }
    }

    /* real_select() mutates the fd_sets in place (clears them, then sets bits
     * for ready descriptors). To keep polling the same descriptors across
     * iterations we must snapshot the caller's sets and restore them on every
     * loop. */
    fd_set rfds_in, wfds_in, efds_in;
    ако (readfds != NULL) {
        rfds_in = *readfds;
    }
    ако (writefds != NULL) {
        wfds_in = *writefds;
    }
    ако (exceptfds != NULL) {
        efds_in = *exceptfds;
    }

    long waited_ms = 0;
    докато (1) {
        ако (readfds != NULL) {
            *readfds = rfds_in;
        }
        ако (writefds != NULL) {
            *writefds = wfds_in;
        }
        ако (exceptfds != NULL) {
            *exceptfds = efds_in;
        }

        struct timeval zero_tv = {0, 0};
        цяло ret = real_select(nfds, readfds, writefds, exceptfds, &zero_tv);
        ако (ret != 0) {
            върни ret;
        }
        ако (timeout_ms == 0) {
            върни 0;
        }
        ако (timeout_ms > 0 && waited_ms >= timeout_ms) {
            върни 0;
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

__attribute__((weak)) ssize_t read(цяло fd, void *buf, size_t count)
{
    върни freertos_linux_coop_read(fd, buf, count);
}

__attribute__((weak)) ssize_t write(цяло fd, const void *buf, size_t count)
{
    върни freertos_linux_coop_write(fd, buf, count);
}

__attribute__((weak)) ssize_t pread(цяло fd, void *buf, size_t count, off_t offset)
{
    върни freertos_linux_coop_pread(fd, buf, count, offset);
}

__attribute__((weak)) ssize_t pwrite(цяло fd, const void *buf, size_t count, off_t offset)
{
    върни freertos_linux_coop_pwrite(fd, buf, count, offset);
}

__attribute__((weak)) цяло open(const char *path, цяло flags, ...)
{
    цяло mode = 0;
    ако (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        mode = va_arg(ap, цяло);
        va_end(ap);
    }
    върни freertos_linux_coop_open(path, flags, mode);
}

__attribute__((weak)) цяло close(цяло fd)
{
    върни freertos_linux_coop_close(fd);
}

__attribute__((weak)) цяло fcntl(цяло fd, цяло cmd, ...)
{
    цяло arg = 0;
    ако (cmd != F_GETFL) {
        va_list list;
        va_start(list, cmd);
        arg = va_arg(list, цяло);
        va_end(list);
    }
    върни freertos_linux_coop_fcntl(fd, cmd, arg);
}

__attribute__((weak)) цяло select(цяло nfds, fd_set *readfds, fd_set *writefds,
                                 fd_set *exceptfds, struct timeval *timeout)
{
    върни freertos_linux_coop_select(nfds, readfds, writefds, exceptfds, timeout);
}

ssize_t freertos_linux_coop_readv(цяло fd, const struct iovec *iov, цяло iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_readv(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_writev(цяло fd, const struct iovec *iov, цяло iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_writev(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_recv(цяло sockfd, void *buf, size_t len, цяло flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recv(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_send(цяло sockfd, const void *buf, size_t len, цяло flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_send(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_recvfrom(цяло sockfd, void *buf, size_t len, цяло flags,
                 struct sockaddr *src_addr, socklen_t *addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvfrom(sockfd, buf, len, flags | MSG_DONTWAIT,
                            src_addr, addrlen))
}

ssize_t freertos_linux_coop_sendto(цяло sockfd, const void *buf, size_t len, цяло flags,
               const struct sockaddr *dest_addr, socklen_t addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendto(sockfd, buf, len, flags | MSG_DONTWAIT,
                          dest_addr, addrlen))
}

ssize_t freertos_linux_coop_recvmsg(цяло sockfd, struct msghdr *msg, цяло flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvmsg(sockfd, msg, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_sendmsg(цяло sockfd, const struct msghdr *msg, цяло flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendmsg(sockfd, msg, flags | MSG_DONTWAIT))
}

__attribute__((weak)) ssize_t readv(цяло fd, const struct iovec *iov, цяло iovcnt)
{
    върни freertos_linux_coop_readv(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t writev(цяло fd, const struct iovec *iov, цяло iovcnt)
{
    върни freertos_linux_coop_writev(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t recv(цяло sockfd, void *buf, size_t len, цяло flags)
{
    върни freertos_linux_coop_recv(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t send(цяло sockfd, const void *buf, size_t len, цяло flags)
{
    върни freertos_linux_coop_send(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t recvfrom(цяло sockfd, void *buf, size_t len, цяло flags,
                 struct sockaddr *src_addr, socklen_t *addrlen)
{
    върни freertos_linux_coop_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

__attribute__((weak)) ssize_t sendto(цяло sockfd, const void *buf, size_t len, цяло flags,
               const struct sockaddr *dest_addr, socklen_t addrlen)
{
    върни freertos_linux_coop_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

__attribute__((weak)) ssize_t recvmsg(цяло sockfd, struct msghdr *msg, цяло flags)
{
    върни freertos_linux_coop_recvmsg(sockfd, msg, flags);
}

__attribute__((weak)) ssize_t sendmsg(цяло sockfd, const struct msghdr *msg, цяло flags)
{
    върни freertos_linux_coop_sendmsg(sockfd, msg, flags);
}

цяло freertos_linux_coop_connect(цяло sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    докато (1)
    {
        цяло ret = real_connect(sockfd, addr, addrlen);
        ако (ret == 0) {
            върни ret;
        }

        ако (errno == EINPROGRESS || errno == EALREADY) {
            struct pollfd pfd;
            pfd.fd = sockfd;
            pfd.events = POLLOUT;
            pfd.revents = 0;

            цяло poll_retries = 0;
            докато (1)
            {
                цяло press = real_poll(&pfd, 1, 0);
                ако (press > 0) {
                    цяло so_err = 0;
                    socklen_t len = sizeof(so_err);
                    ако (getsockopt(sockfd, SOL_SOCKET, SO_ERROR,
                                   &so_err, &len) < 0) {
                        върни -1;
                    }
                    ако (so_err == 0) {
                        върни 0;
                    } иначе {
                        errno = so_err;
                        върни -1;
                    }
                } иначе ако (press == 0) {
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    continue;
                } иначе {
                    ако (errno == EINTR) {
                        continue;
                    }
                    /* Non-EINTR poll error; bail out after a few retries */
                    ако (++poll_retries > 3) {
                        върни -1;
                    }
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    continue;
                }
            }
        }

        ако (errno == EINTR) {
            върни -1;
        }

        върни -1;
    }
}

цяло freertos_linux_coop_accept(цяло sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    докато (1)
    {
        цяло fd = real_accept(sockfd, addr, addrlen);
        ако (fd >= 0) {
            linux_coop_set_nonblocking(fd);
            linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(sockfd) ? O_NONBLOCK : 0);
            върни fd;
        }
        ако (errno == EAGAIN || errno == EWOULDBLOCK) {
            ако (linux_coop_fd_user_nonblocking(sockfd)) {
                върни -1;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            continue;
        }
        върни -1;
    }
}

цяло freertos_linux_coop_pselect(цяло nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, const struct timespec *timeout,
            const sigset_t *sigmask)
{
    long timeout_ms = -1;
    ако (timeout != NULL) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_nsec + 999999) / 1000000;
        ако (timeout_ms == 0) {
            върни real_pselect(nfds, readfds, writefds, exceptfds,
                                timeout, sigmask);
        }
    }

    long waited_ms = 0;
    докато (1)
    {
        struct timespec zero_ts = {0, 0};
        цяло ret = real_pselect(nfds, readfds, writefds, exceptfds,
                               &zero_ts, sigmask);
        ако (ret != 0) {
            върни ret;
        }

        ако (timeout_ms == 0 ||
            (timeout_ms > 0 && waited_ms >= timeout_ms)) {
            върни 0;
        }

        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

цяло freertos_linux_coop_poll(struct pollfd *fds, nfds_t nfds, цяло timeout)
{
    ако (timeout == 0) {
        върни real_poll(fds, nfds, 0);
    }

    long timeout_ms = -1;
    ако (timeout > 0) {
        timeout_ms = timeout;
    }

    long waited_ms = 0;

    докато (1)
    {
        цяло ret = real_poll(fds, nfds, 0);
        ако (ret > 0) {
            върни ret;
        } иначе ако (ret == 0) {
            ако (timeout_ms == 0 ||
                (timeout_ms > 0 && waited_ms >= timeout_ms)) {
                върни 0;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            waited_ms += LINUX_COOP_TICK_MS;
            continue;
        } иначе {
            ако (errno == EINTR) {
                continue;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            ако (timeout_ms > 0 && waited_ms >= timeout_ms) {
                върни -1;
            }
            waited_ms += LINUX_COOP_TICK_MS;
        }
    }
}

цяло nanosleep(const struct timespec *req, struct timespec *rem)
{
    ако (req == NULL) {
        errno = EFAULT;
        върни -1;
    }
    ако (!linux_port_in_freertos_task()) {
        върни real_nanosleep(req, rem);
    }
    long ms = req->tv_sec * 1000 + (req->tv_nsec + 999999) / 1000000;
    ако (ms <= 0) {
        ms = 1;
    }
    vTaskDelay(ms);
    ако (rem) {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }
    върни 0;
}

unsigned цяло sleep(unsigned цяло seconds)
{
    linux_coop_yield(seconds * 1000);
    върни 0;
}

цяло usleep(useconds_t usec)
{
    linux_coop_yield(usec / 1000);
    върни 0;
}

цяло freertos_linux_coop_socket(цяло domain, цяло type, цяло protocol)
{
    цяло fd = real_socket(domain, type, protocol);
    linux_coop_set_nonblocking(fd);
#ifdef SOCK_NONBLOCK
    const цяло user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#иначе
    const цяло user_flags = 0;
#endif
    linux_coop_track_fd(fd, user_flags);
    върни fd;
}

цяло freertos_linux_coop_socketpair(цяло domain, цяло type, цяло protocol, цяло *sv)
{
    цяло ret = real_socketpair(domain, type, protocol, sv);
    ако (ret == 0) {
        linux_coop_set_nonblocking(sv[0]);
        linux_coop_set_nonblocking(sv[1]);
#ifdef SOCK_NONBLOCK
        const цяло user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#иначе
        const цяло user_flags = 0;
#endif
        linux_coop_track_fd(sv[0], user_flags);
        linux_coop_track_fd(sv[1], user_flags);
    }
    върни ret;
}

цяло freertos_linux_coop_pipe(цяло *fds)
{
    цяло ret = real_pipe(fds);
    ако (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        linux_coop_track_fd(fds[0], 0);
        linux_coop_track_fd(fds[1], 0);
    }
    върни ret;
}

цяло freertos_linux_coop_pipe2(цяло *fds, цяло flags)
{
    цяло ret = real_pipe2(fds, flags);
    ако (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        const цяло user_flags = (flags & O_NONBLOCK) ? O_NONBLOCK : 0;
        linux_coop_track_fd(fds[0], user_flags);
        linux_coop_track_fd(fds[1], user_flags);
    }
    върни ret;
}

цяло freertos_linux_coop_dup(цяло oldfd)
{
    цяло fd = real_dup(oldfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    върни fd;
}

цяло freertos_linux_coop_dup2(цяло oldfd, цяло newfd)
{
    цяло fd = real_dup2(oldfd, newfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    върни fd;
}

__attribute__((weak)) цяло connect(цяло sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    върни freertos_linux_coop_connect(sockfd, addr, addrlen);
}

__attribute__((weak)) цяло accept(цяло sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    върни freertos_linux_coop_accept(sockfd, addr, addrlen);
}

__attribute__((weak)) цяло pselect(цяло nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, const struct timespec *timeout,
            const sigset_t *sigmask)
{
    върни freertos_linux_coop_pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}

__attribute__((weak)) цяло poll(struct pollfd *fds, nfds_t nfds, цяло timeout)
{
    върни freertos_linux_coop_poll(fds, nfds, timeout);
}

__attribute__((weak)) цяло socket(цяло domain, цяло type, цяло protocol)
{
    върни freertos_linux_coop_socket(domain, type, protocol);
}

__attribute__((weak)) цяло socketpair(цяло domain, цяло type, цяло protocol, цяло sv[2])
{
    върни freertos_linux_coop_socketpair(domain, type, protocol, sv);
}

__attribute__((weak)) цяло pipe(цяло fds[2])
{
    върни freertos_linux_coop_pipe(fds);
}

__attribute__((weak)) цяло pipe2(цяло fds[2], цяло flags)
{
    върни freertos_linux_coop_pipe2(fds, flags);
}

__attribute__((weak)) цяло dup(цяло oldfd)
{
    върни freertos_linux_coop_dup(oldfd);
}

__attribute__((weak)) цяло dup2(цяло oldfd, цяло newfd)
{
    върни freertos_linux_coop_dup2(oldfd, newfd);
}
