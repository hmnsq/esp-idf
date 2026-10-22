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

#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <errno.h>
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <stdarg.h>
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <sys/uio.h>
#અન્તર્ભાવ <sys/socket.h>
#અન્તર્ભાવ <sys/select.h>
#અન્તર્ભાવ <sys/time.h>
#અન્તર્ભાવ <time.h>
#અન્તર્ભાવ <poll.h>
#અન્તર્ભાવ <dlfcn.h>
#અન્તર્ભાવ <pthread.h>

#અન્તર્ભાવ "linux_port_coop_internal.h"
#અન્તર્ભાવ "esp_private/freertos_linux_coop_syscalls.h"

સ્થિર ssize_t  (*real_readv)(પૂર્ણાંક, અચલ સંરચના iovec *, પૂર્ણાંક);
સ્થિર ssize_t  (*real_writev)(પૂર્ણાંક, અચલ સંરચના iovec *, પૂર્ણાંક);
સ્થિર ssize_t  (*real_recv)(પૂર્ણાંક, શૂન્ય *, size_t, પૂર્ણાંક);
સ્થિર ssize_t  (*real_send)(પૂર્ણાંક, અચલ શૂન્ય *, size_t, પૂર્ણાંક);
સ્થિર ssize_t  (*real_recvfrom)(પૂર્ણાંક, શૂન્ય *, size_t, પૂર્ણાંક,
                                 સંરચના sockaddr *, socklen_t *);
સ્થિર ssize_t  (*real_sendto)(પૂર્ણાંક, અચલ શૂન્ય *, size_t, પૂર્ણાંક,
                                અચલ સંરચના sockaddr *, socklen_t);
સ્થિર ssize_t  (*real_recvmsg)(પૂર્ણાંક, સંરચના msghdr *, પૂર્ણાંક);
સ્થિર ssize_t  (*real_sendmsg)(પૂર્ણાંક, અચલ સંરચના msghdr *, પૂર્ણાંક);
સ્થિર પૂર્ણાંક      (*real_connect)(પૂર્ણાંક, અચલ સંરચના sockaddr *, socklen_t);
સ્થિર પૂર્ણાંક      (*real_accept)(પૂર્ણાંક, સંરચના sockaddr *, socklen_t *);
સ્થિર પૂર્ણાંક      (*real_pselect)(પૂર્ણાંક, fd_set *, fd_set *, fd_set *,
                                અચલ સંરચના timespec *, અચલ sigset_t *);
સ્થિર પૂર્ણાંક      (*real_poll)(સંરચના pollfd *, nfds_t, પૂર્ણાંક);
સ્થિર પૂર્ણાંક      (*real_socket)(પૂર્ણાંક, પૂર્ણાંક, પૂર્ણાંક);
સ્થિર પૂર્ણાંક      (*real_socketpair)(પૂર્ણાંક, પૂર્ણાંક, પૂર્ણાંક, પૂર્ણાંક[2]);
સ્થિર પૂર્ણાંક      (*real_pipe)(પૂર્ણાંક[2]);
સ્થિર પૂર્ણાંક      (*real_pipe2)(પૂર્ણાંક[2], પૂર્ણાંક);
સ્થિર પૂર્ણાંક      (*real_dup)(પૂર્ણાંક);
સ્થિર પૂર્ણાંક      (*real_dup2)(પૂર્ણાંક, પૂર્ણાંક);
સ્થિર પૂર્ણાંક      (*real_nanosleep)(અચલ સંરચના timespec *, સંરચના timespec *);
સ્થિર ssize_t  (*real_read)(પૂર્ણાંક, શૂન્ય *, size_t);
સ્થિર ssize_t  (*real_write)(પૂર્ણાંક, અચલ શૂન્ય *, size_t);
સ્થિર ssize_t  (*real_pread)(પૂર્ણાંક, શૂન્ય *, size_t, off_t);
સ્થિર ssize_t  (*real_pwrite)(પૂર્ણાંક, અચલ શૂન્ય *, size_t, off_t);
સ્થિર પૂર્ણાંક      (*real_close)(પૂર્ણાંક);
સ્થિર પૂર્ણાંક      (*real_select)(પૂર્ણાંક, fd_set *, fd_set *, fd_set *,
                                સંરચના timeval *);

પ્રકારવ્યાખ્યા પૂર્ણાંક (*linux_coop_fcntl_fn_t)(પૂર્ણાંક, પૂર્ણાંક, ...);
સ્થિર linux_coop_fcntl_fn_t real_fcntl;

પ્રકારવ્યાખ્યા પૂર્ણાંક (*linux_coop_open_fn_t)(અચલ અક્ષર *, પૂર્ણાંક, mode_t);
સ્થિર linux_coop_open_fn_t real_open;

પ્રકારવ્યાખ્યા સંરચના linux_coop_fd_state {
    પૂર્ણાંક fd;
    પૂર્ણાંક user_flags;
    સંરચના linux_coop_fd_state *next;
} linux_coop_fd_state_t;

સ્થિર linux_coop_fd_state_t *s_fd_state_list;
સ્થિર pthread_mutex_t s_fd_state_lock = PTHREAD_MUTEX_INITIALIZER;

સ્થિર linux_coop_fd_state_t *linux_coop_find_fd_locked(પૂર્ણાંક fd)
{
    linux_coop_fd_state_t *ગાંઠ = s_fd_state_list;
    જ્યારે (ગાંઠ != ખાલી) {
        જો (ગાંઠ->fd == fd) {
            પાછા ગાંઠ;
        }
        ગાંઠ = ગાંઠ->next;
    }
    પાછા ખાલી;
}

સ્થિર શૂન્ય __attribute__((constructor)) linux_coop_resolve_all(શૂન્ય)
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
    LINUX_COOP_RESOLVE(પઠ);
    LINUX_COOP_RESOLVE(લિખ);
    LINUX_COOP_RESOLVE(pread);
    LINUX_COOP_RESOLVE(pwrite);
    LINUX_COOP_RESOLVE(close);
    LINUX_COOP_RESOLVE(select);
    LINUX_COOP_RESOLVE(open);
}

શૂન્ય linux_coop_set_nonblocking(પૂર્ણાંક fd)
{
    જો (fd >= 0) {
        પૂર્ણાંક flags = real_fcntl(fd, F_GETFL, 0);
        જો (flags >= 0) {
            real_fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        }
    }
}

શૂન્ય linux_coop_track_fd(પૂર્ણાંક fd, પૂર્ણાંક user_flags)
{
    જો (fd < 0) {
        પાછા;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *ગાંઠ = linux_coop_find_fd_locked(fd);
    જો (ગાંઠ != ખાલી) {
        ગાંઠ->user_flags = user_flags;
        pthread_mutex_unlock(&s_fd_state_lock);
        પાછા;
    }

    ગાંઠ = calloc(1, કદ(*ગાંઠ));
    જો (ગાંઠ != ખાલી) {
        ગાંઠ->fd = fd;
        ગાંઠ->user_flags = user_flags;
        ગાંઠ->next = s_fd_state_list;
        s_fd_state_list = ગાંઠ;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

શૂન્ય linux_coop_untrack_fd(પૂર્ણાંક fd)
{
    જો (fd < 0) {
        પાછા;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *prev = ખાલી;
    linux_coop_fd_state_t *ગાંઠ = s_fd_state_list;
    જ્યારે (ગાંઠ != ખાલી) {
        જો (ગાંઠ->fd == fd) {
            જો (prev == ખાલી) {
                s_fd_state_list = ગાંઠ->next;
            } નહીંતર {
                prev->next = ગાંઠ->next;
            }
            મુક્ત(ગાંઠ);
            તોડો;
        }
        prev = ગાંઠ;
        ગાંઠ = ગાંઠ->next;
    }
    pthread_mutex_unlock(&s_fd_state_lock);
}

શૂન્ય linux_coop_set_user_flags(પૂર્ણાંક fd, પૂર્ણાંક user_flags)
{
    linux_coop_track_fd(fd, user_flags);
}

bool linux_coop_get_user_flags(પૂર્ણાંક fd, પૂર્ણાંક *flags)
{
    જો (fd < 0 || flags == ખાલી) {
        પાછા false;
    }

    pthread_mutex_lock(&s_fd_state_lock);
    linux_coop_fd_state_t *ગાંઠ = linux_coop_find_fd_locked(fd);
    જો (ગાંઠ == ખાલી) {
        pthread_mutex_unlock(&s_fd_state_lock);
        પાછા false;
    }
    *flags = ગાંઠ->user_flags;
    pthread_mutex_unlock(&s_fd_state_lock);
    પાછા true;
}

bool linux_coop_fd_user_nonblocking(પૂર્ણાંક fd)
{
    પૂર્ણાંક user_flags = 0;
    જો (linux_coop_get_user_flags(fd, &user_flags)) {
        પાછા (user_flags & O_NONBLOCK) != 0;
    }
    પાછા false;
}


ssize_t freertos_linux_coop_read(પૂર્ણાંક fd, શૂન્ય *buf, size_t ગણતરી)
{
    LINUX_COOP_IO_LOOP(fd, real_read(fd, buf, ગણતરી))
}

ssize_t freertos_linux_coop_write(પૂર્ણાંક fd, અચલ શૂન્ય *buf, size_t ગણતરી)
{
    LINUX_COOP_IO_LOOP(fd, real_write(fd, buf, ગણતરી))
}

ssize_t freertos_linux_coop_pread(પૂર્ણાંક fd, શૂન્ય *buf, size_t ગણતરી, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pread(fd, buf, ગણતરી, offset))
}

ssize_t freertos_linux_coop_pwrite(પૂર્ણાંક fd, અચલ શૂન્ય *buf, size_t ગણતરી, off_t offset)
{
    LINUX_COOP_IO_LOOP(fd, real_pwrite(fd, buf, ગણતરી, offset))
}

પૂર્ણાંક freertos_linux_coop_open(અચલ અક્ષર *path, પૂર્ણાંક flags, પૂર્ણાંક mode)
{
    પૂર્ણાંક fd = real_open(path, flags, (mode_t)mode);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, flags & O_NONBLOCK);
    પાછા fd;
}

પૂર્ણાંક freertos_linux_coop_close(પૂર્ણાંક fd)
{
    પૂર્ણાંક ret = real_close(fd);
    જો (ret == 0 || errno == EINTR) {
        /* On Linux, close() always releases the fd even when returning
         * EINTR.  Retrying would risk closing a fd reused by another
         * thread.  Treat EINTR as success. */
        linux_coop_untrack_fd(fd);
        પાછા 0;
    }
    પાછા -1;
}

પૂર્ણાંક freertos_linux_coop_fcntl(પૂર્ણાંક fd, પૂર્ણાંક cmd, પૂર્ણાંક arg)
{
    જો (cmd == F_GETFL) {
        પૂર્ણાંક real_flags = real_fcntl(fd, F_GETFL, 0);
        જો (real_flags < 0) {
            પાછા -1;
        }
        પૂર્ણાંક user_flags = 0;
        જો (linux_coop_get_user_flags(fd, &user_flags)) {
            real_flags &= ~O_NONBLOCK;
            real_flags |= (user_flags & O_NONBLOCK);
        }
        પાછા real_flags;
    }
    જો (cmd == F_SETFL) {
        linux_coop_set_user_flags(fd, arg);
        પૂર્ણાંક internal_flags = arg | O_NONBLOCK;
        પાછા real_fcntl(fd, F_SETFL, internal_flags);
    }
    પાછા real_fcntl(fd, cmd, arg);
}

પૂર્ણાંક freertos_linux_coop_select(પૂર્ણાંક nfds, fd_set *readfds, fd_set *writefds,
                      fd_set *exceptfds, સંરચના timeval *timeout)
{
    long timeout_ms = -1;
    જો (timeout != ખાલી) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_usec + 999) / 1000;
        જો (timeout_ms == 0) {
            પાછા real_select(nfds, readfds, writefds, exceptfds, timeout);
        }
    }

    /* real_select() mutates the fd_sets in place (clears them, then sets bits
     * for ready descriptors). To keep polling the same descriptors across
     * iterations we must snapshot the caller's sets and restore them on every
     * loop. */
    fd_set rfds_in, wfds_in, efds_in;
    જો (readfds != ખાલી) {
        rfds_in = *readfds;
    }
    જો (writefds != ખાલી) {
        wfds_in = *writefds;
    }
    જો (exceptfds != ખાલી) {
        efds_in = *exceptfds;
    }

    long waited_ms = 0;
    જ્યારે (1) {
        જો (readfds != ખાલી) {
            *readfds = rfds_in;
        }
        જો (writefds != ખાલી) {
            *writefds = wfds_in;
        }
        જો (exceptfds != ખાલી) {
            *exceptfds = efds_in;
        }

        સંરચના timeval zero_tv = {0, 0};
        પૂર્ણાંક ret = real_select(nfds, readfds, writefds, exceptfds, &zero_tv);
        જો (ret != 0) {
            પાછા ret;
        }
        જો (timeout_ms == 0) {
            પાછા 0;
        }
        જો (timeout_ms > 0 && waited_ms >= timeout_ms) {
            પાછા 0;
        }
        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

શૂન્ય freertos_linux_coop_syscalls_init(શૂન્ય)
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

__attribute__((weak)) ssize_t પઠ(પૂર્ણાંક fd, શૂન્ય *buf, size_t ગણતરી)
{
    પાછા freertos_linux_coop_read(fd, buf, ગણતરી);
}

__attribute__((weak)) ssize_t લિખ(પૂર્ણાંક fd, અચલ શૂન્ય *buf, size_t ગણતરી)
{
    પાછા freertos_linux_coop_write(fd, buf, ગણતરી);
}

__attribute__((weak)) ssize_t pread(પૂર્ણાંક fd, શૂન્ય *buf, size_t ગણતરી, off_t offset)
{
    પાછા freertos_linux_coop_pread(fd, buf, ગણતરી, offset);
}

__attribute__((weak)) ssize_t pwrite(પૂર્ણાંક fd, અચલ શૂન્ય *buf, size_t ગણતરી, off_t offset)
{
    પાછા freertos_linux_coop_pwrite(fd, buf, ગણતરી, offset);
}

__attribute__((weak)) પૂર્ણાંક open(અચલ અક્ષર *path, પૂર્ણાંક flags, ...)
{
    પૂર્ણાંક mode = 0;
    જો (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        mode = va_arg(ap, પૂર્ણાંક);
        va_end(ap);
    }
    પાછા freertos_linux_coop_open(path, flags, mode);
}

__attribute__((weak)) પૂર્ણાંક close(પૂર્ણાંક fd)
{
    પાછા freertos_linux_coop_close(fd);
}

__attribute__((weak)) પૂર્ણાંક fcntl(પૂર્ણાંક fd, પૂર્ણાંક cmd, ...)
{
    પૂર્ણાંક arg = 0;
    જો (cmd != F_GETFL) {
        va_list list;
        va_start(list, cmd);
        arg = va_arg(list, પૂર્ણાંક);
        va_end(list);
    }
    પાછા freertos_linux_coop_fcntl(fd, cmd, arg);
}

__attribute__((weak)) પૂર્ણાંક select(પૂર્ણાંક nfds, fd_set *readfds, fd_set *writefds,
                                 fd_set *exceptfds, સંરચના timeval *timeout)
{
    પાછા freertos_linux_coop_select(nfds, readfds, writefds, exceptfds, timeout);
}

ssize_t freertos_linux_coop_readv(પૂર્ણાંક fd, અચલ સંરચના iovec *iov, પૂર્ણાંક iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_readv(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_writev(પૂર્ણાંક fd, અચલ સંરચના iovec *iov, પૂર્ણાંક iovcnt)
{
    LINUX_COOP_IO_LOOP(fd, real_writev(fd, iov, iovcnt))
}

ssize_t freertos_linux_coop_recv(પૂર્ણાંક sockfd, શૂન્ય *buf, size_t len, પૂર્ણાંક flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recv(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_send(પૂર્ણાંક sockfd, અચલ શૂન્ય *buf, size_t len, પૂર્ણાંક flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_send(sockfd, buf, len, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_recvfrom(પૂર્ણાંક sockfd, શૂન્ય *buf, size_t len, પૂર્ણાંક flags,
                 સંરચના sockaddr *src_addr, socklen_t *addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvfrom(sockfd, buf, len, flags | MSG_DONTWAIT,
                            src_addr, addrlen))
}

ssize_t freertos_linux_coop_sendto(પૂર્ણાંક sockfd, અચલ શૂન્ય *buf, size_t len, પૂર્ણાંક flags,
               અચલ સંરચના sockaddr *dest_addr, socklen_t addrlen)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendto(sockfd, buf, len, flags | MSG_DONTWAIT,
                          dest_addr, addrlen))
}

ssize_t freertos_linux_coop_recvmsg(પૂર્ણાંક sockfd, સંરચના msghdr *સંદેશ, પૂર્ણાંક flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_recvmsg(sockfd, સંદેશ, flags | MSG_DONTWAIT))
}

ssize_t freertos_linux_coop_sendmsg(પૂર્ણાંક sockfd, અચલ સંરચના msghdr *સંદેશ, પૂર્ણાંક flags)
{
    LINUX_COOP_IO_LOOP(sockfd, real_sendmsg(sockfd, સંદેશ, flags | MSG_DONTWAIT))
}

__attribute__((weak)) ssize_t readv(પૂર્ણાંક fd, અચલ સંરચના iovec *iov, પૂર્ણાંક iovcnt)
{
    પાછા freertos_linux_coop_readv(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t writev(પૂર્ણાંક fd, અચલ સંરચના iovec *iov, પૂર્ણાંક iovcnt)
{
    પાછા freertos_linux_coop_writev(fd, iov, iovcnt);
}

__attribute__((weak)) ssize_t recv(પૂર્ણાંક sockfd, શૂન્ય *buf, size_t len, પૂર્ણાંક flags)
{
    પાછા freertos_linux_coop_recv(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t send(પૂર્ણાંક sockfd, અચલ શૂન્ય *buf, size_t len, પૂર્ણાંક flags)
{
    પાછા freertos_linux_coop_send(sockfd, buf, len, flags);
}

__attribute__((weak)) ssize_t recvfrom(પૂર્ણાંક sockfd, શૂન્ય *buf, size_t len, પૂર્ણાંક flags,
                 સંરચના sockaddr *src_addr, socklen_t *addrlen)
{
    પાછા freertos_linux_coop_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}

__attribute__((weak)) ssize_t sendto(પૂર્ણાંક sockfd, અચલ શૂન્ય *buf, size_t len, પૂર્ણાંક flags,
               અચલ સંરચના sockaddr *dest_addr, socklen_t addrlen)
{
    પાછા freertos_linux_coop_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
}

__attribute__((weak)) ssize_t recvmsg(પૂર્ણાંક sockfd, સંરચના msghdr *સંદેશ, પૂર્ણાંક flags)
{
    પાછા freertos_linux_coop_recvmsg(sockfd, સંદેશ, flags);
}

__attribute__((weak)) ssize_t sendmsg(પૂર્ણાંક sockfd, અચલ સંરચના msghdr *સંદેશ, પૂર્ણાંક flags)
{
    પાછા freertos_linux_coop_sendmsg(sockfd, સંદેશ, flags);
}

પૂર્ણાંક freertos_linux_coop_connect(પૂર્ણાંક sockfd, અચલ સંરચના sockaddr *addr, socklen_t addrlen)
{
    જ્યારે (1)
    {
        પૂર્ણાંક ret = real_connect(sockfd, addr, addrlen);
        જો (ret == 0) {
            પાછા ret;
        }

        જો (errno == EINPROGRESS || errno == EALREADY) {
            સંરચના pollfd pfd;
            pfd.fd = sockfd;
            pfd.events = POLLOUT;
            pfd.revents = 0;

            પૂર્ણાંક poll_retries = 0;
            જ્યારે (1)
            {
                પૂર્ણાંક press = real_poll(&pfd, 1, 0);
                જો (press > 0) {
                    પૂર્ણાંક so_err = 0;
                    socklen_t len = કદ(so_err);
                    જો (getsockopt(sockfd, SOL_SOCKET, SO_ERROR,
                                   &so_err, &len) < 0) {
                        પાછા -1;
                    }
                    જો (so_err == 0) {
                        પાછા 0;
                    } નહીંતર {
                        errno = so_err;
                        પાછા -1;
                    }
                } નહીંતર જો (press == 0) {
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    ચાલુ;
                } નહીંતર {
                    જો (errno == EINTR) {
                        ચાલુ;
                    }
                    /* Non-EINTR poll error; bail out after a few retries */
                    જો (++poll_retries > 3) {
                        પાછા -1;
                    }
                    linux_coop_yield(LINUX_COOP_TICK_MS);
                    ચાલુ;
                }
            }
        }

        જો (errno == EINTR) {
            પાછા -1;
        }

        પાછા -1;
    }
}

પૂર્ણાંક freertos_linux_coop_accept(પૂર્ણાંક sockfd, સંરચના sockaddr *addr, socklen_t *addrlen)
{
    જ્યારે (1)
    {
        પૂર્ણાંક fd = real_accept(sockfd, addr, addrlen);
        જો (fd >= 0) {
            linux_coop_set_nonblocking(fd);
            linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(sockfd) ? O_NONBLOCK : 0);
            પાછા fd;
        }
        જો (errno == EAGAIN || errno == EWOULDBLOCK) {
            જો (linux_coop_fd_user_nonblocking(sockfd)) {
                પાછા -1;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            ચાલુ;
        }
        પાછા -1;
    }
}

પૂર્ણાંક freertos_linux_coop_pselect(પૂર્ણાંક nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, અચલ સંરચના timespec *timeout,
            અચલ sigset_t *sigmask)
{
    long timeout_ms = -1;
    જો (timeout != ખાલી) {
        timeout_ms = (long)timeout->tv_sec * 1000 +
                     (timeout->tv_nsec + 999999) / 1000000;
        જો (timeout_ms == 0) {
            પાછા real_pselect(nfds, readfds, writefds, exceptfds,
                                timeout, sigmask);
        }
    }

    long waited_ms = 0;
    જ્યારે (1)
    {
        સંરચના timespec zero_ts = {0, 0};
        પૂર્ણાંક ret = real_pselect(nfds, readfds, writefds, exceptfds,
                               &zero_ts, sigmask);
        જો (ret != 0) {
            પાછા ret;
        }

        જો (timeout_ms == 0 ||
            (timeout_ms > 0 && waited_ms >= timeout_ms)) {
            પાછા 0;
        }

        linux_coop_yield(LINUX_COOP_TICK_MS);
        waited_ms += LINUX_COOP_TICK_MS;
    }
}

પૂર્ણાંક freertos_linux_coop_poll(સંરચના pollfd *fds, nfds_t nfds, પૂર્ણાંક timeout)
{
    જો (timeout == 0) {
        પાછા real_poll(fds, nfds, 0);
    }

    long timeout_ms = -1;
    જો (timeout > 0) {
        timeout_ms = timeout;
    }

    long waited_ms = 0;

    જ્યારે (1)
    {
        પૂર્ણાંક ret = real_poll(fds, nfds, 0);
        જો (ret > 0) {
            પાછા ret;
        } નહીંતર જો (ret == 0) {
            જો (timeout_ms == 0 ||
                (timeout_ms > 0 && waited_ms >= timeout_ms)) {
                પાછા 0;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            waited_ms += LINUX_COOP_TICK_MS;
            ચાલુ;
        } નહીંતર {
            જો (errno == EINTR) {
                ચાલુ;
            }
            linux_coop_yield(LINUX_COOP_TICK_MS);
            જો (timeout_ms > 0 && waited_ms >= timeout_ms) {
                પાછા -1;
            }
            waited_ms += LINUX_COOP_TICK_MS;
        }
    }
}

પૂર્ણાંક nanosleep(અચલ સંરચના timespec *req, સંરચના timespec *rem)
{
    જો (req == ખાલી) {
        errno = EFAULT;
        પાછા -1;
    }
    જો (!linux_port_in_freertos_task()) {
        પાછા real_nanosleep(req, rem);
    }
    long ms = req->tv_sec * 1000 + (req->tv_nsec + 999999) / 1000000;
    જો (ms <= 0) {
        ms = 1;
    }
    vTaskDelay(ms);
    જો (rem) {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }
    પાછા 0;
}

unsigned પૂર્ણાંક sleep(unsigned પૂર્ણાંક seconds)
{
    linux_coop_yield(seconds * 1000);
    પાછા 0;
}

પૂર્ણાંક usleep(useconds_t usec)
{
    linux_coop_yield(usec / 1000);
    પાછા 0;
}

પૂર્ણાંક freertos_linux_coop_socket(પૂર્ણાંક domain, પૂર્ણાંક type, પૂર્ણાંક protocol)
{
    પૂર્ણાંક fd = real_socket(domain, type, protocol);
    linux_coop_set_nonblocking(fd);
#ifdef SOCK_NONBLOCK
    અચલ પૂર્ણાંક user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#નહીંતર
    અચલ પૂર્ણાંક user_flags = 0;
#endif
    linux_coop_track_fd(fd, user_flags);
    પાછા fd;
}

પૂર્ણાંક freertos_linux_coop_socketpair(પૂર્ણાંક domain, પૂર્ણાંક type, પૂર્ણાંક protocol, પૂર્ણાંક *sv)
{
    પૂર્ણાંક ret = real_socketpair(domain, type, protocol, sv);
    જો (ret == 0) {
        linux_coop_set_nonblocking(sv[0]);
        linux_coop_set_nonblocking(sv[1]);
#ifdef SOCK_NONBLOCK
        અચલ પૂર્ણાંક user_flags = (type & SOCK_NONBLOCK) ? O_NONBLOCK : 0;
#નહીંતર
        અચલ પૂર્ણાંક user_flags = 0;
#endif
        linux_coop_track_fd(sv[0], user_flags);
        linux_coop_track_fd(sv[1], user_flags);
    }
    પાછા ret;
}

પૂર્ણાંક freertos_linux_coop_pipe(પૂર્ણાંક *fds)
{
    પૂર્ણાંક ret = real_pipe(fds);
    જો (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        linux_coop_track_fd(fds[0], 0);
        linux_coop_track_fd(fds[1], 0);
    }
    પાછા ret;
}

પૂર્ણાંક freertos_linux_coop_pipe2(પૂર્ણાંક *fds, પૂર્ણાંક flags)
{
    પૂર્ણાંક ret = real_pipe2(fds, flags);
    જો (ret == 0) {
        linux_coop_set_nonblocking(fds[0]);
        linux_coop_set_nonblocking(fds[1]);
        અચલ પૂર્ણાંક user_flags = (flags & O_NONBLOCK) ? O_NONBLOCK : 0;
        linux_coop_track_fd(fds[0], user_flags);
        linux_coop_track_fd(fds[1], user_flags);
    }
    પાછા ret;
}

પૂર્ણાંક freertos_linux_coop_dup(પૂર્ણાંક oldfd)
{
    પૂર્ણાંક fd = real_dup(oldfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    પાછા fd;
}

પૂર્ણાંક freertos_linux_coop_dup2(પૂર્ણાંક oldfd, પૂર્ણાંક newfd)
{
    પૂર્ણાંક fd = real_dup2(oldfd, newfd);
    linux_coop_set_nonblocking(fd);
    linux_coop_track_fd(fd, linux_coop_fd_user_nonblocking(oldfd) ? O_NONBLOCK : 0);
    પાછા fd;
}

__attribute__((weak)) પૂર્ણાંક connect(પૂર્ણાંક sockfd, અચલ સંરચના sockaddr *addr, socklen_t addrlen)
{
    પાછા freertos_linux_coop_connect(sockfd, addr, addrlen);
}

__attribute__((weak)) પૂર્ણાંક accept(પૂર્ણાંક sockfd, સંરચના sockaddr *addr, socklen_t *addrlen)
{
    પાછા freertos_linux_coop_accept(sockfd, addr, addrlen);
}

__attribute__((weak)) પૂર્ણાંક pselect(પૂર્ણાંક nfds, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, અચલ સંરચના timespec *timeout,
            અચલ sigset_t *sigmask)
{
    પાછા freertos_linux_coop_pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}

__attribute__((weak)) પૂર્ણાંક poll(સંરચના pollfd *fds, nfds_t nfds, પૂર્ણાંક timeout)
{
    પાછા freertos_linux_coop_poll(fds, nfds, timeout);
}

__attribute__((weak)) પૂર્ણાંક socket(પૂર્ણાંક domain, પૂર્ણાંક type, પૂર્ણાંક protocol)
{
    પાછા freertos_linux_coop_socket(domain, type, protocol);
}

__attribute__((weak)) પૂર્ણાંક socketpair(પૂર્ણાંક domain, પૂર્ણાંક type, પૂર્ણાંક protocol, પૂર્ણાંક sv[2])
{
    પાછા freertos_linux_coop_socketpair(domain, type, protocol, sv);
}

__attribute__((weak)) પૂર્ણાંક pipe(પૂર્ણાંક fds[2])
{
    પાછા freertos_linux_coop_pipe(fds);
}

__attribute__((weak)) પૂર્ણાંક pipe2(પૂર્ણાંક fds[2], પૂર્ણાંક flags)
{
    પાછા freertos_linux_coop_pipe2(fds, flags);
}

__attribute__((weak)) પૂર્ણાંક dup(પૂર્ણાંક oldfd)
{
    પાછા freertos_linux_coop_dup(oldfd);
}

__attribute__((weak)) પૂર્ણાંક dup2(પૂર્ણાંક oldfd, પૂર્ણાંક newfd)
{
    પાછા freertos_linux_coop_dup2(oldfd, newfd);
}
