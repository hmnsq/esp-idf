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

#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <errno.h>
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <stdarg.h>
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <sys/uio.h>
#অন্তর্ভাব <sys/socket.h>
#অন্তর্ভাব <sys/select.h>
#অন্তর্ভাব <sys/time.h>
#অন্তর্ভাব <time.h>
#অন্তর্ভাব <poll.h>
#অন্তর্ভাব <dlfcn.h>
#অন্তর্ভাব <pthread.h>

#অন্তর্ভাব "linux_port_coop_internal.h"
#অন্তর্ভাব "esp_private/freertos_linux_coop_syscalls.h"

স্থির ssize_t  (*real_readv)(পূর্ণসংখ্যা, ধ্রুবক কাঠামো iovec *, পূর্ণসংখ্যা);
স্থির ssize_t  (*real_writev)(পূর্ণসংখ্যা, ধ্রুবক কাঠামো iovec *, পূর্ণসংখ্যা);
স্থির ssize_t  (*real_recv)(পূর্ণসংখ্যা, শূন্য *, size_t, পূর্ণসংখ্যা);
স্থির ssize_t  (*real_send)(পূর্ণসংখ্যা, ধ্রুবক শূন্য *, size_t, পূর্ণসংখ্যা);
স্থির ssize_t  (*real_recvfrom)(পূর্ণসংখ্যা, শূন্য *, size_t, পূর্ণসংখ্যা,
                                 কাঠামো sockaddr *, socklen_t *);
স্থির ssize_t  (*real_sendto)(পূর্ণসংখ্যা, ধ্রুবক শূন্য *, size_t, পূর্ণসংখ্যা,
                                ধ্রুবক কাঠামো sockaddr *, socklen_t);
স্থির ssize_t  (*real_recvmsg)(পূর্ণসংখ্যা, কাঠামো msghdr *, পূর্ণসংখ্যা);
স্থির ssize_t  (*real_sendmsg)(পূর্ণসংখ্যা, ধ্রুবক কাঠামো msghdr *, পূর্ণসংখ্যা);
স্থির পূর্ণসংখ্যা      (*real_connect)(পূর্ণসংখ্যা, ধ্রুবক কাঠামো sockaddr *, socklen_t);
স্থির পূর্ণসংখ্যা      (*real_accept)(পূর্ণসংখ্যা, কাঠামো sockaddr *, socklen_t *);
স্থির পূর্ণসংখ্যা      (*real_pselect)(পূর্ণসংখ্যা, fd_set *, fd_set *, fd_set *,
                                ধ্রুবক কাঠামো timespec *, ধ্রুবক sigset_t *);
স্থির পূর্ণসংখ্যা      (*real_poll)(কাঠামো pollfd *, nfds_t, পূর্ণসংখ্যা);
স্থির পূর্ণসংখ্যা      (*real_socket)(পূর্ণসংখ্যা, পূর্ণসংখ্যা, পূর্ণসংখ্যা);
স্থির পূর্ণসংখ্যা      (*real_socketpair)(পূর্ণসংখ্যা, পূর্ণসংখ্যা, পূর্ণসংখ্যা, পূর্ণসংখ্যা[2]);
স্থির পূর্ণসংখ্যা      (*real_pipe)(পূর্ণসংখ্যা[2]);
স্থির পূর্ণসংখ্যা      (*real_pipe2)(পূর্ণসংখ্যা[2], পূর্ণসংখ্যা);
স্থির পূর্ণসংখ্যা      (*real_dup)(পূর্ণসংখ্যা);
স্থির পূর্ণসংখ্যা      (*real_dup2)(পূর্ণসংখ্যা, পূর্ণসংখ্যা);
স্থির পূর্ণসংখ্যা      (*real_nanosleep)(ধ্রুবক কাঠামো timespec *, কাঠামো timespec *);
স্থির ssize_t  (*real_read)(পূর্ণসংখ্যা, শূন্য *, size_t);
স্থির ssize_t  (*real_write)(পূর্ণসংখ্যা, ধ্রুবক শূন্য *, size_t);
স্থির ssize_t  (*real_pread)(পূর্ণসংখ্যা, শূন্য *, size_t, off_t);
স্থির ssize_t  (*real_pwrite)(পূর্ণসংখ্যা, ধ্রুবক শূন্য *, size_t, off_t);
স্থির পূর্ণসংখ্যা      (*real_close)(পূর্ণসংখ্যা);
স্থির পূর্ণসংখ্যা      (*real_select)(পূর্ণসংখ্যা, fd_set *, fd_set *, fd_set *,
                                কাঠামো timeval *);

প্রকারলেখ পূর্ণসংখ্যা (*linux_coop_fcntl_fn_t)(পূর্ণসংখ্যা, পূর্ণসংখ্যা, ...);
স্থির linux_coop_fcntl_fn_t real_fcntl;

প্রকারলেখ পূর্ণসংখ্যা (*linux_coop_open_fn_t)(ধ্রুবক অক্ষর *, পূর্ণসংখ্যা, mode_t);
স্থির linux_coop_open_fn_t real_open;

প্রকারলেখ কাঠামো linux_coop_fd_state {
    পূর্ণসংখ্যা fd;
    পূর্ণসংখ্যা user_flags;
    কাঠামো linux_coop_fd_state *পরবর্তী;
} linux_coop_fd_state_t;

স্থির linux_coop_fd_state_t *s_fd_state_list;
স্থির pthread_mutex_t s_fd_state_lock = PTHREAD_MUTEX_INITIALIZER;

স্থির linux_coop_fd_state_t *linux_coop_find_fd_locked(পূর্ণসংখ্যা fd)
{
    linux_coop_fd_state_t *গ্রন্থি = s_fd_state_list;
    যখন (গ্রন্থি != ফাঁকা) {
        যদি (গ্রন্থি->fd == fd) {
            ফেরত গ্রন্থি;
        }
        গ্রন্থি = গ্রন্থি->পরবর্তী;
    }
    ফেরত ফাঁকা;
}

স্থির শূন্য __attribute__((constructor)) linux_coop_resolve_all(শূন্য)
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
    LINUX_COOP_RESOLVE(পঠ);
    LINUX_COOP_RESOLVE(লিখ);
    LINUX_COOP_RESOLVE(pread);
    LINUX_COOP_RESOLVE(pwrite);
    LINUX_COOP_RESOLVE(close);
    LINUX_COOP_RESOLVE(select);
    LINUX_COOP_RESOLVE(open);
}

শূন্য linux_coop_set_nonblocking(পূর্ণসংখ্যা fd)
{
    যদি (fd >= 0) {
        পূর্ণসংখ্যা flags = real_fcntl(fd, F_GETFL, 0);
        যদি (flags >= 0) {
            real_fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
    }
}

শূন্য linux_coop_track_fd(পূর্ণসংখ্যা fd, পূর্ণসংখ্যা user_flags)
{
    যদি (fd < 0) {
        ফেরত;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *গ্রন্থি = linux_coop_find_fd_locked(fd);
    যদি (গ্রন্থি != ফাঁকা) {
        গ্রন্থি->user_flags = user_flags;
        pthread_mutex_unlock(&s_fd_state_lock);
        ফেরত;
    }

    গ্রন্থি = calloc(1, আকার(*গ্রন্থি));
    যদি (গ্রন্থি != ফাঁকা) {
        গ্রন্থি->fd = fd;
        গ্রন্থি->user_flags = user_flags;
        গ্রন্থি->পরবর্তী = s_fd_state_list;
        s_fd_state_list = গ্রন্থি;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

শূন্য linux_coop_untrack_fd(পূর্ণসংখ্যা fd)
{
    যদি (fd < 0) {
        ফেরত;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *পূর্ব = ফাঁকা;
    linux_coop_fd_state_t *গ্রন্থি = s_fd_state_list;
    যখন (গ্রন্থি != ফাঁকা) {
        যদি (গ্রন্থি->fd == fd) {
            যদি (পূর্ব == ফাঁকা) {
                s_fd_state_list = গ্রন্থি->পরবর্তী;
            } অন্যথা {
                পূর্ব->পরবর্তী = গ্রন্থি->পরবর্তী;
            }
            মুক্ত(গ্রন্থি);
            বিরতি;
        }
        পূর্ব = গ্রন্থি;
        গ্রন্থি = গ্রন্থি->পরবর্তী;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

শূন্য linux_coop_set_user_flags(পূর্ণসংখ্যা fd, পূর্ণসংখ্যা user_flags)
{
    linux_coop_track_fd(fd, user_flags);
}

bool linux_coop_get_user_flags(পূর্ণসংখ্যা fd, পূর্ণসংখ্যা *flags)
{
    যদি (fd < 0 || flags == ফাঁকা) {
        ফেরত false;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *গ্রন্থি = linux_coop_find_fd_locked(fd);
    যদি (গ্রন্থি == ফাঁকা) {
        pthread_mutex_unlock(&s_fd_state_lock);
        ফেরত false;
    }
    *flags = গ্রন্থি->user_flags;
    pthread_mutex_unlock(&s_fd_state_lock);
    ফেরত true;
}

bool linux_coop_fd_user_nonblocking(পূর্ণসংখ্যা fd)
{
    পূর্ণসংখ্যা user_flags = 0;
    যদি (linux_coop_get_user_flags(fd, &user_flags)) {
        ফেরত (user_flags & O_NONBLOCK) != 0;
    }
    ফেরত false;
}


ssize_t freertos_linux_coop_read(পূর্ণসংখ্যা fd, শূন্য *buf, size_t গুনতি)
{
    LINUX_COOP_IO_LOOP(fd, real_read(fd, buf, গুনতি))
}

ssize_t freertos_linux_coop_write(পূর্ণসংখ্যা fd, ধ্রুবক শূন্য *buf, size_t গুনতি)
{
    LINUX_COOP_IO_LOOP(fd, real_write(fd, buf, গুনতি))
}

ssize_t freertos_linux_coop_pread(পূর্ণসংখ্যা fd, শূন্য *buf, size_t গুনতি, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pread(fd, buf, গুনতি, offset))
}

ssize_t freertos_linux_coop_pwrite(পূর্ণসংখ্যা fd, ধ্রুবক শূন্য *buf, size_t গুনতি, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pwrite(fd, buf, গুনতি, offset))
}

পূর্ণসংখ্যা freertos_linux_coop_open(ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা flags, পূর্ণসংখ্যা mode)
{
    পূর্ণসংখ্যা fd = real_open(path, flags, (mode_t)mode);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, flags & O_NONBLOCK);
    ফেরত fd;
}

পূর্ণসংখ্যা freertos_linux_coop_close(পূর্ণসংখ্যা fd)
{
    পূর্ণসংখ্যা ret = real_close(fd);
    যদি (ret == 0 || errno == EINTR) {
        /* On Linux, close() always releases the fd even when returning
         * EINTR.  Retrying would risk closing a fd reused by another
         * thread.  Treat EINTR as success. */
        linux_coop_untrack_fd(fd);
        ফেরত 0;
    }
    ফেরত -1;
}

পূর্ণসংখ্যা freertos_linux_coop_fcntl(পূর্ণসংখ্যা fd, পূর্ণসংখ্যা cmd, পূর্ণসংখ্যা arg)
{
    যদি (cmd == F_GETFL) {
        পূর্ণসংখ্যা real_flags = real_fcntl(fd, F_GETFL, 0);
        যদি (real_flags < 0) {
            ফেরত -1;
        }
        পূর্ণসংখ্যা user_flags = 0;
        যদি (linux_coop_get_user_flags(fd, &user_flags)) {
            real_flags &= ~O_NONBLOCK;
            real_flags |= (user_flags & O_NONBLOCK);
        }
        ফেরত real_flags;
    }
    যদি (cmd == F_SETFL) {
        linux_coop_set_user_flags(fd, arg);
        পূর্ণসংখ্যা internal_flags = arg | O_NONBLOCK;
        ফেরত real_fcntl(fd, F_SETFL, internal_flags);
    }
    ফেরত real_fcntl(fd, cmd, arg);
}

পূর্ণসংখ্যা freertos_linux_coop_select(পূর্ণসংখ্যা nfds, fd_set *readfds, fd_set *writefds,
                      fd_set *exceptfds, কাঠামো timeval *timeout)
{
    long timeout_ms = -1;
    যদি (timeout != ফাঁকা) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_usec + 999) / 1000;
        যদি (timeout_ms == 0) {
            ফেরত real_select(nfds, readfds, writefds, exceptfds, timeout);
        }
    }

    /* real_select() mutates the fd_sets in place (clears them, then sets bits
     * for ready descriptors). To keep polling the same descriptors across
     * iterations we must snapshot the caller's sets and restore them on every
     * loop. */
    fd_set rfds_in, wfds_in, efds_in;
    যদি (readfds != ফাঁকা) {
        rfds_in = *readfds;
    }
    যদি (writefds != ফাঁকা) {
        wfds_in = *writefds;
    }
    যদি (exceptfds != ফাঁকা) {
        efds_in = *exceptfds;
    }

    long waited_ms = 0;
    যখন (1) {
        যদি (readfds != ফাঁকা) {
            *readfds = rfds_in;
        }
        যদি (writefds != ফাঁকা) {
            *writefds = wfds_in;
        }
        যদি (exceptfds != ফাঁকা) {
            *exceptfds = efds_in;
        }

        কাঠামো timeval zero_tv = {0, 0};
        পূর্ণসংখ্যা ret = real_select(nfds, readfds, writefds, exceptfds, &zero_tv);
        যদি (ret != 0) {
            ফেরত ret;
        }
        যদি (timeout_ms == 0) {
            ফেরত 0;
        }
        যদি (timeout_ms > 0 && waited_ms >= timeout_ms) {
            ফেরত 0;
        }
        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

শূন্য freertos_linux_coop_syscalls_init(শূন্য)
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

__attribute__((weak)) ssize_t পঠ(পূর্ণসংখ্যা fd, শূন্য *buf, size_t গুনতি)
{
    ফেরত freertos_linux_coop_read(fd, buf, গুনতি);
}

__attribute__((weak)) ssize_t লিখ(পূর্ণসংখ্যা fd, ধ্রুবক শূন্য *buf, size_t গুনতি)
{
    ফেরত freertos_linux_coop_write(fd, buf, গুনতি);
}

__attribute__((weak)) ssize_t pread(পূর্ণসংখ্যা fd, শূন্য *buf, size_t গুনতি, off_t offset)
{
    ফেরত freertos_linux_coop_pread(fd, buf, গুনতি, offset);
}

__attribute__((weak)) ssize_t pwrite(পূর্ণসংখ্যা fd, ধ্রুবক শূন্য *buf, size_t গুনতি, off_t offset)
{
    ফেরত freertos_linux_coop_pwrite(fd, buf, গুনতি, offset);
}

__attribute__((weak)) পূর্ণসংখ্যা open(ধ্রুবক অক্ষর *path, পূর্ণসংখ্যা flags, ...)
{
    পূর্ণসংখ্যা mode = 0;
    যদি (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        mode = va_arg(ap, পূর্ণসংখ্যা);
        va_end(ap);
    }
    ফেরত freertos_linux_coop_open(path, flags, mode);
}

__attribute__((weak)) পূর্ণসংখ্যা close(পূর্ণসংখ্যা fd)
{
    ফেরত freertos_linux_coop_close(fd);
}

__attribute__((weak)) পূর্ণসংখ্যা fcntl(পূর্ণসংখ্যা fd, পূর্ণসংখ্যা cmd, ...)
{
    পূর্ণসংখ্যা arg = 0;
    যদি (cmd != F_GETFL) {
        va_list list;
        va_start(list, cmd);
        arg = va_arg(list, পূর্ণসংখ্যা);
        va_end(list);
    }
    ফেরত freertos_linux_coop_fcntl(fd, cmd, arg);
}

__attribute__((weak)) পূর্ণসংখ্যা select(পূর্ণসংখ্যা nfds, fd_set *readfds, fd_set *writefds,
                                 fd_set *exceptfds, কাঠামো timeval *timeout)
{
    ফেরত freertos_linux_coop_select(nfds, readfds, writefds, exceptfds, timeout);
}

ssize_t freertos_linux_coop_readv(পূর্ণসংখ্যা fd, ধ্রুবক কাঠামো iovec *iov, পূর্ণসংখ্যা iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_readv(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_writev(পূর্ণসংখ্যা fd, ধ্রুবক কাঠামো iovec *iov, পূর্ণসংখ্যা iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_writev(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_recv(পূর্ণসংখ্যা sockfd, শূন্য *buf, size_t len, পূর্ণসংখ্যা flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recv(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_send(পূর্ণসংখ্যা sockfd, ধ্রুবক শূন্য *buf, size_t len, পূর্ণসংখ্যা flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_send(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_recvfrom(পূর্ণসংখ্যা sockfd, শূন্য *buf, size_t len, পূর্ণসংখ্যা flags,
                 কাঠামো sockaddr *src_addr, socklen_t *addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvfrom(sockfd, buf, len, flags | MSG_DONTWAIT,
                            src_addr, addrlen))
}

ssize_t freertos_linux_coop_sendto(পূর্ণসংখ্যা sockfd, ধ্রুবক শূন্য *buf, size_t len, পূর্ণসংখ্যা flags,
               ধ্রুবক কাঠামো sockaddr *dest_addr, socklen_t addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendto(sockfd, buf, len, flags | MSG_DONTWAIT,
                          dest_addr, addrlen))
}

ssize_t freertos_linux_coop_recvmsg(পূর্ণসংখ্যা sockfd, কাঠামো msghdr *বার্তা, পূর্ণসংখ্যা flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvmsg(sockfd, বার্তা, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_sendmsg(পূর্ণসংখ্যা sockfd, ধ্রুবক কাঠামো msghdr *বার্তা, পূর্ণসংখ্যা flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendmsg(sockfd, বার্তা, flags | MSG_DONTWAIT))
}

__attribute__((weak)) ssize_t readv(পূর্ণসংখ্যা fd, ধ্রুবক কাঠামো iovec *iov, পূর্ণসংখ্যা iovcnt)
{
    ফেরত freertos_linux_coop_readv(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t writev(পূর্ণসংখ্যা fd, ধ্রুবক কাঠামো iovec *iov, পূর্ণসংখ্যা iovcnt)
{
    ফেরত freertos_linux_coop_writev(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t recv(পূর্ণসংখ্যা sockfd, শূন্য *buf, size_t len, পূর্ণসংখ্যা flags)
{
    ফেরত freertos_linux_coop_recv(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t send(পূর্ণসংখ্যা sockfd, ধ্রুবক শূন্য *buf, size_t len, পূর্ণসংখ্যা flags)
{
    ফেরত freertos_linux_coop_send(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t recvfrom(পূর্ণসংখ্যা sockfd, শূন্য *buf, size_t len, পূর্ণসংখ্যা flags,
                 কাঠামো sockaddr *src_addr, socklen_t *addrlen)
{
    ফেরত freertos_linux_coop_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

__attribute__((weak)) ssize_t sendto(পূর্ণসংখ্যা sockfd, ধ্রুবক শূন্য *buf, size_t len, পূর্ণসংখ্যা flags,
               ধ্রুবক কাঠামো sockaddr *dest_addr, socklen_t addrlen)
{
    ফেরত freertos_linux_coop_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

__attribute__((weak)) ssize_t recvmsg(পূর্ণসংখ্যা sockfd, কাঠামো msghdr *বার্তা, পূর্ণসংখ্যা flags)
{
    ফেরত freertos_linux_coop_recvmsg(sockfd, বার্তা, flags);
}

__attribute__((weak)) ssize_t sendmsg(পূর্ণসংখ্যা sockfd, ধ্রুবক কাঠামো msghdr *বার্তা, পূর্ণসংখ্যা flags)
{
    ফেরত freertos_linux_coop_sendmsg(sockfd, বার্তা, flags);
}

পূর্ণসংখ্যা freertos_linux_coop_connect(পূর্ণসংখ্যা sockfd, ধ্রুবক কাঠামো sockaddr *addr, socklen_t addrlen)
{
    যখন (1)
    {
        পূর্ণসংখ্যা ret = real_connect(sockfd, addr, addrlen);
        যদি (ret == 0) {
            ফেরত ret;
        }

        যদি (errno == EINPROGRESS || errno == EALREADY) {
            কাঠামো pollfd pfd;
            pfd.fd = sockfd;
            pfd.events = POLLOUT;
            pfd.revents = 0;

            পূর্ণসংখ্যা poll_retries = 0;
            যখন (1)
            {
                পূর্ণসংখ্যা press = real_poll(&pfd, 1, 0);
                যদি (press > 0) {
                    পূর্ণসংখ্যা so_err = 0;
                    socklen_t len = আকার(so_err);
                    যদি (getsockopt(sockfd, SOL_SOCKET, SO_ERROR,
                                   &so_err, &len) < 0) {
                        ফেরত -1;
                    }
                    যদি (so_err == 0) {
                        ফেরত 0;
                    } অন্যথা {
                        errno = so_err;
                        ফেরত -1;
                    }
                } অন্যথা যদি (press == 0) {
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    চালু;
                } অন্যথা {
                    যদি (errno == EINTR) {
                        চালু;
                    }
                    /* Non-EINTR poll error; bail out after a few retries */
                    যদি (++poll_retries > 3) {
                        ফেরত -1;
                    }
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    চালু;
                }
            }
        }

        যদি (errno == EINTR) {
            ফেরত -1;
        }

        ফেরত -1;
    }
}

পূর্ণসংখ্যা freertos_linux_coop_accept(পূর্ণসংখ্যা sockfd, কাঠামো sockaddr *addr, socklen_t *addrlen)
{
    যখন (1)
    {
        পূর্ণসংখ্যা fd = real_accept(sockfd, addr, addrlen);
        যদি (fd >= 0) {
            linux_coop_set_nonblocking(fd);
            linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(sockfd) ? O_NONBLOCK : 0);
            ফেরত fd;
        }
        যদি (errno == EAGAIN || errno == EWOULDBLOCK) {
            যদি (linux_coop_fd_user_nonblocking(sockfd)) {
                ফেরত -1;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            চালু;
        }
        ফেরত -1;
    }
}

পূর্ণসংখ্যা freertos_linux_coop_pselect(পূর্ণসংখ্যা nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, ধ্রুবক কাঠামো timespec *timeout,
            ধ্রুবক sigset_t *sigmask)
{
    long timeout_ms = -1;
    যদি (timeout != ফাঁকা) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_nsec + 999999) / 1000000;
        যদি (timeout_ms == 0) {
            ফেরত real_pselect(nfds, readfds, writefds, exceptfds,
                                timeout, sigmask);
        }
    }

    long waited_ms = 0;
    যখন (1)
    {
        কাঠামো timespec zero_ts = {0, 0};
        পূর্ণসংখ্যা ret = real_pselect(nfds, readfds, writefds, exceptfds,
                               &zero_ts, sigmask);
        যদি (ret != 0) {
            ফেরত ret;
        }

        যদি (timeout_ms == 0 ||
            (timeout_ms > 0 && waited_ms >= timeout_ms)) {
            ফেরত 0;
        }

        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

পূর্ণসংখ্যা freertos_linux_coop_poll(কাঠামো pollfd *fds, nfds_t nfds, পূর্ণসংখ্যা timeout)
{
    যদি (timeout == 0) {
        ফেরত real_poll(fds, nfds, 0);
    }

    long timeout_ms = -1;
    যদি (timeout > 0) {
        timeout_ms = timeout;
    }

    long waited_ms = 0;

    যখন (1)
    {
        পূর্ণসংখ্যা ret = real_poll(fds, nfds, 0);
        যদি (ret > 0) {
            ফেরত ret;
        } অন্যথা যদি (ret == 0) {
            যদি (timeout_ms == 0 ||
                (timeout_ms > 0 && waited_ms >= timeout_ms)) {
                ফেরত 0;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            waited_ms += LINUX_COOP_TICK_MS;
            চালু;
        } অন্যথা {
            যদি (errno == EINTR) {
                চালু;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            যদি (timeout_ms > 0 && waited_ms >= timeout_ms) {
                ফেরত -1;
            }
            waited_ms += LINUX_COOP_TICK_MS;
        }
    }
}

পূর্ণসংখ্যা nanosleep(ধ্রুবক কাঠামো timespec *req, কাঠামো timespec *rem)
{
    যদি (req == ফাঁকা) {
        errno = EFAULT;
        ফেরত -1;
    }
    যদি (!linux_port_in_freertos_task()) {
        ফেরত real_nanosleep(req, rem);
    }
    long ms = req->tv_sec * 1000 + (req->tv_nsec + 999999) / 1000000;
    যদি (ms <= 0) {
        ms = 1;
    }
    vTaskDelay(ms);
    যদি (rem) {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }
    ফেরত 0;
}

unsigned পূর্ণসংখ্যা sleep(unsigned পূর্ণসংখ্যা seconds)
{
    linux_coop_yield(seconds * 1000);
    ফেরত 0;
}

পূর্ণসংখ্যা usleep(useconds_t usec)
{
    linux_coop_yield(usec / 1000);
    ফেরত 0;
}

পূর্ণসংখ্যা freertos_linux_coop_socket(পূর্ণসংখ্যা domain, পূর্ণসংখ্যা type, পূর্ণসংখ্যা protocol)
{
    পূর্ণসংখ্যা fd = real_socket(domain, type, protocol);
    linux_coop_set_nonblocking(fd);
#ifdef SOCK_NONBLOCK
    ধ্রুবক পূর্ণসংখ্যা user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#অন্যথা
    ধ্রুবক পূর্ণসংখ্যা user_flags = 0;
#endif
    linux_coop_track_fd(fd, user_flags);
    ফেরত fd;
}

পূর্ণসংখ্যা freertos_linux_coop_socketpair(পূর্ণসংখ্যা domain, পূর্ণসংখ্যা type, পূর্ণসংখ্যা protocol, পূর্ণসংখ্যা *sv)
{
    পূর্ণসংখ্যা ret = real_socketpair(domain, type, protocol, sv);
    যদি (ret == 0) {
        linux_coop_set_nonblocking(sv[0]);
        linux_coop_set_nonblocking(sv[1]);
#ifdef SOCK_NONBLOCK
        ধ্রুবক পূর্ণসংখ্যা user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#অন্যথা
        ধ্রুবক পূর্ণসংখ্যা user_flags = 0;
#endif
        linux_coop_track_fd(sv[0], user_flags);
        linux_coop_track_fd(sv[1], user_flags);
    }
    ফেরত ret;
}

পূর্ণসংখ্যা freertos_linux_coop_pipe(পূর্ণসংখ্যা *fds)
{
    পূর্ণসংখ্যা ret = real_pipe(fds);
    যদি (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        linux_coop_track_fd(fds[0], 0);
        linux_coop_track_fd(fds[1], 0);
    }
    ফেরত ret;
}

পূর্ণসংখ্যা freertos_linux_coop_pipe2(পূর্ণসংখ্যা *fds, পূর্ণসংখ্যা flags)
{
    পূর্ণসংখ্যা ret = real_pipe2(fds, flags);
    যদি (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        ধ্রুবক পূর্ণসংখ্যা user_flags = (flags & O_NONBLOCK) ? O_NONBLOCK : 0;
        linux_coop_track_fd(fds[0], user_flags);
        linux_coop_track_fd(fds[1], user_flags);
    }
    ফেরত ret;
}

পূর্ণসংখ্যা freertos_linux_coop_dup(পূর্ণসংখ্যা oldfd)
{
    পূর্ণসংখ্যা fd = real_dup(oldfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    ফেরত fd;
}

পূর্ণসংখ্যা freertos_linux_coop_dup2(পূর্ণসংখ্যা oldfd, পূর্ণসংখ্যা newfd)
{
    পূর্ণসংখ্যা fd = real_dup2(oldfd, newfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    ফেরত fd;
}

__attribute__((weak)) পূর্ণসংখ্যা connect(পূর্ণসংখ্যা sockfd, ধ্রুবক কাঠামো sockaddr *addr, socklen_t addrlen)
{
    ফেরত freertos_linux_coop_connect(sockfd, addr, addrlen);
}

__attribute__((weak)) পূর্ণসংখ্যা accept(পূর্ণসংখ্যা sockfd, কাঠামো sockaddr *addr, socklen_t *addrlen)
{
    ফেরত freertos_linux_coop_accept(sockfd, addr, addrlen);
}

__attribute__((weak)) পূর্ণসংখ্যা pselect(পূর্ণসংখ্যা nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, ধ্রুবক কাঠামো timespec *timeout,
            ধ্রুবক sigset_t *sigmask)
{
    ফেরত freertos_linux_coop_pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}

__attribute__((weak)) পূর্ণসংখ্যা poll(কাঠামো pollfd *fds, nfds_t nfds, পূর্ণসংখ্যা timeout)
{
    ফেরত freertos_linux_coop_poll(fds, nfds, timeout);
}

__attribute__((weak)) পূর্ণসংখ্যা socket(পূর্ণসংখ্যা domain, পূর্ণসংখ্যা type, পূর্ণসংখ্যা protocol)
{
    ফেরত freertos_linux_coop_socket(domain, type, protocol);
}

__attribute__((weak)) পূর্ণসংখ্যা socketpair(পূর্ণসংখ্যা domain, পূর্ণসংখ্যা type, পূর্ণসংখ্যা protocol, পূর্ণসংখ্যা sv[2])
{
    ফেরত freertos_linux_coop_socketpair(domain, type, protocol, sv);
}

__attribute__((weak)) পূর্ণসংখ্যা pipe(পূর্ণসংখ্যা fds[2])
{
    ফেরত freertos_linux_coop_pipe(fds);
}

__attribute__((weak)) পূর্ণসংখ্যা pipe2(পূর্ণসংখ্যা fds[2], পূর্ণসংখ্যা flags)
{
    ফেরত freertos_linux_coop_pipe2(fds, flags);
}

__attribute__((weak)) পূর্ণসংখ্যা dup(পূর্ণসংখ্যা oldfd)
{
    ফেরত freertos_linux_coop_dup(oldfd);
}

__attribute__((weak)) পূর্ণসংখ্যা dup2(পূর্ণসংখ্যা oldfd, পূর্ণসংখ্যা newfd)
{
    ফেরত freertos_linux_coop_dup2(oldfd, newfd);
}
