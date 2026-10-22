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

ایستا ssize_t  (*real_readv)(صحیح, ثابت ساختار iovec *, صحیح);
ایستا ssize_t  (*real_writev)(صحیح, ثابت ساختار iovec *, صحیح);
ایستا ssize_t  (*real_recv)(صحیح, تهی *, size_t, صحیح);
ایستا ssize_t  (*real_send)(صحیح, ثابت تهی *, size_t, صحیح);
ایستا ssize_t  (*real_recvfrom)(صحیح, تهی *, size_t, صحیح,
                                 ساختار sockaddr *, socklen_t *);
ایستا ssize_t  (*real_sendto)(صحیح, ثابت تهی *, size_t, صحیح,
                                ثابت ساختار sockaddr *, socklen_t);
ایستا ssize_t  (*real_recvmsg)(صحیح, ساختار msghdr *, صحیح);
ایستا ssize_t  (*real_sendmsg)(صحیح, ثابت ساختار msghdr *, صحیح);
ایستا صحیح      (*real_connect)(صحیح, ثابت ساختار sockaddr *, socklen_t);
ایستا صحیح      (*real_accept)(صحیح, ساختار sockaddr *, socklen_t *);
ایستا صحیح      (*real_pselect)(صحیح, fd_set *, fd_set *, fd_set *,
                                ثابت ساختار timespec *, ثابت sigset_t *);
ایستا صحیح      (*real_poll)(ساختار pollfd *, nfds_t, صحیح);
ایستا صحیح      (*real_socket)(صحیح, صحیح, صحیح);
ایستا صحیح      (*real_socketpair)(صحیح, صحیح, صحیح, صحیح[2]);
ایستا صحیح      (*real_pipe)(صحیح[2]);
ایستا صحیح      (*real_pipe2)(صحیح[2], صحیح);
ایستا صحیح      (*real_dup)(صحیح);
ایستا صحیح      (*real_dup2)(صحیح, صحیح);
ایستا صحیح      (*real_nanosleep)(ثابت ساختار timespec *, ساختار timespec *);
ایستا ssize_t  (*real_read)(صحیح, تهی *, size_t);
ایستا ssize_t  (*real_write)(صحیح, ثابت تهی *, size_t);
ایستا ssize_t  (*real_pread)(صحیح, تهی *, size_t, off_t);
ایستا ssize_t  (*real_pwrite)(صحیح, ثابت تهی *, size_t, off_t);
ایستا صحیح      (*real_close)(صحیح);
ایستا صحیح      (*real_select)(صحیح, fd_set *, fd_set *, fd_set *,
                                ساختار timeval *);

تعریفنوع صحیح (*linux_coop_fcntl_fn_t)(صحیح, صحیح, ...);
ایستا linux_coop_fcntl_fn_t real_fcntl;

تعریفنوع صحیح (*linux_coop_open_fn_t)(ثابت نویسه *, صحیح, mode_t);
ایستا linux_coop_open_fn_t real_open;

تعریفنوع ساختار linux_coop_fd_state {
    صحیح fd;
    صحیح user_flags;
    ساختار linux_coop_fd_state *next;
} linux_coop_fd_state_t;

ایستا linux_coop_fd_state_t *s_fd_state_list;
ایستا pthread_mutex_t s_fd_state_lock = PTHREAD_MUTEX_INITIALIZER;

ایستا linux_coop_fd_state_t *linux_coop_find_fd_locked(صحیح fd)
{
    linux_coop_fd_state_t *گره = s_fd_state_list;
    وقتی (گره != پوچ) {
        اگر (گره->fd == fd) {
            بازگشت گره;
        }
        گره = گره->next;
    }
    بازگشت پوچ;
}

ایستا تهی __attribute__((constructor)) linux_coop_resolve_all(تهی)
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

تهی linux_coop_set_nonblocking(صحیح fd)
{
    اگر (fd >= 0) {
        صحیح flags = real_fcntl(fd, F_GETFL, 0);
        اگر (flags >= 0) {
            real_fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
    }
}

تهی linux_coop_track_fd(صحیح fd, صحیح user_flags)
{
    اگر (fd < 0) {
        بازگشت;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *گره = linux_coop_find_fd_locked(fd);
    اگر (گره != پوچ) {
        گره->user_flags = user_flags;
        pthread_mutex_unlock(&s_fd_state_lock);
        بازگشت;
    }

    گره = calloc(1, اندازه(*گره));
    اگر (گره != پوچ) {
        گره->fd = fd;
        گره->user_flags = user_flags;
        گره->next = s_fd_state_list;
        s_fd_state_list = گره;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

تهی linux_coop_untrack_fd(صحیح fd)
{
    اگر (fd < 0) {
        بازگشت;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *prev = پوچ;
    linux_coop_fd_state_t *گره = s_fd_state_list;
    وقتی (گره != پوچ) {
        اگر (گره->fd == fd) {
            اگر (prev == پوچ) {
                s_fd_state_list = گره->next;
            } وگرنه {
                prev->next = گره->next;
            }
            آزاد(گره);
            بشکن;
        }
        prev = گره;
        گره = گره->next;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

تهی linux_coop_set_user_flags(صحیح fd, صحیح user_flags)
{
    linux_coop_track_fd(fd, user_flags);
}

bool linux_coop_get_user_flags(صحیح fd, صحیح *flags)
{
    اگر (fd < 0 || flags == پوچ) {
        بازگشت false;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *گره = linux_coop_find_fd_locked(fd);
    اگر (گره == پوچ) {
        pthread_mutex_unlock(&s_fd_state_lock);
        بازگشت false;
    }
    *flags = گره->user_flags;
    pthread_mutex_unlock(&s_fd_state_lock);
    بازگشت true;
}

bool linux_coop_fd_user_nonblocking(صحیح fd)
{
    صحیح user_flags = 0;
    اگر (linux_coop_get_user_flags(fd, &user_flags)) {
        بازگشت (user_flags & O_NONBLOCK) != 0;
    }
    بازگشت false;
}


ssize_t freertos_linux_coop_read(صحیح fd, تهی *buf, size_t شمار)
{
    LINUX_COOP_IO_LOOP(fd, real_read(fd, buf, شمار))
}

ssize_t freertos_linux_coop_write(صحیح fd, ثابت تهی *buf, size_t شمار)
{
    LINUX_COOP_IO_LOOP(fd, real_write(fd, buf, شمار))
}

ssize_t freertos_linux_coop_pread(صحیح fd, تهی *buf, size_t شمار, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pread(fd, buf, شمار, offset))
}

ssize_t freertos_linux_coop_pwrite(صحیح fd, ثابت تهی *buf, size_t شمار, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pwrite(fd, buf, شمار, offset))
}

صحیح freertos_linux_coop_open(ثابت نویسه *path, صحیح flags, صحیح mode)
{
    صحیح fd = real_open(path, flags, (mode_t)mode);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, flags & O_NONBLOCK);
    بازگشت fd;
}

صحیح freertos_linux_coop_close(صحیح fd)
{
    صحیح ret = real_close(fd);
    اگر (ret == 0 || errno == EINTR) {
        /* On Linux, close() always releases the fd even when returning
         * EINTR.  Retrying would risk closing a fd reused by another
         * thread.  Treat EINTR as success. */
        linux_coop_untrack_fd(fd);
        بازگشت 0;
    }
    بازگشت -1;
}

صحیح freertos_linux_coop_fcntl(صحیح fd, صحیح cmd, صحیح arg)
{
    اگر (cmd == F_GETFL) {
        صحیح real_flags = real_fcntl(fd, F_GETFL, 0);
        اگر (real_flags < 0) {
            بازگشت -1;
        }
        صحیح user_flags = 0;
        اگر (linux_coop_get_user_flags(fd, &user_flags)) {
            real_flags &= ~O_NONBLOCK;
            real_flags |= (user_flags & O_NONBLOCK);
        }
        بازگشت real_flags;
    }
    اگر (cmd == F_SETFL) {
        linux_coop_set_user_flags(fd, arg);
        صحیح internal_flags = arg | O_NONBLOCK;
        بازگشت real_fcntl(fd, F_SETFL, internal_flags);
    }
    بازگشت real_fcntl(fd, cmd, arg);
}

صحیح freertos_linux_coop_select(صحیح nfds, fd_set *readfds, fd_set *writefds,
                      fd_set *exceptfds, ساختار timeval *timeout)
{
    long timeout_ms = -1;
    اگر (timeout != پوچ) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_usec + 999) / 1000;
        اگر (timeout_ms == 0) {
            بازگشت real_select(nfds, readfds, writefds, exceptfds, timeout);
        }
    }

    /* real_select() mutates the fd_sets in place (clears them, then sets bits
     * for ready descriptors). To keep polling the same descriptors across
     * iterations we must snapshot the caller's sets and restore them on every
     * loop. */
    fd_set rfds_in, wfds_in, efds_in;
    اگر (readfds != پوچ) {
        rfds_in = *readfds;
    }
    اگر (writefds != پوچ) {
        wfds_in = *writefds;
    }
    اگر (exceptfds != پوچ) {
        efds_in = *exceptfds;
    }

    long waited_ms = 0;
    وقتی (1) {
        اگر (readfds != پوچ) {
            *readfds = rfds_in;
        }
        اگر (writefds != پوچ) {
            *writefds = wfds_in;
        }
        اگر (exceptfds != پوچ) {
            *exceptfds = efds_in;
        }

        ساختار timeval zero_tv = {0, 0};
        صحیح ret = real_select(nfds, readfds, writefds, exceptfds, &zero_tv);
        اگر (ret != 0) {
            بازگشت ret;
        }
        اگر (timeout_ms == 0) {
            بازگشت 0;
        }
        اگر (timeout_ms > 0 && waited_ms >= timeout_ms) {
            بازگشت 0;
        }
        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

تهی freertos_linux_coop_syscalls_init(تهی)
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

__attribute__((weak)) ssize_t اقرأ(صحیح fd, تهی *buf, size_t شمار)
{
    بازگشت freertos_linux_coop_read(fd, buf, شمار);
}

__attribute__((weak)) ssize_t اكتب(صحیح fd, ثابت تهی *buf, size_t شمار)
{
    بازگشت freertos_linux_coop_write(fd, buf, شمار);
}

__attribute__((weak)) ssize_t pread(صحیح fd, تهی *buf, size_t شمار, off_t offset)
{
    بازگشت freertos_linux_coop_pread(fd, buf, شمار, offset);
}

__attribute__((weak)) ssize_t pwrite(صحیح fd, ثابت تهی *buf, size_t شمار, off_t offset)
{
    بازگشت freertos_linux_coop_pwrite(fd, buf, شمار, offset);
}

__attribute__((weak)) صحیح open(ثابت نویسه *path, صحیح flags, ...)
{
    صحیح mode = 0;
    اگر (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        mode = va_arg(ap, صحیح);
        va_end(ap);
    }
    بازگشت freertos_linux_coop_open(path, flags, mode);
}

__attribute__((weak)) صحیح close(صحیح fd)
{
    بازگشت freertos_linux_coop_close(fd);
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
    بازگشت freertos_linux_coop_fcntl(fd, cmd, arg);
}

__attribute__((weak)) صحیح select(صحیح nfds, fd_set *readfds, fd_set *writefds,
                                 fd_set *exceptfds, ساختار timeval *timeout)
{
    بازگشت freertos_linux_coop_select(nfds, readfds, writefds, exceptfds, timeout);
}

ssize_t freertos_linux_coop_readv(صحیح fd, ثابت ساختار iovec *iov, صحیح iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_readv(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_writev(صحیح fd, ثابت ساختار iovec *iov, صحیح iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_writev(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_recv(صحیح sockfd, تهی *buf, size_t len, صحیح flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recv(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_send(صحیح sockfd, ثابت تهی *buf, size_t len, صحیح flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_send(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_recvfrom(صحیح sockfd, تهی *buf, size_t len, صحیح flags,
                 ساختار sockaddr *src_addr, socklen_t *addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvfrom(sockfd, buf, len, flags | MSG_DONTWAIT,
                            src_addr, addrlen))
}

ssize_t freertos_linux_coop_sendto(صحیح sockfd, ثابت تهی *buf, size_t len, صحیح flags,
               ثابت ساختار sockaddr *dest_addr, socklen_t addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendto(sockfd, buf, len, flags | MSG_DONTWAIT,
                          dest_addr, addrlen))
}

ssize_t freertos_linux_coop_recvmsg(صحیح sockfd, ساختار msghdr *پیام, صحیح flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvmsg(sockfd, پیام, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_sendmsg(صحیح sockfd, ثابت ساختار msghdr *پیام, صحیح flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendmsg(sockfd, پیام, flags | MSG_DONTWAIT))
}

__attribute__((weak)) ssize_t readv(صحیح fd, ثابت ساختار iovec *iov, صحیح iovcnt)
{
    بازگشت freertos_linux_coop_readv(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t writev(صحیح fd, ثابت ساختار iovec *iov, صحیح iovcnt)
{
    بازگشت freertos_linux_coop_writev(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t recv(صحیح sockfd, تهی *buf, size_t len, صحیح flags)
{
    بازگشت freertos_linux_coop_recv(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t send(صحیح sockfd, ثابت تهی *buf, size_t len, صحیح flags)
{
    بازگشت freertos_linux_coop_send(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t recvfrom(صحیح sockfd, تهی *buf, size_t len, صحیح flags,
                 ساختار sockaddr *src_addr, socklen_t *addrlen)
{
    بازگشت freertos_linux_coop_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

__attribute__((weak)) ssize_t sendto(صحیح sockfd, ثابت تهی *buf, size_t len, صحیح flags,
               ثابت ساختار sockaddr *dest_addr, socklen_t addrlen)
{
    بازگشت freertos_linux_coop_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

__attribute__((weak)) ssize_t recvmsg(صحیح sockfd, ساختار msghdr *پیام, صحیح flags)
{
    بازگشت freertos_linux_coop_recvmsg(sockfd, پیام, flags);
}

__attribute__((weak)) ssize_t sendmsg(صحیح sockfd, ثابت ساختار msghdr *پیام, صحیح flags)
{
    بازگشت freertos_linux_coop_sendmsg(sockfd, پیام, flags);
}

صحیح freertos_linux_coop_connect(صحیح sockfd, ثابت ساختار sockaddr *addr, socklen_t addrlen)
{
    وقتی (1)
    {
        صحیح ret = real_connect(sockfd, addr, addrlen);
        اگر (ret == 0) {
            بازگشت ret;
        }

        اگر (errno == EINPROGRESS || errno == EALREADY) {
            ساختار pollfd pfd;
            pfd.fd = sockfd;
            pfd.events = POLLOUT;
            pfd.revents = 0;

            صحیح poll_retries = 0;
            وقتی (1)
            {
                صحیح press = real_poll(&pfd, 1, 0);
                اگر (press > 0) {
                    صحیح so_err = 0;
                    socklen_t len = اندازه(so_err);
                    اگر (getsockopt(sockfd, SOL_SOCKET, SO_ERROR,
                                   &so_err, &len) < 0) {
                        بازگشت -1;
                    }
                    اگر (so_err == 0) {
                        بازگشت 0;
                    } وگرنه {
                        errno = so_err;
                        بازگشت -1;
                    }
                } وگرنه اگر (press == 0) {
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    ادامه;
                } وگرنه {
                    اگر (errno == EINTR) {
                        ادامه;
                    }
                    /* Non-EINTR poll error; bail out after a few retries */
                    اگر (++poll_retries > 3) {
                        بازگشت -1;
                    }
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    ادامه;
                }
            }
        }

        اگر (errno == EINTR) {
            بازگشت -1;
        }

        بازگشت -1;
    }
}

صحیح freertos_linux_coop_accept(صحیح sockfd, ساختار sockaddr *addr, socklen_t *addrlen)
{
    وقتی (1)
    {
        صحیح fd = real_accept(sockfd, addr, addrlen);
        اگر (fd >= 0) {
            linux_coop_set_nonblocking(fd);
            linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(sockfd) ? O_NONBLOCK : 0);
            بازگشت fd;
        }
        اگر (errno == EAGAIN || errno == EWOULDBLOCK) {
            اگر (linux_coop_fd_user_nonblocking(sockfd)) {
                بازگشت -1;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            ادامه;
        }
        بازگشت -1;
    }
}

صحیح freertos_linux_coop_pselect(صحیح nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, ثابت ساختار timespec *timeout,
            ثابت sigset_t *sigmask)
{
    long timeout_ms = -1;
    اگر (timeout != پوچ) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_nsec + 999999) / 1000000;
        اگر (timeout_ms == 0) {
            بازگشت real_pselect(nfds, readfds, writefds, exceptfds,
                                timeout, sigmask);
        }
    }

    long waited_ms = 0;
    وقتی (1)
    {
        ساختار timespec zero_ts = {0, 0};
        صحیح ret = real_pselect(nfds, readfds, writefds, exceptfds,
                               &zero_ts, sigmask);
        اگر (ret != 0) {
            بازگشت ret;
        }

        اگر (timeout_ms == 0 ||
            (timeout_ms > 0 && waited_ms >= timeout_ms)) {
            بازگشت 0;
        }

        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

صحیح freertos_linux_coop_poll(ساختار pollfd *fds, nfds_t nfds, صحیح timeout)
{
    اگر (timeout == 0) {
        بازگشت real_poll(fds, nfds, 0);
    }

    long timeout_ms = -1;
    اگر (timeout > 0) {
        timeout_ms = timeout;
    }

    long waited_ms = 0;

    وقتی (1)
    {
        صحیح ret = real_poll(fds, nfds, 0);
        اگر (ret > 0) {
            بازگشت ret;
        } وگرنه اگر (ret == 0) {
            اگر (timeout_ms == 0 ||
                (timeout_ms > 0 && waited_ms >= timeout_ms)) {
                بازگشت 0;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            waited_ms += LINUX_COOP_TICK_MS;
            ادامه;
        } وگرنه {
            اگر (errno == EINTR) {
                ادامه;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            اگر (timeout_ms > 0 && waited_ms >= timeout_ms) {
                بازگشت -1;
            }
            waited_ms += LINUX_COOP_TICK_MS;
        }
    }
}

صحیح nanosleep(ثابت ساختار timespec *req, ساختار timespec *rem)
{
    اگر (req == پوچ) {
        errno = EFAULT;
        بازگشت -1;
    }
    اگر (!linux_port_in_freertos_task()) {
        بازگشت real_nanosleep(req, rem);
    }
    long ms = req->tv_sec * 1000 + (req->tv_nsec + 999999) / 1000000;
    اگر (ms <= 0) {
        ms = 1;
    }
    vTaskDelay(ms);
    اگر (rem) {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }
    بازگشت 0;
}

unsigned صحیح sleep(unsigned صحیح seconds)
{
    linux_coop_yield(seconds * 1000);
    بازگشت 0;
}

صحیح usleep(useconds_t usec)
{
    linux_coop_yield(usec / 1000);
    بازگشت 0;
}

صحیح freertos_linux_coop_socket(صحیح domain, صحیح type, صحیح protocol)
{
    صحیح fd = real_socket(domain, type, protocol);
    linux_coop_set_nonblocking(fd);
#ifdef SOCK_NONBLOCK
    ثابت صحیح user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#وگرنه
    ثابت صحیح user_flags = 0;
#endif
    linux_coop_track_fd(fd, user_flags);
    بازگشت fd;
}

صحیح freertos_linux_coop_socketpair(صحیح domain, صحیح type, صحیح protocol, صحیح *sv)
{
    صحیح ret = real_socketpair(domain, type, protocol, sv);
    اگر (ret == 0) {
        linux_coop_set_nonblocking(sv[0]);
        linux_coop_set_nonblocking(sv[1]);
#ifdef SOCK_NONBLOCK
        ثابت صحیح user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#وگرنه
        ثابت صحیح user_flags = 0;
#endif
        linux_coop_track_fd(sv[0], user_flags);
        linux_coop_track_fd(sv[1], user_flags);
    }
    بازگشت ret;
}

صحیح freertos_linux_coop_pipe(صحیح *fds)
{
    صحیح ret = real_pipe(fds);
    اگر (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        linux_coop_track_fd(fds[0], 0);
        linux_coop_track_fd(fds[1], 0);
    }
    بازگشت ret;
}

صحیح freertos_linux_coop_pipe2(صحیح *fds, صحیح flags)
{
    صحیح ret = real_pipe2(fds, flags);
    اگر (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        ثابت صحیح user_flags = (flags & O_NONBLOCK) ? O_NONBLOCK : 0;
        linux_coop_track_fd(fds[0], user_flags);
        linux_coop_track_fd(fds[1], user_flags);
    }
    بازگشت ret;
}

صحیح freertos_linux_coop_dup(صحیح oldfd)
{
    صحیح fd = real_dup(oldfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    بازگشت fd;
}

صحیح freertos_linux_coop_dup2(صحیح oldfd, صحیح newfd)
{
    صحیح fd = real_dup2(oldfd, newfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    بازگشت fd;
}

__attribute__((weak)) صحیح connect(صحیح sockfd, ثابت ساختار sockaddr *addr, socklen_t addrlen)
{
    بازگشت freertos_linux_coop_connect(sockfd, addr, addrlen);
}

__attribute__((weak)) صحیح accept(صحیح sockfd, ساختار sockaddr *addr, socklen_t *addrlen)
{
    بازگشت freertos_linux_coop_accept(sockfd, addr, addrlen);
}

__attribute__((weak)) صحیح pselect(صحیح nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, ثابت ساختار timespec *timeout,
            ثابت sigset_t *sigmask)
{
    بازگشت freertos_linux_coop_pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}

__attribute__((weak)) صحیح poll(ساختار pollfd *fds, nfds_t nfds, صحیح timeout)
{
    بازگشت freertos_linux_coop_poll(fds, nfds, timeout);
}

__attribute__((weak)) صحیح socket(صحیح domain, صحیح type, صحیح protocol)
{
    بازگشت freertos_linux_coop_socket(domain, type, protocol);
}

__attribute__((weak)) صحیح socketpair(صحیح domain, صحیح type, صحیح protocol, صحیح sv[2])
{
    بازگشت freertos_linux_coop_socketpair(domain, type, protocol, sv);
}

__attribute__((weak)) صحیح pipe(صحیح fds[2])
{
    بازگشت freertos_linux_coop_pipe(fds);
}

__attribute__((weak)) صحیح pipe2(صحیح fds[2], صحیح flags)
{
    بازگشت freertos_linux_coop_pipe2(fds, flags);
}

__attribute__((weak)) صحیح dup(صحیح oldfd)
{
    بازگشت freertos_linux_coop_dup(oldfd);
}

__attribute__((weak)) صحیح dup2(صحیح oldfd, صحیح newfd)
{
    بازگشت freertos_linux_coop_dup2(oldfd, newfd);
}
