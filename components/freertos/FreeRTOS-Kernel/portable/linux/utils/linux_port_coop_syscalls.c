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

স্থির ssize_t  (*real_readv)(পূর্ণাঙ্ক, ধ্রুব সংরচনা iovec *, পূর্ণাঙ্ক);
স্থির ssize_t  (*real_writev)(পূর্ণাঙ্ক, ধ্রুব সংরচনা iovec *, পূর্ণাঙ্ক);
স্থির ssize_t  (*real_recv)(পূর্ণাঙ্ক, শূন্য *, size_t, পূর্ণাঙ্ক);
স্থির ssize_t  (*real_send)(পূর্ণাঙ্ক, ধ্রুব শূন্য *, size_t, পূর্ণাঙ্ক);
স্থির ssize_t  (*real_recvfrom)(পূর্ণাঙ্ক, শূন্য *, size_t, পূর্ণাঙ্ক,
                                 সংরচনা sockaddr *, socklen_t *);
স্থির ssize_t  (*real_sendto)(পূর্ণাঙ্ক, ধ্রুব শূন্য *, size_t, পূর্ণাঙ্ক,
                                ধ্রুব সংরচনা sockaddr *, socklen_t);
স্থির ssize_t  (*real_recvmsg)(পূর্ণাঙ্ক, সংরচনা msghdr *, পূর্ণাঙ্ক);
স্থির ssize_t  (*real_sendmsg)(পূর্ণাঙ্ক, ধ্রুব সংরচনা msghdr *, পূর্ণাঙ্ক);
স্থির পূর্ণাঙ্ক      (*real_connect)(পূর্ণাঙ্ক, ধ্রুব সংরচনা sockaddr *, socklen_t);
স্থির পূর্ণাঙ্ক      (*real_accept)(পূর্ণাঙ্ক, সংরচনা sockaddr *, socklen_t *);
স্থির পূর্ণাঙ্ক      (*real_pselect)(পূর্ণাঙ্ক, fd_set *, fd_set *, fd_set *,
                                ধ্রুব সংরচনা timespec *, ধ্রুব sigset_t *);
স্থির পূর্ণাঙ্ক      (*real_poll)(সংরচনা pollfd *, nfds_t, পূর্ণাঙ্ক);
স্থির পূর্ণাঙ্ক      (*real_socket)(পূর্ণাঙ্ক, পূর্ণাঙ্ক, পূর্ণাঙ্ক);
স্থির পূর্ণাঙ্ক      (*real_socketpair)(পূর্ণাঙ্ক, পূর্ণাঙ্ক, পূর্ণাঙ্ক, পূর্ণাঙ্ক[2]);
স্থির পূর্ণাঙ্ক      (*real_pipe)(পূর্ণাঙ্ক[2]);
স্থির পূর্ণাঙ্ক      (*real_pipe2)(পূর্ণাঙ্ক[2], পূর্ণাঙ্ক);
স্থির পূর্ণাঙ্ক      (*real_dup)(পূর্ণাঙ্ক);
স্থির পূর্ণাঙ্ক      (*real_dup2)(পূর্ণাঙ্ক, পূর্ণাঙ্ক);
স্থির পূর্ণাঙ্ক      (*real_nanosleep)(ধ্রুব সংরচনা timespec *, সংরচনা timespec *);
স্থির ssize_t  (*real_read)(পূর্ণাঙ্ক, শূন্য *, size_t);
স্থির ssize_t  (*real_write)(পূর্ণাঙ্ক, ধ্রুব শূন্য *, size_t);
স্থির ssize_t  (*real_pread)(পূর্ণাঙ্ক, শূন্য *, size_t, off_t);
স্থির ssize_t  (*real_pwrite)(পূর্ণাঙ্ক, ধ্রুব শূন্য *, size_t, off_t);
স্থির পূর্ণাঙ্ক      (*real_close)(পূর্ণাঙ্ক);
স্থির পূর্ণাঙ্ক      (*real_select)(পূর্ণাঙ্ক, fd_set *, fd_set *, fd_set *,
                                সংরচনা timeval *);

প্রকারনির্দেশ পূর্ণাঙ্ক (*linux_coop_fcntl_fn_t)(পূর্ণাঙ্ক, পূর্ণাঙ্ক, ...);
স্থির linux_coop_fcntl_fn_t real_fcntl;

প্রকারনির্দেশ পূর্ণাঙ্ক (*linux_coop_open_fn_t)(ধ্রুব বর্ণ *, পূর্ণাঙ্ক, mode_t);
স্থির linux_coop_open_fn_t real_open;

প্রকারনির্দেশ সংরচনা linux_coop_fd_state {
    পূর্ণাঙ্ক fd;
    পূর্ণাঙ্ক user_flags;
    সংরচনা linux_coop_fd_state *next;
} linux_coop_fd_state_t;

স্থির linux_coop_fd_state_t *s_fd_state_list;
স্থির pthread_mutex_t s_fd_state_lock = PTHREAD_MUTEX_INITIALIZER;

স্থির linux_coop_fd_state_t *linux_coop_find_fd_locked(পূর্ণাঙ্ক fd)
{
    linux_coop_fd_state_t *গ্রন্থি = s_fd_state_list;
    যাবত্ (গ্রন্থি != রিক্ত) {
        যদি (গ্রন্থি->fd == fd) {
            প্রত্যাগম গ্রন্থি;
        }
        গ্রন্থি = গ্রন্থি->next;
    }
    প্রত্যাগম রিক্ত;
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

শূন্য linux_coop_set_nonblocking(পূর্ণাঙ্ক fd)
{
    যদি (fd >= 0) {
        পূর্ণাঙ্ক flags = real_fcntl(fd, F_GETFL, 0);
        যদি (flags >= 0) {
            real_fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
    }
}

শূন্য linux_coop_track_fd(পূর্ণাঙ্ক fd, পূর্ণাঙ্ক user_flags)
{
    যদি (fd < 0) {
        প্রত্যাগম;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *গ্রন্থি = linux_coop_find_fd_locked(fd);
    যদি (গ্রন্থি != রিক্ত) {
        গ্রন্থি->user_flags = user_flags;
        pthread_mutex_unlock(&s_fd_state_lock);
        প্রত্যাগম;
    }

    গ্রন্থি = calloc(1, পরিমাণ(*গ্রন্থি));
    যদি (গ্রন্থি != রিক্ত) {
        গ্রন্থি->fd = fd;
        গ্রন্থি->user_flags = user_flags;
        গ্রন্থি->next = s_fd_state_list;
        s_fd_state_list = গ্রন্থি;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

শূন্য linux_coop_untrack_fd(পূর্ণাঙ্ক fd)
{
    যদি (fd < 0) {
        প্রত্যাগম;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *prev = রিক্ত;
    linux_coop_fd_state_t *গ্রন্থি = s_fd_state_list;
    যাবত্ (গ্রন্থি != রিক্ত) {
        যদি (গ্রন্থি->fd == fd) {
            যদি (prev == রিক্ত) {
                s_fd_state_list = গ্রন্থি->next;
            } অন্যথা {
                prev->next = গ্রন্থি->next;
            }
            মুক্ত(গ্রন্থি);
            ভঙ্গ;
        }
        prev = গ্রন্থি;
        গ্রন্থি = গ্রন্থি->next;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

শূন্য linux_coop_set_user_flags(পূর্ণাঙ্ক fd, পূর্ণাঙ্ক user_flags)
{
    linux_coop_track_fd(fd, user_flags);
}

bool linux_coop_get_user_flags(পূর্ণাঙ্ক fd, পূর্ণাঙ্ক *flags)
{
    যদি (fd < 0 || flags == রিক্ত) {
        প্রত্যাগম false;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *গ্রন্থি = linux_coop_find_fd_locked(fd);
    যদি (গ্রন্থি == রিক্ত) {
        pthread_mutex_unlock(&s_fd_state_lock);
        প্রত্যাগম false;
    }
    *flags = গ্রন্থি->user_flags;
    pthread_mutex_unlock(&s_fd_state_lock);
    প্রত্যাগম true;
}

bool linux_coop_fd_user_nonblocking(পূর্ণাঙ্ক fd)
{
    পূর্ণাঙ্ক user_flags = 0;
    যদি (linux_coop_get_user_flags(fd, &user_flags)) {
        প্রত্যাগম (user_flags & O_NONBLOCK) != 0;
    }
    প্রত্যাগম false;
}


ssize_t freertos_linux_coop_read(পূর্ণাঙ্ক fd, শূন্য *buf, size_t গুনতি)
{
    LINUX_COOP_IO_LOOP(fd, real_read(fd, buf, গুনতি))
}

ssize_t freertos_linux_coop_write(পূর্ণাঙ্ক fd, ধ্রুব শূন্য *buf, size_t গুনতি)
{
    LINUX_COOP_IO_LOOP(fd, real_write(fd, buf, গুনতি))
}

ssize_t freertos_linux_coop_pread(পূর্ণাঙ্ক fd, শূন্য *buf, size_t গুনতি, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pread(fd, buf, গুনতি, offset))
}

ssize_t freertos_linux_coop_pwrite(পূর্ণাঙ্ক fd, ধ্রুব শূন্য *buf, size_t গুনতি, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pwrite(fd, buf, গুনতি, offset))
}

পূর্ণাঙ্ক freertos_linux_coop_open(ধ্রুব বর্ণ *path, পূর্ণাঙ্ক flags, পূর্ণাঙ্ক mode)
{
    পূর্ণাঙ্ক fd = real_open(path, flags, (mode_t)mode);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, flags & O_NONBLOCK);
    প্রত্যাগম fd;
}

পূর্ণাঙ্ক freertos_linux_coop_close(পূর্ণাঙ্ক fd)
{
    পূর্ণাঙ্ক ret = real_close(fd);
    যদি (ret == 0 || errno == EINTR) {
        /* On Linux, close() always releases the fd even when returning
         * EINTR.  Retrying would risk closing a fd reused by another
         * thread.  Treat EINTR as success. */
        linux_coop_untrack_fd(fd);
        প্রত্যাগম 0;
    }
    প্রত্যাগম -1;
}

পূর্ণাঙ্ক freertos_linux_coop_fcntl(পূর্ণাঙ্ক fd, পূর্ণাঙ্ক cmd, পূর্ণাঙ্ক arg)
{
    যদি (cmd == F_GETFL) {
        পূর্ণাঙ্ক real_flags = real_fcntl(fd, F_GETFL, 0);
        যদি (real_flags < 0) {
            প্রত্যাগম -1;
        }
        পূর্ণাঙ্ক user_flags = 0;
        যদি (linux_coop_get_user_flags(fd, &user_flags)) {
            real_flags &= ~O_NONBLOCK;
            real_flags |= (user_flags & O_NONBLOCK);
        }
        প্রত্যাগম real_flags;
    }
    যদি (cmd == F_SETFL) {
        linux_coop_set_user_flags(fd, arg);
        পূর্ণাঙ্ক internal_flags = arg | O_NONBLOCK;
        প্রত্যাগম real_fcntl(fd, F_SETFL, internal_flags);
    }
    প্রত্যাগম real_fcntl(fd, cmd, arg);
}

পূর্ণাঙ্ক freertos_linux_coop_select(পূর্ণাঙ্ক nfds, fd_set *readfds, fd_set *writefds,
                      fd_set *exceptfds, সংরচনা timeval *timeout)
{
    long timeout_ms = -1;
    যদি (timeout != রিক্ত) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_usec + 999) / 1000;
        যদি (timeout_ms == 0) {
            প্রত্যাগম real_select(nfds, readfds, writefds, exceptfds, timeout);
        }
    }

    /* real_select() mutates the fd_sets in place (clears them, then sets bits
     * for ready descriptors). To keep polling the same descriptors across
     * iterations we must snapshot the caller's sets and restore them on every
     * loop. */
    fd_set rfds_in, wfds_in, efds_in;
    যদি (readfds != রিক্ত) {
        rfds_in = *readfds;
    }
    যদি (writefds != রিক্ত) {
        wfds_in = *writefds;
    }
    যদি (exceptfds != রিক্ত) {
        efds_in = *exceptfds;
    }

    long waited_ms = 0;
    যাবত্ (1) {
        যদি (readfds != রিক্ত) {
            *readfds = rfds_in;
        }
        যদি (writefds != রিক্ত) {
            *writefds = wfds_in;
        }
        যদি (exceptfds != রিক্ত) {
            *exceptfds = efds_in;
        }

        সংরচনা timeval zero_tv = {0, 0};
        পূর্ণাঙ্ক ret = real_select(nfds, readfds, writefds, exceptfds, &zero_tv);
        যদি (ret != 0) {
            প্রত্যাগম ret;
        }
        যদি (timeout_ms == 0) {
            প্রত্যাগম 0;
        }
        যদি (timeout_ms > 0 && waited_ms >= timeout_ms) {
            প্রত্যাগম 0;
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

__attribute__((weak)) ssize_t পঠ(পূর্ণাঙ্ক fd, শূন্য *buf, size_t গুনতি)
{
    প্রত্যাগম freertos_linux_coop_read(fd, buf, গুনতি);
}

__attribute__((weak)) ssize_t লিখ(পূর্ণাঙ্ক fd, ধ্রুব শূন্য *buf, size_t গুনতি)
{
    প্রত্যাগম freertos_linux_coop_write(fd, buf, গুনতি);
}

__attribute__((weak)) ssize_t pread(পূর্ণাঙ্ক fd, শূন্য *buf, size_t গুনতি, off_t offset)
{
    প্রত্যাগম freertos_linux_coop_pread(fd, buf, গুনতি, offset);
}

__attribute__((weak)) ssize_t pwrite(পূর্ণাঙ্ক fd, ধ্রুব শূন্য *buf, size_t গুনতি, off_t offset)
{
    প্রত্যাগম freertos_linux_coop_pwrite(fd, buf, গুনতি, offset);
}

__attribute__((weak)) পূর্ণাঙ্ক open(ধ্রুব বর্ণ *path, পূর্ণাঙ্ক flags, ...)
{
    পূর্ণাঙ্ক mode = 0;
    যদি (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        mode = va_arg(ap, পূর্ণাঙ্ক);
        va_end(ap);
    }
    প্রত্যাগম freertos_linux_coop_open(path, flags, mode);
}

__attribute__((weak)) পূর্ণাঙ্ক close(পূর্ণাঙ্ক fd)
{
    প্রত্যাগম freertos_linux_coop_close(fd);
}

__attribute__((weak)) পূর্ণাঙ্ক fcntl(পূর্ণাঙ্ক fd, পূর্ণাঙ্ক cmd, ...)
{
    পূর্ণাঙ্ক arg = 0;
    যদি (cmd != F_GETFL) {
        va_list list;
        va_start(list, cmd);
        arg = va_arg(list, পূর্ণাঙ্ক);
        va_end(list);
    }
    প্রত্যাগম freertos_linux_coop_fcntl(fd, cmd, arg);
}

__attribute__((weak)) পূর্ণাঙ্ক select(পূর্ণাঙ্ক nfds, fd_set *readfds, fd_set *writefds,
                                 fd_set *exceptfds, সংরচনা timeval *timeout)
{
    প্রত্যাগম freertos_linux_coop_select(nfds, readfds, writefds, exceptfds, timeout);
}

ssize_t freertos_linux_coop_readv(পূর্ণাঙ্ক fd, ধ্রুব সংরচনা iovec *iov, পূর্ণাঙ্ক iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_readv(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_writev(পূর্ণাঙ্ক fd, ধ্রুব সংরচনা iovec *iov, পূর্ণাঙ্ক iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_writev(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_recv(পূর্ণাঙ্ক sockfd, শূন্য *buf, size_t len, পূর্ণাঙ্ক flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recv(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_send(পূর্ণাঙ্ক sockfd, ধ্রুব শূন্য *buf, size_t len, পূর্ণাঙ্ক flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_send(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_recvfrom(পূর্ণাঙ্ক sockfd, শূন্য *buf, size_t len, পূর্ণাঙ্ক flags,
                 সংরচনা sockaddr *src_addr, socklen_t *addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvfrom(sockfd, buf, len, flags | MSG_DONTWAIT,
                            src_addr, addrlen))
}

ssize_t freertos_linux_coop_sendto(পূর্ণাঙ্ক sockfd, ধ্রুব শূন্য *buf, size_t len, পূর্ণাঙ্ক flags,
               ধ্রুব সংরচনা sockaddr *dest_addr, socklen_t addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendto(sockfd, buf, len, flags | MSG_DONTWAIT,
                          dest_addr, addrlen))
}

ssize_t freertos_linux_coop_recvmsg(পূর্ণাঙ্ক sockfd, সংরচনা msghdr *বার্তা, পূর্ণাঙ্ক flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvmsg(sockfd, বার্তা, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_sendmsg(পূর্ণাঙ্ক sockfd, ধ্রুব সংরচনা msghdr *বার্তা, পূর্ণাঙ্ক flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendmsg(sockfd, বার্তা, flags | MSG_DONTWAIT))
}

__attribute__((weak)) ssize_t readv(পূর্ণাঙ্ক fd, ধ্রুব সংরচনা iovec *iov, পূর্ণাঙ্ক iovcnt)
{
    প্রত্যাগম freertos_linux_coop_readv(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t writev(পূর্ণাঙ্ক fd, ধ্রুব সংরচনা iovec *iov, পূর্ণাঙ্ক iovcnt)
{
    প্রত্যাগম freertos_linux_coop_writev(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t recv(পূর্ণাঙ্ক sockfd, শূন্য *buf, size_t len, পূর্ণাঙ্ক flags)
{
    প্রত্যাগম freertos_linux_coop_recv(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t send(পূর্ণাঙ্ক sockfd, ধ্রুব শূন্য *buf, size_t len, পূর্ণাঙ্ক flags)
{
    প্রত্যাগম freertos_linux_coop_send(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t recvfrom(পূর্ণাঙ্ক sockfd, শূন্য *buf, size_t len, পূর্ণাঙ্ক flags,
                 সংরচনা sockaddr *src_addr, socklen_t *addrlen)
{
    প্রত্যাগম freertos_linux_coop_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

__attribute__((weak)) ssize_t sendto(পূর্ণাঙ্ক sockfd, ধ্রুব শূন্য *buf, size_t len, পূর্ণাঙ্ক flags,
               ধ্রুব সংরচনা sockaddr *dest_addr, socklen_t addrlen)
{
    প্রত্যাগম freertos_linux_coop_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

__attribute__((weak)) ssize_t recvmsg(পূর্ণাঙ্ক sockfd, সংরচনা msghdr *বার্তা, পূর্ণাঙ্ক flags)
{
    প্রত্যাগম freertos_linux_coop_recvmsg(sockfd, বার্তা, flags);
}

__attribute__((weak)) ssize_t sendmsg(পূর্ণাঙ্ক sockfd, ধ্রুব সংরচনা msghdr *বার্তা, পূর্ণাঙ্ক flags)
{
    প্রত্যাগম freertos_linux_coop_sendmsg(sockfd, বার্তা, flags);
}

পূর্ণাঙ্ক freertos_linux_coop_connect(পূর্ণাঙ্ক sockfd, ধ্রুব সংরচনা sockaddr *addr, socklen_t addrlen)
{
    যাবত্ (1)
    {
        পূর্ণাঙ্ক ret = real_connect(sockfd, addr, addrlen);
        যদি (ret == 0) {
            প্রত্যাগম ret;
        }

        যদি (errno == EINPROGRESS || errno == EALREADY) {
            সংরচনা pollfd pfd;
            pfd.fd = sockfd;
            pfd.events = POLLOUT;
            pfd.revents = 0;

            পূর্ণাঙ্ক poll_retries = 0;
            যাবত্ (1)
            {
                পূর্ণাঙ্ক press = real_poll(&pfd, 1, 0);
                যদি (press > 0) {
                    পূর্ণাঙ্ক so_err = 0;
                    socklen_t len = পরিমাণ(so_err);
                    যদি (getsockopt(sockfd, SOL_SOCKET, SO_ERROR,
                                   &so_err, &len) < 0) {
                        প্রত্যাগম -1;
                    }
                    যদি (so_err == 0) {
                        প্রত্যাগম 0;
                    } অন্যথা {
                        errno = so_err;
                        প্রত্যাগম -1;
                    }
                } অন্যথা যদি (press == 0) {
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    অনুবর্ত;
                } অন্যথা {
                    যদি (errno == EINTR) {
                        অনুবর্ত;
                    }
                    /* Non-EINTR poll error; bail out after a few retries */
                    যদি (++poll_retries > 3) {
                        প্রত্যাগম -1;
                    }
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    অনুবর্ত;
                }
            }
        }

        যদি (errno == EINTR) {
            প্রত্যাগম -1;
        }

        প্রত্যাগম -1;
    }
}

পূর্ণাঙ্ক freertos_linux_coop_accept(পূর্ণাঙ্ক sockfd, সংরচনা sockaddr *addr, socklen_t *addrlen)
{
    যাবত্ (1)
    {
        পূর্ণাঙ্ক fd = real_accept(sockfd, addr, addrlen);
        যদি (fd >= 0) {
            linux_coop_set_nonblocking(fd);
            linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(sockfd) ? O_NONBLOCK : 0);
            প্রত্যাগম fd;
        }
        যদি (errno == EAGAIN || errno == EWOULDBLOCK) {
            যদি (linux_coop_fd_user_nonblocking(sockfd)) {
                প্রত্যাগম -1;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            অনুবর্ত;
        }
        প্রত্যাগম -1;
    }
}

পূর্ণাঙ্ক freertos_linux_coop_pselect(পূর্ণাঙ্ক nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, ধ্রুব সংরচনা timespec *timeout,
            ধ্রুব sigset_t *sigmask)
{
    long timeout_ms = -1;
    যদি (timeout != রিক্ত) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_nsec + 999999) / 1000000;
        যদি (timeout_ms == 0) {
            প্রত্যাগম real_pselect(nfds, readfds, writefds, exceptfds,
                                timeout, sigmask);
        }
    }

    long waited_ms = 0;
    যাবত্ (1)
    {
        সংরচনা timespec zero_ts = {0, 0};
        পূর্ণাঙ্ক ret = real_pselect(nfds, readfds, writefds, exceptfds,
                               &zero_ts, sigmask);
        যদি (ret != 0) {
            প্রত্যাগম ret;
        }

        যদি (timeout_ms == 0 ||
            (timeout_ms > 0 && waited_ms >= timeout_ms)) {
            প্রত্যাগম 0;
        }

        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

পূর্ণাঙ্ক freertos_linux_coop_poll(সংরচনা pollfd *fds, nfds_t nfds, পূর্ণাঙ্ক timeout)
{
    যদি (timeout == 0) {
        প্রত্যাগম real_poll(fds, nfds, 0);
    }

    long timeout_ms = -1;
    যদি (timeout > 0) {
        timeout_ms = timeout;
    }

    long waited_ms = 0;

    যাবত্ (1)
    {
        পূর্ণাঙ্ক ret = real_poll(fds, nfds, 0);
        যদি (ret > 0) {
            প্রত্যাগম ret;
        } অন্যথা যদি (ret == 0) {
            যদি (timeout_ms == 0 ||
                (timeout_ms > 0 && waited_ms >= timeout_ms)) {
                প্রত্যাগম 0;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            waited_ms += LINUX_COOP_TICK_MS;
            অনুবর্ত;
        } অন্যথা {
            যদি (errno == EINTR) {
                অনুবর্ত;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            যদি (timeout_ms > 0 && waited_ms >= timeout_ms) {
                প্রত্যাগম -1;
            }
            waited_ms += LINUX_COOP_TICK_MS;
        }
    }
}

পূর্ণাঙ্ক nanosleep(ধ্রুব সংরচনা timespec *req, সংরচনা timespec *rem)
{
    যদি (req == রিক্ত) {
        errno = EFAULT;
        প্রত্যাগম -1;
    }
    যদি (!linux_port_in_freertos_task()) {
        প্রত্যাগম real_nanosleep(req, rem);
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
    প্রত্যাগম 0;
}

unsigned পূর্ণাঙ্ক sleep(unsigned পূর্ণাঙ্ক seconds)
{
    linux_coop_yield(seconds * 1000);
    প্রত্যাগম 0;
}

পূর্ণাঙ্ক usleep(useconds_t usec)
{
    linux_coop_yield(usec / 1000);
    প্রত্যাগম 0;
}

পূর্ণাঙ্ক freertos_linux_coop_socket(পূর্ণাঙ্ক domain, পূর্ণাঙ্ক type, পূর্ণাঙ্ক protocol)
{
    পূর্ণাঙ্ক fd = real_socket(domain, type, protocol);
    linux_coop_set_nonblocking(fd);
#ifdef SOCK_NONBLOCK
    ধ্রুব পূর্ণাঙ্ক user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#অন্যথা
    ধ্রুব পূর্ণাঙ্ক user_flags = 0;
#endif
    linux_coop_track_fd(fd, user_flags);
    প্রত্যাগম fd;
}

পূর্ণাঙ্ক freertos_linux_coop_socketpair(পূর্ণাঙ্ক domain, পূর্ণাঙ্ক type, পূর্ণাঙ্ক protocol, পূর্ণাঙ্ক *sv)
{
    পূর্ণাঙ্ক ret = real_socketpair(domain, type, protocol, sv);
    যদি (ret == 0) {
        linux_coop_set_nonblocking(sv[0]);
        linux_coop_set_nonblocking(sv[1]);
#ifdef SOCK_NONBLOCK
        ধ্রুব পূর্ণাঙ্ক user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#অন্যথা
        ধ্রুব পূর্ণাঙ্ক user_flags = 0;
#endif
        linux_coop_track_fd(sv[0], user_flags);
        linux_coop_track_fd(sv[1], user_flags);
    }
    প্রত্যাগম ret;
}

পূর্ণাঙ্ক freertos_linux_coop_pipe(পূর্ণাঙ্ক *fds)
{
    পূর্ণাঙ্ক ret = real_pipe(fds);
    যদি (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        linux_coop_track_fd(fds[0], 0);
        linux_coop_track_fd(fds[1], 0);
    }
    প্রত্যাগম ret;
}

পূর্ণাঙ্ক freertos_linux_coop_pipe2(পূর্ণাঙ্ক *fds, পূর্ণাঙ্ক flags)
{
    পূর্ণাঙ্ক ret = real_pipe2(fds, flags);
    যদি (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        ধ্রুব পূর্ণাঙ্ক user_flags = (flags & O_NONBLOCK) ? O_NONBLOCK : 0;
        linux_coop_track_fd(fds[0], user_flags);
        linux_coop_track_fd(fds[1], user_flags);
    }
    প্রত্যাগম ret;
}

পূর্ণাঙ্ক freertos_linux_coop_dup(পূর্ণাঙ্ক oldfd)
{
    পূর্ণাঙ্ক fd = real_dup(oldfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    প্রত্যাগম fd;
}

পূর্ণাঙ্ক freertos_linux_coop_dup2(পূর্ণাঙ্ক oldfd, পূর্ণাঙ্ক newfd)
{
    পূর্ণাঙ্ক fd = real_dup2(oldfd, newfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    প্রত্যাগম fd;
}

__attribute__((weak)) পূর্ণাঙ্ক connect(পূর্ণাঙ্ক sockfd, ধ্রুব সংরচনা sockaddr *addr, socklen_t addrlen)
{
    প্রত্যাগম freertos_linux_coop_connect(sockfd, addr, addrlen);
}

__attribute__((weak)) পূর্ণাঙ্ক accept(পূর্ণাঙ্ক sockfd, সংরচনা sockaddr *addr, socklen_t *addrlen)
{
    প্রত্যাগম freertos_linux_coop_accept(sockfd, addr, addrlen);
}

__attribute__((weak)) পূর্ণাঙ্ক pselect(পূর্ণাঙ্ক nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, ধ্রুব সংরচনা timespec *timeout,
            ধ্রুব sigset_t *sigmask)
{
    প্রত্যাগম freertos_linux_coop_pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}

__attribute__((weak)) পূর্ণাঙ্ক poll(সংরচনা pollfd *fds, nfds_t nfds, পূর্ণাঙ্ক timeout)
{
    প্রত্যাগম freertos_linux_coop_poll(fds, nfds, timeout);
}

__attribute__((weak)) পূর্ণাঙ্ক socket(পূর্ণাঙ্ক domain, পূর্ণাঙ্ক type, পূর্ণাঙ্ক protocol)
{
    প্রত্যাগম freertos_linux_coop_socket(domain, type, protocol);
}

__attribute__((weak)) পূর্ণাঙ্ক socketpair(পূর্ণাঙ্ক domain, পূর্ণাঙ্ক type, পূর্ণাঙ্ক protocol, পূর্ণাঙ্ক sv[2])
{
    প্রত্যাগম freertos_linux_coop_socketpair(domain, type, protocol, sv);
}

__attribute__((weak)) পূর্ণাঙ্ক pipe(পূর্ণাঙ্ক fds[2])
{
    প্রত্যাগম freertos_linux_coop_pipe(fds);
}

__attribute__((weak)) পূর্ণাঙ্ক pipe2(পূর্ণাঙ্ক fds[2], পূর্ণাঙ্ক flags)
{
    প্রত্যাগম freertos_linux_coop_pipe2(fds, flags);
}

__attribute__((weak)) পূর্ণাঙ্ক dup(পূর্ণাঙ্ক oldfd)
{
    প্রত্যাগম freertos_linux_coop_dup(oldfd);
}

__attribute__((weak)) পূর্ণাঙ্ক dup2(পূর্ণাঙ্ক oldfd, পূর্ণাঙ্ক newfd)
{
    প্রত্যাগম freertos_linux_coop_dup2(oldfd, newfd);
}
