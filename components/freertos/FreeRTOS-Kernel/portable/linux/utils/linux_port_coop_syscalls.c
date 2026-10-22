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

static ssize_t  (*real_readv)(정수, const struct iovec *, 정수);
static ssize_t  (*real_writev)(정수, const struct iovec *, 정수);
static ssize_t  (*real_recv)(정수, void *, size_t, 정수);
static ssize_t  (*real_send)(정수, const void *, size_t, 정수);
static ssize_t  (*real_recvfrom)(정수, void *, size_t, 정수,
                                 struct sockaddr *, socklen_t *);
static ssize_t  (*real_sendto)(정수, const void *, size_t, 정수,
                                const struct sockaddr *, socklen_t);
static ssize_t  (*real_recvmsg)(정수, struct msghdr *, 정수);
static ssize_t  (*real_sendmsg)(정수, const struct msghdr *, 정수);
static 정수      (*real_connect)(정수, const struct sockaddr *, socklen_t);
static 정수      (*real_accept)(정수, struct sockaddr *, socklen_t *);
static 정수      (*real_pselect)(정수, fd_set *, fd_set *, fd_set *,
                                const struct timespec *, const sigset_t *);
static 정수      (*real_poll)(struct pollfd *, nfds_t, 정수);
static 정수      (*real_socket)(정수, 정수, 정수);
static 정수      (*real_socketpair)(정수, 정수, 정수, 정수[2]);
static 정수      (*real_pipe)(정수[2]);
static 정수      (*real_pipe2)(정수[2], 정수);
static 정수      (*real_dup)(정수);
static 정수      (*real_dup2)(정수, 정수);
static 정수      (*real_nanosleep)(const struct timespec *, struct timespec *);
static ssize_t  (*real_read)(정수, void *, size_t);
static ssize_t  (*real_write)(정수, const void *, size_t);
static ssize_t  (*real_pread)(정수, void *, size_t, off_t);
static ssize_t  (*real_pwrite)(정수, const void *, size_t, off_t);
static 정수      (*real_close)(정수);
static 정수      (*real_select)(정수, fd_set *, fd_set *, fd_set *,
                                struct timeval *);

typedef 정수 (*linux_coop_fcntl_fn_t)(정수, 정수, ...);
static linux_coop_fcntl_fn_t real_fcntl;

typedef 정수 (*linux_coop_open_fn_t)(const char *, 정수, mode_t);
static linux_coop_open_fn_t real_open;

typedef struct linux_coop_fd_state {
    정수 fd;
    정수 user_flags;
    struct linux_coop_fd_state *next;
} linux_coop_fd_state_t;

static linux_coop_fd_state_t *s_fd_state_list;
static pthread_mutex_t s_fd_state_lock = PTHREAD_MUTEX_INITIALIZER;

static linux_coop_fd_state_t *linux_coop_find_fd_locked(정수 fd)
{
    linux_coop_fd_state_t *node = s_fd_state_list;
    동안 (node != NULL) {
        만약 (node->fd == fd) {
            반환 node;
        }
        node = node->next;
    }
    반환 NULL;
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

void linux_coop_set_nonblocking(정수 fd)
{
    만약 (fd >= 0) {
        정수 flags = real_fcntl(fd, F_GETFL, 0);
        만약 (flags >= 0) {
            real_fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
    }
}

void linux_coop_track_fd(정수 fd, 정수 user_flags)
{
    만약 (fd < 0) {
        반환;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *node = linux_coop_find_fd_locked(fd);
    만약 (node != NULL) {
        node->user_flags = user_flags;
        pthread_mutex_unlock(&s_fd_state_lock);
        반환;
    }

    node = calloc(1, sizeof(*node));
    만약 (node != NULL) {
        node->fd = fd;
        node->user_flags = user_flags;
        node->next = s_fd_state_list;
        s_fd_state_list = node;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

void linux_coop_untrack_fd(정수 fd)
{
    만약 (fd < 0) {
        반환;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *prev = NULL;
    linux_coop_fd_state_t *node = s_fd_state_list;
    동안 (node != NULL) {
        만약 (node->fd == fd) {
            만약 (prev == NULL) {
                s_fd_state_list = node->next;
            } 아니면 {
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

void linux_coop_set_user_flags(정수 fd, 정수 user_flags)
{
    linux_coop_track_fd(fd, user_flags);
}

bool linux_coop_get_user_flags(정수 fd, 정수 *flags)
{
    만약 (fd < 0 || flags == NULL) {
        반환 false;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *node = linux_coop_find_fd_locked(fd);
    만약 (node == NULL) {
        pthread_mutex_unlock(&s_fd_state_lock);
        반환 false;
    }
    *flags = node->user_flags;
    pthread_mutex_unlock(&s_fd_state_lock);
    반환 true;
}

bool linux_coop_fd_user_nonblocking(정수 fd)
{
    정수 user_flags = 0;
    만약 (linux_coop_get_user_flags(fd, &user_flags)) {
        반환 (user_flags & O_NONBLOCK) != 0;
    }
    반환 false;
}


ssize_t freertos_linux_coop_read(정수 fd, void *buf, size_t count)
{
    LINUX_COOP_IO_LOOP(fd, real_read(fd, buf, count))
}

ssize_t freertos_linux_coop_write(정수 fd, const void *buf, size_t count)
{
    LINUX_COOP_IO_LOOP(fd, real_write(fd, buf, count))
}

ssize_t freertos_linux_coop_pread(정수 fd, void *buf, size_t count, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pread(fd, buf, count, offset))
}

ssize_t freertos_linux_coop_pwrite(정수 fd, const void *buf, size_t count, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pwrite(fd, buf, count, offset))
}

정수 freertos_linux_coop_open(const char *path, 정수 flags, 정수 mode)
{
    정수 fd = real_open(path, flags, (mode_t)mode);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, flags & O_NONBLOCK);
    반환 fd;
}

정수 freertos_linux_coop_close(정수 fd)
{
    정수 ret = real_close(fd);
    만약 (ret == 0 || errno == EINTR) {
        /* On Linux, close() always releases the fd even when returning
         * EINTR.  Retrying would risk closing a fd reused by another
         * thread.  Treat EINTR as success. */
        linux_coop_untrack_fd(fd);
        반환 0;
    }
    반환 -1;
}

정수 freertos_linux_coop_fcntl(정수 fd, 정수 cmd, 정수 arg)
{
    만약 (cmd == F_GETFL) {
        정수 real_flags = real_fcntl(fd, F_GETFL, 0);
        만약 (real_flags < 0) {
            반환 -1;
        }
        정수 user_flags = 0;
        만약 (linux_coop_get_user_flags(fd, &user_flags)) {
            real_flags &= ~O_NONBLOCK;
            real_flags |= (user_flags & O_NONBLOCK);
        }
        반환 real_flags;
    }
    만약 (cmd == F_SETFL) {
        linux_coop_set_user_flags(fd, arg);
        정수 internal_flags = arg | O_NONBLOCK;
        반환 real_fcntl(fd, F_SETFL, internal_flags);
    }
    반환 real_fcntl(fd, cmd, arg);
}

정수 freertos_linux_coop_select(정수 nfds, fd_set *readfds, fd_set *writefds,
                      fd_set *exceptfds, struct timeval *timeout)
{
    long timeout_ms = -1;
    만약 (timeout != NULL) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_usec + 999) / 1000;
        만약 (timeout_ms == 0) {
            반환 real_select(nfds, readfds, writefds, exceptfds, timeout);
        }
    }

    /* real_select() mutates the fd_sets in place (clears them, then sets bits
     * for ready descriptors). To keep polling the same descriptors across
     * iterations we must snapshot the caller's sets and restore them on every
     * loop. */
    fd_set rfds_in, wfds_in, efds_in;
    만약 (readfds != NULL) {
        rfds_in = *readfds;
    }
    만약 (writefds != NULL) {
        wfds_in = *writefds;
    }
    만약 (exceptfds != NULL) {
        efds_in = *exceptfds;
    }

    long waited_ms = 0;
    동안 (1) {
        만약 (readfds != NULL) {
            *readfds = rfds_in;
        }
        만약 (writefds != NULL) {
            *writefds = wfds_in;
        }
        만약 (exceptfds != NULL) {
            *exceptfds = efds_in;
        }

        struct timeval zero_tv = {0, 0};
        정수 ret = real_select(nfds, readfds, writefds, exceptfds, &zero_tv);
        만약 (ret != 0) {
            반환 ret;
        }
        만약 (timeout_ms == 0) {
            반환 0;
        }
        만약 (timeout_ms > 0 && waited_ms >= timeout_ms) {
            반환 0;
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

__attribute__((weak)) ssize_t read(정수 fd, void *buf, size_t count)
{
    반환 freertos_linux_coop_read(fd, buf, count);
}

__attribute__((weak)) ssize_t write(정수 fd, const void *buf, size_t count)
{
    반환 freertos_linux_coop_write(fd, buf, count);
}

__attribute__((weak)) ssize_t pread(정수 fd, void *buf, size_t count, off_t offset)
{
    반환 freertos_linux_coop_pread(fd, buf, count, offset);
}

__attribute__((weak)) ssize_t pwrite(정수 fd, const void *buf, size_t count, off_t offset)
{
    반환 freertos_linux_coop_pwrite(fd, buf, count, offset);
}

__attribute__((weak)) 정수 open(const char *path, 정수 flags, ...)
{
    정수 mode = 0;
    만약 (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        mode = va_arg(ap, 정수);
        va_end(ap);
    }
    반환 freertos_linux_coop_open(path, flags, mode);
}

__attribute__((weak)) 정수 close(정수 fd)
{
    반환 freertos_linux_coop_close(fd);
}

__attribute__((weak)) 정수 fcntl(정수 fd, 정수 cmd, ...)
{
    정수 arg = 0;
    만약 (cmd != F_GETFL) {
        va_list list;
        va_start(list, cmd);
        arg = va_arg(list, 정수);
        va_end(list);
    }
    반환 freertos_linux_coop_fcntl(fd, cmd, arg);
}

__attribute__((weak)) 정수 select(정수 nfds, fd_set *readfds, fd_set *writefds,
                                 fd_set *exceptfds, struct timeval *timeout)
{
    반환 freertos_linux_coop_select(nfds, readfds, writefds, exceptfds, timeout);
}

ssize_t freertos_linux_coop_readv(정수 fd, const struct iovec *iov, 정수 iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_readv(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_writev(정수 fd, const struct iovec *iov, 정수 iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_writev(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_recv(정수 sockfd, void *buf, size_t len, 정수 flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recv(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_send(정수 sockfd, const void *buf, size_t len, 정수 flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_send(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_recvfrom(정수 sockfd, void *buf, size_t len, 정수 flags,
                 struct sockaddr *src_addr, socklen_t *addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvfrom(sockfd, buf, len, flags | MSG_DONTWAIT,
                            src_addr, addrlen))
}

ssize_t freertos_linux_coop_sendto(정수 sockfd, const void *buf, size_t len, 정수 flags,
               const struct sockaddr *dest_addr, socklen_t addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendto(sockfd, buf, len, flags | MSG_DONTWAIT,
                          dest_addr, addrlen))
}

ssize_t freertos_linux_coop_recvmsg(정수 sockfd, struct msghdr *msg, 정수 flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvmsg(sockfd, msg, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_sendmsg(정수 sockfd, const struct msghdr *msg, 정수 flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendmsg(sockfd, msg, flags | MSG_DONTWAIT))
}

__attribute__((weak)) ssize_t readv(정수 fd, const struct iovec *iov, 정수 iovcnt)
{
    반환 freertos_linux_coop_readv(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t writev(정수 fd, const struct iovec *iov, 정수 iovcnt)
{
    반환 freertos_linux_coop_writev(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t recv(정수 sockfd, void *buf, size_t len, 정수 flags)
{
    반환 freertos_linux_coop_recv(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t send(정수 sockfd, const void *buf, size_t len, 정수 flags)
{
    반환 freertos_linux_coop_send(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t recvfrom(정수 sockfd, void *buf, size_t len, 정수 flags,
                 struct sockaddr *src_addr, socklen_t *addrlen)
{
    반환 freertos_linux_coop_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

__attribute__((weak)) ssize_t sendto(정수 sockfd, const void *buf, size_t len, 정수 flags,
               const struct sockaddr *dest_addr, socklen_t addrlen)
{
    반환 freertos_linux_coop_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

__attribute__((weak)) ssize_t recvmsg(정수 sockfd, struct msghdr *msg, 정수 flags)
{
    반환 freertos_linux_coop_recvmsg(sockfd, msg, flags);
}

__attribute__((weak)) ssize_t sendmsg(정수 sockfd, const struct msghdr *msg, 정수 flags)
{
    반환 freertos_linux_coop_sendmsg(sockfd, msg, flags);
}

정수 freertos_linux_coop_connect(정수 sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    동안 (1)
    {
        정수 ret = real_connect(sockfd, addr, addrlen);
        만약 (ret == 0) {
            반환 ret;
        }

        만약 (errno == EINPROGRESS || errno == EALREADY) {
            struct pollfd pfd;
            pfd.fd = sockfd;
            pfd.events = POLLOUT;
            pfd.revents = 0;

            정수 poll_retries = 0;
            동안 (1)
            {
                정수 press = real_poll(&pfd, 1, 0);
                만약 (press > 0) {
                    정수 so_err = 0;
                    socklen_t len = sizeof(so_err);
                    만약 (getsockopt(sockfd, SOL_SOCKET, SO_ERROR,
                                   &so_err, &len) < 0) {
                        반환 -1;
                    }
                    만약 (so_err == 0) {
                        반환 0;
                    } 아니면 {
                        errno = so_err;
                        반환 -1;
                    }
                } 아니면 만약 (press == 0) {
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    continue;
                } 아니면 {
                    만약 (errno == EINTR) {
                        continue;
                    }
                    /* Non-EINTR poll error; bail out after a few retries */
                    만약 (++poll_retries > 3) {
                        반환 -1;
                    }
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    continue;
                }
            }
        }

        만약 (errno == EINTR) {
            반환 -1;
        }

        반환 -1;
    }
}

정수 freertos_linux_coop_accept(정수 sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    동안 (1)
    {
        정수 fd = real_accept(sockfd, addr, addrlen);
        만약 (fd >= 0) {
            linux_coop_set_nonblocking(fd);
            linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(sockfd) ? O_NONBLOCK : 0);
            반환 fd;
        }
        만약 (errno == EAGAIN || errno == EWOULDBLOCK) {
            만약 (linux_coop_fd_user_nonblocking(sockfd)) {
                반환 -1;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            continue;
        }
        반환 -1;
    }
}

정수 freertos_linux_coop_pselect(정수 nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, const struct timespec *timeout,
            const sigset_t *sigmask)
{
    long timeout_ms = -1;
    만약 (timeout != NULL) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_nsec + 999999) / 1000000;
        만약 (timeout_ms == 0) {
            반환 real_pselect(nfds, readfds, writefds, exceptfds,
                                timeout, sigmask);
        }
    }

    long waited_ms = 0;
    동안 (1)
    {
        struct timespec zero_ts = {0, 0};
        정수 ret = real_pselect(nfds, readfds, writefds, exceptfds,
                               &zero_ts, sigmask);
        만약 (ret != 0) {
            반환 ret;
        }

        만약 (timeout_ms == 0 ||
            (timeout_ms > 0 && waited_ms >= timeout_ms)) {
            반환 0;
        }

        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

정수 freertos_linux_coop_poll(struct pollfd *fds, nfds_t nfds, 정수 timeout)
{
    만약 (timeout == 0) {
        반환 real_poll(fds, nfds, 0);
    }

    long timeout_ms = -1;
    만약 (timeout > 0) {
        timeout_ms = timeout;
    }

    long waited_ms = 0;

    동안 (1)
    {
        정수 ret = real_poll(fds, nfds, 0);
        만약 (ret > 0) {
            반환 ret;
        } 아니면 만약 (ret == 0) {
            만약 (timeout_ms == 0 ||
                (timeout_ms > 0 && waited_ms >= timeout_ms)) {
                반환 0;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            waited_ms += LINUX_COOP_TICK_MS;
            continue;
        } 아니면 {
            만약 (errno == EINTR) {
                continue;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            만약 (timeout_ms > 0 && waited_ms >= timeout_ms) {
                반환 -1;
            }
            waited_ms += LINUX_COOP_TICK_MS;
        }
    }
}

정수 nanosleep(const struct timespec *req, struct timespec *rem)
{
    만약 (req == NULL) {
        errno = EFAULT;
        반환 -1;
    }
    만약 (!linux_port_in_freertos_task()) {
        반환 real_nanosleep(req, rem);
    }
    long ms = req->tv_sec * 1000 + (req->tv_nsec + 999999) / 1000000;
    만약 (ms <= 0) {
        ms = 1;
    }
    vTaskDelay(ms);
    만약 (rem) {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }
    반환 0;
}

unsigned 정수 sleep(unsigned 정수 seconds)
{
    linux_coop_yield(seconds * 1000);
    반환 0;
}

정수 usleep(useconds_t usec)
{
    linux_coop_yield(usec / 1000);
    반환 0;
}

정수 freertos_linux_coop_socket(정수 domain, 정수 type, 정수 protocol)
{
    정수 fd = real_socket(domain, type, protocol);
    linux_coop_set_nonblocking(fd);
#ifdef SOCK_NONBLOCK
    const 정수 user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#아니면
    const 정수 user_flags = 0;
#endif
    linux_coop_track_fd(fd, user_flags);
    반환 fd;
}

정수 freertos_linux_coop_socketpair(정수 domain, 정수 type, 정수 protocol, 정수 *sv)
{
    정수 ret = real_socketpair(domain, type, protocol, sv);
    만약 (ret == 0) {
        linux_coop_set_nonblocking(sv[0]);
        linux_coop_set_nonblocking(sv[1]);
#ifdef SOCK_NONBLOCK
        const 정수 user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#아니면
        const 정수 user_flags = 0;
#endif
        linux_coop_track_fd(sv[0], user_flags);
        linux_coop_track_fd(sv[1], user_flags);
    }
    반환 ret;
}

정수 freertos_linux_coop_pipe(정수 *fds)
{
    정수 ret = real_pipe(fds);
    만약 (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        linux_coop_track_fd(fds[0], 0);
        linux_coop_track_fd(fds[1], 0);
    }
    반환 ret;
}

정수 freertos_linux_coop_pipe2(정수 *fds, 정수 flags)
{
    정수 ret = real_pipe2(fds, flags);
    만약 (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        const 정수 user_flags = (flags & O_NONBLOCK) ? O_NONBLOCK : 0;
        linux_coop_track_fd(fds[0], user_flags);
        linux_coop_track_fd(fds[1], user_flags);
    }
    반환 ret;
}

정수 freertos_linux_coop_dup(정수 oldfd)
{
    정수 fd = real_dup(oldfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    반환 fd;
}

정수 freertos_linux_coop_dup2(정수 oldfd, 정수 newfd)
{
    정수 fd = real_dup2(oldfd, newfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    반환 fd;
}

__attribute__((weak)) 정수 connect(정수 sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    반환 freertos_linux_coop_connect(sockfd, addr, addrlen);
}

__attribute__((weak)) 정수 accept(정수 sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    반환 freertos_linux_coop_accept(sockfd, addr, addrlen);
}

__attribute__((weak)) 정수 pselect(정수 nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, const struct timespec *timeout,
            const sigset_t *sigmask)
{
    반환 freertos_linux_coop_pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}

__attribute__((weak)) 정수 poll(struct pollfd *fds, nfds_t nfds, 정수 timeout)
{
    반환 freertos_linux_coop_poll(fds, nfds, timeout);
}

__attribute__((weak)) 정수 socket(정수 domain, 정수 type, 정수 protocol)
{
    반환 freertos_linux_coop_socket(domain, type, protocol);
}

__attribute__((weak)) 정수 socketpair(정수 domain, 정수 type, 정수 protocol, 정수 sv[2])
{
    반환 freertos_linux_coop_socketpair(domain, type, protocol, sv);
}

__attribute__((weak)) 정수 pipe(정수 fds[2])
{
    반환 freertos_linux_coop_pipe(fds);
}

__attribute__((weak)) 정수 pipe2(정수 fds[2], 정수 flags)
{
    반환 freertos_linux_coop_pipe2(fds, flags);
}

__attribute__((weak)) 정수 dup(정수 oldfd)
{
    반환 freertos_linux_coop_dup(oldfd);
}

__attribute__((weak)) 정수 dup2(정수 oldfd, 정수 newfd)
{
    반환 freertos_linux_coop_dup2(oldfd, newfd);
}
