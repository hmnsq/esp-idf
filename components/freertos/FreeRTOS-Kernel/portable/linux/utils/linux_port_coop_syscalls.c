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

#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <errno.h>
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <stdarg.h>
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <sys/uio.h>
#அந்தர்பாவ <sys/socket.h>
#அந்தர்பாவ <sys/select.h>
#அந்தர்பாவ <sys/time.h>
#அந்தர்பாவ <time.h>
#அந்தர்பாவ <poll.h>
#அந்தர்பாவ <dlfcn.h>
#அந்தர்பாவ <pthread.h>

#அந்தர்பாவ "linux_port_coop_internal.h"
#அந்தர்பாவ "esp_private/freertos_linux_coop_syscalls.h"

நிலையான ssize_t  (*real_readv)(முழு, மாறிலி கட்டமைப்பு iovec *, முழு);
நிலையான ssize_t  (*real_writev)(முழு, மாறிலி கட்டமைப்பு iovec *, முழு);
நிலையான ssize_t  (*real_recv)(முழு, வெறுமை *, size_t, முழு);
நிலையான ssize_t  (*real_send)(முழு, மாறிலி வெறுமை *, size_t, முழு);
நிலையான ssize_t  (*real_recvfrom)(முழு, வெறுமை *, size_t, முழு,
                                 கட்டமைப்பு sockaddr *, socklen_t *);
நிலையான ssize_t  (*real_sendto)(முழு, மாறிலி வெறுமை *, size_t, முழு,
                                மாறிலி கட்டமைப்பு sockaddr *, socklen_t);
நிலையான ssize_t  (*real_recvmsg)(முழு, கட்டமைப்பு msghdr *, முழு);
நிலையான ssize_t  (*real_sendmsg)(முழு, மாறிலி கட்டமைப்பு msghdr *, முழு);
நிலையான முழு      (*real_connect)(முழு, மாறிலி கட்டமைப்பு sockaddr *, socklen_t);
நிலையான முழு      (*real_accept)(முழு, கட்டமைப்பு sockaddr *, socklen_t *);
நிலையான முழு      (*real_pselect)(முழு, fd_set *, fd_set *, fd_set *,
                                மாறிலி கட்டமைப்பு timespec *, மாறிலி sigset_t *);
நிலையான முழு      (*real_poll)(கட்டமைப்பு pollfd *, nfds_t, முழு);
நிலையான முழு      (*real_socket)(முழு, முழு, முழு);
நிலையான முழு      (*real_socketpair)(முழு, முழு, முழு, முழு[2]);
நிலையான முழு      (*real_pipe)(முழு[2]);
நிலையான முழு      (*real_pipe2)(முழு[2], முழு);
நிலையான முழு      (*real_dup)(முழு);
நிலையான முழு      (*real_dup2)(முழு, முழு);
நிலையான முழு      (*real_nanosleep)(மாறிலி கட்டமைப்பு timespec *, கட்டமைப்பு timespec *);
நிலையான ssize_t  (*real_read)(முழு, வெறுமை *, size_t);
நிலையான ssize_t  (*real_write)(முழு, மாறிலி வெறுமை *, size_t);
நிலையான ssize_t  (*real_pread)(முழு, வெறுமை *, size_t, off_t);
நிலையான ssize_t  (*real_pwrite)(முழு, மாறிலி வெறுமை *, size_t, off_t);
நிலையான முழு      (*real_close)(முழு);
நிலையான முழு      (*real_select)(முழு, fd_set *, fd_set *, fd_set *,
                                கட்டமைப்பு timeval *);

வகைவரை முழு (*linux_coop_fcntl_fn_t)(முழு, முழு, ...);
நிலையான linux_coop_fcntl_fn_t real_fcntl;

வகைவரை முழு (*linux_coop_open_fn_t)(மாறிலி எழுத்து *, முழு, mode_t);
நிலையான linux_coop_open_fn_t real_open;

வகைவரை கட்டமைப்பு linux_coop_fd_state {
    முழு fd;
    முழு user_flags;
    கட்டமைப்பு linux_coop_fd_state *next;
} linux_coop_fd_state_t;

நிலையான linux_coop_fd_state_t *s_fd_state_list;
நிலையான pthread_mutex_t s_fd_state_lock = PTHREAD_MUTEX_INITIALIZER;

நிலையான linux_coop_fd_state_t *linux_coop_find_fd_locked(முழு fd)
{
    linux_coop_fd_state_t *முனை = s_fd_state_list;
    போது (முனை != வெற்று) {
        எனில் (முனை->fd == fd) {
            திரும்பு முனை;
        }
        முனை = முனை->next;
    }
    திரும்பு வெற்று;
}

நிலையான வெறுமை __attribute__((constructor)) linux_coop_resolve_all(வெறுமை)
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
    LINUX_COOP_RESOLVE(பட);
    LINUX_COOP_RESOLVE(லிக);
    LINUX_COOP_RESOLVE(pread);
    LINUX_COOP_RESOLVE(pwrite);
    LINUX_COOP_RESOLVE(close);
    LINUX_COOP_RESOLVE(select);
    LINUX_COOP_RESOLVE(open);
}

வெறுமை linux_coop_set_nonblocking(முழு fd)
{
    எனில் (fd >= 0) {
        முழு flags = real_fcntl(fd, F_GETFL, 0);
        எனில் (flags >= 0) {
            real_fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
    }
}

வெறுமை linux_coop_track_fd(முழு fd, முழு user_flags)
{
    எனில் (fd < 0) {
        திரும்பு;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *முனை = linux_coop_find_fd_locked(fd);
    எனில் (முனை != வெற்று) {
        முனை->user_flags = user_flags;
        pthread_mutex_unlock(&s_fd_state_lock);
        திரும்பு;
    }

    முனை = calloc(1, அளவு(*முனை));
    எனில் (முனை != வெற்று) {
        முனை->fd = fd;
        முனை->user_flags = user_flags;
        முனை->next = s_fd_state_list;
        s_fd_state_list = முனை;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

வெறுமை linux_coop_untrack_fd(முழு fd)
{
    எனில் (fd < 0) {
        திரும்பு;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *prev = வெற்று;
    linux_coop_fd_state_t *முனை = s_fd_state_list;
    போது (முனை != வெற்று) {
        எனில் (முனை->fd == fd) {
            எனில் (prev == வெற்று) {
                s_fd_state_list = முனை->next;
            } இல்லையெனில் {
                prev->next = முனை->next;
            }
            விடுவி(முனை);
            நிறுத்து;
        }
        prev = முனை;
        முனை = முனை->next;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

வெறுமை linux_coop_set_user_flags(முழு fd, முழு user_flags)
{
    linux_coop_track_fd(fd, user_flags);
}

bool linux_coop_get_user_flags(முழு fd, முழு *flags)
{
    எனில் (fd < 0 || flags == வெற்று) {
        திரும்பு false;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *முனை = linux_coop_find_fd_locked(fd);
    எனில் (முனை == வெற்று) {
        pthread_mutex_unlock(&s_fd_state_lock);
        திரும்பு false;
    }
    *flags = முனை->user_flags;
    pthread_mutex_unlock(&s_fd_state_lock);
    திரும்பு true;
}

bool linux_coop_fd_user_nonblocking(முழு fd)
{
    முழு user_flags = 0;
    எனில் (linux_coop_get_user_flags(fd, &user_flags)) {
        திரும்பு (user_flags & O_NONBLOCK) != 0;
    }
    திரும்பு false;
}


ssize_t freertos_linux_coop_read(முழு fd, வெறுமை *buf, size_t மொத்தம்)
{
    LINUX_COOP_IO_LOOP(fd, real_read(fd, buf, மொத்தம்))
}

ssize_t freertos_linux_coop_write(முழு fd, மாறிலி வெறுமை *buf, size_t மொத்தம்)
{
    LINUX_COOP_IO_LOOP(fd, real_write(fd, buf, மொத்தம்))
}

ssize_t freertos_linux_coop_pread(முழு fd, வெறுமை *buf, size_t மொத்தம், off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pread(fd, buf, மொத்தம், offset))
}

ssize_t freertos_linux_coop_pwrite(முழு fd, மாறிலி வெறுமை *buf, size_t மொத்தம், off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pwrite(fd, buf, மொத்தம், offset))
}

முழு freertos_linux_coop_open(மாறிலி எழுத்து *path, முழு flags, முழு mode)
{
    முழு fd = real_open(path, flags, (mode_t)mode);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, flags & O_NONBLOCK);
    திரும்பு fd;
}

முழு freertos_linux_coop_close(முழு fd)
{
    முழு ret = real_close(fd);
    எனில் (ret == 0 || errno == EINTR) {
        /* On Linux, close() always releases the fd even when returning
         * EINTR.  Retrying would risk closing a fd reused by another
         * thread.  Treat EINTR as success. */
        linux_coop_untrack_fd(fd);
        திரும்பு 0;
    }
    திரும்பு -1;
}

முழு freertos_linux_coop_fcntl(முழு fd, முழு cmd, முழு arg)
{
    எனில் (cmd == F_GETFL) {
        முழு real_flags = real_fcntl(fd, F_GETFL, 0);
        எனில் (real_flags < 0) {
            திரும்பு -1;
        }
        முழு user_flags = 0;
        எனில் (linux_coop_get_user_flags(fd, &user_flags)) {
            real_flags &= ~O_NONBLOCK;
            real_flags |= (user_flags & O_NONBLOCK);
        }
        திரும்பு real_flags;
    }
    எனில் (cmd == F_SETFL) {
        linux_coop_set_user_flags(fd, arg);
        முழு internal_flags = arg | O_NONBLOCK;
        திரும்பு real_fcntl(fd, F_SETFL, internal_flags);
    }
    திரும்பு real_fcntl(fd, cmd, arg);
}

முழு freertos_linux_coop_select(முழு nfds, fd_set *readfds, fd_set *writefds,
                      fd_set *exceptfds, கட்டமைப்பு timeval *timeout)
{
    long timeout_ms = -1;
    எனில் (timeout != வெற்று) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_usec + 999) / 1000;
        எனில் (timeout_ms == 0) {
            திரும்பு real_select(nfds, readfds, writefds, exceptfds, timeout);
        }
    }

    /* real_select() mutates the fd_sets in place (clears them, then sets bits
     * for ready descriptors). To keep polling the same descriptors across
     * iterations we must snapshot the caller's sets and restore them on every
     * loop. */
    fd_set rfds_in, wfds_in, efds_in;
    எனில் (readfds != வெற்று) {
        rfds_in = *readfds;
    }
    எனில் (writefds != வெற்று) {
        wfds_in = *writefds;
    }
    எனில் (exceptfds != வெற்று) {
        efds_in = *exceptfds;
    }

    long waited_ms = 0;
    போது (1) {
        எனில் (readfds != வெற்று) {
            *readfds = rfds_in;
        }
        எனில் (writefds != வெற்று) {
            *writefds = wfds_in;
        }
        எனில் (exceptfds != வெற்று) {
            *exceptfds = efds_in;
        }

        கட்டமைப்பு timeval zero_tv = {0, 0};
        முழு ret = real_select(nfds, readfds, writefds, exceptfds, &zero_tv);
        எனில் (ret != 0) {
            திரும்பு ret;
        }
        எனில் (timeout_ms == 0) {
            திரும்பு 0;
        }
        எனில் (timeout_ms > 0 && waited_ms >= timeout_ms) {
            திரும்பு 0;
        }
        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

வெறுமை freertos_linux_coop_syscalls_init(வெறுமை)
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

__attribute__((weak)) ssize_t பட(முழு fd, வெறுமை *buf, size_t மொத்தம்)
{
    திரும்பு freertos_linux_coop_read(fd, buf, மொத்தம்);
}

__attribute__((weak)) ssize_t லிக(முழு fd, மாறிலி வெறுமை *buf, size_t மொத்தம்)
{
    திரும்பு freertos_linux_coop_write(fd, buf, மொத்தம்);
}

__attribute__((weak)) ssize_t pread(முழு fd, வெறுமை *buf, size_t மொத்தம், off_t offset)
{
    திரும்பு freertos_linux_coop_pread(fd, buf, மொத்தம், offset);
}

__attribute__((weak)) ssize_t pwrite(முழு fd, மாறிலி வெறுமை *buf, size_t மொத்தம், off_t offset)
{
    திரும்பு freertos_linux_coop_pwrite(fd, buf, மொத்தம், offset);
}

__attribute__((weak)) முழு open(மாறிலி எழுத்து *path, முழு flags, ...)
{
    முழு mode = 0;
    எனில் (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        mode = va_arg(ap, முழு);
        va_end(ap);
    }
    திரும்பு freertos_linux_coop_open(path, flags, mode);
}

__attribute__((weak)) முழு close(முழு fd)
{
    திரும்பு freertos_linux_coop_close(fd);
}

__attribute__((weak)) முழு fcntl(முழு fd, முழு cmd, ...)
{
    முழு arg = 0;
    எனில் (cmd != F_GETFL) {
        va_list list;
        va_start(list, cmd);
        arg = va_arg(list, முழு);
        va_end(list);
    }
    திரும்பு freertos_linux_coop_fcntl(fd, cmd, arg);
}

__attribute__((weak)) முழு select(முழு nfds, fd_set *readfds, fd_set *writefds,
                                 fd_set *exceptfds, கட்டமைப்பு timeval *timeout)
{
    திரும்பு freertos_linux_coop_select(nfds, readfds, writefds, exceptfds, timeout);
}

ssize_t freertos_linux_coop_readv(முழு fd, மாறிலி கட்டமைப்பு iovec *iov, முழு iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_readv(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_writev(முழு fd, மாறிலி கட்டமைப்பு iovec *iov, முழு iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_writev(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_recv(முழு sockfd, வெறுமை *buf, size_t len, முழு flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recv(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_send(முழு sockfd, மாறிலி வெறுமை *buf, size_t len, முழு flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_send(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_recvfrom(முழு sockfd, வெறுமை *buf, size_t len, முழு flags,
                 கட்டமைப்பு sockaddr *src_addr, socklen_t *addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvfrom(sockfd, buf, len, flags | MSG_DONTWAIT,
                            src_addr, addrlen))
}

ssize_t freertos_linux_coop_sendto(முழு sockfd, மாறிலி வெறுமை *buf, size_t len, முழு flags,
               மாறிலி கட்டமைப்பு sockaddr *dest_addr, socklen_t addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendto(sockfd, buf, len, flags | MSG_DONTWAIT,
                          dest_addr, addrlen))
}

ssize_t freertos_linux_coop_recvmsg(முழு sockfd, கட்டமைப்பு msghdr *செய்தி, முழு flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvmsg(sockfd, செய்தி, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_sendmsg(முழு sockfd, மாறிலி கட்டமைப்பு msghdr *செய்தி, முழு flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendmsg(sockfd, செய்தி, flags | MSG_DONTWAIT))
}

__attribute__((weak)) ssize_t readv(முழு fd, மாறிலி கட்டமைப்பு iovec *iov, முழு iovcnt)
{
    திரும்பு freertos_linux_coop_readv(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t writev(முழு fd, மாறிலி கட்டமைப்பு iovec *iov, முழு iovcnt)
{
    திரும்பு freertos_linux_coop_writev(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t recv(முழு sockfd, வெறுமை *buf, size_t len, முழு flags)
{
    திரும்பு freertos_linux_coop_recv(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t send(முழு sockfd, மாறிலி வெறுமை *buf, size_t len, முழு flags)
{
    திரும்பு freertos_linux_coop_send(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t recvfrom(முழு sockfd, வெறுமை *buf, size_t len, முழு flags,
                 கட்டமைப்பு sockaddr *src_addr, socklen_t *addrlen)
{
    திரும்பு freertos_linux_coop_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

__attribute__((weak)) ssize_t sendto(முழு sockfd, மாறிலி வெறுமை *buf, size_t len, முழு flags,
               மாறிலி கட்டமைப்பு sockaddr *dest_addr, socklen_t addrlen)
{
    திரும்பு freertos_linux_coop_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

__attribute__((weak)) ssize_t recvmsg(முழு sockfd, கட்டமைப்பு msghdr *செய்தி, முழு flags)
{
    திரும்பு freertos_linux_coop_recvmsg(sockfd, செய்தி, flags);
}

__attribute__((weak)) ssize_t sendmsg(முழு sockfd, மாறிலி கட்டமைப்பு msghdr *செய்தி, முழு flags)
{
    திரும்பு freertos_linux_coop_sendmsg(sockfd, செய்தி, flags);
}

முழு freertos_linux_coop_connect(முழு sockfd, மாறிலி கட்டமைப்பு sockaddr *addr, socklen_t addrlen)
{
    போது (1)
    {
        முழு ret = real_connect(sockfd, addr, addrlen);
        எனில் (ret == 0) {
            திரும்பு ret;
        }

        எனில் (errno == EINPROGRESS || errno == EALREADY) {
            கட்டமைப்பு pollfd pfd;
            pfd.fd = sockfd;
            pfd.events = POLLOUT;
            pfd.revents = 0;

            முழு poll_retries = 0;
            போது (1)
            {
                முழு press = real_poll(&pfd, 1, 0);
                எனில் (press > 0) {
                    முழு so_err = 0;
                    socklen_t len = அளவு(so_err);
                    எனில் (getsockopt(sockfd, SOL_SOCKET, SO_ERROR,
                                   &so_err, &len) < 0) {
                        திரும்பு -1;
                    }
                    எனில் (so_err == 0) {
                        திரும்பு 0;
                    } இல்லையெனில் {
                        errno = so_err;
                        திரும்பு -1;
                    }
                } இல்லையெனில் எனில் (press == 0) {
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    தொடர்;
                } இல்லையெனில் {
                    எனில் (errno == EINTR) {
                        தொடர்;
                    }
                    /* Non-EINTR poll error; bail out after a few retries */
                    எனில் (++poll_retries > 3) {
                        திரும்பு -1;
                    }
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    தொடர்;
                }
            }
        }

        எனில் (errno == EINTR) {
            திரும்பு -1;
        }

        திரும்பு -1;
    }
}

முழு freertos_linux_coop_accept(முழு sockfd, கட்டமைப்பு sockaddr *addr, socklen_t *addrlen)
{
    போது (1)
    {
        முழு fd = real_accept(sockfd, addr, addrlen);
        எனில் (fd >= 0) {
            linux_coop_set_nonblocking(fd);
            linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(sockfd) ? O_NONBLOCK : 0);
            திரும்பு fd;
        }
        எனில் (errno == EAGAIN || errno == EWOULDBLOCK) {
            எனில் (linux_coop_fd_user_nonblocking(sockfd)) {
                திரும்பு -1;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            தொடர்;
        }
        திரும்பு -1;
    }
}

முழு freertos_linux_coop_pselect(முழு nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, மாறிலி கட்டமைப்பு timespec *timeout,
            மாறிலி sigset_t *sigmask)
{
    long timeout_ms = -1;
    எனில் (timeout != வெற்று) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_nsec + 999999) / 1000000;
        எனில் (timeout_ms == 0) {
            திரும்பு real_pselect(nfds, readfds, writefds, exceptfds,
                                timeout, sigmask);
        }
    }

    long waited_ms = 0;
    போது (1)
    {
        கட்டமைப்பு timespec zero_ts = {0, 0};
        முழு ret = real_pselect(nfds, readfds, writefds, exceptfds,
                               &zero_ts, sigmask);
        எனில் (ret != 0) {
            திரும்பு ret;
        }

        எனில் (timeout_ms == 0 ||
            (timeout_ms > 0 && waited_ms >= timeout_ms)) {
            திரும்பு 0;
        }

        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

முழு freertos_linux_coop_poll(கட்டமைப்பு pollfd *fds, nfds_t nfds, முழு timeout)
{
    எனில் (timeout == 0) {
        திரும்பு real_poll(fds, nfds, 0);
    }

    long timeout_ms = -1;
    எனில் (timeout > 0) {
        timeout_ms = timeout;
    }

    long waited_ms = 0;

    போது (1)
    {
        முழு ret = real_poll(fds, nfds, 0);
        எனில் (ret > 0) {
            திரும்பு ret;
        } இல்லையெனில் எனில் (ret == 0) {
            எனில் (timeout_ms == 0 ||
                (timeout_ms > 0 && waited_ms >= timeout_ms)) {
                திரும்பு 0;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            waited_ms += LINUX_COOP_TICK_MS;
            தொடர்;
        } இல்லையெனில் {
            எனில் (errno == EINTR) {
                தொடர்;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            எனில் (timeout_ms > 0 && waited_ms >= timeout_ms) {
                திரும்பு -1;
            }
            waited_ms += LINUX_COOP_TICK_MS;
        }
    }
}

முழு nanosleep(மாறிலி கட்டமைப்பு timespec *req, கட்டமைப்பு timespec *rem)
{
    எனில் (req == வெற்று) {
        errno = EFAULT;
        திரும்பு -1;
    }
    எனில் (!linux_port_in_freertos_task()) {
        திரும்பு real_nanosleep(req, rem);
    }
    long ms = req->tv_sec * 1000 + (req->tv_nsec + 999999) / 1000000;
    எனில் (ms <= 0) {
        ms = 1;
    }
    vTaskDelay(ms);
    எனில் (rem) {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }
    திரும்பு 0;
}

unsigned முழு sleep(unsigned முழு seconds)
{
    linux_coop_yield(seconds * 1000);
    திரும்பு 0;
}

முழு usleep(useconds_t usec)
{
    linux_coop_yield(usec / 1000);
    திரும்பு 0;
}

முழு freertos_linux_coop_socket(முழு domain, முழு type, முழு protocol)
{
    முழு fd = real_socket(domain, type, protocol);
    linux_coop_set_nonblocking(fd);
#ifdef SOCK_NONBLOCK
    மாறிலி முழு user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#இல்லையெனில்
    மாறிலி முழு user_flags = 0;
#endif
    linux_coop_track_fd(fd, user_flags);
    திரும்பு fd;
}

முழு freertos_linux_coop_socketpair(முழு domain, முழு type, முழு protocol, முழு *sv)
{
    முழு ret = real_socketpair(domain, type, protocol, sv);
    எனில் (ret == 0) {
        linux_coop_set_nonblocking(sv[0]);
        linux_coop_set_nonblocking(sv[1]);
#ifdef SOCK_NONBLOCK
        மாறிலி முழு user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#இல்லையெனில்
        மாறிலி முழு user_flags = 0;
#endif
        linux_coop_track_fd(sv[0], user_flags);
        linux_coop_track_fd(sv[1], user_flags);
    }
    திரும்பு ret;
}

முழு freertos_linux_coop_pipe(முழு *fds)
{
    முழு ret = real_pipe(fds);
    எனில் (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        linux_coop_track_fd(fds[0], 0);
        linux_coop_track_fd(fds[1], 0);
    }
    திரும்பு ret;
}

முழு freertos_linux_coop_pipe2(முழு *fds, முழு flags)
{
    முழு ret = real_pipe2(fds, flags);
    எனில் (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        மாறிலி முழு user_flags = (flags & O_NONBLOCK) ? O_NONBLOCK : 0;
        linux_coop_track_fd(fds[0], user_flags);
        linux_coop_track_fd(fds[1], user_flags);
    }
    திரும்பு ret;
}

முழு freertos_linux_coop_dup(முழு oldfd)
{
    முழு fd = real_dup(oldfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    திரும்பு fd;
}

முழு freertos_linux_coop_dup2(முழு oldfd, முழு newfd)
{
    முழு fd = real_dup2(oldfd, newfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    திரும்பு fd;
}

__attribute__((weak)) முழு connect(முழு sockfd, மாறிலி கட்டமைப்பு sockaddr *addr, socklen_t addrlen)
{
    திரும்பு freertos_linux_coop_connect(sockfd, addr, addrlen);
}

__attribute__((weak)) முழு accept(முழு sockfd, கட்டமைப்பு sockaddr *addr, socklen_t *addrlen)
{
    திரும்பு freertos_linux_coop_accept(sockfd, addr, addrlen);
}

__attribute__((weak)) முழு pselect(முழு nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, மாறிலி கட்டமைப்பு timespec *timeout,
            மாறிலி sigset_t *sigmask)
{
    திரும்பு freertos_linux_coop_pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}

__attribute__((weak)) முழு poll(கட்டமைப்பு pollfd *fds, nfds_t nfds, முழு timeout)
{
    திரும்பு freertos_linux_coop_poll(fds, nfds, timeout);
}

__attribute__((weak)) முழு socket(முழு domain, முழு type, முழு protocol)
{
    திரும்பு freertos_linux_coop_socket(domain, type, protocol);
}

__attribute__((weak)) முழு socketpair(முழு domain, முழு type, முழு protocol, முழு sv[2])
{
    திரும்பு freertos_linux_coop_socketpair(domain, type, protocol, sv);
}

__attribute__((weak)) முழு pipe(முழு fds[2])
{
    திரும்பு freertos_linux_coop_pipe(fds);
}

__attribute__((weak)) முழு pipe2(முழு fds[2], முழு flags)
{
    திரும்பு freertos_linux_coop_pipe2(fds, flags);
}

__attribute__((weak)) முழு dup(முழு oldfd)
{
    திரும்பு freertos_linux_coop_dup(oldfd);
}

__attribute__((weak)) முழு dup2(முழு oldfd, முழு newfd)
{
    திரும்பு freertos_linux_coop_dup2(oldfd, newfd);
}
