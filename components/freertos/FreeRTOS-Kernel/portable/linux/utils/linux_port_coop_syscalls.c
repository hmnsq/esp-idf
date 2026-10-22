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

static ssize_t  (*real_readv)(整数, const struct iovec *, 整数);
static ssize_t  (*real_writev)(整数, const struct iovec *, 整数);
static ssize_t  (*real_recv)(整数, void *, size_t, 整数);
static ssize_t  (*real_send)(整数, const void *, size_t, 整数);
static ssize_t  (*real_recvfrom)(整数, void *, size_t, 整数,
                                 struct sockaddr *, socklen_t *);
static ssize_t  (*real_sendto)(整数, const void *, size_t, 整数,
                                const struct sockaddr *, socklen_t);
static ssize_t  (*real_recvmsg)(整数, struct msghdr *, 整数);
static ssize_t  (*real_sendmsg)(整数, const struct msghdr *, 整数);
static 整数      (*real_connect)(整数, const struct sockaddr *, socklen_t);
static 整数      (*real_accept)(整数, struct sockaddr *, socklen_t *);
static 整数      (*real_pselect)(整数, fd_set *, fd_set *, fd_set *,
                                const struct timespec *, const sigset_t *);
static 整数      (*real_poll)(struct pollfd *, nfds_t, 整数);
static 整数      (*real_socket)(整数, 整数, 整数);
static 整数      (*real_socketpair)(整数, 整数, 整数, 整数[2]);
static 整数      (*real_pipe)(整数[2]);
static 整数      (*real_pipe2)(整数[2], 整数);
static 整数      (*real_dup)(整数);
static 整数      (*real_dup2)(整数, 整数);
static 整数      (*real_nanosleep)(const struct timespec *, struct timespec *);
static ssize_t  (*real_read)(整数, void *, size_t);
static ssize_t  (*real_write)(整数, const void *, size_t);
static ssize_t  (*real_pread)(整数, void *, size_t, off_t);
static ssize_t  (*real_pwrite)(整数, const void *, size_t, off_t);
static 整数      (*real_close)(整数);
static 整数      (*real_select)(整数, fd_set *, fd_set *, fd_set *,
                                struct timeval *);

typedef 整数 (*linux_coop_fcntl_fn_t)(整数, 整数, ...);
static linux_coop_fcntl_fn_t real_fcntl;

typedef 整数 (*linux_coop_open_fn_t)(const char *, 整数, mode_t);
static linux_coop_open_fn_t real_open;

typedef struct linux_coop_fd_state {
    整数 fd;
    整数 user_flags;
    struct linux_coop_fd_state *next;
} linux_coop_fd_state_t;

static linux_coop_fd_state_t *s_fd_state_list;
static pthread_mutex_t s_fd_state_lock = PTHREAD_MUTEX_INITIALIZER;

static linux_coop_fd_state_t *linux_coop_find_fd_locked(整数 fd)
{
    linux_coop_fd_state_t *node = s_fd_state_list;
    ながら (node != NULL) {
        もし (node->fd == fd) {
            戻る node;
        }
        node = node->next;
    }
    戻る NULL;
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

void linux_coop_set_nonblocking(整数 fd)
{
    もし (fd >= 0) {
        整数 flags = real_fcntl(fd, F_GETFL, 0);
        もし (flags >= 0) {
            real_fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
    }
}

void linux_coop_track_fd(整数 fd, 整数 user_flags)
{
    もし (fd < 0) {
        戻る;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *node = linux_coop_find_fd_locked(fd);
    もし (node != NULL) {
        node->user_flags = user_flags;
        pthread_mutex_unlock(&s_fd_state_lock);
        戻る;
    }

    node = calloc(1, sizeof(*node));
    もし (node != NULL) {
        node->fd = fd;
        node->user_flags = user_flags;
        node->next = s_fd_state_list;
        s_fd_state_list = node;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

void linux_coop_untrack_fd(整数 fd)
{
    もし (fd < 0) {
        戻る;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *prev = NULL;
    linux_coop_fd_state_t *node = s_fd_state_list;
    ながら (node != NULL) {
        もし (node->fd == fd) {
            もし (prev == NULL) {
                s_fd_state_list = node->next;
            } 否则 {
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

void linux_coop_set_user_flags(整数 fd, 整数 user_flags)
{
    linux_coop_track_fd(fd, user_flags);
}

bool linux_coop_get_user_flags(整数 fd, 整数 *flags)
{
    もし (fd < 0 || flags == NULL) {
        戻る false;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *node = linux_coop_find_fd_locked(fd);
    もし (node == NULL) {
        pthread_mutex_unlock(&s_fd_state_lock);
        戻る false;
    }
    *flags = node->user_flags;
    pthread_mutex_unlock(&s_fd_state_lock);
    戻る true;
}

bool linux_coop_fd_user_nonblocking(整数 fd)
{
    整数 user_flags = 0;
    もし (linux_coop_get_user_flags(fd, &user_flags)) {
        戻る (user_flags & O_NONBLOCK) != 0;
    }
    戻る false;
}


ssize_t freertos_linux_coop_read(整数 fd, void *buf, size_t count)
{
    LINUX_COOP_IO_LOOP(fd, real_read(fd, buf, count))
}

ssize_t freertos_linux_coop_write(整数 fd, const void *buf, size_t count)
{
    LINUX_COOP_IO_LOOP(fd, real_write(fd, buf, count))
}

ssize_t freertos_linux_coop_pread(整数 fd, void *buf, size_t count, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pread(fd, buf, count, offset))
}

ssize_t freertos_linux_coop_pwrite(整数 fd, const void *buf, size_t count, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pwrite(fd, buf, count, offset))
}

整数 freertos_linux_coop_open(const char *path, 整数 flags, 整数 mode)
{
    整数 fd = real_open(path, flags, (mode_t)mode);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, flags & O_NONBLOCK);
    戻る fd;
}

整数 freertos_linux_coop_close(整数 fd)
{
    整数 ret = real_close(fd);
    もし (ret == 0 || errno == EINTR) {
        /* On Linux, close() always releases the fd even when returning
         * EINTR.  Retrying would risk closing a fd reused by another
         * thread.  Treat EINTR as success. */
        linux_coop_untrack_fd(fd);
        戻る 0;
    }
    戻る -1;
}

整数 freertos_linux_coop_fcntl(整数 fd, 整数 cmd, 整数 arg)
{
    もし (cmd == F_GETFL) {
        整数 real_flags = real_fcntl(fd, F_GETFL, 0);
        もし (real_flags < 0) {
            戻る -1;
        }
        整数 user_flags = 0;
        もし (linux_coop_get_user_flags(fd, &user_flags)) {
            real_flags &= ~O_NONBLOCK;
            real_flags |= (user_flags & O_NONBLOCK);
        }
        戻る real_flags;
    }
    もし (cmd == F_SETFL) {
        linux_coop_set_user_flags(fd, arg);
        整数 internal_flags = arg | O_NONBLOCK;
        戻る real_fcntl(fd, F_SETFL, internal_flags);
    }
    戻る real_fcntl(fd, cmd, arg);
}

整数 freertos_linux_coop_select(整数 nfds, fd_set *readfds, fd_set *writefds,
                      fd_set *exceptfds, struct timeval *timeout)
{
    long timeout_ms = -1;
    もし (timeout != NULL) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_usec + 999) / 1000;
        もし (timeout_ms == 0) {
            戻る real_select(nfds, readfds, writefds, exceptfds, timeout);
        }
    }

    /* real_select() mutates the fd_sets in place (clears them, then sets bits
     * for ready descriptors). To keep polling the same descriptors across
     * iterations we must snapshot the caller's sets and restore them on every
     * loop. */
    fd_set rfds_in, wfds_in, efds_in;
    もし (readfds != NULL) {
        rfds_in = *readfds;
    }
    もし (writefds != NULL) {
        wfds_in = *writefds;
    }
    もし (exceptfds != NULL) {
        efds_in = *exceptfds;
    }

    long waited_ms = 0;
    ながら (1) {
        もし (readfds != NULL) {
            *readfds = rfds_in;
        }
        もし (writefds != NULL) {
            *writefds = wfds_in;
        }
        もし (exceptfds != NULL) {
            *exceptfds = efds_in;
        }

        struct timeval zero_tv = {0, 0};
        整数 ret = real_select(nfds, readfds, writefds, exceptfds, &zero_tv);
        もし (ret != 0) {
            戻る ret;
        }
        もし (timeout_ms == 0) {
            戻る 0;
        }
        もし (timeout_ms > 0 && waited_ms >= timeout_ms) {
            戻る 0;
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

__attribute__((weak)) ssize_t read(整数 fd, void *buf, size_t count)
{
    戻る freertos_linux_coop_read(fd, buf, count);
}

__attribute__((weak)) ssize_t write(整数 fd, const void *buf, size_t count)
{
    戻る freertos_linux_coop_write(fd, buf, count);
}

__attribute__((weak)) ssize_t pread(整数 fd, void *buf, size_t count, off_t offset)
{
    戻る freertos_linux_coop_pread(fd, buf, count, offset);
}

__attribute__((weak)) ssize_t pwrite(整数 fd, const void *buf, size_t count, off_t offset)
{
    戻る freertos_linux_coop_pwrite(fd, buf, count, offset);
}

__attribute__((weak)) 整数 open(const char *path, 整数 flags, ...)
{
    整数 mode = 0;
    もし (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        mode = va_arg(ap, 整数);
        va_end(ap);
    }
    戻る freertos_linux_coop_open(path, flags, mode);
}

__attribute__((weak)) 整数 close(整数 fd)
{
    戻る freertos_linux_coop_close(fd);
}

__attribute__((weak)) 整数 fcntl(整数 fd, 整数 cmd, ...)
{
    整数 arg = 0;
    もし (cmd != F_GETFL) {
        va_list list;
        va_start(list, cmd);
        arg = va_arg(list, 整数);
        va_end(list);
    }
    戻る freertos_linux_coop_fcntl(fd, cmd, arg);
}

__attribute__((weak)) 整数 select(整数 nfds, fd_set *readfds, fd_set *writefds,
                                 fd_set *exceptfds, struct timeval *timeout)
{
    戻る freertos_linux_coop_select(nfds, readfds, writefds, exceptfds, timeout);
}

ssize_t freertos_linux_coop_readv(整数 fd, const struct iovec *iov, 整数 iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_readv(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_writev(整数 fd, const struct iovec *iov, 整数 iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_writev(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_recv(整数 sockfd, void *buf, size_t len, 整数 flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recv(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_send(整数 sockfd, const void *buf, size_t len, 整数 flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_send(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_recvfrom(整数 sockfd, void *buf, size_t len, 整数 flags,
                 struct sockaddr *src_addr, socklen_t *addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvfrom(sockfd, buf, len, flags | MSG_DONTWAIT,
                            src_addr, addrlen))
}

ssize_t freertos_linux_coop_sendto(整数 sockfd, const void *buf, size_t len, 整数 flags,
               const struct sockaddr *dest_addr, socklen_t addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendto(sockfd, buf, len, flags | MSG_DONTWAIT,
                          dest_addr, addrlen))
}

ssize_t freertos_linux_coop_recvmsg(整数 sockfd, struct msghdr *msg, 整数 flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvmsg(sockfd, msg, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_sendmsg(整数 sockfd, const struct msghdr *msg, 整数 flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendmsg(sockfd, msg, flags | MSG_DONTWAIT))
}

__attribute__((weak)) ssize_t readv(整数 fd, const struct iovec *iov, 整数 iovcnt)
{
    戻る freertos_linux_coop_readv(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t writev(整数 fd, const struct iovec *iov, 整数 iovcnt)
{
    戻る freertos_linux_coop_writev(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t recv(整数 sockfd, void *buf, size_t len, 整数 flags)
{
    戻る freertos_linux_coop_recv(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t send(整数 sockfd, const void *buf, size_t len, 整数 flags)
{
    戻る freertos_linux_coop_send(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t recvfrom(整数 sockfd, void *buf, size_t len, 整数 flags,
                 struct sockaddr *src_addr, socklen_t *addrlen)
{
    戻る freertos_linux_coop_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

__attribute__((weak)) ssize_t sendto(整数 sockfd, const void *buf, size_t len, 整数 flags,
               const struct sockaddr *dest_addr, socklen_t addrlen)
{
    戻る freertos_linux_coop_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

__attribute__((weak)) ssize_t recvmsg(整数 sockfd, struct msghdr *msg, 整数 flags)
{
    戻る freertos_linux_coop_recvmsg(sockfd, msg, flags);
}

__attribute__((weak)) ssize_t sendmsg(整数 sockfd, const struct msghdr *msg, 整数 flags)
{
    戻る freertos_linux_coop_sendmsg(sockfd, msg, flags);
}

整数 freertos_linux_coop_connect(整数 sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    ながら (1)
    {
        整数 ret = real_connect(sockfd, addr, addrlen);
        もし (ret == 0) {
            戻る ret;
        }

        もし (errno == EINPROGRESS || errno == EALREADY) {
            struct pollfd pfd;
            pfd.fd = sockfd;
            pfd.events = POLLOUT;
            pfd.revents = 0;

            整数 poll_retries = 0;
            ながら (1)
            {
                整数 press = real_poll(&pfd, 1, 0);
                もし (press > 0) {
                    整数 so_err = 0;
                    socklen_t len = sizeof(so_err);
                    もし (getsockopt(sockfd, SOL_SOCKET, SO_ERROR,
                                   &so_err, &len) < 0) {
                        戻る -1;
                    }
                    もし (so_err == 0) {
                        戻る 0;
                    } 否则 {
                        errno = so_err;
                        戻る -1;
                    }
                } 否则 もし (press == 0) {
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    continue;
                } 否则 {
                    もし (errno == EINTR) {
                        continue;
                    }
                    /* Non-EINTR poll error; bail out after a few retries */
                    もし (++poll_retries > 3) {
                        戻る -1;
                    }
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    continue;
                }
            }
        }

        もし (errno == EINTR) {
            戻る -1;
        }

        戻る -1;
    }
}

整数 freertos_linux_coop_accept(整数 sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    ながら (1)
    {
        整数 fd = real_accept(sockfd, addr, addrlen);
        もし (fd >= 0) {
            linux_coop_set_nonblocking(fd);
            linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(sockfd) ? O_NONBLOCK : 0);
            戻る fd;
        }
        もし (errno == EAGAIN || errno == EWOULDBLOCK) {
            もし (linux_coop_fd_user_nonblocking(sockfd)) {
                戻る -1;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            continue;
        }
        戻る -1;
    }
}

整数 freertos_linux_coop_pselect(整数 nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, const struct timespec *timeout,
            const sigset_t *sigmask)
{
    long timeout_ms = -1;
    もし (timeout != NULL) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_nsec + 999999) / 1000000;
        もし (timeout_ms == 0) {
            戻る real_pselect(nfds, readfds, writefds, exceptfds,
                                timeout, sigmask);
        }
    }

    long waited_ms = 0;
    ながら (1)
    {
        struct timespec zero_ts = {0, 0};
        整数 ret = real_pselect(nfds, readfds, writefds, exceptfds,
                               &zero_ts, sigmask);
        もし (ret != 0) {
            戻る ret;
        }

        もし (timeout_ms == 0 ||
            (timeout_ms > 0 && waited_ms >= timeout_ms)) {
            戻る 0;
        }

        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

整数 freertos_linux_coop_poll(struct pollfd *fds, nfds_t nfds, 整数 timeout)
{
    もし (timeout == 0) {
        戻る real_poll(fds, nfds, 0);
    }

    long timeout_ms = -1;
    もし (timeout > 0) {
        timeout_ms = timeout;
    }

    long waited_ms = 0;

    ながら (1)
    {
        整数 ret = real_poll(fds, nfds, 0);
        もし (ret > 0) {
            戻る ret;
        } 否则 もし (ret == 0) {
            もし (timeout_ms == 0 ||
                (timeout_ms > 0 && waited_ms >= timeout_ms)) {
                戻る 0;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            waited_ms += LINUX_COOP_TICK_MS;
            continue;
        } 否则 {
            もし (errno == EINTR) {
                continue;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            もし (timeout_ms > 0 && waited_ms >= timeout_ms) {
                戻る -1;
            }
            waited_ms += LINUX_COOP_TICK_MS;
        }
    }
}

整数 nanosleep(const struct timespec *req, struct timespec *rem)
{
    もし (req == NULL) {
        errno = EFAULT;
        戻る -1;
    }
    もし (!linux_port_in_freertos_task()) {
        戻る real_nanosleep(req, rem);
    }
    long ms = req->tv_sec * 1000 + (req->tv_nsec + 999999) / 1000000;
    もし (ms <= 0) {
        ms = 1;
    }
    vTaskDelay(ms);
    もし (rem) {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }
    戻る 0;
}

unsigned 整数 sleep(unsigned 整数 seconds)
{
    linux_coop_yield(seconds * 1000);
    戻る 0;
}

整数 usleep(useconds_t usec)
{
    linux_coop_yield(usec / 1000);
    戻る 0;
}

整数 freertos_linux_coop_socket(整数 domain, 整数 type, 整数 protocol)
{
    整数 fd = real_socket(domain, type, protocol);
    linux_coop_set_nonblocking(fd);
#ifdef SOCK_NONBLOCK
    const 整数 user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#否则
    const 整数 user_flags = 0;
#endif
    linux_coop_track_fd(fd, user_flags);
    戻る fd;
}

整数 freertos_linux_coop_socketpair(整数 domain, 整数 type, 整数 protocol, 整数 *sv)
{
    整数 ret = real_socketpair(domain, type, protocol, sv);
    もし (ret == 0) {
        linux_coop_set_nonblocking(sv[0]);
        linux_coop_set_nonblocking(sv[1]);
#ifdef SOCK_NONBLOCK
        const 整数 user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#否则
        const 整数 user_flags = 0;
#endif
        linux_coop_track_fd(sv[0], user_flags);
        linux_coop_track_fd(sv[1], user_flags);
    }
    戻る ret;
}

整数 freertos_linux_coop_pipe(整数 *fds)
{
    整数 ret = real_pipe(fds);
    もし (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        linux_coop_track_fd(fds[0], 0);
        linux_coop_track_fd(fds[1], 0);
    }
    戻る ret;
}

整数 freertos_linux_coop_pipe2(整数 *fds, 整数 flags)
{
    整数 ret = real_pipe2(fds, flags);
    もし (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        const 整数 user_flags = (flags & O_NONBLOCK) ? O_NONBLOCK : 0;
        linux_coop_track_fd(fds[0], user_flags);
        linux_coop_track_fd(fds[1], user_flags);
    }
    戻る ret;
}

整数 freertos_linux_coop_dup(整数 oldfd)
{
    整数 fd = real_dup(oldfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    戻る fd;
}

整数 freertos_linux_coop_dup2(整数 oldfd, 整数 newfd)
{
    整数 fd = real_dup2(oldfd, newfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    戻る fd;
}

__attribute__((weak)) 整数 connect(整数 sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    戻る freertos_linux_coop_connect(sockfd, addr, addrlen);
}

__attribute__((weak)) 整数 accept(整数 sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    戻る freertos_linux_coop_accept(sockfd, addr, addrlen);
}

__attribute__((weak)) 整数 pselect(整数 nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, const struct timespec *timeout,
            const sigset_t *sigmask)
{
    戻る freertos_linux_coop_pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}

__attribute__((weak)) 整数 poll(struct pollfd *fds, nfds_t nfds, 整数 timeout)
{
    戻る freertos_linux_coop_poll(fds, nfds, timeout);
}

__attribute__((weak)) 整数 socket(整数 domain, 整数 type, 整数 protocol)
{
    戻る freertos_linux_coop_socket(domain, type, protocol);
}

__attribute__((weak)) 整数 socketpair(整数 domain, 整数 type, 整数 protocol, 整数 sv[2])
{
    戻る freertos_linux_coop_socketpair(domain, type, protocol, sv);
}

__attribute__((weak)) 整数 pipe(整数 fds[2])
{
    戻る freertos_linux_coop_pipe(fds);
}

__attribute__((weak)) 整数 pipe2(整数 fds[2], 整数 flags)
{
    戻る freertos_linux_coop_pipe2(fds, flags);
}

__attribute__((weak)) 整数 dup(整数 oldfd)
{
    戻る freertos_linux_coop_dup(oldfd);
}

__attribute__((weak)) 整数 dup2(整数 oldfd, 整数 newfd)
{
    戻る freertos_linux_coop_dup2(oldfd, newfd);
}
