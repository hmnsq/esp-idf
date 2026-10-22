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

static ssize_t  (*real_readv)(целое, const struct iovec *, целое);
static ssize_t  (*real_writev)(целое, const struct iovec *, целое);
static ssize_t  (*real_recv)(целое, void *, size_t, целое);
static ssize_t  (*real_send)(целое, const void *, size_t, целое);
static ssize_t  (*real_recvfrom)(целое, void *, size_t, целое,
                                 struct sockaddr *, socklen_t *);
static ssize_t  (*real_sendto)(целое, const void *, size_t, целое,
                                const struct sockaddr *, socklen_t);
static ssize_t  (*real_recvmsg)(целое, struct msghdr *, целое);
static ssize_t  (*real_sendmsg)(целое, const struct msghdr *, целое);
static целое      (*real_connect)(целое, const struct sockaddr *, socklen_t);
static целое      (*real_accept)(целое, struct sockaddr *, socklen_t *);
static целое      (*real_pselect)(целое, fd_set *, fd_set *, fd_set *,
                                const struct timespec *, const sigset_t *);
static целое      (*real_poll)(struct pollfd *, nfds_t, целое);
static целое      (*real_socket)(целое, целое, целое);
static целое      (*real_socketpair)(целое, целое, целое, целое[2]);
static целое      (*real_pipe)(целое[2]);
static целое      (*real_pipe2)(целое[2], целое);
static целое      (*real_dup)(целое);
static целое      (*real_dup2)(целое, целое);
static целое      (*real_nanosleep)(const struct timespec *, struct timespec *);
static ssize_t  (*real_read)(целое, void *, size_t);
static ssize_t  (*real_write)(целое, const void *, size_t);
static ssize_t  (*real_pread)(целое, void *, size_t, off_t);
static ssize_t  (*real_pwrite)(целое, const void *, size_t, off_t);
static целое      (*real_close)(целое);
static целое      (*real_select)(целое, fd_set *, fd_set *, fd_set *,
                                struct timeval *);

typedef целое (*linux_coop_fcntl_fn_t)(целое, целое, ...);
static linux_coop_fcntl_fn_t real_fcntl;

typedef целое (*linux_coop_open_fn_t)(const char *, целое, mode_t);
static linux_coop_open_fn_t real_open;

typedef struct linux_coop_fd_state {
    целое fd;
    целое user_flags;
    struct linux_coop_fd_state *next;
} linux_coop_fd_state_t;

static linux_coop_fd_state_t *s_fd_state_list;
static pthread_mutex_t s_fd_state_lock = PTHREAD_MUTEX_INITIALIZER;

static linux_coop_fd_state_t *linux_coop_find_fd_locked(целое fd)
{
    linux_coop_fd_state_t *node = s_fd_state_list;
    пока (node != NULL) {
        если (node->fd == fd) {
            вернуть node;
        }
        node = node->next;
    }
    вернуть NULL;
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

void linux_coop_set_nonblocking(целое fd)
{
    если (fd >= 0) {
        целое flags = real_fcntl(fd, F_GETFL, 0);
        если (flags >= 0) {
            real_fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
    }
}

void linux_coop_track_fd(целое fd, целое user_flags)
{
    если (fd < 0) {
        вернуть;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *node = linux_coop_find_fd_locked(fd);
    если (node != NULL) {
        node->user_flags = user_flags;
        pthread_mutex_unlock(&s_fd_state_lock);
        вернуть;
    }

    node = calloc(1, sizeof(*node));
    если (node != NULL) {
        node->fd = fd;
        node->user_flags = user_flags;
        node->next = s_fd_state_list;
        s_fd_state_list = node;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

void linux_coop_untrack_fd(целое fd)
{
    если (fd < 0) {
        вернуть;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *prev = NULL;
    linux_coop_fd_state_t *node = s_fd_state_list;
    пока (node != NULL) {
        если (node->fd == fd) {
            если (prev == NULL) {
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

void linux_coop_set_user_flags(целое fd, целое user_flags)
{
    linux_coop_track_fd(fd, user_flags);
}

bool linux_coop_get_user_flags(целое fd, целое *flags)
{
    если (fd < 0 || flags == NULL) {
        вернуть false;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *node = linux_coop_find_fd_locked(fd);
    если (node == NULL) {
        pthread_mutex_unlock(&s_fd_state_lock);
        вернуть false;
    }
    *flags = node->user_flags;
    pthread_mutex_unlock(&s_fd_state_lock);
    вернуть true;
}

bool linux_coop_fd_user_nonblocking(целое fd)
{
    целое user_flags = 0;
    если (linux_coop_get_user_flags(fd, &user_flags)) {
        вернуть (user_flags & O_NONBLOCK) != 0;
    }
    вернуть false;
}


ssize_t freertos_linux_coop_read(целое fd, void *buf, size_t count)
{
    LINUX_COOP_IO_LOOP(fd, real_read(fd, buf, count))
}

ssize_t freertos_linux_coop_write(целое fd, const void *buf, size_t count)
{
    LINUX_COOP_IO_LOOP(fd, real_write(fd, buf, count))
}

ssize_t freertos_linux_coop_pread(целое fd, void *buf, size_t count, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pread(fd, buf, count, offset))
}

ssize_t freertos_linux_coop_pwrite(целое fd, const void *buf, size_t count, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pwrite(fd, buf, count, offset))
}

целое freertos_linux_coop_open(const char *path, целое flags, целое mode)
{
    целое fd = real_open(path, flags, (mode_t)mode);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, flags & O_NONBLOCK);
    вернуть fd;
}

целое freertos_linux_coop_close(целое fd)
{
    целое ret = real_close(fd);
    если (ret == 0 || errno == EINTR) {
        /* On Linux, close() always releases the fd even when returning
         * EINTR.  Retrying would risk closing a fd reused by another
         * thread.  Treat EINTR as success. */
        linux_coop_untrack_fd(fd);
        вернуть 0;
    }
    вернуть -1;
}

целое freertos_linux_coop_fcntl(целое fd, целое cmd, целое arg)
{
    если (cmd == F_GETFL) {
        целое real_flags = real_fcntl(fd, F_GETFL, 0);
        если (real_flags < 0) {
            вернуть -1;
        }
        целое user_flags = 0;
        если (linux_coop_get_user_flags(fd, &user_flags)) {
            real_flags &= ~O_NONBLOCK;
            real_flags |= (user_flags & O_NONBLOCK);
        }
        вернуть real_flags;
    }
    если (cmd == F_SETFL) {
        linux_coop_set_user_flags(fd, arg);
        целое internal_flags = arg | O_NONBLOCK;
        вернуть real_fcntl(fd, F_SETFL, internal_flags);
    }
    вернуть real_fcntl(fd, cmd, arg);
}

целое freertos_linux_coop_select(целое nfds, fd_set *readfds, fd_set *writefds,
                      fd_set *exceptfds, struct timeval *timeout)
{
    long timeout_ms = -1;
    если (timeout != NULL) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_usec + 999) / 1000;
        если (timeout_ms == 0) {
            вернуть real_select(nfds, readfds, writefds, exceptfds, timeout);
        }
    }

    /* real_select() mutates the fd_sets in place (clears them, then sets bits
     * for ready descriptors). To keep polling the same descriptors across
     * iterations we must snapshot the caller's sets and restore them on every
     * loop. */
    fd_set rfds_in, wfds_in, efds_in;
    если (readfds != NULL) {
        rfds_in = *readfds;
    }
    если (writefds != NULL) {
        wfds_in = *writefds;
    }
    если (exceptfds != NULL) {
        efds_in = *exceptfds;
    }

    long waited_ms = 0;
    пока (1) {
        если (readfds != NULL) {
            *readfds = rfds_in;
        }
        если (writefds != NULL) {
            *writefds = wfds_in;
        }
        если (exceptfds != NULL) {
            *exceptfds = efds_in;
        }

        struct timeval zero_tv = {0, 0};
        целое ret = real_select(nfds, readfds, writefds, exceptfds, &zero_tv);
        если (ret != 0) {
            вернуть ret;
        }
        если (timeout_ms == 0) {
            вернуть 0;
        }
        если (timeout_ms > 0 && waited_ms >= timeout_ms) {
            вернуть 0;
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

__attribute__((weak)) ssize_t read(целое fd, void *buf, size_t count)
{
    вернуть freertos_linux_coop_read(fd, buf, count);
}

__attribute__((weak)) ssize_t write(целое fd, const void *buf, size_t count)
{
    вернуть freertos_linux_coop_write(fd, buf, count);
}

__attribute__((weak)) ssize_t pread(целое fd, void *buf, size_t count, off_t offset)
{
    вернуть freertos_linux_coop_pread(fd, buf, count, offset);
}

__attribute__((weak)) ssize_t pwrite(целое fd, const void *buf, size_t count, off_t offset)
{
    вернуть freertos_linux_coop_pwrite(fd, buf, count, offset);
}

__attribute__((weak)) целое open(const char *path, целое flags, ...)
{
    целое mode = 0;
    если (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        mode = va_arg(ap, целое);
        va_end(ap);
    }
    вернуть freertos_linux_coop_open(path, flags, mode);
}

__attribute__((weak)) целое close(целое fd)
{
    вернуть freertos_linux_coop_close(fd);
}

__attribute__((weak)) целое fcntl(целое fd, целое cmd, ...)
{
    целое arg = 0;
    если (cmd != F_GETFL) {
        va_list list;
        va_start(list, cmd);
        arg = va_arg(list, целое);
        va_end(list);
    }
    вернуть freertos_linux_coop_fcntl(fd, cmd, arg);
}

__attribute__((weak)) целое select(целое nfds, fd_set *readfds, fd_set *writefds,
                                 fd_set *exceptfds, struct timeval *timeout)
{
    вернуть freertos_linux_coop_select(nfds, readfds, writefds, exceptfds, timeout);
}

ssize_t freertos_linux_coop_readv(целое fd, const struct iovec *iov, целое iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_readv(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_writev(целое fd, const struct iovec *iov, целое iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_writev(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_recv(целое sockfd, void *buf, size_t len, целое flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recv(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_send(целое sockfd, const void *buf, size_t len, целое flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_send(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_recvfrom(целое sockfd, void *buf, size_t len, целое flags,
                 struct sockaddr *src_addr, socklen_t *addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvfrom(sockfd, buf, len, flags | MSG_DONTWAIT,
                            src_addr, addrlen))
}

ssize_t freertos_linux_coop_sendto(целое sockfd, const void *buf, size_t len, целое flags,
               const struct sockaddr *dest_addr, socklen_t addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendto(sockfd, buf, len, flags | MSG_DONTWAIT,
                          dest_addr, addrlen))
}

ssize_t freertos_linux_coop_recvmsg(целое sockfd, struct msghdr *msg, целое flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvmsg(sockfd, msg, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_sendmsg(целое sockfd, const struct msghdr *msg, целое flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendmsg(sockfd, msg, flags | MSG_DONTWAIT))
}

__attribute__((weak)) ssize_t readv(целое fd, const struct iovec *iov, целое iovcnt)
{
    вернуть freertos_linux_coop_readv(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t writev(целое fd, const struct iovec *iov, целое iovcnt)
{
    вернуть freertos_linux_coop_writev(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t recv(целое sockfd, void *buf, size_t len, целое flags)
{
    вернуть freertos_linux_coop_recv(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t send(целое sockfd, const void *buf, size_t len, целое flags)
{
    вернуть freertos_linux_coop_send(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t recvfrom(целое sockfd, void *buf, size_t len, целое flags,
                 struct sockaddr *src_addr, socklen_t *addrlen)
{
    вернуть freertos_linux_coop_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

__attribute__((weak)) ssize_t sendto(целое sockfd, const void *buf, size_t len, целое flags,
               const struct sockaddr *dest_addr, socklen_t addrlen)
{
    вернуть freertos_linux_coop_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

__attribute__((weak)) ssize_t recvmsg(целое sockfd, struct msghdr *msg, целое flags)
{
    вернуть freertos_linux_coop_recvmsg(sockfd, msg, flags);
}

__attribute__((weak)) ssize_t sendmsg(целое sockfd, const struct msghdr *msg, целое flags)
{
    вернуть freertos_linux_coop_sendmsg(sockfd, msg, flags);
}

целое freertos_linux_coop_connect(целое sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    пока (1)
    {
        целое ret = real_connect(sockfd, addr, addrlen);
        если (ret == 0) {
            вернуть ret;
        }

        если (errno == EINPROGRESS || errno == EALREADY) {
            struct pollfd pfd;
            pfd.fd = sockfd;
            pfd.events = POLLOUT;
            pfd.revents = 0;

            целое poll_retries = 0;
            пока (1)
            {
                целое press = real_poll(&pfd, 1, 0);
                если (press > 0) {
                    целое so_err = 0;
                    socklen_t len = sizeof(so_err);
                    если (getsockopt(sockfd, SOL_SOCKET, SO_ERROR,
                                   &so_err, &len) < 0) {
                        вернуть -1;
                    }
                    если (so_err == 0) {
                        вернуть 0;
                    } иначе {
                        errno = so_err;
                        вернуть -1;
                    }
                } иначе если (press == 0) {
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    continue;
                } иначе {
                    если (errno == EINTR) {
                        continue;
                    }
                    /* Non-EINTR poll error; bail out after a few retries */
                    если (++poll_retries > 3) {
                        вернуть -1;
                    }
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    continue;
                }
            }
        }

        если (errno == EINTR) {
            вернуть -1;
        }

        вернуть -1;
    }
}

целое freertos_linux_coop_accept(целое sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    пока (1)
    {
        целое fd = real_accept(sockfd, addr, addrlen);
        если (fd >= 0) {
            linux_coop_set_nonblocking(fd);
            linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(sockfd) ? O_NONBLOCK : 0);
            вернуть fd;
        }
        если (errno == EAGAIN || errno == EWOULDBLOCK) {
            если (linux_coop_fd_user_nonblocking(sockfd)) {
                вернуть -1;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            continue;
        }
        вернуть -1;
    }
}

целое freertos_linux_coop_pselect(целое nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, const struct timespec *timeout,
            const sigset_t *sigmask)
{
    long timeout_ms = -1;
    если (timeout != NULL) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_nsec + 999999) / 1000000;
        если (timeout_ms == 0) {
            вернуть real_pselect(nfds, readfds, writefds, exceptfds,
                                timeout, sigmask);
        }
    }

    long waited_ms = 0;
    пока (1)
    {
        struct timespec zero_ts = {0, 0};
        целое ret = real_pselect(nfds, readfds, writefds, exceptfds,
                               &zero_ts, sigmask);
        если (ret != 0) {
            вернуть ret;
        }

        если (timeout_ms == 0 ||
            (timeout_ms > 0 && waited_ms >= timeout_ms)) {
            вернуть 0;
        }

        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

целое freertos_linux_coop_poll(struct pollfd *fds, nfds_t nfds, целое timeout)
{
    если (timeout == 0) {
        вернуть real_poll(fds, nfds, 0);
    }

    long timeout_ms = -1;
    если (timeout > 0) {
        timeout_ms = timeout;
    }

    long waited_ms = 0;

    пока (1)
    {
        целое ret = real_poll(fds, nfds, 0);
        если (ret > 0) {
            вернуть ret;
        } иначе если (ret == 0) {
            если (timeout_ms == 0 ||
                (timeout_ms > 0 && waited_ms >= timeout_ms)) {
                вернуть 0;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            waited_ms += LINUX_COOP_TICK_MS;
            continue;
        } иначе {
            если (errno == EINTR) {
                continue;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            если (timeout_ms > 0 && waited_ms >= timeout_ms) {
                вернуть -1;
            }
            waited_ms += LINUX_COOP_TICK_MS;
        }
    }
}

целое nanosleep(const struct timespec *req, struct timespec *rem)
{
    если (req == NULL) {
        errno = EFAULT;
        вернуть -1;
    }
    если (!linux_port_in_freertos_task()) {
        вернуть real_nanosleep(req, rem);
    }
    long ms = req->tv_sec * 1000 + (req->tv_nsec + 999999) / 1000000;
    если (ms <= 0) {
        ms = 1;
    }
    vTaskDelay(ms);
    если (rem) {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }
    вернуть 0;
}

unsigned целое sleep(unsigned целое seconds)
{
    linux_coop_yield(seconds * 1000);
    вернуть 0;
}

целое usleep(useconds_t usec)
{
    linux_coop_yield(usec / 1000);
    вернуть 0;
}

целое freertos_linux_coop_socket(целое domain, целое type, целое protocol)
{
    целое fd = real_socket(domain, type, protocol);
    linux_coop_set_nonblocking(fd);
#ifdef SOCK_NONBLOCK
    const целое user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#иначе
    const целое user_flags = 0;
#endif
    linux_coop_track_fd(fd, user_flags);
    вернуть fd;
}

целое freertos_linux_coop_socketpair(целое domain, целое type, целое protocol, целое *sv)
{
    целое ret = real_socketpair(domain, type, protocol, sv);
    если (ret == 0) {
        linux_coop_set_nonblocking(sv[0]);
        linux_coop_set_nonblocking(sv[1]);
#ifdef SOCK_NONBLOCK
        const целое user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#иначе
        const целое user_flags = 0;
#endif
        linux_coop_track_fd(sv[0], user_flags);
        linux_coop_track_fd(sv[1], user_flags);
    }
    вернуть ret;
}

целое freertos_linux_coop_pipe(целое *fds)
{
    целое ret = real_pipe(fds);
    если (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        linux_coop_track_fd(fds[0], 0);
        linux_coop_track_fd(fds[1], 0);
    }
    вернуть ret;
}

целое freertos_linux_coop_pipe2(целое *fds, целое flags)
{
    целое ret = real_pipe2(fds, flags);
    если (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        const целое user_flags = (flags & O_NONBLOCK) ? O_NONBLOCK : 0;
        linux_coop_track_fd(fds[0], user_flags);
        linux_coop_track_fd(fds[1], user_flags);
    }
    вернуть ret;
}

целое freertos_linux_coop_dup(целое oldfd)
{
    целое fd = real_dup(oldfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    вернуть fd;
}

целое freertos_linux_coop_dup2(целое oldfd, целое newfd)
{
    целое fd = real_dup2(oldfd, newfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    вернуть fd;
}

__attribute__((weak)) целое connect(целое sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    вернуть freertos_linux_coop_connect(sockfd, addr, addrlen);
}

__attribute__((weak)) целое accept(целое sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    вернуть freertos_linux_coop_accept(sockfd, addr, addrlen);
}

__attribute__((weak)) целое pselect(целое nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, const struct timespec *timeout,
            const sigset_t *sigmask)
{
    вернуть freertos_linux_coop_pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}

__attribute__((weak)) целое poll(struct pollfd *fds, nfds_t nfds, целое timeout)
{
    вернуть freertos_linux_coop_poll(fds, nfds, timeout);
}

__attribute__((weak)) целое socket(целое domain, целое type, целое protocol)
{
    вернуть freertos_linux_coop_socket(domain, type, protocol);
}

__attribute__((weak)) целое socketpair(целое domain, целое type, целое protocol, целое sv[2])
{
    вернуть freertos_linux_coop_socketpair(domain, type, protocol, sv);
}

__attribute__((weak)) целое pipe(целое fds[2])
{
    вернуть freertos_linux_coop_pipe(fds);
}

__attribute__((weak)) целое pipe2(целое fds[2], целое flags)
{
    вернуть freertos_linux_coop_pipe2(fds, flags);
}

__attribute__((weak)) целое dup(целое oldfd)
{
    вернуть freertos_linux_coop_dup(oldfd);
}

__attribute__((weak)) целое dup2(целое oldfd, целое newfd)
{
    вернуть freertos_linux_coop_dup2(oldfd, newfd);
}
