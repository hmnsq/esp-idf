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

static ssize_t  (*real_readv)(จำนวนเต็ม, const sa_nracanaa iovec *, จำนวนเต็ม);
static ssize_t  (*real_writev)(จำนวนเต็ม, const sa_nracanaa iovec *, จำนวนเต็ม);
static ssize_t  (*real_recv)(จำนวนเต็ม, void *, size_t, จำนวนเต็ม);
static ssize_t  (*real_send)(จำนวนเต็ม, const void *, size_t, จำนวนเต็ม);
static ssize_t  (*real_recvfrom)(จำนวนเต็ม, void *, size_t, จำนวนเต็ม,
                                 sa_nracanaa sockaddr *, socklen_t *);
static ssize_t  (*real_sendto)(จำนวนเต็ม, const void *, size_t, จำนวนเต็ม,
                                const sa_nracanaa sockaddr *, socklen_t);
static ssize_t  (*real_recvmsg)(จำนวนเต็ม, sa_nracanaa msghdr *, จำนวนเต็ม);
static ssize_t  (*real_sendmsg)(จำนวนเต็ม, const sa_nracanaa msghdr *, จำนวนเต็ม);
static จำนวนเต็ม      (*real_connect)(จำนวนเต็ม, const sa_nracanaa sockaddr *, socklen_t);
static จำนวนเต็ม      (*real_accept)(จำนวนเต็ม, sa_nracanaa sockaddr *, socklen_t *);
static จำนวนเต็ม      (*real_pselect)(จำนวนเต็ม, fd_set *, fd_set *, fd_set *,
                                const sa_nracanaa timespec *, const sigset_t *);
static จำนวนเต็ม      (*real_poll)(sa_nracanaa pollfd *, nfds_t, จำนวนเต็ม);
static จำนวนเต็ม      (*real_socket)(จำนวนเต็ม, จำนวนเต็ม, จำนวนเต็ม);
static จำนวนเต็ม      (*real_socketpair)(จำนวนเต็ม, จำนวนเต็ม, จำนวนเต็ม, จำนวนเต็ม[2]);
static จำนวนเต็ม      (*real_pipe)(จำนวนเต็ม[2]);
static จำนวนเต็ม      (*real_pipe2)(จำนวนเต็ม[2], จำนวนเต็ม);
static จำนวนเต็ม      (*real_dup)(จำนวนเต็ม);
static จำนวนเต็ม      (*real_dup2)(จำนวนเต็ม, จำนวนเต็ม);
static จำนวนเต็ม      (*real_nanosleep)(const sa_nracanaa timespec *, sa_nracanaa timespec *);
static ssize_t  (*real_read)(จำนวนเต็ม, void *, size_t);
static ssize_t  (*real_write)(จำนวนเต็ม, const void *, size_t);
static ssize_t  (*real_pread)(จำนวนเต็ม, void *, size_t, off_t);
static ssize_t  (*real_pwrite)(จำนวนเต็ม, const void *, size_t, off_t);
static จำนวนเต็ม      (*real_close)(จำนวนเต็ม);
static จำนวนเต็ม      (*real_select)(จำนวนเต็ม, fd_set *, fd_set *, fd_set *,
                                sa_nracanaa timeval *);

typedef จำนวนเต็ม (*linux_coop_fcntl_fn_t)(จำนวนเต็ม, จำนวนเต็ม, ...);
static linux_coop_fcntl_fn_t real_fcntl;

typedef จำนวนเต็ม (*linux_coop_open_fn_t)(const var_na *, จำนวนเต็ม, mode_t);
static linux_coop_open_fn_t real_open;

typedef sa_nracanaa linux_coop_fd_state {
    จำนวนเต็ม fd;
    จำนวนเต็ม user_flags;
    sa_nracanaa linux_coop_fd_state *next;
} linux_coop_fd_state_t;

static linux_coop_fd_state_t *s_fd_state_list;
static pthread_mutex_t s_fd_state_lock = PTHREAD_MUTEX_INITIALIZER;

static linux_coop_fd_state_t *linux_coop_find_fd_locked(จำนวนเต็ม fd)
{
    linux_coop_fd_state_t *node = s_fd_state_list;
    ขณะ (node != NULL) {
        ถ้า (node->fd == fd) {
            คืน node;
        }
        node = node->next;
    }
    คืน NULL;
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

void linux_coop_set_nonblocking(จำนวนเต็ม fd)
{
    ถ้า (fd >= 0) {
        จำนวนเต็ม flags = real_fcntl(fd, F_GETFL, 0);
        ถ้า (flags >= 0) {
            real_fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
    }
}

void linux_coop_track_fd(จำนวนเต็ม fd, จำนวนเต็ม user_flags)
{
    ถ้า (fd < 0) {
        คืน;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *node = linux_coop_find_fd_locked(fd);
    ถ้า (node != NULL) {
        node->user_flags = user_flags;
        pthread_mutex_unlock(&s_fd_state_lock);
        คืน;
    }

    node = calloc(1, parimaa_na(*node));
    ถ้า (node != NULL) {
        node->fd = fd;
        node->user_flags = user_flags;
        node->next = s_fd_state_list;
        s_fd_state_list = node;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

void linux_coop_untrack_fd(จำนวนเต็ม fd)
{
    ถ้า (fd < 0) {
        คืน;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *prev = NULL;
    linux_coop_fd_state_t *node = s_fd_state_list;
    ขณะ (node != NULL) {
        ถ้า (node->fd == fd) {
            ถ้า (prev == NULL) {
                s_fd_state_list = node->next;
            } มิฉะนั้น {
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

void linux_coop_set_user_flags(จำนวนเต็ม fd, จำนวนเต็ม user_flags)
{
    linux_coop_track_fd(fd, user_flags);
}

bool linux_coop_get_user_flags(จำนวนเต็ม fd, จำนวนเต็ม *flags)
{
    ถ้า (fd < 0 || flags == NULL) {
        คืน false;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *node = linux_coop_find_fd_locked(fd);
    ถ้า (node == NULL) {
        pthread_mutex_unlock(&s_fd_state_lock);
        คืน false;
    }
    *flags = node->user_flags;
    pthread_mutex_unlock(&s_fd_state_lock);
    คืน true;
}

bool linux_coop_fd_user_nonblocking(จำนวนเต็ม fd)
{
    จำนวนเต็ม user_flags = 0;
    ถ้า (linux_coop_get_user_flags(fd, &user_flags)) {
        คืน (user_flags & O_NONBLOCK) != 0;
    }
    คืน false;
}


ssize_t freertos_linux_coop_read(จำนวนเต็ม fd, void *buf, size_t count)
{
    LINUX_COOP_IO_LOOP(fd, real_read(fd, buf, count))
}

ssize_t freertos_linux_coop_write(จำนวนเต็ม fd, const void *buf, size_t count)
{
    LINUX_COOP_IO_LOOP(fd, real_write(fd, buf, count))
}

ssize_t freertos_linux_coop_pread(จำนวนเต็ม fd, void *buf, size_t count, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pread(fd, buf, count, offset))
}

ssize_t freertos_linux_coop_pwrite(จำนวนเต็ม fd, const void *buf, size_t count, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pwrite(fd, buf, count, offset))
}

จำนวนเต็ม freertos_linux_coop_open(const var_na *path, จำนวนเต็ม flags, จำนวนเต็ม mode)
{
    จำนวนเต็ม fd = real_open(path, flags, (mode_t)mode);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, flags & O_NONBLOCK);
    คืน fd;
}

จำนวนเต็ม freertos_linux_coop_close(จำนวนเต็ม fd)
{
    จำนวนเต็ม ret = real_close(fd);
    ถ้า (ret == 0 || errno == EINTR) {
        /* On Linux, close() always releases the fd even when returning
         * EINTR.  Retrying would risk closing a fd reused by another
         * thread.  Treat EINTR as success. */
        linux_coop_untrack_fd(fd);
        คืน 0;
    }
    คืน -1;
}

จำนวนเต็ม freertos_linux_coop_fcntl(จำนวนเต็ม fd, จำนวนเต็ม cmd, จำนวนเต็ม arg)
{
    ถ้า (cmd == F_GETFL) {
        จำนวนเต็ม real_flags = real_fcntl(fd, F_GETFL, 0);
        ถ้า (real_flags < 0) {
            คืน -1;
        }
        จำนวนเต็ม user_flags = 0;
        ถ้า (linux_coop_get_user_flags(fd, &user_flags)) {
            real_flags &= ~O_NONBLOCK;
            real_flags |= (user_flags & O_NONBLOCK);
        }
        คืน real_flags;
    }
    ถ้า (cmd == F_SETFL) {
        linux_coop_set_user_flags(fd, arg);
        จำนวนเต็ม internal_flags = arg | O_NONBLOCK;
        คืน real_fcntl(fd, F_SETFL, internal_flags);
    }
    คืน real_fcntl(fd, cmd, arg);
}

จำนวนเต็ม freertos_linux_coop_select(จำนวนเต็ม nfds, fd_set *readfds, fd_set *writefds,
                      fd_set *exceptfds, sa_nracanaa timeval *timeout)
{
    long timeout_ms = -1;
    ถ้า (timeout != NULL) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_usec + 999) / 1000;
        ถ้า (timeout_ms == 0) {
            คืน real_select(nfds, readfds, writefds, exceptfds, timeout);
        }
    }

    /* real_select() mutates the fd_sets in place (clears them, then sets bits
     * for ready descriptors). To keep polling the same descriptors across
     * iterations we must snapshot the caller's sets and restore them on every
     * loop. */
    fd_set rfds_in, wfds_in, efds_in;
    ถ้า (readfds != NULL) {
        rfds_in = *readfds;
    }
    ถ้า (writefds != NULL) {
        wfds_in = *writefds;
    }
    ถ้า (exceptfds != NULL) {
        efds_in = *exceptfds;
    }

    long waited_ms = 0;
    ขณะ (1) {
        ถ้า (readfds != NULL) {
            *readfds = rfds_in;
        }
        ถ้า (writefds != NULL) {
            *writefds = wfds_in;
        }
        ถ้า (exceptfds != NULL) {
            *exceptfds = efds_in;
        }

        sa_nracanaa timeval zero_tv = {0, 0};
        จำนวนเต็ม ret = real_select(nfds, readfds, writefds, exceptfds, &zero_tv);
        ถ้า (ret != 0) {
            คืน ret;
        }
        ถ้า (timeout_ms == 0) {
            คืน 0;
        }
        ถ้า (timeout_ms > 0 && waited_ms >= timeout_ms) {
            คืน 0;
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

__attribute__((weak)) ssize_t read(จำนวนเต็ม fd, void *buf, size_t count)
{
    คืน freertos_linux_coop_read(fd, buf, count);
}

__attribute__((weak)) ssize_t write(จำนวนเต็ม fd, const void *buf, size_t count)
{
    คืน freertos_linux_coop_write(fd, buf, count);
}

__attribute__((weak)) ssize_t pread(จำนวนเต็ม fd, void *buf, size_t count, off_t offset)
{
    คืน freertos_linux_coop_pread(fd, buf, count, offset);
}

__attribute__((weak)) ssize_t pwrite(จำนวนเต็ม fd, const void *buf, size_t count, off_t offset)
{
    คืน freertos_linux_coop_pwrite(fd, buf, count, offset);
}

__attribute__((weak)) จำนวนเต็ม open(const var_na *path, จำนวนเต็ม flags, ...)
{
    จำนวนเต็ม mode = 0;
    ถ้า (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        mode = va_arg(ap, จำนวนเต็ม);
        va_end(ap);
    }
    คืน freertos_linux_coop_open(path, flags, mode);
}

__attribute__((weak)) จำนวนเต็ม close(จำนวนเต็ม fd)
{
    คืน freertos_linux_coop_close(fd);
}

__attribute__((weak)) จำนวนเต็ม fcntl(จำนวนเต็ม fd, จำนวนเต็ม cmd, ...)
{
    จำนวนเต็ม arg = 0;
    ถ้า (cmd != F_GETFL) {
        va_list list;
        va_start(list, cmd);
        arg = va_arg(list, จำนวนเต็ม);
        va_end(list);
    }
    คืน freertos_linux_coop_fcntl(fd, cmd, arg);
}

__attribute__((weak)) จำนวนเต็ม select(จำนวนเต็ม nfds, fd_set *readfds, fd_set *writefds,
                                 fd_set *exceptfds, sa_nracanaa timeval *timeout)
{
    คืน freertos_linux_coop_select(nfds, readfds, writefds, exceptfds, timeout);
}

ssize_t freertos_linux_coop_readv(จำนวนเต็ม fd, const sa_nracanaa iovec *iov, จำนวนเต็ม iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_readv(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_writev(จำนวนเต็ม fd, const sa_nracanaa iovec *iov, จำนวนเต็ม iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_writev(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_recv(จำนวนเต็ม sockfd, void *buf, size_t len, จำนวนเต็ม flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recv(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_send(จำนวนเต็ม sockfd, const void *buf, size_t len, จำนวนเต็ม flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_send(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_recvfrom(จำนวนเต็ม sockfd, void *buf, size_t len, จำนวนเต็ม flags,
                 sa_nracanaa sockaddr *src_addr, socklen_t *addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvfrom(sockfd, buf, len, flags | MSG_DONTWAIT,
                            src_addr, addrlen))
}

ssize_t freertos_linux_coop_sendto(จำนวนเต็ม sockfd, const void *buf, size_t len, จำนวนเต็ม flags,
               const sa_nracanaa sockaddr *dest_addr, socklen_t addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendto(sockfd, buf, len, flags | MSG_DONTWAIT,
                          dest_addr, addrlen))
}

ssize_t freertos_linux_coop_recvmsg(จำนวนเต็ม sockfd, sa_nracanaa msghdr *msg, จำนวนเต็ม flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvmsg(sockfd, msg, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_sendmsg(จำนวนเต็ม sockfd, const sa_nracanaa msghdr *msg, จำนวนเต็ม flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendmsg(sockfd, msg, flags | MSG_DONTWAIT))
}

__attribute__((weak)) ssize_t readv(จำนวนเต็ม fd, const sa_nracanaa iovec *iov, จำนวนเต็ม iovcnt)
{
    คืน freertos_linux_coop_readv(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t writev(จำนวนเต็ม fd, const sa_nracanaa iovec *iov, จำนวนเต็ม iovcnt)
{
    คืน freertos_linux_coop_writev(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t recv(จำนวนเต็ม sockfd, void *buf, size_t len, จำนวนเต็ม flags)
{
    คืน freertos_linux_coop_recv(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t send(จำนวนเต็ม sockfd, const void *buf, size_t len, จำนวนเต็ม flags)
{
    คืน freertos_linux_coop_send(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t recvfrom(จำนวนเต็ม sockfd, void *buf, size_t len, จำนวนเต็ม flags,
                 sa_nracanaa sockaddr *src_addr, socklen_t *addrlen)
{
    คืน freertos_linux_coop_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

__attribute__((weak)) ssize_t sendto(จำนวนเต็ม sockfd, const void *buf, size_t len, จำนวนเต็ม flags,
               const sa_nracanaa sockaddr *dest_addr, socklen_t addrlen)
{
    คืน freertos_linux_coop_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

__attribute__((weak)) ssize_t recvmsg(จำนวนเต็ม sockfd, sa_nracanaa msghdr *msg, จำนวนเต็ม flags)
{
    คืน freertos_linux_coop_recvmsg(sockfd, msg, flags);
}

__attribute__((weak)) ssize_t sendmsg(จำนวนเต็ม sockfd, const sa_nracanaa msghdr *msg, จำนวนเต็ม flags)
{
    คืน freertos_linux_coop_sendmsg(sockfd, msg, flags);
}

จำนวนเต็ม freertos_linux_coop_connect(จำนวนเต็ม sockfd, const sa_nracanaa sockaddr *addr, socklen_t addrlen)
{
    ขณะ (1)
    {
        จำนวนเต็ม ret = real_connect(sockfd, addr, addrlen);
        ถ้า (ret == 0) {
            คืน ret;
        }

        ถ้า (errno == EINPROGRESS || errno == EALREADY) {
            sa_nracanaa pollfd pfd;
            pfd.fd = sockfd;
            pfd.events = POLLOUT;
            pfd.revents = 0;

            จำนวนเต็ม poll_retries = 0;
            ขณะ (1)
            {
                จำนวนเต็ม press = real_poll(&pfd, 1, 0);
                ถ้า (press > 0) {
                    จำนวนเต็ม so_err = 0;
                    socklen_t len = parimaa_na(so_err);
                    ถ้า (getsockopt(sockfd, SOL_SOCKET, SO_ERROR,
                                   &so_err, &len) < 0) {
                        คืน -1;
                    }
                    ถ้า (so_err == 0) {
                        คืน 0;
                    } มิฉะนั้น {
                        errno = so_err;
                        คืน -1;
                    }
                } มิฉะนั้น ถ้า (press == 0) {
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    continue;
                } มิฉะนั้น {
                    ถ้า (errno == EINTR) {
                        continue;
                    }
                    /* Non-EINTR poll error; bail out after a few retries */
                    ถ้า (++poll_retries > 3) {
                        คืน -1;
                    }
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    continue;
                }
            }
        }

        ถ้า (errno == EINTR) {
            คืน -1;
        }

        คืน -1;
    }
}

จำนวนเต็ม freertos_linux_coop_accept(จำนวนเต็ม sockfd, sa_nracanaa sockaddr *addr, socklen_t *addrlen)
{
    ขณะ (1)
    {
        จำนวนเต็ม fd = real_accept(sockfd, addr, addrlen);
        ถ้า (fd >= 0) {
            linux_coop_set_nonblocking(fd);
            linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(sockfd) ? O_NONBLOCK : 0);
            คืน fd;
        }
        ถ้า (errno == EAGAIN || errno == EWOULDBLOCK) {
            ถ้า (linux_coop_fd_user_nonblocking(sockfd)) {
                คืน -1;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            continue;
        }
        คืน -1;
    }
}

จำนวนเต็ม freertos_linux_coop_pselect(จำนวนเต็ม nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, const sa_nracanaa timespec *timeout,
            const sigset_t *sigmask)
{
    long timeout_ms = -1;
    ถ้า (timeout != NULL) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_nsec + 999999) / 1000000;
        ถ้า (timeout_ms == 0) {
            คืน real_pselect(nfds, readfds, writefds, exceptfds,
                                timeout, sigmask);
        }
    }

    long waited_ms = 0;
    ขณะ (1)
    {
        sa_nracanaa timespec zero_ts = {0, 0};
        จำนวนเต็ม ret = real_pselect(nfds, readfds, writefds, exceptfds,
                               &zero_ts, sigmask);
        ถ้า (ret != 0) {
            คืน ret;
        }

        ถ้า (timeout_ms == 0 ||
            (timeout_ms > 0 && waited_ms >= timeout_ms)) {
            คืน 0;
        }

        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

จำนวนเต็ม freertos_linux_coop_poll(sa_nracanaa pollfd *fds, nfds_t nfds, จำนวนเต็ม timeout)
{
    ถ้า (timeout == 0) {
        คืน real_poll(fds, nfds, 0);
    }

    long timeout_ms = -1;
    ถ้า (timeout > 0) {
        timeout_ms = timeout;
    }

    long waited_ms = 0;

    ขณะ (1)
    {
        จำนวนเต็ม ret = real_poll(fds, nfds, 0);
        ถ้า (ret > 0) {
            คืน ret;
        } มิฉะนั้น ถ้า (ret == 0) {
            ถ้า (timeout_ms == 0 ||
                (timeout_ms > 0 && waited_ms >= timeout_ms)) {
                คืน 0;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            waited_ms += LINUX_COOP_TICK_MS;
            continue;
        } มิฉะนั้น {
            ถ้า (errno == EINTR) {
                continue;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            ถ้า (timeout_ms > 0 && waited_ms >= timeout_ms) {
                คืน -1;
            }
            waited_ms += LINUX_COOP_TICK_MS;
        }
    }
}

จำนวนเต็ม nanosleep(const sa_nracanaa timespec *req, sa_nracanaa timespec *rem)
{
    ถ้า (req == NULL) {
        errno = EFAULT;
        คืน -1;
    }
    ถ้า (!linux_port_in_freertos_task()) {
        คืน real_nanosleep(req, rem);
    }
    long ms = req->tv_sec * 1000 + (req->tv_nsec + 999999) / 1000000;
    ถ้า (ms <= 0) {
        ms = 1;
    }
    vTaskDelay(ms);
    ถ้า (rem) {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }
    คืน 0;
}

unsigned จำนวนเต็ม sleep(unsigned จำนวนเต็ม seconds)
{
    linux_coop_yield(seconds * 1000);
    คืน 0;
}

จำนวนเต็ม usleep(useconds_t usec)
{
    linux_coop_yield(usec / 1000);
    คืน 0;
}

จำนวนเต็ม freertos_linux_coop_socket(จำนวนเต็ม domain, จำนวนเต็ม type, จำนวนเต็ม protocol)
{
    จำนวนเต็ม fd = real_socket(domain, type, protocol);
    linux_coop_set_nonblocking(fd);
#ifdef SOCK_NONBLOCK
    const จำนวนเต็ม user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#มิฉะนั้น
    const จำนวนเต็ม user_flags = 0;
#endif
    linux_coop_track_fd(fd, user_flags);
    คืน fd;
}

จำนวนเต็ม freertos_linux_coop_socketpair(จำนวนเต็ม domain, จำนวนเต็ม type, จำนวนเต็ม protocol, จำนวนเต็ม *sv)
{
    จำนวนเต็ม ret = real_socketpair(domain, type, protocol, sv);
    ถ้า (ret == 0) {
        linux_coop_set_nonblocking(sv[0]);
        linux_coop_set_nonblocking(sv[1]);
#ifdef SOCK_NONBLOCK
        const จำนวนเต็ม user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#มิฉะนั้น
        const จำนวนเต็ม user_flags = 0;
#endif
        linux_coop_track_fd(sv[0], user_flags);
        linux_coop_track_fd(sv[1], user_flags);
    }
    คืน ret;
}

จำนวนเต็ม freertos_linux_coop_pipe(จำนวนเต็ม *fds)
{
    จำนวนเต็ม ret = real_pipe(fds);
    ถ้า (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        linux_coop_track_fd(fds[0], 0);
        linux_coop_track_fd(fds[1], 0);
    }
    คืน ret;
}

จำนวนเต็ม freertos_linux_coop_pipe2(จำนวนเต็ม *fds, จำนวนเต็ม flags)
{
    จำนวนเต็ม ret = real_pipe2(fds, flags);
    ถ้า (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        const จำนวนเต็ม user_flags = (flags & O_NONBLOCK) ? O_NONBLOCK : 0;
        linux_coop_track_fd(fds[0], user_flags);
        linux_coop_track_fd(fds[1], user_flags);
    }
    คืน ret;
}

จำนวนเต็ม freertos_linux_coop_dup(จำนวนเต็ม oldfd)
{
    จำนวนเต็ม fd = real_dup(oldfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    คืน fd;
}

จำนวนเต็ม freertos_linux_coop_dup2(จำนวนเต็ม oldfd, จำนวนเต็ม newfd)
{
    จำนวนเต็ม fd = real_dup2(oldfd, newfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    คืน fd;
}

__attribute__((weak)) จำนวนเต็ม connect(จำนวนเต็ม sockfd, const sa_nracanaa sockaddr *addr, socklen_t addrlen)
{
    คืน freertos_linux_coop_connect(sockfd, addr, addrlen);
}

__attribute__((weak)) จำนวนเต็ม accept(จำนวนเต็ม sockfd, sa_nracanaa sockaddr *addr, socklen_t *addrlen)
{
    คืน freertos_linux_coop_accept(sockfd, addr, addrlen);
}

__attribute__((weak)) จำนวนเต็ม pselect(จำนวนเต็ม nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, const sa_nracanaa timespec *timeout,
            const sigset_t *sigmask)
{
    คืน freertos_linux_coop_pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}

__attribute__((weak)) จำนวนเต็ม poll(sa_nracanaa pollfd *fds, nfds_t nfds, จำนวนเต็ม timeout)
{
    คืน freertos_linux_coop_poll(fds, nfds, timeout);
}

__attribute__((weak)) จำนวนเต็ม socket(จำนวนเต็ม domain, จำนวนเต็ม type, จำนวนเต็ม protocol)
{
    คืน freertos_linux_coop_socket(domain, type, protocol);
}

__attribute__((weak)) จำนวนเต็ม socketpair(จำนวนเต็ม domain, จำนวนเต็ม type, จำนวนเต็ม protocol, จำนวนเต็ม sv[2])
{
    คืน freertos_linux_coop_socketpair(domain, type, protocol, sv);
}

__attribute__((weak)) จำนวนเต็ม pipe(จำนวนเต็ม fds[2])
{
    คืน freertos_linux_coop_pipe(fds);
}

__attribute__((weak)) จำนวนเต็ม pipe2(จำนวนเต็ม fds[2], จำนวนเต็ม flags)
{
    คืน freertos_linux_coop_pipe2(fds, flags);
}

__attribute__((weak)) จำนวนเต็ม dup(จำนวนเต็ม oldfd)
{
    คืน freertos_linux_coop_dup(oldfd);
}

__attribute__((weak)) จำนวนเต็ม dup2(จำนวนเต็ม oldfd, จำนวนเต็ม newfd)
{
    คืน freertos_linux_coop_dup2(oldfd, newfd);
}
