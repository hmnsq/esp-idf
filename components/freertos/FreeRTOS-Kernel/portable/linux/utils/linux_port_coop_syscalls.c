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

#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <string.h>
#ضمن <errno.h>
#ضمن <fcntl.h>
#ضمن <stdarg.h>
#ضمن <unistd.h>
#ضمن <sys/types.h>
#ضمن <sys/uio.h>
#ضمن <sys/socket.h>
#ضمن <sys/select.h>
#ضمن <sys/time.h>
#ضمن <time.h>
#ضمن <poll.h>
#ضمن <dlfcn.h>
#ضمن <pthread.h>

#ضمن "linux_port_coop_internal.h"
#ضمن "esp_private/freertos_linux_coop_syscalls.h"

ساكن ssize_t  (*real_readv)(صحيح, ثابت بنية iovec *, صحيح);
ساكن ssize_t  (*real_writev)(صحيح, ثابت بنية iovec *, صحيح);
ساكن ssize_t  (*real_recv)(صحيح, فراغ *, size_t, صحيح);
ساكن ssize_t  (*real_send)(صحيح, ثابت فراغ *, size_t, صحيح);
ساكن ssize_t  (*real_recvfrom)(صحيح, فراغ *, size_t, صحيح,
                                 بنية sockaddr *, socklen_t *);
ساكن ssize_t  (*real_sendto)(صحيح, ثابت فراغ *, size_t, صحيح,
                                ثابت بنية sockaddr *, socklen_t);
ساكن ssize_t  (*real_recvmsg)(صحيح, بنية msghdr *, صحيح);
ساكن ssize_t  (*real_sendmsg)(صحيح, ثابت بنية msghdr *, صحيح);
ساكن صحيح      (*real_connect)(صحيح, ثابت بنية sockaddr *, socklen_t);
ساكن صحيح      (*real_accept)(صحيح, بنية sockaddr *, socklen_t *);
ساكن صحيح      (*real_pselect)(صحيح, fd_set *, fd_set *, fd_set *,
                                ثابت بنية timespec *, ثابت sigset_t *);
ساكن صحيح      (*real_poll)(بنية pollfd *, nfds_t, صحيح);
ساكن صحيح      (*real_socket)(صحيح, صحيح, صحيح);
ساكن صحيح      (*real_socketpair)(صحيح, صحيح, صحيح, صحيح[2]);
ساكن صحيح      (*real_pipe)(صحيح[2]);
ساكن صحيح      (*real_pipe2)(صحيح[2], صحيح);
ساكن صحيح      (*real_dup)(صحيح);
ساكن صحيح      (*real_dup2)(صحيح, صحيح);
ساكن صحيح      (*real_nanosleep)(ثابت بنية timespec *, بنية timespec *);
ساكن ssize_t  (*real_read)(صحيح, فراغ *, size_t);
ساكن ssize_t  (*real_write)(صحيح, ثابت فراغ *, size_t);
ساكن ssize_t  (*real_pread)(صحيح, فراغ *, size_t, off_t);
ساكن ssize_t  (*real_pwrite)(صحيح, ثابت فراغ *, size_t, off_t);
ساكن صحيح      (*real_close)(صحيح);
ساكن صحيح      (*real_select)(صحيح, fd_set *, fd_set *, fd_set *,
                                بنية timeval *);

تعريفنوع صحيح (*linux_coop_fcntl_fn_t)(صحيح, صحيح, ...);
ساكن linux_coop_fcntl_fn_t real_fcntl;

تعريفنوع صحيح (*linux_coop_open_fn_t)(ثابت حرف *, صحيح, mode_t);
ساكن linux_coop_open_fn_t real_open;

تعريفنوع بنية linux_coop_fd_state {
    صحيح fd;
    صحيح user_flags;
    بنية linux_coop_fd_state *next;
} linux_coop_fd_state_t;

ساكن linux_coop_fd_state_t *s_fd_state_list;
ساكن pthread_mutex_t s_fd_state_lock = PTHREAD_MUTEX_INITIALIZER;

ساكن linux_coop_fd_state_t *linux_coop_find_fd_locked(صحيح fd)
{
    linux_coop_fd_state_t *تۈگۈن = s_fd_state_list;
    طالما (تۈگۈن != عدم) {
        إذا (تۈگۈن->fd == fd) {
            أرجع تۈگۈن;
        }
        تۈگۈن = تۈگۈن->next;
    }
    أرجع عدم;
}

ساكن فراغ __attribute__((constructor)) linux_coop_resolve_all(فراغ)
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

فراغ linux_coop_set_nonblocking(صحيح fd)
{
    إذا (fd >= 0) {
        صحيح flags = real_fcntl(fd, F_GETFL, 0);
        إذا (flags >= 0) {
            real_fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
    }
}

فراغ linux_coop_track_fd(صحيح fd, صحيح user_flags)
{
    إذا (fd < 0) {
        أرجع;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *تۈگۈن = linux_coop_find_fd_locked(fd);
    إذا (تۈگۈن != عدم) {
        تۈگۈن->user_flags = user_flags;
        pthread_mutex_unlock(&s_fd_state_lock);
        أرجع;
    }

    تۈگۈن = calloc(1, حجم(*تۈگۈن));
    إذا (تۈگۈن != عدم) {
        تۈگۈن->fd = fd;
        تۈگۈن->user_flags = user_flags;
        تۈگۈن->next = s_fd_state_list;
        s_fd_state_list = تۈگۈن;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

فراغ linux_coop_untrack_fd(صحيح fd)
{
    إذا (fd < 0) {
        أرجع;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *prev = عدم;
    linux_coop_fd_state_t *تۈگۈن = s_fd_state_list;
    طالما (تۈگۈن != عدم) {
        إذا (تۈگۈن->fd == fd) {
            إذا (prev == عدم) {
                s_fd_state_list = تۈگۈن->next;
            } وإلا {
                prev->next = تۈگۈن->next;
            }
            حرر(تۈگۈن);
            اكسر;
        }
        prev = تۈگۈن;
        تۈگۈن = تۈگۈن->next;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

فراغ linux_coop_set_user_flags(صحيح fd, صحيح user_flags)
{
    linux_coop_track_fd(fd, user_flags);
}

bool linux_coop_get_user_flags(صحيح fd, صحيح *flags)
{
    إذا (fd < 0 || flags == عدم) {
        أرجع false;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *تۈگۈن = linux_coop_find_fd_locked(fd);
    إذا (تۈگۈن == عدم) {
        pthread_mutex_unlock(&s_fd_state_lock);
        أرجع false;
    }
    *flags = تۈگۈن->user_flags;
    pthread_mutex_unlock(&s_fd_state_lock);
    أرجع true;
}

bool linux_coop_fd_user_nonblocking(صحيح fd)
{
    صحيح user_flags = 0;
    إذا (linux_coop_get_user_flags(fd, &user_flags)) {
        أرجع (user_flags & O_NONBLOCK) != 0;
    }
    أرجع false;
}


ssize_t freertos_linux_coop_read(صحيح fd, فراغ *buf, size_t سانى)
{
    LINUX_COOP_IO_LOOP(fd, real_read(fd, buf, سانى))
}

ssize_t freertos_linux_coop_write(صحيح fd, ثابت فراغ *buf, size_t سانى)
{
    LINUX_COOP_IO_LOOP(fd, real_write(fd, buf, سانى))
}

ssize_t freertos_linux_coop_pread(صحيح fd, فراغ *buf, size_t سانى, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pread(fd, buf, سانى, offset))
}

ssize_t freertos_linux_coop_pwrite(صحيح fd, ثابت فراغ *buf, size_t سانى, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pwrite(fd, buf, سانى, offset))
}

صحيح freertos_linux_coop_open(ثابت حرف *path, صحيح flags, صحيح mode)
{
    صحيح fd = real_open(path, flags, (mode_t)mode);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, flags & O_NONBLOCK);
    أرجع fd;
}

صحيح freertos_linux_coop_close(صحيح fd)
{
    صحيح ret = real_close(fd);
    إذا (ret == 0 || errno == EINTR) {
        /* On Linux, close() always releases the fd even when returning
         * EINTR.  Retrying would risk closing a fd reused by another
         * thread.  Treat EINTR as success. */
        linux_coop_untrack_fd(fd);
        أرجع 0;
    }
    أرجع -1;
}

صحيح freertos_linux_coop_fcntl(صحيح fd, صحيح cmd, صحيح arg)
{
    إذا (cmd == F_GETFL) {
        صحيح real_flags = real_fcntl(fd, F_GETFL, 0);
        إذا (real_flags < 0) {
            أرجع -1;
        }
        صحيح user_flags = 0;
        إذا (linux_coop_get_user_flags(fd, &user_flags)) {
            real_flags &= ~O_NONBLOCK;
            real_flags |= (user_flags & O_NONBLOCK);
        }
        أرجع real_flags;
    }
    إذا (cmd == F_SETFL) {
        linux_coop_set_user_flags(fd, arg);
        صحيح internal_flags = arg | O_NONBLOCK;
        أرجع real_fcntl(fd, F_SETFL, internal_flags);
    }
    أرجع real_fcntl(fd, cmd, arg);
}

صحيح freertos_linux_coop_select(صحيح nfds, fd_set *readfds, fd_set *writefds,
                      fd_set *exceptfds, بنية timeval *timeout)
{
    long timeout_ms = -1;
    إذا (timeout != عدم) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_usec + 999) / 1000;
        إذا (timeout_ms == 0) {
            أرجع real_select(nfds, readfds, writefds, exceptfds, timeout);
        }
    }

    /* real_select() mutates the fd_sets in place (clears them, then sets bits
     * for ready descriptors). To keep polling the same descriptors across
     * iterations we must snapshot the caller's sets and restore them on every
     * loop. */
    fd_set rfds_in, wfds_in, efds_in;
    إذا (readfds != عدم) {
        rfds_in = *readfds;
    }
    إذا (writefds != عدم) {
        wfds_in = *writefds;
    }
    إذا (exceptfds != عدم) {
        efds_in = *exceptfds;
    }

    long waited_ms = 0;
    طالما (1) {
        إذا (readfds != عدم) {
            *readfds = rfds_in;
        }
        إذا (writefds != عدم) {
            *writefds = wfds_in;
        }
        إذا (exceptfds != عدم) {
            *exceptfds = efds_in;
        }

        بنية timeval zero_tv = {0, 0};
        صحيح ret = real_select(nfds, readfds, writefds, exceptfds, &zero_tv);
        إذا (ret != 0) {
            أرجع ret;
        }
        إذا (timeout_ms == 0) {
            أرجع 0;
        }
        إذا (timeout_ms > 0 && waited_ms >= timeout_ms) {
            أرجع 0;
        }
        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

فراغ freertos_linux_coop_syscalls_init(فراغ)
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

__attribute__((weak)) ssize_t اقرأ(صحيح fd, فراغ *buf, size_t سانى)
{
    أرجع freertos_linux_coop_read(fd, buf, سانى);
}

__attribute__((weak)) ssize_t اكتب(صحيح fd, ثابت فراغ *buf, size_t سانى)
{
    أرجع freertos_linux_coop_write(fd, buf, سانى);
}

__attribute__((weak)) ssize_t pread(صحيح fd, فراغ *buf, size_t سانى, off_t offset)
{
    أرجع freertos_linux_coop_pread(fd, buf, سانى, offset);
}

__attribute__((weak)) ssize_t pwrite(صحيح fd, ثابت فراغ *buf, size_t سانى, off_t offset)
{
    أرجع freertos_linux_coop_pwrite(fd, buf, سانى, offset);
}

__attribute__((weak)) صحيح open(ثابت حرف *path, صحيح flags, ...)
{
    صحيح mode = 0;
    إذا (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        mode = va_arg(ap, صحيح);
        va_end(ap);
    }
    أرجع freertos_linux_coop_open(path, flags, mode);
}

__attribute__((weak)) صحيح close(صحيح fd)
{
    أرجع freertos_linux_coop_close(fd);
}

__attribute__((weak)) صحيح fcntl(صحيح fd, صحيح cmd, ...)
{
    صحيح arg = 0;
    إذا (cmd != F_GETFL) {
        va_list list;
        va_start(list, cmd);
        arg = va_arg(list, صحيح);
        va_end(list);
    }
    أرجع freertos_linux_coop_fcntl(fd, cmd, arg);
}

__attribute__((weak)) صحيح select(صحيح nfds, fd_set *readfds, fd_set *writefds,
                                 fd_set *exceptfds, بنية timeval *timeout)
{
    أرجع freertos_linux_coop_select(nfds, readfds, writefds, exceptfds, timeout);
}

ssize_t freertos_linux_coop_readv(صحيح fd, ثابت بنية iovec *iov, صحيح iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_readv(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_writev(صحيح fd, ثابت بنية iovec *iov, صحيح iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_writev(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_recv(صحيح sockfd, فراغ *buf, size_t len, صحيح flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recv(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_send(صحيح sockfd, ثابت فراغ *buf, size_t len, صحيح flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_send(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_recvfrom(صحيح sockfd, فراغ *buf, size_t len, صحيح flags,
                 بنية sockaddr *src_addr, socklen_t *addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvfrom(sockfd, buf, len, flags | MSG_DONTWAIT,
                            src_addr, addrlen))
}

ssize_t freertos_linux_coop_sendto(صحيح sockfd, ثابت فراغ *buf, size_t len, صحيح flags,
               ثابت بنية sockaddr *dest_addr, socklen_t addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendto(sockfd, buf, len, flags | MSG_DONTWAIT,
                          dest_addr, addrlen))
}

ssize_t freertos_linux_coop_recvmsg(صحيح sockfd, بنية msghdr *ئۇچۇر, صحيح flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvmsg(sockfd, ئۇچۇر, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_sendmsg(صحيح sockfd, ثابت بنية msghdr *ئۇچۇر, صحيح flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendmsg(sockfd, ئۇچۇر, flags | MSG_DONTWAIT))
}

__attribute__((weak)) ssize_t readv(صحيح fd, ثابت بنية iovec *iov, صحيح iovcnt)
{
    أرجع freertos_linux_coop_readv(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t writev(صحيح fd, ثابت بنية iovec *iov, صحيح iovcnt)
{
    أرجع freertos_linux_coop_writev(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t recv(صحيح sockfd, فراغ *buf, size_t len, صحيح flags)
{
    أرجع freertos_linux_coop_recv(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t send(صحيح sockfd, ثابت فراغ *buf, size_t len, صحيح flags)
{
    أرجع freertos_linux_coop_send(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t recvfrom(صحيح sockfd, فراغ *buf, size_t len, صحيح flags,
                 بنية sockaddr *src_addr, socklen_t *addrlen)
{
    أرجع freertos_linux_coop_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

__attribute__((weak)) ssize_t sendto(صحيح sockfd, ثابت فراغ *buf, size_t len, صحيح flags,
               ثابت بنية sockaddr *dest_addr, socklen_t addrlen)
{
    أرجع freertos_linux_coop_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

__attribute__((weak)) ssize_t recvmsg(صحيح sockfd, بنية msghdr *ئۇچۇر, صحيح flags)
{
    أرجع freertos_linux_coop_recvmsg(sockfd, ئۇچۇر, flags);
}

__attribute__((weak)) ssize_t sendmsg(صحيح sockfd, ثابت بنية msghdr *ئۇچۇر, صحيح flags)
{
    أرجع freertos_linux_coop_sendmsg(sockfd, ئۇچۇر, flags);
}

صحيح freertos_linux_coop_connect(صحيح sockfd, ثابت بنية sockaddr *addr, socklen_t addrlen)
{
    طالما (1)
    {
        صحيح ret = real_connect(sockfd, addr, addrlen);
        إذا (ret == 0) {
            أرجع ret;
        }

        إذا (errno == EINPROGRESS || errno == EALREADY) {
            بنية pollfd pfd;
            pfd.fd = sockfd;
            pfd.events = POLLOUT;
            pfd.revents = 0;

            صحيح poll_retries = 0;
            طالما (1)
            {
                صحيح press = real_poll(&pfd, 1, 0);
                إذا (press > 0) {
                    صحيح so_err = 0;
                    socklen_t len = حجم(so_err);
                    إذا (getsockopt(sockfd, SOL_SOCKET, SO_ERROR,
                                   &so_err, &len) < 0) {
                        أرجع -1;
                    }
                    إذا (so_err == 0) {
                        أرجع 0;
                    } وإلا {
                        errno = so_err;
                        أرجع -1;
                    }
                } وإلا إذا (press == 0) {
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    استمر;
                } وإلا {
                    إذا (errno == EINTR) {
                        استمر;
                    }
                    /* Non-EINTR poll error; bail out after a few retries */
                    إذا (++poll_retries > 3) {
                        أرجع -1;
                    }
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    استمر;
                }
            }
        }

        إذا (errno == EINTR) {
            أرجع -1;
        }

        أرجع -1;
    }
}

صحيح freertos_linux_coop_accept(صحيح sockfd, بنية sockaddr *addr, socklen_t *addrlen)
{
    طالما (1)
    {
        صحيح fd = real_accept(sockfd, addr, addrlen);
        إذا (fd >= 0) {
            linux_coop_set_nonblocking(fd);
            linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(sockfd) ? O_NONBLOCK : 0);
            أرجع fd;
        }
        إذا (errno == EAGAIN || errno == EWOULDBLOCK) {
            إذا (linux_coop_fd_user_nonblocking(sockfd)) {
                أرجع -1;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            استمر;
        }
        أرجع -1;
    }
}

صحيح freertos_linux_coop_pselect(صحيح nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, ثابت بنية timespec *timeout,
            ثابت sigset_t *sigmask)
{
    long timeout_ms = -1;
    إذا (timeout != عدم) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_nsec + 999999) / 1000000;
        إذا (timeout_ms == 0) {
            أرجع real_pselect(nfds, readfds, writefds, exceptfds,
                                timeout, sigmask);
        }
    }

    long waited_ms = 0;
    طالما (1)
    {
        بنية timespec zero_ts = {0, 0};
        صحيح ret = real_pselect(nfds, readfds, writefds, exceptfds,
                               &zero_ts, sigmask);
        إذا (ret != 0) {
            أرجع ret;
        }

        إذا (timeout_ms == 0 ||
            (timeout_ms > 0 && waited_ms >= timeout_ms)) {
            أرجع 0;
        }

        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

صحيح freertos_linux_coop_poll(بنية pollfd *fds, nfds_t nfds, صحيح timeout)
{
    إذا (timeout == 0) {
        أرجع real_poll(fds, nfds, 0);
    }

    long timeout_ms = -1;
    إذا (timeout > 0) {
        timeout_ms = timeout;
    }

    long waited_ms = 0;

    طالما (1)
    {
        صحيح ret = real_poll(fds, nfds, 0);
        إذا (ret > 0) {
            أرجع ret;
        } وإلا إذا (ret == 0) {
            إذا (timeout_ms == 0 ||
                (timeout_ms > 0 && waited_ms >= timeout_ms)) {
                أرجع 0;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            waited_ms += LINUX_COOP_TICK_MS;
            استمر;
        } وإلا {
            إذا (errno == EINTR) {
                استمر;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            إذا (timeout_ms > 0 && waited_ms >= timeout_ms) {
                أرجع -1;
            }
            waited_ms += LINUX_COOP_TICK_MS;
        }
    }
}

صحيح nanosleep(ثابت بنية timespec *req, بنية timespec *rem)
{
    إذا (req == عدم) {
        errno = EFAULT;
        أرجع -1;
    }
    إذا (!linux_port_in_freertos_task()) {
        أرجع real_nanosleep(req, rem);
    }
    long ms = req->tv_sec * 1000 + (req->tv_nsec + 999999) / 1000000;
    إذا (ms <= 0) {
        ms = 1;
    }
    vTaskDelay(ms);
    إذا (rem) {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }
    أرجع 0;
}

unsigned صحيح sleep(unsigned صحيح seconds)
{
    linux_coop_yield(seconds * 1000);
    أرجع 0;
}

صحيح usleep(useconds_t usec)
{
    linux_coop_yield(usec / 1000);
    أرجع 0;
}

صحيح freertos_linux_coop_socket(صحيح domain, صحيح type, صحيح protocol)
{
    صحيح fd = real_socket(domain, type, protocol);
    linux_coop_set_nonblocking(fd);
#ifdef SOCK_NONBLOCK
    ثابت صحيح user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#وإلا
    ثابت صحيح user_flags = 0;
#endif
    linux_coop_track_fd(fd, user_flags);
    أرجع fd;
}

صحيح freertos_linux_coop_socketpair(صحيح domain, صحيح type, صحيح protocol, صحيح *sv)
{
    صحيح ret = real_socketpair(domain, type, protocol, sv);
    إذا (ret == 0) {
        linux_coop_set_nonblocking(sv[0]);
        linux_coop_set_nonblocking(sv[1]);
#ifdef SOCK_NONBLOCK
        ثابت صحيح user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#وإلا
        ثابت صحيح user_flags = 0;
#endif
        linux_coop_track_fd(sv[0], user_flags);
        linux_coop_track_fd(sv[1], user_flags);
    }
    أرجع ret;
}

صحيح freertos_linux_coop_pipe(صحيح *fds)
{
    صحيح ret = real_pipe(fds);
    إذا (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        linux_coop_track_fd(fds[0], 0);
        linux_coop_track_fd(fds[1], 0);
    }
    أرجع ret;
}

صحيح freertos_linux_coop_pipe2(صحيح *fds, صحيح flags)
{
    صحيح ret = real_pipe2(fds, flags);
    إذا (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        ثابت صحيح user_flags = (flags & O_NONBLOCK) ? O_NONBLOCK : 0;
        linux_coop_track_fd(fds[0], user_flags);
        linux_coop_track_fd(fds[1], user_flags);
    }
    أرجع ret;
}

صحيح freertos_linux_coop_dup(صحيح oldfd)
{
    صحيح fd = real_dup(oldfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    أرجع fd;
}

صحيح freertos_linux_coop_dup2(صحيح oldfd, صحيح newfd)
{
    صحيح fd = real_dup2(oldfd, newfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    أرجع fd;
}

__attribute__((weak)) صحيح connect(صحيح sockfd, ثابت بنية sockaddr *addr, socklen_t addrlen)
{
    أرجع freertos_linux_coop_connect(sockfd, addr, addrlen);
}

__attribute__((weak)) صحيح accept(صحيح sockfd, بنية sockaddr *addr, socklen_t *addrlen)
{
    أرجع freertos_linux_coop_accept(sockfd, addr, addrlen);
}

__attribute__((weak)) صحيح pselect(صحيح nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, ثابت بنية timespec *timeout,
            ثابت sigset_t *sigmask)
{
    أرجع freertos_linux_coop_pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}

__attribute__((weak)) صحيح poll(بنية pollfd *fds, nfds_t nfds, صحيح timeout)
{
    أرجع freertos_linux_coop_poll(fds, nfds, timeout);
}

__attribute__((weak)) صحيح socket(صحيح domain, صحيح type, صحيح protocol)
{
    أرجع freertos_linux_coop_socket(domain, type, protocol);
}

__attribute__((weak)) صحيح socketpair(صحيح domain, صحيح type, صحيح protocol, صحيح sv[2])
{
    أرجع freertos_linux_coop_socketpair(domain, type, protocol, sv);
}

__attribute__((weak)) صحيح pipe(صحيح fds[2])
{
    أرجع freertos_linux_coop_pipe(fds);
}

__attribute__((weak)) صحيح pipe2(صحيح fds[2], صحيح flags)
{
    أرجع freertos_linux_coop_pipe2(fds, flags);
}

__attribute__((weak)) صحيح dup(صحيح oldfd)
{
    أرجع freertos_linux_coop_dup(oldfd);
}

__attribute__((weak)) صحيح dup2(صحيح oldfd, صحيح newfd)
{
    أرجع freertos_linux_coop_dup2(oldfd, newfd);
}
