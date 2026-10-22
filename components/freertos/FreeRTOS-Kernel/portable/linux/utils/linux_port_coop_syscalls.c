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

#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <string.h>
#شامل <errno.h>
#شامل <fcntl.h>
#شامل <stdarg.h>
#شامل <unistd.h>
#شامل <sys/types.h>
#شامل <sys/uio.h>
#شامل <sys/socket.h>
#شامل <sys/select.h>
#شامل <sys/time.h>
#شامل <time.h>
#شامل <poll.h>
#شامل <dlfcn.h>
#شامل <pthread.h>

#شامل "linux_port_coop_internal.h"
#شامل "esp_private/freertos_linux_coop_syscalls.h"

ساکن ssize_t  (*real_readv)(صحیح, ثابت ساخت iovec *, صحیح);
ساکن ssize_t  (*real_writev)(صحیح, ثابت ساخت iovec *, صحیح);
ساکن ssize_t  (*real_recv)(صحیح, خالی *, size_t, صحیح);
ساکن ssize_t  (*real_send)(صحیح, ثابت خالی *, size_t, صحیح);
ساکن ssize_t  (*real_recvfrom)(صحیح, خالی *, size_t, صحیح,
                                 ساخت sockaddr *, socklen_t *);
ساکن ssize_t  (*real_sendto)(صحیح, ثابت خالی *, size_t, صحیح,
                                ثابت ساخت sockaddr *, socklen_t);
ساکن ssize_t  (*real_recvmsg)(صحیح, ساخت msghdr *, صحیح);
ساکن ssize_t  (*real_sendmsg)(صحیح, ثابت ساخت msghdr *, صحیح);
ساکن صحیح      (*real_connect)(صحیح, ثابت ساخت sockaddr *, socklen_t);
ساکن صحیح      (*real_accept)(صحیح, ساخت sockaddr *, socklen_t *);
ساکن صحیح      (*real_pselect)(صحیح, fd_set *, fd_set *, fd_set *,
                                ثابت ساخت timespec *, ثابت sigset_t *);
ساکن صحیح      (*real_poll)(ساخت pollfd *, nfds_t, صحیح);
ساکن صحیح      (*real_socket)(صحیح, صحیح, صحیح);
ساکن صحیح      (*real_socketpair)(صحیح, صحیح, صحیح, صحیح[2]);
ساکن صحیح      (*real_pipe)(صحیح[2]);
ساکن صحیح      (*real_pipe2)(صحیح[2], صحیح);
ساکن صحیح      (*real_dup)(صحیح);
ساکن صحیح      (*real_dup2)(صحیح, صحیح);
ساکن صحیح      (*real_nanosleep)(ثابت ساخت timespec *, ساخت timespec *);
ساکن ssize_t  (*real_read)(صحیح, خالی *, size_t);
ساکن ssize_t  (*real_write)(صحیح, ثابت خالی *, size_t);
ساکن ssize_t  (*real_pread)(صحیح, خالی *, size_t, off_t);
ساکن ssize_t  (*real_pwrite)(صحیح, ثابت خالی *, size_t, off_t);
ساکن صحیح      (*real_close)(صحیح);
ساکن صحیح      (*real_select)(صحیح, fd_set *, fd_set *, fd_set *,
                                ساخت timeval *);

قسمتعریف صحیح (*linux_coop_fcntl_fn_t)(صحیح, صحیح, ...);
ساکن linux_coop_fcntl_fn_t real_fcntl;

قسمتعریف صحیح (*linux_coop_open_fn_t)(ثابت حرف *, صحیح, mode_t);
ساکن linux_coop_open_fn_t real_open;

قسمتعریف ساخت linux_coop_fd_state {
    صحیح fd;
    صحیح user_flags;
    ساخت linux_coop_fd_state *اگلا;
} linux_coop_fd_state_t;

ساکن linux_coop_fd_state_t *s_fd_state_list;
ساکن pthread_mutex_t s_fd_state_تالا = PTHREAD_MUTEX_INITIALIZER;

ساکن linux_coop_fd_state_t *linux_coop_ڈھونڈو_fd_locked(صحیح fd)
{
    linux_coop_fd_state_t *گرہ = s_fd_state_list;
    جب (گرہ != تہی) {
        اگر (گرہ->fd == fd) {
            واپس گرہ;
        }
        گرہ = گرہ->اگلا;
    }
    واپس تہی;
}

ساکن خالی __attribute__((constructor)) linux_coop_resolve_all(خالی)
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
    LINUX_COOP_RESOLVE(اقرأ);
    LINUX_COOP_RESOLVE(اكتب);
    LINUX_COOP_RESOLVE(pread);
    LINUX_COOP_RESOLVE(pwrite);
    LINUX_COOP_RESOLVE(close);
    LINUX_COOP_RESOLVE(select);
    LINUX_COOP_RESOLVE(open);
}

خالی linux_coop_سیٹ_nonblocking(صحیح fd)
{
    اگر (fd >= 0) {
        صحیح flags = real_fcntl(fd, F_GETFL, 0);
        اگر (flags >= 0) {
            real_fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
    }
}

خالی linux_coop_track_fd(صحیح fd, صحیح user_flags)
{
    اگر (fd < 0) {
        واپس;
    }

    pthread_mutex_lock(&s_fd_state_تالا);
    linux_coop_fd_state_t *گرہ = linux_coop_ڈھونڈو_fd_locked(fd);
    اگر (گرہ != تہی) {
        گرہ->user_flags = user_flags;
        pthread_mutex_unlock(&s_fd_state_تالا);
        واپس;
    }

    گرہ = صفر_حافظہ_دو(1, حجم(*گرہ));
    اگر (گرہ != تہی) {
        گرہ->fd = fd;
        گرہ->user_flags = user_flags;
        گرہ->اگلا = s_fd_state_list;
        s_fd_state_list = گرہ;
    }
    pthread_mutex_unlock(&s_fd_state_تالا);
}

خالی linux_coop_untrack_fd(صحیح fd)
{
    اگر (fd < 0) {
        واپس;
    }

    pthread_mutex_lock(&s_fd_state_تالا);
    linux_coop_fd_state_t *پچھلا = تہی;
    linux_coop_fd_state_t *گرہ = s_fd_state_list;
    جب (گرہ != تہی) {
        اگر (گرہ->fd == fd) {
            اگر (پچھلا == تہی) {
                s_fd_state_list = گرہ->اگلا;
            } ورنہ {
                پچھلا->اگلا = گرہ->اگلا;
            }
            آزاد(گرہ);
            توڑو;
        }
        پچھلا = گرہ;
        گرہ = گرہ->اگلا;
    }
    pthread_mutex_unlock(&s_fd_state_تالا);
}

خالی linux_coop_سیٹ_user_flags(صحیح fd, صحیح user_flags)
{
    linux_coop_track_fd(fd, user_flags);
}

bool linux_coop_get_user_flags(صحیح fd, صحیح *flags)
{
    اگر (fd < 0 || flags == تہی) {
        واپس false;
    }

    pthread_mutex_lock(&s_fd_state_تالا);
    linux_coop_fd_state_t *گرہ = linux_coop_ڈھونڈو_fd_locked(fd);
    اگر (گرہ == تہی) {
        pthread_mutex_unlock(&s_fd_state_تالا);
        واپس false;
    }
    *flags = گرہ->user_flags;
    pthread_mutex_unlock(&s_fd_state_تالا);
    واپس true;
}

bool linux_coop_fd_user_nonblocking(صحیح fd)
{
    صحیح user_flags = 0;
    اگر (linux_coop_get_user_flags(fd, &user_flags)) {
        واپس (user_flags & O_NONBLOCK) != 0;
    }
    واپس false;
}


ssize_t freertos_linux_coop_read(صحیح fd, خالی *buf, size_t گنتی)
{
    LINUX_COOP_IO_LOOP(fd, real_read(fd, buf, گنتی))
}

ssize_t freertos_linux_coop_write(صحیح fd, ثابت خالی *buf, size_t گنتی)
{
    LINUX_COOP_IO_LOOP(fd, real_write(fd, buf, گنتی))
}

ssize_t freertos_linux_coop_pread(صحیح fd, خالی *buf, size_t گنتی, off_t انحراف)
{
    LINUX_COOP_IO_LOOP(fd, real_pread(fd, buf, گنتی, انحراف))
}

ssize_t freertos_linux_coop_pwrite(صحیح fd, ثابت خالی *buf, size_t گنتی, off_t انحراف)
{
    LINUX_COOP_IO_LOOP(fd, real_pwrite(fd, buf, گنتی, انحراف))
}

صحیح freertos_linux_coop_open(ثابت حرف *راستہ, صحیح flags, صحیح منوال)
{
    صحیح fd = real_open(راستہ, flags, (mode_t)منوال);
    linux_coop_سیٹ_nonblocking(fd);
    linux_coop_track_fd(fd, flags & O_NONBLOCK);
    واپس fd;
}

صحیح freertos_linux_coop_close(صحیح fd)
{
    صحیح ret = real_close(fd);
    اگر (ret == 0 || errno == EINTR) {
        /* On Linux, close() always releases the fd even when returning
         * EINTR.  Retrying would risk closing a fd reused by another
         * thread.  Treat EINTR as success. */
        linux_coop_untrack_fd(fd);
        واپس 0;
    }
    واپس -1;
}

صحیح freertos_linux_coop_fcntl(صحیح fd, صحیح cmd, صحیح arg)
{
    اگر (cmd == F_GETFL) {
        صحیح real_flags = real_fcntl(fd, F_GETFL, 0);
        اگر (real_flags < 0) {
            واپس -1;
        }
        صحیح user_flags = 0;
        اگر (linux_coop_get_user_flags(fd, &user_flags)) {
            real_flags &= ~O_NONBLOCK;
            real_flags |= (user_flags & O_NONBLOCK);
        }
        واپس real_flags;
    }
    اگر (cmd == F_SETFL) {
        linux_coop_سیٹ_user_flags(fd, arg);
        صحیح اندرونی_flags = arg | O_NONBLOCK;
        واپس real_fcntl(fd, F_SETFL, اندرونی_flags);
    }
    واپس real_fcntl(fd, cmd, arg);
}

صحیح freertos_linux_coop_select(صحیح nfds, fd_set *readfds, fd_set *writefds,
                      fd_set *exceptfds, ساخت timeval *timeout)
{
    طویل timeout_ms = -1;
    اگر (timeout != تہی) {
        timeout_ms = (طویل)timeout->tv_sec * 1000 +
                     (timeout->tv_usec + 999) / 1000;
        اگر (timeout_ms == 0) {
            واپس real_select(nfds, readfds, writefds, exceptfds, timeout);
        }
    }

    /* real_select() mutates the fd_sets in place (clears them, then sets bits
     * for ready descriptors). To keep polling the same descriptors across
     * iterations we must snapshot the caller's sets and restore them on every
     * loop. */
    fd_set rfds_اندر, wfds_اندر, efds_اندر;
    اگر (readfds != تہی) {
        rfds_اندر = *readfds;
    }
    اگر (writefds != تہی) {
        wfds_اندر = *writefds;
    }
    اگر (exceptfds != تہی) {
        efds_اندر = *exceptfds;
    }

    طویل waited_ms = 0;
    جب (1) {
        اگر (readfds != تہی) {
            *readfds = rfds_اندر;
        }
        اگر (writefds != تہی) {
            *writefds = wfds_اندر;
        }
        اگر (exceptfds != تہی) {
            *exceptfds = efds_اندر;
        }

        ساخت timeval zero_tv = {0, 0};
        صحیح ret = real_select(nfds, readfds, writefds, exceptfds, &zero_tv);
        اگر (ret != 0) {
            واپس ret;
        }
        اگر (timeout_ms == 0) {
            واپس 0;
        }
        اگر (timeout_ms > 0 && waited_ms >= timeout_ms) {
            واپس 0;
        }
        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

خالی freertos_linux_coop_syscalls_init(خالی)
{
    linux_coop_سیٹ_nonblocking(STDIN_FILENO);
    linux_coop_سیٹ_nonblocking(STDOUT_FILENO);
    linux_coop_سیٹ_nonblocking(STDERR_FILENO);
    linux_coop_track_fd(STDIN_FILENO, 0);
    linux_coop_track_fd(STDOUT_FILENO, 0);
    linux_coop_track_fd(STDERR_FILENO, 0);
}

/* These are overridden by strong definitions in VFS when that component
 * is linked.  When VFS is absent the weak versions provide cooperative
 * behaviour directly via real libc. */

__attribute__((weak)) ssize_t اقرأ(صحیح fd, خالی *buf, size_t گنتی)
{
    واپس freertos_linux_coop_read(fd, buf, گنتی);
}

__attribute__((weak)) ssize_t اكتب(صحیح fd, ثابت خالی *buf, size_t گنتی)
{
    واپس freertos_linux_coop_write(fd, buf, گنتی);
}

__attribute__((weak)) ssize_t pread(صحیح fd, خالی *buf, size_t گنتی, off_t انحراف)
{
    واپس freertos_linux_coop_pread(fd, buf, گنتی, انحراف);
}

__attribute__((weak)) ssize_t pwrite(صحیح fd, ثابت خالی *buf, size_t گنتی, off_t انحراف)
{
    واپس freertos_linux_coop_pwrite(fd, buf, گنتی, انحراف);
}

__attribute__((weak)) صحیح open(ثابت حرف *راستہ, صحیح flags, ...)
{
    صحیح منوال = 0;
    اگر (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        منوال = va_arg(ap, صحیح);
        va_end(ap);
    }
    واپس freertos_linux_coop_open(راستہ, flags, منوال);
}

__attribute__((weak)) صحیح close(صحیح fd)
{
    واپس freertos_linux_coop_close(fd);
}

__attribute__((weak)) صحیح fcntl(صحیح fd, صحیح cmd, ...)
{
    صحیح arg = 0;
    اگر (cmd != F_GETFL) {
        va_list list;
        va_start(list, cmd);
        arg = va_arg(list, صحیح);
        va_end(list);
    }
    واپس freertos_linux_coop_fcntl(fd, cmd, arg);
}

__attribute__((weak)) صحیح select(صحیح nfds, fd_set *readfds, fd_set *writefds,
                                 fd_set *exceptfds, ساخت timeval *timeout)
{
    واپس freertos_linux_coop_select(nfds, readfds, writefds, exceptfds, timeout);
}

ssize_t freertos_linux_coop_readv(صحیح fd, ثابت ساخت iovec *iov, صحیح iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_readv(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_writev(صحیح fd, ثابت ساخت iovec *iov, صحیح iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_writev(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_recv(صحیح sockfd, خالی *buf, size_t len, صحیح flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recv(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_send(صحیح sockfd, ثابت خالی *buf, size_t len, صحیح flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_send(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_recvfrom(صحیح sockfd, خالی *buf, size_t len, صحیح flags,
                 ساخت sockaddr *src_addr, socklen_t *addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvfrom(sockfd, buf, len, flags | MSG_DONTWAIT,
                            src_addr, addrlen))
}

ssize_t freertos_linux_coop_sendto(صحیح sockfd, ثابت خالی *buf, size_t len, صحیح flags,
               ثابت ساخت sockaddr *dest_addr, socklen_t addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendto(sockfd, buf, len, flags | MSG_DONTWAIT,
                          dest_addr, addrlen))
}

ssize_t freertos_linux_coop_recvmsg(صحیح sockfd, ساخت msghdr *پیغام, صحیح flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvmsg(sockfd, پیغام, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_sendmsg(صحیح sockfd, ثابت ساخت msghdr *پیغام, صحیح flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendmsg(sockfd, پیغام, flags | MSG_DONTWAIT))
}

__attribute__((weak)) ssize_t readv(صحیح fd, ثابت ساخت iovec *iov, صحیح iovcnt)
{
    واپس freertos_linux_coop_readv(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t writev(صحیح fd, ثابت ساخت iovec *iov, صحیح iovcnt)
{
    واپس freertos_linux_coop_writev(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t recv(صحیح sockfd, خالی *buf, size_t len, صحیح flags)
{
    واپس freertos_linux_coop_recv(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t send(صحیح sockfd, ثابت خالی *buf, size_t len, صحیح flags)
{
    واپس freertos_linux_coop_send(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t recvfrom(صحیح sockfd, خالی *buf, size_t len, صحیح flags,
                 ساخت sockaddr *src_addr, socklen_t *addrlen)
{
    واپس freertos_linux_coop_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

__attribute__((weak)) ssize_t sendto(صحیح sockfd, ثابت خالی *buf, size_t len, صحیح flags,
               ثابت ساخت sockaddr *dest_addr, socklen_t addrlen)
{
    واپس freertos_linux_coop_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

__attribute__((weak)) ssize_t recvmsg(صحیح sockfd, ساخت msghdr *پیغام, صحیح flags)
{
    واپس freertos_linux_coop_recvmsg(sockfd, پیغام, flags);
}

__attribute__((weak)) ssize_t sendmsg(صحیح sockfd, ثابت ساخت msghdr *پیغام, صحیح flags)
{
    واپس freertos_linux_coop_sendmsg(sockfd, پیغام, flags);
}

صحیح freertos_linux_coop_connect(صحیح sockfd, ثابت ساخت sockaddr *addr, socklen_t addrlen)
{
    جب (1)
    {
        صحیح ret = real_connect(sockfd, addr, addrlen);
        اگر (ret == 0) {
            واپس ret;
        }

        اگر (errno == EINPROGRESS || errno == EALREADY) {
            ساخت pollfd pfd;
            pfd.fd = sockfd;
            pfd.events = POLLOUT;
            pfd.revents = 0;

            صحیح poll_retries = 0;
            جب (1)
            {
                صحیح press = real_poll(&pfd, 1, 0);
                اگر (press > 0) {
                    صحیح so_err = 0;
                    socklen_t len = حجم(so_err);
                    اگر (getsockopt(sockfd, SOL_SOCKET, SO_ERROR,
                                   &so_err, &len) < 0) {
                        واپس -1;
                    }
                    اگر (so_err == 0) {
                        واپس 0;
                    } ورنہ {
                        errno = so_err;
                        واپس -1;
                    }
                } ورنہ اگر (press == 0) {
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    جاری;
                } ورنہ {
                    اگر (errno == EINTR) {
                        جاری;
                    }
                    /* Non-EINTR poll error; bail out after a few retries */
                    اگر (++poll_retries > 3) {
                        واپس -1;
                    }
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    جاری;
                }
            }
        }

        اگر (errno == EINTR) {
            واپس -1;
        }

        واپس -1;
    }
}

صحیح freertos_linux_coop_accept(صحیح sockfd, ساخت sockaddr *addr, socklen_t *addrlen)
{
    جب (1)
    {
        صحیح fd = real_accept(sockfd, addr, addrlen);
        اگر (fd >= 0) {
            linux_coop_سیٹ_nonblocking(fd);
            linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(sockfd) ? O_NONBLOCK : 0);
            واپس fd;
        }
        اگر (errno == EAGAIN || errno == EWOULDBLOCK) {
            اگر (linux_coop_fd_user_nonblocking(sockfd)) {
                واپس -1;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            جاری;
        }
        واپس -1;
    }
}

صحیح freertos_linux_coop_pselect(صحیح nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, ثابت ساخت timespec *timeout,
            ثابت sigset_t *sigmask)
{
    طویل timeout_ms = -1;
    اگر (timeout != تہی) {
        timeout_ms = (طویل)timeout->tv_sec * 1000 +
                     (timeout->tv_nsec + 999999) / 1000000;
        اگر (timeout_ms == 0) {
            واپس real_pselect(nfds, readfds, writefds, exceptfds,
                                timeout, sigmask);
        }
    }

    طویل waited_ms = 0;
    جب (1)
    {
        ساخت timespec zero_ts = {0, 0};
        صحیح ret = real_pselect(nfds, readfds, writefds, exceptfds,
                               &zero_ts, sigmask);
        اگر (ret != 0) {
            واپس ret;
        }

        اگر (timeout_ms == 0 ||
            (timeout_ms > 0 && waited_ms >= timeout_ms)) {
            واپس 0;
        }

        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

صحیح freertos_linux_coop_poll(ساخت pollfd *fds, nfds_t nfds, صحیح timeout)
{
    اگر (timeout == 0) {
        واپس real_poll(fds, nfds, 0);
    }

    طویل timeout_ms = -1;
    اگر (timeout > 0) {
        timeout_ms = timeout;
    }

    طویل waited_ms = 0;

    جب (1)
    {
        صحیح ret = real_poll(fds, nfds, 0);
        اگر (ret > 0) {
            واپس ret;
        } ورنہ اگر (ret == 0) {
            اگر (timeout_ms == 0 ||
                (timeout_ms > 0 && waited_ms >= timeout_ms)) {
                واپس 0;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            waited_ms += LINUX_COOP_TICK_MS;
            جاری;
        } ورنہ {
            اگر (errno == EINTR) {
                جاری;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            اگر (timeout_ms > 0 && waited_ms >= timeout_ms) {
                واپس -1;
            }
            waited_ms += LINUX_COOP_TICK_MS;
        }
    }
}

صحیح nanosleep(ثابت ساخت timespec *req, ساخت timespec *rem)
{
    اگر (req == تہی) {
        errno = EFAULT;
        واپس -1;
    }
    اگر (!linux_پورٹ_اندر_freertos_task()) {
        واپس real_nanosleep(req, rem);
    }
    طویل ms = req->tv_sec * 1000 + (req->tv_nsec + 999999) / 1000000;
    اگر (ms <= 0) {
        ms = 1;
    }
    vTaskDelay(ms);
    اگر (rem) {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }
    واپس 0;
}

غیرعلامتی صحیح sleep(غیرعلامتی صحیح seconds)
{
    linux_coop_yield(seconds * 1000);
    واپس 0;
}

صحیح usleep(useconds_t usec)
{
    linux_coop_yield(usec / 1000);
    واپس 0;
}

صحیح freertos_linux_coop_socket(صحیح domain, صحیح type, صحیح protocol)
{
    صحیح fd = real_socket(domain, type, protocol);
    linux_coop_سیٹ_nonblocking(fd);
#اگر_تعریف_شدہ SOCK_NONBLOCK
    ثابت صحیح user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#ورنہ
    ثابت صحیح user_flags = 0;
#اگر_ختم
    linux_coop_track_fd(fd, user_flags);
    واپس fd;
}

صحیح freertos_linux_coop_socketpair(صحیح domain, صحیح type, صحیح protocol, صحیح *sv)
{
    صحیح ret = real_socketpair(domain, type, protocol, sv);
    اگر (ret == 0) {
        linux_coop_سیٹ_nonblocking(sv[0]);
        linux_coop_سیٹ_nonblocking(sv[1]);
#اگر_تعریف_شدہ SOCK_NONBLOCK
        ثابت صحیح user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#ورنہ
        ثابت صحیح user_flags = 0;
#اگر_ختم
        linux_coop_track_fd(sv[0], user_flags);
        linux_coop_track_fd(sv[1], user_flags);
    }
    واپس ret;
}

صحیح freertos_linux_coop_pipe(صحیح *fds)
{
    صحیح ret = real_pipe(fds);
    اگر (ret == 0) {
        linux_coop_سیٹ_nonblocking(fds[0]);
        linux_coop_سیٹ_nonblocking(fds[1]);
        linux_coop_track_fd(fds[0], 0);
        linux_coop_track_fd(fds[1], 0);
    }
    واپس ret;
}

صحیح freertos_linux_coop_pipe2(صحیح *fds, صحیح flags)
{
    صحیح ret = real_pipe2(fds, flags);
    اگر (ret == 0) {
        linux_coop_سیٹ_nonblocking(fds[0]);
        linux_coop_سیٹ_nonblocking(fds[1]);
        ثابت صحیح user_flags = (flags & O_NONBLOCK) ? O_NONBLOCK : 0;
        linux_coop_track_fd(fds[0], user_flags);
        linux_coop_track_fd(fds[1], user_flags);
    }
    واپس ret;
}

صحیح freertos_linux_coop_dup(صحیح oldfd)
{
    صحیح fd = real_dup(oldfd);
    linux_coop_سیٹ_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    واپس fd;
}

صحیح freertos_linux_coop_dup2(صحیح oldfd, صحیح newfd)
{
    صحیح fd = real_dup2(oldfd, newfd);
    linux_coop_سیٹ_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    واپس fd;
}

__attribute__((weak)) صحیح connect(صحیح sockfd, ثابت ساخت sockaddr *addr, socklen_t addrlen)
{
    واپس freertos_linux_coop_connect(sockfd, addr, addrlen);
}

__attribute__((weak)) صحیح accept(صحیح sockfd, ساخت sockaddr *addr, socklen_t *addrlen)
{
    واپس freertos_linux_coop_accept(sockfd, addr, addrlen);
}

__attribute__((weak)) صحیح pselect(صحیح nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, ثابت ساخت timespec *timeout,
            ثابت sigset_t *sigmask)
{
    واپس freertos_linux_coop_pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}

__attribute__((weak)) صحیح poll(ساخت pollfd *fds, nfds_t nfds, صحیح timeout)
{
    واپس freertos_linux_coop_poll(fds, nfds, timeout);
}

__attribute__((weak)) صحیح socket(صحیح domain, صحیح type, صحیح protocol)
{
    واپس freertos_linux_coop_socket(domain, type, protocol);
}

__attribute__((weak)) صحیح socketpair(صحیح domain, صحیح type, صحیح protocol, صحیح sv[2])
{
    واپس freertos_linux_coop_socketpair(domain, type, protocol, sv);
}

__attribute__((weak)) صحیح pipe(صحیح fds[2])
{
    واپس freertos_linux_coop_pipe(fds);
}

__attribute__((weak)) صحیح pipe2(صحیح fds[2], صحیح flags)
{
    واپس freertos_linux_coop_pipe2(fds, flags);
}

__attribute__((weak)) صحیح dup(صحیح oldfd)
{
    واپس freertos_linux_coop_dup(oldfd);
}

__attribute__((weak)) صحیح dup2(صحیح oldfd, صحیح newfd)
{
    واپس freertos_linux_coop_dup2(oldfd, newfd);
}
